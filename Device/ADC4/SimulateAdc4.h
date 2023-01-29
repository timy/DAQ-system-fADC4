#pragma once

#ifndef SIMULATE_ADC4_H_
#define SIMULATE_ADC4_H_

//
// Header file containing structs and #defines commonly used by header files of derived CRONO device classes
// The current driver version for Ndigo devices is 1.3.0.0
// The current driver version for xTDC4/TimeTagger4 devices is 1.3.0.0
//

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
		void* device;
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




//
// Header file containing structs and #defines commonly used by header files of derived CRONO device classes
// The current driver version for Ndigo devices is 1.3.0.0
//

#ifdef _MSC_VER
#ifdef CRONO_TOOLS_EXPORTS
#define CRONO_API __declspec(dllexport)
#else
//#define CRONO_API __declspec(dllimport)
#define CRONO_API
#endif
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned __int64 uint64;
typedef __int64 int64;
#else
#include <stdint.h>
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int64_t int64;
#define CRONO_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/*! \defgroup init Initialization
	 *	@{
	 */
	 /*!	\defgroup initfuncts Functions for Initialization
	  * @{
	  */
	  /*! \defgroup defsyncinit #defines for crono_sync_init()
	   */
	   /*!@}*/
	   /*! \defgroup cronosync Structure crono_sync
		*/
		/*! \defgroup cronosyncinitparams crono_sync_init_parameters
		 */
		 /*!@}*/
		 /*!	\defgroup config Configuration
		  *	@{
		  */
		  /*!	\defgroup conffuncts Functions for Configuration
		   *	@{
		   */
		   /*!	\defgroup defsyncconf #defines for crono_sync_configure()
			*/
			/*!@}*/
			/*! \defgroup confstruct Structure crono_sync_configuration
			 */
			 /*!@}*/
			 /*! \defgroup runtime Run Time Control
			  */
			  /*!	\defgroup readout Readout
			   *	@{
			   */
			   /*! \defgroup readfuncts Functions for Readout
				*	@{
				*/
				/*! \defgroup defread #defines for crono_sync_read()
				 */
				 /*!@}*/
				 /*!	\defgroup readin Structure crono_sync_read_in
				  */
				  /*! \defgroup strucreadout Structure crono_sync_read_out
				   */
				   /*!@}*/
				   /*!	\defgroup packettypes Packet Types
					*/
					/*! \defgroup funcerrors Function Errors\
					 */

					 /*! \ingroup defsyncconf
					  *	\brief No card with the matching board id was found in the device list.
					  *
					  *	A trigger must be defined.
					  */
#define CRONO_ERROR_TRIGGER_CARD_NOT_FOUND	5
					  /* The pointer given to a Ndigo driver function was not a valid pointer*/
					  //#define CRONO_INVALID_DEVICE				6
					  /*!	\ingroup defsyncinit
					   *	\brief A Ndigo 5G board must be present
					   */
#define CRONO_SYNC_NDIGO_MASTER_REQUIRED	7
					   /*! \ingroup defsyncconf
						*	\brief Crono Sync has invalid parameters
						*/
#define CRONO_SYNC_INVALID_CONFIG			100

						/*!	\ingroup packettypes
						 *	@{
						 */
#define CRONO_PACKET_TYPE_8_BIT_SIGNED		  0 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_16_BIT_SIGNED		  1 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_32_BIT_SIGNED		  2 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_64_BIT_SIGNED		  3 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_8_BIT_UNSIGNED	  4 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_16_BIT_UNSIGNED	  5 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_32_BIT_UNSIGNED	  6 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_64_BIT_UNSIGNED	  7 //!< NOT IMPLEMENTED
#define CRONO_PACKET_TYPE_TDC_DATA			  8 //!< NOT IMPLEMENTED
						 /*!*/
#define CRONO_PACKET_TYPE_TIMESTAMP_ONLY	128
/*!*/
#define CRONO_PACKET_TYPE_END_OF_BUFFER		129
/*!*/
#define CRONO_PACKET_TYPE_TRIGGER_PATTERN	130
/*!*/
#define CRONO_PACKET_TYPE_TDC_RISING		144
/*!*/
#define CRONO_PACKET_TYPE_TDC_FALLING		145
/*!@}*/




/*! \ingroup cronosync
 */
	typedef struct {
		/*!*/
		void* sync;
	} crono_sync;

	/*! \ingroup cronosyncinitparams
	 *	\brief struct for the initialization of synchronized ndigo boards
	 *
	 *  The master card must be the first device!
	 *	this structure MUST be completely INITIALIZED.
	 *  For the HPTDCs the board_id field in the crono_device structure MUST be filled.
	 *  It is ignored for all other devices.
	 */
	typedef struct {
		/*!*/
		int device_count;
		/*!*/
		crono_device* devices;

	} crono_sync_init_parameters;


	/*! \ingroup initfuncts
	 *	\brief init the card for the given client
	 *
	 *	for the version number, use CRONO_API_VERSION (not implemented?!)
	 */
	CRONO_API crono_sync* crono_sync_init(crono_sync_init_parameters* params, int* error_code, const char** error_message);

	/*! \ingroup confstruct
	 *
	 *	max. amount of packets in a group is 10000
	 *	- -1ms < group_range_start < 1ms
	 *	- -1ms < group_range_end < 1ms
	 *
	 *	the trigger packet is always included in the group even if group_range_start > 0.
	*/
	typedef struct {
		/*!	\brief contains TDC channel settings
		 */
		int trigger_channel;
		/*!	\brief the board id of the card
		 */
		int trigger_card;
		/*!*/
		int64 group_range_start;
		/*!*/
		int64 group_range_end;
	} crono_sync_configuration;

	/*! \ingroup defsyncconf
	 * @{
	 */
#define CRONO_SYNC_GROUP_RANGE_MAX 5000000000LL
#define CRONO_SYNC_GROUP_RANGE_MIN -5000000000LL
	 /*!@}*/

	 /*!	\ingroup conffuncts
	  *	\brief Sets up the synchronization between cards
	  *
	  *	Setup the syncing does not call any functions on the cards!, may return the following @link defsyncconf errors @endlink
	  */
	CRONO_API int crono_sync_configure(crono_sync* sync, crono_sync_configuration* config, int* error_code, const char** error_message);

	/*! \ingroup initfuncts
	 *	\brief Closes and frees the synchronization objects
	 *
	 *	does NOT close the devices!
	*/
	CRONO_API int crono_sync_close(crono_sync* device);

	/*! \ingroup runtime
	 *	\brief Starts capture on the cards
	 *
	 *	the return code is either CRONO_OK (0) or the index of the failed device in the top byte(32bit) and the error code in the remaining bytes.
	*/
	CRONO_API int crono_sync_start_capture(crono_sync* device);

	/*! \ingroup runtime
	 *	\brief Stops capture on the boards
	 */
	CRONO_API int crono_sync_stop_capture(crono_sync* device);

	/*!	\ingroup readfuncts
	 *	\brief returns the last error message
	 *
	 *	encountered if one of the cards returns an error code
	 */
	CRONO_API const char* crono_sync_get_last_error_message(crono_sync* device);


	//CRONO_API int crono_sync_pause_capture(crono_sync *device);

	//CRONO_API int crono_sync_continue_capture(crono_device *device);


	// default is 10 milliseconds
	static const int CRONO_DEFAULT_READ_TIMEOUT = 10;


	/*!	\ingroup readin
	 *	\brief The parameters of the read commands
	 */
	struct crono_sync_read_in {
		int max_groups;
		/*!	\brief timeout is in milliseconds
		 */
		int timeout_limit;
		crono_sync_read_in() : max_groups(1), timeout_limit(CRONO_DEFAULT_READ_TIMEOUT) {}
	};


	typedef struct {
		int trigger_index;
		int flags;
		int64 timestamp;
		int packet_count;
		// array of pointers to packets
		crono_packet** packets;
	} crono_group;

	/*! \ingroup strucreadout
	 *	\brief parameters for readout
	 */
	typedef struct {
		int group_count;
		//! \brief array of pointers to packets
		crono_group* groups;
		int error_code;
		const char* error_message;
	} crono_sync_read_out;

	/*!	\ingroup readfuncts
	 *	\brief	Reads packets from the board
	 *
	 *	automatically acknowledges the last read packets
	 */
	CRONO_API int crono_sync_read(crono_sync* device, crono_sync_read_in* in, crono_sync_read_out* out);


	/*!	\ingroup readfuncts
	 *	\brief acknowledges the processing of the last read block
	 *
	 *	this is only necessary if read is not called
	 */
	CRONO_API int crono_sync_acknowledge(crono_sync* device);

	/*!	\ingroup initfuncts
	 *	\brief initializes all boards involved
	 *
	 *	Initializes primarily the first ndigo (master) and then all slaves simultaneously.
	 */
	CRONO_API int ndigo_init_multiple(void* init_params, int count, crono_device** ndigo, int* error_code, const char** error_message);

#ifdef __cplusplus
}
#endif









