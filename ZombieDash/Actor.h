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
    :GraphObject(imageID, startX, startY, dir, depth, size), m_stdPtr(stdPtr), m_passable(false), m_isAlive(true), m_canBeBurned(true), m_canUseExit(false), m_canBeInfected(true),m_canStepOnLandmine(false), m_explosive(false), m_blockFireVomit(false), m_canFallIntoPit(false)
    {
    }
    virtual ~Actor(){}
    virtual void doSomething()=0;
    
    bool getPassable(){return m_passable;}
    void setPassable(bool passVal){m_passable = passVal;}
    bool getIsAlive(){return m_isAlive;}
    //Maybe not needed if kill() is used instead...
    void setIsAlive(bool status){m_isAlive = status;}
//    void kill(){m_isAlive = false;}
    bool getCanBeBurned(){return m_canBeBurned;}
    void setCanBeBurned(bool burnable){m_canBeBurned = burnable;}
    bool getCanUseExit(){return m_canUseExit;}
    void setCanUseExit(bool exitUsable){m_canUseExit = exitUsable;}
    bool getCanBeInfected(){return m_canBeInfected;}
    void setCanBeInfected(bool infectable){m_canBeInfected = infectable;}
    bool getCanStepOnLandmine(){return m_canStepOnLandmine;}
    void setCanStepOnLandmine(bool stepStatus){m_canStepOnLandmine = stepStatus;}
    bool getExplosive(){return m_explosive;}
    void setExplosive(int explosive){m_explosive = explosive;}
    bool getBlockFireVomit(){return m_blockFireVomit;}
    void setBlockFireVomit(bool canBlock){m_blockFireVomit = canBlock;}
    bool getCanFallIntoPit(){return m_canFallIntoPit;}
    void setCanFallIntoPit(bool isPossible){m_canFallIntoPit = isPossible;}
    StudentWorld* getWorld() const{return m_stdPtr;}
private:
    StudentWorld* m_stdPtr;
    
    bool m_passable;
    bool m_isAlive;
    bool m_canBeBurned;
    bool m_canBeInfected;
    bool m_canUseExit;
    bool m_canStepOnLandmine;
    bool m_explosive;
    bool m_blockFireVomit;
    bool m_canFallIntoPit;

};

class Wall : public Actor{
public:
    Wall(StudentWorld* stdWorld, double startX, double startY)
    :Actor(stdWorld, IID_WALL, startX, startY, right, 0, 1.0){
        setCanBeBurned(false);
        setCanBeInfected(false);
        setBlockFireVomit(true);
    }
    virtual ~Wall(){}
    void doSomething(){}
private:
};

class ActivatingObject : public Actor{
public:
    ActivatingObject(StudentWorld* stdWorld, int imageID, double startX, double startY, Direction dir, int depth, double size)
    :Actor(stdWorld, imageID, startX, startY, dir, depth, size){
    }
    virtual ~ActivatingObject(){}
};

class Vomit : public ActivatingObject{
public:
    Vomit(StudentWorld* stdWorld, double startX, double startY, Direction dir)
    :ActivatingObject(stdWorld, IID_VOMIT, startX, startY, dir, 0, 1.0), m_ticksPassed(0)
    {
        
    }
    virtual ~Vomit(){}
    void doSomething();
    void setTicksPassed(int tick){m_ticksPassed = tick;}
    int getTicksPassed(){return m_ticksPassed;}
private:
    int m_ticksPassed;

};

class Exit : public ActivatingObject{
public:
    Exit(StudentWorld* stdWorld, double startX, double startY)
    :ActivatingObject(stdWorld, IID_EXIT, startX, startY, right, 1, 1.0){
        setPassable(true);
        setCanBeBurned(false);
        setCanBeInfected(false);
        setBlockFireVomit(true);

    }
    virtual ~Exit(){}
    void doSomething();
private:
    
};

class Pit : public ActivatingObject{
public:
    Pit(StudentWorld* stdWorld, double startX, double startY)
    :ActivatingObject(stdWorld, IID_PIT, startX, startY, right, 0, 1.0)
    {
        setCanBeBurned(false);
        setCanBeInfected(false);
        setPassable(true);

    }
    virtual ~Pit(){}
    void doSomething();

};

class Flame : public ActivatingObject{
public:
    Flame(StudentWorld* stdWorld, double startX, double startY, Direction dir)
    :ActivatingObject(stdWorld, IID_FLAME, startX, startY, dir, 1, 1.0), m_ticksPassed(0)
    {
        setCanBeInfected(false);
        setCanBeBurned(false);
        setPassable(true);
    }
    virtual ~Flame(){}
    void doSomething();
    void setTicksPassed(int tick){m_ticksPassed = tick;}
    int getTicksPassed(){return m_ticksPassed;}
private:
    int m_ticksPassed;
};

class Landmine : public ActivatingObject{
public:
    Landmine(StudentWorld* stdWorld, double startX, double startY)
    :ActivatingObject(stdWorld, IID_LANDMINE, startX, startY, right, 1, 1.0), m_isActive(false), m_safetyTicks(30)
    {
        setPassable(true);
        setCanBeInfected(false);
        setCanBeBurned(true);
        setExplosive(true);
    }
    virtual ~Landmine(){}
    void doSomething();
    bool getIsActive(){return m_isActive;}
    void setIsActive(bool status){m_isActive = status;}
    int getSafetyTicks(){return m_safetyTicks;}
    void decrementSafetyTicks(){m_safetyTicks--;}
//    bool getDamagedByFire(){return m_damagedByFire;}
//    void setDamagedByFire(bool fireAttacked){m_damagedByFire = fireAttacked;}
private:
    bool m_isActive;
    int m_safetyTicks;
//    bool m_damagedByFire;
};


