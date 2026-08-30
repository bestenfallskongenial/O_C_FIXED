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
/*
u32             CKernel::bufferScreenFindStartIndex (   const char* p_buffer,
                                                        u32         endIndex,
                                                        unsigned    cols,
                                                        unsigned    rows)
{
                if (p_buffer == 0) return 0;
                if (endIndex == 0) return 0;
                if (cols == 0) return 0;
                if (rows <= 1) return 0;

                u32 visible_chars = cols * (rows - 1);

                if (endIndex <= visible_chars) return 0;

                u32 startIndex = endIndex - visible_chars;

                while (startIndex < endIndex && p_buffer[startIndex] != '\n')
                    {
                    startIndex++;
                    }

                if (startIndex < endIndex)
                    {
                    startIndex++;
                    }

                return startIndex;
}
*/
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
/*
void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows == 0) return;
                if (MY_INDEX == 0) return;

                u32 startIndex = bufferScreenFindStartIndex(
                                                            MY_BUFFER,
                                                            MY_INDEX,
                                                            gE_Cols,
                                                            gE_Rows
                                                            );
// new
                u32 drawIndex = 0;

                while (drawIndex < startIndex)
                    {
                    bufferScreenClear();

                    bufferScreenDraw(
                                        MY_BUFFER,
                                        drawIndex,
                                        MY_INDEX,
                                        0,
                                        0,
                                        0xFFFFFFFF
                                        );

                    msDelay(SCROLLSPEED);

                    while (drawIndex < startIndex && MY_BUFFER[drawIndex] != '\n')
                        {
                        drawIndex++;
                        }

                    if (drawIndex < startIndex)
                        {
                        drawIndex++;
                        }
                    }
// end
                bufferScreenClear();

                bufferScreenDraw(
                                    MY_BUFFER,
                                    startIndex,
                                    MY_INDEX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );
}
*/

/*
void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows == 0) return;
                if (MY_INDEX == 0) return;

                u32 startIndex = bufferScreenFindStartIndex(
                                                            MY_BUFFER,
                                                            MY_INDEX,
                                                            gE_Cols,
                                                            gE_Rows
                                                            );

                if (m_logScreenStartIndex > startIndex)
                    {
                    m_logScreenStartIndex = 0;
                    }

                u32 drawIndex = m_logScreenStartIndex;

                while (drawIndex < startIndex)
                    {
                    bufferScreenClear();

                    bufferScreenDraw(
                                        MY_BUFFER,
                                        drawIndex,
                                        MY_INDEX,
                                        0,
                                        0,
                                        0xFFFFFFFF
                                        );

                    msDelay(SCROLLSPEED);

                    while (drawIndex < startIndex && MY_BUFFER[drawIndex] != '\n')
                        {
                        drawIndex++;
                        }

                    if (drawIndex < startIndex)
                        {
                        drawIndex++;
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    MY_BUFFER,
                                    startIndex,
                                    MY_INDEX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                m_logScreenStartIndex = startIndex;
}
*/

/*
u32             CKernel::bufferScreenFindStartIndex (   const char* p_buffer,
                                                        u32         endIndex,
                                                        unsigned    cols,
                                                        unsigned    rows)
{
                if (p_buffer == 0) return 0;
                if (endIndex == 0) return 0;
                if (cols == 0) return 0;
                if (rows <= 1) return 0;

                u32 visible_chars = cols * (rows - 1);

                if (endIndex <= visible_chars) return 0;

                u32 startIndex = endIndex - visible_chars;

                while (startIndex < endIndex && p_buffer[startIndex] != '\n')
                    {
                    startIndex++;
                    }

                if (startIndex < endIndex)
                    {
                    startIndex++;
                    }

                return startIndex;
}

void            CKernel::logScreenAppendDelta       (   const char* pSource,
                                                        u32         deltaStart,
                                                        u32         deltaEnd )
{
                if (pSource == 0) return;
                if (deltaStart >= deltaEnd) return;
            //  if (m_screenLogBuffer == 0) return;

                for (u32 i = deltaStart; i < deltaEnd; i++)
                    {
                    if (pSource[i] == '\0') break;

                    if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1))
                        {
                        logScreenCompactVisible();
                        }

                    if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1))
                        {
                        m_screenLogBufferIndex = 0;
                        m_screenLogBuffer[0] = '\0';
                        }

                    m_screenLogBuffer[m_screenLogBufferIndex++] = pSource[i];
                    }

                m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
}

void            CKernel::logScreenCompactVisible    (   void )
{
                if (m_screenLogBufferIndex == 0) return;

                u32 startIndex = bufferScreenFindStartIndex(
                                                            m_screenLogBuffer,
                                                            m_screenLogBufferIndex,
                                                            gE_Cols,
                                                            gE_Rows
                                                            );

                if (startIndex == 0) return;

                u32 newIndex = 0;

                for (u32 i = startIndex; i < m_screenLogBufferIndex; i++)
                    {
                    m_screenLogBuffer[newIndex++] = m_screenLogBuffer[i];
                    }

                m_screenLogBufferIndex = newIndex;
                m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
}

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;
            //  if (m_screenLogBuffer == 0) return;

                boolean bChanged = FALSE;

                u32 deltaStart = m_logBufferIndexLast;
                u32 deltaEnd   = m_logBufferIndex;

                if (deltaEnd < deltaStart)
                    {
                    m_logBufferIndexLast = deltaEnd;
                    }
                else if (deltaEnd > deltaStart)
                    {
                    logScreenAppendDelta(
                                            m_logBuffer,
                                            deltaStart,
                                            deltaEnd
                                            );

                    m_logBufferIndexLast = m_logBufferIndex;

                    bChanged = TRUE;
                    }

                for (unsigned n = 0; n < (LOG_SD+LOG_USB); n++)
                    {
                    if (m_bufferLog[n] == 0)
                        {
                        m_bufferLogIndexLast[n] = m_bufferLogIndex[n];
                        continue;
                        }

                    deltaStart = m_bufferLogIndexLast[n];
                    deltaEnd   = m_bufferLogIndex[n];

                    if (deltaEnd < deltaStart)
                        {
                        m_bufferLogIndexLast[n] = deltaEnd;
                        continue;
                        }

                    if (deltaEnd > deltaStart)
                        {
                        logScreenAppendDelta(
                                                m_bufferLog[n],
                                                deltaStart,
                                                deltaEnd
                                                );

                        m_bufferLogIndexLast[n] = m_bufferLogIndex[n];

                        bChanged = TRUE;
                        }
                    }

                if (!bChanged) return;

                u32 finalStartIndex = bufferScreenFindStartIndex(
                                                            m_screenLogBuffer,
                                                            m_screenLogBufferIndex,
                                                            gE_Cols,
                                                            gE_Rows
                                                            );

                u32 drawStartIndex = 0;

                while (drawStartIndex < finalStartIndex)
                    {
                    bufferScreenClear();

                    bufferScreenDraw(
                                        m_screenLogBuffer,
                                        drawStartIndex,
                                        m_screenLogBufferIndex,
                                        0,
                                        0,
                                        0xFFFFFFFF
                                        );

                    msDelay(SCROLLSPEED);

                    while (drawStartIndex < finalStartIndex && m_screenLogBuffer[drawStartIndex] != '\n')
                        {
                        drawStartIndex++;
                        }

                    if (drawStartIndex < finalStartIndex)
                        {
                        drawStartIndex++;
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    m_screenLogBuffer,
                                    finalStartIndex,
                                    m_screenLogBufferIndex,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                logScreenCompactVisible();
}
*/