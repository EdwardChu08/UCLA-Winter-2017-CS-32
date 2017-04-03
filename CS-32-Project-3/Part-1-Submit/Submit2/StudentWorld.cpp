#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

///////////////////////////////////////////////
//FUNCTION IMPLEMENTATION
///////////////////////////////////////////////

#include "Actor.h"
#include "Field.h"
#include <list>
#include <string>

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    
}

StudentWorld::~StudentWorld(){
    cleanUp();
}



//INIT

int StudentWorld::init(){
    
    m_ticksElapsed = 0;
    
    //Provided
    std::string fieldFileName;
    Field f;
    
    std::string fieldFile = getFieldFilename();
    
    if (f.loadField(fieldFile) != Field::LoadResult::load_success)
        return false; // something bad happened!
    
    //Loads field.h data into m_field and m_actors
    for(int x = 0; x < VIEW_WIDTH; x++){
        for(int y = 0; y < VIEW_HEIGHT; y++){
            
            switch(f.getContentsOf(x,y)){
                    
                case Field::FieldItem::rock:{
                    m_field[y][x].push_back(new Pebble(x,y,this));
                    break;
                }
                    
                case Field::FieldItem::grasshopper:{
                    m_field[y][x].push_back(new BabyGrasshopper(x,y,this));
                    break;
                }
                    
                case Field::FieldItem::food: {
                    m_field[y][x].push_back(new Food(x,y,6000,this));
                    break;
                }
                    
                case Field::FieldItem::poison: {
                    m_field[y][x].push_back(new Poison(x,y,this));
                    break;
                }
                    
                case Field::FieldItem::water: {
                    m_field[y][x].push_back(new WaterPool(x,y,this));
                    break;
                }
                    
                default:;
                //Incorrect field input
            }
            
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
    

}



//MOVE: Called every tick
//TODO::
int StudentWorld::move()
{
    updateTickCount();
    
    //Do something for each actor
    
    for(int x = 0; x < VIEW_WIDTH; x++){
        for(int y = 0; y < VIEW_HEIGHT; y++){
            for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
    
                if((*it) == nullptr) continue;
                
                int origX = (*it)->getX();
                int origY = (*it)->getY();
                
                if((*it)->isAlive())
                    (*it)->doSomething();
                
                if(origX != (*it)->getX() || origY != (*it)->getY())
                    moveActor(it, origX, origY);
            }
        }
    }
    
    //Activate Damaging Objects
    for(int x = 0; x < VIEW_WIDTH; x++){
        for(int y = 0; y < VIEW_HEIGHT; y++){
            for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
                
                if((*it) == nullptr) continue;
                if(!(*it)->isDamagingObject()) continue;
                
                //We're sure that it's a damagingObject now
                DamagingObject* pp = dynamic_cast<DamagingObject*>(*it);
                
                pp->damageInsects();
                
            }
        }
    }
    
    //Remove dead actors
    for(int x = 0; x < VIEW_WIDTH; x++){
        for(int y = 0; y < VIEW_HEIGHT; y++){
            for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
                if((*it) == nullptr) continue;
                
                if(!(*it)->isAlive()){
                    //Remove from 3D array
                    delete *it;
                    m_field[y][x].erase(it);
                    
                }    
            }
        }
    }
    
    setDisplayText();
    
    if(m_ticksElapsed >= 2000){
        
        //TODO: Check if there's an ant winning
        
        return GWSTATUS_NO_WINNER;
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}



//CLEANUP

void StudentWorld::cleanUp()
{
    for(int x = 0; x < VIEW_WIDTH; x++){
        for(int y = 0; y < VIEW_HEIGHT; y++){
            for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
                delete *it;
                m_field[y][x].erase(it);
            }            
        }
    }
    
}

//FUNCTIONS FOR ACTORS

//Allows actors to check if they can move onto a square
bool StudentWorld::ableToMoveHere(int x, int y){
    
    if(x < 0 || y < 0) return false;
    if(x >= VIEW_WIDTH || y >= VIEW_HEIGHT) return false;
    
    for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        if((*it)->blocksInsect())
            return false;
    }
    return true;
}

//Returns amount of food eaten
int StudentWorld::eatFoodHere(int x, int y, int amount){
    for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        Food* pp = dynamic_cast<Food*>(*it);
        if(pp != nullptr && pp->isAlive()){
            return pp->eat(amount);
        }
    }
    
    return 0;
}


int StudentWorld::addFoodHere(int x, int y, int amount){
    
    for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        Food* pp = dynamic_cast<Food*>(*it);
        if(pp != nullptr){
            return pp->eat(-amount);
        }
    }
    
    m_field[y][x].push_back(new Food(x, y, amount, this));
    
    return amount;    
}

//Return false if spawning on a pebble or out of bounds, else return true
bool StudentWorld::spawnAdultGrasshopper(int x, int y){
    if(!ableToMoveHere(x,y)) return false;
    
    m_field[y][x].push_back(new AdultGrasshopper(x, y, this));
    
    return true;
}


void StudentWorld::damageInsects(Actor* attacker, int x, int y, int damageType, int damage, bool damageAll){
    
    Actor* *insectsHere = new Actor* [m_field[y][x].size()];
    int insectCount = 0;
    
    for(std::list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        if((*it) != nullptr && (*it) != attacker && (*it)->isInsect()){
            insectsHere[insectCount] = *it;
            insectCount++;
        }
    }
    
    if(insectCount == 0) return;
    
    if(damageAll){
        for(int i = 0; i < insectCount; i++){
            insectsHere[i]->getHurt(damageType, damage);
        }
    } else {
        int victim = randInt(0, insectCount-1);
        insectsHere[victim]->getHurt(damageType, damage);
    }
    
    delete[] insectsHere;
    
}

///////////////////////////////////////////////
//HELPER FUNCTIONS
///////////////////////////////////////////////


//TODO: Update display text
void StudentWorld::setDisplayText(){
    int ticks = getCurrentTicks();
    
    int antsAnt0, antsAnt1, antsAnt2, antsAnt3; int winningAntNumber;
    antsAnt0 = 0; //getNumberOfAntsForAnt(0);
    antsAnt1 = 0; //getNumberOfAntsForAnt(1);
    antsAnt2 = 0; //getNumberOfAntsForAnt(2);
    antsAnt3 = 0; //getNumberOfAntsForAnt(3);
    winningAntNumber = 0; //getWinningAntNum();
    
    // Create a string from your statistics, of the form:
    // Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
    string s = "Ticks: ";
    s += to_string(ticks);
    //s += prettyFunction(antsAnt0, antsAnt1, antsAnt2, antsAnt3, winningAntNumber);
    s += "  - NachenBug*: 0 IceBug: 0 SmallBug: 0 GoldBug: 0";
    
    
    setGameStatText(s);
}


//Updates m_field to show actor at new position
void StudentWorld::moveActor(std::list<Actor*>::iterator it, int origX, int origY){
    m_field[(*it)->getY()][(*it)->getX()].push_back(*it);
    
    m_field[origY][origX].erase(it);
    
}

