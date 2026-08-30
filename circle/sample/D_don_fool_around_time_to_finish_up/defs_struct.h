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
    GLuint                      gl_buf;
    GLint                       gl_vtx[MAX_SHADER];    
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

struct tex_state
{
    u32         				max_tex_size;

    bool        				tex_valid[MAX_TEXTURE];

    unsigned    				width[MAX_TEXTURE];
    unsigned    				height[MAX_TEXTURE];
    unsigned    				offset[MAX_TEXTURE];

    unsigned    				file_size[MAX_TEXTURE];
    unsigned    				image_size[MAX_TEXTURE];

    u8*         				data[MAX_TEXTURE];
    size_t      				size[MAX_TEXTURE];

    GLuint      				gl_tex_id[MAX_TEXTURE];
    GLint       				u_tex_id[MAX_SHADER][MAX_TEXTURE];
       // for my video frame texture 
    GLuint          			gl_tex_vid;                 // video texture handle
    EGLImageKHR     			m_EGLimage = nullptr;       // backing (changes per frame) 
};
/*
| Model 					| CONFIG once                     | RUNTIME per frame                               |
| ------------------------- | ------------------------------- | ----------------------------------------------- |
| H264_DEFAULT / Annex-B  A | `startcode+SPS + startcode+PPS` | `startcode+SPS + startcode+PPS + startcode+IDR` |
| H264_DEFAULT / Annex-B  B | `startcode+SPS + startcode+PPS` | `startcode+IDR`                                 |
| H264_RAW - not feasible!! | SPS without startcode/		  |													|
|							| EPB + PPS without startcode/EPB | IDR without startcode/EPB                       |
*/
struct h264_state
{
    // raw input will be populated by parser_init
    u8*             			data[MAX_VIDEOS];
    size_t          			size[MAX_VIDEOS];
    // frame table -  the actual data we need for model a decoding
    void*           			nal_block_address[MAX_VIDEOS][MAX_FRAMES];
    size_t          			nal_block_offset[MAX_VIDEOS][MAX_FRAMES];
    size_t          			nal_block_length[MAX_VIDEOS][MAX_FRAMES];
    // frame table -  the actual data we need for model b decoding 
    void*           			frame_address[MAX_VIDEOS][MAX_FRAMES];
    size_t          			frame_offset[MAX_VIDEOS][MAX_FRAMES];
    size_t          			frame_length[MAX_VIDEOS][MAX_FRAMES];

	size_t						idr_sc_len[MAX_VIDEOS];
    // extradata
    u8              			extradata[MAX_VIDEOS][256];
    size_t          			extradata_len[MAX_VIDEOS];
    bool            			extradata_valid[MAX_VIDEOS];
    // parsed metadata
    u16             			video_width[MAX_VIDEOS];
    u16             			video_height[MAX_VIDEOS];
    u8              			vid_profile[MAX_VIDEOS];
    u8              			vid_level[MAX_VIDEOS];
    // state
    size_t          			frame_count[MAX_VIDEOS];                // unsigned frame_count[MAX_VIDEOS]; ??
    bool            			vid_valid[MAX_VIDEOS];
    // shared base
    char*           			block_base; // void*   block_base;
    // constraints from the firmware
    u16             			min_width;
	u16             			max_width;

    u16             			min_height;	
    u16             			max_height;

	u8              			min_profile;
    u8              			max_profile;

	u8							min_level;
    u8              			max_level;
};
//  VCSM
struct vc_sm_msg_hdr_t 								// Message header for all messages in HOST->VC direction //
	{
	u32 type;
	u32 trans_id;
	u8 body[0];
	};
struct vc_sm_alloc_t 								// Request to allocate memory (HOST->VC) //
	{
	enum vc_sm_alloc_type_t type;					// type of memory to allocate //
	u32 base_unit;									// byte amount of data to allocate per unit //
	u32 num_unit;									// number of unit to allocate //
	u32 alignment;									// alignment to be applied on allocation //
	u32 allocator;									// identity of who allocated this block //
	char name[VC_SM_RESOURCE_NAME];					// resource name (for easier tracking on vc side) //
	};
