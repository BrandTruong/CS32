#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <map>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject {
public:
	//Constructor
	Actor(int imageID, double startX, double startY, Direction startDirection, StudentWorld* myWorld, int depth = 0);
	//Virtual Destructor
	virtual ~Actor(){}
	//Accessor
		//Pointers
	StudentWorld* getWorld() const { return m_world; } //Returns world
		//Conditional
	bool isDead() const { return m_dead; }
	virtual bool isEnemy() const { return false; } //If enemies exist then prevent level from ending
	virtual bool blocksBacteria() const { return false; } //for Movement block eg Dirt
	virtual bool isEdible() const { return false; } //for Food items
	//Mutator
	virtual void doSomething() = 0;
	void setDead() { m_dead = true; }
	virtual bool takeDamage(int amt) { return false; } //returns whether or not something is damageable (default false, true for Goodies, Dirt, Agents)
private:
	bool m_dead;
	StudentWorld* m_world;
};

//Derived classes from Actor


class Agent : public Actor {
public:
	//Constructor
	Agent(int imageID, double x, double y, int dir, StudentWorld* m_world, int hitPoints);
	//Virtual Destructor
	virtual ~Agent() {}
	virtual bool takeDamage(int amt); //decreases health and sets to dead if lower than or equal to 0 health, plays according sound
	int getHealth() const { return m_health; } //return m_health
protected: //Only called within Agent and its derived classes
	void setHealth(int amt) { m_health = amt; } //resets hp if necessary (Socrates)
	virtual int soundWhenHurt() const = 0; //returns sound value when takes damage
	virtual int soundWhenDie() const = 0; //returns sound value when dies
private:
	int m_health;
	
};

class Projectile: public Actor {
public:
	Projectile(int imageID, double startX, double startY, Direction startDirection, int startDistance, StudentWorld* myWorld);
	//Virtual Destructor
	virtual ~Projectile() {}
	virtual void doSomething(); //All projectiles do the same thing
	virtual int getDamage() const = 0; //returns how much damage it deals, pure virtual makes Projectile abstract class as well
private:
	int m_distance;
};

