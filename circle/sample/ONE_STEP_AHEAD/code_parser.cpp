#include "kernel.h"

    #define MY_BUFFER   m_bufferLog[0]      //m_logBuffer
    #define MY_INDEX    m_bufferLogIndex[0] //m_logBufferIndex

bool            CKernel::BMPparser                  (   tex_state*  t,
                                                        char*       p_buffer_array[],
                                                        char*       filename_array[],
                                                        size_t      size_array[],
                                                        u32         max_tex_size,
                                                        int         p_fromFile,
                                                        int         p_toFile )
{
                t->max_tex_size = max_tex_size;

                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    u8* data = (u8*)p_buffer_array[i];

                    u32 fileSize    = data[2]  | (data[3]<<8)  | (data[4]<<16)  | (data[5]<<24);
                    u32 dataOffset  = data[10] | (data[11]<<8) | (data[12]<<16) | (data[13]<<24);
                    u32 headerSize  = data[14] | (data[15]<<8) | (data[16]<<16) | (data[17]<<24);
                    u16 planes      = data[26] | (data[27]<<8);
                    u16 bpp         = data[28] | (data[29]<<8);
                    u32 compression = data[30] | (data[31]<<8) | (data[32]<<16) | (data[33]<<24);
                    u32 width       = data[18] | (data[19]<<8) | (data[20]<<16) | (data[21]<<24);
                    u32 height      = data[22] | (data[23]<<8) | (data[24]<<16) | (data[25]<<24);
                    u32 imgSize     = data[34] | (data[35]<<8) | (data[36]<<16) | (data[37]<<24);

                    t->tex_valid[i] =   data[0]             == 'B' && data[1]                   == 'M'      &&
                                        fileSize            <= t->max_tex_size && headerSize    == 40       &&
                                        planes              == 1 &&bpp                          == 24       &&
                                        compression         == 0 &&width * height * 3           == imgSize  &&
                                        ((width & 3)        == 0) &&((height & 3)               == 0);

                    t->width[i]     = width;
                    t->height[i]    = height;
                    t->offset[i]    = dataOffset;
                    t->file_size[i] = fileSize;
                    t->image_size[i]= imgSize;
                    t->data[i]      = data;
                    t->size[i]      = size_array[i];

#ifdef __LOG_PARSER__
                    storeLog(   MY_BUFFER, MY_INDEX,    "File         [", (u32)i, 
                                                        "] buffer      ", (u32)p_buffer_array[i] );
                    storeMsg(   MY_BUFFER, MY_INDEX,    "Header Dump   ", data, headerSize );
                    storeLog(   MY_BUFFER, MY_INDEX, t->tex_valid[i] ? 
                                                        "header   VALID" : 
                                                        "header  FAILED", EMPTYLOG, 
                                                        " Name         ", EMPTYLOG, 
                                                        filename_array[i], EMPTYLOG );
                    storeLog(   MY_BUFFER, MY_INDEX,    "exp. Filesize ", (u32)fileSize, 
                                                        "act. Filesize ", (u32)size_array[i], 
                                                        "max. FileSize ", (u32)t->max_tex_size );
                    storeLog(   MY_BUFFER, MY_INDEX,    "Header offset ", (u32)dataOffset, 
                                                        "Header size   ", (u32)headerSize);
                    storeLog(   MY_BUFFER, MY_INDEX,    "Bitmap Width  ", (u32)width, 
                                                        "Bitmap Height ", (u32)height, 
                                                        "BitsPerPixel  ", (u32)bpp );
                    storeLog(   MY_BUFFER, MY_INDEX,    "Bitmap-Planes ", (u32)planes, 
                                                        "Compression   ", (u32)compression );
                    storeLog(   MY_BUFFER, MY_INDEX,    "exp. Bmpsize  ", (u32)(width * height * 3), 
                                                        "act. Bmpsize  ", (u32)imgSize );
                    nextline(   MY_BUFFER, MY_INDEX);                       
#endif
                    }
                return true;
}

