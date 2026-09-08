#pragma once

#include "legoapi/render/fx/edsplines.h"
typedef SPLINEPOS_s SPLINEPOSITION_RUNTIME_s;

DECOMP_ASSERT(sizeof(SPLINEPOSITION_RUNTIME_s) == 0x20, "SPLINEPOS runtime size");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, finished) == 0x7, "SPLINEPOS finished flag offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, position) == 0x10, "SPLINEPOS position offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, normalized_position) == 0x1c, "SPLINEPOS normalized position offset");

void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, f32 distance, i32 looping);
void MoveSplinePosition(SPLINEPOS_s *position, f32 distance);
void PointAlongSpline(nugspline_s *spline, f32 position, nuvec_s *result, u16 *yaw, u16 *pitch, i32 looping);
void GetNearestSplinePos(nuvec_s *point, SPLINEPOS_s *position, nugspline_s *spline, i32 looping, i16 first_point,
                         i16 last_point);
