#pragma once
#include "error.h"

#include <algorithm>
#include <exception>
#include <stdexcept>

namespace azbyn {

template <class T>
class Expected {
private:
    union {
        T value;
        Error error;
    };
    bool hasValue;
    // Expected() : Expected(false, Error()) {}
public:
    Expected(const T& rhs) : value(rhs), hasValue(true) {}

    Expected(T&& rhs) : value(std::move(rhs)), hasValue(true) {}

    Expected(const Expected& rhs) : hasValue(rhs.hasValue) {
        if (hasValue)
            new (&value) T(rhs.value);
        else
            new (&error) Error(rhs.error);
    }
    Expected(Expected&& rhs) : hasValue(rhs.hasValue) {
        if (hasValue)
            new (&value) T(std::move(rhs.value));
        else
            new (&error) Error(std::move(rhs.error));
    }

    Expected(const Error& error) : error(error), hasValue(false) {}

    bool Valid() const { return hasValue; }
    bool HasError() const { return !hasValue; }
    T& Get() {
        if (!hasValue)
            error.Throw();
        return value;
    }
    const T& Get() const {
        if (!hasValue)
            error.Throw();
        return value;
    }
    Error& GetError() {
        if (hasValue)
            throw std::runtime_error("No error found");
        return error;
    }

    const Error& GetError() const {
        if (hasValue)
            throw std::runtime_error("No error found");
        return error;
    }
    ~Expected() {}

    void swap(Expected& rhs) {
        if (hasValue) {
            if (rhs.hasValue) {
                using std::swap;
                swap(value, rhs.value);
            }
            else {
                auto t = std::move(rhs.error);
                new (&rhs.value) T(std::move(value));
                new (&error) Error(t);
                std::swap(hasValue, rhs.hasValue);
            }
        }
        else {
            if (rhs.hasValue) {
                rhs.swap(*this);
            }
            else {
                error.swap(rhs.error);
                std::swap(hasValue, rhs.hasValue);
            }
        }
    }
};

} // namespace twgn
