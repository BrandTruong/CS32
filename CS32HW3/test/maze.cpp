/*
	If the start location is equal to the ending location, then we've
		solved the maze, so return true.
	Mark the start location as visted.
	For each of the four directions,
		If the location one step in that direction (from the start
		location) is unvisited,
			then call pathExists starting from that location (and
				ending at the same ending location as in the
				current call).
			 If that returned true,
				 then return true.
	Return false
*/

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	if (sr == er && sc == ec) {
		return true;
	}
	maze[sr][sc] = 'o'; //Visited = 'o', Path = '.'
	if (sr - 1 >= 0 && maze[sr - 1][sc] == '.') { //North
		if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec)) {
			return true;
		}
	}
	if (sr + 1 < nRows && maze[sr + 1][sc] == '.') { //South
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)) {
			return true;
		}
	}
	if (sc - 1 >= 0 && maze[sr][sc - 1] == '.') { //West
		if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec)) {
			return true;
		}
	}
	if (sc + 1 < nCols && maze[sr][sc + 1] == '.') { //East
		if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec)) {
			return true;
		}
	}
	return false;
}
int main() {
	string maze1[10] = {
	  "XXXXXXXXXX",
	  "X.X..X...X",
	  "X.XX.X.XXX",
	  "X....X.X.X",
	  "XX.X.X...X",
	  "XXX..X.X.X",
	  "X...X...XX",
	  "X.XX..X.XX",
	  "X....X...X",
	  "XXXXXXXXXX",
	};

	string maze2[10] = {
	  "XXXXXXXXXX",
	  "X.X..X...X",
	  "XXXX.X.XXX",
	  "X....X.X.X",
	  "XX.X.X...X",
	  "XXX..X.X.X",
	  "X...X...XX",
	  "X.XX..X.XX",
	  "X....X...X",
	  "XXXXXXXXXX",
	};

	string maze3[10] = {
	  "XXXXXXXXXX",
	  "XX.....XXX",
	  "X..XX....X",
	  "X...X...XX",
	  "X.X.XXX..X",
	  "XXXX..X..X",
	  "XX....X..X",
	  "X.......XX",
	  "X..XXXXXXX",
	  "XXXXXXXXXX",
	};

	string maze4[10] = {
	  "XXXXXXXXXX",
	  "XX.....XXX",
	  "X..XX....X",
	  "X...X...XX",
	  "X.X.XXX..X",
	  "XXXX..X..X",
	  "XX....X..X",
	  "X.X.....XX",
	  "X..XXXXXXX",
	  "XXXXXXXXXX",
	};

	assert(pathExists(maze1, 10, 10, 8, 6, 1, 1));
	assert(!pathExists(maze2, 10, 10, 8, 6, 1, 1));
	assert(pathExists(maze3, 10, 10, 4, 3, 7, 1));
	assert(!pathExists(maze4, 10, 10, 4, 3, 7, 1));
	cout << "Passed all tests" << endl;
}