#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_logKernelIndex 

TShutdownMode CKernel::Run(void)
{
                while ( m_resetFlag == false )
                    {
                    fpsBegin();                 // here starts the actual runtimeloop

                    m_CPUThrottle.Update();     // NEW!!!
                       
                    loadFromSDruntime();

                    loadFromUSBruntime();

                    storeBootLogsRuntime();

                    updateMIDI();
                  
                    resetMenuPickUpFlags();

                    storeModes();
                    
                    readAndConvertADC();
             
                    adc_ProcessAudio();

                    adc_AdvanceIndex();

                    randomVec8(g_frameStart);

                    buttonPingB( 0, SW_PIN_A, 1, SW_PIN_B );                        // 

                    buttonConsumer(0,1);                                              //  here goes the button consumer and menu code

                    updateTimeouts();   // NEW - common timeout

                    dispatchLayer();

                    applyTargetModes();  // <- correct place here?

                    if ( m_SD_has_load && m_USB_has_load && filecounter[FT_VID][FLD_VALID] > 0 )
                        {
                        framePollerMMAL( m_vid.nal_block_offset[m_activeVideo][m_activeFrame], m_vid.nal_block_length[m_activeVideo][m_activeFrame]);
                        }

                    storeMMALlogRuntime();

                    storeGLSLlogRuntime();

                    getChannelModeB();

                    menuLedUpdate();

                    sample1WaveTable( m_bufferLfo, 0, LF1_WAVE, LF1, 1023 );
                    sample1WaveTable( m_bufferLfo, 1, LF2_WAVE, LF2, 1023 );                    

                    predict1Beat( 0, LF1_MULT );
                    predict1Beat( 1, LF2_MULT ); 

        if (m_tex.m_EGLimage != EGL_NO_IMAGE_KHR)
            {
            convertVideoTexture(   &m_ogl,
                                   &m_vtx,
                                   &m_csh,
                                   &m_tex,
                                   &m_cvt);
            }                    
                    
                    setUniPrg(&m_ogl, &m_fsh, &m_tex, filecounter[FT_TEX][FLD_VALID]);
                            
                    setTexPrg(&m_ogl, &m_fsh, &m_tex, m_activeTex, filecounter[FT_TEX][FLD_VALID]);                            

                    setTexVideo(&m_tex, &m_cvt);

                    setTexBackbuffer( &m_fsh, &m_tex );
                    
                    drawGLsPrg();

                    captureBackbuffer( &m_ogl, &m_tex );

                    if (g_menuLayer != 0 || m_BPM_hold_A)
                        {
                        provideTileCoordByLayer( g_menuLayer );

                        setUniOvl( &m_ogl, &m_osh, &m_omt );

                        setTexOvl( &m_ogl, &m_osh, &m_omt );

                        drawGLsOvl( &m_vtx );
                        }                               
                      
                fpsBreak();

                    frmBufferSwap(&m_ogl); 

                fpsEnd();


                    buttonPingB( 0, SW_PIN_A, 1, SW_PIN_B );

                    m_resetFlag = m_resetFlag || (g_buttons_states[0][BTN_HOLD_TICK] >= 100 && g_buttons_states[1][BTN_HOLD_TICK] >= 100);

                    buttonConsumer(0,1);             
                    }

                    storeBootlogReset();
                    storeMMALlogReset();
                    storeGLSLlogReset();


                return ShutdownHalt;                    
}