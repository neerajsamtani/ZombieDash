#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <list>
using namespace std;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	// ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	void loadCurrentLevel();
private:
	Penelope *m_pen;
	// Wall *m_wall;
	list<Actor*> actors;
};

#endif // STUDENTWORLD_H_
