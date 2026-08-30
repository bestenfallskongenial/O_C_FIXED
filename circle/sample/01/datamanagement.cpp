//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------
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

bool            CKernel::UnMount                    ()
{
                if (m_pFileSystem == 0)
                    {
                    return false;
                    }
                m_pFileSystem->UnMount();
                delete m_pFileSystem;
                m_pFileSystem = 0;
                return true;
}

bool            CKernel::openFile                   (   const   char*       p_fileName)
{   
	            g_hFile = m_pFileSystem->FileOpen (p_fileName);
	            if (g_hFile == 0)
		            {
		            return false;
		            }	
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

                    if (f_bytesRead == FS_ERROR)
                        {
                        return 0;
                        }
                    if (f_bytesRead == 0)
                        {
                        return f_totalBytesRead;
                        }
                    f_totalBytesRead += f_bytesRead;

                    m_Watchdog.Start(TIMEOUT);
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
                    storeLog( MY_BUFFER, MY_INDEX, "Failed to Mount Device");
                    storeLog( MY_BUFFER, MY_INDEX, p_deviceName);    
#endif                      
                    return false;
                    }
                if (m_pFileSystem == 0 || p_fileName == 0 || p_bufferArray == 0 || p_bufferSize  == 0)
                    {
                    return false;
                    }
                g_hFile = m_pFileSystem->FileCreate(p_fileName);
                if (g_hFile == 0)
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog( MY_BUFFER, MY_INDEX, "Failed to Create File");
                    storeLog( MY_BUFFER, MY_INDEX, p_fileName);    
#endif                      
                    return false;
                    }
                if (m_pFileSystem->FileWrite(g_hFile, p_bufferArray, p_bufferSize) != p_bufferSize)
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog( MY_BUFFER, MY_INDEX, "Failed to Store File");
                    storeLog( MY_BUFFER, MY_INDEX, p_fileName);      
#endif                                        
                    return false;
                    }
                closeFile();
                UnMount();
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "Successful Stored")
                storeLog( MY_BUFFER, MY_INDEX, p_fileName);
                storeLog( MY_BUFFER, MY_INDEX, "into Buffer");
                storeLog( MY_BUFFER, MY_INDEX, p_bufferArray, p_bufferSize);
                storeLog( MY_BUFFER, MY_INDEX, "on Device");
                storeLog( MY_BUFFER, MY_INDEX, p_deviceName);
#endif                                   
                return true;
}

bool            CKernel::closeFile                  ()	                                                                        
{
	            if (!m_pFileSystem->FileClose(g_hFile))
		            {
		            return false;
		            }
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
                storeLog (MY_BUFFER, MY_INDEX, "BULKLOAD begin max/valid/size", (u32) p_maxFiles, (u32) p_validFiles, (u32) p_fileSize);
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
                            storeLog (MY_BUFFER, MY_INDEX, p_fileNameArray[i], (u32) f_bytesRead);
                            storeLog (MY_BUFFER, MY_INDEX, p_bufferArray, (u32) p_validFiles);
#endif                              
                            p_validFiles++;   
                            }
                        closeFile();
                        }
#ifdef __DEBUG_LOG__
                storeLog (MY_BUFFER, MY_INDEX, "BULKLOAD end prev/new/loaded", (u32) p_prevFiles, (u32) p_validFiles, (u32) (p_validFiles - p_prevFiles));
#endif  
                    }   
}

bool            CKernel::IsValidFile                (   const   char*       pFileName,
                                                        const   char*       extension)
{
                if (!pFileName || !extension)
                    return false;
                const char* dot = 0;
                const char* p = pFileName;
                int index = 0;
                
                while (*p)
                    {
                    if (*p == '.')
                        dot = p;
                    p++;
                    index++;
                    }
                if (!dot)
                    return false;
                int dotPos = dot - pFileName;
                if (dotPos == 0 || dotPos > 8)
                    return false;
                const char* suffix = dot + 1;
                
                while (*suffix && *extension)
                    {
                    if (*suffix != *extension)
                        return false;
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

                if(f_nextEntry == 0)
                        {
                    return false;
                        }
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

bool            CKernel::updateUSB                  (   const   char*       p_deviceType )
{
                if (m_USBHCI.UpdatePlugAndPlay())
                    {
                    CDevice* pDevice = m_DeviceNameService.GetDevice(p_deviceType, TRUE);
                    if (pDevice != nullptr)
                        {
                        m_bStorageAttached = true;

                        pDevice->RegisterRemovedHandler(removeUSB, this);
                        return true;
                        }
                    }
                return false;
}

void            CKernel::removeUSB                  (           CDevice*    pDevice,
                                                                void*       pContext )
{
	            CKernel *pThis = (CKernel *) pContext;
	            assert (pThis != 0);
	        //  assert (pThis->m_bStorageAttached);
	            pThis->m_bStorageAttached = FALSE;
}

char**          CKernel::allocBufferMEM             (           size_t      p_count, 
                                                                size_t      bufferSize ) 
{
                char** buffers = (char**)malloc(p_count * sizeof(char*));
#ifdef ALLOC_DEBUG                   ␊
                storeLog( MY_BUFFER, MY_INDEX, "ALLOC-DMA buffers", (u32) buffers);
#endif 
                for (size_t i = 0; i < p_count; ++i) 
                {
                    buffers[i] = (char*)calloc(bufferSize, sizeof(char));
#ifdef ALLOC_DEBUG                    
                storeLog( MY_BUFFER, MY_INDEX, "ALLOC-DMA buffers[i]", (u32) i, (u32) buffers[i]);
#endif 
                }
#ifdef ALLOC_DEBUG   
                storeLog( MY_BUFFER, MY_INDEX, "ALLOC-DMA final", (u32) buffers, (u32) p_count, (u32) bufferSize);
#endif          
                msleep(100);
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
                storeLog( MY_BUFFER, MY_INDEX,  "Alloc RAW:", (u32) raw, " / Alloc DMA Block:", (u32) dma_block )
#endif 
                char** buffers = new char*[p_count];
                for (size_t i = 0; i < p_count; ++i)
                    {
                    buffers[i] = dma_block + i * bufferSize;
#ifdef ALLOC_DEBUG   
                storeLog( MY_BUFFER, MY_INDEX,  "ALLOC-DMA buffers[", (u32) i, "] -" (u32) sizeof(buffers[i]));
#endif   
                    memset(buffers[i], 0, bufferSize);
                    }
#ifdef ALLOC_DEBUG   
                storeLog( MY_BUFFER, MY_INDEX,  "ALLOC-DMA RAW", (u32) raw,
                                                "in Block", (u32) dma_block, 
                                                "Buffer Size", (u32) total_size, 
                                                "Aligned Size",  (u32) aligned_total_size);
#endif 
                *blockBaseOut = dma_block;
                *rawBlockOut = raw;
                *alignedSizeOut = aligned_total_size;

                msleep(100);
                return buffers;
}

void            CKernel::clearBufferMEM             (           char**      buffers, 
                                                                size_t      p_count) 
{
                for (size_t i = 0; i < p_count; ++i)
                    {
                    free(buffers[i]);
                    }
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
// END OF FILE
//------------------------------------------------------------------------------------------------------------------------------------------------ 341