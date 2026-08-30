// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include kernel.h
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TShutdownMode   CKernel::Run(void)
{
                prepParameters();                                                     // we prep the parameters in the system arrays

                wrapper_load_sd();                                                     // we than load the files from sd

                wrapper_init_gl_sd();                                                     // we than init the glsl system and parse the media files from sd
/*
                CString bufferVCSM = m_SharedMemory.m_DebugCharArray;                       // we now use saveFromBufferD() instead of all CString operations!
                CString bufferMMAL =  m_H264Decoder.m_DebugCharArray;

                saveFromBuffer( "emmc1-1", VCSM_LOG_BUFFER, bufferVCSM, bufferIndexVCSM );        // this is for debug purposes because vc4 is a blackbox we are reverse-engineering, so we need to have a log of the vcsm calls and the mmal calls to see if we can find out what is causing the crash after ~ 22 minutes of runtime, because the theory is that there is an memory violation in the vcsm or mmal code that causes the crash, so we need to have a log of the vcsm and mmal calls to see if we can find out what is causing the crash, and if we can find out what is causing the crash, we can try to fix it or work around it, but for now we just want to have a log of the vcsm and mmal calls to see if we can find out what is causing the crash
                saveFromBuffer( "emmc1-1", VCSM_LOG_BUFFER, bufferMMAL);
*/
                while (m_resetFlag == false)                                                // the actual mainloop - we loop while the reset flag is false
                    {
                    if(updateUSB("umsd1") == true && m_USBhasLoadOnes == false) // usb attach detection - do we need the flag when we manage to a) clean up the gl system and the filesystem properly ( soft reset )
                        {
                        wrapper_init_usb();                                                 // if true we run the load from usb function
                        wrapper_init_gl_usb();                                              // we than init the glsl system and parse the media files from sd

//                      saveFromBuffer( "umsd1-1", FILENAME_GL_LOG, g_log_string);
                        }
                    start_time_fps_calculation = m_Timer.GetClockTicks();                   // for the "fps break" - shall we make this our main timestamp?

                    wrapper_io();
                    wrapper_modes();
                    wrapper_render();
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
okay, hight level breakdown

boot:

    constructor 
        hardware recourses
    initialize
        hardware driver
    run
    * store log files

        while loop <- as long as no reset event happens

        if ( !sd_has_load )
            load system files from sd <- really before the while loop?      -> filecounter[FT_COUNT][FLD_COUNT]
            parse and initialize system files ( textures / shader etc. )    -> glsl_state / tex_state
            * store log files                                               -> saveFromBufferM()
            sd_has_load = true  

    if ( !usb_has_load && usb_is_connected )                                <- check for usb device             
            load user files from usb                                        -> filecounter[FT_COUNT][FLD_COUNT]
            parse and initialize user files ( textures / shader etc. )      -> glsl_state / tex_state / h264_state
            * store log files
            usb_has_load = true

        read adc                                                            -> g_inOutMatrix*[CHANNEL][IO_TYPE_COUNT];

        check buttons                                                       -> g_buttons_states[NR_BUTTONS][BTN_INDEX_COUNT]
            * my simple button poller populates the  
        set menu layer
            * here is the logic hidden: we check the buttons and set the menu_layer accordingly like
                - layer 0 ( no button ) button b is tab bpm 
                - layer 1 ( button a pressed ) sets up the modes for ch 0-3
                - layer 2 ( button b pressed ) sets up the modes for ch 4-7
                - layer 3 ( button a hold and b pressed ones ) sets up the lfo
                - layer 4 ( button a hold and b pressed ones again ) sets up the the audio sensitivity
                - layer 5 ( button a hold and b pressed ones again ) sets up the extra modes ( assign the extra modes to actual channels )
                - layer 6 ( button a hold and b pressed ones again ) sets up the file operations ( load / store userdata, store logs, update fw )
        
    map modes                                                               -> modeMenuAssignGroup()

        set led
            * will update the color of the 4 leds according to 
                a) the mode 
                b) the value of the inOutMatrix 
                c) the menu layer
        render loop
            render pass 1 ( user shaders )
            if menu_is_active -> render overlay

*/