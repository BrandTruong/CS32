#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <string>
#include <algorithm>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor
Actor::Actor(int imageID, double startX, double startY, Direction startDirection, StudentWorld* myWorld, int depth) : GraphObject(imageID, startX, startY, startDirection, depth), m_dead(false), m_world(myWorld)
{
}

//Agent
Agent::Agent(int imageID, double x, double y, int dir, StudentWorld* m_world, int hitPoints) : Actor(imageID, x, y, dir, m_world)
{
	setHealth(hitPoints);
}

bool Agent::takeDamage(int amt)
{
	m_health -= amt; //decreases hp based on amt of damage
	if (m_health > 0) { //If still alive
		getWorld()->playSound(soundWhenHurt()); //hurt sound
	}
	else { //dies
		getWorld()->playSound(soundWhenDie()); //death sound
		setDead();
	}
	return true; //Agent has taken damage
}

//LifespanObjects

LifespanObjects::LifespanObjects(int imageID, double startX, double startY, StudentWorld* myWorld) : Actor(imageID, startX, startY, 0, myWorld, 1)
{
	m_lifetime = max(randInt(0, 300 - 10 * getWorld()->getLevel() - 1), 50); //Lifetime are all based on same idea
	//Scoremod init by Derived Goodies
}

bool LifespanObjects::overlapSocrates()
{
	return getWorld()->actorOverlap(this, getWorld()->playerPtr()); //returns whether the object overlaps with Socrates or not
}

void LifespanObjects::doSomething()
{
	if (isDead()) { //Doesn't do anything if dead
		return;
	}
	if (overlapSocrates()) { //Picked up by Socrates
		getWorld()->increaseScore(m_scoreMod);
		setDead();
		getSound();
		doSpecificFunc(); //Does something depending on what type picked up
		return;
	}
	lifespan(); //Decrement lifetime or dies if reached end
}