//
// Header file containing structs and #defines commonly used by header files of derived Ndigo device classes
// The current driver version for Ndigo devices is 1.3.0.0
//
/*! \defgroup constants Constants
 */
 /*!	\defgroup initialization Initialization
  *@{
  */
  /*! \defgroup initfuncts Functions for Initialization
   */
   /*!	\defgroup initparamsstruct Structure ndigo_init_parameters
	* \brief struct for the instantiation of the Ndigo board
	*
	* this structure MUST be completely INITIALIZED
	*@{
	*/
	/*! \defgroup defversion defines for version
	 *	\brief defines for @link ndigo_init_parameters version @endlink
	 */
	 /*! \defgroup bufferdef defines for buffer_type
	  * \brief defines for @link ndigo_init_parameters buffer_type @endlink
	  */
	  /*!@}*/
  /*!@}*/
  /*!	\defgroup status Status Information
   *	@{
   */
   /*! \defgroup statfuncts Functions for Information Retrieval
	*	\brief Functions for getting detailed information about the xtdc4 board.
	*
	*	The driver provides functions to retrieve detailed information on the type of board,
	*	it's configuration, settings and state. The information is split according to its scope
	*	and the computational requirements to query the information from the board.
	*/
	/*!	\defgroup staticinfo Structure ndigo_static_info
	 *	\brief Structure contains static information
	 *
	 *	This structure contains information about the board that does not change during run time.
	 *	It is provided by the function @link statfuncts xtdc4_get_static_info() @endlink.
	 */
	 /*! \defgroup paraminfo Structure ndigo_param_info
	  *	\brief contains configuration changes
	  *
	  *	Structure filled by @link statfuncts xtdc4_get_param_info() @endlink. This structure
	  *	contains information that change indirectly due to configuration changes.
	  */
	  /*!	\defgroup fastinfo Structure ndigo_fast_info
	   *	\brief contains fast dynamic information
	   *
	   *	This call returns a structure that contains dynamic information that can be obtained
	   *	within a few microseconds.
	   *@{
	   */
	   /*! \defgroup alertdefs #defines for alerts
		*	\brief Alert bits from the system monitor
		*/
		/*@}*/
		/*!	\defgroup slowinfo Structure ndigo_slow_info
		 *	\brief contains slow dynamic information
		 *
		 *	The data reported in this structure requires milliseconds to be obtained. The
		 *	application should only call it in situation where the program flow can cope with an
		 *	interruption of that magnitude.
		 */
		 /*!@}*/
		 /*! \defgroup config Configuration
		  * \brief The device is configured with a configuration structure
		  *
		  * The user should first obtain a structure that contains the default settings of the device
		  * read from an on board ROM, then modify the structure as needed for the user application
		  * and use the result to configure the device.
		  * @{
		  */
		  /*! \defgroup conffuncts Functions for Configuration
		   */
		   /*! \defgroup ndigotrg Structure ndigo_trigger
			*  \brief contains trigger settings
			*/
			/*! \defgroup devicestruct Structure ndigo_device
			 *	\brief contains information of the device in use
			 */
			 /*! \defgroup ndigotrgblock Structure ndigo_trigger_block
			  *	\brief configuration of the trigger block
			  *@{
			  */
			  /*! \defgroup precursordef #define for precursor
			   */
			   /*! \defgroup multishotdef #define for multi_shot_count
				*/
				/*! \defgroup sourcedefs #defines for sources
				 *	\brief bit mask with a bit set for attribute sources in @link ndigo_trigger_block
				 *	ndigo_trigger_block @endlink and @link ndigo_gating_block ndigo_gating_block @endlink
				 *
				 *	also used by @link ndigogat ndigo_gating_block @endlink
				 */
				 /*! \defgroup gatedefs #defines for gates
				  *	\brief bit mask with a bit set for all trigger @link ndigo_trigger_block gates @endlink
				  */
				  /*!@}*/
				  /*! \defgroup ndigogat Structure ndigo_gating_block
				   *	\brief contains settings of the gating block
				   */
				   /*! \defgroup ndigoext Structure ndigo_extension_block
					* \brief configures the optional extension board.
					*
					* This structure configures the merging of the inputs from the synchronization bus and the
					* optional extension board.
					*/
					/*!@}*/
					/*! \defgroup runtime Run Time Control
					 *	\brief Controls data acquisition throughout run time
					 */
					 /*! \defgroup readout Readout
					  *@{
					  */
					  /*! \defgroup readfuncts Functions for readout
					   *@{
					   */
					   /*! \defgroup readerror Errors for readout functions
						*/
						/*!@}*/
					/*!@}*/
					/*! \defgroup others Other Functions
					 *@{
					 */
					 /*! \defgroup led LED control
					  *	\brief LED control functions
					  *
					  *	There are six LEDs on the front panel. The intensity of the red and green part can be
					  *	set from 0 to 255. There is no blue component in the current version. Per default all
					  *	LEDs are set to auto mode. This means that used channels are lit green, activity is
					  *	shown as yellow and overflow is shown as red.
					  */
					  /*!@}*/
					  /*! \defgroup packformat Packet Format
					   *@{
					   */
					   /*! \defgroup ndigopack Output Structure ndigo_packet
						*	\brief contains all packet information
						*@{
						*/
						/*! \defgroup flagdefs #defines for flags
						 *	\brief flags of the packet reporting error conditions
						 */
						 /*! \defgroup typedefs #defines for type
						  *	\brief data format of @link ndigopack ndigo_packet @endlink
						  */
						  /*!@}*/
					  /*!@}*/
					  /*!	\defgroup funcerrors Function Errors
					   *
					   *	All ERRORS must be positive, because the upper byte is used by crono_tools
					   *@{
					   */
					   /*! \defgroup errorfuncts Error functions
						*/
						/*!@}*/

						/*!\ingroup defversion
						 *	\brief definitions for @link ndigo_init_parameters version @endlink
						 */
#define NDIGO_API_VERSION 0

						 /*! \ingroup triggerdefs
						  * @{
						  */
						  /*!	\brief the number of triggers.
						   *
						   * Two per analog input, one per digital input plus some specials.
						   */
#define NDIGO_TRIGGER_COUNT 16	
#define NDIGO_ADD_TRIGGER_COUNT 6 //!< additional set of triggers for digital inputs
						   /*!@}*/

						   /*!\ingroup constants
							*@{
							*/
							/*!\brief permitted ADC value
							*
							* RAW mode should not be used by the customer.
							*/
#define NDIGO_RAW_VALUE_MIN		0
							/*!\brief permitted ADC value
							*
							* RAW mode should not be used by the customer.
							*/
#define NDIGO_RAW_VALUE_ZERO	512
							/*!\brief permitted ADC value
							*
							* RAW mode should not be used by the customer.
							*/
#define NDIGO_RAW_VALUE_MAX		1023
							/*!\brief permitted ADC value
							*
							* RAW mode should not be used by the customer.
							*/
#define NDIGO_RAW_VALUE_COUNT	1024
							/*!@}*/

							/*!	\ingroup funcerrors
							 *@{
							 */
							 /*! \brief no error encountered
							  */
#define NDIGO_OK							0
#define NDIGO_WINDRIVER_NOT_FOUND			1
#define NDIGO_DEVICE_NOT_FOUND				2
#define NDIGO_NOT_INITIALIZED				3
#define NDIGO_WRONG_STATE					4	//!< when a capture on a closed card is called
#define NDIGO_INVALID_DEVICE				5	//!< The pointer given to a Ndigo driver function was not a valid pointer
#define NDIGO_BUFFER_ALLOC_FAILED			6
#define NDIGO_TDC_NO_EDGE_FOUND				7
#define NDIGO_INVALID_BUFFER_PARAMETERS		8
#define NDIGO_INVALID_CONFIG_PARAMETERS		9
#define NDIGO_WINDOW_CALIBRATION_FAILED		10
#define NDIGO_HARDWARE_FAILURE				11
#define NDIGO_INVALID_ADC_MODE				12
#define NDIGO_SYNCHRONIZATION_FAILED		13
#define NDIGO_DEVICE_OPEN_FAILED			14
#define NDIGO_INTERNAL_ERROR				15
							  /*!@}*/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#ifdef NDIGO_DRIVER_EXPORTS
#define NDIGO_API __declspec(dllexport)
#else
//#define NDIGO_API __declspec(dllimport)
#define NDIGO_API
#endif
#else
#define __int64 long long
#define NDIGO_API 
#endif

	/*!	\ingroup initialization
	 *	\brief universally used boolean
	 */
#define ndigo_bool_t unsigned char

	 /*! \ingroup flagdefs
	  *	\brief less than the requested number of samples have been written due to a full FIFO.
	  */
#define NDIGO_PACKET_FLAG_SHORTENED			 1
	  /*! \ingroup flagdefs
	   *	\brief the DMA controller has discarded packets due to full host buffer
	   */
#define NDIGO_PACKET_FLAG_PACKETS_LOST		 2
	   /*! \ingroup flagdefs
		* \brief the previous packet included ADC samples with overflows
		*/
#define NDIGO_PACKET_FLAG_OVERFLOW			 4
		/*! \ingroup flagdefs
		 *	\brief the trigger unit has discarded packets due to full FIFO
		 */
#define NDIGO_PACKET_FLAG_TRIGGER_MISSED	 8
		 /*! \ingroup flagdefs
		  *	\brief DMA FIFO was full, might not result in dropped packets
		  */
#define NDIGO_PACKET_FLAG_DMA_FIFO_FULL		16
		  /*! \ingroup flagdefs
		   *	\brief Host buffer was full, might not result in dropped packets
		   */
