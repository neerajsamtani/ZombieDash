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
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	// Determines blocking of movement
	bool locationEmpty(int dest_x, int dest_y);

	// Determines objectOverlap
	bool objectOverlap(Actor* A, Actor* B);

	// Determines if any actor can exit
	bool exitPen(Actor* exitPtr);
	// TODO: exitCitizen

	void setLevelFinished();
	bool getLevelFinished();

private:
	Penelope *m_pen;
	list<Actor*> actors;
	bool m_levelFinished;
};

#endif // STUDENTWORLD_H_
