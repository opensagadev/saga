#pragma once

#include "decomp.h"
#include "nu2api/numath/nuvec.h"

struct GameObject_s;
struct WORLDINFO_s;

struct SNAKESEGMENT_s {
    NUVEC position;
    f32 ground_height;
    i32 yaw;
    i32 pitch;
};

struct SNAKEBODY_s {
    SNAKESEGMENT_s segments[11];
    f32 scale;
    u16 segment_count;
    u8 flags;
    u8 reserved;
};

DECOMP_ASSERT(sizeof(SNAKESEGMENT_s) == 0x18, "Snake segment size");
DECOMP_ASSERT(offsetof(SNAKESEGMENT_s, position) == 0, "Snake segment position offset");
DECOMP_ASSERT(offsetof(SNAKESEGMENT_s, ground_height) == 0xc, "Snake segment ground height offset");
DECOMP_ASSERT(offsetof(SNAKESEGMENT_s, yaw) == 0x10, "Snake segment yaw offset");
DECOMP_ASSERT(offsetof(SNAKESEGMENT_s, pitch) == 0x14, "Snake segment pitch offset");
DECOMP_ASSERT(sizeof(SNAKEBODY_s) == 0x110, "Snake body size");
DECOMP_ASSERT(offsetof(SNAKEBODY_s, scale) == 0x108, "Snake body scale offset");
DECOMP_ASSERT(offsetof(SNAKEBODY_s, segment_count) == 0x10c, "Snake segment count offset");
DECOMP_ASSERT(offsetof(SNAKEBODY_s, flags) == 0x10e, "Snake body flags offset");

void InitSnakes(WORLDINFO_s *world);
SNAKEBODY_s *CreateSnakeBody(GameObject_s *object, i32 segment_count);
void DestroySnakeBody(GameObject_s *object);
