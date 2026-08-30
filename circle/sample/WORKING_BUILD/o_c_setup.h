    #define 		LOGLEVEL				    4	                            // 0: only panic 
                                                                                // 1: also errors 
                                                                                // 2: also warnings 
                                                                                // 3: also notices 
                                                                                // 4: also debug output (default))
    
    #define __DEBUG_LOG__          // generic debug log blocks

    #define __LOG_ALLOC__          // allocation / buffer allocation logging
    #define __LOG_FILE__           // filesystem / file load-store logging
    #define __LOG_PARSER__         // parser / file parser logging
    #define __LOG_MMAL__           // MMAL init/wrapper/message logging
    #define __LOG_VC04__           // VC/VCHI service logging
//  #define __LOG_VC04_MSG__       // detailed VC/VCHI message dump
    #define __LOG_VCSM__           // VCSM alloc/import/lock logging
    #define __LOG_INIT__           // kernel initialization step logging
    #define __LOG_GL__             // shader/program/uniform/gfx_check OpenGL logging

    #define __DEBUG_CHECK__           // GL/EGL runtime check() calls
    #define __DEBUG_OGL__          // older assert/check based OGL init debug
    #define __SCROLLING__          // animated/intermediate log-screen scrolling frames
//  #define __SHADER_DUMP__        // dump shader source from GL
//  #define __H264_DEBUG_TEX__     // H264 debug texture path

    #define HEAP_DEBUG             // heap dump / CMemorySystem::DumpStatus()

