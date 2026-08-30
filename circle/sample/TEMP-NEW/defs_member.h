private:        // circle system requirements
                CActLED                         m_ActLED;
                CKernelOptions                  m_Options;          
                CMachineInfo                    m_MachineInfo;
                CDeviceNameService              m_DeviceNameService;
                CExceptionHandler               m_ExceptionHandler;

                CInterruptSystem                m_Interrupt;
                CTimer                          m_Timer;
                CLogger                         m_Logger;
                CCPUThrottle                    m_CPUThrottle;  // NEW!!!    

                CMemorySystem                   m_Memory;

                CBcmFrameBuffer                 gE_FrameBuffer;

                CDMAChannel                     m_SMITxDMA;                                     // SMI

                CEMMCDevice                     m_EMMC;
                CUSBHCIDevice                   m_USBHCI;
                CVCHIQDevice                    m_VCHIQ;

    volatile    bool                            m_bStorageAttached                              = false;
                CFATFileSystem*                 m_pFileSystem;                                  // where to put the *?
                bool                            m_resetFlag                                     = false;

                CScheduler                      m_Scheduler;

                CCharGenerator                  gE_CharGenerator;

                u32*                            gE_PixelBuffer                                  = nullptr;      // frameBufferInit() & logScreenUpdate()
                unsigned                        gE_PitchBytes                                   = 0;
                unsigned                        gE_ScreenWidth                                  = 0;
                unsigned                        gE_ScreenHeight                                 = 0;
                unsigned                        gE_CharWidth                                    = 0;
                unsigned                        gE_CharHeight                                   = 0;
                unsigned                        gE_Cols                                         = 0;
                unsigned                        gE_Rows                                         = 0;
// SMI / DMA / WS2812
                uintptr                         m_SPIBaseAddress                                = 0;
                bool                            m_SPIValid                                      = false;

                unsigned                        m_SMIGpioPin                                    = 0;
                unsigned                        m_SMISDMask                                     = 0;
                unsigned                        m_LEDCount                                      = 0;
                unsigned                        m_BufferLength                                  = 0;
                TXDATA_T*                       m_pBuffer                                       = 0;

                bool                            m_SMIValid                                      = false;
public:         // Logging
                u32                             m_bufferLogIndex[LOG_SD+LOG_USB]                =       {0};          // for the new model where we use the char* m_bufferLog[LOG_SD+LOG_USB] 

                char                            m_logKernel[LOG_KERNEL_SIZE]                    =       {0};          //  pre-init buffer 
                u32                             m_logKernelIndex                                = 0;

                u32                             m_logScreenStartIndex                           = 0;            // logScreenUpdate()
// local copies of my graphics related structs
                olg_state                       m_ogl                                           =       {};           
                vtx_state                       m_vtx                                           =       {};
                glsl_state                      m_vsh                                           =       {};
                glsl_state                      m_fsh                                           =       {};
                glsl_state                      m_osh                                           =       {};
                tex_state                       m_tex                                           =       {};
                tex_state                       m_omt                                           =       {};
                h264_state                      m_vid                                           =       {};    

                int                             m_activeTex                                     = 0;
                int                             m_activeVideo                                   = 0;
                int                             m_activeFrame                                   = 0;  
                int                             g_activeProgram                                 = 0;
                int                             g_activeProgramTemp                             = 0;
