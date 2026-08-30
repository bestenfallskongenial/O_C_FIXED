//----------------------------------------------------------------------------------------------------------------------------------------------------
// h264_hardware_decoder.h
//----------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _H264_DECODER_CLASS_H
#define _H264_DECODER_CLASS_H


#define EMPTYLOG 255

#define __H264_DECODER_DEBUG_INIT__
#define __H264_DECODER_DEBUG_RUNTIME__


#include <circle/types.h> 
#include <circle/bcm2835.h>

#include "h264_decoder_defs.h"


#include <vc4/vchi/vchi.h>
#include <vc4/vchiq/vchiq.h>

#include "interface/vcos/vcos.h"
//#include <circle/logger.h>

//#include "interface/vcos/vcos_event.h"


#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <GLES2/gl2.h>
#include <GLES/glext.h>
#include <GLES2/gl2ext.h>

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);
//----------------------------------------------------------------------------------------------------------------------------------------------------
struct mmal_buffer_header_wire32 {
    u32 next;
    u32 priv;
    u32 cmd;
    u32 data;           // 16
    u32 alloc_size;
    u32 length;
    u32 offset;
    u32 flags;          // 32
    u32 pts_lo;
    u32 pts_hi;   // replaces s64 pts
    u32 dts_lo;
    u32 dts_hi;   // replaces s64 dts 48
    u32 type;
    u32 user_data; // 52
};

struct mmal_msg_buffer_from_host_wire32
{
    struct mmal_driver_buffer drvbuf;     // you set these fields → keep as struct (16)
    struct mmal_driver_buffer drvbuf_ref; // 16
    struct mmal_buffer_header_wire32 buffer_header; // you patch these → keep (56)
    struct mmal_buffer_header_type_specific buffer_header_type_specific;

    s32 is_zero_copy;              // 4
    s32 has_reference;             // 4
    u32 payload_in_message;        // 4

    u8 short_data[128];           // unchanged, stays zero
}; // total: 16 + 16 + 56 + 40 + 4 + 4 + 4 + 128 = 268 bytes

