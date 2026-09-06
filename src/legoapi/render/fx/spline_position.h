#pragma once

#include "decomp.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuvec.h"

struct SPLINEPOS_s;

// Runtime layout of the original 0x20-byte SPLINEPOS block. SPLINEPOS_s is
// still opaque in the broad type header, so spline code uses this exact view.
struct SPLINEPOSITION_RUNTIME_s {
    NUGSPLINE *spline;
    i16 segment;
    u8 looping;
    u8 finished;
    f32 distance;
    f32 segment_length;
    NUVEC position;
    f32 normalized_position;
};

DECOMP_ASSERT(sizeof(SPLINEPOSITION_RUNTIME_s) == 0x20, "SPLINEPOS runtime size");

void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, f32 distance, i32 looping);
void MoveSplinePosition(SPLINEPOS_s *position, f32 distance);
void PointAlongSpline(nugspline_s *spline, f32 position, nuvec_s *result, u16 *yaw, u16 *pitch, i32 looping);
void GetNearestSplinePos(nuvec_s *point, SPLINEPOS_s *position, nugspline_s *spline, i32 looping, i16 first_point,
                         i16 last_point);
