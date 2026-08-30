//----------------------------------------------------------------------------------------------------------------------------------------------------
// h264_hardware_decoder.cpp
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CH264Decoder::MMALinitialize              ( u32         InBufferHandle,         // my input buffer handle from smem
                                                            u32         InBufferPointer,        // i got the feeling i rather need this
                                                            u32         InBufferSize,           // my allocated input buffer size 
                                                            u32         OutBufferHandleA,       // my output buffer handle a from smem
                                                            u32         OutBufferPointerA,      // i got the feeling i rather need this
                                                            u32         OutBufferHandleB,       // my output buffer handle b from smem
                                                            u32         OutBufferPointerB,      // i got the feeling i rather need this
                                                            u32         OutBufferSize,          // my allocated output buffer size
                                                            u32         ResolutionX,            // 
                                                            u32         ResolutionY,            //
                                                            EGLDisplay  eglDisplay,             // EGL display connection
                                                            EGLContext  eglContext )            // EGL rendering context
{
                /* store parameters */                                  // from the 
                m_InputBufferHandle         = InBufferHandle;                 //
                m_InputBufferPointer        = InBufferPointer;

                m_OutputBufferHandleA       = OutBufferHandleA;
                m_OutputBufferPointerA      = OutBufferPointerA;

                m_OutputBufferPointerB      = OutBufferHandleB;
                m_OutputBufferPointerB      = OutBufferPointerB;

                m_InputBufferSize           = InBufferSize;
                m_OutputBufferSize          = OutBufferSize;

                m_ResolutionX               = ResolutionX;
                m_ResolutionY               = ResolutionY;

                m_eglDisplay                = eglDisplay;
                m_eglContext                = eglContext;

                getVCHIstate                ( );
                initEvents                  ( );
                
bool bOK = true;

    if (bOK)
        {
    bOK = openService(m_ServiceCreate);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMAL openService FAILED");
#endif
        }
    if (bOK)
        {
    bOK = createComponent(m_ComponentCreateTx, m_ComponentCreateRx);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALcreateComponent FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_A, m_PortInfoGetRx_Input_A);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALgetPortInfo Input A FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_A, m_PortInfoGetRx_Output_A);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALgetPortInfo Output A FAILED");
#endif
        }
    if (bOK)
        {
        setPortFormatInput (m_PortInfoGetRx_Input_A,  m_PortInfoSetTx_Input);
        setPortFormatOutput(m_PortInfoGetRx_Output_A, m_PortInfoSetTx_Output);
        }
    if (bOK)
        {
        bOK = setPortInfo(m_PortInfoSetTx_Input, m_PortInfoSetRx_Input);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALsetPortInfo Input FAILED");
#endif
        }
    if (bOK)
        {
        bOK = setPortInfo(m_PortInfoSetTx_Output, m_PortInfoSetRx_Output);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALsetPortInfo Output FAILED");
#endif
        }
    if (bOK)
        {
        bOK = enableComponent(m_ComponentEnableTx, m_ComponentEnableRx);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALenableComponent FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_B, m_PortInfoGetRx_Input_B);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALgetPortInfo Input B FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_B, m_PortInfoGetRx_Output_B);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALgetPortInfo Output B BFAILED");
#endif
        }
    if (bOK)
        {
        bOK = setZeroCopyMode(m_PortInfoGetRx_Input_B, m_PortParamTx_Input, m_PortParamRx_Input);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALsetZeroCopyMode Input FAILED");
#endif
        }
    if (bOK)
        {
        bOK = setZeroCopyMode(m_PortInfoGetRx_Output_B, m_PortParamTx_Output, m_PortParamRx_Output);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALsetZeroCopyMode Output FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_C, m_PortInfoGetRx_Input_C);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALgetPortInfo Input C FAILED");
#endif
        }
