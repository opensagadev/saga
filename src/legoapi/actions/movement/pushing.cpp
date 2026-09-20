#include "decomp.h"
#include "legoapi/world/world.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/nufloat.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nutex.h"
#include <math.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void ReleasePush(GameObject_s *object) {
    if (object == NULL || object->character_context == -1 ||
        (CInfo[object->character_context].flags & 0x20000000) == 0) {
        return;
    }
    if (LEGOCONTEXT_PUSHOBSTACLE != -1 && object->character_context == LEGOCONTEXT_PUSHOBSTACLE) {
        GameCam_Blend(NULL, 0.5f, 0.0f, 1);
    }
    object->field_0xdc4 = 0.0f;
    object->field_0x788 = NULL;
    object->character_context = -1;
}


void SetPushAngle(GameObject_s *object) {
    u16 angle;
    if (Pushing(object, &angle, NULL, NULL) != 0) {
        u16 adjustment = 0x8000;
        if (object->field_0x788 != NULL && LEGOCONTEXT_PUSHSPINNER != -1 &&
            LEGOCONTEXT_PUSHSPINNER == object->character_context) {
            GIZSPINNER_s *spinner = static_cast<GIZSPINNER_s *>(object->field_0x788);
            if ((spinner->state_flags & 0x40) != 0)
                adjustment = (spinner->state_flags & 1) != 0 ? 0x7d27 : 0x82d8;
        }
        object->apiobj.movement_facing_angle = angle + adjustment;
    }
}

// Original: 1,554 bytes.
f32 ForceTowardsMid(GameObject_s *object) {
    SOCKSYS *system = WorldInfo_CurrentlyActive()->sock_sys;
    if (system == NULL || object->sock_position.location.sock == -1 ||
        static_cast<u8>(object->sock_position.candidate_count) > 1)
        return 0.0f;
    SOCK *sock = &system->sock[object->sock_position.location.sock];
    f32 inner = sock->mid_force_inner_radius;
    f32 outer = sock->mid_force_outer_radius;
    if (sock->flags & 2) {
        if (object->field_0x1086 == 4 || !(inner > 0.0f) || !(outer > 0.0f))
            return 0.0f;
        f32 dy = object->sock_position.midpoint.y - object->apiobj.position.y;
        if (!(dy * dy >= inner * inner))
            return 0.0f;
        f32 amount = (fabsf(dy) - inner) / (outer - inner);
        object->target_velocity.y += (dy * object->apiobj.character_data->game_character->run_speed) * amount;
        return amount;
    }
    f32 amount = 0.0f;
    if (inner > 0.0f && outer > 0.0f) {
        i32 planar = sock->flags & 4;
        NUVEC delta;
        f32 distance_squared;
        if (planar != 0) {
            delta.x = object->sock_position.midpoint.x - object->apiobj.position.x;
            delta.y = 0.0f;
            delta.z = object->sock_position.midpoint.z - object->apiobj.position.z;
            distance_squared = delta.x * delta.x + delta.z * delta.z;
        } else {
            NuVecSub(&delta, &object->sock_position.midpoint, &object->apiobj.position);
            distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        }
        if (distance_squared >= inner * inner) {
            f32 ratio = (NuFsqrt(distance_squared) - inner) / (outer - inner);
            if (ratio > 3.0f)
                ratio = 3.0f;
            if (ratio >= 0.0f) {
                amount = ratio;
                if (planar == 0 && sock->unknown_7c != 1.0f) {
                    if (object->field_0x1086 == 4) {
                        NuVecInvMtxRotate(&delta, &delta, &object->vehicle_orientation);
                        delta.y *= 1.0f / sock->unknown_7c;
                        NuVecMtxRotate(&delta, &delta, &object->vehicle_orientation);
                    } else {
                        i32 pitch = RotDiff(0, object->sock_position.midpoint_rotation.x);
                        i32 yaw = RotDiff(0, object->sock_position.midpoint_rotation.y);
                        NuVecRotateY(&delta, &delta, -yaw);
                        NuVecRotateX(&delta, &delta, -pitch);
                        delta.y *= 1.0f / sock->unknown_7c;
                        NuVecRotateX(&delta, &delta, pitch);
                        NuVecRotateY(&delta, &delta, yaw);
                    }
                }
                f32 inverse = 1.0f / NuFsqrt(distance_squared);
                f32 speed = object->apiobj.character_data->game_character->run_speed * amount;
                delta.x *= inverse;
                if (planar == 0)
                    delta.y *= inverse;
                delta.z *= inverse;
                object->target_velocity.x += delta.x * speed;
                if (planar == 0)
                    object->target_velocity.y += delta.y * speed;
                object->target_velocity.z += delta.z * speed;
            }
        }
        if (planar == 0)
            return amount;
    }
    if ((sock->flags & 8) != 0 && object->apiobj.position.y >= object->sock_position.midpoint.y) {
        CHARACTERDATA *data = object->apiobj.character_data;
        f32 ceiling = object->sock_position.midpoint.y + data->field14_0x30 * data->field17_0x3c * 3.0f;
        f32 vertical = ceiling > object->apiobj.position.y
                           ? (object->apiobj.position.y - object->sock_position.midpoint.y) /
                                 (ceiling - object->sock_position.midpoint.y)
                           : 1.0f;
        object->target_velocity.y -= vertical * data->game_character->run_speed;
    }
    return amount;
}

