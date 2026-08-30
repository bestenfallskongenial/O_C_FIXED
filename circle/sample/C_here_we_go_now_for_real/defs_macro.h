    #define             X_STR                                               0 // ""                                                 // for the logger
    #define             X_VAL                                               255                                                // for the logger


    #define             BPM_NUM                                             9
    #define             BPM_DEN                                             8   // 1.25  = 5/4   -> *4 < *5
                                                                                // 1.2   = 6/5   -> *5 < *6
                                                                                // 1.125 = 9/8   -> *8 < *9
// #define             LAYER                                                6

// #define             BLOCK_COUNT                                          8  // <---- ****** confusion?!?! i need to be this as single source of trueh or derived from g_centralModeBuffer

    #define BLOCK_WIDTH                                                     4
    #define BLOCK_COUNT                                                     (MODETABLE_COUNT / BLOCK_WIDTH)
#ifdef USE_MIDI    
    #define ACCESSIBLE_LAYER                                                8
#else
    #define ACCESSIBLE_LAYER                                                7
#endif
    #define             check() 				                            gfx_check(__FILE__, __LINE__) 	// my own assertiion implimentation

    #define             MAX_WATCHDOG_SEC                                    15

    #define             MAX_FILE_NAME_LENGTH                                13

    #define             BTN_PRESSED                                         0

    #define             CHANNEL                                             8

    #define             MAX_TILES                                           16
    #define             MENU_GPU_TILE_COUNT                                 16

    #define             ADC_CHANNELS         	                            8                                   // number of adc input channels ( dependency <sensor/mcp300x.h>	)
    #define             VREF			 		                            5.0f

    #define 		    ADC_BUFFER_COUNT		                            4	

    #define             NAL_TYPE_SLICE                                      1                                // NAL unit types
    #define             NAL_TYPE_IDR                                        5
    #define             NAL_TYPE_SEI                                        6
    #define             NAL_TYPE_SPS                                        7
    #define             NAL_TYPE_PPS                                        8

    #define             MMAL_FORMAT_EXTRADATA_MAX_SIZE                      128              // Maximum size of the format extradata. //


// SPI
    #define         SPI0_BASE                                           (ARM_IO_BASE + 0x204000)

    #define         SPI_MASTER_DEVICE	 	                            0		                            // 0    m_SPIMaster ( also for the mcp3008 )
    #define         SPI_CHIP_SELECT		 	                            0		                            // 0
    #define         SPI_CLOCK_SPEED	   		                            1000000		                        // Hz

    #define         ARM_SPI_CS                                          (m_SPIBaseAddress + 0x00)
    #define         ARM_SPI_FIFO                                        (m_SPIBaseAddress + 0x04)
    #define         ARM_SPI_CLK                                         (m_SPIBaseAddress + 0x08)
    #define         ARM_SPI_DLEN                                        (m_SPIBaseAddress + 0x0C)

    #define         CS_TXD                                              (1 << 18)
    #define         CS_RXD                                              (1 << 17)
    #define         CS_DONE                                             (1 << 16)
    #define         CS_TA                                               (1 << 7)
    #define         CS_CLEAR_RX                                         (1 << 5)
    #define         CS_CLEAR_TX                                         (1 << 4)

    #define         CS_CPOL_SHIFT                                       3
    #define         CS_CPHA_SHIFT                                       2

    #define         CS_CS                                               (3 << 0)
    #define         CS_CS_SHIFT                                         0
    #define         CS_NONE                                             3
// GPIO PINS FOR THE SPI
    #define         MISO                                                9
    #define         MOSI                                                10
    #define         SCLK                                                11

    #define         CE0                                                 8
    #define         CE1                                                 7

    #define         GPIO_ALT0                                           4
    #define         GPIO_ALT1                                           5
