#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
struct WORLDINFO_s;
extern f32 DeathStar2BattleFire_GetSlowDownMul(GameObject_s *);
i32 SarlaccPitDiscoActive(WORLDINFO_s *world);
void DeathStar2BattleFire_SetSlowDownMul(GameObject_s *object);
void DeathStar2BattleFire_UpdateSlowDownMul(f32 dt);
void SarlaccPitB_SpecialUpdate(WORLDINFO_s *);
