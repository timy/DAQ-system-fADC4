#pragma once

#include <thread>

// thread wrapper to implement "Resource Acquisition Is Initialization"
class ThreadWrapper {
public:
	ThreadWrapper(std::thread& thr) : m_thread(thr) {}
	virtual ~ThreadWrapper() {
		if (m_thread.joinable())
			m_thread.detach();
	}
private:
	std::thread& m_thread;
};