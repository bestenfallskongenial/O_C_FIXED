//----------------------------------------------------------------------------------------------------------------------------------------------------
                CActLED			        m_ActLED;                           // * CActLED: Switch the Act LED on and off, checks the Raspberry Pi model to use the right LED pin.
                                                                            // still not sure if i want to keep it
                CKernelOptions		        m_Options;                          // * CKernelOptions: Providing kernel options from file cmdline.txt (see doc/cmdline.txt).
                                                                            // mandatory of need for my stuff?
                CMachineInfo		        m_MachineInfo;                      // * CMachineInfo: Helper class to get different information about the running computer.
                                                                            // mandatory of need for my stuff?
                CDeviceNameService	        m_DeviceNameService;                // * CDeviceNameService: Devices can be registered by name and retrieved later by this name
                                                                            // mandatory - but for what?
                CScreenDevice		        m_Screen;                           // * CScreenDevice: Writing characters to screen, some escape sequences (some are not yet implemented)
                                                                            // tried to replace it -> logging.cpp
                CSerialDevice		        m_Serial;                           // * CSerialDevice: Driver for PL011 UART, interrupt or polling mode
                                                                            // who needs this?
                CExceptionHandler	        m_ExceptionHandler;                 // * CExceptionHandler: Generates a stack-trace and a panic message if an abort exception occurs.
                                                                            // why we need it?
                CInterruptSystem	        m_Interrupt;                        // * CInterruptSystem: Connecting to interrupts, an interrupt handler will be called on interrupt.
                                                                            // really? for time? and what else?
                CTimer			        m_Timer;                            // * CTimer: Manages the system clock, supports kernel timers and a calibrated delay loop.
                                                                            // what is controlled here? is there a different simpler way to delay?
                CLogger			        m_Logger;                           // * CLogger: Writing logging messages to a target device
                                                                            // i got my own, is this mandatory?
                CEMMCDevice		        m_EMMC;                             // *The SD card device driver CEMMCDevice does not use DMA and does not need cache-aligned DMA buffers. 
                                                                            // mandatory for the sd device
                CUSBHCIDevice		        m_USBHCI;                           // * CUSBHCIDevice: Alias for CDWHCIDevice or CXHCIDevice, depending on Raspberry Pi model.
                                                                            // mandatory for the usb device
                CVCHIQDevice		        m_VCHIQ;                            // mandatory for the vc04 device ( uses high level linux emulation, correct? )

                CMemorySystem		        m_Memory;                           // * CMemorySystem: Enabling MMU if requested, switching page tables (not used here).
                                                                            // mandatory for my mem allocation

                CFATFileSystem*                 m_pFileSystem;                      // * CFATFileSystem: File system driver for FAT16 and FAT32 storage partitions.
                                                                            // mandatory for file operation -> datamanagement.cpp

                CScheduler		        m_Scheduler;                        // * CScheduler: Cooperative non-preemtive scheduler which controls which task runs at a time.
                                                                            // really? who needs this? khronos stuff?

 //             CBcmWatchdog       	        m_Watchdog;                         // * CBcmWatchdog: Driver for the BCM2835 watchdog device.
 //                                                                             // Watchdog instance - tried to implement as class code!

                CSPIMaster		        m_SPIMaster;                        // * CSPIMaster: Driver for (non-AUX) SPI master device. Synchronous polling operation.
                                                                            // mandatory for the mpc control interfacing
                CMCP300X		        m_MCP300X;                          // *  Driver for MCP3004/3008 DAC with SPI interface
                                                                            // mandatory for the adc

                CWS2812OverSMI		        m_NeoPixels;                        // * Driver for multiple WS2812 controlled LED strips
                                                                            // mandatory for the ws2812 rgb led´s

                CGPIOManager		        m_GPIOManager;		                // * CGPIOManager: Interrupt multiplexer for CGPIOPin (only required if GPIO interrupt is used).
                                                                            // mandatory for the pin control BUT not needed in polling mode 
                                                                            // we included our own "write to gpiopin" but why does the constructor def uses "m_GPIOManager (&m_Interrupt),..." oh i see, the ws2812 led i assume

	        CGPIOPin 		        m_ChipSelectPin;  	                // * CGPIOPin: Encapsulates a GPIO pin, can be read, write or inverted. Supports interrupts. Simple initialization.
                                                                            // mandatory, but why again? 
                                                                            // Add this line for the chip select gpiopin - we try included our own "write to gpiopin"

