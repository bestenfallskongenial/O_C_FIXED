	#include "kernel.h"

				CKernel::CKernel						(	void )
				:	m_Interrupt         (),
                    m_Timer             (   &m_Interrupt ),
                    m_Logger            (   LOGLEVEL, &m_Timer ),
                    gE_FrameBuffer      (   m_Options.GetWidth(), 
                                            m_Options.GetHeight(), 
                                            32, 
                                            0, 
                                            TRUE ),

                    m_SMITxDMA          (   DMA_CHANNEL_LITE ),      // DMA_CHANNEL_LITE comes from machineinfo.h
                    m_EMMC              (   &m_Interrupt, &m_Timer,     &m_ActLED ),
                    m_USBHCI            (   &m_Interrupt, &m_Timer,     TRUE ),

                    m_VCHIQ             (   CMemorySystem::Get (), &m_Interrupt),
                    m_bStorageAttached  (   FALSE ),
                    m_pFileSystem       (   0 ),
                    m_resetFlag         (   false )
{                    
                m_ActLED.Blink(1);
}

				CKernel::~CKernel						(	void )
{
}

void 			CKernel::LoggerSink						(	        void* 			    pContext, 
															const   char* 	            pText, 
															        unsigned 		    nLength )
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logBufferIndex >= LOG_SIZ - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logBuffer[pThis->m_logBufferIndex++] = ch; // means the log goes into the pre-init buffer 
                    }
            //  pThis->m_logBuffer[pThis->m_logBufferIndex++] = '\n';
                pThis->m_logBuffer[pThis->m_logBufferIndex] = '\0'; // means the log goes into the pre-init buffer 

                pThis->logScreenUpdate();
}