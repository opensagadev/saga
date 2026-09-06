#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
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
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;

float SLAMGRAVITY = -15.0f;
static float applygravity_extrahoveroffset;

void MovePlayer_DIRECTIONAL(GameObject_s *object);
void MovePlayer_VEHICLEDIRECTIONAL(GameObject_s *object);
i32 MovePlayer_TWIST(GameObject_s *object);
i32 MovePlayer_CIRCLE(GameObject_s *object);
i32 MovePlayer_GUNSHIPIN(GameObject_s *object);
i32 MovePlayer_POD(GameObject_s *object);
void ApplyGravity(GameObject_s *object, float *gravity, float hover_height, float seek_rate, float *ground_height);
float VehicleTurnOrLoopOffset(GameObject_s *object);
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
i32 ThermalDetonator_MoveCode(GameObject_s *object);
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
void UpdateLastSafePosition(GameObject_s *object);
extern "C" TERRAIN_SURFACE_s TerSurface[32];
i32 NoLayerKill(GameObject_s *object);
void Punch_Hit(GameObject_s *, GameObject_s *, f32, f32);
void StartQuickShoot(GameObject_s *, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, i16, i16);
i32 (*FindSlamOrigin_UseCPosFn)(GameObject_s *) = NULL;
void (*Jump_EndOfLandContextFn)(GameObject_s *) = NULL;
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
i32 ZapTarget(GameObject_s *);
i32 CannotKill(GameObject_s *);
i32 FaceOpponent(GameObject_s *object, NUVEC *position);
void SetProtocolDroidDeactivatedAction(GameObject_s *);
extern i16 id_JAWA;
extern i16 id_GONKDROID;
void NewBuzz(nupad_s *, f32, i32);
void Arcade_AIKilled(i32);
i32 ObjHitObj(GameObject_s *, GameObject_s *, i32, u16, i32, i32);
i16 *objhitobj_killparts_yrot;
i32 objhitobj_noimpactsfx;
void Player_ClearContext(GameObject_s *, i32);
void Player_ResetContexts(PLAYERPACKET_s *);
void PlayDieSfx(GameObject_s *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
static void CommunicateCode(GameObject_s *, i32, i32);
static void PunchCode(GameObject_s *, i32, i32, i32, i32, f32);
static void ShootCode(GameObject_s *, i32, i32, i32, i32, i32);
static void DodgeCode(GameObject_s *, i32, i32);
void Grapple_MoveCode(GameObject_s *);
void SuperCarry_MoveCode(WORLDINFO_s *, GameObject_s *);
void SpecialMove_VictimCode(GameObject_s *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
i32 SuperCarry_Carrying(GameObject_s *);
void Torpedo_UpdateJobbies(GameObject_s *);
void Tag_Check(GameObject_s *);
void TorpedoCode(GameObject_s *, i32, f32);
void PeriscodeCode(GameObject_s *);
i32 PodLevel(AREADATA_s *);
void KeepOnScreen(GameObject_s *);
void CreateSnakeBody(GameObject_s *, i32);
void UpdateSnakeBody(GameObject_s *);
void Teleport_NetMoveCode(GameObject_s *);
void TractorBeamCode(GameObject_s *);
void AddSurfaceRipples(GameObject_s *);
extern i16 id_SNAKE;
extern i16 id_ATAT;
void Attracto_MoveCode(WORLDINFO_s *, GameObject_s *);
void SecurityDoor_MoveCode(WORLDINFO_s *, GameObject_s *);
void Batarang_MoveCode(GameObject_s *);
void FireBountyHunterRocket(GameObject_s *);
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

void Move_BEAST(GameObject_s *object);

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

void Move_CHARACTER(GameObject_s *object);

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

void MovePlayer_ROLLING(GameObject_s *) {
}

void MoveSplinePosition(SPLINEPOS_s *, float) {
}

void MoveBlocksOverBlock(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *) {
}

void MoveInactiveVehicle(GameObject_s *object, i32, GameObject_s **followed_object) {
    object->target_velocity.x = object->target_velocity.y = object->target_velocity.z = 0.0f;
    GameObject_s *other;
    if (object == Player[0] || object == Player[1]) {
        other = object == Player[0] ? Player[1] : Player[0];
        NUVEC position = other->apiobj.position;
        SeekVec(&object->apiobj.position, &object->apiobj.position, &position, 10.0f);
        object->apiobj.velocity = object->target_velocity;
        object->apiobj.facing_angle = object->apiobj.movement_facing_angle = object->apiobj.field_0x276 =
            other->apiobj.field_0x276;
    } else {
        NUVEC position = v000;
        float count = 0.0f;
        if (Player[0] != NULL) {
            NuVecAdd(&position, &position, &Player[0]->apiobj.position);
            count += 1.0f;
        }
        if (Player[1] != NULL) {
            NuVecAdd(&position, &position, &Player[1]->apiobj.position);
            count += 1.0f;
        }
        if (count > 0.0f) {
            NuVecScale(&position, &position, 1.0f / count);
        }
        SeekVec(&object->apiobj.position, &object->apiobj.position, &position, 10.0f);
        object->apiobj.velocity = object->target_velocity;
        other = NULL;
    }
    if (followed_object != NULL) {
        *followed_object = other;
    }
    object->delayed_turn_timer = 0.0f;
    object->field_0xddc = 0.0f;
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
                (CInfo[target->character_context].flags & 0x8000) != 0 || target == object->field_0xcc0)
                continue;
            NUVEC delta;
            f32 distance = NuVecDistSqr(&target->apiobj.position, &object->apiobj.position, &delta);
            if (distance >= range * range ||
                (object->pad_gamepad->input_magnitude > 0.0f && forward.x * delta.x + forward.z * delta.z < 0.0f))
                continue;
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
                if (target != object && (target->apiobj.field_0x1f8 & 0x1000) != 0 && target->apiobj.field_0x287 == 0 &&
                    target->character_context == 5 && (target->context_flags & 0xc) == 0 &&
                    target->force_target == object) {
                    melee = target;
                    break;
                }
            }
        }
        object->incoming_melee = melee;
        GameObject_s *special = NULL;
        if (((object->apiobj.character_data->model_flags & 8) != 0 || object->id == id_BODYGUARD ||
             object->id == id_IMPERIALGUARD) &&
            has_block) {
            for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
                GameObject_s *target = &Obj[i];
                if (target != object && (target->apiobj.field_0x1f8 & 0x1000) != 0 && target->apiobj.field_0x287 == 0 &&
                    target->character_context == 0x26 && target->force_target == object &&
                    (target->id != id_GAMORREANGUARD || target->context_animation != 0x56 ||
                     (target->context_flags & 0x40) == 0)) {
                    special = target;
                    break;
                }
            }
        }
        object->incoming_special = special;
        if (object->incoming_bolt == NULL && melee == NULL) {
            if (object->apiobj.field_0x27c == -1 || !has_block)
                object->incoming_part = NULL;
            else {
                f32 radius = object->apiobj.field_0x1e0 > object->apiobj.field_0x1dc ? object->apiobj.field_0x1e0
                                                                                     : object->apiobj.field_0x1dc;
                object->incoming_part = FindIncomingPart(object, &object->apiobj.collision_position, radius, 10, 0.0f);
            }
        }
        if ((object->field_0xe22 & 8) == 0 &&
            (object->incoming_bolt != NULL || object->incoming_melee != NULL || object->incoming_part != NULL) &&
            object->character_context == -1)
            object->field_0xe22 |= 8;
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
                if ((object->apiobj.flags_low & 0x80) != 0)
                    object->field_0xef9 |= 8;
                return;
            }
            if (object->block_latch == 0 && object->blocked_attack_stage != 0) {
                if ((attacker->apiobj.field_0x1f8 & 0x1000) != 0 && attacker->apiobj.field_0x287 == 0) {
                    if ((object->field_0xe23 & 0x20) == 0) {
                        if (attacker->character_context == 5 && attacker->combo_stage < object->blocked_attack_stage)
                            return;
                    } else if (attacker->character_context == 0x26 &&
                               attacker->context_animation == object->blocked_attack_stage &&
                               (attacker->id != id_GAMORREANGUARD || attacker->context_animation != 0x56 ||
                                (attacker->context_flags & 0x40) == 0))
                        return;
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
            if (object->context_animation_timer > 0.0f)
                return;
            if (held != 0 &&
                (attacker == NULL || attacker->id != id_GAMORREANGUARD || attacker->character_context != 0x26 ||
                 attacker->context_animation != 0x56 || (attacker->context_flags & 0x40) == 0)) {
                StartHold(object);
                return;
            }
            object->character_context = -1;
            return;
        }
        if (!TouchHacks::ShouldBlock(*object))
            return;
    } else {
        if (object->block_cooldown > 0.0f) {
            object->block_cooldown -= FRAMETIME;
            return;
        }
        if (!allow_start || !pressed || object->apiobj.field_0x27d == 0 ||
            (CInfo[object->character_context].flags & 0x20) != 0 || (object->field_0xef8 & 2) == 0 ||
            (object->incoming_bolt == NULL && object->incoming_melee == NULL && object->incoming_special == NULL &&
             object->incoming_part == NULL))
            return;
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
        if ((object->apiobj.flags_low & 0x80) != 0)
            Hint_SetComplete(0x5dd);
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
                if (hit_frame >= 1.0f && hit_frame <= *frame)
                    ComboHitFrame(object, 1);
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
                    } else if (held)
                        StartHold(object);
                }
            }
        }
    } else if ((object->field_0xe22 & 1) != 0 && object->weapon_scale_state == 0 && pressed &&
               object->apiobj.field_0x27d != 0 && object->apiobj.character_model != NULL &&
               (context == 1 || context == -1 || context == 2 || context == 4 || context == 3) &&
               SetComboOpponent(object, 0.6f, 0, 2) == 1) {
        NUVEC *position;
        if (object->force_target != NULL)
            position = &object->force_target->apiobj.position;
        else if (object->blowup_target != NULL)
            position = &object->blowup_target->mid_position;
        else
            return;
        NUVEC delta;
        NuVecSub(&delta, position, &object->apiobj.position);
        NuVecRotateY(&delta, &delta, -object->apiobj.movement_facing_angle);
        if ((object->pad_gamepad->input_magnitude <= 0.0f || fabsf(delta.x) <= fabsf(delta.z)) && delta.z < 0.0f &&
            ComboOpponent_Range2 < 0.48999998f) {
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

static void LightSabreComboCode(GameObject_s *object, i32 action_pressed, i32 action_held, i32 jump_pressed,
                                i32 buttons_pressed) {
    ANIMPACKET_s &animation = object->apiobj.anim_packet;
    if ((object->apiobj.flags_low & 0x80) != 0)
        object->ai_combo_cooldown = 0.0f;
    else if (object->ai_combo_cooldown > 0.0f)
        object->ai_combo_cooldown -= FRAMETIME;
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
                    if (action_held != 0)
                        StartHold(object);
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
                    if ((object->apiobj.flags_low & 0x80) != 0)
                        object->field_0xef9 |= 8;
                }
            } else {
                SetComboOpponent(object, 1.0f, 1, 0);
                object->context_flags &= ~4;
                object->combo_stage = object->combo_stage == 1 ? 2 : 1;
                if (object->id != id_IMPERIALGUARD)
                    PlaySabreSfx(NULL, object, NULL, 1);
                if (object->combo_stage == 2)
                    NewRumble(object->pad_gamepad->pad, 0.35f, 0);
                if (object->combo_stage == 1) {
                    object->combo_branch =
                        (object->field_0xe20 & 0x40) != 0 &&
                                JediHasAction(object, static_cast<JEDI_ACTION>(object->queued_context_animation + 2))
                            ? 2
                            : 1;
                    if (object->combo_branch == 1)
                        object->field_0xe20 &= ~0x40;
                    NewRumble(object->pad_gamepad->pad, object->combo_branch == 2 ? 0.5f : 0.35f, 0);
                    if (object->combo_branch == 2 && object->id != id_IMPERIALGUARD) {
                        PlaySabreSfx("ComboSaber2", object, &object->apiobj.collision_position, 0);
                    }
                } else if (object->combo_branch == 1) {
                    object->combo_branch =
                        (object->field_0xe20 & 0x40) != 0 &&
                                JediHasAction(object, static_cast<JEDI_ACTION>(object->queued_context_animation + 4))
                            ? 4
                            : 3;
                    if (object->combo_branch == 3)
                        object->field_0xe20 &= ~0x40;
                } else {
                    object->combo_branch =
                        (object->field_0xe20 & 0x40) != 0 &&
                                JediHasAction(object, static_cast<JEDI_ACTION>(object->queued_context_animation + 6))
                            ? 6
                            : 5;
                    if (object->combo_branch == 5)
                        object->field_0xe20 &= ~0x40;
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
                if (frames[1] > 0.0f && animation.current_time >= frames[1])
                    object->context_flags |= 8;
                CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(
                    object->apiobj.character_model->model_data_a[object->context_animation]);
                if ((animation.flags & ANIMPACKET_FLAG_FINISHED) != 0 ||
                    (info->blend_out_time > 0.0f &&
                     AnimDuration(object->id, object->context_animation, animation.current_time, 0.0f, 0) <
                         info->blend_out_time)) {
                    object->character_context = CHARACTER_CONTEXT_NONE;
                    object->context_flags |= 2;
                }
                object->sabre_flags |= 2;
                if ((object->context_flags & 0x40) == 0 &&
                    (((object->context_flags & 8) != 0 && frames[0] == 0.0f) || (object->context_flags & 2) != 0)) {
                    ComboHitFrame(object, object->combo_branch == 6 ? 3 : 1);
                }
                if (object->character_context == CHARACTER_CONTEXT_NONE && action_held != 0 &&
                    (object->id != id_IMPERIALGUARD || object->combo_stage != 2))
                    StartHold(object);
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
                if ((object->context_flags & 0x40) == 0)
                    ComboHitFrame(object, 1);
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
            if (frame > 0.0f && *time >= frame)
                object->context_flags |= 0x10;
            can_queue = AnimDuration(object->id, object->context_animation, *time, 0.0f, 0) < 0.1f;
            if (can_queue)
                object->field_0xe22 |= 8;
        }
        if (action_pressed != 0 && object->combo_input_latched == 0) {
            if (can_queue)
                object->combo_input_latched = 1;
            else
                object->field_0xe20 &= ~0x40;
        }
        object->sabre_flags |= 3;
        goto combo_finished;
    }

    {
        const i8 context = object->character_context;
        const bool player_active = (object->apiobj.flags_low & 0x80) != 0;
        if (action_pressed == 0 || (!player_active && object->ai_combo_cooldown > 0.0f) || context == 0x15 ||
            context == 0x5a || context == 0x13 || context == 0x2e ||
            (!player_active && static_cast<u8>(context - 6) < 2) || context == 8 || context == 0x1b ||
            context == 0x2d || context == 0xf || context == 0x27 || context == 0x28 || context == 0x59 ||
            context == 0x30 || context == 0x3d || context == 0x43 || context == 0x4a || context == 0x61 ||
            context == 0x4b) {
            if (((object->pad_gamepad->allocated_5a & 4) != 0 || buttons_pressed != 0) &&
                (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 &&
                object->weapon_scale_state == WEAPON_SCALE_IDLE && context != 6 && context != 7 && context != 18 &&
                context != 27 && context != 29 && context != 13 && context != 14 && context != 46 &&
                !(context == 0 && static_cast<u8>(object->action_movement_state - 3) < 2) &&
                (object->field_0xe23 & 1) == 0 && (CInfo[context].flags & 0x4000000) == 0 &&
                ((CInfo[context].flags & 0x8000000) == 0 || (object->jump_flags & 2) == 0) && context != 8) {
                GAMECHARACTERDATA *data = GetGameCharacterData(object);
                const i32 action =
                    data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 126 : 16;
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
            const i32 draw_action =
                data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 127 : 17;
            if (object->weapon_scale_state == 0 && object->apiobj.field_0x27d != 0 &&
                object->pad_gamepad->input_magnitude == 0.0f &&
                object->apiobj.character_model->model_data_b[draw_action] != NULL && context >= -1 && context <= 4) {
                SlowWeaponOut(object);
            } else {
                FastWeaponOut(object, 1);
            }
            goto combo_finished;
        }
        if ((object->field_0xe20 & 4) != 0 || (!player_active && object->weapon_scale_state != 0) ||
            object->apiobj.field_0x27d == 0 ||
            (context != 1 && context != -1 && context != 2 && context != 4 && context != 3 && context != 6 &&
             context != 7))
            goto combo_finished;
        SetWeaponOut(object);
        if (object->incoming_bolt != NULL || object->incoming_melee != NULL || object->incoming_special != NULL ||
            object->incoming_part != NULL) {
            if (!TouchHacks::ShouldBlock(*object))
                goto combo_finished;
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
                if ((object->apiobj.flags_low & 0x80) != 0)
                    Hint_SetComplete(0x5dd);
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
        if (!player_active &&
            ((object->field_0xf04 & 2) != 0 || (object->apiobj.character_data->model_flags & 4) == 0) &&
            (object->field_0xef9 & 4) == 0)
            goto combo_finished;
        SetComboOpponent(object, 1.0f, 1, 0);
        if ((object->apiobj.flags_low & 0x80) != 0 && object->force_target != NULL &&
            object->force_target->apiobj.field_0x27c == -1)
            Hint_SetComplete(0x277);
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
        if (object->id != id_IMPERIALGUARD)
            PlaySabreSfx(NULL, object, NULL, 1);

        if (!JediHasAction(object, action)) {
            object->character_context = CHARACTER_CONTEXT_NONE;
        }
    }
combo_finished:
    if (object->weapon_scale == 1.0f && (object->field_0xe22 & 8) == 0 && object->character_context == 5 &&
        (object->context_flags & 4) != 0)
        object->field_0xe22 |= 8;
}

static void JediLightCode(GameObject_s *object) {
    if (object->dynamic_light_id == -1)
        return;
    NUVEC colour;
    NUVEC position;
    if (object->character_context == 0x1b && (object->field_0xe21 & 5) == 1 && object->force_target != NULL) {
        rtlDynamicEnable(object->dynamic_light_id, 1);
        NUVEC delta;
        f32 distance =
            NuVecDist(&object->force_target->apiobj.collision_position, &object->apiobj.collision_position, &delta);
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
    if (object->weapon_scale <= 0.0f || object->sabre_flags == 0)
        return;
    position = v000;
    if (object->apiobj.field_0x288 == 0)
        return;
    i32 point_count = 0;
    for (i32 blade = 0; blade < 4; ++blade) {
        if (object->apiobj.field_0x288 == 0)
            break;
        if ((object->field_0xe23 & 8) == 0)
            continue;
        GAMECHARACTERDATA *data = GetGameCharacterData(object);
        i32 first = data->streak_joints[blade][0];
        i32 second = data->streak_joints[blade][1];
        if (first == -1 || object->apiobj.character_model->points_of_interest[first] == NULL || second == -1 ||
            object->apiobj.character_model->points_of_interest[second] == NULL)
            continue;
        NuVecAdd(&position, &position, reinterpret_cast<NUVEC *>(&object->joint_matrices[first].m30));
        NuVecAdd(&position, &position, reinterpret_cast<NUVEC *>(&object->joint_matrices[second].m30));
        point_count += 2;
    }
    if (point_count == 0)
        return;
    NuVecScale(&position, &position, 1.0f / point_count);
    rtlDynamicEnable(object->dynamic_light_id, 1);
    f32 radius = object->sabre_collision_radius * 3.0f * object->weapon_scale * object->apiobj.field_0xa8;
    rtlDynamicSetRadii(object->dynamic_light_id, radius + 0.25f, radius + 0.5f);
    f32 brightness = qrand() < 0x8000 ? 0.003921569f : 0.007843138f;
    i32 blade = -1;
    if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(0x19))
        blade = 0;
    else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
        blade = 3;
    else if (object->id == id_GRIEVOUS) {
        f32 phase =
            (NU_SIN_LUT(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 2.0f) * 0.5f * 65536.0f)) + 1.0f) * 0.5f;
        colour.x = ((BladeTab[2].colour[0] - static_cast<f32>(BladeTab[1].colour[0])) * phase + BladeTab[1].colour[0]) *
                   brightness;
        colour.y = ((BladeTab[2].colour[1] - static_cast<f32>(BladeTab[1].colour[1])) * phase + BladeTab[1].colour[1]) *
                   brightness;
        colour.z = ((BladeTab[2].colour[2] - static_cast<f32>(BladeTab[1].colour[2])) * phase + BladeTab[1].colour[2]) *
                   brightness;
    } else
        blade = static_cast<i8>(GetGameCharacterData(object)->field_0x117);
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
    if (object->field_0xd80 == 0.0f &&
        (object->force_glow_previous == NULL || object->force_glow_previous == object->force_glow_candidate)) {
        object->force_glow_object = object->force_glow_candidate;
        object->force_glow_kind = object->force_glow_candidate_kind;
    }
    f32 target;
    if (object->force_glow_object == NULL || object->force_glow_candidate != object->force_glow_object) {
        target = FORCEGLOWTIME;
        if (object->field_0xd80 == target && object->force_hold_time > 0.0f) {
            object->force_hold_time -= FRAMETIME;
        } else
            target = 0.0f;
    } else if ((object->apiobj.flags_low & 0x80) == 0 && object->force_glow_kind == 0 &&
               object->character_context != 8 && AnimPlaying(&object->apiobj.anim_packet, 0xb, 1, 1) == NULL &&
               AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL)
        target = 0.0f;
    else
        target = FORCEGLOWTIME;
    f32 previous = object->field_0xd80;
    if (object->force_glow_target == previous)
        object->force_glow_target = target;
    else {
        if (previous > object->force_glow_target) {
            object->field_0xd80 = previous - FRAMETIME * 0.5f;
            if (object->field_0xd80 < 0.0f)
                object->field_0xd80 = 0.0f;
        } else {
            object->field_0xd80 = previous + FRAMETIME;
            if (object->field_0xd80 > object->force_glow_target)
                object->field_0xd80 = object->force_glow_target;
        }
        if (object->field_0xd80 == FORCEGLOWTIME && previous != FORCEGLOWTIME)
            object->force_hold_time = 0.333f;
    }
    if (object->field_0xd80 > 0.0f && object->force_glow_object == NULL)
        ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    if (!(object->field_0xd80 > 0.0f) || object->force_glow_object == NULL)
        return;
    switch (object->force_glow_kind) {
        case 0: {
            GIZFORCE_s *force = static_cast<GIZFORCE_s *>(object->force_glow_object);
            if ((force->progress_flags & 1) == 0) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            f32 radius = force->radius;
            object->field_0xd8c = TouchHacks::TouchControlsActive && force->strength_0x6c > 0.0f
                                      ? force->strength_0x6c
                                      : force->horizontal_range * radius;
            object->field_0xe1e = model;
            object->force_glow_position = force->position;
            force->field_0xaa |= 2;
            break;
        }
        case 1: {
            GAMEANIMOBJ_s *animation = static_cast<GAMEANIMOBJ_s *>(object->force_glow_object);
            f32 scale = object->field_0xd8c;
            if (!NuSpecialExistsFn(&animation->special)) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            NuSpecialGetRadius(&animation->special, &object->force_glow_position, &object->field_0xd8c);
            NuVecMtxTransformVU0(&object->force_glow_position, &object->force_glow_position,
                                 NuSpecialGetDrawMtx(&animation->special));
            object->field_0xe1e = model;
            object->field_0xd8c *= scale;
            GIZFORCEANIMDATA_s *data = static_cast<GIZFORCEANIMDATA_s *>(animation->object_data);
            if (data != NULL)
                data->force_glow_active = 1;
            break;
        }
        case 2: {
            GameObject_s *target_object = static_cast<GameObject_s *>(object->force_glow_object);
            if ((target_object->apiobj.field_0x1f8 & 0x1001) == 0 ||
                (TouchHacks::TouchControlsActive && target_object->apiobj.field_0x27c != -1)) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            f32 radius = NuFmax(target_object->apiobj.field_0x1dc, target_object->apiobj.field_0x1e0) * 1.75f;
            object->field_0xe1e = model;
            object->force_glow_position.x = target_object->apiobj.collision_position.x;
            f32 bottom = target_object->character_bottom;
            f32 scale = target_object->apiobj.field_0xa8;
            object->force_glow_position.y = bottom * scale + target_object->apiobj.position.y +
                                            (target_object->character_top - bottom) * 0.5f * scale;
            object->force_glow_position.z = target_object->apiobj.collision_position.z;
            object->field_0xd8c = radius;
            break;
        }
        case 3: {
            PART_s *part = static_cast<PART_s *>(object->force_glow_object);
            if ((part->active & 1) == 0) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            f32 radius;
            if (NuSpecialExistsFn(&part->special)) {
                NUVEC center;
                NuSpecialGetRadius(&part->special, &center, &object->field_0xd8c);
                radius = object->field_0xd8c + object->field_0xd8c;
            } else
                radius = part->radius + part->radius;
            object->field_0xe1e = model;
            object->force_glow_position = part->position;
            object->field_0xd8c = radius;
            break;
        }
    }
    object->field_0xd8c *= 1.125f;
}

static void ForceCode(GameObject_s *object, i32 pressed, i32 held, i32) {
    u8 previous_force = object->force_repeat_requested;
    object->force_repeat_requested = 0;
    if (object->character_context == 8) {
        if ((!held && (object->pad_gamepad->allocated_5a & 4) == 0) || object->gizforce_target == NULL) {
            object->character_context = -1;
        } else {
            if ((dagobah_training == 0 || object->field_0xcc0 != NULL || FreePlay != 0) &&
                !GizForce_Complete(object->gizforce_target) &&
                !GizForce_StoodOnForce(object->gizforce_target, object)) {
                AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                if ((WORLD->current_level == VADERC_LDATA || (object->apiobj.flags_low & 0x80) == 0 ||
                     object->apiobj.model_draw_result == 0 || !(object->field_0xc54 > 0.0f) ||
                     !(object->head_target_facing < -0.85f || object->head_target_facing > 0.85f)) &&
                    !(WORLD->current_level == DAGOBAHC_LDATA && object->id == id_LUKESKYWALKERDAGOBAH &&
                      LevGizmo[0] != NULL && object->gizforce_target == LevGizmo[0]->object &&
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
                        if (force != NULL)
                            SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                    } else {
                        object->force_glow_candidate = object->gizforce_target_object;
                        object->force_glow_candidate_kind = 1;
                        object->field_0xd8c = force == NULL ? 1.0f : force->horizontal_range;
                        SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f,
                                      5.0f, 10.0f);
                    }
                    if (force != NULL)
                        SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                } else {
                    object->character_context = -1;
                    object->gizforce_target = NULL;
                }
            } else {
                AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                object->character_context = -1;
                object->gizforce_target = NULL;
            }
        }
        if (object->character_context == -1)
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        return;
    }
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[1] & 0x80) != 0 ||
        (object->field_0xe23 & 1) != 0 || object->character_context == 0x12)
        return;
    bool start = false;
    if (object->apiobj.field_0x27d != 0 &&
        (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0)) {
        if (((object->apiobj.flags_low & 0x80) != 0 && pressed) || (object->pad_gamepad->allocated_5a & 4) != 0 ||
            (held && previous_force))
            start = true;
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
                if ((object->gizforce_target->config_flags & 0x10) != 0)
                    Hint_SetComplete(0x623);
                LSW_HintConditions |= 1;
            }
        }
    } else if ((object->apiobj.flags_low & 0x80) != 0)
        GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
    GIZFORCE_s *force = object->gizforce_target;
    if (force != NULL && (object->apiobj.flags_low & 0x80) != 0 && (object->field_0xe22 & 2) == 0 &&
        object->apiobj.field_0x27d != 0) {
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
    if (object->character_context == 8 && object->id == id_LUKESKYWALKERDAGOBAH && dagobah_training != 0 &&
        object->field_0xcc0 == NULL && FreePlay == 0) {
        EndForce(object, 0);
        if (AnimPlaying(&object->apiobj.anim_packet, 0xb, 1, 1) == NULL &&
            AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL) {
            PlaySfx("JForcePush", &object->apiobj.collision_position);
            PlayGruntSfx(object);
        }
        NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        if (object->gizforce_target != NULL) {
            object->gizforce_target->using_object = NULL;
            object->apiobj.movement_facing_angle =
                GizForces_AngleToForce(&object->apiobj.collision_position, object->gizforce_target);
        }
        object->field_0xe23 |= 1;
        GameCam_HitRoll();
    }
}

