// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include <string.h>
#include "legoapi/characters/motion.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nutrig.h"

#include "globals.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/misc/utilities.h"
#include "nu2api/numath/nuvec.h"
#include "decomp.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/world/levels/levels.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/hint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <stdio.h>
#include <math.h>
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
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
#include "legoapi/characters/motion/camera.h"

MINICAM_s MiniCam = {};

nugspline_s ObstacleCamCutSpline;
i32 ObstacleCamTargetGuid;

void Minicam_InitSystem() {
    memset(&MiniCam, 0, sizeof(MiniCam));
}

void Minicam_AddCommand(i32 type, f32 duration, i32 argument, void *target, nuvec_s position) {
    if (type == 6 || type == 7) {
        i32 i = MiniCam.command_count;
        for (; i >= 0 && MiniCam.commands[i].type != type; --i) {
        }
        if (i != -1 && MiniCam.commands[i].argument == argument)
            return;
    }
    if (MiniCam.command_count == 32)
        return;
    switch (type) {
        case 1:
            MiniCam.commands[MiniCam.command_count].type = 1;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = duration;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 3:
            MiniCam.commands[MiniCam.command_count].type = 3;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = duration;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 4:
            MiniCam.commands[MiniCam.command_count].type = 4;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = duration;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 5:
            MiniCam.commands[MiniCam.command_count].type = 5;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = duration;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 6:
            MiniCam.commands[MiniCam.command_count].type = 6;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 7:
            MiniCam.commands[MiniCam.command_count].type = 7;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 8:
            MiniCam.commands[MiniCam.command_count].type = 8;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = (NUVEC *)target;
            break;
        case 9:
            MiniCam.commands[MiniCam.command_count].type = 9;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = duration;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 10:
            MiniCam.commands[MiniCam.command_count].type = 10;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 11:
            MiniCam.commands[MiniCam.command_count].type = 11;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 12:
            MiniCam.commands[MiniCam.command_count].type = 12;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 13:
            MiniCam.commands[MiniCam.command_count].type = 13;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 14:
            MiniCam.commands[MiniCam.command_count].type = 14;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 15:
            MiniCam.commands[MiniCam.command_count].type = 15;
            MiniCam.commands[MiniCam.command_count].argument = argument;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 16:
            MiniCam.commands[MiniCam.command_count].position = position;
            MiniCam.commands[MiniCam.command_count].type = 16;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].target = NULL;
            break;
        case 17:
            MiniCam.commands[MiniCam.command_count].type = 17;
            MiniCam.commands[MiniCam.command_count].argument = 0;
            MiniCam.commands[MiniCam.command_count].duration = 0.0f;
            MiniCam.commands[MiniCam.command_count].position = v000;
            MiniCam.commands[MiniCam.command_count].target = (NUVEC *)target;
            break;
    }
    ++MiniCam.command_count;
}

void MiniCam_ChangeMode(i32 mode) {
    if (mode == 0) {
        NUVEC delta;
        MiniCam.distance = NuVecDist(&MiniCam.target, &MiniCam.position, &delta);
        FindAnglesXY(&delta, &MiniCam.pitch, &MiniCam.yaw);
        MiniCam.yaw += 0x8000;
        MiniCam.pitch = -MiniCam.pitch;
        MiniCam.mode = 0;
    } else if (mode == 1) {
        MiniCam.mode = 1;
    } else if (mode == 2) {
        MiniCam.mode = 2;
    }
}

