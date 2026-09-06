#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/technos.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

void MovePlayer_DIRECTIONAL(GameObject_s *object);
void MovePlayer_VEHICLEDIRECTIONAL(GameObject_s *object);
i32 MovePlayer_TWIST(GameObject_s *object);
i32 MovePlayer_CIRCLE(GameObject_s *object);
i32 MovePlayer_GUNSHIPIN(GameObject_s *object);
i32 MovePlayer_POD(GameObject_s *object);
void ApplyGravity(GameObject_s *object, float *gravity, float terminal_velocity, float gravity_scale,
                  float *vertical_velocity);
void GameObjectOrigin(GameObject_s *object);
void ComboHitFrame(GameObject_s *object, i32 damage);
i32 Grapple_LookAtPos(GameObject_s *object, NUVEC *position);
NUVEC *Technos_TgtPos(TECHNO_s *techno);
void GameCam_UpdateLookRot(GAMECAMERA_s *camera);
void GameCam_UpdateShake(GAMECAMERA_s *camera, f32 ambient_amount);
void MakePlayPlanes(GAMECAMERA_s *camera);
u16 SeekRot(u16 current, u16 target, f32 rate);
void SeekVec(NUVEC *result, NUVEC *current, NUVEC *target, f32 rate);
void ShoveSystemCheckGameObject(GameObject_s *object);
void Techno_FindOperator(void *techno, GAMEPAD_s **pad, GameObject_s **operator_object);
bool MovingBackwards(GameObject_s *object);
void GizmoBlowupCheckProximity(WORLDINFO_s *world, GameObject_s *object);
void KeepWeaponOut(GameObject_s *object);
void DropInOutCode(GameObject_s *object);
void Signal_MoveCode(WORLDINFO_s *world, GameObject_s *object);
void TakeHitCode(GameObject_s *object);
void FloatCode(GameObject_s *object);
void SlideCode(GameObject_s *object);
void FlattenCode(GameObject_s *object);
void Hang_MoveCode(GameObject_s *object);
void Ledge_MoveCode(WORLDINFO_s *world, GameObject_s *object);
void LedgeTerrain_MoveCode(GameObject_s *object);
void Climb_MoveCode(GameObject_s *object);
void TightRope_MoveCode(GameObject_s *object, i32 jump_pressed);
void ForcedBackCode(GameObject_s *object);
void Tube_MoveCode(GameObject_s *object, WORLDINFO_s *world);
void PushCode(GameObject_s *object, i32 allow_grab);
void BackFlipCode(GameObject_s *object);
void TakeOverCode(GameObject_s *object, i32 tag_pressed);
void Glide_MoveCode(GameObject_s *object);
void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 special_animation);
void GizPanel_MoveCode(WORLDINFO_s *world, GameObject_s *object, i32 special_pressed);
void HatMachine_MoveCode(WORLDINFO_s *world, GameObject_s *object, i32 special_pressed);
void ZipUp_MoveCode(GameObject_s *object, i32 special_pressed);
void BuildIt_MoveCode(GameObject_s *object);
void Lever_MoveCode(WORLDINFO_s *world, GameObject_s *object);
void ThermalDetonator_MoveCode(GameObject_s *object);
void Detonator_MoveCode(GameObject_s *object);
extern "C" void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
void Teleport_MoveCode(GameObject_s *object, i32 special_pressed);
void ComboRotateCode(GameObject_s *object, i32 action_held);
void WeaponOutCode(GameObject_s *object);
void WeaponInCode(GameObject_s *object);
void WeaponScalingCode(GameObject_s *object);
void HoldCode(GameObject_s *object);
void SetWeaponOut(GameObject_s *object);
void SlowWeaponIn(GameObject_s *object);
void FastWeaponIn(GameObject_s *object, i32 sound);
void SlowWeaponOut(GameObject_s *object);
void FastWeaponOut(GameObject_s *object, i32 sound);
void StartHold(GameObject_s *object);
void BlockSfx(GameObject_s *object);
i32 NewBlockAction(GameObject_s *object);
void MakeJumpReachHeight(GameObject_s *, f32, i32);
void PlayJumpSfx(GameObject_s *, i32);
void NewRumble(nupad_s *, f32, i32);
void Hint_SetComplete(i32);
i32 LEGOCONTEXT_HOLD = -1;
i32 LEGOCONTEXT_JUMP = -1;
i16 LEGOACT_SLAM = -1;
i32 (*CanStartHoldFn)(GameObject_s *) = NULL;
void PlaySabreSfx(char *, GameObject_s *, NUVEC *, i32);
extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
extern "C" f32 *AnimListFrameArray(CHARACTERMODEL_s *, i32);
void HeadMovement(GameObject_s *object);
void CloakMovement(GameObject_s *object);
void HairMovement(GameObject_s *object);
i32 AnakinGreenSabre(GameObject_s *object);
i32 SuperWeirdo(GameObject_s *object);
extern i16 id_IMPERIALGUARD;
extern i16 id_GAMORREANGUARD;
BOLT_s *FindIncomingBolt(GameObject_s *, i32, i32);
PART_s *FindIncomingPart(void *, NUVEC *, f32, u32, f32);
i32 StartFallLand(GameObject_s *object, i32 action);
void ConstantRumble(GameObject_s *object, f32 strength, f32 duration);
extern "C" void PlaySfxAndSetVolume(char *name, NUVEC *position, f32 volume);
void GizForce_FindBestForceTarget(GIZFORCESYS_s *, GameObject_s *);
i32 GizForce_StoodOnForce(GIZFORCE_s *, GameObject_s *);
u16 GizForces_AngleToForce(NUVEC *, GIZFORCE_s *);
void SetHeadTarget(GameObject_s *, NUVEC *, i8, f32, f32, f32);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);
void GameCam_HitRoll(void);
void EndForce(GameObject_s *, i32);
void ResetForceGlow(PLAYERPACKET_s *);
f32 FORCEGLOWTIME = 0.1f;
f32 ForceThrowGravity = -2.5f;
f32 ForceThrowSpeed = 5.0f;
f32 MaulF_ForceThrowSpeed = 1.0f;
f32 MaulF_ForceThrowGravity = -0.5f;
f32 DookuC_ForceThrowSpeed = 2.5f;
f32 DookuC_ForceThrowGravity = -2.5f;
PART_s *Part_FindFromHSpecial(nuhspecial_s *);
PART_s *GizForce_Throw(GameObject_s *, GIZFORCE_s *, f32, f32, i32);
f32 DEACTIVATEDTIME = 8.0f;
i32 ForcePush_SuperMindTrick;
i32 ForcePush_SuperPush;
i32 ForcePush_Waft;
bool ZapTarget(GameObject_s *);
i32 CannotKill(GameObject_s *);
void SetProtocolDroidDeactivatedAction(GameObject_s *);
extern i16 id_JAWA;
extern i16 id_GONKDROID;
void NewBuzz(nupad_s *, f32, i32);
void Arcade_AIKilled(i32);
i32 ObjHitObj(GameObject_s *, GameObject_s *, i32, u16, i32, i32);
i16 *objhitobj_killparts_yrot;
f32 DIEAIRSPEED = 1.5f;
f32 DIEAIRJUMPSPEED = 2.0f;
void SetObjAsHeadTarget(GameObject_s *, GameObject_s *, i8, f32, f32, f32);
void KillRumble(GameObject_s *);
void PlayHurtSfx(GameObject_s *);
void SetFlicker(GameObject_s *, f32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);
extern i16 id_BATTLEDROIDSECURITY, id_PKDROID, id_PITDROID;
void FindForcePushTarget(GameObject_s *, i32, i32);
void ReleaseForce(GameObject_s *, i32);
void BobaRocket_Move(PART_s *, f32);
void Boulder_Move(PART_s *, f32);
void Boulder_Kill(PART_s *, i32);
extern "C" void NewPartRotation(PART_s *);
void NewBuzzFrames(nupad_s *, i32, i32);
void PlayGruntSfx(GameObject_s *);
extern i32 dagobah_training;
extern i16 id_LUKESKYWALKERDAGOBAH;
extern i32 newgamecam;
extern f32 newgamecamtime;
extern FadeSystem FadeSys;
i32 GetMenuID(void);

// Multiplies the ordinary camera's per-frame position blend.  The original
// keeps this as writable camera state (default 1.0), rather than folding it
// into the socket seek rate.
f32 CamStopBlend = 1.0f;

extern "C" {
    extern i16 id_BODYGUARD;
    extern i16 id_GRIEVOUS;
}

enum JEDI_ACTION : i16 {
    JEDI_ACTION_COMBO_1_1 = 46,
    JEDI_ACTION_COMBO_2_1 = 53,
};

static GAMECHARACTERDATA *GetGameCharacterData(GameObject_s *object) {
    if (object == NULL || object->apiobj.character_data == NULL) {
        return NULL;
    }
    return static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
}

static i32 RotationDistance(u16 current, u16 target) {
    const i32 difference = RotDiff(current, target);
    return difference < 0 ? -difference : difference;
}

static void ShiftPadDirectionHistory(GameObject_s *object, GAMEPAD_s *pad) {
    i32 player_index = -1;
    if (Player[0] == object) {
        player_index = 0;
    } else if (Player[1] == object) {
        player_index = 1;
    }
    if (player_index == -1) {
        return;
    }

    PadOldSpeed2[player_index] = PadOldSpeed[player_index];
    PadOldAngle2[player_index] = PadOldAngle[player_index];
    PadOldSpeed[player_index] = pad->input_magnitude;
    PadOldAngle[player_index] = pad->input_angle;
}

void MoveBlocks(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *) {
}

void MovePlayer(GameObject_s *object) {
    GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
    if (object == NULL || object->pad_gamepad == NULL || game_character == NULL) {
        return;
    }

    GAMEPAD_s *pad = object->pad_gamepad;
    APIOBJECT &api = object->apiobj;

    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_INPUT_ANGLE_VALID;
    if (pad->input_magnitude != pad->previous_input_magnitude) {
        pad->allocated_5a |= GAMEPAD_RUNTIME_MAGNITUDE_CHANGED;
    }

    api.field_0x214 = api.field_0x218;
    api.start_position = api.position;
    api.initial_position = {api.pos_x, api.pos_y, api.pos_z};
    api.field_0x27e = api.field_0x27d;
    pad->previous_input_angle = pad->input_angle;
    pad->previous_input_magnitude = pad->input_magnitude;
    object->field_0xefd &= ~GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS;
    object->previous_movement_angle = api.field_0x276;
    object->field_0xf1c = api.model_draw_result == 0 ? object->field_0xf1c + FRAMETIME : 0.0f;

    if ((object->field_0xf03 & 4) != 0) {
        APIObjectVelocities(object);
        GameObjectOrigin(object);
        return;
    }
    if (api.field_0x287 != 0) {
        return;
    }

    pad->allocated_5a &= ~GAMEPAD_RUNTIME_WAGGLED;
    pad->waggle_magnitude = 0.0f;
    object->field_0x107a = -1;
    object->field_0xefd &= ~GAMEOBJECT_MOVEMENT_FLAG_FACE_REVERSED;

    f32 input_x = 0.0f;
    f32 input_z = 0.0f;
    const bool accepts_player_input = (api.field_0x1f8 & APIOBJECT_FLAG_AI_PLAYER_MASK) == APIOBJECT_FLAG_PLAYER_ACTIVE;
    const i32 menu_id = GetMenuID();
    const bool input_blocked =
        FadeSys.fade > 0.0f || newgamecam != 0 ||
        (static_cast<i8>(api.flags_low) < 0 && (MiniCutCam == 2 || (menu_id != 0x0e && menu_id != -1)));
    if (accepts_player_input && !input_blocked) {
        const u32 dpad = pad->buttons_held & (GAMEPAD_DLEFT | GAMEPAD_DRIGHT | GAMEPAD_DUP | GAMEPAD_DDOWN);
        if (dpad != 0) {
            input_x = (dpad & GAMEPAD_DLEFT) != 0 ? -1.0f : (dpad & GAMEPAD_DRIGHT) != 0 ? 1.0f : 0.0f;
            input_z = (dpad & GAMEPAD_DDOWN) != 0 ? -1.0f : (dpad & GAMEPAD_DUP) != 0 ? 1.0f : 0.0f;
            pad->input_state = 1;
        } else if (pad->pad != NULL) {
            static const f32 kAnalogCentre = 127.5f;
            static const f32 kAnalogDeadzoneSquared = 1806.25f;
            const f32 raw_x = static_cast<f32>(pad->pad->analog_left_x) - kAnalogCentre;
            const f32 raw_z = kAnalogCentre - static_cast<f32>(pad->pad->analog_left_y);
            if (raw_x * raw_x + raw_z * raw_z >= kAnalogDeadzoneSquared) {
                input_x = raw_x / kAnalogCentre;
                input_z = raw_z / kAnalogCentre;
                pad->input_state = 2;
            }
        }
    } else if (!accepts_player_input) {
        const f32 delta_x = object->ai.movement_position.x - api.position.x;
        const f32 delta_z = object->ai.movement_position.z - api.position.z;
        const f32 distance = NuFsqrt(delta_x * delta_x + delta_z * delta_z);
        if (distance > 0.0f) {
            input_x = delta_x / distance;
            input_z = delta_z / distance;
        }
        pad->input_state = 1;
    } else {
        pad->input_state = 1;
    }

    ShiftPadDirectionHistory(object, pad);
    const f32 stick_magnitude = NuFsqrt(input_x * input_x + input_z * input_z);
    pad->input_direction_z = input_z;
    pad->input_direction_x = input_x;
    pad->input_magnitude = 0.0f;
    if (stick_magnitude >= 0.2f) {
        if (stick_magnitude < 0.5f && (game_character->flags_090 & 0x08) == 0) {
            pad->input_magnitude = game_character->tiptoe_speed;
        } else if (stick_magnitude < 0.8f) {
            pad->input_magnitude = game_character->walk_speed;
        } else {
            pad->input_magnitude = game_character->run_speed;
        }
        if (pad->input_magnitude > 0.0f) {
            const NUANG world_input_angle = NuAtan2D(input_x, input_z);
            // AI destinations are already expressed in world space.  The
            // shared directional mover consumes camera-relative controller
            // angles and adds GameCam's yaw again, so match the target's
            // MovePlayer AI branch by removing that yaw here.
            pad->input_angle = !accepts_player_input && GameCam != NULL
                                   ? NuAngSub(world_input_angle, GameCam->input_yaw)
                                   : world_input_angle;
        }
    }

    enum AI_GOAL_SPEED_MODE : u8 {
        AI_GOAL_SPEED_DEFAULT = 0,
        AI_GOAL_SPEED_RUN = 1,
        AI_GOAL_SPEED_WALK = 2,
    };
    if (!accepts_player_input) {
        if (object->ai.goal_speed_mode == AI_GOAL_SPEED_RUN && pad->input_magnitude > game_character->walk_speed) {
            pad->input_magnitude = game_character->walk_speed;
        } else if (object->ai.goal_speed_mode == AI_GOAL_SPEED_WALK &&
                   pad->input_magnitude > game_character->tiptoe_speed) {
            pad->input_magnitude = game_character->tiptoe_speed;
        }
    }

    const i32 waggle = GamePad_Waggle(pad);
    pad->allocated_5a =
        static_cast<u8>((pad->allocated_5a & ~GAMEPAD_RUNTIME_WAGGLED) | (waggle != 0 ? GAMEPAD_RUNTIME_WAGGLED : 0));
    pad->waggle_magnitude = GamePad_Rotate(object);
    pad->animation_input_magnitude = pad->input_magnitude;

    object->field_0x1086 = 2;
    ShoveSystemCheckGameObject(object);
    if (MovePlayer_TWIST(object) == 0 && MovePlayer_CIRCLE(object) == 0 && MovePlayer_GUNSHIPIN(object) == 0 &&
        MovePlayer_POD(object) == 0) {
        if ((api.character_data->model_flags & 0x2000) == 0) {
            MovePlayer_DIRECTIONAL(object);
        } else {
            MovePlayer_VEHICLEDIRECTIONAL(object);
        }
    }

    api.field_0x1fa |= 1;
    api.previous_velocity = api.velocity;
    APIObjectVelocities(object);
    GameObjectOrigin(object);
}

