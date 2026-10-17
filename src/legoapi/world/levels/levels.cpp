// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/world/level.h"

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/audio/audio.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"

extern "C" char ConfigBuffer[0x10000];

void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process);

// These are extern (U) in the original level.cpp.o — defined here as stubs
// until the original defining file is decompiled.
LEVELDATA *levelconfig_ldata = NULL;

extern "C" {
    struct LEVELDATA_s *PODRACELEVELS[11]; // Arrival1-4, Intro, B, C, A, Outro1, Outro2, Status
}

static void Pictures_FixUp(WORLDINFO *world);
static i32 Pictures_NumLevels;

static void Credits_Init_Game(WORLDINFO *world);
static void Credits_Update_Game(WORLDINFO *);
static void Credits_Draw_Game(WORLDINFO *);

extern void NewGame(void);
extern void Door_Reset(void);
extern void BackDrop_ResetColours(void);
extern i16 id_DEFAULTCHARACTER[2];
extern i32 GetMenuID(void);

static NUVEC titlesstartpos;

static void Titles_Init(WORLDINFO *world);

static void Titles_Update(WORLDINFO *);

static void Titles_Draw(WORLDINFO *);

i32 Text_StripComments(char *text, char *dest, i32 param);

LEVELDATA *LDataList = NULL;
extern LEVELDATA * NEWGAME_LDATA;
extern LEVELDATA * LOADGAME_LDATA;


i32 LEVELCOUNT;

LEVELDATA *Levels_ConfigureList(char *file, VARIPTR *buf, VARIPTR *buf_end, i32 max_level_count, i32 *level_count_out,
                                LEVELSETDEFAULTSFN *set_defaults_fn);

void Level_SetDefaults(LEVELDATA *level);

LEVELDATA *Level_FindByName(char *name, i32 *idx_out);

void Level_Draw(WORLDINFO *world);

LEVELFIXUP LevFixUp;

void Levels_FixUp(LEVELFIXUP *fixup);

void Level_RegisterGameConfigKeywords(nufpcomjmp_s *beforeLoadKeywords, nufpcomjmp_s *afterLoadKeywords);

LEVELDATA *ANEWHOPELEVELS[4];
void Credits_Load(WORLDINFO_s *, variptr_u *, variptr_u *);

void FixUpLevels(LEVELFIXUP *fixup);

void Level_Update(WORLDINFO *world);

i32 LevelObject_GetReflection(i32 objId);

char *LevelObject_FindNameFromIndex(i32 index);

i32 LevelObject_FindIndexFromName(char *name);

i32 LevelObject_FindIndexFromName_RefOnly(char *name);

void GameAnimSys_ClearProgress(i32 idx);

void ClearLevelProgress(i32 index, WORLDINFO *world);

void GoToNewLevel(i32 levelIdx);

void Level_LoadConfigFile(WORLDINFO *world);

// ==== src/legoapi/world/world.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/surfaces.h"

#include <stdio.h>
#include <string.h>

#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/items/objects/grabber.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/NuDeviceSpecs.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edgra.h"

// Globals shared across world loading — defined here until moved to globals.cpp
extern TIMER LevelTimer;

extern void (*WorldInfo_InitMenuFn)(WORLDINFO *, i32 *, i32 *);
extern void (*WorldInfo_InitLastFn)(WORLDINFO *);
volatile i32 abort_load = 0;
char ConfigBuffer[0x10000];
i32 numtl_force_mipmode = 0;
i32 GAMEDEMO = 0;
NUGSCN *big_icon_scene = NULL;
NUGSCN *area_scene = NULL;
NUGSCN *things_scene = NULL;
extern void * things_scene_terrain;
LEVELDATA *PLATFORM_LDATA = NULL;
LEVELDATA *RETAKED_LDATA = NULL;
LEVELDATA *CREDITS_LDATA = NULL;
i32 Text_Language = 1;
nufpcomjmp_s LevelConfigKeywords_AfterLoad;

// --- Players globals ---
// NOTE: `WORLD` (WORLDINFO) field offsets in world.h are an approximation; the
// original binary reaches the level/area pointers through WORLDINFO[0x128]
// while world.h places them at 0x130/0x134.  Players_Init still links and runs
// with the world.h layout but its level/area guard bytes will differ until that
// WORLDINFO drift is corrected.

GameObject_s *Player[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
extern i32 PLAYERCOUNT;
extern i32 netclient;
extern i32 UsePlayerList;
extern i32 makeplayerlist_freeplay;
i16 PlayerList[8];
extern i32 PlayerID[2];
extern i16 Area_PlayerIDList[9];
extern APICHARACTERMODELLIST_s Area_StoryModelList[52];
extern PLAYERPROGRESS PlayerProgress[8];
extern i32 Hub_UsePlayerList;
extern i32 LevelChangesInArea;
i32 bonusmodearcade = 0;
extern i32 VehicleArea;
extern SOCKPOSITION OldPlrSPos[8];
extern BATARANG_s Batarang[8];
extern void * PlayerSuit[8];
extern u8 PlayerTorpedoCount[8];
extern COINPACKET CoinPacket[2];
extern u32 BackUpPlayers[2162];
extern GizForceLOSState_s GizForceLOSInfo[2];
extern i32 DEFAULT_PLAYERHITPOINTS;
extern u32 LEGOOBJ_DEFAULTLASTCOIN;

// --- World-module helpers (kept with the WorldInfo API) ---

void SetAreaPickupGravity(i32 area, i32 level);
void StoreSceneProgress(NUGSCN *gscn, SCENEPROGRESS_s *progress, i32 param);

void StoreLevelProgressFn(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress, i32 area_progress);

void SaveSceneObjectAnimTFactors(NUGSCN *gscn);
void CalculateWorldSize(WORLDINFO *world);

extern WORLDINFO WorldInfo[2];
WORLDINFO *WORLD = &WorldInfo[0];

extern f32 g_BgLoadDelayHackTimer;

extern volatile i32 waiting_for_level;
extern i32 level_already_loaded;
extern i32 next_level;

void WorldInfo_Load(WORLDINFO *world);

extern "C" {
    extern i16 id_DARTHVADER;
    extern i16 id_THEEMPEROR;
    extern i16 id_GRANDMOFFTARKIN;
    extern i16 id_IMPERIALOFFICER;
    extern i16 id_IMPERIALSHUTTLEPILOT;
}

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *, u32, u32, i32, i32);
static inline i32 World_AddFreePlayModel(i32 model) {
    if (model == -1 || FreePlayModelCount >= 48)
        return 0;
    if (FreePlayModelCount < 2)
        PlayerList[FreePlayModelCount] = static_cast<i16>(model);
    for (i32 i = 0; i < FreePlayModelCount; ++i) {
        if (FreePlayModelList[i].model_id == model)
            return 0;
    }
    FreePlayModelList[FreePlayModelCount].model_id = static_cast<i16>(model);
    FreePlayModelList[FreePlayModelCount].count = 1;
    ++FreePlayModelCount;
    FreePlayModelList[FreePlayModelCount].model_id = -1;
    return 1;
}

static inline __attribute__((always_inline)) bool World_FreePlayImperial(i32 model) {
    const i8 action = static_cast<i8>(GCDataList[model].uses_weapon_action);
    return action == 8 || action == 1 || model == id_DARTHVADER || model == id_THEEMPEROR ||
           model == id_GRANDMOFFTARKIN || model == id_IMPERIALOFFICER || model == id_IMPERIALSHUTTLEPILOT;
}

void MakeFreePlayModelList(i32 model1, i32 model2, i32 area, i32 level, i32 param5);

// ==== src/legoapi/world/area.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/render/core/screen.h"

#include <stdlib.h>

#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"

i32 Area_CharIDInCurrentList(i32 character_id);
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/world/world.h"

struct APICHARACTERMODELLIST_s;
struct CUSTOMISER;
struct MISSIONSYS_s;

extern "C++" volatile i32 abort_load;
extern NUGSCN *area_scene;
extern NUGSCN *vehicle_scene;
extern NUGSCN *big_icon_scene;
extern "C" i16 id_ANAKINJEDISCARRED;
extern "C" i16 id_HANINCARBONITE;
extern i16 Area_PlayerModelList[18];
extern APICHARACTERMODELLIST_s Area_MissionModelList[49];
extern APICHARACTERMODELLIST_s FreePlayModelList[52];
extern APICHARACTERMODELLIST_s Hub_ModelList[6];
extern i32 Area_PlayerModelCount;
extern CUSTOMISER *CharacterCustomiser;
extern "C" i32 makefreeplaymodellist;
extern i32 BonusArea;
extern AREADATA *ANEWHOPE_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern AREADATA *PODRACE_ADATA;
extern AREADATA *BONUS_GUNSHIP_ADATA;
extern AREADATA *NEGOTIATIONS_ADATA;
extern AREADATA *GUNGAN_ADATA;
extern AREADATA *RESCUE_ADATA;
extern AREADATA *RETAKE_ADATA;
extern AREADATA *MAUL_ADATA;
extern AREADATA *BOUNTYHUNTERPURSUIT_ADATA;
extern AREADATA *FACTORY_ADATA;
extern AREADATA *JEDI_ADATA;
extern AREADATA *GUNSHIP_ADATA;
extern AREADATA *DOOKU_ADATA;
extern AREADATA *DOGFIGHT_ADATA;
extern AREADATA *BATTLEOVERCORUSCANT_ADATA;
extern AREADATA *CRUISER_ADATA;
extern AREADATA *GRIEVOUS_ADATA;
extern AREADATA *KASHYYYK_ADATA;
extern AREADATA *TEMPLE_ADATA;
extern AREADATA *VADER_ADATA;
extern AREADATA *BLOCKADERUNNER_ADATA;
extern AREADATA *TATOOINE_ADATA;
extern AREADATA *MOSEISLEY_ADATA;
extern AREADATA *DEATHSTARRESCUE_ADATA;
extern AREADATA *DEATHSTARESCAPE_ADATA;
extern AREADATA *DEATHSTARBATTLE_ADATA;
extern AREADATA *HOTHESCAPE_ADATA;
extern AREADATA *ASTEROIDCHASE_ADATA;
extern AREADATA *DAGOBAH_ADATA;
extern AREADATA *CLOUDCITYTRAP_ADATA;
extern AREADATA *CLOUDCITYESCAPE_ADATA;
extern AREADATA *JABBASPALACE_ADATA;
extern AREADATA *SARLACCPIT_ADATA;
extern AREADATA *SPEEDERCHASE_ADATA;
extern AREADATA *ENDORBATTLE_ADATA;
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *EMPERORFIGHT_ADATA;
extern AREADATA *E1CHARACTER_ADATA;
extern AREADATA *E2CHARACTER_ADATA;
extern AREADATA *E3CHARACTER_ADATA;
extern AREADATA *E4CHARACTER_ADATA;
extern AREADATA *E5CHARACTER_ADATA;
extern AREADATA *E6CHARACTER_ADATA;
extern AREADATA *E1VEHICLE_ADATA;
extern AREADATA *E2VEHICLE_ADATA;
extern AREADATA *E3VEHICLE_ADATA;
extern AREADATA *E4VEHICLE_ADATA;
extern AREADATA *E5VEHICLE_ADATA;
extern AREADATA *E6VEHICLE_ADATA;
extern AREADATA *ANAKINSFLIGHT_ADATA;
extern AREADATA *LEGOCITY_ADATA;
extern AREADATA *NEWTOWN_ADATA;
extern AREADATA *JIMTEST_ADATA;
extern i32 CHARPAK;
extern i32 CharacterDataLoad;
extern i32 loadareadata_loadlevel;
extern i32 AreaDataLoaded;
extern i32 Level;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;
extern i32 last_area;
extern i32 BGLOAD;
extern i32 LOADEROFF;
extern i32 new_level_from_menu;
extern "C" i32 SuperStory;
extern i32 GAMEDEMO;
extern i32 textcrawlactive;
extern i32 readpads_always;
extern "C" i32 loadareacharacters_loadedlevel;
extern "C" i32 waiting_for_character;
extern "C" GAMEPAD_s GamePad[64];
extern "C" f32 MainRenderTime;
extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;
extern "C" OPTIONSSAVE *Game_OptionsSave;
extern FadeSystem FadeSys;
extern void *theGameThings;

extern void MakeFreePlayModelList(i32, i32, i32, i32, i32);
extern void Customiser_LoadAccessories(CUSTOMISER *, APICHARACTERMODELLIST_s *);
extern void Customiser_SetAnimsToLoad(CUSTOMISER *, i32);
extern void GameLoadCharacterModels(APICHARACTERMODELLIST_s *, i32, variptr_u *, variptr_u *, i32, i32);
extern void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *, variptr_u *, variptr_u);
extern void Area_Configure(i32, i32, EXTRAMODEL *, i16 *);
extern void SetBackgroundMusic(i32);
extern f32 GameSetMusicVolume(OPTIONSSAVE_s *);
extern void legoSetMusicVolume(f32);
extern void MusicClearAll(void);
extern void BackDrop_Update(f32);
extern void BackDrop_UpdateColours(i32);
extern void BackDrop_Draw(f32, i32);
extern i16 tTOUCHTOSTART;
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" f32 NuFrameEnd(void);
extern "C" void edGraEnableTerrainSwap(void);
extern "C" void edGraDisableTerrainSwap(void);
extern "C" i32 NuKeyboard_db(i32);
extern "C" i32 NuSound3LoadingSfx(void);
extern void Particles_LoadAreaPage(char *);
extern "C" {
    extern void SoundKillAll(void);
}

extern AREADATA * ADataList;
AREADATA *HUB_ADATA = NULL;
i32 loadareacharacters_no_backdrop_reset = 0;

static f32 LoadTime;
static f32 LOADWAITTIME;
static f32 LoadWait;

static AREAFIXUP AreaFixUp_LSW[] = {
    {"map", &HUB_ADATA},
    {"negotiations", &NEGOTIATIONS_ADATA},
    {"Gungan", &GUNGAN_ADATA},
    {"PalaceRescue", &RESCUE_ADATA},
    {"podsprint", &PODSPRINT_ADATA},
    {"RetakePalace", &RETAKE_ADATA},
    {"Maul", &MAUL_ADATA},
    {"pursuit", &BOUNTYHUNTERPURSUIT_ADATA},
    {"Kamino", &KAMINO_ADATA},
    {"Factory", &FACTORY_ADATA},
    {"Jedi", &JEDI_ADATA},
    {"gunship", &GUNSHIP_ADATA},
    {"Dooku", &DOOKU_ADATA},
    {"Dogfight", &DOGFIGHT_ADATA},
    {"Dogfight", &BATTLEOVERCORUSCANT_ADATA},
    {"Cruiser", &CRUISER_ADATA},
    {"Grievous", &GRIEVOUS_ADATA},
    {"Kashyyyk", &KASHYYYK_ADATA},
    {"Temple", &TEMPLE_ADATA},
    {"Vader", &VADER_ADATA},
    {"BlockadeRunner", &BLOCKADERUNNER_ADATA},
    {"tatooine", &TATOOINE_ADATA},
    {"moseisley", &MOSEISLEY_ADATA},
    {"DeathStarRescue", &DEATHSTARRESCUE_ADATA},
    {"DeathStarEscape", &DEATHSTARESCAPE_ADATA},
    {"DeathStarBattle", &DEATHSTARBATTLE_ADATA},
    {"HothBattle", &HOTHBATTLE_ADATA},
    {"HothEscape", &HOTHESCAPE_ADATA},
    {"AsteroidChase", &ASTEROIDCHASE_ADATA},
    {"Dagobah", &DAGOBAH_ADATA},
    {"CloudCityTrap", &CLOUDCITYTRAP_ADATA},
    {"CloudCityEscape", &CLOUDCITYESCAPE_ADATA},
    {"JabbasPalace", &JABBASPALACE_ADATA},
    {"SarlaccPit", &SARLACCPIT_ADATA},
    {"SpeederChase", &SPEEDERCHASE_ADATA},
    {"EndorBattle", &ENDORBATTLE_ADATA},
    {"DeathStar2Battle", &DEATHSTARBATTLE2_ADATA},
    {"emperorfight", &EMPERORFIGHT_ADATA},
    {"E1CharacterBonus", &E1CHARACTER_ADATA},
    {"E2CharacterBonus", &E2CHARACTER_ADATA},
    {"E3CharacterBonus", &E3CHARACTER_ADATA},
    {"E4CharacterBonus", &E4CHARACTER_ADATA},
    {"E5CharacterBonus", &E5CHARACTER_ADATA},
    {"E6CharacterBonus", &E6CHARACTER_ADATA},
    {"E1VehicleBonus", &E1VEHICLE_ADATA},
    {"E2VehicleBonus", &E2VEHICLE_ADATA},
    {"E3VehicleBonus", &E3VEHICLE_ADATA},
    {"E4VehicleBonus", &E4VEHICLE_ADATA},
    {"E5VehicleBonus", &E5VEHICLE_ADATA},
    {"E6VehicleBonus", &E6VEHICLE_ADATA},
    {"PodRace", &PODRACE_ADATA},
    {"AnakinsFlight", &ANAKINSFLIGHT_ADATA},
    {"Bonus_Gunship", &BONUS_GUNSHIP_ADATA},
    {"ANewHope", &ANEWHOPE_ADATA},
    {"Bonus2", &LEGOCITY_ADATA},
    {"Bonus", &NEWTOWN_ADATA},
    {"LostTemple", &LOSTTEMPLE_ADATA},
    {"senate", &SENATE_ADATA},
    {"utapau", &UTAPAU_ADATA},
    {"hoth", &HOTH_ADATA},
    {"nb_dagobah", &BONUSDAGOBAH_ADATA},
    {"nb_kamino", &BONUSKAMINO_ADATA},
    {"nb_kashyyyk", &BONUSKASHYYYK_ADATA},
    {"JimTest", &JIMTEST_ADATA},
    {"Vehicles", &VEHICLES_ADATA},
    {NULL, NULL},
};

i32 Area = -1;

extern i32 AREA_DEFAULTBONUSTIMETRIALTIME;
extern i32 AREA_DEFAULTCHALLENGETIME;

void FixUpAreas(void);

i32 AreaFromMiniKitID(i32 minikitId);

struct bgprocinfo_s;
static __used__ void LoadAreaData(bgprocinfo_s *);

void LoadAreaCharacters();

// ==== src/legoapi/world/boss.cpp: all remaining content ====
#include "decomp.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/episode.h"
#include "globals.h"

extern i32 LevFlag[4];

i32 KillBoss(i32 character_id, i32 flag_index, float delay);

void KillBossNewLevel(i32 a, i32 b, float c, i32 d);

i32 KillBossPlayCutScene(i32 a, i32 b, float c, char *name);

void KillBossCompleteLevel(i32 a, i32 b, float c);

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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern HUBMINIKITPIECES_s ** Char_MiniKit;

void MiniKits_Init(variptr_u *buffer, variptr_u *);

void CharacterMiniKits_Load(COLLECTION_s *collection, WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end);

extern f32 KITPOSX, PANEL_MINIKITY, PANEL_MINIKITSCALE;
void MiniKit_GameMsg_Update(GAMEMESSAGE_s *message);
void MiniKit_GameMsg_End(GAMEMESSAGE_s *message);

void CollectMinikit(nuvec_s *position, char *name, i32);

i32 AllMiniKitsDone(AREASAVE_s *save);

i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);

extern "C" void PlaySfx(char *, nuvec_s *);
void DrawMiniKitCount(f32, f32, i32, i32);

void CollectAllMiniKits(AREASAVE_s *save);

void EndChallenge(i32, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

void ResetMinikitCounter();

extern i16 id_SLAVE1, tALLMINIKITSBUILT;

void CharacterMiniKits_Dump(WORLDINFO_s *world);

void SetEffectVisibility(char *, i32);

void IncrementMinikitCounter(GameObject_s *);

// ==== src/legoapi/world/levelstreaming.cpp: declarations ====
#include "decomp.h"
#include "batman.h"
#include "legoapi/world/area.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nutime.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "globals.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numath/nufloat.h"

#include <string.h>

extern i32 Players_AveragePos(struct nuvec_s *, struct SOCKPOSITION_s *);

void LevelStreaming_Update(WORLDINFO_s *world);

void LevelStreaming_DoorOverride(WORLDINFO_s *world, LEVELDATA_s *level, f32 radius, f32 *distance);

// ==== src/legoapi/world/areas.cpp: declarations ====
#include "decomp.h"
#include "batman.h"
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legogame/game.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/numusic.h"

extern i32 openlevels;

void Areas_OpenAll(i32 mode);

AREADATA *Areas_ConfigureList(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd, i32 count, i32 *countDest);

void Areas_ConfigureResidents(VARIPTR *buffer, VARIPTR *);

AREADATA *Area_FindByName(char *name, i32 *indexDest);

void Areas_FixUp(AREAFIXUP *fixup);

struct LEVELDATA_s *Area_FindStatusLevel(AREADATA *area, i32 *indexDest);

LEVELDATA *Area_FindNextPlayLevel(i32 levelIdx);

void SuperCounters_Reset(i32 area_index);

void SuperCounters_FixUpGizmos(WORLDINFO_s *world);

SUPERCOUNTER *SuperCounters_FindPickup(WORLDINFO_s *world, GIZMO_s *gizmo, nuvec_s *position,
                                       SUPERCOUNTERPICKUP **pickup_dest);

void SuperCounter_ActivateGizmoPickup(GIZMO_s *gizmo, GIZMOPICKUP_s *gizmo_pickup);

SUPERCOUNTER *SuperCounter_FindFromNameAndLevel(char *name, WORLDINFO_s *world, SUPERCOUNTERPICKUP **pickup_dest);

i32 SuperCounter_AnyCollected(SUPERCOUNTER *counter, WORLDINFO_s *world);

void SuperCounters_ResetProcessed(WORLDINFO_s *world);

void Area_Configure(i32 area, i32 param, EXTRAMODEL *models, i16 *s);

void ClearUpAreaData();

void ClearAreaProgress(i32 a, i32 b);

void NewArea();

// ==== src/legoapi/world/leveldata.cpp: declarations ====

#include <string.h>
#include "legoapi/world/level.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/gizmos/fx/gizmopickups.h"

extern i32 LevFlag[4]; // shared per-level 16-byte state scratch

extern GameObject_s * CutDeadVehiclePlayer;

void ResetLevel(WORLDINFO_s *world, char *cutscene, i32);

void ClearLevData();

void *SetLevelHack(i32 size);

// ==== src/legoapi/characters/core/characters.cpp: declarations ====
#include "nu2api/nu3d/nuportal.h"
#include "decomp.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "legoapi/menus/core/text.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 rtlInitDynamic(VARIPTR *, VARIPTR, i32);
    void DebrisSetup(VARIPTR *, VARIPTR, char *, i32, i32, i32);
    void DebrisRegisterCutoffCameraVec(void *);
    void edgraSetup(VARIPTR *, VARIPTR, i32, i32, i32);
    void InitParts(i32, VARIPTR *, VARIPTR *);
    void ParticleReset(void);
    i32 NuFileExists(char *);
    i32 NuStrCpy(char *, const char *);
    i32 edppLoadPage(char *, i32, usize);
    void NuRndrShadowInit(u8 *);
    void NuTexAnimProgSysInit(void);
    void terrainpickupinit(char *, void **);

    extern i32 Grass_Available;
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern void *perm_debrissys;
}

struct AISYS_s;
void AIScriptLoadAll(char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS_s *sys);
void InitTexAnimScripts(char **names);
void BackDrop_Init(char *path, VARIPTR *buf, VARIPTR *buf_end);

i32 PARTPAGE_GENERAL = -1;

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.
char *TexAnimList_LSW[32] = {
    (char *)"arrow",          (char *)"blink_01",      (char *)"blink_02",    (char *)"blink_03",
    (char *)"coin",           (char *)"coin_pause",    (char *)"control",     (char *)"ds_esc_intro_1",
    (char *)"ds_esc_intro_2", (char *)"ds_1",          (char *)"helpmeobi",   (char *)"lightening1",
    (char *)"plasma",         (char *)"screen",        (char *)"Dso_screen",  (char *)"anakinbows",
    (char *)"anakinkilling",  (char *)"anakinspod",    (char *)"boost1",      (char *)"boost2",
    (char *)"boost3",         (char *)"boost4",        (char *)"gasganospod", (char *)"helpobi",
    (char *)"hologram1",      (char *)"hologramnoise", (char *)"play8",       (char *)"quidie",
    (char *)"sebulbaspod",    (char *)"sidious",       (char *)"crowd",       NULL,
};

void InitRipples(ripple_set_s **, VARIPTR *, VARIPTR *, i32);
void CreateFadeMaterials();
void CreateUsefulMaterials();
extern "C" ripple_set_s *ripples;
extern "C" NUMTL *ShadowMat;

void CharConfig_ConfigureAll(i32 permanent, nufpcomjmp_s *game_keywords);
void ExtraCharacterFixUpAfterConfig();

extern i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count);
extern i32 Collection_Got(i32 id);
extern f32 WaitingForCharacterTime;

extern "C" {
extern i32 hub_character_ready;
}

APICHARACTERMODELLIST_s PermModelList[] = {{-1, 0}};

#include "legoapi/characters/core/characters_types.h"

void LSW_SetIndy(i32);

i32 CharIDFromName(char *name);

CHARACTERDATA *CDataFromName(char *name);

i32 RedirectAnim(char *directory, ANIMREDIRECT *redirects, ANIMLIST_s *animation_list, char *path);

void CharScenes_LevelLoad(WORLDINFO *world);

extern i16 tUNKNOWN;
extern "C" {
    void PlaySfx(char *, nuvec_s *);
}

void PostAnimate_FETT(GameObject_s *object);

void LightGameObject(GameObject_s *, void *);
void InitSurfaceInfo(GameObject_s *);
i32 SetObjOnSurface(GameObject_s *, i32);
extern "C" NUVEC plr_lastpos;

void FinishWeirdoNames(i32 which);

GameObject_s *ActivateCharacter(char *name, nuvec_s *position, i32 angle);

extern void GameObjectOrigin(GameObject_s *obj);

// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif

i32 InitCreature(GameObject_s *obj, i32 id, i32 param);

void DestroySnakeBody(GameObject_s *);
void InitPlayerAI(GameObject_s *);
void GizForce_ResetLOS(GameObject_s *);
i32 StartSlide(GameObject_s *, i32);

i32 NewPlayerCharacter(GameObject_s *object, i32 id, i32 old_id, i32);

i32 GetDefaultIdle(GameObject_s *obj);

void UpdateCharacterIDs();

void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *list, variptr_u *buf, variptr_u buf_end);

void CharScenes_AreaDump();

void SetMoveAndAnimateFunctions(u32 model_flag_mask, u32 model_flag_value, u32 game_flag_mask, u32 game_flag_value,
                                i32 movement_type, void *move_function, void *animate_function, void *draw_function);

void DeactivateCharacter(char *name);

void LoadSingleCharacter(bgprocinfo_s *);

void UpdateCharacterLoad();

extern "C" VARIPTR characterbuffer_base;
extern i32 CHARACTERBUFFERSIZE;
extern i32 Area;

void ResetCharacterBuffer(i32 force_reset);

void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
extern f32 ICONSIZE;

void PostAnimate_ASTROMECH(GameObject_s *object);

extern "C" i32 dagobah_training;

NUMTL *lightningmat;
extern "C" NUMTL *DebMat[10];
extern "C" void NuWaterInit(void);
extern "C" void NuBridgeInit(void);

void LoadPerm1();

// ==== src/legogame/game.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/numusic/sfx.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/core/config/cheat.h"
#include "legogame/game.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/render/core/terrain_internal.h"

#include <string.h>

#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/numath/nuvec.h"

extern "C" {
    extern void (*APIObjResetShadowMapRenderingFn)(void);
    extern void (*APIObjEnableShadowMapRenderingFn)(void);
}
i32 GameAudio_GetSfxId(i32);
void ReCalculateCompletionPoints();

extern f32 minikittime;
void ReleaseTakeOver(GameObject_s *object, i32 immediate);

extern char *ASCII_UP;
#include "legoapi/audio/audio.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/area.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/cutscenes/cutscenes.h"
extern i32 CutScenePlayCount;
#include "legoapi/world/level.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/motion/gameanim.h"
extern i16 id_THEEMPEROR;
#include "legoapi/world/world.h"

extern "C" void PlaySfx(char *, NUVEC *);

extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
f32 GameSetSoundVolume(OPTIONSSAVE_s *);
f32 GameSetMusicVolume(OPTIONSSAVE_s *);
void GameAudio_Reset(void);
void GizSpinners_InitTerrain(WORLDINFO_s *world);
void SecurityDoors_InitTerrain(WORLDINFO_s *world);
void Levers_InitTerrain(WORLDINFO_s *world);
void GizPanel_InitTerrain(WORLDINFO_s *world);
void GizmoBlowupsFinalSetup(WORLDINFO_s *world);
void InitClimbObjectSys(WORLDINFO_s *world);
void GizmoPushBlockInitAndReset(WORLDINFO_s *world, void *progress);
extern NUFPCOMJMP LevelConfigKeywords_AfterLoad;
extern i32 (*GamePads_IgnoreInputFn)(void);

extern i32 drawautosaveicon;

extern SUPEROPTIONS_s SuperOptions;
void GameAudio_Init(GAMEAUDIO *);

extern "C" i32 DoubleScore;
i32 Players_AveragePos(nuvec_s *position, SOCKPOSITION_s *socket_position);

// Retail data @0x617200: common animations shared by the old and new characters.

void InitGameAfterConfig(void);

void InitGameBeforeConfig(void);

void CompleteLevel(WORLDINFO *world);

void StoreLevelProgress(WORLDINFO *world);

void NewAreaMusicChanges(void);

void Game_WorldInfo_InitLast(WORLDINFO *world);

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

i32 Suit_GetIndex(SUIT_s *);
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
void PartCollide_3D(PART_s *);
extern "C" f32 sabrerubwait;
extern i32 lightning_type;
extern "C" i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;
extern i32 TERRAINMASK_NONWEAPON, TERRAINMASK_NONDROID;

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
i32 TightRope_SnapTo(GameObject_s *object, NUVEC *position);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
void GameAISysSetGame();
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world);
void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index);

extern i32 LEGO_AIPATHCNX_BLOCKAGE;

extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

extern GAMEFOG_STATE GameFog;

