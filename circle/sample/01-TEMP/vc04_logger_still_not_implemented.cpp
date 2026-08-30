//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_openService(const SERVICE_CREATION_T &src)
{
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "version                           ", src.version.version);
    storeLog( MY_BUFFER, MY_INDEX, "version_min                       ", src.version.version_min);
    storeLog( MY_BUFFER, MY_INDEX, "service_id                        ", src.service_id);
    storeLog( MY_BUFFER, MY_INDEX, "connection                        ", (u32)(uintptr_t)src.connection);
    storeLog( MY_BUFFER, MY_INDEX, "rx_fifo_size                      ", src.rx_fifo_size);
    storeLog( MY_BUFFER, MY_INDEX, "tx_fifo_size                      ", src.tx_fifo_size);
    storeLog( MY_BUFFER, MY_INDEX, "callback                          ", (u32)(uintptr_t)src.callback);
    storeLog( MY_BUFFER, MY_INDEX, "callback_param                    ", (u32)(uintptr_t)src.callback_param);
    storeLog( MY_BUFFER, MY_INDEX, "want_unaligned_bulk_rx            ", src.want_unaligned_bulk_rx);
    storeLog( MY_BUFFER, MY_INDEX, "want_unaligned_bulk_tx            ", src.want_unaligned_bulk_tx);
    storeLog( MY_BUFFER, MY_INDEX, "want_crc                          ", src.want_crc);                            // complete
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_createComponent( const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply    & rx)
{
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", tx.hdr.magic);                   // tx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", tx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", tx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", tx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                        ", tx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", tx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "msg.client_component             ", tx.msg.client_component);       // tx BODY
    nextline( MY_BUFFER, MY_INDEX ); 
    storeMsg( MY_BUFFER, MY_INDEX, "msg.name                         ", tx.msg.name, sizeof(tx.msg.name));           // shows name[128] exactly as sent
    storeLog( MY_BUFFER, MY_INDEX, "msg.pid                          ", tx.msg.pid);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", rx.hdr.magic);                   // rx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", rx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", rx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", rx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                      **", rx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", rx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "msg.status                     **", rx.msg.status);                 // rx BODY
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", rx.msg.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "msg.input_num                   *", rx.msg.input_num);
    storeLog( MY_BUFFER, MY_INDEX, "msg.output_num                  *", rx.msg.output_num);
    storeLog( MY_BUFFER, MY_INDEX, "msg.clock_num                   *", rx.msg.clock_num);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_getPortInfo( const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply    & rx)
{
    nextline( MY_BUFFER, MY_INDEX );                                                             // tx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", tx.hdr.magic);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", tx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", tx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", tx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                        ", tx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", tx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );                                                             // tx BODY
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", tx.msg.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_type                    ", tx.msg.port_type);
    storeLog( MY_BUFFER, MY_INDEX, "msg.index                        ", tx.msg.index);
    nextline( MY_BUFFER, MY_INDEX );                                                             // rx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", rx.hdr.magic);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", rx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", rx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", rx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                      **", rx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", rx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );                                                             // rx BODY
    storeLog( MY_BUFFER, MY_INDEX, "msg.status                     **", rx.msg.status);
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", rx.msg.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_type                    ", rx.msg.port_type);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_index                   ", rx.msg.port_index);
    storeLog( MY_BUFFER, MY_INDEX, "msg.found                        ", rx.msg.found);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_handle                **", rx.msg.port_handle);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "port.priv                         ", rx.msg.port.priv);              // rx PORT //
    storeLog( MY_BUFFER, MY_INDEX, "port.name                         ", rx.msg.port.name);
    storeLog( MY_BUFFER, MY_INDEX, "port.type                         ", rx.msg.port.type);
    storeLog( MY_BUFFER, MY_INDEX, "port.index                        ", rx.msg.port.index);
    storeLog( MY_BUFFER, MY_INDEX, "port.index_all                    ", rx.msg.port.index_all);
    storeLog( MY_BUFFER, MY_INDEX, "port.is_enabled                   ", rx.msg.port.is_enabled);
    storeLog( MY_BUFFER, MY_INDEX, "port.format                       ", rx.msg.port.format);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_min               ", rx.msg.port.buffer_num_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_min              ", rx.msg.port.buffer_size_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_alignment_min         ", rx.msg.port.buffer_alignment_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_recommended       ", rx.msg.port.buffer_num_recommended);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_recommended      ", rx.msg.port.buffer_size_recommended);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num                 **", rx.msg.port.buffer_num);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size                **", rx.msg.port.buffer_size);
    storeLog( MY_BUFFER, MY_INDEX, "port.component                    ", rx.msg.port.component);
    storeLog( MY_BUFFER, MY_INDEX, "port.userdata                     ", rx.msg.port.userdata);
    storeLog( MY_BUFFER, MY_INDEX, "port.capabilities                 ", rx.msg.port.capabilities);
    nextline( MY_BUFFER, MY_INDEX ); 
    storeLog( MY_BUFFER, MY_INDEX, "format.type                       ", rx.msg.format.type);            // rx FORMAT //
    storeLog( MY_BUFFER, MY_INDEX, "format.encoding                 **", rx.msg.format.encoding);
    storeLog( MY_BUFFER, MY_INDEX, "format.encoding_variant         **", rx.msg.format.encoding_variant);
    storeLog( MY_BUFFER, MY_INDEX, "format.es                         ", rx.msg.format.es);
    storeLog( MY_BUFFER, MY_INDEX, "format.bitrate                  **", rx.msg.format.bitrate);
    storeLog( MY_BUFFER, MY_INDEX, "format.flags                      ", rx.msg.format.flags);
    storeLog( MY_BUFFER, MY_INDEX, "format.extradata_size             ", rx.msg.format.extradata_size);
    storeLog( MY_BUFFER, MY_INDEX, "format.extradata                  ", rx.msg.format.extradata);
    nextline( MY_BUFFER, MY_INDEX ); 
    storeLog( MY_BUFFER, MY_INDEX, "es.video.width                    ", rx.msg.es.video.width);         // rx ES
    storeLog( MY_BUFFER, MY_INDEX, "es.video.height                   ", rx.msg.es.video.height);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.x                   ", rx.msg.es.video.crop.x);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.y                   ", rx.msg.es.video.crop.y);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.width               ", rx.msg.es.video.crop.width);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.height              ", rx.msg.es.video.crop.height);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.frame_rate.num           ", rx.msg.es.video.frame_rate.num);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.frame_rate.den           ", rx.msg.es.video.frame_rate.den);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.par.num                  ", rx.msg.es.video.par.num);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.par.den                  ", rx.msg.es.video.par.den);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.color_space              ", rx.msg.es.video.color_space);
    nextline( MY_BUFFER, MY_INDEX );
    storeMsg( MY_BUFFER, MY_INDEX, "extradata", rx.msg.extradata, rx.msg.format.extradata_size);    // rx EXTRADATA
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_setPortInfo( const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply    & rx)
{
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", tx.hdr.magic);                       // tx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", tx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", tx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", tx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                        ", tx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", tx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", tx.msg.component_handle);           // tx BODY
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_type                    ", tx.msg.port_type);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_index                   ", tx.msg.port_index);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "port.priv                         ", tx.msg.port.priv);                       // Private member used by the framework
    storeLog( MY_BUFFER, MY_INDEX, "port.name                         ", tx.msg.port.name);                       // Port name. Used for debugging purposes (RO)
    storeLog( MY_BUFFER, MY_INDEX, "port.type                         ", tx.msg.port.type);                       // Type of the port (RO) enum mmal_port_type
    storeLog( MY_BUFFER, MY_INDEX, "port.index                        ", tx.msg.port.index);                      // Index of the port in its type list (RO)
    storeLog( MY_BUFFER, MY_INDEX, "port.index_all                    ", tx.msg.port.index_all);                  // Index of the port in the list of all ports (RO)
    storeLog( MY_BUFFER, MY_INDEX, "port.is_enabled                   ", tx.msg.port.is_enabled);                 // Indicates whether the port is enabled or not (RO)
    storeLog( MY_BUFFER, MY_INDEX, "port.format                       ", tx.msg.port.format);                     // Format of the elementary stream
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_min               ", tx.msg.port.buffer_num_min);             // Minimum number of buffers the port requires (RO).  This is set by the component.
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_min              ", tx.msg.port.buffer_size_min);            // Minimum size of buffers the port requires (RO).  This is set by the component.
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_alignment_min         ", tx.msg.port.buffer_alignment_min);       // Minimum alignment requirement for the buffers (RO).  A value of zero means no special alignment requirements.  This is set by the component.
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_recommended       ", tx.msg.port.buffer_num_recommended);     // Number of buffers the port recommends for optimal performance (RO).  A value of zero means no special recommendation.  This is set by the component.
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_recommended      ", tx.msg.port.buffer_size_recommended);    // Size of buffers the port recommends for optimal performance (RO).  A value of zero means no special recommendation.  This is set by the component.
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num                 **", tx.msg.port.buffer_num);                 // Actual number of buffers the port will use. This is set by the client.
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size                **", tx.msg.port.buffer_size);                // Actual maximum size of the buffers that will be sent to the port. This is set by the client.
    storeLog( MY_BUFFER, MY_INDEX, "port.component                    ", tx.msg.port.component);                  // Component this port belongs to (Read Only)
    storeLog( MY_BUFFER, MY_INDEX, "port.userdata                     ", tx.msg.port.userdata);                   // Field reserved for use by the client
    storeLog( MY_BUFFER, MY_INDEX, "port.capabilities                 ", tx.msg.port.capabilities);               // Flags describing the capabilities of a port (RO).  Bitwise combination of \ref portcapabilities "Port capabilities" values.
    nextline( MY_BUFFER, MY_INDEX ); 
    storeLog( MY_BUFFER, MY_INDEX, "format.type                       ", tx.msg.format.type);                     // enum mmal_es_type
    storeLog( MY_BUFFER, MY_INDEX, "format.encoding                 **", tx.msg.format.encoding);                 // FourCC specifying encoding of the elementary stream.
    storeLog( MY_BUFFER, MY_INDEX, "format.encoding_variant         **", tx.msg.format.encoding_variant);         // FourCC specifying the specific encoding variant of the elementary stream.
    storeLog( MY_BUFFER, MY_INDEX, "format.es                         ", tx.msg.format.es);                       // Type specific information for the elementary stream
    storeLog( MY_BUFFER, MY_INDEX, "format.bitrate                  **", tx.msg.format.bitrate);                  // Bitrate in bits per second
    storeLog( MY_BUFFER, MY_INDEX, "format.flags                      ", tx.msg.format.flags);                    // Flags describing properties of the elementary stream.
    storeLog( MY_BUFFER, MY_INDEX, "format.extradata_size             ", tx.msg.format.extradata_size);           // Size of the codec specific data
    storeLog( MY_BUFFER, MY_INDEX, "format.extradata                  ", tx.msg.format.extradata);                // Codec specific data
    nextline( MY_BUFFER, MY_INDEX ); 
    storeLog( MY_BUFFER, MY_INDEX, "es.video.width                    ", tx.msg.es.video.width);                  // Width of frame in pixels
    storeLog( MY_BUFFER, MY_INDEX, "es.video.height                   ", tx.msg.es.video.height);                 // Height of frame in rows of pixels
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.x                   ", tx.msg.es.video.crop.x);                 // Visible region of the frame  // x coordinate (from left)
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.y                   ", tx.msg.es.video.crop.y);                                                 // y coordinate (from top)
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.width               ", tx.msg.es.video.crop.width);                                             // width
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.height              ", tx.msg.es.video.crop.height);                                            // height
    storeLog( MY_BUFFER, MY_INDEX, "es.video.frame_rate.num           ", tx.msg.es.video.frame_rate.num);         // Frame rate                   // Numerator
    storeLog( MY_BUFFER, MY_INDEX, "es.video.frame_rate.den           ", tx.msg.es.video.frame_rate.den);                                         // Denominator
    storeLog( MY_BUFFER, MY_INDEX, "es.video.par.num                  ", tx.msg.es.video.par.num);                // Pixel aspect ratio           // Numerator
    storeLog( MY_BUFFER, MY_INDEX, "es.video.par.den                  ", tx.msg.es.video.par.den);                                                // Denominator
    storeLog( MY_BUFFER, MY_INDEX, "es.video.color_space              ", tx.msg.es.video.color_space);            // FourCC specifying the color space of the video stream. See the MmalColorSpace "pre-defined color spaces" for some examples.
    nextline( MY_BUFFER, MY_INDEX ); 
    storeMsg( MY_BUFFER, MY_INDEX, "extradata                         ", tx.msg.extradata, tx.msg.format.extradata_size);     // tx extradata
    nextline( MY_BUFFER, MY_INDEX );                                                                 // rx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", rx.hdr.magic);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", rx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", rx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", rx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                      **", rx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", rx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );                                                                 // rx BODY
    storeLog( MY_BUFFER, MY_INDEX, "msg.status                     **", rx.msg.status);
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", rx.msg.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_type                    ", rx.msg.port_type);
    storeLog( MY_BUFFER, MY_INDEX, "msg.index                        ", rx.msg.index);
    storeLog( MY_BUFFER, MY_INDEX, "msg.found                        ", rx.msg.found);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_handle                **", rx.msg.port_handle);
    nextline( MY_BUFFER, MY_INDEX );                                                                 // rx port
    storeLog( MY_BUFFER, MY_INDEX, "port.priv                         ", rx.msg.port.priv);
    storeLog( MY_BUFFER, MY_INDEX, "port.name                         ", rx.msg.port.name);
    storeLog( MY_BUFFER, MY_INDEX, "port.type                         ", rx.msg.port.type);
    storeLog( MY_BUFFER, MY_INDEX, "port.index                        ", rx.msg.port.index);
    storeLog( MY_BUFFER, MY_INDEX, "port.index_all                    ", rx.msg.port.index_all);
    storeLog( MY_BUFFER, MY_INDEX, "port.is_enabled                   ", rx.msg.port.is_enabled);
    storeLog( MY_BUFFER, MY_INDEX, "port.format                       ", rx.msg.port.format);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_min               ", rx.msg.port.buffer_num_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_min              ", rx.msg.port.buffer_size_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_alignment_min         ", rx.msg.port.buffer_alignment_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_recommended       ", rx.msg.port.buffer_num_recommended);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_recommended      ", rx.msg.port.buffer_size_recommended);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num                 **", rx.msg.port.buffer_num);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size                **", rx.msg.port.buffer_size);
    storeLog( MY_BUFFER, MY_INDEX, "port.component                    ", rx.msg.port.component);
    storeLog( MY_BUFFER, MY_INDEX, "port.userdata                     ", rx.msg.port.userdata);
    storeLog( MY_BUFFER, MY_INDEX, "port.capabilities                 ", rx.msg.port.capabilities);
    nextline( MY_BUFFER, MY_INDEX ); 
    storeLog( MY_BUFFER, MY_INDEX, "format.type                       ", rx.msg.format.type);                // rx format
    storeLog( MY_BUFFER, MY_INDEX, "format.encoding                 **", rx.msg.format.encoding);
    storeLog( MY_BUFFER, MY_INDEX, "format.encoding_variant         **", rx.msg.format.encoding_variant);
    storeLog( MY_BUFFER, MY_INDEX, "format.es                         ", rx.msg.format.es);
    storeLog( MY_BUFFER, MY_INDEX, "format.bitrate                  **", rx.msg.format.bitrate);
    storeLog( MY_BUFFER, MY_INDEX, "format.flags                      ", rx.msg.format.flags);
    storeLog( MY_BUFFER, MY_INDEX, "format.extradata_size             ", rx.msg.format.extradata_size);
    storeLog( MY_BUFFER, MY_INDEX, "format.extradata                  ", rx.msg.format.extradata);
    nextline( MY_BUFFER, MY_INDEX ); 
    storeLog( MY_BUFFER, MY_INDEX, "es.video.width                    ", rx.msg.es.video.width);             // rx ES
    storeLog( MY_BUFFER, MY_INDEX, "es.video.height                   ", rx.msg.es.video.height);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.x                   ", rx.msg.es.video.crop.x);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.y                   ", rx.msg.es.video.crop.y);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.width               ", rx.msg.es.video.crop.width);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.crop.height              ", rx.msg.es.video.crop.height);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.frame_rate.num           ", rx.msg.es.video.frame_rate.num);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.frame_rate.den           ", rx.msg.es.video.frame_rate.den);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.par.num                  ", rx.msg.es.video.par.num);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.par.den                  ", rx.msg.es.video.par.den);
    storeLog( MY_BUFFER, MY_INDEX, "es.video.color_space              ", rx.msg.es.video.color_space);
    nextline( MY_BUFFER, MY_INDEX );
    storeMsg( MY_BUFFER, MY_INDEX, "extradata                         ", rx.msg.extradata, rx.msg.format.extradata_size);     // rx extradata
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_enableComponentMMAL( const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply    & rx)
{
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", tx.hdr.magic);                                   // tx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", tx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", tx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", tx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                        ", tx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", tx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", tx.msg.component_handle);                       // tx BODY
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", rx.hdr.magic);                                   // rx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", rx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", rx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", rx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                      **", rx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", rx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "msg.status                     **", rx.msg.status);                                 // rx BODY
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_enablePort( const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx)
{
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", tx.hdr.magic);                                   // tx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", tx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", tx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", tx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                        ", tx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", tx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", tx.msg.component_handle);                       // tx BODY
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_handle                **", tx.msg.port_handle);
    storeLog( MY_BUFFER, MY_INDEX, "msg.action                       ", tx.msg.action);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "port.priv                         ", tx.msg.port.priv);
    storeLog( MY_BUFFER, MY_INDEX, "port.name                         ", tx.msg.port.name);
    storeLog( MY_BUFFER, MY_INDEX, "port.type                         ", tx.msg.port.type);
    storeLog( MY_BUFFER, MY_INDEX, "port.index                        ", tx.msg.port.index);
    storeLog( MY_BUFFER, MY_INDEX, "port.index_all                    ", tx.msg.port.index_all);
    storeLog( MY_BUFFER, MY_INDEX, "port.is_enabled                   ", tx.msg.port.is_enabled);
    storeLog( MY_BUFFER, MY_INDEX, "port.format                       ", tx.msg.port.format);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_min               ", tx.msg.port.buffer_num_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_min              ", tx.msg.port.buffer_size_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_alignment_min         ", tx.msg.port.buffer_alignment_min);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num_recommended       ", tx.msg.port.buffer_num_recommended);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size_recommended      ", tx.msg.port.buffer_size_recommended);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_num                 **", tx.msg.port.buffer_num);
    storeLog( MY_BUFFER, MY_INDEX, "port.buffer_size                **", tx.msg.port.buffer_size);
    storeLog( MY_BUFFER, MY_INDEX, "port.component                    ", tx.msg.port.component);
    storeLog( MY_BUFFER, MY_INDEX, "port.userdata                     ", tx.msg.port.userdata);
    storeLog( MY_BUFFER, MY_INDEX, "port.capabilities                 ", tx.msg.port.capabilities);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", rx.hdr.magic);                                   // rx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", rx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", rx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", rx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                      **", rx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", rx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "msg.status                     **", rx.msg.status);                                 // rx BODY
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_setZeroCopyModeMMAL( const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply    & rx)
{
    nextline( MY_BUFFER, MY_INDEX );                                                         // tx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", tx.hdr.magic);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", tx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", tx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", tx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                        ", tx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", tx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );                                                         // tx BODY
    storeLog( MY_BUFFER, MY_INDEX, "msg.component_handle           **", tx.msg.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "msg.port_handle                **", tx.msg.port_handle);
    storeLog( MY_BUFFER, MY_INDEX, "msg.id                         **", tx.msg.id);
    storeLog( MY_BUFFER, MY_INDEX, "msg.size                       **", tx.msg.size);
    nextline( MY_BUFFER, MY_INDEX ); 
    storeMsg( MY_BUFFER, MY_INDEX, "msg.value                        ", tx.msg.value, tx.msg.size);             // opaque parameter payload
    nextline( MY_BUFFER, MY_INDEX );                                                         // rx HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", rx.hdr.magic);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                        **", rx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", rx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", rx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                      **", rx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", rx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );                                                         
    storeLog( MY_BUFFER, MY_INDEX, "msg.status                     **", rx.msg.status);         // rx BODY
}

