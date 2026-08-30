***MEMORY ALLOCATION :

| type  | buffer member | alloc kind | SD count | USB count | total slices | size per slice | total reserved | location / source role    | purpose                                 |
| ----- | ------------- | ---------: | -------: | --------: | -----------: | -------------: | -------------: | ------------------------- | --------------------------------------- |
| VSH   | `m_bufferVsh` |        MEM |        1 |         0 |            1 |          32 KB |          32 KB | SD                        | vertex shader                           |
| OMF   | `m_bufferOmf` |        MEM |        1 |         0 |            1 |          32 KB |          32 KB | SD                        | overlay/menu fragment shader            |
| FSH   | `m_bufferFsh` |        MEM |        1 |        32 |           33 |          32 KB |        1056 KB | SD + USB                  | user fragment shaders                   |
| OMT   | `m_bufferOmt` |        DMA |        1 |         0 |            1 |        4096 KB |        4096 KB | SD                        | overlay texture atlas                   |
| TEX   | `m_bufferTex` |        DMA |        0 |         8 |            8 |        4096 KB |       32768 KB | USB                       | user textures                           |
| VID   | `m_bufferVid` |        DMA |        0 |         8 |            8 |        8192 KB |       65536 KB | USB                       | encoded video / H.264 input buffers     |
| KLN   | `m_bufferKnl` |        MEM |        1 |         1 |            2 |        2048 KB |        4096 KB | SD + USB                  | kernel fallback + USB update kernel     |
| FRM A | `m_bufferFrA` |        DMA |        2 |         0 |            2 |        1024 KB |        2048 KB | internal / decoder output | decoded frame buffer bank A             | <- changed to 1 not 2
| FRM B | `m_bufferFrB` |        DMA |        2 |         0 |            2 |        1024 KB |        2048 KB | internal / decoder output | decoded frame buffer bank B             |
| LOG   | `m_bufferLog` |        MEM |       24 |         0 |           24 |          64 KB |        1536 KB | internal logs             | startup, vc_sm, mmal, glsl, parser logs | <- too small, also do i combine parsing etc in one files


| buffer        |      total |
| ------------- | ---------: |
| `m_bufferVid` |      64 MB |
| `m_bufferFrA` |       2 MB |
| `m_bufferFrB` |       2 MB |
| `m_bufferOmt` |       4 MB |
| `m_bufferTex` |      32 MB |
| **DMA total** | **104 MB** |

***K-OPTION LOG:

Machine Model   Raspberry Pi Zero
SoC Name        BCM2835 Model Major     0x00000001 Model Revision  0x00000004

RAM Size     MB 0x00000200

CPU Speed Mode  0x00000000
SoC Max Temp    0x0000003C

Clock CORE  MHz 0x000000FA Clock ARM   MHz 0x000002BC
Clock EMMC  MHz 0x000000C8 Clock EMMC2 MHz 0x00000064
Clock UART  MHz 0x00000030

DMA Channel     0x00000006

USB Delay       0x00000000 USB FullSpeed   0x00000000

CF Screen X     0x00000000 CF Screen Y     0x00000000
FB Screen X     0x00000320 FB Screen Y     0x00000258
gE Screen X     0x00000000 gE Screen Y     0x00000000

CONSTUCTOR / INIT LOG:

emmc: Found a valid version 4.xx SD card
dwhci: No device connected to root port

***MEMORY ALLOCATION LOG:

ALLOC-DMA raw 0x005057E0 block 0x00506000
ALLOC-DMA slice 0x00000000 ptr 0x00506000 size 0x00800000
ALLOC-DMA slice 0x00000001 ptr 0x00D06000 size 0x00800000
ALLOC-DMA slice 0x00000002 ptr 0x01506000 size 0x00800000
ALLOC-DMA slice 0x00000003 ptr 0x01D06000 size 0x00800000
ALLOC-DMA slice 0x00000004 ptr 0x02506000 size 0x00800000
ALLOC-DMA slice 0x00000005 ptr 0x02D06000 size 0x00800000
ALLOC-DMA slice 0x00000006 ptr 0x03506000 size 0x00800000
ALLOC-DMA slice 0x00000007 ptr 0x03D06000 size 0x00800000
ALLOC-DMA raw 0x005057E0 block 0x00506000 total 0x04000000 aligned 0x04000000
ALLOC-DMA raw 0x04506860 block 0x04507000
ALLOC-DMA slice 0x00000000 ptr 0x04507000 size 0x00100000
ALLOC-DMA slice 0x00000001 ptr 0x04607000 size 0x00100000
ALLOC-DMA raw 0x04506860 block 0x04507000 total 0x00200000 aligned 0x00200000
ALLOC-DMA raw 0x047078E0 block 0x04708000
ALLOC-DMA slice 0x00000000 ptr 0x04708000 size 0x00100000
ALLOC-DMA slice 0x00000001 ptr 0x04808000 size 0x00100000
ALLOC-DMA raw 0x047078E0 block 0x04708000 total 0x00200000 aligned 0x00200000
ALLOC-DMA raw 0x04908960 block 0x04909000
ALLOC-DMA slice 0x00000000 ptr 0x04909000 size 0x00400000
ALLOC-DMA raw 0x04908960 block 0x04909000 total 0x00400000 aligned 0x00400000
ALLOC-DMA raw 0x04D099E0 block 0x04D0A000
ALLOC-DMA slice 0x00000000 ptr 0x04D0A000 size 0x00400000
ALLOC-DMA slice 0x00000001 ptr 0x0510A000 size 0x00400000
ALLOC-DMA slice 0x00000002 ptr 0x0550A000 size 0x00400000
ALLOC-DMA slice 0x00000003 ptr 0x0590A000 size 0x00400000
ALLOC-DMA slice 0x00000004 ptr 0x05D0A000 size 0x00400000
ALLOC-DMA slice 0x00000005 ptr 0x0610A000 size 0x00400000
ALLOC-DMA slice 0x00000006 ptr 0x0650A000 size 0x00400000
ALLOC-DMA slice 0x00000007 ptr 0x0690A000 size 0x00400000
ALLOC-DMA raw 0x04D099E0 block 0x04D0A000 total 0x02000000 aligned 0x02000000
ALLOC-MEM base 0x06D0AA60 count 0x00000002 size 0x00200000
ALLOC-MEM slice 0x00000000 ptr 0x06D0AAC0
ALLOC-MEM slice 0x00000001 ptr 0x06F0AAE0
ALLOC-MEM done 0x06D0AA60 count 0x00000002 size 0x00200000
ALLOC-MEM base 0x0710AB00 count 0x00000018 size 0x00010000
ALLOC-MEM slice 0x00000000 ptr 0x0710AF20
ALLOC-MEM slice 0x00000001 ptr 0x0711AF40
ALLOC-MEM slice 0x00000002 ptr 0x0712AF60
ALLOC-MEM slice 0x00000003 ptr 0x0713AF80
ALLOC-MEM slice 0x00000004 ptr 0x0714AFA0
ALLOC-MEM slice 0x00000005 ptr 0x0715AFC0
ALLOC-MEM slice 0x00000006 ptr 0x0716AFE0
ALLOC-MEM slice 0x00000007 ptr 0x0717B000
ALLOC-MEM slice 0x00000008 ptr 0x0718B020
ALLOC-MEM slice 0x00000009 ptr 0x0719B040
ALLOC-MEM slice 0x0000000A ptr 0x071AB060
ALLOC-MEM slice 0x0000000B ptr 0x071BB080
ALLOC-MEM slice 0x0000000C ptr 0x071CB0A0
ALLOC-MEM slice 0x0000000D ptr 0x071DB0C0
ALLOC-MEM slice 0x0000000E ptr 0x071EB0E0
ALLOC-MEM slice 0x0000000F ptr 0x071FB100
ALLOC-MEM slice 0x00000010 ptr 0x0720B120
ALLOC-MEM slice 0x00000011 ptr 0x0721B140
ALLOC-MEM slice 0x00000012 ptr 0x0722B160
ALLOC-MEM slice 0x00000013 ptr 0x0723B180
ALLOC-MEM slice 0x00000014 ptr 0x0724B1A0
ALLOC-MEM slice 0x00000015 ptr 0x0725B1C0
ALLOC-MEM slice 0x00000016 ptr 0x0726B1E0
ALLOC-MEM slice 0x00000017 ptr 0x0727B200
ALLOC-MEM done 0x0710AB00 count 0x00000018 size 0x00010000
ALLOC-MEM base 0x0728B220 count 0x00000001 size 0x00008000
ALLOC-MEM slice 0x00000000 ptr 0x0728B280
ALLOC-MEM done 0x0728B220 count 0x00000001 size 0x00008000
ALLOC-MEM base 0x0729B2A0 count 0x00000001 size 0x00008000
ALLOC-MEM slice 0x00000000 ptr 0x0729B300
ALLOC-MEM done 0x0729B2A0 count 0x00000001 size 0x00008000
ALLOC-MEM base 0x072AB320 count 0x00000021 size 0x00008000
ALLOC-MEM slice 0x00000000 ptr 0x072AB740
ALLOC-MEM slice 0x00000001 ptr 0x072BB760
ALLOC-MEM slice 0x00000002 ptr 0x072CB780
ALLOC-MEM slice 0x00000003 ptr 0x072DB7A0
ALLOC-MEM slice 0x00000004 ptr 0x072EB7C0
ALLOC-MEM slice 0x00000005 ptr 0x072FB7E0
ALLOC-MEM slice 0x00000006 ptr 0x0730B800
ALLOC-MEM slice 0x00000007 ptr 0x0731B820
ALLOC-MEM slice 0x00000008 ptr 0x0732B840
ALLOC-MEM slice 0x00000009 ptr 0x0733B860
ALLOC-MEM slice 0x0000000A ptr 0x0734B880
ALLOC-MEM slice 0x0000000B ptr 0x0735B8A0
ALLOC-MEM slice 0x0000000C ptr 0x0736B8C0
ALLOC-MEM slice 0x0000000D ptr 0x0737B8E0
ALLOC-MEM slice 0x0000000E ptr 0x0738B900
ALLOC-MEM slice 0x0000000F ptr 0x0739B920
ALLOC-MEM slice 0x00000010 ptr 0x073AB940
ALLOC-MEM slice 0x00000011 ptr 0x073BB960
ALLOC-MEM slice 0x00000012 ptr 0x073CB980
ALLOC-MEM slice 0x00000013 ptr 0x073DB9A0
ALLOC-MEM slice 0x00000014 ptr 0x073EB9C0
ALLOC-MEM slice 0x00000015 ptr 0x073FB9E0
ALLOC-MEM slice 0x00000016 ptr 0x0740BA00
ALLOC-MEM slice 0x00000017 ptr 0x0741BA20
ALLOC-MEM slice 0x00000018 ptr 0x0742BA40
ALLOC-MEM slice 0x00000019 ptr 0x0743BA60
ALLOC-MEM slice 0x0000001A ptr 0x0744BA80
ALLOC-MEM slice 0x0000001B ptr 0x0745BAA0
ALLOC-MEM slice 0x0000001C ptr 0x0746BAC0
ALLOC-MEM slice 0x0000001D ptr 0x0747BAE0
ALLOC-MEM slice 0x0000001E ptr 0x0748BB00
ALLOC-MEM slice 0x0000001F ptr 0x0749BB20
ALLOC-MEM slice 0x00000020 ptr 0x074ABB40
ALLOC-MEM done 0x072AB320 count 0x00000021 size 0x00008000