void Move_BEAST(GameObject_s *) {
}

void Move_BARMAN(GameObject_s *) {
}

void Move_CANNON(GameObject_s *) {
}

void Move_WALKER(GameObject_s *) {
}

void Move_CRITTER(GameObject_s *) {
}

void Move_DEFAULT(GameObject_s *) {
}

void Move_DRAGBOMB(GameObject_s *) {
}

void Move_DROIDEKA(GameObject_s *) {
}

static void PlayerCamPos(GameObject_s *object, NUVEC *camera_position, NUVEC *) {
    CHARACTERDATA *character = object->apiobj.character_data;
    f32 centre_height = (character->field15_0x34 + character->field16_0x38) * character->field17_0x3c * 0.5f;

    // This is the common character path in the original. Special vehicles,
    // grapples and targetable objects select a different focus point below
    // this branch; ordinary hub characters use their world position plus the
    // vertical centre of the configured character bounds.
    *camera_position = object->apiobj.position;
    camera_position->y += centre_height;
}

static void GameCam_UpdateJudder(GAMECAMERA_s *camera) {
    if (camera->judder_time <= 0.0f) {
        return;
    }

    camera->judder_time -= FRAMETIME;
    if (camera->judder_time <= 0.0f) {
        return;
    }

    constexpr f32 kJudderPeriod = 1.0f / 3.0f;
    constexpr f32 kJudderAngleScale = 546.0f;
    const i32 cycles = static_cast<i32>(camera->judder_duration / kJudderPeriod) + 1;
    const f32 elapsed_ratio = (camera->judder_duration - camera->judder_time) / camera->judder_duration;
    const NUANG phase = static_cast<NUANG>(static_cast<i32>(static_cast<f32>(cycles * 0x10000) * elapsed_ratio));
    f32 angle = kJudderAngleScale * camera->judder_time * NU_SIN_LUT(phase);
    if (camera->judder_reverse != 0) {
        angle = -angle;
    }

    const NUANG rotation = static_cast<NUANG>(static_cast<i32>(angle));
    if (camera->judder_axis == 0) {
        NuMtxPreRotateX(&camera->render_mtx, rotation);
    } else if (camera->judder_axis == 1) {
        NuMtxPreRotateY(&camera->render_mtx, rotation);
    } else {
        NuMtxPreRotateZ(&camera->render_mtx, rotation);
    }
}

static void SetGameCameraView(GAMECAMERA_s *camera, const NUVEC &position, const NUVEC &target, bool snap_angles,
                              f32 camera_shake) {
    NUVEC delta;
    NuVecSub(&delta, const_cast<NUVEC *>(&target), const_cast<NUVEC *>(&position));

    const u16 desired_pitch = static_cast<u16>(-NuAtan2D(delta.y, NuFsqrt(delta.x * delta.x + delta.z * delta.z)));
    const u16 desired_yaw = static_cast<u16>(NuAtan2D(delta.x, delta.z));
    camera->desired_pitch = desired_pitch;
    camera->desired_yaw = desired_yaw;
    camera->desired_roll = 0;

    if (snap_angles) {
        camera->pitch = desired_pitch;
        camera->yaw = desired_yaw;
        camera->roll = 0;
    } else {
        camera->pitch = SeekRot(static_cast<u16>(camera->pitch), desired_pitch, camera->angle_seek);
        camera->yaw = SeekRot(static_cast<u16>(camera->yaw), desired_yaw, camera->angle_seek);
        camera->roll = SeekRot(static_cast<u16>(camera->roll), 0, camera->angle_seek);
    }

    camera->pos = position;
    camera->target = target;
    GameCam_UpdateLookRot(camera);

    const NUANG render_pitch = camera->pitch + static_cast<u16>(static_cast<i32>(camera->field_0x214));
    const NUANG render_yaw = camera->yaw + static_cast<u16>(static_cast<i32>(camera->field_0x218));

    NuMtxSetRotationZ(&camera->mtx, camera->roll);
    NuMtxRotateX(&camera->mtx, render_pitch);
    NuMtxRotateY(&camera->mtx, render_yaw);
    NuMtxTranslate(&camera->mtx, const_cast<NUVEC *>(&position));
    camera->render_mtx = camera->mtx;
    GameCam_UpdateJudder(camera);
    GameCam_UpdateShake(camera, camera_shake);

    NuMtxSetRotationZ(&camera->target_mtx, camera->roll);
    NuMtxRotateX(&camera->target_mtx, camera->pitch);
    NuMtxRotateY(&camera->target_mtx, camera->yaw);
    NuMtxTranslate(&camera->target_mtx, const_cast<NUVEC *>(&position));

    camera->shaken_right = *NUMTX_GET_ROW_VEC(&camera->render_mtx, 0);
    camera->shaken_up = *NUMTX_GET_ROW_VEC(&camera->render_mtx, 1);
    camera->dir = *NUMTX_GET_ROW_VEC(&camera->render_mtx, 2);
    if (pNuCam != NULL) {
        pNuCam->mtx = camera->render_mtx;
        NuCameraSet(pNuCam);
    }
    MakePlayPlanes(camera);
}

void MoveGameCamera(GAMECAMERA_s *camera) {
    // Original title-camera mode (3): portal_places[2] contains one camera
    // position followed by its look target.  The complete function selects
    // many gameplay camera modes; only this currently reachable mode is
    // transcribed here.
    if (camera == NULL || WORLD == NULL || WORLD->current_level == NULL) {
        return;
    }

    if (WORLD->current_level == TITLES_LDATA) {
        if (WORLD->portal_places == NULL || WORLD->portal_places[2] == NULL ||
            WORLD->portal_places[2]->positions == NULL) {
            return;
        }
        f32 *points = WORLD->portal_places[2]->positions;
        NUVEC position = {points[0], points[1], points[2]};
        NUVEC target = {points[3], points[4], points[5]};
        camera->mode = 3;
        camera->desired_position = position;
        SetGameCameraView(camera, position, target, true, 0.0f);
        camera->previous_mode = camera->mode;
        return;
    }

    // New-game input remains locked while the initial portal-camera pair is
    // available, but never for more than ten seconds.  This is the target's
    // ordinary-camera prologue: without it `newgamecam` remains set forever
    // and MovePlayer correctly rejects every stick/D-pad sample.
    if (newgamecam != 0) {
        constexpr f32 kNewGameCameraTimeout = 10.0f;
        newgamecamtime += FRAMETIME;
        const bool initial_camera_missing =
            WORLD->portal_places == NULL || WORLD->portal_places[6] == NULL || WORLD->portal_places[7] == NULL;
        if (newgamecamtime >= kNewGameCameraTimeout || initial_camera_missing) {
            newgamecam = 0;
        }
    }

    // The original selects its ordinary free-camera mode when no socket
    // system is present and its rail-camera mode otherwise.  Both modes use
    // the same player-focus path below.
    camera->mode = WORLD->sock_sys == NULL ? 0 : 1;
    const bool mode_changed = camera->mode != camera->previous_mode;
    NUVEC player_camera_positions[2];
    NUVEC player_positions[2];
    i32 player_count = 0;
    for (i32 i = 0; i < 2; ++i) {
        if (Player[i] == NULL) {
            continue;
        }
        PlayerCamPos(Player[i], &player_camera_positions[player_count], &camera->pos);
        player_positions[player_count] = Player[i]->apiobj.position;
        ++player_count;
    }
    if (player_count == 0) {
        return;
    }

    NUVEC position = camera->pos;
    NUVEC target = {0.0f, 0.0f, 0.0f};
    for (i32 i = 0; i < player_count; ++i) {
        NuVecAdd(&target, &target, &player_camera_positions[i]);
    }
    NuVecScale(&target, &target, 1.0f / static_cast<f32>(player_count));

    f32 overlap_blend = 0.0f;
    f32 position_seek = camera->position_seek;
    f32 angle_seek = camera->angle_seek;
    f32 camera_shake = 0.0f;
    f32 separation_scale = 0.0f;
    SockSysCamera(WORLD->sock_sys, &camera->pos, camera->mode != camera->previous_mode, player_camera_positions,
                  player_positions, player_count, &camera->sock_position, &position, &target, &overlap_blend,
                  &position_seek, &angle_seek, &camera_shake, &separation_scale);
    overlap_blend *= 1.5f;
    camera->position_seek = position_seek;
    camera->angle_seek = angle_seek;
    camera->desired_position = position;

    if (!mode_changed) {
        // Target common path: clamp the socket seek contribution first, then
        // apply the independently writable stop blend to all three axes.
        const f32 seek_blend = MIN(camera->position_seek * FRAMETIME, 1.0f) * CamStopBlend;
        position.x = camera->pos.x + (position.x - camera->pos.x) * seek_blend;
        position.y = camera->pos.y + (position.y - camera->pos.y) * seek_blend;
        position.z = camera->pos.z + (position.z - camera->pos.z) * seek_blend;
    }

    // On the no-rail path SockSysCamera deliberately retains the previous
    // camera position while still producing the current player focus.  Its
    // return value is not a validity gate; both paths continue into the
    // common matrix update.
    SetGameCameraView(camera, position, target, mode_changed, camera_shake);
    camera->previous_mode = camera->mode;
}

i32 MovePlayer_POD(GameObject_s *) {
    return 0;
}

void Move_CHARACTER(GameObject_s *object) {
    if (object == NULL) {
        return;
    }
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
}

void Move_GEONOSIAN(GameObject_s *) {
}

void Move_HOVERDROID(GameObject_s *) {
}

void MovePlayerSpline(GameObject_s *) {
}

i32 MovePlayer_TWIST(GameObject_s *) {
    return 0;
}

void MoveSubItemsLeft(i32 *, nuvec_s *, i32) {
}

void Move_SPEEDERBIKE(GameObject_s *) {
}

i32 MovePlayer_CIRCLE(GameObject_s *) {
    return 0;
}

void MoveSubItemsRight(i32 *, nuvec_s *, i32) {
}

void Move_DROIDGENERIC(GameObject_s *) {
}

void MovePlayer_NETWORK(GameObject_s *) {
}

void MovePlayer_ROLLING(GameObject_s *) {
}

void MoveSplinePosition(SPLINEPOS_s *, float) {
}

void MoveBlocksOverBlock(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *) {
}

void MoveInactiveVehicle(GameObject_s *, i32, GameObject_s **) {
}

i32 MovePlayer_GUNSHIPIN(GameObject_s *) {
    return 0;
}

void Move_REPUBLICGUNSHIP(GameObject_s *) {
}

void Move_SUPERBATTLEDROID(GameObject_s *) {
}

void MovePlayer_DIRECTIONAL(GameObject_s *object) {
    GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
    if (object == NULL || object->pad_gamepad == NULL || game_character == NULL) {
        return;
    }

    GAMEPAD_s *pad = object->pad_gamepad;
    GameObject_s *operator_object = object;
    Techno_FindOperator(object, &pad, &operator_object);

    GAMECHARACTERDATA *operator_character = GetGameCharacterData(operator_object);
    if (operator_character == NULL || operator_character->run_speed == 0.0f) {
        operator_character = game_character;
        operator_object = object;
    }
    const f32 desired_speed = game_character->run_speed * (pad->input_magnitude / operator_character->run_speed);

    object->field_0xe23 &= ~0x10;
    if (MovingBackwards(object)) {
        object->field_0xefd |= GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS;
    }

    APIOBJECT &api = object->apiobj;
    const u16 input_angle = GameCam != NULL ? GamePad_InputAngle(object, pad) : pad->input_angle;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_INPUT_ANGLE_VALID;
    object->current_input_angle = input_angle;

    if (object->delayed_turn_timer > 0.0f) {
        if (pad->input_magnitude > 0.0f) {
            object->delayed_turn_target_angle = input_angle;
        }
        object->delayed_turn_timer -= FRAMETIME;
        if (object->delayed_turn_timer <= 0.0f) {
            api.movement_facing_angle = object->delayed_turn_target_angle;
        }
    } else {
        const i8 context = object->build_context;
        const bool ordinary_context = context == CHARACTER_CONTEXT_NONE || context == 1;
        const bool has_turn_animation =
            api.character_model != NULL && api.character_model->model_data_b != NULL &&
            (api.character_model->model_data_b[12] != NULL || api.character_model->model_data_b[119] != NULL);
        if (ordinary_context && has_turn_animation && pad->input_magnitude > 0.0f &&
            RotationDistance(api.movement_facing_angle, input_angle) >= 0x6aab) {
            object->delayed_turn_timer = 0.2f;
            object->delayed_turn_target_angle = input_angle;
        } else if (pad->input_magnitude > 0.0f && ordinary_context) {
            api.movement_facing_angle = input_angle;
        }
    }

    const i32 turn_speed = static_cast<i32>(game_character->turn_rate * 8.0f * 16384.0f);
    api.facing_angle = TurnRot(api.facing_angle, api.movement_facing_angle, turn_speed, NULL);
    api.field_0x276 = SeekRot(api.field_0x276, api.facing_angle, 10.0f);
    api.movement_facing_angle = api.facing_angle;

    f32 final_speed = (pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) != 0 ? 0.0f : desired_speed;
    if ((object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS) != 0) {
        final_speed *= game_character->backwards_speed_multiplier;
    }

    NUVEC forward = {0.0f, 0.0f, final_speed};
    NuVecRotateY(&object->target_velocity, &forward, input_angle);
    object->target_velocity.y = 0.0f;
    if ((object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_REVERSE_VELOCITY) != 0) {
        NuVecRotateY(&object->target_velocity, &object->target_velocity, 0x8000);
    }

    api.velocity.x = SeekValF(api.velocity.x, object->target_velocity.x, game_character->velocity_seek_rate);
    api.velocity.z = SeekValF(api.velocity.z, object->target_velocity.z, game_character->velocity_seek_rate);
    object->movement_lean_angle = 0;
    GizmoBlowupCheckProximity(WORLD, object);
}

