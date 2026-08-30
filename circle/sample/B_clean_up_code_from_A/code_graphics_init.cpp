#include "kernel.h"

    #define MY_BFR   m_logBuffer                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logBufferIndex

void            CKernel::initOGL                    (   olg_state*  o )
{
                EGLint num_config;

                static EGL_DISPMANX_WINDOW_T nativewindow;

                DISPMANX_UPDATE_HANDLE_T dispman_update;
                VC_RECT_T dst_rect;
                VC_RECT_T src_rect;

                static const EGLint attribute_list[] =      {   EGL_RED_SIZE, 8,
                                                                EGL_GREEN_SIZE, 8,
                                                                EGL_BLUE_SIZE, 8,
                                                                EGL_ALPHA_SIZE, 8,
                                                                EGL_SURFACE_TYPE, 
                                                                EGL_WINDOW_BIT,
                                                                EGL_NONE };
   
                static const EGLint context_attributes[] =  {   EGL_CONTEXT_CLIENT_VERSION, 
                                                                2,
                                                                EGL_NONE };

                EGLConfig config;
                
                o->display                  = eglGetDisplay             (   EGL_DEFAULT_DISPLAY     );
#ifdef __DEBUG_GL__        
                check();        
#endif  
                                             eglInitialize              (   o->display, NULL, NULL );
#ifdef __DEBUG_GL__
                check();
#endif  
                                             eglChooseConfig            (   o->display, attribute_list, &config, 1, &num_config );
#ifdef __DEBUG_GL__       
                check();         
#endif  
                                             eglBindAPI                 (   EGL_OPENGL_ES_API );
#ifdef __DEBUG_GL__
                check();
#endif  
                o->context                  = eglCreateContext          (   o->display, config, EGL_NO_CONTEXT, context_attributes );
#ifdef __DEBUG_GL__
                check();
#endif  
                                             graphics_get_display_size  (   0, &o->screen_width, &o->screen_height );
#ifdef __DEBUG_GL__
                check();
#endif  
                dst_rect.x                  = 0;
                dst_rect.y                  = 0;
                dst_rect.width              = o->screen_width;
                dst_rect.height             = o->screen_height;
      
                src_rect.x                  = 0;
                src_rect.y                  = 0;
                src_rect.width              = o->screen_width << 16;
                src_rect.height             = o->screen_height << 16;        

                o->dispman_display          = vc_dispmanx_display_open  (   0 );
                dispman_update              = vc_dispmanx_update_start  (   0 );
      
                o->dispman_element          = vc_dispmanx_element_add   (   dispman_update, o->dispman_display, 0, &dst_rect, 0, &src_rect, DISPMANX_PROTECTION_NONE, 0, 0, DISPMANX_NO_ROTATE );
      
                nativewindow.element        = o->dispman_element;
                nativewindow.width          = o->screen_width;
                nativewindow.height         = o->screen_height;

                vc_dispmanx_update_submit_sync                          (   dispman_update );
#ifdef __DEBUG_GL__
                check();
#endif              
                o->surface                  = eglCreateWindowSurface    (   o->display, config, &nativewindow, NULL );
#ifdef __DEBUG_GL__
                check();
#endif                  
                                             eglMakeCurrent             (   o->display, o->surface, o->surface, o->context );
#ifdef __DEBUG_GL__
                check();
#endif  
                                              glClearColor              (   0.15f, 0.25f, 0.35f, 1.0f );
                                              glClear                   (   GL_COLOR_BUFFER_BIT );
#ifdef __DEBUG_GL__
                check();
#endif              
}

void            CKernel::initVbuffer                (   olg_state*  o,
                                                        vtx_state*  v )
{
                static const GLfloat vertex_data[] =  {  -1.0,-1.0, 1.0, 1.0, 1.0,-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0  };

                glClearColor(0.0, 1.0, 1.0, 1.0);
                glGenBuffers(1, &v->gl_buf);
#ifdef __DEBUG_GL__
                check();
#endif  
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef __DEBUG_GL__
                check();
#endif  
                glViewport(0, 0, o->screen_width, o->screen_height);
#ifdef __DEBUG_GL__
                check();
#endif  
                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
                glVertexAttribPointer(v->gl_vtx[0], 4, GL_FLOAT, 0, 16, 0);
                glEnableVertexAttribArray(v->gl_vtx[0]);
#ifdef __DEBUG_GL__
                check();
#endif  
}

void            CKernel::initShader                 (   vtx_state*  v,
                                                        glsl_state* s,
                                                        tex_state*  t,
                                                        char**      p_buffer,
                                                        int         p_fromFile,
                                                        int         p_toFile,
                                                        GLenum      type)
{
                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    const char* src = p_buffer[i];

                    s->gl_shader_id[i] = glCreateShader(type);
                    glShaderSource(s->gl_shader_id[i], 1, &src, 0);
                    glCompileShader(s->gl_shader_id[i]);

                    s->shader_valid[i] = shaderLog(s->gl_shader_id[i], i);
#ifdef __DEBUG_GL__
                    check();
#endif
                    }
}

void            CKernel::initProgram                (   vtx_state*  v,
                                                        glsl_state* vsh,
                                                        glsl_state* fsh,
                                                        tex_state*  t,
                                                        int         p_fromFile,
                                                        int         p_toFile,
                                                        unsigned&   valid_count)
{
                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    fsh->gl_program_id[valid_count] = glCreateProgram();
                    if (vsh->shader_valid[0] && fsh->shader_valid[i] && fsh->gl_program_id[valid_count] != 0)
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
                        }
                }
                 // m_Watchdog.Start(TIMEOUT * 3);
}

void            CKernel::initTexture                (   vtx_state*  v,
                                                        glsl_state* s,
                                                        tex_state*  t,
                                                        int         p_fromFile,
                                                        int         p_toFile,
                                                        unsigned&   valid_count,
                                                        GLint       wrap_s,
                                                        GLint       wrap_t )
{
                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    if (t->tex_valid[i])
                        {
                        glGenTextures(1, &t->gl_tex_id[valid_count]);
                        glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[valid_count]);

                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                        GLvoid* bitmapData = &t->data[i][t->offset[i]];
                        glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGB, t->width[i],
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
                        }
                     // m_Watchdog.Start(TIMEOUT);
                    }
}

void            CKernel::initUniform                (   vtx_state*  v,
                                                        glsl_state* s,
                                                        tex_state*  t,
                                                        int         p_fromFile,
                                                        int         p_toFile )
{
                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    glUseProgram(s->gl_program_id[i]);
#ifdef __DEBUG_GL__
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

                for (int j = 0; j < MAX_TEXTURE; ++j)
                    {
                    char name[8];

                    name[0] = 't'; name[1] = 'e'; name[2] = 'x'; name[3] = '[';

                    if (j < 10)
                        {
                        name[4] = '0' + j;
                        name[5] = ']';
                        name[6] = '\0';
                        }
                    else
                        {
                        name[4] = '0' + (j / 10);
                        name[5] = '0' + (j % 10);
                        name[6] = ']';
                        name[7] = '\0';
                        }
                    t->u_tex_id[i][j] = glGetUniformLocation(s->gl_program_id[i], name);
                    }
#ifdef __DEBUG_GL__
                check();
#endif
                }
}