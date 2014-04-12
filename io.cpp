/*
 * I/O subsystem based on curses
 *
 * Mike Szymaniak, 2014
 */

#include "io.h"
#include <signal.h>
#include <stdlib.h>


#define INPUT_TIMEOUT	5*60*1000	// in ms
#define LOG_NAME		"2k48"


static void exitHandlerStub(const char*)
{}


static bool HARDRESCUR = false;
static ExitHandler exitHandlerFunc = &exitHandlerStub;


#ifdef LOGSIG
static void signalHandler(int signo)
{
    endwin();
    if (signo == SIGHUP)
	{
		LOGS("closed by user");
		exitHandlerFunc("ended by user");
	}
	else
	{
		LOG("closed with signal %d", signo);
		exitHandlerFunc("terminated");
	}
    exit(0);
}
#endif

void initIO(ExitHandler eh)
{
	exitHandlerFunc = eh;

#if defined(LOGON) || defined(LOGSIG)
    openlog(LOG_NAME, 0, LOG_USER);
#endif

#ifdef LOGSIG
	signal(SIGINT,  signalHandler);
	signal(SIGQUIT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGHUP,  signalHandler);
#endif

	initscr();
	cbreak();
	start_color();
	curs_set(0);
    noecho();
    timeout(INPUT_TIMEOUT);
	keypad(stdscr, TRUE);

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

int getkey()
{
	int k = getch();
#ifdef DEAMON
	if (k == ERR)
	{
		LOGS("closed due to timeout");
		endwin();
		exitHandlerFunc("user inactive");
		exit(0);
	}
#endif
	return k;
}

int getlowkey()
{
	int k = getkey();
	if (k>='A' && k<='Z') k += 'a' - 'A';
	return k;
}

void getanykey()
{
	getkey();
}

void rescur()
{
	if (HARDRESCUR) clear();
	else move(0, 0);
}

bool swRescur()
{
	HARDRESCUR = !HARDRESCUR;
	return HARDRESCUR;
}

void coloron(int color)
{
	attron(COLOR_PAIR(color));
}

void coloroff(int color)
{
	attroff(COLOR_PAIR(color));
}

void boldon()
{
	attron(A_BOLD);
}

void boldoff()
{
	attroff(A_BOLD);
}

void blinkon()
{
	attron(A_BLINK);
}

void blinkoff()
{
	attroff(A_BLINK);
}

