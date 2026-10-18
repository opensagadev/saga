// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/objects/gameobjects.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/actions/movement/jumping_types.h"
#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/motion/action_info.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/system/socksys.h"
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
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include <string.h>
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
#include <math.h>
#include "legoapi/audio/audio.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/items/base/animpacket.h"
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
#include "legoapi/characters/motion/move.h"
#include "legoapi/items/fx/explosions.h"

i32 DoubleJump_JediSlam = 0;
f32 SLAMJUMPSPEED = 3.0f;
bool (*IsWearingBackPackFn)(GameObject_s *) = NULL;
i32 (*Jump_PreventJumpFn)(GameObject_s *) = NULL;
i32 (*CanMagnetClimbFn)(GameObject_s *) = NULL;
i32 (*CanGlideFn)(GameObject_s *) = NULL;
i32 DoubleJump_AlwaysReachJump2Height = 0;
i32 (*Slam_GetDebrisFn)(GameObject_s *, i32) = NULL;

static const f32 PLAYER_JUMP_MINIMUM_AIR_TIME = 0.1f;
static const f32 PLAYER_JUMP_REENTRY_DELAY = 0.2f;

void (*BigJump_EndOfLandFn)(GameObject_s *) = NULL;

static i32 BigJump_JumpAction_Default(GameObject_s *object);
i32 (*BigJump_JumpActionFn)(GameObject_s *) = BigJump_JumpAction_Default;

static i32 BigJump_LandAction_Default(GameObject_s *object);
i32 (*BigJump_LandActionFn)(GameObject_s *) = BigJump_LandAction_Default;

// Original action/context lookup data, with its arrays and pointers in one owner.

i32 (*FindSlamOrigin_UseCPosFn)(GameObject_s *) = NULL;
void (*Jump_EndOfLandContextFn)(GameObject_s *) = NULL;

extern i32 LevFlag[4];

extern i16 id_GAMORREANGUARD;

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

struct PLAYERPACKET_s;

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

extern i32 (*Slam_GetDebrisFn)(GameObject_s *, i32);

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

static i32 BigJump_JumpAction_Default(GameObject_s *object) {
    if (object->field_0x7aa == 0) {
        if (LEGOACT_COMBOJUMP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_COMBOJUMP] != NULL)
            return LEGOACT_COMBOJUMP;
        if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
            return LEGOACT_JUMP2;
    } else if (object->field_0x7aa == 4) {
        if (LEGOACT_FLIP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL)
            return LEGOACT_FLIP;
    }
    return LEGOACT_JUMP;
}

static i32 BigJump_LandAction_Default(GameObject_s *object) {
    switch (object->field_0x7aa) {
        case 0:
            if (LEGOACT_COMBOLAND != -1 && object->apiobj.character_model->model_data_b[LEGOACT_COMBOLAND] != NULL)
                return LEGOACT_COMBOLAND;
            break;
        case 2:
        case 3:
            if (LEGOACT_LAND3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LAND3] != NULL)
                return LEGOACT_LAND3;
            break;
        case 4:
            if (LEGOACT_FLIPLAND != -1 && object->apiobj.character_model->model_data_b[LEGOACT_FLIPLAND] != NULL)
                return LEGOACT_FLIPLAND;
            return LEGOACT_LAND;
        case 1:
            return LEGOACT_LAND;
        default:
            return LEGOACT_LAND;
    }
    if (LEGOACT_LAND2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LAND2] != NULL)
        return LEGOACT_LAND2;
    return LEGOACT_LAND;
}

void StartJump(GameObject_s *object, i32 movement_state) {
    if (LEGOCONTEXT_JUMP != -1) {
        object->character_context = LEGOCONTEXT_JUMP;
        object->action_movement_state = static_cast<u8>(movement_state);
        object->jump_sequence = 1;
        object->context_animation_timer = 0.0f;
        object->jump_start_height = object->apiobj.collision_min.y;
        object->context_animation = LEGOACT_JUMP;
        object->jump_flags =
            static_cast<u8>((object->jump_flags & ~PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF) |
                            ((movement_state == 3 || movement_state == 4) ? PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF : 0));
        object->context_variant_flags =
            static_cast<i8>(static_cast<u8>(object->context_variant_flags) & ~PLAYER_JUMP_VARIANT_START_CLEAR);
        object->airborne_action_timer = 0.0f;
        if (static_cast<u32>(movement_state - 6) > 1) {
            PlayJumpSfx(object, 0);
        }
    }
    object->apiobj.field_0x27d = 0;
    object->field_0x105c = 0;
    GAMECHARACTERDATA *game_character = object->apiobj.character_data->game_character;
    f32 jump_speed = game_character->jump_speed;
    object->field_0xe22 |= PLAYER_JUMP_RUNTIME_BUTTON_HELD;
    object->delayed_turn_timer = 0.0f;
    object->airborne_input_timer = 0.0f;
    object->apiobj.velocity.y = jump_speed;
    object->airborne_collision_target = NULL;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->apiobj.pitch_angle = 0;
    object->apiobj.roll_angle = 0;
    object->field_0x1086 = 2;
}

void MakeJumpReachHeight(GameObject_s *object, float height, i32 force) {
    const f32 current_height = object->apiobj.collision_min.y - object->jump_start_height;
    const f32 remaining_height = height - current_height;
    if (remaining_height > 0.0f) {
        const GAMECHARACTERDATA *game_character =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        const f32 vertical_speed = NuFsqrt(-2.0f * game_character->gravity * remaining_height);
        if (force != 0 || vertical_speed > object->apiobj.velocity.y) {
            object->apiobj.velocity.y = vertical_speed;
        }
    } else if (force != 0) {
        object->apiobj.velocity.y = 0.0f;
    }
}

void StartEndOfJump(GameObject_s *object) {
    if (LEGOCONTEXT_JUMP != -1) {
        object->character_context = LEGOCONTEXT_JUMP;
        object->action_movement_state = 0;
        object->jump_sequence = 2;
        object->context_animation = LEGOACT_FALL;
        object->jump_flags &= ~PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF;
        object->jump_falling = 1;
        object->jump_button_released = 0;
        object->jump_second_jump = 0;
    }
    object->airborne_collision_target = NULL;
}

