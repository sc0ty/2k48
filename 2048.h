/*
 * 2048 game engine
 *
 * Mike Szymaniak, 2014
 */

#ifndef __2048_H__
#define __2048_H__

#include <cstddef>


#define SIZE 4
#define WX SIZE
#define WY SIZE


class Grid;

// function to be called for rendering animation
typedef void (*DrawGridHandler)(const Grid& grid);


// position in the grid
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


// directions to shift
enum Direction
{
	DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
};


// game engine
class Grid
{
public:
	Grid();
	void reset();

	bool shift(Direction dir, DrawGridHandler drawgr = NULL);
	bool genBlock();

	int getTile(int x, int y) const;

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

