#pragma once

#include <stdint.h>
#include <stddef.h>

// ---- basic types ----
typedef uint32_t        u32;
typedef int32_t         s32;

// ---- GL / EGL stubs ----
typedef unsigned int    GLenum;
typedef unsigned int    GLuint;
typedef void*           EGLDisplay;
typedef void*           EGLContext;
typedef void*           EGLImageKHR;
typedef void*           EGLClientBuffer;

#ifndef GL_NO_ERROR
#define GL_NO_ERROR 0
#endif

#ifndef GL_INVALID_ENUM
#define GL_INVALID_ENUM 0x0500
#endif

#ifndef GL_INVALID_VALUE
#define GL_INVALID_VALUE 0x0501
#endif

#ifndef GL_INVALID_OPERATION
#define GL_INVALID_OPERATION 0x0502
#endif

#ifndef GL_OUT_OF_MEMORY
#define GL_OUT_OF_MEMORY 0x0505
#endif

#ifndef GL_INVALID_FRAMEBUFFER_OPERATION
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#endif

#ifndef GL_TEXTURE_2D
#define GL_TEXTURE_2D 0x0DE1
#endif

#ifndef GL_LINEAR
#define GL_LINEAR 0x2601
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#ifndef EGL_NO_IMAGE_KHR
#define EGL_NO_IMAGE_KHR ((EGLImageKHR)0)
#endif

#ifndef EGL_IMAGE_BRCM_VCSM
#define EGL_IMAGE_BRCM_VCSM 0x33A5
#endif

// ---- VCHI / VCOS stubs ----
typedef void*           VCHI_INSTANCE_T;
typedef void*           VCHI_CONNECTION_T;
typedef void*           VCHI_SERVICE_HANDLE_T;
typedef int32_t         VCHI_CALLBACK_REASON_T;
typedef void            (*VCHI_CALLBACK_T)(void*, VCHI_CALLBACK_REASON_T, void*);

typedef struct
{
    int dummy;
} VCOS_EVENT_T;

typedef int VCOS_STATUS_T;

#ifndef VCOS_SUCCESS
#define VCOS_SUCCESS 0
#endif

#ifndef VCHI_CALLBACK_MSG_AVAILABLE
#define VCHI_CALLBACK_MSG_AVAILABLE 1
#endif

#ifndef VCHI_FLAGS_BLOCK_UNTIL_QUEUED
#define VCHI_FLAGS_BLOCK_UNTIL_QUEUED 1
#endif

#ifndef VCHI_FLAGS_NONE
#define VCHI_FLAGS_NONE 0
#endif

// ---- service creation stub ----
struct SERVICE_CREATION_VERSION_T
{
    uint32_t version;
    uint32_t version_min;
};

struct SERVICE_CREATION_T
{
    SERVICE_CREATION_VERSION_T version;
    int32_t                    service_id;
    VCHI_CONNECTION_T          connection;

    uint32_t                   rx_fifo_size;
    uint32_t                   tx_fifo_size;

    VCHI_CALLBACK_T            callback;
    void*                      callback_param;

    int                        want_unaligned_bulk_rx;
    int                        want_unaligned_bulk_tx;
    int                        want_crc;
};

// ---- EGL image info stub ----
struct egl_image_brcm_vcsm_info
{
    uint32_t width;
    uint32_t height;
    uint32_t vcsm_handle;
};

// ---- VCSM message header ----
struct vc_sm_msg_hdr_t
{
    u32 type;
    u32 trans_id;
};

// ---- MMAL message header ----
struct mmal_msg_header
{
    u32 magic;
    u32 type;
    u32 control_service;
    u32 context;
    u32 status;
    u32 padding;
};

// ---- generic MMAL/VCSM message stubs ----
struct VCSM_Import_MEM_Msg
{
    vc_sm_msg_hdr_t hdr;
    struct
    {
        u32  type;
        u32  addr;
        u32  size;
        u32  kernel_id;
        u32  allocator;
        char name[32];
    } body;
};

struct VCSM_Import_MEM_Reply
{
    vc_sm_msg_hdr_t hdr;
    struct
    {
        u32 res_handle;
    } body;
};

struct VCSM_Lock_MEM_Msg
{
    vc_sm_msg_hdr_t hdr;
    struct
    {
        u32 res_handle;
        u32 res_mem;
    } body;
};

