#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct DETONATOR_s;
struct GameObject_s;
void Detonator_Detonate(DETONATOR_s *);
void Detonator_MoveCode(GameObject_s *object);
