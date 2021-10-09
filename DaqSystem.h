#pragma once


class DataProducer;
class BaseDevice;
class DataProcessorMemCopy;
class DataSender;
class DevMonitor;
class DaqSignal;
class DeviceItemParams;
class MainWindow;
class DevMonitorDataBaseType;

namespace std {
	class thread;
}


class DaqSystem {
public:
	DaqSystem();
	virtual ~DaqSystem();

	void toggleStart(DeviceItemParams** params);
	bool isRunning();
	void setConfigRequired();
	int getNumberOfDevices();
	void setDevMonitor(MainWindow*, void (*funcUpdate)(MainWindow*, DevMonitorDataBaseType*));

private:

	void start();
	void stop();

	// DAQSystem 负责两部分，数据采集卡（硬件采集） 与 数据后处理（数据转储）
	// 被抽象为 生产者-消费者模型
	
	// 生产者（数据采集卡部分）
	DataProducer* dp;
	BaseDevice* dev;
	DataProcessorMemCopy* dataProc;
	// 消费者（数据后处理部分）
	DataSender* ds;

	// 设备监控
	DevMonitor* dm;

	// 控制信号
	DaqSignal* sig;

	// 错误状态
	int err;

	// 线程
	//std::thread* thrDataProd;
	//std::thread* thrDataSend;

};