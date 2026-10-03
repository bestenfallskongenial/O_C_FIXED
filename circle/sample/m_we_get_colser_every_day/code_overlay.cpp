#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_logKernelIndex 

void            CKernel::provideTileCoordForMode(   unsigned    mode,
                                                    unsigned    matrixIndex,
                                                //  int     local,
                                                    float*      qCoord,
                                                    float*      d0Coord,
                                                    float*      d1Coord,
                                                    float*      t00Coord,
                                                    float*      t01Coord,
                                                    float*      t02Coord,
                                                    float*      t03Coord,
                                                    int         modeCoord,
                                                    int         detail0Coord,
                                                    int         detail1Coord )
{
                const unsigned local = (matrixIndex / 4) * 4;

                qCoord[0] = g_atlasTileMap[ATLAS_TILE_MODE_ADC + mode][0];
                qCoord[1] = g_atlasTileMap[ATLAS_TILE_MODE_ADC + mode][1];
                qCoord[2] = g_menuCoordinates[modeCoord][0];
                qCoord[3] = g_menuCoordinates[modeCoord][1];

                // target mode
                if      ( g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] == matrixIndex)
                            {
                            t00Coord[0] = g_atlasTileMap[ATLAS_TILE_TARGET_TIME][0];
                            t00Coord[1] = g_atlasTileMap[ATLAS_TILE_TARGET_TIME][1];
                            t00Coord[2] = g_menuCoordinates[modeCoord][0];
                            t00Coord[3] = g_menuCoordinates[modeCoord][1];
                            }
                else if ( g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] < local || g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] > local + 3)
                            {
                            t00Coord[0] = g_atlasTileMap[ATLAS_TILE_BLANK][0];
                            t00Coord[1] = g_atlasTileMap[ATLAS_TILE_BLANK][1];
                            }
                if      ( g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] == matrixIndex)
                            {
                            t01Coord[0] = g_atlasTileMap[ATLAS_TILE_TARGET_TEXTURE][0];
                            t01Coord[1] = g_atlasTileMap[ATLAS_TILE_TARGET_TEXTURE][1];
                            t01Coord[2] = g_menuCoordinates[modeCoord][0];
                            t01Coord[3] = g_menuCoordinates[modeCoord][1];
                            }
                else if ( g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] < local || g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] > local + 3)
                            {
                            t01Coord[0] = g_atlasTileMap[ATLAS_TILE_BLANK][0];
                            t01Coord[1] = g_atlasTileMap[ATLAS_TILE_BLANK][1];
                            }
                if      ( g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] == matrixIndex)
                            {
                            t02Coord[0] = g_atlasTileMap[ATLAS_TILE_TARGET_VIDEO][0];
                            t02Coord[1] = g_atlasTileMap[ATLAS_TILE_TARGET_VIDEO][1];
                            t02Coord[2] = g_menuCoordinates[modeCoord][0];
                            t02Coord[3] = g_menuCoordinates[modeCoord][1];
                            }
                else if ( g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] < local || g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] > local + 3)
                            {
                            t02Coord[0] = g_atlasTileMap[ATLAS_TILE_BLANK][0];
                            t02Coord[1] = g_atlasTileMap[ATLAS_TILE_BLANK][1];
                            }
                if      ( g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM] == matrixIndex)
                            {
                            t03Coord[0] = g_atlasTileMap[ATLAS_TILE_TARGET_FRAME][0];
                            t03Coord[1] = g_atlasTileMap[ATLAS_TILE_TARGET_FRAME][1];
                            t03Coord[2] = g_menuCoordinates[modeCoord][0];
                            t03Coord[3] = g_menuCoordinates[modeCoord][1];
                            }
                else if ( g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM] < local || g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM] > local + 3)
                            {
                            t03Coord[0] = g_atlasTileMap[ATLAS_TILE_BLANK][0];
                            t03Coord[1] = g_atlasTileMap[ATLAS_TILE_BLANK][1];
                            }
                // modes    
                switch (mode)
                    {
                    case IN_MODE_ADC:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1] - (g_inOutMatrixInt[matrixIndex][RAW] >> 5);
                        break;
                    case IN_MODE_TRG:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_L][1];
                        d1Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        d1Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1] - (g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L] >> 4);
                        d1Coord[2] = g_menuCoordinates[detail1Coord][0];
                        d1Coord[3] = g_menuCoordinates[detail1Coord][1] - (g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_H] >> 4);
                        break;
                    case IN_MODE_BMP:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_BPM][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1] - (g_inOutMatrixInt[matrixIndex][RAW] >> 5);
                        break;
                    case IN_MODE_LF_0:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1];
                        d1Coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][0];
                        d1Coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]][1];
                        d1Coord[2] = g_menuCoordinates[detail1Coord][0];
                        d1Coord[3] = g_menuCoordinates[detail1Coord][1];
                        break;
                    case IN_MODE_LF_1:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1];
                        d1Coord[0] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][0];
                        d1Coord[1] = g_atlasTileMap[ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]][1];
                        d1Coord[2] = g_menuCoordinates[detail1Coord][0];
                        d1Coord[3] = g_menuCoordinates[detail1Coord][1];
                        break;
                    case IN_MODE_AU_AL:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1] - (g_centralModeBuffer[g_currentProgramBuffer][SENS_A] >> 1);
                        break;
                    case IN_MODE_AU_AH:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1] - (g_centralModeBuffer[g_currentProgramBuffer][SENS_B] >> 1);
                        break;
                    case IN_MODE_AU_BL:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1] - (g_centralModeBuffer[g_currentProgramBuffer][SENS_C] >> 1);
                        break;
                    case IN_MODE_AU_BH:
                        d0Coord[0] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][0];
                        d0Coord[1] = g_atlasTileMap[ATLAS_TILE_INDICATOR_BAR_R][1];
                        d0Coord[2] = g_menuCoordinates[detail0Coord][0];
                        d0Coord[3] = g_menuCoordinates[detail0Coord][1] - (g_centralModeBuffer[g_currentProgramBuffer][SENS_D] >> 1);
                        break;
                    case 9:
                        break;
                    default:
                        break;
                    }
}

