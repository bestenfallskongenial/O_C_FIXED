#include "h264_decoder.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------
// h264_hardware_decoder.cpp
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
void            CH264Decoder::initHeader            (   mmal_msg_header& hdr, u32 type)
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
                nextline();
                storeLog("TX MSG", (u32)msg_size);
                storeMsg( "Raw TX", msg, msg_size);
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
                        nextline();    
                        storeLog("RX MSG", ReplyLength);
                        storeMsg("Raw RX", rx_msg, ReplyLength);
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
                    const mmal_msg_header* h = (const mmal_msg_header*)msg;
                    nextline();
                    storeLog("MMALsendAndWait ANSWER TO SHORT - MSG #", h->context );
#endif                                    
                    return false;
                    }
                return true;                                    //  return (ReplyLength != 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------