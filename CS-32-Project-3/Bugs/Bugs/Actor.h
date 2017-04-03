#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

#include "Compiler.h"

//Helper functions:
int randInt(double min, double max);
GraphObject::Direction getRandomDirection();
void oneSpotAhead(int &x, int &y, GraphObject::Direction dir);

//Actor class: Parent for all in-game objects
class Actor : public GraphObject {
    
public:
    Actor(int imageID, int startX, int startY, StudentWorld *sw, Direction dir = right, int depth = 0) : GraphObject(imageID, startX, startY, dir, depth), m_sw(sw), m_actionsTaken(0), m_isAlive(true){
        
    };
    
    virtual ~Actor(){};
    
    //Called every tick
    virtual void doSomething(){
        m_actionsTaken++;
    };
    
    //Called when actor is bitten, poisoned or stunned
    virtual void getHurt(int damageType, int damage) = 0;
    
    //Idetifiers for identifying objects
    virtual bool blocksInsect() const {return false;};
    virtual bool isInsect() const {return false;};
    virtual bool isDamagingObject() const {return false;};
    virtual bool isPoison() const {return false;};
    virtual bool isAnt() const  {return false;};
    virtual bool isAnthill() const {return false;};
    virtual bool isGrasshopper() const {return false;};
    virtual bool isPheromone() const {return false;};
    
    bool isAlive() const {return m_isAlive;};
    StudentWorld* getStudentWorld() const {return m_sw;};

protected:
    virtual void setDead(){
        m_isAlive = false;
    }
    
    int getActionsTaken(){return m_actionsTaken;};
    bool alreadyMovedThisTurn();
    
    GraphObject::Direction rotate90(bool clockwise);
    
private:
    bool m_isAlive;
    int m_actionsTaken;
    StudentWorld *m_sw;
};


//EnergyHolder class: Parent for all energy-holding classes
class EnergyHolder : public Actor {
    
public:
    EnergyHolder(int imageID, int startX, int startY, int initialHP, StudentWorld *sw, Direction dir = right, int depth = 0) : Actor(imageID, startX, startY, sw, dir, depth), m_hitpoints(initialHP){};
    
    int getHitpoints() const {return m_hitpoints;};
    
    virtual void setDead(){Actor::setDead(); m_hitpoints = 0;}
    
protected:
    bool addHitpoints(int nHP);
    void setHitpoints(int nHP){m_hitpoints = nHP;};
    
private:
    int m_hitpoints;
};


//Insect class: Parent for baby grasshopper, adult grasshopper & ant
class Insect : public EnergyHolder {

public:
    Insect(int imageID, int startX, int startY, int initialHP, StudentWorld *sw, Direction dir = right, int depth = 0) : EnergyHolder(imageID, startX, startY, initialHP, sw, dir, depth), m_stunnedTurns(0), m_tempInvul(false){

    };
    
    //DamageType: 0 = Bite damage, 1 = stun damage, 2 = Poison damage
    virtual void getHurt(int damageType, int damage);
    
    
protected:
    //Lose 1 hp, check if hp <= 0, check if stunned
    //Return true if continuing with rest of actions, return false when hp <= 0 or currently stunned
    bool takeStartOfTurnActions();
    
    virtual void setDead();
    void setStun(int nTurns){m_stunnedTurns = nTurns;};
    int addStun (int nTurns){m_stunnedTurns += nTurns; return m_stunnedTurns;};
    int getStunnedTurns() const {return m_stunnedTurns;};
    
    
    //So that it doesn't take damage more than once from damagtming object
    bool isInvulnerable() const {return m_tempInvul;};
    void setVulnerable(){m_tempInvul = false;};
    void setInvulnerable(){m_tempInvul = true;};
    
    bool moveInDirection(GraphObject::Direction dir);
    
    virtual bool isInsect() const {return true;};

    
private:
    int m_stunnedTurns;
    bool m_tempInvul;
};


//Class Grasshopper: Parent for baby grasshopper & adult grasshopper
class Grasshopper : public Insect {
    
public:
    Grasshopper(int imageID, int startX, int startY, int initialHP, StudentWorld *sw) : Insect(imageID, startX, startY, initialHP, sw, getRandomDirection(), 0){
        
        m_desiredDistance = randInt(2, 10);
    };
    
    //Identifiers
    virtual bool isGrasshopper() const {return true;};
    
protected:
    int eatFoodOnCurrentSquare();
    void sleep();
    int getDesiredDistance() const {return m_desiredDistance;};
    void resetDesiredDistance(){m_desiredDistance = 0;};
    void pickNewDirection();
    bool moveInDirection(GraphObject::Direction dir);
    void eatAndMove();

private:
    int m_desiredDistance;

};


//Class DamagingObject: Parent for WaterPool and Poison
class DamagingObject : public Actor {
    
public:
    DamagingObject(int imageID ,int startX, int startY, StudentWorld *sw) : Actor(imageID, startX, startY, sw, right, 2){};
    
    virtual void getHurt(int damageType, int damage){};
    virtual void damageInsects() = 0;
    
    //Identifiers
    virtual bool isDamagingObject() const {return true;};
    
};



