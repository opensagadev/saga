// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "globals.h"

static void Pup_CollectCharKit(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32);

static void EndRedBrickMessage(GAMEMESSAGE_s *);

static void Pup_CollectRedBrick(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32);

static void Pup_UpdatePurpleCoin(WORLDINFO_s *, GIZMOPICKUP_s *pickup);

static void Pup_UpdateBlueCoin(WORLDINFO_s *, GIZMOPICKUP_s *pickup);

static void Pup_UpdatePowerUp(WORLDINFO_s *world, GIZMOPICKUP_s *pickup);

static void Pup_CollectPowerUp(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 flags);

void Pup_CollectCoin(WORLDINFO_s *world, GIZMOPICKUP_s *pickup, i32 type, GameObject_s *object, i32 arg);

static void Pup_CollectMinikit(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 type);

static void Pup_CollectHeart(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 flags);

u8 CoinTab[4] = {0, 1, 2, 3};

GIZMO_PICKUP_TYPE GizmoPickupType[10] = {
    {"Silver Coin", NULL, NULL, 's', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00b7, 10, 0.045f, 0.045f, 10.0f, 0.0f, NULL,
     Pup_CollectCoin, -1, -1, 0.0f, 0.0f},
    {"Gold Coin", NULL, NULL, 'g', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00bf, 100, 0.045f, 0.045f, 10.0f, 0.0f, NULL,
     Pup_CollectCoin, -1, -1, 0.0f, 0.0f},
    {"Blue Coin", "PickupCoinB", NULL, 'b', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00c7, 1000, 0.045f, 0.045f, 6.0f, 0.0f,
     Pup_UpdateBlueCoin, Pup_CollectCoin, 8, -1, 0.0f, 0.0f},
    {"Purple Coin", "PickupCoinB", NULL, 'p', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00d5, 10000, 0.045f, 0.045f, 6.0f, 0.0f,
     Pup_UpdatePurpleCoin, Pup_CollectCoin, 8, -1, 0.0f, 0.0f},
    {"Minikit", "MK-Appear", NULL, 'm', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_MINIKIT_DETECTOR | GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00ce, 0,
     0.125f, 0.15f, 2.0f, 0.05f, NULL, Pup_CollectMinikit, 20, -1, 0.0f, 0.0f},
    {"Heart", NULL, NULL, 'h', 0, GIZMOPICKUP_TYPE_DRAW_Y_ROTATION, 0, 0x00cb, 0, 0.1f, 0.08f, 4.0f, 0.0f, NULL,
     Pup_CollectHeart, -1, -1, 0.0f, 0.0f},
    {"Red Brick", "MK-Appear", NULL, 'r', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_RED_BRICK_DETECTOR | GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00d2, 0,
     0.12f, 0.06f, 2.0f, 0.1f, NULL, Pup_CollectRedBrick, 99, -1, 0.0f, 0.0f},
    {"Charkit", "MK-Appear", "MK-Pickup", 'c', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_CHALLENGE_MODE_FILTER, 0, 0x00cf, 0, 0.125f, 0.15f, 2.0f, 0.05f,
     NULL, Pup_CollectCharKit, 20, -1, 0.0f, 0.0f},
    {"Torpedo", NULL, NULL, 't', 0, GIZMOPICKUP_TYPE_DRAW_Y_ROTATION, 0, 0x0079, 0, 0.1f, 0.1f, 5.0f, 0.0f, NULL, NULL,
     -1, -1, 0.0f, 0.0f},
    {"Power Up", "MK-Appear", NULL, 'u', 0, GIZMOPICKUP_TYPE_DRAW_TUMBLING, 0, 0x00d0, 0, 0.125f, 0.125f, 2.0f, 0.05f,
     Pup_UpdatePowerUp, Pup_CollectPowerUp, -1, 0x00d1, 0.0f, 0.0f},
};

GIZMOPICKUPSYS_s GizmoPickupSys_Game = {
    GizmoPickupType, CoinTab, 10, 4, 4, 2, 0, 0,
};

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

BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
void PartCollide_3D(PART_s *);
extern "C" f32 sabrerubwait;
extern i32 lightning_type;
extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;
extern i32 VehicleArea;

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).
extern void *theThingManager;

void legoSetMusicVolume(float);
void AnimatePlayer(GameObject_s *object);
void InitPlayerAI(GameObject_s *object);
void InitSurfaceInfo(GameObject_s *object);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
void GameAISysSetGame();
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);
void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process);
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world);
void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index);

extern i32 LEGO_AIPATHCNX_BLOCKAGE;

extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

extern GAMEFOG_STATE GameFog;

#include "legoapi/items/objects/gameobjects_types.h"

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

extern "C" f32 NuAnimEndFrameOld(void *animation);

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

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

extern i32 TimingBarSet;
extern "C" i32 SHADOWCALLS;

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask);

extern "C" f32 MainRenderTime;
extern "C" f32 MainRenderTargetTime;
extern i32 Paused;
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

extern "C" i32 DoubleScore;
extern FadeSystem FadeSys;
extern i32 pause_rndr_on;
extern "C" TIMER BonusTimer;
extern "C" i32 screendump;

void GameTiming(WORLDINFO_s *world, float *game_time);

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);
extern "C" i32 RAYCASTCALLS;

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask);

extern i32 active_neutral_count;
extern "C" i32 party_cant_be_under_cover;
extern AREADATA *VADER_ADATA;
extern i32 active_baddy_count;

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.

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

}

extern f32 OFFSCREEN_CATCHUP_TIME;
extern f32 drop_back_in_timer;

i32 TryToTeleportToNextNode(GameObject_s *object, AIPATHNODE_s *node, i32 tightrope);

void SetSpecialMove(GameObject_s *object, AIPATHNODE_s *target, AIPATHNODE_s *node, char move);

void ClearSpecialMove(GameObject_s *object);

void GameAISysInit();

void GameAISysReset(AISYS_s *system);

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

void GameAnimSys_Update(GAMEANIMSYS_s *system);

void FindAnglesZX(NUVEC *, u16 *, u16 *);
u16 SeekRot(u16, u16, f32);
i32 RotDiff(u16, u16);
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

f32 GameGetMusicVolume(OPTIONSSAVE_s *options);

f32 GameSetMusicVolume(OPTIONSSAVE_s *options);

void GameAISysStartFrame(AISYS_s *system);

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours);

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object);

i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);

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

i32 NoLayerKill(GameObject_s *object);

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only);

void DieRumble(GameObject_s *);

void ConstantRumble(GameObject_s *object, f32 strength, f32 phase);

extern char *ASCII_DOWN;

void RemoveGameObject(GameObject_s *obj, i32);

extern i32 nethost;
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

extern "C" i32 adaptivedifficulty[3];
extern i8 (*adtab)[4];
extern f32 lightning_sizew[2];
extern f32 lightning_sizel[2];
extern f32 lightning_sizewab[2];
extern f32 lightning_endw[2];
extern u32 lightning_col[2];
extern AREADATA *PODSPRINT_ADATA;
extern i16 id_ANAKINSNEWPOD, id_ANAKINSNEWPODGREEN, id_SEBULBASPOD;
extern "C" f32 tieonsfxwait, tieoffsfxwait;
void UpdateRumble(RUMBLEPACKET *);
void Player_ToggleCharacter(GameObject_s *, i32, i32);

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set);

GameObject_s *GetNamedGameObject(AISYS_s *system, char *name);

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance);

void GameObjectToCameraDistances();

// ==== src/legoapi/render/fx/parts.cpp: declarations ====
#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/motion/gameanim.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

ADDPART_s Default_ADDPART = {NULL,
                             0,
                             NULL,
                             0,
                             0,
                             0.1f,
                             0.1f,
                             -5.0f,
                             0.75f,
                             NULL,
                             -1,
                             0,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             -1,
                             NULL,
                             -1,
                             -1,
                             60.0f,
                             60.0f,
                             -1,
                             -1,
                             -1,
                             1.0f,
                             0,
                             -1,
                             2000000.0f,
                             1.0f / 60.0f,
                             -1,
                             0,
                             1.0f,
                             1.0f,
                             1.0f,
                             0.0f,
                             {0, 0, 0, 0, 0, 0},
                             1.0f,
                             0,
                             {0, 0, 0}};

struct rtlset;

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.

    extern debinftype **debtab;
    f32 CameraEmitterDistance(NUVEC *);
    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);
    void AddFiniteShotDebrisEffect2(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32);
    void AddVariableShotDebrisEffectMtx3(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *);
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;

    i32 NuCameraClipTestExtentsAxisAligned(NUVEC *, NUVEC *, f32);
    void NuVecAddScale(NUVEC *, NUVEC *, NUVEC *, f32);
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

