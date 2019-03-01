#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject{
public:
    //Simple Constructor
    Actor(StudentWorld* stdPtr, int imageID, double startX, double startY, Direction dir, int depth, double size)
    :GraphObject(imageID, startX, startY, dir, depth, size), m_stdPtr(stdPtr), m_passable(false), m_isAlive(true), m_canBeBurned(true), m_canUseExit(false), m_canBeInfected(true),m_canStepOnLandmine(false), m_explosive(false), m_blockFire(false), m_canFallIntoPit(false), m_isInfected(false),m_canHoldVaccine(false), m_numVaccines(0), m_ticksPassed(0)
    {
    }
    virtual ~Actor(){}
    virtual void doSomething()=0;
    bool getPassable(){return m_passable;}
    bool getIsAlive(){return m_isAlive;}
    void setIsAlive(bool status){m_isAlive = status;}
    bool getCanBeBurned(){return m_canBeBurned;}
    bool getCanUseExit(){return m_canUseExit;}
    bool getCanBeInfected(){return m_canBeInfected;}
    bool getCanStepOnLandmine(){return m_canStepOnLandmine;}
    bool getExplosive(){return m_explosive;}
    bool getBlockFire(){return m_blockFire;}
    bool getCanFallIntoPit(){return m_canFallIntoPit;}
    void setCanFallIntoPit(bool isPossible){m_canFallIntoPit = isPossible;}
    void setIsInfected(bool infectionStatus){m_isInfected = infectionStatus;}
    bool getCanHoldVaccine(){return m_canHoldVaccine;}
    int getNumVaccines(){return m_numVaccines;}
    void changeNumVaccines(int num){m_numVaccines += num;}
    
private:
    StudentWorld* m_stdPtr;
    bool m_passable;
    bool m_isAlive;
    bool m_canBeBurned;
    bool m_canBeInfected;
    bool m_isInfected;
    bool m_canUseExit;
    bool m_canStepOnLandmine;
    bool m_explosive;
    bool m_blockFire;
    bool m_canFallIntoPit;
    bool m_canHoldVaccine;
    int m_numVaccines;
    int m_ticksPassed;

    
protected:
    StudentWorld* getWorld() const{return m_stdPtr;}
    void setPassable(bool passVal){m_passable = passVal;}
    void setCanBeBurned(bool burnable){m_canBeBurned = burnable;}
    void setCanUseExit(bool exitUsable){m_canUseExit = exitUsable;}
    void setCanBeInfected(bool infectable){m_canBeInfected = infectable;}
    void setCanStepOnLandmine(bool stepStatus){m_canStepOnLandmine = stepStatus;}
    void setExplosive(int explosive){m_explosive = explosive;}
    void setBlockFire(bool canBlock){m_blockFire = canBlock;}
    bool getIsInfected(){return m_isInfected;}
    void setCanHoldVaccine(bool canHold){m_canHoldVaccine = canHold;}
    int getTicksPassed(){return m_ticksPassed;}
    void setTicksPassed(int num){m_ticksPassed = num;}

};

class Wall : public Actor{
public:
    Wall(StudentWorld* stdWorld, double startX, double startY)
    :Actor(stdWorld, IID_WALL, startX, startY, right, 0, 1.0)
    {
        setCanBeBurned(false);
        setCanBeInfected(false);
        setBlockFire(true);
    }
    virtual ~Wall(){}
    void doSomething(){}
private:
};

class ActivatingObject : public Actor{
public:
    ActivatingObject(StudentWorld* stdWorld, int imageID, double startX, double startY, Direction dir, int depth, double size)
    :Actor(stdWorld, imageID, startX, startY, dir, depth, size)
    {
    }
    virtual ~ActivatingObject(){}
};

