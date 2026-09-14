void CKernel::adc_ProcessAudio(void)
{
                int i0 = m_adc_index & 3;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

                int w0 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63) + 1;
                int w1 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63) + 1;
                int w2 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63) + 1;
                int w3 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63) + 1;

                int detectedChannel = -1;

                for (int channel = 0; channel < 4; ++channel)
                    {
                    g_audioIreg[channel] =
                        m_adc_ring[channel][i0]
                      - m_adc_ring[channel][i1]
                      + m_adc_ring[channel][i2]
                      - m_adc_ring[channel][i3];

                    if (g_audioIreg[channel] > AUDIO_THRESHOLD
                        || g_audioIreg[channel] < -AUDIO_THRESHOLD)
                        {
                        detectedChannel = channel;
                        break;
                        }
                    }

                if (detectedChannel >= 0)
                    {
                    bool f_audio_was_off =
                        g_centralModeBuffer
                            [g_currentProgramBuffer]
                            [FLAG_AUDIO_A] == 0;

                    is_audio[0] = detectedChannel;
                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;

                    g_centralModeBuffer
                        [g_currentProgramBuffer]
                        [FLAG_AUDIO_A] = 1;

                    if (f_audio_was_off)
                        {
                        for (int channel = 0; channel < 8; ++channel)
                            {
                            unsigned& mode =
                                g_centralModeBuffer
                                    [g_currentProgramBuffer]
                                    [channel];

                            if (mode >= 5)
                                {
                                mode += 4;
                                g_menuPickUpFlag[channel] = false;
                                }
                            }
                        }

                    int s = m_adc_ring[is_audio[0]][i0];

                    if (m_idx0 >= w0)
                        {
                        m_idx0 = 0;
                        }

                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;

                    g_inOutMatrixInt[0][AU0] =
                        m_sum[0] / w0;

                    g_inOutMatrixFlt[0][AU0] =
                        (float)g_inOutMatrixInt[0][AU0] / 1023.0f;

                    if (++m_idx0 >= w0)
                        {
                        m_idx0 = 0;
                        }

                    if (m_idx1 >= w1)
                        {
                        m_idx1 = 0;
                        }

                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;

                    g_inOutMatrixInt[0][AU1] =
                        m_sum[1] / w1;

                    g_inOutMatrixFlt[0][AU1] =
                        (float)g_inOutMatrixInt[0][AU1] / 1023.0f;

                    if (++m_idx1 >= w1)
                        {
                        m_idx1 = 0;
                        }

                    if (m_idx2 >= w2)
                        {
                        m_idx2 = 0;
                        }

                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;

                    g_inOutMatrixInt[0][AU2] =
                        m_sum[2] / w2;

                    g_inOutMatrixFlt[0][AU2] =
                        (float)g_inOutMatrixInt[0][AU2] / 1023.0f;

                    if (++m_idx2 >= w2)
                        {
                        m_idx2 = 0;
                        }

                    if (m_idx3 >= w3)
                        {
                        m_idx3 = 0;
                        }

                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;

                    g_inOutMatrixInt[0][AU3] =
                        m_sum[3] / w3;

                    g_inOutMatrixFlt[0][AU3] =
                        (float)g_inOutMatrixInt[0][AU3] / 1023.0f;

                    if (++m_idx3 >= w3)
                        {
                        m_idx3 = 0;
                        }
                    }
                else
                    {
                    if (m_audio_hold_A > 0)
                        {
                        --m_audio_hold_A;
                        }

                    if (m_audio_hold_A == 0
                        && g_centralModeBuffer
                            [g_currentProgramBuffer]
                            [FLAG_AUDIO_A])
                        {
                        for (int channel = 0; channel < 8; ++channel)
                            {
                            unsigned& mode =
                                g_centralModeBuffer
                                    [g_currentProgramBuffer]
                                    [channel];

                            if (mode >= 5 && mode < 9)
                                {
                                mode = 0;
                                g_menuPickUpFlag[channel] = false;
                                }
                            else if (mode >= 9)
                                {
                                mode -= 4;
                                g_menuPickUpFlag[channel] = false;
                                }
                            }

                        is_audio[0] = -1;

                        g_centralModeBuffer
                            [g_currentProgramBuffer]
                            [FLAG_AUDIO_A] = 0;
                        }
                    }
}

void CKernel::adc_ProcessAudio(void)
{
                int i0 = m_adc_index & 3;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

                int w0 =
                    (g_centralModeBuffer
                        [g_currentProgramBuffer]
                        [SENS_A] & 63) + 1;

                int w1 =
                    (g_centralModeBuffer
                        [g_currentProgramBuffer]
                        [SENS_B] & 63) + 1;

                bool audioDetected = false;

                for (int channel = 0; channel < 4; ++channel)
                    {
                    g_audioIreg[channel] =
                        m_adc_ring[channel][i0]
                      - m_adc_ring[channel][i1]
                      + m_adc_ring[channel][i2]
                      - m_adc_ring[channel][i3];

                    if (g_audioIreg[channel] > AUDIO_THRESHOLD
                        || g_audioIreg[channel] < -AUDIO_THRESHOLD)
                        {
                        is_audio = channel;
                        audioDetected = true;

                        break;
                        }
                    }

                if (audioDetected)
                    {
                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;

                    g_centralModeBuffer
                        [g_currentProgramBuffer]
                        [FLAG_AUDIO_A] = 1;

                    int sample =
                        m_adc_ring[is_audio][i0];

                    if (m_idx0 >= w0)
                        {
                        m_idx0 = 0;
                        }

                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = sample;
                    m_sum[0] += sample;

                    g_inOutMatrixInt[0][AU0] =
                        m_sum[0] / w0;

                    g_inOutMatrixFlt[0][AU0] =
                        (float)g_inOutMatrixInt[0][AU0]
                        / 1023.0f;

                    if (++m_idx0 >= w0)
                        {
                        m_idx0 = 0;
                        }

                    if (m_idx1 >= w1)
                        {
                        m_idx1 = 0;
                        }

                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = sample;
                    m_sum[1] += sample;

                    g_inOutMatrixInt[0][AU1] =
                        m_sum[1] / w1;

                    g_inOutMatrixFlt[0][AU1] =
                        (float)g_inOutMatrixInt[0][AU1]
                        / 1023.0f;

                    if (++m_idx1 >= w1)
                        {
                        m_idx1 = 0;
                        }
                    }

                if (m_audio_hold_A > 0)
                    {
                    --m_audio_hold_A;
                    }

                if (m_audio_hold_A == 0)
                    {
                    is_audio = -1;

                    g_centralModeBuffer
                        [g_currentProgramBuffer]
                        [FLAG_AUDIO_A] = 0;
                    }
}

