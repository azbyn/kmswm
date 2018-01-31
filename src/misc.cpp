#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>

#include <iostream>
#include <memory>

#include "misc.h"

namespace Kmswm {
void Panic(int code, const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	fputs(": ", stderr);
	perror(nullptr);
	exit(code);
}
suseconds_t GetDeltaMicroseconds(struct timeval t0, struct timeval t1) {
	suseconds_t deltaUS = t1.tv_usec - t0.tv_usec;
	time_t deltaS  = t1.tv_sec - t0.tv_sec;
	
	return (deltaS * 1000000) + deltaUS;
}
}//namespace Kmswm
/*
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[ size ]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
}
*/
