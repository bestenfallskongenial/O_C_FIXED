#include "kernel.h"

//  #undef  __DEBUG_LOG__
    #define __DEBUG_LOG__

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex   

TShutdownMode CKernel::Run(void)
{

                unsigned r = 0;
                unsigned g = 127;
                unsigned b = 186;

                while (/*m_resetFlag == false*/ 1)
                    {

                //  bufferScreenClear();

                    g_currentTime = m_Timer.GetClockTicks(); 

                    if (!m_SD_has_load) 
                        {
                        wrapper_from_sd();
                        wrapper_parser();
                        wrapper_init_gl_sd();

                        m_SD_has_load = true;
                        }

                    if (updateUSB("umsd1") == true && m_USB_has_load == false)
                        {

                        wrapper_load_usb();   

                        wrapper_parser();
                        wrapper_init_gl_usb();
       
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                  /*gen83FileName("TXT"*/
                                                    "bootlog.txt",
                                                    m_logBuffer,            // stores the pre-init buffer
                                                    m_logBufferIndex );

                                                    msDelay(100);
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    "GLSL.txt",
                                                    m_bufferLog[1],
                                                    m_bufferLogIndex[1] );

                                                    msDelay(100);
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    "parser.txt",
                                                    m_bufferLog[0],
                                                    m_bufferLogIndex[0] );
/*
                                                    msDelay(100);
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    "vi0pars.txt",
                                                    m_bufferLog[0],
                                                    m_bufferLogIndex[0] );
                                                    
                                                    msDelay(100);
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    "vi1pars.txt",
                                                    m_bufferLog[1],
                                                    m_bufferLogIndex[1] );                                                    
*/
                        m_USB_has_load = true;   
                        bufferScreenDraw( "we are done here", 0, sizeof("we are done here"), 0, 20, 0xFFFFFFFF );
                        }

                    WS2812_SetLED(0, r, g, b);
                    WS2812_SetLED(1, b, r, g);
                    WS2812_SetLED(2, g, b, r);
                    WS2812_SetLED(3, (r*2)%255, g, b);

                    WS2812_Update();
                    
                    r = (r + 1) % 256;
                    g = (g + 2) % 256;
                    b = (b + 3) % 256;

                    buttonPing(0, SW_PIN_A);
                    buttonPing(1, SW_PIN_B);

                //  logButtonStatesRuntime();

                    randomVec8(g_currentTime);

                    readAndConvertADC();
                    adc_AdvanceIndex();

                //  logInOutRuntime();
/*
                    frmBufferSet               (   &m_vtx );
                    setUniPrg                  (   &m_ogl, 
                                                    &m_fsh, 
                                                    &m_tex,
                                                //  int         gl_current_tex,
                                                    0 );

                    setTexPrg                   (   &m_ogl, 
                                                    &m_fsh, 
                                                    &m_tex,
                                                    0,
                                                    0 );

                    drawGLsPrg                  (   );

                    frmRateBreak                (   false );        

                    frmBufferSwap               (   &m_ogl );
*/
                    msDelay(25);

                    if ( checkUpdate() == true ) bufferScreenDraw( "update present", 0, sizeof("update present"), 0, 21, 0xFFFFFFFF );
                    }

                return ShutdownHalt;
}
