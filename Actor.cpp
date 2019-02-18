#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///// ACTOR /////

Actor::Actor(int imageID, double startX, double startY,
	int startDirection, int depth)
	: GraphObject(imageID, startX, startY, startDirection, depth), m_alive(true)
{
}

bool Actor::isAlive()
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
}

///// PENELOPE /////

Penelope::Penelope(double startX, double startY)
	: Actor(IID_PLAYER, startX, startY, right, 0),
	m_landmines(0),
	m_flamethrowerCharges(0),
	m_vaccines(0),
	m_infectionStatus(false),
	m_infectionCount(0)
{
}

void Penelope::doSomething()
{
	return;
}

///// WALL /////

Wall::Wall(double startX, double startY)
	: Actor(IID_WALL, startX, startY, right, 0)
{
}

void Wall::doSomething()
{
	return;
}

