#pragma once
#include "misc.h"

#include <errno.h>
#include <string.h>

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

namespace azbyn {

class Error {
private:
    int errno_;
    std::string msg;

public:
    Error() = default; // only used for class Result
    Error(int errno_, const char* msg) : errno_(errno_), msg(msg) {}
    Error(const char* msg) : Error(errno, msg) {}

    template <class... Args>
    Error(int errno_, const char* format, Args... args)
            : errno_(errno_), msg(string_format(format, args...)) {}
    template <class... Args>
    Error(const char* format, Args... args) : Error(errno, format, args...) {}

    Error(const Error& rhs) : errno_(rhs.errno_), msg(rhs.msg) {}
    Error(Error&& rhs) : errno_(rhs.errno_), msg(std::move(rhs.msg)) {}

    template <class... Args>
    Error(const Error& org, const char* format, Args... args)
            : errno_(org.errno_) {
        msg = org.msg + "&&\n" + string_format(format, args...);
    }
    Error& operator=(const Error& rhs) {
        msg = rhs.msg;
        errno_ = rhs.errno_;
        return *this;
    }

    inline int GetErrno() const { return errno_; }
    inline const char* GetRawMsg() const { return msg.c_str(); }
    inline std::string GetMsg() const { return msg; }
    inline void Print() const {
        if (errno_)
            fprintf(stderr, "%s (%d): %s\n", msg.c_str(), errno_ ? errno_ : -1,
                    strerror(errno_));
        else
            fprintf(stderr, "%s\n", msg.c_str());
    }
    [[noreturn]] inline void Throw() const {
        Print();
        exit(errno_ ? errno_ : -1);
    }
    void swap(Error& rhs) {
        using std::swap;
        swap(msg, rhs.msg);
        swap(errno_, rhs.errno_);
    }
};

} // namespace azbyn
