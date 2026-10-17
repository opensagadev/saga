#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
class VuVec;
bool CalculateRayBoxIntersection(VuVec const &, VuVec const &, VuVec const &, VuVec const &, f32, f32 &);
f32 CalcCapsuleIntersectDistance(VuVec const &, VuVec const &, f32, VuVec const &, f32);
