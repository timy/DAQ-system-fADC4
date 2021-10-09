#include "DaqSystem.h"

#include "BaseDevice.h"
#include "Device/ADC4/DeviceAdc4Params.h"
#include "Device/ADC4/DeviceAdc4.h"
#include "DeviceParams.h"

#include "DataProcessorMemCopy.h"
#include "DataProducer.h"
#include "DataSender.h"
#include "DevMonitor.h"
#include "DaqSignal.h"
#include "ThreadWrapper.h"

class MainWindow;
DaqSystem::DaqSystem() : err(0) {

	dataProc = new DataProcessorMemCopy;
	dev = new DeviceAdc4;     // NOTE!!!! we choose ADC as the device
	dp = new DataProducer;
	ds = new DataSender;
	dm = new DevMonitor;
	sig = new DaqSignal;

	// init acquisition card
	dev->setDataProcessor(dataProc);
	// dev->setActiveDevice(0); // NOTE!!!! we may have multiple cards run simultaneously
	// dev.startAcquisition(); // this launches "ndigo_capture", which actually should be started when clicking the button
	// on the other hand, "dev.capture" is not to capture, but invoke "ndigo_read" to collect already launched DAQ device
	
	// init producer
	dp->setDevice(dev);
	dp->setSignal(sig);

	// init consumer (store, send via net, and postprocess etc.)
	ds->setSignal(sig);

	// init device monitor
	dm->setSignal(sig);
	dm->setDataSource(dev->status);
}

DaqSystem::~DaqSystem() {
	delete sig;
	delete dm;
	delete ds;
	delete dp;
	delete dev;
	delete dataProc;
}



void DaqSystem::start() {
	cout << "DaqSystem::start, wait..." << endl;
	sig->bRun = true;
	
	std::thread thrDataProd(&DataProducer::start, dp);
	ThreadWrapper wrapThrDataProd(thrDataProd);

	std::thread thrDevMonitor(&DevMonitor::start, dm);
	ThreadWrapper wrapThrDevMonitor(thrDevMonitor);

	// NOTE!!!! we comment the following just to focus on the debug of the DAQ system
	//std::thread thrDataSend(&DataSender::start, ds);
	//ThreadWrapper wrapThrDataSend(thrDataSend);
}

void DaqSystem::stop() {
	cout << "DaqSystem::stop, wait..." << endl;
	sig->bRun = false;
}

void DaqSystem::toggleStart(DeviceItemParams** params) {
	if (!sig->bRun) {		
		if (sig->bConfig) {
			// no need to check if the device is opened, since we have checked in "initialize"
			//if (dev->isOpened())
			//	dev->finalize();
			dev->initialize(params, &err);
			if (err)
				return;
			sig->bConfig = false;
		}
		start();
	} 
	else {
		stop();
	}
}

bool DaqSystem::isRunning() {
	return sig->bRun;
}

void DaqSystem::setConfigRequired() {
	sig->bConfig = true;
}

int DaqSystem::getNumberOfDevices() {
	return dev->getDeviceCount();
}

void DaqSystem::setDevMonitor(MainWindow* wnd, 
	void (*funcUpdate)(MainWindow*, DevMonitorDataBaseType*)) {
	dm->setDataDisplay(wnd);
	dm->setUpdateFunc(funcUpdate);
}