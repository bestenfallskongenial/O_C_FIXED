#include "kernel.h"

    #define MY_BUFFER   m_logBuffer                 // means the log goes into the pre-init buffer 
    #define MY_INDEX    m_logBufferIndex

void            CKernel::resetMenuPickUpFlags()
{
                if (g_menuLayer != g_lastLayer)
                    {
                    memset(g_menuPickUpFlag, 0, sizeof(g_menuPickUpFlag));

                    g_selectedProgramFlag = false;

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
                
                if (!g_buttons_states[buttonA][BTN_HOLD_TICK] && !g_buttons_states[buttonB][BTN_HOLD_TICK])                                                 // no button hold -> layer 0
                    {
                    stepLayer       = 2;
                    g_menuLayer     = 0;
                    g_lastLayerLED  = 0;

                    if (g_buttons_states[buttonA][BTN_SINGLE])
                        {
                        calculate1BPMnew(0, TB0, DB0, g_currentTime);
                        g_buttons_states[buttonA][BTN_SINGLE] = 0;
                        }
                    if (g_buttons_states[buttonB][BTN_DOUBLE])
                        {
                        g_centralModeBuffer[g_gl_program_current][IS_STORED] = !g_centralModeBuffer[g_gl_program_current][IS_STORED];

                        g_buttons_states[buttonB][BTN_DOUBLE] = 0;
                        }
                    return;
                    }
                if (g_buttons_states[buttonA][BTN_HOLD_TICK] && !g_buttons_states[buttonB][BTN_HOLD_TICK])                                                  // B hold only -> current stepLayer
                    {
                    stepLayer       = 2;
                    g_menuLayer     = 1;
                    g_lastLayerLED  = 1;

                    return;
                    }
                if (g_buttons_states[buttonB][BTN_HOLD_TICK] && !g_buttons_states[buttonA][BTN_HOLD_TICK] && !g_buttons_states[buttonA][BTN_SINGLE])        // B hold only -> current stepLayer
                    {
                    g_menuLayer     = stepLayer;
                    g_lastLayerLED  = stepLayer;

                    return;
                    }
                if (g_buttons_states[buttonB][BTN_HOLD_TICK] && g_buttons_states[buttonA][BTN_SINGLE])                                                      // B hold + A press -> cycle layer 3..7
                    {
                    if (stepLayer < 3)
                        {
                        stepLayer   = 3;
                        }
                    else
                        {
                        stepLayer++;

                        if (stepLayer > 6)  // was 7
                            {
                            stepLayer = 3;
                            }
                        }
                    g_menuLayer     = stepLayer;
                    g_lastLayerLED  = stepLayer;

                    g_buttons_states[buttonA][BTN_SINGLE] = 0;

                    return;
                    }
}

void            CKernel::dispatchLayer()
{
                switch (g_menuLayer)
                {
                    case 0:
                        break;

                    case 1:                         // mode 0-3
                        set_mode_roof_map(0);
                        mapMenuGroup(0);
                        break;

                    case 2:                         // mode 4-7
                        set_mode_roof_map(1);
                        mapMenuGroup(1);
                        break;

                    case 3:                         // waveform / mult
                        set_mode_roof_map(2);
                        mapMenuGroup(2);
                        break;

                    case 4:                         // audio sensitivity
                        set_mode_roof_map(3);
                        mapMenuGroup(3);
                        break;

                    case 5:                         // target selector
                        set_mode_roof_map(4);
                        mapMenuGroup(4);
                        break;

                    case 6:                         // firmware / file operations
                        set_mode_roof_map(5);
                        mapMenuGroup(5);
                        break;
/*
                    case 7:
                        set_mode_roof_map(7);
                        mapMenuGroup(7);
                        break;
*/
                    default:
                        break;
                }
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

void            CKernel::getChannelModeB             ()
{
                uint8_t mode; 
                
                ModeFunc fn;

                mode = g_modeMap[0][g_centralModeBuffer[g_currentProgramBuffer][0]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(0);

                mode = g_modeMap[1][g_centralModeBuffer[g_currentProgramBuffer][1]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(1);

                mode = g_modeMap[2][g_centralModeBuffer[g_currentProgramBuffer][2]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(2);

                mode = g_modeMap[3][g_centralModeBuffer[g_currentProgramBuffer][3]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(3);

                mode = g_modeMap[4][g_centralModeBuffer[g_currentProgramBuffer][4]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(4);

                mode = g_modeMap[5][g_centralModeBuffer[g_currentProgramBuffer][5]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(5);

                mode = g_modeMap[6][g_centralModeBuffer[g_currentProgramBuffer][6]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(6);

                mode = g_modeMap[7][g_centralModeBuffer[g_currentProgramBuffer][7]];

                fn = g_modeTable[mode];

                if (fn) (this->*fn)(7);                
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
                        modeAudioAb0(p_channel);
                    break;

                    case 5:
                        modeAudioAb1(p_channel);
                    break;

                    case 6:
                        modeAudioBb0(p_channel);
                    break;

                    case 7:
                        modeAudioBb1(p_channel);
                    break;
                    }
}


void            CKernel::modeADC                    (   int p_channel) 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][VAL];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][VAL];       
}

void            CKernel::modeTRG                    (   int p_channel) // current
{
                if (  g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] && !g_inOutMatrixInt[p_channel][TRF] )
                    {
                    g_inOutMatrixFlt[p_channel][OUT]    = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT]    = g_inOutMatrixInt[p_channel][RND];

                    g_extClockTime[p_channel]           = g_currentTime;

                    g_inOutMatrixInt[p_channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }
}
/*
void            CKernel::modeBPM                    (   int p_channel)
{ 
                if ( g_currentTime >= g_lfoBpmMatrix[p_channel][NBT] )
                    {
                    g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][RND];
                    }
}
*/
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

void            CKernel::modeAudioAb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU0];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU0];
}

void            CKernel::modeAudioAb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU1];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU1];
}