extern f32 AreaPickupGravity;
extern i16 temp_xrot;
extern i16 temp_zrot;

// Forward declarations for local (static) part/gizmo helper stubs.
struct spacelevel_s;

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern GameObject_s *Obj;
extern i32 HIGHGAMEOBJECT, VehicleArea;
extern WORLDINFO_s *WORLD;
extern GAMECAMERA_s *GameCam;
extern "C" void KillPart(PART_s *, i32);
void SetCoinType(i32 model, GIZMOPICKUP_s *pickup);
u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);

extern "C" f32 coinimpactwait;
extern i32 GAMEDEMO;
extern "C" void PlaySfxAndSetVolume(char *, NUVEC *, f32);

static __used__ void PartImpact_Coin(PART_s *part);

static __used__ void PartStolen_Coin(PART_s *part);

extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
static __used__ void PartExtra_BlueCoin(PART_s *part);

static __used__ void PartExtra_PurpleCoin(PART_s *part);

void PowerUp_Particles(WORLDINFO_s *, NUVEC *);

static __used__ i32 PowerUp_DrawPart(PART_s *part);

static __used__ void PowerUp_ImpactPart(PART_s *part);

static __used__ void PowerUp_UpdatePart(PART_s *part);

static __used__ void PowerUp_EndMsg(GAMEMESSAGE_s *message);

static __used__ void PowerUp_UpdateMsg(GAMEMESSAGE_s *message);

void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;

static __used__ i32 PartDraw_VehicleHeart(PART_s *part);

static __used__ void PartMove_VehicleHeart(PART_s *part, f32);

static __used__ void PartMove_VehiclePickup(PART_s *part, f32);

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position, i32 count);

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position, NUVEC *emitter_momentum,
                                    NUVEC *particle_momentum, i32 count);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

    void KillPart(PART_s *, i32);
    i32 rtlDynamicAllocTemplate(rtlset *, i32);

PART_s *AddPart(ADDPART_s *params);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectMtx3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                         i8 flags);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed5(i32 effect_index, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 duration, NUMTX *emitter_orientation,
                                           NUMTX *particle_orientation, u16 render_priority, i8 timed_flags);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

void DrawParts(i32 keep_offscreen);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

    // The editor loader clears these page IDs with aligned vector stores.

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

i32 PARTLookupType(char *name);

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

    i32 rtlDynamicSetPos(i32, NUVEC *);

void UpdateParts(f32 time);

} // extern "C"

PART_s *TargetPart(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius, f32 range_squared,
                   i32 directional, i32 bolt_id);

extern "C" i32 NuGScnNumSpecials(nugscn_s *);

extern "C" {
extern part_typedesc_s * edpart_nearest_type;
}

extern "C" {

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}

extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

void InitPartTable(char **names);

void LoadPartFile(WORLDINFO *world);

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position);

i32 SetPartTarget(GameObject_s *object, PART_s *target);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartCollide_2D(PART_s *);
void PartUpdate_Heart(PART_s *);
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);

void AddTorpedoAsPart(nuvec_s *position, nuvec_s *velocity, float scale, float lifetime);

void AddHeartAsPart(GameObject_s *recipient, nuvec_s *position, nuvec_s *velocity, float scale, float lifetime);

void PowerUp_AddPart(NUVEC *position, NUVEC *velocity, f32 scale, f32 lifetime);

void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);

i32 FindPartDebris(PARTDEBSYS_s *system, char *name);

void MakePartVector(NUVEC *velocity, NUVEC *direction, f32 scale);

void PartCollide_2D(PART_s *part);

void PartCollide_3D(PART_s *part);

void PartStop_Coin(PART_s *part);

void AddCoinsAsParts(i32 type_id, nuvec_s *position, nuvec_s *velocity, float lifetime, float scale);

// The original repeats the per-sound emitter checks for each of the four sound slots.
#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \
    if (type->sounds[slot] != -1 && type->sound_modes[slot] == (mode))                                                 \
        PlaySfxById(type->sounds[slot], &emitter->position);

extern "C" f32 brickimpactwait;
extern "C" i32 GetSfxId(const char *);

void PartImpact_Brick(PART_s *part);

void PartUpdate_Heart(PART_s *part);

void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32);

// ==== src/legoapi/menus/core/gamemessages.cpp: declarations ====
#include <cstdio>

#include "decomp.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/store.h"
#include "globals.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern void IncreaseScore(u32 *, u64, i32);
extern void SetBonusWinner(i32);
extern void Arcade_CoinCollected(i32, u32 *, u32);
extern void PlayRepeatSfx(char *, i32, f32, char, f32, nuvec_s *);
void ResetGameMessages();

#include "legoapi/menus/core/gamemessages_types.h"

static_assert(sizeof(GAME_MESSAGE_DATA) == sizeof(GAMEMESSAGE_s), "game message backing layout");
static_assert(offsetof(GAME_MESSAGE_DATA, tick_fn) == offsetof(GAMEMESSAGE_s, tick_fn),
              "game message backing callback offset");

extern ADDGAMEMSG AddGameMsg_Default;

GAMEMESSAGE_s *AddGameMsg(ADDGAMEMSG *message);

void AddFancyMessage(char *text, float x, float y, float scale, float duration, i32 message_id, i32 colour_type);

void EndScoreMessage(GAMEMESSAGE_s *message);

i32 FindGameMsgsWithID(i32 id, i32 remove, i32 player, GAMEMESSAGE_s *exclude);

void AddGameMsgCount(nuvec_s *position, i32 count, i32 total, unsigned char red, unsigned char green,
                     unsigned char blue, float field_0xd4);

void TransformGameMessages(nuvec_s *camera_position, nuvec_s *camera_scale, nuvec_s *camera_direction);

// ==== src/legoapi/gizmo/gizmos/gizmos_gizmopickups.cpp: all remaining content ====
#include "decomp.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"

void NewBuzz(nupad_s *pad, f32 amount, i32 flags);

void GizmoPickup_CollectCoin(WORLDINFO_s *world, nuvec_s *position, i32 type_index, i32 model_variant,
                             GameObject_s *object, i32);

// ==== src/legoapi/items/collect/minikits.cpp: declarations ====
#include "decomp.h"
#include "legoapi/items/collect/minikits.h"
#include "globals.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/light/lighting.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

extern HUBMINIKITPIECES_s ** Char_MiniKit;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;

void MiniKits_Init(variptr_u *buffer, variptr_u *);

void CharacterMiniKits_Load(COLLECTION_s *collection, WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end);

extern f32 KITPOSX, PANEL_MINIKITY, PANEL_MINIKITSCALE;
void MiniKit_GameMsg_Update(GAMEMESSAGE_s *message);
void MiniKit_GameMsg_End(GAMEMESSAGE_s *message);
void Hint_SetHintFromId(i32, i32, i32);

void CollectMinikit(nuvec_s *position, char *name, i32);

i32 AllMiniKitsDone(AREASAVE_s *save);

i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);

void IncreaseScore(u32 *, u64, i32);
extern "C" void PlaySfx(char *, nuvec_s *);
void DrawMiniKitCount(f32, f32, i32, i32);

void CollectAllMiniKits(AREASAVE_s *save);

void EndChallenge(i32, i32);

void ResetMinikitCounter();

extern i16 id_SLAVE1, tALLMINIKITSBUILT;

void CharacterMiniKits_Dump(WORLDINFO_s *world);

void IncrementMinikitCounter(GameObject_s *);

// ==== src/legoapi/items/base/collection.cpp: declarations ====
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"

u32 GizmoBlowups_TotalScore(void *world);

#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamehint.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>
#include <stdlib.h>
#include "legoapi/characters/motion.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
struct GIZMOPICKUP_s;
struct PART_s;
struct starfighter_s;

struct APICHARACTERMODELLIST_s;

extern i32 COLLECTION_COMPLETIONCOUNT;

i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count);

i32 Collection_Got(i32 id);

void Collection_Configure(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd);

extern f32 COLLECTION_DX;
extern f32 COLLECTION_DY;
extern f32 COLLECTION_ICONSIZE;
extern i32 LEGOOBJ_ICON_FRAME_NEUTRAL;
extern i32 LEGOOBJ_ICON_FRAME_BLUE;
extern i32 LEGOOBJ_ICON_FRAME_GREEN;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);

i32 Collection_GetIDList(COLLECTION_s *collection, u32 model_flag_mask, u32 required_model_flags, i16 *ids,
                         i32 *first_id, i32 *second_id, i32 unused);