i32 FaceOpponent(GameObject_s *object, NUVEC *position) {
    if (position == NULL) {
        GameObject_s *target = object->force_target;
        if (target != NULL) {
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001) {
                return 0;
            }
            position = &target->apiobj.position;
            if (target->apiobj.field_0x287 != 0) {
                return 0;
            }
        } else {
            if (object->blowup_target == NULL) {
                return 0;
            }
            position = &object->blowup_target->mid_position;
        }
    }
    object->apiobj.movement_facing_angle =
        NuAtan2D(position->x - object->apiobj.position.x, position->z - object->apiobj.position.z);
    return 1;
}

i32 ForcePushed_SetTargetMom(GameObject_s *object, float *seek_rate) {
    GameObject_s *source = object->force_target;
    if (source == NULL) {
        object->target_velocity.x = object->target_velocity.z = 0.0f;
        return 0;
    }
    if ((source->action_flags & 0x180) != 0) {
        object->target_velocity.x = object->target_velocity.z = 0.0f;
        if (source->character_context == 0x1b && source->field_0x7a3 == 1) {
            NUVEC target = {source->external_force.x,
                            source->external_force.y - object->character_bottom * object->apiobj.field_0xa8,
                            source->external_force.z};
            NUVEC direction;
            float distance_squared = NuVecDistSqr(&target, &object->apiobj.position, &direction);
            if (!(0.1f * 0.1f > distance_squared) &&
                (!(0.2f * 0.2f > distance_squared) || (NuFsqrt(distance_squared) - 0.1f) / 0.1f > 0.0f)) {
                NuVecNorm(&direction, &direction);
                NuVecScale(&object->target_velocity, &direction, 1.0f);
            }
            *seek_rate = 20.0f;
            return 1;
        }
        return 0;
    }
    if (object->action_movement_state == 4 && Cheat_IsOn(0x13) != 0) {
        u16 angle = NuAtan2D(object->apiobj.pos_x - source->apiobj.pos_x, object->apiobj.pos_z - source->apiobj.pos_z);
        object->target_velocity.x = -NU_SIN_LUT(angle) * 2.5f;
        object->target_velocity.z = -NU_COS_LUT(angle) * 2.5f;
        return 0;
    }
    GameObject_s *nearest = NULL;
    if (ForcePush_SuperPush != 0 && static_cast<i8>(object->apiobj.flags_low) >= 0 &&
        (source->action_flags & 0x380) == 0) {
        float push_x = object->apiobj.position.x - source->apiobj.position.x;
        float push_z = object->apiobj.position.z - source->apiobj.position.z;
        float nearest_distance_squared = 2.25f;
        GameObject_s *candidate = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 || candidate->apiobj.field_0x287 != 0 ||
                candidate == source || candidate == object || candidate->apiobj.field_0x27c != -1 ||
                (candidate->field_0xefb & 8) != 0 || CannotKill(candidate) != 0 ||
                (candidate->apiobj.character_data->model_flags & 0x4002010) != 0x10) {
                continue;
            }
            GAMECHARACTERDATA *character =
                static_cast<GAMECHARACTERDATA *>(candidate->apiobj.character_data->field11_0x24);
            if ((character->flags_090 & 0x40) != 0 || (character->flags_094[1] & 2) != 0 ||
                candidate->id == id_GONKDROID || candidate->apiobj.collision_min.y > object->apiobj.collision_max.y ||
                object->apiobj.collision_min.y > candidate->apiobj.collision_max.y) {
                continue;
            }
            float dx = candidate->apiobj.position.x - object->apiobj.position.x;
            float dz = candidate->apiobj.position.z - object->apiobj.position.z;
            if (!(0.0f > push_x * dx + push_z * dz)) {
                float distance_squared = dx * dx + dz * dz;
                if (distance_squared < nearest_distance_squared) {
                    nearest_distance_squared = distance_squared;
                    nearest = candidate;
                }
            }
        }
    }
    float dx;
    float dz;
    if (nearest != NULL) {
        dz = nearest->apiobj.pos_z - object->apiobj.pos_z;
        dx = nearest->apiobj.pos_x - object->apiobj.pos_x;
    } else {
        dz = object->apiobj.pos_z - source->apiobj.pos_z;
        dx = object->apiobj.pos_x - source->apiobj.pos_x;
    }
    u16 angle = NuAtan2D(dx, dz);
    object->target_velocity.x = NU_SIN_LUT(angle) * 2.5f;
    object->target_velocity.z = NU_COS_LUT(angle) * 2.5f;
    return 0;
}

