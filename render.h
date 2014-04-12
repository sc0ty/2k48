/*
 * Grid rendering functions
 *
 * Mike Szymaniak, 2014
 */

#ifndef __RENDER_H__
#define __RENDER_H__

#include <cstddef>


class Grid;


void drawGrid(const Grid& grid);
void drawTile(int tile);
void animDelay();

#endif //  __RENDER_H__