i32 StartBigJump(GameObject_s *object, NUVEC *destination, i32 mode, f32 height, f32 duration_scale, i32 animation,
                 i8 variant) {
    if (LEGOCONTEXT_BIGJUMP == -1 || (object->apiobj.character_data->model_flags & 0x200000) != 0)
        return 0;
    Player_ClearContext(object, 0);
    GameObject_s *carried = object->field_0xcc0;
    if (carried == NULL || carried->id != id_YODA || WORLD->current_level != DAGOBAHE_LDATA)
        carried = NULL;
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    object->field_0xcc0 = carried;
    object->build_button_taps = animation;
    object->field_0x7a3 = 1;
    object->jump_flags &= ~4;
    object->character_context = LEGOCONTEXT_BIGJUMP;
    object->field_0x7aa = variant;
    object->context_animation = BigJump_JumpActionFn(object);
    object->external_force = object->apiobj.position;
    object->launch_origin = *destination;
    object->airborne_action_duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
    if (static_cast<u8>(variant - 2) <= 1)
        object->airborne_action_duration = object->apiobj.character_data->game_character->second_jump_duration;
    if (object->field_0x7aa != 0) {
        f32 distance = NuVecXZDist(&object->external_force, &object->launch_origin, NULL);
        f32 speed = object->apiobj.character_data->game_character->run_speed;
        f32 duration;
        if (distance == 0.0f || speed == 0.0f)
            duration = 0.0f;
        else
            duration = distance / speed;
        object->airborne_action_duration =
            object->airborne_action_duration > duration ? object->airborne_action_duration : duration;
        if (object->field_0x7aa == 2) {
            if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                height *= 1.5f;
        } else if (object->field_0x7aa == 3) {
            if (LEGOACT_JUMP3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP3] != NULL)
                height *= 1.8f;
        }
    }
    if (object->airborne_action_duration == 0.0f) {
        object->airborne_action_duration = 1.0f;
        object->context_animation = LEGOACT_FALL;
    } else {
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
    }
    object->airborne_action_duration *= duration_scale;
    object->context_animation_timer = 0.0f;
    object->launch_origin.y -= object->character_bottom * object->apiobj.field_0xa8;
    u16 angle = NuAtan2D(destination->x - object->external_force.x, destination->z - object->external_force.z);
    if (object->field_0x7aa == 4)
        angle -= 0x8000;
    object->apiobj.facing_angle = angle;
    object->apiobj.field_0x276 = angle;
    object->apiobj.movement_facing_angle = angle;
    object->jump_mode = mode;
    PlayJumpSfx(object, 2);
    object->ai.movement_event_flags |= 2;
    object->ai.field_0x180 = NULL;
    object->context_variant_flags &= ~1;
    if (!(height >= 0.0f))
        height = 0.0f;
    object->big_jump_height = height;
    return 1;
}

void FindSlamOrigin(GameObject_s *object, NUVEC *start, NUVEC *end) {
    if (FindSlamOrigin_UseCPosFn != NULL && FindSlamOrigin_UseCPosFn(object)) {
        start->x = object->apiobj.collision_position.x;
        start->z = object->apiobj.collision_position.z;
    } else {
        const i32 joint =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->streak_joints[0][0];
        if (joint == -1 || object->apiobj.field_0x288 == 0 || object->apiobj.character_model == NULL ||
            object->apiobj.character_model->points_of_interest[joint] == NULL) {
            start->x = object->apiobj.position.x;
            start->z = object->apiobj.position.z;
        } else {
            start->x = object->joint_matrices[joint].m30;
            start->z = object->joint_matrices[joint].m32;
        }
    }
    start->y = object->apiobj.field_0x218;
    if (end != NULL) {
        const i32 joint =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->streak_joints[0][1];
        if (joint == -1 || object->apiobj.field_0x288 == 0 || object->apiobj.character_model == NULL ||
            object->apiobj.character_model->points_of_interest[joint] == NULL) {
            *end = *start;
        } else {
            *end = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint], 3);
        }
    }
}

