struct glsl_state
{
    GLuint                      gl_shader_id[MAX_SHADER];
    GLuint                      gl_program_id[MAX_SHADER];

    bool                        shader_valid[MAX_SHADER];
    // user uniforms                                            // this is the actual common shader struct we define for 
    GLint                       u_time[MAX_SHADER];
    GLint                       u_tres[MAX_SHADER];
    GLint                       u_seed[MAX_SHADER];
    GLint                       u_aud[MAX_SHADER];
    GLint                       u_col[MAX_SHADER];
    GLint                       u_par_a[MAX_SHADER];
    GLint                       u_par_b[MAX_SHADER];
    GLint                       u_tex_l[MAX_SHADER];
    // overlay uniforms
    GLint                       u_atlas[MAX_OMF];
    GLint                       u_tile_count[MAX_OMF];
    GLint                       u_tile_rect[MAX_OMF];
    GLint                       u_tile_index[MAX_OMF];
    // Overlay atlas sampler
//  GLint                       u_atlas[MAX_OMF]; 		// retarded shit, only shows that you have not read my gfx code or have no understanding anyway!
														// ever wondered why i use the same strucs for tex and shaders 
														// and why i have gl_tex_id[MAX_TEXTURE] or u_tex_id[MAX_SHADER][MAX_TEXTURE]; ???

	GLfloat						u_q00_coord[4];			// MENU_COORD_MODE_0

	GLfloat						u_d00_coord[4];			// MENU_COORD_DETAIL_0
	GLfloat						u_d01_coord[4];			// MENU_COORD_DETAIL_1

	GLfloat						u_q01_coord[4];			// MENU_COORD_MODE_1

	GLfloat						u_d02_coord[4];			// MENU_COORD_DETAIL_2
	GLfloat						u_d03_coord[4];			// MENU_COORD_DETAIL_3

	GLfloat						u_q02_coord[4];			// MENU_COORD_MODE_2

	GLfloat						u_d04_coord[4];			// MENU_COORD_DETAIL_4
	GLfloat						u_d05_coord[4];			// MENU_COORD_DETAIL_5

	GLfloat						u_q03_coord[4];			// MENU_COORD_MODE_3

	GLfloat						u_d06_coord[4];			// MENU_COORD_DETAIL_6
	GLfloat						u_d07_coord[4];			// MENU_COORD_DETAIL_7

	GLfloat						u_t00_coord[4];			// MENU_COORD_TARGET_TIME
	GLfloat						u_t01_coord[4];			// MENU_COORD_TARGET_TEXTURE
	GLfloat						u_t02_coord[4];			// MENU_COORD_TARGET_VIDEO
	GLfloat						u_t03_coord[4];			// MENU_COORD_TARGET_FRAME
	
	GLfloat						u_t05_coord[4];			// MENU_COORD_TARGET_PROGRAM

	GLfloat						u_x00_coord[4];			// MENU_COORD_EXTERN_SELECTOR

	GLfloat						u_up_coord[4];			// MENU_COORD_ARROW_UP
	GLfloat						u_dw_coord[4];			// MENU_COORD_ARROW_DOWN

	GLfloat						u_bpm_coord[4];			// MENU_COORD_BPM_STRING
	GLfloat						u_100_coord[4];			// MENU_COORD_BPM_100
	GLfloat						u_010_coord[4];			// MENU_COORD_BPM_010
	GLfloat						u_001_coord[4];			// MENU_COORD_BPM_001
	GLfloat						u_dot_coord[4];			// MENU_COORD_BPM_DOT
	GLfloat						u_10d_coord[4];			// MENU_COORD_BPM_10D
	GLfloat						u_01d_coord[4];			// MENU_COORD_BPM_01D
};

enum inModeNames
{
    IN_MODE_ADC,
    IN_MODE_TRG,
    IN_MODE_BMP,    

    IN_MODE_LF_0,
    IN_MODE_LF_1,

    IN_MODE_AU_AL,
    IN_MODE_AU_AH,
    IN_MODE_AU_BL,
    IN_MODE_AU_BH,

    IN_MODE_MIDI_NOTE,
    IN_MODE_MIDI_CC0,
    IN_MODE_MIDI_CC1,

    IN_MODE_NAME_COUNT
};

