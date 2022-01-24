#pragma once

#include "../../DaqStatusBaseType.h"
#include <mutex>
#include <vector>

struct ndigo_packet;
class DaqStatusAdc4Type : public DaqStatusBaseType {
public:
	DaqStatusAdc4Type(size_t nDevices_);
	virtual ~DaqStatusAdc4Type();

	void retrieveWaveform(ndigo_packet* packet);

	long cyclesReadTotal = 0;
	long cyclesReadValid = 0;
	long countPacketsTotal = 0;
	long countDataTotal = 0;

	long countData[2] = { 0, 0 };

	// sampling waveform
	unsigned long long *** waveforms;
	int** lengths;
	unsigned long long ** timestamp;
	bool bRetrieveWaveform = false;
	bool isWaveformRetrieved = false;
	const static int nDataMax = 65536; // 2^16, need check...

	std::mutex lock;
	std::condition_variable cv;

	const static int nChannels = 4;

	std::vector<int>* idxChannels; // channels in use for the ith device/card
	size_t nDevices;	
};