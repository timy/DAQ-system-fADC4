//
// Header file containing structs and #defines commonly used by header files of derived CRONO device classes
// The current driver version for Ndigo devices is 1.3.0.0
// The current driver version for xTDC4/TimeTagger4 devices is 1.3.0.0
//

#ifndef CRONO_COMMON_INTERFACE_H
#define CRONO_COMMON_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif


	/** All ERRORS must be positive, because the upper byte is used by crono_tools*/
#define CRONO_OK							0
#define CRONO_WINDRIVER_NOT_FOUND			1
#define CRONO_DEVICE_NOT_FOUND				2
#define CRONO_NOT_INITIALIZED				3
	/* when a capture on a closed card is called */
#define CRONO_WRONG_STATE					4
	/* The pointer given to a xtdc4 driver function was not a valid pointer*/
#define CRONO_INVALID_DEVICE				5
#define CRONO_BUFFER_ALLOC_FAILED			6
#define CRONO_TDC_NO_EDGE_FOUND				7
#define CRONO_INVALID_BUFFER_PARAMETERS		8
#define CRONO_INVALID_CONFIG_PARAMETERS		9
#define CRONO_WINDOW_CALIBRATION_FAILED		10
#define CRONO_HARDWARE_FAILURE				11
#define CRONO_INVALID_ADC_MODE				12
#define CRONO_SYNCHRONIZATION_FAILED		13
#define CRONO_DEVICE_OPEN_FAILED			14
#define CRONO_INTERNAL_ERROR				15

#define crono_bool_t unsigned char

#define CRONO_PACKET_TYPE_16_BIT_SIGNED 1
#define CRONO_PACKET_TYPE_TIMESTAMP_ONLY 128
#define CRONO_PACKET_TYPE_END_OF_BUFFER 129
#define CRONO_PACKET_TYPE_TRIGGER_PATTERN 130

#define CRONO_PACKET_FLAG_SHORTENED 1
#define CRONO_PACKET_FLAG_PACKETS_LOST 2
#define CRONO_PACKET_FLAG_OVERFLOW 4
#define CRONO_PACKET_FLAG_TRIGGER_MISSED 8
#define CRONO_PACKET_FLAG_DMA_FIFO_FULL 16
#define CRONO_PACKET_FLAG_HOST_BUFFER_FULL 32
#define CRONO_PACKET_FLAG_TDC_NO_EDGE 64

	/** Based on PCI Device ID */
#define CRONO_DEVICE_UNKNOWN		0
#define CRONO_DEVICE_HPTDC			1
#define CRONO_DEVICE_NDIGO5G		2
#define CRONO_DEVICE_NDIGO250M		4
#define CRONO_DEVICE_NDIGO_AVRG		3
#define CRONO_DEVICE_XTDC4			6
#define CRONO_DEVICE_FMC_TDC10		7
#define CRONO_DEVICE_TIMETAGGER4	8
#define CRONO_DEVICE_D_AVE12		9
#define CRONO_DEVICE_D_AVE14		10


	/* Contains all user changeable settings*/
	typedef struct {
		/* CRONO_DEVICE_*/
		int device_type;
		/** For HPTDC use this board id, Ndigo uses configured board id*/
		int board_id;
		void * device;
	} crono_device;

	typedef struct {
		unsigned char channel;
		unsigned char card;
		unsigned char type;
		unsigned char flags;
		unsigned int length;
		__int64 timestamp;
		unsigned __int64 data[1];
	} crono_packet;

	typedef struct {
		unsigned char channel;
		unsigned char card;
		unsigned char type;
		unsigned char flags;
		unsigned int length;
		__int64 timestamp;
	} crono_packet_only_timestamp;

#define crono_packet_data_length(current) ((current)->type &128?0:(current)->length)

#define crono_packet_bytes(current) ((crono_packet_data_length(current) + 2) * 8)

#define crono_next_packet(current) ((crono_packet*) (((__int64) (current)) +( ((current)->type&128?0:(current)->length) + 2) * 8))

	/*! \ingroup defread
	*/
#define CRONO_READ_OK 0	//!< no error encountered
	/*! \ingroup defread
	*/
#define CRONO_READ_NO_DATA 1 //!< 
	/*! \ingroup funcerrors
	*/
#define CRONO_READ_INTERNAL_ERROR 2 //!<
	/*! \ingroup defread
	*/
#define CRONO_READ_TIMEOUT 3

#ifdef __cplusplus
}
#endif

#endif
