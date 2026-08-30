#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run(void)
{

                g_currentTime = m_Timer.GetClockTicks();    
 
                randomVec8              ( g_currentTime );

                saveFromBufferM         (   PARTITION_NAME_SD,
                                            make83FileName("TXT"),
                                            m_logBuffer,
                                            m_logBufferIndex );
                msDelay(1000);

                while (/*m_resetFlag == false*/ 1)
                    {

                    g_currentTime = m_Timer.GetClockTicks(); 

                    bufferScreenClear();

                    if (!m_SD_has_load) 
                        {
                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';

                        wrapper_from_sd();

                        randomVec8              ( g_currentTime );            
                                                    
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    make83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );

                        bufferScreenDraw(   "load and log from/to sd",
                                                    0,
                                                    sizeof("load and log from/to sd"),
                                                    0,
                                                    0,
                                                    0xFFFFFFFF );

                        msDelay(1000);        
                        bufferScreenClear();                                                                    
                        m_SD_has_load = true;
                        }

                    //  debug();
              
                    if (updateUSB("umsd1") == true && m_USB_has_load == false)
                        {
                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0'; 

                        wrapper_load_usb();          
                        
                        bufferScreenDraw(   "load from usb",
                                                    0,
                                                    sizeof("load from usb"),
                                                    0,
                                                    0,
                                                    0xFFFFFFFF );
                      

                        randomVec8              ( g_currentTime );            

                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    make83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );
                        msDelay(1000);    
                        bufferScreenClear();
                        m_USB_has_load = true;   
                                               
                        }

/*
                  readAndConvertADC();

                  adc_AdvanceIndex();

                bufferScreenDraw(   "read adc",
                                            0,
                                            sizeof("read adc"),
                                            10,
                                            0,
                                            0xFFFFFFFF );
                                        //  msDelay(1000); 

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    bufferScreenClear();

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 0",g_inOutMatrixInt[0][RAW],
                                        "channel 1",g_inOutMatrixInt[1][RAW],
                                        "channel 2",g_inOutMatrixInt[2][RAW],
                                        "channel 3",g_inOutMatrixInt[3][RAW]);     

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 4",g_inOutMatrixInt[4][RAW],
                                        "channel 5",g_inOutMatrixInt[5][RAW],
                                        "channel 6",g_inOutMatrixInt[6][RAW],
                                        "channel 7",g_inOutMatrixInt[7][RAW]);              
                                        
                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 0",g_inOutMatrixInt[0][VAL],
                                        "channel 1",g_inOutMatrixInt[1][VAL],
                                        "channel 2",g_inOutMatrixInt[2][VAL],
                                        "channel 3",g_inOutMatrixInt[3][VAL]);    

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 4",g_inOutMatrixInt[4][VAL],
                                        "channel 5",g_inOutMatrixInt[5][VAL],
                                        "channel 6",g_inOutMatrixInt[6][VAL],
                                        "channel 7",g_inOutMatrixInt[7][VAL]);                                              

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 0",g_inOutMatrixFlt[0][VAL],
                                        "channel 1",g_inOutMatrixFlt[1][VAL],
                                        "channel 2",g_inOutMatrixFlt[2][VAL],
                                        "channel 3",g_inOutMatrixFlt[3][VAL]);     

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 4",g_inOutMatrixFlt[4][VAL],
                                        "channel 5",g_inOutMatrixFlt[5][VAL],
                                        "channel 6",g_inOutMatrixFlt[6][VAL],
                                        "channel 7",g_inOutMatrixFlt[7][VAL]);               
                                        
                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 0",g_inOutMatrixInt[0][RND],
                                        "channel 1",g_inOutMatrixInt[1][RND],
                                        "channel 2",g_inOutMatrixInt[2][RND],
                                        "channel 3",g_inOutMatrixInt[3][RND]);     

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 4",g_inOutMatrixInt[4][RND],
                                        "channel 5",g_inOutMatrixInt[5][RND],
                                        "channel 6",g_inOutMatrixInt[6][RND],
                                        "channel 7",g_inOutMatrixInt[7][RND]);    

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 0",g_inOutMatrixFlt[0][RND],
                                        "channel 1",g_inOutMatrixFlt[1][RND],
                                        "channel 2",g_inOutMatrixFlt[2][RND],
                                        "channel 3",g_inOutMatrixFlt[3][RND]);     

                    storeLog(           m_logBuffer, m_logBufferIndex,
                                        "channel 4",g_inOutMatrixFlt[4][RND],
                                        "channel 5",g_inOutMatrixFlt[5][RND],
                                        "channel 6",g_inOutMatrixFlt[6][RND],
                                        "channel 7",g_inOutMatrixFlt[7][RND]);                                                

                    bufferScreenDraw(   m_logBuffer,
                                        0,
                                        m_logBufferIndex,
                                        0,
                                        0,
                                        0xFFFFFFFF );      
                //  msDelay(1000);
*/      
                    }

                return ShutdownHalt;
}
