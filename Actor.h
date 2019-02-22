#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

// constants required for these classes
const bool SOLID_OBJECT		= true;
const bool NOT_SOLID_OBJECT = false;

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, StudentWorld* sWorld, bool isSolidObject,
		int startDirection = 0, int depth = 0);

	virtual void doSomething() = 0;

	StudentWorld* getWorld();

	bool isAlive();
	void setIsAlive(bool isAlive);

	bool isSolidObject();
	void setIsSolidObject(bool isSolidObject);

private:
	bool m_isAlive;
	bool m_isSolidObject;
	StudentWorld* m_sWorld;
};


class Penelope : public Actor
{
public:
	Penelope(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();

private:
	int m_landmines;
	int m_flamethrowerCharges;
	int m_vaccines;
	bool m_infectionStatus;
	int m_infectionCount;
};


class Wall : public Actor
{
public:
	Wall(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething(); // returns immediately
private:
};


class Exit : public Actor
{
public:
	Exit(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething(); // returns immediately
private:
};


class Zombie : public Actor
{
public:
	Zombie(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();

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
	DumbZombie(double startX, double startY, StudentWorld* sWorld);
	virtual void decideMovementPlan();

private:
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(double startX, double startY, StudentWorld* sWorld);
	virtual void decideMovementPlan();
private:
};


#endif // ACTOR_H_
