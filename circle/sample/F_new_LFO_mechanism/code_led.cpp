#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex
/*
void            CKernel::circleLedColor()        // simple display_debug / feedback function
{
                static int counter = 0;

                WS2812_SetLED(LED_A, (g_waveTable[WAVE_SINE][(1 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(2 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(3 * counter) % 255] >> 6));

                WS2812_SetLED(LED_B, (g_waveTable[WAVE_SINE][(2 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(3 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(1 * counter) % 255] >> 6));

                WS2812_SetLED(LED_C, (g_waveTable[WAVE_SINE][(3 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(1 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(2 * counter) % 255] >> 6));

                WS2812_SetLED(LED_D, (g_waveTable[WAVE_SINE][(2 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(1 * counter) % 255] >> 6), (g_waveTable[WAVE_SINE][(3 * counter) % 255] >> 6));

                WS2812_Update();

                counter++;
}
*/
void            CKernel::circleLedColor()
{
                static int counter = 0;

                WS2812_SetLED(LED_A, (((uint16_t*)m_bufferLfo[waveSinus])[(1 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(2 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(3 * counter) % LFO_SAMPLES] >> 6));

                WS2812_SetLED(LED_B, (((uint16_t*)m_bufferLfo[waveSinus])[(2 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(3 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(1 * counter) % LFO_SAMPLES] >> 6));

                WS2812_SetLED(LED_C, (((uint16_t*)m_bufferLfo[waveSinus])[(3 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(1 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(2 * counter) % LFO_SAMPLES] >> 6));

                WS2812_SetLED(LED_D, (((uint16_t*)m_bufferLfo[waveSinus])[(2 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(1 * counter) % LFO_SAMPLES] >> 6), (((uint16_t*)m_bufferLfo[waveSinus])[(3 * counter) % LFO_SAMPLES] >> 6));

                WS2812_Update();

                counter++;
}

void            CKernel::menuLedUpdate()
{
                int     offset = 33; // just a value to have a quantitation for the color shown

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

// Remember the last channel bank. Layer 1 selects channel 0-3. Layer 2 selects channel 4-7. Layers 3 and higher do not alter the remembered bank.

                if (g_menuLayer == 1)
                    {
                    showBank = 0;
                    }
                else if (g_menuLayer == 2)
                    {
                    showBank = 1;
                    }

 * Layer 0: Show the live output of the last selected channel bank. Every channel uses the color of its stored mode.

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

// Layer 1 and 2: Show the block base color before pickup. Show the stored mode color after pickup. The mode color itself represents the discrete mode value, so these layers do not use the sine table for their brightness.

                else if (g_menuLayer == 1 || g_menuLayer == 2)
                    {
                    block = g_menuLayer - 1;
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

// Every layer above 2: Show the block base color at full level before pickup. After pickup, retain the block color and represent the stored value through the existing sine-table quantization.

                else
                    {
                    block = g_menuLayer - 1;
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
                        levelA = ((uint16_t*)m_bufferLfo[waveSinus])[idx];
                    //  levelA = g_waveTable[WAVE_SINE][idx];
                        }

                    if (!g_menuPickUpFlag[base + 1])
                        {
                        levelB = 1023;
                        }
                    else
                        {
                        idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 1] * offset) % 255;
                        levelB = ((uint16_t*)m_bufferLfo[waveSinus])[idx];                        
                    //  levelB = g_waveTable[WAVE_SINE][idx];
                        }

                    if (!g_menuPickUpFlag[base + 2])
                        {
                        levelC = 1023;
                        }
                    else
                        {
                        idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 2] * offset) % 255;
                        levelC = ((uint16_t*)m_bufferLfo[waveSinus])[idx];                        
                    //  levelC = g_waveTable[WAVE_SINE][idx];
                        }

                    if (!g_menuPickUpFlag[base + 3])
                        {
                        levelD = 1023;
                        }
                    else
                        {
                        idx = (g_centralModeBuffer[g_currentProgramBuffer][base + 3] * offset) % 255;
                        levelD = ((uint16_t*)m_bufferLfo[waveSinus])[idx];                        
                    //  levelD = g_waveTable[WAVE_SINE][idx];
                        }
                    }

                WS2812_SetLED(LED_A,    (colorA[0] * levelA) >> 10, (colorA[1] * levelA) >> 10, (colorA[2] * levelA) >> 10);
                WS2812_SetLED(LED_B,    (colorB[0] * levelB) >> 10, (colorB[1] * levelB) >> 10, (colorB[2] * levelB) >> 10);
                WS2812_SetLED(LED_C,    (colorC[0] * levelC) >> 10, (colorC[1] * levelC) >> 10, (colorC[2] * levelC) >> 10);
                WS2812_SetLED(LED_D,    (colorD[0] * levelD) >> 10, (colorD[1] * levelD) >> 10, (colorD[2] * levelD) >> 10);

                WS2812_Update();
}