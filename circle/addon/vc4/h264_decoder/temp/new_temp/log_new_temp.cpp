


void            CH264Decoder::storeLog              (   const char* label, 
                                                            u32         value1, 
                                                            u32         value2, 
                                                            u32         value3, 
                                                            u32         value4 )
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
void            CH264Decoder::storeMsg              (   const void* tx_msg, 
                                                            u32         total_size, 
                                                            const char* label)
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


void CH264Decoder::Log_initialize()
{

}

void CH264Decoder::Log_openService(const SERVICE_CREATION_T &src)
{
    MMALstoreLog("version                       ", src.version.version);
    MMALstoreLog("version_min                   ", src.version.version_min);
    MMALstoreLog("service_id                    ", src.service_id);
    MMALstoreLog("connection                    ", (u32)(uintptr_t)src.connection);
    MMALstoreLog("rx_fifo_size                  ", src.rx_fifo_size);
    MMALstoreLog("tx_fifo_size                  ", src.tx_fifo_size);
    MMALstoreLog("callback                      ", (u32)(uintptr_t)src.callback);
    MMALstoreLog("callback_param                ", (u32)(uintptr_t)src.callback_param);
    MMALstoreLog("want_unaligned_bulk_rx        ", src.want_unaligned_bulk_rx);
    MMALstoreLog("want_unaligned_bulk_tx        ", src.want_unaligned_bulk_tx);
    MMALstoreLog("want_crc                      ", src.want_crc);                            // complete
}
/*
void        CH264Decoder::Log_createComponent( const MMAL_Component_Create_Reply_Msg& src)
{
            MMALstoreLog("rx.hdr.magic                  ", src.hdr.magic);
            MMALstoreLog("rx.hdr.type                 **", src.hdr.type);
            MMALstoreLog("rx.hdr.control_service        ", src.hdr.control_service);
            MMALstoreLog("rx.hdr.context                ", src.hdr.context);
            MMALstoreLog("rx.hdr.status               **", src.hdr.status);
            MMALstoreLog("rx.hdr.padding                ", src.hdr.padding);

            MMALstoreLog("rx.body.status              **", src.body.status);
            MMALstoreLog("rx.body.component_handle    **", src.body.component_handle);
            MMALstoreLog("rx.body.input_num             ", src.body.input_num);
            MMALstoreLog("rx.body.output_num            ", src.body.output_num);
            MMALstoreLog("rx.body.clock_num             ", src.body.clock_num);              // complete
}
*/
void CH264Decoder::Log_createComponent( const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply_Msg& rx)
{
    // tx HEADER //
    MMALstoreLog("tx.hdr.magic              ", tx.hdr.magic);
    MMALstoreLog("tx.hdr.type             **", tx.hdr.type);
    MMALstoreLog("tx.hdr.control_service    ", tx.hdr.control_service);
    MMALstoreLog("tx.hdr.context            ", tx.hdr.context);
    MMALstoreLog("tx.hdr.status             ", tx.hdr.status);
    MMALstoreLog("tx.hdr.padding            ", tx.hdr.padding);
    // tx BODY //
    MMALstoreLog("tx.body.client_component  ", tx.body.client_component);
    /* shows name[128] exactly as sent */
    storeMsg(tx.body.name, sizeof(tx.body.name), "tx.body.name");
    MMALstoreLog("tx.body.pid               ", tx.body.pid);
    // rx HEADER //
    MMALstoreLog("rx.hdr.magic              ", rx.hdr.magic);
    MMALstoreLog("rx.hdr.type             **", rx.hdr.type);
    MMALstoreLog("rx.hdr.control_service    ", rx.hdr.control_service);
    MMALstoreLog("rx.hdr.context            ", rx.hdr.context);
    MMALstoreLog("rx.hdr.status           **", rx.hdr.status);
    MMALstoreLog("rx.hdr.padding            ", rx.hdr.padding);
    // rx BODY //
    MMALstoreLog("rx.body.status          **", rx.body.status);
    MMALstoreLog("rx.body.component_handle**", rx.body.component_handle);
    MMALstoreLog("rx.body.input_num         ", rx.body.input_num);
    MMALstoreLog("rx.body.output_num        ", rx.body.output_num);
    MMALstoreLog("rx.body.clock_num         ", rx.body.clock_num);
}
/*
void CH264Decoder::Log_getPortInfo( const MMAL_Port_Info_Get_Reply_Msg& src)
{
            MMALstoreLog("rx.hdr.magic                  ", src.hdr.magic);                   //  looks complete andd correct
            MMALstoreLog("rx.hdr.type                 **", src.hdr.type);
            MMALstoreLog("rx.hdr.control_service        ", src.hdr.control_service);
            MMALstoreLog("rx.hdr.context                ", src.hdr.context);
            MMALstoreLog("rx.hdr.status               **", src.hdr.status);
            MMALstoreLog("rx.hdr.padding                ", src.hdr.padding);

            MMALstoreLog("rx.body.status              **", src.body.status);                 // body
            MMALstoreLog("rx.body.component_handle    **", src.body.component_handle);
            MMALstoreLog("rx.body.port_type             ", src.body.port_type);
            MMALstoreLog("rx.body.port_index            ", src.body.port_index);
            MMALstoreLog("rx.body.found                 ", src.body.found);
            MMALstoreLog("rx.body.port_handle         **", src.body.port_handle);

            MMALstoreLog("port.priv                     ", src.body.port.priv);              // body.port
            MMALstoreLog("port.name                     ", src.body.port.name);
            MMALstoreLog("port.type                     ", src.body.port.type);
            MMALstoreLog("port.index                    ", src.body.port.index);
            MMALstoreLog("port.index_all                ", src.body.port.index_all);
            MMALstoreLog("port.is_enabled               ", src.body.port.is_enabled);
            MMALstoreLog("port.format                   ", src.body.port.format);
            MMALstoreLog("port.buffer_num_min           ", src.body.port.buffer_num_min);
            MMALstoreLog("port.buffer_size_min          ", src.body.port.buffer_size_min);
            MMALstoreLog("port.buffer_alignment_min     ", src.body.port.buffer_alignment_min);
            MMALstoreLog("port.buffer_num_recommended   ", src.body.port.buffer_num_recommended);
            MMALstoreLog("port.buffer_size_recommended  ", src.body.port.buffer_size_recommended);
            MMALstoreLog("port.buffer_num             **", src.body.port.buffer_num);
            MMALstoreLog("port.buffer_size            **", src.body.port.buffer_size);
            MMALstoreLog("port.component                ", src.body.port.component);
            MMALstoreLog("port.userdata                 ", src.body.port.userdata);
            MMALstoreLog("port.capabilities             ", src.body.port.capabilities);

            MMALstoreLog("format.type                   ", src.body.format.type);
            MMALstoreLog("format.encoding             **", src.body.format.encoding);
            MMALstoreLog("format.encoding_variant     **", src.body.format.encoding_variant);
            MMALstoreLog("format.es                     ", src.body.format.es);                        
            MMALstoreLog("format.bitrate              **", src.body.format.bitrate);
            MMALstoreLog("format.flags                  ", src.body.format.flags);
            MMALstoreLog("format.extradata_size         ", src.body.format.extradata_size);
            MMALstoreLog("format.extradata              ", src.body.format.extradata);

            MMALstoreLog("es.video.width                ", src.body.es.video.width);
            MMALstoreLog("es.video.height               ", src.body.es.video.height);
            MMALstoreLog("es.video.crop.x               ", src.body.es.video.crop.x);
            MMALstoreLog("es.video.crop.y               ", src.body.es.video.crop.y);
            MMALstoreLog("es.video.crop.width           ", src.body.es.video.crop.width);
            MMALstoreLog("es.video.crop.height          ", src.body.es.video.crop.height);
            MMALstoreLog("es.video.frame_rate.num       ", src.body.es.video.frame_rate.num);
            MMALstoreLog("es.video.frame_rate.den       ", src.body.es.video.frame_rate.den);
            MMALstoreLog("es.video.par.num              ", src.body.es.video.par.num);
            MMALstoreLog("es.video.par.den              ", src.body.es.video.par.den);
            MMALstoreLog("es.video.color_space          ", src.body.es.video.color_space);

            MMALstoreMsg(src.body.extradata, src.body.format.extradata_size, "extradata");        
}
*/
void CH264Decoder::Log_getPortInfo( const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply_Msg& rx)
{
    // tx HEADER //
    MMALstoreLog("tx.hdr.magic              ", tx.hdr.magic);
    MMALstoreLog("tx.hdr.type             **", tx.hdr.type);
    MMALstoreLog("tx.hdr.control_service    ", tx.hdr.control_service);
    MMALstoreLog("tx.hdr.context            ", tx.hdr.context);
    MMALstoreLog("tx.hdr.status             ", tx.hdr.status);
    MMALstoreLog("tx.hdr.padding            ", tx.hdr.padding);

    // tx BODY //
    MMALstoreLog("tx.body.component_handle**", tx.body.component_handle);
    MMALstoreLog("tx.body.port_type         ", tx.body.port_type);
    MMALstoreLog("tx.body.index             ", tx.body.index);

    // rx HEADER //
    MMALstoreLog("rx.hdr.magic              ", rx.hdr.magic);
    MMALstoreLog("rx.hdr.type             **", rx.hdr.type);
    MMALstoreLog("rx.hdr.control_service    ", rx.hdr.control_service);
    MMALstoreLog("rx.hdr.context            ", rx.hdr.context);
    MMALstoreLog("rx.hdr.status           **", rx.hdr.status);
    MMALstoreLog("rx.hdr.padding            ", rx.hdr.padding);

    // rx BODY //
    MMALstoreLog("rx.body.status          **", rx.body.status);
    MMALstoreLog("rx.body.component_handle**", rx.body.component_handle);
    MMALstoreLog("rx.body.port_type         ", rx.body.port_type);
    MMALstoreLog("rx.body.port_index        ", rx.body.port_index);
    MMALstoreLog("rx.body.found             ", rx.body.found);
    MMALstoreLog("rx.body.port_handle     **", rx.body.port_handle);

    // rx PORT //
    MMALstoreLog("port.priv                 ", rx.body.port.priv);
    MMALstoreLog("port.name                 ", rx.body.port.name);
    MMALstoreLog("port.type                 ", rx.body.port.type);
    MMALstoreLog("port.index                ", rx.body.port.index);
    MMALstoreLog("port.index_all             ", rx.body.port.index_all);
    MMALstoreLog("port.is_enabled            ", rx.body.port.is_enabled);
    MMALstoreLog("port.format                ", rx.body.port.format);
    MMALstoreLog("port.buffer_num_min        ", rx.body.port.buffer_num_min);
    MMALstoreLog("port.buffer_size_min       ", rx.body.port.buffer_size_min);
    MMALstoreLog("port.buffer_alignment_min  ", rx.body.port.buffer_alignment_min);
    MMALstoreLog("port.buffer_num_recommended", rx.body.port.buffer_num_recommended);
    MMALstoreLog("port.buffer_size_recommended", rx.body.port.buffer_size_recommended);
    MMALstoreLog("port.buffer_num          **", rx.body.port.buffer_num);
    MMALstoreLog("port.buffer_size         **", rx.body.port.buffer_size);
    MMALstoreLog("port.component             ", rx.body.port.component);
    MMALstoreLog("port.userdata              ", rx.body.port.userdata);
    MMALstoreLog("port.capabilities          ", rx.body.port.capabilities);

    // rx FORMAT //
    MMALstoreLog("format.type               ", rx.body.format.type);
    MMALstoreLog("format.encoding         **", rx.body.format.encoding);
    MMALstoreLog("format.encoding_variant **", rx.body.format.encoding_variant);
    MMALstoreLog("format.es                 ", rx.body.format.es);
    MMALstoreLog("format.bitrate          **", rx.body.format.bitrate);
    MMALstoreLog("format.flags              ", rx.body.format.flags);
    MMALstoreLog("format.extradata_size     ", rx.body.format.extradata_size);
    MMALstoreLog("format.extradata          ", rx.body.format.extradata);

    // rx ES ===================== */
    MMALstoreLog("es.video.width            ", rx.body.es.video.width);
    MMALstoreLog("es.video.height           ", rx.body.es.video.height);
    MMALstoreLog("es.video.crop.x           ", rx.body.es.video.crop.x);
    MMALstoreLog("es.video.crop.y           ", rx.body.es.video.crop.y);
    MMALstoreLog("es.video.crop.width       ", rx.body.es.video.crop.width);
    MMALstoreLog("es.video.crop.height      ", rx.body.es.video.crop.height);
    MMALstoreLog("es.video.frame_rate.num   ", rx.body.es.video.frame_rate.num);
    MMALstoreLog("es.video.frame_rate.den   ", rx.body.es.video.frame_rate.den);
    MMALstoreLog("es.video.par.num          ", rx.body.es.video.par.num);
    MMALstoreLog("es.video.par.den          ", rx.body.es.video.par.den);
    MMALstoreLog("es.video.color_space      ", rx.body.es.video.color_space);

    // rx EXTRADATA ============== */
    MMALstoreMsg(rx.body.extradata,
                 rx.body.format.extradata_size,
                 "extradata");
}
/*
void CH264Decoder::Log_setPortInfo( const MMAL_Port_Info_Set_Reply_Msg& src)
{
            MMALstoreLog("rx.hdr.magic                  ", src.hdr.magic);                   //  looks complete andd correct
            MMALstoreLog("rx.hdr.type                 **", src.hdr.type);
            MMALstoreLog("rx.hdr.control_service        ", src.hdr.control_service);
            MMALstoreLog("rx.hdr.context                ", src.hdr.context);
            MMALstoreLog("rx.hdr.status               **", src.hdr.status);
            MMALstoreLog("rx.hdr.padding                ", src.hdr.padding);

            MMALstoreLog("rx.body.status              **", src.body.status);                 // body
            MMALstoreLog("rx.body.component_handle    **", src.body.component_handle);
            MMALstoreLog("rx.body.port_type             ", src.body.port_type);
            MMALstoreLog("rx.body.index                 ", src.body.index);
            MMALstoreLog("rx.body.found                 ", src.body.found);
            MMALstoreLog("rx.body.port_handle         **", src.body.port_handle);

            MMALstoreLog("port.priv                     ", src.body.port.priv);              // body.port
            MMALstoreLog("port.name                     ", src.body.port.name);
            MMALstoreLog("port.type                     ", src.body.port.type);
            MMALstoreLog("port.index                    ", src.body.port.index);
            MMALstoreLog("port.index_all                ", src.body.port.index_all);
            MMALstoreLog("port.is_enabled               ", src.body.port.is_enabled);
            MMALstoreLog("port.format                   ", src.body.port.format);
            MMALstoreLog("port.buffer_num_min           ", src.body.port.buffer_num_min);
            MMALstoreLog("port.buffer_size_min          ", src.body.port.buffer_size_min);
            MMALstoreLog("port.buffer_alignment_min     ", src.body.port.buffer_alignment_min);
            MMALstoreLog("port.buffer_num_recommended   ", src.body.port.buffer_num_recommended);
            MMALstoreLog("port.buffer_size_recommended  ", src.body.port.buffer_size_recommended);
            MMALstoreLog("port.buffer_num             **", src.body.port.buffer_num);
            MMALstoreLog("port.buffer_size            **", src.body.port.buffer_size);
            MMALstoreLog("port.component                ", src.body.port.component);
            MMALstoreLog("port.userdata                 ", src.body.port.userdata);
            MMALstoreLog("port.capabilities             ", src.body.port.capabilities);

            MMALstoreLog("format.type                   ", src.body.format.type);
            MMALstoreLog("format.encoding             **", src.body.format.encoding);
            MMALstoreLog("format.encoding_variant     **", src.body.format.encoding_variant);
            MMALstoreLog("format.es                     ", src.body.format.es);                        
            MMALstoreLog("format.bitrate              **", src.body.format.bitrate);
            MMALstoreLog("format.flags                  ", src.body.format.flags);
            MMALstoreLog("format.extradata_size         ", src.body.format.extradata_size);
            MMALstoreLog("format.extradata              ", src.body.format.extradata);

            MMALstoreLog("es.video.width                ", src.body.es.video.width);
            MMALstoreLog("es.video.height               ", src.body.es.video.height);
            MMALstoreLog("es.video.crop.x               ", src.body.es.video.crop.x);
            MMALstoreLog("es.video.crop.y               ", src.body.es.video.crop.y);
            MMALstoreLog("es.video.crop.width           ", src.body.es.video.crop.width);
            MMALstoreLog("es.video.crop.height          ", src.body.es.video.crop.height);
            MMALstoreLog("es.video.frame_rate.num       ", src.body.es.video.frame_rate.num);
            MMALstoreLog("es.video.frame_rate.den       ", src.body.es.video.frame_rate.den);
            MMALstoreLog("es.video.par.num              ", src.body.es.video.par.num);
            MMALstoreLog("es.video.par.den              ", src.body.es.video.par.den);
            MMALstoreLog("es.video.color_space          ", src.body.es.video.color_space);

            MMALstoreMsg(src.body.extradata, src.body.format.extradata_size, "extradata");       
}
*/
void CH264Decoder::Log_setPortInfo( const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply_Msg& rx)
{
    // tx HEADER //
    MMALstoreLog("tx.hdr.magic                  ", tx.hdr.magic);
    MMALstoreLog("tx.hdr.type                 **", tx.hdr.type);
    MMALstoreLog("tx.hdr.control_service        ", tx.hdr.control_service);
    MMALstoreLog("tx.hdr.context                ", tx.hdr.context);
    MMALstoreLog("tx.hdr.status                 ", tx.hdr.status);
    MMALstoreLog("tx.hdr.padding                ", tx.hdr.padding);

    // tx BODY //
    MMALstoreLog("tx.body.component_handle    **", tx.body.component_handle);
    MMALstoreLog("tx.body.port_type             ", tx.body.port_type);
    MMALstoreLog("tx.body.port_index            ", tx.body.port_index);

    /* tx port */
    MMALstoreLog("tx.port.priv                  ", tx.body.port.priv);
    MMALstoreLog("tx.port.name                  ", tx.body.port.name);
    MMALstoreLog("tx.port.type                  ", tx.body.port.type);
    MMALstoreLog("tx.port.index                 ", tx.body.port.index);
    MMALstoreLog("tx.port.index_all             ", tx.body.port.index_all);
    MMALstoreLog("tx.port.is_enabled            ", tx.body.port.is_enabled);
    MMALstoreLog("tx.port.format                ", tx.body.port.format);
    MMALstoreLog("tx.port.buffer_num_min        ", tx.body.port.buffer_num_min);
    MMALstoreLog("tx.port.buffer_size_min       ", tx.body.port.buffer_size_min);
    MMALstoreLog("tx.port.buffer_alignment_min  ", tx.body.port.buffer_alignment_min);
    MMALstoreLog("tx.port.buffer_num_recommended", tx.body.port.buffer_num_recommended);
    MMALstoreLog("tx.port.buffer_size_recommended", tx.body.port.buffer_size_recommended);
    MMALstoreLog("tx.port.buffer_num           **", tx.body.port.buffer_num);
    MMALstoreLog("tx.port.buffer_size          **", tx.body.port.buffer_size);
    MMALstoreLog("tx.port.component             ", tx.body.port.component);
    MMALstoreLog("tx.port.userdata              ", tx.body.port.userdata);
    MMALstoreLog("tx.port.capabilities          ", tx.body.port.capabilities);

    /* tx format */
    MMALstoreLog("tx.format.type                ", tx.body.format.type);
    MMALstoreLog("tx.format.encoding          **", tx.body.format.encoding);
    MMALstoreLog("tx.format.encoding_variant  **", tx.body.format.encoding_variant);
    MMALstoreLog("tx.format.es                  ", tx.body.format.es);
    MMALstoreLog("tx.format.bitrate           **", tx.body.format.bitrate);
    MMALstoreLog("tx.format.flags               ", tx.body.format.flags);
    MMALstoreLog("tx.format.extradata_size      ", tx.body.format.extradata_size);
    MMALstoreLog("tx.format.extradata           ", tx.body.format.extradata);

    /* tx ES */
    MMALstoreLog("tx.es.video.width             ", tx.body.es.video.width);
    MMALstoreLog("tx.es.video.height            ", tx.body.es.video.height);
    MMALstoreLog("tx.es.video.crop.x            ", tx.body.es.video.crop.x);
    MMALstoreLog("tx.es.video.crop.y            ", tx.body.es.video.crop.y);
    MMALstoreLog("tx.es.video.crop.width        ", tx.body.es.video.crop.width);
    MMALstoreLog("tx.es.video.crop.height       ", tx.body.es.video.crop.height);
    MMALstoreLog("tx.es.video.frame_rate.num    ", tx.body.es.video.frame_rate.num);
    MMALstoreLog("tx.es.video.frame_rate.den    ", tx.body.es.video.frame_rate.den);
    MMALstoreLog("tx.es.video.par.num           ", tx.body.es.video.par.num);
    MMALstoreLog("tx.es.video.par.den           ", tx.body.es.video.par.den);
    MMALstoreLog("tx.es.video.color_space       ", tx.body.es.video.color_space);

    /* tx extradata */
    MMALstoreMsg(tx.body.extradata, tx.body.format.extradata_size, "tx.extradata");

    // rx HEADER //
    MMALstoreLog("rx.hdr.magic                  ", rx.hdr.magic);
    MMALstoreLog("rx.hdr.type                 **", rx.hdr.type);
    MMALstoreLog("rx.hdr.control_service        ", rx.hdr.control_service);
    MMALstoreLog("rx.hdr.context                ", rx.hdr.context);
    MMALstoreLog("rx.hdr.status               **", rx.hdr.status);
    MMALstoreLog("rx.hdr.padding                ", rx.hdr.padding);

    // rx BODY //
    MMALstoreLog("rx.body.status              **", rx.body.status);
    MMALstoreLog("rx.body.component_handle    **", rx.body.component_handle);
    MMALstoreLog("rx.body.port_type             ", rx.body.port_type);
    MMALstoreLog("rx.body.index                 ", rx.body.index);
    MMALstoreLog("rx.body.found                 ", rx.body.found);
    MMALstoreLog("rx.body.port_handle         **", rx.body.port_handle);

    /* rx port */
    MMALstoreLog("rx.port.priv                  ", rx.body.port.priv);
    MMALstoreLog("rx.port.name                  ", rx.body.port.name);
    MMALstoreLog("rx.port.type                  ", rx.body.port.type);
    MMALstoreLog("rx.port.index                 ", rx.body.port.index);
    MMALstoreLog("rx.port.index_all             ", rx.body.port.index_all);
    MMALstoreLog("rx.port.is_enabled            ", rx.body.port.is_enabled);
    MMALstoreLog("rx.port.format                ", rx.body.port.format);
    MMALstoreLog("rx.port.buffer_num_min        ", rx.body.port.buffer_num_min);
    MMALstoreLog("rx.port.buffer_size_min       ", rx.body.port.buffer_size_min);
    MMALstoreLog("rx.port.buffer_alignment_min  ", rx.body.port.buffer_alignment_min);
    MMALstoreLog("rx.port.buffer_num_recommended", rx.body.port.buffer_num_recommended);
    MMALstoreLog("rx.port.buffer_size_recommended", rx.body.port.buffer_size_recommended);
    MMALstoreLog("rx.port.buffer_num           **", rx.body.port.buffer_num);
    MMALstoreLog("rx.port.buffer_size          **", rx.body.port.buffer_size);
    MMALstoreLog("rx.port.component             ", rx.body.port.component);
    MMALstoreLog("rx.port.userdata              ", rx.body.port.userdata);
    MMALstoreLog("rx.port.capabilities          ", rx.body.port.capabilities);

    /* rx format */
    MMALstoreLog("rx.format.type                ", rx.body.format.type);
    MMALstoreLog("rx.format.encoding          **", rx.body.format.encoding);
    MMALstoreLog("rx.format.encoding_variant  **", rx.body.format.encoding_variant);
    MMALstoreLog("rx.format.es                  ", rx.body.format.es);
    MMALstoreLog("rx.format.bitrate           **", rx.body.format.bitrate);
    MMALstoreLog("rx.format.flags               ", rx.body.format.flags);
    MMALstoreLog("rx.format.extradata_size      ", rx.body.format.extradata_size);
    MMALstoreLog("rx.format.extradata           ", rx.body.format.extradata);

    /* rx ES */
    MMALstoreLog("rx.es.video.width             ", rx.body.es.video.width);
    MMALstoreLog("rx.es.video.height            ", rx.body.es.video.height);
    MMALstoreLog("rx.es.video.crop.x            ", rx.body.es.video.crop.x);
    MMALstoreLog("rx.es.video.crop.y            ", rx.body.es.video.crop.y);
    MMALstoreLog("rx.es.video.crop.width        ", rx.body.es.video.crop.width);
    MMALstoreLog("rx.es.video.crop.height       ", rx.body.es.video.crop.height);
    MMALstoreLog("rx.es.video.frame_rate.num    ", rx.body.es.video.frame_rate.num);
    MMALstoreLog("rx.es.video.frame_rate.den    ", rx.body.es.video.frame_rate.den);
    MMALstoreLog("rx.es.video.par.num           ", rx.body.es.video.par.num);
    MMALstoreLog("rx.es.video.par.den           ", rx.body.es.video.par.den);
    MMALstoreLog("rx.es.video.color_space       ", rx.body.es.video.color_space);

    /* rx extradata */
    MMALstoreMsg(rx.body.extradata, rx.body.format.extradata_size, "rx.extradata");
}
/*
void CH264Decoder::Log_enableComponent( const MMAL_Component_Enable_Reply_Msg&  src)
{
            MMALstoreLog("rx.hdr.magic                  ", src.hdr.magic);               // complete
            MMALstoreLog("rx.hdr.type                 **", src.hdr.type);
            MMALstoreLog("rx.hdr.control_service        ", src.hdr.control_service);
            MMALstoreLog("rx.hdr.context                ", src.hdr.context);
            MMALstoreLog("rx.hdr.status               **", src.hdr.status);
            MMALstoreLog("rx.hdr.padding                ", src.hdr.padding);

            MMALstoreLog("\nRX.body.status              ", src.body.status);             // the only answer? how i can see also the tx component_handle? 

}
*/
void CH264Decoder::Log_enableComponent( const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply_Msg& rx)
{
    
    MMALstoreLog("tx.hdr.magic                  ", tx.hdr.magic);                                   // tx HEADER //
    MMALstoreLog("tx.hdr.type                 **", tx.hdr.type);
    MMALstoreLog("tx.hdr.control_service        ", tx.hdr.control_service);
    MMALstoreLog("tx.hdr.context                ", tx.hdr.context);
    MMALstoreLog("tx.hdr.status                 ", tx.hdr.status);
    MMALstoreLog("tx.hdr.padding                ", tx.hdr.padding);

    
    MMALstoreLog("tx.body.component_handle    **", tx.body.component_handle);                       // tx BODY //

    
    MMALstoreLog("rx.hdr.magic                  ", rx.hdr.magic);                                   // rx HEADER //
    MMALstoreLog("rx.hdr.type                 **", rx.hdr.type);
    MMALstoreLog("rx.hdr.control_service        ", rx.hdr.control_service);
    MMALstoreLog("rx.hdr.context                ", rx.hdr.context);
    MMALstoreLog("rx.hdr.status               **", rx.hdr.status);
    MMALstoreLog("rx.hdr.padding                ", rx.hdr.padding);

    
    MMALstoreLog("rx.body.status              **", rx.body.status);                                 // rx BODY //
}
/*
void CH264Decoder::Log_enablePort( const MMAL_Port_Action_Reply_Msg&  src)
{
            MMALstoreLog("rx.hdr.magic                  ", src.hdr.magic);               // complete
            MMALstoreLog("rx.hdr.type                 **", src.hdr.type);
            MMALstoreLog("rx.hdr.control_service        ", src.hdr.control_service);
            MMALstoreLog("rx.hdr.context                ", src.hdr.context);
            MMALstoreLog("rx.hdr.status               **", src.hdr.status);
            MMALstoreLog("rx.hdr.padding                ", src.hdr.padding);

            MMALstoreLog("\nRX.body.status              ", src.body.status);             // the only answer? how i can see also the tx component_handle? 
}
*/
void CH264Decoder::Log_enablePort( const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx)
{
    
    MMALstoreLog("tx.hdr.magic                  ", tx.hdr.magic);                                   // tx HEADER //
    MMALstoreLog("tx.hdr.type                 **", tx.hdr.type);
    MMALstoreLog("tx.hdr.control_service        ", tx.hdr.control_service);
    MMALstoreLog("tx.hdr.context                ", tx.hdr.context);
    MMALstoreLog("tx.hdr.status                 ", tx.hdr.status);
    MMALstoreLog("tx.hdr.padding                ", tx.hdr.padding);
    
    MMALstoreLog("tx.body.component_handle    **", tx.body.component_handle);                       // tx BODY //
    MMALstoreLog("tx.body.port_handle         **", tx.body.port_handle);
    MMALstoreLog("tx.body.action                ", tx.body.action);

    MMALstoreLog("tx.port.priv                  ", tx.body.port.priv);
    MMALstoreLog("tx.port.name                  ", tx.body.port.name);
    MMALstoreLog("tx.port.type                  ", tx.body.port.type);
    MMALstoreLog("tx.port.index                 ", tx.body.port.index);
    MMALstoreLog("tx.port.index_all             ", tx.body.port.index_all);
    MMALstoreLog("tx.port.is_enabled            ", tx.body.port.is_enabled);
    MMALstoreLog("tx.port.format                ", tx.body.port.format);
    MMALstoreLog("tx.port.buffer_num_min        ", tx.body.port.buffer_num_min);
    MMALstoreLog("tx.port.buffer_size_min       ", tx.body.port.buffer_size_min);
    MMALstoreLog("tx.port.buffer_alignment_min  ", tx.body.port.buffer_alignment_min);
    MMALstoreLog("tx.port.buffer_num_recommended", tx.body.port.buffer_num_recommended);
    MMALstoreLog("tx.port.buffer_size_recommended", tx.body.port.buffer_size_recommended);
    MMALstoreLog("tx.port.buffer_num           **", tx.body.port.buffer_num);
    MMALstoreLog("tx.port.buffer_size          **", tx.body.port.buffer_size);
    MMALstoreLog("tx.port.component             ", tx.body.port.component);
    MMALstoreLog("tx.port.userdata              ", tx.body.port.userdata);
    MMALstoreLog("tx.port.capabilities          ", tx.body.port.capabilities);
    
    MMALstoreLog("rx.hdr.magic                  ", rx.hdr.magic);                                   // rx HEADER //
    MMALstoreLog("rx.hdr.type                 **", rx.hdr.type);
    MMALstoreLog("rx.hdr.control_service        ", rx.hdr.control_service);
    MMALstoreLog("rx.hdr.context                ", rx.hdr.context);
    MMALstoreLog("rx.hdr.status               **", rx.hdr.status);
    MMALstoreLog("rx.hdr.padding                ", rx.hdr.padding);
    
    MMALstoreLog("rx.body.status              **", rx.body.status);                                 // rx BODY //
}
/*
void CH264Decoder::Log_setZeroCopyMode( const MMAL_Port_Parameter_Set_Reply_Msg&  src)
{
            MMALstoreLog("rx.hdr.magic                  ", src.hdr.magic);               // complete
            MMALstoreLog("rx.hdr.type                 **", src.hdr.type);
            MMALstoreLog("rx.hdr.control_service        ", src.hdr.control_service);
            MMALstoreLog("rx.hdr.context                ", src.hdr.context);
            MMALstoreLog("rx.hdr.status               **", src.hdr.status);
            MMALstoreLog("rx.hdr.padding                ", src.hdr.padding);

            MMALstoreLog("\nRX.body.status              ", src.body.status);             // the only answer? how i can see also the tx component_handle? 
}
*/
void CH264Decoder::Log_setZeroCopyMode( const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply_Msg& rx)
{
    // tx HEADER //
    MMALstoreLog("tx.hdr.magic                  ", tx.hdr.magic);
    MMALstoreLog("tx.hdr.type                 **", tx.hdr.type);
    MMALstoreLog("tx.hdr.control_service        ", tx.hdr.control_service);
    MMALstoreLog("tx.hdr.context                ", tx.hdr.context);
    MMALstoreLog("tx.hdr.status                 ", tx.hdr.status);
    MMALstoreLog("tx.hdr.padding                ", tx.hdr.padding);
    // tx BODY //
    MMALstoreLog("tx.body.component_handle    **", tx.body.component_handle);
    MMALstoreLog("tx.body.port_handle         **", tx.body.port_handle);
    MMALstoreLog("tx.body.id                  **", tx.body.id);
    MMALstoreLog("tx.body.size                **", tx.body.size);
    /* opaque parameter payload (CORRECT PATTERN) */
    storeMsg(tx.body.value, tx.body.size, "tx.body.value");
    // rx HEADER //
    MMALstoreLog("rx.hdr.magic                  ", rx.hdr.magic);
    MMALstoreLog("rx.hdr.type                 **", rx.hdr.type);
    MMALstoreLog("rx.hdr.control_service        ", rx.hdr.control_service);
    MMALstoreLog("rx.hdr.context                ", rx.hdr.context);
    MMALstoreLog("rx.hdr.status               **", rx.hdr.status);
    MMALstoreLog("rx.hdr.padding                ", rx.hdr.padding);
    // rx BODY //
    MMALstoreLog("rx.body.status              **", rx.body.status);
}

