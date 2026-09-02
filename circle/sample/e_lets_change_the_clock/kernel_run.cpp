#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex

TShutdownMode CKernel::Run(void)
{
                while (/*m_resetFlag == false*/ 1)
                    {

                    m_CPUThrottle.Update(); // NEW!!!
                       
                    if (!m_SD_has_load)                                             // first load block - get the system files from sd
                        {
                        wrapper_load_sd();
                        wrapper_parser_sd();
                        wrapper_init_gl_sd();

                        msDelay(3000);
                        m_logKernelIndex = 0;
                        bufferScreenClear();

                        m_SD_has_load = true;
                        }
                    if (updateUSB("umsd1") == true && m_USB_has_load == false)      // second load block - get user files from usb AND there is a devide attached
                        {
                        wrapper_load_usb();   
                        wrapper_parser_usb();
                        wrapper_init_gl_usb();

                        msDelay(3000);
                        m_logKernelIndex = 0;
                        bufferScreenClear();

                        m_USB_has_load = true;
                        }
                    if( m_SD_has_load && m_USB_has_load )
                        {
                        m_logKernelIndex = 0;
                        bufferScreenClear();
                        } 

                fpsBegin();                                                             // here starts the actual runtimeloop
#ifdef USE_MIDI
                    updateMIDI();
#endif                    
                    resetMenuPickUpFlags();
                    storeModes();

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;                            // to debug the audio mode and menu code!!!
                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                    
                    readAndConvertADC();
                
                    adc_ProcessAudio();

                    adc_AdvanceIndex();

                    randomVec8(g_frameStart);

                    buttonPingB( 0, SW_PIN_A, 1, SW_PIN_B );                        // 

                    buttonConsumer(0,1);                                              //  here goes the button consumer and menu code

                    dispatchLayer();

                    applyTargetModes();  // <- correct place here?

                    getChannelModeA(0);
                    getChannelModeA(1);
                    getChannelModeA(2);
                    getChannelModeA(3);
                    getChannelModeA(4);
                    getChannelModeA(5);
                    getChannelModeA(6);
                    getChannelModeA(7);

                    updateLED();

                    sample1WaveTable( 0, LF1_WAVE, LF1, 255 );
                    sample1WaveTable( 1, LF2_WAVE, LF2, 255 );

                    predict1Beat( 0, LF1_MULT );
                    predict1Beat( 1, LF2_MULT ); 
                                       
                //  logButtonStatesRuntime();

                    logModesRuntime( 0 );

                //  logPickUpFlags( 9 );

                //  logInOutRuntime( 10 );

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

                    drawGLsPrg();

                fpsBreak();

                    frmBufferSwap(&m_ogl); 

                fpsEnd();

                    }
                return ShutdownHalt;                    
}