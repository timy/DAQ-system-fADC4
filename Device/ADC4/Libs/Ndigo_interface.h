//
// Header file containing structs and #defines specific for Ndigo5G
// The current driver version for Ndigo devices is 1.3.0.0
//

#ifndef NDIGO_INTERFACE_H
#define NDIGO_INTERFACE_H
#include "Ndigo_common_interface.h"
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
NDIGO_API int ndigo_get_default_configuration(ndigo_device *device,ndigo_configuration *config);

/*! \ingroup conffuncts
 * \brief Copies the current configuration to the specified config pointer
 */
NDIGO_API int ndigo_get_current_configuration(ndigo_device *device, ndigo_configuration *config);

/*! \ingroup conffuncts
 *	\brief Configures Ndigo device.
 *
 * The config information is copied such that it can be changed after the call to
 * @link ndigoconf ndigo_configure()@endlink.
 */
NDIGO_API int ndigo_configure(ndigo_device *device, ndigo_configuration * config);

/*! \ingroup initfuncts
 * \brief returns the number of boards in the system that are supported by this driver
 *	\param *error_code is type int
 *	\param **error_message is type const char
 */
NDIGO_API int ndigo_count_devices(int *error_code, const char** error_message);

/*! \ingroup initfuncts
 * \brief get a set of default parameteres to feed into @link initfuncts ndigo_init()@endlink.
 *
 * This must always be used to initialize the @link initparamsstruct ndigo_init_parameter structure @endlink.
 *	\param *init is type @link initparamsstruct ndigo_init_parameter @endlink
 */
NDIGO_API int ndigo_get_default_init_parameters(ndigo_init_parameters *init);

/*! \ingroup initfuncts
 * \brief Open and initialize the Ndigo board with the given index.
 *
 * With error_code and error_message the user must provide pointers where error information should be
 * written by the driver. The buffer for the error message must contain at least 80 chars.
 *	\param *params is type @link initparamsstruct ndigo_init_parameters @endlink
 *	\param *error_code is type int
 *	\param **error_message is type char. The buffer for the error message has to contain at least 80 chars.
 */
NDIGO_API ndigo_device *ndigo_init(ndigo_init_parameters *params,int *error_code, const char** error_message);

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
	ndigo_packet *first_packet;

	/*! \brief Address of the header of the last packet in the buffer.
	 */
	ndigo_packet *last_packet;

	/*! \brief Error Code defined in @link readerror NDIGO_READ_* @endlink
	 */
	int error_code;

	/*! \brief Error Message
	 */
	const char * error_message;
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
NDIGO_API int ndigo_read(ndigo_device *device, ndigo_read_in *in, ndigo_read_out* out);

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
NDIGO_API int ndigo_acknowledge(ndigo_device *device, ndigo_packet *packet);

/*! \ingroup initbuffer
 *	\brief contains buffer information
 */
struct ndigo_buffer_info {
	/*! \brief buffer address
	 */
	__int64 *buffer_address;

	/*! \brief size of the buffer
	 */
	__int64 buffer_size;
};

/*! \ingroup initfuncts
 *	\brief buffer information
 *
 * this method gets the information about the buffer
 */
NDIGO_API int ndigo_get_buffer_info(ndigo_device *device, ndigo_buffer_info* info);

/*! \ingroup conffuncts
 *	\brief converts output value from one @link outputdefs output_mode @endlink to another @link outputdefs output_mode @endlink.
 */
NDIGO_API short ndigo_convert_value(ndigo_device* device,int channel, short val, int outputModeFrom, int outputModeTo);

#ifdef __cplusplus
}
#endif

#endif
