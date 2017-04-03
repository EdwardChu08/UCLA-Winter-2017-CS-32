#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

#include <cmath>
#include <iostream>

///////////////////////////////////////////////
//HELPER FUNCTIONS
///////////////////////////////////////////////


GraphObject::Direction getRandomDirection(){
    return static_cast<GraphObject::Direction>(randInt(1, 4));
}

int randInt(double min, double max){
    return randInt(ceil(min), floor(max));
}


///////////////////////////////////////////////
//FUNCTION IMPLEMENTATION
///////////////////////////////////////////////

//Actor
bool Actor::alreadyMovedThisTurn(){
    return m_actionsTaken >= getStudentWorld()->getCurrentTicks();
}


//ENERGY HOLDER

bool EnergyHolder::addHitpoints(int nHP){
    m_hitpoints += nHP;
    if(m_hitpoints < 0) m_hitpoints = 0;
    
    if(m_hitpoints <= 0) return false;
    else return true;
}


//INSECT

void Insect::setDead(){
    Actor::setDead();
    
    getStudentWorld()->addFoodHere(getX(), getY(), 100);
}

//Lose 1 hp, check if hp <= 0, check if stunned
//Return true if continuing with rest of actions, return false when hp <= 0 or currently stunned
bool Insect::takeStartOfTurnActions(){
    EnergyHolder::addHitpoints(-1);
    
    if(EnergyHolder::getHitpoints() <= 0){
        Insect::setDead();
        return false;
    }
    
    if(Insect::m_stunnedTurns > 0){
        m_stunnedTurns--;
        return false;
    }
    
    return true;
}

//DamageType: 0 = Bite damage, 1 = stun damage, 2 = Poison damage
void Insect::getHurt(int damageType, int damage){
    switch(damageType){
        case 0:
            addHitpoints(-damage);
            break;
        
        case 1:
            if(isInvulnerable()) return;
            addStun(damage);
            setInvulnerable(); //So that it doesn't take damage more than once from damaging object
            break;
            
        case 2:
            if(isInvulnerable()) return;
            addHitpoints(-damage);
            setInvulnerable(); //So that it doesn't take damage more than once from damaging object
            break;
            
        default:;
    }
}

//Return true if moved, else return false
bool Insect::moveInDirection(GraphObject::Direction dir){
    
    setVulnerable();
    
    if(dir == up){
        if(!getStudentWorld()->ableToMoveHere(getX(), getY()-1)){
            return false;
        }
        GraphObject::moveTo(GraphObject::getX(), GraphObject::getY()-1);
        return true;
    }
    else if(dir == right){
        if(!getStudentWorld()->ableToMoveHere(getX()+1, getY())){
            return false;
        }
        GraphObject::moveTo(GraphObject::getX()+1, GraphObject::getY());
        return true;
    }
    
    else if(dir == down){
        if(!getStudentWorld()->ableToMoveHere(getX(), getY()+1)){
            return false;
        }
        GraphObject::moveTo(GraphObject::getX(), GraphObject::getY()+1);
        return true;
    }
    
    else if(dir == left){
        if(!getStudentWorld()->ableToMoveHere(getX()-1, getY())){
            return false;
        }
        GraphObject::moveTo(GraphObject::getX()-1, GraphObject::getY());
        return true;
    }
    
    return false; //Incorrect Direction!
}

//GRASSHOPPER

bool Grasshopper::moveInDirection(GraphObject::Direction dir){
    
    m_desiredDistance--;
    
    return Insect::moveInDirection(dir);
}


int Grasshopper::eatFoodOnCurrentSquare(){
    
    //Eat 200 units of food if food on square >= 200
    //Else eat the remaining amount of food
    int foodEaten = getStudentWorld()->eatFoodHere(getX(), getY(), 200);
    
    addHitpoints(foodEaten);
    
    //Return the amount of food eaten
    return foodEaten;
}

void Grasshopper::sleep(){
    Insect::addStun(2);
}

void Grasshopper::pickNewDirection(){
    GraphObject::setDirection(getRandomDirection());
    Grasshopper::m_desiredDistance = randInt(2, 10);
}


