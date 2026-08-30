//----------------------------------------------------------------------------------------------------------------------------------------------------
// MACROS / DEFINITIONS
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define MY_SPI0_BASE        (ARM_IO_BASE + 0x204000)                        // SPI

#define ARM_SPI_CS          (m_SPIBaseAddress + 0x00)
#define ARM_SPI_FIFO        (m_SPIBaseAddress + 0x04)
#define ARM_SPI_CLK         (m_SPIBaseAddress + 0x08)
#define ARM_SPI_DLEN        (m_SPIBaseAddress + 0x0C)

#define CS_TXD              (1 << 18)
#define CS_RXD              (1 << 17)
#define CS_DONE             (1 << 16)
#define CS_TA               (1 << 7)
#define CS_CLEAR_RX         (1 << 5)
#define CS_CLEAR_TX         (1 << 4)

#define CS_CPOL_SHIFT       3
#define CS_CPHA_SHIFT       2

#define CS_CS               (3 << 0)
#define CS_CS_SHIFT         0
#define CS_NONE             3

#define MISO                9                                               // GPIO
#define MOSI                10
#define SCLK                11
#define CE0                 8
#define CE1                 7

#define MY_SMI_BASE                    (ARM_IO_BASE + 0x600000)             // SMI

#define ARM_SMI_CS                     (MY_SMI_BASE + 0x00)
#define ARM_SMI_L                      (MY_SMI_BASE + 0x04)
#define ARM_SMI_A                      (MY_SMI_BASE + 0x08)
#define ARM_SMI_D                      (MY_SMI_BASE + 0x0C)
#define ARM_SMI_DSR0                   (MY_SMI_BASE + 0x10)
#define ARM_SMI_DSW0                   (MY_SMI_BASE + 0x14)
#define ARM_SMI_DMC                    (MY_SMI_BASE + 0x30)
#define ARM_SMI_DCS                    (MY_SMI_BASE + 0x34)
#define ARM_SMI_DCA                    (MY_SMI_BASE + 0x38)

#define SMI_CS_ENABLE                  (1 << 0)
#define SMI_CS_START                   (1 << 3)
#define SMI_CS_CLEAR                   (1 << 4)
#define SMI_CS_WRITE                   (1 << 5)
#define SMI_CS_SETERR                  (1 << 13)
#define SMI_CS_PXLDAT                  (1 << 14)

#define SMI_WSTROBE_SHIFT              0
#define SMI_WPACE_SHIFT                8
#define SMI_WHOLD_SHIFT                16
#define SMI_WSETUP_SHIFT               24
#define SMI_WWIDTH_SHIFT               30

#define SMI_DMC_REQW_SHIFT             0
#define SMI_DMC_REQR_SHIFT             6
#define SMI_DMC_PANICW_SHIFT           12
#define SMI_DMC_PANICR_SHIFT           18
#define SMI_DMC_DMAEN                  (1 << 28)

#define SMI_DMA_REQUEST_THRESH         2
#define SMI_DMA_PANIC_LEVEL            8

#define CM_SMICTL_BUSY                 (1 << 7)
#define CM_SMICTL_KILL                 (1 << 5)
#define CM_SMICTL_ENAB                 (1 << 4)

#define CM_SMIDIV_DIVI_SHIFT           12

#define GPIO_TO_SD_LINE(pin)           ((pin) - 8)
#define SD_LINE_TO_MASK(line)          (1 << (line))

#define SMI_WIDTH_16                   1
// MPC 3008 
#define SPI_CHIP_SELECT     0                                                   // MPC 3008 

#define GPIO_PULL_OFF       0
#define GPIO_PULL_DOWN      1
#define GPIO_PULL_UP        2

#define LOW                 0
#define HIGH                1

#define NEOPIXEL_SMI_NS                10                                       // WS2812 timing
#define NEOPIXEL_SMI_SETUP             10
#define NEOPIXEL_SMI_STROBE            20
#define NEOPIXEL_SMI_HOLD              10
#define NEOPIXEL_SMI_PACE              0

