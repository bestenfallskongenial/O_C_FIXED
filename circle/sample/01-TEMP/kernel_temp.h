// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  MENU
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <GLES2/gl2.h>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include "vc04_service.h"
#include "vc04_defs.h"

#define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertion implementation

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  MACROS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define         EMPTYSTR ""
#define         EMPTYLOG 255

#define			TIMEOUT					10
#define 		LOGLEVEL				4	// 0: only panic 1: also errors 2: also warnings 3: also notices 4: also debug output (default))

#define 		FILENAME_KNL			"kernel.img"

#define         PARTITIONSD	            "emmc1-1"	// partition sd
#define         PARTITIONUSB	        "umsd1-1"	// partition usb

#define 		FILENAME_GL_LOG			"LOG-GLES.txt"
#define 		FILENAME_GLERROR		"LOG-GLER.txt"
#define 		FILENAME_VCSM_LOG		"LOG-VCSM.txt"
#define 		FILENAME_MMAL_LOG		"LOG-MMAL.txt"

#define         ADC_CHANNELS         	8                                   // number of adc input channels ( dependency <sensor/mcp300x.h>	)
#define         VREF			 		5.0f	                            // Reference voltage 5 Volt     ( dependency <sensor/mcp300x.h>	)

#define 		ADC_BUFFER			 	4	// number of buffer cells for smoothing

#define         CHANNEL			    	7		                            // im confused - is it needed to init the mcp3008?

#define         SPI_MASTER_DEVICE	 	0		                            // 0    m_SPIMaster ( also for the mcp3008 )
#define         SPI_CHIP_SELECT		 	0		                            // 0
#define         SPI_CLOCK_SPEED	   		1000000		                        // Hz

#define         CS_PIN                 	12                                  // for menu input selector!!!

#define         NR_BUTTONS              2                                   // was addon/sensors/buttons.h is now buttonPing()  no class required 
#define         SW_PIN_A	           	13                                  // button A ( lower )
#define         SW_PIN_B	          	6                                   // button B ( upper )

#define         LED_PIN		           	19		                            // led ( must be between 8 .. 23 )
#define         LED_COUNT		       	4		                            // number of LEDs on the stripes

#define         GPIO_TO_SD_LINE(pin)	((pin) - 8)							// dont touch!
#define         SD_LINE1		      	GPIO_TO_SD_LINE (LED_PIN)			// dont touch!
#define         SD_LINE_TO_MASK(line)	(1 << (line))						// dont touch!
#define         SD_LINES_MASK		 	(  SD_LINE_TO_MASK (SD_LINE1) )		// dont touch! all needed for <WS28XX/ws2812oversmi.h>

#define 		CHUNK_SIZE				1024

#define         SLOTS                   34 // for the g_centralModeBuffer[SLOTS][modetablecount] array - 1 firmware / 32 user / 1 default slot
#define         DEFAULT_SLOT            34 // or 0 ?? 

#define         TEX_SIZ

#define         MAX_VIDEO_WIDTH         640
#define         MAX_VIDEO_HEIGHT        480
#define         MAX_VIDEO_PROFILE        66		// Baseline
#define         MAX_VIDEO_LEVEL

#define         TARGET_FPS              24      // desired frame rate !!! becomes menu !!!
#define 		MIN_BPM					10		// min bpm for adc selector 
#define			MAX_BPM					240 	// max bpm for adc selector

#define 		NUMBER_OF_MODES			6       // is now defined in the g_modeMap array

#define         WAVEFORMS             	4		// number of lfo waves
#define         WAVESAMPLES            	256  	// number of samples per lfo waves
#define 		WAVEFORMS			    4
#define 		LFO_INSTANCES			2
#define			LFO_MULTIPLIERS			7

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// the array for the loader constance - i think its better than scatter the values / constants everywhere around instead unsigned filecounter[FT_COUNT][FLD_COUNT]
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMF_SD             		1	// max number of fragment shader on sd
#define         FSH_SD             		1	// max number of fragment shader on sd
#define         OMT_SD             		1	// max number of fragment shader on sd
#define         TEX_SD             		0	// max number of textures on sd
#define         VID_SD             		0	// max number of videos on sd
#define         KLN_SD                  1

#define         FRM_SD                  1   // i put them here because if my mem/dma allocation
#define         LOG_SD                  24  // here is the trick:
                                            // i will use a mix of hardwired and "open" logs. examples:

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

