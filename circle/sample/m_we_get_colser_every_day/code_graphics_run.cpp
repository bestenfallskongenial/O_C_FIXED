#include "kernel.h"

    #define MY_BFR   m_bufferLog[LOG_GLSL_0]                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_bufferLogIndex[LOG_GLSL_0] 

void            CKernel::frmBufferSet               (   vtx_state* v)
{
                glBindFramebuffer(GL_FRAMEBUFFER,0);

                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
}

void            CKernel::frmBufferSwap              (   olg_state* o )
{
                eglSwapBuffers(o->display, o->surface);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
}

void            CKernel::setUniPrg                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t,
                                                    /*  int         gl_current_tex, */
                                                        unsigned    p_validTextureCount )
{
#ifdef __DEBUG_GL__
                debug_gl();
#endif    
                glUseProgram(s->gl_program_id[g_gl_program_current]);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
                if(s->u_time[g_gl_program_current] != -1)  glUniform1f(s->u_time[g_gl_program_current], GLtime);
#ifdef __DEBUG_GL__
                debug_gl();
#endif                
                if(s->u_tres[g_gl_program_current]!= -1 )  glUniform2f(s->u_tres[g_gl_program_current], o->screen_width, o->screen_height);
#ifdef __DEBUG_GL__
                debug_gl();
#endif                
                if(s->u_seed[g_gl_program_current] != -1)  glUniform4f(s->u_seed[g_gl_program_current], g_inOutMatrixFlt[0][RND], 
                                                                                                        g_inOutMatrixFlt[1][RND], 
                                                                                                        g_inOutMatrixFlt[2][RND], 
                                                                                                        g_inOutMatrixFlt[3][RND]);
#ifdef __DEBUG_GL__
                debug_gl();
#endif                                                                                                        
                if(s->u_aud[g_gl_program_current]!= -1 )   glUniform4f(s->u_aud[g_gl_program_current],  g_inOutMatrixFlt[0][AU0], 
                                                                                                        g_inOutMatrixFlt[0][AU1], 
                                                                                                        g_inOutMatrixFlt[0][AU2], 
                                                                                                        g_inOutMatrixFlt[0][AU3]);
#ifdef __DEBUG_GL__
                debug_gl();
#endif                                                                                                        
                if(s->u_col[g_gl_program_current] != -1)   glUniform4f(s->u_col[g_gl_program_current],  0.0f, 0.0f, 0.0f, g_opaque);
#ifdef __DEBUG_GL__
                debug_gl();
#endif                
                if(s->u_par_a[g_gl_program_current] != -1) glUniform4f(s->u_par_a[g_gl_program_current],g_inOutMatrixFlt[0][OUT], 
                                                                                                        g_inOutMatrixFlt[1][OUT], 
                                                                                                        g_inOutMatrixFlt[2][OUT], 
                                                                                                        g_inOutMatrixFlt[3][OUT]);
#ifdef __DEBUG_GL__
                debug_gl();
#endif                                                                                                        
                if(s->u_par_b[g_gl_program_current] != -1) glUniform4f(s->u_par_b[g_gl_program_current],g_inOutMatrixFlt[4][OUT], 
                                                                                                        g_inOutMatrixFlt[5][OUT], 
                                                                                                        g_inOutMatrixFlt[6][OUT], 
                                                                                                        g_inOutMatrixFlt[7][OUT]);
#ifdef __DEBUG_GL__
                debug_gl();
#endif                                                                                                        
                if(s->u_tex_l[g_gl_program_current] != -1) glUniform1i(s->u_tex_l[g_gl_program_current],p_validTextureCount); 

#ifdef __DEBUG_GL__
                debug_gl();
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
                debug_gl();
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
                        debug_gl();
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
                            debug_gl();
#endif   
                        break;

                        default:
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex]);

                            if (t->u_tex_id[g_gl_program_current][0] != -1) glUniform1i(t->u_tex_id[g_gl_program_current][0], 0);
#ifdef __DEBUG_GL__
                            debug_gl();
#endif   
                            glActiveTexture(GL_TEXTURE1);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex + 1]);

                            if (t->u_tex_id[g_gl_program_current][1] != -1) glUniform1i(t->u_tex_id[g_gl_program_current][1], 1);
