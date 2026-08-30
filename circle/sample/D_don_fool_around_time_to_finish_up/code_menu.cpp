#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex

void            CKernel::resetMenuPickUpFlags()
{
                if (g_menuLayer != g_lastLayer)
                    {
                    memset(g_menuPickUpFlag, 0, sizeof(g_menuPickUpFlag));

                //  g_activeProgramFlag = false;

                    g_centralModeBuffer[g_currentProgramBuffer][SEL_PRG] = 0;

                    g_lastLayer = g_menuLayer;
                    }
}
void            CKernel::storeModes()
{
                if (g_gl_program_current != g_gl_program_last)
                    {
                    g_currentProgramBuffer = g_centralModeBuffer[g_gl_program_current][IS_STORED] ? g_gl_program_current : DEFAULT_SLOT;
                    g_gl_program_last = g_gl_program_current;
                    }

                if (g_centralModeBuffer[g_gl_program_current][IS_STORED] && g_currentProgramBuffer != g_gl_program_current)
                    {
                    int stored = g_centralModeBuffer[g_gl_program_current][IS_STORED];

                    memcpy(&g_centralModeBuffer[g_gl_program_current][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_gl_program_current]));

                    g_centralModeBuffer[g_gl_program_current][IS_STORED] = stored;
                    g_currentProgramBuffer = g_gl_program_current;
                    }
                else if (!g_centralModeBuffer[g_gl_program_current][IS_STORED] && g_currentProgramBuffer != DEFAULT_SLOT)
                    {
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}

void            CKernel::buttonConsumer(int buttonA, int buttonB)
{
                static int stepLayer = 2;

                if (!g_buttons_states[buttonA][BTN_HOLD_TICK] && !g_buttons_states[buttonB][BTN_HOLD_TICK])
                    {
                    stepLayer      = 2;
                    g_menuLayer    = 0;
                    g_lastLayerLED = 0;

                    if (g_buttons_states[buttonA][BTN_SINGLE])
                        {
                        calculate1BPMnew(0, TB0, DB0, g_frameStart);
                        g_buttons_states[buttonA][BTN_SINGLE] = 0;
                        }
                    if (g_buttons_states[buttonB][BTN_DOUBLE])
                        {
                        g_centralModeBuffer[g_gl_program_current][IS_STORED] = !g_centralModeBuffer[g_gl_program_current][IS_STORED];

                        g_buttons_states[buttonB][BTN_DOUBLE] = 0;
                        }
                    return;
                    }

                if (g_buttons_states[buttonA][BTN_HOLD_TICK] && !g_buttons_states[buttonB][BTN_HOLD_TICK])
                    {
                    stepLayer      = 2;
                    g_menuLayer    = 1;
                    g_lastLayerLED = 1;

                    return;
                    }

                if (g_buttons_states[buttonB][BTN_HOLD_TICK] && !g_buttons_states[buttonA][BTN_HOLD_TICK] && !g_buttons_states[buttonA][BTN_SINGLE])
                    {
                    g_menuLayer    = stepLayer;
                    g_lastLayerLED = stepLayer;

                    return;
                    }

                if (g_buttons_states[buttonB][BTN_HOLD_TICK] && g_buttons_states[buttonA][BTN_SINGLE])
                    {
                    bool layerAvailable = false;

                    do  {
                        ++stepLayer;

                        if (stepLayer > ACCESSIBLE_LAYER) stepLayer = 3;

                        layerAvailable =    (   layerModeMap[stepLayer] &   (   modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][0]] | 
                                                                                modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][1]] |
                                                                                modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][2]] | 
                                                                                modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][3]] |
                                                                                modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][4]] | 
                                                                                modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][5]] |
                                                                                modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][6]] | 
                                                                                modeMaskByValue[g_centralModeBuffer[g_currentProgramBuffer][7]]   ) ) != 0;
                        }
                    while (!layerAvailable);

                    g_menuLayer    = stepLayer;
                    g_lastLayerLED = stepLayer;

                    g_buttons_states[buttonA][BTN_SINGLE] = 0;

                    return;
                    }
}

void            CKernel::dispatchLayer()
{
    if (g_menuLayer == 0)
    {
        return;
    }

    const uint8_t block = g_menuLayer - 1;

    set_mode_roof_map(block);
    mapMenuGroup(block);
}

