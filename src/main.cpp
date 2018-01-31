#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <exception>

#include "graphics_handler.h"
#include "input_handler.h"
#include "misc.h"

const char *DEVICE = "/dev/input/event1";

using namespace std;
using namespace Kmswm;

struct termios old_termios;

GraphicsHandler *graphics;
InputHandler *input;

void restore_state() {
	if (input) input->Stop();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
	delete graphics;
	delete input;
	printf("<on_exit>");
}
bool handled = false;
void handle_exception() {
	if (handled) return;
	try {
		throw;
	}
	catch (std::exception& e) {
		handled = true;
		fprintf(stderr, "\nerror:\n\t%s\n", e.what());
		restore_state();
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	tcgetattr(STDIN_FILENO, &old_termios);
	struct termios newt = old_termios;
	atexit(restore_state);
	std::set_terminate(handle_exception);
	
	newt.c_iflag &= ~(IXON);
	newt.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	const char *device = DEVICE;
	if (argc > 1)
		device = argv[1];
	//graphics = new GraphicsHandler();
	input = new InputHandler(device, [] {
	//		graphics->Stop();
		});

	input->ThreadInit();
	//graphics->ThreadInit();
		
	getchar(); //if we don't do this all input will be sent to shell
	return 0;
}


