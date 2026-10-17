#pragma once

#include "nu2api/nucore/common.h"

struct GameObject_s;
struct numtl_s;

i32 SuperWeirdo(GameObject_s *object);
i32 RndrUnfilledCircle(f32 x, f32 y, f32 radius, f32 border_width, f32 aspect, i32 colour, f32 progress, f32 z,
                       numtl_s *material);
void RndrArrow(f32 x, f32 y, f32 scale, i32 angle, i32 colour);

// Declared here for the other units that use them.
struct GameObject_s;
void CloakMovement(GameObject_s *object);
