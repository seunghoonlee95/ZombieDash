#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include <iostream>

#include "GraphObject.h"    //I added this

using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;
class Zombie;
//class Human;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    
    virtual ~StudentWorld(){
        cleanUp();
    }
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool doesIntersect(Actor* actorPtr, double destX, double destY);
    bool doesOverlap(double actorX, double actorY, double otherX, double otherY);
    void escapeHumans(double exitX, double exitY);
    bool doesOverlapWithPlayer(double actorX, double actorY);
    void blastFlame();
    void explodeMine(Actor* minePtr, bool triggeredByFlame);
    void fallIntoPit(Actor* pitPtr);
    double determineDistToPenelope(Actor* actorPtr);
    double determineDistToClosestZombie(double citizenX, double citizenY);
    
    bool throwVomit(Zombie* zombiePtr);
    bool findClosestPersonAndFollow(Zombie* zombiePtr);
    void followPenelope(Actor* actorPtr, int moveDistance);
    void citizenBecomeZombie(Actor* citizenPtr);
    void runaway(Actor* citizenPtr);

    void incrementVaccine();
    void incrementFlameCount();
    void incrementLandmineCount();
    void plantLandmine();
    void damageObjects(Actor* flamePtr);
    void infectObjects(Actor* vomitPtr);
    
private:
    vector<Actor*> actorList;
    Penelope* playerPtr;

protected:
    bool m_finishedLevel;
    void followActor(Actor* followerPtr, Actor* destPtr, int moveDistance);
    bool getFinishedLevel(){return m_finishedLevel;}
    void setFinishedLevel(bool status){m_finishedLevel = status;}
    void setSmartZombieDirection(Actor* zombiePtr, Actor* actorPtr);
    bool moveActor(Actor* actorPtr, Direction dir, int moveDistance);

};

#endif // STUDENTWORLD_H_

