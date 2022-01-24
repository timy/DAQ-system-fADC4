#pragma once

#include "BaseDevice.h"
#include "DaqSignal.h"

// #include <thread>
// #include <chrono> // NOTE!!! remove this when debug is done

class DataProducer {
public:
	DataProducer() : device(nullptr) { // : countPackets(0) {

	}
	virtual ~DataProducer() {

	}
	void setDevice(BaseDevice* dev) {
		device = dev;
	}
	void setSignal(DaqSignal* sig) {
		signal = sig;
	}

	void start() { // 将由 DAQSystem 启动额外的线程而被调用

		device->startAcquisition(); // crono_sync_start_capture()
		// flag signal->bRun is controlled by DAQSystem::start()/stop() from the main thread
		device->capture(&signal->bRun);
		cout << "signal \"quit\" received @ DataProducer, invoke stopAcquisition()" << endl;
		device->stopAcquisition(); // crono_sync_stop_capture()

	}

private:
	BaseDevice* device;
	DaqSignal* signal;
	// ndigo_packet packetBuffer[nElemOfRingBuffer];
	// size_t countPackets;

	
};