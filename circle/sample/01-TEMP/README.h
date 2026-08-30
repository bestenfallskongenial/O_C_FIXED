#pragma once

// Inventory file for circle/sample/01 symbols.
// Requested content:
// 1) function names/signatures
// 2) global structs/variables/arrays (excluding vc04*)

// -----------------------------------------------------------------------------
// Global structs / variables / arrays (non-vc04)
// -----------------------------------------------------------------------------

// gfx_render.cpp
// static const int MENU_GPU_TILE_COUNT = 16;

// struct menu_glsl_state
// {
//     GLuint  gl_omp_id[1];
//     GLuint  gl_omt_id[1];
//     GLint   u_atlas[1];
//     GLint   u_tile_count[1];
//     GLint   u_tile_rect[1];
//     GLint   u_tile_index[1];
//     float   kMenuOrigin[2];
//     float   kMenuTileSize[2];
//     float   kMenuBackgroundScale[2];
//     float   kMenuRelPos[MENU_GPU_TILE_COUNT][2];
//     float   kMenuRelSize[MENU_GPU_TILE_COUNT][2];
//     float   tile_rect_x[MENU_GPU_TILE_COUNT];
//     float   tile_rect_y[MENU_GPU_TILE_COUNT];
//     float   tile_rect_w[MENU_GPU_TILE_COUNT];
//     float   tile_rect_h[MENU_GPU_TILE_COUNT];
//     GLfloat tile_rect[MENU_GPU_TILE_COUNT * 4];
//     GLfloat tile_index[MENU_GPU_TILE_COUNT];
// };

// -----------------------------------------------------------------------------
// Function signatures
// -----------------------------------------------------------------------------

// datamanagement.cpp
// bool CKernel::Mount(const char* p_deviceName);
// bool CKernel::UnMount();
// bool CKernel::openFile(const char* p_fileName);
// unsigned CKernel::loadToBuffer(char* p_buffer, unsigned p_bufferSize);
// bool CKernel::saveFromBufferOld(const char* p_fileName, const char* p_buffer, unsigned p_bufferSize);
// bool CKernel::saveFromBuffer(const char* p_deviceName, const char* p_fileName, const char* p_buffer, unsigned p_bufferSize);
// bool CKernel::closeFile();
// void CKernel::bulkLoad(char* p_fileNameArray[], unsigned p_byteArray[], char* p_bufferArray[], unsigned p_maxFiles, unsigned& p_loadedFiles, unsigned& p_prevFiles, unsigned p_maxFileSize);
// bool CKernel::IsValidFile(const char* pFileName, const char* pSuffixArray[], unsigned pSuffixCount);
// bool CKernel::scanRoot(char** p_fileNameArray, const char* pSuffixArray[], unsigned pSuffixCount, unsigned& p_scannedFiles, unsigned p_maxFiles);
// bool CKernel::updateUSB(const char* p_deviceName);
// void CKernel::removeUSB(CDevice* f_partitionName, boolean f_Added);
// char** CKernel::alllocateBufferMEM(size_t count, size_t bufferSize);
// char** CKernel::alllocateBufferDMA(size_t count, size_t bufferSize, char** blockBaseOut, char** rawBlockOut, size_t* blockSizeOut);
// void CKernel::clearBufferMEM(char** buffers, size_t count);
// void CKernel::clearBufferDMA(char** buffers, char* rawBlock);

// gfx_init.cpp
// void CKernel::shaderLog(GLint shader, int shaderIndex);
// void CKernel::programLog(GLint shader, int program_index);
// void CKernel::gfx_check(const char* file, unsigned line);
// void CKernel::gfx_init_OGL(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initVshaders(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initOshader(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initFshaders(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initOprogram(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initFprograms(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initOuniforms(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initFuniforms(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initOtexture(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initUtextures(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::initVbuffer(glsl_state* m_glsl, char* buffer, u32& index);
// void CKernel::render_shader_a(glsl_state* m_glsl);
// void CKernel::render_shader_b(glsl_state* m_glsl);