struct vc_sm_alloc_result_t 						// Result of a requested memory allocation (VC->HOST) //
	{
	u32 trans_id;									// Transaction identifier //
	u32 res_handle;									// Resource handle //
	u32 res_mem;									// Pointer to resource buffer //
	u32 res_base_size;								// Resource base size (bytes) //
	u32 res_num;									// Resource number //
	};
struct vc_sm_free_t 								// Request to free a previously allocated memory (HOST->VC) //
	{
	u32 res_handle;									// Resource handle (returned from alloc) //
	u32 res_mem;									// Resource buffer (returned from alloc) //
	};
struct vc_sm_lock_unlock_t 							// Request to lock a previously allocated memory (HOST->VC) //
	{
	u32 res_handle;									// Resource handle (returned from alloc) //
	u32 res_mem;									// Resource buffer (returned from alloc) //
	};
struct vc_sm_resize_t 								// Request to resize a previously allocated memory (HOST->VC) //
	{
	u32 res_handle;									// Resource handle (returned from alloc) //
	u32 res_mem;									// Resource buffer (returned from alloc) //
	u32 res_new_size;								// Resource *new* size requested (bytes) //
	};
struct vc_sm_lock_result_t 							// Result of a requested memory lock (VC->HOST) //
	{
	u32 trans_id;									// Transaction identifier //
	u32 res_handle;									// Resource handle //
	u32 res_mem;									// Pointer to resource buffer //
	u32 res_old_mem;								// Pointer to former resource buffer if the memory was reallocated
	};
struct vc_sm_result_t 								// Generic result for a request (VC->HOST) //
	{
	u32 trans_id;									// Transaction identifier //
	s32 success;
	};
struct vc_sm_action_clean_t 						// Request to revert a previously applied action (HOST->VC) //
	{
	enum vc_sm_msg_type 							res_action;			// Action of interest //
	u32 action_trans_id;							// Transaction identifier for the action of interest //
	};
struct vc_sm_free_all_t 							// Request to remove all data associated with a given allocator (HOST->VC) //
	{
	u32 allocator;									// Allocator identifier //
	};
struct vc_sm_import 								// Request to import memory (HOST->VC) //	
	{
	enum vc_sm_alloc_type_t 						type;			// type of memory to allocate //
	u32 addr;										// pointer to the VC (ie physical) address of the allocated memory //
	u32 size;										// size of buffer //
	u32 kernel_id;									// opaque handle returned in RELEASED messages //
	u32 allocator;									// Allocator identifier //
	char name[VC_SM_RESOURCE_NAME];					// resource name (for easier tracking on vc side) //
	};
struct vc_sm_import_result 							// Result of a requested memory import (VC->HOST) //
	{
	u32 trans_id;									// Transaction identifier //
	u32 res_handle;									// Resource handle //
	};
struct vc_sm_released 								// Notification that VC has finished with an allocation (VC->HOST) //
	{
	u32 cmd;										// cmd type / trans_id //
	u32 addr;										// pointer to the VC (ie physical) address of the allocated memory //
	u32 size;										// size of buffer //
	u32 kernel_id;									// opaque handle returned in RELEASED messages //
	u32 vc_handle;
	};							
struct vc_sm_version 								// Client informing VC as to the protocol version it supports.
	{
	u32 version;									// >=2 requires the released callback, and supports VC asking for memory. Failure means that the firmware doesn't support this call, and therefore the client should either fail, or NOT rely on getting the released callback.
	}; 
struct vc_sm_vc_mem_request 						// Request FROM VideoCore for some memory //
	{
	u32 cmd;										// cmd type //
	u32 trans_id;									// trans_id (from VPU) //
	u32 size;										// size of buffer //
	u32 align;										// alignment of buffer //
	char     name[VC_SM_RESOURCE_NAME];				// resource name (for easier tracking) //
	u32 vc_handle;									// VPU handle for the resource //
	};
