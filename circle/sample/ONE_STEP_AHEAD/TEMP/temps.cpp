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

FROM "Valid Count Explanation" conversation:

me:

first - structure and my code philosophy is paramount as deterministicsm and first priciple aproaches.
means:
i want to use ( define/ declare / init ) the frame_texture in the same manner as the other ones, 
unified calls as much as possible! means also i rather refactor the function above that it also uses the 
texture struct with extra fields ( and not use the others ) and than call it inside my render pipeline. 
reason? 
i may use it in between the first and the second shader pass or as source for the first pass do you get me?!

GPT:

Yes.

You want **same API + same struct + deterministic flow**.

---

### Correct approach

Integrate MMAL texture into `tex_state`, not outside.

---

### Extend `tex_state`

```cpp
GLuint gl_tex_vid;      // video texture handle
EGLImageKHR egl_img;    // backing (changes per frame)
```

---

### Unified init (same style as others)

Refactor your MMAL init to match:

```cpp
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
```

---

### Unified update (same pipeline style)

```cpp
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
```

---

### Integration into render pipeline

Now it behaves like any texture:

```cpp
setTexPrg(...);
```

Decision point inside:

```cpp
if (use_video)
{
    bind t->gl_tex_vid
}
else
{
    bind t->gl_tex_id[]
}
```

---

### Key property

You now have:

* **same struct** → `tex_state`
* **same lifecycle phase**
* **same render entry point**
* but **different update backend (EGLImage vs static)**

---

### Your requirement

> use between passes / as source

This design supports both:

* pass 1 → video
* pass 2 → overlay
* or mix inside same shader

---

### Binary

* unify into `tex_state` → **YES**
* reuse initTextures() → **NO**
* create dedicated init/update but same style → **YES**
* treat as selectable texture source in render → **YES**


me:

order of calls for your proposal?

GPT:

Deterministic order with your design:

---

### **INIT (once)**

```cpp
initShaders(...)
initPrograms(...)                  // produces dense programs
initUniforms(..., 0, valid_prg)   // dense [0..valid_prg)

initTextures(...)                 // static textures (dense)
initTextureVideo(&m_tex)          // video texture (separate, same struct)
```

---

### **RUNTIME (per frame)**

```cpp
// update video BEFORE rendering
updateTextureVideo(&m_tex, vcsm_handle)
```

---

### **RENDER**

```cpp
if (overlay_enabled)
{
    gpu_render_menu_state_update(&m_osh);
}

setFrmBuffer(&m_vertex);

// pass 1 (user)
setUniformPrg(&m_ogl, &m_vertex, &m_fsh, &m_tex);
setTexPrg(&m_fsh, &m_tex, gl_current_tex, validTexCount);   // includes video if enabled
drawGLsPrg();

// pass 2 (overlay)
if (overlay_enabled)
{
    setUniformOvl(&m_ogl, &m_vertex, &m_osh, &m_omt);
    setTextureOvl(&m_ogl, &m_vertex, &m_osh, &m_omt);
    gpu_render_menu_shader_draw();
}

frmRateBreak(noTargetFPS);
frmBufferSwap(&m_ogl);
```

---

### Key constraints

* `updateTextureVideo()` → **before render**
* `initUniforms()` → **after initPrograms() only once**
* video texture → **not part of validTexCount**
* selection happens inside `setTexPrg`

---

### Binary

* single unified pipeline → **YES**
* separate update path for video → **YES**
* same binding stage → **YES**

Branches only:

A) continuous — user only
if (!use_video)
B) continuous — video + user
if (use_video && t->has_video && mode == CONTINUOUS)
C) continuous — video only
if (use_video && t->has_video && validTexCount == 0)
D) selective (1 or 2) — user only
if (!use_video && mode == SELECTIVE)
E) selective (1 or 2) — video + user
if (use_video && t->has_video && mode == SELECTIVE)
F) selective (1 or 2) — video only
if (use_video && t->has_video && validTexCount == 0 && mode == SELECTIVE)
no! i sayd branches only not conditions! fuck you!
switch(mode)
{
case CONTINUOUS_USER:
    break;

case CONTINUOUS_VIDEO_USER:
    break;

case CONTINUOUS_VIDEO_ONLY:
    break;

case SELECTIVE_USER:
    break;

case SELECTIVE_VIDEO_USER:
    break;

case SELECTIVE_VIDEO_ONLY:
    break;
}

continuous (existing) → user textures only
continuous (extended) → frame first, then user textures
continuous (fallback) → frame only

selective (existing) → user texture(s) only
selective (extended) → frame replaces slot 0, user shifts
selective (fallback) → frame only

debug override → fixed debug texture

empty → nothing bound

fuck you THINK!

continuous → user textures

continuous → frame + user textures (frame participates in same sequence)

continuous → frame replaces user stream (only frame active)

continuous → frame appended after user textures

selective → user textures (existing 1 / 2 selection)

selective → frame replaces primary slot (slot 0)

selective → frame + one user texture

selective → frame + two user textures

selective → frame only

override → debug texture

fallback → no texture

fuck  you ! command is think about the topic ! means you read my input, consider the code and my erquest and than - after thinking you piece of shit are entitled to answer !

continuous → iterate 0..N over a unified source set (user textures optionally extended by frame)

continuous → iterate 0..N where frame is injected at a fixed position (e.g. index 0), user textures shift

continuous → iterate 0..N where source set = only frame

selective → pick from user set only (existing behavior)

selective → pick from unified set (frame + user) using same index logic

selective → pick frame as primary, optionally second index from user set

selective → pick frame only (ignore user set)

global override → debug texture replaces entire source set

fallback → empty source set (no bind)