void CKernel::Log_BufferFromHost( const MMAL_Buffer_From_Host_Msg& rx)
{
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "hdr.magic                         ", rx.hdr.magic);                          // RX HEADER
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                          ", rx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.control_service               ", rx.hdr.control_service);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context                       ", rx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                        ", rx.hdr.status);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.padding                       ", rx.hdr.padding);
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.magic                      ", rx.msg.drvbuf.magic);                  // drvbuf
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.component_handle           ", rx.msg.drvbuf.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.port_handle                ", rx.msg.drvbuf.port_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.client_context             ", rx.msg.drvbuf.client_context);
//  nextline( MY_BUFFER, MY_INDEX );
//  storeMsg( MY_BUFFER, MY_INDEX, "RAW drvbuf                        ", &rx.msg.drvbuf, sizeof(rx.msg.drvbuf));            // raw dump (for wire verification)
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf_ref.magic                  ", rx.msg.drvbuf_ref.magic);              // drvbuf_ref
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf_ref.component_handle       ", rx.msg.drvbuf_ref.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf_ref.port_handle            ", rx.msg.drvbuf_ref.port_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf_ref.client_context         ", rx.msg.drvbuf_ref.client_context);
//  nextline( MY_BUFFER, MY_INDEX );
//  storeMsg( MY_BUFFER, MY_INDEX, "RAW drvbuf_ref                    ", &rx.msg.drvbuf_ref, sizeof(rx.msg.drvbuf_ref));
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.next                       ", rx.msg.buffer_header.next);                // buffer_header
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.priv                       ", rx.msg.buffer_header.priv);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.cmd                        ", rx.msg.buffer_header.cmd);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.data                       ", rx.msg.buffer_header.data);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.alloc_size                 ", rx.msg.buffer_header.alloc_size);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.length                     ", rx.msg.buffer_header.length);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.offset                     ", rx.msg.buffer_header.offset);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.flags                      ", rx.msg.buffer_header.flags);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.pts_lo                     ", rx.msg.buffer_header.pts_lo);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.pts_hi                     ", rx.msg.buffer_header.pts_hi);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.dts_lo                     ", rx.msg.buffer_header.dts_lo);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.dts_hi                     ", rx.msg.buffer_header.dts_hi);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.type                       ", rx.msg.buffer_header.type);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.user_data                  ", rx.msg.buffer_header.user_data);
//  nextline( MY_BUFFER, MY_INDEX );
//  storeMsg( MY_BUFFER, MY_INDEX, "RAW buffer_header                 ", &rx.msg.buffer_header, sizeof(rx.msg.buffer_header));
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "type_specific.planes              ", rx.msg.buffer_header_type_specific.u.video.planes);    // buffer_header_type_specific (video)
    nextline( MY_BUFFER, MY_INDEX );
    storeMsg( MY_BUFFER, MY_INDEX, "type_specific.offset[]            ", rx.msg.buffer_header_type_specific.u.video.offset, sizeof(rx.msg.buffer_header_type_specific.u.video.offset));
    nextline( MY_BUFFER, MY_INDEX );
    storeMsg( MY_BUFFER, MY_INDEX, "type_specific.pitch[]             ", rx.msg.buffer_header_type_specific.u.video.pitch, sizeof(rx.msg.buffer_header_type_specific.u.video.pitch));
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "type_specific.flags               ", rx.msg.buffer_header_type_specific.u.video.flags);
//  nextline( MY_BUFFER, MY_INDEX );
//  storeMsg( MY_BUFFER, MY_INDEX, "RAW type_specific                 ", &rx.msg.buffer_header_type_specific, sizeof(rx.msg.buffer_header_type_specific));
    nextline( MY_BUFFER, MY_INDEX );
    storeLog( MY_BUFFER, MY_INDEX, "is_zero_copy                      ", (u32)rx.msg.is_zero_copy); // flags
    storeLog( MY_BUFFER, MY_INDEX, "has_reference                     ", (u32)rx.msg.has_reference);
    storeLog( MY_BUFFER, MY_INDEX, "payload_in_message                ", rx.msg.payload_in_message);
    nextline( MY_BUFFER, MY_INDEX );
    storeMsg( MY_BUFFER, MY_INDEX, "RAW short_data                    ", rx.msg.short_data, sizeof(rx.msg.short_data)); // short_data
}
#endif
// ---------------------------------------------------------------------------------------------------------------------------------------------------
/*
void CKernel::Log_BufferBody(  const mmal_msg_buffer_from_host_wire32& msg )
{
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.magic                  ", msg.drvbuf.magic);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.component_handle       ", msg.drvbuf.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.port_handle            ", msg.drvbuf.port_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.client_context         ", msg.drvbuf.client_context);

    storeMsg(msg.drvbuf_ref, sizeof(msg.drvbuf_ref), "drvbuf_ref");

    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.next            ", msg.buffer_header.next);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.priv            ", msg.buffer_header.priv);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.cmd             ", msg.buffer_header.cmd);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.data            ", msg.buffer_header.data);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.alloc_size      ", msg.buffer_header.alloc_size);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.length          ", msg.buffer_header.length);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.offset          ", msg.buffer_header.offset);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.flags           ", msg.buffer_header.flags);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.pts_lo          ", msg.buffer_header.pts_lo);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.pts_hi          ", msg.buffer_header.pts_hi);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.dts_lo          ", msg.buffer_header.dts_lo);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.dts_hi          ", msg.buffer_header.dts_hi);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.type            ", msg.buffer_header.type);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.user_data       ", msg.buffer_header.user_data);

    storeMsg(msg.type_specific, sizeof(msg.type_specific), "type_specific");

    storeLog( MY_BUFFER, MY_INDEX, "is_zero_copy                  ", (u32)msg.is_zero_copy);
    storeLog( MY_BUFFER, MY_INDEX, "has_reference                 ", (u32)msg.has_reference);
    storeLog( MY_BUFFER, MY_INDEX, "payload_in_message            ", (u32)msg.payload_in_message);

    storeMsg(msg.short_data, sizeof(msg.short_data), "short_data");
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::Log_queueBufferFromHost( const MMAL_Buffer_From_Host_Msg& tx )
{
    storeLog( MY_BUFFER, MY_INDEX, "sizeof(tx)                   ", (u32)sizeof(tx));

    storeLog( MY_BUFFER, MY_INDEX, "TX.hdr.magic                 ", tx.hdr.magic);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.type                  ", tx.hdr.type);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.context               ", tx.hdr.context);
    storeLog( MY_BUFFER, MY_INDEX, "hdr.status                ", tx.hdr.status);

    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.magic                 ", tx.msg.drvbuf.magic);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.component_handle      ", tx.msg.drvbuf.component_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.port_handle           ", tx.msg.drvbuf.port_handle);
    storeLog( MY_BUFFER, MY_INDEX, "drvbuf.client_context        ", tx.msg.drvbuf.client_context);

    storeMsg(tx.msg.drvbuf_ref, sizeof(tx.msg.drvbuf_ref), "drvbuf_ref");

    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.data           ", tx.msg.buffer_header.data);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.alloc_size     ", tx.msg.buffer_header.alloc_size);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.length         ", tx.msg.buffer_header.length);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.offset         ", tx.msg.buffer_header.offset);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.flags          ", tx.msg.buffer_header.flags);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.pts_lo         ", tx.msg.buffer_header.pts_lo);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.pts_hi         ", tx.msg.buffer_header.pts_hi);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.dts_lo         ", tx.msg.buffer_header.dts_lo);
    storeLog( MY_BUFFER, MY_INDEX, "buffer_header.dts_hi         ", tx.msg.buffer_header.dts_hi);

    storeMsg(tx.msg.type_specific, sizeof(tx.msg.type_specific), "type_specific");

    storeLog( MY_BUFFER, MY_INDEX, "is_zero_copy                 ", (u32)tx.msg.is_zero_copy);
    storeLog( MY_BUFFER, MY_INDEX, "has_reference                ", (u32)tx.msg.has_reference);
    storeLog( MY_BUFFER, MY_INDEX, "payload_in_message           ", (u32)tx.msg.payload_in_message);

    storeMsg(tx.msg.short_data, sizeof(tx.msg.short_data), "short_data");
}
*/
// ---------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------------------------