#define         ADC_SELECT_PRG          7
#define         ADC_SELECT_TEX          6
#define         ADC_SELECT_VID          5
#define			ADC_INPUT_CLK			5	// adc p_channel use as clock ! BIGGER THAN 3 !

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

enum modetable		                        // for the g_centralModeBuffer[SLOTS][modetablecount] array
{
	int CH0_MODE = 0,
	int CH1_MODE,
	int CH2_MODE,
	int CH3_MODE,

	int CH4_MODE,
	int CH5_MODE,
	int CH6_MODE,
	int CH7_MODE,

	int LF1_WAVE,   // 0 to 3
	int LF2_WAVE,   // 0 to 3
	int LF1_MULT,   // 0 to 6
	int LF2_MULT,   // 0 to 6

	int SENS_A,     // 0 to 63
	int SENS_B,     // 0 to 63
	int SENS_C,     // 0 to 63
	int SENS_D,     // 0 to 63

	int FRM_MODE,   // 0 or 1   ( wait - i propose a simple 4 to 7 and 0 as of, means anything else than 0 is the actual nput p_channel problem, it need to overrule CH*_MODE )
	int TEX_MODE,   // 0 or 1   ( how we can do it? also, dont i want more than only p_channel 4-7 assignable? )
	int CLK_MODE,   // 0 or 1   ( whats about the approach in readADC() where i modify the number of possible modes in the modematrix )
	int VID_MODE,   // 0 or 1   ( like if i have one here CH*_MODE "opens" up for this modes - requires a constant check and update but... )

    int STORE_SET,  // 0 or 1   ( if set to 1 and the menu_layer is zero again ( button released ?) the file operation starts )
    int LOAD_SET,   // 0 or 1
    int STORE_LOG,  // 0 or 1
    int LOAD_KLN,   // 0 or 1

	int IS_STORED,                          // not really a mode but a flag - important: obey the "4 per block" rule

	modetablecount = 21 +4                    // right?
}

enum io_types                               // for the array g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT] & g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT]
{
    int RAW = 0,                            //  the position the dampened adc values per channels are stored *
    int VAL,                                //  either the calculated/normalized int or flt value *
    int OUT,                                //  here lands the processed ( after mode ) for the glsl uniforms      
    int RND,                                //  either the per-p_channel random int or flt value *
    int LF1,                                //  either the lfo one int or flt value
    int LF2,                                //  either the lfo two int or flt value
    int AU0,                                //  the audio band 0 flt value
    int AU1,                                //  the audio band 1 flt value
    int AU2,                                //  the audio band 2 flt value
    int AU3,                                //  the audio band 3 flt value
                                            // *means i have a unique value for each p_channel - the other values are singular, and/or only int/flt
    int TRL,                                //  per p_channel threshold low !!! dont forget to copy the values in here    128
    int TRH,                                //  per p_channel threshold high                                              320
    int TRF,                                //  per p_channel threshold "flag" - wehre do i need you again?

    int IO_TYPE_COUNT = 13
}

enum ButtonTSIndex  // for the button state machine - unsigned int g_buttons_states[NR_BUTTONS][5] = {0} !!!
{
    BTN_PRESS_START = 0,
    BTN_DOUBLE      = 1,
    BTN_RELEASE     = 2,
    BTN_SINGLE      = 3,
    BTN_HOLD_TICK   = 4,   // COUNTER: increases while held
    BTN_INDEX_COUNT = 5
};

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
    FLD_VALID, // <- p_validCount 
    FLD_SIZE,
    FLD_COUNT
};

struct olg_state
{
    // EGL Window
    uint32_t                    screen_width;
    uint32_t                    screen_height;

    DISPMANX_ELEMENT_HANDLE_T   dispman_element;
    DISPMANX_DISPLAY_HANDLE_T   dispman_display;

    EGLDisplay                  display;
    EGLSurface                  surface;
    EGLContext                  context;
};

struct vtx_state
{
    // shared attrib/buffer
    GLuint                      gl_buf;                         // this is also an extra struct we need to pass too
    GLint                       gl_vtx[MAX_SHADER];    
};

struct tex_state
{
    u32         max_tex_size;   // moved from CKernel   

    bool        tex_valid[MAX_TEXTURE];

    unsigned    width[MAX_TEXTURE];
    unsigned    height[MAX_TEXTURE];
    unsigned    offset[MAX_TEXTURE];

