#include "kernel.h"

    #define MY_BFR   m_logKernel
    #define MY_IDX   m_logKernelIndex
/*
void            CKernel::appendOverlayTile           (   unsigned    atlasTile,
                                                        int         coordinate,
                                                        int         yOffset )
{
                if (g_overlayVertexCount + OVERLAY_VERTEX_PER_TILE > OVERLAY_TILE_COUNT * OVERLAY_VERTEX_PER_TILE)
                    {
                    return;
                    }

                const GLfloat width  = static_cast<GLfloat>(m_ogl.screen_width);
                const GLfloat height = static_cast<GLfloat>(m_ogl.screen_height);

                const GLfloat pixelX0 = width  * 0.5f + static_cast<GLfloat>(g_menuCoordinates[coordinate][0]);
                const GLfloat pixelY0 = height * 0.5f - static_cast<GLfloat>(g_menuCoordinates[coordinate][1] + yOffset) - 128.0f;
                const GLfloat pixelX1 = pixelX0 + 128.0f;
                const GLfloat pixelY1 = pixelY0 + 128.0f;

                const GLfloat x0 = pixelX0 * 2.0f / width  - 1.0f;
                const GLfloat y0 = pixelY0 * 2.0f / height - 1.0f;
                const GLfloat x1 = pixelX1 * 2.0f / width  - 1.0f;
                const GLfloat y1 = pixelY1 * 2.0f / height - 1.0f;

                const GLfloat u0 = g_atlasTileMap[atlasTile][0];
                const GLfloat v0 = g_atlasTileMap[atlasTile][1];
                const GLfloat u1 = u0 + 0.125f;
                const GLfloat v1 = v0 + 0.125f;

                GLfloat* out = &g_overlayVertices[g_overlayVertexCount * OVERLAY_FLOAT_PER_VERTEX];

                out[0]  = x0; out[1]  = y0; out[2]  = u0; out[3]  = v0;
                out[4]  = x1; out[5]  = y0; out[6]  = u1; out[7]  = v0;
                out[8]  = x1; out[9]  = y1; out[10] = u1; out[11] = v1;

                out[12] = x0; out[13] = y0; out[14] = u0; out[15] = v0;
                out[16] = x1; out[17] = y1; out[18] = u1; out[19] = v1;
                out[20] = x0; out[21] = y1; out[22] = u0; out[23] = v1;

                g_overlayVertexCount += OVERLAY_VERTEX_PER_TILE;
}
*/
void            CKernel::appendOverlayTile           (   unsigned    atlasTile,
                                                        int         coordinate,
                                                        int         yOffset )
{
                if (g_overlayVertexCount + OVERLAY_VERTEX_PER_TILE > OVERLAY_TILE_COUNT * OVERLAY_VERTEX_PER_TILE)
                    {
                    return;
                    }

                const GLfloat width  = static_cast<GLfloat>(m_ogl.screen_width);
                const GLfloat height = static_cast<GLfloat>(m_ogl.screen_height);

                const GLfloat pixelX0 = width  * 0.5f + static_cast<GLfloat>(g_menuCoordinates[coordinate][0]);
                const GLfloat pixelY0 = height * 0.5f - (static_cast<GLfloat>(g_menuCoordinates[coordinate][1]) + static_cast<GLfloat>(yOffset) * 0.5f) - 64.0f;
                const GLfloat pixelX1 = pixelX0 + 64.0f;
                const GLfloat pixelY1 = pixelY0 + 64.0f;

                const GLfloat x0 = pixelX0 * 2.0f / width  - 1.0f;
                const GLfloat y0 = pixelY0 * 2.0f / height - 1.0f;
                const GLfloat x1 = pixelX1 * 2.0f / width  - 1.0f;
                const GLfloat y1 = pixelY1 * 2.0f / height - 1.0f;

                const GLfloat u0 = g_atlasTileMap[atlasTile][0];
                const GLfloat v0 = g_atlasTileMap[atlasTile][1];
                const GLfloat u1 = u0 + 0.125f;
                const GLfloat v1 = v0 + 0.125f;

                GLfloat* out = &g_overlayVertices[g_overlayVertexCount * OVERLAY_FLOAT_PER_VERTEX];

                out[0]  = x0; out[1]  = y0; out[2]  = u0; out[3]  = v0;
                out[4]  = x1; out[5]  = y0; out[6]  = u1; out[7]  = v0;
                out[8]  = x1; out[9]  = y1; out[10] = u1; out[11] = v1;

                out[12] = x0; out[13] = y0; out[14] = u0; out[15] = v0;
                out[16] = x1; out[17] = y1; out[18] = u1; out[19] = v1;
                out[20] = x0; out[21] = y1; out[22] = u0; out[23] = v1;

                g_overlayVertexCount += OVERLAY_VERTEX_PER_TILE;
}