enum centralModeBuffer
{
// block 00 / layer 1
    MODE_CH0 = 0,           // store the mode ( from g_modeTable[] ) for cannel 0
    MODE_CH1,				// store the mode ( from g_modeTable[] ) for cannel 1
    MODE_CH2,               // store the mode ( from g_modeTable[] ) for cannel 2
    MODE_CH3,               // store the mode ( from g_modeTable[] ) for cannel 3
// block 01 / layer 2
    MODE_CH4,               // store the mode ( from g_modeTable[] ) for cannel 4
    MODE_CH5,               // store the mode ( from g_modeTable[] ) for cannel 5
    MODE_CH6,               // store the mode ( from g_modeTable[] ) for cannel 6
    MODE_CH7,               // store the mode ( from g_modeTable[] ) for cannel 7
// block 02 / layer 3
    LF1_WAVE,               // stores waveform for lfo1 from m_bufferLfo[waveTableCount][LFO_SAMPLES]
    LF2_WAVE,               // stores waveform for lfo2 from m_bufferLfo[waveTableCount][LFO_SAMPLES]
    LF1_MULT,               // stores the multiplier for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )
    LF2_MULT,               // stores the multiplier for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )
// block 03 / layer 4
    THRESHOLD_L,            // NEW - the threshold low for IN_MODE_TRG
    THRESHOLD_H,            // NEW - the threshold heigh for IN_MODE_TRG
    SEL_EXT,                // NEW - a) extern clock input
//  EFFECT,                 // NEW - b) hypothetical "strength" for the randomizer - not implemented
    ATTENUATION,            // New - attenuation for the readAndConvertADC() 
// block 04 / layer 5
    SENS_A,                 // stores the sensitivity for the audio mode ( available if enabled ) bandA0
    SENS_B,                 // stores the sensitivity for the audio mode ( available if enabled ) bandA1
    SENS_C,                 // stores the sensitivity for the audio mode ( available if enabled ) bandB0
    SENS_D,                 // stores the sensitivity for the audio mode ( available if enabled ) bandB1
// block 05 / layer 6 
    MIDI_CHANNEL,
    MIDI_CC0,
    MIDI_CC1,
    MIDI_RANGE,
// block 06 / layer 7
    SEL_TIME,               // SEL_TIME & FLAG_TIME should be unified by count any SEL_TIME < MAX as true
    SEL_TEX,
    SEL_VID,
    SEL_FRM,
// block 07 / layer 8 -
    SET_STORE,
    SET_LOAD,
    KLN_LOAD,    
    LOG_STORE,
// block 08 / layer  9  - "mute" block
    SEL_PRG,
    FLAG_AUDIO_A,           // for internal use only! 
    FLAG_AUDIO_B,
    FLAG_MIDI,
// block 09 / layer 10  - "mute" block
    FLAG_DUMMY_B,            // instead of an additional "allow prg changes" global flag - what if this will never be released again!?!
    FLAG_EXT,
    LAST_EXT,
    IS_STORED,              // needs to be the last position as far as i remember

    MODETABLE_COUNT         // theoretical i can now define BLOCK_COUNT as MODETABLE_COUNT / 4 correct????
};

enum AtlasTileIndex
{
    // Row 0: mode tiles
    ATLAS_TILE_MODE_ADC,            // 00 - first of modes 
    ATLAS_TILE_MODE_TRG,            // 01
    ATLAS_TILE_MODE_BMP,            // 02

    ATLAS_TILE_MODE_LF0,            // 03
    ATLAS_TILE_MODE_LF1,            // 04

    ATLAS_TILE_MODE_AU_AL,          // 05
    ATLAS_TILE_MODE_AU_AH,          // 06
    ATLAS_TILE_MODE_AU_BL,          // 07

    // Row 1: remaining mode tiles
    ATLAS_TILE_MODE_AU_BH,          // 08

    ATLAS_TILE_MODE_MIDI_NOTE,      // 09
    ATLAS_TILE_MODE_MIDI_CC0,       // 10
    ATLAS_TILE_MODE_MIDI_CC1,       // 11

    ATLAS_TILE_MODE_12,             // 12
    ATLAS_TILE_MODE_13,             // 13
    ATLAS_TILE_MODE_14,             // 14
    ATLAS_TILE_MODE_15,             // 15

