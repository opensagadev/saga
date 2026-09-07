#pragma once

#include "nu2api/numath/nuvec4.h"
#include "decomp.h"

struct NuWindGType {
    u16 in_use;
    u16 flags;
    u32 unknown_0x04;
    u32 unknown_0x08;
    NUMTX *matrices;
    NUVEC center;
    u8 drawn;
    u8 visible;
    i16 matrix_count;
    f32 unknown_0x20;
    f32 unknown_0x24;
    f32 radius_squared;
    f32 radius;
    f32 near_distance;
    f32 far_distance;
    f32 extended_radius_squared;
    f32 distance_range;
    f32 unknown_0x40;
    f32 unknown_0x44;
    f32 unknown_0x48;
};

DECOMP_ASSERT(sizeof(NuWindGType) == 0x4c, "wind group size");
DECOMP_ASSERT(__builtin_offsetof(NuWindGType, matrices) == 0x0c, "wind matrices offset");
DECOMP_ASSERT(__builtin_offsetof(NuWindGType, matrix_count) == 0x1e, "wind matrix count offset");

typedef struct nuwind_s {
    i32 unk0[8];
    i32 unk1;
    NUVEC4 unk2;
    f32 unk3;
} NUWIND;

#ifdef __cplusplus
extern "C" {
#endif
    void NuWindInitialise(NUWIND *wind);
#ifdef __cplusplus
}
#endif
