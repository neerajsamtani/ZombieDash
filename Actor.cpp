#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///// ACTOR /////

Actor::Actor(int imageID, double startX, double startY, StudentWorld* sWorld,
	int startDirection, int depth)
	: GraphObject(imageID, startX, startY, startDirection, depth), m_alive(true), m_sWorld(sWorld)
{ }

StudentWorld* Actor::getWorld()
{
	return m_sWorld;
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

Penelope::Penelope(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_PLAYER, startX, startY, sWorld, right, 0),
	m_landmines(0),
	m_flamethrowerCharges(0),
	m_vaccines(0),
	m_infectionStatus(false),
	m_infectionCount(0)
{
}

void Penelope::doSomething()
{
	// Check to see if Penelope is alive
	if (!isAlive())
		return;
	// Check to see if Penelope is infected
	if (m_infectionStatus)
		m_infectionCount++;
	if (m_infectionCount >= 500)
	{
		setDead();
		// TODO: The game must play a SOUND_PLAYER_DIE sound effect
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
			// TODO: Vaccine functionality
			break;
		// TODO: Object overlap
		case KEY_PRESS_LEFT:
			cerr << "LEFT" << endl;
			setDirection(left);
			dest_x-= 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_RIGHT:
			cerr << "RIGHT" << endl;
			setDirection(right);
			dest_x += 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_UP:
			cerr << "UP" << endl;
			setDirection(up);
			dest_y += 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_DOWN:
			cerr << "DOWN" << endl;
			setDirection(down);
			dest_y -= 4;
			if (getWorld()->locationEmpty(dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		}
	}
}

///// WALL /////

Wall::Wall(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_WALL, startX, startY, sWorld, right, 0)
{
}

void Wall::doSomething()
{
	return;
}

