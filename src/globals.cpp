#include <stddef.h>

#include "globals.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nusound/nusound.h"
#include "gamelib/util/gamelib_util_types.h"

struct CUSTOMISER;
struct GIZAIMESSAGESYS_s;
char *ASCII_DOWN = const_cast<char *>("\xc2\xa3");
// Original defaults: LEGOASCII_DOWN is null; txt_UNKNOWN points to "?".
char *LEGOASCII_DOWN = NULL;
char *txt_UNKNOWN = const_cast<char *>("?");
u8 PlayerRGB[2][3] = {{0, 127, 255}, {0, 255, 0}};

// ----------------------------------------------------------------------
// Shared game globals, grouped by subsystem. See src/globals.h.
// ----------------------------------------------------------------------

// ------------------------------------------------------------------------
// Frame rate & timestep
// ------------------------------------------------------------------------
i32 PAL = 0;
f32 FRAMETIME = 0;
f32 DEFAULTFPS = 0;
f32 DEFAULTFRAMETIME = 0;
void *globalbuffer = NULL;
i32 MaxAnimJoints = 0;
u8 ForcePlayEndFrame = 0;
u8 ForceEulerToQuat = 0;
u8 QuatPushes[4] = {};
i32 NumQuatPushes = 0;
f32 MAXFRAMETIME = 0;
extern "C" {
    f32 partglobaltime = 0;
    u32 partseed = 0x5c0999;
    i32 g_signedinUser = -1;
}
MAIN_FRAME_COUNTERS_s MainFrameCounters = {};
i32 radios_playing = 0;
i32 last_chatter_sfx = -1;
i32 GAMERAND = 0x1f3ad27f;
BOLT_s Bolt[32] = {};
i32 i_bolt = 0;
f32 BOLT_OVERRIDE_PLAYERBOLTSPEED = 0.0f;
f32 BOLT_OVERRIDE_PLAYERBOLTDURATION = 0.0f;
u8 CutSceneCameraCTRL = 0;
f32 nusound_fade_start = 2.0f;
NUVEC nusound_special_positions[5];
f32 nusound_fade_end = 15.0f;
i32 (*SetSoundFadeDistCallBackFn)(WORLDINFO_s *world) = NULL;
USING_EXTRA_ACTIONS_FN UsingExtraActionsFn = NULL;

// ------------------------------------------------------------------------
// Super buffer / memory arena
// ------------------------------------------------------------------------
i32 SUPERBUFFERSIZE = 0x2EB8EEB;
VARIPTR permbuffer_base;
VARIPTR original_permbuffer_base;
VARIPTR superbuffer_end;
VARIPTR permbuffer_ptr;
VARIPTR permbuffer_end;

// ------------------------------------------------------------------------
// Title & display strings
// ------------------------------------------------------------------------
char prodcode[16] = {0};
char *iconname = "lego.ico";
char unicodename[64] = "LEGO Star Wars Saga";
const char *theEmptyString = "";

// ------------------------------------------------------------------------
// Episode / area counts
// ------------------------------------------------------------------------
i32 EPISODECOUNT = 0;
i32 AREACOUNT = 0;
AREADATA_s *LOSTTEMPLE_ADATA = NULL;

// ------------------------------------------------------------------------
// Game save state
// ------------------------------------------------------------------------
GAMESAVE_s Game = {0};
GAMESAVE_s BackupGame = {0};
STATUSCOLLECTLIST_s StatusCollectList = {};
u32 areaSuitBits = 0;
u8 *Game_LevelSave = NULL;
EPISODESAVE_s *Game_EpisodeSave = NULL;
u16 *Game_CompletionSave = NULL;
void (*CheckLostDataFn)(GIZMOBLOWUP_s *) = NULL;
MISSIONSAVE *Game_MissionSave = NULL;

// ------------------------------------------------------------------------
// Character customiser
// ------------------------------------------------------------------------
i32 Customiser_AccessoriesLoaded = 0;
i32 CUSTOMISER_USEBIGSCENES = 0;
f32 catwoman_stun_time = 2.0f;
CUSTOMISER *CharacterCustomiser = NULL;

// ------------------------------------------------------------------------
// Completion & bonus points
// ------------------------------------------------------------------------
i32 COMPLETIONPOINTS = 0;
i32 POINTS_PER_CHARACTER = 0;
i32 POINTS_PER_SUPERBONUSCOMPLETE = 0;
i32 POINTS_PER_TIMETRIAL = 0;
i32 POINTS_PER_STORY = 0;
i32 POINTS_PER_CHALLENGE = 0;
i32 POINTS_PER_MINIKIT = 0;
i32 POINTS_PER_TRUEJEDI = 0;
i32 POINTS_PER_REDBRICK = 0;
i32 POINTS_PER_MISSION = 0;
i32 POINTS_PER_CHEAT = 0;
i32 POINTS_PER_GOLDBRICK = 0;
i32 BOTHTRUEJEDIGOLDBRICKS = 0;
i32 SHOPGOLDBRICKS = 0;
i32 GOLDBRICKFORSUPERBONUS = 0;
i32 GOLDBRICKFORSUPERSTORY = 0;
i32 GOLDBRICKFORCHALLENGE = 0;
i32 POINTS_PER_SUPERSTORY = 0;
i32 GOLDBRICKPOINTS = 0;
i32 CompletionPointInfo[7] = {0};
i32 OldBonusScore[2] = {0};
i32 BonusScore[2] = {0};
i32 BonusCoinTotal = 0;
u32 BonusCoinTarget = 0;

// Gameplay panel layout and animation state.  The non-zero constants are the
// original panel coordinates; the animation values are reset by Panel_Clear.
f32 STATSPOSY = 0.745f;
f32 STATSPOS2Y = 1.255f;
f32 PANEL_HEARTY = -0.045f;
f32 PANEL_HITPOINTSX = 0.65f;
f32 COINTOTAL_SCOREDY = -0.005f;
f32 COINTOTAL_COINDX = 0.05f;
f32 COINTOTAL_COINSIZE = 0.5f;
f32 COINTOTAL_SCORESIZE = 0.5f;
f32 PANEL_COINADJUSTDY = -0.006f;
f32 PANEL_SCORESCALE = 0.375f;
f32 PANEL_COINSCALE_END = 0.35f;
f32 PANEL_SCOREX = 0.61f;
f32 PANEL_COINSCALE_START = 1.0f;
f32 PANEL_COINY = 0.045f;
f32 PANEL_COINX = 0.65f;
f32 CoinTotalScale = 0.0f;
f32 cointotal_x[2] = {0.0f, 0.0f};
i32 cointotal_i_obj[2] = {187, 187};
i32 SuperStoryEpisode = -1;
f32 SuperStoryTimer[4] = {0.0f, 0.0f, 0.0f, 0.0f};
u32 SuperStoryScore = 0;

f32 DrawMiniKitTime = 0.0f;
f32 MiniKitScale = 0.0f;
f32 DrawBuildUpTime = 0.0f;
f32 builduptime = 0.0f;
f32 BuildUpScale = 0.0f;
f32 DrawRedBrickTime = 0.0f;
f32 RedBrickScale = 0.0f;
f32 DrawCoinTotalTime = 0.0f;

// ------------------------------------------------------------------------
// Audio & music
// ------------------------------------------------------------------------
NUSOUND_FILENAME_INFO *MusicInfo = NULL;
NUSOUND_FILENAME_INFO *g_music = NULL;
u8 g_BackgroundUsedFogColour = 0;
i32 g_BackgroundColour = 0;
i32 SFX_MUSIC_COUNT = 0;
i32 NOSOUND = 0;
i32 NUSOUND_STREAM_3 = 0;
i16 AreaMusic = 0;
i32 LevMusicAction = 0;
i32 LevMusicAmbient = 0;
i32 LevMusicOtherAction = 0;
i32 LevMusicOtherAmbient = 0;
spacelevel_action_config_s Actions_DogFightA = {
    0.0f, 1.0f, 2, 10.0f, 0.0f, 7.5f, 1, 7.5f, 7.5f, 3, 0.0f, 0,
};
anakin_action_config_s Actions_AnakinA = {
    {0.0f, 1.0f, 2, 7.5f, 0.0f, 1.5f, 2, 7.5f, 7.5f, 1, 8.0f, 0}, 4, 8.0f, 8.0f, 0, {0, 0, 0, 0},
};
dogfight_doors_s DogFightDoors = {
    {
        {"door_to_a2", 33.036f, 11.5f},
        {"door_to_a3", 121.77f, 31.24f},
        {"door_to_b", 196.73f, 60.332f},
        {"door_to_b2", 334.37f, 97.5582f},
        {"door_to_c", 435.12f, 131.43f},
        {"door_to_c1", 508.68f, 147.05f},
        {"door_to_c2", 583.73f, 167.5f},
    },
    0,
};
f32 SpaceRumbleTimer = 0.0f;
EXPLOSION Explosion[8] = {};
i32 i_explosion = 0;

// ------------------------------------------------------------------------
// Camera
// ------------------------------------------------------------------------
NUCAMERA *pNuCam = NULL;
i32 (*GameCam_ObjLookingWithLeftStick)(GameObject_s *object) = nullptr;
i32 LookAtBoth = 0;
PLAYPLANE_s PlayPlane[6] = {};
i32 KEEPONSCREEN_SIDESONLY = 0;
NUVEC GunshipANorm = {};
u32 ZeroRTL[0x51] = {};
char *partdebris_name[64] = {
    "TRAINING_P",   "PART_MOUSE", "PART_BLACK_3", "DRAG_PART_1", "SMALL_PART_2", "RAT_PART",
    "BOULDER_PART", "POPCORN",    "PART_THROW",   "ENG_POP",     "DOOKU_PART",   "MAUL_P_1",
    "MAUL_P_2",     "MAUL_P_3",   "SPEAR_HIT",    "RED_BRICK",   "POD_ROCK",
};
u32 GAMEPAD_SKIP = GAMEPAD_BUTTON_START | GAMEPAD_BUTTON_JUMP | GAMEPAD_BUTTON_TAG;

