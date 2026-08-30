bool            CH264Decoder::MMALbufferReady(u32 handle)
{
                if (handle == m_VCSMHandleA)
                    {
                    if (m_EGLimageA)
                        {
                        eglDestroyImageKHR(m_eglDisplay, m_EGLimageA);
                        m_EGLimageA = EGL_NO_IMAGE_KHR;
                        }
                    struct egl_image_brcm_vcsm_info infoA = 
                        {
                        .width       = m_ResolutionX,
                        .height      = m_ResolutionY,
                        .vcsm_handle = m_VCSMHandleA
                        };
                    m_EGLimageA = eglCreateImageKHR(
                        m_eglDisplay, m_eglContext,
                        EGL_IMAGE_BRCM_VCSM, (EGLClientBuffer)&infoA, NULL);

                    if (m_EGLimageA == EGL_NO_IMAGE_KHR)
                        {
                        MMALstoreLog("\nm_EGLimageA FAILED");
                        return false;
                        }
                    // Always bind the public texture
                    glBindTexture(GL_TEXTURE_2D, m_TextureA);
                    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimageA);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    }
                else if (handle == m_VCSMHandleB)
                    {
                    if (m_EGLimageB)
                        {
                        eglDestroyImageKHR(m_eglDisplay, m_EGLimageB);
                        m_EGLimageB = EGL_NO_IMAGE_KHR;
                        }
                    struct egl_image_brcm_vcsm_info infoB = 
                        {
                        .width       = m_ResolutionX,
                        .height      = m_ResolutionY,
                        .vcsm_handle = m_VCSMHandleB
                        };
                    m_EGLimageB = eglCreateImageKHR(
                        m_eglDisplay, m_eglContext,
                        EGL_IMAGE_BRCM_VCSM, (EGLClientBuffer)&infoB, NULL);

                    if (m_EGLimageB == EGL_NO_IMAGE_KHR)
                        {
                        MMALstoreLog("\nm_EGLimageB FAILED");
                        return false;
                        }
                    // Always bind the public texture
                    glBindTexture(GL_TEXTURE_2D, m_TextureA);
                    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimageB);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    }
                return true;
}
bool CH264Decoder::MMALqueueOutputBuffer(mmal_msg_buffer_from_host_wire32& tx_body, u32 vc_handle, u32 alloc_size)
{
    mmal_msg_header tx_hdr = {};

    tx_hdr.magic           = MMAL_MAGIC;
    tx_hdr.type            = MMAL_MSG_TYPE_BUFFER_FROM_HOST;
    tx_hdr.control_service = 0;
    tx_hdr.context         = NextTransId(m_TransactionId);
    tx_hdr.status          = 0;
    tx_hdr.padding         = 0;
    
    tx_body.drvbuf.magic            = MMAL_MAGIC; 
    tx_body.drvbuf.component_handle = m_ComponentHandle;
    tx_body.drvbuf.port_handle      = m_OutputPortHandle;
    tx_body.drvbuf.client_context   = tx_hdr.context;

    tx_body.buffer_header.alloc_size = m_OutputBufferSize;

    tx_body.is_zero_copy       = 1;
    tx_body.has_reference      = 0;
    tx_body.payload_in_message = 0;

    u8 msg[sizeof(tx_hdr) + sizeof(tx_body)];
    memcpy(msg, &tx_hdr, sizeof(tx_hdr));
    memcpy(msg + sizeof(tx_hdr), &tx_body, sizeof(tx_body));

    MMALstoreLog("\nBUFFER FROM HOST MSG", (u32)sizeof(msg),call_counter);
    MMALstoreMsg(msg, (u32)sizeof(msg), "QueueOutputBuffer");
++call_counter;
    return vchi_msg_queue(m_ServiceHandle, msg, (u32)sizeof(msg), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0;
}

bool CH264Decoder::MMALqueueInputBuffer(mmal_msg_buffer_from_host_wire32& tx_body, u32 frame_offset, u32 frame_length)
{
    mmal_msg_header tx_hdr = {};

    tx_hdr.magic           = MMAL_MAGIC;
    tx_hdr.type            = MMAL_MSG_TYPE_BUFFER_FROM_HOST;
    tx_hdr.control_service = 0;
    tx_hdr.context         = NextTransId(m_TransactionId);
    tx_hdr.status          = 0;
    tx_hdr.padding         = 0;
    
    tx_body.drvbuf.magic            = MMAL_MAGIC; 
    tx_body.drvbuf.component_handle = m_ComponentHandle;
    tx_body.drvbuf.port_handle      = m_InputPortHandle;
    tx_body.drvbuf.client_context   = tx_hdr.context;

    tx_body.buffer_header.alloc_size = m_InputBufferSize;
    tx_body.buffer_header.length     = frame_length;
    tx_body.buffer_header.offset     = frame_offset;
    tx_body.buffer_header.flags      = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;

    tx_body.is_zero_copy       = 1;
    tx_body.has_reference      = 0;
    tx_body.payload_in_message = 0;

    u8 msg[sizeof(tx_hdr) + sizeof(tx_body)];
    memcpy(msg, &tx_hdr, sizeof(tx_hdr));
    memcpy(msg + sizeof(tx_hdr), &tx_body, sizeof(tx_body));

    MMALstoreLog("\nBUFFER FROM HOST MSG", (u32)sizeof(msg), call_counter);
++call_counter;    
    MMALstoreLog("\nwhatsgoingonhere?!");
    MMALstoreMsg(msg, (u32)sizeof(msg), "QueueInputBuffer");

    return vchi_msg_queue(m_ServiceHandle, msg, (u32)sizeof(msg), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              END OF FILE
//----------------------------------------------------------------------------------------------------------------------------------------------------