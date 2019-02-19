#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{
	// LESSON: Only constructors can use intializer lists.

	Level lev(assetPath());
	string levelFile = "level01.txt"; // TODO: get current level
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
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
					m_pen = new Penelope(x, y, this);
					break;
				case Level::exit:
					break;
				case Level::wall:
					actors.push_back(new Wall(x, y, this));
					break;
				case Level::pit:
					break;
					// TODO: etc…
				}
			}
		}

	}

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_pen->doSomething();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
    // return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
	if (m_pen != nullptr) // TODO: Is this check necessary?
		delete m_pen;
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
		delete *p;
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