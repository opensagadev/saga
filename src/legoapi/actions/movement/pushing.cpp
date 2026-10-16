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
extern i32 dagobah_training;
extern AREADATA *EMPERORFIGHT_ADATA;
extern i16 id_GAMORREANGUARD;

void FindForcePushTarget(GameObject_s *object, i32 activate, i32 target_filter) {
    NUVEC delta;
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
    if (best != NULL && best->character_context != 0x0f) {

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
            if ((object->field_0xe21 & 2) == 0) {
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
            } else {
                PlaySfx(const_cast<char *>("ForceMindTrick"), &object->apiobj.collision_position);
            }
        }
        PlaySfx(const_cast<char *>("JForcePush"), &object->apiobj.collision_position);
        PlayGruntSfx(best);
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
            if ((static_cast<u32>(surface) - 30) < 2 || (object->field_0xf02 & 2) != 0) {
                GIZSPINNER_s *spinner = NULL;
                if (LEGOCONTEXT_PUSHSPINNER != -1)
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
            if (static_cast<u32>(surface) <= 31 && (TerSurface[surface].flags & 0x10581) != 0)
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
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0)
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
        if (pushing)
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
        if (pushing)
            object->takeover_start_angle = wall_angle;
    }
}
