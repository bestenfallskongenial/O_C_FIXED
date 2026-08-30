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

                GetVCHIstate                ( );
                MMALinitEvents              ( );
                
                MMALopenService             ( m_ServiceCreate );                       /* service */

                if (!MMALcreateComponent    ( m_ComponentCreateTx, m_ComponentCreateRx ))    /* component */
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALcreateComponent FAILED");                        
            #endif                                         
                    return false;
                    }
                if (!MMALgetPortInfo        ( MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_A, m_PortInfoGetRx_Input_A ))    /* ---------- SNAPSHOT A ---------- */
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALgetPortInfo Input FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALgetPortInfo        ( MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_A, m_PortInfoGetRx_Output_A ))
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALgetPortInfo Output FAILED");                        
            #endif                                         
                    return false;
                    }

                MMALsetInputPortFormat      ( m_PortInfoGetRx_Input_A, m_PortInfoSetTx_Input );      /* ---------- SET FORMATS ---------- */

                MMALsetOutputPortFormat     ( m_PortInfoGetRx_Output_A, m_PortInfoSetTx_Output );
                if (!MMALsetPortInfo        ( m_PortInfoSetTx_Input, m_PortInfoSetRx_Input ))
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALsetPortInfo Input FAILED");                        
            #endif                                         
                    return false;
                    }
                if (!MMALsetPortInfo        ( m_PortInfoSetTx_Output, m_PortInfoSetRx_Output ))
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALsetPortInfo Output FAILED");                        
            #endif                                         
                    return false;
                    }
                if (!MMALenableComponent    ( m_ComponentEnableTx, m_ComponentEnableRx ))    /* ---------- ENABLE COMPONENT ---------- */
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALenableComponent FAILED");                        
            #endif                                         
                    return false;
                    }
                if (!MMALgetPortInfo        ( MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_B, m_PortInfoGetRx_Input_B )) /* ---------- SNAPSHOT B ---------- */
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALgetPortInfo Input FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALgetPortInfo        ( MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_B, m_PortInfoGetRx_Output_B ))
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALgetPortInfo Output FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALsetZeroCopyMode    ( m_PortInfoGetRx_Input_B, m_PortParamTx_Input, m_PortParamRx_Input ))    /* ---------- ZERO COPY ---------- */
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALsetZeroCopyMode Input FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALsetZeroCopyMode    ( m_PortInfoGetRx_Output_B, m_PortParamTx_Output, m_PortParamRx_Output ))
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALsetZeroCopyMode Output FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALgetPortInfo        ( MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_C, m_PortInfoGetRx_Input_C ))    /* ---------- SNAPSHOT C ---------- */
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALgetPortInfo Input FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALgetPortInfo        ( MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_C, m_PortInfoGetRx_Output_C ))
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALgetPortInfo Output FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALenablePort         ( m_PortInfoGetRx_Input_C, m_PortActionTx_Input, m_PortActionRx_Input ))   /* ---------- ENABLE PORTS ---------- */
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALenablePort Input FAILED");                        
            #endif                                         
                    return false;
                    }

                if (!MMALenablePort         ( m_PortInfoGetRx_Output_C, m_PortActionTx_Output, m_PortActionRx_Output ))
                    {     
            #ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nMMALenablePort Output FAILED");                        
            #endif                                         
                    return false;
                    }
            /*
                MMALqueueInputBuffer        ( m_BufferFromHostTx_Input,         // ---------- BUFFERS ---------- 
                                            m_BufferFromHostRx_Input );

                MMALqueueOutputBuffer       ( m_BufferFromHostTx_OutputA,
                                            m_BufferFromHostRx_OutputA );

                MMALqueueOutputBuffer       ( m_BufferFromHostTx_OutputB,
                                            m_BufferFromHostRx_OutputB );
            */
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::MMALcallBack              (   void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle )
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {  
                    vcos_event_signal(event); 
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CH264Decoder::NextTransId               (   u32 &tid )
{
                tid = ( tid+1 ) & ~0x80000000u;                                             // mask for async messages really needed ?!                        
                return tid;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::GetVCHIstate              (   )
{
                vc_host_get_vchi_state(&m_VCHIInstance, &m_Connection);                         //1. get the VCHI instance and the connection handle from bcm_host.h
#ifdef __H264_DECODER_DEBUG_INIT__
                MMALstoreLog ( "\nVCHI State Instance & Connection  ", (u32)m_VCHIInstance, (u32)m_Connection);                 
#endif
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALinitEvents            (   )
{
                if (vcos_event_create(&m_VCOSevent, "MMAL") != VCOS_SUCCESS)
                    {
#ifdef __H264_DECODER_DEBUG_INIT__ 
                    MMALstoreLog ( "\nVCOS Event Init FAILED!   ");                        
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__ 
                MMALstoreLog ( "\nVCOS Event Init SUCCESS!  ", (u32)&m_VCOSevent);
#endif                
                return true;    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALsendAndWait           (   const void *msg, size_t msg_size, void *rx_msg, size_t max_reply_len, size_t *actual_reply_len )
{
#ifdef __H264_DECODER_DEBUG_INIT__
                MMALstoreLog("\nTX MSG", (u32)msg_size);
                MMALstoreMsg(msg, msg_size, "Raw TX");
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
                        MMALstoreLog("\nRX MSG", ReplyLength);
                        MMALstoreMsg(rx_msg, ReplyLength, "Raw RX");
#endif
                        break;
                        }
                    } 
                while (vcos_event_wait(&m_VCOSevent) == VCOS_SUCCESS);

                if (actual_reply_len)
                    {
                    *actual_reply_len = ReplyLength;
                    }
                    /* enforce completeness HERE */
                if (ReplyLength != max_reply_len)
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
#endif                                    
                    return false;
                    }

    return true;        

            //  return (ReplyLength != 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALopenService           (   SERVICE_CREATION_T &tx)
{
                tx.version.version                                  = VC_MMAL_VER;
                tx.version.version_min                              = VC_MMAL_MIN_VER;
                tx.service_id                                       = VCHIQ_MAKE_FOURCC('m','m','a','l');
                tx.connection                                       = m_Connection;
                tx.rx_fifo_size                                     = 0;
                tx.tx_fifo_size                                     = 0;
                tx.callback                                         = MMALcallBack;
                tx.callback_param                                   = &m_VCOSevent;
                tx.want_unaligned_bulk_rx                           = 0;
                tx.want_unaligned_bulk_tx                           = 0;
                tx.want_crc                                         = 0;

                int rc = vchi_service_open( m_VCHIInstance, &tx, &m_ServiceHandle );    

            #ifdef __H264_DECODER_DEBUG_INIT__
                Log_MMALopenService(tx);
            #endif

                return rc;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALcreateComponent       (   MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply_Msg& rx)
{
                tx.hdr                                              = {};    /* prime TX header */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_COMPONENT_CREATE;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;

                tx.body                                             = {};   /* prime TX body */
                tx.body.client_component                            = 0;
                memset(tx.body.name, 0, sizeof(tx.body.name));
                strncpy(tx.body.name, "ril.video_decode",
                        sizeof(tx.body.name) - 1);
                tx.body.pid                                         = 0;

                size_t rx_len = 0;

                if (!MMALsendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
            #ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
            #endif
                    return false;
                    }
                if (rx_len < sizeof(rx))
                    {
            #ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
            #endif            
                    return false;
                    }
            #ifdef __H264_DECODER_DEBUG_INIT__
                Log_MMALcreateComponent(tx, rx);
            #endif        
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALgetPortInfo           (   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply_Msg& rx)
{
                tx.hdr                                              = {};       /* prime TX header */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_PORT_INFO_GET;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;

                tx.body                                             = {};       /* prime TX body */
                tx.body.component_handle                            = m_ComponentHandle;
                tx.body.port_type                                   = port_type;
                tx.body.index                                       = 0;

                size_t rx_len = 0;

                if (!MMALsendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
//              if (rx_len < sizeof(rx))
//                  {
//#ifdef __H264_DECODER_DEBUG_INIT__
//                  MMALstoreLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
//#endif            
//                  return false;
//                  }
            /* IM PRETTY SURE THIS IS REDUNDANT ANYWAY, WHAT MESSAGE ELSE I SHOULD RECEIVE?!
                if (rx.hdr.type != MMAL_MSG_TYPE_PORT_INFO_GET)
                    return false;   */
                port_handle = rx.body.port_handle;
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_MMALgetPortInfo(tx, rx);
#endif
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::MMALsetInputPortFormat    (   const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Info_Set_Msg& tx) // const MMAL_Port_Info_Get_Reply_Msg& src
{
                tx.body                                             = src.body;

                tx.body.port.buffer_num                             = NUMBER_INPUTBUFFER;
                tx.body.port.buffer_size                            = m_InputBufferSize;

                tx.body.format.encoding                             = MMAL_ENCODING_H264;
                tx.body.format.encoding_variant                     = MMAL_ENCODING_VARIANT_H264_DEFAULT;

                tx.body.es.video.width                              = m_ResolutionX;
                tx.body.es.video.height                             = m_ResolutionY;
                tx.body.es.video.crop.x                             = 0;
                tx.body.es.video.crop.y                             = 0;
                tx.body.es.video.crop.width                         = m_ResolutionX;
                tx.body.es.video.crop.height                        = m_ResolutionY;

#ifdef __H264_DECODER_DEBUG_INIT__

#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::MMALsetOutputPortFormat   (   const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Info_Set_Msg& tx) // const MMAL_Port_Info_Get_Reply_Msg& src
{
                tx.body                                             = src.body;

                tx.body.port.buffer_num                             = NUMBER_OUTPUTBUFFER;
                tx.body.port.buffer_size                            = m_OutputBufferSize;

                tx.body.format.encoding                             = MMAL_ENCODING_I420;

                tx.body.es.video.width                              = m_ResolutionX;
                tx.body.es.video.height                             = m_ResolutionY;
                tx.body.es.video.crop.x                             = 0;
                tx.body.es.video.crop.y                             = 0;
                tx.body.es.video.crop.width                         = m_ResolutionX;
                tx.body.es.video.crop.height                        = m_ResolutionY;

#ifdef __H264_DECODER_DEBUG_INIT__

#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALsetPortInfo           (   MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply_Msg& rx)
{
                tx.hdr                                              = {};                                            /* prime header only */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_PORT_INFO_SET;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;

                size_t rx_len = 0;

                if (!MMALsendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
//              if (rx_len < sizeof(rx))
//                  {
//#ifdef __H264_DECODER_DEBUG_INIT__
//                  MMALstoreLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
//#endif            
//                  return false;
//                  }
            /* IM PRETTY SURE THIS IS REDUNDANT ANYWAY, WHAT MESSAGE ELSE I SHOULD RECEIVE?!
                if (rx.hdr.type != MMAL_MSG_TYPE_PORT_INFO_SET)
                    return false;   */
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_MMALsetPortInfo(tx, rx); 
#endif                
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALenableComponent       (   MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply_Msg& rx)
{
                tx.hdr                                              = {};                                                    /* prime TX header */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_COMPONENT_ENABLE;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;

                tx.body                                             = {};                                                   /* prime TX body */
                tx.body.component_handle                            = m_ComponentHandle;

                size_t rx_len = 0;

                if (!MMALsendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
//              if (rx_len < sizeof(rx))
//                  {
//#ifdef __H264_DECODER_DEBUG_INIT__
//                  MMALstoreLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
//#endif            
//                  return false;
//                  }
            /* IM PRETTY SURE THIS IS REDUNDANT ANYWAY, WHAT MESSAGE ELSE I SHOULD RECEIVE?!
                if (rx.hdr.type != MMAL_MSG_TYPE_COMPONENT_ENABLE)
                    return false;   */
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_MMALenableComponent(tx, rx);
#endif        
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALenablePort            (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx)
{
                
                tx.hdr                                              = {};                                                    /* prime TX header */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_PORT_ACTION;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;
                
                tx.body                                             = {};                                                   /* prime TX body from GET snapshot */
                tx.body.component_handle                            = m_ComponentHandle;
                tx.body.port_handle                                 = src.body.port_handle;                // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.body.action                                      = MMAL_MSG_PORT_ACTION_TYPE_ENABLE;
                tx.body.port                                        = src.body.port;

                size_t rx_len = 0;

                if (!MMALsendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
//              if (rx_len < sizeof(rx))
//                  {
//#ifdef __H264_DECODER_DEBUG_INIT__
//                  MMALstoreLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
//#endif            
//                  return false;
//                  }
            /* IM PRETTY SURE THIS IS REDUNDANT ANYWAY, WHAT MESSAGE ELSE I SHOULD RECEIVE?!
                if (rx.hdr.type != MMAL_MSG_TYPE_PORT_ACTION)
                    return false;   */
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_MMALenablePort(tx, rx);
#endif        
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALsetZeroCopyMode       (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src,  MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply_Msg& rx)
{
                tx.hdr                                              = {};                                                        /* prime TX header */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_PORT_PARAMETER_SET;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;

                tx.body = {};                                                       /* prime TX body */
                tx.body.component_handle                            = m_ComponentHandle;
                tx.body.port_handle                                 = src.body.port_handle; // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.body.id                                          = MMAL_PARAMETER_ZERO_COPY;
                tx.body.size                                        = sizeof(u32);

                memset(tx.body.value, 0, sizeof(tx.body.value));
                tx.body.value[0] = 1;   /* enable zero-copy */

                size_t rx_len = 0;

                if (!MMALsendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    MMALstoreLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
//              if (rx_len < sizeof(rx))
//                  {
//#ifdef __H264_DECODER_DEBUG_INIT__
//                  MMALstoreLog("\nMMALsendAndWait ANSWER TO SHORT - MSG #", tx.hdr.context );
//#endif            
//                  return false;
//                  }
            /* IM PRETTY SURE THIS IS REDUNDANT ANYWAY, WHAT MESSAGE ELSE I SHOULD RECEIVE?!
                if (rx.hdr.type != MMAL_MSG_TYPE_PORT_PARAMETER_SET)
                    return false;
            */
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_MMALsetZeroCopyMode(tx, rx);
#endif        
                return (rx.body.status == MMAL_MSG_STATUS_SUCCESS);
}
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
                Log_MMALcreateComponent(tx, rx);
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
                Log_MMALcreateComponent(tx, rx);
#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALqueueOutputBuffer     (   MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size)
{
                tx.hdr = {};                                                /* prime header (dynamic per queue) */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_BUFFER_FROM_HOST;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;

                tx.body.drvbuf.client_context                       = tx.hdr.context;  /* patch dynamic fields into already-primed body */

                tx.body.buffer_header.data                          = vc_handle;
                tx.body.buffer_header.alloc_size                    = alloc_size;
                tx.body.buffer_header.length                        = 0;
                tx.body.buffer_header.offset                        = 0;
                tx.body.buffer_header.flags                         = 0;

#ifdef __H264_DECODER_DEBUG_RUNTIME__
                MMALstoreLog("\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                MMALstoreMsg(&tx, (u32)sizeof(tx), "QueueOutputBuffer");
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::MMALqueueInputBuffer      (   MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length)
{
                tx.hdr                                              = {};                                                    /* prime header (dynamic per queue) */
                tx.hdr.magic                                        = MMAL_MAGIC;
                tx.hdr.type                                         = MMAL_MSG_TYPE_BUFFER_FROM_HOST;
                tx.hdr.control_service                              = 0;
                tx.hdr.context                                      = NextTransId(m_TransactionId);
                tx.hdr.status                                       = 0;
                tx.hdr.padding                                      = 0;

                tx.body.drvbuf.client_context                       = tx.hdr.context;                /* patch dynamic fields into already-primed body */

                tx.body.buffer_header.offset                        = frame_offset;
                tx.body.buffer_header.length                        = frame_length;

                u32 flags                                           = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;
                tx.body.buffer_header.flags                         = flags;
#ifdef __H264_DECODER_DEBUG_RUNTIME__
                MMALstoreLog("\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                MMALstoreMsg(&tx, (u32)sizeof(tx), "QueueInputBuffer");
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------