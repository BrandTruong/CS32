#include <string>
#include <stack>
#include <iostream>

using namespace std;

//Global Constants
const char discovered = '#';
const char unknown = '.';

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

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

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X.XXX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10, 10, 4, 3, 1, 8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
/*
    Push the starting coordinate(sr, sc) onto the coordinate stackand
        update maze[sr][sc] to indicate that the algorithm has encountered
        it(i.e., set maze[sr][sc] to have a value other than '.').
        While the stack is not empty,
    { Pop the top coordinate off the stack.This gives you the current
            (r,c) location that your algorithm is exploring.
        If the current(r,c) coordinate is equal to the ending coordinate,
            then we've solved the maze so return true! 
        Check each place you can move from the current cell as follows :
            If you can move EAST and haven't encountered that cell yet,
                then push the coordinate(r,c + 1) onto the stack and update
                maze[r][c + 1] to indicate the algorithm has encountered it.
            If you can move SOUTH and haven't encountered that cell yet,
                then push the coordinate(r + 1,c) onto the stack and update
                maze[r + 1][c] to indicate the algorithm has encountered it.
            If you can move WEST and haven't encountered that cell yet,
                then push the coordinate(r,c - 1) onto the stack and update
                maze[r][c - 1] to indicate the algorithm has encountered it.
            If you can move NORTH and haven't encountered that cell yet,
                then push the coordinate(r - 1,c) onto the stack and update
                maze[r - 1][c] to indicate the algorithm has encountered it.
    }
        There was no solution, so return false
        */
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    stack <Coord> s;
    Coord start(sr, sc); //starting coordinate
    s.push(start);
    maze[sr][sc] = discovered; //mark as discovered with #, unknown = .
    while (!s.empty()) {
        int cRow = s.top().r();
        int cCol = s.top().c();
        s.pop();
        if (cRow == er && cCol == ec) {
            return true; //found endpoint
        }
        //East
        if (cCol + 1 < nCols && maze[cRow][cCol + 1] == unknown) {
            Coord east(cRow, cCol + 1);
            maze[cRow][cCol + 1] = discovered;
            s.push(east);
        }
        //South
        if (cRow + 1 < nRows && maze[cRow + 1][cCol] == unknown) {
            Coord south(cRow + 1, cCol);
            maze[cRow + 1][cCol] = discovered;
            s.push(south);
        }
        //West
        if (cCol - 1 >= 0 && maze[cRow][cCol - 1] == unknown) {
            Coord west(cRow, cCol - 1);
            maze[cRow][cCol - 1] = discovered;
            s.push(west);
        }
        //North
        if (cRow - 1 >= 0 && maze[cRow - 1][cCol] == unknown) {
            Coord north(cRow - 1, cCol);
            maze[cRow - 1][cCol] = discovered;
            s.push(north);
        }
    }
    return false; //no solution
}

