#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex

/*    
void CKernel::generateWaveSinus(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last  = p_count - 1;
    const unsigned long long last2 = last * last;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x   = i;
        unsigned long long q   = x * (last - x);
        unsigned long long num = 16 * q;
        unsigned long long den = 5 * last2 - 4 * q;

        f_buffer[i] =
            (uint16_t)(((unsigned long long)LFO_AMPLITUDE * num * num) /
                       (den * den));
    }
}
*/
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
/*
void CKernel::generateWaveCenterPeak(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last  = p_count - 1;
    const unsigned long long last2 = last * last;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x = i;

        f_buffer[i] =
            (uint16_t)(((unsigned long long)LFO_AMPLITUDE *
                        4 * x * (last - x)) /
                       last2);
    }
}
*/
/*
void CKernel::generateWaveTriangle(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last = p_count - 1;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x = i;
        unsigned long long y;

        if ((x << 1) <= last)
            y = ((unsigned long long)LFO_AMPLITUDE * (x << 1)) / last;
        else
            y = ((unsigned long long)LFO_AMPLITUDE *
                 ((last - x) << 1)) / last;

        f_buffer[i] = (uint16_t)y;
    }
}
*/
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
/*
void CKernel::generateWaveRampUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last = p_count - 1;

    for (int i = 0; i < p_count; ++i)
    {
        f_buffer[i] =
            (uint16_t)(((unsigned long long)i * LFO_AMPLITUDE) / last);
    }
}
*/
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
/*
void CKernel::generateWaveRampDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last = p_count - 1;

    for (int i = 0; i < p_count; ++i)
    {
        f_buffer[i] =
            (uint16_t)(((last - i) *
                        (unsigned long long)LFO_AMPLITUDE) / last);
    }
}
*/
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
/*
void CKernel::generateWaveTrapezoid(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last = p_count - 1;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x = i;
        unsigned long long y;

        if ((x << 2) < last)
        {
            y = ((unsigned long long)LFO_AMPLITUDE *
                 (x << 2)) / last;
        }
        else if ((x << 2) <= (3 * last))
        {
            y = LFO_AMPLITUDE;
        }
        else
        {
            y = ((unsigned long long)LFO_AMPLITUDE *
                 ((last - x) << 2)) / last;
        }

        f_buffer[i] = (uint16_t)y;
    }
}
*/
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
/*
void CKernel::generateWaveSmoothUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last  = p_count - 1;
    const unsigned long long last3 = last * last * last;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x = i;

        unsigned long long y =
            ((unsigned long long)LFO_AMPLITUDE *
             x * x *
             ((3 * last) - (2 * x))) /
            last3;

        f_buffer[i] = (uint16_t)y;
    }
}
*/
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
/*
void CKernel::generateWaveSmoothDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last  = p_count - 1;
    const unsigned long long last3 = last * last * last;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x = i;

        unsigned long long y =
            ((unsigned long long)LFO_AMPLITUDE *
             x * x *
             ((3 * last) - (2 * x))) /
            last3;

        f_buffer[i] = (uint16_t)(LFO_AMPLITUDE - y);
    }
}
*/
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
/*
void CKernel::generateWaveSmoothPeak(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last = p_count - 1;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x;

        if (((unsigned long long)i << 1) <= last)
            x = ((unsigned long long)i << 1);
        else
            x = ((last - i) << 1);

        unsigned long long y =
            ((unsigned long long)LFO_AMPLITUDE *
             x * x *
             ((3 * last) - (2 * x))) /
            (last * last * last);

        f_buffer[i] = (uint16_t)y;
    }
}
*/
/*
void CKernel::generateWaveExponential(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const unsigned long long last = p_count - 1;

    for (int i = 0; i < p_count; ++i)
    {
        unsigned long long x;
        unsigned long long span;

        if (((unsigned long long)i << 1) <= last)
        {
            x    = (unsigned long long)i << 1;
            span = last;

            unsigned long long x2 = x * x;
            unsigned long long x4 = x2 * x2;

            unsigned long long span2 = span * span;
            unsigned long long span4 = span2 * span2;

            f_buffer[i] =
                (uint16_t)(((unsigned long long)LFO_AMPLITUDE *
                            x4 * x) /
                           (span4 * span));
        }
        else
        {
            x    = ((unsigned long long)i << 1) - last;
            span = last;

            unsigned long long x2 = x * x;
            unsigned long long x4 = x2 * x2;

            unsigned long long span2 = span * span;
            unsigned long long span4 = span2 * span2;

            unsigned long long decay =
                ((unsigned long long)LFO_AMPLITUDE *
                 x4 * x) /
                (span4 * span);

            f_buffer[i] =
                (uint16_t)(LFO_AMPLITUDE - decay);
        }
    }
}
*/
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