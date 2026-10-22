// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/world/levels/levels.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include "legoapi/props/doors/door.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nuvec4.h"
#include <limits.h>
#include "legoapi/lib/camera.h"

struct GAMECAMERA_s;

static inline void MakePlayCorner(GAMECAMERA_s *camera, f32 screen_x, f32 screen_y, NUVEC *near_corner,
                                  NUVEC *far_corner) {
    NUVEC ray = {screen_x, screen_y, 1.0f};
    NuVecMtxRotate(&ray, &ray, &camera->target_mtx);
    NuVecAdd(near_corner, &ray, &camera->pos);
    NuVecNorm(&ray, &ray);
    far_corner->y = near_corner->y + ray.y * 5.0f;
    far_corner->z = near_corner->z + ray.z * 5.0f;
    far_corner->x = near_corner->x + ray.x * 5.0f;
}

static inline void SetPlayPlane(PLAYPLANE_s *plane, NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *d, NUVEC *edge_a0,
                                NUVEC *edge_a1, NUVEC *edge_b0, NUVEC *edge_b1) {
    plane->point.x = (a->x + b->x + c->x + d->x) * 0.25f;
    plane->point.y = (a->y + b->y + c->y + d->y) * 0.25f;
    plane->point.z = (a->z + b->z + c->z + d->z) * 0.25f;
    NUVEC second_edge;
    NUVEC first_edge;
    NuVecSub(&first_edge, edge_a0, edge_a1);
    NuVecSub(&second_edge, edge_b0, edge_b1);
    NuVecCross(&plane->normal, &first_edge, &second_edge);
    NuVecNorm(&plane->normal, &plane->normal);
}

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

static NARROWSOCKEXCEPTION *NarrowSockException;

static inline i32 NarrowSockPositionAllowed(GameObject_s *object, SOCKSYS *system, i32 level_index) {
    if (system == NULL || object->sock_position.location.sock == -1)
        return 0;
    if ((system->sock[object->sock_position.location.sock].flags & 0x800) == 0)
        return 0;
    if (NarrowSockException != NULL) {
        for (NARROWSOCKEXCEPTION *exception = NarrowSockException; exception->level_name != NULL; ++exception) {
            if (exception->level_index == -1 || exception->level_index != level_index ||
                exception->sock_index != object->sock_position.location.sock)
                continue;
            if (object->sock_position.distance >= exception->start_distance &&
                object->sock_position.distance <= exception->end_distance)
                return 0;
        }
    }
    return 1;
}

void GameCam_ResetLookRot(GAMECAMERA_s *camera) {
    if (camera == NULL) {
        camera = GameCam;
    }

    camera->field_0x214 = 0.0f;
    camera->field_0x218 = 0.0f;
    camera->field_0x20c = 0.0f;
    camera->field_0x210 = 0.0f;
    camera->field_0x204 = 0.0f;
    camera->field_0x208 = 0.0f;
}