void MovePlayer_VEHICLEDIRECTIONAL(GameObject_s *) {
}

void Move_POD(GameObject_s *) {
}

void Move_ATAT(GameObject_s *) {
}

void Move_JAWA(GameObject_s *) {
}

static bool JediHasAction(const GameObject_s *object, JEDI_ACTION action) {
    return object->apiobj.character_model != NULL && object->apiobj.character_model->model_data_b != NULL &&
           object->apiobj.character_model->model_data_b[action] != NULL;
}

static u8 SetComboOpponent(GameObject_s *object, f32 range, i32 update_heading, i32 mode) {
    object->force_target = NULL;
    object->blowup_target = NULL;
    bool searched = false;
    u8 result = 0;
    if ((object->apiobj.flags_low & 0x80) != 0 && mode == 0 && object->pad_gamepad->input_magnitude > 0.0f) {
        object->force_target = ObjOpponent(object, range, 0.0f, 0, 0, 1);
        searched = true;
        if (object->force_target != NULL) {
            result = 1;
            goto finished;
        }
    }
    if (object->ai.primary_target_ref != NULL && object->ai.nearest_opponent_metric < range) {
        ComboOpponent_Range2 = object->ai.nearest_opponent_metric;
        object->force_target = *object->ai.primary_target_ref;
        result = 1;
        goto finished;
    }
    if ((object->apiobj.flags_low & 0x80) == 0) {
        if ((object->field_0xef8 & 0x40) != 0 || object->use_action == 5) {
            object->blowup_target = GizmoBlowUpOpponent(object, range, 0.0f, 0.0f, mode, 0, 0, 0);
            if (object->blowup_target != NULL) {
                ComboOpponent_Range2 = GizmoBlowUpOpponent_Range2;
                result = 4;
            }
        }
        goto finished;
    }
    if (!searched) {
        object->force_target = ObjOpponent(object, range, 0.0f, 0, 0, 1);
        if (object->force_target != NULL) {
            result = 1;
            goto finished;
        }
    }
    object->blowup_target = GizmoBlowUpOpponent(object, range, 0.0f, 0.0f, mode, 0, 0, 0);
    if (object->blowup_target != NULL) {
        ComboOpponent_Range2 = GizmoBlowUpOpponent_Range2;
        result = 4;
        goto finished;
    }
    {
        NUVEC forward;
        if (object->pad_gamepad->input_magnitude > 0.0f) {
            NuVecRotateY(&forward, &v001, GamePad_InputAngle(object, object->pad_gamepad));
        }
        f32 nearest_distance = 1.0e8f;
        GameObject_s *nearest = NULL;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
            GameObject_s *target = &Obj[i];
            if (target == object || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
                target->apiobj.field_0x287 != 0 || (target->apiobj.field_0x1f4 & 1) != 0 ||
                (target->character_context & 0xfd) == 0x39 || target->character_context == 0x3c ||
                (GetGameCharacterData(target)->flags_090 & 0x8000) != 0 ||
                (CInfo[target->character_context].flags & 0x8000) != 0 || target == object->field_0xcc0) continue;
            NUVEC delta;
            f32 distance = NuVecDistSqr(&target->apiobj.position, &object->apiobj.position, &delta);
            if (distance >= range * range || (object->pad_gamepad->input_magnitude > 0.0f &&
                forward.x * delta.x + forward.z * delta.z < 0.0f)) continue;
            u16 angle = NuAtan2D(target->apiobj.position.x - object->apiobj.position.x,
                                target->apiobj.position.z - object->apiobj.position.z);
            i32 difference = RotDiff(object->apiobj.movement_facing_angle, angle);
            if (difference >= -0x2000 && difference <= 0x2000 && distance < nearest_distance) {
                nearest = target;
                nearest_distance = distance;
            }
        }
        PlayerOpponent_Range2 = nearest_distance;
        object->force_target = nearest;
        if (nearest != NULL) {
            ComboOpponent_Range2 = nearest_distance;
            result = 2;
            goto finished;
        }
    }
    if (!searched) {
        object->force_target = ObjOpponent(object, range, 0.0f, 0, 0, 1);
        result = object->force_target != NULL;
    }
finished:
    if (update_heading != 0 && object->force_target == NULL && object->pad_gamepad->input_magnitude > 0.0f &&
        (object->field_0xe22 & 0x20) != 0) {
        object->apiobj.movement_facing_angle = object->current_input_angle;
    }
    return result;
}

static void BlockCode(GameObject_s *object, i32 pressed, i32 held, i32 jump_pressed, i32 allow_start) {
    if ((object->field_0xe22 & 1) != 0 && object->weapon_scale_state == 0) {
        void **animations = object->apiobj.character_model->model_data_b;
        bool has_block = animations[0x1a] != NULL || animations[0x1b] != NULL || animations[0x1c] != NULL;
        object->incoming_bolt = has_block ? FindIncomingBolt(object, 0, 0) : NULL;
        animations = object->apiobj.character_model->model_data_b;
        has_block = animations[0x1a] != NULL || animations[0x1b] != NULL || animations[0x1c] != NULL;
        GameObject_s *melee = NULL;
        if (has_block) {
            for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
                GameObject_s *target = &Obj[i];
                if (target != object && (target->apiobj.field_0x1f8 & 0x1000) != 0 &&
                    target->apiobj.field_0x287 == 0 && target->character_context == 5 &&
                    (target->context_flags & 0xc) == 0 && target->force_target == object) {
                    melee = target;
                    break;
                }
            }
        }
        object->incoming_melee = melee;
        GameObject_s *special = NULL;
        if (((object->apiobj.character_data->model_flags & 8) != 0 || object->id == id_BODYGUARD ||
             object->id == id_IMPERIALGUARD) && has_block) {
            for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
                GameObject_s *target = &Obj[i];
                if (target != object && (target->apiobj.field_0x1f8 & 0x1000) != 0 &&
                    target->apiobj.field_0x287 == 0 && target->character_context == 0x26 &&
                    target->force_target == object && (target->id != id_GAMORREANGUARD ||
                    target->context_animation != 0x56 || (target->context_flags & 0x40) == 0)) {
                    special = target;
                    break;
                }
            }
        }
        object->incoming_special = special;
        if (object->incoming_bolt == NULL && melee == NULL) {
            if (object->apiobj.field_0x27c == -1 || !has_block) object->incoming_part = NULL;
            else {
                f32 radius = object->apiobj.field_0x1e0 > object->apiobj.field_0x1dc ?
                             object->apiobj.field_0x1e0 : object->apiobj.field_0x1dc;
                object->incoming_part = FindIncomingPart(object, &object->apiobj.collision_position, radius, 10, 0.0f);
            }
        }
        if ((object->field_0xe22 & 8) == 0 &&
            (object->incoming_bolt != NULL || object->incoming_melee != NULL || object->incoming_part != NULL) &&
            object->character_context == -1) object->field_0xe22 |= 8;
    }
    if (object->character_context == 0xc) {
        GameObject_s *attacker = object->block_attacker;
        if (attacker != NULL) {
            if (jump_pressed != 0 && object->apiobj.character_model->model_data_b[0x12] != NULL) {
                SetComboOpponent(object, 1.0f, 0, 1);
                f32 speed = GetGameCharacterData(object)->second_jump_speed;
                object->jump_flags &= ~1;
                object->context_animation_timer = 0.0f;
                object->delayed_turn_timer = 0.0f;
                object->airborne_input_timer = 0.0f;
                object->field_0xe22 |= 0x10;
                object->character_context = 0;
                object->apiobj.velocity.y = speed;
                object->context_variant_flags &= 0x4f;
                object->action_movement_state = 2;
                object->context_animation = 0x12;
                object->apiobj.field_0x27d = 0;
                object->field_0x105c = 0;
                object->collision_target = NULL;
                PlayJumpSfx(object, 2);
                ResetAnimPacket(&object->apiobj.anim_packet, object->context_animation);
                if ((object->apiobj.flags_low & 0x80) != 0) object->field_0xef9 |= 8;
                return;
            }
            if (object->block_latch == 0 && object->blocked_attack_stage != 0) {
                if ((attacker->apiobj.field_0x1f8 & 0x1000) != 0 && attacker->apiobj.field_0x287 == 0) {
                    if ((object->field_0xe23 & 0x20) == 0) {
                        if (attacker->character_context == 5 && attacker->combo_stage < object->blocked_attack_stage) return;
                    } else if (attacker->character_context == 0x26 &&
                               attacker->context_animation == object->blocked_attack_stage &&
                               (attacker->id != id_GAMORREANGUARD || attacker->context_animation != 0x56 ||
                                (attacker->context_flags & 0x40) == 0)) return;
                }
                object->blocked_attack_stage = 0;
                return;
            }
        }
        if (object->block_latch == 0) {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->block_latch = 1;
                object->context_animation_timer = 0.2f;
            }
            return;
        }
        if (object->id == id_GAMORREANGUARD && (object->apiobj.flags_low & 0x80) == 0) {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->character_context = -1;
                object->block_cooldown = 1.5f;
            }
            return;
        }
        if (pressed == 0 || (object->incoming_bolt == NULL && object->incoming_melee == NULL &&
            object->incoming_special == NULL && object->incoming_part == NULL)) {
            if (held != 0 && object->incoming_bolt != NULL) {
                object->context_animation_timer = 0.2f;
                return;
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer > 0.0f) return;
            if (held != 0 && (attacker == NULL || attacker->id != id_GAMORREANGUARD ||
                attacker->character_context != 0x26 || attacker->context_animation != 0x56 ||
                (attacker->context_flags & 0x40) == 0)) {
                StartHold(object);
                return;
            }
            object->character_context = -1;
            return;
        }
        if (!TouchHacks::ShouldBlock(*object)) return;
    } else {
        if (object->block_cooldown > 0.0f) {
            object->block_cooldown -= FRAMETIME;
            return;
        }
        if (!allow_start || !pressed || object->apiobj.field_0x27d == 0 ||
            (CInfo[object->character_context].flags & 0x20) != 0 || (object->field_0xef8 & 2) == 0 ||
            (object->incoming_bolt == NULL && object->incoming_melee == NULL &&
             object->incoming_special == NULL && object->incoming_part == NULL)) return;
    }
    if (!NewBlockAction(object)) {
        object->character_context = -1;
        return;
    }
    object->character_context = 0xc;
    object->block_latch = 0;
    if (object->incoming_bolt != NULL) {
        object->blocked_bolt = object->incoming_bolt;
        object->block_attacker = NULL;
        object->blocked_part = NULL;
        object->context_animation_timer = 0.3f;
        object->blocked_attack_stage = 0;
        if ((object->apiobj.flags_low & 0x80) != 0) Hint_SetComplete(0x5dd);
    } else if (object->incoming_part != NULL) {
        object->blocked_bolt = NULL;
        object->block_attacker = NULL;
        object->blocked_part = object->incoming_part;
        object->context_animation_timer = 0.3f;
        object->blocked_attack_stage = 0;
    } else {
        object->blocked_bolt = NULL;
        object->blocked_part = NULL;
        object->context_animation_timer = 0.1f;
        if (object->incoming_special != NULL) {
            object->block_attacker = object->incoming_special;
            object->blocked_attack_stage = object->incoming_special->context_animation;
            object->field_0xe23 |= 0x20;
        } else {
            object->block_attacker = object->incoming_melee;
            object->blocked_attack_stage = object->incoming_melee->combo_stage + 1;
        }
    }
}

static void SwipeCode(GameObject_s *object, i32 pressed, i32 held) {
    i8 context = object->character_context;
    if (context == 0x10) {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame != NULL) {
            if ((object->context_flags & 0x40) == 0) {
                f32 hit_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (hit_frame >= 1.0f && hit_frame <= *frame) ComboHitFrame(object, 1);
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->field_0xe20 |= 4;
                object->character_context = -1;
                if (object->context_animation == 0x29) {
                    if ((object->context_flags & 0x40) == 0) {
                        ComboHitFrame(object, 1);
                        object->character_context = 0x10;
                        object->context_animation_timer = 0.001f;
                    } else if (held) StartHold(object);
                }
            }
        }
    } else if ((object->field_0xe22 & 1) != 0 && object->weapon_scale_state == 0 && pressed &&
               object->apiobj.field_0x27d != 0 && object->apiobj.character_model != NULL &&
               (context == 1 || context == -1 || context == 2 || context == 4 || context == 3) &&
               SetComboOpponent(object, 0.6f, 0, 2) == 1) {
        NUVEC *position;
        if (object->force_target != NULL) position = &object->force_target->apiobj.position;
        else if (object->blowup_target != NULL) position = &object->blowup_target->mid_position;
        else return;
        NUVEC delta;
        NuVecSub(&delta, position, &object->apiobj.position);
        NuVecRotateY(&delta, &delta, -object->apiobj.movement_facing_angle);
        if ((object->pad_gamepad->input_magnitude <= 0.0f || fabsf(delta.x) <= fabsf(delta.z)) &&
            delta.z < 0.0f && ComboOpponent_Range2 < 0.48999998f) {
            object->context_animation = 0x29;
            if (object->apiobj.character_model->model_data_b[0x29] != NULL) {
                object->context_animation_timer = AnimDuration(object->id, 0x29, 0.0f, 0.0f, 1);
                if (object->context_animation_timer > 0.0f) {
                    object->character_context = 0x10;
                    object->context_flags &= ~0x40;
                    PlaySabreSfx(NULL, object, NULL, 1);
                    NewRumble(object->pad_gamepad->pad, 0.4f, 0);
                }
            }
        }
    }
}

