#pragma once

#include "error.h"

namespace kmswm {

class Result {
	bool failed;
	Error error;
public:
	Result() : failed(false), error() {}
	Result(const Error& error) : failed(true), error(error) {}
	Result(const Result& rhs) : failed(rhs.failed) {
		if (failed) new(&error) Error(rhs.error);
	}
	Result(Result&& rhs) : failed(rhs.failed) {
		if (failed) new(&error) Error(std::move(rhs.error));
	}

	Result& operator=(const Result& rhs) {
		failed = rhs.failed;
		if (failed) error = rhs.error;
		return *this;
	}

	bool HasError() const {
		return failed;
	}
	static Result Success() {
		return Result();
	}

	Error& GetError() {
		if(!failed) throw std::runtime_error("No error found");
		return error;
	}

	const Error& GetError() const {
		if(!failed) throw std::runtime_error("No error found");
		return error;
	}
	int GetErrno() const {
		if(!failed) throw std::runtime_error("No error found");
		return error.GetErrno();
	}

	void swap(Result& rhs) {
		if (failed) {
			if (rhs.failed) {
				error.swap(rhs.error);
			}
			else {
				new(&rhs.error) Error(std::move(error));
				std::swap(failed, rhs.failed);
			}
		}
		else {
			if (rhs.failed) {
				new(&error) Error(std::move(rhs.error));
				std::swap(failed, rhs.failed);
			}
		}
	}
};

}//namespace kmswm
