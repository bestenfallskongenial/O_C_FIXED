//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::MMALcallBack              (   void                   *callback_param, 
                                                            VCHI_CALLBACK_REASON_T  reason, 
                                                            void                   *msg_handle )
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {  
                    vcos_event_signal(event); 
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CH264Decoder::NextTransId               (   u32 &tid )
{
                tid = ( tid+1 ) & ~0x80000000u;                                             // mask for async messages really needed ?!                        
                return tid;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::GetVCHIstate              (   )
{
                vc_host_get_vchi_state(&m_VCHIInstance, &m_Connection);                         //1. get the VCHI instance and the connection handle from bcm_host.h
                MMALstoreLog ( "\nVCHI State Instance & Connection  ", (u32)m_VCHIInstance, (u32)m_Connection);                 

                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALinitEvents            (   )
{
                if (vcos_event_create(&m_VCOSevent, "MMAL") != VCOS_SUCCESS)
                    {
                    MMALstoreLog ( "\nVCOS Event Init FAILED!   ");                        
                    return false;
                    }
                MMALstoreLog ( "\nVCOS Event Init SUCCESS!  ", (u32)&m_VCOSevent);                    
                return true;    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::MMALstoreLog              (   const char* label, u32 value1, u32 value2, u32 value3, u32 value4 )
{
                /* always write the label */
                for (const char* p = label; *p; ++p)
                    m_DebugCharArray[m_CharIndex++] = *p;

                /* if all values are placeholders, finish */
                if ( value1 == STOREDEBUG_WHITESPACE &&
                    value2 == STOREDEBUG_WHITESPACE &&
                    value3 == STOREDEBUG_WHITESPACE &&
                    value4 == STOREDEBUG_WHITESPACE )
                {
                    m_DebugCharArray[m_CharIndex++] = '\n';
                    m_DebugCharArray[m_CharIndex]   = '\0';
                    return;
                }
                /* write first value if valid */
                if (value1 != STOREDEBUG_WHITESPACE) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value1 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write second value if valid */
                if (value2 != STOREDEBUG_WHITESPACE) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value2 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write third value if valid */
                if (value3 != STOREDEBUG_WHITESPACE) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value3 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write fourth value if valid */
                if (value4 != STOREDEBUG_WHITESPACE) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value4 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* terminate line */
                m_DebugCharArray[m_CharIndex++] = '\n';
                m_DebugCharArray[m_CharIndex]   = '\0';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::MMALstoreMsg              (   const void* tx_msg, u32 total_size, const char* label)
{   
                // insert leading newline
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;
                // copy label
                for (const char* p = label; *p; ++p) 
                    {
                    m_DebugCharArray[m_CharIndex] = *p;
                    m_CharIndex++;
                    }
                // next line please
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;
                // hex dump, 16 bytes per line
                const unsigned char* b = (const unsigned char*)tx_msg;
                for (u32 i = 0; i < total_size; ++i) {
                    if (i && (i % 16) == 0) 
                        {
                        m_DebugCharArray[m_CharIndex] = '\n';
                        m_CharIndex++;
                        }
                    unsigned char v = b[i];

                    char hi = "0123456789ABCDEF"[v >> 4];
                    m_DebugCharArray[m_CharIndex] = hi;
                    m_CharIndex++;

                    char lo = "0123456789ABCDEF"[v & 0xF];
                    m_DebugCharArray[m_CharIndex] = lo;
                    m_CharIndex++;

                    m_DebugCharArray[m_CharIndex] = ' ';
                    m_CharIndex++;
                }
                // newline + terminator
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;    
                m_DebugCharArray[m_CharIndex] = '\0';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALsendAndWait           (   const void *msg, 
                                                            size_t      msg_size, 
                                                            void       *rx_msg, 
                                                            size_t      max_reply_len, 
                                                            size_t     *actual_reply_len )
{
                MMALstoreLog("\nTX MSG", (u32)msg_size);
                MMALstoreMsg(msg, msg_size, "Raw TX");

                if (vchi_msg_queue(m_ServiceHandle, msg, msg_size, VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL) != 0)
                    {
                    return false;
                    }
                uint32_t ReplyLength = 0;
                do {
                    if (vchi_msg_dequeue(m_ServiceHandle, rx_msg, max_reply_len, &ReplyLength, VCHI_FLAGS_NONE) == 0)
                        {
                        MMALstoreLog("\nRX MSG", ReplyLength);
                        MMALstoreMsg(rx_msg, ReplyLength, "Raw RX");
                        break;
                        }
                    } 
                while (vcos_event_wait(&m_VCOSevent) == VCOS_SUCCESS);

                if (actual_reply_len)
                    {
                    *actual_reply_len = ReplyLength;
                    }
                return (ReplyLength != 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CALLBACK / HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------

