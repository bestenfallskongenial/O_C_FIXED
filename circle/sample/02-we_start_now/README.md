datamanagement.cpp
373
graphics.cpp
489
hardware.cpp
458
    /*
    `m_BufferLength` is set here:

    m_BufferLength = TX_BUFF_LEN(ledCount);

    inside:

    my_WS2812_Init(unsigned ledCount)

    So the real order is:

    my_SMI_Init(...)
    → my_WS2812_Init(...)
        → m_BufferLength gets set
        → my_SMI_SetupTiming(...)
        → my_SMI_SetupDMA()
    → my_WS2812_SetLED(...)
    → my_WS2812_Update()

    That is why `my_SMI_SetupDMA()` works there:

    because `m_BufferLength` is already valid from `my_WS2812_Init()`.
    */
logging.cpp
354
menu.cpp
225
parser.cpp
361
stubs.cpp
25
util.cpp
42
vc04_service,cpp
988

/*  NEW AUDIO AND ADC SEPERATED !

    adc_AcquireConvert()
    adc_ProcessAudio()
    advance_adc_index()

    set_mode_length(0)
    modeMenuAssignGroup(...)
*/

/*
    make83FileName             (   const char* ext )

    wil create a random filename from my random value generator, the idea 
    is to have some unique and "quick" sollution to store the logs 
    in case of issues / reboot events...

    uses:

    public:

        char m_83FileName[13];

    example calls :

    saveFromBufferM(p_deviceName, make83FileName("LOG"), MY_BUFFER, MY_INDEX);

    saveFromBufferM(p_deviceName, make83FileName("TXT"), MY_BUFFER, MY_INDEX);

    saveFromBufferM("umsd1", make83FileName("TXT"), p_bufferArray, p_bufferSize);

*/

// CLogger shim layer for CKernel!!!
/*
void CKernel::KernelLoggerSink(const char* pText, unsigned nLength)
{
                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (m_logBufferIndex >= LOG_BUFFER_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    m_logBuffer[m_logBufferIndex++] = ch;
                    }

                m_logBuffer[m_logBufferIndex] = '\0';
}

Rest around your fixed function:



// kernel.h relevant member
CLogger m_Logger;

// constructor relevant part
CKernel::CKernel(void)
:               m_Interrupt(),
                m_Timer(&m_Interrupt),
                m_Logger(LOGLEVEL, &m_Timer),
                m_EMMC(&m_Interrupt, &m_Timer, &m_ActLED),
                m_USBHCI(&m_Interrupt, &m_Timer, TRUE),

{
}

// Initialize relevant logger part
if (bOK)
    {
    m_logBufferIndex = 0;

    CLogger::SetRawSink(KernelLoggerSink);

    bOK = m_Logger.Initialize(0);
    }

// draw logger buffer
bufferToScreenClear();
bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Buffer declaration / definition models:

Model 1: fixed global array

#define LOG_BUFFER_SIZE 1024*64

char m_logBuffer[LOG_BUFFER_SIZE];
u32  m_logBufferIndex = 0;

Matching extern:

extern char m_logBuffer[LOG_BUFFER_SIZE];
extern u32  m_logBufferIndex;

Model 2: fixed global storage plus pointer alias

#define LOG_BUFFER_SIZE 1024*64

char  g_logBufferStorage[LOG_BUFFER_SIZE];
char* m_logBuffer = g_logBufferStorage;
u32   m_logBufferIndex = 0;

Matching extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Model 3: pointer to existing allocated buffer

#define LOG_BUFFER_SIZE 1024*64

char* m_logBuffer = nullptr;
u32   m_logBufferIndex = 0;

Later, before logger use:

m_logBuffer = your_existing_buffer;
m_logBufferIndex = 0;

Matching extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Model 4: CKernel member array

// kernel.h
#define LOG_BUFFER_SIZE 1024*64

class CKernel
{
private:
    char m_logBuffer[LOG_BUFFER_SIZE];
    u32  m_logBufferIndex;
};

Then `KernelLoggerSink()` cannot be a plain free function using `m_logBuffer` unless it has access to the active `CKernel` object. For your current free-function sink, use Model 1, 2, or 3.

`bufferToScreenDrawBuffer()` accepts anything that can be passed as:

const char* pSourceBuffer

So these are valid buffer/source models.

Model 1: string literal

bufferToScreenDrawBuffer("hello world", 0, 11, 0, 0, 0xFFFFFFFF);

With length:

const char* text = "hello world";

bufferToScreenDrawBuffer(text, 0, strlen(text), 0, 0, 0xFFFFFFFF);

Model 2: fixed char array

char logBuffer[1024];

bufferToScreenDrawBuffer(logBuffer, 0, logIndex, 0, 0, 0xFFFFFFFF);

Model 3: global fixed char array

extern char m_logBuffer[LOG_BUFFER_SIZE];
extern u32  m_logBufferIndex;

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Model 4: pointer to allocated memory

char* logBuffer = (char*)malloc(LOG_BUFFER_SIZE);

bufferToScreenDrawBuffer(logBuffer, 0, logIndex, 0, 0, 0xFFFFFFFF);

Model 5: pointer to one slice from your `char**`

char** buffers = allocBufferMEM(4, LOG_BUFFER_SIZE);

bufferToScreenDrawBuffer(buffers[0], 0, logIndex, 0, 0, 0xFFFFFFFF);

Model 6: CKernel member array

// kernel.h
char m_logBuffer[LOG_BUFFER_SIZE];
u32  m_logBufferIndex;

Call:

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Model 7: CKernel member pointer

// kernel.h
char* m_logBuffer;
u32   m_logBufferIndex;

Call:

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Model 8: partial range of a buffer

bufferToScreenDrawBuffer(m_logBuffer, 128, 256, 0, 0, 0xFFFFFFFF);

Model 9: last N chars

u32 start = (m_logBufferIndex > 512) ? (m_logBufferIndex - 512) : 0;

bufferToScreenDrawBuffer(m_logBuffer, start, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Required condition for all models:

pSourceBuffer != 0
startIndex < endIndex
endIndex <= real buffer size

Your logger case is simply:

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

*/