// gfx_render.cpp
// void CKernel::render_buffer_setup(glsl_state* m_glsl);
// void CKernel::render_uniform_setup(glsl_state* m_glsl);
// void CKernel::render_textures_setup(glsl_state* m_glsl);
// void CKernel::render_shader_draw(glsl_state* m_glsl);
// void frame_break_mechanism();
// void CKernel::render_buffer_swap(glsl_state* m_glsl);
// void CKernel::gpu_render_menu_state_update(menu_glsl_state* m_menu);
// void CKernel::gpu_render_menu_uniform_setup(menu_glsl_state* m_menu);
// void CKernel::gpu_render_menu_textures_setup(menu_glsl_state* m_menu);
// void CKernel::gpu_render_menu_shader_draw();

// logging.cpp
// void CKernel::storeLog(char* buffer, u32& index, const char* message, u32 valA = 0xFFFFFFFF, u32 valB = 0xFFFFFFFF, u32 valC = 0xFFFFFFFF);
// void CKernel::storeMsg(char* buffer, u32& index, const char* message, const char* value, u32 max_len);
// void CKernel::nextline(char* buffer, u32& index);
// static void CKernel::bufferToScreenPlot(unsigned x, unsigned y, u32 color);
// static void CKernel::bufferToScreenDrawChar(char ch, unsigned x, unsigned y, u32 color);
// boolean CKernel::bufferToScreenInit(void);
// void CKernel::bufferToScreenClear(u32 bgColor);
// void CKernel::bufferToScreenDrawBufferSegment(const char* pSourceBuffer, unsigned startOffset, unsigned byteCount, unsigned x, unsigned y, u32 color);
// unsigned CKernel::bufferToScreenGetGrid(unsigned& cols, unsigned& rows);

// menu.cpp
// void CKernel::resetMenuPickupFlags();
// void CKernel::modeMenuAssignGroup(uint8_t menu_id, uint8_t base);
// void CKernel::applyModeToChannel(int channel);
// void CKernel::modeADC(int channel);
// void CKernel::modeTRG(int channel);
// void CKernel::modeBPM(int channel);
// void CKernel::modeLF1(int channel);
// void CKernel::modeLF2(int channel);
// void CKernel::modeAudioAb0(int channel);
// void CKernel::modeAudioAb1(int channel);
// void CKernel::modeAudioBb0(int channel);
// void CKernel::modeAudioBb1(int channel);

// util.cpp
// bool CKernel::startupScreen(char* buffer, u32& index);
// void CKernel::readADC();
// bool CKernel::checkUpdate();
// bool CKernel::Update();
// void set_pot_routing(bool adc_pot_routing);
// void CKernel::prepParameters();
// void CKernel::chooseProgram(int p_channel, &p_activeShader);
// void CKernel::chooseTexture(int p_channel, &p_activeTexture, &p_validTextureCount);
// int CKernel::chooseVideo(int p_channel, &p_activeVideo, &p_validVideoCount);
// int CKernel::chooseFrame(int p_channel, &p_activeFrame, &p_validFrameCount);
// void CKernel::storeModesV1();
// void CKernel::storeModesV2();
// void CKernel::buttonPing(int p_btn_id, int pin);
// void CKernel::button_consumer(int p_btn_id);
// void CKernel::randomVec8(uint32_t p_seed);
// void CKernel::calculate2BPM(unsigned long p_triggerTimeClockA, unsigned long p_triggerTimeClockB);
// void CKernel::calculate1BPM(int chn, unsigned long p_triggerTimeClock);
// void CKernel::predictedNextBeat2();
// void CKernel::predictedNextBeat1(int chn);
// void CKernel::sampleWaveTable();

