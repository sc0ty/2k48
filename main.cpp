/*
 * 2048 main game functions
 *
 * Mike Szymaniak, 2014
 */

#include "io.h"
#include "2048.h"
#include "render.h"
#include <cstdlib>
#include <ctime>
#include <cstddef>


#define SAVE_FILE	"2k48.save"


static Grid grid;


static void exitHandler(const char* cause)
{
	printf("Exiting, cause: %s\n", cause);
#ifndef DEAMON
	grid.save(SAVE_FILE);
#endif
}


void init(int argc, char *argv[])
{
	initIO(&exitHandler);
    srand (time(NULL));

#ifdef LOGON
	int opt;
	char* host = const_cast<char*>("<unknown>");
	char* user = NULL;
	while ((opt = getopt(argc, argv, "h:p:")) != -1)
	{
		switch (opt)
		{
			case 'h': host = optarg; break;
			case 'p': user = optarg; break;
		}
	}
    if (user) LOG("started from %s@%s", user, host);
	else LOG("started from %s", host);
#endif

}


int getArrowKey(Direction* dir)
{
	int k = getlowkey();
	switch (k)
	{
		case KEY_UP:
		case 'w': *dir = DIR_UP;	break;
		case KEY_DOWN:
		case 's': *dir = DIR_DOWN;	break;
		case KEY_LEFT:
		case 'a': *dir = DIR_LEFT;	break;
		case KEY_RIGHT:
		case 'd': *dir = DIR_RIGHT;	break;
		default: *dir = DIR_NONE;
	}
	return k;
}


void showPopup(const char* msg)
{
	addstr("\n\n   ");
	blinkon();
	coloron(15);
	printw("%s", msg);
	coloroff(15);
	blinkoff();
	refresh();
	getanykey();
}


void showInfo()
{
#define FMTENT(head, msg) \
	boldon(); \
	printw("     %12s", head); \
	boldoff(); \
	addstr(" " msg "\n");

#define FMTPAR(msg) \
	boldon(); \
	addstr("     " msg ":\n"); \
	boldoff();

	clear();
	addstr("\n   ");

	coloron(14);
	addstr("        2k48  by Mike Szymaniak - informations        ");
	coloroff(14);

	addstr("\n\n"
			"     Terminal version of 2048 game - http://git.io/2048\n"
#ifdef DEAMON
			"     Please do not try to hack my machine!\n"
#endif
			"\n");

	FMTENT("author:", "Mike 'sc0ty' Szymaniak");
	FMTENT("email:", "sc0typl[at]gmail.com");
	FMTENT("webpage:", "http://sc0ty.pl\n");
	
	FMTPAR("HOW TO PLAY");
	addstr( "        Use your arrow keys to move the tiles. When two\n"
			"        tiles with the same number touch, they merge\n"
			"        into one.\n\n");

	FMTPAR("CONTROLS");
	FMTENT("move:", "arrows or W/S/A/D");
	FMTENT("quit:", "Q\n");

	FMTPAR("GLITCHES");
	addstr(	"        If you see glitches during game - press R. It\n"
			"        will force screen to be repainted every frame.\n\n");

	FMTPAR("LICENSE");
	addstr( "        GNU GENERAL PUBLIC LICENSE Version 2, June 1991\n"
			"        source code: https://github.com/sc0ty/2k48\n\n\n"
			"        Press any key to continue...");
	getanykey();

#undef FMTENT
}


int mainMenu()
{
#define MENUENT(key, cmd) \
	addstr("         "); \
	boldon(); \
	addstr(key); \
	boldoff(); \
	addstr(" - " cmd "\n");

	clear();
	addstr("\n   ");
	coloron(14);
	addstr(" 2k48  by Mike Szymaniak ");
	coloroff(14);
	addstr("\n\n   Press:\n");
	MENUENT("C", "continue")
	MENUENT("N", "new game")
	MENUENT("I", "show info")
#ifdef DEBUG
	MENUENT("T", "test grid");
#endif
	MENUENT("Q", "quit")
	addstr("\n   http://sc0ty.pl");
	refresh();

	char ch = getlowkey();
	if (ch == 'q' || ch == 'n')
	{
		addstr("\n\n   Press again to confirm\n");
		refresh();
		if (ch != getlowkey()) return 0;
	}

	return ch;

#undef MENUENT
}


void game()
{
	int key = 0;
	Direction dir = DIR_NONE;
	static int info = 5;

	clear();
	if (!grid.canMove())
	{
		drawGrid(grid);
		getanykey();
		return;
	}

	if (grid.getMaxTile() == 0) grid.genBlock();
	bool canwin = grid.getMaxTile() < 2048;

	while (true)
	{
		rescur();
		drawGrid(grid);
		if (info-- > 0) printw("\n\n if you have display problems - press R\n");

		if (canwin && grid.getMaxTile() >= 2048)
		{
			animDelay();
			LOG("win with %d moves", grid.getMoves());
			showPopup("        You win!         ");
			return;
		}
		
		if (!grid.canMove())
		{
			animDelay();
			LOG( "lose with %d moves, score %d", grid.getMoves(), grid.getMaxTile());
			showPopup("        Game over!       ");
			return;
		}

		do
		{
			key = getArrowKey(&dir);
			if (key == 'q') return;
			else if (key == 'r')
			{
				bool rp = swRescur();
				LOG("render mode %d", rp);
				clear();
				drawGrid(grid);
				printw("\n\n   Repaint mode %s", rp ? "ON" : "OFF");
				info = 0;
			}
		}
		while (!grid.shift(dir, drawGrid));

		animDelay();
		grid.genBlock();
		if (info == 0) clear();
	}
}


int main(int argc, char *argv[])
{
	init(argc, argv);

	char ch = 0;
	bool play = true;

#ifdef DEAMON
	showInfo();
#else
	if (!grid.load(SAVE_FILE)) showInfo();
#endif

	if (!grid.canMove()) grid.reset();

	while (true)
	{
		if (play) game();

		ch = mainMenu();

		if (ch == 'q') break;
		play = true;

		switch (ch)
		{
			case 'c': break;
			case 'n': grid.reset(); break;
			case 'i': showInfo(); play = false; break;
#ifdef DEBUG
			case 't': grid.mkTestGrid(); break;
#endif
			default:  play = false;
		}
	}

	endwin();
	exitHandler("ended by user");
	return 0;
}

