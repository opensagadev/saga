#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/speederchase.h"
#include "batman.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/core/legoai.h"
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/collect/bolts.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuvec.h"

extern i32 Hub_GetRandomCharType();

extern void *perm_debrissys;

void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay);

void AddLevSfx(WORLDINFO_s *world, nuvec_s *position, char *name, i32 sfx);

void ResetForceBack();

void AlertSurroundingCreatures(GameObject_s *object, NUVEC *position);

void LetGoOfBalloon(GameObject_s *object);

void GizmoPickup_TurnOnPickup(GIZMOPICKUP_s *pickup);

void SetForceBack(GameObject_s *object, NUVEC *position, f32 radius, i32 type);

void AddGameMsgCount(NUVEC *position, i32 count, i32 total, u8 red, u8 green, u8 blue, f32 duration);

extern void CurrentStart(GameObject_s *object, i32 mode, i32 start);

extern "C" void ComplexSockAngles(SOCKROT *angles);

extern void oneAtOnce_SetInitDistPerRow(f32 distance);

extern i32 oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent);

extern f32 oneAtOnce_GetHoldRange(GameObject_s *object);

extern void Hint_CancelCurrent();

extern i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode);

extern void SetPlayer();

extern void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count, f32 interval, nuvec_s *position);

extern void ResetAICreature(GameObject_s *object, AISYS_s *system);

extern void DeactivateGameObject(GameObject_s *object);

extern void Player_ClearContext(GameObject_s *object, i32 mode);

extern void Player_ResetContexts(PLAYERPACKET_s *packet);

extern EXPLOSION *Detonate(NUVEC *position, u16 flags);

extern void EatVictim(GameObject_s *object);

extern void GrabVictim(GameObject_s *object, GameObject_s *victim);

extern void ReleaseEat(GameObject_s *object);

extern void ReleaseTakeOver(GameObject_s *object, i32 immediate);

extern void TakeOverGameObject(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera, i32 immediate);

extern void FindForcePushTarget(GameObject_s *object, i32 held, i32 mode);

extern void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, f32 distance, i32 looping);

extern void KillParts(GameObject_s *object, i32 part, i32 joint, i32 visible, f32 velocity, i32 flags, u16 *part_ids);

extern u32 StarWars_ParseAIPathCnxFlag(char *name);

extern AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system,
                                                        AIPATH_s *path, char *from, char *to, i32 target_type,
                                                        char *target_name, i32 fake_animation_id, i32 gizmo_output);

extern void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame);

extern void AIPathCnxSetTemporaryBlock(AIPATH_s *path, char *from_name, char *to_name, i32 blocked);

extern AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(AIPATHCNXHELPERSYS_s *system, AIPATHCNX_s *connection,
                                                       u8 direction, void *target, u8 type);

extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);

extern ADDPART_s Default_ADDPART;

extern f32 ForceThrowSpeed, ForceThrowGravity;

extern "C" PART_s *AddPart(ADDPART_s *);

void PartCollide_3D(PART_s *);

void MakeThrowVector(NUVEC *, NUVEC *, NUVEC *, NUVEC *, f32, f32);

