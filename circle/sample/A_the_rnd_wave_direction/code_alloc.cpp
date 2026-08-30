#include "kernel.h"

    #define MY_BUFFER   m_logBuffer                 // means the log goes into the pre-init buffer 
    #define MY_INDEX    m_logBufferIndex

char**          CKernel::allocBufferMEM             (           size_t      p_count, 
                                                                size_t      bufferSize ) 
{
                char** buffers = (char**)malloc(p_count * sizeof(char*));
#ifdef __LOG_ALLOC__
                nextline(       MY_BUFFER, MY_INDEX);     
                storeLogHex(    MY_BUFFER, MY_INDEX,
                                "ALLOC-MEM base   ",        (u32)buffers,
                                " count",                   (u32)p_count,
                                "size ",                    (u32)bufferSize );
#endif
                for (size_t i = 0; i < p_count; ++i) 
                    {
                    buffers[i] = (char*)calloc(bufferSize, sizeof(char));
#ifdef __LOG_ALLOC__
                    storeLogHex(    MY_BUFFER, MY_INDEX,
                                    "ALLOC-MEM slice [",    (u32)i,
                                    "] ptr ",               (u32)buffers[i],
                                    "size ",                (u32)bufferSize );
#endif
                    }
#ifdef __LOG_ALLOC__
                storeLogHex(    MY_BUFFER, MY_INDEX,
                                ":>", m_Timer.GetClockTicks(),
                                "ALLOC-MEM done",           (u32)buffers,
                                "count",                    (u32)p_count,
                                "size",                     (u32)bufferSize);
                nextline(       MY_BUFFER, MY_INDEX );       
#endif         
                msDelay(100);
                return buffers;
}

char**          CKernel::allocBufferDMA             (           size_t      p_count, 
                                                                size_t      bufferSize,
                                                                char**      blockBaseOut,
                                                                char**      rawBlockOut,
                                                                size_t*     alignedSizeOut )
{
                size_t total_size = p_count * bufferSize;

                size_t aligned_total_size = (total_size + 4095) & ~4095;

                char* raw = new (HEAP_DMA30) char[aligned_total_size + 4096];

                char* dma_block = (char*)(((uintptr_t)raw + 4095) & ~4095);

                char** buffers = new char*[p_count];
#ifdef __LOG_ALLOC__ 
                nextline( MY_BUFFER, MY_INDEX );
#endif
                for (size_t i = 0; i < p_count; ++i)
                    {
                    buffers[i] = dma_block + i * bufferSize;
#ifdef __LOG_ALLOC__   
                    storeLogHex(    MY_BUFFER, MY_INDEX,
                                    "ALLOC-DMA slice [",    (u32)i,
                                    "] ptr",                (u32)buffers[i],
                                    "size ",                (u32)bufferSize );
#endif  
                    memset(buffers[i], 0, bufferSize);
                    }
#ifdef __LOG_ALLOC__
                nextline(       MY_BUFFER, MY_INDEX);
                storeLogHex(    MY_BUFFER, MY_INDEX,
                                ":>",                       m_Timer.GetClockTicks(),
                                "ALLOC-DMA raw",            (u32)raw,
                                "block",                    (u32)dma_block,
                                "total",                    (u32)total_size );
#endif
                *blockBaseOut   = dma_block;
                *rawBlockOut    = raw;
                *alignedSizeOut = aligned_total_size;

                msDelay(100);
                return buffers;
}

void            CKernel::clearBufferMEM             (           char**      buffers, 
                                                                size_t      p_count) 
{
                for (size_t i = 0; i < p_count; ++i) free(buffers[i]);

                free(buffers);

                buffers = nullptr;                
}

void            CKernel::clearBufferDMA             (           char**      buffers, 
                                                                char*       rawBlock )
{
                delete[] rawBlock;
                delete[] buffers;

                rawBlock = nullptr;
                buffers  = nullptr;
}