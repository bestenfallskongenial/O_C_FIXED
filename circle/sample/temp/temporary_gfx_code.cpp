bool            CKernel::shaderLogNEW   (   GLint       shader,
                                            int         shaderIndex)
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
#endif // __DEBUG_LOG__

                return success == GL_TRUE;
}

bool            CKernel::programLogNEW  (   GLint       program,
                                            int         program_index)
{
            //  int internal_index = 0;                     // i wonder, if and why we need it, was the indexing in the buffer incorrect? !!! DOUBLECHECK !!!
            //  if (program_index > 0)
            //      internal_index = program_index - 1;

                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);

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

                return success == GL_TRUE;
}

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

void            CKernel::gfx_init_OGL   (   olg_state* o ) // never touch a running horse and if remove the assertions!
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

void            CKernel::initVbuffer    (   olg_state* o,
                                            vertex_state* v )                              // Function to initialize Buffers 
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

void CKernel::initShader(       vertex_state* v,
                                glsl_state* s,
                                texture_state* t,
                                char** srcBuffer,
                                int fromFile,
                                int toFile,
                                GLenum type,
                                bool* flags)
{
    for (int i = fromFile; i < toFile; i++)
    {
        char* src = srcBuffer[i];

        s->gl_shader_id[i] = glCreateShader(type);
        glShaderSource(s->gl_shader_id[i], 1, &src, 0);
        glCompileShader(s->gl_shader_id[i]);

        flags[i] = shaderLog(s->gl_shader_id[i], i);   // ← assign here

#ifdef __GL_DEBUG__
        check();
#endif
    }
}

//!!!!!!!! THIS IS THE ONE!!!!!!!!!!!
void CKernel::initPrograms(     vertex_state* v,
                                glsl_state* vsh,
                                glsl_state* fsh,
                                texture_state* t,
                                int fromFile,
                                int toFile,
                                unsigned& valid_count,
                                bool* flags_vsh,
                                bool* flags_fsh)
{
    for (int i = fromFile; i < toFile; i++)
    {
        fsh->gl_program_id[valid_count] = glCreateProgram();

        if (flags_vsh[0] && flags_fsh[i] && fsh->gl_program_id[valid_count] != 0)
            {
                glAttachShader(fsh->gl_program_id[valid_count], vsh->gl_shader_id[0]);
                glAttachShader(fsh->gl_program_id[valid_count], fsh->gl_shader_id[i]);

                glLinkProgram(fsh->gl_program_id[valid_count]);


            }
        if (programLog(fsh->gl_program_id[valid_count], i))
                {
                valid_count++;
                }
        
        else
                {
                glDeleteProgram(fsh->gl_program_id[valid_count]);
            //  fsh->gl_program_id[valid_count] = 0;
                }
    }

    m_Watchdog.Start(TIMEOUT * 3);
}

