#pragma once

#ifdef _SIMULATE

#include "../../Timer.h"
#include "SimulateAdc4.h"

#include <fstream>
#include <memory>

class DataGenerationFunction {
public:
	DataGenerationFunction() {}
	virtual ~DataGenerationFunction() {}
	virtual double operator () (double t) = 0;
};

class DataGenerationFunctionGaussian : public DataGenerationFunction {
public:
	DataGenerationFunctionGaussian(double sigma, double t0, double ampl) :
		sigma{ sigma }, t0{ t0 }, ampl{ ampl }, DataGenerationFunction() {}
	virtual ~DataGenerationFunctionGaussian() {}
	double operator () (double t) override {
		return ampl * exp(-(t - t0) * (t - t0) / (2 * sigma * sigma));
	}
private:
	double sigma;
	double t0;
	double ampl;
};

class SimulatedDataModel {
public:
	SimulatedDataModel(unsigned char channel, unsigned char card,
		unsigned int length, unsigned __int64 timestamp,
		std::unique_ptr<DataGenerationFunction> func) :
		channel{ channel }, card{ card }, length{ length }, timestamp{ timestamp },
		func{ std::move(func) } {
	};
	virtual ~SimulatedDataModel() {}
	ndigo_packet* fillPacketData(ndigo_packet* packet) {
		// fill the header
		packet->channel = channel;
		packet->card = card;
		packet->type = NDIGO_PACKET_TYPE_16_BIT_SIGNED;
		packet->flags = 0;
		packet->length = length;
		packet->timestamp = timestamp;
		// fill crono_packet.data
		short* pData = reinterpret_cast<short*>(packet->data);
		unsigned int count = 0;
		while (count / 4 < length) {
			double time = (double)count++;
			*pData++ = static_cast<short>((*func)(time));
		}
		// return value: the starting address of the next packet
		return ndigo_next_packet(packet);
	}
private:
	unsigned char channel;
	unsigned char card;
	unsigned int length;
	unsigned __int64 timestamp;
	std::unique_ptr<DataGenerationFunction> func;
};


class SimulatedDataSource {
public:
	SimulatedDataSource() {

		SimulatedDataModel models[nPackets]{
			{0, 0, 92, 11111, std::make_unique<DataGenerationFunctionGaussian>(10., 100., 10000.)},
			{0, 0, 46, 22222, std::make_unique<DataGenerationFunctionGaussian>(20., 150., 8000.)},
			{0, 0, 72, 33333, std::make_unique<DataGenerationFunctionGaussian>(30., 200., 6000.)},
			{0, 0, 84, 44444, std::make_unique<DataGenerationFunctionGaussian>(40., 250., 4000.)}
		};

		// fill the buffer that stores data to be used as packets by other caller
		ndigo_packet* packet = reinterpret_cast<ndigo_packet*>(buffer);
		for (auto& model : models)
			packet = model.fillPacketData(packet);

		timer = new Timer;
		timer->start(msUpdate, std::bind(&SimulatedDataSource::updateByTimer, this));
	}
	virtual ~SimulatedDataSource() {
		timer->stop();
		delete timer;
	}

	bool update(ndigo_packet** firstPacket, ndigo_packet** lastPacket) {
		ndigo_packet* packet = reinterpret_cast<ndigo_packet*>(buffer);
		// "buffer" should have already been filled by parameters and pre-evaluatd data
		*firstPacket = packet;
		for (size_t i = 1; i < nPackets; i++)
			packet = ndigo_next_packet(packet);
		*lastPacket = packet;
		return true;
	}

	inline bool isReady() const { return ready; }

	inline void acknowledge() {
		std::unique_lock<std::mutex> lock(_mutex);
		ready = false;
	}

private:
	Timer* timer;
	const int msUpdate{ 1000 };     // time interval to update data in millisecond

	const static size_t nPackets{ 4 };

	unsigned __int64 buffer[4096];

	bool ready{ false }; // if data is ready to deliver
	std::mutex _mutex;
	void updateByTimer() { // producer, working on the thread initiated by the timer
		{
			std::unique_lock<std::mutex> lock(_mutex);
			ready = true;
		}
	}
};

#endif