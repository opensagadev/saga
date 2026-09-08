#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/gizmos/transport/tightropes.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/world/world.h"
#include <math.h>
#include "globals.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/input/gamepads.h"

DECOMP_ASSERT(offsetof(WORLDINFO_s, tightropes) == 0x505c, "World tightrope pointer offset");
DECOMP_ASSERT(offsetof(WORLDINFO_s, tightrope_count) == 0x5060, "World tightrope count offset");

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
void GameObjectOrigin(GameObject_s *);

static i32 TightRope_Attach(GameObject_s *object, WORLDINFO_s *world) {
    NUVEC target;
    TIGHTROPE *rope = TightRope_InRange(object, world, &target);
    if (rope == NULL)
        return 0;
    object->character_context = 0x44;
    object->field_0x788 = rope;
    object->context_variant_flags = (object->context_variant_flags & ~8) |
                                    ((object->apiobj.character_data->game_character->flags_090 & 0x80000) == 0 ? 8 : 0);
    if (object->apiobj.character_model->model_data_b[0x8f] != NULL &&
        ((object->apiobj.character_data->game_character->flags_090 & 0x80000) == 0 ||
         target.y > (object->apiobj.upper_position.y - object->apiobj.lower_position.y) * 0.25f +
                        object->apiobj.lower_position.y)) {
        object->context_animation = 0x8f;
        object->context_animation_timer = AnimDuration(object->id, 0x8f, 0.0f, 0.0f, 1);
    } else {
        object->context_animation = 0x88;
    }
    object->launch_origin = target;
    i32 difference =
        RotDiff(static_cast<TIGHTROPE *>(object->field_0x788)->y_rotation, object->apiobj.movement_facing_angle);
    if (difference < 0)
        difference = -difference;
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    object->apiobj.movement_facing_angle = rope->y_rotation;
    if (difference > 0x4000)
        object->apiobj.movement_facing_angle += 0x8000;
    object->field_0x768 = NuVecXZDist(&object->apiobj.collision_position, &rope->start_position, NULL);
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    const f32 horizontal = NuFsqrt(rope->direction.x * rope->direction.x + rope->direction.z * rope->direction.z);
    const i32 pitch = NuAtan2D(static_cast<TIGHTROPE *>(object->field_0x788)->direction.y, horizontal);
    NuVecRotateX(&target, &v010, -pitch);
    NuVecRotateY(&target, &target, static_cast<TIGHTROPE *>(object->field_0x788)->y_rotation);
    FindAnglesZX(&target, &object->magnet_surface_angle, &object->grapple_swing_phase);
    if ((object->apiobj.character_data->game_character->flags_090 & 0x10000000) != 0) {
        target = {0.0f, object->apiobj.upper_position.y - object->apiobj.lower_position.y, 0.0f};
        NuVecRotateZ(&object->zipup_swing_position, &target, object->grapple_swing_phase);
        NuVecRotateX(&object->zipup_swing_position, &object->zipup_swing_position, object->magnet_surface_angle);
        NuVecSub(&object->zipup_swing_position, &target, &object->zipup_swing_position);
    } else {
        object->zipup_swing_position = v000;
    }
    return 1;
}

i32 TightRope_SnapTo(GameObject_s *object, nuvec_s *position) {
    const f32 height = object->apiobj.field_0x1e0;
    object->apiobj.lower_position = *position;
    object->apiobj.upper_position = object->apiobj.lower_position;
    object->apiobj.collision_position = object->apiobj.upper_position;
    object->apiobj.upper_position.y += height;
    object->apiobj.lower_position.y -= height;
    if (TightRope_Attach(object, WORLD) == 0)
        return 0;
    object->apiobj.position.x = object->apiobj.collision_position.x;
    object->apiobj.position.z = object->apiobj.collision_position.z;
    const f32 bound = (object->apiobj.character_data->game_character->flags_090 & 0x80000) != 0
                          ? object->character_bottom
                          : object->character_top;
    object->apiobj.position.y = object->launch_origin.y - bound * object->apiobj.field_0xa8;
    const u8 saved_flag = object->field_0xe24 & 8;
    object->field_0xe24 &= ~8;
    GameObjectOrigin(object);
    object->field_0xe24 = (object->field_0xe24 & ~8) | saved_flag;
    return 1;
}

