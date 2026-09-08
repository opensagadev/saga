#pragma once

#include "decomp.h"
#include "nu2api/numath/nuvec.h"

struct NULGTLASER {
    NUVEC start;
    NUVEC end;
    f32 width;
    f32 segment_length;
    f32 width_wobble;
    f32 end_width_ratio;
    f32 length;
    u8 type;
    u8 arc;
    u8 padding[2];
    u32 colour;
    i32 seed;
};
DECOMP_ASSERT(sizeof(NULGTLASER) == 0x38, "Lightning record size");
DECOMP_ASSERT(offsetof(NULGTLASER, type) == 0x2c, "Lightning type offset");
DECOMP_ASSERT(offsetof(NULGTLASER, seed) == 0x34, "Lightning seed offset");

i32 NuLgtRand();
extern "C" {
    extern u32 NuLgtSeed;
    extern i32 NuLgtLaserCnt;
    extern i32 NuLgtArcLaserCnt;
    extern i32 NuLgtArcLaserFrame;
    extern NULGTLASER NuLgtLaserData[64];
    void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
}
