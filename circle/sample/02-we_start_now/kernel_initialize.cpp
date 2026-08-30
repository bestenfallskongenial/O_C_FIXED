boolean         CKernel::Initialize (void)
{
                bool bOK = true;

                if (bOK)
                    {
                    bOK = frameBufferInit();
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
                //  m_Timer.MsDelay(1000);
                    }
                if (bOK)
                    {
                    bOK = m_VCHIQ.Initialize();
                    m_Timer.MsDelay(200);
                    }
                if (bOK)
                    {
                    bcm_host_init();
                //  m_Timer.MsDelay(200);
                    }               
                if (bOK)
                    {
                    initOGL   (&m_ogl);
                //  m_Timer.MsDelay(200);
                    }                     
               if (bOK)
                    {
                    bOK = my_SPI_init(); 
                    }
               if (bOK)
                    {
                    bOK = my_SMI_Init(LED_CTRL_PIN); 
                    }
               if (bOK)
                    {
                    bOK = my_WS2812_Init(LED_COUNT); 
                    }                                        
                if (bOK)
                    {
                    my_GPIO_SetAlt(MY_CTRL_PIN, 1, GPIO_PULL_OFF);
                    my_GPIO_Write(MY_CTRL_PIN, LOW);
                    }

                if (bOK)
                    {
                    my_GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP);
                    }

                if (bOK)
                    {
                    my_GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP);
                    }
                return bOK;
}