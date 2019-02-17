#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY,
		int startDirection = 0, int depth = 0);
	virtual bool doSomething() = 0;

private:
};

class Penelope : public Actor
{
public:
	Penelope(double startX, double startY);
	virtual bool doSomething();

private:
};

#endif // ACTOR_H_
