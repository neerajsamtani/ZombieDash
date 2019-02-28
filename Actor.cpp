#include "Actor.h"
#include "StudentWorld.h"

/////////////////
///// ACTOR /////
/////////////////

Actor::Actor(StudentWorld * w, int imageID, double x, double y, int depth, int dir)
	:GraphObject(imageID, x, y, dir, depth),
	m_sWorld(w),
	m_isDead(false)
{}

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

bool Actor::triggersCitizens() const
{
	return false;
}

bool Actor::threatensCitizens() const
{
	return false;
}

////////////////
///// WALL /////
////////////////

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

///////////////////////////
//// ACTIVATING OBJECT ////
///////////////////////////

ActivatingObject::ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir)
	:Actor(w, imageID, x, y, depth, dir)
{}

////////////////
///// EXIT /////
////////////////

Exit::Exit(StudentWorld* w, double x, double y)
	: ActivatingObject(w, IID_EXIT, x, y, 1, right)
{}

bool Exit::blocksFlame() const
{
	return true;
}

void Exit::doSomething()
{
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
}

////////////////
//// GOODIE ////
////////////////

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
	setDead();
}

void Goodie::pickUp(Penelope* p)
{
	// Common Goodie pickup functionality
	world()->increaseScore(50);
	setDead();
	world()->playSound(SOUND_GOT_GOODIE);
}

////////////////////////
//// VACCINE GOODIE ////
////////////////////////

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
	Goodie::pickUp(p);
	p->increaseVaccines();
}

////////////////////////
//// GAS CAN GOODIE ////
////////////////////////

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
	Goodie::pickUp(p);
	p->increaseFlameCharges();
}

/////////////////////////
//// LANDMINE GOODIE ////
/////////////////////////

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
	Goodie::pickUp(p);
	p->increaseLandmines();
}

//////////////////
//// LANDMINE ////
//////////////////

// TODO: Merge Landmine, Pit, and Flame doSomething()

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
		if (m_safetyTicks <= 0)
		{
			m_active = true;
			return;
		}
	}
	else
		world()->activateOnAppropriateActors(this);
}

void Landmine::explode()
{
	// set the landmine to dead (not the person)
	setDead();
	world()->playSound(SOUND_LANDMINE_EXPLODE);
	// Assume the flame's direction is in the direction of the Landmine
	if (!(world()->isFlameBlockedAt(this, getX(), getY())))
		world()->addActor(new Flame(world(), getX(), getY(), up));
	// Introduce Flames
	// North
	if (!(world()->isFlameBlockedAt(this, getX(), getY() + SPRITE_HEIGHT)))
		world()->addActor(new Flame(world(), getX(), getY() + SPRITE_HEIGHT, up));
	// North-East
	if (!(world()->isFlameBlockedAt(this, getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT)))
		world()->addActor(new Flame(world(), getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up));
	// East
	if (!(world()->isFlameBlockedAt(this, getX() + SPRITE_WIDTH, getY())))
		world()->addActor(new Flame(world(), getX() + SPRITE_WIDTH, getY(), up));
	// South-East
	if (!(world()->isFlameBlockedAt(this, getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT)))
		world()->addActor(new Flame(world(), getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up));
	// South
	if (!(world()->isFlameBlockedAt(this, getX(), getY() - SPRITE_HEIGHT)))
		world()->addActor(new Flame(world(), getX(), getY() - SPRITE_HEIGHT, up));
	// South-West
	if (!(world()->isFlameBlockedAt(this, getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT)))
		world()->addActor(new Flame(world(), getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up));
	// East
	if (!(world()->isFlameBlockedAt(this, getX() - SPRITE_WIDTH, getY())))
		world()->addActor(new Flame(world(), getX() - SPRITE_WIDTH, getY(), up));
	// North-West
	if (!(world()->isFlameBlockedAt(this, getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT)))
		world()->addActor(new Flame(world(), getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up));

	world()->addActor(new Pit(world(), getX(), getY()));
}

void Landmine::activateIfAppropriate(Actor* a)
{
	if (a->triggersOnlyActiveLandmines())
	{
		explode();
	}
}

void Landmine::dieByFallOrBurnIfAppropriate()
{
	explode();
}

/////////////
//// PIT ////
/////////////

