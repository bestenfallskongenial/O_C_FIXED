//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::callbackVCSM               (   void*                   callback_param, 
                                                        VCHI_CALLBACK_REASON_T  reason, 
                                                        void*                   msg_handle )
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {
                    vcos_event_signal(event);
                    }
}

void            CKernel::callbackMMAL               (   void*                   callback_param, 
                                                        VCHI_CALLBACK_REASON_T  reason, 
                                                        void*                   msg_handle )
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {
                    vcos_event_signal(event);
                    }
}

void            CKernel::getStateVCHI               (   )
{
                vc_host_get_vchi_state(&m_VCHIInstance, &m_Connection);
#ifdef __DEBUG_LOG__
                storeLog ( MY_BUFFER, MY_INDEX, "VCHI State Instance", (u32)m_VCHIInstance, "VCHI State Connection", (u32)m_Connection);   
#endif             
}

bool            CKernel::initEventsVCOS             (   VCOS_EVENT_T&           event, 
                                                        const char*             name)
{
                if (vcos_event_create(&event, name) != VCOS_SUCCESS)
                    {
#ifdef __DEBUG_LOG__
                    storeLog ( MY_BUFFER, MY_INDEX, "VCOS Event Init Failed!", (u32)&event);  
#endif               
                    return false;
                    }
                return true;
}

u32             CKernel::convertAddress             (   void*                   p_busAddress, 
                                                        size_t                  p_size )
{
                u32 bus_addr = BUS_ADDRESS(reinterpret_cast<uintptr_t>(p_busAddress));
                u32 vcsm_addr = (bus_addr & ~0xC0000000) | 0xC0000000;
#ifdef __DEBUG_LOG__
                storeLog ( MY_BUFFER, MY_INDEX, "Buffer Address BUS", (u32)p_busAddress, "Buffer Address ARM", (u32)bus_addr, "Buffer Address VPU", (u32)vcsm_addr); 
#endif 
                CleanAndInvalidateDataCacheRange((uintptr_t)(p_busAddress), p_size);

                return vcsm_addr;
}

bool            CKernel::checkGLerrorMMAL           (   )
{
                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                    {
                    const char* error_str;
                    
                    switch(error) 
                        {
                        case        GL_INVALID_ENUM:                    error_str = "GL_INVALID_ENUM"; break;
                        case        GL_INVALID_VALUE:                   error_str = "GL_INVALID_VALUE"; break;
                        case        GL_INVALID_OPERATION:               error_str = "GL_INVALID_OPERATION"; break;
                        case        GL_OUT_OF_MEMORY:                   error_str = "GL_OUT_OF_MEMORY"; break;
                        case        GL_INVALID_FRAMEBUFFER_OPERATION:   error_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
                        default:                                        error_str = "UNKNOWN_ERROR"; break;
                        }
#ifdef __DEBUG_LOG__  
                    storeLog ( MY_BUFFER, MY_INDEX, error_str);
#endif 
                    return false;
                    }
                return true;    
}

u32             CKernel::NextTransId                (   u32&                    tid )
{
                tid = ( tid+1 ) & ~0x80000000u;                  
                return tid;
}

void            CKernel::initHeaderVCSM             (   vc_sm_msg_hdr_t&        hdr, 
                                                        u32                     type)
{␊
                hdr                 = {};
                hdr.type            = type;
                hdr.trans_id        = NextTransId(m_TransactionId);
}␊

void            CKernel::initHeaderMMAL             (   mmal_msg_header&        hdr, 
                                                        u32                     type)
{
                hdr                 = {};
                hdr.magic           = MMAL_MAGIC;
                hdr.type            = type;
                hdr.control_service = 0;
                hdr.context         = NextTransId(m_TransactionId);
                hdr.status          = 0;
                hdr.padding         = 0;
}

