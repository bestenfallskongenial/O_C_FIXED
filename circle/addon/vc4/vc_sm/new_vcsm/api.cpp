//----------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------