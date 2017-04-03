#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

#include <cmath>
//#include <iostream>

///////////////////////////////////////////////
//HELPER FUNCTIONS
///////////////////////////////////////////////


GraphObject::Direction getRandomDirection(){
    return static_cast<GraphObject::Direction>(randInt(1, 4));
}

//Returns randInt with min rounded up and max rounded down
int randInt(double min, double max){
    return randInt((int)ceil(min), (int)floor(max));
}

//Sets x and y to coordinates that are one spot ahead according to direction
void oneSpotAhead(int &x, int &y, GraphObject::Direction dir){
    switch (dir) {
        case GraphObject::up:
            y++;
            break;
            
        case GraphObject::right:
            x++;
            break;
            
        case GraphObject::down:
            y--;
            break;
            
        case GraphObject::left:
            x--;
            break;
            
        default:
            break;
    }
}


///////////////////////////////////////////////
//FUNCTION IMPLEMENTATION
///////////////////////////////////////////////

///////////////////////////////////////////////
//ACTOR
///////////////////////////////////////////////

//Check if actor has already moved this turn, returns true if have moved
bool Actor::alreadyMovedThisTurn(){
    return m_actionsTaken >= getStudentWorld()->getCurrentTicks();
}

//Rotate 90 degrees. Returns the current direction
GraphObject::Direction Actor::rotate90(bool clockwise){
    if(clockwise){
        //Clockwise turn
        if(getDirection() == left){
            setDirection(up);
        } else {
            setDirection(static_cast<GraphObject::Direction>(getDirection()+1));
        }
    } else {
        //Anti-Clockwise turn
        if(getDirection() == up){
            setDirection(left);
        } else {
            setDirection(static_cast<GraphObject::Direction>(getDirection()-1));
        }
    }
    
    return getDirection();
}


///////////////////////////////////////////////
//ENERGY HOLDER
///////////////////////////////////////////////

//Adds nHP to hitpoints, set hitpoints to 0 if hitpoints < 0
bool EnergyHolder::addHitpoints(int nHP){
    m_hitpoints += nHP;
    if(m_hitpoints < 0){ m_hitpoints = 0;}
    
    if(m_hitpoints <= 0) return false;
    else return true;
}


///////////////////////////////////////////////
//INSECT
///////////////////////////////////////////////

//Set dead and add 100 food on this spot
void Insect::setDead(){
    EnergyHolder::setDead();
    
    getStudentWorld()->addFoodHere(getX(), getY(), 100);
}

