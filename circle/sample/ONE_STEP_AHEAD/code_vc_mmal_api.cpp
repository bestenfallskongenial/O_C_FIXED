#include "kernel.h"

    #define MY_BUFFER   m_logBuffer                 // not used here 
    #define MY_INDEX    m_logBufferIndex

bool            CKernel::framePollerMMAL            (   u32 nal_block_offset, u32 nal_block_length)
{
#ifdef __FIRST_FRAME__
                if (!f_firstFrameQueued)
                    {
                    if (!queueInputBufferMMAL(*m_BufferFromHostTx_Input, nal_block_offset, nal_block_length))
                        {
#ifdef __LOG_MMAL__
                        storeLog( MY_BUFFER, MY_INDEX, "very first frame queue ERROR!", EMPTYLOG, "Frame offset", nal_block_offset, "length", nal_block_length);
#endif
                        return false;
                        }
#ifdef __LOG_MMAL__
                    storeLog( MY_BUFFER, MY_INDEX, "very first frame queue SUCCESS", EMPTYLOG, "Frame offset", nal_block_offset, "length", nal_block_length);
#endif
                    getPortInfoMMAL(MMAL_PORT_TYPE_INPUT,  m_InputPortHandle,  *m_PortInfoGetTx_Input_D, *m_PortInfoGetRx_Input_D);
                    getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, *m_PortInfoGetTx_Output_D, *m_PortInfoGetRx_Output_D);

                    f_firstFrameQueued = true;
                    return true;
                    }
#endif 
                const char*  message = "";
                uint32_t msg_len = 0;


    // Drain RX queue unconditionally (non-blocking)

                while (vchi_msg_dequeue(m_ServiceHandleMMAL, &m_BufferFromHostTx_Output, sizeof(m_BufferFromHostTx_Output), &msg_len, VCHI_FLAGS_NONE) == 0)
                    {
                    switch (m_BufferFromHostTx_Output->hdr.type) // FIRST AXIS: message type (semantic meaning)
                        {
                        case MMAL_MSG_TYPE_BUFFER_TO_HOST:
                            {
                            switch (m_BufferFromHostTx_Output->hdr.status)   // SECOND AXIS: status (outcome)
                                {
                                case MMAL_MSG_STATUS_SUCCESS:
                                    {
                                    u32 m_CurrentHandle = m_BufferFromHostTx_Output->msg.buffer_header.data;  // Payload layout reused: buffer_from_host
#ifdef __LOG_MMAL__   
                                    storeLog( MY_BUFFER, MY_INDEX, "frame offset", nal_block_offset, "length", nal_block_length, "status", m_BufferFromHostTx_Output->hdr.status, "handle", m_CurrentHandle);
#endif 
                                    if (m_CurrentHandle != m_output_buffer_handle_a && m_CurrentHandle != m_output_buffer_handle_b)
                                        {
                                        continue; // as long as we really want to drain, break; will only proceed the switch (m_RxMsg.h.status) // unrelated buffer, keep draining
                                        }

                                    if (m_CurrentHandle == m_output_buffer_handle_a) // Ping-pong: requeue the other output buffer
                                        {
                                        if (!queueOutputBufferMMAL(*m_BufferFromHostTx_Output, m_output_buffer_handle_b, m_frameBlockSizeB )) // m_OutputBufferSize correct? or a?
                                            return false;
                                        }
                                    if (m_CurrentHandle == m_output_buffer_handle_b) 
                                        {
                                        if (!queueOutputBufferMMAL(*m_BufferFromHostTx_Output, m_output_buffer_handle_a, m_frameBlockSizeA )) // m_OutputBufferSize
                                            return false;
                                        }
                                    if (!bufferReadyMMAL(m_CurrentHandle))
                                        return false;
                                    if (!queueInputBufferMMAL(*m_BufferFromHostTx_Input, nal_block_offset, nal_block_length))
                                        return false;
#ifdef __LOG_MMAL__   
                                    message = "MMAL_MSG_STATUS_SUCCESS      - All is Fine";
                                    storeLog( MY_BUFFER, MY_INDEX, message, EMPTYLOG, "frame offset", nal_block_offset, "length", nal_block_length);
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
#endif                   
                                default:                         message = "Unknown MMAL status          - WTF!!!                             "; break;
                                }
#ifdef __LOG_MMAL__   
                        //  nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message, EMPTYLOG, "Frame offset", nal_block_offset, "length", nal_block_length);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (BUFFER_TO_HOST)", &m_BufferFromHostTx_Output, msg_len);
#endif               
                            return false;
                            }

                        default: 
                            {
                            message = "UNEXPECTED MESSAGE";
#ifdef __LOG_MMAL__    
                        //  nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message,  EMPTYLOG, "Frame offset", nal_block_offset, "Type",  m_BufferFromHostTx_Output->hdr.type, "Status", m_BufferFromHostTx_Output->hdr.status);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (UNEXPECTED MESSAGE)", &m_BufferFromHostTx_Output, msg_len);
#endif 
                            break;
                            }
                        }
#ifdef __LOG_MMAL__ 
                //  nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "Unexpected Reply", EMPTYLOG, "Frame offset", nal_block_offset, "length", nal_block_length);
                    storeMsg( MY_BUFFER, MY_INDEX, "Unexpected Reply", &m_BufferFromHostTx_Output, msg_len);
#endif       
                    }
#ifdef __LOG_MMAL__   
            //  nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "Nothing in the Pipeline", EMPTYLOG, "Frame offset", nal_block_offset, "length", nal_block_length);    // Nothing relevant received
#endif 
                return true;
}

bool            CKernel::bufferReadyMMAL            (   u32 handle)
{
        //      if (handle != m_VCSMHandleA && handle != m_VCSMHandleB)         // Only react to our two output buffers
        //          return true;                                                // why???? we have filtered before ad infiniti 
                if (m_EGLimage != EGL_NO_IMAGE_KHR)                             // Destroy previous EGLImage (if any)
                    {
                    eglDestroyImageKHR(m_eglDisplay, m_EGLimage);
                    m_EGLimage = EGL_NO_IMAGE_KHR;
                    }
                egl_image_brcm_vcsm_info info =                                 // Describe the finished VCSM buffer
                    {
                    .width       = m_ResolutionX,
                    .height      = m_ResolutionY,
                    .vcsm_handle = handle
                    };
                m_EGLimage = eglCreateImageKHR( m_eglDisplay, m_eglContext, EGL_IMAGE_BRCM_VCSM, (EGLClientBuffer)&info, nullptr ); // Create new EGLImage viewing this buffer
                if (m_EGLimage == EGL_NO_IMAGE_KHR)
                    {
#ifdef __LOG_MMAL__        
                    storeLog( MY_BUFFER, MY_INDEX, "EGLImage creation FAILED", handle);
#endif 
                    return false;
                    }
                glBindTexture(GL_TEXTURE_2D, m_Texture);                    // Bind the EGLImage to the single public texture
                glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimage);
                glBindTexture(GL_TEXTURE_2D, 0);

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
                storeLog( MY_BUFFER, MY_INDEX, "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueOutputBuffer", &tx, (u32)sizeof(tx));
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
                storeLog( MY_BUFFER, MY_INDEX, "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueInputBuffer", &tx, (u32)sizeof(tx));
#endif 
                return (vchi_msg_queue(m_ServiceHandleMMAL, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}

