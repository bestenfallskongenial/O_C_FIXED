#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex

void            CKernel::frmBufferSet               (   vtx_state* v)
{
                glBindFramebuffer(GL_FRAMEBUFFER,0);

                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#ifdef __DEBUG_GL__
                check();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);
#ifdef __DEBUG_GL__
                check();
#endif
}

void            CKernel::frmBufferSwap              (   olg_state* o )
{
                eglSwapBuffers(o->display, o->surface);
#ifdef __DEBUG_GL__
                check();
#endif
}

void            CKernel::setUniPrg                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t,
                                                    /*  int         gl_current_tex, */
                                                        unsigned    p_validTextureCount )
{
#ifdef __DEBUG_GL__
                check();
#endif    
                glUseProgram(s->gl_program_id[g_gl_program_current]);
#ifdef __DEBUG_GL__
                check();
#endif
                if(s->u_time[g_gl_program_current] != -1)  glUniform1f(s->u_time[g_gl_program_current], GLtime);
#ifdef __DEBUG_GL__
                check();
#endif                
                if(s->u_tres[g_gl_program_current]!= -1 )  glUniform2f(s->u_tres[g_gl_program_current], o->screen_width, o->screen_width);
#ifdef __DEBUG_GL__
                check();
#endif                
                if(s->u_seed[g_gl_program_current] != -1)  glUniform4f(s->u_seed[g_gl_program_current], g_inOutMatrixFlt[0][RND], 
                                                                                                        g_inOutMatrixFlt[1][RND], 
                                                                                                        g_inOutMatrixFlt[2][RND], 
                                                                                                        g_inOutMatrixFlt[3][RND]);
#ifdef __DEBUG_GL__
                check();
#endif                                                                                                        
                if(s->u_aud[g_gl_program_current]!= -1 )   glUniform4f(s->u_aud[g_gl_program_current],  g_inOutMatrixFlt[0][AU0], 
                                                                                                        g_inOutMatrixFlt[0][AU1], 
                                                                                                        g_inOutMatrixFlt[0][AU2], 
                                                                                                        g_inOutMatrixFlt[0][AU3]);
#ifdef __DEBUG_GL__
                check();
#endif                                                                                                        
                if(s->u_col[g_gl_program_current] != -1)   glUniform4f(s->u_col[g_gl_program_current],  0.0f, 0.0f, 0.0f, g_opaque);
#ifdef __DEBUG_GL__
                check();
#endif                
                if(s->u_par_a[g_gl_program_current] != -1) glUniform4f(s->u_par_a[g_gl_program_current],g_inOutMatrixFlt[0][OUT], 
                                                                                                        g_inOutMatrixFlt[1][OUT], 
                                                                                                        g_inOutMatrixFlt[2][OUT], 
                                                                                                        g_inOutMatrixFlt[3][OUT]);
#ifdef __DEBUG_GL__
                check();
#endif                                                                                                        
                if(s->u_par_b[g_gl_program_current] != -1) glUniform4f(s->u_par_b[g_gl_program_current],g_inOutMatrixFlt[4][OUT], 
                                                                                                        g_inOutMatrixFlt[5][OUT], 
                                                                                                        g_inOutMatrixFlt[6][OUT], 
                                                                                                        g_inOutMatrixFlt[7][OUT]);
#ifdef __DEBUG_GL__
                check();
#endif                                                                                                        
                if(s->u_tex_l[g_gl_program_current] != -1) glUniform1i(s->u_tex_l[g_gl_program_current],p_validTextureCount); 

#ifdef __DEBUG_GL__
                check();
#endif
}

void            CKernel::setTexPrg                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t,
                                                        int         gl_current_tex,
                                                        unsigned    p_validTextureCount )
{
#ifdef __H264_DEBUG_TEX__
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_TextureA);

                if (t->u_tex_id[g_gl_program_current][0] != -1) glUniform1i(t->u_tex_id[g_gl_program_current][0], 0);
#ifdef __DEBUG_GL__
                check();
#endif   

#endif   

