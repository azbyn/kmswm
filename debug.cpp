#include "debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_FILE stdout

// clang-format off
#define CBLACK   "\x1b[30m"
#define CRED     "\x1b[31m"
#define CGREEN   "\x1b[32m"
#define CYELLOW  "\x1b[33m"
#define CBLUE    "\x1b[34m"
#define CMAGENTA "\x1b[35m"
#define CCYAN    "\x1b[36m"
#define CWHITE   "\x1b[37m"
#define CRESET   "\x1b[0m"
#define CBOLD    "\x1b[1m"
// clang-format off


namespace azbyn {
namespace debug {
static void vbaseprint(const char* color, const char* format, va_list args) {
    fputs(color, DEBUG_FILE);
    vfprintf(DEBUG_FILE, format, args);
    fputs(CRESET "\n", DEBUG_FILE);
}
void log(const char* format, ...) {
    va_list args;
    va_start(args, format);

    vfprintf(DEBUG_FILE, format, args);
    fputs("\n", DEBUG_FILE);
}
void warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vbaseprint(CYELLOW, format, args);
}
void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vbaseprint(CRED, format, args);
}
} // namespace debug
} // namespace azbyn
