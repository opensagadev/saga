#include "legoapi/characters/motion/chris.h"

#include "decomp.h"
#include "globals.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/levels/podrace.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern f32 SpaceRumbleTimer;
extern i32 LevFlag[4];
spacelevel_scale_s STARFIGHTERDRAWSCALE;
NUVEC Jetpos = {0.15f, 0.08f, 0.32f};
void DrawCross_Now(_vuv_s *position, f32 size, i32 colour, i32 mode);
extern GameObject_s *Player[8];
extern f32 FRAMETIME;
extern LEVELDATA_s *DOGFIGHTA_LDATA;
extern BOLT_s Bolt[32];
extern i32 i_bolt;
extern f32 BOLT_OVERRIDE_PLAYERBOLTSPEED;
extern f32 BOLT_OVERRIDE_PLAYERBOLTDURATION;
// Four debug door keys start enabled and are restored by each restart.
i32 DogDebKey[4] __attribute__((aligned(16))) = {-1, -1, -1, -1};
struct quickboltinfo;
extern "C" void NuSpecialList(NUGSCN *);
extern "C" i32 NuSpecialFind(NUGSCN *, nuhspecial_s *, char *, i32);
extern "C" i32 NuSpecialExistsFn(void *);
extern "C" nuvec_s *NuSpecialGetPos(void *);
void ChrisAnakinCReset();
static NUVEC4 RadialMoveCentre;
static __used__ f32 RadialPlayerRadius[2];
static __used__ f32 MaxRadialCamY;

static void MakeWingFormation(_vuv_s *start, _vuv_s *target, f32 duration, i32 mode) {
    spacelevel_s *space = WORLD->space_level;
    if (space == NULL)
        return;
    i32 index;
    if (space->flight_groups[0].active == 0)
        index = 0;
    else if (space->flight_groups[1].active == 0)
        index = 1;
    else if (space->flight_groups[2].active == 0)
        index = 2;
    else if (space->flight_groups[3].active == 0)
        index = 3;
    else if (space->flight_groups[4].active == 0)
        index = 4;
    else if (space->flight_groups[5].active == 0)
        index = 5;
    else if (space->flight_groups[6].active == 0)
        index = 6;
    else if (space->flight_groups[7].active == 0)
        index = 7;
    else
        return;
    spacelevel_flight_group_s *group = &space->flight_groups[index];
    group->active = 1;
    group->draw_target = 0;
    NuMtxSetIdentity(&group->matrix);
    group->matrix = GameCam->render_mtx;
    group->matrix.m30 = start->x;
    group->matrix.m31 = start->y;
    group->matrix.m32 = start->z;
    group->matrix.m33 = start->w;
    if (mode == 0) {
        group->speed = 200.0f;
        group->state = 1;
    } else {
        group->state = 0;
        NuMtxPreRotateY(&group->matrix, 32768);
        group->speed = 35.0f;
    }
    group->velocity.x = target->x - start->x;
    group->velocity.y = target->y - start->y;
    group->velocity.z = target->z - start->z;
    NuVecNorm(reinterpret_cast<NUVEC *>(&group->velocity), reinterpret_cast<NUVEC *>(&group->velocity));
    const f32 speed = group->speed;
    group->velocity.x *= speed;
    group->velocity.y *= speed;
    group->velocity.z *= speed;
    group->duration = duration;
    // The retail vector temporaries use 16-byte aligned stack storage.
    NUVEC4_ALIGNED16 offset = {-5.0f, 0.0f, 0.0f, 0.0f};
    i16 angle = static_cast<i16>(qrand());
    const i16 step = static_cast<i16>(qrand() / 21) + 0x2666;
#define INIT_WING_FIGHTER(index)                                                                                       \
    do {                                                                                                               \
        starfighter_s *fighter = &group->fighters[index];                                                              \
        NuVecRotateZ(reinterpret_cast<NUVEC *>(&fighter->parent_offset), reinterpret_cast<NUVEC *>(&offset), angle);   \
        fighter->shoot_timer = 2.0f;                                                                                   \
        fighter->active = 1;                                                                                           \
        fighter->parent = group;                                                                                       \
        fighter->health = 1;                                                                                           \
        fighter->delete_when_done = 0;                                                                                 \
        fighter->spline = NULL;                                                                                        \
        fighter->model_id = 54;                                                                                        \
        fighter->draw_flags = 1;                                                                                       \
    } while (0)
    INIT_WING_FIGHTER(0);
    angle = static_cast<i16>(angle + step);
    INIT_WING_FIGHTER(1);
    angle = static_cast<i16>(angle + step);
    INIT_WING_FIGHTER(2);
    angle = static_cast<i16>(angle + step);
    INIT_WING_FIGHTER(3);
    angle = static_cast<i16>(angle + step);
    INIT_WING_FIGHTER(4);
#undef INIT_WING_FIGHTER
}

static void StarFighterAlign(starfighter_s *fighter, _vuv_s *direction, f32 distance, i32 mode) {
    NUVEC4_ALIGNED16 local, up;
    NuVecInvMtxRotate(reinterpret_cast<NUVEC *>(&local), reinterpret_cast<NUVEC *>(direction), &fighter->matrix);
    const i16 yaw = static_cast<i16>(NuAtan2D(local.x, local.z));
    fighter->yaw = yaw;
    NuVecRotateY(reinterpret_cast<NUVEC *>(&local), reinterpret_cast<NUVEC *>(&local), -yaw);
    const i16 pitch = static_cast<i16>(NuAtan2D(local.y, local.z));
    fighter->pitch = pitch;
    NuMtxPreRotateY(&fighter->matrix, yaw);
    NuMtxPreRotateX(&fighter->matrix, -pitch);
    if (mode != 0) {
        NuMtxPreRotateZ(&fighter->matrix, 512);
        return;
    }
    const i16 world_yaw = static_cast<i16>(NuAtan2D(direction->x, direction->z));
    NuVecRotateY(reinterpret_cast<NUVEC *>(&up), reinterpret_cast<NUVEC *>(direction), -world_yaw);
    const i16 world_pitch = static_cast<i16>(NuAtan2D(up.y, up.z));
    up.x = 1.0f;
    up.y = up.z = up.w = 0.0f;
    NuVecRotateZ(reinterpret_cast<NUVEC *>(&up), reinterpret_cast<NUVEC *>(&up),
                 static_cast<i16>(static_cast<i32>(fighter->target_position.w)) + 16384);
    NuVecRotateX(reinterpret_cast<NUVEC *>(&up), reinterpret_cast<NUVEC *>(&up), -world_pitch);
    NuVecRotateY(reinterpret_cast<NUVEC *>(&up), reinterpret_cast<NUVEC *>(&up), world_yaw);
    NuVecInvMtxRotate(reinterpret_cast<NUVEC *>(&up), reinterpret_cast<NUVEC *>(&up), &fighter->matrix);
    f32 roll = static_cast<i16>(NuAtan2D(up.x, up.y));
    f32 limit = 24000.0f * FRAMETIME;
    if (limit < roll)
        roll = static_cast<i16>(static_cast<i32>(limit));
    limit = -24000.0f * FRAMETIME;
    if (roll < limit)
        roll = static_cast<i16>(static_cast<i32>(limit));
    i32 adjustment = 0;
    if (distance <= 2.0f) {
        if (distance <= 1.0f)
            adjustment = -static_cast<i16>(static_cast<i32>(roll));
        else
            adjustment = -static_cast<i16>(static_cast<i32>((2.0f - distance) * roll));
    }
    NuMtxPreRotateZ(&fighter->matrix, adjustment);
}

f32 MissileDist = -3.0f;
static NUVEC4 PlayerPos, PlayerVel;

