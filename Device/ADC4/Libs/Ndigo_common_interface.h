//
// Header file containing structs and #defines commonly used by header files of derived Ndigo device classes
// The current driver version for Ndigo devices is 1.3.0.0
//

#ifndef NDIGO_COMMON_INTERFACE_H
#define NDIGO_COMMON_INTERFACE_H
#include "crono_interface.h"

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
		#define NDIGO_API __declspec(dllimport)
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
	void * ndigo;
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
NDIGO_API int ndigo_close(ndigo_device *device);

/*! \ingroup runtime
 *	\brief starts data acquisition
 */
NDIGO_API int ndigo_start_capture(ndigo_device *device);

/*! \ingroup runtime
 *	\brief pauses data acquisition
 */
NDIGO_API int ndigo_pause_capture(ndigo_device *device);

/*! \ingroup runtime
 * \brief Call this to resume data acquisition after a call to ndigo pause capture.
 */
NDIGO_API int ndigo_continue_capture(ndigo_device *device);

/*! \ingroup runtime
 *	\brief stops data acquisition
 */
NDIGO_API int ndigo_stop_capture(ndigo_device *device);

/*! \ingroup runtime
 *	\brief Enables each of the specified channels for one trigger respectively
 */
NDIGO_API int ndigo_single_shot(ndigo_device *device, int channel_mask);

/*! \ingroup runtime
 *	\brief Enables manual triggering
 */
NDIGO_API int ndigo_manual_trigger(ndigo_device *device, int channel_mask);

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
NDIGO_API int ndigo_acknowledge(ndigo_device *device, ndigo_packet *packet);

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
NDIGO_API int ndigo_get_static_info(ndigo_device *device,ndigo_static_info* info);

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
NDIGO_API int ndigo_get_param_info(ndigo_device* device, ndigo_param_info * info);

/*! \ingroup statfuncts
 * \brief returns dynamic information that can be obtained within a few microseconds.
 */
NDIGO_API int ndigo_get_fast_info(ndigo_device* device, ndigo_fast_info * info);

/*! \ingroup statfuncts
 * \brief returns init parameters
 */
NDIGO_API int ndigo_get_init_parameters(ndigo_device* device, ndigo_init_parameters * init_params);


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
NDIGO_API int ndigo_get_slow_info(ndigo_device* device, ndigo_slow_info * info);

/*! \ingroup readfuncts
 *	\brief Calculates the exact TDC packet timestamp
 *
 *	Call on a TDC packet to update the timestamp of the packet with a more accurate value. If called
 *	more than once on a packet the timestamp will be invalid.
 */
NDIGO_API int ndigo_process_tdc_packet(ndigo_device* device, ndigo_packet * packet);

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
NDIGO_API int ndigo_get_device_type(ndigo_device *device);

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
NDIGO_API int ndigo_set_board_id(ndigo_device *device, int board_id);

#ifdef __cplusplus
}
#endif

#endif
