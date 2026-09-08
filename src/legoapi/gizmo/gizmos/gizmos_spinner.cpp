#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nutrig.h"

void GizSpinner_Find(WORLDINFO_s *, nuvec_s *, i32) {
}

void GizSpinner_Push(GIZSPINNER_s *, i32) {
}

void GizSpinner_Spin(GIZSPINNER_s *, i32) {
}

int GizSpinner_Update(GIZSPINNER_s *) {
    return 0;
}

int GizSpinner_GetState(GIZSPINNER_s *) {
    return 0;
}

void GizSpinner_PushFail(GameObject_s *, GIZSPINNER_s *) {
}

void GizSpinner_FindNearest(nuvec_s *, WORLDINFO_s *, float *) {
}

void GizSpinners_InitTerrain(WORLDINFO_s *) {
}

void GizSpinner_GetSpinnerPos(GIZSPINNER_s *, nuvec_s *) {
}

i32 GizSpinner_GetTargetPoints(GIZSPINNER_s *spinner, nuvec_s *positions, nuvec_s *directions) {
    if (spinner == NULL || spinner->type == 0)
        return 0;
    u16 step = static_cast<u16>(65536 / spinner->type);
    u16 base = spinner->rotation + spinner->initial_rotation + spinner->field_0x08c;
    u16 position_angle = base - 0x8000;
    u32 flags = spinner->state_flags & 6;
    u16 direction_angle = base + ((flags == 0 || flags == 6) ? -0x4000 : 0x4000);
    f32 y = spinner->position.y + spinner->field_0x098;
    i32 count = 0;
    for (; count < spinner->type; ++count) {
        if (directions != NULL) {
            directions[count].x = NuTrigTable[direction_angle >> 1];
            directions[count].y = 0.0f;
            directions[count].z = NuTrigTable[((direction_angle + 0x4000) >> 1) & 0x7fff];
        }
        if (positions != NULL) {
            positions[count].x = spinner->field_0x094 * NuTrigTable[position_angle >> 1] + spinner->position.x;
            positions[count].y = y;
            positions[count].z =
                spinner->field_0x094 * NuTrigTable[((position_angle + 0x4000) >> 1) & 0x7fff] + spinner->position.z;
        }
        position_angle += step;
        direction_angle += step;
    }
    return count;
}

f32 GizSpinner_GetNearestTargetPoint(GIZSPINNER_s *spinner, nuvec_s *origin, nuvec_s *position, nuvec_s *direction,
                                     i32 check_direction) {
    NUVEC positions[8], directions[8];
    i32 count = GizSpinner_GetTargetPoints(spinner, positions, directions);
    if (count == 0 || origin == NULL)
        return -1.0f;
    f32 nearest_distance = 1000000000.0f;
    NUVEC *nearest_position = NULL;
    NUVEC *nearest_direction = NULL;
    for (i32 i = 0; i < count; ++i) {
        f32 distance = NuVecDistSqr(origin, &positions[i], NULL);
        if (check_direction != 0) {
            u16 angle = spinner->field_0x08c + 0x4000 - spinner->rotation - spinner->initial_rotation +
                        (-65536 / spinner->type) * i;
            NUVEC offset;
            NuVecSub(&offset, origin, &spinner->position);
            NuVecRotateY(&offset, &offset, angle);
            if (!(distance < nearest_distance && offset.z >= 0.0f))
                continue;
        } else if (!(distance < nearest_distance)) {
            continue;
        }
        nearest_distance = distance;
        nearest_position = &positions[i];
        nearest_direction = &directions[i];
    }
    if (nearest_position == NULL)
        return -1.0f;
    *position = *nearest_position;
    *direction = *nearest_direction;
    return nearest_distance;
}
