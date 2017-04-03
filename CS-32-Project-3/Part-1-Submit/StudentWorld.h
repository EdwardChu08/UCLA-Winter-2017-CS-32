#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

#include <list>

class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
    ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    int getCurrentTicks(){return m_ticksElapsed;};
    
    bool ableToMoveHere(int x, int y);
    int eatFoodHere(int x, int y, int amount);
    int addFoodHere(int x, int y, int amount);
    bool spawnAdultGrasshopper(int x, int y);
    
    //DamageType: 0 = Bite damage, 1 = stun damage, 2 = Poison damage
    void damageInsects(Actor* attacker, int x, int y, int damageType, int damage, bool damageAll);

private:
    
    int updateTickCount(){m_ticksElapsed++; return m_ticksElapsed;};
    void setDisplayText();
    void moveActor(std::list<Actor*>::iterator it, int origX, int origY);
    
    std::list<Actor*> m_field[64][64];
    int m_ticksElapsed; 
};

#endif // STUDENTWORLD_H_
