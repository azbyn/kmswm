#include <assert.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <thread>

#include "input_handler.h"

namespace kmswm {
InputHandler::InputHandler(const char *devicePath, void (*onExit)(void)) :
		keymapStack(KeymapStack::generate(this)),
		onExit(onExit),
		running(false) {
	fd = open(devicePath, O_RDWR);
	if (fd == -1) {
		panic("Can't open '%s'", devicePath);
	}
	char name[256];
	printf("Press a key\n");
	if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0)
		panic("EVIOCGNAME fail");
	if (ioctl(fd, EVIOCGREP, initalRepeat))
		panic("EVIOCGREP fail");

	//printf("initial repeat %d, %d\n", initalRepeat[0], initalRepeat[1]);

	int rep[2] { KEY_HOLD_DELAY_MS, KEY_REPEAT_DELAY };
	if (ioctl(fd, EVIOCSREP, rep))
		panic("EVIOCSREP fail");

	//printf("new repeat %d, %d\n", rep[0], rep[1]);

	printf("Reading from %s (%s)\n", devicePath, name);

	SetLed(LED_CAPSL, false);
	SetLed(LED_NUML, false);
}
InputHandler::InputHandler(const InputHandler& rhs) :
	keymapStack(rhs.keymapStack),
	fd(rhs.fd),
	onExit(rhs.onExit),
	running(false) {}
InputHandler::InputHandler(InputHandler&& rhs) :
	keymapStack(std::move(rhs.keymapStack)),
	fd(rhs.fd),
	onExit(rhs.onExit),
	running(false) {}


InputHandler::~InputHandler() {
	Stop();
}
void InputHandler::ThreadInit() {
	running = true;
	thread = std::thread(&InputHandler::Update, this);
}
void InputHandler::ThreadJoin() {
	thread.join();
}

void InputHandler::Write(uint16_t type, uint16_t code, int32_t value) {
	struct input_event e = {{}, type, code, value };
	write(fd, &e, sizeof(struct input_event));
}
void InputHandler::SetLed(uint16_t led, bool value) {
	Write(EV_LED, led, value);
}

void InputHandler::Stop() {
	if (!running)
		return;
	running = false;
	if (ioctl(fd, EVIOCSREP, initalRepeat))
		panic("EVIOCSREP fail");

	this->onExit();
	close(fd);
}

void InputHandler::Update() {
	struct input_event e;

	const int size = sizeof(e);
	while (running) {
		if (read(fd, &e, size) != size)
			panic("read()");
		//printf("t=%d v=%d c=%d\n", e.type, e.value, e.code);
		if (e.type != EV_KEY)
			continue;
		assert(e.code < KEYMAP_SIZE);
		keymapStack.HandleKey(e.code, e.value);
		fflush(stdout);
	}
}

}// namespace Kmswm