#include "legoapi/items/objects/gameobjects_types.h"

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

extern TERRSET *CurTerr;

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

extern "C" f32 MainRenderTargetTime;
extern void BackDrop_UpdateColours(i32 instant);
extern i32 Paused;
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

extern i32 pause_rndr_on;
extern "C" TIMER BonusTimer;
extern "C" i32 screendump;

void GameTiming(WORLDINFO_s *world, float *game_time);

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);
extern "C" i32 RAYCASTCALLS;

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask);

extern i32 active_neutral_count;
extern "C" i32 party_cant_be_under_cover;
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
extern "C" i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

f32 GameGetMusicVolume(OPTIONSSAVE_s *options);

f32 GameSetMusicVolume(OPTIONSSAVE_s *options);

void GameAISysStartFrame(AISYS_s *system);

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours);

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object);

i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

void GameLoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append, VARIPTR *buf, VARIPTR *buf_end, i32 area_models,
                             i32 area);

void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *);

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

extern CUSTOMPIECE *Customiser_FindPieceByName(CUSTOMISER *, char *, i32 *, i32 *);

i32 NoLayerKill(GameObject_s *object);

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only);

void BlowUpSnakeBody(GameObject_s *);
void DieRumble(GameObject_s *);
extern i32 speeder_hitpoints_lost;

extern char *ASCII_DOWN;

void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32);

extern i32 nethost;
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

extern "C" i32 adaptivedifficulty[3];
extern f32 lightning_sizew[2];
extern f32 lightning_sizel[2];
extern f32 lightning_sizewab[2];
extern f32 lightning_endw[2];
extern u32 lightning_col[2];
extern i16 id_ANAKINSNEWPOD, id_ANAKINSNEWPODGREEN, id_SEBULBASPOD;
extern "C" f32 tieonsfxwait, tieoffsfxwait;
void UpdateRumble(RUMBLEPACKET *);
void Player_ToggleCharacter(GameObject_s *, i32, i32);

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set);

GameObject_s *GetNamedGameObject(AISYS_s *system, char *name);

void RegisterTakeOverObject(GameObject_s *object);

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance);

void GameObjectToCameraDistances();

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

i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

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
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

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
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state

// Graphics loading flags

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

void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawPauseFade();

void DrawRippleSet(ripple_set_s *set);

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen);

void DrawExplosions();

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

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

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();

extern i32 dco_locatorposonly;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

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

void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern "C" i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);

void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);

void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

// ==== src/legoapi/gizmo/gizmos/gizmos_gizmopickups.cpp: declarations ====
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

// ==== src/legoapi/menus/core/gamehint.cpp: declarations ====
#include "decomp.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/legoapi_types.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/world/mission.h"

extern LSW_HINTCONDITIONS_s LSW_HintConditions;
extern HINTSYS_s hintsys;
extern f32 ICONXPOS_TOUCHYFEELY;
extern f32 ICONYPOS_TOUCHYFEELY;
f32 SeekLinearF(f32, f32, f32);

extern "C" i32 NewMode, Paused, editor_active, CutSceneWaiting, PANELOFF;
i32 GetMenuID();
void MechHintUIButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);

#include "legoapi/menus/core/gamehint_types.h"
DECOMP_ASSERT(sizeof(MechHintUIButton) == 0xa8, "MechHintUIButton size");
DECOMP_ASSERT(offsetof(MechHintUIButton, pending_hint) == 0x7c, "Pending touch hint offset");
DECOMP_ASSERT(offsetof(MechHintUIButton, display_elapsed) == 0x80, "Touch hint display timer offset");

void MechHintUIButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &);

void Hint_Process(float elapsed);

HINT_s *Hint_FindHint(i32 hint_id);

void Hint_SaveBits(i32 hint_id, i32 complete);

i32 Hint_CurrentId();

i32 Hint_isComplete(HINT_s *hint);

i32 Hint_isComplete(i32 hint_id);

void Hint_SetComplete(HINT_s *hint);

void Hint_SetComplete(i32 hint_id);

i32 Hint_isAvailable(i32 hint_id);

void Hint_CancelCurrent();

void Hint_Draw(i32 viewport);

// Static game message and hint helpers. Stubbed to satisfy the symbol baseline.

// ==== src/legoapi/items/base/game_object.cpp: declarations ====
#include "legoapi/world/world_shared.h"

#include "decomp.h"
#include "globals.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>
#include <new>

// Forward declarations for local (static) game-object helper stubs.
struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;
struct BOLT_s;
struct debinftype;

extern "C" {
    extern i16 id_MOSEISLEYCITIZEN;
    extern i16 id_CANTINAALIEN;
    extern i16 id_CLOUDCITYCITIZEN;
    extern i16 id_GEONOSIAN;
    extern i16 id_BOB;
    extern i16 id_SPEEDERBIKE;
    extern i16 id_SPEEDERBIKESNOW;
    extern i16 id_STAP;
    extern i16 id_STAP2;
    extern i16 id_ATST;
    extern i16 id_BIGGUN;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_ZAMSSPEEDER;
}

void ClearGameObjects(APIOBJECTSYS_s *api_object_sys);

GameObject_s *AddGameObject(i32 id);

// ==== src/legoapi/characters/core/character.cpp: declarations ====
#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"

i32 CanPullLevers(i32 character_id);

#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/area.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numusic/sfx.h"

#include <string.h>
struct numtx_s;
struct EXTRAMODEL;

// Forward declarations for local (static) character/gameplay helper stubs.
struct EDCREATURE_s;
struct APIOBJECT_s;

extern "C" {
extern i16 id_WEIRDO1;
extern i16 id_WEIRDO2;
extern i16 id_BATMAN;
extern i16 id_ROBIN;
extern i16 id_WHIP;
extern i16 id_QUIGONJINN;
extern i16 id_MACEWINDU;
extern i16 id_IMPERIALGUARD;
extern i16 id_BODYGUARD;
extern i16 id_DARTHMAUL;
extern i16 id_BATTLEDROID;
extern i16 id_BATTLEDROIDGEONOSIAN;
extern i16 id_BATTLEDROIDSECURITY;
extern i16 id_PROBEDROID;
extern i16 id_WOMPRAT;
extern i16 id_HANINCARBONITE;
extern i16 id_YODA;
extern i16 id_C3PO;
extern i16 id_TC14;
extern i16 id_YODAGHOST;
extern i16 id_MOSEISLEYCITIZEN;
extern i16 id_CANTINAALIEN;
extern i16 id_CLOUDCITYCITIZEN;
extern i16 id_GEONOSIAN;
extern i16 id_BOB;
extern i16 id_CHEWBACCA;
extern i16 id_ATST;
extern i16 id_ATST_LOWRES;
extern i16 id_BARMAN;
extern i16 id_DROIDEKA;
extern i16 id_SUPERBATTLEDROID;
extern i16 id_BOBAFETT;
extern i16 id_TRAININGREMOTE;
extern i16 id_ATAT;
extern i16 id_SERVICECAR;
extern i16 id_DRAGBOMB;
extern i16 id_CLONEWALKER;
extern i16 id_WICKET;
extern i16 id_EWOK;
extern i16 id_CATAPULT;
extern i16 id_BANTHA;
extern i16 id_DEWBACK;
extern i16 id_TAUNTAUN;
extern i16 id_SPEEDERBIKE;
extern i16 id_BIGGUN;
extern i16 id_4LOM;
extern i16 id_JABBA;
extern i16 id_SLAVE1;
extern i16 id_YWING;
extern i16 id_TIEBOMBER;
extern i16 id_GRIEVOUS;
extern i16 id_TIEFIGHTERDARTH;
extern i16 id_TIEFIGHTER;
extern i16 id_GRABCONTROL;
extern i16 id_GRABR2CONTROL;
extern i16 id_GRABMACHINE;
extern i16 id_GRABMAGNET;
extern i16 id_ROBOTBASE;
extern i16 id_LUKESKYWALKERDAGOBAH;
extern i16 id_SNOWMOB;
extern i16 id_SNOWTROOPER;
extern i16 id_DEATHSTARTROOPER;
extern i16 id_DARTHVADER;
extern i16 id_THEEMPEROR;
extern i16 id_MILLENNIUMFALCON;
extern i16 id_SPEEDERBIKESNOW;
extern i16 id_KAADU;
extern i16 id_GUNGAN;
extern i16 id_FALUMPASET;
extern i16 id_STAP2;
extern i16 id_JARJAR;
extern i16 id_PADMECLAWED;
extern i16 id_ANAKINPADAWAN;
extern i16 id_OBIWANKENOBIJEDIMASTER;
extern i16 id_SHAAKTI;
extern i16 id_LUMINARA;
extern i16 id_JANGOFETT;
extern i16 id_JEDISTARFIGHTERYELLOWEP3;
extern i16 id_JEDISTARFIGHTERREDEP3;
extern i16 id_RANCOR;
extern i16 id_ANAKINJEDISCARRED;
extern i16 id_REPUBLICGUNSHIP;
extern i16 id_REPUBLICGUNSHIP_GREEN;
extern i16 id_NEW_REPUBLIC_GUNSHIP;
extern i16 id_NEW_REPUBLIC_GUNSHIP_GREEN;
extern i16 id_COUNTDOOKU;
extern i16 id_XWING;
extern i16 id_ROYALGUARD;
extern i16 id_SNOWSPEEDER;
extern i16 id_TIEINTERCEPTOR;
extern i16 id_IMPERIALSHUTTLE;
extern i16 id_UGNAUGHT;
extern i16 id_SENTRYDROID;
extern i16 id_KAMINOANDROID;
extern i16 id_IMPERIALSHUTTLEPILOT;
extern i16 id_IMPERIALOFFICER;
extern i16 id_GRANDMOFFTARKIN;
extern i16 id_GONKDROID;
extern i16 id_JAWA;
extern i16 id_MOONCAR;
extern i16 id_MAPCAR;
extern i16 id_WOOKIEFLYER;
extern i16 id_IMPERIALSPY;
extern i16 id_GREEDO;
extern i16 id_BOSSK;
extern i16 id_CANTINABAND;
extern i16 id_TOWNCAR;
extern i16 id_TRACTOR;
extern i16 id_FIRETRUCK;
extern i16 id_LIFEBOAT;
extern i16 id_CLONEARC;
extern i16 id_NABOOSTARFIGHTER;
extern i16 id_NABOOSTARFIGHTERLIME;
extern i16 id_FLASHSPEEDER;
extern i16 id_MINIXWING;
extern i16 id_MINIYWING;
extern i16 id_MINITIEINTERCEPTOR;
extern i16 id_MINITIEBOMBER;
extern i16 id_MINIATAT;
extern i16 id_MINISTARDESTROYER;
extern i16 id_MINIROYALSTARSHIP;
extern i16 id_MINIIMPERIALSHUTTLE;
extern i16 id_MINIMILLENNIUMFALCON;
extern i16 id_MINIDROIDEKA;
extern i16 id_MINITIEFIGHTER;
extern i16 id_MINITIEADVANCED;
extern i16 id_MINISITHINFILTRATOR;
extern i16 id_MINISOLARSAILOR;
extern i16 id_ANAKINSPOD;
extern i16 id_SEBULBASPOD;
extern i16 id_ANAKINSNEWPOD;
extern i16 id_ANAKINSNEWPODGREEN;
extern i16 id_ANAKINSSPEEDER;
extern i16 id_ANAKINSSPEEDER_GREEN;
extern i16 id_ZAMSSPEEDER;
extern i16 id_VULTUREDROID;
extern i16 id_DROIDTRIFIGHTER;

}

extern i32 CHARCOUNT;
extern CHARACTERDATA * CDataList;
extern GAMECHARACTERDATA * GCDataList;
// Original 0x120-byte data record @0x00666960. FixUpCharacters copies this
// canonical prefix into every character before character-specific overrides.
// These values cover the fully typed common movement and perception fields.
extern GAMECHARACTERDATA GCDATA_DEFAULT;

extern i32 g_loadingCharacterInHub;

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

i32 Collection_GetIDList(COLLECTION_s *collection, u32 model_flag_mask, u32 required_model_flags, i16 *ids,
                         i32 *first_id, i32 *second_id, i32 unused);

void Collection_CreateMaster(char *file, i16 *idlist, COLLECTION_s *collection, i32 param4, float param5);

void Collection_CreateCustom(char *name, i16 *id_list, COLLECTION_s *collection, u32 required_model_flags,
                             u32 excluded_model_flags, u32 required_game_flags, i32 require_buyable, i32 columns,
                             VARIPTR *buffer, VARIPTR *, i32 use_all_characters, f32 scale);

COLLECTID *CollectIDUnlocked(i32 id);

i32 Collection_GotAnyOfType(i32 type, u32 flags);

void CollectAllCharacters(i32 only_story);

u32 GizForce_TotalScore(void *);
u32 GizObstacles_TotalScore(void *);
u32 GizTurrets_TotalScore(void *);

u32 TotalLevelCoinTally(WORLDINFO_s *world, u32 *pickups, u32 *blowups, u32 *buildits, u32 *forces, u32 *obstacles,
                        u32 *turrets, u32 *characters);

COLLECTION_s *GetFreePlayCollection(i32 area);

extern i32 SHOPHINTCOUNT;

// ==== src/legoapi/cutscenes/cutscene.cpp: declarations ====
#include "decomp.h"
#include "globals.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/cutscenes/cutscene_defrag.h"
#include "nu2api/nucore/nugcutscene.h"
#include "legoapi/world/world_shared.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "legoapi/characters/core/character.h"
#include "legoapi/render/fx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
struct instNUGCUTCHAR_s;
struct NUGCUTCHAR_s;
struct NUGCUTRIGID_s;
struct instNUGCUTRIGID_s;

extern i32 CUTCOUNT;
extern CutSceneListEntry * CutList;
extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);
extern i32 CUTCAM;

void *CutScenes_Load(char *config, NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end, i32 param2,
                     i32 param3, WORLDINFO *world);

// --- Extern "C" block: functions with confirmed C linkage in original libTTapp.so ---
extern "C" void *NuAnimData2FixPtrs(void *, isize, isize, i32);
extern "C" void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *, i32, f32, NUMTX *);

#include "legoapi/cutscenes/cutscene_types.h"
DECOMP_ASSERT(sizeof(CutSceneTriggerNameEntry) == 0x34, "cutscene trigger name entry stride");
DECOMP_ASSERT(offsetof(CutSceneTriggerNameTable, entries) == 0x0c, "cutscene trigger name table entries offset");

// Exported (default visibility) in the original library.

// The relocation tables are patched as plain words, so every fixup aliases
// cutscene->string_delta and re-reads it.
typedef isize __attribute__((may_alias)) NuGCutWord;
#define NUGCUT_WORD(field) (*reinterpret_cast<NuGCutWord *>(&(field)))
#define NUGCUT_RELOCATE(field, delta) \
    (NUGCUT_WORD(field) = NUGCUT_WORD(field) != 0 ? NUGCUT_WORD(field) + (delta) : 0)

// Retail keeps NuGCutSceneFixPtrs only as two constant-propagated clones:
// .44 relocates animations by anim_delta, .43 is the anim_delta == 0 case.
#if defined(__i386__)
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone, regparm(n)))
#else
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone))
#endif

extern "C" void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *locator_functions);

void NuGCutSceneSysInitVfx(NUGCUTLOOKUPLOCATORVFXFN lookup, NUGCUTTRIGGERLOCATORVFXFN trigger,
                           NUGCUTRELEASELOCATORVFXFN release, NUGCUTUPDATELOCATORVFXFN update);

void NuGCutSceneRemapFocusIdToLocaterNum(NUGCUTSCENE_s *cutscene, VARIPTR *buffer);

extern "C" {

extern NUGCUTUPDATELOCATORVFXFN UpdateLocatorVfxFn;
extern NUGCUTRELEASELOCATORVFXFN ReleaseLocatorVfxFn;
extern NUGCUTTRIGGERLOCATORVFXFN TriggerLocatorVfxFn;
extern NUGCUTLOOKUPLOCATORVFXFN LookupLocatorVfxFn;
NUGCUTSCENE_s *NuGCutSceneLoad(char *name, VARIPTR *buf, VARIPTR *buf_end, i32 flags);

NUGCUTSCENE_s *NuGCutSceneLoadAddr(NUGCUTSCENE_s *cutscene, i32 loaded_size, VARIPTR *buffer);

void NuGCutSceneDestroy(NUGCUTSCENE_s *cutscene);

void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene, NUGSCN *scene, i32 flags, i32 area);
void NuGCutSceneFixUpExtra(NUGCUTSCENE_s *cutscene, NUGSCN *area);
instNUGCUTSCENE_s *instNuGCutSceneCreate(NUGCUTSCENE_s *cutscene, NUGSCN *scene, void *extra, char *name,
                                             VARIPTR *buf, i32 flags);

void PetesHackOfDeath(void);
} // extern "C"

struct instNUGCUTSCENE_s;
struct NUGCUTLOCATORSYS_s;
struct instNUGCUTLOCATOR_s;
struct NUGCUTLOCATOR_s;
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
i32 Draw3DObjectMtx(WORLDINFO_s *, i32, numtx_s *);

extern "C" void DebFreeInstantly(i32 *handle);

// Retail only keeps the IPA-SRA clone, which drops the unused frame argument.
#if defined(__i386__)
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone))
#endif

extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

static __used__ void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *instance, float frame);

// ==== src/legoapi/ai/core/gameai.cpp: declarations ====
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

#include "legoapi/ai/core/gameai_types.h"

extern void InitSurfaceInfo(GameObject_s *);
extern i32 SetObjOnSurface(GameObject_s *, i32);

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

extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;

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

#include "legoapi/items/objects/gameobjects_types.h"

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

extern "C" {

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

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

extern i16 id_ANAKINJEDISCARRED;

extern "C" {
extern ed_module_s edTimingDesc;
}

// ==== src/legoapi/gizmos/trigger/ai.cpp: all remaining content ====
#include "legoapi/gizmos/trigger/ai.h"

#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"

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
    void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
    void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);
    void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 route, i32 surface_flags);
    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation);
    void SetAnimTimeRandom(CHARACTERMODEL_s *model, ANIMPACKET_s *packet);
}

#include "legoapi/ai/game/creature_types.h"

void NewRumbleAllPlayers(f32, f32, i32, i32);
i32 ObjHitObj_Flags(GameObject_s *);

i32 SpawnMeleeCreatureType(i32 type);

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
extern "C" {
    void LocaledbitsDrawCircleXY(NUVEC *, f32, u32, i32, i32);
    void LocaledbitsDrawSolidCircleXY(NUVEC *, f32, f32, f32, u32, i32, i32);
}

extern void (*AIPathCnxHelperSysInitFn)(WORLDINFO_s *);

void AIPathCnxControlSysUpdate(AIPATHCNXCONTROLSYS_s *system);

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
struct minitrooperteam_s;

void *GameBufferAlloc(VARIPTR *, VARIPTR *, i32);

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

i32 objInNetWaitContext(GameObject_s *object, i32 context);

#include "legoapi/actions/character/simpletons_types.h"

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

#include "legoapi/ai/core/ai_pathgeometry.h"

extern "C" i32 WithinConnection(AISYS *system, NUVEC *position, AIPATH *path, AIPATHCNX *connection, i32 checks,
                                AIPATHCNX *previous_connection, i32 route, i32 ground, AIPATHINFO *path_info,
                                f32 radius, i32 update_once);

// IPA-SRA hands this clone the address of system->path_sys rather than the system.

extern "C" void AISysFindRoute(AIPACKET *packet);

extern "C" void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);

extern "C" void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);

extern "C" f32 AIPathNodeDistanceToPathNode(AIPATH *path, i32 start_node, i32 destination_node, i32 route_index,
                                            u32 excluded_route_mask);

extern "C" i32 AISysGetCharacterWaypoint(const AIPACKET *packet, NUVEC *position);

bool AISysNodeCanReachThisJumpConnection(GameObject_s &, AIPATH_s &path, unsigned char node_index,
                                         AIPATHCNX_s &jump_connection, i32 jump_direction);

bool AISysCharacterCanReachThisJumpConnection(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &jump_connection,
                                              i32 jump_direction);

u32 DoSomeChecks(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &connection, i32 direction);

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

extern "C" void *perm_debrissys;

void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay);

void AddGameMsgCount(NUVEC *position, i32 count, i32 total, u8 red, u8 green, u8 blue, f32 duration);

extern "C" void ComplexSockAngles(SOCKROT *angles);

extern void Hint_CancelCurrent();

extern void ReleaseTakeOver(GameObject_s *object, i32 immediate);

extern void TakeOverGameObject(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera, i32 immediate);

extern void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, f32 distance, i32 looping);

extern void KillParts(GameObject_s *object, i32 part, i32 joint, i32 visible, f32 velocity, i32 flags, u16 *part_ids);

extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);

extern "C" ADDPART_s Default_ADDPART;

extern "C" PART_s *AddPart(ADDPART_s *);

void MakeThrowVector(NUVEC *, NUVEC *, NUVEC *, NUVEC *, f32, f32);

// Game-specific AI actions and conditions (registered via
// RegisterAIScriptActions / RegisterAIScriptConditions). These are stubbed to
// satisfy the symbol baseline; the action/condition logic itself is not
// decompiled. Each stub matches the mangled symbol of the original binary.

void LevelScriptReStoreProgress(WORLDINFO_s *, LEVELSCRIPTPROCESS_s *);

#include "legoapi/props/objects/techno.h"

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

// Retail inlines the system test into its callers; this is the remaining partial clone.

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

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
extern char * AiLevelPathName;
extern AISCRIPTPROCESS * pSetStateDebugee;
extern SCRIPTPROCESSFIRSTTIMEACTION * ScriptProcessFirstTimeActionFn;
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

extern "C" i32 AISysSetLevelPath(AISYS *system, char *path_name);

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

void AIScriptForceParamReEval(AISCRIPTPROCESS *processor);

char *AIScriptNameFromIx(AISYS *system, i32 index);

} // extern "C"

#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, without the unused object and elapsed time.

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

void AISysProcess(AISYS *system, APIOBJECT *player_1, APIOBJECT *player_2);

void AISysProcessCharacter(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed,
                               i32 use_three_dimensions, i32 process_ai);

i32 AISysSetLevelPath(AISYS *system, char *path_name);

void AISysUpdateCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed);

extern AIEDITOR_RENDER_STATE * aieditor;
    void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);

void AiSysOnlyUsePakFile(i32 enabled);

void AiSysSetStateDebugee(AISCRIPTPROCESS *processor);

void AiSysUsePackFile(i32 enabled);

void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out);

void DestroyAIGroup(AIGROUP *group);

void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result);

void FollowAPIObject(APIOBJECT *object, APIOBJECT *target, i32 flags, f32 movement_parameter);

extern AIEDITORPATHNODECALLBACK * AIPathNodeDeletedFn;

void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);

void InitFn_GameParamToFloat(GAMEPARAMTOFLOAT *function);

void InitFn_ScriptProcessFirstTimeAction(SCRIPTPROCESSFIRSTTIMEACTION *function);

void LEGO_AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                         f32 delta_time);

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

// Item colours shared by the AI editors.

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
    void LocaledbitsDrawCircleXY(NUVEC *, f32, u32, i32, i32);
    void LocaledbitsDrawSolidCircleXY(NUVEC *, f32, f32, f32, u32, i32, i32);
extern char *(*EdGetCnxFlagNames)(u32);
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

struct EDAISHAREDPATHNODE_s;
struct AIPATH_s;
struct eduimenu_s;
struct eduiitem_s;

DECOMP_ASSERT(offsetof(EdUiNameInputItem, name) == 0x4c, "editor name input offset");
DECOMP_ASSERT(offsetof(EdUiNameInputItem, max_name_length) == 0x15a, "editor name input limit offset");

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

void pathEditorSaveData(AIPATHSYS_s *system);

void pathEditor_OnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *result, EDAIPATH_s *path, f32 tolerance);
void pathEditor_QuickOnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *previous, EDAIPATHCHECK_s *result);

} // extern "C"

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) __asm__(
    "_ZL22routeEditor_AddToRouteP14EDAIPATHNODE_sS0_.part.7");

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other);

static __used__ void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *shared);

eduimenu_s *pathEditor_Process(nupad_s *pad);

// Retail keeps an unreferenced partial-inlining clone of pathEditorSetCamPos:
// it moves the editor camera to a position at the cursor's height.
#if defined(__i386__)
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone, regparm(1)))
#else
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

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

extern aieditor_settings_s aieditorsettings;
extern "C" {
    extern void *ed_fnt;
}

#if defined(__i386__)
#define EDANTINODE_REGPARM1 __attribute__((regparm(1)))
#else
#define EDANTINODE_REGPARM1
#endif

extern "C" {

void AISysSetPathCylinderCheck(i32 enabled);

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

extern "C" void (*ClearAICreaturesFn)();

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM2 __attribute__((regparm(2)))
#else
#define CREATURE_EDITOR_REGPARM2
#endif

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
    struct EDCREATURE_s;
    extern void (*ClearAICreaturesFn)();
}

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

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before);

#if defined(__i386__)
static __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2))) unsigned int
#else
static __attribute__((noinline, optimize("no-tree-vectorize"))) unsigned int
#endif
AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator) __asm__(
    "_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.constprop.131");

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

#include "legoapi/items/objects/gameobjects_types.h"

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

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

extern "C" {
extern ed_module_s edTimingDesc;
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

f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);

i32 Action_CameraCut(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                     i32 first_time, f32);

i32 Action_EndCameraCut(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params, i32 param_count,
                        i32 first_time, float);

i32 Action_PlayCutScene(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count, i32 first_time,
                        float);

i32 Action_ReleaseLocator(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, float);

struct GIZSPINNER_s;
void ClearSpecialMove(GameObject_s *);
extern i32 spinner_gizmotype_id;
extern u32 GAMEPAD_SPECIAL, GAMEPAD_JUMP, GAMEPAD_TOGGLERIGHT;
extern f32 ai_moveradius;

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
i32 qrand();
extern "C" void KillPart(PART_s *, i32);

extern BLADE_s BladeTab[4];

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
    AIANTINODE_s *AIAntinodeCreateSingleFrame(NUVEC *, f32);
}
i32 ViewCamGetMode();
NUVEC *ViewCamGetTgt();
void GameAntinode_UnregisterAntiNode(GAMEANTINODESYS_s *, GAMEANTINODE_s *);

void *GameAntnode_CreateSys(WORLDINFO_s *world, variptr_u *buf, variptr_u *buf_end, i32 count);

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

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

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

// Camera zoom state

// Graphics loading flags

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

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

// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.

extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}

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

extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

// ==== src/legoapi/actions/character/starwars.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"

struct AIPACKET_s;
struct AIGROUP_s;
struct nufpar_s;

extern PREPARINGSPECIALMOVEFN PreparingForSpecialMoveFn;

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

extern "C" {
    extern void *ed_fnt;
}

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *models, u32 model_flags, u32 game_flags, i32 require_hats,
                         i32 reject_flag_40);

#include "editor/edtoolsall_types.h"

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
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern NUMTL *DebMat[10];
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

void DebrisFreeOldestDmaDebTypeTable();

extern "C" i32 edppLoadPage(char *path, i32 flag, usize scene);
// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain);
extern "C" void TerrSetPlatScanDist(f32 dist);

extern "C" void TerrainPlatformNewUpdate(void);
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page);

void Particles_Load(WORLDINFO *world, char **debris_name, i32 count, i32 flags);

extern "C" {

void Debris(i32 update_panel_time);

} // extern "C"

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

void DebrisRegisterCutoffCameraVec(void *position);

extern i32 debris_render_group;

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

i32 FindPlatInst(i32 instance);

i32 NewPlatInst(void *object, i32 instance);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstRotate(i32 index, i32 rotate);

void PlatOnOff(i32 index, i32 enabled);

void SortDebrisRenderStack(void);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

TERRSET *TerrainGetCur(void);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

void UpdateDebrisRenderStackPriority(void);

} // extern "C"

// ==== functions in original address order ====

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

void DestroyAIGroup(AIGROUP *group);

void AIScriptSetLevelPath(char *path);

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

void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);

void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function);

void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function);

void AISysSetPathCylinderCheck(i32 enabled);

void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result);

}

extern "C" {

extern "C" void aieditor_ClearAllPathCnxTypes(void);

extern "C" void aieditor_RegisterPathCnxType(const char *name, u32 connection_flag, void *context, u32 flags);

extern "C" void aieditor_RegisterDefaultPathCnxTypes(void);

}

extern "C" {

void pathEditor_QuickOnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *previous, EDAIPATHCHECK_s *result);

void pathEditor_OnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *result, EDAIPATH_s *path, f32 tolerance);

}

extern "C" {
AIPATHSYS_s *pathEditorCreateData(VARIPTR *cursor, VARIPTR *end, void **scratch_cursor, void **scratch_end);

void pathEditorSaveData(AIPATHSYS_s *system);

}

// ==== functions in original address order ====

// ==== src/legoapi/items/objects/objectsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nu3d/nuspecial.h"
#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufile.h"

#include <string.h>

void NewRumble(nupad_s *, f32, i32);

void Buck_Start(GameObject_s *object, GameObject_s *rider);

extern i16 id_SNOWMOB;

void Buck_StartRiderJump(GameObject_s *rider, GameObject_s *mount);

void SetEffectVisibility(char *name, i32 visible);

i32 Conveyor_AdjustSpeed(NUVEC *velocity);

void LevelObjects_InitForLevel(WORLDINFO_s *world);

void EquivalentObjects_Configure(WORLDINFO_s *world, char *config);

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"

#include <stdio.h>
#include <string.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_level_save_directory[256], edbits_level_save_name[256], edbits_level_save_extension[256];
    extern i32 edbits_part_general_page, edbits_part_level_page;
    void eduiMenuRender(eduimenu_s *menu);
}

extern "C" {
    extern void *ed_fnt;
}

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

extern "C" {
extern ed_module_s edpartdesc;
}

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

// Runtime-selected groups appended after the fixed terrain allocation.
extern "C" {
extern i32 IgnoreWallSplines;
}
#include "legoapi/render/core/terrain_stubs_types.h"

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern NUMTL *DebMat[10];
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

// NuFileExists @nufile (nucore_plain.cpp).

extern "C" {

void Debris(i32 update_panel_time);

} // extern "C"

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

void DebrisRegisterCutoffCameraVec(void *position);

