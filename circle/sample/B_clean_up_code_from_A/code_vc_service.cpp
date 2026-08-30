#include "kernel.h"

    #define MY_BFR   m_logBuffer                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logBufferIndex

void            CKernel::callbackVCSM               (   void*                   callback_param,         // static ??
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
#ifdef __LOG_VC04__
                storeLogHex (  MY_BFR, MY_IDX, 
                            "VCHI State Instance",      (u32)m_VCHIInstance, 
                            "VCHI State Connection",    (u32)m_Connection);   
#endif             
}

bool            CKernel::initEventsVCOS             (   VCOS_EVENT_T&           event, 
                                                        const char*             name)
{
                if (vcos_event_create(&event, name) != VCOS_SUCCESS)
                    {
#ifdef __LOG_VC04__
                    storeLogHex (  MY_BFR, MY_IDX, 
                                "VCOS Event Init Failed!",  (u32)&event);  
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
#ifdef __LOG_VC04__
                nextline(   MY_BFR, MY_IDX );
                storeLogHex (  MY_BFR, MY_IDX, 
                            "Buffer BUS",       (u32)p_busAddress, 
                            "Buffer ARM",       (u32)bus_addr, 
                            "Buffer VPU",       (u32)vcsm_addr); 
                nextline(   MY_BFR, MY_IDX );                
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
#ifdef __LOG_VC04__  
                    storeLogHex (  MY_BFR, MY_IDX, 
                                error_str);
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
{
                hdr                 = {};
                hdr.type            = type;
                hdr.trans_id        = NextTransId(m_TransactionId);
}

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

bool            CKernel::sendAndWaitVCHI            (           VCHI_SERVICE_HANDLE_T   ServiceHandle, 
                                                                VCOS_EVENT_T&           VCOSevent, 
                                                        const   void*                   msg, 
                                                                size_t                  msg_size, 
                                                                void*                   rx_msg, 
                                                                size_t                  max_reply_len, 
                                                                size_t*                 actual_reply_len )
{
#ifdef __DUMP_VC04__
                storeLogHex(   MY_BFR, MY_IDX, 
                            "TX MSG",           (u32)msg_size);
                storeMsg(   MY_BFR, MY_IDX, 
                            "Raw TX",           msg, 
                            msg_size);
#endif 

                if (vchi_msg_queue(ServiceHandle, msg, msg_size, VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL) != 0)
                    {
                    return false;
                    }
                uint32_t ReplyLength = 0;
                do {
                    if (vchi_msg_dequeue(ServiceHandle, rx_msg, max_reply_len, &ReplyLength, VCHI_FLAGS_NONE) == 0)
                        {
#ifdef __DUMP_VC04__
                        storeLogHex(   MY_BFR, MY_IDX, 
                                    "RX MSG",               ReplyLength);
                        storeMsg(   MY_BFR, MY_IDX, 
                                    "Raw RX",               rx_msg,       
                                    ReplyLength);
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
#ifdef __LOG_VC04__
                    storeLogHex(   MY_BFR, MY_IDX, 
                                "FALSE ANSWER LENGTH - MSG #" );
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