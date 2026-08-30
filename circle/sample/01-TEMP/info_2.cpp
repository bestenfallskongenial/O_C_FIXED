# Derived interfaces from function usage (excluding `kernel.h/.cpp` and `kernel_initialize.cpp`, `kernel_run.cpp`)
# This file derives likely array/enum/struct shapes from call-sites and indexing patterns in the active `.cpp` files.
# This file reconstructs probable enum/array/struct layouts from call-sites and indexing usage only.

# Scope used for derivation:
# - Read: `circle/sample/oculus/README.md`.
# - Scanned only files directly under `circle/sample/oculus` (no subfolders).
# - Explicitly ignored: `kernel.h`, `kernel.cpp`, `kernel_initialize.cpp`, `kernel_run.cpp`.



## 1) Overlay tile struct and fixed arrays

struct MenuTileRect
{
    float x;
    float y;
    float w;
    float h;
};

static constexpr int MENU_GPU_TILE_COUNT = 16;
static const float kMenuRelPos[MENU_GPU_TILE_COUNT][2];
static const float kMenuRelSize[MENU_GPU_TILE_COUNT][2];

MenuTileRect tile_rect[MENU_GPU_TILE_COUNT];
int          tile_index[MENU_GPU_TILE_COUNT];

# Why: overlay render fills `tile_rect[16]`, writes `x/y/w/h`, and uploads it as `glUniform4fv(..., 16, (GLfloat*)tile_rect)`.



## 2) Mode table index enum and central mode buffer

# Observed indices used on `g_centralModeBuffer[program][index]`:
# - channel slots: `CH0_MODE..CH7_MODE`
# - LFO params: `LF1_WAVE`, `LF2_WAVE`, `LF1_MULT`, `LF2_MULT`
# - sensitivity params: `SENS_A..SENS_D`
# - texture mode: `TEX_MODE`
# - frame mode: `FRM_MODE`
# - storage flag: `is_stored`

# Observed as `g_centralModeBuffer[program][mode_slot]` in active function code.
# - First index = program slot (`f_buffer`, `g_currentProgramBuffer`, `g_current_gl_program`).
# - Second index = mode selector token.

# The menu system uses 4 groups × 4 slots (`base=0,4,8,12`), i.e. **16 logical mode slots**.

enum modetable
{
    CH0_MODE = 0,
    CH1_MODE,
    CH2_MODE,
    CH3_MODE,
    CH4_MODE,
    CH5_MODE,
    CH6_MODE,
    CH7_MODE,

    LF1_WAVE,
    LF2_WAVE,
    LF1_MULT,
    LF2_MULT,

    SENS_A,
    SENS_B,
    SENS_C,
    SENS_D,

    TEX_MODE,
    FRM_MODE,
    is_stored,

    modetablecount
};

# Program profile buffer indexed by shader/program id and ModeTableIndex
int g_centralModeBuffer[MAX_PROGRAM_SLOTS][modetablecount];

# Important conflict from usage itself:
# - `modeMenuAssignGroup(... base 12)` implies slot range `[0..15]` is live config data.
# - renderer also uses `TEX_MODE`; mode-store logic uses `is_stored`.
# - therefore final canonical enum either needs >16 entries or a separate metadata table for `is_stored`.

# Important consistency note (from functions only):
# - Function code uses `is_stored` (lowercase) as the `g_centralModeBuffer` state slot.
# - If header/other draft declarations use `IS_STORED`, those declarations are not aligned with these function bodies.



## 3) Button state matrix enum and array

# Direct usage requires exactly 5 per-button fields.

enum ButtonStateField
{
    BTN_SINGLE = 0,
    BTN_DOUBLE,
    BTN_PRESS_START,
    BTN_RELEASE,
    BTN_HOLD_TICK,
    BTN_FIELD_COUNT
};

# 2 buttons (A/B), 5 fields each
unsigned int g_buttons_states[2][BTN_FIELD_COUNT];

# Why: button code resets `SINGLE/DOUBLE`, tracks press start/release, increments hold tick; two physical buttons are polled.

# Notes on naming conflicts found from function use:
# - `button_consumer` function name differs from `button_consume` call-site in wrappers.
# - `button_consumer` uses assignment in conditions (`=10`, `=20`), likely intended comparisons.



## 4) File type / file field enums for `filecounter`

# `filecounter[type][field]` is used by wrappers for scan/load/init paths.

# Used file types:
# `FT_VSH`, `FT_OMF`, `FT_FSH`, `FT_OMT`, `FT_TEX`, `FT_VID`, `FT_KLN`, `FRM_BF`, `LOGGER`

# Used fields:
# `FLD_EXTCNT`, `FLD_SCANNED`, `FLD_MAXSD`, `FLD_MAXUSB`, `FLD_LOADED`, `FLD_PREV`, `FLD_SIZE`

enum FileType
{
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FRM_BF,
    LOGGER,
    FT_COUNT
};

enum FileField
{
    FLD_EXTCNT = 0,
    FLD_SCANNED,
    FLD_MAXSD,
    FLD_MAXUSB,
    FLD_LOADED,
    FLD_PREV,
    FLD_SIZE,
    FLD_COUNT
};