void Collection_CreateMaster(char *file, i16 *idlist, COLLECTION_s *collection, i32 param4, float param5);

void Collection_CreateCustom(char *name, i16 *id_list, COLLECTION_s *collection, u32 required_model_flags,
                             u32 excluded_model_flags, u32 required_game_flags, i32 require_buyable, i32 columns,
                             VARIPTR *buffer, VARIPTR *, i32 use_all_characters, f32 scale);

COLLECTID *CollectIDUnlocked(i32 id);

i32 Collection_GotAnyOfType(i32 type, u32 flags);

void CollectAllCharacters(i32 only_story);

u32 GizmoBlowups_TotalScore(void *);
u32 GizForce_TotalScore(void *);
u32 GizObstacles_TotalScore(void *);
u32 GizTurrets_TotalScore(void *);

u32 TotalLevelCoinTally(WORLDINFO_s *world, u32 *pickups, u32 *blowups, u32 *buildits, u32 *forces, u32 *obstacles,
                        u32 *turrets, u32 *characters);

COLLECTION_s *GetFreePlayCollection(i32 area);

extern i32 SHOPHINTCOUNT;

void ReCalculateCompletionPoints();

// ==== src/legoapi/characters/core/playeritems.cpp: declarations ====
#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/nuvec.h"

void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);

i32 ReleaseHearts();

extern "C" i32 ParticlesPerSecond(f32 rate, f32 elapsed);

// ==== functions in original address order ====

// ==== src/legoapi/gizmo/object/gizmopickups.cpp: all remaining content ====
#include "decomp.h"
#include "batman.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nuvec.h"

void MakePartVector(NUVEC *, NUVEC *, f32);
void AddCoinsAsParts(i32, NUVEC *, NUVEC *, f32, f32);
void AddHeartAsPart(GameObject_s *, NUVEC *, NUVEC *, f32, f32);
void AddTorpedoAsPart(NUVEC *, NUVEC *, f32, f32);
void PowerUp_AddPart(NUVEC *, NUVEC *, f32, f32);
i32 GetRandomCoinType();

void AddCoinsToPanel(i32 coins, nuvec_s *position, i32 player, float, GameObject_s *, i32 random_types);

void AddPickups(i32 coins, i32 hearts, i32 torpedoes, i32 powerups, nuvec_s *position, nuvec_s *direction, float,
                i32 player_id, float speed, float lifetime, GameObject_s *owner, i32, i32 panel, bool consolidate);

void AddMiscPickups(nuvec_s *position, i32 player_id, i32 coins, i32 torpedoes);

i32 IsACoinType(i32 type);

i32 LoseCoins(GameObject_s *object, i32 cause);

void GizmoPickups_SetOnOff();

// ==== functions in original address order ====

void GameMsg_DrawAdjustNewPos_CoinToTotal(GAMEMESSAGE_s *message) {
    message->target_position.x = cointotal_x[message->player_index];
}

static void Pup_CollectCharKit(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32) {
    NewBuzz(object->pad_gamepad->pad, 0.2f, 0);
    AddGameDebris(WORLD->debris_sys, 0x13, &pickup->position);
    if (WORLD->lev_objs[0xcf].active == 0)
        return;
    NUVEC target = {KITPOSX, KITPOSY + PANEL_MINIKITY, 1.0f};
    ADDGAMEMSG message = AddGameMsg_Default;
    message.position = &pickup->position;
    message.scale = AreaPickupScale;
    message.target_position = &target;
    message.target_scale = PANEL_MINIKITSCALE;
    message.flags = 0x2112d;
    message.duration = 1.0f;
    message.icon = 0xcf;
    message.special = &WORLD->lev_objs[0xcf].special;
    message.tick_fn = MiniKit_GameMsg_Update;
    message.end_fn = MiniKit_GameMsg_End;
    message.field_0x4d = 1;
    GAMEMESSAGE_s *queued = AddGameMsg(&message);
    if (queued != NULL)
        queued->target_type = 6;
    DrawMiniKitTime = 2.0f;
    if (AreaGlobals.values.field_0x1c < 10)
        ++AreaGlobals.values.field_0x1c;
}

static void EndRedBrickMessage(GAMEMESSAGE_s *) {
    if (AreaGlobals.values.field_0x08 != 0)
        AreaGlobals.values.field_0x08 = 2;
    RedBrickScale = 2.0f;
    GameAudio_PlaySfx(0x26, NULL, 0, 0);
    NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
    GameCam_Judder(GameCam, -0.2f, 0, NULL);
}

static __used__ void PartMove_VehiclePickup(PART_s *part, f32) {
    f32 gain;
    if (part->field_104 == 0.0f) {
        gain = 0.0f;
    } else if (part->scale_time != 0.0f) {
        gain = 1.0f - part->scale_time / part->field_104;
    } else {
        gain = 1.0f;
    }
    if (gain <= 0.0f) {
        part->active |= 2;
        if (part->stop_callback != NULL)
            part->stop_callback(part);
        return;
    }
    GameObject_s *object =
        FindNearestGameObject(&part->position, NULL, 0, 0.0f, 0.0f, -1, -1, 99, NULL, 0, NULL, false);
    if (object != NULL)
        part->velocity.y = SeekValF(part->velocity.y, (object->apiobj.position.y - part->position.y) * 3.0f, 3.0f);
    part->position.x += part->velocity.x * gain * FRAMETIME;
    part->position.y += part->velocity.y * gain * FRAMETIME;
    part->position.z += part->velocity.z * gain * FRAMETIME;
}

static __used__ void PowerUp_ImpactPart(PART_s *part) {
    f32 scale = AreaPickupScale;
    f32 maximum = 2.5f * scale;
    f32 speed = NuVecMag(&part->velocity);
    f32 volume;
    if (speed >= maximum) {
        volume = 1.0f;
    } else {
        f32 minimum = 0.1f * scale;
        if (!(speed >= minimum))
            return;
        volume = (speed - minimum) / (maximum - minimum);
    }
    if (volume > 0.0f)
        PlaySfxAndSetVolume("Gungan_BlueOrbBounce", &part->position, volume);
}

static __used__ i32 PartDraw_VehicleHeart(PART_s *part) {
    f32 scale;
    if (PODRACE_ADATA != NULL && WORLD->area == PODRACE_ADATA)
        scale = 4.0f;
    else if (BONUS_GUNSHIP_ADATA != NULL && WORLD->area == BONUS_GUNSHIP_ADATA)
        scale = 7.5f;
    else if (GUNSHIP_ADATA != NULL && WORLD->area == GUNSHIP_ADATA)
        scale = 7.5f;
    else
        scale = 5.0f;
    NUVEC scaling;
    scaling.x = (1.0f - part->field_100) * scale;
    if (scaling.x > 0.0f) {
        scaling.y = scaling.z = scaling.x;
        NUMTX_ALIGNED16 matrix = part->transform;
        NuMtxPreScale(&matrix, &scaling);
        if (NuSpecialDrawAt(&part->special, &matrix) != 0)
            part->render_flags |= 6;
    }
    return 0;
}

static __used__ void PartMove_VehicleHeart(PART_s *part, f32) {
    GameObject_s *recipient = part->recipient;
    f32 progress = 1.0f - part->field_100;
    NUVEC start;
    f32 height;
    if (PODRACE_ADATA != NULL && WORLD->area == PODRACE_ADATA) {
        NuVecAdd(&start, &recipient->apiobj.position, &part->initial_position);
        height = 2.0f;
    } else {
        start = part->initial_position;
        height = 3.0f;
    }
    part->position.x = start.x + (recipient->apiobj.position.x - start.x) * progress;
    part->position.y = start.y + (recipient->apiobj.position.y - start.y) * progress +
                       NU_SIN_LUT(static_cast<i32>(progress * 32768.0f)) * height;
    part->position.z = start.z + (recipient->apiobj.position.z - start.z) * progress;
}

