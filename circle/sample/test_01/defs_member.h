private:
                CActLED                         m_ActLED;                   // circle system requirements
                CKernelOptions                  m_Options;
                CMachineInfo                    m_MachineInfo;
                CDeviceNameService	            m_DeviceNameService;
            //  CScreenDevice		            m_Screen; // from the old
	        //  CSerialDevice		            m_Serial;
                CExceptionHandler               m_ExceptionHandler;
                CInterruptSystem                m_Interrupt;
                CTimer                          m_Timer;
                CLogger                         m_Logger;  
                CMemorySystem		            m_Memory;                
                CEMMCDevice		                m_EMMC;
                CUSBHCIDevice		            m_USBHCI;
                CVCHIQDevice		            m_VCHIQ;

                CFATFileSystem                 *m_pFileSystem; 

                


public:
                CBcmFrameBuffer                 gE_FrameBuffer;             // framebuffer
                CCharGenerator                  gE_CharGenerator;

                u32*                            gE_PixelBuffer          = nullptr;
                unsigned                        gE_PitchBytes           = 0;
                unsigned                        gE_ScreenWidth          = 0;
                unsigned                        gE_ScreenHeight         = 0;
                unsigned                        gE_CharWidth            = 0;
                unsigned                        gE_CharHeight           = 0;
                unsigned                        gE_Cols                 = 0;
                unsigned                        gE_Rows                 = 0;

private:
                uintptr                         m_SPIBaseAddress        = 0;        // SPI
                bool                            m_SPIValid              = 0;
// SMI / DMA / WS2812
private:
                CDMAChannel                     m_SMITxDMA;

                unsigned                        m_SMIGpioPin            = 0;
                unsigned                        m_SMISDMask             = 0;
                unsigned                        m_LEDCount              = 0;
                unsigned                        m_BufferLength          = 0;
                TXDATA_T*                       m_pBuffer               = 0;
                bool                            m_SMIValid              = FALSE;
// debug
public:
                char                            m_logBuffer[LOG_SIZ]    = {0};
                u32                             m_logBufferIndex        = 0;



                olg_state                       m_ogl    = {};              // local copies of my graphics related structs

                vtx_state                       m_vtx    = {};

                glsl_state                      m_vsh    = {};
                glsl_state                      m_fsh    = {};
                glsl_state                      m_osh    = {};

                tex_state                       m_tex    = {};
                tex_state                       m_omt    = {};

                h264_state                      m_vid    = {};    

// missing globals / shared state / dummies for now

                bool                            m_resetFlag             = false;

    volatile    bool	                        m_bStorageAttached      = false;

                bool                            m_SD_has_load           = false;
                bool                            m_USB_has_load          = false;

                int                             g_currentProgramBuffer;
                int                             g_current_gl_program;
                int                             g_last_gl_program;
                int                             g_activeBpmChannel;

                
                GLint                           GLtime = 0;
                GLfloat                         g_opaque = 0.5; 

                int                             g_menu_mode_new;    // dummy - i assume this checks the layer of my menu!!
                int                             g_menu_mode_old;

                unsigned long                   g_double_click_time = 50;
                unsigned long                   g_long_click_time   = 250;
                unsigned                        g_currentTime;

                int                             attenuation = 0;
                bool                            m_audio_mode_activated  = true;

                bool                            is_audio[2];

                unsigned                        g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];           // the integer in/out matrix
                float                           g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];           // the float in/out matrix
                bool                            g_menuPickUpFlag[4*MENU_LAYER_COUNT];                     // the flags for the pickup mechanism  
                unsigned                        g_buttons_states[BUTTON_COUNT][BTN_INDEX_COUNT];      // the button state machine
                unsigned                        g_centralModeBuffer[SLOTS][MODETABLE_COUNT];         // the general user settings, storable per program 
                unsigned                        g_lfoMultiplier[LFO_MULTIPLIERS_COUNT]        = {   64, 32, 16, 8, 4, 2, 1 };

                long long                       g_lfoBpmMatrix[4][LFO_BPM_COUNT]; // was unsigned !
// datamanagement.cpp
                unsigned                        g_hFile;

                char                            m_83FileName[MAX_FILE_NAME_LENGTH];
