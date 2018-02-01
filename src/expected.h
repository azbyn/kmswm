#pragma once
#include <algorithm>
#include <exception>
#include <stdexcept>

namespace kmswm {

template<class T>
class Expected {
private:
	union {
		T value;
		std::exception_ptr error;
	};
	bool hasValue;
	Expected() {}
public:
	Expected(const T& rhs) :
			value(rhs), hasValue(true) {}
	Expected(T&& rhs) :
			value(std::move(rhs)), hasValue(true) {}
	Expected(const Expected& rhs) : hasValue(rhs.hasValue) {
		if (hasValue) new(&value) T(rhs.value);
		else new(&error) std::exception_ptr(rhs.error);
	}
	Expected(Expected&& rhs) : hasValue(rhs.hasValue) {
		if (hasValue) new(&value) T(std::move(rhs.value));
		else new(&error) std::exception_ptr(std::move(rhs.error));
	}
	
	template<class E>
	Expected(const E& exception) {
		if (typeid(exception) != typeid(E)) {
			throw std::invalid_argument("slicing detected");
		}

	static_assert(std::is_base_of<std::exception, T>::value,
			"E must be a exception");
		new(this) Expected(std::make_exception_ptr(exception));
	}
	Expected(std::exception_ptr p) :
			hasValue(false) {
		new(&error) std::exception_ptr(std::move(p));
	}

	bool Valid() const {
		return hasValue;
	}
	T& Get() {
		if (!hasValue) std::rethrow_exception(error);
		return value;
	}
	const T& Get() const {
		if (!hasValue) std::rethrow_exception(error);
		return value;
	}

	template<class E>
	bool HasException() const {
		try {
			if (!hasValue) std::rethrow_exception(error);
		}
		catch (const E&) {
			return true;
		}
		catch (...) {
		}
		return false;
	}

	void swap(Expected& rhs) {
		if (hasValue) {
			if (rhs.hasValue) {
				using std::swap;
				swap(value, rhs.value);
			}
			else {
				auto t = std::move(rhs.error);
				new(&rhs.value) T(std::move(value));
				new(&error) std::exception_ptr(t);
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
	static Expected<T> fromException() {
		return Expected(std::current_exception());
	}
	template<class F>
	static Expected fromCode(F fun) {
		try {
			return Expected(fun());
		}
		catch (...) {
			return fromException();
		}
	}
};

}//namespace kmswm
