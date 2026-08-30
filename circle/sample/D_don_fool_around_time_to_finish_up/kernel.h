#ifndef _kernel_h
#define _kernel_h

#include "defs_include.h"

            extern "C" void     vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);

#include "o_c_setup.h"
#include "defs_macro.h"

enum TShutdownMode
{
                ShutdownNone,
                ShutdownHalt,
                ShutdownReboot
};

class CKernel
{
public:
                            CKernel                     (               void );
                           ~CKernel                     (               void );

#include "defs_enum.h"
#include "defs_struct.h"

                boolean         Initialize              (               void );
                TShutdownMode   Run                     (               void );
// code_alloc.cpp               !! TESTED AND APPROVED !!
                char**      allocBufferMEM              (               size_t                          p_count,                // +++ number of buffer slots
                                                                        size_t                          bufferSize);            // size of each buffer in bytes *** msleep ?!
                char**      allocBufferDMA              (               size_t                          p_count,                // +++ number of buffer slots
                                                                        size_t                          bufferSize,             // size of each buffer in bytes
                                                                        char**                          blockBaseOut,           // receives 4K-aligned DMA block base
                                                                        char**                          rawBlockOut,            // receives original raw allocation pointer
                                                                        size_t*                         alignedSizeOut);        // receives total aligned allocation size *** msleep ?!
                void        clearBufferMEM              (               char**                          buffers,                // +++ buffer pointer table returned by allocBufferMEM()
                                                                        size_t                          p_count);               // number of buffers in the table
                void        clearBufferDMA              (               char**                          buffers,                // +++ buffer pointer table returned by allocBufferDMA()
                                                                        char*                           rawBlock);              // original raw allocation pointer to delete
// code_debug.cpp               !! TESTED AND APPROVED !!
                void        logButtonStatesRuntime      (               int                             row ); // +++
                void        logModesRuntime             (               int                             row );
                void        logPickUpFlags              (               int                             row );
                void        logInOutRuntime             (               int                             row );      // +++
                void        logInfosRuntime             (               int                             row );
                void        logScreenUpdate             (               void );
                bool        startupScreen               (               void ); /// +++      
                void        logInfoFrameRate            (               int                             row,
                                                                        float                           value );
                void        logInfoFrameRate            (               int                             row );                                                                           
// code_filesystem.cpp          !! TESTED AND APPROVED !!
                bool        Mount                       (       const   char*                           p_deviceName);          // +++ "emmc1-1" cd ( root ), "umsd1-1" usb returns success
                bool        UnMount                     ();                                                                     // +++ returns success
                bool        openFile                    (       const   char*                           p_fileName);            // +++ filename format "8.3"
                unsigned    loadToBuffer                (               char*                           p_bufferArray,          // +++ destination buffer for file
                                                                        unsigned                        p_bufferSize);          // max bytes to read into the buffer returns loaded bytes - 0 is false/failed !!!
                bool        saveFromBuffer              (       const   char*                           p_deviceName,           // +++ "emmc1-1" cd ( root ), "umsd1-1" usb
                                                                const   char*                           p_fileName,             // filename format "8.3"
                                                                const   char*                           p_bufferArray,               // my allocated buffer 
                                                                        unsigned                        p_bufferSize);          // max buffer size
                bool        closeFile                   ();                                                                     // +++ release g_hFile handle 
                void        bulkLoad                    (               char*                           p_fileNameArray[],      // +++ where we have stored the filenames from the root directory scan
                                                                        unsigned                        p_loadedBytes[],        // where we store the size in bytes for each file
                                                                        char**                          p_bufferArray,          // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                                        unsigned                        p_maxFiles,             // how many files we are allowed to process ( os limitations )
                                                                        unsigned&                       p_validFiles,           // counts successful loads - we need to keep track here <- MUST initialised with 0
                                                                        unsigned&                       p_prevFiles,            // number of loads from the last call - we need it to init the files correctly
                                                                        unsigned                        p_fileSize);            // maximum size for each file
                bool        IsValidFile                 (       const   char*                           pFileName,              // +++
                                                                const   char*                           extension);
                bool        scanRoot                    (               char**                          p_fileNameArray,        // +++ where we store the valid filenames we find
                                                                const   char*                           p_fileExtArray[],       // the array of valid file extensions for this type of file
                                                                        unsigned                        p_extentionCount,       // how many valid file extensions we have in the array above
                                                                        unsigned&                       p_scannedFiles,         // our counter of found files per device / call
                                                                        unsigned                        p_maxFiles,             // how many files are allowed to scan and stored in the array returns success not files found!
                                                                        unsigned                        p_prevFiles);           // the amount of files we already loaded into the buffers 
                char*       gen83FileName               (       const   char*                           ext );   // +++
                bool        UpdateKernel                ();
                bool        updateUSB                   (       const   char*                           p_deviceType);          // +++ "umsd1" is the type, not "umsd1-1"needs volatile boolean	m_bStorageAttached ! 
        static  void        removeUSB                   (               CDevice*                        pDevice,                // +++ USB device that was removed
                                                                        void*                           pContext);              // user context pointer; expected to be CKernel*
// code_graphics_init.cpp         !! TESTED AND APPROVED  - textures as well ?? 
                void        initOGL                     (               olg_state*                      o);                     // +++
                void        initVbuffer                 (               olg_state*                      o, 
                                                                        vtx_state*                      v);
                void        initShader                  (               vtx_state*                      v,
                                                                        glsl_state*                     s,
                                                                        tex_state*                      t,
                                                                        char**                          p_bufferArray,
                                                                        int                             p_fromFile,
                                                                        int                             p_toFile,
                                                                        GLenum                          type);
                void        initProgram                 (               vtx_state*                      v,
                                                                        glsl_state*                     vsh,
                                                                        glsl_state*                     fsh,
                                                                        tex_state*                      t,
                                                                        int                             p_fromFile,
                                                                        int                             p_toFile,
                                                                        unsigned&                       valid_count);
                void        initTexture                 (               vtx_state*                      v,
                                                                        glsl_state*                     s,
                                                                        tex_state*                      t,
                                                                        int                             p_fromFile,
                                                                        int                             p_toFile,
                                                                        unsigned&                       valid_count,
                                                                        GLint                           wrap_s,
                                                                        GLint                           wrap_t);
                void        initUniform                 (               vtx_state*                      v,
                                                                        glsl_state*                     s,
                                                                        tex_state*                      t,
                                                                        int                             p_fromFile,
                                                                        int                             p_toFile);
// code_graphics_log.cpp        !! TESTED AND APPROVED - EXCEPT gfx_check() - does it actually put out log before the crash or should i use CLooger there instead ??
                bool        shaderLog                   (               GLint                           shader, 
                                                                        int                             shaderIndex);
                bool        programLog                  (               GLint                           program, 
                                                                        int                             program_index);
                void        gfx_check                   (       const   char*                           file, 
                                                                        unsigned                        line);
// code_graphics_run.cpp        !! NEED TO TEST THE OVERLAY PART HERE !!
                void        frmBufferSet                (               vtx_state*                      v);                     // !! WORKS !!
                void        frmBufferSwap               (               olg_state*                      o);                     // !! WORKS !! 
                void        setUniPrg                   (               olg_state*                      o,
                                                                        glsl_state*                     s,
                                                                        tex_state*                      t,
                                                                        /*  int                         gl_current_tex, */
                                                                        unsigned                        p_validTextureCount);   // !! WORKS !! 
                void        setTexPrg                   (               olg_state*                      o,
                                                                        glsl_state*                     s,
                                                                        tex_state*                      t,
                                                                        int                             gl_current_tex,
                                                                        unsigned                        p_validTextureCount);   // ?? UNTESTED ??
                void        drawGLsPrg                  ();                                                                     // !! WORKS !!
        //      void        frmRateBreak                (               bool                            noTargetFPS);
                void        setUniOvl                   (               olg_state*                      o, 
                                                                        glsl_state*                     s, 
                                                                        tex_state*                      t);                     
                void        setTexOvl                   (               olg_state*                      o, 
                                                                        glsl_state*                     s, 
                                                                        tex_state*                      t);
                void        drawGLsOvl                  ();                  

