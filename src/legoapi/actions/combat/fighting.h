#pragma once

#include "nu2api/nucore/common.h"

struct GameObject_s;

i32 ObjZappedBlue(GameObject_s *object);

// Declared here for the other units that use them.
struct GameObject_s;
extern i32 (*CanStartHoldFn)(GameObject_s *);
void ComboHitFrame(GameObject_s *object, i32 damage);
void HoldCode(GameObject_s *object);
void HoldCode_Copy(GameObject_s *object);
void Punch_Hit(GameObject_s *, GameObject_s *, f32, f32);
void StartHold(GameObject_s *object);
