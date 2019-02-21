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
		cerr << "Your level was improperly formatted" << endl;
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
				switch (ge) // TODO: Add other actors
				{
				case Level::empty:
					break;
				case Level::citizen:
					break;
				case Level::vaccine_goodie:
					break;
				case Level::gas_can_goodie:
					break;
				case Level::landmine_goodie:
					break;
				case Level::smart_zombie:
					break;
				case Level::dumb_zombie:
					break;
				case Level::player:
					cerr << "Creating Penelope" << endl;
					m_pen = new Penelope(x, y, this);
					break;
				case Level::exit:
					cnt++;
					actors.push_back(new Exit(x, y, this));
					break;
				case Level::wall:
					cnt++;
					actors.push_back(new Wall(x, y, this));
					break;
				case Level::pit:
					break;
					// TODO: etc…
				}
			}
		}
		cerr << "Created " << cnt << " actors" << endl;
		cerr << "Size of list " << actors.size() << endl;
		return GWSTATUS_CONTINUE_GAME;
	}
	return GWSTATUS_PLAYER_WON;
	// TODO: FIX DESTRUCTOR
}

int StudentWorld::move()
{
	// TODO: How to win game?
	m_pen->doSomething();
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
		(*p)->doSomething();
	if (!(m_pen->isAlive()))
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

bool StudentWorld::locationEmpty(int dest_x, int dest_y)
{
	// Create bounding box for destination
	int dest_x_start = dest_x;
	int dest_x_end = (dest_x + SPRITE_WIDTH - 1);
	int dest_y_start = dest_y;
	int dest_y_end = (dest_y + SPRITE_HEIGHT - 1);

	// Check if the destination x and y coordinates are in any actor's bounding box
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
	{
		// Penelope cannot walk through solid objects
		if ((*p)->isSolidObject())
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

bool StudentWorld::objectOverlap(Actor* A, Actor* B)
{
	// TODO: Double check the following note
	// Note that distance between centers is equal to distance between corners
	int deltaX = (A->getX()) - (B->getX());
	int deltaY = (A->getY()) - (B->getY());

	if (((deltaX)*(deltaX)) + ((deltaY)*(deltaY)) <= 100)
		return true;
	else
		return false;
}

bool StudentWorld::exitPen(Actor* exitPtr)
{
	if (objectOverlap(m_pen, exitPtr))
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