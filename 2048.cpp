/*
 * 2048 game engine
 *
 * Mike Szymaniak, 2014
 */

#include "2048.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>


#define tile(p) tiles[p.x][p.y]


Grid::Grid()
{
	reset();
}

void Grid::reset()
{
	memset(tiles, 0, sizeof(tiles));
	moves = 0;
}

bool Grid::shift(Direction dir, DrawGridHandler drawgr)
{
	Pos src, dst;
	bool cont = true;
	bool moved = false;

	while (cont)
	{
		cont = false;
		for (int i=0; i<SIZE; ++i)
		{
			switch (dir)
			{
				case DIR_UP:    cont |= shiftLineUp(i);    break;
				case DIR_DOWN:  cont |= shiftLineDown(i);  break;
				case DIR_LEFT:  cont |= shiftLineLeft(i);  break;
				case DIR_RIGHT: cont |= shiftLineRight(i); break;
				default: return false;
			}
			if (cont && !moved) moves++;
			moved |= cont;
		}
		if (drawgr) drawgr(*this);
	}
	resetTileMoves();
	return moved;
}

bool Grid::genBlock()
{
	// count empty places
	int empty = 0;
	for (int y=0; y<WY; ++y)
	{
		for (int x=0; x<WX; ++x)
		{
			if (tiles[x][y] == 0) ++empty;
		}
	}

	if (empty == 0) return false;

	int id = rand() % empty;

	// place new number in random place
	for (int y=0; y<WY; ++y)
	{
		for (int x=0; x<WX; ++x)
		{
			if (tiles[x][y] == 0) 
			{
				if (id-- == 0)
				{
					tiles[x][y] = rand() >= RAND_MAX/10 ? 1 : 2;
					return true;
				}
			}
		}
	}
	return false;
}

int Grid::getTile(int x, int y) const
{
	if (x < 0 || x >= WX) return -1;
	if (y < 0 || y >= WY) return -1;
	return abs(tiles[x][y]);
}

bool Grid::canMove() const
{
	for (int y=0; y<WY; ++y)
	{
		for (int x=0; x<WX; ++x)
		{
			if (tiles[x][y] == 0) return true;
			if ((x > 0) && (tiles[x-1][y] == tiles[x][y])) return true;
			if ((y > 0) && (tiles[x][y-1] == tiles[x][y])) return true;
		}
	}
	return false;
}

int Grid::getMaxTile() const
{
	int max = tiles[0][0];
	for (int y=0; y<WY; ++y)
	{
		for (int x=0; x<WX; ++x)
		{
			if (abs(tiles[x][y]) > max) max = abs(tiles[x][y]);
		}
	}
	return max ? 1<<max : 0;
}

int Grid::getMoves() const
{
	return moves;
}

void Grid::mkTestGrid()
{
	int id = 0;
	for (int y=0; y<WY; ++y)
	{
		for (int x=0; x<WX; ++x)
		{
			tiles[x][y] = id++;
			id %= 12;
		}
	}
}

bool Grid::load(const char* fname)
{
	FILE* f = fopen(fname, "r");
	if (f == NULL) return false;

	size_t res = fread(tiles, sizeof(tiles), 1, f);
	if (res == 1) res = fread(&moves, sizeof(moves), 1, f);
	fclose(f);
	return (res == 1);
}

bool Grid::save(const char* fname) const 
{
	FILE* f = fopen(fname, "w");
	if (f == NULL) return false;

	size_t res = fwrite(tiles, sizeof(tiles), 1, f);
	if (res == 1) res = fwrite(&moves, sizeof(moves), 1, f);
	fclose(f);
	return (res == 1);
}

bool Grid::shiftLineLeft(int y)
{
	Pos src, dst;
	dst.y = src.y = y;
	bool moved = false;

	for (src.x=1; src.x<WX; src.x++)
	{
		dst.x = src.x - 1;
		moved |= moveTile(dst, src);
	}
	return moved;
}

bool Grid::shiftLineRight(int y)
{
	Pos src, dst;
	dst.y = src.y = y;
	bool moved = false;

	for (src.x=WX-2; src.x>=0; src.x--)
	{
		dst.x = src.x + 1;
		moved |= moveTile(dst, src);
	}
	return moved;
}

bool Grid::shiftLineUp(int x)
{
	Pos src, dst;
	dst.x = src.x = x;
	bool moved = false;

	for (src.y=1; src.y<WY; src.y++)
	{
		dst.y = src.y - 1;
		moved |= moveTile(dst, src);
	}
	return moved;
}

bool Grid::shiftLineDown(int x)
{
	Pos src, dst;
	dst.x = src.x = x;
	bool moved = false;

	for (src.y=WY-2; src.y>=0; src.y--)
	{
		dst.y = src.y + 1;
		moved |= moveTile(dst, src);
	}
	return moved;
}

bool Grid::moveTile(Pos dst, Pos src)
{
	if (tile(src) == 0)
	{
		return false;
	}
	else if (tile(dst) == 0)
	{
		tile(dst) = tile(src);
		tile(src) = 0;
		return true;
	}
	else if (tile(dst) == tile(src) && tile(dst) > 0)
	{
		tile(dst) = -1 * tile(dst) - 1;
		tile(src) = 0;
		return true;
	}
	return false;
}

void Grid::resetTileMoves()
{
	for (int y=0; y<WY; ++y)
	{
		for (int x=0; x<WX; ++x)
		{
			tiles[x][y] = abs(tiles[x][y]);
		}
	}
}

#undef tile

