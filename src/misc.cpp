#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>

#include <iostream>
#include <memory>

#include "misc.h"

#define COL_BLACK   "\x1b[30m"
#define COL_RED     "\x1b[31m"
#define COL_GREEN   "\x1b[32m"
#define COL_YELLOW  "\x1b[33m"
#define COL_BLUE    "\x1b[34m"
#define COL_MAGENTA "\x1b[35m"
#define COL_CYAN    "\x1b[36m"
#define COL_WHITE   "\x1b[37m"
#define COL_RESET   "\x1b[0m"
#define COL_BOLD    "\x1b[1m"

namespace kmswm {
void verror(int code, const char* format, va_list args) {
	fprintf(stderr, COL_RED);
	vfprintf(stderr, format, args);
	fprintf(stderr, " (%d)", code);
	if (!errno) {
		fprintf(stderr, COL_RESET "\n");
		exit(code);
	}
	//fputs(": ", stderr);
	perror(COL_YELLOW);//nullptr);
	fprintf(stderr, COL_RESET);
}

void error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	verror(-1, format, args);
}
void error(int code, const char* format, ...) {
	va_list args;
	va_start(args, format);
	verror(code, format, args);
}

void panic(int code, const char* format, ...) {
	va_list args;
	va_start(args, format);
	verror(code, format, args);
	exit(code);
}
void panic(const char* format, ...) {
	va_list args;
	va_start(args, format);
	verror(-1, format, args);
	exit(-1);
}

suseconds_t getDeltaMicroseconds(struct timeval t0, struct timeval t1) {
	suseconds_t deltaUS = t1.tv_usec - t0.tv_usec;
	time_t deltaS  = t1.tv_sec - t0.tv_sec;

	return (deltaS * 1000000) + deltaUS;
}

}//namespace kmswm

