#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

//Helper functions:
int randInt(double min, double max);
GraphObject::Direction getRandomDirection();

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
    
    //Idetifying object
    virtual bool blocksInsect() = 0;
    virtual bool isInsect() = 0;
    virtual bool isDamagingObject() = 0;
    
    bool isAlive() const {return m_isAlive;};
    StudentWorld* getStudentWorld(){return m_sw;};

protected:
    virtual void setDead(){
        m_isAlive = false;
    }
    
    int getActionsTaken(){return m_actionsTaken;};
    bool alreadyMovedThisTurn();
    
    
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
    virtual bool isDamagingObject(){return false;};
    
protected:
    bool addHitpoints(int nHP);
    
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
    int getStunnedTurns(){return m_stunnedTurns;};
    
    
    //So that it doesn't take damage more than once from damaging object
    bool isInvulnerable(){return m_tempInvul;};
    void setVulnerable(){m_tempInvul = false;};
    void setInvulnerable(){m_tempInvul = true;};
    
    bool moveInDirection(GraphObject::Direction dir);
    
    virtual bool blocksInsect(){return false;};
    virtual bool isInsect(){return true;};

    
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
    
protected:
    int eatFoodOnCurrentSquare();
    void sleep();
    int getDesiredDistance() const {return m_desiredDistance;};
    void resetDesiredDistance(){m_desiredDistance = 0;};
    void pickNewDirection();
    bool moveInDirection(GraphObject::Direction dir);

private:
    int m_desiredDistance;

};


//Class DamagingObject: Parent for WaterPool and Poison
class DamagingObject : public Actor {
    
public:
    DamagingObject(int imageID ,int startX, int startY, StudentWorld *sw) : Actor(imageID, startX, startY, sw, right, 2){};
    
    virtual void getHurt(int damageType, int damage){};
    virtual void damageInsects() = 0;
    
    virtual bool blocksInsect(){return false;};
    virtual bool isInsect(){return false;};
    virtual bool isDamagingObject(){return true;};
    
};



//Pebble class
class Pebble : public Actor {
    
public:
    Pebble(int startX, int startY, StudentWorld *sw) : Actor(IID_ROCK, startX, startY, sw, right, 1){};
    
    virtual void getHurt(int damageType, int damage){};
    
    virtual bool blocksInsect(){return true;};
    virtual bool isInsect(){return false;};
    virtual bool isDamagingObject(){return false;};
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
    
    virtual bool isInsect(){return false;};
    virtual bool blocksInsect(){return false;};
    
    int eat(int amount);
    
};


//Poison class
class Poison : public DamagingObject {
public:
    Poison(int startX, int startY, StudentWorld *sw) : DamagingObject(IID_POISON, startX, startY, sw){
    };
    
    
    virtual void doSomething(){};
    
    //Assumed that each ant only takes damage once when stepping onto poison
    virtual void damageInsects();
    
};


//Water pool class
class WaterPool : public DamagingObject {
public:
    WaterPool(int startX, int startY, StudentWorld *sw) : DamagingObject(IID_WATER_POOL, startX, startY, sw){};
    
    virtual void doSomething(){};
    
    virtual void damageInsects();
};

#endif // ACTOR_H_
