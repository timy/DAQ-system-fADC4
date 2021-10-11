#include "DeviceAdc4.h"
#include "DataProcessor.h"
// #include "../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/Ndigo_interface.h"
#include "../../CoboldPC_2011_R5-2c_fADC4_2020-09-17/Sources/Libs/crono_tools.h"
#include "DeviceAdc4Params.h"
#include "DevMonitorDataAdc4Type.h"
//#include "crono_tools.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

void show_default_init_params(ndigo_init_parameters* params);
void show_device_static_info(ndigo_static_info* info);
void show_device_param_info(ndigo_param_info* info);

DeviceAdc4::DeviceAdc4() : nDevices(0), iCurrentDev(0), devices(nullptr), bOpened(false) {

	inquireCardsNumber();

	if (nDevices > 0) {
		devices = new ndigo_device * [nDevices];
		cro_devices = new crono_device[nDevices];
		init_params = new ndigo_init_parameters[nDevices];
	}

	status = new DevMonitorDataAdc4Type;

	// 此后的指令，应在configuration中执行
	//setInitParams();

	//static_info = new ndigo_static_info[nDevices];
	//getStaticInfo();

	// configure(); // NOTE!!!!
	// cout << "Configuration completed. Parameters of the current setup: " << endl << endl;
	// getParameters();
}

DeviceAdc4::~DeviceAdc4() {

	delete status;

	if (nDevices > 0) {

		delete[] init_params;
		delete[] cro_devices;

		// NOTE!!!! 暂时注释，检查关闭的合适位置
		if (isOpened())
			finalize();

		delete[] devices;
	}
	devices = nullptr;
	// delete[] static_info;
}

void DeviceAdc4::startAcquisition() {
	// ndigo_start_capture(devices[iCurrentDev]);



	////////////////////////////////////////  开始采集数据

	err_code = crono_sync_start_capture(ndigo_sync);
	if (err_code != CRONO_OK) {
		printf("Error %i in crono_sync_start_capture\n(%s)", err_code, crono_sync_get_last_error_message(ndigo_sync));
		return;
	}

	errno_t err;
	err = fopen_s(&file_info, "info.dat", "w"); // NOTE!!!! file operation will be reconstructed soon
	err = fopen_s(&file_data, "waveform.dat", "w"); // NOTE!!!! file operation will be reconstructed soon

	cout << "Data acquisition started" << endl << endl;
}

void DeviceAdc4::pauseAcquisition() {
	ndigo_pause_capture(devices[iCurrentDev]);
	cout << "Data acquisition paused" << endl << endl;
}

void DeviceAdc4::stopAcquisition() {
	// ndigo_stop_capture(devices[iCurrentDev]);
	crono_sync_stop_capture(ndigo_sync);

	fclose(file_info);
	fclose(file_data);
	cout << "Data acquisition stopped" << endl << endl;
}

