
#include "kernel.h"

    #define MY_BUFFER   m_bufferLog                 // not used here
    #define MY_INDEX    m_bufferLogIndex
    
void            CKernel::usDelay(unsigned us)
{
                if (us == 0) return;

                const u32 ticks = us * (CLOCKHZ / 1000000) + 1;

                PeripheralEntry();

                const u32 start = read32(ARM_SYSTIMER_CLO);

                while ((read32(ARM_SYSTIMER_CLO) - start) < ticks)
                    {
                    }

                PeripheralExit();
}

void            CKernel::msDelay(unsigned ms)
{
                if (ms == 0) return;

                const u32 us    = ms * 1000;
                const u32 ticks = us * (CLOCKHZ / 1000000) + 1;

                PeripheralEntry();

                const u32 start = read32(ARM_SYSTIMER_CLO);

                while ((read32(ARM_SYSTIMER_CLO) - start) < ticks)
                    {
                    }

                PeripheralExit();
}

u32             CKernel::read32                  (   uintptr nAddress)
{
                return *(volatile u32 *) nAddress;
}

void            CKernel::write32                 (   uintptr nAddress, 
                                                        u32 nValue)
{
                *(volatile u32 *) nAddress = nValue;
}

void            CKernel::GPIO_SetPull            (   unsigned nPin, 
                                                        unsigned nPullMode)
{
                uintptr nClkReg = ARM_GPIO_GPPUDCLK0 + ((nPin / 32) * 4);

                u32 nMask = 1 << (nPin % 32);

                write32(ARM_GPIO_GPPUD, nPullMode);

            //  CTimer::SimpleusDelay(5);
                usDelay(5);
                write32(nClkReg, nMask);

            //  CTimer::SimpleusDelay(5);
                usDelay(5);
                write32(ARM_GPIO_GPPUD, 0);
                write32(nClkReg, 0);
}

void            CKernel::GPIO_SetAlt             (  unsigned nPin, 
                                                    unsigned nAltMode, 
                                                    unsigned nPullMode)
{
                GPIO_SetPull(nPin, nPullMode);

                uintptr nSelReg = ARM_GPIO_GPFSEL0 + (nPin / 10) * 4;
                unsigned nShift = (nPin % 10) * 3;

                u32 nValue = read32(nSelReg);

                nValue &= ~(7 << nShift);
                nValue |=  (nAltMode << nShift);

                write32(nSelReg, nValue);
}

void            CKernel::GPIO_Write              (  unsigned nPin, 
                                                    unsigned nValue)
{
                uintptr nReg = (nValue ? ARM_GPIO_GPSET0 : ARM_GPIO_GPCLR0) + ((nPin / 32) * 4);

                u32 nMask = 1 << (nPin % 32);

                write32(nReg, nMask);
}

void            CKernel::set_pot_routing            (   int     pin, 
                                                        bool    state)
{
            //  m_ChipSelectPin.Write(adc_pot_routing); // false or true aka high or low?
                GPIO_Write(pin, state);
}

unsigned        CKernel::GPIO_Read               (   unsigned nPin)
{
                uintptr nReg = ARM_GPIO_GPLEV0 + ((nPin / 32) * 4);

                u32 nMask = 1 << (nPin % 32);

                return (read32(nReg) & nMask) ? HIGH : LOW;
}

void            CKernel::buttonPingA                (   int             p_btn_id, 
                                                        int             p_pin )
{
                if (GPIO_Read(p_pin) == BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)                
                    {
                    g_buttons_states[p_btn_id][BTN_SINGLE] = 0;
                    g_buttons_states[p_btn_id][BTN_DOUBLE] = 0;

                    g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;


                    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
                        {
                        g_buttons_states[p_btn_id][BTN_DOUBLE] = 1;
                        }
                    else
                        {
                        g_buttons_states[p_btn_id][BTN_SINGLE] = 1;
                        }
                    g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
                    }

                if (GPIO_Read(p_pin) != BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] != 0)                
                    {
                    g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]   = 0;
                    }
                if (g_buttons_states[p_btn_id][BTN_PRESS_START] != 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START]) >= g_long_click_time)
                    {
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]++;
                    }
}

