#ifndef _kernel_h
#define _kernel_h

#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/machineinfo.h>
#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>

#include "CLogger.h"

#include <circle/types.h>
#include <string.h>

#define         EMPTYSTR                0 // ""                                                 // for the logger
#define         EMPTYLOG                255                                                // for the logger


#define LOG_BUFFER_SIZE (1024 * 64)
#define LOGLEVEL        LogDebug

enum TShutdownMode
{
                ShutdownNone,
                ShutdownHalt,
                ShutdownReboot
};

class CKernel
{
public:
                CKernel                 (   void );
                ~CKernel                (   void );

                boolean Initialize      (   void );

                TShutdownMode Run       (   void );

public:
                char            m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32             m_logBufferIndex = 0;
                char            m_startupBuffer[LOG_BUFFER_SIZE] = {0};
                u32             m_startupBufferIndex = 0;

public:
                boolean         frameBufferInit            (    void );

                void            bufferToScreenPlot         (    unsigned x,
                                                                unsigned y,
                                                                u32 color );

                void            bufferToScreenDrawChar     (    char        ch,
                                                                unsigned    charCol,
                                                                unsigned    charRow,
                                                                u32         fgColor );

                void            bufferToScreenClear        (    void );

                void            bufferToScreenDrawBuffer   (    const char* pSourceBuffer,
                                                                u32         startIndex,
                                                                u32         endIndex,
                                                                unsigned    startCol,
                                                                unsigned    startRow,
                                                                u32         fgColor );

                void            bufferToScreenGetGrid      (    unsigned& cols,
                                                                unsigned& rows );

                void            storeLog                   (            char*                           p_bufferArray,
                                                                        u32&                            index,
                                                                const   char*                           p_string0, 
                                                                        u32                             p_value0    = EMPTYLOG,
                                                                const   char*                           p_string1   = EMPTYSTR, 
                                                                        u32                             p_value1    = EMPTYLOG,
                                                                const   char*                           p_string2   = EMPTYSTR, 
                                                                        u32                             p_value2    = EMPTYLOG,
                                                                const   char*                           p_string3   = EMPTYSTR, 
                                                                        u32                             p_value3    = EMPTYLOG);                                                              

                void            storeMsg                   (            char*                           p_buffer,
                                                                        u32&                            index,
                                                                        const char*                     label,
                                                                        const void*                     tx_msg,
                                                                        u32                             total_size);
                                                                        
                void            nextline                   (            char*                           p_buffer,
                                                                        u32&                            index);

                boolean         startupScreen               (   void );                                                                

private:
        static  void            TimerHandler               (    TKernelTimerHandle hTimer, 
                                                                void *pParam, 
                                                                void *pContext);
                                                                
        static  void            LoggerSink                 (    void*       pContext,
                                                                const char* pText,
                                                                unsigned    nLength );

private:
                CActLED             m_ActLED;
                CKernelOptions      m_Options;
                CMachineInfo        m_MachineInfo;                

public:
                CBcmFrameBuffer     gE_FrameBuffer;
                CCharGenerator      gE_CharGenerator;

                u32*                gE_PixelBuffer      = nullptr;
                unsigned            gE_PitchBytes       = 0;
                unsigned            gE_ScreenWidth      = 0;
                unsigned            gE_ScreenHeight     = 0;
                unsigned            gE_CharWidth        = 0;
                unsigned            gE_CharHeight       = 0;
                unsigned            gE_Cols             = 0;
                unsigned            gE_Rows             = 0;

private:
                CExceptionHandler   m_ExceptionHandler;
                CInterruptSystem    m_Interrupt;
                CTimer              m_Timer;
                CLogger             m_Logger;
};

#endif