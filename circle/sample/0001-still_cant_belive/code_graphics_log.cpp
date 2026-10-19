#include "kernel.h"

    #define MY_BFR   m_bufferLog[LOG_GLSL_0]                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_bufferLogIndex[LOG_GLSL_0] 

//  #define MY_BFR  m_bufferLog[LOG_GLSL_0]
//  #define MY_IDX  m_bufferLogIndex[LOG_GLSL_0]

bool            CKernel::shaderLog                  (   GLint       shader,
                                                        int         shaderIndex,
                                                        const char* fileName )
{
                GLint success      = 0;
                GLint shaderType   = 0;
                GLint sourceLength = 0;

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
                storeLogHex(    MY_BFR, MY_IDX, fileName, X_VAL, "buffer       [", (u32)shaderIndex, "] handle      ", (u32)shader );

                storeLogHex(    MY_BFR, MY_IDX, (shaderType == GL_VERTEX_SHADER) ? "Vertex       " : "Fragment     ", X_VAL, "Source Length ", (u32)sourceLength, "[Buffer] + '0'" );

                storeLogHex(    MY_BFR, MY_IDX, "Status ->    ", X_VAL, (success == GL_TRUE) ? "SUCCESS      " : "FAILED       ", X_VAL, log, X_VAL );

#ifdef __DUMP_GLSL__
                nextline(       MY_BFR, MY_IDX );

                storeLogHex(    MY_BFR, MY_IDX, "Source ->    ", X_VAL, source, X_VAL );
#endif

                nextline(       MY_BFR, MY_IDX );
#endif

                return success == GL_TRUE;
}

bool            CKernel::programLog                 (   GLint       program,
                                                        int         programIndex,
                                                        const char* fileName,
                                                        unsigned    fileSize )
{
                GLint success;

                glGetProgramiv(program, GL_LINK_STATUS, &success);

#ifdef __LOG_GLSL__
                storeLogHex(    MY_BFR, MY_IDX, fileName, X_VAL,
                                "Status Prg.  [", (u32)programIndex,
                                "] Size        ", (u32)fileSize,
                                (success == GL_TRUE) ? "Link SUCCESS  " : "Link FAILED   ",
                                X_VAL );
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
                    storeLogHex(    MY_BFR, MY_IDX, "index     ", (u32)i,
                                    "size      ", (u32)size,
                                    "type      ", (u32)type,
                                    "location  ", (u32)location );
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
                    storeLogHex(    MY_BFR, MY_IDX, "index     ", (u32)i,
                                    "size      ", (u32)size,
                                    "type      ", (u32)type,
                                    "location  ", (u32)location );
#endif
                    }