static i32 ProcessStarFighter(starfighter_s *fighter, quickboltinfo *info) {
    if (Player[0] != NULL && Player[0]->sock_position.distance > 435.0f)
        MissileDist = 2.0f;
    if (fighter->hit_count != 0) {
        const bool escort = fighter->spline != NULL && static_cast<u32>(fighter->spline->id - 84) < 2;
        if (escort)
            fighter->health = 0;
        else
            --fighter->health;
        --fighter->hit_count;
        if (fighter->health == 0) {
            fighter->death_timer = escort ? 1.0f : 2.0f;
            NUVEC impulse;
            if (fighter->parent != NULL) {
                impulse.x = fighter->matrix.m30 - fighter->parent->matrix.m30;
                impulse.y = fighter->matrix.m31 - fighter->parent->matrix.m31;
                impulse.z = fighter->matrix.m32 - fighter->parent->matrix.m32;
                fighter->parent->target.x = fighter->matrix.m30;
                fighter->parent->target.y = fighter->matrix.m31;
                fighter->parent->target.z = fighter->matrix.m32;
                fighter->parent->target.w = fighter->matrix.m33;
            } else {
                impulse = v010;
            }
            const f32 magnitude = NuVecMag(&impulse);
            if (magnitude != 0.0f) {
                const f32 scale = (qrand() * (11.0f / 65535.0f)) / magnitude;
                impulse.x *= scale;
                impulse.y *= scale;
                impulse.z *= scale;
            }
            fighter->velocity.x += impulse.x;
            fighter->velocity.y += impulse.y;
            fighter->velocity.z += impulse.z;
            fighter->death_rotate_x = qrand() * (65536.0f / 65535.0f) - 32768.0f;
            fighter->death_rotate_z = qrand() * (65536.0f / 65535.0f) - 32768.0f;
            if (!escort)
                AddGameDebris(WORLD->debris_sys, 23, reinterpret_cast<NUVEC *>(&fighter->matrix.m30));
        }
        if (fighter->health == -2)
            fighter->death_timer = 0.0f;
    }
    if (fighter->health < 1 && (fighter->spline == NULL || static_cast<u32>(fighter->spline->id - 84) > 1)) {
        fighter->death_timer -= FRAMETIME;
        if (fighter->death_timer > 0.0f) {
            NuMtxPreRotateX(&fighter->matrix,
                            static_cast<i16>(static_cast<i32>(fighter->death_rotate_x * FRAMETIME * 4.0f)));
            NuMtxPreRotateZ(&fighter->matrix,
                            static_cast<i16>(static_cast<i32>(fighter->death_rotate_z * FRAMETIME * 4.0f)));
            fighter->matrix.m31 += FRAMETIME * fighter->velocity.y;
            fighter->matrix.m32 += FRAMETIME * fighter->velocity.z;
            fighter->matrix.m30 += FRAMETIME * fighter->velocity.x;
            AddVariableShotDebrisEffect(WORLD->debris_sys->entries[48].effect,
                                        reinterpret_cast<NUVEC *>(&fighter->matrix.m30), 1, 0, 0);
        } else {
            AddGameDebris(WORLD->debris_sys, 23, reinterpret_cast<NUVEC *>(&fighter->matrix.m30));
            fighter->active = 0;
        }
        return 0;
    }
    if (fighter->parent != NULL) {
        fighter->matrix = fighter->parent->matrix;
        fighter->velocity = fighter->parent->velocity;
        NuVecMtxTransform(reinterpret_cast<NUVEC *>(&fighter->matrix.m30),
                          reinterpret_cast<NUVEC *>(&fighter->parent_offset), &fighter->parent->matrix);
        fighter->shoot_timer -= FRAMETIME;
        if (fighter->shoot_timer > 0.0f)
            return 1;
        _vuv_s position = {0.0f, 0.0f, -10.0f, 1.0f};
        NuVecMtxTransform(reinterpret_cast<NUVEC *>(&position), reinterpret_cast<NUVEC *>(&position), &fighter->matrix);
        fighter->shoot_timer = 1.0f;
        return 1;
    }
    if (fighter->spline == NULL)
        return 0;
    NUVEC4_ALIGNED16 direction;
    direction.x = fighter->target_position.x - fighter->matrix.m30;
    direction.y = fighter->target_position.y - fighter->matrix.m31;
    direction.z = fighter->target_position.z - fighter->matrix.m32;
    direction.w = 0.0f;
    f32 distance_squared = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    if (fighter->death_timer >= 0.0f)
        fighter->death_timer -= FRAMETIME;
    bool finished = false;
    while (distance_squared < 25.0f) {
        fighter->spline_progress += 0.01f;
        if (fighter->spline_progress > 1.0f) {
            finished = true;
            fighter->spline_progress -= 1.0f;
            distance_squared = 1000000.0f;
            break;
        }
        CalcSplinePoint(fighter->spline, reinterpret_cast<_vuv_s *>(&fighter->target_position),
                        fighter->spline_progress);
        if (fighter->model_id == -307) {
            fighter->target_position.x += fighter->matrix.m10 * MissileDist;
            fighter->target_position.y += fighter->matrix.m11 * MissileDist;
            fighter->target_position.z += fighter->matrix.m12 * MissileDist;
        }
        direction.x = fighter->target_position.x - fighter->matrix.m30;
        direction.y = fighter->target_position.y - fighter->matrix.m31;
        direction.z = fighter->target_position.z - fighter->matrix.m32;
        distance_squared = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    }
    const f32 camera_distance = (fighter->matrix.m30 - global_camera.mtx.m30) * global_camera.mtx.m20 +
                                (fighter->matrix.m31 - global_camera.mtx.m31) * global_camera.mtx.m21 +
                                (fighter->matrix.m32 - global_camera.mtx.m32) * global_camera.mtx.m22;
    f32 pitch = 1.25f;
    if (camera_distance < 0.0f) {
        pitch = 0.5f;
        if (camera_distance >= -2.0f)
            pitch = 0.25f * camera_distance + 1.0f;
    }
    NUVEC *position = reinterpret_cast<NUVEC *>(&fighter->matrix.m30);
    if (fighter->model_id == -299) {
        if (fighter->health > 0)
            PlaySfxAndSetPitch("Dog_TriFighterEngLp", position, pitch);
    } else if (fighter->model_id == -300) {
        PlaySfxAndSetPitch("Dog_CloneARC170EngLp", position, pitch);
    } else if ((fighter->model_id == -298 || fighter->model_id == -297) && fighter->health > 0) {
        PlaySfxAndSetPitch("Dog_DroidFighterEngLp", position, pitch);
    }
    if (finished) {
        fighter->active = 0;
    } else {
        const f32 distance = NuFsqrt(distance_squared);
        if (distance != 0.0f) {
            const f32 speed = fighter->speed / distance;
            fighter->velocity.x = direction.x * speed;
            fighter->velocity.y = direction.y * speed;
            fighter->velocity.z = direction.z * speed;
            direction.x = direction.x * speed * FRAMETIME;
            direction.y = direction.y * speed * FRAMETIME;
            direction.z = direction.z * speed * FRAMETIME;
            fighter->matrix.m32 += direction.z;
            fighter->matrix.m30 += direction.x;
            fighter->matrix.m31 += direction.y;
            StarFighterAlign(fighter, reinterpret_cast<_vuv_s *>(&direction), distance, fighter->death_timer > 0.0f);
            fighter->movement.x = direction.x;
            fighter->movement.y = direction.y;
            fighter->movement.z = direction.z;
            fighter->movement.w = direction.w;
        }
    }
    fighter->matrix.m33 = 1.0f;
    if (fighter->delete_when_done != 0)
        return 1;
    if (fighter->initial_delay != 0.0f) {
        fighter->initial_delay -= FRAMETIME;
        if (fighter->initial_delay > 0.0f)
            return 1;
        fighter->initial_delay = 0.0f;
        fighter->aim_bias = 2.0f;
        return 1;
    }
    if (fighter->target_ready == 0) {
        spacelevel_s *space = WORLD->space_level;
        if (fighter->spline == NULL)
            return 1;
        const i32 target_id = fighter->spline->field_0x51c;
        if (target_id == -1) {
            fighter->target_id = -1;
            fighter->target = NULL;
            return 1;
        } else if (target_id == -2 || target_id == -3) {
            fighter->target_ready = 1;
            fighter->target_id = target_id;
            fighter->target = NULL;
        } else {
            starfighter_s *target = space->queued_fighters;
            fighter->target_id = -1;
            while (target->active == 0 || target->object_id != target_id) {
                if (++target == space->queued_fighters + 96)
                    return 1;
            }
            fighter->target = target;
            fighter->target_ready = 1;
            fighter->target_id = target_id;
        }
    }
    if (fighter->model_id == -307)
        return 1;
    if (fighter->fired != 0 && fighter->aim_bias > 0.0f)
        fighter->aim_bias -= FRAMETIME + FRAMETIME;
    if (fighter->shoot_timer > 0.0f) {
        fighter->shoot_timer -= FRAMETIME;
        return 1;
    }
    fighter->shoot_timer = NuRandFloat() * 0.2f + 0.2f;
    if (info == NULL)
        return 1;
    NUMTX_ALIGNED16 aim_matrix;
    if (fighter->target_id == -3) {
        aim_matrix = fighter->matrix;
    } else {
        NUVEC *target_position, *target_velocity;
        if (fighter->target_id == -1)
            goto lost_target;
        if (fighter->target_id == -2) {
            GameObject_s *player = NULL;
            if (NuRandFloat() < 0.5f && Player[0] != NULL && (Player[0]->apiobj.object_flags & 0x80) != 0)
                player = Player[0];
            else if (Player[1] != NULL && (Player[1]->apiobj.object_flags & 0x80) != 0)
                player = Player[1];
            if (player != NULL) {
                memcpy(&PlayerPos, &player->apiobj.collision_position, sizeof(NUVEC));
                memcpy(&PlayerVel, &player->apiobj.velocity, sizeof(NUVEC));
            }
            target_position = reinterpret_cast<NUVEC *>(&PlayerPos);
            target_velocity = reinterpret_cast<NUVEC *>(&PlayerVel);
        } else {
            if (fighter->target == NULL || fighter->target->active == 0)
                goto lost_target;
            target_position = reinterpret_cast<NUVEC *>(&fighter->target->matrix.m30);
            // The retail code predicts this target using the firing ship's velocity.
            target_velocity = reinterpret_cast<NUVEC *>(&fighter->velocity);
        }
        NUVEC offset = {target_position->x - fighter->matrix.m30, target_position->y - fighter->matrix.m31,
                        target_position->z - fighter->matrix.m32};
        f32 forward = offset.x * fighter->matrix.m20 + offset.y * fighter->matrix.m21 + offset.z * fighter->matrix.m22;
        if (forward < 5.0f || forward > 100.0f)
            goto lost_target;
        if (forward * forward / (offset.x * offset.x + offset.y * offset.y + offset.z * offset.z) < 0.969f)
            return 1;
        const f32 closing_speed =
            50.0f - (fighter->matrix.m20 * target_velocity->x + fighter->matrix.m21 * target_velocity->y +
                     fighter->matrix.m22 * target_velocity->z);
        if (closing_speed < 1.0f)
            return 1;
        forward /= closing_speed;
        offset.x += target_velocity->x * forward;
        offset.y += target_velocity->y * forward;
        offset.z += target_velocity->z * forward;
        if (fighter->fired == 0) {
            NUVEC *axis = reinterpret_cast<NUVEC *>(&fighter->matrix.m00);
            f32 least = NuFabs(axis->x * offset.x + axis->y * offset.y + axis->z * offset.z);
            NUVEC *up = reinterpret_cast<NUVEC *>(&fighter->matrix.m10);
            const f32 up_dot = NuFabs(up->x * offset.x + up->y * offset.y + up->z * offset.z);
            if (up_dot <= least) {
                axis = up;
                least = up_dot;
            }
            NUVEC *front = reinterpret_cast<NUVEC *>(&fighter->matrix.m20);
            if (NuFabs(front->x * offset.x + front->y * offset.y + front->z * offset.z) <= least)
                axis = front;
            fighter->aim_direction.y = offset.z * axis->x - axis->z * offset.x;
            fighter->aim_direction.x = offset.y * axis->z - offset.z * axis->y;
            fighter->aim_direction.z = offset.x * axis->y - axis->x * offset.y;
            NuVecNorm(reinterpret_cast<NUVEC *>(&fighter->aim_direction),
                      reinterpret_cast<NUVEC *>(&fighter->aim_direction));
        }
        if (fighter->aim_bias != 0.0f) {
            offset.x += fighter->aim_bias * fighter->aim_direction.x;
            offset.y += fighter->aim_bias * fighter->aim_direction.y;
            offset.z += fighter->aim_bias * fighter->aim_direction.z;
        }
        aim_matrix = fighter->matrix;
        NUVEC local;
        NuVecInvMtxRotate(&local, &offset, &aim_matrix);
        const i16 yaw = static_cast<i16>(NuAtan2D(local.x, local.z));
        NuVecRotateY(&local, &local, -yaw);
        const i16 pitch = static_cast<i16>(NuAtan2D(local.y, local.z));
        NuMtxPreRotateY(&aim_matrix, yaw);
        NuMtxPreRotateX(&aim_matrix, -pitch);
    }
    {
        const NUVEC velocity = {aim_matrix.m20 * 75.0f, aim_matrix.m21 * 75.0f, aim_matrix.m22 * 75.0f};
        if (fighter->missile_count != 0 && static_cast<u32>(fighter->spline->id - 84) > 1 && NuRandFloat() < 0.2f &&
            g_lowEndLevelBehaviour == 0) {
            spacelevel_s *space = WORLD->space_level;
            for (i32 i = 0; i != 96; ++i) {
                starfighter_s *missile = &space->queued_fighters[i];
                if (missile->active != 0)
                    continue;
                --fighter->missile_count;
                *missile = *fighter;
                missile->model_id = -307;
                missile->speed += 10.0f;
                missile->matrix.m31 += missile->matrix.m11 * MissileDist;
                missile->matrix.m30 += missile->matrix.m10 * MissileDist;
                missile->matrix.m32 += missile->matrix.m12 * MissileDist;
                const f32 dx = missile->matrix.m30 - global_camera.mtx.m30;
                const f32 dy = missile->matrix.m31 - global_camera.mtx.m31;
                const f32 dz = missile->matrix.m32 - global_camera.mtx.m32;
                if (dx * dx + dy * dy + dz * dz < 40000.0f)
                    PlaySfx("Ep3_1_ProtoXWingMissile", reinterpret_cast<NUVEC *>(&missile->matrix.m30));
                break;
            }
        }
        quickbolt_s *bolt = info->bolts + info->used;
        quickbolt_s *end = info->bolts + info->count;
        quickbolt_s *slot = bolt;
        for (; slot < end; ++slot) {
            if (slot->duration == 0.0f)
                break;
        }
        if (slot >= end) {
            slot = info->bolts;
            for (; slot < bolt; ++slot) {
                if (slot->duration == 0.0f)
                    break;
            }
            if (slot >= bolt) {
                fighter->fired = 1;
                return 1;
            }
        }
        info->used = slot - info->bolts;
        memset(slot, 0, sizeof(*slot));
        const f32 dx = aim_matrix.m30 - global_camera.mtx.m30;
        const f32 dy = aim_matrix.m31 - global_camera.mtx.m31;
        const f32 dz = aim_matrix.m32 - global_camera.mtx.m32;
        if (dx * dx + dy * dy + dz * dz < 40000.0f) {
            NUVEC *position = reinterpret_cast<NUVEC *>(&aim_matrix.m30);
            if (fighter->model_id == -299)
                PlaySfx("Dog_TriFighterGuns", position);
            else if (fighter->model_id == -300)
                PlaySfx("Dog_CloneARC170Gun", position);
            else if (fighter->model_id == -298 || fighter->model_id == -297)
                PlaySfx("Dog_DroidFighterBlast", position);
        }
        slot->duration = 2.5f;
        slot->matrix = aim_matrix;
        NuMtxPreRotateX(&slot->matrix, 16384);
        slot->type = fighter->model_id == -300;
        slot->velocity.x = velocity.x + aim_matrix.m10 * 0.0f + aim_matrix.m00 * 0.0f;
        slot->velocity.y = velocity.y + aim_matrix.m11 * 0.0f + aim_matrix.m01 * 0.0f;
        slot->velocity.z = velocity.z + aim_matrix.m12 * 0.0f + aim_matrix.m02 * 0.0f;
        slot->velocity.w = 0.0f;
    }
    fighter->fired = 1;
    return 1;
lost_target:
    if (fighter->fired != 0)
        fighter->delete_when_done = 1;
    return 1;
}