extern i32 debris_render_group;

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

i32 FindPlatInst(i32 instance);

i32 NewPlatInst(void *object, i32 instance);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstRotate(i32 index, i32 rotate);

void PlatOnOff(i32 index, i32 enabled);

void SortDebrisRenderStack(void);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

TERRSET *TerrainGetCur(void);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

void UpdateDebrisRenderStackPriority(void);

} // extern "C"

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

extern ADDPART_s Default_ADDPART;

struct rtlset;

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.

    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
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

void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
extern f32 AreaPickupGravity;
extern i16 temp_xrot;
extern i16 temp_zrot;

// Forward declarations for local (static) part/gizmo helper stubs.
struct spacelevel_s;
struct quickboltinfo;

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern GameObject_s *Obj;
extern i32 HIGHGAMEOBJECT, VehicleArea;
extern WORLDINFO_s *WORLD;
extern GAMECAMERA_s *GameCam;
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);

extern "C" f32 coinimpactwait;

extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

void PowerUp_Particles(WORLDINFO_s *, NUVEC *);

void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;

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

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

i32 PARTLookupType(char *name);

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

void ParticleReset(void);

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
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);

void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);

i32 FindPartDebris(PARTDEBSYS_s *system, char *name);

void PartCollide_3D(PART_s *part);

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

void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32);

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

// Animation editor subsystem stubs (static, internal linkage).

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
void edanimRegisterBaseScene(NUGSCN *scene);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

// Remaining animation-editor UI/menu callbacks.

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" i32 PS2_REZ_H;

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

extern "C" void edgraClumpsReset(void);
extern "C" i32 NuRndrDoingScreenGrab;

// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern NUMTL *DebMat[];
}

extern "C" {
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern void *ed_fnt;
    extern i32 EDPP_MAX_TYPES;
    extern debinftype **debtab;
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

void edppStartPage(i32 page);

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
// Clang has no __builtin_va_arg_pack; format here and print the result. The
// generic helper already owns this overload's mangled name, so rename it.
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...)
    __asm__("eduiFntPrintExFormatted");
#else
static inline __attribute__((always_inline)) void eduiFntPrintEx(void *font, int x, int y, int alignment,
                                                                 char *format, ...) {
    if (__builtin_constant_p(alignment) && alignment == 16)
        eduiFntPrintExLeft(font, x, y, alignment, format, __builtin_va_arg_pack());
    else if (__builtin_constant_p(alignment) && alignment == 64)
        eduiFntPrintExCentre(font, x, y, alignment, format, __builtin_va_arg_pack());
    else
        eduiFntPrintExGeneric(font, x, y, alignment, format, __builtin_va_arg_pack());
}
#endif

// Only caller prints the property edit buffer left-aligned.

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.

#include "gameapi/edtools/edtools_types.h"

extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 eduiPropTextPos;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern i32 edpp_nearest;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_num_orphans;
extern i32 edpp_dpad_mode;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

}

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void PlatInstBounce(i32, f32, f32, f32);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern u8 object_switches[128];

extern "C" {
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern NUGSCN * edbits_base_scene;
extern void * edbits_base_terrain;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern i32 edbits_numsounds;
extern i32 edbri_page_used[8];
extern edbridge_s edBridges[64];
}

void edanimDetermineNearestAnim(f32);
extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" i32 NuKeyboard(i32 key);

void edcamSetContoller(i32 invert_pitch);

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
i32 edbitsStartCubemapDump(void);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edpartClearPage(i8 page);
void edppClearPage(i8 page);

i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppStopPage(i32 page_id);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetUsingMenuFocus(i32 enabled);
i32 eduiUsedAlgPad(nupad_s *pad);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include <string.h>
#include <new>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/numath/nurand.h"

extern EdRegistry theRegistry;
extern EdInputContext * Input;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
extern "C" void PlaySfxById(i32, nuvec_s *);
extern i32 EdType_Char;
extern i32 EdType_Short;
extern i32 EdType_Int;
extern i32 EdType_Float;
extern i32 EdType_VuVec;
extern i32 EdType_VuMtx;
extern i32 EdType_Enumeration;
extern i32 EdType_String;
extern i32 EdType_Colour3;
extern i32 EdType_NuHSpecial;
extern i32 EdType_NuVec;
extern i32 EdType_NuMtx;

void SerialiseString(EdStream &, void *, i32);

extern f32 Scale;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
extern SplineHelper theSplineHelper;
extern KnotHelper theKnotHelper;
extern EdSystem theEdSystem;

extern "C" {
extern char edgra_filter_string[16];
    extern NUGSCN *edbits_base_scene;
    extern NUVEC edanim_cam_pos;
extern i32 edgra_last_clump_in_buffer;
extern f32 edgra_global_fadein;
extern f32 edgra_global_fadeout;
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_readout, edpp_num_orphans;
    extern f32 edpp_copy_size;
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
}

void edanimDoInput(nupad_s *pad);

void EdDrawLineCube(VuMtx const &transform, float size, i32 colour);

i32 edanimFileSave(char *path);

extern "C" {
    void DebFreeInstantly(i32 *);
}

void EdDrawLineArrow(VuMtx const &transform, float size, i32 colour);

void EdDrawLineCross(VuVec const &position, float size, i32 colour);

void edanimDrawCursor();

void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour);

i32 edanimParamCreate(i32 instance_id);

void edanimParamDestroy(i32 index);

i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end);

NUGSPLINE *edSpline_SplineFind(NUGSCN *scene, char *name);

void edanimParticlePlace(i32 index, NUVEC *position);

void edanimStartAllPages();

void edanimParticleCreate(NUVEC *position);

void edanimRenderSoundEmitters(i32 parameter_index);

void edanimDetermineNearestAnim(float distance);

void edgraDetermineNearestClump(f32 distance);

void edanimRenderParticleEmitters(i32 parameter_index);

void edanimDetermineNearestSound(float distance);

void edanimDetermineNearestParticle(float distance);

extern EdEnumControl::Item OpenClosedItems[3];
extern EdEnumControl::Item OnOffItems[3];
extern EdEnumControl::Item YesNoItems[3];

extern i32 AxisColour[8];

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

extern f32 MouseScale;

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

// Animation editor subsystem stubs (static, internal linkage).

// Remaining animation-editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern NUMTL *DebMat[];
}

extern "C" {
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern void *ed_fnt;
    extern i32 EDPP_MAX_TYPES;
    extern debinftype **debtab;
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.

// Left-aligned (16) specialisation of the integer print helper.

// Centred (64) specialisation of the integer print helper.

// Only caller prints the property edit buffer left-aligned.

// Only eduiMenuRender calls the integer clip helper, always left-aligned.

#include "gameapi/edtools/edtools_types.h"

extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 eduiPropTextPos;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern i32 edpp_nearest;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_num_orphans;
extern i32 edpp_dpad_mode;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

}

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void PlatInstBounce(i32, f32, f32, f32);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern "C" {
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern NUGSCN * edbits_base_scene;
extern void * edbits_base_terrain;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern i32 edbits_numsounds;
extern i32 edbri_page_used[8];
extern edbridge_s edBridges[64];
}

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
i32 edbitsStartCubemapDump(void);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edpartClearPage(i8 page);
void edppClearPage(i8 page);

i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppStopPage(i32 page_id);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetUsingMenuFocus(i32 enabled);
i32 eduiUsedAlgPad(nupad_s *pad);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

// Text-picker commit callbacks. Retail keeps them unmangled and local.

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

extern MENUPACKET_s MenuPacket;
extern i32 NextArea_FreePlay;
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
extern "C" void BackupMenu(void);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern i32 SAVESLOTS;
extern char **TTab;
extern i16 tCANCEL;
extern "C" OPTIONSSAVE TempOptions;
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
void NewGame(void);
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;

extern i32 memcard_saveneeded;
extern i32 memcard_loadneeded;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

extern "C" i32 FinishLoop_On;

void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

void IconScenes_Dump();

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *);

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end);

NUGSCN *IconScene_FindById(i32 character_id);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

extern "C" {

extern f32 MENUDY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUWARNR;
extern u8 MENUWARNG;
extern u8 MENUWARNB;
extern u8 MENUENTRYR;
extern u8 MENUENTRYG;
extern u8 MENUENTRYB;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 dme_r;
extern u8 dme_g;
extern u8 dme_b;
extern i32 dme_align;
extern i32 dme_rgb;
extern f32 menu_pulse;
extern f32 menu_pulse_speed;
extern i32 menu_pulse_angle;
extern f32 menu_pulsate;
extern f32 menu_pulsate_speed;
extern i32 menu_pulsate_angle;

void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void DrawMenu(i32 paused);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void Draw_CANCEL(MENU *menu);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuReset(void);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

} // extern "C"

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

struct ani3_animheader_s;

extern "C" {

extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
}

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 camfx;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

extern i32 NuRndrDoingScreenGrab;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListBeginCriticalSection(void);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_DeallocateSystemFramebuffers(void);
    // Locale string is owned by the Android/JNI TU; cache only its index here.

void NuIOS_RecordFlurryEvent(char *event_name);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void *NuAnimDataCreate(i32 chunk_count);

void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);

    void NuWindUpdateArray(NUVEC **);

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====
#include "gameapi/edtools/edui.h"

extern "C" {

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
}

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legogame/game.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

extern i32 players_going_forward;

extern f32 zoom_ahead_extra;
extern f32 zoom_ahead_extra_time;
extern f32 speeder_level_height_lerpf;
extern f32 speeder_level_height;
extern SPEEDERCHASEVALUES speedervals[4];
extern f32 speederfirerange;
extern f32 speeder_shootrate;
extern f32 speeder_kill_dist;
extern f32 speeder_midrange_time;
extern f32 speeder_ahead_time;
extern f32 speeder_mode_ahead_timer;
DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i32 objopponent_ignoreaiopponent;

void PodKeyReset();

void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble);

i32 ObjIsTargetSpeeder(GameObject_s *object);

void SpeederChaseA_Init(WORLDINFO_s *world);

void SpeederChaseA_Panel(WORLDINFO_s *);

void SpeederChaseA_Reset(WORLDINFO_s *);

i32 Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32 first_time,
                              f32 elapsed);

void SpeederChaseA_Update(WORLDINFO_s *world);

i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);

extern "C" f32 pod_roll[2];

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
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
i32 GetMenuID(void);
void Hint_CancelCurrent(void);
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

void GameCam_HitRoll();

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();

void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);

extern "C" {

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern NUVEC CustomisePos[2];
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);

void MoveGameCamera(GAMECAMERA_s *camera);

void ViewCamDraw();

void SetDepthOfField();

void SpeedBlur_Apply(WORLDINFO_s *world);

void SpeedBlur_Update();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);

void SetCameraMatrices();

GAMEPAD_s *ViewCamGetGamePad();

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edfile.h"

#include <string.h>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"

void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

i32 EdFileClose();

void EdFileWrite(void *data, i32 len);

void EdFileWriteFloat(f32 value);

void EdFileWriteInt(i32 value);

void EdFileWriteUnsignedInt(u32 value);

void EdFileWriteShort(i16 value);

void EdFileWriteUnsignedShort(u16 value);

void EdFileWriteChar(char value);

void EdFileWriteUnsignedChar(u8 value);

void EdFileWriteNuVec(NUVEC *value);

void EdFileRead(void *buf, i32 len);

f32 EdFileReadFloat();

i32 EdFileReadInt();

u32 EdFileReadUnsignedInt();

i16 EdFileReadShort();

u16 EdFileReadUnsignedShort();

char EdFileReadChar();

unsigned char EdFileReadUnsignedChar();

void EdFileReadNuVec(NUVEC *out);

i32 EdFileWriteMemCard(char *filename, i32 size, void *data);

i32 EdFileReadMemCard(char *filename, i32 size, void *data);

i32 EdFileBackup(char *source, char *destination);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern char FS_LastFileName[64];
extern char FS_Path[256];
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

void SerialiseString(EdStream &stream, void *data, i32 count);

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {

i32 TriggerAutoSave(void);

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

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

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

// Camera zoom state

// Graphics loading flags

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

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

// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.

extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}

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

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

// ==== functions in original address order ====

extern "C" {

i32 cbInteractMenuTitle(edui_interact_s *interact);

i32 cbInteractMenuScrollUp(edui_interact_s *interact);

i32 cbInteractMenuScrollDown(edui_interact_s *interact);

}

extern "C" {

ed_module_s *edmainCurrent(void);

i32 edmainActivate(ed_module_s *module, i32 notify);

void edmainSetCamera(NUMTX *matrix);

NUCAMERA *edmainGetCamera(void);

void edmainExtCamera(NUCAMERA *camera);

void edmainSetCursorEnabled(i32 enabled);

i32 edmainGetCursorEnabled(void);

eduimenu_s *edGetMainMenu(void);

void edmainRegisterLocVec(NUVEC *position);

NUVEC *edmainQueryLocVec(void);

void eduiSetGlobalSliderAccel(f32 acceleration);

void eduiInitMaterials(void);

eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);

void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);

i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);

i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuSortItemsByTxt(eduimenu_s *menu);

void eduiMenuSelectFirstEntry(eduimenu_s *menu);

void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);

f32 eduiGetAnalougePadValue(nupad_s *pad);

eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);

void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);

void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);

i32 eduiItemSetText(eduiitem_s *item, char *text);

}

extern "C" {
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);

eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);

eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);

void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);

eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);

eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);

void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);

void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);

i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);

eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);

}

extern "C" {

eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);

void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);

void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);

void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);

void eduiItemSliderSetVal(edui_slider_s *item, f32 value);

void eduiItemSliderSetFmt(edui_slider_s *item, char *format);

void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);

eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);

i32 eduiItemPropSetText(edui_prop_s *item, char *text);

i32 eduiGetCameraEnabled(void);

i32 NuKeyToAscii(u32 key, i32 shifted);

eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);

eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);

eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);

f32 edmainSetMainMenuScale(f32 scale);

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

void eduiMenuDestroy(eduimenu_s *menu);

void edmainClose(void);

i32 edmainRegister(ed_module_s *module);

void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);

void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);

eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);

void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);

eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);

i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);

void eduiSetUsingMenuFocus(i32 enabled);

i32 eduiGetUsingMenuFocus(void);

void eduiSetDefaultActiveMenu(eduimenu_s *menu);

void eduiSetActiveMenu(eduimenu_s *menu);

i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);

void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);

eduimenu_s *eduiGetActiveMenu(void);

void eduiMenuRender(eduimenu_s *menu);

void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);

i32 eduiMenuDetach(eduimenu_s *menu);

}

extern "C" {

eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);

eduimenu_s *eduiGetActiveMenuParent(void);

i32 eduiClearActiveMenu(void);

i32 eduiMenuIsActive(eduimenu_s *menu);

i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiMenuEnsureSelection(eduimenu_s *menu);

i32 eduiCursorOverMenu(eduimenu_s *menu);

i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);

void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);

void cbInteractMenuKeySelect(eduimenu_s *menu);

i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);

void eduiFlushInteracts(void);

i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);

i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiRenderInteracts(void);

void eduiGetCursorCoords(f32 *x, f32 *y);

void eduiSetCursorCoords(f32 x, f32 y);

void eduiGetCursorDelta(f32 *x, f32 *y);

i32 eduiUsedAlgPad(nupad_s *pad);

void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);

void eduiProcessCursor(f32 delta_time, nupad_s *pad);

i32 edmainProcess(f32 delta_time, nupad_s *pad);

void eduiRenderCursor(void);

void edmainRender(void);

void eduiInit(void);

void edmainInit(void *font, char *configuration_file);

void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);

void eduiSetFont(NUQFNT *font);

void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);

void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);

void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);

void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);

void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);

}

extern "C" {

void edcamMoveEx(nupad_s *pad, f32 delta_time);

void edcamMove(nupad_s *pad);

void edcamSetPos(NUVEC *position);

void edcamSetAng(i32 pitch, i32 yaw);

void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);

void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);

void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);

}

extern "C" {
void edcamSetDist(f32 distance);

f32 edcamGetDist(void);

edcam_s *edcamGetEdCam(void);

void edcamSetOffset(NUVEC *offset);

void edcamGetOffset(NUVEC *offset);

void edcamMtx(NUMTX *matrix);

void edcamSet(void);

NUVEC *edcamGetPosPointer(void);

}

extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

}

extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

}

extern "C" {

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

extern "C" {

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

void FlushMenuHighlights(eduimenu_s *menu);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern "C" {

extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
}

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 camfx;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

extern i32 NuRndrDoingScreenGrab;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListBeginCriticalSection(void);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_DeallocateSystemFramebuffers(void);
    // Locale string is owned by the Android/JNI TU; cache only its index here.

void NuIOS_RecordFlurryEvent(char *event_name);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void *NuAnimDataCreate(i32 chunk_count);

void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);

    void NuWindUpdateArray(NUVEC **);

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

// ==== functions in original address order ====

extern "C" {
void edpartSetParticlePage(i32 page);

}

extern "C" {
void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

}

extern "C" {
extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

void KillPart(PART_s *part, i32 reason);

void DrawParts(i32 keep_offscreen);

PART_s *AddPart(ADDPART_s *params);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

i32 PARTLookupType(char *name);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

}

extern "C" {
i32 PARTLookupTypePageOnly(char *name, i32 page);

}

extern "C" {

i32 edpartLoadPage(char *path, i32 mode, void *scene);

}

extern "C" {

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

void UpdateParts(f32 time);

void edpartClearPage(i8 page);

}

// ==== src/legoapi/characters/core/characters.cpp: declarations ====
#include "nu2api/nu3d/nuportal.h"
#include "decomp.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "legoapi/menus/core/text.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 rtlInitDynamic(VARIPTR *, VARIPTR, i32);
    void DebrisSetup(VARIPTR *, VARIPTR, char *, i32, i32, i32);
    void DebrisRegisterCutoffCameraVec(void *);
    void edgraSetup(VARIPTR *, VARIPTR, i32, i32, i32);
    void InitParts(i32, VARIPTR *, VARIPTR *);
    void ParticleReset(void);
    i32 NuFileExists(char *);
    i32 NuStrCpy(char *, const char *);
    i32 edppLoadPage(char *, i32, usize);
    void NuRndrShadowInit(u8 *);
    void NuTexAnimProgSysInit(void);
    void terrainpickupinit(char *, void **);

    extern i32 Grass_Available;
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern void *perm_debrissys;
}

extern i32 PARTPAGE_GENERAL;

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.
extern char * TexAnimList_LSW[32];

extern "C" {
extern i32 hub_character_ready;
}

extern APICHARACTERMODELLIST_s PermModelList[1];

#include "legoapi/characters/core/characters_types.h"

extern "C" {
    void PlaySfx(char *, nuvec_s *);
}

// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif

extern NUMTL * lightningmat;
extern NUMTL *DebMat[10];

// ==== src/legoapi/core/startup/game.cpp: declarations ====
#include "decomp.h"
#include "globals.h"

#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"

#include <stdio.h>

extern void FinishWeirdoNames(i32);
extern void ReCalculateCompletionPoints(void);
extern void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
extern "C" void NuSound3StopRumble(void);
extern "C" i32 Paused;

namespace {

} // namespace

void NewGame();

// ==== src/legoapi/world/missions.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/world/mission.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/world_shared.h"

extern void SetBonusWinner(i32);
extern GameObject_s *FindGameObject(i32, u32, i32, i32, i32);

void EndChallenge(i32 param1, i32 param2);

void InitChallenge(i32);

void Mission_Clear(MISSIONSYS *ms);

MISSIONDATA *Mission_Active(MISSIONSYS *ms);

GameObject_s *Mission_FindTarget(MISSIONSYS *ms, u64 *target);

i32 Missions_PickupsOff(MISSIONSYS *ms);

i32 Mission_CurrentState(MISSIONSYS *ms);

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

// Retail clears these "fast repeat" latches but never sets them.

extern "C" {

extern f32 MENUDY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUWARNR;
extern u8 MENUWARNG;
extern u8 MENUWARNB;
extern u8 MENUENTRYR;
extern u8 MENUENTRYG;
extern u8 MENUENTRYB;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 dme_r;
extern u8 dme_g;
extern u8 dme_b;
extern i32 dme_align;
extern i32 dme_rgb;
extern f32 menu_pulse;
extern f32 menu_pulse_speed;
extern i32 menu_pulse_angle;
extern f32 menu_pulsate;
extern f32 menu_pulsate_speed;
extern i32 menu_pulsate_angle;

void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void DrawMenu(i32 paused);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void Draw_CANCEL(MENU *menu);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuReset(void);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

} // extern "C"

// ==== src/legoapi/menus/screens/menus.cpp: declarations ====
#include "decomp.h"
#include "batman.h"
#include "gamelib/crc/crc.h"
#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/audio/audio.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/render/fx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);
extern "C" bool TestForController(void);
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_TOGGLERIGHT;
extern "C" i32 reset_area;

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

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

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

// Camera zoom state

// Graphics loading flags

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

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

// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.

extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}

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

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

// ==== src/gameapi/gui/apimenu.cpp: all remaining content ====
#include "gameapi/gui/apimenu_internal.h"

#include <stdio.h>
#include <string.h>

#include "decomp.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

extern "C" i32 NuIOS_IsWidescreen(void);
f32 GameSetSoundVolume(OPTIONSSAVE_s *options);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 volume, i32 flags);
void legoSetMusicVolume(f32 volume);
extern char *Trailer[3];
void MusicClearAll(void);
extern "C" void SoundKillAll(void);
extern i32 saveload_autosave;
extern i32 pause_i_pad;

extern i16 tEXIT;
extern i32 reset_restart;
extern u32 ResetBits;
void Door_Reset(void);

void Arcade_DrawEndMenu(MENU *menu);
void Arcade_UpdateEndMenu(MENU *menu);
void MenuDrawEpisodes(MENU *menu);
void MenuUpdateEpisodes(MENU *menu);

extern MENU GameMenu[10];

extern i32 GameMenuLevel;
extern i32 MenuValidated;
extern i32 MenuResult;
extern i32 CurrentMenuId;

extern "C" bool TestForController();
extern "C" void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment,
                            u8 red, u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box,
                            i32 suppress_draw, u32 alpha);

extern MENUFNINFO MenuInfo[100];

extern i32 MenusUsed;
extern i32 MenuLanguages;
extern char MenuHeader[64];

extern i32 MenuDisableHeaders;

extern u8 MENUHEADERR;
extern u8 MENUHEADERG;
extern u8 MENUHEADERB;

extern i32 MenuFadeEnabled;
extern NUMTL * MenuFadeMtl;

extern i32 MenuDrawDropShadows;

extern char * apitxt_CONTROLLERREMOVED;
extern char * apitxt_PRESSSTART;

extern i32 shop_quit;

extern "C" i32 TriggerAutoSave(void);
i32 UpdateAchievements(STATUSPACKET_s *packet);

void APIMenuDrawGameState(f32 x, f32 y, i32 highlight, i32 slot);

void MenuLoadTechnicalStrings(char *filepath, char *language, VARIPTR *buf, VARIPTR buf_end);

// ==== src/legoapi/core/input/gamepads.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/collect/spacelevel.h"
#include "nu2api/numusic/numusic.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/nupad.h"

#include <string.h>

extern "C" {
    extern i32 Paused;
    extern i32 NewMode;
    extern "C++" FadeSystem FadeSys;
    extern i32 CutSceneWaiting;
}

// Original bss @0x127a500: 64 pads x 0x60 bytes.
extern GAMEPAD_s GamePad[64];
// Original bss @0x127a4e0.

extern u32 GAMEPAD_DRIGHT;
extern u32 GAMEPAD_DLEFT;
extern u32 GAMEPAD_DDOWN;
extern u32 GAMEPAD_DUP;
extern u32 GAMEPAD_LIFT;
extern u32 GAMEPAD_TAG;
extern u32 GAMEPAD_SPECIAL;
extern u32 GAMEPAD_ACTION;
extern u32 GAMEPAD_JUMP;
extern u32 GAMEPAD_R3;

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.
extern f32 PadOldSpeed2[2];
extern f32 PadOldSpeed[2];
extern u16 PadOldAngle2[2];
extern u16 PadOldAngle[2];

void GamePads_Init();

f32 GamePad_Rotate(GameObject_s *object);

i32 GamePad_Waggle(GAMEPAD_s *pad);

GAMEPAD_s *GamePad_Allocate();

u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad);

i32 GamePads_SkipMovie();

extern "C" {

bool TestForController(void);

} // extern "C"

void PadOutPause(i32 port, WORLDINFO_s *world);

void UpdateRumble(RUMBLEPACKET *packet);

void NewBuzzFrames(nupad_s *pad, i32 frames, i32);

extern "C" f32 NuFsqrt(f32);
extern "C" f32 NuRandFloat();

void SpaceRumbleProcess();

void NewRumbleAllPlayers(float strength, float duration, i32 frames, i32);

i32 ObjLookingWithLeftStick(GameObject_s *object);

i32 NoPad(i32 port, i32 require_game_input);

void NewBuzz(nupad_s *pad, float duration, i32);

i32 ReadPad(i32 port);

void ReadPads();

void NewRumble(nupad_s *pad, float strength, i32);

// ==== src/gameframework/savesystem.cpp: all remaining content ====
#include <string.h>

#include "decomp.h"

#include "gameframework/saveload.h"

#include "nu2api/nucore/numemory.h"

extern void * memcard_savedata;
extern i32 memcard_savedatasize;
extern void * memcard_savedatabuffer;

extern void * memcard_extra_savedata;
extern i32 memcard_extra_savedatasize;
extern void * memcard_extra_savedatabuffer;

extern void (*memcard_drawasiconfn)();
extern i32 memcard_autosaveneeded;
extern i32 memcard_autosavestarted;
extern i32 memcard_autosaveinprogress;
extern f32 memcard_autosavepredelay;
extern f32 memcard_autosavepostdelay;

void SaveSystemInitialise(i32 slots, void *makeSaveHash, void *save, i32 saveSize, i32 saveCount,
                          void (*drawSaveIcon)(void), void *extradata, i32 extradataSize);

i32 ChecksumSaveData(void *buffer, i32 size);

bool TriggerExtraDataSave(void);

i32 TriggerExtraDataLoad(void);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.

extern "C" {

i32 TriggerAutoSave(void);

} // extern "C"

// ==== src/legoapi/menus/core/text.cpp: declarations ====
#include "decomp.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/text.h"
extern char * ASCII_UP;
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include <stdio.h>
#include <string.h>

// Original .data @0x667ce0; the names are filled in by the static initializer.
extern LANGUAGEDATA * Text_LanguageList;
extern "C" {
extern i32 smarttextex_drawmessagebox;
    void NuQFntSetJustifiedTolerances(float squash, float stretch);
    unsigned char *NuUnicodeCharFromUTF8(u16 *character, unsigned char *text);
    unsigned char *NuUTF8CharFromUnicode(unsigned char *text, u16 character);
    void NuLanguageSet(i32 language);
    i32 NuRndrBeginScene(i32 flags);
}
extern void (*Text_GameSetLanguageFn)(i32);
char *Text_GetLanguagePath(i32 language);
extern "C" {
    extern u8 MENUFLASH1R;
    extern u8 MENUFLASH1G;
    extern u8 MENUFLASH1B;
    extern u8 MENUFLASH0R;
    extern u8 MENUFLASH0G;
    extern u8 MENUFLASH0B;
    extern u8 MENUENTRYR;
    extern u8 MENUENTRYG;
    extern u8 MENUENTRYB;
    extern u8 MENUNORMALR;
    extern u8 MENUNORMALG;
    extern u8 MENUNORMALB;
}
#include "nu2api/numath/numtx.h"
extern "C" {
    void NuMtxSetRotationX(NUMTX *mtx, i32 angle);
    void NuMtxRotateY(NUMTX *mtx, i32 angle);
    void NuMtxRotateZ(NUMTX *mtx, i32 angle);
    void NuMtxTranslate(NUMTX *mtx, NUVEC *vec);
}
extern "C" void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, i32 alpha);
extern "C" void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                          u8 green, u8 blue, i32 alpha);
void Text_MakeTime(float time, i32 show_hours, i32 show_minutes, i32 show_centiseconds, char *text);

void TextCrawl_Init(TEXTCRAWL_s *crawl, i32 id, i32 unk);

void TextCrawl_Draw(float dt, i32 paragraphs, float alpha, char *text);

void Text_MakeScore(u32 score, char *text);
void Text_SetLanguage(i32 language);
extern f32 g_buttonFontScalePulse;
extern f32 smarttextex_longestwidth;
extern i32 MenuStopDraw;

extern char * txtpath_JAPANESE;
extern char * txtpath_DANISH;
extern char * txtpath_SPANISH;
extern char * txtpath_ITALIAN;
extern char * txtpath_GERMAN;
extern char * txtpath_FRENCH;
extern char * txtpath_ENGLISH;

void Text_InitLanguageList(LANGUAGEDATA *language_list);
void Text_SetLanguage_Game(i32 language);
i32 Text_GetMaxOverallStrings();
void Text_LocaliseDecimalPoint(char *text);
void Text_ExpandAllButtonStrings(char *input, char *output);

extern "C" void MessageBoxInitMtl(void);

void DrawMessageBoxRGBA(f32 x, f32 y, f32 width, f32 height, u32 blue, u32 green, u32 red, u32 alpha, numtl_s *material,
                        i32 alignment, f32 scale);

void DrawMessageBox(i32, f32 x, f32 y, f32 width, f32 height);

extern "C" {

void MenuSmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         i32 alpha);
void MenuText3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, u8 alpha);
void Set3DGameFont(VUFNT *font);
void SetGameFont(VUFNT *font);
void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);
void SmartTextEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                      u32 alpha);
void SmartTextGetWidescreen(f32 *font_scale_x, f32 *coordinate_scale);
void SmartTextSetFWNMode(i32 mode);
void SmartTextSetFont(VUFNT *font);
void SmartTextSetWidescreen(f32 font_scale_x, f32 coordinate_scale);
void Text3D(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red, u8 green,
                u8 blue);
void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32, u32 alignment, u8 red, u8 green,
                  u8 blue, i32 alpha);
void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                   u8 green, u8 blue, i32 alpha);
void TextRegisterButtonMapFn(void (*fn)(char *, char *));
void TextRegisterPulseTimerFn(f32 (*fn)(f32));
}
void MenuUpdateViewTextStrings(MENU_s *menu);

