```cpp
enum ModeGroup
{
    GROUP_BASE = 0,
    GROUP_FLAG1,
    GROUP_FLAG2,
    GROUP_FLAG3,
    GROUP_COUNT
};

static const uint8_t g_groupLength[GROUP_COUNT] =
{
    6,  // base:   0 1 2 3 4 5
    2,  // flag1:  8 9
    4,  // flag2: 16 17 18 19
    1   // flag3: 32
};

static const uint8_t g_groupModes[GROUP_COUNT][4] =
{
    {  0,  1,  2,  3 },   // base, continued below because max row size fixed here
    {  8,  9,  0,  0 },
    { 16, 17, 18, 19 },
    { 32,  0,  0,  0 }
};
```

For base with full 6 entries, use a large enough second dimension:

```cpp
enum ModeGroup
{
    GROUP_BASE = 0,
    GROUP_FLAG1,
    GROUP_FLAG2,
    GROUP_FLAG3,
    GROUP_COUNT
};

static const uint8_t g_groupLength[GROUP_COUNT] =
{
    6,
    2,
    4,
    1
};

static const uint8_t g_groupModes[GROUP_COUNT][6] =
{
    {  0,  1,  2,  3,  4,  5 },
    {  8,  9,  0,  0,  0,  0 },
    { 16, 17, 18, 19,  0,  0 },
    { 32,  0,  0,  0,  0,  0 }
};
```
void CKernel::set_mode_roof(uint8_t base)
{
    const uint8_t f_first_flag = FLAG_TIME;

    for (uint8_t i = 0; i < 4; ++i)
    {
        const uint8_t row = base + i;
        uint8_t roof = g_groupLength[GROUP_BASE];

        for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
        {
            const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

            if (g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
            {
                roof += g_groupLength[group];
            }
        }

        g_modeRoof[row] = roof;
    }
}

or 

static const uint8_t g_groupLength[BLOCK_COUNT][GROUP_COUNT] =
{
    { 6, 2, 0, 1 },
    { 6, 0, 4, 1 },
    { 5, 2, 2, 0 },
    { 4, 0, 0, 3 }
};

void CKernel::set_mode_roof(uint8_t base)
{
    const uint8_t f_first_flag = FLAG_TIME;
    const uint8_t block = base / 4;

    for (uint8_t i = 0; i < 4; ++i)
    {
        const uint8_t row = base + i;
        uint8_t roof = g_groupLength[block][GROUP_BASE];

        for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
        {
            const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

            if (g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
            {
                roof += g_groupLength[block][group];
            }
        }

        g_modeRoof[row] = roof;
    }
}

or

void CKernel::set_mode_roof(uint8_t base)
{
    const uint8_t f_first_flag = FLAG_TIME;
    const uint8_t block = base / 4;

    uint8_t roof = g_groupLength[block][GROUP_BASE];

    for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
    {
        const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

        if (g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
        {
            roof += g_groupLength[block][group];
        }
    }

    g_modeRoof[base + 0] = roof;
    g_modeRoof[base + 1] = roof;
    g_modeRoof[base + 2] = roof;
    g_modeRoof[base + 3] = roof;
}

or *************************************************************************

static const uint8_t g_groupLength[BLOCK_COUNT][GROUP_COUNT] =
{
    { 6, 2, 0, 1 },
    { 6, 0, 4, 1 },
    { 5, 2, 2, 0 },
    { 4, 0, 0, 3 }
};

void CKernel::set_mode_roof(uint8_t base)
{
    const uint8_t f_first_flag = FLAG_TIME;
    const uint8_t block = base / 4;

    uint8_t roof = g_groupLength[block][GROUP_BASE];

    for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
    {
        const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

        if (g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
        {
            roof += g_groupLength[block][group];
        }
    }

    g_modeRoof[block] = roof;
}

or *************************************************************************

void CKernel::set_mode_roof(uint8_t base)
{
    uint8_t row;
    uint8_t roof;

    row = base + 0;
    roof = g_groupLength[GROUP_BASE];
    if (FLAG_AUDIO_A) roof += g_groupLength[GROUP_FLAG1];
    if (m_flag_2_A)     roof += g_groupLength[GROUP_FLAG2];
    if (m_flag_3_A)     roof += g_groupLength[GROUP_FLAG3];
    g_modeRoof[row] = roof;

    row = base + 1;
    roof = g_groupLength[GROUP_BASE];
    if (FLAG_AUDIO_B) roof += g_groupLength[GROUP_FLAG1];
    if (m_flag_2_B)     roof += g_groupLength[GROUP_FLAG2];
    if (m_flag_3_B)     roof += g_groupLength[GROUP_FLAG3];
    g_modeRoof[row] = roof;

    row = base + 2;
    roof = g_groupLength[GROUP_BASE];
    if (FLAG_AUDIO_A) roof += g_groupLength[GROUP_FLAG1];
    if (m_flag_2_A)     roof += g_groupLength[GROUP_FLAG2];
    if (m_flag_3_A)     roof += g_groupLength[GROUP_FLAG3];
    g_modeRoof[row] = roof;

    row = base + 3;
    roof = g_groupLength[GROUP_BASE];
    if (FLAG_AUDIO_B) roof += g_groupLength[GROUP_FLAG1];
    if (m_flag_2_B)     roof += g_groupLength[GROUP_FLAG2];
    if (m_flag_3_B)     roof += g_groupLength[GROUP_FLAG3];
    g_modeRoof[row] = roof;
}

void CKernel::mapMenuGroup_select_map(uint8_t menu_id, uint8_t base)
{
    if (g_menu_mode_new != menu_id)
    {
        return;
    }

    unsigned v;

    v = (g_inOutMatrixInt[4][RAW] * g_modeRoof[base + 0]) >> 10;
    if (v >= g_modeRoof[base + 0]) v = g_modeRoof[base + 0] - 1;

    if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
    {
        g_menuPickUpFlag[base + 0] = true;
    }
    else if (g_menuPickUpFlag[base + 0])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
    }

    v = (g_inOutMatrixInt[5][RAW] * g_modeRoof[base + 1]) >> 10;
    if (v >= g_modeRoof[base + 1]) v = g_modeRoof[base + 1] - 1;

    if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
    {
        g_menuPickUpFlag[base + 1] = true;
    }
    else if (g_menuPickUpFlag[base + 1])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
    }

    v = (g_inOutMatrixInt[6][RAW] * g_modeRoof[base + 2]) >> 10;
    if (v >= g_modeRoof[base + 2]) v = g_modeRoof[base + 2] - 1;

    if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
    {
        g_menuPickUpFlag[base + 2] = true;
    }
    else if (g_menuPickUpFlag[base + 2])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
    }

    v = (g_inOutMatrixInt[7][RAW] * g_modeRoof[base + 3]) >> 10;
    if (v >= g_modeRoof[base + 3]) v = g_modeRoof[base + 3] - 1;

    if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
    {
        g_menuPickUpFlag[base + 3] = true;
    }
    else if (g_menuPickUpFlag[base + 3])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
    }
}
// correct one