void            CKernel::buttonPingB                (   int             p_btn_id_A, 
                                                        int             p_pin_A,
                                                        int             p_btn_id_B, 
                                                        int             p_pin_B )
{
                if (GPIO_Read(p_pin_A) == BTN_PRESSED && g_buttons_states[p_btn_id_A][BTN_PRESS_START] == 0)                
                    {
                    g_buttons_states[p_btn_id_A][BTN_SINGLE] = 0;
                    g_buttons_states[p_btn_id_A][BTN_DOUBLE] = 0;

                    g_buttons_states[p_btn_id_A][BTN_PRESS_START] = g_currentTime;


                    if (g_buttons_states[p_btn_id_A][BTN_RELEASE] > 0 && (g_currentTime - g_buttons_states[p_btn_id_A][BTN_RELEASE]) < g_double_click_time)
                        {
                        g_buttons_states[p_btn_id_A][BTN_DOUBLE] = 1;
                        }
                    else
                        {
                        g_buttons_states[p_btn_id_A][BTN_SINGLE] = 1;
                        }
                    g_buttons_states[p_btn_id_A][BTN_RELEASE] = 0;
                    }

                if (GPIO_Read(p_pin_A) != BTN_PRESSED && g_buttons_states[p_btn_id_A][BTN_PRESS_START] != 0)                
                    {
                    g_buttons_states[p_btn_id_A][BTN_RELEASE]     = g_currentTime;
                    g_buttons_states[p_btn_id_A][BTN_PRESS_START] = 0;
                    g_buttons_states[p_btn_id_A][BTN_HOLD_TICK]   = 0;
                    }
                if (g_buttons_states[p_btn_id_A][BTN_PRESS_START] != 0 && (g_currentTime - g_buttons_states[p_btn_id_A][BTN_PRESS_START]) >= g_long_click_time)
                    {
                    g_buttons_states[p_btn_id_A][BTN_HOLD_TICK]++;
                    }



                if (GPIO_Read(p_pin_B) == BTN_PRESSED && g_buttons_states[p_btn_id_B][BTN_PRESS_START] == 0)                
                    {
                    g_buttons_states[p_btn_id_B][BTN_SINGLE] = 0;
                    g_buttons_states[p_btn_id_B][BTN_DOUBLE] = 0;

                    g_buttons_states[p_btn_id_B][BTN_PRESS_START] = g_currentTime;


                    if (g_buttons_states[p_btn_id_B][BTN_RELEASE] > 0 && (g_currentTime - g_buttons_states[p_btn_id_B][BTN_RELEASE]) < g_double_click_time)
                        {
                        g_buttons_states[p_btn_id_B][BTN_DOUBLE] = 1;
                        }
                    else
                        {
                        g_buttons_states[p_btn_id_B][BTN_SINGLE] = 1;
                        }
                    g_buttons_states[p_btn_id_B][BTN_RELEASE] = 0;
                    }

                if (GPIO_Read(p_pin_B) != BTN_PRESSED && g_buttons_states[p_btn_id_B][BTN_PRESS_START] != 0)                
                    {
                    g_buttons_states[p_btn_id_B][BTN_RELEASE]     = g_currentTime;
                    g_buttons_states[p_btn_id_B][BTN_PRESS_START] = 0;
                    g_buttons_states[p_btn_id_B][BTN_HOLD_TICK]   = 0;
                    }
                if (g_buttons_states[p_btn_id_B][BTN_PRESS_START] != 0 && (g_currentTime - g_buttons_states[p_btn_id_B][BTN_PRESS_START]) >= g_long_click_time)
                    {
                    g_buttons_states[p_btn_id_B][BTN_HOLD_TICK]++;
                    }
}

void            CKernel::watchdog_Start          (   unsigned nTimeoutSeconds)
{
                if (nTimeoutSeconds > MAX_WATCHDOG_SEC) nTimeoutSeconds = MAX_WATCHDOG_SEC;

                write32( ARM_PM_WDOG, ARM_PM_PASSWD | ((nTimeoutSeconds << 16) & ARM_PM_WDOG_TIME) );

                write32( ARM_PM_RSTC, ARM_PM_PASSWD | ARM_PM_RSTC_REBOOT | (read32(ARM_PM_RSTC) & ARM_PM_RSTC_CLEAR) );
}

