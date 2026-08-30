//----------------------------------------------------------------------------------------------------------------------------------------------------
//              vc_h264_parser.cpp 
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool    CKernel::ParseInitialize (  char*      blockBase,
                                        int         max_textures,
                                        u32         max_tex_size,
                                        int         max_videos,  
                                        int         max_frames,
                                        u16         max_width,
                                        u16         max_height,
                                        u8          max_profile,
                                        u8          max_level)
{
    m_videoBlockBase = blockBase;
    m_max_textures = max_textures;
    m_max_tex_size = max_tex_size;

    m_max_videos  = max_videos;
    m_max_frames  = max_frames;
    m_max_width   = max_width;
    m_max_height  = max_height;
    m_max_profile = max_profile;
    m_max_level   = max_level;

    if ( max_videos != MAX_VIDEOS ) return false;

    return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::ParseAnnexB                (   int         file_index, 
                                                        char*       buffer_array[], 
                                                        size_t      size_array[])
{
    m_CharIndex[file_index] = 0;
    memset(m_DebugCharArray[file_index], 0,sizeof m_DebugCharArray[file_index]);                                    // clear the array???

    u8* data  = (u8*)buffer_array[file_index];
    size_t size = size_array[file_index];
    
    if (size > 4 && data[0] != 0 && data[1] == 0 && data[2] == 0)                                                   // Skip potential non-standard leading byte
        {
        data++;
        size--;
        }
    storeLog(file_index,"Parser / Frame Log for Video  ", file_index);

    m_video_width[file_index]    = 0;                                                                              // Reset metadata & log buffer index
    m_video_height[file_index]   = 0;
    m_vid_profile[file_index]    = 0;
    m_vid_level[file_index]      = 0;
    m_vid_is_valid[file_index]   = false;
    m_frame_count[file_index]    = 0;
    m_extradata_valid[file_index] = false;
    m_extradata_len[file_index]   = 0;

    size_t sps_off = 0, pps_off = 0;                                                                                // SPS/PPS offsets for extradata
    size_t sps_len = 0, pps_len = 0;
    size_t last_sps_pos = 0;  
    bool found_sps = false;
    bool found_pps = false;
    
    size_t pos = 0;                                                                                                 // --- First pass: find SPS/PPS ---
    while (pos < size - 3) 
        {
        pos = findNext000001(data, pos, size);
        if (pos >= size - 3) break;

        size_t sc_len = (data[pos + 2] == 1) ? 3 : 4;
        u8 nal_type = data[pos + sc_len] & 0x1F;

        size_t next_pos = findNext000001(data, pos + sc_len, size);
        size_t nal_size = (next_pos == size)
                        ? (size - pos - sc_len)
                        : (next_pos - pos - sc_len);
        if (nal_type == NAL_TYPE_SPS && !found_sps) 
            {
            sps_off = pos;  
            u8 clean_sps[1024];                                                                                     // Clean SPS
            size_t clean_idx = 0;
            for (size_t i = 1; i < nal_size && clean_idx < sizeof(clean_sps); i++) {
                if (i >= 3 &&
                    data[pos + sc_len + i - 2] == 0 &&
                    data[pos + sc_len + i - 1] == 0 &&
                    data[pos + sc_len + i] == 3)
                    continue;
                clean_sps[clean_idx++] = data[pos + sc_len + i];
            }   
            if (ParseSPS(   clean_sps, clean_idx,                                                                   // Parse SPS -> width/height/profile/level
                            &m_video_width[file_index],
                            &m_video_height[file_index],
                            &m_vid_profile[file_index],
                            &m_vid_level[file_index])) {
                found_sps = true;
                // log parsed SPS info
                storeLog(file_index,"\nSPS width/height   ", m_video_width[file_index], m_video_height[file_index]);
                storeLog(file_index,"SPS profile/level  ", m_vid_profile[file_index], m_vid_level[file_index]);
            }
        }
        else if (nal_type == NAL_TYPE_PPS && !found_pps) 
            {
            pps_off = pos;
            found_pps = true;
            }
        if (found_sps && found_pps) break;
        pos = next_pos;
        }
    
    if (found_sps && found_pps)                                                                                     // --- Build extradata ---
        {
        size_t sc_sps = (data[sps_off+2] == 1) ? 3 : 4;
        size_t sc_pps = (data[pps_off+2] == 1) ? 3 : 4;
        sps_len = pps_off - (sps_off + sc_sps);

        size_t next_after_pps = findNext000001(data, pps_off + sc_pps, size);
        if (next_after_pps > size) next_after_pps = size;
        pps_len = next_after_pps - (pps_off + sc_pps);

        static const u8 sc[4] = {0,0,0,1};
        size_t out_pos = 0;

        memcpy(m_extradata[file_index] + out_pos, sc, 4); out_pos += 4;
        memcpy(m_extradata[file_index] + out_pos, data + sps_off + sc_sps, sps_len); out_pos += sps_len;
        memcpy(m_extradata[file_index] + out_pos, sc, 4); out_pos += 4;
        memcpy(m_extradata[file_index] + out_pos, data + pps_off + sc_pps, pps_len); out_pos += pps_len;

        m_extradata_len[file_index]   = out_pos;
        m_extradata_valid[file_index] = true;
        m_vid_is_valid[file_index]    = true;

        ParserStoreMsg(file_index,m_extradata[file_index], m_extradata_len[file_index], "EXTRADATA SPS+PPS\n");     // log full extradata hex dump
    }
    int frame_idx = 0;                                                                                              // --- Second pass: find IDR frames ---
    pos = 0;
    while (pos < size - 3 && frame_idx < MAX_FRAMES) 
        {
        pos = findNext000001(data, pos, size);
        if (pos >= size - 3)
            {
            break;
            }
        size_t sc_len = (data[pos + 2] == 1) ? 3 : 4;
        u8 nal_type = data[pos + sc_len] & 0x1F;
    //  size_t buffer_size = (size_t) buffer_block_start;
        if (nal_type == NAL_TYPE_SPS)                                                                               // record SPS position
            {
            last_sps_pos = pos;
            }
        if (nal_type == NAL_TYPE_IDR) 
            {
            m_frame_address [file_index][frame_idx] = (void*)(data + last_sps_pos);
            m_frameOffset[file_index][frame_idx] = (size_t)((data + last_sps_pos) - (u8*)m_videoBlockBase);         // store SPS addr
            size_t next_pos = findNext000001(data, pos + sc_len, size);
            if (next_pos < size)
                {
                m_framelenght[file_index][frame_idx] = next_pos - last_sps_pos;                                     // SPS→IDR-end length
                }
            else
                {
                m_framelenght[file_index][frame_idx] = size - last_sps_pos;
                }
            m_idr_offset[file_index] = pos - last_sps_pos;                                                          // SPS→IDR offset
            storeLog(file_index,"SPS+PPS+IDR addr/len/off ", (u32)m_frame_address[file_index][frame_idx], (u32)m_framelenght[file_index][frame_idx], (u32)m_frameOffset[file_index][frame_idx]);
            frame_idx++;
            pos = next_pos;
            }
        else 
            {
            pos = findNext000001(data, pos + sc_len, size);
            }
        }

    m_frame_count[file_index] = frame_idx;

    storeLog(file_index,"\nParsed Frames           ",frame_idx);
    storeLog(file_index,"\nParsed IDR-Offset       ",m_idr_offset[file_index]);
    
    
    if (m_video_width[file_index]  != m_max_width || m_video_height[file_index] != m_max_height)                    // Check resolution
        {
        m_vid_is_valid[file_index] = false;
        }
    
    if (m_vid_profile[file_index] != m_max_profile)                                                                 // Check profile
        {
        m_vid_is_valid[file_index] = false;
        }
    
    if (m_vid_level[file_index] != m_max_level)                                                                     // Check level
        {
        m_vid_is_valid[file_index] = false;
        }
     if (m_vid_is_valid[file_index])
        {   
        storeLog(file_index,"\nMetaData Valid for Video",file_index);
        }
    else    
        {   
        storeLog(file_index,"\nMetaData Invalid for Video",file_index);
        }
        return m_vid_is_valid[file_index];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
bool CKernel::ParseBPM          (int file_index, char* filename_array[], char* buffer_array[], size_t size_array[])
{
    m_CharIndex[file_index] = 0;
    memset(m_DebugCharArray[file_index], 
                0, 
                sizeof m_DebugCharArray[file_index]);

    u8*    data = reinterpret_cast<u8*>(buffer_array[file_index]);
    size_t size = size_array[file_index];
    
    storeLog(file_index, "======== BMP header parse start ========");                                               // — initialize log entry for this texture —
    storeLog(file_index, filename_array[file_index], file_index);
    storeLog(file_index, "========================================");
    
    u32 fileSize    = data[2]  | (data[3]<<8)  | (data[4]<<16)  | (data[5]<<24);                                    // read fields
    u32 dataOffset  = data[10] | (data[11]<<8) | (data[12]<<16) | (data[13]<<24);
    u32 headerSize  = data[14] | (data[15]<<8) | (data[16]<<16) | (data[17]<<24);
    u16 planes      = data[26] | (data[27]<<8);
    u16 bpp         = data[28] | (data[29]<<8);
    u32 compression = data[30] | (data[31]<<8) | (data[32]<<16) | (data[33]<<24);
    u32 width       = data[18] | (data[19]<<8) | (data[20]<<16) | (data[21]<<24);
    u32 height      = data[22] | (data[23]<<8) | (data[24]<<16) | (data[25]<<24);
    u32 imgSize     = data[34] | (data[35]<<8) | (data[36]<<16) | (data[37]<<24);

    if (size != fileSize)
        {
        storeLog(file_index, "Filesize parsed/stored", fileSize, size_array[file_index]);
        storeLog(file_index, "Filesize missmatch    ", file_index);
        // return m_tex_valid[file_index] = false;
        }

    if ((width  & 3) != 0 || (height & 3) != 0)                                                                     // not multiple of 4
        {
        
        m_tex_valid[file_index] = false;                                                                            // fail alignment test
        storeLog(file_index, "BMP dim not 4-aligned", width, height);
        return false;
        }
    
    storeLog(file_index, "BMP fileSize/dataOffset  ", fileSize, dataOffset);                                        // log raw header fields
    storeLog(file_index, "BMP headerSize/planes    ", headerSize, planes);
    storeLog(file_index, "BMP bpp/compression      ", bpp, compression);
    storeLog(file_index, "BMP width/height         ", width, height);
    storeLog(file_index, "BMP imgSize              ", imgSize);
    
    ParserStoreMsg(file_index, data, headerSize, "BMP Header Hex Dump");                                            // (optional) dump the first 38 bytes of the header

    // validate
    bool ok = data[0]=='B' && data[1]=='M' && fileSize <= m_max_tex_size && headerSize == 40 && planes == 1 && bpp == 24 && compression== 0 && width*height*3 == imgSize;

    m_tex_valid[file_index]       = ok;                                                                             // i can put the line above here or not?
    m_tex_file_size[file_index]   = fileSize;
    m_tex_data_offset[file_index] = dataOffset;
    m_tex_width[file_index]       = static_cast<u16>(width);
    m_tex_height[file_index]      = static_cast<u16>(height);
    m_tex_image_size[file_index]  = imgSize;

    storeLog(file_index, ok ? "BMP header VALID" : "BMP header FAILED");

    return ok;
}
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::ParseBPM                   (   tex_state*  t,
                                                        char*       filename_array[],
                                                        char*       buffer_array[],
                                                        size_t      size_array[],
                                                        int         fromFile,
                                                        int         toFile)
{
                for (int i = fromFile; i < toFile; i++)
                    {
                    m_CharIndex[i] = 0;
                    memset(m_DebugCharArray[i], 0, sizeof m_DebugCharArray[i]);

                    u8*    data = reinterpret_cast<u8*>(buffer_array[i]);
                    size_t size = size_array[i];

                    storeLog(i, "======== BMP header parse start ========");
                    storeLog(i, filename_array[i], i);
                    storeLog(i, "========================================");

                    u32 fileSize    = data[2]  | (data[3]<<8)  | (data[4]<<16)  | (data[5]<<24);
                    u32 dataOffset  = data[10] | (data[11]<<8) | (data[12]<<16) | (data[13]<<24);
                    u32 headerSize  = data[14] | (data[15]<<8) | (data[16]<<16) | (data[17]<<24);
                    u16 planes      = data[26] | (data[27]<<8);
                    u16 bpp         = data[28] | (data[29]<<8);
                    u32 compression = data[30] | (data[31]<<8) | (data[32]<<16) | (data[33]<<24);
                    u32 width       = data[18] | (data[19]<<8) | (data[20]<<16) | (data[21]<<24);
                    u32 height      = data[22] | (data[23]<<8) | (data[24]<<16) | (data[25]<<24);
                    u32 imgSize     = data[34] | (data[35]<<8) | (data[36]<<16) | (data[37]<<24);

                    bool ok =
                        data[0]             == 'B' &&
                        data[1]             == 'M' &&
                        fileSize            <= m_max_tex_size &&    // is passed from the parser init 
                        headerSize          == 40 &&
                        planes              == 1 &&
                        bpp                 == 24 &&
                        compression         == 0 &&
                        width * height * 3  == imgSize &&
                        ((width & 3)        == 0) &&
                        ((height & 3)       == 0);

                    t->tex_valid[i] = ok;
                    t->width[i]     = width;
                    t->height[i]    = height;
                    t->offset[i]    = dataOffset;
                    t->file_size[i] = fileSize;
                    t->image_size[i]= imgSize;
                    t->data[i]      = data;

                    storeLog(i, ok ? "BMP header VALID" : "BMP header FAILED");

                    m_Watchdog.Start(TIMEOUT);
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              HELPERS / UTILITY / WRAPPER
//----------------------------------------------------------------------------------------------------------------------------------------------------
size_t          CKernel::findNext000001(u8* data, size_t pos, size_t size) const
{
                while (pos < size - 3) {
                    if ((data[pos] == 0 && data[pos+1] == 0 && data[pos+2] == 1) ||
                        (data[pos] == 0 && data[pos+1] == 0 && data[pos+2] == 0 && data[pos+3] == 1)) 
                        {
                        return pos;
                        }
                    pos++;
                }
                return size;                                                                                                    // No more start codes found
}
u32             CKernel::ReadExpGolomb(u8* data, size_t* bit_offset) const
{
                size_t leadingZeroBits = 0;
                size_t offset = *bit_offset;
                size_t byte_offset = offset / 8;
                size_t bit_pos = offset % 8;
                
                while (1)                                                                                                       // Count leading zeros
                    {
                    if (bit_pos == 8) 
                        {
                        bit_pos = 0;
                        byte_offset++;
                        }
                    
                    if ((data[byte_offset] & (0x80 >> bit_pos)) != 0) 
                        {
                        break;
                        }
                    leadingZeroBits++;
                    bit_pos++;
                    offset++;
                    }
                offset++;                                                                                                       // Skip the stop bit
                bit_pos = offset % 8;
                byte_offset = offset / 8;
                
                u32 result = 0;                                                                                                 // Read the coefficient bits
                for (size_t i = 0; i < leadingZeroBits; i++) 
                    {
                    result <<= 1;
                    if (bit_pos == 8) 
                        {
                        bit_pos = 0;
                        byte_offset++;
                        }
                    if ((data[byte_offset] & (0x80 >> bit_pos)) != 0) 
                        {
                        result |= 1;
                        }
                    bit_pos++;
                    offset++;
                    }
                result = (1 << leadingZeroBits) - 1 + result;
                *bit_offset = offset;
                return result;
}
bool            CKernel::ParseSPS(u8* sps_data, size_t sps_size, u16* width, u16* height, u8* profile, u8* level) const
{
                if (sps_size < 3)                       // Ensure we have enough data
                    {
                    return false;
                    }
                *profile = sps_data[0];                 // First byte is profile_idc
                
                *level = sps_data[2];                   // NEW: store level_idc // END NEW
                
                size_t bit_offset = 24;                 // Skip constraint_set flags and level_idc (3 bytes total) // Skip 3 bytes (profile + constraint flags + level)
                
                ReadExpGolomb(sps_data, &bit_offset);   // seq_parameter_set_id 
                
                if (*profile >= 100)                    // FIX 5: Process all profiles without restriction // Handle high profile specific parameters if needed
                    {
                    u32 chroma_format_idc = ReadExpGolomb(sps_data, &bit_offset);   // chroma_format_idc

                    if (chroma_format_idc == 3) 
                        {
                        bit_offset++; // separate_colour_plane_flag // Skip 1 bit
                        }
                    ReadExpGolomb(sps_data, &bit_offset);   // bit_depth_luma_minus8
                    ReadExpGolomb(sps_data, &bit_offset);   // bit_depth_chroma_minus8
                    
                    bit_offset++;   // qpprime_y_zero_transform_bypass_flag // Skip 1 bit
                    
                    u8 seq_scaling_matrix_present_flag = (sps_data[bit_offset/8] >> (7 - (bit_offset % 8))) & 0x01; // seq_scaling_matrix_present_flag
                    bit_offset++;

                    if (seq_scaling_matrix_present_flag) 
                        {
                        bit_offset += 8;    // Simple approximation for scaling matrix
                        }
                    }
                ReadExpGolomb(sps_data, &bit_offset);   // log2_max_frame_num_minus4
                
                u32 pic_order_cnt_type = ReadExpGolomb(sps_data, &bit_offset);  // pic_order_cnt_type

                if (pic_order_cnt_type == 0) 
                    {
                    ReadExpGolomb(sps_data, &bit_offset);   // log2_max_pic_order_cnt_lsb_minus4
                    } 
                else if (pic_order_cnt_type == 1) 
                    {
                    bit_offset++;                           // delta_pic_order_always_zero_flag
                    ReadExpGolomb(sps_data, &bit_offset);   // offset_for_non_ref_pic
                    ReadExpGolomb(sps_data, &bit_offset);   // offset_for_top_to_bottom_field
                    u32 num_ref_frames_in_pic_order_cnt_cycle = ReadExpGolomb(sps_data, &bit_offset);   // num_ref_frames_in_pic_order_cnt_cycle

                    for (u32 i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++) 
                        {
                        ReadExpGolomb(sps_data, &bit_offset);   // offset_for_ref_frame[i]
                        }
                    }
                ReadExpGolomb(sps_data, &bit_offset);   // max_num_ref_frames
                
                bit_offset++;       // gaps_in_frame_num_value_allowed_flag
                
                u32 pic_width_in_mbs_minus1 = ReadExpGolomb(sps_data, &bit_offset); // pic_width_in_mbs_minus1
                
                u32 pic_height_in_map_units_minus1 = ReadExpGolomb(sps_data, &bit_offset);  // pic_height_in_map_units_minus1
                
                u8 frame_mbs_only_flag = (sps_data[bit_offset/8] >> (7 - (bit_offset % 8))) & 0x01; // frame_mbs_only_flag
                bit_offset++;
                
                *width = (pic_width_in_mbs_minus1 + 1) * 16;    // Calculate the dimensions

                if (frame_mbs_only_flag) 
                    {
                    *height = (pic_height_in_map_units_minus1 + 1) * 16;
                    } 
                else 
                    {
                    *height = (pic_height_in_map_units_minus1 + 1) * 32;

                    
                    bit_offset++;   // mb_adaptive_frame_field_flag
                    }
                bit_offset++;   
                
                u8 frame_cropping_flag = (sps_data[bit_offset/8] >> (7 - (bit_offset % 8))) & 0x01; // frame_cropping_flag
                bit_offset++;

                if (frame_cropping_flag) 
                    {
                    u32 frame_crop_left_offset = ReadExpGolomb(sps_data, &bit_offset);  // Apply cropping to the dimensions
                    u32 frame_crop_right_offset = ReadExpGolomb(sps_data, &bit_offset);
                    u32 frame_crop_top_offset = ReadExpGolomb(sps_data, &bit_offset);
                    u32 frame_crop_bottom_offset = ReadExpGolomb(sps_data, &bit_offset);

                    *width -= (frame_crop_left_offset + frame_crop_right_offset) * 2;   // Adjust width and height based on cropping

                    if (frame_mbs_only_flag) 
                        {
                        *height -= (frame_crop_top_offset + frame_crop_bottom_offset) * 2;
                        } 
                    else 
                        {
                        *height -= (frame_crop_top_offset + frame_crop_bottom_offset) * 4;
                        }
                    }
                return true;
}

