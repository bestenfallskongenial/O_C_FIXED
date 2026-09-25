// FIXED - DETERMINISTIC APPROACH TO REALIZE THE OVERLAY MENU

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
    ATLAS_TILE_INDICATOR_BAR_L,     // 40   // used for ATLAS_TILE_MODE_TRG ( THRESHOLD_L ), ATLAS_TILE_MODE_AU_XL ( SENS_A / SENS_C )
    ATLAS_TILE_INDICATOR_BAR_R,     // 41   // used for ATLAS_TILE_MODE_ADC ( g_inOutMatrixInt[0][RAW] ), ATLAS_TILE_MODE_TRG ( THRESHOLD_H ), ATLAS_TILE_MODE_AU_XH ( SENS_B / SENS_D )
    ATLAS_TILE_42,                  // 42   // 
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

enum OverlayScreenCoordinates
{
    MENU_COORD_MODE_0,
    MENU_COORD_MODE_1,
    MENU_COORD_MODE_2,
    MENU_COORD_MODE_3,

    MENU_COORD_TARGET_TIME,
    MENU_COORD_TARGET_TEXTURE,
    MENU_COORD_TARGET_VIDEO,
    MENU_COORD_TARGET_FRAME,

    MENU_COORD_TARGET_PROGRAM,

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
    { 0.250f, 0.250f }, // ATLAS_TILE_42
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

const int g_menuCoordinates[MENU_COORD_COUNT][2] =
{
    { -128, -128 }, // MENU_COORD_MODE_0
    {    0, -128 }, // MENU_COORD_MODE_1
    { -128,    0 }, // MENU_COORD_MODE_2
    {    0,    0 }, // MENU_COORD_MODE_3

    { - 64, -256 }, // MENU_COORD_ARROW_UP
    { - 64,  128 }, // MENU_COORD_ARROW_DOWN

    { 0, 0 },       // MENU_COORD_BPM_STRING

    { 0, 0 },       // MENU_COORD_BPM_100
    { 0, 0 },       // MENU_COORD_BPM_010
    { 0, 0 },       // MENU_COORD_BPM_001

    { 0, 0 },       // MENU_COORD_BPM_DOT

    { 0, 0 },       // MENU_COORD_BPM_10D
    { 0, 0 }        // MENU_COORD_BPM_01D
};

// EXAMPLE CODE! IN WORK / PROGRESS !!!

void            CKernel::Overlay_dispatcher()
{
                switch(g_menuLayer)
                    {
                    case 0:
                    // nothing, show bpm will be handled separately
                    break;

                    case 1:
                    // get the tile index:

                    Q0 = g_centralModeBuffer[g_gl_program_current][MODE_CH0 + 0];
                    Q1 = g_centralModeBuffer[g_gl_program_current][MODE_CH0 + 1];
                    Q2 = g_centralModeBuffer[g_gl_program_current][MODE_CH0 + 2];
                    Q3 = g_centralModeBuffer[g_gl_program_current][MODE_CH0 + 3];

                    // get the tile coordinates:

                    T0x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q0][0];
                    T0y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q0][1];