//For Goodies and Fungus
class LifespanObjects: public Actor {
public:
	LifespanObjects(int imageID, double startX, double startY, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~LifespanObjects() {}
	virtual void doSomething(); //All actors go through the same steps and only differ by what to do when overlapping
	virtual bool takeDamage(int amt) { setDead(); return true; }
protected:
	virtual void getSound(); //default plays the sound of its people, fungus doesn't have a sound :(
	bool overlapSocrates(); //checks if this and Socrates overlap
	void setScoreMod(int mod) { m_scoreMod = mod; } //setScoreMod
	virtual void doSpecificFunc() = 0; //When picked up by Socrates do this
	void lifespan(); //decreases lifespan if not at 0, otherwise set dead
private:
	int m_lifetime;
	int m_scoreMod;
};

class Bacteria : public Agent {
public:
	Bacteria(int imageID, double x, double y, int dealtDamage, int health, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~Bacteria() {}
	virtual bool isEnemy() const { return true; } //Prevents level from finishing
	virtual void doSomething(); //All bacteria go through similar steps, MadSalmonella redefines to have additional step on top
	//Bacteria jobs
	void resetMovement() { m_movementPlan = 10; } //Resets movement to 10
	virtual bool takeDamage(int amt); //Redefined to take damage and if dead, increment score and possibly turn into food
protected:
	void eatFood() { m_foodCount++; } //Yum yum increments m_foodCount
	bool replication(); //All bacterias replicate, uses createSameTypeBacteria to determine what type to create.
	void initialChecks(); //All Bacterias have same checks, checks if overlapping with Socrates, then if ready to replicate, then if overlapping with food
	virtual void attemptMove(); //Base for Salmonellas, either moves in current direction if has a movement plan, or finds the nearest food, 
protected:
	//Default Bacteria does Salmonella things, overwritten if Ecoli
	virtual int soundWhenHurt() const { return SOUND_SALMONELLA_HURT; } //returns sound value when hurt
	virtual int soundWhenDie() const { return SOUND_SALMONELLA_DIE; } //returns sound value when dies
	virtual void createSameTypeBacteria(double x, double y) = 0; //Pure virtual to create same type of Bacteria at new (x,y) coordinate
private:
	int m_movementPlan;
	int m_foodCount;
	int m_dealtDmg;
};

// Derived classes //

class Socrates : public Agent {
public:
	Socrates(StudentWorld* m_world);
	//Virtual Destructor 
	virtual ~Socrates() {}
	//Accessors
	int getfAmmo() const { return m_flameAmmo; } //Returns flamaethrower ammo
	int getsAmmo() const { return m_sprayAmmo; } //Returns spray ammo
	//Mutators
		//Projectile Creators
	void createFlames(); //Creates 16 Flame Objects in ring
	void createSpray(); //Creates Spray in front of Socrates
	virtual void doSomething();
		//Referenced by Goodie
	void heal(); //Resets hp to 100
	void incFlame() { m_flameAmmo += 5; } //Increases flame ammo
protected:
	virtual int soundWhenHurt() const { return SOUND_PLAYER_HURT; } //returns sound value when hurt
	virtual int soundWhenDie() const { return SOUND_PLAYER_DIE; } //returns sound value when dies
private:
	//Helper Func
	void circularMove(int degrees); //Used to help move Socrates, only called by Socrates
	int m_positionalAngle;
	int m_sprayAmmo;
	int m_flameAmmo;
	bool m_waitTick;
};

class Pit : public Actor {
public:
	Pit(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~Pit() {}
	virtual void doSomething(); //Has a chance to send out a random bacteria in its inventory
	virtual bool isEnemy() const { return true; } //Prevents level from finishing
private:
	std::map<int, int> m_inventory;
};

class Dirt : public Actor {
public:
	//Constructor
	Dirt(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~Dirt() {}
	virtual void doSomething() {} //doesn't do anything during a tick, other actors can interact with it
	virtual bool blocksBacteria() const { return true; } //can block
	virtual bool takeDamage(int amt) { setDead(); return true; } //can take damage (dies in one hit)
};

class Food : public Actor {
public:
	Food(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~Food() {}
	virtual void doSomething(); //Doesn't actually do anything, managed by StudentWorld
	virtual bool isEdible() const { return true; } //Can be eaten
};

class Spray : public Projectile {
public:
	Spray(double x, double y, Direction startDirection, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~Spray() {}
	virtual int getDamage() const { return 2; } //Deals 2 damage
};

class Flame : public Projectile {
public:
	Flame(double x, double y, Direction startDirection, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~Flame() {}
	virtual int getDamage() const { return 5; } //Deals 5 damage
};

class Fungus : public LifespanObjects {
public:
	Fungus(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~Fungus() {}
	virtual void getSound() {} //No sound for Fungus
	virtual void doSpecificFunc(); //Damages by 20 if picked up
};

class HealthGoodie : public LifespanObjects {
public:
	HealthGoodie(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~HealthGoodie() {}
	virtual void doSpecificFunc(); //Restores health to full if picked up
};

class FlameGoodie : public LifespanObjects {
public:
	FlameGoodie(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~FlameGoodie() {}
	virtual void doSpecificFunc(); //Increases flamethrower ammo by 5 if picked up
};

class ExtraLifeGoodie : public LifespanObjects {
public:
	ExtraLifeGoodie(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~ExtraLifeGoodie() {}
	virtual void doSpecificFunc(); //Grants extra life if picked up
};

class RegSalmonella : public Bacteria { //Defaults of Bacteria is Regular Salmonella
public:
	RegSalmonella(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~RegSalmonella() {}
protected:
	virtual void createSameTypeBacteria(double x, double y); //Creates a new Regular Salmonella
};


class MadSalmonella : public Bacteria { //Aggressive Salmonella aka MadSalmonella
public:
	MadSalmonella(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~MadSalmonella() {}
	virtual void doSomething(); //Will attempt to move towards Socrates first if within 72 pixels, otherwise just like a RegSalmonella
protected:
	virtual void createSameTypeBacteria(double x, double y); //Creates a new Aggressive Salmonella
};


class EColi : public Bacteria {
public:
	EColi(double x, double y, StudentWorld* myWorld);
	//Virtual Destructor 
	virtual ~EColi() {}
	virtual void attemptMove(); //Will attempt to move towards Socrates if within 256 pixels
protected:
	virtual void createSameTypeBacteria(double x, double y); //Creates a new EColi
	//Redefines sounds played when hurt or killed to those of EColi
	virtual int soundWhenHurt() const { return SOUND_ECOLI_HURT; }
	virtual int soundWhenDie() const { return SOUND_ECOLI_DIE; }
};
#endif // ACTOR_H