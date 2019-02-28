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
        if(getPlayedSoundInfected() == false){
            getWorld()->playSound(SOUND_CITIZEN_INFECTED);
            setPlayedSoundInfected(true);
        }
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
    if(getWorld()->doesOverlapWithPlayer(this->getX(), this->getY())){
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
    if(getIsFromZombie()){
        setTicksPassed(getTicksPassed() + 1);
        if(getTicksPassed() == 2){
            setCanBeBurned(true);
        }
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
        if(getPlayedSoundInfected() == false){
            getWorld()->playSound(SOUND_CITIZEN_INFECTED);
            setPlayedSoundInfected(true);
        }
        setInfectionCount(getInfectionCount() + 1);
        if(getInfectionCount() == 500){
            getWorld()->citizenBecomeZombie(this);
            //Introduce a zombie based on the probability shown in spec..
            return;
        }
    }
    if(getTicksPassed() % 2 == 0){
//        cout << "freeze in this tick : " << getTicksPassed() << endl;
        return;
    }
    setDistP(getWorld()->determineDistToPenelope(this));
    setDistZ(getWorld()->determineDistToClosestZombie(this));
//    cout << "dist_p : " << getDistP() << endl;
//    cout << "dist_z : " << getDistZ() << endl;

    if(getDistP() < getDistZ() && getDistP() <= 80.0){
//        cout << "distance is below 80 follow Penelope!" << endl;
        getWorld()->followPenelope(this,2);
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

void Zombie::computeVomitCoor(Zombie* zombiePtr, Direction dir){
    switch (dir) {
        case right:
            setVomitX(zombiePtr->getX() + SPRITE_WIDTH);
            setVomitY(zombiePtr->getY());
            break;
        case left:
            setVomitX(zombiePtr->getX() - SPRITE_WIDTH);
            setVomitY(zombiePtr->getY());
            break;
        case up:
            setVomitX(zombiePtr->getX());
            setVomitY(zombiePtr->getY() + SPRITE_HEIGHT);
            break;
        case down:
            setVomitX(zombiePtr->getX());
            setVomitY(zombiePtr->getY() - SPRITE_HEIGHT);
            break;
    }
}

void DumbZombie::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    setTicksPassed(getTicksPassed() + 1);
    if(getTicksPassed() % 2 == 0){
        return;
    }
    computeVomitCoor(this, getDirection());
    if(getWorld()->throwVomit(this)){
        return;
    }
    if(getMovementPlan() == 0){
        setMovementPlan(randInt(3, 10));
//        cout << "dumb zombie's new movement plan : " << getMovementPlan() << endl;
    }

        const int rightZ = 1;
        const int leftZ = 2;
        const int downZ = 3;
        const int upZ = 4;
        int newDirection = randInt(1, 4);
        switch (newDirection) {
            case rightZ:
                setDirection(right);
                if(!(getWorld()->doesIntersect(this, getX() + 1, getY()))){
                    moveTo(getX() + 1, getY());
                    setMovementPlan(getMovementPlan() - 1);
                }else{//when the zombie is blocked by wall, Human, Zombie
                    setMovementPlan(0);
                }
                break;
            case leftZ:
                setDirection(left);
                if(!(getWorld()->doesIntersect(this, getX() - 1, getY()))){
                    moveTo(getX() - 1, getY());
                    setMovementPlan(getMovementPlan() - 1);
                }else{//when the zombie is blocked by wall, Human, Zombie
                    setMovementPlan(0);
                }
                break;
            case downZ:
                setDirection(down);
                if(!(getWorld()->doesIntersect(this, getX(), getY() - 1))){
                    moveTo(getX(), getY() - 1);
                    setMovementPlan(getMovementPlan() - 1);
                }else{//when the zombie is blocked by wall, Human, Zombie
                    setMovementPlan(0);
                }
                break;
            case upZ:
                setDirection(up);
                if(!(getWorld()->doesIntersect(this, getX(), getY() + 1))){
                    moveTo(getX(), getY() + 1);
                    setMovementPlan(getMovementPlan() - 1);
                }else{//when the zombie is blocked by wall, Human, Zombie
                    setMovementPlan(0);
                }
                break;
        }
}

void SmartZombie::doSomething(){
    if(getIsAlive() == false){
        return;
    }
    setTicksPassed(getTicksPassed() + 1);
    if(getTicksPassed() % 2 == 0){
        return;
    }
    computeVomitCoor(this, getDirection());
    if(getWorld()->throwVomit(this)){
        return;
    }
    if(getMovementPlan() == 0){
        setMovementPlan(randInt(3, 10));
//        cout << "smart zombie's new movement plan : " << getMovementPlan()<< endl;
    }

    bool foundClosestPerson;
    foundClosestPerson = getWorld()->findClosestPersonAndFollow(this);
    if(!foundClosestPerson){//if there is no Person within 80 pixels, then set its direction randomly.
        //cout << "In here!!" <<endl;
        const int rightZ = 1;
        const int leftZ = 2;
        const int downZ = 3;
        const int upZ = 4;
        int newDirection = randInt(1, 4);
//        cout << "newDirection : " << newDirection << endl;
            switch (newDirection){
                case rightZ:
                    setDirection(right);
                    if(!(getWorld()->doesIntersect(this, getX() + 1, getY()))){
                        moveTo(getX() + 1, getY());
                        setMovementPlan(getMovementPlan() - 1);
                    }else{//when the zombie is blocked by wall, Human, Zombie
                        setMovementPlan(0);
                    }
                    break;
                case leftZ:
                    setDirection(left);
                    if(!(getWorld()->doesIntersect(this, getX() - 1, getY()))){
                        moveTo(getX() - 1, getY());
                        setMovementPlan(getMovementPlan() - 1);
                    }else{//when the zombie is blocked by wall, Human, Zombie
                        setMovementPlan(0);
                    }
                    break;
                case downZ:
                    setDirection(down);
                    if(!(getWorld()->doesIntersect(this, getX(), getY() - 1))){
                        moveTo(getX(), getY() - 1);
                        setMovementPlan(getMovementPlan() - 1);
                    }else{//when the zombie is blocked by wall, Human, Zombie
                        setMovementPlan(0);
                    }
                    break;
                case upZ:
                    setDirection(up);
                    if(!(getWorld()->doesIntersect(this, getX(), getY() + 1))){
                        moveTo(getX(), getY() + 1);
                        setMovementPlan(getMovementPlan() - 1);
                    }else{//when the zombie is blocked by wall, Human, Zombie
                        setMovementPlan(0);
                    }
                    break;
                
        } //end of switch
    }
}//end of doSomething