#define NDIGO_PACKET_FLAG_HOST_BUFFER_FULL	32
		   /*! \ingroup flagdefs
			* \brief no valid edge was found in TDC packet, timestamp uncorrected
			*/
#define NDIGO_PACKET_FLAG_TDC_NO_EDGE		64

			/*! \ingroup typedefs
			 *	\brief 8bit signes samples from one of the ADCs
			 */
#define NDIGO_PACKET_TYPE_8_BIT_SIGNED		  0

			 /*! \ingroup typedefs
			  *	\brief 16bit signed samples from one of the ADCs
			  */
#define NDIGO_PACKET_TYPE_16_BIT_SIGNED		  1

			  /*! \ingroup typedefs
			   * @{
			   */
#define NDIGO_PACKET_TYPE_32_BIT_SIGNED		  2
#define NDIGO_PACKET_TYPE_64_BIT_SIGNED		  3
#define NDIGO_PACKET_TYPE_8_BIT_UNSIGNED	  4
#define NDIGO_PACKET_TYPE_16_BIT_UNSIGNED	  5
#define NDIGO_PACKET_TYPE_32_BIT_UNSIGNED	  6
#define NDIGO_PACKET_TYPE_64_BIT_UNSIGNED	  7
			   /*!@}*/

			   /*! \ingroup typedefs
				*	\brief 64bit unsigned TDC Data
				*
				* only for internal processing.
				*/
#define NDIGO_PACKET_TYPE_TDC_DATA			  8

				/*! \ingroup typedefs
				 *	\brief no data is present - may contain other data
				 */
#define NDIGO_PACKET_TYPE_TIMESTAMP_ONLY	128

				 /*! \ingroup typedefs
				  *	\brief no data is present - may contain other data
				  */
#define NDIGO_PACKET_TYPE_END_OF_BUFFER		129

				  /*! \ingroup ndigopack
				   *	\brief contains all packet information
				   */
	struct ndigo_packet {
		/*! \brief enumeration of channels
		 *
		 *	0 to 3 for the ADC input channels, 4 for the TDC, 5 for the timestamp channel.
		 */
		unsigned char channel;

		/*! \brief identifies card among others
		 *
		 * Identifies the source card in case there are multiple boards present. Defaults to
		 * 0 if no value is assigned to the parameter
		 * @link initparamsstruct board_id in Structure ndigo_init_parameters @endlink or set
		 * via @link conffuncts int ndigo_set_board_id(ndigo_device * device, int board_id) @endlink
		 */
		unsigned char card;

		/*! \brief sets @link typedefs data type @endlink
		 *
		 *	For the ADC channels this is set to 1 to signify 16bit signed data.
		 *	For the TDC channel it is set to 8 to signify 64bit unsigned data - only for internal
		 *	processing.
		 *	If the type field is 128 or greater then there is no data present, even if length is
		 *	not 0. In this cases the length field may contain other data.
		 */
		unsigned char type;

		/*! \brief contains flags for the packet
		 *
		 *	flags are defined in @link flagdefs NDIGO_PACKET_FLAG_* @endlink
		 */
		unsigned char flags;

		/*! \brief Number of 64-bit elements (each containing 4 samples) in the data array < 128.
		 *
		 *	If type = 128 this is the pattern of trigger sources that where active in the clock cycle
		 *	given by the timestamp. Bits are set according to the trigger sources, i.e. bit 0 is set
		 *	if trigger A0 was active, bit 29 is set if trigger @link sourcedefs BUS3_PE @endlink was active. Use the
		 *	@link sourcedefs NDIGO_TRIGGER_SOURCE_* @endlink defines to check for the bits set.
		 */
		unsigned int length;

		/*! \brief Timestamp in ps.
		 *
		 *	ADC channels A to D: timestamp of the last word in the packet.
		 *	TDC: timestamp of the trigger event (falling edge) of the TDC channel. When
		 *	@link readfuncts ndigo_process_tdc_packet()@endlink is called once on the packet the timestamp is replaced with the
		 *	precise timestamp for the edge.
		 *	Timestamp channel: timestamp of the trigger event.
		 */
		unsigned __int64 timestamp;

		/*! \brief sample data
		 *
		 * For the Ndigo5G each 64 bit word vontains four 16bit signed words from the ADC. The user can
		 * cast the array to short* to directly operate on the sample data.
		 */
		unsigned __int64 data[1];
	};

#define ndigo_packet_data_length(current) ((current)->type &128?0:(current)->length)

#define ndigo_packet_bytes(current) ((ndigo_packet_data_length(current) + 2) * 8)

#define ndigo_next_packet(current) ((ndigo_packet*) (((__int64) (current)) +( ((current)->type&128?0:(current)->length) + 2) * 8))

	/*! \ingroup devicestruct
	 *	\brief contains information on the current device.
	 */
	typedef struct {
		void* ndigo;
	} ndigo_device;

	/*!\ingroup ndigotrg
	 * brief contains trigger settings
	 */
	typedef struct {
		/*! \brief Threshold of both triggers in trigger block ADC-counts.
		 *
		 * Sets the threshold for the trigger block within the range of the ADC data of -32768 and +32768.
		 *
		 * For trigger indices @link triggerdefs NDIGO_TRIGGER_TDC @endlink to @link triggerdefs NDIGO_TRIGGER_BUS3_PE @endlink the threshold is ignored.
		 *
		 *  Valid values depend on output mode:
		 *  - @link outputdefs NDIGO_OUTPUT_MODE_RAW @endlink : 0 to 1023
		 *  - @link outputdefs NDIGO_OUTPUT_MODE_SIGNED16 @endlink : -32768 to +32767
		 */
		short threshold;

		/*! \brief sets edge trigger functionality
		 *
		 * else this is a level trigger:
		 *	- 0: level trigger
		 *	- 1: edge trigger
		 *
		 * For trigger indices @link triggerdefs NDIGO_TRIGGER_AUTO @endlink and @link triggerdefs NDIGO_TRIGGER_ONE @endlink this is ignored
		 * For trigger indices @link triggerdefs NDIGO_TRIGGER_TDC_PE @endlink to @link triggerdefs NDIGO_TRIGGER_BUS3_PE @endlink this must be set.
		 *
		 * The edge trigger triggers as soon as its set threshold is crossed by the signal. Thus the roots in
		 * reference to the threshold are recorded. The level trigger triggers as long as the signal is above or
		 * below the set threshold. Followingly the trigger gives the sign of the signal in reference to the
		 * threshold.
		 */
		ndigo_bool_t edge;
		/*! \brief sets rising edge trigger functionality
		 *
		 * if set, triggers on rising edges or when threshold is exceeded:
		 * 0 - falling edge
		 * 1 - rising edge
		 *
		 * for the level trigger:
		 * 0 - triggers parts of the signal above the threshold
		 * 1 - triggers parts of the signal below the threshold
		 *
		 * For trigger indices @link triggerdefs NDIGO_TRIGGER_AUTO @endlink and @link triggerdefs NDIOG_TRIGGER_ONE @endlink this is ignored
		 * For trigger indices @link triggerdefs NDIGO_TRIGGER_TDC_PE @endlink to @link triggerdefs NDIGO_TRIGGER_BUS3_PE @endlink this must be set.
		 */
		ndigo_bool_t rising;
	} ndigo_trigger;

	/*!\ingroup sourcedefs
	 *@{
	 */
#define NDIGO_TRIGGER_SOURCE_NONE		0x00000000
#define NDIGO_TRIGGER_SOURCE_A0			0x00000001
#define NDIGO_TRIGGER_SOURCE_A1			0x00000002
#define NDIGO_TRIGGER_SOURCE_B0			0x00000004
#define NDIGO_TRIGGER_SOURCE_B1			0x00000008
#define NDIGO_TRIGGER_SOURCE_C0			0x00000010
#define NDIGO_TRIGGER_SOURCE_C1			0x00000020
#define NDIGO_TRIGGER_SOURCE_D0			0x00000040
#define NDIGO_TRIGGER_SOURCE_D1			0x00000080
#define NDIGO_TRIGGER_SOURCE_TDC		0x00000100
#define NDIGO_TRIGGER_SOURCE_GATE		0x00000200
#define NDIGO_TRIGGER_SOURCE_BUS0		0x00000400
#define NDIGO_TRIGGER_SOURCE_BUS1		0x00000800
#define NDIGO_TRIGGER_SOURCE_BUS2		0x00001000
#define NDIGO_TRIGGER_SOURCE_BUS3		0x00002000
#define NDIGO_TRIGGER_SOURCE_AUTO		0x00004000
#define NDIGO_TRIGGER_SOURCE_ONE		0x00008000

	 /*! \brief additional digital trigger sources
	  *
	  * always positive edge triggered in hardware
	  */
#define NDIGO_TRIGGER_SOURCE_TDC_PE		0x01000000
#define NDIGO_TRIGGER_SOURCE_GATE_PE	0x02000000
#define NDIGO_TRIGGER_SOURCE_BUS0_PE	0x04000000
#define NDIGO_TRIGGER_SOURCE_BUS1_PE	0x08000000
#define NDIGO_TRIGGER_SOURCE_BUS2_PE	0x10000000
#define NDIGO_TRIGGER_SOURCE_BUS3_PE	0x20000000
	  /*!@}*/

	  /*!\ingroup gatedefs
	   *@{
	   */
