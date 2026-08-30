//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::wrapperInitDMA()
{
                bool bOK = true;

                if (bOK)
                    {
                    bOK = (m_bufferVid = allocBufferDMA  (  filecounter[FT_VID][FLD_MAXSD]+filecounter[FT_VID][FLD_MAXUSB], filecounter[FT_VID][FLD_SIZE], &m_videoBlockBase, &m_videoRawBlock, &m_videoBlockSize));
                    }
                if (bOK)
                    {
                    bOK = (m_bufferFrA = allocBufferDMA  (  filecounter[FRM_BF][FLD_MAXSD]+filecounter[FRM_BF][FLD_MAXUSB], filecounter[FRM_BF][FLD_SIZE], &m_frameBlockBaseA, &m_frameRawBlockA, &m_frameBlockSizeA));
                    }
                if (bOK)
                    {
                    bOK = (m_bufferFrB = allocBufferDMA  (  filecounter[FRM_BF][FLD_MAXSD]+filecounter[FRM_BF][FLD_MAXUSB], filecounter[FRM_BF][FLD_SIZE], &m_frameBlockBaseB, &m_frameRawBlockB, &m_frameBlockSizeB));
                    }
                if (bOK)
                    {
                    bOK = (m_bufferOmt = allocBufferDMA  (  filecounter[FT_OMT][FLD_MAXSD]+filecounter[FT_OMT][FLD_MAXUSB], filecounter[FT_OMT][FLD_SIZE], &m_overlyBlockBase, &m_overlayRawBlock, &m_overlyBlockSize));
                    }
                if (bOK)
                    {
                    bOK = (m_bufferTex = allocBufferDMA  (  filecounter[FT_TEX][FLD_MAXSD]+filecounter[FT_TEX][FLD_MAXUSB], filecounter[FT_TEX][FLD_SIZE], &m_textureBlockBase, &m_textureRawBlock, &m_textureBlockSize));
                    }
                return bOK;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::wrapperInitMEM()
{
                bool bOK = true;

                if (bOK)
                    {
                    bOK = (m_bufferKnl = allocBufferMEM( filecounter[FT_KLN][FLD_MAXSD]+filecounter[FT_KLN][FLD_MAXUSB], filecounter[FT_KLN][FLD_SIZE]));       // same as allocBufferMEM( KLN_SD + KLN_USB, filecounter[FT_KLN][FLD_SIZE]));
                    }
                if (bOK)
                    {
                    bOK = (m_bufferLog = allocBufferMEM( filecounter[LOGGER][FLD_MAXSD]+filecounter[LOGGER][FLD_MAXUSB], filecounter[LOGGER][FLD_SIZE]));      // 1024 *64 <- the new buffer for log/text files <- saveFromBuffer() <-
                    }                   
                if (bOK)
                    {
                    bOK = (m_bufferVsh = allocBufferMEM( filecounter[FT_VSH][FLD_MAXSD]+filecounter[FT_VSH][FLD_MAXUSB], filecounter[FT_VSH][FLD_SIZE]));
                    }
                if (bOK)
                    {
                    bOK = (m_bufferOmf = allocBufferMEM( filecounter[FT_OMF][FLD_MAXSD]+filecounter[FT_OMF][FLD_MAXUSB], filecounter[FT_OMF][FLD_SIZE]));
                    }
                if (bOK)
                    {
                    bOK = (m_bufferFsh = allocBufferMEM( filecounter[FT_FSH][FLD_MAXSD]+filecounter[FT_FSH][FLD_MAXUSB], filecounter[FT_FSH][FLD_SIZE]));
                    }
                return bOK;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapperDMAcleanUp            ()
{
                    clearBufferDMA( m_bufferVid, m_videoRawBlock); 

                    clearBufferDMA( m_bufferFrA, m_frameRawBlockA); 

                    clearBufferDMA( m_bufferFrB, m_frameRawBlockB); 

                    clearBufferDMA( m_bufferOmt, m_overlayRawBlock); 

                    clearBufferDMA( m_bufferTex, m_textureRawBlock); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapperMEMcleanUp            ()
{
                    clearBufferMEM( m_bufferKnl, filecounter[FT_KLN][FLD_MAXSD]+filecounter[FT_KLN][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferLog, filecounter[LOGGER][FLD_MAXSD]+filecounter[LOGGER][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferVsh, filecounter[FT_VSH][FLD_MAXSD]+filecounter[FT_VSH][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferOmf, filecounter[FT_OMF][FLD_MAXSD]+filecounter[FT_OMF][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferFsh, filecounter[FT_FSH][FLD_MAXSD]+filecounter[FT_FSH][FLD_MAXUSB] );        
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void                CKernel::wrapper_from_sd()
{
                if(Mount( PARTITION_NAME_SD ))
                    {
                    scanRoot                (   g_ScnVsh,                           // where we store the valid filenames we find
                                                g_SufVsh,                           // the array of valid file extensions for this type of file
                                                filecounter[FT_VSH][FLD_EXTCNT],                         // how many valid file extensions we have in the array above also part of filecounter?
                                                filecounter[FT_VSH][FLD_SCANNED],         
                                                filecounter[FT_VSH][FLD_MAXSD]);           // how many files are allowed to scan and stored in the array
                    
                    scanRoot                (   g_ScnOmf, 
                                                g_SufOmf, 
                                                filecounter[FT_OMF][FLD_EXTCNT], 
                                                filecounter[FT_OMF][FLD_SCANNED], 
                                                filecounter[FT_OMF][FLD_MAXSD]);                
                    
                    scanRoot                (   g_ScnFsh, 
                                                g_SufFsh, 
                                                filecounter[FT_FSH][FLD_EXTCNT], 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_MAXSD]);
                    
                    scanRoot                (   g_ScnOmt, 
                                                g_SufOmt, 
                                                filecounter[FT_OMT][FLD_EXTCNT], 
                                                filecounter[FT_OMT][FLD_SCANNED], 
                                                filecounter[FT_OMT][FLD_MAXSD]);                
/*                    
                    scanRoot                (   g_ScnTex, 
                                                g_SufTex, 
                                                filecounter[FT_TEX][FLD_EXTCNT], 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_MAXSD]);
                   
                    scanRoot                (   g_ScnVid, 
                                                g_SufVid, 
                                                filecounter[FT_VID][FLD_EXTCNT], 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_MAXSD]);
*/ 
                    scanRoot                (   g_ScnKln, 
                                                g_SufKln, 
                                                filecounter[FT_KLN][FLD_EXTCNT], 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_MAXSD]);    

                    bulkLoad                (   g_ScnVsh,                           // where we have stored the filenames 
                                                g_bytVsh,                           // where we store the loaded bytes for each file 
                                                m_bufferVsh,                        // where we store the loaded file data for each file
                                                filecounter[FT_VSH][FLD_SCANNED],          // how many files we are allowed to process
                                                filecounter[FT_VSH][FLD_LOADED],            // <- is directly modified in the function, we dont need to return it
                                                filecounter[FT_VSH][FLD_PREV],
                                                filecounter[FT_VSH][FLD_SIZE]);                     // maximum size for each file
                                                                            
                    bulkLoad                (   g_ScnOmf, 
                                                g_bytOmf, 
                                                m_bufferOmf, 
                                                filecounter[FT_OMF][FLD_SCANNED], 
                                                filecounter[FT_OMF][FLD_LOADED], 
                                                filecounter[FT_OMF][FLD_PREV],
                                                filecounter[FT_OMF][FLD_SIZE]);
    
                    bulkLoad                (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_LOADED], 
                                                filecounter[FT_FSH][FLD_PREV],
                                                filecounter[FT_FSH][FLD_SIZE]);                           

                    bulkLoad                (   g_ScnOmt, 
                                                g_bytOmt, 
                                                m_bufferOmt, 
                                                filecounter[FT_OMT][FLD_SCANNED], 
                                                filecounter[FT_OMT][FLD_LOADED], 
                                                filecounter[FT_OMT][FLD_PREV],
                                                filecounter[FT_OMT][FLD_SIZE]);                                                                                          
 /*
                    bulkLoad                (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_LOADED], 
                                                filecounter[FT_TEX][FLD_PREV],
                                                filecounter[FT_TEX][FLD_SIZE]);                                   
 
                    bulkLoad                (   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_LOADED], 
                                                filecounter[FT_VID][FLD_PREV],
                                                filecounter[FT_VID][FLD_SIZE]);   
*/
                    bulkLoad                (   g_ScnKln, 
                                                g_bytKln, 
                                                m_bufferKnl, 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_LOADED], 
                                                filecounter[FT_KLN][FLD_PREV],
                                                filecounter[FT_KLN][FLD_SIZE]); 

                    UnMount();   
                    }
                // Flush CPU->RAM so the VPU sees the loaded bitstream
                CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_load_usb()
{
                if(Mount( PARTITION_NAME_USB ))
                    {
                    scanRoot                (   g_ScnFsh, 
                                                g_SufFsh, 
                                                filecounter[FT_FSH][FLD_EXTCNT], 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_MAXUSB]);   
                    scanRoot                (   g_ScnTex, 
                                                g_SufTex, 
                                                filecounter[FT_TEX][FLD_EXTCNT], 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_MAXUSB]);
                    scanRoot                (   g_ScnVid, 
                                                g_SufVid, 
                                                filecounter[FT_VID][FLD_EXTCNT], 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_MAXUSB]);
                    scanRoot                (   g_ScnKln, 
                                                g_SufKln, 
                                                filecounter[FT_KLN][FLD_EXTCNT], 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_MAXUSB]);                                                  
        
                    bulkLoad                (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_LOADED], 
                                                filecounter[FT_FSH][FLD_PREV],
                                                filecounter[FT_FSH][FLD_SIZE]);                           

                    bulkLoad                (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_LOADED], 
                                                filecounter[FT_TEX][FLD_PREV],
                                                filecounter[FT_TEX][FLD_SIZE]);                                   

                    bulkLoad                (   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_LOADED], 
                                                filecounter[FT_VID][FLD_PREV],
                                                filecounter[FT_VID][FLD_SIZE]);    
                    bulkLoad                (   g_ScnKln, 
                                                g_bytKln, 
                                                m_bufferKnl, 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_LOADED], 
                                                filecounter[FT_KLN][FLD_PREV],
                                                filecounter[FT_KLN][FLD_SIZE]);                                                 
                    UnMount();   
                    }
                // Flush CPU->RAM so the VPU sees the loaded bitstream
                CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_init_gl_sd()
{
                initBMPparser   (   &m_omt,                                         // the dedicated struct for the overlay texture
                                    m_bufferOmt,                                    // the actual mem-buffer where i have stored it
                                    g_bytOmt,                                       // the array where i stored the loaded bytes
                                    filecounter[FT_TEX][FLD_SIZE],                  // upper bound for the size
                                    filecounter[FT_OMT][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_OMT][FLD_LOADED]);               // for the continuous loading between devices - upper bound

                ParseBPM        (   &m_omt,
                                    g_ScnOmt,
                                    filecounter[FT_TEX][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_TEX][FLD_LOADED]);               // for the continuous loading between devices - upper bound

                initVbuffer     (   &m_ogl,
                                    &m_vtx ) 

                initShader      (   &m_vtx,                                         // init vertex shader 
                                    &m_vsh, 
                                    &m_tex,
                                    m_bufferVsh,
                                    filecounter[FT_VSH][FLD_PREV],                  // should be 0
                                    filecounter[FT_VSH][FLD_LOADED],                // should be 1 - BUT the code should also ensure that filecounter contains the correct values
                                    GL_VERTEX_SHADER);

                initShader      (   &m_vtx,                                         // init fragment shader ( default )
                                    &m_fsh, 
                                    &m_tex,
                                    m_bufferFsh,                                    // the actual mem-buffer where i have stored it
                                    filecounter[FT_FSH][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_FSH][FLD_LOADED],                // for the continuous loading between devices - upper bound
                                    GL_FRAGMENT_SHADER);          

                initShader      (   &m_vtx,                                         // init Overlay shader
                                    &m_osh, 
                                    &m_omt,
                                    m_bufferOmf,                                    // the actual mem-buffer where i have stored it
                                    filecounter[FT_OMF][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_OMF][FLD_LOADED],                // for the continuous loading between devices - upper bound
                                    GL_FRAGMENT_SHADER);

                initProgram     (   &m_vtx,                                         // user fragment shaders
                                    &m_vsh,
                                    &m_fsh,
                                    &m_tex,
                                    filecounter[FT_FSH][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_FSH][FLD_LOADED],                // for the continuous loading between devices - upper bound
                                    filecounter[FT_FSH][FLD_VALID]);                // for the dense indexing after load and verify
                                
                initProgram     (   &m_vtx,                                         // overlay fragment shader 
                                    &m_vsh,
                                    &m_osh,
                                    &m_omt, 
                                    filecounter[FT_OMF][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_OMF][FLD_LOADED],                // for the continuous loading between devices - upper bound
                                    filecounter[FT_OMF][FLD_VALID]);                // for the dense indexing after load and verify

                initUniform     (   &m_vtx,
                                    &m_fsh,
                                    &m_tex,
                                    0,                                              // why not simply change the call signature? 
                                    filecounter[FT_FSH][FLD_VALID]);                // for the dense indexing after load and verify *

                initUniform     (   &m_vtx,
                                    &m_osh,
                                    &m_omt,
                                    0,
                                    filecounter[FT_OMF][FLD_VALID]);                // for the dense indexing after load and verify *
/*
                initTexture     (   &m_vtx,                                         // no textures on sd!?
                                    &m_fsh,
                                    &m_tex,
                                    m_bufferTex,                                    // the actual mem-buffer where i have stored it
                                    filecounter[FT_TEX][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_TEX][FLD_LOADED],                // for the continuous loading between devices - upper bound
                                    filecounter[FT_TEX][FLD_VALID],                 // for the dense indexing after load and verify *
                                    GL_REPEAT,
                                    GL_REPEAT);
*/
                initTexture     (   &m_vtx,                                         // my overlay texture
                                    &m_osh,
                                    &m_omt,
                                    m_bufferOmt,                                    // the actual mem-buffer where i have stored it
                                    filecounter[FT_OMT][FLD_PREV],                  // for the continuous loading between devices - lower bound
                                    filecounter[FT_OMT][FLD_LOADED],                // for the continuous loading between devices - upper bound
                                    filecounter[FT_OMT][FLD_VALID],                 // for the dense indexing after load and verify *
                                    GL_CLAMP_TO_EDGE,
                                    GL_CLAMP_TO_EDGE);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_init_gl_usb()
{
                initBMPparser   (   &m_tex,
                                    m_bufferTex,
                                    g_bytTex,
                                    filecounter[FT_TEX][FLD_SIZE],
                                    filecounter[FT_TEX][FLD_PREV],
                                    filecounter[FT_TEX][FLD_LOADED]);                

                initH264parser  (   &m_vid,
                                    m_videoBlockBase,
                                    m_bufferVid,
                                    g_bytVid,
                                    filecounter[FT_VID][FLD_PREV],
                                    filecounter[FT_VID][FLD_LOADED],
                                    MAX_VIDEO_WIDTH,
                                    MAX_VIDEO_HEIGHT,
                                    MAX_VIDEO_PROFILE,
                                    MAX_VIDEO_LEVEL);

                ParseBPM        (   &m_tex,
                                    g_ScnTex,
                                    filecounter[FT_TEX][FLD_PREV],
                                    filecounter[FT_TEX][FLD_LOADED]);

                ParseAnnexB     (   &m_vid,
                                    g_bytVid,
                                    filecounter[FT_VID][FLD_PREV],
                                    filecounter[FT_VID][FLD_LOADED]);

                initShader      (   &m_vtx,         // init fragment shader ( default )
                                    &m_fsh, 
                                    &m_tex,
                                    m_bufferFsh,
                                    filecounter[FT_FSH][FLD_PREV],
                                    filecounter[FT_FSH][FLD_LOADED],
                                    GL_FRAGMENT_SHADER);    

                initProgram     (   &m_vtx,         // user fragment shaders
                                    &m_vsh,
                                    &m_fsh,
                                    &m_tex,
                                    filecounter[FT_FSH][FLD_PREV],
                                    filecounter[FT_FSH][FLD_LOADED],
                                    filecounter[FT_FSH][FLD_VALID]);            

                initUniform     (   &m_vtx,
                                    &m_fsh,
                                    &m_tex,
                                    0,                                  // why not simply change the call signature? 
                                    filecounter[FT_FSH][FLD_VALID]);            

                initTexture     (   &m_vtx,
                                    &m_fsh,
                                    &m_tex,
                                    m_bufferTex,
                                    filecounter[FT_TEX][FLD_PREV],
                                    filecounter[FT_TEX][FLD_LOADED],
                                    filecounter[FT_TEX][FLD_VALID],
                                    GL_REPEAT,
                                    GL_REPEAT);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_io()
{
                readADC();                  //  we read and dampen the adc each loop

                chooseIndexD( ADC_SELECT_PRG, m_activePrg, filecounter[FT_FSH][FLD_VALID])      // we derive the 
                chooseIndexD( ADC_SELECT_TEX, m_activeTex, filecounter[FT_TEX][FLD_VALID])
                chooseIndexD( ADC_SELECT_VID, m_activeVid, filecounter[FT_VID][FLD_VALID])

                buttonPing( 0, SW_PIN_A);                  // check button A
                buttonPing( 1, SW_PIN_B);                  // and B

                button_consumer(0);               // and transpose the button input in menulayer information each time
                button_consumer(1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void    CKernel::wrapper_modes()
{
        resetPickUpFlags();                              // we need to reset the threshold flags each loop - why again?!

        switch (g_current_menu_layer)                           // use than the menulayer variable the 
            {
            case 0:
            // case zero is button b ( lower ) tab bpm
                break;            
            case 1:
                modeMenuAssignGroup(1,  0);     // CH0–CH3 parameters in our global array depending on the menulayer
                break;
            case 2:
                modeMenuAssignGroup(2,  4);     // CH4–CH7
                break;
            case 3:
                modeMenuAssignGroup(3,  8);     // LFO
                break;
            case 4:
                modeMenuAssignGroup(4, 12);     // sensitivity layer A..D
                break;

            default:
                break;
            }
        setChannelMode(0);                               // than we apply the mapped modes for the 8 channels
        setChannelMode(1);                               // other menulayer modes are handled in the background
        setChannelMode(2);
        setChannelMode(3);
        setChannelMode(4);
        setChannelMode(5);
        setChannelMode(6);
        setChannelMode(7);

        apply_state_to_led();                                   // than we update the 4 leds depending on the modes - we have to write this function 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool    CKernel::wrapper_VCSM()   // for CKernel::Initialize()
{
                bool bOK = true;

                if (bOK)
                    {
                    bOK = initializeVCSM(); 
                    }
                if (bOK)
                    {
                    bOK = importMemoryVCSM  (   m_videoBlockBase, 
                                                m_videoBlockSize, 
                                                0, 
                                                VCSM_Import_MEM_Msg& tx, 
                                                VCSM_Import_MEM_Reply& rx);
                    bOK = lockMemoryVCSM    (   0, 
                                                CSM_Lock_MEM_Msg&              tx, 
                                                VCSM_Lock_MEM_Reply&            rx);                    
                    }
                if (bOK)
                    {
                    bOK = importMemoryVCSM  (   m_frameBlockBaseA, 
                                                m_frameBlockSizeA, 
                                                1, 
                                                VCSM_Import_MEM_Msg& tx, 
                                                VCSM_Import_MEM_Reply& rx);                    
                    bOK = lockMemoryVCSM    (   1, 
                                                VCSM_Lock_MEM_Msg&              tx, 
                                                VCSM_Lock_MEM_Reply&            rx);
                    }
                if (bOK)
                    {
                    bOK = importMemoryVCSM   (  m_frameBlockBaseB, 
                                                m_frameBlockSizeB, 
                                                2, 
                                                VCSM_Import_MEM_Msg& tx, 
                                                VCSM_Import_MEM_Reply& rx);                    
                    bOK = lockMemoryVCSM    (   2, 
                                                VCSM_Lock_MEM_Msg&              tx, 
                                                VCSM_Lock_MEM_Reply&            rx);                    
                    }

                return bOK;                    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// instead of having life time long structs for my vcsm / mmal i declare pointer instead and provide wrappers to alloc and free the structs after use!
// means also i have to call the wrapper here at the CKernel init phase ( presumably after the memory alloc ) and after the init phase of the vc04 

bool            CKernel::wrapperInitVCSMstruct()
{
                bool bOK = true;

                if (bOK) { m_ServiceCreateVCSM = new SERVICE_CREATION_T();           bOK = (m_ServiceCreateVCSM != nullptr); }
                if (bOK) { m_importTxVCSM_A    = new VCSM_Import_MEM_Msg();          bOK = (m_importTxVCSM_A != nullptr); }
                if (bOK) { m_importRxVCSM_A    = new VCSM_Import_MEM_Reply();        bOK = (m_importRxVCSM_A != nullptr); }
                if (bOK) { m_importTxVCSM_B    = new VCSM_Import_MEM_Msg();          bOK = (m_importTxVCSM_B != nullptr); }
                if (bOK) { m_importRxVCSM_B    = new VCSM_Import_MEM_Reply();        bOK = (m_importRxVCSM_B != nullptr); }
                if (bOK) { m_importTxVCSM_C    = new VCSM_Import_MEM_Msg();          bOK = (m_importTxVCSM_C != nullptr); }
                if (bOK) { m_importRxVCSM_C    = new VCSM_Import_MEM_Reply();        bOK = (m_importRxVCSM_C != nullptr); }
                if (bOK) { m_lockTxVCSM        = new VCSM_Lock_MEM_Msg();            bOK = (m_lockTxVCSM != nullptr); }
                if (bOK) { m_lockRxVCSM        = new VCSM_Lock_MEM_Reply();          bOK = (m_lockRxVCSM != nullptr); }
                if (bOK) { m_freeTxVCSM        = new VCSM_Free_MEM_Msg();            bOK = (m_freeTxVCSM != nullptr); }
                if (bOK) { m_freeRxVCSM        = new VCSM_Free_MEM_Reply();          bOK = (m_freeRxVCSM != nullptr); }

                return bOK;
}

bool            CKernel::wrapperInitMMALstruct()
{
                bool bOK = true;

                if (bOK) { m_ServiceCreateMMAL      = new SERVICE_CREATION_T();              bOK = (m_ServiceCreateMMAL != nullptr); }

                if (bOK) { m_ComponentCreateTx      = new MMAL_Component_Create_Msg();       bOK = (m_ComponentCreateTx != nullptr); }
                if (bOK) { m_ComponentCreateRx      = new MMAL_Component_Create_Reply();     bOK = (m_ComponentCreateRx != nullptr); }

                if (bOK) { m_PortInfoGetTx_Input_A  = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Input_A != nullptr); }
                if (bOK) { m_PortInfoGetRx_Input_A  = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Input_A != nullptr); }

                if (bOK) { m_PortInfoGetTx_Output_A = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Output_A != nullptr); }
                if (bOK) { m_PortInfoGetRx_Output_A = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Output_A != nullptr); }

                if (bOK) { m_PortInfoSetTx_Input    = new MMAL_Port_Info_Set_Msg();         bOK = (m_PortInfoSetTx_Input != nullptr); }
                if (bOK) { m_PortInfoSetTx_Output   = new MMAL_Port_Info_Set_Msg();         bOK = (m_PortInfoSetTx_Output != nullptr); }

                if (bOK) { m_PortInfoSetRx_Input    = new MMAL_Port_Info_Set_Reply();       bOK = (m_PortInfoSetRx_Input != nullptr); }
                if (bOK) { m_PortInfoSetRx_Output   = new MMAL_Port_Info_Set_Reply();       bOK = (m_PortInfoSetRx_Output != nullptr); }

                if (bOK) { m_ComponentEnableTx      = new MMAL_Component_Enable_Msg();      bOK = (m_ComponentEnableTx != nullptr); }
                if (bOK) { m_ComponentEnableRx      = new MMAL_Component_Enable_Reply();    bOK = (m_ComponentEnableRx != nullptr); }

                if (bOK) { m_PortInfoGetTx_Input_B  = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Input_B != nullptr); }
                if (bOK) { m_PortInfoGetRx_Input_B  = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Input_B != nullptr); }

                if (bOK) { m_PortInfoGetTx_Output_B = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Output_B != nullptr); }
                if (bOK) { m_PortInfoGetRx_Output_B = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Output_B != nullptr); }

                if (bOK) { m_PortParamTx_Input      = new MMAL_Port_Parameter_Set_Msg();    bOK = (m_PortParamTx_Input != nullptr); }
                if (bOK) { m_PortParamRx_Input      = new MMAL_Port_Parameter_Set_Reply();  bOK = (m_PortParamRx_Input != nullptr); }

                if (bOK) { m_PortParamTx_Output     = new MMAL_Port_Parameter_Set_Msg();    bOK = (m_PortParamTx_Output != nullptr); }
                if (bOK) { m_PortParamRx_Output     = new MMAL_Port_Parameter_Set_Reply();  bOK = (m_PortParamRx_Output != nullptr); }

                if (bOK) { m_PortInfoGetTx_Input_C  = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Input_C != nullptr); }
                if (bOK) { m_PortInfoGetRx_Input_C  = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Input_C != nullptr); }

                if (bOK) { m_PortInfoGetTx_Output_C = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Output_C != nullptr); }
                if (bOK) { m_PortInfoGetRx_Output_C = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Output_C != nullptr); }

                if (bOK) { m_PortActionTx_Input     = new MMAL_Port_Action_Msg();           bOK = (m_PortActionTx_Input != nullptr); }
                if (bOK) { m_PortActionRx_Input     = new MMAL_Port_Action_Reply_Msg();     bOK = (m_PortActionRx_Input != nullptr); }

                if (bOK) { m_PortActionTx_Output    = new MMAL_Port_Action_Msg();           bOK = (m_PortActionTx_Output != nullptr); }
                if (bOK) { m_PortActionRx_Output    = new MMAL_Port_Action_Reply_Msg();     bOK = (m_PortActionRx_Output != nullptr); }

                if (bOK) { m_BufferFromHostTx_Input = new MMAL_Buffer_From_Host_Msg();      bOK = (m_BufferFromHostTx_Input != nullptr); }
                if (bOK) { m_BufferFromHostRx_Input = new MMAL_Buffer_From_Host_Msg();      bOK = (m_BufferFromHostRx_Input != nullptr); }

                if (bOK) { m_BufferFromHostTx_Output = new MMAL_Buffer_From_Host_Msg();     bOK = (m_BufferFromHostTx_Output != nullptr); }
                if (bOK) { m_BufferFromHostRx_Output = new MMAL_Buffer_From_Host_Msg();     bOK = (m_BufferFromHostRx_Output != nullptr); }

                if (bOK) { m_PortInfoGetTx_Input_D  = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Input_D != nullptr); }
                if (bOK) { m_PortInfoGetRx_Input_D  = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Input_D != nullptr); }

                if (bOK) { m_PortInfoGetTx_Output_D = new MMAL_Port_Info_Get_Msg();         bOK = (m_PortInfoGetTx_Output_D != nullptr); }
                if (bOK) { m_PortInfoGetRx_Output_D = new MMAL_Port_Info_Get_Reply();       bOK = (m_PortInfoGetRx_Output_D != nullptr); }

                return bOK;
}

