//
//  testNewSequence.cpp
//  CS-32-Homework-1
//
//  Created by Edward Chu on 19/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//


#include <stdio.h>
#include <cassert>
#include <iostream>

#include "newSequence.h"

using namespace std;


int main(){
    Sequence a(1000);   // a can hold at most 1000 items
    Sequence b(5);      // b can hold at most 5 items
    Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
    ItemType v = 3;
    
    // No failures inserting 5 items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(k, v));
    
    // Failure if we try to insert a sixth item into b
    assert(!b.insert(5, v));
    
    cerr << "1" << endl;
    // When two Sequences' contents are swapped, their capacities are
    // swapped as well:
    a.swap(b);
    
    cerr << "2" << endl;
    
    
    assert(!a.insert(5, v));
    for (int k = 0; k < 1000; k++)
        assert(b.insert(k, v));
    
    Sequence d(a);
    a.dump();
    d.dump();
    

}


