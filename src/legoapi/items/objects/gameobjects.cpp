#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/screens/arcade.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"

#include <stdio.h>
#include <string.h>

void SetObjAsHeadTarget(GameObject_s *, GameObject_s *, i8, f32, f32, f32);
void SetBallooningHeight(GameObject_s *, f32);
void GameObjectSetCanUse(GameObject_s *, void *, u8, u8, f32);
i32 Suit_GetIndex(SUIT_s *);
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
void Bolt_Shoot(GameObject_s *, i32, i32);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
void PartUpdate_Basketball(PART_s *);
void PartImpact_Basketball(PART_s *);
void AddSlamDebris(GameObject_s *);
void Batarang_Release(GameObject_s *, i32);
void ThermalDetonator_Throw(GameObject_s *);
void BobaRocket_Kill(PART_s *, i32);
void BobaRocket_Move(PART_s *, f32);
void BobaRocket_Deflect(PART_s *);
void PartCollide_3D(PART_s *);
void ConfigureComplexShadow(GameObject_s *);
f32 DropInOutScale(GameObject_s *);
extern f32 rocket_speed;
extern "C" f32 sabrerubwait;
NUVEC *GetZapOrigin(GameObject_s *);
void PlaySabreSfx(char *, GameObject_s *, NUVEC *, i32);
extern i32 testlaser_type;
i32 lightning_type;
extern f32 testlaser_endw;
extern f32 testlaser_sizewab;
extern f32 testlaser_sizel;
extern f32 testlaser_sizew;
extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;
extern i16 id_MOUSEDROID;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;
extern i32 VehicleArea;
extern i32 TERRAINMASK_NONWEAPON, TERRAINMASK_NONDROID;
extern f32 DEFENDTIME;
extern f32 jump_stuck_time;
extern i32 teleport_all_freeplay_modes;
extern i32 drop_in_teleport;

static f32 Condition_IAmAPartyCharacter(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.field_0x27c != -1) {
        return 1.0f;
    }
    return 0.0f;
}

extern "C" {
    void AddToAIGroup(AIGROUP_s *group, APIOBJECT_s *object);
    extern NUVEC plr_lastpos;
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_BASKETCANNON;
    extern i16 id_R2Q5;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    extern i32 FalconDebKey[2];
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).
extern void *theThingManager;
extern void oneAtOnce_MaintainArray();

void legoSetMusicVolume(float);
void MovePlayer(GameObject_s *object);
void AnimatePlayer(GameObject_s *object);
void TerrainPlayer(GameObject_s *object);
void KeepOnScreen(GameObject_s *object);
void SetPlayer();
void InitPlayerAI(GameObject_s *object);
void ResetPlayerMoves(GameObject_s *object);
void MovePlayerSpline(GameObject_s *object);
void GetTopBot(GameObject_s *object);
void ResetRumble(RUMBLEPACKET *packet);
void InitSurfaceInfo(GameObject_s *object);
i32 TightRope_SnapTo(GameObject_s *object, NUVEC *position);
void Player_ClearContext(GameObject_s *object, i32 mode);
void Player_ResetContexts(PLAYERPACKET_s *packet);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
void StarWars_GameAISysInit();
void GameAISysSetGame();
void ClearAICreatures();
void CollideGameObjects(WORLDINFO_s *world);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
i32 TagCode(GameObject_s *source, GameObject_s *target, i32 takeover, i32 blend, i32 mode);
extern i32 do_player_tag;
extern f32 player_tag_timer;
extern GameObject_s *player_tag_from;
extern GameObject_s *player_tag_to;
APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);
i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special);
void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);
void AIPathCnxHelperSysReset(WORLDINFO_s *world, AIPATHCNXHELPERSYS_s *system);
void InitAICreatures(AISYS_s *system);
void ResetAICreatures(AISYS_s *system);
void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process);
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world);
void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index);
void ResetCoinPacket(COINPACKET_s *packet);
GIZMOPICKUP_s *GizmoPickups_Collide(WORLDINFO_s *world, GameObject_s *object, i32 arg);

f32 Condition_InHubArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);
void *Condition_InHubAreaInit(AISYS_s *, char *, AISCRIPT_s *);

extern i32 LEGO_AIPATHCNX_BLOCKAGE;

extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

GAMEFOG_STATE GameFog = {};
f32 dof_fstop;

static i32 GameFogSnap;
static rtlfog_s *GameFogSet;
static f32 GameFogDuration;
static f32 GameFogTime;
static GAMEFOG_STATE GameFogOld;
static GAMEFOG_STATE GameFogNew;

#include "legoapi/items/objects/gameobjects_types.h"






// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.
i32 timebase_updates = 1;


extern TERRSET *CurTerr;

static f32 Condition_OnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_PlayerOnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_EitherPlayerOnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_OnForcePlatformInit(AISYS_s *, char *name, AISCRIPT_s *);

