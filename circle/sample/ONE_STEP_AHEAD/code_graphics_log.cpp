#include "kernel.h"

//  #define MY_BUFFER   m_bufferLog[1] //m_logBuffer
//  #define MY_INDEX    m_bufferLogIndex[1] //m_logBufferIndex

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex    

bool            CKernel::shaderLog                  (   GLint       shader,
                                                        int         shaderIndex )
{
                GLint success         = 0;
                GLint shaderType      = 0;
                GLint sourceLength    = 0;

                char log[1024];

#ifdef __DUMP_GLSL__
                char source[8096];
#endif

                glGetShaderiv(shader, GL_COMPILE_STATUS,       &success);
                glGetShaderiv(shader, GL_SHADER_TYPE,          &shaderType);
                glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceLength);

                glGetShaderInfoLog(shader, sizeof(log), 0, log);

#ifdef __DUMP_GLSL__
                glGetShaderSource(shader, sizeof(source), 0, source);
#endif

#ifdef __LOG_GLSL__
                storeLog(   MY_BUFFER, MY_INDEX,
                            g_ScnFsh[shaderIndex],  EMPTYLOG,
                            "buffer       [",       (u32)shaderIndex,
                            "] handle      ",       (u32)shader );

                storeLog(   MY_BUFFER, MY_INDEX,
                            (shaderType == 0x00008B31) ? 
                            "Vertex       " : 
                            "Fragment     ",        EMPTYLOG,
                            "Source Length ",       (u32)sourceLength, 
                            "[Buffer] + '0'" );

                storeLog(   MY_BUFFER, MY_INDEX,
                            "Status ->    ",        EMPTYLOG,
                            (success == GL_TRUE) ? 
                            "SUCCESS      " : 
                            "FAILED       ",        EMPTYLOG,
                            log,                    EMPTYLOG );

#ifdef __DUMP_GLSL__
                nextline(   MY_BUFFER, MY_INDEX );

                storeLog(   MY_BUFFER, MY_INDEX,
                            "Source ->    ",        EMPTYLOG,
                            source,                 EMPTYLOG );
#endif

                nextline( MY_BUFFER, MY_INDEX );
#endif

                return success == GL_TRUE;
}

bool            CKernel::programLog                 (   GLint       program,
                                                        int         program_index )
{
                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);
#ifdef __LOG_GLSL__
                storeLog(   MY_BUFFER, MY_INDEX, 
                            g_ScnFsh[program_index],    EMPTYLOG, 
                            "Status Prg.  [",           (u32)program_index, 
                            "] Size        ",           (u32)g_bytFsh[program_index], 
                            (success == GL_TRUE) ? 
                            "Link SUCCESS  " : "Link FAILED   ", EMPTYLOG); 
#endif 
                char log[1024];
                GLsizei logLength = 0;
                glGetProgramInfoLog(program, sizeof(log), &logLength, log);
#ifdef __LOG_GLSL__                 
                if (logLength > 0)
                    {
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Prg. InfoLog  ",       EMPTYLOG );
                    storeLog(   MY_BUFFER, MY_INDEX,
                                log,                    EMPTYLOG );
                    }
#endif 
                GLint numUniforms;
                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

                for (GLint i = 0; i < numUniforms; ++i)
                    {
                    char uname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(program, i, sizeof(uname), &length, &size, &type, uname);
#ifdef __LOG_GLSL__ 
                    GLint location = glGetUniformLocation(program, uname);

                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Uniform:      ", EMPTYLOG, uname );
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Uniform idx   ",       (u32)i, 
                                "size          ",       (u32)size, 
                                "type          ",       (u32)type, 
                                "location      ",       (u32)location);
#endif                     
                    }
                GLint numAttributes;
                glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

                for (GLint i = 0; i < numAttributes; ++i)
                    {
                    char aname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveAttrib(program, i, sizeof(aname), &length, &size, &type, aname);
#ifdef __LOG_GLSL__ 
                    GLint location = glGetAttribLocation(program, aname);

                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Attribute:    ", EMPTYLOG, aname );
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Attribute:    ",       (u32)i, 
                                "size          ",       (u32)size, 
                                "type          ",       (u32)type, 
                                "location      ",       (u32)location );
#endif                     
                    }
#ifdef __LOG_GLSL__
                nextline(   MY_BUFFER, MY_INDEX); 
#endif 

                return success == GL_TRUE;
}

void            CKernel::gfx_check                  (   const char* file, 
                                                        unsigned    line )
{
                static unsigned error_count = 0;
                static bool summary_written = false;
                const unsigned ERROR_THRESHOLD = 1024;

                if (m_resetFlag && !summary_written)
                    {
                    summary_written = true;

                    CTimer* pTimer = CTimer::Get();
#ifdef __LOG_GLSL__  
                    unsigned ticks = pTimer->GetTicks();

                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "System Status ticks",  (u32)ticks, 
                                "/ Errorcount",         (u32)error_count );
#endif  
                    GLint value;

                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
#ifdef __LOG_GLSL__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Current Program",      (u32)value);
#endif  
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
#ifdef __LOG_GLSL__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Active Texture Unit",  (u32)value);
#endif  
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
#ifdef __LOG_GLSL__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Viewport x/",          (u32)viewport[0], 
                                "y",                    (u32)viewport[1], 
                                "w",                    (u32)viewport[2], 
                                "h",                    (u32)viewport[3] );
#endif  
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
#ifdef __LOG_GLSL__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Current Framebuffer",  (u32)fb, 
                                "=== End Status Report ===" );
#endif                      
                    return;
                    }

                GLenum error = glGetError();

                while (error != GL_NO_ERROR)
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
                            m_resetFlag = true;
                            break;

                        case GL_INVALID_FRAMEBUFFER_OPERATION:
                            error_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                            severity  = "CRITICAL";
                            m_resetFlag = true;
                            break;

                        default:
                            error_str = "UNKNOWN_ERROR";
                            severity  = "WARNING";
                            break;
                        }
#ifdef __LOG_GLSL__ 
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "OpenGL Error",         (u32)error, 
                                "ticks",                (u32)ticks, 
                                "line",                 (u32)line );
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                severity,               EMPTYLOG, 
                                error_str,              EMPTYLOG, 
                                file,                   EMPTYLOG );
#endif  
                    error_count++;
                    
                    if (error_count >= ERROR_THRESHOLD) m_resetFlag = true;

                    error = glGetError();
                    }
}