// ------------------------------------------------------------------------
// Platform & device info
// ------------------------------------------------------------------------
i32 g_isLowestEndDevice = 0;
i32 g_isLowEndDevice = 0;
i32 g_isMidRangeDevice = 0;
i32 g_lowEndLevelBehaviour = 0;
i32 NOAICREATURES = 0;
u8 aicreature_sets_alive[16] = {};

// ------------------------------------------------------------------------
// Render / compatibility options
// ------------------------------------------------------------------------
u8 g_forceSysMemVbs = 0;
i32 Reflections_On = 1;
i32 (*MatrixReflection_CanOverrideFn)(f32) = NULL;
i32 disable_narrow_socks = 0;
i32 set_speedermode = 0;
nugspline_s *script_spline_selected = NULL;
f32 CutBorderScale = 0.0f;
i32 LEGOCAMMODE_DOORCUT = -1;
i32 LEGOCAMMODE_OBSTACLE = -1;
i32 ObstacleCamBorders = 0;
i32 texanimbits = 0;

// ------------------------------------------------------------------------
// Group scenes (NUGSCN)
// ------------------------------------------------------------------------
NUGSCN *vehicle_scene = NULL;

// ------------------------------------------------------------------------
// Gameplay timers & area state
// ------------------------------------------------------------------------
f32 DoubleScoreTime = 0.0f;
f32 TOGGLEHOLDTIME = 1.0f;
TIMER GameTimer;
TIMER GamePlayTimer;
TIMER OverallGamePlayTimer;
AREA_GLOBALS AreaGlobals = {};
i32 HIGHGAMEOBJECT = 0;
GameObject_s *Obj = NULL;
f32 AreaPickupGravity = 0.0f;
f32 HIGHJUMPHEIGHT = 0.0f;
TIMER AreaTimer;
f32 VehicleAreaRememberSpeed = 0;
nugspline_s *ObstacleCamSpl = NULL;
f32 ObstacleCamStart = 0.0f;
f32 ObstacleCamEnd = 0.0f;
f32 ObstacleCamTime = 0.0f;
f32 ObstacleCamBlendInTime = 0.0f;
f32 ObstacleCamBlendOutTime = 0.0f;
u16 ObstacleCamRotZ = 0;
NUVEC *ObstacleCamCutTgtPtr = NULL;
NUVEC *ObstacleCamCutCamPtr = NULL;
i32 ObstacleCamHoldUntilPlayersMove = 0;
u8 ObstacleCamAlwaysSnapAngles = 0;
f32 LevTime[5] = {0.0f};
i32 Lap = 0;
PART_s *Part = NULL;
i32 MAXPARTS = 0x20;
i32 i_part = -1;
u8 minikitCounter_A = 0;
u8 minikitCounter_C = 0;
DETONATOR_s Detonator[10] = {};

// ------------------------------------------------------------------------
// Bonus / arcade / challenge mode
// ------------------------------------------------------------------------
i32 BonusWinner = -1;
i32 BonusWinFlag = 0;
i32 ChallengeMode = 0;
TIMER ChallengeTimer;
i32 LSW1 = 0;
i32 LSW2 = 0;
i32 Arcade = 0;
i32 BuildUpTotal = 0;
i32 BuildUpDone = 0;
CUTINFO *newmode_cutinfo = NULL;
i32 LevelChange = 0;
GameObject_s *BombGenerator_PlayerBomb[2] = {NULL};
i32 BonusArea = 0;

// ------------------------------------------------------------------------
// Player & character objects
// ------------------------------------------------------------------------
i16 temp_yrot = 0;
i16 temp_xrot = 0;
i16 temp_zrot = 0;
f32 EShadY = 0.0f;
i32 CHARSHADOWS_ON = 1;
TERRAIN_LAYER_s TerLayer[17] = {
    {1.0f, 0, -1, 0}, {2.0f, 0, -1, 0}, {2.0f, 0, -1, 0}, {1.0f, 1, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0x20, -1, 0},
    {1.0f, 1, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0},
    {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0}, {1.0f, 0, -1, 0},
};
GameObject_s *player = NULL;
GameObject_s *player2 = NULL;
extern GameObject_s *CutDeadVehiclePlayer;
f32 avg_currentspeed_mul = 0.0f;
i32 pause_rndr_on = 0;
f32 pause_fade = 0.0f;
i32 wait_till_next_frame = 0;
GIZFORCE_s *force_array[4] = {0};
GameObject_s *ObiWan = NULL;
GameObject_s **game_objects = NULL;
i32 LedgeTerrain_On = 0;
i32 Grapples_Available = 1;
i32 SuperCarry_Bash = 0;
i32 SuperCarry_Jump = 0;
f32 PUNCHGAP = 0.3f;
f32 PUNCHCHARGAP = 0.6f;
i32 HINTS_ON = 1;

// ------------------------------------------------------------------------
// Character preview / free-play model lists
// ------------------------------------------------------------------------
i32 FreePlay = 0;
i32 Area_PlayerModelCount = 0;
i32 Area_StoryModelCount = 0;
i16 Area_PlayerModelList[18] = {0};
i32 Area_FreePlayModelCount = 0;
i16 Area_FreePlayModelList[104] = {0};
i32 Area_MissionModelCount = 0;
APICHARACTERMODELLIST_s Area_MissionModelList[49] = {0};
VARIPTR characterbuffer_ptr = {0};
VARIPTR characterbuffer_end = {0};
i32 CHARACTERBUFFERSIZE = 0x800000;
i32 permbuffer_size = 0;
VARIPTR superbuffer_base = {0};
VARIPTR superbuffer_ptr = {0};
i32 EDITBUFFERENDSIZE = 0;
VARIPTR editbuffer_end = {0};
APICHARACTERMODELLIST_s FreePlayModelList[52] = {0};
APICHARACTERMODELLIST_s Hub_ModelList[6] = {0};
i32 FreePlayModelCount = 0;
i32 FreePlayResidentCount = 0;
i32 FreePlayBonusCount = 0;
CHARCAT_s *CharCategory = NULL;
i32 CHARCATEGORYCOUNT = 0;
EXTRAMODEL ExtraModelList[1] = {{0}};
COLLECTION_s CharacterCollection = {};
COLLECTION_s VehicleCollection = {};
COLLECTION_s MiniKitCollection = {};
COLLECTION_s MasterCollection = {};
COLLECTION_s ShopCollection = {};
COLLECTION_s JediCollection = {};
COLLECTION_s BlasterCollection = {};
COLLECTION_s BountyHunterCollection = {};
f32 COLLECTION_DEFAULTSCALE = 0.6f;
extern i16 tLEVEL, tMODE, tPLAY;
extern i16 tARCADEMODE_BATTLE, tARCADEMODE_COLLECT, tARCADEMODE_HUNT;
ARCADEITEM_s ArcadeItem = {&tLEVEL, 0, 12, 0, &tMODE, 0, 3, 0, &tPLAY, 0, 1, 0};
ARCADE_MODE_s Arcade_Mode[3] = {
    {&tARCADEMODE_BATTLE, 50, 0x62},
    {&tARCADEMODE_COLLECT, 500000, 0x24},
    {&tARCADEMODE_HUNT, 0, 0x58},
};
GAME_CUSTOMISER_s *Game_Customiser = NULL;
AREASAVE_s *Game_AreaSave = NULL;
u8 *Game_CharacterSave = NULL;
APICHARACTERMODELLIST_s *CurrentCList = NULL;
APICHARACTERMODELLIST_s *CurrentStoryCList = NULL;
i32 CharacterDataLoad = 0;
i32 makefreeplaymodellist = 0;
i32 hub_freeplaysource = 0;
i16 id_DEFAULTCHARACTER[2] = {-1, -1};
i16 tBATMANSUIT = 0;
i16 tSHADOWSUIT = 0;
i16 tGLIDESUIT = 0;
i16 tDEMOLITIONSUIT = 0;
i16 tSONARSUIT = 0;
i16 tROBINSUIT = 0;
i16 tWATERSUIT = 0;
i16 tTECHNOLOGYSUIT = 0;
i16 tMAGNETSUIT = 0;
i16 tATTRACTSUIT = 0;
i32 CHARPAK = 0;

// ------------------------------------------------------------------------
// Level object tables
// ------------------------------------------------------------------------
i32 LEVOBJREF_FIRSTOBJ = -1;
i32 LEVOBJREF_LASTOBJ = -1;
i32 LEVOBJREF_FIRSTREFOBJ = -1;
i32 LEVOBJREF_LASTREFOBJ = -1;
LEVELOBJECT *ObjTabList = NULL;
i32 LEVELOBJECTCOUNT = 0;
i32 EXTRALEVELOBJECTCOUNT = 0;
i32 LEVELSPLINECOUNT = 0;
i32 KNOBS = -1;
i32 PLAYERHITPOINTS_2HEARTSIN1 = 1;
i32 drawbosshitpoints_2rows = 0;

i16 drawcharicon_hspecial_spin = 0;
f32 drawcharicon_hspecial_dz = 0.0f;
i32 drawcharicon_find = 0;
f32 drawcharicon_hspecial_scale = 1.0f;
i32 drawcharicon_i_panel = -1;
f32 PANEL3DMULY = 0.0f;
f32 PANEL3DMULX = 0.0f;
i32 LEGOOBJ_ICON_WEIRDO = -1;
i32 LEGOOBJ_ICON_QUESTION = -1;
i32 LEGOOBJ_GRAPPLE_HOOK = -1;
i32 LEGOOBJ_FLOORTARGET = -1;

