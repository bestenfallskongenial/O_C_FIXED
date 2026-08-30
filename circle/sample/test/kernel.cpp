	#include "kernel.h"

    static const char FromKernel[] = "kernel";


				CKernel::CKernel						(	void )
				:	gE_FrameBuffer( m_Options.GetWidth(), 
                                    m_Options.GetHeight(), 
                                    32, 
                                    0, 
                                    TRUE ),
                    m_Timer(&m_Interrupt),
                    m_Logger(LOGLEVEL, &m_Timer)
{
                m_ActLED.Blink(5);
}

				CKernel::~CKernel						(	void )
{
}

boolean			CKernel::Initialize						(	void )
{
                boolean bOK = TRUE;

                if (bOK)
                    {
                    bOK = frameBufferInit();
                    }

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    }

                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
                    }

                return bOK;
}

void            CKernel::TimerHandler                   (           TKernelTimerHandle  hTimer, 
                                                                    void*               pParam, 
                                                                    void*               pContext)
{
                void (*pInvalid)(void) = (void (*)(void)) 0x500000;

                (*pInvalid)();
}

void 			CKernel::LoggerSink						(	        void* 			    pContext, 
															const   char* 	            pText, 
															        unsigned 		    nLength )
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logBufferIndex >= LOG_BUFFER_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logBuffer[pThis->m_logBufferIndex++] = ch;
                    }

                pThis->m_logBuffer[pThis->m_logBufferIndex] = '\0';
}

TShutdownMode CKernel::Run                              (   void )
{
                unsigned tick = 0;
                boolean  bExceptionTimerStarted = FALSE;

                while (1)
                    {
                    m_Timer.MsDelay(1000);

                    tick++;

                    bufferToScreenClear();

                    bufferToScreenDrawBuffer(
                                            "mytest",
                                            0,
                                            sizeof("mytest"),
                                            tick % gE_Cols,
                                            0,
                                            0xFFFFFFFF
                                            );

                    bufferToScreenDrawBuffer(
                                            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                            tick % 20,
                                            (tick % 20) + 8,
                                            0,
                                            2,
                                            0xFFFFFFFF
                                            );

                    bufferToScreenDrawBuffer(
                                            "column and row test",
                                            0,
                                            sizeof("column and row test"),
                                            (tick * 2) % gE_Cols,
                                            4 + ((tick / 2) % 4),
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "individual log tick %u",
                                    tick
                                    );

                    bufferToScreenDrawBuffer(
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
                                    "warning level test"
                                    );

                    bufferToScreenDrawBuffer(
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
                                    "error level test"
                                    );

                    bufferToScreenDrawBuffer(
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

                        bufferToScreenDrawBuffer(
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