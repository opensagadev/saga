#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
void PushCode(GameObject_s *object, i32 allow_grab);
void ReleasePush(GameObject_s *);