void DeviceAdc4::capture(bool* bRun) {
	//ndigo_read_in in;
	//in.acknowledge_last_read = false; // acknowledge_last_read
	//ndigo_read_out out;
	//int result = ndigo_read(devices[iCurrentDev], &in, &out);
	//int countPacketsOfCurrentBatch = 0;
	//if (!result) {
	//	// received buffer with one or more packets
	//	ndigo_packet* packet = out.first_packet;
	//	while (packet <= out.last_packet) {
	//		this->dataProcessor->writeHeader(packet);
	//		if (!(packet->type & NDIGO_PACKET_TYPE_TIMESTAMP_ONLY))
	//			this->dataProcessor->writeData(packet);
	//		ndigo_packet* next_packet = ndigo_next_packet(packet);
	//		ndigo_acknowledge(devices[iCurrentDev], packet); // acknowledge all data up to the packet
	//		packet = next_packet;
	//		countPacketsOfCurrentBatch ++;
	//	}
	//	countPacketsTotal += countPacketsOfCurrentBatch;
	//}
	//return countPacketsOfCurrentBatch;


	



	
	crono_sync_read_in crin;
	crono_sync_read_out crout;
	crout.group_count = 0;
	int currGroup = 0;

	////////////////////////////////////////  进入读取循环
	DevMonitorDataAdc4Type* st = static_cast<DevMonitorDataAdc4Type*>(status);
	st->cyclesReadTotal = 0;
	st->cyclesReadValid = 0;
	st->countPacketsTotal = 0;
	st->countDataTotal = 0;

	while (*bRun) {
		// 两种情况下，需要重新读取数据：
		// 当前 group_count 为零，意味着首次采集；
		// 当前 currGroup 超过 crout 中的数据组群数，意味着之前读取的数据已经处理完毕
		if (crout.group_count < 1 || currGroup >= crout.group_count) {
			currGroup = 0;
			err_code = crono_sync_read(ndigo_sync, &crin, &crout);	// L745
			st->cyclesReadTotal++;
			st->cyclesReadValid++;
		}

		// if crout.group_count < 1，crono_sync_read 再次读取的 group 数量为零，因此无需处理，继续循环
		if (crout.group_count < 1) { // 无数据
			st->cyclesReadValid--;
			continue;
		}

		// if crout.group_count >= 1, start processing
 
		// loop over each group
		crono_group* group = &crout.groups[currGroup++];

		if (group->packet_count < 1) {
			st->cyclesReadValid--;
			continue;
		}

		// loop over each packet within the group
		for (int i = 0; i < group->packet_count; i++) {
			// crono_packet* packet = (crono_packet*)group->packets[i];	// L771
			ndigo_packet* packet = (ndigo_packet*)group->packets[i];	// NOTE!!!! crono_ and ndigo_ should be the same...

			int length;
			if (!(packet->type & NDIGO_PACKET_TYPE_TIMESTAMP_ONLY)) {
				fprintf(file_info, "%02x %02x %02x %6d %11u\n", packet->card, packet->channel,
					packet->flags, packet->length, packet->timestamp);
				// this->dataProcessor->writeHeader(packet); // NOTE!!! switch from file export to dataProcessor
				length = packet->length;
			}

			if (!(packet->type & NDIGO_PACKET_TYPE_TIMESTAMP_ONLY)) {
				short* data = (short*)packet->data;
				// loop over each data segment within each packet
				for (int i = 0; i < length * 4; i++) {
					fprintf(file_data, "%8d", *(data++));
				}
				fprintf(file_data, "\n");

				// this->dataProcessor->writeData(packet); // NOTE!!! switch from file export to dataProcessor

				st->countDataTotal += length;
				st->countData[packet->card] += length;
			}
		}
		st->countPacketsTotal += group->packet_count;

		//cout << "cycleTotal " << st->cyclesReadTotal
		//	<< "; cyclesValid " << st->cyclesReadValid
		//	<< "; packets=" << st->countPacketsTotal
		//	<< "; data=" << st->countDataTotal << endl;
	
		// NOTE!!!! when cyclesValid approaches cycleTotal, it indicates the PC may not be fast enough to process
		// the data transferred from the ADC card due to, for instance, the IO bottleneck, i.e., the PC has consumed 
		// too much time saving data to disk.
	
	}
}

void DeviceAdc4::inquireCardsNumber() {
	nDevices = ndigo_count_devices(&err_code, &err_msg);
	if (nDevices == 0) {
		cout << "Error: no fADC4 device found!" << endl;
		return;
	}
	cout << "DeviceAdc4::inquireCardsNumber: found " << nDevices << " fADC4 devices" << endl;
}

