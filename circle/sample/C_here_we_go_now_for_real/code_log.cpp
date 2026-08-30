#include "kernel.h"

    #define MY_BFR   m_logKernel                 // means the log goes into the pre-init buffer 
    #define MY_IDX    m_logKernelIndex

static const char FromKernel[] = "kernel";

void            CKernel::storeLogHex               (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* p_string0, u32 p_value0,
                                                        const char* p_string1, u32 p_value1,
                                                        const char* p_string2, u32 p_value2,
                                                        const char* p_string3, u32 p_value3)
{
                for (const char* p = p_string0; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }
                if (p_value0 != X_VAL)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value0 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string1 != X_STR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string1; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value1 != X_VAL)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value1 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string2 != X_STR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string2; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value2 != X_VAL)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value2 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string3 != X_STR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string3; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value3 != X_VAL)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value3 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';
#ifdef __LOG_TO_SCREEN__
                logScreenUpdate();
#endif                
}

void            CKernel::storeLogU32                (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* p_string0, u32 p_value0,
                                                        const char* p_string1, u32 p_value1,
                                                        const char* p_string2, u32 p_value2,
                                                        const char* p_string3, u32 p_value3)
{
                for (const char* p = p_string0; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }

                if (p_value0 != X_VAL)
                    {
                    p_buffer[index++] = ' ';

                    char digits[10];
                    u32  count = 0;
                    u32  value = p_value0;

                    do
                        {
                        digits[count++] = '0' + (value % 10);
                        value /= 10;
                        }
                    while (value != 0);

                    while (count > 0)
                        {
                        p_buffer[index++] = digits[--count];
                        }
                    }

                if (p_string1 != X_STR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string1; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }

                if (p_value1 != X_VAL)
                    {
                    p_buffer[index++] = ' ';

                    char digits[10];
                    u32  count = 0;
                    u32  value = p_value1;

                    do
                        {
                        digits[count++] = '0' + (value % 10);
                        value /= 10;
                        }
                    while (value != 0);

                    while (count > 0)
                        {
                        p_buffer[index++] = digits[--count];
                        }
                    }

                if (p_string2 != X_STR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string2; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }

                if (p_value2 != X_VAL)
                    {
                    p_buffer[index++] = ' ';

                    char digits[10];
                    u32  count = 0;
                    u32  value = p_value2;

                    do
                        {
                        digits[count++] = '0' + (value % 10);
                        value /= 10;
                        }
                    while (value != 0);

                    while (count > 0)
                        {
                        p_buffer[index++] = digits[--count];
                        }
                    }

                if (p_string3 != X_STR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string3; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }

                if (p_value3 != X_VAL)
                    {
                    p_buffer[index++] = ' ';

                    char digits[10];
                    u32  count = 0;
                    u32  value = p_value3;

                    do
                        {
                        digits[count++] = '0' + (value % 10);
                        value /= 10;
                        }
                    while (value != 0);

                    while (count > 0)
                        {
                        p_buffer[index++] = digits[--count];
                        }
                    }

                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';

#ifdef __LOG_TO_SCREEN__
                logScreenUpdate();
#endif
}

void            CKernel::storeMsg                   (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* label,
                                                        const void* tx_msg,
                                                        u32         total_size)
{
                p_buffer[index++]           = '\n';

                for (const char* p = label; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }
                p_buffer[index++]           = '\n';

                const unsigned char* b = (const unsigned char*)tx_msg;

                for (u32 i = 0; i < total_size; ++i) 
                    {
                    if (i && (i % 16) == 0)
                        {
                        p_buffer[index++]   = '\n';
                        }
                    unsigned char v         = b[i];

                    char hi = "0123456789ABCDEF"[v >> 4];
                    p_buffer[index++]       = hi;
                    char lo = "0123456789ABCDEF"[v & 0xF];
                    p_buffer[index++]       = lo;
                    p_buffer[index++]       = ' ';
                    }
                p_buffer[index++]           = '\n';
                p_buffer[index++]           = '\n';
                p_buffer[index]             = '\0';
#ifdef __LOG_TO_SCREEN__
                logScreenUpdate();
#endif                
}

