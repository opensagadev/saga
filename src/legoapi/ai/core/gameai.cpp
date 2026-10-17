// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/props/objects/tightrope.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

#include "legoapi/ai/core/gameai_types.h"

extern "C" i32 adaptivedifficulty[3];

i8 adtabentries[9][4] = {{-1, -1, -1, -1}, {-1, -1, -1, 0}, {-1, -1, 0, 0}, {-1, 0, 0, 0}, {0, 0, 0, 0},
                         {1, 0, 0, 0},     {1, 1, 0, 0},    {1, 1, 1, 0},   {1, 1, 1, 1}};
i8 (*adtab)[4] = &adtabentries[4];

extern void InitSurfaceInfo(GameObject_s *);
extern i32 SetObjOnSurface(GameObject_s *, i32);
extern void Player_ClearContext(GameObject_s *, i32);
extern void Player_ResetContexts(PLAYERPACKET_s *);
extern GameObject_s *GetOtherActivePlayer(GameObject_s *object);

i32 CheckPosAIArea(AIAREA_s *area, nuvec_s *position, float tolerance);



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
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
void PartCollide_3D(PART_s *);
f32 DropInOutScale(GameObject_s *);
extern "C" f32 sabrerubwait;
extern i32 lightning_type;
extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;
extern i32 VehicleArea;
extern i32 TERRAINMASK_NONWEAPON, TERRAINMASK_NONDROID;
f32 DEFENDTIME = 4.0f;
extern f32 jump_stuck_time;
i32 teleport_all_freeplay_modes = 1;
i32 drop_in_teleport = 3;


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
void AnimatePlayer(GameObject_s *object);
void SetPlayer();
void InitPlayerAI(GameObject_s *object);
void ResetPlayerMoves(GameObject_s *object);
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
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
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


extern i32 LEGO_AIPATHCNX_BLOCKAGE;

extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

extern GAMEFOG_STATE GameFog;


#include "legoapi/items/objects/gameobjects_types.h"






// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

static const f32 AI_RESPAWN_DELAY = 2.0f;

extern TERRSET *CurTerr;






























































extern "C" f32 NuAnimEndFrameOld(void *animation);

extern "C" {
extern i32 nbaddies_can_see_players;
}














































i32 CanFightLikeAJedi(GameObject_s *object);












































extern "C" {
    f32 default_path_heighttol = 0.2f;
    u8 default_activate_difficulty = AI_DEFAULT_ACTIVATE_DIFFICULTY;
    u8 default_min_n_respawns;
    u8 default_max_n_respawns;
    f32 default_min_t_respawn;
    f32 default_max_t_respawn;

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
static const char *skip_chars[] = {"Batman", "Robin", "Glide_Pack", NULL};

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

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask);

extern "C" f32 MainRenderTime;
extern "C" f32 MainRenderTargetTime;
extern void (*BackDrop_AlphaFn)(f32 *alpha);
extern void BackDrop_UpdateColours(i32 instant);
extern i32 Paused;
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

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

f32 draw_attention_distance = 2.0f;
extern f32 party_follow_offsets[8];
extern i32 active_neutral_count;
extern "C" i32 party_under_cover;
extern "C" i32 party_cant_be_under_cover;
extern AREADATA *VADER_ADATA;
extern GameObject_s *alert_obj;
void AITriggerSetSysProcess(AITRIGGERSETSYS_s *system);
i32 Hub_InMenu();
i32 test_new_interaction;
i32 debug_ignore_players;
i32 WALKSTEALTH;
i32 active_goody_count;
extern i32 active_baddy_count;
f32 timetogetlos;
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

static void TestWalkAround(APIOBJECT *object, f32 *other_x, f32 *other_z, NUVEC *difference, f32 radius) {
    if (difference->x * difference->x + difference->z * difference->z < radius * radius) {
        difference->x = object->position.x - *other_x;
        difference->z = object->position.z - *other_z;
        i32 angle = NuAtan2D(difference->x, difference->z);
        difference->x = object->ai->movement_destination.x - *other_x;
        difference->z = object->ai->movement_destination.z - *other_z;
        i32 turn = NuAngSub(NuAtan2D(difference->x, difference->z), angle);
        i32 limit = static_cast<i32>((ai_moveradius / radius) * 10430.378f);
        if (turn > limit)
            turn = limit;
        else if (turn < -limit)
            turn = -limit;
        angle = NuAngAdd(angle, turn);
        difference->x = 0.0f;
        difference->y = 0.0f;
        difference->z = radius;
        NuVecRotateY(difference, difference, angle);
        object->ai->movement_position.x = difference->x + *other_x;
        object->ai->movement_position.y = object->ai->movement_destination.y;
        object->ai->movement_position.z = difference->z + *other_z;
    }
}

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



void GameAISysReset(AISYS_s *system);

extern f32 alert_timer;

void MakeBaddiesForgetAboutParty(i32 check_hostility);

void GameAttackInit();

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

void GameObjectSetCanUse(GameObject_s *object, void *target, unsigned char action, unsigned char, float parameter);

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object);


void GameObjectDimensions(GameObject_s *object);




i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern i16 id_ANAKINJEDISCARRED;




extern NUVEC alert_pos;
extern "C" {
    i32 party_under_cover;
}
i32 ZapTarget(GameObject_s *object);

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float);


extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
i32 CannotKill(GameObject_s *object);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);
extern f32 ForceThrowSpeed, ForceThrowGravity;
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);


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


i32 NoLayerKill(GameObject_s *object);




GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only);

void DestroySnakeBody(GameObject_s *);
void BlowUpSnakeBody(GameObject_s *);
void DieRumble(GameObject_s *);
void ReleaseEat(GameObject_s *);
extern i32 speeder_hitpoints_lost;




extern GameObject_s * player_tag_to;
extern GameObject_s * player_tag_from;
extern char *ASCII_DOWN;




void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32);

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;

extern i32 nethost;
void ResetAICreature(GameObject_s *, AISYS_s *);
GameObject_s *GetOtherActivePlayer(GameObject_s *);
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

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

void RemoveChunkFromRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

void RemoveChunkControlFromStack(debris_chunk_control_s *control, debris_chunk_control_s **stack);

extern "C" debkeydatatype_s *debris_keystack;


// ==== src/legoapi/gizmos/trigger/ai.cpp: all remaining content ====
#include "legoapi/gizmos/trigger/ai.h"

#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"

static i32 AI_GetMaxGizmos(void *);

static void AI_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *unused);

static char *AI_GetGizmoName(GIZMO *gizmo);

static i32 AI_GetOutput(GIZMO *gizmo, i32, i32);

char *AI_GetOutputName(GIZMO *, i32);

static i32 AI_GetNumOutputs(GIZMO *);

void AI_Activate(GIZMO *gizmo, i32 active);

ADDGIZMOTYPE *AI_RegisterGizmo(i32 type_id);

// ==== src/legoapi/ai/game/creature.cpp: declarations ====
#include "decomp.h"
#include "legoapi/ai/game/creature.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/audio/sfx.h"


void RemoveGameObject(GameObject_s *object, i32 immediate);


void StoreProgressAICharacter(LEVEL_PROGRESS_s *progress);


extern "C" {
    extern NUVEC plr_lastpos;
    AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);
    void AddToAIGroup(AIGROUP *group, APIOBJECT *object);
    void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
    void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);
    void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 route, i32 surface_flags);
    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation);
    void SetAnimTimeRandom(CHARACTERMODEL_s *model, ANIMPACKET_s *packet);
}

void Player_ClearContext(GameObject_s *object, i32 context);
#include "legoapi/ai/game/creature_types.h"



void ResetAICreature(GameObject_s *object, AISYS_s *system);


void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void NewRumbleAllPlayers(f32, f32, i32, i32);
i32 ObjHitObj_Flags(GameObject_s *);


i32 SpawnMeleeCreatureType(i32 type);

GameObject_s *alert_obj;
NUVEC alert_pos;
f32 alert_timer;
f32 alert_time = 10.0f;

void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position);

// ==== src/legoapi/ai/game/aipathcnxhelper.cpp: declarations ====
#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"

#include <string.h>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"


extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
extern void *CutScene_FindInst(CUTSYS *, char *);
extern FLOWBOX_s *FlowBoxFindByName(GIZFLOW_s *, char *);
extern "C" {
    extern f32 aiEditor_DrawYOffset;
    void LocaledbitsDrawCircleXY(NUVEC *, f32, u32, i32, i32);
    void LocaledbitsDrawSolidCircleXY(NUVEC *, f32, f32, f32, u32, i32, i32);
}



__attribute__((force_align_arg_pointer)) void pathEditorDrawNode(nuvec_s *position, float radius, float lower_height,
                                                                 float upper_height, u32 colour, numtl_s *material,
                                                                 i32 segments, i32 solid);

extern void (*AIPathCnxHelperSysInitFn)(WORLDINFO_s *);


AIPATHCNXHELPER_s *AIPathCnxHelperSys_Find(AIPATHCNXHELPERSYS_s *system, GameObject_s *object, AIPATHCNX_s *connection,
                                           u8 direction, u8 type,
                                           i32 (*filter)(AIPATHCNXHELPER_s *, GameObject_s *, AIPATHCNX_s *, u8, u8));


void AIPathCnxControlSysUpdate(AIPATHCNXCONTROLSYS_s *system);

AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system,
                                                 AIPATH_s *path, char *from, char *to, i32 target_type,
                                                 char *target_name, i32 fake_animation_id, i32 gizmo_output);

void AIPathCnxControllerDestroy(AIPATHCNXCONTROLSYS_s *system, AIPATHCNXCONTROLLER_s *controller);

void AIPathCnxSetTemporaryBlock(AIPATH_s *path, char *from_name, char *to_name, i32 blocked);

AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(AIPATHCNXHELPERSYS_s *system, AIPATHCNX_s *connection, u8 direction,
                                                void *target, u8 type);

void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame);

// ==== src/legoapi/ai/core/ai_sys.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
struct nuhspecial_s;
struct minitrooperteam_s;
struct nuvec_s;

void *GameBufferAlloc(VARIPTR *, VARIPTR *, i32);
extern "C" void *AISysLoadEx(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param,
                             char *load_dir);

extern "C" void *AISysLoad(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param);
void *AIPathCnxControlSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count);
void *AIPathCnxHelperSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count);
void *CreateClimbObjectSys(VARIPTR *buf, VARIPTR *buf_end, i32 count);

// ==== src/legoapi/actions/character/simpletons.cpp: declarations ====
#include "legoapi/world/world.h"
#include "legoapi/props/system/socksys.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"


extern "C" {
    extern i16 id_PENGUIN_GOON, id_PENGUIN_GOON_GUN;
    extern i16 id_JOKER_GOON, id_JOKER_GOON_GUN;
    extern i16 id_RIDDLER_GOON, id_RIDDLER_GOON_GUN;
}

i16 GetGenericGoon(i32 armed);




i32 objInNetWaitContext(GameObject_s *object, i32 context);

#include "legoapi/actions/character/simpletons_types.h"

static AtOnceAttacker AtOnce_attackingPlayer[8][17];
static i32 AtOnce_attackersPerRow = 32;
static f32 AtOnce_InitialRowDist = 0.75f;
static f32 AtOnce_RowDist = 0.75f;
static i32 AtOnce_maxAttackers = 1;

i32 oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent);

f32 oneAtOnce_GetHoldRange(GameObject_s *object);

void oneAtOnce_MaintainArray();

void oneAtOnce_SetDistPerRow(float distance);


void oneAtOnce_SetNumAttackers(i32 attackers);

void oneAtOnce_SetInitDistPerRow(float distance);

void oneAtOnce_SetAttackersPerRow(i32 attackers);

// ==== src/legoapi/ai/core/aisysall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include <string.h>

#include <float.h>
#include <math.h>
#include <string.h>

// ---- begin src/legoapi/ai/core/aipathbuild.cpp ----
#include "editor/edpath.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include <float.h>
#include <string.h>




// The route records use a dense route index, while the editor keeps sixteen
// independently switchable slots.  The original helper first marks both ends
// of every route connection, then builds the compact node lookup and matrix.
#if defined(__i386__)
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif
// Retail keeps only the IPA-SRA clone, without the unused path index.
static AIPATHBUILD_CLONE_ATTR2 void pathEditorCreateSpecialRouteData(AIPATH_s *path, EDAIPATH_s *editor_path,
                                                                     VARIPTR *cursor, VARIPTR *end)
    __asm__("_ZL32pathEditorCreateSpecialRouteDataP8AIPATH_siP10EDAIPATH_sP9variptr_uS4_.isra.81");

static void pathEditorCreateSpecialRouteData(AIPATH_s *path, EDAIPATH_s *editor_path, VARIPTR *cursor,
                                             VARIPTR *end);

extern "C" {
AIPATHSYS_s *pathEditorCreateData(VARIPTR *cursor, VARIPTR *end, void **scratch_cursor, void **scratch_end);
}

// ---- end src/legoapi/ai/core/aipathbuild.cpp ----


// Retail keeps several static helpers here only as GCC clones; these attributes
// reproduce the clone names and register conventions.
#if defined(__i386__)
#define AISYSALL_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define AISYSALL_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#define AISYSALL_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define AISYSALL_CLONE_ATTR1 __attribute__((noinline, noclone))
#define AISYSALL_CLONE_ATTR2 __attribute__((noinline, noclone))
#define AISYSALL_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// IPA-SRA keeps only the fields this helper reads: values for those read before any call,
// addresses for those read after one.
static AISYSALL_CLONE_ATTR2_SSE u32 CalculateRightIntersection(f32 object_radius, f32 first_distance,
                                                               f32 *second_distance, i32 first_angle,
                                                               i32 second_angle, AIPATHNODE *before, f32 *shared_y,
                                                               f32 shared_radius, AIPATHNODE *after, NUVEC *result)
    __asm__("_ZL26CalculateRightIntersectionP11APIOBJECT_sP11AIPATHCNX_sS2_iiP12AIPATHNODE_sS4_S4_P7nuvec_s.isra.50");

static u32 CalculateRightIntersection(f32 object_radius, f32 first_distance, f32 *second_distance, i32 first_angle,
                                      i32 second_angle, AIPATHNODE *before, f32 *shared_y, f32 shared_radius,
                                      AIPATHNODE *after, NUVEC *result);

#include "legoapi/ai/core/ai_pathgeometry.h"

extern "C" i32 WithinConnection(AISYS *system, NUVEC *position, AIPATH *path, AIPATHCNX *connection, i32 checks,
                                AIPATHCNX *previous_connection, i32 route, i32 ground, AIPATHINFO *path_info,
                                f32 radius, i32 update_once);

static __attribute__((noinline, noclone)) u32 CalculateIntersection(AISYS *system, AIPACKET *packet, APIOBJECT *object,
                                                                    AIPATHCNX *current_connection,
                                                                    AIPATHCNX *target_connection);

void AISysGetPathPos2(AISYS_s *system, nuvec_s *position, AIPATHINFO_s *path_info, nuvec_s *path_position, AIPATH_s *,
                      i32);


static AISYSALL_CLONE_ATTR2 void AIMoveFindDivertNode(AIPATH *path, AIPACKET *packet, NUVEC *destination)
    __asm__("_ZL20AIMoveFindDivertNodeP7AISYS_sP8AIPATH_sP10AIPACKET_sP7nuvec_s.isra.26");

static void AIMoveFindDivertNode(AIPATH *path, AIPACKET *packet, NUVEC *destination);

static AISYSALL_CLONE_ATTR1 i32 AIMoveAdjustDestinationPath(AIPACKET *packet)
    __asm__("_ZL27AIMoveAdjustDestinationPathP7AISYS_sP10AIPACKET_s.isra.48");

static i32 AIMoveAdjustDestinationPath(AIPACKET *packet);

// IPA-SRA hands this clone the address of system->path_sys rather than the system.
static AISYSALL_CLONE_ATTR2 i32 AIMoveChooseExitNodePath(AIPATHSYS **path_sys, AIPACKET *packet)
    __asm__("_ZL24AIMoveChooseExitNodePathP7AISYS_sP10AIPACKET_s.isra.82");

static i32 AIMoveChooseExitNodePath(AIPATHSYS **path_sys, AIPACKET *packet);

extern "C" void AISysFindRoute(AIPACKET *packet);

extern "C" void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);

extern "C" void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);

extern "C" f32 AIPathNodeDistanceToPathNode(AIPATH *path, i32 start_node, i32 destination_node, i32 route_index,
                                            u32 excluded_route_mask);


extern "C" i32 AISysGetCharacterWaypoint(const AIPACKET *packet, NUVEC *position);

void AIMoveToDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks);

void AIRetreatFromDestination2(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks);

void AIRetreatFromDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks);

void AIRetreatFromDestination2(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32);

void AIMoveDirectlyToDestination(AISYS_s *, AIPACKET_s *packet, APIOBJECT_s *object, i32);

void AIMoveToDestinationAvoidingCamera(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks);

bool AISysNodeCanReachThisJumpConnection(GameObject_s &, AIPATH_s &path, unsigned char node_index,
                                         AIPATHCNX_s &jump_connection, i32 jump_direction);

bool AISysCharacterCanReachThisJumpConnection(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &jump_connection,
                                              i32 jump_direction);

u32 DoSomeChecks(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &connection, i32 direction);

void AICircle(AISYS_s *, AIPACKET_s *packet, APIOBJECT_s *object, i32);

void AIWander(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks);

// ---- begin src/gameapi/ai/aisys/aisys.cpp ----
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

#include "globals.h"

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

#include "legoapi/items/objects/gameobjects.h"

#include "legoapi/props/system/socksys.h"

#include "legoapi/ai/core/ai_sys_stubs.h"

#include "legoapi/gizmos/traps/gizforce.h"

#include "legoapi/gizmos/object/newblowup.h"

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

#include "nu2api/nu3d/nuspecial.h"

#include "nu2api/numath/numtx.h"



void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay);



void AlertSurroundingCreatures(GameObject_s *object, NUVEC *position);




void AddGameMsgCount(NUVEC *position, i32 count, i32 total, u8 red, u8 green, u8 blue, f32 duration);


extern "C" void ComplexSockAngles(SOCKROT *angles);

extern void oneAtOnce_SetInitDistPerRow(f32 distance);

extern i32 oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent);

extern f32 oneAtOnce_GetHoldRange(GameObject_s *object);

extern void Hint_CancelCurrent();


extern void SetPlayer();


extern void ResetAICreature(GameObject_s *object, AISYS_s *system);

extern void DeactivateGameObject(GameObject_s *object);

extern void Player_ClearContext(GameObject_s *object, i32 mode);

extern void Player_ResetContexts(PLAYERPACKET_s *packet);




extern void ReleaseEat(GameObject_s *object);

extern void ReleaseTakeOver(GameObject_s *object, i32 immediate);

extern void TakeOverGameObject(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera, i32 immediate);


extern void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, f32 distance, i32 looping);

extern void KillParts(GameObject_s *object, i32 part, i32 joint, i32 visible, f32 velocity, i32 flags, u16 *part_ids);


extern AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system,
                                                        AIPATH_s *path, char *from, char *to, i32 target_type,
                                                        char *target_name, i32 fake_animation_id, i32 gizmo_output);

extern void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame);

extern void AIPathCnxSetTemporaryBlock(AIPATH_s *path, char *from_name, char *to_name, i32 blocked);

extern AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(AIPATHCNXHELPERSYS_s *system, AIPATHCNX_s *connection,
                                                       u8 direction, void *target, u8 type);

extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);

extern "C" ADDPART_s Default_ADDPART;


extern "C" PART_s *AddPart(ADDPART_s *);


void MakeThrowVector(NUVEC *, NUVEC *, NUVEC *, NUVEC *, f32, f32);


i32 Action_SetState(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_FollowPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UsePanel(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_PullLever(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UseTechno(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_HelpWithTriggers(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UseTriggerSet(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);











i32 Action_SetVisibility(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);










// Game-specific AI actions and conditions (registered via
// RegisterAIScriptActions / RegisterAIScriptConditions). These are stubbed to
// satisfy the symbol baseline; the action/condition logic itself is not
// decompiled. Each stub matches the mangled symbol of the original binary.




void LevelScriptReStoreProgress(WORLDINFO_s *, LEVELSCRIPTPROCESS_s *);

#include "legoapi/props/objects/techno.h"

extern void oneAtOnce_SetDistPerRow(f32);

extern void oneAtOnce_SetNumAttackers(i32);

extern void oneAtOnce_SetAttackersPerRow(i32);





























i16 GetGenericGoon(i32);

extern "C" AIGROUP *CreateAIGroup(AISYS *, i32, f32, f32, f32);






void PartKill_ForceThrow(PART_s *, i32);






























































































extern AIACTIONDEF api_aiactiondefs[55];

extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

extern "C" void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *end, u32 size);

extern "C" void ResetAIMessageSys(AIMESSAGESYS_s *sys);

extern "C" AIMESSAGESYS_s *CreateAIMessageSys(VARIPTR *cursor, VARIPTR *end, i32 count);

extern "C" void ClearAIMessageSys(AIMESSAGESYS_s *system);

extern "C" AIMESSAGE_s *CheckAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message);

extern "C" f32 GetAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message);

extern "C" void SetAIMessage(AIMESSAGESYS_s *system, char *name, f32 value, AIMESSAGE_s *message);

extern "C" AIMESSAGE_s *QueryAIMessage(AIMESSAGESYS_s *system, AIMESSAGE_s *message);

extern "C" void RemoveAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message);

extern "C" AILOCALMESSAGE_s *FindLocalAIMessage(AISCRIPTPROCESS *processor, char *name);

extern "C" void AddLocalAIMessage(AISCRIPTPROCESS *processor, AILOCALMESSAGE_s *message, char *name);

extern "C" AIPATHCNX *AIPathFindPathCnxFromIX(AISYS *system, AIPATH *path, u8 from_index, u8 to_index);

extern AIANTINODE dynamic_antinodes[64];

extern "C" f32 default_path_heighttol;

extern "C" void AISetPathHeightTol(f32 tolerance);

extern "C" AIANTINODE *AIAntinodeCreate(NUVEC *position, f32 radius);

extern "C" AIANTINODE *AIAntinodeCreateSingleFrame(NUVEC *position, f32 radius);

extern "C" void AIAntinodeDestroy(AIANTINODE *antinode);

extern "C" void AIAntinodeCullSingleFrame(void);

extern "C" void AIAntinodeMove(AIANTINODE *antinode, NUVEC *position, f32 radius, f32 below, f32 above);

extern "C" void AIFormationFollow(AIPACKET *packet);

extern "C" void AILocatorSet_CheckLocatorsStillAssigned(AISYS *system, AILOCATORSET *locator_set);

extern "C" void AILocatorSet_AssignNearestLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                  f32 max_range, NUVEC *position, NUVEC *second_position,
                                                  f32 off_screen_radius, i32 ignore_assigned);

extern "C" void AILocatorSet_AssignFurthestLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                   f32 max_range, NUVEC *position, NUVEC *second_position,
                                                   f32 off_screen_radius, i32 ignore_assigned);

extern "C" void AILocatorSet_AssignRandomLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                 f32 max_range, NUVEC *position, f32 off_screen_radius,
                                                 i32 ignore_assigned);

// ---- end src/gameapi/ai/aisys/aisys.cpp ----