***LOADER LOG SD:

fatinfo: FAT32: 1946656 clusters of 16.0K

BULKLOAD Start max 0x00000001 valid 0x00000000 size 0x00008000
file 0x00000000 preset.vsh bytes read 0x00000466 in buffer 0x00000000
BULKLOAD End prev 0x00000000 new 0x00000001 loaded 0x00000001

BULKLOAD Start max 0x00000001 valid 0x00000000 size 0x00008000
file 0x00000000 preset.omf bytes read 0x00000BC5 in buffer 0x00000000
BULKLOAD End prev 0x00000000 new 0x00000001 loaded 0x00000001

BULKLOAD Start max 0x00000001 valid 0x00000000 size 0x00400000
file 0x00000000 preset00.omt bytes read 0x000697B6 in buffer 0x00000000
BULKLOAD End prev 0x00000000 new 0x00000001 loaded 0x00000001

BULKLOAD Start max 0x00000001 valid 0x00000000 size 0x00008000
file 0x00000000 color_~1.fsh bytes read 0x00000B0A in buffer 0x00000000
BULKLOAD End prev 0x00000000 new 0x00000001 loaded 0x00000001

BULKLOAD Start max 0x00000000 valid 0x00000000 size 0x00400000
BULKLOAD End prev 0x00000000 new 0x00000000 loaded 0x00000000

BULKLOAD Start max 0x00000000 valid 0x00000000 size 0x00800000
BULKLOAD End prev 0x00000000 new 0x00000000 loaded 0x00000000