    unsigned    file_size[MAX_TEXTURE];
    unsigned    image_size[MAX_TEXTURE];

    u8*         data[MAX_TEXTURE];
    size_t      size[MAX_TEXTURES];

    GLuint      gl_tex_id[MAX_TEXTURE];
    GLint       u_tex_id[MAX_SHADER][MAX_TEXTURE];
};

struct h264_state
{
    // raw input will be populated by the parser init
    u8*             data[MAX_VIDEOS];
    size_t          size[MAX_VIDEOS];
    // data for the poller
    size_t          sps_off[MAX_VIDEOS][MAX_FRAMES];
    size_t          sps_sc_len[MAX_VIDEOS][MAX_FRAMES];
    size_t          sps_len[MAX_VIDEOS][MAX_FRAMES];

    size_t          pps_off[MAX_VIDEOS][MAX_FRAMES];
    size_t          pps_sc_len[MAX_VIDEOS][MAX_FRAMES];
    size_t          pps_len[MAX_VIDEOS][MAX_FRAMES];

    size_t          idr_off[MAX_VIDEOS][MAX_FRAMES];
    size_t          idr_sc_len[MAX_VIDEOS][MAX_FRAMES];
    size_t          idr_len[MAX_VIDEOS][MAX_FRAMES];
    // frame table
    void*           frame_address[MAX_VIDEOS][MAX_FRAMES];
    size_t          frame_offset[MAX_VIDEOS][MAX_FRAMES];
    size_t          frame_length[MAX_VIDEOS][MAX_FRAMES];
    int             idr_offset[MAX_VIDEOS]; // size_t      idr_offset[MAX_VIDEOS];
    // extradata
    u8              extradata[MAX_VIDEOS][1024];
    size_t          extradata_len[MAX_VIDEOS];
    bool            extradata_valid[MAX_VIDEOS];
    // parsed metadata
    u16             video_width[MAX_VIDEOS];
    u16             video_height[MAX_VIDEOS];
    u8              vid_profile[MAX_VIDEOS];
    u8              vid_level[MAX_VIDEOS];
    // state
    int             frame_count[MAX_VIDEOS]; //     unsigned    frame_count[MAX_VIDEOS];
    bool            vid_valid[MAX_VIDEOS];
    // shared base
    char*           block_base; // void*   block_base;
    // constraints
    u16             max_width;
    u16             max_height;
    u8              max_profile;
    u8              max_level;
    // for my video frame texture 
    GLuint          gl_tex_vid;      // video texture handle
    EGLImageKHR     egl_img;        // backing (changes per frame)
};

struct glsl_state
{
    GLuint                      gl_shader_id[MAX_SHADER];
    GLuint                      gl_program_id[MAX_SHADER];

    bool                        shader_valid[MAX_SHADER];
    // user uniforms                                            // this is the actual common shader struct we define for 
    GLint                       u_time[MAX_SHADER];
    GLint                       u_tres[MAX_SHADER];
    GLint                       u_seed[MAX_SHADER];
    GLint                       u_aud[MAX_SHADER];
    GLint                       u_col[MAX_SHADER];
    GLint                       u_par_a[MAX_SHADER];
    GLint                       u_par_b[MAX_SHADER];
    GLint                       u_tex_l[MAX_SHADER];
    // overlay uniforms
    GLint                       u_atlas[MAX_OMF];
    GLint                       u_tile_count[MAX_OMF];
    GLint                       u_tile_rect[MAX_OMF];
    GLint                       u_tile_index[MAX_OMF];
    // overlay data
    float                       kMenuOrigin[2];
    float                       kMenuTileSize[2];
    float                       kMenuBackgroundScale[2];

    float                       kMenuRelPos[MAX_TILES][2];
    float                       kMenuRelSize[MAX_TILES][2];

    float                       tile_rect_x[MAX_TILES];
    float                       tile_rect_y[MAX_TILES];
    float                       tile_rect_w[MAX_TILES];
    float                       tile_rect_h[MAX_TILES];

