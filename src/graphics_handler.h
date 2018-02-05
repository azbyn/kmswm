#pragma once
#include <xf86drm.h>
#include <xf86drmMode.h>
/*
#include <gbm.h>

#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
*/

#include <thread>
#include <atomic>
#include <vector>
#include <memory>

#include "expected.h"
#include "result.h"
#include "misc.h"
#include "color.h"

namespace kmswm {

struct ModesetBuf {
	uint32_t stride;
	uint32_t size;
	uint32_t handle;
	Color *map;
	uint32_t fb;
	//ModesetBuf() = default;
	Result CreateFb(FD fd, uint32_t width, uint32_t height);
	void DestroyFb(FD fd);
};

struct ModesetDev {
	//ModesetDev(FD fd, drmModeRes *res, drmModeConnector *drmConn);
	uint32_t width;
	uint32_t height;

	unsigned int frontBuf = 0;
	ModesetBuf bufs[2];

	drmModeModeInfo mode;
	uint32_t connId;
	uint32_t crtcId;
	drmModeCrtc *savedCrtc;


	static Expected<ModesetDev> create(const std::vector<ModesetDev>& modesets,
	                                   FD fd, const drmModeRes *res, const drmModeConnector *drmConn);
	ModesetDev(uint32_t connId);
	~ModesetDev();

	Result SetupDev(const std::vector<ModesetDev>& modesets, FD fd, const drmModeRes *res, const drmModeConnector *conn);
	Result FindCrtc(const std::vector<ModesetDev>& modesets, FD fd, const drmModeRes *res, const drmModeConnector *conn);

	//ModesetBuf& GetFrontBuff();
	//ModesetBuf& GetBackBuff();
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

	static Expected<GraphicsHandler *> create(const char *device);
};

} // namespace kmswm
