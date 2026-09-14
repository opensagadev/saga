#pragma once

#include "nu2api/nucore/fixed_width.h"

struct GameObject_s;

extern i32 objopponent_ignoreaiopponent;
i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);
i32 ObjIsTargetSpeeder(GameObject_s *object);