if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_C, m_PortInfoGetRx_Output_C);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALgetPortInfo Output C FAILED");
#endif
        }
    if (bOK)
        {
        bOK = enablePort(m_PortInfoGetRx_Input_C, m_PortActionTx_Input, m_PortActionRx_Input);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALenablePort Input FAILED");
#endif
        }
    if (bOK)
        {
        bOK = enablePort(m_PortInfoGetRx_Output_C, m_PortActionTx_Output, m_PortActionRx_Output);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog("\nMMALenablePort Output FAILED");
#endif
        }
    return bOK;

            /*
                queueInputBuffer        ( m_BufferFromHostTx_Input,         // ---------- BUFFERS ---------- 
                                            m_BufferFromHostRx_Input );

                queueOutputBuffer       ( m_BufferFromHostTx_OutputA,
                                            m_BufferFromHostRx_OutputA );

                queueOutputBuffer       ( m_BufferFromHostTx_OutputB,
                                            m_BufferFromHostRx_OutputB );
            */
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::createTextures       (   )
{
                int count = 0;

                glGenTextures(1, &m_Texture);
                if(!checkGLerror()) count++;
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if(!checkGLerror()) count++;
                glBindTexture(GL_TEXTURE_2D, 0);
                if(!checkGLerror()) count++;

                if( count != 0)
                    { 
#ifdef __H264_DECODER_DEBUG_INIT__                                  
                    storeLog("\nTexture A Creation FAILED");
#endif                    
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__                              
                storeLog("\nTexture Creation SUCCESS");
#endif                
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::FramePoller(u32 frame_offset, u32 frame_length)
{
#ifdef __H264_DECODER_DEBUG_RUNTIME__           // Bootstrap: prime first input buffer and snapshot port state (debug)
                if (!m_FirstFrameQueued)
                    {
                    if (!queueInputBuffer(mBodyIn, frame_offset, frame_length))
                        {
                        
                        storeLog("very first frame queue error!", frame_offset, frame_length);
                        return false;
                        }

                    storeLog("very first frame queue SUCCESS", frame_offset, frame_length);

                    getPortInfo(MMAL_PORT_TYPE_INPUT,  m_InputPortHandle,  m_InputPortInfoReply);
                    getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_OutputPortInfoReply);

                    m_FirstFrameQueued = true;
                    return true;
                    }
#endif
                CString  message = "";
                uint32_t msg_len = 0;

    // ---------------------------------------------------------------------
    // Drain RX queue unconditionally (non-blocking)
    // ---------------------------------------------------------------------
                while (vchi_msg_dequeue(m_ServiceHandle, &m_RxMsg, sizeof(m_RxMsg), &msg_len, VCHI_FLAGS_NONE) == 0)
                    {
        
                    switch (m_RxMsg.h.type) // FIRST AXIS: message type (semantic meaning)
                        {
                        case MMAL_MSG_TYPE_BUFFER_TO_HOST:
                            {
                            switch (m_RxMsg.h.status)   // SECOND AXIS: status (outcome)
                                {
                                case MMAL_MSG_STATUS_SUCCESS:
                                    {
                                    u32 ready_vcsm_handle = m_RxMsg.u.buffer_from_host.buffer_header.data;  // Payload layout reused: buffer_from_host
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                                    storeLog("offset / length / status / data", frame_offset, frame_length, m_RxMsg.h.status, ready_vcsm_handle);
#endif
                                    if (ready_vcsm_handle != m_VCSMHandleA && ready_vcsm_handle != m_VCSMHandleB)
                                        {
                                        continue; // as long as we really want to drain, break; will only proceed the switch (m_RxMsg.h.status) // unrelated buffer, keep draining
                                        }

                                    if (ready_vcsm_handle == m_VCSMHandleA) // Ping-pong: requeue the other output buffer
                                        {
                                        if (!queueOutputBuffer(mBodyOut, m_OutputBufferHandleB, m_OutputBufferSize))
                                            return false;
                                        }
                                    else
                                        {
                                        if (!queueOutputBuffer(mBodyOut, m_OutputBufferHandleA, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (!bufferReady(ready_vcsm_handle))
                                        return false;
                                    if (!queueInputBuffer(mBodyIn, frame_offset, frame_length))
                                        return false;
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                                    message = "MMAL_MSG_STATUS_SUCCESS      - All is Fine";
                                    storeLog(message, frame_offset, frame_length);
#endif                        
                                    return true;
                                    }
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
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
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                            storeLog(message, frame_offset, frame_length);
                            storeMsg(&m_RxMsg, msg_len, "Poller ERROR (BUFFER_TO_HOST)");
#endif                
                            return false;
                            }

                        default: 
                            {
                            message = "UNEXPECTED MESSAGE";
#ifdef __H264_DECODER_DEBUG_RUNTIME__                   
                            storeLog(message, frame_offset, frame_length);
                            storeMsg(&m_RxMsg, msg_len, "Poller ERROR (UNEXPECTED MESSAGE)");
#endif
                            break;
                            }
                        }
                    }
#ifdef __H264_DECODER_DEBUG_RUNTIME__                   
                storeLog("Nothing in the Pipeline", frame_offset, frame_length);    // Nothing relevant received
#endif
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::callBack              (   void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle )
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {  
                    vcos_event_signal(event); 
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::initHeader            (   MMAL_Port_Info_Set_Msg& hdr, u32 type)
{
    hdr                 = {};
    hdr.magic           = MMAL_MAGIC;
    hdr.type            = type;
    hdr.control_service = 0;
    hdr.context         = NextTransId(m_TransactionId);
    hdr.status          = 0;
    hdr.padding         = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CH264Decoder::NextTransId               (   u32 &tid )
{
                tid = ( tid+1 ) & ~0x80000000u;                                             // mask for async messages really needed ?!                        
                return tid;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::getVCHIstate              (   )
{
                vc_host_get_vchi_state(&m_VCHIInstance, &m_Connection);                         //1. get the VCHI instance and the connection handle from bcm_host.h
#ifdef __H264_DECODER_DEBUG_INIT__
                storeLog ( "\nVCHI State Instance & Connection  ", (u32)m_VCHIInstance, (u32)m_Connection);                 
#endif
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::initEvents            (   )
{
                if (vcos_event_create(&m_VCOSevent, "MMAL") != VCOS_SUCCESS)
                    {
#ifdef __H264_DECODER_DEBUG_INIT__ 
                    storeLog ( "\nVCOS Event Init FAILED!   ");                        
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__ 
                storeLog ( "\nVCOS Event Init SUCCESS!  ", (u32)&m_VCOSevent);
#endif                
                return true;    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::checkGLerror                  (   )
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
#ifdef __H264_DECODER_DEBUG_INIT__                        
                    storeLog (error_str);
#endif                         
                    return false;
                    }
                return true;    
}

//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::sendAndWait           (   const void *msg, size_t msg_size, void *rx_msg, size_t max_reply_len, size_t *actual_reply_len )
{
#ifdef __H264_DECODER_DEBUG_INIT__
                storeLog("\nTX MSG", (u32)msg_size);
                storeMsg(msg, msg_size, "Raw TX");
#endif

                if (vchi_msg_queue(m_ServiceHandle, msg, msg_size, VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL) != 0)
                    {
                    return false;
                    }
                uint32_t ReplyLength = 0;
                do {
                    if (vchi_msg_dequeue(m_ServiceHandle, rx_msg, max_reply_len, &ReplyLength, VCHI_FLAGS_NONE) == 0)
                        {
#ifdef __H264_DECODER_DEBUG_INIT__
                        storeLog("\nRX MSG", ReplyLength);
                        storeMsg(rx_msg, ReplyLength, "Raw RX");
#endif
                        break;
                        }
                    } 
                while (vcos_event_wait(&m_VCOSevent) == VCOS_SUCCESS);

                if (actual_reply_len)
                    {
                    *actual_reply_len = ReplyLength;
                    }
                if (ReplyLength != max_reply_len)               /* enforce completeness HERE */
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
#endif                                    
                    return false;
                    }
                return true;        

            //  return (ReplyLength != 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::openService           (   SERVICE_CREATION_T &tx)
{
                tx.version.version                                  = VC_MMAL_VER;
                tx.version.version_min                              = VC_MMAL_MIN_VER;
                tx.service_id                                       = VCHIQ_MAKE_FOURCC('m','m','a','l');
                tx.connection                                       = m_Connection;
                tx.rx_fifo_size                                     = 0;
                tx.tx_fifo_size                                     = 0;
                tx.callback                                         = callBack;
                tx.callback_param                                   = &m_VCOSevent;
                tx.want_unaligned_bulk_rx                           = 0;
                tx.want_unaligned_bulk_tx                           = 0;
                tx.want_crc                                         = 0;

                int rc = vchi_service_open( m_VCHIInstance, &tx, &m_ServiceHandle );    
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_openService(tx);
#endif
                return (rc == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::createComponent       (   MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_COMPONENT_CREATE );

                tx.body                                             = {};   /* prime TX body */
                tx.body.client_component                            = 0;

                memset(tx.body.name, 0, sizeof(tx.body.name));
                strncpy(tx.body.name, "ril.video_decode", sizeof(tx.body.name) - 1);
                        
                tx.body.pid                                         = 0;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_createComponent(rx);
#endif        
                m_ComponentHandle = rx.body.component_handle;

                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::getPortInfo           (   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_GET );

                tx.body                                             = {};       /* prime TX body */
                tx.body.component_handle                            = m_ComponentHandle;
                tx.body.port_type                                   = port_type;
                tx.body.index                                       = 0;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
                port_handle = rx.body.port_handle;
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_getPortInfo(rx);
#endif
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::setPortInfo           (   MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_SET );

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_setPortInfo(rx); 
#endif                
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::enableComponent       (   MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_COMPONENT_ENABLE );

                tx.body                                             = {};                                                   /* prime TX body */
                tx.body.component_handle                            = m_ComponentHandle;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_enableComponent(rx);
#endif        
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::setZeroCopyMode       (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src,  MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_PARAMETER_SET );

                tx.body = {};                                                       /* prime TX body */
                tx.body.component_handle                            = m_ComponentHandle;
                tx.body.port_handle                                 = src.body.port_handle; // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.body.id                                          = MMAL_PARAMETER_ZERO_COPY;
                tx.body.size                                        = sizeof(u32);

                memset(tx.body.value, 0, sizeof(tx.body.value));
                tx.body.value[0] = 1;   /* enable zero-copy */

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_setZeroCopyMode(rx);
#endif        
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::enablePort            (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_ACTION );

                tx.body                                             = {};                                                   /* prime TX body from GET snapshot */
                tx.body.component_handle                            = m_ComponentHandle;
                tx.body.port_handle                                 = src.body.port_handle;                // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.body.action                                      = MMAL_MSG_PORT_ACTION_TYPE_ENABLE;
            //  tx.body.port                                        = src.body.port;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_enablePort(rx);
#endif        
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::PrimeOutputBufferBody     (   MMAL_Buffer_From_Host_Msg& tx)
{
                tx.body = {};
                tx.body.drvbuf.magic                                = MMAL_MAGIC;
                tx.body.drvbuf.component_handle                     = m_ComponentHandle;
                tx.body.drvbuf.port_handle                          = m_OutputPortHandle;

                tx.body.buffer_header.pts_lo                        = 0;
                tx.body.buffer_header.pts_hi                        = 0x80000000;
                tx.body.buffer_header.dts_lo                        = 0;
                tx.body.buffer_header.dts_hi                        = 0x80000000;

                tx.body.is_zero_copy                                = 1;
                tx.body.has_reference                               = 0;
                tx.body.payload_in_message                          = 0;

#ifdef __H264_DECODER_DEBUG_INIT__
//             Log_createComponent(tx);
#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::PrimeInputBufferBody      (   MMAL_Buffer_From_Host_Msg& tx)
{
                tx.body = {};
                tx.body.drvbuf.magic                                = MMAL_MAGIC;
                tx.body.drvbuf.component_handle                     = m_ComponentHandle;
                tx.body.drvbuf.port_handle                          = m_InputPortHandle;

                tx.body.buffer_header.data                          = m_InputBufferHandle;
                tx.body.buffer_header.alloc_size                    = m_InputBufferSize;

                tx.body.buffer_header.pts_lo                        = 0;
                tx.body.buffer_header.pts_hi                        = 0x80000000;
                tx.body.buffer_header.dts_lo                        = 0;
                tx.body.buffer_header.dts_hi                        = 0x80000000;

                tx.body.is_zero_copy                                = 1;
                tx.body.has_reference                               = 0;
                tx.body.payload_in_message                          = 0;

#ifdef __H264_DECODER_DEBUG_INIT__
//               Log_createComponent(tx);
#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::queueOutputBuffer     (   MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.body.drvbuf.client_context                       = tx.hdr.context;  /* patch dynamic fields into already-primed body */

                tx.body.buffer_header.data                          = vc_handle;
                tx.body.buffer_header.alloc_size                    = alloc_size;
                tx.body.buffer_header.length                        = 0;
                tx.body.buffer_header.offset                        = 0;
                tx.body.buffer_header.flags                         = 0;

#ifdef __H264_DECODER_DEBUG_RUNTIME__
                storeLog("\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg(&tx, (u32)sizeof(tx), "QueueOutputBuffer");
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::queueInputBuffer      (   MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.body.drvbuf.client_context                       = tx.hdr.context;                /* patch dynamic fields into already-primed body */

                tx.body.buffer_header.offset                        = frame_offset;
                tx.body.buffer_header.length                        = frame_length;

                u32 flags                                           = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;
                tx.body.buffer_header.flags                         = flags;
#ifdef __H264_DECODER_DEBUG_RUNTIME__
                storeLog("\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg(&tx, (u32)sizeof(tx), "QueueInputBuffer");
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CH264Decoder::bufferReady(u32 handle)
{
    
    if (handle != m_VCSMHandleA && handle != m_VCSMHandleB)         // Only react to our two output buffers
        return true;
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
#ifdef __H264_DECODER_DEBUG_RUNTIME__        
        storeLog("\nEGLImage creation FAILED", handle);
#endif
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, m_Texture);                    // Bind the EGLImage to the single public texture
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimage);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