static void LightSabreComboCode(GameObject_s *object, i32 action_pressed, i32 action_held, i32 jump_pressed, i32 buttons_pressed) {
    ANIMPACKET_s &animation = object->apiobj.anim_packet;
    if ((object->apiobj.flags_low & 0x80) != 0) object->ai_combo_cooldown = 0.0f;
    else if (object->ai_combo_cooldown > 0.0f) object->ai_combo_cooldown -= FRAMETIME;
    u8 previous_flags = object->context_flags;
    object->context_flags &= ~2;
    if (object->character_context == CHARACTER_CONTEXT_COMBO) {
        object->field_0xef9 |= 8;
        if ((object->context_flags & 4) != 0) {
            if ((action_pressed == 0 && object->combo_input_latched == 0) || object->combo_stage > 1) {
                object->combo_input_timer -= FRAMETIME;
                if (object->combo_input_timer <= 0.0f) {
                    object->character_context = CHARACTER_CONTEXT_NONE;
                    object->context_flags |= 2;
                    if (action_held != 0) StartHold(object);
                } else if (object->combo_stage < 2 && jump_pressed != 0 &&
                           JediHasAction(object, static_cast<JEDI_ACTION>(0x12))) {
                    SetComboOpponent(object, 1.0f, 0, 1);
                    object->jump_flags &= ~1;
                    object->context_animation_timer = 0.0f;
                    object->delayed_turn_timer = 0.0f;
                    object->airborne_input_timer = 0.0f;
                    object->field_0xe22 |= 0x10;
                    object->character_context = 0;
                    object->apiobj.velocity.y = GetGameCharacterData(object)->second_jump_speed;
                    object->context_variant_flags &= 0x4f;
                    object->action_movement_state = 2;
                    object->context_animation = 0x12;
                    object->apiobj.field_0x27d = 0;
                    object->field_0x105c = 0;
                    object->collision_target = NULL;
                    PlayJumpSfx(object, 2);
                    ResetAnimPacket(&animation, object->context_animation);
                    if ((object->apiobj.flags_low & 0x80) != 0) object->field_0xef9 |= 8;
                }
            } else {
                SetComboOpponent(object, 1.0f, 1, 0);
                object->context_flags &= ~4;
                object->combo_stage = object->combo_stage == 1 ? 2 : 1;
                if (object->id != id_IMPERIALGUARD) PlaySabreSfx(NULL, object, NULL, 1);
                if (object->combo_stage == 2) NewRumble(object->pad_gamepad->pad, 0.35f, 0);
                if (object->combo_stage == 1) {
                    object->combo_branch = (object->field_0xe20 & 0x40) != 0 &&
                        JediHasAction(object, static_cast<JEDI_ACTION>(object->queued_context_animation + 2)) ? 2 : 1;
                    if (object->combo_branch == 1) object->field_0xe20 &= ~0x40;
                    NewRumble(object->pad_gamepad->pad, object->combo_branch == 2 ? 0.5f : 0.35f, 0);
                    if (object->combo_branch == 2 && object->id != id_IMPERIALGUARD) {
                        PlaySabreSfx("ComboSaber2", object, &object->apiobj.collision_position, 0);
                    }
                } else if (object->combo_branch == 1) {
                    object->combo_branch = (object->field_0xe20 & 0x40) != 0 &&
                        JediHasAction(object, static_cast<JEDI_ACTION>(object->queued_context_animation + 4)) ? 4 : 3;
                    if (object->combo_branch == 3) object->field_0xe20 &= ~0x40;
                } else {
                    object->combo_branch = (object->field_0xe20 & 0x40) != 0 &&
                        JediHasAction(object, static_cast<JEDI_ACTION>(object->queued_context_animation + 6)) ? 6 : 5;
                    if (object->combo_branch == 5) object->field_0xe20 &= ~0x40;
                    else {
                        NewRumble(object->pad_gamepad->pad, 0.6f, 0);
                        if (object->id != id_IMPERIALGUARD) {
                            PlaySabreSfx("ComboSaber1", object, &object->apiobj.collision_position, 0);
                        }
                    }
                }
                object->context_animation = object->queued_context_animation + object->combo_branch;
                object->field_0xe20 |= 0x40;
                object->combo_input_latched = 0;
                object->context_flags &= ~0x50;
            }
            goto combo_finished;
        }
        if (animation.animation_index == object->context_animation && animation.blending == 0 &&
            animation.requested_animation == object->context_animation) {
            if (object->combo_stage == 2) {
                f32 *frames = AnimListFrameArray(object->apiobj.character_model, object->context_animation);
                if ((object->context_flags & 0x40) == 0 && frames[0] > 0.0f && animation.current_time >= frames[0]) {
                    ComboHitFrame(object, object->combo_branch == 6 ? 3 : 1);
                }
                if (frames[1] > 0.0f && animation.current_time >= frames[1]) object->context_flags |= 8;
                CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[object->context_animation]);
                if ((animation.flags & ANIMPACKET_FLAG_FINISHED) != 0 ||
                    (info->blend_out_time > 0.0f && AnimDuration(object->id, object->context_animation,
                     animation.current_time, 0.0f, 0) < info->blend_out_time)) {
                    object->character_context = CHARACTER_CONTEXT_NONE;
                    object->context_flags |= 2;
                }
                object->sabre_flags |= 2;
                if ((object->context_flags & 0x40) == 0 &&
                    (((object->context_flags & 8) != 0 && frames[0] == 0.0f) || (object->context_flags & 2) != 0)) {
                    ComboHitFrame(object, object->combo_branch == 6 ? 3 : 1);
                }
                if (object->character_context == CHARACTER_CONTEXT_NONE && action_held != 0 &&
                    (object->id != id_IMPERIALGUARD || object->combo_stage != 2)) StartHold(object);
                if ((object->context_flags & 0x40) != 0 && (previous_flags & 0x40) == 0) {
                    NewRumble(object->pad_gamepad->pad, object->combo_branch == 6 ? 0.7f : 0.35f, 0);
                }
                if ((object->apiobj.flags_low & 0x80) == 0 &&
                    ((object->field_0xefb & 8) != 0 || WORLD->current_level == VADERC_LDATA)) {
                    object->ai_combo_cooldown = 3.0f;
                }
                goto combo_finished;
            }
            if ((animation.flags & ANIMPACKET_FLAG_FINISHED) != 0) {
                object->combo_input_timer = 0.2f;
                object->context_flags |= 4;
                BlockSfx(object);
                object->sabre_flags |= 2;
                if ((object->context_flags & 0x40) == 0) ComboHitFrame(object, 1);
                goto combo_finished;
            }
        }
        f32 *time = AnimPlaying(&animation, object->context_animation, 1, 0);
        if (time != NULL && (object->context_flags & 0x40) == 0) {
            const f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (frame > 0.0f && *time >= frame) {
                ComboHitFrame(object, object->combo_branch == 2 ? 2 : 1);
            }
        }
        bool can_queue = false;
        if (time != NULL) {
            const f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
            if (frame > 0.0f && *time >= frame) object->context_flags |= 0x10;
            can_queue = AnimDuration(object->id, object->context_animation, *time, 0.0f, 0) < 0.1f;
            if (can_queue) object->field_0xe22 |= 8;
        }
        if (action_pressed != 0 && object->combo_input_latched == 0) {
            if (can_queue) object->combo_input_latched = 1;
            else object->field_0xe20 &= ~0x40;
        }
        object->sabre_flags |= 3;
        goto combo_finished;
    }

    {
    const i8 context = object->character_context;
    const bool player_active = (object->apiobj.flags_low & 0x80) != 0;
    if (action_pressed == 0 || (!player_active && object->ai_combo_cooldown > 0.0f) ||
        context == 0x15 || context == 0x5a || context == 0x13 || context == 0x2e ||
        (!player_active && static_cast<u8>(context - 6) < 2) || context == 8 || context == 0x1b ||
        context == 0x2d || context == 0xf || context == 0x27 || context == 0x28 || context == 0x59 ||
        context == 0x30 || context == 0x3d || context == 0x43 || context == 0x4a || context == 0x61 || context == 0x4b) {
        if (((object->pad_gamepad->allocated_5a & 4) != 0 || buttons_pressed != 0) &&
            (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 &&
            object->weapon_scale_state == WEAPON_SCALE_IDLE && context != 6 && context != 7 &&
            context != 18 && context != 27 && context != 29 && context != 13 && context != 14 && context != 46 &&
            !(context == 0 && static_cast<u8>(object->action_movement_state - 3) < 2) &&
            (object->field_0xe23 & 1) == 0 && (CInfo[context].flags & 0x4000000) == 0 &&
            ((CInfo[context].flags & 0x8000000) == 0 || (object->jump_flags & 2) == 0) && context != 8) {
            GAMECHARACTERDATA *data = GetGameCharacterData(object);
            const i32 action = data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 126 : 16;
            if (object->apiobj.field_0x27d != 0 && object->pad_gamepad->input_magnitude == 0.0f &&
                object->apiobj.character_model->model_data_b[action] != NULL &&
                (context == -1 || (CInfo[context].flags & 4) != 0)) {
                SlowWeaponIn(object);
            } else {
                FastWeaponIn(object, 1);
            }
        }
        goto combo_finished;
    }
    if ((object->field_0xe22 & 1) == 0) {
        GAMECHARACTERDATA *data = GetGameCharacterData(object);
        const i32 draw_action = data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 127 : 17;
        if (object->weapon_scale_state == 0 && object->apiobj.field_0x27d != 0 && object->pad_gamepad->input_magnitude == 0.0f &&
            object->apiobj.character_model->model_data_b[draw_action] != NULL && context >= -1 && context <= 4) {
            SlowWeaponOut(object);
        } else {
            FastWeaponOut(object, 1);
        }
        goto combo_finished;
    }
    if ((object->field_0xe20 & 4) != 0 || (!player_active && object->weapon_scale_state != 0) ||
        object->apiobj.field_0x27d == 0 ||
        (context != 1 && context != -1 && context != 2 && context != 4 && context != 3 && context != 6 && context != 7)) goto combo_finished;
    SetWeaponOut(object);
    if (object->incoming_bolt != NULL || object->incoming_melee != NULL ||
        object->incoming_special != NULL || object->incoming_part != NULL) {
        if (!TouchHacks::ShouldBlock(*object)) goto combo_finished;
        if (!NewBlockAction(object)) {
            object->character_context = -1;
            goto combo_finished;
        }
        object->character_context = 0xc;
        object->block_latch = 0;
        if (object->incoming_bolt != NULL) {
            object->blocked_bolt = object->incoming_bolt;
            object->block_attacker = NULL;
            object->blocked_part = NULL;
            object->context_animation_timer = 0.3f;
            object->blocked_attack_stage = 0;
            if ((object->apiobj.flags_low & 0x80) != 0) Hint_SetComplete(0x5dd);
        } else if (object->incoming_part != NULL) {
            object->blocked_bolt = NULL;
            object->block_attacker = NULL;
            object->blocked_part = object->incoming_part;
            object->blocked_attack_stage = 0;
            object->context_animation_timer = 0.3f;
        } else {
            object->blocked_bolt = NULL;
            object->blocked_part = NULL;
            object->context_animation_timer = 0.1f;
            if (object->incoming_special != NULL) {
                object->block_attacker = object->incoming_special;
                object->blocked_attack_stage = object->incoming_special->context_animation;
                object->field_0xe23 |= 0x20;
            } else {
                object->block_attacker = object->incoming_melee;
                object->blocked_attack_stage = object->incoming_melee->combo_stage + 1;
            }
        }
        goto combo_finished;
    }
    if (!player_active && ((object->field_0xf04 & 2) != 0 || (object->apiobj.character_data->model_flags & 4) == 0) &&
        (object->field_0xef9 & 4) == 0) goto combo_finished;
    SetComboOpponent(object, 1.0f, 1, 0);
    if ((object->apiobj.flags_low & 0x80) != 0 && object->force_target != NULL &&
        object->force_target->apiobj.field_0x27c == -1) Hint_SetComplete(0x277);
    object->character_context = CHARACTER_CONTEXT_COMBO;
    object->combo_stage = 0;
    object->combo_input_timer = 0.0f;

    JEDI_ACTION action = JEDI_ACTION_COMBO_1_1;
    u8 alternate = 0;
    if (JediHasAction(object, JEDI_ACTION_COMBO_2_1) && object->combo_alternate == 0) {
        action = JEDI_ACTION_COMBO_2_1;
        alternate = 1;
    }
    object->queued_context_animation = action;
    object->context_animation = action;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->field_0xe20 |= GAMEOBJECT_E20_FLAG_COMBO_MOVEMENT;
    object->context_flags &= GAMEOBJECT_CONTEXT_FLAGS_COMBO_START_RETAIN_MASK;
    object->combo_alternate = alternate;
    object->combo_branch = 0;
    object->combo_input_latched = 0;
    object->weapon_scale = 1.0f;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
    object->apiobj.velocity.y = 0.0f;
    if (object->id != id_IMPERIALGUARD) PlaySabreSfx(NULL, object, NULL, 1);

    if (!JediHasAction(object, action)) {
        object->character_context = CHARACTER_CONTEXT_NONE;
    }
    }
combo_finished:
    if (object->weapon_scale == 1.0f && (object->field_0xe22 & 8) == 0 &&
        object->character_context == 5 && (object->context_flags & 4) != 0) object->field_0xe22 |= 8;
}

static void JediLightCode(GameObject_s *object) {
    if (object->dynamic_light_id == -1) return;
    NUVEC colour;
    NUVEC position;
    if (object->character_context == 0x1b && (object->field_0xe21 & 5) == 1 && object->force_target != NULL) {
        rtlDynamicEnable(object->dynamic_light_id, 1);
        NUVEC delta;
        f32 distance = NuVecDist(&object->force_target->apiobj.collision_position, &object->apiobj.collision_position, &delta);
        rtlDynamicSetRadii(object->dynamic_light_id, distance * 0.5f, distance * 0.5f + 0.5f);
        colour.x = 0.0f;
        colour.y = qrand() < 0x8000 ? 2.0f : 0.25f;
        colour.z = colour.y;
        rtlDynamicSetColours(object->dynamic_light_id, &colour, NULL);
        position.x = delta.x * 0.5f + object->apiobj.pos_x;
        position.y = delta.y * 0.5f + object->apiobj.pos_y;
        position.z = delta.z * 0.5f + object->apiobj.pos_z;
        rtlDynamicSetPos(object->dynamic_light_id, &position);
        return;
    }
    if (object->weapon_scale <= 0.0f || object->sabre_flags == 0) return;
    position = v000;
    if (object->apiobj.field_0x288 == 0) return;
    i32 point_count = 0;
    for (i32 blade = 0; blade < 4; ++blade) {
        if (object->apiobj.field_0x288 == 0) break;
        if ((object->field_0xe23 & 8) == 0) continue;
        GAMECHARACTERDATA *data = GetGameCharacterData(object);
        i32 first = data->streak_joints[blade][0];
        i32 second = data->streak_joints[blade][1];
        if (first == -1 || object->apiobj.character_model->points_of_interest[first] == NULL ||
            second == -1 || object->apiobj.character_model->points_of_interest[second] == NULL) continue;
        NuVecAdd(&position, &position, reinterpret_cast<NUVEC *>(&object->joint_matrices[first].m30));
        NuVecAdd(&position, &position, reinterpret_cast<NUVEC *>(&object->joint_matrices[second].m30));
        point_count += 2;
    }
    if (point_count == 0) return;
    NuVecScale(&position, &position, 1.0f / point_count);
    rtlDynamicEnable(object->dynamic_light_id, 1);
    f32 radius = object->sabre_collision_radius * 3.0f * object->weapon_scale * object->apiobj.field_0xa8;
    rtlDynamicSetRadii(object->dynamic_light_id, radius + 0.25f, radius + 0.5f);
    f32 brightness = qrand() < 0x8000 ? 0.003921569f : 0.007843138f;
    i32 blade = -1;
    if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(0x19)) blade = 0;
    else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object)) blade = 3;
    else if (object->id == id_GRIEVOUS) {
        f32 phase = (NU_SIN_LUT(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 2.0f) * 0.5f * 65536.0f)) + 1.0f) * 0.5f;
        colour.x = ((BladeTab[2].colour[0] - static_cast<f32>(BladeTab[1].colour[0])) * phase + BladeTab[1].colour[0]) * brightness;
        colour.y = ((BladeTab[2].colour[1] - static_cast<f32>(BladeTab[1].colour[1])) * phase + BladeTab[1].colour[1]) * brightness;
        colour.z = ((BladeTab[2].colour[2] - static_cast<f32>(BladeTab[1].colour[2])) * phase + BladeTab[1].colour[2]) * brightness;
    } else blade = static_cast<i8>(GetGameCharacterData(object)->field_0x117);
    if (blade != -1) {
        colour.x = BladeTab[blade].colour[0] * brightness;
        colour.y = BladeTab[blade].colour[1] * brightness;
        colour.z = BladeTab[blade].colour[2] * brightness;
    }
    rtlDynamicSetColours(object->dynamic_light_id, &colour, NULL);
    rtlDynamicSetColours(object->dynamic_light_id, &colour, NULL);
    rtlDynamicSetPos(object->dynamic_light_id, &position);
}