// ==== src/gameapi/ai/aisys/aiscript.cpp: all remaining content ====
#include "gameapi/ai/aisys/aisys.h"
#include "decomp.h"
#include "legoapi/world/world.h"

#include <stdio.h>
#include <string.h>

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nurdp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nurand.h"







extern AIACTIONDEF * game_aiactiondefs;

extern NULISTHDR global_aiscripts;


extern i32 AiParseExpressionFailed;

extern GAMEPARAMTOFLOAT * GameParamToFloatFn;


f32 AiParseExpression(char *expr);






#if defined(__i386__)
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail inlines the "goto" test into each caller; this is the remaining partial clone.
static AISCRIPT_CLONE_ATTR2 void ConditionsParseFinalise(AICONDITION *cond, NUFPAR *parser)
    __asm__("_ZL23ConditionsParseFinaliseP13AICONDITION_sP8nufpar_s.part.51");

static void ConditionsParseFinalise(AICONDITION *cond, NUFPAR *parser);

// Retail inlines the system test into its callers; this is the remaining partial clone.
static AISCRIPT_CLONE_ATTR2 void AIScriptFindRefScripts(AISYS *sys, AISCRIPT *script)
    __asm__("_ZL22AIScriptFindRefScriptsP7AISYS_sP10AISCRIPT_s.part.61");

static void AIScriptFindRefScripts(AISYS *sys, AISCRIPT *script);

void AIScriptInitConditions(AISYS *sys);



#include "legoapi/ai/core/aisysall_types.h"













extern i32 ai_usepackfile;
extern i32 ai_onlyusepackfile;

void AIScriptLoadAll(char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS *sys);




void AIScriptLoadAllPakFile(void *pak, char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS *sys);

void AIScriptOpenPakFileParse(AISCRIPT **script_ref, void *pak, char *filename, char *path, VARIPTR *buf,
                              VARIPTR *buf_end);

void AIScriptProcessorInit(AISYS *sys, AIPACKET *packet, AISCRIPTPROCESS *processor, AICREATURE *creature,
                           char *script_name, char *start_state_name, i32 can_use_default, AISCRIPT *script,
                           AISTATE *start_state);

AISCRIPT *AIScriptFind(AISYS *sys, char *name, i32 can_use_default, i32 check_level_scripts, i32 check_global_scripts);

void AIScriptClearInterrupt(AISCRIPTPROCESS *processor, char *state_name);

i32 AIScriptSetInterrupt(AISCRIPTPROCESS *processor, u8 priority, u8 id, char *state_name, f32 time);

void AIScriptSetState(AISCRIPTPROCESS *processor, AISTATE *state);

i32 AIScriptSetStateByName(AISCRIPTPROCESS *processor, char *name);

void RegisterAIScriptActions(AIACTIONDEF *action_defs);

i32 AIScriptSetBaseScriptStateByName(AISCRIPTPROCESS *processor, char *name);

void RegisterAIScriptConditions(AICONDITIONDEF *cond_defs);

f32 AIParamToFloat(AISCRIPTPROCESS *processor, char *param);



f32 AIParamToFloatEx(AIPACKET *packet, AISCRIPTPROCESS *processor, char *param);

// ==== src/legoapi/ai/core/ai_sys_stubs.cpp: declarations ====
#include "editor/edpath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "editor/aieditor_state.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuplane.h"

#include <stdio.h>
#include <float.h>
#include <string.h>


extern "C" f32 (*GetCharacterGoalSpeedFn)(APIOBJECT *);



static void FormationMove(AIGROUP *group, i32 (*move)(AIGROUP *, AIROW *, AIROW *, APIOBJECT *));




extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
extern char * AiLevelPathName;
extern AISCRIPTPROCESS * pSetStateDebugee;
extern SCRIPTPROCESSFIRSTTIMEACTION * ScriptProcessFirstTimeActionFn;
    extern f32 default_path_heighttol;
    extern u8 default_activate_difficulty;
    extern u8 default_min_n_respawns;
    extern u8 default_max_n_respawns;
    extern f32 default_min_t_respawn;
    extern f32 default_max_t_respawn;
    extern f32 default_stagger_start;
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

void AIMoveToDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIRetreatFromDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AICircle(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIWander(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIMoveToDestinationAvoidingCamera(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIMoveDirectlyToDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);

typedef i32 MIDSPECIALMOVE(AISYS *system, AIPACKET *packet, APIOBJECT *object);
extern MIDSPECIALMOVE *MidSpecialMoveFn;

extern "C" i32 AISysSetLevelPath(AISYS *system, char *path_name);
extern "C" void (*GameAISYSRebuildFromEditorDataFn)(AISYS *, VARIPTR *, VARIPTR *);

extern "C" void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);






#include "legoapi/ai/core/ai_pathgeometry.h"











extern "C" {

    AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name);

void AIMoveInstruction(AIPACKET *packet, NUVEC *destination, f32 stopping_distance, AIPATHINFO *path_info, i32 mode,
                           f32 movement_parameter);

void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

AILOCATOR *AIPathFindLocator(AISYS *sys, char *name);

AILOCATORSET *AIPathFindLocatorSet(AISYS *system, char *name);

AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name);

void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);

#include "legoapi/ai/core/ai_sys_stubs_types.h"
    DECOMP_ASSERT(sizeof(RebuildEditorCreature) == 0xac, "editor creature rebuild stride");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, path_check) == 0x38, "editor creature path offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation_area) == 0x6c, "editor creature area offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation) == 0x8f, "editor creature activation offset");

void AISYSRebuildFromEditorData(void);

void AIScriptForceParamReEval(AISCRIPTPROCESS *processor);

char *AIScriptNameFromIx(AISYS *system, i32 index);

} // extern "C"


#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, without the unused object and elapsed time.
static AI_SYS_STUBS_CLONE_ATTR2 AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions,
                                                                           AIPACKET *packet,
                                                                           AISCRIPTPROCESS *processor,
                                                                           i32 stack_index)
    __asm__("_ZL30AIScriptProcessStateConditionsP7AISYS_sP11nulisthdr_sP11APIOBJECT_s"
            "P10AIPACKET_sP17AISCRIPTPROCESS_sfi.isra.77");

static AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions, AIPACKET *packet,
                                                   AISCRIPTPROCESS *processor, i32 stack_index);

extern "C" {

void AIScriptProcess(AISYS *system, APIOBJECT *object, AIPACKET *packet, AISCRIPTPROCESS *processor, f32 elapsed);

void AIScriptSetLevelPath(char *path);

void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *buf_end, u32 size);

void AISysCharacterMovement(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);

void AISysCreatureAntinodeInteraction(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *);

void AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

void AISysCreatureInteraction3D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

AIAREA *AISysFindArea(AISYS *sys, char *name);

AIPATH *AISysFindPath(AISYS *sys, char *name);

void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, i32 ground);

u32 AISysGetPathColour(i32 index);

i32 AISysGetPathColourCount(void);

void AISysGetPathPos(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks);

void AISysGetPathPosEx(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks,
                           i32 search_all_paths, f32 *nearest_distance_squared);

void *AISysLoadEx(void *buffer, void *buffer_end, i32 storage_size, void *scene, char *directory, char *name,
                      char *parameter, char *load_directory);

void AISysProcess(AISYS *system, APIOBJECT *player_1, APIOBJECT *player_2);

void AISysProcessCharacter(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed,
                               i32 use_three_dimensions, i32 process_ai);

i32 AISysSetLevelPath(AISYS *system, char *path_name);

void AISysUpdateCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed);

void AddToAIGroup(AIGROUP *group, APIOBJECT *object);

extern f32 AiRndrCullDistance;
void SetAiRndrCullDistance(f32 distance);

extern AIEDITOR_RENDER_STATE * aieditor;
    void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);
void AiRndrLine3d(NURND_VERTEX3D *vertices, NUMTL *, NUMTX *matrix);
void AiRndrLine3dDbg(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 colour);

void AiSysOnlyUsePakFile(i32 enabled);

void AiSysSetStateDebugee(AISCRIPTPROCESS *processor);

void AiSysUsePackFile(i32 enabled);

void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out);

AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);

void DestroyAIGroup(AIGROUP *group);

void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result);

void FollowAPIObject(APIOBJECT *object, APIOBJECT *target, i32 flags, f32 movement_parameter);

    typedef void AIEDITORCALLBACK(void);
    typedef void AIEDITORREBUILDCALLBACK(AISYS *, VARIPTR *, VARIPTR *);

extern AIEDITORCALLBACK * AIPathDeletedFn;
extern AIEDITORPATHNODECALLBACK * AIPathNodeDeletedFn;
extern AIEDITORPATHNODECALLBACK * AIPathNodeMovedFn;
extern AIEDITORREBUILDCALLBACK * GameAISYSRebuildFromEditorDataFn;
extern AIEDITORCALLBACK * GameAISaveFn;

void InitFn_AIPathDeleted(AIEDITORCALLBACK *function);

void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);

void InitFn_AIPathNodeMoved(AIEDITORPATHNODECALLBACK *function);

void InitFn_GameAISYSRebuildFromEditorData(AIEDITORREBUILDCALLBACK *function);

void InitFn_GameAISave(AIEDITORCALLBACK *function);

void InitFn_GameParamToFloat(GAMEPARAMTOFLOAT *function);

void InitFn_ScriptProcessFirstTimeAction(SCRIPTPROCESSFIRSTTIMEACTION *function);

void LEGO_AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                         f32 delta_time);

void *QueryLocalMessage(void *context, void *message);

} // extern "C"

// ==== src/editor/area_editor.cpp: all remaining content ====
#include "decomp.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nuqfnt.h"
#include <string.h>
#include <stdio.h>

struct nupad_s;

DECOMP_ASSERT(sizeof(EDAIAREA_s) == 0x44, "area editor record size");


extern "C" void *ed_fnt;
extern "C" void aieditor_ClearMainMenu();
extern "C" void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
extern "C" void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
extern "C" void SetAiRndrCullDistance(f32);
extern "C" f32 AiRndrCullDistance;
extern "C" i32 aidata_version;
extern "C" i32 AIEDITOR_ROUTES;
void DrawAreaBox(NUVEC *, NUVEC *, i32, i32);
void DrawAreaCylinder(NUVEC *, NUVEC *, i32);
extern "C" void pathEditorDrawPaths();
extern "C" void creatureEditor_RenderAllCreatures();
extern "C" void antinodeEditorDrawAntinodes();
extern "C" void locatorEditorDrawLocators();

// Item colours shared by the AI editors.






__attribute__((optimize("no-tree-loop-optimize"))) void areaEditor_Enter();

extern "C" void areaEditorSaveData();

extern "C" void areaEditorDrawAreas();

void areaEditor_Render(i32 x, i32 y, f32, f32);





eduimenu_s *areaEditor_Process(nupad_s *pad);

// ==== src/editor/edpath.cpp: all remaining content ====
#include "decomp.h"
#include "editor/edpath.h"
#include "editor/path_connections.h"
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <stdint.h>

#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "globals.h"
extern "C" {
    extern void *ed_fnt;
    extern i32 AIEDITOR_PATHS;
    extern i32 AIEDITOR_ROUTES;
    extern i32 AIEDITOR_LOCATORS;
    extern i32 AIEDITOR_CREATURES;
    extern i32 aidata_version;
    extern i32 near_clip_at_cursor;
    extern f32 default_path_heighttol;
    extern f32 aiEditor_DrawYOffset;
    void AiRndrLine3d(NURND_VERTEX3D *, numtl_s *, NUMTX *);
    void creatureEditor_PathNodeMoved(EDAIPATHNODE_s *);
    void locatorEditor_PathNodeMoved(EDAIPATHNODE_s *);
    void creatureEditor_PathDeleted(EDAIPATH_s *);
    void locatorEditor_PathDeleted(EDAIPATH_s *);
    void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *);
    void locatorEditor_PathNodeDeleted(EDAIPATHNODE_s *);
    void creatureEditor_RenderAllCreatures(void);
    void areaEditorDrawAreas(void);
    void locatorEditorDrawLocators(void);
    void antinodeEditorDrawAntinodes(void);
    void pathEditorDrawPaths(void);
    void LocaledbitsDrawCircleXY(NUVEC *, f32, u32, i32, i32);
    void LocaledbitsDrawSolidCircleXY(NUVEC *, f32, f32, f32, u32, i32, i32);
    extern void (*AIPathDeletedFn)(void);
    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
    void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSolidPathDisplayToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbDrawAllToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbShowCreaturesToggle(eduimenu_s *, eduiitem_s *, u32);
    void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *, u32);
extern NUVEC edpath_addoffset;
extern char *(*EdGetCnxFlagNames)(u32);
extern f32 default_path_node_radius;
    extern char *(*SpecialRouteCharacterNameFn)(u8);
}
DECOMP_ASSERT(sizeof(AIPATHCNXTYPE_s) == 0x4c, "editor path connection type size");
DECOMP_ASSERT(offsetof(AIPATHCNXTYPE_s, flags) == 0x48, "editor path connection options offset");

extern "C" void aieditor_ClearAllPathCnxTypes(void);

extern "C" void aieditor_RegisterDefaultPathCnxTypes(void);

extern "C" void aieditor_RegisterPathCnxType(const char *name, u32 connection_flag, void *context, u32 flags);

void pathEditorDrawConnectionInfo(nuvec_s *position, float radius, nuvec_s *other, u32 flags, i32 colour);

// Retail splits the null checks into each caller and keeps the body as a
// partial-inlining clone; its connection searches are fully peeled.
static __attribute__((noinline, noclone, optimize("peel-loops"))) void DestroyAIPathNodeBody(EDAIPATHNODE_s *,
                                                                                            EDAIPATH_s *)
    __asm__("_ZL17DestroyAIPathNodeP14EDAIPATHNODE_sP10EDAIPATH_s.part.42");
static void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *);
extern "C" void aieditor_ClearMainMenu(void);
extern "C" AIEDITORPATHNODECALLBACK *AIPathNodeMovedFn;


struct EDAISHAREDPATHNODE_s;
struct AIPATH_s;
struct eduimenu_s;
struct eduiitem_s;

DECOMP_ASSERT(offsetof(EdUiNameInputItem, name) == 0x4c, "editor name input offset");
DECOMP_ASSERT(offsetof(EdUiNameInputItem, max_name_length) == 0x15a, "editor name input limit offset");

void pathEditorDrawNode(NUVEC *position, f32 radius, f32 lower_height, f32 upper_height, u32 colour, numtl_s *material,
                        i32 segments, i32 solid);







static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *, u32);







static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *item, u32);


static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *menu, eduimenu_s *);




static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *menu, eduimenu_s *);

static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *menu, eduimenu_s *);



static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *, eduimenu_s *);
static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *, u32);




static __used__ __attribute__((optimize("peel-loops"))) void routeEditor_cbRenameRoute(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *, eduimenu_s *);

static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *item, u32);


static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *menu, eduimenu_s *);

static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *menu, eduimenu_s *);

extern "C" {

void pathEditorDrawPaths(void);

void pathEditorSaveData(AIPATHSYS_s *system);

void pathEditor_CalcNodeIXs(void);

EDAIPATH_s *pathEditor_GetPath(const char *name);

void pathEditor_OnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *result, EDAIPATH_s *path, f32 tolerance);
void pathEditor_QuickOnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *previous, EDAIPATHCHECK_s *result);
void pathEditor_UpdateNodesOnPlatforms(void);

} // extern "C"

extern "C" void pathEditor_CalcNodeIXs(void);
extern "C" EDAIPATH_s *pathEditor_GetPath(const char *name);

void pathEditor_Enter(void);

void pathEditor_Render(i32 x, i32 y, float x_scale, float y_scale);


static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) __asm__(
    "_ZL22routeEditor_AddToRouteP14EDAIPATHNODE_sS0_.part.7");

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other);

eduimenu_s *routeEditor_Process(nupad_s *pad);


static __used__ void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *shared);

static void DestroyAIPathNodeBody(EDAIPATHNODE_s *node, EDAIPATH_s *path);




eduimenu_s *pathEditor_Process(nupad_s *pad);

// Retail keeps an unreferenced partial-inlining clone of pathEditorSetCamPos:
// it moves the editor camera to a position at the cursor's height.
#if defined(__i386__)
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone, regparm(1)))
#else
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static PATHEDITOR_SETCAMPOS_BODY_ATTR void pathEditorSetCamPosBody(nuvec_s *position)
    __asm__("_ZL19pathEditorSetCamPosP7nuvec_si.part.43");
static void pathEditorSetCamPosBody(nuvec_s *position);

// ==== src/editor/aieditor.cpp: all remaining content ====

#include "decomp.h"
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "editor/path_connections.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "globals.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuspecial.h"

#include <string.h>
#include <stdio.h>
#include <float.h>

extern "C" {
extern i32 aidata_version;
}

extern "C" void aieditor_SetMode(i32 mode);
extern "C" void AISYSRebuildFromEditorData(void);
extern "C" i32 aieditor_Register(const char *, void (*)(), void (*)(), void (*)(), void (*)());
extern "C" void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *, u32);
extern "C" f32 AITerrShadow(NUVEC *, f32, f32, i32);
extern "C" i32 AITerrShadowOnPlatform(void);
extern "C" i32 near_clip_at_cursor;
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void areaEditorSaveData(void);
extern "C" void locatorEditorSaveData(AIPATHSYS_s *);
extern "C" void creatureEditorSaveData(AIPATHSYS_s *);
extern "C" void antinodeEditorSaveData(void);
extern "C" void (*GameAISaveFn)(void);

void pathEditor_Enter();
void pathEditor_Render(i32, i32, f32, f32);
eduimenu_s *routeEditor_Process(nupad_s *);
void routeEditor_Render(i32, i32, f32, f32);
void areaEditor_Enter();
eduimenu_s *areaEditor_Process(nupad_s *);
void areaEditor_Render(i32, i32, f32, f32);
void locatorEditor_Enter();
eduimenu_s *locatorEditor_Process(nupad_s *);
void locatorEditor_Render(i32, i32, f32, f32);
void creatureEditor_Enter();
eduimenu_s *creatureEditor_Process(nupad_s *);
void creatureEditor_Render(i32, i32, f32, f32);
void antinodeEditor_Enter();
eduimenu_s *antinodeEditor_Process(nupad_s *);
void antinodeEditor_Render(i32, i32, f32, f32);

extern "C" {
extern i32 AIEDITOR_PATHS;
extern i32 AIEDITOR_ROUTES;
extern i32 AIEDITOR_AREAS;
extern i32 AIEDITOR_LOCATORS;
extern i32 AIEDITOR_CREATURES;
extern i32 AIEDITOR_ANTINODES;
}

extern aieditor_settings_s aieditorsettings;
extern "C" {
    extern void *ed_fnt;
}



#if defined(__i386__)
#define EDANTINODE_REGPARM1 __attribute__((regparm(1)))
#else
#define EDANTINODE_REGPARM1
#endif
static __attribute__((noinline,
                      force_align_arg_pointer)) EDANTINODE_REGPARM1 EDANTINODE_s *CreateAntinode(nuvec_s *position);

__attribute__((optimize("no-tree-loop-optimize"))) void antinodeEditor_Enter();







static __attribute__((force_align_arg_pointer)) EDANTINODE_REGPARM1 void
antinodeEditor_AntinodeMoved(EDANTINODE_s *node);


eduimenu_s *antinodeEditor_Process(nupad_s *pad);



extern "C" {

    typedef void AIEDITORMOVEPLAYERS(NUVEC *position);
extern AIEDITORMOVEPLAYERS * AIEditorMovePlayersFn;

    void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);

void InitFn_AIEditorMovePlayers(AIEDITORMOVEPLAYERS *function);

eduimenu_s *aieditor_AddMessage(char *title, char *message);

void aieditor_ClearMainMenu(void);

void aieditor_Enter(AISYS_s *system, NUVEC *camera_position, void *context, void *owner, void *extra_1,
                        void *extra_2, void *extra_3);

void aieditor_Init(AIEDITOR_RENDER_STATE *state, void **external_display_a, void **external_display_b);

void aieditor_Leave(void);

i32 aieditor_Proc(f32 menu_delta_time, nupad_s *pad, f32 frame_delta_time, u32 exit_buttons);

i32 aieditor_Register(const char *name, void (*enter)(), void (*callback_24)(), void (*callback_28)(),
                          void (*leave)());

void AISysSetPathCylinderCheck(i32 enabled);

void aieditor_Render(f32 x_scale, f32 y_scale);

void aieditor_Reset(void);

i32 aieditor_Save(void);

__attribute__((force_align_arg_pointer)) void aieditor_SetCurrentScript(char *name,
                                                                            const AIEditorScriptSelection *selection);

void aieditor_SetMode(i32 mode);

void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);

void aieditor_cbDrawAllToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);

void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);

void aieditor_cbSave(eduimenu_s *parent, eduiitem_s *, u32);

void aieditor_cbShowCreaturesSetToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbShowCreaturesToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbSolidPathDisplayToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cvSelectEditorMode(eduimenu_s *parent, eduiitem_s *, u32);

} // extern "C"

// ==== src/editor/creature_editor.cpp: all remaining content ====

#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/edpath.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nurndr.h"
#include <float.h>
#include <stdio.h>
#include <string.h>
#pragma GCC optimize("O2", "omit-frame-pointer")
struct EDCREATURE_s;
struct AIPATHSYS_s;

DECOMP_ASSERT(offsetof(CreatureEditorRecord, valid_positions) == 0x54, "creature valid position mask offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, position) == 0x28, "creature position offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_name) == 0x18, "creature script name offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_params) == 0x70, "creature script params offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, angle) == 0x34, "creature angle offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, character_type) == 0x58, "creature character type offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, path) == 0x3c, "creature path offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, set) == 0x5a, "creature set offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, flags) == 0x68, "creature flags offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation_area) == 0x6c, "creature activation area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, trigger_area) == 0x80, "creature trigger area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation) == 0x8f, "creature activation offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, view_distance) == 0x9c, "creature view distance offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, negative_min_view_height) == 0xa8, "creature min view height offset");
DECOMP_ASSERT(sizeof(CreatureEditorRecord) == 0xac, "editor creature record size");