// ------------------------------------------------------------------------
// Level / area data pointers (LDATA / ADATA)
// ------------------------------------------------------------------------
LEVELDATA *ANAKINSFLIGHTB_LDATA = NULL;
AREADATA *ANAKINSFLIGHT_ADATA = NULL;
AREADATA *ANEWHOPE_ADATA = NULL;
AREADATA *ASTEROIDCHASE_ADATA = NULL;
AREADATA *BATTLEOVERCORUSCANT_ADATA = NULL;
AREADATA *BLOCKADERUNNER_ADATA = NULL;
AREADATA *BONUSKASHYYYK_ADATA = NULL;
AREADATA *BONUSKAMINO_ADATA = NULL;
AREADATA *BONUSDAGOBAH_ADATA = NULL;
AREADATA *BOUNTYHUNTERPURSUIT_ADATA = NULL;
AREADATA *CLOUDCITYESCAPE_ADATA = NULL;
AREADATA *CLOUDCITYTRAP_ADATA = NULL;
AREADATA *CRUISER_ADATA = NULL;
AREADATA *DAGOBAH_ADATA = NULL;
AREADATA *DEATHSTARBATTLE_ADATA = NULL;
AREADATA *DEATHSTARBATTLE2_ADATA = NULL;
AREADATA *DEATHSTARESCAPE_ADATA = NULL;
AREADATA *DEATHSTARRESCUE_ADATA = NULL;
AREADATA *DOGFIGHT_ADATA = NULL;
AREADATA *E1CHARACTER_ADATA = NULL;
AREADATA *E2CHARACTER_ADATA = NULL;
AREADATA *E3CHARACTER_ADATA = NULL;
AREADATA *E4CHARACTER_ADATA = NULL;
AREADATA *E5CHARACTER_ADATA = NULL;
AREADATA *E6CHARACTER_ADATA = NULL;
AREADATA *E1VEHICLE_ADATA = NULL;
AREADATA *E2VEHICLE_ADATA = NULL;
AREADATA *E3VEHICLE_ADATA = NULL;
AREADATA *E4VEHICLE_ADATA = NULL;
AREADATA *E5VEHICLE_ADATA = NULL;
AREADATA *E6VEHICLE_ADATA = NULL;
AREADATA *EMPERORFIGHT_ADATA = NULL;
AREADATA *ENDORBATTLE_ADATA = NULL;
AREADATA *FACTORY_ADATA = NULL;
AREADATA *GRIEVOUS_ADATA = NULL;
AREADATA *GUNGAN_ADATA = NULL;
AREADATA *GUNSHIP_ADATA = NULL;
AREADATA *HOTHESCAPE_ADATA = NULL;
AREADATA *JABBASPALACE_ADATA = NULL;
AREADATA *JIMTEST_ADATA = NULL;
AREADATA *KASHYYYK_ADATA = NULL;
AREADATA *LEGOCITY_ADATA = NULL;
AREADATA *MAUL_ADATA = NULL;
AREADATA *MOSEISLEY_ADATA = NULL;
AREADATA *NEGOTIATIONS_ADATA = NULL;
AREADATA *NEWTOWN_ADATA = NULL;
AREADATA *RESCUE_ADATA = NULL;
AREADATA *RETAKE_ADATA = NULL;
AREADATA *SARLACCPIT_ADATA = NULL;
AREADATA *SPEEDERCHASE_ADATA = NULL;
AREADATA *TATOOINE_ADATA = NULL;
AREADATA *TEMPLE_ADATA = NULL;
AREADATA *VEHICLES_ADATA = NULL;
AREADATA *VADER_ADATA = NULL;
LEVELDATA *ASTEROIDCHASEA_LDATA = NULL;
LEVELDATA *ASTEROIDCHASEB_LDATA = NULL;
LEVELDATA *ASTEROIDCHASEC_LDATA = NULL;
LEVELDATA *ASTEROIDCHASED_LDATA = NULL;
LEVELDATA *ASTEROIDCHASEMITRO_LDATA = NULL;
LEVELDATA *BLOCKADERUNNERB_LDATA = NULL;
LEVELDATA *BLOCKADERUNNERC_LDATA = NULL;
LEVELDATA *BLOCKADERUNNERD_LDATA = NULL;
LEVELDATA *BONUS_GUNSHIPA_LDATA = NULL;
LEVELDATA *BONUS_GUNSHIPB_LDATA = NULL;
AREADATA *BONUS_GUNSHIP_ADATA = NULL;
LEVELDATA *BOUNTYHUNTERPURSUITA_LDATA = NULL;
LEVELDATA *BOUNTYHUNTERPURSUITB_LDATA = NULL;
LEVELDATA *BOUNTYHUNTERPURSUITC_LDATA = NULL;
LEVELDATA *BOUNTYHUNTERPURSUITD_LDATA = NULL;
LEVELDATA *BOUNTYHUNTERPURSUITE_LDATA = NULL;
LEVELDATA *CLOUDCITYESCAPEA_LDATA = NULL;
LEVELDATA *CLOUDCITYESCAPEB_LDATA = NULL;
LEVELDATA *CLOUDCITYESCAPEC_LDATA = NULL;
LEVELDATA *CLOUDCITYTRAPA_LDATA = NULL;
LEVELDATA *CLOUDCITYTRAPB_LDATA = NULL;
LEVELDATA *CLOUDCITYTRAPC_LDATA = NULL;
LEVELDATA *CLOUDCITYTRAPOUTRO_LDATA = NULL;
LEVELDATA *CRUISERA_LDATA = NULL;
LEVELDATA *CRUISERB_LDATA = NULL;
LEVELDATA *CRUISERC_LDATA = NULL;
LEVELDATA *CRUISERD_LDATA = NULL;
LEVELDATA *CRUISERE_LDATA = NULL;
LEVELDATA *CRUISERG_LDATA = NULL;
LEVELDATA *DAGOBAHA_LDATA = NULL;
LEVELDATA *DAGOBAHB_LDATA = NULL;
LEVELDATA *DAGOBAHC_LDATA = NULL;
LEVELDATA *DAGOBAHD_LDATA = NULL;
LEVELDATA *DAGOBAHE_LDATA = NULL;
LEVELDATA *DEATHSTAR2BATTLEA_LDATA = NULL;
LEVELDATA *DEATHSTAR2BATTLEB_LDATA = NULL;
LEVELDATA *DEATHSTAR2BATTLED_LDATA = NULL;
LEVELDATA *DEATHSTAR2BATTLEE_LDATA = NULL;
LEVELDATA *DEATHSTAR2BATTLEF_LDATA = NULL;
LEVELDATA *DEATHSTAR2BATTLEG_LDATA = NULL;
LEVELDATA *DEATHSTAR2BATTLEMIDTRO_LDATA = NULL;
LEVELDATA *DEATHSTARBATTLEA_LDATA = NULL;
LEVELDATA *DEATHSTARBATTLEB_LDATA = NULL;
LEVELDATA *DEATHSTARBATTLEC_LDATA = NULL;
LEVELDATA *DEATHSTARBATTLED_LDATA = NULL;
LEVELDATA *DEATHSTARBATTLEMIDTRO_LDATA = NULL;
LEVELDATA *DEATHSTARESCAPEA_LDATA = NULL;
LEVELDATA *DEATHSTARESCAPEB_LDATA = NULL;
LEVELDATA *DEATHSTARESCAPEC_LDATA = NULL;
LEVELDATA *DEATHSTARESCAPED_LDATA = NULL;
LEVELDATA *DEATHSTARRESCUEA_LDATA = NULL;
LEVELDATA *DEATHSTARRESCUEB_LDATA = NULL;
LEVELDATA *DEATHSTARRESCUEC_LDATA = NULL;
LEVELDATA *DEATHSTARRESCUED_LDATA = NULL;
LEVELDATA *DEATHSTARRESCUEE_LDATA = NULL;
LEVELDATA *DOGFIGHTA_LDATA = NULL;
LEVELDATA *DOOKUC_LDATA = NULL;
LEVELDATA *DOOKUOUTRO_LDATA = NULL;
AREADATA *DOOKU_ADATA = NULL;
LEVELDATA *E1CHARACTERBONUSA_LDATA = NULL;
LEVELDATA *E2VEHICLEBONUSA_LDATA = NULL;
LEVELDATA *EMPERORFIGHTA_LDATA = NULL;
LEVELDATA *ENDORBATTLEA_LDATA = NULL;
LEVELDATA *ENDORBATTLEB_LDATA = NULL;
LEVELDATA *ENDORBATTLEC_LDATA = NULL;
LEVELDATA *ENDORBATTLED_LDATA = NULL;
LEVELDATA *FACTORYB_LDATA = NULL;
LEVELDATA *FACTORYD_LDATA = NULL;
LEVELDATA *FACTORYF_LDATA = NULL;
LEVELDATA *FACTORYG_LDATA = NULL;
LEVELDATA *GRIEVOUSA_LDATA = NULL;
LEVELDATA *GUNGAN_A_LDATA = NULL;
LEVELDATA *GUNGAN_B_LDATA = NULL;
LEVELDATA *GUNSHIPA_LDATA = NULL;
LEVELDATA *GUNSHIPB_LDATA = NULL;
LEVELDATA *HOTHBATTLEA_LDATA = NULL;
LEVELDATA *HOTHBATTLEB_LDATA = NULL;
LEVELDATA *HOTHBATTLEC_LDATA = NULL;
LEVELDATA *HOTHBATTLED_LDATA = NULL;
LEVELDATA *HOTHBATTLEE_LDATA = NULL;
LEVELDATA *HOTHBATTLEOUTRO_LDATA = NULL;
AREADATA *HOTHBATTLE_ADATA = NULL;
AREADATA *HOTH_ADATA = NULL;
LEVELDATA *HOTHESCAPEA_LDATA = NULL;
LEVELDATA *HOTHESCAPEB_LDATA = NULL;
LEVELDATA *HOTHESCAPEC_LDATA = NULL;
LEVELDATA *HOTHESCAPED_LDATA = NULL;
LEVELDATA *HUB_LDATA = NULL;
LEVELDATA *JABBASPALACEA_LDATA = NULL;
LEVELDATA *JABBASPALACEB_LDATA = NULL;
LEVELDATA *JABBASPALACED_LDATA = NULL;
LEVELDATA *JABBASPALACEE_LDATA = NULL;
LEVELDATA *JABBASPALACE_OUTRO_LDATA = NULL;
AREADATA *JEDI_ADATA = NULL;
LEVELDATA *JEDI_B_LDATA = NULL;
LEVELDATA *JEDI_OUTRO_LDATA = NULL;
LEVELDATA *KAMINOA_LDATA = NULL;
LEVELDATA *KAMINOC_LDATA = NULL;
LEVELDATA *KAMINOD_LDATA = NULL;
LEVELDATA *KAMINOE_LDATA = NULL;
LEVELDATA *KAMINOF_LDATA = NULL;
LEVELDATA *KAMINOOUTRO_LDATA = NULL;
struct AREADATA_s *KAMINO_ADATA = NULL;
LEVELDATA *KASHYYYKA_LDATA = NULL;
LEVELDATA *KASHYYYKB_LDATA = NULL;
LEVELDATA *KASHYYYKC_LDATA = NULL;
LEVELDATA *KASHYYYKD_LDATA = NULL;
LEVELDATA *LastLData = NULL;
i32 last_area = -1;
LEVELDATA *LEGOCITY_LDATA = NULL;
LEVELDATA *MAULA_LDATA = NULL;
LEVELDATA *MAULB_LDATA = NULL;
LEVELDATA *MAULD_LDATA = NULL;
LEVELDATA *MAULE_LDATA = NULL;
LEVELDATA *MAULF_LDATA = NULL;
LEVELDATA *MOSEISLEYA_LDATA = NULL;
LEVELDATA *MOSEISLEYB_LDATA = NULL;
LEVELDATA *MOSEISLEYC_LDATA = NULL;
LEVELDATA *MOSEISLEYD_LDATA = NULL;
LEVELDATA *MOSEISLEYE_LDATA = NULL;
LEVELDATA *NB_KAMINOALDATA = NULL;
LEVELDATA *NEGOTIATIONSA_LDATA = NULL;
LEVELDATA *NEGOTIATIONSB_LDATA = NULL;
LEVELDATA *NEGOTIATIONSC_LDATA = NULL;
LEVELDATA *NEWTOWN_LDATA = NULL;
LEVELDATA *PODRACEA_LDATA = NULL;
LEVELDATA *PODRACEB_LDATA = NULL;
LEVELDATA *PODRACEC_LDATA = NULL;
LEVELDATA *PODRACEOUTRO1_LDATA = NULL;
LEVELDATA *PODRACESTATUS_LDATA = NULL;
AREADATA *PODRACE_ADATA = NULL;
LEVELDATA *PODSPRINTA_LDATA = NULL;
AREADATA *PODSPRINT_ADATA = NULL;
LEVELDATA *RESCUEA_LDATA = NULL;
LEVELDATA *RESCUEB_LDATA = NULL;
LEVELDATA *RESCUEC_LDATA = NULL;
LEVELDATA *RESCUEE_LDATA = NULL;
LEVELDATA *RETAKEB_LDATA = NULL;
LEVELDATA *RETAKEE_LDATA = NULL;
LEVELDATA *RETAKEG_LDATA = NULL;
LEVELDATA *RETAKEINTRO1_LDATA = NULL;
LEVELDATA *RETAKEINTRO2_LDATA = NULL;
LEVELDATA *RETAKEINTRO3_LDATA = NULL;
LEVELDATA *SARLACCPITA_LDATA = NULL;
LEVELDATA *SARLACCPITB_LDATA = NULL;
LEVELDATA *SARLACCPITC_LDATA = NULL;
LEVELDATA *SENATEA_LDATA = NULL;
AREADATA *SENATE_ADATA = NULL;
LEVELDATA *SPEEDERCHASEA_LDATA = NULL;
LEVELDATA *STATUS_LDATA = NULL;
LEVELDATA *TATOOINEA_LDATA = NULL;
LEVELDATA *TATOOINEB_LDATA = NULL;
LEVELDATA *TATOOINEC_LDATA = NULL;
LEVELDATA *TATOOINED_LDATA = NULL;
LEVELDATA *TATOOINEE_LDATA = NULL;
AREADATA *UTAPAU_ADATA = NULL;
LEVELDATA *TEMPLEA_LDATA = NULL;
LEVELDATA *TEMPLEB_LDATA = NULL;
LEVELDATA *TEMPLEC_LDATA = NULL;
LEVELDATA *TEMPLESTATUS_LDATA = NULL;
LEVELDATA *TITLES_LDATA = NULL;
TRENCHRUN_s trenchrun = {};
SceneObjectHelper theSceneObjectHelper;
LEVELDATA *VADERA_LDATA = NULL;
LEVELDATA *VADERB_LDATA = NULL;
LEVELDATA *VADERC_LDATA = NULL;

