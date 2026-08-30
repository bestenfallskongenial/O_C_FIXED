#include "h264_decoder.h"
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

                tx.msg                                             = {};   /* prime TX msg */

                memset(tx.msg.name, 0, sizeof(tx.msg.name));
                strncpy(tx.msg.name, "ril.video_decode", sizeof(tx.msg.name) - 1);
                        
                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_createComponent(tx,rx);
#endif        
                m_ComponentHandle = rx.msg.component_handle;

                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::getPortInfo           (   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_GET );

                tx.msg                                             = {};       /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_type                                   = port_type;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
                port_handle = rx.msg.port_handle;
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_getPortInfo(tx,rx);
#endif
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
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
                Log_setPortInfo(tx,rx); 
#endif                
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::enableComponent       (   MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_COMPONENT_ENABLE );

                tx.msg                                             = {};                                                   /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_enableComponent(tx,rx);
#endif        
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::setZeroCopyMode       (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src,  MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_PARAMETER_SET );

                tx.msg = {};                                                       /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = src.msg.port_handle; // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.msg.id                                          = MMAL_PARAMETER_ZERO_COPY;
                tx.msg.size                                        = sizeof(u32);

                memset(tx.msg.value, 0, sizeof(tx.msg.value));
                tx.msg.value[0] = 1;   /* enable zero-copy */

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_setZeroCopyMode(tx,rx);
#endif        
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::enablePort            (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_ACTION );

                tx.msg                                             = {};                                                   /* prime TX msg from GET snapshot */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = src.msg.port_handle;                // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.msg.action                                      = MMAL_MSG_PORT_ACTION_TYPE_ENABLE;
                tx.msg.port                                        = src.msg.port;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_enablePort(tx,rx);
#endif        
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::PrimeOutputBufferBody     (   MMAL_Buffer_From_Host_Msg& tx)
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

#ifdef __H264_DECODER_DEBUG_INIT__
//             Log_createComponent(tx);
#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::PrimeInputBufferBody      (   MMAL_Buffer_From_Host_Msg& tx)
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

#ifdef __H264_DECODER_DEBUG_INIT__
//               Log_createComponent(tx);
#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::setPortFormatInput( const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Info_Set_Msg& tx)
{
    // SET-prefix fields (must be explicit)
    tx.msg.component_handle = src.msg.component_handle;
    tx.msg.port_type        = src.msg.port_type;
    tx.msg.port_index       = src.msg.port_index;

    // Layout-compatible sub-structs
    tx.msg.port   = src.msg.port;
    tx.msg.format = src.msg.format;
    tx.msg.es     = src.msg.es;

    // Modifications
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::setPortFormatOutput( const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Info_Set_Msg& tx)
{
    // SET-prefix fields (must be explicit)
    tx.msg.component_handle = src.msg.component_handle;
    tx.msg.port_type        = src.msg.port_type;
    tx.msg.port_index       = src.msg.port_index;

    // Layout-compatible sub-structs
    tx.msg.port   = src.msg.port;
    tx.msg.format = src.msg.format;
    tx.msg.es     = src.msg.es;

    // Modifications
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