extern i32 creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32);
extern i32 creatureEditor_IsSelectable(EDCREATURE_s *);
extern "C" EDLOCATOR_s *locatorEditor_GetNearest(i32);
extern "C" void AiRndrLine3d(NURND_VERTEX3D *, struct numtl_s *, struct numtx_s *);
extern "C" f32 aieditor_y_tolerance;
extern "C" void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);
extern "C" i32 AIEDITOR_CREATURES;
extern "C" void (*ClearAICreaturesFn)();
extern "C" u8 default_ngroup;
extern "C" u8 default_nacross;
extern "C" f32 default_xspacing;
extern "C" f32 default_zspacing;
extern "C" f32 default_stagger_start;
extern "C" u8 default_activate_difficulty;
extern "C" u8 default_min_n_respawns;
extern "C" u8 default_max_n_respawns;
extern "C" f32 default_min_t_respawn;
extern "C" f32 default_max_t_respawn;
extern "C" EDAIPATH_s *pathEditor_GetPath(const char *);
extern "C" i32 aisys_maxnumcreaturesets;
extern "C" void aieditor_cbShowCreaturesSetToggle(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_stagger_start(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_maxviewheight(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_minviewheight(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSelectRespawnLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetRespawnLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetTriggerArea(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_difficulty(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetActivation(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetAreaActivation(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_assigntoset(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbCancelMenu(eduimenu_s *, eduimenu_s *);
static void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbResetParams(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSelectTriggerArea(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSelectLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetScriptParam(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *, u32);

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM2 __attribute__((regparm(2)))
#else
#define CREATURE_EDITOR_REGPARM2
#endif



__attribute__((optimize("O3"))) void creatureEditor_Enter();

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM1 __attribute__((regparm(1)))
#else
#define CREATURE_EDITOR_REGPARM1
#endif
static __used__
    __attribute__((always_inline)) CREATURE_EDITOR_REGPARM1 void creatureEditor_Updated(EDCREATURE_s *creature);


static __used__ void creatureEditor_cbCancelMenu(eduimenu_s *menu, eduimenu_s *);







static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbResetParams(eduimenu_s *menu,
                                                                                           eduiitem_s *, unsigned int);


static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbVisionMenu(eduimenu_s *parent,
                                                                                          eduiitem_s *, unsigned int);



static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *item,
                                                                                         unsigned int);

static __used__ void creatureEditor_cbSetScriptParam(eduimenu_s *menu, eduiitem_s *item, unsigned int);


static __used__ void creatureEditor_cbSetActivation(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetAreaActivation(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetLocator(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSelectRespawnLocator(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbSelectLocator(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbSelectTriggerArea(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbSetRespawnLocator(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_assigntoset(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetTriggerArea(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_difficulty(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_minviewheight(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_maxviewheight(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_stagger_start(eduimenu_s *, eduiitem_s *item, unsigned int);

extern "C" {

extern f32 aieditor_y_tolerance;

__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditorSaveData(AIPATHSYS_s *system);

__attribute__((optimize("O2", "omit-frame-pointer"))) EDCREATURE_s *creatureEditor_GetNearest(i32 use_radius);

__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathDeleted(EDAIPATH_s *path);

__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *node);

__attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_PathNodeMoved(EDAIPATHNODE_s *node);

__attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_RenderAllCreatures(void);

} // extern "C"

__attribute__((optimize("O2"), force_align_arg_pointer)) eduimenu_s *creatureEditor_Process(nupad_s *pad);

// ==== src/editor/locator_editor.cpp: all remaining content ====
#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include <string.h>
#include <stdio.h>
#include <float.h>

extern "C" {
    extern void *ed_fnt;
    extern f32 aiEditor_LocatorWidth;
    struct EDCREATURE_s;
    EDCREATURE_s *creatureEditor_GetNearest(i32);
    extern i32 AIEDITOR_ROUTES;
    void aieditor_ClearMainMenu();
    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
    void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
    EDAIPATH_s *pathEditor_GetPath(const char *);
    void areaEditorDrawAreas();
    void pathEditorDrawPaths();
    void antinodeEditorDrawAntinodes();
    void creatureEditor_RenderAllCreatures();
    void AiRndrLine3d(NURND_VERTEX3D *, struct numtl_s *, struct numtx_s *);
    void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);
    extern void (*ClearAICreaturesFn)();
    extern u8 default_ngroup;
    extern u8 default_nacross;
    extern f32 default_xspacing;
    extern f32 default_zspacing;
    extern f32 default_stagger_start;
    extern u8 default_activate_difficulty;
    extern u8 default_min_n_respawns;
    extern u8 default_max_n_respawns;
    extern f32 default_min_t_respawn;
    extern f32 default_max_t_respawn;
    extern i32 aidata_version;
}

extern "C" {
extern u8 default_ngroup;
extern u8 default_nacross;
extern f32 default_xspacing;
extern f32 default_zspacing;
extern f32 default_stagger_start;
}

void DrawLocator(nuvec_s *, f32, i32, i32);
static void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *, eduimenu_s *);
static void locatorEditor_cbCancelRenameMenu(eduimenu_s *, eduimenu_s *);
static void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *);
#if defined(__i386__)
static void __attribute__((regparm(1))) DestroyLocator(EDLOCATOR_s *);
static unsigned int __attribute__((regparm(2))) AddLocatorToSet(EDLOCATORSET_s *, EDLOCATOR_s *, EDLOCATOR_s *);
#else
static void DestroyLocator(EDLOCATOR_s *);
static unsigned int AddLocatorToSet(EDLOCATORSET_s *, EDLOCATOR_s *, EDLOCATOR_s *);
#endif
static __used__ void locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *, u32);


static __used__ __attribute__((optimize("no-tree-vectorize"))) void
locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbCancelRenameMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *menu, eduimenu_s *);
static __used__ void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *);

static __used__ __attribute__((optimize("no-tree-vectorize"))) void DestroyLocator(EDLOCATOR_s *locator);

#if defined(__i386__)
#define LOCATOR_SET_BODY_ATTR __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2)))
#else
#define LOCATOR_SET_BODY_ATTR __attribute__((noinline, optimize("no-tree-vectorize")))
#endif
static LOCATOR_SET_BODY_ATTR unsigned int
AddLocatorToSetBody(EDLOCATORSET_s *set, EDLOCATOR_s *locator,
                    EDLOCATOR_s *before) __asm__("_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.part.5");



static LOCATOR_SET_BODY_ATTR unsigned int AddLocatorToSetBody(EDLOCATORSET_s *set, EDLOCATOR_s *locator,
                                                              EDLOCATOR_s *before);

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before);

#if defined(__i386__)
static __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2))) unsigned int
#else
static __attribute__((noinline, optimize("no-tree-vectorize"))) unsigned int
#endif
AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator) __asm__(
    "_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.constprop.131");

static unsigned int AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator);

extern "C" {

void locatorEditorDrawLocators(void);

void locatorEditorSaveData(AIPATHSYS_s *path_system);

EDLOCATOR_s *locatorEditor_GetNearest(i32 use_width);

void locatorEditor_PathDeleted(EDAIPATH_s *path);

void locatorEditor_PathNodeDeleted(EDAIPATHNODE_s *node);

void locatorEditor_PathNodeMoved(EDAIPATHNODE_s *node);

} // extern "C"

void locatorEditor_Enter(void);

void locatorEditor_Render(i32 x, i32 y, float x_scale, float y_scale);

__attribute__((optimize("no-tree-vectorize"))) eduimenu_s *locatorEditor_Process(nupad_s *pad);

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

extern f32 DEFENDTIME;
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







#include "legoapi/items/objects/gameobjects_types.h"






// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

































































extern "C" {
extern i32 nbaddies_can_see_players;
}


























































































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





























extern f32 draw_attention_distance;
extern i32 test_new_interaction;
extern i32 debug_ignore_players;
extern i32 WALKSTEALTH;
extern i32 active_goody_count;
extern f32 timetogetlos;

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

void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}










































extern "C" {
extern i32 party_under_cover;
}


































extern "C" {
extern ed_module_s edTimingDesc;
}




































































extern "C" {
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
}





// ==== src/gameapi/ai/aisys/aistate.cpp: all remaining content ====
#include "gameapi/ai/aisys/aisys.h"

#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nustring.h"

AISTATE *AIStateFind(char *name, AISCRIPT *script);

// ==== src/legoapi/gizmo/gizmos/gizmos_gizactions.cpp: declarations ====
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspline.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/fx/parts.h"

i32 Action_SetState(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params, i32 param_count,
                    i32 is_first_time, float);

f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
void AISysGetPathPos2(AISYS_s *, NUVEC *, AIPATHINFO_s *, NUVEC *, AIPATH_s *, i32);

i32 Action_CameraCut(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                     i32 first_time, f32);

i32 Action_UsePanel(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                    i32 first_time, f32 elapsed);

i32 Action_UseTechno(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                     i32 first_time, f32 elapsed);

i32 Action_EndCameraCut(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params, i32 param_count,
                        i32 first_time, float);

i32 Action_PullLever(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                     i32 first_time, f32 elapsed);

i32 Action_PlayCutScene(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count, i32 first_time,
                        float);

i32 Action_FollowPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                        i32 first_time, float);

i32 Action_SetVisibility(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count, i32 first_time,
                         float);

i32 Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float);

i32 Action_UseTriggerSet(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                         i32 param_count, i32 first_time, f32 elapsed);



i32 Action_ReleaseLocator(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, float);





struct GIZSPINNER_s;
void ClearSpecialMove(GameObject_s *);
extern i32 spinner_gizmotype_id;
extern u32 GAMEPAD_SPECIAL, GAMEPAD_JUMP, GAMEPAD_TOGGLERIGHT;
extern f32 ai_moveradius;

i32 Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32,
                            f32 elapsed);

i32 Action_GetLocatorFromSet(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                             i32 param_count, i32 first_time, float);

i32 Action_AssignLocatorInSet(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                              i32 first_time, float);

namespace {
#include "legoapi/gizmo/gizmos/gizmos_gizactions_types.h"

} // namespace

// ==== src/legoapi/actions/combat/fighting.cpp: declarations ====
#include "decomp.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/actions/combat/hits.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/misc/utilities.h"

#include <stdlib.h>

void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
void Arcade_Kill(i32, i32);
i32 CannotKill(GameObject_s *);
void Player_ClearContext(GameObject_s *, i32);
void Player_ResetContexts(PLAYERPACKET_s *);
i32 qrand();
extern "C" void KillPart(PART_s *, i32);

extern void (*Punch_HitHoldFn)(GameObject_s *, GameObject_s *);
extern i32 (*Punch_GetDamageFn)(GameObject_s *, GameObject_s *);
extern void (*Punch_HitExtraCodeFn)(GameObject_s *, nuvec_s *);
extern BLADE_s BladeTab[4];

void DeflectPart(PART_s *part, GameObject_s *object, float speed, float gravity, i32 deflect, i32 debris);

bool IsDownSwipe(NuVec2 const &start, NuVec2 const &end);





DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");


void ForceNextShootTarget(MechObjectInterface &target);

i32 ObjZappedBlue(GameObject_s *object);



// ==== src/legoapi/ai/game/aipathcnxhelper.cpp: declarations ====
#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"

#include <string.h>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"


extern "C" {
    extern f32 aiEditor_DrawYOffset;
    void LocaledbitsDrawCircleXY(NUVEC *, f32, u32, i32, i32);
    void LocaledbitsDrawSolidCircleXY(NUVEC *, f32, f32, f32, u32, i32, i32);
}














// ==== src/legoapi/ai/core/ai_sys.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>



// ==== src/legoapi/ai/game/gameantinode.cpp: declarations ====
#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/legoapi_types.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/antinode_editor_private.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include <string.h>

extern "C" {
extern u64 GameAntinode_grid[64];
    extern f32 default_path_heighttol;
    extern i32 AIEDITOR_ANTINODES;
    extern i32 aidata_version;
    AIANTINODE_s *AIAntinodeCreateSingleFrame(NUVEC *, f32);
    void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);
}
i32 ViewCamGetMode();
NUVEC *ViewCamGetTgt();
void GameAntinode_FindGridPosition(WORLDINFO_s *, NUVEC *, f32, f32, u8 *, u8 *, u8 *, u8 *);
void GameAntinode_UnregisterAntiNode(GAMEANTINODESYS_s *, GAMEANTINODE_s *);

void *GameAntnode_CreateSys(WORLDINFO_s *world, variptr_u *buf, variptr_u *buf_end, i32 count);



void GameAntinode_FindGridPosition(WORLDINFO_s *world, NUVEC *position, f32 radius_x, f32 radius_z, u8 *min_x,
                                   u8 *min_z, u8 *max_x, u8 *max_z);

GAMEANTINODE_s *GameAntinode_RegisterAntiNode(GAMEANTINODESYS_s *system, NUVEC *position, f32 radius, f32 extent_x,
                                              f32 extent_z, u16 angle, i32 shape, f32 duration);

GAMEANTINODE_s *GameAntinode_RegisterAntiNodeUsingData(GAMEANTINODESYS_s *system, NUVEC *position, u16 angle,
                                                       GAMEANTINODEDATA_s *data, f32 duration, i32 disabled);


void GameAntinode_UnregisterAntiNode(GAMEANTINODESYS_s *system, GAMEANTINODE_s *node);




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


extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);


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
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);


void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern char *ASCII_UP;
extern f32 MENUTEXTSCALE;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
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
extern "C" i32 PANELOFF;
extern i32 noscenespecials;
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state

// Graphics loading flags


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


// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"


void DrawCables();


void DrawAreaBox(nuvec_s *position, nuvec_s *size, i32 rotation, i32 colour);


extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour);


i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];


void DrawParallax(nuhspecial_s *special);






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



extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);



void DrawPauseFade();


void DrawRippleSet(ripple_set_s *set);










void DrawCutBorders(i32 widescreen);

void DrawExplosions();



i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();


void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);


void DrawAreaCylinder(nuvec_s *centre, nuvec_s *size, i32 colour);


void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
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
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);



i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);


i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];


extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);







void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();


extern i32 dco_locatorposonly;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern f32 DropInOutScale(GameObject_s *object);
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}


i32 DrawGameObjectsProcess();


void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);


extern void DrawMenuEntry(MENU_s *, char *);






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





void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;


#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);







extern void (*BackDrop_AlphaFn)(float *);




void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== src/legoapi/actions/character/starwars.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"

struct AIPACKET_s;
struct APIOBJECT_s;
struct AISYS_s;
struct AIGROUP_s;
struct nufpar_s;

typedef i32 (*MIDSPECIALMOVEFN)(AISYS_s *, AIPACKET_s *, APIOBJECT_s *);

extern PREPARINGSPECIALMOVEFN PreparingForSpecialMoveFn;
extern MIDSPECIALMOVEFN MidSpecialMoveFn;

extern "C" {

void InitFn_MidSpecialMove(MIDSPECIALMOVEFN function);

void InitFn_PreparingForSpecialMove(PREPARINGSPECIALMOVEFN function);

} // extern "C"

// ==== src/editor/antinode_editor.cpp: all remaining content ====
#include "decomp.h"
#include "globals.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include <string.h>

extern "C" {
    extern i32 AIEDITOR_ANTINODES;
    extern i32 aidata_version;
}

extern "C" {

void antinodeEditorDrawAntinodes(void);

void antinodeEditorSaveData(void);

void antinodeEditor_UpdateAntiNodesOnPlatforms(void);

} // extern "C"

// ==== src/editor/edtoolsall.cpp: declarations ====
#include "decomp.h"
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include <stdio.h>
#include <string.h>

i32 creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32);
i32 creatureEditor_IsSelectable(EDCREATURE_s *);

extern "C" {
    extern void *ed_fnt;
    extern i32 AIEDITOR_ANTINODES;
extern i32 aisys_maxnumcreaturesets;
    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
    void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *, u32);
    void pathEditorDrawPaths();
    void creatureEditor_RenderAllCreatures();
    void areaEditorDrawAreas();
    void locatorEditorDrawLocators();
    void antinodeEditorDrawAntinodes();
    void AiRndrLine3d(NURND_VERTEX3D *, numtl_s *, NUMTX *);
}

void routeEditor_Render(i32 x, i32 y, float xscale, float yscale);



void antinodeEditor_Render(i32 x, i32 y, float xscale, float yscale);

void creatureEditor_Render(i32 x, i32 y, float xscale, float yscale);

#include "editor/edtoolsall_types.h"

__attribute__((force_align_arg_pointer)) i32 creatureEditor_CalculatePos(EDCREATURE_s *creature, i32 index,
                                                                         nuvec_s *position, i32 check_path);

i32 creatureEditor_IsSelectable(EDCREATURE_s *creature);

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

extern i16 TerrImpact;
extern i32 terrhitflags;
extern i32 TERRAINMASK_NONWEAPON;
extern i32 TERRAINMASK_NONDROID;
extern NUVEC TerrImpactPos;
extern NUVEC TerrImpactNormal;
extern NUVEC ShadNorm;
extern NUVEC ShadRoofNorm;
extern NUVEC EShadNorm;
extern NUVEC EShadRoofNorm;
extern f32 ShadRoofY;
extern f32 EShadRoofY;
extern TerrainLastImpact_s TerrLastImpact;
extern TERRSET * CurTerr;
// Runtime-selected groups appended after the fixed terrain allocation.
extern i32 WallSplinesOnly;
extern "C" {
extern i32 IgnoreWallSplines;
}
#include "legoapi/render/core/terrain_stubs_types.h"

void TerrainSkinAllocate(terrsitu_s *terrain_group);

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

i32 HitTerrain(void);
void RayImpact(NUVEC *);

extern "C" {
    i32 DebrisCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisCollisionCheckScaleY(NUVEC *, f32, f32);
    i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisTorusCollisionCheckScaleY(NUVEC *, f32, f32);
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 EDPP_SCALE_TYPES;
    extern debinftype *effecttypes;
    extern debscale_s *debscale;
    extern f32 panelglobaltime;
    extern f32 globaltime;
    extern f32 renderpanelglobaltime;
    extern f32 renderglobaltime;
    extern f32 timeincrement;
    extern i32 globalframes;
    extern i32 update_debris_enabled;
    extern i32 debris_setup_called;
    extern usize debris_trash_space;
    extern usize debris_trash_size;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern debris_chunk_control_s *debris_chunk_controls;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debkeydatatype_s *debris_keystack;
    extern u32 *spherecolldata;
    extern u32 *toruscolldata;
    extern i32 numspherecolldata;
    extern i32 numtoruscolldata;
    extern NUMTL *DebMat[10];
    extern i32 debris_suspended;
    void DebrisReScale(i32, f32);
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
    extern "C++" {
        void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
        void DebrisCleanUpDmaDebTypeTables(void);
        void DebrisProcessAllocation(void);
        void DebrisProcessControlChunks(i32);
        void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
    }
}

void DebrisFreeOldestDmaDebTypeTable();

// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain);
extern "C" void TerrSetPlatScanDist(f32 dist);

extern "C" void TerrainPlatformNewUpdate(void);
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page);

void Particles_Load(WORLDINFO *world, char **debris_name, i32 count, i32 flags);

extern "C" {

void AITerrInit(void);

f32 AITerrShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

    i32 NewShadowOnPlatform(void);

i32 AITerrShadowOnPlatform(void);

i32 CheckForPlatInst(i32 instance);


i32 CreateScaledEffect(i32 effect_index, f32 requested_scale);

i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);

void DebFreeAllCreatedEffects(void);

void DebFreeAllDMADebTablesInstantly(void);

void DebFreeAllPanelEffects(void);

void Debris(i32 update_panel_time);

i32 DebrisAllCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

i32 DebrisCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

} // extern "C"



extern "C" {

void DebrisEmitterMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);

void DebrisEmitterOrientationMtx(i32 handle, NUMTX *source);

void DebrisEmitterPos(i32 handle, f32 x, f32 y, f32 z);

i32 DebrisFindAllOfType(i32 effect_index, NUVEC *positions, i32 *handles, i32 capacity, i32 skip, i32 active_only);

i32 DebrisGetConeProperties(i32 handle, NUVEC *origin, NUVEC *end, f32 *start_radius, f32 *end_radius,
                                NUVEC *direction, f32 *length, f32 *speed);

f32 DebrisGetDuration(i32 effect_index);

char *DebrisGetName(i32 effect_index);

i32 DebrisGetParticleCount(i32 handle);

i32 DebrisGetRingProperties(i32 handle, NUVEC *origin, NUVEC *direction, f32 *radius);

void DebrisOrientation(i32 handle, i16 z, i16 y);

void DebrisOrientationMtx(i32 handle, NUMTX *source);

void DebrisParticleMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisPopulateInstance(i32 handle, f32 duration);

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);

void DebrisProcessTimeSlip(void);

i32 DebrisQueryPriority(i32 effect_index);

void DebrisReScale(i32 effect_index, f32 scale);

void DebrisReflectionOrientation(i32 handle, i16 x, i16 y, f32 plane, f32 scale);

extern NUVEC * CutoffCameraVec;

void DebrisRegisterCutoffCameraVec(void *position);

void DebrisReserveTrashableSpace(VARIPTR *buffer, VARIPTR, i32 chunk_count, i32 glass_chunk_count);

void DebrisResetTimers(void);

void DebrisSetDetailLevels(i32 handle, i32 detail_levels);

void DebrisSetDrawFlag(i32 handle, i8 draw_flag);

void DebrisSetFacing(i32 handle, u8 enabled, i16 x_angle, i16 y_angle);

void DebrisSetGroupID(i32 handle, i16 group);

void DebrisSetPriority(i32 handle, u16 priority, u8 automatic);

extern i32 debris_render_group;
void DebrisSetRenderGroup(i32 group);

void DebrisSetRoomID(i32 handle, nugscn_s *scene);

void DebrisSetTrigger(i32 handle, i32 first, i32 second, f32 third);

void DebrisSetUserData(i32 handle, void *user_data);

void DebrisShift(NUMTX *transform);

void DebrisStartOffset(i32 handle, f32 offset);

void DebrisStatusAlwaysOff(i32 *handle);

void DebrisStatusAlwaysOn(i32 *handle);

void DebrisStatusNormal(i32 *handle);

i32 DebrisTorusCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisTorusCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

void DebrisTrashableSetup(VARIPTR *buffer, VARIPTR *);

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

void DrawPlatform(i32 platform_index);

i32 FindPlatInst(i32 instance);

i32 NewMSituTerrEx(NUVEC *position, void *data, NUVEC *minimum, NUVEC *maximum, f32 radius);

i32 NewPlatInst(void *object, i32 instance);

i32 NewPlatInstMSitu(void *object, i32 source);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

    void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

void NewTerrain(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius, f32 collision_radius,
                    i32 scan_flags);

void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

void PartTerrInit(void);

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstCenter(i32 index, NUVEC *position);

i32 PlatInstGetHit(i32 index);

void PlatInstRotate(i32 index, i32 rotate);

void PlatSkinMemRigister(void *start, void *end, i32 maximum);

void PlatSkinMemReset(void);

i32 PlatInstSkinRegisterEx(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags,
                               TERRSET *source);

i32 PlatInstSkinRegister(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags);

void PlatOnOff(i32 index, i32 enabled);

void *PlatSkinEndReigster(i32 cache_slots);

void PlatformRemoveCallback(void (*function)(void *));

void PlatformUpdateCallback(void (*function)(void *), void *argument);

void SortDebrisRenderStack(void);

const char *TerrErrorString(i32 error);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

i32 TerrainFreeId(void *id);

TERRSET *TerrainGetCur(void);

TERRAIN_GROUP *TerrainGetModelByInst(i32 instance);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

void UpdateDebrisRenderStackPriority(void);

void *terraininit(i32 level_num, void *buffer, void *buffer_end, i32 options, char *path, void *scene, i32 group);

void UpdatePlatinst(i32 index, void *object);

} // extern "C"

// ==== functions in original address order ====







































extern "C" {
void aieditor_cbDrawAllToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbSolidPathDisplayToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbShowCreaturesToggle(eduimenu_s *, eduiitem_s *item, u32);

void aieditor_cbShowCreaturesSetToggle(eduimenu_s *, eduiitem_s *item, u32);

}






























































extern "C" {
void aieditor_cvSelectEditorMode(eduimenu_s *parent, eduiitem_s *, u32);

}





































































