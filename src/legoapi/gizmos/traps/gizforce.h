#pragma once

#include "nu2api/nucore/fixed_width.h"
u32 GizForce_TotalScore(void *world);

#include "legoapi/gizmo/base/gizmo.h"

extern i32 force_gizmotype_id;

#ifdef __cplusplus

typedef struct GIZFORCE_s GIZFORCE;
struct GameObject_s;
i32 GizForce_GameObjUsingForce(GameObject_s *object, GIZFORCE_s *force);
void ForceLightning_Origin(GameObject_s *object, NUVEC *primary, NUVEC *secondary);

void GizForce_PlayForwards(GIZFORCE_s *force);
void GizForce_PlayBackwards(GIZFORCE_s *force);
void GizForce_SetVisibility(GIZFORCE_s *force, i32 visibility);
i32 GizForce_AnimComplete(GIZFORCE_s *force);
i32 GizForce_Complete(GIZFORCE_s *force);
struct HINT_s;
i32 GizForce_UpdateHint(HINT_s *hint);

ADDGIZMOTYPE *GizForce_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
