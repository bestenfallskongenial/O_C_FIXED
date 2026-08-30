#
# Makefile
#

CIRCLEHOME = ../..   													# root of Circle

# object files
OBJS = kernel.o datamangement.o graphics.o hardware.o logging.o menu.o parser.o util.o vc04_service.o wrappers.o stubs.o

# object files (one .cpp → one .o → must be listed here)
OBJS = kernel.o \                # constructor / destructor
       kernel_initialize.o \     # CKernel_Initialize()
       kernel_run.o \            # CKernel_Run()
       datamangement.o \         # Memory / Filesystem module
       graphics.o \              # GLSL rendering / FBO
       hardware.o \              # low-level HW (GPIO/SPI/SMI/framebuffer)
       logging.o \               # logging / debug
       menu.o \                  # GUI / menu
       parser.o \                # file parsing (H264/BMP)
       util.o \                  # helpers / utility functions
       vc04_service.o \          # VC4 / VCSM / MMAL bridge
       wrappers.o \              # abstraction / wrapper layer
       stubs.o                   # fallback / stub implementations
	   
# libraries
LIBS =  $(CIRCLEHOME)/lib/usb/libusb.a \                  				# USB
        $(CIRCLEHOME)/addon/SDCard/libsdcard.a \          				# SD card
        $(CIRCLEHOME)/lib/fs/fat/libfatfs.a \             				# FAT
        $(CIRCLEHOME)/lib/fs/libfs.a \                    				# FS core
        $(CIRCLEHOME)/lib/libcircle.a \                   				# Circle base
        $(CIRCLEHOME)/lib/sched/libsched.a \              				# scheduler
        $(CIRCLEHOME)/addon/vc4/interface/bcm_host/libbcm_host.a \   	                # VC host
        $(CIRCLEHOME)/addon/vc4/interface/khronos/libkhrn_client.a \ 	                # GLES
        $(CIRCLEHOME)/addon/vc4/interface/vmcs_host/libvmcs_host.a \ 	                # VMCS
        $(CIRCLEHOME)/addon/vc4/interface/vcos/libvcos.a \           	                # VCOS
        $(CIRCLEHOME)/addon/vc4/vchiq/libvchiq.a \                   	                # VCHIQ
        $(CIRCLEHOME)/addon/linux/liblinuxemu.a                      	                # linux emu

include $(CIRCLEHOME)/Rules.mk   										# build rules

-include $(DEPS)                										# optional deps