void ResetPushProgress(WORLDINFO_s *world, void *progress_data) {
    if (world == NULL || world->push_blocks == NULL || world->push_block_count <= 0) {
        return;
    }

    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_data);
    const i32 count = world->push_block_count < 16 ? world->push_block_count : 16;
    for (i32 index = 0; index < count; ++index) {
        pushblock_s *block = &world->push_blocks[index];
        const u32 bit = 1u << index;
        if (progress == NULL) {
            continue;
        }

        block->flags_0cb = (block->flags_0cb & ~2u) | ((progress->state_mask & bit) != 0 ? 2u : 0u);
        const bool visible = (progress->visible_mask & bit) != 0;
        block->flags_0ca = (block->flags_0ca & ~4u) | (visible ? 4u : 0u);
        if (!visible) {
            NuSpecialSetVisibility(&block->special, 0);
            for (i32 output = 0; output < block->end_position_count; ++output) {
                NuSpecialSetVisibility(&block->end_position_specials[output], 0);
            }
        }

        if ((progress->position_mask & bit) == 0) {
            continue;
        }
        nuinstanim_s *animation = NuSpecialGetInstAnim(&block->special);
        if (!visible && animation != NULL) {
            NUMTX evaluated;
            EvalAnim(&block->special, 1.0f, &evaluated, 0);
            NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
            matrix->m30 = evaluated.m30;
            matrix->m31 = evaluated.m31;
            matrix->m32 = evaluated.m32;
            NuSpecialUpdate(&block->special);
            for (i32 output = 0; output < block->end_position_count; ++output) {
                nuhspecial_s *special = &block->end_position_specials[output];
                EvalAnim(special, 1.0f, &evaluated, 0);
                matrix = NuSpecialGetInstanceMtx(special);
                matrix->m30 = evaluated.m30;
                matrix->m31 = evaluated.m31;
                matrix->m32 = evaluated.m32;
                NuSpecialUpdate(special);
            }
        } else {
            NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
            matrix->m30 = progress->positions[index].x;
            matrix->m31 = progress->positions[index].y;
            matrix->m32 = progress->positions[index].z;
            NuSpecialUpdate(&block->special);
            for (i32 output = 0; output < block->end_position_count; ++output) {
                matrix = NuSpecialGetInstanceMtx(&block->end_position_specials[output]);
                const NUVEC &position = progress->end_positions[output][index];
                matrix->m30 = position.x;
                matrix->m31 = position.y;
                matrix->m32 = position.z;
                NuSpecialUpdate(&block->end_position_specials[output]);
            }
        }
    }
}

void SetObjAsHeadTarget(GameObject_s *, GameObject_s *, i8, f32, f32, f32);
void FastWeaponIn(GameObject_s *, i32);
void PlayGruntSfx(GameObject_s *);
i32 FaceOpponent(GameObject_s *, NUVEC *);

