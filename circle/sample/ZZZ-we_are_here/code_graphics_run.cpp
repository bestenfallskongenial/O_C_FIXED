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
/*
void            CKernel::drawGLsOvl                 (   vtx_state* v )
{
                if (g_overlayVertexCount == 0) return;

                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);
                glBufferSubData(GL_ARRAY_BUFFER,
                                OVERLAY_FIRST_VERTEX * OVERLAY_FLOAT_PER_VERTEX * sizeof(GLfloat),
                                g_overlayVertexCount * OVERLAY_FLOAT_PER_VERTEX * sizeof(GLfloat),
                                g_overlayVertices);

                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
                glDrawArrays(GL_TRIANGLES, OVERLAY_FIRST_VERTEX, g_overlayVertexCount);
                glDisable(GL_BLEND);
#ifdef __DEBUG_GL__
                debug_gl();
#endif
}
*/
void            CKernel::drawGLsOvl                 (   vtx_state* v )
{
                if (g_overlayVertexCount == 0) return;

                const GLfloat offsetX = 2.0f / static_cast<GLfloat>(m_ogl.screen_width);
                const GLfloat offsetY = 2.0f / static_cast<GLfloat>(m_ogl.screen_height);
                GLfloat greyVertices[OVERLAY_FLOAT_COUNT];

                for (unsigned i = 0; i < g_overlayVertexCount * OVERLAY_FLOAT_PER_VERTEX; i += OVERLAY_FLOAT_PER_VERTEX)
                    {
                    greyVertices[i + 0] = g_overlayVertices[i + 0] + offsetX;
                    greyVertices[i + 1] = g_overlayVertices[i + 1] - offsetY;
                    greyVertices[i + 2] = g_overlayVertices[i + 2];
                    greyVertices[i + 3] = g_overlayVertices[i + 3];
                    }

                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);

                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);

                // Grey copy: 96-pixel tiles, one pixel right and down.
                glBufferSubData(GL_ARRAY_BUFFER,
                                OVERLAY_FIRST_VERTEX * OVERLAY_FLOAT_PER_VERTEX * sizeof(GLfloat),
                                g_overlayVertexCount * OVERLAY_FLOAT_PER_VERTEX * sizeof(GLfloat),
                                greyVertices);

                if (m_osh.u_col[0] != -1) glUniform4f(m_osh.u_col[0], 0.5f, 0.5f, 0.5f, 0.8f);      // last is the tile opacity

                glDrawArrays(GL_TRIANGLES, OVERLAY_FIRST_VERTEX, g_overlayVertexCount);

                // Original overlay: 96-pixel tiles, black.
                glBufferSubData(GL_ARRAY_BUFFER,
                                OVERLAY_FIRST_VERTEX * OVERLAY_FLOAT_PER_VERTEX * sizeof(GLfloat),
                                g_overlayVertexCount * OVERLAY_FLOAT_PER_VERTEX * sizeof(GLfloat),
                                g_overlayVertices);

                if (m_osh.u_col[0] != -1) glUniform4f(m_osh.u_col[0], 0.0f, 0.0f, 0.0f, 0.8f);      // last is the tile opacity 

                glDrawArrays(GL_TRIANGLES, OVERLAY_FIRST_VERTEX, g_overlayVertexCount);

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

