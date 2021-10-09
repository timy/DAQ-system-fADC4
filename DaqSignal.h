#pragma once

// #include <atomic>
class DaqSignal {
public:
	DaqSignal() : bRun(false), bConfig(true) {}
	virtual ~DaqSignal() {}

	bool bRun;
	bool bConfig;
};