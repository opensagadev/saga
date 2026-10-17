#pragma once

#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/legoapi_types.h"

struct GameObject_s;

struct LEVER_CONFIG {
    i16 floor_target_object_id;
    i16 reserved;
};

DECOMP_ASSERT(sizeof(LEVER_CONFIG) == 0x4, "LEVER_CONFIG size");

extern LEVER_CONFIG LeverSys;
extern i32 lever_gizmotype_id;

#ifdef __cplusplus

typedef struct LEVER_s LEVER;

ADDGIZMOTYPE *Levers_RegisterGizmo(i32 type_id);
void Levers_InitTerrain(WORLDINFO_s *world);
void Lever_MoveCode(WORLDINFO_s *world, GameObject_s *object);
LEVER_s *Lever_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, f32 *distance_squared);
i32 Lever_BeingPulled(LEVER_s *lever);
i32 Lever_FullyPulledDown(LEVER_s *lever);
void Lever_GetAbsTargetPos(LEVER_s *lever, nuvec_s *target_position);

inline void Lever_StartPull(GameObject_s *object, LEVER_s *lever) {
    object->field_0x788 = lever;
    object->context_animation_timer = 0.0f;
    object->character_context = 0x4a;
    object->field_0x768 = 0.0f;
    object->context_animation = 0x5d;
    object->airborne_action_duration = AnimDuration(object->id, 0x5d, 0.0f, 0.0f, 1);
    if (object->airborne_action_duration <= 0.0f)
        object->airborne_action_duration = 1.0f;
    object->context_flags &= ~0x40;
    object->apiobj.movement_facing_angle = static_cast<LEVER_s *>(object->field_0x788)->y_rotation;
    static_cast<LEVER_s *>(object->field_0x788)->interacting = 1;
    static_cast<LEVER_s *>(object->field_0x788)->pull_progress = 0.0f;
    static_cast<LEVER_s *>(object->field_0x788)->auto_reset_timer = 0.0f;
}

extern "C" {
#endif

#ifdef __cplusplus
}
#endif

// Declared here for the other units that use them.
struct GameObject_s;
void ReleaseLever(GameObject_s *);
