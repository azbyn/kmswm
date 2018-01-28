#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>

#include "graphics_handler.h"
#include "input_handler.h"
#include "misc.h"

const char *DEVICE = "/dev/input/event1";

using namespace std;
using namespace Kmswm;

GraphicsHandler *graphics;

int main(int argc, char *argv[]) {
	struct termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	const char *device = DEVICE;
	if (argc > 1)
		device = argv[1];
	//graphics = new GraphicsHandler();
	InputHandler input(device, [](void) {
	//		graphics->Stop();
		});

	input.ThreadInit();
	//graphics->ThreadInit();
	
	getchar(); //if we don't do this all input will be sent to shell
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	delete graphics;
	return 0;
}