    GLfloat                     tile_rect[MAX_TILES * 4];
    GLfloat                     tile_index[MAX_TILES];
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  PUBLIC / PRIVATE
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CKernel
{
public:
	CKernel (void);
	~CKernel (void);

	boolean     Initialize (void);

	TShutdownMode Run (void);

                bool        Mount                       (   const   char*                           p_deviceName);          // "emmc1-1" cd ( root ), "umsd1-1" usb

                bool        UnMount                     ();

                bool        openFile                    (   const   char*                           p_fileName);            // filename format "8.3"

                unsigned    loadToBuffer                (           char*                           p_buffer,               // destination buffer for the file data
                                                                    unsigned                        p_bufferSize);          // maximum number of bytes to read into the buffer

                bool        saveFromBufferO             (   const   char*                           p_fileName,             // filename format "8.3" 
                                                            const   char*                           p_buffer,               // my allocated buffer
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        saveFromBufferM             (   const   char*                           p_deviceName,           // "emmc1-1" cd ( root ), "umsd1-1" usb
                                                            const   char*                           p_fileName,             // filename format "8.3"
                                                            const   char*                           p_buffer,               // my allocated buffer 
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        closeFile                   ();

                void        bulkLoad                    (           char*                           p_fileNameArray[],      // where we have stored the filenames from the root directory scan
                                                                    unsigned                        p_loadedBytes[],        // where we store the size in bytes for each file
                                                                    char**                          p_bufferArray,          // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                                    unsigned                        p_maxFiles,             // how many files we are allowed to process ( os limitations )
                                                                    unsigned&                       p_validFiles,           // counts successful loads - we need to keep track here <- MUST initialised with 0
                                                                    unsigned&                       p_prevFiles,            // number of loads from the last call - we need it to init the files correctly
                                                                    unsigned                        p_fileSize);            // maximum size for each file

                bool        IsValidFile                 (   const   char*                           pFileName, 
                                                            const   char*                           extension);

                bool        scanRoot                    (           char**                          p_fileNameArray,        // where we store the valid filenames we find
                                                                    const char*                     p_fileExtension[],      // the array of valid file extensions for this type of file
                                                                    unsigned                        p_extentionCount,       // how many valid file extensions we have in the array above
                                                                    unsigned&                       p_scannedFiles,         // our counter of found files
                                                                    unsigned                        p_maxFiles);            // how many files are allowed to scan and stored in the array

                bool        updateUSB                   (   const   char*                           p_deviceName);

    static      void        removeUSB                   (           CDevice*                        pDevice, 
                                                                    void*                           pContext);

                char**      allocBufferMEM              (           size_t                          p_count, 
                                                                    size_t                          bufferSize);

                char**      allocBufferDMA              (           size_t                          p_count,
                                                                    size_t                          bufferSize,
                                                                    char**                          blockBaseOut,
                                                                    char**                          rawBlockOut,
                                                                    size_t*                         alignedSizeOut);

                void        clearBufferMEM              (           char**                          buffers, 
                                                                    size_t                          p_count);

                void        clearBufferDMA              (           char**                          buffers, 
                                                                    char*                           rawBlock);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  GRAPHICS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        initOGL                     (           olg_state*                      o);

                void        initVbuffer                 (           olg_state*                      o, 
                                                                    vtx_state*                      v);

                void        initShader                  (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    char**                          p_buffer,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    GLenum                          type);

                void        initProgram                 (           vtx_state*                      v,
                                                                    glsl_state*                     vsh,
                                                                    glsl_state*                     fsh,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count);
                void        initTexture                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count,
                                                                    GLint                           wrap_s,
                                                                    GLint                           wrap_t);

                void        initUniform                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  RUNTIME RENDER
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        frmBufferSet                (           vtx_state*                      v);

                void        frmBufferSwap               (           olg_state*                      o);

                void        setUniPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                /*  int                             gl_current_tex, */
                                                                    unsigned                        p_validTextureCount);

                void        setTexPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             gl_current_tex,
                                                                    unsigned                        p_validTextureCount);
                void        drawGLsPrg                  ();

                void        frmRateBreak                (           bool*                           noTargetFPS);

                void        updateOvlState              (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);