void EndScoreMessage(GAMEMESSAGE_s *message) {
    GAME_MESSAGE_DATA *data = reinterpret_cast<GAME_MESSAGE_DATA *>(message);
    if (CoinsGoToMainTotal() != 0) {
        i32 icon = static_cast<i16>(data->icon);
        if (data->icon != 0xffff) {
            if (static_cast<u32>(icon - 0xb7) < 4) {
                icon -= 4;
            } else if (static_cast<u32>(icon - 0xbf) < 4) {
                icon -= 4;
            } else if (static_cast<u32>(icon - 0xc7) < 4) {
                icon -= 4;
            } else if (static_cast<u32>(icon - 0xd5) < 4) {
                icon += 4;
            }
            cointotal_i_obj[static_cast<i8>(data->field_0xfd)] = icon;
        }
        CoinTotalScale = 1.5f;

        if (SuperStory != 0) {
            IncreaseScore(&SuperStoryScore, data->score, 0);
        } else {
            WORLDINFO_s *world = WorldInfo_CurrentlyActive();
            if (world->area != NULL && (world->area->flags & 0x100) != 0) {
                if (static_cast<u32>(BonusCoinTotal) < BonusCoinTarget) {
                    IncreaseScore(reinterpret_cast<u32 *>(&BonusCoinTotal), data->score, 0);
                    if (static_cast<u32>(BonusCoinTotal) >= BonusCoinTarget) {
                        BonusCoinTotal = static_cast<i32>(BonusCoinTarget);
                        SetBonusWinner(static_cast<i8>(data->field_0xfd));
                        BonusWinFlag = 0;
                        NewMenu(32, -1, -1);
                        PlaySfx("MK-Panel", NULL);
                        ResetGameMessages();
                    }
                }
            } else {
                IncreaseScore(&Game.coins, data->score, 1);
            }
        }
    } else {
        const i8 player_index = static_cast<i8>(data->field_0xfd);
        if (static_cast<u8>(player_index) > 1 || Player[player_index] == NULL ||
            Player[player_index]->coinpacket == NULL) {
            goto play_counter_sound;
        }

        GameObject_s *object = Player[player_index];
        COINPACKET *packet = object->coinpacket;
        const u32 old_coins = packet->coins;
        if (static_cast<i16>(data->icon) != -1) {
            packet->lastcoin = data->icon;
        }

        if (BonusWinner == -1) {
            IncreaseScore(&packet->coins, data->score, 1);
            packet = object->coinpacket;
            packet->scale = 1.5f;
        }
        if (BuildUpDone == 0) {
            BuildUpScale = 1.5f;
        }
        if (Arcade != 0) {
            Arcade_CoinCollected(static_cast<i8>(data->field_0xfd), &packet->coins, old_coins);
            goto play_counter_sound;
        }
        if (BonusArea == 0 || BonusWinner != -1 || packet->coins <= 999999) {
            goto play_counter_sound;
        }

        packet->coins = 1000000;
        SetBonusWinner(static_cast<i8>(data->field_0xfd));
        BonusWinFlag = 0;
        NewMenu(31, -1, -1);
        PlaySfx("MK-Panel", NULL);
        ResetGameMessages();
    }

play_counter_sound:
    if (DoubleScoreTime > 0.0f) {
        PlayRepeatSfx("Counter", -1, 0.0f, 2, 0.1f, NULL);
    } else {
        PlaySfx("Counter", NULL);
    }
}

static __used__ void PowerUp_UpdateMsg(GAMEMESSAGE_s *message) {
    message->rotation_y += 16384.0f * FRAMETIME;
    if (static_cast<u8>(message->player_index) <= 1)
        message->target_position.y = PowerUp_GetPanelY(message->player_index) + STATSPOSY;
}

static void Pup_CollectRedBrick(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32) {
    AddGameDebris(WORLD->debris_sys, 0x62, &pickup->position);
    GameAudio_PlaySfxById(GetSfxId(const_cast<char *>("MK-Pickup")), &pickup->position, 3, 0);
    if (WORLD->level_sub_id == -1 || Game.area_save[WORLD->level_sub_id].red_brick_collected == 0) {
        NUVEC target = {REDBRICKPOSX, REDBRICKPOSY, 1.0f};
        ADDGAMEMSG message = AddGameMsg_Default;
        message.position = &pickup->position;
        message.target_position = &target;
        message.scale = AreaPickupScale;
        message.target_scale = PANEL_REDBRICKSCALE;
        message.flags = 0x2112d;
        message.duration = 1.0f;
        message.icon = 0xd2;
        message.special = &WORLD->lev_objs[0xd2].special;
        message.end_fn = EndRedBrickMessage;
        message.tick_fn = MiniKit_GameMsg_Update;
        message.field_0x4d = 1;
        AddGameMsg(&message);
        DrawRedBrickTime = 2.0f;
        if (WORLD->level_sub_id != -1)
            AreaGlobals.values.field_0x08 = 1;
    }
    NewBuzz(object->pad_gamepad->pad, 0.2f, 0);
}

static void Pup_UpdatePurpleCoin(WORLDINFO_s *, GIZMOPICKUP_s *pickup) {
    if ((pickup->state_flags & 0x30) != 0x30) {
        return;
    }
    i32 effect = WORLD->debris_sys->entries[57].effect;
    if (effect == -1) {
        return;
    }
    f32 rate = 5.0f;
    if (WORLD->area != NULL && (WORLD->area->flags & 0x104) == 4) {
        rate = 2.5f;
    }
    i32 count = ParticlesPerSecond(rate, FRAMETIME);
    if (count > 0) {
        AddVariableShotDebrisEffect(effect, &pickup->position, count, 0, 0);
    }
}

static __used__ void PartImpact_Coin(PART_s *part) {
    if (coinimpactwait <= 0.0f) {
        f32 speed = NuVecMag(&part->velocity);
        if (speed > 0.0f) {
            i32 angle;
            if (speed > 1.5f)
                angle = 0x4000;
            else
                angle = static_cast<i32>((1.5f - speed) / 1.5f * 16384.0f + 16384.0f);
            f32 volume = NU_SIN_LUT(angle);
            if (GAMEDEMO != 0) {
                PlaySfxAndSetVolume("PickupCoin", &part->position, volume);
                coinimpactwait = 0.1f;
            } else if (static_cast<u16>(part->pickup_type - 0xb7) <= 3) {
                PlaySfxAndSetVolume("CoinDropS", &part->position, volume);
            } else {
                PlaySfxAndSetVolume("CoinDropG", &part->position, volume);
            }
        }
    }
}

static __used__ i32 PowerUp_DrawPart(PART_s *part) {
    i32 draw = PartDraw_Flickerer(part);
    if (draw && WORLD->lev_objs[0xd1].active != 0) {
        NUMTX_ALIGNED16 matrix = part->transform;
        NuSpecialDrawAt(&WORLD->lev_objs[0xd1].special, &matrix);
    }
    return draw;
}

static __used__ void PartExtra_BlueCoin(PART_s *part) {
    if (__builtin_expect((part->render_flags & 2) != 0, 0)) {
        i32 effect = WORLD->debris_sys->entries[56].effect;
        if (effect == -1)
            return;
        f32 rate;
        if (WORLD->area != NULL) {
            rate = (WORLD->area->flags & 0x104) == 4 ? 2.5f : 5.0f;
        } else {
            rate = 5.0f;
        }
        i32 count = ParticlesPerSecond(rate, FRAMETIME);
        if (count > 0)
            AddVariableShotDebrisEffect(effect, &part->position, count, 0, 0);
    }
}

static __used__ void PartExtra_PurpleCoin(PART_s *part) {
    if (__builtin_expect((part->render_flags & 2) != 0, 0)) {
        i32 effect = WORLD->debris_sys->entries[57].effect;
        if (effect == -1)
            return;
        f32 rate;
        if (WORLD->area != NULL) {
            rate = (WORLD->area->flags & 0x104) == 4 ? 2.5f : 5.0f;
        } else {
            rate = 5.0f;
        }
        i32 count = ParticlesPerSecond(rate, FRAMETIME);
        if (count > 0)
            AddVariableShotDebrisEffect(effect, &part->position, count, 0, 0);
    }
}

static void Pup_UpdateBlueCoin(WORLDINFO_s *, GIZMOPICKUP_s *pickup) {
    if ((pickup->state_flags & 0x30) != 0x30) {
        return;
    }
    i32 effect = WORLD->debris_sys->entries[56].effect;
    if (effect == -1) {
        return;
    }
    f32 rate = 5.0f;
    if (WORLD->area != NULL && (WORLD->area->flags & 0x104) == 4) {
        rate = 2.5f;
    }
    i32 count = ParticlesPerSecond(rate, FRAMETIME);
    if (count > 0) {
        AddVariableShotDebrisEffect(effect, &pickup->position, count, 0, 0);
    }
}

static __used__ void PowerUp_EndMsg(GAMEMESSAGE_s *message) {
    GameAudio_PlaySfx(0x26, NULL, 0, 0);
    if (__builtin_expect(FindGameMsgsWithID(7, 0, message->player_index, message) != 0, 0)) {
        NUVEC position;
        const i32 player = message->player_index;
        position.x = player != 0 ? ICONX : -ICONX;
        position.y = PowerUp_GetPanelY(player) + STATSPOSY;
        position.z = 1.0f;
        ADDGAMEMSG_ALIGNED16 add = AddGameMsg_Default;
        add.icon = 0x35;
        add.special = &WORLD->lev_objs[0x35].special;
        add.flags = 0x20;
        add.position = &position;
        add.scale = 0.6f;
        add.target_scale = 0.0f;
        add.duration = 0.3f;
        AddGameMsg(&add);
    }
}