#define LED_NBITS                      24                                       // WS2812 buffer layout
#define BIT_NPULSES                    3
#define LED_PREBITS                    4
#define LED_POSTBITS                   4

#define LED_DLEN                       (LED_NBITS * BIT_NPULSES)
#define LED_TX_OSET(n)                 (LED_PREBITS + (LED_DLEN * (n)))
#define TX_BUFF_LEN(n)                 (LED_TX_OSET(n) + LED_POSTBITS)

#define TXDATA_T                       u16

#define PACKED             __attribute__ ((packed))                             // watchdog / macros.h
#define MAXALIGN           __attribute__ ((aligned))
#define ALIGN(n)           __attribute__ ((aligned (n)))
#define NORETURN           __attribute__ ((noreturn))

#ifndef __clang__
#define NOOPT              __attribute__ ((optimize (0)))
#define STDOPT             __attribute__ ((optimize (2)))
#define MAXOPT             __attribute__ ((optimize (3)))
#else
#define NOOPT
#define STDOPT
#define MAXOPT
#endif

#define WEAK               __attribute__ ((weak))

#define likely(exp)        __builtin_expect (!!(exp), 1)
#define unlikely(exp)      __builtin_expect (!!(exp), 0)

#define BIT(n)             (1U << (n))

#define IS_POWEROF_2(num)  ((num) != 0 && (((num) & ((num) - 1)) == 0))

#define BE(value)          ((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

//----------------------------------------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATIONS
//----------------------------------------------------------------------------------------------------------------------------------------------------
class CKernel
{
public:
                        CKernel (void);
                       ~CKernel (void);
private:

    // MMIO

            u32         my_read32(uintptr nAddress);
            void        my_write32(uintptr nAddress, u32 nValue);


    // GPIO

            void        my_GPIO_SetPullOff(unsigned nPin);
            void        my_GPIO_SetPull(unsigned nPin, unsigned nPullMode);

            void        my_GPIO_SetAlt0(unsigned nPin);
            void        my_GPIO_SetAlt0(unsigned nPin, unsigned nPullMode);
            void        my_GPIO_SetAlt(unsigned nPin, unsigned nAltMode, unsigned nPullMode);

            void        my_GPIO_Write(unsigned nPin, unsigned nValue);
            unsigned    my_GPIO_Read(unsigned nPin);


    // watchdog

            void        my_watchdog_Start(unsigned nTimeoutSeconds);


public:

    // SPI

            boolean     my_SPI_init(void);

            int         my_WriteRead(unsigned nChipSelect,
                     const void *pWriteBuffer,
                     void *pReadBuffer,
                     unsigned nCount);

    boolean        my_SMI_Init(unsigned gpioPin, unsigned ledCount);
    void           my_SMI_SetupTiming(void);
    void           my_SMI_SetupDMA(void);

    void           my_WS2812_SetLED(unsigned index, u8 red, u8 green, u8 blue);
    void           my_WS2812_Update(void);

    // MPC 3008 

    inline  int         ReadMCP3008Raw(unsigned channel);

    // framebuffer

    boolean             bufferToScreenInit(void);

            void        bufferToScreenPlot(unsigned x,
                            unsigned y,
                            u32 color);

            void        bufferToScreenDrawChar(char ch,
                                unsigned charCol,
                                unsigned charRow,
                                u32 fgColor,
                                u32 bgColor);

            void        bufferToScreenClear(u32 bgColor);

            void        bufferToScreenDrawBuffer(const char *pSourceBuffer,
                                  u32 startIndex,
                                  u32 endIndex,
                                  unsigned startCol,
                                  unsigned startRow,
                                  u32 fgColor,
                                  u32 bgColor);

            void        bufferToScreenGetGrid(unsigned& cols,
                               unsigned& rows);


    // routing