static i32 Action_SetCurrentSpeed(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_SetState(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_FollowPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UsePanel(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_PullLever(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UseTechno(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_HelpWithTriggers(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UseTriggerSet(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_GoToOriginalPath(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_BigJumpToLocator(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_UseBigJumpToJump(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_CatchUpForbidden(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_JudderGameCamera(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_ReleaseTakeOver(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetHintComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetAnimSpeedMul(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_ShootAtOpponent(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetInvulnerable(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_SetVisibility(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_PressJumpButton(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetControlSystem(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_FollowDirection(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_MoveAwayFromLastAttacker(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

enum LEVEL_PROGRESS_LAYOUT : isize {
    LEVEL_PROGRESS_STRIDE = 0x2e24,
    LEVEL_PROGRESS_COMPLETION_FLAGS_OFFSET = 0x2800,
    LEVEL_PROGRESS_STORY_COMPLETE = 1 << 0,
};

enum CREATE_CREATURE_LIMITS {
    CREATE_CREATURE_MAX_LOCATORS = 64,
    CREATE_CREATURE_MAX_MODELS = 10,
};

union AI_CONDITION_LEVEL_ARGUMENT {
    void *pointer;
    isize value;
};

void LevelScriptReStoreProgress(WORLDINFO_s *, LEVELSCRIPTPROCESS_s *);

extern void oneAtOnce_SetDistPerRow(f32);

extern void oneAtOnce_SetNumAttackers(i32);

extern void oneAtOnce_SetAttackersPerRow(i32);

static i32 Action_EngageOpponent(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static inline i32 PartyMemberInWay(GameObject_s *, GameObject_s *);

AILOCATOR *LocalGetRandomLocator(AILOCATOR **, i32, f32, NUVEC *, f32, i32, f32, f32);

AILOCATOR *LocalGetNearestLocator(AILOCATOR **, i32, f32, NUVEC *, f32, i32, f32, f32);

i16 GetGenericGoon(i32);

extern "C" AIGROUP *CreateAIGroup(AISYS *, i32, f32, f32, f32);

void PlayJumpSfx(GameObject_s *, i32);

void SetWeaponOut(GameObject_s *);

void StartBallooning(GameObject_s *, i32);

void SetBallooningHeight(GameObject_s *, f32);

void SpawnCreatureFromCrate(GameObject_s *, f32, f32);

void PartKill_ForceThrow(PART_s *, i32);

extern i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);

extern TERRSET *CurTerr;

extern "C" i32 party_under_cover;

extern FLOWBOX_s *FlowBoxFindByName(GIZFLOW_s *, char *);

i32 Action_SetState(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_FollowPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

extern "C" void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *end, u32 size);

extern "C" f32 default_path_heighttol;

extern "C" i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *cutscene);

extern i32 Hub_GetRandomCharType();

extern u8 hub_custodians_finished_loading;

extern void GameObjectOrigin(GameObject_s *object);

extern void PodSprint_GetIAlongVals(nugspline_s *spline, i16 *first_point, i16 *last_point);

extern void TakeOverGameObject(GameObject_s *rider, GameObject_s *mount, i32 seat, i32 immediate);

extern NUVEC plr_lastpos;

enum AI_CREATURE_SET : isize {
    AI_CREATURE_SET_CURRENT = -1,
    AI_CREATURE_SET_NONE = 0,
    AI_CREATURE_SET_FIRST = 1,
    AI_CREATURE_SET_LAST = 16,
};

i32 Action_GameFollowPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);

static isize AIConditionArgumentValue(void *argument) {
    AI_CONDITION_LEVEL_ARGUMENT condition_argument = {};
    condition_argument.pointer = argument;
    return condition_argument.value;
}

static inline void ActionApplySide(AISYS *system, GameObject_s *object, i32 side) {
    if (object == NULL) {
        return;
    }

    const u32 old_state = object->apiobj.field_0x1f4;
    object->apiobj.field_0x1f4 = old_state & 0xfffefffau;
    switch (side) {
        case 1:
            object->apiobj.field_0x1f4 |= 1;
            break;
        case 0:
            object->apiobj.field_0x1f4 |= 4;
            break;
        case 2:
            object->apiobj.field_0x1f4 |= 0x10000;
            break;
        case 3:
            if ((old_state & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) != 0 && object->ai.field_0x134 != 0xff) {
                const i32 type = system->creatures[object->ai.field_0x134].type;
                const u32 model_flags = apicharsys->char_data[type].model_flags;
                if ((model_flags & 0x200) != 0) {
                    object->apiobj.field_0x1f4 |= 4;
                } else if ((model_flags & 4) != 0) {
                    object->apiobj.field_0x1f4 |= 1;
                }
            }
            break;
    }
    const u32 new_state = object->apiobj.field_0x1f4;

    object->ai.capabilities &=
        ~(static_cast<u32>(LEGO_AIPATHCNX_FORGOODIES) | static_cast<u32>(LEGO_AIPATHCNX_FORBADDIES));
    if ((new_state & 1) != 0) {
        object->ai.capabilities |= LEGO_AIPATHCNX_FORBADDIES;
    } else if ((new_state & 4) == 0) {
        object->ai.capabilities |= LEGO_AIPATHCNX_FORGOODIES;
    }

    if ((old_state & new_state & 0x10005) == 0) {
        object->ai.field_0x1e5 &= static_cast<u8>(~8u);
        object->ai.opponent = NULL;
        object->alert_target = NULL;
        object->alert_target_timer = 0.0f;
    }
}

static void ResetAIOverrideCharacter(GameObject_s *object) {
    object->field_0x101c = 0.0f;
    object->field_0x1014 = 0;
    if (object->apiobj.field_0x287 != 0) {
        object->apiobj.field_0x287 = 0;
        object->current_hp = object->hitpoints;
        object->field_0xe37 = object->apiobj.character_data->game_character->field_0xf5;
        object->field_0xe38 = 4;
    }
}

static GameObject_s *ActionOwner(AIPACKET_s *packet) {
    return packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
}

static f32 Condition_GlynTest(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
static f32 Condition_Debug(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
static f32 Condition_Active(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_GotGun(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_PrefersBrawling(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_MySet(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *);
static f32 Condition_TurretAlive(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_IAmAPartyCharacter(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_IAmA(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_OpponentIsA(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_OpponentIsAThreat(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static f32 Condition_IAmAGoody(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_IAmABaddy(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_IAmANeutral(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_IAmAGoodieBaddie(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static void *Condition_EitherPlayerIsInit(AISYS *sys, char *arg, AISCRIPT *);
static f32 Condition_EitherPlayerIs(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_Player1Is(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_Player2Is(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_BuildItComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static void *Condition_IsAliveInit(AISYS *sys, char *arg, AISCRIPT *);
static f32 Condition_IsAlive(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_OffScreenTimer(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_OnGround(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_BeenAlerted(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_EitherPlayerOnObject(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_OnObject(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_SpawnCount(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_BeenHit(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_HoverPhase(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_Colliding(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_XPos(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_YPos(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_ZPos(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_Context(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static __used__ void *Condition_NumBaddiesInit(AISYS_s *, char *, AISCRIPT_s *);
static f32 Condition_NumBaddies(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
static f32 Condition_BeenToLevel(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *void_arg);
static f32 Condition_PartyUnderCover(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
static f32 Condition_TakeOverTargetInTriggerArea(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                                 void *argument);
static f32 Condition_CharacterExists(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_HasTakeOver(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_GotVictim(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_ScreenWipe(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
static __used__ f32 Condition_HeadTurnRestricted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);
static f32 Condition_Side(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
static f32 Condition_FlowBoxComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_AnimSpeedMul(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_CanHearRadio(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
static f32 Condition_BeingTowed(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_AreaComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_InSwamp(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static __used__ f32 Condition_NetworkGameOnGoing(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);
static f32 Condition_IsLowEndDevice(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
__used__ static i32 Action_PressTagButton(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static i32 Action_PressActionButton(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32, f32);
__used__ static i32 Action_UseWeapon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_PressJumpButton(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_BreakFormation(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_FormationMove(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetHint(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
__used__ static i32 Action_SetHintComplete(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CanPullLevers(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_DontAttack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
static i32 Action_CanMoveWhenDeactivated(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                         i32, f32);
static i32 Action_IgnoreTurnAroundSpline(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                         i32, f32);
static i32 Action_SetShootOpponents(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                    i32 first_time, f32);
static i32 Action_CanCollideWithObjects(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                        i32 first_time, f32);
static i32 Action_CanHelpWithTriggers(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                      i32 first_time, f32);
static i32 Action_UseTimeBasedUpdate(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32);
static i32 Action_DisableNarrowSocks(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count, i32, f32);
__used__ static i32 Action_Respawnable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_ImmuneToBolts(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
static i32 Action_ImmuneToKillTerrain(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                      i32 first_time, f32);
__used__ static i32 Action_CanTurn(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
static i32 Action_CannotBeForcedBack(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_CannotBeSeen(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
static i32 Action_IgnoreShoveSystem(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                    i32 first_time, f32);
__used__ static i32 Action_ApplyGravity(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetHoverPhase(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetControlSystem(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
static i32 Action_PressSpecialButton(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_count, i32 first_time, f32);
static i32 Action_DeflectPlayersPart(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_UpdateSockPos(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetBoss(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
__used__ static i32 Action_CanUseWeapon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_DontAimAt(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
static i32 Action_CharClipToBlobShadows(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                        i32 first_time, f32);
__used__ static i32 Action_CanDefend(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
static i32 Action_PlayerSpeederHack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                    i32, f32);
__used__ static i32 Action_AlwaysBackFlip(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static i32 Action_CanHitForceObjects(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count, i32,
                                     f32);
__used__ static i32 Action_UseBigJumpToJump(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
static i32 Action_CanShootObstructions(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                       i32, f32);
static i32 Action_SetBoltsDontGetDeflectedBack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                               i32 param_count, i32, f32);
__used__ static i32 Action_PrefersPlayers(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static i32 Action_CanShootOffScreen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                    i32 first_time, f32);
__used__ static i32 Action_CanOpenDoors(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
static i32 Action_SetAIOverrideControl(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_count, i32 first_time, f32);
__used__ static i32 Action_SetWalkSpeed(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 num_params, i32 first_time, f32);
__used__ static i32 Action_SetAnimSpeedMul(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
static f32 Condition_BehindCamera(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
__used__ static i32 Action_RaceOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
static i32 Action_MoveAwayFromLastAttacker(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_count, i32 first_time, f32);
static void *Condition_GizmoVisibilityInit(AISYS *, char *name, AISCRIPT *);
static void *Condition_GizmoOutputInit(AISYS *, char *name, AISCRIPT *);
static void *Condition_BlowupInit(AISYS *, char *arg, AISCRIPT *);
static void *Condition_BuildItCompleteInit(AISYS *, char *name, AISCRIPT *);
static void *Condition_TurretAliveInit(AISYS *, char *name, AISCRIPT *);
static void *Condition_LocatorOnScreenInit(AISYS *system, char *name, AISCRIPT *);
static void *Condition_LocatorRangeInit(AISYS *sys, char *arg, AISCRIPT *);
__used__ static i32 Action_WalkBackwards(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_GoToNewLevel(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
static i32 Action_DrawBossHitPoints(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32, f32);
__used__ static i32 Action_LetGoOfBalloon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static void *Condition_TakeOverTargetInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *);
__used__ static i32 Action_SetScaleOverride(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SpinOnSpot(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
static i32 Action_SetGravityHeight(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                   i32 param_count, i32 first_time, f32);
static f32 Condition_TakeOverRange(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
__used__ static i32 Action_ForceLightning(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_ThrowDetonator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_AddTorpedoPacket(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CreateRider(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
static f32 Condition_InSameTriggerAreaAsNearestPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static i32 Action_RemoveThrownForceObjects(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_SetFormationCommander(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32, f32);
__used__ static i32 Action_FaceCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CycleCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static f32 Condition_PlayerCategoryIs(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
__used__ static f32 Condition_CategoryIs(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument);
__used__ static i32 Action_CancelHint(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_UseCurrentSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
static __used__ i32 Action_LaunchGuidedMissile(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params,
                                               i32 param_count, i32 first_time, f32);
__used__ static i32 Action_SetRunSpeed(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 num_params, i32 first_time, f32);
static f32 Condition_NearestPartyXZRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static i32 Action_FollowDirection(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                  i32 param_count, i32 first_time, f32 delta_time);
static f32 Condition_NearestPartyRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static i32 Action_SetSpline(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                            i32 param_5, f32 param_6);
__used__ static i32 Action_Explode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
__used__ static i32 Action_UseForce(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32 first_time, f32 elapsed);
__used__ static i32 Action_ReleaseVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_EatVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_EngageObject(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_AttackOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_AnimTimeRandom(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetAnimation(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
i32 Action_MoveForward(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                       i32 first_time, f32);
static i32 Action_SnapToSockPosition(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_count, i32 first_time, f32);
__used__ static i32 Action_GoToOriginalPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_GoToLevelPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
static void *Condition_AreaCompleteInit(AISYS *, char *name, AISCRIPT *);
static void *Condition_CharacterLoadedInit(AISYS *, char *argument, AISCRIPT *);
static f32 Condition_CharacterLoaded(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static void *Condition_FlowBoxCompleteInit(AISYS *, char *name, AISCRIPT *);
static f32 Condition_GizmoVisibility(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_GizmoOutput3(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_GizmoOutput2(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_GizmoOutput1(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_GizmoOutput0(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_BlowupBlownup(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument);
static f32 Condition_ShopActive(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
static f32 Condition_LocatorOnScreen(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *argument);
static f32 Condition_EitherPlayerLocatorRangeXZ(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                                void *argument);
static i32 Action_ClearTakeOverTarget(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32 first_time, f32);
static i32 Action_PartyCanBeUnderCover(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count,
                                       i32 first_time, f32);
__used__ static i32 Action_DontRaycastLOS(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static void *Condition_SideInit(AISYS *, char *arg, AISCRIPT *);
static void *Condition_BeenToLevelInit(AISYS *system, char *arg, AISCRIPT *);
static void *Condition_OpponentIsAInit(AISYS *sys, char *arg, AISCRIPT *);
static void *Condition_IAmAInit(AISYS *sys, char *arg, AISCRIPT *);
__used__ static i32 Action_SetCanTakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static i32 Action_SetZeroAcceleration(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                      i32 first_time, f32);
static i32 Action_SetLastSafePathPos(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                     i32 param_count, i32 first_time, f32);
static i32 Action_SetDefaultMovementRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                          i32 param_count, i32 first_time, f32);
static i32 Action_SetMaxMovementRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32);
__used__ static i32 Action_SetScriptParam(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                          i32 num_params, i32 first_time, f32);
__used__ static i32 Action_AddToSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_TurnOnPickup(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
static i32 Action_CanTriggerObstacle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count,
                                     i32 first_time, f32);
static i32 Action_AlwaysTriggerObstacle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count,
                                        i32 first_time, f32);
static i32 Action_GizmoSetVisibility(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_GizmoActivate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CircleLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 elapsed);
__used__ static i32 Action_CompleteLevel(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_InitRowDist(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
static i32 Action_SetAtOnceRowDistance(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                       i32 first_time, f32);
static i32 Action_SetAttackersPerRow(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32);
static i32 Action_SetAttackersAtOnce(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_SetLocatorSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
static i32 Action_AddScriptProcessor(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_CopyMessage(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 num_params,
                                       i32 first_time, f32);
__used__ static i32 Action_SetMessage(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 num_params,
                                      i32 first_time, f32);
__used__ static i32 Action_AddGameMsgCount(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetObstacleToEnd(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_PlayGizSpecial(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_PlayGizObstacle(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CnxHelper(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
static u32 ParseAIPathCnxFlag(char *name);
__used__ static i32 Action_CnxController(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 param_6);
static void *Condition_CategoryIsInit(AISYS *system, char *arg, AISCRIPT *);
__used__ static i32 Action_SetLevelPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_ResetGameCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CameraShake(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_JudderGameCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_EnableSock(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_AddPart(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 num_params,
                                   i32 first_time, f32);
__used__ static i32 Action_Launch(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32 first_time, f32);
static i32 Action_SelectRandomSpline(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_SetScriptState(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetStateArea(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_TriggerBlowUp(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
static inline i32 PartyMemberInWay(GameObject_s *object, GameObject_s *opponent);
__used__ static i32 Action_SnapToPosition(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetPathCnxFlag(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32 param_6);
static i32 Action_SetTechnoComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                    i32 first_time, f32);
__used__ static i32 Action_SetUseOneAtOnce(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetLayer(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6);
static i32 Action_SplineFollowTerrain(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32);
static i32 Action_IgnoreSlideTerrain(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32);
static i32 Action_AwkwardShapeOverride(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                       i32 param_count, i32 first_time, f32);
static i32 Action_IgnoreLastSafePathPos(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                        i32 param_count, i32 first_time, f32);
__used__ static i32 Action_CanBeCarried(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_AddMiscPickups(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static i32 Action_SetTakeOverTarget(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32 first_time, f32);
static i32 Action_RegisterTakeOverObject(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32);
__used__ static i32 Action_ReleaseTakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_TakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6);
static i32 Action_LinkTurretToController(AISYS *system, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count,
                                         i32 first_time, f32);
__used__ static i32 Action_SetLastAttacker(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_AlertCreatures(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
static i32 Action_FollowCharacter(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                  i32 param_count, i32 first_time, f32);
__used__ static i32 Action_FaceCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 elapsed);
__used__ static i32 Action_SetForceBack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_PlaySfx(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
__used__ static i32 Action_AddDebris(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_NoIdleSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_AddPartDebris(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                         i32 num_params, i32 first_time, f32);
static i32 Action_SetShieldHitPoints(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_count, i32 first_time, f32);
__used__ static i32 Action_SetHitPoints(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 num_params, i32 first_time, f32);
__used__ static i32 Action_SetCurrentSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 num_params, i32 first_time, f32);
static __used__ i32 Action_CreateSplineCreatures(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet,
                                                 char **params, i32 num_params, i32 first_time, f32);
static i32 Action_DontAvoidCharacter(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_DontPush(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetInvulnerable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
static i32 Action_DontSetStoppedFlag(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32);
__used__ static i32 Action_SetDontMove(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_Kill(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                i32 param_5, f32 param_6);
__used__ static i32 Action_ForcePush(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_GrabVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_ShootAtOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_EngageOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_ProbeDroid(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetSide(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
__used__ static i32 Action_TagCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_TakeDamage(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_NotWithParty(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CanAttack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CannotDropIn(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_CatchUpForbidden(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SetTaggable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_ResetContext(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SnapWeaponOut(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_KeepWeaponOut(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);
static i32 Action_SetDoomedEscapeLocator(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32);
__used__ static i32 Action_BigJump(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
__used__ static i32 Action_BigJumpToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SnapToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_SnapToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32);
__used__ static i32 Action_SetPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6);
__used__ static i32 Action_DeActivate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);
__used__ static i32 Action_Activate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6);
static void *Condition_CharacterExistsInit(AISYS *system, char *name, AISCRIPT *);
static void *Condition_XYZPosInit(AISYS *sys, char *arg, AISCRIPT *);
static void *Condition_BeenHitInit(AISYS *sys, char *arg, AISCRIPT *);
static void *Condition_OffScreenTimerInit(AISYS *system, char *name, AISCRIPT *);
__used__ static i32 Action_CreateCreatures(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_count, i32 first_time, f32);
void GameAIScriptAddLevelSfx(WORLDINFO *world, NULISTHDR *scripts);

extern "C" {
    extern i16 id_BATTLEDROID, id_BATTLEDROIDSECURITY, id_BATTLEDROIDGEONOSIAN;
    extern i16 id_BATTLEDROIDCOMMANDER, id_CLONEEP3, id_CLONEEP3SAND;
}

extern "C" {
    // Keep this registry in the exact order used by the shipped script parser.
    AIACTIONDEF lego_aiactiondefs[] = {
        {"Activate", Action_Activate, 1, 0, 0},
        {"DeActivate", Action_DeActivate, 1, 0, 0},
        {"GoToLevelPath", Action_GoToLevelPath, 0, 0, 0},
        {"SetPath", Action_SetPath, 0, 0, 0},
        {"GoToOriginalPath", Action_GoToOriginalPath, 0, 0, 0},
        {"SnapToLocator", Action_SnapToLocator, 1, 0, 0},
        {"SetLocator", Action_SetLocator, 1, 0, 0},
        {"SetLocatorSet", Action_SetLocatorSet, 0, 0, 0},
        {"SnapToOrigin", Action_SnapToOrigin, 1, 0, 0},
        {"BigJumpToLocator", Action_BigJumpToLocator, 0, 0, 0},
        {"BigJump", Action_BigJump, 0, 0, 0},
        {"SetDoomedEscapeLocator", Action_SetDoomedEscapeLocator, 0, 0, 0},
        {"SnapToPosition", Action_SnapToPosition, 1, 0, 0},
        {"SnapToSockPosition", Action_SnapToSockPosition, 1, 0, 0},
        {"SetAnimation", Action_SetAnimation, 0, 0, 0},
        {"AnimTimeRandom", Action_AnimTimeRandom, 0, 0, 0},
        {"CanOpenDoors", Action_CanOpenDoors, 0, 0, 0},
        {"CanShootOffScreen", Action_CanShootOffScreen, 0, 0, 0},
        {"KeepWeaponOut", Action_KeepWeaponOut, 0, 0, 0},
        {"SnapWeaponOut", Action_SnapWeaponOut, 1, 0, 0},
        {"ResetContext", Action_ResetContext, 0, 0, 0},
        {"PrefersPlayers", Action_PrefersPlayers, 0, 0, 0},
        {"SetBoltsDontGetDeflectedBack", Action_SetBoltsDontGetDeflectedBack, 0, 0, 0},
        {"CanShootObstructions", Action_CanShootObstructions, 0, 0, 0},
        {"UseBigJumpToJump", Action_UseBigJumpToJump, 0, 0, 0},
        {"SetTaggable", Action_SetTaggable, 1, 0, 0},
        {"CatchUpForbidden", Action_CatchUpForbidden, 0, 0, 0},
        {"CannotDropIn", Action_CannotDropIn, 1, 0, 0},
        {"CanAttack", Action_CanAttack, 0, 0, 0},
        {"NotWithParty", Action_NotWithParty, 1, 0, 0},
        {"TakeDamage", Action_TakeDamage, 0, 0, 0},
        {"TagCharacter", Action_TagCharacter, 1, 0, 0},
        {"CanHitForceObjects", Action_CanHitForceObjects, 0, 0, 0},
        {"AlwaysBackFlip", Action_AlwaysBackFlip, 0, 0, 0},
        {"PlayerSpeederHack", Action_PlayerSpeederHack, 0, 0, 0},
        {"SetAnimSpeedMul", Action_SetAnimSpeedMul, 0, 0, 0},
        {"SetSide", Action_SetSide, 1, 0, 0},
        {"SetStateArea", Action_SetStateArea, 0, 0, 0},
        {"SetOpponent", Action_SetOpponent, 0, 0, 0},
        {"AttackOpponent", Action_AttackOpponent, 0, 0, 0},
        {"EngageOpponent", Action_EngageOpponent, 0, 0, 0},
        {"ShootAtOpponent", Action_ShootAtOpponent, 0, 0, 0},
        {"EngageObject", Action_EngageObject, 0, 0, 0},
        {"GrabVictim", Action_GrabVictim, 0, 0, 0},
        {"EatVictim", Action_EatVictim, 0, 0, 0},
        {"ReleaseVictim", Action_ReleaseVictim, 0, 0, 0},
        {"CanDefend", Action_CanDefend, 0, 0, 0},
        {"CharClipToBlobShadows", Action_CharClipToBlobShadows, 0, 0, 0},
        {"DontAimAt", Action_DontAimAt, 0, 0, 0},
        {"CanUseWeapon", Action_CanUseWeapon, 0, 0, 0},
        {"SetBoss", Action_SetBoss, 0, 0, 0},
        {"UpdateSockPos", Action_UpdateSockPos, 0, 0, 0},
        {"UseForce", Action_UseForce, 0, 0, 0},
        {"TriggerBlowUp", Action_TriggerBlowUp, 0, 0, 0},
        {"ForcePush", Action_ForcePush, 0, 0, 0},
        {"DeflectPlayersPart", Action_DeflectPlayersPart, 0, 0, 0},
        {"Kill", Action_Kill, 1, 0, 0},
        {"Explode", Action_Explode, 0, 0, 0},
        {"SetScriptState", Action_SetScriptState, 0, 0, 0},
        {"SetAIOverrideControl", Action_SetAIOverrideControl, 0, 0, 0},
        {"SetLastSafePathPos", Action_SetLastSafePathPos, 0, 0, 0},
        {"SetDontMove", Action_SetDontMove, 0, 0, 0},
        {"DontSetStoppedFlag", Action_DontSetStoppedFlag, 0, 0, 0},
        {"PressSpecialButton", Action_PressSpecialButton, 0, 0, 0},
        {"PressTagButton", Action_PressTagButton, 0, 0, 0},
        {"PressActionButton", Action_PressActionButton, 0, 0, 0},
        {"UseWeapon", Action_UseWeapon, 0, 0, 0},
        {"SetInvulnerable", Action_SetInvulnerable, 0, 0, 0},
        {"DontPush", Action_DontPush, 0, 0, 0},
        {"DontAvoidCharacter", Action_DontAvoidCharacter, 0, 0, 0},
        {"PressJumpButton", Action_PressJumpButton, 0, 0, 0},
        {"AddToSet", Action_AddToSet, 0, 0, 0},
        {"SetSpline", Action_SetSpline, 0, 0, 0},
        {"SetControlSystem", Action_SetControlSystem, 0, 0, 0},
        {"SetZeroAcceleration", Action_SetZeroAcceleration, 0, 0, 0},
        {"FollowDirection", Action_FollowDirection, 0, 0, 0},
        {"BreakFormation", Action_BreakFormation, 0, 0, 0},
        {"FormationMove", Action_FormationMove, 0, 0, 0},
        {"CreateCreatures", Action_CreateCreatures, 0, 0, 0},
        {"SelectRandomSpline", Action_SelectRandomSpline, 0, 0, 0},
        {"CreateSplineCreatures", NULL, 0, 0, 0},
        {"Launch", Action_Launch, 0, 0, 0},
        {"SetCurrentSpeed", NULL, 0, 0, 0},
        {"SetRunSpeed", Action_SetRunSpeed, 0, 0, 0},
        {"SetWalkSpeed", Action_SetWalkSpeed, 0, 0, 0},
        {"SetHitPoints", Action_SetHitPoints, 1, 0, 0},
        {"SetShieldHitPoints", Action_SetShieldHitPoints, 1, 0, 0},
        {"SetMessage", Action_SetMessage, 1, 0, 0},
        {"CopyMessage", Action_CopyMessage, 0, 0, 0},
        {"SetScriptParam", Action_SetScriptParam, 0, 0, 0},
        {"AddPart", Action_AddPart, 0, 0, 0},
        {"AddPartDebris", Action_AddPartDebris, 0, 0, 0},
        {"LaunchGuidedMissile", NULL, 0, 0, 0},
        {"SetHoverPhase", Action_SetHoverPhase, 0, 0, 0},
        {"UseCurrentSpeed", NULL, 0, 0, 0},
        {"SetMaxMovementRange", NULL, 0, 0, 0},
        {"SetDefaultMovementRange", NULL, 0, 0, 0},
        {"SetGravityHeight", NULL, 0, 0, 0},
        {"ApplyGravity", Action_ApplyGravity, 0, 0, 0},
        {"IgnoreShoveSystem", Action_IgnoreShoveSystem, 0, 0, 0},
        {"CannotBeSeen", Action_CannotBeSeen, 0, 0, 0},
        {"CannotBeForcedBack", Action_CannotBeForcedBack, 0, 0, 0},
        {"CanTurn", Action_CanTurn, 0, 0, 0},
        {"NoIdleSpeed", Action_NoIdleSpeed, 0, 0, 0},
        {"SetVisibility", Action_SetVisibility, 1, 0, 0},
        {"EnableSock", Action_EnableSock, 1, 0, 0},
        {"AddDebris", Action_AddDebris, 0, 0, 0},
        {"JudderGameCamera", Action_JudderGameCamera, 0, 0, 0},
        {"CameraShake", Action_CameraShake, 0, 0, 0},
        {"ResetGameCamera", NULL, 1, 0, 0},
        {"PlayCutScene", Action_PlayCutScene, 1, 0, 0},
        {"SetLevelPath", Action_SetLevelPath, 0, 0, 0},
        {"ImmuneToKillTerrain", Action_ImmuneToKillTerrain, 0, 0, 0},
        {"ImmuneToBolts", Action_ImmuneToBolts, 0, 0, 0},
        {"Respawnable", Action_Respawnable, 0, 0, 0},
        {"SetPathCnxFlag", Action_SetPathCnxFlag, 1, 0, 0},
        {"SetHint", Action_SetHint, 0, 0, 0},
        {"SetHintComplete", Action_SetHintComplete, 0, 0, 0},
        {"CancelHint", Action_CancelHint, 0, 0, 0},
        {"CycleCharacter", Action_CycleCharacter, 0, 0, 0},
        {"CnxController", Action_CnxController, 0, 0, 0},
        {"CnxHelper", Action_CnxHelper, 0, 0, 0},
        {"PlaySfx", Action_PlaySfx, 0, 0, 0},
        {"CameraCut", Action_CameraCut, 1, 0, 0},
        {"DynamicCameraCut", NULL, 1, 0, 0},
        {"EndCameraCut", Action_EndCameraCut, 1, 0, 0},
        {"DontRaycastLOS", Action_DontRaycastLOS, 0, 0, 0},
        {"SetForceBack", Action_SetForceBack, 1, 0, 0},
        {"FaceCamera", Action_FaceCamera, 0, 0, 0},
        {"FaceCharacter", Action_FaceCharacter, 0, 0, 0},
        {"SpinOnSpot", Action_SpinOnSpot, 0, 0, 0},
        {"FollowCharacter", NULL, 0, 0, 0},
        {"FollowPlayer", NULL, 0, 0, 0},
        {"MoveForward", NULL, 0, 0, 0},
        {"SetFormationCommander", Action_SetFormationCommander, 0, 0, 0},
        {"RemoveThrownForceObjects", Action_RemoveThrownForceObjects, 0, 0, 0},
        {"AlwaysTriggerObstacle", Action_AlwaysTriggerObstacle, 0, 0, 0},
        {"CanTriggerObstacle", Action_CanTriggerObstacle, 0, 0, 0},
        {"PlayGizObstacle", NULL, 1, 0, 0},
        {"PlayObstacle", NULL, 1, 0, 0},
        {"PlayGizSpecial", Action_PlayGizSpecial, 1, 0, 0},
        {"SetObstacleToEnd", NULL, 1, 0, 0},
        {"HelpWithTriggers", Action_HelpWithTriggers, 0, 0, 0},
        {"UseTriggerSet", Action_UseTriggerSet, 0, 0, 0},
        {"PullLever", Action_PullLever, 0, 0, 0},
        {"UsePanel", Action_UsePanel, 0, 0, 0},
        {"UseTechno", Action_UseTechno, 0, 0, 0},
        {"ReleaseLocator", NULL, 0, 0, 0},
        {"AssignLocator", NULL, 0, 0, 0},
        {"GetLocatorFromSet", NULL, 0, 0, 0},
        {"MoveAwayFromLastAttacker", NULL, 0, 0, 0},
        {"ProbeDroid", Action_ProbeDroid, 0, 0, 0},
        {"AlertCreatures", Action_AlertCreatures, 0, 0, 0},
        {"SetLastAttacker", NULL, 0, 0, 0},
        {"LinkTurretToController", Action_LinkTurretToController, 0, 0, 0},
        {"TakeOver", Action_TakeOver, 1, 0, 0},
        {"ReleaseTakeOver", Action_ReleaseTakeOver, 1, 0, 0},
        {"RegisterTakeOverObject", Action_RegisterTakeOverObject, 0, 0, 0},
        {"SetTakeOverTarget", Action_SetTakeOverTarget, 0, 0, 0},
        {"ClearTakeOverTarget", Action_ClearTakeOverTarget, 0, 0, 0},
        {"AddGameMsgCount", Action_AddGameMsgCount, 1, 0, 0},
        {"AddMiscPickups", Action_AddMiscPickups, 0, 0, 0},
        {"SetCanTakeOver", Action_SetCanTakeOver, 0, 0, 0},
        {"CanBeCarried", Action_CanBeCarried, 1, 0, 0},
        {"IgnoreLastSafePathPos", Action_IgnoreLastSafePathPos, 0, 0, 0},
        {"AwkwardShapeOverride", Action_AwkwardShapeOverride, 0, 0, 0},
        {"IgnoreSlideTerrain", Action_IgnoreSlideTerrain, 0, 0, 0},
        {"SplineFollowTerrain", Action_SplineFollowTerrain, 0, 0, 0},
        {"SetLayer", Action_SetLayer, 0, 0, 0},
        {"CreateRider", Action_CreateRider, 0, 0, 0},
        {"AddTorpedoPacket", NULL, 1, 0, 0},
        {"SpeederBeingChased", Action_SpeederBeingChased, 0, 0, 0},
        {"ThrowDetonator", Action_ThrowDetonator, 0, 0, 0},
        {"SetScaleOverride", NULL, 0, 0, 0},
        {"DisableNarrowSocks", Action_DisableNarrowSocks, 1, 0, 0},
        {"UseTimeBasedUpdate", Action_UseTimeBasedUpdate, 0, 0, 0},
        {"ForceLightning", Action_ForceLightning, 0, 0, 0},
        {"WalkBackwards", Action_WalkBackwards, 0, 0, 0},
        {"AddScriptProcessor", Action_AddScriptProcessor, 0, 0, 0},
        {"SetUseOneAtOnce", NULL, 0, 0, 0},
        {"SetAO_MaxAttackers", Action_SetAttackersAtOnce, 0, 0, 0},
        {"SetAO_AttackersPerRow", Action_SetAttackersPerRow, 0, 0, 0},
        {"SetAO_RowDist", Action_SetAtOnceRowDistance, 0, 0, 0},
        {"SetAO_InitRowDist", Action_InitRowDist, 0, 0, 0},
        {"SetTechnoComplete", Action_SetTechnoComplete, 0, 0, 0},
        {"LetGoOfBalloon", Action_LetGoOfBalloon, 0, 0, 0},
        {"DrawBossHitPoints", Action_DrawBossHitPoints, 1, 0, 0},
        {"CompleteLevel", Action_CompleteLevel, 1, 0, 0},
        {"GoToNewLevel", Action_GoToNewLevel, 0, 0, 0},
        {"CircleLocator", Action_CircleLocator, 0, 0, 0},
        {"GizmoActivate", Action_GizmoActivate, 0, 0, 0},
        {"GizmoSetVisibility", Action_GizmoSetVisibility, 0, 0, 0},
        {"TurnOnPickup", Action_TurnOnPickup, 0, 0, 0},
        {"CanHelpWithTriggers", Action_CanHelpWithTriggers, 0, 0, 0},
        {"CanCollideWithObjects", Action_CanCollideWithObjects, 0, 0, 0},
        {"SetShootOpponents", Action_SetShootOpponents, 0, 0, 0},
        {"PartyCanBeUnderCover", Action_PartyCanBeUnderCover, 0, 0, 0},
        {"SetLapTime", NULL, 0, 0, 0},
        {"CreatePod", NULL, 0, 0, 0},
        {"MushroomCollapse", NULL, 0, 0, 0},
        {"BoulderSection", NULL, 0, 0, 0},
        {"RaceOpponent", Action_RaceOpponent, 0, 0, 0},
        {"Sebulba", NULL, 0, 0, 0},
        {"IgnoreTurnAroundSpline", Action_IgnoreTurnAroundSpline, 0, 0, 0},
        {"CanMoveWhenDeactivated", Action_CanMoveWhenDeactivated, 0, 0, 0},
        {"DontAttack", Action_DontAttack, 0, 0, 0},
        {"CanPullLevers", Action_CanPullLevers, 0, 0, 0},
        {"NewSebulba", NULL, 0, 0, 0},
        {NULL, NULL, 0, 0, 0},
    };
}

DECOMP_ASSERT(sizeof(lego_aiactiondefs) / sizeof(lego_aiactiondefs[0]) == LEGO_AI_ACTION_NEW_SEBULBA + 2,
              "complete game action registry");

extern "C" {
    f32 direction_scale = 100.0f;
}

extern "C" {
    f32 debug_condition;
}

namespace {
    struct AISysRegistryCallbacks {
        AISysRegistryCallbacks() {

            lego_aiconditiondefs[LEGO_AI_CONDITION_CATEGORY_IS].eval_fn = Condition_CategoryIs;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_CATEGORY_IS].eval_fn = Condition_PlayerCategoryIs;
            lego_aiconditiondefs[LEGO_AI_CONDITION_I_AM_A_GOODIE_BADDIE].eval_fn = Condition_IAmAGoodieBaddie;
            lego_aiconditiondefs[LEGO_AI_CONDITION_OPPONENT_IS_A_THREAT].eval_fn = Condition_OpponentIsAThreat;
            lego_aiconditiondefs[LEGO_AI_CONDITION_I_AM_A_GOODY].eval_fn = Condition_IAmAGoody;
            lego_aiconditiondefs[LEGO_AI_CONDITION_I_AM_A_BADDY].eval_fn = Condition_IAmABaddy;
            lego_aiconditiondefs[LEGO_AI_CONDITION_I_AM_A_NEUTRAL].eval_fn = Condition_IAmANeutral;
            lego_aiconditiondefs[LEGO_AI_CONDITION_I_AM_A_PARTY_CHARACTER].eval_fn = Condition_IAmAPartyCharacter;
            lego_aiconditiondefs[LEGO_AI_CONDITION_LOCATOR_ON_SCREEN].eval_fn = Condition_LocatorOnScreen;
            lego_aiconditiondefs[LEGO_AI_CONDITION_TURRET_ALIVE].eval_fn = Condition_TurretAlive;
            lego_aiconditiondefs[LEGO_AI_CONDITION_ACTIVE].eval_fn = Condition_Active;
            lego_aiconditiondefs[LEGO_AI_CONDITION_DEBUG].eval_fn = Condition_Debug;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GLYN_TEST].eval_fn = Condition_GlynTest;
            lego_aiconditiondefs[LEGO_AI_CONDITION_HOVER_PHASE].eval_fn = Condition_HoverPhase;
            lego_aiconditiondefs[LEGO_AI_CONDITION_SPAWN_COUNT].eval_fn = Condition_SpawnCount;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_ALERTED].eval_fn = Condition_BeenAlerted;
            lego_aiconditiondefs[LEGO_AI_CONDITION_ON_GROUND].eval_fn = Condition_OnGround;
            lego_aiconditiondefs[LEGO_AI_CONDITION_COLLIDING].eval_fn = Condition_Colliding;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GOT_VICTIM].eval_fn = Condition_GotVictim;
            lego_aiconditiondefs[LEGO_AI_CONDITION_MY_SET].eval_fn = Condition_MySet;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEING_TOWED].eval_fn = Condition_BeingTowed;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_HIT].eval_fn = Condition_BeenHit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_IS_ALIVE].eval_fn = Condition_IsAlive;
            lego_aiconditiondefs[LEGO_AI_CONDITION_IS_ALIVE].init_fn = Condition_IsAliveInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_HIT].init_fn = Condition_BeenHitInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_ON_OBJECT].eval_fn = Condition_OnObject;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CONTEXT].eval_fn = Condition_Context;
            lego_aiconditiondefs[LEGO_AI_CONDITION_IN_SWAMP].eval_fn = Condition_InSwamp;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GOT_GUN].eval_fn = Condition_GotGun;
            lego_aiconditiondefs[LEGO_AI_CONDITION_SIDE].eval_fn = Condition_Side;
            lego_aiconditiondefs[LEGO_AI_CONDITION_SIDE].init_fn = Condition_SideInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_X_POS].eval_fn = Condition_XPos;
            lego_aiconditiondefs[LEGO_AI_CONDITION_X_POS].init_fn = Condition_XYZPosInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_Y_POS].eval_fn = Condition_YPos;
            lego_aiconditiondefs[LEGO_AI_CONDITION_Y_POS].init_fn = Condition_XYZPosInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_Z_POS].eval_fn = Condition_ZPos;
            lego_aiconditiondefs[LEGO_AI_CONDITION_Z_POS].init_fn = Condition_XYZPosInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_TAKE_OVER_RANGE].eval_fn = Condition_TakeOverRange;
            lego_aiconditiondefs[LEGO_AI_CONDITION_HAS_TAKE_OVER].eval_fn = Condition_HasTakeOver;
            lego_aiconditiondefs[LEGO_AI_CONDITION_OFF_SCREEN_TIMER].eval_fn = Condition_OffScreenTimer;
            lego_aiconditiondefs[LEGO_AI_CONDITION_OFF_SCREEN_TIMER].init_fn = Condition_OffScreenTimerInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BUILD_IT_COMPLETE].eval_fn = Condition_BuildItComplete;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BUILD_IT_COMPLETE].init_fn = Condition_BuildItCompleteInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CHARACTER_EXISTS].eval_fn = Condition_CharacterExists;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CHARACTER_EXISTS].init_fn = Condition_CharacterExistsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_TO_LEVEL].init_fn = Condition_BeenToLevelInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_EITHER_PLAYER_ON_OBJECT].eval_fn = Condition_EitherPlayerOnObject;
            lego_aiconditiondefs[LEGO_AI_CONDITION_IS_LOW_END_DEVICE].eval_fn = Condition_IsLowEndDevice;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CHARACTER_LOADED].eval_fn = Condition_CharacterLoaded;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CHARACTER_LOADED].init_fn = Condition_CharacterLoadedInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CATEGORY_IS].init_fn = Condition_CategoryIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_CATEGORY_IS].init_fn = Condition_CategoryIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_EITHER_PLAYER_LOCATOR_RANGE_XZ].eval_fn =
                Condition_EitherPlayerLocatorRangeXZ;
            lego_aiconditiondefs[LEGO_AI_CONDITION_EITHER_PLAYER_LOCATOR_RANGE_XZ].init_fn = Condition_LocatorRangeInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_IN_SAME_TRIGGER_AREA_AS_NEAREST_PLAYER].eval_fn =
                Condition_InSameTriggerAreaAsNearestPlayer;
            lego_aiconditiondefs[LEGO_AI_CONDITION_TAKE_OVER_TARGET_IN_TRIGGER_AREA].eval_fn =
                Condition_TakeOverTargetInTriggerArea;
            lego_aiconditiondefs[LEGO_AI_CONDITION_TAKE_OVER_TARGET_IN_TRIGGER_AREA].init_fn =
                Condition_TakeOverTargetInTriggerAreaInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_SHOP_ACTIVE].eval_fn = Condition_ShopActive;
            lego_aiconditiondefs[LEGO_AI_CONDITION_SCREEN_WIPE].eval_fn = Condition_ScreenWipe;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CAN_HEAR_RADIO].eval_fn = Condition_CanHearRadio;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEHIND_CAMERA].eval_fn = Condition_BehindCamera;
            lego_aiconditiondefs[LEGO_AI_CONDITION_NUM_BADDIES].eval_fn = Condition_NumBaddies;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BLOWUP_BLOWNUP].eval_fn = Condition_BlowupBlownup;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BLOWUP_BLOWNUP].init_fn = Condition_BlowupInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_ANIM_SPEED_MUL].eval_fn = Condition_AnimSpeedMul;
            lego_aiconditiondefs[LEGO_AI_CONDITION_TURRET_ALIVE].init_fn = Condition_TurretAliveInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_0].eval_fn = Condition_GizmoOutput0;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_0].init_fn = Condition_GizmoOutputInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_1].eval_fn = Condition_GizmoOutput1;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_1].init_fn = Condition_GizmoOutputInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_2].eval_fn = Condition_GizmoOutput2;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_2].init_fn = Condition_GizmoOutputInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_3].eval_fn = Condition_GizmoOutput3;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_OUTPUT_3].init_fn = Condition_GizmoOutputInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_LOCATOR_ON_SCREEN].init_fn = Condition_LocatorOnScreenInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_VISIBILITY].eval_fn = Condition_GizmoVisibility;
            lego_aiconditiondefs[LEGO_AI_CONDITION_GIZMO_VISIBILITY].init_fn = Condition_GizmoVisibilityInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FLOW_BOX_COMPLETE].eval_fn = Condition_FlowBoxComplete;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FLOW_BOX_COMPLETE].init_fn = Condition_FlowBoxCompleteInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_AREA_COMPLETE].eval_fn = Condition_AreaComplete;
            lego_aiconditiondefs[LEGO_AI_CONDITION_AREA_COMPLETE].init_fn = Condition_AreaCompleteInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_I_AM_A].eval_fn = Condition_IAmA;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PARTY_UNDER_COVER].eval_fn = Condition_PartyUnderCover;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PREFERS_BRAWLING].eval_fn = Condition_PrefersBrawling;
            lego_aiconditiondefs[LEGO_AI_CONDITION_NEAREST_PARTY_RANGE].eval_fn = Condition_NearestPartyRange;
            lego_aiconditiondefs[LEGO_AI_CONDITION_NEAREST_PARTY_XZ_RANGE].eval_fn = Condition_NearestPartyXZRange;
            lego_aiconditiondefs[LEGO_AI_CONDITION_I_AM_A].init_fn = Condition_IAmAInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_OPPONENT_IS_A].eval_fn = Condition_OpponentIsA;
            lego_aiconditiondefs[LEGO_AI_CONDITION_OPPONENT_IS_A].init_fn = Condition_OpponentIsAInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_1_IS].eval_fn = Condition_Player1Is;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_1_IS].init_fn = Condition_EitherPlayerIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_2_IS].eval_fn = Condition_Player2Is;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_2_IS].init_fn = Condition_EitherPlayerIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_EITHER_PLAYER_IS].eval_fn = Condition_EitherPlayerIs;
            lego_aiconditiondefs[LEGO_AI_CONDITION_EITHER_PLAYER_IS].init_fn = Condition_EitherPlayerIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_TO_LEVEL].eval_fn = Condition_BeenToLevel;

            lego_aiactiondefs[LEGO_AI_ACTION_SET_CURRENT_SPEED].eval_fn = Action_SetCurrentSpeed;
            lego_aiactiondefs[LEGO_AI_ACTION_USE_CURRENT_SPEED].eval_fn = Action_UseCurrentSpeed;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_MAX_MOVEMENT_RANGE].eval_fn = Action_SetMaxMovementRange;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_DEFAULT_MOVEMENT_RANGE].eval_fn = Action_SetDefaultMovementRange;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_GRAVITY_HEIGHT].eval_fn = Action_SetGravityHeight;
            lego_aiactiondefs[LEGO_AI_ACTION_PLAY_GIZ_OBSTACLE].eval_fn = Action_PlayGizObstacle;
            lego_aiactiondefs[LEGO_AI_ACTION_PLAY_OBSTACLE].eval_fn = Action_PlayGizObstacle;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_OBSTACLE_TO_END].eval_fn = Action_SetObstacleToEnd;
            lego_aiactiondefs[LEGO_AI_ACTION_MOVE_AWAY_FROM_LAST_ATTACKER].eval_fn = Action_MoveAwayFromLastAttacker;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_LAST_ATTACKER].eval_fn = Action_SetLastAttacker;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_USE_ONE_AT_ONCE].eval_fn = Action_SetUseOneAtOnce;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CATEGORY_IS].eval_fn = Condition_CategoryIs;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BLOWUP_BLOWNUP].eval_fn = Condition_BlowupBlownup;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BLOWUP_BLOWNUP].init_fn = Condition_BlowupInit;
        }
    };

    AISysRegistryCallbacks aisys_registry_callbacks;
} // namespace

NUVEC test_launch = {0.0f, 0.0f, 1.0f};

f32 test_missile_scale = 8.0f;

i32 guided_rotate_speed = 240000;

f32 guided_start_time = 0.5f;

f32 guided_life = 8.0f;

f32 guided_speed = 30.0f;

namespace {
    struct GameAIRegistryCallbacks {
        GameAIRegistryCallbacks() {
            lego_aiactiondefs[LEGO_AI_ACTION_CREATE_SPLINE_CREATURES].eval_fn = Action_CreateSplineCreatures;
            lego_aiactiondefs[LEGO_AI_ACTION_FOLLOW_CHARACTER].eval_fn = Action_FollowCharacter;
            lego_aiactiondefs[LEGO_AI_ACTION_FOLLOW_PLAYER].eval_fn = Action_GameFollowPlayer;
            lego_aiactiondefs[LEGO_AI_ACTION_MOVE_FORWARD].eval_fn = Action_MoveForward;
        }
    };

    GameAIRegistryCallbacks game_ai_registry_callbacks;
} // namespace

static f32 Condition_GlynTest(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return 1.0f;
}

static f32 Condition_Debug(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return debug_condition;
}

static f32 Condition_Active(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    f32 active = 0.0f;
    if (packet != NULL)
        active = packet->reset_mode == 2 ? 1.0f : 0.0f;
    return active;
}

static f32 Condition_GotGun(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL && (packet->owner->apiobj.objptr->field_0xef8 & 4) != 0)
        return 1.0f;
    return 0.0f;
}

static f32 Condition_PrefersBrawling(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL)
        return static_cast<i32>(packet->owner->apiobj.character_data->model_flags) < 0 ? 1.0f : 0.0f;
    return 0.0f;
}

static f32 Condition_MySet(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *) {
    return static_cast<f32>(processor->creature_set);
}

static f32 Condition_TurretAlive(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    GIZTURRET *turret = static_cast<GIZTURRET *>(argument);
    return turret != NULL && (turret->flags & 0x30) == 0 ? 1.0f : 0.0f;
}

static f32 Condition_IAmAPartyCharacter(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->owner != NULL && static_cast<i8>(packet->owner->apiobj.field_0x27c) != -1 ? 1.0f
                                                                                                               : 0.0f;
}

static f32 Condition_IAmA(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (object != NULL) {
            f32 result = 0.0f;
            if (reinterpret_cast<isize>(argument) == object->id)
                result = 1.0f;
            return result;
        }
    }
    return 0.0f;
}

static f32 Condition_OpponentIsA(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    if (packet != NULL && packet->opponent_object != NULL) {
        GameObject_s *object = packet->opponent_object->objptr;
        if (object != NULL) {
            f32 result = 0.0f;
            if (reinterpret_cast<isize>(argument) == object->id)
                result = 1.0f;
            return result;
        }
    }
    return 0.0f;
}

static f32 Condition_OpponentIsAThreat(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    return packet != NULL ? static_cast<f32>(packet->opponent_is_threat) : 0.0f;
}

static f32 Condition_IAmAGoody(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet == NULL || packet->owner == NULL)
        return 0.0f;
    if (packet->owner->apiobj.field_0x1f4 & 5)
        return 0.0f;
    return 1.0f;
}

static f32 Condition_IAmABaddy(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->owner != NULL && (packet->owner->apiobj.field_0x1f4 & 0x10001) != 0 ? 1.0f : 0.0f;
}

static f32 Condition_IAmANeutral(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->owner != NULL && (packet->owner->apiobj.field_0x1f4 & 4) != 0 ? 1.0f : 0.0f;
}

static f32 Condition_IAmAGoodieBaddie(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->owner != NULL && (packet->owner->apiobj.field_0x1f4 & 0x10000) != 0 ? 1.0f : 0.0f;
}

static void *Condition_EitherPlayerIsInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL && GetNamedAPIObjectFn != NULL ? GetNamedAPIObjectFn(sys, arg) : NULL;
}

static f32 Condition_EitherPlayerIs(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    return argument != NULL && sys != NULL && (sys->player_1 == argument || sys->player_2 == argument) ? 1.0f : 0.0f;
}

static f32 Condition_Player1Is(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    return argument != NULL && sys != NULL && sys->player_1 == argument ? 1.0f : 0.0f;
}

static f32 Condition_Player2Is(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    return argument != NULL && sys != NULL && sys->player_2 == argument ? 1.0f : 0.0f;
}

static f32 Condition_BuildItComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    if (argument != NULL) {
        GIZBUILDIT *build = static_cast<GIZBUILDIT *>(static_cast<GIZMO *>(argument)->object);
        if (build != NULL) {
            return build->build_state == 2 ? 1.0f : 0.0f;
        }
    }
    return 0.0f;
}

static void *Condition_IsAliveInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL && GetNamedAPIObjectFn != NULL ? GetNamedAPIObjectFn(sys, arg) : NULL;
}

static f32 Condition_IsAlive(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    APIOBJECT *object = static_cast<APIOBJECT *>(argument);
    if (object != NULL) {
        AIGROUP *group = object->ai->group;
        if (group != NULL) {
            for (i32 index = 0; index < group->member_count; ++index) {
                APIOBJECT *member = group->members[index];
                if (member != NULL && (member->field_0x1f8 & 0x1001) == 0x1001 && member->field_0x287 == 0)
                    return 1.0f;
            }
        } else if ((object->flags_high & 0x10) != 0 && object->field_0x287 == 0) {
            return 1.0f;
        }
    }
    return 0.0f;
}

static f32 Condition_OffScreenTimer(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    GameObject *object = static_cast<GameObject *>(argument);
    if (object == NULL) {
        object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    }
    return object != NULL ? object->field_0xf1c : 0.0f;
}

static f32 Condition_OnGround(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (object != NULL && (object->apiobj.packed_contact_state & 0xffff00) != 0)
            return 1.0f;
    }
    return 0.0f;
}

static f32 Condition_BeenAlerted(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr->alert_target != NULL) {
        return 1.0f;
    }
    return 0.0f;
}

static f32 Condition_EitherPlayerOnObject(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    i32 platform = static_cast<i32>(reinterpret_cast<isize>(argument));
    if (player != NULL && platform != -1 && (player->apiobj.packed_contact_state & 0xffff00u) != 0 &&
        player->apiobj.supporting_platform_id == platform) {
        NUMTX *transform = static_cast<NUMTX *>(CurTerr->platforms[platform].scene_object);
        if (player->apiobj.position.y >= transform->m31) {
            return 1.0f;
        }
    }
    if (player2 != NULL && platform != -1 && (player2->apiobj.packed_contact_state & 0xffff00u) != 0 &&
        player2->apiobj.supporting_platform_id == platform) {
        NUMTX *transform = static_cast<NUMTX *>(CurTerr->platforms[platform].scene_object);
        if (player2->apiobj.position.y >= transform->m31) {
            return 1.0f;
        }
    }
    return 0.0f;
}

static f32 Condition_OnObject(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        isize platform = reinterpret_cast<isize>(argument);
        if (platform != -1 && object != NULL && (object->apiobj.packed_contact_state & 0xffff00) != 0) {
            f32 result = 0.0f;
            if (object->apiobj.supporting_platform_id == platform) {
                NUMTX *transform = static_cast<NUMTX *>(CurTerr->platforms[platform].scene_object);
                result = object->apiobj.position.y >= transform->m31 ? 1.0f : 0.0f;
            }
            return result;
        }
    }
    return 0.0f;
}

static f32 Condition_SpawnCount(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    return packet != NULL ? static_cast<f32>(packet->respawn_count) : 0.0f;
}

static f32 Condition_BeenHit(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    GameObject_s *object = static_cast<GameObject_s *>(argument);
    if (object == NULL)
        object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL)
        return object->flicker_timer > 0.0f || (object->field_0xef8 & 1) != 0 ? 1.0f : 0.0f;
    return 0.0f;
}

static f32 Condition_HoverPhase(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL)
        return static_cast<f32>(packet->owner->apiobj.objptr->field_0xe31);
    return 0.0f;
}

static f32 Condition_Colliding(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.colliding_objects_mask != 0)
        return 1.0f;
    return 0.0f;
}

static f32 Condition_XPos(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    GameObject_s *object = static_cast<GameObject_s *>(argument);
    if (object == NULL)
        object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    return object != NULL ? object->apiobj.collision_position.x : 0.0f;
}

static f32 Condition_YPos(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    GameObject_s *object = static_cast<GameObject_s *>(argument);
    if (object == NULL)
        object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    return object != NULL ? object->apiobj.collision_position.y : 0.0f;
}

static f32 Condition_ZPos(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    GameObject_s *object = static_cast<GameObject_s *>(argument);
    if (object == NULL)
        object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    return object != NULL ? object->apiobj.collision_position.z : 0.0f;
}

static f32 Condition_Context(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (object != NULL)
            return static_cast<f32>(object->character_context);
    }
    return -1.0f;
}

static __used__ void *Condition_NumBaddiesInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static f32 Condition_NumBaddies(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    i32 count = 0;
    GameObject *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && (object->apiobj.field_0x1f4 & 1) != 0)
            ++count;
    }
    return static_cast<f32>(count);
}

static f32 Condition_BeenToLevel(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *void_arg) {
    const isize area_level = AIConditionArgumentValue(void_arg);
    if (area_level == -1) {
        return 0.0f;
    }
    const u8 *progress = static_cast<const u8 *>(LevelProgressData) + area_level * LEVEL_PROGRESS_STRIDE;
    return (progress[LEVEL_PROGRESS_COMPLETION_FLAGS_OFFSET] & LEVEL_PROGRESS_STORY_COMPLETE) != 0 ? 1.0f : 0.0f;
}

static f32 Condition_PartyUnderCover(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return party_under_cover != 0 ? 1.0f : 0.0f;
}

static f32 Condition_TakeOverTargetInTriggerArea(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                                 void *argument) {
    if (packet != NULL && packet->owner != NULL) {
        AIAREA *area = static_cast<AIAREA *>(argument);
        if (area == NULL) {
            area = processor->unknown_a0;
        }
        i32 index = area - WORLD->ai_sys->areas;
        GameObject *object = packet->owner->apiobj.objptr;
        if (object != NULL) {
            GameObject *target = object->takeover_target;
            if (sys->player_1 != NULL && target != NULL) {
                u64 membership = target->apiobj.ai_area_mask;
                if (((membership >> (index & 63)) & 1) != 0) {
                    return 1.0f;
                }
            }
        }
    }
    return 0.0f;
}

static f32 Condition_CharacterExists(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    return argument != NULL ? 1.0f : 0.0f;
}

static f32 Condition_HasTakeOver(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject *object = packet->owner->apiobj.objptr;
        if (object != NULL && object->takeover_target != NULL)
            return 1.0f;
    }
    return 0.0f;
}

static f32 Condition_GotVictim(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL && (packet->owner->field_0xe24 & 1) != 0)
        return 1.0f;
    return 0.0f;
}

static f32 Condition_ScreenWipe(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    extern FadeSystem FadeSys;
    return FadeSys.fade > 0.0f ? 1.0f : 0.0f;
}

static __used__ f32 Condition_HeadTurnRestricted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    GameObject_s *object = ActionOwner(packet);
    if (object == NULL) {
        return 0.0f;
    }
    return static_cast<f32>(static_cast<u32>(object->field_0xefe & 1));
}

static f32 Condition_Side(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    if (packet != NULL && packet->owner != NULL) {
        isize side = reinterpret_cast<isize>(argument);
        if (side == 2) {
            if (packet->owner->apiobj.field_0x1f4 & 0x10000)
                return 1.0f;
        } else if (side == 1) {
            if ((packet->owner->apiobj.field_0x1f4 & 5) == 0)
                return 1.0f;
            return 0.0f;
        } else if (side == -1) {
            if (packet->owner->apiobj.field_0x1f4 & 1)
                return 1.0f;
        } else if (side == 0) {
            if (packet->owner->apiobj.field_0x1f4 & 4)
                return 1.0f;
        }
    }
    return 0.0f;
}

static f32 Condition_FlowBoxComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    FLOWBOX_s *box = static_cast<FLOWBOX_s *>(argument);
    if (box != NULL) {
        if ((box->state_flags_low & 2) == 0)
            return 0.0f;
        return 1.0f;
    }
    return 0.0f;
}

static f32 Condition_AnimSpeedMul(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (object != NULL)
            return object->animation_speed_multiplier;
    }
    return 1.0f;
}

static f32 Condition_CanHearRadio(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return radios_playing != 0 ? 1.0f : 0.0f;
}

static f32 Condition_BeingTowed(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && (packet->owner->apiobj.objptr->field_0xf01 & 2) != 0)
        return 1.0f;
    return 0.0f;
}

static f32 Condition_AreaComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    AREADATA *area = static_cast<AREADATA *>(argument);
    return area != NULL && Game.area_save[area->index].area_complete == 1 ? 1.0f : 0.0f;
}

static f32 Condition_InSwamp(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        f32 in_swamp = 0.0f;
        if (object != NULL)
            in_swamp = object->apiobj.field_0x27f == 9 ? 1.0f : 0.0f;
        return in_swamp;
    }
    return 0.0f;
}

static __used__ f32 Condition_NetworkGameOnGoing(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static f32 Condition_IsLowEndDevice(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return g_lowEndLevelBehaviour != 0 ? 1.0f : 0.0f;
}

__used__ static i32 Action_PressTagButton(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->pad_gamepad->buttons_pressed |= GAMEPAD_TAG;
    }
    return 1;
}

static i32 Action_PressActionButton(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32, f32) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (object != NULL) {
            object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
            object->field_0xef9 |= 4;
            object->field_0xef8 |= 0x20;
        }
    }
    return 1;
}

__used__ static i32 Action_UseWeapon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (object != NULL) {
        object->field_0xefb |= 0x20;
    }
    return 1;
}

__used__ static i32 Action_PressJumpButton(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
    }
    return 1;
}

__used__ static i32 Action_BreakFormation(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    if (packet != NULL && packet->group != NULL) {
        packet->group->is_in_formation = 0;
    }
    return 1;
}

__used__ static i32 Action_FormationMove(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    if (packet != NULL && packet->group != NULL) {
        packet->group->is_in_formation = 1;
    }
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 1;
}

__used__ static i32 Action_SetHint(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 1;
}

__used__ static i32 Action_SetHintComplete(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 1;
}

__used__ static i32 Action_CanPullLevers(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->field_0xefe |= 0x80;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    object->field_0xefe &= ~0x80;
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_DontAttack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        object->jump_input_flags |= 2;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    object->jump_input_flags &= static_cast<u8>(~2u);
                }
            }
        }
    }
    return 1;
}

static i32 Action_CanMoveWhenDeactivated(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                         i32, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object != NULL) {
        object->field_0xf04 |= 1;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0)
                    object->field_0xf04 &= ~1;
            }
        }
    }
    return 1;
}

static i32 Action_IgnoreTurnAroundSpline(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                         i32, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object != NULL) {
        object->field_0xf03 |= 0x80;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0)
                    object->field_0xf03 &= ~0x80;
            }
        }
    }
    return 1;
}

static i32 Action_SetShootOpponents(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                    i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->field_0xef8 |= 0x80;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "false") == 0)
                object->field_0xef8 &= ~0x80;
        }
    }
    return 1;
}

static i32 Action_CanCollideWithObjects(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                        i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->field_0xf02 &= ~8;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "TRUE") == 0)
                object->field_0xf02 |= 8;
        }
    }
    return 1;
}

static i32 Action_CanHelpWithTriggers(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                      i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->field_0xf02 |= 4;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0)
                object->field_0xf02 &= ~4;
        }
    }
    return 1;
}

static i32 Action_UseTimeBasedUpdate(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject *object = packet->owner->apiobj.objptr;
        i32 disabled = 1;
        if (first_time != 0 && param_count > 0) {
            i32 enabled = 0;
            for (i32 index = 0; index != param_count; ++index) {
                if (NuStrICmp(params[index], "TRUE") == 0)
                    enabled = 1;
                else if (NuStrICmp(params[index], "FALSE") == 0)
                    enabled = 0;
            }
            disabled = (enabled ^ 1) & 1;
        }
        object->field_0xf00 = (object->field_0xf00 & ~0x20) | (disabled << 5);
    }
    return 1;
}

static i32 Action_DisableNarrowSocks(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count, i32, f32) {
    i32 disabled = 1;
    if (param_count != 0 && params != NULL && params[0] != NULL && NuStrICmp("FALSE", params[0]) == 0)
        disabled = 0;
    disable_narrow_socks = disabled;
    return 1;
}

__used__ static i32 Action_Respawnable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            object->respawnable = 1;
            object->respawn_at_origin = 0;
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "origin") == 0) {
                    object->respawn_at_origin = 1;
                } else if (NuStrICmp(params[index], "false") == 0) {
                    object->respawnable = 0;
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_ImmuneToBolts(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            object->field_0xefa |= 8;
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "false") == 0) {
                    object->field_0xefa &= static_cast<u8>(~8u);
                }
            }
        }
    }
    return 1;
}

static i32 Action_ImmuneToKillTerrain(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                      i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->field_0xefa |= 4;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "false") == 0)
                object->field_0xefa &= ~4;
        }
    }
    return 1;
}

__used__ static i32 Action_CanTurn(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL && param_5 != 0) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        object->field_0xeff |= 0x20;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0xeff &= static_cast<u8>(~0x20u);
            }
        }
    }
    return 1;
}

static i32 Action_CannotBeForcedBack(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->field_0xeff |= 0x10;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0)
                object->field_0xeff &= ~0x10;
        }
    }
    return 1;
}

__used__ static i32 Action_CannotBeSeen(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xefc |= 2;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0xefc &= static_cast<u8>(~2u);
            }
        }
    }
    return 1;
}

static i32 Action_IgnoreShoveSystem(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                    i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->field_0xefc |= 1;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0)
                object->field_0xefc &= ~1;
        }
    }
    return 1;
}

__used__ static i32 Action_ApplyGravity(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            object->field_0xefb &= static_cast<u8>(~0x80u);
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    object->field_0xefb |= 0x80;
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetHoverPhase(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL ||
        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->uses_weapon_action != 2) {
        return 1;
    }

    if (param_5 != 0) {
        processor->action_data_1 = 1;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                processor->action_data_1 = 0;
            }
        }
    }

    if (processor->action_data_1 != 0) {
        if (object->field_0xe31 == 1) {
            return 1;
        }
    } else if (object->field_0xe31 != 1) {
        return 1;
    }
    object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
    return 0;
}

__used__ static i32 Action_SetControlSystem(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL && param_5 != 0) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        object->ai.movement_stopped = 0;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "rotational") == 0) {
                object->ai.movement_stopped = 1;
            }
        }
    }
    return 1;
}

static i32 Action_PressSpecialButton(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_count, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "hold_button") == 0)
                    processor->hold_special_button = 1;
            }
        }
    }
    object->pad_gamepad->buttons_pressed |= GAMEPAD_SPECIAL;
    i32 result = 1;
    if (processor->hold_special_button != 0) {
        object->pad_gamepad->buttons_held |= GAMEPAD_SPECIAL;
        result = 0;
    }
    return result;
}

static i32 Action_DeflectPlayersPart(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->field_0xefd |= 1;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0)
                    object->field_0xefd &= ~1;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_UpdateSockPos(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            object->field_0xef9 |= 0x40;
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "false") == 0) {
                    object->field_0xef9 &= ~0x40;
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetBoss(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            object->field_0xefb |= 8;
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "false") == 0) {
                    object->field_0xefb &= static_cast<u8>(~8u);
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CanUseWeapon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            object->field_0xef8 |= 8;
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "false") == 0) {
                    object->field_0xef8 &= static_cast<u8>(~8u);
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_DontAimAt(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            object->field_0xf00 |= 1;
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "false") == 0) {
                    object->field_0xf00 &= static_cast<u8>(~1u);
                }
            }
        }
    }
    return 1;
}

static i32 Action_CharClipToBlobShadows(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                        i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        object->jump_input_flags |= 0x80;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "false") == 0)
                object->jump_input_flags &= ~0x80;
        }
    }
    return 1;
}

__used__ static i32 Action_CanDefend(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL && param_5 != 0) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        object->field_0xef8 |= 2;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                object->field_0xef8 &= static_cast<u8>(~2u);
            }
        }
    }
    return 1;
}

