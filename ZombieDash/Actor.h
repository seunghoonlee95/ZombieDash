#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "SoundFX.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//
class Actor : GraphObject{
public:
    //Simple Constructor
    Actor(int imageID, double startX, double startY, Direction dir, int depth, double size) : GraphObject(imageID, startX, startY, dir, depth, size){
        
    }
    
    StudentWorld* getWorld(){
        StudentWorld* ptr = new StudentWorld();
        return ptr;
    }
    
    
private:
    virtual void doSomething();
};

class Penelope : Actor{
public:
    Penelope(int imageID, double startX, double startY, Direction dir, int depth, double size)
    :Actor(imageID, startX, startY, dir, depth, size), m_isAlive(true), m_direction(0), m_depth(0), m_hasLandmines(false), m_flameThrowerCharges(0), m_hasVaccine(0), m_isInfected(false), m_infectionCount(0)
    //Why can't I do m_direction(right)?? right is a public variable of parent class..!!!
    {
        
    }
    void doSomething(){
        if(m_isAlive == false){//check to see if she is still alive
            return;
        }
        if(m_isInfected){
            m_infectionCount++;
            if(m_infectionCount == 500){
                m_isAlive = false;
                //playClip(SOUND_PLAYER_FIRE);
            }
        }
        
    }
    
private:
    bool m_isAlive;
    Direction m_direction;
    int m_depth;
    bool m_hasLandmines;
    int m_flameThrowerCharges;
    bool m_hasVaccine;
    bool m_isInfected;
    int m_infectionCount;
    
};

class Wall : Actor{
public:
    Wall(int imageID, double startX, double startY, Direction dir, int depth, double size);
    void doSomething();
  
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
