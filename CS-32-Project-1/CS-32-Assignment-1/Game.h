//
//  game.hpp
//  CS-32-Assignment-1
//
//  Created by Edward Chu on 11/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#ifndef Game_h
#define Game_h

//#include <stdio.h>
#include <string>

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    std::string takePlayerTurn();
    bool recommendMove(const Arena& a, int r, int c, int& bestDir);
    int computeDanger(const Arena& a, int r, int c);
    int numRatSurrounding(const Arena* a, int r, int c);
};

#endif /* game_h */