extern "C" {
void SetAiRndrCullDistance(f32 distance);

void AiRndrLine3d(NURND_VERTEX3D *vertices, NUMTL *, NUMTX *matrix);

void AiRndrLine3dDbg(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 colour);

void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);

}









extern "C" {
char *AIScriptNameFromIx(AISYS *system, i32 index);

}













extern "C" {
void AIScriptForceParamReEval(AISCRIPTPROCESS *processor);

}




















extern "C" {
void AIScriptProcess(AISYS *system, APIOBJECT *object, AIPACKET *packet, AISCRIPTPROCESS *processor, f32 elapsed);

}


extern "C" {
void AiSysUsePackFile(i32 enabled);

void AiSysOnlyUsePakFile(i32 enabled);

void AiSysSetStateDebugee(AISCRIPTPROCESS *processor);

}


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

void InitFn_FindAlternativeSpecialObjectFn(AIFINDALTERNATIVESPECIALOBJECT *function);

void InitFn_APIOBJECTFromObjIDFn(APIOBJECTFROMOBJID *function);

void InitFn_GameAILoad(GAMEAILOAD *function);

void InitFn_GameParamToFloat(GAMEPARAMTOFLOAT *function);

void InitFn_ScriptProcessFirstTimeAction(SCRIPTPROCESSFIRSTTIMEACTION *function);

void InitFn_AIActionParseSpeed(AIACTIONPARSESPEED *function);

void InitFn_AIRespawnOnPath(AIRESPAWNONPATH *function);

void InitFn_AIBigJumpToDestination(AIBIGJUMPTODESTINATION *function);

void InitFn_GetViewRange(CHARACTERDISTANCEFN *function);

void InitFn_GetHearDistance(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMaxViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMinViewHeight(CHARACTERDISTANCEFN *function);

extern "C" void AISetPathHeightTol(f32 tolerance);

}





extern "C" {
void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *buf_end, u32 size);

}



extern "C" {
i32 AISysSetLevelPath(AISYS *system, char *path_name);

}



extern "C" {
AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);

void AddToAIGroup(AIGROUP *group, APIOBJECT *object);

void DestroyAIGroup(AIGROUP *group);

void AIScriptSetLevelPath(char *path);

void *AISysLoadEx(void *buffer, void *buffer_end, i32 storage_size, void *scene, char *directory, char *name,
                      char *parameter, char *load_directory);

extern "C" void *AISysLoad(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param);

void AISysProcess(AISYS *system, APIOBJECT *player_1, APIOBJECT *player_2);

extern "C" AIANTINODE *AIAntinodeCreate(NUVEC *position, f32 radius);

extern "C" AIANTINODE *AIAntinodeCreateSingleFrame(NUVEC *position, f32 radius);

extern "C" void AIAntinodeDestroy(AIANTINODE *antinode);

extern "C" void AIAntinodeCullSingleFrame(void);

extern "C" void AIAntinodeMove(AIANTINODE *antinode, NUVEC *position, f32 radius, f32 below, f32 above);

}


extern "C" {
void AISysCreatureAntinodeInteraction(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *);

void AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

void LEGO_AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                         f32 delta_time);

void AISysCreatureInteraction3D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

extern "C" void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);

}


extern "C" {
extern "C" f32 AIPathNodeDistanceToPathNode(AIPATH *path, i32 start_node, i32 destination_node, i32 route_index,
                                            u32 excluded_route_mask);

}


extern "C" {
AILOCATOR *AIPathFindLocator(AISYS *sys, char *name);

}






extern "C" {
AILOCATORSET *AIPathFindLocatorSet(AISYS *system, char *name);

AIAREA *AISysFindArea(AISYS *sys, char *name);

}






extern "C" {
AIPATH *AISysFindPath(AISYS *sys, char *name);

AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name);

}






extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

}



extern "C" {
extern "C" AIPATHCNX *AIPathFindPathCnxFromIX(AISYS *system, AIPATH *path, u8 from_index, u8 to_index);

void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);

extern "C" i32 WithinConnection(AISYS *system, NUVEC *position, AIPATH *path, AIPATHCNX *connection, i32 checks,
                                AIPATHCNX *previous_connection, i32 route, i32 ground, AIPATHINFO *path_info,
                                f32 radius, i32 update_once);

}



extern "C" {
extern "C" void AISysFindRoute(AIPACKET *packet);

extern "C" void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);

}


extern "C" {
void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, i32 ground);

void AISysUpdateCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed);

void AISysGetPathPosEx(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks,
                           i32 search_all_paths, f32 *nearest_distance_squared);

void AISysGetPathPos(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks);

extern "C" void AIFormationFollow(AIPACKET *packet);

void AIMoveInstruction(AIPACKET *packet, NUVEC *destination, f32 stopping_distance, AIPATHINFO *path_info, i32 mode,
                           f32 movement_parameter);

}















extern "C" {
void FollowAPIObject(APIOBJECT *object, APIOBJECT *target, i32 flags, f32 movement_parameter);

}











extern "C" {
void AISysCharacterMovement(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);

void AISysProcessCharacter(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed,
                               i32 use_three_dimensions, i32 process_ai);

extern "C" i32 AISysGetCharacterWaypoint(const AIPACKET *packet, NUVEC *position);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

}


extern "C" {
extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

}


extern "C" {
extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void ResetAIMessageSys(AIMESSAGESYS_s *sys);

extern "C" AIMESSAGESYS_s *CreateAIMessageSys(VARIPTR *cursor, VARIPTR *end, i32 count);

extern "C" void ClearAIMessageSys(AIMESSAGESYS_s *system);

extern "C" AIMESSAGE_s *CheckAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message);

extern "C" f32 GetAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message);

extern "C" void SetAIMessage(AIMESSAGESYS_s *system, char *name, f32 value, AIMESSAGE_s *message);

extern "C" AIMESSAGE_s *QueryAIMessage(AIMESSAGESYS_s *system, AIMESSAGE_s *message);

extern "C" void RemoveAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message);

extern "C" AILOCALMESSAGE_s *FindLocalAIMessage(AISCRIPTPROCESS *processor, char *name);

extern "C" void AddLocalAIMessage(AISCRIPTPROCESS *processor, AILOCALMESSAGE_s *message, char *name);

void *QueryLocalMessage(void *context, void *message);

extern "C" void AILocatorSet_CheckLocatorsStillAssigned(AISYS *system, AILOCATORSET *locator_set);

extern "C" void AILocatorSet_AssignNearestLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                  f32 max_range, NUVEC *position, NUVEC *second_position,
                                                  f32 off_screen_radius, i32 ignore_assigned);

extern "C" void AILocatorSet_AssignFurthestLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                   f32 max_range, NUVEC *position, NUVEC *second_position,
                                                   f32 off_screen_radius, i32 ignore_assigned);

extern "C" void AILocatorSet_AssignRandomLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                 f32 max_range, NUVEC *position, f32 off_screen_radius,
                                                 i32 ignore_assigned);

u32 AISysGetPathColour(i32 index);

i32 AISysGetPathColourCount(void);

void InitFn_GameAISYSRebuildFromEditorData(AIEDITORREBUILDCALLBACK *function);

void InitFn_GameAISave(AIEDITORCALLBACK *function);

void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);

void InitFn_AIPathNodeMoved(AIEDITORPATHNODECALLBACK *function);

void InitFn_AIPathDeleted(AIEDITORCALLBACK *function);

void InitFn_PreparingForSpecialMove(PREPARINGSPECIALMOVEFN function);

void InitFn_MidSpecialMove(MIDSPECIALMOVEFN function);

void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function);

void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function);

i32 aieditor_Register(const char *name, void (*enter)(), void (*callback_24)(), void (*callback_28)(),
                          void (*leave)());

void aieditor_SetMode(i32 mode);

void aieditor_Enter(AISYS_s *system, NUVEC *camera_position, void *context, void *owner, void *extra_1,
                        void *extra_2, void *extra_3);

void AISysSetPathCylinderCheck(i32 enabled);

void aieditor_Render(f32 x_scale, f32 y_scale);

void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result);

extern "C" void areaEditorSaveData();

}



extern "C" {
extern "C" void areaEditorDrawAreas();

void locatorEditorSaveData(AIPATHSYS_s *path_system);

EDLOCATOR_s *locatorEditor_GetNearest(i32 use_width);

void locatorEditor_PathNodeMoved(EDAIPATHNODE_s *node);

void locatorEditor_PathDeleted(EDAIPATH_s *path);

}


extern "C" {
void locatorEditorDrawLocators(void);

extern "C" void aieditor_ClearAllPathCnxTypes(void);

extern "C" void aieditor_RegisterPathCnxType(const char *name, u32 connection_flag, void *context, u32 flags);

extern "C" void aieditor_RegisterDefaultPathCnxTypes(void);

void aieditor_Init(AIEDITOR_RENDER_STATE *state, void **external_display_a, void **external_display_b);

}




extern "C" {
void pathEditorDrawPaths(void);

void pathEditor_CalcNodeIXs(void);

void pathEditor_UpdateNodesOnPlatforms(void);

void pathEditor_QuickOnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *previous, EDAIPATHCHECK_s *result);

void pathEditor_OnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *result, EDAIPATH_s *path, f32 tolerance);

void locatorEditor_PathNodeDeleted(EDAIPATHNODE_s *node);

EDAIPATH_s *pathEditor_GetPath(const char *name);

}



extern "C" {
AIPATHSYS_s *pathEditorCreateData(VARIPTR *cursor, VARIPTR *end, void **scratch_cursor, void **scratch_end);

void AISYSRebuildFromEditorData(void);

void aieditor_Leave(void);

void pathEditorSaveData(AIPATHSYS_s *system);

void aieditor_ClearMainMenu(void);

}






extern "C" {
void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);

void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);

void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);

}



















extern "C" {
void InitFn_AIEditorMovePlayers(AIEDITORMOVEPLAYERS *function);

eduimenu_s *aieditor_AddMessage(char *title, char *message);

void antinodeEditor_UpdateAntiNodesOnPlatforms(void);

void antinodeEditorDrawAntinodes(void);

void antinodeEditorSaveData(void);

__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditorSaveData(AIPATHSYS_s *system);

i32 aieditor_Save(void);

void aieditor_cbSave(eduimenu_s *parent, eduiitem_s *, u32);

}


extern "C" {
__attribute__((optimize("O2", "omit-frame-pointer"))) EDCREATURE_s *creatureEditor_GetNearest(i32 use_radius);

}


extern "C" {
__attribute__((force_align_arg_pointer)) void aieditor_SetCurrentScript(char *name,
                                                                            const AIEditorScriptSelection *selection);

}




extern "C" {
void aieditor_Reset(void);

__attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_PathNodeMoved(EDAIPATHNODE_s *node);

}



extern "C" {
__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *node);

}



extern "C" {
__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathDeleted(EDAIPATH_s *path);

}










extern "C" {
__attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_RenderAllCreatures(void);

}






extern "C" {
void AITerrInit(void);

f32 AITerrShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

i32 AITerrShadowOnPlatform(void);

i32 aieditor_Proc(f32 menu_delta_time, nupad_s *pad, f32 frame_delta_time, u32 exit_buttons);

}

// ==== functions in original address order ====

static char *LevelCharacterName(u8 character_index) {
    if (character_index == 0xff || CurrentStoryCList == NULL) {
        return NULL;
    }

    const i16 character_type = CurrentStoryCList[character_index].model_id;
    if (character_type == -1) {
        return NULL;
    }
    return apicharsys->char_data[character_type].file;
}

static i32 LevelCharacterGlobalID(u8 character_index) {
    if (character_index == 0xff || CurrentStoryCList == NULL) {
        return -1;
    }
    return CurrentStoryCList[character_index].model_id;
}

static char *GlobalCharacterName(i32 character_type) {
    if (character_type == -1 || character_type >= apicharsys->character_count) {
        return NULL;
    }
    return apicharsys->char_data[character_type].file;
}

static void *GlobalCharacterHGobj(i32 character_type) {
    if (character_type == -1) {
        return NULL;
    }

    const i16 model_index = apicharsys->playermodelids[character_type];
    if (model_index == -1) {
        return NULL;
    }
    return apicharsys->models[model_index].hierarchy;
}

static f32 GetViewRange(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->viewdistance;
}

static f32 GetHearDistance(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->heardistance;
}

static f32 GetMaxViewHeight(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->maxviewheight;
}

static f32 GetMinViewHeight(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->minviewheight;
}

APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id) {
    if (object_id >= HIGHGAMEOBJECT) {
        return NULL;
    }

    GameObject_s *object = &Obj[object_id];
    if ((object->apiobj.flags_low & APIOBJECT_FLAG_IN_USE) == 0) {
        return NULL;
    }

    if ((object->apiobj.flags_high & APIOBJECT_HIGH_FLAG_PLAYER_CHARACTER) != 0 ||
        object->ai.reset_mode == AI_OBJECT_ROUTE_STATE_SCRIPT_VISIBLE) {
        if (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) {
            return &object->apiobj;
        }
    }
    return NULL;
}

static i32 AI_GetMaxGizmos(void *) {
    return 32;
}

static i32 AI_GetOutput(GIZMO *gizmo, i32, i32) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return 0;
    }
    LEVELSCRIPTPROCESS *process = static_cast<LEVELSCRIPTPROCESS *>(gizmo->object);
    return process->processor.is_disabled == 0;
}

static i32 AI_GetNumOutputs(GIZMO *) {
    return 1;
}

char *AI_GetOutputName(GIZMO *, i32) {
    static char name[64] = "Processing";
    return name;
}

void AI_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL && gizmo->object != NULL) {
        LEVELSCRIPTPROCESS *process = static_cast<LEVELSCRIPTPROCESS *>(gizmo->object);
        process->processor.is_disabled = active == 0;
    }
}

static char *AI_GetGizmoName(GIZMO *gizmo) {
    static char name[16];
    if (gizmo == NULL || gizmo->object == NULL) {
        return NULL;
    }

    LEVELSCRIPTPROCESS *level_process = static_cast<LEVELSCRIPTPROCESS *>(gizmo->object);
    if (NuStrLen(level_process->name) != 0) {
        return level_process->name;
    }

    AISCRIPTPROCESS *process = &level_process->processor;
    if (process->script != NULL && process->script->name != NULL) {
        NuStrNCpy(name, process->script->name, sizeof(name));
    } else {
        NuStrCpy(name, "");
    }

    const i8 length = NuStrLen(name);
    if (process->unknown_a8 != NULL) {
        i8 total = length;
        total += NuStrLen("()");
        total += NuStrLen(process->unknown_a8->name);
        if (total <= 15) {
            NuStrCat(name, "(");
            NuStrCat(name, process->unknown_a8->name);
            NuStrCat(name, ")");
        }
    } else if (process->unknown_a4 != NULL) {
        i8 total = length;
        total += NuStrLen("()");
        total += NuStrLen(process->unknown_a4->name);
        if (total <= 15) {
            NuStrCat(name, "(");
            NuStrCat(name, process->unknown_a4->name);
            NuStrCat(name, ")");
        }
    } else if (process->unknown_a0 != NULL) {
        i8 total = length;
        total += NuStrLen("()");
        total += NuStrLen(process->unknown_a0->name);
        if (total <= 15) {
            NuStrCat(name, "(");
            NuStrCat(name, process->unknown_a0->name);
            NuStrCat(name, ")");
        }
    }
    return name;
}

static void AI_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *unused) {
    WORLDINFO *world;
    i32 i;

    world = (WORLDINFO *)world_ptr;

    // ORIG_BUG: This comparison can never fail.
    if (world->processors != NULL) { // NOLINT
        for (i = 0; i < WORLD->processor_count; i++) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->processors[i]);
        }
    }
}

static NUVEC *GetAICreatureOrigin(AISYS *system, AIPACKET *packet) {
    if (packet == NULL || system == NULL || packet->field_0x134 == 0xff)
        return NULL;
    if ((packet->navigation_flags & 8) == 0) {
        AICREATURE *creature = &system->creatures[packet->field_0x134];
        NUVEC offset;
        offset.x = ((packet->group_column + 1) >> 1) * creature->x_spacing;
        if ((packet->group_column & 1) != 0)
            offset.x = -offset.x;
        offset.y = 0.0f;
        offset.z = -creature->z_spacing * (f32)(u32)packet->group_row;
        NuVecRotateY(&offset, &offset, creature->y_rot);
        NuVecAdd(&packet->creature_origin, &offset, &creature->pos);
        packet->navigation_flags |= 8;
    }
    return &packet->creature_origin;
}

static APIOBJECT *GetNamedAPIObject(AISYS *system, char *name) {
    if (system != NULL) {
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
            if ((Obj[index].apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0) {
                continue;
            }

            if ((Obj[index].apiobj.field_0x1f4 & 0x400) != 0 && Obj[index].ai.field_0x134 != 0xff) {
                usize creature_address =
                    reinterpret_cast<usize>(system->creatures) + Obj[index].ai.field_0x134 * sizeof(*system->creatures);
                auto *creature = reinterpret_cast<decltype(system->creatures)>(creature_address);
                if (creature != NULL && NuStrICmp(creature->name, name) == 0) {
                    usize result_address = reinterpret_cast<usize>(Obj) + index * sizeof(*Obj);
                    if (result_address == 0) {
                        break;
                    }
                    return reinterpret_cast<APIOBJECT *>(result_address);
                }
            }
            if (NuStrICmp(Obj[index].apiobj.character_data->file, name) != 0) {
                continue;
            }
            usize result_address = reinterpret_cast<usize>(Obj) + index * sizeof(*Obj);
            if (result_address == 0) {
                break;
            }
            return reinterpret_cast<APIOBJECT *>(result_address);
        }
    }

    if (NuStrICmp(name, "player") == 0) {
        return player != NULL ? &player->apiobj : NULL;
    }
    if (NuStrICmp(name, "player_2") == 0) {
        GameObject_s *second = Player[0] == player ? Player[1] : Player[0];
        return second != NULL ? &second->apiobj : NULL;
    }
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] == NULL) {
            continue;
        }
        char player_name[72];
        sprintf(player_name, "Player%d", index);
        if (NuStrICmp(player_name, name) == 0) {
            return &Player[index]->apiobj;
        }
    }
    return NULL;
}

static i32 GlobalCharacterTypeID(char *name) {
    for (i32 character_type = 0; character_type < apicharsys->character_count; ++character_type) {
        if (NuStrICmp(name, apicharsys->char_data[character_type].file) == 0) {
            return character_type;
        }
    }
    return -1;
}

static i32 GameFindAlternativeSpecialObject(AISYS *, nuhspecial_s *special) {
    return EquivalentObject_Find(WorldInfo_CurrentlyActive(), special);
}

static void GameAILoad(AISYS *system, i32 version, NUGSCN *, VARIPTR *, VARIPTR *) {
    if (__builtin_expect(version < 12 && system->path_sys != NULL && system->path_sys->path_count != 0, 1)) {
        AIPATHSYS *path_system = system->path_sys;
        AIPATH **paths = path_system->paths;
        for (i32 path_index = 0; path_index < path_system->path_count; ++path_index) {
            AIPATH *path = paths[path_index];
            const i32 connection_count = path->connection_count;
            AIPATHCNX *entries = path->connections;
            for (i32 connection = 0; connection < connection_count; ++connection) {
                entries[connection].traversal_flags[0] = 0;
                entries[connection].original_traversal_flags[0] = 0;
                entries[connection].traversal_flags[1] = 0;
                entries[connection].original_traversal_flags[1] = 0;
            }
        }
    }

    i32 tag_length = static_cast<i8>(EdFileReadChar());
    if (__builtin_expect(tag_length <= 8 && tag_length != 0, 0)) {
        char tag[16];
        EdFileRead(tag, tag_length);
        if (NuStrICmp(tag, "LEGO") == 0) {
            EdFileReadInt();
            // Retail allocates through a fresh 16-aligned cursor against the
            // system's storage end instead of the caller's buffer.
            VARIPTR cursor __attribute__((aligned(16)));
            system->game_sys = AISysBufferAlloc(&cursor, &system->storage_end, 4);
        }
    }
}

static void GlobalCharacterRender(NUVEC *position, i16 angle, i32 character, i32, EDCREATURE_s *) {
    i16 layers[32];
    if (character < CHARCOUNT) {
        i16 model_id = apicharsys->playermodelids[character];
        if (model_id != -1) {
            CHARACTERMODEL_s *model = &apicharsys->models[model_id];

            i32 layer_count = GCDataList[model->model_id].make_layer_list(
                model, layers, apicharsys->char_data[character].game_character->layer_mask);
            if (layer_count > 0) {
                angle = NuAngAdd(angle, 0x8000);
                NUVEC scale;
                scale.x = scale.y = scale.z = CDataList[character].model_scale;
                NUMTX_ALIGNED16 matrix;
                NuMtxSetScale(&matrix, &scale);
                NuMtxTranslate(&matrix, position);
                NuMtxPreRotateY(&matrix, angle);
                NuHGobjRndr(model->hierarchy, &matrix, layer_count, layers);
            }
        }
    }
}

static f32 GetCharacterGoalSpeed(APIOBJECT *object) {
    if (object == NULL || object->ai == NULL) {
        return 0.0f;
    }

    switch (object->ai->goal_speed_mode) {
        case AI_ACTION_SPEED_RUN:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->movement_speed * FRAMETIME;
        case AI_ACTION_SPEED_WALK:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->field_0x18 * FRAMETIME;
        case AI_ACTION_SPEED_TIPTOE:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->field_0x14 * FRAMETIME;
        default:
            return 0.0f;
    }
}

static i32 GameAIActionParseSpeed(char *name, u8 *speed) {
    if (NuStrICmp(name, "RUN") == 0) {
        *speed = AI_ACTION_SPEED_RUN;
        return 1;
    }
    if (NuStrICmp(name, "WALK") == 0) {
        *speed = AI_ACTION_SPEED_WALK;
        return 1;
    }
    if (NuStrICmp(name, "TIPTOE") == 0) {
        *speed = AI_ACTION_SPEED_TIPTOE;
        return 1;
    }
    return 0;
}

static char *SpecialRouteCharacterName(u8 route_id) {
    if (route_id == 0xff) {
        return NULL;
    }
    if (route_id < 10) {
        return Suit[route_id].suit_character_name;
    }

    i32 route_index = 0;
    i32 skipped_count = 0;
    while (CurrentStoryCList[route_index + skipped_count].model_id != -1 && route_index + skipped_count < 64) {
        char *name = apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file;
        bool skip = false;
        for (i32 skip_index = 0; skip_chars[skip_index] != NULL; ++skip_index) {
            if (NuStrICmp(name, skip_chars[skip_index]) == 0) {
                skip = true;
                break;
            }
        }
        if (skip) {
            ++skipped_count;
            continue;
        }
        if (route_index + 10 == route_id) {
            return apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file;
        }
        ++route_index;
    }
    return NULL;
}

static i32 LevelCharacterTypeID(char *name) {
    i32 result = 0x40;
    if (NuStrICmp(name, "Everyone") != 0) {
        result = -1;
        if (CurrentStoryCList != NULL) {
            for (i32 index = 0; CurrentStoryCList[index].model_id != -1 && index < 64; ++index) {
                if (NuStrICmp(name, apicharsys->char_data[CurrentStoryCList[index].model_id].file) == 0) {
                    result = index;
                    break;
                }
            }
        }
    }
    return result;
}

