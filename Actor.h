#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

// constants required for these classes
// ...

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth);

	// Action to perform for each tick.
	virtual void doSomething() = 0;

	// Is this actor dead?
	bool isDead() const;

	// Mark this actor as dead.
	void setDead();

	// Get this actor's world
	StudentWorld* world() const;

	// If this is an activated object, perform its effect on a (e.g., for an
	// Exit have a use the exit).
	//virtual void activateIfAppropriate(Actor* a);

	// If this object uses exits, use the exit.
	virtual void useExitIfAppropriate();

	// If this object can die by falling into a pit or burning, die.
	//virtual void dieByFallOrBurnIfAppropriate();

	// If this object can be infected by vomit, get infected.
	virtual void beVomitedOnIfAppropriate();

	// If this object can pick up goodies, pick up g
	//virtual void pickUpGoodieIfAppropriate(Goodie* g);

	// Does this object block agent movement?
	virtual bool blocksMovement() const;

	// Does this object block flames?
	virtual bool blocksFlame() const;

	// Does this object trigger landmines only when they're active?
	//virtual bool triggersOnlyActiveLandmines() const;

	// Can this object cause a zombie to vomit?
	virtual bool triggersZombieVomit() const;

	// Is this object a threat to citizens?
	//virtual bool threatensCitizens() const;

	// Does this object trigger citizens to follow it or flee it?
	//virtual bool triggersCitizens() const;

private:
	bool m_isDead;
	bool m_isInfected;
	StudentWorld* m_sWorld;
};

/*
class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, StudentWorld* sWorld, 
		bool m_isSolidObject, bool m_canInfect,
		int startDirection, int depth);

	virtual void doSomething() = 0;

	StudentWorld* getWorld();

	bool isDead();
	void setIsAlive(bool isAlive);

	bool isSolidObject();
	void setIsSolidObject(bool isSolidObject);

	bool canInfect();

private:
	bool m_isAlive;
	bool m_isSolidObject;
	bool m_canInfect;
	
};
*/

class Agent : public Actor
{
public:
	Agent(StudentWorld* w, int imageID, double x, double y, int dir);
	virtual bool blocksMovement() const;
	virtual bool triggersOnlyActiveLandmines() const;
};

class Human : public Agent
{
public:
	Human(StudentWorld* w, int imageID, double x, double y);
	virtual void beVomitedOnIfAppropriate();
	virtual bool triggersZombieVomit() const;

	// Make this human uninfected by vomit.
	void clearInfection();

	// How many ticks since this human was infected by vomit?
	int getInfectionDuration() const;
	void incInfectionDuration(); //TODO: NOT IN INTERFACE

private:
	bool m_infectionStatus;
	int m_infectionCount;
};

class Penelope : public Human
{
public:
	Penelope(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
	//virtual void pickUpGoodieIfAppropriate(Goodie* g);

	// Increase the number of vaccines the object has.
	void increaseVaccines();

	// Increase the number of flame charges the object has.
	void increaseFlameCharges();

	// Increase the number of landmines the object has.
	void increaseLandmines();

	// How many vaccines does the object have?
	int getNumVaccines() const;

	// How many flame charges does the object have?
	int getNumFlameCharges() const;

	// How many landmines does the object have?
	int getNumLandmines() const;

private:
	int m_landmines;
	int m_flamethrowerCharges;
	int m_vaccines;
};

class Citizen : public Human
{
public:
	Citizen(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
};


class Zombie : public Agent
{
public:
	Zombie(StudentWorld* w, double x, double y);

	bool startDoSomething();
	virtual void decideMovementPlan() = 0;
	void move();

	int  getCurrentTick();
	void nextTick();

	int  getMovementPlanDistance();
	void setMovementPlanDistance(int x);
	void decMovementPlanDistance();

private:

	int m_movementPlanDistance;
	int m_currentTick;
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
private:
	void decideMovementPlan();
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
private:
	void decideMovementPlan();
};


class Wall : public Actor
{
public:
	Wall(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement() const;
	virtual bool blocksFlame() const;
};

class ActivatingObject : public Actor
{
public:
	ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir);
};

class Exit : public ActivatingObject
{
public:
	Exit(StudentWorld* w, double x, double y);
	virtual void doSomething();

	virtual void activateIfAppropriate(Actor* a);
	virtual bool blocksFlame() const;
};


#endif // ACTOR_H_