// ------------------------------------------------------------------------
// Level hack data & progress
// ------------------------------------------------------------------------
void *LevelProgressData = NULL;
GameAnimSysProgress gameanimsysprogress = {0, 0, NULL};
void *LevelHackData = NULL;
void *OldLevelHackData = NULL;
i32 LevelHackSize = 0;
i32 LevelHackSendTimer = 0;

// ------------------------------------------------------------------------
// Level load keywords
// ------------------------------------------------------------------------
nufpcomjmp_s *Level_ConfigBeforeLoad_GameKeywords = NULL;
nufpcomjmp_s *Level_ConfigAfterLoad_GameKeywords = NULL;

// ------------------------------------------------------------------------
// Level streaming & loading
// ------------------------------------------------------------------------
LEVELDATA *NewLData = NULL;
i32 grab_screen_image = 0;
i32 waiting_for_new_level = 0;
i32 LOADEROFF = 0;
i32 no_more_loads = 0;
i32 other_level = 0;
i32 other_level_override = 0;
i32 CUTSTOPGAME = 0;
i32 CUTSKIPLOCK = 0;
void *CutStopInfo = NULL;
f32 WaitingForLevelTime = 0;
f32 WaitingForCharacterTime = 0;
i32 LevelLoadCount = 0;
LEVELLOAD_s LevelLoad[12];
i32 new_level_from_menu = 0;
// The original .data initialises BGLOAD to 1 (background loading enabled).
i32 BGLOAD = 1;
i32 reset_restart = 0;
i32 come_from_an_editor = 0;
i32 CanDrawZipUpSwirls = 0;
i32 newlevelfrommenu_newmenuid = -1;
i32 newlevelfrommenu_newmenuy = -1;
i32 NextArea_FreePlay = 0;

// ------------------------------------------------------------------------
// Level script arrays (Lev*)
// ------------------------------------------------------------------------
i32 LevFlag[4] = {0};
nuhspecial_s LevHSpecial[88] = {};
NUMTX LevMtx = {};
f32 LevAlpha = 0.0f;
f32 TitlesAlpha = 0.0f;
f32 newgamealpha = 0.0f;
i32 newgamefade = 0;
f32 newgamewait = 0.0f;
i32 newgame_menudrawoff = 0;
i32 netnewgame = 0;
i32 MenuLoadOccurred = 0;
i32 MenuSaveOccurred = 0;
i32 Tag_DoneFirst = 0;
i32 Tag_DoneAny = 0;
i32 LevSfxFlag[4] = {0};
nuinstanim_s *LevInstAnim[12] = {0};
AIAREA_s *LevArea[4] = {0};
i32 LevPathNodes[8] = {0};
void *LevPathCnx[16] = {0};
GameObject_s *LevGameObject[8] = {0};
PART_s *LevGamePart[8] = {0};
GIZAIMESSAGE_s *LevAIMessage[8] = {0};
GIZBUILDIT_s *LevBuildIt[4] = {};
AILOCATOR_s *LevelLocator = NULL;
GIZOBSTACLE_s *LevGizObst[8] = {0};
GIZMOBLOWUP_s *LevBlowUp[5] = {0};
GIZMO *LevGizmo[12] = {0};
i32 LevForce = 0;
i32 LevSfxId[4] = {0};
NUGSPLINE *LevelCodeSpline[8] = {NULL};
GIZFORCE_s *LevGizForce[4] = {0};
void *LevAIPathNode[4] = {0};
i32 LevBoltIgnorePlatIds[2] = {0};
i32 LevPlatID[2] = {0};
u64 LevHSpecialExists = 0;
i32 LevPathCnxDir = 0;
i32 LevDeaths = 0;
u8 LevLock[5] = {0};
i32 LevSafePlatID[2] = {0};

// ------------------------------------------------------------------------
// Network / multiplayer (podrace, gunship, mines)
// ------------------------------------------------------------------------
RETAKEGNETPACKET_s *retakeg_netpacket = NULL;
i16 trooper_boltid[2] = {0};
i8 trooper_side[10] = {0};
nuhspecial_s *hothtroopers = NULL;
i32 troopers_gdeb[4] = {0};
i32 TimingBarSet = 0;
u32 client_mines[0xc5] = {0};
MINESYS_s minesys;
i32 nethost = 0;
i32 clients_mines_bitfield[2] = {0};
i32 pod_mines_bitfield[2] = {0};
i32 mine_count = 0;
PODSPRINT_s podsprint;
PODRACENETPACKET_s *podrace_netpacket = NULL;
float gungan_a_time_Normal = 0.5f;
float gungan_a_time_LowEnd = 0.75f;
i32 active_neutral_count = 0;
i32 active_baddy_count = 0;
i16 id_STAP = -1;
// Unmangled globals from the original binary (C linkage). Initial values
// match the .data image of res/libTTapp.so.
extern "C" {
    i32 podrace_section = 0;
    i32 max_nsnipers = 0;
    i32 PodRace_nsnipers = 0;
    SNIPER_s PodRace_snipers[10];
    float PodRace_sniper_fire_time = 0.25f;
    float PodRace_sniper_start_fire_radius = 90.0f;
    float PodRace_sniper_fire_radius = 2.5f;
    float PodRace_sniper_fire_range_time = 2.0f;
    float pod_roll[2] = {0.0f, 0.0f};
    float pod_roll_target[2] = {0.0f, 0.0f};
    float pod_animtime[2] = {1.0f, 1.0f};
}
PODSPRINTNETPACKET_s *podsprint_netpacket = NULL;
i32 gunship_player_dead = 0;
void *kaminoe_netpacket = NULL;
void *factoryb_netpacket = NULL;
struct BONUSGUNSHIP_NETPACKET_s *bonusgunshipb_netpacket = NULL;
u8 dookuC_nodesNeedUpdating = 0;
struct vader_c_s vader_c = {0};
CUTINFO *factoryb_cut = NULL;
void *factoryb_conveyor_stopped_msg = NULL;
i32 bonus_gunship_store_progress_flag = 0;
float podanimendframe = 0.0f;
float pacemaker_alpha_table[0x8000] = {0};
GIZAIMESSAGESYS_s *gizaimessagesys = NULL;
i32 loadareadata_loadlevel = 0;