struct vc_sm_vc_mem_request_result 					// Response from the kernel to provide the VPU with some memory //
	{
	u32 trans_id;									// Transaction identifier for the VPU //
	u32 addr;										// pointer to the physical address of the allocated memory //
	u32 kernel_id;									// opaque handle returned in RELEASED messages //
	};
union vc_sm_msg_union_t 							// Union of ALL messages ??
	{
	struct vc_sm_alloc_t 							alloc;
	struct vc_sm_alloc_result_t 					alloc_result;
	struct vc_sm_free_t 							free;
	struct vc_sm_lock_unlock_t 						lock_unlock;
	struct vc_sm_action_clean_t 					action_clean;
	struct vc_sm_resize_t 							resize;
	struct vc_sm_lock_result_t 						lock_result;
	struct vc_sm_result_t 							result;
	struct vc_sm_free_all_t 						free_all;
	struct vc_sm_import 							import;
	struct vc_sm_import_result 						import_result;
	struct vc_sm_version 							version;
	struct vc_sm_released 							released;
	struct vc_sm_vc_mem_request 					vc_request;
	struct vc_sm_vc_mem_request_result 				vc_request_result;
	};
//              FROM MMAL-MSG-COMMON.H
struct mmal_rect 
{
	s32 x;      									// x coordinate (from left) //
	s32 y;      									// y coordinate (from top) //
	s32 width;  									// width //
	s32 height; 									// height //
};

struct mmal_rational 
{
	s32 num;    									// Numerator //
	s32 den;    									// Denominator //
};
//              FROM MMAL-MSG-FORMAT.H
// MMAL_ES_FORMAT_T //
struct mmal_audio_format 
{
	u32 channels;									// Number of audio channels //
	u32 sample_rate;								// Sample rate //
	u32 bits_per_sample;							// Bits per sample //
	u32 block_align;								// Size of a block of data //
};
struct mmal_video_format 
{
	u32 width;										// Width of frame in pixels //
	u32 height;										// Height of frame in rows of pixels //
	struct mmal_rect 								crop;					// Visible region of the frame //
	struct mmal_rational 							frame_rate;		// Frame rate //
	struct mmal_rational 							par;				// Pixel aspect ratio //
	u32 color_space;								// FourCC specifying the color space of the video stream. //
													// See the MmalColorSpace "pre-defined color spaces" for some examples. //
};

struct mmal_subpicture_format 
{
	u32 x_offset;
	u32 y_offset;
};