static void ProcessSpaceLevel(spacelevel_s *space) {
    space->door_elapsed = space->door_countdown;
    space->door_time += FRAMETIME;
    space->door_countdown += FRAMETIME * space->normalized_speed;
    if (space->actions != NULL) {
        if (space->unknown_337c == 4) {
            space->current_action = &space->actions[space->unknown_3378];
            space->unknown_337c = space->current_action->command;
            ++space->unknown_3378;
            if (space->unknown_337c == 3) {
                space->current_action = space->actions;
                space->unknown_3378 = 1;
                space->unknown_337c = space->actions->command;
            }
            if (space->unknown_337c == 1) {
                _vuv_s start = {0.0f, 0.0f, 300.0f, 1.0f};
                _vuv_s target = {GameCam->render_mtx.m30, GameCam->render_mtx.m31, GameCam->render_mtx.m32, 0.0f};
                NuVecMtxTransform(reinterpret_cast<NUVEC *>(&start), reinterpret_cast<NUVEC *>(&start),
                                  &GameCam->render_mtx);
                MakeWingFormation(&start, &target, space->current_action->value, 1);
                space->unknown_337c = 4;
            } else if (space->unknown_337c == 2) {
                _vuv_s start = {-10.0f, -5.0f, -10.0f, 1.0f};
                NuVecRotateZ(reinterpret_cast<NUVEC *>(&start), reinterpret_cast<NUVEC *>(&start), qrand());
                NuVecMtxTransform(reinterpret_cast<NUVEC *>(&start), reinterpret_cast<NUVEC *>(&start),
                                  &GameCam->render_mtx);
                _vuv_s target = {0.0f, 0.0f, 400.0f, 1.0f};
                NuVecMtxTransform(reinterpret_cast<NUVEC *>(&target), reinterpret_cast<NUVEC *>(&target),
                                  &GameCam->render_mtx);
                MakeWingFormation(&start, &target, space->current_action->value, 0);
                space->unknown_337c = 4;
            } else if (space->unknown_337c == 0) {
                space->action_timer = space->current_action->value;
            }
        }
        if (space->unknown_337c == 0) {
            space->action_timer -= FRAMETIME;
            if (space->action_timer <= 0.0f)
                space->unknown_337c = 4;
        }
    }
    quickboltinfo *info = &space->quick_bolts;
    for (i32 i = 0; i != 8; ++i) {
        spacelevel_flight_group_s *group = &space->flight_groups[i];
        if (group->active == 0)
            continue;
        if (static_cast<u32>(group->state) < 2) {
            if (group->state == 0) {
                NuVecSub(reinterpret_cast<NUVEC *>(&group->velocity),
                         reinterpret_cast<NUVEC *>(&GameCam->render_mtx.m30),
                         reinterpret_cast<NUVEC *>(&group->matrix.m30));
                group->velocity.w = 1.0f;
                NuVecNorm(reinterpret_cast<NUVEC *>(&group->velocity), reinterpret_cast<NUVEC *>(&group->velocity));
                const f32 speed = group->speed;
                group->velocity.x *= speed;
                group->velocity.y *= speed;
                group->velocity.z *= speed;
            }
            group->matrix.m30 += group->velocity.x * FRAMETIME;
            group->matrix.m31 += group->velocity.y * FRAMETIME;
            group->matrix.m32 += group->velocity.z * FRAMETIME;
            NuVecInvMtxTransform(reinterpret_cast<NUVEC *>(&group->camera_position),
                                 reinterpret_cast<NUVEC *>(&group->matrix.m30), &GameCam->render_mtx);
            group->duration -= FRAMETIME;
            if (group->duration < 0.0f || (group->camera_position.z > 800.0f && group->state == 1) ||
                (group->camera_position.z < 0.0f && group->state == 0))
                group->active = 0;
        }
        i32 active = 0;
#define PROCESS_WING_FIGHTER(index)                                                                                    \
    if (group->fighters[index].active != 0)                                                                            \
    active |= ProcessStarFighter(&group->fighters[index], info)
        PROCESS_WING_FIGHTER(0);
        PROCESS_WING_FIGHTER(1);
        PROCESS_WING_FIGHTER(2);
        PROCESS_WING_FIGHTER(3);
        PROCESS_WING_FIGHTER(4);
#undef PROCESS_WING_FIGHTER
        if (active == 0)
            group->draw_target = 1;
        if (active == 0 || group->draw_target != 0) {
            group->target.y = group->velocity.y * FRAMETIME + group->matrix.m31;
            group->target.z = group->velocity.z * FRAMETIME + group->matrix.m32;
            group->target.x = group->velocity.x * FRAMETIME + group->matrix.m30;
        }
    }
    if (info->count != 0) {
        bool valid[2] = {false, false};
        NUVEC player_position[2];
        if (Player[0] != NULL && (Player[0]->apiobj.object_flags & 0x80) != 0) {
            player_position[0] = *reinterpret_cast<NUVEC *>(&Player[0]->apiobj.field_0xb8.m30);
            valid[0] = true;
        }
        if (Player[1] != NULL && (Player[1]->apiobj.object_flags & 0x80) != 0) {
            player_position[1] = *reinterpret_cast<NUVEC *>(&Player[1]->apiobj.field_0xb8.m30);
            valid[1] = true;
        }
        quickbolt_s *end = info->bolts + info->count;
        for (quickbolt_s *bolt = info->bolts; bolt < end; ++bolt) {
            if (bolt->duration == 0.0f)
                continue;
            bolt->matrix.m31 += bolt->velocity.y * FRAMETIME;
            bolt->matrix.m32 += bolt->velocity.z * FRAMETIME;
            bolt->matrix.m30 += bolt->velocity.x * FRAMETIME;
            if (Player[0] != NULL && valid[0]) {
                const f32 dx = bolt->matrix.m30 - player_position[0].x;
                const f32 dy = bolt->matrix.m31 - player_position[0].y;
                const f32 dz = bolt->matrix.m32 - player_position[0].z;
                if (dx * dx + dy * dy + dz * dz < 1.0f) {
                    bolt->duration = 0.0f;
                    ObjHitObj(NULL, Player[0], 1, 0, 0, 1);
                }
            }
            if (valid[1] && Player[1] != NULL) {
                const f32 dx = bolt->matrix.m30 - player_position[1].x;
                const f32 dy = bolt->matrix.m31 - player_position[1].y;
                const f32 dz = bolt->matrix.m32 - player_position[1].z;
                if (dx * dx + dy * dy + dz * dz < 1.0f) {
                    bolt->duration = 0.0f;
                    ObjHitObj(NULL, Player[1], 1, 0, 0, 1);
                }
            }
            const f32 duration = bolt->duration - FRAMETIME;
            bolt->duration = duration >= 0.0f ? duration : 0.0f;
        }
    }
    for (i32 i = 0; i != 256; ++i) {
        flightspline_s *spline = &space->flight_splines[i];
        if (spline->point_count == 0 || !(spline->spawn_time > space->door_elapsed) ||
            !(spline->spawn_time <= space->door_countdown))
            continue;
        if (g_lowEndLevelBehaviour == 0) {
            for (i32 j = 0; j != 96; ++j) {
                starfighter_s *fighter = &space->queued_fighters[j];
                if (fighter->active != 0)
                    continue;
                fighter->spawn_time = spline->spawn_time;
                NuMtxSetIdentity(&fighter->matrix);
                fighter->matrix.m30 = spline->points[0].x;
                fighter->matrix.m31 = spline->points[0].y;
                fighter->matrix.m32 = spline->points[0].z;
                fighter->matrix.m33 = spline->points[0].w;
                _vuv_s direction = {spline->points[1].x - spline->points[0].x,
                                    spline->points[1].y - spline->points[0].y,
                                    spline->points[1].z - spline->points[0].z, 0.0f};
                StarFighterAlign(fighter, &direction, 1.0f, 0);
                fighter->velocity.x = fighter->velocity.y = fighter->velocity.z = 0.0f;
                fighter->velocity.w = 1.0f;
                fighter->target_position = *reinterpret_cast<NUVEC4 *>(&spline->points[0]);
                fighter->active = 1;
                fighter->parent = NULL;
                fighter->spline = spline;
                fighter->spline_progress = 0.0f;
                fighter->health = 1;
                fighter->hit_count = fighter->fired = fighter->delete_when_done = 0;
                fighter->initial_delay = fighter->shoot_timer = 0.0f;
                fighter->scale = 1.0f;
                fighter->speed = 50.0f;
                if (spline->id == 87)
                    fighter->model_id = -297;
                else if (spline->id == 86)
                    fighter->model_id = -299;
                else if (spline->id == 54) {
                    fighter->model_id = -298;
                    fighter->scale = 2.0f;
                } else if (spline->id == 85 || spline->id == 84)
                    fighter->model_id = -300;
                else
                    fighter->model_id = spline->id;
                fighter->draw_flags = 1;
                fighter->object_id = spline->field_0x520;
                fighter->target_id = spline->field_0x51c;
                fighter->missile_count = 1;
                if (spline->id == 87 || spline->id == 86 || static_cast<u32>(spline->id - 84) < 2)
                    fighter->speed = 22.5f;
                else if (spline->id == -307)
                    fighter->speed = 33.0f;
                break;
            }
        }
        if (spline->field_0x40c == 0.0f)
            spline->spawn_time = -1.0f;
        else {
            if (spline->field_0x514 != 0) {
                if (--spline->repeat_count == 0) {
                    spline->spawn_time = -1.0f;
                    continue;
                }
            }
            spline->spawn_time += spline->field_0x40c;
        }
    }
    for (i32 i = 0; i != 96; ++i) {
        if (space->queued_fighters[i].active != 0)
            ProcessStarFighter(&space->queued_fighters[i], info);
    }
    i32 door_index = -1;
    for (i32 i = 6; i >= 0; --i) {
        if ((Player[0] != NULL && Player[0]->field_0x68c > DogFightDoors.doors[i].distance) ||
            (Player[1] != NULL && Player[1]->field_0x68c > DogFightDoors.doors[i].distance)) {
            door_index = i;
            break;
        }
    }
    if (door_index >= 0) {
        DOOR_s *door = WORLD->doors;
        for (i32 i = 0; i < WORLD->door_count; ++i, ++door) {
            if (NuStrICmp(door->name, DogFightDoors.doors[door_index].name) == 0) {
                Doors_SetLastDoor(door);
                break;
            }
        }
    }
    if (WORLD->area == DOGFIGHT_ADATA)
        SpaceRumbleProcess();
}