void LifespanObjects::getSound() //Plays default sound of Goodie got, Fungus overwrites to no sound
{
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

void LifespanObjects::lifespan() //checks lifespan and decreases or sets dead
{
	if (m_lifetime <= 0) { //Past its lifetime
		setDead();
	}
	else {
		m_lifetime--; //Still has a lifetime, decrements
	}
}

//Projectile implementations

Projectile::Projectile(int imageID, double startX, double startY, Direction startDirection, int startDistance, StudentWorld* myWorld) : Actor(imageID, startX, startY, startDirection, myWorld, 1), m_distance(startDistance)
{
}

void Projectile::doSomething()
{
	if (isDead()) { //Doesn't do anything if already dead
		return;
	}
	if (getWorld()->dealDamage(this)) { //returns true if damaged an object, false if haven't
		setDead(); //Only attacks once
		return;
	}
	moveForward(SPRITE_WIDTH); //If haven't damaged an object, then keep moving until no more travel distance
	m_distance -= SPRITE_WIDTH;
	if (m_distance <= 0) {
		setDead(); //can't travel anymore
	}
}

//Bacteria implementations

Bacteria::Bacteria(int imageID, double x, double y, int dealtDamage, int health, StudentWorld* myWorld) : Agent(imageID, x, y, 90, myWorld, health) 
{
	m_dealtDmg = dealtDamage;
	m_movementPlan = 0;
	m_foodCount = 0;
}

bool Bacteria::takeDamage(int amt) 
{ 
	Agent::takeDamage(amt); //Takes damage and increases score if dies, plays respective sounds
	if (isDead()) {
		getWorld()->increaseScore(100);
		if (randInt(1, 10) <= 5) { //Chance to turn into food
			getWorld()->createActor(new Food(this->getX(), this->getY(), getWorld()));
		}
	}
	return true;
}

void Bacteria::doSomething() //Default actions based on Salmonella
{
	if (isDead()) { //Step 1
		return;
	}
	initialChecks(); //Steps 2-5
	attemptMove(); //Step 6
}

void Bacteria::initialChecks()
{
	if (getWorld()->actorOverlap(this,getWorld()->playerPtr())) { //Deals damage if overlapping with Socrates
		getWorld()->playerPtr()->takeDamage(m_dealtDmg);
		return; //Skips additional steps if succeeds
	}
	if (replication()) { //Replicates if eaten 3 food objects
		return; //Skips additional steps if succeeds
	}
	if (getWorld()->bacteriaEat(this)) { //Eats food if within overlap
		eatFood();
	}
}

void Bacteria::attemptMove()  //Default movement for Salmonella
{
	if (m_movementPlan > 0) { //Finds movement forwards or random direction if blocked, if movement plan is more than 0
		m_movementPlan--;
		double x, y;
		getPositionInThisDirection(getDirection(), 3, x, y);
		if (getWorld()->blocked(x, y)) { //Is it blocked or not
			setDirection(randInt(0, 359)); //Random direction if blocked
			resetMovement();
		} else {
			moveForward(3); //Not blocked, moves in current direction by 3 (Salmonella moves 3 units)
		}
		return;
	}
	getWorld()->moveToFood(this); //Finds food if no movement plan
}

bool Bacteria::replication() //Tells derived function to replicate or not at (x, y)
{
	if (m_foodCount == 3) { //If it has eaten 3 food and is ready to replicate
		double x, y;
		double oldX = this->getX(); 
		double oldY = this->getY();
		//Compute x
		if (oldX == VIEW_WIDTH / 2) {
			x = oldX;
		}
		else if (oldX < VIEW_WIDTH / 2) {
			x = oldX + SPRITE_WIDTH / 2;
		}
		else {
			x = oldX - SPRITE_WIDTH / 2;
		}
		//Compute y
		if (oldY == VIEW_HEIGHT / 2) {
			y = oldY;
		}
		else if (oldY < VIEW_HEIGHT / 2) {
			y = oldY + SPRITE_WIDTH / 2;
		}
		else {
			y = oldY - SPRITE_WIDTH / 2;
		}
		m_foodCount = 0;
		createSameTypeBacteria(x, y); //creates the same type of Bacteria at location determined by func.
		return true;
	}
	return false;
}



//Specific Actors



// Socrates implementation

Socrates::Socrates(StudentWorld* myWorld) : Agent(IID_PLAYER, 0, VIEW_HEIGHT / 2, right, myWorld, 100), m_positionalAngle(180), m_sprayAmmo(20), m_flameAmmo(5), m_waitTick(true)
{
}

void Socrates::doSomething() {
	if (isDead()) { //Doesn't do anything if dead
		return;
	}
	int key;
	if (getWorld()->getKey(key)) { //Finds what key is pressed
		switch (key) {
			//Escape Key
		case KEY_PRESS_ESCAPE: //Kills Socrates
			setDead();
			break;
			//Directional Movement
		case KEY_PRESS_LEFT: //Moves counterclockwise
			circularMove(5);
			break;
		case KEY_PRESS_RIGHT: //Moves clockwise
			circularMove(-5);
			break;
			//Projectiles
		case KEY_PRESS_SPACE: //Spray
			if (m_sprayAmmo > 0) { //Will not spray if no ammo left
				m_waitTick = false; //Sprayed recently, will not replenish until waited a tick without spraying
				createSpray();
				m_sprayAmmo--;
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
			}
			break;
		case KEY_PRESS_ENTER: //Flames
			if (m_flameAmmo > 0) { //Will not flamethrower if no ammo left
				createFlames();
				m_flameAmmo--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		default:
			break; //key doesn't have a command specified
		}
	}
	else { //No keys pressed
		if (m_waitTick && m_sprayAmmo < 20) {
			m_sprayAmmo++;
		}
		else {
			m_waitTick = true; //Must wait a tick after spraying to refill spray
		}
	}
	return;
}

void Socrates::heal()
{
	setHealth(100); //Reset Socrates HP to full
}

void Socrates::createFlames()
{
	Direction startDirection = getDirection(); //Starting directly in front of socrates
	double x, y;
	for (int i = 0; i < 16; i++) { //Creates a ring of 16 flames in increments of 22 degrees
		getPositionInThisDirection(startDirection - i * 22, SPRITE_WIDTH, x, y); //SPRITE_WIDTH away from Socrates
		getWorld()->createActor(new Flame(x, y, startDirection - i * 22, getWorld()));
	}
}

void Socrates::createSpray()
{
	Direction startDirection = getDirection();  //Starting directly in front of socrates
	double x, y;
	getPositionInThisDirection(startDirection, SPRITE_WIDTH, x, y); //SPRITE_WIDTH away from Socrates
	getWorld()->createActor(new Spray(x, y, startDirection, getWorld()));
}

void Socrates::circularMove(int degrees)
{
	const double PI = 4 * atan(1);
	//Setting Positional Angle
	m_positionalAngle += degrees;
	while (m_positionalAngle < 0) { //Keeps m_positionalAngle into interval [0, 2PI)
		m_positionalAngle += 360;
	}
	m_positionalAngle %= 360;
	//Movement in circumference
	moveTo(VIEW_RADIUS * cos(m_positionalAngle * PI / 180) + VIEW_WIDTH / 2, VIEW_RADIUS * sin(m_positionalAngle * PI / 180) + VIEW_HEIGHT / 2);
	setDirection(m_positionalAngle + 180); //Always facing the center
}

//Dirt implementation

Dirt::Dirt(double x, double y, StudentWorld* myWorld) : Actor(IID_DIRT, x, y, 0, myWorld, 1)
{
}

//Pit implementation

Pit::Pit(double x, double y, StudentWorld* myWorld) : Actor(IID_PIT, x, y, 0, myWorld, 1)
{
	//3 Mapped Values to allow for equal probability of types while still having inventory

	m_inventory.insert(pair<int, int>(0, 5)); //Regular Salmonella
	m_inventory.insert(pair<int, int>(1, 3)); //Aggressive Salmonella
	m_inventory.insert(pair<int, int>(2, 2)); //E.Coli
}

void Pit::doSomething()
{
	if (m_inventory.empty()) { //if empty, sets to dead and doesn't do anything after
		setDead();
		return;
	}
	if (randInt(1, 50) == 1) { //1/50 chance of spawning Bacteria
		map<int, int>::iterator it = m_inventory.begin();
		for (int i = 0; i < randInt(0, m_inventory.size() - 1); i++) {
			it++; //iterates through elements to choose
		} //gets a random Case number (mapped value) based on size
		double x = this->getX();
		double y = this->getY();
		switch (it->first) {
		case 0: //Regular Salmonella
			getWorld()->createActor(new RegSalmonella(x, y, getWorld()));
			break;
		case 1: //Aggressive Salmonella
			getWorld()->createActor(new MadSalmonella(x, y, getWorld()));
			break;
		case 2: //E.Coli
			getWorld()->createActor(new EColi(x, y, getWorld()));
			break;
		default:
			break;
		}
		it->second -= 1; //Decreases inventory
		getWorld()->playSound(SOUND_BACTERIUM_BORN);
		if (it->second == 0) { //No more of that type of Bacteria, erases map to allow for equal probability of remaining actors
			m_inventory.erase(it);
		}
	}
}

//Food implementation

Food::Food(double x, double y, StudentWorld* myWorld) : Actor(IID_FOOD, x, y, 90, myWorld, 1)
{
}

void Food::doSomething() //Doesn't do anything by itself, managed by StudentWorld instead (StudentWorld sets it to dead if overlapping with Bacteria)
{
}

/////////Projectiles implementation/////////

//Spray implementation

Spray::Spray(double x, double y, Direction startDirection, StudentWorld* myWorld) : Projectile(IID_SPRAY, x, y, startDirection, 112, myWorld)
{	
}

//Flame implementation

Flame::Flame(double x, double y, Direction startDirection, StudentWorld* myWorld) : Projectile(IID_FLAME, x, y, startDirection, 32, myWorld)
{
}

/////////LifespanObjects implementation/////////

//Fungus implementation

Fungus::Fungus(double x, double y, StudentWorld* myWorld) : LifespanObjects(IID_FUNGUS, x, y, myWorld)
{
	setScoreMod(-50);
}

void Fungus::doSpecificFunc()
{
	getWorld()->playerPtr()->takeDamage(20); //Deals damage to player if picked up
}

//Goodies implementations

//ExtraLifeGoodie implementation

ExtraLifeGoodie::ExtraLifeGoodie(double x, double y, StudentWorld* myWorld) : LifespanObjects(IID_EXTRA_LIFE_GOODIE, x, y, myWorld)
{
	setScoreMod(500);
}

void ExtraLifeGoodie::doSpecificFunc()
{
	getWorld()->incLives(); //Grants extra life to player if picked up
}

//FlameGoodie implementation

FlameGoodie::FlameGoodie(double x, double y, StudentWorld* myWorld) : LifespanObjects(IID_FLAME_THROWER_GOODIE, x, y, myWorld)
{
	setScoreMod(300);
}

void FlameGoodie::doSpecificFunc()
{
	getWorld()->playerPtr()->incFlame(); //Give more flamethrower ammo if picked up
}

//HealthGoodie implementation

HealthGoodie::HealthGoodie(double x, double y, StudentWorld* myWorld) : LifespanObjects(IID_RESTORE_HEALTH_GOODIE, x, y, myWorld)
{
	setScoreMod(250);
}

void HealthGoodie::doSpecificFunc()
{
	getWorld()->playerPtr()->heal(); //Restores HP if picked up
}

/////////Bacteria implementation/////////



RegSalmonella::RegSalmonella(double x, double y, StudentWorld* myWorld) : Bacteria(IID_SALMONELLA, x, y, 1, 4, myWorld)
{
}

void RegSalmonella::createSameTypeBacteria(double x, double y) //Called when foodCount is at 3
{
	getWorld()->createActor(new RegSalmonella(x, y, getWorld()));
		
}

MadSalmonella::MadSalmonella(double x, double y, StudentWorld* myWorld) : Bacteria(IID_SALMONELLA, x, y, 2, 10, myWorld)
{
}

void MadSalmonella::createSameTypeBacteria(double x, double y) //Called when foodCount is at 3
{
	getWorld()->createActor(new MadSalmonella(x, y, getWorld()));
}

EColi::EColi(double x, double y, StudentWorld* myWorld) : Bacteria(IID_ECOLI, x, y, 4, 5, myWorld)
{
}

void EColi::createSameTypeBacteria(double x, double y) //Called when foodCount is at 3
{
	getWorld()->createActor(new EColi(x, y, getWorld()));
}

void MadSalmonella::doSomething()
{
	bool skip = false;
	Direction dir = 0;
	if (getWorld()->socratesDistance(this, dir) <= (double) 72) { //If within distance of 72 pixels, then try to move towards Socrates
		skip = true;
		double x, y;
		getPositionInThisDirection(dir, 3, x, y); 
		if (!getWorld()->blocked(x, y)) { //If not blocked by dirt or out of bounds
			setDirection(dir);
			moveForward(3);
		} //Doesn't do anything if blocked
	}
	initialChecks();
	if (!skip) { //if within 72 pixels of Socrates, MadSalmonella doesn't attemptMove()
		attemptMove();
	}
}

void EColi::attemptMove() //Overrides default attemptMove() to go towards Socrates if within 256 pixels
{
	Direction dir;
	if (getWorld()->socratesDistance(this, dir) <= (double) 256) { //If within distance of 256 pixels, then gets Direction referenced by funct and tries to move towards
		for (int i = 0; i < 10; i++) {
			double x, y;
			getPositionInThisDirection(dir, 2, x, y);
			if (!getWorld()->blocked(x, y)) { //If EColi does not go out of bounds or is blocked, then move towards Socrates
				setDirection(dir);
				moveForward(2);
				return;
			}
			else {
				dir += 10; //tries to adjust by theta +10 degress, stays still after 10 movement attempts
			}
		}
	}
}