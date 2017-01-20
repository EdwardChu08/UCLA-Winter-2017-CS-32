//
//  main.cpp
//  CS-32-Homework-1
//
//  Created by Edward Chu on 18/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//


#include <iostream>
#include <cassert>

#include "Sequence.h"

using namespace std;


int main(int argc, const char * argv[]) {
    
 
    Sequence a;
    
    a.insert(0, 0);
    a.insert(1, 1);
    a.insert(2, 2);
    a.insert(3, 3);
    
    ItemType test;
    
    assert(a.get(2, test));
    assert(!a.get(4, test));
    assert(test == 2);
    
    assert(a.erase(2));
    //a.dump();
    assert(a.get(2, test));
    assert(test == 3);
    
    Sequence b;
    
    b.insert(5);
    b.insert(6);
    b.insert(7);
    b.insert(8);
    //b.dump();
    
    a.swap(b);
    
    a.dump();
    b.dump();
    
    assert(a.get(1, test));
    assert(!b.get(3, test));
    assert(test == 6);
    
    //b.insert(1.5);
    //assert(b.remove(1.5) == 2);
    
    //b.dump();
    
    b.insert(2, 1);
    
    unsigned long temp;
    
    assert(b.get(2, temp));
    assert(temp == 1);
    
    
    
    assert(b.find(1) == 1);
    assert(b.find(10) == -1);
    
    assert(b.erase(1));
    assert(b.get(1, test) && b.get(2, temp));
    assert(test == 1 && temp == 3);
    
    b.set(2, 1);
    assert(b.get(2, test));
    assert(test == 1);
    
    b.insert(4);
    assert(b.find(4) == 3);
    
    b.insert(0);
    assert(b.get(0, test) && b.get(1, temp));
    assert(test == 0 && temp == 0);
    
    
    for(int i = 3; i < 6; i++)
        b.erase(3);
    
    
    a.swap(b);
    
    assert(!a.get(4, temp));
    assert(a.get(2, temp));
    assert(temp == 1);
    
    //Bad
    assert(!a.insert(10, 2));
    assert(!a.insert(-1, 2));
 
    
    
    return 0;
}


//OMG F TARGET MEMBERSHIP




/*
int main()
{
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    s.insert(42);
    assert(s.size() == 1  &&  s.find(42) == 0);
    cout << "Passed all tests" << endl;
}
*/

