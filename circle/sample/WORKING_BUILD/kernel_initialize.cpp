#include "kernel.h"

//  #undef  __DEBUG__
    #define __DEBUG__

//  #undef  __LOG_INIT__
    #define __LOG_INIT__

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex    

boolean			CKernel::Initialize						(	void )
{
                bool bOK = TRUE;

            //  memoryDebugCheckpoint("BOOT START", true);

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_Interrupt.Initialize()                DONE");
#endif
                    }       
                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_Timer.Initialize()                    DONE");
#endif
                    }
                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SetRawSink(CKernel::LoggerSink, this)   DONE");
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_Logger.Initialize(0)                  DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK = frameBufferInit();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "frameBufferInit()                       DONE");

#endif
                    }
                if (bOK)
                    {
                    bOK = startupScreen();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "Hardware Info Log                       DONE");

#endif
                    }                
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_EMMC.Initialize()                     DONE");

#endif
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
#ifdef __LOG_INIT__                    
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_USBHCI.Initialize()                   DONE");

#endif
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_USBHCI.UpdatePlugAndPlay()            DONE");

#endif
                    }
                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "InitDMA Buffer                          DONE");
#endif
                    }

            //  memoryDebugCheckpoint("AFTER DMA ALLOC", true);    

                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "InitMEM Buffer                          DONE");

#endif
                    }

            //  memoryDebugCheckpoint("AFTER MEM ALLOC / BEFORE VCHIQ INIT", true);

                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_VCHIQ.Initialize()                    DONE");
#endif
                    }
                    if (bOK)
                    {
                    bcm_host_init();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "bcm_host_init()                         DONE");

#endif
                    }    
                if (bOK)
                    {
                    getStateVCHI();
#ifdef __LOG_INIT__
                    storeLog( MY_BUFFER, MY_INDEX, "m_VCHIInstance",           (u32)m_VCHIInstance );
                    storeLog( MY_BUFFER, MY_INDEX, "m_Connection",             (u32)m_Connection );

                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getStateVCHI()                          DONE");
#endif
                    }

            //  memoryDebugCheckpoint("BEFORE STRUCT INIT", true);
                if (bOK)
                    {
                    bOK = wrapperInitVCSMstruct();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "wrapperInitVCSMstruct()                 DONE");
#endif
                    }

            //  memoryDebugCheckpoint("AFTER VCSM STRUCT INIT", true);    

                if (bOK)
                    {
                    bOK = wrapperInitMMALstruct();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "wrapperInitMMALstruct()                 DONE");
#endif
                    }

            //  memoryDebugCheckpoint("AFTER MMAL STRUCT INIT", true);
                
                if (bOK)
                    {
                    bOK = wrapperVCSM();
#ifdef __LOG_INIT__

                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "Allocate/Lock VCSM Buffer               DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK = wrapperMMAL();
#ifdef __LOG_INIT__

                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "Init MMAL / ril.video_decode            DONE");
#endif
                    }

                if (bOK)
                    {
                    initOGL(    &m_ogl);
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "initOGL(&m_ogl)                         DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   SPI_init();
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SPI_init()                              DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   SMI_Init(   LED_PIN);
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SMI_Init(LED_PIN)                       DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "WS2812_Init(LED_COUNT)                  DONE");
#endif
                    }
                if (bOK)
                    {
                    GPIO_SetAlt(    CTRL_PIN, 
                                    1, 
                                    GPIO_PULL_OFF);
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF) DONE");
#endif
                    GPIO_SetAlt(    SW_PIN_A, 
                                    0, 
                                    GPIO_PULL_UP);
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP)  DONE");
#endif
                    GPIO_SetAlt(    SW_PIN_B, 
                                    0, 
                                    GPIO_PULL_UP);
#ifdef __LOG_INIT__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP)  DONE");
#endif
                    }

            //  memoryDebugCheckpoint("AFTER CKERNEL INIT", true);

                return bOK;
}


