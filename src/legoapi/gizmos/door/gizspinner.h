#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GIZSPINNER_s;
struct GameObject_s;
struct WORLDINFO_s;
struct nuvec_s;
extern GIZSPINNER_s *GizSpinner_Find(WORLDINFO_s *world, nuvec_s *position, i32 alternate);
extern i32 GizSpinner_Push(GIZSPINNER_s *spinner, i32 context);
extern void GizSpinner_PushFail(GameObject_s *object, GIZSPINNER_s *spinner);
f32 GizSpinner_GetNearestTargetPoint(GIZSPINNER_s *, nuvec_s *, nuvec_s *, nuvec_s *, i32);
void GizSpinners_InitTerrain(WORLDINFO_s *world);
