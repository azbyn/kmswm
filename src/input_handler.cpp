#include <assert.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <thread>

#include "misc.h"

#include "input_handler.h"

#define KEYMAP_SIZE  256
#define KEY_VAL_UP   0
#define KEY_VAL_DOWN 1
#define KEY_VAL_HOLD 2

namespace Kmswm {
InputHandler::InputHandler(const char *devicePath, void (*onExit) (void)) :
		keymapStack(GenenerateKeymapStack()) {
	//printf("l\n");
	//keymapStack = GenenerateKeymapStack();
	((keymapStack.Top())[KEY_ESC])(nullptr);


	running = true;
	this->onExit = onExit;
	this->fd = open(devicePath, O_RDONLY);
	if (fd == -1) {
		panic(1, "Can't open '%s'", devicePath);
	}
	char name[256];
	printf("Press a key\n");
	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Reading from %s (%s)\n", devicePath, name);
	
	//this->Update();
	thread = std::thread(&InputHandler::Update, this);
}
InputHandler::~InputHandler() {
	close(fd);
	Stop();
}
void InputHandler::ThreadInit() {
	thread.join();
}
void InputHandler::Stop() {
	running = false;
}

void InputHandler::Update() {
	struct input_event e;
	const int size = sizeof(e);
	while (running) {
		if (read(fd, &e, size) != size)
			Kmswm::panic(1, "read()");
		if (e.type != EV_KEY)
			continue;
		assert(e.code < KEYMAP_SIZE);
		std::string msg;
		switch (e.value) {
		case KEY_VAL_UP:
			msg = "up";
			if (e.code == KEY_ESC) {
				this->onExit();
				return;
			}
			break;
		case KEY_VAL_DOWN:
			msg = "down";
			break;
		case KEY_VAL_HOLD:
			continue;
		}
		printf("%s c=%d:", msg.c_str(), e.code);
		keymapStack.PressKey(e.code);
		puts("\n");
	}
}

} // namespace Kmswm
