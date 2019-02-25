#include "Actor.h"
#include "StudentWorld.h"

///// ACTOR /////

/*
Actor::Actor(int imageID, double startX, double startY, StudentWorld* sWorld,
	bool isSolidObject = NOT_SOLID_OBJECT, bool canInfect = CANNOT_INFECT,
	int startDirection = 0, int depth = 0)
*/

Actor::Actor(StudentWorld * w, int imageID, double x, double y, int depth, int dir)
	:GraphObject(imageID, x, y, dir, depth),
	m_sWorld(w),
	m_isDead(false)
{
}

StudentWorld* Actor::world() const
{
	return m_sWorld;
}

bool Actor::isDead() const
{
	return m_isDead;
}
void Actor::setDead()
{
	m_isDead = true;
}

bool Actor::blocksMovement() const
{
	return false;
}

bool Actor::blocksFlame() const
{
	return false;
}

void Actor::beVomitedOnIfAppropriate()
{
	return;
}

bool Actor::triggersZombieVomit() const
{
	return false;
}

void Actor::useExitIfAppropriate()
{
	return;
}

///// WALL /////

Wall::Wall(StudentWorld * w, double x, double y)
	: Actor(w, IID_WALL, x, y, right, 0)
{
}

void Wall::doSomething()
{
	return;
}

bool Wall::blocksMovement() const
{
	return true;
}

bool Wall::blocksFlame() const
{
	return true;
}

//// ACTIVATING OBJECT ////

ActivatingObject::ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir)
	:Actor(w, imageID, x, y, depth, dir)
{
}

///// EXIT /////

Exit::Exit(StudentWorld* w, double x, double y)
	: ActivatingObject(w, IID_EXIT, x, y, 1, right)
{
}

bool Exit::blocksFlame() const
{
	return true;
}

void Exit::doSomething()
{
	// TODO: Determine overlap with person
	// TODO: check if all citizens have exited
	if (world()->exitPen(this))
	{
		world()->setLevelFinished();
		world()->playSound(SOUND_LEVEL_FINISHED);
		cerr << "EXIT" << endl;
	}
	return;
}

void Exit::activateIfAppropriate(Actor* a)
{
	a->useExitIfAppropriate();
}

//// AGENT ////

Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir)
	:Actor(w, imageID, x, y, 0, dir)
{
}
bool Agent::blocksMovement() const
{
	return true;
}
bool Agent::triggersOnlyActiveLandmines() const
{
	return true;
}

//// HUMAN ////

Human::Human(StudentWorld* w, int imageID, double x, double y)
	: Agent(w, imageID, x, y, right),
	m_infectionStatus(false),
	m_infectionCount(0)
{
}

void Human::beVomitedOnIfAppropriate()
{
	return;
	// TODO: COMPLETE
}
bool Human::triggersZombieVomit() const
{
	return true;
}

// Make this human uninfected by vomit.
void Human::clearInfection()
{
	m_infectionStatus = false;
	m_infectionCount = 0;
}

// How many ticks since this human was infected by vomit?
int Human::getInfectionDuration() const
{
	return m_infectionCount;
}

void Human::incInfectionDuration()
{
	m_infectionCount++;
}

///// PENELOPE /////

Penelope::Penelope(StudentWorld* w, double x, double y)
	: Human(w, IID_PLAYER, x, y),
	m_landmines(0),
	m_flamethrowerCharges(0),
	m_vaccines(0)
{}

void Penelope::doSomething()
{
	// Check if Penelope is alive
	if (isDead())
		return;
	// Check if Penelope is infected
	if (getInfectionDuration() > 0)
		incInfectionDuration();
	// Check if Penelope is dead
	if (getInfectionDuration() >= 500)
	{
		setDead();
		world()->playSound(SOUND_PLAYER_DIE);
		return;
		// TODO: (The StudentWorld object should then detect that she’s dead and the current level ends)
	}
	int ch;
	if (world()->getKey(ch))
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
			// cerr << world()->getLives() << endl;
			// TODO: Vaccine functionality
			break;
			// TODO: Object overlap
		case KEY_PRESS_LEFT:
			setDirection(left);
			dest_x -= 4;
			if (world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			dest_x += 4;
			if (world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			dest_y += 4;
			if (world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			dest_y -= 4;
			if (world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		default:
			break;
		}
	}
}

void Penelope::useExitIfAppropriate()
{
	return;
	//TODO: IMPLEMENT
}


void Penelope::dieByFallOrBurnIfAppropriate()
{
	return;
	//TODO: IMPLEMENT
}
/*
void Penelope::pickUpGoodieIfAppropriate(Goodie* g)
{
	return;
	//TODO: IMPLEMENT
}
*/

// Increase the number of vaccines the object has.
void Penelope::increaseVaccines()
{
	m_vaccines++;
}

// Increase the number of flame charges the object has.
void Penelope::increaseFlameCharges()
{
	m_flamethrowerCharges++;
}

// Increase the number of landmines the object has.
void Penelope::increaseLandmines()
{
	m_landmines++;
}

// How many vaccines does the object have?
int Penelope::getNumVaccines() const
{
	return m_vaccines;
}

// How many flame charges does the object have?
int Penelope::getNumFlameCharges() const
{
	return m_flamethrowerCharges;
}

// How many landmines does the object have?
int Penelope::getNumLandmines() const
{
	return m_landmines;
}

//// CITIZEN ////

Citizen::Citizen(StudentWorld* w, double x, double y)
	: Human(w, IID_CITIZEN, x, y)
{
}

void Citizen::doSomething()
{
	return;
	//TODO: IMPLEMENT
}
void Citizen::useExitIfAppropriate()
{
	return;
	//TODO: IMPLEMENT
}
void Citizen::dieByFallOrBurnIfAppropriate()
{
	return;
	//TODO: IMPLEMENT
}

///// ZOMBIE /////

Zombie::Zombie(StudentWorld* w, double x, double y)
	: Agent(w, IID_ZOMBIE, x, y, right),
	m_movementPlanDistance(0), m_currentTick(0)
{
	// TODO: OTHER CIRCUMSTANCES
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
	if (isDead())
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
	if (world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
	{
		moveTo(dest_x, dest_y);
		decMovementPlanDistance();
	}
	else
		setMovementPlanDistance(0);
}


///// DUMB ZOMBIE /////

DumbZombie::DumbZombie(StudentWorld* w, double x, double y)
	: Zombie(w, x, y)
{}

void DumbZombie::doSomething()
{
	// TODO: BETTER INHERITANCE
	if (!startDoSomething())
		return;
	// Check if the zombie needs a new movement plan
	decideMovementPlan();
	// move
	move();
}
void DumbZombie::dieByFallOrBurnIfAppropriate()
{
	return;
	// TODO: IMPLEMENT
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

SmartZombie::SmartZombie(StudentWorld* w, double x, double y)
	: Zombie(w, x, y)
{}

void SmartZombie::doSomething()
{
	// TODO: BETTER INHERITANCE
	if (!startDoSomething())
		return;
	// Check if the zombie needs a new movement plan
	decideMovementPlan();
	// move
	move();
}
void SmartZombie::dieByFallOrBurnIfAppropriate()
{
	return;
	// TODO: IMPLEMENT
}

void SmartZombie::decideMovementPlan()
{
	// Check if the zombie needs a new movement plan
	if (getMovementPlanDistance() == 0)
	{
		setMovementPlanDistance(randInt(3, 10));
		int direction = world()->dirOfClosestPerson(this);
		setDirection(direction);
	}
}
