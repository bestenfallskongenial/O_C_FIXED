/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        okay, oculus mendax software 0.001 rev B with integrated overlay texture atlas menu,
        extended modes and video decoding for raw annex b h264 videos ( i frames only )

        the idea in this header is to have a clear seperation of responsibilities since the code is that big that i lost track long time ago.
        for example do i exclude the #define ( macros ) and #include ( libs / headers ) in seperrate heders here, also enums, structs and members.
        the point is now to include them in the right place and order!

        this file here contains all my functions - i also reitegrated some driver / utility classes back into my project class ( CKernel ),
        sorted by file of origin - we will use this file here also as source of truth for the comments on the function of the parameters!
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
#include "def_include.h"
#include "def_macro.h"

class CKernel
{
public:
	            CKernel (void);
	           ~CKernel (void);

#include "def_enum.h"
#include "def_struct.h"

	        boolean     Initialize (void);

	        TShutdownMode Run (void);

                bool        Mount                       (   const   char*                           p_deviceName);          // "emmc1-1" cd ( root ), "umsd1-1" usb returns success

                bool        UnMount                     ();                                                                 // returns success

                bool        openFile                    (   const   char*                           p_fileName);            // filename format "8.3"

                unsigned    loadToBuffer                (           char*                           p_bufferArray,          // destination buffer for file
                                                                    unsigned                        p_bufferSize);          // max bytes to read into the buffer returns loaded bytes - 0 is false/failed !!!
//5
                bool        saveFromBufferO             (   const   char*                           p_fileName,             // filename format "8.3" 
                                                            const   char*                           p_bufferArray,               // my allocated buffer
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        saveFromBufferM             (   const   char*                           p_deviceName,           // "emmc1-1" cd ( root ), "umsd1-1" usb
                                                            const   char*                           p_fileName,             // filename format "8.3"
                                                            const   char*                           p_bufferArray,               // my allocated buffer 
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        closeFile                   ();                                                                 // release g_hFile handle 

                void        bulkLoad                    (           char*                           p_fileNameArray[],      // where we have stored the filenames from the root directory scan
                                                                    unsigned                        p_loadedBytes[],        // where we store the size in bytes for each file
                                                                    char**                          p_bufferArray,          // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                                    unsigned                        p_maxFiles,             // how many files we are allowed to process ( os limitations )
                                                                    unsigned&                       p_validFiles,           // counts successful loads - we need to keep track here <- MUST initialised with 0
                                                                    unsigned&                       p_prevFiles,            // number of loads from the last call - we need it to init the files correctly
                                                                    unsigned                        p_fileSize);            // maximum size for each file

                bool        IsValidFile                 (   const   char*                           pFileName, 
                                                            const   char*                           extension);
//10
                bool        scanRoot                    (           char**                          p_fileNameArray,        // where we store the valid filenames we find
                                                                    const char*                     p_fileExtArray[],       // the array of valid file extensions for this type of file
                                                                    unsigned                        p_extentionCount,       // how many valid file extensions we have in the array above
                                                                    unsigned&                       p_scannedFiles,         // our counter of found files per device / call
                                                                    unsigned                        p_maxFiles);            // how many files are allowed to scan and stored in the array returns success not files found!

                bool        updateUSB                   (   const   char*                           p_deviceType);          // "umsd1" is the type, not "umsd1-1"needs volatile boolean	m_bStorageAttached ! 

        static  void        removeUSB                   (           CDevice*                        pDevice,                // USB device that was removed
                                                                    void*                           pContext);              // user context pointer; expected to be CKernel*

                char**      allocBufferMEM              (           size_t                          p_count,                // number of buffer slots
                                                                    size_t                          bufferSize);            // size of each buffer in bytes *** msleep ?!

                char**      allocBufferDMA              (           size_t                          p_count,                // number of buffer slots
                                                                    size_t                          bufferSize,             // size of each buffer in bytes
                                                                    char**                          blockBaseOut,           // receives 4K-aligned DMA block base
                                                                    char**                          rawBlockOut,            // receives original raw allocation pointer
                                                                    size_t*                         alignedSizeOut);        // receives total aligned allocation size *** msleep ?!
//15
                void        clearBufferMEM              (           char**                          buffers,                // buffer pointer table returned by allocBufferMEM()
                                                                    size_t                          p_count);               // number of buffers in the table

                void        clearBufferDMA              (           char**                          buffers,                // buffer pointer table returned by allocBufferDMA()
                                                                    char*                           rawBlock);              // original raw allocation pointer to delete
//  GRAPHICS
                void        initOGL                     (           olg_state*                      o);

                void        initVbuffer                 (           olg_state*                      o, 
                                                                    vtx_state*                      v);

                void        initShader                  (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    char**                          p_bufferArray,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    GLenum                          type);
//20
                void        initProgram                 (           vtx_state*                      v,
                                                                    glsl_state*                     vsh,
                                                                    glsl_state*                     fsh,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count);
                void        initTexture                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count,
                                                                    GLint                           wrap_s,
                                                                    GLint                           wrap_t);

                void        initUniform                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);
//  RUNTIME RENDER
                void        frmBufferSet                (           vtx_state*                      v);