static u32 AIBigJumpToDestination(APIOBJECT *object, NUVEC *destination) {
    if (destination == NULL || object == NULL || object->objptr == NULL || object->field_0x287 != 0 ||
        object->objptr->field_0x7a5 == AI_GAME_OBJECT_TYPE_VEHICLE) {
        return 1;
    }

    GameObject_s *game_object = object->objptr;
    if (game_object->field_0xcc0 == NULL) {
        if ((object->character_data->model_flags & CHARACTER_AI_MODEL_FLAG_SNAP_ON_BIG_JUMP) != 0) {
            object->start_position = *destination;
            object->velocity = v000;
            object->position = *destination;
            ResetPlayerMoves(game_object);
            object->respawn_timer = 0.0f;
        } else {
            StartBigJump(game_object, destination, 0, 0.5f, 1.0f, 0, 0);
        }
    } else {
        SnapCreaturePos(game_object, destination, 0, NULL, 0);
    }

    return 1;
}

static u32 AIRespawnOnPath(APIOBJECT *object) {
    if (object->field_0x287 != 0 || (object->ai->path_info.flags & AI_RESPAWN_FLAG_DISABLED) != 0 ||
        (object->flags_high & APIOBJECT_HIGH_FLAG_RESPAWN_ENABLED) == 0) {
        return 0;
    }

    const u32 model_flags = object->character_data->model_flags;
    if ((model_flags & CHARACTER_AI_MODEL_FLAG_DISABLE_RESPAWN) != 0) {
        return 0;
    }

    GameObject_s *game_object = object->objptr;
    if (game_object->field_0x7a5 == AI_GAME_OBJECT_TYPE_VEHICLE || game_object->field_0xcc0 != NULL) {
        return 0;
    }

    if (object->respawn_timer > AI_RESPAWN_DELAY) {
        if ((model_flags & CHARACTER_AI_MODEL_FLAG_SNAP_ON_BIG_JUMP) == 0) {
            StartBigJump(game_object, &object->respawn_position, 0, 0.5f, 1.0f, 0, 0);
        } else {
            object->start_position = object->respawn_position;
            object->position = object->respawn_position;
            object->velocity = v000;
            ResetPlayerMoves(game_object);
            object->respawn_timer = 0.0f;
        }
    }

    return 0;
}

static i32 SpecialRouteCharacterTypeID(char *name) {
    if (NuStrICmp(name, "Everyone") == 0) {
        return 0x40;
    }

    for (i32 suit_index = 0; suit_index < 10; ++suit_index) {
        if (NuStrICmp(name, Suit[suit_index].suit_character_name) == 0) {
            return suit_index;
        }
    }

    i32 route_index = 0;
    i32 skipped_count = 0;
    if (CurrentStoryCList != NULL) {
        while (CurrentStoryCList[route_index].model_id != -1 && route_index < 64) {
            char *character_name = apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file;
            bool skip = false;
            for (i32 skip_index = 0; skip_chars[skip_index] != NULL; ++skip_index) {
                if (NuStrICmp(character_name, skip_chars[skip_index]) == 0) {
                    skip = true;
                    break;
                }
            }
            if (skip) {
                ++skipped_count;
                continue;
            }
            if (NuStrICmp(name, apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file) ==
                0) {
                return route_index + 10;
            }
            ++route_index;
        }
    }
    return -1;
}

void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position) {
    if (object != NULL &&
        ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || (object->apiobj.field_0x1f4 & 5) != 0 ||
         (static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) != 0))
        return;
    alert_obj = object;
    alert_pos = *position;
    alert_timer = alert_time;
}

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float) {
    NUVEC difference = {0.0f, 0.0f, 0.0f};
    if (system != NULL) {
        if (goody_count == 0)
            return;
        f32 distance = 0.0f;
        i32 complete = baddy_count == 0;
        if (alert_obj != NULL && (alert_obj->apiobj.flags_high & 0x10) != 0 &&
            (alert_obj->apiobj.field_0x1f4 & 5) == 0 &&
            (static_cast<GAMECHARACTERDATA *>(alert_obj->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) ==
                0) {
            const u64 alert_mask = (u64)1 << alert_obj->apiobj.field_0x289;
            for (i32 i = 0; i < baddy_count; ++i) {
                APIOBJECT_s *object = baddies[i];
                if ((object->ai_awareness_mask & alert_mask) != 0) {
                    object->objptr->alert_target_timer = 5.0f;
                    object->objptr->alert_target = &alert_obj->apiobj;
                } else if (WORLD->rooms_visible_ptr[object->objptr->room_id] != 0) {
                    difference.x = alert_pos.x - object->collision_position.x;
                    if (object->heardistance > difference.x) {
                        difference.z = alert_pos.z - object->collision_position.z;
                        if (object->heardistance > difference.z) {
                            difference.y = alert_pos.y - object->collision_position.y;
                            if (object->maxviewheight > difference.y && difference.y > object->minviewheight) {
                                distance = difference.x * difference.x + difference.z * difference.z;
                                if (!(object->heardistance * object->heardistance > distance))
                                    continue;
                                if ((WORLD->api_object_sys->line_of_sight[object->field_0x289] & alert_mask) == 0) {
                                    const f32 dx =
                                        alert_obj->apiobj.collision_position.x - object->collision_position.x;
                                    const f32 dz =
                                        alert_obj->apiobj.collision_position.z - object->collision_position.z;
                                    if (!(dx * dx + dz * dz > object->viewdistance * object->viewdistance))
                                        continue;
                                }
                                object->objptr->alert_target = &alert_obj->apiobj;
                                object->ai_awareness_mask |= alert_mask;
                                awareness |= object->ai_awareness_mask;
                                object->objptr->alert_target_timer = 5.0f;
                            }
                        }
                    }
                }
            }
            alert_timer -= FRAMETIME;
            if (alert_timer <= 0.0f)
                alert_obj = NULL;
        }
        if (system->goody_idx >= goody_count)
            system->goody_idx = 0;
        i32 budget = 32;
        // Pending choices survive budget exhaustion. Publish only after a pass
        // finishes without changing the baddies' assignments.
        while (!complete && budget > 0) {
            complete = 0;
            while (system->goody_idx < goody_count && budget > 0) {
                APIOBJECT_s *goody = goodies[system->goody_idx];
                f32 best_distance = 1.0e9f;
                APIOBJECT_s *best_baddy = NULL;
                for (i32 i = 0; i < baddy_count; ++i) {
                    APIOBJECT_s *baddy = baddies[i];
                    if (baddy == goody || baddy->ai->pending_opponent == goody)
                        continue;
                    i32 have_distance = 0;
                    if (baddy->objptr->field_0xefc & 2) {
                        goody->ai_awareness_mask &= ~((u64)1 << baddy->field_0x289);
                    } else if ((goody->ai_awareness_mask >> baddy->field_0x289) & 1) {
                        distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if (distance >= goody->viewdistance + baddy->visibility_range_extension)
                            goody->ai_awareness_mask &= ~((u64)1 << baddy->field_0x289);
                    } else if ((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1) {
                        distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if ((!party_under_cover || ((goody->field_0x1f4 & 0x400) && (baddy->field_0x1f4 & 0x400))) &&
                            (((awareness >> baddy->field_0x289) & 1) ||
                             (((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1) &&
                              ((goody->flags_high & 8) ||
                               difference.x * goody->objptr->facing_direction.x +
                                       difference.z * goody->objptr->facing_direction.z >=
                                   0.0f ||
                               (goody->heardistance > distance && (baddy->objptr->field_0xef9 & 8)))))) {
                            goody->ai_awareness_mask |= (u64)1 << baddy->field_0x289;
                            awareness |= goody->ai_awareness_mask;
                        }
                    }
                    if (goody->objptr->field_0xefc & 2) {
                        baddy->ai_awareness_mask &= ~((u64)1 << goody->field_0x289);
                    } else if (baddy->objptr->alert_target == goody) {
                        baddy->ai_awareness_mask |= (u64)1 << goody->field_0x289;
                        awareness |= baddy->ai_awareness_mask;
                    } else if ((baddy->ai_awareness_mask >> goody->field_0x289) & 1) {
                        if (!have_distance)
                            distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if (distance >= baddy->viewdistance + goody->visibility_range_extension)
                            baddy->ai_awareness_mask &= ~((u64)1 << goody->field_0x289);
                    } else if ((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) & 1) {
                        if (!have_distance)
                            distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if ((!party_under_cover || ((baddy->field_0x1f4 & 0x400) && (goody->field_0x1f4 & 0x400))) &&
                            (((awareness >> goody->field_0x289) & 1) ||
                             (((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) & 1) &&
                              ((baddy->flags_high & 8) ||
                               -difference.z * baddy->objptr->facing_direction.z -
                                       difference.x * baddy->objptr->facing_direction.x >=
                                   0.0f ||
                               (baddy->heardistance > distance && (goody->objptr->field_0xef9 & 8)))))) {
                            baddy->ai_awareness_mask |= (u64)1 << goody->field_0x289;
                            awareness |= baddy->ai_awareness_mask;
                        }
                    }
                    if (!((goody->ai_awareness_mask >> baddy->field_0x289) & 1) &&
                        !((baddy->ai_awareness_mask >> goody->field_0x289) & 1))
                        continue;
                    if (!have_distance)
                        distance = NuVecDist(&baddy->position, &goody->position, &difference);
                    GameObject_s *baddy_object = baddy->objptr;
                    i16 baddy_id = baddy_object->id;
                    if (baddy_id != id_JAWA && baddy_id != id_UGNAUGHT) {
                        if (!(goody->character_data->model_flags & 0x80000)) {
                            if ((baddy_object->field_0xefb & 1) && goody->objptr != player &&
                                goody->objptr != player2) {
                                baddy_object->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                            } else if (goody->objptr->id == id_DRAGBOMB && baddy_id != id_ATAT) {
                                baddy_object->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                            }
                        } else {
                            baddy_object->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                        }
                    } else {
                        if (!ZapTarget(goody->objptr))
                            baddy->objptr->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                        baddy_object = baddy->objptr;
                        baddy_id = baddy_object->id;
                    }
                    GameObject_s *goody_object = goody->objptr;
                    if (!((baddy_object->ai_opponent_exclusion_mask >> goody->field_0x289) & 1)) {
                        AIPACKET *packet = baddy->ai;
                        if (packet->pending_nearest_metric > distance) {
                            packet->pending_nearest_metric = distance;
                            packet->pending_nearest_opponent = goody;
                        }
                        f32 metric = distance;
                        if (packet->opponent_object == goody)
                            metric -= 0.5f;
                        if ((baddy->ai_awareness_mask >> goody->field_0x289) & 1) {
                            if (packet->pending_opponent == NULL) {
                                if (!(packet->runtime_flags & 2) ||
                                    ((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) &
                                     1) ||
                                    baddy_object->alert_target == goody) {
                                    if (best_distance > metric) {
                                        best_distance = distance;
                                        best_baddy = baddy;
                                    }
                                }
                            } else if (goody->player_controlled && (baddy_object->field_0xefb & 0x40)) {
                                if (!packet->pending_opponent->player_controlled ||
                                    packet->pending_opponent_metric > metric) {
                                    best_distance = distance;
                                    best_baddy = baddy;
                                }
                            }
                        }
                    }
                    if (baddy_id == id_JAWA || baddy_id == id_UGNAUGHT ||
                        (baddy->character_data->model_flags & 0x80000))
                        goody_object->ai_opponent_exclusion_mask |= (u64)1 << baddy->field_0x289;
                    if (!((goody_object->ai_opponent_exclusion_mask >> baddy->field_0x289) & 1)) {
                        AIPACKET *packet = goody->ai;
                        if (packet->pending_nearest_metric > distance) {
                            packet->pending_nearest_metric = distance;
                            packet->pending_nearest_opponent = baddy;
                        }
                        f32 metric = distance;
                        if (packet->opponent_object == baddy)
                            metric -= 0.5f;
                        if (((goody->ai_awareness_mask >> baddy->field_0x289) & 1) &&
                            (!(packet->runtime_flags & 2) ||
                             ((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1))) {
                            if (packet->pending_opponent_metric > metric ||
                                (packet->pending_opponent != NULL && packet->pending_opponent->objptr != NULL &&
                                 packet->pending_opponent->objptr->character_context == 0x5a &&
                                 baddy_object->character_context != 0x5a)) {
                                packet->pending_opponent_metric = distance;
                                packet->pending_opponent = baddy;
                            }
                        }
                    }
                }
                if (baddy_count > 0) {
                    budget -= baddy_count;
                    if (best_baddy != NULL && best_baddy->ai->pending_opponent != goody) {
                        best_baddy->ai->pending_opponent = goody;
                        best_baddy->ai->pending_opponent_metric = best_distance;
                        system->unknown_flag_2 = 1;
                        if (!((goody->objptr->ai_opponent_exclusion_mask >> best_baddy->field_0x289) & 1) &&
                            ((goody->ai_awareness_mask >> best_baddy->field_0x289) & 1)) {
                            AIPACKET *packet = goody->ai;
                            if (!(packet->field_0x1e5 & 0x10) || packet->pending_opponent_metric > best_distance) {
                                packet->pending_opponent = best_baddy;
                                packet->pending_opponent_metric = best_distance;
                                packet->field_0x1e5 |= 0x10;
                            }
                        }
                    }
                }
                ++system->goody_idx;
                if (system->goody_idx >= goody_count) {
                    system->goody_idx = 0;
                    if (system->unknown_flag_2) {
                        system->unknown_flag_2 = 0;
                        complete = 0;
                    } else {
                        complete = 1;
                    }
                }
            }
        }
        if (!complete)
            return;
        system->goody_idx = 0;
        nbaddies_can_see_players = 0;
        // Losing LOS alone does not erase the seen history; losing awareness does.
        for (i32 i = 0; i < goody_count; ++i) {
            APIOBJECT_s *goody = goodies[i];
            for (i32 j = 0; j < baddy_count; ++j) {
                APIOBJECT_s *baddy = baddies[j];
                if (!((goody->ai_awareness_mask >> baddy->field_0x289) & 1)) {
                    if ((goody->objptr->ai_seen_mask >> baddy->field_0x289) & 1) {
                        goody->objptr->ai_seen_mask &= ~((u64)1 << baddy->field_0x289);
                    }
                } else {
                    if ((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1)
                        goody->objptr->ai_seen_mask |= (u64)1 << baddy->field_0x289;
                }
                if ((baddy->ai_awareness_mask >> goody->field_0x289) & 1) {
                    if ((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) & 1)
                        baddy->objptr->ai_seen_mask |= (u64)1 << goody->field_0x289;
                } else if ((baddy->objptr->ai_seen_mask >> goody->field_0x289) & 1) {
                    baddy->objptr->ai_seen_mask &= ~((u64)1 << goody->field_0x289);
                }
            }
        }
        for (i32 i = 0; i < count; ++i) {
            APIOBJECT_s *object = objects[i];
            AIPACKET *packet = object->ai;
            packet->opponent_metric = packet->pending_opponent_metric;
            packet->opponent_object = packet->pending_opponent;
            packet->field_0x1e5 = (packet->field_0x1e5 & ~8) | ((packet->field_0x1e5 >> 1) & 8);
            packet = object->ai;
            packet->nearest_opponent_metric = packet->pending_nearest_metric;
            packet->nearest_opponent_object = packet->pending_nearest_opponent;
            packet->pending_opponent_metric = 1.0e9f;
            packet->pending_nearest_metric = 1.0e9f;
            packet->field_0x1e5 &= ~0x10;
            packet->pending_nearest_opponent = NULL;
            packet->pending_opponent = NULL;
            if ((awareness >> object->field_0x289) & 1)
                object->ai->field_0x1e5 |= 0x40;
            else
                object->ai->field_0x1e5 &= ~0x40;
            object->objptr->field_0xef9 &= ~8;
            GameObject_s *game_object = object->objptr;
            game_object->ai_opponent_exclusion_mask = 0;
            if ((player != NULL && ((game_object->ai_seen_mask >> player->apiobj.field_0x289) & 1)) ||
                (player2 != NULL && ((game_object->ai_seen_mask >> player2->apiobj.field_0x289) & 1)))
                ++nbaddies_can_see_players;
            if (game_object->opponent != NULL) {
                packet = object->ai;
                packet->opponent_object = static_cast<APIOBJECT_s *>(game_object->opponent);
                packet->opponent_metric = NuVecDist(&packet->opponent_object->position, &object->position, &difference);
                packet = object->ai;
                if (packet->opponent_object->ai->opponent_object == object)
                    packet->field_0x1e5 |= 8;
                else
                    packet->field_0x1e5 &= ~8;
            }
        }
    }
}

void MakeBaddiesForgetAboutParty(i32 check_hostility) {
    GameObject_s *baddies[64];
    GameObject_s *party[64];
    i32 baddie_count = 0;
    i32 party_count = 0;
    GameObject_s *objects = Obj;
    i32 count = HIGHGAMEOBJECT;
    GameObject_s *object = objects;
    for (i32 i = 0; i < count; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0)
            continue;
        if ((object->apiobj.field_0x1f4 & 1) != 0)
            party[party_count++] = object;
        else if ((object->apiobj.field_0x1f4 & 4) == 0)
            baddies[baddie_count++] = object;
    }
    if (check_hostility != 0) {
        for (i32 i = 0; i < baddie_count; ++i) {
            u64 mask = u64(1) << baddies[i]->apiobj.field_0x289;
            for (i32 j = 0; j < party_count; ++j) {
                APIOBJECT *object = &party[j]->apiobj;
                u64 hostile = u64(object->field387_0x2a0) | (u64(object->field388_0x2a4) << 32);
                if ((hostile & mask) != 0) {
                    u32 *defaults = WORLD->api_object_sys->hostility_masks[object->field_0x289];
                    if (((u64(defaults[0]) | (u64(defaults[1]) << 32)) & mask) != 0)
                        return;
                }
            }
        }
    }
    for (i32 i = 0; i < baddie_count; ++i) {
        APIOBJECT *baddie = &baddies[i]->apiobj;
        u64 mask = ~(u64(1) << baddie->field_0x289);
        for (i32 j = 0; j < party_count; ++j) {
            APIOBJECT *member = &party[j]->apiobj;
            member->field387_0x2a0 &= static_cast<u32>(mask);
            member->field388_0x2a4 &= static_cast<u32>(mask >> 32);
            u64 other_mask = ~(u64(1) << member->field_0x289);
            baddie->field387_0x2a0 &= static_cast<u32>(other_mask);
            baddie->field388_0x2a4 &= static_cast<u32>(other_mask >> 32);
        }
    }
    object = objects;
    for (i32 i = 0; i < count; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0)
            continue;
        object->ai.field_0x1e5 &= ~0x58;
        object->ai.opponent = NULL;
        object->ai.opponent_metric = 1000000000.0f;
        object->ai.nearest_opponent = NULL;
        object->ai.field_0xdc = 0;
        object->ai.nearest_opponent_metric = 1000000000.0f;
        object->ai.target_metric_e0 = 1000000000.0f;
        object->ai.field_0xec = 0;
        object->ai.target_metric_f0 = 1000000000.0f;
        object->apiobj.field387_0x2a0 = 0;
        object->apiobj.field388_0x2a4 = 0;
        object->field_0xebc = 0;
        object->field_0xec0 = 0;
        object->field_0xecc = 0;
    }
    party_under_cover = 0;
    alert_timer = 0.0f;
    alert_obj = NULL;
    if (VADER_ADATA != NULL && WORLD->area == VADER_ADATA)
        return;
    if (party_cant_be_under_cover != 0)
        return;
    i32 all_under_cover = 1;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *player = Player[i];
        if (player == NULL || (player->apiobj.field_0x1f8 & 0x1001) != 0x1001 || player->apiobj.field_0x287 != 0 ||
            (player->field_0xeff & 1) != 0)
            continue;
        if (player->field_0x108e == 5 ||
            ((FreePlay != 0 || WORLD->current_level == HUB_LDATA) &&
             (player->apiobj.character_data->model_flags & 0x204) != 0 && player->field_0xcc0 == NULL)) {
            party_under_cover = 1;
        }
        if ((player->apiobj.character_data->model_flags & 0x80000) == 0)
            all_under_cover = 0;
    }
    if (all_under_cover != 0)
        party_under_cover = 1;
}

void GameAttackInit() {
}

i32 CanFightLikeAJedi(GameObject_s *object) {
    return CharCategory_IsCategory(object, 0) != 0 || object->id == id_GRIEVOUS || object->id == id_BODYGUARD ||
           object->id == id_IMPERIALGUARD;
}

void GameAISysStartFrame(AISYS_s *system) {
    if (system == NULL || netclient != 0) {
        return;
    }

    if (system->path_sys != NULL && system->path_sys->path_count != 0) {
        for (i32 index = 0; index < system->path_sys->path_count; ++index) {
            memset(&system->path_sys->paths[index]->updated_node_bits[0], 0, 0x20);
            memmove(system->path_sys->paths[index]->previous_inside_node_bits,
                    system->path_sys->paths[index]->inside_node_bits, 0x20);
            memset(system->path_sys->paths[index]->inside_node_bits, 0,
                   sizeof(system->path_sys->paths[index]->inside_node_bits));
        }

        // Moving specials can carry path endpoints. Update both ends of each
        // connection; AIPathNodeUpdatePos uses updated_node_bits to ensure a
        // shared endpoint is transformed only once this frame.
        for (i32 path_index = 0; path_index < system->path_sys->path_count; ++path_index) {
            AIPATH *path = system->path_sys->paths[path_index];
            for (i32 connection_index = 0; connection_index < path->connection_count; ++connection_index) {
                AIPATHCNX *connection = &path->connections[connection_index];
                AIPATHNODE *node = &path->nodes[connection->direction_a];
                if (__builtin_expect(node->has_special != 0, 0)) {
                    AIPathNodeUpdatePos(system, path, node);
                }
                path = system->path_sys->paths[path_index];
                node = &path->nodes[connection->direction_b];
                if (__builtin_expect(node->has_special != 0, 0)) {
                    AIPathNodeUpdatePos(system, path, node);
                    path = system->path_sys->paths[path_index];
                }
            }
        }
    }

    // Spread the object/area overlap work across frames. Area runtime flags
    // summarize the kinds of live characters inside it, while every object
    // retains a 64-area occupancy mask for script queries.
    if (__builtin_expect(system->next_area_check < system->area_count, 0)) {
        AIAREA *area = &system->areas[system->next_area_check];
        const i32 area_index = static_cast<i32>(area - WORLD->ai_sys->areas);
        area->runtime_flags &= static_cast<u8>(
            ~(AIAREA_RUNTIME_PLAYER_PRESENT | AIAREA_RUNTIME_OBJECT_STATE_CLEAR | AIAREA_RUNTIME_OBJECT_STATE_SET));

        GameObject_s *object = Obj;
        for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
            if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_CHARACTER | APIOBJECT_FLAG_IN_USE)) !=
                (APIOBJECT_FLAG_CHARACTER | APIOBJECT_FLAG_IN_USE)) {
                continue;
            }
            if (object->apiobj.field_0x287 != 0 && !(object->field_0x101c > 0.0f) &&
                !object->apiobj.player_controlled) {
                continue;
            }

            NUVEC local_position;
            NuVecSub(&local_position, &object->apiobj.position, &area->position);
            NuVecRotateY(&local_position, &local_position, -area->rotation);
            const bool is_inside = local_position.x >= -area->half_width && local_position.y >= -0.1f &&
                                   local_position.z >= -area->half_depth && local_position.x <= area->half_width &&
                                   local_position.y <= area->height && local_position.z <= area->half_depth;
            if (!is_inside) {
                // Outside removal addresses the full 64-bit area bit.
                const u64 area_bit = 1ULL << area_index;
                object->apiobj.ai_area_mask_low &= ~static_cast<u32>(area_bit);
                object->apiobj.ai_area_mask_high &= ~static_cast<u32>(area_bit >> 32);
                continue;
            }

            // Inside occupancy uses the signed, wrapping 32-bit area bit.
            const i64 inside_area_bit = static_cast<i32>(1u << (area_index & 31));
            object->apiobj.ai_area_mask_low |= static_cast<u32>(inside_area_bit);
            object->apiobj.ai_area_mask_high |= static_cast<u32>(inside_area_bit >> 32);
            if ((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0) {
                area->runtime_flags |= AIAREA_RUNTIME_PLAYER_PRESENT;
            }
            if (object->apiobj.field_0x27c != -1) {
                area->runtime_flags |= AIAREA_RUNTIME_CHARACTER_SLOT_SEEN;
            }
            if ((object->apiobj.field_0x1f4 & 1) != 0) {
                area->runtime_flags |= AIAREA_RUNTIME_OBJECT_STATE_SET;
            } else if ((object->apiobj.field_0x1f4 & 4) == 0) {
                area->runtime_flags |= AIAREA_RUNTIME_OBJECT_STATE_CLEAR;
            }
        }

        ++system->next_area_check;
        if (__builtin_expect(system->next_area_check >= system->area_count, 0)) {
            system->next_area_check = 0;
        }
    }

    // Level scripts are ordinary AI processors without an object or packet.
    // Disabled processors retain their state but do not advance this frame.
    for (i32 index = 0; index < WORLD->processor_count; ++index) {
        if (!WORLD->processors[index].processor.is_disabled) {
            AIScriptProcess(system, NULL, NULL, &WORLD->processors[index].processor, FRAMETIME);
        }
    }
}

