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
                bool                            m_bootLogsSaved                                 = false;
                
                int                             g_currentProgramBuffer                          = 0;

                bool                            g_activeProgramFlag                             = false;

                int                             g_gl_program_current                            = 0;        // storeModes() - exposed 
                int                             g_gl_program_last                               = 0;        // storeModes() - local temp 

                int                             g_activeBpmChannel                              = 0;   // <- is telling the lfo what bpm is source!

                GLfloat                         GLtime                                          = 0;
                GLfloat                         g_opaque                                        = 0.5; 

                int                            is_audio[2]                                      =           { 0 };

                unsigned                        g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT]        =           { 0 };           // the integer in/out matrix
                float                           g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT]        =           { 0.0f };           // the float in/out matrix
                bool                            g_menuPickUpFlag[MODETABLE_COUNT]               =           { 0 };                     // the flags for the pickup mechanism  
                unsigned                        g_buttons_states[BTN_COUNT][BTN_INDEX_COUNT]    =           { 0 };      // the button state machine
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

                uint32_t                        m_audio_hold_A                                  = 0;
                uint32_t                        m_audio_hold_B                                  = 0;
                   
                uint32_t                        m_BPM_hold_A                                    = 0;       

                uint8_t                         m_idx0                                          = 0;
                uint8_t                         m_idx1                                          = 0;
                uint8_t                         m_idx2                                          = 0;
                uint8_t                         m_idx3                                          = 0;                                                         

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

                char**                          m_bufferLfo                                     = nullptr;
