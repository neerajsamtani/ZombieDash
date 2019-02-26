#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

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
	Level lev(assetPath());
	m_levelFinished = false;
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
				int x = level_x * SPRITE_WIDTH;
				int y = level_y * SPRITE_HEIGHT;
				switch (ge)
				{
				case Level::empty:
					break;
				case Level::citizen:
					//cnt++;
					// addActor(new Citizen(this, x, y));
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
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		if ((*p)->isDead())
		{
			delete *p;
			list<Actor*>::iterator q = actors.erase(p);
			p = q;
		}
		else
			(*p)->doSomething();
	}
	if (m_pen->isDead())
		return GWSTATUS_PLAYER_DIED;
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

bool StudentWorld::isAgentMovementBlockedAt(Actor* curActor, double dest_x, double dest_y)
{
	// Create bounding box for destination
	int dest_x_start = dest_x;
	int dest_x_end = (dest_x + SPRITE_WIDTH - 1);
	int dest_y_start = dest_y;
	int dest_y_end = (dest_y + SPRITE_HEIGHT - 1);

	// Check if the destination x and y coordinates are in Penelope's bounding box
	if (curActor != m_pen)
	{
		// Create bounding box for current actor
		int pen_x_start = m_pen->getX();
		int pen_x_end = (m_pen->getX() + SPRITE_WIDTH - 1);
		int pen_y_start = m_pen->getY();
		int pen_y_end = (m_pen->getY() + SPRITE_HEIGHT - 1);

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
			return false;
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
			int actor_x_start = (*p)->getX();
			int actor_x_end = ((*p)->getX() + SPRITE_WIDTH - 1);
			int actor_y_start = (*p)->getY();
			int actor_y_end = ((*p)->getY() + SPRITE_HEIGHT - 1);

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
				return false;
			}
		}
	}
	return true;
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

int euclidianDistance(Actor* A, Actor* B)
{
	// TODO: Double check the following note
	// Note that distance between centers is equal to distance between corners
	int deltaX = (A->getX()) - (B->getX());
	int deltaY = (A->getY()) - (B->getY());

	return ((deltaX)*(deltaX)) + ((deltaY)*(deltaY));
}

int StudentWorld::dirOfClosestPerson(Actor* curActor)
{
	const int right = curActor->right;
	const int left = curActor->left;
	const int up = curActor->up;
	const int down = curActor->down;
	const int DIRS[] = { right, left, up, down };
	// Select closest person
	// Assume Penelope is the closest
	int minDistance = euclidianDistance(curActor, m_pen);
	Actor* closestActor = m_pen;
	// Check if other actors are closer
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		// Ensure that the zombie doesn't pick itself 
		// and check if the character can be infected
		if (*p != curActor && (*p)->triggersZombieVomit())
		{
			int distance = euclidianDistance(curActor, *p);
			if (distance < minDistance)
			{
				minDistance = distance;
				closestActor = *p;
			}
		}
	}
	// If the distance to the selected nearest person is more than 80 pixels
	// away, the direction is chosen from up, down, left, and right.
	if (euclidianDistance(curActor, closestActor) > (80 * 80))
		return (DIRS[randInt(0, 3)]);
	// Otherwise, the direction is chosen to be one that would cause the
	// zombie to get closer to the person
	else
	{
		// If the zombie is on the same row or column as the person,
		// choose the(only) direction that gets the zombie closer
		if (closestActor->getX() == curActor->getX())
		{
			if (closestActor->getY() > curActor->getY())
			{
				return (up);
			}
			else
				return (down);
		}
		else if (closestActor->getY() == curActor->getY())
		{
			if (closestActor->getX() > curActor->getX())
			{
				return (right);
			}
			else
				return (left);
		}
		else
		// Otherwise, choose randomly between the two directions
		// (one horizontal and one vertical) that get the zombie closer
		{
			// Set possible directions
			int possibleDirs[2];
			if (closestActor->getX() > curActor->getX())
			{
				possibleDirs[0] = right;
				if (closestActor->getY() > curActor->getY())
					possibleDirs[1] = up;
				else
					possibleDirs[1] = down;
			}
			else
			{
				possibleDirs[0] = left;
				if (closestActor->getY() > curActor->getY())
					possibleDirs[1] = up;
				else
					possibleDirs[1] = down;
			}
			return (possibleDirs[randInt(0, 1)]);
		}

	}
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