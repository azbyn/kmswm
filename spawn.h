#pragma once

#include "error.h"
#include "scope_guard.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace azbyn {

template <class... Args>
void spawn(const char* file, Args... args) {
    pid_t pid = fork();
    if (pid < 0) {
        Error("fork failed").Throw();
    }
    else if (pid == 0) {
        execlp(file, file, args..., nullptr);
        Error("exec failed").Throw();
    }
}
inline void _spawnThread(pid_t pid,
                         int fd,
                         std::function<void(std::string, int)> callback) {
    int status;
    FILE* fp = fdopen(fd, "r");
    if (!fp)
        Error("fdopen").Throw();

    SCOPE_EXIT({ fclose(fp); });
    waitpid(pid, &status, 0);

    const size_t LEN = 1024;
    char buf[LEN];
    std::string result;
    while (fgets(buf, LEN, fp)) {
        result += buf;
    }
    callback(result, status);
}
template <typename... Args>
void spawnCallback(std::function<void(std::string, int)> callback,
                   const char* file,
                   Args... args) {
    FD fd[2];
    FD read_fd, write_fd;
    if (pipe(fd) < 0) {
        Error("pipe error").Throw();
    }
    read_fd = fd[0];
    write_fd = fd[1];

    pid_t pid = fork();
    if (pid < 0) {
        Error("fork failed").Throw();
    }
    else if (pid == 0) {
        close(read_fd);
        dup2(write_fd, 1);
        close(write_fd);
        execlp(file, file, args..., nullptr);
        Error("exec failed").Throw();
    }
    else {
        close(write_fd);
        std::thread t(&_spawnThread, pid, read_fd, callback);
        t.detach();
    }
}

} // namespace azbyn