void MenuDrawViewTextStrings(MENU_s *menu);

extern "C" void Text3DStringEncode(char *src, u16 *dst);
extern "C" void SetQFont2D(void);

// ==== src/legoapi/items/objects/objectsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nu3d/nuspecial.h"
#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufile.h"

#include <string.h>

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"

#include <stdio.h>
#include <string.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_level_save_directory[256], edbits_level_save_name[256], edbits_level_save_extension[256];
    extern i32 edbits_part_general_page, edbits_part_level_page;
    void eduiMenuRender(eduimenu_s *menu);
}

extern "C" {
    extern void *ed_fnt;
}

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

extern "C" {
extern ed_module_s edpartdesc;
}

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

// Runtime-selected groups appended after the fixed terrain allocation.
extern "C" {
extern i32 IgnoreWallSplines;
}
#include "legoapi/render/core/terrain_stubs_types.h"

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern NUMTL *DebMat[10];
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

// NuFileExists @nufile (nucore_plain.cpp).

extern "C" {

void Debris(i32 update_panel_time);

} // extern "C"

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

void DebrisRegisterCutoffCameraVec(void *position);

extern i32 debris_render_group;

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

i32 FindPlatInst(i32 instance);

i32 NewPlatInst(void *object, i32 instance);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstRotate(i32 index, i32 rotate);

void PlatOnOff(i32 index, i32 enabled);

void SortDebrisRenderStack(void);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

TERRSET *TerrainGetCur(void);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

void UpdateDebrisRenderStackPriority(void);

} // extern "C"

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

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.

    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
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

// Forward declarations for local (static) part/gizmo helper stubs.

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

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

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

i32 PARTLookupType(char *name);

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

void ParticleReset(void);

    i32 rtlDynamicSetPos(i32, NUVEC *);

void UpdateParts(f32 time);

} // extern "C"

extern "C" {
extern part_typedesc_s * edpart_nearest_type;
}

extern "C" {

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}

// The original repeats the per-sound emitter checks for each of the four sound slots.
#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

// Animation editor subsystem stubs (static, internal linkage).

// Remaining animation-editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern NUMTL *DebMat[];
}

extern "C" {
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern void *ed_fnt;
    extern i32 EDPP_MAX_TYPES;
    extern debinftype **debtab;
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.

// Left-aligned (16) specialisation of the integer print helper.

// Centred (64) specialisation of the integer print helper.

// Only caller prints the property edit buffer left-aligned.

// Only eduiMenuRender calls the integer clip helper, always left-aligned.

#include "gameapi/edtools/edtools_types.h"

extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 eduiPropTextPos;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern i32 edpp_nearest;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_num_orphans;
extern i32 edpp_dpad_mode;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

}

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void PlatInstBounce(i32, f32, f32, f32);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern "C" {
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern NUGSCN * edbits_base_scene;
extern void * edbits_base_terrain;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern i32 edbits_numsounds;
extern i32 edbri_page_used[8];
extern edbridge_s edBridges[64];
}

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
i32 edbitsStartCubemapDump(void);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edpartClearPage(i8 page);
void edppClearPage(i8 page);

i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppStopPage(i32 page_id);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetUsingMenuFocus(i32 enabled);
i32 eduiUsedAlgPad(nupad_s *pad);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

// Text-picker commit callbacks. Retail keeps them unmangled and local.

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include <string.h>
#include <new>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/numath/nurand.h"

extern "C" {
extern char edgra_filter_string[16];
    extern NUGSCN *edbits_base_scene;
    extern NUVEC edanim_cam_pos;
extern i32 edgra_last_clump_in_buffer;
extern f32 edgra_global_fadein;
extern f32 edgra_global_fadeout;
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_readout, edpp_num_orphans;
    extern f32 edpp_copy_size;
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
}

extern "C" {
    void DebFreeInstantly(i32 *);
}

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

// Animation editor subsystem stubs (static, internal linkage).

// Remaining animation-editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern NUMTL *DebMat[];
}

extern "C" {
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern void *ed_fnt;
    extern i32 EDPP_MAX_TYPES;
    extern debinftype **debtab;
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.

// Left-aligned (16) specialisation of the integer print helper.

// Centred (64) specialisation of the integer print helper.

// Only caller prints the property edit buffer left-aligned.

// Only eduiMenuRender calls the integer clip helper, always left-aligned.

#include "gameapi/edtools/edtools_types.h"

extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 eduiPropTextPos;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern i32 edpp_nearest;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_num_orphans;
extern i32 edpp_dpad_mode;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

}

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void PlatInstBounce(i32, f32, f32, f32);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern "C" {
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern NUGSCN * edbits_base_scene;
extern void * edbits_base_terrain;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern i32 edbits_numsounds;
extern i32 edbri_page_used[8];
extern edbridge_s edBridges[64];
}

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
i32 edbitsStartCubemapDump(void);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edpartClearPage(i8 page);
void edppClearPage(i8 page);

i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppStopPage(i32 page_id);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetUsingMenuFocus(i32 enabled);
i32 eduiUsedAlgPad(nupad_s *pad);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

// Text-picker commit callbacks. Retail keeps them unmangled and local.

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

// Retail clears these "fast repeat" latches but never sets them.

extern "C" {

extern f32 MENUDY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUWARNR;
extern u8 MENUWARNG;
extern u8 MENUWARNB;
extern u8 MENUENTRYR;
extern u8 MENUENTRYG;
extern u8 MENUENTRYB;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 dme_r;
extern u8 dme_g;
extern u8 dme_b;
extern i32 dme_align;
extern i32 dme_rgb;
extern f32 menu_pulse;
extern f32 menu_pulse_speed;
extern i32 menu_pulse_angle;
extern f32 menu_pulsate;
extern f32 menu_pulsate_speed;
extern i32 menu_pulsate_angle;

void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void DrawMenu(i32 paused);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void Draw_CANCEL(MENU *menu);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuReset(void);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

} // extern "C"

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern "C" {

extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
}

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 camfx;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

extern i32 NuRndrDoingScreenGrab;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListBeginCriticalSection(void);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_DeallocateSystemFramebuffers(void);
    // Locale string is owned by the Android/JNI TU; cache only its index here.

void NuIOS_RecordFlurryEvent(char *event_name);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void *NuAnimDataCreate(i32 chunk_count);

void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);

    void NuWindUpdateArray(NUVEC **);

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====
#include "gameapi/edtools/edui.h"

extern "C" {

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
}

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legogame/game.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
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

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

extern "C" {

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edfile.h"

#include <string.h>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.

extern "C" {

i32 TriggerAutoSave(void);

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

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

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

// Camera zoom state

// Graphics loading flags

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

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

// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.

extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}

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

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

// ==== functions in original address order ====

extern "C" {

i32 cbInteractMenuTitle(edui_interact_s *interact);

i32 cbInteractMenuScrollUp(edui_interact_s *interact);

i32 cbInteractMenuScrollDown(edui_interact_s *interact);

}

extern "C" {

ed_module_s *edmainCurrent(void);

i32 edmainActivate(ed_module_s *module, i32 notify);

void edmainSetCamera(NUMTX *matrix);

NUCAMERA *edmainGetCamera(void);

void edmainExtCamera(NUCAMERA *camera);

void edmainSetCursorEnabled(i32 enabled);

i32 edmainGetCursorEnabled(void);

eduimenu_s *edGetMainMenu(void);

void edmainRegisterLocVec(NUVEC *position);

NUVEC *edmainQueryLocVec(void);

void eduiSetGlobalSliderAccel(f32 acceleration);

void eduiInitMaterials(void);

eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);

void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);

i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);

i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuSortItemsByTxt(eduimenu_s *menu);

void eduiMenuSelectFirstEntry(eduimenu_s *menu);

void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);

f32 eduiGetAnalougePadValue(nupad_s *pad);

eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);

void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);

void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);

i32 eduiItemSetText(eduiitem_s *item, char *text);

}

extern "C" {
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);

eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);

eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);

void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);

eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);

eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);

void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);

void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);

i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);

eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);

}

extern "C" {

eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);

void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);

void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);

void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);

void eduiItemSliderSetVal(edui_slider_s *item, f32 value);

void eduiItemSliderSetFmt(edui_slider_s *item, char *format);

void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);

eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);

i32 eduiItemPropSetText(edui_prop_s *item, char *text);

i32 eduiGetCameraEnabled(void);

i32 NuKeyToAscii(u32 key, i32 shifted);

eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);

eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);

eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);

f32 edmainSetMainMenuScale(f32 scale);

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

void eduiMenuDestroy(eduimenu_s *menu);

void edmainClose(void);

i32 edmainRegister(ed_module_s *module);

void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);

void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);

eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);

void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);

eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);

i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);

void eduiSetUsingMenuFocus(i32 enabled);

i32 eduiGetUsingMenuFocus(void);

void eduiSetDefaultActiveMenu(eduimenu_s *menu);

void eduiSetActiveMenu(eduimenu_s *menu);

i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);

void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);

eduimenu_s *eduiGetActiveMenu(void);

void eduiMenuRender(eduimenu_s *menu);

void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);

i32 eduiMenuDetach(eduimenu_s *menu);

}

extern "C" {

eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);

eduimenu_s *eduiGetActiveMenuParent(void);

i32 eduiClearActiveMenu(void);

i32 eduiMenuIsActive(eduimenu_s *menu);

i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiMenuEnsureSelection(eduimenu_s *menu);

i32 eduiCursorOverMenu(eduimenu_s *menu);

i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);

void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);

void cbInteractMenuKeySelect(eduimenu_s *menu);

i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);

void eduiFlushInteracts(void);

i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);

i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiRenderInteracts(void);

void eduiGetCursorCoords(f32 *x, f32 *y);

void eduiSetCursorCoords(f32 x, f32 y);

void eduiGetCursorDelta(f32 *x, f32 *y);

i32 eduiUsedAlgPad(nupad_s *pad);

void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);

void eduiProcessCursor(f32 delta_time, nupad_s *pad);

i32 edmainProcess(f32 delta_time, nupad_s *pad);

void eduiRenderCursor(void);

void edmainRender(void);

void eduiInit(void);

void edmainInit(void *font, char *configuration_file);

void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);

void eduiSetFont(NUQFNT *font);

void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);

void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);

void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);

void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);

void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);

}

extern "C" {

void edcamMoveEx(nupad_s *pad, f32 delta_time);

void edcamMove(nupad_s *pad);

void edcamSetPos(NUVEC *position);

void edcamSetAng(i32 pitch, i32 yaw);

void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);

void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);

void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);

}

extern "C" {
void edcamSetDist(f32 distance);

f32 edcamGetDist(void);

edcam_s *edcamGetEdCam(void);

void edcamSetOffset(NUVEC *offset);

void edcamGetOffset(NUVEC *offset);

void edcamMtx(NUMTX *matrix);

void edcamSet(void);

NUVEC *edcamGetPosPointer(void);

}

extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

}

extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

}

extern "C" {

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

extern "C" {

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

void FlushMenuHighlights(eduimenu_s *menu);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern "C" {

extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
}

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 camfx;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

extern i32 NuRndrDoingScreenGrab;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListBeginCriticalSection(void);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_DeallocateSystemFramebuffers(void);
    // Locale string is owned by the Android/JNI TU; cache only its index here.

void NuIOS_RecordFlurryEvent(char *event_name);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void *NuAnimDataCreate(i32 chunk_count);

void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);

    void NuWindUpdateArray(NUVEC **);

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

// ==== functions in original address order ====

extern "C" {
void edpartSetParticlePage(i32 page);

}

extern "C" {
void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

}

extern "C" {
extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

void KillPart(PART_s *part, i32 reason);

void DrawParts(i32 keep_offscreen);

PART_s *AddPart(ADDPART_s *params);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

i32 PARTLookupType(char *name);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

}

extern "C" {
i32 PARTLookupTypePageOnly(char *name, i32 page);

}

extern "C" {

i32 edpartLoadPage(char *path, i32 mode, void *scene);

}

extern "C" {

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

void UpdateParts(f32 time);

void edpartClearPage(i8 page);

}

// ==== src/legoapi/misc/legoapi_status.cpp: declarations ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"

// ==== functions in original address order ====

extern "C" {

void MenuReset(void);

void MenuRememberCursor(MENU *menu);

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);

}

extern "C" {
void BackupMenu(void);

}

extern "C" {
void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

}

extern "C" {

void DrawMenu(i32 paused);

}

extern "C" {
i32 MenuGetSlotNum(void);

i32 MenuInMemoryCard(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

i32 MenuCurrentID(void);

}

extern "C" {
bool TestForController(void);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

}

extern "C" {

i32 TriggerAutoSave(void);

}

extern "C" {
extern "C" void Text3DStringEncode(char *src, u16 *dst);

void TextRegisterButtonMapFn(void (*fn)(char *, char *));

void TextRegisterPulseTimerFn(f32 (*fn)(f32));

}

extern "C" {
void SetButtonScaleMode(i32 mode);

void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32, u32 alignment, u8 red, u8 green,
                  u8 blue, i32 alpha);

void Text3D(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red, u8 green,
                u8 blue);

void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                   u8 green, u8 blue, i32 alpha);

void MenuText3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, u8 alpha);

}

extern "C" {
extern "C" void MessageBoxInitMtl(void);

}

extern "C" {
void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);

void SmartTextEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                      u32 alpha);

void MenuSmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         i32 alpha);

}

extern "C" {
void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuEntry(MENU *menu, char *text);

}

extern "C" {
void Draw_CANCEL(MENU *menu);

}

extern "C" {

void DrawMenuHeader(void);

void SmartTextSetFWNMode(i32 mode);

void SmartTextSetFont(VUFNT *font);

void SetGameFont(VUFNT *font);

void Set3DGameFont(VUFNT *font);

extern "C" void SetQFont2D(void);

void SmartTextSetWidescreen(f32 font_scale_x, f32 coordinate_scale);

void SmartTextGetWidescreen(f32 *font_scale_x, f32 *coordinate_scale);

}

// ==== src/legoapi/menus/screens/gamestatus_lsw.cpp: declarations ====
#include <string.h>
#include <stdio.h>

#include "decomp.h"
#include "globals.h"
#include "gameframework/saveload.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "gameapi/gui/apimenu.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/render/core/screen.h"

extern "C" i32 reset_load;
extern "C" STATUSPACKET_s StatusPacket;

extern "C" i32 MenuInMemoryCard();
extern "C" void BackupMenu();

extern f32 PANEL_SCOREX;
extern f32 PANEL_SCORESCALE;
void Text_MakeScore(u32, char *);

extern u16 hub_iconang[4];
void LevelComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);

void SetBonusWinner(i32 player);

void ResetIconWibble();

extern i16 id_SLAVE1;
extern i16 tCHALLENGECOMPLETE, tTRUEHERO, tMINIKIT;
extern "C" void NuIOS_RecordFlurryEvent(char *);
extern "C" i32 NuStrCpy(char *, const char *);
i32 Mission_CurrentState(MISSIONSYS *);

extern NEWMINIPIECE_s NewMiniPiece[10];

void InitStatusScreen(WORLDINFO_s *world);

void UpdateIconWibble();

extern "C" void NuStrCat(char *, const char *);

i32 UpdateAchievements(STATUSPACKET_s *);

void UpdateStatusScreen(WORLDINFO_s *);

void DrawStatusScreen(WORLDINFO_s *);

extern i32 grab_screen_image;

extern f32 text3d_width, text3d_height;
extern "C" {
    extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
    extern u8 MENUNORMALR, MENUNORMALG, MENUNORMALB;
    extern u8 MENUFLASH0R, MENUFLASH0G, MENUFLASH0B;
    extern u8 MENUFLASH1R, MENUFLASH1G, MENUFLASH1B;
}
void Hint_SetComplete(i32);
void Hint_Draw(i32);

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.

// Retail keeps the active-stage drawing as the split-out .part clone of
// LevelComplete_LSW_Draw; the inline head only tests `active`.

void LevelComplete_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

// ==== src/legoapi/menus/screens/shop.cpp: declarations ====
#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gamelib/crc/crc.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/render/fx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/world/area.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx_inline.h"

#include <string.h>
#include <stdio.h>
void SetLevelLights(void *, f32);
i32 Draw3DObject(WORLDINFO_s *, i32, nuvec_s *, u16, u16, u16, float, float, float, i32);
// Shop state is file-local, matching the original _ZL symbols.
extern i32 codevalid;
extern i32 codechar;
extern char codechars[37];

extern shopitem_s TopShelf[6];
extern shopitem_s * CharItems;
extern shopitem_s * HintItems;
extern shopitem_s * ExtraItems;
extern shopitem_s * CodeItems;
extern shopitem_s * BrickItems;
extern shopitem_s CutItems[128];
extern nuhspecial_s extrasils[44];
extern nuhspecial_s atoz0to9icon[36];

extern u32 codelist[144];
extern i32 SHOPCHARCOUNT;
extern i32 SHOPEXTRACOUNT;

extern i32 CharShelfIds[7];
extern i32 HintShelfIds[7];
extern i32 ExtraShelfIds[7];
extern i32 BrickShelfIds[7];
extern i32 CutShelfIds[7];
extern NUVEC CharCurPos[7];
extern NUVEC HintCurPos[7];
extern NUVEC ExtraCurPos[7];
extern NUVEC BrickCurPos[7];
extern NUVEC CutCurPos[7];

extern NUVEC ShelfPos[6];
extern NUVEC SubShelfPos[7];
extern NUVEC CodePos[7];
extern u16 shelfang;

extern nuhspecial_s iconback;
extern nuhspecial_s infoblank;
extern nuhspecial_s cutblank;
extern nuhspecial_s cutfilm_unlocked;
extern nuhspecial_s cutfilm_locked;
extern nuhspecial_s toolblank;
extern nuhspecial_s codeblank;
extern nuhspecial_s question;
extern nuhspecial_s arrow1;
extern nuhspecial_s arrow2;
extern nuhspecial_s arrow3;
extern nuhspecial_s arrow4;
extern NUGSPLINE * shopcamspline;
extern NUVEC * shopcampos;
extern NUVEC * shopcamlookat;
extern i32 oldpicked;
extern i32 picked;
extern i32 subpicked;
extern i32 subitemselected;

extern NUVEC selectedoff;
extern void * shopcutsceneplayer;

extern f32 ShopLockedScale;
extern f32 ShopNameAlpha;
extern i32 enteredshop;
extern i32 SHOPACTIVE;
extern i32 shopmenu;
extern i32 col;
extern char usercode[6];
extern i32 shop_from_cutsceneplayer;

extern HINT_s *Hint_FindHint(i32 hint_id);

i32 DoShopMenu(MENU_s *menu);

i32 BuyShopItem(shopitem_s *items, i32 index, i32 charge);

i32 UpdateShop(MENU_s *menu);

void SelectSubItem();

i32 SelectShopItem(shopitem_s *items, i32 index);

i32 Shop_UpdateHint(HINT_s *hint);

void GetShopCamLookPos(nuvec_s *position);

i32 MoveSubItemsRight(i32 *ids, NUVEC *positions, i32 count);

i32 MoveSubItemsLeft(i32 *ids, NUVEC *positions, i32 count);

void InitShop(WORLDINFO_s *world);

i32 CheckCash(shopitem_s *items, i32 item);

extern void Hint_CancelCurrent(void);

extern i16 tSELECT, tEXIT, tBACK, tBUY, tPLAY, tSELECTING;

void LoadShelfSplines();

void DrawTopShelf(i32);

// ==== functions in original address order ====

// ==== src/legoapi/cutscenes/cutscenes.cpp: declarations ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/audio/sfx.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/core/screen.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>
#include <stdio.h>
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

extern "C" i32 CUTDRAWWORLD;
extern "C" i32 CutSceneWaiting;
extern "C" i32 newlevel_resumecutaudio;
extern CUTSCENESYS * CutSceneSys;

void EnableShadowMapRendering(i32);
void ResetShadowMapRendering(void);
i32 qrand(void);
extern "C" void APITransparentCharDraw(nuhgobj_s *, NUMTX *, i32, i16 *, NUMTX *, void **, i32);
CUTSCENEPLAYERCLIP *CutScenePlayer_Active(void);

extern "C" {
    extern i16 id_ANAKINSNEWPOD;
    extern i16 id_ANAKINSNEWPODGREEN;
    extern i16 id_REPUBLICGUNSHIP_GREEN;
    extern i16 id_NEW_REPUBLIC_GUNSHIP_GREEN;
    extern i16 id_JEDISTARFIGHTERYELLOWEP3;
}

extern "C" i32 drawcharactermodel_nobsa;

extern "C" {
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
}

// Retail only keeps the split-out body of the IPA-SRA clone (the unused
// cutscene index dropped), called with world and cut in registers.
#if defined(__i386__)
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone))
#endif

void CutScenes_End();

void CutScenes_Draw(WORLDINFO_s *world);

void CutScenes_Stop(CUTSYS *system);

void CutScenes_Start(WORLDINFO_s *world);

void CutScenes_Update(WORLDINFO_s *world, i32 paused);

CUTINFO *CutScene_Find(CUTSYS *cutscene_system, char *name);

i32 CutScene_HasPlayed(CUTINFO *cut);

void CutScene_SnapToEnd(CUTINFO *cut);

i32 CutScene_IsSkippable(CUTINFO *cut);

// The loop re-reads the active cut through CutStopInfo after each SmartTextEx call.
#define CUT_STOP static_cast<CUTINFO *>(CutStopInfo)
void CutScene_DrawSubtitles();

void CutScene_StoppedFn_LSW(CUTINFO *cut);

void CutScenes_BGLoadManager();

i32 CutScene_PlayingOrRequested(CUTINFO *cut);

CUTINFO *NewCutScene(CUTINFO *cut, CUTSYS *system, char *name, i32);

void AddFancyMessage(char *, f32, f32, f32, f32, i32, i32);
void *AddGameMessage(char *, nuvec_s *, f32, nuvec_s *, f32, u8, u8, u8, u32, f32);

// ==== src/legoapi/cutscenes/gcutscn.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"

#include "globals.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"

#include <string.h>
#include <stdio.h>
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/render/core/render.h"
#include "gameapi/gui/apimenu.h"

extern i32 (*CutScenePlayer_AcceptFn)(CUTSCENEPLAYERCLIP *);

void EvaluateJointOrientationMtx(nugscn_s *scene, i32 joint_index, numtx_s *matrix);

extern CUTSCENEPLAYER_s * CutScenePlayer;

void CutScenePlayer_Start(i32 clip_id, i32 door);

CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

void CutScenePlayer_GetText(i32 id, char *title, char *subtitle, i32 mode);

i32 CutScenePlayer_CanStart(i32 clip_id);

void CutScenePlayer_DrawGrid(COLLECTION_s *collection, i16 *ids, float x, float y, i32 selected, float alpha);

void CutScenePlayer_Configure(char *file, VARIPTR *buffer, VARIPTR *, i16 *clip_text, i16 *intro, i16 *midtro,
                              i16 *outro, i16 *ending);

i32 CutScenePlayer_CountEpisodeClips(i32 episode, i32 include_guests, i16 *ids);

void FindGameCutScenes();

i32 FindMtlInHGObj(nugscn_s *scene, i32 material_type);

// ==== src/legoapi/cutscenes/cutscene.cpp: declarations ====
#include "decomp.h"
#include "globals.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/cutscenes/cutscene_defrag.h"
#include "nu2api/nucore/nugcutscene.h"
#include "legoapi/world/world_shared.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "legoapi/characters/core/character.h"
#include "legoapi/render/fx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"

// --- Extern "C" block: functions with confirmed C linkage in original libTTapp.so ---

#include "legoapi/cutscenes/cutscene_types.h"

// Exported (default visibility) in the original library.

// The relocation tables are patched as plain words, so every fixup aliases
// cutscene->string_delta and re-reads it.
#define NUGCUT_WORD(field) (*reinterpret_cast<NuGCutWord *>(&(field)))
#define NUGCUT_RELOCATE(field, delta) \
    (NUGCUT_WORD(field) = NUGCUT_WORD(field) != 0 ? NUGCUT_WORD(field) + (delta) : 0)

// Retail keeps NuGCutSceneFixPtrs only as two constant-propagated clones:
// .44 relocates animations by anim_delta, .43 is the anim_delta == 0 case.
#if defined(__i386__)
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone, regparm(n)))
#else
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone))
#endif

extern "C" {

extern NUGCUTUPDATELOCATORVFXFN UpdateLocatorVfxFn;
extern NUGCUTRELEASELOCATORVFXFN ReleaseLocatorVfxFn;
extern NUGCUTTRIGGERLOCATORVFXFN TriggerLocatorVfxFn;
extern NUGCUTLOOKUPLOCATORVFXFN LookupLocatorVfxFn;
NUGCUTSCENE_s *NuGCutSceneLoad(char *name, VARIPTR *buf, VARIPTR *buf_end, i32 flags);

NUGCUTSCENE_s *NuGCutSceneLoadAddr(NUGCUTSCENE_s *cutscene, i32 loaded_size, VARIPTR *buffer);

void NuGCutSceneDestroy(NUGCUTSCENE_s *cutscene);

void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene, NUGSCN *scene, i32 flags, i32 area);
void NuGCutSceneFixUpExtra(NUGCUTSCENE_s *cutscene, NUGSCN *area);
instNUGCUTSCENE_s *instNuGCutSceneCreate(NUGCUTSCENE_s *cutscene, NUGSCN *scene, void *extra, char *name,
                                             VARIPTR *buf, i32 flags);

void PetesHackOfDeath(void);
} // extern "C"

// Retail only keeps the IPA-SRA clone, which drops the unused frame argument.
#if defined(__i386__)
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone))
#endif

// ==== src/legoapi/cutscenes/cutscene_stubs.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscene_defrag.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <cfloat>
#include <string.h>

extern "C" {
extern CutSceneCleanUpEntry * DefragCutSceneList;
extern CutSceneCleanUpEntry * DefragCutSceneListBase;
extern void * DefragCutSceneBaseMem;
extern void * DefragCutSceneEndMem;
extern void * DefragInstBaseMem;
extern void * DefragInstEndMem;
extern instNUGCUTSCENE_s *(*DefragGetInstFn)(void *);
extern void *(*DefragCreateInstFn)(void *, NUGCUTSCENE_s *, VARIPTR *);
extern void (*DefragInstDestroyedFn)(void *);
extern i32 DefragCutSceneListSize;
    extern debinftype **debtab;

    void NuMtxPreTranslate(NUMTX *, NUVEC *);
    void NuMtxMul(NUMTX *, NUMTX *, NUMTX *);
    i32 NuStrICmp(const char *, const char *);

    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
}

extern "C" {

void RestoreGameCut(void);

i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index);

void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf);

void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix);

} // extern "C"

// Steers a cutscene character's eyes towards a look-at locator and blinks its eyelids.
// Only version-3 locator animations carry the look and blink channels.
// Retail only keeps the IPA-SRA clone: the system is dropped and the instance
// locator is replaced by its effect handle slot.
#if defined(__i386__)
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone))
#endif

extern "C" {
    // Aligned four-float row used for retail's movaps matrix copies.

} // extern "C"

// ==== src/nu2api/nusound/nu2api_nusound_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>

extern "C" {
extern i32 g_NuSoundMaxVoicesPerSample;
extern f32 AUDIOFADELEVEL;
extern f32 MASTERVOLUME;

extern MusicPlayback Music;

    void NuSound3FlushLoops(void);

void SoundKillAll(void);

void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern "C" {

extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
}

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 camfx;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

extern i32 NuRndrDoingScreenGrab;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListBeginCriticalSection(void);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_DeallocateSystemFramebuffers(void);
    // Locale string is owned by the Android/JNI TU; cache only its index here.

void NuIOS_RecordFlurryEvent(char *event_name);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void *NuAnimDataCreate(i32 chunk_count);

void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);

    void NuWindUpdateArray(NUVEC **);

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

// ==== src/nu2api/nucore/nugcutscene_anim.cpp: all remaining content ====
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/numtx.h"

f32 *NuGCutLocatorCalcMtx_3(NUGCUTLOCATOR_s *locator, numtx_s *mtx, float frame);

extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *locator, float frame, NUMTX *mtx, nuanimtime_s *time);

i32 NuGCutLocatorIsVisble_3(NUGCUTLOCATOR_s *locator, float frame, float *scale, float *rate);

extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *locator, float frame, nuanimtime_s *time, float *scale,
                                     float *rate);

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====
#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>

void EvalAnim(nuhspecial_s *special, f32 frame, numtx_s *matrix, i32 include_instance_translation);
i32 GetDefaultIdle(GameObject_s *object);
// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.

extern i16 id_BODYGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_YODA;
extern i16 id_YODAGHOST;

#include "legoapi/characters/motion/gameanim_types.h"

void Animate_ATAT(GameObject_s *object);

void Animate_POD(GameObject_s *object);

void Animate_JEDI(GameObject_s *object);

void Animate_BEAST(GameObject_s *object);

void Animate_BARMAN(GameObject_s *object);

void Animate_CANNON(GameObject_s *object);

void Animate_WALKER(GameObject_s *object);

void Animate_CRITTER(GameObject_s *object);

void Animate_WEIRDO(GameObject_s *object);

void Animate_DEFAULT(GameObject_s *object);

void Animate_VEHICLE(GameObject_s *object);

void Animate_PROTOCOL(GameObject_s *object);

void Animate_DROIDEKA(GameObject_s *object);

i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state);

i32 GameAnimSet_Stop(GAMEANIMSET_s *set);

void Animate_ASTROMECH(GameObject_s *object);

void Animate_GEONOSIAN(GameObject_s *object);

i32 GameAnimSet_Reset(GAMEANIMSET_s *set);

void Animate_HOVERDROID(GameObject_s *object);

GAMEANIMSET_s *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, GAMEANIMOBJPOOL_s *object_pool,
                                  GAMEANIMSYS_s *system);

void Animate_BATTLEDROID(GameObject_s *object);

void Animate_SPEEDERBIKE(GameObject_s *object);

i32 GameAnimSet_Playing(GAMEANIMSET_s *set);

void GameAnimSet_EvalAnim(GAMEANIMSET_s *set);

GAMEANIMOBJ_s *GameAnimSet_AddObject(GAMEANIMSET_s *set, nuhspecial_s *special, float start_frame, float end_frame,
                                     i32 append);

