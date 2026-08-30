// Arrays used here but not declared in this snippet:

uint8_t g_modeRoof[BLOCK_COUNT * 4];                            // here we store the calculated "roof" from set_mode_roof_map()

uint8_t g_modeMap[BLOCK_COUNT * 4][MAX_MODEMAP_SIZE];           // here we build a new modemap for a gables selection 

uint8_t g_centralModeBuffer[PROGRAM_COUNT][BUFFER_SIZE];        // this is the global per-program store and setup matrix/array 

bool g_menuPickUpFlag[BLOCK_COUNT * 4];                         // the global pickup / "no-jump" table 

uint16_t g_inOutMatrixInt[INPUT_COUNT][MATRIX_FIELD_COUNT];     // my global in/out matrix / array

ModeFunc g_modeTable[MODE_COUNT];                               // the table of mode-function 


// Also used, but not arrays:
// okay, here is still some confusion:
// first i use this as table for the entries of the stored per program parameters variables and flags
// also do i "pack" them in groups by four since mapping is only possible ( and backed by mapMenuGroup )
// for 4 channels at a time, means MODE_CH0 - MODE_CH7 stores the index for the selected mode.
// than i also store values i think i can also map via mapMenuGroup for certain settings like to pic the lfo, multiplier or sensitivity
// that i can use the same mapping mechanism and have kinda menu layers in group by four,
// layer 0 = default, layer 1 = map mode to group 0 ( physical adc 0-3 ), layer 2 = map mode to group 2 ( physical adc 4-7 )
// layer 3 = select/ set LFO 0 & LFO 1, layer 4 = set sensitivity... here it becomes "complicated"
// the audio detection mode is activated by the analysis of adc signals, means a flag ( FLAG_AUDIO_A & FLAG_AUDIO_A ) will decide 
// if i can map my 4-6 default modes to any channel or the additional 4 audio sensitivities.BUT audio sensitivity is auto assigned,
// i have two separate inputs ( adc channel detections ) with two bands ( i can assign to any channel )  and ony on detection 
// i should have access to the layer / group.
// than i need some flags to activate target modes and i have to store them too in enum central mode buffer!



enum source_modes
{
    IN_MODE_ADC,        // adc direct to gl
    IN_MODE_TRG,        // a adc threshold triggers to store a random volume  to gl
    IN_MODE_BMP,        // a internal / external ( later more ) BPM wll store a random value to gl
    IN_MODE_LF1,        // an internal waketable value calculated from the bpm is send continuously to gl 
    IN_MODE_LF2,        // an internal waketable value calculated from the bpm is send continuously to gl

    MODE_BAND_L0,       // if audio0 is detected ( and audiomode flag set ? ) this sends the averaged low 0 band output to gl
    MODE_BAND_H0,       // if audio0 is detected ( and audiomode flag set ? ) this sends the averaged high 0 band output to gl
    MODE_BAND_L1,       // if audio1 is detected ( and audiomode flag set ? ) this sends the averaged low 0 band output to gl
    MODE_BAND_H1        // if audio1 is detected ( and audiomode flag set ? ) this sends the averaged high 0 band output to gl

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

enum centralModeBuffer // organised in blocks/layers for the mapMenuGroup
{
	MODE_CH0 = 0,           // store the mode ( from g_modeTable[] ) for cannel 0 -> layer 1
	MODE_CH1,               // store the mode ( from g_modeTable[] ) for cannel 1
	MODE_CH2,               // store the mode ( from g_modeTable[] ) for cannel 2
	MODE_CH3,               // store the mode ( from g_modeTable[] ) for cannel 3

	MODE_CH4,               // store the mode ( from g_modeTable[] ) for cannel 4 -> layer 2
	MODE_CH5,               // store the mode ( from g_modeTable[] ) for cannel 5
	MODE_CH6,               // store the mode ( from g_modeTable[] ) for cannel 6
	MODE_CH7,               // store the mode ( from g_modeTable[] ) for cannel 7

	LF1_WAVE,               // stores waveform index for lfo1 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )  -> layer 3
	LF2_WAVE,               // stores waveform index for lfo2 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )
	LF1_MULT,               // stores the multiplier index for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )
	LF2_MULT,               // stores the multiplier index for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )

	SENS_A,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandA0  -> layer 4
	SENS_B,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandA1
	SENS_C,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandB0
	SENS_D,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandB1

    SEL_TIME,               
    SEL_TEX,
    SEL_VID,
    SEL_FRM,    
    
