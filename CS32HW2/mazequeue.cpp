#include <queue>
#include <string>
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
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    queue <Coord> q;
    Coord start(sr, sc); //starting coordinate
    q.push(start);
    maze[sr][sc] = discovered; //mark as discovered with #, unknown = .
    while (!q.empty()) {
        int cRow = q.front().r();
        int cCol = q.front().c();
        cout << "(" << cRow << ", " << cCol << ")" << endl;
        q.pop();
        if (cRow == er && cCol == ec) {
            return true; //found endpoint
        }
        //East
        if (cCol + 1 < nCols && maze[cRow][cCol + 1] == unknown) {
            Coord east(cRow, cCol + 1);
            maze[cRow][cCol + 1] = discovered;
            q.push(east);
        }
        //South
        if (cRow + 1 < nRows && maze[cRow + 1][cCol] == unknown) {
            Coord south(cRow + 1, cCol);
            maze[cRow + 1][cCol] = discovered;
            q.push(south);
        }
        //West
        if (cCol - 1 >= 0 && maze[cRow][cCol - 1] == unknown) {
            Coord west(cRow, cCol - 1);
            maze[cRow][cCol - 1] = discovered;
            q.push(west);
        }
        //North
        if (cRow - 1 >= 0 && maze[cRow - 1][cCol] == unknown) {
            Coord north(cRow - 1, cCol);
            maze[cRow - 1][cCol] = discovered;
            q.push(north);
        }
    }
    return false; //no solution
}