class Goodie : public ActivatingObject{
public:
    Goodie(StudentWorld* stdWorld,int imageID, double startX, double startY, Direction dir, int depth, double size)
    :ActivatingObject(stdWorld, imageID, startX, startY, dir, depth, size){
        setCanBeBurned(true);
        setCanBeInfected(false);
        setPassable(true);
    }
    virtual ~Goodie(){}
    void pickUp();
    virtual void grantSpecificGoodieReward() = 0;
};

class VaccineGoodie : public Goodie{
public:
    VaccineGoodie(StudentWorld* stdWorld, double startX, double startY)
    :Goodie(stdWorld, IID_VACCINE_GOODIE, startX, startY, right, 1, 1.0)
    {

    }
    virtual ~VaccineGoodie(){}
    void doSomething();
    virtual void grantSpecificGoodieReward();
};

class GasCanGoodie : public Goodie{
public:
    GasCanGoodie(StudentWorld* stdWorld, double startX, double startY)
    :Goodie(stdWorld, IID_GAS_CAN_GOODIE, startX, startY, right, 1, 1.0)
    {

    }
    virtual ~GasCanGoodie(){}
    void doSomething();
    virtual void grantSpecificGoodieReward();
};

class LandmineGoodie : public Goodie{
public:
    LandmineGoodie(StudentWorld* stdWorld, double startX, double startY)
    :Goodie(stdWorld, IID_LANDMINE_GOODIE, startX, startY, right, 1, 1.0)
    {

    }
    virtual ~LandmineGoodie(){}
    void doSomething();
    virtual void grantSpecificGoodieReward();
};


class Agent : public Actor{
public:
    Agent(StudentWorld* stdWorld,int imageID, double startX, double startY, Direction dir, int depth, double size)
    :Actor(stdWorld, imageID, startX, startY, dir, depth, size)
    {
        setCanStepOnLandmine(true);
        setCanFallIntoPit(true);
    }
    virtual ~Agent(){}
private:
    
};

class Human : public Agent{
public:
    Human(StudentWorld* stdWorld, int imageID, double startX, double startY)
    :Agent(stdWorld, imageID, startX, startY, right, 0, 1.0), m_infectionCount(0), m_isInfected(false){
        setCanUseExit(true);
    }
    virtual ~Human(){}
    int getInfectionCount(){return m_infectionCount;}
    void setInfectionCount(int infCount){m_infectionCount = infCount;}
    bool getIsInfected(){return m_isInfected;}
    void setIsInfected(bool infectionStatus){m_isInfected = infectionStatus;}
    
private:
    int m_infectionCount;
    bool m_isInfected;
};

class Zombie : public Agent{
public:
    Zombie(StudentWorld* stdWorld, int imageID, double startX, double startY)
    :Agent(stdWorld, imageID, startX, startY, right, 0, 1.0), m_ticksPassed(0)
    {
        
    }
    void setTicksPassed(int tick){m_ticksPassed = tick;}
    int getTicksPassed(){return m_ticksPassed;}
    virtual ~Zombie(){}
private:
    int m_ticksPassed;

};

class DumbZombie : public Zombie{
public:
    DumbZombie(StudentWorld* stdWorld, double startX, double startY)
    :Zombie(stdWorld, IID_ZOMBIE, startX, startY)
    {
        
    }
    void doSomething();
};

class Penelope : public Human{
public:
    Penelope(StudentWorld* stdWorld, double startX, double startY)
    :Human(stdWorld, IID_PLAYER, startX, startY), m_numLandmines(110), m_numFlames(100), m_numVaccines(0){
        setIsAlive(true);
    }
    virtual ~Penelope(){}
    void doSomething();
    
    int getNumLandmines(){return m_numLandmines;}
    int getNumFlames(){return m_numFlames;}
    int getNumVaccines(){return m_numVaccines;}
    void changeNumVaccines(int num){m_numVaccines += num;}
    void changeNumLandmines(int num){m_numLandmines += num;}
    void changeNumFlames(int num){m_numFlames += num;}
private:
    int m_numLandmines;
    int m_numFlames;
    int m_numVaccines;
};

class Citizen : public Human{
public:
    Citizen(StudentWorld* stdWorld, double startX, double startY)
    :Human(stdWorld, IID_CITIZEN, startX, startY), m_ticksPassed(0), dist_p(1000), dist_z(1000)
    {
        
    }
    virtual ~Citizen(){}
    void doSomething();
    void setTicksPassed(int tick){m_ticksPassed = tick;}
    int getTicksPassed(){return m_ticksPassed;}
    int getDistP(){return dist_p;}
    void setDistP(double distanceP){dist_p = distanceP;}
    int getDistZ(){return dist_z;}
    void setDistZ(double distanceZ){dist_z = distanceZ;}
private:
    int m_ticksPassed;
    double dist_p;
    double dist_z;
};


//
//class Vomit : public Actor{
//public:
//    Vomit();
//    void doSomething(){} 
//
//};
//



#endif // ACTOR_H_

