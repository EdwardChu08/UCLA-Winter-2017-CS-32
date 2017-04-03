#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

#include <list>

class Actor;
class Ant;
class Anthill;
class Compiler;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
    ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    int getCurrentTicks(){return m_ticksElapsed;};
    
    bool ableToMoveHere(int x, int y) const;
    
    int eatFoodHere(int x, int y, int amount);
    int addFoodHere(int x, int y, int amount);
    bool hasFoodHere(int x, int y) const;
    
    bool spawnAdultGrasshopper(int x, int y);
    
    int getNumAnts(int colonyNum) const {return m_numAnts[colonyNum];};
    bool spawnAnt(Anthill *self);
    
    //DamageType: 0 = Bite damage, 1 = stun damage, 2 = Poison damage
    int damageInsects(Actor* attacker, int x, int y, int damageType, int damage, bool damageAll);
    
    //For Ants
    bool isStandingWithEnemy(Ant *self) const;
    bool hasEnemyHere(Ant *self, int x, int y, bool checkPoison) const;
    bool isOnOwnAnthill(Ant *self) const;
    bool hasPheromoneHere(Ant *self, int x, int y) const;
    int emitPheromone(Ant *self);

private:
    int m_ticksElapsed;
    int updateTickCount(){m_ticksElapsed++; return m_ticksElapsed;};
    
    const int NUMBER_OF_TICKS_TO_SIMULATE = 2000;
    
    void setDisplayText();
    std::string formatDisplayText(int ticksLeft, int antAnt[], int winningAntNum, Compiler *cps[]);
    
    std::list<Actor*> m_field[VIEW_HEIGHT][VIEW_WIDTH];
    void moveActor(std::list<Actor*>::iterator it, int origX, int origY);
    
    int m_numAnts[MAX_ANT_COLONIES];
    Compiler *m_compilerForEntrant[MAX_ANT_COLONIES];
    int getNumAntsForEntrant(int entrantNum) const {return m_numAnts[entrantNum];};
    int m_maxNumAnt;
    int m_winningEntrantNum;
    int updateWinner(Compiler *cps[]);
};

#endif // STUDENTWORLD_H_
