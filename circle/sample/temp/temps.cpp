Correct approach

Integrate MMAL texture into tex_state, not outside.

Extend tex_state
GLuint gl_tex_vid;      // video texture handle
EGLImageKHR egl_img;    // backing (changes per frame)

Unified init (same style as others)

Refactor your MMAL init to match:

bool CKernel::initTextureVideo(tex_state* t)
{
    glGenTextures(1, &t->gl_tex_vid);
    glBindTexture(GL_TEXTURE_2D, t->gl_tex_vid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
Unified update (same pipeline style)
bool CKernel::updateTextureVideo(tex_state* t, u32 handle)
{
    if (t->egl_img != EGL_NO_IMAGE_KHR)
    {
        eglDestroyImageKHR(m_eglDisplay, t->egl_img);
        t->egl_img = EGL_NO_IMAGE_KHR;
    }

    egl_image_brcm_vcsm_info info =
    {
        .width       = m_ResolutionX,
        .height      = m_ResolutionY,
        .vcsm_handle = handle
    };

    t->egl_img = eglCreateImageKHR(m_eglDisplay,
                                  m_eglContext,
                                  EGL_IMAGE_BRCM_VCSM,
                                  (EGLClientBuffer)&info,
                                  nullptr);

    if (t->egl_img == EGL_NO_IMAGE_KHR)
        return false;

    glBindTexture(GL_TEXTURE_2D, t->gl_tex_vid);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, t->egl_img);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
Integration into render pipeline

Now it behaves like any texture:

setTexPrg(...);

Decision point inside:

if (use_video)
{
    bind t->gl_tex_vid
}
else
{
    bind t->gl_tex_id[]
}