                void        frmBufferSwap               (           olg_state*                      o);
//25
                void        setUniPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                /*  int                             gl_current_tex, */
                                                                    unsigned                        p_validTextureCount);

                void        setTexPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             gl_current_tex,
                                                                    unsigned                        p_validTextureCount);
                void        drawGLsPrg                  ();

                void        frmRateBreak                (           bool*                           noTargetFPS);

//30
                void        setUniOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);

                void        setTexOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);
                void        drawGLsOvl                  ();
//  HARDWARE
                u32         read32                   (           uintptr                         nAddress);                          // MMIO
                void        write32                  (           uintptr                         nAddress, 
                                                                    u32                             nValue);
                void        GPIO_SetPull             (           unsigned                        nPin,                               // GPIO
                                                                    unsigned                        nPullMode);
//35
                void        GPIO_SetAlt              (           unsigned                        nPin, 
                                                                    unsigned                        nAltMode, 
                                                                    unsigned                        nPullMode);

                void        GPIO_Write               (           unsigned                        nPin, 
                                                                    unsigned                        nValue);
                unsigned    GPIO_Read                (           unsigned                        nPin);

                void        watchdog_Start           (           unsigned                        nTimeoutSeconds);                   // watchdog

                boolean     SPI_init                 (           void);                                                              // SPI
//40
                int         WriteRead                (           unsigned                        nChipSelect,
                                                            const   void*                           pWriteBuffer,
                                                                    void*                           pReadBuffer,
                                                                    unsigned                        nCount);
    

                boolean     SMI_Init                 (           unsigned                        gpioPin);                           // SMI
                                                                   
                void        SMI_SetupTiming          (           unsigned                        width,
                                                                    unsigned                        cycle_ns,
                                                                    unsigned                        setup,
                                                                    unsigned                        strobe,
                                                                    unsigned                        hold,
                                                                    unsigned                        pace);

                void        SMI_SetupDMA             (           void);
     
                boolean     WS2812_Init              (           unsigned                        ledCount);                          // WS2812
//45            
                void        WS2812_SetLED            (           unsigned                        index, 
                                                                    u8                              red, 
                                                                    u8                              green, 
                                                                    u8                              blue);
                void        WS2812_Update            (           void);    

     

        inline  int         ReadMCP3008Raw              (           unsigned                        channel);                           // MPC 3008

                boolean     frameBufferInit             (           void);                                                              // framebuffer / screen

                void        bufferToScreenPlot          (           unsigned                        x,
                                                                    unsigned                        y,
                                                                    u32                             color);
//50
                void        bufferToScreenDrawChar      (           char                            ch,
                                                                    unsigned                        charCol,
                                                                    unsigned                        charRow,
                                                                    u32                             fgColor,
                                                                    u32                             bgColor);

                void        bufferToScreenClear         (           u32                             bgColor);

                void        bufferToScreenDrawBuffer    (   const   char*                           pSourceBuffer,
                                                                    u32                             startIndex,
                                                                    u32                             endIndex,
                                                                    unsigned                        startCol,
                                                                    unsigned                        startRow,
                                                                    u32                             fgColor,
                                                                    u32                             bgColor);

                void        bufferToScreenGetGrid       (           unsigned&                       cols,
                                                                    unsigned&                       rows);
//  LOGGING
                void        storeLog                (           char*                           p_bufferArray,
                                                                    u32&                            index,
                                                            const   char*                           p_string0, 
                                                                    u32                             p_value0    = EMPTYLOG,
                                                            const   char*                           p_string1   = EMPTYSTR, 
                                                                    u32                             p_value1    = EMPTYLOG,
                                                            const   char*                           p_string2   = EMPTYSTR, 
                                                                    u32                             p_value2    = EMPTYLOG,
                                                            const   char*                           p_string3   = EMPTYSTR, 
                                                                    u32                             p_value3    = EMPTYLOG);
