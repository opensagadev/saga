#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);
bool LineIntersectCircle(NUVEC *, NUVEC *, NUVEC *, f32);

BOLT_s *FindIncomingBolt(GameObject_s *object, i32 exclude_players, i32 mark_direct_hit) {
    BOLT_s *nearest = NULL;
    f32 radius = object->apiobj.field_0x1e0 > object->apiobj.field_0x1dc ? object->apiobj.field_0x1e0
                                                                         : object->apiobj.field_0x1dc;
    f32 nearest_distance = 1.0e8f;
    for (i32 i = 0; i < 32; ++i) {
        BOLT_s *bolt = &Bolt[i];
        if (bolt->active == 0)
            continue;
        GameObject_s *owner = bolt->owner;
        if (exclude_players != 0 && owner != NULL && (owner->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            owner->apiobj.field_0x287 == 0 && owner->apiobj.field_0x27c != -1)
            continue;
        f32 time = TouchHacks::TouchControlsActive ? 1.5f : 0.5f;
        f32 distance = NuVecDistSqr(&bolt->position, &object->apiobj.collision_position, NULL);
        if (distance < time * bolt->speed * time * bolt->speed &&
            LineIntersectSphere(&bolt->position, &bolt->field_0xac, &object->apiobj.collision_position, radius * radius,
                                NULL) &&
            distance < nearest_distance) {
            nearest = bolt;
            nearest_distance = distance;
        }
    }
    if (nearest != NULL && mark_direct_hit != 0) {
        radius = object->apiobj.field_0x1dc * 0.75f;
        if (LineIntersectCircle(&nearest->position, &nearest->field_0xac, &object->apiobj.collision_position,
                                radius * radius))
            object->field_0xe21 |= 0x20;
    }
    return nearest;
}

void GuidedMissile_Kill(PART_s *, i32) {
    STUBBED();
}

void GuidedMissile_Move(PART_s *, float) {
    STUBBED();
}

void GuidedMissile_Deflect(PART_s *) {
    STUBBED();
}

extern "C" i16 id_SPEEDERBIKE;
i32 InitBolt_AddMomentumType_LSW(BOLT_s *bolt, GameObject_s *object, NUVEC *momentum) {
    if (object == NULL)
        return 0;
    if (LSW1 != 0 && momentum != NULL) {
        f32 scale = WORLD->current_level == BONUS_GUNSHIPB_LDATA ? 0.25f : 0.5f;
        NuVecScale(momentum, &object->target_velocity, scale);
        bolt->speed += NuVecMag(momentum);
        return 0;
    }
    if (WORLD->current_level == SPEEDERCHASEA_LDATA && object->id == id_SPEEDERBIKE)
        return 1;
    if (WORLD->current_level == DEATHSTARBATTLED_LDATA)
        return 1;
    return 0;
}
