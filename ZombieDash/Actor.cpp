#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <iostream>
#include "GameWorld.h"
#include "GameConstants.h"
using namespace std;

class StudentWorld;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size)
//    {
//        cout<<"Actor constructor is called!" << endl;
////    m_stdPtr = new StudentWorld;
//}

//StudentWorld* Actor::getWorld() const{
//    return m_stdPtr;
//}

Penelope::Penelope(StudentWorld* stdWorld, int imageID, double startX, double startY, Direction dir, int depth, double size): Actor(stdWorld, imageID, startX, startY, dir, depth, size), m_isAlive(true), m_direction(right), m_depth(0), m_hasLandmines(false), m_flameThrowerCharges(0), m_hasVaccine(0), m_isInfected(false), m_infectionCount(0){
    setActorType("Penelope");
    /*m_stdPtr = new StudentWorld("temp");*/
}

//StudentWorld* Penelope::getWorld() const{
//    return m_stdPtr;
//}

void Penelope::doSomething(){    
    if(m_isAlive == false){//check to see if she is still alive
        return;
    }
    if(m_isInfected){
        m_infectionCount++;
        if(m_infectionCount == 500){
            m_isAlive = false;
            getWorld()->playSound(SOUND_PLAYER_DIE);
        }
    }
//    if(getWorld()->getKey(ch)){
//        cout << "ch : " << ch << endl;
//    }
    int ch;
    if(getWorld()->getKey(ch)){
        //user hit a key during this tick!
        switch(ch){
            case KEY_PRESS_LEFT:
                setDirection(left);
                //move 4 pixels left only when it doesn't overlap(intersect) with wall
                if(!(getWorld()->doesIntersect(this, getX() - 4, getY())) ){
                    moveTo(getX() - 4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(!(getWorld()->doesIntersect(this, getX() + 4, getY()))){
                    moveTo(getX() + 4, getY());
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(!(getWorld()->doesIntersect(this, getX(), getY() + 4))){
                    moveTo(getX(), getY() + 4);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(!(getWorld()->doesIntersect(this, getX(), getY() - 4))){
                    moveTo(getX(), getY() - 4);
                }
                break;


        }

    }
}

void Wall::doSomething(){
//    cout << "wall does nothing!"<<endl;
}