struct VCSM_Lock_MEM_Reply
{
    vc_sm_msg_hdr_t hdr;
    struct
    {
        u32 res_handle;
        u32 res_mem;
    } body;
};

struct VCSM_Free_MEM_Msg
{
    vc_sm_msg_hdr_t hdr;
    struct
    {
        u32 res_handle;
        u32 res_mem;
    } body;
};

struct VCSM_Free_MEM_Reply
{
    vc_sm_msg_hdr_t hdr;
    struct
    {
        u32 success;
    } body;
};

// ---- MMAL port/info/body stubs ----
struct MMAL_Port_Common_Stub
{
    u32 buffer_num;
    u32 buffer_size;
};

struct MMAL_Video_Crop_Stub
{
    u32 x;
    u32 y;
    u32 width;
    u32 height;
};

struct MMAL_ES_Video_Stub
{
    u32 width;
    u32 height;
    MMAL_Video_Crop_Stub crop;
};

struct MMAL_ES_Stub
{
    MMAL_ES_Video_Stub video;
};

struct MMAL_Format_Stub
{
    u32 encoding;
    u32 encoding_variant;
};

struct MMAL_Port_Info_Get_Msg
{
    mmal_msg_header hdr;
    struct
    {
        u32 component_handle;
        u32 port_type;
    } msg;
};

struct MMAL_Port_Info_Get_Reply
{
    mmal_msg_header hdr;
    struct
    {
        u32 status;
        u32 component_handle;
        u32 port_handle;
        u32 port_type;
        u32 port_index;
        u32 port;
        MMAL_Format_Stub format;
        MMAL_ES_Stub     es;
        MMAL_Port_Common_Stub port_info_alias;
        MMAL_Port_Common_Stub port;
    } msg;
};

struct MMAL_Port_Info_Set_Msg
{
    mmal_msg_header hdr;
    struct
    {
        u32 component_handle;
        u32 port_type;
        u32 port_index;
        MMAL_Port_Common_Stub port;
        MMAL_Format_Stub      format;
        MMAL_ES_Stub          es;
    } msg;
};

struct MMAL_Port_Info_Set_Reply
{
    mmal_msg_header hdr;
    struct
    {
        u32 status;
    } msg;
};

struct MMAL_Component_Create_Msg
{
    mmal_msg_header hdr;
    struct
    {
        char name[128];
    } msg;
};

struct MMAL_Component_Create_Reply
{
    mmal_msg_header hdr;
    struct
    {
        u32 status;
        u32 component_handle;
    } msg;
};

struct MMAL_Component_Enable_Msg
{
    mmal_msg_header hdr;
    struct
    {
        u32 component_handle;
    } msg;
};

struct MMAL_Component_Enable_Reply
{
    mmal_msg_header hdr;
    struct
    {
        u32 status;
    } msg;
};

struct MMAL_Port_Parameter_Set_Msg
{
    mmal_msg_header hdr;
    struct
    {
        u32 component_handle;
        u32 port_handle;
        u32 id;
        u32 size;
        u32 value[16];
    } msg;
};

struct MMAL_Port_Parameter_Set_Reply
{
    mmal_msg_header hdr;
    struct
    {
        u32 status;
    } msg;
};

struct MMAL_Port_Action_Msg
{
    mmal_msg_header hdr;
    struct
    {
        u32 component_handle;
        u32 port_handle;
        u32 action;
        u32 port;
    } msg;
};

struct MMAL_Port_Action_Reply_Msg
{
    mmal_msg_header hdr;
    struct
    {
        u32 status;
    } msg;
};

struct MMAL_Buffer_Header_Stub
{
    u32 data;
    u32 alloc_size;
    u32 length;
    u32 offset;
    u32 flags;
    u32 pts_lo;
    u32 pts_hi;
    u32 dts_lo;
    u32 dts_hi;
};

struct MMAL_DrvBuf_Stub
{
    u32 magic;
    u32 component_handle;
    u32 port_handle;
    u32 client_context;
};