void GameCam_UpdateLookRot(GAMECAMERA_s *camera) {
    // Retail handles each player separately, with directional buttons only for
    // the left-stick mode and seek speeds proportional to the angular ranges.
    if (__builtin_expect(camera == NULL, 0))
        camera = GameCam;
    f32 look_pitch = 1820.0f, look_yaw = 2730.0f;
    camera->field_0x204 = 0.0f;
    camera->field_0x208 = 0.0f;
    f32 contributing_players = 0.0f;
    if (MiniCutCam == 0) {
        if (Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.flags_low) < 0 &&
            Player[0]->pad_gamepad->pad != NULL) {
            i32 look_source;
            if (__builtin_expect(GameCam_ObjLookingWithLeftStick == NULL, 0))
                goto player0_right_look;
            look_source = GameCam_ObjLookingWithLeftStick(Player[0]);
            if (look_source != 1 && look_source != 2)
                goto player0_look_done;
            if (look_source == 2) {
                f32 horizontal = 0.0f, vertical = 0.0f;
                if (Player[0]->pad_gamepad->input_mode == 1) {
                    u32 buttons = Player[0]->pad_gamepad->buttons_held;
                    u32 horizontal_buttons = buttons & (GAMEPAD_DLEFT | GAMEPAD_DRIGHT);
                    if (horizontal_buttons == GAMEPAD_DLEFT)
                        horizontal = -look_yaw;
                    else if (__builtin_expect(horizontal_buttons == GAMEPAD_DRIGHT, 0))
                        horizontal = look_yaw;
                    u32 vertical_buttons = buttons & (GAMEPAD_DUP | GAMEPAD_DDOWN);
                    if (vertical_buttons == GAMEPAD_DUP)
                        vertical = -look_pitch;
                    else if (__builtin_expect(vertical_buttons == GAMEPAD_DDOWN, 0))
                        vertical = look_pitch;
                } else {
                    nupad_s *pad = Player[0]->pad_gamepad->pad;
                    vertical = (static_cast<f32>(static_cast<u32>(pad->analog_left_y)) - 127.5f) * (1.0f / 127.5f) *
                               look_pitch;
                    horizontal =
                        (static_cast<f32>(static_cast<u32>(pad->analog_left_x)) - 127.5f) * (1.0f / 127.5f) * look_yaw;
                }
                camera->field_0x204 += vertical;
                camera->field_0x208 += horizontal;
            } else if (look_source == 1) {
            player0_right_look:
                nupad_s *pad = Player[0]->pad_gamepad->pad;
                f32 horizontal = static_cast<f32>(static_cast<u32>(pad->analog_right_x));
                f32 vertical = static_cast<f32>(static_cast<u32>(pad->analog_right_y));
                camera->field_0x208 += (horizontal - 127.5f) * (1.0f / 127.5f) * look_yaw;
                camera->field_0x204 += (vertical - 127.5f) * (1.0f / 127.5f) * look_pitch;
            }
            contributing_players += 1.0f;
        player0_look_done:;
        }
        if (Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.flags_low) < 0 &&
            Player[1]->pad_gamepad->pad != NULL) {
            i32 look_source;
            if (GameCam_ObjLookingWithLeftStick == NULL)
                goto player1_right_look;
            look_source = GameCam_ObjLookingWithLeftStick(Player[1]);
            if (look_source != 1 && look_source != 2)
                goto player1_look_done;
            if (look_source == 2) {
                f32 horizontal = 0.0f, vertical = 0.0f;
                if (Player[1]->pad_gamepad->input_mode == 1) {
                    u32 buttons = Player[1]->pad_gamepad->buttons_held;
                    u32 horizontal_buttons = buttons & (GAMEPAD_DLEFT | GAMEPAD_DRIGHT);
                    if (__builtin_expect(horizontal_buttons == GAMEPAD_DLEFT, 1))
                        horizontal = -look_yaw;
                    else if (horizontal_buttons == GAMEPAD_DRIGHT)
                        horizontal = look_yaw;
                    u32 vertical_buttons = buttons & (GAMEPAD_DUP | GAMEPAD_DDOWN);
                    if (vertical_buttons == GAMEPAD_DUP)
                        vertical = -look_pitch;
                    else if (vertical_buttons == GAMEPAD_DDOWN)
                        vertical = look_pitch;
                } else {
                    nupad_s *pad = Player[1]->pad_gamepad->pad;
                    vertical = (static_cast<f32>(static_cast<u32>(pad->analog_left_y)) - 127.5f) * (1.0f / 127.5f) *
                               look_pitch;
                    horizontal =
                        (static_cast<f32>(static_cast<u32>(pad->analog_left_x)) - 127.5f) * (1.0f / 127.5f) * look_yaw;
                }
                camera->field_0x204 += vertical;
                camera->field_0x208 += horizontal;
            } else if (look_source == 1) {
            player1_right_look:
                nupad_s *pad = Player[1]->pad_gamepad->pad;
                f32 horizontal = static_cast<f32>(static_cast<u32>(pad->analog_right_x));
                f32 vertical = static_cast<f32>(static_cast<u32>(pad->analog_right_y));
                camera->field_0x208 += (horizontal - 127.5f) * (1.0f / 127.5f) * look_yaw;
                camera->field_0x204 += (vertical - 127.5f) * (1.0f / 127.5f) * look_pitch;
            }
            contributing_players += 1.0f;
        player1_look_done:;
        }
    }
    if (contributing_players > 1.0f) {
        f32 inverse_count = 1.0f / contributing_players;
        camera->field_0x204 *= inverse_count;
        camera->field_0x208 *= inverse_count;
    }
    camera->field_0x20c = SeekLinearF(camera->field_0x20c, camera->field_0x204, (FRAMETIME * look_pitch) * 2.0f);
    camera->field_0x210 = SeekLinearF(camera->field_0x210, camera->field_0x208, (FRAMETIME * look_yaw) * 2.0f);
    camera->field_0x214 = SeekValF(camera->field_0x214, camera->field_0x20c, 3.0f);
    camera->field_0x218 = SeekValF(camera->field_0x218, camera->field_0x210, 3.0f);
}

