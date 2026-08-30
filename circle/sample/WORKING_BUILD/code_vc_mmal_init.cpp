#include "kernel.h"

//  #undef  __LOG_MMAL__
    #define __LOG_MMAL__

    #define MY_BUFFER   m_logBuffer                 // not used here 
    #define MY_INDEX    m_logBufferIndex

bool            CKernel::initTexturesMMAL         (   )
{
                int f_count = 0;

                glGenTextures(1, &m_Texture);
                if(!checkGLerrorMMAL()) f_count++;
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if(!checkGLerrorMMAL()) f_count++;
                glBindTexture(GL_TEXTURE_2D, 0);
                if(!checkGLerrorMMAL()) f_count++;

                if( f_count != 0)
                    { 
#ifdef __LOG_MMAL__ 
                //  nextline( MY_BUFFER, MY_INDEX );                                 
                    storeLog( MY_BUFFER, MY_INDEX, "Texture Creation    FAILED");
#endif                   
                    return false;
                    }
#ifdef __LOG_MMAL__             
            //  nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "Texture Creation    SUCCESS");
#endif               
                return true;
}

bool            CKernel::createComponent            (   u32& ComponentHandle,                   // i assume this is the better approach right? 
                                                        MMAL_Component_Create_Msg& tx, 
                                                        MMAL_Component_Create_Reply& rx)
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_COMPONENT_CREATE );

                tx.msg                                             = {};   /* prime TX msg */

                memset(tx.msg.name, 0, sizeof(tx.msg.name));
                strncpy(tx.msg.name, "ril.video_decode", sizeof(tx.msg.name) - 1);
                        
                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleMMAL, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __LOG_MMAL__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __LOG_MMAL__
                Log_createComponent(tx,rx);
#endif       
                ComponentHandle = rx.msg.component_handle;

                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::getPortInfoMMAL            (   u32 port_type, 
                                                        u32& port_handle, 
                                                        MMAL_Port_Info_Get_Msg& tx, 
                                                        MMAL_Port_Info_Get_Reply& rx)
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_GET );

                tx.msg                                             = {};       /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_type                                   = port_type;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleMMAL, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __LOG_MMAL__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
                port_handle = rx.msg.port_handle;
#ifdef __LOG_MMAL__
                Log_getPortInfo(tx,rx);
#endif 
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::setPortInfoMMAL            (   MMAL_Port_Info_Set_Msg& tx, 
                                                        MMAL_Port_Info_Set_Reply& rx)
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_SET );

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleMMAL, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __LOG_MMAL__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __LOG_MMAL__
                Log_setPortInfo(tx,rx); 
#endif               
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

bool            CKernel::enableComponentMMAL        (   MMAL_Component_Enable_Msg& tx, 
                                                        MMAL_Component_Enable_Reply& rx)
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_COMPONENT_ENABLE );

                tx.msg                                             = {};
                tx.msg.component_handle                            = m_ComponentHandle;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleMMAL, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __LOG_MMAL__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __LOG_MMAL__
                Log_enableComponentMMAL(tx,rx);
#endif       
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}


bool            CKernel::setZeroCopyModeMMAL        (   u32                         port_handle,
                                                        MMAL_Port_Parameter_Set_Msg& tx,
                                                        MMAL_Port_Parameter_Set_Reply& rx )
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_PORT_PARAMETER_SET );

                tx.msg                                             = {};
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = port_handle;
                tx.msg.id                                          = MMAL_PARAMETER_ZERO_COPY;
                tx.msg.size                                        = (2 * sizeof(u32)) + sizeof(u32);   // OLD sizeof(u32);

                tx.msg.value[0]                                    = 1;

            //  memset(tx.msg.value, 0, sizeof(tx.msg.value));                                          // OLD

                size_t tx_len                                      = sizeof(mmal_msg_header)
                                                                    + (4 * sizeof(u32))
                                                                    + sizeof(u32);                      // NEW

                size_t rx_len                                      = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleMMAL,
                                      m_EventMMAL,
                                      &tx,
                                      tx_len,                                                           // OLD sizeof(tx)
                                      &rx,
                                      sizeof(rx),
                                      &rx_len ))
                    {
#ifdef __LOG_MMAL__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }

                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);                                      // OLD returns 04 !
}