void Minicam_AddDeltas(float duration) {
    f32 fraction;
    if ((duration == 0.0f) | (MiniCam.delta_time == 0.0f)) {
        fraction = 0.0f;
    } else {
        fraction = MiniCam.delta_time / duration;
        if (fraction < 0.0f)
            fraction = 0.0f;
        else if (fraction > 1.0f)
            fraction = 1.0f;
    }
    switch (MiniCam.easing) {
        case 1:
            fraction = 1.0f + NU_SIN_LUT(fraction * 16384.0f + 32768.0f + 16384.0f);
            break;
        case 2:
            fraction = 1.0f - (NU_SIN_LUT(fraction * 32768.0f + 16384.0f) + 1.0f) * 0.5f;
            break;
        case 3:
            fraction = NU_SIN_LUT(fraction * 16384.0f + 49152.0f + 16384.0f);
            break;
        case 4:
            if (fraction < 0.5f) {
                fraction = NU_SIN_LUT((fraction + fraction) * 16384.0f) * 0.5f;
            } else if (fraction > 0.5f) {
                fraction =
                    (1.0f - NU_SIN_LUT(((fraction - 0.5f) + (fraction - 0.5f)) * 16384.0f + 16384.0f)) * 0.5f + 0.5f;
            } else {
                fraction = 0.5f;
            }
            break;
    }
    if (MiniCam.mode == 0) {
        i32 pitch_delta = MiniCam.target_pitch - MiniCam.start_pitch;
        if (pitch_delta > 32768)
            pitch_delta -= 65536;
        i32 yaw_delta = MiniCam.target_yaw - MiniCam.start_yaw;
        if (yaw_delta > 32768)
            yaw_delta -= 65536;
        i32 roll_delta = MiniCam.target_roll - MiniCam.start_roll;
        if (roll_delta > 32768)
            roll_delta -= 65536;
        MiniCam.pitch = SeekRot(MiniCam.pitch, (u16)(i32)(MiniCam.start_pitch + pitch_delta * fraction), 10.0f);
        MiniCam.yaw = SeekRot(MiniCam.yaw, (u16)(i32)(MiniCam.start_yaw + yaw_delta * fraction), 10.0f);
        MiniCam.roll = SeekRot(MiniCam.roll, (u16)(i32)(MiniCam.start_roll + roll_delta * fraction), 10.0f);
        MiniCam.distance = (MiniCam.target_distance - MiniCam.start_distance) * fraction + MiniCam.start_distance;
    } else if (MiniCam.mode == 1) {
        MiniCam.position.x += MiniCam.position_velocity.x * FRAMETIME;
        MiniCam.position.y += MiniCam.position_velocity.y * FRAMETIME;
        MiniCam.position.z += MiniCam.position_velocity.z * FRAMETIME;
    } else if (MiniCam.mode == 2) {
        PointAlongSpline(MiniCam.position_spline, fraction, &MiniCam.position, NULL, NULL, 0);
    }
    MiniCam.focus_offset.x -= MiniCam.focus_velocity.x * FRAMETIME;
    MiniCam.focus_offset.y -= MiniCam.focus_velocity.y * FRAMETIME;
    MiniCam.focus_offset.z -= MiniCam.focus_velocity.z * FRAMETIME;
    if (MiniCam.focus != NULL) {
        NUVEC target;
        NuVecAdd(&target, MiniCam.focus, &MiniCam.focus_offset);
        MiniCam.target.x = SeekLinearF(MiniCam.target.x, target.x, 10.0f);
        MiniCam.target.y = SeekLinearF(MiniCam.target.y, target.y, 10.0f);
        MiniCam.target.z = SeekLinearF(MiniCam.target.z, target.z, 10.0f);
    }
}

void Minicam_ClearDeltas() {
    MiniCam.target_distance = MiniCam.start_distance = MiniCam.distance;
    MiniCam.target_pitch = MiniCam.start_pitch = MiniCam.pitch;
    MiniCam.target_yaw = MiniCam.start_yaw = MiniCam.yaw;
    MiniCam.target_roll = MiniCam.start_roll = MiniCam.roll;
    MiniCam.focus_velocity = MiniCam.focus_offset = v000;
}

void Minicam_ResetForNextCommand() {
    Minicam_ClearDeltas();

    i32 count = 0;
    for (i32 i = MiniCam.current_command + 1; i < MiniCam.command_count; ++i) {
        MiniCam.commands[count++] = MiniCam.commands[i];
    }

    MiniCam.command_count = static_cast<u8>(count);
    memset(&MiniCam.commands[MiniCam.command_count], 0,
           static_cast<usize>(32 - MiniCam.command_count) * sizeof(MINICAMCOMMAND_s));
    MiniCam.current_command = 0;
    MiniCam.delta_time = 0.0f;
}

void Minicam_ResetForNewCut() {
    memset(&MiniCam, 0, sizeof(MiniCam));
}

void Minicam_CalcCamPos() {
    if (MiniCam.mode == 0) {
        MiniCam.position.x = 0.0f;
        MiniCam.position.y = 0.0f;
        MiniCam.position.z = MiniCam.distance;
        NuVecRotateX(&MiniCam.position, &MiniCam.position, MiniCam.pitch);
        NuVecRotateY(&MiniCam.position, &MiniCam.position, MiniCam.yaw);
        ObstacleCamRotZ = MiniCam.roll;
        NuVecAdd(&MiniCam.position, &MiniCam.position, &MiniCam.target);
    }
}

