#pragma once

#include "nu2api/nucore/fixed_width.h"

struct GameObject_s;

i32 ObjHitObj_Flags(GameObject_s *object);
i32 ObjHitObj(GameObject_s *attacker, GameObject_s *target, i32 damage, u16 flags, i32 probe, i32 context);
