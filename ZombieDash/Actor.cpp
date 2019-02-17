#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//void Penelope::doSomething(){
//
//
//}

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size) : GraphObject(imageID, startX, startY, dir, depth, size){
    m_stdPtr = new StudentWorld;
}

StudentWorld* Actor::getWorld() const{
    return m_stdPtr;
}

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
    
}