i32 ForcePushed_YRotation(GameObject_s *object) {
    GameObject_s *source = object->force_target;
    if (source != NULL && source->character_context == 0x1b && (source->action_flags & 0x380) == 0 &&
        (object->apiobj.velocity.x != 0.0f || object->apiobj.velocity.z != 0.0f)) {
        object->apiobj.movement_facing_angle = NuAtan2D(object->apiobj.velocity.x, object->apiobj.velocity.z) + 0x8000;
        if (object->action_movement_state == 4 && Cheat_IsOn(0x13) != 0) {
            object->apiobj.movement_facing_angle += 0x8000;
        }
    } else {
        FaceOpponent(object, NULL);
    }
    return 0;
}

static void ForcePushed_MoveCode(GameObject_s *object) {
    NuFmax(NuFmax(0.4f, 1.2f), 0.8f);
    if (object->character_context != 0x1c)
        return;
    GameObject_s *source = object->force_target;
    if (source == NULL || source->character_context != 0x1b || (source->field_0xe21 & 1) != 0 ||
        (object->action_flags & 0x280) != 0)
        return;
    if (object->action_movement_state == 4 && Cheat_IsOn(0x13)) {
        if ((source->apiobj.field_0x1e4 & object->apiobj.field_0x1ec) != 0 ||
            (source->apiobj.field_0x1e8 & object->apiobj.field_0x1f0) != 0) {
            i16 angle =
                NuAtan2D(source->apiobj.pos_x - object->apiobj.pos_x, source->apiobj.pos_z - object->apiobj.pos_z);
            objhitobj_killparts_yrot = &angle;
            if (ObjHitObj(NULL, object, -1, 0, 0, 1) == 2) {
                NewRumble(source->pad_gamepad->pad, 0.5f, 0);
                NewBuzz(source->pad_gamepad->pad, 0.1f, 0);
                GameCam_HitJudder();
            }
        }
    } else if (ForcePush_SuperPush) {
        GameObject_s *target = Obj;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
                target == source || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                (target->apiobj.character_data->model_flags & 0x4002010) != 0x10 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x40) !=
                    0 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_094[1] & 2) != 0)
                continue;
            if (target->id == id_GONKDROID) {
                if (target->character_context == 0x17)
                    continue;
                if ((target->apiobj.field_0x1e4 & object->apiobj.field_0x1ec) == 0 &&
                    (target->apiobj.field_0x1e8 & object->apiobj.field_0x1f0) == 0)
                    continue;
                DeactivatePlayer(target, DEACTIVATEDTIME, NULL);
            } else {
                if ((target->apiobj.field_0x1e4 & object->apiobj.field_0x1ec) == 0 &&
                    (target->apiobj.field_0x1e8 & object->apiobj.field_0x1f0) == 0)
                    continue;
                i16 angle =
                    NuAtan2D(target->apiobj.pos_x - object->apiobj.pos_x, target->apiobj.pos_z - object->apiobj.pos_z);
                objhitobj_killparts_yrot = &angle;
                if (ObjHitObj(NULL, target, -1, 0, 0, 1) != 2)
                    continue;
                if (static_cast<u8>(source->apiobj.field_0x27c) < 2 && target->apiobj.field_0x27c == -1 && Arcade)
                    Arcade_AIKilled(source->apiobj.field_0x27c);
            }
            NewRumble(source->pad_gamepad->pad, 0.5f, 0);
            NewBuzz(source->pad_gamepad->pad, 0.1f, 0);
            GameCam_HitJudder();
        }
    }
}

i32 ZapTarget(GameObject_s *object) {
    if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0)
        return 0;
    u32 flags = object->apiobj.character_data->model_flags;
    if ((flags & 0x10) == 0)
        return 0;
    void **animations = object->apiobj.character_model->model_data_b;
    if (animations[0x41] == NULL)
        return 0;
    i8 context = object->character_context;
    if ((CInfo[context].flags & 0x8000) != 0)
        return 0;
    if ((flags & 0x20) != 0 && (animations[0x42] == NULL || animations[0x43] == NULL || animations[0x44] == NULL))
        return 0;
    if (context == 0x15)
        return 0;
    if (context == 0x17)
        return 0;
    if (context == 0x41)
        return 0;
    if (context == 0x33)
        return 0;
    if (context == 0x3b)
        return 0;
    return context != 0x39;
}

static void DeactivatedCode(GameObject_s *object) {
    if (object->character_context == 0x41) {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            return;
        object->airborne_action_duration += FRAMETIME;
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f)
            object->character_context = -1;
        return;
    }
    if ((object->apiobj.character_data->model_flags & 0x20000000) != 0) {
        bool controlled = false;
        for (i32 i = 0; i < 8; ++i) {
            if (Player[i] != NULL && Player[i]->character_context == 0x51 && Player[i]->field_0x788 != NULL &&
                static_cast<TECHNO *>(Player[i]->field_0x788)->controlled_object == object)
                controlled = true;
        }
        if (controlled || object->field_0xcc0 != NULL || (object->apiobj.flags_low & 0x80) != 0) {
            if (object->character_context == 0x17)
                ActivatePlayer(object);
        } else if (object->character_context != 0x2b && object->character_context != 0x3e) {
            if (object->character_context != 0x17)
                DeactivatePlayer(object, 1.0e9f, NULL);
            else
                object->context_animation_timer = 1.0e9f;
        }
    }
    if (object->character_context != 0x17)
        return;
    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
        object->airborne_action_duration += FRAMETIME;
    } else {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            return;
        float elapsed = object->airborne_action_duration + FRAMETIME;
        object->airborne_action_duration = elapsed;
        if (object->context_animation == 0x81) {
            if (AnimDuration(object->id, 0x81, 0.0f, 0.0f, 1) <= elapsed) {
                if (object->apiobj.character_model->model_data_b[0x41] == NULL) {
                    ActivatePlayer(object);
                    return;
                }
                object->context_animation = 0x41;
                if (object->context_animation_timer != 1.0e9f)
                    object->context_animation_timer -= object->airborne_action_duration;
                object->airborne_action_duration = 0.0f;
            }
        } else if ((static_cast<CHARACTERANIM_s *>(
                        object->apiobj.character_model->model_data_a[object->context_animation])
                        ->flags &
                    2) == 0 &&
                   AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1) <= elapsed) {
            ActivatePlayer(object);
            return;
        }
    }
    if (object->context_animation_timer != 1.0e9f && object->context_animation_timer < object->airborne_action_duration)
        ActivatePlayer(object);
    else if ((object->apiobj.character_data->model_flags & 0x20) != 0)
        SetProtocolDroidDeactivatedAction(object);
    if (!(object->field_0x768 > 0.0f))
        return;
    object->field_0x768 -= FRAMETIME;
    if (!(object->field_0x768 <= 0.0f))
        return;
    i32 kind;
    f32 duration;
    f32 range;
    if (object->action_movement_state == 2) {
        kind = 2;
        duration = DEACTIVATEDTIME;
        range = 9.0f;
    } else {
        if (!ForcePush_SuperMindTrick || object->action_movement_state != 1)
            return;
        kind = 1;
        duration = 5.0f;
        range = 1.0f;
    }
    GameObject_s *targets[10];
    i32 count = 0;
    GameObject_s *candidate = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT && count < 10; ++i, ++candidate) {
        GameObject_s *target = candidate;
        if (kind == 1) {
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
                target->apiobj.field_0x27d == 0 || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                target->apiobj.character_model->model_data_b[0x41] == NULL)
                continue;
            i32 context = target->character_context;
            if (context == 0x3c || context == 0x39 || context == 0x3b || context == 0x17 || context == 0x41 ||
                context == 0x0f || context == 0x47 || context == 0x46 ||
                (target->apiobj.character_data->model_flags & 0x10) != 0 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) !=
                    0 ||
                (CInfo[context].flags & 0x8000) != 0 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_094[1] & 2) != 0)
                continue;
        } else {
            if (!ZapTarget(target) || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x8040) !=
                    0 ||
                target->character_context == 0x0f || target->character_context == 0x3c ||
                target->character_context == 0x47 || target->character_context == 0x46)
                continue;
        }
        if (NuVecDistSqr(&target->apiobj.collision_position, &object->apiobj.collision_position, NULL) < range)
            targets[count++] = target;
    }
    i32 selected = count == 1 ? 0 : count > 1 ? qrand() / (0xffff / count + 1) : -1;
    if (selected == -1)
        return;
    GameObject_s *target = targets[selected];
    if (DeactivatePlayer(target, duration, NULL)) {
        target->action_movement_state = kind;
        target->context_variant_flags = (target->context_variant_flags & ~1) | (object->context_variant_flags & 1);
        if (kind == 2) {
            PlaySfx("R2Zap", &object->apiobj.collision_position);
            AddGameDebris(WORLD->debris_sys, (target->context_variant_flags & 1) != 0 ? 1 : 3,
                          &target->apiobj.collision_position);
            GameCam_HitJudder();
        }
        target->field_0x768 = qrand() * 1.5259022e-05f * 0.3f + 0.2f;
        if (target->id == id_JAWA)
            PlaySfx("Jawa_Dizzy", &target->apiobj.collision_position);
    }
}

static void DrawLightningBolts(GameObject_s *, GameObject_s *, i32) {
}

