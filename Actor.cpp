#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///// ACTOR /////

Actor::Actor(int imageID, double startX, double startY, StudentWorld* sWorld, bool isSolidObject,
	int startDirection, int depth)
	: GraphObject(imageID, startX, startY, startDirection, depth), 
		m_isAlive(true), 
		m_sWorld(sWorld), 
		m_isSolidObject(isSolidObject)
{ }

StudentWorld* Actor::getWorld()
{
	return m_sWorld;
}

bool Actor::isAlive()
{
	return m_isAlive;
}
void Actor::setIsAlive(bool isAlive)
{
	m_isAlive = isAlive;
}

bool Actor::isSolidObject()
{
	return m_isSolidObject;
}
void Actor::setIsSolidObject(bool isSolidObject)
{
	m_isSolidObject = isSolidObject;
}

///// PENELOPE /////

Penelope::Penelope(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_PLAYER, startX, startY, sWorld, SOLID_OBJECT, right, 0),
	m_landmines(0),
	m_flamethrowerCharges(0),
	m_vaccines(0),
	m_infectionStatus(false),
	m_infectionCount(0)
{
}

void Penelope::doSomething()
{
	// Check if Penelope is alive
	if (!isAlive())
		return;
	// Check if Penelope is infected
	if (m_infectionStatus)
		m_infectionCount++;
	// Check if Penelope is dead
	if (m_infectionCount >= 500)
	{
		setIsAlive(false);
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
		// TODO: (The StudentWorld object should then detect that she’s dead and the current level ends)
	}
	int ch;
	if (getWorld()->getKey(ch))
	{
		int dest_x = getX();
		int dest_y = getY();
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_SPACE:
			cerr << "SPACE" << endl;
			//	TODO: Flamethrower functionality
			break;
		case KEY_PRESS_TAB:
			cerr << "TAB" << endl;
			// TODO: Landmine functionality
			break;
		case KEY_PRESS_ENTER:
			cerr << "ENTER" << endl;
			// cerr << getWorld()->getLives() << endl;
			// TODO: Vaccine functionality
			break;
		// TODO: Object overlap
		case KEY_PRESS_LEFT:
			setDirection(left);
			dest_x-= 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			dest_x += 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			dest_y += 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			dest_y -= 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		default:
			break;
		}
	}
}

///// WALL /////

Wall::Wall(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_WALL, startX, startY, sWorld, SOLID_OBJECT, right, 0)
{
}

void Wall::doSomething()
{
	return;
}

///// EXIT /////

Exit::Exit(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_EXIT, startX, startY, sWorld, NOT_SOLID_OBJECT, right, 1)
{
}

void Exit::doSomething()
{
	// TODO: Figure out how to block flames but nothing else
	// TODO: Determine overlap with person
	// TODO: check if all citizens have exited
	if (getWorld()->exitPen(this))
	{
		getWorld()->advanceToNextLevel();
		getWorld()->setLevelFinished();
		cerr << "EXIT" << endl;
	}
	return;
}