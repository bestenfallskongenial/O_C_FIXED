//----------------------------------------------------------------------------------------------------------------------------------------------------
//  from memio.h
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32 CKernel::my_read32(uintptr nAddress)
{
    return *(volatile u32 *) nAddress;
}

void CKernel::my_write32(uintptr nAddress, u32 nValue)
{
    *(volatile u32 *) nAddress = nValue;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//  my GPIOPIN implementation
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::my_GPIO_SetPull(unsigned nPin, unsigned nPullMode)
{
    uintptr nClkReg = ARM_GPIO_GPPUDCLK0 + ((nPin / 32) * 4);
    u32 nMask = 1 << (nPin % 32);

    my_write32(ARM_GPIO_GPPUD, nPullMode);

    CTimer::SimpleusDelay(5);

    my_write32(nClkReg, nMask);

    CTimer::SimpleusDelay(5);

    my_write32(ARM_GPIO_GPPUD, 0);
    my_write32(nClkReg, 0);
}

void CKernel::my_GPIO_SetAlt(unsigned nPin, unsigned nAltMode, unsigned nPullMode)
{
    my_GPIO_SetPull(nPin, nPullMode);

    uintptr nSelReg = ARM_GPIO_GPFSEL0 + (nPin / 10) * 4;
    unsigned nShift = (nPin % 10) * 3;

    u32 nValue = my_read32(nSelReg);

    nValue &= ~(7 << nShift);
    nValue |=  (nAltMode << nShift);

    my_write32(nSelReg, nValue);
}

void CKernel::my_GPIO_Write(unsigned nPin, unsigned nValue)
{
    uintptr nReg =
        (nValue ? ARM_GPIO_GPSET0 : ARM_GPIO_GPCLR0)             // dependency: ARM_GPIO_GPSET0 / ARM_GPIO_GPCLR0 from <circle/bcm2835.h>
        + ((nPin / 32) * 4);

    u32 nMask = 1 << (nPin % 32);                                // dependency: <circle/types.h> u32

    my_write32(nReg, nMask);                                     // dependency: my_write32()
}

unsigned CKernel::my_GPIO_Read(unsigned nPin)
{
    uintptr nReg =
        ARM_GPIO_GPLEV0 + ((nPin / 32) * 4);                     // dependency: ARM_GPIO_GPLEV0 from <circle/bcm2835.h>

    u32 nMask = 1 << (nPin % 32);                                // dependency: <circle/types.h> u32

    return (my_read32(nReg) & nMask) ? HIGH : LOW;               // dependency: my_read32()
                                                               // dependency: HIGH / LOW
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//  my watchdog implementation
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::my_watchdog_Start(unsigned nTimeoutSeconds)
{
    if (nTimeoutSeconds > WatchdogMaxTimeoutSeconds)
    {
        nTimeoutSeconds = WatchdogMaxTimeoutSeconds;
    }

    my_write32(
        ARM_PM_WDOG,
        ARM_PM_PASSWD
        | ((nTimeoutSeconds << 16) & ARM_PM_WDOG_TIME)
    );

    my_write32(
        ARM_PM_RSTC,
        ARM_PM_PASSWD
        | ARM_PM_RSTC_REBOOT
        | (my_read32(ARM_PM_RSTC) & ARM_PM_RSTC_CLEAR)
    );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//  my spi
//----------------------------------------------------------------------------------------------------------------------------------------------------
boolean CKernel::my_SPI_init(void)
{
    m_SPIBaseAddress = MY_SPI0_BASE;                             // dependency: MY_SPI0_BASE

    if (m_SPIBaseAddress == 0)
    {
        return FALSE;                                            // dependency: <circle/types.h> FALSE
    }
    // SPI (ALT0)
    my_GPIO_SetAlt(MISO, 4, GPIO_PULL_OFF);
    my_GPIO_SetAlt(MOSI, 4, GPIO_PULL_OFF);
    my_GPIO_SetAlt(SCLK, 4, GPIO_PULL_OFF);
    my_GPIO_SetAlt(CE0,  4, GPIO_PULL_OFF);
    my_GPIO_SetAlt(CE1,  4, GPIO_PULL_OFF);

    unsigned nCoreClockRate = CMachineInfo::Get()->GetClockRate(CLOCK_ID_CORE);        // dependency: <circle/machineinfo.h> / CLOCK_ID_CORE from <circle/bcm2835.h>

    if (nCoreClockRate == 0)
    {
        return FALSE;                                            // dependency: <circle/types.h> FALSE
    }

    if (SPI_CLOCK_SPEED < 4000 || SPI_CLOCK_SPEED > 125000000)   // dependency: existing project macro SPI_CLOCK_SPEED
    {
        return FALSE;                                            // dependency: <circle/types.h> FALSE
    }

    PeripheralEntry();                                           // dependency: <circle/synchronize.h>

    my_write32(ARM_SPI_CLK,                                      // dependency: ARM_SPI_CLK
               nCoreClockRate / SPI_CLOCK_SPEED);                // dependency: my_write32()

    my_write32(ARM_SPI_CS,                                       // dependency: ARM_SPI_CS
                 (0 << CS_CPOL_SHIFT)                            // dependency: CS_CPOL_SHIFT
               | (0 << CS_CPHA_SHIFT));                          // dependency: CS_CPHA_SHIFT

    PeripheralExit();                                            // dependency: <circle/synchronize.h>

    m_SPIValid = TRUE;                                           // dependency: <circle/types.h> TRUE

    return TRUE;                                                 // dependency: <circle/types.h> TRUE
}

int CKernel::my_WriteRead(unsigned nChipSelect,
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

    const u8 *pWritePtr = (const u8 *) pWriteBuffer;             // dependency: <circle/types.h> u8
    u8 *pReadPtr = (u8 *) pReadBuffer;                           // dependency: <circle/types.h> u8


//  m_SpinLock.Acquire ();

    PeripheralEntry();                                           // dependency: <circle/synchronize.h>

    my_write32(ARM_SPI_DLEN, nCount);                            // dependency: ARM_SPI_DLEN
                                                                  // dependency: my_write32()

    my_write32(ARM_SPI_CS,                                       // dependency: ARM_SPI_CS
                 (my_read32(ARM_SPI_CS) & ~CS_CS)                // dependency: my_read32()
                                                                  // dependency: ARM_SPI_CS
                                                                  // dependency: CS_CS
               | (nChipSelect << CS_CS_SHIFT)                    // dependency: CS_CS_SHIFT
               | CS_CLEAR_RX                                     // dependency: CS_CLEAR_RX
               | CS_CLEAR_TX                                     // dependency: CS_CLEAR_TX
               | CS_TA);                                         // dependency: CS_TA

    unsigned nWriteCount = 0;
    unsigned nReadCount  = 0;

    while (nWriteCount < nCount || nReadCount < nCount)
    {
        while (nWriteCount < nCount &&
              (my_read32(ARM_SPI_CS) & CS_TXD))                  // dependency: my_read32()
                                                                  // dependency: ARM_SPI_CS
                                                                  // dependency: CS_TXD
        {
            u32 nData = 0;                                       // dependency: <circle/types.h> u32

            if (pWritePtr != 0)
            {
                nData = *pWritePtr++;
            }

            my_write32(ARM_SPI_FIFO, nData);                     // dependency: ARM_SPI_FIFO
                                                                  // dependency: my_write32()

            nWriteCount++;
        }

        while (nReadCount < nCount &&
              (my_read32(ARM_SPI_CS) & CS_RXD))                  // dependency: my_read32()
                                                                  // dependency: ARM_SPI_CS
                                                                  // dependency: CS_RXD
        {
            u32 nData = my_read32(ARM_SPI_FIFO);                 // dependency: <circle/types.h> u32
                                                                  // dependency: my_read32()
                                                                  // dependency: ARM_SPI_FIFO

            if (pReadPtr != 0)
            {
                *pReadPtr++ = (u8) nData;                        // dependency: <circle/types.h> u8
            }

            nReadCount++;
        }
    }

    while (!(my_read32(ARM_SPI_CS) & CS_DONE))                   // dependency: my_read32()
                                                                  // dependency: ARM_SPI_CS
                                                                  // dependency: CS_DONE
    {
        while (my_read32(ARM_SPI_CS) & CS_RXD)                   // dependency: my_read32()
                                                                  // dependency: ARM_SPI_CS
                                                                  // dependency: CS_RXD
        {
            my_read32(ARM_SPI_FIFO);                             // dependency: my_read32()
                                                                  // dependency: ARM_SPI_FIFO
        }
    }

    my_write32(ARM_SPI_CS,                                       // dependency: ARM_SPI_CS
               my_read32(ARM_SPI_CS) & ~CS_TA);                  // dependency: my_read32()
                                                                  // dependency: CS_TA

    PeripheralExit();                                            // dependency: <circle/synchronize.h>

// 	m_SpinLock.Release ();

    return (int) nCount;
}
//---------------------------------------------------------------------------------------------------------------------
// my SMI
//---------------------------------------------------------------------------------------------------------------------
boolean CKernel::my_SMI_Init(unsigned gpioPin)
{
    // only GPIO8..GPIO25 are valid SMI SD lines
    if (gpioPin < 8 || gpioPin > 25)
    {
        return FALSE;
    }

    m_SMIGpioPin = gpioPin;

    // GPIO8 -> SD0 -> (1 << 0)
    // GPIO9 -> SD1 -> (1 << 1)
    m_SMISDMask = (1 << (gpioPin - 8));

    // switch GPIO to SMI ALT1
    my_GPIO_SetAlt(gpioPin, GPIO_ALT1, GPIO_PULL_OFF);

    m_SMIValid = TRUE;

    return TRUE;
}

void CKernel::my_SMI_SetupTiming(unsigned width,
                                 unsigned cycle_ns,
                                 unsigned setup,
                                 unsigned strobe,
                                 unsigned hold,
                                 unsigned pace)
{
    u32 divi = cycle_ns / 2;

    PeripheralEntry();

    my_write32(ARM_SMI_CS,   0);
    my_write32(ARM_SMI_L,    0);
    my_write32(ARM_SMI_A,    0);
    my_write32(ARM_SMI_DSR0, 0);
    my_write32(ARM_SMI_DSW0, 0);
    my_write32(ARM_SMI_DCS,  0);
    my_write32(ARM_SMI_DCA,  0);

    PeripheralExit();

    PeripheralEntry();

    if (my_read32(ARM_CM_SMICTL) != (divi << CM_SMIDIV_DIVI_SHIFT))
    {
        my_write32(ARM_CM_SMICTL, ARM_CM_PASSWD | CM_SMICTL_KILL);
        CTimer::Get()->usDelay(10);

        while (my_read32(ARM_CM_SMICTL) & CM_SMICTL_BUSY)
        {
        }

        CTimer::Get()->usDelay(10);

        my_write32(
            ARM_CM_SMIDIV,
            ARM_CM_PASSWD | (divi << CM_SMIDIV_DIVI_SHIFT)
        );

        CTimer::Get()->usDelay(10);

        my_write32(
            ARM_CM_SMICTL,
            ARM_CM_PASSWD | 6 | CM_SMICTL_ENAB
        );

        CTimer::Get()->usDelay(10);

        while ((my_read32(ARM_CM_SMICTL) & CM_SMICTL_BUSY) == 0)
        {
        }

        CTimer::Get()->usDelay(100);
    }

    PeripheralExit();

    PeripheralEntry();

    u32 timing =
          (width   << SMI_WWIDTH_SHIFT)
        | (setup   << SMI_WSETUP_SHIFT)
        | (strobe  << SMI_WSTROBE_SHIFT)
        | (hold    << SMI_WHOLD_SHIFT)
        | (pace    << SMI_WPACE_SHIFT);

    my_write32(ARM_SMI_DSW0, timing);
    my_write32(ARM_SMI_DSR0, timing);

    PeripheralExit();
}

void            CKernel::my_SMI_SetupDMA(void)
{
                PeripheralEntry();

                // configure SMI DMA request levels
                my_write32(
                    ARM_SMI_DMC,
                    (SMI_DMA_REQUEST_THRESH << SMI_DMC_REQW_SHIFT)
                    | (SMI_DMA_REQUEST_THRESH << SMI_DMC_REQR_SHIFT)
                    | (SMI_DMA_PANIC_LEVEL    << SMI_DMC_PANICW_SHIFT)
                    | (SMI_DMA_PANIC_LEVEL    << SMI_DMC_PANICR_SHIFT)
                    | SMI_DMC_DMAEN
                );

                // enable SMI + clear FIFO + pack 16-bit data into 32-bit words
                my_write32(
                    ARM_SMI_CS,
                    SMI_CS_ENABLE
                    | SMI_CS_CLEAR
                    | SMI_CS_PXLDAT
                );

                my_write32(
                    ARM_SMI_L,
                    m_BufferLength * sizeof(TXDATA_T)
                );

                my_write32(
                    ARM_SMI_CS,
                    my_read32(ARM_SMI_CS) | SMI_CS_WRITE
                );

                PeripheralExit();
}
//---------------------------------------------------------------------------------------------------------------------
// my WS2812
//---------------------------------------------------------------------------------------------------------------------
boolean CKernel::my_WS2812_Init(unsigned ledCount)
{
    if (!m_SMIValid)
    {
        return FALSE;
    }

    if (ledCount == 0)
    {
        return FALSE;
    }

    m_LEDCount = ledCount;

    m_BufferLength = TX_BUFF_LEN(ledCount);

    m_pBuffer = new TXDATA_T[m_BufferLength];

    if (m_pBuffer == 0)
    {
        return FALSE;
    }

    memset(m_pBuffer, 0, m_BufferLength * sizeof(TXDATA_T));

    // WS2812-specific timing belongs here
    my_SMI_SetupTiming(
        SMI_WIDTH_16,
        NEOPIXEL_SMI_NS,
        NEOPIXEL_SMI_SETUP,
        NEOPIXEL_SMI_STROBE,
        NEOPIXEL_SMI_HOLD,
        NEOPIXEL_SMI_PACE
    );

    my_SMI_SetupDMA();

    return TRUE;
}

void            CKernel::my_WS2812_SetLED(unsigned index, u8 red, u8 green, u8 blue)
{
    if (!m_SMIValid || index >= m_LEDCount)
    {
        return;
    }

    TXDATA_T* tx = &m_pBuffer[LED_TX_OSET(index)];

    // WS2812 byte order is GRB
    unsigned grb =
          ((unsigned) green << 16)
        | ((unsigned) red   << 8)
        | ((unsigned) blue  << 0);

    for (unsigned mask = (1 << 23); mask > 0; mask >>= 1)
    {
        // first pulse: all lines high
        tx[0] = 0xFFFF;

        // second pulse: selected SMI line remains high only for bit 1
        if (grb & mask)
        {
            tx[1] = m_SMISDMask;
        }
        else
        {
            tx[1] = 0;
        }

        // third pulse: all lines low
        tx[2] = 0;

        tx += BIT_NPULSES;
    }
}

void            CKernel::my_WS2812_Update(void)
{
    if (!m_SMIValid)
    {
        return;
    }

    // DMA transfers prepared waveform buffer into SMI FIFO
    m_SMITxDMA.SetupIOWrite(
        ARM_SMI_D,
        m_pBuffer,
        m_BufferLength * sizeof(TXDATA_T),
        DREQSourceSMI
    );

    m_SMITxDMA.Start();

    PeripheralEntry();

    // trigger SMI transfer
    my_write32(
        ARM_SMI_CS,
        my_read32(ARM_SMI_CS) | SMI_CS_START
    );

    PeripheralExit();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//  my MCP3008
//----------------------------------------------------------------------------------------------------------------------------------------------------
inline int      CKernel::ReadMCP3008Raw(unsigned channel)
{
    u8 tx[3] = { 0x01, (u8)((0x08 | channel) << 4), 0x00 };
    u8 rx[3];

    if (my_WriteRead(SPI_CHIP_SELECT, tx, rx, 3) != 3)
        return -1;

    return ((rx[1] & 0x03) << 8) | rx[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//  my "text-to-screen"
//----------------------------------------------------------------------------------------------------------------------------------------------------
boolean         CKernel::bufferToScreenInit                              (   void )
{
                if (!gE_FrameBuffer.Initialize ())
                    {
                    return FALSE;
                    }

                gE_PixelBuffer  = (u32 *) gE_FrameBuffer.GetBuffer ();
                gE_PitchBytes   = gE_FrameBuffer.GetPitch ();
                gE_ScreenWidth  = gE_FrameBuffer.GetWidth ();
                gE_ScreenHeight = gE_FrameBuffer.GetHeight ();
                gE_CharWidth    = gE_CharGenerator.GetCharWidth ();
                gE_CharHeight   = gE_CharGenerator.GetCharHeight ();

                if (gE_PixelBuffer == 0 || gE_CharWidth == 0 || gE_CharHeight == 0)     // failsave for missing framebuffer/chargenerator?
                    {
                    return FALSE;
                    }

                gE_Cols = gE_ScreenWidth / gE_CharWidth;
                gE_Rows = gE_ScreenHeight / gE_CharHeight;

                if (gE_Cols == 0 || gE_Rows == 0)
                    {
                    return FALSE;
                    }

                return TRUE;
}

void            CKernel::bufferToScreenPlot                          (   unsigned x, unsigned y, u32 color )   // why the fuck static?
{
                gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}

void            CKernel::bufferToScreenDrawChar                     (   char ch,
                                                                        unsigned charCol,
                                                                        unsigned charRow,
                                                                        u32 fgColor,
                                                                        u32 bgColor )
{
                const unsigned px = charCol * gE_CharWidth;
                const unsigned py = charRow * gE_CharHeight;

                for (unsigned y = 0; y < gE_CharHeight; y++)
                    {
                    for (unsigned x = 0; x < gE_CharWidth; x++)
                        {
                        bufferToScreenPlot (px + x, py + y,
                                                gE_CharGenerator.GetPixel (ch, x, y) ? fgColor : bgColor );
                        }
                    }
}

void            CKernel::bufferToScreenClear                         (   u32 bgColor)
{
                const unsigned pitch32 = gE_PitchBytes >> 2;

                for (unsigned y = 0; y < gE_ScreenHeight; y++)
                    {
                    for (unsigned x = 0; x < gE_ScreenWidth; x++)
                        {
                        gE_PixelBuffer[y * pitch32 + x] = bgColor;
                        }
                    }
}

void            CKernel::bufferToScreenDrawBuffer        (   const char *pSourceBuffer,
                                                                        u32 startIndex,
                                                                        u32 endIndex,
                                                                        unsigned startCol,
                                                                        unsigned startRow,
                                                                        u32 fgColor,
                                                                        u32 bgColor)
{
                if (startCol >= gE_Cols || startRow >= gE_Rows)
                    {
                    return;
                    }
                unsigned col = startCol;
                unsigned row = startRow;

                for (u32 i = startIndex; i < endIndex; i++)
                    {
                    const char ch = pSourceBuffer[i];

                    if (ch == '\0')
                        {
                        break;
                        }
                    if (ch == '\n')
                        {
                        col = startCol;
                        row++;
                        if (row >= gE_Rows)
                            {
                            break;
                            }
                        continue;
                        }
                    if (col < gE_Cols && row < gE_Rows)
                        {
                        bufferToScreenDrawChar (ch, col, row, fgColor, bgColor);
                        }
                    col++;
                    if (col >= gE_Cols)
                        {
                        col = startCol;
                        row++;
                        if (row >= gE_Rows)
                            {
                            break;
                            }
                        }
                    }
}

void        CKernel::bufferToScreenGetGrid                         (   unsigned& cols, unsigned& rows)
{
                cols = gE_Cols;
                rows = gE_Rows;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//  eof
//----------------------------------------------------------------------------------------------------------------------------------------------------