static i32 Action_PlayerSpeederHack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                    i32, f32) {
    GameObject *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL) {
        return 1;
    }
    object->jump_input_flags |= 8u;
    if (param_count != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->jump_input_flags &= static_cast<u8>(~8u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_AlwaysBackFlip(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        object->field_0xef9 |= 0x01;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    object->field_0xef9 &= static_cast<u8>(~0x01u);
                }
            }
        }
    }
    return 1;
}

static i32 Action_CanHitForceObjects(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count, i32,
                                     f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    object->field_0xef8 |= 0x40;
    if (param_count != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0)
                object->field_0xef8 &= ~0x40;
        }
    }
    return 1;
}

__used__ static i32 Action_UseBigJumpToJump(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        packet->owner->apiobj.objptr->field_0xf03 |= 0x40;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    packet->owner->apiobj.objptr->field_0xf03 &= ~0x40;
                }
            }
        }
    }
    return 1;
}

static i32 Action_CanShootObstructions(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                       i32, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    object->field_0xefb |= 0x10;
    if (param_count != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0)
                object->field_0xefb &= ~0x10;
        }
    }
    return 1;
}

static i32 Action_SetBoltsDontGetDeflectedBack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                               i32 param_count, i32, f32) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL) {
        return 1;
    }
    object->field_0xefc |= 8u;
    if (param_count != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0xefc &= ~8u;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_PrefersPlayers(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_5;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->field_0xefb |= 0x40;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    object->field_0xefb &= static_cast<u8>(~0x40u);
                }
            }
        }
    }
    return 1;
}

static i32 Action_CanShootOffScreen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                    i32 first_time, f32) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL) {
        return 1;
    }
    if (first_time != 0) {
        object->field_0x1050 |= 4u;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "TRUE") == 0) {
                object->field_0x1050 |= 4u;
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0x1050 &= ~4u;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CanOpenDoors(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL || param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    object->field_0x1050 |= 1;
    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "TRUE") == 0) {
            object->field_0x1050 |= 1;
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            object->field_0x1050 &= ~1u;
        }
    }
    return 1;
}

static i32 Action_SetAIOverrideControl(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_count, i32 first_time, f32) {
    APIOBJECT *object;
    i32 enabled = 1;
    if (__builtin_expect(!first_time, 0)) {
        object = processor->override_control_object;
    } else {
        object = packet != NULL ? reinterpret_cast<APIOBJECT *>(packet->owner) : NULL;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "character");
            if (value != NULL) {
                if (GetNamedAPIObjectFn != NULL)
                    object = GetNamedAPIObjectFn(system, value + 10);
            } else if (NuStrICmp("FALSE", params[index]) == 0) {
                enabled = 0;
            }
        }
        processor->override_control_object = object;
    }
    if (object != NULL) {
        if (enabled) {
            GameObject_s *character = object->objptr;
            if (character != NULL) {
                ResetAIOverrideCharacter(character);
                if (__builtin_expect(character->field_0xcc0 != NULL, 0))
                    ResetAIOverrideCharacter(character->field_0xcc0);
            }
        }
        object->flags_high = (object->flags_high & ~1u) | (enabled & 1);
    }
    return 1;
}

__used__ static i32 Action_SetWalkSpeed(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 num_params, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (first_time) {
        object->walk_speed_override = 1000000000.0f;
        if (num_params != 0 && NuStrICmp(params[0], "default") != 0)
            object->walk_speed_override = AIParamToFloat(processor, params[0]);
    }
    return 1;
}

__used__ static i32 Action_SetAnimSpeedMul(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_5;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_4 == 0) {
            return 1;
        }
        f32 minimum = 0.0f;
        f32 maximum = 1.0e9f;
        f32 multiply_by = 1.0f;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "value=");
            if (value != NULL) {
                object->animation_speed_multiplier = AIParamToFloat(processor, value + 6);
            } else if ((value = NuStrIStr(params[index], "multiply_by=")) != NULL) {
                multiply_by = AIParamToFloat(processor, value + 12);
            } else if ((value = NuStrIStr(params[index], "max=")) != NULL) {
                maximum = AIParamToFloat(processor, value + 4);
            } else if ((value = NuStrIStr(params[index], "min=")) != NULL) {
                minimum = AIParamToFloat(processor, value + 4);
            }
        }
        if (multiply_by != 1.0f) {
            f32 speed = object->animation_speed_multiplier * multiply_by;
            if (!(speed > maximum)) {
                object->animation_speed_multiplier = MAX(minimum, speed);
            } else {
                object->animation_speed_multiplier = maximum;
            }
        }
    }
    return 1;
}

static f32 Condition_BehindCamera(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject *object = packet->owner->apiobj.objptr;
        if (object != NULL) {
            NUVEC delta;
            NuVecSub(&delta, &object->apiobj.collision_position, &GameCam->pos);
            return delta.x * GameCam->dir.x + delta.y * GameCam->dir.y + delta.z * GameCam->dir.z < 0.0f ? 1.0f : 0.0f;
        }
    }
    return 0.0f;
}

__used__ static i32 Action_RaceOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL)
        return 1;

    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;

    if (param_5 != 0) {
        object->field_0xef9 |= 0x40;
        object->field_0xf02 |= 0x20;
        object->ai.movement_stopped = 1;
        object->current_speed_multiplier = 1.0f;
        processor->action_data_4 = 10.0f;

        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "distance");
            if (value != NULL)
                processor->action_data_4 = AIParamToFloat(&packet->script_process, value + 9);
        }
    }

    SOCKPOSITION predicted_position;
    MoveSockPosition(WORLD->sock_sys, &object->sock_position, 10.0f, &predicted_position);

    NUVEC destination;
    GameObject_s *opponent = static_cast<GameObject_s *>(packet->opponent);
    if (opponent == NULL || opponent->apiobj.ai == NULL) {
        destination = predicted_position.midpoint;
        object->current_speed_multiplier = 1.0f;
    } else {
        const f32 lead = MidDistanceFromSockStart(WORLD->sock_sys, &object->sock_position) -
                         MidDistanceFromSockStart(WORLD->sock_sys, &opponent->sock_position) - processor->action_data_4;
        const f32 threshold = processor->action_data_4 * 0.75f;
        if (lead > threshold)
            object->current_speed_multiplier = 0.9f;
        else if (lead < -threshold)
            object->current_speed_multiplier = 1.1f;
        else
            object->current_speed_multiplier = 1.0f;

        NUVEC opponent_offset;
        NuVecSub(&opponent_offset, &opponent->apiobj.position, &opponent->sock_position.midpoint);
        NuVecRotateY(&opponent_offset, &opponent_offset, -opponent->sock_position.midpoint_rotation.y);

        NUVEC object_offset;
        NuVecSub(&object_offset, &object->apiobj.position, &object->sock_position.midpoint);
        NuVecRotateY(&object_offset, &object_offset, -object->sock_position.midpoint_rotation.y);

        destination.x = object_offset.x * 0.75f + opponent_offset.x * 0.75f;
        destination.z = 0.0f;
        destination.y = 0.0f;
        NuVecAdd(&destination, &destination, &predicted_position.midpoint);
    }

    AIMoveInstruction(packet, &destination, 0.0f, NULL, AIPACKET_MOVEMENT_TO_DESTINATION, 0.0f);
    return 0;
}

static i32 Action_MoveAwayFromLastAttacker(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_count, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }
    GameObject_s *object = packet->owner->apiobj.objptr;

    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[index], "face") == 0) {
                processor->action_data_1 = 1;
            } else {
                packet->movement_instruction_parameter = AIParamToFloat(processor, params[index]);
            }
        }
    }

    GameObject_s *attacker = object->last_attacker;
    if (attacker != NULL) {
        AIMoveInstruction(packet, &attacker->ai.last_path_position, attacker->ai.mover_height, &attacker->ai.path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
        if (processor->action_data_1 != 0) {
            packet->movement_look_target = &object->last_attacker->apiobj.position;
        }
    }
    return 0;
}

static void *Condition_GizmoVisibilityInit(AISYS *, char *name, AISCRIPT *) {
    return name != NULL ? GizmoFindByName(WORLD->gizmo_sys, -1, name) : NULL;
}

static void *Condition_GizmoOutputInit(AISYS *, char *name, AISCRIPT *) {
    return name != NULL ? GizmoFindByName(WORLD->gizmo_sys, -1, name) : NULL;
}

static void *Condition_BlowupInit(AISYS *, char *arg, AISCRIPT *) {
    return GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, arg);
}

static void *Condition_BuildItCompleteInit(AISYS *, char *name, AISCRIPT *) {
    return GizmoFindByName(WORLD->gizmo_sys, gizbuildit_gizmotype_id, name);
}

static void *Condition_TurretAliveInit(AISYS *, char *name, AISCRIPT *) {
    GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, turret_gizmotype_id, name);
    return gizmo != NULL ? gizmo->object : NULL;
}

static void *Condition_LocatorOnScreenInit(AISYS *system, char *name, AISCRIPT *) {
    return AIPathFindLocator(system, name);
}

static void *Condition_LocatorRangeInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

__used__ static i32 Action_WalkBackwards(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet == NULL) {
        return 1;
    }
    if (param_5 != 0) {
        packet->movement_instruction_parameter = 1.0f;
        for (i32 index = 0; index < param_4; ++index) {
            if (AIActionParseSpeedFn == NULL || AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) == 0) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
    }

    GameObject_s *owner = packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    GameObject_s *opponent = static_cast<GameObject_s *>(packet->opponent);
    if (owner != NULL && opponent != NULL) {
        AIMoveInstruction(packet, &opponent->ai.last_path_position, opponent->ai.mover_height, &opponent->ai.path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
        owner->field_0xefd |= 0x80;
        packet->goal_speed_mode = 1; // GameAIActionParseSpeed's WALK mode.
    }
    return 0;
}

__used__ static i32 Action_GoToNewLevel(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;

    if (param_5 != 0 && param_4 > 0) {
        LEVELDATA *level = NULL;
        i32 param_index = 0;

        char *cutscene_name = NULL;
        do {
            char *value = NuStrIStr(params[param_index], "level=");
            if (value != NULL) {
                level = Level_FindByName(value + NuStrLen("level="), NULL);
            } else {
                value = NuStrIStr(params[param_index], "cutscene=");
                if (value != NULL) {
                    cutscene_name = value + NuStrLen("cutscene=");
                }
            }
            ++param_index;
        } while (param_4 != param_index);

        if (FreePlay == 0 && cutscene_name != NULL &&
            NewCutScene(NULL, WORLD->cutscene_sys, cutscene_name, 0) != NULL) {
            return 1;
        }

        if (level != NULL) {
            GoToNewLevel(level->idx);
        }
    }

    return 1;
}

static i32 Action_DrawBossHitPoints(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32, f32) {
    if (packet != NULL && packet->owner != NULL)
        DrawBossHitPoints(packet->owner->apiobj.objptr);
    return 1;
}

__used__ static i32 Action_LetGoOfBalloon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    if (packet != NULL && packet->owner != NULL) {
        LetGoOfBalloon(packet->owner->apiobj.objptr);
    }
    return 1;
}

static void *Condition_TakeOverTargetInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

__used__ static i32 Action_SetScaleOverride(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    f32 scale;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (param_5 != 0) {
        scale = 1000000000.0f;
        f32 time = 0.0f;
        f32 zero = 0.0f;
        f32 min = 1000000000.0f;
        f32 max = 1000000000.0f;

        if (param_4 <= 0) {
            scale = 1000000000.0f;
        } else {
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "reset") == 0) {
                    scale = 1000000000.0f;
                } else {
                    char *value = NuStrIStr(params[index], "min=");
                    if (value != NULL) {
                        min = AIParamToFloat(processor, value + 4);
                    } else {
                        value = NuStrIStr(params[index], "max=");
                        if (value != NULL) {
                            max = AIParamToFloat(processor, value + 4);
                        } else {
                            value = NuStrIStr(params[index], "time");
                            if (value != NULL) {
                                time = AIParamToFloat(processor, value + NuStrLen("time") + 1);
                            } else {
                                scale = AIParamToFloat(processor, params[index]);
                            }
                        }
                    }
                }
            }
        }

        if (time != zero) {
            if (max != 1000000000.0f && min != 1000000000.0f) {
                f32 random = NuRandFloat();
                scale = random * max + (1.0f - random) * min;
            }
            processor->action_data_4 = time;
            processor->action_data_5 = (scale - object->field_0x1038) / time;
            if (!(time > zero)) {
                return 1;
            }
        } else {
            if (max != 1000000000.0f) {
                if (min != 1000000000.0f) {
                    f32 random = NuRandFloat();
                    object->field_0x1038 = random * max + (1.0f - random) * min;
                    return 1;
                }
            }
            object->field_0x1038 = scale;
            return 1;
        }
    }

    if (!(processor->action_data_4 > 0.0f)) {
        return 1;
    }
    if (processor->action_data_4 > FRAMETIME) {
        object->field_0x1038 += FRAMETIME * processor->action_data_5;
        processor->action_data_4 = processor->action_data_4 - FRAMETIME;
        return 0;
    }
    object->field_0x1038 += processor->action_data_4 * processor->action_data_5;
    processor->action_data_4 = 0.0f;
    return 1;
}

__used__ static i32 Action_SpinOnSpot(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    f32 zero = 0.0f;
    u16 angle;
    if (param_5 == 0) {
        angle = processor->action_data_6;
    } else {
        f32 min_time = 0.0f;
        f32 max_time = 0.0f;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "mintime=");
            if (value != NULL) {
                min_time = AIParamToFloatEx(packet, processor, value + 8);
            } else {
                value = NuStrIStr(params[index], "maxtime=");
                if (value != NULL) {
                    max_time = AIParamToFloatEx(packet, processor, value + 8);
                } else {
                    value = NuStrIStr(params[index], "time=");
                    if (value != NULL) {
                        processor->action_timer = AIParamToFloatEx(packet, processor, value + 5);
                    } else {
                        value = NuStrIStr(params[index], "rot_rate=");
                        if (value != NULL) {
                            processor->action_data_4 = static_cast<f32>(
                                static_cast<i32>(AIParamToFloatEx(packet, processor, value + 9) * 182.04444885253906f));
                        }
                    }
                }
            }
        }
        if (min_time != max_time) {
            processor->action_timer = NuRandFloat() * (max_time - min_time) + min_time;
        }
        angle = object->apiobj.field_0x276;
    }

    angle += static_cast<i32>(param_6 * processor->action_data_4);
    processor->action_data_6 = angle;
    processor->action_pos.x = 0.0f;
    processor->action_pos.y = 0.0f;
    processor->action_pos.z = 1.0f;
    NuVecRotateY(&processor->action_pos, &processor->action_pos, angle);
    NuVecAdd(&processor->action_pos, &processor->action_pos, &object->apiobj.collision_position);
    packet->movement_look_target = &processor->action_pos;

    if (processor->action_timer > zero) {
        processor->action_timer -= param_6;
        if (zero >= processor->action_timer) {
            processor->action_timer = 0.0f;
            return 1;
        }
    }
    return 0;
}

static i32 Action_SetGravityHeight(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                   i32 param_count, i32 first_time, f32) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL || first_time == 0) {
        return 1;
    }
    object->field_0xefb &= 0x7f;
    object->hover_height_override = 1.0e9f;

    f32 minimum = 1.0e9f;
    f32 maximum = 1.0e9f;
    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrICmp(params[index], "reset") == 0) {
            continue;
        }
        char *value = NuStrIStr(params[index], "min=");
        if (value != NULL) {
            minimum = AIParamToFloat(processor, value + 4);
            continue;
        }
        value = NuStrIStr(params[index], "max=");
        if (value != NULL) {
            maximum = AIParamToFloat(processor, value + 4);
        } else {
            object->hover_height_override = AIParamToFloat(processor, params[index]);
        }
    }
    if (minimum != 1.0e9f && maximum != 1.0e9f) {
        const f32 random = NuRandFloat();
        object->hover_height_override = maximum * random + (1.0f - random) * minimum;
    }
    return 1;
}

static f32 Condition_TakeOverRange(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (object != NULL && object->takeover_target != NULL)
            return NuVecDist(&object->takeover_target->apiobj.position, &packet->owner->apiobj.position, NULL);
    }
    return 1.0e9f;
}

__used__ static i32 Action_ForceLightning(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    GameObject_s *object = NULL;
    if (packet != NULL && packet->owner != NULL) {
        object = packet->owner->apiobj.objptr;
    }

    if (param_5 != 0) {
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                char *value = NuStrIStr(params[index], "locator=");
                if (value != NULL) {
                    AILOCATOR *locator = AIPathFindLocator(sys, value + 8);
                    if (locator != NULL) {
                        processor->action_pos = locator->position;
                        processor->action_data_1 = 1;
                    }
                } else {
                    value = NuStrIStr(params[index], "randxz=");
                    if (value != NULL) {
                        processor->action_data_4 = AIParamToFloat(processor, value + 7);
                    }
                }
            }
        }
    }

    if (object == NULL || processor->action_data_1 == 0) {
        return 0;
    }

    NUVEC target;
    NUVEC direction;
    NUVEC primary;
    NUVEC secondary;
    ForceLightning_Origin(object, &primary, &secondary);

    target = processor->action_pos;
    if (processor->action_data_4 > 0.0f) {
        target.x += processor->action_data_4 - NuRandFloat() * (processor->action_data_4 + processor->action_data_4);
        target.z += processor->action_data_4 - NuRandFloat() * (processor->action_data_4 + processor->action_data_4);
    }

    if (primary.y != 1000000000.0f) {
        f32 distance = NuVecDist(&target, &primary, &direction);
        NuLgtLaser(lightning_type, lightning_sizew[0], lightning_sizel[0], lightning_sizewab[0], &primary, &direction,
                   lightning_col[0], lightning_endw[0], distance);
        PlaySfx("ForceLightningLp", &processor->action_pos);
    }
    if (secondary.y != 1000000000.0f) {
        f32 distance = NuVecDist(&target, &secondary, &direction);
        NuLgtLaser(lightning_type, lightning_sizew[0], lightning_sizel[0], lightning_sizewab[0], &secondary, &direction,
                   lightning_col[0], lightning_endw[0], distance);
        PlaySfx("ForceLightningLp", &processor->action_pos);
    }
    packet->movement_look_target = &processor->action_pos;
    return 0;
}

__used__ static i32 Action_ThrowDetonator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        object->character_context = 0x2e;
        if (object->field_0xe31 == 1) {
            object->context_animation = 0x6f;
        } else if ((object->field_0xe22 & 1) != 0) {
            object->context_animation = 0x6e;
        } else {
            object->context_animation = 0x65;
        }
        if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
            object->context_animation = 0x65;
        }
        object->context_flags &= static_cast<u8>(~0x40u);
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
            object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
        } else {
            object->context_animation_timer = 1.0f;
        }
    }
    return 1;
}

__used__ static i32 Action_AddTorpedoPacket(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;

    if (ANAKINSFLIGHTB_LDATA != NULL && WorldInfo_CurrentlyActive()->current_level == ANAKINSFLIGHTB_LDATA) {
        i32 choices = 2;
        if (player != NULL && player->torpedo != NULL && player->torpedo->count != 0)
            choices = player->torpedo->count + player->torpedo->count;
        if (NuRand(NULL) % choices != 0)
            return 1;
    }

    i32 torpedo_count = 0;
    u8 flags = 0;
    if (param_5 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "torpedo");
            if (value != NULL) {
                torpedo_count = static_cast<i32>(AIParamToFloat(&packet->script_process, value + 8));
                if (torpedo_count >= 0) {
                    if (torpedo_count > getMaxTorpedos(NULL))
                        torpedo_count = getMaxTorpedos(NULL);
                } else {
                    torpedo_count = 0;
                }
            } else if (NuStrICmp(params[index], "CANBESTOLEN") == 0) {
                flags = 0x20;
            }
        }
    }

    if (object != NULL) {
        if (object->torpedo != NULL)
            FreeTorpedoPacket(&object->torpedo);
        object->torpedo = GetTorpedoPacket();
        if (object->torpedo != NULL && torpedo_count != 0) {
            object->torpedo->count = static_cast<u8>(torpedo_count);
            object->torpedo->field_0x1 |= flags;
            for (i32 index = 0; index < torpedo_count; ++index) {
                object->torpedo->pickup_positions[index].x = object->apiobj.collision_position.x;
                object->torpedo->pickup_positions[index].y = object->apiobj.collision_position.y;
                object->torpedo->pickup_positions[index].z = object->apiobj.collision_position.z;
                object->torpedo->field_03 = 0;
                object->torpedo->field_08 = 0.8f;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CreateRider(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *vehicle = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (param_5 != 0) {
        if (param_4 > 0) {
            i32 model = -1;
            char *script = NULL;
            for (i32 index = 0; index < param_4; ++index) {
                char *value = NuStrIStr(params[index], "type");
                if (value != NULL) {
                    if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                        model = static_cast<u8>(LevelCharacterTypeIDFn(value + 5));
                        if (model != 0xff) {
                            model = LevelCharacterGlobalIDFn(model);
                        }
                    }
                } else if ((value = NuStrIStr(params[index], "script")) != NULL) {
                    script = value + 7;
                }
            }

            if (model != -1 && vehicle != NULL && vehicle->field_0xcc0 == NULL) {
                GameObject_s *rider = AddDynamicCreature(model, &vehicle->apiobj.collision_position, 0,
                                                         script != NULL ? script : const_cast<char *>("default"), NULL,
                                                         NULL, 0, NULL, NULL, 0, 0);
                if (rider != NULL) {
                    TakeOverGameObject(rider, vehicle, 0, 1);
                }
            }
        }
    }
    return 1;
}

static f32 Condition_InSameTriggerAreaAsNearestPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject *nearest = NULL;
        f32 distance = 1.0e9f;
        if (player != NULL) {
            distance = NuVecXZDistSqr(&player->apiobj.position, &packet->owner->apiobj.position, NULL);
            nearest = player;
        }
        if (player2 != NULL) {
            f32 other_distance = NuVecXZDistSqr(&player2->apiobj.position, &packet->owner->apiobj.position, NULL);
            if (distance > other_distance) {
                nearest = player2;
            }
        }
        if (nearest != NULL && (packet->owner->apiobj.ai_area_mask & nearest->apiobj.ai_area_mask) != 0) {
            return 1.0f;
        }
    }
    return 0.0f;
}

static i32 Action_RemoveThrownForceObjects(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32) {
    PART_s *part = Part;
    for (i32 index = 0; index < MAXPARTS; ++index, ++part) {
        if ((part->active & 1) != 0 && part->field_1c0 == PartKill_ForceThrow) {
            KillPart(part, 0);
        }
    }
    return 1;
}

static i32 Action_SetFormationCommander(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32, f32) {
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject *object = packet->owner->apiobj.objptr;
    if (object == NULL || packet->group == NULL)
        return 1;
    AIGROUP *group = packet->group;
    if (packet->group_member_index != group->member_count - 1)
        return 1;
    if (group->count_across != 1 && group->member_count % group->count_across != 1)
        return 1;
    packet->movement_event_flags |= 1;
    i32 commander_id;
    if (object->id == id_BATTLEDROID || object->id == id_BATTLEDROIDSECURITY || object->id == id_BATTLEDROIDGEONOSIAN) {
        commander_id = id_BATTLEDROIDCOMMANDER;
    } else if (object->id == id_CLONEEP3) {
        commander_id = id_CLONEEP3SAND;
    } else {
        return 1;
    }
    NewPlayerCharacter(object, commander_id, object->id, 1);
    object->current_hp = object->apiobj.character_data->game_character->hitpoints;
    object->hitpoints = object->apiobj.character_data->game_character->hitpoints;
    return 1;
}

__used__ static i32 Action_FaceCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    f32 completion_time = 0.0f;
    if (param_5 != 0) {
        f32 minimum_time = 0.0f;
        f32 maximum_time = 0.0f;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp("look_at_camera", params[index]) == 0) {
                processor->hold_special_button = 1;
            } else {
                char *value = NuStrIStr(params[index], "mintime");
                if (value != NULL) {
                    minimum_time = AIParamToFloatEx(packet, processor, value + 8);
                } else if ((value = NuStrIStr(params[index], "maxtime")) != NULL) {
                    maximum_time = AIParamToFloatEx(packet, processor, value + 8);
                } else {
                    processor->action_timer = AIParamToFloat(processor, params[index]);
                }
            }
        }
        if (processor->action_timer == 0.0f && maximum_time > minimum_time) {
            processor->action_timer = NuRandFloat() * (maximum_time - minimum_time) + minimum_time;
        }
    }

    NUMTX *camera = NuCameraGetMtx();
    if (camera != NULL) {
        packet->movement_look_target = reinterpret_cast<NUVEC *>(&camera->m30);
        if (processor->hold_special_button != 0) {
            SetHeadTarget(object, object->ai.movement_look_target, 6, 1.0f, 0.0f, 0.0f);
        }
    }
    if (processor->action_timer > completion_time) {
        processor->action_timer -= param_6;
        if (completion_time >= processor->action_timer) {
            processor->action_timer = 0.0f;
            return 1;
        }
    }
    return 0;
}

__used__ static i32 Action_CycleCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    if (packet == NULL || packet->owner == NULL)
        return 1;

    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL || FreePlay == 0 || static_cast<i8>(object->apiobj.field_0x27c) == -1)
        return 1;

    i32 single_switch = 0;
    if (param_5 != 0) {
        processor->action_data_1 = static_cast<u8>(CHARCATEGORYCOUNT);
        processor->action_data_2 = static_cast<u8>(CHARCATEGORYCOUNT);

        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "time");
            if (value != NULL) {
                processor->action_data_4 = AIParamToFloat(processor, value + 5);
                continue;
            }

            value = NuStrIStr(params[index], "Category");
            if (value != NULL) {
                const i32 category = CharCategory_FindByName(value + 9);
                if (category != -1) {
                    if (processor->action_data_1 >= CHARCATEGORYCOUNT)
                        processor->action_data_1 = static_cast<u8>(category);
                    else
                        processor->action_data_2 = static_cast<u8>(category);
                }
                continue;
            }

            value = NuStrIStr(params[index], "min_speed=");
            if (value != NULL) {
                processor->action_data_5 = AIParamToFloat(processor, value + 10);
                continue;
            }

            if (NuStrICmp(params[index], "SingleSwitch") == 0)
                single_switch = 1;
        }
    }

    if (object->character_context == 0xb)
        return 0;

    const i32 first_category = processor->action_data_1;
    if (first_category < CHARCATEGORYCOUNT) {
        if (processor->action_data_5 == 0.0f ||
            packet->owner->apiobj.character_data->game_character->run_speed >= processor->action_data_5) {
            if (CharCategory_IsCategory(object, first_category) != 0)
                return 1;

            const i32 second_category = processor->action_data_2;
            if (second_category < CHARCATEGORYCOUNT && CharCategory_IsCategory(object, second_category) != 0)
                return 1;
        }

        processor->action_timer += param_6;
        if (processor->action_timer > processor->action_data_4) {
            processor->action_timer = 0.0f;
            object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLERIGHT;
        }
        return 0;
    }

    if (processor->action_data_5 != 0.0f) {
        if (packet->owner->apiobj.character_data->game_character->run_speed >= processor->action_data_5)
            return 1;
    } else if (single_switch == 0) {
        return 1;
    }

    processor->action_timer += param_6;
    if (processor->action_timer > processor->action_data_4) {
        processor->action_timer = 0.0f;
        object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLERIGHT;
    }
    return 1;
}

static f32 Condition_PlayerCategoryIs(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    const isize category = reinterpret_cast<isize>(argument);
    return category != -1 && player != NULL && CharCategory_IsCategory(player, static_cast<i32>(category)) != 0 ? 1.0f
                                                                                                                : 0.0f;
}

__used__ static f32 Condition_CategoryIs(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *argument) {
    const i32 category = (i32)(isize)argument;
    if (category == -1 || packet == NULL || packet->owner == NULL)
        return 0.0f;
    if (CharCategory_IsCategory(packet->owner->apiobj.objptr, category))
        return 1.0f;
    return 0.0f;
}

__used__ static i32 Action_CancelHint(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    Hint_CancelCurrent();
    return 1;
}

__used__ static i32 Action_UseCurrentSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL || param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    object->field_0xf02 |= 0x20;
    object->current_speed_multiplier = 1.0f;
    i32 snap_to_speed = 0;
    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "FALSE") == 0) {
            object->field_0xf02 &= static_cast<u8>(~0x20u);
        } else if (char *value = NuStrIStr(params[index], "multiplier")) {
            object->current_speed_multiplier = AIParamToFloat(processor, value + 11);
        } else if (NuStrICmp(params[index], "snaptospeed") == 0) {
            snap_to_speed = 1;
        }
    }

    if ((object->field_0xf02 & 0x20) != 0 && (object->field_0xef9 & 0x40) == 0) {
        object->field_0xef9 |= 0x40;
        if (snap_to_speed) {
            ComplexSockPosition(WORLD->sock_sys, &object->apiobj.position, static_cast<i8>(object->field_0x661),
                                object->sock_segment, &object->sock_position);
            ComplexSockAngles(&object->sock_angles);
        }
    }
    if (snap_to_speed) {
        CurrentStart(object, 1, 1);
    }
    return 1;
}

static __used__ i32 Action_LaunchGuidedMissile(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params,
                                               i32 param_count, i32 first_time, f32) {
    NUVEC offset = {0.0f, 0.0f, 0.0f};
    if (first_time != 0) {
        nuhspecial_s source = {};
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "from=");
            if (value != NULL) {
                NuSpecialFind(WORLD->current_gscn, &source, value + 5, 1);
            } else if ((value = NuStrIStr(params[index], "dx=")) != NULL) {
                offset.x = AIParamToFloat(processor, value + 3);
            } else if ((value = NuStrIStr(params[index], "dy=")) != NULL) {
                offset.y = AIParamToFloat(processor, value + 3);
            } else if ((value = NuStrIStr(params[index], "dz=")) != NULL) {
                offset.z = AIParamToFloat(processor, value + 3);
            }
        }

        if (NuSpecialExistsFn(&source) != 0) {
            NUMTX matrix = *NuSpecialGetDrawMtx(&source);
            NUVEC scale;
            scale = NuMtxGetScale(&matrix);
            scale.x = 1.0f / scale.x;
            scale.y = 1.0f / scale.y;
            scale.z = 1.0f / scale.z;
            NuMtxPreScale(&matrix, &scale);
            if (offset.x != 0.0f || offset.y != 0.0f || offset.z != 0.0f) {
                NuMtxTranslate(&matrix, &offset);
            }

            NUVEC direction = {0.0f, 0.0f, 1.0f};
            NuVecMtxRotate(&direction, &direction, &matrix);

            ADDPART_s add = Default_ADDPART;
            add.matrix = &matrix;
            add.velocity = &direction;
            NUVEC centre;
            NuSpecialGetRadius(&WORLD->lev_objs[232].special, &centre, &add.field_14);
            add.field_28 = 232;
            add.field_14 *= test_missile_scale;
            add.field_18 = add.field_14;
            add.gravity = 0.0f;
            add.special = &WORLD->lev_objs[232].special;
            add.flags = 0x831b;
            add.field_44 = GuidedMissile_Kill;
            add.move_fn = GuidedMissile_Move;
            add.update_fn = GuidedMissile_Deflect;
            add.field_40 = PartCollide_3D;
            add.recipient = player;
            add.time_step = FRAMETIME;
            add.field_c0 = test_missile_scale;
            AddPart(&add);
        }
    }
    return 1;
}

__used__ static i32 Action_SetRunSpeed(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 num_params, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (first_time) {
        f32 speed = 1000000000.0f;
        f32 minimum = 0.0f, maximum = 1000000000.0f, multiplier = 1.0f;
        i32 multiply = 0;
        if (num_params != 0) {
            for (i32 i = 0; i < num_params; i++) {
                char *value = NuStrIStr(params[i], "multiply=");
                if (value != NULL) {
                    multiplier = AIParamToFloat(processor, value + 9);
                    multiply = 1;
                } else if (NuStrICmp(params[i], "max=default") == 0)
                    maximum = ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->run_speed;
                else if (NuStrICmp(params[i], "default") == 0)
                    speed = ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->run_speed;
                else if (NuStrICmp(params[i], "clear") == 0) {
                } else if ((value = NuStrIStr(params[i], "max=")) != NULL)
                    maximum = AIParamToFloat(processor, value + 4);
                else if ((value = NuStrIStr(params[i], "min=")) != NULL)
                    minimum = AIParamToFloat(processor, value + 4);
                else if ((value = NuStrIStr(params[i], "seek=")) != NULL)
                    processor->action_data_4 = AIParamToFloat(processor, value + 5);
                else if (NuStrIStr(params[i], "player_run_speed") != NULL)
                    speed = ((GAMECHARACTERDATA *)player->apiobj.character_data->field11_0x24)->run_speed;
                else
                    speed = AIParamToFloat(processor, params[i]);
            }
        }
        minimum = fabsf(minimum);
        maximum = fabsf(maximum);
        if (processor->action_data_4 == 0.0f) {
            if (!multiply) {
                object->run_speed_override = speed;
                return 1;
            }
            if (object->run_speed_override == 1000000000.0f)
                object->run_speed_override =
                    ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->run_speed;
            object->run_speed_override *= multiplier;
            if (object->run_speed_override < 0.0f) {
                if (object->run_speed_override < -maximum)
                    object->run_speed_override = -maximum;
                else if (maximum > minimum && object->run_speed_override > -minimum)
                    object->run_speed_override = -minimum;
            } else {
                if (object->run_speed_override > maximum)
                    object->run_speed_override = maximum;
                else if (maximum > minimum && object->run_speed_override < minimum)
                    object->run_speed_override = minimum;
            }
            return 1;
        }
        if (!multiply)
            processor->action_data_5 = speed;
        else {
            if (object->run_speed_override == 1000000000.0f)
                processor->action_data_5 =
                    ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->run_speed;
            processor->action_data_5 *= multiplier;
            if (processor->action_data_5 < 0.0f) {
                if (processor->action_data_5 < -maximum)
                    processor->action_data_5 = -maximum;
                else if (maximum > minimum && processor->action_data_5 > -minimum)
                    processor->action_data_5 = -minimum;
            } else {
                if (processor->action_data_5 > maximum)
                    processor->action_data_5 = maximum;
                else if (maximum > minimum && processor->action_data_5 < minimum)
                    processor->action_data_5 = minimum;
            }
        }
    }
    if (processor->action_data_4 > 0.0f && object->run_speed_override < 1000000000.0f) {
        object->run_speed_override =
            SeekValF(object->run_speed_override, processor->action_data_5, processor->action_data_4);
        if (fabsf(object->run_speed_override - processor->action_data_5) < 0.01f)
            object->run_speed_override = processor->action_data_5;
        else
            return 0;
    }
    return 1;
}

static f32 Condition_NearestPartyXZRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    f32 nearest = 1.0e9f;
    if (packet != NULL && packet->owner != NULL && sys != NULL) {
        NUVEC difference;
        for (i32 index = 0; index < 8; ++index) {
            GameObject_s *object = Player[index];
            if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001) {
                f32 distance = NuVecXZDistSqr(&object->apiobj.position, &packet->owner->apiobj.position, &difference);
                if (distance < nearest)
                    nearest = distance;
            }
        }
        if (nearest != 1.0e9f)
            nearest = NuFsqrt(nearest);
    }
    return nearest;
}

