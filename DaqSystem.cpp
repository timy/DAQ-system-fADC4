#include "DaqSystem.h"

#include "BaseDevice.h"
#include "DeviceParamsBase.h"
#include "Device/ADC4/DeviceParamsAdc4.h"
#include "Device/ADC4/DeviceAdc4.h"

#include "DataProcessorMemCopy.h"
#include "DataProducer.h"
#include "DataSender.h"
#include "DaqSignal.h"
#include "ThreadWrapper.h"

class MainWindow;

DaqSystem::DaqSystem() : err(0) {

	dataProc = new DataProcessorMemCopy;
	dev = new DeviceAdc4;     // NOTE!!!! we choose ADC4 as the device, should be altered for other possible devices
	dp = new DataProducer;
	ds = new DataSender;
	sig = new DaqSignal;

	// init acquisition card
	dev->setDataProcessor(dataProc);
	
	// init producer
	dp->setDevice(dev);
	dp->setSignal(sig);

	// init consumer (store, send via net, and postprocess etc.)
	ds->setSignal(sig);
}

DaqSystem::~DaqSystem() {
	delete sig;
	delete ds;
	delete dp;
	delete dev;
	delete dataProc;
}



void DaqSystem::startDataProducer() {
	cout << "DaqSystem::startDataProducer, wait..." << endl;
	sig->bRun = true;
	
	std::thread thrDataProd(&DataProducer::start, dp);
	ThreadWrapper wrapThrDataProd(thrDataProd);	
}

void DaqSystem::stopDataProducer() {
	cout << "DaqSystem::stopDataProducer, wait..." << endl;
	sig->bRun = false;
}

void DaqSystem::toggleStartDataAcquisition(DeviceParamsBase* params) {
	if (!sig->bRun) {		
		if (sig->bConfig) {
			// no need to check the device status, since we do it in "initialize"
			//if (dev->isOpened())
			//	dev->finalize();
			dev->initialize(params, &err);
			if (err)
				return;
			sig->bConfig = false;
		}
		startDataProducer();
	} 
	else {
		stopDataProducer();
	}
}

bool DaqSystem::isRunning() {
	return sig->bRun;
}

void DaqSystem::setConfigRequired() {
	sig->bConfig = true;
}

unsigned int DaqSystem::getNumberOfDevices() {
	return dev->getDeviceCount();
}

DaqStatusBaseType* DaqSystem::getDeviceStatus() { 
	return dev->status;
}


void DaqSystem::startDataSender(std::string ip, unsigned short port) {
	cout << "DaqSystem::startDataSender, wait..." << endl;
	sig->bConnect = true;

	std::thread thrDataSend(&DataSender::start, ds, ip, port);
	ThreadWrapper wrapThrDataSend(thrDataSend);
}

void DaqSystem::stopDataSender() {
	cout << "DaqSystem::stopDataSender, wait..." << endl;
	sig->bConnect = false;
}

void DaqSystem::toggleStartRemoteConnection(std::string ip, int port)
{
	if (!ds->connected()) {
		startDataSender(ip, port);
	}
	else {
		stopDataSender();
	}
}