void            CKernel::nextline                   (   char*       p_buffer,
                                                        u32&        index)
{
                p_buffer[index++]           = '\n';
                p_buffer[index]             = '\0';
#ifdef __LOG_TO_SCREEN__
                logScreenUpdate();
#endif                
}

u32             CKernel::convertDecToBCD     (   u32         value)
{
                u32 result = 0;
                u32 shift  = 0;

                do
                    {
                    result |= (value % 10) << shift;
                    value  /= 10;
                    shift  += 4;
                    }
                while (value != 0 && shift < 32);

                return result;
}

#ifdef __LOG_TO_SCREEN__
void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;
                if (MY_IDX == 0) return;

                u32 drawIndex  = m_logScreenStartIndex;
                u32 scanIndex  = m_logScreenStartIndex;

                unsigned col   = 0;
                unsigned row   = 0;
#ifdef __SCROLLING__
                bufferScreenClear();
#endif
                while (scanIndex < MY_IDX)
                    {
                    char ch = MY_BFR[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < MY_IDX && MY_BFR[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < MY_IDX)
                                {
                                drawIndex++;
                                }
#ifdef __SCROLLING__
                            bufferScreenClear();

                            bufferScreenDraw(   MY_BFR,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );
                            msDelay(SCROLLSPEED);
#endif
                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;
#ifdef __SCROLLING__
                            bufferScreenClear();

                            bufferScreenDraw(   MY_BFR,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );
                            msDelay(SCROLLSPEED);
#endif
                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }
                bufferScreenClear();

                bufferScreenDraw(   MY_BFR,
                                    drawIndex,
                                    MY_IDX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );
                m_logScreenStartIndex = drawIndex;
}
#endif 

bool            CKernel::memoryDebugCheckpoint      (   const char* p_Label,
                                                        bool        p_DumpStatus )
{
                CMemorySystem* pMem = CMemorySystem::Get();

                if (pMem == nullptr)
                    {
                    return FALSE;
                    }
#ifdef __LOG_MEMORY__                    
                size_t total = pMem->GetMemSize();
                size_t low   = pMem->GetHeapFreeSpace(HEAP_LOW);
                size_t high  = pMem->GetHeapFreeSpace(HEAP_HIGH);
                size_t any   = pMem->GetHeapFreeSpace(HEAP_ANY);

                nextline( MY_BFR, MY_IDX );

                storeLogHex(   MY_BFR, MY_IDX,
                            "MEM", X_VAL,
                            p_Label, X_VAL,
                            "TOTAL", (u32)total,
                            "LOW", (u32)low );

                storeLogHex(   MY_BFR, MY_IDX,
                            "MEM", X_VAL,
                            p_Label, X_VAL,
                            "HIGH", (u32)high,
                            "ANY", (u32)any );

#ifdef HEAP_DEBUG
                if (p_DumpStatus)
                    {
                    CMemorySystem::DumpStatus();
                    }
#endif
                nextline( MY_BFR, MY_IDX );
#endif            
                return TRUE;
}

void 			CKernel::LoggerSink						(	        void* 			    pContext, 
															const   char* 	            pText, 
															        unsigned 		    nLength )
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logKernelIndex >= LOG_KERNEL_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logKernel[pThis->m_logKernelIndex++] = ch; // means the log goes into the pre-init buffer 
                    }
            //  pThis->m_logKernel[pThis->m_logKernelIndex++] = '\n';
                pThis->m_logKernel[pThis->m_logKernelIndex] = '\0'; // means the log goes into the pre-init buffer 
#ifdef __LOG_TO_SCREEN__
                pThis->logScreenUpdate();
#endif                
}