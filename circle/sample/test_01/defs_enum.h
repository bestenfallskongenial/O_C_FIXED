enum modetable		                                
{
	CH0_MODE = 0,           // store the mode ( from g_modeTable[] ) for cannel 0
	CH1_MODE,               // store the mode ( from g_modeTable[] ) for cannel 1
	CH2_MODE,               // store the mode ( from g_modeTable[] ) for cannel 2
	CH3_MODE,               // store the mode ( from g_modeTable[] ) for cannel 3

	CH4_MODE,               // store the mode ( from g_modeTable[] ) for cannel 4
	CH5_MODE,               // store the mode ( from g_modeTable[] ) for cannel 5
	CH6_MODE,               // store the mode ( from g_modeTable[] ) for cannel 6
	CH7_MODE,               // store the mode ( from g_modeTable[] ) for cannel 7

	LF1_WAVE,               // stores waveform for lfo1 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] ) 
	LF2_WAVE,               // stores waveform for lfo2 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )
	LF1_MULT,               // stores the multiplier for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )
	LF2_MULT,               // stores the multiplier for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )

	SENS_A,                 // stores the sensitivity for the audio mode ( available if enabled ) bandA0
	SENS_B,                 // stores the sensitivity for the audio mode ( available if enabled ) bandA1
	SENS_C,                 // stores the sensitivity for the audio mode ( available if enabled ) bandB0
	SENS_D,                 // stores the sensitivity for the audio mode ( available if enabled ) bandB1

	FRM_MODE,   // 0 or 1   ( wait - i propose a simple 4 to 7 and 0 as of, means anything else than 0 is the actual input p_channel problem, it need to overrule CH*_MODE )
	TEX_MODE,   // 0 or 1   ( how we can do it? also, dont i want more than only p_channel 4-7 assignable? )
	CLK_MODE,   // 0 or 1   ( whats about the approach in readADC() where i modify the number of possible modes in the modematrix )
	VID_MODE,   // 0 or 1   ( like if i have one here CH*_MODE "opens" up for this modes - requires a constant check and update but... )

    TIME_MODE,
    DUMMY_A,
    DUMMY_B,
    DUMMY_C,
    STORE_SET,  // 0 or 1   ( if set to 1 and the MENU_LAYER_COUNT is zero again ( button released ?) the file operation starts )
    LOAD_SET,   // 0 or 1
    STORE_LOG,  // 0 or 1
    LOAD_KLN,   // 0 or 1

	IS_STORED,                          // not really a mode but a flag - important: obey the "4 per block" rule

    MODETABLE_COUNT
};
//------------------------------------------------- // for the array unsigned/float g_inOutMatrix*[CHANNEL][IO_TYPE_COUNT]
//	INT	    ADC_RAW     (SCALED IN)VAL 	(SCALED)OUT 	RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
//	ch0	    U	        U	        	U           	U	        G	        G                                                           U           U
//	ch1	    U	        U	       	 	U           	U	                     	                                                        U           U
//	ch2	    U	        U	        	U           	U	                     	                                                        U           U
//	ch3	    U	        U	        	U           	U	                     	                                                        U           U
//	ch4	    U	        U	        	U           	U	                     	                                                        U           U
//	ch5	    U	        U	        	U           	U	                     	                                                        U           U
//	ch6	    U	        U	        	U           	U	                     	                                                        U           U
//	ch7	    U	        U	        	U           	U	                     	                                                        U           U

