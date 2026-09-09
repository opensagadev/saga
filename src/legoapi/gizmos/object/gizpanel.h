#pragma once

#include "legoapi/gizmo/base/gizmo.h"

extern i32 gizpanel_gizmotype_id;

#ifdef __cplusplus

typedef struct GIZPANEL_s GIZPANEL;
struct GameObject_s;

ADDGIZMOTYPE *GizPanel_RegisterGizmo(i32 type_id);
void GizPanel_Reset(GIZPANEL *panel);
void GizPanel_GetAbsTargetPos(GIZPANEL *panel, NUVEC *position, i32 target_index);
void GizPanel_GetAbsPlayerPos(GIZPANEL *panel, NUVEC *position);
i32 GizPanel_CanUsePanel(GameObject_s *object, GIZPANEL *panel);
i32 GizPanel_BeingUsed(GIZPANEL *panel);
GIZPANEL *GizPanel_FindByName(WORLDINFO_s *world, char *name);
GIZPANEL *GizPanel_FindNearest(WORLDINFO_s *world, NUVEC *position, GameObject_s *object, f32 *distance_squared,
                               i32 check_eligibility);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