	FLAG_TIME,   			// 0 or 1   ( wait - i propose a simple 4 to 7 and 0 as of, means anything else than 0 is the actual input p_channel problem, it need to overrule CH*_MODE )
	FLAG_TEX,   			// 0 or 1   ( how we can do it? also, dont i want more than only p_channel 4-7 assignable? )
	FLAG_EXT,   			// 0 or 1   ( whats about the approach in readADC() where i modify the number of possible modes in the modematrix )
	FLAG_VID,   			// 0 or 1   ( like if i have one here CH*_MODE "opens" up for this modes - requires a constant check and update but... )
// i can use this for "invisible flags" too, like not set by the user
    FLAG_AUDIO_A,
    FLAG_AUDIO_B,
    DUMMY_FLAG_C,
	DUMMY_FLAG_D,

//  this are "one shot toggle flags" to trigger certain system operations 

    SET_STORE,  			// 0 or 1   ( if set to 1 and the MENU_LAYER_COUNT is zero again ( button released ?) the file operation starts )
    SET_LOAD,   			// 0 or 1
    LOG_STORE,  			// 0 or 1
    KLN_LOAD,   			// 0 or 1

	IS_STORED,              // not really a mode but a flag - important: obey the "4 per block" rule

    MODETABLE_COUNT
};

enum ModeLengthFlag // dead code?
{
                MODELENDEFAULT  = 0,
                MODELEN_AUDIO_A,            	    // any extra mode that modifies the number of max modes per channel must have an enum
                MODELEN_AUDIO_B,
                MODELEN_AUDIO_C,
                MODELEN_LFO_A,
                MODELEN_SENS_A,

                MODELEN_FLAG_COUNT
};



uint8_t g_currentProgramBuffer;
uint8_t g_menu_mode_new;

// Minimum missing defines/constants:

#define BLOCK_COUNT       7
#define GROUP_COUNT       4
#define MAX_MODEMAP_SIZE  12   // for current mode groups: 5 + 2 + 4 + 1

// For your current shown data, `g_modeMap` needs at least:

uint8_t g_modeMap[20][12];

// because:

// 5 blocks * 4 slots = 20 rows
// max mode entries = 5 + 2 + 4 + 1 = 12


#define MAP_MODE 0
#define MAP_VALUE 1

static const uint8_t g_mapType[BLOCK_COUNT][4] =
{
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  },   // block 0 - layer 1 - map mode for ch0 to ch3    ( 0 to g_modeRoof[])
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  },   // block 1 - layer 2 - map mode for ch4 to ch7    ( 0 to g_modeRoof[])

    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },   // block 2 - layer 3 - LFO/control values         ( 0 to max_wave / 0 to max mult )
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }    // block 3 - layer 4 -  audio sensitivity         ( 0 to 64 )
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }    // block 4 - layer 5 - target mode flags          ( 0 to 1 / true false )
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }    // block 5 - layer 6 - hardware / system toggles  ( 0 to 1 / true false )
};

static const uint8_t g_valueRoof[BLOCK_COUNT][4] =
{
    { 0, 0, 0, 0 },       // block 0: not used, MAP_MODE
    { 0, 0, 0, 0 },       // block 1: not used, MAP_MODE
    { 3, 3, 7, 7 },       // block 3: LFO wave/wave/mult/mult
    { 64, 64, 64, 64 },   // block 4: audio sensitivity
    { 2,2,2,2 },          
};