void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position) {
    if (VehicleArea == 0 && ParticlesPerSecond(10.0f, FRAMETIME) > 0)
        AddGameDebris(world->debris_sys, GizBuilditGDeb[qrand() / 10923], position);
}

static void Pup_UpdatePowerUp(WORLDINFO_s *world, GIZMOPICKUP_s *pickup) {
    PowerUp_Particles(world, &pickup->position);
    PlaySfx("Grv_GuardWeaponLp", &pickup->position);
}

static __used__ void PowerUp_UpdatePart(PART_s *part) {
    NUVEC position = part->position;
    part->rotation_y = NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f;
    NuMtxSetRotationY(&part->transform, part->rotation_y);
    NuMtxTranslate(&part->transform, &position);
    PowerUp_Particles(WorldInfo_CurrentlyActive(), &position);
    PlaySfx("Grv_GuardWeaponLp", &position);
}

void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32) {
    if (object == NULL || static_cast<u8>(object->apiobj.field_0x27c) >= 2) {
        return;
    }

    if (ONEPLAYERPOWERUPS != 0) {
        const u16 camera_rotation = GameCam_GetAdjustedYRot(NULL);
        const u16 panel_rotation = rotation - RotDiff(0, camera_rotation);
        NUVEC target_position;
        target_position.x = object->apiobj.field_0x27c == 0 ? -ICONX : ICONX;
        target_position.y = PowerUp_GetPanelY(object->apiobj.field_0x27c) + STATSPOSY;
        target_position.z = 1.0f;

        ADDGAMEMSG message;
        if (object->field_0xdec > 0.0f) {
            NUVEC icon_position;
            const i32 player = object->apiobj.field_0x27c;
            icon_position.x = player == 0 ? -ICONX : ICONX;
            icon_position.y = PowerUp_GetPanelY(player) + STATSPOSY;
            icon_position.z = 1.0f;
            message = AddGameMsg_Default;
            message.extra_position = reinterpret_cast<NUVEC *>(&WORLD->lev_objs[0x35]);
            message.icon = 0x35;
            message.position = &icon_position;
            message.scale = 0.6f;
            message.target_scale = 0.0f;
            message.duration = 0.3f;
            message.flags = 0x20;
            AddGameMsg(&message);
        }

        message = AddGameMsg_Default;
        message.icon = 0xd0;
        message.extra_position = reinterpret_cast<NUVEC *>(&WORLD->lev_objs[0xd0]);
        message.position = position;
        message.field_0x4d = 1;
        message.scale = AreaPickupScale;
        message.target_position = &target_position;
        message.flags = 0x2112d;
        message.target_scale = POWERUPOBJSIZE;
        message.tick_fn = PowerUp_UpdateMsg;
        message.end_fn = PowerUp_EndMsg;
        message.field_0x4e = 7;
        message.player_index = object->apiobj.field_0x27c;
        message.duration = 1.0f;
        GAMEMESSAGE_s *game_message = AddGameMsg(&message);
        if (game_message != NULL) {
            game_message->rotation_y = panel_rotation;
            PowerUp_PanelYRot[object->apiobj.field_0x27c] = panel_rotation;
        }
    }

    Cheat_StartPowerUp(position, object);
    NewRumble(object->pad_gamepad->pad, 1.0f, 0);
    NewBuzz(object->pad_gamepad->pad, 0.2f, 0);
}

static void Pup_CollectPowerUp(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 flags) {
    CollectPowerUp(object, &pickup->position, pickup->draw_rotation, flags);
}

void PowerUp_Update(GameObject_s *object) {
    if (object->field_0xdec > 0.0f) {
        object->field_0xdec -= FRAMETIME;
        if (object->field_0xdec <= 0.0f) {
            GameAudio_PlaySfx(0x52, NULL, 0, 0);
        } else {
            GameAudio_PlaySfx(0x51, NULL, 0, 0);
            ConstantRumble(object, (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.5f, 0.0f);
        }
    }
}

void GizmoPickup_CollectCoin(WORLDINFO_s *world, nuvec_s *position, i32 type_index, i32 model_variant,
                             GameObject_s *object, i32) {
    i32 player_index;
    if (object != NULL) {
        player_index = object->apiobj.field_0x27c;
    } else {
        i32 player0_active = Player[0] == NULL ? 0 : Player[0]->apiobj.player_controlled;
        if (__builtin_expect(Player[1] == NULL || !Player[1]->apiobj.player_controlled, 0)) {
            if (player0_active == 0) {
                return;
            }
            player_index = 0;
        } else if (__builtin_expect(player0_active == 0, 0)) {
            player_index = 1;
        } else {
            player_index = qrand() / 0x8000;
        }
    }
    if (static_cast<u32>(player_index) > 1) {
        return;
    }

    if ((type_index == 2 || type_index == 3) && object != NULL) {
        if (world->area != NULL && (world->area->flags & AREAFLAG_SUPER_BONUS_AREA) == AREAFLAG_BONUS_AREA) {
            NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
        } else {
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
        }
    }

    if (DoubleScoreTime > 0.0f || type_index == 2 || type_index == 3) {
        PlaySfx(const_cast<char *>("PickupCoinB"), position);
    } else {
        PlaySfx(const_cast<char *>("PickupCoin"), position);
    }

    i32 main_total;
    NUVEC target_position;
    f32 target_scale;
    if (CoinsGoToMainTotal() != 0) {
        main_total = 1;
        target_position.x = cointotal_x[player_index];
        target_position.y = STATSPOSY;
        DrawCoinTotalTime = 1.0f + COINMSGTIME;
        target_scale = COINTOTAL_COINSIZE;
    } else {
        main_total = 0;
        target_position.x = player_index == 1 ? PANEL_COINX : -PANEL_COINX;
        target_position.y = STATSPOSY + PANEL_COINY;
        DrawBuildUpTime = 1.0f + COINMSGTIME;
        target_scale = PANEL_COINSCALE_END;
    }
    target_position.z = 1.0f;

    GIZMO_PICKUP_TYPE *type = &GizmoPickupType[type_index];
    const i32 model_id = static_cast<i16>(type->first_model_id) + model_variant;
    if (__builtin_expect(model_id != -1 && world->lev_objs[model_id].active != 0, 0)) {

        ADDGAMEMSG message = AddGameMsg_Default;
        message.target_scale = target_scale;
        u32 score = type->score;
        if (object != NULL && object->coinpacket != NULL) {
            if (((DoubleScore >> player_index) & 1) != 0) {
                score *= 2;
            }
            if (object->field_0xdec > 0.0f) {
                score *= 2;
            }
        }

        message.target_position = &target_position;
        message.position = position;
        message.scale = PANEL_COINSCALE_START * AreaPickupScale;
        message.flags = 0x2012d;
        message.duration = COINMSGTIME;
        message.icon = static_cast<i16>(model_id);
        message.extra_position = reinterpret_cast<nuvec_s *>(&world->lev_objs[model_id]);
        message.score = score;
        if (main_total != 0) {
            message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
        }
        message.end_fn = EndScoreMessage;
        message.player_index = static_cast<i8>(player_index);
        AddGameMsg(&message);
    }
}

void Pup_CollectCoin(WORLDINFO_s *world, GIZMOPICKUP_s *pickup, i32 type, GameObject_s *object, i32 arg) {
    GizmoPickup_CollectCoin(world, &pickup->position, type, pickup->model_variant, object, arg);
}

void CollectMinikit(nuvec_s *position, char *name, i32) {
    if (WORLD->lev_objs[0xce].active != 0) {
        NUVEC target_position = {KITPOSX, KITPOSY + PANEL_MINIKITY, 1.0f};
        const i32 area_id = WORLD->level_sub_id;
        ADDGAMEMSG message = AddGameMsg_Default;
        message.position = position;
        message.target_position = &target_position;
        message.scale = AreaPickupScale;
        message.target_scale = PANEL_MINIKITSCALE;
        message.flags = 0x2112d;
        message.duration = 1.0f;
        message.icon = 0xce;
        message.special = &WORLD->lev_objs[0xce].special;
        message.tick_fn = MiniKit_GameMsg_Update;
        message.end_fn = MiniKit_GameMsg_End;
        message.field_0x4d = 1;
        AddGameMsg(&message);
        DrawMiniKitTime = 2.0f;

        if (area_id != -1 && Game.area_save[area_id].minikit_count < 10) {
            if (NuStrLen(name) <= 8) {
                NuStrCpy(NewMiniPiece[AreaGlobals.values.field_0x10].name, name);
            } else {
                NewMiniPiece[AreaGlobals.values.field_0x10].name[0] = '\0';
            }
            NewMiniPiece[AreaGlobals.values.field_0x10].level = WORLD->level_idx;
            ++AreaGlobals.values.field_0x10;
            ++AreaGlobals.values.field_0x0c;
            if (GAMEDEMO != 0) {
                if (AreaGlobals.values.field_0x0c == 1) {
                    Hint_SetHintFromId(0x26d, 0, 0);
                } else if (AreaGlobals.values.field_0x0c == 10) {
                    Hint_SetHintFromId(0x26e, 0, 0);
                }
            }
        }
        AddGameDebris(WORLD->debris_sys, 0x13, position);
        GameAudio_PlaySfxById(GetSfxId(const_cast<char *>("MK-Pickup")), position, 3, 0);
    }
}

static void Pup_CollectMinikit(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 type) {
    CollectMinikit(&pickup->position, pickup->name, type);
    NewBuzz(object->pad_gamepad->pad, 0.2f, 0);
}

void AddTorpedoAsPart(nuvec_s *position, nuvec_s *velocity, float scale, float lifetime) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    ADDPART_ALIGNED16 params = Default_ADDPART;
    params.position = position;
    params.velocity = velocity;
    f32 torpedo_scale = 0.1f * WORLD->giz_torp_machine_sys->scale;
    params.field_14 = torpedo_scale * GizmoPickupType[8].shadow_radius_x;
    params.field_18 = torpedo_scale * GizmoPickupType[8].shadow_extent_x;
    params.special = &world->lev_objs[0x79].special;
    params.field_28 = 0x79;
    params.gravity = AreaPickupGravity;
    if (params.gravity == 0.0f)
        params.field_a4 = 15.0f;
    params.stop_fn = PartStop_Flickerer;
    params.field_40 = FreePlay != 0 && WORLD->current_level == ANAKINSFLIGHTB_LDATA ? PartCollide_2D : PartCollide_3D;
    params.field_88 = lifetime;
    params.draw_fn = PartDraw_Torp;
    params.field_48 = NULL;
    params.flags = 0x256;
    if (params.gravity == 0.0f)
        params.move_fn = PartMove_VehiclePickup;
    params.time_step = FRAMETIME;
    params.field_c0 = scale;
    PART_s *part = AddPart(&params);
    if (part != NULL) {
        part->rotation_y = qrand();
        part->force_player_mask = 3;
        part->field_214 = 0.1f * WORLD->giz_torp_machine_sys->scale * NU_SIN_LUT(0x4000);
    }
}