i32 GameAnimSet_JumpToEnd(GAMEANIMSET_s *set);

void GameAnimSet_SetOffset(GAMEANIMSET_s *set, NUVEC *offset);

f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ_s *object);

void GameAnimSet_SetAnimPos(GAMEANIMOBJ_s *object, float position);

void GameAnimSet_SetTFactor(GAMEANIMSET_s *set, float factor);

void Animate_REPUBLICGUNSHIP(GameObject_s *object);

i32 GameAnimSet_JumpToStart(GAMEANIMSET_s *set);

void Animate_SUPERBATTLEDROID(GameObject_s *object);

void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale);

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating);

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set);

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static);

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set);

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position);

void GameAnimSet_SetVisibility(GAMEANIMSET_s *set, i32 visibility);

void GameAnimSet_DrawReflection(GAMEANIMSET_s *set, i32 axis, float offset, numtx_s *matrix);

GAMEANIMOBJ_s *GameAnimSet_AddObjectByName(GAMEANIMSET_s *set, nugscn_s *scene, char *name, float start_frame,
                                           float end_frame, i32 append, GIZMOSYS_s *gizmo_sys, char *prefix,
                                           char *suffix);

void GameAnimSet_AddToSystemList(GAMEANIMSET_s *set);

f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET_s *set, nuvec_s *position, numtx_s *matrix);

f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET_s *set);

GAMEANIMOBJPOOL_s *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 object_data_size,
                                                i32 capacity);

i32 GameAnimSet_IsAnimationReset(GAMEANIMSET_s *set);

void GameAnimSet_RemoveAllObjects(GAMEANIMSET_s *set);

i32 GameAnimSet_GetCentreAndRadius(GAMEANIMSET_s *set, NUVEC *centre, f32 *radius, i32 frame_selection,
                                   i32 include_animated, i32 include_static);

f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET_s *set);

void GameAnimSet_RemoveFromSystemList(GAMEANIMSET_s *set);

extern "C" {

void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32);

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

extern NUJOINTPROCANIMFN JointProcAnimFn;

void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame);

} // extern "C"

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation);

void EvalAnim2(nuhspecial_s *special, float frame);

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index);

GAMEANIMSYS_s *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end);

// ==== src/legoapi/ai/core/ai_sys_stubs.cpp: all remaining content ====
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

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
extern char * AiLevelPathName;
extern AISCRIPTPROCESS * pSetStateDebugee;
extern SCRIPTPROCESSFIRSTTIMEACTION * ScriptProcessFirstTimeActionFn;
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

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

void AIScriptForceParamReEval(AISCRIPTPROCESS *processor);

char *AIScriptNameFromIx(AISYS *system, i32 index);

} // extern "C"

#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, without the unused object and elapsed time.

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

void AISysProcess(AISYS *system, APIOBJECT *player_1, APIOBJECT *player_2);

void AISysProcessCharacter(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed,
                               i32 use_three_dimensions, i32 process_ai);

i32 AISysSetLevelPath(AISYS *system, char *path_name);

void AISysUpdateCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed);

extern AIEDITOR_RENDER_STATE * aieditor;
    void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);

void AiSysOnlyUsePakFile(i32 enabled);

void AiSysSetStateDebugee(AISCRIPTPROCESS *processor);

void AiSysUsePackFile(i32 enabled);

void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out);

void DestroyAIGroup(AIGROUP *group);

void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result);

void FollowAPIObject(APIOBJECT *object, APIOBJECT *target, i32 flags, f32 movement_parameter);

extern AIEDITORPATHNODECALLBACK * AIPathNodeDeletedFn;

void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);

void InitFn_GameParamToFloat(GAMEPARAMTOFLOAT *function);

void InitFn_ScriptProcessFirstTimeAction(SCRIPTPROCESSFIRSTTIMEACTION *function);

void LEGO_AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                         f32 delta_time);

} // extern "C"

// ==== src/nu2api/nucore/nugcutscene.cpp: all remaining content ====
#include "nu2api/nucore/nugcutscene.h"

// Callback storage is declared in reverse linked-BSS order because the
// original GCC 4.7 -O3 object emits these definitions in reverse source order.
extern "C" {
extern NUGCUTSCENEGETHGOBJFN NuCutSceneGetHGObj;
extern NUGCUTSCENECHARACTERPROCESSFN NuCutSceneCharacterProcess;
extern NUGCUTSCENECHARACTERRENDERFN NuCutSceneCharacterRender;
extern NUGCUTSCENECHARACTERRELEASEFN NuCutSceneCharacterRelease;
extern NUGCUTSCENECHARACTEREVALFN NuCutSceneCharacterEval;
extern NUGCUTSCENEFINDCHARACTERSFN NuCutSceneFindCharacters;
extern void (*NuCutSceneDestroyCharacters)(NUGCUTSCENE_s *);
extern NUGCUTSCENERESETCHARACTERSFN NuCutSceneResetCharactersFn;
extern NUGCUTSCENECHARACTERCREATEDATAFN NuCutSceneCharacterCreateData;
extern NUGCUTSCENECHARACTERDESTROYDATAFN NuCutSceneCharacterDestroyData;
extern NUGCUTSCENERIGIDCOLLISIONCHECKFN NuCutSceneRigidCollisionCheck;
extern NUGCUTSCENERIGIDPOSTRENDERFN NuCutSceneRigidPostRender;
extern NUGCUTSCENESFXFIXUPFN NuCutSceneSFXFixUp;
extern NUGCUTSCENESFXUPDATEFN NuCutSceneSFXUpdate;
extern NUGCUTSCENEREQUESTSFXFN NuCutSceneRequestSFX;

void NuSetGetHGObjFromIndxFn(NUGCUTSCENEGETHGOBJFN function);

void NuSetCutSceneCharacterProcessFn(NUGCUTSCENECHARACTERPROCESSFN function);

void NuSetCutSceneCharacterRenderFn(NUGCUTSCENECHARACTERRENDERFN function);

void NuSetCutSceneCharacterEvalFn(NUGCUTSCENECHARACTEREVALFN function);

void NuSetCutSceneCharacterReleaseFn(NUGCUTSCENECHARACTERRELEASEFN function);

void NuSetCutSceneFindCharactersFn(NUGCUTSCENEFINDCHARACTERSFN function);

void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *));

void NuSetCutSceneResetCharactersFn(NUGCUTSCENERESETCHARACTERSFN function);

void NuSetCutSceneCharacterCreateDataFn(NUGCUTSCENECHARACTERCREATEDATAFN function);

void NuSetCutSceneCharacterDestroyDataFn(NUGCUTSCENECHARACTERDESTROYDATAFN function);

void NuSetCutSceneRigidCollisionCheckFn(NUGCUTSCENERIGIDCOLLISIONCHECKFN callback);

void NuSetCutSceneSFXFixUpFn(NUGCUTSCENESFXFIXUPFN function);

void NuSetCutSceneSFXUpdateFn(NUGCUTSCENESFXUPDATEFN function);

void NuSetCutSceneRequestSFXFn(NUGCUTSCENEREQUESTSFXFN function);

void NuSetCutSceneRigidPostRenderFn(NUGCUTSCENERIGIDPOSTRENDERFN function);
} // extern "C"

// ==== src/legoapi/cutscenes/cutscenes.cpp: declarations ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/audio/sfx.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/core/screen.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>
#include <stdio.h>
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

extern "C" {
    extern i16 id_ANAKINSNEWPOD;
    extern i16 id_ANAKINSNEWPODGREEN;
    extern i16 id_REPUBLICGUNSHIP_GREEN;
    extern i16 id_NEW_REPUBLIC_GUNSHIP_GREEN;
    extern i16 id_JEDISTARFIGHTERYELLOWEP3;
}

extern "C" {
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
}

// Retail only keeps the split-out body of the IPA-SRA clone (the unused
// cutscene index dropped), called with world and cut in registers.
#if defined(__i386__)
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone))
#endif

// The loop re-reads the active cut through CutStopInfo after each SmartTextEx call.
#define CUT_STOP static_cast<CUTINFO *>(CutStopInfo)

// ==== functions in original address order ====

extern "C" {

extern "C" void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *locator_functions);

}

extern "C" {
NUGCUTSCENE_s *NuGCutSceneLoad(char *name, VARIPTR *buf, VARIPTR *buf_end, i32 flags);

NUGCUTSCENE_s *NuGCutSceneLoadAddr(NUGCUTSCENE_s *cutscene, i32 loaded_size, VARIPTR *buffer);

void NuGCutSceneDestroy(NUGCUTSCENE_s *cutscene);

void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene, NUGSCN *scene, i32 flags, i32 area);

void NuGCutSceneFixUpExtra(NUGCUTSCENE_s *cutscene, NUGSCN *area);

void PetesHackOfDeath(void);

instNUGCUTSCENE_s *instNuGCutSceneCreate(NUGCUTSCENE_s *cutscene, NUGSCN *scene, void *extra, char *name,
                                             VARIPTR *buf, i32 flags);

}

extern "C" {

void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix);

void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf);

i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index);

}

extern "C" {
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *locator, float frame, NUMTX *mtx, nuanimtime_s *time);

}

extern "C" {
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *locator, float frame, nuanimtime_s *time, float *scale,
                                     float *rate);

}

extern "C" {
void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out);

}

extern "C" {
extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

}

extern "C" {
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

}

extern "C" {

void NuSetGetHGObjFromIndxFn(NUGCUTSCENEGETHGOBJFN function);

void NuSetCutSceneCharacterProcessFn(NUGCUTSCENECHARACTERPROCESSFN function);

void NuSetCutSceneCharacterRenderFn(NUGCUTSCENECHARACTERRENDERFN function);

void NuSetCutSceneCharacterEvalFn(NUGCUTSCENECHARACTEREVALFN function);

void NuSetCutSceneCharacterReleaseFn(NUGCUTSCENECHARACTERRELEASEFN function);

void NuSetCutSceneFindCharactersFn(NUGCUTSCENEFINDCHARACTERSFN function);

void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *));

void NuSetCutSceneResetCharactersFn(NUGCUTSCENERESETCHARACTERSFN function);

void NuSetCutSceneCharacterCreateDataFn(NUGCUTSCENECHARACTERCREATEDATAFN function);

void NuSetCutSceneCharacterDestroyDataFn(NUGCUTSCENECHARACTERDESTROYDATAFN function);

void NuSetCutSceneRigidCollisionCheckFn(NUGCUTSCENERIGIDCOLLISIONCHECKFN callback);

void NuSetCutSceneSFXFixUpFn(NUGCUTSCENESFXFIXUPFN function);

void NuSetCutSceneSFXUpdateFn(NUGCUTSCENESFXUPDATEFN function);

void NuSetCutSceneRequestSFXFn(NUGCUTSCENEREQUESTSFXFN function);

void NuSetCutSceneRigidPostRenderFn(NUGCUTSCENERIGIDPOSTRENDERFN function);

}

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

#include "legoapi/items/objects/gameobjects_types.h"

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

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

extern "C" {
extern ed_module_s edTimingDesc;
}

// ==== functions in original address order ====

static void Credits_Update_Game(WORLDINFO *) {
    i32 finished;
    Credits_GetInfo(NULL, &finished, NULL);
    LevTime[1] = finished == 0 ? 1.0f : 0.0f;
    LevTime[0] = SeekLinearF(LevTime[0], LevTime[1], FRAMETIME + FRAMETIME);
}