// SMI
    #define         MY_SMI_BASE                                         (ARM_IO_BASE + 0x600000)

    #define         ARM_SMI_CS                                          (MY_SMI_BASE + 0x00)
    #define         ARM_SMI_L                                           (MY_SMI_BASE + 0x04)
    #define         ARM_SMI_A                                           (MY_SMI_BASE + 0x08)
    #define         ARM_SMI_D                                           (MY_SMI_BASE + 0x0C)
    #define         ARM_SMI_DSR0                                        (MY_SMI_BASE + 0x10)
    #define         ARM_SMI_DSW0                                        (MY_SMI_BASE + 0x14)
    #define         ARM_SMI_DMC                                         (MY_SMI_BASE + 0x30)
    #define         ARM_SMI_DCS                                         (MY_SMI_BASE + 0x34)
    #define         ARM_SMI_DCA                                         (MY_SMI_BASE + 0x38)

    #define         SMI_CS_ENABLE                                       (1 << 0)
    #define         SMI_CS_START                                        (1 << 3)
    #define         SMI_CS_CLEAR                                        (1 << 4)
    #define         SMI_CS_WRITE                                        (1 << 5)
    #define         SMI_CS_SETERR                                       (1 << 13)
    #define         SMI_CS_PXLDAT                                       (1 << 14)

    #define         SMI_WSTROBE_SHIFT                                   0
    #define         SMI_WPACE_SHIFT                                     8
    #define         SMI_WHOLD_SHIFT                                     16
    #define         SMI_WSETUP_SHIFT                                    24
    #define         SMI_WWIDTH_SHIFT                                    30

    #define         SMI_DMC_REQW_SHIFT                                  0
    #define         SMI_DMC_REQR_SHIFT                                  6
    #define         SMI_DMC_PANICW_SHIFT                                12
    #define         SMI_DMC_PANICR_SHIFT                                18
    #define         SMI_DMC_DMAEN                                       (1 << 28)

    #define         SMI_DMA_REQUEST_THRESH                              2
    #define         SMI_DMA_PANIC_LEVEL                                 8

    #define         CM_SMICTL_BUSY                                      (1 << 7)
    #define         CM_SMICTL_KILL                                      (1 << 5)
    #define         CM_SMICTL_ENAB                                      (1 << 4)

    #define         CM_SMIDIV_DIVI_SHIFT                                12

    #define         SMI_WIDTH_16                                        1
 
// WS2812 timing
    #define         NEOPIXEL_SMI_NS                                     10
    #define         NEOPIXEL_SMI_SETUP                                  10
    #define         NEOPIXEL_SMI_STROBE                                 20
    #define         NEOPIXEL_SMI_HOLD                                   10

    #define         NEOPIXEL_SMI_WIDTH		                            (LED_NCHANS > 8 ? SMI16Bits : SMI8Bits) // ?
    #define         NEOPIXEL_SMI_PACE                                   0
// WS2812 buffer layout
    #define         LED_NBITS                                           24
    #define         BIT_NPULSES                                         3
    #define         LED_PREBITS                                         4
    #define         LED_POSTBITS                                        4

    #define         LED_DLEN                                            (LED_NBITS * BIT_NPULSES)
    #define         LED_TX_OSET(n)                                      (LED_PREBITS + (LED_DLEN * (n)))
    #define         TX_BUFF_LEN(n)                                      (LED_TX_OSET(n) + LED_POSTBITS)

    #define         TXDATA_T                                            u16
// MPC 3008 
    #define         SPI_CHIP_SELECT                                     0
// GPIO
    #define         GPIO_PULL_OFF                                       0
    #define         GPIO_PULL_DOWN                                      1
    #define         GPIO_PULL_UP                                        2

    #define         LOW                                                 0
    #define         HIGH                                                1
// macros.h - watchdog
    #define         PACKED                                              __attribute__ ((packed))
    #define         MAXALIGN                                            __attribute__ ((aligned))
    #define         ALIGN(n)                                            __attribute__ ((aligned (n)))
    #define         NORETURN                                            __attribute__ ((noreturn))