void AddHeartAsPart(GameObject_s *recipient, nuvec_s *position, nuvec_s *velocity, float scale, float lifetime) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (recipient == NULL)
        return;
    ADDPART_ALIGNED16 params = Default_ADDPART;
    params.position = position;
    params.velocity = velocity;
    params.field_28 = 0xcb;
    params.flags = 0x256;
    params.field_18 = params.field_14 = scale * GizmoPickupType[5].shadow_radius_x;
    params.gravity = AreaPickupGravity;
    params.special = &world->lev_objs[0xcb].special;
    params.field_88 = lifetime;
    params.field_40 = PartCollide_3D;
    params.field_48 = PartUpdate_Heart;
    if ((world->current_level->flags & LEVEL_PICKUPS_TO_PANEL) != 0) {
        params.field_a4 = 1.0f;
        params.move_fn = PartMove_VehicleHeart;
        params.flags |= 0x400;
        params.recipient = recipient;
        params.draw_fn = PartDraw_VehicleHeart;
    } else {
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_Flickerer;
        if (params.gravity == 0.0f)
            params.move_fn = PartMove_VehiclePickup;
    }
    params.time_step = FRAMETIME;
    params.field_c0 = scale;
    PART_s *part = AddPart(&params);
    if (part != NULL)
        part->rotation_y = qrand();
}

void PowerUp_AddPart(NUVEC *position, NUVEC *velocity, f32 scale, f32 lifetime) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    ADDPART_ALIGNED16 params = Default_ADDPART;
    params.position = position;
    params.velocity = velocity;
    params.field_18 = params.field_14 = scale * GizmoPickupType[9].shadow_radius_x;
    params.gravity = AreaPickupGravity;
    params.special = &world->lev_objs[0xd0].special;
    params.field_40 = PartCollide_3D;
    params.field_48 = PowerUp_UpdatePart;
    params.field_3c = PowerUp_ImpactPart;
    params.stop_fn = PartStop_Flickerer;
    params.draw_fn = PowerUp_DrawPart;
    params.field_28 = 0xd0;
    params.flags = 0x256;
    params.field_88 = lifetime;
    if (params.gravity == 0.0f)
        params.move_fn = PartMove_VehiclePickup;
    params.time_step = FRAMETIME;
    params.field_c0 = scale;
    PART_s *part = AddPart(&params);
    if (part != NULL)
        part->rotation_y = NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f;
}

void AddCoinsAsParts(i32 type_id, nuvec_s *position, nuvec_s *velocity, float lifetime, float scale) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    GIZMO_PICKUP_TYPE *type = &GizmoPickupType[type_id];
    i32 model = static_cast<i16>(type->first_model_id);
    if (type->random_model_count != 0)
        model += qrand() / (65535 / type->random_model_count + 1);
    LEVEL_OBJECT_RUNTIME_s *object = &world->lev_objs[model];
    if (object->active == 0)
        return;
    ADDPART_ALIGNED16 params = Default_ADDPART;
    params.field_28 = model;
    params.velocity = velocity;
    params.position = position;
    params.special = &object->special;
    params.field_14 = params.field_18 = scale * type->shadow_radius_x;
    params.flags = 0x56;
    params.gravity = AreaPickupGravity;
    if (static_cast<u32>(model - 0xd5) <= 3) {
        params.flags |= 0x200;
    } else if (static_cast<u32>(model - 0xc7) <= 3) {
        params.flags |= 0x200;
        params.field_30 = PartExtra_BlueCoin;
    } else {
        params.replace_fn = PartStolen_Coin;
    }
    params.field_88 = lifetime;
    params.field_40 = PartCollide_3D;
    params.stop_fn = PartStop_Coin;
    params.field_3c = PartImpact_Coin;
    params.time_step = FRAMETIME;
    params.field_c0 = scale;
    PART_s *part = AddPart(&params);
    if (part != NULL) {
        part->field_100 = 30.0f;
        if (params.gravity == 0.0f) {
            part->move_callback = PartMove_VehiclePickup;
            part->field_104 = static_cast<f32>(qrand()) * (1.0f / 65535.0f) + 4.0f;
        }
    }
}

u32 TotalLevelCoinTally(WORLDINFO_s *world, u32 *pickups, u32 *blowups, u32 *buildits, u32 *forces, u32 *obstacles,
                        u32 *turrets, u32 *characters) {
    u32 value = GizmoPickups_TotalScore(world);
    u32 total = value;
    if (pickups != NULL)
        *pickups = value;
    value = GizmoBlowups_TotalScore(world);
    total += value;
    if (blowups != NULL)
        *blowups = value;
    value = GizBuildIts_TotalScore(world);
    total += value;
    if (buildits != NULL)
        *buildits = value;
    value = GizForce_TotalScore(world);
    total += value;
    if (forces != NULL)
        *forces = value;
    value = GizObstacles_TotalScore(world);
    total += value;
    if (obstacles != NULL)
        *obstacles = value;
    value = GizTurrets_TotalScore(world);
    total += value;
    if (turrets != NULL)
        *turrets = value;
    value = 0;
    if (world->area != NULL && (world->area->flags & 0x100) != 0)
        value = GameAI_TotalScore();
    total += value;
    if (characters != NULL)
        *characters = value;
    return total;
}

void CollectHitPoint(GameObject_s *object, NUVEC *position, i32) {
    if (static_cast<i8>(object->current_hp) > 0 && static_cast<i8>(object->current_hp) < object->hitpoints) {
        object->current_hp += TouchHacks::TouchControlsActive ? 2 : 1;
        if (static_cast<i8>(object->current_hp) > object->hitpoints)
            object->current_hp = object->hitpoints;
    }
    if (object->field_0xe38 < 4) {
        object->field_0xe38 += TouchHacks::TouchControlsActive ? 2 : 1;
        if (object->field_0xe38 == 5)
            object->field_0xe38 = 4;
    }
    if (object->apiobj.character_data->model_flags & 0x2000) {
        NUVEC momentum;
        NuVecScale(&momentum, &object->apiobj.velocity, 0.5f);
        AddGameDebrisMom(WORLD->debris_sys, 0x10, position, 15, &momentum);
    } else {
        AddGameDebris(WORLD->debris_sys, 0x0f, position);
    }
    GameAudio_PlaySfx(0x27, position, 0, 0);
    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
}