i32 CheckPosAIArea(AIAREA_s *area, nuvec_s *position, float tolerance) {
    if (position == NULL || area == NULL) {
        return 0;
    }

    NUVEC local_position;
    NuVecSub(&local_position, position, &area->position);
    NuVecRotateY(&local_position, &local_position, -area->rotation);

    return local_position.x + tolerance >= -area->half_width && local_position.y + tolerance >= -0.1f &&
           local_position.z + tolerance >= -area->half_depth && local_position.x - tolerance <= area->half_width &&
           local_position.y - tolerance <= area->height && local_position.z - tolerance <= area->half_depth;
}

void GameAISysSetGame() {
    AIPathCnxHelperSysInitFn = NULL;
    StarWars_GameAISysInit();
}

void GameAISysInit() {
    RegisterAIScriptActions(lego_aiactiondefs);
    RegisterAIScriptConditions(lego_aiconditiondefs);
    InitFn_LevelCharacterTypeID(LevelCharacterTypeID);
    InitFn_SpecialRouteCharacterTypeID(SpecialRouteCharacterTypeID);
    InitFn_LevelCharacterName(LevelCharacterName);
    InitFn_SpecialRouteCharacterName(SpecialRouteCharacterName);
    InitFn_LevelCharacterGlobalID(LevelCharacterGlobalID);
    InitFn_GlobalCharacterTypeID(GlobalCharacterTypeID);
    InitFn_GlobalCharacterName(GlobalCharacterName);
    InitFn_GlobalCharacterRender(GlobalCharacterRender);
    InitFn_GlobalCharacterHGobj(GlobalCharacterHGobj);
    InitFn_ClearAICreatures(ClearAICreatures);
    InitFn_GetCharacterGoalSpeedFn(GetCharacterGoalSpeed);
    InitFn_GetViewRange(GetViewRange);
    InitFn_GetHearDistance(GetHearDistance);
    InitFn_GlobalGetMaxViewHeight(GetMaxViewHeight);
    InitFn_GlobalGetMinViewHeight(GetMinViewHeight);
    InitFn_GameAILoad(GameAILoad);
    InitFn_AIActionParseSpeed(GameAIActionParseSpeed);
    InitFn_AIRespawnOnPath(AIRespawnOnPath);
    InitFn_AIBigJumpToDestination(AIBigJumpToDestination);
    InitFn_FindAlternativeSpecialObjectFn(GameFindAlternativeSpecialObject);
    InitFn_APIOBJECTFromObjIDFn(GameAPIOBJECTFromObjID);
    InitFn_GetNamedAPIObject(GetNamedAPIObject);
    InitFn_GetAICreatureOrigin(GetAICreatureOrigin);

    default_path_heighttol = 0.2f;
    default_activate_difficulty = AI_DEFAULT_ACTIVATE_DIFFICULTY;
    default_min_n_respawns = 0;
    default_max_n_respawns = 0;
    default_min_t_respawn = 0.0f;
    default_max_t_respawn = 0.0f;

    SetScriptErrorLevel(SCRIPT_ERROR_LEVEL_WARNING);
    GameAISysSetGame();
}

i32 TryToTeleportToNextNode(GameObject_s *object, AIPATHNODE_s *node, i32 tightrope) {
    if ((object->apiobj.field_0x1f4 & 0x400) != 0 || !object->ai.path_info.on_path) {
        return 0;
    }
    if (!(object->field_0xf1c >= OFFSCREEN_CATCHUP_TIME) &&
        (object->apiobj.model_draw_result != 0 || (object->tag_context_flags & 2) != 0 ||
         !(drop_back_in_timer > 0.0f))) {
        return 0;
    }
    NUVEC position = node->position;
    position.y += object->apiobj.field_0x1e0;
    if (NuCameraClipTestSphere(&position, object->apiobj.field_0x1e0, &numtx_identity) == 0) {
        return 0;
    }
    if (tightrope != 0) {
        if (TightRope_SnapTo(object, &node->position) == 0) {
            return 0;
        }
        object->field_0x109c = 0;
        object->field_0x1092 = 0;
        object->field_0x1093 = 0;
        object->ai.path_connection_state = 0;
        return 1;
    }
    object->apiobj.initial_position = node->position;
    object->apiobj.collision_position = object->apiobj.initial_position;
    object->apiobj.start_position = object->apiobj.initial_position;
    object->ai.owner->apiobj.position = object->apiobj.initial_position;
    plr_lastpos = object->apiobj.initial_position;
    object->apiobj.velocity = v000;
    InitSurfaceInfo(object);
    SetObjOnSurface(object, 0);
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->field_0x109c = 0;
    object->ai.path_connection_state = 0;
    if (object->field_0xcc0 == NULL) {
        Player_ClearContext(object, 0);
        Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }
    return 1;
}

void SetSpecialMove(GameObject_s *object, AIPATHNODE_s *target, AIPATHNODE_s *node, char move) {
    object->ai.field_0x180 = target;
    object->field_0x1092 = move;
    object->field_0x1093 = 0;
    object->special_move_node = node;
    object->special_move_progress = 0.0f;
}

void ClearSpecialMove(GameObject_s *object) {
    object->special_move_progress = 1.0f;
    object->ai.field_0x180 = NULL;
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->special_move_node = NULL;
    object->field_0x1080 = 0.0f;
    object->field_0x107f = 0;
}

void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system) {
    if (system == NULL) {
        return;
    }

    system->available_controllers.head = NULL;
    system->available_controllers.tail = NULL;
    system->active_controllers.head = NULL;
    system->active_controllers.tail = NULL;
    memset(system->controllers, 0, system->controller_count * sizeof(*system->controllers));
    auto system_controller_count = system->controller_count;
    for (i32 i = 0; i < system_controller_count; ++i) {
        NuLinkedListAppend(&system->available_controllers, &system->controllers[i].links);
    }
}

void *AIPathCnxControlSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    AIPATHCNXCONTROLSYS_s *system =
        static_cast<AIPATHCNXCONTROLSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(AIPATHCNXCONTROLSYS_s)));
    if (system != NULL) {
        memset(system, 0, sizeof(*system));
        system->controllers = static_cast<AIPATHCNXCONTROLLER_s *>(
            AISysBufferAlloc(buf, buf_end, static_cast<u32>(count) * sizeof(AIPATHCNXCONTROLLER_s)));
        if (system->controllers != NULL) {
            system->controller_count = count;
            AIPathCnxControlSysReset(system);
        }
    }
    return system;
}

void GameAISysReset(AISYS_s *system) {
    if (system == NULL) {
        return;
    }

    AISysSetLevelPath(system, NULL);

    WORLD->processor_count = 0;
    for (i32 script_index = 0; script_index < 32; ++script_index) {
        char script_name[16];
        if (script_index != 0) {
            sprintf(script_name, "Level%d", script_index);
        } else {
            sprintf(script_name, "Level");
        }

        if (AIScriptFind(WORLD->ai_sys, script_name, 0, 1, 0) == NULL) {
            continue;
        }

        AIScriptProcessorInit(system, NULL, &WORLD->processors[WORLD->processor_count].processor, NULL, script_name,
                              NULL, 0, NULL, NULL);
        NuStrCpy(WORLD->processors[WORLD->processor_count].name, script_name);
        LevelScriptReStoreProgress(WORLD, &WORLD->processors[WORLD->processor_count]);
        ++WORLD->processor_count;
    }

    AIPATHSYS *path_system = system->path_sys;
    if (path_system != NULL) {
        for (i32 path_index = 0; path_index < path_system->path_count; ++path_index) {
            AIPATH *path = path_system->paths[path_index];
            AIPATHCNX *connection = path->connections;
            auto path_connection_count = path->connection_count;
            for (i32 connection_index = 0; connection_index < path_connection_count; ++connection_index, ++connection) {
                connection->node_a = connection->previous_node_a;
                connection->node_b = connection->previous_node_b;

                if (LEGO_AIPATHCNX_BLOCKAGE != 0) {
                    connection->node_a &= ~LEGO_AIPATHCNX_BLOCKAGE;
                    connection->node_b &= ~LEGO_AIPATHCNX_BLOCKAGE;
                }
            }
        }
    }

    AIPathCnxControlSysReset(WORLD->ai_path_cnx_control_sys);
    AIPathCnxHelperSysReset(WORLD, WORLD->ai_path_cnx_helper_sys);
    InitAICreatures(system);
    ResetAICreatures(system);

    if (WORLD->processor_count != 0) {
        GizmoSysAddGizmos(WORLD->gizmo_sys, WORLD->giz_flow, WORLD);
    }
}

AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system,
                                                 AIPATH_s *path, char *from, char *to, i32 target_type,
                                                 char *target_name, i32 fake_animation_id, i32 gizmo_output) {
    nuhspecial_s special = {};
    if (from == NULL || control_system == NULL || to == NULL) {
        return NULL;
    }

    i32 direction;
    AIPATHCNX *connection = static_cast<AIPATHCNX *>(AIPAthFindPathCnx(ai_system, path, from, to, &direction));
    void *target = NULL;
    if (connection != NULL) {
        i32 gizmo_type;
        switch (target_type) {
            case 0:
                if (target_name != NULL) {
                    NuSpecialFind(ai_system->scene, &special, target_name, 1);
                }
                break;
            case 1:
                target = CutScene_FindInst(WORLD->cutscene_sys, target_name);
                break;
            case 2:
                target = GizBuildIt_Find(WORLD, target_name);
                break;
            case 3:
                target = GizmoFindByName(WORLD->gizmo_sys, -1, target_name);
                break;
            case 4:
                gizmo_type = blowup_gizmotype_id;
                goto find_typed_gizmo;
            case 7:
                gizmo_type = force_gizmotype_id;
                goto find_typed_gizmo;
            case 8:
                gizmo_type = obstacle_gizmotype_id;
                goto find_typed_gizmo;
            case 9: {
                gizmo_type = zipup_gizmotype_id;
            find_typed_gizmo:
                GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, gizmo_type, target_name);
                target = gizmo != NULL ? gizmo->object : NULL;
                break;
            }
            case 5:
                if (fake_animation_id != 0) {
                    return NULL;
                }
                break;
            case 6:
                target = FlowBoxFindByName(WORLD->giz_flow, target_name);
                break;
            default:
                break;
        }
    }
    if (target == NULL && NuSpecialExistsFn(&special) == 0 && target_type != 5) {
        return NULL;
    }

    NULISTLNK *node = NuLinkedListGetHead(&control_system->available_controllers);
    if (node == NULL) {
        return NULL;
    }
    NuLinkedListRemove(&control_system->available_controllers, node);
    NuLinkedListAppend(&control_system->active_controllers, node);

    AIPATHCNXCONTROLLER_s *controller = reinterpret_cast<AIPATHCNXCONTROLLER_s *>(node);
    AIPATHSYS *path_system = ai_system->path_sys;
    const u8 path_count = path_system->path_count;
    AIPATH **path_cursor = path_system->paths;
    for (u8 index = 0; index < path_count; ++index, ++path_cursor) {
        if (*path_cursor == path) {
            controller->path_index = index;
            break;
        }
    }
    controller->flags = static_cast<u8>((controller->flags & ~1) | (direction & 1));
    controller->connection = connection;
    controller->gizmo_output = gizmo_output;
    if (target != NULL) {
        controller->target = target;
    } else if (NuSpecialExistsFn(&special) != 0) {
        controller->special = special;
    } else {
        controller->fake_animation_id = fake_animation_id;
    }
    controller->target_type = static_cast<u8>(target_type);
    return controller;
}

void AIPathCnxControllerDestroy(AIPATHCNXCONTROLSYS_s *system, AIPATHCNXCONTROLLER_s *controller) {
    if (system == NULL || controller == NULL) {
        return;
    }
    NuLinkedListRemove(&system->active_controllers, &controller->links);
    memset(controller, 0, sizeof(*controller));
    NuLinkedListAppend(&system->available_controllers, &controller->links);
}

void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame) {
    if (controller == NULL) {
        return;
    }

    i32 animation_end = 1;
    switch (controller->target_type) {
        case 0: {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&controller->special);
            if (animation != NULL) {
                nuanimdata_s *data = controller->special.scene->instance_animation_data[animation->anim_ix];
                if (data != NULL)
                    animation_end = static_cast<i32>(NuAnimEndFrameOld(data));
            }
            break;
        }
        case 1: {
            u8 *animation = *reinterpret_cast<u8 **>(static_cast<u8 *>(controller->target) + 0x58);
            animation_end = static_cast<i32>(*reinterpret_cast<f32 *>(animation + 8));
            break;
        }
        case 2:
        case 3:
        case 4:
        case 6:
            return;
        case 5:
            animation_end = static_cast<i32>(fakeanimendframe[controller->fake_animation_id]);
            break;
        default:
            animation_end = 1;
            break;
    }
    if (animation_end > 1024) {
        animation_end = 1024;
    }

    i32 first = start_frame == -1 ? animation_end : start_frame == -2 ? animation_end - 1 : start_frame;
    i32 last = end_frame == -1 ? animation_end : end_frame == -2 ? animation_end - 1 : end_frame;
    first = first > 0 ? first - 1 : 0;
    last = last < animation_end ? last : animation_end;
    for (i32 frame = first; frame < last; ++frame) {
        controller->on_frames[frame >> 5] |= 1u << (frame & 31);
    }
}

void AIPathCnxControlSysUpdate(AIPATHCNXCONTROLSYS_s *system) {
    if (system == NULL)
        return;
    for (NULISTLNK *node = NuLinkedListGetHead(&system->active_controllers); node != NULL;
         node = NuLinkedListGetNext(&system->active_controllers, node)) {
        AIPATHCNXCONTROLLER_s *controller = reinterpret_cast<AIPATHCNXCONTROLLER_s *>(node);
        controller->flags &= ~0x20;
        bool on = true;
        switch (controller->target_type) {
            case 0: {
                if ((controller->flags & 4) != 0 && NuSpecialGetVisibilityFn(&controller->special) == NULL) {
                    on = false;
                    break;
                }
                i32 frame = 0;
                nuinstanim_s *animation = NuSpecialGetInstAnim(&controller->special);
                if (animation != NULL) {
                    nuanimdata_s *data = controller->special.scene->instance_animation_data[animation->anim_ix];
                    if (data != NULL) {
                        float end = NuAnimEndFrameOld(data);
                        i32 current = static_cast<i32>(animation->ltime);
                        if (current > 0) {
                            if (current < 1024)
                                frame = current;
                            else
                                frame = 1024;
                            i32 end_frame = static_cast<i32>(end);
                            if (frame < end_frame)
                                frame = frame;
                            else
                                frame = end_frame;
                            --frame;
                        }
                    }
                }
                on = (controller->on_frames[frame / 32] & (1u << (frame % 32))) != 0;
                break;
            }
            case 1: {
                const u8 *instance = static_cast<const u8 *>(controller->target);
                float end = *reinterpret_cast<f32 *>(*reinterpret_cast<u8 *const *>(instance + 0x58) + 8);
                i32 frame = static_cast<i32>(*reinterpret_cast<const f32 *>(instance + 0x90));
                if (frame > 0) {
                    i32 end_frame = static_cast<i32>(end);
                    if (frame < end_frame)
                        frame = frame;
                    else
                        frame = end_frame;
                    if (frame < 1024)
                        frame = frame;
                    else
                        frame = 1024;
                    --frame;
                } else {
                    frame = 0;
                }
                on = (controller->on_frames[frame / 32] & (1u << (frame % 32))) != 0;
                break;
            }
            case 2:
                on = static_cast<GIZBUILDIT_s *>(controller->target)->build_state == 2;
                break;
            case 3:
                on = GizmoGetOutput(WORLD->gizmo_sys, static_cast<GIZMO *>(controller->target),
                                    controller->gizmo_output, 1) != 0;
                break;
            case 4:
                on = (static_cast<GIZMOBLOWUP_s *>(controller->target)->status_flags & 1) == 0;
                break;
            case 5: {
                i32 frame = static_cast<i32>(fakeanimframe[controller->fake_animation_id]);
                if (frame > 0) {
                    i32 end_frame = static_cast<i32>(fakeanimendframe[controller->fake_animation_id]);
                    if (frame < end_frame)
                        frame = frame;
                    else
                        frame = end_frame;
                    if (frame < 1024)
                        frame = frame;
                    else
                        frame = 1024;
                    --frame;
                } else {
                    frame = 0;
                }
                on = (controller->on_frames[frame / 32] & (1u << (frame % 32))) != 0;
                break;
            }
            case 6:
                on = (static_cast<FLOWBOX_s *>(controller->target)->state_flags & 2) != 0;
                break;
            case 7:
                on = GizForce_Complete(static_cast<GIZFORCE_s *>(controller->target)) != 0;
                break;
            case 8:
                on = static_cast<GIZOBSTACLE_s *>(controller->target)->anim_set->state == GAMEANIMSET_STATE_AT_END;
                break;
            case 9:
                on = (static_cast<ZIPUP *>(controller->target)->flags & (ZIPUP_FLAG_ACTIVE | ZIPUP_FLAG_VISIBLE)) ==
                     (ZIPUP_FLAG_ACTIVE | ZIPUP_FLAG_VISIBLE);
                break;
        }
        if (on) {
            controller->flags |= 0x20;
            controller->connection->open = (controller->flags >> 3) & 1;
            AIPATHCNX *connection = controller->connection;
            connection->traversal_flags[controller->flags & 1] &= ~controller->off_flags;
            connection->traversal_flags[controller->flags & 1] |= controller->on_flags;
            if ((controller->flags & 2) != 0) {
                connection->traversal_flags[(~controller->flags) & 1] &= ~controller->off_flags;
                connection->traversal_flags[(~controller->flags) & 1] |= controller->on_flags;
            }
        } else {
            controller->connection->open = (controller->flags >> 4) & 1;
            AIPATHCNX *connection = controller->connection;
            connection->traversal_flags[controller->flags & 1] &= ~controller->on_flags;
            connection->traversal_flags[controller->flags & 1] |= controller->off_flags;
            if ((controller->flags & 2) != 0) {
                connection->traversal_flags[(~controller->flags) & 1] &= ~controller->on_flags;
                connection->traversal_flags[(~controller->flags) & 1] |= controller->off_flags;
            }
        }
    }
}

i32 oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent) {
    if (object == NULL) {
        return 0;
    }
    if ((object->field_0xf01 & 0x20) == 0) {
        return 1;
    }
    if (object->one_at_once_player == 0xff) {
        return 0;
    }
    return Player[object->one_at_once_player] == opponent;
}

f32 oneAtOnce_GetHoldRange(GameObject_s *object) {
    APIOBJECT *opponent = object->ai.opponent_object;
    const i32 player_index = opponent->field_0x27c;
    if (static_cast<u32>(player_index) > 8) {
        return 0.75f;
    }

    AtOnceAttacker *attackers = AtOnce_attackingPlayer[player_index];
    i32 unassigned_in_row = 0;
    i32 row = 0;
    i32 index = 0;
    GameObject_s *candidate = attackers[index].object;
    while (candidate != NULL && index < 17) {
        if (candidate == object) {
            return static_cast<f32>(row) * AtOnce_RowDist + AtOnce_InitialRowDist;
        }
        unassigned_in_row += candidate->one_at_once_player == 0xff;
        if (unassigned_in_row >= AtOnce_attackersPerRow) {
            ++row;
            unassigned_in_row = 0;
        }
        ++index;
        candidate = attackers[index].object;
    }
    return AtOnce_InitialRowDist;
}

void oneAtOnce_SetNumAttackers(i32 attackers) {
    if (attackers < 0) {
        AtOnce_maxAttackers = 0;
        return;
    }
    if (attackers >= 5) {
        attackers = 4;
    }
    AtOnce_maxAttackers = attackers;
}

void oneAtOnce_SetAttackersPerRow(i32 attackers) {
    AtOnce_attackersPerRow = MAX(0, attackers);
}

void oneAtOnce_SetDistPerRow(float distance) {
    AtOnce_RowDist = MAX(0.0f, distance);
}

void oneAtOnce_SetInitDistPerRow(float distance) {
    AtOnce_InitialRowDist = MAX(0.0f, distance);
}

