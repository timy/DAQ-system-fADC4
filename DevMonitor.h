#pragma once

#include <thread>
#include <chrono>
#include "DevMonitorDataBaseType.h"

class BaseDevice;
class DaqSignal;

class MainWindow;

class DevMonitor {
public:
	DevMonitor() : source(nullptr) {}
	~DevMonitor() {}
	
	void setSignal(DaqSignal* sig) {
		signal = sig;
	}

	void setUpdateFunc(void (*funcUpdate_)(MainWindow*, DevMonitorDataBaseType*)) {
		updateGuiInfo = funcUpdate_;
	}

	void setDataSource(DevMonitorDataBaseType* dataSrc) {
		source = dataSrc;
	}

	void setDataDisplay(MainWindow* wnd) {
		display = wnd;
	}

	void start() {
		while (signal->bRun) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			if (source != nullptr)
				updateGuiInfo(display, source);

		}
	}

private:
	DaqSignal* signal;
	DevMonitorDataBaseType* source;
	MainWindow* display;
	void (*updateGuiInfo)(MainWindow*, DevMonitorDataBaseType*);
};