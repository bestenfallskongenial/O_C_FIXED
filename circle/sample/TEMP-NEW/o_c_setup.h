/*
        this file contains the system relevant macros that defines functionality
*/        
    #define         OS_VERSION                  2
    #define         OS_REVISION                 1

    #define         TARGET_FPS                  24

    #define 		LOGLEVEL				    2	                            // 0: only panic / 1: also errors / 2: also warnings / 3: also notices / 4: also debug output (default))

//  #define         USE_MIDI


    #define         __LOG_ALLOC__                                               // allocation / buffer allocation logging
    #define         __LOG_FILE__                                                // filesystem / file load-store logging
    #define         __LOG_MEMORY__                                              // logs the memory map, only used ones
    #define         __LOG_PARSER__                                              // parser / file parser logging
    #define         __LOG_MMAL__                                                // MMAL init/wrapper/message logging
    #define         __LOG_VC04__                                                // VC/VCHI service logging
    #define         __LOG_VCSM__                                                // VCSM alloc/import/lock logging
    #define         __LOG_INIT__                                                // kernel initialization step logging
    #define         __LOG_RUNTIME__         // maybe i should rename it...
//  #define         __LOG_GLSL__                                                // shader/program/uniform/gfx_check OpenGL logging
//  #define         __LOG_FIRST_FRAME__                                         // logs the fist frame vpu messaging - later useful
    #define         __LOG_TO_SCREEN__                                           // shows the log on the screen

    #define         __DEBUG_GL__                                                // GL/EGL runtime check() calls
    
    #define         __DEBUG_TIMING__

//  #define         __DUMP_HEADER__    
//  #define         __DUMP_GLSL__                                               // dump shader source from GL
//  #define         __DUMP_FRAMES__                                             // dump the parsed per frame data from per video
//  #define         __DUMP_VC04__                                               // detailed VC/VCHI message dump

//  #define         __SCROLLING__                                               // animated/intermediate log-screen scrolling frames
//  #define         __H264_DEBUG_TEX__                                          // H264 debug texture path

    #define         HEAP_DEBUG                                                  // heap dump / CMemorySystem::DumpStatus() - is already set!

// to prepare filecounter[FT_COUNT][FLD_COUNT]

    #define         VSH_SD             		1	// max number of u_vertex shader on sd
    #define         OMF_SD             		1	// max number of fragment shader on sd
    #define         FSH_SD             		1	// max number of fragment shader on sd

    #define         LFO_SD                  16  // max number of wavetables on sd
    
    #define         OMT_SD             		1	// max number of fragment shader on sd
    #define         TEX_SD             		1	// max number of textures on sd
    #define         VID_SD             		0	// max number of videos on sd
    #define         KLN_SD                  1   // max number of kernel.img on sd ( is loaded for the restore function in case of failed FW update )
    #define         FRM_SD                  1   // i put them here because if my mem/dma allocation
    #define         LOG_SD                  16  // here is the trick:

    #define         LOG_SYS_0               0   // m_bufferLog[] slot for INIT, ALLOC, FILE, MEMORY 
    #define         LOG_DUMMY_0             1
    #define         LOG_DUMMY_1             2
    #define         LOG_USERFILE            3

    #define         LOG_GLSL_0              4   // m_bufferLog[] slot for GLSL - shader / texture init
    #define         LOG_VCSM_0              5   // m_bufferLog[] slot for VCSM
    #define         LOG_MMAL_0              6   // m_bufferLog[] slot for MMAL / Framedata
    #define         LOG_TEX_0               7   // m_bufferLog[] slot for TEXTURES - parser

    #define         LOG_VID_0               8   // m_bufferLog[] slot for video 0 - parser  
    #define         LOG_VID_1               9
    #define         LOG_VID_2               10
    #define         LOG_VID_3               11

    #define         LOG_VID_4               12
    #define         LOG_VID_5               13
    #define         LOG_VID_6               14
    #define         LOG_VID_7               15

    #define         VSH_USB                 0	// max number of u_vertex shader on usb
    #define         OMF_USB            		0	// max number of fragment shader on usb
    #define         FSH_USB            		32	// max number of fragment shader on usb

    #define         LFO_USB                 0  // max number of wavetables on usb

    #define         OMT_USB            		0	// max number of fragment shader on usb
    #define         TEX_USB            		7	// max number of textures on usb
    #define         VID_USB            		8	// max number of videos on usb
    #define         KLN_USB                 1   // max number of kernel.img ( loaded if present for update ) 
    #define         FRM_USB                 0   // no additional frm "files"
    #define         LOG_USB                 0   // no additional log "files"
    #define         VSH_EXT                 1   // the file extensions 

    #define         OMF_EXT                 1
    #define         FSH_EXT                 1

    #define         LFO_SIZ                 (1024*32)           // 32kb

    #define         OMT_EXT                 1
    #define         TEX_EXT                 1
    #define         VID_EXT                 1
    #define         KLN_EXT                 1
    #define         VSH_SIZ                 (1024*32)           // 32kb
    #define         OMF_SIZ                 (1024*32)           // 32kb
    #define         FSH_SIZ                 (1024*32)           // 32kb
    #define         OMT_SIZ                 (1024*1024*4)       // 4mb
    #define         TEX_SIZ                 (1024*1024*4)       // 4mb
    #define         VID_SIZ                 (1024*1024*8)       // 8mb
    #define         KLN_SIZ                 (1024*1024*2)       // 2mb
    #define         FRM_SIZ                 (1024*1024)         // 1mb
    #define         LOG_SIZ                 (1024*128)          // 128kb


    #define         LOG_KERNEL_SIZE         (1024*32) // bigger will break my code!!

    #define         ADC_SELECT_PRG          7   // we will map this modes to input channels later

    #define         DEVICE_NAME_SD          "emmc1"
    #define         DEVICE_NAME_USB         "umsd1"
    #define         PARTITION_NAME_SD	    "emmc1-1"	                         // name partition sd dont confuse with device type    
    #define         PARTITION_NAME_USB	    "umsd1-1"	

    #define 		FILENAME_KNL			"kernel.img"                         // for logger