TIGHTROPE *TightRope_InRange(GameObject_s *object, WORLDINFO_s *world, nuvec_s *target) {
    const f32 range = 3.0f * object->apiobj.field_0x1dc;
    const NUVEC position = object->apiobj.collision_position;
    TIGHTROPE *rope = world->tightropes;
    for (i32 index = 0; index < world->tightrope_count; ++index, ++rope) {
        if (rope->visible == 0 || rope->active == 0)
            continue;
        NUVEC local = {position.x - rope->start_position.x, 0.0f, position.z - rope->start_position.z};
        NuVecRotateY(&local, &local, -static_cast<i32>(rope->y_rotation));
        if (!(local.z >= 0.0f && local.z <= rope->length && local.x >= -range && local.x <= range))
            continue;
        local.x = 0.0f;
        local.y = (rope->end_position.y - rope->start_position.y) * (local.z / rope->length) + rope->start_position.y;
        if (!(fabsf(local.y - position.y) < object->apiobj.field_0x1e0))
            continue;
        if (target != NULL) {
            const f32 inset = (object->apiobj.character_data->game_character->flags_090 & 0x10000000) != 0
                                  ? object->apiobj.field_0x1e0
                                  : object->apiobj.field_0x1dc;
            if (local.z > rope->length - inset)
                local.z = rope->length - inset;
            else if (local.z < inset)
                local.z = inset;
            NuVecRotateY(target, &local, rope->y_rotation);
            target->x += rope->start_position.x;
            target->z += rope->start_position.z;
        }
        return rope;
    }
    return NULL;
}

static i32 TightRope_MoveUpdate(GameObject_s *object, i32 airborne) {
    if (!(object->pad_gamepad->input_magnitude > 0.0f) || object->context_animation == 0x8f) {
        if (airborne == 0)
            object->context_animation = 0x88;
        return 1;
    }
    const NUVEC previous = object->launch_origin;
    const u16 yaw = static_cast<TIGHTROPE *>(object->field_0x788)->y_rotation;
    const f32 pushing = PushingTowardsAngle(GamePad_InputAngle(object, object->pad_gamepad), yaw);
    f32 sign;
    if (pushing > NuTrigTable[0x3000])
        sign = 1.0f;
    else if (pushing < -NuTrigTable[0x3000])
        sign = -1.0f;
    else {
        if (airborne == 0)
            object->context_animation = 0x88;
        return 1;
    }
    f32 speed = 0.6f;
    if (airborne == 0) {
        object->context_animation = 0x89;
        object->apiobj.movement_facing_angle = static_cast<TIGHTROPE *>(object->field_0x788)->y_rotation;
        if (sign < 0.0f)
            object->apiobj.movement_facing_angle += 0x8000;
        speed = AnimSpeed(object->apiobj.character_model, 0x89);
    }
    NUVEC step;
    NuVecScale(&step, &static_cast<TIGHTROPE *>(object->field_0x788)->direction, speed * sign * FRAMETIME);
    NuVecAdd(&object->launch_origin, &object->launch_origin, &step);
    TIGHTROPE *rope = static_cast<TIGHTROPE *>(object->field_0x788);
    NuVecSub(&object->launch_origin, &object->launch_origin, &rope->start_position);
    object->launch_origin.z = NuVecDot(&object->launch_origin, &rope->direction);
    const f32 inset = (object->apiobj.character_data->game_character->flags_090 & 0x10000000) != 0
                          ? object->apiobj.field_0x1e0
                          : object->apiobj.field_0x1dc;
    i32 result = 1;
    if (object->launch_origin.z > rope->length - inset) {
        object->launch_origin.z = rope->length - inset;
        result = 0;
    } else if (object->launch_origin.z < inset) {
        object->launch_origin.z = inset;
        result = 0;
    }
    NuVecScale(&object->launch_origin, &rope->direction, object->launch_origin.z);
    NuVecAdd(&object->launch_origin, &object->launch_origin, &rope->start_position);
    if (airborne == 0 && previous.x == object->launch_origin.x && previous.y == object->launch_origin.y &&
        previous.z == object->launch_origin.z)
        object->context_animation = 0x88;
    return result;
}

