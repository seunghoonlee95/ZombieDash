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
                setDirection(left);
                //move 4 pixels left only when it doesn't overlap(intersect) with wall
                if(!getWorld()->doesIntersect(getX() - 4, getY())){
                    moveTo(getX() - 4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(!getWorld()->doesIntersect(getX() + 4, getY())){
                    moveTo(getX() + 4, getY());
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(!getWorld()->doesIntersect(getX(), getY() + 4)){
                    moveTo(getX(), getY() + 4);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(!getWorld()->doesIntersect(getX(), getY() - 4)){
                    moveTo(getX(), getY() - 4);
                }
                break;
                
                
                //Needed for part II
//            case KEY_PRESS_SPACE:
//                break;
//            case KEY_PRESS_TAB:
//                break;
//            case KEY_PRESS_ENTER:
//                break;
        }
        
    }
}
