#include "kernel.h"

    #define MY_BFR   m_logKernel     
    #define MY_IDX    m_logKernelIndex    

void            CKernel::logButtonStatesRuntime( int row )
{
                char f_logBuffer[512];
                u32  f_logIndex = 0;

                f_logBuffer[0] = '\0';

                storeLogHex(   f_logBuffer, f_logIndex, "BTN PRESS_START   DOUBLE        RELEASE       SINGLE", X_VAL );
                storeLogHex(   f_logBuffer, f_logIndex, "A  ", (u32)g_buttons_states[0][BTN_PRESS_START], X_STR, (u32)g_buttons_states[0][BTN_DOUBLE], X_STR, (u32)g_buttons_states[0][BTN_RELEASE], X_STR, (u32)g_buttons_states[0][BTN_SINGLE] );
                storeLogHex(   f_logBuffer, f_logIndex, "A   HOLD_TICK", (u32)g_buttons_states[0][BTN_HOLD_TICK] );
                storeLogHex(   f_logBuffer, f_logIndex, "B  ", (u32)g_buttons_states[1][BTN_PRESS_START], X_STR, (u32)g_buttons_states[1][BTN_DOUBLE], X_STR, (u32)g_buttons_states[1][BTN_RELEASE], X_STR, (u32)g_buttons_states[1][BTN_SINGLE] );
                storeLogHex(   f_logBuffer, f_logIndex, "B   HOLD_TICK", (u32)g_buttons_states[1][BTN_HOLD_TICK] );

                bufferScreenDraw(   f_logBuffer, 0, f_logIndex, 0, row, 0xFFFFFFFF );
}

void            CKernel::logModesRuntime( int row )
{
                char f_logBuffer[512];
                u32  f_logIndex = 0;

                f_logBuffer[0] = '\0';

                storeLogHex(   f_logBuffer, f_logIndex, "MODES  00-03", (u32)g_centralModeBuffer[g_currentProgramBuffer][0], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][1], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][2], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][3] );
                storeLogHex(   f_logBuffer, f_logIndex, "MODES  04-07", (u32)g_centralModeBuffer[g_currentProgramBuffer][4], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][5], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][6], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][7] );
                storeLogHex(   f_logBuffer, f_logIndex, "LFOs   08-11", (u32)g_centralModeBuffer[g_currentProgramBuffer][8], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][9], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][10], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][11] );
                storeLogHex(   f_logBuffer, f_logIndex, "THRESH 12-15", (u32)g_centralModeBuffer[g_currentProgramBuffer][12], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][13], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][14], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][15] );
                storeLogHex(   f_logBuffer, f_logIndex, "SENS   16-19", (u32)g_centralModeBuffer[g_currentProgramBuffer][16], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][17], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][18], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][19] );
                storeLogHex(   f_logBuffer, f_logIndex, "INPUT  20-23", (u32)g_centralModeBuffer[g_currentProgramBuffer][20], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][21], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][22], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][23] );
                storeLogHex(   f_logBuffer, f_logIndex, "SYSTEM 24-27", (u32)g_centralModeBuffer[g_currentProgramBuffer][24], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][25], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][26], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][27] );
#ifdef USE_MIDI
                storeLogHex(   f_logBuffer, f_logIndex, "MIDI   28-31", (u32)g_centralModeBuffer[g_currentProgramBuffer][28], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][29], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][30], X_STR, (u32)g_centralModeBuffer[g_currentProgramBuffer][31] );
#endif
                bufferScreenDraw(   f_logBuffer, 0, f_logIndex, 0, row, 0xFFFFFFFF );
}