// missing globals / shared state / dummies for now
                bool                            m_SD_has_load                                   = false;
                bool                            m_USB_has_load                                  = false;

                int                             g_currentProgramBuffer                          = 0;

                bool                            g_activeProgramFlag                             = false;

                int                             g_gl_program_current                            = 0;        // storeModes() - exposed 
                int                             g_gl_program_last                               = 0;        // storeModes() - local temp 

                int                             g_activeBpmChannel                              = 0;   // <- is telling the lfo what bpm is source!

                GLfloat                         GLtime                                          = 0;
                GLfloat                         g_opaque                                        = 0.5; 

        //      unsigned                        g_frameStart;                                   // !! has become g_frameStart !!

                bool                            m_audio_mode_activated                          = true; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                bool                            is_audio[2]                                     =           { 0 };

                unsigned                        g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT]        =           { 0 };           // the integer in/out matrix
                float                           g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT]        =           { 0.0f };           // the float in/out matrix
                bool                            g_menuPickUpFlag[MODETABLE_COUNT]               =           { 0 };                     // the flags for the pickup mechanism  
                unsigned                        g_buttons_states[BUTTON_COUNT][BTN_INDEX_COUNT] =           { 0 };      // the button state machine
                unsigned                        g_centralModeBuffer[SLOTS][MODETABLE_COUNT]     =           { 0 };         // the general user settings, storable per program 
                unsigned                        g_centralModeBufferTemp[SLOTS][MODETABLE_COUNT] =           { 0 };
                unsigned                        g_lfoMultiplier[LFO_MULTIPLIERS_COUNT]          =           { 64, 32, 16, 8, 4, 2, 1 };

                unsigned long                   g_lfoBpmMatrix[4][LFO_BPM_COUNT]                =           { 0 }; // was unsigned !

// datamanagement.cpp
                unsigned                        g_hFile;

                char                            m_83FileName[MAX_FILE_NAME_LENGTH];
// util
        const   int                             m_scaleFactors[3]                               =           { 1023, 1551, 2047 };      // 5.0V max (1023 * 1) / 3.3V max (1023 * 1.515555...) / 2.5V max (1023 * 2)

                int                             m_adc_ring[ADC_CHANNELS][ADC_BUFFER_COUNT]      =           { 0 };
                int                             m_adc_index = 0;

                int                             g_audioIreg[4]                                  =           { 0 };
                int                             m_band[4][AUDIO_BUFFER_COUNT]                   =           { 0 };
                int                             m_sum[4]                                        =           { 0 };  

                uint8_t                         m_idx0;
                uint8_t                         m_idx1;
                uint8_t                         m_idx2;
                uint8_t                         m_idx3;

                uint32_t                        m_audio_hold_A;
                uint32_t                        m_audio_hold_B;

                char** 				m_bufferVid                                     = nullptr;      // thats the pointer to my "array-like" buffer allocation
                char* 				m_videoBlockBase                                = nullptr;      // returns the aligned DMA base pointer
                char* 				m_videoRawBlock                                 = nullptr;      // returns the original pointer from new[]
                size_t 				m_videoBlockSize                                = 0;            // size of each individual buffer - complete size, not only blocks?

                char**				m_bufferFrA                                     = nullptr;      // i created a struct for it but that means i must 
                char* 			        m_frameBlockBaseA                               = nullptr;      // rewrite the wrappers and initialize the stucts properly
                char* 				m_frameRawBlockA                                = nullptr;      // and that is actually not really progress
                size_t 				m_frameBlockSizeA                               = 0;

                char**				m_bufferFrB                                     = nullptr;
                char* 				m_frameBlockBaseB                               = nullptr;
                char* 				m_frameRawBlockB                                = nullptr;
                size_t 				m_frameBlockSizeB                               = 0;	

                char** 				m_bufferOmt                                     = nullptr;
                char* 				m_overlyBlockBase                               = nullptr;
                char* 				m_overlayRawBlock                               = nullptr;
                size_t 				m_overlyBlockSize                               = 0;

                char** 				m_bufferTex                                     = nullptr;
                char* 				m_textureBlockBase                              = nullptr;
                char* 				m_textureRawBlock                               = nullptr;
                size_t 				m_textureBlockSize                              = 0;

                char**				m_bufferKnl                                     = nullptr;

                char**				m_bufferLog                                     = nullptr;

                char** 				m_bufferVsh                                     = nullptr;
                char** 				m_bufferOmf                                     = nullptr;
                char** 				m_bufferFsh                                     = nullptr; 

                char** 				m_bufferLfo                                     = nullptr;


