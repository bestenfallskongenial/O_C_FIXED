
okay, here now the plan - we need to connect the loose ends
- there was the plan to get rid of CString ( #include <circle/string.h> ) screen ( #include <circle/screen.h>) and the logger ( #include <circle/logger.h> )
- to replace it with a mechanism that logs into a buffer and either write it to a file or show it on screen
new functions here are:

saveToBuffer() and buffer_to_screen_*()

- to integrate a texture atlas driven menu running as a second pass fragment shader:

menu.fsh
initOshader()
initOprogram()
initOuniforms()
initOtexture() 

with an independent set of uniforms and parameters, look at the file circle/sample/04-oculus_close_and_closer/the functions.cpp 

driven by an unified array g_centralModeBuffer[][] ( the old g_centralModeBuffer ) where we store all variables/modes/settings for each channel

- i also combined the input/mode/output values into a single matrix g_inOutMatrixInt[][] and g_inOutMatrixFlt[][]

and i want to do it with the timing related arrays from circle/sample/01-oculus_mendax/global.h too, like in features.cpp

also do i create wrappers for the different steps inside of the CKernel::Run() to organise the steps load - init and io - apply modes - render  better
i have also restructure some of the functions ( i guess i can do more in filesystem.cpp and init_gfx.cpp that i ay not need to pass as much parameters )

as said in  need to get this all together, means for example create the propper kernel.h - arrays variables etc, check for consistency 
create the logic how to navigate the menus circle/sample/04-oculus_close_and_closer/menu_final.cpp - mainly menu_id and buttonPing

the idea is to have my two buttons as followed ( important note here! when no button is pressed the physical input goes into the adc, 
    
is a button pressed long the four pots are routed into the first four adc channels, look at circle/sample/01-oculus_mendax/io.cpp how i did it in the past ):

on single tab event on the upper button is counted as potential tab bpm, means we need to store it,
a hold opens up "menu layer A" ( upper button ) and "menu layer B" ( lower button ) means mapMenuGroup() will map the adc to this group of parameters ( for A and B its the modes )
hold the lower button and single press the upper one will circle through additional menu layers like lfo settings, 
attenuation and sensitivity settings, filesystem operations ( i need a flag base mechanism for load setup data or initialise the firmware update here )

the loop pipeline is something like

read adc -> 

// the array for the loader constance - i think its better than scatter the values / constants everywhere around

#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMF_SD             		1	// max number of fragment shader on sd
#define         FSH_SD             		1	// max number of fragment shader on sd
#define         OMT_SD             		1	// max number of fragment shader on sd
#define         TEX_SD             		0	// max number of textures on sd
#define         VID_SD             		0	// max number of videos on sd
#define         KLN_SD                  1

#define         FRM_SD                  1   // i put them here because if my mem/dma allocation
#define         LOG_SD                  24  // here is the trick:
                                            // i will use a mix of hardwired and "open" logs.
                                            // m_bufferLog[0]       - startup - memory - init log
                                            // m_bufferLog[1]       - vc_sm
                                            // m_bufferLog[2]       - mmal
                                            // m_bufferLog[3]       - check glsl
                                            // m_bufferLog[4]       - glsl compile
                                            // m_bufferLog[5  - 15] - texture parser
                                            // m_bufferLog[16 - 23] - video parser

#define         VSH_USB                 0	// max number of u_vertex shader on sd
#define         OMF_USB            		0	// max number of fragment shader on sd
#define         FSH_USB            		32	// max number of fragment shader on sd
#define         OMT_USB            		0	// max number of fragment shader on sd
#define         TEX_USB            		8	// max number of textures on sd
#define         VID_USB            		8	// max number of videos on sd
#define         KLN_USB                 1

#define         FRM_USB                 1   // i put them here because if my mem/dma allocation
#define         LOG_USB                 1

#define         VSH_EXT                 1
#define         OMF_EXT                 1
#define         FSH_EXT                 1
#define         OMT_EXT                 1
#define         TEX_EXT                 1
#define         VID_EXT                 1
#define         KLN_EXT                 1

#define         VSH_SIZ                 (1024*32)
#define         OMF_SIZ                 (1024*32)
#define         FSH_SIZ                 (1024*32)
#define         OMT_SIZ                 (1024*1024*4)
#define         TEX_SIZ                 (1024*1024*4)
#define         VID_SIZ                 (1024*1024*8)
#define         KLN_SIZ                 (1024*1024*2)

#define         FRM_SIZ                 (1024*1024)

#define         LOG_SIZ                 (1024*64)

enum FileType
{
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FRM_BF,         // i decided to add the output-frames A & B
    LOGGER,         // and logger buffer information here
    FT_COUNT
};

enum FileField
{
    FLD_MAXSD = 0,
    FLD_MAXUSB,
    FLD_EXTCNT,
    FLD_SCANNED,    // new
    FLD_LOADED,
    FLD_PREV,       // new
    FLD_VALID // <- p_validCount 
    FLD_SIZE,
    FLD_COUNT
};

unsigned filecounter[FT_COUNT][FLD_COUNT] =
{   //          MAXSD   MAXUSB      EXTCNT   SCANNED   LOADED  PREV    V_CNT    SIZE  
    /* VSH */ { VSH_SD, VSH_USB,    VSH_EXT, 0,        0,      0,      0,       VSH_SIZ },
    /* OMF */ { OMF_SD, OMF_USB,    OMF_EXT, 0,        0,      0,      0,       OMF_SIZ },
    /* FSH */ { FSH_SD, FSH_USB,    FSH_EXT, 0,        0,      0,      0,       FSH_SIZ },
    /* OMT */ { OMT_SD, OMT_USB,    OMT_EXT, 0,        0,      0,      0,       OMT_SIZ },
    /* TEX */ { TEX_SD, TEX_USB,    TEX_EXT, 0,        0,      0,      0,       TEX_SIZ },
    /* VID */ { VID_SD, VID_USB,    VID_EXT, 0,        0,      0,      0,       VID_SIZ },
    /* KLN */ { KLN_SD, KLN_USB,    KLN_EXT, 0,        0,      0,      0,       KLN_SIZ },
    /* FRM */ { FRM_SD, FRM_USB,          0, 0,        0,      0,      0,       FRM_SIZ },     // i decided to add the output-frames A & B
    /* LOG */ { LOG_SD, LOG_USB,          0, 0,        0,      0,      0,       LOG_SIZ }      // and logger buffer information here      
};
// lists of extensions possible in my scanroot directory function per filetype 
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" }; 
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" }; // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };
        const   char                   *g_SufVid[VID_EXT]			    = { "264" }; // i guess i will remove the whole parse code for anything but h264
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };
// array to store the scanned filenames
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLM_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLM_SD + KLN_USB]      = { 0 };
// our buffers members for the allocation

