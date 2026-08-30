#define BLOCK_COUNT 4



enum ModeFlags          // for now just dummies
{
    GROUP_BASE = 0,
    GROUP_FLAG1,
    GROUP_FLAG2,
    GROUP_FLAG3,
    GROUP_COUNT
};

static const uint8_t g_groupLength[BLOCK_COUNT][GROUP_COUNT] =
{
    { 6, 2, 0, 1 },  //  means GROUP_BASE has 6 entries / modes, FLAG1 has 2, FLAG2 has 0... four the 0th block ( of four )!!
    { 6, 2, 0, 1 },  //  same because design decision   
    { 5, 2, 2, 0 },  // the 2nd block has now **** look below!
    { 4, 0, 0, 3 }
};

**** the array should contain [BLOCK_COUNT][GROUP_COUNT*BLOCK_COUNT] right?

{ 6, 2, 0, 1 }


| Block | Flags         | `set_mode_roof()` adds           | `g_modeRoof[block]` | `mapMenuGroup()` writes into `g_centralModeBuffer[g_currentProgramBuffer][base + x]` | Example RAW `[4,5,6,7] = [0,256,512,768]` → exact written content `[base+0..3]` | Content values of `v` across full RAW span | Resolved modes read from `g_groupModes` |
| ----: | ------------- | -------------------------------- | ------------------: | ------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------- | ------------------------------------------ | --------------------------------------- |
|     0 | none          | BASE `6`                         |                   6 | `v = (RAW * 6) >> 10`                                                                | `[0, 1, 3, 4]`                                                                  | `0, 1, 2, 3, 4, 5`                         | `0, 1, 2, 3, 4, 5`                      |
|     0 | flag1         | BASE `6` + FLAG1 `2`             |                   8 | `v = (RAW * 8) >> 10`                                                                | `[0, 2, 4, 6]`                                                                  | `0, 1, 2, 3, 4, 5, 6, 7`                   | `0, 1, 2, 3, 4, 5, 8, 9`                |
|     0 | flag3         | BASE `6` + FLAG3 `1`             |                   7 | `v = (RAW * 7) >> 10`                                                                | `[0, 1, 3, 5]`                                                                  | `0, 1, 2, 3, 4, 5, 6`                      | `0, 1, 2, 3, 4, 5, 32`                  |
|     0 | flag1 + flag3 | BASE `6` + FLAG1 `2` + FLAG3 `1` |                   9 | `v = (RAW * 9) >> 10`                                                                | `[0, 2, 4, 6]`                                                                  | `0, 1, 2, 3, 4, 5, 6, 7, 8`                | `0, 1, 2, 3, 4, 5, 8, 9, 32`            |
|     1 | none          | BASE `6`                         |                   6 | `v = (RAW * 6) >> 10`                                                                | `[0, 1, 3, 4]`                                                                  | `0, 1, 2, 3, 4, 5`                         | `0, 1, 2, 3, 4, 5`                      |
|     1 | flag1         | BASE `6` + FLAG1 `0`             |                   6 | `v = (RAW * 6) >> 10`                                                                | `[0, 1, 3, 4]`                                                                  | `0, 1, 2, 3, 4, 5`                         | `0, 1, 2, 3, 4, 5`                      |
|     1 | flag3         | BASE `6` + FLAG3 `1`             |                   7 | `v = (RAW * 7) >> 10`                                                                | `[0, 1, 3, 5]`                                                                  | `0, 1, 2, 3, 4, 5, 6`                      | `0, 1, 2, 3, 4, 5, 32`                  |
|     1 | flag1 + flag3 | BASE `6` + FLAG1 `0` + FLAG3 `1` |                   7 | `v = (RAW * 7) >> 10`                                                                | `[0, 1, 3, 5]`                                                                  | `0, 1, 2, 3, 4, 5, 6`                      | `0, 1, 2, 3, 4, 5, 32`                  |