static void ForceGlowCode(GameObject_s *object, i32 model) {
    f32 previous = object->field_0xd80;
    if (previous == 0.0f && (object->force_glow_previous == NULL ||
        object->force_glow_previous == object->force_glow_candidate)) {
        object->force_glow_object = object->force_glow_candidate;
        object->force_glow_kind = object->force_glow_candidate_kind;
    }
    f32 target = FORCEGLOWTIME;
    if (object->force_glow_object == NULL || object->force_glow_candidate != object->force_glow_object) {
        if (previous == FORCEGLOWTIME && object->force_hold_time > 0.0f) object->force_hold_time -= FRAMETIME;
        else target = 0.0f;
    } else if ((object->apiobj.flags_low & 0x80) == 0 && object->force_glow_kind == 0 && object->character_context != 8 &&
               AnimPlaying(&object->apiobj.anim_packet, 0xb, 1, 1) == NULL &&
               AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL) target = 0.0f;
    if (object->force_glow_target == previous) object->force_glow_target = target;
    else {
        if (previous <= object->force_glow_target) {
            object->field_0xd80 = previous + FRAMETIME;
            if (object->field_0xd80 > object->force_glow_target) object->field_0xd80 = object->force_glow_target;
        } else {
            object->field_0xd80 = previous - FRAMETIME * 0.5f;
            if (object->field_0xd80 < 0.0f) object->field_0xd80 = 0.0f;
        }
        if (object->field_0xd80 == FORCEGLOWTIME && previous != FORCEGLOWTIME) object->force_hold_time = 0.333f;
    }
    if (object->field_0xd80 <= 0.0f) return;
    PLAYERPACKET_s *packet = reinterpret_cast<PLAYERPACKET_s *>(object->player_packet);
    if (object->force_glow_object == NULL) {
        ResetForceGlow(packet);
        return;
    }
    switch (object->force_glow_kind) {
    case 0: {
        GIZFORCE_s *force = static_cast<GIZFORCE_s *>(object->force_glow_object);
        if ((force->progress_flags & 1) == 0) {
            ResetForceGlow(packet);
            return;
        }
        object->field_0xd8c = TouchHacks::TouchControlsActive && force->strength_0x6c > 0.0f ?
                             force->strength_0x6c : force->horizontal_range * force->radius;
        object->field_0xe1e = model;
        object->force_glow_position = force->position;
        force->field_0xaa |= 2;
        break;
    }
    case 1: {
        GAMEANIMOBJ_s *animation = static_cast<GAMEANIMOBJ_s *>(object->force_glow_object);
        f32 scale = object->field_0xd8c;
        if (!NuSpecialExistsFn(&animation->special)) {
            ResetForceGlow(packet);
            return;
        }
        NuSpecialGetRadius(&animation->special, &object->force_glow_position, &object->field_0xd8c);
        NuVecMtxTransformVU0(&object->force_glow_position, &object->force_glow_position, NuSpecialGetDrawMtx(&animation->special));
        object->field_0xe1e = model;
        object->field_0xd8c *= scale;
        GIZFORCEANIMDATA_s *data = static_cast<GIZFORCEANIMDATA_s *>(animation->object_data);
        if (data != NULL) data->field_0x04 |= 1;
        break;
    }
    case 2: {
        GameObject_s *target_object = static_cast<GameObject_s *>(object->force_glow_object);
        if ((target_object->apiobj.field_0x1f8 & 0x1001) == 0 ||
            (TouchHacks::TouchControlsActive && target_object->apiobj.field_0x27c != -1)) {
            ResetForceGlow(packet);
            return;
        }
        object->field_0xd8c = NuFmax(target_object->apiobj.field_0x1dc, target_object->apiobj.field_0x1e0) * 1.75f;
        object->field_0xe1e = model;
        object->force_glow_position = target_object->apiobj.collision_position;
        f32 bottom = target_object->character_bottom;
        object->force_glow_position.y = bottom * target_object->apiobj.field_0xa8 + target_object->apiobj.position.y +
            (target_object->character_top - bottom) * 0.5f * target_object->apiobj.field_0xa8;
        break;
    }
    case 3: {
        PART_s *part = static_cast<PART_s *>(object->force_glow_object);
        if ((part->active & 1) == 0) {
            ResetForceGlow(packet);
            return;
        }
        if (NuSpecialExistsFn(&part->special)) {
            NUVEC center;
            NuSpecialGetRadius(&part->special, &center, &object->field_0xd8c);
            object->field_0xd8c += object->field_0xd8c;
        } else object->field_0xd8c = part->radius + part->radius;
        object->field_0xe1e = model;
        object->force_glow_position = part->position;
        break;
    }
    }
    object->field_0xd8c *= 1.125f;
}

static void ForceCode(GameObject_s *object, i32 pressed, i32 held, i32) {
    u8 previous_flags = object->field_0xf02;
    object->field_0xf02 &= ~0x10;
    if (object->character_context == 8) {
        if ((!held && (object->pad_gamepad->allocated_5a & 4) == 0) || object->gizforce_target == NULL) {
            object->character_context = -1;
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
            return;
        }
        if ((dagobah_training == 0 || object->field_0xcc0 != NULL || FreePlay != 0) &&
            !GizForce_Complete(object->gizforce_target) && !GizForce_StoodOnForce(object->gizforce_target, object)) {
            AlertSurroundingCreatures(object, &object->apiobj.collision_position);
            if ((WORLD->current_level == VADERC_LDATA || (object->apiobj.flags_low & 0x80) == 0 ||
                 object->apiobj.model_draw_result == 0 || object->field_0xc54 <= 0.0f ||
                 (object->head_target_facing >= -0.85f && object->head_target_facing <= 0.85f)) &&
                !(WORLD->current_level == DAGOBAHC_LDATA && object->id == id_LUKESKYWALKERDAGOBAH &&
                  static_cast<void *>(object->gizforce_target) == LevGizmo[0] &&
                  GameAnimSet_GetCompletionRatio(object->gizforce_target->anim_set) > 0.5f)) {
                GIZFORCE_s *force = object->gizforce_target;
                force->using_object = object;
                object->force_heading = GizForces_AngleToForce(&object->apiobj.position, force);
                AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                object->field_0xe22 |= 2;
                force = object->gizforce_target;
                if (object->gizforce_target_object == NULL) {
                    object->force_glow_candidate = force;
                    object->force_glow_candidate_kind = 0;
                    if (force != NULL) SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                } else {
                    object->force_glow_candidate = object->gizforce_target_object;
                    object->force_glow_candidate_kind = 1;
                    object->field_0xd8c = force == NULL ? 1.0f : force->horizontal_range;
                    SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f, 5.0f, 10.0f);
                    if (force != NULL) SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                }
                if (object->character_context == -1) GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
                return;
            }
        } else AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        object->character_context = -1;
        object->gizforce_target = NULL;
        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        return;
    }
    if ((GetGameCharacterData(object)->flags_094[1] & 0x80) != 0 || (object->field_0xe23 & 1) != 0 ||
        object->character_context == 0x12) return;
    bool start = false;
    if (object->apiobj.field_0x27d != 0 &&
        (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0)) {
        if (((object->apiobj.flags_low & 0x80) != 0 && pressed) ||
            (object->pad_gamepad->allocated_5a & 4) != 0 || (held && (previous_flags & 0x10) != 0)) start = true;
    }
    if (start) {
        if ((object->pad_gamepad->allocated_5a & 4) == 0 || object->gizforce_target == NULL) {
            GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
        }
        if (object->gizforce_target != NULL) {
            object->context_animation_timer = 0.5f;
            object->character_context = 8;
            NewRumble(object->pad_gamepad->pad, 0.6f, 0);
            object->gizforce_target->using_object = object;
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
            object->force_heading = GizForces_AngleToForce(&object->apiobj.position, object->gizforce_target);
            if ((object->apiobj.flags_low & 0x80) != 0) {
                Hint_SetComplete(0x259);
                if ((object->gizforce_target->config_flags & 0x10) != 0) Hint_SetComplete(0x623);
                LSW_HintConditions |= 1;
            }
        }
    } else if ((object->apiobj.flags_low & 0x80) != 0) GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
    GIZFORCE_s *force = object->gizforce_target;
    if (force != NULL && (object->apiobj.flags_low & 0x80) != 0 &&
        (object->field_0xe22 & 2) == 0 && object->apiobj.field_0x27d != 0) {
        object->field_0xe22 |= 2;
        if (object->gizforce_target_object == NULL) {
            object->force_glow_candidate = force;
            object->force_glow_candidate_kind = 0;
            SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
        } else {
            object->force_glow_candidate_kind = 1;
            object->force_glow_candidate = object->gizforce_target_object;
            object->field_0xd8c = force->horizontal_range;
            SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f, 5.0f, 10.0f);
        }
        SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
    }
    if (object->character_context == 8 && object->id == id_LUKESKYWALKERDAGOBAH &&
        dagobah_training != 0 && object->field_0xcc0 == NULL && FreePlay == 0) {
        EndForce(object, 0);
        if (AnimPlaying(&object->apiobj.anim_packet, 0xb, 1, 1) == NULL &&
            AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL) {
            PlaySfx("JForcePush", &object->apiobj.collision_position);
            PlayGruntSfx(object);
        }
        NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        if (object->gizforce_target != NULL) {
            object->gizforce_target->using_object = NULL;
            object->apiobj.movement_facing_angle = GizForces_AngleToForce(&object->apiobj.collision_position, object->gizforce_target);
        }
        object->field_0xe23 |= 1;
        GameCam_HitRoll();
    }
}

static void ForcePushed_MoveCode(GameObject_s *object) {
    NuFmax(NuFmax(0.4f, 1.2f), 0.8f);
    GameObject_s *source = object->force_target;
    if (object->character_context != 0x1c || source == NULL || source->character_context != 0x1b ||
        (source->field_0xe21 & 1) != 0 || (object->field_0xe20 & 0x80) != 0 || (object->field_0xe21 & 2) != 0) return;
    if (object->action_movement_state == 4 && Cheat_IsOn(0x13)) {
        if ((source->apiobj.field_0x1e4 & object->apiobj.field_0x1ec) != 0 ||
            (source->apiobj.field_0x1e8 & object->apiobj.field_0x1f0) != 0) {
            i16 angle = NuAtan2D(source->apiobj.pos_x - object->apiobj.pos_x, source->apiobj.pos_z - object->apiobj.pos_z);
            objhitobj_killparts_yrot = &angle;
            if (ObjHitObj(NULL, object, -1, 0, 0, 1) == 2) {
                NewRumble(source->pad_gamepad->pad, 0.5f, 0);
                NewBuzz(source->pad_gamepad->pad, 0.1f, 0);
                GameCam_HitJudder();
            }
        }
    } else if (ForcePush_SuperPush) {
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
            GameObject_s *target = &Obj[i];
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
                target == source || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                (target->apiobj.character_data->model_flags & 0x4002010) != 0x10 ||
                (GetGameCharacterData(target)->flags_090 & 0x40) != 0 ||
                (GetGameCharacterData(target)->flags_094[1] & 2) != 0) continue;
            if (target->id == id_GONKDROID) {
                if (target->character_context == 0x17) continue;
                if ((target->apiobj.field_0x1e4 & object->apiobj.field_0x1ec) == 0 &&
                    (target->apiobj.field_0x1e8 & object->apiobj.field_0x1f0) == 0) continue;
                DeactivatePlayer(target, DEACTIVATEDTIME, NULL);
            } else {
                if ((target->apiobj.field_0x1e4 & object->apiobj.field_0x1ec) == 0 &&
                    (target->apiobj.field_0x1e8 & object->apiobj.field_0x1f0) == 0) continue;
                i16 angle = NuAtan2D(target->apiobj.pos_x - object->apiobj.pos_x, target->apiobj.pos_z - object->apiobj.pos_z);
                objhitobj_killparts_yrot = &angle;
                if (ObjHitObj(NULL, target, -1, 0, 0, 1) != 2) continue;
                if (static_cast<u8>(source->apiobj.field_0x27c) < 2 && target->apiobj.field_0x27c == -1 && Arcade)
                    Arcade_AIKilled(source->apiobj.field_0x27c);
            }
            NewRumble(source->pad_gamepad->pad, 0.5f, 0);
            NewBuzz(source->pad_gamepad->pad, 0.1f, 0);
            GameCam_HitJudder();
        }
    }
}


