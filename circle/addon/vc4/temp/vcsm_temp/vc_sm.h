//----------------------------------------------------------------------------------------------------------------------------------------------------
//      vc_sm_class.h
//----------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _VC_SM_CLASS_H
#define _VC_SM_CLASS_H

#include "vc_sm_defs.h"

#include <circle/types.h>                                                           // for u32, size_t, etc.

#include "interface/vcos/vcos.h"
#include <vc4/interface/vcinclude/common.h>
#include <vc4/vchi/vchi.h>                                                          // for VCHI_INSTANCE_T, VCHI_CONNECTION_T, VCHI_SERVICE_HANDLE_T, VCHI_CALLBACK_REASON_T
#include <vc4/vchiq/vchiq.h>

// #include <circle/logger.h>

// #include "interface/vcos/vcos_event.h"


#include <circle/bcm2835.h>

#define VCSMLOG

#define DEBUG_MAX_LENGTH (1024 * 16)

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);
class CVCSharedMemory
{
public:
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CONSTRUCTOR / DECONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------------------------------------
                CVCSharedMemory     (   );
               ~CVCSharedMemory     (   );
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              USER API
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool importMemory   ( void* buffer, size_t size, int slot, vc_sm_import_msg& tx, vc_sm_import_result& rx);
bool lockMemory     ( int slot, vc_sm_lock_msg& tx, vc_sm_lock_result_t& rx);
bool freeMemory     ( int slot, vc_sm_free_msg& tx, vc_sm_result_t& rx);


}
#endif