//  #define         LOG_GL  0
//  #define         LOG_BMP 8

    // to prepare filecounter[FT_COUNT][FLD_COUNT]
    #define         VSH_SD             		    1	// max number of u_vertex shader on sd
    #define         OMF_SD             		    1	// max number of fragment shader on sd
    #define         FSH_SD             		    1	// max number of fragment shader on sd
    #define         OMT_SD             		    1	// max number of fragment shader on sd
    #define         TEX_SD             		    0	// max number of textures on sd
    #define         VID_SD             		    0	// max number of videos on sd
    #define         KLN_SD                      1

    #define         FRM_SD                      1   // i put them here because if my mem/dma allocation
    #define         LOG_SD                      16  // here is the trick:

                                                    //  #define MY_BUFFER   m_bufferLog
                                                    //  #define MY_INDEX    m_bufferLogIndex

                                                    //  for (int i = 0; i < loadedVideos; i++)
                                                    //      {
                                                    //      storeLog( MY_BUFFER[i], MY_INDEX[i], ... );
                                                    //      }

                                                    // m_bufferLog[0 - 7]   - video parser
                                                    // m_bufferLog[8]       - log boot/init
                                                    // m_bufferLog[9]       - glsl compile
                                                    // m_bufferLog[10]      - gfx init
                                                    // m_bufferLog[11]      - gfx runtime
                                                    // m_bufferLog[12]      - vc_sm init
                                                    // m_bufferLog[13]      - mmal init
                                                    // m_bufferLog[14]      - mmal runtime                                                
                                                    // m_bufferLog[15]      - texture parser

    #define         VSH_USB                     0	// max number of u_vertex shader on sd
    #define         OMF_USB            		    0	// max number of fragment shader on sd
    #define         FSH_USB            		    32	// max number of fragment shader on sd
    #define         OMT_USB            		    0	// max number of fragment shader on sd
    #define         TEX_USB            		    8	// max number of textures on sd
    #define         VID_USB            		    8	// max number of videos on sd
    #define         KLN_USB                     1

    #define         FRM_USB                     0   // i put them here because if my mem/dma allocation
    #define         LOG_USB                     0

    #define         VSH_EXT                     1
    #define         OMF_EXT                     1
    #define         FSH_EXT                     1
    #define         OMT_EXT                     1
    #define         TEX_EXT                     1
    #define         VID_EXT                     1
    #define         KLN_EXT                     1

    #define         VSH_SIZ                     (1024*32)           // 32kb
    #define         OMF_SIZ                     (1024*32)
    #define         FSH_SIZ                     (1024*32)
    #define         OMT_SIZ                     (1024*1024*4)       // 4mb
    #define         TEX_SIZ                     (1024*1024*4)
    #define         VID_SIZ                     (1024*1024*8)       // 8mb
    #define         KLN_SIZ                     (1024*1024*2)       // 2mb

    #define         FRM_SIZ                     (1024*1024)         // 1mb

    #define         LOG_SIZ                     (1024*128)          // 256kb

    // #define LOG_SIZ 262144

    #define         ADC_SELECT_PRG              7   // we will map this modes to input channels later
    #define         ADC_SELECT_TEX              6   // this are placeholder mactos
    #define         ADC_SELECT_VID              5
    #define			ADC_INPUT_CLK			    5	// adc p_channel use as clock ! BIGGER THAN 3 !
    #define         ADC_INPUT_TIME              4

    #define         DEVICE_NAME_SD              "emmc1"
    #define         DEVICE_NAME_USB             "umsd1"

    #define         PARTITION_NAME_SD	        "emmc1-1"	                         // name partition sd dont confuse with device type    
    #define         PARTITION_NAME_USB	        "umsd1-1"	

    #define 		FILENAME_KNL			    "kernel.img"                         // for logger

    #define         Filename_SYS_LOG            "LOG-SYS.txt"
    #define 		FILENAME_GL_LOG			    "LOG-GLES.txt"                       // names for my log files 
    #define 		FILENAME_GLERROR		    "LOG-GLER.txt"
    #define 		FILENAME_VCSM_LOG		    "LOG-VCSM.txt"
    #define 		FILENAME_MMAL_LOG		    "LOG-MMAL.txt"    

    #define         WAVEFORMS_COUNT             4		// number of lfo waves
    #define         WAVESAMPLES            	    256  	// number of samples per lfo waves

    #define         MAX_MODES                   5
    #define         MENU_LAYER_COUNT            4

    #define 		LFO_INSTANCES			    2
    #define			LFO_MULTIPLIERS_COUNT		7

    #define         SLOTS                       34                                  // for the g_centralModeBuffer[SLOTS][MODETABLE_COUNT] array -  
                                                                                    // 1 firmware / 32 user / 1 default slot
    #define         DEFAULT_SLOT                33                                  //  because 0 plus 34 = 33 ?? 

    #define         AUDIO_THRESHOLD             160
    #define         AUDIO_BUFFER_COUNT          63
    #define         AUDIO_MENU_HOLD             3000

    #define 		CHUNK_SIZE				    1024                                // for the file load 

    #define			TIMEOUT					    10                                  // for my watchdog         
    // WS2812 pin
    #define         LED_PIN		           	    19		                            // led ( must be between 8 .. 23 )
    #define         LED_COUNT		       	    4		                            // number of LEDs on the stripes   

    #define         BUTTON_COUNT                2                                   // was addon/sensors/buttons.h is now buttonPing()  no class required 
    #define         SW_PIN_A	           	    13                                  // button A ( lower )
    #define         SW_PIN_B	          	    6                                   // button B ( upper )

    #define         CTRL_PIN                 	    12                                  // for menu input selector!!!

  
    #define         MAX_TEXTURE                 8

    #define         MAX_SHADER                  33
    #define         MAX_OMF                     1

    #define         MAX_VIDEOS                  8

    #define         MAX_FRAMES                  2048

    #define         MAX_VIDEO_WIDTH             640
    #define         MAX_VIDEO_HEIGHT            480

    #define         MAX_VIDEO_PROFILE           66		// Baseline

    #define         MIN_VIDEO_LEVEL             30
    #define         MAX_VIDEO_LEVEL             52      // missconception, the parser has only one value,  not a range! was 51

    #define SCREEN_LOG_BUFFER_SIZE  (1024 * 8)

    #define SCROLLSPEED 5 // in 1000/1th

    #define HEAP_DEBUG