bool            CKernel::parse264                  (   h264_state* h,
                                                        char*       blockBase,
                                                        char*       p_buffer_array[],
                                                        char*       filename_array[],
                                                        size_t      size_array[],
                                                        int         p_fromFile,
                                                        int         p_toFile,
                                                        unsigned&   valid_count, // * changed !! *

                                                        u16         min_width,
                                                        u16         max_width,

                                                        u16         min_height,
                                                        u16         max_height,

                                                        u8          min_profile,
                                                        u8          max_profile,

                                                        u8          min_level,
                                                        u8          max_level )
{
                h->block_base  = blockBase;

                h->min_width   = min_width;
                h->max_width   = max_width;

                h->min_height  = min_height;  
                h->max_height  = max_height;

                h->min_profile = min_profile;
                h->max_profile = max_profile;

                h->min_level   = min_level; 
                h->max_level   = max_level;

                size_t sps_off[MAX_VIDEOS][MAX_FRAMES]       = {0};
                size_t sps_sc_len[MAX_VIDEOS][MAX_FRAMES]    = {0};
                size_t sps_len[MAX_VIDEOS][MAX_FRAMES]       = {0};

                size_t pps_off[MAX_VIDEOS][MAX_FRAMES]       = {0};
                size_t pps_sc_len[MAX_VIDEOS][MAX_FRAMES]    = {0};
                size_t pps_len[MAX_VIDEOS][MAX_FRAMES]       = {0};

                size_t idr_off[MAX_VIDEOS][MAX_FRAMES]       = {0};
                size_t idr_sc_len[MAX_VIDEOS][MAX_FRAMES]    = {0};
                size_t idr_len[MAX_VIDEOS][MAX_FRAMES]       = {0};

                for (int file_index = p_fromFile; file_index < p_toFile; file_index++)
                    {
                    unsigned dst = valid_count; // * changed !! *
                    u8*    data = reinterpret_cast<u8*>(p_buffer_array[file_index]);
                    size_t size = size_array[file_index];

                /*  u8*    data = reinterpret_cast<u8*>(p_buffer_array[i]); */
                /*  size_t size = size_array[i]; */
                    size_t i = 0;
#ifdef __LOG_PARSER__
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Videofile    [", file_index, 
                                "] FileSize    ", (u32)size_array[file_index],
                                "Bufferaddress ", (u32)p_buffer_array[file_index],
                                filename_array[file_index], EMPTYLOG );
#endif
                    for (size_t pos = 0; pos < size - 3; )
                        {
                        size_t sc_len = (data[pos + 2] == 1) ? 3 : 4;
                        u8 nal_type = data[pos + sc_len] & 0x1F;

                        size_t next_pos = findNext000001(data, pos + sc_len, size);

                        if (nal_type == NAL_TYPE_SPS)
                            {
                            sps_off[file_index][i] = pos;
                            sps_sc_len[file_index][i] = sc_len;
                            sps_len[file_index][i]    = next_pos - pos;
                            }
                        if (nal_type == NAL_TYPE_PPS)
                            {
                            pps_off[file_index][i] = pos;
                            pps_sc_len[file_index][i] = sc_len;
                            pps_len[file_index][i]    = next_pos - pos;
                            }
                        if (nal_type == NAL_TYPE_IDR)
                            {
                            idr_off[file_index][i] = pos;
                            idr_sc_len[file_index][i] = sc_len;
                            idr_len[file_index][i]    = next_pos - pos;

                            i++;
                            }   
                        pos = next_pos;
                        }

                        u32 vcsm_base = (BUS_ADDRESS(reinterpret_cast<uintptr_t>(h->block_base)) & ~0xC0000000) | 0xC0000000;                       /* ADDED: VPU bus base derived from h->block_base */

                    for (size_t idx = 0; idx < i; idx++)
                        {
                        /* REMOVED: size_t end_off = (idx + 1 < i) ? sps_off[file_index][idx + 1] : size; - REPLACED BY: nal_block_length now uses SPS start to IDR end */

                        h->nal_block_offset[dst][idx]  = (size_t)((data + sps_off[file_index][idx]) - (u8*)h->block_base); // * changed !! *
                        h->nal_block_address[dst][idx] = (void*)(vcsm_base + h->nal_block_offset[dst][idx]);                      /* CHANGED: address stored for runtime submit is VPU bus address */ // * changed !! *
                        h->nal_block_length[dst][idx]  = idr_off[file_index][idx] + idr_len[file_index][idx] - sps_off[file_index][idx]; /* CHANGED: old length used next SPS/file end; new length ends at current IDR end */ // * changed !! *

                        h->frame_offset[dst][idx]      = (size_t)((data + idr_off[file_index][idx]) - (u8*)h->block_base);               /* ADDED: per-frame offset from block_base to startcode+IDR */ // * changed !! *
                        h->frame_address[dst][idx]     = (void*)(vcsm_base + h->frame_offset[dst][idx]);                          /* ADDED: VPU bus address for startcode+IDR */ // * changed !! *
                        h->frame_length[dst][idx]      = idr_len[file_index][idx];                                                       /* ADDED: length of startcode+IDR */ // * changed !! *

                        /* CHANGED: h->idr_offset[file_index] = idr_off[file_index][idx] - sps_off[file_index][idx]; - REPLACED BY: h->frame_offset[file_index][idx], because the old value was per-file and overwritten each idx */
#ifdef __DUMP_FRAMES__
                        storeLog(   MY_BUFFER, MY_INDEX,
                                    "67+68+65", EMPTYLOG,
                                    "address ", (u32)h->nal_block_address[dst][idx], // * changed !! *
                                    "length  ", (u32)h->nal_block_length[dst][idx], // * changed !! *
                                    "offset  ", (u32)h->nal_block_offset[dst][idx]); // * changed !! *
                        storeLog(   MY_BUFFER, MY_INDEX,                                                                          /* ADDED: log startcode+IDR runtime packet */
                                    "65      ", EMPTYLOG,
                                    "address ", (u32)h->frame_address[dst][idx], // * changed !! *
                                    "length  ", (u32)h->frame_length[dst][idx], // * changed !! *
                                    "offset  ", (u32)h->frame_offset[dst][idx]); // * changed !! *
#endif
                        }

                    u8 tmp[256];
                    size_t out_pos = 0;

                    static const u8 sc4[4] = {0,0,0,1};

                    size_t idx = 1;

                    memcpy(tmp + out_pos, sc4, 4); 
                    out_pos += 4;
                    memcpy(tmp + out_pos, data + sps_off[file_index][idx] + sps_sc_len[file_index][idx], 
                                                 sps_len[file_index][idx] - sps_sc_len[file_index][idx]);
                    out_pos += sps_len[file_index][idx] - sps_sc_len[file_index][idx];

                    memcpy(tmp + out_pos, sc4, 4); 
                    out_pos += 4;
                    memcpy(tmp + out_pos, data + pps_off[file_index][idx] + pps_sc_len[file_index][idx],
                                                 pps_len[file_index][idx] - pps_sc_len[file_index][idx]);

                    out_pos += pps_len[file_index][idx] - pps_sc_len[file_index][idx];

                    ParseSPS(   data + sps_off[file_index][1],
                                sps_len[file_index][1],
                                sps_sc_len[file_index][1],
                                &h->video_width[dst], // * changed !! *
                                &h->video_height[dst], // * changed !! *
                                &h->vid_profile[dst], // * changed !! *
                                &h->vid_level[dst]); // * changed !! *

                                h->data[dst]        = data; // * changed !! *
                                h->size[dst]        = size; // * changed !! *

                                h->frame_count[dst] = i; // * changed !! *
                                h->idr_sc_len[dst]  = idr_sc_len[file_index][1]; /* because 0 might be "out of line" like the whole first set of nal units */ // * changed !! *
                                
                    if (out_pos <= sizeof(h->extradata[dst])) // * changed !! *
                        {
                        memcpy(h->extradata[dst], tmp, out_pos); // * changed !! *
                        h->extradata_len[dst] = out_pos; // * changed !! *
                        }
                    h->vid_valid[dst] = // * changed !! *

                        h->video_width[dst]  >= h->min_width &&      /* why not the global macro settings here, and how we can make the resolution matching the config.txt settings here too? */ // * changed !! *
                        h->video_width[dst]  <= h->max_width && // * changed !! *

                        h->video_height[dst] >= h->min_height && // * changed !! *
                        h->video_height[dst] <= h->max_height && // * changed !! *

                        h->vid_profile[dst]  >= h->min_profile && // * changed !! *
                        h->vid_profile[dst]  <= h->max_profile &&                         // * changed !! *

                        h->vid_level[dst]    >= h->min_level && // * changed !! *
                        h->vid_level[dst]    <= h->max_level; // * changed !! *
                        
                    if (h->vid_valid[dst]) valid_count++; // * changed !! *
#ifdef __LOG_PARSER__
                    nextline(   MY_BUFFER, MY_INDEX ); 
                    storeLog(   MY_BUFFER, MY_INDEX,
                                "min.  Width   ",          (u32)h->min_width,
                                "min.  Height  ",          (u32)h->min_height,
                                "min.  Profile ",          (u32)h->min_profile, 
                                "min.  Level   ",          (u32)h->min_level );                    
                    storeLog(   MY_BUFFER, MY_INDEX,
                                "max.  Width   ",          (u32)h->max_width,
                                "max.  Height  ",          (u32)h->max_height,
                                "max.  Profile ",          (u32)h->max_profile,
                                "max.  Level   ",          (u32)h->max_level );
                    nextline(   MY_BUFFER, MY_INDEX );    
                    storeLog(   MY_BUFFER, MY_INDEX,
                                "Video Width   ",          (u32)h->video_width[dst], // * changed !! *
                                "Video Height  ",          (u32)h->video_height[dst], // * changed !! *
                                "Video Profile ",          (u32)h->vid_profile[dst], // * changed !! *
                                "Video Level   ",          (u32)h->vid_level[dst] ); // * changed !! *
                    nextline(   MY_BUFFER, MY_INDEX );
                    storeLog(   MY_BUFFER, MY_INDEX,
                                "Parsed Frames ", h->frame_count[dst]); // * changed !! *
                    nextline(   MY_BUFFER, MY_INDEX );
                    storeLog(   MY_BUFFER, MY_INDEX,
                                "idr_sc_len  ",             EMPTYLOG,
                                (h->idr_sc_len[dst] == 3) ?  // * changed !! *
                                "    00 00 01" : 
                                " 00 00 00 01",             EMPTYLOG,
                                "Extradata-Len.",          (u32)h->extradata_len[dst], // * changed !! *
                                (h->vid_valid[dst]) ?  // * changed !! *
                                "Header VALID  " : 
                                "Header INVALID");
                    nextline(   MY_BUFFER, MY_INDEX );            
                    storeMsg(   MY_BUFFER, MY_INDEX, 
                                "Extradata Dump",          h->extradata[dst], h->extradata_len[dst] ); // * changed !! *
                    nextline(   MY_BUFFER, MY_INDEX );
#endif // __LOG_PARSER__               
                    }
                return true;
}

