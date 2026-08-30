    // home for the global defining macros
    #define 		LOGLEVEL				4	                                // 0: only panic 
                                                                                // 1: also errors 
                                                                                // 2: also warnings 
                                                                                // 3: also notices 
                                                                                // 4: also debug output (default))

    #define			TIMEOUT					10                                  // for my watchdog                                                                  

    #define         CS_PIN                 	12                                  // for menu input selector!!!

    #define         NR_BUTTONS              2                                   // was addon/sensors/buttons.h is now buttonPing()  no class required 
    #define         SW_PIN_A	           	13                                  // button A ( lower )
    #define         SW_PIN_B	          	6                                   // button B ( upper )

    #define         LED_PIN		           	19		                            // led ( must be between 8 .. 23 )
    #define         LED_COUNT		       	4		                            // number of LEDs on the stripes

    #define         ADC_CHANNELS         	8                                   // number of adc input channels ( dependency <sensor/mcp300x.h>	)
    #define         VREF			 		5.0f

    #define         SPI_MASTER_DEVICE	 	0		                            // 0    m_SPIMaster ( also for the mcp3008 )
    #define         SPI_CHIP_SELECT		 	0		                            // 0
    #define         SPI_CLOCK_SPEED	   		1000000		

    #define         MAX_VIDEO_WIDTH         640                                 // want to a) get this from config.txt b) defoverride this the config.txt 
    #define         MAX_VIDEO_HEIGHT        480                                     
    #define         MAX_VIDEO_PROFILE        66		                            // Baseline
    #define         MAX_VIDEO_LEVEL          41                                 // ? taken from 

    #define 		ADC_BUFFER			 	4	

    #define         SLOTS                   34                                  // for the g_centralModeBuffer[SLOTS][modetablecount] array -  
                                                                                // 1 firmware / 32 user / 1 default slot
    #define         DEFAULT_SLOT            33                                  //  because 0 plus 34 = 33 ?? 






    #define         AUDIO_THRESHOLD         160
    #define         MAX_AUDIO_BUFFER        63


    #define         TARGET_FPS              24                                  // desired frame rate !!! becomes menu !!!
    #define 		MIN_BPM					10		                            // min bpm for adc selector 
    #define			MAX_BPM					240 	                            // max bpm for adc selector

    #define 		NUMBER_OF_MODES			6       // is now defined in the g_modeMap array

    #define         WAVEFORMS             	4		// number of lfo waves
    #define         WAVESAMPLES            	256  	// number of samples per lfo waves

    #define 		LFO_INSTANCES			2
    #define			LFO_MULTIPLIERS			7


    // to prepare filecounter[FT_COUNT][FLD_COUNT]
    #define         VSH_SD             		    1	// max number of u_vertex shader on sd
    #define         OMF_SD             		    1	// max number of fragment shader on sd
    #define         FSH_SD             		    1	// max number of fragment shader on sd
    #define         OMT_SD             		    1	// max number of fragment shader on sd
    #define         TEX_SD             		    0	// max number of textures on sd
    #define         VID_SD             		    0	// max number of videos on sd
    #define         KLN_SD                      1

    #define         FRM_SD                      1   // i put them here because if my mem/dma allocation
    #define         LOG_SD                      24  // here is the trick:
                                                // i will use a mix of hardwired and "open" logs. examples:
                                                // just changed the model here. got one global log // screen buffer defined as member to access it on boot ( before alloc )
                                                // than a common log buffer for the file operations, mem-alloc, bmp parser, the annex b parser, the vcsm init, the mmal init, the mmal runtime 

                                                // m_bufferLog[0]       - startup - memory - init log
                                                // m_bufferLog[1]       - vc_sm
                                                // m_bufferLog[2]       - mmal
                                                // m_bufferLog[3]       - check glsl
                                                // m_bufferLog[4]       - glsl compile
                                                // m_bufferLog[5  - 15] - texture parser
                                                // m_bufferLog[16 - 23] - video parser

    #define         VSH_USB                     0	// max number of u_vertex shader on sd
    #define         OMF_USB            		    0	// max number of fragment shader on sd
    #define         FSH_USB            		    32	// max number of fragment shader on sd
    #define         OMT_USB            		    0	// max number of fragment shader on sd
    #define         TEX_USB            		    8	// max number of textures on sd
    #define         VID_USB            		    8	// max number of videos on sd
    #define         KLN_USB                     1

    #define         FRM_USB                     1   // i put them here because if my mem/dma allocation
    #define         LOG_USB                     1

    #define         VSH_EXT                     1
    #define         OMF_EXT                     1
    #define         FSH_EXT                     1
    #define         OMT_EXT                     1
    #define         TEX_EXT                     1
    #define         VID_EXT                     1
    #define         KLN_EXT                     1

    #define         VSH_SIZ                     (1024*32)
    #define         OMF_SIZ                     (1024*32)
    #define         FSH_SIZ                     (1024*32)
    #define         OMT_SIZ                     (1024*1024*4)
    #define         TEX_SIZ                     (1024*1024*4)
    #define         VID_SIZ                     (1024*1024*8)
    #define         KLN_SIZ                     (1024*1024*2)

    #define         FRM_SIZ                     (1024*1024)

    #define         LOG_SIZ                     (1024*64)

    #define         ADC_SELECT_PRG              7
    #define         ADC_SELECT_TEX              6
    #define         ADC_SELECT_VID              5
    #define			ADC_INPUT_CLK			    5	// adc p_channel use as clock ! BIGGER THAN 3 !

    #define 		FILENAME_KNL			"kernel.img"                         // for logger

    #define         PARTITIONSD	            "emmc1-1"	                         // name partition sd dont confuse with device type    
    #define         PARTITIONUSB	        "umsd1-1"	

    #define         Filename_SYS_LOG        "LOG-SYS.txt"
    #define 		FILENAME_GL_LOG			"LOG-GLES.txt"                       // names for my log files 
    #define 		FILENAME_GLERROR		"LOG-GLER.txt"
    #define 		FILENAME_VCSM_LOG		"LOG-VCSM.txt"
    #define 		FILENAME_MMAL_LOG		"LOG-MMAL.txt"
