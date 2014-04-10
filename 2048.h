#ifndef __2048_H__
#define __2048_H__

#include <cstdio>

#define SIZE 4
#define WX SIZE
#define WY SIZE

#define COLORS_NO 14	// number of defined tile colors


struct Pos
{
	int x, y;

	Pos() : x(0), y(0) {};

	Pos(int x, int y) : x(x), y(y) {};

	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};


void drawTile(int val);


class Grid
{
public:
	Grid();
	void reset();

	void show() const;
	bool shift(int dir, unsigned showDelay=0);
	bool genBlock();

	bool canMove() const;
	int getMoves() const;
	int getMaxTile() const;

	bool load(const char* fname);
	bool save(const char* fname) const;

	void mkTestGrid();

private:
	bool shiftLineLeft(int y);
	bool shiftLineRight(int y);
	bool shiftLineUp(int x);
	bool shiftLineDown(int x);

	bool moveTile(Pos dst, Pos src);
	void resetTileMoves();

private:
	int tiles[SIZE][SIZE];
	int moves;
};

#endif // __2048_H__

