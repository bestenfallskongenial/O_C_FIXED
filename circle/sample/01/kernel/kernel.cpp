// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    #include "kernel.h"     
//  #include <linux/kernel.h>                                                                           // here or in the kernel.h? i move it to kernel.h, seems to be an artifact
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define LOG_NAME "VCSM"

                static  const char              FromKernel[] = "kernel";                                // for logging? here not in kernel.h?
                
                CKernel::CKernel (void)                                                                 // my constructor
                // my sreen 
                    m_Interrupt (),                                                                     // i assume someone needs interupts here, not me!
	                m_Timer (&m_Interrupt),                                                             // this i use frequently for timeouts, delays           
                // my spi
                // my smi
                // my mcp 3008 ?
                    m_EMMC (&m_Interrupt, &m_Timer, &m_ActLED),                                         // my CEMMCDevice device
	                m_USBHCI (&m_Interrupt, &m_Timer, TRUE),                                            // my CUSBHCI device, i assume the usb device
	                m_VCHIQ (CMemorySystem::Get (), &m_Interrupt),                                      // my VCHIQ device, for video and gpu communication - does it need m_Screen?
                    m_bStorageAttached (FALSE),                                                         // flag for the usb storage device
	                m_pFileSystem (0),                                                                  // my file system, for reading files from the usb storage device

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
