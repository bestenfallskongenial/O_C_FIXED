// constants / enums used by set_mode_roof_map()
#define BLOCK_COUNT      8
/*
enum source_modes       // actually not used except SOURCE_MODE_MAX
{
    IN_MODE_ADC,        // adc direct to gl
    IN_MODE_TRG,        // a adc threshold triggers to store a random volume  to gl
    IN_MODE_BMP,        // a internal / external ( later more ) BPM wll store a random value to gl
    IN_MODE_LF1,        // an internal waketable value calculated from the bpm is send continuously to gl 
    IN_MODE_LF2,        // an internal waketable value calculated from the bpm is send continuously to gl

    MODE_BAND_L0,       // if audio0 is detected ( and audiomode flag set ? ) this sends the averaged low 0 band output to gl
    MODE_BAND_H0,       // if audio0 is detected ( and audiomode flag set ? ) this sends the averaged high 0 band output to gl
    MODE_BAND_L1,       // if audio1 is detected ( and audiomode flag set ? ) this sends the averaged low 0 band output to gl
    MODE_BAND_H1,       // if audio1 is detected ( and audiomode flag set ? ) this sends the averaged high 0 band output to gl

    SOURCE_MODE_MAX
}
enum target_modes
{
    OUT_MODE_SEL_TEX    // this is not a mode in this sense, if the dedicated flag is set this sends the after mode value g_inOutMatrixFlt[p_channel][OUT] to a system member variable
    OUT_MODE_SEL_VID    // we need to do so because we want the mode effect, not the input adc this system parameter
    OUT_MODE_SEL_FRM
    OUT_MODE_PRG_CLK    // this is the "mode" where i not send and continious timestamp to gl but a value after mode 
    OUT_MODE_EXT_CLK    // here im not certain what to do about it. i can use detected beats also via TRG mode ( because it can reliable detect clocks - if applied )

    TARGET_MODE_MAX
}
*/    
enum centralModeBuffer // organised in blocks/layers for the mapMenuGroup
{
	MODE_CH0 = 0,           // store the mode ( from g_modeTable[] ) for cannel 0 -> block 0 / layer 1
	MODE_CH1,               // store the mode ( from g_modeTable[] ) for cannel 1
	MODE_CH2,               // store the mode ( from g_modeTable[] ) for cannel 2
	MODE_CH3,               // store the mode ( from g_modeTable[] ) for cannel 3

	MODE_CH4,               // store the mode ( from g_modeTable[] ) for cannel 4 -> block 1 / layer 2
	MODE_CH5,               // store the mode ( from g_modeTable[] ) for cannel 5
	MODE_CH6,               // store the mode ( from g_modeTable[] ) for cannel 6
	MODE_CH7,               // store the mode ( from g_modeTable[] ) for cannel 7

	LF1_WAVE,               // stores waveform index for lfo1 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )  -> block 2 / layer 3
	LF2_WAVE,               // stores waveform index for lfo2 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )
	LF1_MULT,               // stores the multiplier index for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )
	LF2_MULT,               // stores the multiplier index for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )

	SENS_A,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandA0  -> block 3 / layer 4
	SENS_B,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandA1
	SENS_C,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandB0
	SENS_D,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandB1

    SEL_TIME,               // i alow the user to set up the input channel for the target_modes -> block 4 / layer 5
    SEL_TEX,
    SEL_VID,
    SEL_FRM,    

	FLAG_TIME,   			// 0 or 1   flag to set the "map the source_mode to the gl time" target mode -> block 5 / layer 6
	FLAG_TEX,   			// 0 or 1   flag to set the "map the source_mode to select loaded textures" target mode
	FLAG_EXT,   			// 0 or 1   flag to set any TRG source_mode to an EXT CLK target mode, fist come first serve 
	FLAG_VID,   			// 0 or 1   flag to set the "map the source:mode to enable video-to-gl texture" target mode 

    FLAG_AUDIO_A,           // "invisible flags" set by the system  -> block 6 / layer 7
    FLAG_AUDIO_B,
    FLAG_DUMMY,             // dummy to keep the 4_per_block_rule 

	IS_STORED,              // "invisible " to indicate a "mode is stored" into the central mode buffer, set by the system 

//  this are "one shot toggle flags" to trigger certain system operations - than reset to zero ny the system