// #define EMPTYSTR ""
// #define EMPTYLOG 255
                void        storeMsg                    (           char*                           p_bufferArray,
                                                                    u32&                            index,
                                                                    const char*                     label,
                                                                    const void*                     tx_msg,
                                                                    u32                             total_size);

                void        nextline                    (           char*                           p_bufferArray,
                                                                    u32&                            index);
// also "only" log / debug or really runtime requirements?
                bool        shaderLog                   (           GLint                           shader, 
                                                                    int                             shaderIndex);

                bool        programLog                  (           GLint                           program, 
                                                                    int                             program_index);
//60
                void        gfx_check                   (   const   char*                           file, 
                                                                    unsigned                        line);

                bool        startupScreen               (           char*                           p_bufferArray,
                                                                    u32&                            index);                                                                    

//  MENU
                void        resetPickUpFlags            ();                                                             // **** okay, i have multiple flags that may or may not be reset ! maybe i will do it centralized here???

                void        set_mode_length             (           uint8_t                         base);              // the "base" of "channels" to process, groups of 4!

                void        mapMenuGroup                (           uint8_t                         menu_id,            // for comparison / exit condition **** here we use a parameter to compair against a global member... nononogo, we can rely only on "m_current_menu" vs "m_last_menu" right? !!! 
                                                                    uint8_t                         base);              // the "base" of "channels" to process, groups of 4!

                void        getChannelModeA             (           int                             p_channel);         // channel to process
//65
                void        getChannelModeB             (           int                             p_channel);         // channel to process 

                void        modeADC                     (           int                             p_channel);         // channel passed by caller

                void        modeTRG                     (           int                             p_channel);         // channel passed by caller

                void        modeBPM                     (           int                             p_channel);         // channel passed by caller

                void        modeLF1                     (           int                             p_channel);         // channel passed by caller
//70
                void        modeLF2                     (           int                             p_channel);         // channel passed by caller

                void        modeAudioAb0                (           int                             p_channel);         // channel passed by caller

                void        modeAudioAb1                (           int                             p_channel);         // channel passed by caller

                void        modeAudioBb0                (           int                             p_channel);         // channel passed by caller

                void        modeAudioBb1                (           int                             p_channel);         // channel passed by caller  
                
                void        updateOvlState              (           olg_state*                      o,                  // because we are concerned with the overlay menu here!
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);                
//  PARSER

                bool        BMPparser                   (           tex_state*                      t,
                                                                    char*                           p_buffer_array[],
                                                                    char*                           filename_array[],
                                                                    size_t                          size_array[],
                                                                    u32                             max_tex_size,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile );
                                
                bool        264parser                   (           h264_state*                     h,
                                                                    char*                           blockBase,
                                                                    char*                           p_buffer_array[],
                                                                    char*                           filename_array[],
                                                                    size_t                          size_array[],
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    u16                             max_width,
                                                                    u16                             max_height,
                                                                    u8                              max_profile,
                                                                    u8                              max_level );

                bool        ParseSPS                    (           u8*                             sps_data,
                                                                    size_t                          sps_size,
                                                                    size_t                          sps_sc_len,
                                                                    u16*                            width,
                                                                    u16*                            height,
                                                                    u8*                             profile,
                                                                    u8*                             level) const;
//80
                size_t      findNext000001              (           u8*                             data, 
                                                                    size_t                          pos, 
                                                                    size_t                          size) const;

                u32         ReadExpGolomb               (           u8*                             data, 
                                                                    size_t*                         bit_offset) const;
//  UTIL
                void        adc_AcquireConvert                     ();                             // can we extract the erraticness / audio engine and the mode_index_mod into separate functions?

                void        adc_ProcessAudio            (   void    );

                void        adc_AdvanceIndex            ();

                bool        checkUpdate                 ();

                bool        Update                      ();

                unsigned    get_gl_time                 (           unsigned                        sys_time );
//85
                void        set_pot_routing             (           bool                            adc_pot_routing );


                char*       make83FileName              (   const   char*                           ext ;)

                void        prepParameters              ();

                void        chooseIndex                 (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount, 
                                                                    bool*                           flags);

                void        chooseIndexD                (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount);
                void        storeModes                  ();
//90
                void        buttonPing                  (           int                             p_btn_id, 
                                                                    int                             p_pin);

                void        button_consumer             (           int                             p_btn_id);