void            CKernel::wrapperFreeVCSMstruct() // here i must check what structs are init/debug and what are runtime code!
{
                delete m_ServiceCreateVCSM; m_ServiceCreateVCSM = nullptr;

                delete m_importTxVCSM_A;    m_importTxVCSM_A = nullptr;
                delete m_importRxVCSM_A;    m_importRxVCSM_A = nullptr;

                delete m_importTxVCSM_B;    m_importTxVCSM_B = nullptr;
                delete m_importRxVCSM_B;    m_importRxVCSM_B = nullptr;

                delete m_importTxVCSM_C;    m_importTxVCSM_C = nullptr;
                delete m_importRxVCSM_C;    m_importRxVCSM_C = nullptr;

                delete m_lockTxVCSM;        m_lockTxVCSM = nullptr;
                delete m_lockRxVCSM;        m_lockRxVCSM = nullptr;

                delete m_freeTxVCSM;        m_freeTxVCSM = nullptr;
                delete m_freeRxVCSM;        m_freeRxVCSM = nullptr;
}

void            CKernel::wrapperFreeMMALstruct() // here i must check what structs are init/debug and what are runtime code!
{
                delete m_ServiceCreateMMAL;       m_ServiceCreateMMAL = nullptr;

                delete m_ComponentCreateTx;       m_ComponentCreateTx = nullptr;
                delete m_ComponentCreateRx;       m_ComponentCreateRx = nullptr;

                delete m_PortInfoGetTx_Input_A;   m_PortInfoGetTx_Input_A = nullptr;
                delete m_PortInfoGetRx_Input_A;   m_PortInfoGetRx_Input_A = nullptr;

                delete m_PortInfoGetTx_Output_A;  m_PortInfoGetTx_Output_A = nullptr;
                delete m_PortInfoGetRx_Output_A;  m_PortInfoGetRx_Output_A = nullptr;

                delete m_PortInfoSetTx_Input;     m_PortInfoSetTx_Input = nullptr;
                delete m_PortInfoSetTx_Output;    m_PortInfoSetTx_Output = nullptr;

                delete m_PortInfoSetRx_Input;     m_PortInfoSetRx_Input = nullptr;
                delete m_PortInfoSetRx_Output;    m_PortInfoSetRx_Output = nullptr;

                delete m_ComponentEnableTx;       m_ComponentEnableTx = nullptr;
                delete m_ComponentEnableRx;       m_ComponentEnableRx = nullptr;

                delete m_PortInfoGetTx_Input_B;   m_PortInfoGetTx_Input_B = nullptr;
                delete m_PortInfoGetRx_Input_B;   m_PortInfoGetRx_Input_B = nullptr;

                delete m_PortInfoGetTx_Output_B;  m_PortInfoGetTx_Output_B = nullptr;
                delete m_PortInfoGetRx_Output_B;  m_PortInfoGetRx_Output_B = nullptr;

                delete m_PortParamTx_Input;       m_PortParamTx_Input = nullptr;
                delete m_PortParamRx_Input;       m_PortParamRx_Input = nullptr;

                delete m_PortParamTx_Output;      m_PortParamTx_Output = nullptr;
                delete m_PortParamRx_Output;      m_PortParamRx_Output = nullptr;

                delete m_PortInfoGetTx_Input_C;   m_PortInfoGetTx_Input_C = nullptr;
                delete m_PortInfoGetRx_Input_C;   m_PortInfoGetRx_Input_C = nullptr;

                delete m_PortInfoGetTx_Output_C;  m_PortInfoGetTx_Output_C = nullptr;
                delete m_PortInfoGetRx_Output_C;  m_PortInfoGetRx_Output_C = nullptr;

                delete m_PortActionTx_Input;      m_PortActionTx_Input = nullptr;
                delete m_PortActionRx_Input;      m_PortActionRx_Input = nullptr;

                delete m_PortActionTx_Output;     m_PortActionTx_Output = nullptr;
                delete m_PortActionRx_Output;     m_PortActionRx_Output = nullptr;

                delete m_BufferFromHostTx_Input;  m_BufferFromHostTx_Input = nullptr;
                delete m_BufferFromHostRx_Input;  m_BufferFromHostRx_Input = nullptr;

                delete m_BufferFromHostTx_Output; m_BufferFromHostTx_Output = nullptr;
                delete m_BufferFromHostRx_Output; m_BufferFromHostRx_Output = nullptr;

                delete m_PortInfoGetTx_Input_D;   m_PortInfoGetTx_Input_D = nullptr;
                delete m_PortInfoGetRx_Input_D;   m_PortInfoGetRx_Input_D = nullptr;

                delete m_PortInfoGetTx_Output_D;  m_PortInfoGetTx_Output_D = nullptr;
                delete m_PortInfoGetRx_Output_D;  m_PortInfoGetRx_Output_D = nullptr;
}
