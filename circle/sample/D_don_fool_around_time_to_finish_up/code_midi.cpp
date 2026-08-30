#include "kernel.h"

#ifdef USE_MIDI

bool            CKernel::updateMIDI()
{
                if (!m_USBHCI.UpdatePlugAndPlay()) return false;

                CUSBMIDIDevice* pMIDI = (CUSBMIDIDevice*)m_DeviceNameService.GetDevice("umidi1", TRUE);

                if (pMIDI == nullptr) return false;

                if (!g_midiConnected)
                    {
                    pMIDI->RegisterPacketHandler(MIDIPacketHandler, this);
                    pMIDI->RegisterRemovedHandler(removeMIDI, this);

                    g_midiConnected = true;

                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_MIDI] = g_midiConnected; // aka true aka 1
                    }

                return true;
}

void            CKernel::removeMIDI(CDevice* pDevice, void* pContext)
{
                (void)pDevice;

                CKernel* pThis = (CKernel*)pContext;

                if (pThis == 0) return;

                pThis->g_midiConnected = false;
                pThis->g_centralModeBuffer[pThis->g_currentProgramBuffer][FLAG_MIDI] = pThis->g_midiConnected; // aka false aka 0;
                memset(pThis->g_midiHeld, 0, sizeof(pThis->g_midiHeld));
}

void            CKernel::MIDIPacketHandler(unsigned nCable,
                                u8* pPacket,
                                unsigned nLength,
                                unsigned nDevice,
                                void* pParam)
{
                (void)nCable;
                (void)nDevice;

                CKernel* pThis = (CKernel*)pParam;

                if (pThis == 0 || nLength < 3) return;

                const unsigned command = pPacket[0] & 0xF0;
                const unsigned channel = pPacket[0] & 0x0F;

                if (channel != pThis->g_centralModeBuffer[pThis->g_currentProgramBuffer][MIDI_CHANNEL]) return;

                if (command == 0xB0)
                    {
                    if (pPacket[1] == pThis->g_centralModeBuffer[pThis->g_currentProgramBuffer][MIDI_CC0])
                        {
                        pThis->g_midiCC0Int = (pPacket[2] * 1023) / 127;
                        pThis->g_midiCC0Flt = pThis->g_midiCC0Int / 1024.0f;
                        }
                    if (pPacket[1] == pThis->g_centralModeBuffer[pThis->g_currentProgramBuffer][MIDI_CC1])
                        {
                        pThis->g_midiCC1Int = (pPacket[2] * 1023) / 127;
                        pThis->g_midiCC1Flt = pThis->g_midiCC1Int / 1024.0f;
                        }
                    return;
                    }

                if (command == 0x90 && pPacket[2] != 0)
                    {
                    pThis->g_midiHeld[pPacket[1]] = true;
                    }
                else if (command == 0x80 || (command == 0x90 && pPacket[2] == 0))
                    {
                    pThis->g_midiHeld[pPacket[1]] = false;
                    }
                else
                    {
                    return;
                    }

                for (int note = 127; note >= 0; --note)
                    {
                    if (pThis->g_midiHeld[note])
                        {
                        pThis->g_midiNote = note;
                        return;
                        }
                    }
}
#endif 