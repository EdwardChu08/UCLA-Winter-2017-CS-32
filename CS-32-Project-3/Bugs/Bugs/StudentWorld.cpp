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
#include "Compiler.h"
#include <list>
#include <string>

//#include <iostream>

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
    m_maxNumAnt = 5;
    m_winningEntrantNum = -1;
    
    for(int i = 0; i < MAX_ANT_COLONIES; i++){
        m_numAnts[i] = 0;
    }
    
    //Provided, get compiler for entrants
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    
    for(int i = 0; i < MAX_ANT_COLONIES; i++){
        m_compilerForEntrant[i] = new Compiler;
        std::string error;
        // compile the source file… If the compile function returns
        // false, there was a syntax error during compilation!
        if (fileNames.size() > i && ! m_compilerForEntrant[i]->compile(fileNames[i], error) )
        {
            
            setError(fileNames[i] + " " + error);
            // return an error to tell our simulation framework
            // that something went wrong, and it’ll inform the user
            return GWSTATUS_LEVEL_ERROR;
        }
    }
    
    //Provided, get field
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
                    
                case Field::FieldItem::anthill0: {
                    m_field[y][x].push_back(new Anthill(x,y,0,m_compilerForEntrant[0],this));
                    break;
                }
                    
                case Field::FieldItem::anthill1: {
                    m_field[y][x].push_back(new Anthill(x,y,1,m_compilerForEntrant[1],this));
                    break;
                }
                    
                case Field::FieldItem::anthill2: {
                    m_field[y][x].push_back(new Anthill(x,y,2,m_compilerForEntrant[2],this));
                    break;
                }
                
                case Field::FieldItem::anthill3: {
                    m_field[y][x].push_back(new Anthill(x,y,3,m_compilerForEntrant[3],this));
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
int StudentWorld::move()
{
    updateTickCount();
    
    //Do something for each actor
    for(int x = 0; x < VIEW_WIDTH; x++){
        for(int y = 0; y < VIEW_HEIGHT; y++){
            for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); ){
    
                if((*it) == nullptr) continue;
                
                int origX = (*it)->getX();
                int origY = (*it)->getY();
                
                if((*it)->isAlive())
                    (*it)->doSomething();
                
                if(origX != (*it)->getX() || origY != (*it)->getY()){
                    m_field[(*it)->getY()][(*it)->getX()].push_back(*it);
                    it = m_field[origY][origX].erase(it);
                } else {
                    it++;
                }
            }
        }
    }
    
    //Activate Damaging Objects (Water pools and poison)
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
            for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); ){
        
                if((*it) == nullptr) continue;
                
                if(!(*it)->isAlive()){
                    //Remove from 3D array
                    delete *it;
                    it = m_field[y][x].erase(it);
                    
                } else {
                    it++;
                }
            }
        }
    }
    
    updateWinner(m_compilerForEntrant);
    setDisplayText();
    
    if(m_ticksElapsed >= 2000){
        
        if(m_winningEntrantNum != -1)
            return GWSTATUS_PLAYER_WON;
        
        return GWSTATUS_NO_WINNER;
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}



//CLEANUP

void StudentWorld::cleanUp()
{
    for(int x = 0; x < VIEW_WIDTH; x++){
        for(int y = 0; y < VIEW_HEIGHT; y++){
            for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); ){
                delete *it;
                it = m_field[y][x].erase(it);
            }            
        }
    }
    
    for(int i = 0; i < MAX_ANT_COLONIES; i++){
        delete m_compilerForEntrant[i];
        m_compilerForEntrant[i] = nullptr; //Because cleanUp may be called twice
    }
    
}

//FUNCTIONS FOR ACTORS

//Allows actors to check if they can move onto a square
bool StudentWorld::ableToMoveHere(int x, int y) const{
    
    if(x < 0 || y < 0) return false;
    if(x >= VIEW_WIDTH || y >= VIEW_HEIGHT) return false;
    
    for(list<Actor*>::const_iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        if((*it)->blocksInsect())
            return false;
    }
    return true;
}

//Returns amount of food eaten
int StudentWorld::eatFoodHere(int x, int y, int amount){
    if(amount < 0) return 0;
    
    for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        Food* pp = dynamic_cast<Food*>(*it);
        if(pp != nullptr && pp->isAlive()){
            return pp->eat(amount);
        }
    }
    
    return 0;
}

bool StudentWorld::hasFoodHere (int x, int y) const{
    
    for(list<Actor*>::const_iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        Food* pp = dynamic_cast<Food*>(*it);
        if(pp != nullptr && pp->getHitpoints() > 0){
            return true;
        }
    }
    
    return false;;
}