// the populated filecounter array - source and truth and hub for init and load                                MAXSD   MAXUSB    EXTCNT     SCANNED   LOADED  PREV    V_CNT    SIZE  
                unsigned                        filecounter[FT_COUNT][FLD_COUNT]                =       {   { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
                                                                                                            { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
                                                                                                            { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader

                                                                                                            { LFO_SD, LFO_USB,        0,    0,        0,      0,      0,       LFO_SIZ },

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

                bool                            g_midiConnected                                 = false;
                bool                            g_midiHeld[128]                                 = { 0 };
                unsigned                        g_midiNote                                      = 36;
                unsigned                        g_midiCC0Int                                    = 0;            // gets into my io matrix, right?!
                unsigned                        g_midiCC1Int                                    = 0;
                float                           g_midiCC0Flt                                    = 0.0f;
                float                           g_midiCC1Flt                                    = 0.0f;

       typedef void                            (CKernel::*ModeFunc)(int);

                ModeFunc                        g_modeTable[12] =
{
                &CKernel::modeADC,
                &CKernel::modeTRG,
                &CKernel::modeBPM,

                &CKernel::modeLF0,
                &CKernel::modeLF1,

                &CKernel::modeAudioAbL,
                &CKernel::modeAudioAbH,
                &CKernel::modeAudioBbL,
                &CKernel::modeAudioBbH,

                &CKernel::modeMidiNote,
                &CKernel::modeMidiCC0,
                &CKernel::modeMidiCC1
};

        const   uint16_t                    modeMaskByValue[IN_MODE_NAME_COUNT]                    =    {   0b0000000000000001,     // mode 0
                                                                                                            0b0000000000000010,     // mode 1
                                                                                                            0b0000000000000100,     // mode 2
                                                                                                            0b0000000000001000,     // mode 3

                                                                                                            0b0000000000010000,     // mode 4
                                                                                                            0b0000000000100000,     // mode 5
                                                                                                            0b0000000001000000,     // mode 6
                                                                                                            0b0000000010000000,     // mode 7

                                                                                                            0b0000000100000000,     // mode 8
                                                                                                            0b0000001000000000,     // mode 9
                                                                                                            0b0000010000000000,
                                                                                                            0b0000100000000000  };  // mode 10

        const   uint16_t                    layerModeMap[BLOCK_COUNT]                           =       {   0b0000011111111111,     // layer 0: dummy row
                                                                                                            0b0000011111111111,     // layer 1: every mode
                                                                                                            0b0000011111111111,     // layer 2: every mode

                                                                                                            0b0000000000011000,     // layer 3: modes 3 or 4 - lfo
                                                                                                            0b0000000000000010,     // layer 4: mode 1
                                                                                                            0b0000000111100000,     // layer 5: modes 5, 6, 7 or 8 - audio
                                                                                                            0b0000111000000000,     // layer 6: modes 9, 10 or 11 - midi
                                                                                                            0b0000011111111111,     // layer 7: every mode
                                                                                                            0b0000011111111111,     // layer 8: every mode

                                                                                                            0b0000011111111111 };   // filler to get BLOCK_COUNT

        const   int                         g_mapType[BLOCK_COUNT][4]                           =       {   { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  },     // layer  1 for mode channel 0-3 
                                                                                                            { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  },     // layer  2 for mode channel 4-7

                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },     // layer  3 for IN_MODE_LF_X 
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },     // layer  4 for IN_MODE_TRG
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },     // layer  5 for IN_MODE_AU_X
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },     // layer  6 for MIDI

                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },     // layer  7 target mode selector
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },     // layer  8 system settings

                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },     // layer  9 hidden layer ( runtime parameters / flags )
                                                                                                            { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE } };   // layer 10 hidden layer ( runtime parameters / flags )

        const   int                         g_valueRoof[BLOCK_COUNT][4]                         =       { //{    4,    4,    4,    4  },                        // layer  1 for mode channel 0-3
                                                                                                          //{    4,    4,    4,    4  },                        // layer  2 for mode channel 4-7
                                                                                                        
                                                                                                            {    5,    5,    5,    5  },
                                                                                                            {    5,    5,    5,    5  },

                                                                                                            {    waveTableCount,    waveTableCount,    7,    7  },                        // layer  3 for IN_MODE_LF_X            ( wave 0, wave 1, mult 0, mult 1 )
                                                                                                            {  511,  511,    8,    3  },                        // layer  4 for IN_MODE_TRG             ( thr_low, thr_hi, ext_selector, attenuation? )

                                                                                                            {   64,   64,   64,   64  },                        // layer  5 for IN_MODE_AU_X            ( aud 0 low, aud o hi, aud 1 low, aud 1 hi )
                                                                                                            {   16,  128,  128,    4  },                        // layer  6 for MIDI                    ( midi channel, cc 0, cc 1, note range )
                                                                                                            {    8,    8,    8,    8  },                        // layer  7 for target mode selector    ( time, texture, video, frame )
                                                                                                            {    2,    2,    2,    2  },                        // layer  8 for system settings         ( store set, load set, store logs,  load firmware )

                                                                                                            {    0,    0,    0,    0  },                        // layer  9 hidden layer ( runtime parameters / flags )
                                                                                                            {    0,    0,    0,    0  } };                      // layer 10 hidden layer ( runtime parameters / flags )

        const   int                         g_groupLen[GROUP_COUNT]                             =           {   5,     2,    2,    3 }; //      {   4,     2,    2,    3 };
