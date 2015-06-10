//
//  maze.h
//  lab_5
//
//  Created by Gentry Tran on 5/11/13.
//  Copyright (c) 2013 Gentry Tran. All rights reserved.
//

#ifndef __lab_5__maze__
#define __lab_5__maze__

#include <iostream>

#endif /* defined(__lab_5__maze__) */
#include <vector>
#include <cstdlib>
#include <utility>
#include <iostream>
#include <algorithm>
using namespace std;

#include <ncurses.h>

const char MAZE_DUDE  = 'D';
const char MAZE_WALL  = '#';
const char MAZE_SPACE = ' ';
const char MAZE_TRACE = '.';
const char MAZE_GOLD  = 'G';
const char MAZE_MONEY = '$';
void display_maze(const vector<vector<char> >& m);

void display_maze_with_attributes(
                                  const vector<vector<char> >& m, unsigned int attr);

int random_n (int n);


void dig_maze(vector<vector<char> >& m, int r, int c, char *wall);

void build_maze(vector<vector<char> >& m);

void print_gameover(const vector<vector<char> >& m);