// SPI

private:
                uintptr m_SPIBaseAddress = 0;
                boolean m_SPIValid = 0;

// framebuffer

public:
                CBcmFrameBuffer gE_FrameBuffer;
                CCharGenerator  gE_CharGenerator;
                CKernelOptions  m_Options;

                u32*            gE_PixelBuffer      = nullptr;
                unsigned        gE_PitchBytes       = 0;
                unsigned        gE_ScreenWidth      = 0;
                unsigned        gE_ScreenHeight     = 0;
                unsigned        gE_CharWidth        = 0;
                unsigned        gE_CharHeight       = 0;
                unsigned        gE_Cols             = 0;
                unsigned        gE_Rows             = 0;

// SMI / DMA / WS2812

private:
                CDMAChannel m_SMITxDMA;

                unsigned    m_SMIGpioPin   = 0;

                unsigned    m_SMISDMask    = 0;

                unsigned    m_LEDCount     = 0;
                unsigned    m_BufferLength = 0;

                TXDATA_T*   m_pBuffer      = 0;

                boolean     m_SMIValid     = FALSE;


// watchdog

static const unsigned WatchdogMaxTimeoutSeconds = 15;
//----------------------------------------------------------------------------------------------------------------------------------------------------
// local copies of my graphics related structs
//----------------------------------------------------------------------------------------------------------------------------------------------------
                olg_state               m_ogl    = {};

                vtx_state               m_vtx    = {};

                glsl_state              m_vsh    = {};
                glsl_state              m_fsh    = {};
                glsl_state              m_osh    = {};

                tex_state               m_tex    = {};
                tex_state               m_omt    = {};

                h264_state              m_vid    = {};    
//----------------------------------------------------------------------------------------------------------------------------------------------------
    volatile    boolean	                            m_bStorageAttached  = false;

                char** 				    m_bufferVid         = nullptr;      // thats the pointer to my "array-like" buffer allocation
                char* 				    m_videoBlockBase    = nullptr;      // returns the aligned DMA base pointer
                char* 				    m_videoRawBlock     = nullptr;      // returns the original pointer from new[]
                size_t 				    m_videoBlockSize    = 0;            // size of each individual buffer - complete size, not only blocks?

                char**				    m_bufferFrA         = nullptr;      // i created a struct for it but that means i must 
                char* 				    m_frameBlockBaseA   = nullptr;      // rewrite the wrappers and initialize the stucts properly
                char* 				    m_frameRawBlockA    = nullptr;      // and that is actually not really progress
                size_t 				    m_frameBlockSizeA   = 0;

                char**				    m_bufferFrB         = nullptr;
                char* 				    m_frameBlockBaseB   = nullptr;
                char* 				    m_frameRawBlockB    = nullptr;
                size_t 				    m_frameBlockSizeB   = 0;	

                char** 				    m_bufferOmt         = nullptr;
                char* 				    m_overlyBlockBase   = nullptr;
                char* 				    m_overlayRawBlock   = nullptr;
                size_t 				    m_overlyBlockSize   = 0;

                char** 				    m_bufferTex         = nullptr;
                char* 				    m_textureBlockBase  = nullptr;
                char* 				    m_textureRawBlock   = nullptr;
                size_t 				    m_textureBlockSize  = 0;

                char**				    m_bufferKnl         = nullptr;
                char**				    m_bufferLog         = nullptr;
                char** 				    m_bufferVsh         = nullptr;
                char** 				    m_bufferOmf         = nullptr;                
                char** 				    m_bufferFsh         = nullptr; 
