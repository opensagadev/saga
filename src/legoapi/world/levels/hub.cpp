// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/config/cheat.h"
#include "globals.h"
#include "legoapi/gizmos/object/gizbuildits.h"

#include "batman.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/core/character.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/render/fx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/net/netplay.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <stdio.h>
#include <string.h>

extern void Customiser_LoadAll(CUSTOMISER *, WORLDINFO_s *);
extern void Customiser_Init(CUSTOMISER *);
extern void Customiser_Reset(CUSTOMISER *);
extern void Customiser_Draw3D(CUSTOMISER *);
extern void Customiser_Update(CUSTOMISER *, WORLDINFO_s *);
void Hub_ResetPanel();
extern void InitShop(WORLDINFO_s *);
extern void DrawShop3D(WORLDINFO_s *);
extern i32 Draw3DObjectMtx(WORLDINFO_s *, i32, NUMTX *);
extern void CutScenePlayer_Reset();
extern FadeSystem FadeSys;
extern GAMESAVE_s TempGame;
extern i32 shop_save_done;
extern i32 shop_quit;
extern f32 ShopLockedScale;
extern HINTSYS_s hintsys;
extern i32 only_process_this_hint_id;
extern u8 show_unlock_shop_hint;
extern void *GetHintFromUIButton();
extern "C" void NuIOS_RecordFlurryEvent(char *);
bool HubShopUnlocked();
bool HubCustomiserUnlocked();
extern i32 Customiser_MenuAvailable(CUSTOMISER *);
extern GAMESAVE_s OldCustomiseGame;
extern i32 customiser_save_done;
extern i32 customiser_quit;
extern i32 customiser_changed;
extern u8 show_unlock_customiser_hint;
extern i32 Missions_PartyAvailable(MISSIONSYS_s *);
extern "C" i32 TriggerAutoSave();
extern AREADATA *E1VEHICLE_ADATA;
extern u8 show_unlock_minikitviewer_hint;
bool HubMinikitViewerUnlocked();
void Hub_ActivateDoorMenu(LEVELDATA_s **);
extern "C" void PlaySfxAndSetPitch(char *, NUVEC *, f32);
AILOCATOR_s *LocalGetNearestLocator(AILOCATOR_s **, i32, f32, NUVEC *, f32, i32, f32, f32);
extern i32 Episode_CountOpenAreas(i32, i32, AREASAVE_s *);
extern void UpdateCharacterLoad();
extern void ResetForceBack();
extern void Hint_CancelCurrent();
extern f32 SeekLinearF(f32, f32, f32);
extern i32 qrand();
extern void Door_GoThrough(WORLDINFO_s *, DOOR_s *, i32);
extern GameObject_s *FindGameObject(i32, u32, i32, i32, i32);
extern void BackDrop_ResetColours();
extern "C" f32 MainRenderTargetTime;
extern void MakeFreePlayModelList(i32 first_model, i32 second_model, i32 area, i32 level, i32 include_bonus);
extern void GameDrawMenuEntry(MENU *menu, char *text);
extern i32 GameAudio_GetSfxId(i32 sfx);
extern i32 MenuSFX;
extern char **TTab;
extern i16 tPLAYER1;
extern i16 tPLAYER2;
extern i16 tPLAY;
extern f32 ICONX;
extern f32 ICONSIZE;
extern i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);

// These two arrays are generic level-loader state rather than hub-owned state.
extern u64 LevHSpecialExists;
extern GIZBUILDIT_s *LevBuildIt[4];

i32 hub_from_superstory = -1;
i32 hub_from_mission = -1;
i32 hub_startoutsidebonusdoor_area = -1;
i32 hub_from_arcade = -1;
i32 hub_from_cutsceneplayer = 0;
i32 hub_goto_clipsmenu_episode = -1;
void *HubStartDoor = NULL;

typedef void (*HUBCALLBACK)(WORLDINFO_s *);

HUBCALLBACK Hub_UpdateAIFn = NULL;
HUBCALLBACK Hub_ResetAIFn = NULL;
HUBCALLBACK Hub_InitAIFn = NULL;

u8 Hub_LowEnd_IconsInsteadOfModels = 0;
f32 Hub_PreventDropOutTime = 0.0f;
f32 Hub_HologramTargetAlpha = 0.0f;
f32 Hub_HologramAlpha = 0.0f;
f32 hub_jabbaawake = 0.0f;
i32 hub_new_level = 0;
f32 hub_buildit_time = 0.0f;
i32 hub_buildit = 0;
f32 hub_area_time = 0.0f;
i32 hub_area = 0;
f32 hub_episode_time = 0.0f;
i32 hub_episode = 0;
f32 hub_minikitarea_opentime = 0.0f;
f32 hub_minikitarea_time = 0.0f;
i32 hub_minikitarea = 0;
i32 last_hub_area = 0;
f32 statstime = 0.0f;
f32 cointotaltime = 0.0f;
f32 goldbricktime = 0.0f;
u8 hub_custodians_finished_loading = 0;
i32 hub_freeplay_area = 0;
f32 COLLECTION_Y_HUB = -0.3f;
i32 freeplaymode = 0;
i32 hub_selectmode = 0;
f32 selectmodetime = 0.0f;
i32 selectmodemode = 0;
f32 episodesduration;
f32 episodestime;
i8 lastepisodesmode;
i8 episodesmode;
i8 i_episodes;
i8 i_clip[6];
i32 freeplay_selected[2] = {};
f32 uprepeattime[2] = {};
f32 rightrepeattime[2] = {};
f32 downrepeattime[2] = {};
f32 leftrepeattime[2] = {};
u8 uprepeatcount[2] = {};
u8 rightrepeatcount[2] = {};
u8 downrepeatcount[2] = {};
u8 leftrepeatcount[2] = {};
u8 hub_makefreeplaylist_addotherid = 0;

static i32 buildits_reset = 0;
static GIZMO *hub_minikitviewer_gizmo = NULL;
NUGSPLINE *hub_minikitviewer_camspl = NULL;
static f32 freeplaytime = 0.0f;
static f32 freeplayduration = 0.0f;
static f32 freeplay_time[2] = {};
static f32 BigIconAlpha[2];
static i32 freeplay_i[2];
static f32 selectmodeduration = 0.0f;
static i32 hub_bonusarea;
static i32 hub_bonusepisode;
static i32 bonusmodemode;
static f32 bonusmodetime;
static f32 bonusmodeduration;
static i32 fpcount = 0;
static APICHARACTERMODELLIST_s fplist[341] = {};
static f32 stats_xscale = 1.0f;
static i32 TJTYPEA = 96;
static i32 hub_drawminikitcount_charkit = 0;
static i8 i_selectminikitepisode = 0;
static i32 hub_minikitviewer_area = 0;
static f32 hub_minikitviewer_alpha = 0.0f;
extern f32 Hub_PadSpeed[2];
extern u16 Hub_PadAngle[2];
f32 hub_minikitviewer_movewait;
f32 hub_minikitviewer_move;
static NUVEC hub_minikitviewer_pos = {-27.0f, 0.1f, -22.125f};
void NewRumbleAllPlayers(f32, f32, i32, i32);
extern "C" void PlaySfx(char *, NUVEC *);
void PushAway(NUVEC *, f32, NUVEC *, NUVEC *, GameObject_s *, GameObject_s *, f32, u32);

static char *EpisodeNumerals[6] = {"I", "II", "III", "IV", "V", "VI"};

u32 HUB_EPISODER = 255;
u32 HUB_EPISODEG = 191;
u32 HUB_EPISODEB = 0;
f32 HUB_EPISODETITLESIZE = 0.5f;
f32 HUB_EPISODESUBTITLESIZE = 0.6f;

extern f32 HUB_EPISODETITLEY, HUB_EPISODESUBTITLEY;
extern f32 PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITSCALE, PANEL_MINIKITY, PANEL_MINIKITCOUNTY, PANEL3DMULX;
extern i32 newgamecam;
extern f32 newgamecamtime;
extern i16 tMAP, tSTORY, tFREEPLAY, tREPLAYSTORY, tCHAPTER, tBONUS2, tMINIKITS, tMINIKIT;
extern i16 tBOUNTYHUNTERMISSIONS, tBOUNTYHUNTERMISSIONS2, tSTORYCLIPS2;
extern i32 EpMiniKitCount, EpMiniKitTotal, EpCharKitCount, EpCharKitTotal, EpBuildUpCount, EpBuildUpTotal;
extern i32 EpStoryBuildUpCount, EpStoryBuildUpTotal, EpFreePlayBuildUpCount, EpFreePlayBuildUpTotal;
extern i32 EpRedBrickCount, EpRedBrickTotal, EpGoldBrickCount, EpGoldBrickTotal, EpCompleteCount;
extern u8 MENUNORMALR, MENUNORMALG, MENUNORMALB, MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R, MENUFLASH0G, MENUFLASH0B, MENUFLASH1R, MENUFLASH1G, MENUFLASH1B;
extern f32 menu_pulse, menu_pulsate;
extern i32 menu_flash;
extern AREADATA *SENATE_ADATA;
void DrawShopPanel();
i32 Missions_NumCompleted(MISSIONSYS *, MISSIONSAVE *, i32);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
void Hub_DrawAreaStats(f32, i32, i32);
static void Hub_DrawMiniKitCount(f32, f32, i32, i32, f32);
static void Hub_DrawSuperBonusStats(AREADATA *, f32);
static void Hub_DrawArcadeStats(f32);
static void Hub_DrawBonusModeMenu(i32, f32);
static void Hub_DrawSelectModeMenu(i32, f32);
void Collection_Draw(COLLECTION_s *, f32, f32, f32, APICHARACTERMODELLIST_s *, f32, i32);

// Retail only has the IPA-SRA clone (the unused fourth argument dropped).
static __attribute__((noinline, noclone)) void
Hub_DrawBonusStats(f32 alpha, i32 area, i32 episode) __asm__("_ZL18Hub_DrawBonusStatsfiii.isra.3");

static void Hub_DrawBonusStats(f32 alpha, i32 area, i32 episode);

static const NUVEC Hub_PercentPos = {-26.713f, 0.7f, -48.777f};
static const NUVEC ClipsSignOffset = {-0.114f, -0.0385f, -0.045f};

static inline void Hub_SetStatsTextMtx(NUMTX *mtx, i32 angle) {
    const f32 cosine = NU_COS_LUT(angle);
    const f32 sine = NU_SIN_LUT(angle);
    mtx->m00 = cosine;
    mtx->m01 = 0.0f;
    mtx->m02 = -sine;
    mtx->m03 = 0.0f;
    mtx->m10 = 0.0f;
    mtx->m11 = 1.0f;
    mtx->m12 = 0.0f;
    mtx->m13 = 0.0f;
    mtx->m20 = sine;
    mtx->m21 = 0.0f;
    mtx->m22 = cosine;
    mtx->m23 = 0.0f;
    mtx->m30 = 0.0f;
    mtx->m31 = 0.0f;
    mtx->m32 = 0.0f;
    mtx->m33 = 1.0f;
}

void Hub_ClearStats();
void Hub_UpdateMiniKits(WORLDINFO_s *);
void Hub_InitFreePlaySelect(i32, i32, i32);
void WipeBackToHub();
static void Hub_MakeFreePlayList(i32 first_model, i32 second_model);

enum HUB_DOOR_MENU_ID {
    HUB_DOOR_MENU_STANDARD = 15,
    HUB_DOOR_MENU_BONUS = 16,
    HUB_DOOR_MENU_VEHICLE = 17,
    HUB_DOOR_MENU_EPISODE_BONUS = 18,
};

enum HUB_VEHICLE_COLLECTION_MASK : u32 {
    HUB_VEHICLE_COLLECTION_ALLOWED = 0x04002000,
    HUB_VEHICLE_COLLECTION_REQUIRED = 0x00002000,
};

enum HUB_FREEPLAY_MODEL_FLAGS : u32 {
    HUB_FREEPLAY_MODEL_VEHICLE = 0x00002000,
    HUB_FREEPLAY_MODEL_MINIKIT = 0x04000000,
};

enum HUB_AUDIO_EVENT {
    HUB_AUDIO_EVENT_OPEN_DOOR_MENU = 0x2d,
};

enum {
    HUB_VEHICLE_ID_CAPACITY = 344,
};

struct HUBAREAINFO_s {
    const char *area_name;
    const char *door_name;
    const char *lock_name;
    const char *bonus_gizmo_name;
    const char *bonus_gizmo_name_2;
    f32 panel_offset;
    f32 panel_scale;
    u8 flags;
    u8 force_open;
    AREADATA_s *area;
    GIZMO *door;
    nuhspecial_s lock;
    GIZMO *bonus_gizmo;
    GIZMO *bonus_gizmo_2;
};

struct HUBEPISODEINFO_s {
    i16 episode;
    i16 flags;
    const char *door_name;
    const char *lock_on_name;
    const char *lock_off_name;
    u8 force_open;
    EPISODEDATA *data;
    GIZMO *door;
    nuhspecial_s lock_on;
    nuhspecial_s lock_off;
};

#if UINTPTR_MAX == 0xffffffff
DECOMP_ASSERT(sizeof(HUBAREAINFO_s) == 0x3c, "HUBAREAINFO_s size");
DECOMP_ASSERT(offsetof(HUBAREAINFO_s, force_open) == 0x1d, "HUBAREAINFO_s force_open offset");
DECOMP_ASSERT(offsetof(HUBAREAINFO_s, area) == 0x20, "HUBAREAINFO_s area offset");
DECOMP_ASSERT(sizeof(HUBEPISODEINFO_s) == 0x34, "HUBEPISODEINFO_s size");
#endif

// The original table is ordered exactly like areas.txt.  The last two entries
// are the network/bonus doors and the null record terminates every hub scan.
static HUBAREAINFO_s HubAreaInfo[] = {{"negotiations", "de1_1", "lock_1_1_on", NULL, NULL, 0.1f, 1.0f, 1},
                                      {"gungan", "de1_2", "lock_1_2_on", NULL, NULL, 0.025f, 0.8f, 1},
                                      {"palacerescue", "de1_3", "lock_1_3_on", NULL, NULL, 0.0f, 0.5f, 1},
                                      {"podsprint", "de1_4", "lock_1_4_on", NULL, NULL, 0.025f, 1.0f, 1},
                                      {"retakepalace", "de1_5", "lock_1_5_on", NULL, NULL, 0.075f, 1.0f, 1},
                                      {"maul", "de1_6", "lock_1_6_on", NULL, NULL, 0.225f, 1.0f, 1},
                                      {"e1vehiclebonus", "de1_7", "lock_1_7_on", NULL, NULL, 0.0f, 1.0f, 1},
                                      {"pursuit", "de2_1", "lock_2_1_on", NULL, NULL, 0.125f, 1.0f, 1},
                                      {"kamino", "de2_2", "lock_2_2_on", NULL, NULL, 0.075f, 1.0f, 1},
                                      {"factory", "de2_3", "lock_2_3_on", NULL, NULL, 0.25f, 1.0f, 1},
                                      {"jedi", "de2_4", "lock_2_4_on", NULL, NULL, 0.0125f, 1.0f, 1},
                                      {"gunship", "de2_5", "lock_2_5_on", NULL, NULL, 0.11f, 1.0f, 1},
                                      {"dooku", "de2_6", "lock_2_6_on", NULL, NULL, 0.42f, 0.5f, 1},
                                      {"e2vehiclebonus", "de2_7", "lock_2_7_on", NULL, NULL, 0.0f, 1.0f, 1},
                                      {"dogfight", "de3_1", "lock_3_1_on", NULL, NULL, 0.275f, 1.0f, 1},
                                      {"cruiser", "de3_2", "lock_3_2_on", NULL, NULL, 0.02f, 1.0f, 1},
                                      {"grievous", "de3_3", "lock_3_3_on", NULL, NULL, 0.1375f, 1.0f, 1},
                                      {"kashyyyk", "de3_4", "lock_3_4_on", NULL, NULL, 0.01f, 1.0f, 1},
                                      {"temple", "de3_5", "lock_3_5_on", NULL, NULL, 0.025f, 1.0f, 1},
                                      {"vader", "de3_6", "lock_3_6_on", NULL, NULL, 0.21f, 1.0f, 1},
                                      {"e3vehiclebonus", "de3_7", "lock_3_7_on", NULL, NULL, 0.0f, 1.0f, 1},
                                      {"blockaderunner", "de4_1", "lock_4_1_on", NULL, NULL, 0.16f, 1.0f, 1},
                                      {"tatooine", "de4_2", "lock_4_2_on", NULL, NULL, 0.01f, 1.0f, 1},
                                      {"moseisley", "de4_3", "lock_4_3_on", NULL, NULL, 0.02f, 1.0f, 1},
                                      {"deathstarrescue", "de4_4", "lock_4_4_on", NULL, NULL, 0.17f, 1.0f, 1},
                                      {"deathstarescape", "de4_5", "lock_4_5_on", NULL, NULL, 0.025f, 1.0f, 1},
                                      {"deathstarbattle", "de4_6", "lock_4_6_on", NULL, NULL, 0.165f, 1.0f, 1},
                                      {"e4vehiclebonus", "de4_7", "lock_4_7_on", NULL, NULL, 0.0f, 1.0f, 1},
                                      {"hothbattle", "de5_1", "lock_5_1_on", NULL, NULL, 0.0f, 1.0f, 1},
                                      {"hothescape", "de5_2", "lock_5_2_on", NULL, NULL, 0.01f, 1.0f, 1},
                                      {"asteroidchase", "de5_3", "lock_5_3_on", NULL, NULL, 0.1825f, 1.0f, 1},
                                      {"dagobah", "de5_4", "lock_5_4_on", NULL, NULL, 0.165f, 1.0f, 1},
                                      {"cloudcitytrap", "de5_5", "lock_5_5_on", NULL, NULL, 0.1f, 1.0f, 1},
                                      {"cloudcityescape", "de5_6", "lock_5_6_on", NULL, NULL, 0.0125f, 1.0f, 1},
                                      {"e5vehiclebonus", "de5_7", "lock_5_7_on", NULL, NULL, 0.0f, 1.0f, 1},
                                      {"jabbaspalace", "de6_1", "lock_6_1_on", NULL, NULL, 0.01f, 1.0f, 1},
                                      {"sarlaccpit", "de6_2", "lock_6_2_on", NULL, NULL, 0.01f, 0.8f, 1},
                                      {"speederchase", "de6_3", "lock_6_3_on", NULL, NULL, 0.165f, 1.0f, 1},
                                      {"endorbattle", "de6_4", "lock_6_4_on", NULL, NULL, 0.01f, 1.0f, 1},
                                      {"emperorfight", "de6_5", "lock_6_5_on", NULL, NULL, 0.245f, 1.0f, 1},
                                      {"deathstar2battle", "de6_6", "lock_6_6_on", NULL, NULL, 0.1825f, 1.0f, 1},
                                      {"e6vehiclebonus", "de6_7", "lock_6_7_on", NULL, NULL, 0.0f, 1.0f, 1},
                                      {"podrace", "de7_1", "lock_7_1_on", "frame_1", NULL, 0.0f, 0.0f, 1},
                                      {"anakinsflight", "de7_2", "lock_7_2_on", "frame_2", NULL, 0.0f, 0.0f, 1},
                                      {"bonus_gunship", "de7_3", "lock_7_3_on", "frame_3", NULL, 0.0f, 0.0f, 1},
                                      {"anewhope", "de7_4", "lock_7_4_on", "frame_4", NULL, 0.0f, 0.0f, 1},
                                      {"bonus2", "de7_5", "lock_7_5_on", "frame_5", NULL, 0.0f, 0.0f, 1},
                                      {"bonus", "de7_6", "lock_7_6_on", "frame_6", NULL, 0.0f, 0.0f, 1},
                                      {"losttemple", "de7_7", "lock_7_7_on", NULL, NULL, 0.0f, 0.0f, 1, 1},
                                      {"senate", "network_door", NULL, NULL, NULL, 0.0f, 0.0f, 1, 1},
                                      {}};

static HUBEPISODEINFO_s HubEpisodeInfo[] = {
    {0, 1, "DE1", "lock_1_on", "lock_1_off"},    {1, 1, "DE2", "lock_2_on", "lock_2_off"},
    {2, 1, "DE3", "lock_3_on", "lock_3_off"},    {3, 1, "DE4", "lock_4_on", "lock_4_off"},
    {4, 1, "DE5", "lock_5_on", "lock_5_off"},    {5, 1, "DE6", "lock_6_on", "lock_6_off"},
    {8, 1, "DE7", "lock_7_on", "lock_7_off", 1}, {6, 2, "hologram", "lock_8_on", "lock_8_off"},
    {7, 2, "jabba", "lock_B_on", "lock_B_off"},  {-1}};

f32 HUB_AREAPANELX_ONETRUEJEDIGOLDBRICK[6] = {-0.3f, 0.0f, 0.6f, -0.6f, 0.0f, 0.3f};
static f32 HUB_AREAPANELX_TWOTRUEJEDIGOLDBRICKS[6] = {-0.15f, -0.45f, 0.75f, -0.75f, 0.45f, 0.15f};
static f32 HUB_AREAPANELX_1TRUEJEDIGB_NOCHALLENGE[6] = {-0.201f, 0.201f, 0.6f, -0.6f, 0.0f, 0.0f};
f32 *HUB_AREAPANELX = HUB_AREAPANELX_ONETRUEJEDIGOLDBRICK;

void Hub_Draw3D(WORLDINFO_s *world);

i32 Hub_InMenu();

static void Hub_UpdatePackObstacle(i32 pack) {
    if (LevGizObst[12 - pack] == NULL) {
        return;
    }
    if (!Store_IsPackUnlocked(pack)) {
        GIZOBSTACLE_s *obstacle = LevGizObst[12 - pack];
        obstacle->progress_flags &= static_cast<u8>(~1);
        obstacle->runtime_flags |= 8;
    } else {
        GIZOBSTACLE_s *obstacle = LevGizObst[12 - pack];
        obstacle->progress_flags |= 1;
        obstacle->runtime_flags &= static_cast<u8>(~8);
    }
}

static void Hub_ResetBuildIts() {
    i32 buildit_index = 0;
    for (i32 i = 0; HubAreaInfo[i].area_name != NULL; ++i) {
        if (HubAreaInfo[i].bonus_gizmo != NULL) {
            if ((Game.field_0x7c26[2] & (1 << buildit_index)) != 0) {
                GizBuildIt_SetToEnd(static_cast<GIZBUILDIT_s *>(HubAreaInfo[i].bonus_gizmo->object));
            }
            ++buildit_index;
        }
    }
    if ((Game.field_0x7c26[2] & 0x80) != 0 && LevGizmo[0] != NULL) {
        GizBuildIt_SetToEnd(static_cast<GIZBUILDIT_s *>(LevGizmo[0]->object));
    }
}

void Hub_Update(WORLDINFO_s *world);

void Hub_DrawPanel(WORLDINFO_s *);

i32 Hub_PanelBusy();

void Hub_UpdateKit();

void Hub_CallBarman(GameObject_s *) {
}

void Hub_ClearStats();

i32 hub_forceshopsave = 0;

void Hub_ResetPanel();

bool HubShopUnlocked();

i32 Hub_BonusBuildIt(GIZBUILDIT_s *buildit);

void TurnEpisodeDoorLightsOn(i32 episode);

#include "nu2api/nu3d/nurndr.h"
extern f32 hub_minikitviewer_movewait, hub_minikitviewer_move;
f32 HUB_MINIKITVIEWER_REFLECTY = 0.09f;
i32 RotDiff(u16, u16);
i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);

static inline void MiniKitSetRotationX(NUMTX *m, NUANG a) {
    const f32 cosine = NU_COS_LUT(a);
    const f32 sine = NU_SIN_LUT(a);
    m->m00 = 1.0f;
    m->m11 = cosine;
    m->m12 = sine;
    m->m21 = -sine;
    m->m22 = cosine;
    m->m01 = 0.0f;
    m->m02 = 0.0f;
    m->m03 = 0.0f;
    m->m10 = 0.0f;
    m->m13 = 0.0f;
    m->m20 = 0.0f;
    m->m23 = 0.0f;
    m->m30 = 0.0f;
    m->m31 = 0.0f;
    m->m32 = 0.0f;
    m->m33 = 1.0f;
}

static inline void MiniKitRotateX(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m01 = m->m01;
    f32 m11 = m->m11;
    f32 m21 = m->m21;
    f32 m31 = m->m31;

    m->m01 = m01 * cosx - m->m02 * sinx;
    m->m02 = m01 * sinx + m->m02 * cosx;
    m->m11 = m11 * cosx - m->m12 * sinx;
    m->m12 = m11 * sinx + m->m12 * cosx;
    m->m21 = m21 * cosx - m->m22 * sinx;
    m->m22 = m21 * sinx + m->m22 * cosx;
    m->m31 = m31 * cosx - m->m32 * sinx;
    m->m32 = m31 * sinx + m->m32 * cosx;
}

static inline void MiniKitRotateY(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m10 = m->m10;
    f32 m20 = m->m20;
    f32 m30 = m->m30;

    m->m00 = m00 * cosx + m->m02 * sinx;
    m->m02 = m->m02 * cosx - m00 * sinx;
    m->m10 = m10 * cosx + m->m12 * sinx;
    m->m12 = m->m12 * cosx - m10 * sinx;
    m->m20 = m20 * cosx + m->m22 * sinx;
    m->m22 = m->m22 * cosx - m20 * sinx;
    m->m30 = m30 * cosx + m->m32 * sinx;
    m->m32 = m->m32 * cosx - m30 * sinx;
}

static inline void MiniKitRotateZ(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m10 = m->m10;
    f32 m20 = m->m20;
    f32 m30 = m->m30;

    m->m00 = m00 * cosx - m->m01 * sinx;
    m->m01 = m00 * sinx + m->m01 * cosx;
    m->m10 = m10 * cosx - m->m11 * sinx;
    m->m11 = m10 * sinx + m->m11 * cosx;
    m->m20 = m20 * cosx - m->m21 * sinx;
    m->m21 = m20 * sinx + m->m21 * cosx;
    m->m30 = m30 * cosx - m->m31 * sinx;
    m->m31 = m30 * sinx + m->m31 * cosx;
}

static inline HUBAREAINFO_s *Hub_FindAreaInfoByIndex(i32 area) {
    for (HUBAREAINFO_s *info = HubAreaInfo; info->area_name != NULL; ++info) {
        if (info->area != NULL && area == info->area->index)
            return info;
    }
    return NULL;
}

void Hub_DrawMiniKits(WORLDINFO_s *world);

f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);

void Hub_InitMiniKits(WORLDINFO_s *world);

void Hub_DrawAreaStats(f32 phase, i32 area_index, i32 mode);

void Hub_DrawStarField();

void Hub_MakeModelList();

void Hub_UpdateMiniKits(WORLDINFO_s *world);

void Hub_LockUnlockDoors(WORLDINFO_s *);

void Hub_ActivateDoorMenu(LEVELDATA_s **level);

bool HubCustomiserUnlocked();

// All recovered callers use player zero and full opacity. Keeping this private
// allows the compiler to specialize those constant arguments naturally.
// Retail only keeps the player-one, full-alpha specialisation of the cursor
// drawing body; its name records the partial-inlining and constprop clones.
static __attribute__((noinline, noclone)) void
Hub_DrawIconCursors(COLLECTION_s *collection,
                    f32 scale) __asm__("_ZL19Hub_DrawIconCursorsP12COLLECTION_siff.part.9.constprop.51");
static void Hub_DrawIconCursors(COLLECTION_s *collection, f32 scale);

extern i16 tSELECT, tSELECTED, tSELECTING, tCONFIRMED, tYES, tNO, tEXIT;

void Hub_DrawFreePlaySelect();

extern f32 PANEL_REDBRICKSCALE;
void Hub_DrawImportantBrick(i32 object, f32 x, f32 y, f32 alpha, i32 count, i32 total);

void Hub_InitFreePlaySelect(i32 area, i32 first_model, i32 second_model);

bool HubMinikitViewerUnlocked();

void Hub_UpdateFreePlaySelect();

static void Hub_FindSpecial(WORLDINFO_s *world, nuhspecial_s *special, const char *name) {
    NuSpecialFind(world->current_gscn, special, const_cast<char *>(name), 1);
}

void Hub_Init(WORLDINFO_s *world);

void Hub_Load(WORLDINFO_s *world, variptr_u *, variptr_u *);

void Hub_Reset(WORLDINFO_s *world);

// Private hub menu and drawing helpers.

extern i16 tARCADE_NAME, tARCADE_NEEDTWOPLAYERS;
extern i32 Arcade_BothPlayersActive();
struct ARCADE_LEVEL_s {
    AREADATA **area;
    i16 *character_id;
};
extern ARCADE_LEVEL_s ArcadeLevel[12];
static void Hub_DrawArcadeStats(float alpha);

static void Hub_DrawMiniKitCount(f32 x, f32 y, i32 count, i32 total, f32 alpha);

static void Hub_MakeFreePlayList(i32 first_model, i32 second_model);

extern "C" f32 MainRenderTime;

static __used__ void Hub_UpdateSelectMode();

static void DrawEpisodesMenu(i32 selected, f32 alpha);

void MenuInitEpisodes(MENU_s *);

extern "C" f32 MainRenderTime;
extern i16 tSELECT, tBACK, tSELECTING;
extern "C" void MenuRepeat(i32 *, i32 *, f32 *, u8 *, f32, f32);
void WipeBackToHub();

void MenuUpdateEpisodes(MENU_s *);

void MenuDrawEpisodes(MENU_s *);

void MenuInitSelectMode(MENU_s *);

void MenuUpdateSelectMode(MENU_s *);

extern "C" f32 MainRenderTime;
extern i16 tSELECT, tBACK, tSELECTING;
static void Hub_DrawSelectModeMenu(i32 selected, f32 alpha);

void MenuDrawSelectMode(MENU_s *);

extern AREADATA *E1CHARACTER_ADATA, *E2CHARACTER_ADATA, *E3CHARACTER_ADATA, *E4CHARACTER_ADATA, *E5CHARACTER_ADATA,
    *E6CHARACTER_ADATA, *UTAPAU_ADATA, *HOTH_ADATA;
extern i16 id_PALPATINE, id_LAMASU, id_WOOKIEE, id_RANCOR, id_JAWA, id_WAMPA, id_BAT, id_HANINCARBONITE, id_EWOK;
ARCADE_LEVEL_s ArcadeLevel[12] = {{&E1CHARACTER_ADATA, &id_GUNGAN},
                                  {&SENATE_ADATA, &id_PALPATINE},
                                  {&BONUSKAMINO_ADATA, &id_LAMASU},
                                  {&E2CHARACTER_ADATA, &id_GEONOSIAN},
                                  {&UTAPAU_ADATA, &id_GRIEVOUS},
                                  {&BONUSKASHYYYK_ADATA, &id_WOOKIEE},
                                  {&E3CHARACTER_ADATA, &id_RANCOR},
                                  {&E4CHARACTER_ADATA, &id_JAWA},
                                  {&HOTH_ADATA, &id_WAMPA},
                                  {&BONUSDAGOBAH_ADATA, &id_BAT},
                                  {&E5CHARACTER_ADATA, &id_HANINCARBONITE},
                                  {&E6CHARACTER_ADATA, &id_EWOK}};
extern i16 tSUPERSTORY, tCHARACTERBONUS, tVEHICLEBONUS;
i32 hub_bonusmode = 0;
extern f32 text3d_width, text3d_height;
f32 BlipL[2] = {};
f32 BlipR[2] = {};
f32 BlipU[2] = {};
f32 BlipD[2] = {};
void MenuInitBonusMode(MENU_s *);
void MenuUpdateBonusMode(MENU_s *);
static void Hub_DrawBonusModeMenu(i32 selected, f32 alpha);
void MenuDrawBonusMode(MENU_s *);
static void Hub_DrawBonusModeMenu(int selected, float alpha);

void Hint_SetHintFromId(i32, i32, i32);
void Hint_Draw(i32);
extern f32 text3d_width, text3d_height;
extern i16 tCHALLENGE, tLOCKED;
static void Hub_DrawSelectModeMenu(int selected, float alpha);

extern i16 tBESTTIME;
static void Hub_DrawSuperBonusStats(AREADATA_s *area, float alpha);

void WipeBackToHub();

// ==== src/legoapi/menus/screens/arcade.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/misc.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

extern i16 tWINNER;
extern i16 tARCADE_NEEDTWOPLAYERS;

static f32 Arcade_NeedTwoPlayers_Scale = 1.0f;
i32 Arcade_Score[2];

i32 Arcade_BothPlayersActive();

i32 Arcade_GetMode(u32 *flags);

void Arcade_ResetPanel();

void Arcade_UpdatePanel(i32 paused);

void Arcade_DrawPanel(i32 paused);

void Arcade_AwardPoint(i32 player_index, i32 reset_score, i32);

void Arcade_PlayerKilled(i32 player_index, i32 extra);

void Arcade_AIKilled(i32 player_index);

void Arcade_CoinCollected(i32 player_index, u32 *score, u32 previous_score);

void Arcade_Kill(i32 player_index, i32 killed_player);

void Arcade_UpdateEndMenu(MENU_s *menu);

void Arcade_DrawEndMenu(MENU_s *);

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
extern "C" i32 DoubleScore;

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

extern COLLECTID * TempCollectID;

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
extern nuhspecial_s * collection_draw_hspecial;
extern i32 (*collection_draw_IsValidFn)(COLLECTION_s *, i32);
extern void (*Collection_GetSelectingPlayerIDsFn)(i16 *);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
extern FadeSystem FadeSys;


void Collection_Draw(COLLECTION_s *collection, float x, float y, float scale, APICHARACTERMODELLIST_s *models,
                     float alpha, i32 hide_selected);

void Collection_GetPos(COLLECTION_s *collection, i32 id, float *x, float *y);

i32 Collection_GetIDList(COLLECTION_s *collection, u32 model_flag_mask, u32 required_model_flags, i16 *ids,
                         i32 *first_id, i32 *second_id, i32 unused);

void Collection_CreateMaster(char *file, i16 *idlist, COLLECTION_s *collection, i32 param4, float param5);

void Collection_CreateCustom(char *name, i16 *id_list, COLLECTION_s *collection, u32 required_model_flags,
                             u32 excluded_model_flags, u32 required_game_flags, i32 require_buyable, i32 columns,
                             VARIPTR *buffer, VARIPTR *, i32 use_all_characters, f32 scale);

COLLECTID *CollectIDUnlocked(i32 id);

i32 Collection_GotAnyOfType(i32 type, u32 flags);

i32 AddToCollection(i32 id);
void CollectAllCharacters(i32 only_story);

extern i32 freeplaymode;
extern i32 freeplay_selected[2];
static __used__ void Collection_GetSelectingPlayerIDs(i16 *ids);

void ReleaseEat(GameObject_s *object);

i32 AddToCollection(i32 id);

void ResetCoinPacket(COINPACKET_s *packet);

void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index);

u32 GizmoBlowups_TotalScore(void *);
u32 GizForce_TotalScore(void *);
u32 GizObstacles_TotalScore(void *);
u32 GizTurrets_TotalScore(void *);
u32 GameAI_TotalScore();

u32 TotalLevelCoinTally(WORLDINFO_s *world, u32 *pickups, u32 *blowups, u32 *buildits, u32 *forces, u32 *obstacles,
                        u32 *turrets, u32 *characters);

COLLECTION_s *GetFreePlayCollection(i32 area);

extern i32 POINTS_PER_HINT;
extern i32 SHOPHINTCOUNT;
extern i16 HintTab[24];

void ReCalculateCompletionPoints();

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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" i32 NewMode;
extern "C" i32 reset_load;
extern "C" i32 Paused;
extern FadeSystem FadeSys;
extern "C" i32 screendump;
extern "C" STATUSPACKET_s StatusPacket;
extern AREADATA *NEGOTIATIONS_ADATA;

extern "C" i32 MenuInMemoryCard();
void ResetRumble(RUMBLEPACKET *packet);
void ReCalculateCompletionPoints();
void StatusStage_Reset(STATUS_STAGE_s *stage);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void IncreaseScore(u32 *, u64, i32);
extern "C" void PlaySfx(char *, nuvec_s *);
void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
void FinishStatusPacket(i32);
extern "C" void BackupMenu();
void Text_FillInExtendedSaveInfo();
void DrawStatusBG_LSW(STATUSPACKET_s *);
i32 InitStatusScreen_LSW(WORLDINFO_s *, STATUSPACKET_s *);
i32 FinishStatusPacket_LSW(WORLDINFO_s *, STATUSPACKET_s *, i32);
void ResetStatusPacket_LSW(STATUSPACKET_s *);
void RegisterStatusScreen(STATUS_STAGE_s *, i32 *, REGISTERSTATUSPACKET_s *);
void EndOfDemo(i32);

i32 AddGoldBrickMessage(STATUSPACKET_s *packet, i16 brick);
extern f32 DROPINALPHA;
extern f32 PANEL_SCOREX;
extern f32 PANEL_SCORESCALE;
extern i16 tCOINTOTAL;
extern i16 tSELECT;
extern i16 tSELECTING;
void Text_MakeScore(u32, char *);
void Status_DrawPromptMenu(STATUSPACKET_s *, i32, f32);
f32 getFinishedStatusAlpha(STATUSPACKET_s *);
extern f32 STATUS_TITLE_Y;

u16 hub_iconang[4] = {};
static f32 hub_icontime[4] = {};
static i32 hub_icondang[4] = {};
extern STATUS_STAGE_s * StatusStages;
extern f32 iconalphaoverride;
extern f32 icon_y;
extern i32 draw_player_icons;
extern i32 status_prompt;
void RedBrick_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void RedBrick_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void RedBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void CollectCharcters_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void CollectCharcters_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void CollectCharcters_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void CollectCharactersOff_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void CollectCharactersOff_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void CollectCharactersOff_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void Coins_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void Coins_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Coins_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void TrueHero_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void TrueHero_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void TrueHero_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void MiniKit_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void MiniKit_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void MiniKit_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void AllMiniKits_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void AllMiniKits_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void AllMiniKits_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void GoldBrick_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void GoldBrick_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void GoldBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void LevelComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void LevelComplete_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void LevelComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusTime_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusTime_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void ChallangeCash_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void ChallangeCash_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void ChallangeCash_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void SuperStoryTime_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void SuperStoryTime_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void SuperStoryTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void SuperStoryScore_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void SuperStoryScore_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void SuperStoryScore_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusComplete_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusWin_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusWin_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusWin_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void Prompt_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void Prompt_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Exit_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Save_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Fade_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);

extern STATUSPACKET_LSW_s StatusPacket_LSW;
extern STATUS_STAGE_s StatusStages_LSW[34];

void AddStatusStage(STATUSPACKET_s *packet, i32 type, i32 gold_brick_enabled);

void SetBonusWinner(i32 player);

STATUS_STAGE_s *FindStatusStage(i32 type);

void NextStatusStage(STATUSPACKET_s *packet);

void Prompt_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void ResetIconWibble();

void Save_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

extern i32 from_save_and_exit;
extern "C" TIMER BonusTimer;
extern i16 id_SLAVE1;
extern i16 tSUPERSTORYCOMPLETE, tNEWBESTTIME, tLEVELCOMPLETE, tMISSIONCOMPLETE;
extern i16 tNONEWBESTTIME;
extern i16 tNEWHIGHSCORE, tNONEWHIGHSCORE;
extern i16 tCHALLENGECOMPLETE, tTRUEHERO, tMINIKIT;
extern "C" void NuIOS_RecordFlurryEvent(char *);
extern "C" i32 NuStrCpy(char *, const char *);
i32 AddToCollection(i32);
i32 Mission_CurrentState(MISSIONSYS *);
i32 newCharactersCollected(STATUSPACKET_s *);
void StatusPacketReset(STATUSPACKET_s *);
extern i32 newminikitcount;
extern i32 currentminikit;

extern NEWMINIPIECE_s NewMiniPiece[10];

void InitStatusScreen(WORLDINFO_s *world);

void SetDrawGoldBrick(STATUSPACKET_s *packet, i32);


void NewStatusRumbleBuzz(i32 player, float amount, float buzz, i32 priority);

f32 StatusIconsOnOff(f32 progress);

static inline void UpdateIconWibbleAxis(i32 i) {
    hub_icontime[i] -= FRAMETIME;
    if (hub_icontime[i] <= 0.0f) {
        hub_icondang[i] = static_cast<i32>(QRAND_FLOAT() * 262144.0f - 131072.0f);
        hub_icontime[i] = QRAND_FLOAT() * 2.0f + 1.0f;
    }
    hub_iconang[i] = static_cast<u16>(static_cast<i32>(hub_iconang[i] + hub_icondang[i] * FRAMETIME));
}

void UpdateIconWibble();

void Prompt_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

extern i32 STATUS_R, STATUS_G, STATUS_B;
extern i16 tUNLOCKED;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
void RedBrick_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void RedBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);

void StatusPacketReset(STATUSPACKET_s *packet);

void StatusStage_Reset(STATUS_STAGE_s *stage);

extern i16 tTRUEJEDI, tSTORY, tFREEPLAY;
extern "C" void NuStrCat(char *, const char *);

void TrueHero_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void TrueHero_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);

i32 UpdateAchievements(STATUSPACKET_s *);

void UpdateStatusScreen(WORLDINFO_s *);

void DrawStatusScreen(WORLDINFO_s *);

extern "C" STATUSPACKET_s StatusPacket;
extern FadeSystem FadeSys;
extern "C" i32 reset_area;
extern i32 grab_screen_image;
void InitChallenge(i32);
void InitMission(MISSIONSYS_s *, i32);

void FinishStatusPacket(i32 choice);

void RedBrick_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

void TrueHero_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

i32 InitStatusScreen_LSW(WORLDINFO_s *, STATUSPACKET_s *);

void RegisterStatusScreen(STATUS_STAGE_s *stages, i32 *, REGISTERSTATUSPACKET_s *registration);

void ResetStatusPacket_LSW(STATUSPACKET_s *packet);

extern i16 tMAP, tRESTART, tFINISHSTORY, tCONTINUESTORY, tFINALCHAPTERLOCKED, tNEXTCHAPTERLOCKED;
extern f32 text3d_width, text3d_height;
extern "C" {
    extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
    extern u8 MENUNORMALR, MENUNORMALG, MENUNORMALB;
    extern u8 MENUFLASH0R, MENUFLASH0G, MENUFLASH0B;
    extern u8 MENUFLASH1R, MENUFLASH1G, MENUFLASH1B;
}
void Hint_SetHintFromId(i32, i32, i32);
void Hint_SetComplete(i32);
void Hint_Draw(i32);

f32 getFinishedStatusAlpha(STATUSPACKET_s *packet);

void Status_DrawPromptMenu(STATUSPACKET_s *packet, i32 selected, float alpha);

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.
static __attribute__((noinline, noclone)) void SuperStoryTime_LSW_DrawBody(STATUS_STAGE_s *stage, STATUSPACKET_s *packet)
    __asm__("_Z23SuperStoryTime_LSW_DrawP14STATUS_STAGE_sP14STATUSPACKET_si.part.12");


void SuperStoryTime_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current);

void SuperStoryTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);

void LSW_registerStatusScreen();

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.
static __attribute__((noinline, noclone)) void SuperStoryScore_LSW_DrawBody(STATUS_STAGE_s *stage, STATUSPACKET_s *packet)
    __asm__("_Z24SuperStoryScore_LSW_DrawP14STATUS_STAGE_sP14STATUSPACKET_si.part.8");


void SuperStoryScore_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current);

void SuperStoryScore_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);

void SuperStoryTime_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

void SuperStoryScore_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

extern i16 tFREEPLAYUNLOCKED, tEPISODEVUNLOCKED, tEPISODEVIUNLOCKED, tBONUSUNLOCKED;
extern i16 tOUTOFTIME, tYOUDIDNTCOLLECTALL10CANISTERS, tSTORYCLIPSUNLOCKED;
extern i16 tEPISODEICOMPLETE, tEPISODEIICOMPLETE, tEPISODEIIICOMPLETE;
extern i16 tEPISODEIVCOMPLETE, tEPISODEVCOMPLETE, tEPISODEVICOMPLETE;

// Retail keeps the active-stage drawing as the split-out .part clone of
// LevelComplete_LSW_Draw; the inline head only tests `active`.
static __attribute__((noinline, noclone)) void LevelComplete_LSW_DrawActive(STATUS_STAGE_s *stage,
                                                                         STATUSPACKET_s *packet)
    __asm__("_Z22LevelComplete_LSW_DrawP14STATUS_STAGE_sP14STATUSPACKET_si.part.27");


void LevelComplete_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void Coins_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);
void Coins_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);
void Coins_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);
extern i16 tWINNER;
void DrawBonusTime(STATUSPACKET_s *, f32, i32);

void BonusWin_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current);
void BonusWin_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusTime_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current);
void BonusTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void ChallangeCash_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 draw);
void ChallangeCash_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusWin_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);
void BonusTime_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);
void ChallangeCash_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);
void BonusComplete_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current);
void BonusComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);
void BonusComplete_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

// ==== src/legoapi/gizmo/base/gizmo_sys.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/area.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuspecial.h"

#include <stdio.h>
#include <string.h>
struct FLOWBOX_s;

extern i32 gizmoerrorlogsize;

GIZMOSYS *CreateGizmoSys(void *world, VARIPTR *buf, VARIPTR *buf_end);
void LoadGizmoSys(GIZMOSYS_s *gizmo_sys, void *world, char *config_file);
void Hub_LoadAndFixUpMiniKits(WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end);
void MiniKit_Load(MINIKIT *minikit, i32 id, VARIPTR *buf, VARIPTR *buf_end, void *param);
void MiniKit_InitPieces(MINIKIT *minikit, i32 count, VARIPTR *buf, VARIPTR *buf_end);
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world);

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
extern HINTUIBUTTON_s * hintUIButton;
extern f32 AlphaBlendTime;
extern f32 ICONX;
extern f32 ICONXPOS_TOUCHYFEELY;
extern f32 ICONYPOS_TOUCHYFEELY;
extern f32 ICONXPOS_VIRTUALS;
extern f32 ICONYPOS_VIRTUALS;
static NUVEC PercentHint_TriggerPos = {-28.421f, 0.0f, -52.462f};
extern f32 (*Hint_AlphaTargetFn)();
extern i32 (*Hub_PanelBusyFn)();
extern i32 only_process_this_hint_id;
f32 SeekLinearF(f32, f32, f32);
void Hint_SetHint(HINT_s *, i32, i32);

extern "C" i32 NewMode, Paused, editor_active, CutSceneWaiting, PANELOFF;
extern FadeSystem FadeSys;
i32 GetMenuID();
void MechHintUIButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);

#include "legoapi/menus/core/gamehint_types.h"
DECOMP_ASSERT(sizeof(MechHintUIButton) == 0xa8, "MechHintUIButton size");
DECOMP_ASSERT(offsetof(MechHintUIButton, pending_hint) == 0x7c, "Pending touch hint offset");
DECOMP_ASSERT(offsetof(MechHintUIButton, display_elapsed) == 0x80, "Touch hint display timer offset");

void MechHintUIButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &);

void initHintSys();



void CLEAR_HINT_COMPLETE(i32 hint_id);
i32 HINT_COMPLETE(i32 hint_id);
void SET_HINT_COMPLETE(i32 hint_id);
void Hint_SaveGameState(HINT_s *hint);
void initHintSys();
void Hint_Reset();

void Hint_Process(float elapsed);

void Hint_SetHint(HINT_s *hint, i32 force, i32 allow_completed);

HINT_s *Hint_FindHint(i32 hint_id);

void Hint_SaveBits(i32 hint_id, i32 complete);

i32 Hint_CurrentId();

void Hint_ResetHint(i32 hint_id, i32 reset_completed);

i32 Hint_isComplete(HINT_s *hint);

i32 Hint_isComplete(i32 hint_id);

void Hint_SetComplete(HINT_s *hint);

void Hint_SetComplete(i32 hint_id);

i32 Hint_isAvailable(i32 hint_id);

void Hint_CancelCurrent();

void Hint_ExpandButtons(char *input, char *output);

void Hint_SaveGameState(HINT_s *hint);

void Hint_SetHintFromId(i32 hint_id, i32 force, i32 allow_completed);

void Hint_SaveAllGameState();

void Hint_LoadAllGameState();

void Hint_ClearHintsAndDoneFlags();

void Hint_Draw(i32 viewport);

// Static game message and hint helpers. Stubbed to satisfy the symbol baseline.

static __used__ nuhspecial_s *GameMsg_GetExtraObj(GAMEMESSAGE_s *message);

static __used__ void GameMsg_EndDelay_Game(GAMEMESSAGE_s *message);

i32 Percent_UpdateHint(HINT_s *hint);

void RegisterWithHintSys(void (*update_fn)(HINT_s *, i32), HINT_s *hints, u32 *save_bits, i32 save_bit_count);

// ==== src/legoapi/menus/core/hint.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/input/gamepads.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern i32 only_process_this_hint_id;
extern f32 ICONXPOS_VIRTUALS;
extern f32 ICONYPOS_VIRTUALS;
extern f32 ICONYPOS_TOUCHYFEELY;
extern f32 AlphaBlendTime;

// The hint icon transitions and the touch icon x position are dynamically
// initialized in this unit's static constructor in the reference.
extern HintVectorTransition hintIconPos;
extern HintScalarTransition hintIconScale;
extern HintScalarTransition hintYPop;
extern f32 ICONXPOS_TOUCHYFEELY;

extern u8 show_unlock_shop_hint;
extern u8 show_unlock_customiser_hint;
extern u8 show_unlock_minikitviewer_hint;

i32 HINT_COMPLETE(i32 hint_id);

i32 Tilt_UpdateHint(HINT_s *);

f32 CurrentHintAlpha();

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.
static __attribute__((noinline, noclone)) void DrawHint_LSWBody(HINT_s *hint, i32 max_lines)
    __asm__("_Z12DrawHint_LSWP6HINT_si.part.19");


void DrawHint_LSW(HINT_s *hint, i32 max_lines);

i32 Dodge_UpdateHint(HINT_s *hint);

void SET_HINT_COMPLETE(i32 hint_id);

void CLEAR_HINT_COMPLETE(i32 hint_id);

i32 DragBomb_UpdateHint(HINT_s *hint);

// Original local callbacks referenced by Hints_LSW.
extern u8 show_lever_hint;
extern i32 show_autojump_hint;
extern AREADATA_s *PODRACE_ADATA, *PODSPRINT_ADATA, *BONUS_GUNSHIP_ADATA;
extern FadeSystem FadeSys;







extern u8 show_hatmachine_hint;


extern HINTUIBUTTON_s *hintUIButton;

void *GetHintFromUIButton();

extern HINT_s Hints_LSW[55];
void RegisterWithHintSys(void (*)(HINT_s *, i32), HINT_s *, u32 *, i32);
extern i32 id_HINT_LSW_AUTOJUMP;
extern i32 id_HINT_LSW_AUTOJUMP_FAIL;

void initGameHintSys_LSW();

i32 SmartBomb_UpdateHint(HINT_s *);

extern AREADATA *JABBASPALACE_ADATA;

i32 ShinyMetal_UpdateHint(HINT_s *hint);

f32 CurrentHintButtonScale();

void initGameHintSys_Batman();

i32 IndyUnlocked_UpdateHint(HINT_s *hint);





i32 GizForce_UpdateHint(HINT_s *);
i32 ZipUps_UpdateHint(HINT_s *);
i32 Push_UpdateHints(HINT_s *);
i32 GizPanel_UpdateHint(HINT_s *);
i32 Shop_UpdateHint(HINT_s *);
extern HINT_s Hints_LSW[55];

// Each control mode retains its own requested and interpolated hint row.

void MenuUpdateHints(MENU_s *menu);

void MenuDrawHints(MENU_s *menu);

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
extern i32 NextArea_FreePlay;
extern i16 tCHAPTER;
extern f32 ICONSIZE;
void NewLevelFromMenu(LEVELDATA_s *, i32, i32, i32);

void EvaluateJointOrientationMtx(nugscn_s *scene, i32 joint_index, numtx_s *matrix);

extern CUTSCENEPLAYER_s * CutScenePlayer;

void CutScenePlayer_Reset();

void CutScenePlayer_Start(i32 clip_id, i32 door);

CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

void CutScenePlayer_GetText(i32 id, char *title, char *subtitle, i32 mode);

i32 CutScenePlayer_CanStart(i32 clip_id);

void CutScenePlayer_DrawGrid(COLLECTION_s *collection, i16 *ids, float x, float y, i32 selected, float alpha);

void CutScenePlayer_Configure(char *file, VARIPTR *buffer, VARIPTR *, i16 *clip_text, i16 *intro, i16 *midtro,
                              i16 *outro, i16 *ending);

void CutScenePlayer_SetObjects(CUTINFO *cut);

i32 CutScenePlayer_CountEpisodeClips(i32 episode, i32 include_guests, i16 *ids);

void FindGameCutScenes();

SCENESPECIALPROGRESS_s *FindSceneStateObj(nugscn_s *scene, SCENEPROGRESS_s *progress, nuhspecial_s *target);

i32 FindMtlInHGObj(nugscn_s *scene, i32 material_type);

i32 FindTexAnimFromMtl(nugscn_s *, numtl_s *);

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *opaque_state);

i32 instNuGCutGetNextRigidInfo(instNUGCUTSCENE_s *instance, float frame, i32 index, numtx_s *matrix,
                               nuhspecial_s *special);

i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *instance, i32 force);

void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);

void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);

i32 instNuGCutContainsInstancedRigids(instNUGCUTSCENE_s *instance);

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
struct CUTSCENEPLAYERCLIP;
struct instNUGCUTCHAR_s;
struct NUGCUTCHAR_s;
struct NUGCUTRIGID_s;
struct instNUGCUTRIGID_s;

extern CUTSYS * CS_cutsys;


static void copyAnims(NUGCUTSCENE_s *, NUGCUTSCENE_s *);
void NewCopyAnims(instNUGCUTSCENE_s *);
i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *, i32);
NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *, VARIPTR *);
extern "C" void instNuGCutSceneDestroy(instNUGCUTSCENE_s *);


extern i32 CUTCOUNT;
extern CutSceneListEntry * CutList;
extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);
extern i32 ACTIVECUTCOUNT;
extern f32 CutSceneScale;
extern "C" {
extern u8 CUTSUBTITLEDEFAULT_R;
extern u8 CUTSUBTITLEDEFAULT_G;
extern u8 CUTSUBTITLEDEFAULT_B;
extern u8 CUTSUBTITLEDEFAULT_A;
extern u8 CUTSUBTITLEDEFAULT_ALIGN;
extern f32 CUTSUBTITLEDEFAULT_X;
extern f32 CUTSUBTITLEDEFAULT_Y;
extern f32 CUTSUBTITLEDEFAULT_SCALEX;
extern f32 CUTSUBTITLEDEFAULT_SCALEY;
extern f32 CUTSUBTITLEDEFAULT_FITWIDTH;
}
extern i32 CUTCAM;
extern i32 CUTCAMONLY;
extern NUMTX cutscenecammtx;
extern i32 cutscenecamchange;
extern f32 cutscenecam_fstop;
extern u8 cutscenecam_usefocusloc;
extern NUVEC cutscenecam_focusloc;
extern f32 cutscenecam_focusDistance;
extern f32 cutscenecam_focalLength;
extern i32 CameraDOFHack;
extern u8 set_cutscenecammtx;












































void *CutScenes_Load(char *config, NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end, i32 param2,
                     i32 param3, WORLDINFO *world);

// --- Extern "C" block: functions with confirmed C linkage in original libTTapp.so ---
extern "C" void *NuAnimData2FixPtrs(void *, isize, isize, i32);
extern "C" StateAnim *StateAnimFixPtrs(StateAnim *, isize);
extern "C" i32 StateAnimEvaluate(StateAnim *, u8 *, u8 *, f32);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);
extern "C" void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *, i32, f32, NUMTX *);
extern "C" {
    extern i32 NuGCutDebFixUp_SearchAllPages;
    extern NUGCUTLOCATORFNENTRY_s *locatorfns;
}
void NuGCutRigidCalcMtx(NUGCUTRIGID_s *, f32, numtx_s *);

#include "legoapi/cutscenes/cutscene_types.h"
DECOMP_ASSERT(sizeof(CutSceneTriggerNameEntry) == 0x34, "cutscene trigger name entry stride");
DECOMP_ASSERT(offsetof(CutSceneTriggerNameTable, entries) == 0x0c, "cutscene trigger name table entries offset");

// Exported (default visibility) in the original library.
extern i32 cutscene_quit_prompt;
extern i32 cutscene_has_quit;
extern i32 cutscene_lock_player;
extern i32 termcutstream_hack;

extern "C" {
extern instNUGCUTSCENE_s * cutscene_load_instance;
extern i32 NumCommonStreamingBuffers;

instNUGCUTSCENE_s *instNuGCutSceneFind(const char *name);

void *instNuGCutSceneCleanUp(void);
}

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
static NUGCUT_FIXPTRS_ATTR(2) void NuGCutSceneFixPtrsAnims(NUGCUTSCENE_s *cutscene, isize anim_delta)
    __asm__("_ZL18NuGCutSceneFixPtrsP13NUGCUTSCENE_sii.constprop.44");
static void NuGCutSceneFixPtrsAnims(NUGCUTSCENE_s *cutscene, isize anim_delta);
static NUGCUT_FIXPTRS_ATTR(1) void NuGCutSceneFixPtrsNoAnims(NUGCUTSCENE_s *cutscene)
    __asm__("_ZL18NuGCutSceneFixPtrsP13NUGCUTSCENE_sii.constprop.43");
static void NuGCutSceneFixPtrsNoAnims(NUGCUTSCENE_s *cutscene);

extern "C" void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *locator_functions);

void NuGCutSceneSysInitVfx(NUGCUTLOOKUPLOCATORVFXFN lookup, NUGCUTTRIGGERLOCATORVFXFN trigger,
                           NUGCUTRELEASELOCATORVFXFN release, NUGCUTUPDATELOCATORVFXFN update);

void NuGCutSceneRemapFocusIdToLocaterNum(NUGCUTSCENE_s *cutscene, VARIPTR *buffer);

extern "C" {

extern i32 NuGCutDebFixUp_SearchAllPages;
extern NUGCUTLOCATORFNENTRY_s * locatorfns;
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

void instNuGCutSceneReset(instNUGCUTSCENE_s *instance);

void instNuGCutSceneStart(instNUGCUTSCENE_s *instance);

void instNuGCutScenePause(instNUGCUTSCENE_s *instance, u8 paused);

void instNuGCutSceneDestroy(instNUGCUTSCENE_s *instance);

i32 NuGCutSceneIsBackgroundLoading(void);

i32 instNuGCutScenePreload(instNUGCUTSCENE_s *instance);

void instNuGCutSceneServiceLoad(void);

void NuGCutSceneSysBackgroundFlush(void);

void NuGCutSceneSysPostBackgroundLoad(void);

void PetesHackOfDeath(void);
} // extern "C"

struct instNUGCUTSCENE_s;
struct NUGCUTLOCATORSYS_s;
struct instNUGCUTLOCATOR_s;
struct NUGCUTLOCATOR_s;
struct numtx_s;
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);
extern "C" void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                        NUGCUTLOCATOR_s *, f32, NUMTX *, i32);
i32 Draw3DObjectMtx(WORLDINFO_s *, i32, numtx_s *);

void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);
extern "C" void DebFreeInstantly(i32 *handle);

static void instNuGCutRigidSysEnd(instNUGCUTSCENE_s *instance, float frame);

// Retail only keeps the IPA-SRA clone, which drops the unused frame argument.
#if defined(__i386__)
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone))
#endif
static INST_LOCATOR_SYS_END_ATTR void instNuGCutLocatorSysEnd(instNUGCUTLOCATORSYS_s *instance,
                                                              NUGCUTLOCATORSYS_s *system)
    __asm__("_ZL23instNuGCutLocatorSysEndP22instNUGCUTLOCATORSYS_sP18NUGCUTLOCATORSYS_sf.isra.8");
static void instNuGCutLocatorSysEnd(instNUGCUTLOCATORSYS_s *instance, NUGCUTLOCATORSYS_s *system);

void instNuGCutSceneEndFirstFrame(instNUGCUTSCENE_s *instance);

extern "C" void instNuGCutSceneJumpToEnd(instNUGCUTSCENE_s *instance);

extern "C" void instNuGCutSceneJumpToLastFrame(instNUGCUTSCENE_s *instance);

extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

static void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *, float, int);
static void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *, float);
static void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *, float);
static void instNuGCutSceneClipTest(instNUGCUTSCENE_s *);



static void instNuGCutSceneUpdate(instNUGCUTSCENE_s *instance, int paused, int skip, float elapsed);

static void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *instance, float frame);

static void instNuGCutSceneClipTest(instNUGCUTSCENE_s *instance);

static void instNuGCutRigidSysRender(instNUGCUTSCENE_s *instance, float frame, int paused);

static void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *instance, float frame, int paused);

static __used__ void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *instance, float frame);

extern "C" void NuGCutSceneSysUpdate(i32 paused, i32 skip, f32 elapsed);

extern "C" void NuGCutSceneSysRender(i32 paused);


void NewCopyAnims(instNUGCUTSCENE_s *instance);

static void copyAnims(NUGCUTSCENE_s *destination, NUGCUTSCENE_s *source);

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

extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *);

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
    extern NUGCUTLOCATORFNENTRY_s *locatorfns;
    extern f32 timeincrement;
    extern i32 processdeb;
    extern f32 glyntestha;

    void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);
    void NuMtxPreTranslate(NUMTX *, NUVEC *);
    void NuMtxMul(NUMTX *, NUMTX *, NUMTX *);
    i32 NuStrICmp(const char *, const char *);
    void instNuGCutSceneStart(instNUGCUTSCENE_s *);

    i32 AddScaledVariableShotDebrisEffect2(i32, NUVEC *, i32, f32, NUMTX *, NUMTX *, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPopulateInstance(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientationMtx(i32, NUMTX *);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void AddScaledFiniteShotDebrisEffect(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32, f32);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
}

extern "C" {

i32 CheckStreamFileID(void);

void DisplayCutSceneMemory(void);

void PauseGameCut(void);

void RestoreGameCut(void);

extern u8 ForceScenePlayBack;

void SetForceScenePlayBack(i32 enabled);

f32 instCutSceneTimeElapsed(instNUGCUTSCENE_s *instance);

i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index);

void instNuGCutSceneAddCleanUpItem(void *handle, i32 enabled);

void instNuGCutSceneCalculateAverageCentre(instNUGCUTSCENE_s *instance, NUMTX *matrix, NUVEC *out);

void instNuGCutSceneChain(instNUGCUTSCENE_s *instance, instNUGCUTSCENE_s *next);

i32 instNuGCutSceneCharGetStartMtx(instNUGCUTSCENE_s *instance, const char *name, NUMTX *out);

void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf);

void instNuGCutSceneDisable(instNUGCUTSCENE_s *instance);

void instNuGCutSceneEnable(instNUGCUTSCENE_s *instance);

i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance);

void instNuGCutScenePlay(instNUGCUTSCENE_s *instance, i32 forward);

void instNuGCutSceneResetCleanUp(void *list_storage, void *cut_base, void *cut_end, void *inst_base, void *inst_end,
                                     instNUGCUTSCENE_s *(*get_inst)(void *),
                                     void *(*create_inst)(void *, NUGCUTSCENE_s *, VARIPTR *),
                                     void (*inst_destroyed)(void *));

void instNuGCutSceneRotateY(instNUGCUTSCENE_s *instance, NUANG angle);

void instNuGCutSceneSetEndCallback(instNUGCUTSCENE_s *instance, void (*callback)(instNUGCUTSCENE_s *));

void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix);

void instNuGCutSceneSetPos(instNUGCUTSCENE_s *instance, NUVEC *pos);

void instNuGCutSceneSetRepeat(instNUGCUTSCENE_s *instance, i32 repeat);

void instNuGCutSceneStop(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTimeLeft(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTotalTime(instNUGCUTSCENE_s *instance);

void instNuGCutSceneTranslate(instNUGCUTSCENE_s *instance, NUVEC *translation);

void instNuGCutSceneWaitAtEnd(instNUGCUTSCENE_s *instance, u8 enabled);

void instNuGCutSoundStream(void);

} // extern "C"

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *state);

// Steers a cutscene character's eyes towards a look-at locator and blinks its eyelids.
// Only version-3 locator animations carry the look and blink channels.
// Retail only keeps the IPA-SRA clone: the system is dropped and the instance
// locator is replaced by its effect handle slot.
#if defined(__i386__)
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone))
#endif
static INST_LOOKAT_UPDATE_ATTR void instNuGCutLookAtLocatorUpdate(i32 *effect_handle, NUGCUTLOCATOR_s *locator,
                                                                  f32 frame, NUMTX *parent_mtx,
                                                                  instNUGCUTSCENE_s *instance)
    __asm__("_ZL29instNuGCutLookAtLocatorUpdateP18NUGCUTLOCATORSYS_sP19instNUGCUTLOCATOR_sP15NUGCUTLOCATOR_"
            "sfP7numtx_sP17instNUGCUTSCENE_s.isra.23");
static void instNuGCutLookAtLocatorUpdate(i32 *effect_handle, NUGCUTLOCATOR_s *locator, f32 frame,
                                          NUMTX *parent_mtx, instNUGCUTSCENE_s *instance);

extern "C" {
    // Aligned four-float row used for retail's movaps matrix copies.
    typedef f32 LocatorMtxRow __attribute__((vector_size(16)));

void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *instance, NUGCUTLOCATORSYS_s *system,
                                 instNUGCUTLOCATOR_s *inst_locator, NUGCUTLOCATOR_s *locator, f32 frame,
                                 NUMTX *parent_mtx, i32 paused);

} // extern "C"

// ==== src/nu2api/nusound/nu2api_nusound_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>

typedef void (*SoundBitCallback)(i32 sound_id);

extern "C" void MusicPreSeek(i32 track);
extern "C" void RestoreGameMusic(void);

extern "C" {
extern i32 g_NuSoundMaxVoicesPerSample;
extern f32 AUDIOFADELEVEL;
extern f32 MASTERVOLUME;
extern i32 gcutSoundMusVol;
extern i32 gcutSoundVol;

extern MusicPlayback Music;

    void NuSound3FlushLoops(void);
    void SoundStopMusic(void);

f32 GetSoundVolume(void);
void SetSoundBitsById(const i32 *sound_ids, SoundBitCallback set_bit);
void SetSoundBitsBySingleId(i32 sound_id, SoundBitCallback set_bit);
void SetSoundVolume(f32 volume);
void SoundKillAll(void);
void SoundStopMusic(void);

void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
void edbitsSoundPlay(NUVEC *, i32);
void gcutSetSoundVol(i32 sound_volume, i32 music_volume);
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

void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
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

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;
nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count);

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
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
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
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

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
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
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
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


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
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
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
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
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
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
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
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
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
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
void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== src/nu2api/nucore/nugcutscene_anim.cpp: all remaining content ====
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/numtx.h"

f32 *NuGCutLocatorCalcMtx_3(NUGCUTLOCATOR_s *locator, numtx_s *mtx, float frame);

extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *locator, float frame, NUMTX *mtx, nuanimtime_s *time);

extern char CurveFilterLOCATORScale[4];
extern char CurveFilterLOCATOR[3];
extern char CurveFilterSFX[2];

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
i32 UseFallAnim(GameObject_s *object);
i32 GetDefaultIdle(GameObject_s *object);
i32 SetProtocolDroidFallAnim(GameObject_s *object);
// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.
static void MoveAnim_Manage(GameObject_s *object, f32 movement_speed, i32 allow_tiptoe, i32 weapon_variant);
static void MoveAnim_Check(GameObject_s *object);
static void JumpAnimCode(GameObject_s *object);
static bool JumpAnim_HasAction(const GameObject_s *object, i16 action);
static GAMECHARACTERDATA *GetGameCharacterData(GameObject_s *object);
void UpdateCharacterIdle(GameObject_s *object);
void AutoWeaponOnOff(GameObject_s *object);
void AddFootSteps(GameObject_s *object);
f32 PodSprint_InStartCountdown(WORLDINFO_s *world);
i32 PodRace_InStartCountdown(WORLDINFO_s *world);
void SetPodMergeAnims(ANIMPACKET_s *packet, i32 index);
extern "C" i16 id_ANAKINSPODGREEN;
extern "C" void PlaySfxByIdAndSetVolume(i32 sfx_id, NUVEC *position, f32 volume);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern i16 id_BODYGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_YODA;
extern i16 id_YODAGHOST;
extern i16 id_GONKDROID;
extern i16 id_JUMBOHOMINGDROID;
extern i16 id_GAMORREANGUARD, id_PRINCESSLEIASLAVE, id_LAMASU, id_TAUNWE;
extern i16 id_PENGUIN, id_UMBRELLA, id_GLIDEPACK;
extern f32 hub_jabbaawake;

#include "legoapi/characters/motion/gameanim_types.h"






void Animate_ATAT(GameObject_s *object);

void Animate_POD(GameObject_s *object);


void Animate_JEDI(GameObject_s *object);


void AnimatePlayer(GameObject_s *object);

void Animate_BEAST(GameObject_s *object);

void Animate_BARMAN(GameObject_s *object);

void Animate_CANNON(GameObject_s *object);

void Animate_WALKER(GameObject_s *object);

void Animate_CRITTER(GameObject_s *object);

void Animate_WEIRDO(GameObject_s *object);

void Animate_DEFAULT(GameObject_s *object);

extern i32 NeedsPretendAnim(GameObject_s *object);

void Animate_VEHICLE(GameObject_s *object);

void Animate_PROTOCOL(GameObject_s *object);

void Animate_DROIDEKA(GameObject_s *object);

void GameAnimSet_Draw(GAMEANIMSET_s &set);

i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state);

i32 GameAnimSet_Stop(GAMEANIMSET_s *set);

void Animate_ASTROMECH(GameObject_s *object);

void Animate_CHARACTER(GameObject_s *object);

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

void GameAnimSet_RemoveObject(GAMEANIMSET_s *set, GAMEANIMOBJ_s *object);

void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale);

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating);

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set);

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static);

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set);

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position);

void GameAnimSet_RemoveSpecial(GAMEANIMSET_s *set, nuhspecial_s *special);

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

i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                            i32 joint_count, i32 first_joint, NUVEC *root_translation);
i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                             i32 joint_count, i32 first_joint, NUVEC *root_translation);

extern "C" {

    i32 ANI_SimpleAni3PlayerV4Joint_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                              i32 first_joint);
    i32 ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                                    i32 joint_count, i32 first_joint, NUVEC *root_translation);

void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32);

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

f32 GetInstAnimEndFrame(nugscn_s *scene, nuinstanim_s *instance_animation);

extern NUJOINTPROCANIMFN JointProcAnimFn;

void SetProceduralAnimationFn(void *function);

i32 StateAnimEvaluate(StateAnim *state, u8 *index, u8 *value, f32 frame);

i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);

void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame);

StateAnim *StateAnimFixPtrs(StateAnim *state, isize delta);

} // extern "C"

void SetAnimFrame(nuhspecial_s *special, float frame);

i32 GetAnimDirection(nuinstanim_s *animation);

i32 FindTexAnimFromMtl(nugscn_s *scene, numtl_s *material);


void InitTexAnimScripts(char **names);

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation);

void EvalAnim2(nuhspecial_s *special, float frame);

void GameAnimSys_AllocateLevelProgressData(variptr_u *buf, variptr_u *buf_end, i32 capacity, i32 level_count);

u8 *GameAnimSys_GetProgressData(i32 index);

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index);

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 index);

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


extern "C" f32 (*GetCharacterGoalSpeedFn)(APIOBJECT *);

static __used__ i32 RowMoveWander(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);

static __used__ i32 RowMoveTowards(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);

static void FormationMove(AIGROUP *group, i32 (*move)(AIGROUP *, AIROW *, AIROW *, APIOBJECT *));

static void AISysCheckAntinode_Rectangle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

static void AISysCheckAntinode_Ellipse(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

static void AISysCheckAntinode_Circle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

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

void AIPathCalcExtents(AIPATH *path);
i32 AIPathCheckExtents(AIPATH *path, NUVEC *position);
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
extern "C" void AISysFindRoute(AIPACKET *packet);
extern "C" void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
extern "C" void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);

extern "C" void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);






#include "legoapi/ai/core/ai_pathgeometry.h"

static u32 AISysCharacterTestPathCnx(AISYS *system, APIOBJECT *object, AIPACKET *packet, AIPATHCNX *connection,
                                     i32 direction, f32 *nearest_distance);










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

extern i32 ACTIVECUTCOUNT;
extern "C" i32 CUTDRAWWORLD;
extern "C" i32 Paused;
extern i32 CUTCAM;
extern "C" i32 CutSceneWaiting;
extern i32 CUTCAMONLY;
extern i32 cut_waiting_for_new_level;
extern volatile i32 waiting_for_level;
extern "C" i32 newlevel_resumecutaudio;
extern CUTSYS *CS_cutsys;
extern FadeSystem FadeSys;
extern CUTSCENESYS * CutSceneSys;

static void CutScene_DrawCharacter(instNUGCUTSCENE_s *, NUGCUTSCENE_s *, instNUGCUTCHAR_s *, NUGCUTCHAR_s *, f32, i32);
static void CutScene_EvalCharacter(instNUGCUTSCENE_s *, NUGCUTSCENE_s *, instNUGCUTCHAR_s *, NUGCUTCHAR_s *, f32);
static void CutScene_FindCharacters(NUGCUTSCENE_s *);
static void CutScene_ResetCharacters(instNUGCUTSCENE_s *);
static void CutScene_RigidPostRender(NUGCUTRIGID_s *, instNUGCUTRIGID_s *, NUMTX *);
static void CutScene_CreateCharacterInstance(NUGCUTCHAR_s *, instNUGCUTCHAR_s *, variptr_u *);

extern "C" {
    i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance);
    void instNuGCutScenePause(instNUGCUTSCENE_s *, u8);
    void instNuGCutSceneReset(instNUGCUTSCENE_s *);
    void instNuGCutSceneStart(instNUGCUTSCENE_s *);
    void instNuGCutSceneStop(instNUGCUTSCENE_s *);
    void instNuGCutSceneDestroy(instNUGCUTSCENE_s *);
    void NuGCutSceneSysRender(i32);
    void NuGCutSceneSysUpdate(i32, i32, f32);
}

void SetLevelLights(void *, f32);
void NewLevelFromMenu(LEVELDATA_s *, i32, i32, i32);
void FindAndSetLights(NUVEC *, f32, void *);
void SetZeroLights(void);
void GameFog_Reset(void);
void EnableShadowMapRendering(i32);
void ResetShadowMapRendering(void);
i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
f32 FindReflectionNoPlatforms(NUVEC *);
extern "C" i32 NewShadowOnPlatform(void);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
i32 qrand(void);
void NewRumbleAllPlayers(f32, f32, i32, i32);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
extern "C" void APITransparentCharDraw(nuhgobj_s *, NUMTX *, i32, i16 *, NUMTX *, void **, i32);
extern "C" void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                        NUGCUTLOCATOR_s *, f32, NUMTX *, i32);
CUTSCENEPLAYERCLIP *CutScenePlayer_Active(void);
void CutScenePlayer_SetObjects(CUTINFO *);
extern "C" void DebrisSetRenderGroup(i32);
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;


extern "C" {
    extern i16 id_ANAKINSPODGREEN;
    extern i16 id_ANAKINSNEWPOD;
    extern i16 id_ANAKINSNEWPODGREEN;
    extern i16 id_REPUBLICGUNSHIP_GREEN;
    extern i16 id_NEW_REPUBLIC_GUNSHIP_GREEN;
    extern i16 id_JEDISTARFIGHTERYELLOWEP3;
}

extern i32 CutInstEndCount;
extern i32 NewCutInfoCount;
extern u8 cutskip_dontplaylevelintro;
extern i32 CUTNOFOG;
extern i32 CUTSKIPLOCK;
extern f32 LevelNameMul;
extern f32 LevelNameTime;

extern "C" i32 drawcharactermodel_nobsa;
extern "C" i32 NewMode;
void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);

extern "C" {
    void PauseGameCut(void);
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
    extern instNUGCUTSCENE_s *cutscene_load_instance;
    void instNuGCutSceneServiceLoad(void);
    void NuGCutSceneSysPostBackgroundLoad(void);
}



// Retail only keeps the split-out body of the IPA-SRA clone (the unused
// cutscene index dropped), called with world and cut in registers.
#if defined(__i386__)
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone))
#endif
static CUTSCENE_START_ATTR i32 CutScene_StartBody(WORLDINFO_s *world, CUTINFO *cut)
    __asm__("_ZL14CutScene_StartP11WORLDINFO_sP7CUTINFOi.isra.24.part.25");


void CutScenes_End();

void CutScenes_Draw(WORLDINFO_s *world);

void CutScenes_Stop(CUTSYS *system);

void CutScenes_Reset(WORLDINFO_s *world);

void CutScenes_Start(WORLDINFO_s *world);

void CutScenes_Update(WORLDINFO_s *world, i32 paused);

CUTINFO *CutScene_Find(CUTSYS *cutscene_system, char *name);

void CutScenes_Destroy(CUTSYS *system);

void *CutScene_FindInst(CUTSYS *system, char *name);

i32 CutScene_HasPlayed(CUTINFO *cut);

void CutScene_SnapToEnd(CUTINFO *cut);

void CutScene_StartAudio();

i32 CutScene_IsSkippable(CUTINFO *cut);

i32 CutScene_StartFn_LSW(CUTINFO *cut);

extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);



void CutScenes_InitSystem(CUTSCENESYS *system);

extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s *dco_gcdata;
extern CHARACTERMODEL_s *dco_cmodel;







// The loop re-reads the active cut through CutStopInfo after each SmartTextEx call.
#define CUT_STOP static_cast<CUTINFO *>(CutStopInfo)
void CutScene_DrawSubtitles();

void CutScene_StoppedFn_LSW(CUTINFO *cut);

void CutScenes_BGLoadManager();

void CutScenes_ConfigureList(char *filename, variptr_u *buffer, variptr_u);

void CutScene_PreUpdateFn_LSW(CUTINFO *cut);

void CutScene_PostUpdateFn_LSW();

i32 CutScene_PlayingOrRequested(CUTINFO *cut);

i32 CutScene_ReplaceCharacterModelFn_LSW(CUTINFO *cut, NUGCUTCHAR_s *character);

CUTINFO *NewCutScene(CUTINFO *cut, CUTSYS *system, char *name, i32);

void ResetScene(nugscn_s *scene, SCENEPROGRESS_s *progress);

void NextStatusStage(STATUSPACKET_s *);
void FinishStatusPacket(i32);
f32 StatusIconsOnOff(f32);
extern f32 icon_y;

void Exit_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

void Fade_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

extern "C" void *NuAnimData2FixPtrs(void *, isize, isize, i32);
extern "C" StateAnim *StateAnimFixPtrs(StateAnim *, isize);

NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *source, variptr_u *buffer);

extern i32 STATUS_R;
extern i32 STATUS_G;
extern i32 STATUS_B;
extern f32 iconalphaoverride;
extern i16 tTRUEJEDI, tSTORY, tFREEPLAY;
extern i16 tLEVELCOMPLETE, tFREEPLAYUNLOCKED, tSUPERSTORYCOMPLETE, tBONUSUNLOCKED;
extern i16 tEPISODEVUNLOCKED, tEPISODEVIUNLOCKED, tCHALLENGECOMPLETE, tOUTOFTIME;
extern i16 tYOUDIDNTCOLLECTALL10CANISTERS, tMISSIONCOMPLETE, tSTORYCLIPSUNLOCKED;
extern i16 tEPISODEICOMPLETE, tEPISODEIICOMPLETE, tEPISODEIIICOMPLETE;
extern i16 tEPISODEIVCOMPLETE, tEPISODEVCOMPLETE, tEPISODEVICOMPLETE;
f32 getFinishedStatusAlpha(STATUSPACKET_s *);
void IncreaseScore(u32 *, u64, i32);
void NewStatusRumbleBuzz(i32, f32, f32, i32);
void Text_MakeScore(u32, char *);
void AddFancyMessage(char *, f32, f32, f32, f32, i32, i32);
void *AddGameMessage(char *, nuvec_s *, f32, nuvec_s *, f32, u8, u8, u8, u32, f32);
extern "C" void PlaySfx(char *, nuvec_s *);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);

void GoldBrick_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void GoldBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);

void GoldBrick_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

void LevelComplete_LSW_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet);

void SetDrawGoldBrick(STATUSPACKET_s *, i32);
void NewStatusRumbleBuzz(i32, f32, f32, i32);
extern "C" void PlaySfx(char *, nuvec_s *);

void LevelComplete_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

// ==== functions in original address order ====







extern "C" {
void NuGCutSceneSysBackgroundFlush(void);

extern "C" void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *locator_functions);

}

void NuGCutSceneSysInitVfx(NUGCUTLOOKUPLOCATORVFXFN lookup, NUGCUTTRIGGERLOCATORVFXFN trigger,
                           NUGCUTRELEASELOCATORVFXFN release, NUGCUTUPDATELOCATORVFXFN update);

void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);

void NuGCutSceneRemapFocusIdToLocaterNum(NUGCUTSCENE_s *cutscene, VARIPTR *buffer);

extern "C" {
NUGCUTSCENE_s *NuGCutSceneLoad(char *name, VARIPTR *buf, VARIPTR *buf_end, i32 flags);

NUGCUTSCENE_s *NuGCutSceneLoadAddr(NUGCUTSCENE_s *cutscene, i32 loaded_size, VARIPTR *buffer);

void NuGCutSceneDestroy(NUGCUTSCENE_s *cutscene);

void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene, NUGSCN *scene, i32 flags, i32 area);

void NuGCutSceneFixUpExtra(NUGCUTSCENE_s *cutscene, NUGSCN *area);

void NuGCutSceneSysPostBackgroundLoad(void);

void PetesHackOfDeath(void);

instNUGCUTSCENE_s *instNuGCutSceneCreate(NUGCUTSCENE_s *cutscene, NUGSCN *scene, void *extra, char *name,
                                             VARIPTR *buf, i32 flags);

void instNuGCutSceneStop(instNUGCUTSCENE_s *instance);

i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTimeLeft(instNUGCUTSCENE_s *instance);

f32 instCutSceneTimeElapsed(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTotalTime(instNUGCUTSCENE_s *instance);

void instNuGCutSceneReset(instNUGCUTSCENE_s *instance);

void gcutSetSoundVol(i32 sound_volume, i32 music_volume);

void instNuGCutSceneStart(instNUGCUTSCENE_s *instance);

void instNuGCutScenePlay(instNUGCUTSCENE_s *instance, i32 forward);

void instNuGCutScenePause(instNUGCUTSCENE_s *instance, u8 paused);

}

void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);

extern "C" {
void instNuGCutSceneEnable(instNUGCUTSCENE_s *instance);

void instNuGCutSceneDisable(instNUGCUTSCENE_s *instance);

void instNuGCutSceneSetPos(instNUGCUTSCENE_s *instance, NUVEC *pos);

void instNuGCutSceneTranslate(instNUGCUTSCENE_s *instance, NUVEC *translation);

void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix);

void instNuGCutSceneSetRepeat(instNUGCUTSCENE_s *instance, i32 repeat);

void instNuGCutSceneWaitAtEnd(instNUGCUTSCENE_s *instance, u8 enabled);

void instNuGCutSceneChain(instNUGCUTSCENE_s *instance, instNUGCUTSCENE_s *next);

void instNuGCutSceneSetEndCallback(instNUGCUTSCENE_s *instance, void (*callback)(instNUGCUTSCENE_s *));

void instNuGCutSceneRotateY(instNUGCUTSCENE_s *instance, NUANG angle);

void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf);

void instNuGCutSoundStream(void);

instNUGCUTSCENE_s *instNuGCutSceneFind(const char *name);

i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index);

}

void NewCopyAnims(instNUGCUTSCENE_s *instance);

extern "C" {
i32 NuGCutSceneIsBackgroundLoading(void);

i32 instNuGCutScenePreload(instNUGCUTSCENE_s *instance);

}

i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *instance, i32 force);

extern "C" {
void instNuGCutSceneServiceLoad(void);

void NuGCutSetCutAudioStream(i32 stream);

i32 instNuGCutSceneCharGetStartMtx(instNUGCUTSCENE_s *instance, const char *name, NUMTX *out);

}

f32 *NuGCutLocatorCalcMtx_3(NUGCUTLOCATOR_s *locator, numtx_s *mtx, float frame);

extern "C" {
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *locator, float frame, NUMTX *mtx, nuanimtime_s *time);

}


i32 NuGCutLocatorIsVisble_3(NUGCUTLOCATOR_s *locator, float frame, float *scale, float *rate);

extern "C" {
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *locator, float frame, nuanimtime_s *time, float *scale,
                                     float *rate);

}

i32 FindMtlInHGObj(nugscn_s *scene, i32 material_type);

i32 FindTexAnimFromMtl(nugscn_s *scene, numtl_s *material);

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *opaque_state);

extern "C" {
void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out);

}

void EvaluateJointOrientationMtx(nugscn_s *scene, i32 joint_index, numtx_s *matrix);


extern "C" {
extern "C" void instNuGCutSceneJumpToEnd(instNUGCUTSCENE_s *instance);

}

void instNuGCutSceneEndFirstFrame(instNUGCUTSCENE_s *instance);

extern "C" {
extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

void instNuGCutSceneDestroy(instNUGCUTSCENE_s *instance);

extern "C" void instNuGCutSceneJumpToLastFrame(instNUGCUTSCENE_s *instance);

}

i32 instNuGCutContainsInstancedRigids(instNUGCUTSCENE_s *instance);

i32 instNuGCutGetNextRigidInfo(instNUGCUTSCENE_s *instance, float frame, i32 index, numtx_s *matrix,
                               nuhspecial_s *special);

extern "C" {
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

}


extern "C" {
void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *instance, NUGCUTLOCATORSYS_s *system,
                                 instNUGCUTLOCATOR_s *inst_locator, NUGCUTLOCATOR_s *locator, f32 frame,
                                 NUMTX *parent_mtx, i32 paused);

}




extern "C" {
extern "C" void NuGCutSceneSysUpdate(i32 paused, i32 skip, f32 elapsed);

extern "C" void NuGCutSceneSysRender(i32 paused);

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

void instNuGCutSceneCalculateAverageCentre(instNUGCUTSCENE_s *instance, NUMTX *matrix, NUVEC *out);

void instNuGCutSceneResetCleanUp(void *list_storage, void *cut_base, void *cut_end, void *inst_base, void *inst_end,
                                     instNUGCUTSCENE_s *(*get_inst)(void *),
                                     void *(*create_inst)(void *, NUGCUTSCENE_s *, VARIPTR *),
                                     void (*inst_destroyed)(void *));

void instNuGCutSceneAddCleanUpItem(void *handle, i32 enabled);

}

NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *source, variptr_u *buffer);

extern "C" {
void *instNuGCutSceneCleanUp(void);

void DisplayCutSceneMemory(void);

}

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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern MENUPACKET_s MenuPacket;
extern i32 NextArea_FreePlay;
void InitMission(MISSIONSYS *, i32);
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void BackupMenu(void);
extern "C" void BackupMenuNoFn(void);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                                      u8 alpha);
extern "C" void DrawMenuButtonPromptsEx(i32 confirm_prompt, i32 cancel_prompt, i32 flags, i32 enabled, u8 red, u8 green,
                                        u8 blue, u8 alpha);
extern i32 SAVESLOTS;
extern i32 MenuSFX;
extern i32 MENUSFX_MENUSELECT;
extern i32 MENUSFX_MENUBACK;
extern i32 MENUSFX_MENUNOENTRY;
extern i32 MENUSFX_MENUMOVE;
extern char *apitxt_LOADGAME;
extern char *apitxt_SAVEGAME;
extern char *apitxt_CONFIRMSAVE;
extern char *apitxt_CONFIRMOVERWRITE;
extern char *apitxt_CONFIRMLOAD;
extern char *apitxt_DOYOUWANTTOABORT;
extern char *apitxt_DOYOUWANTTOABORTLOAD;
extern char *apitxt_DOYOUWANTTOABORTFORMAT;
extern char *apitxt_CONFIRMDELETE;
extern char *apitxt_RETRY;
extern char *apitxt_SLOT;
extern char *apitxt_NEWSAVE;
extern char *apitxt_CANCEL;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_NOTENOUGHSPACE;
extern char *apitxt_DELETEGAME;
extern char *apitxt_DELETING;
extern char *apitxt_DELETECOMPLETE;
extern char *apitxt_FORMATTING;
extern char *apitxt_FORMATTINGCOMPLETE;
extern char *apiGameName;
extern char *apitxt_YES;
extern char *apitxt_NO;
extern char **TTab;
extern i16 tSELECTCONTROLS;
extern i16 tCONTROLSCANBECHANGED;
extern i16 tCLASSIC;
extern i16 tTOUCHSCREEN;
extern i16 tCONTROLLERCONNECTED;
extern i16 tCONTROLS;
extern i16 tCONSOLE;
extern i16 tTOUCH;
extern i16 tSURROUNDSOUND;
extern i16 tAUDIOVOLUME;
extern i16 tMUSIC;
extern i16 tWIDESCREEN;
extern i16 tON;
extern i16 tOFF;
extern i16 tACCEPT;
extern i16 tBACK;
extern i16 tCONTINUE;
extern i16 tCANCEL;
extern i16 tSTORY;
extern i16 tFREEPLAY;
extern i16 tREPLAYSTORY;
extern i16 tLOCKED;
extern i16 tAVAILABLETOBUY;
extern i16 tPOWERBRICK;
extern i16 tHOWTOPLAY;
extern "C" OPTIONSSAVE TempOptions;
extern i32 GAMEDEMO;
extern i32 menu_flash;
extern i16 tOUTOFTIME;
extern f32 minikittime;
extern "C" TIMER BonusTimer;
extern f32 text3d_height;
extern f32 text3d_width;
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
void GameDrawMenuEntry(MENU *menu, char *text);
i32 GetParentMenuID(void);
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
void SfxCheckMusicOnOff(OPTIONSSAVE *options);
void NewGame(void);
void NuIOS_RestoreInAppPurchases(void);
extern i32 startnewgame;
extern i32 startnewgame_initiated;
extern f32 newgamecamtime;
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;
extern "C" f32 sfx_wait;
extern "C" f32 MENUBOTY;
void Draw_AUTOSAVEWARNING();
void Draw_NODATAAVAILABLE();
void Draw_NOMEMORYCARD();
extern "C" void Draw_NOTENOUGHSPACE(void);
extern "C" void Draw_SPACENEEDED(void);
extern "C" void Draw_CHECKINGMEMORYCARD(void);
extern "C" void Draw_DONOTREMOVEMEMORYCARD(void);
void Draw_OK(MENU_s *menu);

extern i32 memcard_cardchanged;
extern i32 MenuASCancelFinished;
extern i32 MenuCardWarningState;
extern i32 ButtonScaleMode;
extern i32 Menu_InLoadFlow;
extern i32 Menu_InWarningFlow;
extern i32 Menu_LastFlow;
extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 Menu_InSaveFlow;
extern i32 Menu_DisableCancel;
extern i32 Menu_InHeaderSave;
extern i32 Menu_Overwriteflag;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern i32 memcard_deleteneeded;
extern i32 memcard_deletestarted;
extern i32 memcard_deletefailed;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
extern i32 memcard_formatting;
extern i32 memcard_formatme;
extern i32 memcard_formatfailed;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 SAVESIZE;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

extern char * extrasavetext;
extern f32 extrasavealpha;
extern u8 RAP_WARNING_B;
extern u8 RAP_WARNING_G;
extern u8 RAP_WARNING_R;


extern i32 hub_new_level;
extern i32 hub_selectmode;
extern f32 selectmodetime;
extern i32 selectmodemode;
extern "C" i32 FinishLoop_On;
extern FadeSystem FadeSys;
void Hub_DrawFreePlaySelect();
void Hub_InitFreePlaySelect(i32 area, i32 first_model, i32 second_model);
void Hub_UpdateFreePlaySelect();
void WipeBackToHub();
void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);




void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

void MenuDrawLoad(MENU_s *menu);

void MenuDrawSave(MENU_s *menu);


void MenuExitLoad(MENU_s *);

void MenuExitSave(MENU_s *);

void MenuDrawClips(MENU_s *);

void MenuEnterLoad(MENU_s *menu);

void MenuEnterSave(MENU_s *menu);

void MenuInitClips(MENU_s *);

void MenuStartLoad();

void MenuStartSave();




void MenuDrawNoData(MENU_s *menu);

void MenuDrawSaving(MENU_s *);

void MenuUpdateLoad(MENU_s *menu);

void MenuUpdateSave(MENU_s *menu);




void IconScenes_Dump();

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *);

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end);

void MenuDrawLoading(MENU_s *);




i32 MenuIsAvailable();

void MenuUpdateClips(MENU_s *);

void MenuDrawDeleting(MENU_s *);

void MenuDrawFreePlay(MENU_s *);



void MenuInitFreePlay(MENU_s *menu);



void MenuUpdateNoData(MENU_s *menu);

void MenuUpdateLoading(MENU_s *);

void MenuUpdateSaving(MENU_s *);



NUGSCN *IconScene_FindById(i32 character_id);


void MenuDrawFormatting(MENU_s *);

void MenuDrawInsertCard(MENU_s *);

void MenuDrawLoadCancel(MENU_s *menu);

void MenuDrawSaveCancel(MENU_s *menu);

void MenuUpdateDeleting(MENU_s *);

void MenuUpdateFreePlay(MENU_s *);


void MenuDrawCardWarning(MENU_s *);

void MenuDrawFileCorrupt(MENU_s *);

void MenuDrawLoadConfirm(MENU_s *menu);

void MenuDrawSaveConfirm(MENU_s *menu);

void MenuEnterHeaderSave(MENU_s *);

void MenuEnterInsertCard(MENU_s *);

void MenuExitCardWarning(MENU_s *);

void MenuDrawEndChallenge(MENU_s *);

void MenuDrawFormatCancel(MENU_s *menu);

void MenuDrawNoMemoryCard(MENU_s *menu);

void MenuEnterCardWarning(MENU_s *);

void MenuEnterSaveConfirm(MENU_s *);


void MenuUpdateFormatting(MENU_s *);

void MenuUpdateLoadCancel(MENU_s *menu);

void MenuUpdateSaveCancel(MENU_s *menu);

void MenuUpdateInsertCard(MENU_s *menu);

void MenuDrawDeleteConfirm(MENU_s *menu);

void MenuDrawFormatConfirm(MENU_s *);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuEnterNoMemoryCard(MENU_s *);

void MenuEnterStartNewGame(MENU_s *);

void MenuUpdateCardWarning(MENU_s *menu);

void MenuUpdateFileCorrupt(MENU_s *menu);

void MenuUpdateLoadConfirm(MENU_s *menu);

void MenuUpdateSaveConfirm(MENU_s *menu);

void MenuDrawAutoSaveCancel(MENU_s *menu);

void MenuDrawNotEnoughSpace(MENU_s *);



void MenuUpdateEndChallenge(MENU_s *menu);

void MenuUpdateFormatCancel(MENU_s *menu);

void MenuUpdateNoMemoryCard(MENU_s *menu);

void MenuDrawAutoSaveWarning(MENU_s *menu);

void MenuDrawDoNotRemoveCard(MENU_s *);

void MenuEnterAutoSaveCancel(MENU_s *menu);

void MenuUpdateDeleteConfirm(MENU_s *menu);

void MenuUpdateFormatConfirm(MENU_s *menu);

void MenuEnterAutoSaveWarning(MENU_s *);

extern f32 memcard_autosavecanceldelay;
void MenuUpdateAutoSaveCancel(MENU_s *menu);

void MenuUpdateNotEnoughSpace(MENU_s *menu);



void MenuUpdateAutoSaveWarning(MENU_s *menu);

void MenuUpdateDoNotRemoveCard(MENU_s *menu);

extern "C" {

extern f32 MENUDY;
extern f32 MENUBOTY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern f32 MENUENTRYEXWIDTH;
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
extern void (*headerdrawfn)();
extern void (*predrawfn)(MENU *);


void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void BackupMenuNoFn(void);

void DrawMenu(i32 paused);

void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha);

void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue);

void Draw_CANCEL(MENU *menu);

void Draw_CHECKINGMEMORYCARD(void);

void Draw_DONOTREMOVEMEMORYCARD(void);

void Draw_NOTENOUGHSPACE(void);

void Draw_SPACENEEDED(void);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

void MenuDrawBackground(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b);

void MenuSetPulsateSpeed(f32 speed);

void MenuSetTopBottom(f32 top, f32 bottom);

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));

void MenuReset(void);

void RemapAddr(void *new_base, void *old_base, void **address);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);
extern "C" void loadsaveCallEachFrame(void);
extern "C" bool TestForController(void);
i32 GetMenuID(void);
void MenuDrawBackground(void);
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_TOGGLERIGHT;
extern "C" GAMEPAD_s GamePad[64];
extern i32 readpads_always;
extern "C" i32 reset_area;
extern i32 MenuSFX;

eduimenu_s *GetMenuActiveChild(eduimenu_s *menu);
void ResizePauseScreenTexture(i32, i32);

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);

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
extern "C" f32 MainRenderTime;
extern "C" i32 editor_active;
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

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);

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


extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);
extern "C" void BackupMenu(void);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" i32 NuIOS_IsWidescreen(void);
f32 GameGetMusicVolume(OPTIONSSAVE_s *options);
f32 GameSetSoundVolume(OPTIONSSAVE_s *options);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 volume, i32 flags);
void legoSetMusicVolume(f32 volume);
extern u8 cutskip_dontplaylevelintro;
void Hint_LoadAllGameState(void);
void NuIOS_RestoreInAppPurchases(void);
void NewGame(void);
void Hub_UpdateKit(void);
void MenuDrawNewGame(MENU *menu);
void MenuUpdateNewGame(MENU *menu);
void MenuDrawLoad(MENU *menu);
void MenuDrawLoading(MENU *menu);
void MenuDrawAutoSaveCancel(MENU *menu);
void MenuDrawAutoSaveWarning(MENU *menu);
void MenuDrawCardWarning(MENU *menu);
void MenuDrawDeleteConfirm(MENU *menu);
void MenuDrawDeleting(MENU *menu);
void MenuDrawDoNotRemoveCard(MENU *menu);
void MenuDrawFileCorrupt(MENU *menu);
void MenuDrawFormatCancel(MENU *menu);
void MenuDrawFormatConfirm(MENU *menu);
void MenuDrawFormatting(MENU *menu);
void MenuDrawInsertCard(MENU *menu);
void MenuDrawLoadCancel(MENU *menu);
void MenuDrawLoadConfirm(MENU *menu);
void MenuDrawNoData(MENU *menu);
void MenuDrawNoMemoryCard(MENU *menu);
void MenuDrawNotEnoughSpace(MENU *menu);
void MenuDrawSave(MENU *menu);
void MenuDrawSaveCancel(MENU *menu);
void MenuDrawSaveConfirm(MENU *menu);
void MenuDrawSaving(MENU *menu);
void MenuEnterAutoSaveCancel(MENU *menu);
void MenuEnterAutoSaveWarning(MENU *menu);
void MenuEnterCardWarning(MENU *menu);
void MenuEnterHeaderSave(MENU *menu);
void MenuEnterInsertCard(MENU *menu);
void MenuEnterLoad(MENU *menu);
void MenuEnterNoMemoryCard(MENU *menu);
void MenuEnterSave(MENU *menu);
void MenuEnterSaveConfirm(MENU *menu);
void MenuEnterStartNewGame(MENU *menu);
void MenuExitCardWarning(MENU *menu);
void MenuExitLoad(MENU *menu);
void MenuExitSave(MENU *menu);
void MenuUpdateAutoSaveCancel(MENU *menu);
void MenuUpdateAutoSaveWarning(MENU *menu);
void MenuUpdateCardWarning(MENU *menu);
void MenuUpdateDeleteConfirm(MENU *menu);
void MenuUpdateDeleting(MENU *menu);
void MenuUpdateDoNotRemoveCard(MENU *menu);
void MenuUpdateFileCorrupt(MENU *menu);
void MenuUpdateFormatCancel(MENU *menu);
void MenuUpdateFormatConfirm(MENU *menu);
void MenuUpdateFormatting(MENU *menu);
void MenuUpdateInsertCard(MENU *menu);
void MenuUpdateLoad(MENU *menu);
void MenuUpdateLoadCancel(MENU *menu);
void MenuUpdateLoadConfirm(MENU *menu);
void MenuUpdateLoading(MENU *menu);
void MenuUpdateNoData(MENU *menu);
void MenuUpdateNoMemoryCard(MENU *menu);
void MenuUpdateNotEnoughSpace(MENU *menu);
void MenuUpdateSave(MENU *menu);
void MenuUpdateSaveCancel(MENU *menu);
void MenuUpdateSaveConfirm(MENU *menu);
void MenuUpdateSaving(MENU *menu);
extern char *Trailer[3];
extern "C" OPTIONSSAVE TempOptions;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;
void MusicClearAll(void);
extern "C" void SoundKillAll(void);
void GameDrawMenuEntry(MENU *menu, char *text);
void DropOut(i32 player, i32 show_effect, i32 silent, i32 force);
void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);
struct CUTSCENEPLAYERCLIP;
extern i32 saveload_autosave;
extern FadeSystem FadeSys;
extern i32 level_already_loaded;
extern "C" i32 NewMode;
extern i32 from_save_and_exit;
extern i16 tCANCEL;
extern u8 MENULOSTR1, MENULOSTG1, MENULOSTB1, MENULOSTA1;
extern u8 MENULOSTR2, MENULOSTG2, MENULOSTB2, MENULOSTA2;
extern i16 tALLLEVELPROGRESSWILLBELOST, tALLSUPERSTORYPROGRESSWILLBELOST;
extern i16 tGAMEPROGRESSWILLBELOST, tEXITTOMAP, tQUITGAME, tRESTARTDEMO2;
extern i16 tARCADE_NOWIN, tEXITWITHOUTSAVING, tSAVEANDEXIT;
extern "C" i32 SuperStory;
extern "C" GAMEPAD_s GamePad[64];
extern i32 pause_i_pad;

extern i16 tHELPANDOPTIONS;
extern i16 tEXTRAS;
extern i16 tDROPOUT;
extern i16 tMAP;
extern i16 tRESTARTDEMO;
extern i16 tQUIT;
extern i16 tRESTARTLEVEL;
extern i16 tYES;
extern i16 tNO;
extern i16 tCANCEL;
extern i16 tRESUME;
extern i16 tSKIP;
extern i16 tEXITTOMAP;
extern i16 tALLLEVELPROGRESSWILLBELOST;
extern i16 tALLSUPERSTORYPROGRESSWILLBELOST;
extern i16 tEXITWITHOUTSAVING;
extern i16 tSAVEANDEXIT;
extern i16 tRESTARTDEMO2;
extern i16 tGAMEPROGRESSWILLBELOST;
extern i16 tQUITGAME;
extern i16 tMUSIC;
extern i16 tHOWTOPLAY;
extern i16 tON;
extern i16 tOFF;
extern i16 tALLSTUDSCOLLECTED;
extern i16 tEXIT;
extern i16 tSELECTING;
extern i16 tCONTROLLER;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
extern u8 MENUEXITR;
extern u8 MENUEXITG;
extern u8 MENUEXITB;
extern i32 reset_restart;
extern u32 ResetBits;
void Door_Reset(void);

void Arcade_DrawEndMenu(MENU *menu);
void Arcade_UpdateEndMenu(MENU *menu);
void Credits_UpdateMenu(MENU *menu);
void CustomiserMenu_Draw(MENU *menu);
void CustomiserMenu_Update(MENU *menu);
void MenuDrawClips(MENU *menu);
void MenuDrawEndChallenge(MENU *menu);
void MenuDrawEndMission(MENU *menu);
void MenuDrawEpisodes(MENU *menu);
void MenuDrawExtras(MENU *menu);
void MenuDrawFreePlay(MENU *menu);
void MenuDrawMissions(MENU *menu);
void MenuDrawOptions(MENU *menu);
void MenuDrawRestoreNewGame(MENU *menu);
void MenuDrawSelectControls(MENU *menu);
void MenuEnterOptions(MENU *menu);
void MenuExitOptions(MENU *menu);
void MenuInitClips(MENU *menu);
void MenuInitFreePlay(MENU *menu);
void MenuInitMissions(MENU *menu);
void MenuUpdateClips(MENU *menu);
void MenuUpdateEndChallenge(MENU *menu);
void MenuUpdateEndMission(MENU *menu);
void MenuUpdateEpisodes(MENU *menu);
void MenuUpdateExtras(MENU *menu);
void MenuUpdateFreePlay(MENU *menu);
void MenuUpdateMissions(MENU *menu);
void MenuUpdateOptions(MENU *menu);
void MenuUpdateRestoreNewGame(MENU *menu);
void MenuUpdateSelectControls(MENU *menu);

extern MENU GameMenu[10];

extern i32 MenuSFX;
extern i32 MENUSFX_MENUMOVE;
extern i32 MENUSFX_MENUBACK;
extern i32 MENUSFX_MENUNOENTRY;
extern i32 MENUSFX_MENUSELECT;
extern i32 GameMenuLevel;
extern i32 MenuValidated;
extern i32 MenuResult;
extern i32 CurrentMenuId;
extern i32 from_save_and_exit;
extern i32 pausecut_skip_to_level;
extern u8 pausecut_skip_to_gameplay;
extern i32 startnewgame;
extern i32 startnewgame_initiated;
extern f32 newgamecamtime;
extern i32 newgamecam;
extern f32 MenuAlpha;
extern i32 MenuA;
extern f32 MENUTEXTSCALE;

extern i16 tTOUCHTOSTART;
extern i16 tNEWGAME;
extern i16 tLOADGAME;
extern i16 tCONTROLS;
extern i16 tCONTROLLERCONNECTED;
extern i16 tTOUCH;
extern i16 tCONSOLE;
extern "C" bool TestForController();
extern "C" void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment,
                            u8 red, u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box,
                            i32 suppress_draw, u32 alpha);
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);

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

extern char * apiGameName;
extern char * apiGameTitle;
extern char * apiGameSubTitle;

extern char * apitxt_YES;
extern char * apitxt_NO;
extern char * apitxt_OK;
extern char * apitxt_CANCEL;
extern char * apitxt_RETRY;
extern char * apitxt_SLOT;
extern char * apitxt_PRESENT;
extern char * apitxt_EMPTY;
extern char * apitxt_NOSPACE;
extern char * apitxt_CONTINUE;
extern char * apitxt_SAVEGAME;
extern char * apitxt_LOADGAME;
extern char * apitxt_DELETEGAME;
extern char * apitxt_GAMESAVED;
extern char * apitxt_GAMEOVERWRITTEN;
extern char * apitxt_GAMELOADED;
extern char * apitxt_DELETING;
extern char * apitxt_FORMATTING;
extern char * apitxt_GAMEOVERWRITING;
extern char * apitxt_CORRUPTLOAD;
extern char * apitxt_CONFIRMLOAD;
extern char * apitxt_CONFIRMSAVE;
extern char * apitxt_CONFIRMOVERWRITE;
extern char * apitxt_DOYOUWANTTOSTART;
extern char * apitxt_DOYOUWANTTOABORT;
extern char * apitxt_DOYOUWANTTOABORTLOAD;
extern char * apitxt_DOYOUWANTTOABORTFORMAT;
extern char * apitxt_CONTINUEWITHOUTSAVING;
extern char * apitxt_NEWSAVE;
extern char * apitxt_CONTINUEWITHOUTLOADING;
extern char * apitxt_COMPLETED;
extern char * apitxt_SELECT;
extern char * apitxt_BACK;
extern char * apitxt_VIBRATION;
extern char * apitxt_CONFIRMDELETE;
extern char * apitxt_PROGRESS;
extern char * apitxt_TIMEPLAYED;
extern char * apitxt_CONTROLLERREMOVED;
extern char * apitxt_PRESSSTART;
extern char * apitxt_GAMESAVING;
extern char * apitxt_GAMELOADING;
extern char * apitxt_FAILEDTOLOAD;
extern char * apitxt_FAILEDTOSAVE;
extern char * apitxt_FAILEDTOOVERWRITE;
extern char * apitxt_NODATAAVAILABLE;
extern char * apitxt_AUTOSAVE_ENABLED;
extern char * apitxt_FORMATTINGCOMPLETE;
extern char * apitxt_DELETECOMPLETE;
extern char * apitxt_NOTENOUGHSPACE;
extern char * apitxt_AUTOSAVE_CANCEL1;
extern char * apitxt_AUTOSAVE_CANCEL2;
extern char * apitxt_AUTOSAVE_CANCEL3;
extern char * apitxt_AUTOSAVE_WARNING;
extern char * apitxt_LOADING;
extern char * apitxt_SAVING;

extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern i32 shop_quit;
extern i32 shop_save_done;
extern GAMESAVE_s TempGame;

extern i32 hub_forceshopsave;
extern "C" i32 TriggerAutoSave(void);
void Hub_ClearStats();
void Text_FillInExtendedSaveInfo();
i32 UpdateAchievements(STATUSPACKET_s *packet);
void ReCalculateCompletionPoints();

void DrawShopPrompts();

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

extern "C" void NuSound3AddRumble(nupad_s *, f32, i32, i32, f32);
extern "C" void BackupMenu(void);
extern GAMECAMERA_s *GameCam;
extern WORLDINFO_s *WORLD;
extern i32 (*GamePads_IgnoreInputFn)(void);
extern NUPAD *pActivePad;
extern "C" {
    extern i32 Paused;
    extern i32 NewMode;
    extern "C++" FadeSystem FadeSys;
    extern i32 CutSceneWaiting;
}

// Original bss @0x127a500: 64 pads x 0x60 bytes.
extern GAMEPAD_s GamePad[64];
// Original bss @0x127a4e0.
extern i32 readpads_always;

extern u32 GAMEPAD_DRIGHT;
extern u32 GAMEPAD_DLEFT;
extern u32 GAMEPAD_DDOWN;
extern u32 GAMEPAD_DUP;
extern u32 GAMEPAD_TOGGLERIGHT;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_LIFT;
extern u32 GAMEPAD_TAG;
extern u32 GAMEPAD_SPECIAL;
extern u32 GAMEPAD_ACTION;
extern u32 GAMEPAD_JUMP;
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_R3;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;

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

void GamePads_NetHost();

void GamePads_NetReset(i32);

u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad);

void GamePads_NetClient();

i32 GamePads_SkipMovie();

extern "C" {

bool TestForController(void);

} // extern "C"

void PadOutPause(i32 port, WORLDINFO_s *world);

void ResetRumble(RUMBLEPACKET *packet);

void UpdateRumble(RUMBLEPACKET *packet);

void NewBuzzFrames(nupad_s *pad, i32 frames, i32);

void TakeHitRumble(GameObject_s *object, float strength);

extern f32 SpaceRumbleTimer;
extern "C" f32 NuFsqrt(f32);
extern "C" f32 NuRandFloat();
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern f32 MulDist;
extern f32 SpeedAdd;
extern f32 SpeedDist;
extern f32 MulAdd;

extern "C" void SetPreSeekStartPoint(f32 start_point);

void SpaceResetAudioPoint();

void SpaceRumbleProcess();

void NewRumbleAllPlayers(float strength, float duration, i32 frames, i32);

i32 ObjLookingWithLeftStick(GameObject_s *object);

void PerformPauseButtonStuff();

void VirtualControlDPad_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch);

void VirtualControlButton_OnDown_Callback(MechTouchUIElement &, TouchHolder &);

void VirtualControlButtonMover_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch);

void VirtualControlDPad_LockButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &);

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

typedef i16 (*hashfn_t)(void);

extern i32 SAVESLOTS;

extern void * memcard_savedata;
extern i32 memcard_savedatasize;
extern void * memcard_savedatabuffer;

extern void * memcard_extra_savedata;
extern i32 memcard_extra_savedatasize;
extern void * memcard_extra_savedatabuffer;

extern hashfn_t memcard_hashfn;
extern void (*memcard_drawasiconfn)();
extern i32 memcard_autosave;
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

extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 memcard_cardchanged;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);
extern i32 numfilteroutblocks;
extern char filteroutblocks[8][12];
extern i32 numfilterblocks;
extern char filterblocks[8][96];

extern char FS_FileList[65536];
extern char FS_LastFileName[64];
extern char FS_Path[256];
extern i32 FS_NumFiles;
extern i32 FS_CurrentPosFileNum;
extern i32 FS_CursorLine;
extern char FS_ParentEntry[8];
extern i32 FS_SortMode;
extern char * FS_CurrentCursorPos;
extern char * FS_CurrentPos;
extern char * FS_FileListEnd;
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

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);

extern f32 memcard_autosavecanceldelay;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
extern f32 memcard_createmessage_delay;
extern f32 memcard_createresult_delay;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
extern f32 memcard_message_delay;
extern f32 memcard_result_delay;
extern i32 memcard_justformatted;
extern i32 saveload_autosavedisabled;

void InitMemCard();

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *);

void SerialiseInt(EdStream &stream, void *data, i32);

void FS_GetDirList(char *path, char *filter, char *filter_out);

void SerialiseChar(EdStream &stream, void *data, i32);

i32 FS_PrevNameLen(char *name);


void FS_SortStrings(char *start, char *end, i32 mode);

void SerialiseFloat(EdStream &stream, void *data, i32);

void SerialiseNuMtx(EdStream &stream, void *data, i32);

void SerialiseNuVec(EdStream &stream, void *data, i32);

void SerialiseShort(EdStream &stream, void *data, i32);

void SerialiseVuMtx(EdStream &stream, void *data, i32);

void SerialiseVuVec(EdStream &stream, void *data, i32);

void FS_MoveCursorUp(i32 steps);

void SerialiseString(EdStream &stream, void *data, i32 count);

void SerialiseColour3(EdStream &stream, void *data, i32);

i32 FS_FileNameFilter(char *name);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material);

extern char * SortStrings[4];

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

void FS_MoveCursorDown(i32 steps);

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 getsaveload_status();

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

void SerialiseNuHSpecial(EdStream &stream, void *data, i32);

void SerialiseStringAddr(EdStream &stream, void *data, i32 count);

void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);

void FS_SetCursorToLastFileName();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {
extern void (*savesuccessfn)();
extern void * memcard_headerdata;
extern i32 memcard_headerdatasize;
extern void * memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

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
i32 FindNextBreak(unsigned char *, i32);
i32 FindNearestBreak(unsigned char *, i32);
extern char **TTab;
extern i16 tNULL;
extern i16 tUNKNOWN;
extern i16 tENGLISH;
extern i16 tFRENCH;
extern i16 tGERMAN;
extern i16 tITALIAN;
extern i16 tSPANISH;
extern i16 tDANISH;
extern i16 tPLAYER1, tPLAYER2, tIOSAUTOSAVEWARNING, tLOADING, tSAVING;
extern char *apitxt_AUTOSAVE_WARNING, *apitxt_LOADING, *apitxt_SAVING;
extern char * txt_NULL;
extern char * apitxt_ENGLISH;
extern char * apitxt_FRENCH;
extern char * apitxt_GERMAN;
extern char * apitxt_ITALIAN;
extern char * apitxt_SPANISH;
extern char * apitxt_DANISH;

// Original .data @0x667ce0; the names are filled in by the static initializer.
extern LANGUAGEDATA * Text_LanguageList;
extern f32 text3d_height;
extern f32 text3d_width;
extern void (*buttonmapfn)(char *, char *);
extern f32 (*pulsetimerfn)(f32);
extern "C" {
extern i32 smarttextex_drawmessagebox;
    void NuQFntSetJustifiedTolerances(float squash, float stretch);
    unsigned char *NuUnicodeCharFromUTF8(u16 *character, unsigned char *text);
    unsigned char *NuUTF8CharFromUnicode(unsigned char *text, u16 character);
    VUFNT *LoadGameFont(char *, char *, variptr_u *, variptr_u *, i32);
    VUFNT *LoadButtonFont(char *, char *, variptr_u *, variptr_u *, i32);
    void NuLanguageSet(i32 language);
    void NuQFntDestroy(VUFNT *font);
    i32 NuRndrBeginScene(i32 flags);
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
}
extern void (*Text_GameSetLanguageFn)(i32);
char *Text_GetLanguagePath(i32 language);
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count);
void IntroText_SetTextID(i32 id);
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length);
void Text_DecodeButtons(char *source, char *destination);
void GameDrawMenuEntry(MENU *menu, char *text);
extern "C" void BackupMenu(void);
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
void Text_LoadFont(char *path, variptr_u *buf, variptr_u *buf_end);
#include "nu2api/numath/numtx.h"
extern "C" {
    void NuMtxSetRotationX(NUMTX *mtx, i32 angle);
    void NuMtxRotateY(NUMTX *mtx, i32 angle);
    void NuMtxRotateZ(NUMTX *mtx, i32 angle);
    void NuMtxTranslate(NUMTX *mtx, NUVEC *vec);
}
void Text3DStringEncodeFont(unsigned char *src, u16 *dst, void *font);
void MatrixTextStringEncode(void *font, unsigned char *source, u16 *destination);
i32 SplitTextFindNextWS(unsigned char *text, i32 position);
extern "C" void TextDecode(char *source, unsigned char *dest);
extern "C" void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, i32 alpha);
extern "C" void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                          u8 green, u8 blue, i32 alpha);
void Text_MakeTime(float time, i32 show_hours, i32 show_minutes, i32 show_centiseconds, char *text);

extern f32 APITEXTSCALEX;
extern f32 APITEXTSCALEY;
extern i32 textcrawlactive;
extern i32 Arcade_TextCrawlID;
extern i32 Arcade_TextCrawlParagraphs;
extern i32 g_bgloadAsFastAsPossibleHack;

void TextCrawl_Init(TEXTCRAWL_s *crawl, i32 id, i32 unk);

void TextCrawl_Draw(float dt, i32 paragraphs, float alpha, char *text);
f32 TextPulseTimer(f32 delay);

void Text_InitTable(TEXTENTRY *entry, i32 first, i32 last);
void Text_MakeScore(u32 score, char *text);
extern i16 tALONGTIMEAGO;
void Text_LoadStrings(variptr_u *buf, variptr_u *buf_end);
void Text_SetLanguage(i32 language);
void *Text_IsFontLoaded();
void TextDecodeCodeword(char *source, char *destination);
extern f32 g_buttonFontScalePulse;
extern f32 smarttextex_longestwidth;
extern i32 MenuStopDraw;
extern i32 smarttext_fwn;
extern i32 apitext_half_rgb;
extern u8 text_preset_b[10];
extern u8 text_preset_g[10];
extern u8 text_preset_r[10];
extern i32 ButtonScaleMode;

f32 TextPrintSubstring(unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, i32 colour,
                       i32 button_font);
void Text_DecodeButtons(char *source, char *destination);
extern char * txtpath_JAPANESE;
extern char * txtpath_DANISH;
extern char * txtpath_SPANISH;
extern char * txtpath_ITALIAN;
extern char * txtpath_GERMAN;
extern char * txtpath_FRENCH;
extern char * txtpath_ENGLISH;

char *Text_GetLanguagePath(i32 language);
void Text_InitStringTable(i32 count, variptr_u *buf, variptr_u *);
void Text_InitLanguageList(LANGUAGEDATA *language_list);
void Text_SetLanguage_Game(i32 language);
void Text3DStringEncodeFont(unsigned char *src, u16 *dst, void *font);
void Text_InitDefaultStrings();
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count);
i32 Text_GetMaxOverallStrings();
void Text_LocaliseDecimalPoint(char *text);
void Text_ExpandAllButtonStrings(char *input, char *output);
void Text_FillInExtendedSaveInfo();
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length);
extern NUMTL * MessageMtl;
extern i32 MessageMtlInit;

extern "C" void MessageBoxInitMtl(void);

void DrawMessageBoxRGBA(f32 x, f32 y, f32 width, f32 height, u32 blue, u32 green, u32 red, u32 alpha, numtl_s *material,
                        i32 alignment, f32 scale);

void DrawMessageBox(i32, f32 x, f32 y, f32 width, f32 height);

extern "C" {
void FixUpButtonsInFont(VUFNT *game_font, VUFNT *button_font);

VUFNT *LoadButtonFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane);

VUFNT *LoadGameFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane);
void MatrixText(VUFNT *font, unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, u32 alignment,
                    u32 colour, NUMTX *matrix, i32 camera_relative);
void MenuSmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         i32 alpha);
void MenuText3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, u8 alpha);
void Set3DGameFont(VUFNT *font);
void SetGameFont(VUFNT *font);
void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                   u8 green, u8 blue, f32 max_width, i32 max_lines);
void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);
void SmartTextEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                      u32 alpha);
void SmartTextExDrop(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u32 red,
                         u32 green, u32 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         u32 alpha);
void SmartTextGetWidescreen(f32 *font_scale_x, f32 *coordinate_scale);
void SmartTextSetFWNMode(i32 mode);
void SmartTextSetFont(VUFNT *font);
void SmartTextSetWidescreen(f32 font_scale_x, f32 coordinate_scale);
i32 SplitText(unsigned char *text, f32 max_width);
void Text3D(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red, u8 green,
                u8 blue);
void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32, u32 alignment, u8 red, u8 green,
                  u8 blue, i32 alpha);
void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                   u8 green, u8 blue, i32 alpha);
void TextDecode(char *source, unsigned char *dest);
void TextRegisterButtonMapFn(void (*fn)(char *, char *));
void TextRegisterPulseTimerFn(f32 (*fn)(f32));
void UnloadGameFont(void);
}
void MenuUpdateViewTextStrings(MENU_s *menu);


void MenuDrawViewTextStrings(MENU_s *menu);

abi_ulong GetMatchLength(unsigned char *first, unsigned char *second, abi_ulong maximum);
i32 SplitTextFindNextWS(unsigned char *text, i32 position);
void MatrixTextStringEncode(void *font, unsigned char *source, u16 *destination);
extern "C" void Text3DStringEncode(char *src, u16 *dst);
extern "C" void SetQFont2D(void);
void IntroText_SetTextID(i32 id);
void IntroText_Draw(float alpha);

i32 GetMenuID(void);

i32 GetParentMenuID();

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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern f32 BUCK_RIDERJUMPCLEARANCE;
extern void (*BuckStartExtraFn)(GameObject_s *);
void NewRumble(nupad_s *, f32, i32);

void Buck_Start(GameObject_s *object, GameObject_s *rider);

extern i16 id_SNOWMOB;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

i32 DoBuckStart(GameObject_s *object);

pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support);

extern PART_s *boulder_part[2];
extern i32 boulder_blowup_type;
extern NUVEC boulder_oldpos[2];
extern "C" void KillPart(PART_s *, i32);

void Boulder_Kill(PART_s *part, i32);

void Boulder_Move(PART_s *part, float);

void Buck_MoveCode(GameObject_s *object, i32 start);

i32 FindNextBreak(unsigned char *text, i32 index);


i32 FindNearestBreak(unsigned char *text, i32 index);

void BuckStartExtra_LSW(GameObject_s *object);

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

void edpartDoInput(nupad_s *pad);
void edpartDetermineNearest(f32 distance);
void edpartDrawCursor();
void edpartHighlightNearest();
void edpartScaleType(i32 type, f32 scale);
void edpartInitType(i32 type);
void edpartDestroy(i32 index);
void edpartPlace(i32 index, nuvec_s *position);
void edpartCreate(nuvec_s *position, i32 type);
void edpartMultipleCopyCopy();
void edpartMultipleCopyClear();
i32 edpartSaveEffects(char *path, char page);

extern "C" {
extern i32 edpart_set_part;
extern i32 edpart_filter;
extern char edpart_filter_string[16];
extern i32 edpart_first_time_this_level;
extern i32 edpart_curr;
extern i32 edpart_create_type;
extern i32 edpart_nearest_orphans;
extern i32 edpart_nearest_duplicates;
extern i8 edpart_effect_list;
extern i32 edpart_emitrotx;
extern i32 edpart_emitroty;
extern i32 edpart_emitrotz;
extern i32 edpart_roty;
extern i32 edpart_rotz;
extern f32 edpart_offset;
extern i32 edpart_dpad_mode;
extern i32 edpart_copy_mode;
extern i32 edpart_copy_enclosed;
extern i32 edpart_copy_source[8];
extern i32 edpart_copy_source_count;
extern NUVEC edpart_copy_source_vec;
extern i32 edpart_copyrotz;
extern f32 edpart_copy_size;
extern i32 edpart_copyroty;
extern i32 edpart_snap_enabled;
extern i32 edpart_refroty;
extern i32 edpart_refrotz;
extern i32 edpart_cam_ax;
extern i32 edpart_cam_ay;
    extern NUVEC edpart_cam_pos;
extern i32 edpart_num_orphans;
extern i32 edpart_readout;
extern f32 edpart_scale_factor;
extern NUMTL * edpart_mtl;
extern NUMTL * edpart_boxmtl;
    extern i32 edpart_nearest;
    extern part_emit_s *edpart_nearest_emit;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern i32 part_platimpactcnt;
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern char edbits_general_save_directory[256], edbits_general_save_name[256], edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256], edbits_level_save_name[256], edbits_level_save_extension[256];
    extern i32 edbits_part_general_page, edbits_part_level_page;
    extern i32 edbits_override_backups;
    void ResetParts(void);
    extern eduimenu_s *edpart_active_menu;
    extern eduimenu_s *edpart_opt_menu;
    extern eduimenu_s *edpart_type_menu;
    void eduiMenuRender(eduimenu_s *menu);
extern eduimenu_s * edpart_active_menu;
extern eduimenu_s * edpart_opt_menu;
extern eduimenu_s * edpart_sscale_menu;
extern eduimenu_s * edpart_message_menu;
extern eduimenu_s * edpart_name_menu;
extern eduimenu_s * edpart_data_menu;
extern eduimenu_s * edpart_scaletype_menu;
extern eduimenu_s * edpart_switchtype_menu;
extern eduimenu_s * edpart_switch_menu;
extern eduimenu_s * edpart_soundcontrol_menu;
extern eduimenu_s * edpart_soundid_menu;
extern eduimenu_s * edpart_soundx_menu;
extern eduimenu_s * edpart_sounds_menu;
extern eduimenu_s * edpart_debrisscale_menu;
extern eduimenu_s * edpart_partindex_menu;
extern eduimenu_s * edpart_impactpart_menu;
extern eduimenu_s * edpart_debrisindex_menu;
extern eduimenu_s * edpart_diedebris_menu;
extern eduimenu_s * edpart_impactdebris_menu;
extern eduimenu_s * edpart_emitterdebris_menu;
extern eduimenu_s * edpart_trail2debris_menu;
extern eduimenu_s * edpart_trail1debris_menu;
extern eduimenu_s * edpart_debrissettings_menu;
extern eduimenu_s * edpart_cutoff_menu;
extern eduimenu_s * edpart_emittime_menu;
extern eduimenu_s * edpart_changegenrate_menu;
extern eduimenu_s * edpart_varemit_menu;
extern eduimenu_s * edpart_varstart_menu;
extern eduimenu_s * edpart_grav_menu;
extern eduimenu_s * edpart_emitvel_menu;
extern eduimenu_s * edpart_emit_menu;
extern eduimenu_s * edpart_instancescale_menu;
extern eduimenu_s * edpart_tint_menu;
extern eduimenu_s * edpart_instanceflags_menu;
extern eduimenu_s * edpart_instorient_menu;
extern eduimenu_s * edpart_maxlife_menu;
extern eduimenu_s * edpart_instancesettings_menu;
extern eduimenu_s * edpart_instanceorphans_menu;
extern eduimenu_s * edpart_thingsinstance_menu;
extern eduimenu_s * edpart_worldinstance_menu;
extern eduimenu_s * edpart_instance_menu;
extern eduimenu_s * edpart_leveltype_menu;
extern eduimenu_s * edpart_generaltype_menu;
extern eduimenu_s * edpart_type_menu;
}




extern "C" {
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern eduimenu_s *edpart_sscale_menu;
    extern eduimenu_s *edpart_cutoff_menu;
    extern eduimenu_s *edpart_emittime_menu;
    extern eduimenu_s *edpart_scaletype_menu;
    extern eduimenu_s *edpart_debrisscale_menu;
    extern eduimenu_s *edpart_instancescale_menu;
    extern eduimenu_s *edpart_changegenrate_menu;
    extern eduimenu_s *edpart_name_menu;
    extern i32 edpart_nearest;
    extern i32 edpart_create_type;
    extern f32 edpart_scale_factor;
    extern part_typedesc_s part_types[128];
}

static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *);
static void edpartSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSoundControlMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);
static void edpartSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsGeneral(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsLevel(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddGeneralType(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartGeneralPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *, u32);
static void edpartWorldInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartThingsInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartInstanceOrphansMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void edpartDeleteInstanceOrphan(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceDuplicates(eduimenu_s *, eduiitem_s *, u32);

extern "C" {
extern i32 edpart_which_scene;
extern i32 edpart_set_debris;
extern i32 edpart_part_list;
extern i32 edpart_particle_list;
}


static void edpartInit();
static i32 edpartProc(float delta_time, nupad_s *pad);
static void edpartApply();
static void edpartClose();
static void edpartEnter();
static void edpartRender();
static void edpartSelType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartCopyType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartDataMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartEmitMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartGravMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartTintMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeGrav(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeName(eduimenu_s *, eduiitem_s *item, u32);
static void edpartCutOffMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartSoundXMenu(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartSoundsMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSwitchMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSoundIDMenu(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartVarEmitMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitTimeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartInstanceMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartToggleFilter(eduimenu_s *, eduiitem_s *item, u32);
static void edpartVarStartMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartCancelOptMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *item, u32);
static void edpartDieDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartLevelTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartScaleTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartAddGeneralType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *item, u32);
static void edpartImpactPartMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSwitchTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangePartIndex(eduimenu_s *, eduiitem_s *item, u32);
static void edpartDebrisScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);


static void edpartFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeFilterName(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *item, u32);

static void edpartImpactDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartTrail1DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartTrail2DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeDebrisIndex(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeGenRateMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeMaxLifeMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartEmitterDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceFlagsMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartWorldInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *item, u32);

static void edpartDebrisSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceOrientMenu(eduimenu_s *menu, eduiitem_s *, u32);

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartPartIndexMenu(eduimenu_s *menu) __asm__("_ZL19edpartPartIndexMenuP10eduimenu_sP10eduiitem_sj.isra.12");
static void edpartPartIndexMenu(eduimenu_s *menu);

static void edpartLevelPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartThingsInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartInstanceOrphansMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((optimize("O3"))) void edpartDeleteInstanceOrphan(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartFileSaveEffectsLevel(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartInstanceSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32);

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartDebrisIndexMenu(eduimenu_s *menu) __asm__("_ZL21edpartDebrisIndexMenuP10eduimenu_sP10eduiitem_sj.isra.13");
static void edpartDebrisIndexMenu(eduimenu_s *menu);

static void edpartLevelDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartFileSaveEffectsGeneral(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceOrphans(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceDuplicates(eduimenu_s *menu, eduiitem_s *, u32);

void edpartDoInput(nupad_s *pad);

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

extern i16 TerrImpact;
extern i32 terrhitflags;
extern i32 TERRAINMASK_NONWEAPON;
extern i32 TERRAINMASK_NONDROID;
extern i32 geom;
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
extern f32 TerrPlatScanDist;
extern i32 cntrots;
#include "legoapi/render/core/terrain_stubs_types.h"
void ScanTerrIDRemovePlat(i32 platform_index);

void TerrainSkinAllocate(terrsitu_s *terrain_group);

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

TERRAIN_TRACK_SLOT *ScanTerrId(void *hit_flags);
void ScanTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
i32 PlatformChecks(i32 count, NUVEC *movement);
i32 HitTerrain(void);
void TerrainImpactNorm(void);
void RayImpact(NUVEC *);
void StorePlatImpact(void);
i32 TerrainPlatformEmbedded(NUVEC *movement);
i32 TerrShapeSideStep(NUVEC *position, NUVEC *movement, u8 *hit_flags);
void TerrainImpact(NUVEC *position, NUVEC *movement, u8 *hit_flags);
void TerrFlush(void);
void NewScanRot(NUVEC *position, i32 terrain_mask);
f32 NewCast(NUVEC *position, f32 height_above, f32 height_below);
void TerrDrawPlat(tertype *terrain, i16 index);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
i32 DebrisSingleCollisionCheckScaleYFlag(i32, NUVEC *, f32, f32, u8);
i32 DebrisSingleTorusCollisionCheckScaleYFlag(i32, NUVEC *, f32, f32, u8);

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

extern "C" i32 edppLoadPage(char *path, i32 flag, usize scene);
// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain);
extern "C" void TerrSetPlatScanDist(f32 dist);

extern "C" void TerrainPlatformNewUpdate(void);
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
extern "C" part_type_s part_types[128];
extern "C" i32 part_types_used;
extern "C" f32 partglobaltime;
void edpartScaleType(i32 index, f32 scale);
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

void DebrisTimeSlip(i32 group);


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
extern rtlset * PartRTL;
extern f32 PARTSCALEUPTIME;
void NewPartOrderedRotation(PART_s *);
void PartTimeSlip();
void PartCleanupTypes();
void UpdatePartEmits(f32);

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.
extern i32 debris_suspended;

    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    extern f32 globaltime;
    extern f32 partglobaltime;
    extern f32 panelglobaltime;
    extern f32 timeincrement;
    f32 CameraEmitterDistance(NUVEC *);
    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);
    void DebrisEmitterMomentum(i32, f32, f32, f32);
    void DebrisParticleMomentum(i32, f32, f32, f32);
    void AddFiniteShotDebrisEffect2(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32);
    void AddVariableShotDebrisEffectMtx3(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *);
    void DebrisSetUserData(i32, void *);
    i32 CreateScaledPARTEffect(i32, f32);
    extern i32 debris_suspended;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern u32 partseed;
    extern NUGSCN *part_scene[32];

void NuPartEnableRayCasts(i32 enabled);

u32 NuPartGetSeed(void);

void NuPartSetSeed(i32 seed);

    extern part_type_s part_types[128];
    extern NUGSCN *part_scene[32];
    extern part_emit_s part_emits[40];
    extern i32 part_emits_used;
    void CheckPartCount();
    i32 AddPARTEffect(i32, NUVEC *);
    void PARTStartOffset(i32, f32);

    void ResetParts(void);

    i32 DebAlloc(void);
    void DebrisStartOffset(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisOrientation(i32, i16, i16);
    void DebrisOrientationMtx(i32, NUMTX *);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    i32 CreateScaledEffect(i32, f32);
    i32 NuCameraClipTestExtentsAxisAligned(NUVEC *, NUVEC *, f32);
    void NuVecAddScale(NUVEC *, NUVEC *, NUVEC *, f32);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

void AddDebrisEffectToStack(debkeydatatype_s *);
void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
void AddChunkControlToStack(debris_chunk_control_s *, debris_chunk_control_s **);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
void DebrisGetControlStackLock();
void DebrisReleaseControlStackLock();
void FindAnglesZX(NUVEC *, u16 *, u16 *);
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
extern f32 ForceThrowSpeed, ForceThrowGravity;
extern "C" void KillPart(PART_s *, i32);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);
void SetCoinType(i32 model, GIZMOPICKUP_s *pickup);
u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void ReleasePush(GameObject_s *);
void ObjHitShield(GameObject_s *, GameObject_s *, i32, BOLT_s *);
i32 CannotKill(GameObject_s *);



extern "C" f32 coinimpactwait;
extern i32 GAMEDEMO;
extern "C" void PlaySfxAndSetVolume(char *, NUVEC *, f32);



extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);


void PowerUp_Particles(WORLDINFO_s *, NUVEC *);






void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern WORLDINFO_s *WORLD;
extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;




extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position, i32 count);

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position, NUVEC *emitter_momentum,
                                    NUVEC *particle_momentum, i32 count);

void AddFiniteShotDebrisEffectUserData(i32 *handle, i32 effect, NUVEC *position, i32 count, void *user_data);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 AddPARTEffect(i32 effect, NUVEC *position);

    void KillPart(PART_s *, i32);
    void NewPartRotation(PART_s *);
    void DebrisPopulateInstance(i32, f32);
    i32 rtlDynamicAllocTemplate(rtlset *, i32);

PART_s *AddPart(ADDPART_s *params);

void AddRotatedDebrisEffect(i32 *handle, i32 effect, f32 x, f32 y, f32 z, i16 rotation_x, i16 rotation_y);

void AddScaledFiniteShotDebrisEffect(i32 *key, i32 effect, NUVEC *position, NUVEC *orientation, NUVEC *momentum,
                                         i32 count, f32 scale);

i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale);

i32 AddScaledVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                          i16 y_rotation, f32 scale);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

i32 AddScaledVariableShotDebrisEffect1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect2(i32 effect_index, NUVEC *position, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, i32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                           i8 flags, f32 scale);

i32 AddScaledVariableShotDebrisEffect5(i32 effect, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 time, NUMTX *emitter_orientation, NUMTX *particle_orientation,
                                           u16 priority, i8 flags, f32 scale);

void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale);

    void AddVariableShotDebrisEffectMtx(i32, NUVEC *, i32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectMtx(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation,
                                        NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                         i8 flags);

    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed3(i32 effect_index, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed5(i32 effect_index, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 duration, NUMTX *emitter_orientation,
                                           NUMTX *particle_orientation, u16 render_priority, i8 timed_flags);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void CheckPartCount(void);

void DrawParts(i32 keep_offscreen);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

i32 GetMaxPartTypes(void);

i32 GetPartCount(void);

char *GetPartName(i32 index);

extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
extern i32 part_types_used;
extern i32 part_emits_used;
extern NUGSCN * part_scene[32];
    // The editor loader clears these page IDs with aligned vector stores.
extern i32 part_scene_pageid[32];
extern i32 part_platimpactcnt;
extern i16 part_platimpactlist[4];
    void ResetParts(void);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

void KillAllParts(void);

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

void KillPartsByScene(NUGSCN *scene);

void NewPartRotation(PART_s *part);

void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation);

void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z);

f32 PARTGetTotalOffTime(i32 index);

f32 PARTGetTotalOnTime(i32 index);

i32 PARTLookupType(char *name);

extern i32 edpart_load_particle_page;

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

void PARTStartOffset(i32 index, f32 offset);

void ParticleReset(void);

void RemovePARTEffect(i32 index);

void ResetParts(void);

    void DebrisStatusAlwaysOff(i32 *);
    i32 rtlDynamicSetPos(i32, NUVEC *);

void UpdateParts(f32 time);

} // extern "C"

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
PART_s *TargetPart(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius, f32 range_squared,
                   i32 directional, i32 bolt_id);

extern "C" i32 NuGScnNumSpecials(nugscn_s *);
i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);
i32 edpartLookupDebrisEffect(char *name);
extern "C" i32 GetSfxIdN(char *, i32);

void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page);

extern "C" {
extern i32 edpart_nearest;
extern NUVEC edpart_cam_pos;
extern part_emit_s * edpart_nearest_emit;
extern part_typedesc_s * edpart_nearest_type;
}

void edpartDetermineNearest(f32 distance);
extern "C" {
extern i32 edbits_part_level_page;

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}

void PartTimeSlip();

extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);


void InitPartTable(char **names);

void LoadPartFile(WORLDINFO *world);

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position);

i32 SetPartTarget(GameObject_s *object, PART_s *target);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartCollide_3D(PART_s *);
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

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
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

void UpdatePartEmits(f32 time);

i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);

PART_s *FindIncomingPart(void *owner, NUVEC *position, f32 radius, u32 flags, f32 range);

void edpartDestroy(i32 index);

void PartCleanupTypes();

extern "C" f32 brickimpactwait;
extern "C" i32 GetSfxId(const char *);
extern "C" void PlaySfxByIdAndSetVolume(i32, NUVEC *, f32);

void PartImpact_Brick(PART_s *part);

void PartUpdate_Heart(PART_s *part);

void Asteroid_PartKill(PART_s *part, i32 reason);

void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32);

void PartImpact_Basketball(PART_s *part);

void PartUpdate_Basketball(PART_s *part);

PART_s *Part_FindFromHSpecial(nuhspecial_s *special);

void NewPartOrderedRotation(PART_s *part);

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
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");
static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue);

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

extern "C" i32 edSfxAllCount;

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

extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" i32 PS2_REZ_H;
extern "C" void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")





static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32);
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad);

extern "C" void CreateColourPicker(void);

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

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

float edanimPlayerAnimDistance(i32 parameter_index);
void edpartDestroy(i32 index);
void edgraDoInput(nupad_s *pad);
void edgraDetermineNearestInstance(f32 distance);
void edgraDrawCursor();
extern "C" void edgraClumpsReset(void);
extern "C" void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                       i32 rotation_y);
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
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}

void edppStartSingleEffect(i32 index);
void edppPtlDestroy(i32 index);
void edppDestroyAllPages();
i32 edppSaveEffects(char *filename, char page);
void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void UpdateTotalPtls(debinftype *effect);

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
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *namemenu;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32);


extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern i32 EDPP_MAX_TYPES;
    extern i32 edbits_particle_level_page;
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    void edppDeleteEffect(i32 index);
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
    void DebrisOrientation(i32 handle, i16 z, i16 y);
    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);
    void DebrisStartOffset(i32 handle, f32 offset);
    void DebrisSetTrigger(i32 handle, i32 type, i32 id, f32 value);
    void DebrisReflectionOrientation(i32 handle, i16 z, i16 y, f32 offset, f32 bounce);
    void DebrisSetFacing(i32 handle, u8 enabled, i16 x, i16 y);
    void DebrisSetGroupID(i32 handle, i16 group);
    void DebrisSetRoomID(i32 handle, nugscn_s *scene);
}

void edppPtlShelve(i32 index);


extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 edpp_readout;
    extern i32 edpp_showAllPlaced;
    extern i32 edpp_snap_enabled;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlstartvelmenu;
}
static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *item, u32);

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeX(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeY(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeZ(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlColMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlJibMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlRotMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCollMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDataMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelGCode(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSelGSort(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSizeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlGSortMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *item, u32);

static void cbSelEffectList(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbEffectListMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);

static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlInstanceSettingsMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCancel(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCancelRepeatBoxMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void UpdateTotalPtls(debinftype *effect);

// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbPageMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbGroupMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDetail(eduimenu_s *, eduiitem_s *item, u32);
void edppStartPage(i32 page);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbBounceMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbSoundXMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbSoundsMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetDrawflag(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbResetParticles(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edptlall.cpp ----


void edppDoInput(nupad_s *pad);
void edppDetermineNearest(f32 distance);
void edppDrawCursor();
void edppHighlightNearest();
extern "C" void DebrisStatusNormal(i32 *handle);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
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
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...);

extern "C" i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
extern "C" i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
extern "C" f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);

#include "gameapi/edtools/edtools_types.h"

static void edgraInit();
static void edgraClose();
static void edgraEnter();
static i32 edgraProc(f32 delta_time, nupad_s *pad);
static void edgraRender();
static void edppInit();
static void edppClose();
static void edppEnter();
static void edppApply();
static i32 edppProc(f32 delta_time, nupad_s *pad);
static void edppRender();
static void edbriInit();
static void edbriClose();
static void edbriEnter();
static i32 edbriProc(f32 delta_time, nupad_s *pad);
static void edbriRender();
static void edanimInit();
static void edanimClose();
static void edanimEnter();
static i32 edanimProc(f32 delta_time, nupad_s *pad);
static void edanimRender();

extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 bCameraEnabled;
extern i32 edui_filepick_listsize;
extern u32 eduiitem_context_outline_colour;
extern u32 eduiitem_selected_outline_colour;
extern u32 eduimenu_selected_outline_colour;
extern u32 eduimenu_outline_colour;
extern f32 edmain_default_camera_speed;
extern i32 ed_main_menu_h;
extern i32 ed_main_menu_w;
extern i32 ed_main_menu_y;
extern i32 ed_main_menu_x;
extern eduiiattr_s ed_attr;
extern i32 eduiPropTextPos;
extern i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
extern NUMTL * uimtls[5];
extern i32 ui_bgmtl;
extern i32 ui_outmtl;

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
extern i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern f32 edpp_cam_dist;
extern i32 edpp_cam_ax;
extern i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
extern i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
extern eduimenu_s * memcardmenu;
extern eduimenu_s * messagemenu;
extern eduimenu_s * loadeffectsmenu;
extern eduimenu_s * mergeeffectsmenu;
extern eduimenu_s * deleteeffectsmenu;
extern eduimenu_s * filenamemenu;
extern eduimenu_s * namemenu;
extern eduimenu_s * confirmmenu;
extern eduimenu_s * changegenratemenu;
extern eduimenu_s * totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern f32 edpp_offset;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}

static void edgraInit();

static void edgraClose();

static void edgraEnter();

static i32 edgraProc(f32 delta_time, nupad_s *pad);

static void edgraRender();

static void edppInit();

static void edppClose();

static void edppEnter();

static void edppApply();

static i32 edppProc(f32 delta_time, nupad_s *pad);

static void edppRender();

void edbriDoInput(nupad_s *);
void edbriDetermineNearest(f32);
void edbriDrawCursor();
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}

static void edbriInit();

static void edbriClose();

static i32 edbriProc(f32 delta_time, nupad_s *pad);

static void edbriRender();

static void edanimInit();

static void edanimClose();

static i32 edanimProc(f32 delta_time, nupad_s *pad);

static void edanimRender();

void edgraClumpReseed(i32 index);

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

void edgraClumpDestroy(i32 index);
void edbitsDoSingleDump(i32 face);
void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);


extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
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
extern f32 edanim_cam_dist;
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
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
extern i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern NUVEC edbri_cam_pos;
extern f32 edbri_cam_dist;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}

static void edbriEnter();

static void edanimEnter();

void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);
extern "C" void NuBridgeRemove(i32 bridge);
extern "C" i32 NuBridgeCreate(NUGSCN *, nuhspecial_s *, nuhspecial_s *, NUVEC *, NUVEC *, f32, i16, f32, f32, f32, f32,
                              i32, f32, f32, i32, u32);

void edbriBridgeUpdate(i32 index, NUGSCN *scene);

void edbriBridgePlace(i32 index, NUVEC *position);

i32 edbriBridgeCreate(NUVEC *position);

void edbriBridgeDestroy(i32 index);

void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlDestroy(i32);
void edppPtlShelve(i32);
extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}

extern "C" void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
extern "C" void do_maya_mouse_camera(edcam_s *camera);
extern "C" void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
extern "C" i32 NuKeyboard(i32 key);


void edcamSetContoller(i32 invert_pitch);



extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;
extern i32 edmain_cursor_enabled;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
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
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
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
void edmainSetReturn(i32 result);
void edpartClearPage(i8 page);
void edpartDestroyAllParticles(void);
void edpartParticleReset(void);
void edpartRegisterPointerToGameCharLocation(NUVEC *position);
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
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
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
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
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
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
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
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
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

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
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
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
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
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

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

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
extern "C" VuVec SpherePos1;
extern "C" VuVec SpherePos2;
extern VuVec SpherePos1;
extern VuVec SpherePos2;
extern EdInputContext * Input;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
extern PropertyTool thePropertyTool;
extern eduiiattr_s EdLevelAttr;
extern i32 EdLevelFnt;
extern "C" void eduiSetCameraEnabled(i32);
extern "C" void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
extern "C" i32 edbri_nearest;
extern "C" NUVEC edbri_cam_pos;
extern "C" NUMTL *edbri_mtl, *edbri_mtl_zoff;
extern "C" f32 edbri_length, edbri_width;
extern "C" i32 edbri_rotz, edbri_roty, edbri_planks, edbri_post_interval;
extern "C" i32 edbri_plank_instance_type, edbri_post_instance_type, edbri_bridges_used;
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void NuRndrLine2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void edbitsDrawCircleTilted(NUVEC *, f32, i32, NUMTL *, i32, i32);
NUVEC NuFadeObjGetAngleTerrainValues(NUVEC *);
extern "C" void eduiAddPropTextPickEnt(eduimenu_s *, eduiitem_s *);
extern "C" char *GetSfxName(i32);
extern "C" void PlaySfxById(i32, nuvec_s *);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
static i32 get_manipulator_attribute(ClassObjectListEntry *, i32, i32, void *);
static void set_manipulator_attribute(ClassObjectListEntry *, i32, i32, void *);
void cbEdLevelDestroy(eduimenu_s *, eduimenu_s *);
void cbEdLevelDestroyOnSelect(eduimenu_s *, eduiitem_s *, u32);
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

void SerialiseChar(EdStream &, void *, i32);
void SerialiseShort(EdStream &, void *, i32);
void SerialiseInt(EdStream &, void *, i32);
void SerialiseFloat(EdStream &, void *, i32);
void SerialiseVuVec(EdStream &, void *, i32);
void SerialiseVuMtx(EdStream &, void *, i32);
void SerialiseString(EdStream &, void *, i32);
void SerialiseColour3(EdStream &, void *, i32);
void SerialiseNuHSpecial(EdStream &, void *, i32);
void SerialiseNuVec(EdStream &, void *, i32);
void SerialiseNuMtx(EdStream &, void *, i32);

extern f32 Scale;
extern EdManipulator theDefaultManipulator;
extern EdManMove theMoveManipulator;
extern EdManRotate theRotateManipulator;
extern EdManScale theScaleManipulator;
extern SplineTool theSplineTool;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
extern SplineHelper theSplineHelper;
extern KnotHelper theKnotHelper;
extern ClassEditor theClassEditor;
extern i32 pad_disabled;
extern EdSystem theEdSystem;
extern eduimenu_s * edLevelPinnedMenu;

void eduiSetPinnedMenu(eduimenu_s *menu);

void edSetPadDisabled(i32 disabled);

i32 edGetPadDisabled();

extern NUMTL * EdDrawMtl[2];
void EdDrawPolyArrow(VuVec const &, VuVec const &, i32, i32, float, float, float, float);
void EdDrawPolyCylinder(VuMtx const &, float, float, float, i32, i32, i32, i32);
void EdDrawMtx(VuMtx const *);
extern char * EDSPLINE_FILECHECK;

extern "C" {
extern char edgra_filter_string[16];
extern i32 edgra_mode;
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 edpart_emitrotx, edpart_emitroty, edpart_emitrotz;
    extern NUVEC edanim_cam_pos;
extern i32 edgra_last_clump_in_buffer;
extern i32 edgra_copy_source;
extern f32 edgra_global_fadein;
extern f32 edgra_global_fadeout;
    void edgraInitAllClumps(void);
    void DebFreeInstantly(i32 *);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetPriority(i32, u16, u8);
    void DebrisSetRoomID(i32, NUGSCN *);
    void DebrisSetDetailLevels(i32, i32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    i32 AddPARTEffect(i32, NUVEC *);
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_usememcard;
    extern i32 edpp_copy_mode, edpp_copyrotz, edpp_copyroty, edpp_copy_source_count, edpp_copy_enclosed;
    extern i32 edpp_readout, edpp_num_orphans;
    extern i32 globalframes;
    extern f32 edpp_copy_size;
    extern NUMTL *edpp_mtl, *edpp_boxmtl;
    void edppDrawSpheres(debinftype *, i32);
    void edppDrawTorus(debinftype *, i32);
    extern i32 part_page_used[8];
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
    extern f32 edpp_offset;
extern i32 edbits_particle_level_page;
}

void edpartDestroy(i32 index);

i32 edpartLookupObjectInScene(char *, NUGSCN *);

void EdTerrInit(void *, void *);

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char);

void EdDrawBegin(i32 material);

void edpartPlace(i32 index, nuvec_s *position);

void edppDetermineNearest(f32);
void edppMultipleCopyPaste();
void edppMultipleCopyCopy();
void edppMultipleCopyClear();
i32 edppPtlCreate(NUVEC *, i32);
i32 edppPtlPlace(i32, NUVEC *);
void edppPtlDestroy(i32);

void edppDoInput(nupad_s *pad);

void EdTerrShadow(nuvec_s *position, float height_above, float height_below, i32 terrain_mask);

void edpartCreate(nuvec_s *position, i32 type);

i32 edppPtlPlace(i32 index, NUVEC *position);

void EdDrawPolyTri(VuVec const &a, VuVec const &b, VuVec const &c, i32 colour);

void edanimDoInput(nupad_s *pad);

i32 edbriFileSave(char *path);

i32 edppPtlCreate(NUVEC *position, i32 effect_index);

i32 edgraFileSave(char *path);

void edppPtlShelve(i32 index);

void EdDrawLineCube(VuMtx const &transform, float size, i32 colour);

void EdDrawPolyAxis(VuMtx const &transform, float size, i32 opacity);

i32 edanimFileSave(char *path);

void edpartInitType(i32 index);

void edppDrawCursor();

extern "C" {
    void DebFreeInstantly(i32 *);
}

void edppPtlDestroy(i32 index);

void EdDrawLineArrow(VuMtx const &transform, float size, i32 colour);

void EdDrawLineCross(VuVec const &position, float size, i32 colour);

void EdDrawPolyArrow(VuVec const &start, VuVec const &end, i32 sides, i32 colour, float radius, float limit,
                     float radius_factor, float radius_offset);

void edbriDrawCursor();

void edgraClumpPlace(i32 index, NUVEC *position);

void edgraDrawCursor();

void edpartPtlShelve(i32 index);

void edpartScaleType(i32 index, float scale);

i32 edppSaveEffects(char *filename, char page);

void EdDrawLineSphere(VuVec const &center, float radius, float scale, i32 colour);

void EdDrawPolySector(VuVec const &center, float radius, i32 axis, i32 first_angle, i32 last_angle, i32 colour,
                      i32 segments);

void edanimDrawCursor();

extern "C" i32 part_types_used;
extern "C" i32 part_emits_used;

void edpartDrawCursor();

i32 edgraClumpCreate(NUVEC *position);


void EdDrawLineCircleX(VuVec const &centre, float radius, i32 colour, i32 segments);

void EdDrawLineCircleZ(VuVec const &centre, float radius, i32 colour, i32 segments);

void EdDrawLineCircleY(VuVec const &centre, float radius, i32 colour, i32 segments);

void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour);

i32 edanimParamCreate(i32 instance_id);

i32 edpartSaveEffects(char *filename, char page);

void edppPtlChangeType(i32 index, i32 effect_index);

i32 edppPtlCreateCopy(NUVEC *position, i32 source_index);

void EdDrawPolyCylinder(VuMtx const &transform, float half_length, float radius, float end_radius, i32 sides,
                        i32 colour, i32 cap_start, i32 cap_end);

__attribute__((force_align_arg_pointer)) void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides,
                                                                 i32 colour, i32 cap_colour, float radius, float limit,
                                                                 float offset);

void edanimParamDestroy(i32 index);

void edbitsDoSingleDump(i32 face);

void edgraCalculatePage(char page, i32 calculate_vectors);

void edgraInstancePlace(i32 index, NUVEC *position);

i32 edpartLookupObject(char *name);

i32 edSpline_FindAllBeg(NUGSCN *scene, char *prefix, NUGSPLINE **results, i32 capacity);

i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end);

i32 edSpline_FindAllSub(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity);

NUGSPLINE *edSpline_SplineFind(NUGSCN *scene, char *name);

void edSpline_SplineList(nugscn_s *);

void edanimParticlePlace(i32 index, NUVEC *position);

void edanimStartAllPages();

void edgraInstanceCreate(NUVEC *position);

i32 edpartPtlChangeType(i32 index, i32 type);

void edppDestroyAllPages();

void edanimParticleCreate(NUVEC *position);

void edgraInstanceDestroy(i32 index);

void edppDetermineNearest(float max_distance_squared);

void edppHighlightNearest();

void edppMultipleCopyCopy();

void edbriDetermineNearest(float distance);

void edgraSortVectorBuffer(i32 index);

void edpartDestroyAllPages();

void edppMultipleCopyClear();

void edppMultipleCopyPaste();

void edppStartSingleEffect(i32 index);

void edpartHighlightNearest();

void edpartMultipleCopyCopy();

void edpartMultipleCopyClear();

float edanimPlayerAnimDistance(i32 parameter_index);

void edanimRenderSoundEmitters(i32 parameter_index);

void edanimDetermineNearestAnim(float distance);

void edgraDetermineNearestClump(f32 distance);

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text);

void edanimRenderParticleEmitters(i32 parameter_index);

void edanimDetermineNearestSound(float distance);

void edgraDetermineNearestInstance(f32 distance);

void edanimDetermineNearestParticle(float distance);

void EdDrawEnd();

void EdDrawMtx(VuMtx const *matrix);

i32 EdTerrRay(VuVec &, VuVec &);
















































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































extern EdSfxNameControl * sfxNameControl;



























extern eduimenu_s *edLevelDestroyThisMenu;
extern eduimenu_s *edLevelDestroyThisMenu2;



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
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");
static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue);

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

extern "C" i32 edSfxAllCount;

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

extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" i32 PS2_REZ_H;
extern "C" void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")





static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32);
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad);

extern "C" void CreateColourPicker(void);

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

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

float edanimPlayerAnimDistance(i32 parameter_index);
void edpartDestroy(i32 index);
void edgraDoInput(nupad_s *pad);
void edgraDetermineNearestInstance(f32 distance);
void edgraDrawCursor();
extern "C" void edgraClumpsReset(void);
extern "C" void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                       i32 rotation_y);
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
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}

void edppStartSingleEffect(i32 index);
void edppPtlDestroy(i32 index);
void edppDestroyAllPages();
i32 edppSaveEffects(char *filename, char page);
void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void UpdateTotalPtls(debinftype *effect);

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
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *namemenu;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32);


extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern i32 EDPP_MAX_TYPES;
    extern i32 edbits_particle_level_page;
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    void edppDeleteEffect(i32 index);
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
    void DebrisOrientation(i32 handle, i16 z, i16 y);
    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);
    void DebrisStartOffset(i32 handle, f32 offset);
    void DebrisSetTrigger(i32 handle, i32 type, i32 id, f32 value);
    void DebrisReflectionOrientation(i32 handle, i16 z, i16 y, f32 offset, f32 bounce);
    void DebrisSetFacing(i32 handle, u8 enabled, i16 x, i16 y);
    void DebrisSetGroupID(i32 handle, i16 group);
    void DebrisSetRoomID(i32 handle, nugscn_s *scene);
}

void edppPtlShelve(i32 index);


extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 edpp_readout;
    extern i32 edpp_showAllPlaced;
    extern i32 edpp_snap_enabled;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlstartvelmenu;
}
static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *item, u32);

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeX(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeY(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeZ(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlColMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlJibMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlRotMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCollMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDataMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelGCode(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSelGSort(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSizeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlGSortMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *item, u32);

static void cbSelEffectList(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbEffectListMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);

static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlInstanceSettingsMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCancel(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCancelRepeatBoxMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void UpdateTotalPtls(debinftype *effect);

// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbPageMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbGroupMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDetail(eduimenu_s *, eduiitem_s *item, u32);
void edppStartPage(i32 page);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbBounceMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbSoundXMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbSoundsMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetDrawflag(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbResetParticles(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edptlall.cpp ----


void edppDoInput(nupad_s *pad);
void edppDetermineNearest(f32 distance);
void edppDrawCursor();
void edppHighlightNearest();
extern "C" void DebrisStatusNormal(i32 *handle);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
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



// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...);

extern "C" i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
extern "C" i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
extern "C" f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);

#include "gameapi/edtools/edtools_types.h"

static void edgraInit();
static void edgraClose();
static void edgraEnter();
static i32 edgraProc(f32 delta_time, nupad_s *pad);
static void edgraRender();
static void edppInit();
static void edppClose();
static void edppEnter();
static void edppApply();
static i32 edppProc(f32 delta_time, nupad_s *pad);
static void edppRender();
static void edbriInit();
static void edbriClose();
static void edbriEnter();
static i32 edbriProc(f32 delta_time, nupad_s *pad);
static void edbriRender();
static void edanimInit();
static void edanimClose();
static void edanimEnter();
static i32 edanimProc(f32 delta_time, nupad_s *pad);
static void edanimRender();

extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 bCameraEnabled;
extern i32 edui_filepick_listsize;
extern u32 eduiitem_context_outline_colour;
extern u32 eduiitem_selected_outline_colour;
extern u32 eduimenu_selected_outline_colour;
extern u32 eduimenu_outline_colour;
extern f32 edmain_default_camera_speed;
extern i32 ed_main_menu_h;
extern i32 ed_main_menu_w;
extern i32 ed_main_menu_y;
extern i32 ed_main_menu_x;
extern eduiiattr_s ed_attr;
extern i32 eduiPropTextPos;
extern i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
extern NUMTL * uimtls[5];
extern i32 ui_bgmtl;
extern i32 ui_outmtl;

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
extern i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern f32 edpp_cam_dist;
extern i32 edpp_cam_ax;
extern i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
extern i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
extern eduimenu_s * memcardmenu;
extern eduimenu_s * messagemenu;
extern eduimenu_s * loadeffectsmenu;
extern eduimenu_s * mergeeffectsmenu;
extern eduimenu_s * deleteeffectsmenu;
extern eduimenu_s * filenamemenu;
extern eduimenu_s * namemenu;
extern eduimenu_s * confirmmenu;
extern eduimenu_s * changegenratemenu;
extern eduimenu_s * totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern f32 edpp_offset;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}

static void edgraInit();

static void edgraClose();

static void edgraEnter();

static i32 edgraProc(f32 delta_time, nupad_s *pad);

static void edgraRender();

static void edppInit();

static void edppClose();

static void edppEnter();

static void edppApply();

static i32 edppProc(f32 delta_time, nupad_s *pad);

static void edppRender();

void edbriDoInput(nupad_s *);
void edbriDetermineNearest(f32);
void edbriDrawCursor();
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}

static void edbriInit();

static void edbriClose();

static i32 edbriProc(f32 delta_time, nupad_s *pad);

static void edbriRender();

static void edanimInit();

static void edanimClose();

static i32 edanimProc(f32 delta_time, nupad_s *pad);

static void edanimRender();

void edgraClumpReseed(i32 index);

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

void edgraClumpDestroy(i32 index);
void edbitsDoSingleDump(i32 face);
void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);


extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
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
extern f32 edanim_cam_dist;
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
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
extern i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern NUVEC edbri_cam_pos;
extern f32 edbri_cam_dist;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}

static void edbriEnter();

static void edanimEnter();

void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);
extern "C" void NuBridgeRemove(i32 bridge);
extern "C" i32 NuBridgeCreate(NUGSCN *, nuhspecial_s *, nuhspecial_s *, NUVEC *, NUVEC *, f32, i16, f32, f32, f32, f32,
                              i32, f32, f32, i32, u32);

void edbriBridgeUpdate(i32 index, NUGSCN *scene);

void edbriBridgePlace(i32 index, NUVEC *position);

i32 edbriBridgeCreate(NUVEC *position);

void edbriBridgeDestroy(i32 index);

void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlDestroy(i32);
void edppPtlShelve(i32);
extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}

extern "C" void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
extern "C" void do_maya_mouse_camera(edcam_s *camera);
extern "C" void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
extern "C" i32 NuKeyboard(i32 key);


void edcamSetContoller(i32 invert_pitch);



extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;
extern i32 edmain_cursor_enabled;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
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
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
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
void edmainSetReturn(i32 result);
void edpartClearPage(i8 page);
void edpartDestroyAllParticles(void);
void edpartParticleReset(void);
void edpartRegisterPointerToGameCharLocation(NUVEC *position);
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
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
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
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
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
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
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
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
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

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
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
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
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
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

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern MENUPACKET_s MenuPacket;
extern i32 NextArea_FreePlay;
void InitMission(MISSIONSYS *, i32);
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void BackupMenu(void);
extern "C" void BackupMenuNoFn(void);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                                      u8 alpha);
extern "C" void DrawMenuButtonPromptsEx(i32 confirm_prompt, i32 cancel_prompt, i32 flags, i32 enabled, u8 red, u8 green,
                                        u8 blue, u8 alpha);
extern i32 SAVESLOTS;
extern i32 MenuSFX;
extern i32 MENUSFX_MENUSELECT;
extern i32 MENUSFX_MENUBACK;
extern i32 MENUSFX_MENUNOENTRY;
extern i32 MENUSFX_MENUMOVE;
extern char *apitxt_LOADGAME;
extern char *apitxt_SAVEGAME;
extern char *apitxt_CONFIRMSAVE;
extern char *apitxt_CONFIRMOVERWRITE;
extern char *apitxt_CONFIRMLOAD;
extern char *apitxt_DOYOUWANTTOABORT;
extern char *apitxt_DOYOUWANTTOABORTLOAD;
extern char *apitxt_DOYOUWANTTOABORTFORMAT;
extern char *apitxt_CONFIRMDELETE;
extern char *apitxt_RETRY;
extern char *apitxt_SLOT;
extern char *apitxt_NEWSAVE;
extern char *apitxt_CANCEL;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_NOTENOUGHSPACE;
extern char *apitxt_DELETEGAME;
extern char *apitxt_DELETING;
extern char *apitxt_DELETECOMPLETE;
extern char *apitxt_FORMATTING;
extern char *apitxt_FORMATTINGCOMPLETE;
extern char *apiGameName;
extern char *apitxt_YES;
extern char *apitxt_NO;
extern char **TTab;
extern i16 tSELECTCONTROLS;
extern i16 tCONTROLSCANBECHANGED;
extern i16 tCLASSIC;
extern i16 tTOUCHSCREEN;
extern i16 tCONTROLLERCONNECTED;
extern i16 tCONTROLS;
extern i16 tCONSOLE;
extern i16 tTOUCH;
extern i16 tSURROUNDSOUND;
extern i16 tAUDIOVOLUME;
extern i16 tMUSIC;
extern i16 tWIDESCREEN;
extern i16 tON;
extern i16 tOFF;
extern i16 tACCEPT;
extern i16 tBACK;
extern i16 tCONTINUE;
extern i16 tCANCEL;
extern i16 tSTORY;
extern i16 tFREEPLAY;
extern i16 tREPLAYSTORY;
extern i16 tLOCKED;
extern i16 tAVAILABLETOBUY;
extern i16 tPOWERBRICK;
extern i16 tHOWTOPLAY;
extern "C" OPTIONSSAVE TempOptions;
extern i32 GAMEDEMO;
extern i32 menu_flash;
extern i16 tOUTOFTIME;
extern f32 minikittime;
extern "C" TIMER BonusTimer;
extern f32 text3d_height;
extern f32 text3d_width;
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
void GameDrawMenuEntry(MENU *menu, char *text);
i32 GetParentMenuID(void);
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
void SfxCheckMusicOnOff(OPTIONSSAVE *options);
void NewGame(void);
void NuIOS_RestoreInAppPurchases(void);
extern i32 startnewgame;
extern i32 startnewgame_initiated;
extern f32 newgamecamtime;
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;
extern "C" f32 sfx_wait;
extern "C" f32 MENUBOTY;
void Draw_AUTOSAVEWARNING();
void Draw_NODATAAVAILABLE();
void Draw_NOMEMORYCARD();
extern "C" void Draw_NOTENOUGHSPACE(void);
extern "C" void Draw_SPACENEEDED(void);
extern "C" void Draw_CHECKINGMEMORYCARD(void);
extern "C" void Draw_DONOTREMOVEMEMORYCARD(void);
void Draw_OK(MENU_s *menu);

extern i32 memcard_cardchanged;
extern i32 MenuASCancelFinished;
extern i32 MenuCardWarningState;
extern i32 ButtonScaleMode;
extern i32 Menu_InLoadFlow;
extern i32 Menu_InWarningFlow;
extern i32 Menu_LastFlow;
extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 Menu_InSaveFlow;
extern i32 Menu_DisableCancel;
extern i32 Menu_InHeaderSave;
extern i32 Menu_Overwriteflag;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern i32 memcard_deleteneeded;
extern i32 memcard_deletestarted;
extern i32 memcard_deletefailed;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
extern i32 memcard_formatting;
extern i32 memcard_formatme;
extern i32 memcard_formatfailed;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 SAVESIZE;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

extern char * extrasavetext;
extern f32 extrasavealpha;
extern u8 RAP_WARNING_B;
extern u8 RAP_WARNING_G;
extern u8 RAP_WARNING_R;


extern i32 hub_new_level;
extern i32 hub_selectmode;
extern f32 selectmodetime;
extern i32 selectmodemode;
extern "C" i32 FinishLoop_On;
extern FadeSystem FadeSys;
void Hub_DrawFreePlaySelect();
void Hub_InitFreePlaySelect(i32 area, i32 first_model, i32 second_model);
void Hub_UpdateFreePlaySelect();
void WipeBackToHub();
void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);




void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

void MenuDrawLoad(MENU_s *menu);

void MenuDrawSave(MENU_s *menu);


void MenuExitLoad(MENU_s *);

void MenuExitSave(MENU_s *);

void MenuDrawClips(MENU_s *);

void MenuEnterLoad(MENU_s *menu);

void MenuEnterSave(MENU_s *menu);

void MenuInitClips(MENU_s *);

void MenuStartLoad();

void MenuStartSave();




void MenuDrawNoData(MENU_s *menu);

void MenuDrawSaving(MENU_s *);

void MenuUpdateLoad(MENU_s *menu);

void MenuUpdateSave(MENU_s *menu);




void IconScenes_Dump();

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *);

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end);

void MenuDrawLoading(MENU_s *);




i32 MenuIsAvailable();

void MenuUpdateClips(MENU_s *);

void MenuDrawDeleting(MENU_s *);

void MenuDrawFreePlay(MENU_s *);



void MenuInitFreePlay(MENU_s *menu);



void MenuUpdateNoData(MENU_s *menu);

void MenuUpdateLoading(MENU_s *);

void MenuUpdateSaving(MENU_s *);



NUGSCN *IconScene_FindById(i32 character_id);


void MenuDrawFormatting(MENU_s *);

void MenuDrawInsertCard(MENU_s *);

void MenuDrawLoadCancel(MENU_s *menu);

void MenuDrawSaveCancel(MENU_s *menu);

void MenuUpdateDeleting(MENU_s *);

void MenuUpdateFreePlay(MENU_s *);


void MenuDrawCardWarning(MENU_s *);

void MenuDrawFileCorrupt(MENU_s *);

void MenuDrawLoadConfirm(MENU_s *menu);

void MenuDrawSaveConfirm(MENU_s *menu);

void MenuEnterHeaderSave(MENU_s *);

void MenuEnterInsertCard(MENU_s *);

void MenuExitCardWarning(MENU_s *);

void MenuDrawEndChallenge(MENU_s *);

void MenuDrawFormatCancel(MENU_s *menu);

void MenuDrawNoMemoryCard(MENU_s *menu);

void MenuEnterCardWarning(MENU_s *);

void MenuEnterSaveConfirm(MENU_s *);


void MenuUpdateFormatting(MENU_s *);

void MenuUpdateLoadCancel(MENU_s *menu);

void MenuUpdateSaveCancel(MENU_s *menu);

void MenuUpdateInsertCard(MENU_s *menu);

void MenuDrawDeleteConfirm(MENU_s *menu);

void MenuDrawFormatConfirm(MENU_s *);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuEnterNoMemoryCard(MENU_s *);

void MenuEnterStartNewGame(MENU_s *);

void MenuUpdateCardWarning(MENU_s *menu);

void MenuUpdateFileCorrupt(MENU_s *menu);

void MenuUpdateLoadConfirm(MENU_s *menu);

void MenuUpdateSaveConfirm(MENU_s *menu);

void MenuDrawAutoSaveCancel(MENU_s *menu);

void MenuDrawNotEnoughSpace(MENU_s *);



void MenuUpdateEndChallenge(MENU_s *menu);

void MenuUpdateFormatCancel(MENU_s *menu);

void MenuUpdateNoMemoryCard(MENU_s *menu);

void MenuDrawAutoSaveWarning(MENU_s *menu);

void MenuDrawDoNotRemoveCard(MENU_s *);

void MenuEnterAutoSaveCancel(MENU_s *menu);

void MenuUpdateDeleteConfirm(MENU_s *menu);

void MenuUpdateFormatConfirm(MENU_s *menu);

void MenuEnterAutoSaveWarning(MENU_s *);

extern f32 memcard_autosavecanceldelay;
void MenuUpdateAutoSaveCancel(MENU_s *menu);

void MenuUpdateNotEnoughSpace(MENU_s *menu);



void MenuUpdateAutoSaveWarning(MENU_s *menu);

void MenuUpdateDoNotRemoveCard(MENU_s *menu);

extern "C" {

extern f32 MENUDY;
extern f32 MENUBOTY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern f32 MENUENTRYEXWIDTH;
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
extern void (*headerdrawfn)();
extern void (*predrawfn)(MENU *);


void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void BackupMenuNoFn(void);

void DrawMenu(i32 paused);

void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha);

void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue);

void Draw_CANCEL(MENU *menu);

void Draw_CHECKINGMEMORYCARD(void);

void Draw_DONOTREMOVEMEMORYCARD(void);

void Draw_NOTENOUGHSPACE(void);

void Draw_SPACENEEDED(void);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

void MenuDrawBackground(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b);

void MenuSetPulsateSpeed(f32 speed);

void MenuSetTopBottom(f32 top, f32 bottom);

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));

void MenuReset(void);

void RemapAddr(void *new_base, void *old_base, void **address);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

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

void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
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

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;
nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count);

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
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
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
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

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
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
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
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


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
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
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
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
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
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
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
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
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
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

void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

extern SPEEDERCHASEANETPACKET_s * speederchasea_netpacket;

extern u8 troopercannons_beenReset;
extern i32 players_going_forward;
extern i32 players_cannot_exit_speeder;

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
extern i32 test_ai_combo;
extern i32 CanPunchGirls;
extern i16 id_GAMORREANGUARD;
extern i16 id_STORMTROOPER;
i32 SpecialMove_Check(GameObject_s *, GameObject_s *);
u32 SpecialMove_GetFlags(i32, u32);

void PodKeyReset();

void TakeHitRumble(GameObject_s *, f32);

void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble);

i32 SpeederBlowupHack(GIZMOBLOWUP_s *blowup, i32);

f32 FindPodHoverHeight(GameObject_s *object);

extern i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
extern i32 IDLESPEEDINNARROWSOCKSONLY;

i32 ObjIsTargetSpeeder(GameObject_s *object);
f32 GetVehicleSpeedMul(GameObject_s *object, f32 speed);

void SetLevelExBlowupFunc(i32 (*callback)(GIZMOBLOWUP_s *, i32));
void InitTrooperCannons(WORLDINFO_s *world);
void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id);
void UpdateTrooperCannons(WORLDINFO_s *world);
i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void SpeederChaseA_Init(WORLDINFO_s *world);

void ProcessCurrentSpeed(WORLDINFO_s *world, speedup_s *speedup);

void SpeederChaseA_Panel(WORLDINFO_s *);

void SpeederChaseA_Reset(WORLDINFO_s *);

i32 SpeedersDroppedBack();

i32 Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32 first_time,
                              f32 elapsed);

void SpeederChaseA_Update(WORLDINFO_s *world);

i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);

f32 SpeederChaseATATInOutMul(nuvec_s *start, nuvec_s *end);

f32 GetVehicleAreaRememberSpeed();

i32 SpeederChase_ObjIsAGroundTroop(GameObject_s *object);

extern "C" {

extern i32 edmain_auto_speed;

void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32);

} // extern "C"

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

void PodDust(WORLDINFO_s *world, GameObject_s *object);

extern "C" f32 pod_roll[2];
f32 getPodRoll(i32 index);

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
extern VIEWCAM_s ViewCam;
// Original data symbol testmovef is float0.005 at 0x617cb0.
extern f32 testmovef;

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);

void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

void GameCam_HitRoll();

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();

void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount);

void GameCam_ResetLookRot(GAMECAMERA_s *camera);

void GameCam_UpdateLookRot(GAMECAMERA_s *camera);

void GameCameraMakeMiniCut(nugspline_s *spline, f32 start, f32 end, f32 blend_in, f32 blend_out, i32 borders,
                           i32 hold_until_players_move);

extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern NUVEC ObstacleCamCutPts[2];

void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);

void GameCameraMakeMiniCut3(u32 flags, float distance, i32 pitch, i32 yaw, i32 roll, void *focus, i32 target_guid,
                            nuvec_s *position, float start_time, float blend_in_time, float end_time,
                            float blend_out_time, float blend_time, float hold_time, float max_time, i32 borders,
                            nugspline_s *spline, char mode, char easing);

u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);

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

extern NUVEC CustomisePos[2];
extern f32 HUB_MINIKITVIEWER_CAMDY;
extern f32 PodCamDist;
extern NUVEC ObstacleCamCutPts[2];
extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern f32 EMPERORFIGHTA_CAMDYHACK;
extern i32 movegamecamera_forcesock;
extern NUMTX CutCamMtx;
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);
void CentreTwoPlayerCamera(NUVEC *, NUVEC *, NUVEC *, NUVEC *);


void MoveGameCamera(GAMECAMERA_s *camera);

void ViewCamDraw();

void KeepOnScreen(GameObject_s *object);

NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();

void SetDepthOfField();

void SpeedBlur_Apply(WORLDINFO_s *world);

void SpeedBlur_Update();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);

void SetCameraMatrices();

GAMEPAD_s *ViewCamGetGamePad();

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference);

void KeepVehicleOnScreen(GameObject_s *object, i32 sides, i32 top, i32 bottom);

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

void InitCameraTargetMaterial();

i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void GetTopBot(GameObject_s *object);

extern "C" {

extern i32 near_clip_at_cursor;

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

} // extern "C"

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edfile.h"

#include <string.h>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"

extern i32 edfile_handle;
extern i32 edfile_write_flag;


extern char edfile_buffer[4096];
extern i32 edfile_buffer_pointer;
extern i32 edfile_lock[2];

extern i32 edfile_readwrongendianess;
extern i32 edfile_mcresult;

void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);

void EdFileResetBuffers();

void EdFileFillBuffer();

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

void EdFileFlushBuffer();

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

extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 memcard_cardchanged;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);
extern i32 numfilteroutblocks;
extern char filteroutblocks[8][12];
extern i32 numfilterblocks;
extern char filterblocks[8][96];

extern char FS_FileList[65536];
extern char FS_LastFileName[64];
extern char FS_Path[256];
extern i32 FS_NumFiles;
extern i32 FS_CurrentPosFileNum;
extern i32 FS_CursorLine;
extern char FS_ParentEntry[8];
extern i32 FS_SortMode;
extern char * FS_CurrentCursorPos;
extern char * FS_CurrentPos;
extern char * FS_FileListEnd;
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

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);

extern f32 memcard_autosavecanceldelay;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
extern f32 memcard_createmessage_delay;
extern f32 memcard_createresult_delay;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
extern f32 memcard_message_delay;
extern f32 memcard_result_delay;
extern i32 memcard_justformatted;
extern i32 saveload_autosavedisabled;

void InitMemCard();

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *);

void SerialiseInt(EdStream &stream, void *data, i32);

void FS_GetDirList(char *path, char *filter, char *filter_out);

void SerialiseChar(EdStream &stream, void *data, i32);

i32 FS_PrevNameLen(char *name);


void FS_SortStrings(char *start, char *end, i32 mode);

void SerialiseFloat(EdStream &stream, void *data, i32);

void SerialiseNuMtx(EdStream &stream, void *data, i32);

void SerialiseNuVec(EdStream &stream, void *data, i32);

void SerialiseShort(EdStream &stream, void *data, i32);

void SerialiseVuMtx(EdStream &stream, void *data, i32);

void SerialiseVuVec(EdStream &stream, void *data, i32);

void FS_MoveCursorUp(i32 steps);

void SerialiseString(EdStream &stream, void *data, i32 count);

void SerialiseColour3(EdStream &stream, void *data, i32);

i32 FS_FileNameFilter(char *name);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material);

extern char * SortStrings[4];

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

void FS_MoveCursorDown(i32 steps);

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 getsaveload_status();

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

void SerialiseNuHSpecial(EdStream &stream, void *data, i32);

void SerialiseStringAddr(EdStream &stream, void *data, i32 count);

void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);

void FS_SetCursorToLastFileName();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {
extern void (*savesuccessfn)();
extern void * memcard_headerdata;
extern i32 memcard_headerdatasize;
extern void * memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

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
extern "C" f32 MainRenderTime;
extern "C" i32 editor_active;
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

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);

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

extern "C" {








i32 cbInteractMenuTitle(edui_interact_s *interact);

i32 cbInteractMenuScrollUp(edui_interact_s *interact);

i32 cbInteractMenuScrollDown(edui_interact_s *interact);


}


extern "C" {
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);


void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);




}



extern "C" {


}






extern "C" {

























void edmainSetReturn(i32 result);



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

void eduiSetRenderPlane(i8 plane);

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

eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);

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

void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);

void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);

}


extern "C" {
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);

void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);

void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);

void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);

void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);

void eduiItemSliderSetVal(edui_slider_s *item, f32 value);

i32 eduicbInteractSlider(edui_interact_s *interact);

void eduiItemSliderSetFmt(edui_slider_s *item, char *format);

void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);

eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);

eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);

i32 eduiItemPropSetText(edui_prop_s *item, char *text);

i32 eduiGetCameraEnabled(void);

void eduiSetCameraEnabled(i32 enabled);

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

void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);


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


void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);

void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);




edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);

void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);

void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);



eduimenu_s *eduiGetActiveMenu(void);

void eduiMenuRender(eduimenu_s *menu);

void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);



i32 eduiMenuDetach(eduimenu_s *menu);

}


extern "C" {
void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);









__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);



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

void eduiShowCursor(i32 show);

void eduiGetCursorCoords(f32 *x, f32 *y);

void eduiSetCursorCoords(f32 x, f32 y);

void eduiGetCursorDelta(f32 *x, f32 *y);

i32 eduiUsedAlgPad(nupad_s *pad);

void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);

void eduiProcessCursor(f32 delta_time, nupad_s *pad);

i32 edmainProcess(f32 delta_time, nupad_s *pad);

void eduiSetCursorColour(u32 colour);

void eduiRenderCursor(void);

void edmainRender(void);

void eduiInit(void);

void edmainInit(void *font, char *configuration_file);

void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);

void eduiSetFont(NUQFNT *font);

void eduiSetFontScale(f32 x, f32 y);

void CreateTestMenu(void *font);

void TestMenu(nupad_s *pad);

eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);

void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);

void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);


void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);

void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32);

void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);

void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);

}

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

extern "C" {
void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

void edcamMoveEx(nupad_s *pad, f32 delta_time);

void edcamMove(nupad_s *pad);

void edcamSetPos(NUVEC *position);

void edcamSetAng(i32 pitch, i32 yaw);

void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);

void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);

void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);

}

void edcamSetContoller(i32 invert_pitch);

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

void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);

void EdFileResetBuffers();

void EdFileFillBuffer();

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

void EdFileFlushBuffer();

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

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);


void FS_MoveCursorDown(i32 steps);

i32 FS_PrevNameLen(char *name);

void FS_MoveCursorUp(i32 steps);

void RenderFileSel3(i32 draw_title);

extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

}

void RenderFileSel();

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 FS_FileNameFilter(char *name);

void FS_BuildFilterBlocks(char *input);

void FS_BuildFilterOutBlocks(char *input);

void FS_SortStrings(char *start, char *end, i32 mode);

void FS_SetCursorToLastFileName();

void FS_GetDirList(char *path, char *filter, char *filter_out);

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

}

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" {
void FS_SetFileSelPathFromName(char *name);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text);

extern "C" {
void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

}

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char);

extern "C" {
extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" void CreateColourPicker(void);

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

void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
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

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;
nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count);

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
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
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
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

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
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
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
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


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
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
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
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
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
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
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
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
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
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

void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== functions in original address order ====






































































































































extern "C" {
void PartTerrInit(void);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void edpartRegisterPointerToGameCharLocation(NUVEC *position);

}

void edpartPtlShelve(i32 index);

void edpartPlace(i32 index, nuvec_s *position);

void edpartMultipleCopyCopy();

void edpartMultipleCopyClear();

void edpartDrawCursor();

void edpartDetermineNearest(f32 distance);

void edpartHighlightNearest();


void edpartInitType(i32 index);



void edpartScaleType(i32 index, float scale);


i32 edpartSaveEffects(char *filename, char page);




i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);

i32 edpartLookupDebrisEffect(char *name);

extern "C" {
void edpartSetParticlePage(i32 page);

}

i32 edpartLookupObject(char *name);

extern "C" {
void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

void edpartParticleReset(void);

void NuPartEnableRayCasts(i32 enabled);

void NuPartSetSeed(i32 seed);

u32 NuPartGetSeed(void);

void NuPartResetGlobalTime(void);

void ResetParts(void);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

void NewPartRotation(PART_s *part);

}

void NewPartOrderedRotation(PART_s *part);

void PartTimeSlip();

extern "C" {
extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

void KillPart(PART_s *part, i32 reason);

void DrawParts(i32 keep_offscreen);

PART_s *AddPart(ADDPART_s *params);

void KillAllParts(void);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

void PARTStartOffset(i32 index, f32 offset);

i32 PARTLookupType(char *name);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

}

void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page);

extern "C" {
i32 PARTLookupTypePageOnly(char *name, i32 page);

i32 GetPartCount(void);

i32 GetMaxPartTypes(void);

char *GetPartName(i32 index);

}



extern "C" {
void CheckPartCount(void);

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);

}


void edpartDestroy(i32 index);

void PartCleanupTypes();


extern "C" {
void edpartDestroyAllParticles(void);

}

i32 edpartPtlChangeType(i32 index, i32 type);

extern "C" {
f32 PARTGetTotalOnTime(i32 index);

f32 PARTGetTotalOffTime(i32 index);

i32 AddPARTEffect(i32 effect, NUVEC *position);

}

void edpartCreate(nuvec_s *position, i32 type);

void edpartDoInput(nupad_s *pad);


extern "C" {
void RemovePARTEffect(i32 index);

}

void UpdatePartEmits(f32 time);

extern "C" {
void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z);

void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

void UpdateParts(f32 time);

i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);

i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale);

void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale);

void KillPartsByScene(NUGSCN *scene);

void edpartClearPage(i8 page);

}

void edpartDestroyAllPages();


// ==== src/legoapi/misc/legoapi_status.cpp: declarations ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {

i32 DEVCDDVDROM_Interrogate(NUFILE_DEVICE *device);

i32 DEVMEMORYCARD_Interrogate(NUFILE_DEVICE *device);

void DebugLog(void);

void Debug_Print(void);

} // extern "C"

// ==== functions in original address order ====

void MenuEnterInsertCard(MENU_s *);

void MenuEnterNoMemoryCard(MENU_s *);

void MenuEnterAutoSaveWarning(MENU_s *);

void MenuEnterAutoSaveCancel(MENU_s *menu);

void MenuEnterCardWarning(MENU_s *);

void MenuExitSave(MENU_s *);

void MenuExitLoad(MENU_s *);

void MenuExitCardWarning(MENU_s *);

void MenuDrawFormatConfirm(MENU_s *);

void MenuDrawCardWarning(MENU_s *);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

extern "C" {
void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b);

void MenuSetPulsateSpeed(f32 speed);

void MenuSetTopBottom(f32 top, f32 bottom);

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));

void MenuReset(void);

void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);

void MenuRememberCursor(MENU *menu);

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);

}

void MenuEnterLoad(MENU_s *menu);

void MenuEnterSave(MENU_s *menu);

extern "C" {
void BackupMenu(void);

}

void MenuUpdateCardWarning(MENU_s *menu);

void MenuUpdateAutoSaveCancel(MENU_s *menu);

void MenuUpdateDeleting(MENU_s *);

void MenuUpdateFormatting(MENU_s *);

void MenuUpdateDoNotRemoveCard(MENU_s *menu);

void MenuEnterStartNewGame(MENU_s *);

extern "C" {
void BackupMenuNoFn(void);

}

void MenuUpdateFormatCancel(MENU_s *menu);

void MenuUpdateLoadCancel(MENU_s *menu);

void MenuUpdateSaveCancel(MENU_s *menu);

void MenuUpdateLoading(MENU_s *);

void MenuUpdateAutoSaveWarning(MENU_s *menu);

void MenuUpdateSaving(MENU_s *);

void MenuUpdateDeleteConfirm(MENU_s *menu);

void MenuUpdateFileCorrupt(MENU_s *menu);

void MenuUpdateFormatConfirm(MENU_s *menu);

void MenuUpdateNoData(MENU_s *menu);

void MenuUpdateNotEnoughSpace(MENU_s *menu);

void MenuUpdateNoMemoryCard(MENU_s *menu);

void MenuUpdateInsertCard(MENU_s *menu);

void MenuEnterHeaderSave(MENU_s *);

extern "C" {
void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

}

i32 MenuIsAvailable();

extern "C" {
void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8);

void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha);

void DrawMenu(i32 paused);

}

void MenuStartLoad();

void MenuUpdateLoadConfirm(MENU_s *menu);

void MenuUpdateLoad(MENU_s *menu);

void MenuStartSave();

void MenuUpdateSaveConfirm(MENU_s *menu);

void MenuUpdateSave(MENU_s *menu);

void MenuEnterSaveConfirm(MENU_s *);

extern "C" {
void Draw_SPACENEEDED(void);

}

void Draw_NOMEMORYCARD();

void MenuDrawInsertCard(MENU_s *);

void Draw_AUTOSAVECANCEL();

extern "C" {
void Draw_CHECKINGMEMORYCARD(void);

void Draw_DONOTREMOVEMEMORYCARD(void);

}

void MenuDrawDoNotRemoveCard(MENU_s *);

extern "C" {
i32 MenuGetSlotNum(void);

i32 MenuInMemoryCard(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

i32 MenuCurrentID(void);

void MenuDrawBackground(void);

}

void MenuLoadTechnicalStrings(char *filepath, char *language, VARIPTR *buf, VARIPTR buf_end);

extern "C" {
bool TestForController(void);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

}

void SaveSystemInitialise(i32 slots, void *makeSaveHash, void *save, i32 saveSize, i32 saveCount,
                          void (*drawSaveIcon)(void), void *extradata, i32 extradataSize);

extern "C" {
void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

}

i32 ChecksumSaveData(void *buffer, i32 size);

extern "C" {
void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

i32 TriggerAutoSave(void);

}

bool TriggerExtraDataSave(void);

i32 TriggerExtraDataLoad(void);

i32 getsaveload_status();

void Text3DStringEncodeFont(unsigned char *src, u16 *dst, void *font);

extern "C" {
extern "C" void Text3DStringEncode(char *src, u16 *dst);

void TextRegisterButtonMapFn(void (*fn)(char *, char *));

void TextRegisterPulseTimerFn(f32 (*fn)(f32));

}

void TextDecodeCodeword(char *source, char *destination);

extern "C" {
void TextDecode(char *source, unsigned char *dest);

}

f32 TextPrintSubstring(unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, i32 colour,
                       i32 button_font);

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

void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

i32 FindNextBreak(unsigned char *text, i32 index);

i32 FindNearestBreak(unsigned char *text, i32 index);

extern "C" {
extern "C" void MessageBoxInitMtl(void);

}

void DrawMessageBoxRGBA(f32 x, f32 y, f32 width, f32 height, u32 blue, u32 green, u32 red, u32 alpha, numtl_s *material,
                        i32 alignment, f32 scale);

void DrawMessageBox(i32, f32 x, f32 y, f32 width, f32 height);

extern "C" {
void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);

void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                   u8 green, u8 blue, f32 max_width, i32 max_lines);

void SmartTextEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                      u32 alpha);

void MenuSmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         i32 alpha);

}

void MenuDrawFileCorrupt(MENU_s *);

void MenuDrawDeleting(MENU_s *);

void MenuDrawFormatting(MENU_s *);

void Draw_AUTOSAVEWARNING();

void MenuDrawLoading(MENU_s *);

void MenuDrawSaving(MENU_s *);

void Draw_LOADCORRUPT();

void Draw_LOADFAILED();

void Draw_LOADED();

void Draw_LOADING();

void Draw_NODATAAVAILABLE();

extern "C" {
void Draw_NOTENOUGHSPACE(void);

}

void MenuDrawNotEnoughSpace(MENU_s *);

void APIMenuDrawGameState(f32 x, f32 y, i32 highlight, i32 slot);

extern "C" {
void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuEntry(MENU *menu, char *text);

}

void MenuDrawLoadConfirm(MENU_s *menu);

void MenuDrawNoMemoryCard(MENU_s *menu);

extern "C" {
void Draw_CANCEL(MENU *menu);

}

void MenuDrawLoad(MENU_s *menu);

void MenuDrawNoData(MENU_s *menu);

void MenuDrawSave(MENU_s *menu);

void Draw_OK(MENU_s *menu);

void MenuDrawAutoSaveCancel(MENU_s *menu);

void MenuDrawAutoSaveWarning(MENU_s *menu);

void MenuDrawFormatCancel(MENU_s *menu);

void MenuDrawLoadCancel(MENU_s *menu);

void MenuDrawSaveCancel(MENU_s *menu);

void MenuDrawSaveConfirm(MENU_s *menu);

void MenuDrawDeleteConfirm(MENU_s *menu);

extern "C" {
void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuHeader(void);

void SmartTextExDrop(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u32 red,
                         u32 green, u32 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         u32 alpha);

void SmartTextSetFWNMode(i32 mode);

void SmartTextSetFont(VUFNT *font);

void SetGameFont(VUFNT *font);

void Set3DGameFont(VUFNT *font);

VUFNT *LoadGameFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane);

void UnloadGameFont(void);

extern "C" void SetQFont2D(void);

void SmartTextSetWidescreen(f32 font_scale_x, f32 coordinate_scale);

void SmartTextGetWidescreen(f32 *font_scale_x, f32 *coordinate_scale);

void Debug_Print(void);

}

i32 SplitTextFindNextWS(unsigned char *text, i32 position);

extern "C" {
i32 SplitText(unsigned char *text, f32 max_width);

}

void MatrixTextStringEncode(void *font, unsigned char *source, u16 *destination);

extern "C" {
void MatrixText(VUFNT *font, unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, u32 alignment,
                    u32 colour, NUMTX *matrix, i32 camera_relative);

void FixUpButtonsInFont(VUFNT *game_font, VUFNT *button_font);

VUFNT *LoadButtonFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane);

}

// ==== functions in original address order ====

static void Hub_DrawBonusModeMenu(int selected, float alpha) {
    char text[3][128];
    if (__builtin_expect(bonusmodearcade, 0)) {
        const i32 full_opacity = static_cast<i32>(alpha * 128.0f);
        f32 y = 0.3062499761581421f;
        MENU *menu = &GameMenu[GameMenuLevel];
        AREADATA *area;
        for (i32 i = 0; i < 3; ++i) {
            i16 *label;
            memcpy(&label, reinterpret_cast<const u8 *>(&ArcadeItem) + i * 8, sizeof(label));
            if (__builtin_expect(!!(label), 0))
                NuStrCpy(text[0], TTab[*label]);
            else
                text[0][0] = 0;
            i32 opacity = full_opacity;
            if (i != 1 && i != 2) {
                if (text[0][0])
                    NuStrCat(text[0], ": ");
                area = *ArcadeLevel[ArcadeItem.level].area;
                if (area && area->name_id != 0 && TTab[area->name_id])
                    NuStrCat(text[0], TTab[area->name_id]);
                else
                    NuStrCat(text[0], "?");
            } else if (i == 1) {
                if (text[0][0])
                    NuStrCat(text[0], ": ");
                char *mode_text = TTab[*Arcade_Mode[static_cast<i8>(ArcadeItem.field_c_0xc)].text];
                NuStrCat(text[0], mode_text ? mode_text : "?");
                if (__builtin_expect(area && Game_LevelSave && area->levels[0] != -1, 0)) {
                    const i32 level = area->levels[0];
                    if ((reinterpret_cast<LEVELSAVE_s *>(Game_LevelSave)[level].arcade_flags &
                         (1u << (static_cast<i8>(ArcadeItem.field_c_0xc) & 31))) != 0)
                        NuStrCat(text[0], " ~2$");
                    if ((reinterpret_cast<LEVELSAVE_s *>(Game_LevelSave)[level].arcade_flags & 7) == 7) {
                        const f32 scale = NU_SIN_LUT(static_cast<i32>(alpha * 16384.0f)) * PANEL_REDBRICKSCALE;
                        const i32 rotation =
                            static_cast<i32>((NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f) * 65536.0f) & 0xffff;
                        const u16 tilt = static_cast<i32>(1820.0f * NuTrigTable[rotation & 0x7fff]) - 0x1555;
                        DrawPanel3DObjectNoAlpha(0.0f, y + 0.35f, 1.0f, scale, scale, scale, tilt, rotation, 0,
                                                 &WORLD->lev_objs[211].special, 2);
                    }
                }
            } else if (*ArcadeLevel[ArcadeItem.level].area == NULL)
                opacity = full_opacity / 3;
            i32 red, green, blue;
            if (selected && i == hub_bonusmode && TestForController()) {
                if (menu_pulsate > 0.0f) {
                    red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                           static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                    green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                             static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                    blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                            static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
                } else {
                    red = menu_flash ? MENUFLASH0R : MENUFLASH1R;
                    green = menu_flash ? MENUFLASH0G : MENUFLASH1G;
                    blue = menu_flash ? MENUFLASH0B : MENUFLASH1B;
                }
            } else if (menu_pulse > 0.0f) {
                red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                       static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
                green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                         static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
                blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                        static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
            } else {
                red = MENUENTRYR;
                blue = MENUENTRYB;
                green = MENUENTRYG;
            }

            smarttextex_drawmessagebox = 1;
            SmartTextEx(text[0], 0.0f, y, 1.0f, 0.7f, 0.7f, 0.7f, 0, static_cast<u8>(red), static_cast<u8>(green),
                        static_cast<u8>(blue), 1.7f, 1, 0, 0, opacity);
            menu->item_x[i] = 0.0f;
            menu->item_y[i] = y;
            menu->item_width[i] = text3d_width;
            menu->item_height[i] = text3d_height;
            menu->item_column[i] = 0;
            menu->item_row[i] = i;

            y -= 0.175f;
        }
        return;
    }
    const i32 menu_level = GameMenuLevel;
    if (hub_new_level == -1 || LDataList[hub_new_level].area_index == -1 ||
        (ADataList[LDataList[hub_new_level].area_index].flags & 0x100) == 0)
        return;
    i32 count;
    if (hub_bonusepisode != -1) {
        NuStrCpy(text[0], TTab[tSUPERSTORY]);
        NuStrCpy(text[1], TTab[tCHARACTERBONUS]);
        NuStrCpy(text[2], TTab[tVEHICLEBONUS]);
        const i32 character_area = Episode_FindAreaFromFlags(&EDataList[hub_bonusepisode], 5, 4);
        const i32 vehicle_area = Episode_FindAreaFromFlags(&EDataList[hub_bonusepisode], 5, 5);
        if (character_area != -1 && ADataList[character_area].name_id != -1 &&
            TTab[ADataList[character_area].name_id]) {
            const i32 name = ADataList[character_area].name_id;
            NuStrCat(text[1], " (");
            NuStrCat(text[1], TTab[name]);
            NuStrCat(text[1], ")");
        }
        if (vehicle_area != -1 && ADataList[vehicle_area].name_id != -1 && TTab[ADataList[vehicle_area].name_id]) {
            const i32 name = ADataList[vehicle_area].name_id;
            NuStrCat(text[2], " (");
            NuStrCat(text[2], TTab[name]);
            NuStrCat(text[2], ")");
        }
        count = 3;
    } else {
        NuStrCpy(text[0], TTab[hub_bonusarea != -1 ? ADataList[hub_bonusarea].name_id : -1]);
        count = 1;
    }
    f32 y = (count - 1) * 0.1f + 0.125f;
    const i32 full_opacity = static_cast<i32>(alpha * 128.0f);
    MENU *menu = &GameMenu[menu_level];
    for (i32 i = 0; i < count; ++i) {
        i32 red, green, blue;
        if (selected && i == hub_bonusmode && TestForController()) {
            if (menu_pulsate > 0.0f) {
                red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                       static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                         static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                        static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
            } else {
                red = menu_flash ? MENUFLASH0R : MENUFLASH1R;
                green = menu_flash ? MENUFLASH0G : MENUFLASH1G;
                blue = menu_flash ? MENUFLASH0B : MENUFLASH1B;
            }
        } else if (menu_pulse > 0.0f) {
            green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                     static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
            red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                   static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
            blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                    static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
        } else {
            red = MENUENTRYR;
            green = MENUENTRYG;
            blue = MENUENTRYB;
        }

        i32 opacity = full_opacity;
        if (i == 2 && Collection_GetIDList(&MiniKitCollection, 0x4000000, 0x4000000, NULL, NULL, NULL, 0) <= 0)
            opacity = full_opacity / 4;
        smarttextex_drawmessagebox = 1;
        SmartTextEx(text[i], 0.0f, y, 1.0f, 0.7f, 0.7f, 0.7f, 0, static_cast<u8>(red), static_cast<u8>(green),
                    static_cast<u8>(blue), 1.7f, 1, 0, 0, opacity);
        menu->item_x[i] = 0.0f;
        menu->item_y[i] = y;
        menu->item_width[i] = text3d_width;
        menu->item_height[i] = text3d_height;
        menu->item_column[i] = 0;
        menu->item_row[i] = i;

        y -= 0.2f;
    }
}

static void Hub_DrawMiniKitCount(f32 x, f32 y, i32 count, i32 total, f32 alpha) {
    const i32 object = hub_drawminikitcount_charkit ? 207 : 206;
    hub_drawminikitcount_charkit = 0;
    char text[128];
    if (total == 1)
        NuStrCpy(text, count == 1 ? "$" : "X");
    else
        sprintf(text, "%i/%i", count, total);

    Text3DEx(text, x, y, 1.0f, PANEL_MINIKITCOUNTSCALE * stats_xscale, PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE,
             0, 255, 0, 127, static_cast<u8>(static_cast<i32>(128.0f * alpha)));
    const u16 rotation = (NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f) * 65536.0f;
    const f32 scale = NU_SIN_LUT(static_cast<i32>(alpha * 16384.0f)) * PANEL_MINIKITSCALE;
    const i16 tilt = 1820.0f * NuTrigTable[rotation & 0x7fff];
    DrawPanel3DObjectNoAlpha(x, y + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 1.0f, scale, scale, scale, tilt, rotation, 0,
                             &WORLD->lev_objs[object].special, 2);
}

static void Hub_DrawSelectModeMenu(int selected, float alpha) {
    const i32 area = LDataList[hub_new_level].area_index;
    const i32 complete = Game.area_save[area].area_complete;
    const i32 lost_temple = LOSTTEMPLE_ADATA != NULL && area == LOSTTEMPLE_ADATA->index;
    i32 unlocked = FreePlayUnlocked();
    if (unlocked != 1 && area != -1) {
        unlocked = ADataList[area].episode_index == 0xff && (ADataList[area].flags & 0x4000) != 0;
    }
    i32 opacity[10];
    i32 text_ids[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    i32 count = 0;
    if (!lost_temple) {
        text_ids[count] = complete ? tREPLAYSTORY : tSTORY;
        opacity[count++] = 128;
        if ((ADataList[area].flags & 0x1000) == 0) {
            text_ids[count] = tFREEPLAY;
            opacity[count++] = complete && unlocked ? 128 : 48;
            if ((ADataList[area].flags & 0x10) != 0 && Store_IsPackUnlocked(8)) {
                text_ids[count] = tCHALLENGE;
                opacity[count++] = complete && Store_IsPackUnlocked(8) ? 128 : 48;
            }
        }
    }
    f32 y = (count - 1) * 0.1f + 0.125f;
    if (text_ids[1] == tFREEPLAY && !unlocked)
        y += 0.2f;
    MENU *menu = &GameMenu[GameMenuLevel];
    for (i32 i = 0; i < count; ++i) {
        i32 red, green, blue;
        if (selected && i == hub_selectmode && TestForController()) {
            if (menu_pulsate > 0.0f) {
                red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                       static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                         static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                        static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
            } else {
                red = menu_flash ? MENUFLASH0R : MENUFLASH1R;
                green = menu_flash ? MENUFLASH0G : MENUFLASH1G;
                blue = menu_flash ? MENUFLASH0B : MENUFLASH1B;
            }
        } else if (menu_pulse > 0.0f) {
            red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                   static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
            green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                     static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
            blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                    static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
        } else {
            red = MENUENTRYR;
            green = MENUENTRYG;
            blue = MENUENTRYB;
        }
        const i32 text_id = text_ids[i];
        char *text = TTab[text_id];
        char buffer[128];
        if (!unlocked) {
            if (alpha == 1.0f) {
                Hint_SetHintFromId(0x164, 0, 1);
                Hint_Draw(-1);
            } else {
                Hint_CancelCurrent();
            }
            if (text_id == tFREEPLAY) {
                sprintf(buffer, "%s (%s)", text, TTab[tLOCKED]);
                text = buffer;
            }
        }
        smarttextex_drawmessagebox = 1;
        SmartTextEx(text, 0.0f, y, 1.0f, 0.7f, 0.7f, 0.7f, 0, static_cast<u8>(red), static_cast<u8>(green),
                    static_cast<u8>(blue), 1.7f, 1, 0, 0, static_cast<i32>(opacity[i] * alpha));
        menu->item_x[i] = 0.0f;
        menu->item_y[i] = y;
        menu->item_width[i] = text3d_width;
        menu->item_height[i] = text3d_height;
        menu->item_column[i] = 0;
        menu->item_row[i] = i;
        y -= 0.2f;
    }
}

static void Hub_MakeFreePlayList(i32 first_model, i32 second_model) {
    fpcount = 0;
    const i32 area = LDataList[hub_new_level].area_index;

    if (hub_makefreeplaylist_addotherid != 0) {
        if (second_model == -1) {
            const i32 other_player = PlayerID[1];
            if (other_player != -1 && other_player != PlayerID[0] && other_player != first_model) {
                second_model = other_player;
            }
        }
        hub_makefreeplaylist_addotherid = 0;
    }

    MakeFreePlayModelList(first_model, second_model, area, -1, 1);
    if (area != -1 && (ADataList[area].flags & (AREAFLAG_VEHICLE_AREA | AREAFLAG_BONUS_AREA)) ==
                          (AREAFLAG_VEHICLE_AREA | AREAFLAG_BONUS_AREA)) {
        for (i32 index = 0; index < FreePlayModelCount; ++index) {
            const i32 model = FreePlayModelList[index].model_id;
            if ((CDataList[model].model_flags & HUB_FREEPLAY_MODEL_MINIKIT) == 0 ||
                InCollectList_Index(model, MiniKitCollection.list, MiniKitCollection.count_y) == -1 ||
                PlayerList[0] == model || PlayerList[1] == model || Collection_Got(model) == 0) {
                continue;
            }
            fplist[fpcount++] = FreePlayModelList[index];
        }
    } else {
        const i32 selectable_count = FreePlayResidentCount + FreePlayBonusCount;
        for (i32 offset = 0; offset < selectable_count; ++offset) {
            const i32 index = offset + 2;
            const i32 model = FreePlayModelList[index].model_id;
            if (model == -1) {
                break;
            }
            if (area != -1) {
                if ((ADataList[area].flags & AREAFLAG_VEHICLE_AREA) != 0) {
                    if ((CDataList[model].model_flags & HUB_FREEPLAY_MODEL_VEHICLE) == 0) {
                        continue;
                    }
                } else if ((CDataList[model].model_flags & HUB_FREEPLAY_MODEL_VEHICLE) != 0) {
                    continue;
                }
            }
            if (Collection_Got(model) != 0) {
                fplist[fpcount++] = FreePlayModelList[index];
            }
        }
    }

    fplist[fpcount].model_id = -1;
    if (fpcount > 3) {
        for (i32 shuffle = 0; shuffle < 64; ++shuffle) {
            const i32 first_offset = qrand() / (0xffff / (fpcount - 2) + 1);
            const i32 second_offset = qrand() / (0xffff / (fpcount - 3) + 1);
            const i32 second_index = (second_offset + first_offset) % (fpcount - 2) + 2;
            const i32 first_index = first_offset + 2;
            const APICHARACTERMODELLIST_s saved = fplist[first_index];
            fplist[first_index] = fplist[second_index];
            fplist[second_index] = saved;
        }
    }
}

static void Hub_DrawBonusStats(f32 alpha, i32 area, i32 episode) {
    if (episode == -1 && area != -1)
        episode = static_cast<i8>(ADataList[area].episode_index);
    if (episode != -1) {
        Episode_FindAreaFromFlags(&EDataList[episode], 5, 4);
        Episode_FindAreaFromFlags(&EDataList[episode], 5, 5);
        SmartTextEx(TTab[tSTORYCLIPS2], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, HUB_EPISODESUBTITLESIZE,
                    HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB, 1.7f,
                    1, NULL, 0, static_cast<i32>(alpha * 128.0f));
    }
}

static void Hub_DrawIconCursors(COLLECTION_s *collection, f32 scale) {
    const i32 player = 0;
    const f32 alpha = 1.0f;
    i32 model = MenuPacket.player_model[player];
    const i32 index = InCollectList_Index(model, collection->list, collection->count_y);
    if (index == -1)
        return;
    if (Collection_Got(model) == 0)
        model = -1;
    if (MenuPacket.active_player[player] == 0) {
        DrawCharIcon(model, collection->list[index].grid_x, collection->list[index].grid_y, 0.0f,
                     collection->field_10 * ICONSIZE * scale, 0xa6, alpha, alpha, 1, NULL);
        return;
    }
    f32 size = Game_OptionsSave != NULL && Game_OptionsSave->widescreen != 0 ? 0.14f : 0.16f;
    size *= collection->field_10 / 0.675f;
    if (freeplay_time[player] <= 0.0f) {
        if (freeplay_selected[player] != 0)
            size = 0.0f;
    } else {
        f32 progress = freeplay_time[player] / 0.75f;
        if (freeplay_selected[player] == 0)
            progress = 1.0f - progress;
        size *= 1.0f - (NU_SIN_LUT(static_cast<i32>(progress * 32768.0f + 16384.0f)) + 1.0f) * 0.5f;
    }
    if (size * scale > 0.0f) {
        const f32 offset = (collection->field_10 / 0.675f) * 0.01f;
        DrawCharIcon(model, collection->list[index].grid_x + offset * NU_SIN_LUT(hub_iconang[player * 2]),
                     collection->list[index].grid_y + NU_SIN_LUT(hub_iconang[player * 2 + 1]) * offset, -0.0025f,
                     size * scale, 0xa6, alpha, alpha, 1, NULL);
    }
}

i32 Hub_BonusBuildIt(GIZBUILDIT_s *buildit) {
    for (HUBAREAINFO_s *info = HubAreaInfo; info->area_name != NULL; ++info) {
        if (info->bonus_gizmo != NULL && info->bonus_gizmo->object == buildit) {
            return 1;
        }
    }
    return 0;
}

void TurnEpisodeDoorLightsOn(i32 episode) {
    for (i32 i = 0; i < 9; ++i) {
        if (HubEpisodeInfo[i].episode == episode) {
            if (NuSpecialExistsFn(&HubEpisodeInfo[i].lock_on))
                NuSpecialSetVisibility(&HubEpisodeInfo[i].lock_on, 1);
            if (NuSpecialExistsFn(&HubEpisodeInfo[i].lock_off))
                NuSpecialSetVisibility(&HubEpisodeInfo[i].lock_off, 0);
            break;
        }
    }
}

void WipeBackToHub() {
    NewMenu(-1, -1, -1);
    ResetTimer(&JoinInTimer, 0.0f);
    NuStrCpy(Door_ExitName, Door_Last->name);
    StartDoorPositions();
    NewGameMode();
    bonusmodearcade = 0;
    HubMainRenderTimeHack = 1;
}

i32 Arcade_BothPlayersActive() {
    // The shipped mobile implementation returns true unconditionally.
    return true;
}

i32 Arcade_GetMode(u32 *flags) {
    if (Arcade == 0) {
        if (flags != NULL) {
            *flags = 0;
        }
        return -1;
    }
    if (flags != NULL) {
        *flags = Arcade_Mode[ArcadeItem.field_c_0xc].field8_0x8;
    }
    return ArcadeItem.field_c_0xc;
}

void Arcade_ResetPanel() {
    Arcade_NeedTwoPlayers_Scale = 1.0f;
}

void Arcade_UpdatePanel(i32 paused) {
    if (Arcade == 0 || paused != 0 || Arcade_BothPlayersActive()) {
        Arcade_NeedTwoPlayers_Scale = 1.0f;
    } else {
        Arcade_NeedTwoPlayers_Scale = SeekLinearF(Arcade_NeedTwoPlayers_Scale, 1.0f, FRAMETIME);
        if (static_cast<i32>(GameTimer.time_elapsed * 2.0f) != static_cast<i32>(GameTimer.last_time_elapsed * 2.0f)) {
            GameAudio_PlaySfx(0x35, NULL, 0, 0);
        }
    }
}

void Arcade_DrawPanel(i32 paused) {
    if (Arcade == 0 || paused != 0 || Arcade_BothPlayersActive()) {
        return;
    }
    f32 phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    i32 angle = static_cast<i32>((phase + phase) * 65536.0f);
    i32 alpha = static_cast<i32>((NuTrigTable[(angle >> 1) & 0x7fff] * 0.2f + 0.8f) * 128.0f);
    f32 scale = Arcade_NeedTwoPlayers_Scale * 0.6f;
    SmartTextEx(TTab[tARCADE_NEEDTWOPLAYERS], 0.0f, -0.55f, 1.0f, scale, scale, scale, 0, 255, 0, 0,
                Arcade_NeedTwoPlayers_Scale * 1.7f, 1, NULL, 0, alpha);
}

void Arcade_AwardPoint(i32 player_index, i32 reset_score, i32) {
    if (reset_score != 0) {
        Arcade_Score[player_index] = 0;
    } else {
        if (BonusWinner != -1) {
            return;
        }
        if (Arcade_BothPlayersActive()) {
            ++Arcade_Points[player_index];
            if (Arcade_Points[player_index] > 0) {
                SetBonusWinner(player_index);
                BonusWinFlag = 0;
                NewMenu(0xb, -1, -1);
                ++Arcade_Score[player_index];
            }
        }
    }
    if (Game_LevelSave != NULL) {
        u8 mode_bit = 1u << Arcade_GetMode(NULL);
        LEVELSAVE_s *saves = reinterpret_cast<LEVELSAVE_s *>(Game_LevelSave);
        LEVELSAVE_s *save = &saves[WORLD->level_idx];
        if ((save->arcade_flags & mode_bit) == 0) {
            save->arcade_flags |= mode_bit;
            if (Game_AreaSave != NULL && Area != -1 && (saves[WORLD->level_idx].arcade_flags & 7) == 7) {
                Game_AreaSave[Area].area_complete = 1;
                ReCalculateCompletionPoints();
            }
            TriggerAutoSave();
        }
    }
}

void Arcade_PlayerKilled(i32 player_index, i32 extra) {
    if (static_cast<u32>(player_index) > 1 || (Arcade_Mode[ArcadeItem.field_c_0xc].field8_0x8 & 1) == 0) {
        return;
    }
    if (Arcade_BothPlayersActive()) {
        ++AreaGlobals.values.arcade_player_kills[player_index];
        Arcade_AwardPoint(player_index, 0, extra);
    } else {
        Arcade_AwardPoint(player_index, 0, 0);
    }
}

void Arcade_AIKilled(i32 player_index) {
    if (static_cast<u32>(player_index) > 1 || (Arcade_Mode[ArcadeItem.field_c_0xc].field8_0x8 & 2) == 0) {
        return;
    }
    if (Arcade_BothPlayersActive()) {
        ++AreaGlobals.values.arcade_ai_kills[player_index];
        PlaySfx(const_cast<char *>("env_padLight_on"), NULL);
        if (Player[player_index] != NULL && Player[player_index]->coinpacket != NULL) {
            Player[player_index]->coinpacket->scale = 1.5f;
        }
        if (AreaGlobals.values.arcade_ai_kills[player_index] >=
            static_cast<u32>(Arcade_Mode[ArcadeItem.field_c_0xc].target)) {
            Arcade_AwardPoint(player_index, 0, 0);
            AreaGlobals.values.arcade_ai_kills[0] = 0;
            AreaGlobals.values.arcade_ai_kills[1] = 0;
            GameObject_s *other = Player[(player_index + 1) & 1];
            if (other != NULL && other->coinpacket != NULL) {
                other->coinpacket->scale = 1.5f;
            }
        }
    } else {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
        if (Player[player_index] != NULL && Player[player_index]->coinpacket != NULL) {
            Player[player_index]->coinpacket->scale = 1.5f;
        }
        Arcade_NeedTwoPlayers_Scale = 1.5f;
    }
}

void Arcade_CoinCollected(i32 player_index, u32 *score, u32 previous_score) {
    if (static_cast<u32>(player_index) > 1) {
        return;
    }
    i32 flags = Arcade_Mode[ArcadeItem.field_c_0xc].field8_0x8;
    if ((flags & 8) != 0) {
        if (Player[player_index] != NULL && Player[player_index]->coinpacket != NULL &&
            Player[player_index]->coinpacket->coins >= arcade_placed_stud_total) {
            Arcade_AwardPoint(player_index, 0, 0);
        }
    } else if ((flags & 4) != 0) {
        if (Arcade_BothPlayersActive()) {
            if (Player[player_index] != NULL && Player[player_index]->coinpacket != NULL &&
                Player[player_index]->coinpacket->coins >=
                    static_cast<u32>(Arcade_Mode[ArcadeItem.field_c_0xc].target)) {
                Arcade_AwardPoint(player_index, 0, 0);
                if (Player[0] != NULL && Player[0]->coinpacket != NULL) {
                    Player[0]->coinpacket->coins = 0;
                }
                if (Player[1] != NULL && Player[1]->coinpacket != NULL) {
                    Player[1]->coinpacket->coins = 0;
                }
                GameObject_s *other = Player[(player_index + 1) & 1];
                if (other != NULL && other->coinpacket != NULL) {
                    other->coinpacket->scale = 1.5f;
                }
            }
        } else {
            *score = previous_score;
            Arcade_NeedTwoPlayers_Scale = 1.25f;
        }
    }
}

void Arcade_Kill(i32 player_index, i32 killed_player) {
    if (Arcade != 0 && static_cast<u32>(player_index) <= 1) {
        if (static_cast<u32>(killed_player) <= 1) {
            Arcade_PlayerKilled(player_index, 0);
        } else if (killed_player == -1) {
            Arcade_AIKilled(player_index);
        }
    }
}

void Arcade_UpdateEndMenu(MENU_s *menu) {
    if (menu->menu_time >= 5.0f) {
        NewLData = HUB_LDATA;
    } else if (menu->menu_time >= 1.5f && BonusWinFlag == 0) {
        PlaySfx(const_cast<char *>("Victory"), NULL);
        BonusWinFlag = 1;
    }
}

void Arcade_DrawEndMenu(MENU_s *) {
    if (MenuStopDraw != 0) {
        return;
    }
    SmartTextEx(TTab[tWINNER], 0.0f, STATSPOSY, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0, menu_flash ? 191 : 255, 0, 1.7f, 1, NULL,
                0, 128);
}

COLLECTION_s *GetFreePlayCollection(i32 area) {
    const u16 area_flags = ADataList[area].flags;
    if ((area_flags & AREAFLAG_VEHICLE_AREA) == 0) {
        return &CharacterCollection;
    }
    if ((area_flags & AREAFLAG_BONUS_AREA) != 0) {
        return &MiniKitCollection;
    }
    return &VehicleCollection;
}

void ResetIconWibble() {
    hub_iconang[0] = static_cast<u16>(qrand());
    hub_icontime[0] = 0.0f;
    hub_iconang[1] = static_cast<u16>(qrand());
    hub_icontime[1] = 0.0f;
    hub_iconang[2] = static_cast<u16>(qrand());
    hub_icontime[2] = 0.0f;
    hub_iconang[3] = static_cast<u16>(qrand());
    hub_icontime[3] = 0.0f;
}

void Hub_InitFreePlaySelect(i32 area, i32 first_model, i32 second_model) {
    COLLECTION_s *collection = GetFreePlayCollection(area);
    if (collection == &VehicleCollection) {
        if (first_model == -1)
            first_model = VehicleCollection.list[0].id;
        if (second_model == -1) {
            second_model = VehicleCollection.list[1].id;
            goto make_default_pair;
        }
        goto make_selected_pair;
    }
    if (collection == &MiniKitCollection) {
        if (first_model == -1)
            first_model = MiniKitCollection.list[0].id;
        if (second_model == -1) {
            second_model = MiniKitCollection.list[1].id;
            goto make_default_pair;
        }
        goto make_selected_pair;
    }
    Hub_MakeFreePlayList(-1, -1);
    goto use_player_models;

make_selected_pair:
    Hub_MakeFreePlayList(first_model, second_model);
    if (first_model == -1)
        goto use_player_models;
    goto store_model_pair;

make_default_pair:
    Hub_MakeFreePlayList(first_model, second_model);
    if (first_model == -1 || second_model == -1)
        goto use_player_models;

store_model_pair:
    MenuPacket.player_model[0] = static_cast<i16>(first_model);
    MenuPacket.player_model[1] = static_cast<i16>(second_model);
    goto initialize_selection;

use_player_models:
    MenuPacket.player_model[0] = Player[0] != NULL ? Player[0]->id : -1;
    MenuPacket.player_model[1] = Player[1] != NULL ? Player[1]->id : -1;

initialize_selection:
    freeplaytime = 0.0f;
    freeplaymode = 0;
    freeplayduration = 1.0f;
    freeplay_selected[0] = 0;
    freeplay_selected[1] = 0;
    uprepeattime[0] = 0.0f;
    rightrepeattime[0] = 0.0f;
    uprepeattime[1] = 0.0f;
    rightrepeattime[1] = 0.0f;
    downrepeattime[0] = 0.0f;
    leftrepeattime[0] = 0.0f;
    downrepeattime[1] = 0.0f;
    leftrepeattime[1] = 0.0f;
    ResetIconWibble();
    hub_freeplay_area = area;
}

static __used__ void Hub_UpdateSelectMode() {
    i16 ids[HUB_VEHICLE_ID_CAPACITY];
    if (FadeSys.fade > 0.0f || MainRenderTime > 0.0f) {
        return;
    }
    MENU *menu = &GameMenu[GameMenuLevel];
    const i32 area = static_cast<i8>(LDataList[hub_new_level].area_index);
    const i32 lost_temple = LOSTTEMPLE_ADATA != NULL && area == LOSTTEMPLE_ADATA->index;
    u16 freeplay_unlocked = FreePlayUnlocked();
    if (freeplay_unlocked != 1 && area != -1) {
        freeplay_unlocked =
            static_cast<i8>(ADataList[area].episode_index) == -1 && (ADataList[area].flags & 0x4000) != 0;
    }
    switch (selectmodemode) {
        case 1:
            selectmodetime += FRAMETIME;
            if (!(selectmodeduration <= selectmodetime)) {
                return;
            }
            MakeFreePlayModelList(MenuPacket.player_model[0], MenuPacket.player_model[1],
                                  static_cast<i8>(LDataList[hub_new_level].area_index), -1, 1);
            makeplayerlist_freeplay = 2;
            NextArea_FreePlay = 1;
            FreePlay = 1;
            NewLData = Area_FindNextPlayLevel(hub_new_level);
            break;
        case 2:
            selectmodetime += FRAMETIME;
            if (!(selectmodeduration <= selectmodetime) || NewLData != NULL) {
                return;
            }
            NextArea_FreePlay = 0;
            FreePlay = 0;
            NewLData = &LDataList[hub_new_level];
            break;
        case 3:
            selectmodetime += FRAMETIME;
            if (selectmodeduration <= selectmodetime) {
                WipeBackToHub();
            }
            return;
        case 4:
            selectmodetime += FRAMETIME;
            if (selectmodeduration <= selectmodetime) {
                NewLData = HUB_LDATA;
                PlayTrailer = hub_selectmode;
            }
            return;
        case 0: {
            i32 last_choice = 0;
            if (!lost_temple && (ADataList[area].flags & AREAFLAG_NO_FREEPLAY) == 0) {
                last_choice = 1;
                if ((ADataList[area].flags & 0x10) != 0 && Store_IsPackUnlocked(8)) {
                    last_choice = 2;
                }
            }
            const u32 select_mask = GAMEPAD_MENUSELECT;
            const u32 cancel_mask = GAMEPAD_MENUCANCEL;
            const u32 up_mask = GAMEPAD_DUP;
            const u32 down_mask = GAMEPAD_DDOWN;
            i32 confirm = 0, cancel = 0, up = 0, down = 0;
            for (i32 i = 0; i < 2; ++i) {
                if (!MenuPacket.active_player[i])
                    continue;
                const u32 pressed = GamePad[i].buttons_pressed;
                if ((pressed & select_mask) != 0) {
                    confirm = true;
                    break;
                }
                if ((pressed & cancel_mask) != 0) {
                    cancel = true;
                    break;
                }
                const u32 directions = pressed | GamePad[i].left_directions;
                if ((directions & up_mask) != 0) {
                    up = true;
                } else if ((directions & down_mask) != 0) {
                    down = true;
                }
            }
            if (menu->input_activity != 0) {
                if (menu->confirm_pressed != 0) {
                    menu->confirm_pressed = 0;
                    hub_selectmode = menu->selected_item;
                    confirm = true;
                } else if (menu->cancel_pressed != 0 && !confirm) {
                    cancel = true;
                }
            }
            if (confirm) {
                if (!lost_temple) {
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                    if (hub_selectmode == 0) {
                        selectmodetime = 0.0f;
                        selectmodemode = 2;
                        selectmodeduration = 0.6f;
                        return;
                    }
                    if (hub_selectmode == 1 && Game.area_save[area].area_complete != 0 && freeplay_unlocked) {
                        i32 first = -1, second = -1;
                        if ((ADataList[area].flags & 1) != 0) {

                            const i32 count =
                                Collection_GetIDList(&VehicleCollection, HUB_VEHICLE_COLLECTION_ALLOWED,
                                                     HUB_VEHICLE_COLLECTION_REQUIRED, ids, &first, &second, 0);
                            if (count == 1) {
                                selectmodetime = 0.0f;
                                selectmodemode = 1;
                                selectmodeduration = 0.6f;
                                MenuPacket.player_model[0] = ids[0];
                                MenuPacket.player_model[1] = ids[0];
                                return;
                            }
                        }
                        hub_freeplaysource = 0;
                        Hub_InitFreePlaySelect(area, first, second);
                        NewMenu(17, -1, -1);
                        return;
                    }
                    if (hub_selectmode == 2 && Game.area_save[area].area_complete != 0 && Store_IsPackUnlocked(8)) {
                        hub_freeplaysource = 0;
                        Hub_InitFreePlaySelect(area, -1, -1);
                        NewMenu(17, -1, -1);
                        return;
                    }
                    if (hub_selectmode != 1 && hub_selectmode != 2) {
                        return;
                    }
                }
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
            } else if (cancel) {
                GameAudio_PlaySfx(0x31, NULL, 0, 0);
                selectmodetime = 0.0f;
                selectmodemode = 3;
                selectmodeduration = 0.6f;
            } else {
                const i32 previous = hub_selectmode;
                if (up) {
                    if (hub_selectmode > 0) {
                        --hub_selectmode;
                    }
                } else if (down && hub_selectmode < last_choice) {
                    ++hub_selectmode;
                }
                if (hub_selectmode != previous) {
                    GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                }
            }
            return;
        }
        default:
            return;
    }
    loadareacharacters_no_backdrop_reset = 1;
    const FADETYPE fade = {FADE_TYPE_STILL};
    FadeSys.SetFade(fade, 0);
    FinishLoop_On = 0;
}

void UpdateIconWibble() {
    UpdateIconWibbleAxis(0);
    UpdateIconWibbleAxis(1);
    UpdateIconWibbleAxis(2);
    UpdateIconWibbleAxis(3);
}

void Hub_UpdateFreePlaySelect() {
    COLLECTION_s *collection = GetFreePlayCollection(hub_freeplay_area);
    if (freeplay_time[0] > 0.0f)
        freeplay_time[0] -= FRAMETIME;
    if (freeplay_time[1] > 0.0f)
        freeplay_time[1] -= FRAMETIME;
    UpdateIconWibble();
    MENU *menu = &GameMenu[GameMenuLevel];
    switch (freeplaymode) {
        case 0:
            freeplaytime += FRAMETIME;
            if (__builtin_expect(freeplaytime >= freeplayduration, 1)) {
                freeplaytime = 0.0f;
                freeplaymode = 1;
                freeplayduration = 0.0f;
                freeplay_i[0] = InCollectList_Index(MenuPacket.player_model[0], collection->list, collection->count_y);
                if (freeplay_i[0] == -1)
                    freeplay_i[0] = 0;
                freeplay_selected[0] = 0;
                freeplay_i[1] = InCollectList_Index(MenuPacket.player_model[1], collection->list, collection->count_y);
                if (__builtin_expect(freeplay_i[1] == -1, 1))
                    freeplay_i[1] = freeplay_i[0] == 1 ? 0 : 1;
                freeplay_selected[1] = 0;
            }
            break;
        case 1: {
            i32 active = 0, confirmed = 0;
            bool exit = false;
            auto menu_input_activity = menu->input_activity;
            auto collection_count_x = collection->count_x;
            for (i32 player = 0; player < 2; ++player) {
                const i32 other = (player + 1) & 1;
                const i32 previous = freeplay_i[player];
                i32 index = previous;
                bool select = false, cancel = false;
                if (__builtin_expect(menu_input_activity != 0 && player == 0, 1)) {
                    if (menu->confirm_pressed != 0) {
                        if (__builtin_expect(menu->selected_item == 9999, 0))
                            select = 1;
                        else
                            index = menu->selected_item;
                    }
                    cancel = menu->cancel_pressed != 0;
                }
                if (MenuPacket.active_player[player] != 0) {
                    if ((GamePad[player].pad->digital_buttons_pressed & GAMEPAD_MENUSELECT) != 0) {
                        select = 1;
                    } else if ((GamePad[player].pad->digital_buttons_pressed & GAMEPAD_MENUCANCEL) != 0) {
                        cancel = 1;
                    } else if (__builtin_expect(freeplay_selected[player] == 0, 0)) {
                        const u32 pressed = GamePad[player].buttons_pressed | GamePad[player].left_directions;
                        const u32 held = GamePad[player].buttons_held | GamePad[player].buttons_released;
                        i32 up = pressed & GAMEPAD_DUP, down = pressed & GAMEPAD_DDOWN;
                        i32 left = pressed & GAMEPAD_DLEFT, right = pressed & GAMEPAD_DRIGHT;
                        i32 up_held = held & GAMEPAD_DUP, down_held = held & GAMEPAD_DDOWN;
                        i32 left_held = held & GAMEPAD_DLEFT, right_held = held & GAMEPAD_DRIGHT;
                        if (down && up)
                            down = up = 0;
                        if (right && left)
                            right = left = 0;
                        if (down_held && up_held)
                            down_held = up_held = 0;
                        if (right_held && left_held)
                            right_held = left_held = 0;
                        MenuRepeat(&up_held, &up, &uprepeattime[player], &uprepeatcount[player], 0.1f, FRAMETIME);
                        MenuRepeat(&down_held, &down, &downrepeattime[player], &downrepeatcount[player], 0.1f,
                                   FRAMETIME);
                        MenuRepeat(&left_held, &left, &leftrepeattime[player], &leftrepeatcount[player], 0.1f,
                                   FRAMETIME);
                        MenuRepeat(&right_held, &right, &rightrepeattime[player], &rightrepeatcount[player], 0.1f,
                                   FRAMETIME);
                        if (up) {
                            const i32 columns = collection_count_x;
                            const i32 count = collection->count_y;
                            const i32 span = (static_cast<u32>(count) / static_cast<u32>(columns) + 1) * columns;
                            index = previous;
                            do {
                                index -= columns;
                                if (__builtin_expect(index < 0, 1)) {
                                    index += span;
                                    if (index >= count)
                                        index -= columns;
                                }
                            } while (collection->list[index].id == MenuPacket.player_model[other]);
                        } else if (down) {
                            const i32 count = collection->count_y;
                            const i32 columns = collection_count_x;
                            const i32 span = (static_cast<u32>(count) / static_cast<u32>(columns) + 1) * columns;
                            index = previous;
                            do {
                                index += columns;
                                if (index >= count) {
                                    if (index < span)
                                        index += columns;
                                    index -= span;
                                }
                            } while (collection->list[index].id == MenuPacket.player_model[other]);
                        } else if (__builtin_expect(!!(left), 0)) {
                            const i32 count = collection->count_y;
                            const i32 columns = collection_count_x;
                            index = previous;
                            do {
                                i32 column = index % columns - 1;
                                if (column < 0)
                                    column += columns;
                                index = (index / columns) * columns + column;
                                if (index >= count)
                                    index = count - 1;
                            } while (collection->list[index].id == MenuPacket.player_model[other]);
                        } else if (right) {
                            const i32 columns = collection_count_x;
                            const i32 count = collection->count_y;
                            index = previous;
                            do {
                                i32 column = index % columns + 1;
                                const i32 row = (index / columns) * columns;
                                if (column >= columns)
                                    column -= columns;
                                index = row + column < count ? row + column : row;
                            } while (collection->list[index].id == MenuPacket.player_model[other]);
                        }
                    }
                    ++active;
                    if (freeplay_selected[player] >= 2)
                        ++confirmed;
                }
                if (freeplay_selected[player] == 1 && freeplay_time[player] < 0.0f) {
                    freeplay_selected[player] = 2;
                } else if (select && freeplay_time[player] <= 0.0f) {
                    if (__builtin_expect(freeplay_selected[player] == 0, 1)) {
                        if (Collection_Got(collection->list[freeplay_i[player]].id) == 0) {
                            GameAudio_PlaySfx(0x32, NULL, 0, 0);
                        } else {
                            freeplay_time[player] = 0.75f;
                            freeplay_selected[player] = 1;
                            GameAudio_PlaySfx(0x30, NULL, 0, 0);
                        }
                    } else if (freeplay_selected[player] == 1) {
                        freeplay_selected[player] = 2;
                        GameAudio_PlaySfx(0x30, NULL, 0, 0);
                    }
                }
                if (cancel) {
                    if (freeplay_selected[player] != 1) {
                        if (freeplay_selected[player] != 2) {
                            if (freeplay_selected[player] == 0 && freeplay_time[player] <= 0.0f) {
                                exit = 1;
                            }
                        } else {
                            if (MenuPacket.active_player[other] != 0 && freeplay_selected[other] < 2) {
                                freeplay_selected[player] = 1;
                                GameAudio_PlaySfx(0x31, NULL, 0, 0);
                            }
                        }
                    } else {
                        if (freeplay_time[player] <= 0.0f) {
                            BigIconAlpha[player] = 0.0f;
                            freeplay_time[player] = 0.75f;
                            freeplay_selected[player] = 0;
                            GameAudio_PlaySfx(0x31, NULL, 0, 0);
                        }
                    }
                }
                // Touch navigation commits after selection, just like the
                // original: confirmation checks the previously selected model.
                if (index != previous) {
                    freeplay_i[player] = index;
                    MenuPacket.player_model[player] = collection->list[index].id;
                    GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                }
            }
            if (confirmed == active) {
                if (netclient == 0) {
                    const i32 area = LDataList[hub_new_level].area_index;
                    if (area == -1 || (ADataList[area].flags & 1) == 0)
                        hub_makefreeplaylist_addotherid = 1;
                    Hub_MakeFreePlayList(MenuPacket.player_model[0], MenuPacket.player_model[1]);
                } else {
                    if (net_recievedFreePlayList == 0)
                        break;
                    fpcount = 0;
                    for (i32 i = 0; i < net_FreePlayModelCount; ++i) {
                        FreePlayModelList[i] = NetFreePlayModelList[i];
                        if (Collection_Got(NetFreePlayModelList[i].model_id) != 0)
                            fplist[fpcount++] = NetFreePlayModelList[i];
                    }
                    fplist[fpcount].model_id = -1;
                    if (fpcount > 3) {
                        for (i32 shuffle = 0; shuffle < 64; ++shuffle) {
                            const i32 first_offset = qrand() / (0xffff / (fpcount - 2) + 1);
                            const i32 first = first_offset + 2;
                            const i32 second =
                                (qrand() / (0xffff / (fpcount - 3) + 1) + first_offset) % (fpcount - 2) + 2;
                            const APICHARACTERMODELLIST_s saved = fplist[first];
                            fplist[first] = fplist[second];
                            fplist[second] = saved;
                        }
                    }
                    NextArea_FreePlay = 1;
                    FreePlay = 1;
                }
                freeplaytime = 0.0f;
                freeplaymode = 3;
                freeplayduration = 1.0f;
            } else if (exit) {
                fpcount = 0;
                freeplaytime = 0.0f;
                freeplaymode = 2;
                freeplayduration = 0.6f;
                GameAudio_PlaySfx(0x31, NULL, 0, 0);
            }
            break;
        }
        case 2:
            freeplaytime += FRAMETIME;
            if (__builtin_expect(freeplaytime >= freeplayduration, 1))
                WipeBackToHub();
            break;
        case 3:
            freeplaytime += FRAMETIME;
            if (freeplaytime >= freeplayduration) {
                freeplaytime = 0.0f;
                freeplaymode = 4;
                freeplayduration = 1.5f;
            }
            break;
        case 4:
            freeplaytime += FRAMETIME;
            if (freeplaytime >= freeplayduration) {
                makeplayerlist_freeplay = 1;
                NewLData = &LDataList[hub_new_level];
                NextArea_FreePlay = 1;
                FreePlay = 1;
                NewLData = Area_FindNextPlayLevel(NewLData->idx);
                loadareacharacters_no_backdrop_reset = 1;
                FadeSys.fade = 1.0f;
                FinishLoop_On = 0;
                if (hub_freeplaysource == 0) {
                    if (hub_selectmode == 2)
                        InitChallenge(NewLData->area_index);
                } else if (__builtin_expect(hub_freeplaysource == 1 && bonusmodearcade != 0, 0)) {
                    Arcade = 1;
                    Arcade_Points[0] = Arcade_Points[1] = 0;
                }
            }
            break;
    }
    if (__builtin_expect(freeplaymode == 1, 1))
        BigIconAlpha[0] = SeekLinearF(BigIconAlpha[0], 1.0f, FRAMETIME + FRAMETIME);
    else
        BigIconAlpha[0] = 0.0f;
    if (freeplaymode == 1)
        BigIconAlpha[1] = SeekLinearF(BigIconAlpha[1], 1.0f, FRAMETIME + FRAMETIME);
    else
        BigIconAlpha[1] = 0.0f;
}

// The level callback carries the streaming-buffer arguments used by most
// level loaders, but the original hub loader only forwards the world to the
// persistent character customiser.
void Hub_Load(WORLDINFO_s *world, variptr_u *, variptr_u *) {
    Customiser_LoadAll(CharacterCustomiser, world);
}

void Hub_LoadAndFixUpMiniKits(WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end) {
    buf->addr = ALIGN(buf->addr, 4);
    world->hub_minikits = reinterpret_cast<HUBMINIKIT_s *>(buf->addr);
    buf->addr = ALIGN(buf->addr + static_cast<usize>(AREACOUNT) * sizeof(HUBMINIKIT_s), 4);
    world->minikit_pieces_buf = reinterpret_cast<HUBMINIKITPIECES_s **>(buf->addr);
    memset(world->minikit_pieces_buf, 0, static_cast<usize>(AREACOUNT) * sizeof(*world->minikit_pieces_buf));
    buf->addr += static_cast<usize>(AREACOUNT) * sizeof(*world->minikit_pieces_buf);

    for (i32 i = 0; i < AREACOUNT; ++i) {
        world->minikit_pieces_buf[i] = NULL;
        if ((ADataList[i].flags & AREAFLAG_MINIKIT) == 0 || ADataList[i].minikit_id == -1) {
            continue;
        }
        world->minikit_pieces_buf[i] = reinterpret_cast<HUBMINIKITPIECES_s *>(buf->addr);
        buf->addr += 0x18;
        MiniKit_Load(reinterpret_cast<MINIKIT *>(world->minikit_pieces_buf[i]), ADataList[i].minikit_id, buf, buf_end,
                     NULL);
        MINIKIT *minikit = reinterpret_cast<MINIKIT *>(world->minikit_pieces_buf[i]);
        if (minikit->gscn == NULL) {
            world->minikit_pieces_buf[i] = NULL;
            buf->addr -= 0x18;
        } else {
            MiniKit_InitPieces(minikit, 10, buf, buf_end);
            minikit->field_0x9 = static_cast<i8>(i);
        }
    }
}

void Hub_InitMiniKits(WORLDINFO_s *world) {
    HUBMINIKIT_s *kit = world->hub_minikits;
    if (kit != NULL) {
        for (i32 area_index = 0; area_index < AREACOUNT; ++area_index, ++kit) {
            kit->position.y = 2000000.0f;
            kit->scale = 0.0f;
            kit->field_0x10 = 0.0f;
            kit->rotation = qrand();
            for (HUBAREAINFO_s *info = HubAreaInfo; info->area_name != NULL; ++info) {
                if (info->area != NULL && area_index == info->area->index)
                    break;
            }
            const f32 height = GameShadow(NULL, &kit->position, 5.0f, -1);
            if (height != 2000000.0f)
                kit->position.y = height;
            kit->phase_x = qrand();
            kit->phase_y = qrand();
            kit->phase_z = qrand();
            kit->phase_rotation = qrand();
            kit->rate_x = static_cast<i32>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 5461.0f + 5461.0f);
            kit->rate_y = static_cast<i32>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 5461.0f + 5461.0f);
            kit->rate_z = static_cast<i32>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 5461.0f + 5461.0f);
            kit->rate_rotation = static_cast<i32>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 10922.0f + 10922.0f);
            kit->velocity_x = 0.0f;
            kit->rotation_velocity = 0;
            kit->offset_x = 0.0f;
            kit->velocity_z = 0.0f;
            kit->target_rotation = kit->rotation;
            kit->offset_z = 0.0f;
            kit->tilt = 0;
            kit->target_piece_count = 0;
            kit->displayed_piece_count = 0;
        }
    }
}

void Hub_UpdateMiniKits(WORLDINFO_s *world) {
    i32 count[72];
    i32 viewer = 0;
    f32 speed = 0.0f;
    u16 angle = 0;
    if (GetMenuID() == 14) {
        viewer = 1;
        MENU *menu = &GameMenu[GameMenuLevel];
        if (hub_minikitviewer_movewait <= 0.0f) {
            if (Hub_PadSpeed[0] > 0.0f && Hub_PadSpeed[1] > 0.0f) {
                speed = (Hub_PadSpeed[0] + Hub_PadSpeed[1]) * 0.5f;
                angle = Hub_PadAngle[0] + RotDiff(Hub_PadAngle[0], Hub_PadAngle[1]) * 0.5f + GameCam->yaw;
            } else if (Hub_PadSpeed[0] > 0.0f) {
                speed = Hub_PadSpeed[0];
                angle = Hub_PadAngle[0] + GameCam->yaw;
            } else if (Hub_PadSpeed[1] > 0.0f) {
                speed = Hub_PadSpeed[1];
                angle = Hub_PadAngle[1] + GameCam->yaw;
            }
            i32 confirm = 0;
            if (menu->input_activity && menu->confirm_pressed) {
                if (menu->selected_item == i_selectminikitepisode)
                    confirm = 1;
                else
                    i_selectminikitepisode = menu->selected_item;
            }
            if (confirm || (GamePad[0].buttons_pressed & GAMEPAD_MENUSELECT)) {
                if (Game_AreaSave == NULL || Game_AreaSave[hub_minikitviewer_area].minikit_complete) {
                    if (Episode_CountOpenAreas(i_selectminikitepisode, -1, Game_AreaSave)) {
                        i32 area = Episode_FindAreaFromFlags(&EDataList[i_selectminikitepisode], 5, 5);
                        if (area != -1) {
                            hub_new_level = ADataList[area].levels[0];
                            Hub_MakeFreePlayList(ADataList[hub_minikitviewer_area].minikit_id, -1);
                            makeplayerlist_freeplay = 1;
                            NewLData = &LDataList[hub_new_level];
                            NextArea_FreePlay = 1;
                            FreePlay = 1;
                            GameAudio_PlaySfx(48, NULL, 0, 0);
                        }
                    } else {
                        GameAudio_PlaySfx(50, NULL, 0, 0);
                        GameCam_HitRoll();
                    }
                }
            } else if ((GamePad[0].buttons_pressed & GAMEPAD_DLEFT) && i_selectminikitepisode > 0) {
                --i_selectminikitepisode;
            } else if ((GamePad[0].buttons_pressed & GAMEPAD_DRIGHT) && i_selectminikitepisode < 5) {
                ++i_selectminikitepisode;
            }
        }
    }
    if (world->minikit_pieces_buf != NULL && world->hub_minikits != NULL) {
        if (viewer) {
            if (hub_minikitviewer_alpha < 1.0f) {
                hub_minikitviewer_alpha += FRAMETIME * 2.0f;
                if (hub_minikitviewer_alpha > 1.0f)
                    hub_minikitviewer_alpha = 1.0f;
            }
        } else
            hub_minikitviewer_alpha = 0.0f;
        if (hub_minikitviewer_movewait > 0.0f) {
            hub_minikitviewer_movewait -= FRAMETIME;
            if (hub_minikitviewer_movewait <= 0.0f)
                hub_minikitviewer_move = 0.0f;
        }
        PlaySfx("ui_hover_lp", &hub_minikitviewer_pos);
        HUBMINIKIT_s *kit = world->hub_minikits;
        for (i32 i = 0; i < AREACOUNT; ++i, ++kit) {
            HUBMINIKITPIECES_s *pieces = world->minikit_pieces_buf[i];
            if (pieces == NULL)
                continue;
            count[i] = Game.area_save[i].minikit_count;
            if (count[i] > pieces->piece_count)
                count[i] = pieces->piece_count;
            kit->scale = 1.0f;
            f32 vx = (0.0f - kit->offset_x) * 5.0f;
            f32 vz = (0.0f - kit->offset_z) * 5.0f;
            kit->velocity_x = SeekValF(kit->velocity_x, vx, 10.0f);
            kit->velocity_z = SeekValF(kit->velocity_z, vz, 10.0f);
            kit->offset_z += kit->velocity_z * FRAMETIME;
            kit->offset_x += kit->velocity_x * FRAMETIME;
            u16 old_rotation = kit->rotation;
            if (i == hub_minikitviewer_area) {
                if (speed > 0.0f)
                    kit->target_rotation =
                        TurnRot(kit->target_rotation, angle + 0x8000, (u16)(32768.0f * kit->scale), NULL);
                kit->target_piece_count = count[i];
                if (viewer && count[i] >= pieces->piece_count &&
                    ((MenuPacket.active_player[0] && (GamePad[0].buttons_held & GAMEPAD_ACTION)) ||
                     (MenuPacket.active_player[1] && (GamePad[1].buttons_held & GAMEPAD_ACTION))))
                    kit->target_piece_count = 1;
            } else if (count[i] != 0)
                kit->target_piece_count = count[i];
            else {
                kit->target_piece_count = 0;
                kit->displayed_piece_count = 0;
            }
            kit->rotation = SeekRot(kit->rotation, kit->target_rotation, 3.0f);
            i32 rate = (i32)((f32)RotDiff(old_rotation, kit->rotation) / FRAMETIME);
            rate = (MAX(-65536, (MIN(65536, rate))));
            rate /= 8;
            rate = (MAX(-4551, (MIN(4551, rate))));
            kit->rotation_velocity = SeekRot(kit->rotation_velocity, rate, 8.0f);
            i32 tilt = 0;
            if (i == hub_minikitviewer_area && speed > 0.0f) {
                i32 diff = RotDiff(old_rotation, kit->rotation);
                f32 angular_speed = (f32)(diff < 0 ? -diff : diff) / FRAMETIME;
                if (angular_speed < 32768.0f)
                    tilt = -(i32)((1.0f - angular_speed * (1.0f / 32768.0f)) * 4551.0f);
            }
            kit->tilt = SeekRot(kit->tilt, tilt, 3.0f);
            for (i32 j = 0; j < pieces->piece_count; ++j) {
                pieces->pieces[j].enabled = 0;
                if (NuSpecialExistsFn(&pieces->pieces[j])) {
                    NuSpecialSetVisibility(&pieces->pieces[j], 0);
                    if (j < count[i])
                        pieces->pieces[j].enabled = 1;
                }
            }
            kit->phase_x = (i32)((f32)kit->phase_x + kit->rate_x * FRAMETIME);
            if (kit->phase_x > 65536)
                kit->phase_x -= 65536;
            kit->phase_y = (i32)((f32)kit->phase_y + kit->rate_y * FRAMETIME);
            if (kit->phase_y > 65536)
                kit->phase_y -= 65536;
            kit->phase_z = (i32)((f32)kit->phase_z + kit->rate_z * FRAMETIME);
            if (kit->phase_z > 65536)
                kit->phase_z -= 65536;
            kit->phase_rotation = (i32)((f32)kit->phase_rotation + kit->rate_rotation * FRAMETIME);
            if (kit->phase_rotation > 65536)
                kit->phase_rotation -= 65536;
            i32 model = ADataList[i].minikit_id;
            if (model != -1 && world->hub_minikits != NULL) {
                HUBMINIKIT_s *current = &world->hub_minikits[i];
                HUBAREAINFO_s *info = Hub_FindAreaInfoByIndex(i);
                CHARACTERDATA *data = &CDataList[model];
                current->radius = data->collision_radius;
                current->height_ratio = (data->bounds_max_y - data->bounds_min_y) / (data->collision_radius * 2.0f);
                current->collision_center.x = current->position.x + current->offset_x;
                current->collision_center.y = current->position.y + (data->bounds_max_y + data->bounds_min_y) * 0.5f;
                current->collision_center.z = current->position.z + current->offset_z;
                current->collision_center.y +=
                    (1.0f - (NU_SIN_LUT((i32)(32768.0f * current->scale + 16384.0f)) + 1.0f) * 0.5f) * 0.333f;
                current->collision_center.y += 0.025f * NU_SIN_LUT(current->phase_rotation) * current->scale;
                current->collision_center.y += info ? info->panel_offset : 0.0f;
                current->bounds_min.x = current->collision_center.x - current->radius;
                current->bounds_min.z = current->collision_center.z - current->radius;
                current->bounds_min.y = current->collision_center.y - current->radius * current->height_ratio;
                current->bounds_max.y = current->collision_center.y + current->radius * current->height_ratio;
                current->bounds_max.z = current->collision_center.z + current->radius;
                current->bounds_max.x = current->collision_center.x + current->radius;
            }
        }
        kit = world->hub_minikits;
        for (i32 i = 0; i < AREACOUNT; ++i, ++kit) {
            if (world->minikit_pieces_buf[i] == NULL)
                continue;
            if (kit->displayed_piece_count != kit->target_piece_count &&
                (i32)(GameTimer.time_elapsed / 0.15f) != (i32)(GameTimer.last_time_elapsed / 0.15f)) {
                if (kit->displayed_piece_count < kit->target_piece_count)
                    ++kit->displayed_piece_count;
                else if (kit->displayed_piece_count > kit->target_piece_count)
                    --kit->displayed_piece_count;
                if (i == hub_minikitarea)
                    PlaySfx("LegoForm", &kit->position);
                NewRumbleAllPlayers(0.0f, 0.0f, 1, 0);
            }
            if (count[i] > 0 && kit->displayed_piece_count != 0) {
                PushAway(&kit->collision_center, kit->radius, &kit->bounds_min, &kit->bounds_max, NULL, NULL, 2.5f, 5);
                AIAntinodeCreateSingleFrame(&kit->collision_center, kit->radius);
            }
        }
    }
}

void Hub_DrawMiniKits(WORLDINFO_s *world) {
    NUMTX piece_matrix;
    if (world->minikit_pieces_buf == NULL || world->hub_minikits == NULL)
        return;
    i32 areas[72];
    i32 selected = -1;
    i32 count = 0;
    for (i32 area = 0; area < AREACOUNT; ++area) {
        if (ADataList[area].flags & 0x10) {
            areas[count] = area;
            if (area == hub_minikitviewer_area)
                selected = count;
            ++count;
        }
    }
    for (i32 index = 0; index < count; ++index) {
        HUBMINIKIT_s *kit = world->hub_minikits;
        const i32 area = areas[index];
        HUBMINIKITPIECES_s *pieces = world->minikit_pieces_buf[area];
        if (pieces == NULL || ADataList[area].minikit_id == -1)
            continue;
        HUBAREAINFO_s *info = Hub_FindAreaInfoByIndex(area);
        f32 x;
        if (area == areas[(selected + (count - 2)) % count])
            x = -4.0f;
        else if (area == areas[(selected + (count - 1)) % count])
            x = -2.0f;
        else if (area == areas[selected])
            x = 0.0f;
        else if (area == areas[(selected + 1) % count])
            x = 2.0f;
        else if (area == areas[(selected + 2) % count])
            x = 4.0f;
        else
            continue;
        x += hub_minikitviewer_pos.x;
        if (hub_minikitviewer_movewait > 0.0f) {
            x += (1.0f - (1.0f + NU_SIN_LUT((i32)(hub_minikitviewer_movewait * 32768.0f + 16384.0f))) * 0.5f) * 2.0f *
                 hub_minikitviewer_move;
        }
        kit += area;
        const f32 scale = kit->scale;
        f32 lift = 1.0f - (1.0f + NU_SIN_LUT((i32)(scale * 32768.0f + 16384.0f))) * 0.5f;
        const u16 phase_x = (i32)(NU_SIN_LUT(kit->phase_x) * 910.0f);
        const u16 phase_z = (i32)(NU_SIN_LUT(kit->phase_z) * 910.0f);
        const u16 phase_y = (i32)(NU_SIN_LUT(kit->phase_y) * 910.0f);
        lift *= 0.333f;
        const u16 rotation = (i32)(RotDiff(0, phase_y) * scale) + kit->rotation;
        const f32 bob = NU_SIN_LUT(kit->phase_rotation) * 0.025f * scale;
        const f32 tilt_scale = (info ? info->panel_scale : 1.0f) * scale;
        NUMTX matrix;
        MiniKitSetRotationX(&matrix, (i32)(RotDiff(0, kit->tilt) * tilt_scale));
        MiniKitRotateX(&matrix, (i32)(RotDiff(0, phase_x) * scale));
        MiniKitRotateZ(&matrix, (i32)(RotDiff(0, phase_z) * scale));
        MiniKitRotateY(&matrix, rotation);
        matrix.m30 = x + kit->offset_x;
        matrix.m31 = hub_minikitviewer_pos.y + (lift + bob + (info ? info->panel_offset : 0.0f));
        matrix.m32 = hub_minikitviewer_pos.z + kit->offset_z;
        NuMtxPreRotateZ(&matrix, (i32)(RotDiff(0, kit->rotation_velocity) * scale));
        for (i32 piece = 0; piece < pieces->piece_count; ++piece) {
            if (pieces->pieces[piece].enabled && NuSpecialExistsFn(&pieces->pieces[piece].special) &&
                piece < kit->displayed_piece_count) {
                piece_matrix = pieces->pieces[piece].matrix;
                NuMtxMulVU0(&piece_matrix, &piece_matrix, &matrix);
                NUMTX reflected;
                const i32 drawn = NuSpecialDrawAt(&pieces->pieces[piece].special, &piece_matrix);
                if (HUB_MINIKITVIEWER_REFLECTY != 2000000.0f &&
                    MatrixReflection(&piece_matrix, 2, HUB_MINIKITVIEWER_REFLECTY, 2000000.0f, &reflected)) {
                    NuRndrStartReflectionRender(drawn);
                    NuSpecialDrawAt(&pieces->pieces[piece].special, &reflected);
                    NuRndrEndReflectionRender();
                }
            }
        }
        if (NuSpecialExistsFn(&pieces->base.special)) {
            NUMTX base_matrix;
            NuMtxSetRotationY(&base_matrix, rotation);
            base_matrix.m30 = x;
            base_matrix.m31 = 0.143f;
            base_matrix.m32 = hub_minikitviewer_pos.z;
            NuSpecialDrawAtAlpha(&pieces->base.special, &base_matrix, 1.0f - 0.5f * kit->scale);
        }
    }
    SetLevelLights(world->rtl_set, 1.0f);
}

void Hub_LockUnlockDoors(WORLDINFO_s *) {
}

void Hub_DrawStarField() {
    GetMenuID();
}

void Hub_DrawImportantBrick(i32 object, f32 x, f32 y, f32 alpha, i32 count, i32 total) {
    char text[32];
    const i32 rotation = static_cast<i32>((NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f) * 65536.0f) & 0xffff;
    const f32 scale = NU_SIN_LUT(static_cast<i32>(alpha * 16384.0f)) * PANEL_REDBRICKSCALE;
    const u16 tilt = static_cast<i32>(1820.0f * NuTrigTable[rotation & 0x7fff]) - 910;
    DrawPanel3DObjectNoAlpha(x, y + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 1.0f, scale, scale, scale, tilt, rotation, 0,
                             &WORLD->lev_objs[object].special, 2);
    if (total >= 0 && count >= 0) {
        if (total == 1)
            NuStrCpy(text, count == 1 ? "$" : "X");
        else
            sprintf(text, "%i/%i", count, total);
        Text3DEx(text, x, y, 1.0f, PANEL_MINIKITCOUNTSCALE * stats_xscale, PANEL_MINIKITCOUNTSCALE,
                 PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(static_cast<i32>(128.0f * alpha)));
    }
}

static void Hub_DrawArcadeStats(float alpha) {
    const i32 menu_id = GetMenuID();
    f32 alpha_scaled = alpha * 128.0f;
    const i32 opacity = static_cast<i32>(alpha_scaled);
    const f32 title_scale = HUB_EPISODESUBTITLESIZE;
    SmartTextEx(TTab[tARCADE_NAME], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, title_scale, title_scale, title_scale, 0,
                static_cast<u8>(HUB_EPISODER), static_cast<u8>(HUB_EPISODEG), static_cast<u8>(HUB_EPISODEB), 1.7f, 1, 0,
                0, opacity);

    i32 complete_count = 0;
    i32 area_count = 0;
    if (Game_AreaSave != NULL) {
        for (i32 i = 0; i < 12; ++i) {
            AREADATA *area = *ArcadeLevel[i].area;
            if ((area->flags & 0x800) != 0)
                continue;
            ++area_count;
            AREASAVE_s *save = &Game_AreaSave[area->index];
            if (save->area_complete ||
                static_cast<f32>(static_cast<i32>(area->challenge_trial_time)) > save->challenge_trial_time)
                ++complete_count;
        }
    }
    Hub_DrawImportantBrick(211, 0.0f, HUB_EPISODETITLEY, alpha, complete_count, area_count);

    if (GetMenuID() == 16) {
        return;
    }
    if (menu_id != -1 || Arcade_BothPlayersActive())
        return;
    i32 text_id = tARCADE_NEEDTWOPLAYERS;
    if (text_id == -1)
        return;
    const f32 pulse = 0.75f + 0.25f * NU_SIN_LUT(static_cast<u16>(static_cast<i32>(
                                          NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f)));
    SmartTextEx(TTab[text_id], 0.0f, HUB_EPISODETITLEY, 1.0f, HUB_EPISODETITLESIZE, HUB_EPISODETITLESIZE,
                HUB_EPISODETITLESIZE, 0, 255, 0, 0, 1.7f, 1, 0, 0, static_cast<i32>(pulse * alpha_scaled));
}

static void Hub_DrawSuperBonusStats(AREADATA_s *area, float alpha) {
    const i32 opacity = static_cast<i32>(alpha * 128.0f);
    char *title = "?";
    if (area)
        title = TTab[area->name_id];
    SmartTextEx(title, 0.0f, HUB_EPISODESUBTITLEY, 1.0f, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE,
                HUB_EPISODESUBTITLESIZE, 0, static_cast<u8>(HUB_EPISODER), static_cast<u8>(HUB_EPISODEG),
                static_cast<u8>(HUB_EPISODEB), 1.7f, 1, NULL, 0, opacity);
    if (area && Game.area_save[area->index].area_complete) {
        char text[64];
        SmartTextEx(TTab[tBESTTIME], 0.0f, HUB_EPISODETITLEY + 0.125f, 1.0f, HUB_EPISODETITLESIZE, HUB_EPISODETITLESIZE,
                    HUB_EPISODETITLESIZE, 0, 191, 0, 127, 0.7f, 1, NULL, 0, opacity);
        Text_MakeTime(Game.area_save[area->index].challenge_trial_time, 1, 1, 1, text);
        Text3DEx(text, 0.0f, HUB_EPISODETITLEY, 1.0f, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE,
                 HUB_EPISODESUBTITLESIZE, 0, 255, 255, 255, static_cast<u8>(opacity));
        if (GOLDBRICKFORSUPERBONUS)
            Hub_DrawImportantBrick(211, 0.0f, HUB_EPISODETITLEY + 0.125f, alpha, -1, -1);
    }
}

void Hub_DrawAreaStats(f32 phase, i32 area_index, i32 mode) {
    AREADATA *areas = ADataList;
    char heading[128];
    char text[128];
    i16 clips[128];
    if (mode == 18 || (LOSTTEMPLE_ADATA != NULL && LOSTTEMPLE_ADATA->index == area_index)) {
        char *subtitle = TTab[tSTORYCLIPS2];
        heading[0] = '\0';
        bool fade = lastepisodesmode != -1;
        if (mode == 18) {
            if (episodesmode == 3) {
                const i32 all = hub_new_level == -1 || LDataList[hub_new_level].episode_index == -1;
                CutScenePlayer_CountEpisodeClips(i_episodes, all, clips);
                CutScenePlayer_GetText(clips[i_clip[i_episodes]], heading, text, 3);
                subtitle = text;
            } else if (episodesmode == 2 ||
                       (episodesmode == 1 && hub_new_level != -1 && LDataList[hub_new_level].episode_index != -1)) {
                subtitle = TTab[EDataList[i_episodes].text_id];
                NuStrCpy(heading, TTab[EDataList[i_episodes].name_id]);
            }
            if (__builtin_expect(lastepisodesmode == -1, 1)) {
                fade = episodesmode == 2 && hub_new_level != -1 && LDataList[hub_new_level].episode_index != -1;
            } else {
                fade = episodesmode != 1;
            }
        }
        if (fade && episodestime < episodesduration) {
            phase = (episodestime / episodesduration) * phase;
        }
        if (subtitle != NULL) {
            SmartTextEx(subtitle, 0.0f, HUB_EPISODESUBTITLEY, 1.0f, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE,
                        HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0,
                        static_cast<i32>(128.0f * phase));
        }
        if (heading[0] != '\0') {
            SmartTextEx(heading, 0.0f, HUB_EPISODETITLEY, 1.0f, HUB_EPISODETITLESIZE, HUB_EPISODETITLESIZE,
                        HUB_EPISODETITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0,
                        static_cast<i32>(phase * 128.0f));
        }
        return;
    }
    AREADATA *area = &areas[area_index];
    const i32 alpha = static_cast<i32>(phase * 128.0f);

    if (VEHICLES_ADATA != NULL && VEHICLES_ADATA->index == area_index) {
        return;
    }

    SmartTextEx(TTab[area->name_id], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE,
                HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0, alpha);

    AREASAVE_s *save = &Game.area_save[area_index];
    const u16 flags = area->flags;
    if ((flags & 0x10) != 0) {
        if (save->area_complete != 0 && (flags & 4) == 0) {
            Episode_CountOpenAreas(static_cast<i8>(area->episode_index), area_index, Game.area_save);

            if (BOTHTRUEJEDIGOLDBRICKS != 0) {
                DrawBuildUpBar(HUB_AREAPANELX[1], HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                               NU_SIN_LUT(static_cast<i32>(16384.0f * phase)), 1.0f, 1.0f, 0);
                NuStrCpy(text, save->true_hero_complete[0] ? "$" : "X");
                Text3DEx(text, HUB_AREAPANELX[1], HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE,
                         PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
                SmartTextEx(TTab[tSTORY], HUB_AREAPANELX[1], HUB_EPISODETITLEY + 0.235f, 1.0f, 0.45f, 0.45f, 0.45f, 0,
                            255, 255, 255, 0.4f, 1, NULL, 0, static_cast<i32>(static_cast<f32>(TJTYPEA) * phase));

                DrawBuildUpBar(HUB_AREAPANELX[4], HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                               NU_SIN_LUT(static_cast<i32>(16384.0f * phase)), 1.0f, 1.0f, 0);
                NuStrCpy(text, save->true_hero_complete[1] ? "$" : "X");
                Text3DEx(text, HUB_AREAPANELX[4], HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE,
                         PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
                SmartTextEx(TTab[tFREEPLAY], HUB_AREAPANELX[4], HUB_EPISODETITLEY + 0.235f, 1.0f, 0.45f, 0.45f, 0.45f,
                            0, 255, 255, 255, 0.4f, 1, NULL, 0, static_cast<i32>(static_cast<f32>(TJTYPEA) * phase));
            } else {
                DrawBuildUpBar(HUB_AREAPANELX[1], HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                               NU_SIN_LUT(static_cast<i32>(16384.0f * phase)), 1.0f, 1.0f, 0);
                NuStrCpy(text, (save->true_hero_complete[0] || save->true_hero_complete[1]) ? "$" : "X");
                Text3DEx(text, HUB_AREAPANELX[1], HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE,
                         PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
            }

            if ((area->flags & 0x10) != 0) {
                Hub_DrawMiniKitCount(HUB_AREAPANELX[0], HUB_EPISODETITLEY, save->minikit_count, 10, phase);
            }
            Hub_DrawImportantBrick(210, HUB_AREAPANELX[2], HUB_EPISODETITLEY, phase, save->red_brick_collected, 1);
            Hub_DrawImportantBrick(211, HUB_AREAPANELX[3], HUB_EPISODETITLEY, phase, EpGoldBrickCount,
                                   EpGoldBrickTotal);
            if (Store_IsPackUnlocked(8)) {
                hub_drawminikitcount_charkit = 1;
                Hub_DrawMiniKitCount(HUB_AREAPANELX[5], HUB_EPISODETITLEY, EpCharKitCount, EpCharKitTotal, phase);
            }
            return;
        }

        if (static_cast<i8>(area->area_index) != -1 && (flags & 0x14) == 0x10) {
            sprintf(text, "%s %i", TTab[tCHAPTER], static_cast<i8>(area->area_index) + 1);
            SmartTextEx(text, 0.0f, HUB_EPISODETITLEY, 1.0f, HUB_EPISODETITLESIZE, HUB_EPISODETITLESIZE,
                        HUB_EPISODETITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0, alpha);
        }
        return;
    }

    if (__builtin_expect(save->area_complete == 0, 1)) {
        return;
    }

    f32 build_x = 0.0f;
    if ((flags & 0x4000) != 0)
        build_x = 0.20100002f;
    if ((flags & 0x800) != 0) {
        Hub_DrawImportantBrick(251, build_x, HUB_EPISODESUBTITLEY - 0.05f, phase, -1, -1);
    } else if ((flags & 0x4000) != 0) {
        const i32 gold_count = 1 + (save->true_hero_complete[0] || save->true_hero_complete[1]);
        Hub_DrawImportantBrick(211, -0.20100002f, HUB_EPISODETITLEY, phase, gold_count, 2);
    } else {
        Hub_DrawImportantBrick(211, -0.0f, HUB_EPISODESUBTITLEY + 0.05f, phase, -1, -1);
    }
    if ((area->flags & 0x4000) == 0) {
        return;
    }
    DrawBuildUpBar(build_x, HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                   NU_SIN_LUT(static_cast<i32>(16384.0f * phase)), 1.0f, 1.0f, 0);
    NuStrCpy(text, (save->true_hero_complete[0] || save->true_hero_complete[1]) ? "$" : "X");
    Text3DEx(text, build_x, HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE,
             PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
}

void Hub_DrawFreePlaySelect() {
    static f32 radarpulsetimer = 2.0f;
    static APICHARACTERMODELLIST_s list[341];
    COLLECTION_s *collection = GetFreePlayCollection(hub_freeplay_area);
    if (FadeSys.fade > 0.0f)
        return;
    const i32 menu_level = GameMenuLevel;
    if (freeplaymode == 0) {
        const f32 progress = freeplaytime / (freeplayduration * 0.5f);
        const f32 alpha = 1.0f - progress;
        if (progress <= 1.0f && alpha > 0.0f) {
            AREADATA *area = &ADataList[last_hub_area];
            if ((area->flags & 0x100) == 0) {
                if ((area->flags & AREAFLAG_BONUS_AREA) != 0) {
                    if (bonusmodearcade != 0) {
                        Hub_DrawArcadeStats(alpha);
                    } else {
                        Hub_DrawBonusStats(alpha, -1, hub_bonusepisode);
                    }
                    Hub_DrawBonusModeMenu(0, alpha);
                } else {
                    Hub_DrawAreaStats(alpha, last_hub_area, -1);
                    if (VEHICLES_ADATA == NULL || VEHICLES_ADATA->index != last_hub_area) {
                        Hub_DrawSelectModeMenu(0, alpha);
                    }
                }
            } else {
                Hub_DrawSuperBonusStats(area, alpha);
                Hub_DrawBonusModeMenu(0, alpha);
            }
        }
    }

    if (freeplaymode != 3 && freeplaymode != 4) {
        f32 fade = 1.0f;
        if (freeplaymode == 2) {
            fade = 0.0f;
            if (freeplaytime < freeplayduration - 0.1f)
                fade = 1.0f - freeplaytime / (freeplayduration - 0.1f);
        }
        i32 model = FreePlayModelList[0].model_id;
        f32 opacity = (MenuPacket.active_player[0] ? 1.0f : DROPINALPHA) * fade;
        if (freeplay_time[0] > 0.0f) {
            f32 progress = freeplay_time[0] / 0.75f;
            if (__builtin_expect(freeplay_selected[0] == 0, 0))
                progress = 1.0f - progress;
            opacity *= progress;
        } else if (freeplay_selected[0] != 0) {
            model = MenuPacket.player_model[0];
        }
        if (opacity > 0.0f)
            DrawCharIcon(model, -ICONX, STATSPOSY, 0.0f, ICONSIZE, 0xa6, opacity, opacity, 1, NULL);
        if (__builtin_expect(freeplaymode == 1 && MenuPacket.active_player[0] != 0, 0)) {
            const i32 selected = MenuPacket.player_model[0];
            model = Collection_Got(selected) ? selected : -1;
            opacity = MenuPacket.active_player[0] ? 1.0f : DROPINALPHA;
            if (freeplay_time[0] <= 0.0f)
                opacity *= BigIconAlpha[0];
            const f32 base_x = MenuPacket.active_player[1] ? -0.4f : 0.0f;
            const f32 base_size = Game_OptionsSave != NULL && Game_OptionsSave->widescreen != 0 ? 0.30625f : 0.35f;
            f32 x = base_x, y = 0.7f, size = base_size;
            // An already-selected icon remains at its base without drawing or
            // name/radar updates until the next transition starts.
            if (freeplay_time[0] > 0.0f || freeplay_selected[0] == 0) {
                f32 eased = 0.0f;
                f32 progress = 1.0f;
                if (__builtin_expect(freeplay_time[0] > 0.0f, 0)) {
                    progress = freeplay_time[0] / 0.75f;
                    if (freeplay_selected[0] == 0)
                        progress = 1.0f - progress;
                    eased =
                        1.0f - (NU_SIN_LUT(static_cast<i32>((1.0f - progress) * 32768.0f + 16384.0f)) + 1.0f) * 0.5f;
                }
                x = (-ICONX - base_x) * eased + base_x;
                size = (COLLECTION_ICONSIZE - base_size) * eased + base_size;
                y = (STATSPOSY - 0.7f) * eased + 0.7f;
                if (opacity > 0.0f) {
                    if (freeplay_time[0] > 0.0f || opacity != 1.0f) {
                        radarpulsetimer = 2.0f;
                    } else if (radarpulsetimer < 0.0f) {
                        const VuVec position{x, y, 0.0f, 1.0f};
                        MechSystems::Get()->NewRadarPulse(position, false);
                        radarpulsetimer = 2.0f;
                    } else {
                        radarpulsetimer -= FRAMETIME;
                    }
                    f32 grow = 1.0f;
                    if (!TestForController()) {
                        const f32 elapsed = GlobalTimer.time_elapsed - (LastTouchTime + 2.0f);
                        if (elapsed > 4.0f) {
                            f32 wave = NU_SIN_LUT(static_cast<i32>(NuFmod(elapsed, 4.0f) * 0.25f * 65536.0f)) - 0.8f;
                            if (wave < 0.0f)
                                wave = 0.0f;
                            grow += wave;
                        }
                    }
                    DrawCharIcon(model, x, y, 0.0f, grow * size, 0xa6, opacity, opacity, 1, NULL);
                }
                if (model != -1 && progress > 0.0f) {
                    char name[64];
                    char *text = GameObj_GetName(model, NULL, name);
                    SmartTextEx(text, base_x, 0.475f, 1.0f, 0.45f, 0.45f, 0.45f, 1, 255, 191, 0, 1.25f, 2, NULL, 0,
                                static_cast<i32>(opacity * 128.0f * progress));
                }
            }
            GameMenu[menu_level].item_x[0] = x;
            GameMenu[menu_level].item_y[0] = y;
            GameMenu[menu_level].item_width[0] = size * 0.5f;
            GameMenu[menu_level].item_height[0] = 0.0f;
        }
    }
    if (freeplaymode == 0) {
        list[0].model_id = MenuPacket.player_model[0];
        list[1].model_id = -1;
        const f32 offset = (1.0f - NuFabs(COLLECTION_Y_HUB)) + 1.0f;
        const f32 y =
            (COLLECTION_Y_HUB + offset) * NU_SIN_LUT(static_cast<i32>((freeplaytime / freeplayduration) * 16384.0f)) -
            offset;
        Collection_Draw(collection, 0.0f, y, collection->field_10, list, 1.0f, 1);
        Hub_DrawIconCursors(collection, 1.0f);
        return;
    }
    if (freeplaymode == 1) {
        list[0].model_id = MenuPacket.player_model[0];
        list[1].model_id = -1;
        Collection_Draw(collection, 0.0f, COLLECTION_Y_HUB, collection->field_10, list, 1.0f, 1);
        Hub_DrawIconCursors(collection, 1.0f);
        i32 status = -1, select = -1, cancel = -1;
        if (freeplay_selected[0] == 0 && freeplay_time[0] <= 0.0f) {
            select = tSELECT;
            status = tSELECTING;
            cancel = tEXIT;
        } else if (freeplay_selected[0] == 1 && freeplay_time[0] <= 0.0f) {
            status = tSELECTED;
            select = tYES;
            cancel = tNO;
        } else if (freeplay_selected[0] >= 2 && freeplay_selected[0] < 4 && MenuPacket.active_player[1] != 0 &&
                   freeplay_selected[0] > freeplay_selected[1]) {
            status = tCONFIRMED;
            cancel = tCANCEL;
        }
        DrawPlayerIconPrompts(MenuPacket.active_player[0], select, 1.0f, -1, cancel, -1, status,
                              MenuPacket.active_player[1], 0, 1.0f, -1, 0, -1, 0);
        return;
    }
    if (freeplaymode != 3 && freeplaymode != 4) {
        if (freeplaymode == 2) {
            list[0].model_id = MenuPacket.player_model[0];
            i32 count = 1;
            for (i32 i = 0; i < fpcount; ++i)
                list[count++] = fplist[i];
            list[count].model_id = -1;
            f32 alpha = 0.0f;
            if (freeplaytime < freeplayduration - 0.1f)
                alpha = 1.0f - freeplaytime / (freeplayduration - 0.1f);
            Collection_Draw(collection, 0.0f, COLLECTION_Y_HUB, collection->field_10, list, alpha, 1);
            if (alpha >= 0.0f)
                Hub_DrawIconCursors(collection, alpha);
        }
        return;
    }
    list[0].model_id = MenuPacket.player_model[0];
    i32 count = 1;
    i16 second = MenuPacket.player_model[1];
    if (second == -1) {
        second = PlayerList[1];
        if (__builtin_expect(second == list[0].model_id, 0))
            second = -1;
    }
    if (second != -1)
        list[count++].model_id = second;
    list[count].model_id = -1;
    for (i32 i = 0; i < fpcount; ++i) {
        if (InModelList(list, fplist[i].model_id, NULL) == 0) {
            list[count++] = fplist[i];
            list[count].model_id = -1;
        }
    }
    const f32 progress = freeplaytime / freeplayduration;
    if (freeplaymode == 3) {
        const f32 alpha = freeplaytime >= 0.8f ? 0.0f : 1.0f - freeplaytime / 0.8f;
        Collection_Draw(collection, 0.0f, COLLECTION_Y_HUB, collection->field_10, list, alpha, 1);
    }
    f32 size = 0.35f;
    if (count > 8) {
        size = 0.35f - static_cast<f32>(count - 8) * 0.02f;
        if (size < 0.1f)
            size = 0.1f;
    }
    f32 dx = COLLECTION_DX * (size / COLLECTION_ICONSIZE);
    if (Game_OptionsSave != NULL && Game_OptionsSave->widescreen != 0)
        dx *= 0.75f;
    const f32 dy = 0.875f * (size / COLLECTION_ICONSIZE) * COLLECTION_DY * 0.5f;
    f32 x = -(static_cast<f32>(count - 1) * dx * 0.5f * 0.5f);
    for (i32 i = 0; i < count; ++i) {
        const f32 y = (i & 1) == 0 ? -dy : dy;
        f32 target_size = size;
        if (Game_OptionsSave != NULL && Game_OptionsSave->widescreen != 0)
            target_size *= 0.875f;
        if (freeplaymode == 3) {
            f32 source_x, source_y, source_size, opacity;
            i32 frame;
            if (i == 0) {
                source_x = -ICONX;
                source_y = STATSPOSY;
                source_size = COLLECTION_ICONSIZE;
                opacity = MenuPacket.active_player[0] ? 1.0f : DROPINALPHA;
                frame = 0xa6;
            } else {
                const i32 index = InCollectList_Index(list[i].model_id, collection->list, collection->count_y);
                if (index == -1) {
                    x += dx * 0.5f;
                    continue;
                }
                source_x = collection->list[index].grid_x;
                source_y = collection->list[index].grid_y;
                source_size = collection->field_10 * COLLECTION_ICONSIZE;
                if (__builtin_expect(Game_OptionsSave != NULL && Game_OptionsSave->widescreen != 0, 0))
                    source_size *= 0.875f;
                opacity = 1.0f;
                frame = 0xa7;
            }
            const f32 eased = 1.0f - (NU_SIN_LUT(static_cast<i32>(progress * 32768.0f + 16384.0f)) + 1.0f) * 0.5f;
            DrawCharIcon(list[i].model_id, (x - source_x) * eased + source_x, (y - source_y) * eased + source_y, 0.0f,
                         (target_size - source_size) * eased + source_size, frame, eased * (1.0f - opacity) + opacity,
                         eased * (1.0f - opacity) + opacity, 1, NULL);
        } else {
            const f32 offset = 1.0f - NU_SIN_LUT(static_cast<i32>(progress * 16384.0f + 16384.0f));
            DrawCharIcon(list[i].model_id, offset + offset + x, y, 0.0f, target_size, i == 0 ? 0xa6 : 0xa7, 1.0f, 1.0f,
                         1, NULL);
        }
        x += dx * 0.5f;
    }
}

void Hub_UpdateKit() {
    MENU *menu = &GameMenu[GameMenuLevel];
    i32 right = 0, left = 0, cancel = 0;
    if (hub_minikitviewer_movewait <= 0.0f) {
        for (i32 i = 0; i < 2; ++i) {
            if (MenuPacket.active_player[i]) {
                if ((GamePad[i].buttons_held & GAMEPAD_TOGGLELEFT) == 0) {
                    if ((GamePad[i].buttons_held & GAMEPAD_TOGGLERIGHT) != 0)
                        ++right;
                } else if ((GamePad[i].buttons_held & GAMEPAD_TOGGLERIGHT) == 0)
                    ++left;
                if ((GamePad[i].buttons_pressed & GAMEPAD_MENUCANCEL) != 0)
                    cancel = 1;
            }
        }
    }
    if (menu->input_activity) {
        if (menu->left_pressed)
            ++left;
        if (menu->right_pressed)
            ++right;
        if (menu->cancel_pressed)
            cancel = 1;
    }
    i32 previous = hub_minikitviewer_area;
    if (left && !right) {
        hub_minikitviewer_move = -1.0f;
        i32 index = previous;
        i32 attempts = 0;
        do {
            --index;
            // Original backward traversal wraps to AREACOUNT itself.
            if (index < 0)
                index = AREACOUNT;
            if (++attempts >= AREACOUNT)
                break;
        } while ((ADataList[index].flags & 0x10) == 0);
        hub_minikitviewer_area = index;
    } else if (right && !left) {
        hub_minikitviewer_move = 1.0f;
        i32 index = previous;
        i32 attempts = 0;
        do {
            ++index;
            if (index >= AREACOUNT)
                index = 0;
            if (++attempts >= AREACOUNT)
                break;
        } while ((ADataList[index].flags & 0x10) == 0);
        hub_minikitviewer_area = index;
    }
    if (hub_minikitviewer_area != previous) {
        hub_minikitviewer_movewait = 1.0f;
        hub_minikitviewer_alpha = 0.0f;
        PlaySfx(const_cast<char *>("JForcePush"), &hub_minikitviewer_pos);
    }
    if (cancel) {
        GameAudio_PlaySfx(49, NULL, 0, 0);
        MenuReset();
        hub_minikitarea = -1;
        hub_minikitarea_time = 0.0f;
        hub_minikitarea_opentime = 0.0f;
        LevLock[4] = 1;
        LevTime[4] = 0.6f;
    }
}

void Hub_Reset(WORLDINFO_s *world) {
    Customiser_Reset(CharacterCustomiser);
    if (Hub_ResetAIFn != NULL) {
        Hub_ResetAIFn(world);
    }

    for (i32 i = 20; i < 52; ++i) {
        if (((LevHSpecialExists >> i) & 1) != 0) {
            NuSpecialSetVisibility(&LevHSpecial[i], 1);
        }
        const i32 hidden = i + 32;
        if (((LevHSpecialExists >> hidden) & 1) != 0) {
            NuSpecialSetVisibility(&LevHSpecial[hidden], 0);
        }
    }

    if (NuSpecialExistsFn(&LevHSpecial[84]) != 0) {
        NuSpecialSetVisibility(&LevHSpecial[84], 0);
    }

    for (i32 i = 0; HubAreaInfo[i].area_name != NULL; ++i) {
        if (HubAreaInfo[i].door == NULL) {
            continue;
        }
        AREADATA_s *area = HubAreaInfo[i].area;
        i32 open = Episode_CountOpenAreas(static_cast<i8>(area->episode_index), area->index, Game_AreaSave);
        GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(HubAreaInfo[i].door->object);
        if (open != 0) {
            obstacle->runtime_flags &= static_cast<u8>(~GIZOBSTACLE_RUNTIME_FLAG_BLOCKED);
            NuSpecialSetVisibility(&HubAreaInfo[i].lock, 1);
        } else {
            obstacle->runtime_flags |= GIZOBSTACLE_RUNTIME_FLAG_BLOCKED;
            NuSpecialSetVisibility(&HubAreaInfo[i].lock, 0);
        }
    }

    for (i32 i = 0; static_cast<u16>(HubEpisodeInfo[i].episode) <= 8; ++i) {
        if (HubEpisodeInfo[i].door == NULL) {
            continue;
        }
        i32 open = Episode_CountOpenAreas(HubEpisodeInfo[i].episode, -1, Game_AreaSave);
        if (open == 0) {
            open = HubEpisodeInfo[i].force_open;
        }
        if (open == 0) {
            static_cast<GIZOBSTACLE_s *>(HubEpisodeInfo[i].door->object)->runtime_flags |=
                GIZOBSTACLE_RUNTIME_FLAG_BLOCKED;
            NuSpecialSetVisibility(&HubEpisodeInfo[i].lock_on, 0);
        } else {
            static_cast<GIZOBSTACLE_s *>(HubEpisodeInfo[i].door->object)->runtime_flags &=
                static_cast<u8>(~GIZOBSTACLE_RUNTIME_FLAG_BLOCKED);
            NuSpecialSetVisibility(&HubEpisodeInfo[i].lock_on, 1);
        }
    }
    if (Store_IsPackUnlocked(6) == 0) {
        NuSpecialSetVisibility(&HubEpisodeInfo[8].lock_on, 0);
    }

    hub_jabbaawake = 0.0f;
    buildits_reset = 0;
    Hub_HologramAlpha = 0.0f;
    Hub_HologramTargetAlpha = 0.0f;
    Hub_PreventDropOutTime = 0.0f;
}

void Hub_ClearStats() {
    statstime = 0.0f;
    cointotaltime = 0.0f;
    goldbricktime = 0.0f;
}

i32 Percent_UpdateHint(HINT_s *hint) {
    i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    if (hint->completion_flags[control_mode] != 0 || WORLD->current_level != HUB_LDATA || FadeSys.fade != 0.0f ||
        Game_CompletionSave == NULL || Game_CompletionSave[0] < COMPLETIONPOINTS)
        return 0;

    i32 active_players = 0;
    i32 nearby_players = 0;
    GameObject_s *object = Player[0];
    if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0) {
        active_players++;
        if (NuVecXZDistSqr(&object->apiobj.position, &PercentHint_TriggerPos, NULL) < 4.0f)
            nearby_players++;
    }
    object = Player[1];
    if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0) {
        active_players++;
        if (NuVecXZDistSqr(&object->apiobj.position, &PercentHint_TriggerPos, NULL) < 4.0f)
            nearby_players++;
    }
    if (active_players != nearby_players || active_players <= 0)
        return 0;
    hint->pad_0x05[0] = 4;
    return 1;
}

i32 IndyUnlocked_UpdateHint(HINT_s *hint) {
    i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    if (hint->completion_flags[control_mode] != 0 || WORLD->current_level != HUB_LDATA)
        return 0;
    if (FadeSys.fade != 0.0f || Game.indy_unlocked == 0)
        return 0;
    hint->pad_0x05[0] = 4;
    return 1;
}

bool HubShopUnlocked() {
    return true;
}

bool HubCustomiserUnlocked() {
    return true;
}

bool HubMinikitViewerUnlocked() {
    return true;
}

void Hub_Draw3D(WORLDINFO_s *world) {
    Store_HubDrawFloorTargets(world);

    NUMTX sign_mtx;
    for (i32 episode = 0; episode < 6; episode++) {
        nuhspecial_s *special = &HubAreaInfo[episode * 7 + 6].lock;
        if (NuSpecialExistsFn(special) == 0) {
            continue;
        }
        sign_mtx = *NuSpecialGetDrawMtx(special);
        NUVEC direction;
        memcpy(&direction, reinterpret_cast<const byte *>(&sign_mtx) + offsetof(NUMTX, m20), sizeof(direction));
        NuMtxSetTranslation(&sign_mtx, const_cast<NUVEC *>(&ClipsSignOffset));
        NuMtxRotateY(&sign_mtx, NuAtan2D(direction.x, direction.z) + NUANG_90DEG);
        NuMtxTranslate(&sign_mtx, NuSpecialGetDrawPos(special));
        Draw3DObjectMtx(world, 316, &sign_mtx);
        Draw3DObjectMtx(world, Episode_IsComplete(&EDataList[episode], NULL) != 0 ? 320 : 319, &sign_mtx);
    }

    Customiser_Draw3D(CharacterCustomiser);
    DrawShop3D(world);
    Hub_DrawMiniKits(world);

    if (GameCam->sock_position.location.sock == 0) {
        if (Store_IsPackUnlocked(6) == 0) {
            if (NuSpecialExistsFn(&LevHSpecial[16]) != 0 && NuSpecialExistsFn(&LevHSpecial[4]) != 0) {
                NUMTX draw_mtx = *NuSpecialGetDrawMtx(&LevHSpecial[16]);
                NuSpecialDrawAt(&LevHSpecial[4], &draw_mtx);
            }
        }

        nuhspecial_s *display = &LevHSpecial[Store_IsPackUnlocked(5) != 0 ? 5 : 4];
        if (NuSpecialExistsFn(display) != 0) {
            NuSpecialDrawAt(display, &LevMtx);
        }
    }

    if (QFont3DZ != NULL) {
        const i32 spin = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 5.0f) / 5.0f * 65536.0f);
        const u16 angle = static_cast<u16>(NU_SIN_LUT(spin) * 2730.0f + 8192.0f);
        const i32 pulse = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 3.21f) / 3.21f * 65536.0f);
        const f32 bob = NU_SIN_LUT(pulse) * 0.01f;
        const i32 alpha = static_cast<i32>(Hub_HologramAlpha * 15.0f + 56.0f);

        char text[128];
        sprintf(text, "%.1f%%", static_cast<f32>(Game.completion * 100) / COMPLETIONPOINTS);
        Text_LocaliseDecimalPoint(text);

        NUMTX text_mtx;
        Hub_SetStatsTextMtx(&text_mtx, angle);
        NuMtxTranslate(&text_mtx, const_cast<NUVEC *>(&Hub_PercentPos));
        text_mtx.m31 += bob;

        NuQFntPushPrintMode(NUQFNT_CSMODE_ABSOLUTE);
        NuQFntSet(QFont3DZ);
        NuQFntSetMtx(QFont3DZ, &text_mtx);
        NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
        const u32 colour = (static_cast<u32>(alpha) << 24) | 0xff7f00;
        NuQFntSetColour(QFont3DZ, colour);
        NuQFntSetScale(QFont3DZ, 0.00375f, 0.005f);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenU(QFont3DZ, text) * 0.5f, 0.0f, 0.0f);
        NuQFntPrintU(QFont3DZ, text);
        NuQFntPopPrintMode();

        Text_MakeTime(Game.field30_0x7c2c, 1, 1, 1, text);
        Text_LocaliseDecimalPoint(text);
        Hub_SetStatsTextMtx(&text_mtx, angle);
        NuMtxTranslate(&text_mtx, const_cast<NUVEC *>(&Hub_PercentPos));
        text_mtx.m31 += bob;
        text_mtx.m31 += 0.16f;

        NuQFntPushPrintMode(NUQFNT_CSMODE_ABSOLUTE);
        NuQFntSet(QFont3DZ);
        NuQFntSetMtx(QFont3DZ, &text_mtx);
        NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
        NuQFntSetColour(QFont3DZ, colour);
        NuQFntSetScale(QFont3DZ, 0.0028124998f, 0.00375f);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenU(QFont3DZ, text) * 0.5f, 0.0f, 0.0f);
        NuQFntPrintU(QFont3DZ, text);
        NuQFntPopPrintMode();

        if (NuSpecialExistsFn(&LevHSpecial[15]) != 0) {
            NUVEC position = *NuSpecialGetDrawPos(&LevHSpecial[15]);
            Hub_SetStatsTextMtx(&text_mtx, angle);
            NuMtxTranslate(&text_mtx, &position);
            NuSpecialDrawAtAlpha(&LevHSpecial[15], &text_mtx, Hub_HologramAlpha * 0.2f + 0.8f);
        }
    }
}

i32 Hub_InMenu() {
    const i32 menu = GetMenuID();
    if (menu == 20)
        return 1;
    if (menu == 13)
        return 1;
    if (menu == 21 || menu == 22)
        return 1;
    if (menu == 12)
        return 1;
    if (menu == 23)
        return 1;
    if (menu == 14)
        return 1;
    if (menu >= 15 && menu <= 19)
        return 1;
    if (menu == 8) {
        return 1;
    }
    return 0;
}

void Hub_DrawPanel(WORLDINFO_s *) {
    char text[256];
    char title[128];
    if (newgamecam) {
        f32 alpha = 0.0f;
        if (newgamecamtime < 2.0f)
            alpha = 0.0f;
        else if (newgamecamtime < 2.5f)
            alpha = (newgamecamtime - 2.0f) * 2.0f;
        else if (newgamecamtime < 7.5f)
            alpha = 1.0f;
        else if (newgamecamtime < 8.0f)
            alpha = 1.0f - (newgamecamtime - 7.5f) * 2.0f;
        if (alpha > 0.0f)
            SmartTextEx(TTab[tMAP], 0.0f, (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f, 1.0f, 0.8f, 0.8f, 0.8f, 0,
                        HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0, static_cast<i32>(alpha * 128.0f));
    }
    DrawShopPanel();

    if (hub_episode != -1 && hub_episode_time > 0.0f) {
        const i32 alpha = static_cast<i32>(128.0f * hub_episode_time);
        if (hub_episode == 6) {
            i32 red_total = 0, red_count = 0, free_total = 0, free_count = 0;
            i32 story_total = 0, story_count = 0, buildup_total = 0, buildup_count = 0;
            i32 char_total = 0, char_count = 0, mini_total = 0, mini_count = 0;
            i32 both_true_jedi_bricks;
            i32 i = 0;
            for (; i < EPISODECOUNT; ++i) {
                Episode_CountOpenAreas(i, -1, Game.area_save);
                mini_count += EpMiniKitCount;
                mini_total += EpMiniKitTotal;
                char_count += EpCharKitCount;
                char_total += EpCharKitTotal;
                buildup_count += EpBuildUpCount;
                buildup_total += EpBuildUpTotal;
                both_true_jedi_bricks = BOTHTRUEJEDIGOLDBRICKS;
                if (__builtin_expect(!!(both_true_jedi_bricks), 1)) {
                    story_total += EpStoryBuildUpTotal;
                    story_count += EpStoryBuildUpCount;
                    free_count += EpFreePlayBuildUpCount;
                    free_total += EpFreePlayBuildUpTotal;
                }
                red_count += EpRedBrickCount;
                red_total += EpRedBrickTotal;
            }
            if (i == 0)
                both_true_jedi_bricks = BOTHTRUEJEDIGOLDBRICKS;
            stats_xscale = 0.8f;
            if (both_true_jedi_bricks) {
                DrawBuildUpBar(HUB_AREAPANELX[1], HUB_EPISODESUBTITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                               NU_SIN_LUT(static_cast<i32>(16384.0f * hub_episode_time)), 1.0f, 1.0f, 0);
                sprintf(text, "%i/%i", story_count, story_total);
                Text3DEx(text, HUB_AREAPANELX[1], HUB_EPISODESUBTITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE * stats_xscale,
                         PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
                SmartTextEx(TTab[tSTORY], HUB_AREAPANELX[1], 0.235f + HUB_EPISODESUBTITLEY, 1.0f, 0.45f, 0.45f, 0.45f,
                            0, 255, 255, 255, 0.4f, 1, NULL, 0,
                            static_cast<i32>(static_cast<f32>(TJTYPEA) * hub_episode_time));
                DrawBuildUpBar(HUB_AREAPANELX[4], HUB_EPISODESUBTITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                               NU_SIN_LUT(static_cast<i32>(16384.0f * hub_episode_time)), 1.0f, 1.0f, 0);
                sprintf(text, "%i/%i", free_count, free_total);
                Text3DEx(text, HUB_AREAPANELX[4], HUB_EPISODESUBTITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE * stats_xscale,
                         PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
                SmartTextEx(TTab[tFREEPLAY], HUB_AREAPANELX[4], 0.235f + HUB_EPISODESUBTITLEY, 1.0f, 0.45f, 0.45f,
                            0.45f, 0, 255, 255, 255, 0.4f, 1, NULL, 0,
                            static_cast<i32>(static_cast<f32>(TJTYPEA) * hub_episode_time));
            } else {
                DrawBuildUpBar(HUB_AREAPANELX[1], HUB_EPISODESUBTITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                               NU_SIN_LUT(static_cast<i32>(16384.0f * hub_episode_time)), 1.0f, 1.0f, 0);
                sprintf(text, "%i/%i", buildup_count, buildup_total);
                Text3DEx(text, HUB_AREAPANELX[1], HUB_EPISODESUBTITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE * stats_xscale,
                         PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
            }
            Hub_DrawMiniKitCount(HUB_AREAPANELX[0], HUB_EPISODESUBTITLEY, mini_count, mini_total, hub_episode_time);
            Hub_DrawImportantBrick(210, HUB_AREAPANELX[2], HUB_EPISODESUBTITLEY, hub_episode_time, red_count,
                                   red_total);
            Hub_DrawImportantBrick(211, HUB_AREAPANELX[3], HUB_EPISODESUBTITLEY, hub_episode_time, Game.gold_bricks,
                                   GOLDBRICKPOINTS);
            hub_drawminikitcount_charkit = 1;
            Hub_DrawMiniKitCount(HUB_AREAPANELX[5], HUB_EPISODESUBTITLEY, char_count, char_total, hub_episode_time);
            stats_xscale = 1.0f;
        } else if (hub_episode == 7) {
            if (MissionSys) {
                const i32 total = MissionSys->count;
                const i32 count = Missions_NumCompleted(MissionSys, &Game.mission_save, 0);
                Hub_DrawImportantBrick(211, 0.0f, HUB_EPISODETITLEY, hub_episode_time, count, total);
                SmartTextEx(TTab[tBOUNTYHUNTERMISSIONS2 ? tBOUNTYHUNTERMISSIONS2 : tBOUNTYHUNTERMISSIONS], 0.0f,
                            HUB_EPISODESUBTITLEY, 1.0f, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE,
                            HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0,
                            alpha);
            }
        } else if (hub_episode == 8) {
            i32 gold_count = 0, gold_total = 0, buildup_count = 0, buildup_total = 0;
            for (i32 i = 0; i < AREACOUNT; ++i) {
                AREADATA *area = &ADataList[i];
                AREASAVE_s *save = &Game.area_save[i];
                if (area == HUB_ADATA || (area->flags & 0x22) || area->episode_index != 0xff || (area->flags & 0x2010))
                    continue;
                if (area->flags & 0x100) {
                    if (GOLDBRICKFORSUPERBONUS) {
                        ++gold_total;
                        if (save->area_complete)
                            ++gold_count;
                    }
                    continue;
                }
                if (area->flags & 4 || area->flags & 0x800)
                    continue;
                ++gold_total;
                if (save->area_complete)
                    ++gold_count;
                if (area->flags & 0x4000) {
                    ++gold_total;
                    ++buildup_total;
                    if (save->true_hero_complete[0] || save->true_hero_complete[1]) {
                        ++gold_count;
                        ++buildup_count;
                    }
                }
            }
            if (__builtin_expect(!buildup_total, 0)) {
                f32 x = 0.0f;
                if (Game.indy_unlocked) {
                    Hub_DrawImportantBrick(251, 0.1f, HUB_EPISODETITLEY - 0.225f, hub_episode_time, -1, -1);
                    x = -0.1f;
                }
                Hub_DrawImportantBrick(211, x, HUB_EPISODETITLEY, hub_episode_time, gold_count, gold_total);
            } else {
                Hub_DrawImportantBrick(211, -0.20100002f, HUB_EPISODETITLEY, hub_episode_time, gold_count, gold_total);
                DrawBuildUpBar(0.20100002f, HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100, 100,
                               NU_SIN_LUT(static_cast<i32>(16384.0f * hub_episode_time)), 1.0f, 1.0f, 0);
                sprintf(text, "%i/%i", buildup_count, buildup_total);
                Text3DEx(text, 0.20100002f, HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE,
                         PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127,
                         static_cast<u8>(static_cast<i32>(128.0f * hub_episode_time)));
            }
            SmartTextEx(TTab[tBONUS2], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, HUB_EPISODESUBTITLESIZE,
                        HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB,
                        1.7f, 1, NULL, 0, alpha);
        } else if (hub_episode >= 0 && hub_episode < EPISODECOUNT) {
            EPISODEDATA *episode = &EDataList[hub_episode];
            const f32 phase = hub_episode_time;
            Episode_CountOpenAreas(hub_episode, -1, Game.area_save);
            if (EpCompleteCount) {
                if (BOTHTRUEJEDIGOLDBRICKS) {
                    DrawBuildUpBar(HUB_AREAPANELX[1], HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100,
                                   100, NU_SIN_LUT(static_cast<i32>(16384.0f * phase)), 1.0f, 1.0f, 0);
                    sprintf(text, "%i/%i", EpStoryBuildUpCount, EpStoryBuildUpTotal);
                    Text3DEx(text, HUB_AREAPANELX[1], HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE,
                             PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
                    SmartTextEx(TTab[tSTORY], HUB_AREAPANELX[1], 0.235f + HUB_EPISODETITLEY, 1.0f, 0.45f, 0.45f, 0.45f,
                                0, 255, 255, 255, 0.4f, 1, NULL, 0,
                                static_cast<i32>(static_cast<f32>(TJTYPEA) * phase));
                    DrawBuildUpBar(HUB_AREAPANELX[4], HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100,
                                   100, NU_SIN_LUT(static_cast<i32>(16384.0f * phase)), 1.0f, 1.0f, 0);
                    sprintf(text, "%i/%i", EpFreePlayBuildUpCount, EpFreePlayBuildUpTotal);
                    Text3DEx(text, HUB_AREAPANELX[4], HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE,
                             PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
                    SmartTextEx(TTab[tFREEPLAY], HUB_AREAPANELX[4], 0.235f + HUB_EPISODETITLEY, 1.0f, 0.45f, 0.45f,
                                0.45f, 0, 255, 255, 255, 0.4f, 1, NULL, 0,
                                static_cast<i32>(static_cast<f32>(TJTYPEA) * phase));
                } else {
                    DrawBuildUpBar(HUB_AREAPANELX[1], HUB_EPISODETITLEY + PANEL_MINIKITY - PANEL_MINIKITCOUNTY, 100,
                                   100, NU_SIN_LUT(static_cast<i32>(16384.0f * phase)), 1.0f, 1.0f, 0);
                    sprintf(text, "%i/%i", EpBuildUpCount, EpBuildUpTotal);
                    Text3DEx(text, HUB_AREAPANELX[1], HUB_EPISODETITLEY, 1.0f, PANEL_MINIKITCOUNTSCALE,
                             PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTSCALE, 0, 255, 0, 127, static_cast<u8>(alpha));
                }
                Hub_DrawMiniKitCount(HUB_AREAPANELX[0], HUB_EPISODETITLEY, EpMiniKitCount, EpMiniKitTotal, phase);
                Hub_DrawImportantBrick(210, HUB_AREAPANELX[2], HUB_EPISODETITLEY, phase, EpRedBrickCount,
                                       EpRedBrickTotal);
                Hub_DrawImportantBrick(211, HUB_AREAPANELX[3], HUB_EPISODETITLEY, phase, EpGoldBrickCount,
                                       EpGoldBrickTotal);
                if (__builtin_expect(!!(Store_IsPackUnlocked(8)), 0)) {
                    hub_drawminikitcount_charkit = 1;
                    Hub_DrawMiniKitCount(HUB_AREAPANELX[5], HUB_EPISODETITLEY, EpCharKitCount, EpCharKitTotal, phase);
                }
            } else if (episode->name_id != -1) {
                SmartTextEx(TTab[episode->name_id], 0.0f, HUB_EPISODETITLEY, 1.0f, HUB_EPISODETITLESIZE,
                            HUB_EPISODETITLESIZE, HUB_EPISODETITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG, HUB_EPISODEB,
                            1.7f, 1, NULL, 0, alpha);
            }
            if (__builtin_expect(episode->text_id != -1, 0))
                SmartTextEx(TTab[episode->text_id], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, HUB_EPISODESUBTITLESIZE,
                            HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER, HUB_EPISODEG,
                            HUB_EPISODEB, 1.7f, 1, NULL, 0, alpha);
        }
    }
    if (hub_area != -1 && hub_area_time > 0.0f) {
        if (GameCam->sock_position.location.sock == 7 && (ADataList[hub_area].flags & 5) == 5) {
            i32 count = 0, total = 0;
            if (Game_AreaSave) {
                for (i32 i = 0; i < AREACOUNT; ++i) {
                    if (__builtin_expect((ADataList[i].flags & 5) == 5, 0)) {
                        ++total;
                        if (Game_AreaSave[i].area_complete ||
                            static_cast<f32>(static_cast<i32>(ADataList[i].challenge_trial_time)) >
                                Game_AreaSave[i].challenge_trial_time)
                            ++count;
                    }
                }
            }
            Hub_DrawImportantBrick(211, 0.0f, HUB_EPISODETITLEY, hub_area_time, count, total);
            SmartTextEx(TTab[tMINIKITS ? tMINIKITS : tMINIKIT], 0.0f, HUB_EPISODESUBTITLEY, 1.0f,
                        HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER,
                        HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0, static_cast<i32>(128.0f * hub_area_time));
        } else {
            AREADATA *area = &ADataList[hub_area];
            if (area->flags & 0x100) {
                Hub_DrawSuperBonusStats(area, hub_area_time);
            } else if (area->flags & 4) {
                if (SENATE_ADATA && SENATE_ADATA->index == hub_area)
                    Hub_DrawArcadeStats(hub_area_time);
                else
                    Hub_DrawBonusStats(hub_area_time, hub_area, -1);
            } else {
                Hub_DrawAreaStats(hub_area_time, hub_area, -1);
            }
        }
    }
    if (GetMenuID() == 14) {
        const f32 phase = hub_minikitviewer_alpha;
        const i32 index = hub_minikitviewer_area;
        AREADATA *area = &ADataList[index];
        AREASAVE_s *save = &Game.area_save[index];
        const i32 alpha = static_cast<i32>(phase * 128.0f);
        if (save->minikit_complete) {
            const f32 scale = HUB_EPISODETITLESIZE;
            if (area->minikit_id != -1 && CDataList[area->minikit_id].name_id != -1)
                SmartTextEx(TTab[CDataList[area->minikit_id].name_id], 0.0f, HUB_EPISODETITLEY, 1.0f,
                            HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, 0, HUB_EPISODER,
                            HUB_EPISODEG, HUB_EPISODEB, 1.7f, 1, NULL, 0, alpha);
            sprintf(title, "(%s)", TTab[area->name_id]);
            if (scale > 0.0f)
                SmartTextEx(title, 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, HUB_EPISODER, HUB_EPISODEG,
                            HUB_EPISODEB, 1.7f, 1, NULL, 0, alpha);
        } else if (save->minikit_count) {
            const f32 scale = HUB_EPISODESUBTITLESIZE;
            Hub_DrawMiniKitCount(0.0f, HUB_EPISODETITLEY, save->minikit_count, 10, phase);
            char *name = TTab[area->name_id];
            if (scale > 0.0f && name)
                SmartTextEx(name, 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, HUB_EPISODER, HUB_EPISODEG,
                            HUB_EPISODEB, 1.7f, 1, NULL, 0, alpha);
        } else if (area->area_index != 0xff && area->episode_index != 0xff) {
            const f32 scale = HUB_EPISODESUBTITLESIZE;
            const i8 episode = area->episode_index;
            Hub_DrawMiniKitCount(0.0f, HUB_EPISODETITLEY, 0, 10, phase);
            sprintf(title, "%s, %s %i", TTab[EDataList[episode].name_id], TTab[tCHAPTER],
                    static_cast<i8>(ADataList[index].area_index) + 1);
            if (scale > 0.0f)
                SmartTextEx(title, 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, HUB_EPISODER, HUB_EPISODEG,
                            HUB_EPISODEB, 1.7f, 1, NULL, 0, alpha);
        }
        MENU *menu = &GameMenu[GameMenuLevel];
        if (!Game_AreaSave || Game_AreaSave[index].minikit_complete) {
            NUVEC minimum, maximum;
            NuSpecialGetBounds(&WORLD->lev_objs[167].special, &minimum, &maximum);
            const f32 icon_size = ICONSIZE;
            const f32 text_scale = (icon_size / 0.35f) * 0.95f;
            const f32 spacing = 1.2f * (icon_size * (maximum.x - minimum.x)) / PANEL3DMULX;
            f32 x = -spacing * 2.5f;
            const f32 y = HUB_EPISODETITLEY + (HUB_EPISODETITLEY - HUB_EPISODESUBTITLEY) * 1.5f;
            f32 grow = 1.0f;
            if (!TestForController()) {
                const f32 elapsed = GlobalTimer.time_elapsed - (LastTouchTime + 2.0f);
                if (elapsed > 4.0f) {
                    f32 wave = NU_SIN_LUT(static_cast<i32>((NuFmod(elapsed, 4.0f) * 0.25f) * 65536.0f)) - 0.8f;
                    if (wave < 0.0f)
                        wave = 0.0f;
                    grow = wave + 1.0f;
                }
            }
            for (i32 i = 0; i < 6; ++i) {
                i32 red, green, blue;
                f32 size = 1.0f;
                if (i == i_selectminikitepisode) {
                    size = 1.2f;
                    if (TestForController()) {
                        if (menu_pulsate > 0.0f) {
                            red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                                   static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                            green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                                     static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                            blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                                    static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
                        } else {
                            red = menu_flash ? MENUFLASH0R : MENUFLASH1R;
                            green = menu_flash ? MENUFLASH0G : MENUFLASH1G;
                            blue = menu_flash ? MENUFLASH0B : MENUFLASH1B;
                        }
                        goto hub_episode_colour_ready;
                    }
                }
                // Touch selection and the other entries share the normal menu pulse.
                if (menu_pulse > 0.0f) {
                    red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                           static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
                    green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                             static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
                    blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                            static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
                } else {
                    red = MENUENTRYR;
                    green = MENUENTRYG;
                    blue = MENUENTRYB;
                }
            hub_episode_colour_ready:
                size *= grow;
                f32 opacity = phase;
                if (!Episode_CountOpenAreas(i, -1, Game_AreaSave))
                    opacity *= 0.25f;
                Text3DEx(EpisodeNumerals[i], x, y, 1.0f, text_scale * size, text_scale * size, text_scale * size, 0,
                         red, green, blue, static_cast<u8>(static_cast<i32>(128.0f * opacity)));
                const i32 bonus = Episode_FindAreaFromFlags(&EDataList[i], 5, 5);
                i32 object = 167;
                if (bonus != -1 && Game_AreaSave &&
                    (Game_AreaSave[bonus].area_complete ||
                     static_cast<f32>(static_cast<i32>(ADataList[bonus].challenge_trial_time)) >
                         Game_AreaSave[bonus].challenge_trial_time))
                    object = 168;
                const f32 scale = icon_size * size;
                DrawPanel3DObject(x, y, 1.0f, scale, scale, scale, 0, 0, 0, &WORLD->lev_objs[object].special, 0,
                                  opacity);
                menu->item_x[i] = x;
                menu->item_y[i] = y;
                menu->item_width[i] = scale * 0.5f;
                menu->item_height[i] = 0.0f;
                x += spacing;
            }
        } else {
            for (i32 i = 0; i < 6; ++i)
                menu->item_width[i] = 0.0f;
        }
    }
    if (hub_buildit != -1 && hub_buildit_time > 0.0f) {
        i32 total;
        if (hub_buildit == 999)
            total = LevBuildIt[0]->anim_object_count * 2;
        else {
            GIZMO *gizmo = HubAreaInfo[hub_buildit].bonus_gizmo;
            if (!gizmo)
                return;
            total = static_cast<GIZBUILDIT_s *>(gizmo->object)->anim_object_count;
        }
        const i32 count = total > Game.gold_bricks ? Game.gold_bricks : total;
        Hub_DrawImportantBrick(211, 0.0f, HUB_EPISODESUBTITLEY, hub_buildit_time, count, total);
    }
}

void Hub_ResetPanel() {
    hub_episode = -1;
    hub_episode_time = 0.0f;
    hub_area = -1;
    hub_area_time = 0.0f;
    hub_buildit = -1;
    hub_buildit_time = 0.0f;
    hub_minikitarea = -1;
    hub_minikitarea_time = 0.0f;
}

void Hub_Init(WORLDINFO_s *world) {
    static NUVEC arcadelightpos = {-22.49f, 0.79f, -49.9975f};

    Store_RestorePurchases();
    Hub_LowEnd_IconsInsteadOfModels = 0;

    if (BOTHTRUEJEDIGOLDBRICKS != 0) {
        HUB_AREAPANELX = HUB_AREAPANELX_TWOTRUEJEDIGOLDBRICKS;
    } else if (Store_IsPackUnlocked(8) == 0) {
        HUB_AREAPANELX = HUB_AREAPANELX_1TRUEJEDIGB_NOCHALLENGE;
    } else {
        HUB_AREAPANELX = HUB_AREAPANELX_ONETRUEJEDIGOLDBRICK;
    }

    for (HUBAREAINFO_s *info = HubAreaInfo; info->area_name != NULL; ++info) {
        info->door = GizmoFindByName(world->gizmo_sys, -1, const_cast<char *>(info->door_name));
        info->bonus_gizmo = GizmoFindByName(world->gizmo_sys, -1, const_cast<char *>(info->bonus_gizmo_name));
        info->bonus_gizmo_2 = GizmoFindByName(world->gizmo_sys, -1, const_cast<char *>(info->bonus_gizmo_name_2));
        info->area = Area_FindByName(const_cast<char *>(info->area_name), NULL);
        Hub_FindSpecial(world, &info->lock, info->lock_name);
    }

    for (HUBEPISODEINFO_s *info = HubEpisodeInfo; info->episode != -1; ++info) {
        info->data = info->episode >= 0 && info->episode < EPISODECOUNT ? &EDataList[info->episode] : NULL;
        info->door = GizmoFindByName(world->gizmo_sys, -1, const_cast<char *>(info->door_name));
        Hub_FindSpecial(world, &info->lock_on, info->lock_on_name);
        Hub_FindSpecial(world, &info->lock_off, info->lock_off_name);
    }

    hub_minikitviewer_gizmo =
        GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, const_cast<char *>("conveyor_test"));
    hub_minikitviewer_camspl = NuSplineFind(world->current_gscn, const_cast<char *>("mini_cam_e1_1"));
    LevGizObst[7] = GizObstacle_FindByName(world->giz_obstacle_sys, const_cast<char *>("network_door"));
    LevGizObst[6] = GizObstacle_FindByName(world->giz_obstacle_sys, const_cast<char *>("DE7"));

    static const char *const lock_names[] = {
        "lock_4_off", "lock_4_on",    "lock_5_off",  "lock_5_on",    "lock_6_off",  "lock_6_on",    "lock_b_off",
        "lock_b_on",  "lock_4_7_off", "lock_4_7_on", "lock_5_7_off", "lock_5_7_on", "lock_6_7_off", "lock_6_7_on",
    };
    for (i32 i = 0; i < 14; ++i) {
        Hub_FindSpecial(world, &LevHSpecial[i], lock_names[i]);
    }

    if (NuSpecialFind(world->current_gscn, &LevHSpecial[15], const_cast<char *>("holo"), 1) != 0) {
        NuSpecialSetVisibility(&LevHSpecial[15], 0);
    }
    if (NuSpecialFind(world->current_gscn, &LevHSpecial[16], const_cast<char *>("lock_7_on"), 1) != 0 &&
        Store_IsPackUnlocked(6) == 0) {
        NuSpecialSetVisibility(&LevHSpecial[16], 0);
    }
    NuSpecialFindMulti(world->current_gscn, &LevHSpecial[20], const_cast<char *>("ps2_only_"), 32, 0);
    NuSpecialFindMulti(world->current_gscn, &LevHSpecial[52], const_cast<char *>("psp_only_"), 32, 0);
    Hub_FindSpecial(world, &LevHSpecial[84], "fake_wall");

    Hub_ResetPanel();
    InitShop(world);
    Hub_LockUnlockDoors(world);
    Hub_InitMiniKits(world);
    LevTime[0] = 3.0f;
    LevTime[1] = 3.0f;
    Hub_HologramAlpha = 0.0f;
    Hub_HologramTargetAlpha = 0.0f;
    Customiser_Init(CharacterCustomiser);
    if (Hub_InitAIFn != NULL) {
        Hub_InitAIFn(world);
    }

    hub_startoutsidebonusdoor_area = -1;
    CutScenePlayer_Reset();
    LevBuildIt[0] = GizBuildIt_Find(world, const_cast<char *>("fountain"));
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, -1, const_cast<char *>("fountain"));
    if (LevBuildIt[0] == NULL || LevGizmo[0] == NULL) {
        LevBuildIt[0] = NULL;
        LevGizmo[0] = NULL;
    }
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, -1, const_cast<char *>("garageLever"));
    Arcade = 0;
    NuMtxSetRotationZ(&LevMtx, 0x4000);
    NuMtxRotateY(&LevMtx, 0xf600);
    NuMtxTranslate(&LevMtx, &arcadelightpos);
    Store_HubInitFloorTargets(world);
    hub_custodians_finished_loading = 0;
}

void CutScenePlayer_DrawGrid(COLLECTION_s *collection, i16 *ids, float x, float y, i32 selected, float alpha) {
    if (FadeSys.fade > 0.0f || !WORLD->lev_objs[167].active)
        return;
    if (alpha > 1.0f)
        alpha = 1.0f;
    const i32 rows = collection->count_y / collection->count_x;
    const f32 scale = ICONSIZE * 1.2f;
    NUVEC minimum, maximum;
    NuSpecialGetBounds(&WORLD->lev_objs[167].special, &minimum, &maximum);
    const f32 dx = (maximum.x - minimum.x) * scale / PANEL3DMULX;
    const f32 dy = -(maximum.y - minimum.y) * scale / PANEL3DMULY;
    y -= rows * dy * 0.5f;
    const f32 first_x = x - (collection->count_x - 1) * dx * 0.5f;
    CUTSCENEPLAYER_s *player = static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available());
    MENU *menu = &GameMenu[GameMenuLevel];
    i32 previous_area = -1, colour = -1;
    static const u8 colours[8][3] = {{255, 63, 0},  {255, 127, 0},  {255, 223, 0},  {0, 255, 63},
                                     {0, 127, 255}, {191, 31, 255}, {255, 31, 191}, {255, 255, 255}};
    for (i32 row = 0; row <= rows; ++row, y += dy) {
        x = first_x;
        for (i32 column = 0; column < collection->count_x; ++column, x += dx) {
            const i32 i = row * collection->count_x + column;
            if (i >= collection->count_y)
                continue;
            COLLECTID *item = &collection->list[i];
            item->grid_y = y;
            item->grid_x = x;
            if (__builtin_expect(alpha <= 0.0f, 0))
                continue;
            i32 object = i == selected ? (menu_flash ? 168 : 166) : 167;
            f32 opacity = CutScenePlayer_CanStart(ids[i]) ? alpha : 0.375f * alpha;
            DrawPanel3DObject(x, y, 1.0f, scale, scale, scale, 0, 0, 0, &WORLD->lev_objs[object].special, 0, opacity);
            menu->item_x[i] = x;
            menu->item_y[i] = y;
            menu->item_width[i] = scale * 0.5f;
            menu->item_height[i] = 0.0f;
            char text[32];
            sprintf(text, "%i", i + 1);
            i32 area = LDataList[player->clips[ids[i]].level_id].area_index;
            if (__builtin_expect(area != previous_area, 1)) {
                previous_area = area;
                if (++colour > 7)
                    colour = 7;
            }
            const f32 text_scale = scale * 3.5f;
            Text3DEx(text, x, y, 1.0f, text_scale * 0.925f, text_scale, text_scale, 0, colours[colour][0],
                     colours[colour][1], colours[colour][2], static_cast<u8>(static_cast<i32>(opacity * 128.0f)));
        }
    }
}

// The original private draw helper shares EpisodeNumerals with the hub menus.
static void DrawEpisodesMenu(i32 selected, f32 alpha) {
    const i32 menu_level = GameMenuLevel;
    MENU *menu = &GameMenu[menu_level];
    if (episodesmode != 0) {
        if (episodesmode == 1) {
            if (hub_new_level != -1 && LDataList[hub_new_level].episode_index != -1)
                goto draw_clips;
        } else if (episodesmode == 2) {
            goto draw_clips;
        } else if (episodesmode == 3) {
            goto draw_play;
        } else {
            return;
        }
    }
    {
        NUVEC minimum, maximum;
        NuSpecialGetBounds(&WORLD->lev_objs[167].special, &minimum, &maximum);
        f32 dx = (maximum.x - minimum.x) * 0.35f;
        f32 dy = -(maximum.y - minimum.y) * 0.35f;
        dx /= PANEL3DMULX;
        dy /= PANEL3DMULY;
        f32 y = -dy * 0.5f;
        const f32 first_x = -dx;
        for (i32 row = 0; row < 2; ++row, y += dy) {
            f32 x = first_x;
            for (i32 column = 0; column < 3; ++column, x += dx) {
                const i32 episode = row * 3 + column;
                u32 red, green, blue;
                if (selected && episode == i_episodes && TestForController()) {
                    if (menu_pulsate > 0.0f) {
                        red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                               static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                        blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                                static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
                        green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                                 static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                    } else {
                        red = menu_flash ? MENUFLASH0R : MENUFLASH1R;
                        green = menu_flash ? MENUFLASH0G : MENUFLASH1G;
                        blue = menu_flash ? MENUFLASH0B : MENUFLASH1B;
                    }
                } else if (menu_pulse > 0.0f) {
                    red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                           static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
                    green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                             static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
                    blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                            static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
                } else {
                    red = MENUENTRYR;
                    green = MENUENTRYG;
                    blue = MENUENTRYB;
                }
                f32 opacity = 1.0f;
                if (Game_AreaSave != NULL && Game_AreaSave[EDataList[episode].area_ids[0]].complete == 0)
                    opacity = 0.375f;
                opacity = opacity * alpha;
                if (episodesmode != 1 && episodestime < episodesduration)
                    opacity = opacity * (episodestime / episodesduration);
                Text3DEx(EpisodeNumerals[episode], x, y, 1.0f, 0.95f, 0.95f, 0.95f, 0, red, green, blue,
                         static_cast<i32>(128.0f * opacity));
                DrawPanel3DObject(x, y, 1.0f, 0.35f, 0.35f, 0.35f, 0, 0, 0, &WORLD->lev_objs[167].special, 0, opacity);
                menu->item_x[episode] = x;
                menu->item_y[episode] = y;
                menu->item_width[episode] = 0.175f;
                menu->item_height[episode] = 0.0f;
            }
        }
        memset(GameMenu[menu_level].item_width + 6, 0, 394 * sizeof(f32));
        memset(GameMenu[menu_level].item_height + 6, 0, 394 * sizeof(f32));
        return;
    }
draw_clips: {
    COLLECTION_s collection = CharacterCollection;
    i16 clips[128];
    const i32 all = hub_new_level == -1 || LDataList[hub_new_level].episode_index == -1;
    collection.count_y = CutScenePlayer_CountEpisodeClips(i_episodes, all, clips);
    collection.field_10 = 1.0f;
    collection.count_x = 7;
    if (episodesmode != 1 && episodestime < episodesduration)
        alpha = (episodestime / episodesduration) * alpha;
    CutScenePlayer_DrawGrid(&collection, clips, 0.0f, 0.05f, i_clip[i_episodes], alpha);
    return;
}
draw_play: {
    if (episodestime < episodesduration)
        alpha = (episodestime / episodesduration) * alpha;
    const f32 time = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    alpha = (0.15f * NU_SIN_LUT(static_cast<i32>((time + time) * 65536.0f)) + 0.85f) * alpha;
    const u8 text_alpha = static_cast<i32>(128.0f * alpha);
    Text3DEx(TTab[tPLAY], 0.0f, -0.15f, 1.0f, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE,
             0, 255, 255, 255, text_alpha);
    DrawPanel3DObject(0.0f, 0.15f, 1.0f, 0.35f, 0.35f, 0.35f, 0, 0, 0, &WORLD->lev_objs[167].special, 0, alpha);
    menu->item_x[0] = 0.0f;
    menu->item_y[0] = 0.15f;
    menu->item_width[0] = 0.175f;
    menu->item_height[0] = 0.0f;
    const f32 text_scale = 0.35f * 3.0f;
    Text3DEx(">", 0.0105f, 0.15f, 1.0f, text_scale * 1.5f, text_scale, text_scale, 0, 255, 255, 255, text_alpha);
    memset(GameMenu[menu_level].item_width + 1, 0, 399 * sizeof(f32));
    memset(GameMenu[menu_level].item_height + 1, 0, 399 * sizeof(f32));
}
}

void MenuInitEpisodes(MENU_s *) {
    lastepisodesmode = -1;
    episodestime = 0.0f;
    episodesmode = 0;
    episodesduration = 0.6f;
    if (Game_AreaSave != NULL && Game_AreaSave[EDataList[i_episodes].area_ids[0]].complete == 0) {
        i_episodes = 0;
    }
    if (hub_new_level != -1 && LDataList[hub_new_level].episode_index != -1) {
        episodesmode = 2;
        if (HubStartDoor == NULL) {
            i_episodes = LDataList[hub_new_level].episode_index;
        }
    } else if (HubStartDoor != NULL) {
        episodesmode = 2;
    }
}

void MenuUpdateEpisodes(MENU_s *) {
    if (FadeSys.fade > 0.0f || MainRenderTime > 0.0f)
        return;
    const f32 elapsed = FRAMETIME;
    episodestime = elapsed + episodestime;
    MENU *menu = &GameMenu[GameMenuLevel];
    i16 clips[128];
    switch (episodesmode) {
        case 0: {
            i32 up = 0, down = 0, left = 0, right = 0, confirm = 0, cancel = 0;
            for (i32 player = 0; player < 2; ++player) {
                if (__builtin_expect(!MenuPacket.active_player[player], 0))
                    continue;
                const u32 pressed = GamePad[player].buttons_pressed;
                if (__builtin_expect(!!(pressed & GAMEPAD_MENUSELECT), 0)) {
                    confirm = 1;
                    break;
                }
                if (pressed & GAMEPAD_MENUCANCEL) {
                    cancel = 1;
                    break;
                }
                const u32 directions = pressed | GamePad[player].left_directions;
                if (directions & GAMEPAD_DUP)
                    up = 1;
                else if (directions & GAMEPAD_DDOWN)
                    down = 1;
                else if (directions & GAMEPAD_DLEFT)
                    left = 1;
                else if (directions & GAMEPAD_DRIGHT)
                    right = 1;
            }
            if (menu->input_activity) {
                if (menu->confirm_pressed) {
                    if (menu->selected_item == i_episodes)
                        confirm = 1;
                    else
                        i_episodes = menu->selected_item;
                }
                if (__builtin_expect(!!(menu->cancel_pressed), 0))
                    cancel = 1;
            }
            if (confirm) {
                if (Game_AreaSave != NULL && Game_AreaSave[EDataList[i_episodes].area_ids[0]].complete) {
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                    episodestime = 0.0f;
                    lastepisodesmode = episodesmode;
                    episodesmode = 2;
                    episodesduration = 0.6f;
                } else {
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                }
            } else if (cancel) {
                GameAudio_PlaySfx(0x31, NULL, 0, 0);
                episodestime = 0.0f;
                lastepisodesmode = episodesmode;
                episodesmode = 1;
                episodesduration = 0.6f;
            } else {
                const i8 previous = i_episodes;
                if (up && previous >= 3)
                    i_episodes -= 3;
                else if (down && previous < 3)
                    i_episodes += 3;
                else if (left && previous % 3 > 0)
                    --i_episodes;
                else if (right && previous % 3 != 2)
                    ++i_episodes;
                if (previous != i_episodes)
                    GameAudio_PlaySfx(0x2f, NULL, 0, 0);
            }
            break;
        }
        case 1:
            episodestime = episodestime + elapsed;
            if (episodestime < episodesduration)
                return;
            WipeBackToHub();
            break;
        case 2: {
            const i32 all = hub_new_level == -1 || LDataList[hub_new_level].episode_index == -1;
            const u16 count = CutScenePlayer_CountEpisodeClips(i_episodes, all, clips);
            if (__builtin_expect(i_clip[i_episodes] >= count, 1))
                i_clip[i_episodes] = count - 1;
            const i32 previous = i_clip[i_episodes];
            bool confirm = false, cancel = false;
            if (MenuPacket.active_player[0]) {
                if (GamePad[0].pad->digital_buttons_pressed & GAMEPAD_MENUSELECT) {
                    confirm = true;
                } else if (GamePad[0].pad->digital_buttons_pressed & GAMEPAD_MENUCANCEL) {
                    cancel = true;
                } else {
                    const u32 held = GamePad[0].buttons_held | GamePad[0].buttons_released;
                    const u32 pressed = GamePad[0].buttons_pressed | GamePad[0].left_directions;
                    i32 up = pressed & GAMEPAD_DUP, down = pressed & GAMEPAD_DDOWN;
                    i32 left = pressed & GAMEPAD_DLEFT, right = pressed & GAMEPAD_DRIGHT;
                    i32 up_held = held & GAMEPAD_DUP, down_held = held & GAMEPAD_DDOWN;
                    i32 left_held = held & GAMEPAD_DLEFT, right_held = held & GAMEPAD_DRIGHT;
                    if (down && up)
                        down = up = 0;
                    if (right && left)
                        right = left = 0;
                    if (down_held && up_held)
                        down_held = up_held = 0;
                    if (right_held && left_held)
                        right_held = left_held = 0;
                    MenuRepeat(&up_held, &up, uprepeattime, uprepeatcount, 0.1f, FRAMETIME);
                    MenuRepeat(&down_held, &down, downrepeattime, downrepeatcount, 0.1f, FRAMETIME);
                    MenuRepeat(&left_held, &left, leftrepeattime, leftrepeatcount, 0.1f, FRAMETIME);
                    MenuRepeat(&right_held, &right, rightrepeattime, rightrepeatcount, 0.1f, FRAMETIME);
                    if (__builtin_expect(!up, 0)) {
                        if (down) {
                            i32 next = previous + 7;
                            if (next >= count) {
                                const i32 rows = count / 7 + 1;
                                next = (next < rows * 7 ? previous + 14 : next) - rows * 7;
                            }
                            i_clip[i_episodes] = next;
                        } else if (__builtin_expect(!!(left), 0)) {
                            i32 column = previous % 7 - 1;
                            if (column < 0)
                                column += 7;
                            i32 next = column + previous / 7 * 7;
                            if (next >= count)
                                next = count - 1;
                            i_clip[i_episodes] = next;
                        } else if (right) {
                            i32 column = previous % 7 + 1;
                            if (column == 7)
                                column -= 7;
                            const i32 row = previous / 7 * 7;
                            const i32 next = row + column;
                            if (next < count) {
                                i_clip[i_episodes] = next;
                            } else {
                                i_clip[i_episodes] = row;
                            }
                        }
                    } else {
                        i32 next = previous - 7;
                        if (__builtin_expect(next < 0, 0)) {
                            next += (count / 7 + 1) * 7;
                            if (next >= count)
                                next -= 7;
                        }
                        i_clip[i_episodes] = next;
                    }
                }
            }
            if (menu->input_activity && menu->confirm_pressed) {
                if (__builtin_expect(menu->selected_item == i_clip[i_episodes], 0))
                    confirm = true;
                else
                    i_clip[i_episodes] = menu->selected_item;
            }
            if (confirm) {
                if (CutScenePlayer_CanStart(clips[i_clip[i_episodes]])) {
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                    episodestime = 0.0f;
                    lastepisodesmode = episodesmode;
                    episodesmode = 3;
                    episodesduration = 0.6f;
                } else {
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                }
            } else if (menu->cancel_pressed || cancel) {
                GameAudio_PlaySfx(0x31, NULL, 0, 0);
                episodestime = 0.0f;
                lastepisodesmode = episodesmode;
                episodesmode = 0;
                episodesduration = 0.6f;
                if (__builtin_expect(hub_new_level != -1 && LDataList[hub_new_level].episode_index != -1, 0))
                    episodesmode = 1;
            } else if (previous != i_clip[i_episodes]) {
                GameAudio_PlaySfx(0x2f, NULL, 0, 0);
            }
            break;
        }
        case 3: {
            const i32 all = hub_new_level == -1 || LDataList[hub_new_level].episode_index == -1;
            CutScenePlayer_CountEpisodeClips(i_episodes, all, clips);
            i32 confirm = 0, cancel = 0;
            if (MenuPacket.active_player[0]) {
                if (GamePad[0].pad->digital_buttons_pressed & GAMEPAD_MENUSELECT)
                    confirm = 1;
                else if (GamePad[0].pad->digital_buttons_pressed & GAMEPAD_MENUCANCEL)
                    cancel = 1;
            }
            if (confirm || menu->confirm_pressed) {
                const i16 clip = clips[i_clip[i_episodes]];
                GameAudio_PlaySfx(0x30, NULL, 0, 0);
                CutScenePlayer_Start(clip, hub_new_level);
                hub_goto_clipsmenu_episode = 99;
            } else if (menu->cancel_pressed || cancel) {
                GameAudio_PlaySfx(0x31, NULL, 0, 0);
                lastepisodesmode = episodesmode;
                episodestime = 0.0f;
                episodesmode = 2;
                episodesduration = 0.6f;
            }
            break;
        }
    }
}

void MenuDrawEpisodes(MENU_s *) {
    if (FadeSys.fade > 0.0f)
        return;
    f32 alpha = 1.0f;
    if (episodesmode == 1) {
        if (!(episodestime < 0.5f))
            return;
        alpha = 1.0f - NU_SIN_LUT(static_cast<i32>((episodestime + episodestime) * 16384.0f));
        if (!(alpha > 0.0f))
            return;
    }
    Hub_DrawAreaStats(alpha, last_hub_area, 18);
    const f32 icon_alpha = (MenuPacket.active_player[0] ? 1.0f : DROPINALPHA) * alpha;
    DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 166, icon_alpha, icon_alpha, 1, NULL);
    if (MainRenderTime <= 0.0f) {
        DrawEpisodesMenu(1, alpha);
    } else if (MainRenderTime < 1.0f) {
        const f32 menu_alpha = 1.0f - NU_SIN_LUT(static_cast<i32>((1.0f - MainRenderTime) * 16384.0f + 16384.0f));
        DrawEpisodesMenu(0, menu_alpha * alpha);
    }
    if (MainRenderTime <= 0.0f && alpha == 1.0f)
        DrawPlayerIconPrompts(MenuPacket.active_player[0], tSELECT, 1.0f, -1, tBACK, -1, tSELECTING,
                              MenuPacket.active_player[1], tSELECT, 1.0f, -1, tBACK, -1, tSELECTING);
}

void MenuInitClips(MENU_s *) {
}

void MenuUpdateClips(MENU_s *) {
}

void MenuDrawClips(MENU_s *) {
}

void MenuInitSelectMode(MENU_s *) {
    const i32 area = LDataList[hub_new_level].area_index;
    if (LOSTTEMPLE_ADATA != NULL && LOSTTEMPLE_ADATA->index == area) {
        hub_selectmode = 0;
    } else {
        bool unlocked = FreePlayUnlocked();
        if (unlocked != 1 && area != -1)
            unlocked = ADataList[area].episode_index == 0xff && (ADataList[area].flags & 0x4000) != 0;
        if (unlocked && (ADataList[area].flags & AREAFLAG_NO_FREEPLAY) == 0 && Game.area_save[area].area_complete != 0)
            hub_selectmode = 1;
        else
            hub_selectmode = 0;
    }
    selectmodemode = 0;
}

void MenuUpdateSelectMode(MENU_s *) {
    Hub_UpdateSelectMode();
}

void MenuDrawSelectMode(MENU_s *) {
    if (__builtin_expect(MenuStopDraw, 1))
        return;
    if (__builtin_expect(selectmodemode >= 1 && selectmodemode <= 3, 0)) {
        if (FadeSys.fade > 0.0f)
            return;
    } else if (selectmodemode == 4) {
        if (pFadeInfo->fade > 0.0f)
            return;
    }
    f32 alpha = 1.0f;
    switch (selectmodemode) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (!(selectmodetime < 0.5f))
                return;
            alpha = 1.0f - NU_SIN_LUT(static_cast<i32>((selectmodetime / selectmodeduration) * 16384.0f));
            if (!(alpha > 0.0f))
                return;
            break;
    }
    AREADATA *area = &ADataList[last_hub_area];
    if (area->flags & 0x100)
        Hub_DrawSuperBonusStats(area, alpha);
    else if (area->flags & 4)
        Hub_DrawBonusStats(alpha, -1, hub_bonusepisode);
    else
        Hub_DrawAreaStats(alpha, last_hub_area, hub_selectmode);

    const f32 icon_alpha = (MenuPacket.active_player[0] ? 1.0f : DROPINALPHA) * alpha;
    DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 166, icon_alpha, icon_alpha, 1, NULL);
    if (MainRenderTime <= 0.0f) {
        Hub_DrawSelectModeMenu(1, alpha);
    } else if (MainRenderTime < 1.0f) {
        const f32 menu_alpha = 1.0f - NU_SIN_LUT(static_cast<i32>((1.0f - MainRenderTime) * 16384.0f + 16384.0f));
        Hub_DrawSelectModeMenu(0, menu_alpha * alpha);
    }
    if (MainRenderTime > 0.0f)
        return;
    if (selectmodemode == 0 && alpha == 1.0f)
        DrawPlayerIconPrompts(MenuPacket.active_player[0], tSELECT, 1.0f, -1, tBACK, -1, tSELECTING,
                              MenuPacket.active_player[1], tSELECT, 1.0f, -1, tBACK, -1, tSELECTING);
}

void MenuInitBonusMode(MENU_s *) {
    bonusmodemode = 0;
    hub_bonusmode = 0;
    hub_bonusarea = LDataList[hub_new_level].area_index;
    hub_bonusepisode = static_cast<i8>(ADataList[hub_bonusarea].episode_index);
    bonusmodearcade = SENATE_ADATA != NULL && hub_bonusarea == SENATE_ADATA->index;
}

void MenuUpdateBonusMode(MENU_s *) {
    MENU *menu = &GameMenu[GameMenuLevel];
    if (FadeSys.fade > 0.0f || MainRenderTime > 0.0f)
        return;

    const f32 blip_frame_time = *reinterpret_cast<volatile const f32 *>(&FRAMETIME);
    if (BlipL[0] > 0.0f)
        BlipL[0] -= blip_frame_time;
    if (BlipR[0] > 0.0f)
        BlipR[0] -= blip_frame_time;
    if (BlipU[0] > 0.0f)
        BlipU[0] -= blip_frame_time;
    if (BlipD[0] > 0.0f)
        BlipD[0] -= blip_frame_time;
    i32 player_up[2] = {}, player_down[2] = {};
    if (BlipL[1] > 0.0f)
        BlipL[1] -= blip_frame_time;
    if (BlipR[1] > 0.0f)
        BlipR[1] -= blip_frame_time;
    if (BlipU[1] > 0.0f)
        BlipU[1] -= blip_frame_time;
    if (BlipD[1] > 0.0f)
        BlipD[1] -= blip_frame_time;
    i32 player_left[2] = {}, player_right[2] = {};

    switch (bonusmodemode) {
        case 1:
            bonusmodetime += FRAMETIME;
            if (bonusmodetime < bonusmodeduration)
                return;
            InitSuperStory(SuperStoryEpisode);
            NewLData = &LDataList[hub_new_level];
            FadeSys.fade = 1.0f;
            FinishLoop_On = 0;
            loadareacharacters_no_backdrop_reset = 1;
            return;
        case 2:
        case 3:
            bonusmodetime += FRAMETIME;
            if (bonusmodetime < bonusmodeduration)
                return;
            MakeFreePlayModelList(MenuPacket.player_model[0], MenuPacket.player_model[1],
                                  LDataList[hub_new_level].area_index, -1, 1);
            makeplayerlist_freeplay = 2;
            NextArea_FreePlay = 1;
            FreePlay = 1;
            NewLData = &LDataList[hub_new_level];
            FadeSys.fade = 1.0f;
            FinishLoop_On = 0;
            loadareacharacters_no_backdrop_reset = 1;
            return;
        case 4:
            bonusmodetime += FRAMETIME;
            if (bonusmodetime >= bonusmodeduration)
                WipeBackToHub();
            return;
        case 0:
            break;
        default:
            return;
    }

    i32 confirm = 0, cancel = 0, up = 0, down = 0, left = 0, right = 0;
    for (i32 i = 0; i < 2; ++i) {
        if (!MenuPacket.active_player[i])
            continue;
        const u32 pressed = GamePad[i].buttons_pressed;
        if (pressed & GAMEPAD_MENUSELECT) {
            confirm = 1;
            break;
        } else if (pressed & GAMEPAD_MENUCANCEL) {
            cancel = 1;
            break;
        } else {
            const u32 direction = pressed | GamePad[i].left_directions;
            if (direction & GAMEPAD_DUP)
                up = player_up[i] = 1;
            else if (direction & GAMEPAD_DDOWN)
                down = player_down[i] = 1;
            else if (direction & GAMEPAD_DLEFT)
                left = player_left[i] = 1;
            else if (direction & GAMEPAD_DRIGHT)
                right = player_right[i] = 1;
        }
    }

    if (menu->input_activity) {
        if (!bonusmodearcade) {
            if (menu->confirm_pressed) {
                hub_bonusmode = menu->selected_item;
                confirm = 1;
            }
        } else {
            const i32 item = menu->selected_item;
            const u8 choices = reinterpret_cast<u8 *>(&ArcadeItem)[item * 8 + 5];
            if (menu->left_pressed && choices >= 2)
                right = 1;
            if (menu->right_pressed && choices >= 2)
                left = 1;
            if (menu->confirm_pressed) {
                if (choices == 1) {
                    hub_bonusmode = item;
                    confirm = 1;
                } else if (item == hub_bonusmode) {
                    right = 1;
                } else {
                    hub_bonusmode = item;
                    menu->confirm_pressed = 0;
                }
            }
        }
        if (menu->cancel_pressed)
            cancel = 1;
    }

    if (player_up[0])
        BlipU[0] = 0.1f;
    if (player_down[0])
        BlipD[0] = 0.1f;
    if (player_left[0])
        BlipL[0] = 0.1f;
    if (player_right[0])
        BlipR[0] = 0.1f;
    if (player_up[1])
        BlipU[1] = 0.1f;
    if (player_down[1])
        BlipD[1] = 0.1f;
    if (player_left[1])
        BlipL[1] = 0.1f;
    if (player_right[1])
        BlipR[1] = 0.1f;

    if (bonusmodearcade) {
        if (confirm) {
            if (hub_bonusmode != 2)
                return;
            AREADATA *area = *ArcadeLevel[ArcadeItem.level].area;
            if (area == NULL) {
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
                return;
            }
            GameAudio_PlaySfx(0x30, NULL, 0, 0);
            hub_freeplaysource = 1;
            hub_new_level = area->levels[0];
            Hub_InitFreePlaySelect(area->index, -1, -1);
            NewMenu(17, -1, -1);
            return;
        }
    } else if (confirm) {
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
        if (hub_new_level == -1)
            return;
        const i8 area = LDataList[hub_new_level].area_index;
        if (area == -1 || (ADataList[area].flags & 0x100) == 0)
            return;
        hub_freeplaysource = 1;
        Hub_InitFreePlaySelect(area, -1, -1);
        NewMenu(17, -1, -1);
        return;
    }

    if (cancel) {
        GameAudio_PlaySfx(0x31, NULL, 0, 0);
        bonusmodemode = 4;
        bonusmodetime = 0.0f;
        bonusmodeduration = 0.6f;
        return;
    }

    if (!bonusmodearcade) {
        if (hub_bonusepisode == -1 || !up || hub_bonusmode <= 0)
            return;
        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
        --hub_bonusmode;
        return;
    }

    if (up) {
        if (hub_bonusmode <= 0)
            return;
        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
        --hub_bonusmode;
        return;
    }
    if (down) {
        if (hub_bonusmode >= 2)
            return;
        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
        ++hub_bonusmode;
        return;
    }
    i8 *entries = reinterpret_cast<i8 *>(&ArcadeItem);
    const i32 count = static_cast<u8>(entries[hub_bonusmode * 8 + 5]);
    if (count > 1) {
        if (!left) {
            if (right) {
                GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                if (++entries[hub_bonusmode * 8 + 4] >= count)
                    entries[hub_bonusmode * 8 + 4] = 0;
            }
        } else {
            GameAudio_PlaySfx(0x2f, NULL, 0, 0);
            if (--entries[hub_bonusmode * 8 + 4] < 0)
                entries[hub_bonusmode * 8 + 4] = count - 1;
        }
    }
}

void MenuDrawBonusMode(MENU_s *) {
    if (MenuStopDraw)
        return;
    if (bonusmodemode >= 1 && bonusmodemode <= 4 && FadeSys.fade > 0.0f)
        return;

    f32 alpha = 1.0f;
    if (bonusmodemode >= 1 && bonusmodemode <= 4) {
        if (!(bonusmodetime < 0.5f))
            return;
        alpha = 1.0f - NU_SIN_LUT(static_cast<i32>((bonusmodetime / bonusmodeduration) * 16384.0f));
        if (!(alpha > 0.0f))
            return;
    }

    if (bonusmodearcade)
        Hub_DrawArcadeStats(alpha);
    else if (hub_bonusepisode != -1)
        Hub_DrawBonusStats(alpha, -1, hub_bonusepisode);
    else
        Hub_DrawSuperBonusStats(hub_bonusarea == -1 ? NULL : &ADataList[hub_bonusarea], alpha);

    const f32 icon_alpha = (MenuPacket.active_player[0] ? 1.0f : DROPINALPHA) * alpha;
    DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 166, icon_alpha, icon_alpha, 1, NULL);

    if (MainRenderTime <= 0.0f) {
        Hub_DrawBonusModeMenu(1, alpha);
    } else if (MainRenderTime < 1.0f) {
        const f32 menu_alpha = 1.0f - NU_SIN_LUT(static_cast<i32>((1.0f - MainRenderTime) * 16384.0f + 16384.0f));
        Hub_DrawBonusModeMenu(0, menu_alpha * alpha);
    }

    if (MainRenderTime > 0.0f)
        return;
    if (bonusmodemode == 0 && alpha == 1.0f) {
        f32 prompt_alpha = 0.333f;
        if (bonusmodearcade && hub_bonusmode == 2 && *ArcadeLevel[ArcadeItem.level].area != NULL)
            prompt_alpha = alpha;
        DrawPlayerIconPrompts(MenuPacket.active_player[0], tSELECT, prompt_alpha, -1, tBACK, -1, -1,
                              MenuPacket.active_player[1], tSELECT, prompt_alpha, -1, tBACK, -1, -1);
    }
}

void MenuInitFreePlay(MENU_s *menu) {
    menu->first_row = 0;
    menu->last_row = 2;
    menu->selected_row = 0;
    menu->selected_item = 0;
}

void MenuUpdateFreePlay(MENU_s *) {
    Hub_UpdateFreePlaySelect();
}

void MenuDrawFreePlay(MENU_s *) {
    if (MenuStopDraw == 0) {
        Hub_DrawFreePlaySelect();
    }
}

void Hub_ActivateDoorMenu(LEVELDATA_s **level) {
    const i32 index = (*level)->area_index;
    if (index == -1) {
        return;
    }

    const i32 area = last_hub_area;
    if (area != index) {
        *level = NULL;
        return;
    }

    MakeMenuPacket();
    hub_new_level = ADataList[area].levels[0];
    *level = NULL;
    hub_episode_time = 0.0f;
    hub_area_time = 0.0f;
    MainRenderTargetTime = 0.0f;
    NeedScreenGrab(1);
    BackDrop_ResetColours();

    i32 menu_id;
    if (area != -1 && (ADataList[area].flags & AREAFLAG_BONUS_AREA) != 0) {
        menu_id = ADataList[area].episode_index != 0xff ? HUB_DOOR_MENU_EPISODE_BONUS : HUB_DOOR_MENU_BONUS;
    } else if (VEHICLES_ADATA != NULL && area == VEHICLES_ADATA->index) {
        // The original sizes this list for 340 vehicle ids.
        i16 vehicle_ids[340];
        i32 vehicle_count;
        Collection_GetIDList(&VehicleCollection, HUB_VEHICLE_COLLECTION_ALLOWED, HUB_VEHICLE_COLLECTION_REQUIRED,
                             vehicle_ids, &vehicle_count, NULL, 0);
        menu_id = HUB_DOOR_MENU_VEHICLE;
        hub_freeplaysource = -1;
        Hub_InitFreePlaySelect(area, vehicle_count, -1);
    } else {
        menu_id = HUB_DOOR_MENU_STANDARD;
        if (LOSTTEMPLE_ADATA != NULL) {
            menu_id = area == LOSTTEMPLE_ADATA->index ? HUB_DOOR_MENU_EPISODE_BONUS : HUB_DOOR_MENU_STANDARD;
        }
    }

    GameAudio_PlaySfx(HUB_AUDIO_EVENT_OPEN_DOOR_MENU, NULL, 0, 0);
    Hint_CancelCurrent();
    NewMenu(menu_id, -1, -1);
}

void Hub_Update(WORLDINFO_s *world) {
    const i32 menu = GetMenuID();

    if (HubStartDoor != NULL) {
        DOOR_s *door = static_cast<DOOR_s *>(HubStartDoor);
        hub_new_level = door->level;
        Door_GoThrough(world, door, 1);
        HubStartDoor = NULL;
    }

    show_unlock_shop_hint = 0;
    show_unlock_customiser_hint = 0;
    show_unlock_minikitviewer_hint = 0;

    // Loading completion is tracked separately from the per-frame spawn pass.
    if (hub_custodians_finished_loading == 0) {
        if (BGLOAD == 0) {
            hub_custodians_finished_loading = 1;
        } else {
            i32 required = 0;
            i32 loaded = 0;
            for (i32 pack = 0; pack < 11; ++pack) {
                STOREPACK &store_pack = StorePack[pack];
                if (Store_IsPackUnlocked(pack) != 0 || store_pack.id == NULL) {
                    continue;
                }
                const i32 id = *store_pack.id;
                if (__builtin_expect(id == -1, 0)) {
                    continue;
                }
                ++required;
                if (APICharacterLoaded(id) != NULL) {
                    ++loaded;
                }
            }
            if (loaded == required) {
                hub_custodians_finished_loading = 1;
            }
        }
    }

    for (i32 pack = 0; pack < 11; ++pack) {
        STOREPACK &store_pack = StorePack[pack];
        if (__builtin_expect(store_pack.id != NULL, 1)) {
            const i32 id = *store_pack.id;
            if (id != -1 && Store_IsPackUnlocked(pack) == 0 &&
                (store_pack.field44_0x32 == 0xff ||
                 store_pack.field44_0x32 == static_cast<u8>(GameCam->sock_position.location.sock)) &&
                APICharacterLoaded(id) != NULL && FindGameObject(id, 0, 0, 0, 0) == NULL) {
                AIPATHINFO_s *path_info = NULL;
                AILOCATORSET *locator_set = AIPathFindLocatorSet(world->ai_sys, store_pack.custodian_locator_set);
                if (locator_set != NULL) {
                    AILOCATOR *locators[64] = {};
                    AILocatorSet_CheckLocatorsStillAssigned(world->ai_sys, locator_set);
                    i32 count = 0;
                    for (i32 i = 0; i < locator_set->locator_count && count < 64; ++i) {
                        if (locator_set->assigned[i] == 0xff) {
                            locators[count++] = &world->ai_sys->locators[locator_set->locator_entries[i]];
                        }
                    }
                    if (__builtin_expect(count != 0, 0)) {
                        AILOCATOR *locator = LocalGetNearestLocator(locators, count, 0.0f, &player->apiobj.position,
                                                                    1000000000.0f, 0, 1000000000.0f, 1000000000.0f);
                        if (locator != NULL) {
                            path_info = reinterpret_cast<AIPATHINFO_s *>(&locator->path);
                        }
                    }
                }
                GameObject_s *custodian =
                    AddDynamicCreature(id, &StorePack[pack].custodian_position, store_pack.custodian_angle,
                                       const_cast<char *>("party"), path_info, NULL, 1, NULL, NULL, 0, 1);
                if (custodian != NULL) {
                    custodian->field_0xee8 = store_pack.custodian_position.x;
                    custodian->field_0xeec = store_pack.custodian_position.z;
                    custodian->field_0x106e = store_pack.custodian_angle;
                    Store_RootPackCustodian(pack, custodian);
                    AddGameDebris(world->debris_sys, 92, &custodian->apiobj.collision_position);
                }
            }
        }
    }
    if (buildits_reset == 0) {
        Hub_ResetBuildIts();
        ++buildits_reset;
    }

    const bool memory_card_menu = MenuInMemoryCard() != 0;
    if (memory_card_menu) {
        Hub_ClearStats();
        Hub_PreventDropOutTime = 1.0f;
    } else if (menu >= 12 && menu <= 14) {
        Hub_PreventDropOutTime = 1.0f;
    }

    const f32 pulse_time = NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);
    TJTYPEA = static_cast<i32>(
        NU_SIN_LUT(static_cast<u16>(static_cast<i32>((pulse_time + pulse_time) * 65536.0f))) * 16.0f + 80.0f);
    if (menu == 8) {
        hub_jabbaawake = 1.0f;
    } else if (hub_jabbaawake > 0.0f) {
        hub_jabbaawake -= FRAMETIME * 0.333f;
        if (__builtin_expect(hub_jabbaawake < 0.0f, 0)) {
            hub_jabbaawake = 0.0f;
        }
    }

    Hub_HologramAlpha = SeekLinearF(Hub_HologramAlpha, Hub_HologramTargetAlpha, FRAMETIME * 20.0f);
    if (Hub_HologramAlpha == Hub_HologramTargetAlpha) {
        Hub_HologramTargetAlpha = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 2.0f - 1.0f;
    }
    if (Hub_PreventDropOutTime > 0.0f) {
        Hub_PreventDropOutTime -= FRAMETIME;
    }

    UpdateCharacterLoad();
    bool shop_available = false;
    if (SHOPACTIVE != 0) {
        DrawCoinTotalTime = 1.0f;
    } else if (GetMenuID() != 12) {
        for (i32 player_index = 0; player_index < 2; ++player_index) {
            GameObject_s *player = Player[player_index];
            if (player != NULL && player->apiobj.player_controlled &&
                __builtin_expect(player->apiobj.field_0x27d != 0, 1) && player->pad_gamepad->input_magnitude == 0.0f &&
                player->pad_gamepad->previous_input_magnitude == 0.0f &&
                (player->field_0x7a5 == 0xff || player->field_0x7a5 == 0x31 || player->field_0x7a5 == 0x32)) {
                shop_available = player->apiobj.field_0x281 == 0x0e;
            }
        }
        if (shop_available && !HubShopUnlocked()) {
            if (GetHintFromUIButton() != NULL ||
                (hintsys.active_hint != NULL && hintsys.active_hint->control_mode_ids[0] != 0x619)) {
                Hint_CancelCurrent();
                only_process_this_hint_id = 0x619;
            }
            show_unlock_shop_hint = 1;
            shop_available = false;
        }
    }
    if (LevTime[0] > 0.0f) {
        if (MenuInMemoryCard() == 0) {
            LevTime[0] -= FRAMETIME;
        }
    } else if (LevLock[0] != 0) {
        if (!shop_available) {
            LevLock[0] = 0;
        }
    } else if (GetMenuID() == -1 && shop_available) {
        TempGame = Game;
        SHOPACTIVE = 1;
        MakeMenuPacket();
        Hint_CancelCurrent();
        NewMenu(13, -1, -1);
        shop_save_done = 0;
        shop_quit = 0;
        ShopNameAlpha = 0.0f;
        ShopLockedScale = 1.0f;
        NuIOS_RecordFlurryEvent(const_cast<char *>("hubshop_enter"));
    }
    bool customiser_available = false;
    if (Customiser_MenuAvailable(CharacterCustomiser) != 0) {
        for (i32 player_index = 0; player_index < 2; ++player_index) {
            GameObject_s *player = Player[player_index];
            if (player != NULL && player->apiobj.player_controlled &&
                __builtin_expect(player->apiobj.field_0x27d != 0, 1) && player->pad_gamepad->input_magnitude == 0.0f &&
                player->pad_gamepad->previous_input_magnitude == 0.0f &&
                (player->field_0x7a5 == 0xff || player->field_0x7a5 == 0x31 || player->field_0x7a5 == 0x32)) {
                customiser_available = player->apiobj.field_0x281 == 0x0f;
            }
        }
        if (customiser_available && !HubCustomiserUnlocked()) {
            if (GetHintFromUIButton() != NULL ||
                (hintsys.active_hint != NULL && hintsys.active_hint->control_mode_ids[0] != 0x61a)) {
                Hint_CancelCurrent();
                only_process_this_hint_id = 0x61a;
            }
            show_unlock_customiser_hint = 1;
            customiser_available = false;
        }
    }
    if (LevTime[1] > 0.0f) {
        if (MenuInMemoryCard() == 0) {
            LevTime[1] -= FRAMETIME;
        }
    } else if (LevLock[1] != 0) {
        if (!customiser_available) {
            LevLock[1] = 0;
        }
    } else if (GetMenuID() == -1 && customiser_available) {
        OldCustomiseGame = Game;
        MakeMenuPacket();
        NewMenu(12, -1, -1);
        Hint_CancelCurrent();
        customiser_save_done = 0;
        customiser_quit = 0;
        customiser_changed = 0;
    }
    Customiser_Update(CharacterCustomiser, world);
    const i32 missions_available = Missions_PartyAvailable(NULL);
    bool mission_menu_available = false;
    if (missions_available != 0) {
        for (i32 player_index = 0; player_index < 2; ++player_index) {
            GameObject_s *player = Player[player_index];
            if (player != NULL && player->apiobj.player_controlled &&
                __builtin_expect(player->apiobj.field_0x27d != 0, 1) && player->pad_gamepad->input_magnitude == 0.0f &&
                player->pad_gamepad->previous_input_magnitude == 0.0f &&
                (player->field_0x7a5 == 0xff || player->field_0x7a5 == 0x31 || player->field_0x7a5 == 0x32)) {
                mission_menu_available = player->apiobj.field_0x281 == 0x14;
            }
        }
    }
    if (LevLock[3] != 0) {
        if (!mission_menu_available) {
            LevLock[3] = 0;
        }
    } else if (LevTime[3] > 0.0f) {
        if (MenuInMemoryCard() == 0) {
            LevTime[3] -= FRAMETIME;
        }
    } else if (GetMenuID() == -1 && mission_menu_available) {
        MakeMenuPacket();
        NewMenu(8, -1, -1);
        Hint_CancelCurrent();
    }
    Hub_UpdatePackObstacle(5);
    Hub_UpdatePackObstacle(6);
    if (FadeSys.fade != 0.0f) {
        hub_episode_time = 0.0f;
        hub_episode = -1;
    } else {
        i32 selected_episode = -1;
        for (i32 i = 0; static_cast<u16>(HubEpisodeInfo[i].episode) <= 8; ++i) {
            HUBEPISODEINFO_s &episode = HubEpisodeInfo[i];
            if (episode.door != NULL &&
                GizmoGetOutput(world->gizmo_sys, episode.door, static_cast<u16>(episode.flags), 0) != 0) {
                selected_episode = episode.episode;
                break;
            }
        }
        for (i32 i = 0; static_cast<u16>(HubEpisodeInfo[i].episode) <= 8; ++i) {
            HUBEPISODEINFO_s &episode = HubEpisodeInfo[i];
            if (episode.door == NULL) {
                continue;
            }
            if (!(selected_episode != -1 && selected_episode != episode.episode)) {
                if (static_cast<u8>(episode.force_open) != 0 ||
                    Episode_CountOpenAreas(episode.episode, -1, Game_AreaSave) != 0) {
                    static_cast<GIZOBSTACLE_s *>(episode.door->object)->runtime_flags &= static_cast<u8>(~8);
                }
            } else {
                static_cast<GIZOBSTACLE_s *>(episode.door->object)->runtime_flags |= 8;
            }
        }
        if (missions_available != 0) {
            // Original 0x1b762b writes the low byte of the Jabba entry's override.
            HubEpisodeInfo[8].force_open = 1;
        }
        const i32 gold_bricks = Game.field_0x7c26[0];
        i32 buildit_index = 0;
        for (i32 i = 0; HubAreaInfo[i].area_name != NULL; ++i) {
            GIZMO *gizmo = HubAreaInfo[i].bonus_gizmo;
            if (gizmo == NULL) {
                continue;
            }
            const i32 output = GizmoGetOutput(world->gizmo_sys, gizmo, 0, 0);
            gizmo = HubAreaInfo[i].bonus_gizmo;
            if (__builtin_expect(output == 0, 0)) {
                GIZBUILDIT_s *buildit = static_cast<GIZBUILDIT_s *>(gizmo->object);
                if (__builtin_expect(!(gold_bricks < buildit->anim_object_count), 0)) {
                    if ((buildit->availability_flags & GIZBUILDIT_AVAILABILITY_ACTIVE) == 0) {
                        GizmoActivate(world->gizmo_sys, gizmo, 1, 0);
                    }
                } else {
                    GizmoSetVisibility(world->gizmo_sys, gizmo, 0, 0);
                    if (buildit->linked_buildit != NULL) {
                        buildit = buildit->linked_buildit;
                    }
                    for (i32 piece = 0; piece < buildit->anim_object_count; ++piece) {
                        if (__builtin_expect(buildit->anim_objects[piece] != NULL, 0)) {
                            if (piece < gold_bricks)
                                NuSpecialSetVisibility(&buildit->anim_objects[piece]->special, 1);
                            else
                                NuSpecialSetVisibility(&buildit->anim_objects[piece]->special, 0);
                        }
                    }
                }
            }
            if (GizmoGetOutput(world->gizmo_sys, HubAreaInfo[i].bonus_gizmo, 0, 0) != 0) {
                const u32 bit = 1U << (buildit_index & 31);
                if ((Game.field_0x7c26[2] & bit) == 0) {
                    Game.field_0x7c26[2] = static_cast<u8>(Game.field_0x7c26[2] | bit);
                    TriggerAutoSave();
                }
            }
            ++buildit_index;
        }
        if (hub_episode_time == 0.0f) {
            hub_episode = selected_episode;
        }
        const f32 target = hub_episode != -1 && hub_episode == selected_episode ? 1.0f : 0.0f;
        hub_episode_time = SeekLinearF(hub_episode_time, target, FRAMETIME * 2.0f);
    }
    if (GameCam->sock_position.location.sock == 0) {
        NuSpecialSetVisibility(&LevHSpecial[16], Store_IsPackUnlocked(6));
    } else if (GameCam->sock_position.location.sock == 7) {
        if (NuSpecialExistsFn(&LevHSpecial[6]) != 0) {
            NuSpecialSetVisibility(&LevHSpecial[6], missions_available == 0);
        }
        if (NuSpecialExistsFn(&LevHSpecial[7]) != 0) {
            NuSpecialSetVisibility(&LevHSpecial[7], missions_available != 0);
        }
    }
    i32 completed_areas;
    Episode_IsComplete(HubEpisodeInfo[0].data, &completed_areas);
    if (completed_areas == 6) {
        HubAreaInfo[6].force_open = 1;
    }
    Episode_IsComplete(HubEpisodeInfo[1].data, &completed_areas);
    if (completed_areas == 6) {
        HubAreaInfo[13].force_open = 1;
    }
    Episode_IsComplete(HubEpisodeInfo[2].data, &completed_areas);
    if (completed_areas == 6) {
        HubAreaInfo[20].force_open = 1;
    }
    Episode_IsComplete(HubEpisodeInfo[3].data, &completed_areas);
    if (completed_areas == 6) {
        HubAreaInfo[27].force_open = 1;
    }
    Episode_IsComplete(HubEpisodeInfo[4].data, &completed_areas);
    if (completed_areas == 6) {
        HubAreaInfo[34].force_open = 1;
    }
    Episode_IsComplete(HubEpisodeInfo[5].data, &completed_areas);
    if (completed_areas == 6) {
        HubAreaInfo[41].force_open = 1;
    }
    i32 selected_area = -1;
    for (i32 i = 0; HubAreaInfo[i].area_name != NULL; ++i) {
        HUBAREAINFO_s &area = HubAreaInfo[i];
        if (area.area != NULL && area.door != NULL && GizmoGetOutput(world->gizmo_sys, area.door, 1, 0) != 0) {
            selected_area = area.area->index;
            break;
        }
    }
    if (selected_area != -1) {
        for (i32 i = 0; HubAreaInfo[i].area_name != NULL; ++i) {
            HUBAREAINFO_s &area = HubAreaInfo[i];
            if (area.door != NULL && area.area != NULL && area.area->index != selected_area) {
                static_cast<GIZOBSTACLE_s *>(area.door->object)->runtime_flags |= 8;
            }
        }
    } else {
        for (i32 i = 0; HubAreaInfo[i].area_name != NULL; ++i) {
            HUBAREAINFO_s &area = HubAreaInfo[i];
            if (area.door == NULL) {
                continue;
            }
            if (Episode_CountOpenAreas(static_cast<i8>(area.area->episode_index), area.area->index, Game_AreaSave) !=
                    0 ||
                (area.bonus_gizmo != NULL && GizmoGetOutput(world->gizmo_sys, area.bonus_gizmo, 0, 0) != 0) ||
                area.force_open == 1) {
                static_cast<GIZOBSTACLE_s *>(area.door->object)->runtime_flags &= static_cast<u8>(~8);
                NuSpecialSetVisibility(&area.lock, 1);
            }
        }
        static NUVEC minikit_hubpos = {-27.0f, 0.0f, -24.6f};
        if (E1VEHICLE_ADATA != NULL && NuVecXZDistSqr(&player->apiobj.position, &minikit_hubpos, NULL) < 0.25f) {
            selected_area = E1VEHICLE_ADATA->index;
            if (__builtin_expect(!HubMinikitViewerUnlocked(), 0)) {
                if (GetHintFromUIButton() != NULL ||
                    (hintsys.active_hint != NULL && hintsys.active_hint->control_mode_ids[0] != 0x61b)) {
                    Hint_CancelCurrent();
                    only_process_this_hint_id = 0x61b;
                }
                show_unlock_minikitviewer_hint = 1;
                selected_area = -1;
            }
        }
    }
    const i32 area_menu = GetMenuID();
    if (FadeSys.fade == 0.0f && area_menu == -1) {
        if (hub_episode != -1 && hub_episode_time > 0.0f) {
            selected_area = -1;
        }
        if (hub_area_time == 0.0f) {
            hub_area = selected_area;
            last_hub_area = selected_area;
        }
        const f32 target = hub_area != -1 && hub_area == selected_area ? 1.0f : 0.0f;
        hub_area_time = SeekLinearF(hub_area_time, target, FRAMETIME * 2.0f);
        if (hub_area_time == 1.0f && VEHICLES_ADATA != NULL && VEHICLES_ADATA->index == hub_area &&
            PLATFORM_LDATA != NULL) {
            NewLData = PLATFORM_LDATA;
            Hub_ActivateDoorMenu(&NewLData);
        }
    } else {
        Hub_ResetPanel();
    }
    for (i32 i = 0; HubAreaInfo[i].area_name != NULL; ++i) {
        HUBAREAINFO_s &area = HubAreaInfo[i];
        if (area.area == NULL || area.bonus_gizmo_2 == NULL ||
            GizmoGetOutput(world->gizmo_sys, area.bonus_gizmo_2, 1, 0) == 0) {
            continue;
        }
        NUVEC *position = GizmoGetPos(world->gizmo_sys, area.bonus_gizmo_2);
        if (position == NULL) {
            continue;
        }
        if (Player[0] != NULL && Player[0]->apiobj.player_controlled &&
            NuVecDistSqr(&Player[0]->apiobj.position, position, NULL) < 0.25f) {
            break;
        }
        if (Player[1] != NULL && Player[1]->apiobj.player_controlled &&
            NuVecDistSqr(&Player[1]->apiobj.position, position, NULL) < 0.25f) {
            break;
        }
    }
    i32 minikit_candidate = -1;
    if (hub_minikitviewer_gizmo != NULL && GizmoGetOutput(world->gizmo_sys, hub_minikitviewer_gizmo, 1, 0) != 0) {
        const i32 viewer_area = hub_minikitviewer_area;
        if (viewer_area != -1 && FreePlayUnlocked()) {
            minikit_candidate = viewer_area;
        }
    }
    if (LevLock[4] != 0) {
        if (minikit_candidate == -1) {
            LevLock[4] = 0;
        }
    } else if (LevTime[4] > 0.0f) {
        LevTime[4] -= FRAMETIME;
    }
    if (FadeSys.fade == 0.0f) {
        if (LevLock[4] == 0) {
            i32 selected_minikit = -1;
            if (__builtin_expect(!(hub_episode != -1 && hub_episode_time > 0.0f) &&
                                     !(hub_area != -1 && hub_area_time > 0.0f &&
                                       (E1VEHICLE_ADATA == NULL || hub_area != E1VEHICLE_ADATA->index)) &&
                                     minikit_candidate != -1,
                                 1)) {
                const auto player_ready = [](GameObject_s *object) {
                    return object != NULL && object->apiobj.player_controlled &&
                           object->pad_gamepad->input_magnitude == 0.0f;
                };
                if (player_ready(Player[0]) || player_ready(Player[1])) {
                    selected_minikit = hub_minikitviewer_area;
                }
            }
            if (hub_minikitarea_time == 0.0f) {
                if (hub_minikitarea != selected_minikit) {
                    hub_minikitarea_opentime = 0.0f;
                }
                hub_minikitarea = selected_minikit;
            }
            f32 target = 0.0f;
            if (hub_minikitarea != -1 && hub_minikitarea == selected_minikit) {
                hub_minikitarea_opentime += FRAMETIME;
                target = 1.0f;
            }
            if (GetMenuID() == 14) {
                target = 1.0f;
            }
            hub_minikitarea_time = SeekLinearF(hub_minikitarea_time, target, FRAMETIME * 2.0f);
            if (hub_minikitarea_opentime >= 1.0f) {
                MakeMenuPacket();
                Hint_CancelCurrent();
                NewMenu(14, -1, -1);
                PlaySfxAndSetPitch(const_cast<char *>("JForcePush"), NULL, 1.5f);
            }
        }
    } else {
        hub_minikitarea = -1;
        hub_minikitarea_time = 0.0f;
        hub_minikitarea_opentime = 0.0f;
    }
    Hub_UpdateMiniKits(world);

    GIZBUILDIT_s *nearest_buildit = GizBuildIt_FindNearest(WORLD, Player[0], BUILDIT_FIND_ANY, ShadowMode);
    if (FadeSys.fade == 0.0f) {
        i32 selected_buildit = -1;
        for (i32 i = 0; HubAreaInfo[i].area_name != NULL && selected_buildit == -1; ++i) {
            GIZMO *gizmo = HubAreaInfo[i].bonus_gizmo;
            if (gizmo != NULL && GizmoGetOutput(world->gizmo_sys, gizmo, 0, 0) == 0 && Player[0] != NULL &&
                Player[0]->apiobj.player_controlled && Player[0]->field_0x7a5 != 0x2d) {
                GIZBUILDIT_s *buildit = static_cast<GIZBUILDIT_s *>(HubAreaInfo[i].bonus_gizmo->object);
                if ((buildit->availability_flags & GIZBUILDIT_AVAILABILITY_INTERACTING) != 0 ||
                    (nearest_buildit != NULL && nearest_buildit == buildit)) {
                    selected_buildit = i;
                }
            }
        }
        if (__builtin_expect(LevGizmo[0] != NULL, 1)) {
            if (selected_buildit == -1 && GizmoGetOutput(world->gizmo_sys, LevGizmo[0], 0, 0) == 0) {
                if (Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[0]->field_0x7a5 != 0x2d &&
                    ((LevBuildIt[0]->availability_flags & GIZBUILDIT_AVAILABILITY_INTERACTING) != 0 ||
                     (nearest_buildit != NULL && nearest_buildit == LevBuildIt[0]))) {
                    selected_buildit = 999;
                }
                // Original 0x1b7edc / 0x1b8880: the fountain displays one
                // progress piece per two gold bricks until all 160 are earned.
                const f32 gold_bricks = static_cast<f32>(Game.field_0x7c26[0]);
                if (gold_bricks < 160.0f) {
                    GizmoSetVisibility(world->gizmo_sys, LevGizmo[0], 0, 0);
                    GIZBUILDIT_s *buildit = static_cast<GIZBUILDIT_s *>(LevGizmo[0]->object);
                    const i32 visible_pieces = static_cast<i32>(static_cast<f32>(Game.field_0x7c26[0]) * 0.5f);
                    if (buildit->linked_buildit != NULL) {
                        buildit = buildit->linked_buildit;
                    }
                    for (i32 i = 0; i < buildit->anim_object_count; ++i) {
                        if (buildit->anim_objects[i] != NULL) {
                            NuSpecialSetVisibility(&buildit->anim_objects[i]->special, i < visible_pieces ? 1 : 0);
                        }
                    }
                } else if ((static_cast<GIZBUILDIT_s *>(LevGizmo[0]->object)->availability_flags &
                            GIZBUILDIT_AVAILABILITY_ACTIVE) == 0) {
                    GizmoActivate(world->gizmo_sys, LevGizmo[0], 1, 0);
                }
            } else if (GizmoGetOutput(world->gizmo_sys, LevGizmo[0], 0, 0) != 0) {
                Game.field_0x7c26[2] |= 0x80;
            }
        }
        if (hub_buildit_time == 0.0f) {
            hub_buildit = selected_buildit;
        }
        const f32 target = hub_buildit != -1 && hub_buildit == selected_buildit ? 1.0f : 0.0f;
        hub_buildit_time = SeekLinearF(hub_buildit_time, target, FRAMETIME * 2.0f);
    } else {
        hub_buildit_time = 0.0f;
        hub_buildit = -1;
    }
    if (Hub_UpdateAIFn != NULL) {
        Hub_UpdateAIFn(world);
    }

    if (hub_episode != -1 || hub_area != -1 || hub_minikitarea != -1 || hub_buildit != -1) {
        Hint_CancelCurrent();
    } else {
        const i32 active_menu = GetMenuID();
        if (active_menu != -1 && active_menu != 13 && (active_menu != 15 || hub_new_level == -1)) {
            Hint_CancelCurrent();
        }
    }
    ResetForceBack();
}

void Hub_MakeModelList() {
    Hub_ModelList[0] = {static_cast<i16>(PlayerID[0]), 1};
    Hub_ModelList[1] = {static_cast<i16>(PlayerID[1]), 1};

    i32 count = 2;
    if (HUB_ADATA != NULL && HUB_ADATA->hub_player_ids != NULL) {
        i16 *model_id = HUB_ADATA->hub_player_ids;
        while (count < 5 && *model_id != -1) {
            Hub_ModelList[count++] = {*model_id++, 1};
        }
    }
    Hub_ModelList[count].model_id = -1;

    PlayerList[0] = Hub_ModelList[0].model_id;
    PlayerList[1] = Hub_ModelList[1].model_id;
    PlayerList[2] = -1;
}

i32 Hub_PanelBusy() {
    if (hub_episode != -1 || hub_area != -1 || hub_minikitarea != -1 || hub_buildit != -1) {
        return 1;
    }

    if (GetMenuID() == 20 || GetMenuID() == 21 || GetMenuID() == 22 || GetMenuID() == 23) {
        return 1;
    }
    return 0;
}

