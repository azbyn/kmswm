#pragma once
#include <errno.h>
#include <string.h>

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#include "misc.h"

namespace kmswm {

class Error {
private:
	int _errno;
	std::string msg;
public:
	Error() = default; //only used for class Result
	Error(int _errno, const char *msg) : _errno(_errno), msg(msg) {}
	Error(const char *msg) : Error(errno, msg) {}

	template<class ... Args>
	Error(int _errno, const char *format, Args... args) :
		_errno(_errno),
		msg(string_format(format, args...)) {}
	template<class ... Args>
	Error(const char *format, Args... args) : Error(errno, format, args...) {}

	Error(const Error& rhs) : _errno(rhs._errno), msg(rhs.msg) {}
	Error(Error&& rhs) : _errno(rhs._errno), msg(std::move(rhs.msg)) {}

	template<class ... Args>
	Error(const Error& org, const char *format, Args... args) : _errno(org._errno) {
		msg = org.msg + "&&\n" + string_format(format, args ...);
	}
	Error& operator=(const Error& rhs) {
		msg = rhs.msg;
		_errno = rhs._errno;
		return *this;
	}



	inline int GetErrno() const {
		return _errno;
	}
	inline const char *GetRawMsg() const {
		return msg.c_str();
	}
	inline std::string GetMsg() const {
		return msg;
	}
	inline void Print() const {
		if (_errno)
			fprintf(stderr, "%s (%d): %s\n",
			        msg.c_str(), _errno ? _errno : -1, strerror(_errno));
		else fprintf(stderr, "%s\n", msg.c_str());
	}
	inline void Throw() const {
		Print();
		exit(_errno ? _errno : -1);
	}
	void swap(Error& rhs) {
		using std::swap;
		swap(msg, rhs.msg);
		swap(_errno, rhs._errno);
	}
};


} //namespace kmswm