#ifdef __DEBUG_GL__
                            debug_gl();
#endif   
                        break;
                        }
                    }
#endif   
}

bool            CKernel::setTexBackbuffer           (   glsl_state* s,
                                                        tex_state*  t,
                                                        unsigned    p_validTextureCount )
{
                if (!t->gl_tex_bfr || t->u_tex_bfr[g_gl_program_current] == -1)
                    {
                    return false;
                    }

                GLint f_textureUnits = 0;
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &f_textureUnits);

                unsigned f_usedTextureUnits = 0;

                if (g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] == FLAG_THRESHOLD)
                    {
                    f_usedTextureUnits = p_validTextureCount;
                    }
                else if (p_validTextureCount == 1)
                    {
                    f_usedTextureUnits = 1;
                    }
                else if (p_validTextureCount > 1)
                    {
                    f_usedTextureUnits = 2;
                    }

                if (f_usedTextureUnits >= (unsigned)f_textureUnits)
                    {
                    return false;
                    }

                glActiveTexture(GL_TEXTURE0 + f_usedTextureUnits);
                glBindTexture(GL_TEXTURE_2D, t->gl_tex_bfr);

                glUniform1i(t->u_tex_bfr[g_gl_program_current], f_usedTextureUnits);

#ifdef __DEBUG_GL__
                debug_gl();
#endif

                return true;
}

void            CKernel::captureBackbuffer          (   olg_state*  o,
                                                        tex_state*  t )
{
                if (!t->gl_tex_bfr)
                    {
                    return;
                    }

                glBindTexture(GL_TEXTURE_2D, t->gl_tex_bfr);

                glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0,
                                    o->screen_width, o->screen_height);

#ifdef __DEBUG_GL__
                debug_gl();
#endif
}

