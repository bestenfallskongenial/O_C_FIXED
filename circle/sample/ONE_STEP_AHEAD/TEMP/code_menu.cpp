
#include "kernel.h"

    #define MY_BUFFER   m_bufferLog                 // not used here !
    #define MY_INDEX    m_bufferLogIndex

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

void            CKernel::chooseIndex                (   int             p_channel, 
                                                        int&            p_activeIndex, 
                                                        int             p_maxCount, 
                                                        bool*           flags)    // non-condensed valid arrays, max number of files ( macros for example!)
{
//              static int p_activeIndex = 0;

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
                
                if (g_gl_program_current != g_gl_program_last)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_gl_program_current][IS_STORED] ? g_gl_program_current : DEFAULT_SLOT;
                    g_gl_program_last = g_gl_program_current;
                    }               
                if (g_centralModeBuffer[g_gl_program_current][IS_STORED] == true /* && g_currentProgramBuffer != g_gl_program_current */)
                    {  
                    memcpy(&g_centralModeBuffer[g_gl_program_current][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_gl_program_current])); // replaces 16 * sizeof(int)
                    g_currentProgramBuffer = g_gl_program_current;
                    }
                else if (g_centralModeBuffer[g_gl_program_current][IS_STORED] == false /* && g_currentProgramBuffer != DEFAULT_SLOT */)
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}

void            CKernel::button_consumer            (   int                 p_btn_id ) // this is where the magic happens: we need to set the states of menu layer, menu, we need to use one button for bpm input and so on 
{
/*
                if (g_buttons_states[p_btn_id][BTN_SINGLE]) counter += 1;
                if (g_buttons_states[p_btn_id][BTN_DOUBLE]) counter -= 1;

                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] == 1) counter += 5;

                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =10) longhold += 1;
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =20) longhold += 2;
*/
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

                if (FLAG_AUDIO_A)
                    {
                    length0 += g_modeLengthAdd[MODELEN_AUDIO_A];
                    length2 += g_modeLengthAdd[MODELEN_AUDIO_A];
                    }

                if (FLAG_AUDIO_B)
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
void            CKernel::getChannelModeA(int p_channel)
{
                switch (g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel] + 1])
                    {
                    case 0:
                        modeADC (p_channel);
                    break;

                    case 1:
                        modeTRG (p_channel);
                    break;

                    case 2:
                        modeBPM (p_channel);
                    break;

                    case 3:
                        modeLF1 (p_channel);
                    break;
                    case 4:
                        modeLF2 (p_channel);
                    break;
                    case 5:
                    /*  modeTex (p_channel); */
                    break;
                    case 6:
                    /*  modeVid (p_channel); */
                    break;
                    case 7:
                    /*  modeFrm (p_channel); */
                    break;                    
                    case 8:
                        modeAudioAb0 (p_channel);
                    break;

                    case 9:
                        modeAudioAb1 (p_channel);
                    break;

                    case 10:
                        modeAudioBb0 (p_channel);
                    break;

                    case 11:
                        modeAudioBb1 (p_channel);
                    break;                    
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
void            CKernel::modeSelectTex              (   int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = (g_inOutMatrixInt[p_channel][RAW] * filecounter[FT_TEX][FLD_VALID]) >> 10;

                m_activeTEX = g_inOutMatrixInt[p_channel][OUT];
}

void            CKernel::modeSelectVideo            (   int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = (g_inOutMatrixInt[p_channel][RAW] * filecounter[FT_VID][FLD_VALID]) >> 10;

                m_activeVideo = g_inOutMatrixInt[p_channel][OUT];
}

void            CKernel::modeSelectFrame            (   int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = (g_inOutMatrixInt[p_channel][RAW] * m_vid.frame_count[m_activeVideo]) >> 10;

                m_activeFrame = g_inOutMatrixInt[p_channel][OUT];
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

                    s->tile_index[0] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH0];
                    s->tile_index[1] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH1];
                    s->tile_index[2] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH2];
                    s->tile_index[3] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH3];
                    s->tile_index[4] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH4];
                    s->tile_index[5] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH5];
                    s->tile_index[6] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH6];
                    s->tile_index[7] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][MODE_CH7];

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