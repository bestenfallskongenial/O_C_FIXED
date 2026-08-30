#pragma once

#include <stddef.h>
#include <stdint.h>

// ---- reuse / forward ----
struct glsl_state;


// ---- globals ----
extern unsigned        g_current_gl_program;
extern unsigned        g_currentProgramBuffer;
extern unsigned        p_validTextureCount;
extern unsigned        gl_current_tex;
extern float           GLtime;
extern float           g_opaque;

extern float           g_inOutMatrixFlt[8][16];
extern int             g_centralModeBuffer[64][16];

extern unsigned long   g_resultBPM[2];

extern bool            noTargetFPS;

// ---- enums (stubs if missing) ----
enum
{
    rnd = 0,
    AU0, AU1, AU2, AU3,
    OUT
};

enum
{
    TEX_MODE = 0,
    CH0_MODE, CH1_MODE, CH2_MODE, CH3_MODE,
    CH4_MODE, CH5_MODE, CH6_MODE, CH7_MODE
};


    GLuint m_TextureA;

// ---- menu state ----
static const int MENU_GPU_TILE_COUNT = 16;

struct menu_glsl_state
{
    GLuint      gl_omp_id[1];
    GLuint      gl_omt_id[1];

    GLint       u_atlas[1];
    GLint       u_tile_count[1];
    GLint       u_tile_rect[1];
    GLint       u_tile_index[1];

    float       kMenuOrigin[2];
    float       kMenuTileSize[2];
    float       kMenuBackgroundScale[2];

    float       kMenuRelPos[MENU_GPU_TILE_COUNT][2];
    float       kMenuRelSize[MENU_GPU_TILE_COUNT][2];

    float       tile_rect_x[MENU_GPU_TILE_COUNT];
    float       tile_rect_y[MENU_GPU_TILE_COUNT];
    float       tile_rect_w[MENU_GPU_TILE_COUNT];
    float       tile_rect_h[MENU_GPU_TILE_COUNT];

    GLfloat     tile_rect[MENU_GPU_TILE_COUNT * 4];
    GLfloat     tile_index[MENU_GPU_TILE_COUNT];
};

// ---- kernel ----

public:

    // ---- render pipeline split ----
    void render_buffer_setup(glsl_state* m_glsl);
    void render_uniform_setup(glsl_state* m_glsl);
    void render_textures_setup(glsl_state* m_glsl);
    void render_shader_draw(glsl_state* m_glsl);
    void render_buffer_swap(glsl_state* m_glsl);

    // ---- menu ----
    void gpu_render_menu_state_update(menu_glsl_state* m_menu);
    void gpu_render_menu_uniform_setup(menu_glsl_state* m_menu);
    void gpu_render_menu_textures_setup(menu_glsl_state* m_menu);
    void gpu_render_menu_shader_draw();

// ---- frame control ----
void frame_break_mechanism();