bool            CKernel::SPI_init                (   void)
{
                m_SPIBaseAddress = SPI0_BASE;

                if (m_SPIBaseAddress == 0) return FALSE;

                GPIO_SetAlt(MISO, 4, GPIO_PULL_OFF);
                GPIO_SetAlt(MOSI, 4, GPIO_PULL_OFF);
                GPIO_SetAlt(SCLK, 4, GPIO_PULL_OFF);
                GPIO_SetAlt(CE0,  4, GPIO_PULL_OFF);
                GPIO_SetAlt(CE1,  4, GPIO_PULL_OFF);

                unsigned nCoreClockRate = CMachineInfo::Get()->GetClockRate(CLOCK_ID_CORE);

                if (nCoreClockRate == 0 || SPI_CLOCK_SPEED < 4000 || SPI_CLOCK_SPEED > 125000000) return FALSE;

                PeripheralEntry();

                write32(ARM_SPI_CLK, nCoreClockRate / SPI_CLOCK_SPEED);

                write32(ARM_SPI_CS, (0 << CS_CPOL_SHIFT) | (0 << CS_CPHA_SHIFT));

                PeripheralExit();

                m_SPIValid = TRUE;

                return TRUE;
}

int             CKernel::WriteRead                  (   unsigned nChipSelect,
                                                        const void *pWriteBuffer,
                                                        void *pReadBuffer,
                                                        unsigned nCount)
{
                if (!m_SPIValid
                    || m_SPIBaseAddress == 0
                    || (pWriteBuffer == 0 && pReadBuffer == 0)
                    || nCount == 0
                    || nCount > 0xFFFF
                    || (nChipSelect > 1 && nChipSelect != CS_NONE))
                    {
                    return -1;
                    }

                    const u8 *pWritePtr = (const u8 *) pWriteBuffer;
                    u8 *pReadPtr = (u8 *) pReadBuffer;

                //  m_SpinLock.Acquire ();

                    PeripheralEntry();

                    write32(ARM_SPI_DLEN, nCount);

                    write32(ARM_SPI_CS, (read32(ARM_SPI_CS) & ~CS_CS) | (nChipSelect << CS_CS_SHIFT) | CS_CLEAR_RX | CS_CLEAR_TX | CS_TA);

                    unsigned nWriteCount = 0;
                    unsigned nReadCount  = 0;

                    while (nWriteCount < nCount || nReadCount < nCount)
                        {
                        while (nWriteCount < nCount && (read32(ARM_SPI_CS) & CS_TXD))
                            {
                            u32 nData = 0;

                            if (pWritePtr != 0)
                                {
                                nData = *pWritePtr++;
                                }
                            write32(ARM_SPI_FIFO, nData);

                            nWriteCount++;
                            }
                        while (nReadCount < nCount && (read32(ARM_SPI_CS) & CS_RXD))
                            {
                            u32 nData = read32(ARM_SPI_FIFO);

                            if (pReadPtr != 0)
                                {
                                *pReadPtr++ = (u8) nData;
                                }
                            nReadCount++;
                            }
                        }
                    while (!(read32(ARM_SPI_CS) & CS_DONE))
                        {
                        while (read32(ARM_SPI_CS) & CS_RXD)
                            {
                            read32(ARM_SPI_FIFO);
                            }
                        }

                    write32(ARM_SPI_CS, read32(ARM_SPI_CS) & ~CS_TA);

                    PeripheralExit();

                // 	m_SpinLock.Release ();

                    return (int) nCount;
}

bool            CKernel::SMI_Init                (   unsigned gpioPin)
{
                if (gpioPin < 8 || gpioPin > 25) return FALSE;


                m_SMIGpioPin = gpioPin;

                m_SMISDMask = (1 << (gpioPin - 8)); // GPIO8 -> SD0 -> (1 << 0) GPIO9 -> SD1 -> (1 << 1)

                GPIO_SetAlt(gpioPin, GPIO_ALT1, GPIO_PULL_OFF); // switch GPIO to SMI ALT1

                m_SMIValid = TRUE;

                return TRUE;
}