void            CKernel::set_mode_roof_map          (uint8_t block)
{
                const uint8_t f_first_flag = FLAG_AUDIO_A;
                const uint8_t base         = block << 2;

                for (uint8_t slot = 0; slot < 4; ++slot)
                    {
                    const uint8_t row = base + slot;

                    if (g_mapType[block][slot] == MAP_VALUE)
                        {
                        g_modeRoof[row] = g_valueRoof[block][slot];
                        continue;
                        }

                    uint8_t dst = 0;

                    const uint8_t baseLen = g_valueRoof[block][slot];       // base group length comes from g_valueRoof for MAP_MODE blocks (e.g. 5,5,5,5)

                    for (uint8_t i = 0; i < baseLen; ++i)
                        {
                        g_modeMap[row][dst++] = g_groupModes[GROUP_BASE][i];
                        }
                    for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group) // optional groups (A/B)
                        {
                        const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

                        if (!g_centralModeBuffer[g_currentProgramBuffer][flag_pos]) 
                            {
                            continue;
                            }
                        for (uint8_t i = 0; i < g_groupLen[group]; ++i)
                            {
                            g_modeMap[row][dst++] = g_groupModes[group][i];
                            }
                        }

                    g_modeRoof[row] = dst;
                    }
}

void            CKernel::mapMenuGroup               (uint8_t block)
{
                const uint8_t base = block << 2;
                unsigned v;

                v = (g_inOutMatrixInt[4][RAW] * g_modeRoof[base + 0]) >> 10;

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
                    {
                    g_menuPickUpFlag[base + 0] = true;
                    }
                else if (g_menuPickUpFlag[base + 0])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
                    }

                v = (g_inOutMatrixInt[5][RAW] * g_modeRoof[base + 1]) >> 10;

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
                    {
                    g_menuPickUpFlag[base + 1] = true;
                    }
                else if (g_menuPickUpFlag[base + 1])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
                    }

                v = (g_inOutMatrixInt[6][RAW] * g_modeRoof[base + 2]) >> 10;

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
                    {
                    g_menuPickUpFlag[base + 2] = true;
                    }
                else if (g_menuPickUpFlag[base + 2])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
                    }

                v = (g_inOutMatrixInt[7][RAW] * g_modeRoof[base + 3]) >> 10;

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
                    {
                    g_menuPickUpFlag[base + 3] = true;
                    }
                else if (g_menuPickUpFlag[base + 3])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
                    }
}

void            CKernel::getChannelModeA(int p_channel)
{
                switch (g_centralModeBuffer[g_currentProgramBuffer][p_channel])
                    {
                    case 0:
                        modeADC(p_channel);
                    break;

                    case 1:
                        modeTRG(p_channel);
                    break;
/*
                    case 2:
                        modeBPM(p_channel);
                    break;
*/
                    case 2:
                        modeLF1(p_channel);
                    break;

                    case 3:
                        modeLF2(p_channel);
                    break;

                    case 4:
                        modeAudioAbL(p_channel);
                    break;

                    case 5:
                        modeAudioAbH(p_channel);
                    break;

                    case 6:
                        modeAudioBbL(p_channel);
                    break;

                    case 7:
                        modeAudioBbH(p_channel);
                    break;
#ifdef USE_MIDI
                    case 8:
                        modeMidiNote(p_channel);
                    break;

                    case 9:
                        modeMidiCC0(p_channel);
                    break;

                    case 10:
                        modeMidiCC1(p_channel);
                    break;
#endif                                        
                    }
}

void            CKernel::modeADC                    (   int p_channel) 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][VAL];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][VAL];       
}

void            CKernel::modeTRG(int p_channel)
{
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT] == (unsigned)p_channel) g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT] = true;

                if (g_inOutMatrixInt[p_channel][VAL] >=  g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L] && !g_inOutMatrixInt[p_channel][TRF])
                    {
                    g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][RND];

                    g_extClockTime[p_channel] = g_frameStart;

                    g_inOutMatrixInt[p_channel][TRF] = true;
                    }
                else if (g_inOutMatrixInt[p_channel][VAL] <= g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L] + 1       // new- hight is never <= low!
                                                           + g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_H])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }
}

void            CKernel::modeLF1                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF1];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF1];
}

void            CKernel::modeLF2                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF2];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF2]; 
}

void            CKernel::modeAudioAbL               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU0];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU0];
}

void            CKernel::modeAudioAbH               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU1];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU1];
}

void            CKernel::modeAudioBbL               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU2];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU2];
}

void            CKernel::modeAudioBbH               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU3];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU3];
}
#ifdef USE_MIDI
void            CKernel::modeMidiNote(int p_channel)
{
                unsigned low;
                unsigned high;

                switch (g_centralModeBuffer[g_currentProgramBuffer][MIDI_RANGE])
                    {
                    case 0: low = 36; high = 47; break;
                    case 1: low = 36; high = 59; break;
                    case 2: low = 36; high = 71; break;
                    default: low = 12; high = 72; break;
                    }

                unsigned note = g_midiNote;

                if (note < low) note = low;
                if (note > high) note = high;

                g_inOutMatrixInt[p_channel][OUT] = ((note - low) * 1023) / (high - low);
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixInt[p_channel][OUT] / 1024.0f;
}

