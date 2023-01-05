#pragma once

class DeviceParamsBase;
class DataProducer;
class BaseDevice;
class DataProcessorMemCopy;
class DataSender;
class DaqSignal;
class CardParamsBase;
class MainWindow;
class DaqStatusBaseType;

#include <string>

class DaqSystem {
public:
	DaqSystem();
	virtual ~DaqSystem();

	void toggleStartDataAcquisition(DeviceParamsBase* params);
	bool isRunning();
	void setConfigRequired();
	unsigned int getNumberOfDevices();
	DaqStatusBaseType* getDeviceStatus();

	void toggleStartRemoteConnection(std::string ip, int port);

private:

	void initializeProducerConsumerModel();
	void finalizeProducerConsumerModel();

	void startDataProducer();
	void stopDataProducer();

	void startDataSender(std::string ip, unsigned short port);
	void stopDataSender();
	
	// producer (data acquisition)
	DataProducer* dp;
	BaseDevice* dev;
	DataProcessorMemCopy* dataProc;
	// consumer (data process)
	DataSender* ds;

	// 控制信号
	DaqSignal* sig;

	// 错误状态
	int err;
};