// ------------------------------------------------------------------------
// Loading screen (LoadPerm) globals
// ------------------------------------------------------------------------
// Original .data @0x622ef0: PermDataLoaded starts at 1.
i32 PermDataLoaded = 1;

// --- LoadPermData working set ---
// Original bss objects carved out of the startup TU; sizes from nm -S.
LEVELOBJECT ObjTab[0x2ee] = {
    {4, 0, 0, "save_icon"},
    {1, 0, 0, "parallax"},
    {1, 0, 0, "legal"},
    {1, 0, 0, "status_01"},
    {1, 0, 0, "status_02"},
    {1, 0, 0, "status_03"},
    {1, 0, 0, "status_04"},
    {1, 0, 0, "status_05"},
    {1, 0, 0, "status_06"},
    {1, 0, 0, "rod"},
    {1, 0, 0, "phone"},
    {1, 0, 0, "slave1_level"},
    {0, 0, 0, "machinegun"},
    {0, 0, 0, "blaster"},
    {0, 0, 0, "walkie_talkie"},
    {0, 0, 0, "pistolgrey"},
    {0, 0, 0, "pistolchrome"},
    {0, 0, 0, "lightsabrehandle"},
    {0, 0, 0, "doublelightsabrehandle"},
    {0, 0, 0, "bentlightsabre"},
    {0, 0, 0, "gun"},
    {0, 0, 0, "bowcaster"},
    {0, 0, 0, "blaster_blue"},
    {0, 0, 0, "submachinegun"},
    {0, 0, 0, "poop"},
    {0, 0, 0, "peri"},
    {0, 0, 0, "spear"},
    {0, 0, 0, "spearblack"},
    {0, 0, 0, "storm_target"},
    {0, 0, 0, "bounty_target"},
    {0, 0, 0, "lever"},
    {0, 0, 0, "lever_base"},
    {0, 0, 0, "lever_nob1"},
    {0, 0, 0, "lever_nob2"},
    {0, 0, 0, "lever_nob3"},
    {0, 0, 0, "lever_nob4"},
    {0, 0, 0, "lever_nob5"},
    {0, 0, 0, "lever_nob6"},
    {0, 0, 0, "lever_nob7"},
    {0, 0, 0, "lever_nob8"},
    {0, 0, 0, "lever_nob9"},
    {0, 0, 0, "lever_glow"},
    {0, 0, 0, "lever_on"},
    {0, 0, 0, "lever_off"},
    {0, 0, 0, "hack_control"},
    {0, 0, 0, "hack_door1"},
    {0, 0, 0, "hack_door2"},
    {0, 0, 0, "winch_base"},
    {0, 0, 0, "winch_bracket"},
    {0, 0, 0, "winch"},
    {0, 0, 0, "batarang"},
    {0, 0, 0, "robinarang"},
    {0, 0, 0, "bullet"},
    {0, 1, 0, "pistolflash"},
    {0, 0, 0, "signal_base"},
    {0, 0, 0, "bat_signal_off"},
    {0, 0, 0, "bat_signal_on"},
    {0, 0, 0, "bat_signal_light"},
    {0, 0, 0, "robin_signal_off"},
    {0, 0, 0, "robin_signal_on"},
    {0, 0, 0, "robin_signal_light"},
    {0, 0, 0, "grapple_point"},
    {0, 0, 0, "grapple_point1"},
    {0, 0, 0, "tightrope_base1"},
    {0, 0, 0, "tightrope_base2"},
    {0, 0, 0, "metal_bit1"},
    {0, 0, 0, "metal_bit2"},
    {0, 0, 0, "metal_bit3"},
    {0, 0, 0, "metal_bit4"},
    {0, 0, 0, "metal_bit5"},
    {0, 0, 0, "metal_bit6"},
    {0, 0, 0, "metal_bit7"},
    {0, 0, 0, "attract_o_matic"},
    {0, 0, 0, "transform1a"},
    {0, 0, 0, "transform1b"},
    {0, 0, 0, "ledge_1"},
    {0, 0, 0, "ledge_2"},
    {0, 0, 0, "ledge_3"},
    {0, 0, 0, "ledge_4"},
    {0, 0, 0, "ledge_5"},
    {0, 0, 0, "ledge_6"},
    {0, 0, 0, "door_1_l"},
    {0, 0, 0, "door_1_r"},
    {0, 0, 0, "zipup_hook"},
    {0, 0, 0, "zipup_hook1"},
    {0, 0, 0, "zipup_target"},
    {0, 0, 0, "target"},
    {0, 0, 0, "Spinner_default_Base"},
    {0, 0, 0, "Spinner_default_Arm"},
    {0, 0, 0, "carrot"},
    {1, 0, 0, "basketball"},
    {0, 0, 1, "blasterbolt"},
    {0, 0, 1, "blasterboltglow"},
    {0, 0, 1, "blasterbolt_red"},
    {0, 0, 1, "blasterboltglow_red"},
    {0, 0, 1, "blasterbolt_green"},
    {0, 0, 1, "blasterboltglow_green"},
    {0, 0, 1, "blasterbolt_blue"},
    {0, 0, 1, "blasterboltglow_blue"},
    {0, 0, 1, "double_blasterBolt"},
    {0, 0, 1, "double_blasterBoltglow"},
    {0, 0, 1, "lightsabrered"},
    {0, 0, 1, "lightsabreglowred"},
    {0, 0, 1, "lightsabregreen"},
    {0, 0, 1, "lightsabreglowgreen"},
    {0, 0, 1, "lightsabreblue"},
    {0, 0, 1, "lightsabreglow"},
    {0, 0, 1, "lightsabrepurple"},
    {0, 0, 1, "lightsabreglowpurple"},
    {0, 0, 1, "doublelightsabrered"},
    {0, 0, 1, "doublelightsabreglow"},
    {3, 0, 1, "destroyershield"},
    {3, 0, 1, "destroyershield_red"},
    {5, 0, 1, "xwingbolt"},
    {5, 0, 1, "xwingboltglow"},
    {5, 0, 1, "laser_green"},
    {5, 0, 1, "laser_green_glow"},
    {0, 0, 1, "green_blob"},
    {0, 0, 1, "green_blob_glow"},
    {5, 0, 1, "xwing_thruster"},
    {5, 0, 1, "xwing_thruster_glow"},
    {5, 0, 1, "photontorpedo"},
    {5, 0, 1, "photontorpedoglow"},
    {5, 0, 1, "photontorpedoglow1"},
    {5, 0, 1, "jedi_starfighter_blasterBolt"},
    {5, 0, 1, "jedi_starfighter_blasterBoltglow"},
    {0, 0, 2, "blasterbolt_ref"},
    {0, 0, 2, "blasterboltglow_ref"},
    {0, 0, 2, "blasterbolt_red_ref"},
    {0, 0, 2, "blasterboltglow_red_ref"},
    {0, 0, 2, "blasterbolt_green_ref"},
    {0, 0, 2, "blasterboltglow_green_ref"},
    {0, 0, 2, "blasterbolt_blue_ref"},
    {0, 0, 2, "blasterboltglow_blue_ref"},
    {0, 0, 2, "double_blasterbolt_ref"},
    {0, 0, 2, "double_blasterboltGlow_ref"},
    {0, 0, 2, "lightsabrered_ref"},
    {0, 0, 2, "lightsabreglowred_ref"},
    {0, 0, 2, "lightsabregreen_ref"},
    {0, 0, 2, "lightsabreglowgreen_ref"},
    {0, 0, 2, "lightsabreblue_ref"},
    {0, 0, 2, "lightsabreglow_ref"},
    {0, 0, 2, "lightsabrepurple_ref"},
    {0, 0, 2, "lightsabreglowpurple_ref"},
    {0, 0, 2, "doublelightsabrered_ref"},
    {0, 0, 2, "doublelightsabreglow_ref"},
    {3, 0, 2, "destroyershield_ref"},
    {3, 0, 2, "destroyershield_red_ref"},
    {0, 0, 2, "xwingbolt_ref"},
    {0, 0, 2, "xwingboltglow_ref"},
    {0, 0, 2, "laser_green_ref"},
    {0, 0, 2, "laserglow_green_ref"},
    {0, 0, 2, "green_blob_ref"},
    {0, 0, 2, "green_blob_glow_ref"},
    {0, 0, 2, "xwing_thruster_ref"},
    {0, 0, 2, "xwing_thruster_glow_ref"},
    {0, 0, 2, "photontorpedo_ref"},
    {0, 0, 2, "photontorpedo_glow_ref"},
    {0, 0, 2, "photontorpedo_glow1_ref"},
    {5, 0, 2, "jedi_starfighter_blasterBolt_ref"},
    {5, 0, 2, "jedi_starfighter_blasterBoltglow_ref"},
    {3, 0, 0, "question_icon"},
    {3, 0, 0, "mouse_glow"},
    {3, 0, 0, "silhouette_icon"},
    {3, 0, 0, "silhouette_icon1"},
    {0, 0, 0, "icon_back_green"},
    {0, 0, 0, "icon_back_blue"},
    {0, 0, 0, "icon_back_neutral"},
    {0, 0, 0, "icon_back_gold"},
    {0, 0, 0, "coin_side1"},
    {0, 0, 0, "coin_side2"},
    {0, 0, 0, "coin_side3"},
    {0, 0, 0, "coin_side4"},
    {0, 0, 0, "coin_side5"},
    {0, 0, 0, "coin_side6"},
    {0, 0, 0, "coin_side7"},
    {0, 0, 0, "coin_side8"},
    {0, 0, 0, "coin_side9"},
    {0, 0, 0, "coin_side10"},
    {0, 0, 0, "silver_pan_coin1"},
    {0, 0, 0, "silver_pan_coin2"},
    {0, 0, 0, "silver_pan_coin3"},
    {0, 0, 0, "silver_pan_coin4"},
    {0, 0, 0, "silver_coin1"},
    {0, 0, 0, "silver_coin2"},
    {0, 0, 0, "silver_coin3"},
    {0, 0, 0, "silver_coin4"},
    {0, 0, 0, "gold_pan_coin1"},
    {0, 0, 0, "gold_pan_coin2"},
    {0, 0, 0, "gold_pan_coin3"},
    {0, 0, 0, "gold_pan_coin4"},
    {0, 0, 0, "gold_coin1"},
    {0, 0, 0, "gold_coin2"},
    {0, 0, 0, "gold_coin3"},
    {0, 0, 0, "gold_coin4"},
    {0, 0, 0, "blue_pan_coin1"},
    {0, 0, 0, "blue_pan_coin2"},
    {0, 0, 0, "blue_pan_coin3"},
    {0, 0, 0, "blue_pan_coin4"},
    {0, 0, 0, "blue_coin1"},
    {0, 0, 0, "blue_coin2"},
    {0, 0, 0, "blue_coin3"},
    {0, 0, 0, "blue_coin4"},
    {0, 0, 0, "heart"},
    {0, 0, 0, "flat_heart"},
    {0, 0, 0, "silver_heart"},
    {0, 0, 0, "mini_kit_pickup"},
    {0, 0, 0, "char_pickup"},
    {0, 0, 0, "plop"},
    {0, 0, 0, "power_up_glow"},
    {0, 0, 0, "red_brick"},
    {0, 0, 0, "gold_brick"},
    {0, 0, 0, "info"},
    {0, 0, 0, "purple_coin1"},
    {0, 0, 0, "purple_coin2"},
    {0, 0, 0, "purple_coin3"},
    {0, 0, 0, "purple_coin4"},
    {0, 0, 0, "purple_pan_coin1"},
    {0, 0, 0, "purple_pan_coin2"},
    {0, 0, 0, "purple_pan_coin3"},
    {0, 0, 0, "purple_pan_coin4"},
    {0, 0, 0, "ripple_green"},
    {0, 0, 0, "ripple_green1"},
    {0, 0, 0, "ripple_red"},
    {0, 0, 0, "ripple_red1"},
    {0, 0, 0, "ripple_blue"},
    {0, 0, 0, "ripple_blue1"},
    {0, 0, 0, "ripple_purple"},
    {0, 0, 0, "ripple_purple1"},
    {0, 0, 0, "zip_ring"},
    {0, 0, 0, "crate"},
    {0, 0, 0, "moustache"},
    {0, 0, 0, "jango_rocket"},
    {0, 0, 0, "thermal"},
    {0, 0, 0, "thermal_red_off"},
    {0, 0, 0, "thermal_red_on"},
    {0, 0, 0, "bomb_main"},
    {0, 0, 0, "bomb_off"},
    {0, 0, 0, "bomb_red_off"},
    {0, 0, 0, "bomb_red_on"},
    {3, 0, 0, "part_boulder"},
    {3, 0, 0, "pebble_ewok"},
    {3, 0, 0, "photontorpedo_ewok"},
    {3, 0, 0, "photontorpedoglow_ewok"},
    {3, 0, 0, "pebble_wicket"},
    {3, 0, 0, "photontorpedo_wicket"},
    {3, 0, 0, "photontorpedoglow_wicket"},
    {3, 0, 0, "umbrella1"},
    {3, 0, 0, "umbrella2"},
    {3, 0, 0, "twoface_coin"},
    {0, 0, 0, "hat_1"},
    {0, 0, 0, "hat_3"},
    {0, 0, 0, "hat_4"},
    {0, 0, 0, "hat_5"},
    {0, 0, 0, "stormTrooperHelmet"},
    {0, 0, 0, "bountyHelmet"},
    {0, 0, 0, "C3PO_base"},
    {0, 0, 0, "C3PO_pic"},
    {0, 0, 0, "TC14_pic"},
    {0, 0, 0, "C3PO_static"},
    {0, 0, 0, "C3PO_light_on"},
    {0, 0, 0, "C3PO_light_off"},
    {0, 0, 0, "R2_base"},
    {0, 0, 0, "R2_base1"},
    {0, 0, 0, "R2_pic"},
    {0, 0, 0, "R4_pic"},
    {0, 0, 0, "R2_static"},
    {0, 0, 0, "R2_light_on"},
    {0, 0, 0, "R2_light_off"},
    {0, 0, 0, "Bounty_base"},
    {0, 0, 0, "Bounty_pic"},
    {0, 0, 0, "Bounty_static"},
    {0, 0, 0, "Bounty_light_on"},
    {0, 0, 0, "Bounty_light_off"},
    {0, 0, 0, "Bounty_cam"},
    {0, 0, 0, "Bounty_Target"},
    {0, 0, 0, "Storm_base"},
    {0, 0, 0, "Storm_pic"},
    {0, 0, 0, "Storm_static"},
    {0, 0, 0, "Storm_light_on"},
    {0, 0, 0, "Storm_light_off"},
    {0, 0, 0, "Storm_cam"},
    {0, 0, 0, "Storm_Target"},
    {0, 0, 0, "Hat_machine_base"},
    {0, 0, 0, "Hat_machine_down"},
    {0, 0, 0, "Hat_machine_out"},
    {0, 0, 0, "Hat_machine_on"},
    {0, 0, 0, "Hat_machine_off"},
    {0, 0, 0, "lever_helmet"},
    {0, 0, 0, "Hat_machine_down1"},
    {0, 0, 0, "photon2"},
    {0, 0, 0, "photon1"},
    {0, 0, 0, "zip_flower1"},
    {0, 0, 0, "zip_flower2"},
    {0, 0, 0, "zip_flower3"},
    {5, 0, 0, "FalconGlow"},
    {5, 0, 0, "ATAT_PART_1"},
    {1, 0, 0, "vd"},
    {1, 0, 0, "bvd"},
    {1, 0, 0, "dtf"},
    {1, 0, 0, "ca"},
    {1, 0, 0, "blasterbolt"},
    {1, 0, 0, "blasterglow"},
    {1, 0, 0, "blasterbolt_red"},
    {1, 0, 0, "blasterglow_red"},
    {1, 0, 0, "blasterbolt_blue"},
    {1, 0, 0, "blasterglow_blue"},
    {1, 0, 0, "missile"},
    {5, 0, 0, "arrow"},
    {5, 0, 0, "death_star"},
    {5, 0, 0, "lap1"},
    {5, 0, 0, "lap2"},
    {5, 0, 0, "lap3"},
    {0, 0, 0, "Network_Icon"},
    {0, 0, 0, "Network_Icon_Glow"},
    {1, 0, 0, "fmv_blank"},
    {1, 0, 0, "code_blank"},
    {1, 0, 0, "tool_blank"},
    {1, 0, 0, "fmv"},
    {1, 0, 0, "shop_film1b"},
    {1, 0, 0, "shop_film1"},
    {6, 0, 0, "pc_button1"},
    {6, 0, 0, "pc_button2"},
    {6, 0, 0, "pc_button3"},
    {6, 0, 0, "pc_button4"},
    {0xff, 0, 0, NULL}, // terminator
};
LEVELSPLINE SplTab[26] = {
    {NULL, "start", 2, 0, -1, -1},
    {NULL, "start_cam", 2, 0, -1, -1},
    {NULL, "point_cam", 2, 2, -1, -1},
    {NULL, "finish_line", 2, 2, -1, -1},
    {NULL, "cam_start", 2, 2, -1, -1},
    {NULL, "split", 2, 2, -1, -1},
    {NULL, "map_cam", 2, 0, -1, -1},
    {NULL, "map_look", 2, 0, -1, -1},
    {NULL, "e1_entrance", 2, 2, -1, -1},
    {NULL, "e2_entrance", 2, 2, -1, -1},
    {NULL, "e3_entrance", 2, 2, -1, -1},
    {NULL, "e4_entrance", 2, 2, -1, -1},
    {NULL, "e5_entrance", 2, 2, -1, -1},
    {NULL, "e6_entrance", 2, 2, -1, -1},
    {NULL, "jabba_entrance1", 2, 2, -1, -1},
    {NULL, "char_cam", 2, 2, -1, -1},
    {NULL, "Turn_Around_1", 2, 0x400, -1, -1},
    {NULL, "Turn_Around_2", 2, 0x400, -1, -1},
    {NULL, "custard", 4, 4, -1, -1},
    {NULL, "bonus_zone_1", 3, 0, -1, -1},
    {NULL, "bonus_zone_2", 3, 0, -1, -1},
    {NULL, "bonus_zone_3", 3, 0, -1, -1},
    {NULL, "bonus_zone_4", 3, 0, -1, -1},
    {NULL, "bonus_zone_5", 3, 0, -1, -1},
    {NULL, "mission_cam", 2, 2, -1, -1},
    {NULL, NULL, 0, 0, 0, 0},
};
CHARCATEGORY LSW_CharCategory[10] = {
    {{"Jedi"}, {0x8}, {0}},
    {{"JediBaddie"}, {0xc}, {0}},
    {{"BountyHunter"}, {0x1100080}, {0}},
    {{"Teleport"}, {0x40000}, {0}},
    {{"HighJump"}, {0}, {0x400000}},
    {{"Astromech"}, {0x40}, {0}},
    {{"Protocol"}, {0x20}, {0}},
    {{"ZipUp"}, {0x100080}, {0}},
    {{"Blaster"}, {0x80}, {0}},
    {{NULL}, {0}, {0}},
};
extern i16 tCHEAT_EXTRATOGGLE;
extern i16 tCHEAT_POO;
extern i16 tCHEAT_DISGUISE;
extern i16 tCHEAT_DAISYCHAINS;
extern i16 tCHEAT_C3POBITS;
extern i16 tCHEAT_TOWDEATHSTAR;
extern i16 tCHEAT_SILHOUETTES;
extern i16 tCHEAT_BEEPBEEP;
extern i16 tCHEAT_SUPERGONK;
extern i16 tCHEAT_POOMONEY;
extern i16 tCHEAT_WALKIETALKIEDISABLE;
extern i16 tCHEAT_POWERBRICKDETECTOR;
extern i16 tCHEAT_SUPERSLAP;
extern i16 tCHEAT_FORCEZIPUP;
extern i16 tCHEAT_COINMAGNET;
extern i16 tCHEAT_DISARMTROOPERS;
extern i16 tCHEAT_CHARACTERSTUDS;
extern i16 tCHEAT_PERFECTDEFLECT;
extern i16 tCHEAT_EXPLODINGBLASTERBOLTS;
extern i16 tCHEAT_FORCEPULL;
extern i16 tCHEAT_VEHICLESMARTBOMB;
extern i16 tCHEAT_SUPERASTROMECH;
extern i16 tCHEAT_SUPERJEDISLAM;
extern i16 tCHEAT_SUPERTHERMALDETONATOR;
extern i16 tCHEAT_DEFLECTBOLTS;
extern i16 tCHEAT_DARKSIDE;
extern i16 tCHEAT_SUPERBLASTERS;
extern i16 tCHEAT_FASTFORCE;
extern i16 tCHEAT_PURPLEFORCE;
extern i16 tCHEAT_TRACTORBEAM;
extern i16 tCHEAT_INVINCIBILITY;
extern i16 tCHEAT_X2;
extern i16 tCHEAT_SELFDESTRUCT;
extern i16 tCHEAT_FASTBUILD;
extern i16 tCHEAT_X4;
extern i16 tCHEAT_REGENERATE;
extern i16 tCHEAT_MINIKITDETECTOR;
extern i16 tCHEAT_X6;
extern i16 tCHEAT_SUPERZAPPER;
extern i16 tCHEAT_ROCKETS;
extern i16 tCHEAT_X8;
extern i16 tCHEAT_SUPEREWOKCATAPULT;
extern i16 tCHEAT_INFINITETORPEDOS;
extern i16 tCHEAT_X10;