static i32 Action_FollowDirection(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                  i32 param_count, i32 first_time, f32 delta_time) {
    NUVEC difference;
    if (packet == NULL || packet->owner == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object == NULL)
        return 1;
    if (first_time) {
        processor->action_pos.x = 0.0f;
        processor->action_pos.y = 0.0f;
        processor->action_pos.z = 1.0f;
        const f32 random = NuRandFloat();
        AILOCATOR *start = NULL;
        AILOCATOR *end = NULL;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "start=");
            if (value != NULL) {
                start = AIPathFindLocator(sys, value + 6);
            } else if ((value = NuStrIStr(params[index], "end=")) != NULL) {
                end = AIPathFindLocator(sys, value + 4);
            } else if ((value = NuStrIStr(params[index], "firerange")) != NULL) {
                processor->follow_direction_fire_range = AIParamToFloat(processor, value + 10);
            } else if ((value = NuStrIStr(params[index], "fireinterval")) != NULL) {
                processor->follow_direction_fire_interval = AIParamToFloat(processor, value + 13);
            }
        }
        if (end != NULL && start != NULL) {
            NuVecSub(&processor->action_pos, &end->position, &start->position);
            NuVecNorm(&processor->action_pos, &processor->action_pos);
        }
        processor->action_timer = random * processor->follow_direction_fire_interval;
        NuVecScale(&processor->action_pos, &processor->action_pos, direction_scale);
    }
    NUVEC destination;
    NuVecAdd(&destination, &object->apiobj.position, &processor->action_pos);
    AIMoveInstruction(packet, &destination, 0.0f, NULL, AIPACKET_MOVEMENT_TO_DESTINATION, 0.0f);
    APIOBJECT *opponent = packet->opponent_object;
    if (processor->follow_direction_fire_range > 0.0f && opponent != NULL && opponent->ai != NULL) {

        f32 distance_squared = NuVecDistSqr(&packet->owner->apiobj.position, &opponent->position, &difference);
        if (((WORLD->api_object_sys->line_of_sight[object->apiobj.field_0x289] >> opponent->field_0x289) & 1) &&
            object->apiobj.model_draw_result != 0 &&
            processor->follow_direction_fire_range * processor->follow_direction_fire_range > distance_squared) {
            if (opponent->field_0x287 == 0 || opponent->objptr->field_0x101c > 0.0f)
                processor->action_timer -= delta_time;
            if (opponent->field_0x287 == 0 && processor->action_timer <= 0.0f) {
                const f32 interval = processor->follow_direction_fire_interval;
                processor->action_timer = interval * 0.5f + NuRandFloat() * interval;
                object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                object->script_fire_target = packet->opponent_object->objptr;
            }
        }
    }
    return 0;
}

static f32 Condition_NearestPartyRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    f32 nearest = 1.0e9f;
    if (packet != NULL && packet->owner != NULL && sys != NULL) {
        NUVEC difference;
        for (i32 index = 0; index < 8; ++index) {
            GameObject_s *object = Player[index];
            if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001) {
                f32 distance = NuVecDistSqr(&object->apiobj.position, &packet->owner->apiobj.position, &difference);
                if (distance < nearest)
                    nearest = distance;
            }
        }
        if (nearest != 1.0e9f)
            nearest = NuFsqrt(nearest);
    }
    return nearest;
}

static i32 Action_SetSpline(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                            i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL) {
        return 1;
    }
    if (param_5 == 0) {
        return 1;
    }

    NUGSPLINE *spline = NULL;
    i32 looping = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "spline=");
        if (value != NULL) {
            spline = NuSplineFind(WORLD->current_gscn, value + 7);
        } else if (NuStrICmp(params[index], "looping") == 0) {
            looping = 1;
        }
    }

    SPLINEPOS_s *position = &object->movement_spline_position;
    memset(position, 0, sizeof(*position));
    if (spline != NULL) {
        InitSplinePosition(position, spline, 0.0f, looping);
    }
    return 1;
}

__used__ static i32 Action_Explode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL) {
        Detonate(&packet->owner->apiobj.position, 0);
    }
    return 1;
}

__used__ static i32 Action_UseForce(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32 first_time, f32 elapsed) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    i32 triggered_by_hit = 0;
    GIZFORCE_s *force;

    if (first_time != 0) {
        processor->action_data_3 = NULL;
        if (param_count <= 0) {
            return 1;
        }

        GIZFORCE_s *candidates[16];
        i32 candidate_count = 0;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "throwable") != 0) {
                if (NuStrICmp(params[index], "inrange") == 0) {
                    processor->action_data_2 = 1;
                } else if (NuStrICmp(params[index], "triggered_by_hit") == 0) {
                    triggered_by_hit = 1;
                } else {
                    char *name = NuStrIStr(params[index], const_cast<char *>("name"));
                    if (name != NULL) {
                        name += 5;
                    } else {
                        name = params[index];
                    }
                    if (name != NULL && candidate_count < 16) {
                        GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, force_gizmotype_id, name);
                        if (gizmo != NULL && gizmo->object != NULL) {
                            force = static_cast<GIZFORCE_s *>(gizmo->object);
                            candidates[candidate_count] = force;
                            if ((force->packed_state &
                                 (GIZFORCE_PROGRESS_VISIBLE | (GIZFORCE_STATE_DESTROYED_OR_THROWN << 16))) ==
                                GIZFORCE_PROGRESS_VISIBLE) {
                                ++candidate_count;
                            }
                        }
                    }
                }
            } else {
                processor->action_data_1 = 1;
            }
        }

        if (candidate_count != 0) {
            const i32 random_index = qrand() / (0xffff / candidate_count + 1);
            force = candidates[random_index];
            processor->action_data_3 = force;
        } else {
            force = static_cast<GIZFORCE_s *>(processor->action_data_3);
        }
    } else {
        force = static_cast<GIZFORCE_s *>(processor->action_data_3);
    }

    if (force == NULL) {
        return 1;
    }
    if ((force->progress_flags & GIZFORCE_PROGRESS_ENABLED) == 0) {
        return 0;
    }

    if ((force->config_flags & GIZFORCE_CONFIG_JEDI_BADDIE_ONLY) != 0) {
        if (CharCategory_IsCategory(object, 1) == 0) {
            goto invalid_category;
        }
    } else if (CharCategory_IsCategory(object, 0) == 0) {
        goto invalid_category;
    }

    if (triggered_by_hit != 0) {
        force->runtime_flags |= GIZFORCE_RUNTIME_PENDING_COMPLETION;
        return 0;
    }

    packet->movement_look_target = &force->position;
    object->pad_gamepad->allocated_5a |= 4;
    object->gizforce_target = force;
    if (processor->action_data_1 == 0) {
        return GizForce_Complete(force) != 0;
    }
    if (packet->nearest_opponent == NULL) {
        object->pad_gamepad->allocated_5a &= static_cast<u8>(~4u);
        object->gizforce_target = NULL;
    }
    return 1;

invalid_category:
    if (triggered_by_hit != 0) {
        force->runtime_flags |= GIZFORCE_RUNTIME_PENDING_COMPLETION;
    } else if (FreePlay != 0) {
        processor->action_timer -= elapsed;
        if (0.0f > processor->action_timer) {
            processor->action_timer = 0.5f;
            object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLERIGHT;
        }
    }
    return 0;
}

__used__ static i32 Action_ReleaseVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        ReleaseEat(packet->owner->apiobj.objptr);
    }
    return 1;
}

__used__ static i32 Action_EatVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        EatVictim(packet->owner->apiobj.objptr);
    }
    return 1;
}

__used__ static i32 Action_EngageObject(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    if (param_5 != 0) {
        packet->movement_instruction_parameter = idealgoalrange;
        processor->action_data_5 = engagefiretime;

        char *name = NULL;
        i32 nearest = 0;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], const_cast<char *>("name"));
            if (value != NULL) {
                name = value + 5;
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("fireinterval"))) != NULL) {
                processor->action_data_5 = AIParamToFloat(processor, value + 13);
            } else if (NuStrIStr(params[index], const_cast<char *>("nearest")) != NULL) {
                nearest = 1;
            } else if (NuStrIStr(params[index], const_cast<char *>("predictive")) != NULL) {
                processor->action_data_1 = 1;
            }
        }

        processor->action_timer = NuRandFloat() * processor->action_data_5;
        if (name != NULL) {
            if (nearest != 0) {
                nuhspecial_s specials[50];
                const i32 special_count = NuSpecialFindMulti(WORLD->current_gscn, specials, name, 50, 0);
                if (special_count > 0) {
                    f32 nearest_distance = 1.0e9f;
                    NUVEC difference;
                    for (i32 index = 0; index < special_count; ++index) {
                        NUVEC *position = NuSpecialGetDrawPos(&specials[index]);
                        const f32 distance = NuVecDistSqr(&packet->owner->apiobj.position, position, &difference);
                        if (distance < nearest_distance) {
                            nearest_distance = distance;
                            processor->action_data_3 = position;
                        }
                    }
                }
            } else {
                nuhspecial_s special;
                if (NuSpecialFind(WORLD->current_gscn, &special, name, 1) != 0) {
                    processor->action_data_3 = NuSpecialGetDrawPos(&special);
                }
            }
        }

        if (processor->action_data_3 != NULL) {
            object->field_0xefa |= 1;
            object->quick_shoot_timer = NuRandFloat() * processor->action_data_5;
        }
    }

    NUVEC *target = static_cast<NUVEC *>(processor->action_data_3);
    if (target == NULL) {
        return 0;
    }

    packet->movement_look_target = target;
    processor->action_timer -= param_6;
    object->field_0xe58 = *target;
    object->field_0xef9 |= 0x80;
    object->field_0xef8 |= 0x20;
    if (processor->action_timer > 0.0f) {
        return 0;
    }

    object->field_0xefa = static_cast<u8>((object->field_0xefa & ~2u) | ((processor->action_data_1 & 1) << 1));
    processor->action_timer = (0.5f + NuRandFloat()) * processor->action_data_5;
    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
    return 0;
}

__used__ static i32 Action_AttackOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    ai_fighting = 1;
    if (param_5 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "goalrange");
            if (value != NULL) {
                packet->movement_instruction_parameter = AIParamToFloat(processor, value + 10);
            } else if ((value = NuStrIStr(params[index], "range")) != NULL) {
                packet->movement_instruction_parameter = AIParamToFloat(processor, value + 6);
            } else if (NuStrICmp(params[index], "jediGoodie_attack") == 0) {
                processor->action_data_2 = 1;
            } else if ((value = NuStrIStr(params[index], "attack_override")) != NULL) {
                char *name = value + 16;
                if (NuStrICmp(name, "PUNCH_1") == 0)
                    processor->action_data_6 = 1;
                else if (NuStrICmp(name, "PUNCH_2") == 0)
                    processor->action_data_6 = 2;
                else if (NuStrICmp(name, "PUNCH_3") == 0)
                    processor->action_data_6 = 3;
                else if (NuStrICmp(name, "PUNCH_BEHIND") == 0)
                    processor->action_data_6 = 4;
                else if (NuStrICmp(name, "PUNCH_SPECIAL") == 0)
                    processor->action_data_6 = 5;
                else if (NuStrICmp(name, "BLOCK") == 0)
                    processor->action_data_6 = 6;
                else
                    processor->action_data_6 = NuStrICmp(name, "SHOOT") == 0 ? 7 : 0;
            }
        }
        processor->action_data_4 = 0.2f;
    }

    if (processor->action_data_6 != 0) {
        object->attack_override = static_cast<u8>(processor->action_data_6);
    }

    if (packet->opponent_object == NULL || packet->opponent_object->ai == NULL) {
        return 0;
    }
    GameObject_s *opponent = packet->opponent_object->objptr;
    object->field_0xef8 |= 0x20;
    if ((opponent->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
            (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
        opponent->apiobj.field_0x287 != 0 || opponent->character_context == CHARACTER_CONTEXT_DOOMED ||
        (static_cast<i8>(opponent->apiobj.flags_low) < 0 && opponent->spawn_protection_timer > 1.5f)) {
        return 0;
    }

    NUVEC difference;
    const f32 distance_squared = NuVecDistSqr(&opponent->apiobj.position, &packet->owner->apiobj.position, &difference);
    if ((object->field_0xf01 & 0x20) != 0) {
        APIOBJECT *attack_opponent = static_cast<APIOBJECT *>(object->ai.opponent);
        if (oneAtOnce_CanAttack(object, attack_opponent->objptr)) {
            packet->movement_instruction_parameter = 0.0f;
            processor->action_data_1 &= static_cast<u8>(~2u);
        } else {
            packet->movement_instruction_parameter = oneAtOnce_GetHoldRange(object);
            processor->action_data_1 |= 2;
        }
    }

    if ((processor->action_data_1 & 2) == 0) {
        const f32 range = packet->movement_instruction_parameter + opponent->ai.mover_height + packet->mover_height;
        AIMoveInstruction(packet, &opponent->ai.last_path_position, 0.0f, &opponent->ai.path_info,
                          AIPACKET_MOVEMENT_TO_DESTINATION, range);
        packet->goal_speed_mode = 0;
    } else {
        const f32 range = packet->movement_instruction_parameter;
        if (distance_squared < (range - aitol) * (range - aitol)) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, packet->mover_height, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_RETREAT, range);
            packet->goal_speed_mode = 0;
        } else if (!(distance_squared > (range + aitol) * (range + aitol))) {
            packet->movement_look_target = &opponent->apiobj.position;
        } else {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, packet->mover_height, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_TO_DESTINATION, range);
            packet->goal_speed_mode = 0;
        }
    }

    if (!(processor->action_data_4 > 0.0f)) {
        object->pad_gamepad->allocated_5a |= GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT;
    } else {
        if (opponent->character_context == 0x5a) {
            return 0;
        }
        const f32 attack_distance_squared =
            NuVecDistSqr(&opponent->apiobj.position, &packet->owner->apiobj.position, &difference);
        const f32 attack_range = processor->action_data_4 + opponent->ai.mover_height + object->ai.mover_height;
        if (!(attack_distance_squared < attack_range * attack_range)) {
            return 0;
        }
    }
    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;

    if (processor->action_data_2 != 0) {
        object->field_0xef9 |= 4;
    }
    return 0;
}

__used__ static i32 Action_AnimTimeRandom(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        SetAnimTimeRandom(object->apiobj.character_model, &object->apiobj.anim_packet);
    }
    return 1;
}

__used__ static i32 Action_SetAnimation(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL && param_5 != 0 &&
        param_4 == 1) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        const i32 animation = FindAnimIX(object->apiobj.character_data, params[0]);
        if (animation != -1) {
            ResetAnimPacket(&object->apiobj.anim_packet, animation);
        }
    }
    return 1;
}

i32 Action_MoveForward(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                       i32 first_time, f32) {
    if (player == NULL || packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 0;
    }
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (first_time != 0) {
        processor->action_data_4 = static_cast<f32>(object->apiobj.field_0x276);
        i32 random_direction = 0;
        i32 turn = 0;
        i32 minimum_turn = 0;
        i32 maximum_turn = 0;
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = NuStrIStr(params[index], "min_turn");
            if (value != NULL) {
                minimum_turn = static_cast<i32>(AIParamToFloat(processor, value + 9)) * (65536 / 360.0f);
                continue;
            }
            value = NuStrIStr(params[index], "max_turn");
            if (value != NULL) {
                maximum_turn = static_cast<i32>(AIParamToFloat(processor, value + 9)) * (65536 / 360.0f);
                continue;
            }
            if (NuStrIStr(params[index], "rand_turn_dir") != NULL) {
                random_direction = 1;
                continue;
            }
            value = NuStrIStr(params[index], "turn");
            if (value != NULL) {
                turn = static_cast<i32>(AIParamToFloat(processor, value + 5)) * (65536 / 360.0f);
            }
        }
        if (maximum_turn != 0) {
            turn = static_cast<i32>(static_cast<f32>(maximum_turn - minimum_turn) * NuRandFloat()) + minimum_turn;
        }
        if (turn != 0) {
            if (random_direction && (NuRand(NULL) & 1) != 0) {
                turn = -turn;
            }
            processor->action_data_4 = static_cast<f32>(NuAngAdd(static_cast<NUANG>(processor->action_data_4), turn));
        }
    }
    if ((packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) != 0) {
        NUVEC destination = {0.0f, 0.0f, 10.0f};
        NuVecRotateY(&destination, &destination, static_cast<NUANG>(processor->action_data_4));
        NuVecAdd(&destination, &destination, &object->apiobj.position);
        AIMoveInstruction(packet, &destination, 0.0f, &packet->path_info, AIPACKET_MOVEMENT_TO_DESTINATION, 0.01f);
    }
    return 0;
}

static i32 Action_SnapToSockPosition(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_count, i32 first_time, f32) {
    NUVEC position = {0.0f, 0.0f, 0.0f};
    if (first_time == 0)
        return 1;
    GameObject *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 from_end = 0;
    i32 party = 0;
    i32 sock_index = 0;
    f32 distance = 0.0f;
    f32 offset_scale = 0.0f;
    for (i32 index = 0; index < param_count; ++index) {
        char *value;
        if (NuStrIStr(params[index], "player1") != NULL || NuStrIStr(params[index], "player") != NULL)
            object = player;
        else if (NuStrIStr(params[index], "player2") != NULL)
            object = player2;
        else if (NuStrIStr(params[index], "party") != NULL)
            party = 1;
        else if ((value = NuStrIStr(params[index], "roty")) != NULL)
            AIParamToFloat(processor, value + 5);
        else if ((value = NuStrIStr(params[index], "distance_from_end")) != NULL) {
            distance = AIParamToFloat(processor, value + 18);
            from_end = 1;
        } else if ((value = NuStrIStr(params[index], "distance")) != NULL)
            distance = AIParamToFloat(processor, value + 9);
        else if ((value = NuStrIStr(params[index], "sock")) != NULL)
            sock_index = static_cast<i32>(AIParamToFloat(processor, value + 5));
        else if ((value = NuStrIStr(params[index], "dx")) != NULL)
            AIParamToFloat(processor, value + 3);
        else if ((value = NuStrIStr(params[index], "dy")) != NULL)
            AIParamToFloat(processor, value + 3);
    }
    SOCKPOSITION sock_position;
    if (from_end != 0) {
        sock_position.location.sock = -1;
        if (WORLD->sock_sys == NULL || static_cast<u32>(sock_index) > 63)
            return 1;
        SetSockPostion(WORLD->sock_sys, &sock_position, sock_index, WORLD->sock_sys->sock[sock_index].length - 1, 1.0f);
    } else {
        SetSockPostion(WORLD->sock_sys, &sock_position, sock_index, 0, 0.0f);
    }
    if (sock_position.location.sock == -1)
        return 1;
    MoveSockPosition(WORLD->sock_sys, &sock_position, distance, &sock_position);
    f32 random_x = NuRandFloat();
    f32 random_x2 = NuRandFloat();
    random_x = random_x * offset_scale + (1.0f - random_x2) * offset_scale;
    f32 random_y = NuRandFloat();
    f32 random_y2 = NuRandFloat();
    random_y = random_y * offset_scale + (1.0f - random_y2) * offset_scale;
    i32 random_angle = NuRand(NULL) % 65536;
    for (i32 index = 0; index < 8; ++index) {
        if (party != 0)
            object = Player[index];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001) {
            f32 x = random_x;
            f32 y = random_y;
            if (index > 1) {
                NUVEC spread = {1.0f, 0.0f, 0.0f};
                NuVecRotateZ(&spread, &spread, NuAngAdd(random_angle, (index << 16) / 3));
                y += spread.y;
                x += spread.x;
            }
            if (y != offset_scale || x != offset_scale) {
                position.x = x;
                position.y = y;
                position.z = 0.0f;
                NuVecRotateX(&position, &position, sock_position.midpoint_rotation.x);
                NuVecRotateY(&position, &position, NuAngAdd(sock_position.midpoint_rotation.y, 0x8000));
            } else {
                position.x = position.y = position.z = offset_scale;
            }
            NuVecAdd(&position, &position, &sock_position.midpoint);
            object->apiobj.field_0x276 = sock_position.midpoint_rotation.y;
            object->apiobj.facing_angle = sock_position.midpoint_rotation.y;
            object->apiobj.movement_facing_angle = sock_position.midpoint_rotation.y;
            object->apiobj.position = position;
            object->apiobj.initial_position = position;
            object->apiobj.collision_position = position;
            plr_lastpos = position;
            object->apiobj.start_position = position;
            object->apiobj.respawn_position = position;
            object->apiobj.last_safe_position = position;
            object->ai_update_position = position;
            object->apiobj.velocity = v000;
            extern void InitSurfaceInfo(GameObject *);
            extern i32 SetObjOnSurface(GameObject *, i32);
            InitSurfaceInfo(object);
            SetObjOnSurface(object, 0);
        }
        if (party == 0)
            break;
    }
    return 1;
}

__used__ static i32 Action_GoToOriginalPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    NUVEC difference;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    if (sys == NULL || sys->path_sys == NULL || (object->apiobj.field_0x1f4 & 0x400) == 0 ||
        object->ai.field_0x134 == 0xff) {
        return 0;
    }

    AIPATH *current_path = packet->path_info.path;
    AIPATH *original_path = sys->creatures[packet->field_0x134].path_info.path;
    if (param_5 != 0) {
        if (current_path == original_path) {
            return 1;
        }
        for (i32 index = 0; index < param_4; ++index) {
            if (AIActionParseSpeedFn != NULL) {
                AIActionParseSpeedFn(params[index], &packet->goal_speed_mode);
            }
        }
        current_path = packet->path_info.path;
        packet->goal_path_node = NULL;
        if (current_path == NULL) {
            return 0;
        }

        if (sys->path_sys->active_path != current_path || original_path == NULL || (original_path->flags & 2) == 0 ||
            original_path->node_count == 0) {
            return 0;
        }

        AIPATHNODE *node = original_path->nodes;
        i32 node_index = 0;
        while (node_index < original_path->node_count && (node->runtime_flags & 1) == 0) {
            ++node_index;
            ++node;
        }
        if (node_index >= original_path->node_count) {
            return 1;
        }

        processor->action_data_3 = node;
        AISysCharacterSetPath(packet, current_path);

        AIPATHCNX *connection = &sys->path_sys->active_path->connections[static_cast<u16>(node->path_flags)];
        AIPATHINFO &move_path = processor->path_info;
        AIPATHNODE *target_node = &move_path.path->nodes[connection->node_indices[0]];
        move_path.connection = connection;
        move_path.flags |= AIPATHINFO_FLAG_ON_PATH;
        move_path.direction = 0;

        difference.x = node->position.x - target_node->position.x;
        difference.z = node->position.z - target_node->position.z;
        NuVecRotateY(&difference, &difference, -connection->rotation);
        move_path.dist = difference.z / connection->max_horizontal_distance;
        move_path.width = difference.x;
        AIMoveInstruction(packet, &node->position, 0.0f, &move_path, AIPACKET_MOVEMENT_TO_DESTINATION,
                          packet->movement_instruction_parameter);
        return 0;
    }

    AIPATHNODE *node = static_cast<AIPATHNODE *>(processor->action_data_3);
    if (node == NULL) {
        return 0;
    }

    const f32 distance = NuVecXZDistSqr(&packet->terrain_origin, &node->position, &difference);
    if (distance < node->radius_squared) {
        memset(&packet->path_info, 0, sizeof(packet->path_info));
        AISysCharacterSetPath(packet, original_path);
        AISysCharacterSetPathCnx(packet, &packet->owner->apiobj.position, packet->fallback_path_info.connection, 0);
        return 1;
    }

    AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                      packet->movement_instruction_parameter);
    return 0;
}

__used__ static i32 Action_GoToLevelPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)params;
    (void)param_4;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL) {
        return 1;
    }

    if (param_5 != 0) {
        if (sys == NULL || packet->path_info.path == NULL || packet->path_info.connection == NULL) {
            return 0;
        }
        if (packet->path_info.path != sys->path_sys->active_path) {
            AISysCharacterSetPath(packet, sys->path_sys->active_path);
            AISysGetCharacterPathPos(WORLD->ai_sys, &packet->owner->apiobj, packet, 0xff, 1);
        }
        return 1;
    }

    AIPATHNODE *node = static_cast<AIPATHNODE *>(processor->action_data_3);
    if (node == NULL) {
        return 0;
    }
    if (NuVecXZDistSqr(&packet->owner->apiobj.position, &node->position, NULL) < node->radius_squared) {
        memset(&packet->path_info, 0, sizeof(packet->path_info));
        AISysCharacterSetPath(packet, sys->path_sys->active_path);
        if ((node->runtime_flags & 1) != 0) {
            AISysCharacterSetPathCnx(packet, &packet->owner->apiobj.position,
                                     &packet->path_info.path->connections[static_cast<u16>(node->path_flags)], 0);
        }
        return 1;
    }
    AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                      packet->movement_instruction_parameter);
    return 0;
}

static void *Condition_AreaCompleteInit(AISYS *, char *name, AISCRIPT *) {
    return name != NULL ? Area_FindByName(name, NULL) : NULL;
}

static void *Condition_CharacterLoadedInit(AISYS *, char *argument, AISCRIPT *) {
    if (argument == NULL) {
        return NULL;
    }
    return reinterpret_cast<void *>(static_cast<isize>(CharIDFromName(argument)));
}

static f32 Condition_CharacterLoaded(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    if (argument != NULL && APICharacterLoaded(static_cast<i32>(reinterpret_cast<isize>(argument))) != NULL) {
        return 1.0f;
    }
    return 0.0f;
}

static void *Condition_FlowBoxCompleteInit(AISYS *, char *name, AISCRIPT *) {
    return name != NULL ? FlowBoxFindByName(WORLD->giz_flow, name) : NULL;
}

static f32 Condition_GizmoVisibility(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    f32 visibility = 0.0f;
    if (argument != NULL)
        visibility = static_cast<f32>(GizmoGetVisibility(WORLD->gizmo_sys, static_cast<GIZMO *>(argument)));
    return visibility;
}

static f32 Condition_GizmoOutput3(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    f32 output = 0.0f;
    if (argument != NULL)
        output =
            static_cast<f32>(static_cast<u32>(GizmoGetOutput(WORLD->gizmo_sys, static_cast<GIZMO *>(argument), 3, 1)));
    return output;
}

static f32 Condition_GizmoOutput2(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    f32 output = 0.0f;
    if (argument != NULL)
        output =
            static_cast<f32>(static_cast<u32>(GizmoGetOutput(WORLD->gizmo_sys, static_cast<GIZMO *>(argument), 2, 1)));
    return output;
}

static f32 Condition_GizmoOutput1(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    f32 output = 0.0f;
    if (argument != NULL)
        output =
            static_cast<f32>(static_cast<u32>(GizmoGetOutput(WORLD->gizmo_sys, static_cast<GIZMO *>(argument), 1, 1)));
    return output;
}

static f32 Condition_GizmoOutput0(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    f32 output = 0.0f;
    if (argument != NULL)
        output =
            static_cast<f32>(static_cast<u32>(GizmoGetOutput(WORLD->gizmo_sys, static_cast<GIZMO *>(argument), 0, 1)));
    return output;
}

static f32 Condition_BlowupBlownup(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *argument) {
    if (argument != NULL && GizmoGetOutput(WORLD->gizmo_sys, static_cast<GIZMO *>(argument), 0, 1) != 0)
        return 1.0f;
    return 0.0f;
}

static f32 Condition_ShopActive(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return GetMenuID() == 13 ? 1.0f : 0.0f;
}

static f32 Condition_LocatorOnScreen(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *argument) {
    AILOCATOR *locator = static_cast<AILOCATOR *>(argument);
    if (locator == NULL)
        locator = processor->locator;
    if (locator != NULL && NuCameraClipTestSphere(&locator->position, 0.0f, &numtx_identity) == 0)
        return 1.0f;
    return 0.0f;
}

static f32 Condition_EitherPlayerLocatorRangeXZ(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                                void *argument) {
    NUVEC difference;
    if (packet != NULL && packet->owner != NULL) {
        AILOCATOR *locator = static_cast<AILOCATOR *>(argument);
        if (locator == NULL) {
            locator = processor->locator;
        }
        if (locator != NULL) {
            f32 first_distance = 1.0e9f;
            f32 second_distance = 1.0e9f;
            if (player != NULL) {
                first_distance = NuVecXZDist(&player->apiobj.position, &locator->position, &difference);
            }
            if (player2 != NULL) {
                second_distance = NuVecXZDist(&player2->apiobj.position, &locator->position, &difference);
            }
            return NuFmin(first_distance, second_distance);
        }
    }
    return 1.0e9f;
}

static i32 Action_ClearTakeOverTarget(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (object != NULL) {
            GameObject_s *target = object->takeover_target;
            if (target != NULL) {
                object->takeover_target = NULL;
                target->takeover_target = NULL;
            }
        }
    }
    return 1;
}

static i32 Action_PartyCanBeUnderCover(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count,
                                       i32 first_time, f32) {
    if (first_time) {
        party_cant_be_under_cover = 0;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0)
                party_cant_be_under_cover = 1;
        }
    }
    return 1;
}

__used__ static i32 Action_DontRaycastLOS(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 != 0 && WORLD->api_object_sys != NULL) {
        WORLD->api_object_sys->state[0x208] |= 1;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                WORLD->api_object_sys->state[0x208] &= static_cast<u8>(~1u);
            }
        }
    }
    return 1;
}

static void *Condition_SideInit(AISYS *, char *arg, AISCRIPT *) {
    if (arg != NULL) {
        if (NuStrICmp(arg, "baddy") == 0 || NuStrICmp(arg, "baddie") == 0)
            return reinterpret_cast<void *>(static_cast<isize>(-1));
        if (NuStrICmp(arg, "goody") == 0 || NuStrICmp(arg, "goodie") == 0)
            return reinterpret_cast<void *>(static_cast<isize>(1));
        if (NuStrICmp(arg, "goodybaddy") == 0 || NuStrICmp(arg, "goodiebaddie") == 0)
            return reinterpret_cast<void *>(static_cast<isize>(2));
    }
    return NULL;
}

static void *Condition_BeenToLevelInit(AISYS *system, char *arg, AISCRIPT *) {
    if (arg == NULL || system == NULL || WORLD->area == NULL) {
        return reinterpret_cast<void *>(static_cast<isize>(-1));
    }
    for (i32 area_level = 0; area_level < WORLD->area->level_count; ++area_level) {
        i32 level = WORLD->area->levels[area_level];
        if (NuStrICmp(arg, LDataList[level].name) == 0) {
            return reinterpret_cast<void *>(static_cast<isize>(area_level));
        }
    }
    return reinterpret_cast<void *>(static_cast<isize>(-1));
}

static void *Condition_OpponentIsAInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (arg != NULL && sys != NULL) {
        for (i32 index = 0; index < CHARCOUNT; ++index) {
            if (NuStrICmp(CDataList[index].file, arg) == 0)
                return reinterpret_cast<void *>(static_cast<isize>(index));
        }
    }
    return reinterpret_cast<void *>(static_cast<isize>(-1));
}

static void *Condition_IAmAInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (arg != NULL) {
        if (sys != NULL) {
            for (i32 index = 0; index < CHARCOUNT; ++index) {
                if (NuStrICmp(CDataList[index].file, arg) == 0)
                    return reinterpret_cast<void *>(static_cast<isize>(index));
            }
        }
    }
    return reinterpret_cast<void *>(static_cast<isize>(-1));
}

__used__ static i32 Action_SetCanTakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0 && packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp("True", params[index]) == 0) {
                object->field_0xeff |= 0x40;
            } else if (NuStrIStr(params[index], "false") != NULL) {
                object->field_0xeff &= ~0x40;
            }
        }
    }
    return 1;
}

static i32 Action_SetZeroAcceleration(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                      i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrIStr(params[index], "player1") != NULL || NuStrIStr(params[index], "player") != NULL)
                object = player;
            else if (NuStrIStr(params[index], "player2") != NULL)
                object = player2;
            else if (NuStrIStr(params[index], "FALSE") != NULL)
                enabled = 0;
        }
        if (object != NULL)
            object->ai.runtime_flags = (object->ai.runtime_flags & ~4u) | ((enabled & 1) << 2);
    }
    return 1;
}

static i32 Action_SetLastSafePathPos(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                     i32 param_count, i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    APIOBJECT_s *object = packet != NULL && packet->owner != NULL ? &packet->owner->apiobj : NULL;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "character");
        if (value != NULL) {
            if (GetNamedAPIObjectFn != NULL)
                object = GetNamedAPIObjectFn(system, value + 10);
        } else {
            NuStrICmp("FALSE", params[index]);
        }
    }
    if (object != NULL) {
        object->respawn_position = object->position;
        object->last_safe_position = object->position;
    }
    return 1;
}

static i32 Action_SetDefaultMovementRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                          i32 param_count, i32 first_time, f32) {
    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            DEFAULT_MOVE_RANGE = AIParamToFloat(processor, params[index]);
        }
    }
    return 1;
}

static i32 Action_SetMaxMovementRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }

    f32 range = 0.0f;
    i32 range_type = 1;
    i32 all_non_party = 0;
    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrICmp("Default", params[index]) == 0) {
            range = DEFAULT_MOVE_RANGE;
        } else if (NuStrICmp("All_Non_Party", params[index]) == 0) {
            all_non_party = 1;
        } else if (NuStrICmp("Locator", params[index]) == 0) {
            range_type = 2;
        } else {
            range = AIParamToFloat(processor, params[index]);
        }
    }

    if (all_non_party) {
        GameObject_s *object = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
            if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                    (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
                (object->apiobj.field_0x1f4 & 0x400) == 0) {
                continue;
            }
            object->ai.movement_target_radius = range;
            object->ai.movement_event_flags = (object->ai.movement_event_flags & 0xf3u) | (range > 0.0f ? 4u : 0u);
        }
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->ai.movement_target_radius = range;
        if (range > 0.0f)
            object->ai.movement_event_flags =
                (object->ai.movement_event_flags & 0xf3u) | (static_cast<u8>(range_type) << 2);
        else
            object->ai.movement_event_flags &= 0xf3u;
    }
    return 1;
}

__used__ static i32 Action_SetScriptParam(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                          i32 num_params, i32 first_time, f32) {
    if (first_time && num_params != 0 && processor->script != NULL) {
        f32 amount = 0.0f;
        i32 operation = 0;
        i32 index = -1;
        for (i32 i = 0; i < num_params; i++) {
            char *value = NuStrIStr(params[i], "name=");
            if (value != NULL) {
                value += 5;
                for (i32 j = 0; j < 4; j++) {
                    if (NuStrICmp(processor->script->params[j].name, value) == 0) {
                        index = j;
                        break;
                    }
                }
            } else if ((value = NuStrIStr(params[i], "ix=")) != NULL) {
                index = (i32)AIParamToFloat(processor, value + 3);
                if (index >= 4)
                    index = -1;
            } else if ((value = NuStrIStr(params[i], "value=")) != NULL) {
                amount = AIParamToFloat(processor, value + 6);
            } else if ((value = NuStrIStr(params[i], "increment=")) != NULL) {
                amount = AIParamToFloat(processor, value + 10);
                operation = 1;
            } else if ((value = NuStrIStr(params[i], "decrement=")) != NULL) {
                amount = AIParamToFloat(processor, value + 10);
                operation = -1;
            }
        }
        if (index >= 0) {
            switch (operation) {
                case 0:
                    processor->params[index] = amount;
                    break;
                case 1:
                    processor->params[index] = amount + processor->params[index];
                    break;
                case -1:
                    processor->params[index] -= amount;
                    break;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_AddToSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (param_5 != 0 && object != NULL && param_4 > 0) {
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "Reset") == 0) {
                object->ai.creature_set = 0;
                continue;
            }
            const i32 creature_set = static_cast<i32>(AIParamToFloat(processor, params[index]));
            if (static_cast<u32>(creature_set - 1) < 16) {
                object->ai.creature_set = static_cast<u8>(creature_set);
                ++aicreature_sets_alive[creature_set - 1];
            }
        }
    }
    return 1;
}