#ifndef __clang__
    #define         NOOPT                                               __attribute__ ((optimize (0)))
    #define         STDOPT                                              __attribute__ ((optimize (2)))
    #define         MAXOPT                                              __attribute__ ((optimize (3)))
    #else
    #define         NOOPT
    #define         STDOPT
    #define         MAXOPT
#endif

    #define         WEAK                                                __attribute__ ((weak))

    #define         likely(exp)                                         __builtin_expect (!!(exp), 1)
    #define         unlikely(exp)                                       __builtin_expect (!!(exp), 0)

    #define         BIT(n)                                              (1U << (n))

    #define         IS_POWEROF_2(num)                                   ((num) != 0 && (((num) & ((num) - 1)) == 0))

    #define         BE(value)                                           ((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

        // vcsm and mmal 
    #define         VC_SM_VER                                           1    // Version to be reported to the VPU VPU assumes 0 (aka 1) which does not require the released callback
    #define         VC_SM_MIN_VER                                       0 // nor expect the client to handle VC_MEM_REQUESTS. Version 2 requires the released callback, and must support VC_MEM_REQUESTS.

    #define         VC_SM_MAX_MSG_LEN                                   (sizeof(union vc_sm_msg_union_t) + sizeof(struct vc_sm_msg_hdr_t))
    #define         VC_SM_MAX_RSP_LEN                                   (sizeof(union vc_sm_msg_union_t))

    #define         VC_SM_RESOURCE_NAME                                 32				// Resource name maximum size //

    #define         VC_MMAL_VER                                         15
    #define         VC_MMAL_MIN_VER                                     10

    #define         MMAL_FOURCC(a, b, c, d) ((a) | (b << 8) | (c << 16) | (d << 24)) // two times!
    #define         MMAL_MAGIC 						                    MMAL_FOURCC('m', 'm', 'a', 'l')

    #define         MMAL_EVENT_ERROR				                    MMAL_FOURCC('E', 'R', 'R', 'O')	// really????
    #define         MMAL_EVENT_EOS					                    MMAL_FOURCC('E', 'E', 'O', 'S')
    #define         MMAL_EVENT_FORMAT_CHANGED		                    MMAL_FOURCC('E', 'F', 'C', 'H')
    #define         MMAL_EVENT_PARAMETER_CHANGED	                    MMAL_FOURCC('E', 'P', 'C', 'H')
    // max total message size is 512 bytes //
    #define         MMAL_MSG_MAX_SIZE                                   512
    // with six 32bit header elements max payload is therefore 488 bytes //
    #define         MMAL_MSG_MAX_PAYLOAD                                488
    // #define MMAL_TIME_UNKNOWN BIT_ULL(63)                               // Special value signalling that time is not known //
    // #define MMAL_TIME_UNKNOWN (1ULL << 63)
    #define         BIT(n)                                              (1U << (n))
    #define         BIT_ULL(n)                                          (1ULL << (n))
    #define         MMAL_TIME_UNKNOWN                                   BIT_ULL(63)

    #define         NUMBER_INPUTBUFFER                                  1
    #define         NUMBER_OUTPUTBUFFER                                 2