static const uint8_t g_groupLength[BLOCK_COUNT][4][GROUP_COUNT] =
{
    {
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 }
    },
    {
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 }
    },
    {
        { 5, 2, 0, 0 },
        { 5, 2, 0, 0 },
        { 5, 2, 0, 0 },
        { 5, 2, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

static const uint8_t g_groupModes[GROUP_COUNT][5] =
{
    {  0,  1,  2,  3,  4 },
    {  8,  9,  0,  0,  0 },
    { 12, 13, 14, 15,  0 },
    { 10,  0,  0,  0,  0 }
};

void CKernel::set_mode_roof_map(uint8_t block)
{
    const uint8_t f_first_flag = FLAG_TIME;
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

        for (uint8_t i = 0; i < g_groupLength[block][slot][GROUP_BASE]; ++i)
        {
            g_modeMap[row][dst++] = g_groupModes[GROUP_BASE][i];
        }

        for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
        {
            const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

            if (!g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
            {
                continue;
            }

            for (uint8_t i = 0; i < g_groupLength[block][slot][group]; ++i)
            {
                g_modeMap[row][dst++] = g_groupModes[group][i];
            }
        }
        g_modeRoof[row] = dst;
    }
}

// we map groups of four, later called "blocks" here, 
// a) because we have 4 adc inputs for mapping each time 
// b) because we than can use a menu-layer model where we change the mode-layer to set up different things
// c) we attempt to use the function not only for mapping of values to modes but also just for values 

void CKernel::mapMenuGroup(uint8_t menu_id, uint8_t block)
{
    if (g_menu_mode_new != menu_id)
    {
        return;
    }

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

// Use result:

uint8_t index = g_centralModeBuffer[g_currentProgramBuffer][row];

if (g_mapType[block][slot] == MAP_MODE)
{
    uint8_t mode = g_modeMap[row][index];
    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(row);
    }
}
else
{
    uint8_t value = index;
}

void CKernel::getChannelModeB(int p_channel)
{
    uint8_t index = g_centralModeBuffer[g_currentProgramBuffer][p_channel];

    uint8_t mode = g_modeMap[p_channel][index];

    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(p_channel);
    }
}

        typedef void                            (CKernel::*ModeFunc)(int);         // for the new menu selector -> easier to expand, right? "add modes by only extending the table"

                ModeFunc                        g_modeTable[9]                                 =       {   &CKernel::modeADC,          // "copy value" mode
                                                                                                            &CKernel::modeTRG,          
                                                                                                            &CKernel::modeBPM,
                                                                                                            &CKernel::modeLF1,          // "copy value" mode
                                                                                                            &CKernel::modeLF2,          // "copy value" mode
                                                                                                            /*
                                                                                                            &CKernel::modeSelectTex,
                                                                                                            &CKernel::modeSelectFrame,
                                                                                                            &CKernel::modeSelectVideo,

                                                                                                            nullptr,
                                                                                                            nullptr,
                                                                                                            nullptr,
                                                                                                            */
                                                                                                            &CKernel::modeAudioAb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioAb1,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb1 };   // "copy value" mode

void CKernel::applyTargetModes(int p_channel)
{
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TEX] && g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] == p_channel)
                    {
                    m_activeTEX = g_inOutMatrixInt[p_channel][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID] && g_centralModeBuffer[g_currentProgramBuffer][SEL_VID_NUM] == p_channel)
                    {
                    m_activeVideo = g_inOutMatrixInt[p_channel][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID] && g_centralModeBuffer[g_currentProgramBuffer][SEL_VID_FRM] == p_channel)
                    {
                    m_activeFrame = g_inOutMatrixInt[p_channel][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME] && g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] == p_channel)
                    {
                    gl_time = g_inOutMatrixInt[p_channel][OUT];
                    }
// i still wonder: is this rather a source-mode than a target-mode, maybe just and automated thingy running aside mode 2 "TRG"?

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT] && g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT] == p_channel && g_centralModeBuffer[g_currentProgramBuffer][p_channel] != p_channel)
                    {
                    if (g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                        !g_inOutMatrixInt[p_channel][TRF])
                        {
                        calculate1BPM(1, g_currentTime);
                        g_extClockTriggerFlag[p_channel] =  = true;
                        }
                    else if (g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                        {
                        g_extClockTriggerFlag[p_channel] =  = false;
                        }
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

           //  applyTargetModes(p_channel);
}

void            CKernel::modeBPM                    (   int p_channel /* , currentTime*/)   // <- currentTime should be g_currentTime -> global member set during Run()  - or a call parameter!
{ 
                if ( /* currentTime */ g_currentTime >= g_lfoBpmMatrix[p_channel][NBT] )// g_nextBeatTime[g_activeBpmChannel])      // <- g_nextBeatTime is now part of the g_lfoBpmMatrix -> enum lfo_bpm_types NBT -> nextBeatTime
                                                                            
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
}

void            CKernel::modeLF2                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF2];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF2]; 

           //  applyTargetModes(p_channel);
}
/*
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

void            CKernel::modeEXT                    (   int p_channel)
{
                    if (g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                        !g_inOutMatrixInt[p_channel][TRF])
                        {
                        calculate1BPM(1, g_currentTime);
                        g_extClockTriggerFlag[p_channel] =  = true;
                        }
                    else if (g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                        {
                        g_extClockTriggerFlag[p_channel] =  = false;
                        }
}
*/
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