#define NDIGO_TRIGGER_GATE_NONE			0x0000
#define NDIGO_TRIGGER_GATE_0			0x0001
#define NDIGO_TRIGGER_GATE_1 			0x0002
#define NDIGO_TRIGGER_GATE_2 			0x0004
#define NDIGO_TRIGGER_GATE_3 			0x0008
	   /*!@}*/

	   /*! \ingroup bufferdef
		*@{
		*/
#define NDIGO_BUFFER_ALLOCATE 0
#define NDIGO_BUFFER_USE_PHYSICAL 1
#define NDIGO_BUFFER_USE_PREALLOCATED 2
		/*!@}*/

		/*! \ingroup initparamsstruct
		 * \brief struct for the initialization of the Ndigo board
		 *
		 * this structure MUST be completely INITIALIZED
		 */
	typedef struct {
		/*! \brief A version number
		 *
		 * that is increased when the definition of the structure is changed. The increment can be larger than
		 * one to match driver version numbers or similar. Set to 0 for all versions up to first release. Must
		 * be set to @link defversion NDIGO_API_VERSION @endlink
		 */
		int version;

		/*! \brief The index in the list of Ndigo boards that should be initialized.
		 *
		 * There might be multiple boards in the system that are handled by this
		 * driver as reported by ndigo count devices. This index selects one of them. Boards are enumerated
		 * depending on the PCIe slot. The lower the bus number and the lower the slot number the lower the
		 * card index.
		 */
		int card_index;

		/*! \brief This 8bit number is filled into each packet created by the board
		 *
		 * and is useful if data streams of	multiple boards will be merged. If only Ndigo250M-14 cards are used
		 * this number can be set to the card index. If boards of different types that use a compatible data
		 * format are used in a system each board should get a unique id. Can be changed with int
		 * @link conffuncts ndigo_set_board_id(ndigo_device *device, int board_id) @endlink
		 */
		int board_id;

		/*! \brief Use 10MHz clock supplied by IPC at band cable.
		 *
		 * Must be set for all slaves when synchronizing multiple boards.
		 */
		ndigo_bool_t use_external_clock;

		/*! \brief Drive internal 10MHz clock of this board to IPC at band cable.
		 *
		 * Must be set for master when synchronizing multiple boards
		 */
		ndigo_bool_t drive_external_clock;

		/*! \brief Data acquisition of this board is controlled by the master board.
		 */
		ndigo_bool_t is_slave;

		/*! \brief Set whether board is used in a multiple boards environment.
		 */
		ndigo_bool_t multiboard_sync;

		/*! \brief If >0 the direct sync method is used for multi board applications.
		 *
		 * Value corresponds to accuracy (higher values for more accurate phase match.
		 */
		int direct_sync;

		/*! \brief Period of the multicard sync pulse.
		 *
		 * Ignored for single board setups. Should be set to 4 if only
		 * Ndigo5G and Ndigo250M boards are present in the system. If other boards are synchronized
		 * the correct value is the lowest common denominator of the value of all boards.
		 */
		int sync_period;

		/*! \brief Fine tap delay for incoming sync signals.
		 *
		 * Ignored for single board setups.
		 */
		int sync_delay;

		/*! \brief Fine tap delay for sync signals.
		 *
		 * Ignored for single board setups
		 */
		int sync_delay_master;

		/*! /brief window calibration
		 *
		 * If true/1, a valid data window is automatically detected at initialization.
		 * Default value is false/0: values from flash memory are used in order to set the data window to
		 * correct position
		 */
		ndigo_bool_t force_window_calibration;

		/*! \brief enables/disables	the CLK and sync line for the HPTDC
		 *
		 *	Must be set to false. If an HPTDC is connected to the board (valid for NDigo5G) this
		 * enables/disables the CLK and sync line for the HPTDC
		 */
		ndigo_bool_t hptdc_sync_enabled;

		/*! \brief The minimum size of the DMA buffer.
		 *
		 * If set to 0 the default size of 16MByte is used.	Ndigo5G only uses buffer_size[0]. Ndigo250M uses
		 * 6 entries.
		 */
		__int64 buffer_size[8];

		/*!	\brief Type of buffer
		 *
		 * either allocated @link bufferdef NDIGO_BUFFER_ALLOCATE, NDIGO_BUFFER_USE_PREALLOCATED @endlink (relevant for 250M on Windows)
		 * or @link bufferdef NDIGO_BUFFER_USE_PHYSICAL @endlink (supported on linux). For Ndigo5G must be set to D125_BUFFER_ALLOCATE.
		 * For Ndigo250M must be set to	NDIGO_BUFFER_ALLOCATE.
		 */
		int buffer_type;

		/*! \brief The start of the reserved memory
		 *
		 * the buffers will be allocated with the sizes given above, make sure that the memory is large enough.
		 * For NDIGO_BUFFER_USE_PHYSICAL, the address of the preallocated buffer. Ignored. Might be used for
		 * future buffer types.
		 */
		__int64 buffer_address;

		/*! \brief the size of the preallocated buffer
		 *
		 * for @link bufferdef NDIGO_BUFFER_USE_PREALLOCATED @endlink
		 */
		unsigned int preallocated_buffer_size;

		/*! \brief Can be used to activate future device variants such as different base frequencies.
		 *
		 * Set to 0.
		 */
		int variant;

		/*! \brief A constant for the different devices from cronologic CRONO_DEVICE_*.
		 *
		 * Initialized by @link initfuncts ndigo_get_default_init_parameters() @endlink. Must be left unchanged.
		 */
		int device_type;

		/*! \brief The write pointer is delayed by this number 4n clock periods.
		 *
		 * Initialized by @link initfuncts ndigo_get_default_init_parameters() @endlink. The write pointer updated is delayed by this
		 * number of 4n clock periods to hide race conditions between software and DMA.
		 */
		int dma_read_delay;

	} ndigo_init_parameters;

	/*! \ingroup ndigotrgblock
	 *	\brief configuration of the trigger block
	 */
	typedef struct {
		ndigo_bool_t enabled; //!< Activates triggers on this channel.

		ndigo_bool_t force_led; //!< Activates LED on input even if trigger block is not enabled. Required for LED activity in Multi/SingleShot mode.

		/*! \brief enable retrigger
		 *
		 * If a new trigger condition occurs while the postcursor is acquired the packet is extended by
		 * starting a new postcursor. Otherwise the new trigger is ignored and the packet ends after the
		 * precursor of the first trigger. The retrigger setting is ignored for the timestamp channel.
		 */
		ndigo_bool_t retrigger;
		ndigo_bool_t reserved1; //!< Defaults to false. Do not change
		ndigo_bool_t reserved2; //!< Defaults to false. Do not change

		/*! \brief Number of packets created in single shot mode before packet generation stops. Maximum is
		 *	@link multishotdef NDIGO_MAX_MULTISHOT @endlink
		 */
		int multi_shot_count;

		/*! \brief Precursor in multiples of 3.2ns.
		 * The amount of data preceding a trigger that is captured. The	precursor setting is ignored for the
		 * timestamp channel. Maximum is set by @link precursordef NDIGO_MAX_PRECURSOR @endlink.
		 */
		int precursor;

		/*! \brief Length in multiples of 3.2ns.
		 *
		 * The total amount of data that is recorded in addition to the trigger window.	Precursor determines
		 * how many of these are ahead of the trigger and how many are appended after the trigger. In edge
		 * trigger mode the trigger window always is 3.2ns wide, in level trigger mode it is as long as the
		 * triger condition is fulfilled. The length setting is ignored for the timestamp channel.
		 */
		int length;

		/*!	\brief A @link sourcedefs bit mask @endlink with a bit set for all trigger sources that can trigger this channel.
		 *
		 * Default @link sourcedefs NDIGO_TRIGGER_SOURCE_x @endlink (x = 0 for index 0 etc)
		 */
		int sources;

		/*! \brief A @link gatedefs bit mask @endlink with a bit set for all trigger gates
		 *
		 * Mask which selects the gates that have to be 1 for the trigger block to use Default
		 * @link gatedefs NDIGO_TRIGGER_GATE_NONE @endlink
		 */
		int gates;

		/*! \brief number of packets that fit into the FIFO
		 *
		 * This parameter sets how many packets are supposed to fit into the on-board FIFO before a new	packet
		 * is recorded after the FIFO was full, i.e. a certain amount of free space in the FIFO is demanded
		 * before a new packet is written after the FIFO was full. As a measure for the packet length the
		 * gatelength set by the user is used. The on-board algorithm checks the free FIFO space only in case
		 * the FIFO is full. Therefore, if this number is 1.0 or more at least every second packet in the DMA
		 * buffer is guaranteed to have the full length set by the gatelength parameters. In many cases smaller
		 * values will also result in full length packets. But below a certain value multiple packets that are
		 * cut off at the end will show up.
		 */
		double minimum_free_packets;
	} ndigo_trigger_block;

	/*! \ingroup ndigogat Structure ndigo_gating_block
	 *	\brief contains settings of the gating block
	 */
	typedef struct {
		ndigo_bool_t negate; //!< Invert output polarity. Defaults to false.

		/*!\brief enable retrigger
		 *
		 * Defaults to false. If retriggering is enabled the timer is reset to the value of the start
		 * parameter, whenever the input signal is set while waiting to	reach the stop time.
		 */
		ndigo_bool_t retrigger;

		/*! \brief NOT IMPLEMENTED!
		 *
		 * Defaults to true. If set, a gate is created with the set timing from the first occurrence of the
		 * input trigger even for short gates. If not set, the input signal must persist for the gate to be
		 * created.
		 */
		ndigo_bool_t extend;
		ndigo_bool_t reserved1; //!< Defaults to false. Do not change.

		/*! \brief The time from the first input signal seen in the idle state until the gating output is set.
		 *
		 * In multiples of 3.2ns. Start needs to be >=0 and <2^16, start must be set to a value <=stop.
		 */
		int start;

		/*! \brief The number of samples from leaving the idle state until the gating output is reset.
		 *
		 * If retriggering is enabled the timer is reset to the value of the start parameter whenever the input
		 * signal is set while waiting to reach the stop time. In multiples of 3.2ns. Stop needs to be >=0 and <2^16.
		 */
		int stop;

		/*! \brief @link sourcedefs bit mask @endlink with a bit set for all trigger sources that can trigger this channel.
		 *
		 * The gates cannot use the additional digital trigger sources @link sourcedefs NDIGO_TRIGGER_SOURCE_TDC_PE  @endlink to
		 * @link sourcedefs NDIGO_TRIGGER_SOURCE_BUS3_PE @endlink. Default is @link sourcedefs NDIGO_TRIGGER_SOURCE_x0 @endlink (0 for index 0 etc).
		 */
		int sources;

	} ndigo_gating_block;

	/*! \ingroup ndigoext
	 * \brief configures the optional extension board.
	 *
	 * This structure configures the merging of the inputs from the synchronization bus and the
	 * optional extension board.
	 */
	typedef struct {
		/*! \brief Enables routing of digital signal
		 *
		 * from Ndigo extension board to the according BUSx trigger unit.
		 */
		ndigo_bool_t enable;

		/*! \brief if set only the signal from Ndigo extension board is used.
		 *
		 * If false input signal and BUS signal are read before routing to the according BUSx trigger unit.
		 */
		ndigo_bool_t ignore_cable;
	} ndigo_extension_block; /* not used for Ndigo250M. */

	/*! \ingroup triggerdefs
	 *@{
	 */
