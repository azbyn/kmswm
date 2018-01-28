#pragma once

#include <thread>
#include <atomic>

namespace Kmswm {
class GraphicsHandler {
public:
	GraphicsHandler();
	~GraphicsHandler();
	void ThreadInit();
	void Stop();
private:
	std::atomic<bool> running;
	std::thread thread;
	void Update();
};
} // namespace Kmswm