    // Row 2: target and navigation tiles
    ATLAS_TILE_TARGET_TIME,         // 16 - first of target modes
    ATLAS_TILE_TARGET_TEXTURE,      // 17
    ATLAS_TILE_TARGET_VIDEO,        // 18
    ATLAS_TILE_TARGET_FRAME,        // 19
    ATLAS_TILE_TARGET_GL_PROGRAM,   // 20

    ATLAS_TILE_BLANK,               // 21 - placeholder

    ATLAS_TILE_ARROW_UP,            // 22 - arrow up
    ATLAS_TILE_ARROW_DOWN,          // 23 - arrow down

    // Row 3: LFO waveform tiles
    ATLAS_TILE_LFO_WAVE_SINE,       // 24 - first of waveforms
    ATLAS_TILE_LFO_WAVE_TRIANGLE,   // 25
    ATLAS_TILE_LFO_WAVE_RAMP_UP,    // 26
    ATLAS_TILE_LFO_WAVE_RAMP_DOWN,  // 27
    ATLAS_TILE_LFO_WAVE_SMOOTH_UP,  // 28
    ATLAS_TILE_LFO_WAVE_SMOOTH_DOWN,// 29
    ATLAS_TILE_LFO_WAVE_EXPONENTIAL,// 30
    ATLAS_TILE_LFO_WAVE_RANDOM,     // 31

    // Row 4: divider tiles
    ATLAS_TILE_DIVIDER_1_64,        // 32 - first of dividers 
    ATLAS_TILE_DIVIDER_1_32,        // 33
    ATLAS_TILE_DIVIDER_1_16,        // 34
    ATLAS_TILE_DIVIDER_1_8,         // 35
    ATLAS_TILE_DIVIDER_1_4,         // 36
    ATLAS_TILE_DIVIDER_1_2,         // 37
    ATLAS_TILE_DIVIDER_1_1,         // 38

    ATLAS_TILE_EXTERN_SELECTOR,     // 39 - "X" for external input mode selected

    // Row 5: labels and reserved tiles
    ATLAS_TILE_INDICATOR_BAR_L,     // 40   // used for ATLAS_TILE_MODE_TRG ( THRESHOLD_L )
    ATLAS_TILE_INDICATOR_BAR_R,     // 41   // used for ATLAS_TILE_MODE_ADC ( g_inOutMatrixInt[0][RAW] ), ATLAS_TILE_MODE_TRG ( THRESHOLD_H ), ATLAS_TILE_MODE_AU_XH ( SENS_B / SENS_D )
    ATLAS_TILE_INDICATOR_BAR_BPM,   // 42   // 
    ATLAS_TILE_43,                  // 43
    ATLAS_TILE_44,                  // 44
    ATLAS_TILE_45,                  // 45

    ATLAS_TILE_LABEL_FPS,           // 46 - "fps"
    ATLAS_TILE_LABEL_BPM,           // 47 - "bpm"

    // Row 6: number glyphs 0-7
    ATLAS_TILE_NUMBER_0,            // 48 - first of numbers ( 0 )
    ATLAS_TILE_NUMBER_1,            // 49
    ATLAS_TILE_NUMBER_2,            // 50
    ATLAS_TILE_NUMBER_3,            // 51
    ATLAS_TILE_NUMBER_4,            // 52
    ATLAS_TILE_NUMBER_5,            // 53
    ATLAS_TILE_NUMBER_6,            // 54
    ATLAS_TILE_NUMBER_7,            // 55

    // Row 7: number glyphs, decimal point, and system tiles
    ATLAS_TILE_NUMBER_8,            // 56
    ATLAS_TILE_NUMBER_9,            // 57
    ATLAS_TILE_NUMBER_DOT,          // 58

    ATLAS_TILE_SYSTEM_IDLE,         // 59 
    ATLAS_TILE_SYSTEM_STORE,        // 60 - fist of sys-layer
    ATLAS_TILE_SYSTEM_LOAD,         // 61
    ATLAS_TILE_SYSTEM_UPDATE,       // 62
    ATLAS_TILE_SYSTEM_LOG,          // 63

    ATLAS_TILE_COUNT
};

