
#include "kernel.h"

//  #undef  __DEBUG_LOG__
    #define __DEBUG_LOG__

    #define MY_BUFFER   m_bufferLog                 // not used here !
    #define MY_INDEX    m_bufferLogIndex

void            CKernel::resetPickUpFlags           (   ) 
{

                if (g_menu_mode_new != g_menu_mode_old) /* or simply memset(menu_pickup_flag, 0, 16 * sizeof(bool)); ? **** */
                    {
                    for(int i = 0; i < /*MODETABLE_COUNT*/ ( 4*MENU_LAYER_COUNT ) ; i++) 
                        {
                        g_menuPickUpFlag[i] = false;
                        }
                    g_menu_mode_old = g_menu_mode_new;  // bot need to be declared globally as part of the menu / layer state-machine!!!
                    }
}

void            CKernel::set_mode_length            (   uint8_t base)
{
                uint8_t length0;
                uint8_t length1;
                uint8_t length2;
                uint8_t length3;

                length0 = g_modeLengthAdd[MODELENDEFAULT];
                length1 = g_modeLengthAdd[MODELENDEFAULT];
                length2 = g_modeLengthAdd[MODELENDEFAULT];
                length3 = g_modeLengthAdd[MODELENDEFAULT];

                if (m_audio_flag_A)
                    {
                    length0 += g_modeLengthAdd[MODELEN_AUDIO_A];
                    length2 += g_modeLengthAdd[MODELEN_AUDIO_A];
                    }

                if (m_audio_flag_B)
                    {
                    length1 += g_modeLengthAdd[MODELEN_AUDIO_B];
                    length3 += g_modeLengthAdd[MODELEN_AUDIO_B];
                    }

                g_modeMap[base + 0][0] = length0;
                g_modeMap[base + 1][0] = length1;
                g_modeMap[base + 2][0] = length2;
                g_modeMap[base + 3][0] = length3;
}

void            CKernel::mapMenuGroup               (   uint8_t menu_id, uint8_t base)
{
                if (g_menu_mode_new != menu_id)
                    {
                    return;
                    }
                unsigned v;

                v = (g_inOutMatrixInt[4][RAW] * g_modeMap[base + 0][0]) >> 10;

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
                    {
                    g_menuPickUpFlag[base + 0] = true;
                    }
                else if (g_menuPickUpFlag[base + 0])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
                    }
                v = (g_inOutMatrixInt[5][RAW] * g_modeMap[base + 1][0]) >> 10;

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
                    {
                    g_menuPickUpFlag[base + 1] = true;
                    }
                else if (g_menuPickUpFlag[base + 1])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
                    }
                v = (g_inOutMatrixInt[6][RAW] * g_modeMap[base + 2][0]) >> 10;

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
                    {
                    g_menuPickUpFlag[base + 2] = true;
                    }
                else if (g_menuPickUpFlag[base + 2])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
                    }
                v = (g_inOutMatrixInt[7][RAW] * g_modeMap[base + 3][0]) >> 10;

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
                    {
                    g_menuPickUpFlag[base + 3] = true;
                    }
                else if (g_menuPickUpFlag[base + 3])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
                    }
}

void            CKernel::getChannelModeB            (   int p_channel)
{
                int mode =
                    g_modeMap[p_channel]
                            [g_centralModeBuffer[g_currentProgramBuffer][p_channel] + 1];

                ModeFunc fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }
}

void            CKernel::modeADC                    (   int p_channel) 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][VAL];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][VAL];         
}

void            CKernel::modeTRG                    (   int p_channel)
{
                if (  g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                    ! g_inOutMatrixInt[p_channel][TRF]) /* treshold flag - but i need 4 each menu layer or is this different than g_menuPickUpFlag??? */
                    {
                    g_inOutMatrixFlt[p_channel][OUT]    = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT]    = g_inOutMatrixInt[p_channel][RND];

                    g_inOutMatrixInt[p_channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }
}

void            CKernel::modeBPM                    (   int p_channel /* , currentTime*/)   // <- currentTime should be g_currentTime -> global member set during Run()  - or a call parameter!
{ 
                if ( /* currentTime */ g_currentTime >= g_lfoBpmMatrix[p_channel][NBT] )// g_nextBeatTime[g_activeBpmChannel])      // <- g_nextBeatTime is now part of the g_lfoBpmMatrix -> enum lfo_bpm_types NBT -> nextBeatTime
                                                                            
                    {
                    g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][RND];
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

void            CKernel::modeAudioAb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU0];
}

void            CKernel::modeAudioAb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU1];
}

void            CKernel::modeAudioBb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU2];
}

void            CKernel::modeAudioBb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU3];
}

void            CKernel::updateOvlState             (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t )
{
                const float ox = s->kMenuOrigin[0];
                const float oy = s->kMenuOrigin[1];
                const float tw = s->kMenuTileSize[0];
                const float th = s->kMenuTileSize[1];

                for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
                    {
                    float sx = s->kMenuRelSize[i][0];
                    float sy = s->kMenuRelSize[i][1];

                    if (i == 14 || i == 15)
                        {
                        sx *= s->kMenuBackgroundScale[0];
                        sy *= s->kMenuBackgroundScale[1];
                        }

                    s->tile_rect_x[i] = ox + s->kMenuRelPos[i][0] * tw;
                    s->tile_rect_y[i] = oy + s->kMenuRelPos[i][1] * th;
                    s->tile_rect_w[i] = sx * tw;
                    s->tile_rect_h[i] = sy * th;

                    s->tile_rect[i * 4 + 0] = s->tile_rect_x[i];
                    s->tile_rect[i * 4 + 1] = s->tile_rect_y[i];
                    s->tile_rect[i * 4 + 2] = s->tile_rect_w[i];
                    s->tile_rect[i * 4 + 3] = s->tile_rect_h[i];
                    }

                    s->tile_index[0] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH0_MODE];
                    s->tile_index[1] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH1_MODE];
                    s->tile_index[2] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH2_MODE];
                    s->tile_index[3] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH3_MODE];
                    s->tile_index[4] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH4_MODE];
                    s->tile_index[5] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH5_MODE];
                    s->tile_index[6] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH6_MODE];
                    s->tile_index[7] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH7_MODE];

                    const unsigned long bpm0 = g_lfoBpmMatrix[0][BPM] /* g_resultBPM[0] */ % 10000UL; // is now part of g_lfoBpmMatrix -> enum lfo_bpm_types -> BPM =0 result BPM 
                    const unsigned long bpm1 = g_lfoBpmMatrix[1][BPM]/* g_resultBPM[1] */ % 10UL;

                    s->tile_index[8]  = (GLfloat) ((bpm0 / 1000UL) % 10UL);
                    s->tile_index[9]  = (GLfloat) ((bpm0 / 100UL) % 10UL);
                    s->tile_index[10] = (GLfloat) ((bpm0 / 10UL) % 10UL);
                    s->tile_index[11] = (GLfloat) (bpm0 % 10UL);
                    s->tile_index[12] = 10.0f;
                    s->tile_index[13] = (GLfloat) bpm1;
                    s->tile_index[14] = 48.0f;
                    s->tile_index[15] = 49.0f;
}
// END OF FILE