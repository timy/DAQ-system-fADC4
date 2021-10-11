#pragma once

#include "DevMonitorDataBaseType.h"

class DevMonitorDataAdc4Type : public DevMonitorDataBaseType {
public:
	long cyclesReadTotal = 0;
	long cyclesReadValid = 0;
	long countPacketsTotal = 0;
	long countDataTotal = 0;

	long countData[2] = { 0, 0 };
	/*
	NOTE!!!! 
	it is also possible to define function to retrieve more complicated 
	data as inquired at the cost of sacrificing performance, for instance, 
	to observe some online sampling waveform
	*/
	void retrieveWaveform() {}
private:

};