                void        fpsBegin                    ();
                void        fpsBreak                    ();
                void        fpsEnd                      ();
// code_hardware.cpp            !! TESTED AND APPROVED - as far as i can tell !! 
                void        usDelay                     (               unsigned us);
                void        msDelay                     (               unsigned ms);                                           // ++
                u32         read32                      (               uintptr                         nAddress);                          // +++ MMIO
                void        write32                     (               uintptr                         nAddress,          // +++             
                                                                        u32                             nValue);
                void        GPIO_SetPull                (               unsigned                        nPin,                               // +++ GPIO
                                                                        unsigned                        nPullMode);
                void        GPIO_SetAlt                 (               unsigned                        nPin,           // +++
                                                                        unsigned                        nAltMode, 
                                                                        unsigned                        nPullMode);
                void        GPIO_Write                  (               unsigned                        nPin,           // +++
                                                                        unsigned                        nValue);
                void        set_pot_routing             (               int                             pin,
                                                                        bool                            adc_pot_routing);                                                                        
                unsigned    GPIO_Read                   (               unsigned                        nPin);
                void        buttonPingA                 (               int                             p_btn_id,           // +++
                                                                        int                             p_pin);
                void        buttonPingB                 (               int                             p_btn_id_A, 
                                                                        int                             p_pin_A,
                                                                        int                             p_btn_id_B, 
                                                                        int                             p_pin_B );                                                                        
                void        watchdog_Start              (               unsigned                        nTimeoutSeconds);                   // watchdog
                bool        SPI_init                    (               void);                                                              // +++ SPI
                int         WriteRead                   (               unsigned                        nChipSelect,    // +++
                                                                const   void*                           pWriteBuffer,
                                                                        void*                           pReadBuffer,
                                                                        unsigned                        nCount);
                bool        SMI_Init                    (               unsigned                        gpioPin);                           // +++ SMI
                void        SMI_SetupTiming             (               unsigned                        width,                              // +++
                                                                        unsigned                        cycle_ns,
                                                                        unsigned                        setup,
                                                                        unsigned                        strobe,
                                                                        unsigned                        hold,
                                                                        unsigned                        pace);
                void        SMI_SetupDMA                (               size_t                          byteLength);                        // +++
                bool        WS2812_Init                 (               unsigned                        ledCount);                          // WS2812
                void        WS2812_SetLED               (               unsigned                        index,                              // +++
                                                                        u8                              red, 
                                                                        u8                              green, 
                                                                        u8                              blue);
                void        WS2812_Update               (               void);                                                              // +++
                int         ReadMCP3008Raw              (               unsigned                        channel);                           // +++ MPC 3008
                void        readAndConvertADC           ();                                                                 // +++ can we extract the erraticness / audio engine and the mode_index_mod into separate functions?