// 
                void        randomVec8                  (           uint32_t                        p_seed);



                void        calculate1BPM               (           int                             p_source, 
                                                                    unsigned long                   p_triggerTimeClock);

                void        predict1Beat                (           int                             p_source, 
                                                                    int                             p_lfoMult);
//95
                void        sample1WaveTable            (           int                             p_source, 
                                                                    int                             p_lfoIn, 
                                                                    int                             p_lfoOut );
//  VC04_SERVICE
public:
        static  void        callbackVCSM                (           void*                           callback_param, 
                                                                    VCHI_CALLBACK_REASON_T          reason, 
                                                                    void*                           msg_handle);

        static  void        callbackMMAL                (           void*                           callback_param, 
                                                                    VCHI_CALLBACK_REASON_T          reason, 
                                                                    void*                           msg_handle);

                void        getStateVCHI                ();                                                             // get the VCHI instance and the connection handle from bcm_host.h

                bool        initEventsVCOS              (           VCOS_EVENT_T&                   event, 
                                                            const   char*                           name);
//100
                u32         convertAddress              (           void*                           p_busAddress, 
                                                                    size_t                          p_size);
                bool        checkGLerrorMMAL            ();

                u32         NextTransId                 (           u32&                            tid);

                void        initHeaderVCSM              (           vc_sm_msg_hdr_t&                hdr, 
                                                                    u32                             type);

                void        initHeaderMMAL              (           mmal_msg_header&                hdr, 
                                                                    u32                             type);
//105
                bool        sendAndWaitVCHI             (           VCHI_SERVICE_HANDLE_T           ServiceHandle,
                                                                    VCOS_EVENT_T&                   VCOSevent,
                                                                    const void*                     msg,
                                                                    size_t                          msg_size,
                                                                    void*                           rx_msg,
                                                                    size_t                          max_reply_len,
                                                                    size_t*                         actual_reply_len);

                bool        openServiceVCHI             (           SERVICE_CREATION_T&             tx,
                                                                    uint32_t                        serviceVersion,
                                                                    uint32_t                        serviceVersionMin,
                                                                    int32_t                         service_id,
                                                                    VCHI_CALLBACK_T                 cb,
                                                                    void*                           cb_param,
                                                                    VCHI_INSTANCE_T                 VCHIInstance,
                                                                    VCHI_SERVICE_HANDLE_T&          ServiceHandle);

                bool        initializeVCSM              ();

                bool        importMemoryVCSM            (           void*                           p_bufferBlockbase, 
                                                                    size_t                          size, 
                                                                    int                             slot, 
                                                                    VCSM_Import_MEM_Msg&            tx, 
                                                                    VCSM_Import_MEM_Reply&          rx);

                bool        lockMemoryVCSM              (           int                             slot, 
                                                                    VCSM_Lock_MEM_Msg&              tx, 
                                                                    VCSM_Lock_MEM_Reply&            rx);
//110
                bool        freeMemoryVCSM              (           int                             slot, 
                                                                    VCSM_Free_MEM_Msg&              tx, 
                                                                    VCSM_Free_MEM_Reply&            rx);

                bool        initializeMMAL              (           u32                             InBufferHandle,             // my input buffer handle  from smem ( we need to figure this out !)
                                                                    u32                             InBufferPointer,            // OR pointer  ( this is the question No 1 here !!! ) i got the feeling i rather need this
                                                                    u32                             InBufferSize,               // my allocated input buffer size
                                                                    u32                             OutBufferHandleA,           // my output buffer handle a from smem
                                                                    u32                             OutBufferPointerA,          // OR pointer?
                                                                    u32                             OutBufferHandleB,           // my output buffer handle a from smem
                                                                    u32                             OutBufferPointerB,          // OR pointer?
                                                                    u32                             OutBufferSize,              // my allocated output buffer size 
                                                                    u32                             ResolutionX,                // obvious, right? use the global macro = less confusion?
                                                                    u32                             ResolutionY,                // obvious, right?
                                                                    EGLDisplay                      eglDisplay,                 // isnt it done anyway globally? redundant?
                                                                    EGLContext                      eglContext);                // isnt it done anyway globally? redundant?
                bool        createTexturesMMAL          ();

                bool        framePollerMMAL             (           u32                             frame_offset,               // comes from the pooler -> h264 struct 
                                                                    u32                             frame_length);              // same same

                bool        bufferReadyMMAL             (           u32                             handle);                    // the vcsm handle of the buffer? from a vc message?
