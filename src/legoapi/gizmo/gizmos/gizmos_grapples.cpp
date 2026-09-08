#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

void Grapple_DrawLine(GameObject_s *) {
}

void Grapple_MoveCode(GameObject_s *) {
}

i32 Grapple_LookAtPos(GameObject_s *, nuvec_s *) {
    return 0;
}

void Grapple_AddDynamic(void *, i32) {
}

void Grapple_ReachedTop(GameObject_s *) {
}

void Grapple_FindNearest(WORLDINFO_s *, nuvec_s *, GameObject_s *, float *) {
}

// Original 0x4d7090, 23 bytes.
void Grapple_SetRotOrder(GameObject_s *object) {
    if (object->field_0x7a3 != 1)
        object->field_0x1086 = 2;
}

// Original 0x4d6f90, 248 bytes.
i32 Grapple_SetTargetMom(GameObject_s *object) {
    if (object->field_0x7a3 == 1) {
        object->target_velocity.x = (object->external_force.x - object->apiobj.upper_position.x) * 20.0f;
        object->target_velocity.y = (object->external_force.y - object->apiobj.upper_position.y) * 20.0f;
        object->target_velocity.z = (object->external_force.z - object->apiobj.upper_position.z) * 20.0f;
    } else {
        NUVEC point;
        Grapple_SetPlayerTargetPoint(object, &point);
        object->target_velocity.x = (point.x - object->apiobj.upper_position.x) * 5.0f;
        object->target_velocity.y = (point.y - object->apiobj.upper_position.y) * 5.0f;
        object->target_velocity.z = (point.z - object->apiobj.upper_position.z) * 5.0f;
    }
    return 1;
}

void Grapple_RemoveDynamic(void *) {
}

void Grapple_FindNearestToPos(WORLDINFO_s *, nuvec_s *) {
}

// Original 0x4d6eb0, 220 bytes.
void Grapple_SetPlayerTargetPoint(GameObject_s *object, nuvec_s *target) {
    target->x = 0.0f;
    target->z = 0.0f;
    target->y = -object->field_0x768;
    i32 amplitude = (static_cast<i32>(object->grapple_swing_degrees) << 16) / 360;
    i32 angle = static_cast<i32>(static_cast<f32>(amplitude) * NuTrigTable[object->grapple_swing_phase >> 1] *
                                 static_cast<GRAPPLE *>(object->field_0x788)->field_0x4c);
    NuVecRotateX(target, target, angle);
    NuVecRotateY(target, target, object->takeover_start_angle);
    NuVecAdd(target, target, &static_cast<GRAPPLE *>(object->field_0x788)->shadow_probe_position);
}

// Static grapple list helpers. Moved from gizmisc_stubs.cpp.

static __used__ void Grapple_FindNearestInList(nuvec_s *, GRAPPLE_s *, int, GameObject_s *, GRAPPLE_s **, float *) {
}
