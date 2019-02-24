#include "Actor.h"
#include "StudentWorld.h"

///// ACTOR /////

Actor::Actor(int imageID, double startX, double startY, StudentWorld* sWorld,
	bool isSolidObject = NOT_SOLID_OBJECT, bool canInfect = CANNOT_INFECT,
	int startDirection = 0, int depth = 0)
	: GraphObject(imageID, startX, startY, startDirection, depth), 
		m_isAlive(true), 
		m_sWorld(sWorld), 
		m_isSolidObject(isSolidObject),
		m_canInfect(canInfect)
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

bool Actor::canInfect()
{
	return m_canInfect;
}

///// PENELOPE /////

Penelope::Penelope(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_PLAYER, startX, startY, sWorld, SOLID_OBJECT, CAN_INFECT, right, 0),
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
			if (getWorld()->locationEmpty(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			dest_x += 4;
			if (getWorld()->locationEmpty(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			dest_y += 4;
			if (getWorld()->locationEmpty(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			dest_y -= 4;
			if (getWorld()->locationEmpty(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		default:
			break;
		}
	}
}

///// WALL /////

Wall::Wall(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_WALL, startX, startY, sWorld, SOLID_OBJECT, CANNOT_INFECT, right, 0)
{
}

void Wall::doSomething()
{
	return;
}

///// EXIT /////

Exit::Exit(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_EXIT, startX, startY, sWorld, NOT_SOLID_OBJECT, CANNOT_INFECT, right, 1)
{
}

void Exit::doSomething()
{
	// TODO: Figure out how to block flames but nothing else
	// TODO: Determine overlap with person
	// TODO: check if all citizens have exited
	if (getWorld()->exitPen(this))
	{
		getWorld()->setLevelFinished();
		getWorld()->playSound(SOUND_LEVEL_FINISHED);
		cerr << "EXIT" << endl;
	}
	return;
}


///// ZOMBIE /////

Zombie::Zombie(double startX, double startY, StudentWorld* sWorld)
	: Actor(IID_ZOMBIE, startX, startY, sWorld, SOLID_OBJECT, CANNOT_INFECT, right, 0),
	m_movementPlanDistance(0), m_currentTick(0)
{
	// TODO: Other Circumstances
}

int Zombie::getCurrentTick()
{
	return m_currentTick;
}
void Zombie::nextTick()
{
	m_currentTick++;
}

int  Zombie::getMovementPlanDistance()
{
	return m_movementPlanDistance;
}
void Zombie::setMovementPlanDistance(int x)
{
	m_movementPlanDistance = x;
}
void Zombie::decMovementPlanDistance()
{
	m_movementPlanDistance--;
}

bool Zombie::startDoSomething()
{
	// Increase tick counter
	nextTick();
	// Check if Zombie is alive
	if (!isAlive())
		return false;
	// Check if Zombie is paralized
	if (getCurrentTick() % 2 == 0)
		return false;
	// TODO: Implement Vomit function
	return true;
}
void Zombie::move()
{
	// Attempt to move zombie in a certain direction
	int dest_x = getX();
	int dest_y = getY();
	switch (getDirection())
	{
		// TODO: Check if one pixel is one in this code
	case left:
		dest_x -= 1;
		break;
	case right:
		dest_x += 1;
		break;
	case up:
		dest_y += 1;
		break;
	case down:
		dest_y -= 1;
		break;
	}
	if (getWorld()->locationEmpty(this, dest_x, dest_y))
	{
		moveTo(dest_x, dest_y);
		decMovementPlanDistance();
	}
	else
		setMovementPlanDistance(0);
}

void Zombie::doSomething()
{
	if (!startDoSomething())
		return;
	// Check if the zombie needs a new movement plan
	decideMovementPlan();
	// move
	move();
}


///// DUMB ZOMBIE /////

DumbZombie::DumbZombie(double startX, double startY, StudentWorld* sWorld)
	: Zombie(startX, startY, sWorld)
{
}

void DumbZombie::decideMovementPlan()
{
	// Check if the zombie needs a new movement plan
	if (getMovementPlanDistance() == 0)
	{
		setMovementPlanDistance(randInt(3, 10));
		int DIRS[] = { up, down, left, right };
		setDirection(DIRS[randInt(0, 3)]);
	}
}

///// SMART ZOMBIE /////

SmartZombie::SmartZombie(double startX, double startY, StudentWorld* sWorld)
	: Zombie(startX, startY, sWorld)
{
}

void SmartZombie::decideMovementPlan()
{
	// Check if the zombie needs a new movement plan
	if (getMovementPlanDistance() == 0)
	{
		setMovementPlanDistance(randInt(3, 10));
		int direction = getWorld()->dirOfClosestPerson(this);
		setDirection(direction);
	}
}