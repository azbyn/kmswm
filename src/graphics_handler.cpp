#include <stdio.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include "graphics_handler.h"

namespace Kmswm {
GraphicsHandler::GraphicsHandler() {
	running = true;
	thread = std::thread(&GraphicsHandler::Update, this);
}
GraphicsHandler::~GraphicsHandler() {
	Stop();
}
void GraphicsHandler::ThreadInit() {
	thread.join();
}
void GraphicsHandler::Stop() {
	running = false;
}
void GraphicsHandler::Update() {
	while (running) {
		printf("a");
	}
}

} // namespace Kmswm

