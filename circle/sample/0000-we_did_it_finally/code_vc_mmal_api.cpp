#include "kernel.h"

#define MY_BFR m_bufferLog[LOG_MMAL_0]
#define MY_IDX m_bufferLogIndex[LOG_MMAL_0]

bool            CKernel::framePollerMMAL            (   u32 nal_block_offset, u32 nal_block_length)
{
                MY_IDX %= (LOG_SIZ - 32768);

                if (!f_firstFrameQueued)            // NEW
                    {
                    if (!queueOutputBufferMMAL(    *m_BufferFromHostTx_Output,
                                                    m_output_buffer_handle_a,
                                                    m_OutputBufferSizeA))
                        {
#ifdef __LOG_MMAL__
                        storeLogHex( MY_BFR, MY_IDX, "First opaque output A queue FAILED", m_output_buffer_handle_a);
#endif
                        return false;
                        }

#ifdef __LOG_MMAL__
                        storeLogHex( MY_BFR, MY_IDX, "First opaque output A queue SUCCESS", m_output_buffer_handle_a);
#endif

                    if (!queueOutputBufferMMAL(    *m_BufferFromHostTx_Output,
                                                    m_output_buffer_handle_b,
                                                    m_OutputBufferSizeB))
                        {
#ifdef __LOG_MMAL__
                        storeLogHex( MY_BFR, MY_IDX, "First opaque output B queue FAILED", m_output_buffer_handle_b);
#endif
                        return false;
                        }

#ifdef __LOG_MMAL__
                        storeLogHex( MY_BFR, MY_IDX, "First opaque output B queue SUCCESS", m_output_buffer_handle_b);
#endif

                    if (!queueInputBufferMMAL(     *m_BufferFromHostTx_Input,
                                                    nal_block_offset,
                                                    nal_block_length))
                        {
#ifdef __LOG_MMAL__
                        storeLogHex( MY_BFR, MY_IDX, "First input frame queue FAILED", X_VAL, "offset", nal_block_offset, "length", nal_block_length);
#endif
                        return false;
                        }

#ifdef __LOG_MMAL__
                        storeLogHex( MY_BFR, MY_IDX, "First input frame queue SUCCESS", X_VAL, "offset", nal_block_offset, "length", nal_block_length);
#endif

#ifdef __LOG_FIRST_FRAME__
                        storeLogHex( MY_BFR, MY_IDX, "First decoder work submitted", X_VAL, "output A", m_output_buffer_handle_a, "output B", m_output_buffer_handle_b);
#endif

                        f_firstFrameQueued = true;
                        return true;
                    }
// END
#ifdef __LOG_MMAL__
                const char*  message = "";
#endif
                uint32_t msg_len = 0;
    // Drain RX queue unconditionally (non-blocking)
            //  while (vchi_msg_dequeue(m_ServiceHandleMMAL, &m_BufferFromHostTx_Output, sizeof(m_BufferFromHostTx_Output), &msg_len, VCHI_FLAGS_NONE) == 0)
                while (vchi_msg_dequeue(m_ServiceHandleMMAL, m_BufferFromHostTx_Output, sizeof(*m_BufferFromHostTx_Output), &msg_len, VCHI_FLAGS_NONE) == 0)
                    {
                    switch (m_BufferFromHostTx_Output->hdr.type) // FIRST AXIS: message type (semantic meaning)
                        {
                        case MMAL_MSG_TYPE_BUFFER_TO_HOST:
                            {
                            switch (m_BufferFromHostTx_Output->hdr.status)   // SECOND AXIS: status (outcome)
                                {                                 
                                case MMAL_MSG_STATUS_SUCCESS:       // NEW
                                    {
                                    u32 currentHandle =
                                        m_BufferFromHostTx_Output->msg.buffer_header.data;

                                    if (currentHandle != m_output_buffer_handle_a && currentHandle != m_output_buffer_handle_b)
                                        {
                                        continue;
                                        }
                                    u32 nextOutputHandle = 0;
                                    u32 nextOutputSize   = 0;

                                    if (currentHandle == m_output_buffer_handle_a)
                                        {
                                        nextOutputHandle = m_output_buffer_handle_b;
                                        nextOutputSize   = m_OutputBufferSizeB;
                                        }
                                    else
                                        {
                                        nextOutputHandle = m_output_buffer_handle_a;
                                        nextOutputSize   = m_OutputBufferSizeA;
                                        }

                                    // If the next decoder destination is still backing the video texture, finish all preceding GL reads before MMAL receives it. On the first returned frame m_displayedOutputHandle is zero, so no wait occurs.

                                    if (m_displayedOutputHandle == nextOutputHandle)
                                        {
                                        glFinish();

                                        if (!checkGLerrorMMAL()) return false;
                                        }

                                    if (!queueOutputBufferMMAL( *m_BufferFromHostTx_Output, nextOutputHandle, nextOutputSize))
                                        {
                                        return false;
                                        }

                                    if (!bufferReadyMMAL(currentHandle)) return false;

                                    // bufferReadyMMAL() has successfully attached currentHandle to m_tex.gl_tex_vid. Record which output must not be recycled until preceding GL reads have completed.

                                    m_displayedOutputHandle = currentHandle;

                                    if (!queueInputBufferMMAL( *m_BufferFromHostTx_Input, nal_block_offset, nal_block_length))
                                        {
                                        return false;
                                        }

#ifdef __LOG_MMAL__
                                    message = "MMAL output ready"; // does this ever apear in my log?!?!
                                    storeLogHex( MY_BFR, MY_IDX, message, X_VAL, "Frame offset", nal_block_offset, "length", nal_block_length);
#endif

                                    return true;
                                    }

#ifdef __LOG_MMAL__   
                                case MMAL_MSG_STATUS_ENOMEM:     message = "MMAL_MSG_STATUS_ENOMEM       - Out of memory                      "; break;
                                case MMAL_MSG_STATUS_ENOSPC:     message = "MMAL_MSG_STATUS_ENOSPC       - Out of resources other than memory "; break;
                                case MMAL_MSG_STATUS_EINVAL:     message = "MMAL_MSG_STATUS_EINVAL       - Argument is invalid                "; break;
                                case MMAL_MSG_STATUS_ENOSYS:     message = "MMAL_MSG_STATUS_ENOSYS       - Function not implemented           "; break;
                                case MMAL_MSG_STATUS_ENOENT:     message = "MMAL_MSG_STATUS_ENOENT       - No such file or directory          "; break;
                                case MMAL_MSG_STATUS_ENXIO:      message = "MMAL_MSG_STATUS_ENXIO        - No such device or address          "; break;
                                case MMAL_MSG_STATUS_EIO:        message = "MMAL_MSG_STATUS_EIO          - I/O error                          "; break;
                                case MMAL_MSG_STATUS_ESPIPE:     message = "MMAL_MSG_STATUS_ESPIPE       - Illegal seek                       "; break;
                                case MMAL_MSG_STATUS_ECORRUPT:   message = "MMAL_MSG_STATUS_ECORRUPT     - Data is corrupt                    "; break;
                                case MMAL_MSG_STATUS_ENOTREADY:  message = "MMAL_MSG_STATUS_ENOTREADY    - Component is not ready             "; break;
                                case MMAL_MSG_STATUS_ECONFIG:    message = "MMAL_MSG_STATUS_ECONFIG      - Component is not configured        "; break;
                                case MMAL_MSG_STATUS_EISCONN:    message = "MMAL_MSG_STATUS_EISCONN      - Port is already connected          "; break;
                                case MMAL_MSG_STATUS_ENOTCONN:   message = "MMAL_MSG_STATUS_ENOTCONN     - Port is disconnected               "; break;
                                case MMAL_MSG_STATUS_EAGAIN:     message = "MMAL_MSG_STATUS_EAGAIN       - Resource temporarily unavailable   "; break;
                                case MMAL_MSG_STATUS_EFAULT:     message = "MMAL_MSG_STATUS_EFAULT       - Bad address                        "; break;
                  
                                default:                         message = "Unknown MMAL status          - WTF!!!                             "; break;
#endif                                 
                                }
#ifdef __LOG_MMAL__   
                        //  nextline( MY_BFR, MY_IDX );
                            storeLogHex( MY_BFR, MY_IDX, message, X_VAL, "Frame offset", nal_block_offset, "length", nal_block_length);
                            storeMsg( MY_BFR, MY_IDX, "Poller ERROR (BUFFER_TO_HOST)", m_BufferFromHostTx_Output, msg_len);
#endif               
                            return false;
                            }
#ifdef __LOG_MMAL__                            
                        // the only purpose was fo figure out what the format change was, we figured that the vpu simply added the framerate to our buffer / port settings!                            
                        case MMAL_MSG_TYPE_EVENT_TO_HOST:
                            {
                            const mmal_msg_event_to_host* event = reinterpret_cast<const mmal_msg_event_to_host*>(  &m_BufferFromHostTx_Output->msg);

                            switch (event->cmd)
                                {
                                case MMAL_EVENT_ERROR:               message = "MMAL_EVENT_ERROR             - Component reported an error       "; break;
                                case MMAL_EVENT_EOS:                 message = "MMAL_EVENT_EOS               - End of stream                     "; break;

                                case MMAL_EVENT_FORMAT_CHANGED:
                                    {
                                    message = "MMAL_EVENT_FORMAT_CHANGED    - Port format changed                ";


                                    const mmal_msg_event_format_changed* formatChanged = reinterpret_cast<const mmal_msg_event_format_changed*>(event->data);

                                    storeLogHex( MY_BFR, MY_IDX, ">:", getClockMilliseconds(), "FORMAT CHANGED RX");
                                    storeLogHex( MY_BFR, MY_IDX, "event.port_type                 ", event->port_type);
                                    storeLogHex( MY_BFR, MY_IDX, "event.port_num                  ", event->port_num);
                                    storeLogHex( MY_BFR, MY_IDX, "event.length                    ", event->length);

                                    storeLogHex( MY_BFR, MY_IDX, "buffer_size_min                 ", formatChanged->buffer_size_min);
                                    storeLogHex( MY_BFR, MY_IDX, "buffer_num_min                  ", formatChanged->buffer_num_min);
                                    storeLogHex( MY_BFR, MY_IDX, "buffer_size_recommended         ", formatChanged->buffer_size_recommended);
                                    storeLogHex( MY_BFR, MY_IDX, "buffer_num_recommended          ", formatChanged->buffer_num_recommended);
                                    storeLogHex( MY_BFR, MY_IDX, "es_ptr                          ", formatChanged->es_ptr);

                                    storeLogHex( MY_BFR, MY_IDX, "format.type                     ", formatChanged->format.type);
                                    storeLogHex( MY_BFR, MY_IDX, "format.encoding                 ", formatChanged->format.encoding);
                                    storeLogHex( MY_BFR, MY_IDX, "format.encoding_variant         ", formatChanged->format.encoding_variant);
                                    storeLogHex( MY_BFR, MY_IDX, "format.es                       ", formatChanged->format.es);
                                    storeLogHex( MY_BFR, MY_IDX, "format.bitrate                  ", formatChanged->format.bitrate);
                                    storeLogHex( MY_BFR, MY_IDX, "format.flags                    ", formatChanged->format.flags);
                                    storeLogHex( MY_BFR, MY_IDX, "format.extradata_size           ", formatChanged->format.extradata_size);
                                    storeLogHex( MY_BFR, MY_IDX, "format.extradata                ", formatChanged->format.extradata);

                                    storeLogHex( MY_BFR, MY_IDX, "video.width                     ", formatChanged->es.video.width);
                                    storeLogHex( MY_BFR, MY_IDX, "video.height                    ", formatChanged->es.video.height);
                                    storeLogHex( MY_BFR, MY_IDX, "video.crop.x                    ", (u32)formatChanged->es.video.crop.x);
                                    storeLogHex( MY_BFR, MY_IDX, "video.crop.y                    ", (u32)formatChanged->es.video.crop.y);
                                    storeLogHex( MY_BFR, MY_IDX, "video.crop.width                ", (u32)formatChanged->es.video.crop.width);
                                    storeLogHex( MY_BFR, MY_IDX, "video.crop.height               ", (u32)formatChanged->es.video.crop.height);
                                    storeLogHex( MY_BFR, MY_IDX, "video.frame_rate.num            ", (u32)formatChanged->es.video.frame_rate.num);
                                    storeLogHex( MY_BFR, MY_IDX, "video.frame_rate.den            ", (u32)formatChanged->es.video.frame_rate.den);
                                    storeLogHex( MY_BFR, MY_IDX, "video.par.num                   ", (u32)formatChanged->es.video.par.num);
                                    storeLogHex( MY_BFR, MY_IDX, "video.par.den                   ", (u32)formatChanged->es.video.par.den);
                                    storeLogHex( MY_BFR, MY_IDX, "video.color_space               ", formatChanged->es.video.color_space);

                                    break;
                                    }

                                case MMAL_EVENT_PARAMETER_CHANGED:   message = "MMAL_EVENT_PARAMETER_CHANGED - Port parameter changed             "; break;

                                default:                             message = "Unknown MMAL event           - Unknown event command              "; break;
                                }

                            storeLogHex( MY_BFR, MY_IDX, ">:", getClockMilliseconds(), message, X_VAL, "command", event->cmd);
                            storeMsg( MY_BFR, MY_IDX, message, m_BufferFromHostTx_Output, msg_len);

                            continue;
                            }                            
#endif
                        default: 
                            {
#ifdef __LOG_MMAL__                                  
                            message = "UNEXPECTED MESSAGE";
  
                        //  nextline( MY_BFR, MY_IDX );
                            storeLogHex( MY_BFR, MY_IDX, message,  X_VAL, "Frame offset", nal_block_offset, "Type",  m_BufferFromHostTx_Output->hdr.type, "Status", m_BufferFromHostTx_Output->hdr.status);
                            storeMsg( MY_BFR, MY_IDX, "Poller ERROR (UNEXPECTED MESSAGE)", m_BufferFromHostTx_Output, msg_len);
#endif 
                            break;
                            }
                        }
#ifdef __LOG_MMAL__ 
                //  nextline( MY_BFR, MY_IDX );
                    storeLogHex( MY_BFR, MY_IDX, "Unexpected Reply", X_VAL, "Frame offset", nal_block_offset, "length", nal_block_length);
                //  storeMsg( MY_BFR, MY_IDX, "Unexpected Reply", &m_BufferFromHostTx_Output, msg_len);
                    storeMsg( MY_BFR, MY_IDX, "Unexpected Reply", m_BufferFromHostTx_Output, msg_len);
#endif       
                    }
#ifdef __LOG_MMAL__   
            //  nextline( MY_BFR, MY_IDX );
                storeLogHex( MY_BFR, MY_IDX, "Nothing in the Pipeline", X_VAL, "Frame offset", nal_block_offset, "length", nal_block_length);    // Nothing relevant received
#endif 
                return true;
}