class Exit : public ActivatingObject{
public:
    Exit(StudentWorld* stdWorld, double startX, double startY)
    :ActivatingObject(stdWorld, IID_EXIT, startX, startY, right, 1, 1.0){
        setPassable(true);
        setCanBeBurned(false);
        setCanBeInfected(false);
        setBlockFire(true);
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
    :ActivatingObject(stdWorld, IID_FLAME, startX, startY, dir, 1, 1.0)
    {
        setCanBeInfected(false);
        setCanBeBurned(false);
        setPassable(true);
        setCanStepOnLandmine(true);
    }
    virtual ~Flame(){}
    void doSomething();
private:
};

class Vomit : public ActivatingObject{
public:
    Vomit(StudentWorld* stdWorld, double startX, double startY, Direction dir)
    :ActivatingObject(stdWorld, IID_VOMIT, startX, startY, dir, 0, 1.0)
    {
        setCanBeInfected(false);
        setCanBeBurned(false);
        setPassable(true);
    }
    virtual ~Vomit(){}
    void doSomething();
private:
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

private:
    bool m_isActive;
    int m_safetyTicks;
    bool getIsActive(){return m_isActive;}
    void setIsActive(bool status){m_isActive = status;}
    int getSafetyTicks(){return m_safetyTicks;}
    void decrementSafetyTicks(){m_safetyTicks--;}
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
protected:
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
protected:
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
protected:
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
protected:
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
    :Agent(stdWorld, imageID, startX, startY, right, 0, 1.0), m_infectionCount(0), m_playedSoundInfected(false)
    {
        setCanUseExit(true);
    }
    virtual ~Human(){}
    int getInfectionCount(){return m_infectionCount;}
private:
    int m_infectionCount;
    bool m_playedSoundInfected;
protected:
    void setInfectionCount(int infCount){m_infectionCount = infCount;}
    void setPlayedSoundInfected(bool played){m_playedSoundInfected = played;}
    bool getPlayedSoundInfected(){return m_playedSoundInfected;}
};

class Penelope : public Human{
public:
    Penelope(StudentWorld* stdWorld, double startX, double startY)
    :Human(stdWorld, IID_PLAYER, startX, startY), m_numLandmines(0), m_numFlames(0)
    {
        setIsAlive(true);
        setCanHoldVaccine(true);

    }
    virtual ~Penelope(){}
    void doSomething();
    int getNumLandmines(){return m_numLandmines;}
    int getNumFlames(){return m_numFlames;}
    void changeNumLandmines(int num){m_numLandmines += num;}
    void changeNumFlames(int num){m_numFlames += num;}
private:
    int m_numLandmines;
    int m_numFlames;
};

class Citizen : public Human{
public:
    Citizen(StudentWorld* stdWorld, double startX, double startY)
    :Human(stdWorld, IID_CITIZEN, startX, startY), dist_p(1000), dist_z(1000)
    {
        
    }
    virtual ~Citizen(){}
    void doSomething();
private:
    double dist_p;
    double dist_z;
    int getDistP(){return dist_p;}
    void setDistP(double distanceP){dist_p = distanceP;}
    int getDistZ(){return dist_z;}
    void setDistZ(double distanceZ){dist_z = distanceZ;}
};

class Zombie : public Agent{
public:
    Zombie(StudentWorld* stdWorld, int imageID, double startX, double startY)
    :Agent(stdWorld, imageID, startX, startY, right, 0, 1.0), m_vomitX(-1), m_vomitY(-1), m_movementPlan(0) /*, m_destX(-1), m_destY(-1)*/
    {
        setCanBeInfected(false);
    }
    virtual ~Zombie(){}
    double getVomitX(){return m_vomitX;}
    double getVomitY(){return m_vomitY;}
private:
    double m_vomitX;
    double m_vomitY;
    int m_movementPlan;
protected:
    void setVomitX(double x){m_vomitX = x;}
    void setVomitY(double y){m_vomitY = y;}
    void computeVomitCoor(Zombie* zombiePtr, Direction dir);
    int getMovementPlan(){return m_movementPlan;}
    void setMovementPlan(int plan){m_movementPlan = plan;}
};

class DumbZombie : public Zombie{
public:
    DumbZombie(StudentWorld* stdWorld, double startX, double startY)
    :Zombie(stdWorld, IID_ZOMBIE, startX, startY)
    {
        setCanHoldVaccine(true);
        int holdsVaccine = 1;
        int holdOrNot = randInt(1, 10);
        if(holdOrNot == holdsVaccine){
            changeNumVaccines(1);
        }
    }
    virtual ~DumbZombie(){}
    void doSomething();
};

class SmartZombie : public Zombie{
public:
    SmartZombie(StudentWorld* stdWorld, double startX, double startY)
    :Zombie(stdWorld, IID_ZOMBIE, startX, startY)
    {
    }
    virtual ~SmartZombie(){}
    void doSomething();
};

#endif // ACTOR_H_