void FindForcePushTarget(GameObject_s *object, i32 activate, i32 target_filter) {
    if (object == NULL || object->apiobj.field_0x27d == 0 || object->character_context == 0x1b ||
        (object->field_0xe22 & 2) != 0 || (object->field_0xe23 & 1) != 0) {
        return;
    }

    GameObject_s *best = object->force_push_target;
    f32 best_distance = 100.0f;
    if (best == NULL) {
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
            GameObject_s *candidate = &Obj[index];
            if (candidate == object || (candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
                candidate->apiobj.field_0x287 != 0 || candidate->apiobj.model_draw_result == 0 ||
                candidate->apiobj.character_data == NULL || candidate->apiobj.character_data->game_character == NULL ||
                (candidate->apiobj.character_data->game_character->flags_090 & 0x80008000) != 0 ||
                (candidate->field_0xefc & 0x400010) != 0) {
                continue;
            }
            const bool candidate_is_player = candidate->apiobj.field_0x27c != -1;
            if ((target_filter == 1 && candidate_is_player) || (target_filter == 2 && !candidate_is_player)) {
                continue;
            }
            const f32 dx = candidate->apiobj.collision_position.x - object->apiobj.collision_position.x;
            const f32 dy = candidate->apiobj.collision_position.y - object->apiobj.collision_position.y;
            const f32 dz = candidate->apiobj.collision_position.z - object->apiobj.collision_position.z;
            const f32 distance = dx * dx + dy * dy + dz * dz;
            if (distance >= best_distance || dx * object->facing_direction.x + dz * object->facing_direction.z <= 0.0f) {
                continue;
            }
            best = candidate;
            best_distance = distance;
        }
    }
    if (best == NULL || best->character_context == 0x0f) {
        return;
    }

    object->field_0xe22 |= 2;
    object->force_push_target = best;
    object->force_glow_candidate = best;
    object->force_glow_candidate_kind = 2;
    SetObjAsHeadTarget(object, best, 2, 1.0f, 0.0f, 0.0f);
    if (activate == 0) {
        return;
    }

    object->character_context = 0x1b;
    object->force_target = best;
    object->field_0x7a3 = 0;
    object->context_animation_timer = 0.0f;
    object->airborne_action_duration = 0.3f;
    object->apiobj.movement_facing_angle =
        NuAtan2D(best->apiobj.collision_position.x - object->apiobj.collision_position.x,
                 best->apiobj.collision_position.z - object->apiobj.collision_position.z);
    FastWeaponIn(object, 0);
    PlaySfx(const_cast<char *>("JForcePush"), &object->apiobj.collision_position);
    PlayGruntSfx(object);

    if (best->apiobj.field_0x27c == -1) {
        best->force_target = object;
        best->character_context = 0x1c;
        best->context_animation = 0x2b;
        best->action_movement_state = 0;
        FastWeaponIn(best, 0);
        FaceOpponent(best, NULL);
    }
}

f32 PushingTowardsAngle(u16 input_angle, u16 direction) {
    NUVEC input, target;
    NuVecRotateY(&input, &v001, input_angle);
    NuVecRotateY(&target, &v001, direction);
    return input.x * target.x + input.z * target.z;
}

i32 CannotKill(GameObject_s *object);
extern i16 id_GONKDROID;

i32 Pushing(GameObject_s *object, u16 *normal_angle, i32 *surface, i32 *angle_difference) {
    i32 pushing_obstacle = 0;
    if (LEGOCONTEXT_PUSHOBSTACLE != -1 && LEGOCONTEXT_PUSHOBSTACLE == object->character_context)
        pushing_obstacle = 1;
    else if (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == object->character_context &&
             object->action_movement_state == 9)
        pushing_obstacle = 1;

    if ((static_cast<i8>(object->apiobj.flags_low) < 0 || (object->field_0xf02 & 3) != 0) &&
        (object->pad_gamepad->input_magnitude > 0.0f || pushing_obstacle != 0) && object->field_0x1084 != 0 &&
        CanClimbSurface(object, static_cast<i8>(object->field_0x6b0)) == 0 &&
        fabsf(object->contact_normal.y) < NuTrigTable[0x3c71]) {
        u16 input_angle = pushing_obstacle != 0 ? object->apiobj.movement_facing_angle
                                                : GamePad_InputAngle(object, object->pad_gamepad);
        u16 angle = NuAtan2D(object->contact_normal.x, object->contact_normal.z);
        i32 difference = RotDiff(input_angle, angle);
        if (angle_difference != NULL)
            *angle_difference = difference;
        if (difference < 0)
            difference = -difference;
        if (difference > 0x4e38 || (difference > 0x1555 && object->context_animation != -1 &&
                                    (object->context_animation == LEGOACT_WALLSHUFFLE_RIGHT ||
                                     object->context_animation == LEGOACT_WALLSHUFFLE_LEFT))) {
            if (normal_angle != NULL)
                *normal_angle = angle;
            if (surface != NULL)
                *surface = static_cast<i8>(object->field_0x6b0);
            return 1;
        }
    }
    if (surface != NULL)
        *surface = -1;
    return 0;
}

