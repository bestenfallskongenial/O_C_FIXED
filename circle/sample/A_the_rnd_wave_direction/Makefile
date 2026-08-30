#
# Makefile
#

CIRCLEHOME = ../..

OBJS  = main.o \
        CLogger.o \
        kernel.o \
        kernel_initialize.o \
        kernel_run.o \
        code_alloc.o \
        code_debug.o \
        code_filesystem.o \
        code_graphics_init.o \
        code_graphics_log.o \
        code_graphics_run.o \
        code_hardware.o \
        code_log.o \
        code_menu.o \
        code_parser.o \
        code_user_file.o \
        code_util.o \
        code_vc_service.o \
        code_vc_mmal_api.o \
        code_vc_mmal_init.o \
        code_vc_vcsm.o \
        code_wrappers.o

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