const GLfloat g_atlasTileMap[ATLAS_TILE_COUNT][2] =
{
    // SVG row 0
    { 0.000f, 0.875f }, // ATLAS_TILE_MODE_ADC
    { 0.125f, 0.875f }, // ATLAS_TILE_MODE_TRG
    { 0.250f, 0.875f }, // ATLAS_TILE_MODE_BMP
    { 0.375f, 0.875f }, // ATLAS_TILE_MODE_LF0
    { 0.500f, 0.875f }, // ATLAS_TILE_MODE_LF1
    { 0.625f, 0.875f }, // ATLAS_TILE_MODE_AU_AL
    { 0.750f, 0.875f }, // ATLAS_TILE_MODE_AU_AH
    { 0.875f, 0.875f }, // ATLAS_TILE_MODE_AU_BL
    // SVG row 1
    { 0.000f, 0.750f }, // ATLAS_TILE_MODE_AU_BH
    { 0.125f, 0.750f }, // ATLAS_TILE_MODE_MIDI_NOTE
    { 0.250f, 0.750f }, // ATLAS_TILE_MODE_MIDI_CC0
    { 0.375f, 0.750f }, // ATLAS_TILE_MODE_MIDI_CC1
    { 0.500f, 0.750f }, // ATLAS_TILE_MODE_12
    { 0.625f, 0.750f }, // ATLAS_TILE_MODE_13
    { 0.750f, 0.750f }, // ATLAS_TILE_MODE_14
    { 0.875f, 0.750f }, // ATLAS_TILE_MODE_15
    // SVG row 2
    { 0.000f, 0.625f }, // ATLAS_TILE_TARGET_TIME
    { 0.125f, 0.625f }, // ATLAS_TILE_TARGET_TEXTURE
    { 0.250f, 0.625f }, // ATLAS_TILE_TARGET_VIDEO
    { 0.375f, 0.625f }, // ATLAS_TILE_TARGET_FRAME
    { 0.500f, 0.625f }, // ATLAS_TILE_TARGET_GL_PROGRAM
    { 0.625f, 0.625f }, // ATLAS_TILE_BLANK
    { 0.750f, 0.625f }, // ATLAS_TILE_ARROW_UP
    { 0.875f, 0.625f }, // ATLAS_TILE_ARROW_DOWN
    // SVG row 3
    { 0.000f, 0.500f }, // ATLAS_TILE_LFO_WAVE_SINE
    { 0.125f, 0.500f }, // ATLAS_TILE_LFO_WAVE_TRIANGLE
    { 0.250f, 0.500f }, // ATLAS_TILE_LFO_WAVE_RAMP_UP
    { 0.375f, 0.500f }, // ATLAS_TILE_LFO_WAVE_RAMP_DOWN
    { 0.500f, 0.500f }, // ATLAS_TILE_LFO_WAVE_SMOOTH_UP
    { 0.625f, 0.500f }, // ATLAS_TILE_LFO_WAVE_SMOOTH_DOWN
    { 0.750f, 0.500f }, // ATLAS_TILE_LFO_WAVE_EXPONENTIAL
    { 0.875f, 0.500f }, // ATLAS_TILE_LFO_WAVE_RANDOM
    // SVG row 4
    { 0.000f, 0.375f }, // ATLAS_TILE_DIVIDER_1_64
    { 0.125f, 0.375f }, // ATLAS_TILE_DIVIDER_1_32
    { 0.250f, 0.375f }, // ATLAS_TILE_DIVIDER_1_16
    { 0.375f, 0.375f }, // ATLAS_TILE_DIVIDER_1_8
    { 0.500f, 0.375f }, // ATLAS_TILE_DIVIDER_1_4
    { 0.625f, 0.375f }, // ATLAS_TILE_DIVIDER_1_2
    { 0.750f, 0.375f }, // ATLAS_TILE_DIVIDER_1_1
    { 0.875f, 0.375f }, // ATLAS_TILE_EXTERN_SELECTOR
    // SVG row 5
    { 0.000f, 0.250f }, // ATLAS_TILE_INDICATOR_BAR_L
    { 0.125f, 0.250f }, // ATLAS_TILE_INDICATOR_BAR_R
    { 0.250f, 0.250f }, // ATLAS_TILE_INDICATOR_BAR_BPM
    { 0.375f, 0.250f }, // ATLAS_TILE_43
    { 0.500f, 0.250f }, // ATLAS_TILE_44
    { 0.625f, 0.250f }, // ATLAS_TILE_45
    { 0.750f, 0.250f }, // ATLAS_TILE_LABEL_FPS
    { 0.875f, 0.250f }, // ATLAS_TILE_LABEL_BPM
    // SVG row 6
    { 0.000f, 0.125f }, // ATLAS_TILE_NUMBER_0
    { 0.125f, 0.125f }, // ATLAS_TILE_NUMBER_1
    { 0.250f, 0.125f }, // ATLAS_TILE_NUMBER_2
    { 0.375f, 0.125f }, // ATLAS_TILE_NUMBER_3
    { 0.500f, 0.125f }, // ATLAS_TILE_NUMBER_4
    { 0.625f, 0.125f }, // ATLAS_TILE_NUMBER_5
    { 0.750f, 0.125f }, // ATLAS_TILE_NUMBER_6
    { 0.875f, 0.125f }, // ATLAS_TILE_NUMBER_7
    // SVG row 7
    { 0.000f, 0.000f }, // ATLAS_TILE_NUMBER_8
    { 0.125f, 0.000f }, // ATLAS_TILE_NUMBER_9
    { 0.250f, 0.000f }, // ATLAS_TILE_NUMBER_DOT
    { 0.375f, 0.000f }, // ATLAS_TILE_SYSTEM_IDLE
    { 0.500f, 0.000f }, // ATLAS_TILE_SYSTEM_STORE
    { 0.625f, 0.000f }, // ATLAS_TILE_SYSTEM_LOAD
    { 0.750f, 0.000f }, // ATLAS_TILE_SYSTEM_UPDATE
    { 0.875f, 0.000f }  // ATLAS_TILE_SYSTEM_LOG
};

