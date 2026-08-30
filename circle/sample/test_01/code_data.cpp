
#include "kernel.h"                 // seemingly the file is complete and working as intended

#define __DEBUG_LOG__
#define ALLOC_DEBUG

#define MY_BUFFER   m_logBuffer
#define MY_INDEX    m_logBufferIndex

bool            CKernel::Mount                      (   const   char*       p_deviceName )
{
                CDevice* f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                m_pFileSystem = new CFATFileSystem;

                if (f_partitionName == 0 || m_pFileSystem == 0 || !m_pFileSystem->Mount(f_partitionName))
                {
                    delete m_pFileSystem;
                    m_pFileSystem = 0;
                    return false;
                }
                return true;
}

bool            CKernel::UnMount                    (   )
{
                if (m_pFileSystem == 0) return false;

                m_pFileSystem->UnMount();
                delete m_pFileSystem;
                m_pFileSystem = 0;
                return true;
}

bool            CKernel::openFile                   (   const   char*       p_fileName)
{   
	            g_hFile = m_pFileSystem->FileOpen (p_fileName);

	            if (g_hFile == 0) return false;
	
                return true;
}

unsigned        CKernel::loadToBuffer               (           char*       p_bufferArray,                                                   
                                                                unsigned    p_bufferSize)                                               
{
                unsigned f_totalBytesRead = 0;
                unsigned f_bytesRead;
    
                while (f_totalBytesRead < p_bufferSize)
                    {
                    unsigned f_currentChunkSize = (p_bufferSize - f_totalBytesRead < CHUNK_SIZE) ? (p_bufferSize - f_totalBytesRead) : CHUNK_SIZE;

                    f_bytesRead = m_pFileSystem->FileRead(g_hFile, p_bufferArray + f_totalBytesRead, f_currentChunkSize);

                    if (f_bytesRead == FS_ERROR) return 0;

                    if (f_bytesRead == 0)return f_totalBytesRead;

                    f_totalBytesRead += f_bytesRead;

                     // m_Watchdog.Start(TIMEOUT);
                    }
                return 0;
}

bool            CKernel::saveFromBufferM            (   const   char*       p_deviceName,  
                                                        const   char*       p_fileName,             
                                                        const   char*       p_bufferArray,
                                                                unsigned    p_bufferSize )
{
                if(!Mount( p_deviceName ))
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog(   MY_BUFFER, MY_INDEX,
                                    "Failed to Mount Device",   EMPTYLOG,
                                    p_deviceName,               EMPTYLOG,
                                    EMPTYSTR,                   EMPTYLOG,
                                    EMPTYSTR,                   EMPTYLOG );
#endif                    
                    return false;
                    }

                if (m_pFileSystem == 0 || p_fileName == 0 || p_bufferArray == 0 || p_bufferSize == 0)
                    {
#ifdef __DEBUG_LOG__
                    storeLog(   MY_BUFFER, MY_INDEX,
                                    "Failed Store Input",       EMPTYLOG,
                                    "file",                     EMPTYLOG,
                                    p_fileName,                 EMPTYLOG,
                                    "size",                     (u32)p_bufferSize );
#endif
                    return false;
                    }

                g_hFile = m_pFileSystem->FileCreate(p_fileName);
                if (g_hFile == 0)
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog(   MY_BUFFER, MY_INDEX,
                                    "Failed to Create File",    EMPTYLOG,
                                    p_fileName,                 EMPTYLOG,
                                    "on Device",                EMPTYLOG,
                                    p_deviceName,               EMPTYLOG );
#endif                    
                    return false;
                    }

                if (m_pFileSystem->FileWrite(g_hFile, p_bufferArray, p_bufferSize) != p_bufferSize)
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog(   MY_BUFFER, MY_INDEX,
                                    "Failed to Store File",     EMPTYLOG,
                                    p_fileName,                 EMPTYLOG,
                                    "size",                     (u32)p_bufferSize,
                                    EMPTYSTR,                   EMPTYLOG );