bool            CKernel::ParseSPS                   (   u8*     sps_data,
                                                        size_t  sps_size,
                                                        size_t  sps_sc_len,
                                                        u16*    width,
                                                        u16*    height,
                                                        u8*     profile,
                                                        u8*     level) const
{
                u8* rbsp = sps_data + sps_sc_len + 1;                               /* skip startcode + NAL header (67) */

                *profile = rbsp[0];
                *level   = rbsp[2];

                size_t bit_offset = 24;

                ReadExpGolomb(rbsp, &bit_offset);

                if (*profile >= 100)
                    {
                    u32 chroma_format_idc = ReadExpGolomb(rbsp, &bit_offset);

                    if (chroma_format_idc == 3) bit_offset++;

                    ReadExpGolomb(rbsp, &bit_offset);
                    ReadExpGolomb(rbsp, &bit_offset);

                    bit_offset++;

                    u8 seq_scaling_matrix_present_flag = (rbsp[bit_offset / 8] >> (7 - (bit_offset % 8))) & 0x01;
                    bit_offset++;

                    if (seq_scaling_matrix_present_flag) bit_offset += 8;
                    }
                ReadExpGolomb(rbsp, &bit_offset);

                u32 pic_order_cnt_type = ReadExpGolomb(rbsp, &bit_offset);

                if (pic_order_cnt_type == 0) ReadExpGolomb(rbsp, &bit_offset);

                else if (pic_order_cnt_type == 1)
                    {
                    bit_offset++;
                    ReadExpGolomb(rbsp, &bit_offset);
                    ReadExpGolomb(rbsp, &bit_offset);

                    u32 num_ref_frames_in_pic_order_cnt_cycle = ReadExpGolomb(rbsp, &bit_offset);

                    for (u32 i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++) ReadExpGolomb(rbsp, &bit_offset);

                    }
                ReadExpGolomb(rbsp, &bit_offset);

                bit_offset++;

                u32 pic_width_in_mbs_minus1  = ReadExpGolomb(rbsp, &bit_offset);
                u32 pic_height_in_map_units_minus1 = ReadExpGolomb(rbsp, &bit_offset);

                u8 frame_mbs_only_flag = (rbsp[bit_offset / 8] >> (7 - (bit_offset % 8))) & 0x01;
                bit_offset++;

                *width = (pic_width_in_mbs_minus1 + 1) * 16;

                if (frame_mbs_only_flag) *height = (pic_height_in_map_units_minus1 + 1) * 16;

                else
                    {
                    *height = (pic_height_in_map_units_minus1 + 1) * 32;
                    bit_offset++;
                    }
                bit_offset++;

                u8 frame_cropping_flag = (rbsp[bit_offset / 8] >> (7 - (bit_offset % 8))) & 0x01;
                bit_offset++;

                if (frame_cropping_flag)
                    {
                    u32 frame_crop_left_offset   = ReadExpGolomb(rbsp, &bit_offset);
                    u32 frame_crop_right_offset  = ReadExpGolomb(rbsp, &bit_offset);
                    u32 frame_crop_top_offset    = ReadExpGolomb(rbsp, &bit_offset);
                    u32 frame_crop_bottom_offset = ReadExpGolomb(rbsp, &bit_offset);

                    *width -= (frame_crop_left_offset + frame_crop_right_offset) * 2;

                    if (frame_mbs_only_flag) *height -= (frame_crop_top_offset + frame_crop_bottom_offset) * 2;

                    else *height -= (frame_crop_top_offset + frame_crop_bottom_offset) * 4;
                    }
                return true;
}

