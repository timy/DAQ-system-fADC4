#ifdef _SIMULATE

#include "SimulateAdc4.h"
#include "SimulatedDataSourceAdc4.h"

NDIGO_API int ndigo_count_devices(int* error_code, const char** error_message) {
	const int nDevices = 2;
	return nDevices;
}

NDIGO_API int ndigo_get_static_info(ndigo_device* device, ndigo_static_info* info) {
	return 0;
}

NDIGO_API int ndigo_get_fast_info(ndigo_device* device, ndigo_fast_info* info) {
	return 0;
}

NDIGO_API int ndigo_get_param_info(ndigo_device* device, ndigo_param_info* info) {
	return 0;
}

NDIGO_API int ndigo_get_default_init_parameters(ndigo_init_parameters* init) {
	return 0;
}

NDIGO_API int ndigo_get_default_configuration(ndigo_device* device, ndigo_configuration* config) {
	return 0;
}

NDIGO_API int ndigo_configure(ndigo_device* device, ndigo_configuration* config) {
	return NDIGO_OK;
}

NDIGO_API int ndigo_set_board_id(ndigo_device* device, int board_id) {
	return 0;
}

NDIGO_API int ndigo_close(ndigo_device* device) {
	return 0;
}

static crono_sync sync;
SimulatedDataSource* sds;


NDIGO_API int ndigo_read(ndigo_device* device, ndigo_read_in* in, ndigo_read_out* out) {
	sds->update(&out->first_packet, &out->last_packet);
	out->error_code = 0;
	out->error_message = "OK";
	return 0;
}

NDIGO_API int ndigo_acknowledge(ndigo_device* device, ndigo_packet* packet) {
	return 0;
}


CRONO_API crono_sync* crono_sync_init(crono_sync_init_parameters* params, int* error_code, const char** error_message) {
	return &sync;
}

CRONO_API int crono_sync_close(crono_sync* device) {
	return 0;
}

CRONO_API int ndigo_init_multiple(void* init_params, int count, crono_device** ndigo, int* error_code, const char** error_message) {
	error_code = NDIGO_OK;
	return 0;
}

CRONO_API int crono_sync_configure(crono_sync* sync, crono_sync_configuration* config, int* error_code, const char** error_message) {
	error_code = CRONO_OK;
	return 0;
}

CRONO_API int crono_sync_start_capture(crono_sync* device) {
	sds = new SimulatedDataSource;
	return 0;
}

CRONO_API int crono_sync_stop_capture(crono_sync* device) {
	delete sds;
	return 0;
}

#include <iostream> // NOTE!!!! remove me later!!!!
CRONO_API int crono_sync_read(crono_sync* device, crono_sync_read_in* in, crono_sync_read_out* out) {

	// pre-allocat buffer to store out->group[0].packets
	const int group_count{ 1 };
	const int maxNumPackets{ 16 };
	static crono_group croGroup[group_count]; // a buffer to store an array of crono_groups
	static crono_packet* croPacketAddresses[maxNumPackets]; // a buffer to store an array of packet addresses of all groups
	static unsigned __int64 cronoPacketBuffer[4096]; // a buffer to store actual packets of all groups
	ndigo_device* currentDevice = nullptr; // nullptr for simulated device
	ndigo_read_in ndigo_in;
	ndigo_in.acknowledge_last_read = false;
	ndigo_read_out ndigo_out;

	if (sds->isReady()) {
		int result = ndigo_read(currentDevice, &ndigo_in, &ndigo_out);
		// assign the read data, as structured by group -> packet, to the static buffer
		if (!result) {
			ndigo_packet* ndgPacket = ndigo_out.first_packet;
			crono_packet** pCroPacketAddr = croPacketAddresses; // pointer to current crono packet address
			// Next, fill the buffer of crono_group from ndigo_read_out struct
			for (int iGroup = 0; iGroup < group_count; iGroup++) {
				if (pCroPacketAddr - croPacketAddresses > maxNumPackets) {
					std::cout << "[Warning] Maximum number of packets reached!\n";
					break;
				}
				// set crono_group.packets points to the moving address in buffer croPacketAddresses
				croGroup[iGroup].packets = pCroPacketAddr;
				// set the first packet of the current group to point to the pre-allocated cronoPacketBuffer
				int iPkt = 0;
				croGroup[iGroup].packets[iPkt] = reinterpret_cast<crono_packet*>(cronoPacketBuffer);
				while (ndgPacket <= ndigo_out.last_packet) {
					crono_packet* croPacket = croGroup[iGroup].packets[iPkt];
					// fill fields of packet
					croPacket->channel = ndgPacket->channel;
					croPacket->card = ndgPacket->card;
					croPacket->type = ndgPacket->type;
					croPacket->flags = ndgPacket->flags;
					croPacket->length = ndgPacket->length;
					croPacket->timestamp = ndgPacket->timestamp;
					unsigned __int64* pData = croPacket->data;
					for (unsigned int iData = 0; iData < ndgPacket->length; iData++)
						*pData++ = ndgPacket->data[iData];
					// next packet on the buffer
					iPkt += 1;
					// calculate the address of next packet on the cronoPacketBuffer
					croGroup[iGroup].packets[iPkt] = reinterpret_cast<crono_packet*>(ndigo_next_packet(croPacket));
					// calculate the address of next packet of the data source 
					ndigo_packet* nextPacket = ndigo_next_packet(ndgPacket);
					ndigo_acknowledge(currentDevice, ndgPacket); // acknowledge all date up to the packet
					// move to the next packet of the data source
					ndgPacket = nextPacket;
				}
				croGroup[iGroup].packet_count = iPkt;
				pCroPacketAddr += iPkt;
			}
			sds->acknowledge();
			out->group_count = group_count;
		}
	}
	else {
		out->group_count = 0;
	}
	out->groups = croGroup; // fill the output struct with the buffer address 
	out->error_code = 0;
	out->error_message = "OK";
	return 0;
}

CRONO_API const char* crono_sync_get_last_error_message(crono_sync* device) {
	return "OK";
}

#endif