void PushAway(NUVEC *position, f32 radius, NUVEC *minimum, NUVEC *maximum, GameObject_s *object, GameObject_s *excluded,
              f32 speed_multiplier, u32 flags) {
    i32 count = 1;
    if (object == NULL) {
        object = Obj;
        count = HIGHGAMEOBJECT;
    }
    NUVEC bounds_min, bounds_max;
    if (maximum == NULL || minimum == NULL) {
        bounds_min.x = position->x - radius;
        bounds_min.y = position->y - radius;
        bounds_min.z = position->z - radius;
        bounds_max.x = position->x + radius;
        bounds_max.y = position->y + radius;
        bounds_max.z = position->z + radius;
        minimum = &bounds_min;
        maximum = &bounds_max;
    }
    for (i32 i = 0; i < count; ++i, ++object) {
        if (object == excluded || (object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            (object->field_0xe20 & 0x20) != 0)
            continue;
        if ((flags & 2) != 0 && object->apiobj.field_0x27d == 0)
            continue;
        if ((CInfo[object->character_context].flags & 0x40000000) != 0)
            continue;
        GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if ((character->flags_090 & 0x8000) != 0)
            continue;
        if (minimum->x > object->apiobj.collision_max.x || object->apiobj.collision_min.x > maximum->x ||
            minimum->z > object->apiobj.collision_max.z || object->apiobj.collision_min.z > maximum->z)
            continue;
        if ((flags & 1) != 0 &&
            (minimum->y > object->apiobj.collision_max.y || object->apiobj.collision_min.y > maximum->y))
            continue;
        f32 dx = object->apiobj.position.x - position->x;
        f32 dz = object->apiobj.position.z - position->z;
        f32 combined_radius = radius + object->apiobj.field_0x1dc;
        if (dx * dx + dz * dz < combined_radius * combined_radius) {
            u16 angle;
            if (dz == 0.0f && dx == 0.0f)
                angle = qrand();
            else
                angle = NuAtan2D(dx, dz);
            f32 speed = speed_multiplier *
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->run_speed;
            if (speed < 1.0f)
                speed = 1.0f;
            else if (speed > 3.0f)
                speed = 3.0f;
            f32 target_x = speed * NU_SIN_LUT(angle);
            f32 target_z = speed * NU_COS_LUT(angle);
            object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, target_x, 10.0f);
            object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, target_z, 10.0f);
        }
    }
}

void PushCode(GameObject_s *object, i32 allow_push) {
    if (object == NULL || VehicleArea != 0 || object->apiobj.field_0x27c == -1) {
        return;
    }

    u16 wall_angle = 0;
    i32 surface = -1;
    i32 angle_difference = 0;
    const bool against_wall = Pushing(object, &wall_angle, &surface, &angle_difference) != 0;
    if (against_wall) {
        wall_angle += 0x8000;
        object->field_0xdc4 += MAX(FRAMETIME, 1.0f / 30.0f);
        if (object->field_0xdc4 > 0.5f) {
            object->field_0xdc4 = 0.5f;
        }
        object->takeover_start_angle = wall_angle;
    } else {
        object->field_0xdc4 -= FRAMETIME;
        if (object->field_0xdc4 < 0.0f) {
            object->field_0xdc4 = 0.0f;
        }
    }

    const bool in_push_context =
        (LEGOCONTEXT_PUSH != -1 && object->character_context == LEGOCONTEXT_PUSH) ||
        (LEGOCONTEXT_PUSHOBSTACLE != -1 && object->character_context == LEGOCONTEXT_PUSHOBSTACLE) ||
        (LEGOCONTEXT_PUSHSPINNER != -1 && object->character_context == LEGOCONTEXT_PUSHSPINNER);
    if (!against_wall || allow_push == 0 || object->pad_gamepad == NULL ||
        object->pad_gamepad->input_magnitude <= 0.0f) {
        if (in_push_context && object->field_0xdc4 == 0.0f) {
            ReleasePush(object);
        }
        return;
    }

    if (!in_push_context && object->field_0xdc4 >= 0.2f) {
        object->character_context = LEGOCONTEXT_PUSH != -1 ? LEGOCONTEXT_PUSH : LEGOCONTEXT_PUSHOBSTACLE;
        object->context_animation = LEGOACT_PUSH;
        object->apiobj.movement_facing_angle = wall_angle;
        object->apiobj.facing_angle = wall_angle;
        FastWeaponIn(object, 0);
        PlayGruntSfx(object);
    }
    if (object->character_context == LEGOCONTEXT_PUSH) {
        object->target_velocity.x = 0.0f;
        object->target_velocity.z = 0.0f;
        object->apiobj.movement_facing_angle = wall_angle;
    }
}
