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
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "nu2api/nu3d/nuspecial.h"
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
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/render/fx/parts.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/items/objects/grabber.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/render/core/screen.h"
#include <stdlib.h>
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx.h"
#include <math.h>
#include "batman.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/actions/character/specialmoves.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/props/objects/signal.h"
#include "gameframework/saveload.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/render/core/rtl.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include <float.h>
#include "editor/edpath.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/render/core/gameliball.h"
#include <stddef.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include <cfloat>
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/actions/movement/pushblocks.h"
#include "legoapi/characters/core/characters.h"
#include "legoapi/gizmos/door/gizspinner.h"
#include "legoapi/items/fx/ripples.h"
#include "legoapi/lib/text.h"
#include "nu2api/nu3d/android/nubridge_android.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/nucore/android/stubs_android.h"

extern "C" char ConfigBuffer[0x10000];

// These are extern (U) in the original level.cpp.o — defined here as stubs
// until the original defining file is decompiled.
LEVELDATA *levelconfig_ldata = NULL;

extern "C" {
    struct LEVELDATA_s *PODRACELEVELS[11]; // Arrival1-4, Intro, B, C, A, Outro1, Outro2, Status
}

static i32 Pictures_NumLevels;
static NUVEC titlesstartpos;

LEVELDATA *LDataList = NULL;

i32 LEVELCOUNT;

LEVELFIXUP LevFixUp;

LEVELDATA *ANEWHOPELEVELS[4];
void Credits_Load(WORLDINFO_s *, variptr_u *, variptr_u *);

extern void (*WorldInfo_InitMenuFn)(WORLDINFO *, i32 *, i32 *);
volatile i32 abort_load = 0;
char ConfigBuffer[0x10000];
i32 numtl_force_mipmode = 0;
i32 GAMEDEMO = 0;
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
i16 PlayerList[8];
i32 bonusmodearcade = 0;
extern BATARANG_s Batarang[8];
extern u32 BackUpPlayers[2162];

extern WORLDINFO WorldInfo[2];
WORLDINFO *WORLD = &WorldInfo[0];

extern "C" {
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

struct APICHARACTERMODELLIST_s;

extern "C++" volatile i32 abort_load;
extern "C" i16 id_ANAKINJEDISCARRED;
extern "C" i16 id_HANINCARBONITE;
extern APICHARACTERMODELLIST_s FreePlayModelList[52];
extern APICHARACTERMODELLIST_s Hub_ModelList[6];
extern AREADATA *E1CHARACTER_ADATA;
extern AREADATA *E2CHARACTER_ADATA;
extern AREADATA *E3CHARACTER_ADATA;
extern AREADATA *E4CHARACTER_ADATA;
extern AREADATA *E5CHARACTER_ADATA;
extern AREADATA *E6CHARACTER_ADATA;
extern i16 tTOUCHTOSTART;

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

struct bgprocinfo_s;
static __used__ void LoadAreaData(bgprocinfo_s *);
extern i32 LevFlag[4];

extern i16 tALLMINIKITSBUILT;

extern i32 LevFlag[4]; // shared per-level 16-byte state scratch

struct AISYS_s;

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

APICHARACTERMODELLIST_s PermModelList[] = {{-1, 0}};

NUMTL *lightningmat;
extern "C" NUMTL *DebMat[10];

void SecurityDoors_InitTerrain(WORLDINFO_s *world);

extern "C" i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern "C" i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

struct nuhspecial_s;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern "C" i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct GameObject_s;
struct WORLDINFO_s;
struct debinftype;

struct EXTRAMODEL;

struct EDCREATURE_s;
struct APIOBJECT_s;

extern "C" {
extern i16 id_HANINCARBONITE;
extern i16 id_ANAKINJEDISCARRED;
extern i16 id_IMPERIALSHUTTLEPILOT;
extern i16 id_IMPERIALOFFICER;
extern i16 id_GRANDMOFFTARKIN;

}

extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);

typedef isize __attribute__((may_alias)) NuGCutWord;

extern i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;
extern i16 id_ANAKINJEDISCARRED;
extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);

extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);
} // extern "C"

struct AIPATH_s;
struct eduimenu_s;
struct eduiitem_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *models, u32 model_flags, u32 game_flags, i32 require_hats,
                         i32 reject_flag_40);

extern "C" {
    extern NUMTL *DebMat[10];
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

}

extern "C" {
    extern NUMTL *DebMat[10];
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern NUMTL *DebMat[];
}

static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
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

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);
static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern EdInputContext * Input;
extern f32 Scale;
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

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern NUMTL *DebMat[];
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;

extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
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
} // extern "C"

union variptr_u;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);

extern void (*FS_Callback)(char *, char *);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" {
extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