void DeviceAdc4::getStaticInfo() {
	for (int i = 0; i < nDevices; i++) {
		ndigo_get_static_info(devices[i], &static_info[i]);
		// cout << "Static information of device " << i << " ------------------\n";
		// show_device_static_info(&static_info[i]);
		cout << endl;
	}
}
void DeviceAdc4::configInitParams(DeviceItemParams** params) {
	
	for (int i = 0; i < nDevices; i++) {
		DeviceAdc4Params* p = static_cast<DeviceAdc4Params*>(params[i]);
		ndigo_init_parameters* pp = &init_params[i];
		ndigo_get_default_init_parameters(pp);


		pp->card_index = i;
		pp->board_id = i;
		pp->version = NDIGO_API_VERSION;
		pp->hptdc_sync_enabled = 0;
		pp->buffer_size[0] = 1 << 23; // the minimum size of the DMA buffer, fADC4/10 only uses buffer_size[0]
		// pp.buffer_size[0] = 0; // if set to 0, the default size of 16 MB is used
		// pp.is_slave = (p->id != 0);             // NOTE!!!!! config for DeviceItemParams!!!
		pp->is_slave = (i != p->idxMasterChannel);
		// params.drive_external_clock = (p->id == 0); // NOTE!!!!! config for DeviceItemParams!!!
		pp->drive_external_clock = (i == p->idxMasterChannel);
		// params.use_external_clock = (p->id != 0);   // NOTE!!!!! config for DeviceItemParams!!!
		pp->use_external_clock = (i != p->idxMasterChannel);
		pp->multiboard_sync = 1;
		pp->sync_period = 4; // NOTE!!!! require further test
		// pp.sync_delay = 1;
		// pp.force_window_calibration = 0;
		// pp.variant = 0;

		cout << "DeviceAdc4::configInitParams: init device " << i << endl;
		//show_default_init_params(&pp);
	}

	// 应当在设置所有板卡参数后，只调用一次 init_multiple
	ndigo_init_multiple(init_params, nDevices, (crono_device**)devices, &err_code, &err_msg);
	if (err_code != NDIGO_OK && err_code != NDIGO_WINDRIVER_NOT_FOUND) {
		// no idea why err_code may be NDIGO_WINDRIVER_NOT_FOUND when the function are called again. Just skip the msg.
		throw "[ERROR] DeviceAdc4::configInitParams - nidgo_init_multiple";
		return;
	}
}

void DeviceAdc4::configTrigParams(DeviceItemParams** params) {

	ndigo_configuration cfg;
	const unsigned int trigger_entries[] = { 
		NDIGO_TRIGGER_A0, 
		NDIGO_TRIGGER_B0, 
		NDIGO_TRIGGER_C0, 
		NDIGO_TRIGGER_D0,
	}; // NOTE!!!! also consider using NDIGO_TRIGGER_A1, B1, ...
	const unsigned int trigger_sources[] = {
		NDIGO_TRIGGER_SOURCE_A0,
		NDIGO_TRIGGER_SOURCE_B0,
		NDIGO_TRIGGER_SOURCE_C0,
		NDIGO_TRIGGER_SOURCE_D0,
	};


	for (int i = 0; i < nDevices; i++) {
		// 板卡设置
		DeviceAdc4Params* p = static_cast<DeviceAdc4Params*>(params[i]);
		ndigo_get_default_configuration(devices[i], &cfg);
		
		cfg.adc_mode = p->mode; // NOTE!!!! should get the sequence the same for ndigo and my own mode
		cfg.trigger_block[4].enabled = false; // NOTE!!!! including i=4, also consider future extension using the TDC as trigger

		//cout << "[Card " << i << "]" << endl;
		//cout << "adc_mode: " << cfg.adc_mode << endl;
		for (int iCh = 0; iCh < 4; iCh++) {
			// 通道设置
			cfg.trigger_block[iCh].enabled = true;
			cfg.trigger_block[iCh].minimum_free_packets = 1.;
			cfg.trigger_block[iCh].precursor = p->channel[iCh].precursor;
			cfg.trigger_block[iCh].retrigger = p->channel[iCh].isRetrigger;
			cfg.trigger_block[iCh].sources = trigger_sources[iCh]; // NOTE!!! depends on the mode
			cfg.trigger_block[iCh].length = p->channel[iCh].length;
			cfg.trigger_block[iCh].gates = NDIGO_TRIGGER_GATE_NONE;
			cfg.analog_offset[iCh] = p->channel[iCh].offset;
			cfg.trigger[trigger_entries[iCh]].edge = p->channel[iCh].isEdgeMode; // NOTE!!!! note the trigger_entries
			cfg.trigger[trigger_entries[iCh]].rising = p->channel[iCh].isRising; // NOTE!!!! note the trigger_entries
			cfg.trigger[trigger_entries[iCh]].threshold = p->channel[iCh].threshold; // NOTE!!!! note the trigger_entries

			//cout << "[Ch " << iCh << "]" << endl;
			//cout << "precursor: " << cfg.trigger_block[iCh].precursor << endl;
			//cout << "retrigger: " << (int) cfg.trigger_block[iCh].retrigger << endl;
			//cout << "length: " << cfg.trigger_block[iCh].length << endl;
			//cout << "edge: " << (int) cfg.trigger[trigger_entries[iCh]].edge << endl;
			//cout << "rising: " << (int) cfg.trigger[trigger_entries[iCh]].rising << endl;
			//cout << "threshold: " << cfg.trigger[trigger_entries[iCh]].threshold << endl;

		}

		if (ndigo_configure(devices[i], &cfg) != NDIGO_OK) {
			cout << "Fatal configuration of device " << i << " error. Aborting..." << endl;
			throw "[ERROR] DeviceAdc4::configTrigParam - ndigo_configure";
			return;
		}
	}
}

