#include "kernel.h"

    #define MY_BFR   m_logBuffer                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logBufferIndex

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
                storeLogHex(    MY_BFR, MY_IDX, g_ScnFsh[shaderIndex], X_VAL, "buffer       [", (u32)shaderIndex, "] handle      ", (u32)shader );

                storeLogHex(    MY_BFR, MY_IDX, (shaderType == 0x00008B31) ? "Vertex       " : "Fragment     ", X_VAL, "Source Length ", (u32)sourceLength, "[Buffer] + '0'" );

                storeLogHex(    MY_BFR, MY_IDX, "Status ->    ", X_VAL, (success == GL_TRUE) ? "SUCCESS      " : "FAILED       ", X_VAL, log, X_VAL );

#ifdef __DUMP_GLSL__
                nextline(   MY_BFR, MY_IDX );

                storeLogHex(   MY_BFR, MY_IDX, "Source ->    ", X_VAL, source, X_VAL );
#endif

                nextline( MY_BFR, MY_IDX );
#endif

                return success == GL_TRUE;
}

bool            CKernel::programLog                 (   GLint       program,
                                                        int         program_index )
{
                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);
#ifdef __LOG_GLSL__
                storeLogHex(    MY_BFR, MY_IDX, g_ScnFsh[program_index], X_VAL, "Status Prg.  [", (u32)program_index, "] Size        ", (u32)g_bytFsh[program_index], (success == GL_TRUE) ? "Link SUCCESS  " : "Link FAILED   ", X_VAL); 
#endif 
                char log[1024];

                GLsizei logLength = 0;

                glGetProgramInfoLog(program, sizeof(log), &logLength, log);
#ifdef __LOG_GLSL__                 
                if (logLength > 0)
                    {
                    storeLogHex(    MY_BFR, MY_IDX, "Prg. InfoLog  ", X_VAL );
                    storeLogHex(    MY_BFR, MY_IDX, log, X_VAL );
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

                    storeLogHex(    MY_BFR, MY_IDX, "Uniform:  ", X_VAL, uname );
                    storeLogHex(    MY_BFR, MY_IDX, "index     ", (u32)i, "size      ", (u32)size, "type      ", (u32)type, "location  ", (u32)location);
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

                    storeLogHex(    MY_BFR, MY_IDX, "Attribute:", X_VAL, aname );
                    storeLogHex(    MY_BFR, MY_IDX, "index     ", (u32)i, "size      ", (u32)size, "type      ", (u32)type, "location  ", (u32)location );
#endif                     
                    }
#ifdef __LOG_GLSL__
                nextline(   MY_BFR, MY_IDX); 
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
#ifdef __LOG_GLSL__  
                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();

                    storeLogHex(    MY_BFR, MY_IDX, "*** Final System Status ticks", (u32)ticks, "/ Errorcount", (u32)error_count );
#endif  
                    GLint value;

                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
#ifdef __LOG_GLSL__                     
                    storeLogHex(    MY_BFR, MY_IDX, "Current Program", (u32)value);
#endif  
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
#ifdef __LOG_GLSL__                     
                    storeLogHex(    MY_BFR, MY_IDX, "Active Texture Unit", (u32)value);
#endif  
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
#ifdef __LOG_GLSL__                     
                    storeLogHex(    MY_BFR, MY_IDX, "Viewport x/", (u32)viewport[0], "y", (u32)viewport[1], "w", (u32)viewport[2], "h", (u32)viewport[3] );
#endif  
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
#ifdef __LOG_GLSL__                     
                    storeLogHex(    MY_BFR, MY_IDX, "Current Framebuffer", (u32)fb, "End Status Report ***" );
#endif                      
                    return;
                    }

                GLenum error = glGetError();

                while (error != GL_NO_ERROR)
                    {
#ifdef __LOG_GLSL__
                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();
   
                    const char* error_str;
                    const char* severity;
#endif

                    switch (error)
                        {
                        case GL_INVALID_ENUM:
#ifdef __LOG_GLSL__
                            error_str = "GL_INVALID_ENUM";
                            severity  = "WARNING";
#endif
                            break;

                        case GL_INVALID_VALUE:
#ifdef __LOG_GLSL__
                            error_str = "GL_INVALID_VALUE";
                            severity  = "WARNING";
#endif
                            break;

                        case GL_INVALID_OPERATION:
#ifdef __LOG_GLSL__
                            error_str = "GL_INVALID_OPERATION";
                            severity  = "WARNING";
#endif
                            break;

                        case GL_OUT_OF_MEMORY:
#ifdef __LOG_GLSL__
                            error_str = "GL_OUT_OF_MEMORY";
                            severity  = "CRITICAL";
#endif
                            m_resetFlag = true;
                            break;

                        case GL_INVALID_FRAMEBUFFER_OPERATION:
#ifdef __LOG_GLSL__
                            error_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                            severity  = "CRITICAL";
#endif
                            m_resetFlag = true;
                            break;

                        default:
#ifdef __LOG_GLSL__
                            error_str = "UNKNOWN_ERROR";
                            severity  = "WARNING";
#endif
                            break;
                        }
#ifdef __LOG_GLSL__ 
                    storeLogHex(    MY_BFR, MY_IDX, "OpenGL Error", (u32)error, "ticks", (u32)ticks, "line", (u32)line );
                    storeLogHex(    MY_BFR, MY_IDX, severity, X_VAL, error_str, X_VAL, file, X_VAL );
#endif  
                    error_count++;
                    
                    if (error_count >= ERROR_THRESHOLD) m_resetFlag = true;

                    error = glGetError();
                    }
}