#ifndef __H264_DEBUG_TEX__
                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] == FLAG_THRESHOLD)
                    {
                    for (unsigned i = 0; i < p_validTextureCount; i++)
                        {
                        glActiveTexture(GL_TEXTURE0+i);
                        glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[i]);

                        if (t->u_tex_id[g_gl_program_current][i] != -1) glUniform1i(t->u_tex_id[g_gl_program_current][i], i);
#ifdef __DEBUG_GL__
                        check();
#endif   
                        }
                    }
                else
                    {
                    switch(p_validTextureCount)
                        {
                        case 0:

                        break;

                        case 1:
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex]);

                            if (t->u_tex_id[g_gl_program_current][0] != -1) glUniform1i(t->u_tex_id[g_gl_program_current][0], 0);
#ifdef __DEBUG_GL__
                            check();
#endif   
                        break;

                        default:
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex]);

                            if (t->u_tex_id[g_gl_program_current][0] != -1) glUniform1i(t->u_tex_id[g_gl_program_current][0], 0);
#ifdef __DEBUG_GL__
                            check();
#endif   
                            glActiveTexture(GL_TEXTURE1);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex + 1]);

                            if (t->u_tex_id[g_gl_program_current][1] != -1) glUniform1i(t->u_tex_id[g_gl_program_current][1], 1);
#ifdef __DEBUG_GL__
                            check();
#endif   
                        break;
                        }
                    }
#endif   
}

void            CKernel::drawGLsPrg                 (   )
{
#ifdef __DEBUG_GL__
                check();
#endif    
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#ifdef __DEBUG_GL__
                check();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, 0);
}
/*
void            CKernel::frmRateBreak               (   bool noTargetFPS )
{
                glFlush();

                if ( noTargetFPS )
                    {
                    glFinish();
#ifdef __DEBUG_GL__
                    check();
#endif
                    }
}
*/
void            CKernel::setUniOvl                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t )
{
                glUseProgram(s->gl_program_id[0]);
#ifdef __DEBUG_GL__
                check();
#endif
                if (s->u_tile_count[0] != -1) glUniform1i(s->u_tile_count[0], MENU_GPU_TILE_COUNT);

                if (s->u_tile_rect[0] != -1) glUniform4fv(s->u_tile_rect[0], MENU_GPU_TILE_COUNT, s->tile_rect);

                if (s->u_tile_index[0] != -1) glUniform1fv(s->u_tile_index[0], MENU_GPU_TILE_COUNT, s->tile_index);
#ifdef __DEBUG_GL__
                check();
#endif
}

void            CKernel::setTexOvl                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t )
{
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[0]);

                if (s->u_atlas[0] != -1) glUniform1i(s->u_atlas[0], 0);
#ifdef __DEBUG_GL__
                check();
#endif
}

void            CKernel::drawGLsOvl                 (   )
{
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //  glBlendFunc(GL_ZERO, GL_SRC_COLOR);                 // shall fix the "no alfa in my 24bit bmp texture bitmap" issue
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glDisable(GL_BLEND);
#ifdef __DEBUG_GL__
                check();
#endif
}

// lets try the fps break here:

void            CKernel::fpsBegin()
{
                g_frameStart = m_Timer.GetClockTicks();
}

void            CKernel::fpsBreak()
{
#ifdef __DEBUG_TIMING__    
                g_runtimeDuration = m_Timer.GetClockTicks() - g_frameStart;
#endif                
                glFlush();
#ifdef __DEBUG_GL__
                check();
#endif
#ifdef __DEBUG_TIMING__
                g_glDuration = (m_Timer.GetClockTicks() - g_frameStart) - g_runtimeDuration;
#endif
                g_frameCurrent = m_Timer.GetClockTicks();
                g_frameTarget  = g_frameStart + (1000000 / TARGET_FPS);

                if (g_limitFPS && g_frameTarget > g_frameCurrent + g_lastSwapDuration)
                    {
                    g_frameDelay = g_frameTarget - (g_frameCurrent + g_lastSwapDuration);

                    usDelay(g_frameDelay);
                    }

                g_frameCurrent = m_Timer.GetClockTicks();
}

void            CKernel::fpsEnd()
{
                g_frameEnd = m_Timer.GetClockTicks();

                g_lastSwapDuration = g_frameEnd - g_frameCurrent;

#ifdef __DEBUG_TIMING__
                g_glDuration += g_lastSwapDuration;
#endif

                g_frameTime        = g_frameEnd - g_frameStart;

                if (g_frameTime) g_currentFPS = 1000000.0f / g_frameTime;
}

// END OF FILE


