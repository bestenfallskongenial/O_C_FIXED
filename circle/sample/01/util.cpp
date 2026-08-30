//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------
// my adc read and convert function plus audio detection!
void            CKernel::readADC() 
{
                const float maxA = 1023.0;
                const int scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                1023    };  // 5.0V max       

                static int f_ring_buffer[ADC_CHANNELS][ADC_BUFFER] = { 0 };
                static int f_index_ring_buffer;

                const int f_scale = scaleFactors[attenuation];
#ifdef __AUDIO_DETECTION__
                const int AUDIO_THRESHOLD = 160;  // Threshold for audio detection

                const int f_maxBuffer = 33; // is defined as macro later

                static float f_band0[f_maxBuffer] = {0};
                static float f_band1[f_maxBuffer] = {0};
                static float f_band2[f_maxBuffer] = {0};
                static float f_band3[f_maxBuffer] = {0};

                static float sum0 = 0;
                static float sum1 = 0;
                static float sum2 = 0;
                static float sum3 = 0;

                static unsigned char idx0 = 0;
                static unsigned char idx1 = 0;
                static unsigned char idx2 = 0;
                static unsigned char idx3 = 0;

                static uint32_t audio_hold_A = 0;
                static uint32_t audio_hold_B = 0;

                int w0 = g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63; // dont need the modulo! its a retardo saveguard!
                int w1 = g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63;
                int w2 = g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63;
                int w3 = g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63;

                int i0 =  f_index_ring_buffer;
                int i1 = (f_index_ring_buffer - 1) & 3;
                int i2 = (f_index_ring_buffer - 2) & 3;
                int i3 = (f_index_ring_buffer - 3) & 3;

                    g_modeMap[0][0] = 5; // resets the max mode position to 5 ( current number of modes )
                    g_modeMap[1][0] = 5;
                    g_modeMap[2][0] = 5;
                    g_modeMap[3][0] = 5;
#endif // __AUDIO_DETECTION__
                f_ring_buffer[0][f_index_ring_buffer] = ReadMCP3008Raw(0); // m_MCP300X.DoSingleEndedConversionRaw(0);    // Channel 0 - First of pair for audio_sample[0]
#ifdef __AUDIO_DETECTION__
                g_irregularity[0]  =   f_ring_buffer[0][i0] - f_ring_buffer[0][i1] + f_ring_buffer[0][i2] - f_ring_buffer[0][i3];

                if(g_irregularity[0] > AUDIO_THRESHOLD || g_irregularity[0] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[0] = 0;

                    audio_hold_A = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] = v;
                        g_modeMap[1][0] = v;
                        g_modeMap[2][0] = v;
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[0][f_index_ring_buffer] * 0.0009765625f; // -> / 1024.0f;

                    sum0 -= f_band0[idx0];
                    f_band0[idx0] = s;
                    sum0 += s;
                    g_inOutMatrixFlt[0][AU0] = sum0 / w0;

                    ++idx0;
                    if(idx0 == w0) idx0 = 0;

                    sum1 -= f_band1[idx1];
                    f_band1[idx1] = s;
                    sum1 += s;
                    g_inOutMatrixFlt[0][AU1] = sum1 / w1;

                    ++idx1;
                    if(idx1 == w1) idx1 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[0][RAW] =  (f_ring_buffer[0][0] + f_ring_buffer[0][1] + f_ring_buffer[0][2] + f_ring_buffer[0][3]) >>2 ; 

                g_inOutMatrixInt[0][VAL] = (g_inOutMatrixInt[0][RAW] * f_scale) >> 10; //  -> / 1023;                                
                
                g_inOutMatrixFlt[0][VAL] = (g_inOutMatrixInt[0][VAL]) * 0.0009765625f;

                f_ring_buffer[1][f_index_ring_buffer] = ReadMCP3008Raw(1); // m_MCP300X.DoSingleEndedConversionRaw(1);    // Channel 1 - First of pair for audio_sample[1]
