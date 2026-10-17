#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
f32 DropInOutScale(GameObject_s *);
void DropInOutCode(GameObject_s *object);
void DropOut(i32 player, i32 show_effect, i32 silent, i32 force);
