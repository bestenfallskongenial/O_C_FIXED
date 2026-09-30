// i guess i need to scan gpt for this conversation!!!
                // The single audio group contains either: Model 2: two audio modes / Model 3: four audio modes
                const unsigned audioFirstMode = g_groupModes[GROUP_AUDIO][0];

                const unsigned audioModeCount = g_groupLen[GROUP_AUDIO];

                const unsigned audioLastMode = audioFirstMode + audioModeCount - 1;


                // FLAG_AUDIO is the existing audio/no-audio state.
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO])
                    {
                    // is_audio contains the physical ADC input channel, 0-3, on which the audio signal was detected.
                    const unsigned audioChannel = is_audio;
                    // Read the selected position for that channel.
                    const unsigned selection = g_centralModeBuffer[g_currentProgramBuffer][audioChannel];
                    // Resolve the selected position to the mode that getChannelModeB() would execute.
                    const unsigned mode = g_modeMap[audioChannel][selection];
                    // The physical audio source must itself use one of the available audio modes.
                    if (mode < audioFirstMode || mode > audioLastMode)
                        {
                        // Find the packed position of the first audio mode in this channel's currently available mode map.
                        for (unsigned position = 0; position < (unsigned)g_modeRoof[audioChannel]; ++position)
                            {
                            if (g_modeMap[audioChannel][position] == audioFirstMode)
                                {
                                // Override the invalid selection made by mapMenuGroup().
                                g_centralModeBuffer [g_currentProgramBuffer][audioChannel] = position;
                                // The selection was changed by code rather than by the physical potentiometer.
                                g_menuPickUpFlag[audioChannel] = false;

                                break;
                                }
                            }
                        }
                    }
                else
                    {
                    // There is no detected audio signal. No channel may continue using an audio mode.
                    for (unsigned channel = 0; channel < 8; ++channel)
                        {
                        // Resolve this channel's stored selection to its mode.
                        const unsigned selection = g_centralModeBuffer[g_currentProgramBuffer][channel];

                        const unsigned mode = g_modeMap[channel][selection];
                        // Reset only channels that currently resolve to one of the audio modes.
                        if (mode >= audioFirstMode && mode <= audioLastMode)
                            {
                            // Position zero is the existing ADC fallback.
                            g_centralModeBuffer[g_currentProgramBuffer] [channel] = 0;
                            // Require the physical control to pick up the new programmatically assigned ADC selection.
                            g_menuPickUpFlag[channel] = false;
                            }
                        }
                    }

// OR:

                const unsigned audioFirstMode = g_groupModes[GROUP_AUDIO][0];                               // The single audio group contains either: Model 2: two audio modes / Model 3: four audio modes

                const unsigned audioModeCount = g_groupLen[GROUP_AUDIO];

                const unsigned audioLastMode = audioFirstMode + audioModeCount - 1;

                const unsigned audioFirstPosition = g_groupLen[GROUP_BASE];                                 // The audio group is appended immediately after the base group.

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO])                                // FLAG_AUDIO is the existing audio/no-audio state.
                    {
                    const unsigned audioChannel = is_audio;                                                 // is_audio is limited by audio detection to physical channels 0-3.
                    const unsigned selection = g_centralModeBuffer[g_currentProgramBuffer][audioChannel];   // Resolve the source channel's stored selection to its mode.
                    const unsigned mode = g_modeMap[audioChannel][selection];

                    if (mode < audioFirstMode || mode > audioLastMode)                                      // The physical audio source must itself use an audio mode.
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][audioChannel] = audioFirstPosition;     // The first audio position is known because GROUP_AUDIO follows GROUP_BASE in every packed mode map.
                        g_menuPickUpFlag[audioChannel] = false;                                             // The selection was changed by code.
                        }
                    }
                else
                    {
                    const unsigned selection0 =g_centralModeBuffer[g_currentProgramBuffer][0];              // Channel 0
                    const unsigned mode0 = g_modeMap[0][selection0];

                    if (mode0 >= audioFirstMode && mode0 <= audioLastMode)
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][0] = 0;
                        g_menuPickUpFlag[0] = false;
                        }

                    const unsigned selection1 = g_centralModeBuffer[g_currentProgramBuffer][1];             // Channel 1
                    const unsigned mode1 = g_modeMap[1][selection1];

                    if (mode1 >= audioFirstMode && mode1 <= audioLastMode)
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][1] = 0;
                        g_menuPickUpFlag[1] = false;
                        }
                    
                    const unsigned selection2 = g_centralModeBuffer[g_currentProgramBuffer][2];             // Channel 2
                    const unsigned mode2 = g_modeMap[2][selection2];

                    if (mode2 >= audioFirstMode && mode2 <= audioLastMode)
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][2] = 0;
                        g_menuPickUpFlag[2] = false;
                        }
                    
                    const unsigned selection3 = g_centralModeBuffer[g_currentProgramBuffer][3];             // Channel 3
                    const unsigned mode3 = g_modeMap[3][selection3];

                    if (mode3 >= audioFirstMode && mode3 <= audioLastMode)
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][3] = 0;
                        g_menuPickUpFlag[3] = false;
                        }
                    
                    const unsigned selection4 = g_centralModeBuffer[g_currentProgramBuffer][4];             // Channel 4
                    const unsigned mode4 = g_modeMap[4][selection4];

                    if (mode4 >= audioFirstMode && mode4 <= audioLastMode)
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][4] = 0;
                        g_menuPickUpFlag[4] = false;
                        }
                    
                    const unsigned selection5 = g_centralModeBuffer[g_currentProgramBuffer][5];             // Channel 5
                    const unsigned mode5 = g_modeMap[5][selection5];

                    if (mode5 >= audioFirstMode && mode5 <= audioLastMode)
                        {
                        g_centralModeBuffer [g_currentProgramBuffer][5] = 0;
                        g_menuPickUpFlag[5] = false;
                        }
                    
                    const unsigned selection6 = g_centralModeBuffer[g_currentProgramBuffer][6];             // Channel 6
                    const unsigned mode6 =  g_modeMap[6][selection6];

                    if (mode6 >= audioFirstMode && mode6 <= audioLastMode)
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][6] = 0;
                        g_menuPickUpFlag[6] = false;
                        }
                    
                    const unsigned selection7 = g_centralModeBuffer[g_currentProgramBuffer][7];             // Channel 7
                    const unsigned mode7 = g_modeMap[7][selection7];

                    if (mode7 >= audioFirstMode && mode7 <= audioLastMode)
                        {
                        g_centralModeBuffer[g_currentProgramBuffer][7] = 0;
                        g_menuPickUpFlag[7] = false;
                        }
                    }
