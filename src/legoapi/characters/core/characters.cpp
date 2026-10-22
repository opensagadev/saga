// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nu3d/nuportal.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "nu2api/numath/nutrig.h"
#include "gameapi/edtools/edui.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/area.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/core/render.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "batman.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nurndr.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nucore/numemory.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include <stddef.h>
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <stdlib.h>
#include <cstdio>
#include "nu2api/nu3d/nuspline.h"
#include <new>
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/characters/core/characters.h"

static CHARSCENE_s *CharScene_Area;

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.

void HairMovement(GameObject_s *object) {
    constexpr f32 gravity = -1.8325958f;
    u8 current_index;
    u32 state_index;
    u8 *state;

    const u8 index = object->field_0x1089;
    if (index > 2) {
        return;
    }
    if (object->apiobj.character_data->game_character->cloak_joint_2 == -1) {
        return;
    }

    f32 phase = NuFsqrt(*reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(object) + index * 0x34 + 0xf50) / gravity);
    if (LEGOACT_FALL == -1 ||
        (object->apiobj.anim_packet.blending != 0 ? LEGOACT_FALL != object->apiobj.anim_packet.blend_animation_b
                                                  : LEGOACT_FALL != object->apiobj.anim_packet.animation_index)) {
        phase = phase - (FRAMETIME + FRAMETIME);
        if (phase < 0.0f) {
            current_index = object->field_0x1089;
            state_index = current_index;
            state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
            const u8 joint = static_cast<u8>(object->apiobj.character_data->game_character->cloak_joint_2);
            *reinterpret_cast<f32 *>(state + 0xf50) = 0.0f;
            state[0xf78] = joint;
            goto increment_index;
        }
    } else {
        phase = FRAMETIME + FRAMETIME + phase;
        if (phase > 1.0f) {
            current_index = object->field_0x1089;
            state_index = current_index;
            state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
            state[0xf78] = static_cast<u8>(object->apiobj.character_data->game_character->cloak_joint_2);
            *reinterpret_cast<f32 *>(state + 0xf50) = gravity;
            goto setup_state;
        }
    }

    {
        const f32 squared = phase * phase;
        const f32 value = gravity * squared + (1.0f - squared) * 0.0f;
        current_index = object->field_0x1089;
        state_index = current_index;
        state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
        state[0xf78] = static_cast<u8>(object->apiobj.character_data->game_character->cloak_joint_2);
        *reinterpret_cast<f32 *>(state + 0xf50) = value;
        if (value == 0.0f) {
            goto increment_index;
        }
    }

setup_state:
    state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
    state[0xf79] = 0x21;
    *reinterpret_cast<i16 *>(state + 0xf70) = 0;
    *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(0xb556);

increment_index:
    object->field_0x1089 = current_index + 1;
}

