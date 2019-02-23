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

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath)
{
    actorList.reserve(256);
    playerPtr = nullptr;
    m_userScore = 0;
    m_finishedLevel = false;
}

int StudentWorld::init(){
    Level lev(assetPath());
    int level;
    level = getLevel();
//    cout << "level : " << level << endl;
    string levelFile;

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
                        }
                    }
                }
            }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    playerPtr->doSomething();
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        (*actIt)->doSomething();
        actIt++;
    }
    if(getFinishedLevel()){
        playSound(SOUND_LEVEL_FINISHED);
        setFinishedLevel(false);
        
        //What do I do when the player completes all 6 stages????
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
                if((*actIt)->getPassable() == false){//if the destination object is not passable return true
                    return true;
                }
            }
        actIt++;
    }
    return false;
}

//The distance between the center points should be less than or equal to 10 pixels..
bool StudentWorld::doesOverlap(Actor *sameActor, double x, double y){
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        double distance;
        distance = sqrt(pow(sameActor->getX() - x, 2) + pow(sameActor->getY() - y, 2));
        if(sameActor != (*actIt) && distance <= 10.0){
            return true;
        }
        actIt++;
    }
    return false;
}

void StudentWorld::escapeHumans(double exitX, double exitY){
    bool freedAllCitizens = true;
    
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        if((*actIt)->getCanUseExit()){
            if(doesOverlap(*actIt, exitX, exitY)){//when exit overlaps with citizen, free the citizen
                changeScore(500);
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
