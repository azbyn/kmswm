#pragma once

#include <atomic>
#include <string>
#include <thread>

#include "keymaps.h"

namespace Kmswm {
class InputHandler {
public:
	InputHandler(const char *devicePath, void (*onExit) (void));
	~InputHandler();
	void ThreadInit();
	void Stop();
private:
	KeymapStack keymapStack;
	int fd;
	std::atomic<bool> running;
	void (*onExit) (void);
	std::thread thread;
	void Update();

	static KeymapStack GenenerateKeymapStack();

};
}// namespace Kmswm