//115
                bool        queueOutputBufferMMAL       (           MMAL_Buffer_From_Host_Msg&      tx, 
                                                                    u32                             vc_handle,                  /
                                                                    u32                             alloc_size);

                bool        queueInputBufferMMAL        (           MMAL_Buffer_From_Host_Msg&      tx, 
                                                                    u32                             frame_offset, 
                                                                    u32                             frame_length);

                bool        createComponent             (           MMAL_Component_Create_Msg&      tx, 
                                                                    MMAL_Component_Create_Reply&    rx);

                bool        getPortInfoMMAL             (           u32                             port_type, 
                                                                    u32&                            port_handle, 
                                                                    MMAL_Port_Info_Get_Msg&         tx, 
                                                                    MMAL_Port_Info_Get_Reply&       rx);

                bool        setPortInfoMMAL             (           MMAL_Port_Info_Set_Msg&         tx, 
                                                                    MMAL_Port_Info_Set_Reply&       rx);
//120
                bool        enableComponentMMAL         (           MMAL_Component_Enable_Msg&      tx, 
                                                                    MMAL_Component_Enable_Reply&    rx);

                bool        setZeroCopyModeMMAL         (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Parameter_Set_Msg&    tx, 
                                                                    MMAL_Port_Parameter_Set_Reply&  rx);
                                                                    
                bool        enablePortMMAL              (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Action_Msg&           tx, 
                                                                    MMAL_Port_Action_Reply_Msg&     rx);

                void        PrimeOutputBufferBodyMMAL   (           MMAL_Buffer_From_Host_Msg&      tx);                          // here we "prime" the messages for usage!

                void        PrimeInputBufferBodyMMAL    (           MMAL_Buffer_From_Host_Msg&      tx);
//125
                void        primePortFormatInputMMAL    (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Info_Set_Msg&         tx);

                void        primePortFormatOutputMMAL   (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Info_Set_Msg&         tx);
/*
// NOT IMPLEMENTED YET!!!
#ifdef __DEBUG_LOG__
        void Log_openService                                      (     const SERVICE_CREATION_T &src);
        void Log_createComponent                                  (     const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply    & rx);
        void Log_getPortInfo                                      (     const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply    & rx);
        void Log_setPortInfo                                      (     const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply    & rx);
        void Log_enableComponentMMAL                              (     const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply    & rx);
        void Log_enablePort                                       (     const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx);
        void Log_setZeroCopyModeMMAL                              (     const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply    & rx);
        void Log_BufferFromHost                                   (     const MMAL_Buffer_From_Host_Msg& rx);
//      void Log_BufferBody                                       (     const mmal_msg_buffer_from_host_wire32& msg );
//      void Log_queueBufferFromHost                              (     const MMAL_Buffer_From_Host_Msg& tx );        
#endif 
*/                                                
//  STUBS
                void            Log_createComponent( const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply& rx);
                void            Log_getPortInfo( const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply& rx);
                void            Log_setPortInfo( const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply& rx);
                void            Log_enableComponentMMAL( const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply& rx);
                void            Log_setZeroCopyModeMMAL( const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply& rx);
                void            Log_enablePort( const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx);
//  WRAPPERS - HERE THE JOY BEGINS
                bool        wrapperInitDMA              ();             // init/alloc the dma buffers 

                bool        wrapperInitMEM              ();             // init/alloc the mem buffers

                void        wrapperDMAcleanUp           ();             // clean/delete the dma buffers
//130                
                void        wrapperMEMcleanUp           ();             // clean delete the mem buffers

                void        wrapper_from_sd             ();             // mount/scan/load and validate memory for sd

                void        wrapper_load_usb            ();             // mount/scan/load and validate memory for usb

                void        wrapper_init_gl_sd          ();             // parse/init vertex/default shader and overlay texture 

                void        wrapper_init_gl_usb         ();             // parse/init user shader/texture/video 
//135
                void        wrapper_io                  ();             // hardware io ( led/adc/ctrl_pin/button )

                void        wrapper_menu                ();             // the true gui, menulayer

                void        wrapper_modes               ();             // the hardware->gui->glsl pipeline 

                bool        wrapperInitVCSMstruct       ();             // for the dynamical vcsm structs allocation

                bool        wrapperInitMMALstruct       ();             // for the dynamical mmal structs allocation
//140
                bool        wrapperFreeVCSMstruct       ();             // for the cleanup of the vcsm structs

                bool        wrapperFreeMMALstruct       ();             // for the cleanup of the mmal structs

#include "def_member.h" // <- should go to the bottom, right?

};