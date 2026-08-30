#pragma once

#include <stdint.h>
#include <stddef.h>

#include "interface/vchi/vchi.h"
#include "interface/vcos/vcos.h"
#include "interface/mmal/mmal.h"
#include "interface/mmal/vc/mmal_vc_api.h"
#include "interface/vc_sm/vc_sm.h"

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "bcm_host.h"

// ---- GL / shader state ----
struct glsl_state
{
    EGLDisplay      display;
    EGLSurface      surface;
    EGLContext      context;

    u32             screen_width;
    u32             screen_height;

    DISPMANX_DISPLAY_HANDLE_T   dispman_display;
    DISPMANX_ELEMENT_HANDLE_T   dispman_element;

    GLuint          gl_vsh_id[128];
    GLuint          gl_fsh_id[128];
    GLuint          gl_prg_id[128];

    GLuint          gl_oms_id[1];
    GLuint          gl_omp_id[1];

    GLuint          gl_tex_id[128];
    GLuint          gl_omt_id[1];

    GLuint          gl_buf;
    GLint           gl_vtx;

    GLint           u_time[128];
    GLint           u_tres[128];
    GLint           u_seed[128];
    GLint           u_aud[128];
    GLint           u_col[128];
    GLint           u_par_a[128];
    GLint           u_par_b[128];

    GLint           u_tex_l[128];
    GLint           u_tex_id[128][10];

    GLint           u_atlas[1];
    GLint           u_tile_count[1];
    GLint           u_tile_rect[1];
    GLint           u_tile_index[1];
};

// ---- extern shader data ----
extern char* m_bufferVsh[1];
extern char* m_bufferFsh[128];
extern char* m_bufferOmf[1];
extern char* m_bufferOmt[1];
extern char* m_bufferTex[128];

extern char* g_ScnFsh[128];
extern u32   g_bytFsh[128];

extern int   m_shaderStatusFlags[128];

// ---- runtime state ----
extern int   g_current_gl_program;
extern int   g_currentProgramBuffer;

extern float g_inOutMatrixFlt[8][16];

extern int   p_validTextureCount;
extern int   gl_current_tex;

extern float GLtime;
extern float g_opaque;

extern int   noTargetFPS;
extern bool  resetFlag;

// ---- parsers ----

    bool m_tex_valid[128];
    u32  m_tex_width[128];
    u32  m_tex_height[128];
    u32  m_tex_data_offset[128];

    bool m_tex_valid[1];
    u32  m_tex_width[1];
    u32  m_tex_height[1];
    u32  m_tex_data_offset[1];

public:

    // ---- shader / program logging ----
    void shaderLog(GLint shader, int shaderIndex);
    void programLog(GLint shader, int program_index);

    // ---- debug ----
    void gfx_check(const char* file, unsigned line);

    // ---- init ----
    void gfx_init_OGL(glsl_state* m_glsl, char* buffer, u32& index);

    void initVshaders(glsl_state* m_glsl, int from, int to, char* buffer, u32& index);
    void initOshader (glsl_state* m_glsl, char* buffer, u32& index);
    void initFshaders(glsl_state* m_glsl, int from, int to, char* buffer, u32& index);

    void initOprogram (glsl_state* m_glsl, char* buffer, u32& index);
    void initFprograms(glsl_state* m_glsl, int from, int to, char* buffer, u32& index);

    void initOuniforms(glsl_state* m_glsl, char* buffer, u32& index);
    void initFuniforms(glsl_state* m_glsl, int from, int to, char* buffer, u32& index);

    void initOtexture (glsl_state* m_glsl, char* buffer, u32& index);
    void initUtextures(glsl_state* m_glsl, int from, int to, char* buffer, u32& index);

    void initVbuffer(glsl_state* m_glsl, char* buffer, u32& index);

    // ---- render ----
    void render_shader_a(glsl_state* m_glsl);
    void render_shader_b(glsl_state* m_glsl);