//  #define MIN_BUFFERS 2                                                 // from CKernel ??
//  #define MAX_BUFFER 8 // ???

    #define         VC_SM_PROTOCOL_VERSION	                            2 // ???
    //              FROM MMAL-VCHIQ.H
    #define         MAX_PORT_COUNT                                      4
    // Maximum size of the format extradata. //
    #define         MMAL_FORMAT_EXTRADATA_MAX_SIZE                      128
    //              FROM MMAL-VCHIQ.H
    #define         MAX_PORT_COUNT 4
    // Maximum size of the format extradata. //
    #define         MMAL_FORMAT_EXTRADATA_MAX_SIZE                      128
    //              FROM MMAL-PARAMETERS.H
    #define         MMAL_PARAMETER_GROUP_COMMON	                        (0 << 16)
    //              FROM MMAL-MSG-PORT.H what do we really need here?
    // The port is pass-through and doesn't need buffer headers allocated //
    #define         MMAL_PORT_CAPABILITY_PASSTHROUGH                    0x01
    //The port wants to allocate the buffer payloads.
    // This signals a preference that payload allocation should be done
    // on this port for efficiency reasons.
    #define         MMAL_PORT_CAPABILITY_ALLOCATION                     0x02
    // The port supports format change events.
    // This applies to input ports and is used to let the client know
    // whether the port supports being reconfigured via a format
    // change event (i.e. without having to disable the port).
    #define         MMAL_PORT_CAPABILITY_SUPPORTS_EVENT_FORMAT_CHANGE   0x04
    // mmal port structure (MMAL_PORT_T)
    //
    // most elements are informational only, the pointer values for
    // interogation messages are generally provided as additional
    // structures within the message. When used to set values only the
    // buffer_num, buffer_size and userdata parameters are writable.
    //              FROM MMAL-MSG.H
    // MMAL buffer transfer //
    
    #define         MMAL_VC_SHORT_DATA 128                                      // Size of space reserved in a buffer message for short messages. //
    #define         MMAL_BUFFER_HEADER_FLAG_EOS                         BIT(0)       // Signals that the current payload is the end of the stream of data //
    #define         MMAL_BUFFER_HEADER_FLAG_FRAME_START                 BIT(1)       // Signals that the start of the current payload starts a frame //
    #define         MMAL_BUFFER_HEADER_FLAG_FRAME_END                   BIT(2)       // Signals that the end of the current payload ends a frame //
    #define         MMAL_BUFFER_HEADER_FLAG_FRAME \
                    (MMAL_BUFFER_HEADER_FLAG_FRAME_START | \
                    MMAL_BUFFER_HEADER_FLAG_FRAME_END)                              // Signals that the current payload contains only complete frames (>1) //
    #define         MMAL_BUFFER_HEADER_FLAG_KEYFRAME                    BIT(3)       // Signals that the current payload is a keyframe (i.e. self decodable) //
    #define         MMAL_BUFFER_HEADER_FLAG_DISCONTINUITY               BIT(4)       // Signals a discontinuity in the stream of data (e.g. after a seek). Can be used for instance by a decoder to reset its state
    #define         MMAL_BUFFER_HEADER_FLAG_CONFIG                      BIT(5)       // Signals a buffer containing some kind of config data for the component (e.g. codec config data)
    #define         MMAL_BUFFER_HEADER_FLAG_ENCRYPTED                   BIT(6)       // Signals an encrypted payload //
    #define         MMAL_BUFFER_HEADER_FLAG_CODECSIDEINFO               BIT(7)       // Signals a buffer containing side information //
    #define         MMAL_BUFFER_HEADER_FLAGS_SNAPSHOT                   BIT(8)       // Signals a buffer which is the snapshot/postview image from a stills capture
    #define         MMAL_BUFFER_HEADER_FLAG_CORRUPTED                   BIT(9)       // Signals a buffer which contains data known to be corrupted //
    #define         MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED         BIT(10)      // Signals that a buffer failed to be transmitted //
    // Video buffer header flags
    // videobufferheaderflags
    // The following flags describe properties of a video buffer header.
    // As there is no collision with the MMAL_BUFFER_HEADER_FLAGS_ defines, these
    
    #define         MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START_BIT   16
    #define         MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START \
                    (1 << MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START_BIT)// flags will also be present in the MMAL_BUFFER_HEADER_T flags field.
    
    #define         MMAL_BUFFER_HEADER_VIDEO_FLAG_INTERLACED \
                    (MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START << 0)    // Signals an interlaced video frame //
    
    // 
    #define         MMAL_BUFFER_HEADER_VIDEO_FLAG_TOP_FIELD_FIRST \
                    (MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START << 1)    // Signals that the top field of the current interlaced frame should be displayed first
    // port parameter setting //
    #define         MMAL_WORKER_PORT_PARAMETER_SPACE                    96        
    // event messages //
    #define         MMAL_WORKER_EVENT_SPACE                             256
    
