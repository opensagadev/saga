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

void GuidedMissile_Kill(PART_s *part, i32 reason) {
    AddGameDebris(WORLD->debris_sys, 0x7d, &part->position);
    AddGameDebris(WORLD->debris_sys, 0x21, &part->position);
    GameCam_Judder(GameCam, 0.2f, 0, NULL);
    Game_KillPart(part, reason);
    NewRumbleAllPlayers(0.7f, 0.1f, 0, 0);
}

void GuidedMissile_Move(PART_s *part, float time) {
    if (part->scale_time > guided_life) {
        part->field_124[3] = static_cast<i32>(SeekValF(static_cast<f32>(part->field_124[3]), 0.0f, 2.0f));
    } else if (part->recipient != NULL && part->scale_time > guided_start_time) {
        NUVEC direction;
        NuVecSub(&direction, &part->recipient->apiobj.collision_position, &part->position);
        i32 yaw = NuAtan2D(direction.x, direction.z);
        NuVecRotateY(&direction, &direction, -yaw);
        i32 pitch = -NuAtan2D(part->recipient->apiobj.collision_position.y + 0.5f - part->position.y, direction.z);
        part->rotation_x = SeekRot(part->rotation_x, static_cast<u16>(pitch), 1.0f);
        part->rotation_y = SeekRot(part->rotation_y, static_cast<u16>(yaw), 3.0f);
        part->field_124[3] = guided_rotate_speed;
    } else {
        part->field_124[3] = guided_rotate_speed;
    }

    part->field_13c += static_cast<i32>(static_cast<f32>(part->field_124[3]) * FRAMETIME);

    part->velocity.x = 0.0f;
    part->velocity.y = 0.0f;
    part->velocity.z = guided_speed;
    NuVecRotateX(&part->velocity, &part->velocity, part->rotation_x);
    NuVecRotateY(&part->velocity, &part->velocity, part->rotation_y);

    NUVEC position;
    position.x = part->position.x + part->velocity.x * time;
    position.y = part->position.y + part->velocity.y * time + part->gravity * time;
    position.z = part->position.z + part->velocity.z * time;

    NuMtxSetRotationX(&part->transform, NuAngAdd(part->rotation_x, 0x4000));
    NuMtxRotateY(&part->transform, part->rotation_y);
    NuMtxPreRotateY(&part->transform, part->field_13c);
    NuMtxTranslate(&part->transform, &position);

    if (part->scale_time < 0.2f) {
        NUVEC scale = {part->scale_time / 0.2f, part->scale_time / 0.2f, part->scale_time / 0.2f};
        NuMtxPreScale(&part->transform, &scale);
    }
    if (static_cast<i8>(part->active) < 0) {
        i32 count = ParticlesPerFrame(1.0f, FRAMETIME);
        NUVEC momentum = {-part->velocity.x, -part->velocity.y, -part->velocity.z};
        AddGameDebrisMom(WORLD->debris_sys, 0xb, &position, count, &momentum);
    }
}

void GuidedMissile_Deflect(PART_s *part) {
    part->flags &= ~0x4000;
    part->flags |= 0x80;
    NewPartRotation(part);
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
