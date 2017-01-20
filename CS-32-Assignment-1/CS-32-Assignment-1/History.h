//
//  History.hpp
//  CS-32-Assignment-1
//
//  Created by Edward Chu on 11/1/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#ifndef History_h
#define History_h

#include <stdio.h>
#include "globals.h"

class History
{
    public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
    
    private:
        int     m_grid[MAXROWS][MAXCOLS];
        int     m_rows;
        int     m_cols;
};

#endif /* History_h */
