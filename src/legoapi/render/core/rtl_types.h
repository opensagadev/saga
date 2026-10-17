#pragma once

#include "decomp.h"

// Types shared by legoapi/render/core/rtl.c and legoapi/render/light/lighting.cpp.

struct rtlindex_s {
    i32 enabled;
    u8 reserved_04[8];
    i32 width;
    i32 depth;
    f32 scale;
    f32 offset_x;
    f32 offset_z;
    char **cells;
    u8 reserved_24[4];
};
// Debug line vertex layout used by the RTL editor line helper.
struct nuvtx_tc1_s {
    NUVEC position; // 0x00
    NUVEC normal;   // 0x0c
    i32 colour;     // 0x18
    f32 uv[2];      // 0x1c
};

DECOMP_ASSERT(sizeof(rtlindex_s) == 0x28, "RTL index size");