bool            CKernel::bufferReadyMMAL(u32 handle)
{
                EGLImageKHR nextImage = eglCreateImageKHR(
                    m_ogl.display,
                    EGL_NO_CONTEXT,
                    EGL_IMAGE_BRCM_MULTIMEDIA,
                    (EGLClientBuffer)(uintptr_t)handle,
                    nullptr);

                if (nextImage == EGL_NO_IMAGE_KHR)
                    {
#ifdef __LOG_MMAL__
                    storeLogHex( MY_BFR, MY_IDX, "EGL multimedia image creation FAILED", handle);
#endif
                    return false;
                    }

                glBindTexture( GL_TEXTURE_EXTERNAL_OES, m_tex.gl_tex_vid);

                glEGLImageTargetTexture2DOES( GL_TEXTURE_EXTERNAL_OES, nextImage);

                if (!checkGLerrorMMAL())
                    {
                    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
                    eglDestroyImageKHR(m_ogl.display, nextImage);
                    return false;
                    }

                glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);

                EGLImageKHR previousImage = m_tex.m_EGLimage;
                m_tex.m_EGLimage = nextImage;

                if (previousImage != EGL_NO_IMAGE_KHR)
                    {
                    eglDestroyImageKHR( m_ogl.display, previousImage);
                    }

                return true;
}