//----------------------------------------------------------------------------------------------------------------------------------------------------
// the populated filecounter array - source and truth and hub for init and load
//----------------------------------------------------------------------------------------------------------------------------------------------------
                unsigned                filecounter[FT_COUNT][FLD_COUNT] =
{                                       // MAXSD   MAXUSB    EXTCNT      SCANNED   LOADED  PREV    V_CNT    SIZE  
                                        { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
                                        { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
                                        { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
                                        { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
                                        { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
                                        { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
                                        { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
                                        { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
                                        { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }   // LOG logging buffers   
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
// lists of extensions possible in my scanroot directory function per filetype 
//----------------------------------------------------------------------------------------------------------------------------------------------------
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" };    // vertex shaders
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };    // fragment shaders 
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" };    // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };    // for textures 24bit rgb
        const   char                   *g_SufVid[VID_EXT]			    = { "264" };    // video in raw h264 annex b encoded 
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };    // kernel.img for the update mechanism
// array to store the scanned filenames
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };    
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLN_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLN_SD + KLN_USB]      = { 0 };
//----------------------------------------------------------------------------------------------------------------------------------------------------
// okay, we need for each separate log buffer our own logIndex[MAX_LOG_LENGTH]
                uint32_t                logIndex_vc04[MAX_LOG_LENGTH];
// than we  #define MY_BUFFER buffer* and #define MY_INDEX logIndex* to assign a log buffer ti the functions that log / parse

        const   int                     m_scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                                1023    };  // 5.0V max     

                int                     m_adc_ring[ADC_CHANNELS][ADC_BUFFER];
                int                     m_adc_index;

                float                   m_band[4][MAX_AUDIO_BUFFER];

                float                   m_sum[4];  

                uint8_t                 m_idx0;
                uint8_t                 m_idx1;
                uint8_t                 m_idx2;
                uint8_t                 m_idx3;

                uint32_t                m_audio_hold_A;
                uint32_t                m_audio_hold_B;

                bool                    m_audio_flag_A = false;          
                bool                    m_audio_flag_B = false;
                  
                uint8_t                 g_modeLengthAdd[MODELEN_FLAG_COUNT] =
                                                                {
                                                                MAX_MODES,                  // 5 for now    
                                                                2,                          // MODELEN_AUDIO_A  // this enum than is used here to get the actual numbers for the  [i][0] in g_modeMap
                                                                2,                          // MODELEN_AUDIO_B
                                                                };
//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t g_modeMap[MENU_LAYERS*4][MENU_LAYERS*4] =	// the first element is the max of modes for each p_channel, than we have the order ( switch case of setChannelMode(int p_channel) )
                                                    // i just wonder if i need a dedicated function to edit the first element because other code might do it as read adc 
                                                    {  //  A    /  B    /  LFO  / Sens  / etc     
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0} 	// layer a is adc in 0-3 
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer b is adc in 4-7
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}   // layer d is adc in 12-15
                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    };
//----------------------------------------------------------------------------------------------------------------------------------------------------
// Use a **member function pointer table** instead of `switch`.
// That lets you add modes by only extending the table.
typedef void (CKernel::*ModeFunc)(int);         // for the new menu selector -> easier to expand, right?

ModeFunc g_modeTable[] =
                                                        {   
                                                        &CKernel::modeADC,
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
                                                        &CKernel::modeAudioBb1
                                                        };                                                    
//----------------------------------------------------------------------------------------------------------------------------------------------------
                unsigned                g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];           // the integer in/out matrix
                float                   g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];           // the float in/out matrix
                bool                    g_menuPickUpFlag[4*MENU_LAYER];                     // the flags for the pickup mechanism  
                unsigned                g_buttons_states[NR_BUTTONS][BTN_INDEX_COUNT];      // the button state machine
                unsigned                g_centralModeBuffer[SLOTS][modetablecount];         // the general user settings, storable per program 
                unsigned                g_lfoMultiplier[LFO_MULTIPLIERS];

                unsigned                g_lfoBpmMatrix[4][LFO_BPM_COUNT]        = {   64, 32, 16, 8, 4, 2, 1 };

                unsigned                g_hFile;                                            // file management!

                char                    m_83FileName[13];
