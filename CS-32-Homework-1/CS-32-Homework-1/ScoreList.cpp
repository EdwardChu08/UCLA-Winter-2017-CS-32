//
//  ScoreList.cpp
//  CS-32-Homework-1
//
//  Created by Edward Chu on 18/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include "ScoreList.h"

ScoreList::ScoreList(){
    
}

bool ScoreList::add(unsigned long score){
    
    //Out of bounds score
    if(score > 100)
        return false;
    
    return scores.insert(score);
}


bool ScoreList::remove(unsigned long score){
    
    //No score found
    if(scores.find(score) == -1)
        return false;
    
    
    return scores.erase(scores.find(score));
}

unsigned long ScoreList::minimum() const {
    
    if(scores.size() == 0)
        return NO_SCORE;
    
    unsigned long least;
    scores.get(0, least); //Cuz array is sorted
    
    return least;
}

unsigned long ScoreList::maximum() const{
    
    if(scores.size() == 0)
        return NO_SCORE;
    
    unsigned long max;
    scores.get(scores.size()-1, max); //Cuz array is sorted
    
    return max;
}

