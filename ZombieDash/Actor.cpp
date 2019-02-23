#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <iostream>
#include "GameWorld.h"
#include "GameConstants.h"
using namespace std;

class StudentWorld;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Penelope::doSomething(){    
    if(getIsAlive() == false){//check to see if she is still alive
        return;
    }

    if(getIsInfected()){
        setInfectionCount(getInfectionCount() + 1);
        if(getInfectionCount() == 500){
            setIsAlive(false);
            return;
//            getWorld()->playSound(SOUND_PLAYER_DIE);
        }
    }
    int ch;
    if(getWorld()->getKey(ch)){
        //user hit a key during this tick!
        switch(ch){
            case KEY_PRESS_LEFT:
                Actor::setDirection(left);
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

void Exit::doSomething(){
    getWorld()->escapeHumans(getX(), getY());
    
}