                void        adc_ProcessAudio            (               void );

                void        adc_AdvanceIndex            ();                              
                bool        frameBufferInit             (               void );                                                             // +++
                void        bufferScreenPlot            (               unsigned                        x,                                  // +++
                                                                        unsigned                        y,
                                                                        u32                             color );
                void        bufferScreenDrawChar        (               char                            ch,                                // +++
                                                                        unsigned                        charCol,
                                                                        unsigned                        charRow,
                                                                        u32                             fgColor );
                void        bufferScreenClear           (               void );                                                           // +++
                void        bufferScreenDraw            (       const   char*                           pSourceBuffer,                    // +++
                                                                        u32                             startIndex,
                                                                        u32                             endIndex,
                                                                        unsigned                        startCol,
                                                                        unsigned                        startRow,
                                                                        u32                             fgColor );
                void        bufferScreenGetGrid         (               unsigned&                       cols,           // +++
                                                                        unsigned&                       rows );
// code_log.cpp:
                void        storeLogHex                 (               char*                           p_buffer,                    // +++
                                                                        u32&                            index,
                                                                const   char*                           p_string0, 
                                                                        u32                             p_value0    = X_VAL,
                                                                const   char*                           p_string1   = X_STR, 
                                                                        u32                             p_value1    = X_VAL,
                                                                const   char*                           p_string2   = X_STR, 
                                                                        u32                             p_value2    = X_VAL,
                                                                const   char*                           p_string3   = X_STR, 
                                                                        u32                             p_value3    = X_VAL);
                void        storeLogU32                 (               char*                           p_buffer,                    // +++
                                                                        u32&                            index,
                                                                const   char*                           p_string0, 
                                                                        u32                             p_value0    = X_VAL,
                                                                const   char*                           p_string1   = X_STR, 
                                                                        u32                             p_value1    = X_VAL,
                                                                const   char*                           p_string2   = X_STR, 
                                                                        u32                             p_value2    = X_VAL,
                                                                const   char*                           p_string3   = X_STR, 
                                                                        u32                             p_value3    = X_VAL);                                                
                                                                        
