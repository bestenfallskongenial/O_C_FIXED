#include "kernel.h"

//  #undef  __LOG_FILE__
    #define __LOG_FILE__

    #define MY_BUFFER   m_logBuffer                 // means the log goes into the pre-init buffer 
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
#ifdef __LOG_FILE__
                    storeLog(   MY_BUFFER, MY_INDEX, "Failed to Mount Device", EMPTYLOG, p_deviceName );
#endif                    
                    return false;
                    }

                if (m_pFileSystem == 0 || p_fileName == 0 || p_bufferArray == 0 || p_bufferSize == 0)
                    {
#ifdef __LOG_FILE__
                    storeLog(   MY_BUFFER, MY_INDEX, "Failed store Input", EMPTYLOG, "file", EMPTYLOG, p_fileName, EMPTYLOG, "size", (u32)p_bufferSize );
#endif
                    return false;
                    }

                g_hFile = m_pFileSystem->FileCreate(p_fileName);
                if (g_hFile == 0)
                    {
#ifdef __LOG_FILE__                        
                    storeLog(   MY_BUFFER, MY_INDEX, "Failed to create File", EMPTYLOG, p_fileName, EMPTYLOG, "on Device", EMPTYLOG, p_deviceName );
#endif                    
                    return false;
                    }

                if (m_pFileSystem->FileWrite(g_hFile, p_bufferArray, p_bufferSize) != p_bufferSize)
                    {
#ifdef __LOG_FILE__                        
                    storeLog(   MY_BUFFER, MY_INDEX, "Failed to store File", EMPTYLOG, p_fileName, EMPTYLOG, "size", (u32)p_bufferSize );
#endif                                      
                    return false;
                    }
#ifdef __LOG_FILE__                        
                storeLog(   MY_BUFFER, MY_INDEX, "File stored successful", EMPTYLOG, p_fileName, EMPTYLOG, "size", (u32)p_bufferSize );        
#endif
                closeFile();
                UnMount();

#ifdef __LOG_FILE__
                storeLog(   MY_BUFFER, MY_INDEX, "Successful Stored", EMPTYLOG, p_fileName, EMPTYLOG, "from Buffer size", (u32)p_bufferSize );
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
#ifdef __LOG_FILE__
                storeLog(   MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "BULKLOAD Start  scanned", (u32)p_maxFiles, "valid", (u32)p_validFiles, "  size", (u32)p_fileSize );
#endif

                for (unsigned i = 0; i < p_maxFiles; ++i) 
                    {
                    if (openFile(p_fileNameArray[i]))
                        {
                        unsigned f_bytesRead = loadToBuffer(p_bufferArray[p_validFiles], p_fileSize);
                        if (f_bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = f_bytesRead;
#ifdef __LOG_FILE__
                            storeLog(   MY_BUFFER, MY_INDEX, p_fileNameArray[i], EMPTYLOG, "bytes read", (u32)f_bytesRead, "in buffer [", (u32)p_validFiles, "]" ); // new
#endif                            
                            p_validFiles++;   
                            }
                        closeFile();
                        }
                    }
#ifdef __LOG_FILE__
                storeLog(   MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "BULKLOAD End       prev", (u32)p_prevFiles, "  new", (u32)p_validFiles, "loaded", (u32)(p_validFiles - p_prevFiles) );
                nextline( MY_BUFFER, MY_INDEX );
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
/*
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
*/
bool            CKernel::scanRoot                   (           char**      p_fileNameArray,
                                                        const   char*       p_fileExtArray[],
                                                                unsigned    p_extentionCount,
                                                                unsigned&   p_scannedFiles,
                                                                unsigned    p_maxFiles,
                                                                unsigned    p_prevFiles )                 // *** CHANGED
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
                                unsigned dst = p_prevFiles + p_scannedFiles;             // *** CHANGED
                                p_fileNameArray[dst] = new char[strlen(f_directoryEntry.chTitle) + 1]; // *** CHANGED
                                strcpy(p_fileNameArray[dst], f_directoryEntry.chTitle);  // *** CHANGED
                                p_scannedFiles++;
                                break;
                                }
                            }
                        }
                    f_nextEntry = m_pFileSystem->RootFindNext(&f_directoryEntry, &f_currentDirectoryEntry);
                    }
                return true;
}

char*           CKernel::gen83FileName             (   const char*     ext )
{
                static const char map[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

                m_83FileName[0] = map[g_inOutMatrixInt[0][RND] & 31];
                m_83FileName[1] = map[g_inOutMatrixInt[1][RND] & 31];
                m_83FileName[2] = map[g_inOutMatrixInt[2][RND] & 31];
                m_83FileName[3] = map[g_inOutMatrixInt[3][RND] & 31];
                m_83FileName[4] = map[g_inOutMatrixInt[4][RND] & 31];
                m_83FileName[5] = map[g_inOutMatrixInt[5][RND] & 31];
                m_83FileName[6] = map[g_inOutMatrixInt[6][RND] & 31];
                m_83FileName[7] = map[g_inOutMatrixInt[7][RND] & 31];

                m_83FileName[8]  = '.';
                m_83FileName[9]  = ext[0];
                m_83FileName[10] = ext[1];
                m_83FileName[11] = ext[2];
                m_83FileName[12] = '\0';

                return m_83FileName;
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