#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include <memory>

#include "scope_guard.h"
#include "graphics_handler.h"

namespace kmswm {
GraphicsHandler::GraphicsHandler(const char *device) {
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

FD GraphicsHandler::Open(const char *node) {

	FD fd = open(node, O_RDWR | O_CLOEXEC);

	if (fd < 0) {
		panic("cannot open '%s'", node);
	}

	uint64_t has_dumb;
	if (drmGetCap(fd, DRM_CAP_DUMB_BUFFER, &has_dumb) < 0 || !has_dumb) {
		close(fd);
		panic(EOPNOTSUPP, "drm device '%s' does not support dumb buffers\n",
		      node);
	}
	return fd;
}
void GraphicsHandler::Prepare(FD fd) {
	int ret;

	/* retrieve resources */
	drmModeRes *res = drmModeGetResources(fd);
	SCOPE_EXIT({ drmModeFreeResources(res); });

	if (!res)
		panic("cannot retrieve DRM resources ");

	/* iterate all connectors */
	for (auto i = 0; i < res->count_connectors; ++i) {
		/* get information for each connector */
		drmModeConnector *conn = drmModeGetConnector(fd, res->connectors[i]);
		SCOPE_EXIT({ drmModeFreeConnector(conn); });
		if (!conn) {
			error("cannot retrieve DRM connector %u:%u",
			      i, res->connectors[i]);
			continue;
		}

		/* create a device structure */
		ModesetDev dev;
		dev.conn = conn->connector_id;

		/* call helper function to prepare this connector */
		ret = modeset_setup_dev(fd, res, conn, dev);
		if (ret) {
			if (ret != -ENOENT) {
				error("cannot setup device for connector %u:%u",
				      i, res->connectors[i]);
			}
			continue;
		}

		modesets.push_back(dev);
	}
}
void GraphicsHandler::Update() {

}

} // namespace kmswm

