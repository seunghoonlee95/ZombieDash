#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

//Careful....
#include "StudentWorld.h"
//class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject{
public:
    //Simple Constructor
    Actor(StudentWorld* stdPtr, int imageID, double startX, double startY, Direction dir, int depth, double size)
    :GraphObject(imageID, startX, startY, dir, depth, size), m_stdPtr(stdPtr), passable(false), m_isAlive(true)
    {
    }
    virtual ~Actor(){
    }
    virtual void doSomething()=0;
    
    bool getPassable(){
        return passable;
    }
    void setPassable(bool passVal){
        passable = passVal;
    }
    bool getIsAlive(){
        return m_isAlive;
    }
    void setIsAlive(bool status){
        m_isAlive = status;
    }
    StudentWorld* getWorld() const{return m_stdPtr;}
private:
    StudentWorld* m_stdPtr;
    bool passable;
    bool m_isAlive;
    
};

class Penelope : public Actor{
public:
    Penelope(StudentWorld* stdWorld, double startX, double startY)
    :Actor(stdWorld, IID_PLAYER, startX, startY, right, 0, 1.0), m_hasLandmines(false), m_flameThrowerCharges(0), m_hasVaccine(0), m_isInfected(false), m_infectionCount(0){
        setIsAlive(true);
    }
    virtual ~Penelope(){
        
    }
    void doSomething();
    
private:
    bool m_hasLandmines;
    int m_flameThrowerCharges;
    bool m_hasVaccine;
    bool m_isInfected;
    int m_infectionCount;
    
};

class Wall : public Actor{
public:
    Wall(StudentWorld* stdWorld, double startX, double startY)
    :Actor(stdWorld, IID_WALL, startX, startY, right, 0, 1.0){
    }
    virtual ~Wall(){
    }
    void doSomething(){}
private:

    
};

//Comment out these for now.
//class Exit : Actor{
//public:
//    Exit();
//    void doSomething();
//
//};
//
//class Pit : Actor{
//public:
//    Pit();
//    void doSomething();
//
//};
//
//class Flame : Actor{
//public:
//    Flame();
//    void doSomething();
//
//};
//
//class Vomit : Actor{
//public:
//    Vomit();
//    void doSomething();
//
//};
//


// Make a Goodie class and derive classes(VaccineGoodie, GasCanGoodie, LandmineGoodie...)
//class VaccineGoodie : Actor{
//public:
//    VaccineGoodie();
//    void doSomething();
//
//};
//
//class GasCanGoodie : Actor{
//public:
//    GasCanGoodie();
//    void doSomething();
//
//};
//
//class LandmineGoodie : Actor{
//public:
//    LandmineGoodie();
//    void doSomething();
//
//};
//
//class Landmine : Actor{
//public:
//    Landmine();
//    void doSomething();
//
//};
//
//
//
//
//

#endif // ACTOR_H_
