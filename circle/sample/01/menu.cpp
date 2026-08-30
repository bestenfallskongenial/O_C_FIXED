//----------------------------------------------------------------------------------------------------------------------------------------------------
// globals and variablesVAL used here:
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::resetPickUpFlags    ()
{

                if (g_menu_mode_new != g_menu_mode_old) // or simply memset(menu_pickup_flag, 0, 16 * sizeof(bool)); ?
                    {
                    for(int i = 0; i < modetablecount; i++) 
                        {
                        g_menuPickUpFlag[i] = false;
                        }
                    g_menu_mode_old = g_menu_mode_new;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeMenuAssignGroup(uint8_t menu_id, uint8_t base)
{
                if (g_menu_mode_new != menu_id)
                    {
                    return;
                    }
                int v;

                v = (g_inOutMatrixInt[4][RAW] * g_modeMap[base + 0][0]) >> 10;        // slot 0

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
                    {
                    g_menuPickUpFlag[base + 0] = true;
                    }
                else if (g_menuPickUpFlag[base + 0])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
                    }
                v = (g_inOutMatrixInt[5][RAW] * g_modeMap[base + 1][0]) >> 10;        // slot 1

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
                    {
                    g_menuPickUpFlag[base + 1] = true;
                    }
                else if (g_menuPickUpFlag[base + 1])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
                    }
                v = (g_inOutMatrixInt[6][RAW] * g_modeMap[base + 2][0]) >> 10;        // slot 2

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
                    {
                    g_menuPickUpFlag[base + 2] = true;
                    }
                else if (g_menuPickUpFlag[base + 2])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
                    }
                v = (g_inOutMatrixInt[7][RAW] * g_modeMap[base + 3][0]) >> 10;        // slot 3

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
                    {
                    g_menuPickUpFlag[base + 3] = true;
                    }
                else if (g_menuPickUpFlag[base + 3])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::getChannelModeA(int p_channel) // this is also a wrapper right ??
{
                switch (g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel] + 1]) // <- is this correct! g_modeMap gives me the max of modes ( needed for mapping ), also shall it determine if i can use a channelVAL this function...
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
                    //  modeTex (p_channel);
                    break;
                    case 6:
                    //  modeVid (p_channel);
                    break;
                    case 7:
                    //  modeFrm (p_channel);
                    break;                    
                    case 8:
                        modeAudioAb0 (p_channel);     // this modes will be "available" if audio is detected.
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::getChannelModeB(int p_channel)
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeADC (int p_channel) 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][VAL] // adc_float_value[p_channel];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][VAL] // adc_int_value[p_channel];            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeTRG (int p_channel)
{
                if (  g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                    ! g_inOutMatrixInt[p_channel][TRF]) // treshold flag - but i need 4 each menu layer or is this different than g_menuPickUpFlag???
                    {
                    g_inOutMatrixFlt[p_channel][OUT]    = g_inOutMatrixFlt[p_channel][RND] // /* g_randomFloatValue */[p_channel];
                    g_inOutMatrixInt[p_channel][OUT]    = g_inOutMatrixInt[p_channel][RND] // g_randomIntegerValue[p_channel];

                    g_inOutMatrixInt[p_channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeBPM (int p_channel) 
{ 
                if (currentTime >= g_nextBeatTime[g_activeBpmChannel])
                    {
                    g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][RND] // /* g_randomFloatValue */[p_channel];
                    g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][RND] // g_randomIntegerValue[p_channel];
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeLF1 (int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF1] // g_lfoFltOut[0] <- lfo comes always from [0] since it is global
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF1] // g_lfoIntOut[0];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeLF2 (int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[1][LF2] // g_lfoFltOut[1];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[1][LF2] // g_lfoIntOut[1];    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioAb0 (int p_channel)     // 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU0] // g_lfoFltOut[1];  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioAb1 (int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU1] // g_lfoFltOut[1];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioBb0 (int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU2] // g_lfoFltOut[1];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioBb1 (int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU3] // g_lfoFltOut[1];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