// util
        const   int                             m_scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                                        1551,       // 3.3V max (1023 * 1.515555...)
                                                                        1023    };  // 5.0V max     

                int                             m_adc_ring[ADC_CHANNELS][ADC_BUFFER_COUNT];
                int                             m_adc_index = 0;

                float                           m_band[4][AUDIO_BUFFER_COUNT];

                float                           m_sum[4];  

                uint8_t                         m_idx0;
                uint8_t                         m_idx1;
                uint8_t                         m_idx2;
                uint8_t                         m_idx3;

                uint32_t                        m_audio_hold_A;
                uint32_t                        m_audio_hold_B;

                bool                            m_audio_flag_A                      = false;          
                bool                            m_audio_flag_B                      = false;


                                                            
                char** 				            m_bufferVid                         = nullptr;      // thats the pointer to my "array-like" buffer allocation
                char* 				            m_videoBlockBase                    = nullptr;      // returns the aligned DMA base pointer
                char* 				            m_videoRawBlock                     = nullptr;      // returns the original pointer from new[]
                size_t 				            m_videoBlockSize                    = 0;            // size of each individual buffer - complete size, not only blocks?

                char**				            m_bufferFrA                         = nullptr;      // i created a struct for it but that means i must 
                char* 				            m_frameBlockBaseA                   = nullptr;      // rewrite the wrappers and initialize the stucts properly
                char* 				            m_frameRawBlockA                    = nullptr;      // and that is actually not really progress
                size_t 				            m_frameBlockSizeA                   = 0;

                char**				            m_bufferFrB                         = nullptr;
                char* 				            m_frameBlockBaseB                   = nullptr;
                char* 				            m_frameRawBlockB                    = nullptr;
                size_t 				            m_frameBlockSizeB                   = 0;	

                char** 				            m_bufferOmt                         = nullptr;
                char* 				            m_overlyBlockBase                   = nullptr;
                char* 				            m_overlayRawBlock                   = nullptr;
                size_t 				            m_overlyBlockSize                   = 0;

                char** 				            m_bufferTex                         = nullptr;
                char* 				            m_textureBlockBase                  = nullptr;
                char* 				            m_textureRawBlock                   = nullptr;
                size_t 				            m_textureBlockSize                  = 0;

                char**				            m_bufferKnl                         = nullptr;
                char**				            m_bufferLog                         = nullptr;
                char** 				            m_bufferVsh                         = nullptr;
                char** 				            m_bufferOmf                         = nullptr;                
                char** 				            m_bufferFsh                         = nullptr; 
                