//	FLT	    ADC_RAW     (SCALED IN)VAL  (SCALED)OUT 	RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
//	ch0	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch1	    opt         U	        	U           	U	                     	                                                        opt        opt   
//	ch2	    opt         U	        	U           	U	                     	                                                        opt        opt   
//	ch3	    opt         U	        	U           	U	                     	                                                        opt        opt   
//	ch4	    opt         U	        	U           	U	                     	                                                        opt        opt   
//	ch5	    opt         U	        	U           	U	                     	                                                        opt        opt   
//	ch6	    opt         U	        	U           	U	                     	                                                        opt        opt   
//	ch7	    opt         U	        	U           	U	                     	                                                        opt        opt   
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum io_types                               
{
    RAW = 0,                                    		//  the position the dampened adc values per channels are stored *
    VAL,                                        		//  either the calculated/normalized int or flt value *
    OUT,                                        		//  here lands the processed ( after mode ) for the glsl uniforms      
    RND,                                        		//  either the per-p_channel random int or flt value *
    LF1,                                        		//  either the lfo one int or flt value
    LF2,                                        		//  either the lfo two int or flt value
    AU0,                                        		//  the audio band 0 flt value
    AU1,                                        		//  the audio band 1 flt value
    AU2,                                        		//  the audio band 2 flt value
    AU3,                                        		//  the audio band 3 flt value
    TRL,                                        		//  per p_channel threshold low !!! dont forget to copy the values in here    128
    TRH,                                        		//  per p_channel threshold high                                              320
    TRF,                                        		//  per p_channel threshold "flag" - where do i need you again?

    IO_TYPE_COUNT 										// 13	*means i have a unique value for each p_channel - the other values are singular, and/or only int/flt
};
//------------------------------------------------- // for the array unsigned (long?) g_lfoBpmMatrix[4][LFO_BPM_COUNT]; <- no float needed here - its stores in the io matrix
// Layer / Instance | BPM        | INTV       | NBT        | LCB        | NCB        | LBC        | LBCT       | LMT        | ELP        | CYL        | SMP        | LTIME      | TIDX       | TB            | DB             | IREG
// ---------------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ------------- | -------------- | --------------
// `[0]`            | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | timeBuffer[0] | deltaBuffer[0] | irregularity[0]
// `[1]`            | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | timeBuffer[1] | deltaBuffer[1] | irregularity[1]
// `[2]`            | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | timeBuffer[2] | deltaBuffer[2] | irregularity[2]
// `[3]`            | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | timeBuffer[3] | unused         | irregularity[3]
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum lfo_bpm_types
{
    BPM =0,         								// result BPM
    INTV,           								// intervalCalculated
    NBT,            								// nextBeatTime
    LCB,            								// lastCircleBuffer
   	NCB,            								// nextCircleBuffer
    LBC,            								// lastBpmCalculation
    LBCT,           								// lastBpmCalculationTMP
    LMT,            								// lfoMultiplierTMP
    ELP,            								// elapsedMicroseconds
    CYL,            								// cycleLength
    SMP,            								// sampleIndex
    LTIME,          							    // lastTime
    TIDX,           							    // timeIndex
    TB,             							    // timeBuffer
    DB,             							    // deltaBuffer
    IREG,                                           // irregularity ... is not really bpm/lfo but audio therefore... it fits so nicely here
    LFO_BPM_COUNT								    // 15
};
//------------------------------------------------- // g_modeLengthAdd[MODELEN_FLAG_COUNT] 
enum ModeLengthFlag
{
                MODELENDEFAULT  = 0,
                MODELEN_AUDIO_A,            	    // any extra mode that modifies the number of max modes per channel must have an enum
                MODELEN_AUDIO_B,
                MODELEN_AUDIO_C,
                MODELEN_LFO_A,
                MODELEN_SENS_A,

