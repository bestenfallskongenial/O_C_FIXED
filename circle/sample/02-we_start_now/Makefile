#
# Makefile
#

CIRCLEHOME = ../..   													# root of Circle

# object files
OBJS = kernel.o datamangement.o graphics.o hardware.o logging.o menu.o parser.o util.o vc04_service.o wrappers.o stubs.o

# object files (one .cpp → one .o → must be listed here)
OBJS = kernel.o \
       kernel_initialize.o \
       kernel_run.o \
       datamangement.o \
       graphics.o \
       hardware.o \
       logging.o \
       menu.o \
       parser.o \
       util.o \
       vc04_service.o \
       wrappers.o \
       stubs.o
	   
# libraries
LIBS =  $(CIRCLEHOME)/lib/usb/libusb.a \
        $(CIRCLEHOME)/addon/SDCard/libsdcard.a \
        $(CIRCLEHOME)/lib/fs/fat/libfatfs.a \
        $(CIRCLEHOME)/lib/fs/libfs.a \
        $(CIRCLEHOME)/lib/libcircle.a \
        $(CIRCLEHOME)/lib/sched/libsched.a \
        $(CIRCLEHOME)/addon/vc4/interface/bcm_host/libbcm_host.a \
        $(CIRCLEHOME)/addon/vc4/interface/khronos/libkhrn_client.a \
        $(CIRCLEHOME)/addon/vc4/interface/vmcs_host/libvmcs_host.a \
        $(CIRCLEHOME)/addon/vc4/interface/vcos/libvcos.a \
        $(CIRCLEHOME)/addon/vc4/vchiq/libvchiq.a \
        $(CIRCLEHOME)/addon/linux/liblinuxemu.a

include $(CIRCLEHOME)/Rules.mk   										# build rules

-include $(DEPS)                										# optional deps