#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Socrates;
class Bacteria;
class Projectile;

using Direction = int; //Why isn't this defined in GameConstants bruh moment

class StudentWorld : public GameWorld
{
public:
    //Given functs
    StudentWorld(std::string assetPath);
    virtual int init(); //Initializes the petri dish with initial actors
    virtual int move(); //Calls all Actors to doSomething per tick and removes dead Actors
    virtual void cleanUp(); //Level is finished and all Actors are deleted
    //Additional functions
        //Destructor
    virtual ~StudentWorld() { cleanUp(); } //In case cleanUp is not called, destructor calls cleanUp()
        //Accessors 
    Socrates* playerPtr() const { return m_player; } //returns pointer to Socrates, only pointer ever returned
    double socratesDistance(Actor* ptr, Direction& dir) const; //returns distance and angle from Socrates
    bool blocked(double x, double y) const; //returns whether location is blocked or out of bounds
    bool actorOverlap(Actor* ptr1, Actor* ptr2) const; //returns true if overlapping two actors
        //Mutators
    void createActor(Actor* actPtr); //pushes dynamically allocated object to list
    bool dealDamage(Projectile* ptr); //returns true if damaged an object
    bool bacteriaEat(Bacteria* ptr); //returns true if food is eaten by Bacteria
    void moveToFood(Bacteria* ptr); //tells Bacteria to move towards food
private:
    //Helper functions (only called within StudentWorld)
        //init funct
    void randDistance(double& x, double& y) const; //creates a random (x,y) coordinate within the 120 units of the center
    void overlapFix(double& x, double& y) const; //finds a random(x,y) coordinate by using randDistance to find a point that doesn't overlap with existing Actors (except Dirt)
        //move funct
    void createLifespanObjects(); //prompts the world to have a 1/50 chance of creating Goodies or Fungus (exclusively)
    void updateGameText(); //updates gameText on top
    bool isFinishedLevel() const; //checks if all enemies (Pits and Bacteria) are dead
        //Object Analysis
    double distanceFromCenter(double x, double y) const; //finds distance from center given a coordinate
    double distanceBetweenTwo(Actor* ptr1, Actor* ptr2, Direction& dir) const; //finds distance between two actors and returns the Direction that ptr1 must face to be towards ptr2
    //Data members
    std::list<Actor*> m_actors;
    Socrates* m_player;
};

#endif // STUDENTWORLD_H_