Pit::Pit(StudentWorld* w, double x, double y)
	: ActivatingObject(w, IID_PIT, x, y, 0, right)
{}

void Pit::doSomething()
{
	world()->activateOnAppropriateActors(this);
}

void Pit::activateIfAppropriate(Actor* a)
{
	// Pit shouldn't try to kill a dead object
	if(!(a->isDead()))
		a->dieByFallOrBurnIfAppropriate();
}

///////////////
//// FLAME ////
///////////////

Flame::Flame(StudentWorld* w, double x, double y, int dir)
	: ActivatingObject(w, IID_FLAME, x, y, 0, dir),
	m_ticksLeft(2)
{}

void Flame::doSomething()
{
	if (isDead())
		return;
	m_ticksLeft--;
	if (m_ticksLeft <= 0)
		setDead();
	else
		world()->activateOnAppropriateActors(this);
}

void Flame::activateIfAppropriate(Actor* a)
{
	// Flame shouldn't try to kill a dead object
	if (!(a->isDead()))
		a->dieByFallOrBurnIfAppropriate();
}

bool Flame::blocksFlame() const
{
	// Assume a flame blocks another flame
	return true;
}

///////////////
//// VOMIT ////
///////////////

Vomit::Vomit(StudentWorld* w, double x, double y, int dir)
	: ActivatingObject(w, IID_VOMIT, x, y, 0, dir),
	m_ticksLeft(2)
{}

void Vomit::doSomething()
{
	if (isDead())
		return;
	m_ticksLeft--;
	if (m_ticksLeft <= 0)
		setDead();
	else
		world()->activateOnAppropriateActors(this);
}

void Vomit::activateIfAppropriate(Actor* a)
{
	// Vomit shouldn't try to infect a dead object
	if (!(a->isDead()))
		a->beVomitedOnIfAppropriate();
}

///////////////
//// AGENT ////
///////////////

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

///////////////
//// HUMAN ////
///////////////

Human::Human(StudentWorld* w, int imageID, double x, double y)
	: Agent(w, imageID, x, y, right),
	m_infectionStatus(false),
	m_infectionCount(0)
{}

void Human::beVomitedOnIfAppropriateHelper()
{
	return;
}