bool            CKernel::queueOutputBufferMMAL      (   MMAL_Buffer_From_Host_Msg& tx, 
                                                        u32 vc_handle, 
                                                        u32 alloc_size)
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;  /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.data                          = vc_handle;
                tx.msg.buffer_header.alloc_size                    = alloc_size;  // why again?
                tx.msg.buffer_header.length                        = 0;
                tx.msg.buffer_header.offset                        = 0;
                tx.msg.buffer_header.flags                         = 0;

#ifdef __LOG_MMAL__
                storeLogHex( MY_BFR, MY_IDX, ">:", getClockMilliseconds(), "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BFR, MY_IDX, "QueueOutputBuffer", &tx, (u32)sizeof(tx));
#endif 
                return (vchi_msg_queue(m_ServiceHandleMMAL, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}

bool            CKernel::queueInputBufferMMAL       (   MMAL_Buffer_From_Host_Msg& tx, 
                                                        u32 nal_block_offset, 
                                                        u32 nal_block_length)
{
                initHeaderMMAL( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;                /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.offset                        = nal_block_offset;
                tx.msg.buffer_header.length                        = nal_block_length;

                u32 flags                                           = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;
                tx.msg.buffer_header.flags                         = flags;
#ifdef __LOG_MMAL__
                storeLogHex( MY_BFR, MY_IDX, ">:", getClockMilliseconds(), "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BFR, MY_IDX, "QueueInputBuffer", &tx, (u32)sizeof(tx));
#endif 
                return (vchi_msg_queue(m_ServiceHandleMMAL, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}



void            CKernel::dumpPortInfoMMAL           (   const MMAL_Port_Info_Get_Reply&         p_info)
{
#ifdef __LOG_MMAL__ 
                // the only purpose was fo figure out what the format change was, we figured that the vpu simply added the framerate to our buffer / port settings!
                storeLogHex( MY_BFR, MY_IDX, ">:", getClockMilliseconds(), "MMAL PORT INFO");

                storeLogHex( MY_BFR, MY_IDX, "status                          ", p_info.msg.status);
                storeLogHex( MY_BFR, MY_IDX, "component_handle                ", p_info.msg.component_handle);
                storeLogHex( MY_BFR, MY_IDX, "port_type                       ", p_info.msg.port_type);
                storeLogHex( MY_BFR, MY_IDX, "port_index                      ", p_info.msg.port_index);
                storeLogHex( MY_BFR, MY_IDX, "found                           ", (u32)p_info.msg.found);
                storeLogHex( MY_BFR, MY_IDX, "port_handle                     ", p_info.msg.port_handle);

                storeLogHex( MY_BFR, MY_IDX, "port.priv                       ", p_info.msg.port.priv);
                storeLogHex( MY_BFR, MY_IDX, "port.name                       ", p_info.msg.port.name);
                storeLogHex( MY_BFR, MY_IDX, "port.type                       ", p_info.msg.port.type);
                storeLogHex( MY_BFR, MY_IDX, "port.index                      ", p_info.msg.port.index);
                storeLogHex( MY_BFR, MY_IDX, "port.index_all                  ", p_info.msg.port.index_all);
                storeLogHex( MY_BFR, MY_IDX, "port.is_enabled                 ", p_info.msg.port.is_enabled);
                storeLogHex( MY_BFR, MY_IDX, "port.format                     ", p_info.msg.port.format);
                storeLogHex( MY_BFR, MY_IDX, "port.buffer_num_min             ", p_info.msg.port.buffer_num_min);
                storeLogHex( MY_BFR, MY_IDX, "port.buffer_size_min            ", p_info.msg.port.buffer_size_min);
                storeLogHex( MY_BFR, MY_IDX, "port.buffer_alignment_min       ", p_info.msg.port.buffer_alignment_min);
                storeLogHex( MY_BFR, MY_IDX, "port.buffer_num_recommended     ", p_info.msg.port.buffer_num_recommended);
                storeLogHex( MY_BFR, MY_IDX, "port.buffer_size_recommended    ", p_info.msg.port.buffer_size_recommended);
                storeLogHex( MY_BFR, MY_IDX, "port.buffer_num                 ", p_info.msg.port.buffer_num);
                storeLogHex( MY_BFR, MY_IDX, "port.buffer_size                ", p_info.msg.port.buffer_size);
                storeLogHex( MY_BFR, MY_IDX, "port.component                  ", p_info.msg.port.component);
                storeLogHex( MY_BFR, MY_IDX, "port.userdata                   ", p_info.msg.port.userdata);
                storeLogHex( MY_BFR, MY_IDX, "port.capabilities               ", p_info.msg.port.capabilities);

                storeLogHex( MY_BFR, MY_IDX, "format.type                     ", p_info.msg.format.type);
                storeLogHex( MY_BFR, MY_IDX, "format.encoding                 ", p_info.msg.format.encoding);
                storeLogHex( MY_BFR, MY_IDX, "format.encoding_variant         ", p_info.msg.format.encoding_variant);
                storeLogHex( MY_BFR, MY_IDX, "format.es                       ", p_info.msg.format.es);
                storeLogHex( MY_BFR, MY_IDX, "format.bitrate                  ", p_info.msg.format.bitrate);
                storeLogHex( MY_BFR, MY_IDX, "format.flags                    ", p_info.msg.format.flags);
                storeLogHex( MY_BFR, MY_IDX, "format.extradata_size           ", p_info.msg.format.extradata_size);
                storeLogHex( MY_BFR, MY_IDX, "format.extradata                ", p_info.msg.format.extradata);

                storeLogHex( MY_BFR, MY_IDX, "video.width                     ", p_info.msg.es.video.width);
                storeLogHex( MY_BFR, MY_IDX, "video.height                    ", p_info.msg.es.video.height);
                storeLogHex( MY_BFR, MY_IDX, "video.crop.x                    ", (u32)p_info.msg.es.video.crop.x);
                storeLogHex( MY_BFR, MY_IDX, "video.crop.y                    ", (u32)p_info.msg.es.video.crop.y);
                storeLogHex( MY_BFR, MY_IDX, "video.crop.width                ", (u32)p_info.msg.es.video.crop.width);
                storeLogHex( MY_BFR, MY_IDX, "video.crop.height               ", (u32)p_info.msg.es.video.crop.height);
                storeLogHex( MY_BFR, MY_IDX, "video.frame_rate.num            ", (u32)p_info.msg.es.video.frame_rate.num);
                storeLogHex( MY_BFR, MY_IDX, "video.frame_rate.den            ", (u32)p_info.msg.es.video.frame_rate.den);
                storeLogHex( MY_BFR, MY_IDX, "video.par.num                   ", (u32)p_info.msg.es.video.par.num);
                storeLogHex( MY_BFR, MY_IDX, "video.par.den                   ", (u32)p_info.msg.es.video.par.den);
                storeLogHex( MY_BFR, MY_IDX, "video.color_space               ", p_info.msg.es.video.color_space);
#endif                
}