void HeadMovement(GameObject_s *object) {
    f32 previous;
    NUVEC direction;
    if (CInfo[object->character_context].flags & 0x200000) {
        return;
    }
    if (!(object->apiobj.character_data->game_character->field_0x94 & 0x40000000) &&
        !(object->apiobj.character_data->model_flags & 0x20000)) {
        CHARACTERDATA *character = object->apiobj.character_data;

        if (object->field_0x1089 < 3 && character->game_character->head_joint != -1) {
            object->field_0xefe &= ~1;
            i32 allowed;
            i16 animation;
            if (!object->apiobj.anim_packet.blending) {
                animation = object->apiobj.anim_packet.animation_index;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_ordinary;
            } else {
                animation = object->apiobj.anim_packet.blend_animation_b;
                if (animation >= 0 && animation < apicharsys->model_id_capacity &&
                    object->apiobj.character_model->model_data_b[animation] != NULL) {
                    if (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                        0x40) {
                        allowed = 0;
                        goto gate_done_ordinary;
                    }
                }
                animation = object->apiobj.anim_packet.blend_animation_a;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_ordinary;
            }

            if (animation < apicharsys->model_id_capacity &&
                object->apiobj.character_model->model_data_b[animation] != NULL) {
                allowed =
                    (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                     0x40) == 0;
            }
        gate_done_ordinary:;

            f32 first = 0.0f;
            f32 second = 0.0f;
            if (object->head_target != NULL && object->head_target_timer >= object->head_target_delay &&
                character->game_character->head_locator != -1 && allowed) {
                object->field_0xefe |= 1;

                NuVecInvMtxTransform(&direction, &object->head_target_position,
                                     &object->joint_matrices[character->game_character->head_locator]);
                if (direction.z < 0.0f) {
                    f32 angle = NuAtan2(-direction.y, NuFsqrt(direction.x * direction.x + direction.z * direction.z));
                    first = object->joint_modifiers[object->field_0x1089].rotation.y - angle;
                    f32 limit = character->game_character->field_0x54;
                    if (first >= limit)
                        first = limit;
                    else if (first <= -limit)
                        first = -limit;
                    angle = NuAtan2(-direction.x, -direction.z);
                    second = angle + object->joint_modifiers[object->field_0x1089].rotation.z;
                    limit = character->game_character->field_0x50;
                    if (second >= limit)
                        second = limit;
                    else if (second <= -limit)
                        second = -limit;
                    else
                        object->field_0xefe &= ~1;
                }
            }
            object->joint_modifiers[object->field_0x1089].joint_index = character->game_character->head_joint;
            object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(0);
            f32 blend = character->game_character->field_0x58 * FRAMETIME;

            if (blend > 1.0f) {
                blend = 1.0f;
                previous = 0.0f;
            } else {
                previous = 1.0f - blend;
            }
            object->joint_modifiers[object->field_0x1089].rotation.x = 0.0f;
            object->joint_modifiers[object->field_0x1089].rotation.y =
                first * blend + object->joint_modifiers[object->field_0x1089].rotation.y * previous;
            object->joint_modifiers[object->field_0x1089].rotation.z =
                second * blend + object->joint_modifiers[object->field_0x1089].rotation.z * previous;
            i32 limit = static_cast<i32>(character->game_character->field_0x54 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[1] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[1] = -limit;
            limit = static_cast<i32>(character->game_character->field_0x50 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[2] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[2] = -limit;
            if (object->joint_modifiers[object->field_0x1089].rotation.y > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.y < -0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z < -0.0017453293548896909f) {
                object->joint_modifiers[object->field_0x1089].flags = NUJOINTANIM_ROTATION;
                if (allowed) {
                    if (object->joint_modifiers[object->field_0x1089].rotation.y != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags =
                            static_cast<NUJOINTANIM_FLAGS>(NUJOINTANIM_ROTATION | NUJOINTANIM_LIMIT_ROTATION_Y);
                    if (object->joint_modifiers[object->field_0x1089].rotation.z != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(
                            object->joint_modifiers[object->field_0x1089].flags | NUJOINTANIM_LIMIT_ROTATION_Z);
                }
            }
            if (object->head_target != NULL) {
                object->head_target_timer -= FRAMETIME;
                if (object->head_target_timer < 0.0f) {
                    object->head_target_timer = 0.0f;
                    object->head_target_delay = 0.0f;
                    object->head_target = NULL;
                    object->head_target_priority = 0;
                }
            }
            object->field_0x1089 = object->field_0x1089 + 1;
        }
    } else if (!(object->apiobj.character_data->game_character->field_0x94 & 0x40000000)) {
        CHARACTERDATA *character = object->apiobj.character_data;

        if (object->field_0x1089 > 2 || character->game_character->head_joint == -1) {
            return;
        }

        i32 allowed;
        i16 animation;
        if (!object->apiobj.anim_packet.blending) {
            animation = object->apiobj.anim_packet.animation_index;
            allowed = 1;
            if (animation < 0)
                goto gate_done_old;
        } else {
            animation = object->apiobj.anim_packet.blend_animation_b;
            if (animation >= 0 && animation < apicharsys->model_id_capacity &&
                object->apiobj.character_model->model_data_b[animation] != NULL) {
                if (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                    0x40) {
                    allowed = 0;
                    goto gate_done_old;
                }
            }
            animation = object->apiobj.anim_packet.blend_animation_a;
            allowed = 1;
            if (animation < 0)
                goto gate_done_old;
        }

        if (animation < apicharsys->model_id_capacity &&
            object->apiobj.character_model->model_data_b[animation] != NULL) {
            allowed = (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                       0x40) == 0;
        }
    gate_done_old:;

        f32 first = 0.0f;
        f32 second = 0.0f;
        if (object->head_target != NULL && object->head_target_timer >= object->head_target_delay &&
            character->game_character->head_locator != -1 && allowed) {

            NuVecInvMtxTransform(&direction, &object->head_target_position,
                                 &object->joint_matrices[character->game_character->head_locator]);
            if (direction.z < 0.0f) {
                f32 angle = NuAtan2(-direction.x, -direction.z);
                first = object->joint_modifiers[object->field_0x1089].rotation.x - angle;
                f32 limit = character->game_character->field_0x50;
                if (first >= limit)
                    first = limit;
                else if (first <= -limit)
                    first = -limit;
                angle = NuAtan2(-direction.y, -direction.z);
                second = object->joint_modifiers[object->field_0x1089].rotation.y - angle;
                limit = character->game_character->field_0x54;
                if (second >= limit)
                    second = limit;
                else if (second <= -limit)
                    second = -limit;
            }
        }
        object->joint_modifiers[object->field_0x1089].joint_index = character->game_character->head_joint;
        object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(0);
        f32 blend = character->game_character->field_0x58 * FRAMETIME;
        f32 previous;
        if (blend > 1.0f) {
            blend = 1.0f;
            previous = 0.0f;
        } else {
            previous = 1.0f - blend;
        }
        object->joint_modifiers[object->field_0x1089].rotation.z = 0.0f;
        object->joint_modifiers[object->field_0x1089].rotation.x =
            first * blend + object->joint_modifiers[object->field_0x1089].rotation.x * previous;
        object->joint_modifiers[object->field_0x1089].rotation.y =
            second * blend + object->joint_modifiers[object->field_0x1089].rotation.y * previous;
        i32 limit = static_cast<i32>(character->game_character->field_0x50 * 10430.3779296875f);
        object->joint_modifiers[object->field_0x1089].rotation_limit_start[0] = limit;
        object->joint_modifiers[object->field_0x1089].rotation_limit_end[0] = -limit;
        limit = static_cast<i32>(character->game_character->field_0x54 * 10430.3779296875f);
        object->joint_modifiers[object->field_0x1089].rotation_limit_start[1] = limit;
        object->joint_modifiers[object->field_0x1089].rotation_limit_end[1] = -limit;
        if (object->joint_modifiers[object->field_0x1089].rotation.x > 0.0017453293548896909f ||
            object->joint_modifiers[object->field_0x1089].rotation.x < -0.0017453293548896909f ||
            object->joint_modifiers[object->field_0x1089].rotation.y > 0.0017453293548896909f ||
            object->joint_modifiers[object->field_0x1089].rotation.y < -0.0017453293548896909f) {
            object->joint_modifiers[object->field_0x1089].flags = NUJOINTANIM_ROTATION;
            if (allowed) {
                if (object->joint_modifiers[object->field_0x1089].rotation.x != 0.0f)
                    object->joint_modifiers[object->field_0x1089].flags =
                        static_cast<NUJOINTANIM_FLAGS>(NUJOINTANIM_ROTATION | NUJOINTANIM_LIMIT_ROTATION_X);
                if (object->joint_modifiers[object->field_0x1089].rotation.y != 0.0f)
                    object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(
                        object->joint_modifiers[object->field_0x1089].flags | NUJOINTANIM_LIMIT_ROTATION_Y);
            }
        }
        if (object->head_target != NULL) {
            object->head_target_timer -= FRAMETIME;
            if (object->head_target_timer < 0.0f) {
                object->head_target_timer = 0.0f;
                object->head_target_delay = 0.0f;
                object->head_target = NULL;
                object->head_target_priority = 0;
            }
        }
        object->field_0x1089 = object->field_0x1089 + 1;
    } else {
        if (object->field_0x1089 < 3 && object->apiobj.character_data->game_character->head_joint != -1) {
            object->field_0xefe &= ~1;
            i32 allowed;
            i16 animation;
            if (!object->apiobj.anim_packet.blending) {
                animation = object->apiobj.anim_packet.animation_index;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_alternate;
            } else {
                animation = object->apiobj.anim_packet.blend_animation_b;
                if (animation >= 0 && animation < apicharsys->model_id_capacity &&
                    object->apiobj.character_model->model_data_b[animation] != NULL) {
                    if (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                        0x40) {
                        allowed = 0;
                        goto gate_done_alternate;
                    }
                }
                animation = object->apiobj.anim_packet.blend_animation_a;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_alternate;
            }

            if (animation < apicharsys->model_id_capacity &&
                object->apiobj.character_model->model_data_b[animation] != NULL) {
                allowed =
                    (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                     0x40) == 0;
            }
        gate_done_alternate:;

            f32 first = 0.0f;
            f32 second = 0.0f;
            if (object->head_target != NULL && object->head_target_timer >= object->head_target_delay &&
                object->apiobj.character_data->game_character->head_locator != -1 && allowed) {
                object->field_0xefe |= 1;

                NuVecInvMtxTransform(
                    &direction, &object->head_target_position,
                    &object->joint_matrices[object->apiobj.character_data->game_character->head_locator]);
                if (direction.z < 0.0f) {
                    f32 angle = NuAtan2(-direction.y, NuFsqrt(direction.x * direction.x + direction.z * direction.z));
                    first = angle - object->joint_modifiers[object->field_0x1089].rotation.y;
                    f32 limit = object->apiobj.character_data->game_character->field_0x54;
                    if (first >= limit)
                        first = limit;
                    else if (first <= -limit)
                        first = -limit;
                    angle = NuAtan2(-direction.x, -direction.z);
                    second = -angle - object->joint_modifiers[object->field_0x1089].rotation.z;
                    limit = object->apiobj.character_data->game_character->field_0x50;
                    if (second >= limit)
                        second = limit;
                    else if (second <= -limit)
                        second = -limit;
                    else
                        object->field_0xefe &= ~1;
                }
            }
            CHARACTERDATA *character = object->apiobj.character_data;
            object->joint_modifiers[object->field_0x1089].joint_index = character->game_character->head_joint;
            object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(0);
            f32 blend = object->apiobj.character_data->game_character->field_0x58 * FRAMETIME;
            f32 previous;
            if (blend > 1.0f) {
                blend = 1.0f;
                previous = 0.0f;
            } else {
                previous = 1.0f - blend;
            }
            object->joint_modifiers[object->field_0x1089].rotation.x = 0.0f;
            object->joint_modifiers[object->field_0x1089].rotation.y =
                first * blend + object->joint_modifiers[object->field_0x1089].rotation.y * previous;
            object->joint_modifiers[object->field_0x1089].rotation.z =
                second * blend + object->joint_modifiers[object->field_0x1089].rotation.z * previous;
            i32 limit = static_cast<i32>(object->apiobj.character_data->game_character->field_0x54 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[1] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[1] = -limit;
            limit = static_cast<i32>(object->apiobj.character_data->game_character->field_0x50 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[2] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[2] = -limit;
            if (object->joint_modifiers[object->field_0x1089].rotation.y > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.y < -0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z < -0.0017453293548896909f) {
                object->joint_modifiers[object->field_0x1089].flags = NUJOINTANIM_ROTATION;
                if (allowed) {
                    if (object->joint_modifiers[object->field_0x1089].rotation.y != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags =
                            static_cast<NUJOINTANIM_FLAGS>(NUJOINTANIM_ROTATION | NUJOINTANIM_LIMIT_ROTATION_Y);
                    if (object->joint_modifiers[object->field_0x1089].rotation.z != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(
                            object->joint_modifiers[object->field_0x1089].flags | NUJOINTANIM_LIMIT_ROTATION_Z);
                }
            }
            if (object->head_target != NULL) {
                object->head_target_timer -= FRAMETIME;
                if (object->head_target_timer < 0.0f) {
                    object->head_target_timer = 0.0f;
                    object->head_target_delay = 0.0f;
                    object->head_target = NULL;
                    object->head_target_priority = 0;
                }
            }
            object->field_0x1089 = object->field_0x1089 + 1;
        }
    }
}

void ResetCharacterBuffer(i32 force_reset) {
    if (force_reset == 0 && Area != -1 && Area == last_area) {
        return;
    }

    characterbuffer_ptr = characterbuffer_base;
    memset(characterbuffer_base.void_ptr, 0, CHARACTERBUFFERSIZE);
    apicharsys->loaded_model_count = apicharsys->permanent_model_count;
    apicharsys->animation_load_attempts = apicharsys->area_animation_count;
    apicharsys->loaded_animation_count = apicharsys->area_animation_count;
}

static NUGSCN **IconScene;
static char IconPath[0x40] = "stuff\\icons\\";

struct nuhspecial_s;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern EdInputContext * Input;

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
} // extern "C"

union variptr_u;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

i32 CharIDFromName(char *name) {
    for (i32 i = 0; i < CHARCOUNT; i++) {
        if (NuStrICmp(CDataList[i].file, name) == 0) {
            return i;
        }
    }

    return -1;
}

CHARACTERDATA *CDataFromName(char *name) {
    for (i32 i = 0; i < CHARCOUNT; i++) {
        if (NuStrICmp(CDataList[i].file, name) == 0) {
            return &CDataList[i];
        }
    }

    return nullptr;
}

// Retail copies the first argument into the fourth and appends the animation name there.
// Retail also returns 0 for a NULL redirect table; that check is not reproduced yet.
i32 RedirectAnim(char *directory, ANIMREDIRECT *redirects, ANIMLIST_s *animation_list, char *path) {
    CHARACTERANIM_s *animation = reinterpret_cast<CHARACTERANIM_s *>(animation_list);
    for (ANIMREDIRECT *redirect = redirects; redirect->name != NULL; ++redirect) {
        if (!(redirect->animation_id == animation->animation_id && NuStrICmp(redirect->name, animation->name) == 0)) {
            continue;
        }
        NuStrCpy(path, directory);
        NuStrCat(path, animation->name);
        animation->flags &= ~CHARACTER_ANIMATION_FLAG_BSA;
        return 1;
    }
    return 0;
}

void CharScenes_Init(variptr_u *buf, variptr_u *) {
    const usize size = static_cast<usize>(CHARCOUNT) * sizeof(*CharScene_Area);
    const usize area = ALIGN(buf->addr, 4);
    CharScene_Area = reinterpret_cast<CHARSCENE_s *>(area);
    buf->addr = area + size;
    memset(reinterpret_cast<void *>(area), 0, size);
}

void CharScenes_LevelLoad(WORLDINFO *world) {
    if (CHARCOUNT <= 0) {
        return;
    }

    for (i32 i = 0; i < CHARCOUNT; i++) {
        CHARSCENE_s *entry = &world->minikit.character_scenes[i];
        entry->scene = NULL;

        // Check if we should load this character scene
        if ((CharScene_Area == NULL || CharScene_Area[i].scene == NULL) && (CDataList[i].flags & 1) != 0 &&
            world->cutscene_sys != NULL) {
            // Check if this character is in a cutscene
            u32 *cutscene_flags = *(u32 **)((char *)world->cutscene_sys + 8);
            u32 flag = (cutscene_flags[i >> 5] >> (i & 0x1f)) & 1;
            if (flag != 0) {
                // Load the character scene
                char path[128];
                VARIPTR buf_end = world->unknown_0108;
                sprintf(path, "chars\\%s\\%s.gsc", CDataList[i].dir, CDataList[i].file);
                NUGSCN *scene = NuGScnRead(&world->giz_buffer, buf_end, path);
                entry->scene = scene;
                if (scene != NULL) {
                    NuSpecialFind(scene, &entry->special_scene, CDataList[i].file, 1);
                }
            }
        }
    }
}

void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *list, variptr_u *buf, variptr_u buf_end) {
    char path[128];
    if (CharScene_Area == NULL) {
        return;
    }

    for (i32 i = 0; i < apicharsys->loaded_model_count; ++i) {
        const i32 model_id = apicharsys->models[i].model_id;
        CHARSCENE_s &entry = CharScene_Area[model_id];
        if (entry.scene == NULL && (CDataList[model_id].flags & 1) != 0) {
            // The original reuses the loop counter as InModelList's output index.
            if (InModelList(list, model_id, &i) == 0 || list[i].count == 0) {
                continue;
            }

            sprintf(path, "chars\\%s\\%s.gsc", CDataList[model_id].dir, CDataList[model_id].file);
            entry.scene = NuGScnRead(buf, buf_end, path);
            if (entry.scene != NULL) {
                NuSpecialFind(entry.scene, &entry.special_scene, CDataList[model_id].file, 1);
            }
        }
    }
}

nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id) {
    nuhspecial_s *special;
    if (__builtin_expect(CharScene_Area != NULL && CharScene_Area[character_id].scene != NULL, 1)) {
        special = &CharScene_Area[character_id].special_scene;
    } else if (world->minikit.character_scenes[character_id].scene != NULL) {
        special = &world->minikit.character_scenes[character_id].special_scene;
    } else {
        return NULL;
    }
    if (NuSpecialExistsFn(special) == 0) {
        special = NULL;
    }
    return special;
}

void CharScene_Draw(WORLDINFO_s *world, i32 character_id, numtx_s *matrix, numtx_s *reflection_matrix) {
    nuhspecial_s *special = CharScene_FindHSpecial(world, character_id);
    if (special != NULL) {
        if (matrix != NULL) {
            NuSpecialDrawAt(special, matrix);
        }
        if (reflection_matrix != NULL) {
            NuSpecialDrawAt(special, reflection_matrix);
        }
    }
}

void CharScenes_LevelDump(WORLDINFO_s *world) {
    CHARSCENE_s *scenes = world->minikit.character_scenes;
    if (scenes != NULL) {
        for (i32 i = 0; i < CHARCOUNT; ++i) {
            if (world->minikit.character_scenes[i].scene != NULL) {
                NuGScnRemove(world->minikit.character_scenes[i].scene);
            }
            world->minikit.character_scenes[i].scene = NULL;
        }
    }
}

void CharScenes_AreaDump() {
    if (CharScene_Area == NULL) {
        return;
    }
    for (i32 i = 0; i < CHARCOUNT; ++i) {
        if (CharScene_Area[i].scene != NULL) {
            NuGScnRemove(CharScene_Area[i].scene);
        }
        CharScene_Area[i].scene = NULL;
    }
}

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *) {
    const usize table_size = static_cast<usize>(CHARCOUNT) * sizeof(*IconScene);
    IconScene = reinterpret_cast<NUGSCN **>(ALIGN(buf->addr, 4));
    buf->addr = reinterpret_cast<usize>(IconScene) + table_size;
    memset(IconScene, 0, table_size);

    if (path != NULL && NuStrLen(path) <= 0x3f) {
        NuStrCpy(IconPath, path);
    }
}

NUGSCN *IconScene_FindById(i32 character_id) {
    if (character_id < 0 || character_id >= CHARCOUNT) {
        return NULL;
    }
    return IconScene[character_id];
}

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end) {
    if (list == NULL || IconScene == NULL) {
        return;
    }

    for (; list->model_id != -1; ++list) {
        const i32 character_id = list->model_id;
        if (IconScene[character_id] != NULL || list->count == 0) {
            continue;
        }

        const i32 object_index = CDataList[character_id].field20_0x42;
        if (object_index == -1) {
            continue;
        }

        char path[0x100];
        NuStrCpy(path, IconPath);
        NuStrCat(path, LevelObject_FindNameFromIndex(object_index));
        NuStrCat(path, ".gsc");
        IconScene[character_id] = NuGScnRead(buf, *buf_end, path);
        if (IconScene[character_id] != NULL && IconScene[character_id]->display_list != NULL) {
            IconScene[character_id]->display_list->flags |= NU_DISPLAYSCENE_FLAG_NEEDS_BUILD;
        }
    }
}

void IconScenes_Dump() {
    if (IconScene == NULL) {
        return;
    }

    for (i32 i = 0; i < CHARCOUNT; ++i) {
        if (IconScene[i] != NULL) {
            NuGScnRemove(IconScene[i]);
            IconScene[i] = NULL;
        }
    }
}

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    const u16 character_spin = static_cast<u16>(drawcharicon_hspecial_spin);
    drawcharicon_hspecial_spin = 0;

    if (ObjTabList == NULL || world == NULL) {
        return;
    }

    nuhspecial_s special;
    if (drawcharicon_find != 0) {
        NuSpecialFind(things_scene, &special, ObjTabList[frame_object_id].name, 1);
    } else {
        special = world->lev_objs[frame_object_id].special;
    }

    if (NuSpecialExistsFn(&special) != 0) {
        DrawPanel3DObject(x, y, z + 1.0f, scale, scale, scale, 0, 0, 0, &special, 0, frame_alpha);
    }

    if (draw_character != 0) {
        if (override_special != NULL && NuSpecialExistsFn(override_special) != 0) {
            const f32 special_scale = scale * drawcharicon_hspecial_scale;
            DrawPanel3DObject(x, y, z + 1.0f + drawcharicon_hspecial_dz, special_scale, special_scale, special_scale, 0,
                              character_spin, 0, override_special, 0, character_alpha);
        } else {
            i32 icon_object_id;
            if (character_id == -1) {
                icon_object_id = LEGOOBJ_ICON_QUESTION;
            } else {
                icon_object_id = CDataList[character_id].field20_0x42;
                if (icon_object_id != -1 && GCDataList[character_id].field275_0x116 == 0 &&
                    icon_object_id != LEGOOBJ_ICON_WEIRDO) {
                    ++icon_object_id;
                }
            }

            if (icon_object_id != -1) {
                if (drawcharicon_find != 0) {
                    NUGSCN *icon_scene = character_id == -1 ? NULL : IconScene_FindById(character_id);
                    if (icon_scene != NULL) {
                        NuSpecialFind(icon_scene, &special, ObjTabList[icon_object_id].name, 1);
                    } else {
                        NuSpecialFind(things_scene, &special, ObjTabList[icon_object_id].name, 1);
                    }
                } else {
                    special = world->lev_objs[icon_object_id].special;
                }

                if (NuSpecialExistsFn(&special) != 0) {
                    DrawPanel3DObject(x, y, z + 1.0f, scale, scale, scale, 0, 0, 0, &special, 0, character_alpha);
                }
            }
        }
    }

    drawcharicon_find = 0;
    drawcharicon_i_panel = -1;
}