bool            CKernel::sendAndWaitVCHI            (   VCHI_SERVICE_HANDLE_T   ServiceHandle, 
                                                        VCOS_EVENT_T&           VCOSevent, 
                                                        const void*             msg, 
                                                        size_t                  msg_size, 
                                                        void*                   rx_msg, 
                                                        size_t                  max_reply_len, 
                                                        size_t*                 actual_reply_len )
{
#ifdef __DEBUG_LOG__
            //  nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "TX MSG", (u32)msg_size);
                storeMsg( MY_BUFFER, MY_INDEX, "Raw TX", msg, msg_size);
#endif 

                if (vchi_msg_queue(ServiceHandle, msg, msg_size, VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL) != 0)
                    {
                    return false;
                    }
                uint32_t ReplyLength = 0;
                do {
                    if (vchi_msg_dequeue(ServiceHandle, rx_msg, max_reply_len, &ReplyLength, VCHI_FLAGS_NONE) == 0)
                        {
#ifdef __DEBUG_LOG__
                        storeLog( MY_BUFFER, MY_INDEX, "RX MSG", ReplyLength);
                        storeMsg( MY_BUFFER, MY_INDEX, "Raw RX", rx_msg, ReplyLength);
#endif 
                        break;
                        }
                    } 
                while (vcos_event_wait(&VCOSevent) == VCOS_SUCCESS);

                if (actual_reply_len)
                    {
                    *actual_reply_len = ReplyLength;
                    }
                if (ReplyLength != max_reply_len)
                    {
#ifdef __DEBUG_LOG__
                //  const mmal_msg_header* h = (const mmal_msg_header*)msg; 
                //  nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "ANSWER TO SHORT - MSG #" /*, h->context*/ ); // is only available with mmal, not for vcsm
#endif 
                    return false;
                    }
                return true;                                    //  return (ReplyLength != 0);
}

bool            CKernel::openServiceVCHI(               SERVICE_CREATION_T&     tx, 
                                                        uint32_t                serviceVersion, 
                                                        uint32_t                serviceVersionMin, 
                                                        int32_t                 service_id, 
                                                        VCHI_CALLBACK_T         cb, 
                                                        void*                   cb_param, 
                                                        VCHI_INSTANCE_T         VCHIInstance, 
                                                        VCHI_SERVICE_HANDLE_T&  ServiceHandle )

{
            //  SERVICE_CREATION_T tx = {}; // why not here?!
                tx.version.version          = serviceVersion;
                tx.version.version_min      = serviceVersionMin;
                tx.service_id               = service_id;
                tx.connection               = m_Connection;

                tx.rx_fifo_size             = 0;
                tx.tx_fifo_size             = 0;

                tx.callback                 = cb;
                tx.callback_param           = cb_param;

                tx.want_unaligned_bulk_rx   = 0;
                tx.want_unaligned_bulk_tx   = 0;
                tx.want_crc                 = 0;

                int rc = vchi_service_open(VCHIInstance, &tx, &ServiceHandle);
                return (rc == 0);
}

bool            CKernel::initializeVCSM             (   )
{
bool bOK = true;

#ifdef __DEBUG_LOG__
            //  storeLog ( MY_BUFFER, MY_INDEX, SERVICENAMESTRING, SERVICEVERSIONSTRING);    
#endif 

                getStateVCHI                ();
                if (bOK)
                    {
                    bOK = initEventsVCOS(m_EventSMEM, "SMEM");
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "VCSM initEventsVCOS FAILED");
#endif 
                    if (!bOK) return false;
                    }

                if (bOK)
                    {
                    bOK = openServiceVCHI ( m_ServiceCreateVCSM,
                                            VC_SM_VER,
                                            VC_SM_MIN_VER,
                                            VCHIQ_MAKE_FOURCC('S','M','E','M'),
                                            callbackVCSM,
                                            &m_EventSMEM,
                                            m_VCHIInstance,
                                            m_ServiceHandleVCSM
                                            );
#ifdef __DEBUG_LOG__            
                    if (!bOK) storeLog ( MY_BUFFER, MY_INDEX, "VCHI openService FAILED!");      
#endif                     
                    if (!bOK) return false;
                    }
#ifdef __DEBUG_LOG__ 
            //  nextline ( MY_BUFFER, MY_INDEX );
                storeLog ( MY_BUFFER, MY_INDEX, "VCSM Successful Initialized");
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");                
#endif 
                return bOK;
}