//----------------------------------------------------------------------------------------------------------------------------------------------------
// the populated filecounter array - source and truth and hub for init and load
//----------------------------------------------------------------------------------------------------------------------------------------------------
                                                                                    // MAXSD   MAXUSB    EXTCNT     SCANNED   LOADED  PREV    V_CNT    SIZE  
                unsigned                        filecounter[FT_COUNT][FLD_COUNT]    =       {   { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
                                                                                                { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
                                                                                                { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
                                                                                                { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
                                                                                                { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
                                                                                                { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
                                                                                                { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
                                                                                                { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
                                                                                                { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }}; // LOG logging buffers   

//----------------------------------------------------------------------------------------------------------------------------------------------------
// lists of extensions possible in my scanroot directory function per filetype 
//----------------------------------------------------------------------------------------------------------------------------------------------------
        const   char*                           g_SufVsh[VSH_EXT]			        =           { "vsh" };    // vertex shaders
        const   char*                           g_SufOmf[OMF_EXT]			        =           { "omf" };	// is a fsh file but used for the overlay atlas
        const   char*                           g_SufFsh[FSH_EXT]			        =           { "fsh" };    // fragment shaders 
        const   char*                           g_SufOmt[OMT_EXT]			        =           { "omt" };    // is a bpm file but used for the overlay atlas
        const   char*                           g_SufTex[TEX_EXT]			        =           { "bmp" };    // for textures 24bit rgb
        const   char*                           g_SufVid[VID_EXT]			        =           { "264" };    // video in raw h264 annex b encoded 
        const   char*                           g_SufKln[KLN_EXT]			        =           { "img" };    // kernel.img for the update mechanism
// array to store the scanned filenames
                char*                           g_ScnVsh[VSH_SD + VSH_USB]     	    =           { 0 };    
        		char*				            g_ScnOmf[OMF_SD + OMF_USB] 		    =           { 0 };
                char*                           g_ScnFsh[FSH_SD + FSH_USB]     	    =           { 0 };
        		char*				            g_ScnOmt[OMT_SD + OMT_USB] 		    =           { 0 };
                char*                           g_ScnTex[TEX_SD + TEX_USB]     	    =           { 0 };
                char*                           g_ScnVid[VID_SD + VID_USB]     	    =           { 0 };
                char*                           g_ScnKln[KLN_SD + KLN_USB]     	    =           { 0 };
// array to store the length of the loaded files
                unsigned                        g_bytVsh[VSH_SD + VSH_USB]          =           { 0 };
                unsigned                        g_bytOmf[OMF_SD + OMF_USB]          =           { 0 };
                unsigned                        g_bytFsh[FSH_SD + FSH_USB]          =           { 0 };
                unsigned                        g_bytOmt[OMT_SD + OMT_USB]          =           { 0 };
                unsigned                        g_bytTex[TEX_SD + TEX_USB]          =           { 0 };
                unsigned                        g_bytVid[VID_SD + VID_USB]          =           { 0 };
                unsigned                        g_bytKln[KLN_SD + KLN_USB]          =           { 0 };
// CODE_MENU.CPP
// Use a **member function pointer table** instead of `switch`.
// That lets you add modes by only extending the table.
typedef void (CKernel::*ModeFunc)(int);         // for the new menu selector -> easier to expand, right?

                ModeFunc                        g_modeTable[12]                     =       {   &CKernel::modeADC,
                                                                                                &CKernel::modeTRG,
                                                                                                &CKernel::modeBPM,
                                                                                                &CKernel::modeLF1,
                                                                                                &CKernel::modeLF2,
                                                                                                nullptr,
                                                                                                nullptr,
                                                                                                nullptr,
                                                                                                &CKernel::modeAudioAb0,
                                                                                                &CKernel::modeAudioAb1,
                                                                                                &CKernel::modeAudioBb0,
                                                                                                &CKernel::modeAudioBb1 };

                uint8_t                         g_modeLengthAdd[MODELEN_FLAG_COUNT] =       {   MAX_MODES,                  // 5 for now    
                                                                                                2,                          // MODELEN_AUDIO_A  // this enum than is used here to get the actual numbers for the  [i][0] in g_modeMap
                                                                                                2,                          // MODELEN_AUDIO_B
                                                                                                };
// the first element is the max of modes for each p_channel, than we have the order ( switch case of setChannelMode(int p_channel) )
// i just wonder if i need a dedicated function to edit the first element because other code might do it as read adc 
                                                                                        //  A    /  B    /  LFO  / Sens  / etc     
                uint8_t                         g_modeMap[LAYER*4][LAYER*4]         =       {   { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}, 	// layer a is adc in 0-3 
                                                                                                { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},

                                                                                                { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},    // layer b is adc in 4-7
                                                                                                { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},

                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},    // layer c is adc in 8-11
                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},

                                                                                                {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},   // layer d is adc in 12-15
                                                                                                {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},

                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},    // layer c is adc in 8-11
                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0},
                                                                                                { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0} };
//---------------------------------------------------------------------------------------------------------------------------------------------------- 
                SERVICE_CREATION_T*              m_ServiceCreateVCSM                = nullptr;

                VCSM_Import_MEM_Msg*             m_importTxVCSM_A                   = nullptr;
                VCSM_Import_MEM_Reply*           m_importRxVCSM_A                   = nullptr;

                VCSM_Import_MEM_Msg*             m_importTxVCSM_B                   = nullptr;
                VCSM_Import_MEM_Reply*           m_importRxVCSM_B                   = nullptr;

                VCSM_Import_MEM_Msg*             m_importTxVCSM_C                   = nullptr;
                VCSM_Import_MEM_Reply*           m_importRxVCSM_C                   = nullptr;

                VCSM_Lock_MEM_Msg*               m_lockTxVCSM                       = nullptr;
                VCSM_Lock_MEM_Reply*             m_lockRxVCSM                       = nullptr;

                VCSM_Free_MEM_Msg*               m_freeTxVCSM                       = nullptr;
                VCSM_Free_MEM_Reply*             m_freeRxVCSM                       = nullptr;

                // MMAL predefined messages as public member here group B — kernel.h (full pointer conversion)

                SERVICE_CREATION_T*              m_ServiceCreateMMAL                = nullptr;

                MMAL_Component_Create_Msg*       m_ComponentCreateTx                = nullptr;
                MMAL_Component_Create_Reply*     m_ComponentCreateRx                = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_A            = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_A            = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_A           = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_A           = nullptr;

                MMAL_Port_Info_Set_Msg*          m_PortInfoSetTx_Input              = nullptr;
                MMAL_Port_Info_Set_Msg*          m_PortInfoSetTx_Output             = nullptr;

                MMAL_Port_Info_Set_Reply*        m_PortInfoSetRx_Input              = nullptr;
                MMAL_Port_Info_Set_Reply*        m_PortInfoSetRx_Output             = nullptr;

                MMAL_Component_Enable_Msg*       m_ComponentEnableTx                = nullptr;
                MMAL_Component_Enable_Reply*     m_ComponentEnableRx                = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_B            = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_B            = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_B           = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_B           = nullptr;

                MMAL_Port_Parameter_Set_Msg*     m_PortParamTx_Input                = nullptr;
                MMAL_Port_Parameter_Set_Reply*   m_PortParamRx_Input                = nullptr;

                MMAL_Port_Parameter_Set_Msg*     m_PortParamTx_Output               = nullptr;
                MMAL_Port_Parameter_Set_Reply*   m_PortParamRx_Output               = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_C            = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_C            = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_C           = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_C           = nullptr;

                MMAL_Port_Action_Msg*            m_PortActionTx_Input               = nullptr;
                MMAL_Port_Action_Reply_Msg*      m_PortActionRx_Input               = nullptr;

                MMAL_Port_Action_Msg*            m_PortActionTx_Output              = nullptr;
                MMAL_Port_Action_Reply_Msg*      m_PortActionRx_Output              = nullptr;

                MMAL_Buffer_From_Host_Msg*       m_BufferFromHostTx_Input           = nullptr;
                MMAL_Buffer_From_Host_Msg*       m_BufferFromHostRx_Input           = nullptr;

                MMAL_Buffer_From_Host_Msg*       m_BufferFromHostTx_Output          = nullptr;
                MMAL_Buffer_From_Host_Msg*       m_BufferFromHostRx_Output          = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_D            = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_D            = nullptr;

                MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_D           = nullptr;
                MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_D           = nullptr;
