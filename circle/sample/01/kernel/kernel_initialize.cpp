// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include kernel.h
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean         CKernel::Initialize (void)
{
                bool bOK = true;

                if (bOK)
                    {
                    bOK = m_Screen.Initialize(); //
                    /*
                    q: IS MY OWN BUFFER TO SCREEN SUFFICIENT TO REPLACE CSCREEN?
                     */
                    }
                if (bOK)
                    {
                    bOK = m_Serial.Initialize(115200); //
                    /*
                    Q: REALY? IS THIS THE ONLY REASON TO HAVE SERIAL HERE?
                    */
                    }   
                if (bOK) // the idea is to remove the CScreen Class form our code, to much overhead, also  the CString code !!!
                    {
                    CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
                    if (pTarget == 0)
                    {
                        pTarget = &m_Screen;
                    }
                    bOK = m_Logger.Initialize(pTarget);  //  i want to get rid of this!
                    }   
                if (bOK)
                {
                    bOK = m_Interrupt.Initialize();
                    }
                if (bOK)
                    {
                    bOK = m_Timer.Initialize(); 
                    }
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize(); 
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize(); 
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    m_Timer.MsDelay(1000);  
                    }
                if (bOK)
                    {
                    bOK = wrapperInitDMA();
                    bOK = wrapperInitMEM();
                    }
                if (bOK)
                    {
                    bOK = wrapperInitVCSMstruct();
                    bOK = wrapperInitMMALstruct();
                    }                    
                if (bOK)
                    {
                    bOK = m_VCHIQ.Initialize();
                    m_Timer.MsDelay(200);
                    }
                if (bOK)
                {
                    bcm_host_init();  // we need to initialize the bcm_host before the graphics
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    initOGL   (&m_glsl); // we need to initialize the graphics before the shared memory
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                    {
                    bOK = m_SPIMaster.Initialize();
                    }

                return bOK;
                }
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------