void            CKernel::mapMenuGroup               (   uint8_t menu_id, uint8_t base)
{
                if (g_menu_mode_new != menu_id)
                    {
                    return;
                    }
                unsigned v;

                v = (g_inOutMatrixInt[4][RAW] * g_modeRoof[base]) >> 10;

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
                    {
                    g_menuPickUpFlag[base + 0] = true;
                    }
                else if (g_menuPickUpFlag[base + 0])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
                    }
                v = (g_inOutMatrixInt[5][RAW] * g_modeRoof[base]) >> 10;

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
                    {
                    g_menuPickUpFlag[base + 1] = true;
                    }
                else if (g_menuPickUpFlag[base + 1])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
                    }
                v = (g_inOutMatrixInt[6][RAW] * g_modeRoof[base]) >> 10;

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
                    {
                    g_menuPickUpFlag[base + 2] = true;
                    }
                else if (g_menuPickUpFlag[base + 2])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
                    }
                v = (g_inOutMatrixInt[7][RAW] * g_modeRoof[base]) >> 10;

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
                    {
                    g_menuPickUpFlag[base + 3] = true;
                    }
                else if (g_menuPickUpFlag[base + 3])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
                    }
}


If you keep `g_modeMap` as the fixed/master table and use `g_modeRoof` for the compact selector range, then the corrected form is:

```cpp
void CKernel::getChannelModeB(int p_channel)
{
    const uint8_t sel =
        g_centralModeBuffer[g_currentProgramBuffer][p_channel];

    const uint8_t mode =
        g_modeSelectMap[p_channel][sel];

    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(p_channel);
    }
}
```

And for the switch version:

```cpp
void CKernel::getChannelModeA(int p_channel)
{
    const uint8_t sel =
        g_centralModeBuffer[g_currentProgramBuffer][p_channel];

    switch (g_modeSelectMap[p_channel][sel])
    {
        case 0:
            modeADC(p_channel);
        break;

        case 1:
            modeTRG(p_channel);
        break;

        case 2:
            modeBPM(p_channel);
        break;

        case 3:
            modeLF1(p_channel);
        break;

        case 4:
            modeLF2(p_channel);
        break;

        case 5:
        /*  modeTex(p_channel); */
        break;

        case 6:
        /*  modeVid(p_channel); */
        break;

        case 7:
        /*  modeFrm(p_channel); */
        break;

        case 8:
            modeAudioAb0(p_channel);
        break;

        case 9:
            modeAudioAb1(p_channel);
        break;

        case 10:
            modeAudioBb0(p_channel);
        break;

        case 11:
            modeAudioBb1(p_channel);
        break;
    }
}
```

This is correct only if `g_modeSelectMap[p_channel][sel]` stores the actual mode values in compact selector order.
