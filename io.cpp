#include "io.h"
#include <signal.h>
#include <stdlib.h>


#define INPUT_TIMEOUT	5*60*1000	// in ms
#define LOG_NAME		"2k48"


static bool HARDRESCUR = false;


#ifdef LOGSIG
static void signalHandler(int signo)
{
    LOG("ended with signal %d", signo);
    endwin();
    exit(0);
}
#endif


void initIO()
{
#if defined(LOGON) || defined(LOGSIG)
    openlog(LOG_NAME, 0, LOG_USER);
#endif

#ifdef LOGSIG
	signal(SIGINT,  signalHandler);
	signal(SIGQUIT, signalHandler);
	signal(SIGTERM, signalHandler);
#endif

	initscr();
	cbreak();
	start_color();
	curs_set(0);
    noecho();
    timeout(INPUT_TIMEOUT);
	keypad(stdscr, TRUE);
}


int getkey()
{
	int k = getch();
#ifdef DEAMON
	if (k == ERR)
	{
		LOGS("closed due to timeout");
		endwin();
		puts("User inactive - closing");
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


void boldon()
{
	attron(A_BOLD);
}


void boldoff()
{
	attroff(A_BOLD);
}