void ChrisAnakinAUpdate(WORLDINFO_s *) {
    ProcessSpaceLevel(WORLD->space_level);
}

void ChrisDogFightAUpdate(WORLDINFO_s *world) {
    SpaceResetAudioPoint();
    ProcessCurrentSpeed(world, DogFightSpeedList);
    ProcessSpaceLevel(world->space_level);
    if (AreaGlobals.values.field_0x00 == 0 && *reinterpret_cast<u8 *>(LevFlag) == 0 && DOGFIGHT_ADATA != NULL &&
        Game.area_save[DOGFIGHT_ADATA->index].area_complete == 0 && GamePlayTimer.time_elapsed >= 3.0f)
        *reinterpret_cast<u8 *>(LevFlag) = 1;
}

anakin_door_setup_s DoorSetupList[15] = {
    {"door1", "door1r", 15.0f, 1.0f, 0, {}, {-1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door2", "door2r", 15.0f, 1.0f, 0, {}, {1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door3", "door3r", 15.0f, 1.0f, 0, {}, {1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door4", "door4r", 15.0f, 1.0f, 0, {}, {-1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door5", "door5r", 15.0f, 1.0f, 0, {}, {1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door6", "door6r", 15.0f, 1.0f, 0, {}, {-1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door7", "door7r", 15.0f, 1.0f, 0, {}, {1.0f, 1.0f, 0.0f}, -0.5f, 0.0f},
    {"door8", "door8r", 15.0f, 1.0f, 0, {}, {0.0f, -1.0f, 0.0f}, -0.5f, 0.0f},
    {"door9", "door9r", 15.0f, 1.0f, 0, {}, {0.0f, 1.0f, 0.0f}, -0.5f, 0.0f},
    {"door10", "door10r", 15.0f, 1.0f, 0, {}, {0.0f, -1.0f, 0.0f}, -0.5f, 0.0f},
    {"door11", "door11r", 15.0f, 1.0f, 0, {}, {1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door12", "door12r", 15.0f, 1.0f, 0, {}, {-1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door15", "door15r", 15.0f, 1.0f, 0, {}, {1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    {"door16", "door16r", 15.0f, 1.0f, 0, {}, {-1.0f, 0.0f, 0.0f}, -0.5f, 0.0f},
    // The retail table stores an empty string here, not a null name.
    {"", NULL, 0.0f, 0.0f, 0, {}, {0.0f, 0.0f, 0.0f}, 0.0f, 0.0f},
};

static void DrawStarFighter(starfighter_s *starfighter) {
    const i32 model_id = starfighter->model_id;
    NUMTX_ALIGNED16 matrices[2];
    NUMTX &scaled_special_matrix = matrices[0];
    NUMTX &scaled_model_matrix = matrices[1];
    if (model_id >= 0) {
        const f32 scale = starfighter->scale;
        const i16 draw_flags = starfighter->draw_flags;
        const i16 model_index = apicharsys->playermodelids[model_id];
        if (model_index == -1)
            return;
        NUMTX *matrix = &starfighter->matrix;
        if (scale != 1.0f) {
            scaled_model_matrix = starfighter->matrix;
            NuMtxPreScaleUVU0(&scaled_model_matrix, scale);
            matrix = &scaled_model_matrix;
        }
        GameDrawCharacterModel(&apicharsys->models[model_index], NULL, matrix, NULL, NULL, NULL, NULL, draw_flags);
    } else {
        NUMTX *matrix = &starfighter->matrix;
        if (model_id == -299 || model_id == -297 || model_id == -298 || model_id == -307) {
            scaled_special_matrix = starfighter->matrix;
            scaled_special_matrix.m00 *= 1.15f;
            scaled_special_matrix.m01 *= 1.15f;
            scaled_special_matrix.m02 *= 1.15f;
            scaled_special_matrix.m10 *= 1.15f;
            scaled_special_matrix.m11 *= 1.15f;
            scaled_special_matrix.m12 *= 1.15f;
            scaled_special_matrix.m20 *= 1.15f;
            scaled_special_matrix.m21 *= 1.15f;
            scaled_special_matrix.m22 *= 1.15f;
            matrix = &scaled_special_matrix;
        }
        NuSpecialDrawAt(&WORLD->lev_objs[-model_id].special, matrix);
        if (model_id == -307)
            AddVariableShotDebrisEffect(WORLD->debris_sys->entries[49].effect,
                                        reinterpret_cast<NUVEC *>(&starfighter->matrix.m30), 1, 0, 0);
    }
}

static inline void QuickBolt_Draw(quickboltinfo *info) {
    static const i32 BoltObjA[4] = {303, 305, 301, 301};
    static const i32 BoltObjB[4] = {304, 306, 302, 302};
    if (info->count != 0) {
        quickbolt_s *bolt = info->bolts;
        quickbolt_s *end = bolt + info->count;
        for (; bolt < end; ++bolt) {
            if (bolt->duration != 0.0f) {
                Draw3DObjectMtx(WORLD, BoltObjA[bolt->type], &bolt->matrix);
                Draw3DObjectMtx(WORLD, BoltObjB[bolt->type], &bolt->matrix);
            }
        }
    }
}

static void DrawSpaceLevel(spacelevel_s *space) {
    if (space->crosses[0].enabled != 0) {
        NuVecMtxTransform(reinterpret_cast<NUVEC *>(&space->crosses[0].world_position),
                          reinterpret_cast<NUVEC *>(&space->crosses[0].local_position), &GameCam->render_mtx);
        DrawCross_Now(reinterpret_cast<_vuv_s *>(&space->crosses[0].world_position), space->crosses[0].scale,
                      space->crosses[0].colour, 1);
    }
    if (space->crosses[1].enabled != 0) {
        NuVecMtxTransform(reinterpret_cast<NUVEC *>(&space->crosses[1].world_position),
                          reinterpret_cast<NUVEC *>(&space->crosses[1].local_position), &GameCam->render_mtx);
        DrawCross_Now(reinterpret_cast<_vuv_s *>(&space->crosses[1].world_position), space->crosses[1].scale,
                      space->crosses[1].colour, 1);
    }
#define DRAW_SPACE_FIGHTER(group_index, fighter_index)                                                                 \
    if (space->flight_groups[group_index].fighters[fighter_index].active != 0)                                         \
    DrawStarFighter(&space->flight_groups[group_index].fighters[fighter_index])
#define DRAW_SPACE_GROUP(group_index)                                                                                  \
    do {                                                                                                               \
        if (space->flight_groups[group_index].active != 0) {                                                           \
            DRAW_SPACE_FIGHTER(group_index, 0);                                                                        \
            DRAW_SPACE_FIGHTER(group_index, 1);                                                                        \
            DRAW_SPACE_FIGHTER(group_index, 2);                                                                        \
            DRAW_SPACE_FIGHTER(group_index, 3);                                                                        \
            DRAW_SPACE_FIGHTER(group_index, 4);                                                                        \
            if (space->flight_groups[group_index].draw_target != 0)                                                    \
                DrawCross_Now(reinterpret_cast<_vuv_s *>(&space->flight_groups[group_index].target), 3.0f, 0xffffff,   \
                              1);                                                                                      \
        }                                                                                                              \
    } while (0)
    DRAW_SPACE_GROUP(0);
    DRAW_SPACE_GROUP(1);
    DRAW_SPACE_GROUP(2);
    DRAW_SPACE_GROUP(3);
    DRAW_SPACE_GROUP(4);
    DRAW_SPACE_GROUP(5);
    DRAW_SPACE_GROUP(6);
    DRAW_SPACE_GROUP(7);
#undef DRAW_SPACE_GROUP
#undef DRAW_SPACE_FIGHTER
    for (i32 i = 0; i != 96; ++i) {
        if (space->queued_fighters[i].active != 0)
            DrawStarFighter(&space->queued_fighters[i]);
    }
    QuickBolt_Draw(&space->quick_bolts);

#define DRAW_SPACE_JET(player_index, key_index, left)                                                                  \
    do {                                                                                                               \
        GameObject_s *player = Player[player_index];                                                                   \
        if (player != NULL) {                                                                                          \
            if (player->id == id_JEDISTARFIGHTERYELLOWEP3 || player->id == id_JEDISTARFIGHTERREDEP3) {                 \
                if (DogDebKey[key_index] == -1) {                                                                      \
                    AddDebrisEffect(&DogDebKey[key_index], WORLD->debris_sys->entries[47].effect, 0.0f, 0.0f, 0.0f);   \
                } else {                                                                                               \
                    NUMTX_ALIGNED16 matrix = player->apiobj.field_0xb8;                                                \
                    matrix.m10 = -matrix.m10;                                                                          \
                    matrix.m11 = -matrix.m11;                                                                          \
                    matrix.m12 = -matrix.m12;                                                                          \
                    matrix.m13 = -matrix.m13;                                                                          \
                    if (left) {                                                                                        \
                        matrix.m30 += (matrix.m10 * Jetpos.y - matrix.m00 * Jetpos.x) + matrix.m20 * Jetpos.z;         \
                        matrix.m31 += (matrix.m11 * Jetpos.y - matrix.m01 * Jetpos.x) + matrix.m21 * Jetpos.z;         \
                        matrix.m32 += (matrix.m12 * Jetpos.y - matrix.m02 * Jetpos.x) + matrix.m22 * Jetpos.z;         \
                    } else {                                                                                           \
                        matrix.m30 += matrix.m00 * Jetpos.x + matrix.m10 * Jetpos.y + matrix.m20 * Jetpos.z;           \
                        matrix.m31 += matrix.m01 * Jetpos.x + matrix.m11 * Jetpos.y + matrix.m21 * Jetpos.z;           \
                        matrix.m32 += matrix.m02 * Jetpos.x + matrix.m12 * Jetpos.y + matrix.m22 * Jetpos.z;           \
                    }                                                                                                  \
                    DebrisPosOrientationMtx(DogDebKey[key_index], &matrix);                                            \
                }                                                                                                      \
            } else if (DogDebKey[key_index] != -1) {                                                                   \
                DebFreeInstantly(&DogDebKey[key_index]);                                                               \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)
    DRAW_SPACE_JET(0, 0, false);
    DRAW_SPACE_JET(0, 1, true);
    DRAW_SPACE_JET(1, 2, false);
    DRAW_SPACE_JET(1, 3, true);
#undef DRAW_SPACE_JET
}

void ChrisAnakinADraw() {
    DrawSpaceLevel(WORLD->space_level);
}

void ChrisAnakinDDraw() {
    DrawSpaceLevel(WORLD->space_level);
}

void ChrisDogFightADraw(WORLDINFO_s *world) {
    DrawSpaceLevel(world->space_level);
}

void ResetSpaceLevel(WORLDINFO_s *, spacelevel_s *) __asm__("_ZL15ResetSpaceLevelP11WORLDINFO_sP12spacelevel_s")
    __attribute__((visibility("hidden")));
void ResetSpaceLevel(WORLDINFO_s *world, spacelevel_s *space) {
    i32 door_index;
    space->unknown_62eb8 = 0;
    space->player_origin = {-1456.9f, 326.5f, -394.0f};
    space->player_origin_padding = 0.0f;
    space->direction = {1447.4901f, -492.25f, -704.0f};
    space->camera_origin = {-9.409912f, -165.75f, -1098.0f};

    space->direction_length = NuVecMag(&space->direction);
    space->inverse_direction_length = 1.0f / space->direction_length;

#define DOOR_REACHED(player_index, door_index)                                                                         \
    (Player[player_index]->field_0x68c > DogFightDoors.doors[door_index].distance)
    if (Player[0] != NULL) {
        if (DOOR_REACHED(0, 6)) {
            goto door_6;
        }
        if (Player[1] != NULL && DOOR_REACHED(1, 6)) {
            goto door_6;
        }
        if (DOOR_REACHED(0, 5)) {
            goto door_5;
        }
        if (Player[1] != NULL && DOOR_REACHED(1, 5)) {
            goto door_5;
        }
        if (DOOR_REACHED(0, 4)) {
            goto door_4;
        }
        if (Player[1] != NULL && DOOR_REACHED(1, 4)) {
            goto door_4;
        }
        if (DOOR_REACHED(0, 3)) {
            goto door_3;
        }
        if (Player[1] != NULL && DOOR_REACHED(1, 3)) {
            goto door_3;
        }
        if (DOOR_REACHED(0, 2)) {
            goto door_2;
        }
        if (Player[1] != NULL && DOOR_REACHED(1, 2)) {
            goto door_2;
        }
        if (DOOR_REACHED(0, 1)) {
            goto door_1;
        }
        if (Player[1] != NULL && DOOR_REACHED(1, 1)) {
            goto door_1;
        }
        if (DOOR_REACHED(0, 0)) {
            goto door_0;
        }
        if (Player[1] != NULL && DOOR_REACHED(1, 0)) {
            goto door_0;
        }
    } else if (Player[1] != NULL) {
        if (DOOR_REACHED(1, 6)) {
            goto door_6;
        }
        if (DOOR_REACHED(1, 5)) {
            goto door_5;
        }
        if (DOOR_REACHED(1, 4)) {
            goto door_4;
        }
        if (DOOR_REACHED(1, 3)) {
            goto door_3;
        }
        if (DOOR_REACHED(1, 2)) {
            goto door_2;
        }
        if (DOOR_REACHED(1, 1)) {
            goto door_1;
        }
        if (DOOR_REACHED(1, 0)) {
            goto door_0;
        }
    }

#undef DOOR_REACHED

    space->door_countdown = 0.0f;
    space->door_time = 0.0f;
    space->door_elapsed = 0.0f;

reset_space:

#define RESET_STARFIGHTER(fighter)                                                                                     \
    fighter.active = 0;                                                                                                \
    fighter.parent = NULL;                                                                                             \
    fighter.spline = NULL
#define RESET_SPACE_GROUP(index)                                                                                       \
    do {                                                                                                               \
        RESET_STARFIGHTER(space->flight_groups[index].fighters[0]);                                                    \
        RESET_STARFIGHTER(space->flight_groups[index].fighters[1]);                                                    \
        RESET_STARFIGHTER(space->flight_groups[index].fighters[2]);                                                    \
        RESET_STARFIGHTER(space->flight_groups[index].fighters[3]);                                                    \
        RESET_STARFIGHTER(space->flight_groups[index].fighters[4]);                                                    \
        space->flight_groups[index].reset_colour = 0xff00;                                                             \
        space->flight_groups[index].active = 0;                                                                        \
        space->flight_groups[index].draw_target = 0;                                                                   \
    } while (0)
    RESET_SPACE_GROUP(0);
    RESET_SPACE_GROUP(1);
    RESET_SPACE_GROUP(2);
    RESET_SPACE_GROUP(3);
    RESET_SPACE_GROUP(4);
    RESET_SPACE_GROUP(5);
    RESET_SPACE_GROUP(6);
    RESET_SPACE_GROUP(7);
#undef RESET_SPACE_GROUP
#undef RESET_STARFIGHTER

    STARFIGHTERDRAWSCALE = {4.0f, 4.0f, 4.0f, 1.0f};
    space->draw_scale = {-141.0f, -121.0f, -1034.0f, 1.0f};
    if (world->current_level == DOGFIGHTA_LDATA) {
        space->unknown_3370 = NULL;
    } else {
        space->unknown_3370 = &Actions_AnakinA;
    }
    if (space->reset_buffer_count != 0) {
        memset(space->reset_buffer, 0, space->reset_buffer_count * 96);
        space->reset_buffer_used = 0;
    }
    space->unknown_337c = 4;
    space->current_action = NULL;
    space->unknown_3378 = 0;
    space->unknown_338c = space;
    space->value_one_a = 1.0f;
    space->value_one_b = 1.0f;

    space->player_matrix_flags = 0;
    space->player_colour = 0xffffff;
    space->player_matrix.m00 = 80.0f;
    space->player_matrix.m01 = 55.0f;
    space->player_matrix.m02 = 200.0f;
    space->player_matrix.m03 = 1.0f;
    space->player_matrix.m30 = 0.0f;
    space->player_matrix.m31 = 0.0f;
    space->player_matrix.m32 = 0.0f;
    space->player_matrix.m33 = 1.0f;
    space->player_matrix.m10 = 0.0f;
    space->player_matrix.m11 = 0.0f;
    space->player_matrix.m13 = 1.0f;
    space->player_matrix.m20 = 0.0f;
    space->player_matrix.m21 = 0.0f;
    space->player_matrix.m22 = 0.0f;
    space->player_matrix.m23 = 1.0f;
    space->player_matrix.m12 = 200.0f;
    space->player_matrix_state = 0;

    space->camera_matrix.m00 = 5.0f;
    space->camera_matrix.m01 = 80.0f;
    space->camera_matrix.m02 = 55.0f;
    space->camera_matrix.m03 = 200.0f;
    space->camera_matrix.m30 = 0.0f;
    space->camera_matrix.m31 = 0.0f;
    space->camera_matrix.m32 = 0.0f;
    space->camera_matrix.m33 = 1.0f;
    space->camera_matrix.m10 = 0.0f;
    space->camera_matrix.m11 = 0.0f;
    space->camera_matrix.m13 = 1.0f;
    space->camera_matrix.m20 = 0.0f;
    space->camera_matrix.m21 = 0.0f;
    space->camera_matrix.m22 = 0.0f;
    space->camera_matrix.m23 = 1.0f;
    space->camera_matrix.m12 = 200.0f;
    space->camera_matrix_w = 1.0f;
    space->camera_value = 5.0f;
    space->camera_colour = 0xffffff;
    space->camera_matrix_flags = 0;
    space->camera_matrix_state = 0;

    for (i32 i = 0; i < 96; ++i) {
        space->queued_fighters[i].active = 0;
        space->queued_fighters[i].parent = NULL;
        space->queued_fighters[i].spline = NULL;
    }
    for (i32 i = 0; i < 256; ++i) {
        space->large_records[i].saved_value = space->large_records[i].reset_value;
        space->large_records[i].reset_state = space->large_records[i].saved_state;
    }
    SpaceRumbleTimer = NuRandFloat() * 10.0f + 3.0f;
    return;

door_6:
    door_index = 6;
    goto set_door_timer;
door_5:
    door_index = 5;
    goto set_door_timer;
door_4:
    door_index = 4;
    goto set_door_timer;
door_3:
    door_index = 3;
    goto set_door_timer;
door_2:
    door_index = 2;
    goto set_door_timer;
door_1:
    door_index = 1;
    goto set_door_timer;
door_0:
    door_index = 0;

set_door_timer:
    space->door_time = DogFightDoors.doors[door_index].timer;
    space->door_countdown = space->door_time * 94.977417f / 59.449684f;
    space->door_elapsed = (space->door_countdown - FRAMETIME) * 94.977417f / 59.449684f;
    goto reset_space;
}

void ChrisRadialCam(nuvec_s *position, nuvec_s *target) {
    const f32 position_y = position->y;
    const f32 target_y = target->y;
    NUVEC position_delta = {position->x - RadialMoveCentre.x, 0.0f, position->z - RadialMoveCentre.z};
    const f32 position_radius = NuVecMag(&position_delta);
    NUVEC origin_delta = {-RadialMoveCentre.x, 0.0f, -RadialMoveCentre.z};
    const f32 origin_radius = NuVecMag(&origin_delta);
    RadialPlayerRadius[0] = origin_radius;

    f32 base_radius = 120.0f;
    if (origin_radius >= 120.0f) {
        base_radius = MIN(150.0f, origin_radius);
    }
    const f32 extra_radius = origin_radius - base_radius;
    f32 target_radius = base_radius + extra_radius;
    if (position_radius != 0.0f) {
        const f32 scale = target_radius / position_radius;
        position_delta.x *= scale;
        position_delta.z *= scale;
    }
    position->x = RadialMoveCentre.x + position_delta.x;
    position->y = position_y;
    position->z = RadialMoveCentre.z + position_delta.z;

    if (base_radius <= origin_radius) {
        target_radius = base_radius + 0.6f * extra_radius;
    }
    if (origin_radius != 0.0f) {
        const f32 scale = target_radius / origin_radius;
        origin_delta.x *= scale;
        origin_delta.z *= scale;
    }
    target->x = RadialMoveCentre.x + origin_delta.x;
    target->y = target_y;
    target->z = RadialMoveCentre.z + origin_delta.z;
}

void ChrisAnakinAInit(WORLDINFO_s *world) {
    ResetSpaceLevel(world, world->space_level);
}

void ChrisAnakinBDraw() {
}

void ChrisAnakinBInit() {
    RadialMoveCentre.x = 0.0f;
    RadialMoveCentre.y = 0.0f;
    RadialMoveCentre.z = 0.0f;
    RadialMoveCentre.w = 1.0f;
    MaxRadialCamY = 8.36f;
    NuSpecialList(WORLD->current_gscn);
    nuhspecial_s centre;
    if (NuSpecialFind(WORLD->current_gscn, &centre, "Centre", 1) != 0 && NuSpecialExistsFn(&centre) != 0) {
        nuvec_s *position = NuSpecialGetPos(&centre);
        memcpy(&RadialMoveCentre, position, sizeof(NUVEC));
    }
}

void ChrisAnakinCInit() {
    NuSpecialList(WORLD->current_gscn);
    ChrisAnakinCReset();
}

void ChrisAnakinDInit(WORLDINFO_s *world) {
    ResetSpaceLevel(world, world->space_level);
}

void DogFightARestart() {
    memset(DogDebKey, 0xff, sizeof(DogDebKey));
}

void ChrisAnakinAPanel(WORLDINFO_s *) {
}

void ChrisAnakinAReset(WORLDINFO_s *world) {
    ResetSpaceLevel(world, world->space_level);
}

void ChrisAnakinBReset() {
}

void ChrisAnakinCReset() {
    anakin_door_s *door = AnakinC;
    i32 count = 0;
    for (anakin_door_setup_s *setup = DoorSetupList; count < 12; ++setup) {
        if (setup->name == NULL) {
            for (; count < 12; ++count, ++door)
                door->active = 0;
            return;
        }
        if (!NuSpecialFind(WORLD->current_gscn, &door->special, setup->name, 1) || !NuSpecialExistsFn(&door->special))
            continue;

        if (NuSpecialFind(WORLD->current_gscn, &door->secondary_special, setup->secondary_name, 1))
            door->has_secondary = static_cast<i16>(NuSpecialExistsFn(&door->secondary_special));

        NuMtxSetIdentity(&door->original_matrix);
        door->matrix = door->original_matrix = *NuSpecialGetMtx(&door->special);
        if (door->has_secondary != 0) {
            NuMtxSetIdentity(&door->original_secondary_matrix);
            door->secondary_matrix = door->original_secondary_matrix = *NuSpecialGetMtx(&door->secondary_special);
        }
        door->platform_id = static_cast<i16>(FindPlatInst(NuSpecialGetInstanceix(&door->special)));
        door->active = 1;
        door->flags = static_cast<u8>(setup->flags);
        door->direction = setup->direction;
        door->offset = setup->initial_offset;
        door->speed = setup->speed;
        door->minimum_offset = setup->minimum_offset;
        door->unknown_134 = setup->unknown_20;
        ++door;
        ++count;
    }
}

void ChrisAnakinDReset(WORLDINFO_s *world) {
    ResetSpaceLevel(world, world->space_level);
}

void ChrisAnakinBUpdate() {
}

void ChrisAnakinCUpdate() {
    anakin_door_s *door = AnakinC;
    anakin_door_s *end = door + 12;
    for (; door != end; ++door) {
        if (door->active == 0)
            continue;

        const f32 offset = door->offset - door->speed * FRAMETIME;
        if (offset <= door->minimum_offset)
            door->offset = door->minimum_offset;
        else
            door->offset = offset;

        // The retail transform temporaries require a 16-byte-aligned stack.
        NUVEC_ALIGNED16 translation = {door->direction.x * door->offset, door->direction.y * door->offset,
                                       door->direction.z * door->offset};
        NUVEC_ALIGNED16 position;
        NuVecMtxTransform(&position, &translation, &door->original_matrix);
        memcpy(&door->matrix.m30, &position, sizeof(position));
        door->matrix.m33 = 1.0f;
        NuSpecialSetDrawMtx(&door->special, &door->matrix);
        if (door->has_secondary != 0) {
            NuVecMtxTransform(&position, &translation, &door->original_secondary_matrix);
            memcpy(&door->secondary_matrix.m30, &position, sizeof(position));
            door->secondary_matrix.m33 = 1.0f;
            NuSpecialSetDrawMtx(&door->secondary_special, &door->secondary_matrix);
        }
    }
}

void ChrisAnakinDUpdate(WORLDINFO_s *) {
}

void ChrisAfterBurnerCam(nuvec_s *, nuvec_s *camera) {
    *camera = WORLD->space_level->camera_origin;
}

void ChrisAllocLevelStuff(WORLDINFO_s *world) {
    world->has_level_specific_data = 1;
    if (world->current_level == DOGFIGHTA_LDATA) {
        world->space_level = static_cast<spacelevel_s *>(
            GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, sizeof(spacelevel_s)));
        spacelevel_s *space = world->space_level;
        space->reset_buffer = space->unknown_5ce90;
        space->reset_buffer_count = 256;
        world->space_level->normalized_speed = 1.0f;
        if (world->current_level == DOGFIGHTA_LDATA && world->sock_sys->sock[0].current_speed != 0.0f) {
            world->space_level->normalized_speed = world->sock_sys->sock[0].current_speed / 11.0f;
        }
        world->space_level->unknown_62ef0 = 0;
    } else if (world->current_level == PODRACEA_LDATA || world->current_level == PODRACEB_LDATA ||
               world->current_level == PODRACEC_LDATA) {
        world->podrace = GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, sizeof(PODRACE_s));
    } else {
        world->has_level_specific_data = 0;
    }
}

i32 DidBoltHitChrisJobby(WORLDINFO_s *, BOLT_s *) {
    STUBBED();
    return 0;
}

i32 ShipDropCoins(starfighter_s *fighter) {
    spacelevel_s *space = WORLD->space_level;
    // Formation ships have no spline identity to record. The retail lookup
    // dereferences that null pointer; do not create an invalid history entry.
    if (fighter->spline == NULL || space->coin_history_count > 256)
        return 0;
    i32 index = 0;
    if (space->coin_history_count > 0) {
        for (; index < space->coin_history_count; ++index) {
            if (space->coin_history[index].spline_id == fighter->spline->id &&
                space->coin_history[index].spawn_time == fighter->spawn_time)
                return 0;
        }
        if (index > 255)
            return 0;
    }
    space->coin_history[index].spline_id = fighter->spline->id;
    WORLD->space_level->coin_history[index].spawn_time = fighter->spawn_time;
    ++WORLD->space_level->coin_history_count;
    return 1;
}

static i32 CollideBoltStarFighter(BOLT_s *bolt, starfighter_s *fighter, _vuv_s *position, _vuv_s *velocity) {
    const f32 vx = velocity->x - fighter->velocity.x;
    const f32 vy = velocity->y - fighter->velocity.y;
    const f32 vz = velocity->z - fighter->velocity.z;
    const f32 dx = position->x - fighter->matrix.m30;
    const f32 dy = position->y - fighter->matrix.m31;
    const f32 dz = position->z - fighter->matrix.m32;
    const f32 a = vx * vx + vy * vy + vz * vz;
    const f32 c = dx * dx + dy * dy + dz * dz - 2.0f;
    if (a <= 0.0f) {
        if (!(c <= 0.0f))
            return 0;
    } else {
        const f32 b = 2.0f * (dx * vx + dy * vy + dz * vz);
        const f32 discriminant = b * b - 4.0f * a * c;
        if (!(discriminant >= 0.0f))
            return 0;
        const f32 root = NuFsqrt(discriminant);
        f32 time = -FRAMETIME;
        if (!(time <= (root - b) / (a + a)))
            return 0;
        const f32 enter = (-b - root) / (a + a);
        if (!(enter <= 0.0f))
            return 0;
        if (time <= enter)
            time = enter;
        position->x += vx * time;
        position->y += vy * time;
        position->z += vz * time;
    }
    if (fighter->spline == NULL || static_cast<u32>(fighter->spline->id - 84) > 1) {
        BoltSys->debris(bolt, reinterpret_cast<NUVEC *>(position), 0, reinterpret_cast<NUVEC *>(&fighter->velocity), 0);
        bolt->active = 0;
        i32 coins = 0;
        if (ShipDropCoins(fighter) != 0)
            coins = fighter->model_id == -299 ? 500 : 1000;
        const i32 player = bolt->owner == NULL ? -1 : static_cast<i8>(bolt->owner->apiobj.field_0x27c);
        NUVEC *ship_position = reinterpret_cast<NUVEC *>(&fighter->matrix.m30);
        const i32 hearts = ReleaseHearts();
        AddPickups(coins, hearts, 0, 0, ship_position, NULL, 2.0f, player, 1.0f, 2000000.0f, NULL, 1, 1, true);
        const i32 part_type = PARTLookupType("DogBits");
        AddFiniteShotPART(part_type, ship_position, 1);
        const f32 dx = fighter->matrix.m30 - global_camera.mtx.m30;
        const f32 dy = fighter->matrix.m31 - global_camera.mtx.m31;
        const f32 dz = fighter->matrix.m32 - global_camera.mtx.m32;
        if (dx * dx + dy * dy + dz * dz < 40000.0f) {
            if (fighter->health < 1)
                PlaySfx("Ep3_1_ExplosionXXL", ship_position);
            else if (fighter->model_id == -299)
                PlaySfx("Dog_TriFighterHit", ship_position);
            else if (fighter->model_id == -298 || fighter->model_id == -297)
                PlaySfx("Dog_DroidFighterHit", ship_position);
        }
    }
    ++fighter->hit_count;
    return 1;
}

i32 ChrisExtraBoltCollision(BOLT_s *bolt, nuvec_s *points) {
    if (WORLD->has_level_specific_data == 0 || WORLD->space_level == NULL || (bolt->flags & 3) == 0)
        return 0;
    spacelevel_s *space = WORLD->space_level;
    NUVEC4_ALIGNED16 velocity = {bolt->velocity.x, bolt->velocity.y, bolt->velocity.z, 0.0f};
    NUVEC4_ALIGNED16 position = {points[1].x, points[1].y, points[1].z, 0.0f};
#define COLLIDE_SPACE_FIGHTER(group_index, fighter_index)                                                              \
    if (space->flight_groups[group_index].fighters[fighter_index].active != 0 &&                                       \
        CollideBoltStarFighter(bolt, &space->flight_groups[group_index].fighters[fighter_index],                       \
                               reinterpret_cast<_vuv_s *>(&position), reinterpret_cast<_vuv_s *>(&velocity)) != 0)     \
    return 1
#define COLLIDE_SPACE_GROUP(group_index)                                                                               \
    do {                                                                                                               \
        if (space->flight_groups[group_index].active != 0) {                                                           \
            COLLIDE_SPACE_FIGHTER(group_index, 0);                                                                     \
            COLLIDE_SPACE_FIGHTER(group_index, 1);                                                                     \
            COLLIDE_SPACE_FIGHTER(group_index, 2);                                                                     \
            COLLIDE_SPACE_FIGHTER(group_index, 3);                                                                     \
            COLLIDE_SPACE_FIGHTER(group_index, 4);                                                                     \
        }                                                                                                              \
    } while (0)
    COLLIDE_SPACE_GROUP(0);
    COLLIDE_SPACE_GROUP(1);
    COLLIDE_SPACE_GROUP(2);
    COLLIDE_SPACE_GROUP(3);
    COLLIDE_SPACE_GROUP(4);
    COLLIDE_SPACE_GROUP(5);
    COLLIDE_SPACE_GROUP(6);
    COLLIDE_SPACE_GROUP(7);
#undef COLLIDE_SPACE_GROUP
#undef COLLIDE_SPACE_FIGHTER
    for (i32 i = 0; i != 96; ++i) {
        if (space->queued_fighters[i].active != 0 &&
            CollideBoltStarFighter(bolt, &space->queued_fighters[i], reinterpret_cast<_vuv_s *>(&position),
                                   reinterpret_cast<_vuv_s *>(&velocity)) != 0)
            return 1;
    }
    return 0;
}

void ChrisGetSpaceShipMatrix(GameObject_s *object, numtx_s *matrix) {
    *matrix = object->apiobj.field_0xb8;
    NuMtxPreRotateY(matrix, 0x8000);
}

void ChrisGetTargetedSpaceShipMatrix(GameObject_s *object, numtx_s *matrix) {
    *matrix = object->apiobj.field_0xb8;
    NuMtxPreRotateY(matrix, 0x8000);
}