                void        storeMsg                   (                char*                           p_buffer,
                                                                        u32&                            index,
                                                                const   char*                           label,
                                                                const   void*                           tx_msg,
                                                                        u32                             total_size);
                void        nextline                    (               char*                           p_buffer,                       // +++
                                                                        u32&                            index);
                u32         convertDecToBCD     (                       u32                             value );
                bool        memoryDebugCheckpoint       (       const   char*                           p_Label,
                                                                        bool                            p_DumpStatus = false );
          
        static  void        LoggerSink                 (                void*                           pContext,
                                                                const   char*                           pText,
                                                                        unsigned                        nLength );      // +++ 
// code_menu.cpp                !! NEED FINALLY TO CREATE THE BUTTON CONSUMER / MENU-LAYER CODE DONE !!
                void        resetMenuPickUpFlags        ();
                void        storeModes                  ();
                void        buttonConsumer              (               int                             buttonA, 
                                                                        int                             buttonB);
                void        dispatchLayer               ();
                void        set_mode_roof_map           (               uint8_t                         block   );
                void        mapMenuGroup                (               uint8_t                         block   );
                void        getChannelModeA             (               int                             p_channel );                
                void        getChannelModeB             ();
                void        modeADC                     (               int                             p_channel);         // channel passed by caller
                void        modeTRG                     (               int                             p_channel);         // channel passed by caller
//              void        modeBPM                     (               int                             p_channel);         // channel passed by caller
                void        modeLF1                     (               int                             p_channel);         // channel passed by caller
                void        modeLF2                     (               int                             p_channel);         // channel passed by caller
                void        modeAudioAbL                (               int                             p_channel);         // channel passed by caller
                void        modeAudioAbH                (               int                             p_channel);         // channel passed by caller
                void        modeAudioBbL                (               int                             p_channel);         // channel passed by caller
                void        modeAudioBbH                (               int                             p_channel);         // channel passed by caller  
                void        applyTargetModes            ();
                void        updateLED                   (); // current!!
                void        checkSystemFlags            ();

// code_midi.cpp
#ifdef USE_MIDI
                bool        updateMIDI                  ();
        static  void        removeMIDI                  (               CDevice*                        pDevice,
                                                                        void*                           pContext);
        static  void        MIDIPacketHandler           (               unsigned                        nCable,
                                                                        u8*                             pPacket,
                                                                        unsigned                        nLength,
                                                                        unsigned                        nDevice,
                                                                        void*                           pParam);
// relocated to code_menu.cpp!                                                                        
                void        modeMidiNote                (               int                             p_channel);
                void        modeMidiCC0                 (               int                             p_channel);
                void        modeMidiCC1                 (               int                             p_channel);
#endif
// code_parser.cpp              !! TESTED AND APPROVED !!
                bool        BMPparser                   (               tex_state*                      t,
                                                                        char*                           p_buffer_array[],
                                                                        char*                           filename_array[],
                                                                        size_t                          size_array[],
                                                                        u32                             max_tex_size,
                                                                        int                             p_fromFile,
                                                                        int                             p_toFile );
                bool        parse264                    (               h264_state*                     h,
                                                                        char*                           blockBase,
                                                                        char*                           p_buffer_array[],
                                                                        char*                           filename_array[],
                                                                        size_t                          size_array[],
                                                                        int                             p_fromFile,
                                                                        int                             p_toFile,
                                                                        unsigned&                       valid_count, // * changed !! *
                                                                        u16                             min_width,
                                                                        u16                             max_width,
                                                                        u16                             min_height,
                                                                        u16                             max_height,
                                                                        u8                              min_profile,
                                                                        u8                              max_profile,
                                                                        u8                              min_level,
                                                                        u8                              max_level );
                bool        ParseSPS                    (               u8*                             sps_data,
                                                                        size_t                          sps_size,
                                                                        size_t                          sps_sc_len,
                                                                        u16*                            width,
                                                                        u16*                            height,
                                                                        u8*                             profile,
                                                                        u8*                             level) const;
                size_t      findNext000001              (               u8*                             data, 
                                                                        size_t                          pos, 
                                                                        size_t                          size) const;
                u32         ReadExpGolomb               (               u8*                             data, 
                                                                        size_t*                         bit_offset) const;
// code_user_file.cpp
                void        create_buffer_file          ();
                void        parse_buffer_file           ();