#define CHEAT_ENTRY(name_, text_, code_, price_, extra_, flag_)                                                        \
    {const_cast<char *>(name_),  &text_, 0, 0, 0, 0xff, -1, const_cast<char *>(code_), price_,                         \
     const_cast<char *>(extra_), flag_}

CHEAT Cheat[45] = {
    CHEAT_ENTRY("extratoggle", tCHEAT_EXTRATOGGLE, "VX8FZ6", 25000, "shop_tool1", 0x100),
    CHEAT_ENTRY("poo", tCHEAT_POO, "F4035S", 8000, "shop_tool2", 0),
    CHEAT_ENTRY("disguises", tCHEAT_DISGUISE, "4HBIHK", 10000, "shop_tool3", 0),
    CHEAT_ENTRY("daisychains", tCHEAT_DAISYCHAINS, "2J0VGC", 5000, "shop_tool4", 0),
    CHEAT_ENTRY("c3pobits", tCHEAT_C3POBITS, "RTI8PX", 10000, "shop_tool5", 0),
    CHEAT_ENTRY("towdeathstar", tCHEAT_TOWDEATHSTAR, "YDQYJR", 5000, "shop_tool6", 0),
    CHEAT_ENTRY("silhouettes", tCHEAT_SILHOUETTES, "J2ZFT7", 10000, "shop_silhouette", 0x1),
    CHEAT_ENTRY("beepbeep", tCHEAT_BEEPBEEP, "V1VV95", 7500, "shop_beepbeep", 0),
    CHEAT_ENTRY("supergonk", tCHEAT_SUPERGONK, "K0HOMF", 50000, "shop_supergonk", 0),
    CHEAT_ENTRY("poomoney", tCHEAT_POOMONEY, "1J2G9Z", 50000, "shop_poomoney", 0x200000),
    CHEAT_ENTRY("walkietalkiedisable", tCHEAT_WALKIETALKIEDISABLE, "9DIHAR", 5000, "shop_walkietalkie", 0),
    CHEAT_ENTRY("powerbrickdetector", tCHEAT_POWERBRICKDETECTOR, "0SY75X", 62500, "shop_powerbrick", 0x40000),
    CHEAT_ENTRY("superslap", tCHEAT_SUPERSLAP, "YHXD63", 5000, "shop_superslap", 0),
    CHEAT_ENTRY("forcezipup", tCHEAT_FORCEZIPUP, "GPHF03", 15000, "shop_forcegrapple", 0),
    CHEAT_ENTRY("coinmagnet", tCHEAT_COINMAGNET, "FE0YXY", 100000, "shop_coinmagnet", 0x238000),
    CHEAT_ENTRY("disarmtroopers", tCHEAT_DISARMTROOPERS, "D0FSCN", 50000, "shop_disarmtroopers", 0),
    CHEAT_ENTRY("characterstuds", tCHEAT_CHARACTERSTUDS, "PSA7PM", 75000, "shop_characterstuds", 0x200000),
    CHEAT_ENTRY("perfectdeflect", tCHEAT_PERFECTDEFLECT, "HTWHIS", 20000, "shop_perfectdeflect", 0x100000),
    CHEAT_ENTRY("explodingblasterbolts", tCHEAT_EXPLODINGBLASTERBOLTS, "GYL04B", 20000, "shop_explodingbolts", 0),
    CHEAT_ENTRY("forcepull", tCHEAT_FORCEPULL, "7XR7Z1", 12000, "shop_forcepull", 0),
    CHEAT_ENTRY("vehiclesmartbomb", tCHEAT_VEHICLESMARTBOMB, "OYS2JP", 15000, "shop_vehiclesmart", 0),
    CHEAT_ENTRY("superastromech", tCHEAT_SUPERASTROMECH, "0OJO5O", 10000, "shop_superastromech", 0),
    CHEAT_ENTRY("superjedislam", tCHEAT_SUPERJEDISLAM, "6DT2CK", 11000, "shop_superjedislam", 0),
    CHEAT_ENTRY("superthermaldetonator", tCHEAT_SUPERTHERMALDETONATOR, "ZURPDI", 25000, "shop_superthermal", 0),
    CHEAT_ENTRY("deflectbolts", tCHEAT_DEFLECTBOLTS, "TPF1LL", 100000, "shop_deflectbolts", 0xb0000),
    CHEAT_ENTRY("darkside", tCHEAT_DARKSIDE, "3O1DUB", 25000, "shop_darkside", 0),
    CHEAT_ENTRY("superblasters", tCHEAT_SUPERBLASTERS, "CRX1LO", 15000, "shop_tool7", 0x32002),
    CHEAT_ENTRY("fastforce", tCHEAT_FASTFORCE, "QTDYVG", 40000, "shop_tool8", 0x412000),
    CHEAT_ENTRY("supersabres", tCHEAT_PURPLEFORCE, "3X7303", 40000, "shop_tool9", 0x12400),
    CHEAT_ENTRY("tractorbeam", tCHEAT_TRACTORBEAM, "LEMPCP", 15000, "shop_tool10", 0x20000),
    CHEAT_ENTRY("invincibility", tCHEAT_INVINCIBILITY, "4T2X0G", 500000, "shop_tool11", 0x32080),
    CHEAT_ENTRY("scorex2", tCHEAT_X2, "R26FOR", 625000, "shop_tool13", 0x230004),
    CHEAT_ENTRY("selfdestruct", tCHEAT_SELFDESTRUCT, "5VPUHC", 25000, "shop_tool12", 0),
    CHEAT_ENTRY("fastbuild", tCHEAT_FASTBUILD, "E7ZZLD", 30000, "shop_tool14", 0x16000),
    CHEAT_ENTRY("scorex4", tCHEAT_X4, "YNHS94", 1250000, "shop_tool15", 0x200008),
    CHEAT_ENTRY("regenerate", tCHEAT_REGENERATE, "JEHZU6", 150000, "shop_tool16", 0x33000),
    CHEAT_ENTRY("minikitdetector", tCHEAT_MINIKITDETECTOR, "WGXC8Y", 125000, "shop_tool17", 0x200),
    CHEAT_ENTRY("scorex6", tCHEAT_X6, "YW9S6L", 2500000, "shop_tool18", 0x200010),
    CHEAT_ENTRY("superzapper", tCHEAT_SUPERZAPPER, "2T7WBD", 14000, "shop_tool19", 0x2000),
    CHEAT_ENTRY("rockets", tCHEAT_ROCKETS, "1P9CGR", 20000, "shop_tool20", 0x2000),
    CHEAT_ENTRY("scorex8", tCHEAT_X8, "VYWFEV", 5000000, "shop_tool21", 0x200020),
    CHEAT_ENTRY("superewokcatapult", tCHEAT_SUPEREWOKCATAPULT, "ALQFGF", 25000, "shop_tool22", 0),
    CHEAT_ENTRY("infinitetorpedos", tCHEAT_INFINITETORPEDOS, "3Z3AFX", 25000, "shop_tool23", 0),
    CHEAT_ENTRY("scorex10", tCHEAT_X10, "OC7VIL", 10000000, "shop_tool24", 0x200040),
    {},
};

