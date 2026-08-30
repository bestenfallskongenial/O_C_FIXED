#include "h264_decoder.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CONSTRUCTOR / DECONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------------------------------------
                CH264Decoder::CH264Decoder              (   )
                    : m_ServiceHandle(0)
                    , m_TransactionId(0)
                    {
                    }
                CH264Decoder::~CH264Decoder             (   )
{
                if (m_ServiceHandle)
                    {
                    vchi_service_close(m_ServiceHandle);
                    }
}

bool            CH264Decoder::initialize                  ( u32         InBufferHandle,         // my input buffer handle from smem
                                                            u32         InBufferPointer,        // i got the feeling i rather need this
                                                            u32         InBufferSize,           // my allocated input buffer size 
                                                            u32         OutBufferHandleA,       // my output buffer handle a from smem
                                                            u32         OutBufferPointerA,      // i got the feeling i rather need this
                                                            u32         OutBufferHandleB,       // my output buffer handle b from smem
                                                            u32         OutBufferPointerB,      // i got the feeling i rather need this
                                                            u32         OutBufferSize,          // my allocated output buffer size
                                                            u32         ResolutionX,            // obvious, right?
                                                            u32         ResolutionY,            // obvious, right?
                                                            EGLDisplay  eglDisplay,             // EGL display connection
                                                            EGLContext  eglContext )            // EGL rendering context
{
                /* store parameters */                                  // from the 
                m_InputBufferHandle         = InBufferHandle;                 //
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
#ifdef __H264_DECODER_DEBUG_INIT__                
                storeLog ( "----------------------------------------------------------------");
#endif
                getVCHIstate                ( );
#ifdef __H264_DECODER_DEBUG_INIT__
                nextline();
#endif                
                initEvents                  ( );
#ifdef __H264_DECODER_DEBUG_INIT__
                storeLog ( "----------------------------------------------------------------");   
                storeLog ( "Input Port      Handle / Size ",m_InputBufferHandle, m_InputBufferPointer, m_InputBufferSize);
                storeLog ( "Output A Port   Handle / Size ",m_OutputBufferHandleA, m_OutputBufferPointerA, m_OutputBufferSize);
                storeLog ( "Output B Port   Handle / Size ",m_OutputBufferHandleB, m_OutputBufferPointerB, m_OutputBufferSize); 
                nextline();       
                storeLog ( "Resolution      Height / Width",m_ResolutionX, m_ResolutionY);
                storeLog ( "EGL Display / Contex          ", (u32)m_eglDisplay, (u32)m_eglContext);
                storeLog ( "----------------------------------------------------------------");
#endif

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
                    nextline();                                 
                    storeLog("\nTexture A Creation FAILED");
#endif                    
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__             
                nextline();
                storeLog("\nTexture Creation SUCCESS");
#endif                
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::framePoller(u32 frame_offset, u32 frame_length)
{
#ifdef __H264_DECODER_DEBUG_RUNTIME__           // Bootstrap: prime first input buffer and snapshot port state (debug)
                if (!m_FirstFrameQueued)
                    {
                    if (!queueInputBuffer(m_BufferFromHostTx_Input, frame_offset, frame_length))
                        {
                        nextline();
                        storeLog("very first frame queue error!", frame_offset, frame_length);
                        return false;
                        }
                    nextline();
                    storeLog("very first frame queue SUCCESS", frame_offset, frame_length);

                    getPortInfo(MMAL_PORT_TYPE_INPUT,  m_InputPortHandle,  m_PortInfoGetTx_Input_D, m_PortInfoGetRx_Input_D);
                    getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_D, m_PortInfoGetRx_Output_D);

                    m_FirstFrameQueued = true;
                    return true;
                    }
#endif
                const char*  message = "";
                uint32_t msg_len = 0;

    // ---------------------------------------------------------------------
    // Drain RX queue unconditionally (non-blocking)
    // ---------------------------------------------------------------------
                while (vchi_msg_dequeue(m_ServiceHandle, &m_BufferFromHostTx_Output, sizeof(m_BufferFromHostTx_Output), &msg_len, VCHI_FLAGS_NONE) == 0)
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
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                                    nextline();
                                    storeLog("offset / length / status / data", frame_offset, frame_length, m_BufferFromHostTx_Output.hdr.status, m_CurrentHandle);
#endif
                                    if (m_CurrentHandle != m_OutputBufferHandleA && m_CurrentHandle != m_OutputBufferHandleB)
                                        {
                                        continue; // as long as we really want to drain, break; will only proceed the switch (m_RxMsg.h.status) // unrelated buffer, keep draining
                                        }

                                    if (m_CurrentHandle == m_OutputBufferHandleA) // Ping-pong: requeue the other output buffer
                                        {
                                        if (!queueOutputBuffer(m_BufferFromHostTx_Output, m_OutputBufferHandleB, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (m_CurrentHandle == m_OutputBufferHandleB) 
                                        {
                                        if (!queueOutputBuffer(m_BufferFromHostTx_Output, m_OutputBufferHandleA, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (!bufferReady(m_CurrentHandle))
                                        return false;
                                    if (!queueInputBuffer(m_BufferFromHostTx_Input, frame_offset, frame_length))
                                        return false;
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                                    message = "MMAL_MSG_STATUS_SUCCESS      - All is Fine";
                                    nextline();
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
                            nextline();
                            storeLog(message, frame_offset, frame_length);
                            storeMsg("Poller ERROR (BUFFER_TO_HOST)", &m_BufferFromHostTx_Output, msg_len);
#endif                
                            return false;
                            }

                        default: 
                            {
                            message = "UNEXPECTED MESSAGE";
#ifdef __H264_DECODER_DEBUG_RUNTIME__    
                            nextline();
                            storeLog(message, frame_offset, frame_length,m_BufferFromHostTx_Output.hdr.type, m_BufferFromHostTx_Output.hdr.status);
                            storeMsg("Poller ERROR (UNEXPECTED MESSAGE)", &m_BufferFromHostTx_Output, msg_len);
#endif
                            break;
                            }
                        }
#ifdef __H264_DECODER_DEBUG_RUNTIME__ 
                    nextline();
                    storeLog("Unexpected Reply", frame_offset, frame_length);
                    storeMsg("Unexpected Reply", &m_BufferFromHostTx_Output, msg_len);
#endif        
                    }
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                nextline();
                storeLog("Nothing in the Pipeline", frame_offset, frame_length);    // Nothing relevant received
#endif
                return true;
}