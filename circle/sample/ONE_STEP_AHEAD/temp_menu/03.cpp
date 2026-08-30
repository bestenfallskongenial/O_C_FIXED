enum ModeGroup
{
    GROUP_BASE = 0,
    GROUP_FLAG1,
    GROUP_FLAG2,
    GROUP_FLAG3,
    GROUP_COUNT
};

static const uint8_t g_groupLength[BLOCK_COUNT][GROUP_COUNT] =
{
    { 6, 2, 0, 1 },
    { 6, 0, 4, 1 },
    { 5, 2, 2, 0 },
    { 4, 0, 0, 3 }
};

uint8_t g_modeRoof[BLOCK_COUNT];

typedef void (CKernel::*ModeFunc)(int);

ModeFunc g_modeTable[12] =
{
    &CKernel::modeADC,
    &CKernel::modeTRG,
    &CKernel::modeBPM,
    &CKernel::modeLF1,
    &CKernel::modeLF2,
    nullptr,
    nullptr,
    nullptr,
    &CKernel::modeAudioAb0,
    &CKernel::modeAudioAb1,
    &CKernel::modeAudioBb0,
    &CKernel::modeAudioBb1
};

uint8_t g_modeMap[LAYER * 4][MAX_MODES];

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

void CKernel::set_mode_map(uint8_t base)
{
    const uint8_t f_first_flag = FLAG_TIME;
    const uint8_t block = base / 4;

    uint8_t dst = 0;

    for (uint8_t i = 0; i < g_groupLength[block][GROUP_BASE]; ++i)
    {
        g_modeMap[base + 0][dst] = g_groupModes[block][GROUP_BASE][i];
        g_modeMap[base + 1][dst] = g_groupModes[block][GROUP_BASE][i];
        g_modeMap[base + 2][dst] = g_groupModes[block][GROUP_BASE][i];
        g_modeMap[base + 3][dst] = g_groupModes[block][GROUP_BASE][i];
        ++dst;
    }

    for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
    {
        const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

        if (!g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
        {
            continue;
        }

        for (uint8_t i = 0; i < g_groupLength[block][group]; ++i)
        {
            g_modeMap[base + 0][dst] = g_groupModes[block][group][i];
            g_modeMap[base + 1][dst] = g_groupModes[block][group][i];
            g_modeMap[base + 2][dst] = g_groupModes[block][group][i];
            g_modeMap[base + 3][dst] = g_groupModes[block][group][i];
            ++dst;
        }
    }
}

void CKernel::mapMenuGroup_select_map(uint8_t menu_id, uint8_t base)
{
    if (g_menu_mode_new != menu_id)
    {
        return;
    }

    const uint8_t block = base / 4;
    const uint8_t roof  = g_modeRoof[block];

    unsigned v;

    v = (g_inOutMatrixInt[4][RAW] * roof) >> 10;
    if (v >= roof) v = roof - 1;

    if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
    {
        g_menuPickUpFlag[base + 0] = true;
    }
    else if (g_menuPickUpFlag[base + 0])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
    }

    v = (g_inOutMatrixInt[5][RAW] * roof) >> 10;
    if (v >= roof) v = roof - 1;

    if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
    {
        g_menuPickUpFlag[base + 1] = true;
    }
    else if (g_menuPickUpFlag[base + 1])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
    }

    v = (g_inOutMatrixInt[6][RAW] * roof) >> 10;
    if (v >= roof) v = roof - 1;

    if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
    {
        g_menuPickUpFlag[base + 2] = true;
    }
    else if (g_menuPickUpFlag[base + 2])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
    }

    v = (g_inOutMatrixInt[7][RAW] * roof) >> 10;
    if (v >= roof) v = roof - 1;

    if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
    {
        g_menuPickUpFlag[base + 3] = true;
    }
    else if (g_menuPickUpFlag[base + 3])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
    }
}

void CKernel::getChannelModeA(int p_channel)
{
    switch (g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]])
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

void CKernel::getChannelModeB(int p_channel)
{
    int mode =
        g_modeMap[p_channel]
                 [g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(p_channel);
    }
}

void CKernel::getChannelModeB(int p_channel)
{
    int mode =
        g_modeMap[p_channel]
                 [g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(p_channel);
    }
}


void CKernel::getChannelModeB(int p_channel, int group)
{
    int mode =
        g_groupModes[group]
                    [g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(p_channel);
    }
}

void CKernel::getChannelModeB(int p_channel)
{
    const uint8_t f_first_flag = FLAG_TIME;
    const uint8_t block        = p_channel / 4;

    uint8_t index =
        g_centralModeBuffer[g_currentProgramBuffer][p_channel];

    for (uint8_t group = GROUP_BASE; group < GROUP_COUNT; ++group)
    {
        uint8_t active = 0;

        if (group == GROUP_BASE)
        {
            active = 1;
        }
        else
        {
            const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

            active =
                g_centralModeBuffer[g_currentProgramBuffer][flag_pos];
        }

        if (!active)
        {
            continue;
        }

        const uint8_t len = g_groupLength[block][group];

        if (index < len)
        {
            const uint8_t mode = g_groupModes[group][index];

            ModeFunc fn = g_modeTable[mode];

            if (fn)
            {
                (this->*fn)(p_channel);
            }

            return;
        }

        index -= len;
    }
}