    SET_STORE,  			// 0 or 1   on 1 the central mode buffer will be converted to an human readable and stored on usb -> block 7 / layer 8
    SET_LOAD,   			// 0 or 1   on 1 the central mode buffer will be loaded and parsed from a file on usb
    LOG_STORE,  			// 0 or 1   will store the system logs un usb - is also triggered on critical issues
    KLN_LOAD,   			// 0 or 1   will update the kernel on sd from usb if a .img file is available  

    MODETABLE_COUNT
};

        typedef void                            (CKernel::*ModeFunc)(int);         // for the new menu selector -> easier to expand, right? "add modes by only extending the table"

                ModeFunc                        g_modeTable[9/* SOURCE_MODE_MAX */]            =        {   &CKernel::modeADC,          // "copy value" mode
                                                                                                            &CKernel::modeTRG,          
                                                                                                            &CKernel::modeBPM,
                                                                                                            &CKernel::modeLF1,          // "copy value" mode
                                                                                                            &CKernel::modeLF2,          // "copy value" mode

                                                                                                            &CKernel::modeAudioAb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioAb1,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb1 };   // "copy value" mode

enum MapType 
{ 
    MAP_MODE = 0, 
    MAP_VALUE 
};

enum ModeFlags 
{
    GROUP_BASE  = 0,
    GROUP_FLAG1 = 1,
    GROUP_FLAG2 = 2,
    GROUP_COUNT = 3
};
// per-block/slot: mode vs value
static const uint8_t g_mapType[BLOCK_COUNT][4] =
{
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // block 0 (CH0..CH3 modes)
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // block 1 (CH4..CH7 modes)

    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 2 (LF1_WAVE..LF2_MULT)
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 3 (SENS_A..SENS_D)
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 4 (SEL_TIME..SEL_FRM) 0..7
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 5 (target flags) 0/1
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 6 (mute) 0/1
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }  // block 7 (hw/sys toggles) 0/1
};
// per-block/slot: roof for MAP_VALUE blocks, and baseLen for MAP_MODE blocks (blocks 0/1)
static const uint8_t g_valueRoof[BLOCK_COUNT][4] =
{
    { 5,  5,  5,  5  },   // block 0 (MAP_MODE) baseLen
    { 5,  5,  5,  5  },   // block 1 (MAP_MODE) baseLen

    { 4,  4,  7,  7  },   // block 2 (MAP_VALUE) roofs <- wait! 4 waves and 7 mults ( 1,2,4,8,16,32,64)
    { 64, 64, 64, 64 },   // block 3 (MAP_VALUE) roofs
    { 7,  7,  7,  7  },   // block 4 (MAP_VALUE) roofs (0..6 because ch7 is reserved! )
    { 2,  2,  2,  2  },   // block 5 (MAP_VALUE) roofs (0/1)
    { 0,  0,  0,  2  },   // block 6 (MAP_VALUE) roofs (0/1)
    { 2,  2,  2,  2  }    // block 6 (MAP_VALUE) roofs (0/1)
};
// per-group lengths (used for optional groups)
static const uint8_t g_groupLen[GROUP_COUNT] =
{
    5, // GROUP_BASE
    2, // GROUP_FLAG1 -> AUDIO_A
    2  // GROUP_FLAG1+1 -> AUDIO_B
};
// per-group mode IDs (open table; order + holes preserved here)
static const uint8_t g_groupModes[GROUP_COUNT][5] =
{
    { 0, 1, 2, 3, 4 },   // GROUP_BASE
    { 5, 6, 0, 0, 0 },   // AUDIO_A
    { 7, 8, 0, 0, 0 }    // AUDIO_B
};
// runtime arrays produced/consumed by the 3 functions
uint8_t g_modeRoof[BLOCK_COUNT * 4];                            // actually we only need 2 * 4 since only block 0 and 0 need active remapping
uint8_t g_modeMap [BLOCK_COUNT * 4][9/*SOURCE_MODE_MAX*/];           // actually we only need 2 * 4 since only block 0 and 0 need active remapping


void            CKernel::get_gl_time(unsigned sys_time)
{
                if (!g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME])
                    {
                    gl_time = sys_time / 1000000.0f;
                    }
}

