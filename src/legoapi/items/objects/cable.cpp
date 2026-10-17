// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/numusic/sfx.h"
#include <string.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/minikits.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/nutrig.h"

extern CABLE_s cables[8];
extern AREADATA_s *GUNSHIP_ADATA;
void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
f32 cable_ground_damping = 20.0f;
f32 cable_damping = 2.0f;
f32 cable_gravity = 3.0f;
f32 cable_speed = 20.0f;
i32 atat_locators[4] = {0, 1, 2, 3};
extern AREADATA_s *HOTHBATTLE_ADATA;
extern LEVELDATA_s *HOTHBATTLED_LDATA;
i32 FaceOpponent(GameObject_s *, NUVEC *);
extern i16 id_ATAT, id_ATST, id_ATST_LOWRES;
extern i32 TERRAINMASK_NONDROID;
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);
i32 XZLinesIntersect(NUVEC *, NUVEC *, NUVEC *, NUVEC *, f32 *, f32 *);
f32 default_targetmomf = 4.0f;
f32 targetmomf = 4.0f;
f32 default_ownermomf = 5.0f;
f32 ownermomf = 5.0f;
f32 default_max_cable_length = 15.0f;
f32 max_cable_length = 15.0f;
f32 default_tow_length = 6.0f;
f32 tow_length = 6.0f;

void InitCables(WORLDINFO_s *world);

CABLE_s *CreateCable(GameObject_s *source, GameObject_s *target, i32 sound);

void DestroyCable(CABLE_s *cable);
void ReleaseCable(CABLE_s *cable, i32 snapped);
void UpdateCables();

GameObject_s *CableTargetGameObject(GameObject_s *source, nuvec_s *position, f32 radius);

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====
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
extern i32 lightning_type;
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

extern GAMEFOG_STATE GameFog;
extern f32 dof_fstop;


#include "legoapi/items/objects/gameobjects_types.h"






// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.
extern i32 timebase_updates;


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

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask);

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

void GameTiming(WORLDINFO_s *world, float *game_time);

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);
extern "C" i32 RAYCASTCALLS;

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask);

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

void GameFog_Reset();

void Game_KillPart(PART_s *part, i32 reason);

void GameAISysReset(AISYS_s *system);

extern f32 alert_timer;

void MakeBaddiesForgetAboutParty(i32 check_hostility);

void GameAttackInit();

extern "C" void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);

void GameFog_Update(WORLDINFO_s *world);

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);


void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);

void GameObjectOrigin(GameObject_s *object);

void PortalGameObject(GameObject_s *object, i32 enabled, i32 update_room, i16 room, nugscn_s *scene);

void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface);

i32 Game_IgnoreInput();

extern i16 id_GONKDROID;
u32 GameAI_TotalScore();

void GameDrawMenuEntry(MENU_s *menu, char *text);

void GameAnimSys_Update(GAMEANIMSYS_s *system);

CABLE_s cables[8];
CABLE_s *GameObjIsCableTied(GameObject_s *object);

void FindAnglesZX(NUVEC *, u16 *, u16 *);
f32 GetVehicleHoverHeight(GameObject_s *, f32 *);
u16 SeekRot(u16, u16, f32);
i32 RotDiff(u16, u16);
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

f32 GameGetMusicVolume(OPTIONSSAVE_s *options);

f32 GameSetMusicVolume(OPTIONSSAVE_s *options);

void GameAISysStartFrame(AISYS_s *system);

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours);

extern u8 grapple_attach_frames;
void GameObjectSetCanUse(GameObject_s *object, void *target, unsigned char action, unsigned char, float parameter);

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object);

void GameObjectDimensionsExtra_LSW(GameObject_s *object);

void GameObjectDimensions(GameObject_s *object);

void GameAntiNodeData_Init(GAMEANTINODEDATA_s *data, nuhspecial_s *special);

void GameAntiNodeData_Read(GAMEANTINODEDATA_s *data);


i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern i16 id_ANAKINJEDISCARRED;

void GameBlowUpBlownUpFn_LSW(GIZMOBLOWUP_s *blowup);

void GameLoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append, VARIPTR *buf, VARIPTR *buf_end, i32 area_models,
                             i32 area);

void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *);

extern NUVEC alert_pos;
extern "C" {
extern i32 party_under_cover;
}
i32 ZapTarget(GameObject_s *object);

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float);


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




















// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}








extern "C" {
extern ed_module_s edTimingDesc;
}




































extern CUSTOMISER *CharacterCustomiser;
extern CUSTOMPIECE *Customiser_FindPieceByName(CUSTOMISER *, char *, i32 *, i32 *);
extern void Customiser_Set100PercentPieces(CUSTOMISER *);

static __used__ void LEGO_100PercentFn();
static __used__ void LEGO_AllGoldBricksFn();

i32 NoLayerKill(GameObject_s *object);



void InitExtraList();

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only);

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
extern i32 speeder_hitpoints_lost;

void ConstantRumble(GameObject_s *object, f32 strength, f32 phase);

void PowerUp_Update(GameObject_s *object);


extern GameObject_s * player_tag_to;
extern GameObject_s * player_tag_from;
i32 TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, NUVEC *);
extern char *ASCII_DOWN;



void ScaleGameObject(GameObject_s *object);

void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32);

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;

extern i32 nethost;
extern i16 id_PKDROID;
void ResetAICreature(GameObject_s *, AISYS_s *);
void SpawnCreatureFromCrate(GameObject_s *, f32, f32);
GameObject_s *GetOtherActivePlayer(GameObject_s *);
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

extern "C" i32 adaptivedifficulty[3];
extern i8 (*adtab)[4];
extern f32 Hub_PadSpeed[2];
extern u16 Hub_PadAngle[2];
extern GameObject_s * CarWashHack;
extern f32 AIFireIntervalMul;
extern f32 GhostLightMul;
extern f32 GhostLightTargetMul;
extern f32 CurrentSpeedOverride;
extern f32 lightning_sizew[2];
extern f32 lightning_sizel[2];
extern f32 lightning_sizewab[2];
extern f32 lightning_endw[2];
extern u32 lightning_col[2];
extern f32 CurrentSpeed;
extern f32 BaseCurrentSpeed;
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


GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance);

void GameObjectToCameraDistances();

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

extern i32 GAMEDEMO;
extern f32 FORCEGLOWTIME;
extern i16 tHINTS;
extern i16 tCHARACTERS;
extern i16 tEXTRAS;
extern i16 tGOLDBRICKS;
extern i16 tSTORYCLIPS;
extern i16 tENTERCODE;
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
extern f32 cointotaltime;
extern "C" i32 editor_active;
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

i32 solid_cable;
i32 nsegments_drawn;
f32 slack_factor = 1.0f;
f32 cable_slack = 1.0f;
f32 nsegments_per_unit = 6.0f;
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

void InitCables(WORLDINFO_s *world) {
    memset(cables, 0, sizeof(cables));
    targetmomf = default_targetmomf;
    ownermomf = default_ownermomf;
    max_cable_length = default_max_cable_length;
    tow_length = default_tow_length;
    if (GUNSHIP_ADATA != NULL && world != NULL && GUNSHIP_ADATA == world->area) {
        targetmomf = 4.0f;
        max_cable_length = 10.0f;
        tow_length = 0.0f;
    }
}

