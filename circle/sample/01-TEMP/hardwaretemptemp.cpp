// kernel.h additions

private:
    uintptr m_SPIBaseAddress = 0;                         // dependency: <circle/types.h> uintptr
    boolean m_SPIValid = 0;                               // dependency: <circle/types.h> boolean / TRUE / FALSE

    u32  my_read32(uintptr nAddress);                 // dependency: local replacement for <circle/memio.h> read32()
    void my_write32(uintptr nAddress, u32 nValue);    // dependency: local replacement for <circle/memio.h> write32()

    void my_GPIO_SetPull(unsigned nPin, unsigned nPullMode);           // dependency: local replacement for CGPIOPin::SetPullMode(GPIOPullModeOff)
    void my_GPIO_SetAlt0(unsigned nPin);              // dependency: local replacement for CGPIOPin::SetMode(GPIOModeAlternateFunction0)
    void my_GPIO_Write(unsigned nPin, unsigned nValue); // dependency: local replacement for CGPIOPin::Write()
    unsigned my_GPIO_Read(unsigned nPin);             // dependency: local replacement for CGPIOPin::Read()

public:
    boolean my_SPI_init(void);

    int my_WriteRead(unsigned nChipSelect,
                  const void *pWriteBuffer,
                  void *pReadBuffer,
                  unsigned nCount);

// kernel.cpp additions

#include <circle/bcm2835.h>       // dependency: ARM_IO_BASE
                                  // dependency: ARM_GPIO_GPFSEL0
                                  // dependency: ARM_GPIO_GPPUD
                                  // dependency: ARM_GPIO_GPPUDCLK0
                                  // dependency: ARM_GPIO_GPSET0
                                  // dependency: ARM_GPIO_GPCLR0
                                  // dependency: ARM_GPIO_GPLEV0
                                  // dependency: CLOCK_ID_CORE

#include <circle/machineinfo.h>   // dependency: CMachineInfo::Get()->GetClockRate()

#include <circle/synchronize.h>   // dependency: PeripheralEntry()
                                  // dependency: PeripheralExit()

#include <circle/timer.h>         // dependency: CTimer::SimpleusDelay()


#define MY_SPI0_BASE        (ARM_IO_BASE + 0x204000)  // dependency: ARM_IO_BASE from <circle/bcm2835.h>

#define ARM_SPI_CS          (m_SPIBaseAddress + 0x00) // dependency: m_SPIBaseAddress
#define ARM_SPI_FIFO        (m_SPIBaseAddress + 0x04) // dependency: m_SPIBaseAddress
#define ARM_SPI_CLK         (m_SPIBaseAddress + 0x08) // dependency: m_SPIBaseAddress
#define ARM_SPI_DLEN        (m_SPIBaseAddress + 0x0C) // dependency: m_SPIBaseAddress

#define CS_TXD              (1 << 18)                 // dependency: SPI CS register bit TXD
#define CS_RXD              (1 << 17)                 // dependency: SPI CS register bit RXD
#define CS_DONE             (1 << 16)                 // dependency: SPI CS register bit DONE
#define CS_TA               (1 << 7)                  // dependency: SPI CS register bit TA
#define CS_CLEAR_RX         (1 << 5)                  // dependency: SPI CS register bit CLEAR_RX
#define CS_CLEAR_TX         (1 << 4)                  // dependency: SPI CS register bit CLEAR_TX
#define CS_CPOL_SHIFT       3                         // dependency: SPI CS register CPOL shift
#define CS_CPHA_SHIFT       2                         // dependency: SPI CS register CPHA shift
#define CS_CS               (3 << 0)                  // dependency: SPI CS register chip-select mask
#define CS_CS_SHIFT         0                         // dependency: SPI CS register chip-select shift
#define CS_NONE             3                         // dependency: SPI no chip-select value

#define MISO                9
#define MOSI                10
#define SCLK                11
#define CE0                 8
#define CE1                 7

#define GPIO_PULL_OFF       0
#define GPIO_PULL_DOWN      1
#define GPIO_PULL_UP        2

