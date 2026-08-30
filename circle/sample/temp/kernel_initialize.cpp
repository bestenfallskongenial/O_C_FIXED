// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include kernel.h
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean         CKernel::Initialize (void)
{
                bool bOK = true;

                if (bOK)
                    {
                    bOK = m_Screen.Initialize(); // we need to initialize the screen before the logger, because the logger needs a target device and the screen is the default target device if no other is specified
                    /*
                    q: IS MY OWN BUFFER TO SCREEN SUFFICIENT TO REPLACE CSCREEN?
                     */
                    }
                if (bOK)
                    {
                    bOK = m_Serial.Initialize(115200); // we need to initialize the serial port before the logger, because the logger needs a target device and the serial port is the default target device if no other is specified
                    /*
                    Q: REALY? IS THIS THE ONLY REASON TO HAVE SERIAL HERE?
                    */
                    }   
                if (bOK) // the idea is to remove the CScreen Class form our code, to much overhead, also  the CString code !!!
                    {
                    CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
                    if (pTarget == 0)
                    {
                        pTarget = &m_Screen;
                    }
                    bOK = m_Logger.Initialize(pTarget);  //  i want to get rid of this!
                    }   
                if (bOK)
                {
                    bOK = m_Interrupt.Initialize();
                    }
                if (bOK)
                    {
                    bOK = display_startup_screen(&m_glsl); 
                    }
                if (bOK)
                    {
                    bOK = m_Timer.Initialize(); 
                    }
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize(); 
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize(); 
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    m_Timer.MsDelay(1000);  
                    }
                if (bOK)
                    {
                    bOK = wrapperMemoryAllocate(); // we need to allocate memory before the VCHIQ
                    }
                if (bOK)
                    {
                    bOK = m_VCHIQ.Initialize(); // we need to initialize the VCHIQ before the shared memory, because the shared memory uses the VCHIQ for communication and if the VCHIQ is not initialized, it will cause a crash when the shared memory tries to access the VCHIQ
                    m_Timer.MsDelay(200);
                    }
                if (bOK)
                {
                    bcm_host_init();  // we need to initialize the bcm_host before the graphics
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    initOGL   (&m_glsl); // we need to initialize the graphics before the shared memory
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMInitialize(); 
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMimportMemory(m_videoBlockBase, m_videoBlockSize, 0); 
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
                if (bOK)// new only for the system texture, the overlay atlas, i guess i will rework the parser too, a dedicated bpm parser, one for h264 I WILL INCLUDE IT AGAIN INTO THE CKernel Class code!!!
                    {
                    bOK = m_H264SystemParser.ParseInitialize(   TEX_FILE_SIZE,           // max_tex_size
                                                                8,                  // max_textures
                                                                
                                                                m_videoBlockBase,   // aka max vid_size ??
                                                                8,                  // max_videos   h264
                                                                MAX_FRAMES,         // max_frames   h264
                                                                VIDEO_WIDTH,        // max_width    h264
                                                                VIDEO_HEIGHT,       // max_height   h264
                                                                BASELINE_PROFILE,   // max_profile  h264
                                                                41                  // max_level    h264
                                                            );
                    }                    
                if (bOK)
                    {
                    bOK = m_H264Parser.ParseInitialize      (   TEX_FILE_SIZE,                  // we will rework the parser into a dedicated bpm and one for h264 ( i still dont know what codes ) I WILL INCLUDE IT AGAIN INTO THE CKernel Class code!!!
                                                                8,
                                                                
                                                                m_videoBlockBase,
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
                    bOK = m_H264Decoder.MMALinitialize(     m_SharedMemory.m_vc_handle[0],      // means also that i will rework the decode, I WILL INCLUDE IT AGAIN INTO THE CKernel Class code!!!
                                                          /*m_SharedMemory.m_vc_pointer[0],*/   // still a blackbox,
                                                            m_videoBlockSize,                   // still about 90% finish
                                                            m_SharedMemory.m_vc_handle[1],
                                                          /*m_SharedMemory.m_vc_pointer[1],*/
                                                            m_SharedMemory.m_vc_handle[2],
                                                          /*m_SharedMemory.m_vc_pointer[2],*/
                                                            m_frameBlockSizeA,
                                                            VIDEO_WIDTH,
                                                            VIDEO_HEIGHT,
                                                            m_glsl.display,
                                                            m_glsl.context
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
                    /*
                    shall we create our own mechanism too?
                    */
                    }
                if (bOK)                                                                            // we will get rid of m_button in this version
                    {
                    bOK = m_Button_A.Initialize();
                    m_Button_A.RegisterEventHandler(io_event_button_A, this);
                    /*
                    should be not needed anymore, right
                    */
                    }
                if (bOK)
                    {
                    bOK = m_Button_B.Initialize();
                    m_Button_B.RegisterEventHandler(io_event_button_B, this);
                    }
                return bOK;
                }
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------