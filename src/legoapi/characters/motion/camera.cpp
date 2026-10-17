// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void Minicam_InitSystem(void);
extern "C" void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void GameCam_ResetLookRot(GAMECAMERA_s *camera);
extern i32 newgamecam;
i32 GetMenuID(void);
i32 OnOrInsidePlane(NUVEC *point, NUVEC *plane_point, NUVEC *plane_normal, NUVEC *corrected_point, f32 normal_offset,
                    f32 *distance_out);
void Surface_Deflect(NUVEC *normal, NUVEC *movement, NUVEC *result, i32 mode);
void SpecialMove_Cancel(GameObject_s *object);
void Hint_CancelCurrent(void);
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
VIEWCAM_s ViewCam = {0, {1000000000.0f, 0.0f, 0.0f}, -8192, -29500, 2.66f, 0.0f, 120.0f, 200.0f, NULL};
// Original data symbol testmovef is float0.005 at 0x617cb0.
f32 testmovef = 0.005f;

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

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

void GameCam_HitRoll() {
    const f32 amount = qrand() <= 0x7fff ? -0.25f : 0.25f;
    GameCam_Judder(GameCam, amount, 2, NULL);
}

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed) {
    if (camera == NULL) {
        camera = GameCam;
    }
    camera->shake_target_amount = amount;
    camera->shake_time = duration;
    camera->shake_speed = speed;
}

void GameCam_HitJudder() {
    f32 amount = VehicleArea != 0 ? -0.3f : -0.2f;
    if (qrand() <= 0x7fff) {
        amount = -amount;
    }
    GameCam_Judder(GameCam, amount, qrand() / 0x5556, NULL);
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
    if (camera == NULL)
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
                    else if (horizontal_buttons == GAMEPAD_DRIGHT)
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
                    if (horizontal_buttons == GAMEPAD_DLEFT)
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

void GameCameraMakeMiniCut(nugspline_s *spline, f32 start, f32 end, f32 blend_in, f32 blend_out, i32 borders,
                           i32 hold_until_players_move) {
    if (spline == NULL)
        return;
    ObstacleCamSpl = spline;
    ObstacleCamStart = start;
    ObstacleCamEnd = end;
    ObstacleCamTime = 0.0f;
    ObstacleCamRotZ = 0;
    ObstacleCamBlendInTime = blend_in;
    ObstacleCamBlendOutTime = blend_out;
    ObstacleCamCutTgtPtr = NULL;
    ObstacleCamCutCamPtr = NULL;
    ObstacleCamBorders = borders;
    if (VehicleArea != 0)
        hold_until_players_move = 0;
    ObstacleCamHoldUntilPlayersMove = hold_until_players_move;
    if (borders != 0) {
        if (start == 0.0f && blend_in <= 0.0f)
            CutBorderScale = 1.0f;
    }
    if (start <= 0.0f && blend_in <= 0.0f) {
        GameCam->blend_time = GameCam->blend_duration;
        GameCam->mode = -1;
    }
    Hint_CancelCurrent();
    ObstacleCamAlwaysSnapAngles = 0;
}

extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern NUVEC ObstacleCamCutPts[2];

void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders) {
    ObstacleCamCutSpline.length = 2;
    ObstacleCamCutSpline.pt_size = 12;
    ObstacleCamCutSpline.pts = ObstacleCamCutPts;
    ObstacleCamTargetGuid = target_guid;
    if (camera != NULL) {
        ObstacleCamCutPts[0] = *camera;
    }
    if (target != NULL) {
        ObstacleCamCutPts[1] = *target;
    }
    GameCameraMakeMiniCut(&ObstacleCamCutSpline, start, end, blend_in, blend_out, borders, 0);
    if (follow_target != 0 && target != NULL) {
        ObstacleCamCutTgtPtr = target;
    }
    if (follow_camera != 0 && camera != NULL) {
        ObstacleCamCutCamPtr = camera;
    }
}

void GameCameraMakeMiniCut3(u32 flags, float distance, i32 pitch, i32 yaw, i32 roll, void *focus, i32 target_guid,
                            nuvec_s *position, float start_time, float blend_in_time, float end_time,
                            float blend_out_time, float blend_time, float hold_time, float max_time, i32 borders,
                            nugspline_s *spline, char mode, char easing) {
    GameCam->blend_time = 0.0f;
    if (flags & 0x200) {
        Minicam_ResetForNewCut();
        Minicam_AddCommand(1, blend_in_time, 0, NULL, v000);
        MiniCam.duration = max_time;
        if ((flags & 0x80) && focus != NULL) {
            MiniCam.focus = static_cast<NUVEC *>(focus);
            MiniCam.focus_offset = v000;
            MiniCam.target = *static_cast<NUVEC *>(focus);
            MiniCam.position = *position;
            GameCameraMakeMiniCut2(&MiniCam.position, &MiniCam.target, target_guid, start_time, 1000000000.0f,
                                   blend_in_time, 0.0f, 1, 1, MiniCam.reserved_384);
        } else {
            MiniCam.target = v000;
            MiniCam.focus_offset = v000;
            MiniCam.focus = &MiniCam.target;
            MiniCam.position = *position;
            GameCameraMakeMiniCut2(&MiniCam.position, NULL, target_guid, start_time, 1000000000.0f, blend_in_time, 0.0f,
                                   0, 1, MiniCam.reserved_384);
        }
    }

    if ((flags & 0x1c00) == 0) {
        blend_time = 0.0f;
        flags |= 0x800;
    }
    if (flags & 0x80)
        Minicam_AddCommand(8, 0.0f, 0, focus, v000);
    if (flags & 0x100)
        Minicam_AddCommand(16, 0.0f, 0, NULL, *position);
    else if (flags & 0x2000)
        Minicam_AddCommand(17, 0.0f, 0, spline, v000);
    else if (flags & 1)
        Minicam_AddCommand(9, distance, 0, NULL, v000);
    if (flags & 0x10)
        Minicam_AddCommand(13, 0.0f, pitch, NULL, v000);
    else if (flags & 2)
        Minicam_AddCommand(10, 0.0f, pitch, NULL, v000);
    if (flags & 0x20)
        Minicam_AddCommand(14, 0.0f, yaw, NULL, v000);
    else if (flags & 4)
        Minicam_AddCommand(11, 0.0f, yaw, NULL, v000);
    if (flags & 0x40)
        Minicam_AddCommand(15, 0.0f, roll, NULL, v000);
    if (flags & 8)
        Minicam_AddCommand(12, 0.0f, roll, NULL, v000);
    if (mode != -1)
        Minicam_AddCommand(6, 0.0f, mode, NULL, v000);
    if (easing != -1)
        Minicam_AddCommand(7, 0.0f, easing, NULL, v000);

    if ((flags & 5) == 5 && blend_in_time > 0.0f && blend_time == 0.0f)
        blend_time = 0.01f;
    if ((flags & 0x1800) == 0x1800) {
        if (hold_time > blend_time) {
            const f32 remainder = hold_time - blend_time;
            Minicam_AddCommand(4, blend_time, 0, NULL, v000);
            Minicam_AddCommand(5, remainder, 0, NULL, v000);
        } else {
            Minicam_AddCommand(4, blend_time, 0, NULL, v000);
        }
    } else if (flags & 0x800) {
        Minicam_AddCommand(4, blend_time, 0, NULL, v000);
    } else if (flags & 0x1000) {
        Minicam_AddCommand(5, hold_time, 0, NULL, v000);
    }
    if (flags & 0x400) {
        float remaining = end_time;
        if ((flags & 0x1800) == 0x1800) {
            if (hold_time > blend_time)
                remaining = end_time - hold_time;
            else
                remaining = end_time - blend_time;
        } else if (flags & 0x800) {
            remaining = end_time - blend_time;
        } else if (flags & 0x1000) {
            remaining = end_time - hold_time;
        }
        Minicam_AddCommand(3, MAX(0.0f, remaining), 0, NULL, v000);
        ObstacleCamBlendOutTime = blend_out_time;
    }
    if (borders != 0)
        MiniCam.reserved_384 = 1;
}

u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera) {
    if (camera == NULL) {
        camera = GameCam;
    }
    return static_cast<u16>(camera->input_yaw + static_cast<i32>(camera->field_0x218));
}

extern "C" {

    extern NUVEC *CutoffCameraVec;

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern "C" f32 MainRenderTime;
extern f32 newgamecamtime, cutscenecam_focalLength, CamStopBlend, EMPERORFIGHTA_CAMDYHACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
extern i32 LevFlag[4];
extern NUMTX cutscenecammtx, CutCamMtx;
extern u8 set_cutscenecammtx;
extern NUGSPLINE *hub_minikitviewer_camspl;
extern AREADATA_s *PODRACE_ADATA;
f32 getPodRoll(i32);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
f32 PodSprint_InStartCountdown(WORLDINFO_s *);
i32 PodRace_InStartCountdown(WORLDINFO_s *);
extern "C" i32 editor_active;
extern "C" void NuSpeedBlurSetMotionFactors(f32 pan, f32 pull, f32 clamp);
extern "C" void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
void MakePlayPlanes(GAMECAMERA_s *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

static NUVEC GunshipAOffset = {12.0f, 22.5f, -12.0f};
NUVEC CustomisePos[2];
f32 HUB_MINIKITVIEWER_CAMDY = 0.3f;
f32 PodCamDist = 0.0f;
NUVEC ObstacleCamCutPts[2];
nugspline_s ObstacleCamCutSpline;
i32 ObstacleCamTargetGuid;
f32 EMPERORFIGHTA_CAMDYHACK = 0.35f;
i32 movegamecamera_forcesock = -1;
NUMTX CutCamMtx;
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);
void CentreTwoPlayerCamera(NUVEC *, NUVEC *, NUVEC *, NUVEC *);

static void PlayerCamPos(GameObject_s *object, NUVEC *position, NUVEC *reference);

void MoveGameCamera(GAMECAMERA_s *camera);

void ViewCamDraw();

void KeepOnScreen(GameObject_s *object);

NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();

void SetDepthOfField() {
}

void SpeedBlur_Apply(WORLDINFO_s *world);

void SpeedBlur_Update();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity) {
    if (position->x < -0.85f) {
        position->x = -0.85f;
        if (velocity != NULL)
            velocity->x = 0.0f;
    } else if (position->x > 0.85f) {
        position->x = 0.85f;
        if (velocity != NULL)
            velocity->x = 0.0f;
    }
    if (position->y < -0.85f) {
        position->y = -0.85f;
        if (velocity != NULL)
            velocity->y = 0.0f;
    } else if (position->y > 0.85f) {
        position->y = 0.85f;
        if (velocity != NULL)
            velocity->y = 0.0f;
    }
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

GAMEPAD_s *ViewCamGetGamePad();

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference) {
    const f32 distance_a = NuVecDist(reference, player_a, NULL);
    const f32 distance_b = NuVecDist(reference, player_b, NULL);
    const f32 blend = distance_a / (distance_a + distance_b);
    center->x = player_a->x + (player_b->x - player_a->x) * blend;
    center->y = player_a->y + (player_b->y - player_a->y) * blend;
    center->z = player_a->z + (player_b->z - player_a->z) * blend;
}

