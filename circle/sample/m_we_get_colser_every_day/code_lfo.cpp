#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX   m_logKernelIndex 

void CKernel::generateWaveSinus(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        uint32_t t = (i * (uint32_t)LFO_AMPLITUDE) / last;

        uint32_t q =
            (t * ((uint32_t)LFO_AMPLITUDE - t)) /
            (uint32_t)LFO_AMPLITUDE;

        uint32_t num = 16u * q;

        uint32_t den =
            5u * (uint32_t)LFO_AMPLITUDE -
            4u * q;

        uint32_t s =
            (num * (uint32_t)LFO_AMPLITUDE) / den;

        f_buffer[i] = (uint16_t)
            ((s * s) / (uint32_t)LFO_AMPLITUDE);
    }
}

void CKernel::generateWaveTriangle(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        uint32_t t =
            (i * (uint32_t)LFO_AMPLITUDE) / last;

        if ((t << 1) <= (uint32_t)LFO_AMPLITUDE)
        {
            f_buffer[i] = (uint16_t)(t << 1);
        }
        else
        {
            f_buffer[i] = (uint16_t)
                (((uint32_t)LFO_AMPLITUDE - t) << 1);
        }
    }
}

void CKernel::generateWaveRampUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        f_buffer[i] = (uint16_t)
            ((i * (uint32_t)LFO_AMPLITUDE) / last);
    }
}

void CKernel::generateWaveRampDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        f_buffer[i] = (uint16_t)
            (((last - i) * (uint32_t)LFO_AMPLITUDE) / last);
    }
}

void CKernel::generateWaveTrapezoid(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        uint32_t t =
            (i * (uint32_t)LFO_AMPLITUDE) / last;

        if ((t << 2) < (uint32_t)LFO_AMPLITUDE)
        {
            f_buffer[i] = (uint16_t)(t << 2);
        }
        else if ((t << 2) <=
                 3u * (uint32_t)LFO_AMPLITUDE)
        {
            f_buffer[i] = (uint16_t)LFO_AMPLITUDE;
        }
        else
        {
            f_buffer[i] = (uint16_t)
                (((uint32_t)LFO_AMPLITUDE - t) << 2);
        }
    }
}

void CKernel::generateWaveSmoothUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        uint32_t t =
            (i * (uint32_t)LFO_AMPLITUDE) / last;

        uint32_t t2 =
            (t * t) / (uint32_t)LFO_AMPLITUDE;

        f_buffer[i] = (uint16_t)
            ((t2 *
              (3u * (uint32_t)LFO_AMPLITUDE - 2u * t)) /
             (uint32_t)LFO_AMPLITUDE);
    }
}

void CKernel::generateWaveSmoothDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        uint32_t t =
            (i * (uint32_t)LFO_AMPLITUDE) / last;

        uint32_t t2 =
            (t * t) / (uint32_t)LFO_AMPLITUDE;

        uint32_t y =
            (t2 *
             (3u * (uint32_t)LFO_AMPLITUDE - 2u * t)) /
            (uint32_t)LFO_AMPLITUDE;

        f_buffer[i] = (uint16_t)
            ((uint32_t)LFO_AMPLITUDE - y);
    }
}

void CKernel::generateWaveExponential(  char** p_buffer,
                                        int p_index,
                                        int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const uint32_t last = (uint32_t)p_count - 1u;

    for (uint32_t i = 0; i < (uint32_t)p_count; ++i)
    {
        uint32_t t =
            (i * (uint32_t)LFO_AMPLITUDE) / last;

        uint32_t x;

        if ((t << 1) <= (uint32_t)LFO_AMPLITUDE)
        {
            x = t << 1;
        }
        else
        {
            x =
                (t << 1) -
                (uint32_t)LFO_AMPLITUDE;
        }

        uint32_t y = x;

        y = (y * x) / (uint32_t)LFO_AMPLITUDE;
        y = (y * x) / (uint32_t)LFO_AMPLITUDE;
        y = (y * x) / (uint32_t)LFO_AMPLITUDE;
        y = (y * x) / (uint32_t)LFO_AMPLITUDE;

        if ((t << 1) <= (uint32_t)LFO_AMPLITUDE)
        {
            f_buffer[i] = (uint16_t)y;
        }
        else
        {
            f_buffer[i] = (uint16_t)
                ((uint32_t)LFO_AMPLITUDE - y);
        }
    }
}