//  1. Create component
struct MMAL_Component_Create_Msg            // correct
{
    mmal_msg_header                 hdr;
    mmal_msg_component_create       msg;
};
struct MMAL_Component_Create_Reply_Msg      // correct
{
    mmal_msg_header                 hdr;
    mmal_msg_component_create_reply msg;
};
//  5. Enable component
struct MMAL_Component_Enable_Msg            // correct
{
    mmal_msg_header                 hdr;
    mmal_msg_component_enable       msg;
};
struct MMAL_Component_Enable_Reply_Msg      // correct
{
    mmal_msg_header                 hdr;
    mmal_msg_component_enable_reply msg;
};
//  2. Snapshot – get initial port state (before any modification)
struct MMAL_Port_Info_Get_Msg               // correct
{
    mmal_msg_header                 hdr;
    mmal_msg_port_info_get          msg;
};
struct MMAL_Port_Info_Get_Reply_Msg         // correct
{
    mmal_msg_header                 hdr;
    mmal_msg_port_info_get_reply    msg;
};
//  4. Send SET (this is where data from step 3 is sent)
struct MMAL_Port_Info_Set_Msg
{
    mmal_msg_header                 hdr;
    mmal_msg_port_info_set          msg;
};
struct MMAL_Port_Info_Set_Reply_Msg
{
    mmal_msg_header                 hdr;
    mmal_msg_port_info_set_reply    msg;
};
//  7. Zero-copy parameter set
struct MMAL_Port_Parameter_Set_Msg
{
    mmal_msg_header                 hdr;
    mmal_msg_port_parameter_set     msg;
};
struct MMAL_Port_Parameter_Set_Reply_Msg
{
    mmal_msg_header                 hdr;
    mmal_msg_port_parameter_set_reply msg;
};
//  9. Enable ports
struct MMAL_Port_Action_Msg
{
    mmal_msg_header                 hdr;
    mmal_msg_port_action_port       msg;
};
struct MMAL_Port_Action_Reply_Msg
{
    mmal_msg_header                 hdr;
    mmal_msg_port_action_reply      msg;
};
//  10. Runtime buffer flow
struct MMAL_Buffer_From_Host_Msg
{
    mmal_msg_header                   hdr;
    mmal_msg_buffer_from_host_wire32  msg;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
class CH264Decoder
{
public:
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CONSTRUCTOR / DECONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------------------------------------
                CH264Decoder();
               ~CH264Decoder();
// api.cpp -------------------------------------------------------------------------------------------------------------------------------------------
bool initialize                                           ( u32         InBufferHandle,         // my input buffer handle from smem
                                                            u32         InBufferPointer,        // i got the feeling i rather need this
                                                            u32         InBufferSize,           // my allocated input buffer size 
                                                            u32         OutBufferHandleA,       // my output buffer handle a from smem
                                                            u32         OutBufferPointerA,      // i got the feeling i rather need this
                                                            u32         OutBufferHandleB,       // my output buffer handle b from smem
                                                            u32         OutBufferPointerB,      // i got the feeling i rather need this
                                                            u32         OutBufferSize,          // my allocated output buffer size
                                                            u32         ResolutionX,            // obvious, right?
                                                            u32         ResolutionY,            // obvious, right?
                                                            EGLDisplay  eglDisplay,             // EGL display connection
                                                            EGLContext  eglContext );           // EGL rendering context
bool createTextures                                       (   );
bool framePoller                                          ( u32 frame_offset, u32 frame_length);
// runtime.cpp ---------------------------------------------------------------------------------------------------------------------------------------
private:
bool bufferReady                                          ( u32 handle);
bool queueOutputBuffer                                    ( MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size);
bool queueInputBuffer                                     ( MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length);
// helper.cpp ----------------------------------------------------------------------------------------------------------------------------------------
static void callBack                                      ( void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle );
void initHeader                                           ( mmal_msg_header& hdr, u32 type);
u32  NextTransId                                          ( u32 &tid );
bool getVCHIstate                                         (   );
bool initEvents                                           (   );
bool checkGLerror                                         (   );
bool sendAndWait                                          (   const void *msg, size_t msg_size, void *rx_msg, size_t max_reply_len, size_t *actual_reply_len );
// init.cpp ------------------------------------------------------------------------------------------------------------------------------------------
bool openService                                          (   SERVICE_CREATION_T &tx);
bool createComponent                                      (   MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply_Msg& rx);
bool getPortInfo                                          (   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply_Msg& rx);
bool setPortInfo                                          (   MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply_Msg& rx);
bool enableComponent                                      (   MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply_Msg& rx);
bool setZeroCopyMode                                      (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src,  MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply_Msg& rx);
bool enablePort                                           (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx);

void PrimeOutputBufferBody                                (   MMAL_Buffer_From_Host_Msg& tx);
void PrimeInputBufferBody                                 (   MMAL_Buffer_From_Host_Msg& tx);

void setPortFormatInput                                   ( const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Info_Set_Msg& tx);
void setPortFormatOutput                                  ( const MMAL_Port_Info_Get_Reply_Msg& src, MMAL_Port_Info_Set_Msg& tx);
// log.cpp -------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __H264_DECODER_DEBUG_INIT__
void storeLog                                             (   const char* label, u32  value1 = EMPTYLOG, u32  value2 = EMPTYLOG, u32  value3 = EMPTYLOG, u32  value4 = EMPTYLOG );        // NEW
void storeMsg                                             (   const char* label, const void* tx_msg, u32 total_size);
void nextline();
void Log_openService                                      ( const SERVICE_CREATION_T &src);
void Log_createComponent                                  ( const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply_Msg& rx);
void Log_getPortInfo                                      ( const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply_Msg& rx);
void Log_setPortInfo                                      ( const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply_Msg& rx);
void Log_enableComponent                                  ( const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply_Msg& rx);
void Log_enablePort                                       ( const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx);
void Log_setZeroCopyMode                                  ( const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply_Msg& rx);
void Log_BufferFromHost                                   ( const MMAL_Buffer_From_Host_Msg& rx);
#endif
// void Log_BufferBody                                    ( const mmal_msg_buffer_from_host_wire32& msg );
// void Log_queueBufferFromHost                           ( const MMAL_Buffer_From_Host_Msg& tx );
// ---------------------------------------------------------------------------------------------------------------------------------------------------
public:
        GLuint                                              m_Texture = 0;                                              // must be exposed

