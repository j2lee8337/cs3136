//
//  main.cpp
//  lab_5
//
//  Created by Gentry Tran on 5/11/13.
//  Copyright (c) 2013 Gentry Tran. All rights reserved.
//

#include <iostream>
#include "maze.h"

void print_maze(const vector<vector<char> >& m)
{
    for(int row = 0; row < m.size(); row++) {
        for(int col = 0; col < m[0].size(); col++) {
            if(col == m[0].size()-1){
                std::cout << m[row][col] << "\n";
            }
            else{
                std::cout << m[row][col];
            }
        }
    }
    // Ncurses doesn't actually print anything to the physical
    // terminal screen until refresh() is called.
    
}

void solve_maze(vector<vector<char> >& m, int r, int c)
{
    //DFS
    
    //Out of bounds
    if (r < 0 || c < 0 || r >= m.size() || c >= m[0].size())
        return;
    
    // Are we on a wall, or on a cell that we have visited already?
    if (m[r][c] == MAZE_WALL || m[r][c] == MAZE_SPACE)
        return;
    m[r][c] = MAZE_TRACE;
    
    
    // Randomly decide the order in which we explore the directions
    // N, S, E, W.  We use STL's random_shuffle() to shuffle an array.
    char D[4] = { 'N', 'S', 'E', 'W' };
    random_shuffle(D, D+4, random_n);
    
    for (int i = 0; i < 4; i++) {
        
        // m[rr][cc] will be the cell that we'll try to dig.
        int rr = r;
        int cc = c;
        char *wall;
        
        switch (D[i]) {
            case 'N':
                rr -= 2;
                wall = &m[r-1][c];
                break;
            case 'S':
                rr += 2;
                wall = &m[r+1][c];
                break;
            case 'W':
                cc -= 2;
                wall = &m[r][c-1];
                break;
            case 'E':
                cc += 2;
                wall = &m[r][c+1];
                break;
            default:
                cerr << "unknown direction" << endl;
                exit(1);
        }
        
        // recursively dig from (rr,cc).
        solve_maze(m

    }

    
}

int main(int argc, const char * argv[])
{
    vector<vector<char> > m(10, vector<char>(10, MAZE_WALL));

    build_maze(m);
    
    int goldRow = -1;
    int goldCol = -1;
    for (int row = m.size() - 2; row >= 1; --row) {
        for (int col = m[0].size() - 2; col >= 1; --col) {
            if (m[row][col] == MAZE_SPACE) {
                goldRow = row;
                goldCol = col;
                break;
            }
        }
        if (goldRow != -1)
            break;
    }
    if (goldRow != -1) {
        //mvaddch(goldRow, goldCol, MAZE_GOLD);
        m[goldRow][goldCol] = MAZE_GOLD;
        
    } else {
        cerr << "no space to put gold - make your screen bigger" << endl;
        exit(1);
    }
   
    
    // The dude start at the upper left corner.
    int row = 1;
    int col = 1;
    m[row][col] = MAZE_DUDE;
    
    solve_maze(m, row, col);
    print_maze(m);
    

    // insert code here...
//    std::cout << "Hello, World!\n";
    return 0;
}