void KeepVehicleOnScreen(GameObject_s *object, i32 sides, i32 top, i32 bottom);

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

void InitCameraTargetMaterial();

i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void GetTopBot(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    const f32 bottom = character->field15_0x34;
    const f32 top = character->field16_0x38;
    object->field_0xffc = bottom;
    object->field_0x1000 = top;

    if (object->field_0x1008 == 0.0f) {
        object->collision_y_scale = 0.0f;
    } else {
        object->collision_y_scale = (top - bottom) / (object->field_0x1008 * 2.0f);
    }
}

extern "C" {

extern i32 near_clip_at_cursor;

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

} // extern "C"

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void DrawSubItems();

extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
extern i32 GAMEDEMO;
extern f32 FORCEGLOWTIME;
extern i16 tHINTS;
extern i16 tCHARACTERS;
extern i16 tEXTRAS;
extern i16 tGOLDBRICKS;
extern i16 tSTORYCLIPS;
extern i16 tENTERCODE;
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern f32 HUB_EPISODESUBTITLEY;
extern f32 HUB_EPISODETITLEY;
extern i32 draw_para;

void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();
static void DrawParaphernalia(GameObject_s *object);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern "C" void NuCameraUnlock(void);
extern "C" void NuCameraRelock(void);
extern char *apiGameName;
extern char *apitxt_EMPTY;
extern char *apitxt_PRESENT;
extern char *apitxt_AUTOSAVE_WARNING;
extern char *apitxt_LOADING;
extern char *apitxt_SAVING;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_OK;
extern char *apitxt_GAMELOADED;
extern char *apitxt_GAMELOADING;
extern char *apitxt_FAILEDTOLOAD;
extern char *apitxt_CORRUPTLOAD;
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern i16 tCURRENTGAME;
extern i16 tEMPTY;
extern i16 tGAME;
extern i16 tNOSPACE;
extern f32 MENUTEXTSCALE;
extern f32 MENUBOTY;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern i32 menu_flash;
extern f32 text3d_height;
extern f32 text3d_width;
extern FadeSystem FadeSys;
extern f32 cointotaltime;
extern i32 Paused;
extern "C" i32 screendump;
extern f32 pause_fade;
extern "C" i32 PANELOFF;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state
extern f32 CameraZoom;

// Graphics loading flags
extern i32 RemoveDirectionalMaps;
extern i32 RemoveNormalMaps;

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

extern "C" void NuSevereWarning(const char *format, ...);

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"

extern i32 solid_cable;
extern i32 nsegments_drawn;
extern f32 slack_factor;
extern f32 cable_slack;
extern f32 nsegments_per_unit;
extern CABLE_s cables[8];
extern f32 tow_length;
extern NUMTL *SolidMtl3D;

void DrawCables();

void DrawRipple(ripple_node_s *node);

void DrawAreaBox(nuvec_s *position, nuvec_s *size, i32 rotation, i32 colour);

void DrawBox_Now(_vuv_s *, _vuv_s *, i32, i32);

extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour);

void Draw_LOADED();

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];

void DrawLine_Now(_vuv_s *, _vuv_s *, i32, i32);

void DrawParallax(nuhspecial_s *special);

void DrawQuestion(nuvec_s *position, float scale_value, float y_push);

void DrawRectRGBA(float x, float y, float width, float height, u32 colour, numtl_s *material, i32 alignment,
                  float scale);

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float unused, float y_push, u16 x_rot,
              u16 y_rot, u16 z_rot);


void Draw_LOADING();

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

void DrawAlphaGrid(i32 rows, i32 cols, NuBloomParameters *parameters);

void DrawArrow_Now(_vum_s *, float, i32, i32);

extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawBonusTime(STATUSPACKET_s *packet, float position, i32 alpha);

void DrawCross_Now(_vuv_s *, float, i32, i32);

void DrawPauseFade();


void DrawRippleSet(ripple_set_s *set);



void ClearScreen();



void DrawAlphaImage(i32 rows, i32 cols, numtl_s *material, i32 use_pixel_offsets, NuBloomParameters *parameters);

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour);

void DrawBoxMtx_Now(_vum_s *, _vuv_s *, i32, i32);

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen);

void DrawExplosions();

void DrawItemMenu2D();

void DrawStatusText(char *text, u16 angle, float x, float y, float scale, u32 colour, i32 alignment);

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();

extern nuhspecial_s green_light;
extern nuhspecial_s painttargetcolour[3];
extern nuhspecial_s paintlights[3];

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

void Draw_LOADFAILED();

void DrawAreaCylinder(nuvec_s *centre, nuvec_s *size, i32 colour);

void DrawCameraTarget(nuvec_s *);

void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 KITPOS2X;
extern f32 KITPOS2Y;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
extern f32 PANEL_MINIKITCOUNTSCALE;
extern f32 PANEL_MINIKITCOUNTY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
extern i32 LEGOOBJ_CHARKIT;
extern i32 LEGOOBJ_MINIKIT;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);

void DrawStatusBG_LSW(STATUSPACKET_s *status);

void Draw_LOADCORRUPT();

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);

void DrawCameraTarget2(nuvec_s *);

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];
extern i32 currentminikit;
extern f32 slideseek;

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);
void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet,
                       float slide);

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

void DrawSubItemMenu2D();

void DrawSubItemMenu3D();

void Draw_NOMEMORYCARD();

void DrawFadeScreenWipe();


void DrawForceGlowSprite(nuvec_s *position, float radius, i32 model, float alpha, GameObject_s *);

void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();

void Draw_NODATAAVAILABLE();

extern i32 dco_locatorposonly;
extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern u16 dco_prerotatez;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s * dco_gcdata;
extern CHARACTERMODEL_s * dco_cmodel;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern f32 DropInOutScale(GameObject_s *object);
extern void ApplyExtraRotation(GameObject_s *object, NUMTX *matrix);
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}
extern u8 CharClipToBlobShadows;


i32 DrawGameObjectsProcess();

void DrawStatusTextFraction(i32 current, i32 total, float x, float y, u16 angle, float scale, u32 colour,
                            float animation, float animation_duration);

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);


extern void DrawMenuEntry(MENU_s *, char *);
void Draw_OK(MENU_s *menu);

void DrawAABox(_vuv_s *position, _vuv_s *size, i32 colour);

void DrawArrow(nuhspecial_s *special, float scale_value);

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour);

extern f32 SwipeDecalRenderer_MaxWidth;
extern f32 SwipeDecalRenderer_MinWidth;


#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)




extern i32 GetMenuID(void);
static __used__ i32 PauseRenderOff();

static __used__ i32 MatrixReflection_CanOverride();

static void DrawWeapon_SetSabreObjects(GameObject_s *object, i32 red, i32 green, i32 blue, i32 purple, i32 *models,
                                       i32 *hilt);

static void DrawWeapons(GameObject_s *object, i32 reflection, f32 weapon_scale);

static void DrawCharacterAttachments(GameObject_s *object, NUMTX *joint_matrices);

void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
void CharMiniKit_Draw(i32, NUMTX *, i32, f32, f32);
void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, NUMTX *);
i32 Batarang_GetObjectFromCharID(i32);
void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;


#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);




extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;


extern i32 backdrop_black;

extern void (*BackDrop_AlphaFn)(float *);

static __used__ void BackDrop_Alpha(float *alpha);

void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);

void BackDrop_Dump();

void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== functions in original address order ====