int filecounter[FT_COUNT][FLD_COUNT];



## 5) In/out signal matrix layout

# `g_inOutMatrixFlt[ch][field]` and `g_inOutMatrixInt[ch][field]` are indexed with:
# - base ADC/input pipeline: `in`, `raw`, `val`, `out`
# - random/LFO/audio/event fields: `rnd`, `lf1`, `lf2`
# - audio bands: `au0`, `au1`, `au2`, `au3`
# - trigger fields: `trH`, `trL`, `trF`

enum InOutField
{
    in = 0,
    raw,
    val,
    out,
    rnd,
    trH,
    trL,
    trF,
    lf1,
    lf2,
    au0,
    au1,
    au2,
    au3,
    INOUT_FIELD_COUNT
};

float g_inOutMatrixFlt[8][INOUT_FIELD_COUNT];
int   g_inOutMatrixInt[8][INOUT_FIELD_COUNT];



## 6) Mode map table shape

# Usage pattern:
# - `g_modeMap[channel_or_slot][0]` stores per-slot max selectable range.
# - `g_modeMap[channel][selected + 1]` maps menu selection to execution mode id (`0..11` used in switch).
# - indexed with base offsets up to `base+3` where `base` can be 12.

uint8_t g_modeMap[16][13];

int  g_modeMap[modetablecount][/*1 + max menu choices*/];
bool g_menuPickUpFlag[modetablecount];



## 7) parser tables / waveform/color tables

# Directly defined in source:
unsigned long        g_waveTable[WAVEFORMS][WAVESAMPLES];
const unsigned char  g_rgb_color_table[49][3];

# `sampleWaveTable()` clamps to `0..255`, so `WAVESAMPLES` must be at least `256`.



## 8) GLSL runtime state struct

# Fields required by EGL/OGL init, shader compile/link, uniforms, and draw.

struct glsl_state
{
    # EGL / windowing
    EGLDisplay  display;
    EGLSurface  surface;
    EGLContext  context;
    uint32_t    screen_width;
    uint32_t    screen_height;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_ELEMENT_HANDLE_T dispman_element;

    # GL objects
    GLuint gl_buf;
    GLint  gl_vtx;

    GLuint gl_vsh_id[MAX_SHADERS];
    GLuint gl_fsh_id[MAX_SHADERS];
    GLuint gl_prg_id[MAX_SHADERS];

    # overlay shader/program/texture
    GLuint gl_oms_id[1];
    GLuint gl_omp_id[1];
    GLuint gl_omt_id[1];

    # user textures
    GLuint gl_tex_id[MAX_TEXTURES];

    # uniforms for user shader programs
    GLint u_time [MAX_SHADERS];
    GLint u_tres [MAX_SHADERS];
    GLint u_seed [MAX_SHADERS];
    GLint u_aud  [MAX_SHADERS];
    GLint u_col  [MAX_SHADERS];
    GLint u_par_a[MAX_SHADERS];
    GLint u_par_b[MAX_SHADERS];
    GLint u_tex_l[MAX_SHADERS];
    GLint u_tex_id[MAX_SHADERS][10];

    # uniforms for overlay program
    GLint u_atlas[1];
    GLint u_tile_count[1];
    GLint u_tile_rect[1];
    GLint u_tile_index[1];
};

# Strict lower bounds from usage:
# - `u_tex_id[][0..9]` => second dimension must be at least `10`.
# - overlay arrays accessed at index `[0]` => size at least `1`.



## 9) Buffer pointer families implied by loaders/parsers

# Bulk loaders and init functions require these pointer matrices:

char* m_bufferVsh[/*vsh_file_slots*/];
char* m_bufferOmf[/*omf_file_slots*/];
char* m_bufferFsh[/*fsh_file_slots*/];
char* m_bufferOmt[/*omt_file_slots*/];
char* m_bufferTex[/*tex_file_slots*/];
char* m_bufferVid[/*vid_file_slots*/];
char* m_bufferKnl[/*kln_file_slots*/];



# Plus scan name arrays / byte counters per family:

char*    g_ScnVsh[]; unsigned g_bytVsh[]; const char* g_SufVsh[];
char*    g_ScnOmf[]; unsigned g_bytOmf[]; const char* g_SufOmf[];
char*    g_ScnFsh[]; unsigned g_bytFsh[]; const char* g_SufFsh[];
char*    g_ScnOmt[]; unsigned g_bytOmt[]; const char* g_SufOmt[];
char*    g_ScnTex[]; unsigned g_bytTex[]; const char* g_SufTex[];
char*    g_ScnVid[]; unsigned g_bytVid[]; const char* g_SufVid[];
char*    g_ScnKln[]; unsigned g_bytKln[]; const char* g_SufKln[];



## 10) Notes on naming conflicts / consistency notes found from function use

# - `is_stored` appears as lowercase at runtime call-sites (even if some docs mention uppercase token).
# - `button_consumer` function name differs from `button_consume` call-site in wrappers.
# - `button_consumer` uses assignment in conditions (`=10`, `=20`), likely intended comparisons.

# These are behavioral/code consistency issues but do not change the inferred shapes above.