/*
void CH264Decoder::Log_BufferBody(  const mmal_msg_buffer_from_host_wire32& body )
{
    MMALstoreLog("drvbuf.magic                  ", body.drvbuf.magic);
    MMALstoreLog("drvbuf.component_handle       ", body.drvbuf.component_handle);
    MMALstoreLog("drvbuf.port_handle            ", body.drvbuf.port_handle);
    MMALstoreLog("drvbuf.client_context         ", body.drvbuf.client_context);

    MMALstoreMsg(body.drvbuf_ref, sizeof(body.drvbuf_ref), "drvbuf_ref");

    MMALstoreLog("buffer_header.next            ", body.buffer_header.next);
    MMALstoreLog("buffer_header.priv            ", body.buffer_header.priv);
    MMALstoreLog("buffer_header.cmd             ", body.buffer_header.cmd);
    MMALstoreLog("buffer_header.data            ", body.buffer_header.data);
    MMALstoreLog("buffer_header.alloc_size      ", body.buffer_header.alloc_size);
    MMALstoreLog("buffer_header.length          ", body.buffer_header.length);
    MMALstoreLog("buffer_header.offset          ", body.buffer_header.offset);
    MMALstoreLog("buffer_header.flags           ", body.buffer_header.flags);
    MMALstoreLog("buffer_header.pts_lo          ", body.buffer_header.pts_lo);
    MMALstoreLog("buffer_header.pts_hi          ", body.buffer_header.pts_hi);
    MMALstoreLog("buffer_header.dts_lo          ", body.buffer_header.dts_lo);
    MMALstoreLog("buffer_header.dts_hi          ", body.buffer_header.dts_hi);
    MMALstoreLog("buffer_header.type            ", body.buffer_header.type);
    MMALstoreLog("buffer_header.user_data       ", body.buffer_header.user_data);

    MMALstoreMsg(body.type_specific, sizeof(body.type_specific), "type_specific");

    MMALstoreLog("is_zero_copy                  ", (u32)body.is_zero_copy);
    MMALstoreLog("has_reference                 ", (u32)body.has_reference);
    MMALstoreLog("payload_in_message            ", (u32)body.payload_in_message);

    MMALstoreMsg(body.short_data, sizeof(body.short_data), "short_data");
}

void CH264Decoder::Log_queueBufferFromHost( const MMAL_Buffer_From_Host_Msg& tx )
{
    MMALstoreLog("sizeof(tx)                   ", (u32)sizeof(tx));

    MMALstoreLog("\nTX.hdr.magic                 ", tx.hdr.magic);
    MMALstoreLog("tx.hdr.type                  ", tx.hdr.type);
    MMALstoreLog("tx.hdr.context               ", tx.hdr.context);
    MMALstoreLog("tx.hdr.status                ", tx.hdr.status);

    MMALstoreLog("\ndrvbuf.magic                 ", tx.body.drvbuf.magic);
    MMALstoreLog("drvbuf.component_handle      ", tx.body.drvbuf.component_handle);
    MMALstoreLog("drvbuf.port_handle           ", tx.body.drvbuf.port_handle);
    MMALstoreLog("drvbuf.client_context        ", tx.body.drvbuf.client_context);

    MMALstoreMsg(tx.body.drvbuf_ref, sizeof(tx.body.drvbuf_ref), "drvbuf_ref");

    MMALstoreLog("buffer_header.data           ", tx.body.buffer_header.data);
    MMALstoreLog("buffer_header.alloc_size     ", tx.body.buffer_header.alloc_size);
    MMALstoreLog("buffer_header.length         ", tx.body.buffer_header.length);
    MMALstoreLog("buffer_header.offset         ", tx.body.buffer_header.offset);
    MMALstoreLog("buffer_header.flags          ", tx.body.buffer_header.flags);
    MMALstoreLog("buffer_header.pts_lo         ", tx.body.buffer_header.pts_lo);
    MMALstoreLog("buffer_header.pts_hi         ", tx.body.buffer_header.pts_hi);
    MMALstoreLog("buffer_header.dts_lo         ", tx.body.buffer_header.dts_lo);
    MMALstoreLog("buffer_header.dts_hi         ", tx.body.buffer_header.dts_hi);

    MMALstoreMsg(tx.body.type_specific, sizeof(tx.body.type_specific), "type_specific");

    MMALstoreLog("is_zero_copy                 ", (u32)tx.body.is_zero_copy);
    MMALstoreLog("has_reference                ", (u32)tx.body.has_reference);
    MMALstoreLog("payload_in_message           ", (u32)tx.body.payload_in_message);

    MMALstoreMsg(tx.body.short_data, sizeof(tx.body.short_data), "short_data");
}
*/
