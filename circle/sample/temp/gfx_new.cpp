// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------      
void            CKernel::shaderLog(GLint shader, int shaderIndex, bool* flags)
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

                flags[shaderIndex] = (success == GL_TRUE);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
#endif // __DEBUG_LOG__                 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::programLog(GLint program, int program_index, bool* flags)
{
            //  int internal_index = 0;                     // i wonder, if and why we need it, was the indexing in the buffer incorrect? !!! DOUBLECHECK !!!
            //  if (program_index > 0)
            //      internal_index = program_index - 1;

                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);

                flags[program_index] = (success == GL_TRUE);
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program link status idx/success", (u32)program_index, (u32)success);
#endif // __DEBUG_LOG__
            //  char name[27];
            //  strncpy(name, &m_bufferFsh[program_index][2], 26);
            //  name[26] = '\0';
#ifdef __DEBUG_LOG__ 
            //  storeMsg( MY_BUFFER, MY_INDEX, "Program short name", name, 26);
            //  storeMsg( MY_BUFFER, MY_INDEX, "Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
                storeLog( MY_BUFFER, MY_INDEX, "Program byte size", (u32)g_bytFsh[program_index]);
#endif // __DEBUG_LOG__
                char log[1024];
                glGetProgramInfoLog(program, sizeof(log), NULL, log);
#ifdef __DEBUG_LOG__                 
                storeMsg( MY_BUFFER, MY_INDEX, "Program InfoLog", log, sizeof(log));
#endif // __DEBUG_LOG__
                GLint numUniforms;
                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Uniforms", (u32)numUniforms);
#endif // __DEBUG_LOG__
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
#endif // __DEBUG_LOG__                    
                }

                GLint numAttributes;
                glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Attributes", (u32)numAttributes);
#endif // __DEBUG_LOG__
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
#ifdef __DEBUG_LOG__  
                    storeLog( MY_BUFFER, MY_INDEX, "=== Final System Status ticks/count ===", (u32)ticks, (u32)error_count);
#endif // __DEBUG_LOG__ 
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
void            CKernel::initOGL      (   olg_state* o )
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
                
                o->display             = eglGetDisplay             (   EGL_DEFAULT_DISPLAY     );  // get an EGL display connection
#ifdef __OLG_DEBUG__                
assert(o->display!=EGL_NO_DISPLAY);
                check();
#endif // __OLG_DEBUG__
                result                      = eglInitialize             (   o->display,         // initialize the EGL display connection
                                                                            NULL, 
                                                                            NULL                    );
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
                check();