void            CKernel::SMI_SetupTiming            (   unsigned width,
                                                        unsigned cycle_ns,
                                                        unsigned setup,
                                                        unsigned strobe,
                                                        unsigned hold,
                                                        unsigned pace)
                {
                u32 divi = cycle_ns / 2;

                PeripheralEntry();

                write32(ARM_SMI_CS,   0);
                write32(ARM_SMI_L,    0);
                write32(ARM_SMI_A,    0);
                write32(ARM_SMI_DSR0, 0);
                write32(ARM_SMI_DSW0, 0);
                write32(ARM_SMI_DCS,  0);
                write32(ARM_SMI_DCA,  0);

                if (read32(ARM_CM_SMICTL) != (divi << CM_SMIDIV_DIVI_SHIFT))
                    {
                    write32(ARM_CM_SMICTL, ARM_CM_PASSWD | CM_SMICTL_KILL);
                //  CTimer::Get()->usDelay(10);
                    usDelay(10);
                    while (read32(ARM_CM_SMICTL) & CM_SMICTL_BUSY) {}

                //  CTimer::Get()->usDelay(10);
                    usDelay(10);
                    write32( ARM_CM_SMIDIV, ARM_CM_PASSWD | (divi << CM_SMIDIV_DIVI_SHIFT));

                //  CTimer::Get()->usDelay(10);
                    usDelay(10);
                    write32( ARM_CM_SMICTL, ARM_CM_PASSWD | 6 | CM_SMICTL_ENAB );

                //  CTimer::Get()->usDelay(10);
                    usDelay(10);
                    while ((read32(ARM_CM_SMICTL) & CM_SMICTL_BUSY) == 0) {}
                //  CTimer::Get()->usDelay(100);
                    usDelay(100);
                    }

                u32 timing =    (width   << SMI_WWIDTH_SHIFT)  | (setup   << SMI_WSETUP_SHIFT) | 
                                (strobe  << SMI_WSTROBE_SHIFT) | (hold    << SMI_WHOLD_SHIFT)  | 
                                (pace    << SMI_WPACE_SHIFT);

                write32(ARM_SMI_DSW0, timing);
                write32(ARM_SMI_DSR0, timing);

                PeripheralExit();
}

void            CKernel::SMI_SetupDMA            (   size_t byteLength)
{
                PeripheralEntry();

                write32( ARM_SMI_DMC,   (SMI_DMA_REQUEST_THRESH << SMI_DMC_REQW_SHIFT) | (SMI_DMA_REQUEST_THRESH << SMI_DMC_REQR_SHIFT) | 
                                        (SMI_DMA_PANIC_LEVEL << SMI_DMC_PANICW_SHIFT) | (SMI_DMA_PANIC_LEVEL << SMI_DMC_PANICR_SHIFT)  | SMI_DMC_DMAEN );
                
                write32( ARM_SMI_CS, SMI_CS_ENABLE | SMI_CS_CLEAR | SMI_CS_PXLDAT );

                write32( ARM_SMI_L, byteLength );

                write32( ARM_SMI_CS, read32(ARM_SMI_CS) | SMI_CS_WRITE );

                PeripheralExit();
}

bool            CKernel::WS2812_Init             (   unsigned ledCount)
{
                if (!m_SMIValid || ledCount == 0) return FALSE;

                m_LEDCount = ledCount;

                m_BufferLength = TX_BUFF_LEN(ledCount);

                m_pBuffer = new TXDATA_T[m_BufferLength];

                if (m_pBuffer == 0) return FALSE;
  
                memset(m_pBuffer, 0, m_BufferLength * sizeof(TXDATA_T));

                SMI_SetupTiming(    SMI_WIDTH_16, NEOPIXEL_SMI_NS, NEOPIXEL_SMI_SETUP, NEOPIXEL_SMI_STROBE, NEOPIXEL_SMI_HOLD, NEOPIXEL_SMI_PACE );

                size_t byteLength = m_BufferLength * sizeof(TXDATA_T);

                SMI_SetupDMA(byteLength);

                return TRUE;
}

