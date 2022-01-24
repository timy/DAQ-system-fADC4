#pragma once

class DataProcessor;
class DeviceParamsBase;
class DaqStatusBaseType;
#include <iostream>
class BaseDevice {
public:
	BaseDevice() : nDevices(0), bOpened(false), dataProcessor(nullptr), status(nullptr) {}
	virtual void initialize(DeviceParamsBase* params, int* err) = 0;
	virtual void finalize() = 0;
	virtual bool isOpened() { return bOpened; }
	virtual void startAcquisition() {}
	virtual void pauseAcquisition() {}
	virtual void stopAcquisition() {}
	virtual void capture(bool* bRun) {
		std::cout << "BaseDevice::capture()" << std::endl;
	}
	//virtual inline void setActiveDevice(int i) { iCurrentDev = i; }
	virtual inline void setDataProcessor(DataProcessor* dp) { dataProcessor = dp; }
	//virtual inline long getTotalCountPackets() { return countPacketsTotal; }
	virtual unsigned int getDeviceCount() { return nDevices; }

	// data struct to be implemented as needed for status monitor
	DaqStatusBaseType* status;
protected:
	unsigned int nDevices;
	bool bOpened;
	DataProcessor* dataProcessor;
};