void DeviceAdc4::configCronoSync() {

	// setup 主要用于对多卡同步的 crono_tool 进行初始化
	// 在 crono 同步配置之前，需先完成 ndigo device 配置

	////////////////////////////////////////  配置 crono_device

	for (int i = 0; i < nDevices; i++) {					// L1479
		cro_devices[i].board_id = i;   // cro_devices[0] = 0. Master must be 1st card.
		ndigo_set_board_id(devices[i], i);  // will be packet->card
	}
	cout << "[DONE] ndigo_set_board_id" << endl;

	// 为crono添加设备
	for (int i = 0; i < nDevices; i++) {					// L1709
		cro_devices[i].device_type = CRONO_DEVICE_NDIGO5G;
		cro_devices[i].device = devices[i];
	}

	for (int i = 0; i < nDevices; i++) {			// L1517
		ndigo_fast_info info;
		int result = ndigo_get_fast_info(devices[i], &info); // 这里info包含温度信息
	}
	cout << "[DONE] ndigo_get_fast_info" << endl;

	//////////////////////////////////////// 初始化 crono_sync

	// 为crono初始化结构体指定设备数量、设备指针

	sync_init.device_count = nDevices;					// L1714
	sync_init.devices = cro_devices;

	ndigo_sync = crono_sync_init(&sync_init, &err_code, &err_msg);
	if (!ndigo_sync) {
		throw "[ERROR] DeviceAdc4::configCronoSync - crono_sync_init";
		return;
	}
	cout << "[DONE] ndigo_sync_init" << endl;

	//////////////////////////////////////// 配置 crono sync

	crono_sync_configuration sync_conf;
	int ndigo_status;

	// crono_sync_get_default_configuration(ndigo_sync, &sync_conf);
	sync_conf.trigger_card = 0; // i32TriggerChannel / 6;				// L1798
	sync_conf.trigger_channel = 0; // i32TriggerChannel - sync_conf.trigger_card * 6;
	int dGroupRangeStart = -500; // ns
	int dGroupRangeEnd = +500;   // ns
	sync_conf.group_range_start = __int32(dGroupRangeStart * 1000.);		// L1802
	sync_conf.group_range_end = __int32(dGroupRangeEnd * 1000.);
	ndigo_status = crono_sync_configure(ndigo_sync, &sync_conf, &err_code, &err_msg);
	if (err_code != CRONO_OK) {
		throw "[ERROR] DeviceAdc4::configCronoSync - crono_sync_configure";
		return;
	}

	cout << "Configuration completed." << endl << endl;
	ndigo_param_info param_info[10];

	// ndigo 获得参数信息
	for (int i = 0; i < nDevices; i++)
		ndigo_get_param_info(devices[i], &param_info[i]);

}

void DeviceAdc4::initialize(DeviceItemParams** params, int* err) {
	// 初始化前保证设备处于关闭状态
	if (bOpened)
		finalize();
	try {
		configInitParams(params); // 初始化参数
		configTrigParams(params); // 触发参数
		configCronoSync(); // crono 多卡同步配置
	}
	catch (const char* msg) {		
		cout << msg << endl;
		cout << "err_code: " << err_code << "; err_msg: " << err_msg << endl;
		*err = 1;
		return;
	}
	bOpened = true;
}

