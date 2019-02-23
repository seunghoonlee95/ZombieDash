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
    :GraphObject(imageID, startX, startY, dir, depth, size), m_stdPtr(stdPtr), passable(false), m_isAlive(true), canBeBurned(true), m_canUseExit(false)
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
    //Maybe not needed if kill() is used instead...
    void setIsAlive(bool status){
        m_isAlive = status;
    }
    void kill(){
        m_isAlive = false;
    }
    
    bool getCanBeBurned(){
        return canBeBurned;
    }
    
    void setCanBeBurned(bool burnable){
        canBeBurned = burnable;
    }
    
    bool getCanUseExit(){
        return m_canUseExit;
    }
    void setCanUseExit(bool exitUsable){
        m_canUseExit = exitUsable;
    }
    
    StudentWorld* getWorld() const{return m_stdPtr;}
private:
    StudentWorld* m_stdPtr;
    bool passable;
    bool m_isAlive;
    bool canBeBurned;
    bool m_canUseExit;
    
};

class Human : public Actor{
public:
    Human(StudentWorld* stdWorld, int imageID, double startX, double startY)
    :Actor(stdWorld, imageID, startX, startY, right, 0, 1.0), m_infectionCount(0), m_isInfected(false){
        setCanUseExit(true);
    }
    virtual ~Human(){}
    int getInfectionCount(){
        return m_infectionCount;
    }
    void setInfectionCount(int infCount){
        m_infectionCount = infCount;
    }
    bool getIsInfected(){
        return m_isInfected;
    }
    void setIsInfected(bool infectionStatus){
        m_isInfected = infectionStatus;
    }

private:
    int m_infectionCount;
    bool m_isInfected;
};

class Penelope : public Human{
public:
    Penelope(StudentWorld* stdWorld, double startX, double startY)
    :Human(stdWorld, IID_PLAYER, startX, startY), m_hasLandmines(false), m_flameThrowerCharges(0), m_hasVaccine(0){
        setIsAlive(true);
    }
    virtual ~Penelope(){
    }
    void doSomething();
    
private:
    bool m_hasLandmines;
    int m_flameThrowerCharges;
    bool m_hasVaccine;
    
};

class Wall : public Actor{
public:
    Wall(StudentWorld* stdWorld, double startX, double startY)
    :Actor(stdWorld, IID_WALL, startX, startY, right, 0, 1.0){}
    virtual ~Wall(){}
    void doSomething(){}
private:

    
};

class Exit : public Actor{
public:
    Exit(StudentWorld* stdWorld, double startX, double startY)
    :Actor(stdWorld, IID_EXIT, startX, startY, right, 1, 1.0){
        setPassable(true);
    }
    virtual ~Exit(){}
    void doSomething();
private:

};

//class Citizen : public Human{
//public:
//    Citizen(StudentWorld* stdWorld, double startX, double startY)
//    :Human(stdWorld, IID_CITIZEN, startX, startY){}
//    virtual ~Citizen(){}
//    void doSomething(){}
//private:
//    
//};

//
//class Pit : public Actor{
//public:
//    Pit();
//    void doSomething(){}
//
//};
//
//class Flame : public Actor{
//public:
//    Flame();
//    void doSomething(){}
//
//};
//
//class Vomit : public Actor{
//public:
//    Vomit();
//    void doSomething(){}
//
//};
//


// Make a Goodie class and derive classes(VaccineGoodie, GasCanGoodie, LandmineGoodie...)
//class VaccineGoodie : public Actor{
//public:
//    VaccineGoodie();
//    void doSomething(){}
//
//};
//
//class GasCanGoodie : public Actor{
//public:
//    GasCanGoodie();
//    void doSomething(){}
//
//};
//
//class LandmineGoodie : public Actor{
//public:
//    LandmineGoodie();
//    void doSomething(){}
//
//};
//
//class Landmine : public Actor{
//public:
//    Landmine();
//    void doSomething(){}
//
//};
//
//
//
//
//

#endif // ACTOR_H_
