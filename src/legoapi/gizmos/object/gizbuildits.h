#pragma once

#include "nu2api/nucore/fixed_width.h"
u32 GizBuildIts_TotalScore(void *world);

#include "legoapi/gizmo/base/gizmo.h"

extern i32 gizbuildit_gizmotype_id;

#ifdef __cplusplus

typedef struct GIZBUILDIT_s GIZBUILDIT;

ADDGIZMOTYPE *GizBuildIts_RegisterGizmo(i32 type_id);
struct GameObject_s;
struct WORLDINFO_s;
GIZBUILDIT_s *GizBuildIt_AnyReacting(WORLDINFO_s *world);
void GizBuildIt_KillParts(GIZBUILDIT_s *buildit);
void GizGetBuildItPlayerPos(GameObject_s *player, nuvec_s *position, nuvec_s *target);
void GizBuildItPushAwayFromStart(GameObject_s *player, GIZBUILDIT_s *buildit);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
