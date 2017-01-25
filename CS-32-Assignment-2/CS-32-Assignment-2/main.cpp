//
//  main.cpp
//  CS-32-Assignment-2
//
//  Created by Edward Chu on 25/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//
#include "Sequence.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
    
    Sequence a;
    
    assert(a.empty());
    assert(a.size() == 0);
    
    a.insert(0, "Apples"); //Empty
    a.insert(0, "Aaa"); //Insert start
    a.insert(2, "Cheerios"); //Insert end
    a.insert(2, "Bananas"); //Insert middle
    a.insert(4, "Doritos"); //Insert end
    
    a.dump();
    
    assert(a.insert("Abc") == 1);
    assert(a.insert("Aa") == 0);
    assert(a.insert("Elephants") == 7);
    
    a.dump();
    
    return 0;
}
