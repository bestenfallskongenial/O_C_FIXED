#include "kernel.h"

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex   

TShutdownMode CKernel::Run(void)
{
                g_centralModeBuffer[g_gl_program_current][FLAG_TEX] = 0;            // start values for the glsl code

                while (/*m_resetFlag == false*/ 1)
                    {
                       
                    if (!m_SD_has_load)                                             // first load block - get the system files from sd
                        {
                        wrapper_load_sd();
                        wrapper_parser_sd();
                        wrapper_init_gl_sd();

                        m_SD_has_load = true;
                        }
                      
                    if (updateUSB("umsd1") == true && m_USB_has_load == false)      // second load block - get user files from usb AND there is a devide attached
                        {
                        wrapper_load_usb();   
                        wrapper_parser_usb();
                        wrapper_init_gl_usb();

                        m_USB_has_load = true;
                        }
                        
                    if( m_SD_has_load && m_USB_has_load )
                        {
                        m_logBufferIndex = 0;
                        bufferScreenClear();
                        } 

                    g_currentTime = m_Timer.GetClockTicks();                        // here starts the actual runtimeloop

                    resetMenuPickUpFlags();
                    storeModes();

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;                            // to debug the audio mode and menu code!!!
                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                    
                    readAndConvertADC();
                    adc_ProcessAudio();
                    adc_AdvanceIndex();

                    randomVec8(g_currentTime);

                    buttonPingB( 0, SW_PIN_A, 1, SW_PIN_B );                        // 

                    setLayer(0,1);                                              //  here goes the button consumer and menu code

                    dispatchLayer();

                    applyTargetModes();  // <- correct place here?
              
                //  getChannelModeA( 0 );
                //  getChannelModeA( 1 );                    

                //  getChannelModeB();
                    
                    getChannelModeA(0);
                    getChannelModeA(1);
                    getChannelModeA(2);
                    getChannelModeA(3);
                    getChannelModeA(4);
                    getChannelModeA(5);
                    getChannelModeA(6);
                    getChannelModeA(7);

                    updateLED();

                    predict1Beat( 0, LF1_MULT );
                    predict1Beat( 1, LF2_MULT ); 
                                       
                    sample1WaveTable( 0, LF1_WAVE, LF1 );
                    sample1WaveTable( 1, LF2_WAVE, LF2 );

                    logButtonStatesRuntime();

                    setUniPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            filecounter[FT_FSH][FLD_VALID]);

                    setTexPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            0,
                            filecounter[FT_TEX][FLD_VALID]);                            

                    drawGLsPrg();

                    frmRateBreak(false);

                    frmBufferSwap(&m_ogl); 

                    msDelay(25);
                    }
                return ShutdownHalt;                    
}