//----------------------------------------------------------------------------------------------------------------------------------------------------
// h264_vc_sm.cpp
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "vc_sm_class.h"

#include <circle/types.h>
#include <circle/logger.h>
#include "interface/vcos/vcos.h"
// #include "interface/vcos/vcos_event.h"
#include <vc4/vchi/vchi.h>
#include <vc4/vchiq/vchiq.h>
#include <circle/bcm2835.h>

#define VCSMLOG

#define DEBUG_MAX_LENGTH (1024 * 16)

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CONSTRUCTOR / DECONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------------------------------------
                CVCSharedMemory::CVCSharedMemory    (   )
                : m_ServiceHandle(0)
                , m_TransactionId(0)
{
}
                CVCSharedMemory::~CVCSharedMemory   (   )
{
                if (m_ServiceHandle)
                    {
                    vchi_service_close(m_ServiceHandle);
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              USER API
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::initialize     (   )
{
#ifdef VCSMLOG
                storeLog ( SERVICENAMESTRING, SERVICEVERSIONSTRING);    
#endif // VCSMLOG
                getVCHI();

                if(!initEvents())
                    {
#ifdef VCSMLOG
                    storeLog ( "\nVCOS Init FAILED!");      
#endif // VCSMLOG
                    return false;
                    }
                if(!openService())
                    {
#ifdef VCSMLOG            
                    storeLog ( "VCHI Init FAILED!");      
#endif // VCSMLOG                    
                    return false;
                    }
#ifdef VCSMLOG  
                storeLog ( "\nVCSM Successful Initialized");      
#endif // VCSMLOG
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::importMemory   ( void* buffer, size_t size, int slot, vc_sm_import_msg& tx, vc_sm_import_result& rx)
{
                initHeader(tx, VC_SM_MSG_TYPE_IMPORT);

                tx.body = {};
                tx.body.type       = VC_SM_ALLOC_NON_CACHED;
                tx.body.addr       = convertAddress(buffer, size);
                tx.body.size       = static_cast<u32>(size);
                tx.body.kernel_id  = 0;
                tx.body.allocator  = 0;
                strncpy(tx.body.name, "SMEM", sizeof(tx.body.name));

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    return false;

                if (rx.res_handle != 0)
                {
                    m_vc_handle[slot] = rx.res_handle;
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::lockMemory     ( int slot, vc_sm_lock_msg& tx, vc_sm_lock_result_t& rx)
{
                initHeader(tx, VC_SM_MSG_TYPE_LOCK);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot];
                tx.body.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    return false;

                if (rx.res_mem != 0)
                {
                    m_vc_pointer[slot] = rx.res_mem;
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::freeMemory     ( int slot, vc_sm_free_msg& tx, vc_sm_result_t& rx)
{
                initHeader(tx, VC_SM_MSG_TYPE_FREE);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot];
                tx.body.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    return false;

                if (rx.success == 0)
                {
                    m_vc_handle[slot]  = 0;
                    m_vc_pointer[slot] = 0;
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// h264_vc_sm.cpp
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CVCSharedMemory::callback       ( void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle )
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {
                    vcos_event_signal(event);
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CVCSharedMemory::nextId         ( u32 &tid )
{
                tid = ( tid+1 ) & ~0x80000000u;                                             // mask for async messages really needed ?!                        
                return tid;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CVCSharedMemory::getVCHI        ( )
{
                vc_host_get_vchi_state(&m_VCHIInstance, &m_Connection);                         //1. get the VCHI instance and the connection handle from bcm_host.h
#ifdef VCSMLOG
                storeLog ( "\nVCHI State Instance     ", (u32)m_VCHIInstance);   
                storeLog ( "VCHI State Connection   ", (u32)m_Connection);   
#endif // VCSMLOG            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::initEvents     ( )
{
                if (vcos_event_create(&m_VCOSevent, "SMEM") != VCOS_SUCCESS)
                    {
            #ifdef VCSMLOG                        
                    storeLog ( "\nVCOS Event Init Failed!", (u32)&m_VCOSevent);  
            #endif // VCSMLOG                    
                    return false;                
                    }
                return true;    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CVCSharedMemory::convertAddress ( void* buffer, size_t size )
{
                u32 bus_addr = BUS_ADDRESS(reinterpret_cast<uintptr_t>(buffer));
                u32 vcsm_addr = (bus_addr & ~0xC0000000) | 0xC0000000;
            #ifdef VCSMLOG
                storeLog ( "\nBuffer Address USR", (u32)buffer); 
                storeLog ( "Buffer Address ARM", (u32)bus_addr); 
                storeLog ( "Buffer Address VPU", (u32)vcsm_addr); 
            #endif // VCSMLOG
                CleanAndInvalidateDataCacheRange((uintptr_t)(buffer), size);

                return vcsm_addr;
}

void            CVCSharedMemory::initHeader(vc_sm_msg_hdr_t& tx, u16 type)
{
                tx.hdr = {};
                tx.hdr.type     = type;
                tx.hdr.trans_id = nextId(m_TransactionId);
}
void            CH264Decoder::initHeader            (   MMAL_Port_Info_Set_Msg& tx, u32 type)
{
    tx.hdr = {};
    tx.hdr.magic           = MMAL_MAGIC;
    tx.hdr.type            = type;
    tx.hdr.control_service = 0;
    tx.hdr.context         = NextTransId(m_TransactionId);
    tx.hdr.status          = 0;
    tx.hdr.padding         = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::sendAndWait    ( const void *msg, size_t msg_size, void *rx_msg, size_t max_reply_len, size_t *actual_reply_len )
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
bool            CVCSharedMemory::openService    ( SERVICE_CREATION_T &tx)
{
                tx.version.version                                  = VC_MMAL_VER;
                tx.version.version_min                              = VC_MMAL_MIN_VER;
                tx.service_id                                       = VCHIQ_MAKE_FOURCC('S','M','E','M');
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
                return rc;
}
