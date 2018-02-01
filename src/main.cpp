#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <exception>

#include "graphics_handler.h"
#include "input_handler.h"
#include "misc.h"

using namespace kmswm;

struct termios oldTermios;
GraphicsHandler *graphics;
InputHandler *input;

void restoreState() {
	if (input)
		input->Stop();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
	delete graphics;
	delete input;
	printf("<on_exit>");
}
bool handled = false;
void handleException() {
	if (handled) return;
	try {
		throw;
	}
	catch (std::exception& e) {
		handled = true;
		fprintf(stderr, "\nerror:\n\t%s\n", e.what());
		restoreState();
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	tcgetattr(STDIN_FILENO, &oldTermios);
	struct termios newTermios = oldTermios;
	atexit(restoreState);
	std::set_terminate(handleException);

	newTermios.c_iflag &= ~(IXON);
	newTermios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

	const char *inputDev = "/dev/input/event1";
	const char *graphicsDev = "/dev/dri/card0";
	if (argc > 1)
		inputDev = argv[1];
	if (argc > 2)
		graphicsDev = argv[2];

	graphics = new GraphicsHandler(graphicsDev);
	input = new InputHandler(inputDev, [] {
			graphics->Stop();
		});

	input->ThreadInit();
	graphics->ThreadInit();
		
	getchar(); //if we don't do this all input will be sent to shell
	return 0;
}