void            CKernel::drawGLsPrg                 (   )
{
#ifdef __DEBUG_GL__
                debug_gl();
#endif    
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void            CKernel::setUniOvl                  (   olg_state*  o,
                                                        glsl_state* s,
                                                        tex_state*  t )
{
                glUseProgram(s->gl_program_id[0]);

#ifdef __DEBUG_GL__
                debug_gl();
#endif

                if (s->u_tres[0] != -1) { glUniform2f( s->u_tres[0], o->screen_width, o->screen_height); }


                if (s->u_q00[0] != -1) { glUniform4f( s->u_q00[0], g_menuTarget[MENU_COORD_MODE_0][0], g_menuTarget[MENU_COORD_MODE_0][1], g_menuTarget[MENU_COORD_MODE_0][2], g_menuTarget[MENU_COORD_MODE_0][3]); }
                if (s->u_d00[0] != -1) { glUniform4f( s->u_d00[0], g_menuTarget[MENU_COORD_DETAIL_0][0], g_menuTarget[MENU_COORD_DETAIL_0][1], g_menuTarget[MENU_COORD_DETAIL_0][2], g_menuTarget[MENU_COORD_DETAIL_0][3]); }
                if (s->u_d01[0] != -1) { glUniform4f( s->u_d01[0], g_menuTarget[MENU_COORD_DETAIL_1][0], g_menuTarget[MENU_COORD_DETAIL_1][1], g_menuTarget[MENU_COORD_DETAIL_1][2], g_menuTarget[MENU_COORD_DETAIL_1][3]); }

                if (s->u_q01[0] != -1) { glUniform4f( s->u_q01[0], g_menuTarget[MENU_COORD_MODE_1][0], g_menuTarget[MENU_COORD_MODE_1][1], g_menuTarget[MENU_COORD_MODE_1][2], g_menuTarget[MENU_COORD_MODE_1][3]); }
                if (s->u_d02[0] != -1) { glUniform4f( s->u_d02[0], g_menuTarget[MENU_COORD_DETAIL_2][0], g_menuTarget[MENU_COORD_DETAIL_2][1], g_menuTarget[MENU_COORD_DETAIL_2][2], g_menuTarget[MENU_COORD_DETAIL_2][3]); }
                if (s->u_d03[0] != -1) { glUniform4f( s->u_d03[0], g_menuTarget[MENU_COORD_DETAIL_3][0], g_menuTarget[MENU_COORD_DETAIL_3][1], g_menuTarget[MENU_COORD_DETAIL_3][2], g_menuTarget[MENU_COORD_DETAIL_3][3]); }

                if (s->u_q02[0] != -1) { glUniform4f( s->u_q02[0], g_menuTarget[MENU_COORD_MODE_2][0], g_menuTarget[MENU_COORD_MODE_2][1], g_menuTarget[MENU_COORD_MODE_2][2], g_menuTarget[MENU_COORD_MODE_2][3]); }
                if (s->u_d04[0] != -1) { glUniform4f( s->u_d04[0], g_menuTarget[MENU_COORD_DETAIL_4][0], g_menuTarget[MENU_COORD_DETAIL_4][1], g_menuTarget[MENU_COORD_DETAIL_4][2], g_menuTarget[MENU_COORD_DETAIL_4][3]); }
                if (s->u_d05[0] != -1) { glUniform4f( s->u_d05[0], g_menuTarget[MENU_COORD_DETAIL_5][0], g_menuTarget[MENU_COORD_DETAIL_5][1], g_menuTarget[MENU_COORD_DETAIL_5][2], g_menuTarget[MENU_COORD_DETAIL_5][3]); }

                if (s->u_q03[0] != -1) { glUniform4f( s->u_q03[0], g_menuTarget[MENU_COORD_MODE_3][0], g_menuTarget[MENU_COORD_MODE_3][1], g_menuTarget[MENU_COORD_MODE_3][2], g_menuTarget[MENU_COORD_MODE_3][3]); }
                if (s->u_d06[0] != -1) { glUniform4f( s->u_d06[0], g_menuTarget[MENU_COORD_DETAIL_6][0], g_menuTarget[MENU_COORD_DETAIL_6][1], g_menuTarget[MENU_COORD_DETAIL_6][2], g_menuTarget[MENU_COORD_DETAIL_6][3]); }
                if (s->u_d07[0] != -1) { glUniform4f( s->u_d07[0], g_menuTarget[MENU_COORD_DETAIL_7][0], g_menuTarget[MENU_COORD_DETAIL_7][1], g_menuTarget[MENU_COORD_DETAIL_7][2], g_menuTarget[MENU_COORD_DETAIL_7][3]); }

                if (s->u_t00[0] != -1) { glUniform4f( s->u_t00[0], g_menuTarget[MENU_COORD_TARGET_TIME][0], g_menuTarget[MENU_COORD_TARGET_TIME][1], g_menuTarget[MENU_COORD_TARGET_TIME][2], g_menuTarget[MENU_COORD_TARGET_TIME][3]); }
                if (s->u_t01[0] != -1) { glUniform4f( s->u_t01[0], g_menuTarget[MENU_COORD_TARGET_TEXTURE][0], g_menuTarget[MENU_COORD_TARGET_TEXTURE][1], g_menuTarget[MENU_COORD_TARGET_TEXTURE][2], g_menuTarget[MENU_COORD_TARGET_TEXTURE][3]); }
                if (s->u_t02[0] != -1) { glUniform4f( s->u_t02[0], g_menuTarget[MENU_COORD_TARGET_VIDEO][0], g_menuTarget[MENU_COORD_TARGET_VIDEO][1], g_menuTarget[MENU_COORD_TARGET_VIDEO][2], g_menuTarget[MENU_COORD_TARGET_VIDEO][3]); }
                if (s->u_t03[0] != -1) { glUniform4f( s->u_t03[0], g_menuTarget[MENU_COORD_TARGET_FRAME][0], g_menuTarget[MENU_COORD_TARGET_FRAME][1], g_menuTarget[MENU_COORD_TARGET_FRAME][2], g_menuTarget[MENU_COORD_TARGET_FRAME][3]); }

                if (s->u_bpm[0] != -1) { glUniform4f(s->u_bpm[0], g_menuTarget[MENU_COORD_BPM_STRING][0], g_menuTarget[MENU_COORD_BPM_STRING][1], g_menuTarget[MENU_COORD_BPM_STRING][2], g_menuTarget[MENU_COORD_BPM_STRING][3]); }
                if (s->u_100[0] != -1) { glUniform4f(s->u_100[0], g_menuTarget[MENU_COORD_BPM_100][0], g_menuTarget[MENU_COORD_BPM_100][1], g_menuTarget[MENU_COORD_BPM_100][2], g_menuTarget[MENU_COORD_BPM_100][3]); }
                if (s->u_010[0] != -1) { glUniform4f(s->u_010[0], g_menuTarget[MENU_COORD_BPM_010][0], g_menuTarget[MENU_COORD_BPM_010][1], g_menuTarget[MENU_COORD_BPM_010][2], g_menuTarget[MENU_COORD_BPM_010][3]); }
                if (s->u_001[0] != -1) { glUniform4f(s->u_001[0], g_menuTarget[MENU_COORD_BPM_001][0], g_menuTarget[MENU_COORD_BPM_001][1], g_menuTarget[MENU_COORD_BPM_001][2], g_menuTarget[MENU_COORD_BPM_001][3]); }
                if (s->u_dot[0] != -1) { glUniform4f(s->u_dot[0], g_menuTarget[MENU_COORD_BPM_DOT][0], g_menuTarget[MENU_COORD_BPM_DOT][1], g_menuTarget[MENU_COORD_BPM_DOT][2], g_menuTarget[MENU_COORD_BPM_DOT][3]); }
                if (s->u_10d[0] != -1) { glUniform4f(s->u_10d[0], g_menuTarget[MENU_COORD_BPM_10D][0], g_menuTarget[MENU_COORD_BPM_10D][1], g_menuTarget[MENU_COORD_BPM_10D][2], g_menuTarget[MENU_COORD_BPM_10D][3]); }
                if (s->u_01d[0] != -1) { glUniform4f(s->u_01d[0], g_menuTarget[MENU_COORD_BPM_01D][0], g_menuTarget[MENU_COORD_BPM_01D][1], g_menuTarget[MENU_COORD_BPM_01D][2], g_menuTarget[MENU_COORD_BPM_01D][3]); }

                
#ifdef __DEBUG_GL__
                debug_gl();
#endif
}

void            CKernel::setTexOvl                  (   olg_state*  o,
                                                        glsl_state* s,
                                                        tex_state*  t )
{
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[0]);

                if (t->u_tex_id[0][0] != -1) glUniform1i(t->u_tex_id[0][0], 0);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
}

