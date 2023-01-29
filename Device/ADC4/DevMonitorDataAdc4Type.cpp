#include "DaqStatusAdc4Type.h"
#include <iostream>
// #include "../../../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/Ndigo_interface.h"
#ifndef _SIMULATE
#include "Libs/Ndigo_interface.h"
#else
#include "SimulateAdc4.h"
#endif

DaqStatusAdc4Type::DaqStatusAdc4Type(size_t nDevices_) : nDevices(nDevices_) {
	// allocate memory for waveform samples
	waveforms = new unsigned long long** [nDevices];
	for (int i = 0; i < nDevices; i++) {
		waveforms[i] = new unsigned long long* [nChannels];
		for (int j = 0; j < nChannels; j++)
			waveforms[i][j] = new unsigned long long[nDataMax];
	}

	lengths = new int* [nDevices];
	for (int i = 0; i < nDevices; i++)
		lengths[i] = new int[nChannels];

	timestamp = new unsigned long long* [nDevices];
	for (int i = 0; i < nDevices; i++)
		timestamp[i] = new unsigned long long[nChannels];

	idxChannels = new std::vector<int>[nDevices];
}

DaqStatusAdc4Type::~DaqStatusAdc4Type() {
	// free memeory for waveform samples

	delete[] idxChannels;

	for (int i = 0; i < nDevices; i++)
		delete[] timestamp[i];
	delete[] timestamp;
	
	for (int i = 0; i < nDevices; i++)
		delete[] lengths[i];
	delete[] lengths;

	for (int i = 0; i < nDevices; i++) {
		for (int j = 0; j < nChannels; j++)
			delete[] waveforms[i][j];
		delete[] waveforms[i];
	}
	delete[] waveforms;
}

void DaqStatusAdc4Type::retrieveWaveform(ndigo_packet* packet) {
	// a lock is required here
	std::lock_guard<std::mutex> mylock_guard(lock);
	lengths[packet->card][packet->channel] = packet->length;
	timestamp[packet->card][packet->channel] = packet->timestamp;
	memset(waveforms[packet->card][packet->channel], 0, nDataMax);
	if (packet->length * 4 > nDataMax) {
		std::cout << "online detection error: length*4 (" << packet->length * 4 
			<< ") > nDataMax" << std::endl;
	}
	else {
		memcpy(waveforms[packet->card][packet->channel], packet->data, 
			packet->length * sizeof(packet->data[0]));
		isWaveformRetrieved = true;
		cv.notify_one();
	}
	
}