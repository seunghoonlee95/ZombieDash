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
    bool doesIntersect(Actor* sameActor, int x, int y);
    bool doesOverlap(int x, int y);
    void foo(){
        cout << "for testing..." << endl;
    }
    int testInt = 100;
private:
    vector<Actor*> actorList;
    Penelope* playerPtr;
};

#endif // STUDENTWORLD_H_
