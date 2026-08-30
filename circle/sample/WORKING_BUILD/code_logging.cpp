#include "kernel.h"

//  #undef  __DEBUG_LOG__
    #define __DEBUG_LOG__

    #define MY_BUFFER   m_logBuffer
    #define MY_INDEX    m_logBufferIndex

void            CKernel::storeLog                   (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* p_string0, u32 p_value0,
                                                        const char* p_string1, u32 p_value1,
                                                        const char* p_string2, u32 p_value2,
                                                        const char* p_string3, u32 p_value3)
{
                for (const char* p = p_string0; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }
                if (p_value0 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value0 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string1 != EMPTYSTR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string1; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value1 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value1 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string2 != EMPTYSTR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string2; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value2 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value2 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string3 != EMPTYSTR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string3; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value3 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value3 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';

                logScreenUpdate();
}

void            CKernel::storeMsg                   (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* label,
                                                        const void* tx_msg,
                                                        u32         total_size)
{
                p_buffer[index++]           = '\n';

                for (const char* p = label; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }
                p_buffer[index++]           = '\n';

                const unsigned char* b = (const unsigned char*)tx_msg;

                for (u32 i = 0; i < total_size; ++i) 
                    {
                    if (i && (i % 16) == 0)
                        {
                        p_buffer[index++]   = '\n';
                        }
                    unsigned char v         = b[i];

                    char hi = "0123456789ABCDEF"[v >> 4];
                    p_buffer[index++]       = hi;
                    char lo = "0123456789ABCDEF"[v & 0xF];
                    p_buffer[index++]       = lo;
                    p_buffer[index++]       = ' ';
                    }
                p_buffer[index++]           = '\n';
                p_buffer[index++]           = '\n';
                p_buffer[index]             = '\0';

                logScreenUpdate();
}

void            CKernel::nextline                   (   char*       p_buffer,
                                                        u32&        index)
{
                p_buffer[index++]           = '\n';
                p_buffer[index]             = '\0';

                logScreenUpdate();
}

bool            CKernel::startupScreen          (   void )
{
                const char* machineName =  m_MachineInfo.GetMachineName();
                const char* socName     =  m_MachineInfo.GetSoCName();

                unsigned modelMajor     =  m_MachineInfo.GetModelMajor();
                unsigned modelRevision  =  m_MachineInfo.GetModelRevision();
                unsigned ramSize        =  m_MachineInfo.GetRAMSize();

                unsigned cpuSpeedMode   = (m_Options.GetCPUSpeed() == CPUSpeedMaximum) ? 1 : 0;
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

                unsigned conWidth       =  m_Options.GetWidth();
                unsigned conHeight      =  m_Options.GetHeight();

                unsigned fbWidth        =  gE_FrameBuffer.GetWidth();
                unsigned fbHeight       =  gE_FrameBuffer.GetHeight();
                
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Machine Model  ", EMPTYLOG, 
                            machineName, EMPTYLOG );
                nextline(   MY_BUFFER, MY_INDEX);                            
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "SoC Name       ", EMPTYLOG, 
                            socName          , EMPTYLOG, 
                            "Model Major    ", modelMajor,
                            "Model Revision ", modelRevision);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "RAM Size     MB", ramSize);
                nextline(   MY_BUFFER, MY_INDEX);                
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "CPU Speed Mode ", cpuSpeedMode);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "SoC Max Temp   ", socMaxTemp);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Clock CORE  MHz", coreClock,
                            "Clock ARM   MHz", armClock);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Clock EMMC  MHz", emmcClock,
                            "Clock EMMC2 MHz", emmc2Clock);                            
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Clock UART  MHz", uartClock);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "DMA Channel    ", dmaChannel);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "USB Delay      ", usbDelay,
                            "USB FullSpeed  ", usbSpeed);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "CF Screen X    ", conWidth,
                            "CF Screen Y    ", conHeight);  

                storeLog(   MY_BUFFER, MY_INDEX, 
                            "FB Screen X    ", fbWidth,
                            "FB Screen Y    ", fbHeight);
         
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "gE Screen X    ", gE_ScreenWidth,
                            "gE Screen Y    ", gE_ScreenHeight);
                nextline(   MY_BUFFER, MY_INDEX);                            
/*
                bufferScreenClear();

                bufferScreenDraw(
                                        MY_BUFFER,
                                        0,
                                        MY_INDEX,
                                        0,
                                        0,
                                        0xFFFFFFFF
                                        );
*/
                return TRUE;
}