extern NUMTL *DebMat[10];

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

void SaveSystemInitialise(i32 slots, void *makeSaveHash, void *save, i32 saveSize, i32 saveCount,
                          void (*drawSaveIcon)(void), void *extradata, i32 extradataSize);

extern "C" {
void TextRegisterButtonMapFn(void (*fn)(char *, char *));
void TextRegisterPulseTimerFn(f32 (*fn)(f32));
}

extern "C" {
    extern NUMTL *DebMat[10];
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern NUMTL *DebMat[];
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern NUMTL *DebMat[];
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

extern "C" {
extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

extern "C" {
void TextRegisterButtonMapFn(void (*fn)(char *, char *));

void TextRegisterPulseTimerFn(f32 (*fn)(f32));

}

extern i16 tCHALLENGECOMPLETE, tMINIKIT;

extern char codechars[37];
extern char usercode[6];
extern i16 tSELECT, tBACK, tBUY, tPLAY, tSELECTING;

extern "C" {
extern void *(*DefragCreateInstFn)(void *, NUGCUTSCENE_s *, VARIPTR *);

}

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

#include "legoapi/ai/core/ai_sys_stubs_types.h"
} // extern "C"

// Callback storage is declared in reverse linked-BSS order because the
// original GCC 4.7 -O3 object emits these definitions in reverse source order.
extern "C" {
void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *));
} // extern "C"

extern "C" {
void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *));

}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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
        if (alpha > 0.0f) {
            if (NuSpecialExistsFn(&LevHSpecial[14]) != 0) {
                draw_mtx.m31 = -0.15f;
                NuSpecialDrawAtAlpha(&LevHSpecial[14], &draw_mtx, newgamealpha * 0.5f * alpha * LevTime[0]);
            }
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
    extrasavealpha = 1.0f;
    if (NuSpecialExistsFn(&LevHSpecial[0]) != 0) {
        i32 menu_id = GetMenuID();
        f32 target =
            (MenuLoadOccurred == 0 && MenuSaveOccurred == 0 && (menu_id == -1 || menu_id == 0 || menu_id == 1))
                ? 1.0f
                : 0.0f;
        LevAlpha = SeekLinearF(LevAlpha, target, FRAMETIME + FRAMETIME);
    }

    TitlesAlpha = SeekLinearF(TitlesAlpha, 1.0f, FRAMETIME + FRAMETIME);
    LevTime[1] = (GetMenuID() == 0 && GameTimer.time_elapsed >= 4.0f) ? 1.0f : 0.0f;
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
    i32 i;
    for (i = 0; i < 12; ++i) {
        if (NuStrICmp(progress->disabled_effect_names[i], name) == 0)
            break;
    }
    if (i < 12) {
        if (visible != 0) {
            progress->disabled_effect_names[i][0] = '\0';
            return 2;
        }
        return 3;
    }
    if (visible != 0)
        return 0;
    for (i = 0; i < 12; ++i) {
        if (progress->disabled_effect_names[i][0] == '\0')
            break;
    }
    if (i < 12)
        NuStrCpy(progress->disabled_effect_names[i], name);
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
    if (Area != last_area || Area == -1) {
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

        if (HUB_ADATA->index == Area) {
            if (character_load_active && AreaDataLoaded != 0 && waiting_for_character == -1) {
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
            }
        } else {
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
                if (VEHICLES_ADATA != NULL && VEHICLES_ADATA->index == Area) {
                    skip_text_scroll = true;
                    draw_touch_prompt = false;
                } else if ((GamePad[0].buttons_pressed &
                            (GAMEPAD_JUMP | GAMEPAD_START | GAMEPAD_SPECIAL | GAMEPAD_ACTION | GAMEPAD_TAG)) != 0 ||
                           (GamePad[1].buttons_pressed &
                            (GAMEPAD_JUMP | GAMEPAD_START | GAMEPAD_SPECIAL | GAMEPAD_ACTION | GAMEPAD_TAG)) != 0 ||
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
                const f32 wobble_sin =
                    NuTrigTable[(static_cast<i32>(NuFmod(LoadTime, 1.0f) * 65536.0f) >> 1) & 0x7fff];
                if (HUB_ADATA == NULL || HUB_ADATA->index != Area) {
                    fade_scene_active = true;
                } else if (CharacterDataLoad != 0 && icon_time > 0.0f && icon_time < icons_end_time) {
                    const f32 wobble = wobble_sin * 0.15f;
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

        if (fade_scene_active) {
            NuRndrEndScene();
            FadeSys.Update();
            FadeSys.Draw();
            NuRndrBeginScene(-1);
            fade_scene_active = true;
        }

        static_cast<ThingManager *>(theGameThings)->DisplayThings(NULL);
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
        strcpy(ai_name, "ai");
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

