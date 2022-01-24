#pragma once

// #include "../../../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/Ndigo_interface.h"
// #include "../../../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/crono_tools.h"

#include "Libs/Ndigo_interface.h"
#include "Libs/crono_tools.h"
#include "../../BaseDevice.h"
#include <cstdio>


class DataProcessor;
class DaqStatusAdc4Type;

class DeviceAdc4 : public BaseDevice {
public:
	DeviceAdc4();
	virtual ~DeviceAdc4();
	void startAcquisition();
	void stopAcquisition();
	void capture(bool* bRun);
	virtual void initialize(DeviceParamsBase* params, int* err);
	virtual void finalize();
	
	virtual bool isOpened() { return bOpened; }

	

private:

	void inquireCardsNumber();

	// called by initialize (CardParamsBase* params)
	void configInitParams(DeviceParamsBase* params);
	void configTrigParams(DeviceParamsBase* params);
	void configCronoSync(DeviceParamsBase* params);
	void setupStatus(DeviceParamsBase* params);

	void getStaticInfo();
	void getParameters();

	ndigo_device** devices;

	ndigo_init_parameters* init_params;

	ndigo_static_info* static_info;

	crono_device* cro_devices;

	crono_sync* ndigo_sync;

	crono_sync_init_parameters sync_init;

	FILE* file_info; // NOTE!!!! for temporary test
	FILE* file_data; // NOTE!!!! for temporary test

	int err_code;
	const char* err_msg;

};