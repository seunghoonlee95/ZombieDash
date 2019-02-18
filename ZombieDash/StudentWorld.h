#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
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
    StudentWorld(); // I just made this default constructor up..
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool doesIntersect(int x, int y);
    bool doesOverlap(int x, int y);

private:
    vector<Actor*> actorList;
    vector<Penelope*> playerList;
};

#endif // STUDENTWORLD_H_