#define NDIGO_TRIGGER_A0		 0
#define NDIGO_TRIGGER_A1		 1
#define NDIGO_TRIGGER_B0		 2
#define NDIGO_TRIGGER_B1		 3
#define NDIGO_TRIGGER_C0		 4
#define NDIGO_TRIGGER_C1		 5
#define NDIGO_TRIGGER_D0		 6
#define NDIGO_TRIGGER_D1		 7
#define NDIGO_TRIGGER_TDC		 8
#define NDIGO_TRIGGER_GATE		 9
#define NDIGO_TRIGGER_BUS0		10
#define NDIGO_TRIGGER_BUS1		11
#define NDIGO_TRIGGER_BUS2		12
#define NDIGO_TRIGGER_BUS3		13
#define NDIGO_TRIGGER_AUTO		14
#define NDIGO_TRIGGER_ONE		15
	 /*!@}*/

	 /*! \ingroup triggerdefs
	  *	\brief additional digital trigger sources
	  *
	  * always positive edge triggered in hardware
	  */
#define NDIGO_TRIGGER_TDC_PE	16

	  /*! \ingroup triggerdefs
	   *@{
	   */
#define NDIGO_TRIGGER_GATE_PE	17
#define NDIGO_TRIGGER_BUS0_PE	18
#define NDIGO_TRIGGER_BUS1_PE	19
#define NDIGO_TRIGGER_BUS2_PE	20
#define NDIGO_TRIGGER_BUS3_PE	21
	   /*!@}*/

	   /*! \ingroup initfuncts
		* \brief Finalize the driver for this device.
		*
		*	\param *device is type @link ndigo_device ndigo_device @endlink
		*/
	NDIGO_API int ndigo_close(ndigo_device* device);

	/*! \ingroup runtime
	 *	\brief starts data acquisition
	 */
	NDIGO_API int ndigo_start_capture(ndigo_device* device);

	/*! \ingroup runtime
	 *	\brief pauses data acquisition
	 */
	NDIGO_API int ndigo_pause_capture(ndigo_device* device);

	/*! \ingroup runtime
	 * \brief Call this to resume data acquisition after a call to ndigo pause capture.
	 */
	NDIGO_API int ndigo_continue_capture(ndigo_device* device);

	/*! \ingroup runtime
	 *	\brief stops data acquisition
	 */
	NDIGO_API int ndigo_stop_capture(ndigo_device* device);

	/*! \ingroup runtime
	 *	\brief Enables each of the specified channels for one trigger respectively
	 */
	NDIGO_API int ndigo_single_shot(ndigo_device* device, int channel_mask);

	/*! \ingroup runtime
	 *	\brief Enables manual triggering
	 */
	NDIGO_API int ndigo_manual_trigger(ndigo_device* device, int channel_mask);

	/*! \ingroup readerror
	 *@{
	 */
#define NDIGO_READ_OK 0
#define NDIGO_READ_NO_DATA 1

	 /*! \brief Error in buffer handling, must restart
	  */
#define NDIGO_READ_INTERNAL_ERROR 2

	  /*! \brief Not used yet
	   */
#define NDIGO_READ_TIMEOUT 3
	   /*!@}*/

	   /*!	\ingroup runtime
		* \brief Acknowledge all data up to the packet provided as parameter.
		*
		* This is mandatory if acknowledge last read in the ndigo read in structure is set to false for calls to
		* ndigo read.
		*/
	NDIGO_API int ndigo_acknowledge(ndigo_device* device, ndigo_packet* packet);

	/*! \ingroup staticinfo
	 *	\brief Structure contains static information
	 *
	 *	This structure contains information about the board that does not change during run time.
	 *	It is provided by the function @link statfuncts xtdc4_get_static_info() @endlink.
	 */
	struct ndigo_static_info {
		/*! \brief The number of bytes occupied by the structure
		 */
		int size;

		/*! \brief A version number
		 *
		 * that is increased when the definition of the structure is changed.
		 * The increment can be larger than one to match driver version numbers or similar.
		 * Set to 0 for all versions up to first release.
		 */
		int version;

		/*! \brief Index of the board as passed to the constructor
		 *
		 * or set via int @link conffuncts ndigo_set_board_id(ndigo_device *device, int board_id) @endlink.
		 */
		int board_id;

		/*! \brief driver revision number
		 *
		 * The lower three bytes contain a triple level hierarchy of version numbers. * E.g. 0x010103 codes
		 * version 1.1.3. A change in the first digit generally requires a recompilation of user applications.
		 * Change in the second digit denote significant improvements or changes that don't break compatibility
		 * and the third digit changes with minor bugfixes and the like.
		 */
		int driver_revision;

		/*! \brief Revision number of the FPGA configuration.
		 *
		 * This can be read from a register.
		 */
		int firmware_revision;

		/*! \brief board revision number
		 *
		 * This can be read from a register. It is a four bit number that changes when the schematic of the
		 * board is changed.
		 *  - 0: Experimental first board Version. Labeled Rev. 1
		 *  - 2: First commercial Version. Labeled "Rev. 2"
		 *  - 3: for the version produced starting in 2011 labeled "Rev. 3"
		 */
		int board_revision;

		/*! \brief The same board schematic can be populated in multiple Variants.
		 *
		 * This is a four bit code that	can be read from a register.
		 * - For board revision 0 this always reads 0.
		 * - For board revision 2
		 *		- bit 0 determines whether this board contains an 8-bit or 10-bit ADC
		 *		- bit 1 determines whether differential inputs are used
		 *		- bit 2 determines whether the tdc-oscillator is present
		 *		- bit 3 = 1 signifies a special version of the board.
		 * - For board revision 3
		 *		- bit 2 determines input connectors (0: single ende, 1: differential)
		 *		- for the other bits see user guide
		*/
		int board_configuration;

		/*! \brief Number of bits of the ADC
		 *
		 * set to 0 if unknown. Should be 14.
		 */
		int adc_resolution;

		/*! \brief Maximum sample rate.
		 *	- 2:5e8 = 250Msps for the Ndigo250M,
		 *	- 1:25e8 = 125Msps for the Ndigo125M
		 *	- 5e9 = 5Gsps for the Ndigo5G
		 */
		double nominal_sample_rate;

		/*! \brief analog bandwidth
		 *
		 *	- 1.25e8 for 125MHz for Ndigo250M
		 *	- 3e9 for 3Ghz for Ndigo5G
		 */
		double analog_bandwidth;

		/*! \brief chipID as read from the 16 bit adc chip id register at SPI address 0.
		 *
		 * his value should be cached.
		 */
		int chip_id;

		/*! \brief Serial number with year and running number in 8.24 format.
		 *
		 * The number is identical to the one printed on the silvery sticker on the board.
		 */
		int board_serial;

		/*! \brief 64bit serial number from the configuration flash chip
		 */
		int flash_serial_low;

		/*! \brief 64bit serial number from the configuration flash chip
		 */
		int flash_serial_high;

		/*! \brief If not 0 the driver found valid calibration data in the flash on the board and is using it.
		 */
		int flash_valid;

		/*! \brief Returns false for the standard AC coupled Ndigo5G.
		 *
		 * Returns true for the Ndigo250M.
		 */
		ndigo_bool_t dc_coupled;

		/*! \brief Subversion revision id of the FPGA configuration.
		 *
		 * This can be read from a register.
		 */
		int subversion_revision;

		/*! \brief calibration date
		 *
		 * DIN EN ISO 8601 string YYYY-MM-DD HH:DD describing the time when the card was calibrated.
		 */
		char calibration_date[20];
	};

	/*! \ingroup statfuncts
	 * \brief contains the information about the board which does not change during run time.
	 */
	NDIGO_API int ndigo_get_static_info(ndigo_device* device, ndigo_static_info* info);

	/*! \ingroup alertdefs
	 *	\brief FPGA temperature alert (> 85@htmlonly &#176C @endhtmlonly)
	 */
