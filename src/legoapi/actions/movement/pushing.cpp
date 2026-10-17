#include "decomp.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/nufloat.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nutex.h"
#include <math.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void ReleasePush(GameObject_s *object) {
    if ((CInfo[object->character_context].flags & 0x2000) != 0) {
        if (LEGOCONTEXT_PUSHOBSTACLE != -1 && LEGOCONTEXT_PUSHOBSTACLE == object->character_context) {
            GameCam_Blend(NULL, 0.5f, 0.0f, 1);
        }
        object->field_0xdc4 = 0.0f;
        object->character_context = -1;
    }
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

f32 ForceTowardsMid(GameObject_s *object);

void ResetPushProgress(WORLDINFO_s *world, void *progress_data) {
    pushblock_s *block = world->push_blocks;
    if (block == NULL || world->push_block_count <= 0) {
        return;
    }

    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_data);
    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        if (index < 16 && progress != NULL) {
            const u32 bit = 1u << index;
            block->flags_0cb = (block->flags_0cb & ~2u) | (((progress->state_mask[0] & bit) != 0) << 1);
            block->push_visible = (progress->visible_mask[0] & bit) != 0;
            if (!block->push_visible) {
                NuSpecialSetVisibility(&block->special, 0);
                block->push_visible = 0;
                for (i32 output = 0; output < block->end_position_count; ++output) {
                    NuSpecialSetVisibility(&block->end_position_specials[output], 0);
                }
            }

            if ((progress->position_mask[0] & bit) == 0) {
                continue;
            }
            nuinstanim_s *animation = NuSpecialGetInstAnim(&block->special);
            if (block->push_visible || animation == NULL) {
                NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
                *NUMTX_GET_ROW_VEC(matrix, 3) = progress->positions[index];
                NuSpecialUpdate(&block->special);
                if (block->end_position_count != 0) {
                    i32 output = 0;
                    do {
                        matrix = NuSpecialGetInstanceMtx(&block->end_position_specials[output]);
                        *NUMTX_GET_ROW_VEC(matrix, 3) = progress->end_positions[output][index];
                        NuSpecialUpdate(&block->end_position_specials[output]);
                    } while (block->end_position_count > ++output);
                }
            } else {
                NUMTX evaluated;
                NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
                EvalAnim(&block->special, 1.0f, &evaluated, 0);
                *NUMTX_GET_ROW_VEC(matrix, 3) = *NUMTX_GET_ROW_VEC(&evaluated, 3);
                NuSpecialUpdate(&block->special);
                if (block->end_position_count != 0) {
                    i32 output = 0;
                    do {
                        nuhspecial_s *special = &block->end_position_specials[output];
                        matrix = NuSpecialGetInstanceMtx(special);
                        EvalAnim(special, 1.0f, &evaluated, 0);
                        *NUMTX_GET_ROW_VEC(matrix, 3) = *NUMTX_GET_ROW_VEC(&evaluated, 3);
                        NuSpecialUpdate(special);
                    } while (block->end_position_count > ++output);
                }
            }
        }
    }
}

void SetObjAsHeadTarget(GameObject_s *, GameObject_s *, i8, f32, f32, f32);
void FastWeaponIn(GameObject_s *, i32);
void PlayGruntSfx(GameObject_s *);
i32 FaceOpponent(GameObject_s *, NUVEC *);
void Player_ClearContext(GameObject_s *, i32);
void Player_ResetContexts(PLAYERPACKET_s *);
i32 SuperWeirdo(GameObject_s *);
void GameCam_HitRoll(void);
extern "C" i32 dagobah_training;
extern AREADATA *EMPERORFIGHT_ADATA;
extern i16 id_GAMORREANGUARD;

void FindForcePushTarget(GameObject_s *object, i32 activate, i32 target_filter);

f32 PushingTowardsAngle(u16 input_angle, u16 direction) {
    NUVEC input, target;
    NuVecRotateY(&input, &v001, input_angle);
    NuVecRotateY(&target, &v001, direction);
    return input.x * target.x + input.z * target.z;
}

i32 CannotKill(GameObject_s *object);
extern i16 id_GONKDROID;