void            CKernel::logPickUpFlags( int row)
{
                char f_logBuffer[512];
                u32  f_logIndex = 0;

                f_logBuffer[0] = '\0';

                storeLogHex(    f_logBuffer, f_logIndex, "Flags mode     00-03", g_menuPickUpFlag[0], X_STR, g_menuPickUpFlag[1], X_STR, g_menuPickUpFlag[2], X_STR, g_menuPickUpFlag[3]);    
                storeLogHex(    f_logBuffer, f_logIndex, "Flags mode     04-07", g_menuPickUpFlag[4], X_STR, g_menuPickUpFlag[5], X_STR, g_menuPickUpFlag[6], X_STR, g_menuPickUpFlag[7]); 
                storeLogHex(    f_logBuffer, f_logIndex, "Flags lfo      08-11", g_menuPickUpFlag[8], X_STR, g_menuPickUpFlag[9], X_STR,  g_menuPickUpFlag[10], X_STR, g_menuPickUpFlag[11]); 
                storeLogHex(    f_logBuffer, f_logIndex, "Flags thresh   12-15", g_menuPickUpFlag[12], X_STR, g_menuPickUpFlag[13], X_STR, g_menuPickUpFlag[14], X_STR, g_menuPickUpFlag[15]);   
                storeLogHex(    f_logBuffer, f_logIndex, "Flags snes     16-19", g_menuPickUpFlag[16], X_STR, g_menuPickUpFlag[17], X_STR, g_menuPickUpFlag[18], X_STR, g_menuPickUpFlag[19]); 
                storeLogHex(    f_logBuffer, f_logIndex, "Flags input    20-23", g_menuPickUpFlag[20], X_STR, g_menuPickUpFlag[21], X_STR, g_menuPickUpFlag[22], X_STR, g_menuPickUpFlag[23]);
                storeLogHex(    f_logBuffer, f_logIndex, "Flags system   24-27", g_menuPickUpFlag[24], X_STR, g_menuPickUpFlag[25], X_STR, g_menuPickUpFlag[26], X_STR, g_menuPickUpFlag[27]);    

                bufferScreenDraw(   f_logBuffer, 0, f_logIndex, 0, row, 0xFFFFFFFF );
}

void            CKernel::logInOutRuntime(int row)
{
                char f_logBuffer[512];
                u32  f_logIndex = 0;

                f_logBuffer[0] = '\0';

                storeLogHex(    f_logBuffer, f_logIndex, "0 RAW", (u32)g_inOutMatrixInt[0][RAW], "VAL", (u32)g_inOutMatrixInt[0][VAL], "RND", (u32)g_inOutMatrixInt[0][RND], "OUT", (u32)g_inOutMatrixInt[0][OUT] );
                storeLogHex(    f_logBuffer, f_logIndex, "1 RAW", (u32)g_inOutMatrixInt[1][RAW], "VAL", (u32)g_inOutMatrixInt[1][VAL], "RND", (u32)g_inOutMatrixInt[1][RND], "OUT", (u32)g_inOutMatrixInt[1][OUT] );
                storeLogHex(    f_logBuffer, f_logIndex, "2 RAW", (u32)g_inOutMatrixInt[2][RAW], "VAL", (u32)g_inOutMatrixInt[2][VAL], "RND", (u32)g_inOutMatrixInt[2][RND], "OUT", (u32)g_inOutMatrixInt[2][OUT] );
                storeLogHex(    f_logBuffer, f_logIndex, "3 RAW", (u32)g_inOutMatrixInt[3][RAW], "VAL", (u32)g_inOutMatrixInt[3][VAL], "RND", (u32)g_inOutMatrixInt[3][RND], "OUT", (u32)g_inOutMatrixInt[3][OUT] );
                storeLogHex(    f_logBuffer, f_logIndex, "4 RAW", (u32)g_inOutMatrixInt[4][RAW], "VAL", (u32)g_inOutMatrixInt[4][VAL], "RND", (u32)g_inOutMatrixInt[4][RND], "OUT", (u32)g_inOutMatrixInt[4][OUT] );
                storeLogHex(    f_logBuffer, f_logIndex, "5 RAW", (u32)g_inOutMatrixInt[5][RAW], "VAL", (u32)g_inOutMatrixInt[5][VAL], "RND", (u32)g_inOutMatrixInt[5][RND], "OUT", (u32)g_inOutMatrixInt[5][OUT] );
                storeLogHex(    f_logBuffer, f_logIndex, "6 RAW", (u32)g_inOutMatrixInt[6][RAW], "VAL", (u32)g_inOutMatrixInt[6][VAL], "RND", (u32)g_inOutMatrixInt[6][RND], "OUT", (u32)g_inOutMatrixInt[6][OUT] );
                storeLogHex(    f_logBuffer, f_logIndex, "7 RAW", (u32)g_inOutMatrixInt[7][RAW], "VAL", (u32)g_inOutMatrixInt[7][VAL], "RND", (u32)g_inOutMatrixInt[7][RND], "OUT", (u32)g_inOutMatrixInt[7][OUT] );

                bufferScreenDraw(   f_logBuffer, 0, f_logIndex, 0, row, 0xFFFFFFFF );
}