struct MMAL_Buffer_From_Host_Msg
{
    mmal_msg_header hdr;
    struct
    {
        MMAL_DrvBuf_Stub       drvbuf;
        MMAL_Buffer_Header_Stub buffer_header;
        u32 is_zero_copy;
        u32 has_reference;
        u32 payload_in_message;
    } msg;
};

struct MMAL_Buffer_To_Host_Msg
{
    mmal_msg_header hdr;
    struct
    {
        MMAL_Buffer_Header_Stub buffer_header;
    } msg;
};

// ---- external helpers / APIs ----
void vc_host_get_vchi_state(VCHI_INSTANCE_T* instance, VCHI_CONNECTION_T* connection);

VCOS_STATUS_T vcos_event_create(VCOS_EVENT_T* event, const char* name);
VCOS_STATUS_T vcos_event_wait(VCOS_EVENT_T* event);
void          vcos_event_signal(VCOS_EVENT_T* event);

int vchi_service_open(VCHI_INSTANCE_T instance, SERVICE_CREATION_T* setup, VCHI_SERVICE_HANDLE_T* handle);
int vchi_msg_queue(VCHI_SERVICE_HANDLE_T handle, const void* msg, uint32_t size, uint32_t flags, void* msg_handle);
int vchi_msg_dequeue(VCHI_SERVICE_HANDLE_T handle, void* msg, uint32_t max_size, uint32_t* actual_size, uint32_t flags);

GLenum      glGetError(void);
void        glGenTextures(int n, GLuint* textures);
void        glBindTexture(GLenum target, GLuint texture);
void        glTexParameteri(GLenum target, GLenum pname, int value);
void        glEGLImageTargetTexture2DOES(GLenum target, void* image);

EGLImageKHR eglCreateImageKHR(EGLDisplay display, EGLContext context, int target, EGLClientBuffer buffer, const int* attrib_list);
int         eglDestroyImageKHR(EGLDisplay display, EGLImageKHR image);

void CleanAndInvalidateDataCacheRange(uintptr_t addr, size_t size);

// ---- utility / log stubs ----
extern char*        m_bufferLog;
extern uint32_t     vc04_logIndex;