#endif // __OLG_DEBUG__
                result                      = eglChooseConfig           (   o->display,         // get an appropriate EGL frame buffer configuration 
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
                o->context             = eglCreateContext          (   o->display,         // create an EGL rendering context
                                                                            config, 
                                                                            EGL_NO_CONTEXT, 
                                                                            context_attributes      );
#ifdef __OLG_DEBUG__
assert(o->context!=EGL_NO_CONTEXT);//?
                check();
#endif // __OLG_DEBUG__
                success                     = graphics_get_display_size (   0 /* LCD */,            // create an EGL window surface
                                                                            &o->screen_width, 
                                                                            &o->screen_height   );
#ifdef __OLG_DEBUG__
assert( success >= 0 );
                check();
#endif // __OLG_DEBUG__
                dst_rect.x                  = 0;
                dst_rect.y                  = 0;
                dst_rect.width              = o->screen_width;
                dst_rect.height             = o->screen_height;
      
                src_rect.x                  = 0;
                src_rect.y                  = 0;
                src_rect.width              = o->screen_width << 16;
                src_rect.height             = o->screen_height << 16;        

                o->dispman_display     = vc_dispmanx_display_open  (   0 /* LCD */ );
                dispman_update              = vc_dispmanx_update_start  (   0 );
      
                o->dispman_element     = vc_dispmanx_element_add   (   dispman_update, 
                                                                            o->dispman_display,
                                                                            0/*layer*/, 
                                                                            &dst_rect, 
                                                                            0/*src*/,
                                                                            &src_rect, 
                                                                            DISPMANX_PROTECTION_NONE, /*protection*/
                                                                            0 /*alpha*/, 
                                                                            0 /*clamp*/, 
                                                                            DISPMANX_NO_ROTATE /*transform*/ );   // was 0/*transform*/ before DISPMANX_ROTATE_90 
      
                nativewindow.element        = o->dispman_element;
                nativewindow.width          = o->screen_width;
                nativewindow.height         = o->screen_height;

                vc_dispmanx_update_submit_sync                          (   dispman_update  );
#ifdef __OLG_DEBUG__
                check();
#endif // __OLG_DEBUG__            
                o->surface             = eglCreateWindowSurface    (   o->display, 
                                                                            config, 
                                                                            &nativewindow, 
                                                                            NULL            );
#ifdef __OLG_DEBUG__
assert(o->surface != EGL_NO_SURFACE);//?
                check();
#endif // __OLG_DEBUG__                
                result                      = eglMakeCurrent            (   o->display,     // connect the context to the surface
                                                                            o->surface, 
                                                                            o->surface, 
                                                                            o->context  );
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
void            CKernel::initVbuffer    (   olg_state* o,
                                            vtx_state* v )                              // Function to initialize Buffers 
{
                static const GLfloat vertex_data[] =  {  -1.0,-1.0, 1.0, 1.0, 1.0,-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0  };

                glClearColor(0.0, 1.0, 1.0, 1.0);

                glGenBuffers(1, &v->gl_buf);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                
                glViewport(0, 0, o->screen_width, o->screen_height);                        // Prepare viewport
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);                                          // Upload vertex data to a buffer
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
                glVertexAttribPointer(v->gl_vtx[0], 4, GL_FLOAT, 0, 16, 0);
                glEnableVertexAttribArray(v->gl_vtx[0]);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::initShaders(  vtx_state* v,
                            glsl_state* s,
                            tex_state* t,
                            char** srcBuffer, // removed the const here
                            int fromFile,
                            int toFile,
                            GLenum type,
                            bool* flags)
{
    for (int i = fromFile; i < toFile; i++)
    {
        char* src = srcBuffer[i]; // and here

        s->gl_shader_id[i] = glCreateShader(type);
        glShaderSource(s->gl_shader_id[i], 1, &src, 0);
        glCompileShader(s->gl_shader_id[i]);

        shaderLog(s->gl_shader_id[i], i, flags); // should rather return 0/1 that we can do  flags[i] = ... right here!!!

#ifdef __GL_DEBUG__
        check();
#endif
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::initProgram( vtx_state* v,
                            glsl_state* vsh,
                            glsl_state* fsh,
                            tex_state* t,
                            int fromFile,
                            int toFile,
                            bool* flags_vsh,
                            bool* flags_fsh)
{
            if (!flags_vsh[0])
                return;

    for (int i = fromFile; i < toFile; i++)
    {
        if (!flags_fsh[i])
            continue;

        fsh->gl_program_id[i] = glCreateProgram();
        
        if (!fsh->gl_program_id[i])     // i dont know! another suggestion from the wanna be smarter but knows shit retard!
            continue;

        glAttachShader(fsh->gl_program_id[i], vsh->gl_shader_id[0]);
        glAttachShader(fsh->gl_program_id[i], fsh->gl_shader_id[i]);

        glLinkProgram(fsh->gl_program_id[i]);

        programLog(fsh->gl_program_id[i], i, flags_fsh); // should rather return 0/1 that we can do  flags[i] = ... right here!!!

#ifdef __GL_DEBUG__
        check();
#endif

        if (!flags_fsh[i])
        {
            glDeleteProgram(fsh->gl_program_id[i]);
            fsh->gl_program_id[i] = 0;
        }
        else
        {
            g_linked_programs_counter++;
        }
    }

    m_Watchdog.Start(TIMEOUT * 3);
}

void CKernel::initProgram(  vtx_state* v,
                            glsl_state* vsh,
                            glsl_state* fsh,
                            tex_state* t,
                            int fromFile,
                            int toFile,
                            unsigned& valid_count,
                            bool* flags_vsh,
                            bool* flags_fsh)
{
            if (!flags_vsh[0])
                return;

    for (int i = fromFile; i < toFile; i++)
    {
        if (!flags_fsh[i])
            continue;

        fsh->gl_program_id[i] = glCreateProgram();
        
        if (!fsh->gl_program_id[i])
            continue;

        glAttachShader(fsh->gl_program_id[i], vsh->gl_shader_id[0]);
        glAttachShader(fsh->gl_program_id[i], fsh->gl_shader_id[i]);

        glLinkProgram(fsh->gl_program_id[i]);

        programLog(fsh->gl_program_id[i], i, flags_fsh);

#ifdef __GL_DEBUG__
        check();
#endif

        if (!flags_fsh[i])
        {
            glDeleteProgram(fsh->gl_program_id[i]);
            fsh->gl_program_id[i] = 0;
        }
        else
        {
            fsh->gl_program_id[valid_count] = fsh->gl_program_id[i];
            valid_count++;
        }
    }

    m_Watchdog.Start(TIMEOUT * 3);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::initUniform( vtx_state* v,
                            glsl_state* s,
                            tex_state* t,
                            int fromFile,
                            int toFile,
                            bool* flags)
{
    for (int i = fromFile; i < toFile; i++)
    {
        if (!flags[i])
            continue;

        glUseProgram(s->gl_program_id[i]);

#ifdef __GL_DEBUG__
        check();
#endif
        v->gl_vtx[i]       = glGetAttribLocation(s->gl_program_id[i], "vertex");

        s->u_time[i]       = glGetUniformLocation(s->gl_program_id[i], "time");
        s->u_tres[i]       = glGetUniformLocation(s->gl_program_id[i], "tres");
        s->u_seed[i]       = glGetUniformLocation(s->gl_program_id[i], "p_seed");
        s->u_aud[i]        = glGetUniformLocation(s->gl_program_id[i], "audio");
        s->u_col[i]        = glGetUniformLocation(s->gl_program_id[i], "color");
        s->u_par_a[i]      = glGetUniformLocation(s->gl_program_id[i], "par_a");
        s->u_par_b[i]      = glGetUniformLocation(s->gl_program_id[i], "par_b");

        s->u_tex_l[i]      = glGetUniformLocation(s->gl_program_id[i], "tex_l");

        s->u_atlas[i]      = glGetUniformLocation(s->gl_program_id[i], "u_menu_atlas");
        s->u_tile_count[i] = glGetUniformLocation(s->gl_program_id[i], "u_menu_tile_count");
        s->u_tile_rect[i]  = glGetUniformLocation(s->gl_program_id[i], "u_menu_tile_rect");
        s->u_tile_index[i] = glGetUniformLocation(s->gl_program_id[i], "u_menu_tile_index");

        t->u_tex_id[i][0] = glGetUniformLocation(s->gl_program_id[i], "tex[0]");
        t->u_tex_id[i][1] = glGetUniformLocation(s->gl_program_id[i], "tex[1]");
        t->u_tex_id[i][2] = glGetUniformLocation(s->gl_program_id[i], "tex[2]");
        t->u_tex_id[i][3] = glGetUniformLocation(s->gl_program_id[i], "tex[3]");
        t->u_tex_id[i][4] = glGetUniformLocation(s->gl_program_id[i], "tex[4]");
        t->u_tex_id[i][5] = glGetUniformLocation(s->gl_program_id[i], "tex[5]");
        t->u_tex_id[i][6] = glGetUniformLocation(s->gl_program_id[i], "tex[6]");
        t->u_tex_id[i][7] = glGetUniformLocation(s->gl_program_id[i], "tex[7]"); // in theory a loop up to TEX_MAX but im lazy!!

#ifdef __GL_DEBUG__
        check();
#endif
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::initTexture(
    vtx_state* v,
    glsl_state* s,
    tex_state* t,
    char** buffer,
    int fromFile,
    int toFile,
    bool* flags,
    GLint wrap_s,
    GLint wrap_t)       // should we not implement a dence index packing or even a translation matrix to support u_tex_l and p_validTextureCount???  
{
    for (int i = fromFile; i < toFile; i++)
    {
        if (!flags[i])  // parser does the first pass
            continue;

        glGenTextures(1, &t->gl_tex_id[i]);
        glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[i]);
#ifdef __GL_DEBUG__
        check();
#endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __GL_DEBUG__
        check();
#endif
        GLvoid* bitmapData = &buffer[i][t->offset[i]]; // parser need to be refactored to fill the tex struct!

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     t->width[i],
                     t->height[i],
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     bitmapData);

        flags[i] = (glGetError() == GL_NO_ERROR);   // second pass overwrite - get gl error clears the error queue check too!
// #ifdef __GL_DEBUG__
//         check();        
// #endif
        glBindTexture(GL_TEXTURE_2D, 0);
#ifdef __GL_DEBUG__
        check();
#endif
        m_Watchdog.Start(TIMEOUT); // there was an purpose here, just because some textures may take longer?
    }
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::render_uniforms_all(glsl_state* s, glsl_state* m) // shall i combine them here or just keep the two separate calls?
{
#ifdef __GL_DEBUG__
    check();
#endif

    // ================= USER =================
    int i = g_current_gl_program;

    glUseProgram(s->gl_program_id[i]);
#ifdef __GL_DEBUG__
    check();
#endif

    GLuint cx = s->screen_width;
    GLuint cy = s->screen_height;

    if (s->u_time[i]  != -1) glUniform1f(s->u_time[i], GLtime);
    if (s->u_tres[i]  != -1) glUniform2f(s->u_tres[i], cx, cy);
    if (s->u_seed[i]  != -1) glUniform4f(s->u_seed[i],
        g_inOutMatrixFlt[0][RND],
        g_inOutMatrixFlt[1][RND],
        g_inOutMatrixFlt[2][RND],
        g_inOutMatrixFlt[3][RND]);

    if (s->u_aud[i]   != -1) glUniform4f(s->u_aud[i],
        g_inOutMatrixFlt[0][AU0],
        g_inOutMatrixFlt[0][AU1],
        g_inOutMatrixFlt[0][AU2],
        g_inOutMatrixFlt[0][AU3]);

    if (s->u_col[i]   != -1) glUniform4f(s->u_col[i], 0.0f, 0.0f, 0.0f, g_opaque);

    if (s->u_par_a[i] != -1) glUniform4f(s->u_par_a[i],
        g_inOutMatrixFlt[0][OUT],
        g_inOutMatrixFlt[1][OUT],
        g_inOutMatrixFlt[2][OUT],
        g_inOutMatrixFlt[3][OUT]);

    if (s->u_par_b[i] != -1) glUniform4f(s->u_par_b[i],
        g_inOutMatrixFlt[4][OUT],
        g_inOutMatrixFlt[5][OUT],
        g_inOutMatrixFlt[6][OUT],
        g_inOutMatrixFlt[7][OUT]);

    if (s->u_tex_l[i] != -1) glUniform1i(s->u_tex_l[i], p_validTextureCount);

#ifdef __GL_DEBUG__
    check();
#endif


    // ================= MENU =================
    glUseProgram(m->gl_program_id[0]);
#ifdef __GL_DEBUG__
    check();
#endif

    if (m->u_tile_count[0] != -1)
        glUniform1i(m->u_tile_count[0], MENU_GPU_TILE_COUNT);

    if (m->u_tile_rect[0] != -1)
        glUniform4fv(m->u_tile_rect[0], MENU_GPU_TILE_COUNT, m->tile_rect);

    if (m->u_tile_index[0] != -1)
        glUniform1fv(m->u_tile_index[0], MENU_GPU_TILE_COUNT, m->tile_index);

#ifdef __GL_DEBUG__
    check();
#endif
}
/* calls, but i still need to add the parameters in the call signature i introduced earlier to have a unified style

initShaders(&m_vsh, 0, vsh_count, m_bufferVsh, GL_VERTEX_SHADER, vsh_flags);
initShaders(&m_fsh, 0, fsh_count, m_bufferFsh, GL_FRAGMENT_SHADER, fsh_flags);
initShaders(&m_osh, 0, omf_count, m_bufferOsh, GL_FRAGMENT_SHADER, omf_flags);

initProgram(&m_vsh, &m_fsh, 0, fsh_count, fsh_flags);
initProgram(&m_vsh, &m_osh, 0, omf_count, omf_flags);

initUniform(&m_vtx, &m_fsh, &m_tex, 0, fsh_count, fsh_flags);
initUniform(&m_vtx, &m_osh, &m_omt, 0, omf_count, omf_flags);

initTexture(&m_tex, 0, tex_count, m_bufferTex, tex_flags, p_validTextureCount, GL_REPEAT, GL_REPEAT);
initTexture(&m_omt, 0, 1,         m_bufferOtm, omt_flags, dummyTextureCount, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

render_uniforms_all(fm_glsl, m_menu);
*/