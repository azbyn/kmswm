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
	void Write(uint16_t type, uint16_t code, int32_t value);

private:
	KeymapStack keymapStack;
	int fd;
	std::atomic<bool> running;
	void (*onExit) (void);
	std::thread thread;
	void Update();

	KeymapStack GenenerateKeymapStack();

};
}// namespace Kmswm


