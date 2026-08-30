***depends on the ( later i hope obsolete ) files in

    circle/addon/vc4/h264_decoder 
    circle/addon/vc4/h264_parser
    circle/addon/vc4/new_decoder
    circle/addon/vc4/temp
    circle/addon/vc4/vc_sm

    circle/addon/sensor

    circle/addon/WS28XX

    since 

    LIBS	= 	$(CIRCLEHOME)/lib/usb/libusb.a \
	  		...
			$(CIRCLEHOME)/addon/vc4/vc_sm/libvc_sm.a \
			$(CIRCLEHOME)/addon/vc4/h264_decoder/VersionA/libh264_decoder.a \
			$(CIRCLEHOME)/addon/vc4/h264_parser/VersionA/libh264parser.a \
            ...
	  		$(CIRCLEHOME)/addon/sensor/libsensor.a \
	  		$(CIRCLEHOME)/addon/WS28XX/libws28xx.a
	  