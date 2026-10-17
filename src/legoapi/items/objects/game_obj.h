#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct ADDPART_s;
struct BOLT_s;
struct GameObject_s;
struct nuvec_s;
i32 StunGameObject(GameObject_s *, GameObject_s *, f32, i32);
void ClearLastSafeTakeOver(GameObject_s *object);
void InstantKillParts(GameObject_s *object, i32 mode, f32 delay);
void KillParts_ATAT(ADDPART_s *, i32, i32, GameObject_s *);
void KillParts_TIEFIGHTER(ADDPART_s *, i32, i32, GameObject_s *, i32, u16, u16, nuvec_s *);
void LoseHelmet(GameObject_s *, i32, i32);
void ObjHitShield(GameObject_s *, GameObject_s *, i32, BOLT_s *);
void SpawnCreatureFromCrate(GameObject_s *, f32, f32);