union mmal_es_specific_format 
{
	struct mmal_audio_format 						audio;
	struct mmal_video_format 						video;
	struct mmal_subpicture_format 					subpicture;
};
/*
struct mmal_es_format_local 	// Definition of an elementary stream format (MMAL_ES_FORMAT_T) - we must investigate how this end up here!!
{
	u32 type;	// enum mmal_es_type //
	u32 encoding;	// FourCC specifying encoding of the elementary stream.
	u32 encoding_variant;	// FourCC specifying the specific encoding variant of the elementary stream.
	union mmal_es_specific_format *es;	// Type specific information for the elementary stream
	u32 bitrate;	// Bitrate in bits per second //
	u32 flags;	// Flags describing properties of the elementary stream.
	u32 extradata_size;	// Size of the codec specific data //
	u8  *extradata;		// Codec specific data //
};
*/
struct mmal_es_format 	// Remote definition of an elementary stream format (MMAL_ES_FORMAT_T) //
{
	u32 type;										// enum mmal_es_type //
	u32 encoding;									// FourCC specifying encoding of the elementary stream.
	u32 encoding_variant;							// FourCC specifying the specific encoding variant of the elementary stream.
	u32 es;											// Type specific information for the elementary stream
	u32 bitrate;									// Bitrate in bits per second //
	u32 flags;										// Flags describing properties of the elementary stream.
	u32 extradata_size;								// Size of the codec specific data //
	u32 extradata;									// Codec specific data //
};
//              FROM MMAL-MSG-PORT.H
struct mmal_port 
{
	u32 priv;										// Private member used by the framework //
	u32 name;										// Port name. Used for debugging purposes (RO) //
	u32 type;										// Type of the port (RO) enum mmal_port_type //
	u16 index;										// Index of the port in its type list (RO) //
	u16 index_all;									// Index of the port in the list of all ports (RO) //
	u32 is_enabled;									// Indicates whether the port is enabled or not (RO) //
	u32 format;										// Format of the elementary stream //
	u32 buffer_num_min;								// Minimum number of buffers the port requires (RO).  This is set by the component.
	u32 buffer_size_min;							// Minimum size of buffers the port requires (RO).  This is set by the component.
	u32 buffer_alignment_min;						// Minimum alignment requirement for the buffers (RO).  A value of zero means no special alignment requirements.  This is set by the component.
	u32 buffer_num_recommended;						// Number of buffers the port recommends for optimal performance (RO).  A value of zero means no special recommendation.  This is set by the component.
	u32 buffer_size_recommended;					// Size of buffers the port recommends for optimal performance (RO).  A value of zero means no special recommendation.  This is set by the component.
	u32 buffer_num;									// Actual number of buffers the port will use. This is set by the client.
	u32 buffer_size; 								// Actual maximum size of the buffers that will be sent to the port. This is set by the client.
	u32 component;									// Component this port belongs to (Read Only) //
	u32 userdata;									// Field reserved for use by the client //
	u32 capabilities;								// Flags describing the capabilities of a port (RO).  Bitwise combination of \ref portcapabilities "Port capabilities" values.
};
//              FROM MMAL-MSG.H
struct mmal_msg_header 
{
	u32 magic;
	u32 type;										// enum mmal_msg_type //
	u32 control_service; 							// Opaque handle to the control service //
	u32 context;									// a u32 per message context //
	u32 status;										// The status of the vchiq operation //
	u32 padding;
};

struct mmal_msg_version 							// Send from VC to host to report version //
{
	u32 flags;
	u32 major;
	u32 minor;
	u32 minimum;
};

struct mmal_msg_component_create 					// request to VC to create component //
{
	u32 client_component;							// component context //
	char name[128];
	u32 pid;										// For debug //
};

struct mmal_msg_component_create_reply 				// reply from VC to component creation request //
{
	u32 status;										// enum mmal_msg_status - how does this differ to the one in the header?
	u32 component_handle; 							// VideoCore handle for component //
	u32 input_num;        							// Number of input ports //
	u32 output_num;       							// Number of output ports //
	u32 clock_num;        							// Number of clock ports //
};

struct mmal_msg_component_destroy 					// request to VC to destroy a component //
{
	u32 component_handle;
};

struct mmal_msg_component_destroy_reply 
{
	u32 status; 									// The component destruction status //
};

struct mmal_msg_component_enable 					// request and reply to VC to enable a component //
{
	u32 component_handle;
};

struct mmal_msg_component_enable_reply 
{
	u32 status; 									// The component enable status //
};

struct mmal_msg_component_disable 					// request and reply to VC to disable a component //
{
	u32 component_handle;
};
struct mmal_msg_component_disable_reply 
{
	u32 status; 									// The component disable status //
};

struct mmal_msg_port_info_get 						// request to VC to get port information //
{
	u32 component_handle;  							// component handle port is associated with //
	u32 port_type;         							// enum mmal_msg_port_type //
	u32 index;             							// port index to query //
};

