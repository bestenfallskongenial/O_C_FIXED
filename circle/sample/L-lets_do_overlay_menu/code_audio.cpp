#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_logKernelIndex 

void            CKernel::adc_ProcessAudio           (   void    )   // Model 1 - two pipiline, two bands each
{
            //  if (!m_audio_mode_activated) return; // is a fixed position in g_centralModeBuffer mapped by modeMenuAssignGroup()

                int i0 = m_adc_index & 3;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

                int w0 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63) + 1;
                int w1 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63) + 1;
                int w2 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63) + 1;
                int w3 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63) + 1;

                g_audioIreg[0] = m_adc_ring[0][i0] - m_adc_ring[0][i1] + m_adc_ring[0][i2] - m_adc_ring[0][i3];

                if (g_audioIreg[0] > AUDIO_THRESHOLD || g_audioIreg[0] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 0;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[0][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }

                g_audioIreg[1] = m_adc_ring[1][i0] - m_adc_ring[1][i1] + m_adc_ring[1][i2] - m_adc_ring[1][i3];

                if (g_audioIreg[1] > AUDIO_THRESHOLD || g_audioIreg[1] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 1;

                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = 1;

                    int s = m_adc_ring[1][i0];

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }

                g_audioIreg[2] = m_adc_ring[2][i0] - m_adc_ring[2][i1] + m_adc_ring[2][i2] - m_adc_ring[2][i3];

                if (g_audioIreg[2] > AUDIO_THRESHOLD || g_audioIreg[2] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 2;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[2][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }

                g_audioIreg[3] = m_adc_ring[3][i0] - m_adc_ring[3][i1] + m_adc_ring[3][i2] - m_adc_ring[3][i3];

                if (g_audioIreg[3] > AUDIO_THRESHOLD || g_audioIreg[3] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 3;

                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = 1;

                    int s = m_adc_ring[3][i0];

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }
/*
                if (m_audio_hold_A > 0) --m_audio_hold_A;
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = (m_audio_hold_A > 0);

                if (m_audio_hold_B > 0) --m_audio_hold_B;
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = (m_audio_hold_B > 0);
*/
}

void            CKernel::adc_ProcessAudio_1_2           (   void    )   // Model 2 — one pipeline, two bands
{
                int i0 = m_adc_index & 3;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

                int w0 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63) + 1;
                int w1 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63) + 1;

                g_audioIreg[0] = m_adc_ring[0][i0] - m_adc_ring[0][i1] + m_adc_ring[0][i2] - m_adc_ring[0][i3];

                if (g_audioIreg[0] > AUDIO_THRESHOLD || g_audioIreg[0] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 0;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[0][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }

                g_audioIreg[1] = m_adc_ring[1][i0] - m_adc_ring[1][i1] + m_adc_ring[1][i2] - m_adc_ring[1][i3];

                if (g_audioIreg[1] > AUDIO_THRESHOLD || g_audioIreg[1] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 1;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[1][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }

                g_audioIreg[2] = m_adc_ring[2][i0] - m_adc_ring[2][i1] + m_adc_ring[2][i2] - m_adc_ring[2][i3];

                if (g_audioIreg[2] > AUDIO_THRESHOLD || g_audioIreg[2] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 2;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[2][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }

                g_audioIreg[3] = m_adc_ring[3][i0] - m_adc_ring[3][i1] + m_adc_ring[3][i2] - m_adc_ring[3][i3];

                if (g_audioIreg[3] > AUDIO_THRESHOLD || g_audioIreg[3] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 3;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[3][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }
/*
                if (m_audio_hold_A > 0) --m_audio_hold_A;
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = (m_audio_hold_A > 0);
*/
}

void            CKernel::adc_ProcessAudio_1_4           (   void    )   // Model 3 — one pipeline, four bands
{
                int i0 = m_adc_index & 3;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

                int w0 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63) + 1;
                int w1 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63) + 1;
                int w2 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63) + 1;
                int w3 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63) + 1;

                g_audioIreg[0] = m_adc_ring[0][i0] - m_adc_ring[0][i1] + m_adc_ring[0][i2] - m_adc_ring[0][i3];

                if (g_audioIreg[0] > AUDIO_THRESHOLD || g_audioIreg[0] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 0;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[0][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }

                g_audioIreg[1] = m_adc_ring[1][i0] - m_adc_ring[1][i1] + m_adc_ring[1][i2] - m_adc_ring[1][i3];

                if (g_audioIreg[1] > AUDIO_THRESHOLD || g_audioIreg[1] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 1;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[1][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }

                g_audioIreg[2] = m_adc_ring[2][i0] - m_adc_ring[2][i1] + m_adc_ring[2][i2] - m_adc_ring[2][i3];

                if (g_audioIreg[2] > AUDIO_THRESHOLD || g_audioIreg[2] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 2;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[2][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }

                g_audioIreg[3] = m_adc_ring[3][i0] - m_adc_ring[3][i1] + m_adc_ring[3][i2] - m_adc_ring[3][i3];

                if (g_audioIreg[3] > AUDIO_THRESHOLD || g_audioIreg[3] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 3;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[3][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }
/*
                if (m_audio_hold_A > 0) --m_audio_hold_A;       // look at code_util.cpp -> CKernel::updateTimeouts()
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = (m_audio_hold_A > 0);
*/
}