#ifdef __LOG_GLSL__
                nextline(           MY_BFR, MY_IDX );
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

                    if (error_count == 0)
                        {
                        GLint f_program = 0;
                        GLint f_framebuffer = 0;
                        GLint f_textureUnit = 0;
                        GLint f_texture2D = 0;
                        GLint f_textureExternal = 0;
                        GLint f_vertexBuffer = 0;

                        glGetIntegerv(GL_CURRENT_PROGRAM, &f_program);
                        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &f_framebuffer);
                        glGetIntegerv(GL_ACTIVE_TEXTURE, &f_textureUnit);
                        glGetIntegerv(GL_TEXTURE_BINDING_2D, &f_texture2D);
                        glGetIntegerv(GL_TEXTURE_BINDING_EXTERNAL_OES, &f_textureExternal);
                        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &f_vertexBuffer);

                        storeLogHex(    MY_BFR, MY_IDX,
                                        "Selected", (u32)g_gl_program_current,
                                        "Valid", (u32)filecounter[FT_FSH][FLD_VALID],
                                        "Select OUT", (u32)g_inOutMatrixInt[ADC_SELECT_PRG][OUT],
                                        "Mode buffer", (u32)g_currentProgramBuffer );

                        storeLogHex(    MY_BFR, MY_IDX,
                                        "GL program", (u32)f_program,
                                        "Conversion", (u32)m_csh.gl_program_id[0],
                                        "Overlay", (u32)m_osh.gl_program_id[0],
                                        "GL framebuffer", (u32)f_framebuffer );

                        storeLogHex(    MY_BFR, MY_IDX,
                                        "GL unit", (u32)f_textureUnit,
                                        "GL texture2D", (u32)f_texture2D,
                                        "GL external", (u32)f_textureExternal,
                                        "GL vertex buffer", (u32)f_vertexBuffer );

                        storeLogHex(    MY_BFR, MY_IDX,
                                        "Video source", (u32)m_tex.gl_tex_vid,
                                        "Video converted", (u32)m_cvt.gl_tex_id[0],
                                        "Backbuffer", (u32)m_tex.gl_tex_bfr,
                                        "Overlay atlas", (u32)m_omt.gl_tex_id[0] );

                        storeLogHex(    MY_BFR, MY_IDX,
                                        "Conversion FBO", (u32)m_cvt.gl_fbo_vid,
                                        "Vertex buffer", (u32)m_vtx.gl_buf,
                                        "Bitmap index", (u32)m_activeTex,
                                        "Valid bitmaps", (u32)filecounter[FT_TEX][FLD_VALID] );

                        storeLogHex(    MY_BFR, MY_IDX,
                                        "Kernel log index", (u32)m_logKernelIndex,
                                        "Kernel log size", (u32)sizeof(m_logKernel),
                                        "GLSL log index", (u32)m_bufferLogIndex[LOG_GLSL_0],
                                        "MMAL log index", (u32)m_bufferLogIndex[LOG_MMAL_0] );

                        if (g_gl_program_current >= 0 &&
                            g_gl_program_current < MAX_SHADER)
                            {
                            storeLogHex(    MY_BFR, MY_IDX,
                                            "User handle", (u32)m_fsh.gl_program_id[g_gl_program_current],
                                            "GL recognizes", (u32)glIsProgram(m_fsh.gl_program_id[g_gl_program_current]),
                                            "Audio location", (u32)m_fsh.u_aud[g_gl_program_current],
                                            "Time location", (u32)m_fsh.u_time[g_gl_program_current] );

                            storeLogHex(    MY_BFR, MY_IDX,
                                            "Video location", (u32)m_tex.u_tex_vid[g_gl_program_current],
                                            "Backbuffer location", (u32)m_tex.u_tex_bfr[g_gl_program_current],
                                            "tex6 location", (u32)m_tex.u_tex_id[g_gl_program_current][6],
                                            "tex7 location", (u32)m_tex.u_tex_id[g_gl_program_current][7] );
                            }

                        storeLogHex(    MY_BFR, MY_IDX,
                                        "Video index", (u32)m_activeVideo,
                                        "Valid videos", (u32)filecounter[FT_VID][FLD_VALID],
                                        "Frame index", (u32)m_activeFrame,
                                        "Frame capacity", (u32)MAX_FRAMES );

                        if (m_activeVideo >= 0 && m_activeVideo < MAX_VIDEOS)
                            {
                            storeLogHex(    MY_BFR, MY_IDX,
                                            "Frame count", (u32)m_vid.frame_count[m_activeVideo],
                                            "Video size", (u32)m_vid.size[m_activeVideo] );

                            if (m_activeFrame >= 0 && m_activeFrame < MAX_FRAMES)
                                {
                                storeLogHex(    MY_BFR, MY_IDX,
                                                "NAL offset", (u32)m_vid.nal_block_offset[m_activeVideo][m_activeFrame],
                                                "NAL length", (u32)m_vid.nal_block_length[m_activeVideo][m_activeFrame] );
                                }
                            }
                        }
#endif
                    error_count++;
                    
                    if (error_count >= ERROR_THRESHOLD) m_resetFlag = true;

                    error = glGetError();
                    }
}