int StudentWorld::addFoodHere(int x, int y, int amount){
    if(amount <= 0) return 0;
    
    for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        Food* pp = dynamic_cast<Food*>(*it);
        if(pp != nullptr){
            return pp->add(amount);
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

bool StudentWorld::spawnAnt(Anthill *self){
    if(self->getHitpoints() < 2000)
        return false; //No idea how you got here, should never happen
    
    int x = self->getX();
    int y = self->getY();
    
    m_field[y][x].push_back(new Ant(x, y, self->getColonyNum(), self->getCompiler(), this));
    m_numAnts[self->getColonyNum()]++;
    
    return true;
}

//Return number of insects damaged
int StudentWorld::damageInsects(Actor* attacker, int x, int y, int damageType, int damage, bool damageAll){
    
    //Dynamic Array storing all attackable insects on this square
    Actor* *insectsHere = new Actor* [m_field[y][x].size()];
    int insectCount = 0;
    
    //So that ants don't attack other ants from same colony
    Ant* antAttacker = dynamic_cast<Ant*>(attacker);
    int attackerColonyNum = -1;
    if(antAttacker != nullptr){
        attackerColonyNum = antAttacker->getColonyNum();
    }
    
    for(std::list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        if((*it) != nullptr && (*it) != attacker && (*it)->isInsect()){
            
            //To prevent ants from attacking other ants from same colony, for ant attackers
            if(attackerColonyNum != -1 && (*it)->isAnt()){
                Ant* antVictim = dynamic_cast<Ant*>(*it);
                if(attackerColonyNum != antVictim->getColonyNum()){
                    insectsHere[insectCount] = *it;
                    insectCount++;
                    continue;
                }
            }
            
            //Otherwise
            insectsHere[insectCount] = *it;
            insectCount++;
        }
    }
    
    //No insects to attack!
    if(insectCount == 0) return insectCount;
    
    //For damaging objects (Water pools and poison)
    if(damageAll){
        for(int i = 0; i < insectCount; i++){
            insectsHere[i]->getHurt(damageType, damage);
        }
    }
    //For ants and grasshopper attacking
    else {
        int victim = randInt(0, insectCount-1);
        insectsHere[victim]->getHurt(damageType, damage);
    }
    
    delete[] insectsHere;
    return insectCount;
}

bool StudentWorld::isStandingWithEnemy (Ant *self) const{
    
    return hasEnemyHere(self, self->getX(), self->getY(), false);
}

bool StudentWorld::hasEnemyHere(Ant *self, int x, int y, bool checkPoison) const{
    
    for(list<Actor*>::const_iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        //Grasshoppers are enemies!
        if((*it)->isGrasshopper())
            return true;
        
        //Check if other ant is an enemy
        if((*it)->isAnt()){
            Ant* pp = dynamic_cast<Ant*>(*it);
            if(pp->getColonyNum() != self->getColonyNum())
                return true;
        }
        
        //Check for poison?
        if(checkPoison && (*it)->isPoison())
            return true;
    }
    return false; //No enemy founds
}

bool StudentWorld::isOnOwnAnthill(Ant *self) const{
    int x = self->getX();
    int y = self->getY();
    
    for(list<Actor*>::const_iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        if((*it)->isAnthill()){
            Anthill* pp = dynamic_cast<Anthill*>(*it);
            if(pp->getColonyNum() == self->getColonyNum())
                return true;
        }
    }
    return false;
}

bool StudentWorld::hasPheromoneHere(Ant *self, int x, int y) const{
    
    for(list<Actor*>::const_iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        if((*it)->isPheromone()){
            Pheromone* pp = dynamic_cast<Pheromone*>(*it);
            if(pp->getColonyNum() == self->getColonyNum())
                return true;
        }
    }
    return false;
}

//Return pheromone strength here
int StudentWorld::emitPheromone(Ant *self){
    int x = self->getX();
    int y = self->getY();
    int colonyNum = self->getColonyNum();
    
    for(list<Actor*>::iterator it = m_field[y][x].begin(); it != m_field[y][x].end(); it++){
        
        if((*it) == nullptr) continue;
        
        Pheromone* pp = dynamic_cast<Pheromone*>(*it);
        if(pp != nullptr && pp->getColonyNum() == colonyNum){
            pp->addStrength(256);
            return pp->getHitpoints();
        }
    }
    
    m_field[y][x].push_back(new Pheromone(x,y,colonyNum,this));
    return 256;
}


///////////////////////////////////////////////
//HELPER FUNCTIONS
///////////////////////////////////////////////


void StudentWorld::setDisplayText(){
    int ticksLeft = NUMBER_OF_TICKS_TO_SIMULATE - getCurrentTicks();
    
    int antsAnt[4]; int winningAntNumber;
    for(int i = 0; i < MAX_ANT_COLONIES; i++){
        antsAnt[i] = getNumAnts(i);
    }
    winningAntNumber = m_winningEntrantNum;
    
    // Create a string from your statistics, of the form:
    // Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
    std::string output = formatDisplayText(ticksLeft, m_numAnts, m_winningEntrantNum, m_compilerForEntrant);
    
    
    setGameStatText(output);
}

/*
//Updates m_field to show actor at new position
void StudentWorld::moveActor(std::list<Actor*>::iterator it, int origX, int origY){
    m_field[(*it)->getY()][(*it)->getX()].push_back(*it);
    
    m_field[origY][origX].erase(it);
    
}
*/

//At the end of move(), update if there's a new winner
int StudentWorld::updateWinner(Compiler *cps[]){
    for(int i = 0; i < MAX_ANT_COLONIES; i++){
        if(getNumAnts(i) > m_maxNumAnt){
            m_winningEntrantNum = i;
            m_maxNumAnt = getNumAnts(i);
            setWinner(cps[i]->getColonyName());
        }
    }
    return m_winningEntrantNum;
}


std::string StudentWorld::formatDisplayText(int ticksLeft, int antAnt[], int winningAntNum, Compiler *cps[]){
    std::string s = "Ticks:";
    
    std::string stringTicksLeft = to_string(ticksLeft);
    for(int i = 0; i < 5-stringTicksLeft.size(); i++){
        s += " ";
    }
    s += stringTicksLeft;
    
    s += " - ";
    for(int i = 0; i < MAX_ANT_COLONIES; i++){
        s += cps[i]->getColonyName();
        if(i == winningAntNum)
            s += "*";
        s += ": ";
        
        if(antAnt[i] < 10)
            s += "0";
        
        s += to_string(antAnt[i]);
        s += " ";
    }
    s.pop_back(); //Remove extra space at end
    
    return s;
}