#ifdef __AUDIO_DETECTION__
                g_irregularity[1] =    f_ring_buffer[1][i0] - f_ring_buffer[1][i1] + f_ring_buffer[1][i2] - f_ring_buffer[1][i3];

                if(g_irregularity[1] > AUDIO_THRESHOLD || g_irregularity[1] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[1] = 1;

                    audio_hold_B = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] = v;
                        g_modeMap[1][0] = v;
                        g_modeMap[2][0] = v;
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[1][f_index_ring_buffer] * 0.0009765625f;

                    sum2 -= f_band2[idx2];
                    f_band2[idx2] = s;
                    sum2 += s;
                    g_inOutMatrixFlt[0][AU2] = sum2 / w2;

                    ++idx2;
                    if(idx2 == w2) idx2 = 0;

                    sum3 -= f_band3[idx3];
                    f_band3[idx3] = s;
                    sum3 += s;
                    g_inOutMatrixFlt[0][AU3] = sum3 / w3;

                    ++idx3;
                    if(idx3 == w3) idx3 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[1][RAW] =  (f_ring_buffer[1][0] + f_ring_buffer[1][1] + f_ring_buffer[1][2] + f_ring_buffer[1][3]) >>2 ; 

                g_inOutMatrixInt[1][VAL] = (g_inOutMatrixInt[1][RAW] * f_scale) >> 10;                               
                
                g_inOutMatrixFlt[1][VAL] = (g_inOutMatrixInt[1][VAL]) * 0.0009765625f;

                f_ring_buffer[2][f_index_ring_buffer] = ReadMCP3008Raw(2); // m_MCP300X.DoSingleEndedConversionRaw(2);    // Channel 2 - Second of pair for audio_sample[0]