static void ForcePushCode(GameObject_s *object, i32 held, i32) {
    if (object->character_context != 0x1b)
        return;
    AlertSurroundingCreatures(object, &object->apiobj.collision_position);
    GameObject_s *initial_target = object->force_target;
    if ((object->pad_gamepad->allocated_5a & 0x10) != 0)
        held = 1;
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
            if ((object->field_0xe21 & 1) != 0)
                DrawLightningBolts(object, target, 0);
        } else {
            NewRumble(target->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.4f, 0);
            if ((object->field_0xe21 & 1) != 0)
                DrawLightningBolts(object, target, 1);
            object->force_glow_candidate = target;
            object->force_glow_candidate_kind = 2;
            if (target->character_context != 0x1c || (target->apiobj.character_data->model_flags & 0x20) != 0 ||
                target->apiobj.character_model->model_data_b[target->context_animation] == NULL ||
                CurrentAnim(&target->apiobj.anim_packet) == target->context_animation)
                object->context_animation_timer += FRAMETIME;
            NewRumble(object->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.4f, 0);
            f32 duration = (object->field_0xe21 & 2) != 0                                          ? 0.4f
                           : ((object->field_0xe20 & 0x80) != 0 || (object->field_0xe21 & 1) != 0) ? 1.2f
                                                                                                   : 0.8f;
            object->field_0x7a3 = 0;
            bool throw_target = false;
            if ((object->apiobj.flags_low & 0x80) != 0 && (object->apiobj.field_0x1f4 & 0x40000) == 0 &&
                ForcePush_Waft && (target->apiobj.flags_low & 0x80) == 0 && object->context_animation_timer >= 0.2f &&
                object->pad_gamepad->input_magnitude > 0.0f &&
                ((object->field_0xe21 & 1) == 0 || (GetGameCharacterData(target)->flags_090 & 0x4000) == 0) &&
                ((object->field_0xe20 & 0x80) != 0 || (object->field_0xe21 & 1) != 0)) {
                object->field_0x7a3 = 1;
                object->external_force.x = NU_SIN_LUT(object->apiobj.facing_angle) * 0.5f + object->apiobj.position.x;
                object->external_force.y = (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.75f +
                                           object->apiobj.collision_min.y;
                object->external_force.z = NU_COS_LUT(object->apiobj.facing_angle) * 0.5f + object->apiobj.position.z;
                u16 angle = NuAtan2D(target->apiobj.position.x - object->apiobj.position.x,
                                     target->apiobj.position.z - object->apiobj.position.z);
                i32 difference = RotDiff(GamePad_InputAngle(object, object->pad_gamepad), angle);
                if ((difference < 0 ? -difference : difference) < 0xaaa)
                    throw_target = true;
                else if (duration - 0.3f <= object->context_animation_timer)
                    object->context_animation_timer = duration - 0.3f;
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
                    } else if (target->apiobj.character_model->model_data_b[5] != NULL)
                        target->context_animation = 5;
                    target->current_hp = 1;
                    target->context_animation_timer = 0.0f;
                    target->airborne_action_duration = 2.0f;
                    u16 angle = NuAtan2D(target->apiobj.pos_x - object->apiobj.pos_x,
                                         target->apiobj.pos_z - object->apiobj.pos_z);
                    f32 speed = (qrand() * 1.5259022e-05f * 0.4f + 0.8f) * DIEAIRSPEED;
                    target->external_force.x = NU_SIN_LUT(angle) * speed;
                    target->external_force.z = NU_COS_LUT(angle) * speed;
                    target->context_variant_flags &= ~1;
                    target->apiobj.velocity.y = (qrand() * 1.5259022e-05f * 0.4f + 0.8f) * DIEAIRJUMPSPEED;
                    target->hit_variant =
                        static_cast<u8>(object->apiobj.field_0x27c) <= 1 ? object->apiobj.field_0x27c : -1;
                    SetFlicker(target, 0.4f);
                } else {
                    bool deactivate = true;
                    if ((object->field_0xe21 & 2) != 0) {
                        if (DeactivatePlayer(target, 5.0f, NULL)) {
                            target->action_movement_state = 1;
                            target->field_0x768 = qrand() * 1.5259022e-05f * 0.3f + 0.2f;
                            if (target->id == id_JAWA)
                                PlaySfx("Jawa_Dizzy", &target->apiobj.collision_position);
                        }
                    } else if (target->id == id_GONKDROID ||
                               ((target->apiobj.character_data->model_flags & 0x10) != 0 && target->hitpoints >= 2))
                        DeactivatePlayer(target, DEACTIVATEDTIME, NULL);
                    else {
                        bool immune = FreePlay != 0 && (object->field_0xe21 & 1) != 0 &&
                                      (GetGameCharacterData(target)->flags_090 & 0x4000) != 0;
                        i32 damage = immune                             ? 0
                                     : target->apiobj.field_0x27c == -1 ? static_cast<i8>(target->current_hp)
                                                                        : 1;
                        u16 flags =
                            WORLD->current_level == BLOCKADERUNNERB_LDATA ? (object->apiobj.field_0x1f4 >> 10) & 1 : 0;
                        if (ObjHitObj(object, target, damage, flags, 0, 1) != 2 && (object->field_0xe20 & 0x80) != 0 &&
                            !immune)
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
        object->airborne_action_duration =
            object->airborne_action_duration - FRAMETIME > 0.2f ? object->airborne_action_duration - FRAMETIME : 0.2f;
    } else if ((object->apiobj.field_0x1f4 & 0x40000) == 0) {
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f) {
            target = object->force_target;
            if (object->context_animation_timer < 0.4f) {
                ReleaseForce(object, 0);
                if ((object->field_0xe20 & 0x80) == 0 || target == NULL)
                    return;
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
                if ((object->field_0xe20 & 0x80) == 0)
                    return;
            }
            target->fall_animation_timer = 0.2f;
        }
    }
}

static void ForceThrowCode(GameObject_s *object, i32 pressed, i32) {
    if (object->character_context == 0x12) {
        AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            CurrentAnim(&object->apiobj.anim_packet) != object->context_animation)
            return;
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
                        SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f,
                                      5.0f, 10.0f);
                    }
                    SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                    object->force_heading = GizForces_AngleToForce(&object->apiobj.position, object->gizforce_target);
                } else if (force->anim_set != NULL && force->anim_set->object_count == 1) {
                    part = Part_FindFromHSpecial(&force->anim_set->objects->special);
                    if (part != NULL) {
                        object->force_glow_candidate = part;
                        object->force_glow_candidate_kind = 3;
                        SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
                        object->force_heading = NuAtan2D(part->position.x - object->apiobj.position.x,
                                                         part->position.z - object->apiobj.position.z);
                        object->force_glow_object = object->force_glow_candidate;
                        object->force_glow_kind = object->force_glow_candidate_kind;
                    }
                }
            }
        } else if ((part->active & 1) == 0)
            object->character_context = -1;
        else {
            object->force_heading =
                NuAtan2D(part->position.x - object->apiobj.position.x, part->position.z - object->apiobj.position.z);
            SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
        }
        if (previous_time <= 1.0f || object->context_animation_timer > 1.0f)
            return;
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
    if (!pressed && (object->pad_gamepad->allocated_5a & 4) == 0)
        return;
    if ((object->apiobj.field_0x1f8 & 0x180) == 0x80)
        GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
    if (object->gizforce_target == NULL || (object->gizforce_target->config_flags & 0x80) == 0)
        return;
    object->character_context = 0x12;
    object->context_animation = 0x27;
    if (object->apiobj.character_model->model_data_b[0x27] == NULL)
        object->context_animation = 0xb;
    object->context_flags &= ~0x40;
    object->field_0x788 = NULL;
    object->force_throw_target = NULL;
    object->context_animation_timer = 2.0f;
    f32 distance = 1.0e9f;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *target = Player[i];
        if (target == NULL || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
            (target->apiobj.character_data->model_flags & 0x80000) != 0)
            continue;
        f32 next_distance = NuVecDistSqr(&object->apiobj.position, &target->apiobj.position, NULL);
        if (object->force_throw_target == NULL ||
            (((object->apiobj.flags_low & 0x80) != 0 || (object->force_throw_target->apiobj.flags_low & 0x80) == 0) &&
             next_distance < distance)) {
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
            if (object->context_animation_timer < 0.2f)
                part->velocity.y = SeekValF(part->velocity.y, 2.0f, 6.0f);
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
                                                   ? object->airborne_action_duration - FRAMETIME
                                                   : 0.2f;
            return;
        }
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f)
            ReleaseForce(object, 0);
        return;
    }
    PART_s *assist = NULL;
    if ((object->apiobj.flags_low & 0x80) == 0) {
        if ((object->field_0xefd & 1) == 0 || player == NULL || player->force_part == NULL)
            return;
        assist = player->force_part;
    }
    if (object->apiobj.field_0x27d == 0)
        return;
    i32 context = object->character_context;
    if (context != -1 && (CInfo[context].flags & 4) == 0 && context != 6 && context != 7 &&
        !objInNetWaitContext(object, 0x1d))
        return;
    PART_s *part;
    if (waiting)
        part = object->force_part;
    else if (assist != NULL)
        part = assist;
    else {
        f32 range = TouchHacks::GetIncomingPartRange();
        f32 radius = object->apiobj.field_0x1e0;
        if (radius <= object->apiobj.field_0x1dc)
            radius = object->apiobj.field_0x1dc;
        part = FindIncomingPart(object, &object->apiobj.collision_position, radius, 0x800a, range);
    }
    if (part == NULL)
        return;
    object->field_0xe22 |= 2;
    object->force_glow_candidate = part;
    object->force_glow_candidate_kind = 3;
    SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
    if (assist == NULL && pressed == 0 && !waiting)
        return;
    object->force_part = part;
    part->flags |= 0x10000;
    part->gravity = 0.0f;
    object->context_animation_timer = 0.0f;
    object->airborne_action_duration = 0.3f;
    object->force_target = NULL;
    object->blowup_target = NULL;
    u8 mask = 1u << (object->apiobj.field_0x27c & 31);
    object->character_context = 0x1d;
    if (part->force_player_mask != -1)
        mask |= part->force_player_mask;
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
    if (SuperWeirdo(object) && (object->apiobj.character_data->model_flags & 8) == 0)
        glow_model = 0xdf;
    else
        glow_model = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].glow_model;
    DropInOutCode(object);
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    u16 weapon = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->weapon_model & 0xfffd;
    bool has_sabre = weapon == 0x65 || weapon == 0x69;
    if (object->suit != NULL)
        Signal_MoveCode(WORLD, object);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    Hang_MoveCode(object);
    Ledge_MoveCode(WORLD, object);
    if (LedgeTerrain_On)
        LedgeTerrain_MoveCode(object);
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
    } else
        jump_animations = SuperWeirdo(object) ? 0x11b : 0x1b;
    JumpCode(object, jump_pressed, jump_held, jump_animations, action_pressed, action_held, hit_effect);
    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    if ((object->apiobj.character_data->model_flags & 0x100000) != 0)
        Grapple_MoveCode(object);
    BuildIt_MoveCode(object);
    if (has_sabre || SuperWeirdo(object)) {
        ForceDeflectCode(object, special_pressed, special_held, 0);
        ForcePushCode(object, special_held, 0);
        FindForcePushTarget(object, special_pressed, 1);
        ForceThrowCode(object, special_pressed, 0);
        ForceCode(object, special_pressed, special_held, 0);
        FindForcePushTarget(object, special_pressed, 2);
        if (object->apiobj.field_0x287 == 0 && FadeSys.fade == 0.0f) {
            if (object->force_glow_step > 0.0f)
                object->force_glow_step -= FRAMETIME;
            else
                ForceGlowCode(object, glow_model);
        } else
            ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }
    ForcePushed_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    if ((object->apiobj.character_data->model_flags & 0x40000) != 0 || SuperWeirdo(object))
        Teleport_MoveCode(object, special_pressed);
    i32 detonator_used = ThermalDetonator_MoveCode(object);
    SuperCarry_MoveCode(WORLD, object);
    GizmoBlowupCheckProximity(WORLD, object);
    ComboRotateCode(object, action_held);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    SpecialMove_VictimCode(object);
    HoldCode(object);
    Attracto_MoveCode(WORLD, object);
    SecurityDoor_MoveCode(WORLD, object);
    if (has_sabre) {
        BlockCode(object, action_pressed, action_held, jump_pressed, 0);
        SwipeCode(object, action_pressed, action_held);
        if (detonator_used == 1)
            special_pressed = 0;
        LightSabreComboCode(object, action_pressed, action_held, jump_pressed, special_pressed);
        JediLightCode(object);
    } else {
        BlockCode(object, action_pressed, action_held, 0, 0);
        PunchCode(object, action_pressed, action_held, 1, 0, 0.0f);
        DodgeCode(object, action_pressed, jump_pressed);
        if ((object->apiobj.character_data->model_flags & 0x80) != 0) {
            if (detonator_used == 1)
                special_pressed = 0;
            ShootCode(object, action_pressed, special_pressed, 1, 1, 0);
        } else if (object->batarang != NULL)
            Batarang_MoveCode(object);
    }
    if (static_cast<i8>(static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[0]) <
        0) {
        CommunicateCode(object, GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed, 0);
    }
    if (object->character_context == 0x14) {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame != NULL) {
            if ((object->field_0xe22 & 4) == 0) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (event >= 1.0f && event <= *frame) {
                    object->field_0xe22 |= 4;
                    object->field_0xe20 |= 0x10;
                    PlaySfx("JgRocket", &object->apiobj.collision_position);
                }
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f)
                object->character_context = -1;
        }
    } else if (object->character_context == -1 && (object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0 &&
               ((object->apiobj.character_data->model_flags & 0x1000000) != 0 || object->field_0x108e == 6)) {
        FireBountyHunterRocket(object);
    }
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0 &&
        ((object->movement_runtime_flags & 4) != 0 ||
         (object->fall_animation_timer >= 0.2f &&
          (object->pad_gamepad->input_magnitude == 0.0f ||
           ((object->apiobj.flags_low & 0x80) == 0 && object->apiobj.character_model->model_data_b[0x59] != NULL)))))
        StartFallLand(object, -1);
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
}

