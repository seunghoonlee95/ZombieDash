#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace std;

//returns a pointer to GameWorld!!!- seems useful...?
GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), playerPtr(nullptr), m_finishedLevel(false) /*, m_gotVaccine(false), m_gotFlames(false)*/
{
    actorList.reserve(256);
}

int StudentWorld::init(){
    Level lev(assetPath());
    int level;
    level = getLevel();
    //    cout << "level : " << level << endl;
    string levelFile;
    if(level < 0){
        return GWSTATUS_LEVEL_ERROR; // Not sure!!
    }else if(level < 10){
        levelFile = "level0" + to_string(level) + ".txt";
    }else{
        levelFile = "level" + to_string(level) + ".txt";
    }
    
    switch(level){
        case 1:
            levelFile = "level01.txt";
            break;
        case 2:
            levelFile = "level02.txt";
            break;
        case 3:
            levelFile = "level03.txt";
            break;
        case 4:
            levelFile = "level04.txt";
            break;
        case 5:
            levelFile = "level05.txt";
            break;
        case 6:
            levelFile = "level06.txt";
            break;
    }
    
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found){
        cerr << "Cannot find level01.txt data file" << endl;
    }else if (result == Level::load_fail_bad_format){
        cerr << "Your level was improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }else if (result == Level::load_success){
        cerr << "Successfully loaded level" << endl;
        for(double x = 0; x < VIEW_WIDTH / SPRITE_WIDTH; x++){
            for(double y = 0; y < VIEW_HEIGHT / SPRITE_HEIGHT; y++){
                Level::MazeEntry ge = lev.getContentsOf(x,y); // level_x=5, level_y=10 switch (ge) // so x=80 and y=160
                switch (ge){
                    case Level::empty:
                        break;
                    case Level::smart_zombie:
                        actorList.push_back(new SmartZombie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::dumb_zombie:
                        actorList.push_back(new DumbZombie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::citizen:
                        actorList.push_back(new Citizen(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::player:
                        playerPtr = new Penelope(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                        break;
                    case Level::exit:
                        actorList.push_back(new Exit(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::wall:
                        actorList.push_back(new Wall(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::pit:
                        actorList.push_back(new Pit(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::vaccine_goodie:
                        actorList.push_back(new VaccineGoodie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::gas_can_goodie:
                        actorList.push_back(new GasCanGoodie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                    case Level::landmine_goodie:
                        actorList.push_back(new LandmineGoodie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}



int StudentWorld::move(){
    
    string gameStatus = "";
    gameStatus = "Score: " + to_string(getScore()) + "  Level: " + to_string(getLevel()) + "  Lives: " + to_string(getLives()) + "  Vaccines: " + to_string(playerPtr->getNumVaccines()) + "  Flames: " + to_string(playerPtr->getNumFlames()) + "  Mines: " + to_string(playerPtr->getNumLandmines()) + "  Infected: " + to_string(playerPtr->getInfectionCount());
    
    setGameStatText(gameStatus);
    
    playerPtr->doSomething();
    if(playerPtr->getIsAlive() == false){
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end() && *actIt != nullptr){
        if((*actIt)->getIsAlive() == false){
            delete (*actIt);
            actorList.erase(actIt);
//            actIt = actorList.erase(actIt);
            //  actIt++;      //this was causing the error!!
        }else if((*actIt)->getIsAlive() == true){
                (*actIt)->doSomething();
                actIt++;
        }
    }

    if(getFinishedLevel()){
        playSound(SOUND_LEVEL_FINISHED);
        setFinishedLevel(false);
        
        //Whhen the player completes all 6 stages..
        if(getLevel() == 99){
            return GWSTATUS_PLAYER_WON;
        }
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if(!actorList.empty()){
        vector<Actor*>::iterator actIt;
        actIt = actorList.begin();
        while(actIt != actorList.end()){
            delete(*actIt);
            actIt++;
        }
        actorList.clear();
        delete playerPtr;
    }
}

//Cannot intersect others' bounding boxes!!
bool StudentWorld::doesIntersect(Actor* actorPtr, double destX, double destY){
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        if(actorPtr != (*actIt) && abs((*actIt)->getX() - destX) < 16 && abs((*actIt)->getY() - destY) < 16){
            if((*actIt)->getPassable() == false){//if the destination object is not passable(wall, citizen, zombie only among actorList) return true
                return true;
            }
        }
        actIt++;
    }

    if(actorPtr != playerPtr && abs((playerPtr)->getX() - destX) < 16 && abs((playerPtr)->getY() - destY) < 16){//check if it intersects with Penelope
        return true;
    }
    return false;
}

//The distance between the center points should be less than or equal to 10 pixels..
bool StudentWorld::doesOverlap(double actorX, double actorY, double otherX, double otherY){
    double distance;
    distance = sqrt(pow(actorX - otherX, 2) + pow(actorY - otherY, 2));
    if(distance <= 10.0){
        return true;
    }
    return false;
}

void StudentWorld::escapeHumans(double exitX, double exitY){
    bool freedAllCitizens = true;
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        if((*actIt)->getCanUseExit()){//only citizen can use exit among ActorList.
            if(doesOverlap((*actIt)->getX(), (*actIt)->getY(),exitX, exitY)){//when exit overlaps with citizen, free the citizen
                increaseScore(500);
                (*actIt)->setIsAlive(false);
                playSound(SOUND_CITIZEN_SAVED);
            }
            freedAllCitizens = false;
        }
        actIt++;
    }
    //Now check if exit overlaps with Penelope
    if(doesOverlap(playerPtr->getX(), playerPtr->getY(), exitX, exitY)){
        if(freedAllCitizens){
            setFinishedLevel(true);
        }
    }
}

void StudentWorld::fallIntoPit(Actor *pitPtr){
    if(doesOverlapWithPlayer(pitPtr->getX(), pitPtr->getY())){
        if(playerPtr->getIsAlive() == true){
            playerPtr->setIsAlive(false);
        }
    }
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        if((*actIt)->getCanFallIntoPit() == true){
            if(doesOverlap(pitPtr->getX(), pitPtr->getY(),(*actIt)->getX(), (*actIt)->getY())){
                //check if the actor is citizen or a zombie...
                if((*actIt)->getCanBeInfected()){//when citizen falls into a pit
                        (*actIt)->setIsAlive(false);
                        playSound(SOUND_CITIZEN_DIE);
                        increaseScore(-1000);
                }else{//when a zombie falls into a pit.
                        (*actIt)->setIsAlive(false);
                        playSound(SOUND_ZOMBIE_DIE);
                    //increaseScore(2000); //Depends on whether it's a smart or dumb zombie..
                }
            }
        }
        actIt++;
    }
    
}

void StudentWorld::explodeMine(Actor* minePtr, bool triggeredByFlame){
    if(triggeredByFlame || doesOverlap(playerPtr->getX(), playerPtr->getY(), minePtr->getX(), minePtr->getY())){//overlaps with a player or flame
        cout << "Explode mine either by flame or Penelope!" << endl;

        actorList.push_back(new Flame(this, minePtr->getX(), minePtr->getY(), playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX() - SPRITE_WIDTH, minePtr->getY() - SPRITE_HEIGHT, playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX(), minePtr->getY() - SPRITE_HEIGHT, playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX() + SPRITE_WIDTH, minePtr->getY() - SPRITE_HEIGHT, playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX() + SPRITE_WIDTH, minePtr->getY(), playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX() + SPRITE_WIDTH, minePtr->getY() + SPRITE_HEIGHT, playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX(), minePtr->getY() + SPRITE_HEIGHT, playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX() - SPRITE_HEIGHT, minePtr->getY() + SPRITE_HEIGHT, playerPtr->getDirection()));
        actorList.push_back(new Flame(this, minePtr->getX() - SPRITE_HEIGHT, minePtr->getY(), playerPtr->getDirection()));
        minePtr->setIsAlive(false);
        playSound(SOUND_LANDMINE_EXPLODE);
        actorList.push_back(new Pit(this, minePtr->getX(), minePtr->getY()));
        return;
    }
        vector<Actor*>::iterator actIt = actorList.begin();
        while(actIt != actorList.end() && (*actIt) != nullptr){
            if((*actIt)->getCanStepOnLandmine() == true){
                if(doesOverlap((*actIt)->getX(), (*actIt)->getY(), minePtr->getX(), minePtr->getY())){
                    //overlaps with a citizen/zombie or a flame.
                    cout << "citizen/zombie/flame overlapped with landmine!!" << endl;
                    actorList.push_back(new Flame(this, minePtr->getX(), minePtr->getY(), playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX() - SPRITE_WIDTH, minePtr->getY() - SPRITE_HEIGHT, playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX(), minePtr->getY() - SPRITE_HEIGHT, playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX() + SPRITE_WIDTH, minePtr->getY() - SPRITE_HEIGHT, playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX() + SPRITE_WIDTH, minePtr->getY(), playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX() + SPRITE_WIDTH, minePtr->getY() + SPRITE_HEIGHT, playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX(), minePtr->getY() + SPRITE_HEIGHT, playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX() - SPRITE_HEIGHT, minePtr->getY() + SPRITE_HEIGHT, playerPtr->getDirection()));
                    actorList.push_back(new Flame(this, minePtr->getX() - SPRITE_HEIGHT, minePtr->getY(), playerPtr->getDirection()));
                    minePtr->setIsAlive(false);
                    playSound(SOUND_LANDMINE_EXPLODE);
                    actorList.push_back(new Pit(this, minePtr->getX(), minePtr->getY()));
                    cout<<"executed until here~(end of explodeMine method)" << endl;
                    return;

                }
            }
            actIt++;
        }
}

void StudentWorld::plantLandmine(){
    actorList.push_back(new Landmine(this, playerPtr->getX(), playerPtr->getY()));
}

bool StudentWorld::doesOverlapWithPlayer(double actorX, double actorY){
    if(doesOverlap(actorX, actorY, playerPtr->getX(), playerPtr->getY())){
        return true;
    }
    return false;
}



void StudentWorld::infectObjects(Actor* vomitPtr){
    vector<Actor*>::iterator actIt = actorList.begin();
    if(doesOverlapWithPlayer(vomitPtr->getX(), vomitPtr->getY())){
        playerPtr->setIsInfected(true);
    }else{
        while(actIt != actorList.end() && *actIt != nullptr){
            if((*actIt)->getCanBeInfected() == true && doesOverlap((*actIt)->getX(), (*actIt)->getY(), vomitPtr->getX(), vomitPtr->getY())){
                    (*actIt)->setIsInfected(true);
            }
            actIt++;
        }
    }
}

void StudentWorld::damageObjects(Actor* flamePtr){//this is for "each" flame!!!
    vector<Actor*>::iterator actIt = actorList.begin();
    if(doesOverlapWithPlayer(flamePtr->getX(), flamePtr->getY())){    //check if Penelope dies due to flame
        playerPtr->setIsAlive(false);
        return;
    }else{
        while(actIt != actorList.end() && *actIt != nullptr){
            if((*actIt)->getCanBeBurned() == true && doesOverlap((*actIt)->getX(), (*actIt)->getY(), flamePtr->getX(), flamePtr->getY())){
                if((*actIt)->getPassable() == true){//it's either landmine or goodie.
                    if((*actIt)->getExplosive() == true && (*actIt)->getIsAlive() == true){//it's a landmine
                        explodeMine(*actIt, true);
                        return;
                    }else{//it's a goodie
                        (*actIt)->setIsAlive(false);
                        return;
                    }
                }else{//if the actor is not passable.
                     if((*actIt)->getCanBeInfected() && (*actIt)->getIsAlive()){//when citizen dies due to flame
                        (*actIt)->setIsAlive(false);
                        playSound(SOUND_CITIZEN_DIE);
                        cout << "citizen died " << endl;
                        increaseScore(-1000);
                        return;
                    }else if((*actIt)->getCanHoldVaccine() && (*actIt)->getIsAlive()){ //need to add case do differentiate the case when zombie dies. or it can be a goodie.
                        //it's a dumb zombie
                        (*actIt)->setIsAlive(false);
                        playSound(SOUND_ZOMBIE_DIE);
                        increaseScore(1000);
                        if((*actIt)->getNumVaccines() == 1 && (*actIt)->getIsAlive()){
                            VaccineGoodie* vaccineDropped =new VaccineGoodie(this, (*actIt)->getX(), (*actIt)->getY());
                            vaccineDropped->setIsFromZombie(true);
                            vaccineDropped->setCanBeBurned(false);
                            actorList.push_back(vaccineDropped);
                        }
                        return;
                    }else if(!(*actIt)->getCanHoldVaccine() && (*actIt)->getIsAlive()){ //it's a smart zombie
                        (*actIt)->setIsAlive(false);
                        playSound(SOUND_ZOMBIE_DIE);
                        increaseScore(2000);
                        return;
                    }
                }
            }
            actIt++;
        }
    }
}

void StudentWorld::blastFlame(){
    bool isBlocked = false;
    if(playerPtr->getDirection() == GraphObject::up){
        for(int i = 1; i < 4; i++){                 //I think it should be from 1 to 4 not 0 to 3 as in sepc....!!
            vector<Actor*>::iterator actIt = actorList.begin();
            while(actIt != actorList.end()){
                if((*actIt)->getBlockFireVomit() == true && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX(), playerPtr->getY() + i * SPRITE_HEIGHT)){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }
//                //from HERE!
//                else if((*actIt)->getBlockFireVomit() == false && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX(), playerPtr->getY() + i * SPRITE_HEIGHT)){
//                    if((*actIt)->getExplosive() == true){//when it overlaps with a mine.
//                        explodeMine(*actIt, true);
//                    }else if((*actIt)->getCanBeBurned()){//does not blcok fire & not explosive & cannot be burned
//                        (*actIt)->setIsAlive(false);
//                    }
//                }//to HERE!
                actIt++;
            }
            if(isBlocked == true){
                isBlocked = false;
                break;
            }
            actorList.push_back(new Flame(this, playerPtr->getX(), playerPtr->getY() + i * SPRITE_HEIGHT, GraphObject::up));
        }
    }else if(playerPtr->getDirection() == GraphObject::down){
        for(int i = 1; i < 4; i++){                 //I think it should be from 1 to 4 not 0 to 3 as in sepc....!!
            vector<Actor*>::iterator actIt = actorList.begin();
            while(actIt != actorList.end()){
                if((*actIt)->getBlockFireVomit() == true && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX(), playerPtr->getY() - i * SPRITE_HEIGHT)){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }
//                else if((*actIt)->getBlockFireVomit() == false && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX(), playerPtr->getY() - i * SPRITE_HEIGHT)){
//                    if((*actIt)->getExplosive() == true){
//                        explodeMine(*actIt, true);
//                    }else if((*actIt)->getCanBeBurned()){//does not blcok fire & not explosive & cannot be burned
//                        (*actIt)->setIsAlive(false);
//                    }
//                }
                actIt++;
            }
            if(isBlocked == true){
                isBlocked = false;
                break;
            }
            actorList.push_back(new Flame(this, playerPtr->getX(), playerPtr->getY() - i * SPRITE_HEIGHT, GraphObject::down));
        }
    }else if(playerPtr->getDirection() == GraphObject::left){
        for(int i = 1; i < 4; i++){                 //I think it should be from 1 to 4 not 0 to 3 as in sepc....!!
            vector<Actor*>::iterator actIt = actorList.begin();
            while(actIt != actorList.end()){
                if((*actIt)->getBlockFireVomit() == true && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX() - i * SPRITE_WIDTH, playerPtr->getY())){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }
//                else if((*actIt)->getBlockFireVomit() == false && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX() - i * SPRITE_WIDTH, playerPtr->getY())){
//                    if((*actIt)->getExplosive() == true){
//                        explodeMine(*actIt, true);
//                    }else if((*actIt)->getCanBeBurned()){//does not blcok fire & not explosive & cannot be burned
//                        (*actIt)->setIsAlive(false);
//                    }
//                }
                actIt++;
            }
            if(isBlocked == true){
                isBlocked = false;
                break;
            }
            actorList.push_back(new Flame(this, playerPtr->getX() - i * SPRITE_WIDTH, playerPtr->getY(), GraphObject::left));
        }
    }else if(playerPtr->getDirection() == GraphObject::right){
        for(int i = 1; i < 4; i++){                 //I think it should be from 1 to 4 not 0 to 3 as in sepc....!!
            vector<Actor*>::iterator actIt = actorList.begin();
            while(actIt != actorList.end()){
                if((*actIt)->getBlockFireVomit() == true && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX() + i * SPRITE_WIDTH, playerPtr->getY())){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }
//                else if((*actIt)->getBlockFireVomit() == false && doesOverlap((*actIt)->getX(), (*actIt)->getY(), playerPtr->getX() + i * SPRITE_WIDTH, playerPtr->getY())){
//                    if((*actIt)->getExplosive() == true){
//                        explodeMine(*actIt, true);
//                    }else if((*actIt)->getCanBeBurned()){//does not blcok fire & not explosive & cannot be burned
//                        (*actIt)->setIsAlive(false);
//                    }
//                }
                actIt++;
            }
            if(isBlocked == true){
                isBlocked = false;
                break;
            }
            actorList.push_back(new Flame(this, playerPtr->getX() + i * SPRITE_WIDTH, playerPtr->getY(), GraphObject::right));
        }
    }
}


void StudentWorld::incrementVaccine(){
    playerPtr->changeNumVaccines(1);
}
void StudentWorld::incrementFlameCount(){
    playerPtr->changeNumFlames(5);
}
void StudentWorld::incrementLandmineCount(){
    playerPtr->changeNumLandmines(2);
}

double StudentWorld::determineDistToPenelope(Actor* actorPtr){
    double distance;
    distance = sqrt(pow(actorPtr->getX() - playerPtr->getX(), 2) + pow(actorPtr->getY() - playerPtr->getY(), 2));
    return distance;
}

void StudentWorld::followPenelope(Actor *actorPtr){
    //if the citizen is on the same row or column as Penelope
    if(actorPtr->getX() == playerPtr->getX() || actorPtr->getY() == playerPtr->getY()){ //Citizen and Penelope are in the same column
        if(actorPtr->getX() == playerPtr->getX() && actorPtr->getY() < playerPtr->getY()){  // Penelope is above citizen
            moveCitizen(actorPtr, GraphObject::up);

        }else if(actorPtr->getX() == playerPtr->getX() && actorPtr->getY() > playerPtr->getY()){    // citizen is above Penelope
            moveCitizen(actorPtr, GraphObject::down);

        }else if(actorPtr->getX() < playerPtr->getX() && actorPtr->getY() == playerPtr->getY()){    // Penelope is standing right to citizen
            moveCitizen(actorPtr, GraphObject::right);

        }else if(actorPtr->getX() > playerPtr->getX() && actorPtr->getY() == playerPtr->getY()){    //Citizen is standing right to Penelope.
            moveCitizen(actorPtr, GraphObject::left);

        }
    }else{//citizen is not in the same row or column as Penelope.
        int horizontal = 0;
        int vertical = 1;
        int direction;
        direction = randInt(0, 1);
        if(direction == horizontal){
            if(actorPtr->getX() < playerPtr->getX()){//when Penelope is standing right to citizen
                if(!moveCitizen(actorPtr, GraphObject::right)){//if moving right is blocked, go vertically instead.
                    if(actorPtr->getY() < playerPtr->getY()){//when citizen is below Penelope.
                        moveCitizen(actorPtr, GraphObject::up);
                    }else if(actorPtr->getY() > playerPtr->getY()){//when citizen is above Penelope.
                        moveCitizen(actorPtr, GraphObject::down);
                    }
                }
            }else if(actorPtr->getX() > playerPtr->getX()){//when citizen is standing right to Penelope
                if(!moveCitizen(actorPtr, GraphObject::left)){//if moving left is blocked, go vertically instead
                    if(actorPtr->getY() < playerPtr->getY()){//when citizen is below Penelope.
                        moveCitizen(actorPtr, GraphObject::up);
                    }else if(actorPtr->getY() > playerPtr->getY()){//when citizen is above Penelope.
                        moveCitizen(actorPtr, GraphObject::down);
                    }
                }
            }
        }else if(direction == vertical){
            if(actorPtr->getY() < playerPtr->getY()){//when Penelope is standing above to citizen
                if(!moveCitizen(actorPtr, GraphObject::up)){//if moving up is blocked, go horizontally instead.
                    if(actorPtr->getX() < playerPtr->getX()){//when citizen is left to Penelope.
                        moveCitizen(actorPtr, GraphObject::right);
                    }else if(actorPtr->getX() > playerPtr->getX()){//when citizen is right Penelope.
                        moveCitizen(actorPtr, GraphObject::left);
                    }
                }
            }else if(actorPtr->getY() > playerPtr->getY()){//when Penelope is standing above to citizen
                if(!moveCitizen(actorPtr, GraphObject::down)){//if moving down is blocked, go horizontally instead.
                    if(actorPtr->getX() < playerPtr->getX()){//when citizen is left to Penelope.
                        moveCitizen(actorPtr, GraphObject::right);
                    }else if(actorPtr->getX() > playerPtr->getX()){//when citizen is right Penelope.
                        moveCitizen(actorPtr, GraphObject::left);
                    }
                }
            }
        }
    }
}


//returns false when it failed to move.
bool StudentWorld::moveCitizen(Actor* actorPtr, Direction dir){
    bool movedSuccessfully = false;
    switch (dir) {
        case GraphObject::right:
            if(doesIntersect(actorPtr, actorPtr->getX() + 2, actorPtr->getY()) == false){
                actorPtr->setDirection(GraphObject::right);
                actorPtr->moveTo(actorPtr->getX() + 2, actorPtr->getY());
                movedSuccessfully = true;
            }
            break;
        case GraphObject::left:
            if(doesIntersect(actorPtr, actorPtr->getX() - 2, actorPtr->getY()) == false){
                actorPtr->setDirection(GraphObject::left);
                actorPtr->moveTo(actorPtr->getX() - 2, actorPtr->getY());
                movedSuccessfully = true;
            }
            break;
        case GraphObject::up:
            if(doesIntersect(actorPtr, actorPtr->getX(), actorPtr->getY() + 2) == false){
                actorPtr->setDirection(GraphObject::up);
                actorPtr->moveTo(actorPtr->getX(), actorPtr->getY() + 2);
                movedSuccessfully = true;
            }
            break;
        case GraphObject::down:
            if(doesIntersect(actorPtr, actorPtr->getX(), actorPtr->getY() - 2) == false){
                actorPtr->setDirection(GraphObject::down);
                actorPtr->moveTo(actorPtr->getX(), actorPtr->getY() - 2);
                movedSuccessfully = true;
            }
            break;
    }
    return movedSuccessfully;
}

//Returns true if successfully threw the vomit to Human
bool StudentWorld::throwVomit(Zombie *zombiePtr){
    int chooseToVomit = 1;
    if(doesOverlapWithPlayer(zombiePtr->getVomitX(), zombiePtr->getVomitY())){
        int vomitDecision = randInt(1, 3);
        if(vomitDecision == chooseToVomit){
            actorList.push_back(new Vomit(this, zombiePtr->getVomitX(), zombiePtr->getVomitY(), zombiePtr->getDirection()));
            playSound(SOUND_ZOMBIE_VOMIT);
            return true;
        }
    }else{
        vector<Actor*>::iterator actIt = actorList.begin();
        while(actIt != actorList.end() && *actIt != nullptr){
            if((*actIt)->getCanBeInfected() == true && doesOverlap((*actIt)->getX(), (*actIt)->getY(), zombiePtr->getVomitX(), zombiePtr->getVomitY())){
                int vomitDecision = randInt(1, 3);
                if(vomitDecision == chooseToVomit){
                    actorList.push_back(new Vomit(this, zombiePtr->getVomitX(), zombiePtr->getVomitY(), zombiePtr->getDirection()));
                    playSound(SOUND_ZOMBIE_VOMIT);
                    return true;
                }
            }
            actIt++;
        }
    }
    return false;
}

bool StudentWorld::findClosestPerson(Zombie *zombiePtr){
    double distanceToPenelope = determineDistToPenelope(zombiePtr);
    double distanceToCitizen = 10000;
    double minDistance;
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end() && *actIt != nullptr){

        double temp = sqrt(pow(zombiePtr->getX() - (*actIt)->getX(), 2) + pow(zombiePtr->getY() - (*actIt)->getY(), 2));
        if(temp <= distanceToCitizen){
            distanceToCitizen = temp;
        }
        actIt++;
    }
    if(distanceToPenelope > distanceToCitizen){
        minDistance = distanceToCitizen;
    }else{
        minDistance = distanceToPenelope;
    }
    
    if(minDistance > 80){
        return false;
    }else{
        if(distanceToPenelope <= distanceToCitizen){    //follow Penelope.
            
        }
        return true;
    }
    
}
