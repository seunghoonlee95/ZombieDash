#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
//#include "Actor.h"

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    StudentWorld(); // I just made this default constructor up..
    virtual int init();
    virtual int move();
    virtual void cleanUp();
   // *StudentWorld

private:
   // vector<*Actor> actorList;
};

#endif // STUDENTWORLD_H_
