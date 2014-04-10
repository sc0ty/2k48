#ifndef __IO_H__
#define __IO_H__

#include <ncurses.h>
#include <unistd.h>


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


void initIO();

int getkey();
int getlowkey();
void getanykey();

void rescur();
bool swRescur();

void boldon();
void boldoff();

#endif // __IO_H__