void MakePlayPlanes(GAMECAMERA_s *camera) {
    NUVEC far_corner[4];
    NUVEC near_corner[4];
    MakePlayCorner(camera, -0.85f * PANEL3DMULX, 0.85f * PANEL3DMULY, &near_corner[0], &far_corner[0]);
    MakePlayCorner(camera, 0.85f * PANEL3DMULX, 0.85f * PANEL3DMULY, &near_corner[1], &far_corner[1]);
    MakePlayCorner(camera, -0.85f * PANEL3DMULX, -0.85f * PANEL3DMULY, &near_corner[2], &far_corner[2]);
    MakePlayCorner(camera, 0.85f * PANEL3DMULX, -0.85f * PANEL3DMULY, &near_corner[3], &far_corner[3]);

    // Plane order is near, left, right, top, bottom, far.
    SetPlayPlane(&PlayPlane[1], &near_corner[0], &far_corner[0], &far_corner[2], &near_corner[2], &far_corner[0],
                 &near_corner[0], &far_corner[2], &near_corner[0]);
    SetPlayPlane(&PlayPlane[2], &far_corner[1], &near_corner[1], &near_corner[3], &far_corner[3], &near_corner[1],
                 &far_corner[1], &near_corner[3], &far_corner[1]);
    SetPlayPlane(&PlayPlane[3], &near_corner[0], &near_corner[1], &far_corner[1], &far_corner[0], &near_corner[1],
                 &near_corner[0], &far_corner[1], &near_corner[0]);
    SetPlayPlane(&PlayPlane[4], &near_corner[2], &far_corner[2], &far_corner[3], &near_corner[3], &far_corner[2],
                 &near_corner[2], &far_corner[3], &near_corner[2]);
    SetPlayPlane(&PlayPlane[0], &near_corner[0], &near_corner[2], &near_corner[3], &near_corner[1], &near_corner[2],
                 &near_corner[0], &near_corner[3], &near_corner[0]);
    SetPlayPlane(&PlayPlane[5], &far_corner[0], &far_corner[1], &far_corner[3], &far_corner[2], &far_corner[1],
                 &far_corner[0], &far_corner[3], &far_corner[0]);
}

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed) {
    if (camera == NULL) {
        camera = GameCam;
    }
    camera->shake_target_amount = amount;
    camera->shake_time = duration;
    camera->shake_speed = speed;
}

