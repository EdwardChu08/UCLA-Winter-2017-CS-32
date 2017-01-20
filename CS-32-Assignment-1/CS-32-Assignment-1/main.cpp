//
//  main.cpp
//  CS-32-Assignment-1
//
//  Created by Edward Chu on 11/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

// main.cpp


//Actual Program

#include "globals.h"
#include "Rat.h"
#include "Arena.h"
#include "Player.h"
#include "Game.h"
#include "History.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 20);
    
    // Play the game
    g.play();
}