//----------------------------------------------------------------------------------------------------------------------------------------------------
// VCSM and MMAL
//----------------------------------------------------------------------------------------------------------------------------------------------------
private:
        VCHI_INSTANCE_T                 m_VCHIInstance                  = 0;
        VCHI_CONNECTION_T*              m_Connection                    = 0;
        VCOS_EVENT_T                    m_EventSMEM                     = {};
        VCOS_EVENT_T                    m_EventMMAL                     = {};
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleVCSM             = 0;
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleMMAL             = 0;
        u32                             m_TransactionId                 = 0;
    //  u32                             m_vc_handle[MAX_BUFFER]         = {0};  // why an array, why not simply by u32& my_current_handle instead of slot?
    //  u32                             m_vc_pointer[MAX_BUFFER]        = {0};  // why an array, why not simply by u32& my_current_pointer instead of slot?
        u32                             m_input_buffer_handle           = 0;            // wait, thats all already below
        u32                             m_output_buffer_handle_a        = 0;
        u32                             m_output_buffer_handle_b        = 0;

        u32                             m_input_buffer_pointer          = 0;
        u32                             m_output_buffer_pointer_a       = 0;
        u32                             m_output_buffer_pointer_b       = 0;
// ---------------------------------------------------------------------------------------------------------------------------------------------------
public:
        GLuint                          m_Texture = 0;                                              // must be exposed
private:
        bool                            f_firstFrameQueued              = false;    
//---------------------------------------------------------------------------------------------------------------------------------------------------- 
// artifacts from the earlier mmal class code initializer - what do we really need here? -> check also the mmal_init code!!!

        u32                             m_InputBufferHandle;
        u32                             m_InputBufferPointer;

        u32                             m_InputBufferSize;              // aka m_videoBlockSize ?

        u32                             m_OutputBufferHandleA;
        u32                             m_OutputBufferPointerA;

        u32                             m_OutputBufferHandleB;
        u32                             m_OutputBufferPointerB;

        u32                             m_OutputBufferSize;             // aka m_frameBlockSizeA / m_frameBlockSizeB ?

        u32                             m_ResolutionX;                  // m_Options.GetWidth()
        u32                             m_ResolutionY;                  // m_Options.GetHeight()

        u32                             m_ComponentHandle;              // from createComponent() 

        u32                             m_VCSMHandleA;                  // obsolete - we now have a different mechanism in bufferReadyMMAL
        u32                             m_VCSMHandleB;                  // obsolete - we now have a different mechanism in bufferReadyMMAL

        u32                             m_InputPortHandle;              // getPortInfoMMAL(   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply& rx)
        u32                             m_OutputPortHandle;             // getPortInfoMMAL(   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply& rx)
        
        u32                             m_CurrentHandle;                // u32 m_CurrentHandle = m_BufferFromHostTx_Output.msg.buffer_header.data;
//----------------------------------------------------------------------------------------------------------------------------------------------------        
        // Core invariant (unchanged, explicit)
        // Snapshot **A** → build **SET**
        // Snapshot **B** → build **PARAM**
        // Snapshot **C** → build **ACTION**
        // Snapshot **D** → buffer runtime
        // Buffer TX/RX is independent of snapshots  
//----------------------------------------------------------------------------------------------------------------------------------------------------
// instead of having life time long structs for my vcsm / mmal i declare pointer instead and provide wrappers to alloc and free the structs after use!
// VCSM predefined messages as public member here group A — kernel.h (pointer conversion)

SERVICE_CREATION_T*              m_ServiceCreateVCSM             = nullptr;

VCSM_Import_MEM_Msg*             m_importTxVCSM_A                = nullptr;
VCSM_Import_MEM_Reply*           m_importRxVCSM_A                = nullptr;

VCSM_Import_MEM_Msg*             m_importTxVCSM_B                = nullptr;
VCSM_Import_MEM_Reply*           m_importRxVCSM_B                = nullptr;

VCSM_Import_MEM_Msg*             m_importTxVCSM_C                = nullptr;
VCSM_Import_MEM_Reply*           m_importRxVCSM_C                = nullptr;

VCSM_Lock_MEM_Msg*               m_lockTxVCSM                    = nullptr;
VCSM_Lock_MEM_Reply*             m_lockRxVCSM                    = nullptr;

