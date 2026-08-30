//----------------------------------------------------------------------------------------------------------------------------------------------------
// deterministic log and print
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeLog       (   char*       p_buffer, 
                                            u32&        index,
                                            const char* label,
                                            u32         p_value0, 
                                            u32         p_value1,
                                            u32         p_value2, 
                                            u32         p_value3)
{
                for (const char* p = label; *p; ++p)                    // always write the label
                    {
                    p_buffer[index++] = *p;
                    }
                if (p_value0 == EMPTYLOG &&                              // if all values are placeholders, finish
                    p_value1 == EMPTYLOG &&
                    p_value2 == EMPTYLOG &&
                    p_value3 == EMPTYLOG )
                        {
                        p_buffer[index++] = '\n';
                        p_buffer[index]   = '\0';
                        return;
                        }
                if (p_value0 != EMPTYLOG)                                 // write first value if valid
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
                if (p_value1 != EMPTYLOG)                                 // write second value if valid
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
                if (p_value2 != EMPTYLOG)                                 // write third value if valid
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
                if (p_value3 != EMPTYLOG)                                 // write fourth value if valid
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
                p_buffer[index++] = '\n';                                 // terminate line
                p_buffer[index]   = '\0';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::storeLogLong  (   char*       p_buffer,
                                u32&        index,
                                const char* p_string0, u32 p_value0,
                                const char* p_string1, u32 p_value1,
                                const char* p_string2, u32 p_value2,
                                const char* p_string3, u32 p_value3)
{
    for (const char* p = p_string0; *p; ++p)   // always write first label
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
    if (p_string1 != EMPTYSTR) // label 2
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
    if (p_string2 != EMPTYSTR) // label 3
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
    if (p_string3 != EMPTYSTR) // label 4
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeMsg       (   char*       p_buffer,
                                            u32&        index,
                                            const char* label,
                                            const void* tx_msg,
                                            u32         total_size)
{
                p_buffer[index++] = '\n';                                   // insert leading newline

                for (const char* p = label; *p; ++p)                    // copy label
                    {
                    p_buffer[index++] = *p;
                    }
                p_buffer[index++] = '\n';                                   // next line please

                const unsigned char* b = (const unsigned char*)tx_msg;  // hex dump, 16 bytes per line

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
                p_buffer[index++] = '\n';                                   // newline + terminator
                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::nextline(char* p_buffer,
                                   u32& index)
{
                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// glsl logging
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::shaderLog                  (   GLint       shader,
                                                        int         shaderIndex )
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
#endif  
                return success == GL_TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::programLog                 (   GLint       program,
                                                        int         program_index )
{
            //  int internal_index = 0;                     // i wonder, if and why we need it, was the indexing in the p_buffer incorrect? !!! DOUBLECHECK !!!
            //  if (program_index > 0)
            //      internal_index = program_index - 1;
                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program link status idx/success", (u32)program_index, (u32)success);
#endif  
            //  char name[27];
            //  strncpy(name, &m_bufferFsh[program_index][2], 26);
            //  name[26] = '\0';
#ifdef __DEBUG_LOG__ 
            //  storeMsg( MY_BUFFER, MY_INDEX, "Program short name", name, 26);
            //  storeMsg( MY_BUFFER, MY_INDEX, "Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
                storeLog( MY_BUFFER, MY_INDEX, "Program byte size", (u32)g_bytFsh[program_index]);
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
                    storeLog( MY_BUFFER, MY_INDEX, "Uniform idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
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
                    storeLog( MY_BUFFER, MY_INDEX, "Attribute idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Attribute name", aname, length);
#endif                      
                    }
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif  

                return success == GL_TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// #define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertion implementation
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
                    storeLog( MY_BUFFER, MY_INDEX, "=== Final System Status ticks/p_count ===", (u32)ticks, (u32)error_count);
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
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport x/y", (u32)viewport[0], (u32)viewport[1]);
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport w/h", (u32)viewport[2], (u32)viewport[3]);
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
                    storeLog( MY_BUFFER, MY_INDEX, "OpenGL Error err/ticks/line", (u32)error, (u32)ticks, (u32)line);
                    storeLog( MY_BUFFER, MY_INDEX, severity);
                    storeLog( MY_BUFFER, MY_INDEX, error_str);
                    storeLog( MY_BUFFER, MY_INDEX, file);
#endif   
                    error_count++;
                    if (error_count >= ERROR_THRESHOLD)
                        resetFlag = true;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::startupScreen(char* p_buffer, u32& index)
{
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

                /* just clean it up */    m_MachineInfo.FreeDMAChannel(dmaChannel);

                unsigned usbDelay       = m_Options.GetUSBPowerDelay();
                unsigned usbSpeed       = m_Options.GetUSBFullSpeed();

                storeLog( MY_BUFFER, MY_INDEX, "Machine Model");                        // text labels
                storeLog( MY_BUFFER, MY_INDEX, machineName);
                nextline(p_buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "SoC Name");
                storeLog( MY_BUFFER, MY_INDEX, socName);
                nextline(p_buffer, index);

                storeLog( MY_BUFFER, MY_INDEX, "Model Major    ", modelMajor);          // numeric values
                storeLog( MY_BUFFER, MY_INDEX, "Model Revision ", modelRevision);
                nextline(p_buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "RAM Size     MB", ramSize);
                storeLog( MY_BUFFER, MY_INDEX, "CPU Speed Mode ", cpuSpeedMode);
                nextline(p_buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "SoC Max Temp   ", socMaxTemp);
                nextline(p_buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "Clock CORE  MHz", coreClock);
                storeLog( MY_BUFFER, MY_INDEX, "Clock ARM   MHz", armClock);    
                storeLog( MY_BUFFER, MY_INDEX, "Clock EMMC  MHz", emmcClock, emmc2Clock, uartClock);
                storeLog( MY_BUFFER, MY_INDEX, "Clock EMMC2 MHz", emmcClock, emmc2Clock, uartClock);
                storeLog( MY_BUFFER, MY_INDEX, "Clock UART  MHz", emmcClock, emmc2Clock, uartClock);    
                nextline(p_buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "DMA Channel    ", dmaChannel);
                nextline(p_buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "USB Delay      ", usbDelay);
                storeLog( MY_BUFFER, MY_INDEX, "USB FullSpeed  ", usbSpeed);
                
                screen_clear_screen(0x00000000);

                screen_draw_buffer_segment  (
                                            p_buffer,
                                            0,
                                            index,
                                            0,
                                            0,
                                            0xFFFFFFFF,
                                            0x00000000
                                            );

                return true;
}//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