bool            CKernel::importMemoryVCSM           (   void*                   p_bufferBlockbase, 
                                                        size_t                  size, 
                                                        int                     slot, 
                                                    /*  u32&                    vcsm_handle,  */ 
                                                        VCSM_Import_MEM_Msg&    tx, 
                                                        VCSM_Import_MEM_Reply&  rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_IMPORT);

                tx.body = {};
                tx.body.type       = VC_SM_ALLOC_NON_CACHED;
                tx.body.addr       = convertAddress(p_bufferBlockbase, size);
                tx.body.size       = static_cast<u32>(size);
                tx.body.kernel_id  = 0;
                tx.body.allocator  = 0;
                strncpy(tx.body.name, "SMEM", sizeof(tx.body.name));

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len ))
                    return false;

                if (rx.body.res_handle != 0)
                {
                    m_vc_handle[slot]   = rx.body.res_handle;
                /*  vcsm_handle         = rx.body.res_handle;      like this ? */                   
#ifdef __DEBUG_LOG__
                    //  nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Import VCSM Memory to Slot", slot /*vcsm_handle*/); 
                        storeLog( MY_BUFFER, MY_INDEX, "ARM Address", p_bufferBlockbase, "GPU Address", tx.body.addr, "Size", size, "VCSM Handle", rx.body.res_handle);
#endif                     
                    return true;
                }
                return false;
}

bool            CKernel::lockMemoryVCSM             (   int                     slot,
                                                    /*  u32&                    vcsm_handle, */
                                                    /*  u32&                    vcsm_pointer,  */ 
                                                        VCSM_Lock_MEM_Msg&      tx, 
                                                        VCSM_Lock_MEM_Reply&    rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_LOCK);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot] /*vcsm_handle*/; 
                tx.body.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) )
                    return false;

                if (rx.body.res_mem != 0)
                {
                    m_vc_pointer[slot]  = rx.body.res_mem;   
                /*  vcsm_pointer        = rx.body.res_mem;   like this ?   */                 
#ifdef __DEBUG_LOG__
                    //  nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Lock VCSM Memory in Slot   ", slot /*vcsm_handle*/);  
                        storeLog( MY_BUFFER, MY_INDEX, "Lock  Memory in VCSM Handle", rx.body.res_handle, "VCSM Pointer", rx.body.res_mem);
#endif         
                    return true;
                }
                return false;
}

bool            CKernel::freeMemoryVCSM             (   int                     slot, 
                                                    /*  u32&                    vcsm_handle, */
                                                    /*  u32&                    vcsm_pointer, */                                            
                                                        VCSM_Free_MEM_Msg&      tx, 
                                                        VCSM_Free_MEM_Reply&    rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_FREE);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot];
                tx.body.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) )
                    return false;

                if (rx.body.success == 0)
                {
                    m_vc_handle[slot]   = 0;
                    m_vc_pointer[slot]  = 0;
                /*  vcsm_handle         = 0; */
                /*  vcsm_pointer        = 0; */
#ifdef __DEBUG_LOG__
                    //  nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Free  Memory in VCSM Handle", vcsm_handle, "VCSM Pointer", vcsm_pointer);  
#endif        
                    return true;
                }
                return false;
}