/*
        const   int                         g_groupModes[GROUP_COUNT][4]                        =       {   {   0,     1,    2,    3 },
                                                                                                            {   4,     5,    0,    0 },
                                                                                                            {   6,     7,    0,    0 }, 
                                                                                                            {   8,     9,   10,    0 } };           // new for midi
*/
        const   int                         g_groupModes[GROUP_COUNT][5]                        =       {   {   0,     1,    2,    3,    4 },
                                                                                                            {   5,     6,    0,    0,    0 },
                                                                                                            {   7,     8,    0,    0,    0 }, 
                                                                                                            {   9,    10,   11,    0,    0 } };           // new for midi

                int                         g_modeRoof[MODETABLE_COUNT]                         =           { 0 };
                int                         g_modeMap[MODETABLE_COUNT][IN_MODE_NAME_COUNT]      =           { 0 };


                int                         g_blockColor[BLOCK_COUNT][3]                        =       {   {190,  60,  50},   // block 0               - warm red
                                                                                                            { 55, 155,  95},   // block 1               - jade green
                                                                                                            { 60, 105, 180},   // block 2               - medium blue
                                                                                                            {185, 105,  40},   // block 3               - burnt orange
                                                                                                            { 45, 140, 160},   // block 4               - blue teal
                                                                                                            { 95,  90, 170},   // block 5               - indigo violet
                                                                                                            {  0,   0,   0},   // block 6               - invisible
                                                                                                            {150, 115,  45} }; // block 7               - muted gold

                int                         g_modeColor[IN_MODE_NAME_COUNT][3]                  =       {   { 40, 180, 180},   // IN_MODE_ADC           - cyan
                                                                                                            {210,  35,  35},   // IN_MODE_TRG           - red
                                                                                                            { 45, 135, 135},   // IN_MODE_BMP           - ???
                                                                                                            { 55, 190,  55},   // IN_MODE_LF_0          - green
                                                                                                            { 45,  75, 210},   // IN_MODE_LF_1          - blue

                                                                                                            {220, 125,  25},   // IN_MODE_AU_AL         - orange
                                                                                                            {210,  45, 155},   // IN_MODE_AU_AH         - magenta
                                                                                                            { 30, 150, 105},   // IN_MODE_AU_BL         - jade
                                                                                                            {135,  55, 205},   // IN_MODE_AU_BH         - violet

                                                                                                            {220, 200,  25},   // IN_MODE_MIDI_NOTE     - yellow
                                                                                                            { 45, 125, 215},   // IN_MODE_MIDI_CC0      - azure
                                                                                                            {205,  75,  35}    // IN_MODE_MIDI_CC1      - vermilion
                                                                                                        };