#undef CHEAT_ENTRY
CHARVARIANT CharVariants_Game[23] = {
    {"weirdo"},       {"stormtrooper"}, {"fett"},       {"macewindu"}, {"battledroid"},
    {"wookiee"},      {"obiwankenobi"}, {"leia"},       {"clone"},     {"rebel"},
    {"tie"},          {"lando"},        {"luke"},       {"hansolo"},   {"padme"},
    {"walker_2legs"}, {"walker_4legs"}, {"hoverdroid"}, {"critter"},   {"naboostarfighter"},
    {"henchman"},     {NULL},           {NULL},
};
MemoryManager theMemoryManager;
#include "legoapi/menus/core/lsw_text_data.inc"
TEXTCRAWL_s TextCrawl_LSW = {&tCHAPTER, &tVEHICLEBONUS, &tCHARACTERBONUS, 3};

void *theGameThings = NULL;
void *theThingManager = NULL;

NUGSCN *saveicon_scene = NULL;
NUGSCN *button_scene = NULL;

FadeSystem *pFadeInfo = NULL;

// Cut-scene / gameplay hook wiring (original .data function pointers).
i32 (*CutScene_StartFn)(CUTINFO *) = NULL;
void (*CutScene_PreUpdateFn)(CUTINFO *) = NULL;
void (*CutScene_PostUpdateFn)(void) = NULL;
void (*CutScene_StoppedFn)(CUTINFO *) = NULL;
i32 (*CutScene_ReplaceCharacterModelFn)(CUTINFO *, NUGCUTCHAR_s *) = NULL;
i32 (*InitBolt_AddMomentumType)(BOLT_s *, GameObject_s *, nuvec_s *) = NULL;
i32 (*Bolt_HitPlatFn)(BOLT_s *) = NULL;
i32 (*Bolt_HitCustomFn)(BOLT_s *, nuvec_s *) = NULL;
void (*GizObstacle_SetDefaultSFXFn)(void *, GIZOBSTACLE_s *) = NULL;
// Original bss @0x6a3f54 / @0x6a3f50.
volatile i32 LoadPerm_LanguageSelect = 0;
i32 LoadPerm_StringsLoaded = 0;
// Original bss @0x124f9c0.
i32 menu_flash = 0;
f32 game_pulse = 0.0f;
f32 global_pulse = 0.0f;
// Original .data @0x667cb0 / @0x667cc0.
i32 IntroText_TextID = -1;
i32 LANGUAGECOUNT = 6;
// Original .data @0x667ce0: default language list, entries of {language, 0}
// (8 bytes per entry); Text_LanguageList points at it (@0x667d30).
LANGUAGEDATA Text_LanguageList_Default[6] = {{1, 0}, {2, 0}, {4, 0}, {5, 0}, {3, 0}, {8, 0}};
LANGUAGEDATA *Text_LanguageList = Text_LanguageList_Default;
// Original .data @0x667ca0/@0x667ca4.
f32 INTROTEXT_Y = 0.175f;
f32 INTROTEXT_SCALE = 0.79f;