enum io_types
{
int RAW = 0,        //  the position the dampened adc values per channels are stored *
int VAL,            //  either the calculated int or flt value *
int OUT,            //  here lands the processed ( after mode ) for the glsl uniforms      
int RND,            //  either the per-channel random int or flt value *
int LF1,            //  either the lfo one int or flt value
int LF2,            //  either the lfo two int or flt value
int AU0,            //  the audio band 0 flt value
int AU1,            //  the audio band 1 flt value
int AU2,            //  the audio band 2 flt value
int au3,            //  the audio band 3 flt value
                    // *means i have a unique value for each channel - the other values are singular, and/or only int/flt
int TRL,            //  per channel threshold low !!! dont forget to copy the values in here    128
int TRH,            //  per channel threshold high                                              320
//  trF,            //  per channel threshold "flag"
int IO_TYPE_COUNT
}

g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];
g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];
g_menuPickUpFlag[4*menu_layers];

                char** 				    m_bufferVid;
                char* 				    m_videoBlockBase;
                char* 				    m_videoRawBlock;
                size_t 				    m_videoBlockSize;

                char**				    m_bufferFrA;
                char* 				    m_frameBlockBaseA;
                char* 				    m_frameRawBlockA;
                size_t 				    m_frameBlockSizeA;

                char**				    m_bufferFrB;
                char* 				    m_frameBlockBaseB;
                char* 				    m_frameRawBlockB;
                size_t 				    m_frameBlockSizeB;	

                char** 				    m_bufferOmt;
                char* 				    m_overlyBlockBase;
                char* 				    m_overlayRawBlock;
                size_t 				    m_overlyBlockSize;

                char** 				    m_bufferTex;
                char* 				    m_textureBlockBase;
                char* 				    m_textureRawBlock;
                size_t 				    m_textureBlockSize;

                char**				    m_bufferKnl;
                char**				    m_bufferLog;
                char** 				    m_bufferVsh;
                char** 				    m_bufferOmf;                
                char** 				    m_bufferFsh;