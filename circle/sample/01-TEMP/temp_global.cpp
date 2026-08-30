#include "kernel.h"
#include "global.h"

#include <circle/string.h> 
#include <stdint.h>
/*
                < type                 < name                                   < size                      < content/value
                                                                                                                    <apostroph
*/   


                unsigned                hFile                                                               =     0;  // ?? empty ??
                bool                    noTargetFPS                                                         =  true;
                bool                    adc_pot_routing                                                     = false;
                bool                    resetFlag                                                           = false;
                bool                    g_show_bank                                                         = false;

                float                   current_fps                                                         =     0.0;
                unsigned long           last_swap_duration                                                  =     0;
                int                     last_gl_current_prg = -1;  // Store last active shader program

                int                     current_buffer = 0;

                int                     gl_current_prg                                                      =     0; // display_debug value
                int                     gl_current_tex                                                      =     0;
                int                     linked_programs                                                     =     0;
                int                     audio_source_channel                                                =    -1;                // Tracks which channel is audio source (-1 if none)

                int                     active_bpm_channel                                                  =     0;
                int                     g_menu_mode_new                                                     =     0;                        // Current menu mode (0, 1, or 2)
                int                     g_menu_mode_old                                                     =     0;
                int                     g_current_texture                                                   =     0;
                int                     attenuation                                                         =     2;
                unsigned long           start_time_fps_calculation                                          =     0;
                unsigned long           end_time_fps_calculation                                            =     0;
                unsigned long           total_time_fps_calculation                                          =     0;
// for the fps breack mechanism                
                unsigned long           currentTime = 0;
                unsigned long           targetTime = 0;
                unsigned long           delayTime = 0;
                


                float                   GLtime                                                              =  0.0f;
                float                   opaque                                                              =  1.0f;

                int                     multiplier                              [7]                         = {   64, 32, 16, 8, 4, 2, 1 };

                unsigned long           last_circle_buffer                      [LFO_INSTANCES]             = {   0    };
                unsigned long           next_circle_buffer                      [LFO_INSTANCES]             = {   0    };
                float                   LFO_float_output                        [LFO_INSTANCES]             = {   0    };
                int                     LFO_int_output                          [LFO_INSTANCES]             = {   0    };
                int                     last_multiplier                         [LFO_INSTANCES]             = {   0    };


                int                   menu_pickup_buffer                        [ADC_CHANNELS+ADC_CHANNELS]              = { 0 };               // Array storing previous pot values for pickup detection
                unsigned long         current_time_for_bpm                                                  = 0;                      // Array storing timing information per channel
                bool                  menu_pickup_flag                          [ADC_CHANNELS+ADC_CHANNELS]              = { 0 };                 // Array tracking if pot value has been "picked up"
                bool                  input_threshold_flag                      [ADC_CHANNELS]              = { 0 };             // Array tracking threshold state per channel
                unsigned long         next_beat_time                            [2]                         = { 0 };                     // Next beat timestamp for BPM mode
                unsigned long         buffer_interval                           [2]                         = { 0 };
                unsigned long         last_valid_bpm_calculation                [2]                         = { 0 };
                unsigned long         last_valid_bpm_buffer                     [2]                         = { 0 };
                unsigned long         resultBPM                                 [2]                         = { 0 };
                unsigned long         intervalBuffer                            [2][3]                      = { 0 };
                int                   threshold_high                            [ADC_CHANNELS]              = {   320,  320,  320,  320,  320,  320,  320,  320 };                    // Array of upper threshold values per channel - i guess this should become a menu layer where i can set them up
                int                   threshold_low                             [ADC_CHANNELS]              = {   128,  128,  128,  128,  128,  128,  128,  128 };                     // Array of lower threshold values per channel
               unsigned char          led_col_new                               [20][3]                     = { 0 };

                                                                                                     
         
        unsigned long elapsedMicroseconds[LFO_INSTANCES];
        unsigned long cycleLength[LFO_INSTANCES];
        int sampleIndex[LFO_INSTANCES];

        int LFO_mode[LFO_PARAMETERS] = { 0,3,3,3 };

        int sensitivity_new = 0;
        int sensitivity_old = 0;


         int mode_storage_buffers[16][33] = {0};               // array to store the modes and specials for all loaded fshaders?
         int shader_has_stored_params[33] = { 0 };
         int audio_sample[2] = { 0,0};



         int m_validTextureCount=0;
/*
new menu_input_mode

[8 ( for adc inputs ) + 4 ( LFO ) + 4 ( extra mode flags )][progs on sd + progs on usb]
 menu_input_mode        LFO_mode    mode_storage_buffers[FRM_MODE][current_buffer],mode_storage_buffers[TEX_MODE][current_buffer],  adc_attenuation    

eventual also the variable buffers ***_old???
*/