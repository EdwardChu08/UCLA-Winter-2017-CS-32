//
//  rats.h
//  CS-32-Assignment-1
//
//  Created by Edward Chu on 11/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#ifndef Rat_h
#define Rat_h

#include <stdio.h>

//#include "Arena.h"

class Arena; // type name, since it's mentioned in the Rat declaration.

class Rat
{
public:
    // Constructor
    Rat(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* rat_h */