void BigJumpCode(GameObject_s *object) {
    if (LEGOCONTEXT_BIGJUMP == -1 || object->character_context != LEGOCONTEXT_BIGJUMP)
        return;
    object->apiobj.respawn_timer = 0.0f;
    if (object->build_button_taps != 0 && object->field_0x1084 != 0) {
        object->character_context = -1;
        return;
    }
    if (object->field_0x7a3 == 2) {
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            object->character_context = -1;
            if (BigJump_EndOfLandFn == NULL)
                return;
            BigJump_EndOfLandFn(object);
        }
    } else if (object->field_0x7a3 == 1) {
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            if (LEGOACT_FALL != -1 && object->context_animation == LEGOACT_FALL) {
                object->character_context = -1;
                return;
            }
            if (object->field_0x7aa != 0 &&
                (object->pad_gamepad->input_magnitude > 0.0f || object->IsRunningTaskType(MechTouchTaskGoTo::HashId))) {
                object->character_context = -1;
                return;
            }
            WORLDINFO *world = WorldInfo_CurrentlyActive();
            if (LEGOACT_COMBOJUMP != -1 && object->context_animation == LEGOACT_COMBOJUMP) {
                object->apiobj.facing_angle += 0x8000;
                object->apiobj.movement_facing_angle += 0x8000;
                object->apiobj.field_0x276 += 0x8000;
                ResetAnimPacket(&object->apiobj.anim_packet, -1);
            }
            if ((object->context_variant_flags & 1) != 0 && object->apiobj.field_0x27d == 0) {
                f32 ground = GameShadow(NULL, &object->launch_origin, 5.0f, -1);
                if (ground != 2000000.0f && object->launch_origin.y - ground >= 0.1f) {
                    StartEndOfJump(object);
                    return;
                }
            }
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
            object->field_0x7a3 = 2;
            object->context_animation = BigJump_LandActionFn(object);
            f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
            object->airborne_action_duration = duration == 0.0f ? 1.0f : duration;
            object->context_animation_timer = 0.0f;
            if (object->is_boss || (object->field_0xefd & 0x20) != 0) {
                GameCam_Judder(GameCam, -0.3f, 0, &object->apiobj.collision_position);
                PlayLandSfx(object, 3, 0);
                NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
            } else if (object->apiobj.player_controlled && (TestForController() || SuperOptions.touch_controls == 0)) {
                GameCam_Judder(GameCam, -0.2f, 0, &object->apiobj.collision_position);
                PlayLandSfx(object, 3, 0);
            } else {
                PlayLandSfx(object, 0, 0);
            }
            object->external_force = object->apiobj.position;
            object->apiobj.velocity.x = object->apiobj.velocity.y = object->apiobj.velocity.z = 0.0f;
            if ((object->field_0xefd & 0x20) != 0) {
                NUVEC origin;
                FindSlamOrigin(object, &origin, NULL);
                AddExplosion(&origin, 2.0f, 0.4f, object, object->slam_debris_effect, 0x19);
            }
            AISysGetCharacterPathPos(world->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
        } else {
            f32 fraction = object->context_animation_timer / object->airborne_action_duration;
            f32 horizontal = fraction;
            f32 arc = 0.0f;
            if (object->field_0x7aa != 0) {
                f32 rise = object->launch_origin.y - object->external_force.y;
                if (rise > 0.21f) {
                    horizontal = NU_SIN_LUT(static_cast<i32>(fraction * 16384.0f + 32768.0f + 16384.0f)) + 1.0f;
                    if (rise > 0.42f)
                        arc = rise / 3.0f + 0.0f;
                } else if (rise < -0.21f) {
                    horizontal = NU_SIN_LUT(static_cast<i32>(fraction * 16384.0f + 49152.0f + 16384.0f));
                    if (rise < -0.42f)
                        arc = 0.0f - rise / 3.0f;
                }
            }
            f32 sine = NU_SIN_LUT(static_cast<i32>(fraction * 32768.0f));
            object->apiobj.position.x =
                object->external_force.x + (object->launch_origin.x - object->external_force.x) * horizontal;
            object->apiobj.position.y =
                object->external_force.y + (object->launch_origin.y - object->external_force.y) * fraction + arc * sine;
            object->apiobj.position.z =
                object->external_force.z + (object->launch_origin.z - object->external_force.z) * horizontal;
            f32 height = sine * object->apiobj.character_data->game_character->second_jump_height;
            object->apiobj.position.y += (height + height) * object->big_jump_height;
            NuVecSub(&object->apiobj.velocity, &object->apiobj.position, &object->apiobj.start_position);
            NuVecScale(&object->apiobj.velocity, &object->apiobj.velocity, 1.0f / FRAMETIME);
            if ((object->jump_flags & 4) == 0) {
                f32 half_duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0) * 0.5f;
                if (object->context_animation_timer >= half_duration) {
                    if (static_cast<u8>(object->field_0x7aa - 2) <= 1) {
                        if (LEGOACT_JUMP3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP3] != NULL)
                            object->context_animation = LEGOACT_JUMP3;
                        else if (LEGOACT_JUMP2 != -1 &&
                                 object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                            object->context_animation = LEGOACT_JUMP2;
                    }
                    object->jump_flags |= 4;
                }
            }
        }
    }
    if (object->character_context != -1)
        object->ai.movement_event_flags |= 2;
}

i32 ObjLandReady(GameObject_s *object) {
    if (object == NULL) {
        return 0;
    }
    if ((CInfo[object->character_context].flags & 0x1004) != 0 &&
        (LEGOCONTEXT_LAND_COMBATROLL == -1 || LEGOCONTEXT_LAND_COMBATROLL != object->character_context ||
         object->field_0x7a3 != 0)) {
        if (LEGOCONTEXT_WALLSHUFFLE != -1 && LEGOCONTEXT_WALLSHUFFLE == object->character_context) {
            return 0;
        }
        return 1;
    }
    return 0;
}

i32 UseFallAnim(GameObject_s *object) {
    const CHARACTER_CONTEXT_INFO_s &context = CInfo[object->character_context];
    return (context.flags & CHARACTER_CONTEXT_INFO_FLAG_USE_FALL_ANIMATION) != 0 && LEGOACT_FALL != -1 &&
           object->apiobj.character_model->model_data_b[LEGOACT_FALL] != NULL;
}

i32 StartFallLand(GameObject_s *object, i32 action) {
    PlayLandSfx(object, 0, 0);
    if (LEGOCONTEXT_LAND_JUMP == -1) {
        object->movement_runtime_flags &= ~4;
        return 0;
    }
    void **animations = object->apiobj.character_model->model_data_b;
    if (!(action == -1 || animations[action] == NULL)) {
        object->context_animation = action;
        if (animations[object->context_animation] == NULL) {
            object->movement_runtime_flags &= ~4;
            return 0;
        }
    } else {
        if (IsWearingBackPackFn != NULL && IsWearingBackPackFn(object) && LEGOACT_BACKPACKFALLLAND != -1 &&
            animations[LEGOACT_BACKPACKFALLLAND] != NULL) {
            object->context_animation = LEGOACT_BACKPACKFALLLAND;
        } else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_LAND2 != -1 &&
                   animations[LEGOACT_EXTRA_LAND2] != NULL) {
            object->context_animation = LEGOACT_EXTRA_LAND2;
        } else if (LEGOACT_FALLLAND != -1 && animations[LEGOACT_FALLLAND] != NULL) {
            object->context_animation = LEGOACT_FALLLAND;
        } else if (LEGOACT_LAND2 != -1 && animations[LEGOACT_LAND2] != NULL) {
            object->context_animation = LEGOACT_LAND2;
        } else {
            object->context_animation = LEGOACT_LAND;
            if (animations[object->context_animation] == NULL) {
                object->movement_runtime_flags &= ~4;
                return 0;
            }
        }
    }
    object->character_context = LEGOCONTEXT_LAND_JUMP;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    ResetMiniAnimPacket(&object->mini_animation, -1);
    object->fall_animation_timer = 0.0f;
    object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
    object->movement_runtime_flags =
        static_cast<u8>((object->movement_runtime_flags & ~0x08) | ((object->movement_runtime_flags & 0x04) << 1));
    return 1;
}

i32 StartBackFlip(GameObject_s *object) {
    if (LEGOCONTEXT_BACKFLIP == -1 || LEGOACT_BACKFLIP == -1)
        return 0;
    if (object->apiobj.character_model->model_data_b[LEGOACT_BACKFLIP] == NULL)
        return 0;
    object->character_context = LEGOCONTEXT_BACKFLIP;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->context_animation = LEGOACT_BACKFLIP;
    const f32 duration = AnimDuration(object->id, LEGOACT_BACKFLIP, 0.0f, 0.0f, 1);
    object->field_0xe22 |= 0x10;
    object->context_variant_flags &= ~0x80;
    object->delayed_turn_timer = 0.0f;
    object->airborne_input_timer = 0.0f;
    object->context_animation_timer = duration;
    if (object->context_animation_timer <= 0.0f)
        object->context_animation_timer = 1.0f;
    PlayJumpSfx(object, 2);
    return 1;
}

