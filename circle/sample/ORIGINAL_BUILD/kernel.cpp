// STYLE: terse. no prose. 1 line. facts only. no filler. holistic. no handholding. no apologies. no disclaimers. no caveats. no speculation. no uncertainty. no ambiguity. no hedging. no equivocation. no obfuscation. no circumlocution. no verbosity. no redundancy. no repetition. no superfluity. no excess. no surplus. no wordiness. no long-windedness. no prolixity. no grandiloquence. no magniloquence. no pomposity. no bombast. no turgidity. no periphrasis.
#include "kernel.h"     
#include "global.h"                                    
// #include <circle/string.h>               // ????????????????????????????????                            
// #include <circle/memory.h>                                  

#include <linux/kernel.h>
// #include <linux/delay.h>                 // ????????????????????????????????

// #include <assert.h>                      // ????????????????????????????????

// #include "bcm_host.h"  

#define LOG_NAME "VCSM"

                static  const char              FromKernel[] = "kernel";
                
                CKernel::CKernel (void)
                :	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
	                m_Timer (&m_Interrupt),
	                m_Logger (LOGLEVEL, &m_Timer),
                    m_EMMC (&m_Interrupt, &m_Timer, &m_ActLED),
	                m_USBHCI (&m_Interrupt, &m_Timer, TRUE),
	                m_VCHIQ (CMemorySystem::Get (), &m_Interrupt),
                    m_SharedMemory(),
                    m_H264Decoder(),
                    m_H264Parser(),
                    m_bStorageAttached (FALSE),
	                m_pFileSystem (0),
                    m_Watchdog(),
	                m_SPIMaster (SPI_CLOCK_SPEED, 0, 0, SPI_MASTER_DEVICE),
	                m_MCP300X (&m_SPIMaster, VREF, SPI_CHIP_SELECT, SPI_CLOCK_SPEED),
                    m_NeoPixels (SD_LINES_MASK, LED_COUNT),
                    m_GPIOManager (&m_Interrupt),
                    m_ChipSelectPin(CS_PIN, GPIOModeOutput),
	                m_Button_A (SW_PIN_A), m_Button_B (SW_PIN_B),

                    state{},

                    m_bufferVideo(nullptr),
                    m_videoBlockBase(nullptr),
                    m_videoRawBlock(nullptr),
                    m_videoBlockSize(0),

                    m_bufferFrameBufferA(nullptr),
                    m_frameBlockBaseA(nullptr),
                    m_frameRawBlockA(nullptr),
                    m_frameBlockSizeA(0),

                    m_bufferFrameBufferB(nullptr),
                    m_frameBlockBaseB(nullptr),
                    m_frameRawBlockB(nullptr),
                    m_frameBlockSizeB(0),

                    m_bufferTexture(nullptr),

                    m_bufferKernel(nullptr),

                    m_bufferVshader(nullptr), 
                    m_bufferFshader(nullptr),

                    m_USBhasLoadOnes(false),                                // we load only one time from usb, sorry user!
                    m_resetFlag(false)
                    {
	                m_ActLED.Blink (5);	// show we are alive
                    }
                CKernel::~CKernel (void)
                    {
                    memory_clean_up(); 
                    }