void            CKernel::logInfosRuntime( int row )
{
                char f_logBuffer[512];
                u32  f_logIndex = 0;

                f_logBuffer[0] = '\0';

                storeLogHex(    f_logBuffer, f_logIndex, "g_menuLayer", g_menuLayer, "CTRL_PIN", GPIO_Read(CTRL_PIN), "bpm 0",  g_lfoBpmMatrix[0][BPM], "bpm 1", g_lfoBpmMatrix[1][BPM]);
                storeLogHex(    f_logBuffer, f_logIndex, "g_gl_program_current", g_gl_program_current, "g_currentProgramBuffer", g_currentProgramBuffer, "active texture", m_activeTex);
                storeLogHex(    f_logBuffer, f_logIndex, "filecounter[FT_FSH][FLD_LOADED]", filecounter[FT_FSH][FLD_LOADED], "filecounter[FT_FSH][FLD_VALID]", filecounter[FT_FSH][FLD_VALID]);
                storeLogHex(    f_logBuffer, f_logIndex, "filecounter[FT_TEX][FLD_LOADED]", filecounter[FT_TEX][FLD_LOADED], "filecounter[FT_TEX][FLD_VALID]", filecounter[FT_TEX][FLD_VALID]);
            //  storeLogHex(    f_logBuffer, f_logIndex, "mapping", ((g_inOutMatrixInt[ADC_SELECT_PRG][RAW] * (filecounter[FT_FSH][FLD_VALID]  ) ) >> 10) ); // (filecounter[FT_FSH][FLD_VALID] -1 )
#ifdef USE_MIDI
                storeLogHex(    f_logBuffer, f_logIndex, "MIDI", (u32)g_midiConnected );            
#endif
                bufferScreenDraw(   f_logBuffer, 0, f_logIndex, 0, row, 0xFFFFFFFF );                
}