void            CKernel::WS2812_SetLED           (   unsigned index, u8 red, u8 green, u8 blue)
{
                if (!m_SMIValid || index >= m_LEDCount) return;

                TXDATA_T* tx = &m_pBuffer[LED_TX_OSET(index)];

                unsigned grb = ((unsigned) green << 16) | ((unsigned) red   << 8) | ((unsigned) blue  << 0);

                for (unsigned mask = (1 << 23); mask > 0; mask >>= 1)
                    {
                    tx[0] = 0xFFFF;

                    if (grb & mask) tx[1] = m_SMISDMask;

                    else            tx[1] = 0;

                    tx[2] = 0;

                    tx += BIT_NPULSES;
                    }
}

void            CKernel::WS2812_Update              (   void)
{
                if (!m_SMIValid) return;

                m_SMITxDMA.SetupIOWrite( ARM_SMI_D, m_pBuffer, m_BufferLength * sizeof(TXDATA_T), DREQSourceSMI );

                m_SMITxDMA.Start();

                PeripheralEntry();

                write32( ARM_SMI_CS, read32(ARM_SMI_CS) | SMI_CS_START );

                PeripheralExit();
}

int             CKernel::ReadMCP3008Raw             (   unsigned    channel)
{
                u8 tx[3] = { 0x01, (u8)((0x08 | channel) << 4), 0x00 };
                u8 rx[3];

                if (WriteRead(SPI_CHIP_SELECT, tx, rx, 3) != 3) return -1;

                return ((rx[1] & 0x03) << 8) | rx[2];
}

