#include "kernel.h"

    #define MY_BFR  m_bufferLog[8]                 // means the log goes into the pre-init buffer 
    #define MY_IDX  m_bufferLogIndex[8]

const char*     CKernel::bufferSourceModeName       (   u32         p_value)
{
                switch (p_value)
                    {
                    case IN_MODE_ADC:    return "ADC";
                    case IN_MODE_TRG:    return "TRG";
                //  case IN_MODE_BMP:    return "BMP";
                    case IN_MODE_LF1:    return "LF1";
                    case IN_MODE_LF2:    return "LF2";
                    case MODE_AU_AL:     return "LOW0";
                    case MODE_AU_AH:     return "HIGH0";
                    case MODE_AU_BL:     return "LOW1";
                    case MODE_AU_BH:     return "HIGH1";
                    default:             return "ADC";
                    }
}

const char*     CKernel::bufferWaveName             (   u32         p_value)
{
                switch (p_value)
                    {
                    case 0:              return "SIN";
                    case 1:              return "SAW";
                    case 2:              return "RSAW";
                    case 3:              return "TRI";
                    case 4:              return "RND";
                    default:             return "SIN";
                    }
}

const char*     CKernel::bufferChannelName          (   u32         p_value)
{
                switch (p_value)
                    {
                    case MODE_CH0:       return "CH0";
                    case MODE_CH1:       return "CH1";
                    case MODE_CH2:       return "CH2";
                    case MODE_CH3:       return "CH3";
                    case MODE_CH4:       return "CH4";
                    case MODE_CH5:       return "CH5";
                    case MODE_CH6:       return "CH6";
                    case MODE_CH7:       return "CH7";
                    default:             return "CH0";
                    }
}

u32             CKernel::bufferMultValue            (   u32         p_index)
{
                switch (p_index)
                    {
                    case 0:              return 1;
                    case 1:              return 2;
                    case 2:              return 3;
                    case 3:              return 4;
                    case 4:              return 8;
                    case 5:              return 16;
                    case 6:              return 32;
                    case 7:              return 64;
                    default:             return 1;
                    }
}

void            CKernel::create_buffer_file         (   )
{
                MY_BFR[MY_IDX] = '\0';

                storeLogHex(MY_BFR, MY_IDX, "# central mode buffer", X_VAL );

                nextline(MY_BFR, MY_IDX);

                for (u32 program = 0; program < filecounter[FT_FSH][FLD_VALID]; ++program)
                    {
                    if (program == DEFAULT_SLOT)
                        {
                        continue;
                        }

                    storeLogHex(MY_BFR, MY_IDX, "program", hexToDec(program) );

                    if (g_ScnFsh[program])
                        {
                        storeLogHex(MY_BFR, MY_IDX, "file", X_VAL, g_ScnFsh[program] );
                        }

                    nextline   (MY_BFR, MY_IDX);
                    storeLogHex(MY_BFR, MY_IDX, "ch0", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH0]) );
                    storeLogHex(MY_BFR, MY_IDX, "ch1", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH1]) );
                    storeLogHex(MY_BFR, MY_IDX, "ch2", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH2]) );
                    storeLogHex(MY_BFR, MY_IDX, "ch3", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH3]) );
                    storeLogHex(MY_BFR, MY_IDX, "ch4", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH4]) );
                    storeLogHex(MY_BFR, MY_IDX, "ch5", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH5]) );
                    storeLogHex(MY_BFR, MY_IDX, "ch6", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH6]) );
                    storeLogHex(MY_BFR, MY_IDX, "ch7", X_VAL, bufferSourceModeName(g_centralModeBuffer[program][MODE_CH7]) );
                    nextline   (MY_BFR, MY_IDX);
                    storeLogHex(MY_BFR, MY_IDX, "lfo1", X_VAL, bufferWaveName(g_centralModeBuffer[program][LF1_WAVE]) );
                    storeLogHex(MY_BFR, MY_IDX, "mult1", hexToDec(bufferMultValue(g_centralModeBuffer[program][LF1_MULT])) );
                    storeLogHex(MY_BFR, MY_IDX, "lfo2", X_VAL, bufferWaveName(g_centralModeBuffer[program][LF2_WAVE]) );
                    storeLogHex(MY_BFR, MY_IDX, "mult2", hexToDec(bufferMultValue(g_centralModeBuffer[program][LF2_MULT])) );
                    nextline   (MY_BFR, MY_IDX);
                    storeLogHex(MY_BFR, MY_IDX, "audioA", hexToDec(g_centralModeBuffer[program][SENS_A]) );
                    storeLogHex(MY_BFR, MY_IDX, "audioB", hexToDec(g_centralModeBuffer[program][SENS_B]) );
                    storeLogHex(MY_BFR, MY_IDX, "audioC", hexToDec(g_centralModeBuffer[program][SENS_C]) );
                    storeLogHex(MY_BFR, MY_IDX, "audioD", hexToDec(g_centralModeBuffer[program][SENS_D]) );
                    nextline   (MY_BFR, MY_IDX);

                    if (g_centralModeBuffer[program][SEL_TIME] < FLAG_THRESHOLD) storeLogHex(MY_BFR, MY_IDX, "time", X_VAL, bufferChannelName(g_centralModeBuffer[program][SEL_TIME]) );
                    if (g_centralModeBuffer[program][SEL_TEX] < FLAG_THRESHOLD) storeLogHex(MY_BFR, MY_IDX, "tex", X_VAL, bufferChannelName(g_centralModeBuffer[program][SEL_TEX]) );
                    if (g_centralModeBuffer[program][SEL_VID] < FLAG_THRESHOLD) storeLogHex(MY_BFR, MY_IDX, "vid", X_VAL, bufferChannelName(g_centralModeBuffer[program][SEL_VID]) );
                    if (g_centralModeBuffer[program][SEL_VID] < FLAG_THRESHOLD) storeLogHex(MY_BFR, MY_IDX, "frm", X_VAL, bufferChannelName(g_centralModeBuffer[program][SEL_FRM]) );

                    nextline(MY_BFR, MY_IDX);
                    }
}

