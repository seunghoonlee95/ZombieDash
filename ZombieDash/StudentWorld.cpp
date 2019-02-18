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
//    Penelope player(IID_PLAYER, 200, 160, GraphObject::right, 0, 1.0);
    Penelope* player = new Penelope(IID_PLAYER, 200, 160, GraphObject::right, 0, 1.0);
    playerList.push_back(player);
 //=============================================
//    Level* lev;
//    int level;
//    string levelFile;
//    Level::LoadResult result;
//    level = getLevel();
//    cout << "Level : " << level << endl;
//    switch(level){
//        case 1:
//            levelFile = assetPath() + "/" + "level01.txt";
//            cout << "levelFile : " << levelFile << endl;
//             result = lev->loadLevel(levelFile);
//            break;
//        case 2:
//            levelFile = assetPath() + "/" + "level02.txt";
//            result = lev->loadLevel(levelFile);
//            break;
//        case 3:
//            levelFile = assetPath() + "/" + "level03.txt";
//            result = lev->loadLevel(levelFile);
//            break;
//        case 4:
//            levelFile = assetPath() + "/" + "level04.txt";
//            result = lev->loadLevel(levelFile);
//            break;
//        case 5:
//            levelFile = assetPath() + "/" + "level05.txt";
//            result = lev->loadLevel(levelFile);
//            break;
//        case 6:
//            levelFile = assetPath() + "/" + "level06.txt";
//            result = lev->loadLevel(levelFile);
//            break;
//    }
//    if(result == Level::load_fail_file_not_found){
//        cerr << "Could not find level0" << level << ".txt file!" << endl;
//        exit(1);
//    }else if(result == Level::load_fail_bad_format){
//        cerr << "Your level was improperly formatted!" << endl;
//        exit(1);
//    }else if(result == Level::load_success){
//        cerr << "Successfully loaded level" << endl;
//        Level::MazeEntry ge;
//        for(int x = 0; x < VIEW_WIDTH / SPRITE_WIDTH; x++){
//            for(int y = 0; y < VIEW_HEIGHT / SPRITE_HEIGHT; y++){
//                ge = lev->getContentsOf(x, y);
//                switch(ge){
//                    case Level::empty:
//                        break;
//                    case Level::player:
//                        actorList.push_back(new Penelope(IID_PLAYER, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, GraphObject::right,0 , 1.0));
//                        cout << "Penelope starting x : " << SPRITE_WIDTH * x << "/ starting y : " << SPRITE_HEIGHT * y << endl;
//                        break;
//                    case Level::wall:
//                        actorList.push_back(new Wall(IID_WALL, SPRITE_WIDTH * x, SPRITE_HEIGHT * y, GraphObject::right, 0, 1.0));
//                        cout << "Wall starting x : " << SPRITE_WIDTH * x << "/ starting y : " << SPRITE_HEIGHT * y << endl;
//                        break;
//                }
//            }
//        }
//    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
//    vector<Actor*>::iterator it = actorList.begin();
//    while(it != actorList.end()){
//        (*it)->doSomething();
//        it++;
//    }
    //playerList[0]->doSomething();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
//    vector<Actor*>::iterator it;
//    it = actorList.begin();
//    while(it != actorList.end()){
//        delete(*it);
//    }
//    actorList.clear();
}

bool StudentWorld::doesIntersect(int x, int y){
    vector<Actor*>::iterator it = actorList.begin();
    while(it != actorList.end()){
        if((*it)-> getActorType() == "Penelope" || (*it)-> getActorType() == "Wall" || (*it)-> getActorType() == "Citizen" || (*it)-> getActorType() == "Zombie" ){
            if(abs((*it)->getX() - x) < 16 && abs((*it)->getY() - y) < 16){
                return false;
            }
            
        }
        it++;
    }
    return true;
}