void            CKernel::readAndConvertADC         (   void    )
{
                const int f_scale = m_scaleFactors[attenuation];

                m_adc_ring[0][m_adc_index] = ReadMCP3008Raw(0);
                g_inOutMatrixInt[0][RAW] = (m_adc_ring[0][0] + m_adc_ring[0][1] + m_adc_ring[0][2] + m_adc_ring[0][3]) >> 2;
                g_inOutMatrixInt[0][VAL] = (g_inOutMatrixInt[0][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[0][VAL] = g_inOutMatrixInt[0][VAL] * 0.0009765625f;

                m_adc_ring[1][m_adc_index] = ReadMCP3008Raw(1);
                g_inOutMatrixInt[1][RAW] = (m_adc_ring[1][0] + m_adc_ring[1][1] + m_adc_ring[1][2] + m_adc_ring[1][3]) >> 2;
                g_inOutMatrixInt[1][VAL] = (g_inOutMatrixInt[1][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[1][VAL] = g_inOutMatrixInt[1][VAL] * 0.0009765625f;

                m_adc_ring[2][m_adc_index] = ReadMCP3008Raw(2);
                g_inOutMatrixInt[2][RAW] = (m_adc_ring[2][0] + m_adc_ring[2][1] + m_adc_ring[2][2] + m_adc_ring[2][3]) >> 2;
                g_inOutMatrixInt[2][VAL] = (g_inOutMatrixInt[2][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[2][VAL] = g_inOutMatrixInt[2][VAL] * 0.0009765625f;

                m_adc_ring[3][m_adc_index] = ReadMCP3008Raw(3);
                g_inOutMatrixInt[3][RAW] = (m_adc_ring[3][0] + m_adc_ring[3][1] + m_adc_ring[3][2] + m_adc_ring[3][3]) >> 2;
                g_inOutMatrixInt[3][VAL] = (g_inOutMatrixInt[3][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[3][VAL] = g_inOutMatrixInt[3][VAL] * 0.0009765625f;

                m_adc_ring[4][m_adc_index] = ReadMCP3008Raw(4);
                g_inOutMatrixInt[4][RAW] = (m_adc_ring[4][0] + m_adc_ring[4][1] + m_adc_ring[4][2] + m_adc_ring[4][3]) >> 2;
                g_inOutMatrixInt[4][VAL] = (g_inOutMatrixInt[4][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[4][VAL] = g_inOutMatrixInt[4][VAL] * 0.0009765625f;

                m_adc_ring[5][m_adc_index] = ReadMCP3008Raw(5);
                g_inOutMatrixInt[5][RAW] = (m_adc_ring[5][0] + m_adc_ring[5][1] + m_adc_ring[5][2] + m_adc_ring[5][3]) >> 2;
                g_inOutMatrixInt[5][VAL] = (g_inOutMatrixInt[5][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[5][VAL] = g_inOutMatrixInt[5][VAL] * 0.0009765625f;

                m_adc_ring[6][m_adc_index] = ReadMCP3008Raw(6);
                g_inOutMatrixInt[6][RAW] = (m_adc_ring[6][0] + m_adc_ring[6][1] + m_adc_ring[6][2] + m_adc_ring[6][3]) >> 2;
                g_inOutMatrixInt[6][VAL] = (g_inOutMatrixInt[6][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[6][VAL] = g_inOutMatrixInt[6][VAL] * 0.0009765625f;

                m_adc_ring[7][m_adc_index] = ReadMCP3008Raw(7);
                g_inOutMatrixInt[7][RAW] = (m_adc_ring[7][0] + m_adc_ring[7][1] + m_adc_ring[7][2] + m_adc_ring[7][3]) >> 2;
                g_inOutMatrixInt[7][VAL] = (g_inOutMatrixInt[7][RAW] * f_scale) >> 10;
                g_inOutMatrixFlt[7][VAL] = g_inOutMatrixInt[7][VAL] * 0.0009765625f;
}
/*
void            CKernel::adc_ProcessAudio           (   void    )
{
                if (!m_audio_mode_activated) return; // is a fixed position in g_centralModeBuffer mapped by modeMenuAssignGroup()

                int i0 = m_adc_index & 3;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

            //  int w0 = g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63;
            //  int w1 = g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63;
            //  int w2 = g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63;
            //  int w3 = g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63;

                int w0 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63) + 1;
                int w1 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63) + 1;
                int w2 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63) + 1;
                int w3 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63) + 1;

                g_audioIreg[0] = m_adc_ring[0][i0] - m_adc_ring[0][i1] + m_adc_ring[0][i2] - m_adc_ring[0][i3];

                if (g_audioIreg[0] > AUDIO_THRESHOLD || g_audioIreg[0] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 0;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                //  FLAG_AUDIO_A = true;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    float s = m_adc_ring[0][i0] * 0.0009765625f;

                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixFlt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixInt[0][AU0] = (int)((m_sum[0] * 1023.0f) / w0); // new - we need int too for the led!
                    if (++m_idx0 == w0) m_idx0 = 0;

                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixFlt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixInt[0][AU1] = (int)((m_sum[1] * 1023.0f) / w1); 
                    if (++m_idx1 == w1) m_idx1 = 0;
                }

                g_audioIreg[1] = m_adc_ring[1][i0] - m_adc_ring[1][i1] + m_adc_ring[1][i2] - m_adc_ring[1][i3];

                if (g_audioIreg[1] > AUDIO_THRESHOLD || g_audioIreg[1] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 1;

                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                //  FLAG_AUDIO_B = true;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = 1;

                    float s = m_adc_ring[1][i0] * 0.0009765625f;

                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixFlt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixInt[0][AU2] = (int)((m_sum[2] * 1023.0f) / w2);
                    if (++m_idx2 == w2) m_idx2 = 0;

                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixFlt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixInt[0][AU3] = (int)((m_sum[3] * 1023.0f) / w3);
                    if (++m_idx3 == w3) m_idx3 = 0;
                }

                g_audioIreg[2] = m_adc_ring[2][i0] - m_adc_ring[2][i1] + m_adc_ring[2][i2] - m_adc_ring[2][i3];

                if (g_audioIreg[2] > AUDIO_THRESHOLD || g_audioIreg[2] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 2;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                //  FLAG_AUDIO_A = true;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    float s = m_adc_ring[2][i0] * 0.0009765625f;

                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixFlt[0][AU0] =  m_sum[0] / w0;
                    g_inOutMatrixInt[0][AU0] = (int)((m_sum[0] * 1023.0f) / w0);
                    if (++m_idx0 == w0) m_idx0 = 0;

                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixFlt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixInt[0][AU1] = (int)((m_sum[1] * 1023.0f) / w1);
                    if (++m_idx1 == w1) m_idx1 = 0;
                }

                g_audioIreg[3] = m_adc_ring[3][i0] - m_adc_ring[3][i1] + m_adc_ring[3][i2] - m_adc_ring[3][i3];

                if (g_audioIreg[3] > AUDIO_THRESHOLD || g_audioIreg[3] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 3;

                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                //  FLAG_AUDIO_B = true;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = 1;

                    float s = m_adc_ring[3][i0] * 0.0009765625f;

                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixFlt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixInt[0][AU2] = (int)((m_sum[2] * 1023.0f) / w2);
                    if (++m_idx2 == w2) m_idx2 = 0;

                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixFlt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixInt[0][AU3] = (int)((m_sum[3] * 1023.0f) / w3);
                    if (++m_idx3 == w3) m_idx3 = 0;
                }

                if (m_audio_hold_A > 0) --m_audio_hold_A;
            //  FLAG_AUDIO_A = (m_audio_hold_A > 0);
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = (m_audio_hold_A > 0);

                if (m_audio_hold_B > 0) --m_audio_hold_B;
            //  FLAG_AUDIO_B = (m_audio_hold_B > 0);
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = (m_audio_hold_B > 0);
}
*/
void            CKernel::adc_ProcessAudio           (   void    )
{
                if (!m_audio_mode_activated) return; // is a fixed position in g_centralModeBuffer mapped by modeMenuAssignGroup()

                int i0 = m_adc_index & 3;
                int i1 = (m_adc_index - 1) & 3;
                int i2 = (m_adc_index - 2) & 3;
                int i3 = (m_adc_index - 3) & 3;

                int w0 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63) + 1;
                int w1 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63) + 1;
                int w2 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63) + 1;
                int w3 = (g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63) + 1;

                g_audioIreg[0] = m_adc_ring[0][i0] - m_adc_ring[0][i1] + m_adc_ring[0][i2] - m_adc_ring[0][i3];

                if (g_audioIreg[0] > AUDIO_THRESHOLD || g_audioIreg[0] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 0;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[0][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }

                g_audioIreg[1] = m_adc_ring[1][i0] - m_adc_ring[1][i1] + m_adc_ring[1][i2] - m_adc_ring[1][i3];

                if (g_audioIreg[1] > AUDIO_THRESHOLD || g_audioIreg[1] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 1;

                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = 1;

                    int s = m_adc_ring[1][i0];

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }

                g_audioIreg[2] = m_adc_ring[2][i0] - m_adc_ring[2][i1] + m_adc_ring[2][i2] - m_adc_ring[2][i3];

                if (g_audioIreg[2] > AUDIO_THRESHOLD || g_audioIreg[2] < -AUDIO_THRESHOLD)
                {
                    is_audio[0] = 2;

                    m_audio_hold_A = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = 1;

                    int s = m_adc_ring[2][i0];

                    if (m_idx0 >= w0) m_idx0 = 0;
                    m_sum[0] -= m_band[0][m_idx0];
                    m_band[0][m_idx0] = s;
                    m_sum[0] += s;
                    g_inOutMatrixInt[0][AU0] = m_sum[0] / w0;
                    g_inOutMatrixFlt[0][AU0] = (float)g_inOutMatrixInt[0][AU0] / 1023.0f;
                    if (++m_idx0 >= w0) m_idx0 = 0;

                    if (m_idx1 >= w1) m_idx1 = 0;
                    m_sum[1] -= m_band[1][m_idx1];
                    m_band[1][m_idx1] = s;
                    m_sum[1] += s;
                    g_inOutMatrixInt[0][AU1] = m_sum[1] / w1;
                    g_inOutMatrixFlt[0][AU1] = (float)g_inOutMatrixInt[0][AU1] / 1023.0f;
                    if (++m_idx1 >= w1) m_idx1 = 0;
                }

                g_audioIreg[3] = m_adc_ring[3][i0] - m_adc_ring[3][i1] + m_adc_ring[3][i2] - m_adc_ring[3][i3];

                if (g_audioIreg[3] > AUDIO_THRESHOLD || g_audioIreg[3] < -AUDIO_THRESHOLD)
                {
                    is_audio[1] = 3;

                    m_audio_hold_B = AUDIO_HOLD_TIMEOUT;
                    g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = 1;

                    int s = m_adc_ring[3][i0];

                    if (m_idx2 >= w2) m_idx2 = 0;
                    m_sum[2] -= m_band[2][m_idx2];
                    m_band[2][m_idx2] = s;
                    m_sum[2] += s;
                    g_inOutMatrixInt[0][AU2] = m_sum[2] / w2;
                    g_inOutMatrixFlt[0][AU2] = (float)g_inOutMatrixInt[0][AU2] / 1023.0f;
                    if (++m_idx2 >= w2) m_idx2 = 0;

                    if (m_idx3 >= w3) m_idx3 = 0;
                    m_sum[3] -= m_band[3][m_idx3];
                    m_band[3][m_idx3] = s;
                    m_sum[3] += s;
                    g_inOutMatrixInt[0][AU3] = m_sum[3] / w3;
                    g_inOutMatrixFlt[0][AU3] = (float)g_inOutMatrixInt[0][AU3] / 1023.0f;
                    if (++m_idx3 >= w3) m_idx3 = 0;
                }

                if (m_audio_hold_A > 0) --m_audio_hold_A;
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_A] = (m_audio_hold_A > 0);

                if (m_audio_hold_B > 0) --m_audio_hold_B;
                g_centralModeBuffer[g_currentProgramBuffer][FLAG_AUDIO_B] = (m_audio_hold_B > 0);
}

void            CKernel::adc_AdvanceIndex           (   void    )
{
                m_adc_index = (m_adc_index + 1) & 3;
}

bool            CKernel::frameBufferInit            (   void )
{
                if (!gE_FrameBuffer.Initialize ()) return FALSE;

                gE_PixelBuffer  = (u32 *) gE_FrameBuffer.GetBuffer ();
                gE_PitchBytes   = gE_FrameBuffer.GetPitch ();
                gE_ScreenWidth  = gE_FrameBuffer.GetWidth ();
                gE_ScreenHeight = gE_FrameBuffer.GetHeight ();
                gE_CharWidth    = gE_CharGenerator.GetCharWidth ();
                gE_CharHeight   = gE_CharGenerator.GetCharHeight ();

                if (gE_PixelBuffer == 0 || gE_CharWidth == 0 || gE_CharHeight == 0) return FALSE;

                gE_Cols = gE_ScreenWidth / gE_CharWidth;
                gE_Rows = gE_ScreenHeight / gE_CharHeight;

                if (gE_Cols == 0 || gE_Rows == 0) return FALSE;

                return TRUE;
}

void            CKernel::bufferScreenPlot         (   unsigned x, unsigned y, u32 color )
{
                gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}

void            CKernel::bufferScreenDrawChar     (   char        ch,
                                                      unsigned    charCol,
                                                      unsigned    charRow,
                                                      u32         fgColor )
{
                const unsigned px = charCol * gE_CharWidth;
                const unsigned py = charRow * gE_CharHeight;

                for (unsigned y = 0; y < gE_CharHeight; y++)
                    {
                    for (unsigned x = 0; x < gE_CharWidth; x++)
                        {
                        bufferScreenPlot (px + x, py + y,  gE_CharGenerator.GetPixel (ch, x, y) ? fgColor : 0 );
                        }
                    }
}

void            CKernel::bufferScreenClear        (   void)
{
                memset(gE_PixelBuffer, 0, gE_PitchBytes * gE_ScreenHeight);
}

void            CKernel::bufferScreenDraw   (   const char* pSourceBuffer,
                                                        u32         startIndex,
                                                        u32         endIndex,
                                                        unsigned    startCol,
                                                        unsigned    startRow,
                                                        u32         fgColor )
{
                if (startCol >= gE_Cols || startRow >= gE_Rows || pSourceBuffer == 0 || startIndex >= endIndex ) return;

                unsigned col = startCol;
                unsigned row = startRow;

                for (u32 i = startIndex; i < endIndex; i++)
                    {
                    const char ch = pSourceBuffer[i];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = startCol; // startCol = indented wrap // 0 = left-edge wrap
                        row++;
                        if (row >= gE_Rows) break;

                        continue;
                        }
                    if (col < gE_Cols && row < gE_Rows) bufferScreenDrawChar (ch, col, row, fgColor );

                    col++;
                    if (col >= gE_Cols)
                        {
                        col = startCol; // startCol = indented wrap // 0 = left-edge wrap
                        row++;
                        if (row >= gE_Rows) break;
                        }
                    }
}

void            CKernel::bufferScreenGetGrid      (   unsigned& cols, unsigned& rows)
{
                cols = gE_Cols;
                rows = gE_Rows;
}

//  eof