void oneAtOnce_MaintainArray() {
    GameObject_s *previous_attackers[8][4];
    i32 previous_count[8] = {};
    i32 attacker_count[8] = {};

    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *object = &Obj[index];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if (__builtin_expect(object == NULL || (object->apiobj.field_0x1f8 & character_flags) != character_flags ||
            object->ai.opponent == NULL || (object->field_0xf01 & 0x20) == 0, 0)) {
            continue;
        }

        const u8 old_player = object->one_at_once_player;
        if (__builtin_expect(old_player != 0xff, 1)) {
            previous_attackers[old_player][previous_count[old_player]++] = object;
        }
        object->one_at_once_player = 0xff;

        APIOBJECT *opponent_api = static_cast<APIOBJECT *>(object->ai.opponent);
        GameObject_s *opponent = opponent_api != NULL ? opponent_api->objptr : NULL;
        i32 player_index = -1;
        for (i32 player = 0; player < 8; ++player) {
            if (Player[player] != opponent) {
                continue;
            }
            player_index = player;
            break;
        }
        if (player_index < 0) {
            continue;
        }

        const i32 slot = attacker_count[player_index];
        AtOnce_attackingPlayer[player_index][slot].object = object;
        AtOnce_attackingPlayer[player_index][slot].distance =
            NuVecDistSqr(&opponent->apiobj.collision_position, &object->apiobj.collision_position, NULL);
        ++attacker_count[player_index];
    }

    for (i32 player = 0; player < 8; ++player) {
        const i32 count = attacker_count[player];
        AtOnce_attackingPlayer[player][count].object = NULL;
        AtOnce_attackingPlayer[player][count].distance = 1.0e9f;
    }

    for (i32 player = 0; player < 8; ++player) {
        if (Player[player] == NULL) {
            continue;
        }
        const i32 count = attacker_count[player];
        for (i32 slot = 0; slot < count && AtOnce_attackingPlayer[player][slot].object != NULL; ++slot) {
            if (previous_count[player] > 0 &&
                AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][0]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            if (previous_count[player] > 1 &&
                AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][1]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            if (previous_count[player] > 2 &&
                AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][2]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            if (previous_count[player] > 3 &&
                AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][3]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            for (i32 adjacent = 1; adjacent <= count - slot; ++adjacent) {
                if (AtOnce_attackingPlayer[player][adjacent].distance <
                    AtOnce_attackingPlayer[player][adjacent - 1].distance) {
                    GameObject_s *object = AtOnce_attackingPlayer[player][adjacent - 1].object;
                    const f32 distance = AtOnce_attackingPlayer[player][adjacent - 1].distance;
                    AtOnce_attackingPlayer[player][adjacent - 1].object =
                        AtOnce_attackingPlayer[player][adjacent].object;
                    AtOnce_attackingPlayer[player][adjacent - 1].distance =
                        AtOnce_attackingPlayer[player][adjacent].distance;
                    AtOnce_attackingPlayer[player][adjacent].object = object;
                    AtOnce_attackingPlayer[player][adjacent].distance = distance;
                }
            }
        }

        if (Player[player] != NULL) {
            const i32 attack_limit = AtOnce_maxAttackers < count ? AtOnce_maxAttackers : count;
            for (i32 slot = 0; slot < attack_limit && AtOnce_attackingPlayer[player][slot].object != NULL; ++slot) {
                AtOnce_attackingPlayer[player][slot].object->one_at_once_player = static_cast<u8>(player);
            }
        }
    }
}