enum OverlayScreenCoordinates
{
    MENU_COORD_MODE_0,
    MENU_COORD_DETAIL_0,        // literally NOT NEEDED but seemingly importent that you retard can understand my model!
    MENU_COORD_DETAIL_1,        // 
    MENU_COORD_MODE_1,
    MENU_COORD_DETAIL_2,
    MENU_COORD_DETAIL_3,        
    MENU_COORD_MODE_2,
    MENU_COORD_DETAIL_4,
    MENU_COORD_DETAIL_5,        
    MENU_COORD_MODE_3,
    MENU_COORD_DETAIL_6,
    MENU_COORD_DETAIL_7,        

    MENU_COORD_TARGET_TIME,
    MENU_COORD_TARGET_TEXTURE,
    MENU_COORD_TARGET_VIDEO,
    MENU_COORD_TARGET_FRAME,

    MENU_COORD_TARGET_PROGRAM,

	MENU_COORD_EXTERN_SELECTOR,

    MENU_COORD_ARROW_UP,
    MENU_COORD_ARROW_DOWN,

    MENU_COORD_BPM_STRING,

    MENU_COORD_BPM_100,
    MENU_COORD_BPM_010,
    MENU_COORD_BPM_001,

    MENU_COORD_BPM_DOT,

    MENU_COORD_BPM_10D,
    MENU_COORD_BPM_01D,

    MENU_COORD_COUNT
};