                void        setUniOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);

                void        setTexOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);
                void        drawGLsOvl                  ();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  LOGGING
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        storeLog                    (           char*                           p_buffer, 
                                                                    u32&                            index,
                                                            const   char*                           label,
                                                                    u32                             p_value0, 
                                                                    u32                             p_value1,
                                                                    u32                             p_value2, 
                                                                    u32                             p_value3);

                void        storeLogLong                (           char*                           p_buffer,
                                                                    u32&                            index,
                                                            const   char*                           p_string0, 
                                                                    u32                             p_value0,
                                                            const   char*                           p_string1, 
                                                                    u32                             p_value1,
                                                            const   char*                           p_string2, 
                                                                    u32                             p_value2,
                                                            const   char*                           p_string3, 
                                                                    u32                             p_value3);

                void        storeMsg                    (           char*                           p_buffer,
                                                                    u32&                            index,
                                                                    const char*                     label,
                                                                    const void*                     tx_msg,
                                                                    u32                             total_size);

                void        nextline                    (           char*                           p_buffer,
                                                                    u32&                            index);

    static      void        bufferToScreenPlot          (           unsigned                        x, 
                                                                    unsigned                        y, 
                                                                    u32                             color );       

    static      void        bufferToScreenDrawChar      (           char                            ch,
                                                                    unsigned                        charCol,
                                                                    unsigned                        charRow,
                                                                    u32                             fgColor,
                                                                    u32                             bgColor );

                boolean     bufferToScreenInit          (           void )

                void        bufferToScreenClear         (           u32                             bgColor);

                void        bufferToScreenDrawBuffer    (   const   char*                           pSourceBuffer,
                                                                    u32                             startIndex,
                                                                    u32                             endIndex,
                                                                    unsigned                        startCol,
                                                                    unsigned                        startRow,
                                                                    u32                             fgColor,
                                                                    u32                             bgColor);

                unsigned    bufferToScreenGetGrid       (           unsigned&                       cols, 
                                                                    unsigned&                       rows);

                bool        shaderLog                   (           GLint                           shader, 
                                                                    int                             shaderIndex);

                bool        programLog                  (           GLint                           program, 
                                                                    int                             program_index);

                void        gfx_check                   (   const   char*                           file, 
                                                                    unsigned                        line);

                bool        startupScreen               (           char*                           p_buffer,
                                                                    u32&                            index);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  MENU
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        resetPickUpFlags            ();

                void        mapMenuGroup                (           uint8_t                         menu_id, 
                                                                    uint8_t                         base);

                void        setChannelMode              (           int                             p_channel);

                void        modeADC                     (           int                             p_channel);

                void        modeTRG                     (           int                             p_channel);

                void        modeBPM                     (           int                             p_channel);

                void        modeLF1                     (           int                             p_channel);

                void        modeLF2                     (           int                             p_channel);

                void        modeAudioAb0                (           int                             p_channel);

                void        modeAudioAb1                (           int                             p_channel);

                void        modeAudioBb0                (           int                             p_channel);

                void        modeAudioBb1                (           int                             p_channel);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  PARSER
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        initBMPparser               (           tex_state*                      t,
                                                                    char*                           p_buffer_array[],
                                                                    size_t                          size_array[],
                                                                    u32                             max_tex_size,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);

                bool        initH264parser              (           h264_state*                     h,
                                                                    char*                           blockBase,
                                                                    char*                           p_buffer_array[],
                                                                    size_t                          size_array[],
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    u16                             max_width,
                                                                    u16                             max_height,
                                                                    u8                              max_profile,
                                                                    u8                              max_level);

                void        ParseBPM                    (           tex_state*                      t,
                                                                    char*                           filename_array[],
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);

                void        ParseAnnexB                 (           h264_state*                     h,
                                                                    char*                           filename_array[],
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);
                                 
                bool        ParseSPS                    (           u8*                             sps_data,
                                                                    size_t                          sps_size,
                                                                    size_t                          sps_sc_len,
                                                                    u16*                            width,
                                                                    u16*                            height,
                                                                    u8*                             profile,
                                                                    u8*                             level) const;

                size_t      findNext000001              (           u8*                             data, 
                                                                    size_t                          pos, 
                                                                    size_t                          size) const;

                u32         ReadExpGolomb               (           u8*                             data, 
                                                                    size_t*                         bit_offset) const;
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  UTIL
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
                void        readADC                     ();

                bool        checkUpdate                 ();

                bool        Update                      ();

                void        set_pot_routing             (           bool                            adc_pot_routing);

                void        prepParameters              ();

                void        chooseIndex                 (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount, 
                                                                    bool*                           flags);

                void        chooseIndexD                (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount);
                void        storeModes                  ();

                void        buttonPing                  (           int                             p_btn_id, 
                                                                    int                             p_pin);

                void        button_consumer             (           int                             p_btn_id);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        randomVec8                  (           uint32_t                        p_seed);

                void        calculate1BPM               (           int                             p_source, 
                                                                    unsigned long                   p_triggerTimeClock);

                void        predict1Beat                (           int                             p_source, 
                                                                    int                             p_lfoMult);

                void        sample1WaveTable            (           int                             p_source, 
                                                                    int                             p_lfoIn, 
                                                                    int                             p_lfoOut );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  VC04_SERVICE
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
                void        callbackVCSM                (           void*                           callback_param, 
                                                                    VCHI_CALLBACK_REASON_T          reason, 
                                                                    void*                           msg_handle); // NOT STATIC REQUIRED?!

                void        callbackMMAL                (           void*                           callback_param, 
                                                                    VCHI_CALLBACK_REASON_T          reason, 
                                                                    void*                           msg_handle);

                void        getStateVCHI();
                bool        initEventsVCOS              (           VCOS_EVENT_T&                   event, 
                                                            const   char*                           name);

                u32         convertAddress              (           void*                           p_buffer, 
                                                                    size_t                          size);
                bool        checkGLerrorMMAL            ();

                u32         NextTransId                 (           u32&                            tid);

                void        initHeaderVCSM              (           vc_sm_msg_hdr_t&                hdr, 
                                                                    u32                             type);

                void        initHeaderMMAL              (           mmal_msg_header&                hdr, 
                                                                    u32                             type);

                bool        sendAndWaitVCHI             (           VCHI_SERVICE_HANDLE_T           ServiceHandle,
                                                                    VCOS_EVENT_T&                   VCOSevent,
                                                                    const void*                     msg,
                                                                    size_t                          msg_size,
                                                                    void*                           rx_msg,
                                                                    size_t                          max_reply_len,
                                                                    size_t*                         actual_reply_len);

                bool        openServiceVCHI             (           SERVICE_CREATION_T&             tx,
                                                                    uint32_t                        serviceVersion,
                                                                    uint32_t                        serviceVersionMin,
                                                                    int32_t                         service_id,
                                                                    VCHI_CALLBACK_T                 cb,
                                                                    void*                           cb_param,
                                                                    VCHI_INSTANCE_T                 VCHIInstance,
                                                                    VCHI_SERVICE_HANDLE_T&          ServiceHandle);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        initializeVCSM              ();
                bool        importMemoryVCSM            (           void*                           p_buffer, 
                                                                    size_t                          size, 
                                                                    int                             slot, 
                                                                    VCSM_Import_MEM_Msg&            tx, 
                                                                    VCSM_Import_MEM_Reply&          rx);

                bool        lockMemoryVCSM              (           int                             slot, 
                                                                    VCSM_Lock_MEM_Msg&              tx, 
                                                                    VCSM_Lock_MEM_Reply&            rx);

                bool        freeMemoryVCSM              (           int                             slot, 
                                                                    VCSM_Free_MEM_Msg&              tx, 
                                                                    VCSM_Free_MEM_Reply&            rx);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        initializeMMAL              (           u32                             InBufferHandle,
                                                                    u32                             InBufferPointer,
                                                                    u32                             InBufferSize,
                                                                    u32                             OutBufferHandleA,
                                                                    u32                             OutBufferPointerA,
                                                                    u32                             OutBufferHandleB,
                                                                    u32                             OutBufferPointerB,
                                                                    u32                             OutBufferSize,
                                                                    u32                             ResolutionX,
                                                                    u32                             ResolutionY,
                                                                    EGLDisplay                      eglDisplay,
                                                                    EGLContext                      eglContext);
                bool        createTexturesMMAL          ();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        framePollerMMAL             (           u32                             frame_offset, 
                                                                    u32                             frame_length);

                bool        bufferReadyMMAL             (           u32                             handle);

                bool        queueOutputBufferMMAL       (           MMAL_Buffer_From_Host_Msg&      tx, 
                                                                    u32                             vc_handle, 
                                                                    u32                             alloc_size);

                bool        queueInputBufferMMAL        (           MMAL_Buffer_From_Host_Msg&      tx, 
                                                                    u32                             frame_offset, 
                                                                    u32                             frame_length);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        createComponent             (           MMAL_Component_Create_Msg&      tx, 
                                                                    MMAL_Component_Create_Reply&    rx);

                bool        getPortInfoMMAL             (           u32                             port_type, 
                                                                    u32&                            port_handle, 
                                                                    MMAL_Port_Info_Get_Msg&         tx, 
                                                                    MMAL_Port_Info_Get_Reply&       rx);

                bool        setPortInfoMMAL             (           MMAL_Port_Info_Set_Msg&         tx, 
                                                                    MMAL_Port_Info_Set_Reply&       rx);

                bool        enableComponentMMAL         (           MMAL_Component_Enable_Msg&      tx, 
                                                                    MMAL_Component_Enable_Reply&    rx);

                bool        setZeroCopyModeMMAL         (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Parameter_Set_Msg&    tx, 
                                                                    MMAL_Port_Parameter_Set_Reply&  rx);
                                                                    
                bool        enablePortMMAL              (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Action_Msg&           tx, 
                                                                    MMAL_Port_Action_Reply_Msg&     rx);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        PrimeOutputBufferBodyMMAL   (           MMAL_Buffer_From_Host_Msg&      tx);

                void        PrimeInputBufferBodyMMAL    (           MMAL_Buffer_From_Host_Msg&      tx);

                void        primePortFormatInputMMAL    (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Info_Set_Msg&         tx);

                void        primePortFormatOutputMMAL   (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Info_Set_Msg&         tx);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  WRAPPERS - HERE THE JOY BEGINS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        wrapperInitDMA              ();
                bool        wrapperInitMEM              ();

                void        wrapperDMAcleanUp           ();
                void        wrapperMEMcleanUp           ();

                void        wrapper_from_sd             ();
                void        wrapper_load_usb            ();

                void        wrapper_init_gl_sd          ();
                void        wrapper_init_gl_usb         ();

                void        wrapper_io                  ();
                void        wrapper_modes               ();