BULKLOAD Start max 0x00000001 valid 0x00000000 size 0x00200000
file 0x00000000 kernel.img bytes read 0x00037428 in buffer 0x00000000
BULKLOAD End prev 0x00000000 new 0x00000001 loaded 0x00000001

***LOADER LOG USB:

fatinfo: FAT32: 1924864 clusters of 8.0K

BULKLOAD Start max 0x00000019 valid 0x00000001 size 0x00008000
file 0x00000000 color_~1.fsh bytes read 0x00000B0A in buffer 0x00000001
file 0x00000001 comple~1.fsh bytes read 0x00000AAC in buffer 0x00000002
file 0x00000002 f(10)_~1.fsh bytes read 0x000007DE in buffer 0x00000003
file 0x00000003 f(22)_~1.fsh bytes read 0x000005D8 in buffer 0x00000004
file 0x00000004 1misty~1.fsh bytes read 0x000007E4 in buffer 0x00000005
file 0x00000005 1triso~1.fsh bytes read 0x000005F0 in buffer 0x00000006
file 0x00000006 1vec4r~1.fsh bytes read 0x00000B4C in buffer 0x00000007
file 0x00000007 1xs2gd~1.fsh bytes read 0x00000948 in buffer 0x00000008
file 0x00000008 1xt2xr~1.fsh bytes read 0x00000680 in buffer 0x00000009
file 0x00000009 1z_cdi~1.fsh bytes read 0x0000070C in buffer 0x0000000A
file 0x0000000A 2thebe~1.fsh bytes read 0x000005F3 in buffer 0x0000000B
file 0x0000000B 03_tor~1.fsh bytes read 0x00000D83 in buffer 0x0000000C
file 0x0000000C 4getco~1.fsh bytes read 0x00000A95 in buffer 0x0000000D
file 0x0000000D 06tayl~1.fsh bytes read 0x00000D73 in buffer 0x0000000E
file 0x0000000E 6tl3fz~1.fsh bytes read 0x000011EC in buffer 0x0000000F
file 0x0000000F 7exito~1.fsh bytes read 0x00000F6E in buffer 0x00000010
file 0x00000010 bw_cir~1.fsh bytes read 0x00000ADE in buffer 0x00000011
file 0x00000011 bw_geo~1.fsh bytes read 0x000008F7 in buffer 0x00000012
file 0x00000012 bw_smo~1.fsh bytes read 0x000005A4 in buffer 0x00000013
file 0x00000013 bw_tri~1.fsh bytes read 0x00000565 in buffer 0x00000014
file 0x00000014 bw_tun~1.fsh bytes read 0x00000820 in buffer 0x00000015
file 0x00000015 calai_~1.fsh bytes read 0x0000068F in buffer 0x00000016
file 0x00000016 caleid~1.fsh bytes read 0x00000D07 in buffer 0x00000017
file 0x00000017 circle~1.fsh bytes read 0x0000065C in buffer 0x00000018
file 0x00000018 cloud_~1.fsh bytes read 0x00000CA9 in buffer 0x00000019
BULKLOAD End prev 0x00000001 new 0x0000001A loaded 0x00000019

BULKLOAD Start max 0x00000007 valid 0x00000000 size 0x00400000
file 0x00000000 user05.bmp bytes read 0x00300036 in buffer 0x00000000
file 0x00000001 user02.bmp bytes read 0x00030036 in buffer 0x00000001
file 0x00000002 user00.bmp bytes read 0x00030036 in buffer 0x00000002
file 0x00000003 user01.bmp bytes read 0x00030036 in buffer 0x00000003
file 0x00000004 user04.bmp bytes read 0x00300036 in buffer 0x00000004
file 0x00000005 sc01.bmp bytes read 0x00030036 in buffer 0x00000005
file 0x00000006 5e.bmp bytes read 0x000C0036 in buffer 0x00000006
BULKLOAD End prev 0x00000000 new 0x00000007 loaded 0x00000007

BULKLOAD Start max 0x00000002 valid 0x00000000 size 0x00800000
file 0x00000000 test-d.264 bytes read 0x0008918A in buffer 0x00000000
file 0x00000001 test-a.264 bytes read 0x0057A9D0 in buffer 0x00000001
BULKLOAD End prev 0x00000000 new 0x00000002 loaded 0x00000002

BULKLOAD Start max 0x00000000 valid 0x00000001 size 0x00200000
BULKLOAD End prev 0x00000001 new 0x00000001 loaded 0x00000000