bool            CKernel::enablePortMMAL             ( /*u32 port_handle,*/                          // why not also here? it works now but...
                                                        const MMAL_Port_Info_Get_Reply& src, 
                                                        MMAL_Port_Action_Msg& tx, 
                                                        MMAL_Port_Action_Reply_Msg& rx)
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_PORT_ACTION );

                tx.msg                                             = {};
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = src.msg.port_handle;                // port_handle - my original code takes it as parameter! i assume this is chosen because src has the correct handle
                tx.msg.action                                      = MMAL_MSG_PORT_ACTION_TYPE_ENABLE;
                tx.msg.port                                        = src.msg.port;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleMMAL, m_EventMMAL, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __LOG_MMAL__
                    storeLog( MY_BUFFER, MY_INDEX, "MMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif 
                    return false;
                    }
#ifdef __LOG_MMAL__
                Log_enablePort(tx,rx);
#endif       
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}

void            CKernel::PrimeOutputBufferBodyMMAL  (   MMAL_Buffer_From_Host_Msg& tx)
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
}

void            CKernel::PrimeInputBufferBodyMMAL   (   MMAL_Buffer_From_Host_Msg& tx)
{
                tx.msg = {};
                tx.msg.drvbuf.magic                                = MMAL_MAGIC;
                tx.msg.drvbuf.component_handle                     = m_ComponentHandle;
                tx.msg.drvbuf.port_handle                          = m_InputPortHandle;

                tx.msg.buffer_header.data                          = m_input_buffer_handle;
                tx.msg.buffer_header.alloc_size                    = m_InputBufferSize;

                tx.msg.buffer_header.pts_lo                        = 0;
                tx.msg.buffer_header.pts_hi                        = 0x80000000;
                tx.msg.buffer_header.dts_lo                        = 0;
                tx.msg.buffer_header.dts_hi                        = 0x80000000;

                tx.msg.is_zero_copy                                = 1;
                tx.msg.has_reference                               = 0;
                tx.msg.payload_in_message                          = 0;
}

void            CKernel::primePortFormatInputMMAL   (   u32 bufferSize, 
                                                        const MMAL_Port_Info_Get_Reply& src, 
                                                        MMAL_Port_Info_Set_Msg& tx)
{
            //  tx.msg = {};

                tx.msg.component_handle         = m_ComponentHandle;
                tx.msg.port_type                = MMAL_PORT_TYPE_INPUT;
                tx.msg.port_index               = 0;

                tx.msg.port                     = src.msg.port;
                tx.msg.format                   = src.msg.format;
                tx.msg.es                       = src.msg.es;

                tx.msg.port.buffer_num          = NUMBER_INPUTBUFFER;
                tx.msg.port.buffer_size         = bufferSize;

                tx.msg.format.encoding          = MMAL_ENCODING_H264;
                tx.msg.format.encoding_variant  = MMAL_ENCODING_VARIANT_H264_DEFAULT;

                tx.msg.es.video.width           = m_ResolutionX;
                tx.msg.es.video.height          = m_ResolutionY;
                tx.msg.es.video.crop.x          = 0;
                tx.msg.es.video.crop.y          = 0;
                tx.msg.es.video.crop.width      = m_ResolutionX;
                tx.msg.es.video.crop.height     = m_ResolutionY;
}
void            CKernel::primePortFormatOutputMMAL  (   u32 bufferSize, 
                                                        const MMAL_Port_Info_Get_Reply& src, 
                                                        MMAL_Port_Info_Set_Msg& tx)
{
            //  tx.msg = {};

                tx.msg.component_handle         = m_ComponentHandle;
                tx.msg.port_type                = MMAL_PORT_TYPE_OUTPUT;
                tx.msg.port_index               = 0;

                tx.msg.port                     = src.msg.port;
                tx.msg.format                   = src.msg.format;
                tx.msg.es                       = src.msg.es;

                tx.msg.port.buffer_num          = NUMBER_OUTPUTBUFFER;
                tx.msg.port.buffer_size         = bufferSize;

                tx.msg.format.encoding          = MMAL_ENCODING_I420;

                tx.msg.es.video.width           = m_ResolutionX;
                tx.msg.es.video.height          = m_ResolutionY;
                tx.msg.es.video.crop.x          = 0;
                tx.msg.es.video.crop.y          = 0;
                tx.msg.es.video.crop.width      = m_ResolutionX;
                tx.msg.es.video.crop.height     = m_ResolutionY;
}
