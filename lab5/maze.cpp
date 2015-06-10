/*
 * A simple terminal-based maze game written in C++ using Ncurses library.
 *
 * Copyright (C) 2013 Jae Woo Lee - All Rights Reserved.
 */

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
const int DELAY = 100 *1000; //wait millisecond
bool done = false;
int goldRowGlobal = -1;
int goldColumnGlobal = -1;
int currentRow = -1;
int currentColumn = -1;

/*
 * Display the maze on the terminal screen by printing each 
 * character using Ncurses mvaddch() function.
 */
void display_maze(const vector<vector<char> >& m)
{
    for(int row = 0; row < m.size(); row++) {
        for(int col = 0; col < m[0].size(); col++) {
            mvaddch(row, col, m[row][col]);
        }
    }
    // Ncurses doesn't actually print anything to the physical 
    // terminal screen until refresh() is called.
    refresh();
}

/*
 * Display the maze on the terminal screen by printing each 
 * character with the given video attributes using Ncurses mvaddch().
 *
 * An example of attr could be (A_BOLD | A_BLINK) to make the character
 * bold and blinking.  See "man curs_attr" for more detail.
 */
void display_maze_with_attributes(
        const vector<vector<char> >& m, unsigned int attr)
{
    for(int row = 0; row < m.size(); row++) {
        for(int col = 0; col < m[0].size(); col++) {
            mvaddch(row, col, m[row][col] | attr);
        }
    }
    // Ncurses doesn't actually print anything to the physical 
    // terminal screen until refresh() is called.
    refresh();
}

/*
 * Returns a random integer x, where 0 <= x < n.
 */
int random_n (int n) { return random() % n; }

/*
 * Recursively "dig" maze corridors starting from row r and column c.
 * 
 * The maze matrix m is initialized in alternating walls and cells 
 * like this:
 *
 *      #########
 *      #.#.#.#.# 
 *      #########
 *      #.#.#.#.# 
 *      #########
 *      #.#.#.#.# 
 *      #########
 */
void dig_maze(vector<vector<char> >& m, int r, int c, char *wall)
{
    // Are we out of bounds?
    if (r < 0 || c < 0 || r >= m.size() || c >= m[0].size())
        return;

    // Are we on a wall, or on a cell that we have visited already?
    if (m[r][c] == MAZE_WALL || m[r][c] == MAZE_SPACE)
        return;

    // The wall pointer points to the wall that we jumped over 
    // between recursive calls.  Knock it down.
    // (wall == NULL for the 1st invocation of this function.)
    if (wall) {
        *wall = MAZE_SPACE;
    }

    // Dig this cell.
    m[r][c] = MAZE_SPACE;
    display_maze(m);

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
        dig_maze(m, rr, cc, wall);
    }
}

/*
 * Build a maze on a maze matrix m.
 *
 * The maze matrix m is assumed to have been filled with MAZE_WALL.
 */
void build_maze(vector<vector<char> >& m)
{
    // min/maxRow/Col indicate the "interior" of the maze.
    int minRow = 1;
    int minCol = 1;
    int maxRow = m.size()    - 2;
    int maxCol = m[0].size() - 2;

    // Seed the pseudo-random number generator with the screen size
    // so that we get the same maze on a given terminal size.
    srandom(maxRow * maxCol);

    // Initialize the maze matrix m in alternating walls and cells
    // like this:
    //
    //      #########
    //      #.#.#.#.# 
    //      #########
    //      #.#.#.#.# 
    //      #########
    //
    for (int r = minRow; r <= maxRow; r += 2) {
        for (int c = minCol; c <= maxCol; c += 2) {
            m[r][c] = MAZE_TRACE;
        }
    }

    // Start digging recursively.
    dig_maze(m, 1, 1, NULL);
}

/*
 * Print game-over message at the lower center in reverse video 
 * and blinking.
 */
void print_gameover(const vector<vector<char> >& m)
{
    string s("[GAME OVER]");
    move(m.size()-1, (m[0].size()-1)/2 - (s.length()-1)/2);
    attron(A_REVERSE | A_BLINK);
    printw("%s", s.c_str());
    attroff(A_REVERSE | A_BLINK);
}

void print_maze(const vector<vector<char> >& m)
{
    for(int row = 0; row < m.size(); row++) {
	for(int col = 0; col < m[0].size(); col++) {
	    if(col == m[0].size()-1){
		std::cout << m[row][col] << "\n";
	    }else{
		std::cout << m[row][col];
	    }
	}
    }
}