bool            CKernel::initializeMMAL             (   u32                     InBufferHandle,
                                                        u32                     InBufferPointer,
                                                        u32                     InBufferSize, 
                                                        u32                     OutBufferHandleA,
                                                        u32                     OutBufferPointerA,
                                                        u32                     OutBufferHandleB,
                                                        u32                     OutBufferPointerB,
                                                        u32                     OutBufferSize,
                                                        u32                     ResolutionX,
                                                        u32                     ResolutionY,
                                                        EGLDisplay              eglDisplay,
                                                        EGLContext              eglContext )
{
                m_InputBufferHandle         = InBufferHandle;
                m_InputBufferPointer        = InBufferPointer;

                m_OutputBufferHandleA       = OutBufferHandleA;
                m_OutputBufferPointerA      = OutBufferPointerA;

                m_OutputBufferHandleB       = OutBufferHandleB;
                m_OutputBufferPointerB      = OutBufferPointerB;

                m_InputBufferSize           = InBufferSize;
                m_OutputBufferSize          = OutBufferSize;

                m_ResolutionX               = ResolutionX;
                m_ResolutionY               = ResolutionY;

                m_eglDisplay                = eglDisplay;
                m_eglContext                = eglContext;

bool bOK = true;

#ifdef __DEBUG_LOG__                
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif
                getStateVCHI                ( );
                if (bOK)
                    {
                    bOK = initEventsVCOS( m_EventMMAL, "MMAL" );
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMAL initEventsVCOS FAILED");
#endif                   
                    if (!bOK) return false;
                    }

#ifdef __DEBUG_LOG__
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");   
                storeLog ( MY_BUFFER, MY_INDEX, "Input    Buffer Handle",m_InputBufferHandle,   "Pointer", m_InputBufferPointer,    "Size", m_InputBufferSize);
                storeLog ( MY_BUFFER, MY_INDEX, "Output A Buffer Handle",m_OutputBufferHandleA, "Pointer", m_OutputBufferPointerA,  "Size", m_OutputBufferSize);
                storeLog ( MY_BUFFER, MY_INDEX, "Output B Buffer Handle",m_OutputBufferHandleB, "Pointer", m_OutputBufferPointerB,  "Size", m_OutputBufferSize); 
            //  nextline ( MY_BUFFER, MY_INDEX );       
                storeLog ( MY_BUFFER, MY_INDEX, "Resolution      Height", m_ResolutionX, "Width", m_ResolutionY, "EGL Display", (u32)m_eglDisplay, "EGL Context", (u32)m_eglContext);
            //  storeLog ( MY_BUFFER, MY_INDEX, "EGL Display", (u32)m_eglDisplay, "EGL Context", (u32)m_eglContext);
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif 

                if (bOK)
                    {
                    bOK = openServiceVCHI(  m_ServiceCreateMMAL,
                                            VC_MMAL_VER,
                                            VC_MMAL_MIN_VER,
                                            VCHIQ_MAKE_FOURCC('m','m','a','l'),
                                            callbackMMAL,
                                            &m_EventMMAL,
                                            m_VCHIInstance,
                                            m_ServiceHandleMMAL);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMAL openService FAILED");
#endif 
                    if (!bOK) return false;
                    }
                if (bOK)
                    {
                    bOK = createComponent(m_ComponentCreateTx, m_ComponentCreateRx);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALcreateComponent FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_A, m_PortInfoGetRx_Input_A);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Input A FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_A, m_PortInfoGetRx_Output_A);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Output A FAILED");
#endif 
                    }

                primePortFormatInputMMAL (m_PortInfoGetRx_Input_A,  m_PortInfoSetTx_Input);
                primePortFormatOutputMMAL(m_PortInfoGetRx_Output_A, m_PortInfoSetTx_Output); 

                if (bOK)
                    {
                    bOK = setPortInfoMMAL(m_PortInfoSetTx_Input, m_PortInfoSetRx_Input);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetPortInfo Input FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = setPortInfoMMAL(m_PortInfoSetTx_Output, m_PortInfoSetRx_Output);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetPortInfo Output FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = enableComponentMMAL(m_ComponentEnableTx, m_ComponentEnableRx);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALenableComponentMMAL FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_B, m_PortInfoGetRx_Input_B);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Input B FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_B, m_PortInfoGetRx_Output_B);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Output B BFAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = setZeroCopyModeMMAL(m_PortInfoGetRx_Input_B, m_PortParamTx_Input, m_PortParamRx_Input);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetZeroCopyModeMMAL Input FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = setZeroCopyModeMMAL(m_PortInfoGetRx_Output_B, m_PortParamTx_Output, m_PortParamRx_Output);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetZeroCopyModeMMAL Output FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_C, m_PortInfoGetRx_Input_C);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Input C FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_C, m_PortInfoGetRx_Output_C);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Output C FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = enablePortMMAL(m_PortInfoGetRx_Input_C, m_PortActionTx_Input, m_PortActionRx_Input);
#ifdef __DEBUG_LOG__
                if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALenablePort Input FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = enablePortMMAL(m_PortInfoGetRx_Output_C, m_PortActionTx_Output, m_PortActionRx_Output);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALenablePort Output FAILED");
#endif 
                    }
            /*
                queueInputBufferMMAL        ( m_BufferFromHostTx_Input,
                                            m_BufferFromHostRx_Input );
                queueOutputBufferMMAL       ( m_BufferFromHostTx_OutputA,
                                            m_BufferFromHostRx_OutputA );
                queueOutputBufferMMAL       ( m_BufferFromHostTx_OutputB,
                                            m_BufferFromHostRx_OutputB );
            */
#ifdef __DEBUG_LOG__ 
            //  nextline ( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "MMAL Successful Initialized");
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");                
#endif 
                return bOK;
}

