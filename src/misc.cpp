#include <memory>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include "misc.h"



namespace Kmswm {
void panic(int code, const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	perror("");
	exit(code);
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