void            CKernel::provideTileCoordForMode     (   unsigned    mode,
                                                        unsigned    matrixIndex,
                                                        int         modeCoord,
                                                        int         detail0Coord,
                                                        int         detail1Coord )
{
                appendOverlayTile(ATLAS_TILE_MODE_ADC + mode, modeCoord);

                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] == matrixIndex)
                    {
                    appendOverlayTile(ATLAS_TILE_TARGET_TIME, modeCoord);
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] == matrixIndex)
                    {
                    appendOverlayTile(ATLAS_TILE_TARGET_TEXTURE, modeCoord);
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_VID] == matrixIndex)
                    {
                    appendOverlayTile(ATLAS_TILE_TARGET_VIDEO, modeCoord);
                    }
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM] == matrixIndex)
                    {
                    appendOverlayTile(ATLAS_TILE_TARGET_FRAME, modeCoord);
                    }

                switch (mode)
                    {
                    case IN_MODE_ADC:
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_R, detail0Coord, -(g_inOutMatrixInt[matrixIndex][RAW] >> 5));
                        break;

                    case IN_MODE_TRG:
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_L, detail0Coord, -(g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_L] >> 4));
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_R, detail1Coord, -(g_centralModeBuffer[g_currentProgramBuffer][THRESHOLD_H] >> 4));
                        break;

                    case IN_MODE_BMP:
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_BPM, detail0Coord, -(g_inOutMatrixInt[matrixIndex][RAW] >> 5));
                        break;

                    case IN_MODE_LF_0:
                        appendOverlayTile(ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE], detail0Coord);
                        appendOverlayTile(ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT], detail1Coord);
                        break;

                    case IN_MODE_LF_1:
                        appendOverlayTile(ATLAS_TILE_LFO_WAVE_SINE + g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE], detail0Coord);
                        appendOverlayTile(ATLAS_TILE_DIVIDER_1_64 + g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT], detail1Coord);
                        break;

                    case IN_MODE_AU_AL:
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_R, detail0Coord, -(g_centralModeBuffer[g_currentProgramBuffer][SENS_A] >> 1));
                        break;

                    case IN_MODE_AU_AH:
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_R, detail0Coord, -(g_centralModeBuffer[g_currentProgramBuffer][SENS_B] >> 1));
                        break;

                    case IN_MODE_AU_BL:
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_R, detail0Coord, -(g_centralModeBuffer[g_currentProgramBuffer][SENS_C] >> 1));
                        break;

                    case IN_MODE_AU_BH:
                        appendOverlayTile(ATLAS_TILE_INDICATOR_BAR_R, detail0Coord, -(g_centralModeBuffer[g_currentProgramBuffer][SENS_D] >> 1));
                        break;

                    default:
                        break;
                    }
}

void            CKernel::provideTileCoordByLayer     (   int layer )
{
                g_overlayVertexCount = 0;

                const unsigned local = layer > 0 ? (layer - 1) * 4 : 0;

                switch (layer)
                    {
                    case 1:
                    case 2:
                        provideTileCoordForMode(g_centralModeBuffer[g_currentProgramBuffer][local + 0], local + 0, MENU_COORD_MODE_0, MENU_COORD_DETAIL_0, MENU_COORD_DETAIL_1);
                        provideTileCoordForMode(g_centralModeBuffer[g_currentProgramBuffer][local + 1], local + 1, MENU_COORD_MODE_1, MENU_COORD_DETAIL_2, MENU_COORD_DETAIL_3);
                        provideTileCoordForMode(g_centralModeBuffer[g_currentProgramBuffer][local + 2], local + 2, MENU_COORD_MODE_2, MENU_COORD_DETAIL_4, MENU_COORD_DETAIL_5);
                        provideTileCoordForMode(g_centralModeBuffer[g_currentProgramBuffer][local + 3], local + 3, MENU_COORD_MODE_3, MENU_COORD_DETAIL_6, MENU_COORD_DETAIL_7);
                        break;

                    case 8:
                        appendOverlayTile(ATLAS_TILE_TARGET_TIME,    MENU_COORD_MODE_0);
                        appendOverlayTile(ATLAS_TILE_TARGET_TEXTURE, MENU_COORD_MODE_1);
                        appendOverlayTile(ATLAS_TILE_TARGET_VIDEO,   MENU_COORD_MODE_2);
                        appendOverlayTile(ATLAS_TILE_TARGET_FRAME,   MENU_COORD_MODE_3);
                        break;

                    default:
                        break;
                    }

                if (m_BPM_hold_A)
                    {
                    const unsigned bpm      = g_lfoBpmMatrix[g_activeBpmChannel][BPM];
                    const unsigned digit100 = (bpm / 10000) % 10;
                    const unsigned digit010 = (bpm / 1000)  % 10;
                    const unsigned digit001 = (bpm / 100)   % 10;
                    const unsigned digit10d = (bpm / 10)    % 10;
                    const unsigned digit01d =  bpm          % 10;

                    appendOverlayTile(ATLAS_TILE_LABEL_BPM,                 MENU_COORD_BPM_STRING);
                    appendOverlayTile(ATLAS_TILE_NUMBER_0 + digit100,       MENU_COORD_BPM_100);
                    appendOverlayTile(ATLAS_TILE_NUMBER_0 + digit010,       MENU_COORD_BPM_010);
                    appendOverlayTile(ATLAS_TILE_NUMBER_0 + digit001,       MENU_COORD_BPM_001);
                    appendOverlayTile(ATLAS_TILE_NUMBER_DOT,                MENU_COORD_BPM_DOT);
                    appendOverlayTile(ATLAS_TILE_NUMBER_0 + digit10d,       MENU_COORD_BPM_10D);
                    appendOverlayTile(ATLAS_TILE_NUMBER_0 + digit01d,       MENU_COORD_BPM_01D);
                    }
}