#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;

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
    bool doesIntersect(Actor* sameActor, double x, double y);
    bool doesOverlap(Actor* sameActor, double x, double y);
    void escapeHumans(double exitX, double exitY);
    bool doesOverlapWithPlayer(Actor* goodie);
    void blastFlame();
    void explodeMine(Actor* landMinePtr, double mineX, double mineY);
    
    void setGotVaccine(bool status){m_gotVaccine = status;}
    bool getGotVaccine(){return m_gotVaccine;}
    bool getFinishedLevel(){return m_finishedLevel;}
    void setFinishedLevel(bool status){m_finishedLevel = status;}
    bool getGotFlames(){return m_gotFlames;}
    void setGotFlames(bool status){m_gotFlames = status;}
    
    
private:
    vector<Actor*> actorList;
    Penelope* playerPtr;
    bool m_finishedLevel;
    bool m_gotVaccine;
    bool m_gotFlames;
};

#endif // STUDENTWORLD_H_

