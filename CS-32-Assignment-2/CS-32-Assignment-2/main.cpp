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
#include <string>

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
    
    //Testing erase
    assert(!a.erase(-1));
    assert(!a.erase(8));
    
    assert(a.erase(0));
    assert(a.erase(1));
    assert(a.erase(5));

    a.dump();
    
    //Testing remove
    assert(a.remove("Zebras") == 0);
    assert(a.remove("Aaa") == 1);
    
    a.dump();
    
    a.insert(0, "Remove");
    a.insert(2, "Remove");
    a.insert(6, "Remove");
    
    assert(a.remove("Remove") == 3);
    
    a.dump();
    
    //Testing get
    string test;
    assert(a.get(0, test) && test == "Apples");
    assert(a.get(1, test) && test == "Bananas");
    assert(a.get(2, test) && test == "Cheerios");
    assert(a.get(3, test) && test == "Doritos");
    assert(!a.get(-1, test) && test == "Doritos");
    assert(!a.get(4, test) && test == "Doritos");
    
    //Tesing set
    assert(a.set(0, "Anas"));
    assert(a.get(0, test) && test == "Anas");
    assert(a.set(1, "Banas"));
    assert(a.get(1, test) && test == "Banas");
    assert(!a.set(-1, test));
    assert(!a.set(4, test));
    
    
    //Testing find
    a.dump();
    assert(a.find("Anas") == 0);
    assert(a.find("Banas") == 1);
    assert(a.find("Doritos") == 3);
    a.insert(4, "Banas");
    assert(a.find("Banas") == 1);
    
    a.dump();
    
    //Testing Swap
    Sequence b;
    b.insert("Hi");
    a.swap(b);
    b.swap(b);
    
    a.dump();
    b.dump();
    
    //Testing Copy Constructor
    Sequence c(b);
    
    c.dump();
    
    //Testing Assignment Operator
    a = c;
    a = a;
    a.dump();
    
    b.set(0, "Banas");
    c.set(0, "Canas");
    assert(a.get(0, test) && test == "Anas");
    assert(b.get(0, test) && test == "Banas");
    assert(c.get(0, test) && test == "Canas");
    
    //Testing new Functions
    Sequence uno;
    Sequence dos;
    uno.insert("5");
    uno.insert("6");
    uno.insert("7");
    uno.insert("8");
    uno.insert("9");
    uno.dump();
    
    dos.insert("7");
    dos.insert("8");
    dos.insert("9");
    dos.dump();
    
    assert(subsequence(uno, dos) == 2);
    dos.set(2, "8");
    assert(subsequence(uno, dos) == -1);
    dos.set(0, "5");
    dos.set(1, "6");
    dos.set(2, "7");
    assert(subsequence(uno, dos) == 0);
    dos = uno;
    assert(subsequence(uno, dos) == 0);
    
    
    cerr << endl << "All tests succeeded" << endl;
    return 0;
}