void Move_JEDI(GameObject_s *object) {
    const u32 action_mask = GAMEPAD_ACTION;
    GAMEPAD_s *pad = object->pad_gamepad;
    const u32 pressed = pad->buttons_pressed;
    const u32 held = pad->buttons_held;
    const u32 jump_mask = GAMEPAD_JUMP;
    const u32 special_mask = GAMEPAD_SPECIAL;

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
    const i32 jump_pressed = pressed & jump_mask;
    Climb_MoveCode(object);
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

    u32 jump_animations;
    if (object->id == id_IMPERIALGUARD)
        jump_animations = 0x10;
    else if (object->id == id_BODYGUARD)
        jump_animations = 0x10;
    else
        jump_animations = 0x1b;
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
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
            if (object->force_glow_step > 0.0f)
                object->force_glow_step -= FRAMETIME;
            else
                ForceGlowCode(object, glow_model);
        } else
            ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
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
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) != 0 &&
        WORLD->debris_sys->entries[124].effect != -1)
        AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[124].effect, &object->apiobj.collision_position, 6,
                                          FRAMETIME, 0, 0, NULL);
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0) {
        if (!(object->fall_animation_timer >= 0.2f)) {
            if ((object->movement_runtime_flags & 4) != 0)
                StartFallLand(object, -1);
        } else if ((object->movement_runtime_flags & 4) != 0 || object->pad_gamepad->input_magnitude == 0.0f ||
                   ((object->apiobj.flags_low & 0x80) == 0 &&
                    object->apiobj.character_model->model_data_b[0x59] != NULL))
            StartFallLand(object, -1);
    }
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
    f32 force_volume = 0.0f;
    if ((object->character_context == 0x1b && object->field_0x7a3 == 1) || object->character_context == 0x1d ||
        object->character_context == 8) {
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

void MovePlayer_NETWORK(GameObject_s *object) {
    APIOBJECT_s &api = object->apiobj;
    if ((api.character_data->model_flags & 0x2000) != 0)
        object->in_narrow_socket = ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx);

    f32 turn_override = 0.0f;
    if ((api.flags_low & 0x80) != 0 && object->character_context == 0x1b && (object->field_0xe21 & 2) == 0) {
        if (object->field_0x7a3 != 1)
            FaceOpponent(object, NULL);
        turn_override = 0.333f;
    }
    i32 direct_turn = object->character_context == 0x33 ? 1 : object->snap_facing;
    f32 turn_rate;
    if (SuperCarry_Carrying(object))
        turn_rate = object->field_0x768 * 8.0f;
    else if (object->character_context == 0x1c || object->character_context == 0x33)
        turn_rate = 8.0f;
    else if (turn_override != 0.0f)
        turn_rate = turn_override * 8.0f;
    else {
        turn_rate = static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->turn_rate * 8.0f;
        if (object->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA)
            turn_rate += turn_rate;
    }

    if ((object->field_0xefd & 0x80) != 0 && (CInfo[object->character_context].flags & 1) == 0)
        api.movement_facing_angle += 0x8000;
    if ((object->field_0xefd & 4) != 0) {
        api.facing_angle = api.movement_facing_angle;
        api.field_0x276 = api.movement_facing_angle;
    } else if (direct_turn == 0 &&
               (turn_override != 0.0f || (CInfo[object->character_context].parameter & 2) != 0 ||
                (static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->flags_090 & 0x100) != 0 ||
                SuperCarry_Carrying(object))) {
        if (object->character_context == 0x4b &&
            (static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->flags_090 & 0x100) == 0)
            turn_rate *= 0.333f;
        api.facing_angle =
            TurnRot(api.facing_angle, api.movement_facing_angle, static_cast<i32>(turn_rate * 3.0f * 16384.0f), NULL);
        api.field_0x276 = SeekRot(api.field_0x276, api.facing_angle, 10.0f);
        api.movement_facing_angle = api.facing_angle;
    } else {
        api.facing_angle = SeekRot(api.facing_angle, api.movement_facing_angle, turn_rate);
        api.field_0x276 = api.facing_angle;
    }
    if (object->torpedo != NULL && (api.field_0x1f8 & 0x1000) != 0 && api.field_0x287 == 0 &&
        (object->field_0xe20 & 0x20) == 0)
        Torpedo_UpdateJobbies(object);
    SpecialMove_VictimCode(object);
    i32 glow_model;
    if (SuperWeirdo(object) && (api.character_data->model_flags & 8) == 0)
        glow_model = 0xdf;
    else {
        glow_model = object->blade_index == -1 ? 0xdf : BladeTab[object->blade_index].glow_model;
        if (glow_model == -1)
            glow_model = 0xdf;
    }
    Tag_Check(object);
    ForcePushCode(object, 0, 0);
    object->field_0xd8c = 1.0f;
    if (api.field_0x287 == 0 && FadeSys.fade == 0.0f) {
        if (object->force_glow_step > 0.0f)
            object->force_glow_step -= FRAMETIME;
        else
            ForceGlowCode(object, glow_model);
    } else
        ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    if ((object->field_0xe20 & 0x20) == 0)
        TorpedoCode(object, 0, 0.0f);
    PeriscodeCode(object);
    MovePlayer_TWIST(object);
    if (PodLevel(WORLD->area)) {
        Move_POD(object);
        KeepOnScreen(object);
    }
    if (object->id == id_SNAKE) {
        if (object->field_0x10b8 == NULL)
            CreateSnakeBody(object, 0xb);
        if (object->field_0x10b8 != NULL)
            UpdateSnakeBody(object);
    }
    object->field_0xefd &= ~0x40;
    api.field_0x214 = api.field_0x218;
    api.start_position = api.position;
    api.initial_position = api.collision_position;
    api.field_0x27e = api.field_0x27d;
    object->previous_movement_angle = api.field_0x276;
    Teleport_NetMoveCode(object);
    if (api.field_0x27c != -1)
        TractorBeamCode(object);
    api.previous_velocity.x = api.velocity.x;
    api.previous_velocity.y = api.velocity.y;
    api.previous_velocity.z = api.velocity.z;
    object->target_velocity = api.velocity;
    object->field_0xefd &= ~4;
    object->field_0x107a = -1;
    api.field_0x1fa |= 1;
    api.flags_low &= ~4;
    APIObjectVelocities(object);
    GameObjectOrigin(object);
    AddSurfaceRipples(object);
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
static __used__ i32 Move_UpdateHint(HINT_s *) {
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
    if (LEGOCONTEXT_JUMP == -1)
        return 0;
    if ((object->apiobj.character_data->model_flags & 8) != 0)
        SetWeaponOut(object);
    object->character_context = LEGOCONTEXT_JUMP;
    object->action_movement_state = 4;
    object->context_animation_timer = 0.0f;
    object->context_animation = LEGOACT_SLAM;
    if ((object->apiobj.flags_low & 0x80) != 0)
        object->field_0xef9 |= 8;
    object->jump_sequence = 0;
    object->apiobj.velocity.y = speed;
    PlayJumpSfx(object, 4);
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
    object->jump_flags |= 1;
    if ((object->apiobj.flags_low & 0x80) != 0)
        Hint_SetComplete(0x60a);
    return 1;
}

void StartLunge(GameObject_s *object, f32 speed, f32 height) {
    object->context_animation_timer = 0.0f;
    object->context_flags &= ~0x80;
    object->character_context = 0;
    object->action_movement_state = 3;
    object->context_animation = 0x1f;
    if ((object->apiobj.flags_low & 0x80) != 0)
        object->field_0xef9 |= 8;
    object->jump_flags |= 1;
    object->apiobj.velocity.y = speed;
    object->jump_sequence = 0;
    if (height > 0.0f)
        MakeJumpReachHeight(object, height, 0);
    PlayJumpSfx(object, 3);
    if ((object->apiobj.character_data->model_flags & 8) != 0)
        SetWeaponOut(object);
}

void StartSlide(GameObject_s *, i32) {
}

void CanObjSlide(GameObject_s *, i32) {
}

i32 CanStepBack(GameObject_s *object) {
    i8 context = object->character_context;
    if ((CInfo[context].flags & 0x10000) != 0 ||
        (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == context && object->action_movement_state == 3)) {
        return 1;
    }
    GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if ((character->flags_090 & 0x8000) != 0 && LEGOCONTEXT_COMBO != 0 && LEGOCONTEXT_COMBO == context) {
        return 1;
    }
    return 0;
}

void FlattenCode(GameObject_s *) {
}

i32 Glide_Start(GameObject_s *object) {
    if (LEGOCONTEXT_GLIDE == -1)
        return 0;
    object->character_context = 0x4f;
    object->context_animation = 0x93;
    object->context_animation_timer = 0.0f;
    object->airborne_action_duration = 0.4f;
    object->field_0x7a3 = 0;
    object->field_0x788 = NULL;
    return 1;
}

void JetPackCode(GameObject_s *, i32, i32, i32) {
}

float SeekLinearF(float current, float target, float step) {
    if (current > target) {
        current -= step;
        if (current < target) {
            current = target;
        }
    } else if (current < target) {
        current += step;
        if (current > target) {
            current = target;
        }
    }
    return current;
}

void StartLaunch(GameObject_s *) {
}

static i32 getvehiclehoverheight_hothbattlehack;

float GetVehicleHoverHeight(GameObject_s *object, float *separation_offset) {
    float height = object->hover_height_override;
    LEVELDATA *level = WORLD->current_level;
    if (height == 1.0e9f) {
        height = level->hover_height;
        if (height == 0.0f) {
            height = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28;
            if (object->id == id_SPEEDERBIKE && level == SPEEDERCHASEA_LDATA && disable_narrow_socks != 0) {
                height *= 0.5f;
            }
        }
    }

    bool separate;
    if (level == SPEEDERCHASEA_LDATA) {
        separate = disable_narrow_socks == 0 && object->apiobj.field_0x27c != -1 && object->movement_spline == NULL &&
                   Player[0] != NULL && Player[0]->takeover_source != NULL && Player[0]->id == id_SPEEDERBIKE &&
                   Player[1] != NULL && Player[1]->takeover_source != NULL && Player[0]->id == Player[1]->id;
    } else {
        separate = static_cast<i8>(object->apiobj.flags_low) < 0 && Player[0] != NULL &&
                   (Player[0]->apiobj.character_data->model_flags & 0x2000) != 0 &&
                   (static_cast<i8>(Player[0]->apiobj.flags_low) < 0 || Player[0]->character_context == 0x24) &&
                   Player[1] != NULL && (Player[1]->apiobj.character_data->model_flags & 0x2000) != 0 &&
                   (static_cast<i8>(Player[1]->apiobj.flags_low) < 0 || Player[1]->character_context == 0x24);
    }
    if (separate) {
        float combined_height = (Player[0]->apiobj.field_0x1e0 + Player[1]->apiobj.field_0x1e0) * 0.6f;
        float combined_radius = Player[0]->apiobj.field_0x1dc + Player[1]->apiobj.field_0x1dc;
        float inner_distance = combined_radius * combined_radius;
        float outer_radius = combined_radius * 3.0f;
        float outer_distance = outer_radius * outer_radius;
        float distance = NuVecXZDistSqr(&Player[0]->apiobj.position, &Player[1]->apiobj.position, NULL);
        float blend;
        if (distance < inner_distance) {
            blend = 1.0f;
        } else if (distance < outer_distance) {
            blend = 1.0f - (distance - inner_distance) / (outer_distance - inner_distance);
        } else {
            blend = 0.0f;
        }
        float offset = blend * combined_height;
        float correction = 0.0f;
        float adjusted_height = height - offset;
        if (adjusted_height < offset) {
            correction = offset - adjusted_height;
        }
        if (Player[0] == object) {
            adjusted_height = height + offset;
            if (separation_offset != NULL) {
                *separation_offset = offset;
            }
        } else if (separation_offset != NULL) {
            *separation_offset = -offset;
        }
        height = adjusted_height + correction;
        level = WORLD->current_level;
    } else if (separation_offset != NULL) {
        *separation_offset = 0.0f;
    }

    i32 hoth_battle = 0;
    if (level == HOTHBATTLEA_LDATA && (TerLayer[static_cast<i8>(object->apiobj.field_0x27f)].flags & 1) != 0) {
        height = 0.0f;
        hoth_battle = 1;
    }
    getvehiclehoverheight_hothbattlehack = hoth_battle;
    return height;
}

static __used__ i32 IsAFallAnim(i32 animation) {
    if (animation == 5)
        return 1;
    if (animation == 0x28)
        return 1;
    if (animation == 0x4b)
        return 1;
    if (animation == 0x4c)
        return 1;
    return animation == 0x74;
}

void ApplyGravity(GameObject_s *object, float *gravity, float hover_height, float seek_rate, float *ground_height) {
    float extra_hover_offset = applygravity_extrahoveroffset;
    applygravity_extrahoveroffset = 0.0f;
    if (object->movement_spline != NULL) {
        return;
    }
    i32 flags = object->apiobj.flags_low;
    if ((flags & 0x20) != 0 || object->move_override != NULL) {
        return;
    }
    i32 context = object->character_context;
    float acceleration;
    if ((CInfo[context].flags & 0x400) != 0) {
        if (context != 0x44 || static_cast<u16>(object->context_animation - 5) > 1) {
            return;
        }
    } else if (context == 0x5f) {
        if ((object->context_variant_flags & 1) != 0) {
            return;
        }
    } else if (context == 0x4b) {
        if (object->suit == NULL || (static_cast<SUIT_s *>(object->suit)->store_flag & 0x10) == 0) {
            return;
        }
        acceleration = -1.0f;
        gravity = &acceleration;
    }
    if ((flags & 4) != 0 || static_cast<i8>(object->field_0xefb) < 0 || context == 0x2c) {
        return;
    }
    if ((object->field_0xe20 & 0x20) != 0) {
        object->apiobj.velocity.y = 0.0f;
        return;
    }
    CHARACTERDATA *character = object->apiobj.character_data;
    if ((character->model_flags & 0x2000) == 0 && static_cast<u8>(context - 0x23) < 2) {
        object->apiobj.velocity.y = 0.0f;
        return;
    }
    WORLDINFO_s *world = WORLD;
    if ((WORLD->current_level->flags & 0x40000) != 0 && gravity == NULL &&
        static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity == 0.0f) {
        return;
    }
    if (object->field_0x1086 == 4) {
        return;
    }

    float target_ground_height;
    if (context == 0x1e || context == 0x22 || context == 0x42) {
        hover_height = 0.2f;
        seek_rate = 8.0f;
    } else if (context == 0x1c) {
        GameObject_s *target = object->force_target;
        if (target != NULL && (target->action_flags & 0x180) != 0) {
            if (target->field_0x7a3 == 1) {
                return;
            }
            if ((target->field_0xe21 & 4) == 0 &&
                ((target->field_0xe21 & 1) == 0 ||
                 (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->flags_090 & 0x4000) == 0)) {
                target_ground_height = target->apiobj.lower_position.y;
                ground_height = &target_ground_height;
                hover_height = (target->apiobj.upper_position.y - target_ground_height) * 0.75f;
                seek_rate = 8.0f;
            }
        }
    } else if (context == 0x5d && object->field_0x768 != 1.0e9f) {
        float target_velocity = (object->apiobj.position.y - object->field_0x768) *
                                static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity * 0.333f;
        object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, 8.0f);
        return;
    }

    float height = object->hover_height_override == 1.0e9f ? hover_height : object->hover_height_override;
    bool falling_hover = false;
    if (height != 0.0f && object->fall_recovery_timer > 0.0f && WORLD->current_level != E2VEHICLEBONUSA_LDATA) {
        if ((GUNSHIP_ADATA != NULL && GUNSHIP_ADATA == WORLD->area) ||
            (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area)) {
            height = 0.0f;
        } else if (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field275_0x116 == 2 &&
                   static_cast<i8>(flags) < 0) {
            if (object->fall_hover_height != 2.0e6f) {
                falling_hover = true;
                if (object->fall_recovery_timer >= 0.6f) {
                    object->fall_hover_height -= FRAMETIME * 3.0f;
                }
            }
        } else {
            height -= object->fall_recovery_timer * 0.5f;
        }
    }
    if (context != 0x33) {
        if ((object->apiobj.field_0x27d & 1) != 0 && object->field_0x1084 == 0 && object->apiobj.velocity.y == 0.0f &&
            object->surface_normal.y > NuTrigTable[0x21c7]) {
            object->apiobj.field_0x27d &= ~1;
        }
        if (height == 0.0f && (object->apiobj.field_0x27d & 1) != 0) {
            object->apiobj.velocity.y = 0.0f;
            return;
        }
    }

    bool hold_jump = false;
    if (gravity != NULL) {
        acceleration = *gravity;
    } else {
        if (object->apiobj.field_0x218 != 2.0e6f && height != 0.0f && context != 0x2b) {
            if (falling_hover) {
                object->terrain_origin_floor_offset = 0.0f;
                float target_velocity = (object->apiobj.position.y - object->fall_hover_height) *
                                        static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity * 0.333f;
                object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, seek_rate);
                return;
            }
            if ((object->apiobj.field_0x27f == 0x10 || object->apiobj.field_0x27f == 7) &&
                (VehicleArea != 0 ||
                 (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->flags_090 & 0x40) == 0 ||
                 object->takeover_source != NULL)) {
                float separation;
                GetVehicleHoverHeight(object, &separation);
                separation += object->apiobj.water_height;
                if (object->hover_height_override != 1.0e9f) {
                    separation += object->hover_height_override;
                }
                float offset = VehicleTurnOrLoopOffset(object);
                float target_velocity;
                if (object->apiobj.model_draw_result != 0 || FRAMETIME < 0.016666668f) {
                    target_velocity =
                        (object->apiobj.collision_position.y - (offset + separation)) *
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->gravity * 0.333f;
                } else {
                    target_velocity =
                        ((object->apiobj.collision_position.y - (offset + separation)) *
                         static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->gravity) /
                        (FRAMETIME * 180.0f);
                }
                object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, seek_rate);
                return;
            }
            if (PODSPRINT_ADATA != NULL && PODSPRINT_ADATA == world->area && static_cast<i8>(flags) < 0) {
                TUBE *tube = Tube_InAnyCylinder(world, object, 1);
                if (tube != NULL) {
                    ground_height = &tube->top;
                }
            }
            float floor =
                ground_height != NULL && *ground_height != 2.0e6f ? *ground_height : object->apiobj.field_0x218;
            if (object->apiobj.water_height != 2.0e6f && object->apiobj.water_height > floor &&
                object->apiobj.field_0x27f != 5 && object->apiobj.field_0x27f != 7 &&
                object->apiobj.field_0x27f != 0x10) {
                floor = object->apiobj.water_height;
            }
            floor += extra_hover_offset;
            float offset = VehicleTurnOrLoopOffset(object);
            character = object->apiobj.character_data;
            floor += offset;
            height += offset;
            GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
            if ((data->flags_094[0] & 0x20) != 0 || (PODSPRINT_ADATA != NULL && PODSPRINT_ADATA == WORLD->area) ||
                !(GamePlayTimer.time_elapsed >= 1.0f) || !(object->apiobj.collision_origin.y > height * 5.0f + floor)) {
                object->terrain_origin_floor_offset = height;
                float pull = PODSPRINT_ADATA != NULL && PODSPRINT_ADATA == WORLD->area ? -35.0f : data->gravity;
                float target_velocity;
                if (object->apiobj.model_draw_result != 0 || FRAMETIME < 0.016666668f) {
                    target_velocity = (object->apiobj.collision_origin.y - (floor + height)) * pull * 0.333f;
                } else {
                    target_velocity =
                        ((object->apiobj.collision_origin.y - (floor + height)) * pull) / (FRAMETIME * 180.0f);
                }
                object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, seek_rate);
                return;
            }
        }

        bool default_gravity = true;
        if ((object->apiobj.field_0x218 == 2.0e6f || height == 0.0f) && context == 0) {
            if (object->action_movement_state == 3) {
                acceleration = -10.0f;
                default_gravity = false;
            } else if (object->action_movement_state == 4) {
                acceleration = SLAMGRAVITY;
                default_gravity = false;
            } else if (object->action_movement_state == 5) {
                if (object->apiobj.field_0x281 == 0x1a) {
                    object->field_0xe31 = 3;
                    object->jump_variant_timer = 0.0f;
                } else if ((object->pad_gamepad->buttons_held & GAMEPAD_JUMP) == 0) {
                    if (object->field_0xe31 == 2) {
                        object->field_0xe31 = 3;
                    }
                    object->jump_variant_timer = 0.0f;
                } else if (object->field_0xe31 == 3) {
                    acceleration = -1.0f;
                    default_gravity = false;
                } else if (object->field_0xe31 == 2) {
                    object->apiobj.velocity.y = 0.0f;
                    return;
                } else {
                    hold_jump = true;
                }
            }
        }
        if (default_gravity) {
            acceleration = static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity;
            if (object->character_context == 0x2b) {
                if (object->field_0xe36 == 3) {
                    acceleration = -acceleration;
                } else if (object->field_0xe36 == 2) {
                    acceleration = -0.25f;
                } else if (object->field_0xe36 == 4) {
                    acceleration = -2.0f;
                }
            }
        }
        float middle = (object->apiobj.position.y - object->character_bottom * object->apiobj.field_0xa8) +
                       (object->character_top - object->character_bottom) * 0.5f * object->apiobj.field_0xa8;
        if ((object->apiobj.field_0x27d == 0 && object->field_0x1084 != 0 && object->contact_position.y < middle &&
             object->apiobj.anim_packet.blending == 0 &&
             object->apiobj.character_model->model_data_b[object->apiobj.anim_packet.animation_index] != NULL &&
             IsAFallAnim(object->apiobj.anim_packet.animation_index) != 0) ||
            object->fall_acceleration_timer > 0.0f) {
            acceleration += acceleration;
            if (acceleration > -5.0f) {
                acceleration = -5.0f;
            }
        }
    }
    float previous_velocity = object->apiobj.velocity.y;
    if ((object->apiobj.field_0x27d & 0xfd) == 0 || object->id == id_DRAGBOMB) {
        object->apiobj.velocity.y = acceleration * FRAMETIME + previous_velocity;
    }
    if (object->character_context == 0x2b) {
        if (object->field_0xe36 == 3) {
            if (object->apiobj.velocity.y < 0.1f) {
                object->apiobj.velocity.y = 0.1f;
            }
        } else if (object->apiobj.velocity.y > -0.1f) {
            object->apiobj.velocity.y = -0.1f;
        }
    }
    if (hold_jump && previous_velocity > 0.0f && object->apiobj.velocity.y <= 0.0f) {
        float duration = static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field_0x48;
        if (duration > 0.0f) {
            object->apiobj.velocity.y = 0.0f;
            object->jump_variant_timer = (TouchHacks::TouchControlsActive ? 1.3f : 1.0f) * duration;
            if (static_cast<i8>(object->apiobj.flags_low) >= 0) {
                object->jump_variant_timer *= 1.5f;
            }
            object->field_0xe31 = 2;
        } else {
            object->field_0xe31 = 3;
        }
    }
}

