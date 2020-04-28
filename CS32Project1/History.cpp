///////////////////////////////////////////////////////////////////////////
//  History implementation
///////////////////////////////////////////////////////////////////////////

#include "History.h"
#include "globals.h"

#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	this->nRows = nRows;
	this->nCols = nCols;
	for (int i = 0; i < this->nRows; i++) {
		for (int j = 0; j < this->nCols; j++) {
			m_grid[i][j] = '.';
		}
	}
}
bool History::record(int r, int c)
{
	if (r <= 0 || c <= 0 || r > nRows || c > nCols) {
		return false;
	}
	if (m_grid[r - 1][c - 1] == '.') {
		m_grid[r - 1][c - 1] = 'A';
	}
	else if (m_grid[r - 1][c - 1] == 'Z') {
		m_grid[r - 1][c - 1] = 'Z';
	}
	else {
		m_grid[r - 1][c - 1]++;
	}
	return true;
}
void History::display() const
{
	clearScreen();
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++) {
			cout << m_grid[i][j];
		}
		cout << "\n";
	}
	cout << "\n";
}
