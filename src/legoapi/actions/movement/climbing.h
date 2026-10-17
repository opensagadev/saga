#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
struct WORLDINFO_s;
struct nuvec_s;
extern i32 (*Fighting_WeaponInActionFn)(GameObject_s *);
extern i32 (*Fighting_WeaponOutActionFn)(GameObject_s *);
i32 CanStepBack(GameObject_s *object);
i32 NewBlockAction(GameObject_s *);
i32 StepBackFromTarget(GameObject_s *object);
void AutoWeaponOnOff(GameObject_s *object);
void BlockSfx(GameObject_s *object);
void Climb_MoveCode(GameObject_s *object);
void Climb_SetMagnetAction(GameObject_s *);
void Climb_SetMagnetDrawOffsetTarget(GameObject_s *, nuvec_s *);
void FastWeaponIn(GameObject_s *object, i32 sound);
void FastWeaponOut(GameObject_s *object, i32 sound);
void Hang_MoveCode(GameObject_s *object);
void InitClimbObjectSys(WORLDINFO_s *world);
void KeepWeaponIn(GameObject_s *);
void KeepWeaponOut(GameObject_s *object);
void LedgeTerrain_MoveCode(GameObject_s *object);
void SetWeaponIn(GameObject_s *);
void SetWeaponOut(GameObject_s *);
void SlowWeaponIn(GameObject_s *object);
void SlowWeaponOut(GameObject_s *);
void WeaponInCode(GameObject_s *object);
void WeaponOutCode(GameObject_s *object);
void WeaponScalingCode(GameObject_s *object);
