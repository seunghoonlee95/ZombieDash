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
            case KEY_PRESS_ENTER:
                if(getNumVaccines() >= 1){
                    setIsInfected(false);
                    changeNumVaccines(-1);
                    setInfectionCount(0);
                }
                break;
            case KEY_PRESS_SPACE:
                if(getNumFlames() > 0){
                    changeNumFlames(-1);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    getWorld()->blastFlame();
                }
                break;
            case KEY_PRESS_TAB:
                if(getNumLandmines() > 0){
                    getWorld()->plantLandmine();
                    changeNumLandmines(-1);
                }
                break;
        }//end of switch statement
    }
}

void Goodie::pickUp(){
    if(getWorld()->doesOverlapWithPlayer(this)){
        getWorld()->increaseScore(50);
        setIsAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        grantSpecificGoodieReward();
    }
    
}

void VaccineGoodie::grantSpecificGoodieReward(){
//    getWorld()->setGotVaccine(true);
    getWorld()->incrementVaccine();
}

void GasCanGoodie::grantSpecificGoodieReward(){
//    getWorld()->setGotFlames(true);
    getWorld()->incrementFlameCount();
}

void LandmineGoodie::grantSpecificGoodieReward(){
    getWorld()->incrementLandmineCount();
}


void Exit::doSomething(){
    getWorld()->escapeHumans(getX(), getY());
}

void Pit::doSomething(){
    getWorld()->fallIntoPit(this);
}

void VaccineGoodie::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    pickUp();
}

void GasCanGoodie::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    pickUp();
}

void LandmineGoodie::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    pickUp();
}

void Flame::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    if(getTicksPassed() == 2){
        setIsAlive(false);
        return;
    }
    setTicksPassed(getTicksPassed() + 1);
    getWorld()->damageObjects(this);
}

void Landmine::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    if(getIsActive() == false){
        decrementSafetyTicks();
        if(getSafetyTicks() == 0){
            setIsActive(true);
            return;
        }
    }else{//when Landmine is active
        getWorld()->explodeMine(this, false);
    }
}

void Citizen::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    setTicksPassed(getTicksPassed() + 1);
    if(getIsInfected() == true){
        setInfectionCount(getInfectionCount() + 1);
        if(getInfectionCount() == 500){
            setIsAlive(false);
            getWorld()->playSound(SOUND_ZOMBIE_BORN);
            getWorld()->increaseScore(-1000);
            //Introduce a zombie based on the probability shown in spec..
            return;
        }
    }
    if(getTicksPassed() % 2 == 0){
//        cout << "freeze in this tick : " << getTicksPassed() << endl;
        return;
    }
    setDistP(getWorld()->determineDistToPenelope(this));
    //setDistZ too here!
    
    if(getDistP() < getDistZ() && getDistP() <= 80.0){
//        cout << "distance is below 80 follow Penelope!" << endl;
        getWorld()->followPenelope(this);
        return;
    }

}

void Vomit::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    if(getTicksPassed() == 2){
        setIsAlive(false);
        return;
    }
    setTicksPassed(getTicksPassed() + 1);
    getWorld()->infectObjects(this);
}

void DumbZombie::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    setTicksPassed(getTicksPassed() + 1);
    if(getTicksPassed() % 2 == 0){
        return;
    }
    //Continue here@@@@@@@@@@@@@@@@@@@@@@@@@@
}