                    T1x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q1][0];
                    T1y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q1][1];

                    T2x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q2][0];
                    T2y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q2][1];

                    T3x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q3][0];
                    T3y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q3][1];

                    // get on-screen overlay coordinates:

                    O0x = g_menuCoordinates[MENU_COORD_MODE_0][0];
                    O0y = g_menuCoordinates[MENU_COORD_MODE_0][1];

                    O1x = g_menuCoordinates[MENU_COORD_MODE_1][0];
                    O1y = g_menuCoordinates[MENU_COORD_MODE_1][1];

                    O2x = g_menuCoordinates[MENU_COORD_MODE_2][0];
                    O2y = g_menuCoordinates[MENU_COORD_MODE_2][1];

                    O3x = g_menuCoordinates[MENU_COORD_MODE_3][0];
                    O3y = g_menuCoordinates[MENU_COORD_MODE_3][1];

                    runTheModeDispatcher();
                    break;

                    case 2:
                    // mode 04 to 07 - depending on the button either this or layer 1 is shown

                    Q0 = g_centralModeBuffer[g_gl_program_current][MODE_CH4 + 0];
                    Q1 = g_centralModeBuffer[g_gl_program_current][MODE_CH4 + 1];
                    Q2 = g_centralModeBuffer[g_gl_program_current][MODE_CH4 + 2];
                    Q3 = g_centralModeBuffer[g_gl_program_current][MODE_CH4 + 3];

                    // get the tile coordinates:

                    T0x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q0][0];
                    T0y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q0][1];

                    T1x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q1][0];
                    T1y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q1][1];

                    T2x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q2][0];
                    T2y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q2][1];

                    T3x = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q3][0];
                    T3y = g_atlasTileMap[ATLAS_TILE_MODE_ADC + Q3][1];

                    // get on-screen overlay coordinates:

                    O0x = g_menuCoordinates[MENU_COORD_MODE_0][0];
                    O0y = g_menuCoordinates[MENU_COORD_MODE_0][1];

                    O1x = g_menuCoordinates[MENU_COORD_MODE_1][0];
                    O1y = g_menuCoordinates[MENU_COORD_MODE_1][1];

                    O2x = g_menuCoordinates[MENU_COORD_MODE_2][0];
                    O2y = g_menuCoordinates[MENU_COORD_MODE_2][1];

                    O3x = g_menuCoordinates[MENU_COORD_MODE_3][0];
                    O3y = g_menuCoordinates[MENU_COORD_MODE_3][1];

                    runTheModeDispatcher();
                    break;

                    case 3:
                    // needs no special overlay, this settings will be displayed
                    // with layer 1/2 if the mode is selected somewhere
                    break;

                    case 4:
                    // needs no special overlay, this settings will be displayed
                    // with layer 1/2 if the mode is selected somewhere
                    break;

                    case 5:
                    // needs no special overlay, this settings will be displayed
                    // with layer 1/2 if the mode is selected somewhere
                    break;

                    case 6:
                    // needs no special overlay, this settings will be displayed
                    // with layer 1/2 if the mode is selected somewhere
                    break;

                    case 7:
                    // SEL_X determines where the tile is shown
                    break;

                    case 8:
                    // same as above but the layer8Dispatcher() is different

                    Q0 = g_centralModeBuffer[g_gl_program_current][SET_STORE];
                    Q1 = g_centralModeBuffer[g_gl_program_current][SET_LOAD];
                    Q2 = g_centralModeBuffer[g_gl_program_current][KLN_LOAD];
                    Q3 = g_centralModeBuffer[g_gl_program_current][LOG_STORE];

                    // get the tile coordinates:

                    T0x = g_atlasTileMap[ATLAS_TILE_SYSTEM_STORE][0];
                    T0y = g_atlasTileMap[ATLAS_TILE_SYSTEM_STORE][1];

                    T1x = g_atlasTileMap[ATLAS_TILE_SYSTEM_LOAD][0];
                    T1y = g_atlasTileMap[ATLAS_TILE_SYSTEM_LOAD][1];

                    T2x = g_atlasTileMap[ATLAS_TILE_SYSTEM_UPDATE][0];
                    T2y = g_atlasTileMap[ATLAS_TILE_SYSTEM_UPDATE][1];

                    T3x = g_atlasTileMap[ATLAS_TILE_SYSTEM_LOG][0];
                    T3y = g_atlasTileMap[ATLAS_TILE_SYSTEM_LOG][1];

                    // get on-screen overlay coordinates:                    

                    O0x = g_menuCoordinates[MENU_COORD_MODE_0][0];
                    O0y = g_menuCoordinates[MENU_COORD_MODE_0][1];

                    O1x = g_menuCoordinates[MENU_COORD_MODE_1][0];
                    O1y = g_menuCoordinates[MENU_COORD_MODE_1][1];

                    O2x = g_menuCoordinates[MENU_COORD_MODE_2][0];
                    O2y = g_menuCoordinates[MENU_COORD_MODE_2][1];

                    O3x = g_menuCoordinates[MENU_COORD_MODE_3][0];
                    O3y = g_menuCoordinates[MENU_COORD_MODE_3][1];

                    runTheSystemDispatcher( /* WHAT DO I NEED TO PASS HERE ?? */ );

                    break;

                    default:
                    break;
                    }

                // show bpm - we use the same timeout mechanism
                // as in m_audio_hold_A in code_audio.cpp
}

void CKernel::runTheModeDispatcher( /* WHAT DO I NEED TO PASS HERE ?? */ )
{

}


void CKernel::Overlay_ADC ( unsigned channel,
                            unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_ADC;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_TRG ( unsigned channel,
                            unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_TRG;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_BMP ( unsigned channel,
                            unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_BMP;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_LF0 ( unsigned channel,
                            unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_LF0;

    m_osh.overlayTile[coordinate][1] =
        ATLAS_TILE_LFO_WAVE_SINE +
        g_centralModeBuffer
            [g_currentProgramBuffer]
            [LF1_WAVE];

    m_osh.overlayTile[coordinate][2] =
        ATLAS_TILE_DIVIDER_1_64 +
        g_centralModeBuffer
            [g_currentProgramBuffer]
            [LF1_MULT];

    m_osh.overlayTileCount[coordinate] =
        3;
}
void CKernel::Overlay_LF1 ( unsigned channel,
                            unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_LF1;

    m_osh.overlayTile[coordinate][1] =
        ATLAS_TILE_LFO_WAVE_SINE +
        g_centralModeBuffer
            [g_currentProgramBuffer]
            [LF2_WAVE];

    m_osh.overlayTile[coordinate][2] =
        ATLAS_TILE_DIVIDER_1_64 +
        g_centralModeBuffer
            [g_currentProgramBuffer]
            [LF2_MULT];

    m_osh.overlayTileCount[coordinate] =
        3;
}
void CKernel::Overlay_AU_AL (   unsigned channel,
                                unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_AU_AL;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_AU_AH (   unsigned channel,
                                unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_AU_AH;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_AU_BL (   unsigned channel,
                                unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_AU_BL;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_AU_BH (   unsigned channel,
                                unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_AU_BH;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_MIDI_NOTE (   unsigned channel,
                                    unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_MIDI_NOTE;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_MIDI_CC0 (    unsigned channel,
                                    unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_MIDI_CC0;

    m_osh.overlayTileCount[coordinate] =
        1;
}
void CKernel::Overlay_MIDI_CC1 (    unsigned channel,
                                    unsigned coordinate )
{
    m_osh.overlayTile[coordinate][0] =
        ATLAS_TILE_MODE_MIDI_CC1;

    m_osh.overlayTileCount[coordinate] =
        1;
}

