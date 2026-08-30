#
# Makefile
#

CIRCLEHOME = ../..

OBJS  = main.o \
        CLogger.o \
        kernel.o \
        kernel_initialize.o \
        kernel_run.o \
        code_data.o \
        code_graphics.o \
        code_hardware.o \
        code_logging.o \
        code_menu.o \
        code_parser.o \
        code_stubs.o \
        code_util.o \
        code_wrappers.o \
        code_debug.o

# libraries
LIBS =  $(CIRCLEHOME)/lib/usb/libusb.a \
        $(CIRCLEHOME)/lib/input/libinput.a \
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

include ../Rules.mk

-include $(DEPS)