//Pebble class
class Pebble : public Actor {
    
public:
    Pebble(int startX, int startY, StudentWorld *sw) : Actor(IID_ROCK, startX, startY, sw, right, 1){};
    
    virtual void getHurt(int damageType, int damage){};
    
    //Identifiers
    virtual bool blocksInsect() const {return true;};
};



//Baby Grasshopper class
class BabyGrasshopper : public Grasshopper {
    
public:
    BabyGrasshopper(int startX, int startY, StudentWorld *sw) : Grasshopper(IID_BABY_GRASSHOPPER, startX, startY, 500, sw){
        
    };
    
    virtual void doSomething();

private:
};



//Adult Grasshopper class
class AdultGrasshopper : public Grasshopper {
public:
    AdultGrasshopper(int startX, int startY, StudentWorld *sw) : Grasshopper(IID_ADULT_GRASSHOPPER, startX, startY, 1600, sw){};

    virtual void doSomething();
    
    //DamageType: 0 = Bite damage, 1 = stun damage, 2 = Poison damage
    virtual void getHurt(int damageType, int damage);
    
private:
};



//Food class
class Food : public EnergyHolder {
    
public:
    Food(int startX, int startY, int initialAmount, StudentWorld *sw) : EnergyHolder(IID_FOOD, startX, startY, initialAmount, sw, right, 2){};
    
    
    virtual void getHurt(int damageType, int damage){};
    
    int eat(int amount);
    int add(int amount){addHitpoints(amount); return getHitpoints();};
    
};


//Poison class
class Poison : public DamagingObject {
public:
    Poison(int startX, int startY, StudentWorld *sw) : DamagingObject(IID_POISON, startX, startY, sw){
    };
    
    virtual bool isPoison() const {return true;};
    
    //Assumed that each ant only takes damage once when stepping onto poison
    virtual void damageInsects();
    
};


//Water pool class
class WaterPool : public DamagingObject {
public:
    WaterPool(int startX, int startY, StudentWorld *sw) : DamagingObject(IID_WATER_POOL, startX, startY, sw){};
    
    virtual void damageInsects();
};


//Pheromone class
class Pheromone : public EnergyHolder {
public:
    Pheromone(int startX, int startY, int colonyNum, StudentWorld *sw) : EnergyHolder(IID_PHEROMONE_TYPE0+colonyNum, startX, startY, 256, sw, right, 2), m_colonyNum(colonyNum){};
    
    virtual void doSomething();
    virtual void getHurt(int damageType, int damage){};
    
    //Return strength added
    int addStrength(int amount);
    
    //Identifiers
    virtual bool isPheromone() const {return true;};
    int getColonyNum() const {return m_colonyNum;};
    
    
private:
    const int MAX_PHEROMONE_STRENGTH = 768;
    int m_colonyNum;
};


//Anthill class
class Anthill : public EnergyHolder {
public:
    Anthill(int startX, int startY, int colonyNum, Compiler *cp, StudentWorld *sw) : EnergyHolder(IID_ANT_HILL, startX, startY, 8999, sw, right, 2), m_colonyNum(colonyNum), m_cp(cp){};
    
    virtual void doSomething();
    virtual void getHurt(int damageType, int damage){};
    
    Compiler* getCompiler() const {return m_cp;};
    
    //Identifiers
    virtual bool isAnthill() const {return true;};    
    int getColonyNum() const {return m_colonyNum;};
    
private:
    int m_colonyNum;
    Compiler *m_cp;
};


//Ant class
class Ant : public Insect {
public:
    Ant(int startX, int startY, int colonyNum, Compiler *cp, StudentWorld *sw) : Insect(IID_ANT_TYPE0+colonyNum, startX, startY, 1500, sw, getRandomDirection(), 1), m_colonyNum(colonyNum), m_lastRandNum(0), m_amountFood(0), m_isBlockedHere(false), m_isBittenHere(false), m_cp(cp), m_instructionCount(0){};
    
    
    virtual void doSomething();
    
    //Identifiers
    virtual bool isAnt() const {return true;};
    
    int getColonyNum() const {return m_colonyNum;};
    
private:
    int m_colonyNum;
    int m_lastRandNum;
    
    int m_amountFood;
    const int ANT_MAX_AMOUNT_FOOD = 1800;
    const int ANT_PICKUP_AMOUNT = 400;
    const int ANT_EAT_AMOUNT = 100;
    int eatFoodBeingHeld(); //Returns amount of food eaten
    int pickUpAmount(); //Returns amount of food this ant can pick up
    
    const int ANT_DAMAGE_TYPE = 0;
    const int ANT_DAMAGE = 15;
    
    bool m_isBittenHere;
    bool isBittenHere(){return m_isBittenHere;};
    bool m_isBlockedHere;
    bool isBlockedHere(){return m_isBlockedHere;};
    void resetIsBittenAndIsBlockedHere(){m_isBittenHere = m_isBlockedHere = false;};
    
    Compiler *m_cp;
    int m_instructionCount;
    int updateInstructionCount(){m_instructionCount++; return m_instructionCount;};
    
};

#endif // ACTOR_H_