#endif                                      
                    return false;
                    }

                closeFile();
                UnMount();

#ifdef __DEBUG_LOG__
                storeLog(   MY_BUFFER, MY_INDEX,
                                "Successful Stored",    EMPTYLOG,
                                p_fileName,             EMPTYLOG,
                                "from Buffer",          EMPTYLOG,
                                p_bufferArray,          (u32)p_bufferSize );
#endif                                 
                return true;
}

bool            CKernel::closeFile                  (   )	                                                                        
{
	            if (!m_pFileSystem->FileClose(g_hFile)) return false;

                return true;
}

void            CKernel::bulkLoad                   (           char*       p_fileNameArray[],
                                                                unsigned    p_loadedBytes[],
                                                                char**      p_bufferArray,
                                                                unsigned    p_maxFiles,
                                                                unsigned&   p_validFiles,
                                                                unsigned&   p_prevFiles,
                                                                unsigned    p_fileSize)
{
                p_prevFiles = p_validFiles;

#ifdef __DEBUG_LOG__
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX,
                            "BULKLOAD Start scanned",   (u32)p_maxFiles,
                            "valid",                (u32)p_validFiles,
                            "size",                 (u32)p_fileSize,
                            EMPTYSTR,               EMPTYLOG );
#endif

                for (unsigned i = 0; i < p_maxFiles; ++i) 
                    {
                    if (openFile(p_fileNameArray[i]))
                        {
                        unsigned f_bytesRead = loadToBuffer(p_bufferArray[p_validFiles], p_fileSize);
                        if (f_bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = f_bytesRead;
#ifdef __DEBUG_LOG__
                            storeLog(   MY_BUFFER, MY_INDEX,
                                        "file",              (u32)i,
                                        p_fileNameArray[i],  EMPTYLOG,
                                        "bytes read",        (u32)f_bytesRead,
                                        "in buffer",         (u32)p_validFiles );
#endif                            
                            p_validFiles++;   
                            }
                        closeFile();
                        }
                    }

#ifdef __DEBUG_LOG__
            //  nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX,
                                "BULKLOAD End prev",    (u32)p_prevFiles,
                                "new",                  (u32)p_validFiles,
                                "loaded",               (u32)(p_validFiles - p_prevFiles),
                                EMPTYSTR,               EMPTYLOG );
#endif
}

bool            CKernel::IsValidFile                (   const   char*       pFileName,
                                                        const   char*       extension)
{
                if (!pFileName || !extension) return false;
                const char* dot = 0;
                const char* p = pFileName;
                int index = 0;
                
                while (*p)                              {
                                                        if (*p == '.') dot = p;
                                                        p++;
                                                        index++;
                                                        }
                if (!dot) return false;

                int dotPos = dot - pFileName;

                if (dotPos == 0 || dotPos > 8) return false;

                const char* suffix = dot + 1;
                
                while (*suffix && *extension)           {
                                                        if (*suffix != *extension) return false;
                                                        suffix++;
                                                        extension++;
                                                        }
                
                return (*suffix == '\0' && *extension == '\0');
}

bool            CKernel::scanRoot                   (           char**      p_fileNameArray,
                                                        const   char*       p_fileExtArray[],
                                                                unsigned    p_extentionCount,
                                                                unsigned&   p_scannedFiles,
                                                                unsigned    p_maxFiles )
{
                p_scannedFiles = 0;

                TDirentry           f_directoryEntry;
                TFindCurrentEntry   f_currentDirectoryEntry;
                
                unsigned            f_nextEntry                 = m_pFileSystem->RootFindFirst(&f_directoryEntry, &f_currentDirectoryEntry);

                if(f_nextEntry == 0) return false;

                while (f_nextEntry != 0 && p_scannedFiles < p_maxFiles) 
                    {
                    if (!(f_directoryEntry.nAttributes & FS_ATTRIB_SYSTEM)) 
                        {
                        for (unsigned i = 0; i < p_extentionCount; ++i)
                            {
                            if (IsValidFile(f_directoryEntry.chTitle, p_fileExtArray[i])) 
                                {
                                p_fileNameArray[p_scannedFiles] = new char[strlen(f_directoryEntry.chTitle) + 1];
                                strcpy(p_fileNameArray[p_scannedFiles], f_directoryEntry.chTitle);
                                p_scannedFiles++;
                                break;
                                }
                            }
                        }
                    f_nextEntry = m_pFileSystem->RootFindNext(&f_directoryEntry, &f_currentDirectoryEntry);
                    }
                return true;
}

