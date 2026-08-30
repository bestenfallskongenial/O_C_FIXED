
#ifndef EMPTYLOG
#define EMPTYLOG 0xFFFFFFFF
#endif

// ---- external framebuffer / font system stubs ----
extern u32*     gE_PixelBuffer;
extern u32      gE_PitchBytes;
extern u32      gE_ScreenWidth;
extern u32      gE_ScreenHeight;
extern u32      gE_CharWidth;
extern u32      gE_CharHeight;
extern u32      gE_Cols;
extern u32      gE_Rows;

struct FrameBufferStub
{
    boolean Initialize();
    void*   GetBuffer();
    u32     GetPitch();
    u32     GetWidth();
    u32     GetHeight();
};

struct CharGeneratorStub
{
    u32     GetCharWidth();
    u32     GetCharHeight();
    boolean GetPixel(char ch, unsigned x, unsigned y);
};

extern FrameBufferStub     gE_FrameBuffer;
extern CharGeneratorStub   gE_CharGenerator;


public:

    // ---- logging ----
    void storeLog(char* buffer,
                  u32& index,
                  const char* label,
                  u32 value1,
                  u32 value2,
                  u32 value3,
                  u32 value4);

    void storeMsg(char* buffer,
                  u32& index,
                  const char* label,
                  const void* tx_msg,
                  u32 total_size);

    void nextline(char* buffer,
                  u32& index);

    // ---- framebuffer text ----
    static void bufferToScreenPlot(unsigned x, unsigned y, u32 color);

    static void bufferToScreenDrawChar(char ch,
                                       unsigned charCol,
                                       unsigned charRow,
                                       u32 fgColor,
                                       u32 bgColor);

    boolean bufferToScreenInit(void);

    void bufferToScreenClear(u32 bgColor);

    void bufferToScreenDrawBufferSegment(const char* pSourceBuffer,
                                         u32 startIndex,
                                         u32 endIndex,
                                         unsigned startCol,
                                         unsigned startRow,
                                         u32 fgColor,
                                         u32 bgColor);

    unsigned bufferToScreenGetGrid(unsigned& cols,
                                   unsigned& rows);
