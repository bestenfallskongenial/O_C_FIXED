#pragma once

#include <stdint.h>
#include <stddef.h>

#include "interface/vchi/vchi.h"
#include "interface/vcos/vcos.h"
#include "interface/mmal/mmal.h"
#include "interface/mmal/vc/mmal_vc_api.h"
#include "interface/vc_sm/vc_sm.h"

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

public:

    // ---- VCHI ----
    VCHI_INSTANCE_T          m_VCHIInstance;
    VCHI_CONNECTION_T        m_Connection;

    SERVICE_CREATION_T       m_ServiceCreateVCSM;
    SERVICE_CREATION_T       m_ServiceCreateMMAL;

    VCHI_SERVICE_HANDLE_T    m_ServiceHandleVCSM;
    VCHI_SERVICE_HANDLE_T    m_ServiceHandleMMAL;

    VCOS_EVENT_T             m_EventSMEM;
    VCOS_EVENT_T             m_EventMMAL;

    u32                      m_TransactionId;

    // ---- VCSM ----
    u32                      m_vc_handle[16];
    u32                      m_vc_pointer[16];

    // ---- MMAL ----
    u32                      m_ComponentHandle;
    u32                      m_InputPortHandle;
    u32                      m_OutputPortHandle;

    // ---- buffers ----
    u32                      m_InputBufferHandle;
    u32                      m_InputBufferPointer;
    u32                      m_InputBufferSize;

    u32                      m_OutputBufferHandleA;
    u32                      m_OutputBufferPointerA;
    u32                      m_OutputBufferHandleB;
    u32                      m_OutputBufferPointerB;
    u32                      m_OutputBufferSize;

    // ---- EGL / GL ----
    EGLDisplay               m_eglDisplay;
    EGLContext               m_eglContext;
    EGLImageKHR              m_EGLimage;
    GLuint                   m_Texture;

    u32                      m_ResolutionX;
    u32                      m_ResolutionY;

    bool                     m_FirstFrameQueued;

    // ---- cached messages ----
    MMAL_COMPONENT_CREATE_T              m_ComponentCreateTx;
    MMAL_COMPONENT_CREATE_REPLY_T        m_ComponentCreateRx;

    MMAL_PORT_INFO_GET_T                 m_PortInfoGetTx_Input_A;
    MMAL_PORT_INFO_GET_REPLY_T           m_PortInfoGetRx_Input_A;
    MMAL_PORT_INFO_GET_T                 m_PortInfoGetTx_Output_A;
    MMAL_PORT_INFO_GET_REPLY_T           m_PortInfoGetRx_Output_A;

    MMAL_PORT_INFO_SET_T                 m_PortInfoSetTx_Input;
    MMAL_PORT_INFO_SET_REPLY_T           m_PortInfoSetRx_Input;
    MMAL_PORT_INFO_SET_T                 m_PortInfoSetTx_Output;
    MMAL_PORT_INFO_SET_REPLY_T           m_PortInfoSetRx_Output;

    MMAL_COMPONENT_ENABLE_T              m_ComponentEnableTx;
    MMAL_COMPONENT_ENABLE_REPLY_T        m_ComponentEnableRx;

    MMAL_PORT_PARAMETER_SET_T            m_PortParamTx_Input;
    MMAL_PORT_PARAMETER_SET_REPLY_T      m_PortParamRx_Input;
    MMAL_PORT_PARAMETER_SET_T            m_PortParamTx_Output;
    MMAL_PORT_PARAMETER_SET_REPLY_T      m_PortParamRx_Output;

    MMAL_PORT_ACTION_T                   m_PortActionTx_Input;
    MMAL_PORT_ACTION_REPLY_T             m_PortActionRx_Input;
    MMAL_PORT_ACTION_T                   m_PortActionTx_Output;
    MMAL_PORT_ACTION_REPLY_T             m_PortActionRx_Output;

    MMAL_BUFFER_FROM_HOST_T              m_BufferFromHostTx_Input;
    MMAL_BUFFER_FROM_HOST_T              m_BufferFromHostTx_Output;
    MMAL_BUFFER_TO_HOST_T                m_BufferFromHostRx_Input;

    // ---- callbacks ----
    static void callbackVCSM(void* param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);
    static void callbackMMAL(void* param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);

    // ---- base ----
    void getStateVCHI();
    bool initEventsVCOS(VCOS_EVENT_T& event, const char* name);

    u32  convertAddress(void* buffer, size_t size);
    u32  NextTransId(u32& tid);

    void initHeaderVCSM(vc_sm_msg_hdr_t& hdr, u32 type);
    void initHeaderMMAL(mmal_msg_header& hdr, u32 type);

    bool sendAndWaitVCHI(VCHI_SERVICE_HANDLE_T handle,
                         VCOS_EVENT_T& event,
                         const void* msg,
                         size_t msg_size,
                         void* rx,
                         size_t rx_size,
                         size_t* rx_len);

    bool openServiceVCHI(SERVICE_CREATION_T& tx,
                         u32 version,
                         u32 version_min,
                         int32_t fourcc,
                         VCHI_CALLBACK_T cb,
                         void* cb_param,
                         VCHI_INSTANCE_T instance,
                         VCHI_SERVICE_HANDLE_T& handle);

    // ---- VCSM ----
    bool initializeVCSM();
    bool importMemoryVCSM(void* buffer, size_t size, int slot, VCSM_Import_MEM_Msg& tx, VCSM_Import_MEM_Reply& rx);
    bool lockMemoryVCSM(int slot, VCSM_Lock_MEM_Msg& tx, VCSM_Lock_MEM_Reply& rx);
    bool freeMemoryVCSM(int slot, VCSM_Free_MEM_Msg& tx, VCSM_Free_MEM_Reply& rx);

    // ---- MMAL ----
    bool initializeMMAL(u32 inHandle,
                        u32 inPtr,
                        u32 inSize,
                        u32 outHandleA,
                        u32 outPtrA,
                        u32 outHandleB,
                        u32 outPtrB,
                        u32 outSize,
                        u32 resX,
                        u32 resY,
                        EGLDisplay dpy,
                        EGLContext ctx);

    bool createTexturesMMAL();
    bool framePollerMMAL(u32 offset, u32 length);
    bool bufferReadyMMAL(u32 handle);

    bool queueOutputBufferMMAL(MMAL_BUFFER_FROM_HOST_T& tx, u32 handle, u32 size);
    bool queueInputBufferMMAL(MMAL_BUFFER_FROM_HOST_T& tx, u32 offset, u32 length);

    bool createComponent(MMAL_COMPONENT_CREATE_T& tx, MMAL_COMPONENT_CREATE_REPLY_T& rx);
    bool getPortInfoMMAL(u32 type, u32& handle, MMAL_PORT_INFO_GET_T& tx, MMAL_PORT_INFO_GET_REPLY_T& rx);
    bool setPortInfoMMAL(MMAL_PORT_INFO_SET_T& tx, MMAL_PORT_INFO_SET_REPLY_T& rx);
    bool enableComponentMMAL(MMAL_COMPONENT_ENABLE_T& tx, MMAL_COMPONENT_ENABLE_REPLY_T& rx);
    bool setZeroCopyModeMMAL(const MMAL_PORT_INFO_GET_REPLY_T& src, MMAL_PORT_PARAMETER_SET_T& tx, MMAL_PORT_PARAMETER_SET_REPLY_T& rx);
    bool enablePortMMAL(const MMAL_PORT_INFO_GET_REPLY_T& src, MMAL_PORT_ACTION_T& tx, MMAL_PORT_ACTION_REPLY_T& rx);

    // ---- priming ----
    void PrimeOutputBufferBodyMMAL(MMAL_BUFFER_FROM_HOST_T& tx);
    void PrimeInputBufferBodyMMAL(MMAL_BUFFER_FROM_HOST_T& tx);

    void primePortFormatInputMMAL(const MMAL_PORT_INFO_GET_REPLY_T& src, MMAL_PORT_INFO_SET_T& tx);
    void primePortFormatOutputMMAL(const MMAL_PORT_INFO_GET_REPLY_T& src, MMAL_PORT_INFO_SET_T& tx);
