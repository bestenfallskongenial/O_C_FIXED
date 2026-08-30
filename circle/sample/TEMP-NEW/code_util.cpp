#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex

bool            CKernel::setSystemVariables()
{
            //  FOR NOW A DUMMY!!!
            g_centralModeBuffer[g_currentProgramBuffer][SEL_PRG] = 7;
                return true;
}   

bool            CKernel::checkUpdate                (   )
{
                if (filecounter[FT_KLN][FLD_LOADED] == 2 ) // or is 1 correct? 
                    {
                    return true;
                    }
                else
                    {
                    return false;    
                    }
}

void            CKernel::randomVec8                 (   uint32_t            p_seed )
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;

                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[0][RND] = p_seed * f_scale;
                g_inOutMatrixInt[0][RND] = ( g_inOutMatrixFlt[0][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[1][RND] = p_seed * f_scale;
                g_inOutMatrixInt[1][RND] = ( g_inOutMatrixFlt[1][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[2][RND] = p_seed * f_scale;
                g_inOutMatrixInt[2][RND] = ( g_inOutMatrixFlt[2][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[3][RND] = p_seed * f_scale;
                g_inOutMatrixInt[3][RND] = ( g_inOutMatrixFlt[3][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[4][RND] = p_seed * f_scale;
                g_inOutMatrixInt[4][RND] = ( g_inOutMatrixFlt[4][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[5][RND] = p_seed * f_scale;
                g_inOutMatrixInt[5][RND] = ( g_inOutMatrixFlt[5][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[6][RND] = p_seed * f_scale;
                g_inOutMatrixInt[6][RND] = ( g_inOutMatrixFlt[6][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[7][RND] = p_seed * f_scale;
                g_inOutMatrixInt[7][RND] = ( g_inOutMatrixFlt[7][RND] * f_max_int);
}

void            CKernel::calculate1BPMnew           (   int             p_source,
                                                        int             p_timeBuffer,
                                                        int             p_deltaBuffer,
                                                        unsigned long   p_triggerTimeClock)
{
                unsigned long f_intervalAverage = 0;

                if (p_triggerTimeClock != g_lfoBpmMatrix[p_source][LTIME])
                    {
                    int f_writeIndex                       =   g_lfoBpmMatrix[p_source][TIDX];

                    g_lfoBpmMatrix[f_writeIndex][p_timeBuffer] = p_triggerTimeClock;

                    int f_index0                           =   (f_writeIndex + 1) % 4;
                    int f_index1                           =   (f_writeIndex + 2) % 4;
                    int f_index2                           =   (f_writeIndex + 3) % 4;
                    int f_index3                           =   f_writeIndex;

                    g_lfoBpmMatrix[0][p_deltaBuffer]       =   g_lfoBpmMatrix[f_index1][p_timeBuffer] - g_lfoBpmMatrix[f_index0][p_timeBuffer];
                    g_lfoBpmMatrix[1][p_deltaBuffer]       =   g_lfoBpmMatrix[f_index2][p_timeBuffer] - g_lfoBpmMatrix[f_index1][p_timeBuffer];
                    g_lfoBpmMatrix[2][p_deltaBuffer]       =   g_lfoBpmMatrix[f_index3][p_timeBuffer] - g_lfoBpmMatrix[f_index2][p_timeBuffer];

                    if( g_lfoBpmMatrix[1][p_deltaBuffer] *BPM_DEN < g_lfoBpmMatrix[0][p_deltaBuffer] * BPM_NUM &&  // 1.25 - 4/5, 1.2 - 5/6, 1.125 - 8/9
                        g_lfoBpmMatrix[2][p_deltaBuffer] *BPM_DEN < g_lfoBpmMatrix[0][p_deltaBuffer] * BPM_NUM &&
                        g_lfoBpmMatrix[0][p_deltaBuffer] *BPM_DEN < g_lfoBpmMatrix[2][p_deltaBuffer] * BPM_NUM )
                        {
                        f_intervalAverage                 = ( g_lfoBpmMatrix[0][p_deltaBuffer] + g_lfoBpmMatrix[1][p_deltaBuffer] + g_lfoBpmMatrix[2][p_deltaBuffer] ) / 3;

                        g_lfoBpmMatrix[p_source][BPM]     =   60000000 / f_intervalAverage;
                        g_lfoBpmMatrix[p_source][INTV]    =   f_intervalAverage;
                        g_lfoBpmMatrix[p_source][LBC]     =   m_Timer.GetClockTicks();
                        }
                    g_lfoBpmMatrix[p_source][LTIME]       =   p_triggerTimeClock;
                    g_lfoBpmMatrix[p_source][TIDX]        =   (g_lfoBpmMatrix[p_source][TIDX] + 1) % 4;
                    }
}

void            CKernel::predict1Beat               (   int             p_source, 
                                                        int             p_lfoMultIn )
{
                g_lfoBpmMatrix[p_source][NLMT]          =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]]; // v1: cache requested mult/div


                if (g_frameStart                       >=   g_lfoBpmMatrix[p_source][NBT])
                    {
                    g_lfoBpmMatrix[p_source][NBT]      +=   g_lfoBpmMatrix[p_source][INTV];
                    }
                if (g_frameStart                       >=   g_lfoBpmMatrix[p_source][NCB]) 
                    {
                    g_lfoBpmMatrix[p_source][LCB]       =   g_lfoBpmMatrix[p_source][NCB];
                    g_lfoBpmMatrix[p_source][NCB]       =   g_lfoBpmMatrix[p_source][NCB] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]); // v1: normal circle advance with current active mult/div
                    }
                if (g_lfoBpmMatrix[p_source][LBCT]     !=   g_lfoBpmMatrix[p_source][LBC])
                    {
                    g_lfoBpmMatrix[p_source][NBT]       =   g_lfoBpmMatrix[p_source][LBC];
                    g_lfoBpmMatrix[p_source][LBCT]      =   g_lfoBpmMatrix[p_source][LBC];
                    }
                if (g_lfoBpmMatrix[p_source][LMT]      !=   g_lfoBpmMatrix[p_source][NLMT]) // v1: any change applies immediately
                    {
                    g_lfoBpmMatrix[p_source][LMT]       =   g_lfoBpmMatrix[p_source][NLMT]; // v1: activate new mult/div now
                    g_lfoBpmMatrix[p_source][LCB]       =   g_lfoBpmMatrix[g_activeBpmChannel][LBC]; // v1: anchor to last valid bpm calculation
                    g_lfoBpmMatrix[p_source][NCB]       =   g_lfoBpmMatrix[g_activeBpmChannel][LBC] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]); // v1: rebuild circle immediately from bpm anchor
                    }
}
/*
void CKernel::sample1WaveTable(int p_source, int p_lfoIn, int p_lfoOut, int p_samples)   // p_samples is the max sample -1 NEW VERSION WITH ADDITIONAL SMOOTHING
{
    unsigned long currentTime = m_Timer.GetClockTicks();

    if (g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn] >= waveTableCount)
    {
        if (g_lfoBpmMatrix[p_source][LCB] != g_lfoBpmMatrix[p_source][LLCB])
            {
            g_lfoBpmMatrix[p_source][LLCB] = g_lfoBpmMatrix[p_source][LCB];
            g_inOutMatrixFlt[0][p_lfoOut] = g_inOutMatrixFlt[p_source][RND];
            g_inOutMatrixInt[0][p_lfoOut] = g_inOutMatrixInt[p_source][RND];
            }
        return;
    }
    g_lfoBpmMatrix[p_source][ELP]                   = currentTime - g_lfoBpmMatrix[p_source][LCB];

    g_lfoBpmMatrix[p_source][CYL]                   = g_lfoBpmMatrix[p_source][NCB] - g_lfoBpmMatrix[p_source][LCB];

    int f_index                                    = (g_lfoBpmMatrix[p_source][ELP] * p_samples) / g_lfoBpmMatrix[p_source][CYL];

    g_lfoBpmMatrix[p_source][SMPA]                 = f_index+0 > p_samples ? p_samples : f_index+0;
    g_lfoBpmMatrix[p_source][SMPB]                 = f_index+1 > p_samples ? p_samples : f_index+1;

    // ADDED: average two neighbouring samples
    int f_sample                                    = (g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMPA]] +
                                                       g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMPB]]) >> 1;

    g_inOutMatrixFlt[0][p_lfoOut]                   = f_sample / 1023.0f;

    g_inOutMatrixInt[0][p_lfoOut]                   = f_sample;
}
*/
void CKernel::sample1WaveTable(char** p_buffer, int p_source, int p_lfoIn, int p_lfoOut, int p_samples)   // p_samples is the max sample -1 NEW VERSION WITH ADDITIONAL SMOOTHING
{
    unsigned long currentTime = m_Timer.GetClockTicks();

    if (g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn] >= waveTableCount)
    {
        if (g_lfoBpmMatrix[p_source][LCB] != g_lfoBpmMatrix[p_source][LLCB])
            {
            g_lfoBpmMatrix[p_source][LLCB] = g_lfoBpmMatrix[p_source][LCB];
            g_inOutMatrixFlt[0][p_lfoOut] = g_inOutMatrixFlt[p_source][RND];
            g_inOutMatrixInt[0][p_lfoOut] = g_inOutMatrixInt[p_source][RND];
            }
        return;
    }

    g_lfoBpmMatrix[p_source][ELP]                   = currentTime - g_lfoBpmMatrix[p_source][LCB];

    g_lfoBpmMatrix[p_source][CYL]                   = g_lfoBpmMatrix[p_source][NCB] - g_lfoBpmMatrix[p_source][LCB];

    int f_index                                    = (g_lfoBpmMatrix[p_source][ELP] * p_samples) / g_lfoBpmMatrix[p_source][CYL];

    g_lfoBpmMatrix[p_source][SMPA]                 = f_index+0 > p_samples ? p_samples : f_index+0;
    g_lfoBpmMatrix[p_source][SMPB]                 = f_index+1 > p_samples ? p_samples : f_index+1;

    // ADDED: average two neighbouring samples
    int f_sample                                    = (((uint16_t*)p_buffer[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]])[g_lfoBpmMatrix[p_source][SMPA]] +
                                                       ((uint16_t*)p_buffer[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]])[g_lfoBpmMatrix[p_source][SMPB]]) >> 1;

    g_inOutMatrixFlt[0][p_lfoOut]                   = f_sample / 1023.0f;

    g_inOutMatrixInt[0][p_lfoOut]                   = f_sample;
}

//  sample1WaveTable(m_bufferLfo, p_source, p_lfoIn, p_lfoOut, 1023);
