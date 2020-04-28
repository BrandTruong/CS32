#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int    nRows;
	int    nCols;
	char   m_grid[MAXROWS][MAXCOLS];
};
#endif