VCSM_Free_MEM_Msg*               m_freeTxVCSM                    = nullptr;
VCSM_Free_MEM_Reply*             m_freeRxVCSM                    = nullptr;

// MMAL predefined messages as public member here group B — kernel.h (full pointer conversion)

SERVICE_CREATION_T*              m_ServiceCreateMMAL             = nullptr;

MMAL_Component_Create_Msg*       m_ComponentCreateTx             = nullptr;
MMAL_Component_Create_Reply*     m_ComponentCreateRx             = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_A         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_A         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_A        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_A        = nullptr;

MMAL_Port_Info_Set_Msg*          m_PortInfoSetTx_Input           = nullptr;
MMAL_Port_Info_Set_Msg*          m_PortInfoSetTx_Output          = nullptr;

MMAL_Port_Info_Set_Reply*        m_PortInfoSetRx_Input           = nullptr;
MMAL_Port_Info_Set_Reply*        m_PortInfoSetRx_Output          = nullptr;

MMAL_Component_Enable_Msg*       m_ComponentEnableTx             = nullptr;
MMAL_Component_Enable_Reply*     m_ComponentEnableRx             = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_B         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_B         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_B        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_B        = nullptr;

MMAL_Port_Parameter_Set_Msg*     m_PortParamTx_Input             = nullptr;
MMAL_Port_Parameter_Set_Reply*   m_PortParamRx_Input             = nullptr;

MMAL_Port_Parameter_Set_Msg*     m_PortParamTx_Output            = nullptr;
MMAL_Port_Parameter_Set_Reply*   m_PortParamRx_Output            = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_C         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_C         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_C        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_C        = nullptr;

MMAL_Port_Action_Msg*            m_PortActionTx_Input            = nullptr;
MMAL_Port_Action_Reply_Msg*      m_PortActionRx_Input            = nullptr;

MMAL_Port_Action_Msg*            m_PortActionTx_Output           = nullptr;
MMAL_Port_Action_Reply_Msg*      m_PortActionRx_Output           = nullptr;

MMAL_Buffer_From_Host_Msg*       m_BufferFromHostTx_Input        = nullptr;
MMAL_Buffer_From_Host_Msg*       m_BufferFromHostRx_Input        = nullptr;

