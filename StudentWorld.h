#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <list>
using namespace std;

class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
	/*

	// Record that one more citizen on the current level is gone (exited,
	// died, or turned into a zombie).
	void recordCitizenGone();

	// Indicate that the player has finished the level if all citizens
	// are gone.
	void recordLevelFinishedIfAllCitizensGone();

	// For each actor overlapping a, activate a if appropriate.
	void activateOnAppropriateActors(Actor* a);

	// Is creation of a flame blocked at the indicated location?
	bool isFlameBlockedAt(double x, double y) const;

	// Is there something at the indicated location that might cause a
	// zombie to vomit (i.e., a human)?
	bool isZombieVomitTriggerAt(double x, double y) const;

	// Return true if there is a living human, otherwise false.  If true,
	// otherX, otherY, and distance will be set to the location and distance
	// of the human nearest to (x,y).
	bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);

	// Return true if there is a living zombie or Penelope, otherwise false.
	// If true, otherX, otherY, and distance will be set to the location and
	// distance of the one nearest to (x,y), and isThreat will be set to true
	// if it's a zombie, false if a Penelope.
	bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const;

	// Return true if there is a living zombie, false otherwise.  If true,
	// otherX, otherY and distance will be set to the location and distance
	// of the one nearest to (x,y).
	bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const;

	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	// Add an actor to the world.
	void addActor(Actor* a);

	// Determines blocking of movement
	bool isAgentMovementBlockedAt(Actor* curActor, double dest_x, double dest_y);

	// Determines objectOverlap
	bool objectOverlap(Actor* A, Actor* B);

	// Determines if any actor can exit
	bool exitPen(Actor* exitPtr);
	// TODO: exitCitizen

	void setLevelFinished();
	bool getLevelFinished();

	int dirOfClosestPerson(Actor* curActor);

private:
	Penelope *m_pen;
	list<Actor*> actors;
	bool m_levelFinished;
};

#endif // STUDENTWORLD_H_