boolean         CKernel::Initialize (void)
{
boolean bOK = TRUE;

                if (bOK)
                    {
                    bOK = m_Screen.Initialize(); // we need to initialize the screen before the logger, because the logger needs a target device and the screen is the default target device if no other is specified
                    }
                if (bOK)
                    {
                    bOK = m_Serial.Initialize(115200); // we need to initialize the serial port before the logger, because the logger needs a target device and the serial port is the default target device if no other is specified
                }   
                if (bOK)
                    {
                    CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
                    if (pTarget == 0)
                    {
                        pTarget = &m_Screen;
                    }
                    bOK = m_Logger.Initialize(pTarget);
                    }   
                if (bOK)
                {
                    bOK = m_Interrupt.Initialize();
                    }
                if (bOK)
                    {
                    bOK = display_startup_screen(&state); // we need to initialize the display before the timer, because the timer is used in the display initialization for the startup screen and if the timer is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    bOK = m_Timer.Initialize(); // we need to initialize the timer before the filesystem, because the filesystem uses the timer for delays and if the timer is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize(); // we need to initialize the emmc before the filesystem, because the filesystem uses the emmc for storage and if the emmc is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize(); // we need to initialize the USBHCI before the filesystem, because the filesystem uses the USBHCI for storage and if the USBHCI is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); // we need to call UpdatePlugAndPlay() after initializing the USBHCI to detect if a USB device is already connected at startup, because if we don't call it, the filesystem will not detect the USB device and if the user has a USB device connected at startup, it will cause a crash when the filesystem tries to access the USB device
                    m_Timer.MsDelay(1000);  // we need to delay after initializing the USBHCI to give it time to detect the USB device and if we don't delay, the filesystem will not detect the USB device and if the user has a USB device connected at startup, it will cause a crash when the filesystem tries to access the USB device
                    }
                if (bOK)
                    {
                    bOK = memory_allocate(); // we need to allocate memory before the VCHIQ, because the VCHIQ uses the memory for communication and if the memory is not allocated, it will cause a crash when the VCHIQ tries to access the memory
                    }
                if (bOK)
                    {
                    bOK = m_VCHIQ.Initialize(); // we need to initialize the VCHIQ before the shared memory, because the shared memory uses the VCHIQ for communication and if the VCHIQ is not initialized, it will cause a crash when the shared memory tries to access the VCHIQ
                    m_Timer.MsDelay(200);
                    }
                if (bOK)
                {
                    bcm_host_init();  // we need to initialize the bcm_host before the graphics, because the graphics uses the bcm_host for initialization and if the bcm_host is not initialized, it will cause a crash when the graphics tries to access the bcm_host
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    gfx_init_OGL(&state); // we need to initialize the graphics before the shared memory, because the shared memory uses the graphics for initialization and if the graphics is not initialized, it will cause a crash when the shared memory tries to access the graphics
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMInitialize(); // we need to initialize the shared memory before importing the memory, because we need to have the shared memory initialized to import the memory and if the shared memory is not initialized, it will cause a crash when we try to import the memory
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMimportMemory(m_videoBlockBase, m_videoBlockSize, 0); // we can do all vcsm imports in one bOK block, dont you think?
                    m_SharedMemory.VCSMLockMemory(0);
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMimportMemory(m_frameBlockBaseA, m_frameBlockSizeA, 1);
                    bOK = m_SharedMemory.VCSMLockMemory(1);
                }
                if (bOK)
                    {
                    bOK = m_SharedMemory.VCSMimportMemory(m_frameBlockBaseB, m_frameBlockSizeB, 2);
                    bOK = m_SharedMemory.VCSMLockMemory(2);
                    }
                if (bOK)
                    {
                    bOK = m_H264Parser.ParseInitialize(     m_videoBlockBase,
                                                            8,
                                                            TEX_SIZE,
                                                            8,
                                                            MAX_FRAMES,
                                                            VIDEO_WIDTH,
                                                            VIDEO_HEIGHT,
                                                            BASELINE_PROFILE,
                                                            41
                                                            );
                    }
                if (bOK)
                    {
                    bOK = m_H264Decoder.MMALinitialize(     m_SharedMemory.m_vc_handle[0],
                                                          /*m_SharedMemory.m_vc_pointer[0],*/
                                                            m_videoBlockSize,
                                                            m_SharedMemory.m_vc_handle[1],
                                                          /*m_SharedMemory.m_vc_pointer[1],*/
                                                            m_SharedMemory.m_vc_handle[2],
                                                          /*m_SharedMemory.m_vc_pointer[2],*/
                                                            m_frameBlockSizeA,
                                                            VIDEO_WIDTH,
                                                            VIDEO_HEIGHT,
                                                            state.display,
                                                            state.context
                                                            );
                    }
                if (bOK)
                    {
                    m_Watchdog.Start(TIMEOUT);
                    }
                if (bOK)
                    {
                    bOK = m_SPIMaster.Initialize();
                    }
                if (bOK)
                    {
                    m_ChipSelectPin.Write(LOW);
                    }
                if (bOK)
                    {
                    bOK = m_Button_A.Initialize();
                    m_Button_A.RegisterEventHandler(io_event_button_A, this);
                    }
                if (bOK)
                    {
                    bOK = m_Button_B.Initialize();
                    m_Button_B.RegisterEventHandler(io_event_button_B, this);
                    }

                return bOK;

                }