bool            CKernel::createTexturesMMAL         (   )
{
                int f_count = 0;

                glGenTextures(1, &m_Texture);
                if(!checkGLerrorMMAL()) f_count++;
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if(!checkGLerrorMMAL()) f_count++;
                glBindTexture(GL_TEXTURE_2D, 0);
                if(!checkGLerrorMMAL()) f_count++;

                if( f_count != 0)
                    { 
#ifdef __DEBUG_LOG__ 
                //  nextline( MY_BUFFER, MY_INDEX );                                 
                    storeLog( MY_BUFFER, MY_INDEX, "Texture Creation FAILED");
#endif                   
                    return false;
                    }
#ifdef __DEBUG_LOG__             
            //  nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "Texture Creation SUCCESS");
#endif               
                return true;
}

bool            CKernel::framePollerMMAL            (   u32 frame_offset, u32 frame_length)
{
#ifdef __DEBUG_LOG__
                if (!f_firstFrameQueued)
                    {
                    if (!queueInputBufferMMAL(m_BufferFromHostTx_Input, frame_offset, frame_length))
                        {
                    //  nextline( MY_BUFFER, MY_INDEX );
                        storeLog( MY_BUFFER, MY_INDEX, "very first frame queue ERROR!", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);
                        return false;
                        }
                //  nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "very first frame queue SUCCESS", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);

                    getPortInfoMMAL(MMAL_PORT_TYPE_INPUT,  m_InputPortHandle,  m_PortInfoGetTx_Input_D, m_PortInfoGetRx_Input_D);
                    getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_D, m_PortInfoGetRx_Output_D);

                    f_firstFrameQueued = true;
                    return true;
                    }