void            CKernel::set_mode_roof_map(uint8_t block)
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

                    // base group length comes from g_valueRoof for MAP_MODE blocks (e.g. 5,5,5,5)
                    const uint8_t baseLen = g_valueRoof[block][slot];

                    for (uint8_t i = 0; i < baseLen; ++i)
                        g_modeMap[row][dst++] = g_groupModes[GROUP_BASE][i];

                    // optional groups (A/B)
                    for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
                    {
                        const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);
                        if (!g_centralModeBuffer[g_currentProgramBuffer][flag_pos]) continue;

                        for (uint8_t i = 0; i < g_groupLen[group]; ++i)
                            g_modeMap[row][dst++] = g_groupModes[group][i];
                    }

                    g_modeRoof[row] = dst;
                }
}

void            CKernel::mapMenuGroup(uint8_t block)
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
                uint8_t mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                ModeFunc fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }
}

void            CKernel::getChannelModeB(uint8_t block )
{
                const uint8_t base = block << 2;



                int p_channel = base + 0;

                uint8_t mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                ModeFunc fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }



                p_channel = base + 1;

                mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }



                p_channel = base + 2;

                mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }



                p_channel = base + 3;

                mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }
}

void            CKernel::applyTargetModes(int p_channel)
{
                // hardwired target:
                // CH7 always selects the current program
                if (p_channel == ADC_SELECT_PRG)
                {
                    g_gl_program_current = g_inOutMatrixInt[p_channel][OUT];
                }

                // optional target:
                // selected channel OUT controls active texture
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TEX] &&
                    g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] == p_channel)
                {
                    m_activeTEX = g_inOutMatrixInt[p_channel][OUT];
                }

                // optional target:
                // selected channel OUT controls active video
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID] &&
                    g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] == p_channel)
                {
                    m_activeVideo = g_inOutMatrixInt[p_channel][OUT];
                }

                // optional target:
                // selected channel OUT controls active frame
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID] &&
                    g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM] == p_channel)
                {
                    m_activeFrame = g_inOutMatrixInt[p_channel][OUT];
                }

                // optional target:
                // selected channel OUT controls shader/program time
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME] &&
                    g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] == p_channel)
                {
                    gl_time = g_inOutMatrixInt[p_channel][OUT];
                }

                // optional target:
                // selected channel trigger state is used as external BPM clock
                // no own threshold logic here; TRF comes from modeTRG()
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT] &&
                //  g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT] == p_channel &&
                    g_inOutMatrixInt[p_channel][TRF])
                {
                    calculate1BPM(1, g_currentTime);
                }
}

void            CKernel::applyTargetModes           (   )       // current!
{
                // hardwired target:
                // CH7 always selects the current program
                g_gl_program_current = g_inOutMatrixInt[ADC_SELECT_PRG][OUT];   // rather RAW?

                // optional target:
                // selected channel OUT controls active texture
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TEX])
                    {
                    m_activeTEX =   g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX]][OUT];
                    }

                // optional target:
                // selected channel OUT controls active video
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID])
                    {
                    m_activeVideo = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_VID]][OUT];
                    }

                // optional target:
                // selected channel OUT controls active frame
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID])
                    {
                    m_activeFrame = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM]][OUT];
                    }

                // optional target:
                // selected channel OUT controls shader/program time
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME])
                    {
                    gl_time =       g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME]][OUT];
                    }

                if (!g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME])
                    {
                    gl_time =       g_currentTime / 1000000.0f;
                    }

                // optional target:
                // external BPM clock
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT])
                    {
                    calculate1BPM(1, g_extClockTime);
                    }
}

void            CKernel::modeADC                    (   int p_channel) 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][VAL];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][VAL];       
                
           //  applyTargetModes(p_channel);
}

void            CKernel::modeTRG                    (   int p_channel)
{
                if (  g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                    ! g_inOutMatrixInt[p_channel][TRF])
                    {
                    g_inOutMatrixFlt[p_channel][OUT]    = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT]    = g_inOutMatrixInt[p_channel][RND];

                    g_inOutMatrixInt[p_channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }

           //  applyTargetModes(p_channel);    // i do need it here too, right??
}

void            CKernel::modeTRG                    (   int p_channel) // current
{
                if (  g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                    ! g_inOutMatrixInt[p_channel][TRF])
                    {
                    g_inOutMatrixFlt[p_channel][OUT]    = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT]    = g_inOutMatrixInt[p_channel][RND];
/*
                    if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT])
                        {
                        calculate1BPM(1, g_currentTime);
                        }
*/
                    g_extClockTime                      = g_currentTime;

                    g_inOutMatrixInt[p_channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }
}