CABLE_s *CreateCable(GameObject_s *source, GameObject_s *target, i32 sound) {
    if (target == NULL || source == NULL) {
        return NULL;
    }
    for (i32 i = 0; i < 8; ++i) {
        CABLE_s *cable = &cables[i];
        if ((cable->flags_1e9 & 1) == 0) {
            cable->flags_1e9 |= 1;
            cable->source = source;
            cable->target = target;
            if (source == player || source == player2) {
                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            }
            if (sound) {
                PlaySfx("TowCable_Fire", &source->apiobj.collision_position);
            }
            return cable;
        }
    }
    return NULL;
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

void DestroyCable(CABLE_s *cable) {
    if (cable != NULL) {
        if (cable->source != NULL && cable->source->cable == cable) {
            cable->source->cable = NULL;
        }
        memset(cable, 0, sizeof(*cable));
    }
}

void ReleaseCable(CABLE_s *cable, i32 snapped) {
    cable->flags_1e9 |= 4;
    if (cable->target != NULL) {
        cable->target->field_0xf00 &= static_cast<u8>(~0x20u);
        cable->target->field_0xf01 &= static_cast<u8>(~2u);
    }
    i32 sound = GetSfxId(snapped ? "TowCable_Snap" : "TowCable_Detach");
    GameAudio_PlaySfxById(sound, cable->points, 1, 0);
    if (cable->source != NULL) {
        if (cable->source == player || cable->source == player2) {
            GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
        }
    }
    if (cable->source != NULL && cable->source->cable == cable) {
        cable->source->cable = NULL;
        cable->source = NULL;
    }
    cable->target = NULL;
    for (i32 i = 0; i < cable->point_count; ++i) {
        cable->velocities[i].y = 0.0f;
    }
}

void UpdateCables() {
    NUVEC closest;
    for (CABLE_s *cable = cables; cable != cables + 8; ++cable) {
        if ((cable->flags_1e9 & 1) == 0) {
            continue;
        }
        NUVEC delta;
        if (cable->flags_1e9 & 4) {
            cable->total_length = 0.0f;
            i32 grounded = 1;
            for (i32 i = 0; i < cable->point_count; ++i) {
                f32 ground = GameShadow(NULL, &cable->points[i], 5.0f, -1);
                if (ground == 2000000.0f)
                    ground = cable->points[i].y;
                cable->velocities[i].x -= (cable->velocities[i].x * cable_damping) * FRAMETIME;
                cable->velocities[i].y -= FRAMETIME * cable_gravity;
                cable->velocities[i].z -= (cable_damping * cable->velocities[i].z) * FRAMETIME;
                cable->points[i].x += FRAMETIME * cable->velocities[i].x;
                cable->points[i].y += cable->velocities[i].y * FRAMETIME;
                cable->points[i].z += FRAMETIME * cable->velocities[i].z;
                if (ground > cable->points[i].y) {
                    cable->points[i].y = ground;
                    cable->velocities[i].x -= (cable->velocities[i].x * cable_ground_damping) * FRAMETIME;
                    cable->velocities[i].z -= (cable_ground_damping * cable->velocities[i].z) * FRAMETIME;
                } else {
                    grounded = 0;
                }
                if (i < cable->point_count - 1) {
                    f32 length = NuVecDist(&cable->points[i + 1], &cable->points[i], &delta);
                    cable->segment_lengths[i] = length;
                    cable->total_length = length + cable->total_length;
                }
            }
            if (__builtin_expect(!!(grounded), 0))
                DestroyCable(cable);
            continue;
        }

        GameObject_s *target = cable->target;
        i32 lost_target = target != NULL && target->apiobj.field_0x218 < -100.0f;
        GameObject_s *source = cable->source;
        if (source == NULL || (source->apiobj.field_0x1f8 & 0x1000) == 0 || source->apiobj.field_0x287 != 0 ||
            (source->field_0xe20 & 0x20) || (target != NULL && target->character_context == 0x17) ||
            ((cable->flags_1e9 & 2) && source->pad_gamepad->pad != NULL &&
             (source->pad_gamepad->pad->digital_buttons_pressed & GAMEPAD_SPECIAL))) {
            ReleaseCable(cable, 0);
            continue;
        }
        cable->flags_1e9 |= 2;
        NUVEC path[16];
        path[0] = source->apiobj.collision_position;
        if (target == NULL || (target->apiobj.field_0x1f8 & 0x1000) == 0 || target->apiobj.field_0x287 != 0) {
            ReleaseCable(cable, 0);
            continue;
        }
        if (target->id != id_ATAT) {
            if (__builtin_expect(target->id == id_DRAGBOMB, 0)) {
                target->character_context = 0x34;
                cable->target->context_animation_timer = 0.0f;
            }
        } else {
            if (cable->wrap_count == 0) {
                f32 nearest = NuVecDistSqr(
                    &path[0], NUMTX_GET_ROW_VEC(&cable->target->joint_matrices[atat_locators[0]], 3), &delta);
                if (!(nearest < 1000000000.0f))
                    nearest = 1000000000.0f;
                i32 selected = 0;
                for (i32 i = 1; i < 4; ++i) {
                    f32 distance = NuVecDistSqr(
                        &path[0], NUMTX_GET_ROW_VEC(&cable->target->joint_matrices[atat_locators[i]], 3), &delta);
                    if (nearest > distance) {
                        nearest = distance;
                        selected = i;
                    }
                }
                // Distance callbacks can replace the target; geometry uses the resulting target snapshot.
                cable->wrap_indices[cable->wrap_count++] = static_cast<u8>(selected);
                target = cable->target;
            }
            i32 last = cable->wrap_indices[cable->wrap_count - 1];
            NUVEC *last_position = NUMTX_GET_ROW_VEC(&target->joint_matrices[atat_locators[last]], 3);
            if (!(cable->wrap_count > 1)) {
                f32 along0, along1;
                i32 next = (last + 1) & 3;
                i32 previous = (last + 3) & 3;
                // Preserve the reverse-edge locators across the first intersection callback.
                i32 previous_locator = atat_locators[previous];
                i32 opposite_locator = atat_locators[(previous + 3) & 3];
                if (!(XZLinesIntersect(&path[0], last_position,
                                       NUMTX_GET_ROW_VEC(&target->joint_matrices[atat_locators[next]], 3),
                                       NUMTX_GET_ROW_VEC(&target->joint_matrices[atat_locators[(last + 2) & 3]], 3),
                                       &along0, &along1))) {
                    if (XZLinesIntersect(
                            &path[0], last_position, NUMTX_GET_ROW_VEC(&target->joint_matrices[previous_locator], 3),
                            NUMTX_GET_ROW_VEC(&target->joint_matrices[opposite_locator], 3), &along0, &along1)) {
                        cable->wrap_indices[cable->wrap_count++] = static_cast<u8>(previous);
                    }
                } else {
                    cable->wrap_indices[cable->wrap_count++] = static_cast<u8>(next);
                }
            } else {
                i32 previous = cable->wrap_indices[cable->wrap_count - 2];
                i32 next = (last + 1) & 3;
                i32 direction = 1;
                if (next == previous) {
                    next = (last + 3) & 3;
                    direction = -1;
                }
                i32 next_locator = atat_locators[next];
                i32 previous_locator = atat_locators[previous];
                NuVecSub(&delta, &path[0], last_position);
                i32 angle = NuAtan2D(delta.x, delta.z);
                NuVecSub(&delta, NUMTX_GET_ROW_VEC(&target->joint_matrices[next_locator], 3), last_position);
                i32 next_angle = NuAtan2D(delta.x, delta.z);
                i32 turn = NuAngSub(next_angle, angle);
                if (cable->wrap_count < 15 && turn * direction > 0) {
                    cable->wrap_indices[cable->wrap_count++] = static_cast<u8>(next);
                    if (cable->wrap_count == 15) {
                        DeactivatePlayer(cable->target, WORLD->current_level == HOTHBATTLED_LDATA ? 60.0f : 30.0f,
                                         NULL);
                        PlaySfx("AT_AT_FALL", &cable->target->apiobj.collision_position);
                        lost_target = true;
                    }
                } else {
                    NuVecSub(&delta, last_position, NUMTX_GET_ROW_VEC(&target->joint_matrices[previous_locator], 3));
                    i32 previous_angle = NuAtan2D(delta.x, delta.z);
                    if (NuAngSub(previous_angle, angle) * direction < 0) {
                        // The original clears the slot at the old count, which overlaps point_count at 15.
                        if (cable->wrap_count == 15)
                            cable->point_count = 0xff;
                        else
                            cable->wrap_indices[cable->wrap_count] = 0xff;
                        --cable->wrap_count;
                    }
                }
            }
        }
        i32 path_count;
        if (cable->wrap_count != 0) {
            path_count = 1;
            for (i32 i = cable->wrap_count - 1; i >= 0; --i) {
                i32 index = cable->wrap_indices[i];
                if (cable->target->apiobj.character_model->points_of_interest[index]) {
                    path[path_count++] = *NUMTX_GET_ROW_VEC(&cable->target->joint_matrices[atat_locators[index]], 3);
                }
            }
        } else {
            path_count = 2;
            path[1] = cable->target->apiobj.collision_position;
        }
        if (lost_target) {
            target = cable->target;
            ReleaseCable(cable, 0);
            continue;
        }
        if ((cable->source->apiobj.character_data->game_character->flags_090 & 0x400) == 0) {
            target = cable->target;
            ReleaseCable(cable, 0);
            continue;
        }
        f32 distance_sq = NuVecDistSqr(&path[0], &path[1], &delta);
        if (GameRayCast(&path[1], &delta, 0.0f, TERRAINMASK_NONDROID | 0x1f)) {
            target = cable->target;
            ReleaseCable(cable, 1);
            continue;
        }
        i32 obstruction = 0;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
            GameObject_s *object = &Obj[i];
            if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
                (object->field_0xe20 & 0x20) || object->apiobj.player_controlled)
                continue;
            if (WORLD->area != NULL && WORLD->area == HOTHBATTLE_ADATA && object->id != id_ATAT &&
                object->id != id_DRAGBOMB && object->id != id_ATST && object->id != id_ATST_LOWRES)
                continue;
            if (object->cable != NULL &&
                (object->cable->target == cable->target || object->cable->target == cable->source))
                continue;
            if (!(object == cable->source || object == cable->target)) {

                f32 line_distance =
                    NuLineToPointDistSqrEx(&path[0], &path[1], &object->apiobj.collision_position, &closest);
                f32 radius = object->apiobj.field_0x1dc;
                f32 half_height = object->apiobj.field_0x1e0;
                f32 combined = radius + half_height;
                if (combined * combined > line_distance &&
                    object->apiobj.collision_position.y + half_height > closest.y &&
                    closest.y > object->apiobj.collision_position.y - half_height) {
                    f32 dz = object->apiobj.collision_position.z - closest.z;
                    f32 dx = object->apiobj.collision_position.x - closest.x;
                    if (radius * radius > dx * dx + dz * dz) {
                        obstruction = 1;
                        break;
                    }
                }
            }
        }
        if (obstruction) {
            target = cable->target;
            ReleaseCable(cable, 1);
            continue;
        }
        target = cable->target;
        if (target != NULL && (target->apiobj.field_0x1f8 & 0x1000)) {
            if (target->id != id_ATAT) {
                target->field_0xf01 |= 2;
                AIScriptSetBaseScriptStateByName(&cable->target->ai.script_process, "BeingTowed");
                cable->target->ai.opponent = cable->source;
                FaceOpponent(cable->target, &cable->source->apiobj.collision_position);
                target = cable->target;
                if ((target->field_0xefd & 0x80) && !(CInfo[static_cast<i8>(target->character_context)].flags & 1))
                    target->apiobj.movement_facing_angle += 0x8000;
                target->ai.goal_speed_mode = 1;
                cable->target->field_0xefd |= 0x80;
            }
            if (distance_sq > tow_length * tow_length) {
                f32 maximum_sq = max_cable_length * max_cable_length;
                f32 strength = distance_sq > maximum_sq ? distance_sq / maximum_sq : 1.0f;
                NuVecScale(&delta, &delta, FRAMETIME);
                NUVEC target_impulse, source_impulse;
                NuVecScale(&target_impulse, &delta, strength * targetmomf);
                NuVecScale(&source_impulse, &delta, strength * ownermomf);
                if (cable->max_length == 1000000000.0f && cable->target->id != id_ATAT) {
                    target_impulse.y = 0.0f;
                    NuVecAdd(&cable->target->apiobj.velocity, &cable->target->apiobj.velocity, &target_impulse);
                    if (cable->total_length > tow_length) {
                        cable->pull_time += FRAMETIME;
                        cable->target->field_0xf00 |= 0x20;
                    } else {
                        cable->pull_time = 0.0f;
                    }
                    if ((cable->target->id == id_ATST || cable->target->id == id_ATST_LOWRES) &&
                        cable->pull_time > 1.0f) {
                        NuVecSub(&cable->source->apiobj.velocity, &cable->source->apiobj.velocity, &source_impulse);
                        target = cable->target;
                        ReleaseCable(cable, 0);
                        target->character_context = 0x3d;
                        target->context_animation = 0x85;
                        f32 duration = AnimDuration(static_cast<i16>(target->id), 0x85, 0, 0, 1);
                        target->context_animation_timer = 0.0f;
                        target->field_0xf02 |= 0x80;
                        target->airborne_action_duration = duration;
                        IncrementMinikitCounter(target);
                        GameObject_s *object = Obj;
                        for (i32 j = 0; j < HIGHGAMEOBJECT; ++j, ++object) {
                            if ((object->apiobj.field_0x1f8 & 0x1000) && object->apiobj.field_0x287 == 0 &&
                                object->cable != NULL && object->cable->target == target) {
                                ReleaseCable(object->cable, 1);
                            }
                        }
                        continue;
                    }
                }
                NuVecSub(&cable->source->apiobj.velocity, &cable->source->apiobj.velocity, &source_impulse);
            }
        }
        cable->total_length = 0.0f;
        if (cable->max_length == 1000000000.0f) {
            cable->point_count = static_cast<u8>(path_count);
            for (i32 i = 0; i < path_count; ++i) {
                cable->points[i] = path[i];
                if (i < path_count - 1) {
                    f32 length = NuVecDist(&path[i + 1], &path[i], &delta);
                    cable->segment_lengths[i] = length;
                    cable->total_length = length + cable->total_length;
                }
            }
        } else {
            f32 remaining = cable->max_length + FRAMETIME * cable_speed;
            cable->points[0] = path[0];
            cable->point_count = 1;
            cable->max_length = remaining;
            for (i32 i = 0; i < path_count - 1; ++i) {
                f32 length = NuVecDist(&path[i + 1], &path[i], &delta);
                if (!(remaining >= length)) {
                    cable->segment_lengths[i] = remaining;
                    cable->total_length += remaining;
                    f32 scale = NuFdiv(remaining, length);
                    NuVecScale(&delta, &delta, scale);
                    NuVecAdd(&cable->points[cable->point_count], &cable->points[cable->point_count - 1], &delta);
                    ++cable->point_count;
                    break;
                }
                remaining -= length;
                cable->segment_lengths[i] = length;
                cable->total_length += length;
                cable->points[cable->point_count++] = path[i + 1];
                if (i == path_count - 2) {
                    cable->max_length = 1000000000.0f;
                    PlaySfx("TowCable_Latch", &path[i + 1]);
                }
            }
        }
        if (cable->source != NULL && cable->target != NULL) {
            for (i32 i = 0; i < cable->point_count; ++i)
                if (i == 0)
                    cable->velocities[i] = cable->source->apiobj.velocity;
                else
                    cable->velocities[i] = cable->target->apiobj.velocity;
        }
    }
}