void            CKernel::modeAudioBb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU2];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU2];
}

void            CKernel::modeAudioBb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU3];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][AU3];
}
/*
void            CKernel::applyTargetModes           (   )       // current!
{
//              if (g_menuLayer != 0)
//                  {
//                  g_gl_program_current = (g_inOutMatrixInt[ADC_SELECT_PRG][RAW] * (filecounter[FT_FSH][FLD_VALID] ) ) >> 10;
//                  }

                if (g_menuLayer == 0)
                    {
                    g_selectedProgram = (g_inOutMatrixInt[ADC_SELECT_PRG][RAW] * (filecounter[FT_FSH][FLD_VALID])) >> 10; // -1??

                    if (!g_selectedProgramFlag)
                        {
                        if (g_selectedProgram == g_gl_program_current)
                            {
                            g_selectedProgramFlag = true;
                            }
                        }
                    else
                        {
                        g_gl_program_current = g_selectedProgram;
                        }
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TEX])      // selected channel OUT controls active texture
                    {
                    m_activeTex =   g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX]][OUT];
                    }

//             if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID])      // selected channel OUT controls active video
//                  {
//                  m_activeVideo = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_VID]][OUT];
//                  }
//              if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID])      // selected channel OUT controls active frame
//                  {
//                  m_activeFrame = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM]][OUT];
//                  }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME])     // selected channel OUT controls shader/program time
                    {
                    GLtime =       g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME]][OUT];
                    }
                if (!g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME])    // time is a product of system time
                    {
                    GLtime =       g_currentTime / 1000000.0f;
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT])      // external BPM clock
                    {
                    calculate1BPMnew(1, TB1, DB1, g_extClockTime);
                    }
}
*/