const int g_menuCoordinates[MENU_COORD_COUNT][2] =
{
    { -128, -128 }, //  0 MENU_COORD_MODE_04			*

    { -128, -128 }, //  1 MENU_COORD_DETAIL_0			**
    { -128, -128 }, //  2 MENU_COORD_DETAIL_1 			**

    {    0, -128 }, //  3 MENU_COORD_MODE_15			*

    {    0, -128 }, //  4 MENU_COORD_DETAIL_2 			**
    {    0, -128 }, //  5 MENU_COORD_DETAIL_3 			**

    { -128,    0 }, //  6 MENU_COORD_MODE_26			*

    { -128,    0 }, //  7 MENU_COORD_DETAIL_4 			**
    { -128,    0 }, //  8 MENU_COORD_DETAIL_5 			**

    {    0,    0 }, //  9 MENU_COORD_MODE_37			*

    {    0,    0 }, // 10 MENU_COORD_DETAIL_6 			**
    {    0,    0 }, // 11 MENU_COORD_DETAIL_7 			**

    {    0,    0 }, // 12 MENU_COORD_TARGET_TIME 		***
    {    0,    0 }, // 13 MENU_COORD_TARGET_TEXTURE		***
    {    0,    0 }, // 14 MENU_COORD_TARGET_VIDEO		***
    {    0,    0 }, // 15 MENU_COORD_TARGET_FRAME		***

    {    0,    0 }, // 16 MENU_COORD_TARGET_PROGRAM		Y

    {    0,    0 }, // 17 MENU_COORD_EXTERN_SELECTOR	X


    { - 64, -256 }, // 18 MENU_COORD_ARROW_UP
    { - 64,  128 }, // 19 MENU_COORD_ARROW_DOWN

    {    0, 160 },  // 20 MENU_COORD_BPM_STRING	****

    {    0, 160 },	// 21 MENU_COORD_BPM_100		****
    {    0, 160 },	// 22 MENU_COORD_BPM_010		****
    {    0, 160 },	// 23 MENU_COORD_BPM_001		****

    {    0, 160 },	// 24 MENU_COORD_BPM_DOT		****

    {    0, 160 },	// 25 MENU_COORD_BPM_10D		****
    {    0, 160 }	// 26 MENU_COORD_BPM_01D		****
};

// * 	= 	the coordinates for the four quadrants ( q0 for mode 0 in layer 1, mode 4 in layer 2, q1 for mode 1 in layer 1, mode 5 in layer 2, etc. )
// **	= 	is usually the same as the coordinates for the quadrant for the mode pair of 0/4, 1/5, 2/6. 3/7 ( depending on the layer 1 or 2)
// ***	= 	is shown depending on block 7/ layer 8 either < 4 ( layer 1 ) or > 3 ( layer 2 )
// **** = 	the "bmp" xyz.xy numeric display, depending on m_BPM_hold_A ( will be shown after changes in bpm ) 
// Y	=	is shown if layer 2 on the fixed position q3 !
// X	= 	same logic as **** taken from g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT]

