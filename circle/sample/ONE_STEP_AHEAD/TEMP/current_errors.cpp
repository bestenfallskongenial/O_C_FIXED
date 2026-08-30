root@WIN-HS0BFGPCN4F:/home/bestenfalls/O_C_Rev_2.0/circle/sample/test_01# make
  CPP   main.o
  CPP   CLogger.o
  CPP   kernel.o
In file included from kernel.h:440,
                 from kernel.cpp:1:
defs_member.h: In constructor 'CKernel::CKernel()':
defs_member.h:37:37: warning: 'CKernel::m_SMITxDMA' will be initialized after [-Wreorder]
   37 |                 CDMAChannel         m_SMITxDMA;
      |                                     ^~~~~~~~~~
defs_member.h:11:45: warning:   'CEMMCDevice CKernel::m_EMMC' [-Wreorder]
   11 |                 CEMMCDevice                 m_EMMC;
      |                                             ^~~~~~
kernel.cpp:3:33: warning:   when initialized here [-Wreorder]
    3 |                                 CKernel::CKernel                                                (       void )
      |                                 ^~~~~~~
defs_member.h:69:33: warning: 'CKernel::m_bStorageAttached' will be initialized after [-Wreorder]
   69 |     volatile    bool            m_bStorageAttached  = false;
      |                                 ^~~~~~~~~~~~~~~~~~
defs_member.h:14:37: warning:   'CFATFileSystem* CKernel::m_pFileSystem' [-Wreorder]
   14 |                 CFATFileSystem*     m_pFileSystem;
      |                                     ^~~~~~~~~~~~~
kernel.cpp:3:33: warning:   when initialized here [-Wreorder]
    3 |                                 CKernel::CKernel                                                (       void )
      |                                 ^~~~~~~
  CPP   kernel_initialize.o
  CPP   kernel_run.o
  CPP   code_data.o
  CPP   code_graphics.o
  CPP   code_hardware.o
  CPP   code_logging.o
  CPP   code_menu.o
  CPP   code_parser.o
code_parser.cpp: In member function 'bool CKernel::parse264(h264_state*, char*, char**, char**, size_t*, int, int, u16, u16, u8, u8':
code_parser.cpp:112:24: warning: variable 'idr_len' set but not used [-Wunused-but-set-variable]
  112 |                 size_t idr_len[MAX_VIDEOS][MAX_FRAMES]       = {0};
      |                        ^~~~~~~
  CPP   code_stubs.o
  CPP   code_util.o
  CPP   code_wrappers.o
  CPP   code_debug.o
  LD    kernel.elf
  DUMP  kernel.lst
  COPY  kernel.img
  WC    kernel.img => 516564
root@WIN-HS0BFGPCN4F:/home/bestenfalls/O_C_Rev_2.0/circle/sample/test_01#