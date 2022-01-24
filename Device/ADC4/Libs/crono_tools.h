//
// Header file containing structs and #defines commonly used by header files of derived CRONO device classes
// The current driver version for Ndigo devices is 1.3.0.0
//

#ifndef CRONO_TOOLS_H
#define CRONO_TOOLS_H
#include "crono_interface.h"
#ifdef _MSC_VER
	#ifdef CRONO_TOOLS_EXPORTS
		#define CRONO_API __declspec(dllexport)
	#else
		#define CRONO_API __declspec(dllimport)
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
	void * sync;
} crono_sync;

/*! \ingroup cronosyncinitparams
 *	\brief struct for the initialization of synchronized ndigo boards
 *  
 *  The master card must be the first device!
 *	this structure MUST be completely INITIALIZED.
 *  For the HPTDCs the board_id field in the crono_device structure MUST be filled.
 *  It is ignored for all other devices.
 */
typedef struct  {
	/*!*/
	int device_count;
	/*!*/
	crono_device *devices;

} crono_sync_init_parameters;


/*! \ingroup initfuncts
 *	\brief init the card for the given client
 *
 *	for the version number, use CRONO_API_VERSION (not implemented?!)
 */
CRONO_API crono_sync *crono_sync_init(crono_sync_init_parameters *params,int *error_code, const char** error_message);

/*! \ingroup confstruct
 *
 *	max. amount of packets in a group is 10000 
 *	- -1ms < group_range_start < 1ms
 *	- -1ms < group_range_end < 1ms
 *
 *	the trigger packet is always included in the group even if group_range_start > 0.
*/
typedef struct  {
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
CRONO_API int crono_sync_configure(crono_sync *sync, crono_sync_configuration *config,int *error_code, const char** error_message);

/*! \ingroup initfuncts
 *	\brief Closes and frees the synchronization objects
 *
 *	does NOT close the devices!
*/
CRONO_API int crono_sync_close(crono_sync *device);

/*! \ingroup runtime
 *	\brief Starts capture on the cards
 *
 *	the return code is either CRONO_OK (0) or the index of the failed device in the top byte(32bit) and the error code in the remaining bytes.
*/
CRONO_API int crono_sync_start_capture(crono_sync *device);

/*! \ingroup runtime
 *	\brief Stops capture on the boards
 */
CRONO_API int crono_sync_stop_capture(crono_sync *device);

/*!	\ingroup readfuncts
 *	\brief returns the last error message
 *
 *	encountered if one of the cards returns an error code
 */
CRONO_API const char* crono_sync_get_last_error_message(crono_sync *device);


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
	crono_packet **packets;
} crono_group;

/*! \ingroup strucreadout
 *	\brief parameters for readout
 */
typedef struct  {
	int group_count;
	//! \brief array of pointers to packets
	crono_group *groups;
	int error_code;
	const char * error_message;
} crono_sync_read_out;

/*!	\ingroup readfuncts
 *	\brief	Reads packets from the board
 *	
 *	automatically acknowledges the last read packets
 */
CRONO_API int crono_sync_read(crono_sync *device, crono_sync_read_in *in, crono_sync_read_out* out);


/*!	\ingroup readfuncts
 *	\brief acknowledges the processing of the last read block
 *
 *	this is only necessary if read is not called
 */
CRONO_API int crono_sync_acknowledge(crono_sync *device);

/*!	\ingroup initfuncts
 *	\brief initializes all boards involved
 *
 *	Initializes primarily the first ndigo (master) and then all slaves simultaneously.
 */
CRONO_API int ndigo_init_multiple(void *init_params, int count, crono_device **ndigo, int *error_code, const char **error_message);

#ifdef __cplusplus
}
#endif

#endif