static void DeactivatedCode(GameObject_s *object) {
    if (object->character_context == 0x41) {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL) return;
        object->airborne_action_duration += FRAMETIME;
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) object->character_context = -1;
        return;
    }
    if ((object->apiobj.character_data->model_flags & 0x20000000) != 0) {
        bool controlled = false;
        for (i32 i = 0; i < 8; ++i) {
            if (Player[i] != NULL && Player[i]->character_context == 0x51 && Player[i]->field_0x788 != NULL &&
                static_cast<TECHNO *>(Player[i]->field_0x788)->controlled_object == object) controlled = true;
        }
        if (controlled || object->field_0xcc0 != NULL || (object->apiobj.flags_low & 0x80) != 0) {
            if (object->character_context == 0x17) ActivatePlayer(object);
        } else if (object->character_context != 0x2b && object->character_context != 0x3e) {
            if (object->character_context != 0x17) DeactivatePlayer(object, 1.0e9f, NULL);
            else object->context_animation_timer = 1.0e9f;
        }
    }
    if (object->character_context != 0x17) return;
    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
        object->airborne_action_duration += FRAMETIME;
    } else {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL) return;
        object->airborne_action_duration += FRAMETIME;
        if (object->context_animation == 0x81) {
            if (AnimDuration(object->id, 0x81, 0.0f, 0.0f, 1) <= object->airborne_action_duration) {
                if (object->apiobj.character_model->model_data_b[0x41] == NULL) {
                    ActivatePlayer(object);
                    return;
                }
                object->context_animation = 0x41;
                if (object->context_animation_timer != 1.0e9f) object->context_animation_timer -= object->airborne_action_duration;
                object->airborne_action_duration = 0.0f;
            }
        } else if ((static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[object->context_animation])->flags & 2) == 0 &&
                   AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1) <= object->airborne_action_duration) {
            ActivatePlayer(object);
            return;
        }
    }
    if (object->context_animation_timer != 1.0e9f && object->context_animation_timer < object->airborne_action_duration)
        ActivatePlayer(object);
    else if ((object->apiobj.character_data->model_flags & 0x20) != 0) SetProtocolDroidDeactivatedAction(object);
    if (object->field_0x768 <= 0.0f) return;
    object->field_0x768 -= FRAMETIME;
    if (object->field_0x768 > 0.0f) return;
    i32 kind;
    f32 duration;
    f32 range;
    if (object->action_movement_state == 2) {
        kind = 2;
        duration = DEACTIVATEDTIME;
        range = 9.0f;
    } else {
        if (!ForcePush_SuperMindTrick || object->action_movement_state != 1) return;
        kind = 1;
        duration = 5.0f;
        range = 1.0f;
    }
    GameObject_s *targets[10];
    i32 count = 0;
    for (i32 i = 0; i < HIGHGAMEOBJECT && count < 10; ++i) {
        GameObject_s *target = &Obj[i];
        if (kind == 1) {
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
                target->apiobj.field_0x27d == 0 || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                target->apiobj.character_model->model_data_b[0x41] == NULL) continue;
            i32 context = target->character_context;
            if (context == 0x3c || context == 0x39 || context == 0x3b || context == 0x17 || context == 0x41 ||
                context == 0x0f || context == 0x47 || context == 0x46 ||
                (target->apiobj.character_data->model_flags & 0x10) != 0 ||
                (GetGameCharacterData(target)->flags_090 & 0x8000) != 0 || (CInfo[context].flags & 0x8000) != 0 ||
                (GetGameCharacterData(target)->flags_094[1] & 2) != 0) continue;
        } else {
            if (!ZapTarget(target) || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                (GetGameCharacterData(target)->flags_090 & 0x8040) != 0 ||
                target->character_context == 0x0f || target->character_context == 0x3c ||
                target->character_context == 0x47 || target->character_context == 0x46) continue;
        }
        if (NuVecDistSqr(&target->apiobj.collision_position, &object->apiobj.collision_position, NULL) < range)
            targets[count++] = target;
    }
    i32 selected = count == 1 ? 0 : count > 1 ? qrand() / (0xffff / count + 1) : -1;
    if (selected == -1) return;
    GameObject_s *target = targets[selected];
    if (DeactivatePlayer(target, duration, NULL)) {
        target->action_movement_state = kind;
        target->context_variant_flags = (target->context_variant_flags & ~1) | (object->context_variant_flags & 1);
        if (kind == 2) {
            PlaySfx("R2Zap", &object->apiobj.collision_position);
            AddGameDebris(WORLD->debris_sys, (target->context_variant_flags & 1) != 0 ? 1 : 3, &target->apiobj.collision_position);
            GameCam_HitJudder();
        }
        target->field_0x768 = qrand() * 1.5259022e-05f * 0.3f + 0.2f;
        if (target->id == id_JAWA) PlaySfx("Jawa_Dizzy", &target->apiobj.collision_position);
    }
}


static void DrawLightningBolts(GameObject_s *, GameObject_s *, i32) {
}

static void ForcePushCode(GameObject_s *object, i32 held, i32) {
    if (object->character_context != 0x1b) return;
    AlertSurroundingCreatures(object, &object->apiobj.collision_position);
    GameObject_s *initial_target = object->force_target;
    if ((object->pad_gamepad->allocated_5a & 0x10) != 0) held = 1;
    object->field_0xe22 |= 2;
    GameObject_s *target = initial_target;
    if (target != NULL) {
        if (target->character_context == 0x0f) {
            object->force_target = NULL;
            EndForce(object, 0);
            target = object->force_target;
        } else if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0) {
            object->force_target = NULL;
            target = NULL;
        } else if ((object->field_0xe21 & 6) == 0 && target->character_context != 0x1c) {
            object->force_target = NULL;
            EndForce(object, 0);
            target = object->force_target;
        }
    }
    if (target != NULL) {
        SetObjAsHeadTarget(object, target, 2, 1.0f, 0.0f, 0.0f);
        SetObjAsHeadTarget(target, object, 2, 1.0f, 0.0f, 0.0f);
        if ((object->field_0xe21 & 4) != 0) {
            object->force_glow_candidate = target;
            object->force_glow_candidate_kind = 2;
            if ((object->field_0xe21 & 1) != 0) DrawLightningBolts(object, target, 0);
        } else {
            NewRumble(target->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.4f, 0);
            if ((object->field_0xe21 & 1) != 0) DrawLightningBolts(object, target, 1);
            object->force_glow_candidate = target;
            object->force_glow_candidate_kind = 2;
            if (target->character_context != 0x1c || (target->apiobj.character_data->model_flags & 0x20) != 0 ||
                target->apiobj.character_model->model_data_b[target->context_animation] == NULL ||
                CurrentAnim(&target->apiobj.anim_packet) == target->context_animation)
                object->context_animation_timer += FRAMETIME;
            NewRumble(object->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.4f, 0);
            f32 duration = (object->field_0xe21 & 2) != 0 ? 0.4f :
                ((object->field_0xe20 & 0x80) != 0 || (object->field_0xe21 & 1) != 0) ? 1.2f : 0.8f;
            object->field_0x7a3 = 0;
            bool throw_target = false;
            if ((object->apiobj.flags_low & 0x80) != 0 && (object->apiobj.field_0x1f4 & 0x40000) == 0 &&
                ForcePush_Waft && (target->apiobj.flags_low & 0x80) == 0 && object->context_animation_timer >= 0.2f &&
                object->pad_gamepad->input_magnitude > 0.0f && ((object->field_0xe21 & 1) == 0 ||
                (GetGameCharacterData(target)->flags_090 & 0x4000) == 0) &&
                ((object->field_0xe20 & 0x80) != 0 || (object->field_0xe21 & 1) != 0)) {
                object->field_0x7a3 = 1;
                object->external_force.x = NU_SIN_LUT(object->apiobj.facing_angle) * 0.5f + object->apiobj.position.x;
                object->external_force.y = (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.75f + object->apiobj.collision_min.y;
                object->external_force.z = NU_COS_LUT(object->apiobj.facing_angle) * 0.5f + object->apiobj.position.z;
                u16 angle = NuAtan2D(target->apiobj.position.x - object->apiobj.position.x, target->apiobj.position.z - object->apiobj.position.z);
                i32 difference = RotDiff(GamePad_InputAngle(object, object->pad_gamepad), angle);
                if ((difference < 0 ? -difference : difference) < 0xaaa) throw_target = true;
                else if (duration - 0.3f <= object->context_animation_timer) object->context_animation_timer = duration - 0.3f;
            }
            if (object->context_animation_timer >= duration ||
                (((object->field_0xe20 & 0x80) == 0 && (object->field_0xe21 & 3) == 0) &&
                 object->context_animation_timer >= 0.2f && initial_target->field_0x1084 != 0 &&
                 NuFabs(initial_target->contact_normal.y) < NU_SIN_LUT(0x6aaa))) {
                if (throw_target) {
                    object->force_target = NULL;
                    object->character_context = -1;
                    object->pad_gamepad->allocated_5a &= ~0x10;
                    target->force_target = NULL;
                    object->field_0xd14 = 0;
                    EndForce(target, 0);
                    PlaySfx("JForcePush", &object->apiobj.collision_position);
                    KillRumble(object);
                    PlayHurtSfx(target);
                    GameCam_NewShake(NULL, 0.75f, 0.75f, 1.0f);
                    target->character_context = 0x5f;
                    if (target->apiobj.character_model->model_data_b[0xb8] != NULL) {
                        target->context_animation = 0xb8;
                        ResetAnimPacket(&target->apiobj.anim_packet, 0xffff);
                    } else if (target->apiobj.character_model->model_data_b[5] != NULL) target->context_animation = 5;
                    target->current_hp = 1;
                    target->context_animation_timer = 0.0f;
                    target->airborne_action_duration = 2.0f;
                    u16 angle = NuAtan2D(target->apiobj.pos_x - object->apiobj.pos_x, target->apiobj.pos_z - object->apiobj.pos_z);
                    f32 speed = (qrand() * 1.5259022e-05f * 0.4f + 0.8f) * DIEAIRSPEED;
                    target->external_force.x = NU_SIN_LUT(angle) * speed;
                    target->external_force.z = NU_COS_LUT(angle) * speed;
                    target->context_variant_flags &= ~1;
                    target->apiobj.velocity.y = (qrand() * 1.5259022e-05f * 0.4f + 0.8f) * DIEAIRJUMPSPEED;
                    target->hit_variant = static_cast<u8>(object->apiobj.field_0x27c) <= 1 ? object->apiobj.field_0x27c : -1;
                    SetFlicker(target, 0.4f);
                } else {
                    bool deactivate = true;
                    if ((object->field_0xe21 & 2) != 0) {
                        if (DeactivatePlayer(target, 5.0f, NULL)) {
                            target->action_movement_state = 1;
                            target->field_0x768 = qrand() * 1.5259022e-05f * 0.3f + 0.2f;
                            if (target->id == id_JAWA) PlaySfx("Jawa_Dizzy", &target->apiobj.collision_position);
                        }
                    } else if (target->id == id_GONKDROID ||
                        ((target->apiobj.character_data->model_flags & 0x10) != 0 && target->hitpoints >= 2))
                        DeactivatePlayer(target, DEACTIVATEDTIME, NULL);
                    else {
                        bool immune = FreePlay != 0 && (object->field_0xe21 & 1) != 0 &&
                            (GetGameCharacterData(target)->flags_090 & 0x4000) != 0;
                        i32 damage = immune ? 0 : target->apiobj.field_0x27c == -1 ? static_cast<i8>(target->current_hp) : 1;
                        u16 flags = WORLD->current_level == BLOCKADERUNNERB_LDATA ? (object->apiobj.field_0x1f4 >> 10) & 1 : 0;
                        if (ObjHitObj(object, target, damage, flags, 0, 1) != 2 && (object->field_0xe20 & 0x80) != 0 && !immune)
                            target->fall_animation_timer = 0.2f;
                        if (object->force_target != NULL) {
                            object->force_target->force_target = NULL;
                            EndForce(object->force_target, 0);
                        }
                        object->force_target = NULL;
                        object->pad_gamepad->allocated_5a &= ~0x10;
                        object->field_0xd14 = 0;
                        deactivate = false;
                    }
                    if (deactivate) {
                        NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
                    }
                }
                object->airborne_action_duration = 0.0f;
                GameCam_HitJudder();
            }
        }
    }
    if (object->airborne_action_duration >= 0.2f && held != 0 && object->force_target != NULL) {
        object->airborne_action_duration = object->airborne_action_duration - FRAMETIME > 0.2f
            ? object->airborne_action_duration - FRAMETIME : 0.2f;
    } else if ((object->apiobj.field_0x1f4 & 0x40000) == 0) {
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f) {
            target = object->force_target;
            if (object->context_animation_timer < 0.4f) {
                ReleaseForce(object, 0);
                if ((object->field_0xe20 & 0x80) == 0 || target == NULL) return;
            } else {
                if (target == NULL) {
                    ReleaseForce(object, 0);
                    return;
                }
                if ((target->apiobj.field_0x1f8 & 1) != 0 && target->apiobj.field_0x287 == 0 &&
                    (GetGameCharacterData(target)->uses_weapon_action == 4 || target->id == id_BATTLEDROIDSECURITY ||
                     target->id == id_GONKDROID || target->id == id_PKDROID || target->id == id_PITDROID)) {
                    DeactivatePlayer(target, DEACTIVATEDTIME * 0.5f, NULL);
                    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                }
                ReleaseForce(object, 0);
                if ((object->field_0xe20 & 0x80) == 0) return;
            }
            target->fall_animation_timer = 0.2f;
        }
    }
}


static void ForceThrowCode(GameObject_s *object, i32 pressed, i32) {
    if (object->character_context == 0x12) {
        AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            CurrentAnim(&object->apiobj.anim_packet) != object->context_animation) return;
        f32 previous_time = object->context_animation_timer;
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
            object->gizforce_target = NULL;
            return;
        }
        NewRumble(object->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.5f, 0);
        PART_s *part = static_cast<PART_s *>(object->field_0x788);
        if (part == NULL) {
            GIZFORCE_s *force = object->gizforce_target;
            if (force != NULL) {
                object->field_0xe22 |= 2;
                if ((object->context_flags & 0x40) == 0) {
                    if (object->gizforce_target_object == NULL) {
                        object->force_glow_candidate = force;
                        object->force_glow_candidate_kind = 0;
                        SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                    } else {
                        object->force_glow_candidate = object->gizforce_target_object;
                        object->force_glow_candidate_kind = 1;
                        object->field_0xd8c = force->horizontal_range;
                        SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f, 5.0f, 10.0f);
                    }
                    SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                    object->force_heading = GizForces_AngleToForce(&object->apiobj.position, object->gizforce_target);
                } else if (force->anim_set != NULL && force->anim_set->object_count == 1) {
                    part = Part_FindFromHSpecial(&force->anim_set->objects->special);
                    if (part != NULL) {
                        object->force_glow_candidate = part;
                        object->force_glow_candidate_kind = 3;
                        SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
                        object->force_heading = NuAtan2D(part->position.x - object->apiobj.position.x, part->position.z - object->apiobj.position.z);
                        object->force_glow_object = object->force_glow_candidate;
                        object->force_glow_kind = object->force_glow_candidate_kind;
                    }
                }
            }
        } else if ((part->active & 1) == 0) object->character_context = -1;
        else {
            object->force_heading = NuAtan2D(part->position.x - object->apiobj.position.x, part->position.z - object->apiobj.position.z);
            SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
        }
        if (previous_time <= 1.0f || object->context_animation_timer > 1.0f) return;
        f32 speed, gravity;
        if (WORLD->current_level == MAULF_LDATA) {
            speed = MaulF_ForceThrowSpeed;
            gravity = MaulF_ForceThrowGravity;
        } else if (WORLD->current_level == DOOKUC_LDATA) {
            speed = DookuC_ForceThrowSpeed;
            gravity = DookuC_ForceThrowGravity;
        } else {
            speed = ForceThrowSpeed;
            gravity = ForceThrowGravity;
        }
        object->field_0x788 = GizForce_Throw(object, object->gizforce_target, speed, gravity, 0);
        object->context_flags |= 0x40;
        return;
    }
    if (!pressed && (object->pad_gamepad->allocated_5a & 4) == 0) return;
    if ((object->apiobj.field_0x1f8 & 0x180) == 0x80) GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
    if (object->gizforce_target == NULL || (object->gizforce_target->config_flags & 0x80) == 0) return;
    object->character_context = 0x12;
    object->context_animation = 0x27;
    if (object->apiobj.character_model->model_data_b[0x27] == NULL) object->context_animation = 0xb;
    object->context_flags &= ~0x40;
    object->field_0x788 = NULL;
    object->force_throw_target = NULL;
    object->context_animation_timer = 2.0f;
    f32 distance = 1.0e9f;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *target = Player[i];
        if (target == NULL || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            target->apiobj.field_0x287 != 0 || (target->apiobj.character_data->model_flags & 0x80000) != 0) continue;
        f32 next_distance = NuVecDistSqr(&object->apiobj.position, &target->apiobj.position, NULL);
        if (object->force_throw_target == NULL || (((object->apiobj.flags_low & 0x80) != 0 ||
            (object->force_throw_target->apiobj.flags_low & 0x80) == 0) && next_distance < distance)) {
            object->force_throw_target = target;
            distance = next_distance;
        }
    }
    NewRumble(object->pad_gamepad->pad, 0.6f, 0);
    object->gizforce_target->using_object = object;
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    object->force_heading = GizForces_AngleToForce(&object->apiobj.position, object->gizforce_target);
}