void            CKernel::drawGLsOvl                 (   )
{
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // shall fix the "no alfa in my 24bit bmp texture bitmap" issue
            //  glBlendFunc(GL_ZERO, GL_SRC_COLOR);                 // Because the atlas texture is uploaded as GL_RGB, use multiplicative blending. The current upload uses GL_RGB
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glDisable(GL_BLEND);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
}

void            CKernel::fpsBegin()
{
                g_frameStart = getClockMilliseconds();
}

void            CKernel::fpsBreak()
{
#ifdef __DEBUG_TIMING__    
                g_runtimeDuration = getClockMilliseconds() - g_frameStart;
#endif                
                glFlush();
#ifdef __DEBUG_GL__
                debug_gl();
#endif
#ifdef __DEBUG_TIMING__
                g_glDuration = (getClockMilliseconds() - g_frameStart) - g_runtimeDuration;                
#endif
                g_frameCurrent = getClockMilliseconds();
                g_frameTarget  = g_frameStart + (1000 / TARGET_FPS);

                if (g_limitFPS && g_frameTarget > g_frameCurrent + g_lastSwapDuration)
                    {
                    g_frameDelay = g_frameTarget - (g_frameCurrent + g_lastSwapDuration);

                    msDelay(g_frameDelay);
                    }
                g_frameCurrent = getClockMilliseconds();
}

void            CKernel::fpsEnd()
{
                g_frameEnd = getClockMilliseconds();

                g_lastSwapDuration = g_frameEnd - g_frameCurrent;
#ifdef __DEBUG_TIMING__
                g_glDuration += g_lastSwapDuration;
#endif
                g_frameTime        = g_frameEnd - g_frameStart;

                if (g_frameTime) g_currentFPS = 1000.0f / g_frameTime;
}