        u32                                                 m_CharIndex = 0;
        char                                                m_DebugCharArray[MMAL_MAX_DEBUG_FILE_LENGTH] = { 0 };       // is exposed for 
private:
        bool                                                m_FirstFrameQueued = false;    
//---------------------------------------------------------------------------------------------------------------------------------------------------- 
        u32 m_InputBufferHandle;
        u32 m_InputBufferPointer;

        u32 m_InputBufferSize;

        u32 m_OutputBufferHandleA;
        u32 m_OutputBufferPointerA;

        u32 m_OutputBufferHandleB;
        u32 m_OutputBufferPointerB;

        u32 m_OutputBufferSize;

        u32 m_ResolutionX;
        u32 m_ResolutionY;

        u32 m_ComponentHandle;

        u32 m_VCSMHandleA;
        u32 m_VCSMHandleB;

        u32 m_InputPortHandle;
        u32 m_OutputPortHandle;  
        
        u32 m_CurrentHandle;

        VCHI_INSTANCE_T                                     m_VCHIInstance;                                             // global vchi instance from bcm_host.c via GetVCHIstate
        VCHI_CONNECTION_T*                                  m_Connection;                                               // global vchi instance from bcm_host.c via GetVCHIstate
        VCOS_EVENT_T                                        m_VCOSevent;                                                // vcos event from vcos for the callback control
        VCHI_SERVICE_HANDLE_T                               m_ServiceHandle;                                            // vchi service handle 

        EGLDisplay                                          m_eglDisplay;                                               // egl display handle 
        EGLContext                                          m_eglContext;                                               // egl contey handle
        EGLImageKHR                                         m_EGLimage                  = nullptr;