const GLfloat g_atlasTileMap[ATLAS_TILE_COUNT][2] = {
    { 0.000f, 0.875f },  //  0 ATLAS_TILE_MODE_ADC
    { 0.125f, 0.875f },  //  1 ATLAS_TILE_MODE_TRG
    { 0.250f, 0.875f },  //  2 ATLAS_TILE_MODE_BMP
    { 0.375f, 0.875f },  //  3 ATLAS_TILE_MODE_LF0
    { 0.500f, 0.875f },  //  4 ATLAS_TILE_MODE_LF1
    { 0.625f, 0.875f },  //  5 ATLAS_TILE_MODE_AU_AL
    { 0.750f, 0.875f },  //  6 ATLAS_TILE_MODE_AU_AH
    { 0.875f, 0.875f },  //  7 ATLAS_TILE_MODE_AU_BL

    { 0.000f, 0.750f },  //  8 ATLAS_TILE_MODE_AU_BH
    { 0.125f, 0.750f },  //  9 ATLAS_TILE_MODE_MIDI_NOTE
    { 0.250f, 0.750f },  // 10 ATLAS_TILE_MODE_MIDI_CC0
    { 0.375f, 0.750f },  // 11 ATLAS_TILE_MODE_MIDI_CC1
    { 0.500f, 0.750f },  // 12 ATLAS_TILE_SYSTEM_STORE
    { 0.625f, 0.750f },  // 13 ATLAS_TILE_SYSTEM_LOAD
    { 0.750f, 0.750f },  // 14 ATLAS_TILE_SYSTEM_UPDATE
    { 0.875f, 0.750f },  // 15 ATLAS_TILE_SYSTEM_LOG

    { 0.000f, 0.625f },  // 16 ATLAS_TILE_LFO_WAVE_SINE
    { 0.125f, 0.625f },  // 17 ATLAS_TILE_LFO_WAVE_TRIANGLE
    { 0.250f, 0.625f },  // 18 ATLAS_TILE_LFO_WAVE_RAMP_UP
    { 0.375f, 0.625f },  // 19 ATLAS_TILE_LFO_WAVE_RAMP_DOWN
    { 0.500f, 0.625f },  // 20 ATLAS_TILE_LFO_WAVE_SMOOTH_UP
    { 0.625f, 0.625f },  // 21 ATLAS_TILE_LFO_WAVE_SMOOTH_DOWN
    { 0.750f, 0.625f },  // 22 ATLAS_TILE_LFO_WAVE_EXPONENTIAL
    { 0.875f, 0.625f },  // 23 ATLAS_TILE_LFO_WAVE_RANDOM

    { 0.000f, 0.500f },  // 24 ATLAS_TILE_DIVIDER_1_64
    { 0.125f, 0.500f },  // 25 ATLAS_TILE_DIVIDER_1_32
    { 0.250f, 0.500f },  // 26 ATLAS_TILE_DIVIDER_1_16
    { 0.375f, 0.500f },  // 27 ATLAS_TILE_DIVIDER_1_8
    { 0.500f, 0.500f },  // 28 ATLAS_TILE_DIVIDER_1_4
    { 0.625f, 0.500f },  // 29 ATLAS_TILE_DIVIDER_1_2
    { 0.750f, 0.500f },  // 30 ATLAS_TILE_DIVIDER_1_1
    { 0.875f, 0.500f },  // 31 ATLAS_TILE_EXTERN_SELECTOR

    { 0.000f, 0.375f },  // 32 ATLAS_TILE_NUMBER_0
    { 0.125f, 0.375f },  // 33 ATLAS_TILE_NUMBER_1
    { 0.250f, 0.375f },  // 34 ATLAS_TILE_NUMBER_2
    { 0.375f, 0.375f },  // 35 ATLAS_TILE_NUMBER_3
    { 0.500f, 0.375f },  // 36 ATLAS_TILE_NUMBER_4
    { 0.625f, 0.375f },  // 37 ATLAS_TILE_NUMBER_5
    { 0.750f, 0.375f },  // 38 ATLAS_TILE_NUMBER_6
    { 0.875f, 0.375f },  // 39 ATLAS_TILE_NUMBER_7

    { 0.000f, 0.250f },  // 40 ATLAS_TILE_NUMBER_8
    { 0.125f, 0.250f },  // 41 ATLAS_TILE_NUMBER_9
    { 0.250f, 0.250f },  // 42 ATLAS_TILE_NUMBER_DOT
    { 0.375f, 0.250f },  // 43 ATLAS_TILE_TARGET_TIME
    { 0.500f, 0.250f },  // 44 ATLAS_TILE_TARGET_TEXTURE
    { 0.625f, 0.250f },  // 45 ATLAS_TILE_TARGET_VIDEO
    { 0.750f, 0.250f },  // 46 ATLAS_TILE_TARGET_FRAME
    { 0.875f, 0.250f },  // 47 ATLAS_TILE_TARGET_GL_PROGRAM

    { 0.000f, 0.125f },  // 48 ATLAS_TILE_INDICATOR_BAR_L
    { 0.125f, 0.125f },  // 49 ATLAS_TILE_INDICATOR_BAR_R
    { 0.250f, 0.125f },  // 50 ATLAS_TILE_INDICATOR_BAR_BPM
    { 0.375f, 0.125f },  // 51 ATLAS_TILE_SYSTEM_IDLE
    { 0.500f, 0.125f },  // 52 ATLAS_TILE_BRACKET_TOP
    { 0.625f, 0.125f },  // 53 ATLAS_TILE_BRACKET_BOTTOM
    { 0.750f, 0.125f },  // 54 ATLAS_TILE_LABEL_FPS
    { 0.875f, 0.125f },  // 55 ATLAS_TILE_LABEL_BPM

    { 0.000f, 0.000f },  // 56 ATLAS_TILE_BLANK
    { 0.125f, 0.000f },  // 57 ATLAS_TILE_LABEL_ADC
    { 0.250f, 0.000f },  // 58 ATLAS_TILE_LABEL_TRG
    { 0.375f, 0.000f },  // 59 ATLAS_TILE_LABEL_LFO
    { 0.500f, 0.000f },  // 60 ATLAS_TILE_LABEL_AUD
    { 0.625f, 0.000f },  // 61 ATLAS_TILE_LABEL_MID
    { 0.750f, 0.000f },  // 62 ATLAS_TILE_LABEL_SYS
    { 0.875f, 0.000f }   // 63 ATLAS_TILE_BRACKET_BRIDGE
};
                                                                                        