void DeviceAdc4::finalize() {
	// 从构造函数中移到此处
	crono_sync_close(ndigo_sync);
	for (int i = 0; i < nDevices; i++)
		ndigo_close(devices[i]);
	bOpened = false;
}



void DeviceAdc4::getParameters() {
	ndigo_param_info param_info;
	for (int i = 0; i < nDevices; i++) {
		ndigo_get_param_info(devices[i], &param_info);
		cout << "Param information of device " << i << " ------------------\n";
		show_device_param_info(&param_info);
		cout << endl;
	}
}




void show_default_init_params(ndigo_init_parameters* params) {
	cout << "\t" << "version: " << params->version << endl;
	cout << "\t" << "card_index: " << params->card_index << endl;
	cout << "\t" << "board_id: " << params->board_id << endl;
	cout << "\t" << "use_external_clock: " << bool(params->use_external_clock) << endl;
	cout << "\t" << "drive_external_clock: " << bool(params->drive_external_clock) << endl;
	cout << "\t" << "is_slave: " << bool(params->is_slave) << endl;
	cout << "\t" << "sync_period: " << params->sync_period << endl;
	cout << "\t" << "sync_delay: " << params->sync_delay << endl;
	cout << "\t" << "force_window_calibration: " << bool(params->force_window_calibration) << endl;
	cout << "\t" << "hptdc_sync_enabled: " << bool(params->hptdc_sync_enabled) << endl;
	cout << "\t" << "multiboard_sync: " << bool(params->multiboard_sync) << endl;
	cout << "\t" << "buffer_size: " << params->buffer_size[0] << endl;
	cout << "\t" << "buffer_type: " << params->buffer_type << endl;
	cout << "\t" << "variant: " << params->variant << endl;
	cout << "\t" << "device_type: " << params->device_type << endl;
}

void show_device_static_info(ndigo_static_info* info) {
	cout << "\t" << "version: " << info->version << endl;
	cout << "\t" << "board_id: " << info->board_id << endl;
	cout << "\t" << "driver_version: " << info->driver_revision << endl;
	cout << "\t" << "firmware_revision: " << info->firmware_revision << endl;
	cout << "\t" << "board_revsion: " << info->board_revision << endl;
	cout << "\t" << "board_configuration: " << info->board_configuration << endl;
	cout << "\t" << "adc_revision: " << info->adc_resolution << endl;
	cout << "\t" << "nominal_sample_rate: " << info->nominal_sample_rate << endl;
	cout << "\t" << "analog_bandwidth: " << info->analog_bandwidth << endl;
	cout << "\t" << "chip_id: " << info->chip_id << endl;
	cout << "\t" << "board_serial: " << info->board_serial << endl;
	cout << "\t" << "flash_serial: (" << info->flash_serial_high << ", " << info->flash_serial_low << ")" << endl;
	cout << "\t" << "flash_valid: " << info->flash_valid << endl;
	cout << "\t" << "dc_coupled: " << bool(info->dc_coupled) << endl;
	cout << "\t" << "subversion_revision: " << info->subversion_revision << endl;
	cout << "\t" << "calibration_date: " << info->calibration_date << endl;
}

void show_device_param_info(ndigo_param_info* info) {
	cout << "\t" << "size: " << info->size << endl;
	cout << "\t" << "version: " << info->version << endl;
	cout << "\t" << "bandwidth: " << info->bandwidth << " Hz" << endl;
	cout << "\t" << "sample_rate: " << info->sample_rate << " samples/s" << endl;
	cout << "\t" << "sample_period: " << info->sample_period << " ps" << endl;
	cout << "\t" << "board_id: " << info->board_id << endl;
	cout << "\t" << "channels: " << info->channels << endl;
	cout << "\t" << "channel_mask: " << info->channel_mask << endl;
	cout << "\t" << "total_buffer: " << info->total_buffer << endl;
}
