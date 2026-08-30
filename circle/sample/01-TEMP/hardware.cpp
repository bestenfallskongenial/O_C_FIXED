//----------------------------------------------------------------------------------------------------------------------------------------------------
// my own spi and mcp 3008 driver, still needs InitSPI(SPI_CLOCK_SPEED, 0, 0, SPI_MASTER_DEVICE) in the constructor!
//----------------------------------------------------------------------------------------------------------------------------------------------------
// 

#include <circle/gpiopin.h>
#include <circle/machineinfo.h>
#include <circle/bcm2835.h>
#include <circle/memio.h>
#include <circle/synchronize.h>

boolean CKernel::InitSPI(unsigned nClockSpeed,   // target SPI bus speed in Hz (example: 1000000 = 1 MHz)
                         unsigned CPOL,          // clock polarity: 0 = idle low, 1 = idle high
                         unsigned CPHA,          // clock phase: 0 = sample on first edge, 1 = sample on second edge
                         unsigned nDevice)       // SPI controller selection (Pi Zero: only device 0 = SPI0)
{
    // exact replacement for:
    //
    // m_SPIMaster(nClockSpeed, CPOL, CPHA, nDevice)
    // +
    // m_SPIMaster.Initialize();

    // Raspberry Pi Zero / BCM2835:
    //
    // nDevice = 0 means SPI0 with fixed pins:
    //
    // GPIO9  = MISO  (Master In, Slave Out)
    // GPIO10 = MOSI  (Master Out, Slave In)
    // GPIO11 = SCLK  (SPI clock)
    // GPIO8  = CE0   (Chip Enable 0)
    // GPIO7  = CE1   (Chip Enable 1)

    if (nDevice != 0)
        return FALSE;

    static CGPIOPin miso;
    static CGPIOPin mosi;
    static CGPIOPin sclk;
    static CGPIOPin ce0;
    static CGPIOPin ce1;

    // bind GPIO objects to physical BCM pins
    miso.AssignPin(9);
    mosi.AssignPin(10);
    sclk.AssignPin(11);
    ce0.AssignPin(8);
    ce1.AssignPin(7);

    // ALT0 switches these GPIO pins
    // from normal GPIO mode to SPI hardware mode
    miso.SetMode(GPIOModeAlternateFunction0);
    mosi.SetMode(GPIOModeAlternateFunction0);
    sclk.SetMode(GPIOModeAlternateFunction0);
    ce0.SetMode(GPIOModeAlternateFunction0);
    ce1.SetMode(GPIOModeAlternateFunction0);

    // core clock is needed to calculate SPI divider register
    unsigned core_clock =
        CMachineInfo::Get()->GetClockRate(CLOCK_ID_CORE);

    PeripheralEntry();

    // SPI clock register
    //
    // actual SPI speed =
    // core_clock / divider
    //
    // here:
    // divider = core_clock / nClockSpeed
    write32(ARM_IO_BASE + 0x204008,
            core_clock / nClockSpeed);

    // SPI control register
    //
    // CPOL goes to bit 3
    // CPHA goes to bit 2
    //
    // MCP3008 uses:
    // CPOL = 0
    // CPHA = 0
    //
    // which is SPI mode 0
    write32(ARM_IO_BASE + 0x204000,
            (CPOL << 3) | (CPHA << 2));

    PeripheralExit();

    return TRUE;
}

// and

inline int CKernel::SPIWriteRead(unsigned nChipSelect,
                                 const void *pWriteBuffer,
                                 void *pReadBuffer,
                                 unsigned nCount)
{
    const u8 *pWritePtr = (const u8 *) pWriteBuffer;
    u8 *pReadPtr = (u8 *) pReadBuffer;

    PeripheralEntry();

    // transfer length in bytes
    write32(ARM_IO_BASE + 0x20400C, nCount);

    // select CE, clear FIFOs, start transfer
    write32(ARM_IO_BASE + 0x204000,
            (nChipSelect)
            | (1 << 5)   // CLEAR_RX
            | (1 << 4)   // CLEAR_TX
            | (1 << 7)); // TA

    unsigned nWriteCount = 0;
    unsigned nReadCount  = 0;

    while (nWriteCount < nCount || nReadCount < nCount)
    {
        while (nWriteCount < nCount &&
              (read32(ARM_IO_BASE + 0x204000) & (1 << 18)))   // TXD
        {
            u32 nData = 0;
            if (pWritePtr != 0)
                nData = *pWritePtr++;

            write32(ARM_IO_BASE + 0x204004, nData);           // FIFO
            ++nWriteCount;
        }

        while (nReadCount < nCount &&
              (read32(ARM_IO_BASE + 0x204000) & (1 << 17)))   // RXD
        {
            u32 nData = read32(ARM_IO_BASE + 0x204004);       // FIFO
            if (pReadPtr != 0)
                *pReadPtr++ = (u8) nData;

            ++nReadCount;
        }
    }

    while (!(read32(ARM_IO_BASE + 0x204000) & (1 << 16)))     // DONE
    {
    }

    write32(ARM_IO_BASE + 0x204000,
            read32(ARM_IO_BASE + 0x204000) & ~(1 << 7));      // clear TA

    PeripheralExit();

    return (int) nCount;
}

