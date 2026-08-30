#pragma once

#include <stddef.h>
#include <stdint.h>

// ---- GL / EGL / BCM stubs ----
typedef int             GLint;
typedef unsigned int    GLuint;
typedef unsigned int    GLenum;
typedef int             GLsizei;
typedef float           GLfloat;
typedef void            GLvoid;
typedef int             EGLBoolean;
typedef void*           EGLDisplay;
typedef void*           EGLSurface;
typedef void*           EGLContext;
typedef void*           EGLConfig;
typedef int             EGLint;

typedef void*           DISPMANX_UPDATE_HANDLE_T;

struct VC_RECT_T
{
    int x;
    int y;
    int width;
    int height;
};

struct EGL_DISPMANX_WINDOW_T
{
    int element;
    int width;
    int height;
};

// ---- GL constants (stubs) ----
#define GL_TRUE 1
#define GL_FALSE 0
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_OUT_OF_MEMORY 0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_DEPTH_BUFFER_BIT 0x0100

#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_TRIANGLE_FAN 0x0006

#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE0 0x84C0

#define GL_RGB 0x1907
#define GL_UNSIGNED_BYTE 0x1401

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_REPEAT 0x2901
#define GL_LINEAR 0x2601

#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_VIEWPORT 0x0BA2
#define GL_FRAMEBUFFER_BINDING 0x8CA6

#define EGL_DEFAULT_DISPLAY 0
#define EGL_NO_DISPLAY 0
#define EGL_NO_CONTEXT 0
#define EGL_NO_SURFACE 0
#define EGL_WINDOW_BIT 0x0004
#define EGL_OPENGL_ES_API 0x30A0

#define EGL_RED_SIZE 0x3024
#define EGL_GREEN_SIZE 0x3023
#define EGL_BLUE_SIZE 0x3022
#define EGL_ALPHA_SIZE 0x3021
#define EGL_SURFACE_TYPE 0x3033
#define EGL_CONTEXT_CLIENT_VERSION 0x3098
#define EGL_NONE 0x3038

#define DISPMANX_PROTECTION_NONE 0
#define DISPMANX_NO_ROTATE 0

// ---- GL / EGL function stubs ----
void glGetShaderiv(GLint, GLenum, GLint*);
void glGetProgramiv(GLint, GLenum, GLint*);
void glGetProgramInfoLog(GLint, GLsizei, GLsizei*, char*);
void glGetActiveUniform(GLint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, char*);
GLint glGetUniformLocation(GLint, const char*);
void glGetActiveAttrib(GLint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, char*);
GLint glGetAttribLocation(GLint, const char*);
GLenum glGetError();

void glGetIntegerv(GLenum, GLint*);
void glUseProgram(GLuint);

GLuint glCreateShader(GLenum);
void glShaderSource(GLuint, GLsizei, const char**, const GLint*);
void glCompileShader(GLuint);

GLuint glCreateProgram();
void glAttachShader(GLuint, GLuint);
void glLinkProgram(GLuint);
void glDeleteProgram(GLuint);

