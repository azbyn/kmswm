#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <memory>
#include <string>

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMUS_VARIABLE(str) \
	CONCATENATE(str, __COUNTER__)
#else
#define ANONYMUS_VARIABLE(str) \
	CONCATENATE(str, __LINE__)
#endif


namespace kmswm {
typedef int FD;

template<typename ... Args>
inline std::string string_format(const std::string& format, Args ... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[ size ]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

//print error msg
void verror(int code, const char* format, va_list args);
void error(int code, const char* format, ...);
void error(const char* format, ...);

//print error msg then exit with code
void panic(int code, const char* format, ...);
void panic(const char* format, ...);

suseconds_t getDeltaMicroseconds(struct timeval a, struct timeval b);

}//namespace kmswm
