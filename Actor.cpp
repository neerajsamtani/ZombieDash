#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///// ACTOR /////

Actor::Actor(int imageID, double startX, double startY,
	int startDirection, int depth)
	: GraphObject(imageID, startX, startY, startDirection, depth)
{
}

///// PENELOPE /////

Penelope::Penelope(double startX, double startY)
	: Actor(IID_PLAYER, startX, startY)
{
}

bool Penelope::doSomething()
{
	return true;
}