//  #define         MMAL_FOURCC(a, b, c, d) ((a) | (b << 8) | (c << 16) | (d << 24))    // Four CC's for events //

    #define         MMAL_EVENT_ERROR		                            MMAL_FOURCC('E', 'R', 'R', 'O')
    #define         MMAL_EVENT_EOS			                            MMAL_FOURCC('E', 'E', 'O', 'S')
    #define         MMAL_EVENT_FORMAT_CHANGED	                        MMAL_FOURCC('E', 'F', 'C', 'H')
    #define         MMAL_EVENT_PARAMETER_CHANGED                        MMAL_FOURCC('E', 'P', 'C', 'H')
    //              FROM MMAL-ENCODINGS.H
    #define         MMAL_ENCODING_H264                                  MMAL_FOURCC('H', '2', '6', '4')
    #define         MMAL_ENCODING_I420                                  MMAL_FOURCC('I', '4', '2', '0')
    #define         MMAL_ENCODING_EGL_IMAGE                             MMAL_FOURCC('E', 'G', 'L', 'I')     // An EGL image handle
    // Pre-defined H264 encoding variants //
    #define         MMAL_ENCODING_VARIANT_H264_DEFAULT                  0                                   // ISO 14496-10 Annex B byte stream format //
    #define         MMAL_ENCODING_VARIANT_H264_AVC1                     MMAL_FOURCC('A', 'V', 'C', '1')    // ISO 14496-15 AVC stream format //
    #define         MMAL_ENCODING_VARIANT_H264_RAW                      MMAL_FOURCC('R', 'A', 'W', ' ')    // Implicitly delineated NAL units without emulation prevention //
    // \defgroup MmalColorSpace List of pre-defined video color spaces
    // This defines a list of common color spaces. This list isn't exhaustive and
    // is only provided as a convenience to avoid clients having to use FourCC
    // codes directly. However components are allowed to define and use their own
    // FourCC codes.
    #define         MMAL_COLOR_SPACE_UNKNOWN                            0                                        // Unknown color space //
    #define         MMAL_COLOR_SPACE_ITUR_BT601                         MMAL_FOURCC('Y', '6', '0', '1')          // ITU-R BT.601-5 [SDTV] //
    #define         MMAL_COLOR_SPACE_ITUR_BT709                         MMAL_FOURCC('Y', '7', '0', '9')          // ITU-R BT.709-3 [HDTV] //
    #define         MMAL_COLOR_SPACE_JPEG_JFIF                          MMAL_FOURCC('Y', 'J', 'F', 'I')          // JPEG JFIF //
    #define         MMAL_COLOR_SPACE_FCC                                MMAL_FOURCC('Y', 'F', 'C', 'C')          // Title 47 Code of Federal Regulations (2003) 73.682 (a) (20) //
    #define         MMAL_COLOR_SPACE_SMPTE240M                          MMAL_FOURCC('Y', '2', '4', '0')          // Society of Motion Picture and Television Engineers 240M (1999) //
    #define         MMAL_COLOR_SPACE_BT470_2_M                          MMAL_FOURCC('Y', '_', '_', 'M')          // ITU-R BT.470-2 System M //
    #define         MMAL_COLOR_SPACE_BT470_2_BG                         MMAL_FOURCC('Y', '_', 'B', 'G')          // ITU-R BT.470-2 System BG //
    #define         MMAL_COLOR_SPACE_JFIF_Y16_255                       MMAL_FOURCC('Y', 'Y', '1', '6')          // JPEG JFIF, but with 16..255 luma //