// the populated filecounter array - source and truth and hub for init and load                                MAXSD   MAXUSB    EXTCNT     SCANNED   LOADED  PREV    V_CNT    SIZE  
                unsigned                        filecounter[FT_COUNT][FLD_COUNT]                =       {   { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
                                                                                                            { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
                                                                                                            { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
                                                                                                            { LFO_SD, LFO_USB,        0,    0,        0,      0,      0,       LFO_SIZ },  // wavetables
                                                                                                            { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
                                                                                                            { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
                                                                                                            { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
                                                                                                            { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
                                                                                                            { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
                                                                                                            { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }}; // LOG logging buffers
// lists of extensions possible in my scanroot directory function per filetype 
        const   char*                           g_SufVsh[VSH_EXT]			        =           { "vsh" };    // vertex shaders
        const   char*                           g_SufOmf[OMF_EXT]			        =           { "omf" };	// is a fsh file but used for the overlay atlas
        const   char*                           g_SufFsh[FSH_EXT]			        =           { "fsh" };    // fragment shaders 
        const   char*                           g_SufOmt[OMT_EXT]			        =           { "omt" };    // is a bpm file but used for the overlay atlas
        const   char*                           g_SufTex[TEX_EXT]			        =           { "bmp" };    // for textures 24bit rgb
        const   char*                           g_SufVid[VID_EXT]			        =           { "264" };    // video in raw h264 annex b encoded 
        const   char*                           g_SufKln[KLN_EXT]			        =           { "img" };    // kernel.img for the update mechanism
// array to store the scanned filenames
                char*                           g_ScnVsh[VSH_SD + VSH_USB]     	                =           { 0 };    
        	char*				g_ScnOmf[OMF_SD + OMF_USB] 		        =           { 0 };
                char*                           g_ScnFsh[FSH_SD + FSH_USB]     	                =           { 0 };
        	char*				g_ScnOmt[OMT_SD + OMT_USB] 		        =           { 0 };
                char*                           g_ScnTex[TEX_SD + TEX_USB]     	                =           { 0 };
                char*                           g_ScnVid[VID_SD + VID_USB]     	                =           { 0 };
                char*                           g_ScnKln[KLN_SD + KLN_USB]     	                =           { 0 };
// array to store the length of the loaded files
                unsigned                        g_bytVsh[VSH_SD + VSH_USB]                      =           { 0 };
                unsigned                        g_bytOmf[OMF_SD + OMF_USB]                      =           { 0 };
                unsigned                        g_bytFsh[FSH_SD + FSH_USB]                      =           { 0 };
                unsigned                        g_bytOmt[OMT_SD + OMT_USB]                      =           { 0 };
                unsigned                        g_bytTex[TEX_SD + TEX_USB]                      =           { 0 };
                unsigned                        g_bytVid[VID_SD + VID_USB]                      =           { 0 };
                unsigned                        g_bytKln[KLN_SD + KLN_USB]                      =           { 0 };
// CODE_MENU.CPP
                int                             g_menuLayer                                     = 0;
                int                             g_lastLayer                                     = 0;
                int                             g_lastLayerLED                                  = 0;
                unsigned                        g_extClockTime[8]                               =           { 0 }; // number of my adc channels!
#ifdef USE_MIDI
                bool                            g_midiConnected                                 = false;
                bool                            g_midiHeld[128]                                 = { 0 };
                unsigned                        g_midiNote                                      = 36;
                unsigned                        g_midiCC0Int                                    = 0;            // gets into my io matrix, right?!
                unsigned                        g_midiCC1Int                                    = 0;
                float                           g_midiCC0Flt                                    = 0.0f;
                float                           g_midiCC1Flt                                    = 0.0f;

        const   uint16_t                    modeMaskByValue[MODE_NAME_COUNT]                    =       {   0b00000000001,     // mode 0
                                                                                                            0b00000000010,     // mode 1
                                                                                                            0b00000000100,     // mode 2
                                                                                                            0b00000001000,     // mode 3
                                                                                                            0b00000010000,     // mode 4
                                                                                                            0b00000100000,     // mode 5
                                                                                                            0b00001000000,     // mode 6
                                                                                                            0b00010000000,     // mode 7
                                                                                                            0b00100000000,     // mode 8
                                                                                                            0b01000000000,     // mode 9
                                                                                                            0b10000000000 };   // mode 10

        const   uint16_t                    layerModeMap[BLOCK_COUNT]                           =       {   0b11111111111,     // layer 0: dummy row
                                                                                                            0b11111111111,     // layer 1: every mode
                                                                                                            0b11111111111,     // layer 2: every mode

                                                                                                            0b00000001100,     // layer 3: modes 2 or 3
                                                                                                            0b00000000010,     // layer 4: mode 1
                                                                                                            0b00011110000,     // layer 5: modes 4, 5, 6 or 7

                                                                                                            0b11111111111,     // layer 6: every mode
                                                                                                            0b11111111111,     // layer 7: every mode
                                                                                                            0b11100000000,     // layer 8: modes 8, 9 or 10 - midi 
                                                                                                            0b11111111111 };   // filler to get BLOCK_COUNT

        const   int                         g_mapType[BLOCK_COUNT][4]                           =       {   { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // mode channel 0-3 
                                                                                                            { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // mode channel 4-7

                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // 
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // 
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // 
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },

                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // new for midi
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },                                                    
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE } };

        const   int                         g_valueRoof[BLOCK_COUNT][4]                         =       {   {    4,    4,    4,    4  },                        // channel 0-3 four modes ( before roof mapping )
                                                                                                            {    4,    4,    4,    4  },                        // channel 4-7 four modes ( before roof mapping )

                                                                                                            {    5,    5,    7,    7  },                        // LF0 wave, LF0 mult, LF1 wave, LF1 mult
                                                                                                            {  511,  511,    8,    3  },                        // HIGH = THRESHOLD_L + 1 + THRESHOLD_H was 1024 / 1024 and in the original code 128 and 320 ( middle-ground )                                    // threshold, effect, attenuation, none/dummy

                                                                                                            {   64,   64,   64,   64  },                        // sensitivity Aud0_L, Aud0_H, Aud1_L, Aud1_H

                                                                                                            {    8,    8,    8,    8  },                        // means i need the max +1
                                                                                                            {    2,    2,    2,    2  },
                                                                                                            {   16,  128,  128,    4  }, // new for midi
                                                                                                            {    0,    0,    0,    0  },
                                                                                                            {    0,    0,    0,    0  } };

        const   int                         g_groupLen[GROUP_COUNT]                             =           { 4, 2, 2, 3 };
        const   int                         g_groupModes[GROUP_COUNT][4]                        =       {   { 0, 1, 2, 3 },
                                                                                                            { 4, 5, 0, 0 },
                                                                                                            { 6, 7, 0, 0 }, 
                                                                                                            { 8, 9,10, 0 } };           // new for midi
#else 
        const   uint8_t                     modeMaskByValue[8]                                  =       {   0b00000001,     // mode 0
                                                                                                            0b00000010,     // mode 1
                                                                                                            0b00000100,     // mode 2
                                                                                                            0b00001000,     // mode 3
                                                                                                            0b00010000,     // mode 4
                                                                                                            0b00100000,     // mode 5
                                                                                                            0b01000000,     // mode 6
                                                                                                            0b10000000 };   // mode 7 

        const   uint8_t                     layerModeMap[BLOCK_COUNT]                           =       {   0b11111111,     // layer 0: dummy row
                                                                                                            0b11111111,     // layer 1: every mode
                                                                                                            0b11111111,     // layer 2: every mode

                                                                                                            0b00001100,     // layer 3: modes 2 or 3
                                                                                                            0b00000010,     // layer 4: mode 1
                                                                                                            0b11110000,     // layer 5: modes 4, 5, 6 or 7

                                                                                                            0b11111111,     // layer 6: every mode
                                                                                                            0b11111111,     // layer 7: every mode
                                                                                                            0b11111111 };   // filler to get BLOCK_COUNT

        const   int                         g_mapType[BLOCK_COUNT][4]                           =       {   { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // mode channel 0-3 
                                                                                                            { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // mode channel 4-7

                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // 
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // 
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // 
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },

                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },                                                    
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE } };

        const   int                         g_valueRoof[BLOCK_COUNT][4]                         =       {   {    4,    4,    4,    4  },                                      // channel 0-3 four modes ( before roof mapping )
                                                                                                            {    4,    4,    4,    4  },                                      // channel 4-7 four modes ( before roof mapping )

                                                                                                            {    5,    5,    7,    7  },                                      // LF0 wave, LF0 mult, LF1 wave, LF1 mult
                                                                                                            {  511,  511,    8,    3  },                                      // threshold, effect, attenuation, none/dummy - HIGH = THRESHOLD_L + 1 + THRESHOLD_H was 1024 / 1024 and in the original code 128 and 320 ( middle-ground )

                                                                                                            {   64,   64,   64,   64  },                                      // sensitivity Aud0_L, Aud0_H, Aud1_L, Aud1_H

                                                                                                            {    8,    8,    8,    8  },                         // means i need the max +1
                                                                                                            {    2,    2,    2,    2  },
                                                                                                            {    0,    0,    0,    0  },
                                                                                                            {    0,    0,    0,    0  } };
        const   int                         g_groupLen[GROUP_COUNT]                             =           { 4, 2, 2 };
        const   int                         g_groupModes[GROUP_COUNT][4]                        =       {   { 0, 1, 2, 3 },
                                                                                                            { 4, 5, 0, 0 },
                                                                                                            { 6, 7, 0, 0 } };                                                                                                        
#endif
                int                         g_modeRoof[MODETABLE_COUNT]                         =           { 0 };
                int                         g_modeMap[MODETABLE_COUNT][MODE_NAME_COUNT]         =           { 0 };


                int                         g_blockColor[BLOCK_COUNT][3]                        =       {   {190,  60,  50},   // block 0 - warm red
                                                                                                            { 55, 155,  95},   // block 1 - jade green
                                                                                                            { 60, 105, 180},   // block 2 - medium blue
                                                                                                            {185, 105,  40},   // block 3 - burnt orange
                                                                                                            { 45, 140, 160},   // block 4 - blue teal
                                                                                                            { 95,  90, 170},   // block 5 - indigo violet
                                                                                                            {  0,   0,   0},   // block 6 - invisible
                                                                                                            {150, 115,  45} }; // block 7 - muted gold

                int                         g_modeColor[MODE_NAME_COUNT][3]                     =       {   { 40, 180, 180},   // IN_MODE_ADC       - cyan
                                                                                                            {210,  35,  35},   // IN_MODE_TRG       - red
                                                                                                            { 55, 190,  55},   // IN_MODE_LF1       - green
                                                                                                            { 45,  75, 210},   // IN_MODE_LF2       - blue

                                                                                                            {220, 125,  25},   // MODE_AU_AL        - orange
                                                                                                            {210,  45, 155},   // MODE_AU_AH        - magenta
                                                                                                            { 30, 150, 105},   // MODE_AU_BL        - jade
                                                                                                            {135,  55, 205}    // MODE_AU_BH        - violet
#ifdef USE_MIDI
                                                                                                           ,{220, 200,  25},   // IN_MODE_MIDI_NOTE - yellow
                                                                                                            { 45, 125, 215},   // IN_MODE_MIDI_CC0  - azure
                                                                                                            {205,  75,  35}    // IN_MODE_MIDI_CC1  - vermilion
#endif
                                                                                                        };


private:
                VCHI_INSTANCE_T                 m_VCHIInstance                                  = 0;
                VCHI_CONNECTION_T*              m_Connection                                    = 0;
                VCOS_EVENT_T                    m_EventSMEM                                     =           {};
                VCOS_EVENT_T                    m_EventMMAL                                     =           {};
                VCHI_SERVICE_HANDLE_T           m_ServiceHandleVCSM                             = 0;
                VCHI_SERVICE_HANDLE_T           m_ServiceHandleMMAL                             = 0;
                u32                             m_TransactionId                                 = 0;
                // returned from vcsm        
                u32                             m_input_buffer_handle                           = 0;        // comes from VCSM
                u32                             m_input_buffer_pointer                          = 0;        // comes from VCSM
                u32                             m_InputBufferSize                               = 0;    // MMAL from alloc aka m_videoBlockSize
                u32                             m_output_buffer_handle_a                        = 0;        // comes from VCSM
                u32                             m_output_buffer_pointer_a                       = 0;        // comes from VCSM
                u32                             m_OutputBufferSizeA                             = 0;    // MMAL ask for this but means  m_frameBlockSizeA
                u32                             m_output_buffer_handle_b                        = 0;        // comes from VCSM
                u32                             m_output_buffer_pointer_b                       = 0;        // comes from VCSM
                u32                             m_OutputBufferSizeB                             = 0;    // MMAL ask for this but means m_frameBlockSizeB          
                u32                             m_ComponentHandle                               = 0;    // used in mmal_init either direct ( inside the functions ) or rather by reference ( & ) 
                u32                             m_InputPortHandle                               = 0;    // mmal needs it!
                u32                             m_OutputPortHandle                              = 0;    // mmal needs it!

        const   char*                           m_debug_table[16]                               =       {   "MMAL_MSG_STATUS_SUCCESS", 							    // Success //
                                                                                                            "MMAL_MSG_STATUS_ENOMEM",      							// Out of memory //
                                                                                                            "MMAL_MSG_STATUS_ENOSPC",      							// Out of resources other than memory //
                                                                                                            "MMAL_MSG_STATUS_EINVAL",      							// Argument is invalid //
                                                                                                            "MMAL_MSG_STATUS_ENOSYS",      							// Function not implemented //
                                                                                                            "MMAL_MSG_STATUS_ENOENT",      							// No such file or directory //
                                                                                                            "MMAL_MSG_STATUS_ENXIO",       							// No such device or address //
                                                                                                            "MMAL_MSG_STATUS_EIO",         							// I/O error //
                                                                                                            "MMAL_MSG_STATUS_ESPIPE",      							// Illegal seek //
                                                                                                            "MMAL_MSG_STATUS_ECORRUPT",    							// Data is corrupt \attention //
                                                                                                            "MMAL_MSG_STATUS_ENOTREADY",   							// Component is not ready //
                                                                                                            "MMAL_MSG_STATUS_ECONFIG",     							// Component is not configured //
                                                                                                            "MMAL_MSG_STATUS_EISCONN",     							// Port is already connected //
                                                                                                            "MMAL_MSG_STATUS_ENOTCONN",    							// Port is disconnected //
                                                                                                            "MMAL_MSG_STATUS_EAGAIN",      							// Resource temporarily unavailable. //
                                                                                                            "MMAL_MSG_STATUS_EFAULT" };   							// Bad address //
// VCSM predefined messages as public member
public:
                SERVICE_CREATION_T*             m_ServiceCreateVCSM                             = nullptr;
                VCSM_Import_MEM_Msg*            m_importTxVCSM_A                                = nullptr;
                VCSM_Import_MEM_Reply*          m_importRxVCSM_A                                = nullptr;
                VCSM_Import_MEM_Msg*            m_importTxVCSM_B                                = nullptr;
                VCSM_Import_MEM_Reply*          m_importRxVCSM_B                                = nullptr;
                VCSM_Import_MEM_Msg*            m_importTxVCSM_C                                = nullptr;
                VCSM_Import_MEM_Reply*          m_importRxVCSM_C                                = nullptr;
                VCSM_Lock_MEM_Msg*              m_lockTxVCSM                                    = nullptr;
                VCSM_Lock_MEM_Reply*            m_lockRxVCSM                                    = nullptr;
                VCSM_Free_MEM_Msg*              m_freeTxVCSM                                    = nullptr;
                VCSM_Free_MEM_Reply*            m_freeRxVCSM                                    = nullptr;
// MMAL predefined messages as public member
                SERVICE_CREATION_T*             m_ServiceCreateMMAL                             = nullptr;
                MMAL_Component_Create_Msg*      m_ComponentCreateTx                             = nullptr;
                MMAL_Component_Create_Reply*    m_ComponentCreateRx                             = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Input_A                         = nullptr;
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Input_A                         = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Output_A                        = nullptr; 
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Output_A                        = nullptr;
                MMAL_Port_Info_Set_Msg*         m_PortInfoSetTx_Input                           = nullptr;
                MMAL_Port_Info_Set_Msg*         m_PortInfoSetTx_Output                          = nullptr;
                MMAL_Port_Info_Set_Reply*       m_PortInfoSetRx_Input                           = nullptr;
                MMAL_Port_Info_Set_Reply*       m_PortInfoSetRx_Output                          = nullptr;
                MMAL_Component_Enable_Msg*      m_ComponentEnableTx                             = nullptr;
                MMAL_Component_Enable_Reply*    m_ComponentEnableRx                             = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Input_B                         = nullptr;
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Input_B                         = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Output_B                        = nullptr;
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Output_B                        = nullptr;
                MMAL_Port_Parameter_Set_Msg*    m_PortParamTx_Input                             = nullptr;
                MMAL_Port_Parameter_Set_Reply*  m_PortParamRx_Input                             = nullptr;
                MMAL_Port_Parameter_Set_Msg*    m_PortParamTx_Output                            = nullptr;
                MMAL_Port_Parameter_Set_Reply*  m_PortParamRx_Output                            = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Input_C                         = nullptr;
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Input_C                         = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Output_C                        = nullptr;
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Output_C                        = nullptr;
                MMAL_Port_Action_Msg*           m_PortActionTx_Input                            = nullptr;
                MMAL_Port_Action_Reply_Msg*     m_PortActionRx_Input                            = nullptr;
                MMAL_Port_Action_Msg*           m_PortActionTx_Output                           = nullptr;
                MMAL_Port_Action_Reply_Msg*     m_PortActionRx_Output                           = nullptr;
                MMAL_Buffer_From_Host_Msg*      m_BufferFromHostTx_Input                        = nullptr;
                MMAL_Buffer_From_Host_Msg*      m_BufferFromHostRx_Input                        = nullptr;
                MMAL_Buffer_From_Host_Msg*      m_BufferFromHostTx_Output                       = nullptr;
                MMAL_Buffer_From_Host_Msg*      m_BufferFromHostRx_Output                       = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Input_D                         = nullptr;
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Input_D                         = nullptr;
                MMAL_Port_Info_Get_Msg*         m_PortInfoGetTx_Output_D                        = nullptr;
                MMAL_Port_Info_Get_Reply*       m_PortInfoGetRx_Output_D                        = nullptr;

                bool                            f_firstFrameQueued                              = false;

// placeholder until i solved this!

                EGLDisplay                      m_eglDisplay;      // is stored in the olg_state struct -> display     and needed by bufferReadyMMAL
                EGLContext                      m_eglContext;      // is stored in the olg_state struct -> context     and needed by bufferReadyMMAL
                EGLImageKHR                     m_EGLimage;        // is stored in the tex_state struct -> m_EGLimage  and needed by bufferReadyMMAL
                GLuint                          m_Texture;         // is stored in the tex_state struct -> gl_tex_vid  and needed by bufferReadyMMAL               


// lets try the fps break here:



u32   g_frameStart       = 0;
u32   g_frameEnd         = 0;
u32   g_frameCurrent     = 0;
u32   g_frameTarget      = 0;
u32   g_frameDelay       = 0;
u32   g_frameTime        = 0;
u32   g_lastSwapDuration = 0;
#ifdef __DEBUG_TIMING__ 
u32   g_runtimeDuration  = 0;
u32   g_glDuration       = 0;
#endif
float g_currentFPS       = 0.0f;

bool  g_limitFPS         = true;

    