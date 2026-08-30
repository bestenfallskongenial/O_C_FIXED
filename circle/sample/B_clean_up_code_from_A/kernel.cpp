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