#pragma once

#include "decomp.h"

// Types shared by legoapi/misc/supportall.cpp and legoapi/render/core/gameliball.cpp.

struct BIGBUFFER_s {
    void *start;
    void *end;
    u32 field_0x8[2];
    BIGBUFFER_s() : start(NULL), end(NULL) {
    }
};
struct TIMINGBAR_LABEL {
    char name[11];
    u8 type;
};
struct TexQuadVertex {
    f32 x, y, z;
    u32 colour;
    union {
        f32 uv[2];
        u16 half_uv[4];
    };
};

DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");
