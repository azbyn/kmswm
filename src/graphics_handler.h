#pragma once
#include <xf86drm.h>
#include <xf86drmMode.h>

#include <thread>
#include <atomic>
#include <vector>

#include "expected.h"
#include "result.h"
#include "misc.h"

namespace kmswm {
class ModesetDev {
	//ModesetDev(FD fd, drmModeRes *res, drmModeConnector *drmConn);
public:
	uint32_t width;
	uint32_t height;
	uint32_t stride;
	uint32_t size;
	uint32_t handle;
	uint8_t *map;

	drmModeModeInfo mode;
	uint32_t fb;
	uint32_t connId;
	uint32_t crtc;
	drmModeCrtc *savedCrtc;

	static Expected<ModesetDev> create(const std::vector<ModesetDev>& modesets,
	                                   int i, FD fd, drmModeRes *res, drmModeConnector *drmConn);
	ModesetDev(uint32_t connId);
	~ModesetDev();

	Result SetupDev(const std::vector<ModesetDev>& modesets, FD fd, drmModeRes *res, drmModeConnector *conn);
	Result FindCrtc(const std::vector<ModesetDev>& modesets, FD fd, drmModeRes *res, drmModeConnector *conn);
	Result CreateFb(FD fd);
};

class GraphicsHandler {
private:
	std::atomic<bool> running;
	std::thread thread;
	FD fd;
	std::vector<ModesetDev> modesets;

	GraphicsHandler() = default;
public:
	GraphicsHandler(const GraphicsHandler& rhs);
	GraphicsHandler(GraphicsHandler&& rhs);
	~GraphicsHandler();
	void ThreadInit();
	void ThreadJoin();
	void Stop();
	void Update();

	Result Main(const char *card);
	Expected<FD> Open(const char *node);
	Result Prepare();
	void Draw();
	void Cleanup();

	static Expected<GraphicsHandler> create(const char *device);
};

} // namespace kmswm
