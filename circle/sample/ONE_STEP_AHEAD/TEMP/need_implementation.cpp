

TShutdownMode   CKernel::Run(void)
{

                while (m_resetFlag == false)                                                  // mainloop
                    {
                    start_time_fps_calculation = m_Timer.GetClockTicks();                   // for the "fps break"
                    m_Watchdog.Start(8);

                    m_H264Decoder.MMALFramePoller( m_H264Parser.m_frameOffset[0][100],m_H264Parser.m_framelenght[0][100]);

//  i still need to figure out if / how this function causes a crash on ~ 22 minutes of runtime!
// the theory for now is that the internal debug mechanism keeps filling up an array until an memory voilation
// therefore i #ifdef all the relevant lines that may fill the debug message array,
// the next possible cause may be an timing or math issue                      

if (m_H264Decoder.m_CharIndex >= 16384 && m_runtimelog == false )
                    {
                        CString bufferMMAL = m_H264Decoder.m_DebugCharArray;
                        filesystem_save_log_file( "emmc1-1", "MMAL_RUN.TXT", bufferMMAL);
                        m_runtimelog = true;
                    }
                    gfx_render_shader_a(&state);

                    gl_current_prg = util_choose_program();
                    gl_current_tex = util_choose_texture();

                    util_store_program();

                    io_read_ADC();

                    m_Button_A.Update ();
                    m_Button_B.Update ();
                    m_ChipSelectPin.Write(adc_pot_routing); // change to occationally call, not each u_time 

                    switch (mode_storage_buffers[FLAG_TIME][current_buffer])
                        {
                        case true:  GLtime = output_int_value[4]/36.0f;                     break;
                        case false: GLtime = start_time_fps_calculation / 1000000.0f;       break;
                        }
                    util_random_vec8(start_time_fps_calculation);

// can one of this functions / calculations can cause a crash, especally at the very same system time since the actual gl code has variable framerates?                     
 
                    menu_general();
                    
                    menu_mode_default_A();
                    menu_mode_default_B();
                    menu_mode_assign_A();
                    menu_mode_assign_B();
                    menu_mode_assign_LFO();

                    display_append_modes();

                    LED_update();

                    util_calculate_BPM(m_Button_B.GetLastEventTime(), current_time_for_bpm);
                    util_update_predicted_beat();
                    util_determine_bpm_source();
                    util_LFO();                 

// i like to do it as function !!!
                    // FPS limiting using previous swap time prediction
                    currentTime = m_Timer.GetClockTicks();
                    targetTime = start_time_fps_calculation + (1000000 / TARGET_FPS);
                    if (noTargetFPS == true && targetTime > (currentTime + last_swap_duration))
                        {
                        delayTime = (targetTime - (currentTime + last_swap_duration)) / 1000;
                        m_Timer.MsDelay(delayTime);
                        }
                    // Measure swap time and calculate FPS
                    unsigned long before_swap = m_Timer.GetClockTicks();
                    gfx_render_shader_b(&state);
                    last_swap_duration = m_Timer.GetClockTicks() - before_swap;
                    // Calculate actual FPS including everything
                    unsigned long total_frame_time = m_Timer.GetClockTicks() - start_time_fps_calculation;
                    current_fps = 1000000.0f / total_frame_time;
                }
                return ShutdownReboot;
}


