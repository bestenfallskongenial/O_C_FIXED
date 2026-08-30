#include "kernel.h"
#include <circle/string.h> 
#include <stdint.h>
/*
< extern        < type                 < name                                   < size
        <const
*/   

extern          bool                    noTargetFPS;
extern          bool                    adc_pot_routing;
extern          bool                    resetFlag;
extern          bool                    g_show_bank;

extern          bool                    timer_mode;
extern          bool                    single_tex_mode;

extern          float                   current_fps;
extern          unsigned long           last_swap_duration;
extern          int                     last_gl_current_prg;  // Store last active shader program
extern          int                     current_buffer;
extern          int                     gl_current_prg; // display_debug value
extern          int                     gl_current_tex;

extern          int                     g_current_texture;
extern          int                     active_bpm_channel;
extern          int                     attenuation;
extern          unsigned                hFile;  // extern          bool                    showMenuFlag;

extern          unsigned long           start_time_fps_calculation;
extern          unsigned long           end_time_fps_calculation;
extern          unsigned long           total_time_fps_calculation;

extern          unsigned long           currentTime;
extern          unsigned long           targetTime;
extern          unsigned long           delayTime;

extern          float                   GLtime;
extern          float                   opaque;

extern          int                     multiplier                              [LFO_MULTIPLIERS];





//extern          float                   float_lfo_output                        [LFO_INSTANCES];         // realtime output for 2 x 4 lfo waves ( sine, saw, tri, ... )

extern          bool                    m_shaderStatusFlags                     [FSH_SD+FSH_USB]; 
extern          bool                    menu_pickup_flag                        [ADC_CHANNELS+ADC_CHANNELS];                 // Array tracking if pot value has been "picked up"
extern          bool                    input_threshold_flag                    [ADC_CHANNELS];             // Array tracking threshold state per channel


extern          int                     menu_pickup_buffer                      [ADC_CHANNELS+ADC_CHANNELS];               // Array storing previous pot values for pickup detection
extern          unsigned long           current_time_for_bpm;                      // Array storing timing information per channel


extern          unsigned long           last_valid_bpm_calculation              [2];
extern          unsigned long           last_valid_bpm_buffer                   [2];

extern          unsigned char           led_col_new                             [20][3];





extern          int LFO_mode[4];

extern          int sensitivity_new;
extern          int sensitivity_old;

extern          unsigned long     loaded_bytes_kernel[2];

extern          int mode_storage_buffers[16][33];
extern          int shader_has_stored_params[33];
extern          int audio_sample[2];

/*
new menu_input_mode

[8 ( for adc inputs ) + 4 ( LFO ) + 4 ( extra mode flags )][progs on sd + progs on usb]
 menu_input_mode        LFO_mode    mode_storage_buffers[FRM_MODE][current_buffer],mode_storage_buffers[TEX_MODE][current_buffer],  adc_attenuation    

eventual also the variable buffers ***_old???
*/