void            CKernel::applyTargetModes           (   )       // current!
{
                if (g_menuLayer == 0)
                    {
                    g_selectedProgram = (g_inOutMatrixInt[ADC_SELECT_PRG][RAW] * (filecounter[FT_FSH][FLD_VALID] )) >> 10;

                    if (/*!g_selectedProgramFlag*/ g_centralModeBuffer[g_currentProgramBuffer][SEL_PRG] == 0)                                 
                        {
                        if (g_selectedProgram == g_gl_program_current)
                            {
                        //  g_selectedProgramFlag = true;
                            g_centralModeBuffer[g_currentProgramBuffer][SEL_PRG] = 1;
                            }
                        }
                    else
                        {
                        g_gl_program_current = g_selectedProgram;
                        }
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] < FLAG_THRESHOLD)       // selected channel OUT controls active texture
                    {
                    m_activeTex = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX]][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] < FLAG_THRESHOLD)       // selected channel OUT controls active video
                    {
                    m_activeVideo = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_VID]][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] < FLAG_THRESHOLD)       // selected channel OUT controls active frame
                    {
                    m_activeFrame = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM]][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] < FLAG_THRESHOLD)      // selected channel OUT controls shader/program time
                    {
                    GLtime = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME]][OUT];
                    }
                else                                                                            // time is a product of system time
                    {
                    GLtime = g_currentTime / 1000000.0f;
                    }
/*
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT] < FLAG_THRESHOLD)       // external BPM clock
                    {
                    calculate1BPMnew(1, TB1, DB1, g_extClockTime[SEL_EXT]);                              // automatized?
                    }
*/
}

void            CKernel::updateLED() // current!! <-having the block here is a problem we only have the layer avaieble, block is kinda internal knowledge!
{
                int     offset = 33; // just a value to have a quantitation for the color shown

                static const uint8_t layerToBlock[7] =
                    {
                    0,  // layer 0
                    0,  // layer 1
                    1,  // layer 2
                    2,  // layer 3
                    3,  // layer 4
                    4,  // layer 5
                    5,  // layer 6
                //  7   // layer 7
                    };

                uint8_t base;
                uint8_t idx;

                uint16_t levelA;
                uint16_t levelB;
                uint16_t levelC;
                uint16_t levelD;

                uint8_t block;

                if (g_menuLayer == 0)
                    {
                    if (g_lastLayer == 1)
                        {
                        block = 0;
                        }
                    if (g_lastLayer == 2)
                        {
                        block = 1;
                        }

                    base = block << 2;

                    levelA = g_inOutMatrixInt[base + 0][OUT];
                    levelB = g_inOutMatrixInt[base + 1][OUT];
                    levelC = g_inOutMatrixInt[base + 2][OUT];
                    levelD = g_inOutMatrixInt[base + 3][OUT];
                    }
                else
                    {
                    block = layerToBlock[g_menuLayer];

                    base = block << 2;

                    idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 0] * offset) % 255;
                    levelA = g_waveTable[WAVE_SINE][idx];

                    idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 1] * offset) % 255;
                    levelB = g_waveTable[WAVE_SINE][idx];

                    idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 2] * offset) % 255;
                    levelC = g_waveTable[WAVE_SINE][idx];

                    idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 3] * offset) % 255;
                    levelD = g_waveTable[WAVE_SINE][idx];
                    }

                WS2812_SetLED(LED_A,    (g_blockColor[block][0] * levelA) >> 10, 
                                        (g_blockColor[block][1] * levelA) >> 10, 
                                        (g_blockColor[block][2] * levelA) >> 10);
                WS2812_SetLED(LED_B,    (g_blockColor[block][0] * levelB) >> 10, 
                                        (g_blockColor[block][1] * levelB) >> 10, 
                                        (g_blockColor[block][2] * levelB) >> 10);
                WS2812_SetLED(LED_C,    (g_blockColor[block][0] * levelC) >> 10, 
                                        (g_blockColor[block][1] * levelC) >> 10, 
                                        (g_blockColor[block][2] * levelC) >> 10);
                WS2812_SetLED(LED_D,    (g_blockColor[block][0] * levelD) >> 10, 
                                        (g_blockColor[block][1] * levelD) >> 10, 
                                        (g_blockColor[block][2] * levelD) >> 10);

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
                                                m_logBuffer,            // stores the pre-init buffer
                                                m_logBufferIndex );
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