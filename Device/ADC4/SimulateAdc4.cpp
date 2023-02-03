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

CRONO_API int crono_sync_read(crono_sync* device, crono_sync_read_in* in, crono_sync_read_out* out) {
	sds->generate(out);
	return 0;
}

CRONO_API const char* crono_sync_get_last_error_message(crono_sync* device) {
	return "OK";
}