            void        my_set_pot_routing(bool adc_pot_routing);
//----------------------------------------------------------------------------------------------------------------------------------------------------
// MEMBERS
//----------------------------------------------------------------------------------------------------------------------------------------------------

// SPI

private:
    uintptr m_SPIBaseAddress = 0;
    boolean m_SPIValid = 0;

// framebuffer

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

// watchdog

static const unsigned WatchdogMaxTimeoutSeconds = 15;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// MEMBERS

//----------------------------------------------------------------------------------------------------------------------------------------------------
// CONTEXT / CODE INTENT
//----------------------------------------------------------------------------------------------------------------------------------------------------
//
// This code is not a new subsystem.
//
// It is a replacement layer for already existing Circle functionality,
// moved into CKernel as local code.
//
// The goal is not “more features”, but removing abstraction layers that
// hide hardware behavior and make debugging harder.
//
// -------------------------------------------------------------------------------------------------
// SPI
//
// Previously:
//
//     m_SPIMaster(...)
//     m_SPIMaster.Initialize()
//
// and older chip-select handling through:
//
//     m_ChipSelectPin.Write(...)
//
//
//
// Now:
//
//     my_SPI_init()
//     my_WriteRead()
//
// and direct GPIO controlled chip select:
//
//     my_GPIO_Write(...)
//
//
// Intention:
//
// exact control over SPI transfers,
// exact control over CE handling,
// no hidden behavior from CSPIMaster,
// easier debugging when MCP3008 / ADC / timing problems happen.
//
// -------------------------------------------------------------------------------------------------
// ADC / MCP3008
//
// This reads one raw ADC channel value through SPI.
//
// It sends the standard 3-byte command sequence and extracts the
// 10-bit result from the returned SPI bytes.
//
// Used for raw analog input before scaling, smoothing, band detection,
// or routing logic.
//
// -------------------------------------------------------------------------------------------------
// GPIO
//
// Previously:
//
//     CGPIOPin
//
// Now:
//
// use in constructor
//
//     : m_ChipSelectPin(CS_PIN, GPIOModeOutput),   // old
//
// use in init
//
//     if (bOK)
//     {
//         my_GPIO_SetOutput(CS_PIN);               // new
//         my_GPIO_SetPull(CS_PIN, GPIO_PULL_OFF);  // new
//         my_GPIO_Write(CS_PIN, LOW);              // new
//     }
//
// example use
//
//     void CKernel::my_set_pot_routing(bool adc_pot_routing)
//     {
//         m_ChipSelectPin.Write(adc_pot_routing);  // old
//
//         my_GPIO_Write(CS_PIN, adc_pot_routing);  // new
//     }
//
// local register access:
//
//     my_read32()
//     my_write32()
//
// and local helpers:
//
//     my_GPIO_SetPull()
//     my_GPIO_SetAlt()
//     my_GPIO_Write()
//     my_GPIO_Read()
//
//
// Intention:
//
// explicit control over pull state,
// explicit ALT mode setup (ALT0 / ALT1),
// manual pin state control,
// no dependency on CGPIOPin behavior.
//
// -------------------------------------------------------------------------------------------------
// SMI + WS2812
//
// Previously:
//
//     CWS2812OverSMI
//     CSMIMaster
//
// constructor:
//
//     : m_NeoPixels(SD_LINES_MASK, LED_COUNT)      // old
//
// internal hidden handling:
//
//     SetupTiming()
//     SetupDMA()
//     WriteDMA()
//
// and GPIO handled through:
//
//     CGPIOPin(..., GPIOModeAlternateFunction1)
//
//
// Now:
//
// constructor:
//
//     : m_SMITxDMA(DMA_CHANNEL_LITE)               // new
//
// init:
//
//     my_SMI_Init(gpioPin, ledCount)
//
// local control:
//
//     my_SMI_SetupTiming()
//     my_SMI_SetupDMA()
//     my_WS2812_SetLED()
//     my_WS2812_Update()
//
// GPIO handled through:
//
//     my_GPIO_SetAlt(gpioPin, GPIO_ALT1, GPIO_PULL_OFF)
//
//
// Intention:
//
// exact control over SMI timing,
// exact control over DMA-triggered LED transfers,
// exact control over WS2812 pulse generation,
// no hidden behavior from CWS2812OverSMI / CSMIMaster,
// easier debugging of DMA / timing / LED signal problems.
//
// -------------------------------------------------------------------------------------------------
// framebuffer output
//
// use in the constructor:
//
//     CKernel::CKernel(void)
//     : gE_FrameBuffer(
//           m_Options.GetWidth(),
//           m_Options.GetHeight(),
//           32,
//           0,
//           TRUE
//       )
//
// Previously:
//
//     higher-level screen helpers
//
// Now:
//
//     direct:
//
//     CBcmFrameBuffer
//     CCharGenerator
//
// through:
//
//     bufferToScreenPlot()
//     bufferToScreenDrawChar()
//     bufferToScreenDrawBuffer()
//
//
// Intention:
//
// lightweight debug output directly to framebuffer,
// without depending on CScreen / CString paths.
//
// This is mainly for runtime inspection and debugging.
//
// -------------------------------------------------------------------------------------------------
// watchdog
//
// Previously:
//
// wrapper class approach
//
// Now:
//
//     my_watchdog_Start()
//
//
// Intention:
//
// direct reboot path using PM_WDOG / PM_RSTC,
// simple permanent watchdog usage,
// no unnecessary wrapper dependency.
//
// -------------------------------------------------------------------------------------------------
//
// Old code blocks remain in comments only as migration reference.
//
// Final intention:
//
// keep only deterministic local code,
// remove wrapper-based legacy paths after validation.
//
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
//---------------------------------------------------------------------------------------------------------------------
// my SMI
//---------------------------------------------------------------------------------------------------------------------
boolean CKernel::my_SMI_Init(unsigned gpioPin, unsigned ledCount)
{
    // only GPIO8..GPIO25 are valid SMI SD lines
    if (gpioPin < 8 || gpioPin > 25)
    {
        return FALSE;
    }

    if (ledCount == 0)
    {
        return FALSE;
    }

    m_SMIGpioPin   = gpioPin;
    m_SMISDLine    = GPIO_TO_SD_LINE(gpioPin);
    m_SMISDMask    = SD_LINE_TO_MASK(m_SMISDLine);
    m_LEDCount     = ledCount;
    m_BufferLength = TX_BUFF_LEN(ledCount);

    // switch GPIO to SMI ALT1
    my_GPIO_SetAlt(gpioPin, GPIO_ALT1, GPIO_PULL_OFF);

    // original Circle allocation style
    m_pBuffer = new TXDATA_T[m_BufferLength];

    if (m_pBuffer == 0)
    {
        return FALSE;
    }

    memset(m_pBuffer, 0, m_BufferLength * sizeof(TXDATA_T));

    my_SMI_SetupTiming();
    my_SMI_SetupDMA();

    m_SMIValid = TRUE;

    return TRUE;
}

void CKernel::my_SMI_SetupTiming(void)
{
    u32 divi = NEOPIXEL_SMI_NS / 2;

    PeripheralEntry();

    // reset SMI registers
    my_write32(ARM_SMI_CS,   0);
    my_write32(ARM_SMI_L,    0);
    my_write32(ARM_SMI_A,    0);
    my_write32(ARM_SMI_DSR0, 0);
    my_write32(ARM_SMI_DSW0, 0);
    my_write32(ARM_SMI_DCS,  0);
    my_write32(ARM_SMI_DCA,  0);

    PeripheralExit();

    PeripheralEntry();

    // only reconfigure SMI clock if divider is not already correct
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
          (SMI_WIDTH_16        << SMI_WWIDTH_SHIFT)
        | (NEOPIXEL_SMI_SETUP  << SMI_WSETUP_SHIFT)
        | (NEOPIXEL_SMI_STROBE << SMI_WSTROBE_SHIFT)
        | (NEOPIXEL_SMI_HOLD   << SMI_WHOLD_SHIFT)
        | (NEOPIXEL_SMI_PACE   << SMI_WPACE_SHIFT);

    my_write32(ARM_SMI_DSW0, timing);
    my_write32(ARM_SMI_DSR0, timing);

    PeripheralExit();
}

void CKernel::my_SMI_SetupDMA(void)
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
void CKernel::my_WS2812_SetLED(unsigned index, u8 red, u8 green, u8 blue)
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

void CKernel::my_WS2812_Update(void)
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
inline int CKernel::ReadMCP3008Raw(unsigned channel)
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