inline int CKernel::ReadMCP3008Raw(unsigned channel)
{
    u8 tx[3] = { 0x01, (u8)((0x08 | channel) << 4), 0x00 };
    u8 rx[3];

    if (SPIWriteRead(SPI_CHIP_SELECT, tx, rx, 3) != 3)
        return -1;

    return ((rx[1] & 0x03) << 8) | rx[2];
}

// or 
/*
int CKernel::ReadMCP3008Raw(unsigned channel)
{
    u8 tx[3] = { 0x01, (u8)((0x08 | channel) << 4), 0x00 };
    u8 rx[3];

    PeripheralEntry();

    // transfer length = 3 bytes
    write32(ARM_IO_BASE + 0x20400C, 3);

    // clear RX/TX + select CE0 + start transfer
    write32(ARM_IO_BASE + 0x204000,
            (SPI_CHIP_SELECT)
            | (1 << 5)
            | (1 << 4)
            | (1 << 7));

    unsigned w = 0;
    unsigned r = 0;

    while (w < 3 || r < 3)
    {
        // TXD = TX FIFO can accept data
        while (w < 3 &&
              (read32(ARM_IO_BASE + 0x204000) & (1 << 18)))
        {
            write32(ARM_IO_BASE + 0x204004, tx[w]);
            ++w;
        }

        // RXD = RX FIFO has received data
        while (r < 3 &&
              (read32(ARM_IO_BASE + 0x204000) & (1 << 17)))
        {
            rx[r] = (u8) read32(ARM_IO_BASE + 0x204004);
            ++r;
        }
    }

    // wait DONE
    while (!(read32(ARM_IO_BASE + 0x204000) & (1 << 16)))
    {
    }

    // clear TA (transfer active)
    write32(ARM_IO_BASE + 0x204000,
            read32(ARM_IO_BASE + 0x204000) & ~(1 << 7));

    PeripheralExit();

    return ((rx[1] & 0x03) << 8) | rx[2];
}
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------

/* ---------------------------------------------------------------------------------------------------------------------------------------------------
// purpose:
// - direct framebuffer text debug output independent from CScreen/CString output stack.

// needed headers / dependencies:
#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>

// needed variables:
//     static CBcmFrameBuffer gE_FrameBuffer (0, 0, 32, 0, TRUE)
//     static CCharGenerator  gE_CharGenerator
//     static u32*      gE_PixelBuffer
//     static unsigned  gE_PitchBytes, gE_ScreenWidth, gE_ScreenHeight
//     static unsigned  gE_CharWidth, gE_CharHeight, gE_Cols, gE_Rows
// OR 

//  Self-contained framebuffer setup:
//  width/height are queried from firmware via mailbox when constructed with 0,0.
//  This reflects the effective display mode (typically driven by config.txt/EDID firmware states).


static CBcmFrameBuffer gE_FrameBuffer (0, 0, 32, 0, TRUE);
static CCharGenerator  gE_CharGenerator;

static u32      *gE_PixelBuffer  = 0;
static unsigned  gE_PitchBytes   = 0;
static unsigned  gE_ScreenWidth  = 0;
static unsigned  gE_ScreenHeight = 0;
static unsigned  gE_CharWidth    = 0;
static unsigned  gE_CharHeight   = 0;
static unsigned  gE_Cols         = 0;
static unsigned  gE_Rows         = 0;
};
// END OR

// key functions / entry points:
// - screen_plot(unsigned x, unsigned y, u32 color)
// - screen_draw_char(char ch, unsigned charCol, unsigned charRow, u32 fgColor, u32 bgColor)
// - screen_init(void)
// - screen_clear_screen(u32 bgColor)
// - screen_draw_buffer_segment(...)
// - screen_get_grid(unsigned &cols, unsigned &rows)

// implementation notes:
// - these globals define the debug text plane geometry; re-init required after display mode changes.
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

                if (gE_PixelBuffer == 0 || gE_CharWidth == 0 || gE_CharHeight == 0)
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

/* ---------------------------------------------------------------------------------------------------------------------------------------------------
// VERSION B
// pointer version
// reduced bloat
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>

class CKernel
{
public:
        CKernel (void);
        ~CKernel (void);

        boolean         bufferToScreenInit (
                                unsigned width,
                                unsigned height
                        );

public:
        CBcmFrameBuffer *gE_FrameBuffer;
        CCharGenerator   gE_CharGenerator;
        u32*             gE_PixelBuffer;
        unsigned         gE_PitchBytes;
        unsigned         gE_ScreenWidth;
        unsigned         gE_ScreenHeight;
        unsigned         gE_CharWidth;
        unsigned         gE_CharHeight;
        unsigned         gE_Cols;
        unsigned         gE_Rows;
};

CKernel::CKernel (void)
:       gE_FrameBuffer (0)
{
}

CKernel::~CKernel (void)
{
                delete gE_FrameBuffer;
}
//------------------------------------------------------------------------------------------------------------------------------------------------- */
boolean         CKernel::bufferToScreenInit                              (   unsigned width,
                                                                            unsigned height )
{
                delete gE_FrameBuffer;

                gE_FrameBuffer = new CBcmFrameBuffer (
                                width,
                                height,
                                32,
                                0,
                                TRUE
                                );

                if (gE_FrameBuffer == 0)
                    {
                    return FALSE;
                    }

                if (!gE_FrameBuffer->Initialize ())
                    {
                    return FALSE;
                    }

                gE_PixelBuffer  = (u32 *) gE_FrameBuffer->GetBuffer ();
                gE_PitchBytes   = gE_FrameBuffer.GetPitch ();
                gE_ScreenWidth  = gE_FrameBuffer.GetWidth ();
                gE_ScreenHeight = gE_FrameBuffer.GetHeight ();
                gE_CharWidth    = gE_CharGenerator.GetCharWidth ();
                gE_CharHeight   = gE_CharGenerator.GetCharHeight ();

                if (gE_PixelBuffer == 0 || gE_CharWidth == 0 || gE_CharHeight == 0)
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


// my own gpiopin code 

// can we use this instead of the CGPIOPin class????
// the point is GPOIPin is used by multible subsystems 
// like 
// #include <circle/actled.h> 
// #include <circle/machineinfo.h>
// #include <circle/serial.h>
// #include <circle/gpiomanager.h>
// and i dont know which more, and also is my code this far optimized to use it
// having my "own" GPOI code in my kernel class may not be helpful at all... but...

#include <circle/bcm2835.h>
#include <circle/types.h>
#include <circle/timer.h>

#define LOW  0
#define HIGH 1

#define PULL_OFF  0
#define PULL_DOWN 1
#define PULL_UP   2

static inline u32 CKernel::mmio_read32(uintptr addr)
{
    return *(volatile u32 *)addr;
}

static inline void CKernel::mmio_write32(uintptr addr, u32 value)
{
    *(volatile u32 *)addr = value;
}

void CKernel::gpio_write(unsigned p_pin, unsigned state, int pull)
{
    unsigned shift = (p_pin % 10) * 3;
    uintptr sel = ARM_GPIO_GPFSEL0 + (p_pin / 10) * 4;

    // set OUTPUT
    u32 v = mmio_read32(sel);
    v &= ~(7 << shift);
    v |=  (1 << shift);
    mmio_write32(sel, v);

    // optional pull
    if (pull >= 0)
    {
        u32 mask = 1 << (p_pin % 32);

        mmio_write32(ARM_GPIO_GPPUD, pull);
        CTimer::SimpleusDelay(5);
        mmio_write32(ARM_GPIO_GPPUDCLK0 + (p_pin / 32) * 4, mask);
        CTimer::SimpleusDelay(5);
        mmio_write32(ARM_GPIO_GPPUD, 0);
        mmio_write32(ARM_GPIO_GPPUDCLK0 + (p_pin / 32) * 4, 0);
    }

    // write HIGH / LOW (explicit)
    u32 mask = 1 << (p_pin % 32);

    if (state == HIGH)
    {
        mmio_write32(ARM_GPIO_GPSET0 + (p_pin / 32) * 4, mask);
    }

    if (state == LOW)
    {
        mmio_write32(ARM_GPIO_GPCLR0 + (p_pin / 32) * 4, mask);
    }
}


// from marcos.h

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

// from memio.h

#include <circle/types.h>

#ifdef __cplusplus      // why? 
extern "C" {
#endif

/// \brief Read 32-bit value from MMIO address
static inline u32 CKernel::read32 (uintptr nAddress)
{
	return *(u32 volatile *) nAddress;
}
/// \brief Write 32-bit value to MMIO address
static inline void CKernel::write32 (uintptr nAddress, u32 nValue)
{
	*(u32 volatile *) nAddress = nValue;
}
#ifdef __cplusplus
}
#endif


// from bcmwatchdog.h 
#include <circle/spinlock.h>


	static const unsigned MaxTimeoutSeconds = 15;

    CSpinLock m_SpinLock; // really ?!?!


void            CKernel::watchDogStart (unsigned nTimeoutSeconds)
{
                if (nTimeoutSeconds > MaxTimeoutSeconds)
                    {
                    nTimeoutSeconds = MaxTimeoutSeconds;
                    }
                m_SpinLock.Acquire ();  // really??

                write32 (ARM_PM_WDOG, ARM_PM_PASSWD | ((nTimeoutSeconds << 16) & ARM_PM_WDOG_TIME));

                write32 (ARM_PM_RSTC,   ARM_PM_PASSWD | ARM_PM_RSTC_REBOOT (read32 (ARM_PM_RSTC) & ARM_PM_RSTC_CLEAR));

                m_SpinLock.Release ();  // really??
}






































