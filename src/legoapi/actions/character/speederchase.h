#pragma once

#include "nu2api/nucore/fixed_width.h"

struct GameObject_s;
struct ADDPART_s;

extern i32 objopponent_ignoreaiopponent;
i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);
i32 ObjIsTargetSpeeder(GameObject_s *object);
void InitBikeParts();
void KillParts_SpeederBike(ADDPART_s *params, i32 animation, i32 variant, GameObject_s *object);
