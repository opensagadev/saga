#pragma once

#include "decomp.h"

// Types shared by legoapi/render/core/gameliball.cpp and nu2api/nu3d/nu3d_includes.cpp.

namespace {
    struct TERRAIN_FILE_CHUNK {
        i16 chunk_count; // only meaningful in the first descriptor
        u16 version;
        u32 data_word_count;
        NUVEC origin;
        i16 chunk_type;
        u16 field_0x16;
        u8 pad_0x18[0x14];
        u32 field_0x2c;
        u16 field_0x30;
        u16 field_0x32;
    };
}

struct SoundGroup {
    i16 first_sample;
    i16 sample_count;
    i16 field_0x4;
    i16 field_0x6;
};

DECOMP_ASSERT(sizeof(SoundGroup) == 8, "SoundGroup size");
DECOMP_ASSERT(sizeof(TERRAIN_FILE_CHUNK) == 0x34, "TERRAIN_FILE_CHUNK ABI");
