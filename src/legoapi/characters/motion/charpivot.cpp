#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

static CHARPIVOT *CharPivot;

CHARPIVOT CharPivot_LSW[32] = {
    {"atst", 0, -1, 7.0f, 11.0f, 0, 0},         {"atst", 0, -1, 21.0f, 25.0f, 0, 0},
    {"atst", 0, 2, 8.0f, 25.0f, 0, 0},          {"atst", 0, 3, 0.0f, 11.0f, 0, 0},
    {"atst", 0, 3, 22.0f, 999.0f, 0, 0},        {"atst_lowres", 0, -1, 7.0f, 11.0f, 0, 0},
    {"atst_lowres", 0, -1, 21.0f, 25.0f, 0, 0}, {"atst_lowres", 0, 2, 8.0f, 25.0f, 0, 0},
    {"atst_lowres", 0, 3, 0.0f, 11.0f, 0, 0},   {"atst_lowres", 0, 3, 22.0f, 999.0f, 0, 0},
    {"mini_atst", 0, 2, 15.0f, 999.0f, 0, 0},   {"mini_atst", 0, 3, 0.0f, 15.0f, 0, 0},
    {"rancor", 0, -1, 0.0f, 3.0f, 0, 0},        {"rancor", 0, -1, 19.0f, 24.0f, 0, 0},
    {"rancor", 0, -1, 40.0f, 999.0f, 0, 0},     {"rancor", 64, -1, 0.0f, 3.0f, 0, 0},
    {"rancor", 64, -1, 19.0f, 24.0f, 0, 0},     {"rancor", 64, -1, 40.0f, 999.0f, 0, 0},
    {"rancor", 0, 0, 0.0f, 3.0f, 0, 0},         {"rancor", 0, 0, 20.0f, 999.0f, 0, 0},
    {"rancor", 0, 1, 0.0f, 24.0f, 0, 0},        {"rancor", 0, 1, 0.0f, 999.0f, 0, 0},
    {"rancor", 64, 0, 0.0f, 3.0f, 0, 0},        {"rancor", 64, 0, 20.0f, 999.0f, 0, 0},
    {"rancor", 64, 1, 0.0f, 24.0f, 0, 0},       {"rancor", 64, 1, 41.0f, 999.0f, 0, 0},
    {"clonewalker", 0, -1, 1.0f, 4.0f, 0, 0},   {"clonewalker", 0, -1, 15.0f, 18.0f, 0, 0},
    {"clonewalker", 0, 0, 2.0f, 18.0f, 0, 0},   {"clonewalker", 0, 1, 16.0f, 999.0f, 0, 0},
    {"clonewalker", 0, 1, 0.0f, 4.0f, 0, 0},    {NULL, 0, 0, 0.0f, 0.0f, 0, 0},
};

CHARPIVOT CharPivot_Batman;

void CharPivot_Init(CHARPIVOT *table) {
    CharPivot = table;
    if (table != NULL) {
        for (; table->name != NULL; ++table)
            table->character_id = CharIDFromName(table->name);
    }
}

void CharPivot_Check(GameObject_s *object, NUVEC *velocity) {
    if (object->apiobj.field_0x276 == object->previous_movement_angle || CharPivot == NULL)
        return;
    for (CHARPIVOT *pivot = CharPivot; pivot->name != NULL; ++pivot) {
        if (object->id != pivot->character_id)
            continue;
        if (pivot->animation != -1) {
            f32 *frame = AnimPlaying(&object->apiobj.anim_packet, pivot->animation, 1, 1);
            if (frame == NULL || !(*frame >= pivot->start_frame) || !(*frame <= pivot->end_frame))
                continue;
        }
        if (pivot->point_of_interest == -1) {
            object->apiobj.field_0x276 = object->previous_movement_angle;
            object->apiobj.movement_facing_angle = object->previous_movement_angle;
            object->apiobj.facing_angle = object->previous_movement_angle;
        } else if (object->apiobj.character_model->points_of_interest[pivot->point_of_interest] != NULL) {
            i32 rotation = -RotDiff(object->previous_movement_angle, object->apiobj.field_0x276);
            NUVEC point = *NUMTX_GET_ROW_VEC(&object->joint_matrices[pivot->point_of_interest], 3);
            NUVEC offset, previous_point;
            NuVecSub(&offset, &point, &object->apiobj.position);
            NuVecRotateY(&previous_point, &offset, rotation);
            NuVecAdd(&previous_point, &previous_point, &object->apiobj.start_position);
            NuVecSub(&offset, &point, &previous_point);
            NuVecScale(&offset, &offset, 1.0f / FRAMETIME);
            velocity->x -= offset.x;
            velocity->z -= offset.z;
        }
        return;
    }
}
