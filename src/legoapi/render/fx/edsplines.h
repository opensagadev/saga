#pragma once

#include "decomp.h"
#include "nu2api/nu3d/nuspline.h"

struct SPLINEPOS_s {
    NUGSPLINE *spline;    // 0x00
    i16 segment;          // 0x04
    i8 looping;           // 0x06
    u8 reached_end;       // 0x07
    f32 segment_distance; // 0x08
    f32 segment_length;   // 0x0c
    NUVEC position;       // 0x10
    f32 along;            // 0x1c
};
DECOMP_ASSERT(sizeof(SPLINEPOS_s) == 0x20, "SPLINEPOS size");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, segment) == 4, "SPLINEPOS segment offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, looping) == 6, "SPLINEPOS looping offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, segment_distance) == 8, "SPLINEPOS segment distance offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, segment_length) == 0xc, "SPLINEPOS segment length offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, position) == 0x10, "SPLINEPOS position offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, along) == 0x1c, "SPLINEPOS along offset");

void PointAlongSpline(NUGSPLINE *, f32, NUVEC *, u16 *, u16 *, i32);
void InitSplinePosition(SPLINEPOS_s *, NUGSPLINE *, f32, i32);
void MoveSplinePosition(SPLINEPOS_s *, f32);
void GetNearestSplinePos(NUVEC *, SPLINEPOS_s *, NUGSPLINE *, i32, i16, i16);