void            CKernel::modeMidiCC0(int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = g_midiCC0Int;
                g_inOutMatrixFlt[p_channel][OUT] = g_midiCC0Flt;
}

void            CKernel::modeMidiCC1(int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = g_midiCC1Int;
                g_inOutMatrixFlt[p_channel][OUT] = g_midiCC1Flt;
}
#endif

void            CKernel::applyTargetModes           (   )
{
                if (g_menuLayer == 0)
                    {
                    g_activeProgram = (g_inOutMatrixInt[ADC_SELECT_PRG][OUT] * (filecounter[FT_FSH][FLD_VALID])) >> 10;

                    if (g_centralModeBuffer[g_currentProgramBuffer][SEL_PRG] == 0)
                        {
                        if (g_activeProgram == g_gl_program_current)
                            {
                            g_centralModeBuffer[g_currentProgramBuffer][SEL_PRG] = 1;
                            }
                        }
                    else
                        {
                        g_gl_program_current = g_activeProgram;
                        }
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] < FLAG_THRESHOLD)
                    {
                    m_activeTex = (g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX]][OUT] * (filecounter[FT_TEX][FLD_VALID])) >> 10;
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] < FLAG_THRESHOLD)
                    {
                    m_activeVideo = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_VID]][OUT];
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM] < FLAG_THRESHOLD)
                    {
                    m_activeFrame = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM]][OUT];
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] < FLAG_THRESHOLD)
                    {
                    GLtime = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME]][OUT] / 36.0f;
                    }
                else
                    {
                    GLtime = g_frameStart / 1000000.0f;
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT] && g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT] < FLAG_THRESHOLD)
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][LAST_EXT] = g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT];

                    calculate1BPMnew( 1, TB1, DB1, g_extClockTime[g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT]]);
                    }
/*
                else
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT] = g_centralModeBuffer[g_currentProgramBuffer][LAST_EXT];
                    }
*/
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT] = false;
}

void            CKernel::updateLED()
{
                int     offset = 33; // just a value to have a quantitation for the color shown

                static const uint8_t layerToBlock[ACCESSIBLE_LAYER + 1] =
                    {
                    0,  // layer 0 - special, block comes from showBank
                    0,  // layer 1 - mode channel 0-3
                    1,  // layer 2 - mode channel 4-7
                    2,  // layer 3 - lfo
                    3,  // layer 4 - threshold
                    4,  // layer 5 - audio
                    5,  // layer 6 - selection
                    6,  // layer 7 - system
#ifdef USE_MIDI
                    7,  // layer 8 - midi
#endif
                    };

                static uint8_t showBank = 0; // 0 = channel 0-3, 1 = channel 4-7

                uint8_t base;
                uint8_t idx;
                uint8_t block;

                uint16_t levelA;
                uint16_t levelB;
                uint16_t levelC;
                uint16_t levelD;

                const int *colorA;
                const int *colorB;
                const int *colorC;
                const int *colorD;

/*
 * Remember the last channel bank.
 *
 * Layer 1 selects channel 0-3.
 * Layer 2 selects channel 4-7.
 *
 * Layers 3 and higher do not alter the remembered bank.
 */
                if (g_menuLayer == 1)
                    {
                    showBank = 0;
                    }
                else if (g_menuLayer == 2)
                    {
                    showBank = 1;
                    }

/*
 * Layer 0:
 *
 * Show the live output of the last selected channel bank.
 * Every channel uses the color of its stored mode.
 */
                if (g_menuLayer == 0)
                    {
                    block = showBank;
                    base  = block << 2;

                    levelA = g_inOutMatrixInt[base + 0][OUT];
                    levelB = g_inOutMatrixInt[base + 1][OUT];
                    levelC = g_inOutMatrixInt[base + 2][OUT];
                    levelD = g_inOutMatrixInt[base + 3][OUT];

                    colorA = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 0]];
                    colorB = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 1]];
                    colorC = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 2]];
                    colorD = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 3]];
                    }

