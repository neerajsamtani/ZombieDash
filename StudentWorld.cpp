#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

double euclidianDistance(Actor* A, Actor* B)
{
	// TODO: Double check the following note
	// Note that distance between centers is equal to distance between corners
	double deltaX = (A->getX()) - (B->getX());
	double deltaY = (A->getY()) - (B->getY());

	return ((deltaX)*(deltaX)) + ((deltaY)*(deltaY));
}

double euclidianDistance(double x, double y, Actor* B)
{
	// TODO: Double check the following note
	// Note that distance between centers is equal to distance between corners
	double deltaX = (x) - (B->getX());
	double deltaY = (y) - (B->getY());

	return ((deltaX)*(deltaX)) + ((deltaY)*(deltaY));
}

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	m_pen = nullptr;
}

// TODO: Winning the game destructs twice
StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{
	// LESSON: Only constructors can use intializer lists.
	// TODO: Remove cerr
	m_numCitizens = 0;
	m_canExit = false;
	m_levelFinished = false;

	Level lev(assetPath());
	ostringstream oss;
	oss << "level0" << getLevel() << ".txt";
	string levelFile = oss.str();

	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find " << "level0" << getLevel() << ".txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl; // TODO: CHECK IF LEVEL IS SURROUNDED BY WALLS
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded " << "level0" << getLevel() << ".txt" << endl;
		int cnt = 0;
		for (int level_x = 0; level_x < SPRITE_WIDTH; level_x++)
		{
			for (int level_y = 0; level_y < SPRITE_HEIGHT; level_y++)
			{
				Level::MazeEntry ge = lev.getContentsOf(level_x, level_y);
				double x = level_x * SPRITE_WIDTH;
				double y = level_y * SPRITE_HEIGHT;
				switch (ge)
				{
				case Level::empty:
					break;
				case Level::citizen:
					cnt++;
					m_numCitizens++;
					addActor(new Citizen(this, x, y));
					break;
				case Level::vaccine_goodie:
					cnt++;
					addActor(new VaccineGoodie(this, x, y));
					break;
				case Level::gas_can_goodie:
					cnt++;
					addActor(new GasCanGoodie(this, x, y));
					break;
				case Level::landmine_goodie:
					cnt++;
					addActor(new LandmineGoodie(this, x, y));
					break;
				case Level::smart_zombie:
					cnt++;
					addActor(new SmartZombie(this, x, y));
					break;
				case Level::dumb_zombie:
					cnt++;
					addActor(new DumbZombie(this, x, y));
					break;
				case Level::player:
					cerr << "Creating Penelope" << endl;
					m_pen = new Penelope(this, x, y);
					break;
				case Level::exit:
					cnt++;
					addActor(new Exit(this, x, y));
					break;
				case Level::wall:
					cnt++;
					addActor(new Wall(this, x, y));
					break;
				case Level::pit:
					break;
				}
			}
		}
		recordLevelFinishedIfAllCitizensGone();
		cerr << "Created " << cnt << " actors" << endl;
		cerr << "Size of list " << actors.size() << endl;
		return GWSTATUS_CONTINUE_GAME;
	}
	return GWSTATUS_PLAYER_WON;
}

void StudentWorld::addActor(Actor * a)
{
	actors.push_back(a);
}

int StudentWorld::move()
{
	// TODO: How to win game?
	m_pen->doSomething();
	list<Actor*>::iterator p = actors.begin();
	while (p != actors.end())
	{
		if ((*p)->isDead())
		{
			delete *p;
			list<Actor*>::iterator q = actors.erase(p);
			p = q;
		}
		else
		{
			(*p)->doSomething();
			p++;
		}
	}

	// Set Status Text
	ostringstream oss;
	oss.fill('0');
	oss << "Score: "<< setw(5) << getScore();
	oss << "  Level: " << getLevel();
	oss << "  Lives : " << getLives();
	oss << "  Vaccines : " << m_pen->getNumVaccines();
	oss << "  Flames : " << m_pen->getNumFlameCharges();
	oss << "  Mines : " << m_pen->getNumLandmines();
	oss << "  Infected : 0";// TODO: Get Number Infected
	//cerr << oss.str() << endl;
	setGameStatText(oss.str());

	if (m_pen->isDead())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	else if (getLevelFinished())
		return GWSTATUS_FINISHED_LEVEL;
	else
		return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	// TODO: Remove cerr lines

	cerr << "Ending Level " << getLevel()-1 << endl;
	if (m_pen != nullptr) // TODO: Is this check necessary?
	{
		cerr << "Deleting Penelope" << endl;
		delete m_pen;
		m_pen = nullptr;
		// cerr << "Deleted Penelope" << endl;
	}
	int cnt = 0;
	// cerr << "Set count to 0" << endl;
	list<Actor*>::iterator p = actors.begin();
	// cerr << "Initialized p" << endl;
	while (p != actors.end())
	{
		cnt++;
		if (*p != nullptr)
			delete *p;
		list<Actor*>::iterator q = actors.erase(p);
		p = q;
	}
	cerr << "Deleted " << cnt << " Actors" << endl;
}