void GameAIProcess() {
    if (TimingBarSet == 4)
        TBOPENFN("(Sys)", 4);
    ai_fighting = 0;
    AISysProcess(WORLD->ai_sys, reinterpret_cast<APIOBJECT *>(player), reinterpret_cast<APIOBJECT *>(player2));

    GameObject_s *object = Obj;
    u8 high_flags;
    i32 object_limit = HIGHGAMEOBJECT;
    // Keep the first-loop counter available in edx at the latch and body entry.
    for (i32 index = 0; index < object_limit; ++object, ++index) {
        f32 frame_time = FRAMETIME;
        high_flags = object->apiobj.flags_high;
        if (!__builtin_expect((high_flags & 0x10) != 0 && object->apiobj.field_0x287 == 0, 1)) {
            continue;
        }
        object->ai.capabilities &= ~0x00e00000u;
        if (object->apiobj.player_controlled && TouchHacks::TouchControlsActive) {
            if ((object->ai.capabilities & 1) != 0)
                object->ai.capabilities |= 0x00200000;
            if ((object->ai.capabilities & 2) != 0)
                object->ai.capabilities |= 0x00400000;
            if ((object->ai.capabilities & 0x20) != 0)
                object->ai.capabilities |= 0x00800000;
        }
        object->field_0xefd &= 0x7f;
        object->field_0xf01 &= 0x3f;
        object->field_0xf02 &= 0xfc;
        object->field_0x107e = 0;
        if (__builtin_expect(object->use_action_frames != 0, 1)) {
            --object->use_action_frames;
        } else if (object->use_action_parameter > 0.0f) {
            object->use_action_parameter -= frame_time;
        } else {
            object->can_use_object = NULL;
            object->use_action = 0;
        }
        if (object->field_0x1094 != 0)
            --object->field_0x1094;
        object->apiobj.visibility_range_extension =
            (object->ai.field_0x1e5 & 0x40) != 0 ? draw_attention_distance : 0.0f;
        i32 mini_cut_cam = MiniCutCam;
        if (mini_cut_cam != 0 && (high_flags & 1) != 0) {
            i32 gamepad_start = GAMEPAD_START;
            GAMEPAD_s *pad = object->pad_gamepad;
            pad->buttons_held &= gamepad_start;
            pad->buttons_pressed &= gamepad_start;
        }
        if (object->alert_target != NULL) {
            object->alert_target_timer -= frame_time;
            if (__builtin_expect(object->alert_target_timer <= 0.0f, 0)) {
                object->alert_target = NULL;
                object->alert_target_timer = 0.0f;
            }
        }
    }
    f32 follow_offset = 0.0f;
    active_neutral_count = 0;
    party_under_cover = 0;
    i32 all_under_cover = 1;
    i32 cover_result = 0;
    GameObject_s **player_slots = Player;
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object = player_slots[index];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) && (object->field_0xeff & 1) == 0 &&
            (VADER_ADATA == NULL || WORLD->area != VADER_ADATA) && party_cant_be_under_cover == 0) {
            if (object->field_0x108e == 5 ||
                ((FreePlay != 0 || WORLD->current_level == HUB_LDATA) &&
                 (object->apiobj.character_data->model_flags & 0x204) != 0 && object->field_0xcc0 == NULL)) {
                if (alert_obj == NULL || alert_obj->apiobj.field_0x27c == -1 || (alert_obj->field_0xeff & 1) != 0)
                    cover_result = 1;
            }
            all_under_cover = (object->apiobj.character_data->model_flags & 0x80000) != 0 ? all_under_cover : 0;
        }
        party_follow_offsets[index] = 0.5f;
    }
    party_under_cover = cover_result;
    if ((VADER_ADATA == NULL || WORLD->area != VADER_ADATA) && __builtin_expect(all_under_cover != 0, 0) &&
        party_cant_be_under_cover == 0) {
        party_under_cover = 1;
    }
    AITriggerSetSysProcess(WORLD->ai_trigger_set_sys);

    f32 drop_timer = drop_back_in_timer;
    APIOBJECT *object_lists[4][64];
    // Keep the original stack frame and pointer-list offsets while scratch slots are identified.
    APIOBJECT **neutral_objects = object_lists[0];
    APIOBJECT **interactive_objects = object_lists[1];
    APIOBJECT **goodies = object_lists[2];
    APIOBJECT **baddies = object_lists[3];
    i32 neutral_count = 0;
    i32 interactive_count = 0;
    i32 goody_count = 0;
    i32 baddy_count = 0;
    GameObject_s *active_player = player;
    APIOBJECT **volatile neutral_cursor = neutral_objects;
    APIOBJECT **volatile goody_cursor = goodies;
    APIOBJECT **baddy_cursor = baddies;
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object;
        if (index < 2 && (object = Player[1]) == active_player) {
            if (index != 0)
                object = player_slots[0];
        } else {
            object = player_slots[index];
        }
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) && object->character_context != 0x17 &&
            object->character_context != 0x5a) {
            if (party_under_cover != 0 && (object->ai.field_0x1e5 & 0x40) != 0)
                party_under_cover = 0;
            if (!(object->camera_screen_position.x > -0.85f && object->camera_screen_position.x < 0.85f &&
                  object->camera_screen_position.y > -0.85f && object->camera_screen_position.y < 0.85f) &&
                (object->tag_flags & 2) == 0 && drop_timer > 0.0f) {
                object->ai.nearest_opponent = NULL;
                object->ai.pending_nearest_opponent = NULL;
                object->ai.pending_nearest_metric = 1.0e9f;
                object->ai.opponent = NULL;
                object->ai.pending_opponent = NULL;
                object->ai.pending_opponent_metric = 1.0e9f;
                object->ai.field_0x1e5 &= ~0x10;
            } else if ((object->apiobj.field_0x1f4 & 1) != 0) {
                ++baddy_count;
                *baddy_cursor++ = &object->apiobj;
            } else if ((object->apiobj.field_0x1f4 & 4) != 0) {
                ++neutral_count;
                *neutral_cursor++ = &object->apiobj;
            } else {
                ++goody_count;
                *goody_cursor++ = &object->apiobj;
            }
        }
        object = Player[index];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) && (object->field_0xeff & 1) == 0 &&
            !object->apiobj.player_controlled) {
            party_follow_offsets[object->apiobj.field_0x27c] = follow_offset;
            follow_offset += 0.2f;
        }
    }
    u64 awareness = 0;
    NUVEC wall_direction;
    u8 shifted_flag;
    u8 cleared_flag;
    object = Obj;
    APIOBJECT **interactive_cursor = interactive_objects;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        NUVEC direction;
        i32 ground_checks;
        u8 ai_update_flags;
        GAMEPAD_s *pad;
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        object->active_trigger_set = NULL;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags || object->apiobj.field_0x287 != 0 ||
            object->character_context == 0x3b) {
            goto interaction;
        }

        ground_checks = object->apiobj.field_0x27d != 0;
        if (ground_checks == 0) {
            ground_checks = (object->apiobj.character_data->model_flags >> 13) & 1;
        }

        if ((object->apiobj.field_0x1f4 & 0x400) != 0) {
            if ((object->apiobj.field_0x1f4 & 0x10000) != 0) {
                ++baddy_count;
                *baddy_cursor++ = &object->apiobj;
                ++goody_count;
                *goody_cursor++ = &object->apiobj;
            } else if ((object->apiobj.field_0x1f4 & 1) != 0) {
                ++baddy_count;
                *baddy_cursor++ = &object->apiobj;
            } else if ((object->apiobj.field_0x1f4 & 4) != 0) {
                ++neutral_count;
                *neutral_cursor++ = &object->apiobj;
            } else {
                ++goody_count;
                *goody_cursor++ = &object->apiobj;
            }
        }
        if ((object->apiobj.field_0x1f4 & 0x40000) != 0) {
            if ((object->apiobj.flags_high & 0x10) != 0 && object->apiobj.field_0x287 == 0 &&
                object->context_target_position == NULL &&
                (object->apiobj.character_data->game_character->flags_090 & 0x8000) == 0) {
                ++interactive_count;
                *interactive_cursor++ = &object->apiobj;
                object->apiobj.resolved_collision_priority = 0;
                object->apiobj.collision_priority = HIGHGAMEOBJECT - object->apiobj.field_0x289;
                object->ai.field_0x1e5 |= 0x20;
            } else if (object->field_0x101c > 0.0f) {
                object->ai.field_0x1e5 |= 0x20;
            } else {
                object->ai.nearest_opponent = NULL;
                object->ai.opponent = NULL;
                object->ai.field_0x1e5 &= ~0x20;
            }
            if (object->opponent != NULL && (static_cast<APIOBJECT *>(object->opponent)->flags_low & 1) == 0)
                object->opponent = NULL;
            if (object->ai.opponent_object != NULL && ((object->ai.opponent_object->flags_low & 1) == 0 ||
                                                       (object->ai.opponent_object->ai->field_0x1e5 & 0x20) == 0)) {
                object->ai.opponent_metric = 1.0e9f;
                object->ai.opponent = NULL;
            }
            if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0)
                AISysUpdateCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, ground_checks,
                                            object->ai_elapsed_time);
            continue;
        }
        pad = object->pad_gamepad;
        if (pad->pad == NULL) {
            pad->buttons_held = 0;
            pad->buttons_pressed = 0;
            pad->buttons_released = 0;
            pad->left_directions = 0;
            pad->previous_left_directions = 0;
            pad->right_directions = 0;
            pad->previous_right_directions = 0;
            pad->unknown_20 = 0;
        }
        awareness |= object->apiobj.ai_awareness_mask;
        // Network-controlled objects use the direct field test above. The
        // scripted path snapshots its update flags after resetting the pad.
        ai_update_flags = object->field_0xf00;
        if ((ai_update_flags & GAME_OBJECT_AI_UPDATE_PROCESS) != 0) {
            object->script_fire_target = NULL;
            shifted_flag = (object->field_0xef9 << 1) & 4;
            cleared_flag = object->field_0xef9 & ~4;
            cleared_flag |= shifted_flag;
            object->field_0xef9 = cleared_flag;
            object->field_0xef8 &= ~0x20;
            if (__builtin_expect((cleared_flag & 0x80) != 0, 1)) {
                object->field_0xef9 &= ~0x80;
                object->field_0xe64 = object->field_0xe58;
            }
            object->field_0xefa &= ~3;
        }
        AISysProcessCharacter(WORLD->ai_sys, &object->apiobj, &object->ai, ground_checks, object->ai_elapsed_time, 0,
                              (ai_update_flags >> 3) & 1);
        goto ai_precombat;
    ai_combat:
        if ((object->apiobj.field_0x1f8 & 0x180) == 0x80) {
            object->field_0xefc &= ~4;
            object->apiobj.flags_high =
                (object->apiobj.flags_high & ~2) | ((object->apiobj.character_data->model_flags >> 27) & 2);
        } else {
            object->apiobj.flags_high &= ~2;
            i32 defend = false;
            if ((object->field_0xef8 & 0x80) != 0 && object->ai.opponent_object != NULL) {
                BOLTTYPE_s *bolt = BoltType_FindByID(0, WORLD);
                if (bolt->field_10 * bolt->field_14 > object->ai.opponent_metric) {
                    NUVEC forward;
                    NuVecSub(&direction, &object->ai.opponent_object->collision_position,
                             &object->apiobj.collision_position);
                    f32 scale = object->ai.opponent_metric == 0.0f ? 0.0f : 1.0f / object->ai.opponent_metric;
                    NuVecScale(&direction, &direction, scale);
                    NuVecRotateY(&forward, &v001, object->apiobj.movement_facing_angle);
                    f32 dot = NuVecDot(&direction, &forward);
                    u16 angle = object->ai.opponent_metric < 0.5f   ? 0x3aaa
                                : object->ai.opponent_metric < 1.0f ? 0x3000
                                                                    : 0x2555;
                    if (dot > NuTrigTable[angle]) {
                        object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                        object->script_fire_target = *object->ai.action_target_ref;
                    }
                }
                defend = true;
            }
            if (FreePlay != 0 && (object->apiobj.field_0x1f4 & 0x400) == 0 && object->id == id_DROIDEKA)
                object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLERIGHT;
            if ((object->field_0xef8 & 2) != 0 &&
                ((object->apiobj.field_0x1f4 & 0x400) != 0 || party_under_cover == 0 ||
                 (object->field_0xeff & 1) != 0) &&
                (CanFightLikeAJedi(object) != 0 || object->id == id_GAMORREANGUARD)) {
                if ((object->field_0xef8 & 1) != 0 || object->ai.opponent_object != NULL ||
                    (object->field_0xe22 & 8) != 0)
                    object->field_0xed8 = DEFENDTIME;
                if (object->field_0xed8 > 0.0f) {
                    defend = true;
                    if ((object->field_0xe22 & 9) == 9)
                        object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                }
            }
            if ((object->field_0xefb & 0x20) != 0) {
                defend = true;
                if ((object->field_0xe22 & 1) != 0) {
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                    object->field_0xefb &= ~0x20;
                }
            }
            if ((object->field_0xef8 & 8) != 0 &&
                (defend || (object->field_0xef8 & 0x30) != 0 ||
                 ((object->id == id_YODA || object->id == id_YODAGHOST) && !object->apiobj.player_controlled &&
                  object->apiobj.field_0x27c != -1))) {
                if ((object->field_0xe22 & 1) == 0 && object->character_context != 7 && object->field_0xe32 == 0)
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
            } else if ((object->pad_gamepad->buttons_pressed & GAMEPAD_ACTION) == 0 && (object->field_0xe22 & 1) != 0 &&
                       object->character_context != 6 && object->field_0xe32 == 0) {
                object->pad_gamepad->buttons_pressed |= GAMEPAD_SPECIAL;
            }
        }
        if ((object->apiobj.character_data->game_character->flags_094[3] & 0x10) != 0 &&
            !object->apiobj.player_controlled) {
            f32 threshold = object->apiobj.character_data->game_character->walk_speed - 0.01f;
            if (object->field_0xe31 == 1) {
                if (threshold >= object->pad_gamepad->input_magnitude) {
                    object->ai_jump_timer += FRAMETIME;
                    if (object->ai_jump_timer > 0.1f) {
                        object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
                        object->ai_jump_timer = 0.0f;
                    }
                } else {
                    object->ai_jump_timer = 0.0f;
                }
            } else if (object->pad_gamepad->input_magnitude > threshold) {
                object->ai_jump_timer += FRAMETIME;
                if (object->ai_jump_timer > 1.0f) {
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
                    object->ai_jump_timer = 0.0f;
                }
            } else {
                object->ai_jump_timer = 0.0f;
            }
        }
        if (__builtin_expect(object->ai.action_target_ref != NULL, 0))
            SetObjAsHeadTarget(object, *object->ai.action_target_ref, 2, 1.0f, 0.0f, 0.0f);
        goto interaction;
    ai_precombat:
        if (__builtin_expect(!object->apiobj.player_controlled, 1)) {
            if (object->character_context == 0x5d)
                SetBallooningHeight(object, object->ai.movement_destination.y);
            if (__builtin_expect(WORLD->current_level == JEDI_B_LDATA, 0) && object->id == id_JANGOFETT &&
                (object->field_0xefb & 8) != 0 && object->hover_height_override != 1.0e9f && object->field_0xe31 == 0)
                object->field_0xe31 = 1;
            if (object->apiobj.field_0x27c != -1 && object->field_0xe31 != 0 &&
                (object->ai.capabilities & LEGO_AIPATHCNX_R2D2GLIDE) == 0)
                object->field_0xe31 = 0;
            if (object->character_context == 0x4b && object->suit != NULL &&
                (static_cast<SUIT_s *>(object->suit)->flags & 0x10) != 0)
                object->pad_gamepad->buttons_held |= GAMEPAD_SPECIAL;
            AIPATHCNX *connection = object->ai.path_info.connection;
            if (connection != NULL && (connection->traversal_flags[object->ai.path_info.direction] &
                                       object->ai.capabilities & LEGO_AIPATHCNX_WALLSHUFFLE) != 0)
                goto ai_wall_shuffle;
            if (!(object->ai.path_connection_state == 0 && (object->ai.path_info.flags & 1) != 0 &&
                  object->apiobj.movement_stuck_time > jump_stuck_time))
                goto ai_route;
            if (object->apiobj.supporting_platform_id != -1 && connection != NULL &&
                (connection->original_traversal_flags[0] & LEGO_AIPATHCNX_BLOCKAGE) != 0) {
                if ((object->apiobj.character_data->model_flags & 0x88) != 0) {
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                    object->apiobj.movement_stuck_time = 0.0f;
                    GameObjectSetCanUse(object, NULL, 5, 0, 1.0f);
                } else if (FreePlay != 0) {
                    object->input_toggle_hold_time -= FRAMETIME;
                    if (object->input_toggle_hold_time <= 0.0f) {
                        Player_ToggleCharacter(object, 1, 0);
                        if (object->id == id_DROIDEKA)
                            Player_ToggleCharacter(object, 1, 0);
                        if (object->apiobj.model_draw_result == 0)
                            object->input_toggle_hold_time = 0.0f;
                        else
                            object->input_toggle_hold_time = 0.25f;
                    }
                }
            } else if ((object->ai.capabilities & (LEGO_AIPATHCNX_R2D2GLIDE | LEGO_AIPATHCNX_JUMP)) != 0 &&
                       object->field_0xe31 == 0) {
                object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
                object->apiobj.movement_stuck_time = 0.0f;
            } else {
                object->ai.field_0x1e6 |= AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
            }
            goto ai_route;
        ai_wall_shuffle: {
            NUVEC lateral, ray, movement;
            object->field_0xf02 |= 1;
            AIPATHNODE *nodes = object->ai.path_info.path->nodes;
            i32 direction = object->ai.path_info.direction;
            u8 other_node = connection->node_indices[direction == 0];
            u8 node = connection->node_indices[direction];
            u16 angle;
            if (object->character_context == 0x45) {
                angle = object->takeover_start_angle;
                wall_direction = v001;
                NuVecRotateY(&wall_direction, &wall_direction, angle);
            } else {
                NuVecSub(&lateral, &nodes[other_node].position, &nodes[node].position);
                lateral.y = 0.0f;
                NuVecNorm(&lateral, &lateral);
                NuVecRotateY(&lateral, &lateral, 0x4000);
                ray = lateral;
                f32 reverse_x = -lateral.x;
                f32 reverse_z = -lateral.z;
                if (GameRayCast(&nodes[node].position, &ray, 0.0f,
                                TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0) {
                    f32 first_distance = NuVecMag(&ray);
                    ray.x = reverse_x;
                    ray.y = 0.0f;
                    ray.z = reverse_z;
                    if (GameRayCast(&nodes[node].position, &ray, 0.0f,
                                    TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0) {
                        f32 second_distance = NuVecMag(&ray);
                        if (first_distance < 1.0e9f || second_distance < 1.0e9f) {
                            if (second_distance > first_distance) {
                                wall_direction = lateral;
                            } else {
                                wall_direction.x = -lateral.x;
                                wall_direction.y = 0.0f;
                                wall_direction.z = -lateral.z;
                            }
                        }
                    } else if (first_distance < 1.0e9f) {
                        wall_direction = lateral;
                    }
                } else {
                    ray.x = reverse_x;
                    ray.y = 0.0f;
                    ray.z = reverse_z;
                    if (GameRayCast(&nodes[node].position, &ray, 0.0f,
                                    TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0 &&
                        NuVecMag(&ray) < 1.0e9f) {
                        wall_direction.x = -lateral.x;
                        wall_direction.y = 0.0f;
                        wall_direction.z = -lateral.z;
                    }
                }
                angle = NuAtan2D(wall_direction.x, wall_direction.z);
            }
            movement.x = object->ai.movement_position.x - object->apiobj.position.x;
            movement.y = 0.0f;
            movement.z = object->ai.movement_position.z - object->apiobj.position.z;
            f32 distance = NuVecMag(&movement);
            if (__builtin_expect(distance > ai_moveradius, 1))
                NuVecScale(&movement, &movement, (ai_moveradius / distance) * 2.0f);
            NuVecRotateY(&movement, &movement, -static_cast<i32>(angle));
            movement.z = 0.0f;
            NuVecRotateY(&movement, &movement, angle);
            NuVecScale(&wall_direction, &wall_direction, ai_moveradius);
            NuVecAdd(&movement, &movement, &wall_direction);
            object->ai.movement_position.x = object->apiobj.position.x + movement.x;
            object->ai.movement_position.y = object->apiobj.position.y;
            object->ai.movement_position.z = object->apiobj.position.z + movement.z;
        }
        }
    ai_route:
        if (FreePlay != 0 && (object->apiobj.field_0x1f4 & 0x400) == 0) {
            if (!object->apiobj.player_controlled && object->character_context != 0x0b) {
                if (drop_in_teleport == 1 && VehicleArea == 0 && (Arcade != 0 || teleport_all_freeplay_modes != 0) &&
                    object->apiobj.model_draw_result == 0 && (object->tag_flags & 2) == 0 &&
                    drop_back_in_timer > 0.0f) {
                    NUVEC position = player->apiobj.last_safe_position;
                    if (position.x == player->apiobj.position.x && position.y == player->apiobj.position.y &&
                        position.z == player->apiobj.position.z)
                        position.z += 0.01f;
                    object->apiobj.start_position = position;
                    object->apiobj.position = object->apiobj.start_position;
                    object->apiobj.velocity = v000;
                    object->apiobj.field_0x276 = player->apiobj.field_0x276;
                    ResetPlayerMoves(object);
                    object->apiobj.movement_stuck_time = 0.0f;
                    InitSurfaceInfo(object);
                    SetObjOnSurface(object, 1);
                    GameObjectOrigin(object);
                    AddGameDebris(WORLD->debris_sys, 0x5c, &object->apiobj.collision_position);
                    goto ai_combat;
                }
                if ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_ROUTE_SELECTED) != 0) {
                    object->ai.current_route = 0xff;
                    object->route_character_id = -1;
                    object->field_0xf14 = 0;
                } else if ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_USING_PATH_WAYPOINT) != 0 &&
                           ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_PATH_BLOCKED) != 0 ||
                            (object->ai.path_info.connection != NULL &&
                             object->ai.path_info.connection->route_mask != 0))) {
                    if ((object->field_0xefc & 0x40) != 0) {
                        if (object->field_0xf14 != object->ai.frame_state) {
                            object->route_character_id = -1;
                            object->field_0xefc &= ~0x40;
                            object->route_suit_index = -1;
                            object->field_0xf14 = 0;
                        }
                    } else if ((object->ai.frame_state & LEGO_AIPATHCNX_DONTTOGGLE) == 0) {
                        if (object->route_character_id == -1 || object->field_0xf14 != object->ai.frame_state ||
                            object->route_character_id == id_DROIDEKA) {
                            object->route_character_id = object->id;
                            object->route_suit_index =
                                object->suit != NULL ? Suit_GetIndex(static_cast<SUIT_s *>(object->suit)) : -1;
                            object->input_toggle_hold_time = 0.0f;
                            object->field_0xf14 = object->ai.frame_state;
                            object->route_search_index = object->ai.next_route;
                            object->route_start_index = object->ai.next_route;
                        }
                        object->input_toggle_hold_time -= FRAMETIME;
                        if (object->input_toggle_hold_time <= 0.0f) {
                            Player_ToggleCharacter(object, 1, 0);
                            if (object->id == id_DROIDEKA)
                                Player_ToggleCharacter(object, 1, 0);
                            object->input_toggle_hold_time = object->apiobj.model_draw_result != 0 ? 0.25f : 0.0f;
                            if (object->id == object->route_character_id) {
                                const i32 route_suit_index = object->route_suit_index;
                                if (Suit_GetIndex(static_cast<SUIT_s *>(object->suit)) == route_suit_index) {
                                    Player_ToggleCharacter(object, -1, 0);
                                    if (object->id == id_DROIDEKA)
                                        Player_ToggleCharacter(object, -1, 0);
                                    object->route_character_id = object->id;
                                    object->route_suit_index = Suit_GetIndex(static_cast<SUIT_s *>(object->suit));
                                    object->ai.next_route = object->route_search_index;
                                    AISysFindRoute(&object->ai);
                                    object->route_search_index = object->ai.next_route;
                                    if (object->route_search_index == object->route_start_index)
                                        object->field_0xefc |= 0x40;
                                }
                            }
                        }
                    }
                } else {
                    object->route_character_id = -1;
                    object->field_0xefc &= ~0x40;
                    object->route_suit_index = -1;
                    object->field_0xf14 = 0;
                    object->route_search_index = 0;
                }
            }
        } else if ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_USING_PATH_WAYPOINT) != 0) {
            AISysFindRoute(&object->ai);
        }
        goto ai_combat;
    interaction:
        if ((object->apiobj.flags_high & 0x10) != 0 && object->apiobj.field_0x287 == 0 &&
            object->context_target_position == NULL &&
            (object->apiobj.character_data->game_character->flags_090 & 0x8000) == 0) {
            ++interactive_count;
            *interactive_cursor++ = &object->apiobj;
            object->apiobj.resolved_collision_priority = 0;
            object->apiobj.collision_priority = HIGHGAMEOBJECT - object->apiobj.field_0x289;
            if ((object->apiobj.flags_low & 2) != 0 || (object->field_0xeff & 0x80) != 0) {
                object->apiobj.collision_priority = 0x8000;
            } else if (!object->apiobj.player_controlled) {
                if (__builtin_expect((object->ai.field_0x1e7 & 0x80) != 0, 1))
                    object->apiobj.collision_priority |= 0x4000;
                if ((object->ai.field_0x1e7 & 0x40) != 0)
                    object->apiobj.collision_priority |= 0x2000;
                if ((object->ai.navigation_flags & 1) != 0)
                    object->apiobj.collision_priority |= 0x1000;
                if ((object->pad_gamepad->allocated_5a & 2) != 0)
                    object->apiobj.collision_priority |= 0x800;
                if ((object->ai.path_info.flags & 1) == 0 || object->ai.path_info.connection == NULL)
                    object->apiobj.collision_priority |= 0x400;
                if (object->ai.movement_parameter == 0.0f) {
                    const f32 delta_x = object->ai.fallback_destination.x - object->apiobj.position.x;
                    const f32 delta_z = object->ai.fallback_destination.z - object->apiobj.position.z;
                    const f32 radius = object->ai.mover_height + 1.0f;
                    direction.x = delta_x;
                    direction.z = delta_z;
                    if (direction.x * direction.x + direction.z * direction.z < radius * radius)
                        object->apiobj.collision_priority |= 0x200;
                }
            }
            object->ai.field_0x1e5 |= 0x20;
            if (object->opponent != NULL && (static_cast<APIOBJECT *>(object->opponent)->flags_low & 1) == 0)
                object->opponent = NULL;
            if (object->ai.opponent_object != NULL && ((object->ai.opponent_object->flags_low & 1) == 0 ||
                                                       (object->ai.opponent_object->ai->field_0x1e5 & 0x20) == 0)) {
                object->ai.opponent = NULL;
                object->ai.opponent_metric = 1.0e9f;
            }
            if ((object->field_0xeff & 8) != 0 || debug_ignore_players != 0 || Hub_InMenu() != 0) {
                object->ai_opponent_exclusion_mask |= (u64)1 << player->apiobj.field_0x289;
                if (player2 != NULL)
                    object->ai_opponent_exclusion_mask |= (u64)1 << player2->apiobj.field_0x289;
            }
            if (object->apiobj.player_controlled && object->apiobj.field_0x27d != 0 &&
                object->pad_gamepad->input_magnitude >
                    (WALKSTEALTH != 0 ? object->apiobj.character_data->game_character->walk_speed
                                      : object->apiobj.character_data->game_character->tiptoe_speed))
                object->field_0xef9 |= 8;
        } else if (object->field_0x101c > 0.0f) {
            object->ai.field_0x1e5 |= 0x20;
        } else {
            object->ai.nearest_opponent = NULL;
            object->ai.opponent = NULL;
            object->ai.field_0x1e5 &= ~0x20;
        }
    }
    if (TimingBarSet == 4)
        TBCLOSEFN("(Sys)", 4);
    active_goody_count = goody_count;
    active_baddy_count = baddy_count;
    active_neutral_count = neutral_count;
    if (TimingBarSet == 4)
        TBOPENFN("(LOS)", 4);
    for (i32 index = 0; index < neutral_count; ++index)
        baddies[baddy_count + index] = neutral_objects[index];
    i32 los_count = baddy_count + neutral_count;
    timetogetlos = (f32)(los_count * los_count) * FRAMETIME * 0.5f;
    APIObjectLOSChecks(WORLD->api_object_sys, 2, goody_count, goodies, los_count, baddies,
                       (f32) static_cast<u8>(WORLD->current_level->unknown_0da));
    if (TimingBarSet == 4)
        TBCLOSEFN("(LOS)", 4);
    if (TimingBarSet == 4)
        TBOPENFN("(Avoid)", 4);
    if (test_new_interaction == 0) {
        LEGO_AISysCreatureInteraction2D(WORLD->ai_sys, interactive_count, interactive_objects, NULL, FRAMETIME);
    } else {
        for (i32 index = 0; index < interactive_count; ++index) {
            AIPACKET *packet = interactive_objects[index]->ai;
            packet->antinode_timer -= FRAMETIME;
            if (packet->antinode_timer < 0.0f)
                packet->antinode_timer = 0.0f;
        }
        for (i32 first_index = 0; first_index < interactive_count - 1; ++first_index) {
            APIOBJECT *first = interactive_objects[first_index];
            for (i32 second_index = first_index + 1; second_index < interactive_count; ++second_index) {
                APIOBJECT *second = interactive_objects[second_index];
                f32 radius = first->ai->mover_height + second->ai->mover_height;
                NUVEC difference;
                difference.x = first->ai->movement_position.x - second->ai->movement_position.x;
                if (difference.x > radius || difference.x < -radius)
                    continue;
                difference.z = first->ai->movement_position.z - second->ai->movement_position.z;
                if (difference.z > radius || difference.z < -radius)
                    continue;
                if (first->collision_min.y > second->collision_max.y ||
                    second->collision_min.y > first->collision_max.y || first->collision_link == second ||
                    second->collision_link == first)
                    continue;
                bool fixed_first = first->player_controlled;
                bool fixed_second = second->player_controlled;
                if (first->collision_priority > second->collision_priority)
                    fixed_first = true;
                if (first->collision_priority == second->collision_priority)
                    continue;
                if (second->collision_priority > first->collision_priority)
                    fixed_second = true;
                if (fixed_first && fixed_second)
                    continue;
                f32 distance = NuFsqrt(difference.x * difference.x + difference.z * difference.z);
                if (distance < radius) {
                    difference.y = 0.0f;
                    if (__builtin_expect(!fixed_first, 1)) {
                        if (!fixed_second) {
                            first->resolved_collision_priority = second->collision_priority;
                            difference.x = -difference.x;
                            difference.z = -difference.z;
                            TestWalkAround(second, &first->position.x, &first->position.z, &difference, radius);
                        } else if (second->collision_priority > first->resolved_collision_priority) {
                            first->resolved_collision_priority = second->collision_priority;
                            TestWalkAround(first, &second->position.x, &second->position.z, &difference, radius);
                        }
                    } else if (first->collision_priority > second->resolved_collision_priority) {
                        second->resolved_collision_priority = first->collision_priority;
                        difference.x = -difference.x;
                        difference.z = -difference.z;
                        TestWalkAround(second, &first->position.x, &first->position.z, &difference, radius);
                    }
                    AIPACKET *first_ai = first->ai;
                    AIPACKET *second_ai = second->ai;
                    f32 first_timer = first_ai->antinode_timer;
                    f32 second_timer = second_ai->antinode_timer;
                    if (__builtin_expect(first_timer > antinode_time && second_timer > antinode_time, 1)) {
                        if (first_timer > second_timer) {
                            second_ai->antinode_timer = first_timer;
                            second_ai->antinode_clockwise = first_ai->antinode_clockwise;
                        } else {
                            first_ai->antinode_timer = second_timer;
                            first_ai->antinode_clockwise = second_ai->antinode_clockwise;
                        }
                    } else if (second_timer > antinode_time) {
                        first_ai->antinode_timer = second_timer;
                        first_ai->antinode_clockwise = second_ai->antinode_clockwise;
                    } else if (first_timer > 0.0f) {
                        second_ai->antinode_timer = first_timer;
                        second_ai->antinode_clockwise = first_ai->antinode_clockwise;
                    }
                    if (((first->field_0x1f4 ^ second->field_0x1f4) & 1) != 0) {
                        first->ai->field_0x1e5 |= 0x40;
                        second->ai->field_0x1e5 |= 0x40;
                    }
                }
            }
        }
        AISysCreatureAntinodeInteraction(WORLD->ai_sys, interactive_count, interactive_objects, NULL);
    }
    if (TimingBarSet == 4)
        TBCLOSEFN("(Avoid)", 4);
    GameObject_s **cleanup_players = Player;
    if (party_under_cover != 0) {
        for (i32 index = 0; index < 8; ++index) {
            GameObject_s *cover_player = cleanup_players[index];
            if (cover_player != NULL)
                cover_player->ai.field_0x1e5 &= ~0x40;
        }
    }
    if (TimingBarSet == 4)
        TBOPENFN("(Opponent)", 4);
    GameCreatureOpponentSelection(WORLD->ai_sys, interactive_count, interactive_objects, goody_count, goodies,
                                  baddy_count, baddies, awareness, FRAMETIME);
    if (TimingBarSet == 4)
        TBCLOSEFN("(Opponent)", 4);
    if (TimingBarSet == 4)
        TBOPENFN("(Turret)", 4);
    GizTurrets_OpponentSelection(WORLD->giz_turret_sys, goody_count, goodies, baddy_count, baddies);
    if (TimingBarSet == 4)
        TBCLOSEFN("(Turret)", 4);
    oneAtOnce_MaintainArray();
}

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set) {
    u16 yaw, pitch;
    NUVEC offset, spline_point;
    const bool has_no_spline = spline == NULL;
    if (position == NULL && spline == NULL) {
        return NULL;
    }

    if (NOAICREATURES != 0 && model != id_DRAGBOMB && (GCDataList[model].flags_090 & 0x40) == 0) {
        return NULL;
    }

    u32 arcade_mode;
    Arcade_GetMode(&arcade_mode);
    if ((arcade_mode & 0x10) != 0) {
        return NULL;
    }

    if (static_cast<u32>(model) >= 0x154 || apicharsys->playermodelids[model] == -1) {
        return NULL;
    }

    GameObject_s *object = AddCreature(model, 0);
    if (object == NULL) {
        return NULL;
    }

    object->apiobj.field_0x1f4 |= APIOBJECT_MOTION_FLAG_AI_CONTROLLED;
    CHARACTERDATA *character = &apicharsys->char_data[model];
    const u32 model_flags = character->model_flags;
    if ((model_flags & 0x200) != 0) {
        object->apiobj.field_0x1f4 |= 0x404;
    } else if ((model_flags & 0x400) != 0) {
        object->apiobj.field_0x1f4 |= 0x401;
    }
    if ((model_flags & 0x1000) != 0)
        object->field_0x1050 |= 5;
    else
        object->field_0x1050 |= 1;

    GAMECHARACTERDATA &game_character = *(GAMECHARACTERDATA *)character->field11_0x24;
    object->apiobj.viewdistance = game_character.viewdistance;
    object->apiobj.heardistance = game_character.heardistance;
    object->apiobj.maxviewheight = game_character.maxviewheight;
    object->apiobj.minviewheight = game_character.minviewheight;
    object->field_0xef9 &= static_cast<u8>(~8u);
    object->ai.pending_nearest_metric = 1000000000.0f;
    object->ai.pending_opponent_metric = 1000000000.0f;
    object->field_0xef8 &= static_cast<u8>(~1u);
    object->ai.field_0x1e5 &= static_cast<u8>(~0x50u);
    object->apiobj.ai_awareness_mask = 0;
    object->ai_seen_mask = 0;
    object->field_0xecc = 0;
    object->field_0xed0 = 0;
    object->field_0xed8 = 0;
    object->ai.nearest_opponent = NULL;
    object->ai.pending_nearest_opponent = NULL;
    object->ai.pending_opponent = NULL;
    object->ai.opponent = NULL;
    object->ai.antinode_timer = 0.0f;
    InitPlayerAI(object);

    if (spline != NULL) {
        SPLINEPOS_s *spline_position = reinterpret_cast<SPLINEPOS_s *>(&object->movement_spline);
        InitSplinePosition(spline_position, spline, 0.0f, spline_mode);
        SPLINEPOSITION_RUNTIME_s *runtime = reinterpret_cast<SPLINEPOSITION_RUNTIME_s *>(spline_position);
        PointAlongSpline(runtime->spline, runtime->normalized_position, &spline_point, &yaw, &pitch, runtime->looping);
        object->apiobj.movement_facing_angle = yaw;
        object->apiobj.facing_angle = yaw;
        object->apiobj.field_0x276 = yaw;
        object->apiobj.pitch_angle = static_cast<u16>(-pitch);

        if (spline_offset != NULL) {
            NUVEC *stored_offset = &object->movement_spline_offset;
            *stored_offset = *spline_offset;
            if (spline_offset->x != 0.0f || spline_offset->y != 0.0f || spline_offset->z != 0.0f) {
                NuVecRotateX(&offset, spline_offset, static_cast<u16>(-pitch));
                NuVecRotateY(&offset, &offset, yaw);
                NuVecAdd(&spline_point, &spline_point, &offset);
            }
        }
        object->apiobj.position = spline_point;
    } else {
        object->apiobj.position = *position;
        object->apiobj.movement_facing_angle = static_cast<u16>(angle);
        object->apiobj.facing_angle = static_cast<u16>(angle);
        object->apiobj.field_0x276 = static_cast<u16>(angle);
        if (group != NULL) {
            AddToAIGroup(group, &object->apiobj);
            offset.x = ((object->ai.group_column + 1) / 2) * group->x_spacing;
            if (object->ai.group_column & 1)
                offset.x = -offset.x;
            offset.z = -group->z_spacing * object->ai.group_member;
            offset.y = 0.0f;
            NuVecRotateY(&offset, &offset, angle);
            NuVecAdd(&object->apiobj.position, &object->apiobj.position, &offset);
        }
    }

    ResetPlayerMoves(object);
    plr_lastpos = object->apiobj.start_position = object->apiobj.collision_position = object->apiobj.initial_position =
        object->apiobj.position;
    object->apiobj.velocity = v000;

    GetTopBot(object);
    GameObjectDimensions(object);
    ResetRumble(&object->pad_gamepad->rumble_packet);
    ResetLights(&object->apiobj.position, &object->light_data, WORLD->rtl_set);

    object->field_0x661 = 0xff;
    object->sock_segment = -1;
    if (WORLD->sock_sys != NULL) {
        extern i32 complexsockposition_forcesock;
        if (Door_NextSock != -1 && WORLD->sock_sys->sock[Door_NextSock].valid != 0)
            complexsockposition_forcesock = Door_NextSock;
        ComplexSockPosition(WORLD->sock_sys, &object->apiobj.position, -1, -1, &object->sock_position);
        ComplexSockAngles(&object->sock_angles);
    }
    NuVecRotateYValZ(&object->facing_direction, 1.0f, object->apiobj.field_0x276);
    object->apiobj.model_draw_result = 1;
    object->use_model_origin = 0;
    object->apiobj.field_0x288 = 0;
    object->field_0xefe &= ~4u;

    if (has_no_spline) {
        InitSurfaceInfo(object);
        if (set_on_surface != 0) {
            SetObjOnSurface(object, 0);
        }
    }
    PortalGameObject(object, 1, 1, -1, WORLD->current_gscn);
    if (reset_restart != 0) {
        GAMECHARACTERDATA *config = (GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24;
        object->hitpoints = config->hitpoints;
        object->field_0xe38 = 4;
        object->current_hp = config->hitpoints;
        object->field_0xe37 = config->field_0xf5;
    }

    object->apiobj.field_0x1f4 &= ~0x100u;
    object->apiobj.field_0x1f8 &= static_cast<u16>(~4u);
    object->field_0x1004 = 1.0f;
    object->apiobj.field_0x287 = 0;
    object->field_0x7a5 = 0xff;
    object->apiobj.field_0x285 = 0;
    memset(&object->ai.path_info, 0, sizeof(object->ai.path_info));
    object->ai.field_0x124 = -1;
    object->ai.field_0x138 = 0xff;
    object->ai.field_0x139 = 0;
    if (has_no_spline) {
        if (path_info != NULL) {
            AISysCharacterSetPath(&object->ai, path_info->path);
            AISysCharacterSetPathCnx(&object->ai, &object->apiobj.position, path_info->connection,
                                     path_info->direction);
        }
        if (object->ai.path_info.connection == NULL) {
            AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff,
                                     static_cast<i8>(object->apiobj.field_0x27d));
        }
    }
    AIScriptProcessorInit(WORLD->ai_sys, &object->ai, &object->ai.script_process, NULL, script_name, NULL, 1, NULL,
                          NULL);
    object->apiobj.field_0x214 = 2000000.0f;
    if (object->ai.group != NULL) {
        AIGROUP *active_group = object->ai.group;
        u32 member = object->ai.group_member_index;
        active_group->member_is_alive |= 1ULL << member;
        AIROW *row = &active_group->rows[object->ai.group_row];
        u32 column = object->ai.group_member_index - object->ai.group_row * active_group->count_across;
        row->is_alive |= 1ULL << column;
        if (object->ai.group_column == 0) {
            row->pos = object->apiobj.position;
            row->path_info = object->ai.path_info;
            row->next_connection = NULL;
            row->y_rot = object->apiobj.field_0x276;
            row->is_turning = 0;
        }
    }
    PreResetCode(object);
    PostResetCode(object);
    GameObjectOrigin(object);
    object->ai_update_position = object->apiobj.last_safe_position = object->apiobj.position;
    if (static_cast<u32>(creature_set - 1) < 16) {
        object->ai.creature_set = static_cast<u8>(creature_set);
        ++aicreature_sets_alive[creature_set - 1];
    }
    return object;
}

void AISysGetPathPos2(AISYS_s *system, nuvec_s *position, AIPATHINFO_s *path_info, nuvec_s *path_position, AIPATH_s *,
                      i32) {
    if (position != NULL && system != NULL && path_info != NULL) {
        APIOBJECT object;
        AIPACKET packet;
        memset(&object, 0, sizeof(object));
        memset(&packet, 0, sizeof(packet));
        object.ai = &packet;
        object.position = *position;
        packet.terrain_origin = *position;
        AISysGetCharacterPathPos(system, &object, &packet, 0xff, 1);
        *path_info = packet.path_info;
        if (path_position != NULL) {
            *path_position = packet.last_path_position;
        }
    }
}

ADDGIZMOTYPE *AI_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "AIProcessor";
    addtype.prefix = "ai_";
    addtype.fns.unknown1 = 0;
    addtype.fns.get_max_gizmos_fn = AI_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = AI_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.draw_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = AI_GetGizmoName;
    addtype.fns.get_output_fn = AI_GetOutput;
    addtype.fns.get_output_name_fn = AI_GetOutputName;
    addtype.fns.get_num_outputs_fn = AI_GetNumOutputs;
    addtype.fns.activate_fn = AI_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.allocate_progress_data_fn = NULL;
    addtype.fns.clear_progress_fn = NULL;
    addtype.fns.store_progress_fn = NULL;
    addtype.fns.reset_fn = NULL;
    addtype.fns.reserve_buffer_space_fn = NULL;
    addtype.fns.load_fn = NULL;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;

    return &addtype;
}

u32 GameAI_TotalScore() {
    GameObject_s *object = Obj;
    u32 total = 0;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.flags_low & 1) == 0 || (object->apiobj.field_0x1f4 & 0x400) == 0 ||
            object->ai.field_0x134 == 0xff)
            continue;
        GAMECHARACTERDATA_s *config = static_cast<GAMECHARACTERDATA_s *>(object->apiobj.character_data->field11_0x24);
        if ((config->flags_090 & 0x40) == 0 && object->id != id_GONKDROID)
            total += static_cast<u16>(config->field_0xee);
    }
    return total;
}