i32 Pushing(GameObject_s *object, u16 *normal_angle, i32 *surface, i32 *angle_difference) {
    i32 pushing_obstacle;
    if (LEGOCONTEXT_PUSHOBSTACLE != -1 && LEGOCONTEXT_PUSHOBSTACLE == object->character_context)
        pushing_obstacle = 1;
    else if (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == object->character_context &&
             object->action_movement_state == 9)
        pushing_obstacle = 1;
    else
        pushing_obstacle = 0;

    if ((object->apiobj.player_controlled || (object->field_0xf02 & 3) != 0) &&
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

extern u32 (*CanPushObstaclesFn)(GameObject_s *);
extern GIZSPINNER_s *GizSpinner_Find(WORLDINFO_s *world, nuvec_s *position, i32 alternate);
extern i32 GizSpinner_Push(GIZSPINNER_s *spinner, i32 context);
extern void GizSpinner_PushFail(GameObject_s *object, GIZSPINNER_s *spinner);
i32 PushBlock(GameObject_s *object);
void StartLunge(GameObject_s *object, f32 speed, f32 height);
void PlayLandSfx(GameObject_s *object, i32 variant, i32 force);
void StartEndOfJump(GameObject_s *object);
void AlertSurroundingCreatures(GameObject_s *object, NUVEC *position);
extern "C" i32 ParticlesPerSecond(f32 rate, f32 elapsed);

void PushCode(GameObject_s *object, i32 allow_push) {
    if (VehicleArea != 0 || object->apiobj.field_0x27c == -1)
        return;
    const i8 saved_context = object->character_context;
    u16 wall_angle;
    i32 surface, angle_difference;
    const i32 pushing = Pushing(object, &wall_angle, &surface, &angle_difference) != 0;
    if (pushing) {
        wall_angle += 0x8000;
        if (static_cast<u8>(object->apiobj.field_0x281) < 32 &&
            (TerSurface[object->apiobj.field_0x281].flags & 0x20000) != 0) {
            object->field_0xdc4 = 0.4f;
        } else if (object->apiobj.field_0x27d == 0 && GameObjectNearFloor(object, 1.25f, NULL) == 0) {
            object->field_0xdc4 = 0.01f;
        } else {
            const f32 frame_time = FRAMETIME;
            if (frame_time > 1.0f / 30.0f) {
                object->field_0xdc4 += frame_time + frame_time;
            } else {
                const f32 step = (1.0f / frame_time) / 30.0f;
                object->field_0xdc4 += (step + step) * frame_time;
            }
            if (object->field_0xdc4 >= 0.25f)
                object->field_0xdc4 = 0.4f;
        }
    } else if (object->pad_gamepad->input_magnitude > 0.0f ||
               (LEGOCONTEXT_PUSHOBSTACLE != -1 && object->character_context == LEGOCONTEXT_PUSHOBSTACLE)) {
        object->field_0xdc4 -= FRAMETIME;
        if (object->field_0xdc4 < 0.0f)
            object->field_0xdc4 = 0.0f;
    } else {
        object->field_0xdc4 = 0.0f;
    }

    if ((CInfo[object->character_context].flags & 0x2000) == 0) {
        if ((object->apiobj.field_0x27d != 0 || GameObjectNearFloor(object, 1.25f, NULL) != 0) &&
            object->character_context == -1 &&
            (allow_push == 0 ||
             (LEGOACT_PUSH != -1 && object->apiobj.character_model->model_data_b[LEGOACT_PUSH] != NULL))) {
            if (!(object->field_0xdc4 >= 0.25f))
                goto maintain;
            if (__builtin_expect((static_cast<u32>(surface) - 30) < 2 || (object->field_0xf02 & 2) != 0, 0)) {
                GIZSPINNER_s *spinner = NULL;
                if (__builtin_expect(LEGOCONTEXT_PUSHSPINNER != -1, 0))
                    spinner = GizSpinner_Find(WORLD, &object->apiobj.collision_position, 1);
                if (spinner != NULL) {
                    if (((spinner->flags & 8) == 0 || ShadowMode != 0) && spinner->room_index == -1 &&
                        spinner->field_0x090 <= 0.0f) {
                        // A spinner can only be attached to one party member.
                        bool occupied = false;
                        for (i32 player_index = 0; player_index < 8; ++player_index) {
                            if (Player[player_index] != NULL && Player[player_index] != object &&
                                Player[player_index]->character_context == LEGOCONTEXT_PUSHSPINNER &&
                                Player[player_index]->field_0x788 == spinner) {
                                occupied = true;
                                break;
                            }
                        }
                        if (!occupied) {
                            object->field_0x788 = spinner;
                            object->field_0x7a6 = static_cast<u8>(surface);
                            object->character_context = LEGOCONTEXT_PUSHSPINNER;
                            FastWeaponIn(object, 0);
                            AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                        }
                    }
                } else if (LEGOCONTEXT_PUSHOBSTACLE != -1 && CanPushObstaclesFn != NULL &&
                           CanPushObstaclesFn(object) != 0) {
                    f32 distance;
                    GIZOBSTACLE_s *obstacle = GizObstacle_FindNearest(
                        WORLD->giz_obstacle_sys, &object->apiobj.collision_position, object, &distance, 7);
                    if (obstacle != NULL && distance < 6.25f) {
                        object->field_0x758 = 0.75f;
                        object->field_0x788 = obstacle;
                        object->field_0x7a6 = static_cast<u8>(surface);
                        object->character_context = LEGOCONTEXT_PUSHOBSTACLE;
                        FastWeaponIn(object, 0);
                        GameCam_Blend(NULL, 0.5f, 0.0f, 1);
                        AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                    }
                }
            } else {
                object->character_context = LEGOCONTEXT_PUSH;
            }
            if (object->character_context != -1) {
                object->context_animation = LEGOACT_PUSH;
                SetPushAngle(object);
            }
            goto maintain;
        }
        {
            const bool jump_nine = LEGOCONTEXT_JUMP != -1 && object->character_context == LEGOCONTEXT_JUMP &&
                                   object->action_movement_state == 9;
            GAMECHARACTERDATA_s *character = object->apiobj.character_data->game_character;
            if ((character->flags_090 & 0x8000000) == 0 || LEGOCONTEXT_WALLJUMPWAIT == -1 ||
                object->apiobj.supporting_platform_id != -1)
                goto maintain;
            if (__builtin_expect(static_cast<u32>(surface) <= 31 && (TerSurface[surface].flags & 0x10581) != 0, 0))
                goto maintain;
            if (object->pad_gamepad->input_magnitude != character->run_speed && !jump_nine)
                goto maintain;
            if (!pushing)
                goto maintain;
            i32 difference = angle_difference < 0 ? -angle_difference : angle_difference;
            if (difference <= 0x6aaa ||
                !(object->apiobj.horizontal_velocity_magnitude > (character->run_speed + character->field_0x18) * 0.5f))
                goto maintain;
            if (object->apiobj.field_0x27d != 0 ||
                !(object->apiobj.collision_min.y - object->apiobj.field_0x218 > 0.15f))
                goto maintain;
            if (LEGOCONTEXT_JUMP == -1 || object->character_context != LEGOCONTEXT_JUMP)
                goto maintain;
            if (object->action_movement_state == 0) {
                if (object->jump_sequence > 1)
                    goto maintain;
            } else if (object->action_movement_state != 9)
                goto maintain;
            if (!(object->apiobj.velocity.y > -1.25f) || !(object->context_animation_timer >= 0.25f))
                goto maintain;
            object->character_context = LEGOCONTEXT_WALLJUMPWAIT;
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
            object->context_animation = LEGOACT_WALLJUMP_WAIT;
            f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
            object->landing_followup = 0;
            object->context_animation_timer = duration <= 0.0f ? 0.3f : duration;
            object->apiobj.movement_facing_angle = wall_angle;
            object->apiobj.velocity = v000;
        }
    } else if (LEGOCONTEXT_WALLJUMPWAIT != -1 && object->character_context == LEGOCONTEXT_WALLJUMPWAIT) {
        if (__builtin_expect((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0, 0))
            object->landing_followup = 1;
        else if ((object->pad_gamepad->buttons_pressed & GAMEPAD_ACTION) != 0)
            object->landing_followup = 3;
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            goto maintain;
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            if (object->landing_followup == 1) {
                StartJump(object, 9);
                object->airborne_action_timer = 1.2f;
                object->apiobj.movement_facing_angle += 0x8000;
                object->apiobj.facing_angle = object->apiobj.movement_facing_angle;
                object->apiobj.field_0x276 = object->apiobj.movement_facing_angle;
                PlayGruntSfx(object);
            } else if (object->landing_followup == 3 && LEGOACT_LUNGE != -1 &&
                       object->apiobj.character_model->model_data_b[LEGOACT_LUNGE] != NULL) {
                object->apiobj.movement_facing_angle += 0x8000;
                object->apiobj.facing_angle = object->apiobj.movement_facing_angle;
                object->apiobj.field_0x276 = object->apiobj.movement_facing_angle;
                StartLunge(object, 1.0f, 0.0f);
                PlayGruntSfx(object);
            } else
                StartEndOfJump(object);
        }
    } else {
        if (__builtin_expect(!!(pushing), 0))
            object->takeover_start_angle = wall_angle;
        if (object->field_0xdc4 <= 0.0f) {
            if (LEGOCONTEXT_PUSHOBSTACLE != -1 && object->character_context == LEGOCONTEXT_PUSHOBSTACLE)
                GameCam_Blend(NULL, 0.5f, 0.0f, 1);
            object->character_context = -1;
        }
    }
maintain:
    if (LEGOCONTEXT_PUSHOBSTACLE != -1 && object->character_context == LEGOCONTEXT_PUSHOBSTACLE) {
        GAMEPAD_s *pad = object->pad_gamepad;
        if ((pad->buttons_pressed & GAMEPAD_JUMP) != 0) {
            StartJump(object, 0);
            object->movement_runtime_flags |= 0x10;
            object->field_0xdc4 = 0.0f;
            return;
        }
        if (!(pad->input_magnitude > 0.0f)) {
            object->context_animation = LEGOACT_SUPERPUSH_IDLE;
            object->field_0x758 -= FRAMETIME;
            if (object->field_0x758 <= 0.0f) {
                GameCam_Blend(NULL, 0.5f, 0.0f, 1);
                object->character_context = -1;
                return;
            }
        } else {
            i32 difference = RotDiff(GamePad_InputAngle(object, pad), object->takeover_start_angle);
            if (difference < 0)
                difference = -difference;
            bool forward;
            if (difference < 0x31c7) {
                object->field_0x758 = 0.75f;
                object->context_animation = LEGOACT_SUPERPUSH_PUSH;
                forward = object->field_0x7a6 == 0x1e;
            } else if (difference < 0x4e39) {
                object->context_animation = LEGOACT_SUPERPUSH_IDLE;
                goto finalize;
            } else {
                object->field_0x758 = 0.75f;
                object->context_animation = LEGOACT_SUPERPUSH_PULL;
                forward = object->field_0x7a6 != 0x1e;
            }
            GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(object->field_0x788);
            bool stopped;
            if (forward) {
                stopped = obstacle->anim_set->state == 2;
                GizObstacle_SetPushControlled(obstacle, object, 1.0f);
            } else {
                stopped = obstacle->anim_set->state == 0;
                GizObstacle_SetPushControlled(obstacle, object, -1.0f);
            }
            if (stopped) {
                if (ParticlesPerSecond(3.0f, FRAMETIME) > 0)
                    NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
            } else {
                f32 strength = QRAND_FLOAT() * 0.3f;
                NewRumble(object->pad_gamepad->pad, strength, 0);
            }
        }
    } else if (LEGOCONTEXT_PUSHSPINNER != -1 && object->character_context == LEGOCONTEXT_PUSHSPINNER) {
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0) {
            StartJump(object, 0);
            object->field_0xdc4 = 0.0f;
            object->movement_runtime_flags |= 0x10;
            return;
        }
        if (GizSpinner_Push(static_cast<GIZSPINNER_s *>(object->field_0x788), static_cast<i8>(object->field_0x7a6)) ==
            0) {
            if (qrand() < 0x800)
                GizSpinner_PushFail(object, static_cast<GIZSPINNER_s *>(object->field_0x788));
            else
                static_cast<GIZSPINNER_s *>(object->field_0x788)->state_flags &= ~0x300u;
        } else {
            if (object->apiobj.player_controlled) {
                f32 strength = QRAND_FLOAT() * 0.4f;
                NewRumble(object->pad_gamepad->pad, strength, 0);
            }
            GameAudio_PlaySfx(0x38, &object->apiobj.collision_position, 0, 0);
        }
    } else if (LEGOCONTEXT_PUSH != -1 && object->character_context == LEGOCONTEXT_PUSH) {
        if (PushBlock(object) == 0) {
            // Pushing can leave the angle unwritten, so test shuffle availability first.
            if (LEGOCONTEXT_WALLSHUFFLE == -1 || LEGOACT_WALLSHUFFLE_IDLE == -1 ||
                object->apiobj.character_model->model_data_b[LEGOACT_WALLSHUFFLE_IDLE] == NULL ||
                (angle_difference < 0 ? -angle_difference : angle_difference) < 0x71c8)
                object->character_context = -1;
            else {
                object->character_context = LEGOCONTEXT_WALLSHUFFLE;
                object->context_animation = LEGOACT_WALLSHUFFLE_IDLE;
            }
        }
    }
finalize:
    if (saved_context != object->character_context && (CInfo[object->character_context].flags & 0x2000) != 0) {
        FastWeaponIn(object, 0);
        if (LEGOCONTEXT_WALLJUMPWAIT != -1 && object->character_context == LEGOCONTEXT_WALLJUMPWAIT)
            PlayLandSfx(object, 0, 0);
        else
            PlayGruntSfx(object);
        if (__builtin_expect(!!(pushing), 0))
            object->takeover_start_angle = wall_angle;
    }
}