u32             CKernel::hexToDec                   (   u32         p_value)
{
                u32 result = 0;
                u32 shift  = 0;

                if (p_value == 0) return 0;

                while (p_value && shift < 32)
                    {
                    u32 digit = p_value % 10;
                    result |= digit << shift;
                    p_value /= 10;
                    shift += 4;
                    }

                return result;
}

bool            CKernel::sameText                   (   const char* p_a,
                                                        const char* p_b)
{
                while (*p_a && *p_b)
                    {
                    if (*p_a != *p_b) return false;

                    ++p_a;
                    ++p_b;
                    }

                return (*p_a == '\0' && *p_b == '\0');
}

bool            CKernel::tokenEnd                   (   char        p_char)
{
                return  p_char == '\0'
                    ||  p_char == ' '
                    ||  p_char == '\t'
                    ||  p_char == '\n'
                    ||  p_char == '\r'
                    ||  p_char == '#';
}

void            CKernel::skipBufferLine             (   const char*& p_read)
{
                while (*p_read && *p_read != '\n')
                    {
                    ++p_read;
                    }

                if (*p_read == '\n')
                    {
                    ++p_read;
                    }
}

bool            CKernel::readBufferToken            (   const char*& p_read,
                                                        char*       p_token,
                                                        u32         p_size)
{
                if (!p_size) return false;

                p_token[0] = '\0';

                while (*p_read == ' ' || *p_read == '\t')
                    {
                    ++p_read;
                    }

                if (*p_read == '\0' || *p_read == '\n' || *p_read == '\r' || *p_read == '#') return false;

                MY_IDX = 0;
                
                bool valid = true;

                while (!tokenEnd(*p_read))
                    {
                    if (MY_IDX < p_size - 1)
                        {
                        p_token[MY_IDX++] = *p_read;
                        }
                    else
                        {
                        valid = false;
                        }

                    ++p_read;
                    }

                p_token[MY_IDX] = '\0';

                return valid && MY_IDX;
}

bool            CKernel::readBufferDec              (   const char* p_token,
                                                        u32&        p_value)
{
                p_value = 0;

                if (p_token[0] == '0' && (p_token[1] == 'x' || p_token[1] == 'X'))
                    {
                    p_token += 2;
                    }

                bool found = false;

                while (*p_token)
                    {
                    if (*p_token < '0' || *p_token > '9')
                        {
                        return false;
                        }

                    p_value = (p_value * 10) + (*p_token - '0');
                    found = true;

                    ++p_token;
                    }

                return found;
}

bool            CKernel::readSourceMode             (   const char* p_token,
                                                        u32&        p_value)
{
                if (sameText(p_token, "ADC"))   { p_value = IN_MODE_ADC;  return true; }
                if (sameText(p_token, "TRG"))   { p_value = IN_MODE_TRG;  return true; }
            //  if (sameText(p_token, "BMP"))   { p_value = IN_MODE_BMP;  return true; }
                if (sameText(p_token, "LF1"))   { p_value = IN_MODE_LF1;  return true; }
                if (sameText(p_token, "LF2"))   { p_value = IN_MODE_LF2;  return true; }
                if (sameText(p_token, "LOW0"))  { p_value = MODE_AU_AL; return true; }
                if (sameText(p_token, "HIGH0")) { p_value = MODE_AU_AH; return true; }
                if (sameText(p_token, "LOW1"))  { p_value = MODE_AU_BL; return true; }
                if (sameText(p_token, "HIGH1")) { p_value = MODE_AU_BH; return true; }

                return false;
}

