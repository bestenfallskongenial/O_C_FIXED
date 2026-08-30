#include "kernel.h"

//  #undef  __DEBUG_LOG__
    #define __DEBUG_LOG__

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex    

static const char FromKernel[] = "kernel";

void CKernel::debug()
{
 
                 
}

void            CKernel::TimerHandler                   (           TKernelTimerHandle  hTimer, 
                                                                    void*               pParam, 
                                                                    void*               pContext)
{
                void (*pInvalid)(void) = (void (*)(void)) 0x500000;

                (*pInvalid)();
}

void CKernel::logButtonStatesRuntime(void)
{
    char f_logBuffer[512];
    u32  f_logIndex = 0;

    f_logBuffer[0] = '\0';

    storeLog(   f_logBuffer, f_logIndex,
                "BTN PRESS_START   DOUBLE        RELEASE       SINGLE",
                EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "A  ",
                (u32)g_buttons_states[0][BTN_PRESS_START],
                "  ",
                (u32)g_buttons_states[0][BTN_DOUBLE],
                "  ",
                (u32)g_buttons_states[0][BTN_RELEASE],
                "  ",
                (u32)g_buttons_states[0][BTN_SINGLE] );

    storeLog(   f_logBuffer, f_logIndex,
                "A   HOLD_TICK",
                (u32)g_buttons_states[0][BTN_HOLD_TICK],
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "B  ",
                (u32)g_buttons_states[1][BTN_PRESS_START],
                "  ",
                (u32)g_buttons_states[1][BTN_DOUBLE],
                "  ",
                (u32)g_buttons_states[1][BTN_RELEASE],
                "  ",
                (u32)g_buttons_states[1][BTN_SINGLE] );

    storeLog(   f_logBuffer, f_logIndex,
                "B   HOLD_TICK",
                (u32)g_buttons_states[1][BTN_HOLD_TICK],
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       10,
                       0xFFFFFFFF );
}