void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount) {
    if (camera == NULL) {
        camera = GameCam;
    }

    if (camera->shake_time > 0.0f) {
        camera->shake_time -= FRAMETIME;
    }
    f32 target_amount = 0.0f;
    if (camera->shake_time > 0.0f)
        target_amount = camera->shake_target_amount;
    else if (ambient_amount > 0.0f)
        target_amount = ambient_amount;
    camera->shake_amplitude = SeekLinearF(camera->shake_amplitude, target_amount, FRAMETIME * 2.0f);

    constexpr f32 kShakeRadius = 0.1f;
    constexpr f32 kShakeRetargetDistanceSquared = 0.025f * 0.025f;
    if (NuVecDistSqr(&camera->shake_direction, &camera->shake_target, NULL) < kShakeRetargetDistanceSquared) {
        camera->shake_target.x = 0.0f;
        camera->shake_target.y = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * kShakeRadius;
        camera->shake_target.z = 0.0f;
        NuVecRotateZ(&camera->shake_target, &camera->shake_target, static_cast<NUANG>(qrand()));
        camera->shake_target.x *= 4.0f / 3.0f;
    }

    SeekVec(&camera->shake_direction, &camera->shake_direction, &camera->shake_target,
            camera->shake_time > 0.0f ? camera->shake_speed * 10.0f : 10.0f);
    SeekVec(&camera->shake_offset, &camera->shake_offset, &camera->shake_direction,
            camera->shake_time > 0.0f ? camera->shake_speed * 2.0f : 2.0f);

    NUVEC scaled_offset;
    NuVecScale(&scaled_offset, &camera->shake_offset, camera->shake_amplitude);
    constexpr f32 kShakeAngleScale = 910.0f;
    const i32 pitch = static_cast<i32>(kShakeAngleScale * camera->shake_amplitude * (scaled_offset.y / kShakeRadius));
    NuMtxPreRotateX(&camera->render_mtx, pitch);
    const i32 yaw = static_cast<i32>(kShakeAngleScale * camera->shake_amplitude * (scaled_offset.x / kShakeRadius));
    NuMtxPreRotateY(&camera->render_mtx, yaw);
}

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source) {
    if (camera == NULL)
        camera = GameCam;
    const f32 absolute_amount = NuFabs(amount);
    if (!(absolute_amount > camera->judder_time))
        return;
    if (source != NULL) {
        const f32 distance = NuVecDist(&camera->pos, source, NULL);
        const f32 maximum_distance = static_cast<f32>(static_cast<u8>(WORLD->current_level->camera_judder_distance));
        if (maximum_distance > distance) {
            const f32 attenuated_amount = ((maximum_distance - distance) / maximum_distance) * absolute_amount;
            if (attenuated_amount > camera->judder_time) {
                camera->judder_reverse = amount < 0.0f;
                camera->judder_duration = attenuated_amount;
                camera->judder_time = attenuated_amount;
            }
        }
    } else {
        camera->judder_duration = absolute_amount;
        camera->judder_time = absolute_amount;
        camera->judder_reverse = amount < 0.0f;
    }
    camera->judder_axis = static_cast<u8>(axis);
}

void GameCam_HitJudder() {
    f32 amount = VehicleArea != 0 ? -0.3f : -0.2f;
    if (qrand() <= 0x7fff) {
        amount = -amount;
    }
    GameCam_Judder(GameCam, amount, qrand() / 0x5556, NULL);
}

void GameCam_HitRoll() {
    const f32 amount = qrand() <= 0x7fff ? -0.25f : 0.25f;
    GameCam_Judder(GameCam, amount, 2, NULL);
}

void SetCameraMatrices() {
    NuRndrLightingStateCurrent.field_0x60 = 1;
    NuRndrLightingStateCurrent.field_0x74 = 0;
    NuRndrSetSpecularLightPS(NULL, NULL);

    NUMTX effect_matrix;
    NUVEC scale = {1.0f, 1.0f, 1.0f};
    NuMtxInvR(&effect_matrix, &global_camera.mtx);
    NuMtxScale(&effect_matrix, &scale);
    effect_matrix.m30 = 1.0f;
    effect_matrix.m31 = 1.0f;
    effect_matrix.m33 = 1.0f;
    effect_matrix.m32 = 0.0f;
    effect_matrix.m23 = 0.0f;
    effect_matrix.m13 = 0.0f;
    effect_matrix.m03 = 0.0f;
    NuRndrSetFxMtx(&effect_matrix);
}

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference) {
    const f32 distance_a = NuVecDist(reference, player_a, NULL);
    const f32 distance_b = NuVecDist(reference, player_b, NULL);
    const f32 blend = distance_a / (distance_a + distance_b);
    center->x = player_a->x + (player_b->x - player_a->x) * blend;
    center->y = player_a->y + (player_b->y - player_a->y) * blend;
    center->z = player_a->z + (player_b->z - player_a->z) * blend;
}

