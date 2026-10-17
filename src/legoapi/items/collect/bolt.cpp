#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);
bool LineIntersectCircle(NUVEC *, NUVEC *, NUVEC *, f32);
void Game_KillPart(PART_s *part, i32 reason);
extern "C" void NewPartRotation(PART_s *part);
extern f32 guided_life;
extern f32 guided_start_time;
extern i32 guided_rotate_speed;
extern f32 guided_speed;

BOLT_s *FindIncomingBolt(GameObject_s *object, i32 exclude_players, i32 mark_direct_hit) {
    BOLT_s *nearest = NULL;
    f32 radius = object->apiobj.field_0x1e0 > object->apiobj.field_0x1dc ? object->apiobj.field_0x1e0
                                                                         : object->apiobj.field_0x1dc;
    const f32 radius_squared = radius * radius;
    f32 nearest_distance = 1.0e8f;
    BOLT_s *bolts = Bolt;
    for (BOLT_s *bolt = bolts; bolt != bolts + 32; ++bolt) {
        if (bolt->active == 0)
            continue;
        if (exclude_players != 0 && bolt->owner != NULL && (bolt->owner->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            bolt->owner->apiobj.field_0x287 == 0 && bolt->owner->apiobj.field_0x27c != -1)
            continue;
        f32 time = TouchHacks::TouchControlsActive ? 1.5f : 0.5f;
        f32 travel_squared = time * bolt->speed;
        travel_squared *= travel_squared;
        f32 distance = NuVecDistSqr(&bolt->position, &object->apiobj.collision_position, NULL);
        if (distance < travel_squared &&
            LineIntersectSphere(&bolt->position, &bolt->field_0xac, &object->apiobj.collision_position, radius_squared,
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

void GuidedMissile_Kill(PART_s *part, i32 reason);

void GuidedMissile_Move(PART_s *part, float time);

void GuidedMissile_Deflect(PART_s *part);

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
