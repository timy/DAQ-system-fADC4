#pragma once

// #include "../../../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/Ndigo_interface.h"
// #include "../../../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/crono_tools.h"

#ifndef _SIMULATE
#include "Libs/Ndigo_interface.h"
#include "Libs/crono_tools.h"
#else
#include "SimulateAdc4.h"
#endif // _SIMULATE
#include "../../BaseDevice.h"
#include <cstdio>


class DataProcessor;
class DaqStatusAdc4Type;
class DeviceParamsAdc4;

class DeviceAdc4 : public BaseDevice {
public:
	DeviceAdc4();
	~DeviceAdc4();
	void startAcquisition() override;
	void stopAcquisition() override;
	void capture(bool* bRun) override;
	virtual void initialize(DeviceParamsBase* params, int* err) override;
	virtual void finalize() override;
	
	virtual bool isOpened() override { return bOpened; }

	

private:

	void inquireCardsNumber();

	// called by initialize (CardParamsBase* params)
	void configInitParams(DeviceParamsAdc4* params);
	void configTrigParams(DeviceParamsAdc4* params);
	void configCronoSync(DeviceParamsAdc4* params);
	void setupStatus(DeviceParamsAdc4* params);

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