void Human::beVomitedOnIfAppropriate()
{
	// Play sound if citizen is infected
	// Don't play a sound if Penelope is infected
	beVomitedOnIfAppropriateHelper();
	m_infectionStatus = true;
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

bool Human::getInfectionStatus() const
{
	return m_infectionStatus;
}

void Human::incInfectionDuration()
{
	m_infectionCount++;
}

////////////////////
///// PENELOPE /////
////////////////////

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
	if (getInfectionStatus())
		incInfectionDuration();
	// Check if Penelope is dead
	if (getInfectionDuration() >= 500)
	{
		setDead();
		world()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	int ch;
	if (world()->getKey(ch))
	{
		double dest_x = getX();
		double dest_y = getY();
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_SPACE:
			if (getNumFlameCharges() > 0)
			{
				m_flamethrowerCharges--;
				world()->playSound(SOUND_PLAYER_FIRE);
				int direction = getDirection();
				// Chose i to loop from 1 to 3 as this was easier to code
				for (int i = 1; i < 4; i++)
				{
					if (direction == up)
					{
						if (world()->isFlameBlockedAt(this, getX(), getY() + (i*SPRITE_HEIGHT)))
							break;
						else
							world()->addActor(new Flame(world(), getX(), getY() + (i*SPRITE_HEIGHT) , getDirection()));
					}
					else if (direction == down)
					{
						if (world()->isFlameBlockedAt(this, getX(), getY() - (i*SPRITE_HEIGHT)))
							break;
						else
							world()->addActor(new Flame(world(), getX(), getY() - (i*SPRITE_HEIGHT), getDirection()));
					}
					else if (direction == left)
					{
						if (world()->isFlameBlockedAt(this, getX() - (i*SPRITE_WIDTH), getY()))
							break;
						else
							world()->addActor(new Flame(world(), getX() - (i*SPRITE_WIDTH), getY() , getDirection()));
					}
					else if (direction == right)
					{
						if (world()->isFlameBlockedAt(this, getX() + (i*SPRITE_WIDTH), getY()))
							break;
						else
							world()->addActor(new Flame(world(), getX() + (i*SPRITE_WIDTH), getY(), getDirection()));
					}
				}
			}
			break;
		case KEY_PRESS_TAB:
			if (getNumLandmines() > 0)
			{
				world()->addActor(new Landmine(world(), getX(), getY()));
				m_landmines--;
			}
			break;
		case KEY_PRESS_ENTER:
			if (getNumVaccines() > 0)
			{
				clearInfection();
				m_vaccines--;
			}
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			dest_x -= 4;
			if (!world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			dest_x += 4;
			if (!world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			dest_y += 4;
			if (!world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			dest_y -= 4;
			if (!world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
				moveTo(dest_x, dest_y);
			break;
		default:
			break;
		}
	}
}

void Penelope::useExitIfAppropriate()
{
	if (world()->canExit())
	{
		world()->setLevelFinished();
		world()->playSound(SOUND_LEVEL_FINISHED);
	}
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
	world()->playSound(SOUND_PLAYER_DIE);
	setDead();
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

bool Penelope::triggersCitizens() const
{
	return true;
}

/////////////////
//// CITIZEN ////
/////////////////

Citizen::Citizen(StudentWorld* w, double x, double y)
	: Human(w, IID_CITIZEN, x, y),
	m_currentTick(0)
{}

int Citizen::getCurrentTick()
{
	return m_currentTick;
}
void Citizen::nextTick()
{
	m_currentTick++;
}

void Citizen::decideMovementPlan()
{
	// TODO: CITIZEN SHOULD REMAIN STILL UNLESS CLOSE TO PENELOPE OR ZOMBIE
	// I think that the distance threshold is set too high

	// Create directions array
	int DIRS[] = { up, down, left, right };

	// Pointless initial values. Will be changed during function call.
	double other_x = getX();
	double other_y = getY();
	double distance = 0;
	bool isThreat = false;
	world()->locateNearestCitizenTrigger(this, other_x, other_y, distance, isThreat);
	if (!isThreat && distance <= (80 * 80))
	{
		// The direction is chosen to be one that would cause the
		// citizen to get closer to Penelope
		// If the citizen is on the same row or column as Penelope,
		// choose the(only) direction that gets the citizen closer
		if (other_x == getX())
		{
			if (other_y > getY())
			{
				setDirection(up);
			}
			else
				setDirection(down);
			if (move())
				return;
		}
		else if (other_y == getY())
		{
			if (other_x > getX())
			{
				setDirection(right);
			}
			else
				setDirection(left);
			if (move())
				return;
		}
		else
			// Otherwise, choose randomly between the two directions
			// (one horizontal and one vertical) that get the citizen closer
		{
			// Set possible directions
			int possibleDirs[2];
			if (other_x > getX())
			{
				possibleDirs[0] = right;
				if (other_y > getY())
					possibleDirs[1] = up;
				else
					possibleDirs[1] = down;
			}
			else
			{
				possibleDirs[0] = left;
				if (other_y > getY())
					possibleDirs[1] = up;
				else
					possibleDirs[1] = down;
			}
			// Attempt to move in that direction
			int direction = possibleDirs[randInt(0, 1)];
			setDirection(direction);
			if (move())
				return;
			else
			// Moving in that direction didn't work
			// Attempt to move in that direction
			{
				setDirection(1-direction);
				if (move())
					return;
			}
		}
	}
	if (isThreat && distance <= (80*80))
	{
		// The direction is chosen to be one that would cause the
		// citizen to get further from the zombie
		// If the citizen is on the same row or column as the zombie,
		// choose the (only) direction that gets the citizen further

		// We know that the distance has to be <= 80
		// so we can set the starting value of minDistance to 81
		double minDistance = 81;
		double dest_x = getX();
		double dest_y = getY();
		double other_x;
		double other_y;
		double otherDistance = 81;
		int direction = DIRS[randInt(0,3)];
		// For each of the four directions
		for (int i = 0; i < 4; i++)
		{
			switch (DIRS[i])
			{
			case left:
				dest_x -= 2;
				break;
			case right:
				dest_x += 2;
				break;
			case up:
				dest_y += 2;
				break;
			case down:
				dest_y -= 2;
				break;
			}
			// Determine if the citizen is blocked from moving 2 pixels in that direction
			// Determine the distance to the nearest zombie
			if (!(world()->isAgentMovementBlockedAt(this, dest_x, dest_y)) &&
				world()->locateNearestCitizenThreat(dest_x, dest_y, other_x, other_y, otherDistance))
			{
				if (otherDistance < minDistance)
				{
					minDistance = otherDistance;
					direction = DIRS[i];
				}
			}
		}
		// Set the citizen’s direction to the direction that will take it furthest
		// away from the nearest zombie.
		setDirection(direction);
		// Move 2 pixels in that direction using the GraphObject class's
		// moveTo() method.
		move();
	}
	// If the distance to the selected nearest person is more than 80 pixels
	// away, the citizen does nothing
}

bool Citizen::move()
{
	// Attempt to move citizen in a certain direction
	// Zombies move by one pixel, Citizens move by two
	double dest_x = getX();
	double dest_y = getY();
	switch (getDirection())
	{
	case left:
		dest_x -= 2;
		break;
	case right:
		dest_x += 2;
		break;
	case up:
		dest_y += 2;
		break;
	case down:
		dest_y -= 2;
		break;
	}
	if (!world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
	{
		moveTo(dest_x, dest_y);
		return true;
	}
	else
		return false;
}


void Citizen::doSomething()
{
	// Increase tick counter
	nextTick();
	// Check if Citizen is dead
	if (isDead())
		return;
	if (getInfectionStatus())
	{
		incInfectionDuration();
		if (getInfectionDuration() >= 500)
		{
			dieByFallOrBurnIfAppropriate();
			int n = randInt(1, 10);
			// 30% chance of smart zombie
			if (1 <= n && n <= 3)
			{
				world()->addActor(new SmartZombie(world(), getX(), getY()));
			}
			else
				world()->addActor(new DumbZombie(world(), getX(), getY()));
			world()->playSound(SOUND_ZOMBIE_BORN);
			return;
		}
	}
	// Check if Citizen is paralized
	if (getCurrentTick() % 2 == 0)
		return;
	decideMovementPlan();
}

void Citizen::beVomitedOnIfAppropriateHelper()
{
	if (!(getInfectionStatus()))
		world()->playSound(SOUND_CITIZEN_INFECTED);
}

void Citizen::useExitIfAppropriate()
{
	world()->increaseScore(500);
	setDead();
	world()->recordCitizenGone();
	world()->playSound(SOUND_CITIZEN_SAVED);
}

void Citizen::dieByFallOrBurnIfAppropriate()
{
	setDead();
	world()->recordCitizenGone();
	world()->playSound(SOUND_CITIZEN_DIE);
	world()->increaseScore(-1000);
}

//////////////////
///// ZOMBIE /////
//////////////////

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
	else
		return true;
}
void Zombie::move()
{
	// Attempt to move zombie in a certain direction
	double dest_x = getX();
	double dest_y = getY();
	switch (getDirection())
	{
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
	if (!world()->isAgentMovementBlockedAt(this, dest_x, dest_y))
	{
		moveTo(dest_x, dest_y);
		decMovementPlanDistance();
	}
	else
		setMovementPlanDistance(0);
}

bool Zombie::triggersCitizens() const
{
	return true;
}

bool Zombie::threatensCitizens() const
{
	return true;
}

bool Zombie::vomit()
{
	int direction = getDirection();
	int randomInteger = randInt(1, 3);
	// There is a one in three chance that the zombie will vomit
	if (randomInteger == 1)
	{
		if (direction == up)
		{
			if (world()->isZombieVomitTriggerAt(this, getX(), getY() + SPRITE_HEIGHT))
			{
				world()->addActor(new Vomit(world(), getX(), getY() + SPRITE_HEIGHT, getDirection()));
				world()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		}
		else if (direction == down)
		{
			if (world()->isZombieVomitTriggerAt(this, getX(), getY() - SPRITE_HEIGHT))
			{
				world()->addActor(new Vomit(world(), getX(), getY() - SPRITE_HEIGHT, getDirection()));
				world()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		}
		else if (direction == left)
		{
			if (world()->isZombieVomitTriggerAt(this, getX() - SPRITE_WIDTH, getY()))
			{
				world()->addActor(new Vomit(world(), getX() - SPRITE_WIDTH, getY(), getDirection()));
				world()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		}
		else if (direction == right)
		{
			if (world()->isZombieVomitTriggerAt(this, getX() + SPRITE_WIDTH, getY()))
			{
				world()->addActor(new Vomit(world(), getX() + SPRITE_WIDTH, getY(), getDirection()));
				world()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		}
	}
	return false;
}

///////////////////////
///// DUMB ZOMBIE /////
///////////////////////

// TODO: DUMB ZOMBIES DROP VACCINES SOMETIMES

DumbZombie::DumbZombie(StudentWorld* w, double x, double y)
	: Zombie(w, x, y)
{}

void DumbZombie::doSomething()
{
	// TODO: BETTER INHERITANCE
	if (!startDoSomething())
		return;
	if (vomit())
		return;
	// Check if the zombie needs a new movement plan
	decideMovementPlan();
	// move
	move();
}
void DumbZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	world()->playSound(SOUND_ZOMBIE_DIE);
	world()->increaseScore(1000);
	// Attempt to drop Vaccine
	int randomInteger = randInt(1, 10);
	// 1 in 10 Dumb Zombies has a vaccine
	if (randomInteger == 1)
	{
		int DIRS[] = { up, down, left, right };
		int direction = DIRS[randInt(0, 3)];
		double dest_x = getX();
		double dest_y = getY();
		switch (direction)
		{
		case left:
			dest_x -= SPRITE_WIDTH;
			break;
		case right:
			dest_x += SPRITE_WIDTH;
			break;
		case up:
			dest_y += SPRITE_HEIGHT;
			break;
		case down:
			dest_y -= SPRITE_HEIGHT;
			break;
		}
		if (!world()->isVaccineBlockedAt(this, dest_x, dest_y))
			world()->addActor(new VaccineGoodie(world(), dest_x, dest_y));
		// Sometimes, the vaccine get immediately destroyed by the flames in the air
	}
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

////////////////////////
///// SMART ZOMBIE /////
////////////////////////

SmartZombie::SmartZombie(StudentWorld* w, double x, double y)
	: Zombie(w, x, y)
{}

void SmartZombie::doSomething()
{
	// TODO: BETTER INHERITANCE
	if (!startDoSomething())
		return;
	if (vomit())
		return;
	// Check if the zombie needs a new movement plan
	decideMovementPlan();
	// move
	move();
}
void SmartZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	world()->playSound(SOUND_ZOMBIE_DIE);
	world()->increaseScore(2000);
}

void SmartZombie::decideMovementPlan()
{
	// Check if the zombie needs a new movement plan
	if (getMovementPlanDistance() == 0)
	{
		// Create directions array
		int DIRS[] = { up, down, left, right };
		
		setMovementPlanDistance(randInt(3, 10));
		// Pointless initial values. Will be changed during function call.
		double other_x = getX();
		double other_y = getY();
		double distance = 0;

		if (world()->locateNearestVomitTrigger(this, other_x, other_y, distance) && distance <= (80*80))
		{
			// The direction is chosen to be one that would cause the
			// zombie to get closer to the person
			// If the zombie is on the same row or column as the person,
			// choose the(only) direction that gets the zombie closer
			if (other_x == getX())
			{
				if (other_y > getY())
				{
					setDirection(up);
				}
				else
					setDirection(down);
			}
			else if (other_y == getY())
			{
				if (other_x > getX())
				{
					setDirection(right);
				}
				else
					setDirection(left);
			}
			else
				// Otherwise, choose randomly between the two directions
				// (one horizontal and one vertical) that get the zombie closer
			{
				// Set possible directions
				int possibleDirs[2];
				if (other_x > getX())
				{
					possibleDirs[0] = right;
					if (other_y > getY())
						possibleDirs[1] = up;
					else
						possibleDirs[1] = down;
				}
				else
				{
					possibleDirs[0] = left;
					if (other_y > getY())
						possibleDirs[1] = up;
					else
						possibleDirs[1] = down;
				}
				setDirection(possibleDirs[randInt(0, 1)]);
			}
		}
		else
		{
			// If the distance to the selected nearest person is more than 80 pixels
			// away, the direction is randomly chosen from up, down, left, and right.
			setDirection(DIRS[randInt(0, 3)]);
		}

	}
}

///////////////////////////////////////////////////////////////////////////////////////