void StartJump(GameObject_s *, i32);
void StartEndOfJump(GameObject_s *);
i32 StartFallLand(GameObject_s *, i32);

void TightRope_MoveCode(GameObject_s *object, i32 jump) {
    if (object->character_context != 0x44) {
        if (object->apiobj.field_0x27d != 0 || !(object->apiobj.velocity.y <= 0.0f) ||
            object->apiobj.character_model->model_data_b[0x88] == NULL)
            return;
        if (object->character_context != -1 &&
            (object->character_context != 0 || !(object->context_animation_timer >= 0.1f) ||
             object->action_movement_state == 3 || object->action_movement_state == 4 ||
             object->action_movement_state == 8))
            return;
        if ((object->apiobj.flags_low & 0x80) == 0 && (object->field_0xf01 & 0x40) == 0)
            return;
        TightRope_Attach(object, WORLD);
        object->build_button_taps = 0;
        object->external_force.z = 0.0f;
        object->external_force.y = 0.0f;
        return;
    }
    if (object->context_animation == 0x8f) {
        if (jump != 0)
            goto rope_jump;
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, 0x8f, 1, 0);
        if (frame != NULL) {
            const f32 input_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            bool move = input_frame >= 1.0f && *frame >= input_frame && object->pad_gamepad->input_magnitude > 0.0f;
            if (move) {
                const u16 yaw = static_cast<TIGHTROPE *>(object->field_0x788)->y_rotation;
                move = fabsf(PushingTowardsAngle(GamePad_InputAngle(object, object->pad_gamepad), yaw)) >
                       NuTrigTable[0x3000];
            }
            if (move)
                TightRope_MoveUpdate(object, 0);
            else {
                object->context_animation_timer -= FRAMETIME;
                if (object->context_animation_timer <= 0.0f)
                    object->context_animation = 0x88;
            }
        }
        goto rope_distance;
    }
    object->external_force.y += FRAMETIME;
    if (static_cast<u16>(object->context_animation - 5) <= 1) {
        object->context_animation_timer += FRAMETIME;
        TIGHTROPE *previous_rope = static_cast<TIGHTROPE *>(object->field_0x788);
        if (object->context_animation == 5) {
            if (object->context_animation_timer >= object->airborne_action_duration + object->airborne_action_duration)
                StartEndOfJump(object);
        } else if (object->context_animation_timer >= object->airborne_action_duration) {
            object->context_animation = 5;
        }
        if (object->apiobj.velocity.y <= 0.0f && TightRope_Attach(object, WORLD) == 0 &&
            object->apiobj.field_0x27d != 0) {
            object->movement_runtime_flags |= 4;
            if (object->context_animation == 5 && StartFallLand(object, -1) != 0) {
                object->movement_runtime_flags &= ~4;
            } else {
                if (object->apiobj.character_model->model_data_b[7] != NULL &&
                    object->pad_gamepad->input_magnitude == 0.0f) {
                    object->character_context = 1;
                    object->context_animation = 7;
                    object->context_animation_timer = AnimDuration(object->id, 7, 0.0f, 0.0f, 1);
                    object->jump_reentry_timer = 0.2f;
                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                } else {
                    object->character_context = -1;
                }
                object->movement_runtime_flags &= ~4;
            }
        }
        if (object->character_context != 0x44 || object->field_0x788 != previous_rope)
            return;
        TightRope_MoveUpdate(object, 1);
        goto rope_distance;
    }
    if (jump != 0)
        goto rope_jump;
    if (TightRope_MoveUpdate(object, 0) == 0 && object->context_animation != 0x8f && object->external_force.y >= 0.5f) {
        StartJump(object, 0);
        object->apiobj.velocity.y =
            (object->apiobj.character_data->game_character->flags_090 & 0x80000) != 0 ? 1.2f : 1.8f;
    }
