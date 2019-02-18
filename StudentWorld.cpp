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

int StudentWorld::init()
{
	// LESSON: Only constructors can use intializer lists.
	loadCurrentLevel();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_pen->doSomething();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
	return GWSTATUS_CONTINUE_GAME;
    // return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
	delete m_pen;
	for (list<Actor*>::iterator p = actors.begin();
		p != actors.end(); p++)
		delete *p;
}

void StudentWorld::loadCurrentLevel()
{
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
				switch (ge)
				{
				case Level::empty:
					break;
				case Level::smart_zombie:
					break;
				case Level::dumb_zombie:
					break;
				case Level::player:
					m_pen = new Penelope(x, y);
					break;
				case Level::exit:
					break;
				case Level::wall:
					actors.push_back(new Wall(x, y));
					break;
				case Level::pit:
					break;
					// etc…
				}
			}
		}

	}
}