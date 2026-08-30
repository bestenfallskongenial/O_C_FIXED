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

static const uint8_t g_groupModes[GROUP_COUNT][6] =
{
    {  0,  1,  2,  3,  4,  5 },
    {  8,  9,  0,  0,  0,  0 },
    { 16, 17, 18, 19,  0,  0 },
    { 32,  0,  0,  0,  0,  0 }
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

tell me the content for g_centralModeBuffer[g_currentProgramBuffer][base + x]
as example
for block 0 and 1 with 
a) no flags
b) flag1
c) flag3 
d) flad1+falg3

as table! 

void CKernel::set_mode_roof(uint8_t base)
{
    const uint8_t f_first_flag = FLAG_TIME;

    uint8_t roof = 0;
    uint8_t dst  = 0;

    for (uint8_t i = 0; i < g_groupLength[base][GROUP_BASE]; ++i)
    {
        g_modeMap[base][dst++] = g_groupModes[GROUP_BASE][i];
        ++roof;
    }

    for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
    {
        const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

        if (g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
        {
            for (uint8_t i = 0; i < g_groupLength[base][group]; ++i)
            {
                g_modeMap[base][dst++] = g_groupModes[group][i];
                ++roof;
            }
        }
    }

    g_modeRoof[base] = roof;
}

Assuming:

```cpp
GROUP_BASE  = { 0, 1, 2, 3, 4 }
GROUP_FLAG1 = { 8, 9 }
GROUP_FLAG2 = { 12, 13, 14, 15 }
GROUP_FLAG3 = { 10 }
```

and:

```cpp
g_groupLength =
{
    { 5, 2, 4, 1 },   // block 0
    { 5, 2, 4, 1 },   // block 1
    { 5, 2, 0, 0 },   // block 2
    { 5, 0, 0, 0 }    // block 3
};
```

| Block | Active flags          | Groups copied into `g_modeMap[block]`       | `g_modeRoof[block]` | Resulting `g_modeMap[block][...]`         |
| ----: | --------------------- | ------------------------------------------- | ------------------: | ----------------------------------------- |
|     0 | none                  | BASE                                        |                   5 | `0, 1, 2, 3, 4`                           |
|     0 | FLAG1                 | BASE + FLAG1                                |                   7 | `0, 1, 2, 3, 4, 8, 9`                     |
|     0 | FLAG2                 | BASE + FLAG2                                |                   9 | `0, 1, 2, 3, 4, 12, 13, 14, 15`           |
|     0 | FLAG3                 | BASE + FLAG3                                |                   6 | `0, 1, 2, 3, 4, 10`                       |
|     0 | FLAG1 + FLAG3         | BASE + FLAG1 + FLAG3                        |                   8 | `0, 1, 2, 3, 4, 8, 9, 10`                 |
|     0 | FLAG1 + FLAG2 + FLAG3 | BASE + FLAG1 + FLAG2 + FLAG3                |                  12 | `0, 1, 2, 3, 4, 8, 9, 12, 13, 14, 15, 10` |
|     2 | none                  | BASE                                        |                   5 | `0, 1, 2, 3, 4`                           |
|     2 | FLAG1                 | BASE + FLAG1                                |                   7 | `0, 1, 2, 3, 4, 8, 9`                     |
|     2 | FLAG2                 | BASE only, because length is `0`            |                   5 | `0, 1, 2, 3, 4`                           |
|     2 | FLAG3                 | BASE only, because length is `0`            |                   5 | `0, 1, 2, 3, 4`                           |
|     2 | FLAG1 + FLAG2 + FLAG3 | BASE + FLAG1 only                           |                   7 | `0, 1, 2, 3, 4, 8, 9`                     |
|     3 | none                  | BASE                                        |                   5 | `0, 1, 2, 3, 4`                           |
|     3 | FLAG1 + FLAG2 + FLAG3 | BASE only, because all flag lengths are `0` |                   5 | `0, 1, 2, 3, 4`                           |
