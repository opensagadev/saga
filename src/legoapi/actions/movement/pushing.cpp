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
    const i32 context = object->character_context;
    if ((CInfo[context].flags & 0x2000) != 0) {
        if (LEGOCONTEXT_PUSHOBSTACLE != -1 && LEGOCONTEXT_PUSHOBSTACLE == context) {
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

// Original: 1,554 bytes.
f32 ForceTowardsMid(GameObject_s *object) {
    SOCKSYS *system = WorldInfo_CurrentlyActive()->sock_sys;
    if (system == NULL || object->sock_position.location.sock == -1)
        return 0.0f;
    SOCK *socks = system->sock;
    f32 amount = 0.0f;
    if (static_cast<u8>(object->sock_position.candidate_count) > 1)
        return amount;
    SOCK *sock = &socks[object->sock_position.location.sock];
    f32 inner = sock->mid_force_inner_radius;
    f32 outer = sock->mid_force_outer_radius;
    if (sock->flags & 2) {
        if (object->field_0x1086 != 4 && inner > 0.0f && outer > 0.0f) {
            f32 dy = object->sock_position.midpoint.y - object->apiobj.position.y;
            if (dy * dy >= inner * inner) {
                f32 ratio = (NuFabs(dy) - inner) / (outer - inner);
                object->target_velocity.y += (dy * object->apiobj.character_data->game_character->run_speed) * ratio;
                amount = ratio;
            }
        }
        return amount;
    }
    if (inner > 0.0f && outer > 0.0f) {
        i32 planar = sock->flags & 4;
        f32 inner_squared = inner * inner;
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
        if (distance_squared >= inner_squared) {
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
    pushblock_s *block = world->push_blocks;
    if (block == NULL || world->push_block_count <= 0) {
        return;
    }

    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_data);
    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        if (index < 16 && progress != NULL) {
            const u32 bit = 1u << index;
            block->flags_0cb = (block->flags_0cb & ~2u) | (((progress->state_mask & bit) != 0) << 1);
            block->push_visible = (progress->visible_mask & bit) != 0;
            if (!block->push_visible) {
                NuSpecialSetVisibility(&block->special, 0);
                block->push_visible = 0;
                for (i32 output = 0; output < block->end_position_count; ++output) {
                    NuSpecialSetVisibility(&block->end_position_specials[output], 0);
                }
            }

            if ((progress->position_mask & bit) == 0) {
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
extern i32 dagobah_training;
extern AREADATA *EMPERORFIGHT_ADATA;
extern i16 id_GAMORREANGUARD;

void FindForcePushTarget(GameObject_s *object, i32 activate, i32 target_filter) {
    if (object->character_context == 0x1b || (object->field_0xe22 & 2) != 0 ||
        (object->apiobj.field_0x27c != -1 && MiniCutCam != 0) || (object->field_0xe23 & 1) != 0) {
        return;
    }

    if ((object->pad_gamepad->allocated_5a & 0x10) != 0) {
        activate = 1;
    } else if (!object->apiobj.player_controlled) {
        return;
    }
    if (object->apiobj.field_0x27d == 0) {
        return;
    }
    if (object->character_context != -1 && !objInNetWaitContext(object, 0x1b) &&
        (CInfo[object->character_context].flags & 4) == 0 && object->character_context != 6 &&
        object->character_context != 7) {
        return;
    }

    i32 choke_style = object->apiobj.character_data->game_character->flags_090 & 2;
    i32 second_style = object->apiobj.character_data->game_character->flags_090 & 4;
    GameObject_s *best = NULL;
    i32 selected_choke;
    i32 selected_second;
    i32 selected_direct;
    if ((object->pad_gamepad->allocated_5a & 0x10) != 0 && object->force_push_target != NULL) {
        best = object->force_push_target;
        if (choke_style == 0 && object->apiobj.character_data->game_character->uses_weapon_action == 0x0c &&
            (object->apiobj.character_data->model_flags & 8) != 0) {
            choke_style = best->id == id_GAMORREANGUARD;
        }
        i32 target_special = best->apiobj.character_data->model_flags & 0x10;
        i32 continuing_direct = 0;
        if (target_special == 0 && (best->apiobj.character_data->game_character->flags_090 & 0x40) != 0 &&
            best->apiobj.character_model->model_data_b[0x41] != NULL) {
            continuing_direct = 1;
        } else if ((choke_style | second_style) != 0) {
            if (second_style != 0) {
                if (best->apiobj.character_model->model_data_b[0x54] == NULL) {
                    if (target_special == 0)
                        return;
                    choke_style = 0;
                    second_style = 0;
                }
            } else if (best->apiobj.character_model->model_data_b[0x53] == NULL) {
                if (target_special == 0)
                    return;
                choke_style = 0;
            }
        } else if (target_special == 0) {
            if (best->apiobj.character_model->model_data_b[0x41] != NULL) {
                continuing_direct = 1;
            } else if (best->apiobj.character_model->model_data_b[0x2b] == NULL) {
                return;
            }
        } else if (best->apiobj.character_model->model_data_b[0x2b] == NULL &&
                   best->apiobj.character_model->model_data_b[5] == NULL) {
            return;
        }
        selected_choke = choke_style;
        selected_second = second_style;
        selected_direct = continuing_direct;
    } else {
        i32 super_weirdo = SuperWeirdo(object);
        if (super_weirdo != 0) {
            i32 random = qrand();
            second_style = random > 0x7fff;
            choke_style = random <= 0x7fff;
        }

        i32 object_count = HIGHGAMEOBJECT;
        f32 best_distance = 1.5625f;
        GameObject_s *candidate = Obj;
        selected_choke = 0;
        selected_second = 0;
        selected_direct = 0;
        for (i32 index = 0; index < object_count; ++index, ++candidate) {
            if (candidate->apiobj.in_use == 0 || candidate == object || candidate->apiobj.character == 0 ||
                candidate->apiobj.field_0x287 != 0 || candidate->apiobj.model_draw_result == 0 ||
                candidate->character_context == 0x3c || candidate->character_context == 0x39 ||
                candidate->character_context == 0x3b || candidate->character_context == 0x41 ||
                candidate->character_context == 0x17 || candidate->character_context == 0x0f ||
                candidate->character_context == 0x47 || candidate->character_context == 0x46 ||
                (candidate->apiobj.character_data->game_character->flags_090 & 0x8000) != 0 ||
                (CInfo[candidate->character_context].flags & 0x8000) != 0 ||
                (candidate->apiobj.character_data->game_character->flags_094[1] & 2) != 0 ||
                (candidate->field_0xefc_word & 0x400010) != 0) {
                continue;
            }
            if (TouchHacks::CanForceTargetObj(*object, *candidate) &&
                !(WORLD->area != NULL && WORLD->area == EMPERORFIGHT_ADATA &&
                  ((candidate->field_0xefb & 8) != 0 || candidate->apiobj.field_0x27c != -1)) &&
                candidate->id != id_BODYGUARD) {
                if (target_filter == 1) {
                    if (candidate->apiobj.field_0x27c != -1) {
                        goto reload_object_count;
                    }
                    if ((candidate->apiobj.field_0x1f4 & 5) != 0) {
                        const u32 *mask = WORLD->api_object_sys->hostility_masks[object->apiobj.field_0x289];
                        const u64 hostility = static_cast<u64>(mask[0]) | (static_cast<u64>(mask[1]) << 32);
                        if (((hostility >> (candidate->apiobj.field_0x289 & 63)) & 1) == 0) {
                            goto reload_object_count;
                        }
                    }
                } else if (target_filter == 2 && candidate->apiobj.field_0x27c == -1) {
                    goto reload_object_count;
                }
                if ((candidate->field_0xefb & 8) != 0) {
                    goto reload_object_count;
                }

                i32 candidate_choke = choke_style;
                i32 candidate_second = second_style;
                i32 candidate_direct = 0;
                if (candidate_choke == 0 && object->apiobj.character_data->game_character->uses_weapon_action == 0x0c &&
                    (object->apiobj.character_data->model_flags & 8) != 0) {
                    candidate_choke = candidate->id == id_GAMORREANGUARD;
                }
                if ((candidate->apiobj.character_data->model_flags & 0x10) == 0 &&
                    (candidate->apiobj.character_data->game_character->flags_090 & 0x40) != 0 &&
                    candidate->apiobj.character_model->model_data_b[0x41] != NULL) {
                    candidate_direct = 1;
                } else if ((candidate_choke | second_style) != 0) {
                    if (super_weirdo != 0 && qrand() <= 0x7fff &&
                        (candidate->apiobj.character_data->model_flags & 0x10) == 0 &&
                        candidate->apiobj.character_model->model_data_b[0x41] != NULL) {
                        candidate_direct = 1;
                    } else if (second_style != 0) {
                        if (candidate->apiobj.character_model->model_data_b[0x54] == NULL) {
                            if ((candidate->apiobj.character_data->model_flags & 0x10) == 0)
                                goto reload_object_count;
                            candidate_choke = 0;
                            candidate_second = 0;
                        }
                    } else if (candidate->apiobj.character_model->model_data_b[0x53] == NULL) {
                        if ((candidate->apiobj.character_data->model_flags & 0x10) == 0)
                            goto reload_object_count;
                        candidate_choke = 0;
                    }
                } else if ((candidate->apiobj.character_data->model_flags & 0x10) == 0) {
                    if (candidate->apiobj.character_model->model_data_b[0x41] != NULL) {
                        candidate_direct = 1;
                    } else if (candidate->apiobj.character_model->model_data_b[0x2b] == NULL) {
                        goto reload_object_count;
                    }
                } else if (candidate->apiobj.character_model->model_data_b[0x2b] == NULL &&
                           candidate->apiobj.character_model->model_data_b[5] == NULL) {
                    goto reload_object_count;
                }

                NUVEC delta;
                f32 distance =
                    NuVecDistSqr(&object->apiobj.collision_position, &candidate->apiobj.collision_position, &delta);
                if (candidate->id == id_ATST) {
                    distance *= 1.0f / 3.0f;
                }
                if (!(distance < best_distance)) {
                    goto reload_object_count;
                }
                object_count = HIGHGAMEOBJECT;
                if (delta.x * object->facing_direction.x + delta.z * object->facing_direction.z < 0.0f) {
                    best = candidate;
                    best_distance = distance;
                    selected_choke = candidate_choke;
                    selected_second = candidate_second;
                    selected_direct = candidate_direct;
                }
                continue;
            }
        reload_object_count:
            object_count = HIGHGAMEOBJECT;
        }
    }
    if (best == NULL || best->character_context == 0x0f) {
        return;
    }

    object->field_0xe22 |= 2;
    object->force_glow_candidate = best;
    object->force_glow_candidate_kind = 2;
    SetObjAsHeadTarget(object, best, 2, 1.0f, 0.0f, 0.0f);
    if (activate == 0 && !objInNetWaitContext(object, 0x1b)) {
        return;
    }

    if (object->id == id_LUKESKYWALKERDAGOBAH && dagobah_training != 0 && object->field_0xcc0 == NULL &&
        FreePlay == 0) {
        if (AnimPlaying(&object->apiobj.anim_packet, 0x0b, 1, 1) == NULL &&
            AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL) {
            PlaySfx(const_cast<char *>("JForcePush"), &object->apiobj.collision_position);
            PlayGruntSfx(object);
        }
        NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        object->apiobj.movement_facing_angle =
            NuAtan2D(best->apiobj.collision_position.x - object->apiobj.collision_position.x,
                     best->apiobj.collision_position.z - object->apiobj.collision_position.z);
        object->field_0xe23 |= 1;
        GameCam_HitRoll();
        return;
    }

    object->character_context = 0x1b;
    object->force_target = best;
    object->field_0x7a3 = 0;
    object->context_animation_timer = 0.0f;
    object->airborne_action_duration = 0.3f;
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    object->field_0xe21 &= ~3;
    object->field_0xe20 &= ~0x80;
    if (selected_direct != 0) {
        object->field_0xe21 |= 2;
    } else if (selected_choke != 0 || selected_second != 0) {
        object->field_0xe20 |= 0x80;
        if (selected_second != 0) {
            object->field_0xe21 |= 1;
        }
    }

    if ((object->field_0xe21 & 4) == 0) {
        FaceOpponent(object, NULL);
        if ((object->field_0xe21 & 2) != 0) {
            PlaySfx(const_cast<char *>("ForceMindTrick"), &object->apiobj.collision_position);
        } else {
            Player_ClearContext(best, 1);
            Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(best->player_packet));
            if ((object->field_0xe21 & 1) != 0) {
                best->context_animation = 0x54;
                best->action_movement_state = 2;
            } else if ((object->field_0xe20 & 0x80) != 0) {
                PlaySfx(const_cast<char *>("ForceChokeCrunch"), &best->apiobj.collision_position);
                best->context_animation = 0x53;
                best->action_movement_state = 3;
            } else {
                best->context_animation = best->apiobj.character_model->model_data_b[0x2b] != NULL
                                              ? 0x2b
                                              : (best->apiobj.character_model->model_data_b[5] != NULL ? 5 : 0x2b);
                best->action_movement_state = 0;
                if (object->apiobj.player_controlled && Cheat_IsOn(0x13)) {
                    best->action_movement_state = 4;
                }
            }
            best->force_target = object;
            best->character_context = 0x1c;
            FastWeaponIn(best, 0);
            FaceOpponent(best, NULL);
        }
    }
    PlaySfx(const_cast<char *>("JForcePush"), &object->apiobj.collision_position);
    PlayGruntSfx(best);
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
    if (VehicleArea != 0 || object->apiobj.field_0x27c == -1) {
        return;
    }

    i8 saved_context = object->character_context;
    u16 wall_angle;
    i32 surface;
    i32 angle_difference;
    i32 obstacle_input_difference;
    i32 set_facing_angle;
    i32 allow_block_push;
    i32 walljumpwait_context;
    GIZSPINNER_s *found_spinner;
    NUVEC *search_center;
    i8 context;
    if (Pushing(object, &wall_angle, &surface, &angle_difference) == 0) {
        GAMEPAD_s *pad = object->pad_gamepad;
        if (pad->input_magnitude <= 0.0f) {
            goto check_pushobstacle_context;
        }
        context = object->character_context;
        object->field_0xdc4 -= FRAMETIME;
        if (object->field_0xdc4 < 0.0f) {
            object->field_0xdc4 = 0.0f;
        }
        set_facing_angle = 0;
        goto check_cinfo_flags;
    }

    wall_angle += 0x8000;
    // The lean timer only arms on surfaces flagged pushable (0x20000). Plain
    // walls never start it, so walking into them stays walk-in-place.
    if (static_cast<u8>(object->apiobj.field_0x281) <= 31 &&
        (TerSurface[object->apiobj.field_0x281].flags & 0x20000) != 0) {
        object->field_0xdc4 = 0.4f;
        context = object->character_context;
        set_facing_angle = 1;
        goto check_cinfo_flags;
    }
    if (object->apiobj.field_0x27d == 0) {
        if (GameObjectNearFloor(object, 1.25f, NULL) == 0) {
            object->field_0xdc4 = 0.01f;
            context = object->character_context;
            set_facing_angle = 1;
            goto check_cinfo_flags;
        }
    }
    if (FRAMETIME > 1.0f / 30.0f) {
        object->field_0xdc4 += FRAMETIME + FRAMETIME;
    } else {
        f32 step = 1.0f / FRAMETIME / 30.0f;
        step += step;
        step *= FRAMETIME;
        object->field_0xdc4 += step;
    }
    if (object->field_0xdc4 >= 0.25f) {
        object->field_0xdc4 = 0.4f;
    }
    context = object->character_context;
    set_facing_angle = 1;
    goto check_cinfo_flags;

check_pushobstacle_context: {
    context = object->character_context;
    if (LEGOCONTEXT_PUSHOBSTACLE == -1) {
        object->field_0xdc4 = 0.0f;
        set_facing_angle = 0;
        goto check_cinfo_flags;
    }
    if (LEGOCONTEXT_PUSHOBSTACLE != context) {
        object->field_0xdc4 = 0.0f;
        set_facing_angle = 0;
        goto check_cinfo_flags;
    }
    object->field_0xdc4 -= FRAMETIME;
    if (object->field_0xdc4 < 0.0f) {
        object->field_0xdc4 = 0.0f;
    }
    set_facing_angle = 0;
    goto check_cinfo_flags;
}

check_cinfo_flags: {
    if ((CInfo[context].flags & 0x2000) == 0) {
        goto check_jump_context;
    }
    if (LEGOCONTEXT_WALLJUMPWAIT != -1 && LEGOCONTEXT_WALLJUMPWAIT == context) {
        goto walljumpwait_pressed;
    }
    if (set_facing_angle != 0) {
        object->takeover_start_angle = wall_angle;
    }
    if (object->field_0xdc4 <= 0.0f) {
        goto exit_push_check;
    }
    goto check_current_contexts;
}

check_jump_context: {
    if (object->apiobj.field_0x27d == 0) {
        goto near_floor_push_check;
    }
    if (context == -1) {
        goto push_entry_gate;
    }
    allow_block_push = 0;
    if (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == context) {
        allow_block_push = object->action_movement_state == 9;
    }
    goto check_walljump_flag;
}

check_walljump_flag: {
    GAMECHARACTERDATA_s *game_character =
        static_cast<GAMECHARACTERDATA_s *>(object->apiobj.character_data->field11_0x24);
    if ((game_character->flags_090 & 0x8000000) == 0) {
        goto check_current_contexts;
    }
    walljumpwait_context = LEGOCONTEXT_WALLJUMPWAIT;
    if (walljumpwait_context == -1) {
        goto check_current_contexts;
    }
    if (object->apiobj.supporting_platform_id == -1) {
        goto wallshuffle_entry;
    }
    goto check_current_contexts;
}

check_current_contexts: {
    if (LEGOCONTEXT_PUSHOBSTACLE != -1 && LEGOCONTEXT_PUSHOBSTACLE == context) {
        goto pushobstacle_maintain;
    }
    if (LEGOCONTEXT_PUSHSPINNER != -1 && LEGOCONTEXT_PUSHSPINNER == context) {
        goto pushspinner_maintain;
    }
    if (LEGOCONTEXT_PUSH != -1 && LEGOCONTEXT_PUSH == context) {
        goto pushblock_maintain;
    }
    goto revalidate_context;
}

revalidate_context: {
    // Maintain blocks always come back here: when the context is unchanged
    // the frame's push work is done, so return instead of looping. Only a
    // freshly changed context runs the weapon/grunt/facing tail below.
    context = object->character_context;
    if (context == saved_context) {
        return;
    }
    if ((CInfo[context].flags & 0x2000) == 0) {
        return;
    }
    FastWeaponIn(object, 0);
    if (LEGOCONTEXT_WALLJUMPWAIT != -1 && LEGOCONTEXT_WALLJUMPWAIT == context) {
        PlayLandSfx(object, 0, 0);
    } else {
        PlayGruntSfx(object);
    }
    if (set_facing_angle == 0) {
        return;
    }
    object->takeover_start_angle = wall_angle;
    return;
}

near_floor_push_check: {
    i32 near_floor = GameObjectNearFloor(object, 1.25f, NULL);
    context = object->character_context;
    if (near_floor != 0 && context == -1) {
        goto push_entry_gate;
    }
    goto check_jump_context_inner;
}

check_jump_context_inner: {
    allow_block_push = 0;
    if (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == context) {
        allow_block_push = object->action_movement_state == 9;
    }
    goto check_walljump_flag;
}

push_entry_gate: {
    if (allow_push != 0) {
        if (LEGOACT_PUSH == -1 || object->apiobj.character_model->model_data_b[LEGOACT_PUSH] == NULL) {
            goto check_jump_context_inner;
        }
    }
    if (object->field_0xdc4 < 0.25f) {
        goto check_current_contexts;
    }
    if (static_cast<u32>(surface - 30) <= 1) {
        goto obstacle_path;
    }
    if ((object->field_0xf02 & 2) != 0) {
        goto obstacle_path;
    }
    // With or without a supporting platform the original converges on
    // entering PUSH here (the 4f5c66 compare selects identical arms).
    object->character_context = LEGOCONTEXT_PUSH;
    if (object->character_context == -1) {
        goto check_current_contexts;
    }
    object->context_animation = LEGOACT_PUSH;
    SetPushAngle(object);
    context = object->character_context;
    goto check_current_contexts;
}

exit_push_check: {
    if (LEGOCONTEXT_PUSHOBSTACLE != -1 && LEGOCONTEXT_PUSHOBSTACLE == context) {
        GameCam_Blend(NULL, 0.5f, 0.0f, 1);
    }
    object->character_context = -1;
    context = -1;
    goto check_current_contexts;
}

walljumpwait_pressed: {
    GAMEPAD_s *pad = object->pad_gamepad;
    if ((pad->buttons_pressed & GAMEPAD_JUMP) == 0) {
        if ((pad->buttons_pressed & GAMEPAD_ACTION) == 0) {
            goto push_anim_check;
        }
        object->landing_followup = 3;
        goto push_anim_check;
    }
    object->landing_followup = 1;
    goto push_anim_check;
}

push_anim_check: {
    i16 anim = object->context_animation;
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    if (model->model_data_b[anim] == NULL) {
        goto decay_interaction_timer;
    }
    if (AnimPlaying(&object->apiobj.anim_packet, anim, 1, 0) == NULL) {
        goto revalidate_context;
    }
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer > 0.0f) {
        context = object->character_context;
        goto check_current_contexts;
    }
    goto interaction_timer_expired;
}

decay_interaction_timer: {
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer > 0.0f) {
        context = object->character_context;
        goto check_current_contexts;
    }
    // else fall through to interaction_timer_expired
}

interaction_timer_expired: {
    if (object->landing_followup == 1) {
        goto start_jump_nine;
    }
    if (object->landing_followup != 3) {
        StartEndOfJump(object);
        context = object->character_context;
        goto check_current_contexts;
    }
    if (LEGOACT_LUNGE == -1) {
        goto start_end_of_jump;
    }
    {
        CHARACTERMODEL_s *model = object->apiobj.character_model;
        if (model->model_data_b[LEGOACT_LUNGE] == NULL) {
            goto start_end_of_jump;
        }
    }
    {
        u16 facing = object->apiobj.movement_facing_angle;
        facing += 0x8000;
        object->apiobj.facing_angle = facing;
        object->apiobj.movement_facing_angle = facing;
        object->apiobj.field_0x276 = facing;
    }
    StartLunge(object, 1.0f, 0.0f);
    PlayGruntSfx(object);
    context = object->character_context;
    goto revalidate_context;
}

start_jump_nine: {
    StartJump(object, 9);
    object->airborne_action_timer = 1.2f;
    {
        u16 facing = object->apiobj.movement_facing_angle;
        facing += 0x8000;
        object->apiobj.facing_angle = facing;
        object->apiobj.movement_facing_angle = facing;
        object->apiobj.field_0x276 = facing;
    }
    PlayGruntSfx(object);
    context = object->character_context;
    goto revalidate_context;
}

start_end_of_jump: {
    StartEndOfJump(object);
    context = object->character_context;
    goto check_current_contexts;
}

pushobstacle_maintain: {
    GAMEPAD_s *pad = object->pad_gamepad;
    if ((pad->buttons_pressed & GAMEPAD_JUMP) != 0) {
        goto jump_out_with_flag;
    }
    if (pad->input_magnitude <= 0.0f) {
        goto input_dead_obstacle;
    }
    {
        u16 wanted = GamePad_InputAngle(object, pad);
        u16 have = object->takeover_start_angle;
        obstacle_input_difference = RotDiff(wanted, have);
        if (obstacle_input_difference < 0) {
            obstacle_input_difference = -obstacle_input_difference;
        }
        if (obstacle_input_difference > 0x31c6) {
            goto obstacle_steer;
        }
    }
    object->field_0x758 = 0.75f;
    object->context_animation = LEGOACT_SUPERPUSH_PUSH;
    if (object->field_0x7a6 == 0x1e) {
        goto obstacle_control_eq2;
    }
    {
        GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(object->field_0x788);
        i32 mode = obstacle->anim_set->state;
        i32 below = mode == 0;
        i32 at_least = mode != 0;
        GizObstacle_SetPushControlled(obstacle, object, -1.0f);
        if (below) {
            goto obstacle_rumble_a;
        }
        if (at_least) {
            goto obstacle_rumble_b;
        }
    }
    goto revalidate_context;
}

obstacle_control_eq2: {
    GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(object->field_0x788);
    i32 mode = obstacle->anim_set->state;
    i32 is_two = mode == 2;
    i32 not_two = mode != 2;
    GizObstacle_SetPushControlled(obstacle, object, 1.0f);
    if (is_two) {
        goto obstacle_rumble_a;
    }
    if (not_two) {
        goto obstacle_rumble_b;
    }
    goto revalidate_context;
}

obstacle_rumble_a: {
    if (ParticlesPerSecond(3.0f, FRAMETIME) > 0) {
        GAMEPAD_s *pad = object->pad_gamepad;
        NewBuzzFrames(pad->pad, 1, 0);
    }
    goto revalidate_context;
}

obstacle_rumble_b: {
    {
        f32 scaled = QRAND_FLOAT();
        scaled *= 0.3f;
        GAMEPAD_s *pad = object->pad_gamepad;
        NewRumble(pad->pad, scaled, 0);
    }
    goto revalidate_context;
}

jump_out_with_flag: {
    StartJump(object, 0);
    object->field_0xdc4 = 0.0f;
    object->movement_runtime_flags |= 0x10;
    return;
}

input_dead_obstacle: {
    object->context_animation = LEGOACT_SUPERPUSH_IDLE;
    object->field_0x758 -= FRAMETIME;
    if (object->field_0x758 > 0.0f) {
        goto revalidate_context;
    }
    GameCam_Blend(NULL, 0.5f, 0.0f, 1);
    object->character_context = -1;
    return;
}

obstacle_steer: {
    if (obstacle_input_difference <= 0x4e38) {
        goto superpush_idle_set;
    }
    object->field_0x758 = 0.75f;
    object->context_animation = LEGOACT_SUPERPUSH_PULL;
    if (object->field_0x7a6 != 0x1e) {
        goto obstacle_control_eq2;
    }
    {
        GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(object->field_0x788);
        i32 mode = obstacle->anim_set->state;
        i32 below = mode == 0;
        i32 at_least = mode != 0;
        GizObstacle_SetPushControlled(obstacle, object, -1.0f);
        if (below) {
            goto obstacle_rumble_a;
        }
        if (at_least) {
            goto obstacle_rumble_b;
        }
    }
    goto revalidate_context;
}

superpush_idle_set: {
    object->context_animation = LEGOACT_SUPERPUSH_IDLE;
    context = object->character_context;
    goto revalidate_context;
}

pushspinner_maintain: {
    GAMEPAD_s *pad = object->pad_gamepad;
    if ((pad->buttons_pressed & GAMEPAD_JUMP) == 0) {
        GIZSPINNER_s *spinner = static_cast<GIZSPINNER_s *>(object->field_0x788);
        if (GizSpinner_Push(spinner, object->field_0x7a6) == 0) {
            if (qrand() <= 0x7ff) {
                GizSpinner_PushFail(object, spinner);
            } else {
                spinner->state_flags &= ~0x300u;
            }
            goto revalidate_context;
        }
        if (object->apiobj.player_controlled) {
            f32 scaled = QRAND_FLOAT();
            scaled *= 0.4f;
            NewRumble(object->pad_gamepad->pad, scaled, 0);
        }
        search_center = &object->apiobj.collision_position;
        GameAudio_PlaySfx(0x38, search_center, 0, 0);
        context = object->character_context;
        goto revalidate_context;
    }
    StartJump(object, 0);
    object->field_0xdc4 = 0.0f;
    object->movement_runtime_flags |= 0x10;
    return;
}

pushblock_maintain: {
    if (PushBlock(object) != 0) {
        context = object->character_context;
        goto revalidate_context;
    }
    if (LEGOCONTEXT_WALLSHUFFLE == -1) {
        goto clear_context;
    }
    {
        i16 idle_anim = LEGOACT_WALLSHUFFLE_IDLE;
        if (idle_anim == -1) {
            goto clear_context;
        }
        CHARACTERMODEL_s *model = object->apiobj.character_model;
        if (model->model_data_b[idle_anim] == NULL) {
            goto clear_context;
        }
    }
    {
        i32 abs_angle = angle_difference;
        if (abs_angle < 0) {
            abs_angle = -abs_angle;
        }
        if (abs_angle <= 0x71c7) {
            goto clear_context;
        }
    }
    object->character_context = LEGOCONTEXT_WALLSHUFFLE;
    object->context_animation = LEGOACT_WALLSHUFFLE_IDLE;
    goto revalidate_context;
}

clear_context: {
    object->character_context = -1;
    context = -1;
    goto revalidate_context;
}

obstacle_path: {
    if (LEGOCONTEXT_PUSHSPINNER != -1) {
        goto spinner_find;
    }
    goto obstacle_find_check;
}

spinner_find: {
    // NOTE: unlike the obstacle branch below, the original performs no
    // permission check here and goes straight to the spinner search.
    search_center = &object->apiobj.collision_position;
    {
        found_spinner = GizSpinner_Find(WORLD, search_center, 1);
        if (found_spinner == NULL) {
            goto obstacle_find_check;
        }
        if ((found_spinner->flags & 8) == 0) {
            goto check_spinner_room;
        }
        if (ShadowMode != 0) {
            goto check_spinner_room;
        }
    }
    context = object->character_context;
    goto push_anim_set;
}

check_spinner_room: {
    if (found_spinner->room_index != -1) {
        context = object->character_context;
        goto push_anim_set;
    }
    if (found_spinner->field_0x090 <= 0.0f) {
        goto spinner_teamwork_check;
    }
    context = object->character_context;
    goto push_anim_set;
}

spinner_teamwork_check: {
    // Another player already owns this spinner; only an unclaimed one can attach.
    i32 pushspinner_id = LEGOCONTEXT_PUSHSPINNER;
#define CHECK_TEAMMATE(idx)                                                                                            \
    {                                                                                                                  \
        GameObject_s *teammate = Player[idx];                                                                          \
        i8 teammate_ctx;                                                                                               \
        if (teammate == NULL)                                                                                          \
            goto next_teammate_##idx;                                                                                  \
        if (teammate == object)                                                                                        \
            goto next_teammate_##idx;                                                                                  \
        teammate_ctx = teammate->character_context;                                                                    \
        if (teammate_ctx == pushspinner_id) {                                                                          \
            if (teammate->field_0x788 == found_spinner) {                                                              \
                context = object->character_context;                                                                   \
                goto push_anim_set_tail;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
        next_teammate_##idx :;                                                                                         \
    }
    CHECK_TEAMMATE(0)
    CHECK_TEAMMATE(1)
    CHECK_TEAMMATE(2)
    CHECK_TEAMMATE(3)
    CHECK_TEAMMATE(4)
    CHECK_TEAMMATE(5)
    CHECK_TEAMMATE(6)
    CHECK_TEAMMATE(7)
#undef CHECK_TEAMMATE
    // No teammate holds it: attach this spinner.
    object->field_0x788 = found_spinner;
    object->field_0x7a6 = static_cast<u8>(surface);
    object->character_context = LEGOCONTEXT_PUSHSPINNER;
    FastWeaponIn(object, 0);
    AlertSurroundingCreatures(object, search_center);
    context = object->character_context;
    goto push_anim_set_tail;
}

push_anim_set: {
    context = object->character_context;
    goto push_anim_set_tail;
}

push_anim_set_tail: {
    if (context == -1) {
        goto check_current_contexts;
    }
    object->context_animation = LEGOACT_PUSH;
    SetPushAngle(object);
    goto check_current_contexts;
}

obstacle_find_check: {
    if (LEGOCONTEXT_PUSHOBSTACLE == -1) {
        goto attach_obstacle_fail;
    }
    if (CanPushObstaclesFn == NULL) {
        goto attach_obstacle_fail;
    }
    {
        i8 find_context = context;
        if (CanPushObstaclesFn(object) == 0) {
            goto attach_obstacle_fail;
        }
        context = find_context;
    }
    {
        f32 distance;
        search_center = &object->apiobj.collision_position;
        GIZOBSTACLE_s *obstacle = GizObstacle_FindNearest(WORLD->giz_obstacle_sys, search_center, object, &distance, 7);
        if (obstacle == NULL) {
            goto attach_obstacle_fail;
        }
        if (distance >= 6.25f) {
            goto attach_obstacle_fail;
        }
        object->field_0x758 = 0.75f;
        object->field_0x788 = obstacle;
        object->field_0x7a6 = static_cast<u8>(surface);
        object->character_context = LEGOCONTEXT_PUSHOBSTACLE;
        FastWeaponIn(object, 0);
        GameCam_Blend(NULL, 0.5f, 0.0f, 1);
        AlertSurroundingCreatures(object, search_center);
        context = object->character_context;
        goto push_anim_set_tail;
    }
}

attach_obstacle_fail: { goto push_anim_set; }

wallshuffle_entry: {
    if (static_cast<u32>(surface) > 31) {
        goto wallshuffle_entry_gated;
    }
    if ((TerSurface[surface].flags & 0x10581) != 0) {
        goto check_current_contexts;
    }
wallshuffle_entry_gated: {
    GAMECHARACTERDATA_s *gcd = static_cast<GAMECHARACTERDATA_s *>(object->apiobj.character_data->field11_0x24);
    if (object->pad_gamepad->input_magnitude != gcd->run_speed && !allow_block_push) {
        goto check_current_contexts;
    }
    if (set_facing_angle == 0) {
        goto check_current_contexts;
    }
    {
        i32 abs_angle = angle_difference;
        if (abs_angle < 0) {
            abs_angle = -abs_angle;
        }
        if (abs_angle <= 0x6aaa) {
            goto check_current_contexts;
        }
    }
    if (object->apiobj.horizontal_velocity_magnitude <= (gcd->run_speed + gcd->field_0x18) * 0.5f) {
        goto check_current_contexts;
    }
    if (object->apiobj.field_0x27d != 0) {
        goto check_current_contexts;
    }
    if (object->apiobj.collision_min.y - object->apiobj.field_0x218 <= 0.15f) {
        goto check_current_contexts;
    }
    if (LEGOCONTEXT_JUMP == -1) {
        goto check_current_contexts;
    }
    if (LEGOCONTEXT_JUMP != context) {
        goto check_current_contexts;
    }
    if (object->action_movement_state == 0) {
        if (object->jump_sequence > 1) {
            goto check_current_contexts;
        }
    } else if (object->action_movement_state != 9) {
        goto check_current_contexts;
    }
    if (object->apiobj.velocity.y <= -1.25f) {
        goto check_current_contexts;
    }
    if (object->context_animation_timer < 0.25f) {
        goto check_current_contexts;
    }
    {
        i8 entry_context = static_cast<i8>(walljumpwait_context);
        object->character_context = entry_context;
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
        i16 wait_anim = LEGOACT_WALLJUMP_WAIT;
        object->context_animation = wait_anim;
        object->landing_followup = 0;
        f32 duration = AnimDuration(object->id, wait_anim, 0.0f, 0.0f, 0);
        f32 timer_value = duration <= 0.0f ? 0.3f : duration;
        object->apiobj.movement_facing_angle = wall_angle;
        object->context_animation_timer = timer_value;
        object->apiobj.velocity = v000;
    }
    context = object->character_context;
    goto check_current_contexts;
}
}
}
