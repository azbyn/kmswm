#pragma once
#include <xf86drm.h>
#include <xf86drmMode.h>

#include <thread>
#include <atomic>
#include <vector>

#include "expected.h"
#include "misc.h"

namespace kmswm {
class GraphicsHandler {
private:
	struct ModesetDev {
		uint32_t width;
		uint32_t height;
		uint32_t stride;
		uint32_t size;
		uint32_t handle;
		uint8_t *map;

		drmModeModeInfo mode;
		uint32_t fb;
		uint32_t conn;
		uint32_t crtc;
		drmModeCrtc *savedCrtc;
	};
	std::vector<ModesetDev> modesets;

	std::atomic<bool> running;
	std::thread thread;
	FD Open(const char *node);
	void Prepare(FD fd);
	int SetupDev(FD fd, drmModeRes *res, drmModeConnector *conn);

	void Update();

public:
	GraphicsHandler(const char *device);
	~GraphicsHandler();
	void ThreadInit();
	void Stop();
};
} // namespace kmswm
