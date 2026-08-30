#include "kernel.h"

//  #undef  __DEBUG_CHECK__
    #define __DEBUG_CHECK__

    #define MY_BUFFER   m_bufferLog                 // not used here
    #define MY_INDEX    m_bufferLogIndex

void            CKernel::frmBufferSet               (   vtx_state* v)
{
                glBindFramebuffer(GL_FRAMEBUFFER,0);

                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#ifdef __DEBUG_CHECK__
                check();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);
#ifdef __DEBUG_CHECK__
                check();
#endif
}

void            CKernel::frmBufferSwap              (   olg_state* o )
{
                eglSwapBuffers(o->display, o->surface);
#ifdef __DEBUG_CHECK__
                check();
#endif
}

void            CKernel::setUniPrg                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t,
                                                    /*  int         gl_current_tex, */
                                                        unsigned    p_validTextureCount )
{
                glUseProgram(s->gl_program_id[g_current_gl_program]);
#ifdef __DEBUG_CHECK__
                check();
#endif
                if(s->u_time[g_current_gl_program] != -1)  glUniform1f(s->u_time[g_current_gl_program], GLtime);
                if(s->u_tres[g_current_gl_program]!= -1 )  glUniform2f(s->u_tres[g_current_gl_program], o->screen_width, o->screen_width);
                if(s->u_seed[g_current_gl_program] != -1)  glUniform4f(s->u_seed[g_current_gl_program], g_inOutMatrixFlt[0][RND], 
                                                                                                        g_inOutMatrixFlt[1][RND], 
                                                                                                        g_inOutMatrixFlt[2][RND], 
                                                                                                        g_inOutMatrixFlt[3][RND]);
                if(s->u_aud[g_current_gl_program]!= -1 )   glUniform4f(s->u_aud[g_current_gl_program],  g_inOutMatrixFlt[0][AU0], 
                                                                                                        g_inOutMatrixFlt[0][AU1], 
                                                                                                        g_inOutMatrixFlt[0][AU2], 
                                                                                                        g_inOutMatrixFlt[0][AU3]);
                if(s->u_col[g_current_gl_program] != -1)   glUniform4f(s->u_col[g_current_gl_program],  0.0f, 0.0f, 0.0f, g_opaque);
                if(s->u_par_a[g_current_gl_program] != -1) glUniform4f(s->u_par_a[g_current_gl_program],g_inOutMatrixFlt[0][OUT], 
                                                                                                        g_inOutMatrixFlt[1][OUT], 
                                                                                                        g_inOutMatrixFlt[2][OUT], 
                                                                                                        g_inOutMatrixFlt[3][OUT]);
                if(s->u_par_b[g_current_gl_program] != -1) glUniform4f(s->u_par_b[g_current_gl_program],g_inOutMatrixFlt[4][OUT], 
                                                                                                        g_inOutMatrixFlt[5][OUT], 
                                                                                                        g_inOutMatrixFlt[6][OUT], 
                                                                                                        g_inOutMatrixFlt[7][OUT]);
                if(s->u_tex_l[g_current_gl_program] != -1) glUniform1i(s->u_tex_l[g_current_gl_program],p_validTextureCount); 

#ifdef __DEBUG_CHECK__
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

                if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);
#ifdef __DEBUG_CHECK__
                check();
#endif   

#endif   

#ifndef __H264_DEBUG_TEX__
                switch(g_centralModeBuffer[g_currentProgramBuffer][TEX_MODE])
                    {
                    case false:
                        for (unsigned i = 0; i < p_validTextureCount; i++)
                            {
                            glActiveTexture(GL_TEXTURE0+i);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[i]);

                            if (t->u_tex_id[g_current_gl_program][i] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][i], i);
#ifdef __DEBUG_CHECK__
                            check();
#endif   
                            }
                    break;

                    case true:
                        switch(p_validTextureCount)
                        {
                        case 0:
                        break;

                        case 1:
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex]);

                            if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);
#ifdef __DEBUG_CHECK__
                            check();
#endif   
                        break;
                        default:
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex]);

                            if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);
#ifdef __DEBUG_CHECK__
                            check();
#endif   
                            glActiveTexture(GL_TEXTURE1);
                            glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[gl_current_tex + 1]);

                            if (t->u_tex_id[g_current_gl_program][1] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][1], 1);
#ifdef __DEBUG_CHECK__
                            check();
#endif   
                        break;
                        }
                    break;
                    }
#endif   
}

void            CKernel::drawGLsPrg                 (   )
{
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#ifdef __DEBUG_CHECK__
                check();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void            CKernel::frmRateBreak               (   bool noTargetFPS )
{
                glFlush();

                if ( noTargetFPS )
                    {
                    glFinish();
#ifdef __DEBUG_CHECK__
                    check();
#endif
                    }
}

void            CKernel::setUniOvl                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t )
{
                glUseProgram(s->gl_program_id[0]);
#ifdef __DEBUG_CHECK__
                check();
#endif
                if (s->u_tile_count[0] != -1) glUniform1i(s->u_tile_count[0], MENU_GPU_TILE_COUNT);

                if (s->u_tile_rect[0] != -1) glUniform4fv(s->u_tile_rect[0], MENU_GPU_TILE_COUNT, s->tile_rect);

                if (s->u_tile_index[0] != -1) glUniform1fv(s->u_tile_index[0], MENU_GPU_TILE_COUNT, s->tile_index);
#ifdef __DEBUG_CHECK__
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
#ifdef __DEBUG_CHECK__
                check();
#endif
}

void            CKernel::drawGLsOvl                 (   )
{
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glDisable(GL_BLEND);
#ifdef __DEBUG_CHECK__
                check();
#endif
}
// END OF FILE
