#pragma once

#include "DataProcessor.h"
#include "Ndigo_interface.h"
#include <fstream>

class DataProcessorSaveFile : public DataProcessor {
public:
	DataProcessorSaveFile() : DataProcessor() {
		file_info.open("info.dat");
		file_data.open("waveform.dat");
	}
	virtual ~DataProcessorSaveFile() {
		file_info.close();
		file_data.close();
	}

	int writeHeader(ndigo_packet* packet) {
		// fprintf(file_info, "%02x %02x %02x %6d %11u\n", packet->card, packet->channel, packet->flags, length, packet->timestamp);
		int length = 0;
		if (!(packet->type & NDIGO_PACKET_TYPE_TIMESTAMP_ONLY))
			length = packet->length;
		file_info << (int)packet->card << " " << (int)packet->channel << " " << (int)packet->flags << " "
			<< length << " " << packet->timestamp << std::endl;
		return 0;
	}

	int writeData(ndigo_packet* packet) {
		// packet->data is of the type int64, each 64-bit word contains four 16-bit samples, 
		// which can be directly operated on by casting into short type
		short* data = (short*)packet->data;
		for (size_t i = 0; i < packet->length * 4; i++)
			file_data << *(data++) << " ";
		file_data << std::endl;
		return 0;
	}
private:
	std::ofstream file_info;
	std::ofstream file_data;
};