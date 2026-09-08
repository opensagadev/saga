#pragma once

#include "nu2api/nucore/fixed_width.h"
u32 GizBuildIts_TotalScore(void *world);

#include "legoapi/gizmo/base/gizmo.h"

extern i32 gizbuildit_gizmotype_id;

#ifdef __cplusplus

typedef struct GIZBUILDIT_s GIZBUILDIT;

ADDGIZMOTYPE *GizBuildIts_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