private:
	CActLED			    m_ActLED;
	CKernelOptions		m_Options;
	CMachineInfo		m_MachineInfo;
	CDeviceNameService	m_DeviceNameService;
	CScreenDevice		m_Screen;
	CSerialDevice		m_Serial;
	CExceptionHandler	m_ExceptionHandler;
	CInterruptSystem	m_Interrupt;
	CTimer			    m_Timer;
	CLogger			    m_Logger;
	CEMMCDevice			m_EMMC;
	CUSBHCIDevice		m_USBHCI;
	CVCHIQDevice		m_VCHIQ;

	CMemorySystem		m_Memory;

	volatile boolean	m_bStorageAttached;
	CFATFileSystem		*m_pFileSystem;

  	CScheduler		    m_Scheduler; // really? is it needed for the khronos stuff?

	CBcmWatchdog       	m_Watchdog; // Watchdog instance - we included our own!

	CSPIMaster		    m_SPIMaster;
	CMCP300X		    m_MCP300X;

	CWS2812OverSMI		m_NeoPixels;

	CGPIOManager		m_GPIOManager;		// not needed in polling mode - we included our own "write to gpiopin" but why does the constructor def uses "m_GPIOManager (&m_Interrupt),..." oh i see, the ws2812 led i assume

	CGPIOPin 			m_ChipSelectPin;  	// Add this line for the chip select gpiopin - we try included our own "write to gpiopin"

