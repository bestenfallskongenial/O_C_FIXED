#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_logKernelIndex 

TShutdownMode CKernel::Run(void)
{
                while (/*m_resetFlag == false*/ 1)
                    {
                    fpsBegin();                 // here starts the actual runtimeloop

                    m_CPUThrottle.Update();     // NEW!!!
                       
                    if (!m_SD_has_load)         // first load block - get the system files from sd
                        {
                        wrapper_load_sd();
                        wrapper_parser_sd();
                        wrapper_init_gl_sd();

                        bufferScreenClear();

                        m_SD_has_load = true;
                        }
                    if (updateUSB("umsd1") == true && m_USB_has_load == false)      // second load block - get user files from usb AND there is a devide attached
                        {
                        wrapper_load_usb();   
                        wrapper_parser_usb();
                        wrapper_init_gl_usb();

                        bufferScreenClear();

                        m_USB_has_load = true;
                        }
                    if( m_SD_has_load && m_USB_has_load && !m_bootLogsSaved )
                        {
                        m_bootLogsSaved = true;
                        
                        saveFromBuffer  (   PARTITION_NAME_SD,
                                        // gen83FileName("TXT"),
                                            "bootlog.txt",
                                            m_logKernel,            // stores the pre-init buffer
                                            m_logKernelIndex );

                        saveFromBuffer  (   PARTITION_NAME_SD,
                                            "GLSL.txt",
                                            m_bufferLog[LOG_GLSL_0],
                                            m_bufferLogIndex[LOG_GLSL_0] );          

                        m_logKernelIndex = 0;
                        bufferScreenClear();
                        } 

                    updateMIDI();
                  
                    resetMenuPickUpFlags();
                    storeModes();
                    
                    readAndConvertADC();
             
                    adc_ProcessAudio();

                    adc_AdvanceIndex();

                    randomVec8(g_frameStart);

                    buttonPingB( 0, SW_PIN_A, 1, SW_PIN_B );                        // 

                    buttonConsumer(0,1);                                              //  here goes the button consumer and menu code

                    dispatchLayer();

                    applyTargetModes();  // <- correct place here?

                    getChannelModeB();

                    menuLedUpdate();

                    sample1WaveTable( m_bufferLfo, 0, LF1_WAVE, LF1, 1023 );
                    sample1WaveTable( m_bufferLfo, 1, LF2_WAVE, LF2, 1023 );                    

                    predict1Beat( 0, LF1_MULT );
                    predict1Beat( 1, LF2_MULT ); 
                                       
                    logModesRuntime( 0 );

                    logInfosRuntime( 10 );

                    logInfoFrameRate( 20 );

                    setUniPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            filecounter[FT_FSH][FLD_VALID]);
                            
                    setTexPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            m_activeTex,
                            filecounter[FT_TEX][FLD_VALID]);                            

                    setTexBackbuffer( &m_fsh, &m_tex, filecounter[FT_TEX][FLD_VALID]);

                    drawGLsPrg();

                    captureBackbuffer( &m_ogl, &m_tex );
                      
                fpsBreak();

                    frmBufferSwap(&m_ogl); 

                fpsEnd();

                    }
                return ShutdownHalt;                    
}