static void PlayerCamPos(GameObject_s *object, NUVEC *position, NUVEC *reference) {
    CHARACTERDATA *character = object->apiobj.character_data;
    f32 height = (character->field15_0x34 + character->field16_0x38) * character->field17_0x3c * 0.5f;
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    NUVEC direction, focus;
    NUVEC *target;
    if ((object->apiobj.field_0x287 != 0 || object->character_context == 0x2b) &&
        (WORLD->current_level->flags & 0x40000) == 0) {
        if (object->apiobj.field_0x287 != 0)
            *position = object->apiobj.position;
        else
            *position = object->apiobj.field_0x1c0;
        position->y += height;
        if (VehicleArea != 0 && static_cast<i8>(object->apiobj.flags_low) < 0 && object->field_0x101c > 0.0f) {
            f32 blend = 1.0f - object->field_0x101c * 0.5f;
            if (!(blend < 0.5f)) {
                blend =
                    1.0f - (NU_SIN_LUT(static_cast<i32>((blend - 0.5f) * 2.0f * 32768.0f + 16384.0f)) + 1.0f) * 0.5f;
                blend = 1.0f - blend;
            } else
                blend = NU_SIN_LUT(static_cast<i32>(blend * 2.0f * 16384.0f));
            position->x += object->apiobj.velocity.x * blend;
            position->z += object->apiobj.velocity.z * blend;
        }
        return;
    }
    if (object->character_context == 0x0f && object->field_0x788 != NULL &&
        (static_cast<CAMERA_CONTEXT_PATH_s *>(object->field_0x788)->flags & 0x10) != 0) {
        NUVEC *points = static_cast<CAMERA_CONTEXT_PATH_s *>(object->field_0x788)->path->pts;
        NuVecSub(&direction, &points[0], &points[3]);
        NuVecSub(&focus, &points[0], &object->apiobj.position);
        f32 scale = 1.0f / NuVecMagVU0(&direction);
        direction.x = __builtin_fabsf(points[3].x - points[0].x) * scale;
        direction.y = __builtin_fabsf(points[3].y - points[0].y) * scale;
        direction.z = __builtin_fabsf(points[3].z - points[0].z) * scale;
        position->x = points[0].x - direction.x * focus.x;
        position->y = points[0].y - direction.y * focus.y;
        position->z = points[0].z - direction.z * focus.z;
        return;
    }
    position->x = object->apiobj.position.x;
    position->y = object->apiobj.position.y + height;
    position->z = object->apiobj.position.z;
    target = NULL;
    if (object->cable != NULL && object->cable->target != NULL)
        target = &object->cable->target->apiobj.collision_position;
    switch (object->character_context) {
        case 8:
            if (object->gizforce_target != NULL)
                target = &object->gizforce_target->position;
            break;
        case 0x1b:
            if (object->field_0x780 != NULL)
                target = &static_cast<GameObject_s *>(object->field_0x780)->apiobj.collision_position;
            break;
        case 0x1d:
            if (object->force_part != NULL && (object->force_part->active & 1) != 0)
                target = &object->force_part->position;
            break;
        case 0x2d:
            if (object->field_0x788 != NULL)
                target = &static_cast<GIZBUILDIT_s *>(object->field_0x788)->position;
            break;
        case 0x46:
            if (Grapple_LookAtPos(object, &focus) != 0)
                target = &focus;
            break;
        case 0x51:
            target = Technos_TgtPos(static_cast<TECHNO_s *>(object->field_0x788));
            if (target != NULL) {
                NuVecSub(&focus, target, &object->apiobj.position);
                NuVecScale(&focus, &focus, static_cast<TECHNO_s *>(object->field_0x788)->scale);
                NuVecAdd(&focus, &focus, &object->apiobj.position);
                *position = focus;
                target = NULL;
            }
            break;
        case 0x59:
            if (object->field_0x788 != NULL)
                target = &static_cast<CAMERA_CONTEXT_FOCUS_s *>(object->field_0x788)->focus;
            break;
    }
    if ((object->id == id_GRABCONTROL || object->id == id_GRABR2CONTROL) && world->grabber != NULL) {
        direction.x = (position->x + world->grabber->grab_position.x) * 0.5f;
        direction.y = position->y;
        direction.z = (position->z + world->grabber->grab_position.z) * 0.5f;
        target = &direction;
    }
    if (target != NULL)
        CentreTwoPlayerCamera(position, position, target, reference);
}

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad) {
    if (player != NULL) {
        ViewCam.mode = mode;
        if (mode != 0) {
            ViewCam.gamepad = gamepad;
            ViewCam.target = player->apiobj.collision_position;
        } else {
            ViewCam.gamepad = NULL;
        }
    }
}

i32 ViewCamGetMode() {
    return ViewCam.mode;
}

GAMEPAD_s *ViewCamGetGamePad() {
    return ViewCam.gamepad;
}

NUVEC *ViewCamGetTgt() {
    return &ViewCam.target;
}

void ViewCamDraw() {
}

// Function prefix and mode selection. MainRenderTime guard precedes cutscenes.
void MoveGameCamera(GAMECAMERA_s *camera) {
    i32 shared_count = 0;

    NUVEC player_focus[2], player_positions[2];
    GameObject_s *camera_players[2];
    i32 player_roll[2], vehicle_player[2];
    u16 player_yaw[2];
    f32 walker_distance_sq = 0.0f;
    f32 blend_duration = 0.5f;
    f32 camera_shake = 0.0f;
    MiniCutCam = 0;
    nucamera_farclip_hack = WORLD->current_level->data_display.unknown_00;
    i32 menu_id = GetMenuID();
    if (MainRenderTime < 1.0f) {
        camera->render_mtx = numtx_identity;
        camera->mtx = camera->render_mtx;
        pNuCam->mtx = camera->render_mtx;
        NuCameraSet(pNuCam);
        GameCam_ResetLookRot(camera);
        return;
    }
    if (CutSceneCameraCTRL) {
        if (CUTSTOPGAME || CUTCAMONLY) {
            camera->render_mtx = cutscenecammtx;
            camera->mtx = camera->render_mtx;
            set_cutscenecammtx = 0;
            pNuCam->mtx = camera->render_mtx;
            if (cutscenecam_focalLength > 0.0f) {
                pNuCam->fov = NuCameraFocalLenToFOV(cutscenecam_focalLength);
                pNuCam->fov *= (1.0f / NuIOS_GetAspectRatio()) / 0.75f;
            }
            NuCameraSet(pNuCam);
            CutCamMtx = camera->render_mtx;
            CUTCAM = 1;
            return;
        }
    } else if (CUTSTOPGAME) {
        GameCam_ResetLookRot(camera);
        return;
    }
    if (NewMode || NewLData != NULL) {
        GameCam_ResetLookRot(camera);
        return;
    }
    i32 previous_mode = camera->mode;
    camera->mode = -1;
    camera->previous_mode = previous_mode;
    i8 previous_candidate_count = camera->sock_position.candidate_count;
    i8 previous_sock = camera->sock_position.location.sock;
    if (ViewCam.gamepad != NULL)
        camera->mode = 5;
    if (newgamecam) {
        newgamecamtime += FRAMETIME;
        if (newgamecamtime >= 10.0f || WORLD->camera_splines[6] == NULL || WORLD->camera_splines[7] == NULL)
            newgamecam = 0;
    }
    if (menu_id == 14 && hub_minikitviewer_camspl != NULL)
        camera->mode = 9;
    else if (menu_id == 8 && WORLD->camera_splines[24] != NULL)
        camera->mode = 6;
    else if (SHOPACTIVE && shopcampos != NULL && shopcamlookat != NULL)
        camera->mode = 8;
    else if (menu_id == 12 && WORLD->camera_splines[15] != NULL)
        camera->mode = 7;
    else if (ObstacleCamSpl != NULL && (ObstacleCamTime <= ObstacleCamEnd || ObstacleCamHoldUntilPlayersMove)) {
        f32 old_time = ObstacleCamTime;
        if (ObstacleCamEnd > ObstacleCamTime)
            ObstacleCamTime += FRAMETIME;
        if (ObstacleCamTime >= ObstacleCamEnd) {
            ObstacleCamTime = ObstacleCamEnd;
            if (ObstacleCamHoldUntilPlayersMove) {
                camera->mode = 2;
                u32 buttons = GAMEPAD_START | GAMEPAD_JUMP | GAMEPAD_SPECIAL | GAMEPAD_ACTION | GAMEPAD_TAG |
                              GAMEPAD_TOGGLELEFT | GAMEPAD_TOGGLERIGHT;
                for (i32 i = 0; i < 2; i++) {
                    GameObject_s *object = Player[i];
                    if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0 &&
                        (object->apiobj.model_draw_result == 0 || object->pad_gamepad->input_magnitude > 0.0f ||
                         (object->pad_gamepad->buttons_held & buttons) != 0)) {
                        ObstacleCamHoldUntilPlayersMove = 0;
                        break;
                    }
                }
            } else {
                ObstacleCamSpl = NULL;
                if (!(ObstacleCamBlendOutTime <= 0.0f))
                    blend_duration = ObstacleCamBlendOutTime;
                else {
                    GameCam_Reset(camera);
                    previous_mode = camera->mode;
                    camera->previous_mode = previous_mode;
                    previous_sock = camera->sock_position.location.sock;
                    previous_candidate_count = camera->sock_position.candidate_count;
                    CutBorderScale = 0.0f;
                }
                goto choose_gameplay_camera;
            }
        } else if (ObstacleCamTime >= ObstacleCamStart && ObstacleCamEnd > ObstacleCamTime) {
            camera->mode = 2;
            if (ObstacleCamStart > 0.0f && ObstacleCamStart > old_time && ObstacleCamBlendInTime <= 0.0f) {
                camera->blend_duration = 0.0f;
                camera->blend_time = 0.0f;
                camera->previous_mode = -1;
                CutBorderScale = 1.0f;
            }
        } else
            goto choose_gameplay_camera;
    } else if (Door_UseCutCam && Door_CutLookAtPlayers && Door_CutCamWait > 0.0f) {
        Door_CutCamWait -= FRAMETIME;
        camera->mode = 4;
    } else
        goto choose_gameplay_camera;
    goto mode_selected;
choose_gameplay_camera:
    if (camera->mode == -1) {
        camera->mode = 0;
        if (Door_UseCutCam && GameTimer.update_count == 0)
            camera->mode = 4;
        else if (WORLD->current_level == TITLES_LDATA && WORLD->camera_splines[2] != NULL)
            camera->mode = 3;
        else if (PLAYERCOUNT > 0 && WORLD->sock_sys != NULL) {
            camera->mode = 1;
            if (GUNSHIPAHACK && WORLD->current_level == BONUS_GUNSHIPA_LDATA && WORLD->sock_sys->sock[0].valid)
                camera->mode = 11;
        }
    }