MMAL_Buffer_From_Host_Msg*       m_BufferFromHostTx_Output       = nullptr;
MMAL_Buffer_From_Host_Msg*       m_BufferFromHostRx_Output       = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_D         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_D         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_D        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_D        = nullptr;
//----------------------------------------------------------------------------------------------------------------------------------------------------
unsigned long           g_waveTable                               [WAVEFORMS][WAVESAMPLES]    = 
{
                        {   
                        0,    0,    1,    1,    2,    4,    6,    8,   10,   12,   15,   19,   22,   26,   30,   34,
                        39,   44,   49,   55,   60,   66,   73,   79,   86,   93,  101,  108,  116,  124,  133,  141,
                        150,  159,  168,  177,  187,  197,  207,  217,  227,  238,  249,  259,  270,  282,  293,  304,
                        316,  327,  339,  351,  363,  375,  387,  399,  412,  424,  436,  449,  461,  474,  486,  499,
                        512,  524,  537,  549,  562,  574,  587,  599,  611,  624,  636,  648,  660,  672,  684,  696,
                        707,  719,  730,  741,  753,  764,  774,  785,  796,  806,  816,  826,  836,  846,  855,  864,
                        873,  882,  890,  899,  907,  915,  922,  930,  937,  944,  950,  957,  963,  968,  974,  979,
                        984,  989,  993,  997, 1001, 1004, 1008, 1011, 1013, 1015, 1017, 1019, 1021, 1022, 1022, 1023,
                        1023, 1023, 1022, 1022, 1021, 1019, 1017, 1015, 1013, 1011, 1008, 1004, 1001,  997,  993,  989,
                        984,  979,  974,  968,  963,  957,  950,  944,  937,  930,  922,  915,  907,  899,  890,  882,
                        873,  864,  855,  846,  836,  826,  816,  806,  796,  785,  774,  764,  753,  741,  730,  719,
                        707,  696,  684,  672,  660,  648,  636,  624,  611,  599,  587,  574,  562,  549,  537,  524,
                        512,  499,  486,  474,  461,  449,  436,  424,  412,  399,  387,  375,  363,  351,  339,  327,
                        316,  304,  293,  282,  270,  259,  249,  238,  227,  217,  207,  197,  187,  177,  168,  159,
                        150,  141,  133,  124,  116,  108,  101,   93,   86,   79,   73,   66,   60,   55,   49,   44,
                        39,   34,   30,   26,   22,   19,   15,   12,   10,    8,    6,    4,    2,    1,    1,    0  
                        },
                        {    
                        0,    8,   16,   24,   32,   40,   48,   56,   64,   72,   81,   89,   97,  105,  113,  121,
                        129,  137,  145,  153,  161,  169,  177,  185,  193,  201,  209,  217,  226,  234,  242,  250,
                        258,  266,  274,  282,  290,  298,  306,  314,  322,  330,  338,  346,  354,  362,  371,  379,
                        387,  395,  403,  411,  419,  427,  435,  443,  451,  459,  467,  475,  483,  491,  499,  507,
                        516,  524,  532,  540,  548,  556,  564,  572,  580,  588,  596,  604,  612,  620,  628,  636,
                        644,  652,  661,  669,  677,  685,  693,  701,  709,  717,  725,  733,  741,  749,  757,  765,
                        773,  781,  789,  797,  806,  814,  822,  830,  838,  846,  854,  862,  870,  878,  886,  894,
                        902,  910,  918,  926,  934,  942,  951,  959,  967,  975,  983,  991,  999, 1007, 1015, 1023,
                        1015, 1007,  999,  991,  983,  975,  967,  959,  951,  943,  935,  927,  919,  911,  903,  895,
                        887,  879,  871,  863,  855,  847,  839,  831,  823,  815,  807,  799,  791,  783,  775,  767,
                        759,  751,  743,  735,  727,  719,  711,  703,  695,  687,  679,  671,  663,  655,  647,  639,
                        631,  623,  615,  607,  599,  591,  583,  575,  567,  559,  551,  543,  535,  527,  519,  512,
                        504,  496,  488,  480,  472,  464,  456,  448,  440,  432,  424,  416,  408,  400,  392,  384,
                        376,  368,  360,  352,  344,  336,  328,  320,  312,  304,  296,  288,  280,  272,  264,  256,
                        248,  240,  232,  224,  216,  208,  200,  192,  184,  176,  168,  160,  152,  144,  136,  128,
                        120,  112,  104,   96,   88,   80,   72,   64,   56,   48,   40,   32,   24,   16,    8,    0  
                        },
                        {    
                        0,    4,    8,   12,   16,   20,   24,   28,   32,   36,   40,   44,   48,   52,   56,   60,
                        64,   68,   72,   76,   80,   84,   88,   92,   96,  100,  104,  108,  112,  116,  120,  124,
                        128,  132,  136,  140,  144,  148,  152,  156,  160,  164,  168,  173,  177,  181,  185,  189,
                        193,  197,  201,  205,  209,  213,  217,  221,  225,  229,  233,  237,  241,  245,  249,  253,
                        257,  261,  265,  269,  273,  277,  281,  285,  289,  293,  297,  301,  305,  309,  313,  317,
                        321,  325,  329,  333,  337,  341,  345,  349,  353,  357,  361,  365,  369,  373,  377,  381,
                        385,  389,  393,  397,  401,  405,  409,  413,  417,  421,  425,  429,  433,  437,  441,  445,
                        449,  453,  457,  461,  465,  469,  473,  477,  481,  485,  489,  493,  497,  501,  505,  509,
                        514,  518,  522,  526,  530,  534,  538,  542,  546,  550,  554,  558,  562,  566,  570,  574,
                        578,  582,  586,  590,  594,  598,  602,  606,  610,  614,  618,  622,  626,  630,  634,  638,
                        642,  646,  650,  654,  658,  662,  666,  670,  674,  678,  682,  686,  690,  694,  698,  702,
                        706,  710,  714,  718,  722,  726,  730,  734,  738,  742,  746,  750,  754,  758,  762,  766,
                        770,  774,  778,  782,  786,  790,  794,  798,  802,  806,  810,  814,  818,  822,  826,  830,
                        834,  838,  842,  846,  850,  855,  859,  863,  867,  871,  875,  879,  883,  887,  891,  895,
                        899,  903,  907,  911,  915,  919,  923,  927,  931,  935,  939,  943,  947,  951,  955,  959,
                        963,  967,  971,  975,  979,  983,  987,  991,  995,  999, 1003, 1007, 1011, 1015, 1019, 1023  
                        },
                        { 
                        1023, 1019, 1015, 1011, 1007, 1003,  999,  995,  991,  987,  983,  979,  975,  971,  967,  963,
                        959,  955,  951,  947,  943,  939,  935,  931,  927,  923,  919,  915,  911,  907,  903,  899,
                        895,  891,  887,  883,  879,  875,  871,  867,  863,  859,  855,  850,  846,  842,  838,  834,
                        830,  826,  822,  818,  814,  810,  806,  802,  798,  794,  790,  786,  782,  778,  774,  770,
                        766,  762,  758,  754,  750,  746,  742,  738,  734,  730,  726,  722,  718,  714,  710,  706,
                        702,  698,  694,  690,  686,  682,  678,  674,  670,  666,  662,  658,  654,  650,  646,  642,
                        638,  634,  630,  626,  622,  618,  614,  610,  606,  602,  598,  594,  590,  586,  582,  578,
                        574,  570,  566,  562,  558,  554,  550,  546,  542,  538,  534,  530,  526,  522,  518,  514,
                        509,  505,  501,  497,  493,  489,  485,  481,  477,  473,  469,  465,  461,  457,  453,  449,
                        445,  441,  437,  433,  429,  425,  421,  417,  413,  409,  405,  401,  397,  393,  389,  385,
                        381,  377,  373,  369,  365,  361,  357,  353,  349,  345,  341,  337,  333,  329,  325,  321,
                        317,  313,  309,  305,  301,  297,  293,  289,  285,  281,  277,  273,  269,  265,  261,  257,
                        253,  249,  245,  241,  237,  233,  229,  225,  221,  217,  213,  209,  205,  201,  197,  193,
                        189,  185,  181,  177,  173,  168,  164,  160,  156,  152,  148,  144,  140,  136,  132,  128,
                        124,  120,  116,  112,  108,  104,  100,   96,   92,   88,   84,   80,   76,   72,   68,   64,
                        60,   56,   52,   48,   44,   40,   36,   32,   28,   24,   20,   16,   12,    8,    4,    0  
                        } 
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
const unsigned char g_rgb_color_table[49][3] = 
{
                        { 0, 0, 0},                                                 // first four colors are mapped to the different attenuations for the adc mode, 
                        {48,32, 0},                                                 // i know the math looks chaotic here
                        { 0, 0,48},
                        { 0,48, 0},

                        { 0, 0,48},
                        { 0,16,48},
                        { 0,24,48},

                        {56,40, 0},
                        {32,48,32},

                        { 0,48,24},
                        { 0,40,40},

                        {48,48, 0},
                        { 8, 0,48},
                        {16, 0,48},
                        {24, 0,48},
                        {32, 0,48},
                        {48, 0, 0},
                        { 0,42, 6},
                        {12, 0,48},
                        {36,48, 0},
                        { 0,24,42},
                        {48, 6,36},
                        { 6,48,12},
                        {42, 0,48},
                        {48,30, 0},
                        { 0,48,36},
                        {36,12,48},
                        {48, 0,24},
                        {12,42, 0},
                        { 0,18,48},
                        {48,36, 6},
                        {24, 0,42},
                        { 6,48,30},
                        {42,12, 0},
                        { 0,36,48},
                        {48, 0,36},
                        {18,42, 6},
                        { 0,12,48},
                        {48,30, 0},
                        { 6, 0,42},
                        {36,48,12},
                        { 0,24,48},
                        {42, 6, 0},
                        {12,48,36},
                        {48, 0,42},
                        { 0,36, 6},
                        {30,12,48},
                        {48,42, 0},
                        { 6, 0,48}
};
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------