void NarrowSockExceptions_Init(NARROWSOCKEXCEPTION *exceptions) {
    i32 level_index __attribute__((aligned(16)));
    if (exceptions == NULL)
        return;
    NarrowSockException = exceptions;
    for (; exceptions->level_name != NULL; ++exceptions) {
        Level_FindByName(exceptions->level_name, &level_index);
        exceptions->level_index = level_index;
    }
}

i32 ObjInNarrowSock(GameObject_s *object, SOCKSYS *system, i32 level_index) {
    if ((WORLD->current_level->flags & LEVEL_NARROW_SOCKS) == 0 && !VehicleArea)
        return 0;
    if (disable_narrow_socks || object->apiobj.field_0x27c == -1)
        return 0;
    if (object->apiobj.player_controlled) {
        GameObject_s *other = NULL;
        if (object == Player[0])
            other = Player[1];
        else if (object == Player[1])
            other = Player[0];
        if (other != NULL && other->apiobj.player_controlled && !NarrowSockPositionAllowed(other, system, level_index))
            return 0;
    }
    return NarrowSockPositionAllowed(object, system, level_index);
}

void GameCam_Reset(GAMECAMERA_s *camera) {
    if (camera == NULL) {
        camera = GameCam;
    }

    camera->sock_position.location.sock = -1;
    camera->sock_position.location.segment = -1;
    camera->previous_mode = -1;
    camera->judder_time = 0.0f;
    ObstacleCamSpl = NULL;
    camera->shake_amplitude = 0.0f;
    MiniCutCam = 0;
    camera->shake_target_amount = 0.0f;
    camera->mode = -1;
    camera->shake_time = 0.0f;
    camera->shake_speed = 1.0f;
    camera->blend_duration = 0.0f;
    camera->blend_time = 0.0f;
    camera->position_seek = static_cast<f32>(static_cast<u8>(WORLD->current_level->cam_pos_seek));
    camera->field_0x1b8 = 0.0f;
    camera->angle_seek = static_cast<f32>(static_cast<u8>(WORLD->current_level->cam_angle_seek));
    camera->field_0x1ec = 0.0f;
    camera->field_0x1e8 = 0.0f;
    camera->field_0x1f4 = 0.0f;
    camera->field_0x1f0 = 0.0f;
    GameCam_ResetLookRot(camera);
    Minicam_InitSystem();
}

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode) {
    if (camera == NULL) {
        camera = GameCam;
    }
    if (!(duration > 0.0f) || camera->mode == -1) {
        return;
    }

    camera->blend_start_pitch = camera->desired_pitch;
    camera->blend_start_yaw = camera->desired_yaw;
    camera->blend_start_roll = camera->desired_roll;
    camera->blend_mode = mode == 0 ? 1 : 2;
    camera->previous_camera_mode = camera->previous_mode;
    camera->reset_blend = 1;
    camera->blend_curve = curve;
    camera->blend_time = 0.0f;
    camera->blend_duration = duration;
    camera->blend_start_position = camera->desired_position;
    camera->blend_start_target = camera->target;
    camera->blend_end_position = camera->desired_position;
    camera->blend_end_target = camera->target;
}

u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera) {
    if (camera == NULL) {
        camera = GameCam;
    }
    return static_cast<u16>(camera->input_yaw + static_cast<i32>(camera->field_0x218));
}

