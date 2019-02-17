#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"

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
    int ch;
    if(getWorld()->getKey(ch)){
        //user hit a key during this tick!
        switch(ch){
            case KEY_PRESS_LEFT:
                if(getDirection() != left){
                    setDirection(left);
                }else{//move 4 pixels left unless it doesn't overlap with wall
                    moveTo(getX() - 4, getY());
                }
                    
                
                
                //Set Penelope's direction to the specified movement direction
                //Determine her destination location(4 pixels in the facing direction)
                //If not intersect with bounding box of any wall,citizen zombie: update location using GraphObject's moveTo() method.
                break;
        }
    }
        
        
    
}
