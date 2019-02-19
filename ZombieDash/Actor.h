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
    Actor(StudentWorld* stdPtr, int imageID, double startX, double startY, Direction dir, int depth, double size): GraphObject(imageID, startX, startY, dir, depth, size), m_stdPtr(stdPtr){
        
    }
    virtual ~Actor(){
        cout <<"Actor's destructor"<< endl;
        delete m_stdPtr;
    }
     virtual void doSomething() = 0;

    
    StudentWorld* getWorld() const{return m_stdPtr;}
    string getActorType(){return actorType;}
    void setActorType(string actor){actorType = actor;}
    
    
private:
    StudentWorld* m_stdPtr;
    string actorType;
    
};

class Penelope : public Actor{
public:
    Penelope(StudentWorld* stdWorld, int imageID, double startX, double startY, Direction dir, int depth, double size);
    /*
    :Actor(imageID, startX, startY, dir, depth, size), m_isAlive(true), m_direction(right), m_depth(0), m_hasLandmines(false), m_flameThrowerCharges(0), m_hasVaccine(0), m_isInfected(false), m_infectionCount(0)
    {
        setActorType("Penelope");
    }*/
    virtual ~Penelope(){
        
    }
    void doSomething();
//    StudentWorld* getWorld() const;

private:
//    StudentWorld* m_stdPtr;
    bool m_isAlive;
    Direction m_direction;
    int m_depth;
    bool m_hasLandmines;
    int m_flameThrowerCharges;
    bool m_hasVaccine;
    bool m_isInfected;
    int m_infectionCount;
    
};

class Wall : public Actor{
public:
    Wall(StudentWorld* stdWorld, int imageID, double startX, double startY, Direction dir, int depth, double size)
    :Actor(stdWorld, imageID, startX, startY, dir, depth, size), m_direction(right), m_depth(0){
        setActorType("Wall");
    }
    virtual ~Wall(){
        
    }
    void doSomething();
private:
    Direction m_direction;
    int m_depth;
    
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