void glGenTextures(GLsizei, GLuint*);
void glBindTexture(GLenum, GLuint);
void glTexParameteri(GLenum, GLenum, GLint);
void glTexImage2D(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
void glActiveTexture(GLenum);

void glGenBuffers(GLsizei, GLuint*);
void glBindBuffer(GLenum, GLuint);
void glBufferData(GLenum, size_t, const void*, GLenum);
void glVertexAttribPointer(GLuint, GLint, GLenum, int, GLsizei, const void*);
void glEnableVertexAttribArray(GLuint);

void glClearColor(float, float, float, float);
void glClear(GLenum);
void glViewport(GLint, GLint, GLsizei, GLsizei);
void glDrawArrays(GLenum, GLint, GLsizei);
void glBindFramebuffer(GLenum, GLuint);

void glUniform1f(GLint, float);
void glUniform2f(GLint, float, float);
void glUniform4f(GLint, float, float, float, float);
void glUniform1i(GLint, int);

void glFlush();
void glFinish();

// ---- EGL / BCM stubs ----
EGLDisplay eglGetDisplay(int);
EGLBoolean eglInitialize(EGLDisplay, EGLint*, EGLint*);
EGLBoolean eglChooseConfig(EGLDisplay, const EGLint*, EGLConfig*, EGLint, EGLint*);
EGLBoolean eglBindAPI(EGLint);
EGLContext eglCreateContext(EGLDisplay, EGLConfig, EGLContext, const EGLint*);
EGLSurface eglCreateWindowSurface(EGLDisplay, EGLConfig, void*, const EGLint*);
EGLBoolean eglMakeCurrent(EGLDisplay, EGLSurface, EGLSurface, EGLContext);
EGLBoolean eglSwapBuffers(EGLDisplay, EGLSurface);

int graphics_get_display_size(int, uint32_t*, uint32_t*);
int vc_dispmanx_display_open(int);
DISPMANX_UPDATE_HANDLE_T vc_dispmanx_update_start(int);
int vc_dispmanx_element_add(DISPMANX_UPDATE_HANDLE_T, int, int, VC_RECT_T*, int, VC_RECT_T*, int, void*, void*, int);
int vc_dispmanx_update_submit_sync(DISPMANX_UPDATE_HANDLE_T);

// ---- external globals ----
extern bool         resetFlag;
extern unsigned     g_current_gl_program;
extern unsigned     g_currentProgramBuffer;
extern unsigned     g_linked_programs_counter;
extern unsigned     p_validTextureCount;
extern unsigned     gl_current_tex;
extern float        GLtime;
extern float        g_opaque;

extern float        g_inOutMatrixFlt[8][16];
extern int          g_centralModeBuffer[64][16];

extern char*        m_bufferVsh[];
extern char*        m_bufferFsh[];
extern char*        m_bufferOmf[];
extern char*        m_bufferOmt[];
extern char*        m_bufferTex[];

extern char*        g_ScnFsh[];
extern unsigned     g_bytFsh[];

// ---- parser stubs ----
struct H264ParserStub
{
    bool    m_tex_valid[64];
    int     m_tex_data_offset[64];
    int     m_tex_width[64];
    int     m_tex_height[64];
};

struct H264SystemParserStub
{
    bool    m_tex_valid[1];
    int     m_tex_data_offset[1];
    int     m_tex_width[1];
    int     m_tex_height[1];
};

// ---- glsl state ----
struct glsl_state
{
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    uint32_t screen_width;
    uint32_t screen_height;

    int dispman_display;
    int dispman_element;

    GLuint gl_vsh_id[64];
    GLuint gl_fsh_id[64];
    GLuint gl_prg_id[64];

    GLuint gl_oms_id[1];
    GLuint gl_omp_id[1];

    GLuint gl_tex_id[64];
    GLuint gl_omt_id[1];

    GLuint gl_buf;
    GLuint gl_vtx;

    GLint u_time[64];
    GLint u_tres[64];
    GLint u_seed[64];
    GLint u_aud[64];
    GLint u_col[64];
    GLint u_par_a[64];
    GLint u_par_b[64];
    GLint u_tex_l[64];
    GLint u_tex_id[64][10];

    GLint u_atlas[1];
    GLint u_tile_count[1];
    GLint u_tile_rect[1];
    GLint u_tile_index[1];
};

// ---- kernel ----
class CKernel
{
public:

    // ---- members ----
    bool                    m_shaderStatusFlags[64];

    H264ParserStub          m_H264Parser;
    H264SystemParserStub    m_H264SystemParser;

    // ---- logging ----
    void shaderLog(GLint shader, int shaderIndex);
    void programLog(GLint shader, int program_index);

    // ---- debug ----
    void gfx_check(const char* file, unsigned line);

    // ---- init ----
    void gfx_init_OGL(glsl_state* m_glsl, char* buffer, uint32_t& index);

    void initVshaders(glsl_state* m_glsl, int p_fromFile, int p_toFile, char* buffer, uint32_t& index);
    void initOshader(glsl_state* m_glsl, char* buffer, uint32_t& index);
    void initFshaders(glsl_state* m_glsl, int p_fromFile, int p_toFile, char* buffer, uint32_t& index);

    void initOprogram(glsl_state* m_glsl, char* buffer, uint32_t& index);
    void initFprograms(glsl_state* m_glsl, int p_fromFile, int p_toFile, char* buffer, uint32_t& index);

    void initOuniforms(glsl_state* m_glsl, char* buffer, uint32_t& index);
    void initFuniforms(glsl_state* m_glsl, int p_fromFile, int p_toFile, char* buffer, uint32_t& index);

    void initOtexture(glsl_state* m_glsl, char* buffer, uint32_t& index);
    void initUtextures(glsl_state* m_glsl, int p_fromFile, int p_toFile, char* buffer, uint32_t& index);

    void initVbuffer(glsl_state* m_glsl, char* buffer, uint32_t& index);

    // ---- render ----
    void render_shader_a(glsl_state* m_glsl);
    void render_shader_b(glsl_state* m_glsl);
};