
#include "kernel.h"

void            CKernel::adc_AcquireConvert         (   void    )
{
                const int f_scale = m_scaleFactors[attenuation];

                m_adc_ring[0][m_adc_index] = ReadMCP3008Raw(0);
                g_inOutMatrixInt[0][RAW] = (m_adc_ring[0][0] + m_adc_ring[0][1] + m_adc_ring[0][2] + m_adc_ring[0][3]) >> 2;
                g_inOutMatrixInt[0][VAL] = (g_inOutMatrixInt[0][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[0][VAL] = g_inOutMatrixInt[0][VAL] * 0.0009765625f;

                m_adc_ring[1][m_adc_index] = ReadMCP3008Raw(1);
                g_inOutMatrixInt[1][RAW] = (m_adc_ring[1][0] + m_adc_ring[1][1] + m_adc_ring[1][2] + m_adc_ring[1][3]) >> 2;
                g_inOutMatrixInt[1][VAL] = (g_inOutMatrixInt[1][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[1][VAL] = g_inOutMatrixInt[1][VAL] * 0.0009765625f;

                m_adc_ring[2][m_adc_index] = ReadMCP3008Raw(2);
                g_inOutMatrixInt[2][RAW] = (m_adc_ring[2][0] + m_adc_ring[2][1] + m_adc_ring[2][2] + m_adc_ring[2][3]) >> 2;
                g_inOutMatrixInt[2][VAL] = (g_inOutMatrixInt[2][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[2][VAL] = g_inOutMatrixInt[2][VAL] * 0.0009765625f;

                m_adc_ring[3][m_adc_index] = ReadMCP3008Raw(3);
                g_inOutMatrixInt[3][RAW] = (m_adc_ring[3][0] + m_adc_ring[3][1] + m_adc_ring[3][2] + m_adc_ring[3][3]) >> 2;
                g_inOutMatrixInt[3][VAL] = (g_inOutMatrixInt[3][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[3][VAL] = g_inOutMatrixInt[3][VAL] * 0.0009765625f;

                m_adc_ring[4][m_adc_index] = ReadMCP3008Raw(4);
                g_inOutMatrixInt[4][RAW] = (m_adc_ring[4][0] + m_adc_ring[4][1] + m_adc_ring[4][2] + m_adc_ring[4][3]) >> 2;
                g_inOutMatrixInt[4][VAL] = (g_inOutMatrixInt[4][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[4][VAL] = g_inOutMatrixInt[4][VAL] * 0.0009765625f;

                m_adc_ring[5][m_adc_index] = ReadMCP3008Raw(5);
                g_inOutMatrixInt[5][RAW] = (m_adc_ring[5][0] + m_adc_ring[5][1] + m_adc_ring[5][2] + m_adc_ring[5][3]) >> 2;
                g_inOutMatrixInt[5][VAL] = (g_inOutMatrixInt[5][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[5][VAL] = g_inOutMatrixInt[5][VAL] * 0.0009765625f;

                m_adc_ring[6][m_adc_index] = ReadMCP3008Raw(6);
                g_inOutMatrixInt[6][RAW] = (m_adc_ring[6][0] + m_adc_ring[6][1] + m_adc_ring[6][2] + m_adc_ring[6][3]) >> 2;
                g_inOutMatrixInt[6][VAL] = (g_inOutMatrixInt[6][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[6][VAL] = g_inOutMatrixInt[6][VAL] * 0.0009765625f;

                m_adc_ring[7][m_adc_index] = ReadMCP3008Raw(7);
                g_inOutMatrixInt[7][RAW] = (m_adc_ring[7][0] + m_adc_ring[7][1] + m_adc_ring[7][2] + m_adc_ring[7][3]) >> 2;
                g_inOutMatrixInt[7][VAL] = (g_inOutMatrixInt[7][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[7][VAL] = g_inOutMatrixInt[7][VAL] * 0.0009765625f;
}

void            CKernel::adc_ProcessAudio           (   void    )
{
                if (!m_audio_mode_activated) // is a fixed position in g_centralModeBuffer mapped by modeMenuAssignGroup()

                int i0 = m_adc_index;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

                int w0 = g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63;
                int w1 = g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63;
                int w2 = g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63;
                int w3 = g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63;

                g_irregularity[0] = m_adc_ring[0][i0] - m_adc_ring[0][i1] + m_adc_ring[0][i2] - m_adc_ring[0][i3];

                if (g_irregularity[0] > AUDIO_THRESHOLD || g_irregularity[0] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 0;

                    m_audio_hold_A = AUDIO_MENU_HOLD;
                    m_audio_flag_A = true;

                    float s = m_adc_ring[0][m_adc_index] * 0.0009765625f;

                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixFlt[0][AU0] = m_sum[0] / w0;
                    if (++m_idx0 == w0) m_idx0 = 0;

                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixFlt[0][AU1] = m_sum[1] / w1;
                    if (++m_idx1 == w1) m_idx1 = 0;
                }

                g_irregularity[1] = m_adc_ring[1][i0] - m_adc_ring[1][i1] + m_adc_ring[1][i2] - m_adc_ring[1][i3];

                if (g_irregularity[1] > AUDIO_THRESHOLD || g_irregularity[1] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 1;

                    m_audio_hold_B = AUDIO_MENU_HOLD;
                    m_audio_flag_B = true;

                    float s = m_adc_ring[1][m_adc_index] * 0.0009765625f;

                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixFlt[0][AU2] = m_sum[2] / w2;
                    if (++m_idx2 == w2) m_idx2 = 0;

                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixFlt[0][AU3] = m_sum[3] / w3;
                    if (++m_idx3 == w3) m_idx3 = 0;
                }

                g_irregularity[2] = m_adc_ring[2][i0] - m_adc_ring[2][i1] + m_adc_ring[2][i2] - m_adc_ring[2][i3];

                if (g_irregularity[2] > AUDIO_THRESHOLD || g_irregularity[2] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 2;

                    m_audio_hold_A = AUDIO_MENU_HOLD;
                    m_audio_flag_A = true;

                    float s = m_adc_ring[2][m_adc_index] * 0.0009765625f;

                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixFlt[0][AU0] =  m_sum[0] / w0;
                    if (++m_idx0 == w0) m_idx0 = 0;

                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixFlt[0][AU1] = m_sum[1] / w1;
                    if (++m_idx1 == w1) m_idx1 = 0;
                }

                g_irregularity[3] = m_adc_ring[3][i0] - m_adc_ring[3][i1] + m_adc_ring[3][i2] - m_adc_ring[3][i3];

                if (g_irregularity[3] > AUDIO_THRESHOLD || g_irregularity[3] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 3;

                    m_audio_hold_B = AUDIO_MENU_HOLD;
                    m_audio_flag_B = true;

                    float s = m_adc_ring[3][m_adc_index] * 0.0009765625f;

                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixFlt[0][AU2] = m_sum[2] / w2;
                    if (++m_idx2 == w2) m_idx2 = 0;

                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixFlt[0][AU3] = m_sum[3] / w3;
                    if (++m_idx3 == w3) m_idx3 = 0;
                }

                if (m_audio_hold_A > 0) --m_audio_hold_A;
                m_audio_flag_A = (m_audio_hold_A > 0);

                if (m_audio_hold_B > 0) --m_audio_hold_B;
                m_audio_flag_B = (m_audio_hold_B > 0);
}

void            CKernel::adc_AdvanceIndex           (   void    )
{
                m_adc_index = (m_adc_index + 1) & 3;
}


bool            CKernel::checkUpdate                (   )
{
                if (filecounter[FT_KLN][FLD_LOADED] == 2 ) // or is 1 correct? 
                    {
                    return true;
                    }
                else
                    {
                    return false;    
                    }
}

bool            CKernel::Update                     (   )
{
                // assumes:
                // - m_bufferKnl[1] + loaded_bytes_kernel[1] already contain the "new" kernel loaded from usb
                // - m_bufferKnl[0] + loaded_bytes_kernel[0] already contain the fallback kernel loaded from sd ( the running kernel )
                // - filesystem is already mounted by caller

                if (saveFromBuffer(PARTITION_NAME_SD, m_bufferKnl[1], loaded_bytes_kernel[1]))
                    {
                    return true;
                    }

                saveFromBuffer(FILENAME_KERNEL, m_bufferKnl[0], loaded_bytes_kernel[0]); // fail - try to restore the original kernel

                return false;
}

void            CKernel::set_pot_routing            (   bool            adc_pot_routing)
{
                m_ChipSelectPin.Write(adc_pot_routing);
}

char*           CKernel::make83FileName             (   const char*     ext )
{
                static const char map[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

                m_83FileName[0] = map[g_inOutMatrixInt[0][RND] & 31];
                m_83FileName[1] = map[g_inOutMatrixInt[1][RND] & 31];
                m_83FileName[2] = map[g_inOutMatrixInt[2][RND] & 31];
                m_83FileName[3] = map[g_inOutMatrixInt[3][RND] & 31];
                m_83FileName[4] = map[g_inOutMatrixInt[4][RND] & 31];
                m_83FileName[5] = map[g_inOutMatrixInt[5][RND] & 31];
                m_83FileName[6] = map[g_inOutMatrixInt[6][RND] & 31];
                m_83FileName[7] = map[g_inOutMatrixInt[7][RND] & 31];

                m_83FileName[8]  = '.';
                m_83FileName[9]  = ext[0];
                m_83FileName[10] = ext[1];
                m_83FileName[11] = ext[2];
                m_83FileName[12] = '\0';

                return m_83FileName;
}

void            CKernel::prepParameters             (   )               // f_buffer guess here we need much more to do!
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

void            CKernel::chooseIndex                (   int             p_channel, 
                                                        int&            p_activeIndex, 
                                                        int             p_maxCount, 
                                                        bool*           flags)    // non-condensed valid arrays, max number of files ( macros for example!)
{
                static int p_activeIndex = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][RAW] * p_maxCount >> 10;

                if (flags[f_calculated])
                    {
                    p_activeIndex = f_calculated;
                    }
}

void            CKernel::chooseIndexD               (   int             p_channel, 
                                                        int&            p_activeIndex, 
                                                        int             p_maxCount) // condensed valid arrays, max number of files ( macros for example!)
{
                int f_calculated = (g_inOutMatrixInt[p_channel][RAW] * p_maxCount) >> 10;

                p_activeIndex = f_calculated;
}

void            CKernel::storeModes                 (   )
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

void            CKernel::buttonPing                 (   int             p_btn_id, 
                                                        int             p_pin )
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

void            CKernel::button_consumer            (   int                 p_btn_id ) // this is where the magic happens: we need to set the states of menu layer, menu, we need to use one button for bpm input and so on 
{
                if (g_buttons_states[p_btn_id][BTN_SINGLE]) counter += 1;
                if (g_buttons_states[p_btn_id][BTN_DOUBLE]) counter -= 1;

                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] == 1)
                    counter += 5;

                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =10)
                    longhold += 1;
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =20)
                    longhold += 2;
}

void            CKernel::randomVec8                 (   uint32_t            p_seed )
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;

                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[0][RND] = p_seed * f_scale;
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

void            CKernel::calculate1BPM              (   int             p_source, 
                                                        unsigned long   p_triggerTimeClock)  // p_triggerTimeClock ****
{
                unsigned long f_intervalAverage = 0;

                if (p_triggerTimeClock != g_lfoBpmMatrix[p_source][LTIME])
                    {
                    g_lfoBpmMatrix[g_lfoBpmMatrix[p_source][TIDX]][TB] = p_triggerTimeClock;
        
                    g_lfoBpmMatrix[0][DB]                 =   g_lfoBpmMatrix[1][TB] - g_lfoBpmMatrix[0][TB];   
                    g_lfoBpmMatrix[1][DB]                 =   g_lfoBpmMatrix[2][TB] - g_lfoBpmMatrix[1][TB];
                    g_lfoBpmMatrix[2][DB]                 =   g_lfoBpmMatrix[3][TB] - g_lfoBpmMatrix[2][TB];

                    if(     g_lfoBpmMatrix[1][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  g_lfoBpmMatrix[2][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  g_lfoBpmMatrix[0][DB]  <   g_lfoBpmMatrix[2][DB] * 1.25f )
                        {
                        f_intervalAverage                 = ( g_lfoBpmMatrix[0][DB] + g_lfoBpmMatrix[1][DB] + g_lfoBpmMatrix[2][DB]) / 3;
            
                        g_lfoBpmMatrix[p_source][BPM]     =   60000000 / f_intervalAverage;
            
                        g_lfoBpmMatrix[p_source][INTV]    =   f_intervalAverage;
                        g_lfoBpmMatrix[p_source][LBC]     =   m_Timer.GetClockTicks();
                        }
                    g_lfoBpmMatrix[p_source][LTIME]       =   p_triggerTimeClock;

                    g_lfoBpmMatrix[p_source][TIDX]        = ( g_lfoBpmMatrix[p_source][TIDX] + 1) % 4;    
                    }
}

void            CKernel::predict1Beat               (   int             p_source, 
                                                        int             p_lfoMultIn )
{
                unsigned long currentTime               =   m_Timer.GetClockTicks();

                if (currentTime >= g_lfoBpmMatrix[p_source][NBT])
                    {
                    g_lfoBpmMatrix[p_source][NBT]      +=  g_lfoBpmMatrix[p_source][INTV];
                    }
                if (currentTime >= g_lfoBpmMatrix[p_source][NCB]) 
                    {
                    g_lfoBpmMatrix[p_source][LCB]       =   g_lfoBpmMatrix[p_source][NCB];
                    g_lfoBpmMatrix[p_source][NCB]       =   g_lfoBpmMatrix[p_source][NCB] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]); // why again g_lfoMultiplierTMP? isnt it stored already, do we need to back it up?
                    g_lfoBpmMatrix[source0][LMT]        =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]];
                    }
                if ((g_lfoBpmMatrix[p_source][LBCT]    !=  g_lfoBpmMatrix[p_source][LBC]))
                    {
                    g_lfoBpmMatrix[p_source][NBT]       =   g_lfoBpmMatrix[p_source][LBC];
                    g_lfoBpmMatrix[p_source][LBCT]      =   g_lfoBpmMatrix[p_source][LBC];
                    }
                if (g_lfoBpmMatrix[p_source][LMT]      !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]])
                    {
                    g_lfoBpmMatrix[p_source][LCB]       =   g_lfoBpmMatrix[g_activeBpmChannel][LBC];
                    g_lfoBpmMatrix[p_source][NCB]       =   g_lfoBpmMatrix[g_activeBpmChannel][LBC] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]);
                    g_lfoBpmMatrix[p_source][LMT]       =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]];
                    }
}

void            CKernel::sample1WaveTable           (   int             p_source, 
                                                        int             p_lfoIn, 
                                                        int             p_lfoOut )
{
                unsigned long currentTime               =   m_Timer.GetClockTicks();    // ? ****

                g_lfoBpmMatrix[p_source][ELP]           =   currentTime - g_lfoBpmMatrix[p_source][LCB];
                g_lfoBpmMatrix[p_source][CYL]           =   g_lfoBpmMatrix[p_source][NCB] - g_lfoBpmMatrix[p_source][LCB];
                int f_indexA                            =  (g_lfoBpmMatrix[p_source][ELP] * 255) / g_lfoBpmMatrix[p_source][CYL];
                g_lfoBpmMatrix[p_source][SMP]           =   f_indexA > 255 ? 255 : f_indexA;                                                                  // means i need a wraparound - on the other hand: i should have a clear calculation here that will never create a index >255! ****
                g_inOutMatrixFlt[0][p_lfoOut]           =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMP]] / 1023.0f;
                g_inOutMatrixInt[0][p_lfoOut]           =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMP]];
}