// for CKernel!!!
/*
kernel.h relevant complete section inside class CKernel:

#define LOG_BUFFER_SIZE (1024 * 64)

class CKernel
{
public:
                char m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32  m_logBufferIndex = 0;

private:
        static  void LoggerSink          (   void*       pContext,
                                            const char* pText,
                                            unsigned    nLength );

private:
                CLogger m_Logger;
};

// CKernel log sink function:

void CKernel::LoggerSink(void* pContext, const char* pText, unsigned nLength)
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logBufferIndex >= LOG_BUFFER_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logBuffer[pThis->m_logBufferIndex++] = ch;
                    }

                pThis->m_logBuffer[pThis->m_logBufferIndex] = '\0';
}

CKernel constructor relevant complete section:

CKernel::CKernel(void)
:               m_Interrupt(),
                m_Timer(&m_Interrupt),
                m_Logger(LOGLEVEL, &m_Timer),
                m_EMMC(&m_Interrupt, &m_Timer, &m_ActLED),
                m_USBHCI(&m_Interrupt, &m_Timer, TRUE)
{
}

// CKernel::Initialize() logger section:

if (bOK)
    {
    CLogger::SetRawSink(CKernel::LoggerSink, this);

    bOK = m_Logger.Initialize(0);
    }

// draw logger buffer:

screen_clear_screen(0x00000000);

screen_draw_buffer_segment  (
                            m_logBuffer,
                            0,
                            m_logBufferIndex,
                            0,
                            0,
                            0xFFFFFFFF,
                            0x00000000
                            );

Buffer declaration / definition models:

All valid buffer definitions for the logger buffer:

#define LOG_BUFFER_SIZE (1024 * 64)

### 1. `CKernel` member array

public:
                char m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32  m_logBufferIndex = 0;

Use with:

pThis->m_logBuffer
pThis->m_logBufferIndex

### 2. `CKernel` member pointer to allocated/existing buffer

public:
                char* m_logBuffer = 0;
                u32   m_logBufferIndex = 0;

Then before logger use:

m_logBuffer = some_valid_buffer;

Use with:

pThis->m_logBuffer
pThis->m_logBufferIndex

### 3. `CKernel` member pointer to one existing `char**` slice

public:
                char* m_logBuffer = 0;
                u32   m_logBufferIndex = 0;

Then after allocation:

m_logBuffer = m_bufferLog[0];

Use with:

pThis->m_logBuffer
pThis->m_logBufferIndex

### 4. Fixed global array

In one `.`:

char m_logBuffer[LOG_BUFFER_SIZE] = {0};
u32  m_logBufferIndex = 0;

In header if needed elsewhere:

extern char m_logBuffer[LOG_BUFFER_SIZE];
extern u32  m_logBufferIndex;

Use directly:

m_logBuffer
m_logBufferIndex

### 5. Global storage + global pointer alias

In one `.`:

char  g_logBufferStorage[LOG_BUFFER_SIZE] = {0};
char* m_logBuffer = g_logBufferStorage;
u32   m_logBufferIndex = 0;

Extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Use directly:

m_logBuffer
m_logBufferIndex

### 6. Global pointer to allocated/existing buffer

In one `.cpp`:

char* m_logBuffer = 0;
u32   m_logBufferIndex = 0;

Extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Before logger use:

m_logBuffer = some_valid_buffer;

Use directly:

m_logBuffer
m_logBufferIndex

### 7. Local/static file-scope buffer inside one `.`

static char m_logBuffer[LOG_BUFFER_SIZE] = {0};
static u32  m_logBufferIndex = 0;

Only usable inside that `.`.

For your current context-pointer `CKernel::LoggerSink`, the clean matching choices are:

char m_logBuffer[LOG_BUFFER_SIZE] = {0};
u32  m_logBufferIndex = 0;

or:

char* m_logBuffer = 0;
u32   m_logBufferIndex = 0;

*/