        u32                                                 m_TransactionId;                                            // to get unique message numbers
        //  0. Open MMAL service (transport only)
//----------------------------------------------------------------------------------------------------------------------------------------------------        
SERVICE_CREATION_T                 m_ServiceCreate              = {};   //  SEND                    - sent via `openService()`
                                                                        //  RECEIVE none            - open VCHI/MMAL control channel. No MMAL protocol yet.
//  1. Create component
MMAL_Component_Create_Msg          m_ComponentCreateTx          = {};   //  SEND
MMAL_Component_Create_Reply_Msg    m_ComponentCreateRx          = {};   //  RECEIVE                 - create `ril.video_decode`, receive `component_handle`. This handle is stored and used everywhere later.
//  2. Snapshot A – get initial port state (before any modification)
MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Input_A      = {};   //  Input port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Input_A      = {};   //  Input port RECEIVE

MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Output_A     = {};   //  Output port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Output_A     = {};   //  Output port RECEIVE     - Snapshot A is the **authoritative VPU-owned baseline**. You must base `PORT_INFO_SET` on this snapshot.
//  3. Build SET messages locally (nothing sent yet)
MMAL_Port_Info_Set_Msg             m_PortInfoSetTx_Input        = {};   //  Input port WRITE

MMAL_Port_Info_Set_Msg             m_PortInfoSetTx_Output       = {};   //  Output port WRITE       - * This step **does not send anything**. It only prepares TX structs using Snapshot A.
//  4. Send SET (this is where data from step 3 is sent)
// MMAL_Port_Info_Set_Msg             m_PortInfoSetTx_Input     = {};   //  Input port SEND
MMAL_Port_Info_Set_Reply_Msg       m_PortInfoSetRx_Input        = {};   //  Input port RECEIVE

// MMAL_Port_Info_Set_Msg             m_PortInfoSetTx_Output    = {};   //  Output port SEND
MMAL_Port_Info_Set_Reply_Msg       m_PortInfoSetRx_Output       = {};   //  Output port RECEIVE     - Commit format/buffer changes to the VPU.
//  5. Enable component
MMAL_Component_Enable_Msg          m_ComponentEnableTx          = {};   //  SEND
MMAL_Component_Enable_Reply_Msg    m_ComponentEnableRx          = {};   //  RECEIVE                 - Component must be enabled before parameters or port actions.
//  6. Snapshot B – get port state after enable
MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Input_B      = {};   //  Input port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Input_B      = {};   //  Input port RECEIVE

MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Output_B     = {};   //  Output port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Output_B     = {};   //  Output port RECEIVE     - Snapshot B reflects VPU state **after enable**. Used as the base for zero-copy parameters.
//  7. Zero-copy parameter set
MMAL_Port_Parameter_Set_Msg        m_PortParamTx_Input          = {};   //  Input port SEND         - built using `m_PortInfoGetRx_Input_B`
MMAL_Port_Parameter_Set_Reply_Msg  m_PortParamRx_Input          = {};   //  Input port RECEIVE

MMAL_Port_Parameter_Set_Msg        m_PortParamTx_Output         = {};   //  Output port SEND        - built using `m_PortInfoGetRx_Output_B`
MMAL_Port_Parameter_Set_Reply_Msg  m_PortParamRx_Output         = {};   //  Output port RECEIVE     - Enable zero-copy **before** ports are enabled.
//  8. Snapshot C – get port state after zero-copy
MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Input_C      = {};   //  Input port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Input_C      = {};   //  Input port RECEIVE

MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Output_C     = {};   //  Output port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Output_C     = {};   //  Output port RECEIVE     - Snapshot C is the **final authoritative state** before enabling ports.
//  9. Enable ports
MMAL_Port_Action_Msg               m_PortActionTx_Input         = {};   //  Input port SEND         - uses handle from `m_PortInfoGetRx_Input_C`
MMAL_Port_Action_Reply_Msg         m_PortActionRx_Input         = {};   //  Input port RECEIVE

MMAL_Port_Action_Msg               m_PortActionTx_Output        = {};   //  Output port SEND        - uses handle from `m_PortInfoGetRx_Output_C`
MMAL_Port_Action_Reply_Msg         m_PortActionRx_Output        = {};   //  Output port RECEIVE     - Ports become live. After this, buffers may flow.
//  10. Runtime buffer flow
MMAL_Buffer_From_Host_Msg          m_BufferFromHostTx_Input     = {};   //  Input buffers SEND
MMAL_Buffer_From_Host_Msg          m_BufferFromHostRx_Input     = {};   //  Input buffers RECEIVE   (same physical layout, semantic RX)

MMAL_Buffer_From_Host_Msg          m_BufferFromHostTx_Output    = {};   //  Output buffers SEND     (ping/pong)
MMAL_Buffer_From_Host_Msg          m_BufferFromHostRx_Output    = {};   //  Output buffers RECEIVE  (ping/pong) - Buffer ownership transfer and frame delivery.
       
MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Input_D      = {};   //  Input port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Input_D      = {};   //  Input port RECEIVE

MMAL_Port_Info_Get_Msg             m_PortInfoGetTx_Output_D     = {};   //  Output port SEND
MMAL_Port_Info_Get_Reply_Msg       m_PortInfoGetRx_Output_D     = {};   //  Output port RECEIVE
// Core invariant (unchanged, explicit)
// Snapshot **A** → build **SET**
// Snapshot **B** → build **PARAM**
// Snapshot **C** → build **ACTION**
// Snapshot **D** → buffer runtime
// Buffer TX/RX is independent of snapshots
};

#endif