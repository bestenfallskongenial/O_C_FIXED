#include "h264_decoder.h"
// ---------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __H264_DECODER_DEBUG_INIT__
void            CH264Decoder::storeLog              (   const char* label, u32 value1, u32 value2, u32 value3, u32 value4 )
{
                /* always write the label */
                for (const char* p = label; *p; ++p)
                    m_DebugCharArray[m_CharIndex++] = *p;

                /* if all values are placeholders, finish */
                if ( value1 == EMPTYLOG &&
                    value2 == EMPTYLOG &&
                    value3 == EMPTYLOG &&
                    value4 == EMPTYLOG )
                {
                    m_DebugCharArray[m_CharIndex++] = '\n';
                    m_DebugCharArray[m_CharIndex]   = '\0';
                    return;
                }
                /* write first value if valid */
                if (value1 != EMPTYLOG) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value1 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write second value if valid */
                if (value2 != EMPTYLOG) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value2 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write third value if valid */
                if (value3 != EMPTYLOG) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value3 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write fourth value if valid */
                if (value4 != EMPTYLOG) {
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
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void            CH264Decoder::storeMsg              ( const char* label, const void* tx_msg, u32 total_size)
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
// ---------------------------------------------------------------------------------------------------------------------------------------------------
inline void CH264Decoder::nextline()
{
    m_DebugCharArray[m_CharIndex++] = '\n';
    m_DebugCharArray[m_CharIndex]   = '\0';
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_openService(const SERVICE_CREATION_T &src)
{
    nextline();
    storeLog("version                           ", src.version.version);
    storeLog("version_min                       ", src.version.version_min);
    storeLog("service_id                        ", src.service_id);
    storeLog("connection                        ", (u32)(uintptr_t)src.connection);
    storeLog("rx_fifo_size                      ", src.rx_fifo_size);
    storeLog("tx_fifo_size                      ", src.tx_fifo_size);
    storeLog("callback                          ", (u32)(uintptr_t)src.callback);
    storeLog("callback_param                    ", (u32)(uintptr_t)src.callback_param);
    storeLog("want_unaligned_bulk_rx            ", src.want_unaligned_bulk_rx);
    storeLog("want_unaligned_bulk_tx            ", src.want_unaligned_bulk_tx);
    storeLog("want_crc                          ", src.want_crc);                            // complete
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_createComponent( const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply_Msg& rx)
{
    nextline();
    storeLog("hdr.magic                         ", tx.hdr.magic);                   // tx HEADER
    storeLog("hdr.type                        **", tx.hdr.type);
    storeLog("hdr.control_service               ", tx.hdr.control_service);
    storeLog("hdr.context                       ", tx.hdr.context);
    storeLog("hdr.status                        ", tx.hdr.status);
    storeLog("hdr.padding                       ", tx.hdr.padding);
    nextline();
    storeLog("msg.client_component             ", tx.msg.client_component);       // tx BODY
    nextline(); 
    storeMsg("msg.name                         ", tx.msg.name, sizeof(tx.msg.name));           // shows name[128] exactly as sent
    storeLog("msg.pid                          ", tx.msg.pid);
    nextline();
    storeLog("hdr.magic                         ", rx.hdr.magic);                   // rx HEADER
    storeLog("hdr.type                        **", rx.hdr.type);
    storeLog("hdr.control_service               ", rx.hdr.control_service);
    storeLog("hdr.context                       ", rx.hdr.context);
    storeLog("hdr.status                      **", rx.hdr.status);
    storeLog("hdr.padding                       ", rx.hdr.padding);
    nextline();
    storeLog("msg.status                     **", rx.msg.status);                 // rx BODY
    storeLog("msg.component_handle           **", rx.msg.component_handle);
    storeLog("msg.input_num                   *", rx.msg.input_num);
    storeLog("msg.output_num                  *", rx.msg.output_num);
    storeLog("msg.clock_num                   *", rx.msg.clock_num);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_getPortInfo( const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply_Msg& rx)
{
    nextline();                                                             // tx HEADER
    storeLog("hdr.magic                         ", tx.hdr.magic);
    storeLog("hdr.type                        **", tx.hdr.type);
    storeLog("hdr.control_service               ", tx.hdr.control_service);
    storeLog("hdr.context                       ", tx.hdr.context);
    storeLog("hdr.status                        ", tx.hdr.status);
    storeLog("hdr.padding                       ", tx.hdr.padding);
    nextline();                                                             // tx BODY
    storeLog("msg.component_handle           **", tx.msg.component_handle);
    storeLog("msg.port_type                    ", tx.msg.port_type);
    storeLog("msg.index                        ", tx.msg.index);
    nextline();                                                             // rx HEADER
    storeLog("hdr.magic                         ", rx.hdr.magic);
    storeLog("hdr.type                        **", rx.hdr.type);
    storeLog("hdr.control_service               ", rx.hdr.control_service);
    storeLog("hdr.context                       ", rx.hdr.context);
    storeLog("hdr.status                      **", rx.hdr.status);
    storeLog("hdr.padding                       ", rx.hdr.padding);
    nextline();                                                             // rx BODY
    storeLog("msg.status                     **", rx.msg.status);
    storeLog("msg.component_handle           **", rx.msg.component_handle);
    storeLog("msg.port_type                    ", rx.msg.port_type);
    storeLog("msg.port_index                   ", rx.msg.port_index);
    storeLog("msg.found                        ", rx.msg.found);
    storeLog("msg.port_handle                **", rx.msg.port_handle);
    nextline();
    storeLog("port.priv                         ", rx.msg.port.priv);              // rx PORT //
    storeLog("port.name                         ", rx.msg.port.name);
    storeLog("port.type                         ", rx.msg.port.type);
    storeLog("port.index                        ", rx.msg.port.index);
    storeLog("port.index_all                    ", rx.msg.port.index_all);
    storeLog("port.is_enabled                   ", rx.msg.port.is_enabled);
    storeLog("port.format                       ", rx.msg.port.format);
    storeLog("port.buffer_num_min               ", rx.msg.port.buffer_num_min);
    storeLog("port.buffer_size_min              ", rx.msg.port.buffer_size_min);
    storeLog("port.buffer_alignment_min         ", rx.msg.port.buffer_alignment_min);
    storeLog("port.buffer_num_recommended       ", rx.msg.port.buffer_num_recommended);
    storeLog("port.buffer_size_recommended      ", rx.msg.port.buffer_size_recommended);
    storeLog("port.buffer_num                 **", rx.msg.port.buffer_num);
    storeLog("port.buffer_size                **", rx.msg.port.buffer_size);
    storeLog("port.component                    ", rx.msg.port.component);
    storeLog("port.userdata                     ", rx.msg.port.userdata);
    storeLog("port.capabilities                 ", rx.msg.port.capabilities);
    nextline(); 
    storeLog("format.type                       ", rx.msg.format.type);            // rx FORMAT //
    storeLog("format.encoding                 **", rx.msg.format.encoding);
    storeLog("format.encoding_variant         **", rx.msg.format.encoding_variant);
    storeLog("format.es                         ", rx.msg.format.es);
    storeLog("format.bitrate                  **", rx.msg.format.bitrate);
    storeLog("format.flags                      ", rx.msg.format.flags);
    storeLog("format.extradata_size             ", rx.msg.format.extradata_size);
    storeLog("format.extradata                  ", rx.msg.format.extradata);
    nextline(); 
    storeLog("es.video.width                    ", rx.msg.es.video.width);         // rx ES
    storeLog("es.video.height                   ", rx.msg.es.video.height);
    storeLog("es.video.crop.x                   ", rx.msg.es.video.crop.x);
    storeLog("es.video.crop.y                   ", rx.msg.es.video.crop.y);
    storeLog("es.video.crop.width               ", rx.msg.es.video.crop.width);
    storeLog("es.video.crop.height              ", rx.msg.es.video.crop.height);
    storeLog("es.video.frame_rate.num           ", rx.msg.es.video.frame_rate.num);
    storeLog("es.video.frame_rate.den           ", rx.msg.es.video.frame_rate.den);
    storeLog("es.video.par.num                  ", rx.msg.es.video.par.num);
    storeLog("es.video.par.den                  ", rx.msg.es.video.par.den);
    storeLog("es.video.color_space              ", rx.msg.es.video.color_space);
    nextline();
    storeMsg("extradata", rx.msg.extradata, rx.msg.format.extradata_size);    // rx EXTRADATA
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_setPortInfo( const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply_Msg& rx)
{
    nextline();
    storeLog("hdr.magic                         ", tx.hdr.magic);                       // tx HEADER
    storeLog("hdr.type                        **", tx.hdr.type);
    storeLog("hdr.control_service               ", tx.hdr.control_service);
    storeLog("hdr.context                       ", tx.hdr.context);
    storeLog("hdr.status                        ", tx.hdr.status);
    storeLog("hdr.padding                       ", tx.hdr.padding);
    nextline();
    storeLog("msg.component_handle           **", tx.msg.component_handle);           // tx BODY
    storeLog("msg.port_type                    ", tx.msg.port_type);
    storeLog("msg.port_index                   ", tx.msg.port_index);
    nextline();
    storeLog("port.priv                         ", tx.msg.port.priv);                       // Private member used by the framework
    storeLog("port.name                         ", tx.msg.port.name);                       // Port name. Used for debugging purposes (RO)
    storeLog("port.type                         ", tx.msg.port.type);                       // Type of the port (RO) enum mmal_port_type
    storeLog("port.index                        ", tx.msg.port.index);                      // Index of the port in its type list (RO)
    storeLog("port.index_all                    ", tx.msg.port.index_all);                  // Index of the port in the list of all ports (RO)
    storeLog("port.is_enabled                   ", tx.msg.port.is_enabled);                 // Indicates whether the port is enabled or not (RO)
    storeLog("port.format                       ", tx.msg.port.format);                     // Format of the elementary stream
    storeLog("port.buffer_num_min               ", tx.msg.port.buffer_num_min);             // Minimum number of buffers the port requires (RO).  This is set by the component.
    storeLog("port.buffer_size_min              ", tx.msg.port.buffer_size_min);            // Minimum size of buffers the port requires (RO).  This is set by the component.
    storeLog("port.buffer_alignment_min         ", tx.msg.port.buffer_alignment_min);       // Minimum alignment requirement for the buffers (RO).  A value of zero means no special alignment requirements.  This is set by the component.
    storeLog("port.buffer_num_recommended       ", tx.msg.port.buffer_num_recommended);     // Number of buffers the port recommends for optimal performance (RO).  A value of zero means no special recommendation.  This is set by the component.
    storeLog("port.buffer_size_recommended      ", tx.msg.port.buffer_size_recommended);    // Size of buffers the port recommends for optimal performance (RO).  A value of zero means no special recommendation.  This is set by the component.
    storeLog("port.buffer_num                 **", tx.msg.port.buffer_num);                 // Actual number of buffers the port will use. This is set by the client.
    storeLog("port.buffer_size                **", tx.msg.port.buffer_size);                // Actual maximum size of the buffers that will be sent to the port. This is set by the client.
    storeLog("port.component                    ", tx.msg.port.component);                  // Component this port belongs to (Read Only)
    storeLog("port.userdata                     ", tx.msg.port.userdata);                   // Field reserved for use by the client
    storeLog("port.capabilities                 ", tx.msg.port.capabilities);               // Flags describing the capabilities of a port (RO).  Bitwise combination of \ref portcapabilities "Port capabilities" values.
    nextline(); 
    storeLog("format.type                       ", tx.msg.format.type);                     // enum mmal_es_type
    storeLog("format.encoding                 **", tx.msg.format.encoding);                 // FourCC specifying encoding of the elementary stream.
    storeLog("format.encoding_variant         **", tx.msg.format.encoding_variant);         // FourCC specifying the specific encoding variant of the elementary stream.
    storeLog("format.es                         ", tx.msg.format.es);                       // Type specific information for the elementary stream
    storeLog("format.bitrate                  **", tx.msg.format.bitrate);                  // Bitrate in bits per second
    storeLog("format.flags                      ", tx.msg.format.flags);                    // Flags describing properties of the elementary stream.
    storeLog("format.extradata_size             ", tx.msg.format.extradata_size);           // Size of the codec specific data
    storeLog("format.extradata                  ", tx.msg.format.extradata);                // Codec specific data
    nextline(); 
    storeLog("es.video.width                    ", tx.msg.es.video.width);                  // Width of frame in pixels
    storeLog("es.video.height                   ", tx.msg.es.video.height);                 // Height of frame in rows of pixels
    storeLog("es.video.crop.x                   ", tx.msg.es.video.crop.x);                 // Visible region of the frame  // x coordinate (from left)
    storeLog("es.video.crop.y                   ", tx.msg.es.video.crop.y);                                                 // y coordinate (from top)
    storeLog("es.video.crop.width               ", tx.msg.es.video.crop.width);                                             // width
    storeLog("es.video.crop.height              ", tx.msg.es.video.crop.height);                                            // height
    storeLog("es.video.frame_rate.num           ", tx.msg.es.video.frame_rate.num);         // Frame rate                   // Numerator
    storeLog("es.video.frame_rate.den           ", tx.msg.es.video.frame_rate.den);                                         // Denominator
    storeLog("es.video.par.num                  ", tx.msg.es.video.par.num);                // Pixel aspect ratio           // Numerator
    storeLog("es.video.par.den                  ", tx.msg.es.video.par.den);                                                // Denominator
    storeLog("es.video.color_space              ", tx.msg.es.video.color_space);            // FourCC specifying the color space of the video stream. See the MmalColorSpace "pre-defined color spaces" for some examples.
    nextline(); 
    storeMsg("extradata                         ", tx.msg.extradata, tx.msg.format.extradata_size);     // tx extradata
    nextline();                                                                 // rx HEADER
    storeLog("hdr.magic                         ", rx.hdr.magic);
    storeLog("hdr.type                        **", rx.hdr.type);
    storeLog("hdr.control_service               ", rx.hdr.control_service);
    storeLog("hdr.context                       ", rx.hdr.context);
    storeLog("hdr.status                      **", rx.hdr.status);
    storeLog("hdr.padding                       ", rx.hdr.padding);
    nextline();                                                                 // rx BODY
    storeLog("msg.status                     **", rx.msg.status);
    storeLog("msg.component_handle           **", rx.msg.component_handle);
    storeLog("msg.port_type                    ", rx.msg.port_type);
    storeLog("msg.index                        ", rx.msg.index);
    storeLog("msg.found                        ", rx.msg.found);
    storeLog("msg.port_handle                **", rx.msg.port_handle);
    nextline();                                                                 // rx port
    storeLog("port.priv                         ", rx.msg.port.priv);
    storeLog("port.name                         ", rx.msg.port.name);
    storeLog("port.type                         ", rx.msg.port.type);
    storeLog("port.index                        ", rx.msg.port.index);
    storeLog("port.index_all                    ", rx.msg.port.index_all);
    storeLog("port.is_enabled                   ", rx.msg.port.is_enabled);
    storeLog("port.format                       ", rx.msg.port.format);
    storeLog("port.buffer_num_min               ", rx.msg.port.buffer_num_min);
    storeLog("port.buffer_size_min              ", rx.msg.port.buffer_size_min);
    storeLog("port.buffer_alignment_min         ", rx.msg.port.buffer_alignment_min);
    storeLog("port.buffer_num_recommended       ", rx.msg.port.buffer_num_recommended);
    storeLog("port.buffer_size_recommended      ", rx.msg.port.buffer_size_recommended);
    storeLog("port.buffer_num                 **", rx.msg.port.buffer_num);
    storeLog("port.buffer_size                **", rx.msg.port.buffer_size);
    storeLog("port.component                    ", rx.msg.port.component);
    storeLog("port.userdata                     ", rx.msg.port.userdata);
    storeLog("port.capabilities                 ", rx.msg.port.capabilities);
    nextline(); 
    storeLog("format.type                       ", rx.msg.format.type);                // rx format
    storeLog("format.encoding                 **", rx.msg.format.encoding);
    storeLog("format.encoding_variant         **", rx.msg.format.encoding_variant);
    storeLog("format.es                         ", rx.msg.format.es);
    storeLog("format.bitrate                  **", rx.msg.format.bitrate);
    storeLog("format.flags                      ", rx.msg.format.flags);
    storeLog("format.extradata_size             ", rx.msg.format.extradata_size);
    storeLog("format.extradata                  ", rx.msg.format.extradata);
    nextline(); 
    storeLog("es.video.width                    ", rx.msg.es.video.width);             // rx ES
    storeLog("es.video.height                   ", rx.msg.es.video.height);
    storeLog("es.video.crop.x                   ", rx.msg.es.video.crop.x);
    storeLog("es.video.crop.y                   ", rx.msg.es.video.crop.y);
    storeLog("es.video.crop.width               ", rx.msg.es.video.crop.width);
    storeLog("es.video.crop.height              ", rx.msg.es.video.crop.height);
    storeLog("es.video.frame_rate.num           ", rx.msg.es.video.frame_rate.num);
    storeLog("es.video.frame_rate.den           ", rx.msg.es.video.frame_rate.den);
    storeLog("es.video.par.num                  ", rx.msg.es.video.par.num);
    storeLog("es.video.par.den                  ", rx.msg.es.video.par.den);
    storeLog("es.video.color_space              ", rx.msg.es.video.color_space);
    nextline();
    storeMsg("extradata                         ", rx.msg.extradata, rx.msg.format.extradata_size);     // rx extradata
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_enableComponent( const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply_Msg& rx)
{
    nextline();
    storeLog("hdr.magic                         ", tx.hdr.magic);                                   // tx HEADER
    storeLog("hdr.type                        **", tx.hdr.type);
    storeLog("hdr.control_service               ", tx.hdr.control_service);
    storeLog("hdr.context                       ", tx.hdr.context);
    storeLog("hdr.status                        ", tx.hdr.status);
    storeLog("hdr.padding                       ", tx.hdr.padding);
    nextline();
    storeLog("msg.component_handle           **", tx.msg.component_handle);                       // tx BODY
    nextline();
    storeLog("hdr.magic                         ", rx.hdr.magic);                                   // rx HEADER
    storeLog("hdr.type                        **", rx.hdr.type);
    storeLog("hdr.control_service               ", rx.hdr.control_service);
    storeLog("hdr.context                       ", rx.hdr.context);
    storeLog("hdr.status                      **", rx.hdr.status);
    storeLog("hdr.padding                       ", rx.hdr.padding);
    nextline();
    storeLog("msg.status                     **", rx.msg.status);                                 // rx BODY
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_enablePort( const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx)
{
    nextline();
    storeLog("hdr.magic                         ", tx.hdr.magic);                                   // tx HEADER
    storeLog("hdr.type                        **", tx.hdr.type);
    storeLog("hdr.control_service               ", tx.hdr.control_service);
    storeLog("hdr.context                       ", tx.hdr.context);
    storeLog("hdr.status                        ", tx.hdr.status);
    storeLog("hdr.padding                       ", tx.hdr.padding);
    nextline();
    storeLog("msg.component_handle           **", tx.msg.component_handle);                       // tx BODY
    storeLog("msg.port_handle                **", tx.msg.port_handle);
    storeLog("msg.action                       ", tx.msg.action);
    nextline();
    storeLog("port.priv                         ", tx.msg.port.priv);
    storeLog("port.name                         ", tx.msg.port.name);
    storeLog("port.type                         ", tx.msg.port.type);
    storeLog("port.index                        ", tx.msg.port.index);
    storeLog("port.index_all                    ", tx.msg.port.index_all);
    storeLog("port.is_enabled                   ", tx.msg.port.is_enabled);
    storeLog("port.format                       ", tx.msg.port.format);
    storeLog("port.buffer_num_min               ", tx.msg.port.buffer_num_min);
    storeLog("port.buffer_size_min              ", tx.msg.port.buffer_size_min);
    storeLog("port.buffer_alignment_min         ", tx.msg.port.buffer_alignment_min);
    storeLog("port.buffer_num_recommended       ", tx.msg.port.buffer_num_recommended);
    storeLog("port.buffer_size_recommended      ", tx.msg.port.buffer_size_recommended);
    storeLog("port.buffer_num                 **", tx.msg.port.buffer_num);
    storeLog("port.buffer_size                **", tx.msg.port.buffer_size);
    storeLog("port.component                    ", tx.msg.port.component);
    storeLog("port.userdata                     ", tx.msg.port.userdata);
    storeLog("port.capabilities                 ", tx.msg.port.capabilities);
    nextline();
    storeLog("hdr.magic                         ", rx.hdr.magic);                                   // rx HEADER
    storeLog("hdr.type                        **", rx.hdr.type);
    storeLog("hdr.control_service               ", rx.hdr.control_service);
    storeLog("hdr.context                       ", rx.hdr.context);
    storeLog("hdr.status                      **", rx.hdr.status);
    storeLog("hdr.padding                       ", rx.hdr.padding);
    nextline();
    storeLog("msg.status                     **", rx.msg.status);                                 // rx BODY
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_setZeroCopyMode( const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply_Msg& rx)
{
    nextline();                                                         // tx HEADER
    storeLog("hdr.magic                         ", tx.hdr.magic);
    storeLog("hdr.type                        **", tx.hdr.type);
    storeLog("hdr.control_service               ", tx.hdr.control_service);
    storeLog("hdr.context                       ", tx.hdr.context);
    storeLog("hdr.status                        ", tx.hdr.status);
    storeLog("hdr.padding                       ", tx.hdr.padding);
    nextline();                                                         // tx BODY
    storeLog("msg.component_handle           **", tx.msg.component_handle);
    storeLog("msg.port_handle                **", tx.msg.port_handle);
    storeLog("msg.id                         **", tx.msg.id);
    storeLog("msg.size                       **", tx.msg.size);
    nextline(); 
    storeMsg("msg.value                        ", tx.msg.value, tx.msg.size);             // opaque parameter payload
    nextline();                                                         // rx HEADER
    storeLog("hdr.magic                         ", rx.hdr.magic);
    storeLog("hdr.type                        **", rx.hdr.type);
    storeLog("hdr.control_service               ", rx.hdr.control_service);
    storeLog("hdr.context                       ", rx.hdr.context);
    storeLog("hdr.status                      **", rx.hdr.status);
    storeLog("hdr.padding                       ", rx.hdr.padding);
    nextline();                                                         
    storeLog("msg.status                     **", rx.msg.status);         // rx BODY
}

void CH264Decoder::Log_BufferFromHost( const MMAL_Buffer_From_Host_Msg& rx)
{
    nextline();
    storeLog("hdr.magic                         ", rx.hdr.magic);                          // RX HEADER
    storeLog("hdr.type                          ", rx.hdr.type);
    storeLog("hdr.control_service               ", rx.hdr.control_service);
    storeLog("hdr.context                       ", rx.hdr.context);
    storeLog("hdr.status                        ", rx.hdr.status);
    storeLog("hdr.padding                       ", rx.hdr.padding);
    nextline();
    storeLog("drvbuf.magic                      ", rx.msg.drvbuf.magic);                  // drvbuf
    storeLog("drvbuf.component_handle           ", rx.msg.drvbuf.component_handle);
    storeLog("drvbuf.port_handle                ", rx.msg.drvbuf.port_handle);
    storeLog("drvbuf.client_context             ", rx.msg.drvbuf.client_context);
//  nextline();
//  storeMsg("RAW drvbuf                        ", &rx.msg.drvbuf, sizeof(rx.msg.drvbuf));            // raw dump (for wire verification)
    nextline();
    storeLog("drvbuf_ref.magic                  ", rx.msg.drvbuf_ref.magic);              // drvbuf_ref
    storeLog("drvbuf_ref.component_handle       ", rx.msg.drvbuf_ref.component_handle);
    storeLog("drvbuf_ref.port_handle            ", rx.msg.drvbuf_ref.port_handle);
    storeLog("drvbuf_ref.client_context         ", rx.msg.drvbuf_ref.client_context);
//  nextline();
//  storeMsg("RAW drvbuf_ref                    ", &rx.msg.drvbuf_ref, sizeof(rx.msg.drvbuf_ref));
    nextline();
    storeLog("buffer_header.next                       ", rx.msg.buffer_header.next);                // buffer_header
    storeLog("buffer_header.priv                       ", rx.msg.buffer_header.priv);
    storeLog("buffer_header.cmd                        ", rx.msg.buffer_header.cmd);
    storeLog("buffer_header.data                       ", rx.msg.buffer_header.data);
    storeLog("buffer_header.alloc_size                 ", rx.msg.buffer_header.alloc_size);
    storeLog("buffer_header.length                     ", rx.msg.buffer_header.length);
    storeLog("buffer_header.offset                     ", rx.msg.buffer_header.offset);
    storeLog("buffer_header.flags                      ", rx.msg.buffer_header.flags);
    storeLog("buffer_header.pts_lo                     ", rx.msg.buffer_header.pts_lo);
    storeLog("buffer_header.pts_hi                     ", rx.msg.buffer_header.pts_hi);
    storeLog("buffer_header.dts_lo                     ", rx.msg.buffer_header.dts_lo);
    storeLog("buffer_header.dts_hi                     ", rx.msg.buffer_header.dts_hi);
    storeLog("buffer_header.type                       ", rx.msg.buffer_header.type);
    storeLog("buffer_header.user_data                  ", rx.msg.buffer_header.user_data);
//  nextline();
//  storeMsg("RAW buffer_header                 ", &rx.msg.buffer_header, sizeof(rx.msg.buffer_header));
    nextline();
    storeLog("type_specific.planes              ", rx.msg.buffer_header_type_specific.u.video.planes);    // buffer_header_type_specific (video)
    nextline();
    storeMsg("type_specific.offset[]            ", rx.msg.buffer_header_type_specific.u.video.offset, sizeof(rx.msg.buffer_header_type_specific.u.video.offset));
    nextline();
    storeMsg("type_specific.pitch[]             ", rx.msg.buffer_header_type_specific.u.video.pitch, sizeof(rx.msg.buffer_header_type_specific.u.video.pitch));
    nextline();
    storeLog("type_specific.flags               ", rx.msg.buffer_header_type_specific.u.video.flags);
//  nextline();
//  storeMsg("RAW type_specific                 ", &rx.msg.buffer_header_type_specific, sizeof(rx.msg.buffer_header_type_specific));
    nextline();
    storeLog("is_zero_copy                      ", (u32)rx.msg.is_zero_copy); // flags
    storeLog("has_reference                     ", (u32)rx.msg.has_reference);
    storeLog("payload_in_message                ", rx.msg.payload_in_message);
    nextline();
    storeMsg("RAW short_data                    ", rx.msg.short_data, sizeof(rx.msg.short_data)); // short_data
}
#endif
// ---------------------------------------------------------------------------------------------------------------------------------------------------
/*
void CH264Decoder::Log_BufferBody(  const mmal_msg_buffer_from_host_wire32& msg )
{
    storeLog("drvbuf.magic                  ", msg.drvbuf.magic);
    storeLog("drvbuf.component_handle       ", msg.drvbuf.component_handle);
    storeLog("drvbuf.port_handle            ", msg.drvbuf.port_handle);
    storeLog("drvbuf.client_context         ", msg.drvbuf.client_context);

    storeMsg(msg.drvbuf_ref, sizeof(msg.drvbuf_ref), "drvbuf_ref");

    storeLog("buffer_header.next            ", msg.buffer_header.next);
    storeLog("buffer_header.priv            ", msg.buffer_header.priv);
    storeLog("buffer_header.cmd             ", msg.buffer_header.cmd);
    storeLog("buffer_header.data            ", msg.buffer_header.data);
    storeLog("buffer_header.alloc_size      ", msg.buffer_header.alloc_size);
    storeLog("buffer_header.length          ", msg.buffer_header.length);
    storeLog("buffer_header.offset          ", msg.buffer_header.offset);
    storeLog("buffer_header.flags           ", msg.buffer_header.flags);
    storeLog("buffer_header.pts_lo          ", msg.buffer_header.pts_lo);
    storeLog("buffer_header.pts_hi          ", msg.buffer_header.pts_hi);
    storeLog("buffer_header.dts_lo          ", msg.buffer_header.dts_lo);
    storeLog("buffer_header.dts_hi          ", msg.buffer_header.dts_hi);
    storeLog("buffer_header.type            ", msg.buffer_header.type);
    storeLog("buffer_header.user_data       ", msg.buffer_header.user_data);

    storeMsg(msg.type_specific, sizeof(msg.type_specific), "type_specific");

    storeLog("is_zero_copy                  ", (u32)msg.is_zero_copy);
    storeLog("has_reference                 ", (u32)msg.has_reference);
    storeLog("payload_in_message            ", (u32)msg.payload_in_message);

    storeMsg(msg.short_data, sizeof(msg.short_data), "short_data");
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CH264Decoder::Log_queueBufferFromHost( const MMAL_Buffer_From_Host_Msg& tx )
{
    storeLog("sizeof(tx)                   ", (u32)sizeof(tx));

    storeLog("\nTX.hdr.magic                 ", tx.hdr.magic);
    storeLog("hdr.type                  ", tx.hdr.type);
    storeLog("hdr.context               ", tx.hdr.context);
    storeLog("hdr.status                ", tx.hdr.status);

    storeLog("\ndrvbuf.magic                 ", tx.msg.drvbuf.magic);
    storeLog("drvbuf.component_handle      ", tx.msg.drvbuf.component_handle);
    storeLog("drvbuf.port_handle           ", tx.msg.drvbuf.port_handle);
    storeLog("drvbuf.client_context        ", tx.msg.drvbuf.client_context);

    storeMsg(tx.msg.drvbuf_ref, sizeof(tx.msg.drvbuf_ref), "drvbuf_ref");

    storeLog("buffer_header.data           ", tx.msg.buffer_header.data);
    storeLog("buffer_header.alloc_size     ", tx.msg.buffer_header.alloc_size);
    storeLog("buffer_header.length         ", tx.msg.buffer_header.length);
    storeLog("buffer_header.offset         ", tx.msg.buffer_header.offset);
    storeLog("buffer_header.flags          ", tx.msg.buffer_header.flags);
    storeLog("buffer_header.pts_lo         ", tx.msg.buffer_header.pts_lo);
    storeLog("buffer_header.pts_hi         ", tx.msg.buffer_header.pts_hi);
    storeLog("buffer_header.dts_lo         ", tx.msg.buffer_header.dts_lo);
    storeLog("buffer_header.dts_hi         ", tx.msg.buffer_header.dts_hi);

    storeMsg(tx.msg.type_specific, sizeof(tx.msg.type_specific), "type_specific");

    storeLog("is_zero_copy                 ", (u32)tx.msg.is_zero_copy);
    storeLog("has_reference                ", (u32)tx.msg.has_reference);
    storeLog("payload_in_message           ", (u32)tx.msg.payload_in_message);

    storeMsg(tx.msg.short_data, sizeof(tx.msg.short_data), "short_data");
}
*/
// ---------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------------------------