TShutdownMode   CKernel::Run(void)
{
            m_Timer.MsDelay(500);

                util_prep_parameters();
                
                const char* cursoroff = "\x1b[?25l";
                m_Screen.Write(cursoroff, strlen(cursoroff));

                if (util_check_for_update())    // Call util_check_for_update() before entering the loop this will become a menu layer entry later on
                    {
                    return ShutdownReboot;      // If the update was successful, proceed with reboot
                    }

                m_Timer.MsDelay(500);

                if( filesystem_mount("emmc1-1", 
                    SCANED_FILES_VSH,  VSH_LOADED_BYTES, VSH_SD,
                    SCANED_FILES_FSH,  FSH_LOADED_BYTES, FSH_SD,
                    SCANED_FILES_TEX,  TEX_LOADED_BYTES, TEX_SD,
                    SCANED_FILES_VID,  VID_LOADED_BYTES, VID_SD))
                    {                   
                    // Flush CPU->RAM so the VPU sees the loaded bitstream
                    CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // we need to flush the cache after loading the video bitstream to make sure that the VPU can see the updated data in memory, otherwise it may cause a crash when the VPU tries to access the video data and if the video data is not updated in memory, it will cause a crash when the VPU tries to access the video data

                    gfx_init_v_buffer(&state);

                    parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
                    parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);
    
                    gfx_init_vshaders(&state, VSH_LOADED_OLD, VSH_LOADED_NEW);
                    gfx_init_fshaders(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                    gfx_init_programs(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                    gfx_init_uniforms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                //  gfx_init_textures(&state, TEX_LOADED_OLD, TEX_LOADED_NEW); // we commented out the texture loading for the test of the h264 decoder to texture mechanism, but we need to load the textures at least one time for the brandlogo and the ui, so we load them here from the emmc, but we will not load them again from the usb later, so that we can test the h264 decoder to texture mechanism without interference of the usb loading

                    m_Watchdog.Start(TIMEOUT);

                    m_Timer.MsDelay(100);
 
                    VSH_LOADED_OLD = VSH_LOADED_NEW;    // also 0 at bytesize calc beacuse i loaded only here
                    FSH_LOADED_OLD = FSH_LOADED_NEW;
                    TEX_LOADED_OLD = TEX_LOADED_NEW;    // we need to reset the couter casues issues later at byte size calculation
                    VID_LOADED_OLD = VID_LOADED_NEW;
                    }
                    m_Timer.MsDelay(500);
                    CString bufferVCSM = m_SharedMemory.m_DebugCharArray;
                    CString bufferMMAL =  m_H264Decoder.m_DebugCharArray;

                    filesystem_save_log_file( "emmc1-1", FILENAME_VCSM_LOG, bufferVCSM);    // this is for debug purposes because vc4 is a blackbox we are reverse-engineering, so we need to have a log of the vcsm calls and the mmal calls to see if we can find out what is causing the crash after ~ 22 minutes of runtime, because the theory is that there is an memory violation in the vcsm or mmal code that causes the crash, so we need to have a log of the vcsm and mmal calls to see if we can find out what is causing the crash, and if we can find out what is causing the crash, we can try to fix it or work around it, but for now we just want to have a log of the vcsm and mmal calls to see if we can find out what is causing the crash
                    filesystem_save_log_file( "emmc1-1", FILENAME_MMAL_LOG, bufferMMAL);

            m_H264Decoder.m_CharIndex = 0; // resest the logger for the decoder

                while (m_resetFlag == false)                                                  // mainloop
                    {
                    start_time_fps_calculation = m_Timer.GetClockTicks();                   // for the "fps break"
                    m_Watchdog.Start(8);

                    if(filesystem_update_USB("umsd1") == true && m_USBhasLoadOnes == false)         // checks and loads from usb
                        {
                    //  TEX_LOADED_OLD = 0;     // we need to reset the couter
                    //  TEX_LOADED_NEW = 0;     // that the brandlogo is removed

                        if( filesystem_mount("umsd1-1",                                                 // 1. scan the filenames!
                            SCANED_FILES_VSH, VSH_LOADED_BYTES, VSH_USB,
                            SCANED_FILES_FSH, FSH_LOADED_BYTES, FSH_USB,
                            SCANED_FILES_TEX, TEX_LOADED_BYTES, TEX_USB, 
                            SCANED_FILES_VID, VID_LOADED_BYTES, VID_USB))
                            {
                            // Flush CPU->RAM so the VPU sees the loaded bitstream
                            CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize);

                            parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
                            parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

                            gfx_init_fshaders(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                            gfx_init_programs(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                            gfx_init_uniforms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                        //  gfx_init_textures(&state, TEX_LOADED_OLD, TEX_LOADED_NEW);  // outcommentend for the test of the h264 decoder to texture mechanism

                            filesystem_save_log_file( "umsd1-1", FILENAME_GL_LOG, g_log_string);

                            m_USBhasLoadOnes = true;
                            m_Timer.MsDelay(100);
                            }
                        }

                    m_H264Decoder.MMALFramePoller( m_H264Parser.m_frameOffset[0][100],m_H264Parser.m_framelenght[0][100]);

//  i still need to figure out if / how this function causes a crash on ~ 22 minutes of runtime!
// the theory for now is that the internal debug mechanism keeps filling up an array until an memory violation
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

                    switch (mode_storage_buffers[FRM_MODE][current_buffer])
                        {
                        case true:  GLtime = output_int_value[4]/36.0f;                     break;
                        case false: GLtime = start_time_fps_calculation / 1000000.0f;       break;
                        }
                    util_random_vec8(start_time_fps_calculation);

// can one of this functions / calculations can cause a crash? especally at the very same system time since the actual gl code has variable framerates? i obserevd an error after about 22 minutes                
 
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

                    if ( is_hold_for_4_sec_a && is_hold_for_4_sec_b ) m_resetFlag = true;
// i like to do the fps/framerate as function !!!
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
                util_save_modes_file();    
                filesystem_save_log_file( "umsd1-1", FILENAME_MODES, g_modes);
                check();                                                                        // one last check?

                filesystem_save_log_file( "umsd1-1", FILENAME_GLERROR, g_error_string);             // anyway - store the logfile!

                return ShutdownReboot;
}