void            provideTileCoordByLayer( glsl_state* s, int layer )
{
                int local = ( layer - 1) * 4; // i guess i have to convert the layer into to 4 part block in centralModeBuffer
            // GET THE MODE OR VALUE
                Q0 = g_centralModeBuffer[g_gl_program_current][local + 0];  // here i take the values from my g_centralModeBuffer per layer ( four values )
                Q1 = g_centralModeBuffer[g_gl_program_current][local + 1];
                Q2 = g_centralModeBuffer[g_gl_program_current][local + 2];
                Q3 = g_centralModeBuffer[g_gl_program_current][local + 3];

                switch ( layer )
                    {
                    case 1:
                    case 2:
                    s->u_q00_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q0][0];    // here i take the atlas tile coordinates from g_atlasTileMap for layer 1
                    s->u_q00_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q0][1];  

                    s->u_q00_coord = g_menuCoordinates[MENU_COORD_MODE_0][2];
                    s->u_q00_coord = g_menuCoordinates[MENU_COORD_MODE_0][3];
                    switch ( q0 ) // by the mode 
                        {
                        case IN_MODE_ADC: // adc
                        int Va = g_inOutMatrixInt[local + 0][RAW];                         // how i dispatch the mode X correctly here?
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        break;
                        case IN_MODE_TRG: // trg
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L];
                        int Vb = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_H];
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        s->u_d01_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][0];
                        s->u_d01_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][1];
                        break;
                        case IN_MODE_BMP: // bmp
                        int Va = g_inOutMatrixInt[local + 0][RAW];
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][1];                    
                        break;
                        case IN_MODE_LF_0: // lfo 0
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][1];
                        s->u_d01_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][0];
                        s->u_d01_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][1];
                        break;
                        case IN_MODE_LF_1: // lfo 1
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][1];
                        s->u_d01_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][0];
                        s->u_d01_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][1];
                        break;                    
                        case IN_MODE_AU_AL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_A];
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_AH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_B];
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_C];
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_D];
                        s->u_d00_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d00_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case 9: // we ignore the midi modes for now!
                        break;
                        default:
                        break;
                        }
                    s->u_q01_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q1][0];
                    s->u_q01_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q1][1];  

                    s->u_q01_coord = g_menuCoordinates[MENU_COORD_MODE_1][2];
                    s->u_q01_coord = g_menuCoordinates[MENU_COORD_MODE_1][3];
                    switch ( q1 ) // by the mode 
                        {
                        case IN_MODE_ADC: // adc
                        int Va = g_inOutMatrixInt[local + 1][RAW];                         // how i dispatch the mode X correctly here?
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        break;
                        case IN_MODE_TRG: // trg
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L];
                        int Vb = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_H];
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        s->u_d03_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][0];
                        s->u_d03_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][1];
                        break;
                        case IN_MODE_BMP: // bmp
                        int Va = g_inOutMatrixInt[local + 1][RAW];
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][1];                    
                        break;
                        case IN_MODE_LF_0: // lfo 0
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][1];
                        s->u_d03_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][0];
                        s->u_d03_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][1];
                        break;
                        case IN_MODE_LF_1: // lfo 1
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][1];
                        s->u_d03_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][0];
                        s->u_d03_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][1];
                        break;                    
                        case IN_MODE_AU_AL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_A];
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_AH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_B];
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_C];
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_D];
                        s->u_d02_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d02_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case 9: // we ignore the midi modes for now!
                        break;
                        default:
                        break;
                        }
                    s->u_q02_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q2][0];
                    s->u_q02_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q2][1];  

                    s->u_q02_coord = g_menuCoordinates[MENU_COORD_MODE_2][2];
                    s->u_q02_coord = g_menuCoordinates[MENU_COORD_MODE_2][3];
                    switch ( q2 ) // by the mode 
                        {
                        case IN_MODE_ADC: // adc
                        int Va = g_inOutMatrixInt[local + 2][RAW];                         // how i dispatch the mode X correctly here?
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        break;
                        case IN_MODE_TRG: // trg
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L];
                        int Vb = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_H];
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        s->u_d05_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][0];
                        s->u_d05_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][1];
                        break;
                        case IN_MODE_BMP: // bmp
                        int Va = g_inOutMatrixInt[local + 2][RAW];
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][1];                    
                        break;
                        case IN_MODE_LF_0: // lfo 0
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][1];
                        s->u_d05_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][0];
                        s->u_d05_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][1];
                        break;
                        case IN_MODE_LF_1: // lfo 1
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][1];
                        s->u_d05_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][0];
                        s->u_d05_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][1];
                        break;                    
                        case IN_MODE_AU_AL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_A];
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_AH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_B];
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_C];
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_D];
                        s->u_d04_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d04_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case 9: // we ignore the midi modes for now!
                        break;
                        default:
                        break;
                        }
                    s->u_q03_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q3][0];
                    s->u_q03_coord = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q3][1];  

                    s->u_q03_coord = g_menuCoordinates[MENU_COORD_MODE_3][2];
                    s->u_q03_coord = g_menuCoordinates[MENU_COORD_MODE_3][3];
                    switch ( q3 ) // by the mode 
                        {
                        case IN_MODE_ADC: // adc
                        int Va = g_inOutMatrixInt[local + 3][RAW];                         // how i dispatch the mode X correctly here?
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        break;
                        case IN_MODE_TRG: // trg
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L];
                        int Vb = g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_H];
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        s->u_d07_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][0];
                        s->u_d07_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][1];
                        break;
                        case IN_MODE_BMP: // bmp
                        int Va = g_inOutMatrixInt[local + 3][RAW];
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][1];                    
                        break;
                        case IN_MODE_LF_0: // lfo 0
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][1];
                        s->u_d07_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][0];
                        s->u_d07_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][1];
                        break;
                        case IN_MODE_LF_1: // lfo 1
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][1];
                        s->u_d07_coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][0];
                        s->u_d07_coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][1];
                        break;                    
                        case IN_MODE_AU_AL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_A];
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_AH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_B];
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BL:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_C];
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case IN_MODE_AU_BH:
                        int Va = g_centralModeBuffer[g_currentProgramBuffer][SENS_D];
                        s->u_d06_coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        s->u_d06_coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];                    
                        break;
                        case 9: // we ignore the midi modes for now!
                        break;
                        default:
                        break;
                        }                    
                    // now we need to know what and how we do with the detail tiles:
                    case 8:
                    break;
                    default:
                    break;
                    }
}