// BSS 0x127c200 / 0x124f6f0 — QFont & string table pointers
char **TTab = nullptr;
vufnt_s *QFont2D = nullptr;
vufnt_s *QFont2DButtons = nullptr;
vufnt_s *QFont3D = nullptr;
vufnt_s *QFont2DZ = nullptr;
vufnt_s *QFont2DLower = nullptr;
vufnt_s *QFont3DZ = nullptr;
f32 QFont3DTime = 0.0f;
vufnt_s *SmartTextFont = nullptr;
i32 create_qfont3d = 0;
i32 create_qfont2dz = 0;
i32 create_qfont2dlower = 0;
i32 create_qfont3dz = 0;

// ------------------------------------------------------------------------
// Cutscene & system misc
// ------------------------------------------------------------------------
i32 BeenAttacked = 0;
FadeSystem FadeSys;
i32 Paused = 0;
f32 PauseMenus_X;
i32 PauseMenus_Align;
u8 MENUEXITR = 0xff;
u8 MENUEXITG = 0xbf;
u8 MENUEXITB;
extern i32 from_save_and_exit;
u8 MENULOSTR1 = 0xbf;
u8 MENULOSTG1 = 0x5f;
u8 MENULOSTB1;
u8 MENULOSTA1 = 0x80;
u8 MENULOSTR2 = 0xff;
u8 MENULOSTG2 = 0x1f;
u8 MENULOSTB2;
u8 MENULOSTA2 = 0x80;
i32 pause_i_pad = -1;
i32 LEGOMENU_NEWGAME = -1;
i32 LEGOMENU_PAUSEMAIN = -1;
i32 LEGOMENU_PAUSECUT = -1;
i32 LEGOMENU_CREDITS = -1;
i32 MiniCutCam = 0;
i32 LEGOHINT_BUILD = -1;
i32 LEGOHINT_PUSHBLOCKS = -1;
i32 LEGOHINT_FREEPLAYTOGGLE = -1;
i32 WeaponInOut_NoAIJediSfx = 0;
i32 LEGOSPL_START = -1;
i32 LEGOSPL_SPLIT = 0;
GAMECUTSCENES_s game_cutscenes;
float MiscTime = 0.0f;
u32 ResetBits = 0;
i32 AreaDataLoaded = 1;
i32 Level = 0;

// ------------------------------------------------------------------------
// Main game loop state (shared via batman.h; read/written by NuMain).
// Sizes/types match the original binary's .data/.bss layout.
// ------------------------------------------------------------------------
f32 AddCoinDelay[2] = {0};
i32 adaptivedifficulty[3] = {0};
i32 back_rgba[2] = {0};
TIMER BonusTimer = {0};
TIMER JoinInTimer = {0};
TIMER PauseTimer = {0};
f32 brickimpactwait = 0.0f;
i32 BURNOUTON = 1;
VARIPTR characterbuffer_base = {0};
f32 chattersfxwait = 0.0f;
i32 clear_screen_onstill = 1;
f32 coinimpactwait = 0.0f;
i32 COMPLEXSHADOWS = 1;
i32 CUTDRAWWORLD = 0;
i32 CutSceneWaiting = 0;
i32 dagobah_training = 0;
i32 DoubleScore = 0;
i32 game_keepmergeaction = 0;
i32 JointRotation_On = 0;
MAKELAYERLISTFN MakeLayerList = NULL;
i32 DRAWCMODELCALLS = 0;
i32 editor_active = 0;
i32 enable_zero_frametime = 0;
i32 FinishLoop_On = 1;
i32 finishloop_backdroponly = 0;
i32 noscenespecials = 0;
i32 portals_enabled = 1;
i32 portal_special_objects = 1;
u8 PortalVisiFlags[0x271] = {0};
LANGUAGEDATA Game_LanguageList[7] = {{1, 0}, {2, 0}, {4, 0}, {5, 0}, {3, 0}, {8, 0}, {-1, 0}};
OPTIONSSAVE *Game_OptionsSave = NULL;
i32 (*GamePads_IgnoreInputFn)(void) = NULL;
void (*PauseGame_ExtraCodeFn)(void) = NULL;
void (*ResumeGame_ExtraCodeFn)(void) = NULL;
// Original bss @0x127bd00. Selected by ReadPad's successful normal path.
struct nupad_s *pActivePad = NULL;
i32 g_introState = 1;
f32 g_val = 0.0f;
i32 highallocaddr = 0;
i32 HubMainRenderTimeHack = 0;
AREADATA *LastAData = NULL;
i32 loadareacharacters_loadedlevel = 0;
f32 MainRenderTargetTime = 1.0f;
f32 MainRenderTime = 1.0f;
i32 memcard_autosavedisabled = 0;
i32 memcard_autosaveenabled = 0;
i32 menu_i_pack = -1;
i32 newlevel_resumecutaudio = 1;
i32 NewMode = 0;
i32 nurndr_tritot_this_frame = 0;
void (*NuSoundAppTerminateCallback)(void) = NULL;
volatile i32 nuvideo_global_vbcnt = 0;
i32 PANELOFF = 0;
i32 party_cant_be_under_cover = 0;
i32 peak_poly_count = 0;
i32 PlayTrailer = -1;
NUVEC plr_lastpos = {0};
f32 OFFSCREEN_CATCHUP_TIME = 1.0f;
f32 drop_back_in_timer = 0.0f;
i32 poly_count = 0;
i32 RAYCASTCALLS = 0;
i32 reset_area = 0;
i32 reset_load = 1;
i32 ResetOldFStop = 1;
ripple_set_s *ripples = NULL;
f32 sabrerubwait = 0.0f;
i32 save_paused = 0;
i32 screendump = 0;
i32 SHADOWCALLS = 0;
numtl_s *ShadowMat = NULL;
i32 ShadowMode = 0;
STATUSPACKET_s StatusPacket = {0};
u8 status_plr_active[8] = {0};
i32 SuperStory = 0;
OPTIONSSAVE TempOptions = {};
i32 TERRAINCALLS = 0;
f32 tieoffsfxwait = 0.0f;
f32 tieonsfxwait = 0.0f;
i32 waiting_for_character = -1;

f32 fakeanimendframe[1];
f32 fakeanimframe[1];
f32 ai_moveradius = 0.1f;
f32 antinode_time = 1.0f;
f32 antinode_reverse_time = 1.0f;
i32 new_retreat = 1;
i32 mechAutoJumpFlags = 0;
i32 mechAutoJumpCantReachFlags = 0;
f32 testAutoJumpXZCanDisplayRangeSqr = 4.0f;
f32 testAutoJumpXZCanUseRangeSqr = 0.25f;
i32 ai_fighting = 0;
f32 aitol = 0.5f;
f32 DEFAULT_MOVE_RANGE = 0.0f;
u64 _0xffffffffffffffff = ~static_cast<u64>(0);
f32 engagefiretime = 2.0f;
f32 idealgoalrange = 1.5f;

// Shared symbols recovered from the original global data surface.
i32 NetPaused = 0;
f32 mtl_animation_speed_scale = 1.0f;
u16 script_mask = 0xffff;
i32 (*GizObstacle_CheckExcludeFlagsFn)(GIZOBSTACLE_s *, GameObject_s *) = NULL;