void SetObjTarget(GameObject_s *, GameObject_s *) {
}

void SnapPosTaken(WORLDINFO_s *, pushblock_s *, nuvec_s *, i32) {
}

void StartFlatten(GameObject_s *, GameObject_s *) {
}

GAMEANTINODE_s *GameAntinode_RegisterAntiNodeUsingData(GAMEANTINODESYS_s *, NUVEC *, u16, GAMEANTINODEDATA_s *, f32,
                                                       i32);

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
        {minimum.x, minimum.y, minimum.z}, {maximum.x, minimum.y, minimum.z}, {maximum.x, minimum.y, maximum.z},
        {minimum.x, minimum.y, maximum.z}, {minimum.x, maximum.y, minimum.z}, {maximum.x, maximum.y, minimum.z},
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
        if (object->head_target == NULL ||
            (position != object->head_target && object->head_target_priority <= priority)) {
            i32 random = qrand();
            object->head_target = position;
            object->head_target_priority = priority;
            f32 delay = maximum_delay * random * 1.5259022e-05f + (1.0f - random * 1.5259022e-05f) * minimum_delay;
            object->head_target_delay = delay;
            object->head_target_timer = time + delay;
        }
        object->head_target_position = *object->head_target;
    }
}

extern i32 DoubleJump_JediSlam;
extern f32 SLAMJUMPSPEED;
void PlayLandSfx(GameObject_s *, i32, i32);
void AddWaterSplash(GameObject_s *, NUVEC *);

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
    object->context_animation_timer = duration <= 0.0f ? 1.0f : duration;
    PlayJumpSfx(object, 2);
    return 1;
}

i32 LEGOCONTEXT_GETIN = -1;
i32 LEGOCONTEXT_EATEN = -1;

static void ClearLastSafeTakeoverSource(GameObject_s *object) {
    if (object->takeover_source != NULL &&
        (LEGOCONTEXT_GETIN == -1 || object->character_context != LEGOCONTEXT_GETIN) &&
        (LEGOCONTEXT_BEENTAKENOVER == -1 || object->character_context != LEGOCONTEXT_BEENTAKENOVER))
        object->takeover_source = NULL;
}