static void Pup_CollectHeart(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 flags) {
    CollectHitPoint(object, &pickup->position, flags);
}

i32 ReleaseHearts() {
    GameObject_s *player = Player[0];
    i32 missing_hitpoints;
    i32 total_hitpoints;
    if (player != NULL && player->apiobj.field_0x287 == 0) {
        total_hitpoints = player->hitpoints;
        const i32 current_hitpoints = static_cast<i8>(player->current_hp);
        missing_hitpoints = total_hitpoints != 0 ? total_hitpoints - current_hitpoints : 0;
    } else {
        total_hitpoints = 0;
        missing_hitpoints = 0;
    }

    player = Player[1];
    if (player != NULL && player->apiobj.field_0x287 == 0) {
        i32 current_hitpoints = static_cast<i8>(player->current_hp);
        i32 hitpoints = player->hitpoints;
        if (hitpoints != 0) {
            total_hitpoints += hitpoints;
            missing_hitpoints += hitpoints - current_hitpoints;
        } else if (total_hitpoints == 0) {
            return 0;
        }
    } else if (total_hitpoints == 0) {
        return 0;
    }

    if (missing_hitpoints > 0 && MAXPARTS > 0) {
        PART_s *part = Part;
        PART_s *end = part + MAXPARTS;
        do {
            if ((part->active & 1) != 0 && part->type_id == 0xcb) {
                --missing_hitpoints;
            }
            ++part;
        } while (part != end);
    }

    missing_hitpoints -= Game.save_version;
    if (Game.save_version <= 5) {
        missing_hitpoints += 8;
    } else {
        missing_hitpoints += 9;
    }

    i32 release_threshold = total_hitpoints;
    if (missing_hitpoints <= total_hitpoints)
        release_threshold = missing_hitpoints < 0 ? 0 : missing_hitpoints;

    i32 release_chance = (release_threshold << 16) / total_hitpoints;
    const i32 difficulty_adjustment = static_cast<i8>(adtab[adaptivedifficulty[0]][1]);
    if (difficulty_adjustment == 1) {
        release_chance >>= 1;
    } else if (difficulty_adjustment == -1) {
        release_chance += release_chance;
    }
    return release_chance > qrand();
}

void SetCoinType(i32 model, GIZMOPICKUP_s *pickup) {
    if (static_cast<u32>(model - 0xb7) <= 3) {
        pickup->type_index = 0;
        pickup->model_variant = model - 0xb7;
    } else if (static_cast<u32>(model - 0xbf) <= 3) {
        pickup->type_index = 1;
        pickup->model_variant = model - 0xbf;
    } else if (static_cast<u32>(model - 0xc7) <= 3) {
        pickup->type_index = 2;
        pickup->model_variant = model - 0xc7;
    } else {
        pickup->type_index = 3;
        pickup->model_variant = model - 0xd5;
    }
    u8 count = GizmoPickupType[pickup->type_index].random_model_count;
    pickup->model_variant = count ? pickup->model_variant % count : 0;
}

static __used__ void PartStolen_Coin(PART_s *part) {
    if (netclient != 0 || (part->render_flags & 2) == 0)
        return;
    GameObject_s *recipient = Player[0];
    i32 first_active = recipient != NULL && static_cast<i8>(recipient->apiobj.flags_low) < 0;
    if (Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.flags_low) < 0) {
        recipient = Player[1];
        if (first_active)
            recipient = Player[qrand() / 0x8000];
    } else if (!first_active) {
        return;
    }
    if (recipient != NULL) {
        GIZMOPICKUP_s pickup;
        SetCoinType(part->pickup_type, &pickup);
        GizmoPickup_CollectCoin(WORLD, &part->position, pickup.type_index, pickup.model_variant, recipient, 1);
    }
}

void PartStop_Coin(PART_s *part) {
    if (netclient == 0) {
        WORLDINFO_s *world = WORLD;
        GIZMOPICKUPRUNTIMESYS_s *system = world->gizmo_pickup_sys;
        GIZMOPICKUP_s *pickup = &system->temporary_pickups[system->field_0x0c];
        if (++system->field_0x0c > 63)
            system->field_0x0c = 0;
        if (pickup->state_flags & 1)
            GizmoPickup_CollectCoin(world, &pickup->position, pickup->type_index, pickup->model_variant, NULL, 1);
        SetCoinType(part->pickup_type, pickup);
        pickup->state_flags = (pickup->state_flags & 0xe0) | 0x17;
        pickup->position = part->position;
        pickup->draw_rotation = 0;
        pickup->room_index =
            WORLD->current_gscn != NULL ? NuPortalWhichRoom(WORLD->current_gscn, &pickup->position) : -1;
        pickup->floor_height = 2000000.0f;
        pickup->remaining_visible_time = 5.0f;
        pickup->shadow_x_rotation = 0;
        pickup->shadow_z_rotation = 0;
        pickup->state_flags = (pickup->state_flags | 0x20) & ~0x40;
    }
    part->active &= ~1;
}

i32 GetRandomCoinType() {
    i32 random_value = qrand();
    if (random_value < 0) {
        random_value += 0x3fff;
    }
    return CoinTab[random_value >> 14];
}

void AddCoinsToPanel(i32 coins, nuvec_s *position, i32 player, float, GameObject_s *, i32 random_types) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (coins != 0) {
        if (ChallengeMode || Mission_Active(NULL) != NULL)
            coins = 0;
        u8 counts[10] = {};
        i32 remainder = coins % 10;
        if (remainder > 0)
            coins -= remainder;
        if (coins > 512000)
            coins = 512000;
        if (coins > 0)
            PlaySfx("CoinsLand", position);
        if (random_types) {
            while (coins > 0) {
                i32 type = GetRandomCoinType();
                i32 remaining = coins - GizmoPickupType[type].score;
                if (!(remaining >= 0))
                    continue;
                ++counts[type];
                coins = remaining;
            }
        } else if (coins > 0) {
            for (i32 i = 3; coins > 0; --i) {
                i32 type = CoinTab[i];
                while (coins - GizmoPickupType[type].score >= 0) {
                    coins -= GizmoPickupType[type].score;
                    ++counts[type];
                }
            }
        }
        if (static_cast<u32>(player) > 1)
            player = -1;
        NUVEC target;
        target.x = player == 1 ? PANEL_COINX : -PANEL_COINX;
        i32 main_total = CoinsGoToMainTotal() != 0;
        f32 target_scale;
        if (main_total) {
            target.y = STATSPOSY;
            target_scale = COINTOTAL_COINSIZE;
        } else {
            target.y = STATSPOSY + PANEL_COINY;
            target_scale = PANEL_COINSCALE_END;
        }
        target.z = 1.0f;
        DrawBuildUpTime = COINMSGTIME + 1.0f;
        for (i32 i = 0; i < 4; ++i) {
            const i32 type_index = CoinTab[i];
            GIZMO_PICKUP_TYPE *type = &GizmoPickupType[type_index];
            const i32 count = counts[type_index];
            i32 base_model = static_cast<i16>(type->first_model_id);
            for (i32 j = 0; j < count; ++j) {
                i32 model = base_model;
                if (type->random_model_count != 0)
                    model += qrand() / (65535 / type->random_model_count + 1);
                if (world->lev_objs[model].active == 0 || player == -1)
                    continue;
                ADDGAMEMSG_ALIGNED16 message = AddGameMsg_Default;
                message.position = position;
                message.target_position = &target;
                message.target_scale = target_scale;
                message.icon = model;
                message.extra_position = reinterpret_cast<NUVEC *>(&world->lev_objs[message.icon]);
                message.end_fn = EndScoreMessage;
                message.score = type->score;
                message.player_index = player;
                message.field_0x4d = 1;
                message.field_0x20 = AddCoinDelay[player];
                message.flags = 0x12d;
                message.scale = 1.0f;
                message.duration = COINMSGTIME;
                if (main_total) {
                    message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
                    message.field_0x4e = 1;
                }
                AddGameMsg(&message);
                AddCoinDelay[player] += 0.1f;
            }
        }
    }
}

