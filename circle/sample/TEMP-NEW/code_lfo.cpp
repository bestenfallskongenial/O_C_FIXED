#include "kernel.h" 
/*
void CKernel::generateWaveSinus(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float s = sinf(3.14159265358979323846f * t);

        f_buffer[i] =
            (uint16_t)(s * s * (float)LFO_AMPLITUDE + 0.5f);
    }
}


                void CKernel::generateWaveCenterPeak(char** p_buffer, int p_index, int p_count)
                {
                    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
                    const float last = (float)(p_count - 1);

                    for (int i = 0; i < p_count; ++i)
                    {
                        float x = (2.0f * (float)i / last) - 1.0f;
                        float y = 1.0f - x * x;

                        f_buffer[i] =
                            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
                    }
                }


void CKernel::generateWaveTriangle(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = 1.0f - fabsf(2.0f * t - 1.0f);

        f_buffer[i] =
            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
    }
}

void CKernel::generateWaveRampUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;

        f_buffer[i] =
            (uint16_t)(t * (float)LFO_AMPLITUDE + 0.5f);
    }
}

void CKernel::generateWaveRampDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = 1.0f - t;

        f_buffer[i] =
            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
    }
}

void CKernel::generateWaveTrapezoid(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y;

        if (t < 0.25f)
            y = t * 4.0f;
        else if (t <= 0.75f)
            y = 1.0f;
        else
            y = (1.0f - t) * 4.0f;

        f_buffer[i] =
            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
    }
}

void CKernel::generateWaveSmoothUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = t * t * (3.0f - 2.0f * t);

        f_buffer[i] =
            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
    }
}

void CKernel::generateWaveSmoothDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = 1.0f - t * t * (3.0f - 2.0f * t);

        f_buffer[i] =
            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
    }
}


                void CKernel::generateWaveSmoothPeak(char** p_buffer, int p_index, int p_count)
                {
                    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
                    const float last = (float)(p_count - 1);
                    const float half = last * 0.5f;

                    for (int i = 0; i < p_count; ++i)
                    {
                        float t;

                        if ((float)i <= half)
                            t = (float)i / half;
                        else
                            t = (last - (float)i) / half;

                        float y = t * t * (3.0f - 2.0f * t);

                        f_buffer[i] =
                            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
                    }
                }


void CKernel::generateWaveExpRiseSlowFastDecay(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const float k    = 5.0f;
    const float last = (float)(p_count - 1);
    const float half = last * 0.5f;
    const float norm = expf(k) - 1.0f;

    for (int i = 0; i < p_count; ++i)
    {
        float y;

        if ((float)i <= half)
        {
            float t = (float)i / half;

            y = (expf(k * t) - 1.0f) / norm;
        }
        else
        {
            float t = ((float)i - half) / half;

            y = 1.0f -
                (expf(k * t) - 1.0f) / norm;
        }

        f_buffer[i] =
            (uint16_t)(y * (float)LFO_AMPLITUDE + 0.5f);
    }
}

*/

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


void CKernel::generateWaveExpRiseSlowFastDecay(char** p_buffer, int p_index, int p_count)
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