void UpdateLastSafePosition(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    object->field_0xf02 &= ~0x40;
    if (api.field_0x287 == 0 && (LEGOCONTEXT_DOOMED == -1 || object->character_context != LEGOCONTEXT_DOOMED) &&
        (api.flags_low & 4) == 0) {
        api.field_0x1c0 = api.position;
        GameObject_s *terrain_object = object;
        if (object->field_0xcc0 != NULL && LEGOCONTEXT_BEENTAKENOVER != -1 &&
            object->character_context == LEGOCONTEXT_BEENTAKENOVER)
            terrain_object = object->field_0xcc0;
        const i32 surface = static_cast<i8>(terrain_object->apiobj.field_0x281);
        i32 unsafe;
        if (surface != -1 && (TerSurface[surface].flags & 0xc041) != 0) {
            unsafe = 1;
        } else {
            const i8 layer = terrain_object->apiobj.field_0x27f;
            unsafe = 0;
            if (layer != -1 && ((TerLayer[layer].flags & 1) != 0 || layer == 9))
                unsafe = 1;
        }
        if (unsafe && VehicleArea != 0 && BonusArea != 0)
            unsafe =
                !(static_cast<GAMECHARACTERDATA *>(terrain_object->apiobj.character_data->field11_0x24)->field_0x28 >
                  0.0f);
        if (WORLD->current_level == NEWTOWN_LDATA && terrain_object->field_0xe31 == 1 &&
            terrain_object->apiobj.position.y > 0.7f)
            unsafe = true;
        do {
            if ((object->field_0xf00 & 4) != 0)
                break;
            const i8 context = object->character_context;
            if ((CInfo[context].parameter & 8) != 0)
                break;
            if (api.field_0x27d == 0 &&
                (context == -1 || (context != LEGOCONTEXT_BEENTAKENOVER && context != LEGOCONTEXT_EATEN &&
                                   context != LEGOCONTEXT_GETIN)) &&
                VehicleArea == 0 && object->id != id_TRAININGREMOTE) {
                const u32 flags = api.character_data->model_flags;
                if (((flags & 0x2000) == 0 ||
                     !(static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->field_0x28 > 0.0f)) &&
                    ((flags & 0x8000) == 0 || object->field_0xe31 != 1))
                    break;
            }
            if (!(object->surface_normal.y > 0.9f) || api.field_0x218 == 2000000.0f)
                break;
            const i16 platform = object->field_0x1078;
            i16 shadow_platform;
            if (!((platform == -1 || platform == LevSafePlatID[0] || platform == LevSafePlatID[1]) &&
                  ((shadow_platform = api.supporting_platform_id) == -1 || shadow_platform == LevSafePlatID[0] ||
                   shadow_platform == LevSafePlatID[1])) &&
                (LastSafePosExtraFn == NULL || !LastSafePosExtraFn(object)))
                break;
            if (!(api.lower_position.y > api.field_0x218 - 0.1f) || unsafe ||
                (VehicleArea != 0 && TouchHacks::CheckForAboutToRunIntoKillTerrain(*object, 1.0f)))
                break;
            object->field_0xf02 |= 0x40;
            api.last_safe_position = api.position;
            object->fall_recovery_timer = 0.0f;
            if ((object->ai.path_info.flags & 1) != 0 && (object->ai.path_info.connection->traversal_flags[1] |
                                                          object->ai.path_info.connection->traversal_flags[0]) == 0) {
                ClearLastSafeTakeoverSource(object);
                api.respawn_position = api.last_safe_position;
                api.flags_high |= 0x20;
            }
            if (static_cast<u8>(api.field_0x27c) < 2 && object->field_0xda8 <= 0.0f &&
                GameCam->blend_time >= GameCam->blend_duration && object->sock_position.location.sock != -1) {
                ClearLastSafeTakeoverSource(object);
                object->last_safe_camera_position = api.last_safe_position;
            }
            goto safe_position_done;
        } while (false);
        {
            i8 edge_layer;
            if ((api.flags_low & 0x80) != 0 && (edge_layer = api.field_0x27f) != -1 &&
                (TerLayer[edge_layer].flags & 1) != 0 && !NoLayerKill(object)) {
                if (object->fall_recovery_timer <= 0.0f)
                    object->fall_hover_height = api.position.y;
                object->fall_recovery_timer += FRAMETIME;
            } else {
                object->fall_recovery_timer = 0.0f;
            }
            if ((WORLD->current_level->flags & 0x40000) != 0) {
                api.last_safe_position = api.position;
            }
        }
    }
safe_position_done:
    if ((object->field_0xf03 & 1) == 0 && (api.flags_low & 0x80) == 0 &&
        (LEGOCONTEXT_EATEN == -1 || object->character_context != LEGOCONTEXT_EATEN) && (api.flags_high & 0x20) != 0) {
        if ((object->ai.path_info.flags & 1) != 0)
            ClearLastSafeTakeoverSource(object);
        api.last_safe_position = api.respawn_position;
    }
    object->field_0xf00 &= ~4;
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

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 hit_effect) {
    if ((object->movement_runtime_flags & 0x10) != 0) {
        return;
    }
    WORLDINFO *world = WorldInfo_CurrentlyActive();

    const u8 player_flag = object->apiobj.flags_low & 0x80;
    i32 flip_kind = 0;
    bool force_flip = false;
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
                    if ((object->apiobj.character_data->model_flags & 8) == 0)
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
        if (jump_pressed != 0 && object->character_context != LEGOCONTEXT_LAND_LUNGE &&
            object->character_context != LEGOCONTEXT_LAND_SLAM &&
            object->character_context != LEGOCONTEXT_LAND_COMBATROLL)
            goto jump_takeoff;
        if (object->character_context != LEGOCONTEXT_LAND_LUNGE && object->character_context != LEGOCONTEXT_LAND_SLAM &&
            ((object->apiobj.flags_low & 0x80) != 0 || object->character_context != LEGOCONTEXT_LAND_JUMP ||
             object->context_animation == -1 ||
             (object->context_animation != LEGOACT_FALLLAND &&
              object->context_animation != LEGOACT_BACKPACKFALLLAND)) &&
            (object->character_context != LEGOCONTEXT_LAND_COMBATROLL ||
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
        if (object->context_animation_timer > 0.0f && object->character_context == LEGOCONTEXT_LAND_SLAM) {
            i32 count = ParticlesPerSecond(25.0f, FRAMETIME);
            NUVEC start, end;
            FindSlamOrigin(object, &start, &end);
            AddGameDebrisRot(world->debris_sys, hit_effect, &start, ParticlesPerSecond(20.0f, FRAMETIME), 0, 0);
            while (count > 0) {
                const f32 t = qrand() * 1.5259022e-05f;
                NUVEC position;
                position.x = (end.x - start.x) * t + start.x;
                position.y = (end.y - start.y) * t + start.y;
                position.z = (end.z - start.z) * t + start.z;
                AddGameDebrisRot(world->debris_sys, hit_effect, &position, 1, 0, 0);
                --count;
            }
            return;
        }
        if (!(object->context_animation_timer > 0.0f)) {
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
                (object->delayed_turn_timer <= 0.0f && !force_flip && (object->field_0xef9 & 1) == 0)) {
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
            object->character_context = -1;
            object->jump_reentry_timer = 0.0f;
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
            if (DoubleJump_JediSlam != 0 && (animation_set & 0x10) != 0 &&
                (touch_action || (action_pressed != 0 && vertical_speed > -1.25f && variant_flags >= 0)) &&
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
                        object->context_variant_flags &= ~0x80;
                        object->context_animation = LEGOACT_COMBATROLL_JUMP;
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
        if (restart_jump && (object->action_movement_state == 1 || object->action_movement_state == 2) &&
            object->pad_gamepad->input_magnitude > 0.0f && (object->field_0xe22 & 0x20) != 0) {
            i32 difference = RotDiff(object->apiobj.movement_facing_angle, object->current_input_angle);
            if (difference < 0)
                difference = -difference;
            force_flip = object->action_movement_state == 1 ? difference > 0x6aaa : difference < 0x1555;
        }
        {
            if (object->action_movement_state == 3) {
                if (LEGOCONTEXT_LAND_LUNGE != -1 && LEGOACT_LUNGELAND != -1 &&
                    object->apiobj.character_model->model_data_b[LEGOACT_LUNGELAND] != NULL) {
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
                } else {
                    if ((object->apiobj.character_data->model_flags & 8) == 0) {
                        Punch_Hit(object, object->force_target, PUNCHCHARGAP, PUNCHCHARGAP);
                        object->character_context = -1;
                        object->jump_reentry_timer = 0.2f;
                        if (object->action_movement_state == 5)
                            object->jump_chain_timer = 0.0f;
                    } else {
                        object->character_context = -1;
                        object->jump_reentry_timer = 0.2f;
                    }
                    PlayLandSfx(object, 0, 0);
                }
            } else if (object->action_movement_state == 4) {
                if (LEGOCONTEXT_LAND_SLAM != -1 && LEGOACT_SLAMLAND != -1 &&
                    object->apiobj.character_model->model_data_b[LEGOACT_SLAMLAND] != NULL) {
                    GameCam_Judder(GameCam, 0.5f, 0, &object->apiobj.collision_position);
                    NewRumble(object->pad_gamepad->pad, 0.75f, 0);
                    object->slam_debris_effect = hit_effect;
                    if (Slam_GetDebrisFn != NULL)
                        object->slam_debris_effect = Slam_GetDebrisFn(object, hit_effect);
                    object->character_context = LEGOCONTEXT_LAND_SLAM;
                    object->context_animation = LEGOACT_SLAMLAND;
                    object->context_animation_timer = AnimDuration(object->id, LEGOACT_SLAMLAND, 0.0f, 0.0f, 1);
                    if (object->context_animation_timer <= 0.0f)
                        object->context_animation_timer = 0.75f;
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
            } else if (object->action_movement_state == 8) {
                if (LEGOCONTEXT_LAND_COMBATROLL != -1 &&
                    object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_LAND] != NULL) {
                    NewRumble(object->pad_gamepad->pad, 0.4f, 0);
                    object->character_context = LEGOCONTEXT_LAND_COMBATROLL;
                    object->field_0x7a3 = 0;
                    object->context_animation = LEGOACT_COMBATROLL_LAND;
                    object->context_animation_timer = AnimDuration(object->id, LEGOACT_COMBATROLL_LAND, 0.0f, 0.0f, 1);
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
                    object->character_context = -1;
                    object->jump_reentry_timer = 0.2f;
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
            } else if (object->action_movement_state == 1 && LEGOCONTEXT_LAND_FLIP != -1 && LEGOACT_FLIPLAND != -1 &&
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
                if (object->action_movement_state == 0 && object->jump_sequence >= 2 && LEGOCONTEXT_LAND_JUMP2 != -1) {
                    i16 action = LEGOACT_LAND2;
                    if ((object->context_variant_flags & 0x40) != 0 &&
                        (action == -1 || object->apiobj.character_model->model_data_b[action] == NULL))
                        action = LEGOACT_LAND3;
                    second_landing = action != -1 && object->apiobj.character_model->model_data_b[action] != NULL;
                }
                if (second_landing) {
                    object->jump_reentry_timer = 0.0f;
                    if (object->pad_gamepad->input_magnitude != 0.0f) {
                        object->character_context = -1;
                    } else if (object->fall_animation_timer >= 0.2f) {
                        StartFallLand(object, -1);
                    } else {
                        object->character_context = LEGOCONTEXT_LAND_JUMP2;
                        if ((object->context_variant_flags & 0x40) != 0 && LEGOACT_LAND3 != -1 &&
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
                        if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_LAND != -1 &&
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
                    object->character_context = -1;
                    object->jump_reentry_timer = 0.2f;
                    if (object->action_movement_state == 5)
                        object->jump_chain_timer = 0.0f;
                }
                PlayLandSfx(object, 0, 0);
            }
        }
        if (restart_jump) {
            UpdateLastSafePosition(object);
            goto jump_takeoff;
        }
        return;
    }
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
        NUVEC *position = joint == -1 ? &target->apiobj.collision_position
                                      : reinterpret_cast<NUVEC *>(&target->joint_matrices[joint].m30);
        SetHeadTarget(object, position, 2, 1.0f, 0.0f, 0.0f);
    }
}

void Slide_SetTargetMom(GameObject_s *, u16, float) {
}

i32 StepBackFromTarget(GameObject_s *object) {
    NUVEC *target_position;
    float inner_distance;
    float outer_distance;
    if (object->blowup_target != NULL) {
        target_position = &object->blowup_target->mid_position;
        if ((LEGOCONTEXT_COMBO != -1 && LEGOCONTEXT_COMBO == object->character_context) ||
            (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == object->character_context &&
             object->action_movement_state == 3)) {
            float target_radius = object->blowup_target->target_scale;
            inner_distance = object->apiobj.collision_radius * 1.75f + target_radius;
            outer_distance = object->apiobj.collision_radius * 2.25f + target_radius;
        } else {
            inner_distance = object->apiobj.collision_radius * 0.75f + object->blowup_target->target_scale;
            outer_distance = object->apiobj.collision_radius * 1.25f + object->blowup_target->target_scale;
        }
    } else {
        GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if ((character->flags_090 & 0x8000) == 0 || object->force_target == NULL) {
            return 0;
        }
        if ((LEGOCONTEXT_COMBO == -1 || LEGOCONTEXT_COMBO != object->character_context) &&
            (LEGOCONTEXT_JUMP == -1 || LEGOCONTEXT_JUMP != object->character_context ||
             object->action_movement_state != 3)) {
            return 0;
        }
        float target_radius = object->force_target->apiobj.field_0x1dc;
        target_position = &object->force_target->apiobj.collision_position;
        inner_distance = object->apiobj.field_0x1dc * 1.75f + target_radius;
        outer_distance = object->apiobj.field_0x1dc * 2.25f + target_radius;
    }
    float speed = AnimSpeed(object->apiobj.character_model, object->context_animation);
    if (speed != 0.0f) {
        float distance_squared = NuVecXZDistSqr(&object->apiobj.collision_position, target_position, NULL);
        if (!(distance_squared >= outer_distance * outer_distance)) {
            float blend = 1.0f;
            if (!(inner_distance * inner_distance >= distance_squared)) {
                blend = 1.0f - (NuFsqrt(distance_squared) - inner_distance) / (outer_distance - inner_distance);
            }
            speed = (-speed - speed) * blend + speed;
            object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * speed;
            object->target_velocity.z = speed * NU_COS_LUT(object->apiobj.movement_facing_angle);
            return 1;
        }
    }
    return 0;
}

float FindGunshipHoverHeight(GameObject_s *object) {
    float height = 2.0f;
    if (object->apiobj.field_0x27c < 2) {
        float upper_height;
        float lower_height;
        if (WORLD->current_level == BONUS_GUNSHIPA_LDATA) {
            upper_height = 2.8f;
            lower_height = 1.2f;
        } else {
            upper_height = 2.05f;
            lower_height = 0.45f;
            height = 1.25f;
        }
        if (Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.flags_low) < 0 && Player[1] != NULL &&
            static_cast<i8>(Player[1]->apiobj.flags_low) < 0) {
            height = object != Player[0] ? lower_height : upper_height;
        }
    }
    return height;
}

void ApplyGravity_Network(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    if (character->model_flags == 0x2000) {
        GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
        if ((data->flags_094[0] & 0x20) == 0) {
            if (data->field275_0x116 == 0x15) {
                ApplyGravity(object, NULL, FindGunshipHoverHeight(object), 8.0f, NULL);
            } else {
                float height;
                if (data->field_0x28 > 0.0f && ((height = GetVehicleHoverHeight(object, NULL)) != 0.0f ||
                                                getvehiclehoverheight_hothbattlehack != 0)) {
                    ApplyGravity(object, NULL, height, 10.0f, NULL);
                } else {
                    height =
                        object->character_context == 0x17
                            ? 0.0f
                            : static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28;
                    ApplyGravity(object, NULL, height, 8.0f, NULL);
                }
            }
        }
    } else {
        if (object->apiobj.field_0x27d != 0 ||
            static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field_0x48 <= object->jump_variant_timer ||
            object->apiobj.field_0x281 == 0x1a) {
            if (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field275_0x116 == 2 &&
                object->field_0xe31 == 1) {
                ApplyGravity(object, NULL, static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field_0x28, 8.0f,
                             NULL);
            } else {
                ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
            }
        }
    }
}

void VehicleCollisionCode(GameObject_s *) {
}

float VehicleTurnOrLoopOffset(GameObject_s *object) {
    if (object->character_context == 0x2a) {
        return (1.0f -
                NU_SIN_LUT((1.0f - object->context_animation_timer / object->airborne_action_duration) * 65536.0f +
                           16384.0f)) *
               0.5f * static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x88 * 0.5f;
    } else if (object->character_context == 0x36) {
        return (1.0f -
                NU_SIN_LUT((1.0f - object->context_animation_timer / object->airborne_action_duration) * 65536.0f +
                           16384.0f)) *
               0.5f * static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x88;
    } else if (object->character_context == 0x3a) {
        if (static_cast<i8>(object->apiobj.flags_low) < 0 && PODSPRINT_ADATA != NULL &&
            PODSPRINT_ADATA == WORLD->area) {
            return NU_SIN_LUT((1.0f - object->context_animation_timer / object->airborne_action_duration) * 32768.0f) *
                   0.25f;
        }
    }
    return 0.0f;
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
        if (object->context_animation_timer > 0.0f)
            object->context_animation_timer -= FRAMETIME;
        else if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) == 0)
            object->character_context = -1;
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
    if (CanStartHoldFn == NULL)
        return;
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

static void GrabCode(GameObject_s *object) {
    if (object->character_context != 0x38)
        return;
    f32 previous_time = object->context_animation_timer;
    f32 time = FRAMETIME + previous_time;
    object->context_animation_timer = time;
    f32 duration = object->airborne_action_duration;
    if (time >= duration) {
        object->character_context = -1;
        return;
    }
    GameObject_s *target = object->force_target;
    i16 animation = object->context_animation;
    if (animation == 0x1f) {
        if (target == NULL)
            return;
    } else if (target == NULL) {
        object->character_context = -1;
        return;
    }
    if ((object->context_flags & 0x40) != 0)
        return;
    bool hit = false;
    if (object->apiobj.character_model->model_data_b[animation] == NULL) {
        f32 halfway = duration * 0.5f;
        if (halfway > previous_time && time >= halfway)
            hit = true;
    } else {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, animation, 1, 0);
        if (frame != NULL) {
            f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
            if (start >= 1.0f && *frame >= start) {
                f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 2);
                if (end >= 1.0f && end >= *frame)
                    hit = true;
            }
        }
    }
    if (!hit)
        return;
    i32 joint = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->weapon_joints[0];
    if (object->apiobj.field_0x288 != 0 && joint != -1 &&
        object->apiobj.character_model->points_of_interest[joint] != NULL &&
        NuVecDistSqr(NUMTX_GET_ROW_VEC(&object->joint_matrices[joint], 3), &target->apiobj.collision_position, NULL) >
            0.7f * 0.7f)
        return;
    if (target->id == id_GAMORREANGUARD && object->context_animation == 0x1f) {
        object->context_flags |= 0x40;
        if (target->character_context == 0x1c && target->force_target != NULL) {
            target->force_target->force_target = NULL;
        }
        Player_ClearContext(target, 1);
        Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(target->player_packet));
        target->character_context = 0x39;
        target->context_animation = 5;
        target->force_target = target;
        target->force_throw_target = NULL;
        PlayDieSfx(target);
        object->field_0xe24 |= 1;
    } else if ((static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) ==
               0) {
        object->context_flags |= 0x40;
        GameAudio_PlaySfx(0x4a, &target->apiobj.collision_position, 0, 0);
        objhitobj_noimpactsfx = 1;
        ObjHitObj(object, target, 2, 0, 0, 1);
        u16 angle = NuAtan2D(target->apiobj.collision_position.x - object->apiobj.collision_position.x,
                             target->apiobj.collision_position.z - object->apiobj.collision_position.z);
        f32 momentum = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->field_0x18;
        f32 sine = NU_SIN_LUT(angle);
        f32 cosine = NU_COS_LUT(angle);
        target->apiobj.velocity.x += sine * momentum;
        target->apiobj.velocity.z += cosine * momentum;
    }
}