bool            CKernel::updateUSB                  (   const   char*       p_deviceName )
{
                if (!m_USBHCI.UpdatePlugAndPlay()) return false;

                CDevice* pDevice = m_DeviceNameService.GetDevice(p_deviceName, TRUE);

                if (pDevice == nullptr) return false;

                m_bStorageAttached = true;

                pDevice->RegisterRemovedHandler(removeUSB, this);

                return true;
}

void            CKernel::removeUSB                  (           CDevice*    pDevice,
                                                                void*       pContext )
{
	            CKernel *pThis = (CKernel *) pContext;
	            assert (pThis != 0); // ???
	            pThis->m_bStorageAttached = FALSE;
}

char**          CKernel::allocBufferMEM             (           size_t      p_count, 
                                                                size_t      bufferSize ) 
{
                char** buffers = (char**)malloc(p_count * sizeof(char*));
#ifdef ALLOC_DEBUG
                storeLog(   MY_BUFFER, MY_INDEX,
                                "ALLOC-MEM base",      (u32)buffers,
                                "count",               (u32)p_count,
                                "size",                (u32)bufferSize,
                                EMPTYSTR,              EMPTYLOG );
#endif

                for (size_t i = 0; i < p_count; ++i) 
                    {
                    buffers[i] = (char*)calloc(bufferSize, sizeof(char));
#ifdef ALLOC_DEBUG
                    storeLog(   MY_BUFFER, MY_INDEX,
                                    "ALLOC-MEM slice", (u32)i,
                                    "ptr",             (u32)buffers[i],
                                    EMPTYSTR,          EMPTYLOG,
                                    EMPTYSTR,          EMPTYLOG );
#endif
                    }
#ifdef ALLOC_DEBUG
                storeLog(   MY_BUFFER, MY_INDEX,
                                "ALLOC-MEM done",      (u32)buffers,
                                "count",               (u32)p_count,
                                "size",                (u32)bufferSize,
                                EMPTYSTR,              EMPTYLOG );
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
#ifdef ALLOC_DEBUG                  
                storeLog(   MY_BUFFER, MY_INDEX,
                                "ALLOC-DMA raw",        (u32)raw,
                                "block",                (u32)dma_block,
                                EMPTYSTR,               EMPTYLOG,
                                EMPTYSTR,               EMPTYLOG );
#endif
                char** buffers = new char*[p_count];

                for (size_t i = 0; i < p_count; ++i)
                    {
                    buffers[i] = dma_block + i * bufferSize;
#ifdef ALLOC_DEBUG   
                    storeLog(   MY_BUFFER, MY_INDEX,
                                    "ALLOC-DMA slice",  (u32)i,
                                    "ptr",              (u32)buffers[i],
                                    "size",             (u32)bufferSize,
                                    EMPTYSTR,           EMPTYLOG );
#endif  
                    memset(buffers[i], 0, bufferSize);
                    }
#ifdef ALLOC_DEBUG   
                storeLog(   MY_BUFFER, MY_INDEX,
                                "ALLOC-DMA raw",        (u32)raw,
                                "block",                (u32)dma_block,
                                "total",                (u32)total_size,
                                "aligned",              (u32)aligned_total_size );
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