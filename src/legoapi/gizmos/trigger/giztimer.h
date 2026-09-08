#pragma once
#include "decomp_assert.h"

#include "legoapi/gizmo/base/gizmo.h"

extern i32 giztimer_gizmotype_id;

typedef struct GIZTIMER_s {
    f32 time_remaining;
    f32 start_time;
    char filler1[2];
    u8 flags; // unsure if this is actually what this is
    char filler2;
    char name[16];
} GIZTIMER;

DECOMP_ASSERT(sizeof(GIZTIMER) == 0x1c, "GIZTIMER ABI");
DECOMP_ASSERT(offsetof(GIZTIMER, flags) == 0xa, "GIZTIMER flags offset");
DECOMP_ASSERT(offsetof(GIZTIMER, name) == 0xc, "GIZTIMER name offset");

#ifdef __cplusplus

ADDGIZMOTYPE *GizTimer_RegisterGizmo(i32 type_id);
GIZMO *createGizTimer(void *, float time, i32 random_time, char *name);
i32 GizTimer_GetMaxGizmos(void *world_info);
void GizTimer_AddGizmos(GIZMOSYS *gizmo_sys, i32 unknown1, void *world_info, void *unknown2);
void GizTimer_Update(void *world_info, void *, float delta_time);
char *GizTimer_GetGizmoName(GIZMO *gizmo);
i32 GizTimer_GetOutput(GIZMO *gizmo, i32, i32);
char *GizTimer_GetOutputName(GIZMO *gizmo, i32 output_index);
i32 GizTimer_GetNumOutputs(GIZMO *gizmo);
void GizTimer_Activate(GIZMO *gizmo, i32 unknown);
void *GizTimer_ReserveBufferSpace(void *world_info);
i32 GizTimer_Load(void *world_info, void *);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