struct mmal_msg_port_info_get_reply 				// reply from VC to get port info request //
{
	u32 status;										// enum mmal_msg_status //
	u32 component_handle;							// component handle port is associated with //
	u32 port_type;									// enum mmal_msg_port_type //
	u32 port_index;									// port indexed in query //
	s32 found;										// unused //
	u32 port_handle;								// Handle to use for this port //
	struct mmal_port 								port;
	struct mmal_es_format 							format; 				// elementary stream format //
	union mmal_es_specific_format 					es; 			// es type specific data //
	u8 extradata[MMAL_FORMAT_EXTRADATA_MAX_SIZE]; 	// es extra data //
};

struct mmal_msg_port_info_set 						// request to VC to set port information //
{
	u32 component_handle;
	u32 port_type;									// enum mmal_msg_port_type //
	u32 port_index;									// port indexed in query //
	struct mmal_port 								port;
	struct mmal_es_format 							format;
	union mmal_es_specific_format 					es;
	u8 extradata[MMAL_FORMAT_EXTRADATA_MAX_SIZE];
};

struct mmal_msg_port_info_set_reply 				// reply from VC to port info set request //
{
	u32 status;
	u32 component_handle;							// component handle port is associated with //
	u32 port_type;									// enum mmal_msg_port_type //
	u32 index;										// port indexed in query //
	s32 found;										// unused //
	u32 port_handle;								// Handle to use for this port //
	struct mmal_port 								port;
	struct mmal_es_format 							format;
	union mmal_es_specific_format 					es;
	u8 extradata[MMAL_FORMAT_EXTRADATA_MAX_SIZE];
};

struct mmal_msg_port_action_port 					// port action requests that take a mmal_port as a parameter //
{
	u32 component_handle;
	u32 port_handle;
	u32 action;										// enum mmal_msg_port_action_type //
	struct mmal_port port;
};

struct mmal_msg_port_action_handle 					// port action requests that take handles as a parameter //
{
	u32 component_handle;
	u32 port_handle;
	u32 action;										// enum mmal_msg_port_action_type //
	u32 connect_component_handle;
	u32 connect_port_handle;
};

struct mmal_msg_port_action_reply 
{
	u32 status;										// The port action operation status //
};



struct mmal_driver_buffer 
{
	u32 magic;
	u32 component_handle;
	u32 port_handle;
	u32 client_context;
};

struct mmal_buffer_header 	// buffer header //
{
	u32 next;										// next header //
	u32 priv;										// framework private data //
	u32 cmd;
	u32 data;
	u32 alloc_size;
	u32 length;
	u32 offset;
	u32 flags;
	s64 pts;
	s64 dts;
	u32 type;
	u32 user_data;
};

struct mmal_buffer_header_type_specific 
{
	union {
		struct {
		u32 planes;
		u32 offset[4];
		u32 pitch[4];
		u32 flags;
		} video;
	} u;
};

struct mmal_msg_buffer_from_host 
{
	 //The front 32 bytes of the buffer header are copied
	 // back to us in the reply to allow for context. This
	 // area is used to store two mmal_driver_buffer structures to
	 // allow for multiple concurrent service users.

	// control data //
	struct mmal_driver_buffer 						drvbuf;
	// referenced control data for passthrough buffer management //
	struct mmal_driver_buffer 						drvbuf_ref;
	struct mmal_buffer_header 						buffer_header; // buffer header itself //
	struct mmal_buffer_header_type_specific 		buffer_header_type_specific;
	s32 is_zero_copy;
	s32 has_reference;
	// allows short data to be xfered in control message //
	u32 payload_in_message;
	u8 short_data[MMAL_VC_SHORT_DATA];
};

struct mmal_msg_port_parameter_set 
{
	u32 component_handle;							// component //
	u32 port_handle;								// port //
	u32 id;											// Parameter ID  //
	u32 size;										// Parameter size //
	u32 value[MMAL_WORKER_PORT_PARAMETER_SPACE];
};

struct mmal_msg_port_parameter_set_reply 
{
	u32 status;	// enum mmal_msg_status todo: how does this differ to the one in the header?
};

// port parameter getting //