__used__ static i32 Action_TurnOnPickup(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0 && param_4 > 0) {
        GIZMOPICKUP_s *pickup = NULL;
        for (i32 index = 0; index != param_4; ++index) {
            char *value = NuStrIStr(params[index], "name=");
            if (value != NULL) {
                pickup = GizmoPickup_FindByName(WORLD, value + NuStrLen("name="));
            }
        }
        if (pickup != NULL) {
            GizmoPickup_TurnOnPickup(pickup);
        }
    }
    return 1;
}

static i32 Action_CanTriggerObstacle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time == 0 || param_count == 0) {
        return 1;
    }
    GIZOBSTACLE *obstacle = NULL;
    i32 blocked = 0;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, value + 5);
            if (gizmo != NULL) {
                obstacle = static_cast<GIZOBSTACLE *>(gizmo->object);
            }
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            blocked = 1;
        }
    }
    if (obstacle != NULL) {
        obstacle->runtime_flags = static_cast<u8>((obstacle->runtime_flags & ~8u) | ((blocked & 1) << 3));
    }
    return 1;
}

static i32 Action_AlwaysTriggerObstacle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count,
                                        i32 first_time, f32) {
    if (first_time == 0 || param_count == 0) {
        return 1;
    }
    GIZOBSTACLE *obstacle = NULL;
    i32 enabled = 1;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, value + 5);
            if (gizmo != NULL) {
                obstacle = static_cast<GIZOBSTACLE *>(gizmo->object);
            }
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            enabled = 0;
        }
    }
    if (obstacle != NULL) {
        obstacle->runtime_flags = static_cast<u8>((obstacle->runtime_flags & ~4u) | ((enabled & 1) << 2));
    }
    return 1;
}

static i32 Action_GizmoSetVisibility(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    GIZMO *gizmo = NULL;
    i32 visible = 1;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            gizmo = GizmoFindByName(WORLD->gizmo_sys, -1, value + NuStrLen("name="));
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            visible = 0;
        }
    }
    if (gizmo != NULL) {
        GizmoSetVisibility(WORLD->gizmo_sys, gizmo, visible, 1);
    }
    return 1;
}

__used__ static i32 Action_GizmoActivate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0) {
        if (param_4 > 0) {
            GIZMO *gizmo = NULL;
            i32 enabled = 1;
            for (i32 index = 0; index < param_4; ++index) {
                char *value = NuStrIStr(params[index], "name=");
                if (value != NULL) {
                    gizmo = GizmoFindByName(WORLD->gizmo_sys, -1, value + NuStrLen("name="));
                } else if (NuStrICmp(params[index], "FALSE") == 0) {
                    enabled = 0;
                }
            }
            if (gizmo != NULL) {
                GizmoActivate(WORLD->gizmo_sys, gizmo, enabled, 1);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CircleLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 elapsed) {
    (void)elapsed;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL ||
        packet->path_info.path == NULL || packet->path_info.connection == NULL) {
        return 1;
    }
    if (first_time != 0) {
        processor->action_data_1 = 0;
        processor->action_data_3 = processor->locator;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (AIActionParseSpeedFn != NULL &&
                    AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                    continue;
                }
                char *value = NuStrIStr(params[index], "name=");
                if (value == NULL) {
                    value = NuStrIStr(params[index], "teleport");
                }
                if (value != NULL) {
                    processor->action_data_3 = AIPathFindLocator(sys, value + NuStrLen("name="));
                } else if ((value = NuStrIStr(params[index], "goalrange")) != NULL) {
                    packet->movement_instruction_parameter =
                        AIParamToFloatEx(packet, processor, value + NuStrLen("goalrange") + 1);
                } else if (NuStrICmp(params[index], "reverse") == 0) {
                    packet->field_0x1e5 ^= 2;
                } else if (NuStrICmp(params[index], "anticlockwise") == 0) {
                    packet->field_0x1e5 &= static_cast<u8>(~2u);
                } else if (NuStrICmp(params[index], "clockwise") == 0) {
                    packet->field_0x1e5 |= 2;
                } else {
                    packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
                }
            }
        }
    }
    AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
    if (locator != NULL) {
        AIMoveInstruction(packet, &locator->position, packet->movement_instruction_parameter, &locator->path_info,
                          AIPACKET_MOVEMENT_CIRCLE, packet->movement_instruction_parameter);
    }
    return locator == NULL;
}

__used__ static i32 Action_CompleteLevel(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0) {
        char *cutscene = NULL;
        LEVELDATA *level = NULL;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "cutscene=");
            if (value != NULL) {
                cutscene = value + NuStrLen("cutscene=");
            }
            value = NuStrIStr(params[index], "newlevel=");
            if (value != NULL) {
                level = Level_FindByName(value + NuStrLen("newlevel="), NULL);
            }
        }
        if (FreePlay == 0) {
            if (cutscene != NULL && NewCutScene(NULL, WORLD->cutscene_sys, cutscene, 0) != NULL) {
                return 1;
            }
        }
        if (FreePlay != 0 && level != NULL && (level->flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) != 0) {
            level = Area_FindStatusLevel(WORLD->area, NULL);
        }
        if (level == NULL) {
            CompleteLevel(WORLD);
        } else {
            GoToNewLevel(level->idx);
        }
    }
    return 1;
}

__used__ static i32 Action_InitRowDist(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)packet;
    (void)param_6;
    if (param_5 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "Dist");
            if (value != NULL) {
                oneAtOnce_SetInitDistPerRow(AIParamToFloat(processor, value + 4 + 1));
            }
        }
    }
    return 1;
}

static i32 Action_SetAtOnceRowDistance(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                       i32 first_time, f32) {
    if (first_time) {
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "Dist");
            if (value != NULL)
                oneAtOnce_SetDistPerRow(AIParamToFloat(processor, value + 5));
        }
    }
    return 1;
}

static i32 Action_SetAttackersPerRow(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    if (param_count == 0) {
        return 1;
    }
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "num");
        if (value != NULL)
            oneAtOnce_SetAttackersPerRow((i32)AIParamToFloat(processor, value + 4));
    }
    return 1;
}

static i32 Action_SetAttackersAtOnce(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    if (param_count == 0) {
        return 1;
    }
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "max");
        if (value != NULL)
            oneAtOnce_SetNumAttackers((i32)AIParamToFloat(processor, value + 4));
    }
    return 1;
}

__used__ static i32 Action_SetLocatorSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (param_5 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "name=");
            if (value != NULL) {
                processor->unknown_a8 = AIPathFindLocatorSet(WORLD->ai_sys, value + 5);
            }
        }
    }
    return 1;
}

static i32 Action_AddScriptProcessor(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time && param_count > 0) {
        char *script_name = NULL;
        char *name = NULL;
        AIAREA_s *area = NULL;
        AILOCATORSET_s *locator_set = NULL;
        AILOCATOR_s *locator = NULL;
        i32 set = 0;
        NUGSPLINE *spline = NULL;
        i32 override_count = 0;
        char override_names[4][32];
        f32 override_values[4];
        for (i32 index = 0; index < param_count; ++index) {
            char *value;
            if ((value = NuStrIStr(params[index], "script=")) != NULL) {
                value += 7;
                if (AIScriptFind(WORLD->ai_sys, value, 0, 1, 1) != NULL)
                    script_name = value;
            } else if ((value = NuStrIStr(params[index], "name=")) != NULL) {
                name = value + 5;
            } else if ((value = NuStrIStr(params[index], "area=")) != NULL) {
                area = AISysFindArea(sys, value + 5);
            } else if ((value = NuStrIStr(params[index], "locator_set=")) != NULL) {
                locator_set = AIPathFindLocatorSet(sys, value + 12);
            } else if ((value = NuStrIStr(params[index], "locator=")) != NULL) {
                locator = AIPathFindLocator(sys, value + 8);
            } else if ((value = NuStrIStr(params[index], "set=")) != NULL) {
                set = (i32)AIParamToFloat(processor, value + 4);
                if ((u32)set >= 17)
                    set = 0;
            } else if ((value = NuStrIStr(params[index], "spline=")) != NULL) {
                spline = NuSplineFind(WORLD->current_gscn, value + 7);
            } else if ((value = NuStrIStr(params[index], "param")) != NULL && override_count < 4) {
                NuStrNCpy(override_names[override_count], value + 6, 32);
                char *separator = NuStrIStr(override_names[override_count], "=");
                if (separator != NULL) {
                    override_values[override_count] = AIParamToFloat(processor, separator + 1);
                    *separator = '\0';
                    ++override_count;
                }
            }
        }
        if (script_name != NULL && WORLD->processor_count < 32) {
            AISCRIPTPROCESS *created = &WORLD->processors[WORLD->processor_count].processor;
            AIScriptProcessorInit(sys, NULL, created, NULL, script_name, NULL, 0, NULL, NULL);
            created->locator_set = locator_set;
            created->locator = locator;
            created->unknown_a0 = area;
            created->unknown_b0 = set;
            created->unknown_ac = spline;
            if (name != NULL)
                NuStrCpy(WORLD->processors[WORLD->processor_count].name, name);
            LevelScriptReStoreProgress(WORLD, &WORLD->processors[WORLD->processor_count]);
            if (override_count != 0 && created->script != NULL) {
                for (i32 index = 0; index < override_count; ++index) {
                    for (i32 slot = 0; slot < 4; ++slot) {
                        if (NuStrICmp(override_names[index], created->script->params[slot].name) == 0) {
                            created->params[slot] = override_values[index];
                            break;
                        }
                    }
                }
            }
            ++WORLD->processor_count;
            GizmoSysAddGizmos(WORLD->gizmo_sys, WORLD->giz_flow, WORLD);
        }
    }
    return 1;
}

__used__ static i32 Action_CopyMessage(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 num_params,
                                       i32 first_time, f32) {
    if (first_time && gizaimessagesys != NULL && num_params != 0) {
        char *from = NULL;
        char *to = NULL;
        for (i32 i = 0; i < num_params; i++) {
            char *value = NuStrIStr(params[i], "from=");
            if (value != NULL)
                from = value + 5;
            else if ((value = NuStrIStr(params[i], "to=")) != NULL)
                to = value + 3;
        }
        if (to != NULL && from != NULL) {
            GIZAIMESSAGE_s *source = CheckGizAIMessage(gizaimessagesys, from, NULL);
            GIZAIMESSAGE_s *destination = CheckGizAIMessage(gizaimessagesys, to, NULL);
            if (destination != NULL && source != NULL)
                destination->value = source->value;
        }
    }
    return 1;
}

__used__ static i32 Action_SetMessage(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 num_params,
                                      i32 first_time, f32) {
    if (first_time && gizaimessagesys != NULL && num_params != 0) {
        char *name = NULL;
        i32 operation = 0;
        f32 amount = 0.0f;
        for (i32 i = 0; i < num_params; i++) {
            char *value = NuStrIStr(params[i], "name=");
            if (value != NULL)
                name = value + 5;
            else if ((value = NuStrIStr(params[i], "value=")) != NULL)
                amount = AIParamToFloat(processor, value + 6);
            else if ((value = NuStrIStr(params[i], "increment=")) != NULL) {
                amount = AIParamToFloat(processor, value + 10);
                operation = 1;
            } else if ((value = NuStrIStr(params[i], "decrement=")) != NULL) {
                amount = AIParamToFloat(processor, value + 10);
                operation = -1;
            }
        }
        if (name != NULL) {
            GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, name, NULL);
            if (message != NULL) {
                if (operation == 0)
                    message->value = amount;
                else if (operation == 1)
                    message->value = amount + message->value;
                else if (operation == -1)
                    message->value -= amount;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_AddGameMsgCount(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = NULL;
        if (packet != NULL && packet->owner != NULL) {
            object = packet->owner->apiobj.objptr;
        }
        if (param_4 > 0) {
            NUVEC *position = NULL;
            i32 total = 0;
            i32 count = 0;
            u8 red = 0;
            u8 green = 0;
            u8 blue = 0;

            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp("mypos", params[index]) == 0) {
                    if (object != NULL) {
                        position = &object->apiobj.collision_position;
                    }
                } else {
                    char *value = NuStrIStr(params[index], "count=");
                    if (value != NULL) {
                        count = static_cast<i32>(AIParamToFloat(processor, value + 6));
                    } else {
                        value = NuStrIStr(params[index], "total=");
                        if (value != NULL) {
                            total = static_cast<i32>(AIParamToFloat(processor, value + 6));
                        } else {
                            value = NuStrIStr(params[index], "messageval=");
                            if (value != NULL) {
                                GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, value + 11, NULL);
                                if (message != NULL) {
                                    total = static_cast<i32>(message->value);
                                }
                            } else {
                                value = NuStrIStr(params[index], "Red=");
                                if (value != NULL) {
                                    red = static_cast<u8>(AIParamToFloat(processor, value + 4));
                                } else {
                                    value = NuStrIStr(params[index], "Green=");
                                    if (value != NULL) {
                                        green = static_cast<u8>(AIParamToFloat(processor, value + 6));
                                    } else {
                                        value = NuStrIStr(params[index], "Blue=");
                                        if (value != NULL) {
                                            blue = static_cast<u8>(AIParamToFloat(processor, value + 5));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (total > count) {
                return 1;
            }
            if (total == 0) {
                return 1;
            }
            AddGameMsgCount(position, count, total, red, green, blue, 0.75f);
        }
    }
    return 1;
}

__used__ static i32 Action_SetObstacleToEnd(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 != 0 && param_4 != 0) {
        GIZOBSTACLE *obstacle = NULL;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "name=");
            if (value != NULL) {
                obstacle = GizObstacle_FindByName(WORLD->giz_obstacle_sys, value + 5);
            }
        }
        if (obstacle != NULL) {
            GizObstacle_JumpToEnd(obstacle);
        }
    }
    return 1;
}

__used__ static i32 Action_PlayGizSpecial(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 == 0 || param_4 <= 0) {
        return 1;
    }

    GIZSPECIAL *special = NULL;
    i32 backwards = 0;
    i32 snap = 0;
    i32 restart = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, gizspecial_gizmotype_id, value + 5);
            if (gizmo != NULL) {
                special = static_cast<GIZSPECIAL *>(gizmo->object);
            }
        } else if (NuStrICmp(params[index], "backwards") == 0) {
            backwards = 1;
        } else if (NuStrICmp(params[index], "SNAP") == 0) {
            snap = 1;
        } else if (NuStrICmp(params[index], "Restart") == 0) {
            restart = 1;
        }
    }

    if (special == NULL) {
        return 1;
    }
    if (backwards != 0) {
        if (snap != 0) {
            GameAnimSet_JumpToStart(special->anim_set);
        } else {
            if (restart != 0) {
                GameAnimSet_JumpToEnd(special->anim_set);
            }
            GameAnimSet_Play(special->anim_set, -1.0f, 1);
        }
    } else if (snap != 0) {
        GameAnimSet_JumpToEnd(special->anim_set);
    } else {
        if (restart != 0) {
            GameAnimSet_JumpToStart(special->anim_set);
        }
        GameAnimSet_Play(special->anim_set, 1.0f, 1);
    }
    return 1;
}

__used__ static i32 Action_PlayGizObstacle(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 == 0 || param_4 == 0) {
        return 1;
    }

    GIZOBSTACLE *obstacle = NULL;
    i32 backwards = 0;
    i32 stay_open = 0;
    i32 stay_shut = 0;
    i32 snap = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, value + 5);
            if (gizmo != NULL) {
                obstacle = static_cast<GIZOBSTACLE *>(gizmo->object);
            }
        } else if (NuStrICmp(params[index], "backwards") == 0) {
            backwards = true;
        } else if (NuStrICmp(params[index], "stayopen") == 0) {
            stay_open = true;
        } else if (NuStrICmp(params[index], "stayshut") == 0) {
            stay_shut = true;
        } else if (NuStrICmp(params[index], "SNAP") == 0) {
            snap = true;
        }
    }

    if (obstacle != NULL) {
        if (backwards || stay_shut) {
            if (snap) {
                GizObstacle_JumpToStart(obstacle);
            } else {
                GizObstacle_PlayBackwards(obstacle);
            }
        } else if (snap) {
            GizObstacle_JumpToEnd(obstacle);
        } else {
            GizObstacle_PlayForwards(obstacle);
        }
        u32 runtime_flags = obstacle->runtime_flags & 0xf3u;
        runtime_flags |= static_cast<u32>(stay_shut) << 3;
        runtime_flags |= static_cast<u32>(stay_open) << 2;
        obstacle->runtime_flags = static_cast<u8>(runtime_flags);
    }
    return 1;
}

__used__ static i32 Action_CnxHelper(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)packet;
    (void)param_6;
    if (param_5 == 0 || param_4 <= 0) {
        return 1;
    }

    AIPATH *path = NULL;
    char *from = NULL;
    char *to = NULL;
    void *grapple = NULL;
    f32 jump_off_dy = 0.0f;
    i32 both_ways = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "pathname=");
        if (value != NULL) {
            path = AISysFindPath(sys, value + 9);
            continue;
        }
        if (NuStrIStr(params[index], "bothways") != NULL) {
            both_ways = 1;
            continue;
        }
        value = NuStrIStr(params[index], "from=");
        if (value != NULL) {
            from = value + 5;
            continue;
        }
        value = NuStrIStr(params[index], "to=");
        if (value != NULL) {
            to = value + 3;
            continue;
        }
        value = NuStrIStr(params[index], "jump_off_dy=");
        if (value != NULL) {
            jump_off_dy = AIParamToFloat(processor, value + 12);
            continue;
        }
        value = NuStrIStr(params[index], "grapple=");
        if (value != NULL && WORLD != NULL && WORLD->gizmo_sys != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, grapple_gizmotype_id, value + 8);
            if (gizmo != NULL) {
                grapple = gizmo->object;
            }
        }
    }

    if (from != NULL && to != NULL) {
        i32 direction = 0;
        AIPATHCNX *connection = static_cast<AIPATHCNX *>(AIPAthFindPathCnx(sys, path, from, to, &direction));
        if (connection != NULL) {
            if (both_ways) {
                direction = 0xff;
            }
            if (grapple != NULL && WORLD != NULL) {
                AIPATHCNXHELPER_s *helper = AIPathCnxHelperSys_AddHelper(WORLD->ai_path_cnx_helper_sys, connection,
                                                                         static_cast<u8>(direction), grapple, 1);
                if (helper != NULL) {
                    helper->jump_off_dy = jump_off_dy;
                }
            }
        }
    }
    return 1;
}

static u32 ParseAIPathCnxFlag(char *name) {
    u32 flag = StarWars_ParseAIPathCnxFlag(name);
    if (flag != 0) {
        return flag;
    }
    if (NuStrICmp(name, "BLOCK") == 0) {
        return 0x80000000u;
    }
    if (NuStrICmp(name, "BIGJUMP") == 0) {
        return static_cast<u32>(LEGO_AIPATHCNX_BIGJUMP);
    }
    if (NuStrICmp(name, "REQUIRESPERMISSION") == 0) {
        return static_cast<u32>(LEGO_AIPATHCNX_REQUIRESPERMISSION);
    }
    if (NuStrICmp(name, "NO_DESTINATION_CHECK") == 0) {
        return static_cast<u32>(LEGO_AIPATHCNX_NO_DESTINATION_CHECK);
    }
    return 0;
}

__used__ static i32 Action_CnxController(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 param_6) {
    (void)packet;
    (void)param_6;
    if (first_time == 0) {
        return 1;
    }

    AIPATH *path = NULL;
    char *from = NULL;
    char *to = NULL;
    char *target_name = NULL;
    i32 target_type = 0;
    i32 fake_animation_id = -1;
    i32 gizmo_output = 0;
    u32 on_flags = 0;
    u32 off_flags = 0;
    i32 both_ways = 0;
    i32 check_visible = 0;
    i32 on_obstacle_open = 0;
    i32 off_obstacle_open = 0;

    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrIStr(params[index], "on_frames") != NULL) {
            continue;
        }
        char *match = NuStrIStr(params[index], "from=");
        if (match != NULL) {
            from = match + 5;
            continue;
        }
        match = NuStrIStr(params[index], "to=");
        if (match != NULL) {
            to = match + 3;
            continue;
        }
        match = NuStrIStr(params[index], "pathname=");
        if (match != NULL) {
            path = AISysFindPath(sys, match + 9);
            continue;
        }
        if (NuStrIStr(params[index], "CheckVisible") != NULL) {
            check_visible = 1;
            continue;
        }
        if (NuStrIStr(params[index], "bothways") != NULL) {
            both_ways = 1;
            continue;
        }

        match = NuStrIStr(params[index], "on_flag");
        if (match != NULL) {
            char *value = match + 8;
            if (NuStrIStr(params[index], "OBSTACLE_OPEN") != NULL) {
                on_obstacle_open = 1;
            } else if (NuStrIStr(params[index], "OBSTACLE_CLOSED") != NULL) {
                on_obstacle_open = 0;
            } else {
                on_flags |= ParseAIPathCnxFlag(value);
            }
            continue;
        }
        match = NuStrIStr(params[index], "off_flag");
        if (match != NULL) {
            char *value = match + 9;
            if (NuStrIStr(params[index], "OBSTACLE_OPEN") != NULL) {
                off_obstacle_open = 1;
            } else if (NuStrIStr(params[index], "OBSTACLE_CLOSED") != NULL) {
                off_obstacle_open = 0;
            } else {
                off_flags |= ParseAIPathCnxFlag(value);
            }
            continue;
        }

        if ((match = NuStrIStr(params[index], "obj=")) != NULL) {
            target_name = match + 4;
            target_type = 0;
            continue;
        }
        if ((match = NuStrIStr(params[index], "cutscene=")) != NULL) {
            target_name = match + 9;
            target_type = 1;
            continue;
        }
        if ((match = NuStrIStr(params[index], "buildit=")) != NULL) {
            target_name = match + 8;
            target_type = 2;
            continue;
        }
        match = NuStrIStr(params[index], "fakeanimid=");
        if (match != NULL) {
            fake_animation_id = static_cast<i32>(AIParamToFloat(processor, match + 11));
            target_type = 5;
            continue;
        }
        match = NuStrIStr(params[index], "gizmo_output=");
        if (match != NULL) {
            gizmo_output = static_cast<i32>(AIParamToFloat(processor, match + 13));
            continue;
        }
        if ((match = NuStrIStr(params[index], "gizmo=")) != NULL) {
            target_name = match + 6;
            target_type = 3;
        } else if ((match = NuStrIStr(params[index], "flowbox=")) != NULL) {
            target_name = match + NuStrLen("flowbox=");
            target_type = 6;
        } else if ((match = NuStrIStr(params[index], "blowup=")) != NULL) {
            target_name = match + 7;
            target_type = 4;
        } else if ((match = NuStrIStr(params[index], "force=")) != NULL) {
            target_name = match + 6;
            target_type = 7;
        } else if ((match = NuStrIStr(params[index], "obstacle=")) != NULL) {
            target_name = match + 9;
            target_type = 8;
        } else if ((match = NuStrIStr(params[index], "zipup=")) != NULL) {
            target_name = match + 6;
            target_type = 9;
        }
    }

    AIPATHCNXCONTROLLER_s *controller =
        AIPathCnxControllerCreate(WORLD->ai_path_cnx_control_sys, WORLD->ai_sys, path, from, to, target_type,
                                  target_name, fake_animation_id, gizmo_output);
    if (controller == NULL) {
        return 1;
    }
    controller->both_ways = both_ways;
    controller->on_flags = on_flags;
    controller->off_flags = off_flags;
    controller->check_visible = check_visible;
    controller->on_obstacle_open = on_obstacle_open;
    controller->off_obstacle_open = off_obstacle_open;

    for (i32 index = 0; index < param_count; ++index) {
        char *match = NuStrIStr(params[index], "on_frames");
        if (match == NULL) {
            continue;
        }
        char range[64] __attribute__((aligned(16)));
        NuStrCpy(range, match + 10);
        char *separator = NuStrIStr(range, "to");
        if (separator == NULL) {
            continue;
        }
        *separator = '\0';
        char *end_text = separator + 2;
        i32 start_frame = NuStrICmp(range, "lastframe-1") == 0 ? -2
                          : NuStrICmp(range, "lastframe") == 0 ? -1
                                                               : static_cast<i32>(AIParamToFloat(processor, range));
        i32 end_frame = NuStrICmp(end_text, "lastframe-1") == 0 ? -2
                        : NuStrICmp(end_text, "lastframe") == 0 ? -1
                                                                : static_cast<i32>(AIParamToFloat(processor, end_text));
        if (end_frame != -1) {
            if (end_frame != -2 && end_frame < start_frame) {
                continue;
            }
        }
        AIPathCnxControllerSetOnRange(controller, start_frame, end_frame);
    }
    return 1;
}

static void *Condition_CategoryIsInit(AISYS *system, char *arg, AISCRIPT *) {
    isize category = -1;
    if (arg != NULL && system != NULL && CharCategory != NULL) {
        category = CharCategory_FindByName(arg);
    }
    return reinterpret_cast<void *>(category);
}

__used__ static i32 Action_SetLevelPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 == 0 || sys->path_sys == NULL) {
        return 1;
    }

    char *path_name = NULL;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], const_cast<char *>("name"));
        if (value != NULL) {
            path_name = value + 5;
        }
    }

    if (AISysSetLevelPath(sys, path_name) == 0) {
        return 1;
    }

    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object = Player[index];
        if (object == NULL || (object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                                  (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) {
            continue;
        }
        AIPACKET *object_packet = &object->ai;
        AISysCharacterSetPath(object_packet, sys->path_sys->active_path);
        AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, object_packet, 0xff,
                                 static_cast<i8>(object->apiobj.field_0x27d));
    }
    return 1;
}

__used__ static i32 Action_ResetGameCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0) {
        GameCam_Reset(GameCam);
    }
    return 1;
}

__used__ static i32 Action_CameraShake(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)packet;
    (void)param_6;
    if (param_5 != 0) {
        f32 amount = 1.0f;
        f32 duration = 0.0f;
        f32 speed = 1.0f;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "mul=");
            if (value != NULL) {
                amount = AIParamToFloat(processor, value + 4);
            } else if ((value = NuStrIStr(params[index], "time=")) != NULL) {
                duration = AIParamToFloat(processor, value + 5);
            } else if ((value = NuStrIStr(params[index], "speed=")) != NULL) {
                speed = AIParamToFloat(processor, value + 6);
            }
        }
        GameCam_NewShake(GameCam, amount, duration, speed);
    }
    return 1;
}

__used__ static i32 Action_JudderGameCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)packet;
    (void)param_6;
    if (param_5 != 0) {
        i32 axis = 0;
        f32 time = 0.1f;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrIStr(params[index], "axis=x") != NULL) {
                axis = 0;
            } else if (NuStrIStr(params[index], "axis=y") != NULL) {
                axis = 1;
            } else if (NuStrIStr(params[index], "axis=z") != NULL) {
                axis = 2;
            } else {
                char *parameter = NuStrIStr(params[index], "time");
                if (parameter != NULL) {
                    time = AIParamToFloat(processor, parameter + 5);
                }
            }
        }
        if (axis == 2 && qrand() < 0x8000) {
            time = -time;
        }
        GameCam_Judder(GameCam, time, axis, NULL);
    }
    return 1;
}

__used__ static i32 Action_EnableSock(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)packet;
    (void)param_6;
    if (param_5 != 0) {
        i32 sock_index = -1;
        i32 enabled = 1;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "ix=");
            if (value != NULL) {
                sock_index = static_cast<i32>(AIParamToFloat(processor, value + 3));
            } else if ((value = NuStrIStr(params[index], "name=")) != NULL) {
                SOCK *sock = FindSock(WORLD->sock_sys, value + 5);
                if (sock != NULL) {
                    sock_index = sock - WORLD->sock_sys->sock;
                }
            } else if (NuStrIStr(params[index], "FALSE") == NULL) {
                enabled = 1;
            } else {
                enabled = 0;
            }
        }
        if (enabled == 0) {
            SockOff(WORLD->sock_sys, sock_index);
        } else {
            SockOn(WORLD->sock_sys, sock_index);
        }
    }
    return 1;
}

#if defined(__i386__)
#define GAMEAISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define GAMEAISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail splits the first-time test off as the head; the rest is this partial clone.
static GAMEAISCRIPT_CLONE_ATTR2 void Action_AddPartBody(AISCRIPTPROCESS *processor, char **params, i32 num_params)
    __asm__("_ZL14Action_AddPartP7AISYS_sP17AISCRIPTPROCESS_sP10AIPACKET_sPPciif.part.58");

__used__ static i32 Action_AddPart(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params, i32 num_params,
                                   i32 first_time, f32) {
    if (first_time)
        Action_AddPartBody(processor, params, num_params);
    return 1;
}

static void Action_AddPartBody(AISCRIPTPROCESS *processor, char **params, i32 num_params) {
    nuhspecial_s special = {};
    NUVEC position;
    for (i32 i = 0; i < num_params; i++) {
        char *value = NuStrIStr(params[i], "name");
        if (value != NULL)
            NuSpecialFind(WORLD->current_gscn, &special, value + 5, 1);
        else if ((value = NuStrIStr(params[i], "x")) != NULL)
            position.x = AIParamToFloat(processor, value + 2);
        else if ((value = NuStrIStr(params[i], "y")) != NULL)
            position.y = AIParamToFloat(processor, value + 2);
        else if ((value = NuStrIStr(params[i], "z")) != NULL)
            position.z = AIParamToFloat(processor, value + 2);
    }
    if (NuSpecialExistsFn(&special)) {
        NUVEC velocity;
        NUMTX matrix;
        MakeThrowVector(&velocity, &position, &player->apiobj.collision_position, &player->apiobj.velocity,
                        ForceThrowSpeed, ForceThrowGravity);
        NuMtxSetTranslation(&matrix, &position);
        ADDPART_s part = Default_ADDPART;
        part.velocity = &velocity;
        part.field_40 = PartCollide_3D;
        part.gravity = ForceThrowGravity;
        part.special = &special;
        part.matrix = &matrix;
        part.field_14 = 0.1f;
        part.field_18 = 0.1f;
        part.time_step = FRAMETIME;
        part.flags = 0x29b;
        AddPart(&part);
        NewRumble(player->pad_gamepad->pad, 0.5f, 0);
    }
}

__used__ static i32 Action_Launch(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **, i32, i32 first_time, f32) {
    if (first_time && packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL)
        StartLaunch(packet->owner->apiobj.objptr);
    return 1;
}

static i32 Action_SelectRandomSpline(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time) {
        NUGSPLINE *splines[32];
        NUGSPLINE *unused_splines[32];
        i32 count = 0;
        i32 unused = 0;
        script_spline_selected = NULL;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "splines=");
            if (value != NULL) {
                count += NuSplineFindAllBeg(WORLD->current_gscn, value + 8, &splines[count], 32 - count);
            } else if ((value = NuStrIStr(params[index], "spline=")) != NULL) {
                NUGSPLINE *spline = NuSplineFind(WORLD->current_gscn, value + 7);
                if (spline != NULL && count < 32)
                    splines[count++] = spline;
            } else if (NuStrIStr(params[index], "unused") != NULL) {
                unused = 1;
            }
        }
        if (count != 0) {
            if (unused) {
                for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
                    GameObject_s *object = &Obj[index];
                    if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->apiobj.field_0x287 == 0 &&
                        object->movement_spline != NULL)
                        object->movement_spline->length |= 0x8000;
                }
                i32 unused_count = 0;
                for (i32 index = 0; index < count; ++index) {
                    if (splines[index]->length >= 0)
                        unused_splines[unused_count++] = splines[index];
                }
                for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
                    GameObject_s *object = &Obj[index];
                    if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->apiobj.field_0x287 == 0 &&
                        object->movement_spline != NULL)
                        object->movement_spline->length &= 0x7fff;
                }
                if (unused_count != 0)
                    script_spline_selected = unused_splines[qrand() / (0xffff / unused_count + 1)];
            } else {
                script_spline_selected = splines[qrand() / (0xffff / count + 1)];
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetScriptState(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    APIOBJECT *target = packet != NULL ? &packet->owner->apiobj : NULL;
    char *state_name = NULL;
    i32 creature_set = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            if (GetNamedAPIObjectFn != NULL) {
                target = GetNamedAPIObjectFn(sys, value + 10);
            }
            continue;
        }
        value = NuStrIStr(params[index], "set=");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value + 4));
            creature_set = static_cast<u32>(parsed_set) < 17 ? parsed_set : 0;
            continue;
        }
        value = NuStrIStr(params[index], "state=");
        if (value != NULL) {
            state_name = value + 6;
        }
    }
    if (state_name == NULL) {
        return 1;
    }

    if (creature_set == 0) {
        if (target == NULL || target->ai == NULL) {
            return 1;
        }
        AISCRIPTPROCESS *target_processor = reinterpret_cast<AISCRIPTPROCESS *>(target->ai);
        if (target_processor->base_script == NULL) {
            return 1;
        }
        AISTATE *state = AIStateFind(state_name, target_processor->base_script);
        if (state == NULL) {
            return 1;
        }
        target_processor = reinterpret_cast<AISCRIPTPROCESS *>(target->ai);
        target_processor->active_ref_count = 0;
        AIScriptProcessorInit(sys, target->ai, target_processor, NULL, NULL, NULL, 0, target_processor->base_script,
                              state);
        return 1;
    }
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
                (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) &&
            object->ai.creature_set == creature_set) {
            AISTATE *state =
                AIStateFind(state_name, reinterpret_cast<AISCRIPTPROCESS *>(object->apiobj.ai)->base_script);
            if (state != NULL) {
                AISCRIPTPROCESS *target_processor = reinterpret_cast<AISCRIPTPROCESS *>(object->apiobj.ai);
                target_processor->active_ref_count = 0;
                AIScriptProcessorInit(sys, object->apiobj.ai, target_processor, NULL, NULL, NULL, 0,
                                      target_processor->base_script, state);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetStateArea(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    NUVEC difference;
    (void)sys;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    char *state_name = NULL;
    i32 types[10];
    i32 type_count = 0;
    f32 range_squared = 0.0f;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], const_cast<char *>("range="));
        if (value != NULL) {
            const f32 range = AIParamToFloat(processor, value + 6);
            range_squared = range * range;
        } else if ((value = NuStrIStr(params[index], const_cast<char *>("type"))) != NULL) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                const i32 local_type = static_cast<u8>(LevelCharacterTypeIDFn(value + 5));
                if (local_type != 0xff) {
                    const i32 global_type = LevelCharacterGlobalIDFn(local_type);
                    if (global_type != -1 && type_count < 10) {
                        types[type_count++] = global_type;
                    }
                }
            }
        } else if ((value = NuStrIStr(params[index], const_cast<char *>("State="))) != NULL) {
            state_name = value + 6;
        }
    }

    if (state_name == NULL || (type_count == 0 && !(range_squared > 0.0f))) {
        processor->next_state = AIStateFind(state_name, processor->script);
        return 1;
    }

    NUVEC origin = v000;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        origin = packet->owner->apiobj.objptr->apiobj.collision_position;
    }

    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index) {
        GameObject_s *object = &Obj[object_index];
        if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
            (object->apiobj.field_0x1f4 & 0x400) == 0) {
            continue;
        }

        NuVecSub(&difference, &origin, &object->apiobj.position);
        const f32 distance_squared =
            difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;

        if (type_count != 0) {
            i32 matching_type = 0;
            for (i32 type_index = 0; type_index < type_count; ++type_index) {
                if (object->id == types[type_index]) {
                    matching_type = 1;
                }
            }
            if (matching_type == 0) {
                continue;
            }
        }
        if (range_squared > distance_squared) {
            AIPACKET *object_packet = object->apiobj.ai;
            object_packet->script_process.next_state = AIStateFind(state_name, object_packet->script_process.script);
        }
    }
    return 1;
}

