/*
 * I/O subsystem based on curses
 *
 * Mike Szymaniak, 2014
 */

#ifndef __IO_H__
#define __IO_H__

#include <ncurses.h>
#include <cstddef>


#ifdef DEAMON

// log actions to system log
#include <syslog.h>
#define LOGON
#define LOGSIG
#define LOG(fmt, ...)	syslog(LOG_INFO, "%d: " fmt, getpid(), __VA_ARGS__)
#define LOGS(msg)		syslog(LOG_INFO, "%d: " msg, getpid())

#else

// supress logs
#define LOG(...)
#define LOGS(...)

#endif

// function to call before exit
typedef void (*ExitHandler)(const char* cause);

// init and register exit handler function
void initIO(ExitHandler eh = NULL);

int getkey();
int getlowkey();
void getanykey();

void rescur();
bool swRescur();

void coloron(int color);
void coloroff(int color);
void boldon();
void boldoff();
void blinkon();
void blinkoff();

#endif // __IO_H__