void CKernel::logInOutRuntime(void)
{
    char f_logBuffer[512];
    u32  f_logIndex = 0;

    f_logBuffer[0] = '\0';

    storeLog(   f_logBuffer, f_logIndex,
                "0 RAW",
                (u32)g_inOutMatrixInt[0][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[0][VAL],
                "RND",
                (u32)g_inOutMatrixInt[0][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "1 RAW",
                (u32)g_inOutMatrixInt[1][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[1][VAL],
                "RND",
                (u32)g_inOutMatrixInt[1][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "2 RAW",
                (u32)g_inOutMatrixInt[2][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[2][VAL],
                "RND",
                (u32)g_inOutMatrixInt[2][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "3 RAW",
                (u32)g_inOutMatrixInt[3][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[3][VAL],
                "RND",
                (u32)g_inOutMatrixInt[3][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "4 RAW",
                (u32)g_inOutMatrixInt[4][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[4][VAL],
                "RND",
                (u32)g_inOutMatrixInt[4][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "5 RAW",
                (u32)g_inOutMatrixInt[5][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[5][VAL],
                "RND",
                (u32)g_inOutMatrixInt[5][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "6 RAW",
                (u32)g_inOutMatrixInt[6][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[6][VAL],
                "RND",
                (u32)g_inOutMatrixInt[6][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "7 RAW",
                (u32)g_inOutMatrixInt[7][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[7][VAL],
                "RND",
                (u32)g_inOutMatrixInt[7][RND],
                EMPTYSTR, EMPTYLOG );

    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       0,
                       0xFFFFFFFF );
}

#undef __SCROLLING__

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;
                if (MY_INDEX == 0) return;

                u32 drawIndex  = m_logScreenStartIndex;
                u32 scanIndex  = m_logScreenStartIndex;

                unsigned col   = 0;
                unsigned row   = 0;

#ifdef __SCROLLING__
                bufferScreenClear();
#endif

                while (scanIndex < MY_INDEX)
                    {
                    char ch = MY_BUFFER[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < MY_INDEX && MY_BUFFER[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < MY_INDEX)
                                {
                                drawIndex++;
                                }

#ifdef __SCROLLING__
                            bufferScreenClear();

                            bufferScreenDraw(
                                                MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);
#endif

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;

#ifdef __SCROLLING__
                            bufferScreenClear();

                            bufferScreenDraw(
                                                MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);
#endif

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    MY_BUFFER,
                                    drawIndex,
                                    MY_INDEX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                m_logScreenStartIndex = drawIndex;
}
/*
// LAST WORKING FUNCTION 
void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;
                if (MY_INDEX == 0) return;

                u32 drawIndex  = m_logScreenStartIndex;
                u32 scanIndex  = m_logScreenStartIndex;

                unsigned col   = 0;
                unsigned row   = 0;

                bufferScreenClear();

                while (scanIndex < MY_INDEX)
                    {
                    char ch = MY_BUFFER[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < MY_INDEX && MY_BUFFER[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < MY_INDEX)
                                {
                                drawIndex++;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;

                            bufferScreenClear();

                            bufferScreenDraw(
                                                MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    MY_BUFFER,
                                    drawIndex,
                                    MY_INDEX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                m_logScreenStartIndex = drawIndex;
}
*/
/*
void            CKernel::logScreenDeltaUpdate       (   void )
{
                boot_buffer_index_temp_new          = m_logBufferIndex;

                memcpy( runtime_buffer_index_temp_new,  m_bufferLogIndex, sizeof(runtime_buffer_index_temp_new) );

                boot_buffer_index_temp_delta        = boot_buffer_index_temp_new        - boot_buffer_index_temp_old;

                runtime_buffer_index_temp_delta[0]  = runtime_buffer_index_temp_new[0]  - runtime_buffer_index_temp_old[0];
                runtime_buffer_index_temp_delta[1]  = runtime_buffer_index_temp_new[1]  - runtime_buffer_index_temp_old[1];
                runtime_buffer_index_temp_delta[2]  = runtime_buffer_index_temp_new[2]  - runtime_buffer_index_temp_old[2];
                runtime_buffer_index_temp_delta[3]  = runtime_buffer_index_temp_new[3]  - runtime_buffer_index_temp_old[3];
                runtime_buffer_index_temp_delta[4]  = runtime_buffer_index_temp_new[4]  - runtime_buffer_index_temp_old[4];
                runtime_buffer_index_temp_delta[5]  = runtime_buffer_index_temp_new[5]  - runtime_buffer_index_temp_old[5];
                runtime_buffer_index_temp_delta[6]  = runtime_buffer_index_temp_new[6]  - runtime_buffer_index_temp_old[6];
                runtime_buffer_index_temp_delta[7]  = runtime_buffer_index_temp_new[7]  - runtime_buffer_index_temp_old[7];
                runtime_buffer_index_temp_delta[8]  = runtime_buffer_index_temp_new[8]  - runtime_buffer_index_temp_old[8];
                runtime_buffer_index_temp_delta[9]  = runtime_buffer_index_temp_new[9]  - runtime_buffer_index_temp_old[9];
                runtime_buffer_index_temp_delta[10] = runtime_buffer_index_temp_new[10] - runtime_buffer_index_temp_old[10];
                runtime_buffer_index_temp_delta[11] = runtime_buffer_index_temp_new[11] - runtime_buffer_index_temp_old[11];
                runtime_buffer_index_temp_delta[12] = runtime_buffer_index_temp_new[12] - runtime_buffer_index_temp_old[12];
                runtime_buffer_index_temp_delta[13] = runtime_buffer_index_temp_new[13] - runtime_buffer_index_temp_old[13];
                runtime_buffer_index_temp_delta[14] = runtime_buffer_index_temp_new[14] - runtime_buffer_index_temp_old[14];
                runtime_buffer_index_temp_delta[15] = runtime_buffer_index_temp_new[15] - runtime_buffer_index_temp_old[15];

                boot_buffer_index_temp_old          = boot_buffer_index_temp_new;

                memcpy( runtime_buffer_index_temp_old, runtime_buffer_index_temp_new, sizeof(runtime_buffer_index_temp_old) );
}

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;

                logScreenDeltaUpdate();

                if (boot_buffer_index_temp_delta > 0) logScreenBufferUpdate( m_logBuffer, m_logBufferIndex, m_logScreenStartIndexBoot );

                if (m_bufferLog == nullptr) return;

                if (runtime_buffer_index_temp_delta[0]  > 0) logScreenBufferUpdate(m_bufferLog[0],  m_bufferLogIndex[0],  m_logScreenStartIndexRuntime[0]);
                if (runtime_buffer_index_temp_delta[1]  > 0) logScreenBufferUpdate(m_bufferLog[1],  m_bufferLogIndex[1],  m_logScreenStartIndexRuntime[1]);
                if (runtime_buffer_index_temp_delta[2]  > 0) logScreenBufferUpdate(m_bufferLog[2],  m_bufferLogIndex[2],  m_logScreenStartIndexRuntime[2]);
                if (runtime_buffer_index_temp_delta[3]  > 0) logScreenBufferUpdate(m_bufferLog[3],  m_bufferLogIndex[3],  m_logScreenStartIndexRuntime[3]);
                if (runtime_buffer_index_temp_delta[4]  > 0) logScreenBufferUpdate(m_bufferLog[4],  m_bufferLogIndex[4],  m_logScreenStartIndexRuntime[4]);
                if (runtime_buffer_index_temp_delta[5]  > 0) logScreenBufferUpdate(m_bufferLog[5],  m_bufferLogIndex[5],  m_logScreenStartIndexRuntime[5]);
                if (runtime_buffer_index_temp_delta[6]  > 0) logScreenBufferUpdate(m_bufferLog[6],  m_bufferLogIndex[6],  m_logScreenStartIndexRuntime[6]);
                if (runtime_buffer_index_temp_delta[7]  > 0) logScreenBufferUpdate(m_bufferLog[7],  m_bufferLogIndex[7],  m_logScreenStartIndexRuntime[7]);
                if (runtime_buffer_index_temp_delta[8]  > 0) logScreenBufferUpdate(m_bufferLog[8],  m_bufferLogIndex[8],  m_logScreenStartIndexRuntime[8]);
                if (runtime_buffer_index_temp_delta[9]  > 0) logScreenBufferUpdate(m_bufferLog[9],  m_bufferLogIndex[9],  m_logScreenStartIndexRuntime[9]);
                if (runtime_buffer_index_temp_delta[10] > 0) logScreenBufferUpdate(m_bufferLog[10], m_bufferLogIndex[10], m_logScreenStartIndexRuntime[10]);
                if (runtime_buffer_index_temp_delta[11] > 0) logScreenBufferUpdate(m_bufferLog[11], m_bufferLogIndex[11], m_logScreenStartIndexRuntime[11]);
                if (runtime_buffer_index_temp_delta[12] > 0) logScreenBufferUpdate(m_bufferLog[12], m_bufferLogIndex[12], m_logScreenStartIndexRuntime[12]);
                if (runtime_buffer_index_temp_delta[13] > 0) logScreenBufferUpdate(m_bufferLog[13], m_bufferLogIndex[13], m_logScreenStartIndexRuntime[13]);
                if (runtime_buffer_index_temp_delta[14] > 0) logScreenBufferUpdate(m_bufferLog[14], m_bufferLogIndex[14], m_logScreenStartIndexRuntime[14]);
                if (runtime_buffer_index_temp_delta[15] > 0) logScreenBufferUpdate(m_bufferLog[15], m_bufferLogIndex[15], m_logScreenStartIndexRuntime[15]);
}

void            CKernel::logScreenBufferUpdate      (   const char* pSourceBuffer,
                                                        u32         sourceIndex,
                                                        u32&        screenStartIndex )
{
                if (pSourceBuffer == 0) return;
                if (sourceIndex == 0) return;

                u32 drawIndex  = screenStartIndex;
                u32 scanIndex  = screenStartIndex;

                unsigned col   = 0;
                unsigned row   = 0;

                while (scanIndex < sourceIndex)
                    {
                    char ch = pSourceBuffer[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < sourceIndex && pSourceBuffer[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < sourceIndex)
                                {
                                drawIndex++;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                pSourceBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;

                            bufferScreenClear();

                            bufferScreenDraw(
                                                pSourceBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenDraw(
                                    pSourceBuffer,
                                    drawIndex,
                                    sourceIndex,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                screenStartIndex = drawIndex;
}
*/
/*
void            CKernel::logScreenDeltaUpdate       (   void )
{
                boot_buffer_index_temp_new          = m_logBufferIndex;

                memcpy( runtime_buffer_index_temp_new,  m_bufferLogIndex, sizeof(runtime_buffer_index_temp_new) );

                boot_buffer_index_temp_delta        = boot_buffer_index_temp_new        - boot_buffer_index_temp_old;

                runtime_buffer_index_temp_delta[0]  = runtime_buffer_index_temp_new[0]  - runtime_buffer_index_temp_old[0];
                runtime_buffer_index_temp_delta[1]  = runtime_buffer_index_temp_new[1]  - runtime_buffer_index_temp_old[1];
                runtime_buffer_index_temp_delta[2]  = runtime_buffer_index_temp_new[2]  - runtime_buffer_index_temp_old[2];
                runtime_buffer_index_temp_delta[3]  = runtime_buffer_index_temp_new[3]  - runtime_buffer_index_temp_old[3];
                runtime_buffer_index_temp_delta[4]  = runtime_buffer_index_temp_new[4]  - runtime_buffer_index_temp_old[4];
                runtime_buffer_index_temp_delta[5]  = runtime_buffer_index_temp_new[5]  - runtime_buffer_index_temp_old[5];
                runtime_buffer_index_temp_delta[6]  = runtime_buffer_index_temp_new[6]  - runtime_buffer_index_temp_old[6];
                runtime_buffer_index_temp_delta[7]  = runtime_buffer_index_temp_new[7]  - runtime_buffer_index_temp_old[7];
                runtime_buffer_index_temp_delta[8]  = runtime_buffer_index_temp_new[8]  - runtime_buffer_index_temp_old[8];
                runtime_buffer_index_temp_delta[9]  = runtime_buffer_index_temp_new[9]  - runtime_buffer_index_temp_old[9];
                runtime_buffer_index_temp_delta[10] = runtime_buffer_index_temp_new[10] - runtime_buffer_index_temp_old[10];
                runtime_buffer_index_temp_delta[11] = runtime_buffer_index_temp_new[11] - runtime_buffer_index_temp_old[11];
                runtime_buffer_index_temp_delta[12] = runtime_buffer_index_temp_new[12] - runtime_buffer_index_temp_old[12];
                runtime_buffer_index_temp_delta[13] = runtime_buffer_index_temp_new[13] - runtime_buffer_index_temp_old[13];
                runtime_buffer_index_temp_delta[14] = runtime_buffer_index_temp_new[14] - runtime_buffer_index_temp_old[14];
                runtime_buffer_index_temp_delta[15] = runtime_buffer_index_temp_new[15] - runtime_buffer_index_temp_old[15];

                boot_buffer_index_temp_old          = boot_buffer_index_temp_new;

                memcpy( runtime_buffer_index_temp_old, runtime_buffer_index_temp_new, sizeof(runtime_buffer_index_temp_old) );
}

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;

                logScreenDeltaUpdate();

                if (boot_buffer_index_temp_delta > 0) logScreenBufferUpdate( m_logBuffer, m_logBufferIndex, m_logScreenStartIndexBoot, m_logScreenLastRow );                   // *** CHANGE: pass last printed row as draw offset ***

                if (m_bufferLog == nullptr) return;

                if (runtime_buffer_index_temp_delta[0]  > 0) logScreenBufferUpdate(m_bufferLog[0],  m_bufferLogIndex[0],  m_logScreenStartIndexRuntime[0],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[1]  > 0) logScreenBufferUpdate(m_bufferLog[1],  m_bufferLogIndex[1],  m_logScreenStartIndexRuntime[1],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[2]  > 0) logScreenBufferUpdate(m_bufferLog[2],  m_bufferLogIndex[2],  m_logScreenStartIndexRuntime[2],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[3]  > 0) logScreenBufferUpdate(m_bufferLog[3],  m_bufferLogIndex[3],  m_logScreenStartIndexRuntime[3],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[4]  > 0) logScreenBufferUpdate(m_bufferLog[4],  m_bufferLogIndex[4],  m_logScreenStartIndexRuntime[4],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[5]  > 0) logScreenBufferUpdate(m_bufferLog[5],  m_bufferLogIndex[5],  m_logScreenStartIndexRuntime[5],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[6]  > 0) logScreenBufferUpdate(m_bufferLog[6],  m_bufferLogIndex[6],  m_logScreenStartIndexRuntime[6],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[7]  > 0) logScreenBufferUpdate(m_bufferLog[7],  m_bufferLogIndex[7],  m_logScreenStartIndexRuntime[7],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[8]  > 0) logScreenBufferUpdate(m_bufferLog[8],  m_bufferLogIndex[8],  m_logScreenStartIndexRuntime[8],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[9]  > 0) logScreenBufferUpdate(m_bufferLog[9],  m_bufferLogIndex[9],  m_logScreenStartIndexRuntime[9],  m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[10] > 0) logScreenBufferUpdate(m_bufferLog[10], m_bufferLogIndex[10], m_logScreenStartIndexRuntime[10], m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[11] > 0) logScreenBufferUpdate(m_bufferLog[11], m_bufferLogIndex[11], m_logScreenStartIndexRuntime[11], m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[12] > 0) logScreenBufferUpdate(m_bufferLog[12], m_bufferLogIndex[12], m_logScreenStartIndexRuntime[12], m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[13] > 0) logScreenBufferUpdate(m_bufferLog[13], m_bufferLogIndex[13], m_logScreenStartIndexRuntime[13], m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[14] > 0) logScreenBufferUpdate(m_bufferLog[14], m_bufferLogIndex[14], m_logScreenStartIndexRuntime[14], m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
                if (runtime_buffer_index_temp_delta[15] > 0) logScreenBufferUpdate(m_bufferLog[15], m_bufferLogIndex[15], m_logScreenStartIndexRuntime[15], m_logScreenLastRow); // *** CHANGE: pass last printed row as draw offset ***
}

void            CKernel::logScreenBufferUpdate      (   const char* pSourceBuffer,
                                                        u32         sourceIndex,
                                                        u32&        screenStartIndex,
                                                        unsigned    startRow )                 // *** CHANGE: added draw row offset ***
{
                if (pSourceBuffer == 0) return;
                if (sourceIndex == 0) return;

                u32 drawIndex  = screenStartIndex;
                u32 scanIndex  = screenStartIndex;

                unsigned col   = 0;
                unsigned row   = 0;
                unsigned drawStartRow = startRow;                          // *** ADDITION: current draw row offset ***

                while (scanIndex < sourceIndex)
                    {
                    char ch = pSourceBuffer[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if ((drawStartRow + row) >= (gE_Rows - 1))          // *** CHANGE: include draw row offset in bottom test ***
                            {
                            while (drawIndex < sourceIndex && pSourceBuffer[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < sourceIndex)
                                {
                                drawIndex++;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                pSourceBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            drawStartRow = 0;                               // *** ADDITION: after clear/redraw, window starts at row 0 ***

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if ((drawStartRow + row) >= (gE_Rows - 1))          // *** CHANGE: include draw row offset in bottom test ***
                            {
                            drawIndex += gE_Cols;

                            bufferScreenClear();

                            bufferScreenDraw(
                                                pSourceBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            drawStartRow = 0;                               // *** ADDITION: after clear/redraw, window starts at row 0 ***

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenDraw(
                                    pSourceBuffer,
                                    drawIndex,
                                    sourceIndex,
                                    0,
                                    drawStartRow,                           // *** CHANGE: append at last printed row instead of row 0 ***
                                    0xFFFFFFFF
                                    );

                m_logScreenLastRow = drawStartRow + row;                    // *** ADDITION: remember last printed row for next source ***

                screenStartIndex = drawIndex;
}
*/

bool            CKernel::memoryDebugCheckpoint      (   const char* p_Label,
                                                        bool        p_DumpStatus )
{
                CMemorySystem* pMem = CMemorySystem::Get();

                if (pMem == nullptr)
                    {
                    return FALSE;
                    }

                size_t total = pMem->GetMemSize();
                size_t low   = pMem->GetHeapFreeSpace(HEAP_LOW);
                size_t high  = pMem->GetHeapFreeSpace(HEAP_HIGH);
                size_t any   = pMem->GetHeapFreeSpace(HEAP_ANY);

#ifdef __DEBUG_LOG__

                nextline( MY_BUFFER, MY_INDEX );

                storeLog(   MY_BUFFER,
                            MY_INDEX,
                            "MEM",
                            EMPTYLOG,
                            p_Label,
                            EMPTYLOG,
                            "TOTAL",
                            (u32)total,
                            "LOW",
                            (u32)low );

                storeLog(   MY_BUFFER,
                            MY_INDEX,
                            "MEM",
                            EMPTYLOG,
                            p_Label,
                            EMPTYLOG,
                            "HIGH",
                            (u32)high,
                            "ANY",
                            (u32)any );

#endif

#ifdef HEAP_DEBUG
                if (p_DumpStatus)
                    {
                    CMemorySystem::DumpStatus();
                    }
#endif
                nextline( MY_BUFFER, MY_INDEX );
                
                return TRUE;
}