void            CKernel::provideTileCoordByLayer    (   glsl_state* s, 
                                                        int         layer)
{
                memset(g_menuTarget, 0, sizeof(g_menuTarget));

                unsigned local = (layer - 1) * 4;
                // quadrants
                switch (layer)
                    {
                    case 1:
                    case 2:
                        provideTileCoordForMode(    g_centralModeBuffer[g_gl_program_current][local + 0],
                                                    local + 0,
                                                    g_menuTarget[MENU_COORD_MODE_0],
                                                    g_menuTarget[MENU_COORD_DETAIL_0],
                                                    g_menuTarget[MENU_COORD_DETAIL_1],
                                                    g_menuTarget[MENU_COORD_TARGET_TIME],
                                                    g_menuTarget[MENU_COORD_TARGET_TEXTURE],
                                                    g_menuTarget[MENU_COORD_TARGET_VIDEO],
                                                    g_menuTarget[MENU_COORD_TARGET_FRAME],
                                                    MENU_COORD_MODE_0,
                                                    MENU_COORD_DETAIL_0,
                                                    MENU_COORD_DETAIL_1);
                        provideTileCoordForMode(    g_centralModeBuffer[g_gl_program_current][local + 1],
                                                    local + 1,
                                                    g_menuTarget[MENU_COORD_MODE_1],
                                                    g_menuTarget[MENU_COORD_DETAIL_2],
                                                    g_menuTarget[MENU_COORD_DETAIL_3],
                                                    g_menuTarget[MENU_COORD_TARGET_TIME],
                                                    g_menuTarget[MENU_COORD_TARGET_TEXTURE],
                                                    g_menuTarget[MENU_COORD_TARGET_VIDEO],
                                                    g_menuTarget[MENU_COORD_TARGET_FRAME],
                                                    MENU_COORD_MODE_1,
                                                    MENU_COORD_DETAIL_2,
                                                    MENU_COORD_DETAIL_3);
                        provideTileCoordForMode(    g_centralModeBuffer[g_gl_program_current][local + 2],
                                                    local + 2,
                                                    g_menuTarget[MENU_COORD_MODE_2],
                                                    g_menuTarget[MENU_COORD_DETAIL_4],
                                                    g_menuTarget[MENU_COORD_DETAIL_5],
                                                    g_menuTarget[MENU_COORD_TARGET_TIME],
                                                    g_menuTarget[MENU_COORD_TARGET_TEXTURE],
                                                    g_menuTarget[MENU_COORD_TARGET_VIDEO],
                                                    g_menuTarget[MENU_COORD_TARGET_FRAME],
                                                    MENU_COORD_MODE_2,
                                                    MENU_COORD_DETAIL_4,
                                                    MENU_COORD_DETAIL_5);
                        provideTileCoordForMode(    g_centralModeBuffer[g_gl_program_current][local + 3],
                                                    local + 3,
                                                    g_menuTarget[MENU_COORD_MODE_3],
                                                    g_menuTarget[MENU_COORD_DETAIL_6],
                                                    g_menuTarget[MENU_COORD_DETAIL_7],
                                                    g_menuTarget[MENU_COORD_TARGET_TIME],
                                                    g_menuTarget[MENU_COORD_TARGET_TEXTURE],
                                                    g_menuTarget[MENU_COORD_TARGET_VIDEO],
                                                    g_menuTarget[MENU_COORD_TARGET_FRAME],
                                                    MENU_COORD_MODE_3,
                                                    MENU_COORD_DETAIL_6,
                                                    MENU_COORD_DETAIL_7);
                            break;
                    // system modes            
                    case 8:
                        g_menuTarget[MENU_COORD_MODE_0][0] = g_atlasTileMap[ATLAS_TILE_TARGET_TIME][0];
                        g_menuTarget[MENU_COORD_MODE_0][1] = g_atlasTileMap[ATLAS_TILE_TARGET_TIME][1];
                        g_menuTarget[MENU_COORD_MODE_0][2] = g_menuCoordinates[MENU_COORD_MODE_0][0];
                        g_menuTarget[MENU_COORD_MODE_0][3] = g_menuCoordinates[MENU_COORD_MODE_0][1];

                        g_menuTarget[MENU_COORD_MODE_1][0] = g_atlasTileMap[ATLAS_TILE_TARGET_TEXTURE][0];
                        g_menuTarget[MENU_COORD_MODE_1][1] = g_atlasTileMap[ATLAS_TILE_TARGET_TEXTURE][1];
                        g_menuTarget[MENU_COORD_MODE_1][2] = g_menuCoordinates[MENU_COORD_MODE_1][0];
                        g_menuTarget[MENU_COORD_MODE_1][3] = g_menuCoordinates[MENU_COORD_MODE_1][1];

                        g_menuTarget[MENU_COORD_MODE_2][0] = g_atlasTileMap[ATLAS_TILE_TARGET_VIDEO][0];
                        g_menuTarget[MENU_COORD_MODE_2][1] = g_atlasTileMap[ATLAS_TILE_TARGET_VIDEO][1];
                        g_menuTarget[MENU_COORD_MODE_2][2] = g_menuCoordinates[MENU_COORD_MODE_2][0];
                        g_menuTarget[MENU_COORD_MODE_2][3] = g_menuCoordinates[MENU_COORD_MODE_2][1];

                        g_menuTarget[MENU_COORD_MODE_3][0] = g_atlasTileMap[ATLAS_TILE_TARGET_FRAME][0];
                        g_menuTarget[MENU_COORD_MODE_3][1] = g_atlasTileMap[ATLAS_TILE_TARGET_FRAME][1];
                        g_menuTarget[MENU_COORD_MODE_3][2] = g_menuCoordinates[MENU_COORD_MODE_3][0];
                        g_menuTarget[MENU_COORD_MODE_3][3] = g_menuCoordinates[MENU_COORD_MODE_3][1];
                        break;
                    default:
                        break;
                    }
                if (m_BPM_hold_A)
                    {
                    unsigned bpm = g_lfoBpmMatrix[g_activeBpmChannel][BPM];

                    unsigned digit100 = (bpm / 10000) % 10;
                    unsigned digit010 = (bpm / 1000)  % 10;
                    unsigned digit001 = (bpm / 100)   % 10;
                    unsigned digit10d = (bpm / 10)    % 10;
                    unsigned digit01d =  bpm          % 10;

                    g_menuTarget[MENU_COORD_BPM_STRING][0] = g_atlasTileMap[ATLAS_TILE_LABEL_BPM][0];
                    g_menuTarget[MENU_COORD_BPM_STRING][1] = g_atlasTileMap[ATLAS_TILE_LABEL_BPM][1];
                    g_menuTarget[MENU_COORD_BPM_STRING][2] = g_menuCoordinates[MENU_COORD_BPM_STRING][0];
                    g_menuTarget[MENU_COORD_BPM_STRING][3] = g_menuCoordinates[MENU_COORD_BPM_STRING][1];

                    g_menuTarget[MENU_COORD_BPM_100][0] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit100][0];
                    g_menuTarget[MENU_COORD_BPM_100][1] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit100][1];
                    g_menuTarget[MENU_COORD_BPM_100][2] = g_menuCoordinates[MENU_COORD_BPM_100][0];
                    g_menuTarget[MENU_COORD_BPM_100][3] = g_menuCoordinates[MENU_COORD_BPM_100][1];

                    g_menuTarget[MENU_COORD_BPM_010][0] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit010][0];
                    g_menuTarget[MENU_COORD_BPM_010][1] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit010][1];
                    g_menuTarget[MENU_COORD_BPM_010][2] = g_menuCoordinates[MENU_COORD_BPM_010][0];
                    g_menuTarget[MENU_COORD_BPM_010][3] = g_menuCoordinates[MENU_COORD_BPM_010][1];

                    g_menuTarget[MENU_COORD_BPM_001][0] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit001][0];
                    g_menuTarget[MENU_COORD_BPM_001][1] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit001][1];
                    g_menuTarget[MENU_COORD_BPM_001][2] = g_menuCoordinates[MENU_COORD_BPM_001][0];
                    g_menuTarget[MENU_COORD_BPM_001][3] = g_menuCoordinates[MENU_COORD_BPM_001][1];

                    g_menuTarget[MENU_COORD_BPM_DOT][0] = g_atlasTileMap[ATLAS_TILE_NUMBER_DOT][0];
                    g_menuTarget[MENU_COORD_BPM_DOT][1] = g_atlasTileMap[ATLAS_TILE_NUMBER_DOT][1];
                    g_menuTarget[MENU_COORD_BPM_DOT][2] = g_menuCoordinates[MENU_COORD_BPM_DOT][0];
                    g_menuTarget[MENU_COORD_BPM_DOT][3] = g_menuCoordinates[MENU_COORD_BPM_DOT][1];

                    g_menuTarget[MENU_COORD_BPM_10D][0] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit10d][0];
                    g_menuTarget[MENU_COORD_BPM_10D][1] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit10d][1];
                    g_menuTarget[MENU_COORD_BPM_10D][2] = g_menuCoordinates[MENU_COORD_BPM_10D][0];
                    g_menuTarget[MENU_COORD_BPM_10D][3] = g_menuCoordinates[MENU_COORD_BPM_10D][1];

                    g_menuTarget[MENU_COORD_BPM_01D][0] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit01d][0];
                    g_menuTarget[MENU_COORD_BPM_01D][1] = g_atlasTileMap[ATLAS_TILE_NUMBER_0 + digit01d][1];
                    g_menuTarget[MENU_COORD_BPM_01D][2] = g_menuCoordinates[MENU_COORD_BPM_01D][0];
                    g_menuTarget[MENU_COORD_BPM_01D][3] = g_menuCoordinates[MENU_COORD_BPM_01D][1];
                    }                    
}