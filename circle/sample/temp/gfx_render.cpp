// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// new render calls !!! 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::frmBufferSet(vtx_state* v)
{
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#ifdef __GL_DEBUG__
    check();
#endif

    glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);
#ifdef __GL_DEBUG__
    check();
#endif
}


void CKernel::setUniPrg(olg_state* o, glsl_state* s, tex_state* t)
{
    glUseProgram(s->gl_program_id[g_current_gl_program]);
#ifdef __GL_DEBUG__
    check();
#endif

    GLuint cx = o->screen_width;
    GLuint cy = o->screen_height;

    if(s->u_time[g_current_gl_program] != -1)  glUniform1f(s->u_time[g_current_gl_program],   GLtime);
    if(s->u_tres[g_current_gl_program]!= -1 )  glUniform2f(s->u_tres[g_current_gl_program],   cx, cy);
    if(s->u_seed[g_current_gl_program] != -1)  glUniform4f(s->u_seed[g_current_gl_program],   g_inOutMatrixFlt[0][RND], 
                                                                                                        g_inOutMatrixFlt[1][RND], 
                                                                                                        g_inOutMatrixFlt[2][RND], 
                                                                                                        g_inOutMatrixFlt[3][RND]);
    if(s->u_aud[g_current_gl_program]!= -1 )   glUniform4f(s->u_aud[g_current_gl_program],    g_inOutMatrixFlt[0][AU0], 
                                                                                                        g_inOutMatrixFlt[0][AU1], 
                                                                                                        g_inOutMatrixFlt[0][AU2], 
                                                                                                        g_inOutMatrixFlt[0][AU3]);
    if(s->u_col[g_current_gl_program] != -1)   glUniform4f(s->u_col[g_current_gl_program],    0.0f, 0.0f, 0.0f, g_opaque);    // is a stub for a potential color mode, alfa was to see my text display
    if(s->u_par_a[g_current_gl_program] != -1) glUniform4f(s->u_par_a[g_current_gl_program],  g_inOutMatrixFlt[0][OUT], 
                                                                                                        g_inOutMatrixFlt[1][OUT], 
                                                                                                        g_inOutMatrixFlt[2][OUT], 
                                                                                                        g_inOutMatrixFlt[3][OUT]);
    if(s->u_par_b[g_current_gl_program] != -1) glUniform4f(s->u_par_b[g_current_gl_program],  g_inOutMatrixFlt[4][OUT], 
                                                                                                        g_inOutMatrixFlt[5][OUT], 
                                                                                                        g_inOutMatrixFlt[6][OUT], 
                                                                                                        g_inOutMatrixFlt[7][OUT]);
    if(s->u_tex_l[g_current_gl_program] != -1) glUniform1i(s->u_tex_l[g_current_gl_program],  p_validTextureCount);           // relict? 
#ifdef __GL_DEBUG__
    check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::setTexPrg( glsl_state* s, tex_state* t, int& gl_current_tex)
{
#ifdef __H264_DEBUG_TEX__

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureA);

    if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);

#ifdef __GL_DEBUG__
    check();
#endif

#endif

#ifndef __H264_DEBUG_TEX__

    switch(g_centralModeBuffer[g_currentProgramBuffer][TEX_MODE])
    {
    case false:
        for (int i = 0; i < p_validTextureCount; i++)   // we need to implement 
        {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[i]);

            if (t->u_tex_id[g_current_gl_program][i] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][i], i);

#ifdef __GL_DEBUG__
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
            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[gl_current_tex]);

            if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);

#ifdef __GL_DEBUG__
            check();
#endif
            break;

        default:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[gl_current_tex]);

            if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);

#ifdef __GL_DEBUG__
            check();
#endif

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[gl_current_tex + 1]);

            if (t->u_tex_id[g_current_gl_program][1] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][1], 1);

#ifdef __GL_DEBUG__
            check();
#endif
            break;
        }
        break;
    }

