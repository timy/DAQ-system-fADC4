#pragma once

class DataProcessor;
class DeviceItemParams;
class DevMonitorDataBaseType;
#include <iostream>
class BaseDevice {
public:
	virtual void initialize(DeviceItemParams** params, int* err) = 0;
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
	virtual int getDeviceCount() { return nDevices; }

	// data struct to be implemented as needed for status monitor
	DevMonitorDataBaseType* status;
private:
	int nDevices;
	bool bOpened;
	//int iCurrentDev;
	DataProcessor* dataProcessor;
};