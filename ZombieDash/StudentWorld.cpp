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
    }else if (result == Level::load_success){
        cerr << "Successfully loaded level" << endl;
        for(double x = 0; x < VIEW_WIDTH / SPRITE_WIDTH; x++){
            for(double y = 0; y < VIEW_HEIGHT / SPRITE_HEIGHT; y++){
                Level::MazeEntry ge = lev.getContentsOf(x,y); // level_x=5, level_y=10 switch (ge) // so x=80 and y=160
                switch (ge){
                    case Level::empty:
                    case Level::smart_zombie:
                        break;
                    case Level::dumb_zombie:
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
            delete *actIt;
            actorList.erase(actIt);
            //  actIt++;      //this was causing the error!!
        }
        else{
                (*actIt)->doSomething();
                actIt++;
        }
    }

    if(getFinishedLevel()){
        playSound(SOUND_LEVEL_FINISHED);
        setFinishedLevel(false);
        
        //Whhen the player completes all 6 stages..
        if(getLevel() == 6){
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
bool StudentWorld::doesIntersect(Actor* sameActor, double x, double y){
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        if(sameActor != (*actIt) && abs((*actIt)->getX() - x) < 16 && abs((*actIt)->getY() - y) < 16){
            if((*actIt)->getPassable() == false){//if the destination object is not passable(wall or citizen only among actorList) return true
                return true;
            }
        }
        actIt++;
    }
    return false;
}

//The distance between the center points should be less than or equal to 10 pixels..
bool StudentWorld::doesOverlap(Actor *actorPtr, double otherX, double otherY){
    double distance;
    distance = sqrt(pow(actorPtr->getX() - otherX, 2) + pow(actorPtr->getY() - otherY, 2));
    if(distance <= 10.0){
        //            cout << "distance : " << distance << endl;
        return true;
    }
    return false;
}

void StudentWorld::escapeHumans(double exitX, double exitY){
    bool freedAllCitizens = true;
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        if((*actIt)->getCanUseExit()){//only citizen can use exit among ActorList.
            if(doesOverlap(*actIt, exitX, exitY)){//when exit overlaps with citizen, free the citizen
                increaseScore(500);
                (*actIt)->kill();
                playSound(SOUND_CITIZEN_SAVED);
            }
            freedAllCitizens = false;
        }
        actIt++;
    }
    //Now check if exit overlaps with Penelope
    if(doesOverlap(playerPtr, exitX, exitY)){
        if(freedAllCitizens){
            setFinishedLevel(true);
        }
    }
}

void StudentWorld::explodeMine(Actor* minePtr, bool damagedByFlame){
    if(doesOverlap(playerPtr, minePtr->getX(), minePtr->getY()) || damagedByFlame){//overlaps with a player
//        cout<<"player overlaps with the mine!! "<< endl;
//        exit(1);
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
    }
}

void StudentWorld::plantLandmine(){
    actorList.push_back(new Landmine(this, playerPtr->getX()- SPRITE_WIDTH / 2, playerPtr->getY() - SPRITE_HEIGHT / 2));
}

bool StudentWorld::doesOverlapWithPlayer(Actor* goodie){
    if(doesOverlap(goodie, playerPtr->getX(), playerPtr->getY())){
        return true;
    }
    return false;
}

void StudentWorld::damageObjects(Actor* flamePtr){
    vector<Actor*>::iterator actIt = actorList.begin();
    if(doesOverlapWithPlayer(flamePtr)){
        playerPtr->setIsAlive(false);
//        cout << "flame overlaps with the player!! player dies.." << endl;
    }else{
        while(actIt != actorList.end() && *actIt != nullptr){
//                    if(*actIt == nullptr){
//                        cout << "nullptr!!! exiting!!!" << endl;
//                        exit(1);
//                    }
            if((*actIt)->getCanBeBurned() == true && doesOverlap(*actIt, flamePtr->getX(), flamePtr->getY())){
                if((*actIt)->getExplosive() == true){
                    explodeMine(*actIt, true);
                }else{
                    (*actIt)->setIsAlive(false);
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
                if((*actIt)->getCanBeBurned() == false && doesOverlap(*actIt, playerPtr->getX(), playerPtr->getY() + i * SPRITE_HEIGHT)){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }else if((*actIt)->getCanBeBurned() == true && doesOverlap(*actIt, playerPtr->getX(), playerPtr->getY() + i * SPRITE_HEIGHT)){
                    if((*actIt)->getExplosive() == true){
                        explodeMine(*actIt, true);
                    }else{
                        (*actIt)->setIsAlive(false);
                    }
                }
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
                if((*actIt)->getCanBeBurned() == false && doesOverlap(*actIt, playerPtr->getX(), playerPtr->getY() - i * SPRITE_HEIGHT)){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }else if((*actIt)->getCanBeBurned() == true && doesOverlap(*actIt, playerPtr->getX(), playerPtr->getY() - i * SPRITE_HEIGHT)){
                    if((*actIt)->getExplosive() == true){
                        explodeMine(*actIt, true);
                    }else{
                        (*actIt)->setIsAlive(false);
                    }
                }
                actIt++;
            }
            if(isBlocked == true){
                isBlocked = false;
                break;
            }
            actorList.push_back(new Flame(this, playerPtr->getX(), playerPtr->getY() - i * SPRITE_HEIGHT, GraphObject::up));
        }
    }else if(playerPtr->getDirection() == GraphObject::left){
        for(int i = 1; i < 4; i++){                 //I think it should be from 1 to 4 not 0 to 3 as in sepc....!!
            vector<Actor*>::iterator actIt = actorList.begin();
            while(actIt != actorList.end()){
                if((*actIt)->getCanBeBurned() == false && doesOverlap(*actIt, playerPtr->getX() - i * SPRITE_WIDTH, playerPtr->getY())){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }else if((*actIt)->getCanBeBurned() == true && doesOverlap(*actIt, playerPtr->getX() - i * SPRITE_WIDTH, playerPtr->getY())){
                    if((*actIt)->getExplosive() == true){
                        explodeMine(*actIt, true);
                    }else{
                        (*actIt)->setIsAlive(false);
                    }
                }
                actIt++;
            }
            if(isBlocked == true){
                isBlocked = false;
                break;
            }
            actorList.push_back(new Flame(this, playerPtr->getX() - i * SPRITE_WIDTH, playerPtr->getY(), GraphObject::up));
        }
    }else if(playerPtr->getDirection() == GraphObject::right){
        for(int i = 1; i < 4; i++){                 //I think it should be from 1 to 4 not 0 to 3 as in sepc....!!
            vector<Actor*>::iterator actIt = actorList.begin();
            while(actIt != actorList.end()){
                if((*actIt)->getCanBeBurned() == false && doesOverlap(*actIt, playerPtr->getX() + i * SPRITE_WIDTH, playerPtr->getY())){
                    //when flame overlaps with an exit or wall
                    isBlocked = true;
                    break;
                }else if((*actIt)->getCanBeBurned() == true && doesOverlap(*actIt, playerPtr->getX() + i * SPRITE_WIDTH, playerPtr->getY())){
                    if((*actIt)->getExplosive() == true){
                        explodeMine(*actIt, true);
                    }else{
                        (*actIt)->setIsAlive(false);
                    }
                }
                actIt++;
            }
            if(isBlocked == true){
                isBlocked = false;
                break;
            }
            actorList.push_back(new Flame(this, playerPtr->getX() + i * SPRITE_WIDTH, playerPtr->getY(), GraphObject::up));
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
