﻿#pragma once

#include "DataProcessor.h"
#include "Device/ADC4/DataAdc4.h"
#include "DataInRingBuffer.h"
#include "../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/Ndigo_interface.h"
#include <fstream>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;


// TODO: 将采集得到的数据转移到新的存储区并记录到 RingBuffer。开两个线程，一个用于数据采集，一个用于 socket 数据传输

class DataProcessorMemCopy : public DataProcessor {
public:
	DataProcessorMemCopy() : countPackets(0), DataProcessor() {
		for (int i = 0; i < nElemOfRingBuffer; i++) {
			try {
				packetBuffer[i].data = new uint64_t[maxDataLength];
			} catch (std::bad_alloc& e) {
				cerr << "[ERROR] bad_alloc when allocating packetBuffer[" << i << "].data:" << endl;
				cerr << e.what() << endl;
				return;
			}
		}
	}
	virtual ~DataProcessorMemCopy() {
		for (int i = 0; i < nElemOfRingBuffer; i++)
			delete[] packetBuffer[i].data;
	}

	int writeHeader(ndigo_packet* packet) {
		// attempt to transfer all data, whose header will be inserted into ring buffer here.
		copy_to_ring_buffer(packet);
		return 0;
	}

	int writeData(ndigo_packet* packet) {
		return 0;
	}
private:
	DataAdc4 packetBuffer[nElemOfRingBuffer];
	size_t countPackets;

	inline void copy_to_ring_buffer(ndigo_packet* packet) {
		DataAdc4* ptr = &packetBuffer[countPackets++ & (nElemOfRingBuffer - 1)];
		ptr->type = packet->type;
		ptr->card = packet->card;
		ptr->channel = packet->channel;
		ptr->flags = packet->flags;
		ptr->length = packet->length;
		ptr->timestamp = packet->timestamp;
		for (size_t i = 0; i < ptr->length; i++)
			ptr->data[i] = packet->data[i];

		// cout << "curr packet: " << packet << ", moving to " << ptr << endl;
		
		bool result = true;
		if (!queuePacket.insert(ptr)) {
			cout << "Warning: packet skipped due to the full ring buffer" << endl;
			result = false;
		}
		
		return;
	}
};