struct mmal_msg_port_parameter_get 
{
	u32 component_handle;							// component //
	u32 port_handle;								// port //
	u32 id;											// Parameter ID  //
	u32 size;										// Parameter size //
};

struct mmal_msg_port_parameter_get_reply 
{
	u32 status;										// Status of mmal_port_parameter_get call //
	u32 id;											// Parameter ID  //
	u32 size;										// Parameter size //
	u32 value[MMAL_WORKER_PORT_PARAMETER_SPACE];
};


struct mmal_msg_event_eos 							// Structs for each of the event message payloads //
{
	u32 port_type;									// Type of port that received the end of stream //
	u32 port_index;									// Index of port that received the end of stream //
};


struct mmal_msg_event_format_changed 				// Format changed event data. //
{
	u32 buffer_size_min;							// Minimum size of buffers the port requires //
	u32 buffer_num_min;								// Minimum number of buffers the port requires //
	u32 buffer_size_recommended;					// Size of buffers the port recommends for optimal performance. A value of zero means no special recommendation.
	u32 buffer_num_recommended;						// Number of buffers the port recommends for optimal performance. A value of zero means no special recommendation.

	u32 es_ptr;
	struct mmal_es_format 							format;
	union mmal_es_specific_format 					es;
	u8 extradata[MMAL_FORMAT_EXTRADATA_MAX_SIZE];
};

struct mmal_msg_event_to_host 
{
	u32 client_component;							// component context //
	u32 port_type;
	u32 port_num;
	u32 cmd;
	u32 length;
	u8 data[MMAL_WORKER_EVENT_SPACE];
	u32 delayed_buffer;
};

struct mmal_msg 									// all mmal messages are serialised through this structure //
{
	// header //
	struct mmal_msg_header 							h;
	// payload //
	union 
	{
		struct mmal_msg_version 					version;

		struct mmal_msg_component_create 			component_create;
		struct mmal_msg_component_create_reply 		component_create_reply;

		struct mmal_msg_component_destroy 			component_destroy;
		struct mmal_msg_component_destroy_reply 	component_destroy_reply;

		struct mmal_msg_component_enable 			component_enable;
		struct mmal_msg_component_enable_reply 		component_enable_reply;

		struct mmal_msg_component_disable 			component_disable;
		struct mmal_msg_component_disable_reply 	component_disable_reply;

		struct mmal_msg_port_info_get 				port_info_get;
		struct mmal_msg_port_info_get_reply 		port_info_get_reply;

		struct mmal_msg_port_info_set 				port_info_set;
		struct mmal_msg_port_info_set_reply 		port_info_set_reply;

		struct mmal_msg_port_action_port 			port_action_port;
		struct mmal_msg_port_action_handle 			port_action_handle;
		struct mmal_msg_port_action_reply 			port_action_reply;

		struct mmal_msg_buffer_from_host 			buffer_from_host;

		struct mmal_msg_port_parameter_set 			port_parameter_set;
		struct mmal_msg_port_parameter_set_reply 	port_parameter_set_reply;
		struct mmal_msg_port_parameter_get 			port_parameter_get;
		struct mmal_msg_port_parameter_get_reply 	port_parameter_get_reply;

		struct mmal_msg_event_to_host 				event_to_host;

		u8 payload[MMAL_MSG_MAX_PAYLOAD];
	} u;
};
//  MY OWN STRUCTS NEEDED FOR MY MMAL PROGRAM FLOW i need to ensure the correct alignment and length, somehow the compiler mess up here
struct mmal_buffer_header_wire32 {
    u32 next;
    u32 priv;
    u32 cmd;
    u32 data;           							// 16
    u32 alloc_size;
    u32 length;
    u32 offset;
    u32 flags;          							// 32
    u32 pts_lo;
    u32 pts_hi;   									// replaces s64 pts
    u32 dts_lo;
    u32 dts_hi;   									// replaces s64 dts 48
    u32 type;
    u32 user_data; 									// 52
};