void DrawCables() {
    NUVEC step;
    NURND_VERTEX3D vertices[2];
    NUVEC &start = vertices[0].position;
    NUVEC &end = vertices[1].position;
    vertices[0].colour = 0xff000000;
    vertices[1].colour = 0xff000000;
    nsegments_drawn = 0;
    for (CABLE_s *current = cables; current != cables + 8; ++current) {
        CABLE_s &cable = *current;
        if ((cable.flags_1e9 & 1) == 0) {
            continue;
        }

        const f32 y_span = cable.points[cable.point_count - 1].y - cable.points[0].y;
        if ((cable.flags_1e9 & 4) != 0) {
            cable.slack += FRAMETIME * cable_slack * slack_factor;
            if (cable.slack > cable_slack) {
                cable.slack = cable_slack;
            }
        } else if (cable.total_length >= tow_length) {
            cable.slack = 0.0f;
        } else {
            f32 fraction = cable.total_length / tow_length;
            if (fraction > 1.0f) {
                fraction = 0.0f;
            } else {
                fraction = 1.0f - fraction;
            }
            cable.slack = fraction * cable_slack;
        }

        f32 distance_along = 0.0f;
        if (cable.slack == 0.0f) {
            for (i32 segment = 0; segment < cable.point_count - 1; ++segment) {
                const f32 start_fraction = NuFdiv(distance_along, cable.total_length);
                start = cable.points[segment];
                start.y = start_fraction * y_span + cable.points[0].y;
                distance_along += cable.segment_lengths[segment];
                const f32 end_fraction = NuFdiv(distance_along, cable.total_length);
                end = cable.points[segment + 1];
                end.y = y_span * end_fraction + cable.points[0].y;
                if (solid_cable != 0) {
                    DrawRopeSingle(&start, &end, 1.0f, ropemtl, end_fraction, end_fraction, 10.0f, 5.0f);
                } else {
                    NuRndrLine3d(vertices, SolidMtl3D, NULL);
                }
            }
        } else {
            const f32 subdivision_scale = cable.total_length * nsegments_per_unit;
            NUVEC endpoint = cable.points[0];
            endpoint.y = y_span * 0.0f + cable.points[0].y;
            f32 ground_start = GameShadow(NULL, &endpoint, 5.0f, -1);
            if (ground_start == 2000000.0f) {
                ground_start = endpoint.y;
            }
            for (i32 segment = 0; segment < cable.point_count - 1; ++segment) {
                NUVEC previous = endpoint;
                endpoint = cable.points[segment + 1];
                endpoint.y = NuFdiv(distance_along + cable.segment_lengths[segment], cable.total_length) * y_span +
                             cable.points[0].y;
                f32 ground_end = GameShadow(NULL, &endpoint, 5.0f, -1);
                if (ground_end == 2000000.0f) {
                    ground_end = endpoint.y;
                }

                NuVecSub(&step, &endpoint, &previous);
                const f32 segment_length = cable.segment_lengths[segment];
                const f32 subdivisions = static_cast<f32>(
                    ceil(static_cast<double>(NuFdiv(segment_length, cable.total_length) * subdivision_scale)));
                const f32 step_length = NuFdiv(segment_length, subdivisions);
                NuVecScale(&step, &step, NuFdiv(step_length, segment_length));
                const f32 half_step = 0.5f * step_length;
                NUVEC cursor = previous;
                f32 local_distance = 0.0f;
                while (local_distance + half_step < cable.segment_lengths[segment]) {
                    start = cursor;
                    const f32 global_distance = distance_along + local_distance;
                    start.y -= cable.slack *
                               NU_SIN_LUT(static_cast<i32>(NuFdiv(global_distance, cable.total_length) * 32768.0f));
                    const f32 start_fraction = NuFdiv(local_distance, cable.segment_lengths[segment]);
                    const f32 start_floor =
                        start_fraction * ground_end + (1.0f - start_fraction) * ground_start + 0.05f;
                    if (start_floor > start.y) {
                        start.y = start_floor;
                    } else {
                        start.y = start.y;
                    }
                    NuVecAdd(&cursor, &cursor, &step);
                    end = cursor;
                    const f32 phase = NuFdiv(global_distance + step_length, cable.total_length);
                    const i32 angle = !(phase > 1.0f) ? static_cast<i32>(phase * 32768.0f) : 32768;
                    local_distance += step_length;
                    const f32 end_sag = cable.slack * NU_SIN_LUT(angle);
                    end.y -= end_sag;
                    const f32 end_fraction = NuFdiv(local_distance, cable.segment_lengths[segment]);
                    const f32 end_floor = end_fraction * ground_end + (1.0f - end_fraction) * ground_start + 0.05f;
                    if (end.y < end_floor) {
                        end.y = end_floor;
                    }
                    if (solid_cable != 0) {
                        DrawRopeSingle(&start, &end, 1.0f, ropemtl, end_sag, end_sag, 10.0f, 5.0f);
                    } else {
                        NuRndrLine3d(vertices, SolidMtl3D, NULL);
                    }
                    ++nsegments_drawn;
                }
                distance_along += cable.segment_lengths[segment];
                ground_start = ground_end;
            }
        }
    }
}