size_t          CKernel::findNext000001             (   u8* data, size_t pos, size_t size) const
{
                while (pos < size - 3) {
                    if ((data[pos] == 0 && data[pos+1] == 0 && data[pos+2] == 1) || (data[pos] == 0 && data[pos+1] == 0 && data[pos+2] == 0 && data[pos+3] == 1)) return pos;

                    pos++;
                }
                return size;
}

u32             CKernel::ReadExpGolomb              (   u8* data, size_t* bit_offset) const
{
                size_t leadingZeroBits = 0;
                size_t offset = *bit_offset;
                size_t byte_offset = offset / 8;
                size_t bit_pos = offset % 8;
                
                while (1)
                    {
                    if (bit_pos == 8) 
                        {
                        bit_pos = 0;
                        byte_offset++;
                        }
                    
                    if ((data[byte_offset] & (0x80 >> bit_pos)) != 0) break;

                    leadingZeroBits++;
                    bit_pos++;
                    offset++;
                    }
                offset++;
                bit_pos = offset % 8;
                byte_offset = offset / 8;
                
                u32 result = 0;
                for (size_t i = 0; i < leadingZeroBits; i++) 
                    {
                    result <<= 1;
                    if (bit_pos == 8) 
                        {
                        bit_pos = 0;
                        byte_offset++;
                        }
                    if ((data[byte_offset] & (0x80 >> bit_pos)) != 0) result |= 1;

                    bit_pos++;
                    offset++;
                    }
                result = (1 << leadingZeroBits) - 1 + result;
                *bit_offset = offset;
                return result;
}