// vc04_logger.cpp
// void CKernel::Log_openService(const SERVICE_CREATION_T& src);
// void CKernel::Log_createComponent(const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply& rx);
// void CKernel::Log_getPortInfo(const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply& rx);
// void CKernel::Log_setPortInfo(const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply& rx);
// void CKernel::Log_enableComponentMMAL(const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply& rx);
// void CKernel::Log_enablePort(const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg& rx);
// void CKernel::Log_setZeroCopyModeMMAL(const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply& rx);
// void CKernel::Log_BufferFromHost(const MMAL_Buffer_From_Host_Msg& rx);
// void CKernel::Log_BufferBody(const mmal_msg_buffer_from_host_wire32& msg);
// void CKernel::Log_queueBufferFromHost(const MMAL_Buffer_From_Host_Msg& tx);

// vc04_service.cpp
// void CKernel::callbackVCSM(void* callback_param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);
// void CKernel::callbackMMAL(void* callback_param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);
// void CKernel::getStateVCHI();
// bool CKernel::initEventsVCOS(VCOS_EVENT_T& event, const char* name);
// u32 CKernel::convertAddress(void* buffer, size_t size);
// bool CKernel::checkGLerrorMMAL();
// u32 CKernel::NextTransId(u32& tid);
// void CKernel::initHeaderVCSM(vc_sm_msg_hdr_t& hdr, u32 type);
// void CKernel::initHeaderMMAL(mmal_msg_header& hdr, u32 type);
// bool CKernel::sendAndWaitVCHI(VCHI_SERVICE_HANDLE_T ServiceHandle, const void* txBuffer, u32 txSize, void* rxBuffer, u32 rxSize);
// bool CKernel::openServiceVCHI(SERVICE_CREATION_T& tx, VCHI_SERVICE_HANDLE_T& ServiceHandle);
// bool CKernel::initializeVCSM();
// bool CKernel::importMemoryVCSM(void* buffer, size_t size, int slot, VCSM_Import_MEM_Msg& tx, VCSM_Import_MEM_Reply& rx);
// bool CKernel::lockMemoryVCSM(int slot, VCSM_Lock_MEM_Msg& tx, VCSM_Lock_MEM_Reply& rx);
// bool CKernel::freeMemoryVCSM(int slot, VCSM_Free_MEM_Msg& tx, VCSM_Free_MEM_Reply& rx);
// bool CKernel::initializeMMAL(u32 InBufferHandle, u32 OutBufferHandle);
// bool CKernel::createTexturesMMAL();
// bool CKernel::framePollerMMAL(u32 frame_offset, u32 frame_length);
// bool CKernel::bufferReadyMMAL(u32 handle);
// bool CKernel::queueOutputBufferMMAL(MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size);
// bool CKernel::queueInputBufferMMAL(MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length);
// bool CKernel::createComponent(MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply& rx);
// bool CKernel::getPortInfoMMAL(u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply& rx);
// bool CKernel::setPortInfoMMAL(MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply& rx);
// bool CKernel::enableComponentMMAL(MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply& rx);
// bool CKernel::setZeroCopyModeMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply& rx);
// bool CKernel::enablePortMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx);
// void CKernel::PrimeOutputBufferBodyMMAL(MMAL_Buffer_From_Host_Msg& tx);
// void CKernel::PrimeInputBufferBodyMMAL(MMAL_Buffer_From_Host_Msg& tx);
// void CKernel::primePortFormatInputMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Info_Set_Msg& tx);
// void CKernel::primePortFormatOutputMMAL(const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Info_Set_Msg& tx);

// wrappers.cpp
// bool CKernel::wrapperInitDMA();
// bool CKernel::wrapperInitMEM();
// void CKernel::wrapperDMAcleanUp();
// void CKernel::wrapperMEMcleanUp();
// void CKernel::wrapper_from_sd();
// void CKernel::wrapper_load_usb();
// void CKernel::wrapper_init_gl_sd();
// void CKernel::wrapper_init_gl_usb();
// void CKernel::wrapper_io();
// void CKernel::wrapper_modes();