GameObject_s *CableTargetGameObject(GameObject_s *source, nuvec_s *position, f32 radius) {
    f32 nearest_distance = 100000000.0f;
    GameObject_s *nearest = NULL;
    const f32 radius_squared = radius * radius;
    const f32 minimum_x = position->x - radius;
    const f32 minimum_z = position->z - radius;
    const f32 maximum_x = position->x + radius;
    const f32 maximum_z = position->z + radius;

    GameObject_s *candidate = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++candidate) {
        if (candidate == source || (candidate->apiobj.field_0x1f8 & 0x1000) == 0 ||
            candidate->apiobj.field_0x287 != 0 || candidate == Player[0] || candidate == Player[1]) {
            continue;
        }
        const i8 context = candidate->character_context;
        if ((CInfo[context].flags & 0x8000) != 0 || context == 0x17) {
            continue;
        }
        if (!(context == 0x3b || context == 0x41 || context == 0x3d)) {
            if (candidate->id != id_DRAGBOMB && candidate->id != id_ATAT && candidate->id != id_ATST &&
                candidate->id != id_ATST_LOWRES) {
                continue;
            }
            if (candidate->apiobj.collision_position.x < minimum_x ||
                candidate->apiobj.collision_position.x > maximum_x ||
                candidate->apiobj.collision_position.z < minimum_z ||
                candidate->apiobj.collision_position.z > maximum_z) {
                continue;
            }

            NUVEC delta;
            f32 distance = NuVecDistSqr(&candidate->apiobj.collision_position, position, &delta);
            if (candidate->id == id_DRAGBOMB) {
                distance *= 0.8f;
            } else if (candidate->id == id_ATAT) {
                distance *= 0.9f;
            }
            if (!(distance < radius_squared)) {
                continue;
            }
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest = candidate;
            }
        }
    }
    return nearest;
}