/*
        const   int                         g_menuCoordinates[MENU_COORD_COUNT][2]              =       {   { -128, -128 },             //  0 MENU_COORD_MODE_04			*  100% 128px

                                                                                                            { -128, -128 },             //  1 MENU_COORD_DETAIL_0			**
                                                                                                            { -128, -128 },             //  2 MENU_COORD_DETAIL_1 			**

                                                                                                            {    0, -128 },             //  3 MENU_COORD_MODE_15			*

                                                                                                            {    0, -128 },             //  4 MENU_COORD_DETAIL_2 			**
                                                                                                            {    0, -128 },             //  5 MENU_COORD_DETAIL_3 			**

                                                                                                            { -128,    0 },             //  6 MENU_COORD_MODE_26			*

                                                                                                            { -128,    0 },             //  7 MENU_COORD_DETAIL_4 			**
                                                                                                            { -128,    0 },             //  8 MENU_COORD_DETAIL_5 			**

                                                                                                            {    0,    0 },             //  9 MENU_COORD_MODE_37			*

                                                                                                            {    0,    0 },             // 10 MENU_COORD_DETAIL_6 			**
                                                                                                            {    0,    0 },             // 11 MENU_COORD_DETAIL_7 			**

                                                                                                            {    0,    0 },             // 12 MENU_COORD_TARGET_TIME 		***
                                                                                                            {    0,    0 },             // 13 MENU_COORD_TARGET_TEXTURE		***
                                                                                                            {    0,    0 },             // 14 MENU_COORD_TARGET_VIDEO		***
                                                                                                            {    0,    0 },             // 15 MENU_COORD_TARGET_FRAME		***

                                                                                                            {    0,    0 },             // 16 MENU_COORD_TARGET_PROGRAM		Y

                                                                                                            {    0,    0 },             // 17 MENU_COORD_EXTERN_SELECTOR	X


                                                                                                            { - 64, -256 },             // 18 MENU_COORD_ARROW_UP
                                                                                                            { - 64,  128 },             // 19 MENU_COORD_ARROW_DOWN

                                                                                                            { -128, 160 },              // 20 MENU_COORD_BPM_STRING	****

                                                                                                            {    0, 160 },	        // 21 MENU_COORD_BPM_100		****
                                                                                                            {   20, 160 },	        // 22 MENU_COORD_BPM_010		****
                                                                                                            {   40, 160 },	        // 23 MENU_COORD_BPM_001		****

                                                                                                            {   60, 160 },	        // 24 MENU_COORD_BPM_DOT		****

                                                                                                            {    80, 160 },	        // 25 MENU_COORD_BPM_10D		****
                                                                                                            {   100, 160 }  };	        // 26 MENU_COORD_BPM_01D		****
*/                                
/*                               
const int g_menuCoordinates[MENU_COORD_COUNT][2] =                                                      {   { -64, -64 }, // line 409:  0 MENU_COORD_MODE_04               50% 64px                                                     
                                                                                                            { -64, -64 }, // line 411:  1 MENU_COORD_DETAIL_0
                                                                                                            { -64, -64 }, // line 412:  2 MENU_COORD_DETAIL_1

                                                                                                            {   0, -64 }, // line 414:  3 MENU_COORD_MODE_15
                                                                                                            {   0, -64 }, // line 416:  4 MENU_COORD_DETAIL_2
                                                                                                            {   0, -64 }, // line 417:  5 MENU_COORD_DETAIL_3

                                                                                                            { -64,   0 }, // line 419:  6 MENU_COORD_MODE_26
                                                                                                            { -64,   0 }, // line 421:  7 MENU_COORD_DETAIL_4
                                                                                                            { -64,   0 }, // line 422:  8 MENU_COORD_DETAIL_5

                                                                                                            {   0,   0 }, // line 424:  9 MENU_COORD_MODE_37
                                                                                                            {   0,   0 }, // line 426: 10 MENU_COORD_DETAIL_6
                                                                                                            {   0,   0 }, // line 427: 11 MENU_COORD_DETAIL_7

                                                                                                            {   0,   0 }, // line 429: 12 MENU_COORD_TARGET_TIME
                                                                                                            {   0,   0 }, // line 430: 13 MENU_COORD_TARGET_TEXTURE
                                                                                                            {   0,   0 }, // line 431: 14 MENU_COORD_TARGET_VIDEO
                                                                                                            {   0,   0 }, // line 432: 15 MENU_COORD_TARGET_FRAME
                                                                                                            {   0,   0 }, // line 434: 16 MENU_COORD_TARGET_PROGRAM
                                                                                                            {   0,   0 }, // line 436: 17 MENU_COORD_EXTERN_SELECTOR

                                                                                                            { -32, -128 }, // line 439: 18 MENU_COORD_ARROW_UP
                                                                                                            { -32,   64 }, // line 440: 19 MENU_COORD_ARROW_DOWN

                                                                                                            { -64, 80 }, // line 442: 20 MENU_COORD_BPM_STRING
                                                                                                            {   0, 80 }, // line 444: 21 MENU_COORD_BPM_100
                                                                                                            {  10, 80 }, // line 445: 22 MENU_COORD_BPM_010
                                                                                                            {  20, 80 }, // line 446: 23 MENU_COORD_BPM_001
                                                                                                            {  30, 80 }, // line 448: 24 MENU_COORD_BPM_DOT
                                                                                                            {  40, 80 }, // line 450: 25 MENU_COORD_BPM_10D
                                                                                                            {  50, 80 }};  // line 451: 26 MENU_COORD_BPM_01D
*/                                                        
/*                                  
        const   int                         g_menuCoordinates[MENU_COORD_COUNT][2]              =       {   {  -96,  -96 },             //  0 MENU_COORD_MODE_04			* 75% 96px

                                                                                                            {  -96,  -96 },             //  1 MENU_COORD_DETAIL_0			**
                                                                                                            {  -96,  -96 },             //  2 MENU_COORD_DETAIL_1 			**

                                                                                                            {    0,  -96 },             //  3 MENU_COORD_MODE_15			*

                                                                                                            {    0,  -96 },             //  4 MENU_COORD_DETAIL_2 			**
                                                                                                            {    0,  -96 },             //  5 MENU_COORD_DETAIL_3 			**

                                                                                                            {  -96,    0 },             //  6 MENU_COORD_MODE_26			*

                                                                                                            {  -96,    0 },             //  7 MENU_COORD_DETAIL_4 			**
                                                                                                            {  -96,    0 },             //  8 MENU_COORD_DETAIL_5 			**

                                                                                                            {    0,    0 },             //  9 MENU_COORD_MODE_37			*

                                                                                                            {    0,    0 },             // 10 MENU_COORD_DETAIL_6 			**
                                                                                                            {    0,    0 },             // 11 MENU_COORD_DETAIL_7 			**

                                                                                                            {    0,    0 },             // 12 MENU_COORD_TARGET_TIME 		***
                                                                                                            {    0,    0 },             // 13 MENU_COORD_TARGET_TEXTURE		***
                                                                                                            {    0,    0 },             // 14 MENU_COORD_TARGET_VIDEO		***
                                                                                                            {    0,    0 },             // 15 MENU_COORD_TARGET_FRAME		***

                                                                                                            {    0,    0 },             // 16 MENU_COORD_TARGET_PROGRAM		Y

                                                                                                            {    0,    0 },             // 17 MENU_COORD_EXTERN_SELECTOR	X


                                                                                                            { - 48, -192 },             // 18 MENU_COORD_ARROW_UP
                                                                                                            { - 48,   96 },             // 19 MENU_COORD_ARROW_DOWN

                                                                                                            {  -96, 120 },              // 20 MENU_COORD_BPM_STRING	****

                                                                                                            {    0, 120 },	        // 21 MENU_COORD_BPM_100		****
                                                                                                            {   15, 120 },	        // 22 MENU_COORD_BPM_010		****
                                                                                                            {   30, 120 },	        // 23 MENU_COORD_BPM_001		****

                                                                                                            {   45, 120 },	        // 24 MENU_COORD_BPM_DOT		****

                                                                                                            {    60, 120 },	        // 25 MENU_COORD_BPM_10D		****
                                                                                                            {    75, 120 }  };	        // 26 MENU_COORD_BPM_01D		****                                                                                                            
*/
/*
        const   int                         g_menuCoordinates[MENU_COORD_COUNT][2]              =       {
                                                                                                            {  -96, -192 },             //  0 MENU_COORD_MODE_0
                                                                                                            {  -96, -192 },             //  1 MENU_COORD_DETAIL_0
                                                                                                            {  -96, -192 },             //  2 MENU_COORD_DETAIL_1

                                                                                                            {    0, -192 },             //  3 MENU_COORD_MODE_1
                                                                                                            {    0, -192 },             //  4 MENU_COORD_DETAIL_2
                                                                                                            {    0, -192 },             //  5 MENU_COORD_DETAIL_3

                                                                                                            {  -96,  -96 },             //  6 MENU_COORD_MODE_2
                                                                                                            {  -96,  -96 },             //  7 MENU_COORD_DETAIL_4
                                                                                                            {  -96,  -96 },             //  8 MENU_COORD_DETAIL_5

                                                                                                            {    0,  -96 },             //  9 MENU_COORD_MODE_3
                                                                                                            {    0,  -96 },             // 10 MENU_COORD_DETAIL_6
                                                                                                            {    0,  -96 },             // 11 MENU_COORD_DETAIL_7

                                                                                                            {  -96,    0 },             // 12 MENU_COORD_MODE_4
                                                                                                            {  -96,    0 },             // 13 MENU_COORD_DETAIL_8
                                                                                                            {  -96,    0 },             // 14 MENU_COORD_DETAIL_9

                                                                                                            {    0,    0 },             // 15 MENU_COORD_MODE_5
                                                                                                            {    0,    0 },             // 16 MENU_COORD_DETAIL_10
                                                                                                            {    0,    0 },             // 17 MENU_COORD_DETAIL_11

                                                                                                            {  -96,   96 },             // 18 MENU_COORD_MODE_6
                                                                                                            {  -96,   96 },             // 19 MENU_COORD_DETAIL_12
                                                                                                            {  -96,   96 },             // 20 MENU_COORD_DETAIL_13

                                                                                                            {    0,   96 },             // 21 MENU_COORD_MODE_7
                                                                                                            {    0,   96 },             // 22 MENU_COORD_DETAIL_14
                                                                                                            {    0,   96 },             // 23 MENU_COORD_DETAIL_15

                                                                                                            {    0,    0 },             // 24 MENU_COORD_TARGET_TIME
                                                                                                            {    0,    0 },             // 25 MENU_COORD_TARGET_TEXTURE
                                                                                                            {    0,    0 },             // 26 MENU_COORD_TARGET_VIDEO
                                                                                                            {    0,    0 },             // 27 MENU_COORD_TARGET_FRAME

                                                                                                            {    0,    0 },             // 28 MENU_COORD_TARGET_PROGRAM

                                                                                                            {    0,    0 },             // 29 MENU_COORD_EXTERN_SELECTOR

                                                                                                            {  -48, -288 },             // 30 MENU_COORD_ARROW_UP
                                                                                                            {  -48,  192 },             // 31 MENU_COORD_ARROW_DOWN

                                                                                                            {  -96,  216 },             // 32 MENU_COORD_BPM_STRING

                                                                                                            {    0,  216 },             // 33 MENU_COORD_BPM_100
                                                                                                            {   15,  216 },             // 34 MENU_COORD_BPM_010
                                                                                                            {   30,  216 },             // 35 MENU_COORD_BPM_001

                                                                                                            {   45,  216 },             // 36 MENU_COORD_BPM_DOT

                                                                                                            {   60,  216 },             // 37 MENU_COORD_BPM_10D
                                                                                                            {   75,  216 }  };          // 38 MENU_COORD_BPM_01D
*/
const int g_menuCoordinates[MENU_COORD_COUNT][2] = {
    {  -96, -192 },  //  0 MENU_COORD_MODE_0
    {  -96, -192 },  //  1 MENU_COORD_DETAIL_0
    {  -96, -192 },  //  2 MENU_COORD_DETAIL_1

    {    0, -192 },  //  3 MENU_COORD_MODE_1
    {    0, -192 },  //  4 MENU_COORD_DETAIL_2
    {    0, -192 },  //  5 MENU_COORD_DETAIL_3

    {  -96,  -96 },  //  6 MENU_COORD_MODE_2
    {  -96,  -96 },  //  7 MENU_COORD_DETAIL_4
    {  -96,  -96 },  //  8 MENU_COORD_DETAIL_5

    {    0,  -96 },  //  9 MENU_COORD_MODE_3
    {    0,  -96 },  // 10 MENU_COORD_DETAIL_6
    {    0,  -96 },  // 11 MENU_COORD_DETAIL_7

    {  -96,    0 },  // 12 MENU_COORD_MODE_4
    {  -96,    0 },  // 13 MENU_COORD_DETAIL_8
    {  -96,    0 },  // 14 MENU_COORD_DETAIL_9

    {    0,    0 },  // 15 MENU_COORD_MODE_5
    {    0,    0 },  // 16 MENU_COORD_DETAIL_10
    {    0,    0 },  // 17 MENU_COORD_DETAIL_11

    {  -96,   96 },  // 18 MENU_COORD_MODE_6
    {  -96,   96 },  // 19 MENU_COORD_DETAIL_12
    {  -96,   96 },  // 20 MENU_COORD_DETAIL_13

    {    0,   96 },  // 21 MENU_COORD_MODE_7
    {    0,   96 },  // 22 MENU_COORD_DETAIL_14
    {    0,   96 },  // 23 MENU_COORD_DETAIL_15

    {    0,    0 },  // 24 MENU_COORD_TARGET_TIME
    {    0,    0 },  // 25 MENU_COORD_TARGET_TEXTURE
    {    0,    0 },  // 26 MENU_COORD_TARGET_VIDEO
    {    0,    0 },  // 27 MENU_COORD_TARGET_FRAME

    {    0,    0 },  // 28 MENU_COORD_TARGET_PROGRAM

    {    0,    0 },  // 29 MENU_COORD_EXTERN_SELECTOR

    {  -96,  216 },  // 30 MENU_COORD_BPM_STRING

    {    0,  216 },  // 31 MENU_COORD_BPM_100
    {   15,  216 },  // 32 MENU_COORD_BPM_010
    {   30,  216 },  // 33 MENU_COORD_BPM_001

    {   45,  216 },  // 34 MENU_COORD_BPM_DOT

    {   60,  216 },  // 35 MENU_COORD_BPM_10D
    {   75,  216 },  // 36 MENU_COORD_BPM_01D

    {   96, -192 },  // 37 MENU_COORD_BRACKET_ROW_0
    {   96,  -96 },  // 38 MENU_COORD_BRACKET_ROW_1
    {   96,    0 },  // 39 MENU_COORD_BRACKET_ROW_2
    {   96,   96 }   // 40 MENU_COORD_BRACKET_ROW_3
};

// * 	= 	the coordinates for the four quadrants ( q0 for mode 0 in layer 1, mode 4 in layer 2, q1 for mode 1 in layer 1, mode 5 in layer 2, etc. )
// **	= 	is usually the same as the coordinates for the quadrant for the mode pair of 0/4, 1/5, 2/6. 3/7 ( depending on the layer 1 or 2)
// ***	= 	is shown depending on block 7/ layer 8 either < 4 ( layer 1 ) or > 3 ( layer 2 )
// **** = 	the "bmp" xyz.xy numeric display, depending on m_BPM_hold_A ( will be shown after changes in bpm ) 
// Y	=	is shown if layer 2 on the fixed position q3 !
// X	= 	same logic as **** taken from g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT]

		GLfloat                         g_overlayVertices[OVERLAY_FLOAT_COUNT]           =           { 0.0f };
		unsigned                        g_overlayVertexCount                            = 0;

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