#endif 
                const char*  message = "";
                uint32_t msg_len = 0;


    // Drain RX queue unconditionally (non-blocking)

                while (vchi_msg_dequeue(m_ServiceHandleMMAL, &m_BufferFromHostTx_Output, sizeof(m_BufferFromHostTx_Output), &msg_len, VCHI_FLAGS_NONE) == 0)
                    {
                    switch (m_BufferFromHostTx_Output.hdr.type) // FIRST AXIS: message type (semantic meaning)
                        {
                        case MMAL_MSG_TYPE_BUFFER_TO_HOST:
                            {
                            switch (m_BufferFromHostTx_Output.hdr.status)   // SECOND AXIS: status (outcome)
                                {
                                case MMAL_MSG_STATUS_SUCCESS:
                                    {
                                    u32 m_CurrentHandle = m_BufferFromHostTx_Output.msg.buffer_header.data;  // Payload layout reused: buffer_from_host
#ifdef __DEBUG_LOG__   
                                //  nextline( MY_BUFFER, MY_INDEX );
                                    storeLog( MY_BUFFER, MY_INDEX, "frame offset", frame_offset, "length", frame_length, "status", m_BufferFromHostTx_Output.hdr.status, "handle", m_CurrentHandle);
#endif 
                                    if (m_CurrentHandle != m_OutputBufferHandleA && m_CurrentHandle != m_OutputBufferHandleB)
                                        {
                                        continue; // as long as we really want to drain, break; will only proceed the switch (m_RxMsg.h.status) // unrelated buffer, keep draining
                                        }

                                    if (m_CurrentHandle == m_OutputBufferHandleA) // Ping-pong: requeue the other output buffer
                                        {
                                        if (!queueOutputBufferMMAL(m_BufferFromHostTx_Output, m_OutputBufferHandleB, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (m_CurrentHandle == m_OutputBufferHandleB) 
                                        {
                                        if (!queueOutputBufferMMAL(m_BufferFromHostTx_Output, m_OutputBufferHandleA, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (!bufferReadyMMAL(m_CurrentHandle))
                                        return false;
                                    if (!queueInputBufferMMAL(m_BufferFromHostTx_Input, frame_offset, frame_length))
                                        return false;
#ifdef __DEBUG_LOG__   
                                    message = "MMAL_MSG_STATUS_SUCCESS      - All is Fine";
                                //  nextline( MY_BUFFER, MY_INDEX );
                                    storeLog( MY_BUFFER, MY_INDEX, message, EMPTYLOG, "frame offset", frame_offset, "length", frame_length);
#endif                       
                                    return true;
                                    }
#ifdef __DEBUG_LOG__   
                                case MMAL_MSG_STATUS_ENOMEM:     message = "MMAL_MSG_STATUS_ENOMEM       - Out of memory                      "; break;
                                case MMAL_MSG_STATUS_ENOSPC:     message = "MMAL_MSG_STATUS_ENOSPC       - Out of resources other than memory "; break;
                                case MMAL_MSG_STATUS_EINVAL:     message = "MMAL_MSG_STATUS_EINVAL       - Argument is invalid                "; break;
                                case MMAL_MSG_STATUS_ENOSYS:     message = "MMAL_MSG_STATUS_ENOSYS       - Function not implemented           "; break;
                                case MMAL_MSG_STATUS_ENOENT:     message = "MMAL_MSG_STATUS_ENOENT       - No such file or directory          "; break;
                                case MMAL_MSG_STATUS_ENXIO:      message = "MMAL_MSG_STATUS_ENXIO        - No such device or address          "; break;
                                case MMAL_MSG_STATUS_EIO:        message = "MMAL_MSG_STATUS_EIO          - I/O error                          "; break;
                                case MMAL_MSG_STATUS_ESPIPE:     message = "MMAL_MSG_STATUS_ESPIPE       - Illegal seek                       "; break;
                                case MMAL_MSG_STATUS_ECORRUPT:   message = "MMAL_MSG_STATUS_ECORRUPT     - Data is corrupt                    "; break;
                                case MMAL_MSG_STATUS_ENOTREADY:  message = "MMAL_MSG_STATUS_ENOTREADY    - Component is not ready             "; break;
                                case MMAL_MSG_STATUS_ECONFIG:    message = "MMAL_MSG_STATUS_ECONFIG      - Component is not configured        "; break;
                                case MMAL_MSG_STATUS_EISCONN:    message = "MMAL_MSG_STATUS_EISCONN      - Port is already connected          "; break;
                                case MMAL_MSG_STATUS_ENOTCONN:   message = "MMAL_MSG_STATUS_ENOTCONN     - Port is disconnected               "; break;
                                case MMAL_MSG_STATUS_EAGAIN:     message = "MMAL_MSG_STATUS_EAGAIN       - Resource temporarily unavailable   "; break;
                                case MMAL_MSG_STATUS_EFAULT:     message = "MMAL_MSG_STATUS_EFAULT       - Bad address                        "; break;
#endif                   
                                default:                         message = "Unknown MMAL status          - WTF!!!                             "; break;
                                }
#ifdef __DEBUG_LOG__   
                        //  nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message, EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (BUFFER_TO_HOST)", &m_BufferFromHostTx_Output, msg_len);
#endif               
                            return false;
                            }

                        default: 
                            {
                            message = "UNEXPECTED MESSAGE";
#ifdef __DEBUG_LOG__    
                        //  nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message,  EMPTYLOG, "Frame offset", frame_offset, "Type",  m_BufferFromHostTx_Output.hdr.type, "Status", m_BufferFromHostTx_Output.hdr.status);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (UNEXPECTED MESSAGE)", &m_BufferFromHostTx_Output, msg_len);
#endif 
                            break;
                            }
                        }
#ifdef __DEBUG_LOG__ 
                //  nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "Unexpected Reply", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);
                    storeMsg( MY_BUFFER, MY_INDEX, "Unexpected Reply", &m_BufferFromHostTx_Output, msg_len);
#endif       
                    }
#ifdef __DEBUG_LOG__   
            //  nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "Nothing in the Pipeline", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);    // Nothing relevant received
#endif 
                return true;
}

// a-sync VCHI messages ?

bool            CKernel::bufferReadyMMAL            (   u32 handle)
{
        //      if (handle != m_VCSMHandleA && handle != m_VCSMHandleB)         // Only react to our two output buffers
        //          return true;                                                // why???? we have filtered before ad infiniti 
                if (m_EGLimage != EGL_NO_IMAGE_KHR)                             // Destroy previous EGLImage (if any)
                    {
                    eglDestroyImageKHR(m_eglDisplay, m_EGLimage);
                    m_EGLimage = EGL_NO_IMAGE_KHR;
                    }
                egl_image_brcm_vcsm_info info =                                 // Describe the finished VCSM buffer
                    {
                    .width       = m_ResolutionX,
                    .height      = m_ResolutionY,
                    .vcsm_handle = handle
                    };
                m_EGLimage = eglCreateImageKHR( m_eglDisplay, m_eglContext, EGL_IMAGE_BRCM_VCSM, (EGLClientBuffer)&info, nullptr ); // Create new EGLImage viewing this buffer
                if (m_EGLimage == EGL_NO_IMAGE_KHR)
                    {
#ifdef __DEBUG_LOG__        
                    storeLog( MY_BUFFER, MY_INDEX, "EGLImage creation FAILED", handle);
#endif 
                    return false;
                    }
                glBindTexture(GL_TEXTURE_2D, m_Texture);                    // Bind the EGLImage to the single public texture
                glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimage);
                glBindTexture(GL_TEXTURE_2D, 0);

                return true;
}

bool            CKernel::queueOutputBufferMMAL      (   MMAL_Buffer_From_Host_Msg& tx, 
                                                        u32 vc_handle, 
                                                        u32 alloc_size)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;  /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.data                          = vc_handle;
                tx.msg.buffer_header.alloc_size                    = alloc_size;  // why again?
                tx.msg.buffer_header.length                        = 0;
                tx.msg.buffer_header.offset                        = 0;
                tx.msg.buffer_header.flags                         = 0;

#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueOutputBuffer", &tx, (u32)sizeof(tx));
#endif 
                return (vchi_msg_queue(m_ServiceHandleMMAL, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}

bool            CKernel::queueInputBufferMMAL       (   MMAL_Buffer_From_Host_Msg& tx, 
                                                        u32 frame_offset, 
                                                        u32 frame_length)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;                /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.offset                        = frame_offset;
                tx.msg.buffer_header.length                        = frame_length;

                u32 flags                                           = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;
                tx.msg.buffer_header.flags                         = flags;
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueInputBuffer", &tx, (u32)sizeof(tx));
#endif 
                return (vchi_msg_queue(m_ServiceHandleMMAL, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}

bool            CKernel::createComponent            (   MMAL_Component_Create_Msg& tx, 
                                                        MMAL_Component_Create_Reply& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_COMPONENT_CREATE );

                tx.msg                                             = {};   /* prime TX msg */

                memset(tx.msg.name, 0, sizeof(tx.msg.name));
                strncpy(tx.msg.name, "ril.video_decode", sizeof(tx.msg.name) - 1);
                        
                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __DEBUG_LOG__
                Log_createComponent(tx,rx);
#endif       
                m_ComponentHandle = rx.msg.component_handle;

                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::getPortInfoMMAL            (   u32 port_type, 
                                                        u32& port_handle, 
                                                        MMAL_Port_Info_Get_Msg& tx, 
                                                        MMAL_Port_Info_Get_Reply& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_GET );

                tx.msg                                             = {};       /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_type                                   = port_type;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
                port_handle = rx.msg.port_handle;
#ifdef __DEBUG_LOG__
                Log_getPortInfo(tx,rx);
#endif 
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::setPortInfoMMAL            (   MMAL_Port_Info_Set_Msg& tx, 
                                                        MMAL_Port_Info_Set_Reply& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_SET );

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __DEBUG_LOG__
                Log_setPortInfo(tx,rx); 
#endif               
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::enableComponentMMAL        (   MMAL_Component_Enable_Msg& tx, 
                                                        MMAL_Component_Enable_Reply& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_COMPONENT_ENABLE );

                tx.msg                                             = {};
                tx.msg.component_handle                            = m_ComponentHandle;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __DEBUG_LOG__
                Log_enableComponentMMAL(tx,rx);
#endif       
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::setZeroCopyModeMMAL        ( /*u32 port_handle,*/ 
                                                        const MMAL_Port_Info_Get_Reply& src,  
                                                        MMAL_Port_Parameter_Set_Msg& tx, 
                                                        MMAL_Port_Parameter_Set_Reply& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_PARAMETER_SET );

                tx.msg = {};
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = src.msg.port_handle; // port_handle - my original code takes it as parameter! i assume this is chosen because chn has the correct handle
                tx.msg.id                                          = MMAL_PARAMETER_ZERO_COPY;
                tx.msg.size                                        = sizeof(u32);

                memset(tx.msg.value, 0, sizeof(tx.msg.value));
                tx.msg.value[0] = 1;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __DEBUG_LOG__
                Log_setZeroCopyModeMMAL(tx,rx);
#endif       
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::enablePortMMAL             ( /*u32 port_handle,*/ 
                                                        const MMAL_Port_Info_Get_Reply& src, 
                                                        MMAL_Port_Action_Msg& tx, 
                                                        MMAL_Port_Action_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_ACTION );

                tx.msg                                             = {};
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = src.msg.port_handle;                // port_handle - my original code takes it as parameter! i assume this is chosen because chn has the correct handle
                tx.msg.action                                      = MMAL_MSG_PORT_ACTION_TYPE_ENABLE;
                tx.msg.port                                        = src.msg.port;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __DEBUG_LOG__
                Log_enablePort(tx,rx);
#endif       
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

void            CKernel::PrimeOutputBufferBodyMMAL  (   MMAL_Buffer_From_Host_Msg& tx)
{
                tx.msg = {};
                tx.msg.drvbuf.magic                                = MMAL_MAGIC;
                tx.msg.drvbuf.component_handle                     = m_ComponentHandle;
                tx.msg.drvbuf.port_handle                          = m_OutputPortHandle;

                tx.msg.buffer_header.pts_lo                        = 0;
                tx.msg.buffer_header.pts_hi                        = 0x80000000;
                tx.msg.buffer_header.dts_lo                        = 0;
                tx.msg.buffer_header.dts_hi                        = 0x80000000;

                tx.msg.is_zero_copy                                = 1;
                tx.msg.has_reference                               = 0;
                tx.msg.payload_in_message                          = 0;
}

void            CKernel::PrimeInputBufferBodyMMAL   (   MMAL_Buffer_From_Host_Msg& tx)
{
                tx.msg = {};
                tx.msg.drvbuf.magic                                = MMAL_MAGIC;
                tx.msg.drvbuf.component_handle                     = m_ComponentHandle;
                tx.msg.drvbuf.port_handle                          = m_InputPortHandle;

                tx.msg.buffer_header.data                          = m_InputBufferHandle;
                tx.msg.buffer_header.alloc_size                    = m_InputBufferSize;

                tx.msg.buffer_header.pts_lo                        = 0;
                tx.msg.buffer_header.pts_hi                        = 0x80000000;
                tx.msg.buffer_header.dts_lo                        = 0;
                tx.msg.buffer_header.dts_hi                        = 0x80000000;

                tx.msg.is_zero_copy                                = 1;
                tx.msg.has_reference                               = 0;
                tx.msg.payload_in_message                          = 0;
}

void            CKernel::primePortFormatInputMMAL   (   const MMAL_Port_Info_Get_Reply& src, 
                                                        MMAL_Port_Info_Set_Msg& tx)
{
                tx.msg.component_handle = src.msg.component_handle;
                tx.msg.port_type        = src.msg.port_type;
                tx.msg.port_index       = src.msg.port_index;

                tx.msg.port   = src.msg.port;
                tx.msg.format = src.msg.format;
                tx.msg.es     = src.msg.es;

                tx.msg.port.buffer_num  = NUMBER_INPUTBUFFER;
                tx.msg.port.buffer_size = m_InputBufferSize;

                tx.msg.format.encoding         = MMAL_ENCODING_H264;
                tx.msg.format.encoding_variant = MMAL_ENCODING_VARIANT_H264_DEFAULT;

                tx.msg.es.video.width       = m_ResolutionX;
                tx.msg.es.video.height      = m_ResolutionY;
                tx.msg.es.video.crop.x      = 0;
                tx.msg.es.video.crop.y      = 0;
                tx.msg.es.video.crop.width  = m_ResolutionX;
                tx.msg.es.video.crop.height = m_ResolutionY;
}

void            CKernel::primePortFormatOutputMMAL  (   const MMAL_Port_Info_Get_Reply& src, 
                                                        MMAL_Port_Info_Set_Msg& tx)
{
                tx.msg.component_handle = src.msg.component_handle;
                tx.msg.port_type        = src.msg.port_type;
                tx.msg.port_index       = src.msg.port_index;

                tx.msg.port   = src.msg.port;
                tx.msg.format = src.msg.format;
                tx.msg.es     = src.msg.es;

                tx.msg.port.buffer_num  = NUMBER_OUTPUTBUFFER;
                tx.msg.port.buffer_size = m_OutputBufferSize;

                tx.msg.format.encoding = MMAL_ENCODING_I420;

                tx.msg.es.video.width       = m_ResolutionX;
                tx.msg.es.video.height      = m_ResolutionY;
                tx.msg.es.video.crop.x      = 0;
                tx.msg.es.video.crop.y      = 0;
                tx.msg.es.video.crop.width  = m_ResolutionX;
                tx.msg.es.video.crop.height = m_ResolutionY;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// end
//----------------------------------------------------------------------------------------------------------------------------------------------------
#undef __DEBUG_LOG__