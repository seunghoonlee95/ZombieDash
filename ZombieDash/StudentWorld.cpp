#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//returns a pointer to GameWorld!!!- seems useful...?
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

//Default constructor...(Not sure tho,,,,)
StudentWorld::StudentWorld() : GameWorld(""){
    
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath)
{
    
}

int StudentWorld::init()
{
    Level lev(assetPath());
    int level;
    level = getLevel();
    cout << "level : " << level << endl;
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
                        switch (ge)
                        {
                            case Level::empty:
                               // cout << "Location (" << x << ", " << y << ") is empty" << endl;
                            case Level::smart_zombie:
                              //  cout << "Location (" << x << ", " << y << ") is smart_zombie" << endl;
                                break;
                            case Level::dumb_zombie:
                              //  cout << "Location (" << x << ", " << y << ") is dumb_zombie" << endl;
                                break;
                            case Level::player:
//                                cout << "Location (" << x << ", " << y << ") is player" << endl;
                                playerList.push_back(new Penelope(this, IID_PLAYER, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, GraphObject::right,0 , 1.0));
                                break;
                            case Level::exit:
                              //  cout << "Location (" << x << ", " << y << ") is exit" << endl;
                                break;
                            case Level::wall:
//                                cout << "Location (" << x << ", " << y << ") is wall" << endl;
                                actorList.push_back(new Wall(this, IID_WALL, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, GraphObject::right, 0, 1.0));
                                break;
                            case Level::pit:
                               // cout << "Location (" << x << ", " << y << ") is pit" << endl;
                                break;
                        }
                    }
                }
            }
 //=====================================================================================
//    Penelope* player = new Penelope(IID_PLAYER, 200, 160, GraphObject::right, 0, 1.0);
//    playerList.push_back(player);
//    Wall* wall = new Wall(IID_WALL, 1, 1, GraphObject::right, 0, 1.0);
//    actorList.push_back(wall);
 //=============================================

         
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
        (*actIt)->doSomething();
        actIt++;
    }
    
    vector<Penelope*>::iterator playerIt = playerList.begin();
    (*playerIt)->doSomething();
    

    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
    return GWSTATUS_CONTINUE_GAME ;
}

void StudentWorld::cleanUp()
{
//    vector<Actor*>::iterator actIt;
//    actIt = actorList.begin();
//    while(actIt != actorList.end()){
//        delete(*actIt);
//        actIt++;
//    }
//
//    vector<Penelope*>::iterator playerIt;
//    playerIt = playerList.begin();
//    while(playerIt != playerList.end()){
//        delete(*playerIt);
//        playerIt++;
//    }
    actorList.clear();
    playerList.clear();
}

bool StudentWorld::doesIntersect(int x, int y){
    vector<Actor*>::iterator actIt = actorList.begin();
    while(actIt != actorList.end()){
            if(abs((*actIt)->getX() - x) < 16 && abs((*actIt)->getY() - y) < 16){
                return true;
            }
        actIt++;
    }
    
//    vector<Penelope*>::iterator playerIt = playerList.begin();
//    if(abs((*playerIt)->getX() - x) < 16 && abs((*playerIt)->getY() - y) < 16){
//        return true;
//    }
    
    return false;
}
