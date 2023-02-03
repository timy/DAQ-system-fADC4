#pragma once

#ifdef _SIMULATE

#include "../../Timer.h"
#include "SimulateAdc4.h"

#include <fstream>

struct SimulatedDataPacketBufferADC4 {
	crono_packet packet;              // 24 bytes, including "data" of 8 bytes
	unsigned __int64 data_seq[1023];  // 1023 * 8 bytes. Adjust the size as required, imitating the buffer on chip
};


class SimulatedDataSource {
public:
	SimulatedDataSource() : ready(false) {
		// allocate buffer
		buffers = new SimulatedDataPacketBufferADC4* [nBuffers];
		for (int iBuffer = 0; iBuffer < nBuffers; iBuffer++)
			buffers[iBuffer] = new SimulatedDataPacketBufferADC4;
		
		// fill the buffer
		for (int iBuffer = 0; iBuffer < nBuffers; iBuffer++) {
			
			crono_packet& packet = buffers[iBuffer]->packet; // = &buffers[iBuffer]

			// generate data (a gaussian), fill crono_packet.data
			const float sigma = 10;
			nData = 1024;
			short* pData = reinterpret_cast<short*>(packet.data);
			int i = 0;
			while (i / 4 < nData) {
				*pData = static_cast<short>(10000 * exp(-(i - 200.) * (i - 200.) / (2. * sigma * sigma)));
				pData++;
				i++;
			}

			// fill crono_packet
			packet.channel = 0;
			packet.card = 0;
			packet.type = 0;
			packet.flags = 0;
			packet.length = nData;
			packet.timestamp = 12345;
		}

		// crono_group - assuming we have only one group now, i.e., nGroups = 1
		groups = new crono_group[nGroups];
		for (int iGroup = 0; iGroup < nGroups; iGroup++) {
			groups[iGroup].trigger_index = 0;
			groups[iGroup].flags = 0;
			groups[iGroup].timestamp = 12345;
			groups[iGroup].packet_count = (int)nBuffers; // adjust to actual number
			groups[iGroup].packets = reinterpret_cast<crono_packet**>(buffers);
			// NOTE!!!! 当前实现可能仅对 packet_count=1 有效。因为外部对 buffers 的调用会以 crono_packet 为
			// 尺寸，不包含 SimulatedDataPacketBufferADC4 中 data_seq 的部分。这样在调用 group->packets[i]
			// 且 i > 0 时，索引的地址必然会指向 group->packets[0] 中 data_seq 的某个地址，出现错误
			// 还需继续考虑如何设计
		}
		
		timer = new Timer;
		timer->start(msUpdate, std::bind(&SimulatedDataSource::updateByTimer, this));
	}
	virtual ~SimulatedDataSource() {
		timer->stop();
		delete timer;
		delete[] groups;
		for (int iBuffer = 0; iBuffer < nBuffers; iBuffer++)
			delete buffers[iBuffer];
		delete[] buffers;
	}
	void generate(crono_sync_read_out* out) { // consumer
		// simulate the data acquisition of the device, once ready, fill `out`
		if (ready) {
			std::unique_lock<std::mutex> lock(_mutex);
			// crono_sync_read_out
			out->group_count = 1;
			out->groups = groups; // has been prepared in constructor
			out->error_code = 0;
			out->error_message = "OK";
			ready = false;
		}
	}

private:
	Timer* timer;
	const int msUpdate{ 1000 };     // time interval to update data in millisecond
	const size_t nBuffers{ 1 };		// pre-allocated
	const size_t nGroups{ 1 };		// pre-allocated
	int nData;
	SimulatedDataPacketBufferADC4** buffers;
	crono_group* groups;
	bool ready; // if data is ready to deliever
	std::mutex _mutex;
	void updateByTimer() { // producer, working on the thread initiated by the timer
		{
			std::unique_lock<std::mutex> lock(_mutex);
			// produce data
			ready = true;
		}
	}
};

#endif