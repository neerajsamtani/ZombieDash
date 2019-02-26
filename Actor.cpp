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

void Actor::activateIfAppropriate(Actor* a)
{
	return;
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

bool Actor::triggersOnlyActiveLandmines() const
{
	return false;
}

bool Actor::triggersZombieVomit() const
{
	return false;
}

void Actor::useExitIfAppropriate()
{
	return;
}

void Actor::dieByFallOrBurnIfAppropriate()
{
	return;
}

void Actor::pickUpGoodieIfAppropriate(Goodie* g)
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
	cerr << "Create exit" << endl;
}

bool Exit::blocksFlame() const
{
	return true;
}

void Exit::doSomething()
{
	// TODO: Determine overlap with person
	// TODO: check if all citizens have exited
	world()->activateOnAppropriateActors(this);
}

void Exit::activateIfAppropriate(Actor* a)
{
	if (a == this)
		return;
	if (world()->objectOverlap(a, this))
	{
		a->useExitIfAppropriate();
	}
	return;
}

//// GOODIE ////
// TODO: Remove Code Repetition

Goodie::Goodie(StudentWorld* w, int imageID, double x, double y)
	: ActivatingObject(w, imageID, x, y, 1, right)
{}

void Goodie::activateIfAppropriate(Actor* a)
{
	a->pickUpGoodieIfAppropriate(this);
}

void Goodie::dieByFallOrBurnIfAppropriate()
{
	this->setDead();
}

//// VACCINE GOODIE ////

VaccineGoodie::VaccineGoodie(StudentWorld* w, double x, double y)
	: Goodie(w, IID_VACCINE_GOODIE, x, y)
{}

void VaccineGoodie::doSomething()
{
	if (isDead())
		return;
	world()->activateOnAppropriateActors(this);
}

void VaccineGoodie::pickUp(Penelope* p)
{
	world()->increaseScore(50);
	cerr << "VACCINE ACQUIRED" << endl;
	this->setDead();
	world()->playSound(SOUND_GOT_GOODIE);
	p->increaseVaccines();
}

//// GAS CAN GOODIE ////

GasCanGoodie::GasCanGoodie(StudentWorld* w, double x, double y)
	: Goodie(w, IID_GAS_CAN_GOODIE, x, y)
{}

void GasCanGoodie::doSomething()
{
	if (isDead())
		return;
	world()->activateOnAppropriateActors(this);
}

void GasCanGoodie::pickUp(Penelope* p)
{
	world()->increaseScore(50);
	cerr << "GAS CAN ACQUIRED" << endl;
	this->setDead();
	world()->playSound(SOUND_GOT_GOODIE);
	p->increaseFlameCharges();
}

//// LANDMINE GOODIE ////

LandmineGoodie::LandmineGoodie(StudentWorld* w, double x, double y)
	: Goodie(w, IID_LANDMINE_GOODIE, x, y)
{}

void LandmineGoodie::doSomething()
{
	if (isDead())
		return;
	world()->activateOnAppropriateActors(this);
}

void LandmineGoodie::pickUp(Penelope* p)
{
	world()->increaseScore(50);
	cerr << "LANDMINE ACQUIRED" << endl;
	this->setDead();
	world()->playSound(SOUND_GOT_GOODIE);
	p->increaseLandmines();
}

//// LANDMINE ////

Landmine::Landmine(StudentWorld* w, double x, double y)
	: ActivatingObject(w, IID_LANDMINE, x, y, 1, right),
	m_safetyTicks(30),
	m_active(false)
{}

void Landmine::doSomething()
{
	if (isDead())
		return;
	if (!m_active)
	{
		m_safetyTicks--;
		if (m_safetyTicks == 0)
		{
			m_active = true;
			return;
		}
	}
	else
		world()->activateOnAppropriateActors(this);
}

void Landmine::activateIfAppropriate(Actor* a)
{
	if (a->triggersOnlyActiveLandmines())
	{
		this->setDead();
		world()->playSound(SOUND_LANDMINE_EXPLODE);
		// TODO: Introduce flames and pit
	}
}


void Landmine::dieByFallOrBurnIfAppropriate()
{
	this->setDead();
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
	m_vaccines(0),
	m_infectionCount(0),
	m_infectedStatus(false)
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
			if (getNumFlameCharges() > 0)
			{
				m_flamethrowerCharges--;
				world()->playSound(SOUND_PLAYER_FIRE);
				cerr << "USED ONE FLAME CHARGE" << endl;
			}
			// TODO: Flame functionality
			break;
		case KEY_PRESS_TAB:
			if (getNumLandmines() > 0)
			{
				// TODO: Add Landmine
				world()->addActor(new Landmine(world(), getX(), getY()));
				m_landmines--;
				cerr << "USED ONE LANDMINE" << endl;
			}
			// TODO: Landmine functionality
			break;
		case KEY_PRESS_ENTER:
			if (getNumVaccines() > 0)
			{
				m_infectedStatus = false;
				m_infectionCount = 0;
				m_vaccines--;
				cerr << "USED VACCINE" << endl;
			}
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
	// TODO: Check
	world()->setLevelFinished();
	world()->playSound(SOUND_LEVEL_FINISHED);
	cerr << "EXIT" << endl;
}


void Penelope::dieByFallOrBurnIfAppropriate()
{
	this->setDead();
	//TODO: IMPLEMENT
}


void Penelope::pickUpGoodieIfAppropriate(Goodie* g)
{
	g->pickUp(this);
}

// Increase the number of vaccines the object has.
void Penelope::increaseVaccines()
{
	m_vaccines++;
}

// Increase the number of flame charges the object has.
void Penelope::increaseFlameCharges()
{
	m_flamethrowerCharges += 5;
}

// Increase the number of landmines the object has.
void Penelope::increaseLandmines()
{
	m_landmines += 2;
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