/*
 * Layer 1 and 2:
 *
 * Show the block base color before pickup.
 * Show the stored mode color after pickup.
 *
 * The mode color itself represents the discrete mode value, so these
 * layers do not use the sine table for their brightness.
 */
                else if (g_menuLayer == 1 || g_menuLayer == 2)
                    {
                    block = layerToBlock[g_menuLayer];
                    base  = block << 2;

                    levelA = 1023;
                    levelB = 1023;
                    levelC = 1023;
                    levelD = 1023;

                    if (!g_menuPickUpFlag[base + 0])
                        {
                        colorA = g_blockColor[block];
                        }
                    else
                        {
                        colorA = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 0]];
                        }

                    if (!g_menuPickUpFlag[base + 1])
                        {
                        colorB = g_blockColor[block];
                        }
                    else
                        {
                        colorB = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 1]];
                        }

                    if (!g_menuPickUpFlag[base + 2])
                        {
                        colorC = g_blockColor[block];
                        }
                    else
                        {
                        colorC = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 2]];
                        }

                    if (!g_menuPickUpFlag[base + 3])
                        {
                        colorD = g_blockColor[block];
                        }
                    else
                        {
                        colorD = g_modeColor[g_centralModeBuffer[g_currentProgramBuffer][base + 3]];
                        }
                    }

/*
 * Every layer above 2:
 *
 * Show the block base color at full level before pickup.
 * After pickup, retain the block color and represent the stored value
 * through the existing sine-table quantization.
 */
                else
                    {
                    block = layerToBlock[g_menuLayer];
                    base  = block << 2;

                    colorA = g_blockColor[block];
                    colorB = g_blockColor[block];
                    colorC = g_blockColor[block];
                    colorD = g_blockColor[block];

                    if (!g_menuPickUpFlag[base + 0])
                        {
                        levelA = 1023;
                        }
                    else
                        {
                        idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 0] * offset) % 255;
                        levelA = g_waveTable[WAVE_SINE][idx];
                        }

                    if (!g_menuPickUpFlag[base + 1])
                        {
                        levelB = 1023;
                        }
                    else
                        {
                        idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 1] * offset) % 255;
                        levelB = g_waveTable[WAVE_SINE][idx];
                        }

                    if (!g_menuPickUpFlag[base + 2])
                        {
                        levelC = 1023;
                        }
                    else
                        {
                        idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 2] * offset) % 255;
                        levelC = g_waveTable[WAVE_SINE][idx];
                        }

                    if (!g_menuPickUpFlag[base + 3])
                        {
                        levelD = 1023;
                        }
                    else
                        {
                        idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 3] * offset) % 255;
                        levelD = g_waveTable[WAVE_SINE][idx];
                        }
                    }

                WS2812_SetLED(LED_A,    (colorA[0] * levelA) >> 10, (colorA[1] * levelA) >> 10, (colorA[2] * levelA) >> 10);
                WS2812_SetLED(LED_B,    (colorB[0] * levelB) >> 10, (colorB[1] * levelB) >> 10, (colorB[2] * levelB) >> 10);
                WS2812_SetLED(LED_C,    (colorC[0] * levelC) >> 10, (colorC[1] * levelC) >> 10, (colorC[2] * levelC) >> 10);
                WS2812_SetLED(LED_D,    (colorD[0] * levelD) >> 10, (colorD[1] * levelD) >> 10, (colorD[2] * levelD) >> 10);

                WS2812_Update();
}

void            CKernel::checkSystemFlags()
{
                if ( g_centralModeBuffer[g_currentProgramBuffer][SET_STORE] )
                    {
                    /* execute */
                    g_centralModeBuffer[g_currentProgramBuffer][SET_STORE] = 0;
                    }
                if ( g_centralModeBuffer[g_currentProgramBuffer][SET_LOAD] )
                    {
                    /* execute */
                    g_centralModeBuffer[g_currentProgramBuffer][SET_LOAD] = 0;
                    }
                if ( g_centralModeBuffer[g_currentProgramBuffer][LOG_STORE] )
                    {
                    /* execute */
                    saveFromBuffer          (   PARTITION_NAME_SD,
                                            /*  gen83FileName("TXT"), */
                                                "bootlog.txt",
                                                m_logKernel,            // stores the pre-init buffer
                                                m_logKernelIndex );
                    msDelay(100);
                    saveFromBuffer          (   PARTITION_NAME_SD,
                                                "GLSL.txt",
                                                m_bufferLog[1],
                                                m_bufferLogIndex[1] );
                    msDelay(100);
                    saveFromBuffer          (   PARTITION_NAME_SD,
                                                "parser.txt",
                                                m_bufferLog[0],
                                                m_bufferLogIndex[0] );
                    msDelay(100);
                    saveFromBuffer          (   PARTITION_NAME_SD,
                                                "vc04.txt",
                                                m_bufferLog[2],
                                                m_bufferLogIndex[2] );

                    g_centralModeBuffer[g_currentProgramBuffer][LOG_STORE] = 0;
                    }
                if ( g_centralModeBuffer[g_currentProgramBuffer][KLN_LOAD] )
                    {
                    /* execute */
                    UpdateKernel();

                    g_centralModeBuffer[g_currentProgramBuffer][KLN_LOAD] = 0;
                    }
}