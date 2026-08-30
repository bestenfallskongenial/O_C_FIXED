    #include "CLogger.h"                        // my CLogger shimm layer <-because i have to redirect all other includes of CLogger !!
    //  #include <circle/logger.h>
    #include <stdlib.h>   // malloc, calloc, free
    #include <stdint.h>   // uintptr_t

    #include <circle/actled.h>
    #include <circle/koptions.h>
    #include <circle/machineinfo.h>
    #include <circle/devicenameservice.h>
    //  #include <circle/screen.h>
    //  #include <circle/serial.h>
    #include <circle/exceptionhandler.h>

    #include <circle/bcmframebuffer.h>
    #include <circle/chargenerator.h>

    #include <circle/interrupt.h> // outcommented in my woking version, probably because included elsewhere above? 
    #include <circle/timer.h>
    #include <circle/dmachannel.h>

    #include <circle/memory.h>										// new for dma allocated buffers!
    #include <circle/new.h>												// new for dma allocated buffers!

    #include "/home/bestenfalls/circle/addon/SDCard/emmc.h"

    #include <circle/usb/usbhcidevice.h>
    #include <circle/fs/fat/fatfs.h>

    #include "bcm_host.h"

    #include <circle/sched/scheduler.h>     // test this here!

    #include <vc4/vchiq/vchiqdevice.h>
    #include "vc4/vchiq/vchiq_arm.h"

    #include <EGL/egl.h>
    #include <EGL/eglext.h>

    #include <GLES/gl.h>
    #include <GLES/glext.h>

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

    #include <circle/types.h>
    #include <string.h>

