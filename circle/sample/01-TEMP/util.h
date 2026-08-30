#pragma once

#include <stdint.h>

// ---- basic types ----
typedef uint8_t     u8;
typedef uint32_t    u32;

// ---- enums / indices ----
enum
{
    SENS_A = 12,
    SENS_B,
    SENS_C,
    SENS_D,
    LF1_WAVE = 8,
    LF2_WAVE,
    LF1_MULT,
    LF2_MULT,
    TEX_MODE,
    VID_MODE,
    FRM_MODE,
    is_stored
};

// ---- external constants ----
#define ADC_CHANNELS   8
#define ADC_BUFFER     4
#define DEFAULT_SLOT   33

// ---- globals / externs ----
extern int              g_current_gl_program;
extern int              g_last_gl_program;

extern int              g_centralModeBuffer[64][16];
extern int              g_modeMap[64][16];

extern int              g_inOutMatrixInt[8][16];
extern float            g_inOutMatrixFlt[8][16];

extern int              g_irregularity[8];
extern int              is_audio[2];

extern int              attenuation;
extern int              AUDIO_MENU_HOLD;

extern int              g_loaded_fsh_new;
extern int              m_shaderStatusFlags[128];

extern unsigned long    g_intervalCalculated[2];
extern unsigned long    g_lastBpmCalculation[2];
extern unsigned long    g_lastBpmCalculationTMP[2];

extern unsigned long    g_nextBeatTime[2];
extern unsigned long    g_nextCircleBuffer[2];
extern unsigned long    g_lastCircleBuffer[2];

extern unsigned long    g_elapsedMicroseconds[2];
extern unsigned long    g_cycleLength[2];

extern int              g_sampleIndex[2];
extern int              g_waveTable[16][256];

extern int              g_lfoMultiplier[16];
extern int              g_lfoMultiplierTMP[2];

extern int              g_resultBPM[2];
extern int              g_activeBpmChannel;

extern unsigned int     g_buttons_states[2][5];
extern unsigned long    g_currentTime;
extern unsigned long    g_double_click_time;
extern unsigned long    g_long_click_time;

extern int              counter;
extern int              longhold;

extern int              filecounter[16][16];

// ---- external systems ----
struct MCP300XStub
{
    int DoSingleEndedConversionRaw(int ch);
};

struct TimerStub
{
    unsigned long GetClockTicks();
};

struct MachineInfoStub
{
    const char* GetMachineName();
    const char* GetSoCName();
    unsigned    GetModelMajor();
    unsigned    GetModelRevision();
    unsigned    GetRAMSize();
    unsigned    GetClockRate(int id);
    unsigned    AllocateDMAChannel(int type);
    void        FreeDMAChannel(unsigned ch);
};

struct OptionsStub
{
    int         GetCPUSpeed();
    unsigned    GetSoCMaxTemp();
    unsigned    GetUSBPowerDelay();
    unsigned    GetUSBFullSpeed();
};

struct GPIOPinStub
{
    GPIOPinStub(int pin, int mode);
    int Read();
    void Write(bool v);
};

// ---- kernel ----
class CKernel
{
public:

    // systems
    MCP300XStub     m_MCP300X;
    TimerStub       m_Timer;
    MachineInfoStub m_MachineInfo;
    OptionsStub     m_Options;

    // ---- startup ----
    bool startupScreen(char* buffer, u32& index);

    // ---- adc ----
    void readADC();

    // ---- update ----
    bool checkUpdate();
    bool Update();

    // ---- parameters ----
    void prepParameters();

    // ---- selection ----
    void chooseProgram(int p_channel, int& p_activeShader);
    void chooseTexture(int p_channel, int& p_activeTexture, int& p_validTextureCount);
    int  chooseVideo(int p_channel, int& p_activeVideo, int& p_validVideoCount);
    int  chooseFrame(int p_channel, int& p_activeFrame, int& p_validFrameCount);

    // ---- storage ----
    void storeModesV1();
    void storeModesV2();

    // ---- buttons ----
    void buttonPing(int p_btn_id, int pin);
    void button_consumer(int p_btn_id);

    // ---- generators ----
    void randomVec8(uint32_t p_seed);

    // ---- bpm ----
    void calculate2BPM(unsigned long a, unsigned long b);
    void calculate1BPM(int chn, unsigned long t);

    // ---- prediction ----
    void predictedNextBeat2();
    void predictedNextBeat1(int chn);

    // ---- waveform ----
    void sampleWaveTable();
};

// ---- external helpers ----
bool saveFromBuffer(const char* name, void* buffer, unsigned size);

void screen_clear_screen(u32 color);
void screen_draw_buffer_segment(const char* buf, u32 start, u32 end,
                                unsigned col, unsigned row,
                                u32 fg, u32 bg);

extern GPIOPinStub m_ChipSelectPin;