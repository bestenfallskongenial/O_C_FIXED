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

// ---- basic types ----
typedef uint32_t u32;

// ---- file counters ----
extern int filecounter[16][16];

enum
{
    FT_VID,
    FRM_BF,
    FT_OMT,
    FT_TEX,
    FT_KLN,
    FT_VSH,
    FT_OMF,
    FT_FSH,
    LOGGER
};

enum
{
    FLD_MAXSD,
    FLD_MAXUSB,
    FLD_SIZE,
    FLD_EXTCNT,
    FLD_SCANNED,
    FLD_LOADED,
    FLD_PREV
};

// ---- buffers ----
extern char** m_bufferVid;
extern char** m_bufferFrA;
extern char** m_bufferFrB;
extern char** m_bufferOmt;
extern char** m_bufferTex;

extern char** m_bufferKnl;
extern char** m_bufferLog;
extern char** m_bufferVsh;
extern char** m_bufferOmf;
extern char** m_bufferFsh;

// ---- DMA backing ----
extern void* m_videoBlockBase;
extern void* m_videoRawBlock;
extern u32   m_videoBlockSize;

extern void* m_frameBlockBaseA;
extern void* m_frameRawBlockA;
extern u32   m_frameBlockSizeA;

extern void* m_frameBlockBaseB;
extern void* m_frameRawBlockB;
extern u32   m_frameBlockSizeB;

extern void* m_overlyBlockBase;
extern void* m_overlayRawBlock;
extern u32   m_overlyBlockSize;

extern void* m_textureBlockBase;
extern void* m_textureRawBlock;
extern u32   m_textureBlockSize;

// ---- filesystem ----
bool Mount(const char* name);
void UnMount();

void scanRoot(char** OUT,
              const char** suffix,
              int extCount,
              int& scanned,
              int max);

void bulkLoad(char** names,
              u32*   sizes,
              char** buffers,
              int    scanned,
              int&   loaded,
              int&   prev,
              int    maxSize);

// ---- suffix / scan arrays ----
extern char*       g_ScnVsh[128];
extern char*       g_ScnOmf[128];
extern char*       g_ScnFsh[128];
extern char*       g_ScnOmt[128];
extern char*       g_ScnTex[128];
extern char*       g_ScnVid[128];
extern char*       g_ScnKln[128];

extern const char* g_SufVsh[8];
extern const char* g_SufOmf[8];
extern const char* g_SufFsh[8];
extern const char* g_SufOmt[8];
extern const char* g_SufTex[8];
extern const char* g_SufVid[8];
extern const char* g_SufKln[8];

extern u32 g_bytVsh[128];
extern u32 g_bytOmf[128];
extern u32 g_bytFsh[128];
extern u32 g_bytOmt[128];
extern u32 g_bytTex[128];
extern u32 g_bytVid[128];
extern u32 g_bytKln[128];

// ---- alloc / free ----
char** alllocateBufferDMA(int count, int size,
                          void** base,
                          void** raw,
                          u32*   totalSize);

char** alllocateBufferMEM(int count, int size);

void clearBufferDMA(char** buf, void* raw);
void clearBufferMEM(char** buf, int count);

// ---- cache ----
void CleanAndInvalidateDataCacheRange(uintptr_t addr, size_t size);

// ---- GL ----
struct glsl_state;
extern glsl_state m_glsl;

// ---- parsers ----
void parser_bmp(int from, int to);
void parser_h264(int from, int to);

// ---- GL init ----
void initVbuffer   (glsl_state* s);
void initVshaders  (glsl_state* s, int from, int to);
void initOshader   (glsl_state* s, int from, int to);
void initFshaders  (glsl_state* s, int from, int to);
void initOprogram  (glsl_state* s, int from, int to);
void initFprograms (glsl_state* s, int from, int to);
void initOuniforms (glsl_state* s, int from, int to);
void initFuniforms (glsl_state* s, int from, int to);
void initOtexture  (glsl_state* s, int from, int to);
void initUtextures (glsl_state* s, int from, int to);

// ---- IO ----
void readADC();

void chooseProgram(int ch);
void chooseTexture(int ch);
int  chooseVideo(int ch);

void buttonPing(int id, int pin);
void button_consume(int id);

// ---- modes ----
extern int g_current_menu_layer;

void resetMenuPickupFlags();
void modeMenuAssignGroup(int group, int base);
void applyModeToChannel(int ch);
void apply_state_to_led();

// ---- constants ----
#define PARTITION_NAME_SD   "emmc1-1"
#define PARTITION_NAME_USB  "umsd1-1"

#define TEX_LOADED_OLD 0
#define TEX_LOADED_NEW 1
#define VID_LOADED_OLD 0
#define VID_LOADED_NEW 1

#define ADC_SELECT_PRG 0
#define ADC_SELECT_TEX 1
#define ADC_SELECT_VID 2

#define SW_PIN_A 0
#define SW_PIN_B 1

// ---- kernel ----
class CKernel
{
public:

    // ---- DMA / MEM ----
    bool wrapperInitDMA();
    bool wrapperInitMEM();

    void wrapperDMAcleanUp();
    void wrapperMEMcleanUp();

    // ---- load ----
    void wrapper_from_sd();
    void wrapper_load_usb();

    // ---- GL ----
    void wrapper_init_gl_sd();
    void wrapper_init_gl_usb();

    // ---- runtime ----
    void wrapper_io();
    void wrapper_modes();
};