#ifdef __AUDIO_DETECTION__
                g_irregularity[2] =    f_ring_buffer[2][i0] - f_ring_buffer[2][i1] + f_ring_buffer[2][i2] - f_ring_buffer[2][i3];
                
                if(g_irregularity[2] > AUDIO_THRESHOLD || g_irregularity[2] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[0] = 2;

                    audio_hold_A = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[2][f_index_ring_buffer] * 0.0009765625f;

                    sum0 -= f_band0[idx0];
                    f_band0[idx0] = s;
                    sum0 += s;
                    g_inOutMatrixFlt[0][AU0] = sum0 / w0;

                    ++idx0;
                    if(idx0 == w0) idx0 = 0;

                    sum1 -= f_band1[idx1];
                    f_band1[idx1] = s;
                    sum1 += s;
                    g_inOutMatrixFlt[0][AU1] = sum1 / w1;

                    ++idx1;
                    if(idx1 == w1) idx1 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[2][RAW] =  (f_ring_buffer[2][0] + f_ring_buffer[2][1] + f_ring_buffer[2][2] + f_ring_buffer[2][3]) >>2 ; 

                g_inOutMatrixInt[2][VAL] = (g_inOutMatrixInt[2][RAW] * f_scale) >> 10;                 
                
                g_inOutMatrixFlt[2][VAL] = (g_inOutMatrixInt[2][VAL]) * 0.0009765625f;

                f_ring_buffer[3][f_index_ring_buffer] = ReadMCP3008Raw(3); // m_MCP300X.DoSingleEndedConversionRaw(3);    // Channel 3 - Second of pair for audio_sample[1]
#ifdef __AUDIO_DETECTION__
                g_irregularity[3] =    f_ring_buffer[3][i0] - f_ring_buffer[3][i1] + f_ring_buffer[3][i2] - f_ring_buffer[3][i3];

                if(g_irregularity[3] > AUDIO_THRESHOLD || g_irregularity[3] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[1] = 3;

                    audio_hold_B = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[3][f_index_ring_buffer] * 0.0009765625f;

                    sum2 -= f_band2[idx2];
                    f_band2[idx2] = s;
                    sum2 += s;
                    g_inOutMatrixFlt[0][AU2] = sum2 / w2;

                    ++idx2;
                    if(idx2 == w2) idx2 = 0;

                    sum3 -= f_band3[idx3];
                    f_band3[idx3] = s;
                    sum3 += s;
                    g_inOutMatrixFlt[0][AU3] = sum3 / w3;

                    ++idx3;
                    if(idx3 == w3) idx3 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[3][RAW] = (f_ring_buffer[3][0] + f_ring_buffer[3][1] + f_ring_buffer[3][2] + f_ring_buffer[3][3]) >>2 ; 

                g_inOutMatrixInt[3][VAL] = (g_inOutMatrixInt[3][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[3][VAL] = (g_inOutMatrixInt[3][VAL]) * 0.0009765625f;

                f_ring_buffer[4][f_index_ring_buffer] = ReadMCP3008Raw(4); // m_MCP300X.DoSingleEndedConversionRaw(4);    // Channel 4 (no audio detection)

                g_inOutMatrixInt[4][RAW] =  (f_ring_buffer[4][0] + f_ring_buffer[4][1] + f_ring_buffer[4][2] + f_ring_buffer[4][3]) >>2 ; 

                g_inOutMatrixInt[4][VAL] = (g_inOutMatrixInt[4][RAW] * f_scale) >> 10;               
                
                g_inOutMatrixFlt[4][VAL] = (g_inOutMatrixInt[4][VAL]) * 0.0009765625f;

                f_ring_buffer[5][f_index_ring_buffer] = ReadMCP3008Raw(5); // m_MCP300X.DoSingleEndedConversionRaw(5);

                g_inOutMatrixInt[5][RAW] =  (f_ring_buffer[5][0] + f_ring_buffer[5][1] + f_ring_buffer[5][2] + f_ring_buffer[5][3]) >>2 ; 

                g_inOutMatrixInt[5][VAL] = (g_inOutMatrixInt[5][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[5][VAL] = (g_inOutMatrixInt[5][VAL]) * 0.0009765625f;

                f_ring_buffer[6][f_index_ring_buffer] = ReadMCP3008Raw(6); // m_MCP300X.DoSingleEndedConversionRaw(6);

                g_inOutMatrixInt[6][RAW] = (f_ring_buffer[6][0] + f_ring_buffer[6][1] + f_ring_buffer[6][2] + f_ring_buffer[6][3]) >>2 ; 

                g_inOutMatrixInt[6][VAL] = (g_inOutMatrixInt[6][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[6][VAL] = (g_inOutMatrixInt[6][VAL]) * 0.0009765625f;

                f_ring_buffer[7][f_index_ring_buffer] = ReadMCP3008Raw(7); // m_MCP300X.DoSingleEndedConversionRaw(7);

                g_inOutMatrixInt[7][RAW] = (f_ring_buffer[7][0] + f_ring_buffer[7][1] + f_ring_buffer[7][2] + f_ring_buffer[7][3]) >>2 ; 

                g_inOutMatrixInt[7][VAL] = (g_inOutMatrixInt[7][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[7][VAL] = (g_inOutMatrixInt[7][VAL]) * 0.0009765625f;
#ifdef __AUDIO_DETECTION__                
                if(audio_hold_A > 0) --audio_hold_A;
                if(audio_hold_B > 0) --audio_hold_B;
#endif // __AUDIO_DETECTION__
                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// my little helpers 
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::checkUpdate    () // aka is a new firmware present?!
{
                if (filecounter[FT_KLN][FLD_LOADED] == 2 ) // or is 1 correct? 
                    {
                    return true;    // there is the second ( usb ) firmware
                    }
                else
                    {
                    return false;    
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::Update         ()
{
                // assumes:
                // - m_bufferKnl[1] + loaded_bytes_kernel[1] already contain the "new" kernel loaded from usb
                // - m_bufferKnl[0] + loaded_bytes_kernel[0] already contain the fallback kernel loaded from sd ( the running kernel )
                // - filesystem is already mounted by caller

                if (saveFromBuffer(PARTITION_NAME_SD, m_bufferKnl[1], loaded_bytes_kernel[1]))
                    {
                    return true;
                    }

                // fallback attempt (allowed to fail)
                saveFromBuffer(FILENAME_KERNEL, m_bufferKnl[0], loaded_bytes_kernel[0]);

                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::set_pot_routing         (   bool        adc_pot_routing)
{
                m_ChipSelectPin.Write(adc_pot_routing);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// look at readme !! from bcmwatchdog.h 


#ifdef __cplusplus      // why? 
extern "C" {
#endif

/// \brief Read 32-bit value from MMIO address
static inline u32 CKernel::read32 (uintptr nAddress)
{
	return *(u32 volatile *) nAddress;
}
/// \brief Write 32-bit value to MMIO address
static inline void CKernel::write32 (uintptr nAddress, u32 nValue)
{
	*(u32 volatile *) nAddress = nValue;
}
#ifdef __cplusplus
}
#endif

// #include <circle/spinlock.h>

//	static const unsigned MaxTimeoutSeconds = 15;

//  CSpinLock m_SpinLock; // really ?!?!

void            CKernel::watchDogStart (unsigned nTimeoutSeconds)
{
                if (nTimeoutSeconds > MaxTimeoutSeconds)
                    {
                    nTimeoutSeconds = MaxTimeoutSeconds;
                    }
                m_SpinLock.Acquire ();  // really??

                write32 (ARM_PM_WDOG, ARM_PM_PASSWD | ((nTimeoutSeconds << 16) & ARM_PM_WDOG_TIME));

                write32 (ARM_PM_RSTC,   ARM_PM_PASSWD | ARM_PM_RSTC_REBOOT (read32 (ARM_PM_RSTC) & ARM_PM_RSTC_CLEAR));

                m_SpinLock.Release ();  // really??
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::prepParameters       ()        // f_buffer guess here we need much more to do!
{
                for ( int f_buffer=0; f_buffer <= DEFAULT_SLOT; f_buffer++)
                    {
                    g_centralModeBuffer[f_buffer][LF1_WAVE] = 0;
                    g_centralModeBuffer[f_buffer][LF2_WAVE] = 1;
                    g_centralModeBuffer[f_buffer][LF1_MULT] = 3;
                    g_centralModeBuffer[f_buffer][LF2_MULT] = 3;
                    g_centralModeBuffer[f_buffer][SENS_A] = 15;
                    g_centralModeBuffer[f_buffer][SENS_B] = 47;
                    g_centralModeBuffer[f_buffer][SENS_C] = 15;
                    g_centralModeBuffer[f_buffer][SENS_D] = 47;                    
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// NEW generic not condensed valid arrays, max number of files ( macros for example!)
void            CKernel::chooseIndex(int p_channel, int& p_activeIndex, int p_maxCount, bool* flags)
{
                static int p_activeIndex = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][RAW] * p_maxCount >> 10;

                if (flags[f_calculated])
                    {
                    p_activeIndex = f_calculated;
                    }
}
// NEW generic condensed valid arrays, max number of files ( macros for example!)
void            CKernel::chooseIndexD(int p_channel, int& p_activeIndex, int p_maxCount)
{
                int f_calculated = (g_inOutMatrixInt[p_channel][RAW] * p_maxCount) >> 10;

                p_activeIndex = f_calculated;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeModes           ()    // "saver"
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][is_stored] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               

                if (g_centralModeBuffer[g_current_gl_program][is_stored] == true /* && g_currentProgramBuffer != g_current_gl_program */)
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_current_gl_program])); // replaces 16 * sizeof(int)
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else if (g_centralModeBuffer[g_current_gl_program][is_stored] == false /* && g_currentProgramBuffer != DEFAULT_SLOT */)
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::buttonPing(int p_btn_id, int p_pin)
{
                g_buttons_states[p_btn_id][BTN_SINGLE] = 0;
                g_buttons_states[p_btn_id][BTN_DOUBLE] = 0;

                if (CGPIOPin(p_pin, GPIOModeInputPullUp).Read() == BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)
                    {
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_SINGLE] = 1;

                    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
                        {
                        g_buttons_states[p_btn_id][BTN_DOUBLE] = 1;
                        }
                    g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
                    }
                if (CGPIOPin(p_pin, GPIOModeInputPullUp).Read() != BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] != 0)
                    {
                    g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]   = 0;
                    }
                if (g_buttons_states[p_btn_id][BTN_PRESS_START] != 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START]) >= g_long_click_time)
                    {
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]++;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::button_consumer(int p_btn_id) // this is where the magic happens: we need to set the states of menu layer, menu, we need to use one button for bpm input and so on 
{
                if (g_buttons_states[p_btn_id][BTN_SINGLE]) counter += 1;
                if (g_buttons_states[p_btn_id][BTN_DOUBLE]) counter -= 1;

                // long-entry edge (once)
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] == 1)
                    counter += 5;

                // periodic hold event (once per threshold)
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =10)
                    longhold += 1;
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =20)
                    longhold += 2;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// my abstract stuff like random, lfo, bpm needed for glsl shader uniform control
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::randomVec8           (uint32_t p_seed)         // create 8 unique normalised to 1.0 float and to 1024 int values
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;

                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[0][RND] = / p_seed * f_scale;
                g_inOutMatrixInt[0][RND] = ( g_inOutMatrixFlt[0][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[1][RND] = p_seed * f_scale;
                g_inOutMatrixInt[1][RND] = ( g_inOutMatrixFlt[1][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[2][RND] = p_seed * f_scale;
                g_inOutMatrixInt[2][RND] = ( g_inOutMatrixFlt[2][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[3][RND] = p_seed * f_scale;
                g_inOutMatrixInt[3][RND] = ( g_inOutMatrixFlt[3][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[4][RND] = /* (float) */ p_seed * f_scale;
                g_inOutMatrixInt[4][RND] = ( g_inOutMatrixFlt[4][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[5][RND] = /* (float) */ p_seed * f_scale;
                g_inOutMatrixInt[5][RND] = ( g_inOutMatrixFlt[5][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[6][RND] = /* (float) */ p_seed * f_scale;
                g_inOutMatrixInt[6][RND] = ( g_inOutMatrixFlt[6][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[7][RND] = p_seed * f_scale;
                g_inOutMatrixInt[7][RND] = ( g_inOutMatrixFlt[7][RND] * f_max_int);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// here it is - look at defs_struct.h for more!
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::calculate1BPM   (   int p_source, unsigned long   p_triggerTimeClock)       // love to split it but i will need additional parameters right?
{
                unsigned long f_intervalAverage = 0;

                if (p_triggerTimeClock != g_lfoBpmMatrix[p_source][LTIME])                                                          // Process button u_time (instance 0)
                    {
                    g_lfoBpmMatrix[g_lfoBpmMatrix[p_source][TIDX]][TB] = p_triggerTimeClock;
        
                    g_lfoBpmMatrix[0][DB]          =   g_lfoBpmMatrix[1][TB] - g_lfoBpmMatrix[0][TB];   
                    g_lfoBpmMatrix[1][DB]          =   g_lfoBpmMatrix[2][TB] - g_lfoBpmMatrix[1][TB];
                    g_lfoBpmMatrix[2][DB]          =   g_lfoBpmMatrix[3][TB] - g_lfoBpmMatrix[2][TB];

                    if(     g_lfoBpmMatrix[1][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  g_lfoBpmMatrix[2][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  g_lfoBpmMatrix[0][DB]  <   g_lfoBpmMatrix[2][DB] * 1.25f ) // calculates an average and allows 25% play ( quite high right ) 
                        {
                        f_intervalAverage           = ( g_lfoBpmMatrix[0][DB] + g_lfoBpmMatrix[1][DB] + g_lfoBpmMatrix[2][DB]) / 3;
            
                        g_lfoBpmMatrix[p_source][BPM]     =   60000000 / f_intervalAverage;
            
                        g_lfoBpmMatrix[p_source][INTV]    =   f_intervalAverage;
                        g_lfoBpmMatrix[p_source][LBC]     =   m_Timer.GetClockTicks();
                        }
                    g_lfoBpmMatrix[p_source][LTIME]       =   p_triggerTimeClock;

                    g_lfoBpmMatrix[p_source][TIDX]        = ( g_lfoBpmMatrix[p_source][TIDX] + 1) % 4;    
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::predict1Beat ( int p_source, int p_lfoMultIn )  // love to split it but i will need additional parameters right? like LF1_MULT
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                      // Get the current u_time in clock ticks

                if (currentTime >= g_lfoBpmMatrix[p_source][NBT])
                    {
                    g_lfoBpmMatrix[p_source][NBT]         +=  g_lfoBpmMatrix[p_source][INTV];                                                                        // Predict the next beat u_time
                    }
                if (currentTime >= g_lfoBpmMatrix[p_source][NCB]) 
                    {
                    g_lfoBpmMatrix[p_source][LCB]   =   g_lfoBpmMatrix[p_source][NCB];
                    g_lfoBpmMatrix[p_source][NCB]   =   g_lfoBpmMatrix[p_source][NCB] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]); // why again g_lfoMultiplierTMP? isnt it stored already, do we need to back it up?
                    g_lfoBpmMatrix[source0][LMT]    =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]];
                    }
                if ((g_lfoBpmMatrix[p_source][LBCT]!=  g_lfoBpmMatrix[p_source][LBC]))
                    {
                    g_lfoBpmMatrix[p_source][NBT]   =   g_lfoBpmMatrix[p_source][LBC];                                                                      // Reset to current time for new BPM
                    g_lfoBpmMatrix[p_source][LBCT]  =   g_lfoBpmMatrix[p_source][LBC];
                    }
                if (g_lfoBpmMatrix[p_source][LMT]  !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]])
                    {
                    g_lfoBpmMatrix[p_source][LCB]   =   g_lfoBpmMatrix[g_activeBpmChannel][LBC];
                    g_lfoBpmMatrix[p_source][NCB]   =   g_lfoBpmMatrix[g_activeBpmChannel][LBC] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]);
                    g_lfoBpmMatrix[p_source][LMT]   =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]];
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::sample1WaveTable                  ( int p_source, int p_lfoIn, int p_lfoOut ) // love to split it but i will need additional parameters right? like LF1_MULT
{
                unsigned long currentTime       =   m_Timer.GetClockTicks();                                                                          // Get the current u_time in clock ticks why not the start_time_fps_calculation or currentTime from Run()??

                g_lfoBpmMatrix[p_source][ELP]   =   currentTime - g_lfoBpmMatrix[p_source][LCB];
                g_lfoBpmMatrix[p_source][CYL]   =   g_lfoBpmMatrix[p_source][NCB] - g_lfoBpmMatrix[p_source][LCB];                                                    // Total length of the current cycle
                int f_indexA                    =  (g_lfoBpmMatrix[p_source][ELP] * 255) / g_lfoBpmMatrix[p_source][CYL];                                               // 255 is not the amplitude! its the number of samples
                g_lfoBpmMatrix[p_source][SMP]   =   f_indexA > 255 ? 255 : f_indexA;                                                                  // means i need a wraparound - on the other hand: i should have a clear calculation here that will never create a index >255!
                g_inOutMatrixFlt[0][p_lfoOut]   =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMP]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][p_lfoOut]   =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMP]];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
