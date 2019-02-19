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

	bool locationEmpty(int dest_x, int dest_y);

private:
	Penelope *m_pen;
	list<Actor*> actors;
};

#endif // STUDENTWORLD_H_
