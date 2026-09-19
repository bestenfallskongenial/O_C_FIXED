1. defs_enum.h — replace two audio flags with one
Old
FLAG_AUDIO_A,
FLAG_AUDIO_B,
FLAG_MIDI,
FLAG_DUMMY_B,
New for Model 2
FLAG_AUDIO,
FLAG_MIDI,
FLAG_DUMMY_A,
FLAG_DUMMY_B,
New for Model 3
FLAG_AUDIO,
FLAG_MIDI,
FLAG_DUMMY_A,
FLAG_DUMMY_B,
2. defs_enum.h — replace two audio groups with one
Old
enum ModeFlags
{
    GROUP_BASE,
    GROUP_FLAG1,
    GROUP_FLAG2,
    GROUP_FLAG3,

    GROUP_COUNT
};
New for Model 2
enum ModeFlags
{
    GROUP_BASE,
    GROUP_AUDIO,
    GROUP_MIDI,

    GROUP_COUNT
};
New for Model 3
enum ModeFlags
{
    GROUP_BASE,
    GROUP_AUDIO,
    GROUP_MIDI,

    GROUP_COUNT
};
3. defs_member.h — one detected audio source
Old
int is_audio[2] = { 0 };
New for Model 2
int is_audio = 0;
New for Model 3
int is_audio = 0;
FLAG_AUDIO supplies the no-audio state; is_audio is used only while that flag is true.

4. defs_member.h — one audio hold counter
Old
uint32_t m_audio_hold_A = 0;
uint32_t m_audio_hold_B = 0;
New for Model 2
uint32_t m_audio_hold = 0;
New for Model 3
uint32_t m_audio_hold = 0;
5. defs_member.h — combine the audio groups
Old
const int g_groupLen[GROUP_COUNT] =
{
    5,
    2,
    2,
    3
};

const int g_groupModes[GROUP_COUNT][5] =
{
    { 0, 1, 2, 3, 4 },
    { 5, 6, 0, 0, 0 },
    { 7, 8, 0, 0, 0 },
    { 9, 10, 11, 0, 0 }
};
New for Model 2
const int g_groupLen[GROUP_COUNT] =
{
    5,
    2,
    3
};

const int g_groupModes[GROUP_COUNT][5] =
{
    { 0, 1, 2, 3, 4 },
    { 5, 6, 0, 0, 0 },
    { 9, 10, 11, 0, 0 }
};
New for Model 3
const int g_groupLen[GROUP_COUNT] =
{
    5,
    4,
    3
};

const int g_groupModes[GROUP_COUNT][5] =
{
    { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 0 },
    { 9, 10, 11, 0, 0 }
};
6. code_hardware.cpp — change adc_ProcessAudio()
Old
ADC 0/2 → is_audio[0], hold A, flag A, AU0/AU1
ADC 1/3 → is_audio[1], hold B, flag B, AU2/AU3
New for Model 2
ADC 0/1/2/3 → is_audio, m_audio_hold, FLAG_AUDIO
detected sample → AU0/AU1
Each existing explicit ADC branch retains the current averaging code but uses:

is_audio = channel;
m_audio_hold = AUDIO_HOLD_TIMEOUT;
g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO] = 1;
The final timeout becomes:

if (m_audio_hold > 0) --m_audio_hold;

g_centralModeBuffer
    [g_currentProgramBuffer]
    [FLAG_AUDIO] = (m_audio_hold > 0);
New for Model 3
ADC 0/1/2/3 → is_audio, m_audio_hold, FLAG_AUDIO
detected sample → AU0/AU1/AU2/AU3
Each explicit ADC branch performs the four existing averaging calculations using the same sample.

The final timeout is identical to Model 2.

7. code_menu.cpp — update the group/flag names in set_mode_roof_map()
Old
const uint8_t f_first_flag = FLAG_AUDIO_A;
for (uint8_t group = GROUP_FLAG1;
     group < GROUP_COUNT;
     ++group)
    {
    const uint8_t flag_pos =
        f_first_flag + (group - GROUP_FLAG1);
New for Model 2
const uint8_t f_first_flag = FLAG_AUDIO;
for (uint8_t group = GROUP_AUDIO;
     group < GROUP_COUNT;
     ++group)
    {
    const uint8_t flag_pos =
        f_first_flag + (group - GROUP_AUDIO);
New for Model 3
Exactly the same as Model 2.

8. code_menu.cpp — add enforcement/reset to applyTargetModes()
Old
No audio validation.

New for Model 2
At the beginning of applyTargetModes():

if FLAG_AUDIO is true:

resolve the mode of is_audio;

allow IN_MODE_AU_AL through IN_MODE_AU_AH;

otherwise force the first audio mode and clear its pickup;

if FLAG_AUDIO is false:

reset channels using either audio mode to ADC;

clear the pickup flags of those channels.

New for Model 3
The same mechanism, except the accepted/reset range is:

IN_MODE_AU_AL
through:

IN_MODE_AU_BH
9. code_debug.cpp — one audio log
Old
storeLogHex(..., "AUDIO 0", ...[FLAG_AUDIO_A]);
storeLogHex(..., "AUDIO 1", ...[FLAG_AUDIO_B]);
New for Model 2
storeLogHex(..., "AUDIO", ...[FLAG_AUDIO]);
New for Model 3
storeLogHex(..., "AUDIO", ...[FLAG_AUDIO]);
Actual required files
Only these files require changes:

defs_enum.h
defs_member.h
code_hardware.cpp
code_menu.cpp
code_debug.cpp
The difference between Models 2 and 3 is only:

audio group length: 2 versus 4
audio group contents: modes 5–6 versus modes 5–8
averages calculated: AU0/AU1 versus AU0–AU3
accepted audio-mode range in applyTargetModes()