                const char* bufferSourceModeName        (               u32                             value); 
                const char* bufferWaveName              (               u32                             value);
                const char* bufferChannelName           (               u32                             value);
                u32         bufferMultValue             (               u32                             index);
                u32         hexToDec                    (               u32                             value);
                bool        sameText                    (       const   char*                           a, 
                                                                const   char*                           b);
                bool        tokenEnd                    (               char                            value);
                void        skipBufferLine              (       const   char*&                          read);
                bool        readBufferToken             (       const   char*&                          read, 
                                                                        char*                           token, 
                                                                        u32                             size);
                bool        readBufferDec               (       const   char*                           token, 
                                                                        u32&                            value);
                bool        readSourceMode              (       const   char*                           token, 
                                                                        u32&                            value);
                bool        readWave                    (       const   char*                           token, 
                                                                        u32&                            value);
                bool        readMultiplier              (       const   char*                           token, 
                                                                        u32&                            value);
                bool        readChannel                 (       const   char*                           token, 
                                                                        u32&                            value);
                bool        readFlag                    (       const   char*                           token, 
                                                                        u32&                            field);
                bool        readChannelCommand          (       const   char*                           token, 
                                                                        u32&                            field);
// code_util.cpp
                bool        setSystemVariables          ();                
                bool        checkUpdate                 ();
                void        get_gl_time                 (               unsigned                        sys_time );                


                void        randomVec8                  (               uint32_t                        p_seed);            // +++