__used__ static i32 Action_TriggerBlowUp(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0) {
        if (param_4 > 0) {
            GIZMOBLOWUP_s *blowup = NULL;
            for (i32 index = 0; index < param_4; ++index) {
                char *value = NuStrIStr(params[index], "name=");
                if (value != NULL) {
                    GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, value + 5);
                    if (gizmo != NULL) {
                        blowup = static_cast<GIZMOBLOWUP_s *>(gizmo->object);
                    }
                }
            }
            if (blowup != NULL && (blowup->output_flags & 1) == 0) {
                GizmoBlowupBlowup(blowup, 1, -1, 1, NULL, 1);
            }
        }
    }
    return 1;
}

// The original shooting action calls the out-of-line PartyMemberInWay clone.
// Retail splits the flag test off as an inline head; the rest is this partial clone.
static GAMEAISCRIPT_CLONE_ATTR2 i32 PartyMemberInWayBody(GameObject_s *object, GameObject_s *opponent)
    __asm__("_ZL16PartyMemberInWayP12GameObject_sS0_.part.65");

static inline i32 PartyMemberInWay(GameObject_s *object, GameObject_s *opponent) {
    if ((object->apiobj.field_0x1f4 & 5) != 0)
        return false;
    return PartyMemberInWayBody(object, opponent);
}

static i32 PartyMemberInWayBody(GameObject_s *object, GameObject_s *opponent) {
    NUVEC difference, direction;
    const f32 target_distance =
        NuVecDistSqr(&opponent->apiobj.collision_position, &object->apiobj.collision_position, &difference);
    const f32 length = NuFsqrt(target_distance);
    NuVecScale(&direction, &difference, length != 0.0f ? 1.0f / length : 0.0f);
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *member = Player[index];
        if (member == NULL || (member->apiobj.field_0x1f8 & 0x1001) != 0x1001 || member == object || member == opponent)
            continue;
        if (NuVecDistSqr(&object->apiobj.collision_position, &member->apiobj.collision_position, &difference) <
            target_distance) {
            member = Player[index];
            const f32 radius = 0.125f + member->apiobj.field_0x1dc;
            if (LineIntersectSphere(&object->apiobj.collision_position, &direction, &member->apiobj.collision_position,
                                    radius * radius, NULL))
                return true;
        }
    }
    return false;
}

__used__ static i32 Action_SnapToPosition(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = NULL;
    if (packet != NULL && packet->owner != NULL) {
        object = packet->owner->apiobj.objptr;
    }

    i32 check_terrain = 1;
    i32 rotation = 0;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    if (param_4 > 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "player1");
            if (value != NULL) {
                object = player;
            } else {
                value = NuStrIStr(params[index], "player");
                if (value != NULL) {
                    object = player;
                } else {
                    value = NuStrIStr(params[index], "player2");
                    if (value != NULL) {
                        object = player2;
                    } else {
                        value = NuStrIStr(params[index], "roty");
                        if (value != NULL) {
                            rotation = static_cast<i32>(static_cast<i32>(AIParamToFloat(processor, value + 5)) *
                                                        182.04444885253906f);
                        } else {
                            value = NuStrIStr(params[index], "x");
                            if (value != NULL) {
                                x = AIParamToFloat(processor, value + 2);
                            } else {
                                value = NuStrIStr(params[index], "y");
                                if (value != NULL) {
                                    y = AIParamToFloat(processor, value + 2);
                                } else {
                                    value = NuStrIStr(params[index], "z");
                                    if (value != NULL) {
                                        z = AIParamToFloat(processor, value + 2);
                                    } else if (NuStrIStr(params[index], "dont_check_terrain") != NULL) {
                                        check_terrain = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (object == NULL) {
        return 1;
    }

    object->apiobj.field_0x276 = static_cast<u16>(rotation);
    object->apiobj.position.x = x;
    object->apiobj.facing_angle = static_cast<u16>(rotation);
    object->apiobj.position.y = y;
    object->apiobj.movement_facing_angle = static_cast<u16>(rotation);
    object->apiobj.position.z = z;
    object->apiobj.initial_position = object->apiobj.position;
    object->apiobj.collision_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.respawn_position = object->apiobj.position;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.last_safe_position = object->apiobj.position;
    object->saved_position = object->apiobj.position;
    object->apiobj.velocity = v000;
    InitSurfaceInfo(object);
    if (check_terrain != 0) {
        SetObjOnSurface(object, 0);
    }
    return 1;
}

__used__ static i32 Action_SetLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    APIOBJECT *target = packet != NULL ? reinterpret_cast<APIOBJECT *>(packet->owner) : NULL;
    if (param_4 == 0) {
        return 1;
    }

    char *locator_name = NULL;
    i32 personal = 0;
    i32 indexed = 0;
    i32 nearest = 0;
    i32 random_count = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value;
        if ((value = NuStrIStr(params[index], "name=")) != NULL) {
            locator_name = value + 5;
        } else if (NuStrICmp(params[index], "personal") == 0) {
            personal = 1;
        } else if (NuStrICmp(params[index], "indexed") == 0) {
            indexed = 1;
        } else if (NuStrICmp(params[index], "nearest") == 0) {
            nearest = 1;
        } else if ((value = NuStrIStr(params[index], "random=")) != NULL) {
            random_count = static_cast<i32>(AIParamToFloat(processor, value + 7));
        } else if ((value = NuStrIStr(params[index], "character=")) != NULL && GetNamedAPIObjectFn != NULL) {
            target = GetNamedAPIObjectFn(sys, value + 10);
        }
    }

    AILOCATOR *locator = NULL;
    char resolved_name[64];
    if (locator_name != NULL) {
        if (nearest) {
            // Search the numbered locators NAME_0, NAME_1, ... for the one
            // closest to the packet's terrain origin.
            f32 best_distance = 1.0e9f;
            NUVEC difference;
            i32 index = 0;
            while (true) {
                sprintf(resolved_name, "%s_%d", locator_name, index++);
                AILOCATOR *candidate = AIPathFindLocator(sys, resolved_name);
                if (candidate == NULL) {
                    break;
                }
                const f32 distance = NuVecDistSqr(&packet->terrain_origin, &candidate->position, &difference);
                if (distance < best_distance) {
                    best_distance = distance;
                    locator = candidate;
                }
            }
        } else if (packet != NULL) {
            if (indexed && static_cast<i8>(packet->owner->apiobj.field_0x27c) != -1) {
                sprintf(resolved_name, "%s_%d", locator_name, static_cast<i8>(packet->owner->apiobj.field_0x27c));
            } else if (personal && packet->owner->apiobj.character_data != NULL) {
                sprintf(resolved_name, "%s_%s", locator_name, packet->owner->apiobj.character_data->file);
            } else if (random_count != 0) {
                sprintf(resolved_name, "%s_%d", locator_name, NuRand(0) % random_count);
            } else {
                sprintf(resolved_name, locator_name);
            }
            locator = AIPathFindLocator(sys, resolved_name);
        }
    }
    if (target != NULL) {
        target->ai->locator = locator;
    }
    return 1;
}

__used__ static i32 Action_SetPathCnxFlag(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32 param_6) {
    (void)processor;
    (void)packet;
    (void)param_6;
    if (sys == NULL || sys->path_sys == NULL || sys->path_sys->path_count == 0 || first_time == 0 || param_count < 1) {
        return 1;
    }

    char *from = NULL;
    char *to = NULL;
    u32 add_flags = 0;
    u32 remove_flags = 0;
    bool set = true;
    bool both_ways = false;
    for (i32 index = 0; index < param_count; ++index) {
        char *match = NuStrIStr(params[index], "from=");
        if (match != NULL) {
            from = match + 5;
            continue;
        }
        match = NuStrIStr(params[index], "to=");
        if (match != NULL) {
            to = match + 3;
            continue;
        }
        const u32 flag = ParseAIPathCnxFlag(params[index]);
        if (flag != 0) {
            add_flags |= flag;
            if (flag == static_cast<u32>(LEGO_AIPATHCNX_JUMP_NOW)) {
                remove_flags |= static_cast<u32>(LEGO_AIPATHCNX_DONT_JUMP_NOW);
            } else if (flag == static_cast<u32>(LEGO_AIPATHCNX_DONT_JUMP_NOW)) {
                remove_flags |= static_cast<u32>(LEGO_AIPATHCNX_JUMP_NOW);
            } else if (flag == 0x20000000u) {
                both_ways = true;
            }
            continue;
        }
        if (NuStrICmp(params[index], "bothways") == 0) {
            both_ways = true;
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            set = false;
        }
    }

    if (from != NULL && to != NULL) {
        i32 direction = 0;
        AIPATHCNX *connection =
            static_cast<AIPATHCNX *>(AIPAthFindPathCnx(sys, sys->path_sys->active_path, from, to, &direction));
        if (connection != NULL) {
            if (set) {
                connection->traversal_flags[direction] =
                    (connection->traversal_flags[direction] | add_flags) & ~remove_flags;
                if (both_ways) {
                    connection->traversal_flags[direction ^ 1] =
                        (connection->traversal_flags[direction ^ 1] | add_flags) & ~remove_flags;
                }
            } else {
                connection->traversal_flags[direction] &= ~add_flags;
                if (both_ways) {
                    connection->traversal_flags[direction ^ 1] &= ~add_flags;
                }
            }
        }
    }
    return 1;
}

static i32 Action_SetTechnoComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                    i32 first_time, f32) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (first_time) {
        TECHNO *techno = NULL;
        i32 complete = 1;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "techno");
            if (value != NULL) {
                char *name = value + NuStrLen("techno") + 1;
                i32 type = GizmoGetTypeIDByName(WORLD->gizmo_sys, "Techno");
                GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, type, name);
                if (gizmo != NULL && gizmo->object != NULL)
                    techno = static_cast<TECHNO *>(gizmo->object);
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                complete = 0;
            } else if ((value = NuStrIStr(params[index], "controlling")) != NULL) {
                object = GetNamedGameObject(WORLD->ai_sys, value + NuStrLen("controlling") + 1);
            }
        }
        if (object != NULL)
            techno = Technos_FindControllingTechno(object);
        if (techno != NULL)
            techno->complete = complete;
    }
    return 1;
}

__used__ static i32 Action_SetUseOneAtOnce(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                char *name = NuStrIStr(params[index], "character");
                if (name != NULL) {
                    if (NuStrICmp(name + 10, "myself") == 0 && packet != NULL) {
                        object = packet->owner->apiobj.objptr;
                    } else {
                        object = GetNamedGameObject(WORLD->ai_sys, name + 10);
                    }
                } else if (NuStrICmp(params[index], "FALSE") == 0) {
                    enabled = 0;
                }
            }
        }
        if (object != NULL) {
            object->field_0xf01 = static_cast<u8>((object->field_0xf01 & ~0x20u) | ((enabled & 1) << 5));
        }
    }
    return 1;
}

__used__ static i32 Action_SetLayer(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    u32 set_layers = 0;
    u32 clear_layers = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
            continue;
        }
        value = NuStrIStr(params[index], "set_layer=");
        if (value != NULL) {
            const i32 layer = static_cast<i32>(AIParamToFloat(processor, value + 10));
            if (static_cast<u32>(layer - 1) < 32) {
                set_layers |= 1u << (layer - 1);
            }
            continue;
        }
        value = NuStrIStr(params[index], "clear_layer=");
        if (value != NULL) {
            const i32 layer = static_cast<i32>(AIParamToFloat(processor, value + 12));
            if (static_cast<u32>(layer - 1) < 32) {
                clear_layers |= 1u << (layer - 1);
            }
        }
    }
    if (object != NULL) {
        u32 *layers = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(object) + 0x1058);
        *layers = (*layers | set_layers) & ~clear_layers;
    }
    return 1;
}

static i32 Action_SplineFollowTerrain(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 follow = 1;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    follow = 0;
                } else {
                    char *value = NuStrIStr(params[index], "character=");
                    if (value != NULL)
                        object = GetNamedGameObject(system, value + 10);
                }
            }
        }
        if (object != NULL)
            object->spline_follow_terrain = follow;
    }
    return 1;
}

static i32 Action_IgnoreSlideTerrain(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 ignore = 1;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    ignore = 0;
                } else {
                    char *value = NuStrIStr(params[index], "character=");
                    if (value != NULL)
                        object = GetNamedGameObject(system, value + 10);
                }
            }
        }
        if (object != NULL)
            object->ignore_slide_terrain = ignore;
    }
    return 1;
}

static i32 Action_AwkwardShapeOverride(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                       i32 param_count, i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    enabled = 0;
                } else {
                    char *value = NuStrIStr(params[index], "character=");
                    if (value != NULL)
                        object = GetNamedGameObject(system, value + 10);
                }
            }
        }
        if (object != NULL) {
            if (object->character_context == 0x3c || object->field_0xcc0 != NULL)
                Player_ClearContext(object, 1);
            object->awkward_shape_override = enabled;
        }
    }
    return 1;
}

static i32 Action_IgnoreLastSafePathPos(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                        i32 param_count, i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 ignore = 1;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    ignore = 0;
                } else {
                    char *value = NuStrIStr(params[index], "character=");
                    if (value != NULL)
                        object = GetNamedGameObject(system, value + 10);
                }
            }
        }
        if (object != NULL) {
            if (object->character_context == 0x3c || object->field_0xcc0 != NULL)
                Player_ClearContext(object, 1);
            object->field_0xf03 = (object->field_0xf03 & ~1) | (u8)ignore;
        }
    }
    return 1;
}

__used__ static i32 Action_CanBeCarried(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = NULL;
        if (packet != NULL && packet->owner != NULL) {
            object = packet->owner->apiobj.objptr;
        }

        i32 enabled = 1;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                if (NuStrICmp(params[index], "FALSE") == 0) {
                    enabled = 0;
                } else {
                    char *value = NuStrIStr(params[index], "character=");
                    if (value != NULL) {
                        object = GetNamedGameObject(sys, value + 10);
                    }
                }
            }
        }

        if (object != NULL) {
            if (object->character_context == 0x3c || object->field_0xcc0 != NULL) {
                Player_ClearContext(object, 1);
            }
            object->field_0xf00 = static_cast<u8>((object->field_0xf00 & ~2u) | (static_cast<u8>(enabled) << 1));
        }
    }
    return 1;
}

__used__ static i32 Action_AddMiscPickups(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = NULL;
        if (packet != NULL && packet->owner != NULL) {
            object = packet->owner->apiobj.objptr;
        }
        i32 value = 0;
        i32 torpedo = 0;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                char *parameter = NuStrIStr(params[index], "character=");
                if (parameter != NULL) {
                    object = GetNamedGameObject(sys, parameter + 10);
                } else if ((parameter = NuStrIStr(params[index], "value=")) != NULL) {
                    value = static_cast<i32>(AIParamToFloat(processor, parameter + 6));
                } else if ((parameter = NuStrIStr(params[index], "torpedo=")) != NULL) {
                    torpedo = static_cast<i32>(AIParamToFloat(processor, parameter + 8));
                }
            }
        }
        if (object == NULL) {
            return 1;
        }
        NUVEC *position = &object->apiobj.collision_position;
        if (value != 0 || torpedo != 0) {
            AddMiscPickups(position, -1, value, torpedo);
        }
    }
    return 1;
}

static i32 Action_SetTakeOverTarget(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32 first_time, f32) {
    if (!first_time)
        return 1;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    char *type_name = NULL;
    GameObject_s *target = NULL;
    f32 nearest = 1000000000.0f;
    i32 check_takeover = 0;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "type=");
        if (value != NULL)
            type_name = value + 5;
        else if ((value = NuStrIStr(params[index], "opponent=")) != NULL)
            target = GetNamedGameObject(system, value + 9);
        else if ((value = NuStrIStr(params[index], "maxrange=")) != NULL) {
            nearest = AIParamToFloat(processor, value + 9);
            nearest *= nearest;
        } else if (NuStrICmp(params[index], "ifCanTakeover") != 0)
            check_takeover = 1;
    }
    if (target == NULL && type_name == NULL)
        return 1;
    i32 type = -1;
    for (i32 index = 0; index < CHARCOUNT && type == -1; ++index) {
        if (NuStrICmp(CDataList[index].file, type_name) == 0)
            type = index;
    }
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *candidate = &Obj[index];
        if ((candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 || candidate->id != type)
            continue;
        if (candidate->takeover_target != NULL && candidate->takeover_target != object)
            continue;
        if (candidate->field_0xcc0 != NULL &&
            (candidate->field_0xcc0->character_context == 0x3b || candidate->character_context != 0x3b))
            continue;
        NUVEC difference;
        NuVecSub(&difference, &candidate->apiobj.collision_position, &packet->owner->apiobj.collision_position);
        f32 distance = NuVecMagSqr(&difference);
        if (!(distance < nearest))
            continue;
        nearest = distance;
        target = candidate;
    }
    if (target == NULL) {
        object->takeover_target = NULL;
        return 1;
    }
    if (check_takeover &&
        (target->field_0xcc0 != NULL || !(target->apiobj.character_data->game_character->flags_090 & 0x40) ||
         (object->apiobj.character_data->model_flags & 0x10)))
        return 1;
    GameObject_s *previous = object->takeover_target;
    if (previous != NULL && previous->takeover_target != NULL && previous->takeover_target == object)
        previous->takeover_target = NULL;
    object->takeover_target = target;
    target->takeover_target = object;
    return 1;
}

static i32 Action_RegisterTakeOverObject(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    GameObject *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(system, value + 10);
        }
    }
    if (object != NULL) {
        RegisterTakeOverObject(object);
    }
    return 1;
}

__used__ static i32 Action_ReleaseTakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            }
        }
        if (object != NULL && object->field_0xcc0 != NULL) {
            ReleaseTakeOver(object, 0);
        }
    }
    return 1;
}

__used__ static i32 Action_TakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = NULL;
    if (packet != NULL && packet->owner != NULL) {
        object = packet->owner->apiobj.objptr;
    }
    GameObject_s *target = object != NULL ? object->takeover_target : NULL;
    i32 release = 0;

    if (param_4 > 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            } else {
                value = NuStrIStr(params[index], "player=");
                if (value != NULL) {
                    i32 player_index = static_cast<i32>(AIParamToFloat(processor, value + 7)) - 1;
                    if (static_cast<u32>(player_index) < 2) {
                        object = Player[player_index];
                    }
                } else {
                    value = NuStrIStr(params[index], "target=");
                    if (value != NULL) {
                        target = GetNamedGameObject(sys, value + 7);
                    } else if (NuStrIStr(params[index], "Opponent") != NULL) {
                        if (packet != NULL && packet->opponent_object != NULL) {
                            target = packet->opponent_object->objptr;
                        }
                    } else if (NuStrIStr(params[index], "release") != NULL) {
                        release = 1;
                    }
                }
            }
        }
    }

    if (release != 0) {
        if (object != NULL) {
            ReleaseTakeOver(object, 0);
        }
        return 1;
    }
    if (target == NULL || object == NULL) {
        return 1;
    }

    if ((target->apiobj.character_data->model_flags & 0x40000000) != 0) {
        if ((target->character_context == 0x3e || target->character_context == 0x17)) {
            return 0;
        }
    }
    PLAYERCHARACTERCONFIG_s *target_config = target->apiobj.character_data->player_config;
    if (target->field_0xcc0 == NULL && (target_config->flags_090 & 0x40) != 0 &&
        (object->apiobj.character_data->model_flags & 0x10) == 0) {
        TakeOverGameObject(object, target, 1, 0);
    }
    return 1;
}

static i32 Action_LinkTurretToController(AISYS *system, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count,
                                         i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    GIZTURRET *turret = NULL;
    GameObject *controller = NULL;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "turret=");
        if (value != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, turret_gizmotype_id, value + 7);
            if (gizmo != NULL && gizmo->object != NULL) {
                turret = static_cast<GIZTURRET *>(gizmo->object);
            }
        } else if ((value = NuStrIStr(params[index], "controller=")) != NULL) {
            controller = GetNamedGameObject(system, value + 11);
        }
    }
    if (turret != NULL) {
        turret->controller = controller;
    }
    return 1;
}

__used__ static i32 Action_SetLastAttacker(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *victim = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    GameObject_s *attacker = NULL;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "victim=");
        if (value != NULL) {
            victim = GetNamedGameObject(sys, value + 7);
            continue;
        }
        if (NuStrICmp(params[index], "attacker=opponent") == 0) {
            if (packet != NULL && packet->action_target_ref != NULL) {
                attacker = *packet->action_target_ref;
            }
            continue;
        }
        value = NuStrIStr(params[index], "attacker=");
        if (value != NULL) {
            attacker = GetNamedGameObject(sys, value + 9);
        } else if (NuStrICmp(params[index], "attacker=player") == 0) {
            attacker = sys->player_1->objptr;
        }
    }
    if (victim != NULL && attacker != NULL) {
        victim->last_attacker = attacker;
    }
    return 1;
}

__used__ static i32 Action_AlertCreatures(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            }
        }
        if (object != NULL) {
            AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        }
    }
    return 1;
}

static i32 Action_FollowCharacter(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                  i32 param_count, i32 first_time, f32) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                processor->action_data_3 = GetNamedGameObject(system, value + 10);
            } else if (NuStrICmp(params[index], "ignore_radius") == 0) {
                processor->action_data_1 |= 2;
            } else if (NuStrICmp(params[index], "can_go_off_path") == 0) {
                processor->action_data_1 |= 1;
            } else if (NuStrICmp(params[index], "Opponent") == 0) {
                processor->action_data_3 = packet->opponent;
            } else if (NuStrICmp(params[index], "TakeOverTarget") == 0) {
                processor->action_data_3 = packet->owner->apiobj.objptr->takeover_target;
            } else {
                packet->movement_instruction_parameter = AIParamToFloat(processor, params[index]);
            }
        }
    }
    APIOBJECT *target = static_cast<APIOBJECT *>(processor->action_data_3);
    if (target != NULL) {
        FollowAPIObject(&packet->owner->apiobj, target, processor->action_data_1,
                        packet->movement_instruction_parameter);
    }
    return 0;
}

__used__ static i32 Action_FaceCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 elapsed) {
    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                processor->action_data_3 = GetNamedGameObject(sys, value + 10);
            } else {
                processor->action_timer = AIParamToFloat(processor, params[index]);
            }
        }
    }
    GameObject_s *target = static_cast<GameObject_s *>(processor->action_data_3);
    if (packet != NULL) {
        if (target != NULL) {
            packet->movement_look_target = &target->apiobj.position;
        }
    }
    if (processor->action_timer > 0.0f) {
        processor->action_timer -= elapsed;
        if (processor->action_timer <= 0.0f) {
            processor->action_timer = 0.0f;
            return 1;
        }
    }
    return 0;
}

__used__ static i32 Action_SetForceBack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    NUVEC *position = NULL;
    f32 radius = 1.5f;
    i32 type = 0;
    i32 enabled = 1;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
        } else if ((value = NuStrIStr(params[index], "locator=")) != NULL) {
            AILOCATOR *locator = AIPathFindLocator(sys, value + 8);
            position = locator != NULL ? &locator->position : NULL;
        } else if ((value = NuStrIStr(params[index], "radius=")) != NULL) {
            radius = AIParamToFloat(processor, value + 7);
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            enabled = 0;
        } else if (NuStrICmp(params[index], "type=CHOKE") == 0) {
            type = 1;
        } else if (NuStrICmp(params[index], "type=DROID") == 0) {
            type = 2;
        } else if (NuStrICmp(params[index], "type=ComboOpponent") == 0) {
            type = 3;
        }
    }

    if (enabled != 0) {
        if (object != NULL || position != NULL) {
            SetForceBack(object, position, radius, type);
        }
    } else {
        ResetForceBack();
    }
    return 1;
}

__used__ static i32 Action_PlaySfx(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)packet;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }
    if (param_4 <= 0) {
        return 1;
    }

    NUVEC position = {1.0e9f, 1.0e9f, 1.0e9f};
    NUVEC *position_ptr = NULL;
    char *name = NULL;
    char play_count = 1;
    f32 repeat_delay = 0.0f;
    f32 start_delay = 0.0f;

    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], const_cast<char *>("name="));
        if (value != NULL) {
            name = value + 5;
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("playcount="));
        if (value != NULL) {
            play_count = static_cast<char>(AIParamToFloat(processor, value + 10));
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("repdelay="));
        if (value != NULL) {
            repeat_delay = AIParamToFloat(processor, value + 9);
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("startdelay="));
        if (value != NULL) {
            start_delay = AIParamToFloat(processor, value + 11);
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("x="));
        if (value != NULL) {
            position.x = AIParamToFloat(processor, value + 2);
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("y="));
        if (value != NULL) {
            position.y = AIParamToFloat(processor, value + 2);
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("z="));
        if (value != NULL) {
            position.z = AIParamToFloat(processor, value + 2);
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("character_pos="));
        if (value != NULL) {
            GameObject_s *object = GetNamedGameObject(sys, value + 14);
            position_ptr = &object->apiobj.collision_position;
        }
    }

    if (position_ptr == NULL && (position.x != 1.0e9f || position.y != 1.0e9f || position.z != 1.0e9f)) {
        position_ptr = &position;
    }
    if (name != NULL) {
        PlayRepeatSfx(name, -1, start_delay, play_count, repeat_delay, position_ptr);
    }
    return 1;
}

__used__ static i32 Action_AddDebris(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;

    i32 handle = -1;
    NUVEC position = {1000000000.0f, 1000000000.0f, 1000000000.0f};
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;

    if (param_5 != 0) {
        i32 debris_type = 33;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            } else if ((value = NuStrIStr(params[index], "x=")) != NULL) {
                position.x = AIParamToFloat(processor, value + 2);
            } else if ((value = NuStrIStr(params[index], "y=")) != NULL) {
                position.y = AIParamToFloat(processor, value + 2);
            } else if ((value = NuStrIStr(params[index], "z=")) != NULL) {
                position.z = AIParamToFloat(processor, value + 2);
            } else if ((value = NuStrIStr(params[index], "type")) != NULL) {
                debris_type = FindGameDebris(static_cast<APIDEBRISSYS_s *>(perm_debrissys), value + 5);
            }
        }

        NUVEC *debris_position;
        if (position.x == 1000000000.0f && position.y == 1000000000.0f && position.z == 1000000000.0f) {
            if (object == NULL)
                return 1;
            debris_position = &object->apiobj.collision_position;
        } else {
            debris_position = &position;
        }
        AddFiniteShotDebrisEffect(&handle, WORLD->debris_sys->entries[debris_type].effect, debris_position, 1);
    }
    return 1;
}

__used__ static i32 Action_NoIdleSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        if (param_5 != 0) {
            i32 enabled = 1;
            for (i32 index = 0; index < param_4; ++index) {
                char *name = NuStrIStr(params[index], "character=");
                if (name != NULL) {
                    object = GetNamedGameObject(sys, name + 10);
                } else if (NuStrICmp(params[index], "FALSE") == 0) {
                    enabled = 0;
                }
            }
            if (object != NULL) {
                object->field_0xf03 = static_cast<u8>((object->field_0xf03 & ~2u) | ((enabled & 1) << 1));
            }
        }
    }
    return 1;
}

__used__ static i32 Action_AddPartDebris(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                         i32 num_params, i32 first_time, f32) {
    NUVEC offset = {0.0f, 0.0f, 0.0f};
    if (first_time) {
        // The original requires coordinates or a character parameter to supply this position.
        NUVEC position;
        i32 type = -1;
        for (i32 i = 0; i < num_params; i++) {
            char *value = NuStrIStr(params[i], "name");
            if (value != NULL)
                type = PARTLookupType(value + 5);
            else if ((value = NuStrIStr(params[i], "dx=")) != NULL)
                offset.x = AIParamToFloat(processor, value + 3);
            else if ((value = NuStrIStr(params[i], "dy=")) != NULL)
                offset.y = AIParamToFloat(processor, value + 3);
            else if ((value = NuStrIStr(params[i], "dz=")) != NULL)
                offset.z = AIParamToFloat(processor, value + 3);
            else if ((value = NuStrIStr(params[i], "x=")) != NULL)
                position.x = AIParamToFloat(processor, value + 2);
            else if ((value = NuStrIStr(params[i], "y=")) != NULL)
                position.y = AIParamToFloat(processor, value + 2);
            else if ((value = NuStrIStr(params[i], "z=")) != NULL)
                position.z = AIParamToFloat(processor, value + 2);
            else if ((value = NuStrIStr(params[i], "character=")) != NULL) {
                GameObject *object = GetNamedGameObject(sys, value + 10);
                if (object != NULL)
                    position = object->apiobj.collision_position;
            }
        }
        if (type != -1) {
            NuVecAdd(&position, &position, &offset);
            AddFiniteShotPART(type, &position, 1);
        }
    }
    return 1;
}

static i32 Action_SetShieldHitPoints(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_count, i32 first_time, f32) {
    if (first_time != 0) {
        GameObject *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 hit_points = -1;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL)
                object = GetNamedGameObject(system, value + 10);
            else
                hit_points = static_cast<i32>(AIParamToFloat(processor, params[index]));
        }
        if (object != NULL) {
            if (hit_points == -1)
                hit_points = object->apiobj.character_data->game_character->field_0xf5;
            object->field_0xe37 = static_cast<u8>(hit_points);
        }
    }
    return 1;
}

__used__ static i32 Action_SetHitPoints(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 num_params, i32 first_time, f32) {
    if (!first_time)
        return 1;
    GameObject *object = NULL;
    if (packet != NULL && packet->owner != NULL)
        object = packet->owner->apiobj.objptr;
    i32 hitpoints = -1;
    i32 set_max = 1;
    for (i32 i = 0; i < num_params; i++) {
        char *value = NuStrIStr(params[i], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
        } else if ((value = NuStrIStr(params[i], "messageval=")) != NULL) {
            GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, value + 11, NULL);
            if (message != NULL)
                hitpoints = (i32)message->value;
        } else if (NuStrICmp(params[i], "dont_set_max") == 0) {
            set_max = 0;
        } else if (NuStrICmp(params[i], "default") != 0) {
            hitpoints = (i32)AIParamToFloat(processor, params[i]);
        }
    }
    if (object != NULL) {
        if (hitpoints == -1)
            hitpoints = ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->hitpoints;
        if (set_max)
            object->hitpoints = (u8)hitpoints;
        object->current_hp = (i8)hitpoints;
    }
    return 1;
}

__used__ static i32 Action_SetCurrentSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 num_params, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (!first_time)
        return 1;
    i32 speed_mode = -1;
    f32 speed = 0.0f;
    for (i32 i = 0; i < num_params; i++) {
        char *value = NuStrIStr(params[i], "character=");
        if (value != NULL)
            object = GetNamedGameObject(sys, value + 10);
        else if (NuStrICmp("speed=TIPTOE", params[i]) == 0)
            speed_mode = 2;
        else if (NuStrICmp("speed=WALK", params[i]) == 0)
            speed_mode = 1;
        else if (NuStrICmp("speed=RUN", params[i]) == 0)
            speed_mode = 0;
        else
            speed = AIParamToFloat(processor, params[i]);
    }
    if (object != NULL) {
        if (speed_mode == 2)
            speed = ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->tiptoe_speed;
        else if (speed_mode == 1)
            speed = ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->walk_speed;
        else if (speed_mode == 0) {
            speed = ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->run_speed;
            object->field_0xdc8 = 1.0f;
        }
        object->apiobj.velocity.x = 0.0f;
        object->apiobj.velocity.y = 0.0f;
        object->apiobj.velocity.z = speed;
        NuVecRotateY(&object->apiobj.velocity, &object->apiobj.velocity, object->apiobj.facing_angle);
    }
    return 1;
}

static __used__ i32 Action_CreateSplineCreatures(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet,
                                                 char **params, i32 num_params, i32 first_time, f32) {
    char script_name[64] = "default";
    NUVEC spline_offset = {0.0f, 0.0f, 0.0f};
    if (!first_time || num_params <= 0)
        return 1;
    i16 models[10];
    NUGSPLINE *splines[32];
    u16 angle, pitch;
    i32 model_count = 0, spline_count = 0, use_selected = 0;
    i32 min_group_size = -1, max_group_size = -1, group_size = 1;
    f32 min_distance = 1000000000.0f, max_distance = 1000000000.0f, distance = 0.0f;
    i32 relative_to_player = 0, creature_set = 0, looping = 0;
    AILOCATOR *relative_locator = NULL;
    GameObject_s *rider = NULL;
    for (i32 i = 0; i < num_params; i++) {
        char *value = NuStrIStr(params[i], "mingroupsize");
        if (value != NULL)
            min_group_size = (i32)AIParamToFloat(processor, value + 13);
        else if ((value = NuStrIStr(params[i], "maxgroupsize")) != NULL)
            max_group_size = (i32)AIParamToFloat(processor, value + 13);
        else if ((value = NuStrIStr(params[i], "groupsize")) != NULL)
            group_size = (i32)AIParamToFloat(processor, value + 10);
        else if ((value = NuStrIStr(params[i], "type")) != NULL) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                u8 type = LevelCharacterTypeIDFn(value + 5);
                if (type != 0xff) {
                    i16 model = LevelCharacterGlobalIDFn(type);
                    if (model != -1 && model_count < 10)
                        models[model_count++] = model;
                }
            }
        } else if ((value = NuStrIStr(params[i], "script")) != NULL)
            NuStrCpy(script_name, value + 7);
        else if ((value = NuStrIStr(params[i], "mindistance")) != NULL)
            min_distance = AIParamToFloat(processor, value + 12);
        else if ((value = NuStrIStr(params[i], "maxdistance")) != NULL)
            max_distance = AIParamToFloat(processor, value + 12);
        else if ((value = NuStrIStr(params[i], "distance")) != NULL)
            distance = AIParamToFloat(processor, value + 9);
        else if (script_spline_selected != NULL && NuStrICmp("use_selected_spline", params[i]) == 0)
            use_selected = 1;
        else if ((value = NuStrIStr(params[i], "splines=")) != NULL)
            spline_count +=
                NuSplineFindAllBeg(WORLD->current_gscn, value + 8, &splines[spline_count], 32 - spline_count);
        else if (NuStrIStr(params[i], "spline=myspline") != NULL) {
            if (processor->unknown_ac != NULL)
                splines[spline_count++] = processor->unknown_ac;
        } else if ((value = NuStrIStr(params[i], "spline=")) != NULL) {
            NUGSPLINE *spline = NuSplineFind(WORLD->current_gscn, value + 7);
            if (spline != NULL && spline_count < 32)
                splines[spline_count++] = spline;
        } else if ((value = NuStrIStr(params[i], "x_offset=")) != NULL)
            spline_offset.x = AIParamToFloat(processor, value + 9);
        else if ((value = NuStrIStr(params[i], "y_offset=")) != NULL)
            spline_offset.y = AIParamToFloat(processor, value + 9);
        else if ((value = NuStrIStr(params[i], "z_offset=")) != NULL)
            spline_offset.z = AIParamToFloat(processor, value + 9);
        else if (NuStrICmp(params[i], "looping") == 0)
            looping = 1;
        else if (NuStrIStr(params[i], "addtoset=myset") != NULL)
            creature_set = processor->unknown_b0;
        else if ((value = NuStrIStr(params[i], "addtoset=")) != NULL)
            creature_set = (i32)AIParamToFloat(processor, value + 9);
        else if (NuStrICmp(params[i], "relative_to_player") == 0)
            relative_to_player = 1;
        else if ((value = NuStrIStr(params[i], "relative_to_locator=")) != NULL)
            relative_locator = AIPathFindLocator(system, value + 20);
        else if (NuStrIStr(params[i], "ridden_by=myself") != NULL) {
            if (packet != NULL && packet->owner != NULL)
                rider = packet->owner->apiobj.objptr;
        } else if ((value = NuStrIStr(params[i], "ridden_by=")) != NULL)
            rider = GetNamedGameObject(system, value + 10);
    }
    if (min_group_size < max_group_size && min_group_size >= 0)
        group_size = NuRand(NULL) % (max_group_size + 1 - min_group_size) + min_group_size;
    if (min_distance != 1000000000.0f && max_distance != 1000000000.0f) {
        f32 fraction = NuRandFloat();
        distance = max_distance * fraction + min_distance * (1.0f - fraction);
    }
    if ((spline_count | use_selected) == 0 || group_size <= 0 || model_count == 0)
        return 1;
    NUGSPLINE *spline = use_selected ? script_spline_selected : splines[qrand() / (65535 / spline_count + 1)];
    i32 model = models[NuRand(NULL) % model_count];
    for (i32 i = 0; i < group_size; i++) {
        GameObject_s *object = AddDynamicCreature(model, NULL, 0, script_name, NULL, NULL, 0, spline, &spline_offset,
                                                  looping, creature_set);
        if (object == NULL)
            continue;
        if (relative_to_player || relative_locator != NULL) {
            i16 start = -1, end = -1;
            if (WORLD->current_level == PODSPRINTA_LDATA)
                PodSprint_GetIAlongVals(spline, &start, &end);
            SPLINEPOS_s position;
            if (relative_to_player) {
                GetNearestSplinePos(&player->apiobj.collision_position, &position, spline, looping, start, end);
                object->movement_spline_position = position;
            } else if (relative_locator != NULL) {
                GetNearestSplinePos(&relative_locator->position, &position, spline, looping, start, end);
                object->movement_spline_position = position;
            }
        }
        if (distance != 0.0f) {
            MoveSplinePosition(&object->movement_spline_position, distance);
            NUVEC position, offset;
            PointAlongSpline(object->movement_spline, object->movement_spline_position.along, &position, &angle, &pitch,
                             object->movement_spline_position.looping);
            object->apiobj.movement_facing_angle = angle;
            object->apiobj.facing_angle = angle;
            object->apiobj.field_0x276 = angle;
            object->apiobj.pitch_angle = -pitch;
            if (object->movement_spline_offset.x != 0.0f || object->movement_spline_offset.y != 0.0f ||
                object->movement_spline_offset.z != 0.0f) {
                NuVecRotateX(&offset, &object->movement_spline_offset, object->apiobj.pitch_angle);
                NuVecRotateY(&offset, &offset, object->apiobj.field_0x276);
                NuVecAdd(&position, &position, &offset);
            }
            object->apiobj.position = position;
            object->apiobj.initial_position = position;
            object->apiobj.collision_position = position;
            plr_lastpos = position;
            object->apiobj.start_position = position;
            GameObjectOrigin(object);
            object->apiobj.last_safe_position = object->apiobj.position;
            object->ai_update_position = object->apiobj.position;
        }
        if (rider != NULL)
            TakeOverGameObject(rider, object, 0, 1);
    }
    return 1;
}