bool            CKernel::startupScreen( void )
{
                const char* machineName =  m_MachineInfo.GetMachineName();
                const char* socName     =  m_MachineInfo.GetSoCName();

                unsigned modelMajor     =  m_MachineInfo.GetModelMajor();
                unsigned modelRevision  =  m_MachineInfo.GetModelRevision();
                unsigned ramSize        =  m_MachineInfo.GetRAMSize();

            //  unsigned cpuSpeedMode   = (m_Options.GetCPUSpeed() == CPUSpeedMaximum) ? 1 : 0;

                unsigned cpuSpeedMode   = m_CPUThrottle.GetClockRate() == m_CPUThrottle.GetMaxClockRate();

                unsigned socMaxTemp     =  m_Options.GetSoCMaxTemp();

                unsigned coreClock      =  m_MachineInfo.GetClockRate(CLOCK_ID_CORE)  / 1000000;
                unsigned armClock       =  m_MachineInfo.GetClockRate(CLOCK_ID_ARM)   / 1000000;
                unsigned emmcClock      =  m_MachineInfo.GetClockRate(CLOCK_ID_EMMC)  / 1000000;
                unsigned emmc2Clock     =  m_MachineInfo.GetClockRate(CLOCK_ID_EMMC2) / 1000000;
                unsigned uartClock      =  m_MachineInfo.GetClockRate(CLOCK_ID_UART)  / 1000000;

                unsigned dmaChannel     =  m_MachineInfo.AllocateDMAChannel(DMA_CHANNEL_NORMAL);

                                           m_MachineInfo.FreeDMAChannel(dmaChannel);
            
                unsigned usbDelay       =  m_Options.GetUSBPowerDelay();
                unsigned usbSpeed       =  m_Options.GetUSBFullSpeed();

                unsigned fbWidth        =  gE_FrameBuffer.GetWidth();
                unsigned fbHeight       =  gE_FrameBuffer.GetHeight();

                storeLogU32(    MY_BFR, MY_IDX, "--------------------------------------------------", X_VAL);
                storeLogU32(    MY_BFR, MY_IDX, "OCULUS MENDAX Firmware", OS_VERSION, ".", OS_REVISION ); 
                storeLogU32(    MY_BFR, MY_IDX, "--------------------------------------------------", X_VAL);
                storeLogU32(    MY_BFR, MY_IDX, machineName, X_VAL, socName, X_VAL, "V", modelMajor, ".", modelRevision);
                nextline(       MY_BFR, MY_IDX);
                storeLogU32(    MY_BFR, MY_IDX, "RAM Size       ", ramSize, "MB" ); 
                nextline(       MY_BFR, MY_IDX);                
            //  storeLogU32(    MY_BFR, MY_IDX, "CPU Speed Mode", cpuSpeedMode );
                storeLogU32(    MY_BFR, MY_IDX, "CPU Speed Mode ", X_VAL, cpuSpeedMode ? "Fast" : "Normal" );
                storeLogU32(    MY_BFR, MY_IDX, "SoC Max Temp   ", socMaxTemp, "Celsius" );
                nextline(       MY_BFR, MY_IDX);
                storeLogU32(    MY_BFR, MY_IDX, "Clock CORE", coreClock, "/ARM", armClock, "MHz" );
                storeLogU32(    MY_BFR, MY_IDX, "EMMC", emmcClock, "/EMMC2", emmc2Clock, "Mhz" );
                nextline(       MY_BFR, MY_IDX);
                storeLogU32(    MY_BFR, MY_IDX, "Clock UART     ", uartClock, "MHz" );
                storeLogU32(    MY_BFR, MY_IDX, "DMA Channels   ", dmaChannel );
                storeLogU32(    MY_BFR, MY_IDX, "USB Delay      ", usbDelay, "ms");
            //  storeLogU32(    MY_BFR, MY_IDX, "USB Speed", usbSpeed);
                storeLogU32(    MY_BFR, MY_IDX, "USB Speed      ", X_VAL, usbSpeed ? " 12 Mbps" : "480 Mbps" );
                nextline(       MY_BFR, MY_IDX);
                storeLogU32(    MY_BFR, MY_IDX, "FB Screen", fbWidth, "/", fbHeight, "Pixel" );  
                nextline(       MY_BFR, MY_IDX);

                storeLogU32(    MY_BFR, MY_IDX, "--------------------------------------------------", X_VAL);
                nextline(       MY_BFR, MY_IDX);                            

                return TRUE;
}

void CKernel::logInfoFrameRate(int row)
{
                char f_logBuffer[64];
                u32  f_logIndex = 0;

                f_logBuffer[0] = '\0';

                u32 value1000 = (u32)(g_currentFPS * 1000.0f + 0.5f);

                u32 whole = value1000 / 1000;
                u32 frac  = value1000 % 1000;

                char fracStr[5];

                fracStr[0] = '.';
                fracStr[1] = '0' + ((frac / 100) % 10);
                fracStr[2] = '0' + ((frac / 10)  % 10);
                fracStr[3] = '0' + ( frac        % 10);
                fracStr[4] = '\0';

                storeLogU32(f_logBuffer, f_logIndex, "FPS", whole, fracStr );
#ifdef __DEBUG_TIMING__
                storeLogU32(f_logBuffer, f_logIndex, "runtime us", g_runtimeDuration, "gl us", g_glDuration );
#endif
                bufferScreenDraw(f_logBuffer, 0, f_logIndex, 0, row, 0xFFFFFFFF);
}