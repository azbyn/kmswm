#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <exception>
#include <memory>

#include "graphics_handler.h"
#include "input_handler.h"
#include "misc.h"

using namespace kmswm;

bool handledException = false;
struct termios oldTermios;
GraphicsHandler *graphics = nullptr;
InputHandler *input = nullptr;

void start(const char *inputDev, const char *graphicsDev) {
	tcgetattr(STDIN_FILENO, &oldTermios);
	struct termios newTermios = oldTermios;
	newTermios.c_iflag &= ~(IXON);
	newTermios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

	graphics = GraphicsHandler::create(graphicsDev).Get();
	//input = new InputHandler(inputDev, [] {
	//	graphics->Stop();
	//});

	//input->ThreadInit();
	graphics->ThreadInit();

	//input->ThreadJoin();
	graphics->ThreadJoin();
}
void stop() {
	//if (input)
	//	input->Stop();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
	delete graphics;
	graphics = nullptr;
	//delete input;
	printf("<on_exit>\n");
	//getchar(); //if we don't do this all input will be sent to shell
}
void handleException() {
	if (handledException) return;
	try {
		throw;
	}
	catch (std::exception& e) {
		handledException = true;
		fprintf(stderr, "\nerror:\n\t%s\n", e.what());
		stop();
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	atexit(stop);
	std::set_terminate(handleException);

	const char *inputDev = "/dev/input/event1";
	const char *graphicsDev = "/dev/dri/card0";
	if (argc > 1)
		inputDev = argv[1];
	if (argc > 2)
		graphicsDev = argv[2];
	start(inputDev, graphicsDev);
	return 0;
}

