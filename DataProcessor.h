#pragma once

class DataProcessor {
public:
	DataProcessor() {}
	virtual ~DataProcessor() {}

	virtual int writeHeader(ndigo_packet* packet) {
		return 0;
	}
	virtual int writeData(ndigo_packet* packet) {
		return 0;
	}
};