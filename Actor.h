#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, StudentWorld* sWorld,
		int startDirection = 0, int depth = 0);

	virtual void doSomething() = 0;

	StudentWorld* getWorld();

	bool isAlive();
	void setDead();
private:
	bool m_alive;
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

#endif // ACTOR_H_