struct mmal_msg_buffer_from_host_wire32
{
    struct mmal_driver_buffer 						drvbuf;     // you set these fields → keep as struct (16)
    struct mmal_driver_buffer 						drvbuf_ref; // 16
    struct mmal_buffer_header_wire32 				buffer_header; // you patch these → keep (56)
    struct mmal_buffer_header_type_specific 		buffer_header_type_specific;

    s32 is_zero_copy;              					// 4
    s32 has_reference;             					// 4
    u32 payload_in_message;        					// 4

    u8 short_data[128];           					// unchanged, stays zero - total: 16 + 16 + 56 + 40 + 4 + 4 + 4 + 128 = 268 bytes
};

struct VCSM_Alloc_MEM_Msg
{
    vc_sm_msg_hdr_t    								hdr;
    vc_sm_alloc_t      								body;
};

struct VCSM_Alloc_MEM_Reply
{
    vc_sm_msg_hdr_t          						hdr;
    vc_sm_alloc_result_t     						body;
};

struct VCSM_Import_MEM_Msg          				// i need to define them before i can use them !!!
{
        vc_sm_msg_hdr_t                 			hdr;
		vc_sm_import                    			body;
};

struct VCSM_Import_MEM_Reply
{
        vc_sm_import_result             			body;
};
struct VCSM_Lock_MEM_Msg
{
        vc_sm_msg_hdr_t                 			hdr;
        vc_sm_lock_unlock_t             			body;
};
struct VCSM_Lock_MEM_Reply    
{
        vc_sm_lock_result_t             			body;
};
struct VCSM_Free_MEM_Msg
{
        vc_sm_msg_hdr_t                 			hdr;
        vc_sm_free_t                    			body;
};
struct VCSM_Free_MEM_Reply    
{
        vc_sm_result_t                  			body;
};
//  okay, i create my own structs with header and payload since i want them to be stored globally 
// because i am backward engineering the vc04 decoder,
//  means i have to debug a lot and between the states, look at initializeMMAL() 
// and def_members.h section "MMAL predefined messages" to get the idea.
struct MMAL_Component_Create_Msg            		//  1. Create component
{
    mmal_msg_header                 				hdr;
    mmal_msg_component_create       				msg;
};
struct MMAL_Component_Create_Reply
{
    mmal_msg_header                 				hdr;
    mmal_msg_component_create_reply 				msg;
};
struct MMAL_Port_Info_Get_Msg               		//  2 & 3. Snapshot – get initial port state in and out (before primePortFormat*MMAL)
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_info_get          				msg;
};
struct MMAL_Port_Info_Get_Reply
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_info_get_reply    				msg;
};
struct MMAL_Port_Info_Set_Msg               		//  4 & 5. Send SET in and out (this is where data from step 2 & 3 is sent)
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_info_set          				msg;
};
struct MMAL_Port_Info_Set_Reply    
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_info_set_reply    				msg;
};
struct MMAL_Component_Enable_Msg            		//  5. Enable component
{
    mmal_msg_header                 				hdr;
    mmal_msg_component_enable       				msg;
};
struct MMAL_Component_Enable_Reply          		// correct
{
    mmal_msg_header                 				hdr;
    mmal_msg_component_enable_reply 				msg;
};
struct MMAL_Port_Parameter_Set_Msg          		//  6 & 7. Zero-copy parameter set in and out
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_parameter_set     				msg;
};
struct MMAL_Port_Parameter_Set_Reply    
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_parameter_set_reply 				msg;
};
struct MMAL_Port_Action_Msg                 		//  8. Enable ports
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_action_port       				msg;
};
struct MMAL_Port_Action_Reply_Msg
{
    mmal_msg_header                 				hdr;
    mmal_msg_port_action_reply      				msg;
};
struct MMAL_Buffer_From_Host_Msg            		//  9. Runtime buffer flow bidirectional
{
    mmal_msg_header                  				hdr;
    mmal_msg_buffer_from_host_wire32  				msg;
};