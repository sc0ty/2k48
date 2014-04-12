/*
 * Grid rendering functions
 *
 * Mike Szymaniak, 2014
 */

#include "render.h"
#include "2048.h"
#include "io.h"
#include <unistd.h>


#define COLORS_NO	14			// number of defined tile colors
#define ANIM_DELAY	50*1000		// animation delay in us


void drawGrid(const Grid& grid)
{
	rescur();

	addch('\n');
	for (int y=0; y<WY; ++y)
	{
		for (int x=0; x<WX; ++x)
		{
			addstr("   ");
			drawTile(grid.getTile(x, y));
		}
		addstr("\n\n\n");
	}
	
	addstr("   ");
	coloron(14);
	printw("   2k48      moves: %-4d ", grid.getMoves());
	coloroff(14);

	refresh();
	animDelay();
}

void drawTile(int tile)
{
	if (tile)
	{
		int i = 1 << tile;
		int color = (tile % COLORS_NO) + 1;

		coloron(color);
		if (i < 100) printw(" %2d ", i);
		else printw("%4d", i);
		coloroff(color);
	}
	else addstr("  . ");
}

void animDelay()
{
	usleep(ANIM_DELAY);
}