// we need a struct for the videos right?

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
// local copies of my graphics related structs
    olg_state           m_ogl;

    vtx_state           m_vtx;

    glsl_state          m_vsh;
    glsl_state          m_fsh;
    glsl_state          m_osh;

    tex_state           m_tex;
    tex_state           m_omt;

    h264_state          m_vid;
// the populated filecounter array - source and truth and hub for init and load
    unsigned filecounter[FT_COUNT][FLD_COUNT] =
{  // MAXSD   MAXUSB    EXTCNT      SCANNED   LOADED  PREV    V_CNT    SIZE  
    { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
    { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
    { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
    { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
    { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
    { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
    { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
    { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
    { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }   // LOG logging buffers   
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
                char                   *g_ScnKln[KLN_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLN_SD + KLN_USB]      = { 0 };
};

uint8_t g_modeMap[MENU_LAYERS*4][MENU_LAYERS*4] =	// the first element is the max of modes for each p_channel, than we have the order ( switch case of setChannelMode(int p_channel) )
{ //  A    /  B    /  LFO  / Sens  / etc     
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0} 	// layer a is adc in 0-3 
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer b is adc in 4-7
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}   // layer d is adc in 12-15
{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
};

g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];
g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];
g_menuPickUpFlag[4*MENU_LAYER];