void solve_maze(vector<vector<char> >& m, int r, int c)
{
    if (r < 0 || c < 0 || r >= m.size() || c >= m[0].size())
	return;
    if (m[r][c] == MAZE_WALL || m[r][c] == MAZE_TRACE)
	return;
	
    if(r == goldRowGlobal && c == goldColumnGlobal){
		done = true;
	print_gameover(m);	
	return;
	}
	
	if(done) return;
	
	currentRow = r;
	currentColumn = c;
	
	m[r][c] = MAZE_TRACE;
	mvaddch(r, c, MAZE_TRACE);
	mvaddch(r, c, MAZE_DUDE);
	refresh();
	usleep(DELAY);

    char D[4] = { 'N', 'E', 'W', 'S' };

    for(int i = 0; i < 4; i++) {
	switch(D[i]) 
	{
	    case 'N':
		if((m[r-1][c] != MAZE_WALL) && (m[r-1][c] != MAZE_TRACE)){
		    mvaddch(r, c, MAZE_TRACE);
			solve_maze(m,r-1,c);
		}
		break;

	    case 'E':
		if((m[r][c+1] != MAZE_WALL) && (m[r][c+1] != MAZE_TRACE)){		
		    mvaddch(r, c, MAZE_TRACE);
			solve_maze(m,r,c+1);
		}
		break;

	    case 'W':
		if((m[r][c-1] != MAZE_WALL) && (m[r][c-1] != MAZE_TRACE)){
		    mvaddch(r, c, MAZE_TRACE);
			solve_maze(m,r,c-1);
		}		
		break;

	    case 'S':
		if((m[r+1][c] != MAZE_WALL) && (m[r+1][c] != MAZE_TRACE)){
			mvaddch(r, c, MAZE_TRACE);
			solve_maze(m,r+1,c);
		}
		break;
	    default:
		cerr << "unknown direction" << endl;
		exit(1);
	}
    }

}    

int main() 
{
    // Initialize the terminal screen using Ncurses init routines.

    initscr(); // switch terminal screen to fullscreen curses mode
    cbreak(); // disable line buffering so that we get raw keystrokes
    keypad(stdscr, TRUE); // enable keypad mode (for arrow and fn keys)
    noecho(); // do not print the user's keystrokes to the screen
    curs_set(0); // make the cursor invisible

    // Initialize the maze matrix with the full screen dimemsions.
    // - the Ncurses libarary's initscr() sets LINES and COLS to  
    //   the current terminal screen size.

    vector<vector<char> > m(LINES, vector<char>(COLS, MAZE_WALL));

    // Build and display a maze.

    build_maze(m);
    display_maze(m);

    // Place gold ('G') near the lower right corner.

    int goldRow = -1;
    int goldCol = -1;
    for (int row = m.size() - 2; row >= 1; --row) {
        for (int col = m[0].size() - 2; col >= 1; --col) {
            if (m[row][col] == MAZE_SPACE) {
                goldRow = row;
                goldCol = col;
				goldRowGlobal = row;
				goldColumnGlobal = col;
                break;
            }
        }
        if (goldRow != -1) 
            break;
    }
    if (goldRow != -1) {
        mvaddch(goldRow, goldCol, MAZE_GOLD);
    } else {
        cerr << "no space to put gold - make your screen bigger" << endl;
        exit(1);
    }

    // Start the game!

    // The dude start at the upper left corner.
    int row = 1;
    int col = 1;
    mvaddch(row, col, MAZE_DUDE);

    while (1) {
        int key = getch(); // read a keystroke.

        // Type 'q' to quit.
        if (key == 'q') {
            break;
        }

	if (key == 'a') {
	    solve_maze(m, row, col);
		row = currentRow;
		col = currentColumn;
	}
        // Arrow keys move the dude, leaving breadcrumbs on the screen.
        if (key == KEY_UP || key == KEY_DOWN || 
                key == KEY_LEFT || key == KEY_RIGHT) {
            mvaddch(row, col, MAZE_TRACE);
            if (KEY_UP    == key && MAZE_SPACE == m[row - 1][col]) row--;
            if (KEY_DOWN  == key && MAZE_SPACE == m[row + 1][col]) row++;
            if (KEY_LEFT  == key && MAZE_SPACE == m[row][col - 1]) col--;
            if (KEY_RIGHT == key && MAZE_SPACE == m[row][col + 1]) col++;
            mvaddch(row, col, MAZE_DUDE);
            refresh(); // update the terminal screen.
        }

        // If the dude found the gold, game over.
        if (row == goldRow && col == goldCol) {
            print_gameover(m);
            while ((key = getch()) != 'q') {}
            break;
        }
    }

    // All done; get out of the fullscreen curses mode.
    endwin();
} 