//Lose 1 hp, check if hp <= 0, check if stunned
//Return true if continuing with rest of actions, return false when hp <= 0 or currently stunned
bool Insect::takeStartOfTurnActions(){
    if(alreadyMovedThisTurn())
        return false;
    Actor::doSomething();
    
    EnergyHolder::addHitpoints(-1);
    
    if(EnergyHolder::getHitpoints() <= 0 || !Actor::isAlive()){
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
//Called by studentWorld when this insect is damaged
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

//Return true if able to move, else return false
bool Insect::moveInDirection(GraphObject::Direction dir){
    
    setVulnerable();
    
    if(dir == up){
        if(!getStudentWorld()->ableToMoveHere(getX(), getY()+1)){
            return false;
        }
        GraphObject::moveTo(GraphObject::getX(), GraphObject::getY()+1);
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
        if(!getStudentWorld()->ableToMoveHere(getX(), getY()-1)){
            return false;
        }
        GraphObject::moveTo(GraphObject::getX(), GraphObject::getY()-1);
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

///////////////////////////////////////////////
//GRASSHOPPER
///////////////////////////////////////////////

//Move in direction + reduce m_desiredDistance by 1
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


//Try to eat, move in desired direction or reset desired direction
void Grasshopper::eatAndMove(){
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


///////////////////////////////////////////////
//BABY GRASSHOPPER
///////////////////////////////////////////////

void BabyGrasshopper::doSomething(){
    
    //1,2,3,4
    if(!Insect::takeStartOfTurnActions()){
        return; //Died/Stunned/Already moved
    };
    
    //5
    //Morph into an adult grasshopper!
    if(EnergyHolder::getHitpoints() >= 1500){
        Insect::setDead();
        
        getStudentWorld()->spawnAdultGrasshopper(getX(), getY());
        
        return;
    }    
    
    //6,7,8,9,10,11,12
    Grasshopper::eatAndMove();
    return;
}


///////////////////////////////////////////////
//ADULT GRASSHOPPER
///////////////////////////////////////////////

void AdultGrasshopper::doSomething() {
    
    //1,2,3,4
    if(!Insect::takeStartOfTurnActions()){
        return; //Died/Stunned/Already moved
    }
    
    //5
    //1/3 chance to bite another insect, doing 50 points of damage
    if(randInt(1,3) == 1){
        if(getStudentWorld()->damageInsects(this, getX(), getY(), 0, 50, false) > 0){
            //Successfully attacked
            sleep();
            return;
        }
    }
    
    
    //6
    //Jump to an open square without a boulder in circular radius of 10, 1/10 chance
    if(randInt(1,10) == 1){
        
        bool notYetFoundDestination = true;
        
        //Loop until a valid jump destination is found
        while(notYetFoundDestination){
            double angle = double(randInt(0, 6283))/1000; //6283 = 2*PI*1000 = FULL_CIRCLE*1000
            
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
    
    //7,8,9,10,11,12,13
    Grasshopper::eatAndMove();
    return;
    
}

//DamageType: 0 = Bite damage, 1 = stun damage, 2 = Poison damage
//Override Insect::getHurt, since adult grasshoppers get hurt differently
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


///////////////////////////////////////////////
//FOOD
///////////////////////////////////////////////

//Food get eaten
int Food::eat(int amount){
    
    int origAmount = getHitpoints();
    addHitpoints(-amount);
    
    if(getHitpoints() <= 0)
        setDead();
   
    return origAmount - getHitpoints();
}


///////////////////////////////////////////////
//POISON
///////////////////////////////////////////////

void Poison::damageInsects(){
    getStudentWorld()->damageInsects(this, getX(), getY(), 2, 150, true);
}


///////////////////////////////////////////////
//WATER POOL
///////////////////////////////////////////////

void WaterPool::damageInsects(){
    getStudentWorld()->damageInsects(this, getX(), getY(), 1, 2, true);
}


///////////////////////////////////////////////
//PHEROMONE
///////////////////////////////////////////////

void Pheromone::doSomething(){
    addHitpoints(-1);
    
    if(getHitpoints() == 0){
        setDead();
        return;
    }
}

//Return strength added
int Pheromone::addStrength(int amount){
    int origStrength = getHitpoints();
    addHitpoints(amount);
    
    if(getHitpoints() > MAX_PHEROMONE_STRENGTH){
        setHitpoints(MAX_PHEROMONE_STRENGTH);
    }
    return getHitpoints()-origStrength;
}


///////////////////////////////////////////////
//ANTHILL
///////////////////////////////////////////////

void Anthill::doSomething(){
    
    //1
    addHitpoints(-1);
    
    //2
    if(getHitpoints() == 0){
        setDead();
        return;
    }
    
    //3
    //Eat up to 10000 food on it
    int foodEaten = getStudentWorld()->eatFoodHere(getX(), getY(), 10000);
    if(foodEaten > 0){
        addHitpoints(foodEaten);
        return;
    }
        
    //4
    //Spawn ant if HP > 2000
    if(getHitpoints() >= 2000){
        getStudentWorld()->spawnAnt(this);
        
        addHitpoints(-1500);
    }
    
}


///////////////////////////////////////////////
//ANT
///////////////////////////////////////////////

void Ant::doSomething(){
    
    //1,2,3
    if(!Insect::takeStartOfTurnActions()){
        return; //Died/Stunned/Already moved
    }
    
    
    Compiler::Command cmd;
    
    //4
    //Max 10 commands executed
    for(int i = 0; i < 10; i++){
        //4a, 4b
        if(!m_cp->getCommand(m_instructionCount, cmd)){
            setDead();
            return;
        }
        
        switch (cmd.opcode) {
                
            //4ci
            case Compiler::moveForward:
                if(moveInDirection(getDirection())){
                    resetIsBittenAndIsBlockedHere();
                } else {
                    m_isBlockedHere = true;
                }
                updateInstructionCount();
                return;
                break;
                
            //4cii
            case Compiler::eatFood:
                EnergyHolder::addHitpoints(eatFoodBeingHeld());
                updateInstructionCount();
                return;
                break;
                
            //4ciii
            case Compiler::dropFood:
                getStudentWorld()->addFoodHere(getX(), getY(), m_amountFood);
                m_amountFood = 0;
                updateInstructionCount();
                return;
                break;
                
            //4civ
            case Compiler::bite:
                getStudentWorld()->damageInsects(this, getX(), getY(), ANT_DAMAGE_TYPE, ANT_DAMAGE, false);
                updateInstructionCount();
                return;
                break;
                
            //4cv
            case Compiler::pickupFood:
                m_amountFood += getStudentWorld()->eatFoodHere(getX(), getY(), pickUpAmount());
                updateInstructionCount();
                return;
                break;
                
            //4cvi
            case Compiler::emitPheromone:
                getStudentWorld()->emitPheromone(this);
                updateInstructionCount();
                return;
                break;
                
            //4cvii
            case Compiler::faceRandomDirection:
                setDirection(getRandomDirection());
                updateInstructionCount();
                return;
                break;
                
            //4cviii
            case Compiler::rotateClockwise:
                rotate90(true);
                updateInstructionCount();
                return;
                break;
                
            //4cix
            case Compiler::rotateCounterClockwise:
                rotate90(false);
                updateInstructionCount();
                return;
                break;
                
            //4cx
            case Compiler::generateRandomNumber:
                if(stoi(cmd.operand1) == 0){
                    m_lastRandNum = 0;
                } else {
                    m_lastRandNum = randInt(0, stoi(cmd.operand1)-1);
                }
                updateInstructionCount();
                break;
                
            //4cxi
            case Compiler::goto_command:
                m_instructionCount = stoi(cmd.operand1);
                break;
                
            //4cxii
            case Compiler::if_command:
                switch (stoi(cmd.operand1)) {
                    
                    //4cxii1
                    case Compiler::last_random_number_was_zero:
                        if(m_lastRandNum == 0){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                    
                    //4cxii2
                    case Compiler::i_am_carrying_food:
                        if(m_amountFood > 0){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                        
                    //4cxii3
                    case Compiler::i_am_hungry:
                        if(getHitpoints() <= 25){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                        
                    //4cxii4
                    case Compiler::i_am_standing_with_an_enemy:
                        if(getStudentWorld()->isStandingWithEnemy(this)){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                        
                    //4cxii5
                    case Compiler::i_am_standing_on_food:
                        if(getStudentWorld()->hasFoodHere(getX(), getY())){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                        
                    //4cxii6
                    case Compiler::i_am_standing_on_my_anthill:
                        if(getStudentWorld()->isOnOwnAnthill(this)){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                        
                    //4cxii7
                    case Compiler::i_smell_pheromone_in_front_of_me:
                    {
                        int frontX = getX();
                        int frontY = getY();
                        oneSpotAhead(frontX, frontY, getDirection());
                        if(getStudentWorld()->hasPheromoneHere(this, frontX, frontY)){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                    }
                        updateInstructionCount();
                        break;
                        
                    //4cxii8
                    case Compiler::i_smell_danger_in_front_of_me:
                    {
                        int frontX = getX();
                        int frontY = getY();
                        oneSpotAhead(frontX, frontY, getDirection());
                        if(getStudentWorld()->hasEnemyHere(this, frontX, frontY, true)){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                    }
                        updateInstructionCount();
                        break;
                        
                    //4cxii9
                    case Compiler::i_was_bit:
                        if(m_isBittenHere){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                        
                    //4cxii10
                    case Compiler::i_was_blocked_from_moving:
                        if(m_isBlockedHere){
                            m_instructionCount = stoi(cmd.operand2);
                            break;
                        }
                        updateInstructionCount();
                        break;
                        
                    default:
                        updateInstructionCount();
                        break;
                }
                break;
                
                
            default:
                break;
        }
    }
    
}

//Returns amount of food eaten
int Ant::eatFoodBeingHeld(){
    //Normal case
    if(m_amountFood > ANT_EAT_AMOUNT){
        m_amountFood -= ANT_EAT_AMOUNT;
        return ANT_EAT_AMOUNT;
    }
    
    //Less than eat amount, eat all remaining food
    int temp = m_amountFood;
    m_amountFood = 0;
    return temp;
}

//Returns max amount of food this ant can pickup at one time
int Ant::pickUpAmount(){
    //Normal case
    if(m_amountFood <= ANT_MAX_AMOUNT_FOOD - ANT_PICKUP_AMOUNT){
        return ANT_PICKUP_AMOUNT;
    }
    
    //Almost full, pickup til full
    return ANT_MAX_AMOUNT_FOOD - m_amountFood;
}
