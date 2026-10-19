// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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
#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/motion/action_info.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include <string.h>
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"
#include <stdio.h>
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameframework/saveload.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "batman.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include <ctype.h>
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include <stdarg.h>
#include <limits.h>
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/gizmos/door/gizspinner.h"
#include "legoapi/actions/movement/pushblocks.h"

extern u32 (*CanPushObstaclesFn)(GameObject_s *);

extern i32 LevFlag[4];

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct EDCREATURE_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

struct GIZSPINNER_s;

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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
            if (__builtin_expect((static_cast<u32>(surface) - 30) < 2 || (object->field_0xf02 & 2) != 0, 1)) {
                GIZSPINNER_s *spinner = NULL;
                if (__builtin_expect(LEGOCONTEXT_PUSHSPINNER != -1, 0))
                    spinner = GizSpinner_Find(WORLD, &object->apiobj.collision_position, 1);
                if (spinner != NULL) {
                    if (((spinner->flags & 8) == 0 || ShadowMode != 0) && spinner->room_index == -1 &&
                        !(spinner->field_0x090 > 0.0f)) {
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
        if (!!(pushing))
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
        if (__builtin_expect(!!(pushing), 1))
            object->takeover_start_angle = wall_angle;
    }
}

i32 WallShuffle_SetTargetMom(GameObject_s *object, u16 input_angle) {
    object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle);
    object->target_velocity.z = NU_COS_LUT(object->apiobj.movement_facing_angle);
    u16 sideways_angle = object->apiobj.movement_facing_angle + 0x4000;
    f32 direction = PushingTowardsAngle(input_angle, sideways_angle);
    f32 speed;
    if (direction > NuTrigTable[0x3555]) {
        if (LEGOACT_WALLSHUFFLE_RIGHT != -1 &&
            object->apiobj.character_model->model_data_b[LEGOACT_WALLSHUFFLE_RIGHT] != NULL) {
            object->context_animation = LEGOACT_WALLSHUFFLE_RIGHT;
            speed = AnimSpeed(object->apiobj.character_model, LEGOACT_WALLSHUFFLE_RIGHT);
        } else {
            speed = 0.5f;
        }
    } else if (direction < -NuTrigTable[0x3555]) {
        if (LEGOACT_WALLSHUFFLE_LEFT != -1 &&
            object->apiobj.character_model->model_data_b[LEGOACT_WALLSHUFFLE_LEFT] != NULL) {
            object->context_animation = LEGOACT_WALLSHUFFLE_LEFT;
            speed = -AnimSpeed(object->apiobj.character_model, LEGOACT_WALLSHUFFLE_LEFT);
        } else {
            speed = -0.5f;
        }
    } else {
        object->context_animation = LEGOACT_WALLSHUFFLE_IDLE;
        speed = 0.0f;
    }
    if (speed != 0.0f) {
        object->target_velocity.x += NU_SIN_LUT(sideways_angle) * speed;
        object->target_velocity.z += NU_COS_LUT(sideways_angle) * speed;
    }
    return 0;
}

void ReleasePush(GameObject_s *object) {
    if ((CInfo[object->character_context].flags & 0x2000) != 0) {
        if (LEGOCONTEXT_PUSHOBSTACLE != -1 && LEGOCONTEXT_PUSHOBSTACLE == object->character_context) {
            GameCam_Blend(NULL, 0.5f, 0.0f, 1);
        }
        object->field_0xdc4 = 0.0f;
        object->character_context = -1;
    }
}