void Minicam_Update() {
    f32 distance = MiniCam.distance;
    i32 yaw = MiniCam.yaw;
    i32 pitch = MiniCam.pitch;
    NUVEC *focus = MiniCam.focus;
    i32 roll = MiniCam.roll;
    NUVEC position = MiniCam.position;
    i32 pitch_rate = 0, yaw_rate = 0, roll_rate = 0;
    f32 command_duration;
read_command:
    if (MiniCam.current_command < MiniCam.command_count)
        goto dispatch_command;
    command_duration = 0.0f;
execute_command:
    MiniCam.elapsed_time += FRAMETIME;
    if (MiniCam.duration != 0.0f && MiniCam.elapsed_time > MiniCam.duration) {
        Minicam_AddDeltas(command_duration);
        goto end_cut;
    }
    if (MiniCam.current_command >= MiniCam.command_count) {
        Minicam_AddDeltas(1000000000.0f);
        Minicam_CalcCamPos();
        return;
    }
    if (MiniCam.commands[MiniCam.current_command].type == 1) {
        if (command_duration <= MiniCam.delta_time) {
            NUVEC delta;
            MiniCam.distance = MiniCam.start_distance =
                NuVecDist(NUMTX_GET_ROW_VEC(&GameCam->render_mtx, 3), MiniCam.focus, &delta);
            MiniCam.pitch = MiniCam.start_pitch = GameCam->pitch + 0x8000;
            MiniCam.yaw = MiniCam.start_yaw = GameCam->yaw;
            MiniCam.position = GameCam->pos;
            Minicam_ResetForNextCommand();
        } else {
            MiniCam.delta_time += FRAMETIME;
        }
        return;
    }
    if (MiniCam.commands[MiniCam.current_command].type == 3) {
        if (command_duration > MiniCam.delta_time) {
            MiniCam.delta_time += FRAMETIME;
            Minicam_AddDeltas(command_duration);
            Minicam_CalcCamPos();
            return;
        }
        ObstacleCamAlwaysSnapAngles = 0;
        goto end_cut;
    }
    if (MiniCam.commands[MiniCam.current_command].type == 4 && MiniCam.delta_time == 0.0f) {
        if (command_duration == 0.0f) {
            MiniCam.focus = focus;
            MiniCam.distance = distance;
            MiniCam.pitch = pitch;
            MiniCam.yaw = yaw;
            MiniCam.roll = roll;
            MiniCam.focus_offset = v000;
            MiniCam.position = position;
            Minicam_ClearDeltas();
            GameCam->mode = -1;
        } else {
            if (pitch_rate != 0)
                MiniCam.target_pitch = (i32)(pitch_rate * command_duration);
            else
                MiniCam.target_pitch = pitch;
            MiniCam.start_pitch = MiniCam.pitch;
            if (yaw_rate != 0)
                MiniCam.target_yaw = (i32)(yaw_rate * command_duration);
            else
                MiniCam.target_yaw = yaw;
            MiniCam.start_yaw = MiniCam.yaw;
            if (__builtin_expect(roll_rate != 0, 0))
                MiniCam.target_roll = (i32)(roll_rate * command_duration);
            else
                MiniCam.target_roll = roll;
            MiniCam.target_distance = distance;
            MiniCam.start_roll = MiniCam.roll;
            MiniCam.start_distance = MiniCam.distance;
            if (MiniCam.focus != focus) {
                NuVecSub(&MiniCam.focus_offset, MiniCam.focus, focus);
                NuVecScale(&MiniCam.focus_velocity, &MiniCam.focus_offset, 1.0f / command_duration);
                MiniCam.focus = focus;
            }
            NuVecSub(&MiniCam.position_velocity, &position, &MiniCam.position);
            NuVecScale(&MiniCam.position_velocity, &MiniCam.position_velocity, 1.0f / command_duration);
        }
    }
    MiniCam.delta_time += FRAMETIME;
    if (MiniCam.delta_time < command_duration) {
        Minicam_AddDeltas(command_duration);
    } else {
        Minicam_AddDeltas(command_duration);
        Minicam_ResetForNextCommand();
    }
    Minicam_CalcCamPos();
    return;
end_cut:
    Minicam_CalcCamPos();
    ObstacleCamEnd = 0.0f;
    if (ObstacleCamBlendOutTime == 0.0f) {
        GameCam->mode = -1;
        Minicam_ResetForNextCommand();
    }
    if (MiniCutCam == 0)
        Minicam_ResetForNextCommand();
    return;
dispatch_command: {
    MINICAMCOMMAND_s *command = &MiniCam.commands[MiniCam.current_command];
    switch (command->type) {
        case 0:
            return;
        case 1:
        case 3:
        case 4:
            command_duration = command->duration;
            goto execute_command;
        case 2:
            command_duration = 0.0f;
            goto execute_command;
        case 5:
            command_duration = command->duration;
            Minicam_ClearDeltas();
            goto execute_command;
        case 6: {
            MiniCam_ChangeMode(command->argument);
            i32 old_yaw = MiniCam.target_yaw;
            u16 target_pitch = pitch_rate != 0 ? (i32)(pitch_rate * 0.0f) : pitch;
            f32 old_distance = MiniCam.target_distance;
            u16 old_roll = MiniCam.target_roll;
            MiniCam.target_pitch = target_pitch;
            MiniCam.start_pitch = MiniCam.pitch;
            Minicam_ClearDeltas();
            if (pitch == target_pitch)
                pitch = MiniCam.target_pitch;
            if (yaw == old_yaw)
                yaw = MiniCam.target_yaw;
            if (roll == old_roll)
                roll = MiniCam.target_roll;
            if (distance == old_distance)
                distance = MiniCam.target_distance;
            break;
        }
        case 7:
            MiniCam.easing = command->argument;
            break;
        case 8:
            focus = command->target;
            break;
        case 9:
            distance = command->duration;
            break;
        case 10:
            pitch = command->argument;
            break;
        case 11:
            yaw = command->argument;
            break;
        case 12:
            roll = command->argument;
            break;
        case 13:
            pitch_rate = command->argument;
            break;
        case 14:
            yaw_rate = command->argument;
            break;
        case 15:
            roll_rate = command->argument;
            break;
        case 16:
            position = command->position;
            break;
        case 17:
            MiniCam.position_source = command->target;
            break;
        default:
            ++MiniCam.current_command;
            return;
    }
    ++MiniCam.current_command;
    goto read_command;
}
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