static void *Condition_ForcePushingInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_ForcePushing(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_EitherPlayerUsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_PlayerUsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_UsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_UsingForceInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ForceBeingUsed(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_ForceInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ForceAtStart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceAtEnd(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_NumForceObjects(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_NumForceObjectsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_ForceStackComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceStackCompleteInOrder(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_ForceCompleteInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ObstacleOpenedByPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ObstacleOpenedByEitherPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                  void *argument);

static void *Condition_ObstacleOpenedByPlayerInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ObstacleLockedOpen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ObstacleLockedShut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_GizSpecialInit(AISYS_s *, char *name, AISCRIPT_s *);

static void *Condition_ObstacleInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ObstacleAtStart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ObstacleAtEnd(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_OnSpeederBikeInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_OnSpeederBike(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_Player2Active(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_TakenOverInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_TakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *name, void *argument);

static void *Condition_LastLevelInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_LastLevel(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_IsVisibleInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_IsVisible(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_Indy(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PSP(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_CheatProgress(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_ChallengeMode(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_Freeplay(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_MissionMode(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_MissionWon(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_NumInSetAlive(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *, void *argument);

static f32 Condition_IsSetAlive(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *, void *argument);

static void *Condition_IsSetAliveInit(AISYS_s *, char *arg, AISCRIPT_s *);

extern "C" i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *cutscene);

static f32 Condition_SockXDistanceToPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_PlayerDeflectingPart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_CollidingWithOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_OpponentPathPosRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_OpponentToPlayerRange(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_SockDistanceToPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_SockDistanceToOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_FurthestPlayerDistanceAlongSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PlayerDistanceAlongSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PlayerInSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_PlayerInSockInit(AISYS_s *, char *name, AISCRIPT_s *);

extern "C" f32 NuAnimEndFrameOld(void *animation);

extern i32 Hub_GetRandomCharType();
extern u8 hub_custodians_finished_loading;
extern "C" {
extern i32 nbaddies_can_see_players;
}

static f32 Condition_NumBaddiesThatCanSeePlayers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_OnSameObjectAsPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_RandomMapCharsAvailable(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_EitherPlayerWearingHelmet(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_AreaContainsPartyMember(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                             void *argument);

static void *Condition_AreaContainsPartyMemberInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_CharacterTypeExists(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CharacterTypeExistsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_EitherPlayerInMyTriggerArea(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                                 void *);

static f32 Condition_OnDynamicGrapple(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_OnDynamicGrappleInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_AngleAboutMyLocatorToPlayer(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *packet, char *,
                                                 void *argument);

static void *Condition_AngleAboutMyLocatorToPlayerInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_EitherPlayerUsingHatMachine(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

static f32 Condition_EitherPlayerPullingLever(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

static f32 Condition_EitherPlayerUsingPanel(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

static f32 Condition_AreaContainsBaddies(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                         void *argument);

static void *Condition_AreaContainsBaddiesInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_AreaContainsGoodies(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                         void *argument);

static void *Condition_AreaContainsGoodiesInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_PickupBeenTurnedOn(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_PickupBeenTurnedOnInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_AnimationFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_AnimationFinishedInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_RigidAnimFrame(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_RigidAnimFrameInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_FinishedSpline(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_CutSceneFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CutSceneFinishedInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CutSceneStarted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CutSceneStartedInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CutSceneExists(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CutSceneExistsInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CutScenePlaying(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_CutScenePlayingInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_Message(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_MessageInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_ScriptParam(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *, void *argument);

static void *Condition_ScriptParamInit(AISYS_s *, char *name, AISCRIPT_s *script);

static f32 Condition_Blocking(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_RaceLap(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_MusicOn(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_GotOpponentLOS(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_GotLocatorInSet(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                     void *argument);

static void *Condition_GotLocatorInSetInit(AISYS_s *system, char *name, AISCRIPT_s *);

i32 CanFightLikeAJedi(GameObject_s *object);

static f32 Condition_CanFightLikeAJedi(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_EitherPlayerPushingSpinner(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_EitherPlayerPushingSpinnerInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CharacterRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_CharacterRangeInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_MaulShouldRunAway(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_BeenTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_BeenTakenOverInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_LastAttackerRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_LastAttackerIsActivePlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_CannotReachDestination(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_BeenSpawned(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_ShouldAttackOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_RespawnLocatorIs(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_RespawnLocatorIsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_BoltsDontGetDeflectedBack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_DropBackInTimer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_InMiniCut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *arg, void *);

static f32 Condition_BigJumpComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_AIOverrideControl(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_AIOverrideControlInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_IsOnScreen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_IsOnScreenInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_IAmPlayer2(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_PlayerOnObject(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_OnObjectInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_PlayerOnGround(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_UnderPlayerControl(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_UnderPlayerControlInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_PlayerTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_EitherPlayerTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PartyContainsDroids(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_OpponentContext(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_InContextInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_InContext(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_HitPointsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_HitPoints(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_CurrentHintId(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_HintAvailableInit(AISYS_s *, char *argument, AISCRIPT_s *);

static f32 Condition_HintAvailable(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_HintComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_EmptyTakeOver(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

extern f32 default_path_heighttol;
extern u8 default_activate_difficulty;
extern u8 default_min_n_respawns;
extern u8 default_max_n_respawns;
extern f32 default_min_t_respawn;
extern f32 default_max_t_respawn;

extern CHARACTERNAMEFN * LevelCharacterNameFn;
extern CHARACTERNAMEFN * SpecialRouteCharacterNameFn;
extern CHARACTERGLOBALIDFN * LevelCharacterGlobalIDFn;
extern GLOBALCHARACTERNAMEFN * GlobalCharacterNameFn;
extern CHARACTERHGOBJFN * GlobalCharacterHGobjFn;
extern CHARACTERRENDERFN * GlobalCharacterRenderFn;
extern CHARACTERGOALSPEEDFN * GetCharacterGoalSpeedFn;
extern CHARACTERTYPEIDFN * LevelCharacterTypeIDFn;

extern AICHARACTERTYPEID * GlobalCharacterTypeIDFn;
extern AISPECIALROUTECHARACTERTYPEID * SpecialRouteCharacterTypeIDFn;
extern AICHARACTERDISTANCE * GetViewRangeFn;
extern AICHARACTERDISTANCE * GetHearDistanceFn;
extern AICHARACTERDISTANCE * GetMaxViewHeightFn;
extern AICHARACTERDISTANCE * GetMinViewHeightFn;
extern GAMEAILOAD * GameAILoadFn;
extern AIACTIONPARSESPEED * AIActionParseSpeedFn;
extern AIBIGJUMPTODESTINATION * AIBigJumpToDestinationFn;
extern AIRESPAWNONPATH * AIRespawnOnPathFn;
extern AICLEARCREATURES * ClearAICreaturesFn;
extern APIOBJECTFROMOBJID * APIOBJECTFromObjIDFn;
extern AIFINDALTERNATIVESPECIALOBJECT * FindAlternativeSpecialObjectFn;
extern AIGETNAMEDAPIOBJECT * GetNamedAPIObjectFn;
extern AIGETCREATUREORIGIN * GetAICreatureOriginFn;
}


// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.

static char *LevelCharacterName(u8 character_index);

static i32 LevelCharacterGlobalID(u8 character_index);

static char *GlobalCharacterName(i32 character_type);

static void *GlobalCharacterHGobj(i32 character_type);

static f32 GetViewRange(i32 character_type);

static f32 GetHearDistance(i32 character_type);

static f32 GetMaxViewHeight(i32 character_type);

static f32 GetMinViewHeight(i32 character_type);

static NUVEC *GetAICreatureOrigin(AISYS *system, AIPACKET *packet);

static APIOBJECT *GetNamedAPIObject(AISYS *system, char *name);

static i32 GlobalCharacterTypeID(char *name);

static i32 GameFindAlternativeSpecialObject(AISYS *, nuhspecial_s *special);

static void GameAILoad(AISYS *system, i32 version, NUGSCN *, VARIPTR *, VARIPTR *);

static void GlobalCharacterRender(NUVEC *position, i16 angle, i32 character, i32, EDCREATURE_s *);

static f32 GetCharacterGoalSpeed(APIOBJECT *object);

static i32 GameAIActionParseSpeed(char *name, u8 *speed);

static char *SpecialRouteCharacterName(u8 route_id);

static i32 LevelCharacterTypeID(char *name);

static u32 AIBigJumpToDestination(APIOBJECT *object, NUVEC *destination);

static u32 AIRespawnOnPath(APIOBJECT *object);

static i32 SpecialRouteCharacterTypeID(char *name);

extern i32 TimingBarSet;
extern "C" i32 SHADOWCALLS;
extern u32 LAYER_HOVERIGNORE;

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask) {
    i32 disabled_platforms[16];
    i32 disabled_platform_count = 0;

    if (object != NULL) {
        WORLDINFO_s *world = WorldInfo_CurrentlyActive();
        if (object->field_0x107c != -1) {
            disabled_platforms[disabled_platform_count++] = object->field_0x107c;
        }

        if (world != NULL && world->char_platform_sys != NULL && VehicleArea == 0 &&
            !object->apiobj.player_controlled) {
            CHARPLATFORMSYS_s *system = world->char_platform_sys;
            for (i32 i = 0; i < system->platform_count; ++i) {
                if (system->platforms[i].object != NULL) {
                    disabled_platforms[disabled_platform_count++] = system->platforms[i].object->field_0x107c;
                }
            }
        }

        for (i32 i = 0; i < disabled_platform_count; ++i) {
            PlatOnOff(disabled_platforms[i], 0);
        }

        if (LAYER_HOVERIGNORE != 0xffffffff &&
            static_cast<GAMECHARACTERDATA_s *>(object->apiobj.character_data->field11_0x24)->field_0x28 == 0.0f) {
            terrain_mask &= ~LAYER_HOVERIGNORE;
        }
    }

    if (TimingBarSet == 2) {
        TBOPENFN("Ter", 2);
    }
    const f32 shadow_height = NewShadowEx(position, 0, probe_height, probe_height, terrain_mask);
    ++SHADOWCALLS;
    if (TimingBarSet == 2) {
        TBCLOSEFN("Ter", 2);
    }

    if (object != NULL && object->field_0x107c != -1) {
        PlatOnOff(object->field_0x107c, 1);
    }
    for (i32 i = 0; i < disabled_platform_count; ++i) {
        PlatOnOff(disabled_platforms[i], 1);
    }
    return shadow_height;
}

extern "C" f32 MainRenderTime;
extern "C" f32 MainRenderTargetTime;
extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;
extern void (*BackDrop_AlphaFn)(f32 *alpha);
extern void BackDrop_UpdateColours(i32 instant);
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

void UpdateGameMessages();
extern "C" i32 DoubleScore;
extern FadeSystem FadeSys;
extern f32 LevelNameTime, LevelNameMul;
extern i32 pause_rndr_on;
extern "C" TIMER BonusTimer;
extern "C" i32 screendump;

void GameTiming(WORLDINFO_s *world, float *game_time) {
    f32 level_name_target = 1.0f;
    if (LevelNameTime > 0.0f)
        LevelNameTime -= FRAMETIME;
    else
        level_name_target = 0.0f;
    LevelNameMul = SeekLinearF(LevelNameMul, level_name_target, FRAMETIME + FRAMETIME);

    if (Paused != 0) {
        if (screendump == 0) {
            UpdateTimer(&PauseTimer);
            if (LevelNameTime < 0.5f)
                LevelNameTime = 0.5f;
        }
        DoubleScoreTime = 0.0f;
    } else {
        if (game_time != NULL)
            *game_time += FRAMETIME;
        UpdateTimer(&GameTimer);
        UpdateTimer(&LevelTimer);
        UpdateTimer(&AreaTimer);
        UpdatePickupFlicker();
        UpdateTimer(&JoinInTimer);
        if (CUTSTOPGAME != 0) {
            DoubleScoreTime = 0.0f;
        } else {
            if (world != NULL && world->current_level != NULL && (world->current_level->flags & LEVEL_GAMEPLAY) != 0 &&
                pause_rndr_on == 0 && (Player[0] != NULL || Player[1] != NULL)) {
                UpdateTimer(&GamePlayTimer);
                UpdateTimer(&OverallGamePlayTimer);
                if (BonusWinner == -1)
                    UpdateTimer(&BonusTimer);
                if ((world->current_level->flags & 0x2000) != 0) {
                    UpdateTimer(&SuperStoryTimer);
                    if (SuperStoryTimer.time_elapsed > 36000.0f)
                        ResetTimer(&SuperStoryTimer, 36000.0f);
                }
                if (world->area != NULL && (world->area->flags & 0x10) != 0) {
                    if (ChallengeMode == 1) {
                        UpdateTimer(&ChallengeTimer);
                        if (Game_AreaSave != NULL && world->level_sub_id >= 0 &&
                            !(ChallengeTimer.time_elapsed < Game_AreaSave[world->level_sub_id].challenge_trial_time))
                            EndChallenge(3, 1);
                    } else {
                        if (Mission_CurrentState(NULL) == 1 && MissionSys != NULL && MissionSys->mission != NULL) {
                            const f32 time_limit = static_cast<f32>(MissionSys->mission->time);
                            UpdateTimer(&MissionSys->timer);
                            if (time_limit <= MissionSys->timer.time_elapsed && netclient == 0)
                                EndMission(MissionSys, 3, 1);
                        }
                    }
                }
            }
            UpdateGameMessages();
            f32 score_step = FRAMETIME;
            f32 target = 0.0f;
            if (DoubleScore != 0 && GetMenuID() == -1)
                target = 1.0f;
            DoubleScoreTime = SeekLinearF(DoubleScoreTime, target, score_step);
        }
    }
    UpdateTimer(&GlobalTimer);
    menu_flash = NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.2f) < 0.1f;
    f32 pulse_time = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    game_pulse = NuTrigTable[static_cast<u16>(static_cast<i32>((pulse_time + pulse_time) * 65536.0f)) / 2];
    pulse_time = NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);
    global_pulse = NuTrigTable[static_cast<u16>(static_cast<i32>((pulse_time + pulse_time) * 65536.0f)) / 2];
    MainRenderTime = SeekLinearF(MainRenderTime, MainRenderTargetTime, FRAMETIME);
    qrand();
}

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);
extern "C" i32 RAYCASTCALLS;

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask) {
    i32 hit = NewRayCastScaleYMask(position, displacement, radius, 1.0f, 0, mask);
    ++RAYCASTCALLS;
    return hit;
}

extern f32 draw_attention_distance;
extern f32 party_follow_offsets[8];
extern i32 active_neutral_count;
extern "C" i32 party_under_cover;
extern "C" i32 party_cant_be_under_cover;
extern AREADATA *VADER_ADATA;
extern GameObject_s *alert_obj;
void AITriggerSetSysProcess(AITRIGGERSETSYS_s *system);
i32 Hub_InMenu();
extern i32 test_new_interaction;
extern i32 debug_ignore_players;
extern i32 WALKSTEALTH;
extern i32 active_goody_count;
extern i32 active_baddy_count;
extern f32 timetogetlos;
void GameCreatureOpponentSelection(AISYS_s *, i32, APIOBJECT **, i32, APIOBJECT **, i32, APIOBJECT **, u64, f32);
void GizTurrets_OpponentSelection(GIZTURRETSYS_s *, i32, APIOBJECT **, i32, APIOBJECT **);

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.
static GAMEOBJECTS_CLONE_ATTR2_SSE void TestWalkAround(APIOBJECT *object, f32 *other_x, f32 *other_z,
                                                       NUVEC *difference, f32 radius)
    __asm__("_ZL14TestWalkAroundP11APIOBJECT_sS0_P7nuvec_sf.isra.10");


void GameAIProcess();

extern "C" {
void InitFn_LevelCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_SpecialRouteCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_LevelCharacterName(CHARACTERNAMEFN *function);

void InitFn_SpecialRouteCharacterName(CHARACTERNAMEFN *function);

void InitFn_LevelCharacterGlobalID(CHARACTERGLOBALIDFN *function);

void InitFn_GlobalCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_GlobalCharacterName(GLOBALCHARACTERNAMEFN *function);

void InitFn_GlobalCharacterRender(CHARACTERRENDERFN *function);

void InitFn_GlobalCharacterHGobj(CHARACTERHGOBJFN *function);

void InitFn_ClearAICreatures(AICLEARCREATURES *function);

void InitFn_GetCharacterGoalSpeedFn(CHARACTERGOALSPEEDFN *function);

void InitFn_GetViewRange(CHARACTERDISTANCEFN *function);

void InitFn_GetHearDistance(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMaxViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMinViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GameAILoad(GAMEAILOAD *function);

void InitFn_AIActionParseSpeed(AIACTIONPARSESPEED *function);

void InitFn_AIRespawnOnPath(AIRESPAWNONPATH *function);

void InitFn_AIBigJumpToDestination(AIBIGJUMPTODESTINATION *function);

void InitFn_FindAlternativeSpecialObjectFn(AIFINDALTERNATIVESPECIALOBJECT *function);

void InitFn_APIOBJECTFromObjIDFn(APIOBJECTFROMOBJID *function);

void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function);

void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function);

void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}

extern f32 OFFSCREEN_CATCHUP_TIME;
extern f32 drop_back_in_timer;

i32 TryToTeleportToNextNode(GameObject_s *object, AIPATHNODE_s *node, i32 tightrope);

void SetSpecialMove(GameObject_s *object, AIPATHNODE_s *target, AIPATHNODE_s *node, char move);

void ClearSpecialMove(GameObject_s *object);

void GameAISysInit();

void GameFog_Reset() {
    GameFogSnap = 1;
    GameFogDuration = 0.0f;
    GameFogSet = 0;
    GameFogTime = 0.0f;
}

void Game_KillPart(PART_s *part, i32 reason) {
    if (reason == 1 && part->force_player_mask != -1) {
        if ((part->force_player_mask & 1) != 0 && Player[0] != NULL) {
            NewBuzz(Player[0]->pad_gamepad->pad, 0.2f, 0);
            NewRumble(Player[0]->pad_gamepad->pad, 0.8f, 0);
        }
        if ((part->force_player_mask & 2) != 0 && Player[1] != NULL) {
            NewBuzz(Player[1]->pad_gamepad->pad, 0.2f, 0);
            NewRumble(Player[1]->pad_gamepad->pad, 0.8f, 0);
        }
    }
}

void GameAISysReset(AISYS_s *system);

extern "C" i32 party_under_cover;
extern "C" i32 party_cant_be_under_cover;
extern GameObject_s *alert_obj;
extern f32 alert_timer;
extern AREADATA *VADER_ADATA;

void MakeBaddiesForgetAboutParty(i32 check_hostility);

void GameAttackInit();

extern "C" void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);

void GameFog_Update(WORLDINFO_s *world) {
    rtlfog_s *previous = GameFogSet;
    if (world->rtl_set != NULL &&
        !(CUTSTOPGAME != 0 && CutStopInfo != NULL && (static_cast<CUTINFO *>(CutStopInfo)->flags & 4) != 0)) {
        GameFogSet = rtlGetFogSet(world->rtl_set, NUMTX_GET_ROW_VEC(&pNuCam->mtx, 3));
    } else {
        GameFogSet = NULL;
    }

    if (GameFogSnap == 0 && GameFogSet != previous) {
        GameFogTime = 0.0f;
        GameFogDuration = 3.0f;
        GameFogOld = GameFog;
    }
    if (GameFogSnap != 0 || GameFogSet != previous) {
        if (GameFogSet != NULL) {
            u32 colour = GameFogSet->colour;
            GameFogNew.high_quality_colour_channels[0] = colour >> 24;
            GameFogNew.high_quality_colour_channels[1] = (colour >> 16) & 0xff;
            GameFogNew.high_quality_colour_channels[2] = (colour >> 8) & 0xff;
            GameFogNew.high_quality_colour_channels[3] = colour & 0xff;
            GameFogNew.high_quality_start = GameFogSet->start;
            GameFogNew.high_quality_end = GameFogSet->end;
            GameFogNew.high_quality_density = GameFogSet->density;
            GameFogNew.low_quality_density = GameFogSet->low_quality_density;
            colour = GameFogSet->low_quality_colour;
            GameFogNew.low_quality_colour_channels[0] = colour >> 24;
            GameFogNew.low_quality_colour_channels[1] = (colour >> 16) & 0xff;
            GameFogNew.low_quality_colour_channels[2] = (colour >> 8) & 0xff;
            GameFogNew.low_quality_colour_channels[3] = colour & 0xff;
        } else {
            GameFogNew.high_quality_colour_channels[0] = GameFogNew.high_quality_colour_channels[1] =
                GameFogNew.high_quality_colour_channels[2] = GameFogNew.high_quality_colour_channels[3] = 0.0f;
            GameFogNew.high_quality_start = 5.0f;
            GameFogNew.high_quality_end = 20.0f;
            GameFogNew.high_quality_density = 0.0f;
            GameFogNew.low_quality_density = 0.0f;
            GameFogNew.low_quality_colour_channels[0] = GameFogNew.low_quality_colour_channels[1] =
                GameFogNew.low_quality_colour_channels[2] = GameFogNew.low_quality_colour_channels[3] = 0.0f;
        }
    }

    f32 fstop = 22.0f;
    if (GameFogSet != NULL) {
        fstop = static_cast<f32>(static_cast<u32>(GameFogSet->depth_of_field_fstop)) / 10.0f;
        if (fstop == 0.0f)
            fstop = 22.0f;
    }
    dof_fstop = fstop;

    if (NuIOS_IsLowEndDevice() && world->current_level != NULL) {
        GameFogNew.low_quality_end = world->current_level->data_display.far_clip;
        GameFogNew.low_quality_start = world->current_level->data_display.fog_start;
    }
    if (GameFogSnap != 0) {
        GameFog = GameFogNew;
        GameFogDuration = 0.0f;
        GameFogTime = 0.0f;
    } else {
        if (GameFogTime < GameFogDuration) {
            GameFogTime += FRAMETIME;
            if (GameFogTime > GameFogDuration)
                GameFogTime = GameFogDuration;
        }
        if (GameFogTime >= GameFogDuration) {
            GameFog = GameFogNew;
        } else {
            f32 blend = 1.0f - (NU_SIN_LUT(GameFogTime / GameFogDuration * 32768.0f + 16384.0f) + 1.0f) * 0.5f;
            GameFog.high_quality_colour_channels[0] =
                GameFogOld.high_quality_colour_channels[0] +
                (GameFogNew.high_quality_colour_channels[0] - GameFogOld.high_quality_colour_channels[0]) * blend;
            GameFog.high_quality_colour_channels[1] =
                GameFogOld.high_quality_colour_channels[1] +
                (GameFogNew.high_quality_colour_channels[1] - GameFogOld.high_quality_colour_channels[1]) * blend;
            GameFog.high_quality_colour_channels[2] =
                GameFogOld.high_quality_colour_channels[2] +
                (GameFogNew.high_quality_colour_channels[2] - GameFogOld.high_quality_colour_channels[2]) * blend;
            GameFog.high_quality_colour_channels[3] =
                GameFogOld.high_quality_colour_channels[3] +
                (GameFogNew.high_quality_colour_channels[3] - GameFogOld.high_quality_colour_channels[3]) * blend;
            GameFog.high_quality_start =
                GameFogOld.high_quality_start + (GameFogNew.high_quality_start - GameFogOld.high_quality_start) * blend;
            GameFog.high_quality_end =
                GameFogOld.high_quality_end + (GameFogNew.high_quality_end - GameFogOld.high_quality_end) * blend;
            GameFog.high_quality_density = GameFogOld.high_quality_density +
                                           (GameFogNew.high_quality_density - GameFogOld.high_quality_density) * blend;
            GameFog.low_quality_density = GameFogOld.low_quality_density +
                                          (GameFogNew.low_quality_density - GameFogOld.low_quality_density) * blend;
            GameFog.low_quality_colour_channels[0] =
                GameFogOld.low_quality_colour_channels[0] +
                (GameFogNew.low_quality_colour_channels[0] - GameFogOld.low_quality_colour_channels[0]) * blend;
            GameFog.low_quality_colour_channels[1] =
                GameFogOld.low_quality_colour_channels[1] +
                (GameFogNew.low_quality_colour_channels[1] - GameFogOld.low_quality_colour_channels[1]) * blend;
            GameFog.low_quality_colour_channels[2] =
                GameFogOld.low_quality_colour_channels[2] +
                (GameFogNew.low_quality_colour_channels[2] - GameFogOld.low_quality_colour_channels[2]) * blend;
            GameFog.low_quality_colour_channels[3] =
                GameFogOld.low_quality_colour_channels[3] +
                (GameFogNew.low_quality_colour_channels[3] - GameFogOld.low_quality_colour_channels[3]) * blend;
        }
    }
    GameFog.colour = (static_cast<i32>(GameFog.high_quality_colour_channels[0]) << 24) +
                     (static_cast<i32>(GameFog.high_quality_colour_channels[1]) << 16) +
                     (static_cast<i32>(GameFog.high_quality_colour_channels[2]) << 8) +
                     static_cast<i32>(GameFog.high_quality_colour_channels[3]);
    GameFog.low_quality_colour = (static_cast<i32>(GameFog.low_quality_colour_channels[0]) << 24) +
                                 (static_cast<i32>(GameFog.low_quality_colour_channels[1]) << 16) +
                                 (static_cast<i32>(GameFog.low_quality_colour_channels[2]) << 8) +
                                 static_cast<i32>(GameFog.low_quality_colour_channels[3]);
    GameFogSnap = 0;
    g_BackgroundColour = GameFog.colour;
}

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size) {
    if (buf_end == NULL || buf == NULL || buf_end->addr <= buf->addr + size) {
        return NULL;
    }
    buf->addr = (buf->addr + 3) & ~usize(3);
    void *ptr = buf->void_ptr;
    buf->addr += size;
    if (ptr == NULL) {
        return NULL;
    }
    memset(ptr, 0, size);
    return ptr;
}

void GameAISysSetGame();

void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx) {
        return;
    }

    i32 sfx_id = GameAudio_GetSfxId(sfx);
    if (sfx_id == -1) {
        return;
    }
    for (i32 i = 0; i < *sfx_count; ++i) {
        if (sfx_ids[i] == sfx_id) {
            return;
        }
    }
    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void GameObjectOrigin(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    api.field_0x1f4 |= 0x100u;

    f32 predicted_vertical_displacement;
    f32 origin_x;
    f32 origin_z;
    PLAYERCHARACTERCONFIG_s *config;
    i32 model_origin_joint;
    if (object->use_model_origin != 0 &&
        (config = api.character_data->player_config, model_origin_joint = config->model_origin_joint) != -1 &&
        api.character_model != NULL && api.character_model->points_of_interest[model_origin_joint] != NULL) {
        f32 predicted_x;
        f32 predicted_z;
        if ((object->field_0xe24 & GAMEOBJECT_E24_FLAG_JOINT_MATRICES_UPDATED) != 0) {
            const f32 frame_time = FRAMETIME;
            predicted_x = api.previous_velocity.x * frame_time;
            predicted_vertical_displacement = api.previous_velocity.y * frame_time;
            predicted_z = api.previous_velocity.z * frame_time;
            const NUVEC &joint_position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[model_origin_joint], 3);
            api.collision_position.z = joint_position.z + predicted_z;
            api.collision_position.x = joint_position.x + predicted_x;
            api.collision_position.y = joint_position.y + predicted_vertical_displacement;
            api.collision_position.y += (object->character_bottom + object->character_top) * api.field_0xa8 * 0.5f;
        } else {
            api.collision_position = {0.0f, -object->character_bottom, 0.0f};
            NuVecMtxRotate(&api.collision_position, &api.collision_position, &api.field_0xb8);
            NuVecAdd(&api.collision_position, &api.collision_position, &api.position);
            const f32 frame_time = FRAMETIME;
            predicted_x = api.previous_velocity.x * frame_time;
            predicted_vertical_displacement = api.previous_velocity.y * frame_time;
            predicted_z = api.previous_velocity.z * frame_time;
        }
        api.collision_position.x += predicted_x;
        api.collision_position.y += predicted_vertical_displacement;
        api.collision_position.z += predicted_z;
        origin_x = api.position.x + predicted_x;
        origin_z = api.position.z + predicted_z;
    } else if (object->use_model_origin != 0 && object->field_0xd24 == 1.0f && config->collision_origin_joint != -1 &&
               api.character_model->points_of_interest[config->collision_origin_joint] != NULL &&
               api.field_0x288 != 0) {
        api.collision_position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[config->collision_origin_joint], 3);
        const f32 frame_time = FRAMETIME;
        origin_x = api.position.x + api.previous_velocity.x * frame_time;
        origin_z = api.position.z + api.previous_velocity.z * frame_time;
        predicted_vertical_displacement = api.previous_velocity.y * frame_time;
    } else {
        api.field_0x1f4 ^= 0x100u;
        predicted_vertical_displacement = api.previous_velocity.y * FRAMETIME;
        api.collision_position.x = api.position.x + api.previous_velocity.x * FRAMETIME;
        api.collision_position.z = api.position.z + api.previous_velocity.z * FRAMETIME;
        api.collision_position.y = api.position.y + predicted_vertical_displacement;
        api.collision_position.y += (object->character_bottom + object->character_top) * api.field_0xa8 * 0.5f;
        origin_x = api.collision_position.x;
        origin_z = api.collision_position.z;
    }

    const f32 radius = api.field_0x1dc;
    api.collision_origin.x = origin_x;
    api.collision_origin.z = origin_z;
    api.upper_position.x = origin_x;
    api.upper_position.z = origin_z;
    api.lower_position.x = origin_x;
    api.lower_position.z = origin_z;
    api.collision_min.x = api.collision_position.x - radius;
    api.collision_max.x = api.collision_position.x + radius;
    api.collision_min.z = api.collision_position.z - radius;
    api.collision_max.z = api.collision_position.z + radius;
    api.collision_min.y = api.collision_position.y - api.field_0x1e0;
    api.collision_max.y = api.collision_position.y + api.field_0x1e0;
    api.upper_position.y = api.collision_max.y;
    api.lower_position.y = api.collision_min.y;
    api.collision_origin.y = api.collision_min.y + predicted_vertical_displacement;

    object->ai.terrain_origin = api.collision_origin;
    const u32 context_flags = CInfo[object->character_context].flags;
    if ((context_flags & CHARACTER_CONTEXT_INFO_FLAG_TERRAIN_ORIGIN_AT_TOP) != 0 &&
        (object->context_variant_flags & 0x08) != 0) {
        object->ai.terrain_origin.y += api.collision_max.y - api.collision_min.y;
    } else if ((context_flags & CHARACTER_CONTEXT_INFO_FLAG_TERRAIN_ORIGIN_AT_POSITION) != 0) {
        object->ai.terrain_origin = api.position;
    } else {
        object->ai.terrain_origin.y = api.collision_origin.y - object->terrain_origin_floor_offset;
        if (object->ai.terrain_origin.y < api.field_0x218 &&
            api.field_0x218 < api.collision_origin.y + api.scaled_height) {
            object->ai.terrain_origin.y = api.field_0x218;
        }
    }
}

void PortalGameObject(GameObject_s *object, i32 enabled, i32 update_room, i16 room, nugscn_s *scene) {
    if (enabled != 0) {
        object->field_0xefa |= 0x40;
        object->field_0xefa = (object->field_0xefa & 0x7f) | ((update_room & 1) << 7);
        if (room != -1)
            object->room_id = room;
        else
            object->room_id = scene != NULL ? NuPortalWhichRoom(scene, &object->apiobj.position) : -1;
    } else {
        object->field_0xefa &= ~0x40;
    }
}

void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface) {
    object->apiobj.position = *position;
    object->apiobj.field_0x276 = angle;
    object->apiobj.facing_angle = angle;
    object->apiobj.movement_facing_angle = angle;
    object->apiobj.initial_position = object->apiobj.position;
    object->apiobj.collision_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.respawn_position = object->apiobj.position;
    object->apiobj.last_safe_position = object->apiobj.position;
    object->ai_update_position = object->apiobj.position;
    object->reset_velocity = v000;
    object->apiobj.velocity = v000;
    InitSurfaceInfo(object);
    if (set_on_surface != 0) {
        SetObjOnSurface(object, 0);
    }
    if (path_info != NULL) {
        object->ai.path_info = *path_info;
    } else {
        AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
    }
}

i32 Game_IgnoreInput();

extern i16 id_GONKDROID;
u32 GameAI_TotalScore();

void GameDrawMenuEntry(MENU_s *menu, char *text) {
    if (Paused != 0) {
        dme_align = PauseMenus_Align;
        menu->draw_x = PauseMenus_X;
    }
    DrawMenuEntryEx(menu, text, static_cast<u8>(MenuA));
}

void GameAnimSys_Update(GAMEANIMSYS_s *system) {
    if (system == NULL) {
        return;
    }

    GAMEANIMSET_s *set = reinterpret_cast<GAMEANIMSET_s *>(NuLinkedListGetHead(&system->active_sets));
    while (set != NULL) {
        GAMEANIMSET_s *next_set =
            reinterpret_cast<GAMEANIMSET_s *>(NuLinkedListGetNext(&system->active_sets, &set->links));

        const u8 was_no_visibility_test = set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;
        set->flags &= ~GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;

        if ((set->flags & GAMEANIMSET_FLAG_STOP_REQUESTED) != 0) {
            set->flags &= ~(GAMEANIMSET_FLAG_NO_VISIBILITY_TEST | GAMEANIMSET_FLAG_STOP_REQUESTED);
            GameAnimSet_RemoveFromSystemList(set);
            set = next_set;
            continue;
        }

        set->state = GAMEANIMSET_STATE_AT_START;
        i32 all_playing_forward = 1;
        i32 all_at_start = 1;
        i32 all_at_end = 1;
        i32 any_special_no_visibility_test = 0;

        GAMEANIMOBJ_s *object = set->objects;
        while (object != NULL) {
            if (NuSpecialGetNoVisiTestFn(&object->special) != 0) {
                any_special_no_visibility_test = 1;
            }

            if (object->instance_animation != NULL) {
                const f32 direction = object->start_frame > object->end_frame ? -1.0f : 1.0f;
                const f32 previous_frame = object->instance_animation->ltime;

                if (previous_frame * direction >= object->end_frame * direction) {
                    if (object->instance_animation->tfactor * direction < 0.0f) {
                        object->instance_animation->ltime = object->end_frame;
                    } else if (object->instance_animation->repeating != 0) {
                        object->instance_animation->ltime = previous_frame - object->end_frame + object->start_frame;
                    } else {
                        object->instance_animation->ltime = object->end_frame;
                        object->instance_animation->playing = 0;
                    }
                } else if (previous_frame * direction <= object->start_frame * direction) {
                    if (object->instance_animation->tfactor * direction > 0.0f) {
                        object->instance_animation->ltime = object->start_frame;
                    } else if (object->instance_animation->repeating != 0) {
                        object->instance_animation->ltime = object->end_frame - (object->start_frame - previous_frame);
                    } else {
                        object->instance_animation->ltime = object->start_frame;
                        object->instance_animation->playing = 0;
                    }
                }

                if (previous_frame != object->instance_animation->ltime) {
                    EvalAnim2(&object->special, object->instance_animation->ltime);
                }

                if (object->instance_animation->playing != 0) {
                    set->flags |= GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;
                    if (object->instance_animation->tfactor * direction < 0.0f) {
                        all_playing_forward = 0;
                    }
                }

                const f32 directed_frame = object->instance_animation->ltime * direction;
                if (directed_frame > object->start_frame * direction) {
                    all_at_start = 0;
                }
                if (directed_frame < object->end_frame * direction) {
                    all_at_end = 0;
                }
            }
            object = object->next;
        }

        if ((set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST) != any_special_no_visibility_test) {
            object = set->objects;
            auto set_flags = set->flags;
            while (object != NULL) {
                NuSpecialSetNoVisiTest(&object->special, set_flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST);
                object = object->next;
            }
        }

        if ((set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST) != 0) {
            set->state =
                all_playing_forward != 0 ? GAMEANIMSET_STATE_ACTIVE_FORWARD : GAMEANIMSET_STATE_ACTIVE_BACKWARD;
        } else {
            if (all_at_end != 0) {
                set->state = GAMEANIMSET_STATE_AT_END;
            } else if (all_at_start == 0) {
                set->state = GAMEANIMSET_STATE_BETWEEN_ENDPOINTS;
            }
            if (was_no_visibility_test != 0) {
                set->flags |= GAMEANIMSET_FLAG_STOP_REQUESTED;
            }
        }

        if ((set->flags & (GAMEANIMSET_FLAG_NO_VISIBILITY_TEST | GAMEANIMSET_FLAG_STOP_REQUESTED)) == 0) {
            GameAnimSet_RemoveFromSystemList(set);
        }
        set = next_set;
    }
}

CABLE_s cables[8];
CABLE_s *GameObjIsCableTied(GameObject_s *object) {
    if (__builtin_expect(cables[0].target != object, 1))
        return NULL;

    if ((cables[0].flags_1e9 & 1) != 0)
        return &cables[0];
    if ((cables[1].flags_1e9 & 1) != 0)
        return &cables[1];
    if ((cables[2].flags_1e9 & 1) != 0)
        return &cables[2];
    if ((cables[3].flags_1e9 & 1) != 0)
        return &cables[3];
    if ((cables[4].flags_1e9 & 1) != 0)
        return &cables[4];
    if ((cables[5].flags_1e9 & 1) != 0)
        return &cables[5];
    if ((cables[6].flags_1e9 & 1) != 0)
        return &cables[6];
    if ((cables[7].flags_1e9 & 1) != 0)
        return &cables[7];
    return NULL;
}

void FindAnglesZX(NUVEC *, u16 *, u16 *);
f32 GetVehicleHoverHeight(GameObject_s *, f32 *);
u16 SeekRot(u16, u16, f32);
i32 RotDiff(u16, u16);
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

// Original: reads the user's music volume from the options save as the product
// of two 0..10 sliders scaled to 0..1 (option bytes at 0x4 and 0x5).
f32 GameGetMusicVolume(OPTIONSSAVE_s *options) {
    return ((f32)(u8)options->field5_0x5 / 10.0f) * ((f32)(u8)options->field4_0x4 / 10.0f);
}

// Original: applies GameGetMusicVolume, zeroing it while the title logos are
// up (SuperOptions.music_enabled == 0 on the titles level); the title menu restores
// the user's volume via GameGetMusicVolume once the menu phase starts.
f32 GameSetMusicVolume(OPTIONSSAVE_s *options) {
    f32 volume = GameGetMusicVolume(options);
    if (SuperOptions.music_enabled == 0 && WORLD->current_level == TITLES_LDATA) {
        volume = 0.0f;
    }
    legoSetMusicVolume(volume);
    return volume;
}

void GameAISysStartFrame(AISYS_s *system);

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours) {
    CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && cut->camera_near_clip != 0.0f) {
        pNuCam->near_clip = cut->camera_near_clip;
    } else {
        pNuCam->near_clip = display->unknown_04;
    }

    u16 far_clip;
    if (cut != NULL) {
        far_clip = cut->camera_far_clip;
        if (far_clip == 0) {
            far_clip = static_cast<u16>(display->unknown_14);
        }
    } else {
        far_clip = static_cast<u16>(display->unknown_14);
    }
    pNuCam->far_clip = static_cast<f32>(static_cast<u32>(far_clip));

    LEVELDATA *level = WORLD->current_level;
    const bool use_backdrop = level == TITLES_LDATA || (level->flags & LEVEL_STATUS) != 0 || level == STATUS_LDATA ||
                              level == CREDITS_LDATA || MainRenderTime < 0.0f;
    if (!use_backdrop) {
        background_colours[0] =
            0x80000000u | static_cast<u8>(display->bg_red_top) |
            (static_cast<u8>(display->bg_green_top) << 8 | static_cast<u8>(display->bg_blue_top) << 16);
        background_colours[1] =
            0x80000000u | static_cast<u8>(display->bg_red_bottom) |
            (static_cast<u8>(display->bg_green_bottom) << 8 | static_cast<u8>(display->bg_blue_bottom) << 16);
        return;
    }

    BackDrop_UpdateColours(0);

    i32 top_b = static_cast<i32>(backdrop_top_b);
    i32 top_g = static_cast<i32>(backdrop_top_g);
    i32 top_r = static_cast<i32>(backdrop_top_r);
    i32 bottom_r = static_cast<i32>(backdrop_bot_r);
    i32 bottom_b = static_cast<i32>(backdrop_bot_b);
    i32 bottom_g = static_cast<i32>(backdrop_bot_g);

    f32 alpha = 1.0f;
    if (BackDrop_AlphaFn != NULL) {
        BackDrop_AlphaFn(&alpha);
        if (alpha != 0.0f) {
            top_r = static_cast<i32>(static_cast<f32>(top_r) * alpha);
            top_b = static_cast<i32>(static_cast<f32>(top_b) * alpha);
            top_g = static_cast<i32>(static_cast<f32>(top_g) * alpha);
            bottom_r = static_cast<i32>(static_cast<f32>(bottom_r) * alpha);
            bottom_g = static_cast<i32>(static_cast<f32>(bottom_g) * alpha);
            bottom_b = static_cast<i32>(static_cast<f32>(bottom_b) * alpha);
        }
    }

    background_colours[0] = 0x80000000u | (top_r & 0xff) | (top_g & 0xff) << 8 | (top_b & 0xff) << 16;
    background_colours[1] = 0x80000000u | (bottom_r & 0xff) | (bottom_g & 0xff) << 8 | (bottom_b & 0xff) << 16;
}

u8 grapple_attach_frames = 5;
void GameObjectSetCanUse(GameObject_s *object, void *target, unsigned char action, unsigned char, float parameter) {
    object->can_use_object = target;
    object->use_action = action;
    object->use_action_parameter = parameter;
    object->use_action_frames = grapple_attach_frames;
}

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object) {
    // The original checks the first cable's owner for the entire array.
    if (__builtin_expect(cables[0].source != object, 1))
        return NULL;

    if ((cables[0].flags_1e9 & 1) != 0)
        return &cables[0];
    if ((cables[1].flags_1e9 & 1) != 0)
        return &cables[1];
    if ((cables[2].flags_1e9 & 1) != 0)
        return &cables[2];
    if ((cables[3].flags_1e9 & 1) != 0)
        return &cables[3];
    if ((cables[4].flags_1e9 & 1) != 0)
        return &cables[4];
    if ((cables[5].flags_1e9 & 1) != 0)
        return &cables[5];
    if ((cables[6].flags_1e9 & 1) != 0)
        return &cables[6];
    if ((cables[7].flags_1e9 & 1) != 0)
        return &cables[7];
    return NULL;
}

void GameObjectDimensionsExtra_LSW(GameObject_s *object);

void GameObjectDimensions(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    if (object->use_model_origin != 0 && object->field_0xd24 == 1.0f &&
        api.character_data->player_config->collision_origin_joint != -1 &&
        api.character_model->points_of_interest[api.character_data->player_config->collision_origin_joint] != NULL &&
        api.field_0x288 != 0) {
        const f32 radius = object->field_0x1004 * api.character_data->player_config->collision_origin_radius;
        api.field_0x1dc = radius;
        api.field_0x1e0 = radius;
    } else {
        api.field_0x1dc = api.collision_radius;
        api.field_0x1e0 = api.collision_height;
    }
    if (GameObjectDimensionsExtraFn != NULL)
        GameObjectDimensionsExtraFn(object);
}

void GameAntiNodeData_Init(GAMEANTINODEDATA_s *data, nuhspecial_s *special) {
    if (data != NULL) {
        memset(data, 0, sizeof(*data));
        if (special != NULL && NuSpecialExistsFn(special)) {
            NUVEC minimum;
            NUVEC maximum;
            NuSpecialGetBounds(special, &minimum, &maximum);
            NUMTX *matrix = NuSpecialGetMtx(special);
            NuVecMtxTransform(&minimum, &minimum, matrix);
            NuVecMtxTransform(&maximum, &maximum, matrix);
            f32 x = NuFabs((maximum.x - minimum.x) * 0.5f);
            f32 z = NuFabs((maximum.z - minimum.z) * 0.5f);
            if (NuFabs(x - z) < 0.01f) {
                data->radius = x;
                data->use_largest_extent = 0;
            } else {
                data->radius = x > z ? x : z;
                data->use_largest_extent = 1;
            }
            data->extent_x = x;
            data->extent_z = z;
            data->max_y = NuFabs((maximum.y - minimum.y) * 0.5f);
            data->min_y = -data->max_y;
        }
    }
}

void GameAntiNodeData_Read(GAMEANTINODEDATA_s *data) {
    if (EdFileReadChar() == 0) {
        return;
    }
    EdFileReadNuVec(&data->position);
    data->radius = EdFileReadFloat();
    data->min_y = EdFileReadFloat();
    data->max_y = EdFileReadFloat();
    data->extent_x = EdFileReadFloat();
    data->extent_z = EdFileReadFloat();
    data->flags = EdFileReadShort();
    data->use_largest_extent = static_cast<u8>(EdFileReadChar());
    data->mode = static_cast<u8>(EdFileReadChar());
}

APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);

i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern i16 id_ANAKINJEDISCARRED;

void GameBlowUpBlownUpFn_LSW(GIZMOBLOWUP_s *blowup) {
    extern AREADATA *MOSEISLEY_ADATA;
    extern AREADATA *TATOOINE_ADATA;
    if (blowup == NULL || blowup->type == NULL || WORLD->area == NULL || netclient != 0)
        return;
    if (WORLD->area == MOSEISLEY_ADATA || WORLD->area == HUB_ADATA) {
        char *name = NuSpecialGetName(&blowup->type->special);
        if (name == NULL || NuStrIStr(name, "bin_lid") != NULL || NuStrIStr(name, "bin") == NULL || qrand() >= 49200)
            return;
    } else if (WORLD->area == TATOOINE_ADATA) {
        char *name = NuSpecialGetName(&blowup->type->special);
        if (name == NULL || NuStrIStr(name, "womp_gen") == NULL)
            return;
    } else {
        return;
    }
    AddDynamicCreature(id_WOMPRAT, &blowup->position, 0, "spawned_womprat", NULL, NULL, 1, NULL, NULL, 0, 16);
}

void GameLoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append, VARIPTR *buf, VARIPTR *buf_end, i32 area_models,
                             i32 area) {
    if (area_models != 0 && CutScenePlayer_Active() != 0 && area != -1 && &ADataList[area] != HUB_ADATA) {
        area_models = 0;
    }

    APILoadCharacterModels(list, append, buf, *buf_end, area_models);
}

void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *);

extern GameObject_s *alert_obj;
extern NUVEC alert_pos;
extern f32 alert_timer;
extern "C" {
extern i32 party_under_cover;
}
i32 ZapTarget(GameObject_s *object);

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float);

void GameObjectDimensionsExtra_LSW(GameObject_s *object);

i32 AnakinGreenSabre(GameObject_s *object);
extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
i32 CannotKill(GameObject_s *object);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *, NUVEC *, i32, f32, NUVEC *, NUVEC *, BOLT_s *, u32, u8 *);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);
extern f32 ForceThrowSpeed, ForceThrowGravity;
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);

static void LightSabreStreakCode(GameObject_s *object, i32 blade, i32 effect);

void GameObjectStuffAfterAnimation();

void GameMsg_DrawAdjustNewPos_CoinToTotal(GAMEMESSAGE_s *message);

void GameObject_s::ClearAddons() {
    delete addons;
    addons = NULL;
}

void GameObject_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

MechAddonCollection *GameObject_s::GetAddons(bool create) {
    if (addons == NULL && create) {
        MechObjectInterface *object = GetMechObjectInterface();
        addons = new MechAddonCollection(*object);
    }
    return addons;
}

MechObjectInterface *GameObject_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL)
        new CharacterObjectInterface(*this);
    return mech_object_interface;
}

bool GameObject_s::IsRunningTaskType(HashedKey const &type) {
    MechTouchTask *task = touch_task;
    while (task != NULL) {
        MechTouchTask *next = task->next;
        if (task->GetHashId().value == type.value)
            return true;
        task = next;
    }
    return false;
}

void GameObject_s::KillTasks() {
    MechTouchTask *task = touch_task;
    while (task != NULL) {
        MechTouchTask *next = task->next;
        task->OnStop();
        delete task;
        task = next;
    }
    touch_task = NULL;
}














// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}








extern "C" {
    ed_module_s edTimingDesc = {NULL,
                                NULL,
                                "Timings",
                                ThingManager::edTimingInit,
                                NULL,
                                ThingManager::edTimingEnter,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                0x656d6974,
                                ThingManager::edTimingProc,
                                ThingManager::edTimingRender,
                                NULL};
}

i32 SpecialObject::Exists() const {
    return NuSpecialExistsFn(const_cast<nuhspecial_s *>(&special));
}

i32 SpecialObject::GetCollision() const {
    return NuSpecialGetCollision(const_cast<nuhspecial_s *>(&special)) != 0;
}

VuVec const *SpecialObject::GetCurrentPosition() const {
    return reinterpret_cast<VuVec const *>(NuSpecialGetDrawPos(const_cast<nuhspecial_s *>(&special)));
}

VuMtx const *SpecialObject::GetCurrentTransform() const {
    return reinterpret_cast<VuMtx const *>(NuSpecialGetDrawMtx(const_cast<nuhspecial_s *>(&special)));
}

VuVec const *SpecialObject::GetInitialPosition() const {
    return reinterpret_cast<VuVec const *>(NuSpecialGetPos(const_cast<nuhspecial_s *>(&special)));
}

VuMtx const *SpecialObject::GetInitialTransform() const {
    return reinterpret_cast<VuMtx const *>(NuSpecialGetMtx(const_cast<nuhspecial_s *>(&special)));
}

numtl_s *SpecialObject::GetMtl(i32 index) const {
    return NuSpecialGetMtl(const_cast<nuhspecial_s *>(&special), index);
}

char const *SpecialObject::GetName() const {
    return NuSpecialGetName(const_cast<nuhspecial_s *>(&special));
}

i32 SpecialObject::GetNumMtls() const {
    return NuSpecialNumMtls(const_cast<nuhspecial_s *>(&special));
}

f32 SpecialObject::GetRadius() const {
    f32 radius = 1.0f;
    NUVEC position;
    if (NuSpecialExistsFn(const_cast<nuhspecial_s *>(&special))) {
        NuSpecialGetRadius(const_cast<nuhspecial_s *>(&special), &position, &radius);
    }
    return radius;
}

i32 SpecialObject::GetVisibility() const {
    return NuSpecialGetVisibilityFn(const_cast<nuhspecial_s *>(&special));
}

void SpecialObject::Render(VuMtx const *matrix) const {
    if (matrix != NULL) {
        NuSpecialDrawAt(const_cast<nuhspecial_s *>(&special), reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(matrix)));
    } else {
        NuSpecialDrawAt(const_cast<nuhspecial_s *>(&special),
                        NuSpecialGetDrawMtx(const_cast<nuhspecial_s *>(&special)));
    }
}

void SpecialObject::SetCollision(i32 enabled) {
    NuSpecialSetCollision(&special, enabled);
}

void SpecialObject::SetCurrentPosition(VuVec const *position) {
    NuSpecialSetDrawPos(&special, reinterpret_cast<NUVEC *>(const_cast<VuVec *>(position)));
}

void SpecialObject::SetCurrentTransform(VuMtx const *matrix) {
    NuSpecialSetDrawMtx(&special, reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(matrix)));
}

void SpecialObject::SetInitialPosition(VuVec const *) {
}

void SpecialObject::SetInitialTransform(VuMtx const *matrix) {
    NuSpecialSetMtx(&special, reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(matrix)));
}

void SpecialObject::SetVisibility(i32 enabled) {
    NuSpecialSetVisibility(&special, enabled);
}

SpecialObject::SpecialObject() {
    NuSpecialClear(&special);
}

void GameThingManager::AddLevelOnlyThings() {
}

// GameThingManager::AddOnceOnlyThings @0x4e8bb0: registers the MechSystems
// singleton as the manager's once-only thing (via the virtual AddThing slot).
void GameThingManager::AddOnceOnlyThings() {
    this->AddThing(MechSystems::Get());
}

// GameThingManager::GameThingManager @0x4e8b00: stores the object in
// theGameThings (the vptr switch to the derived vtable is compiler-generated).
GameThingManager::GameThingManager(i32 max_things) : ThingManager(max_things) {
    theGameThings = this;
}

// GameThingManager D1 dtor @0x4e8a80 clears the global before destruction.
GameThingManager::~GameThingManager() {
    theGameThings = NULL;
}



i32 BaseThing::RemoveDependancies(ThingRemoveData *) {
    return 1;
}

void BaseThing::EnterLevel(ThingLevelData *) {
}

void BaseThing::ExitLevel(ThingLevelData *) {
}

void BaseThing::Reset(ThingResetData *) {
}

void BaseThing::Process(ThingProcessData *) {
}

void BaseThing::ProcessEvenWhenPaused(ThingProcessData *) {
}

void BaseThing::ProcessOnlyWhenPaused(ThingProcessData *) {
}

void BaseThing::Render(ThingRenderData *) {
}

void BaseThing::Display(ThingRenderData *) {
}

void BaseThing::Effects(ThingRenderData *) {
}

extern CUSTOMISER *CharacterCustomiser;
extern CUSTOMPIECE *Customiser_FindPieceByName(CUSTOMISER *, char *, i32 *, i32 *);
extern void Customiser_Set100PercentPieces(CUSTOMISER *);

static __used__ void LEGO_100PercentFn();
static __used__ void LEGO_AllGoldBricksFn();

i32 NoLayerKill(GameObject_s *object) {
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if ((data->flags_094[2] & 0x80) != 0)
        return object->apiobj.field_0x27f == 6;
    return 0;
}



void InitExtraList() {
    for (i32 i = 0; i < 44; ++i) {
        NuStrCpy(ExtraItems[i].special_name, Cheat[i].extra_name);
        NuSpecialFind(WORLD->current_gscn, &ExtraItems[i].special, ExtraItems[i].special_name, 1);
        ExtraItems[i].type = 2;
        ExtraItems[i].unlocked = 0;
        ExtraItems[i].item_id = i;

        u32 unlocked = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&Game) + 0x7bf0)[i >> 5];
        if (((static_cast<u64>(unlocked) >> (i & 0x1f)) & 1) != 0) {
            ExtraItems[i].unlocked = 1;
            reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&Game) + 0x7c00)[i >> 5] |= 1u << (i & 0x1f);
        }
        ExtraItems[i].price = Cheat[i].extra_price;
    }

    SHOPEXTRACOUNT = 44;
    for (i32 i = 0; i < SHOPEXTRACOUNT; ++i) {
        char name[32];
        NuStrCpy(name, ExtraItems[i].special_name);
        NuStrCat(name, "b");
        NuSpecialFind(WORLD->current_gscn, &extrasils[i], name, 1);
    }
}

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only) {
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        if ((object->apiobj.flags_low & APIOBJECT_FLAG_IN_USE) != 0 &&
            (vehicle_only == 0 || (object->apiobj.flags_high & 0x10) != 0) &&
            (required_flags == 0 || (object->apiobj.field_0x1f4 & required_flags) == required_flags) &&
            (character_id == -1 || object->id == character_id) &&
            (alive_only == 0 ||
             (object->apiobj.field_0x287 == 0 && (CInfo[object->character_context].flags & 0x8000) == 0)) &&
            (non_level_only == 0 || object->field_0x107c == -1)) {
            return object;
        }
    }
    return NULL;
}

extern "C" void NuSound3ClearLoopHold(NUVEC *, i32);
void Grapple_RemoveDynamic(void *);
void Shards_HandleLostObj(WORLDINFO_s *, GameObject_s *);
void DestroySnakeBody(GameObject_s *);
void BlowUpSnakeBody(GameObject_s *);
void PlayDieSfx(GameObject_s *);
void DieRumble(GameObject_s *);
void GetTakeOverPos(GameObject_s *, NUVEC *);
void SetToLastSafePos(GameObject_s *);
void ReleaseLever(GameObject_s *);
void Batarangs_CheckLostData(void *);
i32 SpecialMove_ReleaseVictim(GameObject_s *);
void ReleaseEat(GameObject_s *);
void ReleasePush(GameObject_s *);
void ReleaseForce(GameObject_s *, i32);
void LoseHelmet(GameObject_s *, i32, i32);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);
extern EXPLOSION Explosion[8];
i32 speeder_hitpoints_lost;
extern i16 id_MOUSEDROID;

void ConstantRumble(GameObject_s *object, f32 strength, f32 phase);

void PowerUp_Update(GameObject_s *object);


GameObject_s *player_tag_to;
GameObject_s *player_tag_from;
f32 player_tag_timer;
i32 do_player_tag;
i32 TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, NUVEC *);
extern char *ASCII_DOWN;



void ScaleGameObject(GameObject_s *object) {
    const f32 scale = object->apiobj.field_0xa8;
    CHARACTERDATA *character = object->apiobj.character_data;
    object->apiobj.scaled_radius = character->field13_0x2c * scale;
    object->apiobj.collision_radius = object->field_0x1008 * scale;
    object->apiobj.collision_height = object->apiobj.collision_radius * object->collision_y_scale;
    object->apiobj.scaled_height = (character->field16_0x38 - character->field15_0x34) * object->field_0x1004;
}

void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32) {
    if (obj == NULL) {
        return;
    }

    obj->KillTasks();
    obj->ClearAddons();
    obj->ClearMechObjectInterface();

    GameObject_s *other = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++, other++) {
        other->apiobj.collision_contact_mask &= ~obj->apiobj.collision_identity_mask;
        other->apiobj.ai_awareness_mask &= ~(1ULL << obj->apiobj.field_0x289);
        other->ai_seen_mask &= ~(1ULL << obj->apiobj.field_0x289);
        other->ai_opponent_exclusion_mask &= ~(1ULL << obj->apiobj.field_0x289);
    }

    if (obj->pad_gamepad != NULL) {
        obj->pad_gamepad->allocated_5a &= ~1u;
    }
    DestroySnakeBody(obj);
    APIObjectDestroy(WORLD->api_object_sys, &obj->apiobj);

    i32 object_count = 0;
    for (i32 i = 0; i < 64; i++) {
        if ((Obj[i].apiobj.field_0x1f8 & 1) != 0) {
            object_count = i + 1;
        }
    }
    HIGHGAMEOBJECT = object_count;
    for (i32 i = 0; i < 8; i++) {
        if (LevGameObject[i] == obj) {
            LevGameObject[i] = NULL;
        }
    }
}

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);

extern i32 nethost;
extern i16 id_PKDROID;
void ResetAICreature(GameObject_s *, AISYS_s *);
void SpawnCreatureFromCrate(GameObject_s *, f32, f32);
void SetToLastSafePos(GameObject_s *);
GameObject_s *GetOtherActivePlayer(GameObject_s *);
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

extern "C" i32 adaptivedifficulty[3];
extern i8 (*adtab)[4];
f32 Hub_PadSpeed[2];
u16 Hub_PadAngle[2];
GameObject_s *CarWashHack;
f32 AIFireIntervalMul = 1.0f;
f32 GhostLightMul = 1.0f;
f32 GhostLightTargetMul = 1.0f;
f32 CurrentSpeedOverride;
f32 lightning_sizew[2] = {0.02f, 0.02f};
f32 lightning_sizel[2] = {0.1f, 0.1f};
f32 lightning_sizewab[2] = {0.01f, 0.01f};
f32 lightning_endw[2] = {0.1f, 0.1f};
u32 lightning_col[2] = {0xff808040, 0xff808040};
f32 CurrentSpeed = 10.0f;
f32 BaseCurrentSpeed = 10.0f;
extern AREADATA *PODSPRINT_ADATA;
extern i16 id_ANAKINSNEWPOD, id_ANAKINSNEWPODGREEN, id_SEBULBASPOD;
extern "C" f32 tieonsfxwait, tieoffsfxwait;
void MovePlayer_NETWORK(GameObject_s *);
void UpdateRumble(RUMBLEPACKET *);
void Player_ToggleCharacter(GameObject_s *, i32, i32);
void AveragePlayerCurrentSpeedMul();
void RegenerateHearts(GameObject_s *);
void AddSurfaceRipples(GameObject_s *);
void AddSurfaceDebris(GameObject_s *);

void GameAntinode_Update(GAMEANTINODESYS_s *system);

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set);

GameObject_s *GetNamedGameObject(AISYS_s *system, char *name);

void TakeOverGameObject2(GameObject_s *, GameObject_s *, i32);


void RegisterTakeOverObject(GameObject_s *object);


void DeactivateGameObject(GameObject_s *object);

i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special) {
    if (special != NULL && NuSpecialExistsFn(special)) {
        EQUIVALENTOBJECTGROUP_s *group = world->equivalent_groups;
        if (group != NULL) {
            for (i32 group_index = 0; group_index < world->equivalent_group_count; ++group_index) {
                for (i32 i = 0; i < group->object_count; ++i) {
                    if (NuSpecialCompare(special, &group->objects[i])) {
                        for (i32 j = 0; j < group->object_count; ++j) {
                            if (NuSpecialGetVisibilityFn(&group->objects[j])) {
                                *special = group->objects[j];
                                return 1;
                            }
                        }
                        return 0;
                    }
                }
                group = reinterpret_cast<EQUIVALENTOBJECTGROUP_s *>(reinterpret_cast<u8 *>(group) + group->byte_size);
            }
        }
    }
    return 0;
}

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match) {
    GameObject_s *nearest = NULL;
    f32 nearest_distance = 100000000.0f;
    f32 radius_squared = radius * radius;
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            (object->field_0xe20 & 0x20) || (CInfo[object->character_context].flags & 0x8000))
            continue;
        if (character_id != -1 && object->id != character_id)
            continue;
        if (player_index >= 0) {
            if (player_index <= 1) {
                if (object->apiobj.field_0x27c != player_index)
                    continue;
            } else if (player_index == 99) {
                if (object->apiobj.field_0x27c == -1)
                    continue;
            } else if (player_index == 100) {
                if (object->apiobj.field_0x27c != -1)
                    continue;
            }
        }
        if (exclude && object == exclude)
            continue;
        if (required_flags && (object->apiobj.character_data->model_flags & required_flags) != required_flags)
            continue;
        if (animation != -1 && object->apiobj.character_model->model_data_b[animation] == NULL)
            continue;
        if (filter && !filter(object))
            continue;
        f32 x = object->apiobj.collision_position.x - position->x;
        f32 y = object->apiobj.collision_position.y - position->y;
        f32 z = object->apiobj.collision_position.z - position->z;
        f32 distance = horizontal_only ? x * x + z * z : x * x + y * y + z * z;
        if (exclude) {
            if (radius_squared <= 0.0f || distance < radius_squared) {
                f32 combined_radius = exclude->apiobj.collision_radius + object->apiobj.collision_radius + extra_radius;
                if (distance < combined_radius * combined_radius && distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest = object;
                    if (first_match)
                        break;
                }
            }
        } else if ((radius_squared <= 0.0f || distance < radius_squared) && distance < nearest_distance) {
            nearest_distance = distance;
            nearest = object;
            if (first_match)
                break;
        }
    }
    if (nearest && distance_squared)
        *distance_squared = nearest_distance;
    return nearest;
}

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance) {
    const f32 no_floor_height = 2000000.0f;
    const f32 floor_height = object->apiobj.field_0x218;
    if (floor_height != no_floor_height) {
        const f32 radius_scale = object->apiobj.collision_radius / 0.225f;
        i32 height_steps = static_cast<i32>(height);
        if (height_steps < 0) {
            height_steps = 0;
        }
        f32 tolerance = static_cast<f32>(height_steps) * 0.025f;
        f32 radius_tolerance = radius_scale * tolerance;
        if (radius_tolerance < tolerance) {
            radius_tolerance = tolerance;
        }
        tolerance = radius_tolerance;

        const f32 floor_distance = object->apiobj.collision_min.y - floor_height;
        if (distance != NULL) {
            *distance = floor_distance;
        }
        return tolerance > floor_distance;
    }

    if (distance != NULL) {
        *distance = no_floor_height;
    }
    return 0;
}

void GameObjectToCameraDistances() {
    const NUVEC camera_position = GameCam->pos;
    GameObject_s *object = Obj;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
        const u16 required_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & required_flags) != required_flags) {
            continue;
        }

        const f32 dx = camera_position.x - object->apiobj.position.x;
        const f32 dy = camera_position.y - object->apiobj.position.y;
        const f32 dz = camera_position.z - object->apiobj.position.z;
        object->ai_update_distance = NuFsqrt(dx * dx + dy * dy + dz * dz);
    }
}

extern "C" {
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
}
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
void RemoveAnyChunkControls(i32 *chunk);

void RemoveChunkFromRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

void RemoveChunkControlFromStack(debris_chunk_control_s *control, debris_chunk_control_s **stack);

extern "C" debkeydatatype_s *debris_keystack;

void RemoveDebrisEffectFromStack(debkeydatatype_s *key);
