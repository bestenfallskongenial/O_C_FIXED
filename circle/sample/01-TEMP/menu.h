#pragma once

#include <stdint.h>

// ---- basic types ----
typedef uint8_t     u8;
typedef uint32_t    u32;

// ---- matrix field indices (stub) ----
enum
{
    in = 0,
    raw,
    val,
    OUT,
    rnd,
    TRH,
    TRL,
    TRF,
    lf1,
    lf2,
    AU0,
    AU1,
    AU2,
    AU3,
    INOUT_FIELD_COUNT
};

// ---- mode slot indices (stub) ----
enum
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
    TEX_MODE,
    VID_MODE,
    FRM_MODE,
    MODE_SLOT_COUNT
};

// ---- globals / extern stubs ----
extern int              g_menu_mode_new;
extern int              g_menu_mode_old;
extern int              modetablecount;

extern bool             g_menuPickUpFlag[64];

extern int              g_currentProgramBuffer;
extern int              g_activeBpmChannel;

extern unsigned long    currentTime;
extern unsigned long    g_nextBeatTime[16];

extern int              g_resultBPM[2];

extern int              g_modeMap[64][16];
extern int              g_centralModeBuffer[64][16];

extern int              g_inOutMatrixInt[8][INOUT_FIELD_COUNT];
extern float            g_inOutMatrixFlt[8][INOUT_FIELD_COUNT];

// ---- kernel ----
class CKernel
{
public:

    void resetMenuPickupFlags();

    void modeMenuAssignGroup(uint8_t menu_id, uint8_t base);

    void applyModeToChannel(int channel);

    void modeADC(int channel);
    void modeTRG(int channel);
    void modeBPM(int channel);
    void modeLF1(int channel);
    void modeLF2(int channel);
    void modeTex(int channel);   // stub from switch
    void modeVid(int channel);   // stub from switch
    void modeFrm(int channel);   // stub from switch

    void modeAudioAb0(int channel);
    void modeAudioAb1(int channel);
    void modeAudioBb0(int channel);
    void modeAudioBb1(int channel);
};