static i32 Action_DontAvoidCharacter(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        if (param_count > 0) {
            GameObject_s *dont_avoid = NULL;
            i32 enabled = 1;
            for (i32 index = 0; index < param_count; ++index) {
                char *value = NuStrIStr(params[index], "character=");
                if (value != NULL) {
                    object = GetNamedGameObject(system, value + 10);
                    continue;
                }
                value = NuStrIStr(params[index], "dont_avoid=");
                if (value != NULL) {
                    dont_avoid = GetNamedGameObject(system, value + 11);
                    continue;
                }
                if (NuStrICmp(params[index], "FALSE") == 0)
                    enabled = 0;
            }
            if (object != NULL && dont_avoid != NULL)
                object->ai.dont_avoid_character = enabled ? dont_avoid : NULL;
        }
    }
    return 1;
}

__used__ static i32 Action_DontPush(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    i32 types[10];
    i32 type_count = 0;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 dont_push = 1;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], const_cast<char *>("type"));
        if (value != NULL) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                const i32 local_type = static_cast<u8>(LevelCharacterTypeIDFn(value + 5));
                if (local_type != 0xff) {
                    const i32 global_type = LevelCharacterGlobalIDFn(local_type);
                    if (global_type != -1 && type_count < 10) {
                        types[type_count++] = global_type;
                    }
                }
            }
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("character="));
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            dont_push = 0;
        }
    }

    if (type_count != 0) {
        const i32 object_count = HIGHGAMEOBJECT;
        GameObject_s *candidate = Obj;
        for (i32 object_index = 0; object_index < object_count; ++object_index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                    (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
                (candidate->apiobj.field_0x1f4 & 0x400) == 0) {
                continue;
            }
            for (i32 type_index = 0; type_index < type_count; ++type_index) {
                if (candidate->id == types[type_index]) {
                    candidate->apiobj.flags_low_bit1 = dont_push;
                }
            }
        }
    } else if (object != NULL) {
        object->apiobj.flags_low_bit1 = dont_push;
    }
    return 1;
}

__used__ static i32 Action_SetInvulnerable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    i32 types[10];
    i32 type_count = 0;
    i32 still_take_hit = 0;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 enabled = 1;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], const_cast<char *>("type"));
        if (value != NULL) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                const i32 local_type = static_cast<u8>(LevelCharacterTypeIDFn(value + 5));
                if (local_type != 0xff) {
                    const i32 global_type = LevelCharacterGlobalIDFn(local_type);
                    if (global_type != -1 && type_count < 10) {
                        types[type_count++] = global_type;
                    }
                }
            }
            continue;
        }
        value = NuStrIStr(params[index], const_cast<char *>("character="));
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            enabled = 0;
        } else if (NuStrICmp(params[index], "still_do_take_hit_anim") == 0) {
            still_take_hit = 1;
        }
    }

    if (type_count != 0) {
        const i32 object_count = HIGHGAMEOBJECT;
        GameObject_s *candidate = Obj;
        still_take_hit = (still_take_hit & 1) << 4;
        for (i32 object_index = 0; object_index < object_count; ++object_index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                    (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
                (candidate->apiobj.field_0x1f4 & 0x400) == 0) {
                continue;
            }
            if (enabled) {
                for (i32 type_index = 0; type_index < type_count; ++type_index) {
                    if (candidate->id == types[type_index]) {
                        candidate->field_0xefe |= 0x40;
                        candidate->field_0xefd = static_cast<u8>((candidate->field_0xefd & ~0x10u) | still_take_hit);
                    }
                }
            } else {
                for (i32 type_index = 0; type_index < type_count; ++type_index) {
                    if (candidate->id == types[type_index]) {
                        candidate->field_0xefe &= static_cast<u8>(~0x40u);
                    }
                }
            }
        }
    } else if (object != NULL && (object->apiobj.field_0x1f4 & 0x400) != 0) {
        if (enabled) {
            still_take_hit = (still_take_hit & 1) << 4;
            object->field_0xefe |= 0x40;
            object->field_0xefd = static_cast<u8>((object->field_0xefd & ~0x10u) | still_take_hit);
        } else {
            object->field_0xefe &= static_cast<u8>(~0x40u);
        }
    }
    return 1;
}

static i32 Action_DontSetStoppedFlag(AISYS *system, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "character");
            if (value != NULL)
                object = GetNamedGameObject(system, value + 10);
            else if (NuStrICmp("FALSE", params[0]) == 0)
                enabled = 0;
        }
        if (object != NULL)
            object->field_0xefc = (object->field_0xefc & ~0x20u) | ((enabled & 1) << 5);
    }
    return 1;
}

__used__ static i32 Action_SetDontMove(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        for (i32 index = 0; index < param_4; ++index) {
            char *name = NuStrIStr(params[index], "character");
            if (name != NULL) {
                object = GetNamedGameObject(sys, name + 10);
            } else if (NuStrICmp("FALSE", params[0]) == 0) {
                // This action's FALSE toggle uses the first parameter, not the current one.
                enabled = 0;
            }
        }
        if (object != NULL) {
            object->field_0xefc = static_cast<u8>((object->field_0xefc & ~0x10u) | ((enabled & 1) << 4));
        }
    }
    return 1;
}

__used__ static i32 Action_Kill(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    GameObject_s *excluded = NULL;
    AIAREA *area = NULL;
    i32 creature_set = 0;
    i32 all_ai = 0;
    i32 check_if_dead = 0;
    i32 debris = 0;
    i32 parts_on = 0;
    i32 respawn = 0;
    i32 respawn_at_origin = 0;

    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
            continue;
        }
        if (NuStrIStr(params[index], "opponent") != NULL) {
            if (packet->opponent != NULL) {
                object = static_cast<APIOBJECT *>(packet->opponent)->objptr;
            }
            continue;
        }
        if (NuStrICmp(params[index], "respawn") == 0) {
            respawn = 1;
            continue;
        }
        if (NuStrICmp(params[index], "respawn_at_origin") == 0) {
            respawn = 1;
            respawn_at_origin = 1;
            continue;
        }
        value = NuStrIStr(params[index], "all_ai_except");
        if (value != NULL) {
            excluded = GetNamedGameObject(sys, value + 14);
            all_ai = 1;
            continue;
        }
        if (NuStrICmp(params[index], "all_ai") == 0) {
            all_ai = 1;
            continue;
        }
        if (NuStrICmp(params[index], "check_if_dead") == 0) {
            check_if_dead = 1;
            continue;
        }
        if (NuStrICmp(params[index], "debris") == 0) {
            debris = 1;
            continue;
        }
        if (NuStrICmp(params[index], "parts_on") == 0) {
            parts_on = 1;
            continue;
        }
        value = NuStrIStr(params[index], "set=");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value + 4));
            creature_set = parsed_set >= 0 && parsed_set <= 16 ? parsed_set : 0;
            continue;
        }
        value = NuStrIStr(params[index], "area=");
        if (value != NULL) {
            area = AISysFindArea(WORLD->ai_sys, value + 5);
        }
    }

    const auto has_group_flags = [](GameObject_s *candidate) {
        return (candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
               (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER);
    };
    // Group scans require the object array; the reference checks group
    // membership before applying the optional dead-state filter.
    if (all_ai) {
        GameObject_s *candidate = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++candidate) {
            if (has_group_flags(candidate) && (candidate->apiobj.field_0x1f4 & 0x400u) != 0 && candidate != excluded &&
                (!check_if_dead || (candidate->apiobj.field_0x287 == 0 && candidate->field_0x101c <= 0.0f))) {
                candidate->respawnable = respawn;
                candidate->respawn_at_origin = respawn_at_origin;
                if (parts_on)
                    KillParts(candidate, -1, -1, 1, 0.0f, 0, NULL);
                if (debris)
                    KillGameObject(candidate, 2, 0);
                else
                    KillGameObject(candidate, 4, 0);
            }
        }
        return 1;
    }
    if (creature_set != 0) {
        GameObject_s *candidate = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++candidate) {
            if (has_group_flags(candidate)) {
                if (candidate->ai.creature_set == creature_set &&
                    (!check_if_dead || (candidate->apiobj.field_0x287 == 0 && candidate->field_0x101c <= 0.0f))) {
                    candidate->respawnable = respawn;
                    candidate->respawn_at_origin = respawn_at_origin;
                    if (parts_on)
                        KillParts(candidate, -1, -1, 1, 0.0f, 0, NULL);
                    if (debris)
                        KillGameObject(candidate, 2, 0);
                    else
                        KillGameObject(candidate, 4, 0);
                }
            }
        }
        return 1;
    }
    if (area != NULL) {
        GameObject_s *candidate = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++candidate) {
            if (candidate == NULL || !has_group_flags(candidate)) {
                continue;
            }
            const isize area_index = area - area->system->areas;
            const u64 mask = static_cast<u64>(candidate->apiobj.ai_area_mask_low) |
                             (static_cast<u64>(candidate->apiobj.ai_area_mask_high) << 32);
            if (((mask >> area_index) & 1) == 0) {
                continue;
            }
            if (!check_if_dead || (candidate->apiobj.field_0x287 == 0 && candidate->field_0x101c <= 0.0f)) {
                candidate->respawnable = respawn;
                candidate->respawn_at_origin = respawn_at_origin;
                if (parts_on)
                    KillParts(candidate, -1, -1, 1, 0.0f, 0, NULL);
                if (debris)
                    KillGameObject(candidate, 2, 0);
                else
                    KillGameObject(candidate, 4, 0);
            }
        }
        return 1;
    }

    // An explicitly selected object is not subject to the group scan's
    // in-use/character flags; only check_if_dead gates this path.
    if (object != NULL && (!check_if_dead || (object->apiobj.field_0x287 == 0 && object->field_0x101c <= 0.0f))) {
        object->respawnable = respawn;
        object->respawn_at_origin = respawn_at_origin;
        if (parts_on)
            KillParts(object, -1, -1, 1, 0.0f, 0, NULL);
        if (!debris)
            KillGameObject(object, 4, 0);
        else
            KillGameObject(object, 2, 0);
    }
    return 1;
}

__used__ static i32 Action_ForcePush(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    if (param_5 != 0) {
        processor->action_data_3 = NULL;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "target=opponent") == 0) {
                processor->action_data_3 = packet->opponent;
                if (processor->action_data_3 == NULL) {
                    return 1;
                }
            } else {
                char *value = NuStrIStr(params[index], "target=");
                if (value != NULL) {
                    processor->action_data_3 = GetNamedGameObject(sys, value + 7);
                    if (processor->action_data_3 == NULL) {
                        return 1;
                    }
                } else if (NuStrICmp(params[index], "forcepush_highlight_only") == 0) {
                    processor->action_data_1 = 1;
                }
            }
        }

        if (object->character_context == 0x1b && processor->action_data_1 == 0 &&
            object->takeover_entry_target != NULL && processor->action_data_3 == NULL) {
            object->pad_gamepad->allocated_5a |= 0x10;
            object->force_push_target = object->takeover_entry_target;
            object->character_context = -1;
            u8 highlight = processor->action_data_1 & 1;
            object->field_0xe22 &= ~2;
            object->field_0xe21 = (object->field_0xe21 & ~4) | (highlight << 2);
            FindForcePushTarget(object, 0, 0);
        }
    }

    if (CharCategory_IsCategory(object, 0) == 0) {
        if (FreePlay != 0) {
            processor->action_timer -= param_6;
            if (processor->action_timer < 0.0f) {
                processor->action_timer = 0.5f;
                object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLERIGHT;
            }
        }
        return 0;
    }

    object->pad_gamepad->allocated_5a |= 0x10;
    object->force_push_target = static_cast<GameObject_s *>(processor->action_data_3);
    object->field_0xe21 = (object->field_0xe21 & ~4) | ((processor->action_data_1 & 1) << 2);
    if (processor->action_object != NULL) {
        return processor->action_object != object->takeover_entry_target;
    }
    if (object->character_context == 0x1b && object->takeover_entry_target != NULL) {
        processor->action_object = object->takeover_entry_target;
    }
    return 0;
}

__used__ static i32 Action_GrabVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL && param_5 != 0) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        GameObject_s *victim = object->ai.opponent_object != NULL ? object->ai.opponent_object->objptr : NULL;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "victim=");
            if (value != NULL) {
                victim = GetNamedGameObject(sys, value + 7);
            }
        }
        if (victim != NULL) {
            GrabVictim(object, victim);
        }
    }
    return 1;
}

__used__ static i32 Action_ShootAtOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (__builtin_expect(param_5 != 0, 1)) {
        processor->action_data_5 = engagefiretime;
        f32 fraction = NuRandFloat();
        bool explicit_range = false;
        for (i32 index = 0; index < param_4; ++index) {
            char *value;
            if ((value = NuStrIStr(params[index], "firerange")) != NULL) {
                processor->action_data_4 = AIParamToFloat(processor, value + 10);
                explicit_range = true;
            } else if (__builtin_expect(NuStrIStr(params[index], "offscreen") != NULL, 1)) {
                processor->action_data_1 |= 1;
            } else if (__builtin_expect(NuStrIStr(params[index], "no_fire_in_minicut") != NULL, 0)) {
                processor->action_data_1 |= 4;
            } else if ((value = NuStrIStr(params[index], "fireinterval")) != NULL) {
                processor->action_data_5 = AIParamToFloat(processor, value + 13);
            } else if ((value = NuStrIStr(params[index], "opponent=")) != NULL) {
                processor->action_data_3 = GetNamedGameObject(sys, value + 9);
            } else if (NuStrIStr(params[index], "instant") != NULL) {
                fraction = 0.0f;
            } else if (NuStrICmp(params[index], "frontArcOnly") == 0) {
                processor->action_data_2 = 1;
            }
        }
        processor->action_timer = fraction * processor->action_data_5;
        if (__builtin_expect(!explicit_range, 1))
            processor->action_data_4 = packet->movement_instruction_parameter == 0.0f
                                           ? 9999.9f
                                           : packet->movement_instruction_parameter + aitol;
    }
    GameObject_s *opponent = static_cast<GameObject_s *>(processor->action_data_3);
    if (__builtin_expect(opponent == NULL || (opponent->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
                             (opponent->apiobj.field_0x287 != 0 && !(opponent->field_0x101c > 0.0f)),
                         0)) {
        if (packet->opponent_object == NULL || packet->opponent_object->ai == NULL ||
            packet->opponent_object->objptr == NULL)
            return 0;
        opponent = packet->opponent_object->objptr;
    }
    NUVEC difference;
    const f32 distance = NuVecDistSqr(&packet->owner->apiobj.position, &opponent->apiobj.position, &difference);
    object->field_0xef8 |= 0x20;
    i32 outside_arc = processor->action_data_2;
    if (outside_arc != 0) {
        NUVEC relative;
        NuVecSub(&relative, &opponent->apiobj.position, &packet->owner->apiobj.position);
        NuVecRotateY(&relative, &relative, -static_cast<i32>(packet->owner->apiobj.field_0x276));
        if (relative.z > 0.0f)
            outside_arc = 0;
    }
    if (((WORLD->api_object_sys->line_of_sight[object->apiobj.field_0x289] >> opponent->apiobj.field_0x289) & 1) == 0)
        return 0;
    if (object->apiobj.model_draw_result == 0 && (processor->action_data_1 & 1) == 0)
        return 0;
    if (!(distance < processor->action_data_4 * processor->action_data_4))
        return 0;
    if (opponent->apiobj.field_0x287 != 0 && !(opponent->field_0x101c > 0.0f))
        return 0;
    if (outside_arc != 0)
        return 0;
    packet->movement_look_target = &opponent->apiobj.position;
    if (MiniCutCam != 0 && (processor->action_data_1 & 4) != 0) {
        const f32 interval = processor->action_data_5;
        processor->action_timer = NuRandFloat() * interval + 2.0f;
    } else {
        processor->action_timer -= param_6;
    }
    if (opponent->apiobj.field_0x287 == 0 && processor->action_timer <= 0.0f) {
        const f32 interval = processor->action_data_5;
        const f32 random = NuRandFloat();
        processor->action_timer = 0.5f * interval + random * interval;
        if (!PartyMemberInWay(object, opponent)) {
            object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
            object->script_fire_target = opponent;
        }
    }
    return 0;
}

__used__ static i32 Action_EngageOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    ai_fighting = 1;
    if (param_5 != 0) {
        processor->action_data_5 = engagefiretime;
        packet->movement_instruction_parameter = idealgoalrange;
        f32 initial_fire_fraction = NuRandFloat();
        i32 explicit_fire_range = 0;

        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "firerange");
            if (value != NULL) {
                processor->action_data_4 = AIParamToFloat(processor, value + 10);
                explicit_fire_range = 1;
            } else if ((value = NuStrIStr(params[index], "goalrange")) != NULL) {
                packet->movement_instruction_parameter = AIParamToFloat(processor, value + 10);
            } else if ((value = NuStrIStr(params[index], "minrange=")) != NULL) {
                packet->movement_instruction_parameter = AIParamToFloat(processor, value + 9);
                processor->action_data_1 |= 2;
            } else if (NuStrIStr(params[index], "static") != NULL) {
                packet->movement_instruction_parameter = 0.0f;
            } else if (NuStrIStr(params[index], "offscreen") != NULL) {
                processor->action_data_1 |= 1;
            } else if (NuStrIStr(params[index], "no_fire_in_minicut") != NULL) {
                processor->action_data_1 |= 4;
            } else if (NuStrIStr(params[index], "circle") != NULL) {
                packet->field_0x1e5 ^= 2;
                processor->action_data_1 |= 8;
            } else if ((value = NuStrIStr(params[index], "fireinterval")) != NULL) {
                processor->action_data_5 = AIParamToFloat(processor, value + 13);
            } else if ((value = NuStrIStr(params[index], "opponent=")) != NULL) {
                processor->action_data_3 = GetNamedGameObject(sys, value + 9);
            } else if (NuStrIStr(params[index], "instant") != NULL) {
                initial_fire_fraction = 0.0f;
            } else if ((value = NuStrIStr(params[index], "attack_override")) != NULL) {
                char *name = value + 16;
                if (NuStrICmp(name, "PUNCH_1") == 0)
                    processor->action_data_6 = 1;
                else if (NuStrICmp(name, "PUNCH_2") == 0)
                    processor->action_data_6 = 2;
                else if (NuStrICmp(name, "PUNCH_3") == 0)
                    processor->action_data_6 = 3;
                else if (NuStrICmp(name, "PUNCH_BEHIND") == 0)
                    processor->action_data_6 = 4;
                else if (NuStrICmp(name, "PUNCH_SPECIAL") == 0)
                    processor->action_data_6 = 5;
                else if (NuStrICmp(name, "BLOCK") == 0)
                    processor->action_data_6 = 6;
                else
                    processor->action_data_6 = NuStrICmp(name, "SHOOT") == 0 ? 7 : 0;
            }
        }
        if (LSW1 != 0) {
            processor->action_data_1 |= 0x20;
        }
        processor->action_timer = initial_fire_fraction * processor->action_data_5;
        if (!explicit_fire_range) {
            if (packet->movement_instruction_parameter == 0.0f) {
                processor->action_data_4 = 9999.9f;
            } else {
                processor->action_data_4 = packet->movement_instruction_parameter + aitol;
            }
        }
    }

    GameObject_s *opponent = static_cast<GameObject_s *>(processor->action_data_3);
    if (opponent != NULL) {
        if ((opponent->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            (opponent->apiobj.field_0x287 != 0 && !(opponent->field_0x101c > 0.0f)))
            opponent = NULL;
    }
    if (processor->action_data_6 != 0) {
        object->attack_override = static_cast<u8>(processor->action_data_6);
    }
    if (opponent == NULL) {
        APIOBJECT *api_opponent = static_cast<APIOBJECT *>(packet->opponent);
        if (api_opponent == NULL || api_opponent->ai == NULL || api_opponent->objptr == NULL)
            return 0;
        opponent = api_opponent->objptr;
    }
    object->field_0xef8 |= 0x20;
    if ((opponent->apiobj.field_0x1f8 & 0x1001) != 0x1001 || opponent->apiobj.field_0x287 != 0 ||
        opponent->character_context == 0x2b ||
        (opponent->apiobj.player_controlled && opponent->spawn_protection_timer > 1.5f))
        return 0;

    NUVEC difference;
    const f32 distance_squared = NuVecDistSqr(&opponent->apiobj.position, &packet->owner->apiobj.position, &difference);
    f32 goal_range;
    if ((object->field_0xf01 & 0x20) != 0) {
        if (!(oneAtOnce_CanAttack(object, static_cast<APIOBJECT *>(object->ai.opponent)->objptr))) {
            goal_range = oneAtOnce_GetHoldRange(object);
            processor->action_data_1 |= 2;
        } else {
            goal_range = 0.1f;
            processor->action_data_1 &= static_cast<u8>(~2u);
        }
    } else {
        goal_range = packet->movement_instruction_parameter;
        if (goal_range != 0.0f && ((opponent->apiobj.player_controlled && object->apiobj.model_draw_result == 0) ||
                                   ((WORLD->api_object_sys->line_of_sight[packet->owner->apiobj.field_0x289] >>
                                     opponent->apiobj.field_0x289) &
                                    1) == 0))
            goal_range = 0.1f;
    }

    if (goal_range == 0.0f) {
        if ((object->apiobj.character_data->game_character->flags_090 & 0x100) == 0)
            packet->movement_look_target = &opponent->apiobj.position;
    } else {
        const f32 stopping_distance = MIN(goal_range, opponent->ai.mover_height);
        if ((object->field_0xefe & 1) == 0)
            processor->action_data_1 &= static_cast<u8>(~0x10u);
        const u32 character_flags = object->apiobj.character_data->game_character->flags_090 & 0x100;
        const f32 near_range = goal_range - aitol;
        const f32 far_range = goal_range + aitol;
        if ((object->field_0xefe & 1) != 0 && character_flags != 0) {
            if ((processor->action_data_1 & 0x10) != 0) {
                if (distance_squared > far_range * far_range)
                    processor->action_data_1 &= static_cast<u8>(~0x10u);
            } else if (distance_squared < near_range * near_range) {
                processor->action_data_1 |= 0x10;
            }
            if ((processor->action_data_1 & 0x10) != 0) {
                AIMoveInstruction(packet, &opponent->ai.last_path_position, stopping_distance, &opponent->ai.path_info,
                                  AIPACKET_MOVEMENT_RETREAT, goal_range);
                packet->goal_speed_mode = 1;
                object->field_0xefd |= 0x80;
            } else {
                AIMoveInstruction(packet, &opponent->ai.last_path_position, 0.0f, &opponent->ai.path_info,
                                  AIPACKET_MOVEMENT_TO_DESTINATION, 0.01f);
                packet->goal_speed_mode = 0;
            }
        } else if (character_flags != 0 && distance_squared < near_range * near_range) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, stopping_distance, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_RETREAT, goal_range);
            packet->goal_speed_mode = 1;
            if ((object->apiobj.character_data->game_character->flags_090 & 0x100) != 0)
                object->field_0xefd |= 0x80;
        } else if ((processor->action_data_1 & 0x20) != 0 && distance_squared < near_range * near_range) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, stopping_distance, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_RETREAT, goal_range);
            packet->goal_speed_mode = 1;
        } else if (distance_squared > far_range * far_range && (processor->action_data_1 & 2) == 0) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, stopping_distance, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_TO_DESTINATION, goal_range);
            packet->goal_speed_mode = 0;
        } else if ((processor->action_data_1 & 8) != 0) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, stopping_distance, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_CIRCLE, goal_range);
            packet->goal_speed_mode = 0;
        } else if (character_flags == 0) {
            packet->movement_look_target = &opponent->apiobj.position;
        }
    }

    if (((WORLD->api_object_sys->line_of_sight[object->apiobj.field_0x289] >> opponent->apiobj.field_0x289) & 1) == 0)
        return 0;
    const u32 character_flags = object->apiobj.character_data->game_character->flags_090 & 0x100;
    if (character_flags != 0 && (object->field_0xefe & 1) != 0)
        return 0;
    if (object->apiobj.model_draw_result == 0 && (processor->action_data_1 & 1) == 0)
        return 0;
    if (!(distance_squared < processor->action_data_4 * processor->action_data_4))
        return 0;
    if (character_flags == 0)
        packet->movement_look_target = &opponent->apiobj.position;
    if (MiniCutCam != 0 && (processor->action_data_1 & 4) != 0) {
        const f32 interval = processor->action_data_5;
        processor->action_timer = NuRandFloat() * interval + 2.0f;
    } else {
        processor->action_timer -= param_6;
    }
    if (opponent->apiobj.field_0x287 == 0) {
        object->script_fire_target = opponent;
        if (processor->action_timer <= 0.0f) {
            const f32 interval = processor->action_data_5;
            const f32 random = NuRandFloat();
            processor->action_timer = 0.5f * interval + random * interval;
            if (!PartyMemberInWay(object, opponent)) {
                if (oneAtOnce_CanAttack(object, opponent))
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
            }
        }
    }
    return 0;
}

__used__ static i32 Action_ProbeDroid(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)params;
    (void)param_4;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (object->cable != NULL) {
        object->cable->source = NULL;
        object->cable = NULL;
    }
    AIMoveInstruction(packet, NULL, 0.0f, NULL, AIPACKET_MOVEMENT_WANDER, packet->movement_instruction_parameter);
    char *engage_params[] = {"goalrange 10", "firerange 15", "fireinterval 1.5"};
    Action_EngageOpponent(sys, processor, packet, engage_params, 3, param_5, param_6);
    return 0;
}

__used__ static i32 Action_SetOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0 || packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    GameObject_s *opponent = NULL;
    GameObject_s *droids[10];
    i32 droid_count = 0;
    static i32 prev_droid_ix;
    for (i32 index = 0; index < param_4; ++index) {
        char *value;
        if (NuStrIStr(params[index], "opponent=droid") != NULL) {
            // The retail script action checks eight player slots explicitly.
#define OPPONENT_DROID(slot)                                                                                           \
    {                                                                                                                  \
        GameObject_s *candidate = Player[slot];                                                                        \
        if (candidate != NULL && (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&                                 \
            (candidate->apiobj.field_0x287 == 0 || candidate->field_0x101c > 0.0f) &&                                  \
            (candidate->field_0xeff & 1) == 0 && (candidate->apiobj.character_data->model_flags & 0x10) != 0 &&        \
            droid_count < 10)                                                                                          \
            droids[droid_count++] = candidate;                                                                         \
    }
            OPPONENT_DROID(0)
            OPPONENT_DROID(1)
            OPPONENT_DROID(2)
            OPPONENT_DROID(3)
            OPPONENT_DROID(4)
            OPPONENT_DROID(5)
            OPPONENT_DROID(6)
            OPPONENT_DROID(7)
#undef OPPONENT_DROID
            // Retail accumulates candidates across parameters. Bound the
            // original ten-entry array when malformed scripts repeat droid.
            if (droid_count != 0) {
                prev_droid_ix = (prev_droid_ix + 1) % droid_count;
                opponent = droids[prev_droid_ix];
            }
        } else if (NuStrIStr(params[index], "opponent=nearest_enemy") != NULL) {
            object->opponent = NULL;
        } else if ((value = NuStrIStr(params[index], "opponent=")) != NULL) {
            opponent = GetNamedGameObject(sys, value + 9);
        } else if (NuStrICmp(params[index], "last_attacker") == 0) {
            opponent = static_cast<GameObject_s *>(object->last_attacker);
        } else if ((value = NuStrIStr(params[index], "opponentType")) != NULL) {
            i32 type = 0xff;
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                const u8 local_type = static_cast<u8>(LevelCharacterTypeIDFn(value + 13));
                if (local_type != 0xff)
                    type = LevelCharacterGlobalIDFn(local_type);
            }
            for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index) {
                if (Obj[object_index].id == type) {
                    opponent = &Obj[object_index];
                    break;
                }
            }
        }
    }
    object->opponent = opponent;
    return 1;
}

__used__ static i32 Action_SetSide(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object =
        packet != NULL && packet->owner != NULL && (packet->owner->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0
            ? packet->owner->apiobj.objptr
            : NULL;
    i32 side = 0;
    f32 range_squared = 0.0f;
    i32 type_ids[10];
    i32 type_count = 0;

    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "neutral") == 0) {
            side = 0;
        } else if (NuStrICmp(params[index], "goodie") == 0 || NuStrICmp(params[index], "goody") == 0) {
            side = -1;
        } else if (NuStrICmp(params[index], "baddie") == 0 || NuStrICmp(params[index], "baddy") == 0) {
            side = 1;
        } else if (NuStrICmp(params[index], "goodiebaddie") == 0 || NuStrICmp(params[index], "goodybaddy") == 0) {
            side = 2;
        } else if (NuStrICmp(params[index], "default") == 0) {
            side = 3;
        } else if (char *value = NuStrIStr(params[index], "type")) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                const u8 local_type = static_cast<u8>(LevelCharacterTypeIDFn(value + 5));
                if (local_type != 0xff) {
                    const i32 global_type = LevelCharacterGlobalIDFn(local_type);
                    if (global_type != -1) {
                        if (type_count < 10) {
                            type_ids[type_count++] = global_type;
                        }
                    }
                }
            }
        } else if ((value = NuStrIStr(params[index], "character=")) != NULL) {
            object = GetNamedGameObject(sys, value + 10);
        } else if ((value = NuStrIStr(params[index], "range=")) != NULL) {
            const f32 range = AIParamToFloat(processor, value + 6);
            range_squared = range * range;
        }
    }

    if (type_count == 0 && !(range_squared > 0.0f)) {
        ActionApplySide(sys, object, side);
        return 1;
    }

    f32 distance_squared = 0.0f;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *candidate = &Obj[index];
        if ((candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
            (candidate->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0) {
            continue;
        }

        i32 type_matches = 0;
        for (i32 type_index = 0; type_index < type_count; ++type_index) {
            if (candidate->id == type_ids[type_index])
                type_matches = 1;
        }
        if (range_squared > 0.0f && object != NULL) {
            NUVEC offset;
            NuVecSub(&offset, &object->apiobj.position, &candidate->apiobj.position);
            distance_squared = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
        }
        if ((type_matches == 1 && (range_squared == 0.0f || distance_squared < range_squared)) ||
            (type_count == 0 && distance_squared < range_squared)) {
            ActionApplySide(sys, candidate, side);
        }
    }
    return 1;
}

__used__ static i32 Action_TagCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (param_4 != 0) {
        GameObject_s *target = NULL;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            } else {
                value = NuStrIStr(params[index], "tag_to=");
                if (value != NULL) {
                    target = GetNamedGameObject(sys, value + 7);
                }
            }
        }
        if (object != NULL) {
            TagCharacter(object, target, 0);
            SetPlayer();
        }
    }
    return 1;
}

__used__ static i32 Action_TakeDamage(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    GameObject_s *hitter = NULL;
    i32 damage = 1;
    i32 creature_set = 0;
    if (param_4 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            } else if ((value = NuStrIStr(params[index], "hitter=")) != NULL) {
                hitter = GetNamedGameObject(sys, value + 7);
            } else if ((value = NuStrIStr(params[index], "damage=")) != NULL) {
                damage = static_cast<i32>(AIParamToFloat(processor, value + 7));
            } else if (NuStrIStr(params[index], "opponent") != NULL) {
                if (packet != NULL && packet->opponent_object != NULL && packet->opponent_object->objptr != NULL) {
                    object = packet->opponent_object->objptr;
                    if (packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
                        hitter = packet->owner->apiobj.objptr;
                    }
                }
            } else if ((value = NuStrIStr(params[index], "set=")) != NULL) {
                i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value + 4));
                if (static_cast<u32>(parsed_set) < 17) {
                    creature_set = parsed_set;
                }
            }
        }
    }

    if (creature_set != 0) {
        GameObject_s *candidate = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0 &&
                candidate->ai.creature_set == creature_set) {
                ObjHitObj(hitter, candidate, damage, 0, 0, 1);
            }
        }
    } else if (object != NULL) {
        ObjHitObj(hitter, object, damage, 0, 0, 1);
    }
    return 1;
}

