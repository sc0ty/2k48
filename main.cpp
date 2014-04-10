#include "io.h"
#include "2048.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>


#define SAVE_FILE	"2k48.save"
#define DELAY		50*1000		// in us


void init(int argc, char *argv[])
{
	initIO();
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

	init_pair(1,  COLOR_BLACK,	COLOR_YELLOW);	// 2
	init_pair(2,  COLOR_BLACK,	COLOR_GREEN);	// 4
	init_pair(3,  COLOR_BLACK,	COLOR_CYAN);	// 8
	init_pair(4,  COLOR_BLACK,	COLOR_MAGENTA);	// 16
	init_pair(5,  COLOR_WHITE,	COLOR_BLUE);	// 32
	init_pair(6,  COLOR_WHITE,	COLOR_RED); 	// 64
	init_pair(7,  COLOR_BLACK,	COLOR_WHITE);	// 128
	init_pair(8,  COLOR_BLACK,	COLOR_YELLOW);	// 256
	init_pair(9,  COLOR_BLACK,	COLOR_GREEN);	// 512
	init_pair(10, COLOR_BLACK,	COLOR_CYAN);	// 1024
	init_pair(11, COLOR_BLACK,	COLOR_MAGENTA);	// 2048
	init_pair(12, COLOR_WHITE,	COLOR_BLUE);	// 4k
	init_pair(13, COLOR_WHITE,	COLOR_RED);		// 8k
	init_pair(14, COLOR_BLACK,	COLOR_WHITE);	// header
	init_pair(15, COLOR_WHITE,	COLOR_RED); 	// popup
}


int getArrowKey()
{
	int k = getlowkey();
	switch (k)
	{
		case 'w': return KEY_UP;	break;
		case 's': return KEY_DOWN;	break;
		case 'a': return KEY_LEFT;	break;
		case 'd': return KEY_RIGHT;	break;
		case 'q': return KEY_EXIT;	break;
	}
	return k;
}


void showPopup(const char* msg)
{
	addstr("\n\n   ");
	attron(A_BLINK);
	attron(COLOR_PAIR(15));
	printw("%s", msg);
	attroff(COLOR_PAIR(15));
	attroff(A_BLINK);
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

	attron(COLOR_PAIR(14));
	addstr("        2k48  by Mike Szymaniak - informations        ");
	attroff(COLOR_PAIR(14));

	addstr("\n\n"
			"     Terminal version of 2048 game - http://git.io/2048\n"
#ifdef DEAMON
			"     Please do not try to hack my machine!\n"
#endif
			"\n");

	FMTENT("author:", "Mike 'sc0ty' Szymaniak");
	FMTENT("email:", "sc0typl[at]gmail.com");
	FMTENT("webpage:", "http://sc0ty.pl");
	
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
	addstr(" to " cmd "\n");

	clear();
	addstr("\n   ");
	attron(COLOR_PAIR(14));
	addstr(" 2k48  by Mike Szymaniak ");
	attroff(COLOR_PAIR(14));
	addstr("\n\n   Press:\n");
	MENUENT("C", "continue")
	MENUENT("N", "new game")
	MENUENT("I", "show info")
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


void game(Grid& grid)
{
	int key = 0;
	static int info = 5;

	clear();
	if (!grid.canMove())
	{
		grid.show();
		getanykey();
		return;
	}

	if (grid.getMaxTile() == 0) grid.genBlock();
	bool canwin = grid.getMaxTile() < 2048;

	while (true)
	{
		rescur();
		grid.show();
		if (info-- > 0) printw("\n\n if you have display problems - press R\n");

		if (canwin && grid.getMaxTile() >= 2048)
		{
			usleep(DELAY);
			LOG("win with %d moves", grid.getMoves());
			showPopup("        You win!         ");
			return;
		}
		
		if (!grid.canMove())
		{
			usleep(DELAY);
			LOG( "lose with %d moves, score %d", grid.getMoves(), grid.getMaxTile());
			showPopup("        Game over!       ");
			return;
		}

		do
		{
			key = getArrowKey();
			if (key == KEY_EXIT) return;
			else if (key == 'r')
			{
				bool rp = swRescur();
				LOG("render mode %d", rp);
				clear();
				grid.show();
				printw("\n\n   Repaint mode %s", rp ? "ON" : "OFF");
				info = 0;
			}
		}
		while (!grid.shift(key, DELAY));

		usleep(DELAY);
		grid.genBlock();
		if (info == 0) clear();
	}
}


int main(int argc, char *argv[])
{
	init(argc, argv);

    Grid grid;
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
		if (play) game(grid);

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

#ifndef DEAMON
	grid.save(SAVE_FILE);
#endif

	LOGS("closed normally");

	endwin();
	return 0;
}

