//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                #include "kernel.h"
                #include "bcm_host.h" 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------      
void            CKernel::shaderLog(GLint shader, int shaderIndex)
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

                m_shaderStatusFlags[shaderIndex] = (success == GL_TRUE);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
#endif // __DEBUG_LOG__                 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::programLog(GLint shader, int program_index)
{
                int internal_index = 0;
                if (program_index > 0)
                    internal_index = program_index - 1;

                GLint success;
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                m_shaderStatusFlags[program_index] = (success == GL_TRUE);
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program link status idx/success", (u32)program_index, (u32)success);
#endif // __DEBUG_LOG__
                char name[27];
                strncpy(name, &m_bufferFsh[program_index][2], 26);
                name[26] = '\0';
#ifdef __DEBUG_LOG__ 
                storeMsg( MY_BUFFER, MY_INDEX, "Program short name", name, 26);
                storeMsg( MY_BUFFER, MY_INDEX, "Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
                storeLog( MY_BUFFER, MY_INDEX, "Program byte size", (u32)g_bytFsh[program_index]);
#endif // __DEBUG_LOG__
                char log[1024];
                glGetProgramInfoLog(shader, sizeof(log), NULL, log);
#ifdef __DEBUG_LOG__                 
                storeMsg( MY_BUFFER, MY_INDEX, "Program InfoLog", log, sizeof(log));
#endif // __DEBUG_LOG__
                GLint numUniforms;
                glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numUniforms);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Uniforms", (u32)numUniforms);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numUniforms; ++i)
                {
                    char uname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(shader, i, sizeof(uname), &length, &size, &type, uname);
                    GLint location = glGetUniformLocation(shader, uname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Uniform idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Uniform name", uname, length);
#endif // __DEBUG_LOG__                    
                }

                GLint numAttributes;
                glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &numAttributes);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Attributes", (u32)numAttributes);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numAttributes; ++i)
                {
                    char aname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveAttrib(shader, i, sizeof(aname), &length, &size, &type, aname);
                    GLint location = glGetAttribLocation(shader, aname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Attribute idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Attribute name", aname, length);
#endif // __DEBUG_LOG__                    
                }
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif // __DEBUG_LOG__                 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::gfx_check( const char* file, unsigned line )
{
                static unsigned error_count = 0;
                static bool summary_written = false;
                const unsigned ERROR_THRESHOLD = 1024;

                if (resetFlag && !summary_written)
                {
                    summary_written = true;

                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();

                    storeLog( MY_BUFFER, MY_INDEX, "=== Final System Status ticks/count ===", (u32)ticks, (u32)error_count);

                    GLint value;

                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Program", (u32)value);
#endif // __DEBUG_LOG__ 
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Active Texture Unit", (u32)value);
#endif // __DEBUG_LOG__ 
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport x/y", (u32)viewport[0], (u32)viewport[1]);
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport w/h", (u32)viewport[2], (u32)viewport[3]);
#endif // __DEBUG_LOG__ 
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Framebuffer", (u32)fb);
                    storeLog( MY_BUFFER, MY_INDEX, "=== End Status Report ===");
#endif // __DEBUG_LOG__                     
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
#endif // __DEBUG_LOG__ 
                    error_count++;
                    if (error_count >= ERROR_THRESHOLD)
                        resetFlag = true;
                }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initOGL      (   glsl_state* m_glsl )
{
                int32_t success = 0;
                EGLBoolean result;
                EGLint num_config;

                static EGL_DISPMANX_WINDOW_T nativewindow;

                DISPMANX_UPDATE_HANDLE_T dispman_update;
                VC_RECT_T dst_rect;
                VC_RECT_T src_rect;

                static const EGLint attribute_list[] =
                    {
                    EGL_RED_SIZE, 8,
                    EGL_GREEN_SIZE, 8,
                    EGL_BLUE_SIZE, 8,
                    EGL_ALPHA_SIZE, 8,
                    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                    EGL_NONE
                    };
   
                static const EGLint context_attributes[] = 
                    {
                    EGL_CONTEXT_CLIENT_VERSION, 2,
                    EGL_NONE
                    };

                EGLConfig config;
                
                m_glsl->display             = eglGetDisplay             (   EGL_DEFAULT_DISPLAY     );  // get an EGL display connection
#ifdef __OLG_DEBUG__                
assert(m_glsl->display!=EGL_NO_DISPLAY);
                check();
#endif // __OLG_DEBUG__
                result                      = eglInitialize             (   m_glsl->display,         // initialize the EGL display connection
                                                                            NULL, 
                                                                            NULL                    );
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
                check();
#endif // __OLG_DEBUG__
                result                      = eglChooseConfig           (   m_glsl->display,         // get an appropriate EGL frame buffer configuration 
                                                                            attribute_list, 
                                                                            &config, 
                                                                            1, 
                                                                            &num_config             );
#ifdef __OLG_DEBUG__                
assert(EGL_FALSE != result); //?
                check();
#endif // __OLG_DEBUG__
                result                      = eglBindAPI                (   EGL_OPENGL_ES_API);                                         // get an appropriate EGL frame buffer configuration
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
                check();
#endif // __OLG_DEBUG__
                m_glsl->context             = eglCreateContext          (   m_glsl->display,         // create an EGL rendering context
                                                                            config, 
                                                                            EGL_NO_CONTEXT, 
                                                                            context_attributes      );
#ifdef __OLG_DEBUG__
assert(m_glsl->context!=EGL_NO_CONTEXT);//?
                check();
#endif // __OLG_DEBUG__
                success                     = graphics_get_display_size (   0 /* LCD */,            // create an EGL window surface
                                                                            &m_glsl->screen_width, 
                                                                            &m_glsl->screen_height   );
#ifdef __OLG_DEBUG__
assert( success >= 0 );
                check();
#endif // __OLG_DEBUG__
                dst_rect.x                  = 0;
                dst_rect.y                  = 0;
                dst_rect.width              = m_glsl->screen_width;
                dst_rect.height             = m_glsl->screen_height;
      
                src_rect.x                  = 0;
                src_rect.y                  = 0;
                src_rect.width              = m_glsl->screen_width << 16;
                src_rect.height             = m_glsl->screen_height << 16;        

                m_glsl->dispman_display     = vc_dispmanx_display_open  (   0 /* LCD */ );
                dispman_update              = vc_dispmanx_update_start  (   0 );
      
                m_glsl->dispman_element     = vc_dispmanx_element_add   (   dispman_update, 
                                                                            m_glsl->dispman_display,
                                                                            0/*layer*/, 
                                                                            &dst_rect, 
                                                                            0/*src*/,
                                                                            &src_rect, 
                                                                            DISPMANX_PROTECTION_NONE, /*protection*/
                                                                            0 /*alpha*/, 
                                                                            0 /*clamp*/, 
                                                                            DISPMANX_NO_ROTATE /*transform*/ );   // was 0/*transform*/ before DISPMANX_ROTATE_90 
      
                nativewindow.element        = m_glsl->dispman_element;
                nativewindow.width          = m_glsl->screen_width;
                nativewindow.height         = m_glsl->screen_height;

                vc_dispmanx_update_submit_sync                          (   dispman_update  );
#ifdef __OLG_DEBUG__
                check();
#endif // __OLG_DEBUG__            
                m_glsl->surface             = eglCreateWindowSurface    (   m_glsl->display, 
                                                                            config, 
                                                                            &nativewindow, 
                                                                            NULL            );
#ifdef __OLG_DEBUG__
assert(m_glsl->surface != EGL_NO_SURFACE);//?
                check();
#endif // __OLG_DEBUG__                
                result                      = eglMakeCurrent            (   m_glsl->display,     // connect the context to the surface
                                                                            m_glsl->surface, 
                                                                            m_glsl->surface, 
                                                                            m_glsl->context  );
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
                check();
#endif // __OLG_DEBUG__
                                              glClearColor              (   0.15f, 0.25f, 0.35f, 1.0f);                                    // Set background color and clear buffers
                                              glClear                   (   GL_COLOR_BUFFER_BIT );
#ifdef __OLG_DEBUG__
                check();
#endif // __OLG_DEBUG__            
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initVshaders   (   glsl_state* m_glsl, int p_fromFile, int p_toFile)    // Function to initialize Vertex Shaders
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                    const char *SourcePrtVshader = m_bufferVsh[0]; // will be later maybe changed to multible instances of vshader

                    m_glsl->gl_vsh_id[i] = glCreateShader(GL_VERTEX_SHADER);
                    glShaderSource(m_glsl->gl_vsh_id[i], 1, &SourcePrtVshader, 0);  // will be later maybe changed to multible instances of vshader
                    glCompileShader(m_glsl->gl_vsh_id[i]);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initOshader    (   glsl_state* m_glsl, int p_fromFile, int p_toFile)    // Function to initialize Fragment Shaders                        <- *m_glsl should change too, right?
{
                    const char *SourcePrtFshader = m_bufferOmf[0]; // because this array is only [1] for consistency

                    m_glsl->gl_oms_id[0] = glCreateShader(GL_FRAGMENT_SHADER);       // gl_oms_id is new for the overlay shader
                    glShaderSource(m_glsl->gl_oms_id[0], 1, &SourcePrtFshader, 0);   // will m_glsl not also become a seperate struct here?!
                    glCompileShader(m_glsl->gl_oms_id[0]);
                    shaderLog(m_glsl->gl_oms_id[0], 1);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initFshaders   (   glsl_state* m_glsl, int p_fromFile, int p_toFile)    // Function to initialize Fragment Shaders                        <- we need a copy here for the overlay shader
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                    const char *SourcePrtFshader = m_bufferFsh[i];

                    m_glsl->gl_fsh_id[i] = glCreateShader(GL_FRAGMENT_SHADER);
                    glShaderSource(m_glsl->gl_fsh_id[i], 1, &SourcePrtFshader, 0);
                    glCompileShader(m_glsl->gl_fsh_id[i]);
                    shaderLog(m_glsl->gl_fsh_id[i], i);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initOprogram   (   glsl_state* m_glsl, int p_fromFile, int p_toFile)                                                                   // <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
                    if (m_shaderStatusFlags[0])                                     // we need to figure OUT from where this comes, we need also a separate method here
                        {
                        m_glsl->gl_omp_id[0] = glCreateProgram();
                        glAttachShader(m_glsl->gl_omp_id[0], m_glsl->gl_vsh_id[0]);
                        glAttachShader(m_glsl->gl_omp_id[0], m_glsl->gl_oms_id[0]);
                        glLinkProgram(m_glsl->gl_omp_id[0]);

                         programLog(m_glsl->gl_omp_id[0],i);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__

                        if (!m_shaderStatusFlags[i])
                            {
                            glDeleteProgram(m_glsl->gl_omp_id[0]);
                            m_glsl->gl_omp_id[0] = 0;
                            }
                        else
                            {
                        //  g_linked_programs_counter++; commented OUT because this shader program is not part of the user pipeline! Count valid programs for chooseProgram()       ????????????
                            }
                        }
                m_Watchdog.Start(TIMEOUT*3); // new watchdog        
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initFprograms  (   glsl_state* m_glsl, int p_fromFile, int p_toFile)                                                                   // <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                    if (m_shaderStatusFlags[i]) 
                        {
                        m_glsl->gl_prg_id[i] = glCreateProgram();
                        glAttachShader(m_glsl->gl_prg_id[i], m_glsl->gl_vsh_id[0]);
                        glAttachShader(m_glsl->gl_prg_id[i], m_glsl->gl_fsh_id[i]);
                        glLinkProgram(m_glsl->gl_prg_id[i]);

                        programLog(m_glsl->gl_prg_id[i],i);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__

                        if (!m_shaderStatusFlags[i])
                            {
                            glDeleteProgram(m_glsl->gl_prg_id[i]);
                            m_glsl->gl_prg_id[i] = 0;
                            }
                        else
                            {
                            g_linked_programs_counter++;  // Count valid programs for chooseProgram()       ????????????
                            }
                        }
                    m_Watchdog.Start(TIMEOUT*3); // new watchdog        
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initOuniforms  (   glsl_state* m_glsl, int p_fromFile, int p_toFile)    // Function to initialize Uniforms                           <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                if (m_shaderStatusFlags[i])
                    {
                    glUseProgram(m_glsl->gl_omp_id[0]);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                    m_glsl->gl_vtx           = glGetAttribLocation( m_glsl->gl_omp_id[0], "vertex" );

                    m_glsl->u_atlas[0]       = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_atlas" );
                    m_glsl->u_tile_count[0]  = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_tile_count" );
                    m_glsl->u_tile_rect[0]   = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_tile_rect" );
                    m_glsl->u_tile_index[0]  = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_tile_index" );
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initFuniforms  (   glsl_state* m_glsl, int p_fromFile, int p_toFile)    // Function to initialize Uniforms                           <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                    if (m_shaderStatusFlags[i])
                        {
                        glUseProgram(m_glsl->gl_prg_id[i]);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        m_glsl->gl_vtx           = glGetAttribLocation( m_glsl->gl_prg_id[i], "vertex" );

                        m_glsl->u_time[i]        = glGetUniformLocation(m_glsl->gl_prg_id[i], "time" );
                        m_glsl->u_tres[i]        = glGetUniformLocation(m_glsl->gl_prg_id[i], "tres" );
                        m_glsl->u_seed[i]        = glGetUniformLocation(m_glsl->gl_prg_id[i], "p_seed" );
                        m_glsl->u_aud[i]         = glGetUniformLocation(m_glsl->gl_prg_id[i], "audio" );
                        m_glsl->u_col[i]         = glGetUniformLocation(m_glsl->gl_prg_id[i], "color" );
                        m_glsl->u_par_a[i]       = glGetUniformLocation(m_glsl->gl_prg_id[i], "par_a" );
                        m_glsl->u_par_b[i]       = glGetUniformLocation(m_glsl->gl_prg_id[i], "par_b" );

                        m_glsl->u_tex_l[i]       = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex_l" );

                        m_glsl->u_tex_id[i][0]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[0]" );
                        m_glsl->u_tex_id[i][1]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[1]" );
                        m_glsl->u_tex_id[i][2]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[2]" );
                        m_glsl->u_tex_id[i][3]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[3]" );
                        m_glsl->u_tex_id[i][4]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[4]" );
                        m_glsl->u_tex_id[i][5]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[5]" );
                        m_glsl->u_tex_id[i][6]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[6]" );
                        m_glsl->u_tex_id[i][7]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[7]" );
                        m_glsl->u_tex_id[i][8]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[8]" );
                        m_glsl->u_tex_id[i][9]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[9]" );
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        }
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initOtexture   (   glsl_state* m_glsl, int p_fromFile, int p_toFile)                                                  //  <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
                // p_validTextureCount = 0;  // Counter for valid textures only

                    if(m_H264SystemParser.m_tex_valid[0] == true) // do we really need this too? - yes, otherwise we never know the size of it!! 
                        {
                        glGenTextures(1, &m_glsl->gl_omt_id[0]);  // Use counter instead of i - no in this case we use not this one
                        glBindTexture(GL_TEXTURE_2D, m_glsl->gl_omt_id[0]);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        GLvoid* bitmapData = &m_bufferOmt[0][m_H264SystemParser.m_tex_data_offset[0]]; // oh, we need to figure OUT how we do the bmp parsing for only the atlas!!

                        glTexImage2D(GL_TEXTURE_2D, 
                                   0, 
                                   GL_RGB, 
                                   m_H264SystemParser.m_tex_width[0], // oh, we need to figure OUT how we do the bmp parsing for only the atlas!!
                                   m_H264SystemParser.m_tex_height[0], 
                                   0, 
                                   GL_RGB, 
                                   GL_UNSIGNED_BYTE, 
                                   bitmapData);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        glBindTexture(GL_TEXTURE_2D, 0);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                    //  p_validTextureCount++;  the texture atlas is not part of the user pipeline ! Increment only after successful texture creation
                        }
                    m_Watchdog.Start(TIMEOUT);       // new watchdog
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initUtextures  (   glsl_state* m_glsl, int p_fromFile, int p_toFile)                                                  //  <- we need a dedicated copy here, clear separation but complete emulations ( variables/arrays, etc )
{
                p_validTextureCount = 0;  // Counter for valid textures only

                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    if(m_H264Parser.m_tex_valid[i] == true)
                        {
                        glGenTextures(1, &m_glsl->gl_tex_id[p_validTextureCount]);  // Use counter instead of i
                        glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[p_validTextureCount]);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        GLvoid* bitmapData = &m_bufferTex[i][m_H264Parser.m_tex_data_offset[i]];

                        glTexImage2D(GL_TEXTURE_2D, 
                                   0, 
                                   GL_RGB, 
                                   m_H264Parser.m_tex_width[i],
                                   m_H264Parser.m_tex_height[i], 
                                   0, 
                                   GL_RGB, 
                                   GL_UNSIGNED_BYTE, 
                                   bitmapData);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        glBindTexture(GL_TEXTURE_2D, 0);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                        p_validTextureCount++;  // Increment only after successful texture creation
                        }
                    m_Watchdog.Start(TIMEOUT);       // new watchdog
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initVbuffer    (   glsl_state* m_glsl, int p_fromFile, int p_toFile)                              // Function to initialize Buffers 
{
                static const GLfloat vertex_data[] =  {  -1.0,-1.0, 1.0, 1.0, 1.0,-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0  };

                glClearColor(0.0, 1.0, 1.0, 1.0);

                glGenBuffers(1, &m_glsl->gl_buf);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                
                glViewport(0, 0, m_glsl->screen_width, m_glsl->screen_height);                        // Prepare viewport
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                glBindBuffer(GL_ARRAY_BUFFER, m_glsl->gl_buf);                                          // Upload vertex data to a buffer
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
                glVertexAttribPointer(m_glsl->gl_vtx, 4, GL_FLOAT, 0, 16, 0);
                glEnableVertexAttribArray(m_glsl->gl_vtx);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------