void storeLog(char* buffer, uint32_t& index, const char* msg);
void storeLog(char* buffer, uint32_t& index, const char* msg, uint32_t v1);
void storeLog(char* buffer, uint32_t& index, const char* msg, uint32_t v1, uint32_t v2);
void storeLog(char* buffer, uint32_t& index, const char* msg, uint32_t v1, uint32_t v2, uint32_t v3);
void storeLog(char* buffer, uint32_t& index, const char* msg, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
void storeMsg(char* buffer, uint32_t& index, const char* msg, const void* data, uint32_t size);
void nextline(char* buffer, uint32_t& index);

// ---- external constants / macros stubs ----
#ifndef BUS_ADDRESS
#define BUS_ADDRESS(x) ((u32)(x))
#endif

#ifndef VC_SM_VER
#define VC_SM_VER 1
#endif
#ifndef VC_SM_MIN_VER
#define VC_SM_MIN_VER 1
#endif
#ifndef VC_MMAL_VER
#define VC_MMAL_VER 1
#endif
#ifndef VC_MMAL_MIN_VER
#define VC_MMAL_MIN_VER 1
#endif
#ifndef MMAL_MAGIC
#define MMAL_MAGIC 0x4D4D414C
#endif

#ifndef VC_SM_ALLOC_NON_CACHED
#define VC_SM_ALLOC_NON_CACHED 0
#endif

#ifndef VC_SM_MSG_TYPE_IMPORT
#define VC_SM_MSG_TYPE_IMPORT 1
#endif
#ifndef VC_SM_MSG_TYPE_LOCK
#define VC_SM_MSG_TYPE_LOCK 2
#endif
#ifndef VC_SM_MSG_TYPE_FREE
#define VC_SM_MSG_TYPE_FREE 3
#endif

#ifndef MMAL_MSG_TYPE_COMPONENT_CREATE
#define MMAL_MSG_TYPE_COMPONENT_CREATE 0x100
#endif
#ifndef MMAL_MSG_TYPE_PORT_INFO_GET
#define MMAL_MSG_TYPE_PORT_INFO_GET 0x101
#endif
#ifndef MMAL_MSG_TYPE_PORT_INFO_SET
#define MMAL_MSG_TYPE_PORT_INFO_SET 0x102
#endif
#ifndef MMAL_MSG_TYPE_COMPONENT_ENABLE
#define MMAL_MSG_TYPE_COMPONENT_ENABLE 0x103
#endif
#ifndef MMAL_MSG_TYPE_PORT_PARAMETER_SET
#define MMAL_MSG_TYPE_PORT_PARAMETER_SET 0x104
#endif
#ifndef MMAL_MSG_TYPE_PORT_ACTION
#define MMAL_MSG_TYPE_PORT_ACTION 0x105
#endif
#ifndef MMAL_MSG_TYPE_BUFFER_FROM_HOST
#define MMAL_MSG_TYPE_BUFFER_FROM_HOST 0x106
#endif
#ifndef MMAL_MSG_TYPE_BUFFER_TO_HOST
#define MMAL_MSG_TYPE_BUFFER_TO_HOST 0x107
#endif

#ifndef MMAL_MSG_PORT_ACTION_TYPE_ENABLE
#define MMAL_MSG_PORT_ACTION_TYPE_ENABLE 1
#endif

#ifndef MMAL_PARAMETER_ZERO_COPY
#define MMAL_PARAMETER_ZERO_COPY 1
#endif

#ifndef MMAL_ENCODING_H264
#define MMAL_ENCODING_H264 0x34363248
#endif
#ifndef MMAL_ENCODING_I420
#define MMAL_ENCODING_I420 0x30323449
#endif
#ifndef MMAL_ENCODING_VARIANT_H264_DEFAULT
#define MMAL_ENCODING_VARIANT_H264_DEFAULT 0
#endif

#ifndef MMAL_PORT_TYPE_INPUT
#define MMAL_PORT_TYPE_INPUT 0
#endif
#ifndef MMAL_PORT_TYPE_OUTPUT
#define MMAL_PORT_TYPE_OUTPUT 1
#endif

#ifndef MMAL_MSG_STATUS_SUCCESS
#define MMAL_MSG_STATUS_SUCCESS 0
#endif
#ifndef MMAL_MSG_STATUS_ENOMEM
#define MMAL_MSG_STATUS_ENOMEM 1
#endif
#ifndef MMAL_MSG_STATUS_ENOSPC
#define MMAL_MSG_STATUS_ENOSPC 2
#endif
#ifndef MMAL_MSG_STATUS_EINVAL
#define MMAL_MSG_STATUS_EINVAL 3
#endif
#ifndef MMAL_MSG_STATUS_ENOSYS
#define MMAL_MSG_STATUS_ENOSYS 4
#endif
#ifndef MMAL_MSG_STATUS_ENOENT
#define MMAL_MSG_STATUS_ENOENT 5
#endif
#ifndef MMAL_MSG_STATUS_ENXIO
#define MMAL_MSG_STATUS_ENXIO 6
#endif
#ifndef MMAL_MSG_STATUS_EIO
#define MMAL_MSG_STATUS_EIO 7
#endif
#ifndef MMAL_MSG_STATUS_ESPIPE
#define MMAL_MSG_STATUS_ESPIPE 8
#endif
#ifndef MMAL_MSG_STATUS_ECORRUPT
#define MMAL_MSG_STATUS_ECORRUPT 9
#endif
#ifndef MMAL_MSG_STATUS_ENOTREADY
#define MMAL_MSG_STATUS_ENOTREADY 10
#endif
#ifndef MMAL_MSG_STATUS_ECONFIG
#define MMAL_MSG_STATUS_ECONFIG 11
#endif
#ifndef MMAL_MSG_STATUS_EISCONN
#define MMAL_MSG_STATUS_EISCONN 12
#endif
#ifndef MMAL_MSG_STATUS_ENOTCONN
#define MMAL_MSG_STATUS_ENOTCONN 13
#endif
#ifndef MMAL_MSG_STATUS_EAGAIN
#define MMAL_MSG_STATUS_EAGAIN 14
#endif
#ifndef MMAL_MSG_STATUS_EFAULT
#define MMAL_MSG_STATUS_EFAULT 15
#endif

#ifndef MMAL_BUFFER_HEADER_FLAG_FRAME
#define MMAL_BUFFER_HEADER_FLAG_FRAME 0x1
#endif
#ifndef MMAL_BUFFER_HEADER_FLAG_KEYFRAME
#define MMAL_BUFFER_HEADER_FLAG_KEYFRAME 0x2
#endif

#ifndef NUMBER_INPUTBUFFER
#define NUMBER_INPUTBUFFER 1
#endif
#ifndef NUMBER_OUTPUTBUFFER
#define NUMBER_OUTPUTBUFFER 2
#endif

#ifndef SERVICENAMESTRING
#define SERVICENAMESTRING "SERVICE"
#endif
#ifndef SERVICEVERSIONSTRING
#define SERVICEVERSIONSTRING "VERSION"
#endif

#ifndef VCHIQ_MAKE_FOURCC
#define VCHIQ_MAKE_FOURCC(a,b,c,d) ((int32_t)(((a)<<24)|((b)<<16)|((c)<<8)|(d)))
#endif

// ---- kernel ----
class CKernel
{
public:

    // state / handles
    VCHI_INSTANCE_T          m_VCHIInstance;
    VCHI_CONNECTION_T        m_Connection;

    SERVICE_CREATION_T       m_ServiceCreateVCSM;
    SERVICE_CREATION_T       m_ServiceCreateMMAL;

    VCHI_SERVICE_HANDLE_T    m_ServiceHandleVCSM;
    VCHI_SERVICE_HANDLE_T    m_ServiceHandleMMAL;

    VCOS_EVENT_T             m_EventSMEM;
    VCOS_EVENT_T             m_EventMMAL;

    u32                      m_TransactionId;

    u32                      m_vc_handle[16];
    u32                      m_vc_pointer[16];

    // MMAL / EGL / texture state
    u32                      m_InputBufferHandle;
    u32                      m_InputBufferPointer;
    u32                      m_InputBufferSize;

    u32                      m_OutputBufferHandleA;
    u32                      m_OutputBufferPointerA;
    u32                      m_OutputBufferHandleB;
    u32                      m_OutputBufferPointerB;
    u32                      m_OutputBufferSize;

    u32                      m_ResolutionX;
    u32                      m_ResolutionY;

    EGLDisplay               m_eglDisplay;
    EGLContext               m_eglContext;
    EGLImageKHR              m_EGLimage;
    GLuint                   m_Texture;

    bool                     m_FirstFrameQueued;

    u32                      m_ComponentHandle;
    u32                      m_InputPortHandle;
    u32                      m_OutputPortHandle;

    // cached tx/rx messages
    MMAL_Component_Create_Msg        m_ComponentCreateTx;
    MMAL_Component_Create_Reply      m_ComponentCreateRx;

    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Input_A;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Input_A;
    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Output_A;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Output_A;

    MMAL_Port_Info_Set_Msg           m_PortInfoSetTx_Input;
    MMAL_Port_Info_Set_Reply         m_PortInfoSetRx_Input;
    MMAL_Port_Info_Set_Msg           m_PortInfoSetTx_Output;
    MMAL_Port_Info_Set_Reply         m_PortInfoSetRx_Output;

    MMAL_Component_Enable_Msg        m_ComponentEnableTx;
    MMAL_Component_Enable_Reply      m_ComponentEnableRx;

    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Input_B;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Input_B;
    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Output_B;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Output_B;

    MMAL_Port_Parameter_Set_Msg      m_PortParamTx_Input;
    MMAL_Port_Parameter_Set_Reply    m_PortParamRx_Input;
    MMAL_Port_Parameter_Set_Msg      m_PortParamTx_Output;
    MMAL_Port_Parameter_Set_Reply    m_PortParamRx_Output;

    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Input_C;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Input_C;
    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Output_C;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Output_C;

    MMAL_Port_Action_Msg             m_PortActionTx_Input;
    MMAL_Port_Action_Reply_Msg       m_PortActionRx_Input;
    MMAL_Port_Action_Msg             m_PortActionTx_Output;
    MMAL_Port_Action_Reply_Msg       m_PortActionRx_Output;

    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Input_D;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Input_D;
    MMAL_Port_Info_Get_Msg           m_PortInfoGetTx_Output_D;
    MMAL_Port_Info_Get_Reply         m_PortInfoGetRx_Output_D;

    MMAL_Buffer_From_Host_Msg        m_BufferFromHostTx_Input;
    MMAL_Buffer_From_Host_Msg        m_BufferFromHostTx_Output;
    MMAL_Buffer_From_Host_Msg        m_BufferFromHostTx_OutputA;
    MMAL_Buffer_From_Host_Msg        m_BufferFromHostTx_OutputB;
    MMAL_Buffer_To_Host_Msg          m_BufferFromHostRx_Input;
    MMAL_Buffer_To_Host_Msg          m_BufferFromHostRx_OutputA;
    MMAL_Buffer_To_Host_Msg          m_BufferFromHostRx_OutputB;

    // callbacks
    static void callbackVCSM(void* callback_param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);
    static void callbackMMAL(void* callback_param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);

    // base helpers
    void getStateVCHI();
    bool initEventsVCOS(VCOS_EVENT_T& event, const char* name);
    u32  convertAddress(void* buffer, size_t size);
    bool checkGLerrorMMAL();
    u32  NextTransId(u32& tid);

    void initHeaderVCSM(vc_sm_msg_hdr_t& hdr, u32 type);
    void initHeaderMMAL(mmal_msg_header& hdr, u32 type);
    void initHeader(mmal_msg_header& hdr, u32 type);   // stub alias used by code

    bool sendAndWaitVCHI(VCHI_SERVICE_HANDLE_T ServiceHandle,
                         VCOS_EVENT_T& VCOSevent,
                         const void* msg,
                         size_t msg_size,
                         void* rx_msg,
                         size_t max_reply_len,
                         size_t* actual_reply_len);

    bool openServiceVCHI(SERVICE_CREATION_T& tx,
                         uint32_t serviceVersion,
                         uint32_t serviceVersionMin,
                         int32_t service_id,
                         VCHI_CALLBACK_T cb,
                         void* cb_param,
                         VCHI_INSTANCE_T VCHIInstance,
                         VCHI_SERVICE_HANDLE_T& ServiceHandle);

    // VCSM
    bool initializeVCSM();
    bool importMemoryVCSM(void* buffer, size_t size, int slot, VCSM_Import_MEM_Msg& tx, VCSM_Import_MEM_Reply& rx);
    bool lockMemoryVCSM(int slot, VCSM_Lock_MEM_Msg& tx, VCSM_Lock_MEM_Reply& rx);
    bool freeMemoryVCSM(int slot, VCSM_Free_MEM_Msg& tx, VCSM_Free_MEM_Reply& rx);

    // MMAL init / runtime
    bool initializeMMAL(u32 InBufferHandle,
                        u32 InBufferPointer,
                        u32 InBufferSize,
                        u32 OutBufferHandleA,
                        u32 OutBufferPointerA,
                        u32 OutBufferHandleB,
                        u32 OutBufferPointerB,
                        u32 OutBufferSize,
                        u32 ResolutionX,
                        u32 ResolutionY,
                        EGLDisplay eglDisplay,
                        EGLContext eglContext);

    bool createTexturesMMAL();
    bool framePollerMMAL(u32 frame_offset, u32 frame_length);
    bool bufferReadyMMAL(u32 handle);

    bool queueOutputBufferMMAL(MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size);
    bool queueInputBufferMMAL(MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length);

    bool createComponent(MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply& rx);
    bool getPortInfoMMAL(u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply& rx);
    bool setPortInfoMMAL(MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply& rx);
    bool enableComponentMMAL(MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply& rx);
    bool setZeroCopyModeMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply& rx);
    bool enablePortMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx);

    // priming / patch helpers
    void PrimeOutputBufferBodyMMAL(MMAL_Buffer_From_Host_Msg& tx);
    void PrimeInputBufferBodyMMAL(MMAL_Buffer_From_Host_Msg& tx);
    void primePortFormatInputMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Info_Set_Msg& tx);
    void primePortFormatOutputMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Info_Set_Msg& tx);

    // debug log helpers used in code
    void Log_createComponent(const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply& rx);
    void Log_getPortInfo(const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply& rx);
    void Log_setPortInfo(const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply& rx);
    void Log_enableComponentMMAL(const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply& rx);
    void Log_setZeroCopyModeMMAL(const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply& rx);
    void Log_enablePort(const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg& rx);
};