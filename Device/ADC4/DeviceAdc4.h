#pragma once

#include "../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/Ndigo_interface.h"
#include "../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/crono_tools.h"
#include "BaseDevice.h"
#include <cstdio>


class DataProcessor;
class DeviceAdc4Params;
class DevMonitorDataAdc4Type;

class DeviceAdc4 : public BaseDevice {
public:
	DeviceAdc4();
	virtual ~DeviceAdc4();
	void startAcquisition();
	void pauseAcquisition();
	void stopAcquisition();
	void capture(bool* bRun);
	inline void setActiveDevice(int i) { iCurrentDev = i; }
	inline void setDataProcessor(DataProcessor* dp) { dataProcessor = dp; }
	int getDeviceCount() { return nDevices; }
	virtual void initialize(DeviceItemParams** params, int* err);
	virtual void finalize();
	
	virtual bool isOpened() { return bOpened; }

	

private:

	void inquireCardsNumber();

	// called by initialize (DeviceItemParams* params)
	void configInitParams(DeviceItemParams** params);
	void configTrigParams(DeviceItemParams** params);
	void configCronoSync();

	void getStaticInfo();
	void getParameters();

	int nDevices;
	int iCurrentDev;
	bool bOpened;

	ndigo_device** devices;

	ndigo_init_parameters* init_params;

	ndigo_static_info* static_info;

	crono_device* cro_devices;

	crono_sync* ndigo_sync;

	crono_sync_init_parameters sync_init;

	DataProcessor* dataProcessor;

	FILE* file_info; // NOTE!!!! for temporary test
	FILE* file_data; // NOTE!!!! for temporary test

	int err_code;
	const char* err_msg;

	// DevMonitorDataAdc4Type* status; // NOTE!!!!!

};