static void ForceDeflectCode(GameObject_s *object, i32 pressed, i32 held, i32) {
    i32 waiting = objInNetWaitContext(object, 0x1d);
    if (object->character_context == 0x1d) {
        object->field_0xe22 |= 2;
        object->context_animation_timer += FRAMETIME;
        if ((object->apiobj.flags_low & 0x80) != 0)
            NewRumble(object->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.6f, 0);
        if (object->force_part != NULL) {
            PART_s *part = object->force_part;
            part->force_flags |= 1;
            part->flags &= ~0x4000;
            if (object->context_animation_timer < 0.2f) part->velocity.y = SeekValF(part->velocity.y, 2.0f, 6.0f);
            part = object->force_part;
            NUVEC direction;
            if (part->owner == NULL || part->owner->apiobj.field_0x27c != -1)
                NuVecRotateY(&direction, &v001, object->apiobj.facing_angle);
            else {
                NuVecSub(&direction, &part->owner->apiobj.collision_position, &part->position);
                NuVecNorm(&direction, &direction);
            }
            NUVEC velocity;
            NuVecScale(&velocity, &direction, 4.0f);
            SeekVec(&object->force_part->velocity, &object->force_part->velocity, &velocity, 3.0f);
            object->force_glow_candidate = object->force_part;
            object->force_glow_candidate_kind = 3;
            SetHeadTarget(object, &object->force_part->position, 2, 1.0f, 0.0f, 0.0f);
            part = object->force_part;
            if (part->move_callback == BobaRocket_Move) {
                part->move_callback = NULL;
                part->update_callback = NULL;
                part->flags = (part->flags & ~0x4000) | 0x80;
                NewPartRotation(part);
            } else if (part->move_callback == Boulder_Move && object->context_animation_timer >= 0.5f)
                Boulder_Kill(part, 0);
        }
        if ((held != 0 || (object->field_0xefd & 1) != 0) && object->force_part != NULL &&
            object->airborne_action_duration >= 0.2f && object->context_animation_timer < 1.5f) {
            object->airborne_action_duration = object->airborne_action_duration - FRAMETIME > 0.2f
                ? object->airborne_action_duration - FRAMETIME : 0.2f;
            return;
        }
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f) ReleaseForce(object, 0);
        return;
    }
    PART_s *assist = NULL;
    if ((object->apiobj.flags_low & 0x80) == 0) {
        if ((object->field_0xefd & 1) == 0 || player == NULL || player->force_part == NULL) return;
        assist = player->force_part;
    }
    if (object->apiobj.field_0x27d == 0) return;
    i32 context = object->character_context;
    if (context != -1 && (CInfo[context].flags & 4) == 0 && context != 6 && context != 7 &&
        !objInNetWaitContext(object, 0x1d)) return;
    PART_s *part;
    if (waiting) part = object->force_part;
    else if (assist != NULL) part = assist;
    else {
        f32 range = TouchHacks::GetIncomingPartRange();
        f32 radius = object->apiobj.field_0x1e0;
        if (radius <= object->apiobj.field_0x1dc) radius = object->apiobj.field_0x1dc;
        part = FindIncomingPart(object, &object->apiobj.collision_position, radius, 0x800a, range);
    }
    if (part == NULL) return;
    object->field_0xe22 |= 2;
    object->force_glow_candidate = part;
    object->force_glow_candidate_kind = 3;
    SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
    if (assist == NULL && pressed == 0 && !waiting) return;
    object->force_part = part;
    part->flags |= 0x10000;
    part->gravity = 0.0f;
    object->context_animation_timer = 0.0f;
    object->airborne_action_duration = 0.3f;
    object->force_target = NULL;
    object->blowup_target = NULL;
    u8 mask = 1u << (object->apiobj.field_0x27c & 31);
    object->character_context = 0x1d;
    if (part->force_player_mask != -1) mask |= part->force_player_mask;
    part->force_player_mask = mask;
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    PlaySfx("JForcePush", &object->apiobj.collision_position);
}

void Move_WEIRDO(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    u32 action_held = pad->buttons_held & GAMEPAD_ACTION;
    u32 action_pressed = pad->buttons_pressed & GAMEPAD_ACTION;
    u32 jump_held = pad->buttons_held & GAMEPAD_JUMP;
    u32 jump_pressed = pad->buttons_pressed & GAMEPAD_JUMP;
    u32 special_held = pad->buttons_held & GAMEPAD_SPECIAL;
    u32 special_pressed = pad->buttons_pressed & GAMEPAD_SPECIAL;
    u32 tag_pressed = pad->buttons_pressed & GAMEPAD_TAG;
    i32 hit_effect = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].hit_effect;
    i16 glow_model;
    if (SuperWeirdo(object) && (object->apiobj.character_data->model_flags & 8) == 0) glow_model = 0xdf;
    else glow_model = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].glow_model;
    DropInOutCode(object);
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    u16 weapon = GetGameCharacterData(object)->weapon_model & 0xfffd;
    bool has_sabre = weapon == 0x65 || weapon == 0x69;
    if (object->suit != NULL) Signal_MoveCode(WORLD, object);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    Hang_MoveCode(object);
    Ledge_MoveCode(WORLD, object);
    if (LedgeTerrain_On) LedgeTerrain_MoveCode(object);
    Climb_MoveCode(object);
    TightRope_MoveCode(object, jump_pressed);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    BackFlipCode(object);
    TakeOverCode(object, tag_pressed);
    Glide_MoveCode(object);
    u32 jump_animations;
    if (!has_sabre) {
        hit_effect = -1;
        jump_animations = SuperWeirdo(object) ? 0x109 : 0x208;
    } else jump_animations = SuperWeirdo(object) ? 0x11b : 0x1b;
    JumpCode(object, jump_pressed, jump_held, jump_animations, action_pressed, action_held, hit_effect);
    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    BuildIt_MoveCode(object);
    if (has_sabre || SuperWeirdo(object)) {
        ForceDeflectCode(object, special_pressed, special_held, 0);
        ForcePushCode(object, special_held, 0);
        FindForcePushTarget(object, special_pressed, 1);
        ForceThrowCode(object, special_pressed, 0);
        ForceCode(object, special_pressed, special_held, 0);
        FindForcePushTarget(object, special_pressed, 2);
        if (object->apiobj.field_0x287 == 0 && FadeSys.fade == 0.0f) {
            if (object->force_glow_step <= 0.0f) ForceGlowCode(object, glow_model);
            else object->force_glow_step -= FRAMETIME;
        } else ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }
    ForcePushed_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    if ((object->apiobj.character_data->model_flags & 0x40000) != 0 || SuperWeirdo(object))
        Teleport_MoveCode(object, special_pressed);
    ThermalDetonator_MoveCode(object);
    GizmoBlowupCheckProximity(WORLD, object);
    ComboRotateCode(object, action_held);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    HoldCode(object);
    if (has_sabre) {
        BlockCode(object, action_pressed, action_held, jump_pressed, 0);
        SwipeCode(object, action_pressed, action_held);
        LightSabreComboCode(object, action_pressed, action_held, jump_pressed, special_pressed);
        JediLightCode(object);
    } else BlockCode(object, action_pressed, action_held, 0, 0);
    if (object->character_context == -1 && object->field_0xe31 == 0 &&
        object->apiobj.field_0x27d != 0 && object->apiobj.field_0x27e == 0 &&
        ((object->movement_runtime_flags & 4) != 0 || (object->fall_animation_timer >= 0.2f &&
         (object->pad_gamepad->input_magnitude == 0.0f || ((object->apiobj.flags_low & 0x80) == 0 &&
          object->apiobj.character_model->model_data_b[0x59] != NULL))))) StartFallLand(object, -1);
    if (object->apiobj.field_0x27d != 0) object->movement_runtime_flags &= ~4;
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
}

void Move_JEDI(GameObject_s *object) {
    const u32 jump_mask = GAMEPAD_JUMP;
    const u32 action_mask = GAMEPAD_ACTION;
    const u32 special_mask = GAMEPAD_SPECIAL;
    GAMEPAD_s *pad = object->pad_gamepad;
    const u32 pressed = pad->buttons_pressed;
    const u32 held = pad->buttons_held;

    if (object->id == id_BODYGUARD) {
        KeepWeaponOut(object);
    }
    DropInOutCode(object);
    if ((object->field_0xe20 & GAMEOBJECT_E20_FLAG_MOVEMENT_DISABLED) != 0) {
        return;
    }

    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    i32 glow_model;
    i32 hit_effect;
    if (object->id == id_BOB) {
        if ((object->field_0xefd & 2) != 0) {
            glow_model = 0xdd;
            hit_effect = 2;
        } else {
            glow_model = 0xe1;
            hit_effect = 3;
        }
    } else if (AnakinGreenSabre(object)) {
        glow_model = 0xdd;
        hit_effect = 2;
    } else {
        hit_effect = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].hit_effect;
        if (SuperWeirdo(object) && (object->apiobj.character_data->model_flags & 8) == 0) {
            glow_model = 0xdf;
        } else {
            glow_model = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].glow_model;
        }
    }
    if (object->suit != NULL) {
        Signal_MoveCode(WORLD, object);
    }
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    Hang_MoveCode(object);
    Ledge_MoveCode(WORLD, object);
    if (LedgeTerrain_On != 0) {
        LedgeTerrain_MoveCode(object);
    }
    Climb_MoveCode(object);
    const i32 jump_pressed = pressed & jump_mask;
    TightRope_MoveCode(object, jump_pressed);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    BackFlipCode(object);
    if (object->id != id_GRIEVOUS && object->id != id_BODYGUARD) {
        TakeOverCode(object, object->pad_gamepad->buttons_pressed & GAMEPAD_TAG);
    }
    Glide_MoveCode(object);

    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    u32 jump_animations = 0x1b;
    if (object->id == id_IMPERIALGUARD || object->id == id_BODYGUARD) {
        jump_animations = 0x10;
    }
    if ((game_character->flags_090 & 0x00400000) != 0) {
        jump_animations |= 0x100;
    }
    const i32 action_pressed = pressed & action_mask;
    const i32 action_held = held & action_mask;
    const i32 special_pressed = pressed & special_mask;
    JumpCode(object, jump_pressed, held & jump_mask, jump_animations, action_pressed, action_held, hit_effect);

    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    BuildIt_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[1] & 0x80) == 0) {
        ForceDeflectCode(object, special_pressed, held & special_mask, 0);
        ForcePushCode(object, held & special_mask, 0);
        FindForcePushTarget(object, special_pressed, 1);
        ForceThrowCode(object, special_pressed, 0);
        ForceCode(object, special_pressed, held & special_mask, 0);
        FindForcePushTarget(object, special_pressed, 2);
        if (object->apiobj.field_0x287 == 0 && FadeSys.fade == 0.0f) {
            if (object->force_glow_step <= 0.0f) ForceGlowCode(object, glow_model);
            else object->force_glow_step -= FRAMETIME;
        } else ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }
    if ((object->apiobj.character_data->model_flags & 0x00040000) != 0) {
        Teleport_MoveCode(object, special_pressed);
    }
    ThermalDetonator_MoveCode(object);
    if (object->suit != NULL && (static_cast<SUIT_s *>(object->suit)->store_flag & 4) != 0)
        Detonator_MoveCode(object);
    ComboRotateCode(object, action_held);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    HoldCode(object);
    BlockCode(object, action_pressed, action_held, jump_pressed, 0);
    SwipeCode(object, action_pressed, action_held);
    LightSabreComboCode(object, action_pressed, action_held, jump_pressed, special_pressed);
    JediLightCode(object);
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) != 0 && WORLD->debris_sys->entries[124].effect != -1)
        AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[124].effect,
            &object->apiobj.collision_position, 6, FRAMETIME, 0, 0, NULL);
    if (object->character_context == -1 && object->field_0xe31 == 0 &&
        object->apiobj.field_0x27d != 0 && object->apiobj.field_0x27e == 0) {
        if ((object->movement_runtime_flags & 4) != 0 ||
            (object->fall_animation_timer >= 0.2f &&
             (object->pad_gamepad->input_magnitude == 0.0f ||
              ((object->apiobj.flags_low & 0x80) == 0 &&
               object->apiobj.character_model->model_data_b[0x59] != NULL)))) {
            StartFallLand(object, -1);
        }
    }
    if (object->apiobj.field_0x27d != 0) object->movement_runtime_flags &= ~4;
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
    f32 force_volume = 0.0f;
    if ((object->character_context == 0x1b && object->field_0x7a3 == 1) ||
        object->character_context == 0x1d || object->character_context == 8) {
        force_volume = 1.0f;
    }
    object->force_use_volume = SeekLinearF(object->force_use_volume, force_volume, FRAMETIME);
    if (object->force_use_volume > 0.0f) {
        PlaySfxAndSetVolume("JForceUse", &object->apiobj.collision_position, object->force_use_volume);
    }
    if ((object->apiobj.flags_low & 0x80) != 0 && force_volume == 1.0f &&
        !Cheat_PowerUpActive(object->apiobj.field_0x27c)) {
        ConstantRumble(object, qrand() * 1.5259022e-05f * 0.5f, object->apiobj.field_0x289 * 0.3f);
    }
}

void MoveToMarker::BlowUp() {
}

void MoveToMarker::FadeOut() {
}

MoveToMarker::MoveToMarker(MechObjectInterface &) {
}

void MoveToMarker::Process(float) {
}

void MoveToMarker::Render() {
}

