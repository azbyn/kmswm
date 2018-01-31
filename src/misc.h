#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <functional>


namespace Kmswm {
template<typename ... Args>
inline std::string string_format(const std::string& format, Args ... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[ size ]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

void Panic(int code, const char* format, ...);

template<typename ... Args>
void Spawn(const char* file, Args ... args) {
	pid_t pid = fork();
	if (pid < 0) {
		Panic(2, "fork failed");
	}
	else if (pid == 0) {
		execlp(file, file, args ..., (char*)NULL);
		Panic(-1, "exec failed");
	}
}
inline void _SpawnThread(pid_t pid, int fd, std::function<void(std::string, int)> callback) {
	int status;
	FILE *fp = fdopen(fd, "r");
	
	waitpid(pid, &status, 0);

	const size_t len = 1024;
	char buf[len];
	std::string result;
	while (fgets(buf, len, fp)) {
		result += buf;
	}
	callback(result, status);
	fclose(fp);

}
template<typename ... Args>
void SpawnCallback(std::function<void(std::string, int)> callback, const char* file, Args ... args) {
	int fd[2];
	int read_fd, write_fd;
	if (pipe(fd) < 0) {
		Panic(4, "pipe error");
	}
	read_fd = fd[0];
	write_fd = fd[1];

	pid_t pid = fork();
	if (pid < 0) {
		Panic(2, "fork failed");
	}
	else if (pid == 0) {
		close(read_fd);
		dup2(write_fd, 1);
		close(write_fd);
		execlp(file, file, args ..., (char*)NULL);
		Panic(-1, "exec failed");
	}
	else {
		close(write_fd);
		std::thread t(&_SpawnThread, pid, read_fd, callback);
		t.detach();
	}
}

suseconds_t GetDeltaMicroseconds(struct timeval a, struct timeval b);
}//namespace Kmswm