bool StudentWorld::isAgentMovementBlockedAt(Actor* curActor, double x, double y)
{
	// Create bounding box for destination
	double dest_x_start = x;
	double dest_x_end = (x + SPRITE_WIDTH - 1);
	double dest_y_start = y;
	double dest_y_end = (y + SPRITE_HEIGHT - 1);

	// Check if the destination x and y coordinates are in Penelope's bounding box
	if (curActor != m_pen)
	{
		// Create bounding box for current actor
		double pen_x_start = m_pen->getX();
		double pen_x_end = (m_pen->getX() + SPRITE_WIDTH - 1);
		double pen_y_start = m_pen->getY();
		double pen_y_end = (m_pen->getY() + SPRITE_HEIGHT - 1);

		// Check if the bouding boxes overlap
		if ((pen_x_start <= dest_x_start && dest_x_start <= pen_x_end &&
			pen_y_start <= dest_y_start && dest_y_start <= pen_y_end)
			||
			(pen_x_start <= dest_x_end && dest_x_end <= pen_x_end &&
				pen_y_start <= dest_y_end && dest_y_end <= pen_y_end)
			||
			(pen_x_start <= dest_x_start && dest_x_start <= pen_x_end &&
				pen_y_start <= dest_y_end && dest_y_end <= pen_y_end)
			||
			(pen_x_start <= dest_x_end && dest_x_end <= pen_x_end &&
				pen_y_start <= dest_y_start && dest_y_start <= pen_y_end)
			)
		{
			return true;
		}
	}

	// Check if the destination x and y coordinates are in any other actor's bounding box
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		if (*p == curActor)
			continue;
		// Actors cannot walk through solid objects
		if ((*p)->blocksMovement())
		{

			// Create bounding box for current actor
			double actor_x_start = (*p)->getX();
			double actor_x_end = ((*p)->getX() + SPRITE_WIDTH - 1);
			double actor_y_start = (*p)->getY();
			double actor_y_end = ((*p)->getY() + SPRITE_HEIGHT - 1);

			// Check if the bouding boxes overlap
			if ((actor_x_start <= dest_x_start && dest_x_start <= actor_x_end &&
				actor_y_start <= dest_y_start && dest_y_start <= actor_y_end)
				||
				(actor_x_start <= dest_x_end && dest_x_end <= actor_x_end &&
					actor_y_start <= dest_y_end && dest_y_end <= actor_y_end)
				||
				(actor_x_start <= dest_x_start && dest_x_start <= actor_x_end &&
					actor_y_start <= dest_y_end && dest_y_end <= actor_y_end)
				||
				(actor_x_start <= dest_x_end && dest_x_end <= actor_x_end &&
					actor_y_start <= dest_y_start && dest_y_start <= actor_y_end)
				)
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isFlameBlockedAt(Actor* curActor, double x, double y)
{
	// Create bounding box for destination
	double dest_x_start = x;
	double dest_x_end = (x + SPRITE_WIDTH - 1);
	double dest_y_start = y;
	double dest_y_end = (y + SPRITE_HEIGHT - 1);

	// Check if the destination x and y coordinates are in any other actor's bounding box
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		if (*p == curActor)
			continue;
		// Flames cannot go through certain objects
		if ((*p)->blocksFlame())
		{

			// Create bounding box for current actor
			double actor_x_start = (*p)->getX();
			double actor_x_end = ((*p)->getX() + SPRITE_WIDTH - 1);
			double actor_y_start = (*p)->getY();
			double actor_y_end = ((*p)->getY() + SPRITE_HEIGHT - 1);

			// Check if the bouding boxes overlap
			if ((actor_x_start <= dest_x_start && dest_x_start <= actor_x_end &&
				actor_y_start <= dest_y_start && dest_y_start <= actor_y_end)
				||
				(actor_x_start <= dest_x_end && dest_x_end <= actor_x_end &&
					actor_y_start <= dest_y_end && dest_y_end <= actor_y_end)
				||
				(actor_x_start <= dest_x_start && dest_x_start <= actor_x_end &&
					actor_y_start <= dest_y_end && dest_y_end <= actor_y_end)
				||
				(actor_x_start <= dest_x_end && dest_x_end <= actor_x_end &&
					actor_y_start <= dest_y_start && dest_y_start <= actor_y_end)
				)
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isZombieVomitTriggerAt(Actor* curActor, double x, double y)
{
	// Create bounding box for destination
	double dest_x_start = x;
	double dest_x_end = (x + SPRITE_WIDTH - 1);
	double dest_y_start = y;
	double dest_y_end = (y + SPRITE_HEIGHT - 1);

	// Check if the destination x and y coordinates are in any other actor's bounding box
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		// Only certain actors trigger zombie vomit
		if ((*p)->triggersZombieVomit())
		{

			// Create bounding box for current actor
			double actor_x_start = (*p)->getX();
			double actor_x_end = ((*p)->getX() + SPRITE_WIDTH - 1);
			double actor_y_start = (*p)->getY();
			double actor_y_end = ((*p)->getY() + SPRITE_HEIGHT - 1);

			// Check if the bouding boxes overlap
			if ((actor_x_start <= dest_x_start && dest_x_start <= actor_x_end &&
				actor_y_start <= dest_y_start && dest_y_start <= actor_y_end)
				||
				(actor_x_start <= dest_x_end && dest_x_end <= actor_x_end &&
					actor_y_start <= dest_y_end && dest_y_end <= actor_y_end)
				||
				(actor_x_start <= dest_x_start && dest_x_start <= actor_x_end &&
					actor_y_start <= dest_y_end && dest_y_end <= actor_y_end)
				||
				(actor_x_start <= dest_x_end && dest_x_end <= actor_x_end &&
					actor_y_start <= dest_y_start && dest_y_start <= actor_y_end)
				)
			{
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::activateOnAppropriateActors(Actor* a)
{
	if (objectOverlap(m_pen, a))
	{
		a->activateIfAppropriate(m_pen);
	}
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		if (objectOverlap(*p, a))
		{
			a->activateIfAppropriate(*p);
		}
	}
}

bool StudentWorld::locateNearestVomitTrigger(Actor* curActor, double& otherX, double& otherY, double& distance)
{
	// Select closest person
	// Assume Penelope is the closest
	if (!m_pen->isDead())
	{
		double minDistance = euclidianDistance(curActor, m_pen);
		Actor* closestActor = m_pen;
		// Check if other actors are closer
		for (list<Actor*>::iterator p = actors.begin();
			p != actors.end(); p++)
		{
			// The zombie doesn't pick itself 
			// Check if the character can be infected and is alive
			if ((*p)->triggersZombieVomit() && !((*p)->isDead()))
			{
				double tempDistance = euclidianDistance(curActor, *p);
				if (tempDistance < minDistance)
				{
					minDistance = tempDistance;
					closestActor = *p;
				}
			}
		}
		otherX = closestActor->getX();
		otherY = closestActor->getY();
		distance = minDistance;
		return true;
	}
	else
		// If Penelope is dead, the level ends and this function will not be called
		return false;

}

bool StudentWorld::locateNearestCitizenTrigger(Actor* curActor, double& otherX, double& otherY, double& distance, bool& isThreat)
{
	// Select closest person
	// Assume Penelope is the closest
	if (!m_pen->isDead())
	{
		double minDistance = euclidianDistance(curActor, m_pen);
		Actor* closestActor = m_pen;
		// Check if other actors are closer
		for (list<Actor*>::iterator p = actors.begin();
			p != actors.end(); p++)
		{
			// Check if the character triggers citizens and is alive
			if ((*p)->triggersCitizens() && !((*p)->isDead()))
			{
				double tempDistance = euclidianDistance(curActor, *p);
				if (tempDistance < minDistance)
				{
					minDistance = tempDistance;
					closestActor = *p;
				}
			}
		}
		otherX = closestActor->getX();
		otherY = closestActor->getY();
		distance = minDistance;
		isThreat = closestActor->threatensCitizens();
		return true;
	}
	else
		// If Penelope is dead, the level ends and this function will not be called
		return false;

}

bool StudentWorld::locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance)
{
	// Select closest person
	double minDistance;
	Actor* closestActor = nullptr;
	// Check if other actors are closer
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		// Check if the character triggers citizens and is alive
		if (closestActor == nullptr && (*p)->threatensCitizens() && !((*p)->isDead()))
		{
			closestActor = *p;
			minDistance = euclidianDistance(x, y, *p);
		}
		else if ((*p)->threatensCitizens() && !((*p)->isDead()))
		{
			double tempDistance = euclidianDistance(x, y, *p);
			if (tempDistance < minDistance)
			{
				minDistance = tempDistance;
				closestActor = *p;
			}
		}
	}
	if (closestActor != nullptr)
	{
		otherX = closestActor->getX();
		otherY = closestActor->getY();
		distance = minDistance;
		return true;
	}
	else
		return false;
}

bool StudentWorld::objectOverlap(Actor* A, Actor* B)
{
	if (euclidianDistance(A, B) <= 100)
		return true;
	else
		return false;
}

void StudentWorld::setLevelFinished()
{
	m_levelFinished = true;
}

bool StudentWorld::getLevelFinished()
{
	return m_levelFinished;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
	if (m_numCitizens <= 0)
		m_canExit = true;
}

void StudentWorld::recordCitizenGone()
{
	m_numCitizens--;
	recordLevelFinishedIfAllCitizensGone();
}

bool StudentWorld::canExit()
{
	return m_canExit;
}