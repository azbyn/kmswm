#pragma once

#include <atomic>
#include <string>
#include <thread>

#include "keymaps.h"

#ifndef KEY_HOLD_DELAY_MS
#define KEY_HOLD_DELAY_MS 600//660
#endif
#ifndef KEY_REPEAT_RATE
#define KEY_REPEAT_RATE 30
#endif

#define KEY_REPEAT_DELAY (KEY_REPEAT_RATE != 0 ? 1000 / KEY_REPEAT_RATE : 0)

namespace Kmswm {
class InputHandler {
public:
	InputHandler(const char *devicePath, void (*onExit)(void));
	~InputHandler();
	void ThreadInit();
	void Stop();
	void Write(uint16_t type, uint16_t code, int32_t value);
	void SetLed(uint16_t led, bool value);

private:
	KeymapStack keymapStack;
	int fd;
	int initalRepeat[2];
	std::atomic<bool> running;
	void (*onExit)(void);
	std::thread thread;
	void Update();
};
}// namespace Kmswm