bool            CKernel::readWave                   (   const char* p_token,
                                                        u32&        p_value)
{
                if (sameText(p_token, "SIN"))  { p_value = 0; return true; }
                if (sameText(p_token, "SAW"))  { p_value = 1; return true; }
                if (sameText(p_token, "RSAW")) { p_value = 2; return true; }
                if (sameText(p_token, "TRI"))  { p_value = 3; return true; }

                return false;
}

bool            CKernel::readMultiplier             (   const char* p_token,
                                                        u32&        p_value)
{
                u32 value;

                if (!readBufferDec(p_token, value))
                    {
                    return false;
                    }

                switch (value)
                    {
                    case 1:  p_value = 0; return true;
                    case 2:  p_value = 1; return true;
                    case 3:  p_value = 2; return true;
                    case 4:  p_value = 3; return true;
                    case 8:  p_value = 4; return true;
                    case 16: p_value = 5; return true;
                    case 32: p_value = 6; return true;
                    case 64: p_value = 7; return true;
                    }

                return false;
}

bool            CKernel::readChannel                (   const char* p_token,
                                                        u32&        p_value)
{
                if (sameText(p_token, "CH0")) { p_value = MODE_CH0; return true; }
                if (sameText(p_token, "CH1")) { p_value = MODE_CH1; return true; }
                if (sameText(p_token, "CH2")) { p_value = MODE_CH2; return true; }
                if (sameText(p_token, "CH3")) { p_value = MODE_CH3; return true; }
                if (sameText(p_token, "CH4")) { p_value = MODE_CH4; return true; }
                if (sameText(p_token, "CH5")) { p_value = MODE_CH5; return true; }
                if (sameText(p_token, "CH6")) { p_value = MODE_CH6; return true; }
                if (sameText(p_token, "CH7")) { p_value = MODE_CH7; return true; }

                return false;
}

bool            CKernel::readChannelCommand         (   const char* p_token,
                                                        u32&        p_field)
{
                if (sameText(p_token, "ch0")) { p_field = MODE_CH0; return true; }
                if (sameText(p_token, "ch1")) { p_field = MODE_CH1; return true; }
                if (sameText(p_token, "ch2")) { p_field = MODE_CH2; return true; }
                if (sameText(p_token, "ch3")) { p_field = MODE_CH3; return true; }
                if (sameText(p_token, "ch4")) { p_field = MODE_CH4; return true; }
                if (sameText(p_token, "ch5")) { p_field = MODE_CH5; return true; }
                if (sameText(p_token, "ch6")) { p_field = MODE_CH6; return true; }
                if (sameText(p_token, "ch7")) { p_field = MODE_CH7; return true; }

                return false;
}

void            CKernel::parse_buffer_file          (   )
{
                memset(g_centralModeBufferTemp, 0, sizeof(g_centralModeBufferTemp));

                const char* read    = MY_BFR;
                u32         program = 0xFFFFFFFF;

                char        command[32];
                char        value[64];

                while (*read)
                    {
                    if (!readBufferToken(read, command, sizeof(command)))
                        {
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "program"))
                        {
                        u32 parsedProgram;

                        if (readBufferToken(read, value, sizeof(value)) && readBufferDec(value, parsedProgram) && parsedProgram < filecounter[FT_FSH][FLD_VALID] && parsedProgram != DEFAULT_SLOT)
                            {
                            program = parsedProgram;
                            }
                        else
                            {
                            program = 0xFFFFFFFF;
                            }

                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "file"))
                        {
                        skipBufferLine(read);
                        continue;
                        }

                    if (program == 0xFFFFFFFF)
                        {
                        skipBufferLine(read);
                        continue;
                        }

                    u32 field;
                    u32 parsedValue;

                    if (readChannelCommand(command, field))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readSourceMode(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][field] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "lfo1"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readWave(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][LF1_WAVE] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "lfo2"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readWave(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][LF2_WAVE] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "mult1"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readMultiplier(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][LF1_MULT] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "mult2"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readMultiplier(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][LF2_MULT] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "audioA"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readBufferDec(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SENS_A] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "audioB"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readBufferDec(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SENS_B] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "audioC"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readBufferDec(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SENS_C] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "audioD"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readBufferDec(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SENS_D] = parsedValue;
                            }

                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "time"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readChannel(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SEL_TIME] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "tex"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readChannel(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SEL_TEX] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "vid"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readChannel(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SEL_VID] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }

                    if (sameText(command, "frm"))
                        {
                        if (readBufferToken(read, value, sizeof(value)) && readChannel(value, parsedValue))
                            {
                            g_centralModeBufferTemp[program][SEL_FRM] = parsedValue;
                            }
                        skipBufferLine(read);
                        continue;
                        }
                    skipBufferLine(read);
                    }
}