#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::drawGLsPrg(glsl_state* m_glsl)
{
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#ifdef __GL_DEBUG__
    check();
#endif

    glBindBuffer(GL_ARRAY_BUFFER, 0);
/*
    glFlush();

    if (noTargetFPS==true)  // <- this is my "fps break" - explain why
    {
        glFinish();
#ifdef __GL_DEBUG__
        check();
#endif
    }
*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void frmRateBreak()
{
    glFlush();

    if (noTargetFPS==true)  // <- this is my "fps break" - explain why
        {
        glFinish();
#ifdef __GL_DEBUG__
        check();
#endif
        }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::frmBufferSwap(glsl_state* m_glsl)
{
    eglSwapBuffers(m_glsl->display, m_glsl->surface);
#ifdef __GL_DEBUG__
    check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::updateOvlState(menu_glsl_state* m_menu)
{
            const float ox = m_menu->kMenuOrigin[0];
            const float oy = m_menu->kMenuOrigin[1];
            const float tw = m_menu->kMenuTileSize[0];
            const float th = m_menu->kMenuTileSize[1];

            for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
                {
                float sx = m_menu->kMenuRelSize[i][0];
                float sy = m_menu->kMenuRelSize[i][1];

                if (i == 14 || i == 15)
                    {
                    sx *= m_menu->kMenuBackgroundScale[0];
                    sy *= m_menu->kMenuBackgroundScale[1];
                    }

                m_menu->tile_rect_x[i] = ox + m_menu->kMenuRelPos[i][0] * tw;
                m_menu->tile_rect_y[i] = oy + m_menu->kMenuRelPos[i][1] * th;
                m_menu->tile_rect_w[i] = sx * tw;
                m_menu->tile_rect_h[i] = sy * th;

                m_menu->tile_rect[i * 4 + 0] = m_menu->tile_rect_x[i];
                m_menu->tile_rect[i * 4 + 1] = m_menu->tile_rect_y[i];
                m_menu->tile_rect[i * 4 + 2] = m_menu->tile_rect_w[i];
                m_menu->tile_rect[i * 4 + 3] = m_menu->tile_rect_h[i];
                }

                m_menu->tile_index[0] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH0_MODE];
                m_menu->tile_index[1] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH1_MODE];
                m_menu->tile_index[2] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH2_MODE];
                m_menu->tile_index[3] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH3_MODE];
                m_menu->tile_index[4] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH4_MODE];
                m_menu->tile_index[5] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH5_MODE];
                m_menu->tile_index[6] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH6_MODE];
                m_menu->tile_index[7] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH7_MODE];

                const unsigned long bpm0 = g_resultBPM[0] % 10000UL;
                const unsigned long bpm1 = g_resultBPM[1] % 10UL;

                m_menu->tile_index[8]  = (GLfloat) ((bpm0 / 1000UL) % 10UL);
                m_menu->tile_index[9]  = (GLfloat) ((bpm0 / 100UL) % 10UL);
                m_menu->tile_index[10] = (GLfloat) ((bpm0 / 10UL) % 10UL);
                m_menu->tile_index[11] = (GLfloat) (bpm0 % 10UL);
                m_menu->tile_index[12] = 10.0f;
                m_menu->tile_index[13] = (GLfloat) bpm1;
                m_menu->tile_index[14] = 48.0f;
                m_menu->tile_index[15] = 49.0f;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::setUniOvl(menu_glsl_state* m_menu)
{
                glUseProgram(m_menu->gl_omp_id[0]);
#ifdef __GL_DEBUG__
                check();
#endif

                if (m_menu->u_tile_count[0] != -1) glUniform1i(m_menu->u_tile_count[0], MENU_GPU_TILE_COUNT);

                if (m_menu->u_tile_rect[0] != -1) glUniform4fv(m_menu->u_tile_rect[0], MENU_GPU_TILE_COUNT, m_menu->tile_rect);

                if (m_menu->u_tile_index[0] != -1) glUniform1fv(m_menu->u_tile_index[0], MENU_GPU_TILE_COUNT, m_menu->tile_index);
#ifdef __GL_DEBUG__
                check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::setTexOvl(menu_glsl_state* m_menu)
{
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_menu->gl_omt_id[0]);

                if (m_menu->u_atlas[0] != -1) glUniform1i(m_menu->u_atlas[0], 0);
#ifdef __GL_DEBUG__
                check();
#endif
}

void            CKernel::drawGLsOvl()
{
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glDisable(GL_BLEND);
#ifdef __GL_DEBUG__
                check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
frmBufferSet(&m_glsl);

// pass 1
setUniPrg(&m_glsl);
setTexPrg(&m_glsl);
drawGLsPrg(&m_glsl);

// pass 2
updateOvlState(&m_menu);
setUniOvl(&m_menu);
setTexOvl(&m_menu);
drawGLsOvl();

frmRateBreak()

frmBufferSwap(&m_glsl);
*/