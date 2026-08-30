// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
bool CKernel::initBMPparser( tex_state* t,
                             char* p_buffer_array[],
                             size_t size_array[],
                             u32 max_tex_size,
                             int p_fromFile,
                             int p_toFile )
{
    t->max_tex_size = max_tex_size;

    for (int i = p_fromFile; i < p_toFile; i++)
        {
        t->data[i] = (u8*)p_buffer_array[i];
        t->size[i] = size_array[i];          // REQUIRED
        }
    return true;
}

bool CKernel::initH264parser( h264_state* h,
                              char* blockBase,
                              char* p_buffer_array[],
                              size_t size_array[],
                              int p_fromFile,
                              int p_toFile,
                              u16 max_width,
                              u16 max_height,
                              u8  max_profile,
                              u8  max_level )
{
    h->block_base  = blockBase;
    h->max_width   = max_width;
    h->max_height  = max_height;
    h->max_profile = max_profile;
    h->max_level   = max_level;

    for (int file_index = p_fromFile; file_index < p_toFile; file_index++)
        {
        h->data[file_index] = (u8*)p_buffer_array[file_index];
        h->size[file_index] = size_array[file_index];
        }
    return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::ParseBPM                   (   tex_state*  t,
                                                        char*       filename_array[],
                                                     // char*       p_buffer_array[],
                                                     // size_t      size_array[] ,
                                                        int         p_fromFile,
                                                        int         p_toFile)
{
                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    u8*    data = t->data[i];
                    size_t size = t->size[i];   // now meaningful

                    storeLog(i, "======== Vid header parse start ========");
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
                        fileSize            <= t->max_tex_size &&       // is passed from the parser init but also a macro... 
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
// ----------------------------------------------------------------------------------------------------
void            CKernel::ParseAnnexB(  h264_state* h,
                            char*       filename_array[],
                         // char*       p_buffer_array[],
                         // size_t      size_array[],
                            int         p_fromFile,
                            int         p_toFile)
{
                for (int file_index = p_fromFile; file_index < p_toFile; file_index++)
                    {
                    u8* data = h->data[file_index];
                    size_t size = h->size[file_index];
                //  u8*    data = reinterpret_cast<u8*>(p_buffer_array[i]);
                //  size_t size = size_array[i];
                    size_t i = 0;

                    size_t sps_off[MAX_FRAMES] = {0};
                    size_t sps_sc_len[MAX_FRAMES] = {0};
                    size_t sps_len[MAX_FRAMES] = {0};

                    size_t pps_off[MAX_FRAMES] = {0};
                    size_t pps_sc_len[MAX_FRAMES] = {0};
                    size_t pps_len[MAX_FRAMES] = {0};

                    size_t idr_off[MAX_FRAMES] = {0};
                    size_t idr_sc_len[MAX_FRAMES] = {0};
                    size_t idr_len[MAX_FRAMES] = {0};

                    for (size_t pos = 0; pos < size - 3 )               // PASS 1: detect + store SPS/PPS/IDR, length and startcode length (single index)
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

                            i++;                                        // complete access unit IDR = frames so i increments
                            }   
                        pos = next_pos;
                        }
                    for (size_t idx = 0; idx < i; idx++)                // PASS 2: frame extraction (table only)
                        {
                        size_t end_off =
                            (idx + 1 < i)
                            ? sps_off[file_index][idx + 1]
                            : size;

                        h->frame_address[file_index][idx] = (void*)(data + sps_off[file_index][idx]);
                        h->frame_offset[file_index][idx]  = (size_t)((data + sps_off[file_index][idx]) - (u8*)h->block_base);
                        h->frame_length[file_index][idx]  = end_off - sps_off[file_index][idx];
                        h->idr_offset[file_index]         = idr_off[file_index][idx] - sps_off[file_index][idx];

                        storeLog(file_index,"SPS+PPS+IDR addr/len/off ",
                                (u32)h->frame_address[file_index][idx],
                                (u32)h->frame_length[file_index][idx],
                                (u32)h->frame_offset[file_index][idx]);
                        }
                    u8 tmp[1024];                                       // PASS 3: extradata extraction
                    size_t out_pos = 0;

                    static const u8 sc4[4] = {0,0,0,1};
                    // SPS
                    memcpy(tmp + out_pos, sc4, 4); out_pos += 4;
                    memcpy(tmp + out_pos, data + sps_off[file_index][idx] + sps_sc_len[file_index][idx], 
                                                 sps_len[file_index][idx] - sps_sc_len[file_index][idx]);
                    out_pos += sps_len[file_index][idx] - sps_sc_len[file_index][idx];
                    // PPS
                    memcpy(tmp + out_pos, sc4, 4); out_pos += 4;
                    memcpy(tmp + out_pos, data + pps_off[file_index][idx] + pps_sc_len[file_index][idx],
                                                 pps_len[file_index][idx] - pps_sc_len[file_index][idx]);

                    out_pos += pps_len[file_index][idx] - pps_sc_len[file_index][idx];

                    ParseSPS(   data + sps_off[file_index][1],          // PASS 4: metadata extraction & struct population
                                sps_len[file_index][1],
                                sps_sc_len[file_index][1],
                                &h->video_width[file_index],
                                &h->video_height[file_index],
                                &h->vid_profile[file_index],
                                &h->vid_level[file_index]);

                                h->frame_count[file_index] = i; 
                                h->idr_sc_len[file_index] = idr_sc_len[1]; // because 0 might be "out of line" llike the whole firdt set of nal units
                                
                    if (out_pos <= sizeof(h->extradata[file_index]))
                        {
                        memcpy(h->extradata[file_index], tmp, out_pos);
                        h->extradata_len[file_index] = out_pos;

                        }
                    h->vid_valid[file_index] =                          // PASS 5: validation + report

                        h->video_width[file_index]  == h->max_width &&      // why not the global macro settings here, and how we can make the resolution matching the config.txt settings here too?
                        h->video_height[file_index] == h->max_height &&
                        h->vid_profile[file_index]  == h->max_profile &&
                        h->vid_level[file_index]    == h->max_level;

                    storeLog(file_index,"\nParsed Frames           ", h->frame_count[file_index]);
                    storeLog(file_index,"\nParsed IDR-Offset       ", h->idr_offset[file_index]);

                    if (h->vid_valid[file_index])
                        {
                        storeLog(file_index,"\nMetaData Valid for Video", file_index);
                        }
                    else
                        {
                        storeLog(file_index,"\nMetaData Invalid for Video", file_index);
                        }
                    m_Watchdog.Start(TIMEOUT);
                    }
}
// ----------------------------------------------------------------------------------------------------
bool CKernel::ParseSPS(  u8*     sps_data,
                         size_t  sps_size,
                         size_t  sps_sc_len,
                         u16*    width,
                         u16*    height,
                         u8*     profile,
                         u8*     level) const
{
    u8* rbsp = sps_data + sps_sc_len + 1;                               // skip startcode + NAL header (67)

    *profile = rbsp[0];
    *level   = rbsp[2];

    size_t bit_offset = 24;

    ReadExpGolomb(rbsp, &bit_offset);

    if (*profile >= 100)
        {
        u32 chroma_format_idc = ReadExpGolomb(rbsp, &bit_offset);

        if (chroma_format_idc == 3)
            {
            bit_offset++;
            }
        ReadExpGolomb(rbsp, &bit_offset);
        ReadExpGolomb(rbsp, &bit_offset);

        bit_offset++;

        u8 seq_scaling_matrix_present_flag = (rbsp[bit_offset / 8] >> (7 - (bit_offset % 8))) & 0x01;
        bit_offset++;

        if (seq_scaling_matrix_present_flag)
            {
            bit_offset += 8;
            }
        }
    ReadExpGolomb(rbsp, &bit_offset);

    u32 pic_order_cnt_type = ReadExpGolomb(rbsp, &bit_offset);

    if (pic_order_cnt_type == 0)
        {
        ReadExpGolomb(rbsp, &bit_offset);
        }
    else if (pic_order_cnt_type == 1)
        {
        bit_offset++;
        ReadExpGolomb(rbsp, &bit_offset);
        ReadExpGolomb(rbsp, &bit_offset);

        u32 num_ref_frames_in_pic_order_cnt_cycle = ReadExpGolomb(rbsp, &bit_offset);

        for (u32 i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)
            {
            ReadExpGolomb(rbsp, &bit_offset);
            }
        }
    ReadExpGolomb(rbsp, &bit_offset);

    bit_offset++;

    u32 pic_width_in_mbs_minus1  = ReadExpGolomb(rbsp, &bit_offset);
    u32 pic_height_in_map_units_minus1 = ReadExpGolomb(rbsp, &bit_offset);

    u8 frame_mbs_only_flag = (rbsp[bit_offset / 8] >> (7 - (bit_offset % 8))) & 0x01;
    bit_offset++;

    *width = (pic_width_in_mbs_minus1 + 1) * 16;

    if (frame_mbs_only_flag)
        {
        *height = (pic_height_in_map_units_minus1 + 1) * 16;
        }
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
// ----------------------------------------------------------------------------------------------------
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
                return size;                                            // No more start codes found
}
// ----------------------------------------------------------------------------------------------------
u32             CKernel::ReadExpGolomb(u8* data, size_t* bit_offset) const
{
                size_t leadingZeroBits = 0;
                size_t offset = *bit_offset;
                size_t byte_offset = offset / 8;
                size_t bit_pos = offset % 8;
                
                while (1)                                               // Count leading zeros
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
                offset++;                                               // Skip the stop bit
                bit_pos = offset % 8;
                byte_offset = offset / 8;
                
                u32 result = 0;                                         // Read the coefficient bits
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
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