// LFO
    #define         LFO_SAMPLES             1024 // 256  	// number of samples per lfo waves
    
    #define 		LFO_INSTANCES			2
    #define			LFO_MULTIPLIERS_COUNT	7

    #define         LFO_AMPLITUDE           1024

    #define         SLOTS                   FSH_SD + FSH_USB + 1                // for the g_centralModeBuffer[SLOTS][MODETABLE_COUNT] array -  
                                                                                    // 1 firmware / 32 user / 1 default slot
    #define         DEFAULT_SLOT            FSH_SD + FSH_USB                    //  because 0 plus 34 = 33 ?? 
    #define         AUDIO_BUFFER_COUNT      64
    #define         AUDIO_THRESHOLD         160
    #define         AUDIO_HOLD_TIMEOUT      3000

    #define 		CHUNK_SIZE				1024                                // for the file load 

    #define			TIMEOUT					10                                  // for my watchdog         
// LED & WS2812 pin
    #define         LED_PIN		           	19		                            // led ( must be between 8 .. 23 )
    #define         LED_COUNT		        4		                            // number of LEDs on the stripes
    
    #define         LED_A                   2
    #define         LED_B                   1
    #define         LED_C                   3
    #define         LED_D                   0
//  buttons
    #define         BUTTON_COUNT            2                                   // was addon/sensors/buttons.h is now buttonPing()  no class required 
    #define         SW_PIN_A	            13                                  // button A ( lower )
    #define         SW_PIN_B	          	6                                   // button B ( upper )
// control / adc routing pin
    #define         CTRL_PIN                12                                  // for menu input selector!!!
// media
    #define         MAX_OMF                 1
    #define         MAX_SHADER              33
    #define         MAX_VIDEOS              8
    #define         MAX_TEXTURE             8
    #define         MAX_FRAMES              2048

    #define         MIN_VID_W               640
    #define         MAX_VID_W               800

    #define         MIN_VID_H               480    
    #define         MAX_VID_H               600

    #define         MIN_VID_P               66		// Baseline
    #define         MAX_VID_P               66		// Baseline

    #define         MIN_VID_L               30
    #define         MAX_VID_L               52      // misconception, the parser has only one value,  not a range! was 51

//  #define SCREEN_LOG_BUFFER_SIZE  (1024 * 8)

    #define         SCROLLSPEED             5 // in 1000/1th

    #define         DOUBLE_CLICK_TIME       500000
    #define         LONG_CLICK_TIME         1000000

    #define         FLAG_THRESHOLD          7