                void        calculate1BPMold            (               int                             p_source, 
                                                                        unsigned long                   p_triggerTimeClock);
                void        calculate1BPMnew            (               int                             p_source, 
                                                                        int                             p_timeBuffer,
                                                                        int                             p_deltaBuffer,
                                                                        unsigned long                   p_triggerTimeClock );                                                                         
                void        predict1Beat                (               int                             p_source, 
                                                                        int                             p_lfoMult);
                void        sample1WaveTable            (               int                             p_source, 
                                                                        int                             p_lfoIn, 
                                                                        int                             p_lfoOut,
                                                                        int                             p_samples);   // NEW p_samples is the max sample -1 
// code_vc_mmal_api.cpp 
                bool        framePollerMMAL             (               u32                             nal_block_offset,               // comes from the pooler -> h264 struct 
                                                                        u32                             nal_block_length);              // same same
                bool        bufferReadyMMAL             (               u32                             handle);                    // the vcsm handle of the buffer? from a vc message?
                bool        queueOutputBufferMMAL       (               MMAL_Buffer_From_Host_Msg&      tx, 
                                                                        u32                             vc_handle,                  
                                                                        u32                             alloc_size);
                bool        queueInputBufferMMAL        (               MMAL_Buffer_From_Host_Msg&      tx, 
                                                                        u32                             nal_block_offset, 
                                                                        u32                             nal_block_length);
// code_vc_mmal_init.cpp        !! TESTED AND APPROVED !!
                bool        initTexturesMMAL            ();
                bool        createTexturesMMAL          ();
                bool        createComponent             (               u32&                            ComponentHandle,
                                                                        MMAL_Component_Create_Msg&      tx, 
                                                                        MMAL_Component_Create_Reply&    rx);
                bool        getPortInfoMMAL             (               u32                             port_type, 
                                                                        u32&                            port_handle, 
                                                                        MMAL_Port_Info_Get_Msg&         tx, 
                                                                        MMAL_Port_Info_Get_Reply&       rx);
                bool        setPortInfoMMAL             (               MMAL_Port_Info_Set_Msg&         tx, 
                                                                        MMAL_Port_Info_Set_Reply&       rx);
                bool        enableComponentMMAL         (               MMAL_Component_Enable_Msg&      tx, 
                                                                        MMAL_Component_Enable_Reply&    rx);
                bool        setZeroCopyModeMMAL         (               u32                             port_handle,
                                                             /* const   MMAL_Port_Info_Get_Reply&       src, */
                                                                        MMAL_Port_Parameter_Set_Msg&    tx, 
                                                                        MMAL_Port_Parameter_Set_Reply&  rx);
                bool        enablePortMMAL              (       const   MMAL_Port_Info_Get_Reply&       src, 
                                                                        MMAL_Port_Action_Msg&           tx, 
                                                                        MMAL_Port_Action_Reply_Msg&     rx);
                void        PrimeOutputBufferBodyMMAL   (               MMAL_Buffer_From_Host_Msg&      tx);                          // here we "prime" the messages for usage!
                void        PrimeInputBufferBodyMMAL    (               MMAL_Buffer_From_Host_Msg&      tx);
                void        primePortFormatInputMMAL    (               u32 bufferSize,
                                                                const   MMAL_Port_Info_Get_Reply&       src, 
                                                                        MMAL_Port_Info_Set_Msg&         tx);
                void        primePortFormatOutputMMAL   (               u32 bufferSize,
                                                                const   MMAL_Port_Info_Get_Reply&       src, 
                                                                        MMAL_Port_Info_Set_Msg&         tx);                
// code_vc_service.cpp          !! TESTED AND APPROVED !!
        static  void        callbackVCSM                (               void*                           callback_param, 
                                                                        VCHI_CALLBACK_REASON_T          reason, 
                                                                        void*                           msg_handle);
        static  void        callbackMMAL                (               void*                           callback_param, 
                                                                        VCHI_CALLBACK_REASON_T          reason, 
                                                                        void*                           msg_handle);
                void        getStateVCHI                ();                                                             // get the VCHI instance and the connection handle from bcm_host.h
                bool        initEventsVCOS              (               VCOS_EVENT_T&                   event, 
                                                                const   char*                           name);
                u32         convertAddress              (               void*                           p_busAddress, 
                                                                        size_t                          p_size);
                bool        checkGLerrorMMAL            ();
                u32         NextTransId                 (               u32&                            tid);
                void        initHeaderVCSM              (               vc_sm_msg_hdr_t&                hdr, 
                                                                        u32                             type);
                void        initHeaderMMAL              (               mmal_msg_header&                hdr, 
                                                                        u32                             type);
                bool        sendAndWaitVCHI             (               VCHI_SERVICE_HANDLE_T           ServiceHandle,
                                                                        VCOS_EVENT_T&                   VCOSevent,
                                                                const   void*                           msg,
                                                                        size_t                          msg_size,
                                                                        void*                           rx_msg,
                                                                        size_t                          max_reply_len,
                                                                        size_t*                         actual_reply_len);
                bool        openServiceVCHI             (               SERVICE_CREATION_T&             tx,
                                                                        uint32_t                        serviceVersion,
                                                                        uint32_t                        serviceVersionMin,
                                                                        int32_t                         service_id,
                                                                        VCHI_CALLBACK_T                 cb,
                                                                        void*                           cb_param,
                                                                        VCHI_INSTANCE_T                 VCHIInstance,
                                                                        VCHI_SERVICE_HANDLE_T&          ServiceHandle);
// code_vc_vcsm.cpp             !! TESTED AND APPROVED !!
                bool        allocMemoryVCSM             (               size_t                          size,
                                                                        u32                             base_unit,
                                                                        u32                             alignment,
                                                                        vc_sm_alloc_type_t              type,
                                                                        u32                             allocator,
                                                                const   char*                           name,
                                                                        u32&                            vcsm_handle,
                                                                        VCSM_Alloc_MEM_Msg&             tx,
                                                                        VCSM_Alloc_MEM_Reply&           rx);
                bool        importMemoryVCSM            (               void*                           p_bufferBlockbase, 
                                                                        size_t                          size, 
                                                                    /*  int                             slot, */ 
                                                                        u32&                            vcsm_handle,   
                                                                        VCSM_Import_MEM_Msg&            tx, 
                                                                        VCSM_Import_MEM_Reply&          rx);
                bool        lockMemoryVCSM              (            /* int                             slot, */
                                                                        u32&                            vcsm_handle,
                                                                        u32&                            vcsm_pointer, 
                                                                        VCSM_Lock_MEM_Msg&              tx, 
                                                                        VCSM_Lock_MEM_Reply&            rx);
                bool        freeMemoryVCSM              (            /* int                             slot, */ 
                                                                        u32&                            vcsm_handle, 
                                                                        u32&                            vcsm_pointer,                                             
                                                                        VCSM_Free_MEM_Msg&              tx, 
                                                                        VCSM_Free_MEM_Reply&            rx);
// code_wrappers.cpp - HERE THE JOY BEGINS
                bool        wrapperInitDMA              ();             // init/alloc the dma buffers +++
                bool        wrapperInitMEM              ();             // init/alloc the mem buffers +++
                void        wrapperDMAcleanUp           ();             // clean/delete the dma buffers +++
                void        wrapperMEMcleanUp           ();             // clean delete the mem buffers +++      
                bool        wrapperVCSM                 ();
                bool        wrapperMMAL                 ();
                void        wrapper_load_sd             ();             // mount/scan/load and validate memory for sd +++
                void        wrapper_load_usb            ();             // mount/scan/load and validate memory for usb +++       
                void        wrapper_parser_sd           ();
                void        wrapper_parser_usb          ();                
                void        wrapper_init_gl_sd          ();             // parse/init vertex/default shader and overlay texture                 
                void        wrapper_init_gl_usb         ();             // parse/init user shader/texture/video 
                bool        wrapperInitVCSMstruct       ();             // for the dynamical vcsm structs allocation
                bool        wrapperInitMMALstruct       ();             // for the dynamical mmal structs allocation
                void        wrapperFreeVCSMstruct       ();             // for the cleanup of the vcsm structs
                void        wrapperFreeMMALstruct       ();             // for the cleanup of the mmal structs

                void        FATCallback                 (               void );

#include "defs_member.h"
#include "table_lfo.cpp"
#include "table_col.cpp"
};

#endif