void            CKernel::modeBPM                    (   int p_channel)   // <- currentTime should be g_currentTime -> global member set during Run()  - or a call parameter!
{ 
                if ( g_currentTime >= g_lfoBpmMatrix[p_channel][NBT] )
                    {
                    g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][RND];
                    }

           //  applyTargetModes(p_channel);
}

void            CKernel::modeLF1                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF1];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF1];

           //  applyTargetModes(p_channel);
}

void            CKernel::modeLF2                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF2];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF2]; 

           //  applyTargetModes(p_channel);
}

void            CKernel::modeAudioAb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU0];

           //  applyTargetModes(p_channel);
}

void            CKernel::modeAudioAb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU1];

           //  applyTargetModes(p_channel);                
}

void            CKernel::modeAudioBb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU2];

           //  applyTargetModes(p_channel);                
}

void            CKernel::modeAudioBb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU3];

           //  applyTargetModes(p_channel);
}

void            CKernel::button_consumer()
{
                // read button state once
                const bool a_single = g_buttons_states[BTN_A][BTN_SINGLE]    != 0;
                const bool b_single = g_buttons_states[BTN_B][BTN_SINGLE]    != 0;
                const bool a_hold   = g_buttons_states[BTN_A][BTN_HOLD_TICK] != 0;
                const bool b_hold   = g_buttons_states[BTN_B][BTN_HOLD_TICK] != 0;

                // layer 0: normal runtime
                if (!a_hold && !b_hold)
                    {
                    g_menuLayer = 0;

                    if (a_single)
                        {
                        calculate1BPM(0, g_currentTime);            // button A: BPM tap timestamp
                        g_buttons_states[BTN_A][BTN_SINGLE] = 0;
                        }

                    if (b_single)
                        {
                        g_buttons_states[BTN_B][BTN_SINGLE] = 0;    // button B: store action
                        }
                    return;
                    }

                if (a_hold && !b_hold)                              // hold A -> layer 1 -> block 0
                    {
                    g_menuLayer = 1;
                    }

                if (b_hold && g_menuLayer < 2)                      // hold B -> layer 2 by default
                    {
                    g_menuLayer = 2;
                    }

                if (b_hold && a_single)                             // hold B + press A -> cycle layers 3..7..3
                    {
                    if (g_menuLayer < 3 || g_menuLayer >= 7)
                        {
                        g_menuLayer = 3;
                        }
                    else
                        {
                        g_menuLayer++;
                        }

                    g_buttons_states[BTN_A][BTN_SINGLE] = 0;
                    }

                switch (g_menuLayer)                                // dispatch active layer to block
                    {
                    case 1:                                         // block 0: MODE_CH0..MODE_CH3
                        set_mode_roof_map(0);
                        mapMenuGroup(0);
                        break;

                    case 2:                                         // block 1: MODE_CH4..MODE_CH7
                        set_mode_roof_map(1);
                        mapMenuGroup(1);
                        break;

                    case 3:                                         // block 2: LFO
                        set_mode_roof_map(2);
                        mapMenuGroup(2);
                        break;

                    case 4:                                         // block 3: sensitivity
                        set_mode_roof_map(3);
                        mapMenuGroup(3);
                        break;

                    case 5:                                         // block 4: target channel selectors
                        set_mode_roof_map(4);
                        mapMenuGroup(4);
                        break;

                    case 6:                                         // block 5: target flags
                        set_mode_roof_map(5);
                        mapMenuGroup(5);
                        break;

                    case 7:                                         // block 6: hw/sys toggles
                        set_mode_roof_map(7);
                        mapMenuGroup(7);
                        break;

                    default:
                        break;
                    }
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

/*
Corrected list, only the relevant project-control functions:

1. **per channel**

        getChannelMode(ch);
        applyTargetModes(ch); ( called from the ModeFunc g_modeTable functions )

2. **per active block**

        set_mode_roof_map(block);   ( called from button_consumer )
        mapMenuGroup(block);        ( called from button_consumer )

3. **per loop**
buttonPing(buttonA, pinA,, buttonB, pinB );
button_consumer();
storeModes();
get_gl_time(g_currentTime);

Rendering / overlay functions are outside this control-flow list.

*/