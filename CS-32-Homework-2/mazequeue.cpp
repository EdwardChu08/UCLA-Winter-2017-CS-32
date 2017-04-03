//
//  mazequeue.cpp
//  CS-32-Homework-2
//
//  Created by Edward Chu on 4/2/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include <string>
#include <queue>
#include <iostream>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);


class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    queue<Coord> coordStack;
    
    Coord startPt(sr, sc);
    Coord endPt(er, ec);
    coordStack.push(startPt);
    maze[sr][sc] = 'W';
    
    while(!coordStack.empty()){
        Coord current = coordStack.front();
        
        coordStack.pop();
        
        //Endpoint reached!
        if(current.r() == endPt.r() && current.c() == endPt.c())
            return true;
        
        //NESW
        if(maze[current.r()-1][current.c()] == '.'){
            Coord temp(current.r()-1, current.c());
            coordStack.push(temp);
            maze[current.r()-1][current.c()] = 'W';
        }
        
        
        if(maze[current.r()][current.c()+1] == '.'){
            Coord temp(current.r(), current.c()+1);
            coordStack.push(temp);
            maze[current.r()][current.c()+1] = 'W';
        }
        
        if(maze[current.r()+1][current.c()] == '.'){
            Coord temp(current.r()+1, current.c());
            coordStack.push(temp);
            maze[current.r()+1][current.c()] = 'W';
        }
        
        if(maze[current.r()][current.c()-1] == '.'){
            Coord temp(current.r(), current.c()-1);
            coordStack.push(temp);
            maze[current.r()][current.c()-1] = 'W';
        }
        
        
        
    }
    
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}

