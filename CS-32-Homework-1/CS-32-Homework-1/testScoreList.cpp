//
//  testScoreList.cpp
//  CS-32-Homework-1
//
//  Created by Edward Chu on 18/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//


#include <stdio.h>
#include <cassert>
#include <iostream>

#include "ScoreList.h"

using namespace std;

int main(){
    
    ScoreList s;
    
    assert(s.add(0));
    assert(s.add(5));
    assert(s.add(100));
    assert(s.add(6));
    assert(s.add(35));
    assert(s.add(3));
    
    assert(!s.add(101));
    assert(!s.add(-5));
    
    assert(s.maximum() == 100);
    assert(s.minimum() == 0);
    
    assert(s.remove(100));
    assert(!s.remove(7));
    assert(s.remove(0));
    
    assert(s.maximum() == 35);
    assert(s.minimum() == 3);
    
    
    cout << "All tests succeed" << endl;
}
 
