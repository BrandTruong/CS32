#include "StudentWorld.h"
//For displayText
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
//For actors + movement
#include "Actor.h"
#include <cmath> //for trig
#include <algorithm> //for min/max funcs

using namespace std;

const double PI = 4 * atan(1);

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    int level = getLevel();
    m_player = new Socrates(this); //separate pointer to Socrates
    //Other initial actors
    double x, y;
    int loop = 0;
    //Creates init actors in order, overlapFix assumes this order
    for (loop = 0; loop < level; loop++) {
        overlapFix(x, y);
        createActor(new Pit(x, y, this));
    }
    for (loop = 0; loop < min(5 * level, 25); loop++) {
        overlapFix(x, y);
        createActor(new Food(x, y, this));
    }
    for (loop = 0; loop < max(180 - 20 * level, 20); loop++) {
        overlapFix(x, y);
        createActor(new Dirt(x, y, this));
    }
    updateGameText(); //sets Game text at init
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    list<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) { //Goes through entire list
        if (!(*it)->isDead()) { //Does something if alive
            (*it)->doSomething();
            if (m_player->isDead()) { //Game ends immediately if player dies, decreases Lives
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    m_player->doSomething(); //Player does a move after all other Actors so Projectiles don't skip a frame (when created, doesn't move that tick)

    //re-iterate to check if anything has died and delete if so
    for (list<Actor*>::iterator checker = m_actors.begin(); checker != m_actors.end(); checker++) {
        if ((*checker)->isDead()) {
            delete *checker;
            checker = m_actors.erase(checker); 
            if (checker == m_actors.end()) { //finishes loop if deleted last one 
                break;
            }
            if (checker != m_actors.begin()) { //post-decrement so it checks next in list without skipping
                checker--;
            }
        }
    }
    //checks if bacteria or pits are still in game
    if (isFinishedLevel()) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    //Creates possible interactors
    createLifespanObjects();
    //Updates game text
    updateGameText();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //de-allocate Socrates
    delete m_player;
    m_player = nullptr;
    //de-allocates remaining Actors
    for ( list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
        delete *it;
    }
    m_actors.clear(); //clears entire list rather than erasing in loop
}


//Public Functions


bool StudentWorld::dealDamage(Projectile* ptr) //If overlaps a damageable object, tells affected object to take damage and return true
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
        if (abs((*it)->getX() - ptr->getX()) <= SPRITE_WIDTH && abs((*it)->getY() - ptr->getY()) <= SPRITE_WIDTH) {   //Checks to see if overlapping first, then checks to see if damageable
            if ((*it)->takeDamage(ptr->getDamage())) {
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::createActor(Actor* ptr) //allows storage of dynamically allocated Actors when called in different class
{
    m_actors.push_back(ptr);
}

bool StudentWorld::bacteriaEat(Bacteria* ptr) //If within object overlap between Bacteria and Food, then food is eaten and return true
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->isEdible()) { //If edible first, then if overlapping
            if (actorOverlap(*it, ptr)) {
                (*it)->setDead();
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::blocked(double x, double y) const //Returns if given coordinates are either blocked or out of bounds (petri dish constraint) 
{
    //determines if out of bounds
    double xSQD = (x - VIEW_WIDTH / 2) * (x - VIEW_WIDTH / 2);
    double ySQD = (y - VIEW_HEIGHT / 2) * (y - VIEW_HEIGHT / 2);
    double distance = sqrt(xSQD + ySQD);
    if (distance >= VIEW_RADIUS) {
        return true;
    }
    //determines if blocked by blockable object
    for (list<Actor*>::const_iterator it = m_actors.cbegin(); it != m_actors.cend(); it++) {
        if ((*it)->blocksBacteria()) { //blockable object
            if (abs((*it)->getX() - x) <= SPRITE_WIDTH / 2 && abs((*it)->getY() - y) <= SPRITE_WIDTH / 2) { //within movement block overlap
                return true;
            }
        }
    }
    return false; //nothing is blocking it from moving to location!
}

bool StudentWorld::actorOverlap(Actor* ptr1, Actor* ptr2) const //Returns if objects overlap
{
    if (abs(ptr1->getX() - ptr2->getX()) <= SPRITE_WIDTH && abs(ptr1->getY() - ptr2->getY()) <= SPRITE_WIDTH) { //Overlap
        return true;
    }
    else {
        return false;
    }
}

void StudentWorld::moveToFood(Bacteria* ptr) //Moves to closest food if within 128 pixels
{
    double minD = VIEW_RADIUS;
    Direction foodAngle = 0;
    bool food = false;
    for (list<Actor*>::const_iterator it = m_actors.cbegin(); it != m_actors.cend(); it++) {
        if ((*it)->isEdible()) { //Finds edible objects and sees if closer than previous distance
            Direction tempAngle = 0;
            double distance = distanceBetweenTwo(ptr, *it, tempAngle); 
            if (distance <= minD) {
                minD = distance;
                foodAngle = tempAngle;
                food = true;
            }
        }
    }
    double x, y;
    ptr->setDirection(foodAngle);
    ptr->getPositionInThisDirection(ptr->getDirection(), 3, x, y); //Finds position to move to
    if (!food || blocked(x, y)) { //If either blocked or no food found, sets to random direction
        ptr->setDirection(randInt(0, 359));
        ptr->resetMovement();
    }
    else { //Moves towards food otherwise
        ptr->moveForward(3);
    }
}

double StudentWorld::socratesDistance(Actor* ptr, Direction& dir) const //returns distance from Socrates
{
    return distanceBetweenTwo(ptr, m_player, dir);
}



//Private Helper Functions


double StudentWorld::distanceFromCenter(double x, double y) const //Calculates distance from center
{
    double xSQD = (x - VIEW_WIDTH / 2) * (x - VIEW_WIDTH / 2);
    double ySQD = (y - VIEW_HEIGHT / 2) * (y - VIEW_HEIGHT / 2);
    return sqrt(xSQD + ySQD);
}

double StudentWorld::distanceBetweenTwo(Actor* ptr1, Actor* ptr2, Direction& dir) const
{
    //ptr1 is the object to find direction of in comparison to ptr2, eg. finding direction for a Bacteria (1) towards Socrates (2)
    double x = ptr2->getX() - ptr1->getX();
    double y = ptr2->getY() - ptr1->getY();
    double distance = sqrt(x * x + y * y);
    double radians = atan2(y, x); // from [-PI, PI]
    if (radians < 0) { //Changes interval to [0, 2PI)
        radians = 2 * PI + radians;
    }
    dir = (Direction)(radians * 180 / PI); //Change from radians to degree
    return distance;
}

void StudentWorld::updateGameText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = m_player->getHealth();
    int sprayAmmo = m_player->getsAmmo();
    int flameAmmo = m_player->getfAmmo();
    ostringstream oss;
    //Level: 00000
    oss.fill('0');
    oss << "Score: " << setw(6) << score;
    oss.fill(' ');
    oss << setw(2) << "  " << "Level: " << level;
    oss << setw(2) << "  " << "Lives: " << lives;
    oss << setw(2) << "  " << "Health: " << health;
    oss << setw(2) << "  " << "Sprays: " << sprayAmmo;
    oss << setw(2) << "  " << "Flames: " << flameAmmo;
    setGameStatText(oss.str());
}

bool StudentWorld::isFinishedLevel() const
{
    list<Actor*>::const_iterator it;
    for (it = m_actors.cbegin(); it != m_actors.cend(); it++) {
        if (!(*it)->isDead() && (*it)->isEnemy()) { //alive bacteria or pit
            return false;
        }
    }
    return true; //everything do the die
}

void StudentWorld::createLifespanObjects()
{
    int level = getLevel();
    double x, y = 0;
    int randomAngle = randInt(0, 359);
    if (randInt(0, max(510 - level * 10, 200) - 1) == 0) { //chance of Fungus
        x = VIEW_RADIUS * cos(randomAngle * PI / 180) + VIEW_WIDTH / 2;
        y = VIEW_RADIUS * sin(randomAngle * PI / 180) + VIEW_HEIGHT / 2;
        m_actors.push_back(new Fungus(x, y, this));
    }
    randomAngle = randInt(0, 359); //gets another randomAngle so not overlapping
    if (randInt(0, max(510 - level * 10, 250) - 1) == 0) { //chance of Goodie
        x = VIEW_RADIUS * cos(randomAngle * PI / 180) + VIEW_WIDTH / 2;
        y = VIEW_RADIUS * sin(randomAngle * PI / 180) + VIEW_HEIGHT / 2;
        switch (randInt(0, 9)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            m_actors.push_back(new HealthGoodie(x, y, this));
            break;
        case 6:
        case 7:
        case 8:
            m_actors.push_back(new FlameGoodie(x, y, this));
            break;
        case 9:
            m_actors.push_back(new ExtraLifeGoodie(x, y, this));
            break;
        default:
            break; //Cases should always be from 0 to 9
        }
    }
}


//init fix

void StudentWorld::randDistance(double& x, double& y) const
{
    //determines random location 0-120 units away from center
    double radians = PI * randInt(0, 359) / 180;
    int units = randInt(0, 120) + randInt(0, 120); //makes more uniform rather than clustered, center point less likely but still possible
    if (units > 120) {
        units = 240 - units;
    }
    x = (VIEW_WIDTH / 2 + units * cos(radians));
    y = (VIEW_HEIGHT / 2 + units * sin(radians));
}

void StudentWorld::overlapFix(double& x, double& y) const //only called for init, actors list initialized in order of pit then food then dirt (assumption in order)
{
    bool overlap = true;
    while (overlap) {
        randDistance(x, y);
        list<Actor*>::const_iterator it;
        for (it = m_actors.cbegin(); it != m_actors.cend() && !(*it)->blocksBacteria(); it++) {
            if (abs(x - (*it)->getX()) <= SPRITE_WIDTH && abs(y - (*it)->getY()) <= SPRITE_WIDTH) {
                break;
            }
        }
        if (it == m_actors.cend() || (*it)->blocksBacteria()) { //reaches end or first dirt object in list without overlap
            overlap = false;
        }
    }
}