`:codex-annotation{index="1"}` enables the **GLES 2.0 extension** that defines `samplerExternalOES`. It belongs to GLES 2.0-era VideoCore, so GLES 2.0 itself is compatible.

Confirm it at runtime:

```cpp
const char* extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
```

The returned string must contain:

```text
GL_OES_EGL_image_external
```

Your shader then requires it explicitly:

```glsl
#extension GL_OES_EGL_image_external : require

uniform samplerExternalOES vid;
```

Regarding cost: converting the external texture into `GL_TEXTURE_2D` requires an additional render pass:

```text
external video texture
→ texture sampling
→ fragment processing
→ framebuffer write into GL_TEXTURE_2D
```

That consumes GPU processing time and memory bandwidth. It is similar to your backbuffer copy conceptually, but it is **not free**.

Direct sampling through `samplerExternalOES vid` avoids that additional pass. Therefore:

```text
Best performance:
samplerExternalOES vid → your actual visualizer shader

Compatibility with tex[x]:
samplerExternalOES vid → extra render pass → sampler2D tex[x]
```



You mean: **after EGL exposes the opaque frame, convert it once on the GPU into an ordinary `GL_TEXTURE_2D`, so the existing visualizer can access it through `sampler2D tex[x]`.**

That has a real GPU cost: one 640×480 texture sample and one 640×480 framebuffer write for every decoded frame. It does not require CPU copying.

The extra pass is:

```text
opaque MMAL handle
→ EGL_IMAGE_BRCM_MULTIMEDIA
→ GL_TEXTURE_EXTERNAL_OES
→ small conversion shader
→ FBO-attached GL_TEXTURE_2D
→ existing sampler2D tex[x]
```

The conversion shader:

```glsl
#extension GL_OES_EGL_image_external : require

precision mediump float;

uniform samplerExternalOES vid;
varying vec2 texcoord;

void main()
{
    gl_FragColor = texture2D(vid, texcoord);
}
```

The resulting `GL_TEXTURE_2D` is then bound through your existing model:

```cpp
glActiveTexture(GL_TEXTURE0 + video_texture_index);
glBindTexture(GL_TEXTURE_2D, m_tex.gl_tex_vid_2d);

glUniform1i(
    m_tex.u_tex_id[g_gl_program_current][video_texture_index],
    video_texture_index);
```

Your user shader continues to contain only:

```glsl
uniform sampler2D tex[8];
```

and samples:

```glsl
texture2D(tex[video_texture_index], uv);
```

During the conversion pass, the external source can temporarily use texture unit 0. The main visualizer later reuses that unit for its ordinary `GL_TEXTURE_2D`. Therefore the external sampler does **not** permanently consume an additional unit in the visualizer.

The conversion function itself is:

```cpp
void            CKernel::convertVideoTexture        (   )
{
                glBindFramebuffer(GL_FRAMEBUFFER, m_tex.gl_fbo_vid);

                glViewport( 0,
                            0,
                            m_video_width,
                            m_video_height);

                glUseProgram(m_tex.gl_program_vid);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_tex.gl_tex_vid);

                glUniform1i(m_tex.u_tex_vid, 0);

                glBindBuffer(GL_ARRAY_BUFFER, m_vtx.gl_buf);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                glViewport( 0,
                            0,
                            m_ogl.screen_width,
                            m_ogl.screen_height);
}
```

It requires these prepared objects:

```cpp
GLuint                      gl_tex_vid;       // existing external texture
GLuint                      gl_tex_vid_2d;    // resulting ordinary texture
GLuint                      gl_fbo_vid;       // framebuffer writing gl_tex_vid_2d
GLuint                      gl_program_vid;   // conversion shader
GLint                       u_tex_vid;        // external sampler location
```

The ordinary destination texture and framebuffer are initialized once:

```cpp
glGenTextures(1, &m_tex.gl_tex_vid_2d);
glBindTexture(GL_TEXTURE_2D, m_tex.gl_tex_vid_2d);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

glTexImage2D(   GL_TEXTURE_2D,
                0,
                GL_RGBA,
                m_video_width,
                m_video_height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                nullptr);

glGenFramebuffers(1, &m_tex.gl_fbo_vid);
glBindFramebuffer(GL_FRAMEBUFFER, m_tex.gl_fbo_vid);

glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                m_tex.gl_tex_vid_2d,
                0);

glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

The call belongs after `bufferReadyMMAL()` has attached the returned opaque handle and before the visualizer samples `tex[x]`:

```cpp
convertVideoTexture();

setUniPrg(...);
setTexPrg(...);
setTexBackbuffer(...);
drawGLsPrg();
```

Run it only when MMAL returns a new picture. Reusing the same decoded frame requires no repeated conversion.

The other opaque-handle path **does exist** in your headers:

```cpp
EGL_IMAGE_BRCM_MULTIMEDIA_Y
EGL_IMAGE_BRCM_MULTIMEDIA_U
EGL_IMAGE_BRCM_MULTIMEDIA_V
```

The header describes these explicitly as:

```text
Fastpath for creating greyscale textures from a single plane of a
MMAL opaque buffer.
```

That path is:

```text
same opaque handle
→ three EGLImages: Y, U and V
→ three grayscale textures
→ three sampler2D uniforms
→ YUV-to-RGB calculation in the user shader
```

It avoids the intermediate FBO write, but it requires three texture samplers and performs colour conversion inside every visualizer shader. It does not produce one ready-made RGB `sampler2D`.

Therefore the two coherent choices are:

1. `samplerExternalOES vid` directly in every visualizer — least extra work.
2. One external-to-RGBA render pass, then your established `sampler2D tex[x]` interface — cleanest integration with your existing shader contract.

The complete opaque image has no direct, source-supported path to one ordinary RGB `GL_TEXTURE_2D`. The Y/U/V targets are the only alternative opaque-handle targets exposed by your Khronos headers.