#define NDIGO_ALERT_FPGA_TEMPERATURE			1

	 /*! \ingroup alertdefs
	  *	\brief Internal FPGA voltage out of range(< 1.01V or > 1.08V)
	  */
#define NDIGO_ALERT_VCCINT						2

	  /*! \ingroup alertdefs
	   *	\brief FPGA auxiliary voltage out of range (< 2.375V or > 2.625V)
	   */
#define NDIGO_ALERT_VCCAUX						4

	   /*! \ingroup alertdefs
		*	\brief FPGA temperature critical (> 125@htmlonly &#176C @endhtmlonly)
		*/
#define NDIGO_ALERT_FPGA_TEMPERATURE_CRITICAL	8

		/*! \ingroup alertdefs
		 *	\brief ADC temperature alert (> 90@htmlonly &#176C @endhtmlonly)
		 */
#define NDIGO_ALERT_ADC_TEMPERATURE				16

		 /*! \ingroup alertdefs
		  *	\brief ADC temperature critical (> 100@htmlonly &#176C @endhtmlonly)
		  */
#define NDIGO_ALERT_ADC_TEMPERATURE_CRITICAL	32

		  /*! \ingroup fastinfo
		   *	\brief contains fast dynamic information
		   *
		   *	This call returns a structure that contains dynamic information that can be obtained
		   *	within a few microseconds.
		   */
	typedef struct {
		/*! \brief The number of bytes occupied by the structure
		 */
		int size;

		/*! \brief	A version number
		 *
		 * that is increased when the definition of the structure is changed.
		 * The increment can be larger than one to match driver version numbers or similar. Set to 0 for all
		 * versions up to first release.
		 */
		int version;

		/*! \brief Speed of the ADC fan.
		 *
		 * Reports 0 if no fan is present.
		 */
		int adc_rpm;

		/*! \brief Speed of th FPGA fan.
		 *
		 * Reports 0 if no fan is present.
		 */
		int fpga_rpm;

		/*! \brief Alert bits from the system monitor
		 *
		 *	- bit 0: FPGA temperature alert (> 85@htmlonly &#176C @endhtmlonly)
		 *	- bit 1: Internal FPGA voltage out of range(< 1.01V or > 1.08V)
		 *	- bit 2: FPGA auxiliary voltage out of range (< 2.375V or > 2.625V)
		 *	- bit 3: FPGA temperature critical (> 125@htmlonly &#176C @endhtmlonly)
		 *	- bit 4: ADC temperature alert (> 90@htmlonly &#176C @endhtmlonly)
		 *	- bit 5: ADC temperature critical (> 100@htmlonly &#176C @endhtmlonly)
		 */
		int alerts;

		/*! \brief Auxiliary FPGA voltage, nominal 2.5V
		 */
		double voltage_aux;

		/*! \brief Internal FPGA voltage, nominal 1.0V
		 */
		double voltage_int;

		/*! \brief In @htmlonly &#176C @endhtmlonly measured on die.
		 */
		double fpga_temperature;

		/*! \brief organizes power supply of PCIe lanes
		 */
		int pcie_pwr_mgmt;

		/*! \brief Number of PCIe lanes that the card uses.
		 *
		 * Should be 4 for Ndigo5G
		 */
		int pcie_link_width;

		/*! \brief Maximum size in bytes for one PCIe transaction
		 *
		 * depends on system configuration.
		 */
		int pcie_max_payload;
	} ndigo_fast_info;

	/*! \ingroup paraminfo
	 *	\brief contains configuration changes
	 *
	 *	Structure filled by @link statfuncts xtdc4_get_param_info() @endlink. This structure
	 *	contains information that change indirectly due to configuration changes.
	 */
	typedef struct {
		/*! \brief The number of bytes occupied by the structure
		 */
		int size;

		/*! \brief	A version number
		 *
		 * that is increased when the definition of the structure is changed. The increment can be larger than
		 * one to match driver version numbers or similar. Set to 0 for all versions up to first release.
		 */
		int version;

		/*! \brief Bandwidth.
		 *
		 * Currently fixed at 3G, configurable to 1G (fullBW=false) for 10bit version.
		 */
		double bandwidth;

		/*! \brief	Actual sample rate of currently sampled data.
		 *
		 * depending on ADC mode: sample_rate = 5GHz/#channels.
		 */
		double sample_rate;

		/*! \brief The period one sample in the data represent in picoseconds
		 */
		double sample_period;

		/*! \brief the ID the board uses to identify itself in the output data stream.
		 *
		 * takes values 0 to 255.
		 */
		int board_id;

		/*!\brief Number of channels in the current mode
		 */
		int channels;

		/*! \brief Mask with a set bit for each enabled input channel.
		 */
		int channel_mask;

		/*! \brief The total amount of the DMA buffer in bytes
		 */
		__int64 total_buffer;
	} ndigo_param_info;

	/*! \ingroup statfuncts
	 * \brief returns information that changes indirectly due to configuration changes.
	 */
	NDIGO_API int ndigo_get_param_info(ndigo_device* device, ndigo_param_info* info);

	/*! \ingroup statfuncts
	 * \brief returns dynamic information that can be obtained within a few microseconds.
	 */
	NDIGO_API int ndigo_get_fast_info(ndigo_device* device, ndigo_fast_info* info);

	/*! \ingroup statfuncts
	 * \brief returns init parameters
	 */
	NDIGO_API int ndigo_get_init_parameters(ndigo_device* device, ndigo_init_parameters* init_params);


	/*! \ingroup slowinfo
	 * \brief contains slow dynamic information
	 *
	 *	The data reported in this structure requires milliseconds to be obtained. The
	 *	application should only call it in situation where the program flow can cope with an
	 *	interruption of that magnitude.
	 */
	struct ndigo_slow_info {

		/*! \brief The number of bytes occupied by the structure
		 */
		int size;

		/*! \brief	A version number
		 *
		 * that is increased when the definition of the structure is changed. The increment can be larger than
		 * one to match driver version numbers or similar. Set to 0 for all versions up to first release.
		 */
		int version;

		/*! \brief ADC temperature
		 *
		 * In @htmlonly &#176C @endhtmlonly measured on die.
		 */
		double adc_temperature;

		/*! \brief Temperature of the board
		 *
		 * In @htmlonly &#176C @endhtmlonly.
		 */
		double board_temperature;
	};

	/*! \ingroup statfuncts
	 * \brief returns information that can be obtained within a few milliseconds.
	 */
	NDIGO_API int ndigo_get_slow_info(ndigo_device* device, ndigo_slow_info* info);

	/*! \ingroup readfuncts
	 *	\brief Calculates the exact TDC packet timestamp
	 *
	 *	Call on a TDC packet to update the timestamp of the packet with a more accurate value. If called
	 *	more than once on a packet the timestamp will be invalid.
	 */
	NDIGO_API int ndigo_process_tdc_packet(ndigo_device* device, ndigo_packet* packet);

	/*! \ingroup led
	 *	\brief Set the LED to the selected color.
	 *
	 *	No automatic updates are performed.
	 */
	NDIGO_API int ndigo_set_led_color(ndigo_device* device, int led, unsigned short r, unsigned short g, unsigned short b);

	/*! \ingroup led
	 *	\brief hardware controlled LED
	 */
	NDIGO_API int ndigo_set_led_automode(ndigo_device* device, int led);

	/*! \ingroup errorfuncts
	 *	\brief gets latest error message of the current device
	 */
	NDIGO_API const char* ndigo_get_last_error_message(ndigo_device* device);

	/*! \ingroup led
	 *	\brief flashes the leds on the card
	 */
	NDIGO_API int ndigo_identify(ndigo_device* device, int repeat);

	/*! \ingroup statfuncts
	 *	\brief returns the type of the device
	 *
	 * CRONO_DEVICE_NDIGO5G or CRONO_DEVICE_NDIGO250M
	 */
	NDIGO_API int ndigo_get_device_type(ndigo_device* device);

	/*! \ingroup statfuncts
	 *	\brief returns the product name of the card
	 */
	NDIGO_API const char* ndigo_get_device_name(ndigo_device* device);

	/*! \ingroup conffuncts
	 *	\brief overwrites the board id
	 *
	 * can be changed after initialization of the card. If cronotools are used the board_id
	 * changes have to be done before cronotools initialization.
	 */
	NDIGO_API int ndigo_set_board_id(ndigo_device* device, int board_id);

#ifdef __cplusplus
}
#endif