rope_distance:
    object->field_0x768 = NuVecXZDist(&object->apiobj.collision_position,
                                      &static_cast<TIGHTROPE *>(object->field_0x788)->start_position, NULL);
    return;
rope_jump:
    if ((object->apiobj.flags_low & 0x80) != 0) {
        object->apiobj.velocity.y = 2.0f;
        object->context_animation = 6;
        object->context_animation_timer = 0.0f;
        object->airborne_action_duration = AnimDuration(object->id, 6, 0.0f, 0.0f, 0);
        if (object->airborne_action_duration <= 0.0f)
            object->airborne_action_duration = 1.0f;
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
    } else {
        StartJump(object, 0);
        object->movement_runtime_flags |= 0x10;
    }
}

TIGHTROPE *TightRope_FindNearest(nuvec_s *position, WORLDINFO_s *world, i32 *endpoint, float *distance_squared) {
    TIGHTROPE *nearest = NULL;
    i32 nearest_endpoint = -1;
    f32 nearest_distance = 1.0e9f;
    TIGHTROPE *rope = world->tightropes;
    for (i32 index = 0; index < world->tightrope_count; ++index, ++rope) {
        f32 distance = NuVecDistSqr(position, &rope->start_position, NULL);
        if (distance < nearest_distance) {
            nearest = rope;
            nearest_endpoint = 0;
            nearest_distance = distance;
        }
        distance = NuVecDistSqr(position, &rope->end_position, NULL);
        if (distance < nearest_distance) {
            nearest = rope;
            nearest_endpoint = 1;
            nearest_distance = distance;
        }
    }
    if (endpoint != NULL)
        *endpoint = nearest_endpoint;
    if (distance_squared != NULL)
        *distance_squared = nearest_distance;
    return nearest;
}

// Original 0x1ccf00, 441 bytes.
i32 TightRope_SetTargetMom(GameObject_s *object) {
    NUVEC *position = (object->apiobj.character_data->game_character->flags_090 & 0x80000) != 0
                          ? &object->apiobj.lower_position
                          : &object->apiobj.upper_position;
    object->target_velocity.x = (object->launch_origin.x - position->x) * 3.0f;
    object->target_velocity.y = (object->launch_origin.y - position->y) * 3.0f;
    object->target_velocity.z = (object->launch_origin.z - position->z) * 3.0f;
    TIGHTROPE *rope = static_cast<TIGHTROPE *>(object->field_0x788);
    u16 angle = rope->y_rotation + 0x4000;
    f32 fraction = object->field_0x768 / rope->length;
    if (1.0f < fraction)
        fraction = 1.0f;
    i32 phase = static_cast<i32>(fraction * 32768.0f);
    f32 amplitude = 0.5f * NuTrigTable[(phase >> 1) & 0x7fff];
    f32 lateral = NU_SIN_LUT(angle);
    object->target_velocity.x += (qrand() * (1.0f / 65535.0f) - 0.5f) * amplitude * lateral;
    object->target_velocity.y -= qrand() * (1.0f / 65535.0f) * amplitude;
    lateral = NU_COS_LUT(angle);
    object->target_velocity.z += (qrand() * (1.0f / 65535.0f) - 0.5f) * amplitude * lateral;
    return static_cast<u16>(object->context_animation - 5) > 1;
}