//BABYGRASSHOPPER

void BabyGrasshopper::doSomething(){
    //0
    if(alreadyMovedThisTurn())
        return;
    Actor::doSomething();
    
    
    //1,2,3,4
    if(!Insect::takeStartOfTurnActions()){
        return; //Died/Stunned
    };
    
    //5
    if(EnergyHolder::getHitpoints() >= 1500){
        Insect::setDead();
        
        getStudentWorld()->spawnAdultGrasshopper(getX(), getY());
        
        return;
    }
    
    
    //6,7
    if(Grasshopper::eatFoodOnCurrentSquare() > 0){
        if(randInt(0, 1) == 1){
            Grasshopper::sleep();
            return;
        }
    }
    
    //8
    if(Grasshopper::getDesiredDistance() <= 0){
        Grasshopper::pickNewDirection();
    }
    
    //9,10,11
    if(!Grasshopper::moveInDirection(getDirection())){
        Grasshopper::resetDesiredDistance();
    }
    
    //12
    Grasshopper::sleep();
    return;
}


//ADULT GRASSHOPPER
void AdultGrasshopper::doSomething() {
    //0
    if(alreadyMovedThisTurn())
        return;
    Actor::doSomething();
    
    
    //1,2,3,4
    if(!Insect::takeStartOfTurnActions()){
        return; //Died/Stunned
    }
    
    //5
    //1/3 chance to bite another insect, doing 50 points of damage
    if(randInt(1,3) == 1){
        getStudentWorld()->damageInsects(this, getX(), getY(), 0, 50, false);
        
        sleep();
        return;
    }
    
    
    //6
    //Jump to an open square without a boulder in circular radius of 10, 1/10 chance
    if(randInt(1,10) == 1){
        
        bool notYetFoundDestination = true;
        
        while(notYetFoundDestination){
            double angle = double(randInt(0, 6283))/1000; //6283 = 2*PI*1000
            
            int destX = getX() + randInt(0, (int)round(10 * cos(angle)));
            int destY = getY() + randInt(0, (int)round(10 * sin(angle)));
            
            if(!getStudentWorld()->ableToMoveHere(destX, destY))
                continue;
            
            GraphObject::moveTo(destX, destY);
            notYetFoundDestination = false;
        }
        
        sleep();
        return;
    }
    
    //7, 8
    //Eats Food on current square, 50% chance to sleep after eating
    if(Grasshopper::eatFoodOnCurrentSquare() > 0){
        if(randInt(0, 1) == 1){
            Grasshopper::sleep();
            return;
        }
    }
    
    //9
    //Finished walking in desired distance, pick new direction
    if(Grasshopper::getDesiredDistance() <= 0){
        Grasshopper::pickNewDirection();
    }
    
    //10,11,12
    //Try moving in direction, reset desiredDirection if cannot move in that direction
    if(!Grasshopper::moveInDirection(getDirection())){
        Grasshopper::resetDesiredDistance();
    }
    
    //13
    Grasshopper::sleep();
    return;
    
    
}

//DamageType: 0 = Bite damage, 1 = stun damage, 2 = Poison damage
void AdultGrasshopper::getHurt(int damageType, int damage){
    switch(damageType){
        case 0:
            addHitpoints(-damage);
            //50% chance to retaliate
            if(randInt(0,1) == 1){
                getStudentWorld()->damageInsects(this, getX(), getY(), 0, 50, false);
            }
            break;
        
        case 1:
        case 2:
            break; //Adult grasshoppers don't get poisoned/stunned by water
            
        default:;
    }
}


//FOOD
int Food::eat(int amount){
    if(getHitpoints() <= 0)
        return 0;
    
    int origAmount = getHitpoints();
    addHitpoints(-amount);
    
    if(getHitpoints() <= 0)
        setDead();
    
    return origAmount - getHitpoints();
}


//POISON
void Poison::damageInsects(){
    getStudentWorld()->damageInsects(this, getX(), getY(), 2, 150, true);
}

//WATER POOL
void WaterPool::damageInsects(){
    getStudentWorld()->damageInsects(this, getX(), getY(), 1, 2, true);
}