//
// Header file containing structs and #defines specific for Ndigo5G
// The current driver version for Ndigo devices is 1.3.0.0
//
#ifdef __cplusplus
extern "C" {
#endif

	/*!	\defgroup initialization Initialization
	 *@{
	 */
	 /*! \defgroup initfuncts Functions for Initialization
	  */
	  /*! \defgroup initbuffer Structure ndigo_buffer_info
	   *	\brief contains buffer information
	   */
	   /*!@}*/
	   /*! \defgroup config Configuration
		* \brief The device is configured with a configuration structure
		*
		* The user should first obtain a structure that contains the default settings of the device
		* read from an on board ROM, then modify the structure as needed for the user application
		* and use the result to configure the device.
		* @{
		*/
		/*! \defgroup conffuncts Functions for Configuration
		 */
		 /*! \defgroup ndigoconf Structure ndigo_configuration
		  *	\brief This structure contains the configuration information.
		  *
		  *	It is used in conjunction with @link ndigo_get_default_configuration
		  *	ndigo_get_default_configuration() @endlink
		  *
		  *	Internally it uses the structures @link ndigo_trigger_block ndigo_trigger_block @endlink
		  *	and @link ndigo_trigger ndigo_trigger @endlink
		  *@{
		  */
		  /*! \defgroup outputdefs #defines for output_mode
		   *	\brief defines for @link ndigo_configuration output_mode @endlink
		   */
		   /*! \defgroup adcdefs defines for adc_mode
			*	\brief defines for @link ndigo_configuration adc_mode @endlink
			*/
			/*! \defgroup triggerdefs #defines for trigger
			 *	\brief defines for @link ndigo_configuration trigger @endlink
			 */
			 /*!@}*/
		 /*!@}*/
		 /*! \defgroup readout Readout
		  *@{
		  */
		  /*! \defgroup readfuncts Functions for readout
		   */
		   /*! \defgroup readin Structure ndigo_read_in
			*	\brief The parameters of the read commands
			*/
			/*! \defgroup readoutstruct Structure ndigo_read_out
			 *	\brief contains information about the read process
			 */
			 /*!@}*/

			 /*! \ingroup constants Constants
			  * @{
			  */
#define NDIGO_CHANNEL_COUNT 4	//!< The number of analog input channels.
#define NDIGO_GATE_COUNT 4		//!< The number of gating blocks.
#define NDIGO_EXTENSION_COUNT 4	//!< the number of external triggers.
			  /*!@}*/

			  /*!	\ingroup adcdefs
			   *	\brief 4 channel mode at sample rate 1.25GHz
			   */
#define NDIGO_ADC_MODE_ABCD 0
			   /*!	\ingroup adcdefs
				*	\brief 2 channel mode at sample rate 2.5GHz
				*/
#define NDIGO_ADC_MODE_AC 4
				/*!	\ingroup adcdefs
				 *	\brief 2 channel mode at sample rate 2.5GHz
				 */
#define NDIGO_ADC_MODE_BC 5
				 /*!	\ingroup adcdefs
				  *	\brief 2 channel mode at sample rate 2.5GHz
				  */
#define NDIGO_ADC_MODE_AD 6
				  /*!	\ingroup adcdefs
				   *	\brief 2 channel mode at sample rate 2.5GHz
				   */
#define NDIGO_ADC_MODE_BD 7
				   /*!	\ingroup adcdefs
					*	\brief single channel mode at sample rate 5GHz
					*/
#define NDIGO_ADC_MODE_A 8
					/*!	\ingroup adcdefs
					 *	\brief single channel mode at sample rate 5GHz
					 */
#define NDIGO_ADC_MODE_B 9
					 /*!	\ingroup adcdefs
					  *	\brief single channel mode at sample rate 5GHz
					  */
#define NDIGO_ADC_MODE_C 10
					  /*!	\ingroup adcdefs
					   *	\brief single channel mode at sample rate 5GHz
					   */
#define NDIGO_ADC_MODE_D 11
					   /*!	\ingroup adcdefs
						*	\brief single channel mode at sample rate 1.25GHz
						*/
#define NDIGO_ADC_MODE_AAAA 12
						/*!	\ingroup adcdefs
						 *	\brief single channel mode at sample rate 1.25GHz
						 */
#define NDIGO_ADC_MODE_BBBB 13
						 /*!	\ingroup adcdefs
						  *	\brief single channel mode at sample rate 1.25GHz
						  */
#define NDIGO_ADC_MODE_CCCC 14
						  /*!	\ingroup adcdefs
						   *	\brief single channel mode at sample rate 1.25GHz
						   */
#define NDIGO_ADC_MODE_DDDD 15

						   /*!\ingroup adcdefs
							* \brief temporarily unavailable ADC modes
							*
							* availability of the following modes depends on the specific hardware/ firmware revision
							*/
#define NDIGO_ADC_MODE_A12 28

							/*!\ingroup adcdefs
							 * \brief temporarily unavailable ADC modes
							 *
							 * availability of the following modes depends on the specific hardware/ firmware revision
							 */
#define NDIGO_ADC_MODE_B12 29

							 /*!\ingroup adcdefs
							  * \brief temporarily unavailable ADC modes
							  *
							  * availability of the following modes depends on the specific hardware/ firmware revision
							  */
#define NDIGO_ADC_MODE_C12 30

							  /*!\ingroup adcdefs
							   * \brief temporarily unavailable ADC modes
							   *
							   * availability of the following modes depends on the specific hardware/ firmware revision
							   */
#define NDIGO_ADC_MODE_D12 31

							   /*! \ingroup precursordef
								*	\brief maximum @link ndigo_trigger_block precursor @endlink
								*/
#define NDIGO_MAX_PRECURSOR 25

								/*! \ingroup multishotdef
								 *	\brief maximum @link ndigo_trigger_block number of shots @endlink
								 */
#define NDIGO_MAX_MULTISHOT 65535

								 /*! \ingroup constants
								  *	\brief ADC sample fifo depth
								  */
#define NDIGO_FIFO_DEPTH 8176

								  /*! \ingroup outputdefs
								   *	\brief Output in signed16 integer format.
								   *
								   * Return output in the range -32768 to 32767
								   */
#define NDIGO_OUTPUT_MODE_SIGNED16 0

								   /*! \ingroup outputdefs
									*	\brief Native range
									*
									*	Return the native range 0 - 1023
									*/
#define NDIGO_OUTPUT_MODE_RAW 1

									/*! \ingroup outputdefs
									 *@{
									 */
#define NDIGO_OUTPUT_MODE_CUSTOM 2
#define NDIGO_OUTPUT_MODE_CUSTOM_INL 3
									 /*!@}*/

									 /*! \ingroup ndigoconf
									  *	\brief Maps a value between 0 and 1 to a 16-bit output value.
									  *
									  * Mapping has to be monotonous. It can be used e.g. for custom INL, offset and gain correction
									  * that covers user front end electronics. It can also invert the signal or correct the effect
									  * of logarithmic input amplifiers etc.
									  * The LUT is applied on the board, thus using it does not cause any additional CPU load. In the
									  * mode @link outputdefs NDIGO_OUTPUT_MODE_CUSTOM_INL @endlink the on-board INL correctin table is applied before the
									  * user function, while @link outputdefs NDIGO_OUTPUT_MODE_CUSTOM @endlink does not perform INL correction. In order to
									  * use the user lookup table functionality lut_func must be set to a pointer to the LUT function.
									  */
	typedef short(*lut_func)(int channel, float x);

	/*! \ingroup ndigoconf
	 *	\brief Contains all user changeable settings
	 *
	 * This is the structure containing the configuration information. It is used in
	 * conjunction with @link conffuncts ndigo_get_default_configuration() @endlink, @link conffuncts ndigo_get_current_configuration() @endlink
	 * and @link conffuncts ndigo_configure() @endlink. It uses internally the structures @link ndigotrgblock ndigo_trigger_block @endlink and
	 * @link ndigotrg ndigo_trigger@endlink.
	 */
	typedef struct {
		/*! \brief size in bytes
		 *
		 * The number of bytes occupied by the structure.
		 */
		int size;

		/*! \brief The version number
		 *
		 * that is increased when the definition of the structure is changed. The increment
		 * can be larger than one to match driver version numbers or similar. Set to 0 for
		 * all versions up to first release.
		 */
		int version;

		/*! \brief Reserved for internal usage.
		 *
		 * Do not change.
		 */
		int reserved1;

		/*! \brief ADC mode
		 *
		 * as defined in @link adcdefs NDIGO_ADC_MODE_* @endlink
		 */
		int adc_mode;

		/*! \brief Minimum bandwidth
		 *
		 * Set to the minimum bandwidth required for the application. Lower bandwidth results
		 * in reduced noise. The driver will set the ADC to the minimum setting that has at
		 * least the desired bandwidth and report the selected bandwidth in the ndigo param
		 * info structure. The -8, -10 and -12 versions currently supports 1GHz and 3GHz
		 * bandwidth, the -8AQ version supports 2GHz, 1.5GHz, 600MHz and 500 MHz.
		 */
		double bandwidth;

		/*Was is_slave before, moved to init_parameters.*/
		/*! \brief Reserved for internal usage.
		 *
		 * Do not change.
		 */
		ndigo_bool_t reserved;

		/*! \brief Enable capturing of TDC measurements
		 *
		 * on external digital input channel.
		 */
		ndigo_bool_t tdc_enabled;

		/*! \brief Enable enhanced TDC resolution.
		 *
		 * Currently not implemented.
		 */
		ndigo_bool_t tdc_fb_enabled;

		/*! \brief Sets the input DC offset-values
		 *
		 * to +/- this value in volts. Default is 0.
		 */
		 /* Programmed parameter is dac_neutral +- (channel_offset*64k/2.5).*/
		double analog_offset[NDIGO_CHANNEL_COUNT];

		/* Set DAC on channels 5 (GATE) and 6 (TRIG). Default offset for both is -0.35V. */
		/*! \brief Sets DC offset for GATE input and TDC trigger input.
		 *
		 * Sets the DC offset in volts for the GATE (index 0) and TDC trigger input.
		 * The trigger threshold is zero. For negative 0.8V NIM pulses a value of
		 * -0.35 should be set.
		 */
		double dc_offset[2];

		/*! \brief Configuration of the external trigger sources.
		 *
		 * Threshold is ignored for entries 8 and above. The trigger indeces refer to the
		 * entry in the trigger array @link triggerdefs NDIGO_TRIGGER_* @endlink
		 */
		ndigo_trigger trigger[NDIGO_TRIGGER_COUNT + NDIGO_ADD_TRIGGER_COUNT];

		// including config for timestamp channel
		/*! \brief Trigger settings of input and timestamp channels.
		 *
		 * The number of input channels depends on TDC mode.
		 */
		ndigo_trigger_block trigger_block[NDIGO_CHANNEL_COUNT + 1];

		/*! \brief Configuration of gating blocks
		 *
		 * Gating blocks are used to filter trigger.
		 */
		ndigo_gating_block gating_block[NDIGO_GATE_COUNT];

		/*! \brief Configuration of extension blocks
		 *
		 * A structure describing the routing of the 4 digital channels of the Ndigo extension
		 * board to the trigger matrix.
		 */
		ndigo_extension_block extension_block[NDIGO_EXTENSION_COUNT];

		/*! \brief Enables output drives.
		 *
		 * Enables output drive for each of the four external sync lines. Each integer
		 * represents a bitmask selecting the trigger sources for that line. The bit mapping is
		 * described in @link ndigotrgblock ndigo_trigger_block @endlink.
		 */
		int drive_bus[4];

		/** \brief component to create a trigger either periodically or randomly.
		 *
		 * To be exact, there are two parameters M = @link auto_trigger_period auto_trigger_period @endlink
		 * and N = @link auto_trigger_random_exponent auto_trigger_random_exponent @endlink
		 * that result in a distance between triggers of
		 *
		 * T = 1 + M + [1...2^N]
		 *
		 * clock cycles.
		 *
		 * 0 <= M < 2^32
		 *
		 * 0 <= N < 32
		 *
		 * there is no enable or reset as the usage of this trigger can be configured in the channels.
		 */
		int auto_trigger_period;

		/** \brief component to create a trigger either periodically or randomly.
		 *
		 * To be exact, there are two parameters M = @link auto_trigger_period auto_trigger_period @endlink
		 * and N = @link auto_trigger_random_exponent auto_trigger_random_exponent @endlink
		 * that result in a distance between triggers of
		 *
		 * T = 1 + M + [1...2^N]
		 *
		 * clock cycles.
		 *
		 * 0 <= M < 2^32
		 *
		 * 0 <= N < 32
		 *
		 * there is no enable or reset as the usage of this trigger can be configured in the channels.
		 */
		int auto_trigger_random_exponent;

		/*! \brief defines the data representation in the output.
		 *
		 * Signed16 scales and INL-corrects the input. RAW directly presents the ADC values. Defines
		 * can be found in @link outputdefs NDIGO_OUTPUT_MODE_* @endlink
		 */
		int output_mode;

		/*! \brief used for mapping from ADC value to output value.
		 *
		 * If the output_mode is set to @link outputdefs NDIGO_OUTPUT_MODE_CUSTOM @endlink or @link outputdefs NDIGO_OUTPUT_MODE_CUSTOM_INL @endlink
		 * this function is used for mapping from ADC value to output value. The driver will call
		 * this function with a value from -1 to +1 and the function must return the corresponding
		 * signed 16 bit value that the board should return for an input voltage relative to the full
		 * scale range.
		 */
		lut_func custom_lut;

	} ndigo_configuration;

	/*! \ingroup conffuncts
	 * \brief Copies the default configuration to the specified config pointer
	 */
	NDIGO_API int ndigo_get_default_configuration(ndigo_device* device, ndigo_configuration* config);

	/*! \ingroup conffuncts
	 * \brief Copies the current configuration to the specified config pointer
	 */
	NDIGO_API int ndigo_get_current_configuration(ndigo_device* device, ndigo_configuration* config);

	/*! \ingroup conffuncts
	 *	\brief Configures Ndigo device.
	 *
	 * The config information is copied such that it can be changed after the call to
	 * @link ndigoconf ndigo_configure()@endlink.
	 */
	NDIGO_API int ndigo_configure(ndigo_device* device, ndigo_configuration* config);

	/*! \ingroup initfuncts
	 * \brief returns the number of boards in the system that are supported by this driver
	 *	\param *error_code is type int
	 *	\param **error_message is type const char
	 */
	NDIGO_API int ndigo_count_devices(int* error_code, const char** error_message);

	/*! \ingroup initfuncts
	 * \brief get a set of default parameteres to feed into @link initfuncts ndigo_init()@endlink.
	 *
	 * This must always be used to initialize the @link initparamsstruct ndigo_init_parameter structure @endlink.
	 *	\param *init is type @link initparamsstruct ndigo_init_parameter @endlink
	 */
	NDIGO_API int ndigo_get_default_init_parameters(ndigo_init_parameters* init);

	/*! \ingroup initfuncts
	 * \brief Open and initialize the Ndigo board with the given index.
	 *
	 * With error_code and error_message the user must provide pointers where error information should be
	 * written by the driver. The buffer for the error message must contain at least 80 chars.
	 *	\param *params is type @link initparamsstruct ndigo_init_parameters @endlink
	 *	\param *error_code is type int
	 *	\param **error_message is type char. The buffer for the error message has to contain at least 80 chars.
	 */
	NDIGO_API ndigo_device* ndigo_init(ndigo_init_parameters* params, int* error_code, const char** error_message);

	/*! \ingroup readin
	 *	\brief The parameters of the read commands
	 */
	struct ndigo_read_in {
		/*! \brief automatic acknowledgement of packets
		 *
		 * if set, @link readfuncts ndigo_read() @endlink automatically acknowledges packets from the last read
		 */
		ndigo_bool_t acknowledge_last_read;
	};

	/*! \ingroup readoutstruct
	 *	\brief contains information about the read process
	 */
	struct ndigo_read_out {
		/*! \brief Pointer to the first packet, that was captured by the call of @link readfuncts ndigo_read() @endlink.
		 */
		ndigo_packet* first_packet;

		/*! \brief Address of the header of the last packet in the buffer.
		 */
		ndigo_packet* last_packet;

		/*! \brief Error Code defined in @link readerror NDIGO_READ_* @endlink
		 */
		int error_code;

		/*! \brief Error Message
		 */
		const char* error_message;
	};

	/*! \ingroup readfuncts
	 * \brief reads packets
	 *
	 * Returns a pointer to an array of captured data in @link readout read_out @endlink. The result contains any number of packets
	 * of type @link ndigopack ndigo_packet @endlink. @link readin read_in @endlink provides parameteres to the driver. A call to this method automatically
	 * allows the driver to reuse the memory returned in the previous call.
	 *
	 * Returns an error code as defined in the structure @link readout ndigo_read_out @endlink
	 */
	NDIGO_API int ndigo_read(ndigo_device* device, ndigo_read_in* in, ndigo_read_out* out);

	/*! \ingroup readfuncts
	 * \brief acknowledge data
	 *
	 * Acknowledge all data up to the packet provided as parameter. This is mandatory if @link readin acknowledge_last_read @endlink
	 * in the @link readin ndigo_read_in structure @endlink is set to false for calls to @link readfuncts ndigo_read() @endlink.
	 *
	 * This feature allows to either free up partial DMA space early if there will be no call to @link readfuncts ndigo_read() @endlink
	 * anytime soon. It also allows to keep data over multiple calls to @link readfuncts ndigo_read @endlink to avoid unnecessary copying
	 * of data.
	 */
	NDIGO_API int ndigo_acknowledge(ndigo_device* device, ndigo_packet* packet);

	/*! \ingroup initbuffer
	 *	\brief contains buffer information
	 */
	struct ndigo_buffer_info {
		/*! \brief buffer address
		 */
		__int64* buffer_address;

		/*! \brief size of the buffer
		 */
		__int64 buffer_size;
	};

	/*! \ingroup initfuncts
	 *	\brief buffer information
	 *
	 * this method gets the information about the buffer
	 */
	NDIGO_API int ndigo_get_buffer_info(ndigo_device* device, ndigo_buffer_info* info);

	/*! \ingroup conffuncts
	 *	\brief converts output value from one @link outputdefs output_mode @endlink to another @link outputdefs output_mode @endlink.
	 */
	NDIGO_API short ndigo_convert_value(ndigo_device* device, int channel, short val, int outputModeFrom, int outputModeTo);

#ifdef __cplusplus
}
#endif

#endif