#define LOW                 0                         // dependency: GPIO low value
#define HIGH                1                         // dependency: GPIO high value
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
void CKernel::my_GPIO_SetPullOff(unsigned nPin)
{
    uintptr nClkReg = ARM_GPIO_GPPUDCLK0 + ((nPin / 32) * 4); // dependency: ARM_GPIO_GPPUDCLK0 from <circle/bcm2835.h>
    u32 nMask = 1 << (nPin % 32);                             // dependency: <circle/types.h> u32

    my_write32(ARM_GPIO_GPPUD, 0);                             // dependency: ARM_GPIO_GPPUD from <circle/bcm2835.h>
                                                               // dependency: my_write32()

    CTimer::SimpleusDelay(5);                                  // dependency: <circle/timer.h>

    my_write32(nClkReg, nMask);                                // dependency: my_write32()

    CTimer::SimpleusDelay(5);                                  // dependency: <circle/timer.h>

    my_write32(ARM_GPIO_GPPUD, 0);                              // dependency: ARM_GPIO_GPPUD from <circle/bcm2835.h>
                                                               // dependency: my_write32()
    my_write32(nClkReg, 0);                                     // dependency: my_write32()
}
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::my_GPIO_SetAlt0(unsigned nPin)
{
    my_GPIO_SetPullOff(nPin);                                   // dependency: my_GPIO_SetPullOff()

    uintptr nSelReg = ARM_GPIO_GPFSEL0 + (nPin / 10) * 4;       // dependency: ARM_GPIO_GPFSEL0 from <circle/bcm2835.h>
    unsigned nShift = (nPin % 10) * 3;

    u32 nValue = my_read32(nSelReg);                            // dependency: my_read32()

    nValue &= ~(7 << nShift);
    nValue |=  (4 << nShift);                                   // dependency: ALT0 function value for BCM2835 GPIO

    my_write32(nSelReg, nValue);                                // dependency: my_write32()
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::my_GPIO_SetAlt0(unsigned nPin, unsigned nPullMode)
{
    my_GPIO_SetPull(nPin, nPullMode);

    uintptr nSelReg = ARM_GPIO_GPFSEL0 + (nPin / 10) * 4;
    unsigned nShift = (nPin % 10) * 3;

    u32 nValue = my_read32(nSelReg);

    nValue &= ~(7 << nShift);
    nValue |=  (4 << nShift);

    my_write32(nSelReg, nValue);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::my_GPIO_Write(unsigned nPin, unsigned nValue)
{
    uintptr nReg =
        (nValue ? ARM_GPIO_GPSET0 : ARM_GPIO_GPCLR0)             // dependency: ARM_GPIO_GPSET0 / ARM_GPIO_GPCLR0 from <circle/bcm2835.h>
        + ((nPin / 32) * 4);

    u32 nMask = 1 << (nPin % 32);                                // dependency: <circle/types.h> u32

    my_write32(nReg, nMask);                                     // dependency: my_write32()
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
unsigned CKernel::my_GPIO_Read(unsigned nPin)
{
    uintptr nReg =
        ARM_GPIO_GPLEV0 + ((nPin / 32) * 4);                     // dependency: ARM_GPIO_GPLEV0 from <circle/bcm2835.h>

    u32 nMask = 1 << (nPin % 32);                                // dependency: <circle/types.h> u32

    return (my_read32(nReg) & nMask) ? HIGH : LOW;               // dependency: my_read32()
                                                               // dependency: HIGH / LOW
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
boolean CKernel::my_SPI_init(void)
{
    m_SPIBaseAddress = MY_SPI0_BASE;                             // dependency: MY_SPI0_BASE

    if (m_SPIBaseAddress == 0)
    {
        return FALSE;                                            // dependency: <circle/types.h> FALSE
    }
    my_GPIO_SetAlt0(MISO, GPIO_PULL_OFF);
    my_GPIO_SetAlt0(MOSI, GPIO_PULL_OFF);
    my_GPIO_SetAlt0(SCLK, GPIO_PULL_OFF);
    my_GPIO_SetAlt0(CE0,  GPIO_PULL_OFF);
    my_GPIO_SetAlt0(CE1,  GPIO_PULL_OFF);
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
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
inline int CKernel::ReadMCP3008Raw(unsigned channel)
{
    u8 tx[3] = { 0x01, (u8)((0x08 | channel) << 4), 0x00 };
    u8 rx[3];

    if (my_WriteRead(SPI_CHIP_SELECT, tx, rx, 3) != 3)
        return -1;

    return ((rx[1] & 0x03) << 8) | rx[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
int CKernel::my_WriteRead(unsigned nChipSelect,
                       const void *pWriteBuffer,
                       void *pReadBuffer,
                       unsigned nCount)
{
    if (!m_SPIValid)
    {
        return -1;
    }

    if (m_SPIBaseAddress == 0)
    {
        return -1;
    }

    if (pWriteBuffer == 0 && pReadBuffer == 0)
    {
        return -1;
    }

    if (nCount == 0 || nCount > 0xFFFF)
    {
        return -1;
    }

    if (nChipSelect > 1 && nChipSelect != CS_NONE)               // dependency: CS_NONE
    {
        return -1;
    }
/*
if (!m_SPIValid
    || m_SPIBaseAddress == 0
    || (pWriteBuffer == 0 && pReadBuffer == 0)
    || nCount == 0
    || nCount > 0xFFFF
    || (nChipSelect > 1 && nChipSelect != CS_NONE))
{
    return -1;
}
*/
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
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
// is #include of CBcmWatchdog 
/*
macros.h
...
#ifndef _circle_macros_h
#define _circle_macros_h

#define PACKED		__attribute__ ((packed))
#define	MAXALIGN	__attribute__ ((aligned))
#define	ALIGN(n)	__attribute__ ((aligned (n)))
#define NORETURN	__attribute__ ((noreturn))
#ifndef __clang__
#define NOOPT		__attribute__ ((optimize (0)))
#define STDOPT		__attribute__ ((optimize (2)))
#define MAXOPT		__attribute__ ((optimize (3)))
#else
#define NOOPT
#define STDOPT
#define MAXOPT
#endif
#define WEAK		__attribute__ ((weak))

#define likely(exp)	__builtin_expect (!!(exp), 1)
#define unlikely(exp)	__builtin_expect (!!(exp), 0)

#define BIT(n)		(1U << (n))

#define IS_POWEROF_2(num) ((num) != 0 && (((num) & ((num) - 1)) == 0))

// big endian (to be used for constants only)
#define BE(value)	((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

#endif

kernel.h
...
static const unsigned WatchdogMaxTimeoutSeconds = 15;
...
*/
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
// old cs pin select!
//----------------------------------------------------------------------------------------------------------------------------------------------------
// kernel.h
private:
    CGPIOPin m_ChipSelectPin;               // old


// constructor
: m_ChipSelectPin(CS_PIN, GPIOModeOutput),  // old


if (bOK)
{
    m_ChipSelectPin.Write(LOW);             // old

    my_GPIO_SetOutput(CS_PIN);              // new
    my_GPIO_SetPull(CS_PIN, GPIO_PULL_OFF); // new
    my_GPIO_Write(CS_PIN, LOW);             // new
}


void CKernel::my_set_pot_routing(bool adc_pot_routing)
{
    m_ChipSelectPin.Write(adc_pot_routing); // old

    my_GPIO_Write(CS_PIN, adc_pot_routing); // new
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------
// VERSION A
// object member version
// constructor not bloated
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include <circle/koptions.h>
#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>

class CKernel
{
public:
        CKernel (void);
        boolean         bufferToScreenInit (void);

public:
        CBcmFrameBuffer gE_FrameBuffer;
        CCharGenerator  gE_CharGenerator;
        CKernelOptions  m_Options;        
        u32*            gE_PixelBuffer      = 0;
        unsigned        gE_PitchBytes       = 0;
        unsigned        gE_ScreenWidth      = 0;
        unsigned        gE_ScreenHeight     = 0;
        unsigned        gE_CharWidth        = 0;
        unsigned        gE_CharHeight       = 0;
        unsigned        gE_Cols             = 0;
        unsigned        gE_Rows             = 0;

CKernel::CKernel (void)
:       gE_FrameBuffer (
                m_Options.GetWidth (),
                m_Options.GetHeight (),
                32,
                0,
                TRUE
        )
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------- */
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
//              we try to avoid CString, CScreen etc
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::bufferToScreenPlot                          (   unsigned x, unsigned y, u32 color )   // why the fuck static?
{
                gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
void        CKernel::bufferToScreenGetGrid                         (   unsigned& cols, unsigned& rows)
{
                cols = gE_Cols;
                rows = gE_Rows;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------