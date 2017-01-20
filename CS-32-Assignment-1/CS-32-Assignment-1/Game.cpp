//
//  game.cpp
//  CS-32-Assignment-1
//
//  Created by Edward Chu on 11/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//


#include <string>
#include <iostream>

#include "globals.h"
#include "Game.h"
#include "Arena.h"
#include "Player.h"
#include "History.h"

using namespace std;

//int computeDanger(const Arena& a, int r, int c); //Because recommendMove uses computeDanger
//bool recommendMove(const Arena& a, int r, int c, int& bestDir); //Because takePlayerTurn uses recommendMove

Game::Game(int rows, int cols, int nRats)
{
    if (nRats < 0)
    {
        cout << "***** Cannot create Game with negative number of rats!" << endl;
        exit(1);
    }
    if (nRats > MAXRATS)
    {
        cout << "***** Trying to create Game with " << nRats
        << " rats; only " << MAXRATS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRats > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with rats
    while (nRats > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRat(r, c);
        nRats--;
    }
}

Game::~Game()
{
    delete m_arena;
}



string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x/h or nothing): ";
        string playerMove;
        getline(cin, playerMove);
        
        Player* player = m_arena->player();
        int dir;
        
        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonPellet();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonPellet();
            else if (tolower(playerMove[0]) == 'h'){
                return "Press enter to continue.";
            }
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x/h." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->ratCount() > 0)
    {
        string msg = takePlayerTurn();
        if(msg == "Press enter to continue."){
            m_arena->history().display();
            cout << msg;
            cin.ignore(10000,'\n');
            m_arena->display("");
            continue;
        }
        Player* player = m_arena->player();
        if (player->isDead())
        {
            cout << msg << endl;
            break;
        }
        m_arena->moveRats();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}


// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poison pellet and not
// move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.

//Now with *slightly* more intelligence!
bool Game::recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    bool stay = true;
    int leastRatSurrounding = numRatSurrounding(&a, r, c);
    
    //Go to square with least rats surrounding
    //Compare North
    if(leastRatSurrounding > numRatSurrounding(&a, r-1, c) && r != 1){
        bestDir = NORTH;
        leastRatSurrounding = numRatSurrounding(&a, r-1, c);
        stay = false;
    }
    
    //Compare East
    if(leastRatSurrounding > numRatSurrounding(&a, r, c+1) && c != a.cols()){
        bestDir = EAST;
        leastRatSurrounding = numRatSurrounding(&a, r, c+1);
        stay = false;
    }
    
    //Compare South
    if(leastRatSurrounding > numRatSurrounding(&a, r+1, c) && r != a.rows()){
        bestDir = SOUTH;
        leastRatSurrounding = numRatSurrounding(&a, r+1, c);
        stay = false;
    }
    
    //Compare West
    if(leastRatSurrounding > numRatSurrounding(&a, r, c-1) && c != 1){
        bestDir = WEST;
        leastRatSurrounding = numRatSurrounding(&a, r, c-1);
        stay = false;
    }
    
    //If no surrounding square with less rats surrounding, try to go and lay poison on square with equal
    //amount of rats surrounding
    if(a.getCellStatus(r, c) == HAS_POISON && stay){
        int numRatsHere = numRatSurrounding(&a, r, c);
        //Compare North
        if(r!=1){
            if(numRatsHere == numRatSurrounding(&a, r-1, c) && a.getCellStatus(r-1, c) != HAS_POISON){
                bestDir = NORTH;
                stay = false;
            }
        }
        //Compare East
        if(c != a.cols()){
            if(numRatsHere == numRatSurrounding(&a, r, c+1) && a.getCellStatus(r, c+1) != HAS_POISON){
                bestDir = EAST;
                stay = false;
            }
        }
        //Compare South
        if(r != a.rows()){
            if(numRatsHere == numRatSurrounding(&a, r+1, c) && a.getCellStatus(r+1, c) != HAS_POISON){
                bestDir = SOUTH;
                stay = false;
            }
        }
        //Compare West
        if(c != 1){
            if(numRatsHere == numRatSurrounding(&a, r, c-1) && a.getCellStatus(r, c-1) != HAS_POISON){
                bestDir = WEST;
                stay = false;
            }
        }
    }
    
    return !stay;
}

int Game::computeDanger(const Arena& a, int r, int c)
{
    // Our measure of danger will be the number of rats that might move
    // to position r,c.  If a rat is at that position, it is fatal,
    // so a large value is returned.
    
    if (a.numberOfRatsAt(r,c) > 0)
        return MAXRATS+1;
    
    int danger = 0;
    if (r > 1)
        danger += a.numberOfRatsAt(r-1,c);
    if (r < a.rows())
        danger += a.numberOfRatsAt(r+1,c);
    if (c > 1)
        danger += a.numberOfRatsAt(r,c-1);
    if (c < a.cols())
        danger += a.numberOfRatsAt(r,c+1);
    
    return danger;
}

//Return num of rats surrounding the square. Returns 9999 if there's a rat on the square.
//Equiv to computeDanger
int Game::numRatSurrounding(const Arena* a, int r, int c){
    if(a->numberOfRatsAt(r, c) > 0)
        return 9999;
    int count = 0;
    count += a->numberOfRatsAt(r+1, c) + a->numberOfRatsAt(r-1, c) + a->numberOfRatsAt(r, c+1) + a->numberOfRatsAt(r, c-1);
    return count;
}

