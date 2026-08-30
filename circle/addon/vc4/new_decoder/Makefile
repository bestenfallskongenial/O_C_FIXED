#
# Makefile
#

CIRCLEHOME = ../../..

OBJS = api.o init.o helper.o runtime.o log.o

libh264_decoder.a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk

-include $(DEPS)