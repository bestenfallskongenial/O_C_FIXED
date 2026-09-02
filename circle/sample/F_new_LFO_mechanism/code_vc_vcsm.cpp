#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex 

bool            CKernel::allocMemoryVCSM            (   size_t                  size,
                                                        u32                     base_unit,
                                                        u32                     alignment,
                                                        vc_sm_alloc_type_t      type,
                                                        u32                     allocator,
                                                        const char*             name,
                                                        u32&                    vcsm_handle,
                                                        VCSM_Alloc_MEM_Msg&     tx,
                                                        VCSM_Alloc_MEM_Reply&   rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_ALLOC);

                u32 num_unit = (static_cast<u32>(size) + base_unit - 1) / base_unit;

                tx.body = {};
                tx.body.type       = type;
                tx.body.base_unit  = base_unit;
                tx.body.num_unit   = num_unit;
                tx.body.alignment  = alignment;
                tx.body.allocator  = allocator;
                strncpy(tx.body.name, name, sizeof(tx.body.name));

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len )) return false;

                if (rx.body.res_handle != 0)
                    {
                    vcsm_handle = rx.body.res_handle;

#ifdef __LOG_VCSM__
                    storeLogHex(   MY_BFR, MY_IDX, 
                                "ALLOC   - VCSM Handle",    rx.body.res_handle, 
                                "VCSM Pointer",             rx.body.res_mem, 
                                "Base Size",                rx.body.res_base_size, 
                                "Num",                      rx.body.res_num );
#endif
                    return true;
                    }

                return false;
}

bool            CKernel::importMemoryVCSM           (   void*                   p_bufferBlockbase, 
                                                        size_t                  size, 
                                                        u32&                    vcsm_handle,   
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

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len )) return false;

                if (rx.body.res_handle != 0)
                    {
                    vcsm_handle         = rx.body.res_handle; //   like this ?                    
#ifdef __LOG_VCSM__
                    storeLogHex(   MY_BFR, MY_IDX, 
                                "IMPORT  - ARM Address",    (u32)(uintptr)p_bufferBlockbase, 
                                "GPU  Address",             tx.body.addr, 
                                "Size",                     size, 
                                "VCSM Handle",              rx.body.res_handle);
                    nextline(   MY_BFR, MY_IDX );
#endif                     
                    return true;
                    }
                return false;
}

bool            CKernel::lockMemoryVCSM             (   u32&                    vcsm_handle,
                                                        u32&                    vcsm_pointer, 
                                                        VCSM_Lock_MEM_Msg&      tx, 
                                                        VCSM_Lock_MEM_Reply&    rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_LOCK);

                tx.body = {};

                tx.body.res_handle = vcsm_handle;
                tx.body.res_mem    = 0;             // never set this to zero but have not to either, right?! 

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) ) return false;

                if (rx.body.res_mem != 0)
                    {
                    vcsm_pointer        = rx.body.res_mem;
#ifdef __LOG_VCSM__
                        storeLogHex(   MY_BFR, MY_IDX, 
                                    "LOCK    - VCSM Handle",    rx.body.res_handle, 
                                    "VCSM Pointer",             rx.body.res_mem);
                        nextline(   MY_BFR, MY_IDX );
#endif         
                    return true;
                    }
                return false;
}

bool            CKernel::freeMemoryVCSM             (   u32&                    vcsm_handle, 
                                                        u32&                    vcsm_pointer,                                             
                                                        VCSM_Free_MEM_Msg&      tx, 
                                                        VCSM_Free_MEM_Reply&    rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_FREE);

                tx.body = {};
                tx.body.res_handle = vcsm_handle;
                tx.body.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) ) return false;

                if (rx.body.success == 0)
                {
                    vcsm_handle         = 0; 
                    vcsm_pointer        = 0;
#ifdef __LOG_VCSM__

                        storeLogHex(   MY_BFR, MY_IDX, 
                                    "FREE    - VCSM Handle",    vcsm_handle, 
                                    "VCSM Pointer",             vcsm_pointer);  
#endif        
                    return true;
                }
                return false;
}