                MODELEN_FLAG_COUNT
};
//------------------------------------------------- // unsigned int g_buttons_states[BUTTON_COUNT][5] = {0} !!!
enum ButtonTSIndex  
{
    BTN_PRESS_START = 0,
    BTN_DOUBLE      = 1,
    BTN_RELEASE     = 2,
    BTN_SINGLE      = 3,
    BTN_HOLD_TICK   = 4,                            // COUNTER: increases while held ( runtime loop iterations - unstable but sufficient - needs measurement )
    BTN_INDEX_COUNT = 5
};
//------------------------------------------------- // for unsigned filecounter[FT_COUNT][FLD_COUNT]
enum FileType
{
    FT_VSH = 0,                                     // vertex shader 
    FT_OMF,                                         // overlay fragment shader 
    FT_FSH,                                         // user fragment shader
    FT_OMT,                                         // overlay texture
    FT_TEX,                                         // user texture
    FT_VID,                                         		// user video 
    FT_KLN,                                         		// kernel
    FRM_BF,                                         		// output-frames A & B
    LOGGER,                                         		// log-buffer
    FT_COUNT
};
enum FileField
{
    FLD_MAXSD = 0,                                  		// number of maximal files on sd
    FLD_MAXUSB,                                     		// number of maximal files on usb
    FLD_EXTCNT,                                     		// number of possible file extensions const char *g_Suf*[]
    FLD_SCANNED,                                    		// number of files scanned via scanRoot(...)  
    FLD_LOADED,                                     		// number of actual loaded files after bulkLoad(...)
    FLD_PREV,                                       		// number of actual loaded files before bulkLoad(...)
    FLD_VALID,                                      		// number of valid files after gfx_init / parsing
    FLD_SIZE,                                       		// maximal size of the files / buffer-size
    FLD_COUNT
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
// my vcsm dirver
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum vc_sm_msg_type 										// Message types supported for HOST->VC direction //			
	{				
	VC_SM_MSG_TYPE_ALLOC,									// Allocate shared memory block //
	VC_SM_MSG_TYPE_LOCK,									// Lock allocated shared memory block //
	VC_SM_MSG_TYPE_UNLOCK,									// Unlock allocated shared memory block //
	VC_SM_MSG_TYPE_UNLOCK_NOANS,							// Unlock allocated shared memory block, do not answer command //
	VC_SM_MSG_TYPE_FREE,									// Free shared memory block //
	VC_SM_MSG_TYPE_RESIZE,									// Resize a shared memory block //
	VC_SM_MSG_TYPE_WALK_ALLOC,								// Walk the allocated shared memory block(s) //
	VC_SM_MSG_TYPE_ACTION_CLEAN,							// A previously applied action will need to be reverted //
	VC_SM_MSG_TYPE_IMPORT,									// Import a physical address and wrap into a MEM_HANDLE_T - Release with VC_SM_MSG_TYPE_FREE.
	VC_SM_MSG_TYPE_CLIENT_VERSION,							// Tells VC the protocol version supported by this client. 2 supports the async/cmd messages from the VPU for final release of memory, and for VC allocations.
	VC_SM_MSG_TYPE_VC_MEM_REQUEST_REPLY,					// Response to VC request for memory //
															// Asynchronous/cmd messages supported for VC->HOST direction.
															// Signalled by setting the top bit in vc_sm_result_t trans_id.
															// VC has finished with an imported memory allocation.
															// Release any Linux reference counts on the underlying block.
	VC_SM_MSG_TYPE_RELEASED,
	VC_SM_MSG_TYPE_VC_MEM_REQUEST,							// VC request for memory //
	VC_SM_MSG_TYPE_MAX
	};
enum vc_sm_alloc_type_t 									// Type of memory to be allocated //
	{
	VC_SM_ALLOC_CACHED,
	VC_SM_ALLOC_NON_CACHED,
	};
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-VCHIQ.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum vchiq_mmal_es_type 
{
	MMAL_ES_TYPE_UNKNOWN,     								// Unknown elementary stream type //
	MMAL_ES_TYPE_CONTROL,     								// Elementary stream of control commands //
	MMAL_ES_TYPE_AUDIO,       								// Audio elementary stream //
	MMAL_ES_TYPE_VIDEO,       								// Video elementary stream //
	MMAL_ES_TYPE_SUBPICTURE   								// Sub-picture elementary stream //
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-MSG-COMMON.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum mmal_msg_status 
{
	MMAL_MSG_STATUS_SUCCESS = 0, 							// Success //
	MMAL_MSG_STATUS_ENOMEM,      							// Out of memory //
	MMAL_MSG_STATUS_ENOSPC,      							// Out of resources other than memory //
	MMAL_MSG_STATUS_EINVAL,      							// Argument is invalid //
	MMAL_MSG_STATUS_ENOSYS,      							// Function not implemented //
	MMAL_MSG_STATUS_ENOENT,      							// No such file or directory //
	MMAL_MSG_STATUS_ENXIO,       							// No such device or address //
	MMAL_MSG_STATUS_EIO,         							// I/O error //
	MMAL_MSG_STATUS_ESPIPE,      							// Illegal seek //
	MMAL_MSG_STATUS_ECORRUPT,    							// Data is corrupt \attention //
	MMAL_MSG_STATUS_ENOTREADY,   							// Component is not ready //
	MMAL_MSG_STATUS_ECONFIG,     							// Component is not configured //
	MMAL_MSG_STATUS_EISCONN,     							// Port is already connected //
	MMAL_MSG_STATUS_ENOTCONN,    							// Port is disconnected //
	MMAL_MSG_STATUS_EAGAIN,      							// Resource temporarily unavailable. //
	MMAL_MSG_STATUS_EFAULT,      							// Bad address //
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-PARAMETERS.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum mmal_parameter_common_type 	/* Common parameters */
{
	
	MMAL_PARAMETER_UNUSED = MMAL_PARAMETER_GROUP_COMMON,	/**< Never a valid parameter ID */
	MMAL_PARAMETER_SUPPORTED_ENCODINGS,						/**< MMAL_PARAMETER_ENCODING_T */
	MMAL_PARAMETER_URI,										/**< MMAL_PARAMETER_URI_T */
	MMAL_PARAMETER_CHANGE_EVENT_REQUEST,					/** MMAL_PARAMETER_CHANGE_EVENT_REQUEST_T */
	MMAL_PARAMETER_ZERO_COPY,								/** MMAL_PARAMETER_BOOLEAN_T */
	MMAL_PARAMETER_BUFFER_REQUIREMENTS,						/**< MMAL_PARAMETER_BUFFER_REQUIREMENTS_T */
	MMAL_PARAMETER_STATISTICS,								/**< MMAL_PARAMETER_STATISTICS_T */
	MMAL_PARAMETER_CORE_STATISTICS,							/**< MMAL_PARAMETER_CORE_STATISTICS_T */
	MMAL_PARAMETER_MEM_USAGE,								/**< MMAL_PARAMETER_MEM_USAGE_T */
	MMAL_PARAMETER_BUFFER_FLAG_FILTER,						/**< MMAL_PARAMETER_UINT32_T */
	MMAL_PARAMETER_SEEK,									/**< MMAL_PARAMETER_SEEK_T */
	MMAL_PARAMETER_POWERMON_ENABLE,							/**< MMAL_PARAMETER_BOOLEAN_T */
	MMAL_PARAMETER_LOGGING,									/**< MMAL_PARAMETER_LOGGING_T */
	MMAL_PARAMETER_SYSTEM_TIME,								/**< MMAL_PARAMETER_UINT64_T */
	MMAL_PARAMETER_NO_IMAGE_PADDING,						/**< MMAL_PARAMETER_BOOLEAN_T */
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-MSG-PORT.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum mmal_port_type 										// MMAL_PORT_TYPE_T //
{
	MMAL_PORT_TYPE_UNKNOWN = 0,								// Unknown port type //
	MMAL_PORT_TYPE_CONTROL,									// Control port //
	MMAL_PORT_TYPE_INPUT,									// Input port //
	MMAL_PORT_TYPE_OUTPUT,									// Output port //
	MMAL_PORT_TYPE_CLOCK,									// Clock port //
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-MSG.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum mmal_msg_type 
{
	MMAL_MSG_TYPE_QUIT = 1,
	MMAL_MSG_TYPE_SERVICE_CLOSED,
	MMAL_MSG_TYPE_GET_VERSION,
	MMAL_MSG_TYPE_COMPONENT_CREATE,
	MMAL_MSG_TYPE_COMPONENT_DESTROY,						// 5 //
	MMAL_MSG_TYPE_COMPONENT_ENABLE,
	MMAL_MSG_TYPE_COMPONENT_DISABLE,
	MMAL_MSG_TYPE_PORT_INFO_GET,
	MMAL_MSG_TYPE_PORT_INFO_SET,
	MMAL_MSG_TYPE_PORT_ACTION,								// 10 //
	MMAL_MSG_TYPE_BUFFER_FROM_HOST,
	MMAL_MSG_TYPE_BUFFER_TO_HOST,
	MMAL_MSG_TYPE_GET_STATS,
	MMAL_MSG_TYPE_PORT_PARAMETER_SET,
	MMAL_MSG_TYPE_PORT_PARAMETER_GET,						// 15 //
	MMAL_MSG_TYPE_EVENT_TO_HOST,
	MMAL_MSG_TYPE_GET_CORE_STATS_FOR_PORT,
	MMAL_MSG_TYPE_OPAQUE_ALLOCATOR,
	MMAL_MSG_TYPE_CONSUME_MEM,
	MMAL_MSG_TYPE_LMK,										// 20 //
	MMAL_MSG_TYPE_OPAQUE_ALLOCATOR_DESC,
	MMAL_MSG_TYPE_DRM_GET_LHS32,
	MMAL_MSG_TYPE_DRM_GET_TIME,
	MMAL_MSG_TYPE_BUFFER_FROM_HOST_ZEROLEN,
	MMAL_MSG_TYPE_PORT_FLUSH,								// 25 //
	MMAL_MSG_TYPE_HOST_LOG,
	MMAL_MSG_TYPE_MSG_LAST
};
enum mmal_msg_port_action_type 								// port action request messages differ depending on the action type //
{
	MMAL_MSG_PORT_ACTION_TYPE_UNKNOWN = 0,					// Unknown action //
	MMAL_MSG_PORT_ACTION_TYPE_ENABLE,						// Enable a port //
	MMAL_MSG_PORT_ACTION_TYPE_DISABLE,						// Disable a port //
	MMAL_MSG_PORT_ACTION_TYPE_FLUSH,						// Flush a port //
	MMAL_MSG_PORT_ACTION_TYPE_CONNECT,						// Connect ports //
	MMAL_MSG_PORT_ACTION_TYPE_DISCONNECT,					// Disconnect ports //
	MMAL_MSG_PORT_ACTION_TYPE_SET_REQUIREMENTS, 			// Set buffer requirements//
};