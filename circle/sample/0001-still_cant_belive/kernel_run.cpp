#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_logKernelIndex 

TShutdownMode CKernel::Run(void)
{
                while ( m_resetFlag == false )
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

                    updateTimeouts();   // NEW - common timeout

                    dispatchLayer();

                    applyTargetModes();  // <- correct place here?

                    if ( m_SD_has_load && m_USB_has_load && filecounter[FT_VID][FLD_VALID] > 0 )
                        {
                        framePollerMMAL(
                            m_vid.nal_block_offset[m_activeVideo][m_activeFrame],
                            m_vid.nal_block_length[m_activeVideo][m_activeFrame]);
                          //m_vid.nal_block_offset[m_activeVideo][m_activeFrame],
                          //m_vid.nal_block_length[m_activeVideo][m_activeFrame]);

                        }

                    if (m_bufferLogIndex[LOG_MMAL_0] >= 65536)
                        {
                        const char* f_fileName = m_mmalRuntimeLogSaved
                                               ? "mmalnext.txt"
                                               : "mmalrun.txt";

                        if (saveFromBuffer(
                                PARTITION_NAME_SD,
                                f_fileName,
                                m_bufferLog[LOG_MMAL_0],
                                m_bufferLogIndex[LOG_MMAL_0]))
                            {
                            m_bufferLogIndex[LOG_MMAL_0] = 0;
                            m_bufferLog[LOG_MMAL_0][0] = '\0';

                            m_mmalRuntimeLogSaved = true;
                            }
                        }

                    if (m_bufferLogIndex[LOG_GLSL_0] >= 65536)
                        {
                    //  const char* f_fileName = m_glslRuntimeLogSaved ? "glslnext.txt" : "glslrun.txt";

                        const char* f_fileName = "GLSLRUN";
                        if (saveFromBuffer(
                                PARTITION_NAME_SD,
                                f_fileName,
                                m_bufferLog[LOG_GLSL_0],
                                m_bufferLogIndex[LOG_GLSL_0]))
                            {
                            m_bufferLogIndex[LOG_GLSL_0] = 0;
                            m_bufferLog[LOG_GLSL_0][0] = '\0';

                        //  m_glslRuntimeLogSaved = true;
                            }
                        }

                    getChannelModeB();

                    menuLedUpdate();

                    sample1WaveTable( m_bufferLfo, 0, LF1_WAVE, LF1, 1023 );
                    sample1WaveTable( m_bufferLfo, 1, LF2_WAVE, LF2, 1023 );                    

                    predict1Beat( 0, LF1_MULT );
                    predict1Beat( 1, LF2_MULT ); 
                                       
                //  logModesRuntime( 0 );

                //  logInfosRuntime( 10 );

                //  logInfoFrameRate( 20 );

                //  logTextureUnitsRuntime( 22 );

                    logVideoRuntime( 2 );    
                    
        if (m_tex.m_EGLimage != EGL_NO_IMAGE_KHR)
        {
            convertVideoTexture(
                &m_ogl,
                &m_vtx,
                &m_csh,
                &m_tex,
                &m_cvt);
        }                    
                    
                    setUniPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            filecounter[FT_TEX][FLD_VALID]);
                            
                    setTexPrg(&m_ogl, &m_fsh, &m_tex, m_activeTex, filecounter[FT_TEX][FLD_VALID]);                            

                    setTexVideo(&m_tex, &m_cvt);

                //  setTexBackbuffer( &m_fsh, &m_tex, filecounter[FT_TEX][FLD_VALID]);
                    setTexBackbuffer( &m_fsh, &m_tex ); // removed for the test!!!
                    
                    drawGLsPrg();

                    captureBackbuffer( &m_ogl, &m_tex ); // removed for the test!!!
// lets see...
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


                    buttonPingB( 0, SW_PIN_A, 1, SW_PIN_B );                        //

                    if (g_buttons_states[0][BTN_HOLD_TICK] >= 100 && g_buttons_states[1][BTN_HOLD_TICK] >= 100) m_resetFlag = true;

                    buttonConsumer(0,1);                                              //  here goes the button consumer and menu code                
                    }

                if (m_bufferLogIndex[LOG_MMAL_0] != 0)
                    {
                    saveFromBuffer(
                        PARTITION_NAME_SD,
                        "mmalnext.txt",
                        m_bufferLog[LOG_MMAL_0],
                        m_bufferLogIndex[LOG_MMAL_0]);
                    }

                if (m_bufferLogIndex[LOG_GLSL_0] != 0)
                    {
                    saveFromBuffer(
                        PARTITION_NAME_SD,
                        "glslnext.txt",
                        m_bufferLog[LOG_GLSL_0],
                        m_bufferLogIndex[LOG_GLSL_0]);
                    }

                return ShutdownHalt;                    
}