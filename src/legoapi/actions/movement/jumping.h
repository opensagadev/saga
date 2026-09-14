#pragma once

#include "nu2api/nucore/fixed_width.h"

struct GameObject_s;

void StartJump(GameObject_s *object, i32 movement_state);
void StartEndOfJump(GameObject_s *object);
i32 StartFallLand(GameObject_s *object, i32 action);
