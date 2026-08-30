// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    #include "kernel.h"     
//  #include <linux/kernel.h>                                                                           // here or in the kernel.h? i move it to kernel.h, seems to be an artifact
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define LOG_NAME "VCSM"

                static  const char              FromKernel[] = "kernel";                                // for logging? here not in kernel.h?
                
                CKernel::CKernel (void)                                                                 // my constructor
                :	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),                           // for char output or the global framebuffer?
                    m_Interrupt (),                                                                     // i assume someone needs interupts here, not me!
	                m_Timer (&m_Interrupt),                                                             // this i use frequently for timeouts, delays           
	                m_Logger (LOGLEVEL, &m_Timer),                                                      // for logging, but do i actually use or need it?
                    m_EMMC (&m_Interrupt, &m_Timer, &m_ActLED),                                         // my CEMMCDevice device
	                m_USBHCI (&m_Interrupt, &m_Timer, TRUE),                                            // my CUSBHCI device, i assume the usb device
	                m_VCHIQ (CMemorySystem::Get (), &m_Interrupt),                                      // my VCHIQ device, for video and gpu communication - does it need m_Screen?
                    m_SharedMemory(),                                                                   // my own VCSM implementation, for sharing memory between gpu and cpu
                    m_H264Decoder(),                                                                    // my H264Decoder, for decoding video
                    m_H264SystemParser(),                                                               // my H264SystemParser, for parsing the system media files 
                    m_H264Parser(),                                                                     // my H264SystemParser, for parsing the user media files
                    m_bStorageAttached (FALSE),                                                         // flag for the usb storage device
	                m_pFileSystem (0),                                                                  // my file system, for reading files from the usb storage device
                    m_Watchdog(),                                                                       // for resetting the system if something goes wrong
	                m_SPIMaster (SPI_CLOCK_SPEED, 0, 0, SPI_MASTER_DEVICE),                             // what again? the 3008 or the ws2812
	                m_MCP300X (&m_SPIMaster, ADC_VOLT_REF, SPI_CHIP_SELECT, SPI_CLOCK_SPEED),           // for reading the potentiometer
                    m_NeoPixels (SD_LINES_MASK, LED_COUNT),                                             // for controlling the ws2812 leds, but do i actually use it? 
                    m_GPIOManager (&m_Interrupt),                                                       // for direct read/write of pins ?
                    m_ChipSelectPin(CS_PIN, GPIOModeOutput),                                            // the same again?
	                m_Button_A (SW_PIN_A), m_Button_B (SW_PIN_B),                                       // the two buttons i have

                    m_glsl{},                                                                            // an instance of the glsl parameter struct - but i a) tend to use simple global variables or b) split m_glsl into more structs

                    m_bufferVid(nullptr),                                                               
                    m_videoBlockBase(nullptr),
                    m_videoRawBlock(nullptr),
                    m_videoBlockSize(0),

                    m_bufferFrA(nullptr),
                    m_frameBlockBaseA(nullptr),
                    m_frameRawBlockA(nullptr),
                    m_frameBlockSizeA(0),

                    m_bufferFrB(nullptr),
                    m_frameBlockBaseB(nullptr),
                    m_frameRawBlockB(nullptr),
                    m_frameBlockSizeB(0),

                    m_bufferTex(nullptr),
                    m_bufferKnl(nullptr),
                    m_bufferVsh(nullptr), 
                    m_bufferFsh(nullptr),

                    m_USBhasLoadOnes(false),                                                            // we load only one time from usb, sorry user!
                    m_resetFlag(false)                                                                  // determine if the main while loop runs
                    {
	                m_ActLED.Blink (5);	                                                                // blink blink - show we are alive can i remove it and m_ActLED ??
                    }
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------                    
                CKernel::~CKernel (void)                                                                // my deconstructor
                    {
                    wrapperMemoryCleanUp();                                                                  // despite the fact that i dont use it anyway dont i need also a glsl cleanup for example?
                    }
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