void AddPickups(i32 coins, i32 hearts, i32 torpedoes, i32 powerups, nuvec_s *position, nuvec_s *direction, float,
                i32 player_id, float speed, float lifetime, GameObject_s *owner, i32, i32 panel, bool consolidate) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    const f32 scale = AreaPickupScale;
    u32 arcade_flags;
    Arcade_GetMode(&arcade_flags);
    if ((arcade_flags & 0x40) != 0)
        coins = torpedoes = 0;
    i32 to_panel;
    if ((world->current_level->flags & LEVEL_PICKUPS_TO_PANEL) != 0) {
        to_panel = 1;
        if (__builtin_expect(player_id == -1, 0)) {
            if (player2 != NULL)
                player_id = qrand() / 32768;
            else if (Player[0] != NULL && Player[0]->apiobj.player_controlled)
                player_id = 0;
            else
                player_id = 1;
        }
    } else if (world->area != NULL && (world->area->flags & 0x100) != 0) {
        player_id = qrand() / 32768;
        to_panel = 1;
    } else {
        to_panel = player_id == -1 ? 0 : panel;
    }
    coins = ChallengeMode != 0 || Mission_Active(NULL) != NULL ? 0 : coins;
    if (speed == 1.0f)
        speed = scale;
    u8 counts[10] = {};
    if (netclient != 0)
        return;
    i32 remainder = coins % 10;
    if (remainder > 0)
        coins -= remainder;
    if (coins > 512000)
        coins = 512000;
    bool has_coins = false;
    if (coins > 0) {
        PlaySfx("CoinsLand", position);
        i32 remaining = coins;
        uintptr_t coin = reinterpret_cast<uintptr_t>(&CoinTab[3]);
        do {
            i32 type = *reinterpret_cast<const u8 *>(coin);
            while (remaining - GizmoPickupType[type].score >= 0) {
                remaining -= GizmoPickupType[type].score;
                ++counts[type];
            }
            --coin;
        } while (remaining > 0);
        has_coins = true;
    }
    if (world->area != NULL && (world->area->flags & 0x104) == 0 && consolidate) {
        i32 silver = counts[0], gold = counts[1], blue = counts[2], purple = counts[3];
        if (silver > 1) {
            silver = 0;
            ++gold;
            if (__builtin_expect(gold > 9, 0)) {
                gold %= 10;
                ++blue;
                if (blue > 9) {
                    blue %= 10;
                    ++purple;
                }
            }
        }
        if (__builtin_expect(gold > 4, 0)) {
            gold = qrand() / 32768;
            ++blue;
            if (blue > 9) {
                blue %= 10;
                ++purple;
            }
        }
        if (__builtin_expect(blue > 5, 0)) {
            blue = qrand() / 65536;
            ++purple;
        }
        counts[0] = silver;
        counts[1] = gold;
        counts[2] = blue;
        counts[3] = purple;
    }
    if (static_cast<u32>(player_id) >= 2)
        player_id = -1;
    NUVEC velocity;
    if (to_panel == 0) {
        for (i32 i = 0; i < 4; ++i) {
            i32 type = CoinTab[i];
            for (i32 j = 0; j < counts[type]; ++j) {
                MakePartVector(&velocity, direction, speed);
                AddCoinsAsParts(type, position, &velocity, lifetime, scale);
            }
        }
    } else {
        if (has_coins)
            DrawBuildUpTime = COINMSGTIME + 1.0f;
        AddCoinsToPanel(coins, position, player_id, speed, owner, 0);
    }
    if (hearts > 0) {
        i32 both_players = 0;
        if (VehicleArea != 0 && Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[1] != NULL &&
            Player[1]->apiobj.player_controlled) {
            both_players = 1;
            hearts = 2;
        }
        for (i32 i = 0; i < hearts; ++i) {
            if (to_panel != 0)
                velocity = v000;
            else if (TouchHacks::TouchControlsActive && player != NULL) {
                NUVEC toward_player = {player->apiobj.position.x - position->x, 0.0f,
                                       player->apiobj.position.z - position->z};
                if (toward_player.x * toward_player.x + toward_player.y * toward_player.y +
                        toward_player.z * toward_player.z >
                    1.0f)
                    NuVecNorm(&toward_player, &toward_player);
                if (AreaPickupGravity == 0.0f) {
                    toward_player.y = 0.0f;
                } else {
                    toward_player.y = static_cast<f32>(qrand()) * (1.0f / 65535.0f) + 1.0f;
                }
                velocity = toward_player;
            } else
                MakePartVector(&velocity, direction, speed);
            GameObject_s *recipient = owner;
            if (both_players)
                recipient = i == 0 ? Player[0] : Player[1];
            else if (recipient == NULL) {
                GameObject_s *eligible[2];
                i32 count = 0;
                if (Player[0] != NULL && Player[0]->apiobj.player_controlled)
                    eligible[count++] = Player[0];
                if (Player[1] != NULL && Player[1]->apiobj.player_controlled)
                    eligible[count++] = Player[1];
                if (count != 0)
                    recipient = eligible[count == 1 ? 0 : qrand() / 32768];
            }
            AddHeartAsPart(recipient, position, &velocity, scale, lifetime);
        }
    }
    for (i32 i = 0; i < torpedoes; ++i) {
        MakePartVector(&velocity, direction, speed);
        AddTorpedoAsPart(position, &velocity, scale, lifetime);
    }
    if ((world->current_level->flags & LEVEL_PICKUPS_TO_PANEL) != 0) {
        return;
    }
    for (i32 i = 0; i < powerups; ++i) {
        MakePartVector(&velocity, direction, speed);
        PowerUp_AddPart(position, &velocity, scale, lifetime);
    }
}

void AddMiscPickups(nuvec_s *position, i32 player_id, i32 coins, i32 torpedoes) {
    if (torpedoes != 0) {
        AddPickups(coins, 0, torpedoes, 0, position, &v010, 5.0f, player_id, 1.0f, 2000000.0f, NULL, 1, 0, true);
    } else {
        AddPickups(coins, ReleaseHearts(), 0, 0, position, &v010, 5.0f, player_id, 1.0f, 2000000.0f, NULL, 1, 0, true);
    }
}

i32 IsACoinType(i32 type) {
    for (i32 i = 0; i < 4; ++i) {
        if (CoinTab[i] == type) {
            return 1;
        }
    }
    return 0;
}

i32 LoseCoins(GameObject_s *object, i32 cause) {
    if (Player_HasInvincibility(object) != 0 ||
        (object->apiobj.character_data->game_character->flags_090 & 0x8000) != 0 || object->coinpacket == NULL ||
        (Arcade != 0 && (Arcade_Mode[static_cast<i8>(ArcadeItem.field_c_0xc)].field8_0x8 & 8) != 0)) {
        return 0;
    }
    u32 lost = 0;
    if (cause == 1) {
        lost = static_cast<u32>(TouchHacks::GetLoseStudsDieValue());
    } else if (cause == 2) {
        lost = static_cast<u32>(TouchHacks::GetLoseStudsFallValue());
    }
    if (lost != 0) {
        const i32 adjustment = adtab[adaptivedifficulty[0]][0];
        if (adjustment == 1) {
            lost *= 2;
        } else if (adjustment == -1) {
            lost >>= 1;
        }
        if (lost > object->coinpacket->coins) {
            lost = object->coinpacket->coins;
            object->coinpacket->coins = 0;
        } else if (lost != 0) {
            object->coinpacket->coins -= lost;
        }
    }
    if (Cheats_CheckFlags(0x7c) != 0 || DoubleScoreTime > 0.0f) {
        lost = 0;
    }
    return static_cast<i32>(lost);
}

void GizmoPickups_SetOnOff() {
    u32 arcade_flags;
    Arcade_GetMode(&arcade_flags);
    WORLDINFO_s *world = WORLD;
    for (i32 index = 0; index < 10; ++index) {
        if (ChallengeMode != 0) {
            if (index == 7) {
                GizmoPickupType[index].field_0x0f = 0;
            } else {
                GizmoPickupType[index].field_0x0f = 1;
            }
        } else if (index == 7) {
            GizmoPickupType[index].field_0x0f = 1;
        } else if (index == 6 && world->level_sub_id != -1 && Game.area_save[world->level_sub_id].field_0x5[1] != 0) {
            GizmoPickupType[index].field_0x0f = 1;
        } else if (index == 4 && SuperStory != 0) {
            GizmoPickupType[index].field_0x0f = 1;
        } else if (index == 9 && world->area != NULL && (world->area->flags & 0x100) != 0) {
            GizmoPickupType[index].field_0x0f = 1;
        } else {
            GizmoPickupType[index].field_0x0f = (arcade_flags & 0x20) != 0 && index != 9 && index != 5;
        }
    }
}