mode_selected:
    if (camera->mode != previous_mode) {
        if (previous_mode == 4) {
            GameCam_Blend(camera, Door_CutCamBlendTime, Door_CutLookAtPlayers ? 0.0f : Door_CutCamWait, 1);
            Door_UseCutCam = 0;
        } else if (previous_mode != -1) {
            f32 duration = blend_duration;
            if (camera->mode == 2)
                duration = ObstacleCamBlendInTime;
            else if (camera->mode == 9 || previous_mode == 9 || camera->mode == 6 || previous_mode == 6)
                duration = 1.5f;
            else if (camera->mode == 8 || previous_mode == 8)
                duration = 1.0f;
            else if (camera->mode == 7 || previous_mode == 7)
                duration = GAMEDEMO ? 3.0f : 1.5f;
            if (camera->previous_mode != -1 && duration != 0.0f)
                GameCam_Blend(camera, duration, 0.0f, 1);
        }
    }
    if (camera->mode == 4 || camera->mode == 2)
        MiniCutCam = camera->blend_duration > camera->blend_time ? 1 : (ObstacleCamHoldUntilPlayersMove ? 3 : 2);
    else if (camera->blend_duration > camera->blend_time &&
             (camera->previous_camera_mode == 4 || camera->previous_camera_mode == 2))
        MiniCutCam = 4;
    LEVELDATA *seek_level = WORLD->current_level;
    NUVEC position = *PlayerStart[0].pos;
    NUVEC offset, direction, mode_scratch;
    // The per-mode camera blend duration starts afresh after transition selection.
    blend_duration = 0.5f;
    f32 position_seek = static_cast<u32>(static_cast<u8>(seek_level->cam_pos_seek));
    f32 angle_seek = static_cast<u32>(static_cast<u8>(seek_level->cam_angle_seek));
    NUVEC target = v000;
    GAMEPAD_s *selected_pad = camera->mode == 5 ? ViewCam.gamepad : &GamePad[0];
    f32 left_y = static_cast<f32>(static_cast<i32>(selected_pad->pad->analog_left_y)) - 127.5f;
    left_y = NuFabs(left_y) < 34.0f ? 0.0f : left_y / 127.5f;
    f32 left_x = static_cast<f32>(static_cast<i32>(selected_pad->pad->analog_left_x)) - 127.5f;
    left_x = NuFabs(left_x) < 34.0f ? 0.0f : left_x / 127.5f;
    f32 right_y = static_cast<f32>(static_cast<i32>(selected_pad->pad->analog_right_y)) - 127.5f;
    right_y = NuFabs(right_y) < 34.0f ? 0.0f : right_y / 127.5f;
    f32 right_x = static_cast<f32>(static_cast<i32>(selected_pad->pad->analog_right_x)) - 127.5f;
    right_x = NuFabs(right_x) < 34.0f ? 0.0f : right_x / 127.5f;
    i32 pitch_override = -1, yaw_override = -1, roll_override = 0;
    u16 roll_hint = 0;
    i32 roll_hint_valid = 0;
    f32 roll_seek_override = 0.0f, stop_blend_rate = 0.1f;
    switch (camera->mode) {
        i32 index;

        case 1: {
            // Case 1 prefix. Original 0x111314..0x1129bf and its child branches.

            i32 player_count = 0, vehicle_count = 0;
            f32 landspeeder_speed = 0.0f, landspeeder_yaw = -1.0f, landspeeder_lookahead = 0.0f;
            for (i32 i = 0; i < 2; i++) {
                GameObject_s *object = Player[i];
                if (object == NULL || (static_cast<i8>(object->apiobj.flags_low) >= 0 && !LookAtBoth) ||
                    (netcamera && (object->apiobj.field_0x1f4 & 0x40000) != 0) ||
                    (BonusWinner != -1 && BonusWinner != i))
                    continue;
                vehicle_player[i] = object->apiobj.character_data->model_flags & 0x2000;
                if (!(SPEEDERCHASEA_LDATA != NULL && WORLD->current_level == SPEEDERCHASEA_LDATA &&
                      object->id == id_SPEEDERBIKE && disable_narrow_socks) &&
                    vehicle_player[i]) {
                    vehicle_count++;
                    if (object->id == id_LANDSPEEDER) {
                        if (landspeeder_lookahead != 0.0f) {
                            landspeeder_lookahead =
                                (landspeeder_lookahead + object->apiobj.horizontal_velocity_magnitude * 0.6f) * 0.5f;
                            landspeeder_yaw = (landspeeder_yaw + static_cast<u16>(object->apiobj.field_0x276)) * 0.5f;
                            landspeeder_speed =
                                (landspeeder_speed + object->apiobj.horizontal_velocity_magnitude) * 0.5f;
                        } else {
                            landspeeder_speed = object->apiobj.horizontal_velocity_magnitude;
                            landspeeder_lookahead = object->apiobj.horizontal_velocity_magnitude * 0.6f;
                            landspeeder_yaw = static_cast<u16>(object->apiobj.field_0x276);
                        }
                    }
                }
                if (PODRACE_ADATA != NULL && WORLD->area == PODRACE_ADATA) {
                    player_focus[player_count] = Player[i]->apiobj.collision_position;
                    NUVEC &pod_offset = mode_scratch;
                    pod_offset.x = 0.0f;
                    pod_offset.y = 0.0f;
                    pod_offset.z = 2.0f;
                    NuVecRotateY(
                        &pod_offset, &pod_offset,
                        static_cast<i32>(static_cast<u16>(object->apiobj.facing_angle) + getPodRoll(i) * 5461.0f));
                    NuVecAdd(&player_focus[player_count], &player_focus[player_count], &pod_offset);
                } else
                    PlayerCamPos(object, &player_focus[player_count], &camera->pos);
                NUVEC *source_position = &object->apiobj.position;
                if (object->character_context == 0x2b && !object->apiobj.field_0x287)
                    source_position = &object->apiobj.field_0x1c0;
                else if (object->character_context == 0x51 && object->field_0x788 != NULL) {
                    NUVEC *techno_position = Technos_TgtPos(static_cast<TECHNO_s *>(object->field_0x788));
                    if (techno_position != NULL)
                        source_position = techno_position;
                }
                memcpy(&player_positions[player_count], source_position, sizeof(NUVEC));
                if ((object->apiobj.character_data->model_flags & 0x2000) != 0) {
                    if (PODRACE_ADATA != NULL && WORLD->area == PODRACE_ADATA)
                        player_roll[player_count] = static_cast<i32>(getPodRoll(i) * 8192.0f);
                    else
                        player_roll[player_count] = RotDiff(0, object->movement_lean_angle);
                    RotDiff(0, object->tertiary_lean_angle);
                } else
                    player_roll[player_count] = 0;
                camera_players[player_count] = object;
                player_yaw[player_count] = object->apiobj.field_0x276;
                player_count++;
            }
            if (player_count != 0) {
                complexsockposition_forcesock = movegamecamera_forcesock;
                f32 separation_scale;
                index =
                    SockSysCamera(WORLD->sock_sys, &camera->pos, camera->mode != camera->previous_mode, player_focus,
                                  player_positions, player_count, &camera->sock_position, &position, &target,
                                  &blend_duration, &position_seek, &angle_seek, &camera_shake, &separation_scale);
                NUVEC rail_position = position;
                blend_duration *= 1.5f;
                if (WORLD->current_level->cam_pullback_dist > 0.0f) {
                    i32 angle = NuAtan2D(target.x - position.x, target.z - position.z);
                    NUVEC &pullback_direction = offset;
                    NUVEC &pullback_offset = mode_scratch;
                    NuVecSub(&pullback_direction, &position, &target);
                    pullback_direction.y = 0.0f;
                    NuVecNorm(&pullback_direction, &pullback_direction);
                    f32 pullback = 0.0f;
                    if (vehicle_count) {
                        u16 heading;
                        if (player_count == 2 && vehicle_player[0] && vehicle_player[1])
                            heading = player_yaw[0] + RotDiff(player_yaw[0], player_yaw[1]) / player_count;
                        else if (vehicle_player[0])
                            heading = player_yaw[0];
                        else
                            heading = player_yaw[1];
                        index = RotDiff(angle, heading);
                        if ((camera->sock_position.location.sock == -1 ||
                             (WORLD->sock_sys->sock[camera->sock_position.location.sock].flags & 0x1000) == 0) &&
                            abs(index) > 0x4000)
                            pullback = (abs(index) - 0x4000) * (1.0f / 16384.0f);
                    }
                    camera->field_0x1e8 = SeekLinearF(camera->field_0x1e8, pullback, FRAMETIME);
                    camera->field_0x1ec = SeekValF(camera->field_0x1ec, camera->field_0x1e8, 3.0f);
                    NuVecScale(&pullback_offset, &pullback_direction,
                               camera->field_0x1ec * WORLD->current_level->cam_pullback_dist);
                    NuVecAdd(&position, &position, &pullback_offset);
                    NuVecAdd(&target, &target, &pullback_offset);
                    f32 lateral = 0.0f;
                    if (vehicle_count &&
                        (camera->sock_position.location.sock == -1 ||
                         (WORLD->sock_sys->sock[camera->sock_position.location.sock].flags & 0x1000) == 0)) {
                        lateral = 1.0f - abs(0x4000 - abs(index)) * (1.0f / 16384.0f);
                        if (index < 0)
                            lateral = -lateral;
                    }
                    camera->field_0x1f0 = SeekLinearF(camera->field_0x1f0, lateral, FRAMETIME);
                    camera->field_0x1f4 = SeekValF(camera->field_0x1f4, camera->field_0x1f0, 3.0f);
                    NuVecRotateY(&pullback_offset, &pullback_direction, 0x4000);
                    NuVecScale(&pullback_offset, &pullback_offset,
                               (0.5f * WORLD->current_level->cam_lateral_dist) * camera->field_0x1f4);
                    NuVecSub(&position, &position, &pullback_offset);
                    NuVecSub(&target, &target, &pullback_offset);
                }
                // Original PODRACE_ADATA / PODSPRINT_ADATA branches after SockSysCamera.
                if (WORLD->area != NULL && WORLD->area == PODRACE_ADATA) {
                    f32 distance;
                    if (avg_currentspeed_mul <= 0.333f)
                        distance = 3.0f;
                    else if (avg_currentspeed_mul < 1.0f)
                        distance = 3.0f - (avg_currentspeed_mul - 0.333f) / 0.667f;
                    else
                        distance = 2.0f;
                    PodCamDist = SeekLinearF(PodCamDist, distance, FRAMETIME);
                    NUVEC &average = mode_scratch;
                    average.x = 0.0f;
                    average.y = 0.0f;
                    average.z = 0.0f;
                    index = 0;
                    for (; index < player_count; ++index) {
                        GameObject_s *player = camera_players[index];
                        average.x += player->apiobj.pos_x - NU_SIN_LUT(player->apiobj.facing_angle) * PodCamDist;
                        average.y += player->apiobj.pos_y + 1.0f;
                        average.z += player->apiobj.pos_z - NU_COS_LUT(player->apiobj.facing_angle) * PodCamDist;
                    }
                    if (player_count != 1)
                        NuVecScale(&average, &average, 1.0f / player_count);
                    position = average;
                    position_seek = 10.0f;
                    angle_seek = 10.0f;
                    f32 floor = GameShadow(NULL, &position, 5.0f, -1);
                    if (floor != 2000000.0f) {
                        if (EShadY != 2000000.0f)
                            floor = EShadY > floor ? EShadY : floor;
                        floor += 1.0f;
                        if (floor > position.y)
                            position.y = floor;
                    }
                    target.y = position.y - 0.5f;
                    goto rail_vehicle_focus;
                }
                if (WORLD->area != NULL && WORLD->area == PODSPRINT_ADATA) {
                    f32 countdown = PodSprint_InStartCountdown(WORLD);
                    f32 distance = countdown > 0.0f ? countdown / 3.0f * 10.0f + 2.0f : 2.0f;
                    if (player_count == 2)
                        distance *= 1.5f;
                    PodCamDist = distance;
                    NUVEC &average = mode_scratch;
                    average.x = 0.0f;
                    average.y = 0.0f;
                    average.z = 0.0f;
                    index = 0;
                    for (; index < player_count; ++index) {
                        GameObject_s *player = camera_players[index];
                        average.x += player->apiobj.position.x;
                        average.y += player->apiobj.collision_min.y + 0.65f;
                        average.z += player->apiobj.position.z;
                    }
                    if (player_count != 1)
                        NuVecScale(&average, &average, 1.0f / player_count);
                    target = average;
                    u16 yaw = camera_players[0]->apiobj.movement_facing_angle;
                    if (player_count == 2)
                        yaw += RotDiff(yaw, camera_players[1]->apiobj.movement_facing_angle) / player_count;
                    position.x = target.x - NU_SIN_LUT(yaw) * PodCamDist;
                    position.y = target.y + 0.65f;
                    position.z = target.z - NU_COS_LUT(yaw) * PodCamDist;
                    if (countdown <= 0.0f) {
                        index = player_roll[0];
                        if (player_count == 2)
                            index += RotDiff((u16)index, (u16)player_roll[1]) / player_count;
                        yaw += (i32)(index * 0.6f);
                        target.x = position.x + NU_SIN_LUT(yaw) * PodCamDist;
                        target.z = position.z + NU_COS_LUT(yaw) * PodCamDist;
                    }
                    NUVEC &floor_probe = mode_scratch;
                    floor_probe.x = position.x;
                    floor_probe.y = target.y - 0.65f;
                    floor_probe.z = position.z;
                    f32 floor = GameShadow(NULL, &floor_probe, 5.0f, -1);
                    if (floor != 2000000.0f) {
                        if (EShadY != 2000000.0f)
                            floor = EShadY > floor ? EShadY : floor;
                        floor += 0.65f;
                        if (floor > position.y)
                            position.y = floor;
                    }
                    position_seek = 10.0f;
                    angle_seek = 7.0f;
                    goto rail_vehicle_focus;
                }

                if (WORLD->current_level == BONUS_GUNSHIPB_LDATA && camera->sock_position.location.sock != -1) {
                    position.x += target.x * 0.0625f;
                    position.z += target.z * 0.0625f;
                } else if (WORLD->current_level == EMPERORFIGHTA_LDATA && player != NULL &&
                           player->sock_position.location.sock == 11 && WORLD->current_level != NULL)
                    position.y += EMPERORFIGHTA_CAMDYHACK;
            rail_vehicle_focus:
                if (landspeeder_lookahead != 0.0f) {
                    f32 opposite = static_cast<f32>(camera->yaw - 0x8000);
                    if (opposite < 0.0f)
                        opposite += 65536.0f;
                    NUVEC &offset = mode_scratch;
                    NuVecSub(&offset, &position, &target);
                    NuVecNorm(&offset, &offset);
                    NuVecScale(&offset, &offset, landspeeder_lookahead);
                    position.y += landspeeder_lookahead;
                    if (landspeeder_yaw > opposite - 4096.0f && landspeeder_yaw < opposite + 4096.0f) {
                        position.x += NU_SIN_LUT(static_cast<i32>(opposite)) * (landspeeder_speed * 0.5f) + offset.x;
                        position.z +=
                            NU_SIN_LUT(static_cast<i32>(opposite + 16384.0f)) * (landspeeder_speed * 0.5f) + offset.z;
                    } else if (landspeeder_yaw > static_cast<f32>(camera->yaw - 0x1000) &&
                               landspeeder_yaw < static_cast<f32>(camera->yaw + 0x1000)) {
                        position.x += NU_SIN_LUT(camera->yaw) * (landspeeder_speed * 0.75f) + offset.x;
                        position.z += NU_SIN_LUT(camera->yaw + 0x4000) * (landspeeder_speed * 0.75f) + offset.z;
                    } else {
                        position.x += offset.x;
                        position.z += offset.z;
                    }
                    GameCam_Blend(GameCam, 0.75f, 0.0f, 0);
                } else {
                    bool near_walker = false;
                    if ((SPEEDERCHASEA_LDATA != NULL && WORLD->current_level == SPEEDERCHASEA_LDATA) ||
                        (ENDORBATTLEC_LDATA != NULL && WORLD->current_level == ENDORBATTLEC_LDATA)) {
                        NUVEC average_position;
                        Players_AveragePos(&average_position, NULL);
                    } else if (id_ATST != -1) {
                        GameObject_s *walker = FindNearestGameObject(&position, NULL, 0, 5.0f, 0.0f, -1, id_ATST, -1,
                                                                     &walker_distance_sq, 0, NULL, false);
                        if (walker != NULL) {
                            bool is_player = false;
                            if (Player[0] != NULL && Player[0] == walker)
                                is_player = true;
                            if (Player[1] != NULL && Player[1] == walker)
                                is_player = true;
                            if (Player[2] != NULL && Player[2] == walker)
                                is_player = true;
                            if (Player[3] != NULL && Player[3] == walker)
                                is_player = true;
                            if (Player[4] != NULL && Player[4] == walker)
                                is_player = true;
                            if (Player[5] != NULL && Player[5] == walker)
                                is_player = true;
                            if (Player[6] != NULL && Player[6] == walker)
                                is_player = true;
                            if (Player[7] != NULL && Player[7] == walker)
                                is_player = true;
                            landspeeder_lookahead = is_player ? 4.0f : 2.0f;
                            f32 distance = NuFsqrt(walker_distance_sq);
                            f32 factor = MAX(0.0f, 1.0f - distance / 5.0f);
                            factor = (1.0f + NU_SIN_LUT(static_cast<i32>(factor * 32768.0f + 16384.0f))) * 0.5f;
                            NUVEC &offset = mode_scratch;
                            NuVecSub(&offset, &position, &target);
                            NuVecNorm(&offset, &offset);
                            f32 amount = landspeeder_lookahead * (1.0f - factor);
                            NuVecScale(&offset, &offset, amount);
                            position.y += amount;
                            position.x += offset.x;
                            position.z += offset.z;
                            near_walker = landspeeder_lookahead != 0.0f;
                        }
                    }
                    if (!near_walker && id_DEWBACK != -1) {
                        if (FindNearestGameObject(&position, NULL, 0, 5.0f, 0.0f, -1, id_DEWBACK, -1,
                                                  &walker_distance_sq, 0, NULL, false) != NULL) {
                            f32 distance = NuFsqrt(walker_distance_sq);
                            NUVEC &offset = mode_scratch;
                            NuVecSub(&offset, &position, &target);
                            NuVecNorm(&offset, &offset);
                            f32 amount = 1.5f * (1.0f - distance / 5.0f);
                            NuVecScale(&offset, &offset, amount);
                            position.y += amount;
                            position.x += offset.x;
                            position.z += offset.z;
                        }
                    }
                }
                if (TATOOINEE_LDATA != NULL && WORLD->current_level == TATOOINEE_LDATA &&
                    landspeeder_lookahead == 0.0f) {
                    NUVEC average_position;
                    NUVEC &offset = mode_scratch;
                    Players_AveragePos(&average_position, NULL);
                    NuVecSub(&offset, &rail_position, &average_position);
                    f32 magnitude = NuVecMagSqr(&offset);
                    f32 blend = 0.0f;
                    if (player2 == NULL) {
                        blend = magnitude / 125.0f;
                        if (blend > 0.75f)
                            blend = 0.6f;
                    }
                    f32 scale = 1.0f + NU_SIN_LUT(static_cast<i32>(blend * 16384.0f + 32768.0f + 16384.0f));
                    NuVecScale(&offset, &offset, scale);
                    offset.y = 0.0f;
                    NuVecSub(&position, &position, &offset);
                }
                stop_blend_rate = 0.1f;
                if (WORLD->current_level == PLATFORM_LDATA) {
                    position.x += 0.2f * NU_SIN_LUT(static_cast<u16>(static_cast<i32>(
                                             NuFmod(GameTimer.time_elapsed, 13.9815f) / 13.9815f * 65536.0f)));
                    position.y += stop_blend_rate * NU_SIN_LUT(static_cast<u16>(static_cast<i32>(
                                                        NuFmod(GameTimer.time_elapsed, 8.688f) / 8.688f * 65536.0f)));
                    position.z += 0.2f * NU_SIN_LUT(static_cast<u16>(static_cast<i32>(
                                             NuFmod(GameTimer.time_elapsed, 11.7745f) / 11.7745f * 65536.0f)));
                }
                if (WORLD->area != NULL && WORLD->area == HUB_ADATA && camera->sock_position.location.sock != -1)
                    position.y = WORLD->sock_sys->sock[camera->sock_position.location.sock].camera_height_above_ground;
                roll_hint_valid = 0;
                roll_hint = 0;
                roll_override = 0;
                roll_seek_override = 0.0f;
                if (WORLD->current_level->cam_tilt != 0.0f && camera->mode == camera->previous_mode) {
                    NUVEC forward;
                    NuVecRotateY(&forward, &v001, camera->yaw);
                    i32 total = 0;
                    index = 0;
                    do {
                        i32 roll = -RotDiff(0, player_roll[index]);
                        if (camera_players[index] != NULL)
                            total += static_cast<i32>(roll * (forward.x * camera_players[index]->facing_direction.x +
                                                              forward.z * camera_players[index]->facing_direction.z));
                        ++index;
                    } while (index < player_count);
                    f32 scale = WORLD->current_level->cam_tilt;
                    if (player_count != 1)
                        scale *= 1.0f / player_count;
                    roll_hint = static_cast<i32>(total * scale);
                    roll_hint_valid = 1;
                    roll_seek_override = 1.0f;
                    roll_override = -1;
                }
                if (BonusWinner != -1) {
                    NUVEC &offset = mode_scratch;
                    f32 distance = NuVecDist(&position, &target, &offset);
                    NuVecNorm(&offset, &offset);
                    NuVecScale(&offset, &offset, distance * 0.666f);
                    NuVecAdd(&position, &target, &offset);
                }
            } else {
                roll_hint_valid = 0;
                roll_hint = 0;
                roll_override = 0;
                roll_seek_override = 0.0f;
                stop_blend_rate = 0.1f;
            }
            pitch_override = -1;
            yaw_override = -1;
            movegamecamera_forcesock = -1;

            break;
        }
        case 2: {
            Minicam_Update();
            if (ObstacleCamCutCamPtr != NULL) {
                ObstacleCamCutPts[0] = *ObstacleCamCutCamPtr;
            }
            if (ObstacleCamCutTgtPtr != NULL) {
                ObstacleCamCutPts[1] = *ObstacleCamCutTgtPtr;
            }
            position = ObstacleCamSpl->pts[0];
            target = ObstacleCamSpl->pts[1];
            if (ObstacleCamAlwaysSnapAngles != 0) {
                position_seek = 10.0f;
                angle_seek = 10.0f;
            }
            roll_override = ObstacleCamRotZ != 0 ? static_cast<u16>(ObstacleCamRotZ) : 0;
            Hint_CancelCurrent();
            roll_hint_valid = 0;
            roll_hint = 0;
            roll_seek_override = 0.0f;
            yaw_override = -1;
            pitch_override = -1;
            stop_blend_rate = 0.1f;
            break;
        }

        case 3:
            position = WORLD->camera_splines[2]->pts[0];
            target = WORLD->camera_splines[2]->pts[1];
            break;
        case 4: {
            position = Door_CutCamPos0;
            target = Door_CutCamPos1;
            if (Door_CutLookAtPlayers != 0) {
                target.x = 0.0f;
                target.y = 0.0f;
                target.z = 0.0f;
                shared_count = 0;
                NUVEC player_focus;
                if (Player[0] != NULL && (static_cast<i8>(Player[0]->apiobj.flags_low) < 0 || LookAtBoth != 0)) {
                    PlayerCamPos(Player[0], &player_focus, &position);
                    NuVecAdd(&target, &target, &player_focus);
                    shared_count = 1;
                }
                if (Player[1] != NULL && (static_cast<i8>(Player[1]->apiobj.flags_low) < 0 || LookAtBoth != 0)) {
                    PlayerCamPos(Player[1], &player_focus, &position);
                    NuVecAdd(&target, &target, &player_focus);
                    ++shared_count;
                }
                if (shared_count != 0) {
                    NuVecScale(&target, &target, 1.0f / static_cast<f32>(shared_count));
                }
            }
            ComplexSockPosition(WORLD->sock_sys, &position, -1, -1, &camera->sock_position);
            roll_hint_valid = 0;
            roll_hint = 0;
            roll_seek_override = 0.0f;
            roll_override = 0;
            yaw_override = -1;
            pitch_override = -1;
            stop_blend_rate = 0.1f;
            break;
        }

        // Case 5 in MoveGameCamera. Inputs have already undergone the original
        // signed-stick conversion and deadzone handling in the common prologue.
        // `selected_pad` is the GAMEPAD_s at the original esp+0x108.
        // left_x/left_y correspond to xmm7/xmm6; right_y/right_x to esp+0xfc/+0x104.
        case 5: {
            static f32 addViewLDy = 0.0f;
            f32 speed = NuFabs(ViewCam.distance) * testmovef * FRAMETIME;
            if (ViewCam.gamepad != NULL) {
                offset.x = (left_x * 127.5f) * speed;
                offset.y = static_cast<f32>(static_cast<u32>(ViewCam.gamepad->pad->analog_l1)) * speed -
                           static_cast<f32>(static_cast<u32>(ViewCam.gamepad->pad->analog_l2)) * speed;
                offset.z = (-left_y * 127.5f) * speed;
                NuVecRotateY(&offset, &offset, camera->yaw);
                NuVecAdd(&ViewCam.target, &ViewCam.target, &offset);
                ViewCam.pitch = static_cast<i16>(ViewCam.pitch + static_cast<i32>(right_y * 16384.0f * FRAMETIME));
                ViewCam.pitch = MIN(0x4000, MAX(-0x4000, ViewCam.pitch));
                ViewCam.yaw -= static_cast<i16>(right_x * 16384.0f * FRAMETIME);
                if ((ViewCam.gamepad->pad->digital_buttons & GAMEPAD_SELECT) == 0) {
                    f32 zoom_step = 0.1f / ViewCam.zoom_scale;
                    ViewCam.distance -= static_cast<f32>(static_cast<u32>(ViewCam.gamepad->pad->analog_r1)) * zoom_step;
                    ViewCam.distance += static_cast<f32>(static_cast<u32>(ViewCam.gamepad->pad->analog_r2)) * zoom_step;
                    if ((selected_pad->pad->digital_buttons & 0x40) != 0) {
                        addViewLDy += FRAMETIME;
                    } else if ((selected_pad->pad->digital_buttons & 0x20) != 0) {
                        addViewLDy -= FRAMETIME;
                    } else {
                        addViewLDy = 0.0f;
                    }
                    ViewCam.distance += addViewLDy;
                    ViewCam.distance = NuFmax(0.1f, ViewCam.distance);
                }
            }
            stop_blend_rate = 0.1f;
            position = ViewCam.target;
            target = ViewCam.target;
            target.y += ViewCam.target_height;
            NuVecRotateX(&direction, &v001, ViewCam.pitch);
            NuVecRotateY(&direction, &direction, ViewCam.yaw);
            position.x += ViewCam.distance * direction.x;
            position.y += ViewCam.distance * direction.y;
            position.z += ViewCam.distance * direction.z;
            break;
        }

        // Mode 6, original0x111163..0x1111aa. WORLD is in esi at dispatch.
        // Shares the three sine offsets and common setup with customiser mode7;
        // it does NOT join the unmodified title-camera case3.
        case 6: {
            f32 *points = WORLD->portal_places[24]->positions;
            memcpy(&position, points, sizeof(position));
            memcpy(&target, points + 3, sizeof(target));
            goto customiser_camera_sway; // original0x111012
        }

        case 7: {
            position = WORLD->camera_splines[15]->pts[0];
            target = WORLD->camera_splines[15]->pts[1];
            Customiser_GetActiveWeirdoIndex(&index, &shared_count);
            if (shared_count == 1) {
                if (MenuPacket.reserved_0[index] != 0)
                    index = !index;
                target.x = CustomisePos[index].x;
                target.z = CustomisePos[index].z;
                NUVEC offset;
                NuVecSub(&offset, &target, &position);
                NuVecScale(&offset, &offset, 0.25f);
                NuVecAdd(&position, &position, &offset);
                target.y -= 0.075f;
            }
        customiser_camera_sway:
            position.x +=
                NU_SIN_LUT(static_cast<u16>((i32)(NuFmod(GameTimer.time_elapsed, 9.321f) / 9.321f * 65536.0f))) * 0.05f;
            position.y +=
                NU_SIN_LUT(static_cast<u16>((i32)(NuFmod(GameTimer.time_elapsed, 5.792f) / 5.792f * 65536.0f))) *
                0.025f;
            position.z +=
                NU_SIN_LUT(static_cast<u16>((i32)(NuFmod(GameTimer.time_elapsed, 7.183f) / 7.183f * 65536.0f))) *
                0.025f;
            roll_hint_valid = 0;
            roll_hint = 0;
            roll_seek_override = 0.0f;
            pitch_override = yaw_override = -1;
            roll_override = 0;
            stop_blend_rate = 0.1f;
            break;
        }
        case 8:
            position = *shopcampos;
            GetShopCamLookPos(&target);
            position.x += 0.1f * NU_SIN_LUT(static_cast<u16>(
                                     static_cast<i32>(NuFmod(GameTimer.time_elapsed, 9.321f) / 9.321f * 65536.0f)));
            position.y += 0.05f * NU_SIN_LUT(static_cast<u16>(
                                      static_cast<i32>(NuFmod(GameTimer.time_elapsed, 5.792f) / 5.792f * 65536.0f)));
            position.z += 0.05f * NU_SIN_LUT(static_cast<u16>(
                                      static_cast<i32>(NuFmod(GameTimer.time_elapsed, 7.183f) / 7.183f * 65536.0f)));
            break;
        case 9:
            position = hub_minikitviewer_camspl->pts[0];
            target = hub_minikitviewer_camspl->pts[1];
            target.y += HUB_MINIKITVIEWER_CAMDY;
            position_seek = 10.0f;
            angle_seek = 10.0f;
            roll_hint_valid = 0;
            roll_hint = 0;
            roll_seek_override = 0.0f;
            pitch_override = yaw_override = -1;
            roll_override = 0;
            stop_blend_rate = 0.1f;
            break;
        case 11: {
            f32 spline_position =
                (GamePlayTimer.time_elapsed + (reinterpret_cast<u8 *>(LevFlag)[0] == 2 ? 51.0f : 6.0f)) / 100.0f;
            f32 first_position = spline_position;
            if (spline_position > 0.95f) {
                reinterpret_cast<u8 *>(LevFlag)[1] = 1;
                first_position = 0.95f;
            }
            u16 yaw;
            PointAlongSpline(WORLD->sock_sys->sock[0].mid, first_position, &target, &yaw, NULL, 0);
            NUVEC ahead;
            PointAlongSpline(WORLD->sock_sys->sock[0].mid, spline_position > 1.0f ? 1.0f : spline_position, &ahead,
                             NULL, NULL, 0);
            NUVEC lateral;
            NuVecRotateY(&lateral, &v001, yaw);
            GunshipANorm = lateral;
            NuVecRotateY(&lateral, &lateral, 0x4000);
            NUVEC &offset = mode_scratch;
            offset.z = 0.0f;
            offset.x = 0.0f;
            offset.y = 0.0f;
            shared_count = 0;
            if (Player[0] != NULL) {
                f32 projection =
                    (Player[0]->apiobj.pos_x - target.x) * lateral.x + (Player[0]->apiobj.pos_z - target.z) * lateral.z;
                offset.x = lateral.x * projection + offset.x;
                offset.z = projection * lateral.z + offset.z;
                shared_count = 1;
            }
            if (Player[1] != NULL) {
                f32 projection =
                    (Player[1]->apiobj.pos_x - target.x) * lateral.x + (Player[1]->apiobj.pos_z - target.z) * lateral.z;
                offset.x = lateral.x * projection + offset.x;
                offset.z = projection * lateral.z + offset.z;
                ++shared_count;
                if (shared_count == 2) {
                    offset.x *= 0.5f;
                    offset.z *= 0.5f;
                }
            }
            NuVecAdd(&position, &target, &GunshipAOffset);
            position.x += offset.x;
            position.z += offset.z;
            target.x = ahead.x + offset.x;
            target.z = ahead.z + offset.z;
            NuVecSub(&offset, &target, &position);
            pitch_override = -NuAtan2D(offset.y, NuFsqrt(offset.x * offset.x + offset.z * offset.z));
            yaw_override = NuAtan2D(offset.x, offset.z);
            position_seek = 8.0f;
            roll_hint_valid = 0;
            roll_hint = 0;
            roll_seek_override = 0.0f;
            roll_override = 0;
            stop_blend_rate = 0.1f;
            break;
        }

        default:
            break;
    }
    if (WORLD->current_level == SARLACCPITA_LDATA || WORLD->current_level == SARLACCPITC_LDATA) {
        position.x +=
            stop_blend_rate *
            NU_SIN_LUT(static_cast<u16>(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 9.321f) / 9.321f * 65536.0f)));
        position.y +=
            0.05f *
            NU_SIN_LUT(static_cast<u16>(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 5.792f) / 5.792f * 65536.0f)));
        position.z +=
            0.05f *
            NU_SIN_LUT(static_cast<u16>(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 7.183f) / 7.183f * 65536.0f)));
    }
    // Entry locals: position, target, previous_sock, previous_candidate_count,
    // blend_duration=.5f, position_seek, angle_seek, pitch_override/yaw_override=-1,
    // roll_override=0, roll_hint_valid=0, roll_hint=0, roll_seek_override=0,
    // stop_blend_rate=.1f. Caller has already copied old mode to previous_mode.
    if (camera->mode == previous_mode) {
        if (previous_sock == -1 || camera->sock_position.location.sock == -1) {
            if (previous_sock != camera->sock_position.location.sock && WORLD->current_level != DOGFIGHTA_LDATA)
                GameCam_Blend(camera, 1.0f, 0.0f, 1);
        } else if (camera->sock_position.candidate_count != previous_candidate_count ||
                   camera->sock_position.location.sock != previous_sock) {
            GameCam_Blend(camera, blend_duration, 0.0f, 1);
        }
    }
    if (camera->reset_blend != 0) {
        camera->blend_destination_position = position;
        camera->reset_blend = 0;
        camera->blend_destination_target = target;
    }
    f32 blend = 0.0f, angle_blend = 0.0f;
    if (camera->blend_duration > camera->blend_time) {
        if (camera->blend_curve > 0.0f)
            camera->blend_curve -= FRAMETIME;
        else {
            camera->blend_time += FRAMETIME;
            if (camera->blend_time > camera->blend_duration)
                camera->blend_time = camera->blend_duration;
        }
        if (camera->blend_duration > camera->blend_time) {
            blend = camera->blend_time / camera->blend_duration;
            angle_blend = 2.0f * blend;
            if (angle_blend > 1.0f)
                angle_blend = 1.0f;
            camera->blend_start_position.x =
                camera->blend_end_position.x +
                (camera->blend_destination_position.x - camera->blend_end_position.x) * blend;
            camera->blend_start_position.y =
                camera->blend_end_position.y +
                (camera->blend_destination_position.y - camera->blend_end_position.y) * blend;
            camera->blend_start_position.z =
                camera->blend_end_position.z +
                (camera->blend_destination_position.z - camera->blend_end_position.z) * blend;
            camera->blend_start_target.x =
                camera->blend_end_target.x + (camera->blend_destination_target.x - camera->blend_end_target.x) * blend;
            camera->blend_start_target.y =
                camera->blend_end_target.y + (camera->blend_destination_target.y - camera->blend_end_target.y) * blend;
            camera->blend_start_target.z =
                camera->blend_end_target.z + (camera->blend_destination_target.z - camera->blend_end_target.z) * blend;
        }
    }
    camera->angle_seek = angle_seek;
    camera->position_seek = position_seek;
    if (camera->mode != previous_mode && camera->blend_time >= camera->blend_duration)
        camera->pos = position;
    else {
        if (camera->blend_duration > camera->blend_time) {
            position.x = camera->blend_start_position.x + (position.x - camera->blend_start_position.x) * blend;
            position.y = camera->blend_start_position.y + (position.y - camera->blend_start_position.y) * blend;
            position.z = camera->blend_start_position.z + (position.z - camera->blend_start_position.z) * blend;
        }
        f32 seek = position_seek * FRAMETIME;
        if (seek > 1.0f)
            seek = 1.0f;
        seek *= CamStopBlend;
        camera->pos.x += (position.x - camera->pos.x) * seek;
        camera->pos.y += (position.y - camera->pos.y) * seek;
        camera->pos.z += (position.z - camera->pos.z) * seek;
    }
    camera->desired_position = position;
    camera->target = target;
    const NUVEC position_snapshot = camera->pos;
    const NUVEC target_snapshot = camera->target;
    f32 dx = target_snapshot.x - position_snapshot.x;
    f32 dz = target_snapshot.z - position_snapshot.z;
    i32 pitch = pitch_override;
    if (pitch_override == -1)
        pitch = static_cast<u16>(-NuAtan2D(target_snapshot.y - position_snapshot.y, NuFsqrt(dx * dx + dz * dz)));
    if (camera->blend_duration > camera->blend_time && camera->blend_mode == 2)
        pitch = camera->blend_start_pitch + static_cast<i32>(RotDiff(camera->blend_start_pitch, pitch) * angle_blend);
    camera->desired_pitch = pitch;
    i32 yaw = yaw_override;
    if (yaw_override == -1)
        yaw = NuAtan2D(dx, dz);
    if (camera->blend_duration > camera->blend_time && camera->blend_mode == 2)
        yaw = camera->blend_start_yaw + static_cast<i32>(RotDiff(camera->blend_start_yaw, yaw) * angle_blend);
    camera->desired_yaw = yaw;
    i32 roll = roll_override;
    if (roll_override == -1)
        roll = roll_hint_valid ? roll_hint : 0;
    if (camera->blend_duration > camera->blend_time && camera->blend_mode == 2)
        roll = camera->blend_start_roll + static_cast<i32>(RotDiff(camera->blend_start_roll, roll) * angle_blend);
    camera->desired_roll = roll;
    if (camera->mode != previous_mode && camera->blend_time >= camera->blend_duration) {
        camera->pitch = pitch;
        camera->yaw = yaw;
        camera->roll = roll;
    } else {
        if (pitch_override == -1) {
            if (((camera->pitch - pitch) & 0xffff) > 0x7fff) {
                CamStopBlend += FRAMETIME;
                if (CamStopBlend > 1.0f)
                    CamStopBlend = 1.0f;
            }
            f32 pitch_seek = camera->angle_seek;
            camera->pitch = SeekRot(camera->pitch, pitch, NuFsqrt(CamStopBlend) * pitch_seek);
        } else
            camera->pitch = pitch_override;
        if (yaw_override == -1)
            camera->yaw = SeekRot(camera->yaw, yaw, camera->angle_seek * CamStopBlend);
        else
            camera->yaw = yaw_override;
        if (roll_override == -1)
            roll = SeekRot(camera->roll, roll,
                           (roll_seek_override > 0.0f ? roll_seek_override : camera->angle_seek) * CamStopBlend);
        else
            roll = roll_override;
        camera->roll = roll;
        pitch = camera->pitch;
        yaw = camera->yaw;
    }
    CamStopBlend += stop_blend_rate * FRAMETIME;
    // Preserve NaNs on the quiet unordered path before clamping.
    CamStopBlend = __builtin_isnan(CamStopBlend) ? CamStopBlend : MIN(1.0f, CamStopBlend);
    GameCam_UpdateLookRot(camera);

    NUMTX orientation = {};
    shared_count = 0;
    for (i32 i = 0; i < 2; i++) {
        if (Player[i] != NULL && static_cast<i8>(Player[i]->apiobj.flags_low) < 0 && Player[i]->field_0x1086 == 4) {
            NuVecAdd(NUMTX_GET_ROW_VEC(&orientation, 0), NUMTX_GET_ROW_VEC(&orientation, 0),
                     NUMTX_GET_ROW_VEC(&Player[i]->vehicle_orientation, 0));
            NuVecAdd(NUMTX_GET_ROW_VEC(&orientation, 1), NUMTX_GET_ROW_VEC(&orientation, 1),
                     NUMTX_GET_ROW_VEC(&Player[i]->vehicle_orientation, 1));
            NuVecAdd(NUMTX_GET_ROW_VEC(&orientation, 2), NUMTX_GET_ROW_VEC(&orientation, 2),
                     NUMTX_GET_ROW_VEC(&Player[i]->vehicle_orientation, 2));
            shared_count++;
        }
    }
    if (shared_count > 0) {
        NuMtxSetIdentity(&camera->mtx);
        if (shared_count > 1) {
            f32 scale = 1.0f / shared_count;
            NuVecScale(NUMTX_GET_ROW_VEC(&orientation, 0), NUMTX_GET_ROW_VEC(&orientation, 0), scale);
            NuVecScale(NUMTX_GET_ROW_VEC(&orientation, 1), NUMTX_GET_ROW_VEC(&orientation, 1), scale);
            NuVecScale(NUMTX_GET_ROW_VEC(&orientation, 2), NUMTX_GET_ROW_VEC(&orientation, 2), scale);
            NuVecNorm(NUMTX_GET_ROW_VEC(&orientation, 0), NUMTX_GET_ROW_VEC(&orientation, 0));
            NuVecNorm(NUMTX_GET_ROW_VEC(&orientation, 1), NUMTX_GET_ROW_VEC(&orientation, 1));
            NuVecNorm(NUMTX_GET_ROW_VEC(&orientation, 2), NUMTX_GET_ROW_VEC(&orientation, 2));
        }
        *NUMTX_GET_ROW_VEC(&camera->mtx, 0) = *NUMTX_GET_ROW_VEC(&orientation, 0);
        *NUMTX_GET_ROW_VEC(&camera->mtx, 1) = *NUMTX_GET_ROW_VEC(&orientation, 1);
        *NUMTX_GET_ROW_VEC(&camera->mtx, 2) = *NUMTX_GET_ROW_VEC(&orientation, 2);
        NuMtxTranslate(&camera->mtx, &camera->pos);
        camera->render_mtx = camera->mtx;
        NuMtxPreRotateZ(&camera->render_mtx, roll);
        NuVecNorm(NUMTX_GET_ROW_VEC(&camera->render_mtx, 0), NUMTX_GET_ROW_VEC(&camera->render_mtx, 0));
        NuVecNorm(NUMTX_GET_ROW_VEC(&camera->render_mtx, 1), NUMTX_GET_ROW_VEC(&camera->render_mtx, 1));
        NuVecNorm(NUMTX_GET_ROW_VEC(&camera->render_mtx, 2), NUMTX_GET_ROW_VEC(&camera->render_mtx, 2));
        camera->target_mtx = camera->render_mtx;
    } else {
        NuMtxSetRotationZ(&camera->mtx, roll);
        NuMtxRotateX(&camera->mtx, pitch + static_cast<u16>(camera->field_0x214));
        NuMtxRotateY(&camera->mtx, yaw + static_cast<u16>(camera->field_0x218));
        NuMtxTranslate(&camera->mtx, &camera->pos);
        camera->render_mtx = camera->mtx;
        NuMtxSetRotationZ(&camera->target_mtx, roll);
        NuMtxRotateX(&camera->target_mtx, pitch);
        NuMtxRotateY(&camera->target_mtx, yaw);
        NuMtxTranslate(&camera->target_mtx, &camera->pos);
    }
    if (camera->judder_time > 0.0f) {
        camera->judder_time -= FRAMETIME;
        if (camera->judder_time > 0.0f) {
            i32 degrees = (static_cast<i32>(camera->judder_duration / 0.333f) + 1) * 360;
            i32 phase = static_cast<i32>(((degrees * 0x10000) / 360) *
                                         ((camera->judder_duration - camera->judder_time) / camera->judder_duration));
            f32 amount = (camera->judder_time / camera->judder_duration) *
                         ((546.0f * camera->judder_duration) * NU_SIN_LUT(phase));
            if (camera->judder_reverse)
                amount = -amount;
            if (camera->judder_axis == 0)
                NuMtxPreRotateX(&camera->render_mtx, static_cast<u16>(static_cast<i32>(amount)));
            else if (camera->judder_axis == 1)
                NuMtxPreRotateY(&camera->render_mtx, static_cast<u16>(amount));
            else
                NuMtxPreRotateZ(&camera->render_mtx, static_cast<u16>(amount));
        }
    }
    GIZMO *window = GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, "window_frame1");
    if (Cheat_PowerUpActive(-1) ||
        (WORLD->current_level == SPEEDERCHASEA_LDATA && !disable_narrow_socks &&
         camera->sock_position.location.sock == 1) ||
        (WORLD->current_level == CLOUDCITYTRAPB_LDATA && window != NULL && window->object != NULL &&
         (static_cast<GIZMOBLOWUP_s *>(window->object)->output_flags & 1) != 0 &&
         static_cast<u8>(camera->sock_position.location.sock) <= 1))
        camera_shake = 0.5f;
    else if (WORLD->current_level == DEATHSTAR2BATTLED_LDATA && reinterpret_cast<u8 *>(LevFlag)[0] != 0)
        camera_shake = 1.5f;
    if (VehicleArea && player != NULL && (player->apiobj.character_data->model_flags & 0x2000) != 0 &&
        player->combo_input_timer > 0.0f && Cheat_IsOn(0x14))
        camera_shake = 0.6f;
    else {
        for (i32 i = 0; i < 2; i++) {
            GameObject_s *object = Player[i];
            if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0) {
                if (WORLD->current_level == DEATHSTARBATTLED_LDATA &&
                    ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx)) {
                    camera_shake = 0.6f;
                    break;
                }
                object = Player[i];
                if (object->camera_shake_strength > 0.0f && WORLD->area != NULL &&
                    (WORLD->area == PODRACE_ADATA || WORLD->area == PODSPRINT_ADATA)) {
                    camera_shake = object->camera_shake_strength * 2.0f;
                    break;
                }
            }
        }
    }
    GameCam_UpdateShake(camera, camera_shake);
    camera->shaken_right = *NUMTX_GET_ROW_VEC(&camera->render_mtx, 0);
    camera->shaken_up = *NUMTX_GET_ROW_VEC(&camera->render_mtx, 1);
    camera->dir = *NUMTX_GET_ROW_VEC(&camera->render_mtx, 2);
    pNuCam->mtx = camera->render_mtx;
    NuCameraSet(pNuCam);
    MakePlayPlanes(camera);
    if (!VehicleArea) {
        for (i32 i = 0; i < 2; i++) {
            GameObject_s *object = Player[i];
            if (object == NULL || static_cast<i8>(object->apiobj.flags_low) >= 0)
                continue;
            if (FadeSys.fade != 0.0f || object->apiobj.field_0x287 != 0 || object->character_context == 0x2b ||
                object->timer_d5c > 0.0f || GetMenuID() != -1 || object->character_context == 0x0f ||
                object->field_0xcc0 != NULL || (object->field_0xe24 & 8) == 0 ||
                ((object->apiobj.character_data->model_flags & 0x40) != 0 && object->communicate_blend > 0.0f)) {
                object->field_0xef0 = 0.0f;
                object->pause_context_state = 0.0f;
                object->field_0xefe &= ~0x18;
            } else {
                i32 was_blocked = (object->field_0xefe >> 3) & 1;
                bool changed = false;
                if ((GameTimer.update_count & 1) == i) {
                    NUVEC sample;
                    sample.x = 0.0f;
                    sample.y = 0.0f;
                    sample.z = (qrand() * (1.0f / 65535.0f)) * object->apiobj.field_0x1dc;
                    NuVecRotateX(&sample, &sample, static_cast<i32>((qrand() * (1.0f / 65535.0f)) * -16384.0f));
                    sample.y *= object->apiobj.field_0x1e0 / object->apiobj.field_0x1dc;
                    NuVecRotateY(&sample, &sample, qrand());
                    NuVecAdd(&sample, &sample, &object->apiobj.collision_position);
                    NUVEC ray;
                    NuVecSub(&ray, &sample, &GameCam->pos);
                    i32 blocked = GameRayCast(&GameCam->pos, &ray, 0.0f, 0x1f) & 1;
                    object->field_0xefe = (object->field_0xefe & ~8) | (blocked << 3);
                    if (was_blocked != blocked) {
                        object->field_0xef0 = 0.0f;
                        changed = true;
                    }
                }
                if (!changed && object->field_0xef0 < 0.3f) {
                    object->field_0xef0 += FRAMETIME;
                    if (object->field_0xef0 >= 0.3f) {
                        object->field_0xef0 = 0.3f;
                        object->field_0xefe = (object->field_0xefe & ~0x10) | (was_blocked << 4);
                    }
                }
            }
            object->pause_context_state =
                SeekLinearF(object->pause_context_state, (object->field_0xefe & 0x10) ? 1.0f : 0.0f, 3.0f * FRAMETIME);
        }
    }
}

void InitCameraTargetMaterial() {
}

void DrawCameraTarget(nuvec_s *) {
}

void DrawCameraTarget2(nuvec_s *) {
}

i32 GoingForwardsAlongNarrowSock(GameObject_s *object) {
    if (!object->in_narrow_socket)
        return 0;
    i32 delta = RotDiff(object->yrot, object->apiobj.field_0x276);
    if (delta < 0)
        delta = -delta;
    i32 forwards = delta < 0x4000;
    if (object->character_context == 0x2a && object->airborne_action_duration * 0.8f > object->context_animation_timer)
        forwards = 1 - forwards;
    return forwards;
}

