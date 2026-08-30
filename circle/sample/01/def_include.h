// kernel.h old





// ironically i could remove header files without problems but im still concerned about the correct order here and in kernel.cpp ( for example the memory system must be initialized before the file system, because of the dma buffers )


#include <vc4/vchiq/vchiqdevice.h>
#include "vc4/vchiq/vchiq_arm.h"

#include "bcm_host.h"





#include "kernel.h"

// kernel.h new
//----------------------------------------------------------------------------------------------------------------------------------------------------
// DEPENDENCIES
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include <circle/types.h>   

#include <circle/actled.h>                                  // onboard LED
#include <circle/koptions.h>                                // rasppi hardware options
#include <circle/machineinfo.h>     // dependency: CMachineInfo::Get()->GetClockRate()
#include <circle/devicenameservice.h>						
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>								// ????????????????????????????????


#include <circle/sched/scheduler.h>                         // multitasking ?!


#include <circle/logger.h>

#include <circle/bcm2835.h>       // dependency: ARM_IO_BASE
                                  // dependency: ARM_GPIO_GPFSEL0
                                  // dependency: ARM_GPIO_GPPUD
                                  // dependency: ARM_GPIO_GPPUDCLK0
                                  // dependency: ARM_GPIO_GPSET0
                                  // dependency: ARM_GPIO_GPCLR0
                                  // dependency: ARM_GPIO_GPLEV0
                                  // dependency: CLOCK_ID_CORE



#include <circle/synchronize.h>   // dependency: PeripheralEntry()
                                  // dependency: PeripheralExit()

#include <circle/timer.h>         // dependency: CTimer::SimpleusDelay()


#include <circle/dmachannel.h>


#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>

#include <circle/memory.h>										// new for dma allocated buffers!
#include <circle/new.h>												// new for dma allocated buffers!

#include "/home/bestenfalls/circle/addon/SDCard/emmc.h"
#include <circle/usb/usbhcidevice.h>
#include <circle/fs/fat/fatfs.h>
/*
#include <circle/bcmwatchdog.h> // new watchdog             // Look up hardware.cpp !!!
#include <circle/spimaster.h>
#include <sensor/mcp300x.h>
#include <WS28XX/ws2812oversmi.h> 
#include <circle/gpiomanager.h>
#include <circle/gpiopin.h>
*/
#include <string.h>                 // for memset()???


#include <cstdint>
#include <cstddef> // never seen, never used right?


#include "macros.h"

#include "vc04_service.h"
#include "vc04_defs.h"

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);

// VCSM and MMAL

#include <circle/types.h>                                                           // for u32, size_t, etc.

#include "VCSM_defs.h"
#include "MMAL_defs.h"

#include "interface/vcos/vcos.h"                // For VCHI_INSTANCE_T
#include "interface/vcos/vcos_event.h"          // ?
#include <vc4/interface/vcinclude/common.h>     // ?
#include <vc4/vchi/vchi.h>                      // For VCHI_CONNECTION_T
#include <vc4/vchiq/vchiq.h>


#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <GLES2/gl2.h>
#include <GLES/glext.h>
#include <GLES2/gl2ext.h>