void BackFlipCode(GameObject_s *object) {
    if (LEGOCONTEXT_BACKFLIP == -1 || object->character_context != LEGOCONTEXT_BACKFLIP) {
        return;
    }

    if ((object->field_0xe22 & 0x10) != 0 && (object->pad_gamepad->buttons_held & GAMEPAD_JUMP) == 0) {
        object->field_0xe22 &= static_cast<u8>(~0x10u);
    }

    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
        CurrentAnim(&object->apiobj.anim_packet) != object->context_animation) {
        return;
    }

    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer > 0.0f) {
        return;
    }

    object->character_context = -1;
    if ((object->pad_gamepad->buttons_held & GAMEPAD_JUMP) != 0 && (object->field_0xe22 & 0x10) == 0) {
        StartBackFlip(object);
    } else {
        object->apiobj.velocity.x = 0.0f;
        object->apiobj.velocity.z = 0.0f;
    }
}

i32 Slam_Start(GameObject_s *object, f32 speed) {
    if (LEGOCONTEXT_JUMP == -1)
        return 0;
    if ((object->apiobj.character_data->model_flags & 8) != 0)
        SetWeaponOut(object);
    object->character_context = LEGOCONTEXT_JUMP;
    object->action_movement_state = 4;
    object->context_animation_timer = 0.0f;
    object->context_animation = LEGOACT_SLAM;
    if (object->apiobj.player_controlled)
        object->field_0xef9 |= 8;
    object->jump_sequence = 0;
    object->apiobj.velocity.y = speed;
    PlayJumpSfx(object, 4);
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
    object->jump_flags |= 1;
    if (object->apiobj.player_controlled)
        Hint_SetComplete(0x60a);
    return 1;
}

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 hit_effect) {
    if ((object->movement_runtime_flags & 0x10) != 0) {
        return;
    }
    WORLDINFO *world = WorldInfo_CurrentlyActive();

    const u8 player_flag = object->apiobj.flags_low & 0x80;
    i32 flip_kind = 0;
    i32 force_flip = -1; // set to 1 when a restarted jump should flip
    if (player_flag != 0 || (object->field_0xef9 & 1) != 0) {
        if (LEGOACT_BACKFLIP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_BACKFLIP] != NULL)
            flip_kind = 1;
        else if ((animation_set & 2) != 0 && LEGOACT_FLIP != -1 &&
                 object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL)
            flip_kind = 2;
    }
    if (object->jump_reentry_timer > 0.0f)
        object->jump_reentry_timer -= FRAMETIME;
    f32 chain_timer = object->jump_chain_timer;
    if (chain_timer > 0.0f) {
        chain_timer -= FRAMETIME;
        object->jump_chain_timer = chain_timer;
    }

    if (object->character_context != -1 &&
        (object->character_context == LEGOCONTEXT_LAND_JUMP || object->character_context == LEGOCONTEXT_LAND_JUMP2 ||
         object->character_context == LEGOCONTEXT_LAND_FLIP ||
         object->character_context == LEGOCONTEXT_LAND_COMBOJUMP ||
         object->character_context == LEGOCONTEXT_LAND_LUNGE || object->character_context == LEGOCONTEXT_LAND_SLAM ||
         object->character_context == LEGOCONTEXT_LAND_COMBATROLL)) {
        if (object->character_context == LEGOCONTEXT_LAND_LUNGE && (object->context_flags & 0x40) == 0) {
            const f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (frame > 0.0f) {
                f32 *time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
                if (time != NULL && *time >= frame) {
                    if (__builtin_expect((object->apiobj.character_data->model_flags & 8) == 0, 0))
                        Punch_Hit(object, object->force_target, PUNCHCHARGAP, PUNCHCHARGAP);
                    else
                        ComboHitFrame(object, 1);
                }
            }
        } else if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL) {
            const f32 frame = AnimStopFrame(object->apiobj.character_model, object->context_animation);
            if (frame > 0.0f) {
                f32 *time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
                if (time != NULL && frame <= *time)
                    object->field_0x7a3 = 1;
            }
        }
        if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL &&
            (object->action_input_state & 0xff00ff00) == 0x01000100)
            goto jump_takeoff;
        if (jump_pressed != 0 &&
            (LEGOCONTEXT_LAND_LUNGE == -1 || object->character_context != LEGOCONTEXT_LAND_LUNGE) &&
            (LEGOCONTEXT_LAND_SLAM == -1 || object->character_context != LEGOCONTEXT_LAND_SLAM) &&
            (LEGOCONTEXT_LAND_COMBATROLL == -1 || object->character_context != LEGOCONTEXT_LAND_COMBATROLL))
            goto jump_takeoff;
        if ((LEGOCONTEXT_LAND_LUNGE == -1 || object->character_context != LEGOCONTEXT_LAND_LUNGE) &&
            (LEGOCONTEXT_LAND_SLAM == -1 || object->character_context != LEGOCONTEXT_LAND_SLAM) &&
            ((object->apiobj.flags_low & 0x80) != 0 || object->character_context != LEGOCONTEXT_LAND_JUMP ||
             object->context_animation == -1 ||
             (object->context_animation != LEGOACT_FALLLAND &&
              object->context_animation != LEGOACT_BACKPACKFALLLAND)) &&
            ((LEGOCONTEXT_LAND_COMBATROLL == -1 || object->character_context != LEGOCONTEXT_LAND_COMBATROLL) ||
             ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[2] & 8) != 0 &&
              (object->action_input_state & 0xff00ff00) != 0x01000000)) &&
            object->pad_gamepad->input_magnitude > 0.0f && (object->movement_runtime_flags & 8) == 0) {
            object->character_context = -1;
            return;
        }
        if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL) {
            if (jump_pressed != 0)
                object->landing_followup = 1;
            else if (action_pressed != 0 && LEGOACT_COMBATROLL_FIRE != -1 &&
                     object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_FIRE] != NULL)
                object->landing_followup = 2;
        }
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            if (Jump_EndOfLandContextFn != NULL)
                Jump_EndOfLandContextFn(object);
            if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL) {
                if (object->landing_followup == 2) {
                    object->action_suppressed = 0;
                    const u16 angle = GamePad_InputAngle(object, object->pad_gamepad);
                    object->apiobj.field_0x276 = angle;
                    object->apiobj.movement_facing_angle = angle;
                    object->apiobj.facing_angle = angle;
                    StartQuickShoot(object, LEGOACT_COMBATROLL_FIRE);
                    object->quick_shoot_timer = 0.1f;
                    return;
                }
                if (object->landing_followup == 1)
                    goto jump_takeoff;
                object->character_context = -1;
                return;
            }
            if (object->character_context == LEGOCONTEXT_LAND_LUNGE && (object->context_flags & 0x40) == 0 &&
                (object->apiobj.character_data->model_flags & 8) != 0)
                ComboHitFrame(object, 1);
            if ((object->character_context == LEGOCONTEXT_LAND_LUNGE ||
                 object->character_context == LEGOCONTEXT_LAND_SLAM) &&
                action_held != 0) {
                StartHold(object);
                return;
            }
            object->character_context = CHARACTER_CONTEXT_NONE;
        } else if (object->character_context == LEGOCONTEXT_LAND_SLAM) {
            i32 count = ParticlesPerSecond(25.0f, FRAMETIME);
            NUVEC start, end;
            FindSlamOrigin(object, &start, &end);
            AddGameDebrisRot(world->debris_sys, hit_effect, &start, ParticlesPerSecond(20.0f, FRAMETIME), 0, 0);
            auto world_debris_sys = world->debris_sys;
            while (count > 0) {
                const f32 t = qrand() * 1.5259022e-05f;
                NUVEC position;
                position.x = (end.x - start.x) * t + start.x;
                position.y = (end.y - start.y) * t + start.y;
                position.z = (end.z - start.z) * t + start.z;
                AddGameDebrisRot(world_debris_sys, hit_effect, &position, 1, 0, 0);
                --count;
            }
        }
        return;
    }

    if (LEGOCONTEXT_JUMP == -1)
        return;
    if (object->character_context != LEGOCONTEXT_JUMP) {
        if ((animation_set & 0x80) == 0 && object->fall_acceleration_timer <= 0.0f &&
            (jump_pressed != 0 || ((object->field_0xef9 & 1) != 0 && flip_kind != 0)) &&
            (object->apiobj.field_0x27d != 0 || object->ground_contact_grace_timer > 0.0f) &&
            (CInfo[object->character_context].flags & 0x1000) != 0 &&
            ((animation_set & 4) == 0 ||
             (!(chain_timer > 0.0f) && (object->apiobj.is_underwater == 0 || object->communicate_blend == 0.0f)))) {
        jump_takeoff:
            if (Jump_PreventJumpFn != NULL && Jump_PreventJumpFn(object))
                return;
            if (LEGOCONTEXT_WEAPONOUT != -1 && object->character_context == LEGOCONTEXT_WEAPONOUT)
                FastWeaponOut(object, 0);
            else if (LEGOCONTEXT_WEAPONIN != -1 && object->character_context == LEGOCONTEXT_WEAPONIN)
                FastWeaponIn(object, 0);
            if (object->apiobj.field_0x27d != 0)
                object->field_0xf03 |= 0x20;
            if (object->jump_reentry_timer <= 0.0f)
                object->jump_sequence = 0;
            if (flip_kind == 0 ||
                (!(object->delayed_turn_timer > 0.0f) && force_flip != 1 && (object->field_0xef9 & 1) == 0)) {
                object->action_movement_state = (animation_set & 4) != 0 ? 5 : 0;
                if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_JUMP != -1 &&
                    object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_JUMP] != NULL)
                    object->context_animation = LEGOACT_EXTRA_JUMP;
                else
                    object->context_animation = LEGOACT_JUMP;
                if (CanMagnetClimbFn != NULL && CanMagnetClimbFn(object) && LEGOACT_MAGNET_JUMP != -1 &&
                    object->apiobj.character_model->model_data_b[LEGOACT_MAGNET_JUMP] != NULL)
                    object->context_animation = LEGOACT_MAGNET_JUMP;
                object->jump_start_height = object->apiobj.collision_min.y;
                if ((animation_set & 1) == 0)
                    object->jump_sequence = 1;
                else
                    ++object->jump_sequence;
                if ((animation_set & 1) != 0 && object->jump_sequence > 1) {
                    if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_JUMP2 != -1 &&
                        object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_JUMP2] != NULL)
                        object->context_animation = LEGOACT_EXTRA_JUMP2;
                    else if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                        object->context_animation = LEGOACT_JUMP2;
                    else
                        object->context_animation = LEGOACT_JUMP;
                    object->apiobj.velocity.y =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)
                            ->second_jump_speed;
                    ResetAnimPacket(&object->apiobj.anim_packet, object->context_animation);
                    object->context_variant_flags &= ~0x40;
                    if ((animation_set & 0x100) != 0)
                        MakeJumpReachHeight(object, HIGHJUMPHEIGHT, 1);
                } else {
                    if ((animation_set & 0x40) != 0 &&
                        (object->context_animation == -1 ||
                         object->apiobj.character_model->model_data_b[object->context_animation] == NULL))
                        object->context_animation = LEGOACT_FALL;
                    object->apiobj.velocity.y =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->jump_speed;
                    if (object->action_movement_state != 5)
                        ResetAnimPacket(&object->apiobj.anim_packet, object->context_animation);
                    if ((animation_set & 0x101) == 0x100)
                        MakeJumpReachHeight(object, HIGHJUMPHEIGHT, 1);
                    if ((object->apiobj.flags_low & 0x80) != 0)
                        Hint_SetComplete(0x604);
                }
            } else {
                if (flip_kind == 1 && StartBackFlip(object))
                    return;
                object->action_movement_state = 1;
                object->context_animation = LEGOACT_FLIP;
                ResetAnimPacket(&object->apiobj.anim_packet, LEGOACT_FLIP);
                object->apiobj.velocity.y =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->second_jump_speed;
            }
            const u8 variant_flags = object->context_variant_flags;
            object->context_animation_timer = 0.0f;
            object->jump_flags &= ~1;
            object->character_context = LEGOCONTEXT_JUMP;
            object->context_variant_flags = variant_flags & ~0x10;
            if (object->action_movement_state == 1 && LEGOACT_FLIP != -1 &&
                object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL)
                object->context_animation = LEGOACT_FLIP;
            object->field_0xe22 |= 0x10;
            object->apiobj.field_0x27d = 0;
            object->field_0x105c = 0;
            object->delayed_turn_timer = 0.0f;
            if ((animation_set & 4) != 0) {
                object->jump_variant_timer =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x48;
                if ((object->apiobj.flags_low & 0x80) == 0)
                    object->jump_variant_timer *= 1.5f;
                object->field_0xe31 = 1;
            }
            object->airborne_input_timer = 0.0f;
            object->airborne_collision_target = NULL;
            object->context_variant_flags = variant_flags & 0x6f;
            PlayJumpSfx(object, object->jump_sequence > 1);
            if (object->apiobj.field_0x27f == 1 && object->apiobj.collision_min.y < object->apiobj.water_height)
                AddWaterSplash(object, &object->apiobj.collision_position);
            return;
        }
    } else {
        i8 variant_flags = object->context_variant_flags;
        if ((variant_flags & 0x10) != 0)
            object->movement_runtime_flags |= 0x80;
        if ((object->field_0xe22 & 0x10) != 0 && jump_held == 0) {
            object->field_0xe22 &= ~0x10;
        }

        const f32 vertical_speed = object->apiobj.velocity.y;
        if (vertical_speed < 0.0f) {
            object->airborne_collision_target = NULL;
        }
        const u8 movement_state = object->action_movement_state;
        if (movement_state == 5 && object->field_0xe31 == 2 && object->jump_variant_timer > 0.0f) {
            object->jump_variant_timer -= FRAMETIME;
            if (object->jump_variant_timer <= 0.0f)
                object->field_0xe31 = 3;
        }
        object->context_animation_timer += FRAMETIME;
        if (movement_state == 8 && variant_flags >= 0 &&
            object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            object->context_animation_timer >= object->airborne_action_duration && LEGOACT_COMBATROLL_FALL != -1 &&
            object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_FALL] != NULL) {
            variant_flags |= 0x80;
            object->context_variant_flags = variant_flags;
        }
        if ((movement_state == 0 || movement_state == 2 || movement_state == 8) &&
            object->context_animation_timer >= 2.0f && object->apiobj.start_position.y <= object->apiobj.position.y &&
            object->apiobj.field_0x27d == 0) {
            object->jump_reentry_timer = 0.0f;
            object->character_context = -1;
            object->fall_animation_timer = 0.2f;
            return;
        }

        bool touch_action = false;
        if (MechInputTouchSystem::s_actualTouchMode == 2 && player_flag != 0)
            touch_action = (object->jump_input_flags & 0x20) != 0;
        else
            object->jump_input_flags &= ~0x20;
        bool landed;
        if (movement_state == 5) {
            landed = (object->apiobj.field_0x27d & 1) != 0 ||
                     (player_flag == 0 && object->field_0x1084 != 0 && object->field_0xe31 == 3);
        } else {
            landed = (object->apiobj.field_0x27d != 0 && object->context_animation_timer >= 0.1f) ||
                     ((movement_state == 3 || movement_state == 4) && object->context_animation_timer >= 2.5f);
        }
        if (!landed) {
            GAMECHARACTERDATA *game_character =
                static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
            if ((touch_action || action_pressed != 0) && (vertical_speed > -1.25f || touch_action) &&
                variant_flags >= 0) {
                if (movement_state == 0 && object->jump_sequence < 2 &&
                    (game_character->field275_0x116 != 0 || (object->apiobj.character_data->model_flags & 8) != 0) &&
                    LEGOACT_LUNGE != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LUNGE] != NULL) {
                    object->field_0x780 = NULL;
                    object->blowup_target = NULL;
                    StartLunge(object, 0.0f, object->apiobj.field_0x1e0);
                    return;
                }
            }
            if (DoubleJump_JediSlam != 0 && (animation_set & 0x10) != 0 && (touch_action || action_pressed != 0) &&
                (vertical_speed > -1.25f || touch_action) && variant_flags >= 0 &&
                ((movement_state == 0 && (object->jump_sequence == 2 || (game_character->field_0x98 & 0x20) != 0)) ||
                 movement_state == 1 || movement_state == 2) &&
                LEGOACT_SLAM != -1 && object->apiobj.character_model->model_data_b[LEGOACT_SLAM] != NULL) {
                if (Slam_Start(object, SLAMJUMPSPEED) != 0) {
                    PlaySabreSfx(NULL, object, NULL, 0);
                    if (object->action_movement_state == 1 || object->action_movement_state == 2) {
                        ResetAnimPacket(&object->apiobj.anim_packet, -1);
                        if (object->action_movement_state == 2 && (object->context_variant_flags & 0x20) == 0) {
                            object->apiobj.field_0x276 += 0x8000;
                            object->apiobj.facing_angle += 0x8000;
                            object->apiobj.movement_facing_angle += 0x8000;
                        }
                    }
                    return;
                }
            }
            const bool buffered_second_jump = (object->jump_input_flags & 0x10) != 0;
            const bool can_start_second_jump =
                (jump_pressed != 0 || buffered_second_jump) && (animation_set & 8) != 0 &&
                object->action_movement_state == PLAYER_JUMP_MOVEMENT_BASIC && object->jump_sequence <= 1 &&
                (object->apiobj.velocity.y > -1.25f || buffered_second_jump);
            if (can_start_second_jump) {
                if (LEGOCONTEXT_DOOMED != -1 && object->character_context == LEGOCONTEXT_DOOMED)
                    return;
                game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if ((game_character->flags_094[1] & 0x20) != 0) {
                    object->jump_variant_timer = 0.0f;
                    object->character_context = -1;
                    object->field_0xe31 = 1;
                    FastWeaponOut(object, 1);
                    return;
                }
                if ((animation_set & 0x110) == 0 && (game_character->flags_090 & 0x40400000) == 0)
                    return;
                if ((animation_set & 0x100) != 0) {
                    MakeJumpReachHeight(object, HIGHJUMPHEIGHT, 1);
                } else if ((animation_set & 0x200) != 0) {
                    MakeJumpReachHeight(object, game_character->jump_height, 0);
                } else if (DoubleJump_AlwaysReachJump2Height != 0 || object->apiobj.velocity.y > 0.0f) {
                    MakeJumpReachHeight(object, game_character->second_jump_height, 0);
                } else {
                    object->apiobj.velocity.y = 0.0f;
                }
                if (CanGlideFn != NULL && CanGlideFn(object) && Glide_Start(object))
                    return;
                object->jump_sequence++;
                object->context_variant_flags |= 0x40;
                game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if ((animation_set & 0x110) == 0 && (game_character->flags_090 & 0x400000) == 0) {
                    if ((game_character->flags_090 & 0x40000000) != 0) {
                        object->action_movement_state = 8;
                        object->context_animation = LEGOACT_COMBATROLL_JUMP;
                        object->context_variant_flags &= ~0x80;
                        object->airborne_action_duration =
                            AnimDuration(object->id, LEGOACT_COMBATROLL_JUMP, 0.0f, 0.0f, 1);
                    }
                } else {
                    object->action_movement_state = 0;
                    i16 action;
                    if (LEGOACT_JUMP3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP3] != NULL)
                        action = LEGOACT_JUMP3;
                    else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_JUMP2 != -1 &&
                             object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_JUMP2] != NULL)
                        action = LEGOACT_EXTRA_JUMP2;
                    else if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                        action = LEGOACT_JUMP2;
                    else
                        action = LEGOACT_JUMP;
                    object->context_animation = action;
                    if ((object->apiobj.flags_low & 0x80) != 0)
                        Hint_SetComplete(0x606);
                }
                object->context_animation_timer = 0.0f;
                object->field_0xe22 |= 0x10;
                PlayJumpSfx(object, 1);
                if ((animation_set & 0x10) == 0) {
                    object->context_variant_flags |= 0x10;
                }
                return;
            }
            return;
        }
        if (player_flag != 0 && (object->action_movement_state != 5 || (object->apiobj.field_0x27d & 1) != 0))
            object->field_0xef9 |= 8;
        const bool restart_jump = jump_held != 0 && (object->field_0xe22 & 0x10) == 0 &&
                                  object->action_movement_state != 3 && object->action_movement_state != 4 &&
                                  object->action_movement_state != 5 && object->action_movement_state != 8;
        if (restart_jump) {
            if (object->action_movement_state == 1) {
                if (object->pad_gamepad->input_magnitude > 0.0f && (object->field_0xe22 & 0x20) != 0 &&
                    abs(RotDiff(object->apiobj.movement_facing_angle, object->current_input_angle)) > 0x6aaa)
                    force_flip = 1;
            } else if (object->action_movement_state == 2) {
                if (object->pad_gamepad->input_magnitude > 0.0f && (object->field_0xe22 & 0x20) != 0 &&
                    abs(RotDiff(object->apiobj.movement_facing_angle, object->current_input_angle)) < 0x1555)
                    force_flip = 1;
            }
        }
        {
            if (object->action_movement_state != 3) {
                if (object->action_movement_state != 4) {
                    if (object->action_movement_state == 8) {
                        if (LEGOCONTEXT_LAND_COMBATROLL != -1 &&
                            object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_LAND] != NULL) {
                            NewRumble(object->pad_gamepad->pad, 0.4f, 0);
                            object->character_context = LEGOCONTEXT_LAND_COMBATROLL;
                            object->field_0x7a3 = 0;
                            object->context_animation = LEGOACT_COMBATROLL_LAND;
                            object->context_animation_timer =
                                AnimDuration(object->id, LEGOACT_COMBATROLL_LAND, 0.0f, 0.0f, 1);
                            object->context_flags &= ~0x40;
                            object->jump_reentry_timer = 0.0f;
                            if (!restart_jump)
                                ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            object->landing_followup = 0;
                            if (jump_held != 0 && (object->field_0xe22 & 0x10) == 0)
                                object->landing_followup = 1;
                            else if (action_held != 0 && LEGOACT_COMBATROLL_FIRE != -1 &&
                                     object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_FIRE] != NULL)
                                object->landing_followup = 2;
                            PlayLandSfx(object, 4, 0);
                        } else {
                            object->jump_reentry_timer = 0.2f;
                            object->character_context = -1;
                            PlayLandSfx(object, 0, 0);
                        }
                    } else if (object->action_movement_state == 2 && LEGOCONTEXT_LAND_COMBOJUMP != -1 &&
                               LEGOACT_COMBOLAND != -1 &&
                               object->apiobj.character_model->model_data_b[LEGOACT_COMBOLAND] != NULL) {
                        if ((object->context_variant_flags & 0x20) == 0) {
                            object->apiobj.field_0x276 += 0x8000;
                            object->apiobj.facing_angle += 0x8000;
                            object->apiobj.movement_facing_angle += 0x8000;
                            if (!restart_jump)
                                ResetAnimPacket(&object->apiobj.anim_packet, -1);
                        }
                        object->jump_reentry_timer = 0.0f;
                        if (object->pad_gamepad->input_magnitude == 0.0f) {
                            object->character_context = LEGOCONTEXT_LAND_COMBOJUMP;
                            object->context_animation =
                                (object->context_variant_flags & 0x20) != 0 && LEGOACT_FLIPLAND != -1 &&
                                        object->apiobj.character_model->model_data_b[LEGOACT_FLIPLAND] != NULL
                                    ? LEGOACT_FLIPLAND
                                    : LEGOACT_COMBOLAND;
                            object->context_animation_timer =
                                AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
                        } else {
                            object->character_context = -1;
                        }
                        PlayLandSfx(object, 0, 0);
                    } else if (object->action_movement_state == 1 && LEGOCONTEXT_LAND_FLIP != -1 &&
                               LEGOACT_FLIPLAND != -1 &&
                               object->apiobj.character_model->model_data_b[LEGOACT_FLIPLAND] != NULL) {
                        object->jump_reentry_timer = 0.0f;
                        if (object->pad_gamepad->input_magnitude == 0.0f) {
                            object->character_context = LEGOCONTEXT_LAND_FLIP;
                            object->context_animation = LEGOACT_FLIPLAND;
                            object->context_animation_timer = AnimDuration(object->id, LEGOACT_FLIPLAND, 0.0f, 0.0f, 1);
                            if (!restart_jump)
                                ResetAnimPacket(&object->apiobj.anim_packet, -1);
                        } else {
                            object->character_context = -1;
                        }
                        PlayLandSfx(object, 0, 0);
                    } else {
                        bool second_landing = false;
                        if (object->action_movement_state == 0 && object->jump_sequence >= 2 &&
                            LEGOCONTEXT_LAND_JUMP2 != -1) {
                            i16 action = LEGOACT_LAND2;
                            if ((object->context_variant_flags & 0x40) != 0 &&
                                (action == -1 || object->apiobj.character_model->model_data_b[action] == NULL))
                                action = LEGOACT_LAND3;
                            second_landing =
                                action != -1 && object->apiobj.character_model->model_data_b[action] != NULL;
                        }
                        if (second_landing) {
                            object->jump_reentry_timer = 0.0f;
                            if (object->pad_gamepad->input_magnitude != 0.0f) {
                                object->character_context = -1;
                            } else if (object->fall_animation_timer >= 0.2f) {
                                StartFallLand(object, -1);
                            } else {
                                object->character_context = LEGOCONTEXT_LAND_JUMP2;
                                if (object->jump_second_jump && LEGOACT_LAND3 != -1 &&
                                    object->apiobj.character_model->model_data_b[LEGOACT_LAND3] != NULL)
                                    object->context_animation = LEGOACT_LAND3;
                                else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) &&
                                         LEGOACT_EXTRA_LAND2 != -1 &&
                                         object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_LAND2] != NULL)
                                    object->context_animation = LEGOACT_EXTRA_LAND2;
                                else
                                    object->context_animation = LEGOACT_LAND2;
                                object->context_animation_timer =
                                    AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
                                if (!restart_jump)
                                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            }
                        } else if ((object->action_movement_state == 0 || object->action_movement_state == 6 ||
                                    object->action_movement_state == 7 || object->action_movement_state == 9) &&
                                   LEGOCONTEXT_LAND_JUMP != -1 && LEGOACT_LAND != -1 &&
                                   object->apiobj.character_model->model_data_b[LEGOACT_LAND] != NULL &&
                                   object->pad_gamepad->input_magnitude == 0.0f) {
                            if (object->fall_animation_timer >= 0.2f) {
                                StartFallLand(object, -1);
                            } else {
                                object->character_context = LEGOCONTEXT_LAND_JUMP;
                                if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) &&
                                    LEGOACT_EXTRA_LAND != -1 &&
                                    object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_LAND] != NULL)
                                    object->context_animation = LEGOACT_EXTRA_LAND;
                                else
                                    object->context_animation = LEGOACT_LAND;
                                if (CanMagnetClimbFn != NULL && CanMagnetClimbFn(object) && LEGOACT_MAGNET_JUMP != -1 &&
                                    object->apiobj.character_model->model_data_b[LEGOACT_MAGNET_JUMP] != NULL)
                                    object->context_animation = LEGOACT_MAGNET_JUMP;
                                object->context_animation_timer =
                                    AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
                                object->jump_reentry_timer = object->jump_sequence > 1 ? 0.0f : 0.2f;
                                if (!restart_jump)
                                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            }
                        } else {
                            object->jump_reentry_timer = 0.2f;
                            object->character_context = -1;
                            if (object->action_movement_state == 5)
                                object->jump_chain_timer = 0.0f;
                        }
                        PlayLandSfx(object, 0, 0);
                    }
                } else {
                    if (LEGOCONTEXT_LAND_SLAM != -1 && LEGOACT_SLAMLAND != -1 &&
                        object->apiobj.character_model->model_data_b[LEGOACT_SLAMLAND] != NULL) {
                        GameCam_Judder(GameCam, 0.5f, 0, &object->apiobj.collision_position);
                        NewRumble(object->pad_gamepad->pad, 0.75f, 0);
                        object->slam_debris_effect = hit_effect;
                        if (Slam_GetDebrisFn != NULL)
                            object->slam_debris_effect = Slam_GetDebrisFn(object, hit_effect);
                        object->character_context = LEGOCONTEXT_LAND_SLAM;
                        object->context_animation = LEGOACT_SLAMLAND;
                        const f32 duration = AnimDuration(object->id, LEGOACT_SLAMLAND, 0.0f, 0.0f, 1);
                        object->context_animation_timer = duration <= 0.0f ? 0.75f : duration;
                        object->jump_reentry_timer = 0.0f;
                        if (!restart_jump) {
                            ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            ResetMiniAnimPacket(&object->mini_animation, -1);
                        }
                        PlayLandSfx(object, 2, 0);
                    } else {
                        object->character_context = -1;
                        object->jump_reentry_timer = 0.2f;
                        PlayLandSfx(object, 0, 0);
                    }
                }
            } else {
                if (!(LEGOCONTEXT_LAND_LUNGE != -1 && LEGOACT_LUNGELAND != -1 &&
                      object->apiobj.character_model->model_data_b[LEGOACT_LUNGELAND] != NULL)) {
                    if ((object->apiobj.character_data->model_flags & 8) == 0) {
                        Punch_Hit(object, object->force_target, PUNCHCHARGAP, PUNCHCHARGAP);
                        object->jump_reentry_timer = 0.2f;
                        object->character_context = -1;
                        if (object->action_movement_state == 5)
                            object->jump_chain_timer = 0.0f;
                    } else {
                        object->character_context = -1;
                        object->jump_reentry_timer = 0.2f;
                    }
                    PlayLandSfx(object, 0, 0);
                } else {
                    NewRumble(object->pad_gamepad->pad, 0.6f, 0);
                    object->character_context = LEGOCONTEXT_LAND_LUNGE;
                    object->context_animation = LEGOACT_LUNGELAND;
                    object->context_animation_timer = AnimDuration(object->id, LEGOACT_LUNGELAND, 0.0f, 0.0f, 1);
                    object->context_flags &= ~0x40;
                    object->jump_reentry_timer = 0.0f;
                    if (!restart_jump)
                        ResetAnimPacket(&object->apiobj.anim_packet, -1);
                    if ((object->apiobj.character_data->model_flags & 8) == 0 &&
                        AnimListFrame(object->apiobj.character_model, object->context_animation, 0) == 0.0f)
                        Punch_Hit(object, object->force_target, PUNCHCHARGAP, PUNCHCHARGAP);
                    PlayLandSfx(object, 1, 0);
                }
            }
        }
        if (restart_jump) {
            UpdateLastSafePosition(object);
            goto jump_takeoff;
        }
        return;
    }
}

