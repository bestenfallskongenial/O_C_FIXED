#include "h264_decoder.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CH264Decoder::bufferReady(u32 handle)
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
#ifdef __H264_DECODER_DEBUG_RUNTIME__        
                    storeLog("\nEGLImage creation FAILED", handle);
#endif
                    return false;
                    }
                glBindTexture(GL_TEXTURE_2D, m_Texture);                    // Bind the EGLImage to the single public texture
                glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimage);
                glBindTexture(GL_TEXTURE_2D, 0);

                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::queueOutputBuffer     (   MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;  /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.data                          = vc_handle;
                tx.msg.buffer_header.alloc_size                    = alloc_size;
                tx.msg.buffer_header.length                        = 0;
                tx.msg.buffer_header.offset                        = 0;
                tx.msg.buffer_header.flags                         = 0;

#ifdef __H264_DECODER_DEBUG_RUNTIME__
                storeLog("\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg("QueueOutputBuffer", &tx, (u32)sizeof(tx));
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CH264Decoder::queueInputBuffer      (   MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;                /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.offset                        = frame_offset;
                tx.msg.buffer_header.length                        = frame_length;

                u32 flags                                           = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;
                tx.msg.buffer_header.flags                         = flags;
#ifdef __H264_DECODER_DEBUG_RUNTIME__
                storeLog("\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg("QueueInputBuffer", &tx, (u32)sizeof(tx));
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------