__used__ static i32 Action_NotWithParty(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        for (i32 index = 0; index < param_4; ++index) {
            char *name = NuStrIStr(params[index], "character=");
            if (name != NULL) {
                object = GetNamedGameObject(sys, name + 10);
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                enabled = 0;
            }
        }
        if (object != NULL) {
            object->field_0xeff = static_cast<u8>((object->field_0xeff & ~1u) | (enabled & 1));
        }
    }
    return 1;
}

__used__ static i32 Action_CanAttack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        for (i32 index = 0; index < param_4; ++index) {
            char *name = NuStrIStr(params[index], "character=");
            if (name != NULL) {
                object = GetNamedGameObject(sys, name + 10);
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                enabled = 0;
            }
        }
        if (object != NULL) {
            object->field_0xef9 = static_cast<u8>((object->field_0xef9 & ~2u) | ((enabled & 1) << 1));
        }
    }
    return 1;
}

__used__ static i32 Action_CannotDropIn(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 enabled = 1;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value == NULL) {
                if ((value = NuStrIStr(params[index], "tag_to=")) != NULL) {
                    GetNamedGameObject(sys, value + 7);
                } else if (NuStrICmp(params[index], "FALSE") == 0) {
                    enabled = 0;
                }
            } else {
                object = GetNamedGameObject(sys, value + 10);
            }
        }
        if (object != NULL) {
            object->field_0xefb = static_cast<u8>((object->field_0xefb & ~4u) | ((enabled & 1) << 2));
        }
    }
    return 1;
}

__used__ static i32 Action_CatchUpForbidden(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        i32 forbidden = 1;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                char *parameter = NuStrIStr(params[index], "character=");
                if (parameter != NULL) {
                    object = GetNamedGameObject(sys, parameter + 10);
                } else if (NuStrICmp(params[index], "FALSE") == 0) {
                    forbidden = 0;
                }
            }
        }
        if (object != NULL) {
            object->field_0xefb = static_cast<u8>((object->field_0xefb & ~2u) | ((forbidden & 1) << 1));
        }
    }
    return 1;
}

__used__ static i32 Action_SetTaggable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    GameObject_s *tag_to = NULL;
    i32 disabled = 0;
    if (param_4 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            } else if ((value = NuStrIStr(params[index], "tag_to=")) != NULL) {
                tag_to = GetNamedGameObject(sys, value + 7);
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                disabled = 1;
            }
        }
    }
    if (object != NULL) {
        bool set_disabled = false;
        if (disabled != 0) {
            if (object->apiobj.player_controlled) {
                if (object->field_0xcc0 != NULL) {
                    ReleaseTakeOver(object, 0);
                    set_disabled = disabled;
                } else if (TagCharacter(object, tag_to, 0) != 0) {
                    set_disabled = disabled;
                }
                SetPlayer();
            } else {
                set_disabled = disabled;
            }
        }
        u8 *tag_flags = reinterpret_cast<u8 *>(object) + 0x7b5;
        *tag_flags = static_cast<u8>((*tag_flags & ~2u) | (static_cast<u8>(set_disabled) << 1));
    }
    return 1;
}

__used__ static i32 Action_ResetContext(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        if (param_4 != 0) {
            for (i32 index = 0; index < param_4; ++index) {
                char *value = NuStrIStr(params[index], "character=");
                if (value != NULL) {
                    object = GetNamedGameObject(sys, value + 10);
                }
            }
        }
        if (object != NULL) {
            Player_ClearContext(object, 1);
            Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
        }
    }
    return 1;
}

__used__ static i32 Action_SnapWeaponOut(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 enabled = 1;
    i32 keep_out = -1;
    if (param_4 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            } else if ((value = NuStrIStr(params[index], "keep_out=")) != NULL) {
                keep_out = NuStrICmp(value + 9, "TRUE") == 0;
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                enabled = 0;
            }
        }
    }

    if (object != NULL) {
        if (enabled != 0) {
            object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
            object->weapon_scale = 1.0f;
            if (keep_out != -1) {
                object->field_0xef8 = static_cast<u8>((object->field_0xef8 & ~GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT) |
                                                      (keep_out ? GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT : 0));
            }
        } else {
            object->weapon_scale = 0.0f;
            object->field_0xe22 &= static_cast<u8>(~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION);
            object->field_0xef8 &= static_cast<u8>(~GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT);
        }
    }
    return 1;
}

__used__ static i32 Action_KeepWeaponOut(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 keep_out = 1;
    if (param_4 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                keep_out = 0;
                continue;
            }
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            }
        }
    }
    if (object != NULL) {
        object->field_0xef8 = (object->field_0xef8 & static_cast<u8>(~GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT)) |
                              static_cast<u8>(keep_out ? GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT : 0);
    }
    return 1;
}

static i32 Action_SetDoomedEscapeLocator(AISYS *system, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32) {
    char locator_name[64];
    if (first_time) {
        GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        if (param_count != 0) {
            char *name = NULL;
            i32 personal = 0, indexed = 0, random_count = 0, take_damage = 0;
            for (i32 index = 0; index < param_count; ++index) {
                char *value = NuStrIStr(params[index], "character=");
                if (value != NULL)
                    object = GetNamedGameObject(system, value + 10);
                else if ((value = NuStrIStr(params[index], "name")) != NULL)
                    name = value + 5;
                else if (NuStrIStr(params[index], "personal") != NULL)
                    personal = 1;
                else if (NuStrIStr(params[index], "indexed") != NULL)
                    indexed = 1;
                else if (NuStrIStr(params[index], "take_damage") != NULL)
                    take_damage = 1;
                else if ((value = NuStrIStr(params[index], "random")) != NULL)
                    random_count = static_cast<i32>(AIParamToFloat(processor, value + 9));
            }
            if (object != NULL) {
                object->field_0xefd &= ~8u;
                object->doomed_escape_locator = NULL;
                if (name != NULL) {

                    if (indexed && static_cast<i8>(object->apiobj.field_0x27c) != -1)
                        sprintf(locator_name, "%s_%d", name, static_cast<i8>(object->apiobj.field_0x27c));
                    else if (personal && object->apiobj.character_data != NULL)
                        sprintf(locator_name, "%s_%s", name, object->apiobj.character_data->file);
                    else if (random_count != 0)
                        sprintf(locator_name, "%s_%d", name, NuRand(NULL) % random_count);
                    else
                        sprintf(locator_name, name);
                    object->doomed_escape_locator = AIPathFindLocator(system, locator_name);
                    if (object->doomed_escape_locator != NULL)
                        object->field_0xefd = (object->field_0xefd & ~8u) | (take_damage << 3);
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_BigJump(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    f32 jump_factor;
    (void)param_6;
    NUVEC destination = {1.0e9f, 1.0e9f, 1.0e9f};
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    if (param_5 != 0) {
        GameObject_s *target = NULL;
        i32 check_terrain = 0;
        i32 to_origin = 0;
        jump_factor = 1.0f;
        i32 slam = 0;

        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], const_cast<char *>("tgt_character="));
            if (value != NULL) {
                target = GetNamedGameObject(sys, value + 14);
            } else if (NuStrICmp(params[index], "slam") == 0) {
                slam = 1;
            } else if (NuStrICmp(params[index], "to_origin") == 0) {
                to_origin = 1;
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("jump_factor="))) != NULL) {
                jump_factor = AIParamToFloat(processor, value + 12);
                if (jump_factor < 0.0f) {
                    jump_factor = 0.0f;
                }
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("x="))) != NULL) {
                destination.x = AIParamToFloat(processor, value + 2);
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("y="))) != NULL) {
                destination.y = AIParamToFloat(processor, value + 2);
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("z="))) != NULL) {
                destination.z = AIParamToFloat(processor, value + 2);
            } else if (NuStrICmp(params[index], "check_terrain") == 0) {
                check_terrain = 1;
            }
        }

        if (to_origin != 0 && (object->apiobj.field_0x1f4 & 0x400) != 0 && object->ai.field_0x134 != 0xff) {
            AICREATURE *creature = &sys->creatures[object->ai.field_0x134];
            StartBigJump(object, &creature->pos, slam, jump_factor, 1.0f, 0, 0);
            object->ai.path_info = creature->path_info;
            object->ai.last_path_position = creature->pos;
        } else if (target != NULL) {
            StartBigJump(object, &target->apiobj.last_safe_position, slam, jump_factor, 1.0f, 0, 0);
            object->ai.path_info = target->ai.path_info;
            object->ai.last_path_position = target->ai.last_path_position;
        } else {
            NUVEC *jump_destination = &destination;
            if (destination.x == 1.0e9f && destination.y == 1.0e9f && destination.z == 1.0e9f) {
                jump_destination = &object->apiobj.position;
            }
            StartBigJump(object, jump_destination, slam, jump_factor, 1.0f, check_terrain, 0);
        }
    }
    return object->character_context != 0x1f;
}

__used__ static i32 Action_BigJumpToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;

    GameObject_s *object = processor->action_object;
    if (param_5 != 0) {
        if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
            object = packet->owner->apiobj.objptr;
        }
        processor->action_data_3 = processor->unknown_a4;

        i32 personal = 0;
        i32 indexed = 0;
        i32 random_count = 0;
        char *name = NULL;
        f32 jump_factor = 1.0f;
        AIAREA *required_area = NULL;

        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], const_cast<char *>("name"));
            if (value != NULL) {
                name = value + 5;
            } else if (NuStrIStr(params[index], const_cast<char *>("personal")) != NULL) {
                personal = 1;
            } else if (NuStrIStr(params[index], const_cast<char *>("indexed")) != NULL) {
                indexed = 1;
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("random"))) != NULL) {
                random_count = static_cast<i32>(AIParamToFloat(processor, value + 9));
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("jump_factor="))) != NULL) {
                jump_factor = AIParamToFloat(processor, value + 12);
                if (jump_factor < 0.0f) {
                    jump_factor = 0.0f;
                }
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("IfInArea="))) != NULL) {
                required_area = AISysFindArea(sys, value + 9);
            } else if ((value = NuStrIStr(params[index], const_cast<char *>("character="))) != NULL) {
                object = GetNamedGameObject(sys, value + 10);
            }
        }

        if (object == NULL) {
            return 1;
        }
        processor->action_object = object;

        if (required_area != NULL) {
            const i32 area_index = static_cast<i32>(required_area - WORLD->ai_sys->areas);
            if (sys->player_1 == NULL || (object->apiobj.ai_area_mask & (1ull << area_index)) == 0) {
                return 1;
            }
        }

        if (name != NULL) {
            char locator_name[64];
            if (indexed != 0 && static_cast<i8>(object->apiobj.field_0x27c) != -1) {
                sprintf(locator_name, "%s_%d", name, static_cast<i8>(object->apiobj.field_0x27c));
            } else if (personal != 0 && object->apiobj.character_data != NULL) {
                sprintf(locator_name, "%s_%s", name, object->apiobj.character_data->file);
            } else if (random_count != 0) {
                sprintf(locator_name, "%s_%d", name, NuRand(NULL) % random_count);
            } else {
                sprintf(locator_name, name);
            }
            processor->action_data_3 = AIPathFindLocator(sys, locator_name);
        }

        AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
        if (locator != NULL) {
            StartBigJump(object, &locator->position, 0, jump_factor, 1.0f, 0, 0);
            object->ai.path_info = locator->path_info;
            object->ai.last_path_position = locator->position;
        }
    }
    return object->character_context != 0x1f;
}

__used__ static i32 Action_SnapToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 check_terrain = 1;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
        } else if (NuStrIStr(params[index], "dont_check_terrain") != NULL) {
            check_terrain = 0;
        }
    }

    if (object == NULL || (object->apiobj.field_0x1f4 & 0x400) == 0 || object->ai.field_0x134 == 0xff) {
        return 1;
    }

    AICREATURE *creature = &sys->creatures[object->ai.field_0x134];
    object->apiobj.position = creature->pos;
    u16 rotation = static_cast<u16>(creature->y_rot);
    object->apiobj.field_0x276 = rotation;
    object->apiobj.facing_angle = rotation;
    object->apiobj.movement_facing_angle = rotation;
    object->ai.path_info.on_path = 0;
    object->apiobj.collision_position = object->apiobj.position;
    object->apiobj.initial_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.respawn_position = object->apiobj.position;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.last_safe_position = object->apiobj.position;
    object->saved_position = object->apiobj.position;
    object->apiobj.velocity = v000;
    InitSurfaceInfo(object);
    if (check_terrain != 0) {
        SetObjOnSurface(object, 0);
    }
    return 1;
}

__used__ static i32 Action_SnapToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32) {
    if (first_time != 0) {
        GameObject *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
        char *name = NULL;
        i32 personal = 0;
        i32 indexed = 0;
        i32 random_count = 0;
        i32 check_terrain = 1;
        for (i32 i = 0; i < param_count; ++i) {
            char *value;
            if ((value = NuStrIStr(params[i], "name")) != NULL)
                name = value + 5;
            else if (NuStrIStr(params[i], "personal") != NULL)
                personal = 1;
            else if (NuStrIStr(params[i], "indexed") != NULL)
                indexed = 1;
            else if ((value = NuStrIStr(params[i], "random")) != NULL)
                random_count = (i32)AIParamToFloat(processor, value + 9);
            else if ((value = NuStrIStr(params[i], "character")) != NULL)
                object = GetNamedGameObject(sys, value + 10);
            else if ((value = NuStrIStr(params[i], "player=")) != NULL) {
                i32 player = (i32)AIParamToFloat(processor, value + 7) - 1;
                if ((u32)player <= 1)
                    object = Player[player];
            } else if (NuStrIStr(params[i], "dont_check_terrain") != NULL)
                check_terrain = 0;
        }
        if (object == NULL) {
            if (packet != NULL && packet->owner != NULL)
                object = packet->owner->apiobj.objptr;
        }
        if (object != NULL) {
            AILOCATOR *locator = object->ai.locator;
            if (name != NULL) {
                char locator_name[64];
                if (indexed && object->apiobj.field_0x27c != -1)
                    sprintf(locator_name, "%s_%d", name, object->apiobj.field_0x27c);
                else if (personal && object->apiobj.character_data != NULL)
                    sprintf(locator_name, "%s_%s", name, object->apiobj.character_data->file);
                else if (random_count != 0)
                    sprintf(locator_name, "%s_%d", name, NuRand(NULL) % random_count);
                else
                    sprintf(locator_name, name);
                locator = AIPathFindLocator(sys, locator_name);
            }
            if (locator != NULL) {
                object->apiobj.position = locator->position;
                i32 rotation = locator->flags;
                object->apiobj.field_0x276 = rotation;
                object->apiobj.facing_angle = rotation;
                object->apiobj.movement_facing_angle = rotation;
                object->ai.path_info = locator->path_info;
                object->apiobj.initial_position = object->apiobj.position;
                object->apiobj.collision_position = object->apiobj.position;
                plr_lastpos = object->apiobj.position;
                object->apiobj.start_position = object->apiobj.position;
                object->apiobj.respawn_position = object->apiobj.position;
                object->apiobj.last_safe_position = object->apiobj.position;
                object->saved_position = object->apiobj.position;
                object->apiobj.velocity = v000;
                extern void InitSurfaceInfo(GameObject *);
                extern i32 SetObjOnSurface(GameObject *, i32);
                InitSurfaceInfo(object);
                if (check_terrain != 0)
                    SetObjOnSurface(object, 0);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0 || sys == NULL || sys->path_sys == NULL) {
        return 1;
    }

    AIPATHSYS *path_system = sys->path_sys;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    AIPATH *path = NULL;

    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
            continue;
        }
        if (NuStrIStr(params[index], "path=LevelPath") != NULL) {
            path = sys->path_sys->active_path;
            continue;
        }
        value = NuStrIStr(params[index], "path");
        if (value != NULL) {
            for (i32 path_index = 0; path_index < path_system->path_count; ++path_index) {
                if (NuStrICmp(path_system->paths[path_index]->name, value + 5) == 0) {
                    path = path_system->paths[path_index];
                    break;
                }
            }
        }
    }
    if (object != NULL) {
        if (path != NULL) {
            AISysCharacterSetPath(&object->ai, path);
            AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
        }
    }

    return 1;
}

__used__ static i32 Action_DeActivate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 creature_set = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
            continue;
        }
        value = NuStrIStr(params[index], "set=");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value + 4));
            creature_set = static_cast<u32>(parsed_set) < 17 ? parsed_set : 0;
        }
    }

    if (creature_set != 0) {
        GameObject_s *candidate = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
                    (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) &&
                candidate->ai.creature_set == creature_set) {
                DeactivateGameObject(candidate);
            }
        }
    } else if (object != NULL) {
        DeactivateGameObject(object);
    }
    return 1;
}

__used__ static i32 Action_Activate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 creature_set = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + 10);
            continue;
        }
        value = NuStrIStr(params[index], "set=");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value + 4));
            creature_set = static_cast<u32>(parsed_set) < 17 ? parsed_set : 0;
        }
    }

    if (creature_set != 0) {
        GameObject_s *candidate = Obj;
        i32 object_count = HIGHGAMEOBJECT;
        for (i32 index = 0; index < object_count; ++index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0 &&
                candidate->ai.creature_set == creature_set) {
                if (candidate->ai.field_0x134 == 0xff) {
                    candidate->apiobj.flags_high |= 0x10;
                    AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&candidate->ai),
                                                     const_cast<char *>("Base"));
                } else {
                    ResetAICreature(candidate, sys);
                }
                ++aicreature_sets_alive[creature_set - 1];
                object_count = HIGHGAMEOBJECT;
            }
        }
        return 1;
    }

    if (object != NULL) {
        if (object->ai.field_0x134 == 0xff) {
            object->apiobj.flags_high |= 0x10;
            AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&object->ai),
                                             const_cast<char *>("Base"));
        } else {
            ResetAICreature(object, sys);
        }
        if (static_cast<u32>(object->ai.creature_set - 1) < 16) {
            ++aicreature_sets_alive[object->ai.creature_set - 1];
        }
    }
    return 1;
}

static void *Condition_CharacterExistsInit(AISYS *system, char *name, AISCRIPT *) {
    if (name != NULL && system != NULL) {
        return GetNamedGameObject(system, name);
    }
    return NULL;
}

static void *Condition_XYZPosInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL && sys != NULL ? GetNamedGameObject(sys, arg) : NULL;
}

static void *Condition_BeenHitInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? GetNamedGameObject(sys, arg) : NULL;
}

static void *Condition_OffScreenTimerInit(AISYS *system, char *name, AISCRIPT *) {
    return name != NULL ? GetNamedGameObject(system, name) : NULL;
}

__used__ static i32 Action_CreateCreatures(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_count, i32 first_time, f32) {
    AILOCATOR *locators[64] = {};
    ZIPUP *zipups[32];
    i16 models[10];
    char script[64] = "default";
    NUVEC offset = {0.0f, 0.0f, 0.0f};
    if (first_time == 0)
        return 1;
    NUVEC position;
    AIPATHINFO *path;
    i32 yaw;
    if (packet != NULL && packet->owner != NULL) {
        path = &packet->path_info;
        position = packet->owner->apiobj.position;
        yaw = packet->owner->apiobj.field_0x276;
    } else {
        yaw = 0;
        path = NULL;
    }
    i32 minimum = -1, maximum = -1, count = 1;
    i32 grouped = 1, across = 2;
    f32 xspacing = 0.8f, zspacing = 0.8f;
    i32 locator_count = 0, zipup_count = 0, model_count = 0;
    AILOCATORSET *locator_set = NULL;
    i32 start_locator = 0, inherit = 1;
    char not_low_end = 0;
    f32 max_range = 1000000000.0f, min_dy = 1000000000.0f, max_dy = 1000000000.0f;
    i32 offscreen = 0, nearest = 0, onscreen = 0;
    i32 zipdown = 0;
    f32 fall_chance = 0.0f;
    i32 surface = 1, creature_set = 0;
    f32 crate_height = 0.0f, crate_delay = 0.0f;
    GameObject_s *rider = NULL;
    i32 no_gun = 0, cheap = 0, ballooning = 0;
    i64 add_capabilities = 0, remove_capabilities = 0;
    char *state = NULL;
    for (i32 index = 0; index < param_count; ++index) {
        char *value;
        if ((value = NuStrIStr(params[index], "mingroupsize")) != NULL)
            minimum = static_cast<i32>(AIParamToFloat(processor, value + 13));
        else if ((value = NuStrIStr(params[index], "maxgroupsize")) != NULL)
            maximum = static_cast<i32>(AIParamToFloat(processor, value + 13));
        else if ((value = NuStrIStr(params[index], "groupsize")) != NULL)
            count = static_cast<i32>(AIParamToFloat(processor, value + 10));
        else if ((value = NuStrIStr(params[index], "start_locator_set=")) != NULL) {
            value += 18;
            if (NuStrICmp("myset", value) == 0)
                locator_set = processor->locator_set;
            else
                locator_set = AIPathFindLocatorSet(WORLD->ai_sys, value);
            if (locator_set != NULL)
                start_locator = 1;
        } else if (NuStrICmp("dont_inherit_locator_set", params[index]) == 0)
            inherit = 0;
        else if (NuStrICmp("notOnLowEnd", params[index]) == 0)
            not_low_end = 1;
        else if ((value = NuStrIStr(params[index], "locator_set=")) != NULL) {
            value += 12;
            if (NuStrICmp("myset", value) == 0)
                locator_set = processor->locator_set;
            else
                locator_set = AIPathFindLocatorSet(WORLD->ai_sys, value);
            if (locator_set != NULL) {
                AILocatorSet_CheckLocatorsStillAssigned(sys, locator_set);
                for (i32 i = 0; i < locator_set->locator_count && locator_count < 64; ++i) {
                    if (locator_set->assigned[i] == 0xff)
                        locators[locator_count++] = &sys->locators[locator_set->locator_entries[i]];
                }
            }
        } else if ((value = NuStrIStr(params[index], "locator=")) != NULL) {
            if (locator_count < 64) {
                value += 8;
                AILOCATOR *locator;
                if (NuStrICmp("mylocator", value) == 0)
                    locator = processor->locator;
                else
                    locator = AIPathFindLocator(sys, value);
                if (locator != NULL)
                    locators[locator_count++] = locator;
            }
        } else if ((value = NuStrIStr(params[index], "max_range_to_player=")) != NULL)
            max_range = AIParamToFloat(processor, value + 20);
        else if ((value = NuStrIStr(params[index], "mindy=")) != NULL)
            min_dy = AIParamToFloat(processor, value + 6);
        else if ((value = NuStrIStr(params[index], "maxdy=")) != NULL)
            max_dy = AIParamToFloat(processor, value + 6);
        else if ((value = NuStrIStr(params[index], "zipup")) != NULL) {
            if (locator_count < 32) {
                value += 6;
                ZIPUP *zipup = WORLD->zipups;
                for (i32 i = 0; i < WORLD->zipup_count; ++i, ++zipup) {
                    if ((zipup->flags & 0xc0) == 0xc0 && NuStrICmp(value, zipup->name) == 0) {
                        zipups[zipup_count++] = zipup;
                        break;
                    }
                }
            }
        } else if (NuStrIStr(params[index], "type=randommap") != NULL) {
            if (WORLD->current_level == HUB_LDATA) {
                i16 model = Hub_GetRandomCharType();
                if (model != -1 && model_count < 10)
                    models[model_count++] = model;
            }
        } else if (NuStrIStr(params[index], "type=generic_goon") != NULL) {
            if (model_count < 10) {
                i16 model = GetGenericGoon(0);
                if (model != -1)
                    models[model_count++] = model;
            }
        } else if ((value = NuStrIStr(params[index], "type")) != NULL) {
            value += 5;
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                if (WORLD->current_level != HUB_LDATA) {
                    u8 type = LevelCharacterTypeIDFn(value);
                    if (type != 0xff) {
                        i16 model = LevelCharacterGlobalIDFn(type);
                        if (model != -1 && model_count < 10)
                            models[model_count++] = model;
                    }
                } else {
                    i16 model;
                    if (NuStrICmp("Barman", value) == 0)
                        model = id_BARMAN;
                    else if (NuStrICmp("JABBA", value) == 0)
                        model = id_JABBA;
                    else if (NuStrICmp("CANTINABAND", value) == 0)
                        model = id_CANTINABAND;
                    else
                        continue;
                    if (apicharsys->playermodelids[model] != -1 && model_count < 10)
                        models[model_count++] = model;
                }
            }
        } else if ((value = NuStrIStr(params[index], "state=")) != NULL)
            state = value + 6;
        else if ((value = NuStrIStr(params[index], "script")) != NULL)
            NuStrCpy(script, value + 7);
        else if ((value = NuStrIStr(params[index], "xspacing")) != NULL)
            xspacing = AIParamToFloat(processor, value + 9);
        else if ((value = NuStrIStr(params[index], "zspacing")) != NULL)
            zspacing = AIParamToFloat(processor, value + 9);
        else if ((value = NuStrIStr(params[index], "xoffset")) != NULL)
            offset.x = AIParamToFloat(processor, value + 8);
        else if ((value = NuStrIStr(params[index], "yoffset")) != NULL)
            offset.y = AIParamToFloat(processor, value + 8);
        else if ((value = NuStrIStr(params[index], "zoffset")) != NULL)
            offset.z = AIParamToFloat(processor, value + 8);
        else if ((value = NuStrIStr(params[index], "nacross")) != NULL)
            across = static_cast<i32>(AIParamToFloat(processor, value + 8));
        else if (NuStrIStr(params[index], "addtoset=myset") != NULL)
            creature_set = processor->creature_set;
        else if ((value = NuStrIStr(params[index], "addtoset=")) != NULL)
            creature_set = static_cast<i32>(AIParamToFloat(processor, value + 9));
        else if ((value = NuStrIStr(params[index], "crate_height=")) != NULL)
            crate_height = AIParamToFloat(processor, value + 13);
        else if ((value = NuStrIStr(params[index], "crate_delay=")) != NULL)
            crate_delay = AIParamToFloat(processor, value + 12);
        else if (NuStrICmp("not_grouped", params[index]) == 0)
            grouped = 0;
        else if (NuStrICmp("dont_set_on_surface", params[index]) == 0)
            surface = 0;
        else if (NuStrICmp("zipdown", params[index]) == 0) {
            zipdown = 1;
            surface = 0;
        } else if (NuStrICmp("nearest_player", params[index]) == 0)
            nearest = 1;
        else if (NuStrICmp("off_screen", params[index]) == 0 || NuStrICmp("offscreen", params[index]) == 0)
            offscreen = 1;
        else if (NuStrICmp("on_screen", params[index]) == 0)
            onscreen = 1;
        else if ((value = NuStrIStr(params[index], "fall_chance=")) != NULL)
            fall_chance = AIParamToFloat(processor, value + 12);
        else if (NuStrICmp("CheapAsChips", params[index]) == 0)
            cheap = 1;
        else if (NuStrIStr(params[index], "ridden_by=myself") != NULL) {
            if (packet != NULL && packet->owner != NULL)
                rider = packet->owner->apiobj.objptr;
        } else if ((value = NuStrIStr(params[index], "ridden_by=")) != NULL)
            rider = GetNamedGameObject(sys, value + 10);
        else if (NuStrICmp("no_gun", params[index]) == 0)
            no_gun = 1;
        else if (NuStrICmp("ballooning", params[index]) == 0) {
            ballooning = 1;
            surface = 0;
        } else if ((value = NuStrIStr(params[index], "lose_capability=")) != NULL)
            remove_capabilities |= static_cast<i32>(ParseAIPathCnxFlag(value + NuStrLen("lose_capability=")));
        else if ((value = NuStrIStr(params[index], "capability=")) != NULL)
            add_capabilities |= static_cast<i32>(ParseAIPathCnxFlag(value + NuStrLen("capability=")));
    }
    if (not_low_end != 0 && g_lowEndLevelBehaviour != 0)
        return 1;
    if (minimum >= 0) {
        if (maximum > minimum) {
            count = minimum;
            if (g_lowEndLevelBehaviour == 0)
                count += NuRand(NULL) % (maximum + 1 - minimum);
        } else if (maximum == minimum)
            count = maximum;
    }
    f32 delay = 0.0f;
    i32 offset_applied = 0;
    while (count != 0) {
        if ((locator_count | zipup_count) == 0 && rider == NULL && start_locator == 0)
            break;
        if (model_count == 0)
            break;
        i32 model = models[NuRand(NULL) % model_count];
        AILOCATOR *locator = NULL;
        ZIPUP *zipup = NULL;
        if ((locator_count | start_locator) != 0) {
            if (start_locator != 0 && locator_set != NULL) {
                if (locator_set->locator_count == 0)
                    break;
                locator = &sys->locators[locator_set->locator_entries[0]];
            } else {
                f32 clip_radius = 0.0f;
                if (offscreen != 0)
                    clip_radius = 0.5f + apicharsys->char_data[static_cast<i16>(model)].collision_radius;
                if (nearest != 0)
                    locator = LocalGetNearestLocator(locators, locator_count, clip_radius, &player->apiobj.position,
                                                     max_range, onscreen, max_dy, min_dy);
                else if (max_range == 1000000000.0f)
                    locator = LocalGetRandomLocator(locators, locator_count, clip_radius, NULL, 1000000000.0f, onscreen,
                                                    max_dy, min_dy);
                else
                    locator =
                        LocalGetRandomLocator(locators, locator_count, clip_radius, &player->apiobj.collision_position,
                                              max_range, onscreen, max_dy, min_dy);
            }
            if (locator == NULL)
                break;
            path = &locator->path_info;
            position = locator->position;
            yaw = locator->flags;
            offset_applied = 0;
        } else if (zipup_count != 0) {
            i32 eligible = 0;
            for (i32 i = 0; i < zipup_count && i < 32; ++i) {
                if (zipups[i] != NULL && (zipups[i]->flags & 0x40) != 0)
                    ++eligible;
            }
            if (eligible == 0)
                break;
            i32 selection = qrand() / (65535 / eligible + 1);
            zipup = zipups[selection];
            zipups[selection] = NULL;
            if (zipup == NULL)
                break;
            position = zipup->lower_position;
            path = NULL;
            surface = 0;
        } else if (rider != NULL) {
            path = &rider->ai.path_info;
            position = rider->ai.last_path_position;
            yaw = rider->apiobj.field_0x276;
        }
        if (offset_applied == 0 && (offset.x != 0.0f || offset.y != 0.0f || offset.z != 0.0f)) {
            NuVecRotateY(&offset, &offset, yaw);
            NuVecAdd(&position, &position, &offset);
            offset_applied = 1;
        }
        if (count > 0) {
            i32 members = 1;
            i32 make_group = 0;
            if (grouped == 0)
                --count;
            else {
                members = count;
                make_group = count > 1;
                count = 0;
            }
            AIGROUP *group = NULL;
            for (i32 member = 0; member < members; ++member) {
                if (member == 0 && make_group != 0) {
                    GAMECHARACTERDATA *data =
                        static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[model].field11_0x24);
                    group = CreateAIGroup(sys, across, xspacing, zspacing, data->movement_speed);
                }
                GameObject_s *object = AddDynamicCreature(model, &position, yaw, script, path, group, surface, NULL,
                                                          NULL, 0, creature_set);
                if (object == NULL)
                    continue;
                if (state != NULL)
                    AIScriptSetBaseScriptStateByName(&object->ai.script_process, state);
                object->ai.locator = locator;
                if (inherit != 0)
                    object->ai.locator_set = locator_set;
                if (no_gun != 0)
                    object->field_0xef8 &= ~4;
                if (crate_height != 0.0f)
                    SpawnCreatureFromCrate(object, crate_height, delay);
                delay += crate_delay;
                if (locator_set != NULL && object->ai.locator != NULL) {
                    for (i32 i = 0; i < locator_set->locator_count; ++i) {
                        if (locator_set->locator_entries[i] == object->ai.locator - sys->locators) {
                            locator_set->assigned[i] = object->apiobj.field_0x289;
                            break;
                        }
                    }
                }
                if (zipup != NULL) {
                    object->field_0x788 = zipup;
                    zipup->runtime_flags |= 1;
                    ZIPUP *attached_zipup = static_cast<ZIPUP *>(object->field_0x788);
                    object->context_flags |= 0x20;
                    object->field_0x7a5 = 0x47;
                    object->context_animation = 0x2a;
                    object->apiobj.movement_facing_angle =
                        NuAtan2D(attached_zipup->upper_position.x - attached_zipup->lower_position.x,
                                 attached_zipup->upper_position.z - attached_zipup->lower_position.z);
                    object->field_0xe22 |= 1;
                    object->apiobj.velocity.y = 0.0f;
                    object->context_animation_timer = 0.0f;
                    object->weapon_scale_state = static_cast<WEAPON_SCALE_STATE>(object->weapon_scale < 1.0f);
                    PlaySfx("ZipUp", &object->apiobj.collision_position);
                    PlayJumpSfx(object, 0);
                    object->field_0xe31 = 0;
                    ZIPUP *current_zipup = static_cast<ZIPUP *>(object->field_0x788);
                    f32 dx = current_zipup->hook_origin.x - attached_zipup->lower_position.x;
                    f32 dz = current_zipup->hook_origin.z - attached_zipup->lower_position.z;
                    i32 angle = NuAtan2D(current_zipup->hook_origin.y - attached_zipup->lower_position.y,
                                         NuFsqrt(dx * dx + dz * dz));
                    i32 rotation = 0x4000 - (angle < 0 ? -angle : angle);
                    if (angle < 0)
                        rotation = -rotation;
                    object->context_x_rotation = rotation;
                } else if (zipdown != 0 && !(fall_chance > 0.0f && fall_chance > NuRandFloat())) {
                    object->field_0x7a5 = 0x35;
                    object->context_animation = 0x2a;
                    object->zipup_entry_position = object->apiobj.upper_position;
                    SetWeaponOut(object);
                }
                if (cheap != 0) {
                    object->apiobj.flags_low |= 0x20;
                    object->apiobj.flags_high |= 4;
                }
                if (rider != NULL)
                    TakeOverGameObject(rider, object, 0, 1);
                else if (ballooning != 0) {
                    StartBallooning(object, 0);
                    if (locator != NULL)
                        SetBallooningHeight(object, locator->position.y);
                }
                if (add_capabilities != 0)
                    object->ai.capabilities |= static_cast<u32>(add_capabilities);
                if (remove_capabilities != 0)
                    object->ai.capabilities &= ~static_cast<u32>(remove_capabilities);
            }
        }
    }
    return 1;
}

void GameAIScriptAddLevelSfx(WORLDINFO *world, NULISTHDR *scripts) {
    if (scripts == NULL) {
        return;
    }
    for (NULISTLNK *script_node = NuLinkedListGetHead(scripts); script_node != NULL;
         script_node = NuLinkedListGetNext(scripts, script_node)) {
        AISCRIPT *script = reinterpret_cast<AISCRIPT *>(script_node);
        for (NULISTLNK *state_node = NuLinkedListGetHead(&script->states); state_node != NULL;
             state_node = NuLinkedListGetNext(&script->states, state_node)) {
            AISTATE *state = reinterpret_cast<AISTATE *>(state_node);
            for (NULISTLNK *action_node = NuLinkedListGetHead(&state->actions); action_node != NULL;
                 action_node = NuLinkedListGetNext(&state->actions, action_node)) {
                AIACTION *action = reinterpret_cast<AIACTION *>(action_node);
                if (action->def == NULL || action->def->eval_fn != Action_PlaySfx || action->param_count < 1) {
                    continue;
                }
                for (i32 index = 0; index < action->param_count; ++index) {
                    char *name = NuStrIStr(action->params[index], const_cast<char *>("name="));
                    if (name != NULL) {
                        AddLevSfx(world, NULL, name + 5, -1);
                    }
                }
            }
        }
    }
}
