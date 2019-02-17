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
	m_pen = new Penelope(200, 100);
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
}
