#include "kernel.h"

static const char FromKernel[] = "kernel";

void CKernel::debug()
{
   
unsigned tick = 0;


                    m_Timer.MsDelay(1000);
                    
                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';
                    
                    startupScreen();

                    m_Timer.MsDelay(1000);
                    bufferScreenClear();
                    

                    tick++;
                    
                    for (unsigned i = 0; i < m_LEDCount; i++)
                        {
                        unsigned phase = (tick + i) % 6;

                        u8 r = 0;
                        u8 g = 0;
                        u8 b = 0;

                        if      (phase == 0) { r = 255; g = 0;   b = 0;   }
                        else if (phase == 1) { r = 255; g = 128; b = 0;   }
                        else if (phase == 2) { r = 0;   g = 255; b = 0;   }
                        else if (phase == 3) { r = 0;   g = 255; b = 255; }
                        else if (phase == 4) { r = 0;   g = 0;   b = 255; }
                        else                 { r = 255; g = 0;   b = 255; }

                        WS2812_SetLED(i, r, g, b);
                        }

                    WS2812_Update();

                    bufferScreenClear();

                    u32 colorCycle[8] =
                        {
                        0xFFFFFFFF,
                        0xFFFF0000,
                        0xFF00FF00,
                        0xFF0000FF,
                        0xFFFFFF00,
                        0xFFFF00FF,
                        0xFF00FFFF,
                        0xFF808080
                        };

                    u32 color0 = colorCycle[(tick + 0) & 7];
                    u32 color1 = colorCycle[(tick + 1) & 7];
                    u32 color2 = colorCycle[(tick + 2) & 7];
                    u32 color3 = colorCycle[(tick + 3) & 7];
                    u32 color4 = colorCycle[(tick + 4) & 7];
                    u32 color5 = colorCycle[(tick + 5) & 7];

                    int adc0 = ReadMCP3008Raw(0);
                    int adc1 = ReadMCP3008Raw(1);
                    int adc2 = ReadMCP3008Raw(2);
                    int adc3 = ReadMCP3008Raw(3);
                    int adc4 = ReadMCP3008Raw(4);
                    int adc5 = ReadMCP3008Raw(5);
                    int adc6 = ReadMCP3008Raw(6);
                    int adc7 = ReadMCP3008Raw(7);

                    unsigned mytestCol = tick % gE_Cols;
                    unsigned mytestRow = 0;

                    bufferScreenDraw(
                                            "mytest",
                                            0,
                                            sizeof("mytest"),
                                            mytestCol,
                                            mytestRow,
                                            color0
                                            );

                    unsigned alphabetStart = tick % 20;
                    unsigned alphabetEnd   = alphabetStart + 8;
                    unsigned alphabetCol   = 0;
                    unsigned alphabetRow   = 2;

                    bufferScreenDraw(
                                            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                            alphabetStart,
                                            alphabetEnd,
                                            alphabetCol,
                                            alphabetRow,
                                            color1
                                            );

                    unsigned columnRowCol = (tick * 2) % gE_Cols;
                    unsigned columnRowRow = 4 + ((tick / 2) % 4);

                    bufferScreenDraw(
                                            "column and row test",
                                            0,
                                            sizeof("column and row test"),
                                            columnRowCol,
                                            columnRowRow,
                                            color2
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "tick %u mytest col %u row %u",
                                    tick,
                                    mytestCol,
                                    mytestRow
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            10,
                                            color3
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogWarning,
                                    "alphabet start %u end %u col %u row %u",
                                    alphabetStart,
                                    alphabetEnd,
                                    alphabetCol,
                                    alphabetRow
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            4,
                                            12,
                                            color4
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogError,
                                    "columnrow col %u row %u",
                                    columnRowCol,
                                    columnRowRow
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            8,
                                            14,
                                            color5
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "adc0 %d adc1 %d adc2 %d adc3 %d",
                                    adc0,
                                    adc1,
                                    adc2,
                                    adc3
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            16,
                                            colorCycle[(tick + 6) & 7]
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "adc4 %d adc5 %d adc6 %d adc7 %d",
                                    adc4,
                                    adc5,
                                    adc6,
                                    adc7
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            18,
                                            colorCycle[(tick + 7) & 7]
                                            );
/*
                    if (!bExceptionTimerStarted && tick == 10)
                        {
                        bExceptionTimerStarted = TRUE;

                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';

                        m_Logger.Write(
                                        FromKernel,
                                        LogNotice,
                                        "starting exception timer"
                                        );

                        bufferScreenDraw(
                                                m_logBuffer,
                                                0,
                                                m_logBufferIndex,
                                                0,
                                                16,
                                                0xFFFFFFFF
                                                );

                        m_Timer.StartKernelTimer(5 * HZ, TimerHandler);
                        }
*/
                   
                 
}

/*
TShutdownMode CKernel::Run                              (   void )
{
                unsigned tick = 0;
                bool  bExceptionTimerStarted = FALSE;

                while (1)
                    {
                    m_Timer.MsDelay(1000);

                    tick++;

                    bufferScreenClear();

                    unsigned mytestCol = tick % gE_Cols;
                    unsigned mytestRow = 0;

                    bufferScreenDraw(
                                            "mytest",
                                            0,
                                            sizeof("mytest"),
                                            mytestCol,
                                            mytestRow,
                                            0xFFFFFFFF
                                            );

                    unsigned alphabetStart = tick % 20;
                    unsigned alphabetEnd   = alphabetStart + 8;
                    unsigned alphabetCol   = 0;
                    unsigned alphabetRow   = 2;

                    bufferScreenDraw(
                                            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                            alphabetStart,
                                            alphabetEnd,
                                            alphabetCol,
                                            alphabetRow,
                                            0xFFFFFFFF
                                            );

                    unsigned columnRowCol = (tick * 2) % gE_Cols;
                    unsigned columnRowRow = 4 + ((tick / 2) % 4);

                    bufferScreenDraw(
                                            "column and row test",
                                            0,
                                            sizeof("column and row test"),
                                            columnRowCol,
                                            columnRowRow,
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "tick %u mytest col %u row %u",
                                    tick,
                                    mytestCol,
                                    mytestRow
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            10,
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogWarning,
                                    "alphabet start %u end %u col %u row %u",
                                    alphabetStart,
                                    alphabetEnd,
                                    alphabetCol,
                                    alphabetRow
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            4,
                                            12,
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogError,
                                    "columnrow col %u row %u",
                                    columnRowCol,
                                    columnRowRow
                                    );

                    bufferScreenDraw(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            8,
                                            14,
                                            0xFFFFFFFF
                                            );

                    if (!bExceptionTimerStarted && tick == 10)
                        {
                        bExceptionTimerStarted = TRUE;

                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';

                        m_Logger.Write(
                                        FromKernel,
                                        LogNotice,
                                        "starting exception timer"
                                        );

                        bufferScreenDraw(
                                                m_logBuffer,
                                                0,
                                                m_logBufferIndex,
                                                0,
                                                16,
                                                0xFFFFFFFF
                                                );

                        m_Timer.StartKernelTimer(5 * HZ, TimerHandler);
                        }
   
                    }

}
*/

void            CKernel::TimerHandler                   (           TKernelTimerHandle  hTimer, 
                                                                    void*               pParam, 
                                                                    void*               pContext)
{
                void (*pInvalid)(void) = (void (*)(void)) 0x500000;

                (*pInvalid)();
}