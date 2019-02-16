#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//
class Actor : GraphObject{
public:
    //Simple Constructor
    Actor(/*Add parameters here too!*/) : GraphObject(1, 2, 3, 4, 5, 6){
    }
    virtual void doSomething();
    
};

class Penelope : Actor{
public:
    Penelope();
    void doSomething();
    
};

class Wall : Actor{
public:
    Wall();
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