extern i16 id_RANCOR;
extern i16 id_DEWBACK;
extern i16 id_BIGGUN;
extern i16 id_BANTHA;
extern i16 id_ATAT;
extern i16 id_CATAPULT;
extern i16 id_FIRETRUCK;
extern i32 TERRAINMASK_NONWEAPON;
void PushAway(NUVEC *, f32, NUVEC *, NUVEC *, GameObject_s *, GameObject_s *, f32, u32);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

struct AWKWARDSHAPESPHERE_s {
    i16 *character_id;
    i16 joint;
    u16 flags;
    f32 radius;
};
DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");

static AWKWARDSHAPESPHERE_s AwkwardShapeSphere[] = {
    {&id_RANCOR, 4, 0, 0.3f},    {&id_DEWBACK, 1, 1, 0.2f},  {&id_DEWBACK, 2, 0, 0.125f}, {&id_DEWBACK, 3, 0, 0.1f},
    {&id_DEWBACK, 4, 0, 0.075f}, {&id_BIGGUN, 3, 0, 0.175f}, {&id_BIGGUN, 4, 0, 0.175f},  {&id_BIGGUN, 5, 0, 0.175f},
    {&id_BANTHA, 1, 1, 0.25f},   {&id_ATAT, 7, 1, 1.2f},     {&id_CATAPULT, 1, 0, 0.25f}, {&id_FIRETRUCK, 1, 1, 0.3f},
    {&id_FIRETRUCK, 2, 1, 0.3f}, {NULL, 0, 0, 0.0f},
};

static void AwkwardShapeCode(GameObject_s *object, i32) {
    i32 joints[8] = {};
    u32 flags[8] = {};
    f32 radii[8] = {};
    if (object->apiobj.field_0x288 == 0 || object->apiobj.model_draw_result == 0)
        return;
    if (WORLD->area != NULL && (WORLD->area->flags & 1) != 0 && object->id == id_ATAT)
        return;
    i32 count = 0;
    if (object->id == id_SPEEDERBIKE) {
        if (WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks == 0)
            return;
        joints[0] = 2;
        radii[0] = 0.325f;
        count = 1;
    } else {
        for (AWKWARDSHAPESPHERE_s *sphere = AwkwardShapeSphere; sphere->character_id != NULL && count < 8; ++sphere) {
            if (*sphere->character_id == object->id) {
                joints[count] = sphere->joint;
                flags[count] = sphere->flags;
                radii[count] = sphere->radius;
                ++count;
            }
        }
    }
    for (i32 i = 0; i < count; ++i) {
        if (object->apiobj.character_model->points_of_interest[joints[i]] != NULL) {
            f32 radius = radii[i];
            NUVEC *position = NUMTX_GET_ROW_VEC(&object->joint_matrices[joints[i]], 3);
            PushAway(position, radius, NULL, NULL, NULL, object, 2.0f, 5);
            if (flags[i] != 0 &&
                (static_cast<i8>(object->apiobj.flags_low) < 0 || static_cast<i8>(object->field_0xf00) < 0)) {
                NUVEC displacement;
                NUVEC extension;
                NUVEC hit_position;
                NuVecSub(&displacement, position, &object->apiobj.collision_position);
                NuVecNorm(&extension, &displacement);
                NuVecScale(&extension, &extension, radius);
                NuVecAdd(&displacement, &displacement, &extension);
                if (GameRayCast(&object->apiobj.collision_position, &displacement, radius * 0.5f,
                                TERRAINMASK_NONWEAPON | 0x1f)) {
                    NuVecAdd(&hit_position, &object->apiobj.collision_position, &displacement);
                    PushAway(&hit_position, 10.0f, NULL, NULL, object, NULL, 3.0f, 1);
                }
            }
        }
    }
}

static void CommunicateCode(GameObject_s *object, i32 pressed, i32) {
    i8 context = object->character_context;
    if (context != 0x1a) {
        if (pressed == 0)
            return;
        if (context != -1)
            return;
        if (static_cast<i8>(
                static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[0]) >= 0)
            return;
        if (object->apiobj.character_model->model_data_b[0x6b] == NULL)
            return;
        object->context_animation_timer = 0.0f;
        object->character_context = 0x1a;
        object->context_animation = 0x6b;
        object->airborne_action_duration = AnimDuration(object->id, 0x6b, 0.0f, 0.0f, 1);
        return;
    }
    f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
    if (frame == NULL)
        return;
    f32 *events = AnimListFrameArray(object->apiobj.character_model, object->context_animation);
    if (events != NULL && events[0] >= 1.0f && events[1] >= 1.0f && events[2] >= 1.0f && events[3] >= 1.0f) {
        f32 time = *frame;
        if (!(events[0] >= time)) {
            if (time < events[1])
                object->communicate_blend = (time - events[0]) / (events[1] - events[0]);
            else if (time < events[2])
                object->communicate_blend = 1.0f;
            else if (time < events[3])
                object->communicate_blend = 1.0f - (time - events[2]) / (events[3] - events[2]);
            else
                object->communicate_blend = 0.0f;
        } else
            object->communicate_blend = 0.0f;
    }
    object->context_animation_timer += FRAMETIME;
    if (object->context_animation_timer >= object->airborne_action_duration) {
        object->character_context = -1;
        object->communicate_blend = 0.0f;
        if (static_cast<i8>(object->apiobj.flags_low) < 0 && Cheat_IsOn(10)) {
            GameObject_s *target = Obj;
            for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
                if (ZapTarget(target) && object != target && target->apiobj.field_0x27c == -1 &&
                    (target->field_0xefb & 8) == 0 && !CannotKill(target)) {
                    u32 flags =
                        static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090;
                    if ((flags & 0x40) != 0)
                        continue;
                    i8 context = target->character_context;
                    if (context == 0xf)
                        continue;
                    if (context == 0x3c)
                        continue;
                    if (context == 0x47)
                        continue;
                    if (context == 0x46)
                        continue;
                    if ((flags & 0x8000) == 0 &&
                        NuVecDistSqr(&object->apiobj.collision_position, &target->apiobj.collision_position, NULL) <
                            9.0f &&
                        DeactivatePlayer(target, DEACTIVATEDTIME, object)) {
                        PlaySfx("R2Zap", &target->apiobj.collision_position);
                        AddGameDebris(WORLD->debris_sys, 1, &target->apiobj.collision_position);
                        GameCam_HitJudder();
                        target->action_movement_state = 2;
                        f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                        target->context_variant_flags |= 1;
                        target->field_0x768 = random * 0.3f + 0.2f;
                        return;
                    }
                }
            }
        }
    }
}

static __used__ void PunchCode(GameObject_s *, i32, i32, i32, i32, f32) {
}

static __used__ void ShootCode(GameObject_s *, i32, i32, i32, i32, i32) {
}

static __used__ void DodgeCode(GameObject_s *, i32, i32) {
}

extern i16 id_EWOK;
extern i16 id_WICKET;
extern i16 id_CAPTAINTARPALS;
extern i16 id_SKELETON;
void StartJetPackFall(GameObject_s *, i32);
void KillGameObject(GameObject_s *, i32, i32);

void Move_CHARACTER(GameObject_s *object) {
    i32 jump = GAMEPAD_JUMP;
    i32 action = GAMEPAD_ACTION;
    i32 special = GAMEPAD_SPECIAL;
    i32 tag = GAMEPAD_TAG;
    GAMEPAD_s *pad = object->pad_gamepad;
    i32 pressed = pad->buttons_pressed;
    i32 held = pad->buttons_held;
    if (object->id == id_GAMORREANGUARD || object->id == id_EWOK || object->id == id_WICKET ||
        object->id == id_CAPTAINTARPALS)
        KeepWeaponOut(object);
    DropInOutCode(object);
    if (BonusArea && VehicleArea && (object->apiobj.character_data->model_flags & 0x4000000) != 0 &&
        object->torpedo != NULL)
        object->torpedo->count = 0;
    if ((object->field_0xe20 & 0x20) != 0)
        return;

    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA *runtime = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    bool jetpack_airborne = false;
    if (static_cast<i8>(object->apiobj.flags_low) < 0 && runtime->uses_weapon_action == 2 &&
        object->character_context != 0x2b && object->field_0xe31 == 1) {
        f32 time = object->apiobj.horizontal_velocity_magnitude / runtime->movement_speed * FRAMETIME +
                   object->jump_variant_timer;
        object->jump_variant_timer = time;
        if (object->apiobj.field_0x27d != 0) {
            object->field_0xe31 = 0;
            object->character_context = -1;
        } else if (time >= runtime->field_0x48 || object->apiobj.field_0x281 == 0x1a) {
            StartJetPackFall(object, 0);
            character = object->apiobj.character_data;
        } else {
            object->apiobj.velocity.y = 0.0f;
            jetpack_airborne = true;
        }
    }
    if (!jetpack_airborne) {
        runtime = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
        if (runtime->uses_weapon_action == 2 && object->field_0xe31 == 1) {
            ApplyGravity(object, NULL, runtime->field_0x28, 8.0f, NULL);
        } else
            ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    }
    if (object->suit != NULL)
        Signal_MoveCode(WORLD, object);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    Hang_MoveCode(object);
    Ledge_MoveCode(WORLD, object);
    if (LedgeTerrain_On)
        LedgeTerrain_MoveCode(object);
    i32 jump_pressed = jump & pressed;
    Climb_MoveCode(object);
    TightRope_MoveCode(object, jump_pressed);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    BackFlipCode(object);
    if (object->id != id_SKELETON)
        TakeOverCode(object, tag & pressed);
    i32 action_pressed = action & pressed;
    i32 action_held = action & held;
    Glide_MoveCode(object);
    runtime = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (runtime->uses_weapon_action == 2 && object->field_0xe31 == 1) {
        JetPackCode(object, jump_pressed, 0, 0);
    } else if ((runtime->flags_094[0] & 4) == 0) {
        u32 animations = object->id == id_GAMORREANGUARD ? 0x20 : 8;
        if ((runtime->flags_090 & 0x400000) != 0)
            animations |= 0x101;
        JumpCode(object, jump_pressed, jump & held, animations, action_pressed, action_held, -1);
    }
    i32 special_pressed = special & pressed;
    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    if ((object->apiobj.character_data->model_flags & 0x100000) != 0)
        Grapple_MoveCode(object);
    BuildIt_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    ThermalDetonator_MoveCode(object);
    if ((object->apiobj.character_data->model_flags & 0x40000) != 0)
        Teleport_MoveCode(object, special_pressed);
    SuperCarry_MoveCode(WORLD, object);
    GizmoBlowupCheckProximity(WORLD, object);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    SpecialMove_VictimCode(object);
    HoldCode(object);
    BlockCode(object, action_pressed, action_held, 0, (object->apiobj.character_data->model_flags & 0x80) == 0);
    PunchCode(object, action_pressed, action_held, object->apiobj.character_data->model_flags & 0x80, 0, 0.0f);
    DodgeCode(object, action_pressed, jump_pressed);
    Attracto_MoveCode(WORLD, object);
    SecurityDoor_MoveCode(WORLD, object);
    if ((object->apiobj.character_data->model_flags & 0x80) != 0) {
        ShootCode(object, action_pressed, special_pressed, 1, 1, 0);
    } else if (object->batarang != NULL)
        Batarang_MoveCode(object);
    if (static_cast<i8>(static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[0]) <
        0) {
        CommunicateCode(object, GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed, 0);
    }
    if (object->character_context == 0x14) {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame != NULL) {
            if ((object->field_0xe22 & 4) == 0) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (event >= 1.0f && event <= *frame) {
                    object->field_0xe22 |= 4;
                    object->field_0xe20 |= 0x10;
                    PlaySfx("JgRocket", &object->apiobj.collision_position);
                }
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f)
                object->character_context = -1;
        }
    } else if (object->character_context == -1 && (object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0 &&
               ((object->apiobj.character_data->model_flags & 0x1000000) != 0 || object->field_0x108e == 6)) {
        FireBountyHunterRocket(object);
    }
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0) {
        if ((object->movement_runtime_flags & 4) != 0 ||
            (object->fall_animation_timer >= 0.2f && (object->pad_gamepad->input_magnitude == 0.0f ||
                                                      (static_cast<i8>(object->apiobj.flags_low) >= 0 &&
                                                       object->apiobj.character_model->model_data_b[0x59] != NULL))))
            StartFallLand(object, -1);
    }
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    if (object->character_context == 0x3f) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer > 0.0f) {
            f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (frame != NULL) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (event >= 1.0f && event <= *frame) {
                    if (object->force_target != NULL)
                        KillGameObject(object->force_target, 2, 0);
                    object->field_0xe24 &= ~1;
                    object->context_flags |= 0x40;
                }
            }
        } else {
            object->character_context = -1;
            if ((object->context_flags & 0x40) == 0) {
                if (object->force_target != NULL)
                    KillGameObject(object->force_target, 2, 0);
                object->field_0xe24 &= ~1;
                object->context_flags |= 0x40;
            }
        }
    }
    GrabCode(object);
    if (object->id == id_RANCOR)
        AwkwardShapeCode(object, 0);
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
}

static __used__ void PooCode(GameObject_s *) {
}

void Buck_MoveCode(GameObject_s *, i32);

void Move_BEAST(GameObject_s *object) {
    DropInOutCode(object);
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    KeepWeaponOut(object);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Buck_MoveCode(object, 0);
    DeactivatedCode(object);
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0) {
        if ((object->movement_runtime_flags & 4) != 0 ||
            (object->fall_animation_timer >= 0.2f && (object->pad_gamepad->input_magnitude == 0.0f ||
                                                      (static_cast<i8>(object->apiobj.flags_low) >= 0 &&
                                                       object->apiobj.character_model->model_data_b[0x59] != NULL))))
            StartFallLand(object, -1);
    }
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    if (object->character_context == 0x3f) {
        object->context_animation_timer -= FRAMETIME;
        bool hit = false;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
            hit = (object->context_flags & 0x40) == 0;
        } else {
            f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (frame != NULL) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                hit = event >= 1.0f && event <= *frame;
            }
        }
        if (hit) {
            if (object->force_target != NULL)
                KillGameObject(object->force_target, 2, 0);
            object->field_0xe24 &= ~1;
            object->context_flags |= 0x40;
        }
    }
    GrabCode(object);
    PunchCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed, 0, 0, 1,
              static_cast<i8>(object->apiobj.flags_low) < 0 ? 1.0f : 2.0f);
    PooCode(object);
    AwkwardShapeCode(object, 0);
    GizmoBlowupCheckProximity(WORLD, object);
}
