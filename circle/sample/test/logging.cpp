#include "kernel.h"

void            CKernel::storeLog               (   char*       p_buffer,
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
}

void            CKernel::storeMsg                   (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* label,
                                                        const void* tx_msg,
                                                        u32         total_size)
{
                p_buffer[index++] = '\n';

                for (const char* p = label; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }
                p_buffer[index++] = '\n';

                const unsigned char* b = (const unsigned char*)tx_msg;

                for (u32 i = 0; i < total_size; ++i) 
                    {
                    if (i && (i % 16) == 0)
                        {
                        p_buffer[index++] = '\n';
                        }
                    unsigned char v = b[i];

                    char hi = "0123456789ABCDEF"[v >> 4];
                    p_buffer[index++] = hi;
                    char lo = "0123456789ABCDEF"[v & 0xF];
                    p_buffer[index++] = lo;
                    p_buffer[index++] = ' ';
                    }
                p_buffer[index++] = '\n';
                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';
}

void            CKernel::nextline                   (   char*       p_buffer,
                                                        u32&        index)
{
                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';
}
/*
bool            CKernel::shaderLog                  (   GLint       shader,
                                                        int         shaderIndex )
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx", (u32)shaderIndex, "success" ,(u32)success);
#endif 
                return success == GL_TRUE;
}

bool            CKernel::programLog                 (   GLint       program,
                                                        int         program_index )
{
                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX,  "Filename F-Shader", g_ScnFsh[program_index, 
                                                    "Program link status idx", (u32)program_index,
                                                    "Program byte size", (u32)g_bytFsh[program_index], 
                                                    "success", (u32)success);
#endif 
                char log[1024];
                glGetProgramInfoLog(program, sizeof(log), NULL, log);
#ifdef __DEBUG_LOG__                 
                storeMsg( MY_BUFFER, MY_INDEX, "Program InfoLog", log, sizeof(log));
#endif 
                GLint numUniforms;
                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Uniforms", (u32)numUniforms);
#endif 
                for (GLint i = 0; i < numUniforms; ++i)
                    {
                    char uname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(program, i, sizeof(uname), &length, &size, &type, uname);
                    GLint location = glGetUniformLocation(program, uname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Uniform idx", (u32)i, "size", (u32)size, "type", (u32)type, "loc", (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Uniform name", uname, length);
#endif                     
                    }

                GLint numAttributes;
                glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Attributes", (u32)numAttributes);
#endif 
                for (GLint i = 0; i < numAttributes; ++i)
                    {
                    char aname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveAttrib(program, i, sizeof(aname), &length, &size, &type, aname);
                    GLint location = glGetAttribLocation(program, aname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Attribute idx", (u32)i,"size", (u32)size,"type", (u32)type,"loc", (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Attribute name", aname, length);
#endif                     
                    }
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif 

                return success == GL_TRUE;
}

void            CKernel::gfx_check                  (   const char* file, 
                                                        unsigned    line )
{
                static unsigned error_count = 0;
                static bool summary_written = false;
                const unsigned ERROR_THRESHOLD = 1024;

                if (resetFlag && !summary_written)
                    {
                    summary_written = true;

                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();
#ifdef __DEBUG_LOG__  
                    storeLog( MY_BUFFER, MY_INDEX, "*** Final System Status ticks", (u32)ticks, "/ Errorcount", (u32)error_count);
#endif  
                    GLint value;

                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Program", (u32)value);
#endif  
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Active Texture Unit", (u32)value);
#endif  
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport x/", (u32)viewport[0], "y", (u32)viewport[1],"w", (u32)viewport[2], "h", (u32)viewport[3]);
#endif  
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Framebuffer", (u32)fb);
                    storeLog( MY_BUFFER, MY_INDEX, "=== End Status Report ===");
#endif                      
                    return;
                    }

                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                    {
                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();

                    const char* error_str;
                    const char* severity;

                    switch (error)
                        {
                        case GL_INVALID_ENUM:
                            error_str = "GL_INVALID_ENUM";
                            severity  = "WARNING";
                            break;

                        case GL_INVALID_VALUE:
                            error_str = "GL_INVALID_VALUE";
                            severity  = "WARNING";
                            break;

                        case GL_INVALID_OPERATION:
                            error_str = "GL_INVALID_OPERATION";
                            severity  = "WARNING";
                            break;

                        case GL_OUT_OF_MEMORY:
                            error_str = "GL_OUT_OF_MEMORY";
                            severity  = "CRITICAL";
                            resetFlag = true;
                            break;

                        case GL_INVALID_FRAMEBUFFER_OPERATION:
                            error_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                            severity  = "CRITICAL";
                            resetFlag = true;
                            break;

                        default:
                            error_str = "UNKNOWN_ERROR";
                            severity  = "WARNING";
                            break;
                        }
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "OpenGL Error", (u32)error, "ticks", (u32)ticks, "line", (u32)line);
                    storeLog( MY_BUFFER, MY_INDEX, severity, EMPTYLOG, error_str, EMPTYLOG, file);
#endif  
                    error_count++;
                    if (error_count >= ERROR_THRESHOLD)
                        resetFlag = true;
                    }
}
*/
boolean         CKernel::startupScreen          (   void )
{
            //  m_startupBufferIndex = 0;
            //  m_startupBuffer[0] = '\0';

                const char* machineName = m_MachineInfo.GetMachineName();
                const char* socName     = m_MachineInfo.GetSoCName();

                unsigned modelMajor     = m_MachineInfo.GetModelMajor();
                unsigned modelRevision  = m_MachineInfo.GetModelRevision();
                unsigned ramSize        = m_MachineInfo.GetRAMSize();

                unsigned cpuSpeedMode   = (m_Options.GetCPUSpeed() == CPUSpeedMaximum) ? 1 : 0;
                unsigned socMaxTemp     = m_Options.GetSoCMaxTemp();

                unsigned coreClock      = m_MachineInfo.GetClockRate(CLOCK_ID_CORE)  / 1000000;
                unsigned armClock       = m_MachineInfo.GetClockRate(CLOCK_ID_ARM)   / 1000000;
                unsigned emmcClock      = m_MachineInfo.GetClockRate(CLOCK_ID_EMMC)  / 1000000;
                unsigned emmc2Clock     = m_MachineInfo.GetClockRate(CLOCK_ID_EMMC2) / 1000000;
                unsigned uartClock      = m_MachineInfo.GetClockRate(CLOCK_ID_UART)  / 1000000;

                unsigned dmaChannel     = m_MachineInfo.AllocateDMAChannel(DMA_CHANNEL_NORMAL);
                m_MachineInfo.FreeDMAChannel(dmaChannel);

                unsigned usbDelay       = m_Options.GetUSBPowerDelay();
                unsigned usbSpeed       = m_Options.GetUSBFullSpeed();

                storeLog(m_startupBuffer, m_startupBufferIndex, "Machine Model", EMPTYLOG, machineName);

                storeLog(m_startupBuffer, m_startupBufferIndex, "SoC Name", EMPTYLOG, socName,
                                                                  	EMPTYLOG, "Model Major    ", modelMajor,
                                                                  	"Model Revision ", modelRevision);

                storeLog(m_startupBuffer, m_startupBufferIndex, "RAM Size     MB", ramSize);
                storeLog(m_startupBuffer, m_startupBufferIndex, "CPU Speed Mode", cpuSpeedMode);
                storeLog(m_startupBuffer, m_startupBufferIndex, "SoC Max Temperature", socMaxTemp);

                storeLog(m_startupBuffer, m_startupBufferIndex, "Clock CORE MHz", coreClock,
                                                                  	"Clock ARM MHz", armClock);

                storeLog(m_startupBuffer, m_startupBufferIndex, "Clock EMMC  MHz", emmcClock,
                                                                  	"EMMC2 MHz", emmc2Clock);

                storeLog(m_startupBuffer, m_startupBufferIndex, "Clock UART  MHz", uartClock);

                storeLog(m_startupBuffer, m_startupBufferIndex, "DMA Channel", dmaChannel);

                storeLog(m_startupBuffer, m_startupBufferIndex, "USB Delay", usbDelay,
                                                                  	"USB FullSpeed", usbSpeed);

                bufferToScreenClear();

                bufferToScreenDrawBuffer(
                                        m_startupBuffer,
                                        0,
                                        m_startupBufferIndex,
                                        0,
                                        0,
                                        0xFFFFFFFF
                                        );

                return TRUE;
}