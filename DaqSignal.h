#pragma once

// #include <atomic>
class DaqSignal {
public:
	DaqSignal() : 
		bRun(false), 
		bConfig(true), 
		bPlot(false), 
		bConnect(false), 
		bSend(false) {}

	virtual ~DaqSignal() {}

	bool bRun;
	bool bConfig;
	bool bPlot;
	bool bConnect;
	bool bSend;
};