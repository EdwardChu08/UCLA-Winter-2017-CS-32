//
//  History.cpp
//  CS-32-Assignment-1
//
//  Created by Edward Chu on 11/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include <iostream>

#include "History.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols){
    m_rows = nRows;
    m_cols = nCols;
    
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            m_grid[i][j] = 0;
        }
    }
}


//Records that an unpoisoned rat has eaten a pellet at (r,c)
bool History::record(int r, int c){
    if(r > m_rows || c > m_cols || r < 1 || c < 1)
        return false;
    
    m_grid[r][c]++;
    
    return true;
}


//Displays History
void History::display() const{
    clearScreen();
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            if(m_grid[i][j] == 0)
                cout << '.';
            else if(m_grid[i][j] > 26)
                cout << 'Z';
            else
                cout << "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[m_grid[i][j]-1];
        }
        cout << endl;
    }
    cout << endl;
}