static void Titles_Draw(WORLDINFO *) {
    NUMTX draw_mtx;
    if (NuSpecialExistsFn(&LevHSpecial[0]) != 0 && LevAlpha > 0.0f) {
        draw_mtx = LevMtx;
        if (GameTimer.time_elapsed < 4.0f) {
            f32 t = NuTrigTable[(i32)(GameTimer.time_elapsed * 0.25f * 16384.0f) >> 1 & 0x7fff];
            draw_mtx.m30 = titlesstartpos.x + (LevMtx.m30 - titlesstartpos.x) * t;
            draw_mtx.m31 = titlesstartpos.y + (LevMtx.m31 - titlesstartpos.y) * t;
            draw_mtx.m32 = titlesstartpos.z + (LevMtx.m32 - titlesstartpos.z) * t;
        }

        NUVEC scale = {0.825f, 0.825f, 0.825f};
        NuMtxPreScale(&draw_mtx, &scale);
        f32 alpha = 1.0f - NuTrigTable[(i32)(LevAlpha * 16384.0f + 16384.0f) >> 1 & 0x7fff];
        NuSpecialDrawAtAlpha(&LevHSpecial[0], &draw_mtx, newgamealpha * TitlesAlpha * alpha);
    }

    if (!(LevTime[0] > 0.0f) || !(newgamealpha > 0.0f)) {
        return;
    }

    f32 time = GameTimer.time_elapsed;
    if (time > 30.0f) {
        time = NuFmod(time - 4.0f, 26.0f) + 4.0f;
    }
    draw_mtx = LevMtx;

    f32 alpha;
    if (time > 4.0f) {
        if (time < 5.0f)
            alpha = time - 4.0f;
        else if (time < 8.166666f)
            alpha = 1.0f;
        else if (time < 9.166666f)
            alpha = 1.0f - (time - 8.166666f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[10]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[10], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
    if (time > 8.166666f) {
        if (time < 9.166666f)
            alpha = time - 8.166666f;
        else if (time < 12.333332f)
            alpha = 1.0f;
        else if (time < 13.333332f)
            alpha = 1.0f - (time - 12.333332f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[11]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[11], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
    if (time > 12.333333f) {
        if (time < 13.333333f)
            alpha = time - 12.333333f;
        else if (time < 16.5f)
            alpha = 1.0f;
        else if (time < 17.5f)
            alpha = 1.0f - (time - 16.5f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[12]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[12], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
    if (time > 16.5f) {
        if (time < 17.5f)
            alpha = time - 16.5f;
        else if (time < 20.666666f)
            alpha = 1.0f;
        else if (time < 21.666666f)
            alpha = 1.0f - (time - 20.666666f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[13]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[13], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
    if (time > 20.666666f) {
        if (time < 21.666666f)
            alpha = time - 20.666666f;
        else if (time < 24.833332f)
            alpha = 1.0f;
        else if (time < 25.833332f)
            alpha = 1.0f - (time - 24.833332f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[14]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[14], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
    if (time > 24.833332f) {
        if (time < 25.833332f)
            alpha = time - 24.833332f;
        else if (time < 28.999998f)
            alpha = 1.0f;
        else if (time < 29.999998f)
            alpha = 1.0f - (time - 28.999998f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[15]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[15], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
    if (time > 29.0f) {
        if (time < 30.0f)
            alpha = time - 29.0f;
        else if (time < 33.166668f)
            alpha = 1.0f;
        else if (time < 34.166668f)
            alpha = 1.0f - (time - 33.166668f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[16]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[16], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
    if (time > 33.166664f) {
        if (time < 34.166664f)
            alpha = time - 33.166664f;
        else if (time < 37.333332f)
            alpha = 1.0f;
        else if (time < 38.333332f)
            alpha = 1.0f - (time - 37.333332f);
        else
            alpha = 0.0f;
        if (alpha > 0.0f && NuSpecialExistsFn(&LevHSpecial[17]) != 0) {
            draw_mtx.m31 = -0.15f;
            NuSpecialDrawAtAlpha(&LevHSpecial[17], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
        }
    }
}

static void Titles_Update(WORLDINFO *) {
    if (NuSpecialExistsFn(&LevHSpecial[0]) != 0) {
        i32 menu_id = GetMenuID();
        f32 target =
            (MenuLoadOccurred == 0 && MenuSaveOccurred == 0 && (static_cast<u32>(menu_id + 1) < 2 || menu_id == 1))
                ? 1.0f
                : 0.0f;
        LevAlpha = SeekLinearF(LevAlpha, target, FRAMETIME + FRAMETIME);
    }

    TitlesAlpha = SeekLinearF(TitlesAlpha, 1.0f, FRAMETIME + FRAMETIME);
    LevTime[1] = 0.0f;
    if (GetMenuID() == 0 && !(GameTimer.time_elapsed < 4.0f)) {
        LevTime[1] = 1.0f;
    }
    LevTime[0] = SeekLinearF(LevTime[0], LevTime[1], FRAMETIME + FRAMETIME);
}

static void Credits_Draw_Game(WORLDINFO *) {
    if (LevTime[0] > 0.0f) {
        const f32 time = CreditsTime;
        f32 alpha;
        f32 duration;
        Credits_GetInfo(&duration, NULL, &alpha);
        alpha *= 0.5f * LevTime[0];
        duration -= 1.0f;
        const f32 interval = (duration - 1.0f) / Pictures_NumLevels;
        const f32 picture_duration = interval + 1.0f;

        for (i32 i = 0; i < Pictures_NumLevels; ++i) {
            const f32 start = interval * i + 1.0f;
            if (time > start) {
                const f32 end = start + picture_duration;
                f32 fade;
                if (time < start + 1.0f) {
                    fade = time - start;
                } else if (time < start + (picture_duration - 1.0f)) {
                    fade = 1.0f;
                } else if (time < end) {
                    fade = 1.0f - (time - (start + (picture_duration - 1.0f)));
                } else {
                    fade = 0.0f;
                }

                if (fade > 0.0f && NuSpecialExistsFn(&LevHSpecial[20 + i]) != 0) {
                    const f32 progress = (time - start) / (end - start);
                    NUMTX matrix = LevMtx;
                    NUVEC scale;
                    scale.x = scale.y = scale.z = progress * 0.2f + 0.9f;
                    NuMtxPreScale(&matrix, &scale);
                    NuMtxPreRotateZ(&matrix, static_cast<i32>(546.0f - 1092.0f * progress));
                    matrix.m31 = 0.15f * progress - 0.075f;
                    NuSpecialDrawAtAlpha(&LevHSpecial[20 + i], &matrix, fade * alpha);
                }
            }
        }
    }
}

static void Pictures_FixUp(WORLDINFO *world) {
    if (world->scene != NULL) {
        Pictures_NumLevels = 0;
        for (i32 episode = 0; episode < EPISODECOUNT; episode++) {
            char name[64];
            sprintf(name, "EP_%i", episode + 1);
            NuSpecialFind(world->scene, &LevHSpecial[10 + episode], name, 1);

            for (i32 chapter = 0; chapter < 8; chapter++) {
                sprintf(name, "EP_%i_CH_%i", episode + 1, chapter + 1);
                NuSpecialFind(world->scene, &LevHSpecial[20 + Pictures_NumLevels], name, 1);
                Pictures_NumLevels++;
            }
        }

        static const char *bonus_names[] = {
            "pod_race", "anakin_flight", "gunship", "new_hope", "lego_city", "new_town",
        };
        for (i32 i = 0; i < 6; i++) {
            NuSpecialFind(world->scene, &LevHSpecial[20 + Pictures_NumLevels], const_cast<char *>(bonus_names[i]), 1);
            Pictures_NumLevels++;
        }
    }
    LevTime[0] = 0.0f;
    LevTime[1] = 0.0f;
}

static void Credits_Init_Game(WORLDINFO *world) {
    static const char *const credit_music[] = {
        "Ep1_EndCredits", "Ep2_EndCredits", "Ep3_EndCredits", "Ep4_EndCredits", "Ep5_EndCredits", "Ep6_EndCredits",
    };
    i32 episode = 0;
    if (LastAData != NULL) {
        const i32 last_episode = static_cast<i8>(LastAData->episode_index);
        if (static_cast<u32>(last_episode) <= 5) {
            episode = last_episode;
        }
    }
    const char *music_name = credit_music[episode];
    world->current_level->music_index = GetMusicIndex(const_cast<char *>(music_name), MusicInfo, -1);
    world->current_level->music_tracks[0][0] = world->current_level->music_tracks[0][1] =
        music_man.GetTrackHandle(TRACK_CLASS_QUIET, music_name);
    world->current_level->music_tracks[1][0] = world->current_level->music_tracks[1][1] =
        music_man.GetTrackHandle(TRACK_CLASS_ACTION, music_name);
    world->current_level->music_tracks[2][0] = world->current_level->music_tracks[2][1] =
        music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, music_name);
    Credits_Init(world);
    Pictures_FixUp(world);
    NUVEC scale;
    scale.x = scale.y = scale.z = 2.5f;
    NuMtxSetScale(&LevMtx, &scale);
    LevMtx.m31 = 0.0f;
    LevMtx.m32 = 1.0f;
}

static void Titles_Init(WORLDINFO *world) {
    char title_name[64];

    NewGame();
    switch (Text_Language) {
        case 2:
            NuStrCpy(title_name, "titles_french");
            break;
        case 3:
            NuStrCpy(title_name, "titles_spanish");
            break;
        case 4:
            NuStrCpy(title_name, "titles_german");
            break;
        case 5:
            NuStrCpy(title_name, "titles_italian");
            break;
        case 8:
            NuStrCpy(title_name, "titles_danish");
            break;
        default:
            if (Text_Language == 0x12) {
                NuStrCpy(title_name, "titles_us");
            } else {
                NuStrCpy(title_name, "titles_uk");
            }
            break;
    }

    NuSpecialFind(world->current_gscn, &LevHSpecial[0], title_name, 1);
    LevAlpha = 1.0f;
    nuhspecial_s *title_special = &LevHSpecial[0];
    if (NuSpecialExistsFn(title_special) != 0) {
        NUMTX *draw_mtx = NuSpecialGetDrawMtx(title_special);
        LevMtx = *draw_mtx;
        NuSpecialSetVisibility(title_special, 0);
        titlesstartpos.x = LevMtx.m30;
        titlesstartpos.y = 0.0f;
        titlesstartpos.z = LevMtx.m32 - 0.5f;
    }

    TitlesAlpha = 1.0f;
    if (GAMEDEMO != 0) {
        GAMEDEMO = 1;
    } else {
        PlayerID[0] = id_DEFAULTCHARACTER[0];
        PlayerID[1] = id_DEFAULTCHARACTER[1];
    }

    Door_Reset();
    newgamealpha = 1.0f;
    if (GAMEDEMO == 0) {
        PlayerProgress[0].active = 0;
        PlayerProgress[1].active = 0;
        newgamefade = 0;
        newgame_menudrawoff = 0;
    }
    BackDrop_ResetColours();
    Pictures_FixUp(world);
}

void MakeFreePlayModelList(i32 model1, i32 model2, i32 area, i32 level, i32 param5) {
    i16 ids[341];
    i16 imperial_ids[500];
    u32 flags = 0;
    if (WORLD != NULL && WORLD->area != NULL && WORLD->area == HUB_ADATA && bonusmodearcade != 0)
        flags = Arcade_Mode[static_cast<i8>(ArcadeItem.field_c_0xc)].field8_0x8;

    FreePlayModelCount = 0;
    FreePlayResidentCount = 0;
    FreePlayBonusCount = 0;
    // Empty input must not leave a previous roster visible to category scans.
    FreePlayModelList[0].model_id = -1;
    if (model1 == -1)
        model2 = -1;
    const i32 models[2] = {model1, model2};
    // Area -1 has no resident/vehicle data; never form ADataList[-1].
    AREADATA *data = area == -1 ? NULL : &ADataList[area];
    const u16 area_flags = data == NULL ? 0 : data->flags;
    if ((area_flags & 5) == 5) {
        for (i32 i = 0; i < 2; ++i)
            World_AddFreePlayModel(models[i]);
        Collection_GetIDList(&MiniKitCollection, 0x4000000, 0x4000000, ids, NULL, NULL, makefreeplaymodellist != 0);
        for (i16 *id = ids; *id != -1; ++id)
            World_AddFreePlayModel(*id);
        if (FreePlayModelCount == 1)
            PlayerList[1] = PlayerList[0];
        PlayerList[2] = -1;
    } else {
        for (i32 i = 0; i < 2; ++i) {
            i32 model = models[i];
            if (model == -1 && Player[i] != NULL)
                model = Player[i]->id;
            World_AddFreePlayModel(model);
        }
        if (FreePlayModelCount == 1) {
            PlayerList[1] = PlayerList[0];
            PlayerList[2] = -1;
        } else {
            PlayerList[FreePlayModelCount] = -1;
        }
    }

    if (data != NULL && (flags & 0x10) == 0 && data->hub_player_ids != NULL) {
        for (i16 *id = data->hub_player_ids; *id != -1; ++id) {
            World_AddFreePlayModel(*id);
            // Retail counts resident entries, including a duplicate or one
            // beyond the roster cap, rather than only successful insertions.
            ++FreePlayResidentCount;
        }
    }
    if (data != NULL && (data->flags & 1) != 0) {
        if ((data->flags & 4) == 0) {
            const i32 second = PlayerList[1];
            if (second == -1 || (CDataList[second].model_flags & 0x2000) == 0 || Collection_Got(second) == 0)
                PlayerList[1] = PlayerList[0];
            const i32 count = VehicleCollection.count_y < 340 ? VehicleCollection.count_y : 340;
            for (i32 i = 0; i < count; ++i)
                ids[i] = VehicleCollection.list[i].id;
            ids[count] = -1;
            for (i16 *id = ids; *id != -1; ++id) {
                if (World_AddFreePlayModel(*id) != 0)
                    ++FreePlayBonusCount;
            }
        } else {
            const i32 second = PlayerList[1];
            if (second == -1 || (CDataList[second].model_flags & 0x4000000) == 0 || Collection_Got(second) == 0)
                PlayerList[1] = PlayerList[0];
        }
    } else {
        i32 hats = InModelListDataFlags(FreePlayModelList, 0, 0, 1, 0);
        if (CharCategory != NULL) {
            for (i32 i = 0; i < CHARCATEGORYCOUNT; ++i) {
                if (InModelListDataFlags(FreePlayModelList, CharCategory[i].model_flags, CharCategory[i].game_flags, 0,
                                         1) != 0)
                    continue;
                i32 model = -1;
                if (hats == 0 || (CharCategory[i].model_flags & 0x80) != 0) {
                    model = RandomIDFromFlags(CharCategory[i].model_flags, CharCategory[i].game_flags, 1, NULL, 1);
                    if (model != -1) {
                        hats = 1;
                    }
                }
                if (model == -1) {
                    model = RandomIDFromFlags(CharCategory[i].model_flags, CharCategory[i].game_flags, 0, NULL, 1);
                }
                if (World_AddFreePlayModel(model) != 0)
                    ++FreePlayBonusCount;
            }
            bool imperial = false;
            for (APICHARACTERMODELLIST_s *model = FreePlayModelList; model->model_id != -1; ++model) {
                if (!(Collection_Got(model->model_id) != 0 && World_FreePlayImperial(model->model_id))) {
                    continue;
                }
                imperial = true;
                break;
            }
            if (!imperial) {
                i32 count = 0;
                for (i32 i = 0; i < CHARCOUNT; ++i) {
                    if (InModelList(FreePlayModelList, i, NULL) == 0 &&
                        (Game_Customiser == NULL ||
                         (i != Game_Customiser->character_ids[0] && i != Game_Customiser->character_ids[1])) &&
                        CDataList[i].move_fn != Move_DEFAULT && Collection_Got(i) != 0 && World_FreePlayImperial(i) &&
                        count < 500)
                        imperial_ids[count++] = static_cast<i16>(i);
                }
                if (count != 0) {
                    if (count != 1)
                        imperial_ids[0] = imperial_ids[qrand() / (0xffff / count + 1)];
                    if (World_AddFreePlayModel(imperial_ids[0]) != 0)
                        ++FreePlayBonusCount;
                }
            }
        }
    }
    for (EXTRAMODEL *extra = ExtraModelList; extra->model_list != NULL; ++extra) {
        const i32 source = *extra->model_list;
        bool found = false;
        if (source != -1) {
            for (i32 i = 0; i < FreePlayModelCount; ++i) {
                if (FreePlayModelList[i].model_id == source) {
                    found = true;
                    break;
                }
            }
        }
        if (found && extra->field_04 != NULL)
            World_AddFreePlayModel(*static_cast<i16 *>(extra->field_04));
    }
}

static __used__ void LoadAreaData(bgprocinfo_s *) {
    char pathbuf[0x100];
    APICHARACTERMODELLIST_s ml[3];
    i32 level;
    i32 saved_max_fps;
    i32 area;
    NUGSCN *scene;
    APICHARACTERMODELLIST_s *story_list;
    APICHARACTERMODELLIST_s *list;

    area = Area;
    saved_max_fps = nuapi.max_fps;
    nuapi.max_fps = 30;
    level = Level;

    if (area != -1 && area != last_area) {
        characterbuffer_ptr.addr = ALIGN(characterbuffer_ptr.addr, 4);
        NuStrCpy(pathbuf, "levels\\");
        NuStrCat(pathbuf, ADataList[area].dir);
        NuStrCat(pathbuf, "\\");
        NuStrCat(pathbuf, ADataList[area].file);
        NuStrCat(pathbuf, ".gsc");
        scene = NuGScnRead(&characterbuffer_ptr, characterbuffer_end, pathbuf);
        area_scene = scene;
        if (scene != NULL && scene->display_list != NULL)
            scene->display_list->flags |= NU_DISPLAYSCENE_FLAG_NEEDS_BUILD;

        if ((ADataList[area].flags & (AREAFLAG_VEHICLE_AREA | AREAFLAG_BONUS_AREA)) != 0 && area != last_area) {
            characterbuffer_ptr.addr = ALIGN(characterbuffer_ptr.addr, 4);
            scene = NuGScnRead(&characterbuffer_ptr, characterbuffer_end, "stuff\\vehicle_things.gsc");
            vehicle_scene = scene;
            if (scene != NULL && scene->display_list != NULL)
                scene->display_list->flags |= NU_DISPLAYSCENE_FLAG_NEEDS_BUILD;
        }
    }

    if (HUB_ADATA != NULL && HUB_ADATA->index == area) {
        if (area != last_area) {
            characterbuffer_ptr.addr = ALIGN(characterbuffer_ptr.addr, 0x40);
            scene = NuGScnRead(&characterbuffer_ptr, characterbuffer_end, "stuff\\icons\\starwars_icons_all.gsc");
            big_icon_scene = scene;
            if (scene != NULL && scene->display_list != NULL)
                scene->display_list->flags |= NU_DISPLAYSCENE_FLAG_NEEDS_BUILD;
        }
    }

    if (area != -1 && area != last_area) {
        ml[0].count = 1;
        ml[1].model_id = -1;
        ml[1].count = 1;
        ml[2].model_id = -1;
        ml[2].count = 0;
        ml[0].model_id = id_ANAKINJEDISCARRED;
        if ((ADataList[area].flags & AREAFLAG_HUB_AREA) != 0)
            ml[1].model_id = id_HANINCARBONITE;
        IconScenes_Load(ml, 1, &characterbuffer_ptr, &characterbuffer_end);
    }

    if (makefreeplaymodellist != 0) {
        if ((ADataList[area].flags & AREAFLAG_VEHICLE_AREA) == 0 && !(Area_PlayerModelCount > 1 && BonusArea == 0)) {
            MakeFreePlayModelList(id_DEFAULTCHARACTER[0], id_DEFAULTCHARACTER[1], area, level, 0);
        } else {
            MakeFreePlayModelList(Area_PlayerModelList[0], Area_PlayerModelList[2], area, level, 0);
        }
        story_list = Area_StoryModelList;
        makefreeplaymodellist = 0;
        list = FreePlayModelList;
        goto icon_scenes;
    } else {
        if (area != -1 && area == last_area) {
            CharacterDataLoad = 2;
            goto loadareadata_check;
        }
    }
model_select:
    if (HUB_ADATA != NULL && HUB_ADATA->index == area) {
        list = Hub_ModelList;
        story_list = Hub_ModelList;
    } else {
        list = FreePlayModelList;
        if (FreePlay == 0) {
            if (Mission_Active(0) != 0)
                list = Area_MissionModelList;
            else
                list = Area_StoryModelList;
        }
        if (HUB_ADATA != NULL && HUB_ADATA->index == area) {
            story_list = Hub_ModelList;
        } else {
            if (Mission_Active(0) != 0)
                story_list = Area_MissionModelList;
            else
                story_list = Area_StoryModelList;
        }
    }

icon_scenes:
    CurrentCList = list;
    IconScenes_Load(list, 1, &characterbuffer_ptr, &characterbuffer_end);

    if (HUB_ADATA != NULL && HUB_ADATA->index == area)
        Customiser_SetAnimsToLoad(CharacterCustomiser, 1);
    else
        Customiser_SetAnimsToLoad(CharacterCustomiser, 0);

    if (area != -1 && (ADataList[area].flags & (AREAFLAG_ENDING_AREA | AREAFLAG_TEST_AREA | AREAFLAG_HUB_AREA)) == 0) {
        if (ADataList[area].episode_index <= 2 || (ANEWHOPE_ADATA != NULL && ANEWHOPE_ADATA->index == area) ||
            (PODSPRINT_ADATA != NULL && PODSPRINT_ADATA->index == area) ||
            (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA->index == area)) {
            Particles_LoadAreaPage("stuff\\char_lsw1.ptl");
        }
    }

    apiloadcharactermodels_nopakfile = (CHARPAK == 0);
    APIResetCharacterRemap();
    CharacterDataLoad = 1;
    GameLoadCharacterModels(list, 0, &characterbuffer_ptr, &characterbuffer_end, 1, area);
    CharacterDataLoad = 2;
    CharScenes_AreaLoad(list, &characterbuffer_ptr, characterbuffer_end);
    if (!(HUB_ADATA != NULL && HUB_ADATA->index == area))
        Customiser_LoadAccessories(CharacterCustomiser, list);
    Customiser_ResetModelTextureIDs(CharacterCustomiser);
    CurrentStoryCList = story_list;
loadareadata_check:
    if (loadareadata_loadlevel != 0) {
        next_level = Level;
        abort_load = 0;
        WorldInfo_StreamLevel(0);
    }
    AreaDataLoaded = 1;
    nuapi.max_fps = saved_max_fps;
}

i32 KillBoss(i32 character_id, i32 flag_index, float delay) {
    u8 *flags = reinterpret_cast<u8 *>(LevFlag);
    switch (flags[flag_index]) {
    case 0:
        if (BossKilled(character_id) != NULL) {
            if (delay <= 0.0f) {
                flags[flag_index] = 2;
                return 1;
            }
            flags[flag_index] = 1;
        }
        break;
    case 1:
        LevTime[0] += FRAMETIME;
        if (LevTime[0] >= delay) {
            flags[flag_index] = 2;
            return 1;
        }
        break;
    }
    return 0;
}

void KillBossCompleteLevel(i32 a, i32 b, float c) {
    if (KillBoss(a, b, c) != 0) {
        CompleteLevel(WORLD);
    }
}

void KillBossNewLevel(i32 a, i32 b, float c, i32 d) {
    if (KillBoss(a, b, c) != 0) {
        GoToNewLevel(d);
    }
}

i32 KillBossPlayCutScene(i32 a, i32 b, float c, char *name) {
    if (KillBoss(a, b, c) != 0) {
        NewCutScene(NULL, (CUTSYS *)WORLD->cutscene_sys, name, 1);
        return 1;
    }
    return 0;
}

i32 EffectOffProgress_Update(LEVEL_PROGRESS_s *progress, char *name, i32 visible) {
    if (name == NULL || progress == NULL || NuStrLen(name) > 15)
        return 0;
    for (i32 i = 0; i < 12; ++i) {
        if (NuStrICmp(progress->disabled_effect_names[i], name) != 0) {
            continue;
        }
        if (visible != 0) {
            progress->disabled_effect_names[i][0] = '\0';
            return 2;
        }
        return 3;
    }
    if (visible != 0)
        return 0;
    for (i32 i = 0; i < 12; ++i) {
        if (progress->disabled_effect_names[i][0] != '\0') {
            continue;
        }
        NuStrCpy(progress->disabled_effect_names[i], name);
        break;
    }
    return 1;
}

void EffectOffProgress_Reset(LEVEL_PROGRESS_s *progress) {
    if (progress == NULL)
        return;
    for (i32 i = 0; i < 12; ++i) {
        if (progress->disabled_effect_names[i][0] != '\0')
            SetEffectVisibility(progress->disabled_effect_names[i], 0);
    }
}

void LevelProgress_ReserveBufferSpace(variptr_u *buf, variptr_u) {
    LevelProgressData = (void *)ALIGN(buf->addr, 0x10);
    buf->addr = (usize)LevelProgressData + 0x229b0;
}

void ClearLevelProgress(i32 index, WORLDINFO *world) {
    if (index >= 0) {
        i32 offset = index * sizeof(LEVEL_PROGRESS_s);
        memset((u8 *)LevelProgressData + offset, 0, sizeof(LEVEL_PROGRESS_s));
        u8 *entry = (u8 *)LevelProgressData + offset;
        *(u32 *)(entry + 0x281c) = 0;
        *(u32 *)(entry + 0x2810) = 0x49f42400;
        if (world != NULL) {
            *reinterpret_cast<LEVEL_PROGRESS_DATA_s *>(entry) = world->progress_data;
        }
    }
    GizmoSysClearLevelProgress(NULL, index);
    GameAnimSys_ClearProgress(index);
}

void ClearAreaProgress(i32 a, i32 b) {
    for (i32 i = 0; i < 12; i++) {
        ClearLevelProgress(i, NULL);
    }
    areaSuitBits = Game.initial_store_pack_flags;
    if (b == 0 || a == -1) {
        return;
    }
    AreaGlobals.values.field_0x14 = 0;
    Game.area_save[a] = BackupGame.area_save[a];

    AREADATA *area = &ADataList[a];
    for (i32 i = 0; i < area->level_count; i++) {
        i32 level_index = area->levels[i];
        Game.level_records[level_index] = BackupGame.level_records[level_index];
    }
}

void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process) {
    if (world->level_progress != NULL && process != NULL && NuStrLen(process->name) != 0) {
        for (i32 index = 0; index < 32; ++index) {
            if (NuStrLen(world->level_progress->scripts[index].name) == 0)
                break;
            if (NuStrICmp(world->level_progress->scripts[index].name, process->name) == 0) {
                for (i32 parameter = 0; parameter < 4; ++parameter)
                    process->processor.params[parameter] = world->level_progress->scripts[index].params[parameter];
                LOG_INFO("level-script progress restored name=%s params=[%g, %g, %g, %g]", process->name,
                         process->processor.params[0], process->processor.params[1], process->processor.params[2],
                         process->processor.params[3]);
                break;
            }
        }
    }
}

void NewArea() {
    i32 i;
    u8 area_ep;

    LSW1 = 0;
    LSW2 = 0;
    if (WORLD != NULL && WORLD->area != NULL) {
        i8 episode = static_cast<i8>(WORLD->area->episode_index);
        if (episode >= 0) {
            if (episode <= 2) {
                LSW1 = 1;
            } else {
                LSW2 = 1;
            }
        }
    }
    HIGHJUMPHEIGHT = (LSW1 != 0 || Arcade != 0) ? 1.14f : 0.75f;
    ClearAreaProgress(Area, 0);
    if (WORLD->level_progress != NULL && (WORLD->level_progress->flags & 1) == 0) {
        WORLD->level_progress->flags |= 1;
        WORLD->level_progress->data = WORLD->progress_data;
    }
    SuperCounters_Reset(Area);
    AreaGlobals.values.field_0x10 = 0;
    if (Area == -1) {
        area_ep = 0;
    } else {
        area_ep = Game.area_save[Area].minikit_count;
    }
    AreaGlobals.values.field_0x0c = area_ep;
    AreaGlobals.values.field_0x14 = area_ep;
    Door_UseCutCam = 0;
    AreaGlobals.values.field_0x00 = 0;
    AreaGlobals.values.field_0x1c = 0;
    AreaGlobals.values.field_0x08 = 0;
    AreaGlobals.values.field_0x18 = 0;
    AreaGlobals.values.arcade_player_kills[0] = 0;
    AreaGlobals.values.arcade_player_kills[1] = 0;
    AreaGlobals.values.arcade_ai_kills[0] = 0;
    AreaGlobals.values.arcade_ai_kills[1] = 0;
    BuildUpTotal = 0;
    BuildUpDone = 0;
    if (WORLD->area == HOTHBATTLE_ADATA) {
        ResetMinikitCounter();
    }
    for (i = 0; i < 8; i++) {
        PlayerProgress[i].hitpoints = DEFAULT_PLAYERHITPOINTS;
        if (Player[i] != NULL) {
            Player[i]->current_hp = Player[i]->hitpoints;
        }
    }
    ResetTimer(&AreaTimer, 0.0f);
    memcpy(&BackupGame, &Game, sizeof(GAMESAVE_s));
    NewAreaMusicChanges();
    VehicleAreaRememberSpeed = 0.0f;
    ClearTakeOverObjectSys();
    BonusScore[0] = OldBonusScore[0];
    BonusScore[1] = OldBonusScore[1];
    BonusCoinTotal = 0;
    Door_Last = NULL;
    Door_Reset();
    LevelChange = 1;
    BombGenerator_PlayerBomb[0] = 0;
    BombGenerator_PlayerBomb[1] = 0;
    Lap = 1;
}

void ClearLevData() {
    LevFlag[0] = 0;
    LevFlag[1] = 0;
    LevFlag[2] = 0;
    LevFlag[3] = 0;
    LevSfxFlag[0] = 0;
    LevSfxFlag[1] = 0;
    LevSfxFlag[2] = 0;
    LevSfxFlag[3] = 0;
    memset(LevHSpecial, 0, sizeof(LevHSpecial));
    memset(dynamic_antinodes, 0, 0x1500);
    memset(LevInstAnim, 0, sizeof(LevInstAnim));
    LevArea[0] = 0;
    LevArea[1] = 0;
    LevArea[2] = 0;
    LevArea[3] = 0;
    memset(LevPathNodes, 0, sizeof(LevPathNodes));
    memset(LevPathCnx, 0, sizeof(LevPathCnx));
    memset(LevGameObject, 0, sizeof(LevGameObject));
    memset(LevGamePart, 0, sizeof(LevGamePart));
    memset(LevAIMessage, 0, sizeof(LevAIMessage));
    LevelLocator = 0;
    LevBlowUp[0] = 0;
    LevBlowUp[1] = 0;
    LevBlowUp[2] = 0;
    LevBlowUp[3] = 0;
    LevBlowUp[4] = 0;
    memset(LevGizObst, 0, sizeof(LevGizObst));
    memset(LevGizmo, 0, sizeof(LevGizmo));
    LevSfxId[0] = 0;
    LevSfxId[1] = 0;
    LevSfxId[2] = 0;
    LevSfxId[3] = 0;
    memset(LevelCodeSpline, 0, sizeof(LevelCodeSpline));
    LevGizForce[0] = 0;
    LevGizForce[1] = 0;
    LevGizForce[2] = 0;
    LevGizForce[3] = 0;
    LevAIPathNode[0] = 0;
    LevAIPathNode[1] = 0;
    LevAIPathNode[2] = 0;
    LevAIPathNode[3] = 0;
    LevBoltIgnorePlatIds[0] = -1;
    LevBoltIgnorePlatIds[1] = -1;
    LevPlatID[0] = -1;
    LevPlatID[1] = -1;
    LevPathCnxDir = 0;
    LevDeaths = 0;
    LevLock[3] = 0;
    LevLock[2] = 0;
    LevLock[1] = 0;
    LevLock[0] = 0;
    LevSafePlatID[1] = -1;
    LevSafePlatID[0] = -1;
}

void LoadPerm1() {
    char buf[0x100];

    rtlInitDynamic(&permbuffer_ptr, superbuffer_end, 0x40);
    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    DebrisSetup(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\particle", 0x100, 0x200, 0x200);
    DebrisRegisterCutoffCameraVec(reinterpret_cast<NUVEC *>(&global_camera.mtx.m30));
    if (Grass_Available != 0) {
        edgraSetup(&permbuffer_ptr, permbuffer_end, 0x200, 0x20, 0x40);
    }
    lightningmat = NuMtlCreateEx(1, 10);
    lightningmat->diffuse_color.b = 1.0f;
    lightningmat->diffuse_color.g = 1.0f;
    lightningmat->diffuse_color.r = 1.0f;
    lightningmat->attribs.alpha_mode = 2;
    lightningmat->attribs.filter_mode = 1;
    lightningmat->attribs.unknown_1_1_2 = 1;
    lightningmat->attribs.unknown_1_4_8 = 1;
    lightningmat->attribs.cull_mode = 2;
    lightningmat->attribs.z_mode = 1;
    lightningmat->opacity = 0.999f;
    lightningmat->tex_id = DebMat[5]->tex_id;
    lightningmat->attribs.alpha_fail = 1;
    NuMtlUpdate(lightningmat);
    NuLgtSetArcMat(lightningmat, 0.0f, 0.50390625f, 0.25f, 0.5625f);
    NuWaterInit();
    NuBridgeInit();
    InitParts(0x40, &permbuffer_ptr, &permbuffer_end);
    ParticleReset();

    NuStrCpy(buf, (char *)"stuff\\general.ptl");
    if (NuFileExists(buf) != 0) {
        DEBPAGE_GENERAL = edppLoadPage(buf, 0, 0);
    }
    NuStrCpy(buf, (char *)"stuff\\char.ptl");
    if (NuFileExists(buf) != 0) {
        DEBPAGE_CHARACTER = edppLoadPage(buf, 5, 0);
    }
    perm_debrissys = InitGameDebris(&permbuffer_ptr, &permbuffer_end, 0x190, 0x93, debris_name, 0);

    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    InitStreaks(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\streak.pnt");
    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    InitRopeMtl((char *)"rope", &permbuffer_ptr, &permbuffer_end);
    InitRipples(&ripples, &permbuffer_ptr, &permbuffer_end, 0x40);
    permbuffer_ptr.addr = (permbuffer_ptr.addr + 0xff) & ~0xffu;

    ShadowMat = NuMtlCreate3D(1);
    ShadowMat->diffuse_color.r = 1.0f;
    ShadowMat->diffuse_color.g = 1.0f;
    ShadowMat->diffuse_color.b = 1.0f;
    ShadowMat->sort_pri = 0xff;
    ShadowMat->opacity = 0.999f;
    u8 *attributes = reinterpret_cast<u8 *>(&ShadowMat->attribs);
    attributes[1] = (attributes[1] & 0x0f) | 0x60;
    attributes[0] = (attributes[0] & 0xf0) | 1;
    attributes[2] = (attributes[2] & 0x8c) | 0x12;
    ShadowMat->tex_id = static_cast<i16>(NuTexRead((char *)"stuff\\gradient", &permbuffer_ptr, permbuffer_end));
    NuMtlUpdate(ShadowMat);

    u8 shadow_random[0x800];
    for (i32 i = 0; i < 0x800; ++i) {
        shadow_random[i] = static_cast<u8>(qrand() >> 8);
    }
    NuRndrShadowInit(shadow_random);
    CreateFadeMaterials();
    CreateUsefulMaterials();

    edgraClumpsReset();
    edanimParamReset();
    NuTexAnimProgSysInit();
    InitTexAnimScripts(TexAnimList_LSW);

    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    things_scene = NuGScnRead(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\things.gsc");
    NUGSCN *terrain_scene = NULL;
    if (things_scene != NULL) {
        edbitsRegisterThingsScene(things_scene);
        terrain_scene = things_scene;
        if (things_scene->display_list != NULL) {
            things_scene->display_list->flags |= NU_DISPLAYSCENE_FLAG_NEEDS_BUILD;
        }
    }

    things_scene_terrain = TerrainInitEx(-1, &permbuffer_ptr, permbuffer_end.void_ptr, 0, (char *)"stuff\\things",
                                         terrain_scene, 0, 0x14, 0x14, 0x14);
    terrainpickupinit((char *)"stuff\\things", &things_scene_terrain);
    BackDrop_Init((char *)"stuff\\starfield.gsc", &permbuffer_ptr, &permbuffer_end);

    NuMtlSetCurrentRenderPlane(0xf);
    edpartSetParticlePage(DEBPAGE_GENERAL);
    NuStrCpy(buf, (char *)"stuff\\general.par");
    if (NuFileExists(buf) != 0) {
        PARTPAGE_GENERAL = edpartLoadPage(buf, 0, things_scene);
    }
    NuMtlSetCurrentRenderPlane(0);
    AIScriptLoadAll((char *)"scripts", &permbuffer_ptr, &permbuffer_end, NULL);
}

void LoadPerm2() {
    extern i16 CustomiserActionList_Game[];
    extern i16 tALL;
    extern i16 tJEDI;
    extern i16 tBLASTER;
    extern i16 tBOUNTYHUNTERCHARACTERS;

    CharConfig_ConfigureAll(1, ConfigChar_GameKeywords);
    ExtraCharacterFixUpAfterConfig();
    apiloadcharactermodels_nopakfile = CHARPAK == 0;
    APILoadCharacterModels(PermModelList, 1, &permbuffer_ptr, permbuffer_end, 1);

    Collection_CreateMaster(const_cast<char *>("All"), NULL, &MasterCollection, 15, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("Characters"), &tALL, &CharacterCollection, 0, 0x04002000, 0, 0, 16,
                            &permbuffer_ptr, &permbuffer_end, 0, 0.725f);
    Collection_CreateCustom(const_cast<char *>("Vehicles"), NULL, &VehicleCollection, 0x2000, 0x04000000, 0, 0, 7,
                            &permbuffer_ptr, &permbuffer_end, 0, 1.0f);
    Collection_CreateCustom(const_cast<char *>("Minikits"), NULL, &MiniKitCollection, 0x04000000, 0, 0, 0, 12,
                            &permbuffer_ptr, &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("Shop"), NULL, &ShopCollection, 0, 0, 0, 1, 10, &permbuffer_ptr,
                            &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("Jedi"), &tJEDI, &JediCollection, 8, 0, 0, 0, 7, &permbuffer_ptr,
                            &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("BlasterZipUps"), &tBLASTER, &BlasterCollection, 0x100080, 0x01000000, 0,
                            0, 11, &permbuffer_ptr, &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("BountyHunters"), &tBOUNTYHUNTERCHARACTERS, &BountyHunterCollection,
                            0x01000000, 0, 0, 0, 4, &permbuffer_ptr, &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Areas_ConfigureResidents(&permbuffer_ptr, &permbuffer_end);
    CharacterCustomiser =
        Customiser_Configure(const_cast<char *>("chars\\customiser.txt"), &permbuffer_ptr, &permbuffer_end, id_WEIRDO1,
                             id_WEIRDO2, Customiser_PieceAvailable, Customiser_PieceConfig,
                             LevelObject_FindIndexFromName, Game.customizer, CustomiserActionList_Game);
    SpecialMoves_Configure(const_cast<char *>("chars\\specialmoves.txt"), &permbuffer_ptr, &permbuffer_end);

    // Every status-screen level plays the status screen music in all three track slots.
    for (i32 i = 0; i < LEVELCOUNT; ++i) {
        if ((LDataList[i].flags & LEVEL_STATUS) != 0) {
            LDataList[i].music_index = GetMusicIndex(const_cast<char *>("statusscreen"), MusicInfo, -1);
            LDataList[i].music_tracks[0][0] = LDataList[i].music_tracks[0][1] =
                music_man.GetTrackHandle(TRACK_CLASS_QUIET, "statusscreen");
            LDataList[i].music_tracks[1][0] = LDataList[i].music_tracks[1][1] =
                music_man.GetTrackHandle(TRACK_CLASS_ACTION, "statusscreen");
            LDataList[i].music_tracks[2][0] = LDataList[i].music_tracks[2][1] =
                music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, "statusscreen");
        }
    }
    Store_RestorePurchases();
}

void ClearUpAreaData() {
    i32 object_index;

    if (HIGHGAMEOBJECT > 0) {
        for (object_index = 0; object_index < HIGHGAMEOBJECT; object_index++) {
            GameObject_s *obj = &Obj[object_index];
            if ((obj->apiobj.field_0x1f8 & 1) != 0) {
                FreeTorpedoPacket((TORPEDOPACKET_s **)&obj->torpedo);
                RemoveGameObject(&Obj[object_index], 1);
            }
        }
    }
    if (Area != -1 && Area == last_area) {
        return;
    }
    if (Customiser_AccessoriesLoaded == 2) {
        Customiser_RestoreModelTextureIDs(CharacterCustomiser);
    } else if (Customiser_AccessoriesLoaded == 1) {
        Customiser_DumpAccessories(CharacterCustomiser);
    }
    Customiser_AccessoriesLoaded = 0;
    APIDumpCharacterModels(0);
    IconScenes_Dump();
    CharScenes_AreaDump();
    if (big_icon_scene != NULL) {
        NuGScnRemove(big_icon_scene);
    }
    big_icon_scene = NULL;
    if (area_scene != NULL) {
        NuGScnRemove(area_scene);
    }
    area_scene = NULL;
    if (vehicle_scene != NULL) {
        NuGScnRemove(vehicle_scene);
    }
    vehicle_scene = NULL;
    Particles_DumpAreaPage();
}

void LoadAreaCharacters() {
    if (Area == -1 || Area != last_area) {
        Area_Configure(Area, Level, ExtraModelList, id_DEFAULTCHARACTER);
    }

    if (Area == -1) {
        BonusArea = 0;
        VehicleArea = 0;
    } else {
        const u16 area_flags = ADataList[Area].flags;
        if ((area_flags & AREAFLAG_VEHICLE_AREA) == 0) {
            VehicleArea = 0;
        } else {
            VehicleArea = 1;
        }
        if ((area_flags & AREAFLAG_BONUS_AREA) == 0) {
            BonusArea = 0;
        } else {
            BonusArea = 1;
        }
    }
    AreaGlobals.values.field_0x04 = VehicleArea != 0 && BonusArea == 0;
    loadareacharacters_loadedlevel = 0;
    AreaDataLoaded = 0;
    loadareadata_loadlevel = 0;
    CharacterDataLoad = 0;
    ResetTimer(&GameTimer, 0.0f);

    i32 load_type;
    if (Area != -1 && Area != last_area && (ADataList[Area].flags & AREAFLAG_ENDING_AREA) == 0 &&
        new_level_from_menu == 0) {
        if (HUB_ADATA != NULL && last_area == HUB_ADATA->index) {
            load_type = 2;
            goto load_type_done;
        }
        if (netnewgame != 0) {
            load_type = 2;
            goto load_type_done;
        }
        if (GAMEDEMO != 0 && (LastLData == TITLES_LDATA || LastLData == STATUS_LDATA)) {
            load_type = 2;
            goto load_type_done;
        }
    }

    if (SuperStory != 0 && (ADataList[Area].flags & AREAFLAG_ENDING_AREA) != 0) {
        load_type = 1;
        goto load_type_done;
    } else if (last_area != -1 && LastLData != NULL && (LastLData->flags & 0x400) != 0 &&
               ADataList[Area].flags != AREAFLAG_ENDING_AREA && HUB_ADATA != NULL && Area != HUB_ADATA->index) {
        load_type = 2;
        goto load_type_done;
    }
    load_type = 0;

load_type_done:
    const i32 reload_same_area = Area != -1 && Area == last_area;
    netnewgame = 0;

    if (reload_same_area || LOADEROFF != 0 || BGLOAD == 0) {
        LoadAreaData(NULL);
        MainRenderTime = 1.0f;
        return;
    }

    textcrawlactive = 1;
    MechSystems::Get()->HookUpClickToPressStart();
    pNuCam->mtx = numtx_identity;
    NuCameraSet(pNuCam);

    LoadTime = 0.0f;
    LOADWAITTIME = load_type == 2 ? 1.0f : 0.1f;
    LoadWait = LOADWAITTIME;
    CutBorderScale = 1.0f;
    FRAMETIME = DEFAULTFRAMETIME;
    const f32 music_volume = GameSetMusicVolume(&Game.options_save);
    if (loadareacharacters_no_backdrop_reset == 0) {
        BackDrop_ResetColours();
    }
    TextCrawl_Init(&TextCrawl_LSW, Area, 1);
    i32 icon_stage = HUB_ADATA != NULL && HUB_ADATA->index == Area ? 0 : 2;
    loadareadata_loadlevel = 1;
    WorldInfo_ReArrangeBuffers(Area, last_area);
    SetBackgroundMusic(1);

    if (load_type == 2 && SuperOptions.music_enabled != 0 && NOSOUND == 0 && NOMUSIC == 0) {
        static const char *crawl_music[] = {
            "Ep1_TextCrawl", "Ep2_TextCrawl", "Ep3_TextCrawl", "Ep4_TextCrawl", "Ep5_TextCrawl", "Ep6_TextCrawl",
        };
        const i32 episode = Area == -1 ? -1 : static_cast<i8>(ADataList[Area].episode_index);
        if (!(Arcade == 0 && episode != -1)) {
            GamePlayMusic(TITLES_LDATA, 0, &Game.options_save);
        } else {
            const i32 handle = music_man.GetTrackHandle(TRACK_CLASS_QUIET, crawl_music[episode]);
            music_man.SelectTrackByHandle(TRACK_CLASS_QUIET, handle);
            if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) != -1) {
                music_man.PlayTrack(TRACK_CLASS_QUIET);
            }
        }
    }
    bgPostRequest(LoadAreaData, NULL, NULL, 0);

    f32 icon_time = 0.0f;
    bool character_load_active = true;
    bool fade_scene_active = false;
    bool skip_text_scroll = false;
    bool draw_touch_prompt = false;
    f32 touch_prompt_time = 0.0f;

    const f32 icon_end_time = 3.1f + 0.6f;
    const f32 icons_end_time = icon_end_time + 0.25f;
    const f32 icon_fade_duration = icon_end_time - 3.1f;

    while (true) {
        if (AreaDataLoaded != 0 && !(LoadWait > 0.0f) && !character_load_active) {
            CutBorderScale = 0.0f;
            MainRenderTime = 1.0f;
            music_man.StopAll(0);
            MusicClearAll();
            SoundKillAll();
            legoSetMusicVolume(music_volume);
            textcrawlactive = 0;
            MechSystems::Get()->UnhookClickToPressStart();
            if (loadareadata_loadlevel != 0) {
                loadareadata_loadlevel = 0;
                loadareacharacters_loadedlevel = 1;
            }
            return;
        }

        if (HUB_ADATA != NULL && HUB_ADATA->index == Area && character_load_active && AreaDataLoaded != 0 &&
            waiting_for_character == -1) {
            static i32 lastLoadedCharacter = -1;
            if (hub_character_ready != -1) {
                lastLoadedCharacter = hub_character_ready;
                hub_character_ready = -1;
            }
            UpdateCharacterLoad();
            if (waiting_for_character == -1) {
                hub_character_ready = lastLoadedCharacter;
                character_load_active = false;
            }
        } else if (HUB_ADATA == NULL || HUB_ADATA->index != Area) {
            character_load_active = false;
        }

        NuFrameBegin();
        NuCameraSet(&global_camera);
        readpads_always = 1;
        ReadPads();

        draw_touch_prompt = false;
        if (load_type == 2) {
            if (AreaDataLoaded != 0 && !character_load_active && LoadWait == LOADWAITTIME && !(LoadTime >= 45.0f) &&
                !skip_text_scroll && (NuSound3LoadingSfx() == 0 || LoadTime >= 20.0f)) {
                const u32 skip_buttons = GAMEPAD_JUMP | GAMEPAD_START | GAMEPAD_SPECIAL | GAMEPAD_ACTION | GAMEPAD_TAG;
                if (VEHICLES_ADATA != NULL && VEHICLES_ADATA->index == Area) {
                    skip_text_scroll = true;
                    draw_touch_prompt = false;
                } else if (((GamePad[0].buttons_pressed | GamePad[1].buttons_pressed) & skip_buttons) != 0 ||
                           NuKeyboard_db(1) != 0 || MechSystems::SkipTextScroll != 0) {
                    skip_text_scroll = true;
                    draw_touch_prompt = false;
                    MechSystems::SkipTextScroll = 0;
                } else {
                    draw_touch_prompt = true;
                }
                touch_prompt_time += FRAMETIME;
            }
        } else if (AreaDataLoaded != 0 && !character_load_active) {
            skip_text_scroll = true;
            draw_touch_prompt = false;
        }
        MechSystems::SkipTextScroll = 0;

        UpdateTimer(&GlobalTimer);
        Game.field30_0x7c2c += FRAMETIME;
        LoadTime += FRAMETIME;

        const i32 previous_icon_stage = icon_stage;
        if (icon_stage == 0 &&
            (CharacterDataLoad == 2 || (CharacterDataLoad != 0 && APICharacterLoaded(PlayerID[0]) != NULL &&
                                        APICharacterLoaded(PlayerID[1]) != NULL))) {
            icon_stage = 1;
        }

        if (previous_icon_stage == 1) {
            icon_time += FRAMETIME;
            if (!(icon_time <= 0.6f) && CharacterDataLoad != 2) {
                icon_time = 0.6f;
            }
            if (!(icon_time < icons_end_time)) {
                icon_time = icons_end_time;
                icon_stage = 2;
            }
        }
        if (load_type == 2) {
            if (AreaDataLoaded != 0 && !character_load_active) {
                if (!(LoadWait <= 0.0f) && (!(LoadTime < 45.0f) || skip_text_scroll)) {
                    LoadWait -= FRAMETIME;
                    if (!(LoadWait >= 0.0f)) {
                        LoadWait = 0.0f;
                    }
                }
            }
            legoSetMusicVolume((LoadWait / LOADWAITTIME) * music_volume);
            const f32 backdrop_dt_scale =
                AreaDataLoaded != 0 && !character_load_active ? LoadWait / LOADWAITTIME : 1.0f;
            BackDrop_Update(backdrop_dt_scale * FRAMETIME);
            BackDrop_UpdateColours(1);
        } else if (AreaDataLoaded != 0 && !character_load_active && icon_stage == 2 && !(LoadWait <= 0.0f)) {
            LoadWait -= FRAMETIME;
            if (!(LoadWait >= 0.0f)) {
                LoadWait = 0.0f;
            }
        }

        if (NOSOUND == 0) {
            NuSound3Update();
            music_man.Process(FRAMETIME);
        }
        music_man.Process(FRAMETIME);

        NuRndrBeginScene(-1);
        if (load_type == 2) {
            const u32 top_colour = 0x80000000u | (static_cast<i32>(backdrop_top_b) & 0xff) << 16 |
                                   (static_cast<i32>(backdrop_top_g) & 0xff) << 8 |
                                   (static_cast<i32>(backdrop_top_r) & 0xff);
            const u32 bottom_colour = 0x80000000u | (static_cast<i32>(backdrop_bot_b) & 0xff) << 16 |
                                      (static_cast<i32>(backdrop_bot_g) & 0xff) << 8 |
                                      (static_cast<i32>(backdrop_bot_r) & 0xff);
            NuRndrGradClear(0xf00, static_cast<i32>(top_colour), static_cast<i32>(bottom_colour), 1.0f);
            const f32 backdrop_alpha =
                loadareacharacters_no_backdrop_reset == 0 && LoadTime < 0.5f ? LoadTime * 2.0f : 1.0f;
            BackDrop_Draw(backdrop_alpha, 1);

            f32 crawl_alpha = 1.0f;
            if (AreaDataLoaded == 0 || character_load_active ||
                (LoadWait >= 0.1f && (crawl_alpha = (LoadWait - 0.1f) / (LOADWAITTIME - 0.1f)) > 0.0f)) {
                TextCrawl_Draw(FRAMETIME, 0, crawl_alpha, NULL);
                SetQFont2D();
            }

            if (draw_touch_prompt) {
                const f32 prompt_wave =
                    NuTrigTable[((static_cast<i32>(NuFmod(touch_prompt_time, 1.0f) * 65536.0f) + 0x4000) >> 1) &
                                0x7fff];
                const i32 prompt_alpha = static_cast<i32>(32.0f - prompt_wave * 32.0f);
                SetQFont2D();
                Text3DEx(TTab[tTOUCHTOSTART], 0.0f, 0.745f, 1.0f, 0.4f, 0.4f, 0.4f, 0, 255, 255, 255, prompt_alpha);
            }
        } else {
            NuRndrGradClear(0xf00, static_cast<i32>(0x80000000u), static_cast<i32>(0x80000000u), 1.0f);
            if (load_type == 1) {
                BackDrop_Draw(1.0f, 1);
            }

            if (LoadWait == LOADWAITTIME) {
                const i32 wobble_angle = static_cast<i32>(NuFmod(LoadTime, 1.0f) * 65536.0f);
                if (HUB_ADATA != NULL && HUB_ADATA->index == Area && CharacterDataLoad != 0 && icon_time > 0.0f &&
                    icon_time < icons_end_time) {
                    const f32 wobble = NuTrigTable[(wobble_angle >> 1) & 0x7fff] * 0.15f;
                    const bool widescreen = Game_OptionsSave != NULL && Game_OptionsSave->field11_0xb != 0;
                    const f32 icon_travel = widescreen ? 0.406875f : 0.4425f;
                    const f32 icon_x = widescreen ? 0.106875f : 0.1425f;
                    const f32 icon_scale = widescreen ? 0.35f : 0.4f;

                    if (icon_time < icon_end_time) {
                        f32 alpha = 1.0f;
                        f32 x = -icon_x;
                        if (!(icon_time >= 0.6f)) {
                            const f32 progress = icon_time / 0.6f;
                            const i32 angle = static_cast<i32>(progress * 16384.0f);
                            x = -(NuTrigTable[(angle >> 1) & 0x7fff] * -0.3f + icon_travel);
                            alpha = progress;
                        } else if (!(icon_time < 3.1f)) {
                            alpha = 1.0f - (icon_time - 3.1f) / icon_fade_duration;
                        }
                        drawcharicon_find = 1;
                        DrawCharIcon(PlayerID[0], x, 0.16624999f, 0.0f, icon_scale, 0xa6, alpha,
                                     (0.85f + wobble) * alpha, 1, NULL);
                    }

                    const f32 second_time = icon_time - 0.25f;
                    if (!(second_time <= 0.0f) && !(second_time >= icon_end_time)) {
                        f32 alpha = 1.0f;
                        f32 x = icon_x;
                        if (!(second_time >= 0.6f)) {
                            const f32 progress = second_time / 0.6f;
                            const i32 angle = static_cast<i32>(progress * 16384.0f);
                            x = NuTrigTable[(angle >> 1) & 0x7fff] * -0.3f + icon_travel;
                            alpha = progress;
                        } else if (!(second_time < 3.1f)) {
                            alpha = 1.0f - (second_time - 3.1f) / icon_fade_duration;
                        }
                        drawcharicon_find = 1;
                        DrawCharIcon(PlayerID[1], x, -0.16624999f, 0.0f, icon_scale, 0xa5, alpha,
                                     (0.85f - wobble) * alpha, 1, NULL);
                    }
                }
            }
        }

        if (load_type != 2 && (HUB_ADATA == NULL || HUB_ADATA->index != Area || fade_scene_active)) {
            NuRndrEndScene();
            FadeSys.Update();
            FadeSys.Draw();
            NuRndrBeginScene(-1);
            fade_scene_active = true;
        }

        static_cast<ThingManager *>(theGameThings)->RenderThings(NULL);
        NuRndrEndScene();

        if (AreaDataLoaded != 0 && !character_load_active && !(LoadWait >= 0.1f) &&
            (FadeSys.pending_type == FADE_TYPE_NONE || FadeSys.pending_type == FADE_TYPE_STILL)) {
            FADETYPE wipe = {FADE_TYPE_STILL_WIPE};
            FadeSys.SetFade(wipe, 0);
            NeedScreenGrab(1);
            GrabStillScreen();
        }

        edGraEnableTerrainSwap();
        FRAMETIME = NuFrameEnd();
        edGraDisableTerrainSwap();
    }
}

void StoreSceneProgress(NUGSCN *gscn, SCENEPROGRESS_s *progress, i32 param) {
    if (progress == NULL || gscn == NULL)
        return;
    memset(progress, 0, sizeof(*progress));
    i32 count = NuSpecialGetNumSpecials(gscn);
    nuhspecial_s special;
    for (i32 i = 0; i < count; ++i) {
        if (i != 0)
            NuSpecialGetNext(&special);
        else
            NuSpecialGetFirst(gscn, &special, 1);
        progress->specials[i].visible = NuSpecialGetVisibilityFn(&special);
        if (gscn->instance_animation_data == NULL)
            continue;
        nuinstanim_s *animation = NuSpecialGetInstAnim(&special);
        if (animation == NULL || gscn->instance_animation_data[animation->anim_ix] == NULL)
            continue;
        progress->specials[i].frame =
            animation->ltime <= 2048.0f && animation->ltime >= 0.0f ? static_cast<u32>(animation->ltime) : 1;
        progress->specials[i].playing = animation->playing;
        progress->specials[i].waiting = animation->waiting;
        progress->specials[i].repeating = animation->repeating;
        progress->specials[i].tfactor = animation->tfactor;
        if (param != 0)
            animation->fparam1 = animation->tfactor;
    }
}

void StoreLevelProgressFn(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress, i32 area_progress) {
    i32 index;
    if (area_progress != 0) {
        if (world == NULL || world->area == NULL)
            return;
        index = world->area->level_count;
    } else {
        if (VADER_ADATA != NULL && VADER_ADATA == WORLD->area)
            return;
        if (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area && bonus_gunship_store_progress_flag == 0)
            return;
        if (world == NULL)
            return;
        index = (i8)world->current_level->area_level_index;
        if (world->area != NULL && (world->area->flags & 4) != 0)
            goto store_flags;
    }
    StoreProgressAICharacter(progress);
    Grabber_StoreProgress(world, progress);
    GizmoSysStoreProgress(world->gizmo_sys, world, index);
    if (progress != NULL) {
        GizFlowStoreProgress(world->giz_flow, &progress->giz_flow_progress);
        StoreSceneProgress(world->current_gscn, reinterpret_cast<SCENEPROGRESS_s *>(progress), 0);
    }
    GameAnimSys_StoreProgress(world->game_anim_sys, index);
    for (i32 i = 0; i < world->processor_count; ++i) {
        if (progress == NULL)
            continue;
        LEVELSCRIPTPROCESS &processor = world->processors[i];
        if (NuStrLen(processor.name) == 0)
            continue;
        for (i32 j = 0; j < 32; ++j) {
            if (NuStrLen(progress->scripts[j].name) == 0) {
                NuStrCpy(world->level_progress->scripts[j].name, processor.name);
                for (i32 k = 0; k < 4; ++k)
                    progress->scripts[j].params[k] = processor.processor.params[k];
                break;
            }
            if (NuStrICmp(progress->scripts[j].name, processor.name) != 0) {
                continue;
            }
            memcpy(progress->scripts[j].params, processor.processor.params, 16);
            break;
        }
    }
store_flags:
    if (progress != NULL) {
        if (world->level_progress != progress)
            memmove(progress->disabled_effect_names, world->level_progress->disabled_effect_names, 0xc0);
        reinterpret_cast<u8 *>(&progress->flags)[0] =
            (reinterpret_cast<u8 *>(&progress->flags)[0] & ~4) | ((world->field_0x5174 & 1) << 2) | 2;
    }
}

void StoreLevelProgress(WORLDINFO *world) {
    if (world != NULL) {
        StoreLevelProgressFn(world, world->level_progress, 0);
    }
}

void ResetScene(nugscn_s *scene, SCENEPROGRESS_s *progress) {
    if (progress == NULL || scene == NULL)
        return;
    i32 count = NuSpecialGetNumSpecials(scene);
    if (count == 0)
        return;
    nuhspecial_s special;
    for (i32 i = 0; i < count; ++i) {
        if (i != 0)
            NuSpecialGetNext(&special);
        else
            NuSpecialGetFirst(scene, &special, 1);
        NuSpecialSetVisibility(&special, progress->specials[i].visible);
        NUMTX *base = NuSpecialGetMtx(&special);
        NuSpecialSetDrawMtx(&special, base);
        NuSpecialUpdate(&special);
        if (scene->instance_animation_data == NULL)
            continue;
        nuinstanim_s *animation = NuSpecialGetInstAnim(&special);
        if (animation == NULL)
            continue;
        nuanimdata_s *data = scene->instance_animation_data[animation->anim_ix];
        if (data == NULL)
            continue;
        animation->ltime = static_cast<f32>(static_cast<u32>(progress->specials[i].frame));
        animation->playing = progress->specials[i].playing;
        animation->waiting = progress->specials[i].waiting;
        animation->repeating = progress->specials[i].repeating;
        animation->tfactor = progress->specials[i].tfactor;
        animation->prev_eval_time = animation->ltime;
        NuAnimData2CalcMatrix(data, 0, animation->ltime, &animation->mtx);
        scene->instance_animation_matrices[animation - scene->instance_animations] = animation->mtx;
        NUVEC *translation = NUMTX_GET_ROW_VEC(&animation->mtx, 3);
        NUVEC *base_translation = NUMTX_GET_ROW_VEC(base, 3);
        translation->x += base_translation->x;
        translation->y += base_translation->y;
        translation->z += base_translation->z;
    }
}

SCENESPECIALPROGRESS_s *FindSceneStateObj(nugscn_s *scene, SCENEPROGRESS_s *progress, nuhspecial_s *target) {
    if (progress == NULL || scene == NULL || target == NULL)
        return NULL;
    i32 count = NuSpecialGetNumSpecials(scene);
    if (count == 0)
        return NULL;
    nuhspecial_s special;
    for (i32 i = 0; i < count; ++i) {
        if (i != 0)
            NuSpecialGetNext(&special);
        else
            NuSpecialGetFirst(scene, &special, 1);
        if (NuSpecialCompare(&special, target))
            return &progress->specials[i];
    }
    return NULL;
}

void CalculateWorldSize(WORLDINFO *world) {
    NUVEC minimum = v000;
    NUVEC maximum = v000;
    if (world == NULL || world->current_gscn == NULL) {
        return;
    }

    NUGSCN *scene = world->current_gscn;
    for (i32 i = 0; i < scene->portal_instance_count * 2; i += 2) {
        // Retail walks the boxes as consecutive first/second vectors.
        NUVEC4 *first = &((NUVEC4 *)scene->portal_boxes)[i];
        NUVEC4 *second = &((NUVEC4 *)scene->portal_boxes)[i + 1];
        NUVEC box_maximum = *(NUVEC *)second;
        NUVEC box_minimum = *(NUVEC *)first;
        if (scene->display_list != NULL &&
            (scene->display_list->render_buffer & NUDL_SCENE_RENDER_FLAG_CENTER_EXTENT_BOUNDS) != 0) {
            box_minimum.z = first->z - second->z;
            box_minimum.x = first->x - second->x;
            box_minimum.y = first->y - second->y;
            box_maximum.x = first->x + second->x;
            box_maximum.y = first->y + second->y;
            box_maximum.z = first->z + second->z;
        }
        minimum.x = MIN(box_minimum.x, minimum.x);
        minimum.y = MIN(box_minimum.y, minimum.y);
        minimum.z = MIN(box_minimum.z, minimum.z);
        maximum.x = MAX(box_maximum.x, maximum.x);
        maximum.y = MAX(box_maximum.y, maximum.y);
        maximum.z = MAX(box_maximum.z, maximum.z);
    }

    world->level_max[0] = maximum.x;
    world->level_max[1] = maximum.y;
    world->level_max[2] = maximum.z;
    world->level_min[0] = minimum.x;
    world->level_min[1] = minimum.y;
    world->level_min[2] = minimum.z;
}

void WorldInfo_Load(WORLDINFO *world) {
    LOG_INFO("world load world=%p", (void *)world);
    char buf[268];
    char titles[64];
    NUGSCN *cutscene_scene;
    i32 *page_handles;
    i32 aligned_buf;
    char ai_name[32];
    char title_light_path[] = "levels\\titles\\titles";
    i32 ai_buf_size;
    i32 rtl_id;
    char *light_path;

    Level_LoadConfigFile(world);

    if (abort_load != 0 || (world->config_count > 0 && (LevelConfig_BeforeLoad(world->current_level, ConfigBuffer,
                                                                               Level_ConfigBeforeLoad_GameKeywords),
                                                        abort_load != 0))) {
        goto abort;
    }

    if ((world->current_level->flags & LEVEL_UNKNOWN_FLAG_4) != 0) {
        // Align giz_buffer
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);

        if (world->current_level == TITLES_LDATA) {
            NuStrCpy(buf, "levels\\titles\\");
            switch (Text_Language) {
                case 2:
                    NuStrCpy(titles, "titles_french");
                    break;
                case 3:
                    NuStrCpy(titles, "titles_spanish");
                    break;
                case 4:
                    NuStrCpy(titles, "titles_german");
                    break;
                case 5:
                    NuStrCpy(titles, "titles_italian");
                    break;
                case 8:
                    NuStrCpy(titles, "titles_danish");
                    break;
                default:
                    if (Text_Language != 0x12) {
                        NuStrCpy(titles, "titles_uk");
                    } else {
                        NuStrCpy(titles, "titles_us");
                    }
                    break;
            }
            NuStrCat(buf, titles);
        } else if (world->current_level == (LEVELDATA *)PLATFORM_LDATA) {
            NuStrCpy(buf, "levels\\episode_v\\cloudcityescape\\cloudcityescape_b\\cloudcityescape_b");
        } else {
            NuStrCpy(buf, world->config_file);
        }
        NuStrCat(buf, ".gsc");

        numtl_force_mipmode = (i32)(u8)world->current_level->mipmap_mode + 1;
        world->current_gscn = NuGScnRead(&world->giz_buffer, world->unknown_0108, buf);
        numtl_force_mipmode = 0;

        StoreSceneProgress(world->current_gscn, (SCENEPROGRESS_s *)&world->progress_data, 1);
        SaveSceneObjectAnimTFactors(world->current_gscn);

        if (world->current_gscn != NULL) {
            CalculateWorldSize(world);
        }
        if (abort_load != 0)
            goto abort;
    }

    // Load pictures for titles/credits
    if (world->current_level == TITLES_LDATA || world->current_level == CREDITS_LDATA) {
        numtl_force_mipmode = (i32)(u8)world->current_level->mipmap_mode + 1;
        world->scene = NuGScnRead(&world->giz_buffer, world->unknown_0108, "levels\\titles\\pictures.gsc");
        if (abort_load != 0)
            goto abort;
    }

    // Create game animation system
    world->game_anim_sys = (GAMEANIMSYS_s *)GameAnimSys_Create(&world->giz_buffer, &world->unknown_0108);

    // Load particles
    Particles_Load(world, debris_name, 400, 0x93);
    if (abort_load != 0)
        goto abort;
    LoadPartFile(world);
    if (abort_load != 0)
        goto abort;

    // Configure bolt types
    if (world->config_count > 0) {
        BoltTypes_Configure(world, ConfigBuffer);
    }

    // Create antinode system
    world->game_antinode_sys = (GAMEANTINODESYS_s *)GameAntnode_CreateSys(
        world, &world->giz_buffer, &world->unknown_0108, (i32)(u16)world->current_level->max_antinodes);

    // Create gizmo system
    world->gizmo_sys = (GIZMOSYS_s *)CreateGizmoSys(world, &world->giz_buffer, &world->unknown_0108);

    // Load editor splines
    sprintf(buf, "%s%s", world->config_file, ".spl");
    LoadEditorSplines(buf, &world->giz_buffer, &world->unknown_0108);

    GizmoBlowupResetNameTable();
    LoadGizmoSys((GIZMOSYS_s *)world->gizmo_sys, world, world->config_file);
    if (abort_load != 0)
        goto abort;

    // Area-specific loading
    if (world->area == NULL) {
        goto after_area;
    }
    if (world->area == HUB_ADATA && GAMEDEMO == 0) {
        Hub_LoadAndFixUpMiniKits(world, &world->giz_buffer, &world->unknown_0108);
        if (abort_load != 0)
            goto abort;
        goto after_area;
    }
    if ((world->current_level->flags & LEVEL_STATUS) != 0 && world->area->minikit_id != -1) {
        MiniKit_Load(&world->minikit, (i32)(i16)world->area->minikit_id, &world->giz_buffer, &world->unknown_0108,
                     NULL);
        if (world->minikit.gscn != NULL) {
            MiniKit_InitPieces(&world->minikit, 10, &world->giz_buffer, &world->unknown_0108);
        }
        if (abort_load != 0)
            goto abort;
    }

after_area:
    // Load big icon scene for hub/status levels
    if ((world->current_level == HUB_LDATA || (world->current_level->flags & LEVEL_STATUS) != 0) &&
        big_icon_scene == NULL) {
        world->icons_gscn = NuGScnRead(&world->giz_buffer, world->unknown_0108, "stuff\\icons\\starwars_icons_all.gsc");
    }

    // Determine which scene to use for cutscenes
    cutscene_scene = area_scene;
    if (cutscene_scene == NULL || world->area == NULL || (world->area->flags & AREAFLAG_OVERRIDE_THINGS_SCENE) == 0) {
        cutscene_scene = things_scene;
    }

    // Load cutscenes
    page_handles = (i32 *)&world->page_pp;
    world->cutscene_sys =
        (CUTSYS *)CutScenes_Load(ConfigBuffer, world->current_gscn, cutscene_scene, world->page_pp, &world->giz_buffer,
                                 // Sublevel and level indices follow the two buffer arguments.
                                 &world->unknown_0108, world->level_sub_id, world->level_idx, world);
    if (abort_load != 0)
        goto abort;

    // Character scenes
    aligned_buf = ALIGN((i32)world->giz_buffer.addr, 4);
    world->minikit.character_scenes = reinterpret_cast<CHARSCENE_s *>(aligned_buf);
    world->giz_buffer.addr = (usize)(aligned_buf + CHARCOUNT * 0x10);
    CharScenes_LevelLoad(world);

    // SockSys for certain level types
    if ((world->current_level->flags & (LEVEL_GAMEPLAY | LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == LEVEL_GAMEPLAY) {
        world->sock_sys = SockSysInit(&world->giz_buffer, world->unknown_0108, world->current_gscn);
    }

    LevelSplines_InitForLevel(world);
    LevelObjects_InitForLevel(world);
    BoltTypes_Init(world);

    // Config-based subsystem initialization
    if (world->config_count > 0) {
        LevelConfig_AfterLoad(world->current_level, ConfigBuffer, &LevelConfigKeywords_AfterLoad);
        EquivalentObjects_Configure(world, ConfigBuffer);
        Teleports_Configure(world, ConfigBuffer);
        Doors_Configure(world, ConfigBuffer);
        Faders_Configure(world, ConfigBuffer);
        CharPlatforms_Configure(world, ConfigBuffer);
        Grabber_Configure(world, ConfigBuffer);
        Pulses_Configure(world, ConfigBuffer);
        TrafficAnimSys_Configure(world, ConfigBuffer);
        SpecialMiniKits_Configure(world, ConfigBuffer);
        GizForceSFX_Configure(world, ConfigBuffer);
    }

    // SockSys configuration
    if (world->sock_sys != NULL) {
        if (world->config_count > 0) {
            SockSys_Configure(world->sock_sys, ConfigBuffer, 0, &world->giz_buffer, &world->unknown_0108,
                              world->current_gscn);
        }
        SockSys_GenerateData(world->sock_sys, &world->giz_buffer, world->unknown_0108);
        if (abort_load != 0)
            goto abort;
    }

    // Terrain/grass/bridge loading
    if (world->area != NULL &&
        (world->area->flags & (AREAFLAG_VEHICLE_AREA | AREAFLAG_BONUS_AREA)) ==
            (AREAFLAG_VEHICLE_AREA | AREAFLAG_BONUS_AREA) &&
        (world->current_level->flags & (LEVEL_STATUS | LEVEL_OUTRO | LEVEL_MIDTRO | LEVEL_INTRO)) == 0) {
        CharacterMiniKits_Load(&MiniKitCollection, world, &world->giz_buffer, &world->unknown_0108);
        if (abort_load != 0)
            goto abort;
    } else {
        LoadTerrainFile(world);
        if (abort_load != 0)
            goto abort;
        LoadGrassFile(world);
        if (abort_load != 0)
            goto abort;
        LoadBridgeFile(world);
        if (abort_load != 0)
            goto abort;
    }

    // AI system loading
    if ((world->current_level->flags & (LEVEL_GAMEPLAY | LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == LEVEL_GAMEPLAY &&
        world->current_level != (LEVELDATA *)PLATFORM_LDATA) {
        world->ai_loaded = 0;
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        ai_name[0] = 'a';
        ai_name[1] = 'i';
        ai_name[2] = 0;
        ai_buf_size = 0x1cc00;
        if (RETAKED_LDATA != NULL && world->current_level == (LEVELDATA *)RETAKED_LDATA) {
            ai_buf_size = 0x1e800;
        }
        world->ai_sys = (AISYS *)AISysLoad(&world->giz_buffer, &world->unknown_0108, ai_buf_size, world->current_gscn,
                                           world->current_level->dir, world->current_level->name, ai_name);
        world->ai_path_cnx_control_sys =
            (AIPATHCNXCONTROLSYS_s *)AIPathCnxControlSysCreate(&world->giz_buffer, &world->unknown_0108, 0x40);
        world->ai_path_cnx_helper_sys =
            (AIPATHCNXHELPERSYS_s *)AIPathCnxHelperSysCreate(&world->giz_buffer, &world->unknown_0108, 0x20);
        world->ai_trigger_set_sys =
            (AITRIGGERSETSYS_s *)AITriggerSetSysCreate(&world->giz_buffer, &world->unknown_0108);

        if (world->ai_sys != NULL) {
            world->mech_auto_jump_manager = new MechAutoJumpManager(world->ai_sys);
        }

        GameAIScriptAddLevelSfx(world, &global_aiscripts);
        GameAIScriptAddLevelSfx(world, &world->ai_sys->scripts);

        world->climb_object_sys = (CLIMBOBJECTSYS_s *)CreateClimbObjectSys(
            &world->giz_buffer, &world->unknown_0108, (i32)(u8)world->current_level->max_climb_objs);
    } else {
        world->ai_loaded = 1;
    }

    if (abort_load != 0)
        goto abort;

    // API object system
    world->api_object_sys =
        (APIOBJECTSYS_s *)APIObjectSysInit(sizeof(GameObject_s), &world->giz_buffer, &world->unknown_0108);
    if (abort_load != 0)
        goto abort;

    // Lights
    if ((world->current_level->flags & LEVEL_UNKNOWN_FLAG_4) != 0) {
        light_path = world->current_level == TITLES_LDATA ? title_light_path : world->config_file;
        LoadLights(world, light_path);
        rtl_id = rtlFindByUserId(reinterpret_cast<usize>(world->rtl_set), 1);
        world->rtl_id = rtl_id;
        if (rtl_id == -1) {
            world->light_dir = 0;
        } else {
            rtlGetDirection(reinterpret_cast<usize>(world->rtl_set), rtl_id, (void **)&world->light_dir);
        }
    } else {
        world->rtl_id = -1;
        world->light_dir = 0;
    }

    if (abort_load != 0)
        goto abort;

    // More config-based subsystems
    if (world->config_count > 0) {
        RippleEffects_Configure(world, ConfigBuffer);
        PortalDoors_Configure(world, ConfigBuffer);
        if (abort_load != 0)
            goto abort;
    }

    GizmoSysAddGizmos((GIZMOSYS_s *)world->gizmo_sys, (GIZFLOW_s *)world->giz_flow, world);
    if (abort_load != 0)
        goto abort;

    // Load gizmo flow
    NuStrCpy(buf, world->config_file);
    NuStrCat(buf, ".git");
    world->giz_flow =
        (GIZFLOW_s *)LoadGizFlow(world, (GIZMOSYS_s *)world->gizmo_sys, buf, &world->giz_buffer, &world->unknown_0108);
    if (abort_load != 0)
        goto abort;

    InitSpecialSfx(world);
    LoadSpecialSfxFile(world);
    if (abort_load != 0)
        goto abort;

    // Level load function
    if (world->current_level->load_fn != NULL) {
        world->current_level->load_fn(world, &world->giz_buffer, &world->unknown_0108);
    }

    SetAreaPickupGravity(world->level_sub_id, world->level_idx);
    world->loaded = 1;
    return;

abort:
    WorldInfo_Dump(world);
    WorldInfo_Reset(world, -1);
}

void FixUpLevels(LEVELFIXUP *fixup) {
    Levels_FixUp(fixup);

    LEVELDATA *level = Level_FindByName("titles", NULL);
    TITLES_LDATA = level;
    if (level != NULL) {
        level->init_fn = Titles_Init;
        level->update_fn = Titles_Update;
        level->draw_fn = Titles_Draw;
        level->flags &= ~(LEVEL_GAMEPLAY | LEVEL_TERRAIN);
        level->music_index = (i16)GetMusicIndex("titles", MusicInfo, -1);

        TITLES_LDATA->music_tracks[0][0] = TITLES_LDATA->music_tracks[0][1] =
            music_man.GetTrackHandle(TRACK_CLASS_QUIET, "titles");
        TITLES_LDATA->music_tracks[1][0] = TITLES_LDATA->music_tracks[1][1] =
            music_man.GetTrackHandle(TRACK_CLASS_ACTION, "titles");
        TITLES_LDATA->music_tracks[2][0] = TITLES_LDATA->music_tracks[2][1] =
            music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, "titles");
    }

    level = Level_FindByName("status", NULL);
    STATUS_LDATA = level;
    if (level != NULL) {
        level->update_fn = UpdateStatusScreen;
        level->draw_status_fn = DrawStatusScreen;
        level->flags &= ~(LEVEL_GAMEPLAY | LEVEL_TERRAIN);
        level->flags |= LEVEL_STATUS;
    }

    {
        LEVELDATA *level = Level_FindByName("credits", NULL);
        CREDITS_LDATA = level;
        if (level != NULL) {
            level->load_fn = Credits_Load;
            level->init_fn = Credits_Init_Game;
            level->update_fn = Credits_Update_Game;
            level->draw_fn = Credits_Draw_Game;
            level->draw_status_fn = Credits_DrawPanel;
            level->flags = 0;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("map", NULL);
        HUB_LDATA = level;
        if (level != NULL) {
            level->load_fn = Hub_Load;
            level->init_fn = Hub_Init;
            level->reset_fn = Hub_Reset;
            level->update_fn = Hub_Update;
            level->draw_fn = Hub_Draw3D;
            level->draw_status_fn = Hub_DrawPanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("speederchase_a", NULL);
        SPEEDERCHASEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = SpeederChaseA_Init;
            level->reset_fn = SpeederChaseA_Reset;
            level->update_fn = SpeederChaseA_Update;
            level->draw_status_fn = SpeederChaseA_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("negotiations_a", NULL);
        NEGOTIATIONSA_LDATA = level;
        if (level != NULL) {
            level->init_fn = NegotiationsA_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("negotiations_b", NULL);
        NEGOTIATIONSB_LDATA = level;
        if (level != NULL) {
            level->init_fn = NegotiationsB_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("negotiations_c", NULL);
        NEGOTIATIONSC_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("gungan_a", NULL);
        GUNGAN_A_LDATA = level;
        if (level != NULL) {
            level->init_fn = GunganA_Init;
            level->update_fn = GunganA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("gungan_b", NULL);
        GUNGAN_B_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("rescue_a", NULL);
        RESCUEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = RescueA_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("rescue_b", NULL);
        RESCUEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = RescueB_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("rescue_c", NULL);
        RESCUEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = RescueC_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("rescue_e", NULL);
        RESCUEE_LDATA = level;
        if (level != NULL) {
            level->init_fn = RescueE_Init;
        }
    }

    // PodRace_Arrival/Intro/Outro array — original fills PODRACELEVELS[11]
    // order matches .rodata 0x565c20: Arrival1-4, Intro, B, C, A, Outro1, Outro2, Status
    {
        LEVELDATA *level = Level_FindByName("PodRace_Arrival1", NULL);
        PODRACELEVELS[0] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Arrival2", NULL);
        PODRACELEVELS[1] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Arrival3", NULL);
        PODRACELEVELS[2] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Arrival4", NULL);
        PODRACELEVELS[3] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Intro", NULL);
        PODRACELEVELS[4] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_B", NULL);
        PODRACELEVELS[5] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_C", NULL);
        PODRACELEVELS[6] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_A", NULL);
        PODRACELEVELS[7] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Outro1", NULL);
        PODRACELEVELS[8] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Outro2", NULL);
        PODRACELEVELS[9] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Status", NULL);
        PODRACELEVELS[10] = level;
    }

    {
        LEVELDATA *level = Level_FindByName("ANewHope_Intro", NULL);
        ANEWHOPELEVELS[0] = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("ANewHope_A", NULL);
        ANEWHOPELEVELS[1] = level;
        if (level != NULL) {
            level->init_fn = ANewHopeA_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("ANewHope_B", NULL);
        ANEWHOPELEVELS[2] = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("ANewHope_Status", NULL);
        ANEWHOPELEVELS[3] = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("podrace_b", NULL);
        PODRACEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = PodRaceBInit;
            level->reset_fn = PodRaceBReset;
            level->update_fn = PodRaceBUpdate;
            level->always_update_fn = PodRaceAlwasyUpdate;
            level->draw_status_fn = PodRacePanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("podrace_a", NULL);
        PODRACEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = PodRaceAInit;
            level->reset_fn = PodRaceAReset;
            level->update_fn = PodRaceAUpdate;
            level->always_update_fn = PodRaceA_AlwaysUpdate;
            level->draw_fn = PodRaceADraw;
            level->draw_status_fn = PodRacePanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("podrace_c", NULL);
        PODRACEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = PodRaceCInit;
            level->reset_fn = PodRaceCReset;
            level->update_fn = PodRaceCUpdate;
            level->always_update_fn = PodRaceAlwasyUpdate;
            level->draw_status_fn = PodRacePanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("PodRace_Outro1", NULL);
        PODRACEOUTRO1_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("podrace_status", NULL);
        PODRACESTATUS_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("PodSprint_A", NULL);
        PODSPRINTA_LDATA = level;
        if (level != NULL) {
            level->init_fn = PodSprintA_Init;
            level->reset_fn = PodSprintA_Reset;
            level->update_fn = PodSprintA_Update;
            level->draw_status_fn = PodSprintA_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("AnakinsFlight_B", NULL);
        ANAKINSFLIGHTB_LDATA = level;
        if (level != NULL) {
            level->init_fn = AnakinsFlightB_Init;
            level->update_fn = AnakinsFlightB_Update;
            level->draw_fn = AnakinsFlightB_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("retake_intro1", NULL);
        RETAKEINTRO1_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("retake_intro2", NULL);
        RETAKEINTRO2_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("retake_intro3", NULL);
        RETAKEINTRO3_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("retake_b", NULL);
        RETAKEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("retake_d", NULL);
        RETAKED_LDATA = level;
        if (level != NULL) {
            level->init_fn = RetakeD_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("retake_e", NULL);
        RETAKEE_LDATA = level;
        if (level != NULL) {
            level->init_fn = RetakeE_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("retake_g", NULL);
        RETAKEG_LDATA = level;
        if (level != NULL) {
            level->init_fn = RetakeG_Init;
            level->reset_fn = RetakeG_Reset;
            level->update_fn = RetakeG_Update;
            level->draw_status_fn = RetakeG_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Maul_A", NULL);
        MAULA_LDATA = level;
        if (level != NULL) {
            level->init_fn = MaulA_Init;
            level->reset_fn = MaulA_Reset;
            level->update_fn = MaulA_Update;
            level->draw_status_fn = MaulA_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Maul_B", NULL);
        MAULB_LDATA = level;
        if (level != NULL) {
            level->init_fn = MaulB_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Maul_D", NULL);
        MAULD_LDATA = level;
        if (level != NULL) {
            level->init_fn = MaulD_Init;
            level->update_fn = MaulD_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Maul_E", NULL);
        MAULE_LDATA = level;
        if (level != NULL) {
            level->init_fn = MaulE_Init;
            level->update_fn = MaulE_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Maul_F", NULL);
        MAULF_LDATA = level;
        if (level != NULL) {
            level->init_fn = MaulF_Init;
            level->reset_fn = MaulF_Reset;
            level->update_fn = MaulF_Update;
            level->draw_status_fn = MaulF_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Jedi_B", NULL);
        JEDI_B_LDATA = level;
        if (level != NULL) {
            level->init_fn = JediB_Init;
            level->reset_fn = JediB_Reset;
            level->update_fn = JediB_Update;
            level->draw_status_fn = JediB_DrawPanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Jedi_Outro", NULL);
        JEDI_OUTRO_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("gunship_a", NULL);
        GUNSHIPA_LDATA = level;
        if (level != NULL) {
            level->init_fn = GunshipA_Init;
            level->update_fn = GunshipA_Update;
            level->draw_fn = GunshipA_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("gunship_b", NULL);
        GUNSHIPB_LDATA = level;
        if (level != NULL) {
            level->reset_fn = GunshipB_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("bonus_gunship_a", NULL);
        BONUS_GUNSHIPA_LDATA = level;
        if (level != NULL) {
            level->reset_fn = BonusGunshipA_Reset;
            level->update_fn = BonusGunshipA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("bonus_gunship_b", NULL);
        BONUS_GUNSHIPB_LDATA = level;
        if (level != NULL) {
            level->init_fn = BonusGunshipB_Init;
            level->reset_fn = BonusGunshipB_Reset;
            level->update_fn = BonusGunshipB_Update;
            level->draw_status_fn = BonusGunshipB_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("pursuit_a", NULL);
        BOUNTYHUNTERPURSUITA_LDATA = level;
        if (level != NULL) {
            level->init_fn = BountyHunterPursuitA_Init;
            level->reset_fn = BountyHunterPursuitA_Reset;
            level->update_fn = BountyHunterPursuitA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("pursuit_b", NULL);
        BOUNTYHUNTERPURSUITB_LDATA = level;
        if (level != NULL) {
            level->init_fn = BountyHunterPursuitB_Init;
            level->reset_fn = BountyHunterPursuitB_Reset;
            level->update_fn = BountyHunterPursuitB_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("pursuit_c", NULL);
        BOUNTYHUNTERPURSUITC_LDATA = level;
        if (level != NULL) {
            level->init_fn = BountyHunterPursuitC_Init;
            level->reset_fn = BountyHunterPursuitC_Reset;
            level->update_fn = BountyHunterPursuitC_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("pursuit_d", NULL);
        BOUNTYHUNTERPURSUITD_LDATA = level;
        if (level != NULL) {
            level->init_fn = BountyHunterPursuitD_Init;
            level->reset_fn = BountyHunterPursuitD_Reset;
            level->update_fn = BountyHunterPursuitD_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("pursuit_e", NULL);
        BOUNTYHUNTERPURSUITE_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("dogfight_a", NULL);
        DOGFIGHTA_LDATA = level;
        if (level != NULL) {
            level->init_fn = ChrisDogFightAInit;
            level->reset_fn = ChrisDogFightAReset;
            level->update_fn = ChrisDogFightAUpdate;
            level->draw_fn = ChrisDogFightADraw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Factory_B", NULL);
        FACTORYB_LDATA = level;
        if (level != NULL) {
            level->init_fn = FactoryB_Init;
            level->reset_fn = FactoryB_Reset;
            level->update_fn = FactoryB_Update;
            level->draw_fn = FactoryB_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Factory_D", NULL);
        FACTORYD_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Factory_F", NULL);
        FACTORYF_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Factory_G", NULL);
        FACTORYG_LDATA = level;
        if (level != NULL) {
            level->init_fn = FactoryG_Init;
            level->update_fn = FactoryG_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Dooku_C", NULL);
        DOOKUC_LDATA = level;
        if (level != NULL) {
            level->init_fn = DookuC_Init;
            level->reset_fn = DookuC_Reset;
            level->update_fn = DookuC_Update;
            level->draw_status_fn = DookuC_DrawPanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Dooku_Outro", NULL);
        DOOKUOUTRO_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_A", NULL);
        KAMINOA_LDATA = level;
        if (level != NULL) {
            level->always_update_fn = KaminoA_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_C", NULL);
        KAMINOC_LDATA = level;
        if (level != NULL) {
            level->reset_fn = KaminoC_Reset;
            level->init_fn = KaminoC_Init;
            level->update_fn = KaminoC_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_D", NULL);
        KAMINOD_LDATA = level;
        if (level != NULL) {
            level->init_fn = KaminoD_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_Outro1", NULL);
        KAMINOOUTRO_LDATA = level;
        if (level != NULL) {
            level->init_fn = KaminoOutro_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_A", NULL);
        KAMINOA_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_C", NULL);
        KAMINOC_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_E", NULL);
        KAMINOE_LDATA = level;
        if (level != NULL) {
            level->reset_fn = KaminoE_Reset;
            level->init_fn = KaminoE_Init;
            level->always_update_fn = KaminoE_AlwaysUpdate;
            level->update_fn = KaminoE_Update;
            level->draw_fn = KaminoE_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kamino_F", NULL);
        KAMINOF_LDATA = level;
        if (level != NULL) {
            level->init_fn = KaminoF_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("NB_Kamino_a", NULL);
        NB_KAMINOALDATA = level;
        if (level != NULL) {
            level->init_fn = NbKaminoA_Init;
            level->always_update_fn = KaminoA_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Dogfight_A", NULL);
        DOGFIGHTA_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Cruiser_A", NULL);
        CRUISERA_LDATA = level;
        if (level != NULL) {
            level->init_fn = CruiserAInit;
            level->update_fn = CruiserAUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Cruiser_B", NULL);
        CRUISERB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Cruiser_C", NULL);
        CRUISERC_LDATA = level;
        if (level != NULL) {
            level->reset_fn = CruiserCReset;
            level->update_fn = CruiserCUpdate;
            level->draw_status_fn = CruiserCPanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Cruiser_D", NULL);
        CRUISERD_LDATA = level;
        if (level != NULL) {
            level->init_fn = CruiserDInit;
            level->reset_fn = CruiserDReset;
            level->update_fn = CruiserDUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Cruiser_E", NULL);
        CRUISERE_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("cruiser_g", NULL);
        CRUISERG_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Grievous_A", NULL);
        GRIEVOUSA_LDATA = level;
        if (level != NULL) {
            level->init_fn = GrievousA_Init;
            level->reset_fn = GrievousA_Reset;
            level->update_fn = GrievousA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("temple_A", NULL);
        TEMPLEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = TempleA_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("temple_b", NULL);
        TEMPLEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("temple_c", NULL);
        TEMPLEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = TempleC_Init;
            level->always_update_fn = TempleC_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("temple_status", NULL);
        TEMPLESTATUS_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kashyyyk_A", NULL);
        KASHYYYKA_LDATA = level;
        if (level != NULL) {
            level->init_fn = KashyyykA_Init;
            level->reset_fn = KashyyykA_Reset;
            level->update_fn = KashyyykA_Update;
            level->draw_status_fn = KashyyykA_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kashyyyk_B", NULL);
        KASHYYYKB_LDATA = level;
        if (level != NULL) {
            level->init_fn = KashyyykB_Init;
            level->reset_fn = KashyyykB_Reset;
            level->update_fn = KashyyykB_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kashyyyk_C", NULL);
        KASHYYYKC_LDATA = level;
        if (level != NULL) {
            level->init_fn = KashyyykC_Init;
            level->update_fn = KashyyykC_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Kashyyyk_D", NULL);
        KASHYYYKD_LDATA = level;
        if (level != NULL) {
            level->init_fn = KashyyykD_Init;
            level->reset_fn = KashyyykD_Reset;
            level->update_fn = KashyyykD_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("vader_a", NULL);
        VADERA_LDATA = level;
        if (level != NULL) {
            level->init_fn = VaderA_Init;
            level->reset_fn = VaderA_Reset;
            level->update_fn = VaderA_Update;
            level->draw_status_fn = VaderA_DrawPanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("vader_b", NULL);
        VADERB_LDATA = level;
        if (level != NULL) {
            level->init_fn = VaderB_Init;
            level->reset_fn = VaderB_Reset;
            level->update_fn = VaderB_Update;
            level->draw_status_fn = VaderB_DrawPanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("vader_c", NULL);
        VADERC_LDATA = level;
        if (level != NULL) {
            level->init_fn = VaderC_Init;
            level->reset_fn = VaderC_Reset;
            level->update_fn = VaderC_Update;
            level->draw_status_fn = VaderC_DrawPanel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("BlockadeRunner_B", NULL);
        BLOCKADERUNNERB_LDATA = level;
        if (level != NULL) {
            level->init_fn = BlockadeRunnerB_Init;
            level->update_fn = BlockadeRunnerB_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("BlockadeRunner_C", NULL);
        BLOCKADERUNNERC_LDATA = level;
        if (level != NULL) {
            level->init_fn = BlockadeRunnerC_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("BlockadeRunner_D", NULL);
        BLOCKADERUNNERD_LDATA = level;
        if (level != NULL) {
            level->reset_fn = BlockadeRunnerD_Reset;
            level->update_fn = BlockadeRunnerD_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("tatooine_b", NULL);
        TATOOINEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = TatooineB_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("tatooine_c", NULL);
        TATOOINEC_LDATA = level;
        level->init_fn = TatooineC_Init;
    }

    {
        LEVELDATA *level = Level_FindByName("moseisley_a", NULL);
        MOSEISLEYA_LDATA = level;
        if (level != NULL) {
            level->init_fn = MosEisleyA_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("moseisley_b", NULL);
        MOSEISLEYB_LDATA = level;
        if (level != NULL) {
            level->init_fn = MosEisleyB_Init;
            level->update_fn = MosEisleyB_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("moseisley_c", NULL);
        MOSEISLEYC_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("moseisley_d", NULL);
        MOSEISLEYD_LDATA = level;
        if (level != NULL) {
            level->init_fn = MosEisleyD_Init;
            level->always_update_fn = MosEisleyD_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("moseisley_e", NULL);
        MOSEISLEYE_LDATA = level;
        if (level != NULL) {
            level->init_fn = MosEisleyE_Init;
            level->reset_fn = MosEisleyE_Reset;
            level->update_fn = MosEisleyE_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("tatooine_a", NULL);
        TATOOINEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = TatooineA_Init;
            level->update_fn = TatooineA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("tatooine_b", NULL);
        TATOOINEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("tatooine_d", NULL);
        TATOOINED_LDATA = level;
        if (level != NULL) {
            level->init_fn = TatooineD_Init;
            level->update_fn = TatooineD_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("tatooine_e", NULL);
        TATOOINEE_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarrescue_a", NULL);
        DEATHSTARRESCUEA_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarrescue_b", NULL);
        DEATHSTARRESCUEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStarRescueB_Init;
            level->update_fn = DeathStarRescueB_Update;
            level->always_update_fn = DeathStarRescueB_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarrescue_c", NULL);
        DEATHSTARRESCUEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStarRescueC_Init;
            level->always_update_fn = DeathStarRescueC_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarrescue_d", NULL);
        DEATHSTARRESCUED_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarrescue_e", NULL);
        DEATHSTARRESCUEE_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarescape_a", NULL);
        DEATHSTARESCAPEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStarEscapeA_Init;
            level->update_fn = DeathStarEscapeA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarescape_b", NULL);
        DEATHSTARESCAPEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStarEscapeB_Init;
            level->update_fn = DeathStarEscapeB_Update;
            level->always_update_fn = DeathStarEscapeB_AlwaysUpdate;
            level->draw_fn = DeathStarEscapeB_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarescape_c", NULL);
        DEATHSTARESCAPEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStarEscapeC_Init;
            level->update_fn = DeathStarEscapeC_Update;
            level->reset_fn = DeathStarEscapeC_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarescape_d", NULL);
        DEATHSTARESCAPED_LDATA = level;
        if (level != NULL) {
            level->update_fn = DeathStarEscapeD_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarbattle_a", NULL);
        DEATHSTARBATTLEA_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarbattle_b", NULL);
        DEATHSTARBATTLEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarbattle_c", NULL);
        DEATHSTARBATTLEC_LDATA = level;
        if (level != NULL) {
            level->always_update_fn = DeathStarBattleC_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("deathstarbattle_d", NULL);
        DEATHSTARBATTLED_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStarBattleDInit;
            level->update_fn = DeathStarBattleDUpdate;
            level->draw_fn = DeathStarBattleDDraw;
            level->reset_fn = DeathStarBattleDReset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothbattle_a", NULL);
        HOTHBATTLEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = HothBattleA_Init;
            level->reset_fn = HothBattleA_Reset;
            level->update_fn = HothBattleA_Update;
            level->draw_fn = HothBattleA_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothbattle_b", NULL);
        HOTHBATTLEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothbattle_c", NULL);
        HOTHBATTLEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = HothBattleC_Init;
            level->reset_fn = HothBattleC_Reset;
            level->update_fn = HothBattleC_Update;
            level->draw_fn = HothBattleC_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothbattle_d", NULL);
        HOTHBATTLED_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothbattle_e", NULL);
        HOTHBATTLEE_LDATA = level;
        if (level != NULL) {
            level->init_fn = HothBattleE_Init;
            level->update_fn = HothBattleE_Update;
            level->draw_fn = HothBattleE_Draw;
            level->draw_status_fn = HothBattleE_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("HothBattle_Outro", NULL);
        HOTHBATTLEOUTRO_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothescape_a", NULL);
        HOTHESCAPEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = HothEscapeA_Init;
            level->update_fn = HothEscapeA_Update;
            level->reset_fn = HothEscapeA_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothescape_b", NULL);
        HOTHESCAPEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = HothEscapeB_Init;
            level->update_fn = HothEscapeB_Update;
            level->reset_fn = HothEscapeB_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothescape_c", NULL);
        HOTHESCAPEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = HothEscapeC_Init;
            level->update_fn = HothEscapeC_Update;
            level->always_update_fn = HothEscapeC_AlwaysUpdate;
            level->reset_fn = HothEscapeC_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("hothescape_d", NULL);
        HOTHESCAPED_LDATA = level;
        if (level != NULL) {
            level->init_fn = HothEscapeD_Init;
            level->update_fn = HothEscapeD_Update;
            level->reset_fn = HothEscapeD_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("AsteroidChase_A", NULL);
        ASTEROIDCHASEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = AsteroidChaseA_Init;
            level->reset_fn = AsteroidChaseA_Reset;
            level->update_fn = AsteroidChaseA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("AsteroidChase_B", NULL);
        ASTEROIDCHASEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = AsteroidChaseB_Init;
            level->reset_fn = AsteroidChaseB_Reset;
            level->update_fn = AsteroidChaseB_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("AsteroidChase_C", NULL);
        ASTEROIDCHASEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = AsteroidChaseC_Init;
            level->reset_fn = AsteroidChaseC_Reset;
            level->update_fn = AsteroidChaseC_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("AsteroidChase_D", NULL);
        ASTEROIDCHASED_LDATA = level;
        if (level != NULL) {
            level->init_fn = AsteroidChaseD_Init;
            level->update_fn = AsteroidChaseD_Update;
            level->draw_fn = AsteroidChaseD_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("AsteroidChase_Midtro", NULL);
        ASTEROIDCHASEMITRO_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("dagobah_a", NULL);
        DAGOBAHA_LDATA = level;
        if (level != NULL) {
            level->init_fn = DagobahA_Init;
            level->update_fn = DagobahA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("dagobah_d", NULL);
        DAGOBAHD_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("dagobah_e", NULL);
        DAGOBAHE_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("dagobah_e", NULL);
        DAGOBAHE_LDATA = level;
        if (level != NULL) {
            level->init_fn = DagobahE_Init;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("dagobah_b", NULL);
        DAGOBAHB_LDATA = level;
        if (level != NULL) {
            level->init_fn = DagobahB_Init;
            level->reset_fn = DagobahB_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("dagobah_c", NULL);
        DAGOBAHC_LDATA = level;
        if (level != NULL) {
            level->init_fn = DagobahC_Init;
            level->draw_status_fn = DagobahC_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("CloudCityTrap_A", NULL);
        CLOUDCITYTRAPA_LDATA = level;
        if (level != NULL) {
            level->init_fn = CloudCityTrapA_Init;
            level->update_fn = CloudCityTrapA_Update;
            level->reset_fn = CloudCityTrapA_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("CloudCityTrap_B", NULL);
        CLOUDCITYTRAPB_LDATA = level;
        if (level != NULL) {
            level->init_fn = CloudCityTrapB_Init;
            level->update_fn = CloudCityTrapB_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("CloudCityTrap_C", NULL);
        CLOUDCITYTRAPC_LDATA = level;
        if (level != NULL) {
            level->reset_fn = CloudCityTrapC_Reset;
            level->update_fn = CloudCityTrapC_Update;
            level->draw_status_fn = CloudCityTrapC_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("CloudCityTrap_Outro", NULL);
        CLOUDCITYTRAPOUTRO_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("cloudcityescape_a", NULL);
        CLOUDCITYESCAPEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = CloudCityEscapeA_Init;
            level->reset_fn = CloudCityEscapeA_Reset;
            level->update_fn = CloudCityEscapeA_Update;
            level->draw_status_fn = CloudCityEscapeA_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("cloudcityescape_b", NULL);
        CLOUDCITYESCAPEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("cloudcityescape_c", NULL);
        CLOUDCITYESCAPEC_LDATA = level;
        if (level != NULL) {
            level->init_fn = CloudCityEscapeC_Init;
            level->update_fn = CloudCityEscapeC_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("jabbaspalace_a", NULL);
        JABBASPALACEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = JabbasPalaceA_Init;
            level->reset_fn = JabbasPalaceA_Reset;
            level->update_fn = JabbasPalaceA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("jabbaspalace_b", NULL);
        JABBASPALACEB_LDATA = level;
        if (level != NULL) {
            level->init_fn = JabbasPalaceB_Init;
            level->reset_fn = JabbasPalaceB_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("jabbaspalace_d", NULL);
        JABBASPALACED_LDATA = level;
        if (level != NULL) {
            level->reset_fn = JabbasPalaceD_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("jabbaspalace_e", NULL);
        JABBASPALACEE_LDATA = level;
        if (level != NULL) {
            level->init_fn = JabbasPalaceE_Init;
            level->reset_fn = JabbasPalaceE_Reset;
            level->update_fn = JabbasPalaceE_Update;
            level->draw_status_fn = JabbasPalaceE_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("jabbaspalace_Outro", NULL);
        JABBASPALACE_OUTRO_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("endorbattle_a", NULL);
        ENDORBATTLEA_LDATA = level;
        if (level != NULL) {
            level->init_fn = EndorBattleA_Init;
            level->update_fn = EndorBattleA_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("endorbattle_b", NULL);
        ENDORBATTLEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("endorbattle_c", NULL);
        ENDORBATTLEC_LDATA = level;
        level->init_fn = EndorBattleC_Init;
    }

    {
        LEVELDATA *level = Level_FindByName("endorbattle_d", NULL);
        ENDORBATTLED_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("DeathStar2Battle_A", NULL);
        DEATHSTAR2BATTLEA_LDATA = level;
        if (level != NULL) {
            level->always_update_fn = DeathStar2BattleA_AlwaysUpdate;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("DeathStar2Battle_B", NULL);
        DEATHSTAR2BATTLEB_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("DeathStar2Battle_D", NULL);
        DEATHSTAR2BATTLED_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStar2BattleD_Init;
            level->update_fn = DeathStar2BattleD_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("DeathStar2Battle_E", NULL);
        DEATHSTAR2BATTLEE_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStar2BattleFire_Init;
            level->update_fn = DeathStar2BattleFire_Update;
            level->draw_fn = DeathStar2BattleFire_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("DeathStar2Battle_F", NULL);
        DEATHSTAR2BATTLEF_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStar2BattleFire_Init;
            level->update_fn = DeathStar2BattleFire_Update;
            level->draw_fn = DeathStar2BattleFire_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("DeathStar2Battle_G", NULL);
        DEATHSTAR2BATTLEG_LDATA = level;
        if (level != NULL) {
            level->init_fn = DeathStar2BattleFire_Init;
            level->update_fn = DeathStar2BattleFire_Update;
            level->draw_fn = DeathStar2BattleFire_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("DeathStar2Battle_Midtro", NULL);
        DEATHSTAR2BATTLEMIDTRO_LDATA = level;
        if (level != NULL) {
        }
    }

    {
        LEVELDATA *level = Level_FindByName("EmperorFight_A", NULL);
        EMPERORFIGHTA_LDATA = level;
        if (level != NULL) {
            level->init_fn = EmperorFightA_Init;
            level->reset_fn = EmperorFightA_Reset;
            level->update_fn = EmperorFightA_Update;
            level->draw_status_fn = EmperorFightA_Panel;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("SarlaccPit_A", NULL);
        SARLACCPITA_LDATA = level;
        if (level != NULL) {
            level->reset_fn = SarlaccPitA_Reset;
            level->draw_fn = SarlaccPitA_Draw;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("SarlaccPit_B", NULL);
        SARLACCPITB_LDATA = level;
        if (level != NULL) {
            level->init_fn = SarlaccPitB_Init;
            level->reset_fn = SarlaccPitB_Reset;
            level->update_fn = SarlaccPitB_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("SarlaccPit_C", NULL);
        SARLACCPITC_LDATA = level;
        if (level != NULL) {
            level->init_fn = SarlaccPitC_Init;
            level->reset_fn = SarlaccPitC_Reset;
            level->update_fn = SarlaccPitC_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("lego_city", NULL);
        LEGOCITY_LDATA = level;
        if (level != NULL) {
            level->init_fn = LegoCity_Init;
            level->reset_fn = LegoCity_Reset;
            level->update_fn = LegoCity_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("Senate_a", NULL);
        SENATEA_LDATA = level;
        level->init_fn = SenateA_Init;
    }

    {
        LEVELDATA *level = Level_FindByName("new_town", NULL);
        NEWTOWN_LDATA = level;
        if (level != NULL) {
            level->init_fn = NewTown_Init;
            level->reset_fn = NewTown_Reset;
            level->update_fn = NewTown_Update;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("platform", NULL);
        PLATFORM_LDATA = level;
        if (level != NULL) {
            level->init_fn = Platform_Init;
            level->reset_fn = Platform_Reset;
        }
    }

    {
        LEVELDATA *level = Level_FindByName("E1CharacterBonus_A", NULL);
        E1CHARACTERBONUSA_LDATA = level;
        level->init_fn = E1CharacterBonus_Init;
    }

    {
        LEVELDATA *level = Level_FindByName("E2VehicleBonus_A", NULL);
        E2VEHICLEBONUSA_LDATA = level;
        if (level != NULL) {
        }
    }

    LEVELDATA *entry = LDataList;
    for (i32 i = 0; i < LEVELCOUNT; ++i, ++entry) {
        if ((entry->flags & LEVEL_STATUS) != 0 || entry == STATUS_LDATA) {
            entry->update_fn = UpdateStatusScreen;
            entry->draw_status_fn = DrawStatusScreen;
        }
    }
}

void Level_SetDefaults(LEVELDATA *level) {
    level->max_antinodes = 256;
    level->max_gizmo_blowups = 64;
    level->max_gizmo_blowup_types = 64;
    level->max_pickups = 256;
    level->max_obstacle_objs = 128;
    level->max_buildit_objs = 128;
    level->max_force_objs = 128;
    level->max_bombgen_objs = 16;

    level->max_tightropes = 10;
    level->max_giz_timers = 8;
    level->max_signals = 10;
    level->max_levers = 10;
    level->max_technos = 10;
    level->max_zipups = 10;
    level->max_grapples = 10;
    level->max_obstacles = 32;
    level->max_buildits = 16;
    level->max_shards = 64;
    level->max_spinners = 4;
    level->max_minicuts = 8;
    level->max_minicut_parts = 2;
    level->max_giz_specials = 32;
    level->max_attractos = 5;
    level->max_climb_objs = 10;
    level->max_guidelines = 32;
    level->max_ledges = 96;
    level->max_security_doors = 5;
    level->max_tubes = 5;
    level->max_giz_panels = 8;
    level->max_hat_machines = 2;
    level->max_force = 32;
    level->max_push_blocks = 16;
    level->max_push_block_end_pos = 8;
    level->max_doors = 16;
    level->max_teleports = 4;
    level->max_giz_randoms = 8;
    level->max_torp_machines = 2;
    level->max_spinner_anim_objs = 40;

    level->wind_size = 512.0f;
    level->wind_speed = 3.0f;

    level->max_turrets = 32;
    level->max_bombgens = 8;
    level->max_bridges = 5;
    level->max_plugs = 10;
}

void FixUpAreas(void) {
    Areas_FixUp(AreaFixUp_LSW);
}

void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *) {
    if (world->current_level == TITLES_LDATA) {
        *menu_id = 0;
    } else if (world->current_level == CREDITS_LDATA) {
        *menu_id = 30;
    }
}

void Game_WorldInfo_InitLast(WORLDINFO *world) {
    GizSpinners_InitTerrain(world);
    Signals_InitTerrain(world);
    Attractos_InitTerrain(world);
    SecurityDoors_InitTerrain(world);
    Levers_InitTerrain(world);
    GizPanel_InitTerrain(world);
    HatMachines_InitTerrain(world);
    GizmoBlowupsFinalSetup(world);
    InitClimbObjectSys(world);
    GizmoPushBlockInitAndReset(world, NULL);
}