static __used__ i32 Jump_UpdateHint(HINT_s *) {
    return 0;
}
static __used__ bool Jump_PreventJump(GameObject_s *) {
    return false;
}
static __used__ void Jump_EndOfLandContext(GameObject_s *) {
}
static __used__ i32 Move_UpdateHint(HINT_s *) {
    return 0;
}
static __used__ i32 LastSafePosExtra(GameObject_s *) {
    return 0;
}

static __used__ bool Lever_UpdateHint(HINT_s *) {
    return false;
}
struct _vuv_s;
static __used__ void MakeWingFormation(_vuv_s *, _vuv_s *, f32, i32) {
}

static __used__ void AtatPart_Stop(PART_s *) {
}

static __used__ void AtatPart_Update(PART_s *) {
}

static __used__ void BigJump_EndOfLand(GameObject_s *) {
}

static __used__ void BigJump_JumpAction_Default(GameObject_s *) {
}

static __used__ void BigJump_LandAction_Default(GameObject_s *) {
}

static __used__ bool AutoJump_UpdateHint(HINT_s *) {
    return {};
}

static __used__ void VehicleStuff_UpdateHint(HINT_s *) {
}

i32 Slam_Start(GameObject_s *object, f32 speed) {
    if (LEGOCONTEXT_JUMP == -1) return 0;
    if ((object->apiobj.character_data->model_flags & 8) != 0) SetWeaponOut(object);
    object->character_context = LEGOCONTEXT_JUMP;
    object->action_movement_state = 4;
    object->context_animation_timer = 0.0f;
    object->context_animation = LEGOACT_SLAM;
    if ((object->apiobj.flags_low & 0x80) != 0) object->field_0xef9 |= 8;
    object->jump_sequence = 0;
    object->apiobj.velocity.y = speed;
    PlayJumpSfx(object, 4);
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
    object->jump_flags |= 1;
    if ((object->apiobj.flags_low & 0x80) != 0) Hint_SetComplete(0x60a);
    return 1;
}

void StartLunge(GameObject_s *object, f32 speed, f32 height) {
    object->context_animation_timer = 0.0f;
    object->context_flags &= ~0x80;
    object->character_context = 0;
    object->action_movement_state = 3;
    object->context_animation = 0x1f;
    if ((object->apiobj.flags_low & 0x80) != 0) object->field_0xef9 |= 8;
    object->jump_flags |= 1;
    object->apiobj.velocity.y = speed;
    object->jump_sequence = 0;
    if (height > 0.0f) MakeJumpReachHeight(object, height, 0);
    PlayJumpSfx(object, 3);
    if ((object->apiobj.character_data->model_flags & 8) != 0) SetWeaponOut(object);
}

void StartSlide(GameObject_s *, i32) {
}

void CanObjSlide(GameObject_s *, i32) {
}

void CanStepBack(GameObject_s *) {
}

void FlattenCode(GameObject_s *) {
}

void Glide_Start(GameObject_s *) {
}

void JetPackCode(GameObject_s *, i32, i32, i32) {
}

float SeekLinearF(float current, float target, float step) {
    if (current > target) {
        return MAX(target, current - step);
    }
    if (current < target) {
        return MIN(target, current + step);
    }
    return current;
}

void StartLaunch(GameObject_s *) {
}

void ApplyGravity(GameObject_s *object, float *gravity, float terminal_velocity, float gravity_scale,
                  float *vertical_velocity) {
    if (object == NULL || object->move_override != NULL || (object->apiobj.field_0x1f8 & 0x20) != 0) {
        return;
    }

    f32 &velocity = vertical_velocity != NULL ? *vertical_velocity : object->apiobj.velocity.y;
    if ((object->apiobj.field_0x27d & APIOBJECT_TERRAIN_CONTACT_FLOOR) != 0 && vertical_velocity == NULL) {
        velocity = 0.0f;
        return;
    }

    GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
    if (gravity == NULL && game_character == NULL) {
        return;
    }
    const f32 acceleration = gravity != NULL ? *gravity : game_character->gravity;
    velocity += acceleration * FRAMETIME;
    if (terminal_velocity != 0.0f) {
        const f32 terminal_speed = NuFabs(terminal_velocity);
        velocity = MAX(-terminal_speed, MIN(velocity, terminal_speed));
    }

    (void)gravity_scale; // Used by the target's hovering/seek branches, not ordinary gravity.
}

void SetObjTarget(GameObject_s *, GameObject_s *) {
}

void SnapPosTaken(WORLDINFO_s *, pushblock_s *, nuvec_s *, i32) {
}

void StartFlatten(GameObject_s *, GameObject_s *) {
}

GAMEANTINODE_s *GameAntinode_RegisterAntiNodeUsingData(GAMEANTINODESYS_s *, NUVEC *, u16,
                                                     GAMEANTINODEDATA_s *, f32, i32);

void UpdateMidPos(GIZMOBLOWUP_s *blowup) {
    if ((blowup->draw_flags & 0x1000) != 0) {
        blowup->mid_position = blowup->position;
        if (blowup->anti_node != NULL) {
            blowup->state_flags &= ~1;
            return;
        }
    }
    nuhspecial_s *special = blowup->override_special;
    if (special == NULL || !NuSpecialExistsFn(special)) {
        special = &blowup->type->animated_special;
    }
    NUVEC minimum;
    NUVEC maximum;
    NuSpecialGetBounds(special, &minimum, &maximum);
    NUVEC corners[8] = {
        {minimum.x, minimum.y, minimum.z}, {maximum.x, minimum.y, minimum.z},
        {maximum.x, minimum.y, maximum.z}, {minimum.x, minimum.y, maximum.z},
        {minimum.x, maximum.y, minimum.z}, {maximum.x, maximum.y, minimum.z},
        {maximum.x, maximum.y, maximum.z}, {minimum.x, maximum.y, maximum.z},
    };
    NuVecMtxTransformVU0(&minimum, &minimum, &blowup->transform);
    NuVecMtxTransformVU0(&maximum, &maximum, &blowup->transform);
    for (i32 i = 0; i < 8; ++i) {
        NuVecMtxTransformVU0(&corners[i], &corners[i], &blowup->transform);
    }
    if ((blowup->draw_flags & 0x1000) == 0) {
        blowup->mid_position.x = (maximum.x - minimum.x) * 0.5f + minimum.x;
        blowup->mid_position.y = (maximum.y - minimum.y) * 0.5f + minimum.y;
        blowup->mid_position.z = (maximum.z - minimum.z) * 0.5f + minimum.z;
    }
    if (blowup->anti_node == NULL && (blowup->visibility_flags & 0x40) != 0) {
        blowup->anti_node = GameAntinode_RegisterAntiNodeUsingData(
            WORLD->game_antinode_sys, &blowup->mid_position, blowup->field_0xf4 + blowup->field_0xf2,
            &blowup->type->anti_node_data, 0.0f, blowup->draw_flags & 0x4000);
    }
    if ((blowup->draw_flags & 0x1000) == 0) {
        f32 x = maximum.x - blowup->mid_position.x;
        f32 y = maximum.y - blowup->mid_position.y;
        f32 z = maximum.z - blowup->mid_position.z;
        blowup->target_scale = NuFsqrt(x * x + y * y + z * z);
    }
    blowup->state_flags &= ~1;
}

void Hang_MoveCode(GameObject_s *) {
}

void HoldCode_Copy(GameObject_s *) {
}

void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay) {
    if (position == NULL) {
        object->head_target = NULL;
        object->head_target_priority = 0;
        object->head_target_timer = 0.0f;
        object->head_target_delay = 0.0f;
    } else {
        if (object->head_target == NULL || (position != object->head_target && object->head_target_priority <= priority)) {
            i32 random = qrand();
            object->head_target = position;
            object->head_target_priority = priority;
            f32 delay = maximum_delay * random * 1.5259022e-05f +
                        (1.0f - random * 1.5259022e-05f) * minimum_delay;
            object->head_target_delay = delay;
            object->head_target_timer = time + delay;
        }
        object->head_target_position = *object->head_target;
    }
}

void StartBackFlip(GameObject_s *) {
}

void ForcedBackCode(GameObject_s *) {
}

void Glide_MoveCode(GameObject_s *) {
}

i32 SetObjOnSurface(GameObject_s *object, i32 mode) {
    APIOBJECT &api = object->apiobj;
    if (api.field_0x218 == 2000000.0f || (WorldInfo_CurrentlyActive()->current_level->flags & LEVEL_IN_SPACE) != 0) {
        return 0;
    }

    const f32 lower_bound = object->character_bottom * api.field_0xa8;
    f32 position_y;
    if (mode == 2) {
        position_y = api.collision_min.y - lower_bound + 0.1f - 0.0999f;
        api.position.y = position_y;
        api.initial_position.y = position_y;
        api.collision_position.y = position_y;
    } else if (mode == 0) {
        position_y = api.field_0x218 - lower_bound + 0.1f - 0.0999f;
        api.position.y = position_y;
        api.initial_position.y = position_y;
        api.collision_position.y = position_y;
        api.velocity.y = -1.0f;
        return 1;
    } else {
        position_y = api.position.y;
    }

    if (position_y + lower_bound <= api.field_0x218 + 0.1f) {
        position_y = api.field_0x218 - lower_bound + 0.1f - 0.0999f;
        api.position.y = position_y;
        api.initial_position.y = position_y;
        api.collision_position.y = position_y;
        api.velocity.y = -1.0f;
        return 1;
    }

    api.velocity.y = -1.0f;
    return 0;
}

void TurnCodeCamSafe(GameObject_s *, numtx_s *) {
}

void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z) {
    switch (order) {
        case 0:
            if (x != 0)
                NuMtxRotateX(matrix, x);
            if (y != 0)
                NuMtxRotateY(matrix, y);
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            break;
        case 1:
            if (y != 0)
                NuMtxRotateY(matrix, y);
            if (x != 0)
                NuMtxRotateX(matrix, x);
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            break;
        case 2:
            if (y != 0)
                NuMtxRotateY(matrix, y);
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            if (x != 0)
                NuMtxRotateX(matrix, x);
            break;
        case 3:
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            if (x != 0)
                NuMtxRotateX(matrix, x);
            if (y != 0)
                NuMtxRotateY(matrix, y);
            break;
    }
}

void Hang_SetTargetMom(GameObject_s *) {
}

void Glide_SetTargetMom(GameObject_s *) {
}

void SetObjAsHeadTarget(GameObject_s *object, GameObject_s *target, signed char, float, float, float) {
    if (target != NULL && object != NULL && target->apiobj.character_data != NULL) {
        i32 joint = GetGameCharacterData(target)->head_locator;
        NUVEC *position = joint == -1 ? &target->apiobj.collision_position : reinterpret_cast<NUVEC *>(&target->joint_matrices[joint].m30);
        SetHeadTarget(object, position, 2, 1.0f, 0.0f, 0.0f);
    }
}

void Slide_SetTargetMom(GameObject_s *, u16, float) {
}

void StepBackFromTarget(GameObject_s *) {
}

void ApplyGravity_Network(GameObject_s *) {
}

void VehicleCollisionCode(GameObject_s *) {
}

void VehicleTurnOrLoopOffset(GameObject_s *) {
}

void WallShuffle_SetTargetMom(GameObject_s *, u16) {
}

void SetMoveAndAnimateFunctions(u32 model_flag_mask, u32 model_flag_value, u32 game_flag_mask, u32 game_flag_value,
                                i32 movement_type, void *move_function, void *animate_function, void *draw_function) {
    const CHARACTERUPDATEFN move = reinterpret_cast<CHARACTERUPDATEFN>(move_function);
    const CHARACTERUPDATEFN animate = reinterpret_cast<CHARACTERUPDATEFN>(animate_function);
    const CHARACTERUPDATEFN draw = reinterpret_cast<CHARACTERUPDATEFN>(draw_function);

    for (i32 character_index = 0; character_index < CHARCOUNT; ++character_index) {
        CHARACTERDATA &character = CDataList[character_index];
        GAMECHARACTERDATA &game_character = GCDataList[character_index];

        if ((character.model_flags & model_flag_mask) != model_flag_value ||
            (game_character.flags_090 & game_flag_mask) != game_flag_value ||
            (movement_type != -1 && static_cast<i8>(game_character.field275_0x116) != movement_type)) {
            continue;
        }

        if (move != NULL) {
            character.move_fn = move;
        }
        if (animate != NULL) {
            character.animate_fn = animate;
        }
        if (draw != NULL) {
            character.draw_fn = draw;
        }
    }
}

u16 SeekRot(u16 current, u16 target, f32 rate) {
    const f32 blend = MIN(rate * FRAMETIME, 1.0f);
    i32 difference = static_cast<i32>(target) - static_cast<i32>(current);
    if (difference > 0x8000) {
        difference -= 0x10000;
    } else if (difference < -0x8000) {
        difference += 0x10000;
    }
    return static_cast<u16>(static_cast<f32>(current) + static_cast<f32>(difference) * blend);
}

void SeekVec(NUVEC *result, NUVEC *current, NUVEC *target, f32 rate) {
    const f32 blend = MIN(rate * FRAMETIME, 1.0f);
    result->x = current->x + (target->x - current->x) * blend;
    result->y = current->y + (target->y - current->y) * blend;
    result->z = current->z + (target->z - current->z) * blend;
}

u16 TurnRot(u16 current, u16 target, i32 speed, i32 *difference_out) {
    if (target == current) {
        return target;
    }

    const i32 step = static_cast<i32>(static_cast<f32>(speed) * FRAMETIME);
    const i32 difference = RotDiff(current, target);
    if (difference_out != NULL) {
        *difference_out = difference;
    }
    if (difference > step) {
        return static_cast<u16>(current + step);
    }
    if (difference < -step) {
        return static_cast<u16>(current - step);
    }
    return target;
}

void HoldCode(GameObject_s *object) {
    if (LEGOCONTEXT_HOLD != -1 && object->character_context == LEGOCONTEXT_HOLD) {
        if (object->context_animation_timer > 0.0f) object->context_animation_timer -= FRAMETIME;
        else if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) == 0) object->character_context = -1;
    }
}

f32 SeekValF(f32 current, f32 target, f32 rate) {
    const f32 blend = MIN(rate * FRAMETIME, 1.0f);
    return current + (target - current) * blend;
}

void TurnCode(GameObject_s *, i32, GAMEPAD_s *) {
}

void FloatCode(GameObject_s *) {
}

void SlideCode(GameObject_s *) {
}

void StartHold(GameObject_s *object) {
    if (CanStartHoldFn == NULL) return;
    if (CanStartHoldFn(object) == 2) {
        object->hold_timer = 0.0f;
        object->character_context = -1;
        return;
    }
    if (LEGOCONTEXT_HOLD != -1 && NewBlockAction(object) != 0) {
        object->context_animation_timer = 0.3f;
        object->character_context = LEGOCONTEXT_HOLD;
        PlaySabreSfx(NULL, object, NULL, 0);
        return;
    }
    object->character_context = -1;
}

void StartTurn(GameObject_s *) {
}