//!!!!!!!! THIS IS THE ONE!!!!!!!!!!!
void CKernel::initTextures(     vertex_state* v,
                                glsl_state* s,
                                texture_state* t,
                                char** buffer,
                                int fromFile,
                                int toFile,
                                unsigned& valid_count,
                                bool* flags,
                                GLint wrap_s,
                                GLint wrap_t)
{
    for (int i = fromFile; i < toFile; i++)
    {
    //  t->gl_tex_id[valid_count] = 0;

        if (flags[i])
        {
            glGenTextures(1, &t->gl_tex_id[valid_count]);
            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[valid_count]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            GLvoid* bitmapData = &buffer[i][t->offset[i]];

            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         t->width[i],
                         t->height[i],
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         bitmapData);

            if (glGetError() == GL_NO_ERROR)
            {
                valid_count++;
            }
            else
            {
                glDeleteTextures(1, &t->gl_tex_id[valid_count]);
            }

        //  glBindTexture(GL_TEXTURE_2D, 0); // or outside the if loop!
        }

        m_Watchdog.Start(TIMEOUT);
    }
}
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
/*

okay, what we have done here:

    - we changed the shaderLog() and programLog() from void to bool, means we also removed
      the call parameter bool* flags - its set now on caller side
    - we simplified the function initShader() and initTexture that it give out a dense indexed
      id / handle for the programs / textures -> we now need to refactor the select*() functions

// NEW generic not condensed valid arrays, max number of files ( macros for example!)

void CKernel::chooseIndexSparse(int p_channel, int& p_activeIndex, int p_maxCount, bool* flags)
{
    static int p_activeIndex = 0;

    int f_calculated = g_inOutMatrixInt[p_channel][RAW] * p_maxCount >> 10;

    if (flags[f_calculated])
    {
        p_activeIndex = f_calculated;
    }
}

// NEW generic condensed valid arrays, max number of files ( macros for example!)

void CKernel::chooseIndexDense(int p_channel, int& p_activeIndex, int p_maxCount)
{
    int f_calculated = (g_inOutMatrixInt[p_channel][RAW] * p_maxCount) >> 10;

    p_activeIndex = f_calculated;
}

the involved array/enums:

// the array for the loader constance - i think its better than scatter the values / constants everywhere around

#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMF_SD             		1	// max number of fragment shader on sd
#define         FSH_SD             		1	// max number of fragment shader on sd
#define         OMT_SD             		1	// max number of fragment shader on sd
#define         TEX_SD             		0	// max number of textures on sd
#define         VID_SD             		0	// max number of videos on sd
#define         KLN_SD                  1

#define         FRM_SD                  1   // i put them here because if my mem/dma allocation
#define         LOG_SD                  24  // here is the trick:
                                            // i will use a mix of hardwired and "open" logs.
                                            // m_bufferLog[0]       - startup - memory - init log
                                            // m_bufferLog[1]       - vc_sm
                                            // m_bufferLog[2]       - mmal
                                            // m_bufferLog[3]       - check glsl
                                            // m_bufferLog[4]       - glsl compile
                                            // m_bufferLog[5  - 15] - texture parser
                                            // m_bufferLog[16 - 23] - video parser

#define         VSH_USB                 0	// max number of u_vertex shader on sd
#define         OMF_USB            		0	// max number of fragment shader on sd
#define         FSH_USB            		32	// max number of fragment shader on sd
#define         OMT_USB            		0	// max number of fragment shader on sd
#define         TEX_USB            		8	// max number of textures on sd
#define         VID_USB            		8	// max number of videos on sd
#define         KLN_USB                 1

#define         FRM_USB                 1   // i put them here because if my mem/dma allocation
#define         LOG_USB                 1

#define         VSH_EXT                 1
#define         OMF_EXT                 1
#define         FSH_EXT                 1
#define         OMT_EXT                 1
#define         TEX_EXT                 1
#define         VID_EXT                 1
#define         KLN_EXT                 1

#define         VSH_SIZ                 (1024*32)
#define         OMF_SIZ                 (1024*32)
#define         FSH_SIZ                 (1024*32)
#define         OMT_SIZ                 (1024*1024*4)
#define         TEX_SIZ                 (1024*1024*4)
#define         VID_SIZ                 (1024*1024*8)
#define         KLN_SIZ                 (1024*1024*2)

#define         FRM_SIZ                 (1024*1024)

#define         LOG_SIZ                 (1024*64)

enum FileType
{
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FRM_BF,         // i decided to add the output-frames A & B
    LOGGER,         // and logger buffer information here
    FT_COUNT
};

enum FileField
{
    FLD_MAXSD = 0,
    FLD_MAXUSB,
    FLD_EXTCNT,
    FLD_SCANNED,    // new
    FLD_LOADED,
    FLD_PREV,       // new
    FLD_VALID, // <- p_validCount 
    FLD_SIZE,
    FLD_COUNT
};

unsigned filecounter[FT_COUNT][FLD_COUNT] =
{  // MAXSD   MAXUSB    EXTCNT      SCANNED   LOADED  PREV    V_CNT    SIZE  
    { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
    { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
    { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
    { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
    { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
    { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
    { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
    { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
    { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }   // LOG logging buffers   
};
// lists of extensions possible in my scanroot directory function per filetype 
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" }; 
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" }; // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };
        const   char                   *g_SufVid[VID_EXT]			    = { "264" }; // i guess i will remove the whole parse code for anything but h264
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };
// array to store the scanned filenames
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLN_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLN_SD + KLN_USB]      = { 0 };


*/

// SHADERS (delta)
initShader(&m_vertex, &m_vsh, &m_tex,
           m_bufferVsh,
           filecounter[FT_VSH][FLD_PREV],
           filecounter[FT_VSH][FLD_LOADED],
           GL_VERTEX_SHADER,
           vsh_flags);

initShader(&m_vertex, &m_osh, &m_omt,
           m_bufferOmf,
           filecounter[FT_OMF][FLD_PREV],
           filecounter[FT_OMF][FLD_LOADED],
           GL_FRAGMENT_SHADER,
           omf_flags);

initShader(&m_vertex, &m_fsh, &m_tex,
           m_bufferFsh,
           filecounter[FT_FSH][FLD_PREV],
           filecounter[FT_FSH][FLD_LOADED],
           GL_FRAGMENT_SHADER,
           fsh_flags);


// PROGRAMS (delta)
initPrograms(&m_vertex,
             &m_vsh,
             &m_fsh,
             &m_tex,
             filecounter[FT_FSH][FLD_PREV],
             filecounter[FT_FSH][FLD_LOADED],
             filecounter[FT_FSH][FLD_VALID],
             vsh_flags,
             fsh_flags);

initPrograms(&m_vertex,
             &m_vsh,
             &m_osh,
             &m_omt,
             filecounter[FT_OMF][FLD_PREV],
             filecounter[FT_OMF][FLD_LOADED],
             filecounter[FT_OMF][FLD_VALID],
             vsh_flags,
             omf_flags);


// TEXTURES (delta)
initTextures(&m_vertex,
             &m_fsh,
             &m_tex,
             m_bufferTex,
             filecounter[FT_TEX][FLD_PREV],
             filecounter[FT_TEX][FLD_LOADED],
             filecounter[FT_TEX][FLD_VALID],
             tex_flags,
             GL_REPEAT,
             GL_REPEAT);

initTextures(&m_vertex,
             &m_osh,
             &m_omt,
             m_bufferOmt,
             filecounter[FT_OMT][FLD_PREV],
             filecounter[FT_OMT][FLD_LOADED],
             filecounter[FT_OMT][FLD_VALID],
             omt_flags,
             GL_CLAMP_TO_EDGE,
             GL_CLAMP_TO_EDGE);