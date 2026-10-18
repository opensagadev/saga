// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/combat/fighting.h"
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
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/audio/audio.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include <stdio.h>
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include <new>
#include "legoapi/menus/screens/gamestructure.h"
#include <stdlib.h>
#include "legoapi/ai/core/gameai_types.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/ai/game/creature.h"
#include "nu2api/numath/nurand.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include <float.h>
#include "editor/edpath.h"
#include "legoapi/actions/character/speederchase.h"
#include "batman.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "legoapi/actions/movement/jumping.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/characters/core/customiser.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/world/areas.h"
#include "legoapi/characters/core/characters_types.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/render/core/screen.h"
#include <limits.h>
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/render/fx/particles.h"
#include <cfloat>
#include "legoapi/items/collect/bolts.h"
#include "legoapi/characters/motion/chris.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "gameapi/ai/aisys/gameaiscript.h"
#include "legoapi/actions/character/legoapi_tag.h"
#include "legoapi/actions/character/tagging.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/actions/movement/pushing.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/ai/game/aitrigger.h"
#include "legoapi/ai/game/lsw_hub_ai.h"
#include "legoapi/audio/radio.h"
#include "legoapi/characters/motion/charplatforms.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/core/startup/text.h"
#include "legoapi/cutscenes/gcutscn.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/items/collect/bolt.h"
#include "legoapi/items/fx/explosions.h"
#include "legoapi/items/fx/pulses.h"
#include "legoapi/items/fx/ripples.h"
#include "legoapi/items/objects/game_obj.h"
#include "legoapi/items/objects/grabber.h"
#include "legoapi/items/objects/traffic.h"
#include "legoapi/lib/camera.h"
#include "legoapi/menus/core/hint.h"
#include "legoapi/menus/screens/customise.h"
#include "legoapi/props/objects/zipup.h"
#include "legoapi/render/light/faders.h"
#include "legoapi/world/levels/episodeI.h"
#include "legoapi/world/levels/episodeII.h"
#include "legoapi/world/levels/episodeIII.h"
#include "legoapi/world/levels/episodeIV.h"
#include "legoapi/world/levels/episodeVI.h"
#include "legoapi/world/levels/hub.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/nuandroid/ios_device.h"

void SurfaceMaskOff(u32 *surface_mask);
void SurfaceMaskOn(u32 *surface_mask);

NARROWSOCKEXCEPTION NarrowSockException_LSW[] = {
    {const_cast<char *>("deathstarbattle_d"), -1, 1, 67.5f, 999.0f},
    {const_cast<char *>("deathstar2battle_b"), -1, 0, 67.5f, 999.0f},
    {const_cast<char *>("deathstar2battle_c"), -1, 0, 30.0f, 999.0f},
    {NULL, 0, 0, 0.0f, 0.0f},
};
extern BOLTTYPE_s GlobalBoltType[44];
static BOLTSYS BoltSys_LSW = {GlobalBoltType,
                              44,
                              AlertSurroundingCreatures,
                              Bolt_Debris_LSW,
                              GetShootOrigin_LSW,
                              GetShootDirection_LSW,
                              Bolt_HitPart_LSW,
                              Bolt_AlternateFire_LSW};
extern i16 tALLEXTRASUNLOCKED;

f32 minikittime;
static PLAYERITEMTYPE_s PlayerItemType_INDY[] = {
    {15, 6, 0, const_cast<char *>("blue"), 2, 0, 0, 0},
    {-1, 0, 0, NULL, 0, 0, 0, 0},
};
extern void (*Player_ClearContextFn)(GameObject_s *, i32);
static void GizBuildIt_FinishFn_Game(GIZBUILDIT_s *buildit);
extern u32 (*CanSuperCarryFn)(GameObject_s *);
extern u32 (*CanPushBlocksFn)(GameObject_s *);
extern u32 (*CanPushObstaclesFn)(GameObject_s *);
extern i16 tCLIPi, tINTRO, tMIDTRO, tOUTRO, tENDING;
extern "C" i16 id_GAMORREANGUARD;
extern void (*Punch_HitHoldFn)(GameObject_s *, GameObject_s *);
extern i32 (*Punch_GetDamageFn)(GameObject_s *, GameObject_s *);
extern void (*Punch_HitExtraCodeFn)(GameObject_s *, nuvec_s *);
extern AREADATA *E1CHARACTER_ADATA;
extern AREADATA *E2CHARACTER_ADATA;
extern AREADATA *E3CHARACTER_ADATA;

i32 drawautosaveicon = 0;

SUPEROPTIONS_s SuperOptions = {};
static CUTSCENESYS CutSceneSys_LSW = {0x5b, 0x5c, 0xe7, 2};

static i32 deathstar_hold_count;

// Retail data @0x617200: common animations shared by the old and new characters.
ANIMREDIRECT AnimRedirectList_LSW[] = {
    {const_cast<char *>("ride_bantha_new_blasterchar"), 112, {0, 0}},
    {const_cast<char *>("ride_bantha_new_jedichar"), 112, {0, 0}},
    {const_cast<char *>("ride_bantha_old_blasterchar"), 112, {0, 0}},
    {const_cast<char *>("ride_bantha_old_jedichar"), 112, {0, 0}},
    {const_cast<char *>("ride_dewback_new_blasterchar"), 131, {0, 0}},
    {const_cast<char *>("ride_dewback_new_jedichar"), 131, {0, 0}},
    {const_cast<char *>("ride_dewback_old_blasterchar"), 131, {0, 0}},
    {const_cast<char *>("ride_dewback_old_jedichar"), 131, {0, 0}},
    {const_cast<char *>("ride_landspeeder_new_blasterchar"), 132, {0, 0}},
    {const_cast<char *>("ride_landspeeder_new_jedichar"), 132, {0, 0}},
    {const_cast<char *>("ride_landspeeder_old_blasterchar"), 132, {0, 0}},
    {const_cast<char *>("ride_landspeeder_old_jedichar"), 132, {0, 0}},
    {const_cast<char *>("ride_tauntaun_new_blasterchar"), 134, {0, 0}},
    {const_cast<char *>("ride_tauntaun_new_jedichar"), 134, {0, 0}},
    {const_cast<char *>("ride_tauntaun_old_blasterchar"), 134, {0, 0}},
    {const_cast<char *>("ride_tauntaun_old_jedichar"), 134, {0, 0}},
    {const_cast<char *>("ride_speederbike_new_blasterchar"), 192, {0, 0}},
    {const_cast<char *>("ride_speederbike_new_jedichar"), 192, {0, 0}},
    {const_cast<char *>("ride_speederbike_old_blasterchar"), 192, {0, 0}},
    {const_cast<char *>("ride_speederbike_old_jedichar"), 192, {0, 0}},
    {const_cast<char *>("ride_hrc_new_blasterchar"), 193, {0, 0}},
    {const_cast<char *>("ride_hrc_new_jedichar"), 193, {0, 0}},
    {const_cast<char *>("ride_hrc_old_blasterchar"), 193, {0, 0}},
    {const_cast<char *>("ride_hrc_old_jedichar"), 193, {0, 0}},
    {const_cast<char *>("throw_new_blasterchar"), 101, {0, 0}},
    {const_cast<char *>("throw_new_jedichar"), 101, {0, 0}},
    {const_cast<char *>("throw_old_blasterchar"), 101, {0, 0}},
    {const_cast<char *>("throw_old_jedichar"), 101, {0, 0}},
    {const_cast<char *>("ride_generic_new_blasterchar"), 108, {0, 0}},
    {const_cast<char *>("ride_generic_new_jedichar"), 108, {0, 0}},
    {const_cast<char *>("ride_generic_old_blasterchar"), 108, {0, 0}},
    {const_cast<char *>("ride_generic_old_jedichar"), 108, {0, 0}},
    {const_cast<char *>("splat_new_blasterchar"), 133, {0, 0}},
    {const_cast<char *>("splat_new_jedichar"), 133, {0, 0}},
    {const_cast<char *>("splat_old_blasterchar"), 133, {0, 0}},
    {const_cast<char *>("splat_old_jedichar"), 133, {0, 0}},
    {const_cast<char *>("ride_tcannon_new_blasterchar"), 194, {0, 0}},
    {const_cast<char *>("ride_tcannon_new_jedichar"), 194, {0, 0}},
    {const_cast<char *>("ride_tcannon_old_blasterchar"), 194, {0, 0}},
    {const_cast<char *>("ride_tcannon_old_jedichar"), 194, {0, 0}},
    {NULL, 0, {0, 0}},
};

extern "C" i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct CUTSCENEPLAYERCLIP;

extern f32 LevelNameTime, LevelNameMul;
extern "C" i32 party_under_cover;

extern "C" i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

extern "C" {
extern i32 party_under_cover;
}

extern f32 ForceThrowSpeed, ForceThrowGravity;
static __used__ void LEGO_100PercentFn();
static __used__ void LEGO_AllGoldBricksFn();
extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct STATUSPACKET_s;
struct nuhspecial_s;

extern i32 LEGOOBJ_CHARKIT;
extern i32 LEGOOBJ_MINIKIT;
extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

static __used__ i32 PauseRenderOff();
static __used__ i32 MatrixReflection_CanOverride();
extern "C" i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;
static __used__ void BackDrop_Alpha(float *alpha);
static __used__ float GizmoPickups_Collide2D(GameObject_s *object);

// Static game message and hint helpers. Stubbed to satisfy the symbol baseline.

static __used__ nuhspecial_s *GameMsg_GetExtraObj(GAMEMESSAGE_s *message);
static __used__ void GameMsg_EndDelay_Game(GAMEMESSAGE_s *message);

struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;
struct BOLT_s;
struct debinftype;

extern "C" {
    extern i16 id_GAMORREANGUARD;
}

static __used__ void SurfaceInfo_ExtraReflect(GameObject_s *object);
static __used__ void PauseGame_ExtraCode();

struct EDCREATURE_s;
struct APIOBJECT_s;

extern "C" {
extern i16 id_GAMORREANGUARD;
    i16 id_CHEWBACCA = -1;

}

static __used__ void ExtraDieSfx_LSW(GameObject_s *object);
static __used__ void ExtraHurtSfx_LSW(GameObject_s *object);

struct GIZMOPICKUP_s;
struct PART_s;

static __used__ void Collection_GetSelectingPlayerIDs(i16 *ids);
static u32 CompletionPointInfo_ReCalculate[7];

struct NUGCUTCHAR_s;

extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);

typedef isize __attribute__((may_alias)) NuGCutWord;

struct instNUGCUTSCENE_s;

static __used__ void CutScene_OverrideConfigFileName_LSW(char *filename, int, int);

extern i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

extern "C" {
extern i32 party_under_cover;
}

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
    extern i32 FalconDebKey[2];
}

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern i32 party_under_cover;
}

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct AISYS_s;
struct nufpar_s;

extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

}

extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

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
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);
extern i16 tOUTOFTIME;

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

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i16 id_GAMORREANGUARD;

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

namespace {

    constexpr f32 kSuperStoryTimeLimit = 3600.0f;
    constexpr i32 kSuperStoryScoreTarget = 100000;
    constexpr i32 kPrimaryCustomNameTextId = 0xcc;
    constexpr i32 kSecondaryCustomNameTextId = 0xcd;

} // namespace

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

void SaveSystemInitialise(i32 slots, void *makeSaveHash, void *save, i32 saveSize, i32 saveCount,
                          void (*drawSaveIcon)(void), void *extradata, i32 extradataSize);

extern "C" {
void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                   u8 green, u8 blue, f32 max_width, i32 max_lines);
void TextRegisterButtonMapFn(void (*fn)(char *, char *));
void TextRegisterPulseTimerFn(f32 (*fn)(f32));
}

extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

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

extern "C" {
void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                   u8 green, u8 blue, f32 max_width, i32 max_lines);

}

extern i16 tCHALLENGECOMPLETE, tMINIKIT;
extern i16 tOUTOFTIME, tYOUDIDNTCOLLECTALL10CANISTERS, tSTORYCLIPSUNLOCKED;

extern char codechars[37];
extern char usercode[6];
extern i16 tSELECT, tBACK, tBUY, tPLAY, tSELECTING;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern f32 LevelNameMul, LevelNameTime;
extern "C" {
    extern i32 party_under_cover, nbaddies_can_see_players;
    extern i32 FalconDebKey[2];
    extern "C++" f32 TargetDist_Near2, TargetDist_Mid2;
    extern "C++" u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
    extern i32 makebaddiesforgetinresetbits;
    extern i32 reset_reimport;
    extern u32 arcade_placed_stud_total;
}

u32 arcade_placed_stud_total = 0;
i32 FalconDebKey[2] = {-1, -1};
extern f32 LevelNameTime;
extern f32 LevelNameMul;

f32 TargetDist_Near2;
f32 TargetDist_Mid2;
u16 TargetDeg_Near;
u16 TargetDeg_Mid;
u16 TargetDeg_Far;
extern i32 party_under_cover;
i32 makebaddiesforgetinresetbits;
extern i32 nbaddies_can_see_players;
i32 reset_reimport;

extern "C" {
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern i32 party_under_cover;
}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" {
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern i32 party_under_cover;
}

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

extern i16 id_GAMORREANGUARD, id_PRINCESSLEIASLAVE, id_LAMASU, id_TAUNWE;

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

extern i16 tEPISODEVUNLOCKED, tEPISODEVIUNLOCKED, tCHALLENGECOMPLETE, tOUTOFTIME;

extern "C" {
void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *));

}

static NUVEC PodRaceADiePos = {350.0f, 0.25f, -15.0f};
static NUVEC PodRaceBDiePos = {138.0f, 0.15f, 178.0f};
static NUVEC PodRaceCDiePos = {140.0f, -7.2f, 52.0f};
static NUVEC GunshipADiePos = {10.0f, -1.0f, -20.0f};
static NUVEC GunshipBDiePos = {0.0f, -1.0f, -150.0f};
static NUVEC PodAvalanchePeakPos = {-110.0f, -10.0f, -455.0f};

__attribute__((force_align_arg_pointer)) void Bolt_AddDeflectedBolt(BOLT_s *bolt, nuvec_s *direction, nuvec_s *normal,
                                                                    unsigned char *processed);

static __used__ void LC_BL_max_attractos(nufpar_s *fp);
static __used__ void LC_BL_max_bombgen_objects(nufpar_s *fp);
static __used__ void LC_BL_max_bombgens(nufpar_s *fp);
static __used__ void LC_BL_max_bridges(nufpar_s *fp);
static __used__ void LC_BL_max_buildit_objects(nufpar_s *fp);
static __used__ void LC_BL_max_buildits(nufpar_s *fp);
static __used__ void LC_BL_max_climb_objects(nufpar_s *fp);
static __used__ void LC_BL_max_doors(nufpar_s *fp);
static __used__ void LC_BL_max_force(nufpar_s *fp);
static __used__ void LC_BL_max_force_objects(nufpar_s *fp);
static __used__ void LC_BL_max_gameantinodes(nufpar_s *fp);
static __used__ void LC_BL_max_gizmoblowups(nufpar_s *fp);
static __used__ void LC_BL_max_gizmoblowuptypes(nufpar_s *fp);
static __used__ void LC_BL_max_gizpanels(nufpar_s *fp);
static __used__ void LC_BL_max_gizrandoms(nufpar_s *fp);
static __used__ void LC_BL_max_gizspecials(nufpar_s *fp);
static __used__ void LC_BL_max_grapples(nufpar_s *fp);
static __used__ void LC_BL_max_guidelines(nufpar_s *fp);
static __used__ void LC_BL_max_hatmachines(nufpar_s *fp);
static __used__ void LC_BL_max_ledges(nufpar_s *fp);
static __used__ void LC_BL_max_levers(nufpar_s *fp);
static __used__ void LC_BL_max_minicutParts(nufpar_s *fp);
static __used__ void LC_BL_max_minicuts(nufpar_s *fp);
static __used__ void LC_BL_max_obstacle_objects(nufpar_s *fp);
static __used__ void LC_BL_max_obstacles(nufpar_s *fp);
static __used__ void LC_BL_max_pickups(nufpar_s *fp);
static __used__ void LC_BL_max_plugs(nufpar_s *fp);
static __used__ void LC_BL_max_pushblock_endpos(nufpar_s *fp);
static __used__ void LC_BL_max_pushblocks(nufpar_s *fp);
static __used__ void LC_BL_max_securitydoors(nufpar_s *fp);
static __used__ void LC_BL_max_shards(nufpar_s *fp);
static __used__ void LC_BL_max_signals(nufpar_s *fp);
static __used__ void LC_BL_max_spinneranim_objs(nufpar_s *fp);
static __used__ void LC_BL_max_spinners(nufpar_s *fp);
static __used__ void LC_BL_max_technos(nufpar_s *fp);
static __used__ void LC_BL_max_teleports(nufpar_s *fp);
static __used__ void LC_BL_max_tightropes(nufpar_s *fp);
static __used__ void LC_BL_max_timers(nufpar_s *fp);
static __used__ void LC_BL_max_torpmachines(nufpar_s *fp);
static __used__ void LC_BL_max_tubes(nufpar_s *fp);
static __used__ void LC_BL_max_turrets(nufpar_s *fp);
static __used__ void LC_BL_max_zipups(nufpar_s *fp);
static __used__ void LC_BL_wind_size(nufpar_s *fp);
static __used__ void LC_BL_wind_speed(nufpar_s *fp);

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.
NUFPCOMJMP LevelConfigKeywords_BeforeLoad[] = {
    {const_cast<char *>("max_tightropes"), LC_BL_max_tightropes},
    {const_cast<char *>("max_signals"), LC_BL_max_signals},
    {const_cast<char *>("max_levers"), LC_BL_max_levers},
    {const_cast<char *>("max_technos"), LC_BL_max_technos},
    {const_cast<char *>("max_zipups"), LC_BL_max_zipups},
    {const_cast<char *>("max_grapples"), LC_BL_max_grapples},
    {const_cast<char *>("max_obstacles"), LC_BL_max_obstacles},
    {const_cast<char *>("max_obstacle_objects"), LC_BL_max_obstacle_objects},
    {const_cast<char *>("max_turrets"), LC_BL_max_turrets},
    {const_cast<char *>("max_buildits"), LC_BL_max_buildits},
    {const_cast<char *>("max_buildit_objects"), LC_BL_max_buildit_objects},
    {const_cast<char *>("max_spinneranim_objs"), LC_BL_max_spinneranim_objs},
    {const_cast<char *>("max_shards"), LC_BL_max_shards},
    {const_cast<char *>("max_spinners"), LC_BL_max_spinners},
    {const_cast<char *>("max_minicuts"), LC_BL_max_minicuts},
    {const_cast<char *>("max_minicut_stages"), LC_BL_max_minicutParts},
    {const_cast<char *>("maxgizmoblowuptypes"), LC_BL_max_gizmoblowuptypes},
    {const_cast<char *>("maxgizmoblowups"), LC_BL_max_gizmoblowups},
    {const_cast<char *>("max_climb_objects"), LC_BL_max_climb_objects},
    {const_cast<char *>("max_attractos"), LC_BL_max_attractos},
    {const_cast<char *>("max_timers"), LC_BL_max_timers},
    {const_cast<char *>("max_guidelines"), LC_BL_max_guidelines},
    {const_cast<char *>("max_ledges"), LC_BL_max_ledges},
    {const_cast<char *>("max_securitydoors"), LC_BL_max_securitydoors},
    {const_cast<char *>("max_tubes"), LC_BL_max_tubes},
    {const_cast<char *>("max_pickups"), LC_BL_max_pickups},
    {const_cast<char *>("max_gameantinodes"), LC_BL_max_gameantinodes},
    {const_cast<char *>("max_gizpanels"), LC_BL_max_gizpanels},
    {const_cast<char *>("max_force"), LC_BL_max_force},
    {const_cast<char *>("max_force_objects"), LC_BL_max_force_objects},
    {const_cast<char *>("max_pushblocks"), LC_BL_max_pushblocks},
    {const_cast<char *>("max_pushblock_endpos"), LC_BL_max_pushblock_endpos},
    {const_cast<char *>("max_gizspecials"), LC_BL_max_gizspecials},
    {const_cast<char *>("max_hatmachines"), LC_BL_max_hatmachines},
    {const_cast<char *>("max_doors"), LC_BL_max_doors},
    {const_cast<char *>("max_teleports"), LC_BL_max_teleports},
    {const_cast<char *>("max_gizrandoms"), LC_BL_max_gizrandoms},
    {const_cast<char *>("max_torpmachines"), LC_BL_max_torpmachines},
    {const_cast<char *>("max_bridges"), LC_BL_max_bridges},
    {const_cast<char *>("max_plugs"), LC_BL_max_plugs},
    {const_cast<char *>("max_bombgen_objects"), LC_BL_max_bombgen_objects},
    {const_cast<char *>("max_bombgens"), LC_BL_max_bombgens},
    {const_cast<char *>("windspeed"), LC_BL_wind_speed},
    {const_cast<char *>("windsize"), LC_BL_wind_size},
    {NULL, NULL},
};

u16 MakeSaveHash(void) {
    return Game.completion;
}

static i32 CanMagnetClimb_Game(GameObject_s *object) {
    SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
    return suit != NULL ? suit->can_magnet_climb : 0;
}

static u32 CanPushObstacles_Game(GameObject_s *object) {
    return (object->apiobj.character_data->game_character->flags_090 & GAMECHARACTER_FLAG_CAN_SUPER_CARRY) != 0;
}

static u32 CanSuperCarry_Game(GameObject_s *object) {
    return (object->apiobj.character_data->game_character->flags_090 & GAMECHARACTER_FLAG_CAN_SUPER_CARRY) != 0;
}

static u32 CanPushBlocks_Game(GameObject_s *) {
    return 1;
}

static i32 CanGlide_Game(GameObject_s *object) {
    SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
    return suit != NULL ? (suit->flags & 2) != 0 : 0;
}

static i32 UsingExtraActions_Game(GameObject_s *object) {
    return object->id == id_GRIEVOUS && ((object->field_0xe22 & 1) == 0 || object->weapon_scale_state == 2);
}

static i32 CanStartHold_Game(GameObject_s *) {
    return 1;
}

static bool IsWearingBackPack_Game(GameObject_s *object) {
    SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
    return suit != NULL ? (suit->initially_available & 1) : 0;
}

static i32 Fighting_WeaponInAction_Game(GameObject_s *object) {
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    return data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 126 : 16;
}

static i32 Fighting_WeaponOutAction_Game(GameObject_s *object) {
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    return data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 127 : 17;
}

static i32 SetSoundFadeDistCallBackFn_LSW(WORLDINFO_s *) {
    LEVELDATA *level = WORLD->current_level;

    if (level == KAMINOE_LDATA) {
        nusound_fade_start = 3.0f;
        nusound_fade_end = 22.5f;
    } else if (WORLD->area != NULL && (WORLD->area == GUNSHIP_ADATA || WORLD->area == BONUS_GUNSHIP_ADATA)) {
        nusound_fade_start = 25.0f;
        nusound_fade_end = 125.0f;
    } else if (WORLD->area != NULL && WORLD->area == DOGFIGHT_ADATA) {
        nusound_fade_start = 100.0f;
        nusound_fade_end = 250.0f;
    } else if (level == DEATHSTARRESCUEE_LDATA) {
        nusound_fade_start = 5.0f;
        nusound_fade_end = 50.0f;
    } else if (WORLD->area != NULL && WORLD->area == DEATHSTARESCAPE_ADATA) {
        nusound_fade_start = 3.0f;
        nusound_fade_end = 15.0f;
    } else if (WORLD->area != NULL && WORLD->area == ASTEROIDCHASE_ADATA) {
        nusound_fade_start = 25.0f;
        nusound_fade_end = 150.0f;
    } else if (WORLD->area != NULL && WORLD->area == JABBASPALACE_ADATA) {
        nusound_fade_start = 3.0f;
        nusound_fade_end = 20.0f;
    } else if (level == SARLACCPITA_LDATA) {
        nusound_fade_start = 7.5f;
        nusound_fade_end = 20.0f;
    } else if (level == SARLACCPITC_LDATA) {
        nusound_fade_start = 7.5f;
        nusound_fade_end = 55.0f;
    } else if (WORLD->area != NULL && WORLD->area == SPEEDERCHASE_ADATA) {
        nusound_fade_start = 15.0f;
        nusound_fade_end = 50.0f;
    } else if (level == ENDORBATTLEA_LDATA) {
        nusound_fade_start = 4.0f;
        nusound_fade_end = 12.0f;
    } else if (level == ENDORBATTLEB_LDATA || level == ENDORBATTLEC_LDATA) {
        nusound_fade_start = 15.0f;
        nusound_fade_end = 45.0f;
    } else if (level == ENDORBATTLED_LDATA) {
        nusound_fade_start = 3.0f;
        nusound_fade_end = 12.0f;
    } else if (WORLD->area != NULL && WORLD->area == DEATHSTARBATTLE2_ADATA) {
        nusound_fade_start = 50.0f;
        nusound_fade_end = 200.0f;
    } else if (WORLD->area != NULL && (WORLD->area == E1CHARACTER_ADATA || WORLD->area == E2CHARACTER_ADATA ||
                                       WORLD->area == E3CHARACTER_ADATA || WORLD->area == BONUSKAMINO_ADATA ||
                                       WORLD->area == BONUSDAGOBAH_ADATA)) {
        nusound_fade_start = 4.0f;
        nusound_fade_end = 40.0f;
    } else {
        return 0;
    }
    return 1;
}

static void DisguiseAdjust_LSW(i32 character_id, i32 hat, NUVEC *scale, NUVEC *offset) {
    *scale = v111;
    *offset = v000;

    if (character_id == id_TAUNTAUN) {
        scale->x = 2.2f;
    } else if (character_id == id_DEWBACK) {
        scale->x = 4.6f;
    } else if (character_id == id_BANTHA) {
        scale->x = 3.8f;
    } else if (character_id == id_ATST) {
        scale->x = 5.1f;
    } else if (character_id == id_SNOWMOB) {
        scale->x = 2.25f;
    } else if (character_id == id_MOONCAR || character_id == id_MAPCAR) {
        scale->x = 2.3f;
    } else if (character_id == id_ATAT) {
        scale->x = 7.0f;
    } else if (character_id == id_PROBEDROID) {
        scale->x = 3.0f;
    } else if (character_id == id_XWING) {
        scale->x = 10.0f;
    } else if (character_id == id_YWING) {
        scale->x = 6.5f;
    } else if (character_id == id_SNOWSPEEDER) {
        scale->x = 4.0f;
    } else if (character_id == id_MILLENNIUMFALCON) {
        scale->x = 10.0f;
    } else if (character_id == id_TIEFIGHTER || character_id == id_TIEINTERCEPTOR ||
               character_id == id_TIEFIGHTERDARTH || character_id == id_TIEBOMBER) {
        scale->x = 6.0f;
    } else if (character_id == id_IMPERIALSHUTTLE || character_id == id_SLAVE1) {
        scale->x = 8.0f;
    } else if (character_id == id_WOOKIEFLYER) {
        scale->x = 6.0f;
    } else if (character_id == id_CLONEARC) {
        scale->x = 5.0f;
    } else if (character_id == id_VULTUREDROID || character_id == id_DROIDTRIFIGHTER ||
               character_id == id_ANAKINSNEWPOD || character_id == id_ANAKINSNEWPODGREEN ||
               character_id == id_NEW_REPUBLIC_GUNSHIP || character_id == id_NEW_REPUBLIC_GUNSHIP_GREEN) {
        scale->x = 6.0f;
    } else if (character_id == id_NABOOSTARFIGHTER || character_id == id_NABOOSTARFIGHTERLIME) {
        scale->x = 7.0f;
    } else if (character_id == id_ANAKINSSPEEDER || character_id == id_ANAKINSSPEEDER_GREEN) {
        scale->x = 5.0f;
    } else if (character_id == id_SEBULBASPOD) {
        scale->x = 8.0f;
    }

    if (hat == 5 || hat == 6) {
        offset->z = -0.03f;
    } else if (character_id == id_SNOWTROOPER || character_id == id_DEATHSTARTROOPER) {
        offset->z = -0.00999f;
    } else if (GCDataList[character_id].field275_0x116 == 1) {
        offset->z = -0.03f;
    } else if (character_id == id_GONKDROID) {
        offset->z = -0.0075f;
    } else if (character_id == id_IMPERIALSPY || character_id == id_DARTHVADER || character_id == id_GREEDO ||
               character_id == id_BOSSK || character_id == id_CANTINABAND) {
        offset->z = -0.03f;
    } else if (character_id == id_IMPERIALGUARD) {
        offset->z = 0.015f;
    } else if (character_id == id_UGNAUGHT) {
        offset->z = -0.03f * 0.75f;
    }

    if (scale->x != 1.0f) {
        scale->z = scale->x;
        scale->y = scale->x;
    }
}

static i32 GizBuildIt_CanStartBuildingFn_Game(GIZBUILDIT_s *buildit, GameObject_s *) {
    if (WORLD->area != NULL && (WORLD->area->flags & 0x40) != 0 && LevBuildIt[0] == buildit)
        return Game.field_0x7c26[0] >= 2 * buildit->anim_object_count;
    return 1;
}

static __used__ void LEGO_AllGoldBricksFn() {
}

static i32 LastSafePosExtra(GameObject_s *object) {
    LEVELDATA *level = WORLD->current_level;
    if ((level == TATOOINED_LDATA && object->apiobj.field_0x281 == 0x0e) ||
        (level == DAGOBAHD_LDATA && object->apiobj.field_0x281 == 0x18 && object->apiobj.player_controlled &&
         object->apiobj.field_0x27f == 0xff) ||
        (level == DEATHSTARESCAPEC_LDATA && object->id == id_SERVICECAR))
        return 1;
    return 0;
}

static i32 FindSlamOrigin_UseCPos(GameObject_s *object) {
    return object->id == id_GRIEVOUS;
}

static __used__ i32 MatrixReflection_CanOverride() {
    i32 result = 1;
    if (WORLD->current_level == DEATHSTARESCAPEA_LDATA) {
        const i8 sock = GameCam->sock_position.location.sock;
        if (sock != 0) {
            result = sock == 3;
        }
    }
    return result;
}

static i32 CutScenePlayer_Accept(CUTSCENEPLAYERCLIP *) {
    return 1;
}

static i32 Jump_PreventJump(GameObject_s *object) {
    return WORLD->current_level == KASHYYYKA_LDATA && object->apiobj.field_0x218 != 2000000.0f &&
           object->surface_normal.y < NuTrigTable[0x371c];
}

static f32 Hint_AlphaTarget() {
    if (minikittime > 0.0f && ChallengeMode == 0)
        return 0.0f;
    return 1.0f;
}

static __used__ float GizmoPickups_Collide2D(GameObject_s *object) {
    if (WORLD->current_level == SPEEDERCHASEA_LDATA && object->id == id_SPEEDERBIKE)
        return 2.0f;
    return 0.0f;
}

static i32 GizmoBlowUp_NoTarget(WORLDINFO_s *world, GameObject_s *) {
    if (world->current_level == DEATHSTARRESCUEA_LDATA)
        return GameCam->sock_position.location.sock == 2;
    return 0;
}

static __used__ nuhspecial_s *GameMsg_GetExtraObj(GAMEMESSAGE_s *message) {
    if (message->icon == 0xd0)
        return &WORLD->lev_objs[0xd1].special;
    return NULL;
}

i32 Game_IgnoreInput() {
    extern i32 newgamecam;
    return newgamecam != 0;
}

static i32 Tag_NoHiddenIcon(GameObject_s *) {
    if (WorldInfo_CurrentlyActive()->current_level == DEATHSTARESCAPEB_LDATA) {
        return GameCam->sock_position.location.sock == 5;
    }
    return 0;
}

static __used__ i32 PauseRenderOff() {
    GetMenuID();
    return 0;
}

static __used__ void BackDrop_Alpha(float *alpha) {
    extern STATUSPACKET_s StatusPacket;
    extern i32 selectmodemode;
    extern f32 selectmodetime;
    if (WORLD != NULL) {
        LEVELDATA *level = WORLD->current_level;
        if (level == TITLES_LDATA) {
            *alpha *= newgamealpha;
            return;
        }
        if (level == CREDITS_LDATA && CreditsFlag == 3) {
            *alpha *= CreditsAlpha;
            return;
        }
        if ((level->flags & 0x400) != 0 && StatusPacket.status_flags != 0 && StatusPacket.stage->type == 12 &&
            (StatusPacket.mode_flags & 0x10) == 0) {
            const f32 time = StatusPacket.stage->field_0x18;
            if (time < 0.5f) {
                *alpha = 1.0f - (time + time);
            } else {
                *alpha = 0.0f;
            }
            return;
        }
    }
    if (GetMenuID() == 15 && selectmodemode == 4) {
        const f32 time = selectmodetime;
        if (time < 0.5f) {
            *alpha = 1.0f - (time + time);
        } else {
            *alpha = 0.0f;
        }
    }
}

static __used__ void LEGO_100PercentFn() {
    if (CharacterCustomiser != NULL) {
        Customiser_Set100PercentPieces(CharacterCustomiser);
        i32 index;
        i32 piece;
        if (Customiser_FindPieceByName(CharacterCustomiser, "hat_hair_00", &index, &piece) != NULL && index != -1 &&
            piece != -1) {
            CharacterCustomiser->save->pieces[index] = static_cast<i16>(piece);
        }
    }
}

static i32 CheckMusicOther() {
    nuvec_s position;
    SOCKPOSITION_s socket_position;
    if (Players_AveragePos(&position, &socket_position) == 0) {
        return 0;
    }
    LEVELDATA_s *level = WORLD->current_level;
    if (level == HUB_LDATA) {
        return Hub_Outside() != 0;
    }
    if (level == KAMINOA_LDATA) {
        return KaminoInside() != 0;
    }
    if (level == KAMINOC_LDATA) {
        return KaminoDiscoOn() != 0;
    }
    if (level == KAMINOE_LDATA) {
        return KaminoInside() == 0;
    }
    if (level == MOSEISLEYD_LDATA) {
        return socket_position.location.sock == 3;
    }
    if (level == DEATHSTARBATTLED_LDATA) {
        if (DeathStarShieldDown() != 0) {
            deathstar_hold_count = 30;
            return 1;
        }
        if (deathstar_hold_count > 0) {
            --deathstar_hold_count;
            return 1;
        }
    } else if (level == ASTEROIDCHASEB_LDATA) {
        return GameCam->sock_position.location.sock == 4;
    } else if (level == SARLACCPITB_LDATA) {
        return SarlaccPitDiscoActive(WORLD) != 0;
    }
    return 0;
}

static __used__ void PauseGame_ExtraCode() {
    Hub_ResetPanel();
}

static i32 GizBuildit_AutoBuildPos_Game(void *context, NUVEC *position, NUVEC *result, u16 *angle) {
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    if (world == NULL)
        world = WorldInfo_CurrentlyActive();
    ATTRACTO_s *attracto = Attracto_FindNearest(world, position, NULL, NULL);
    if (attracto == NULL)
        return 0;
    Attracto_GetPos_Top(attracto, result);
    if (angle != NULL)
        *angle = attracto->angle;
    return 1;
}

static void Player_ClearContext_Game(GameObject_s *object, i32 release_takeover) {
    ReleaseForce(object, 0);
    ReleaseEat(object);
    ReleaseBuildIt(object, 0);
    ReleasePush(object);
    if (release_takeover != 0)
        ReleaseTakeOver(object, 1);
}

void GizmoBlowUp_Sfx(GIZMOBLOWUP_s *, NUVEC *position) {
    if (WORLD->current_level == PODSPRINTA_LDATA && static_cast<u8>(GameCam->sock_position.location.sock - 10) <= 2 &&
        GameCam->sock_position.distance > 145.0f && GameCam->sock_position.distance < 195.0f) {
        PlaySfx("PodX_PurpCrysHit", position);
    } else if (WORLD->current_level == PODRACEB_LDATA && GameCam->sock_position.location.sock == 0 &&
               GameCam->sock_position.distance > 145.0f) {
        PlaySfx("PodX_PurpCrysHit", position);
    } else {
        GameAudio_PlaySfx(0x33, position, 0, 0);
    }
}

static i32 BoltInitSfx_LSW(GameObject_s *object) {
    if (object != NULL && object->id == id_GEONOSIAN && (object->field_0xefd & 2) == 0)
        return GameAudio_GetSfxId(0x49);
    return -1;
}

void GameObjectDimensionsExtra_LSW(GameObject_s *object) {
    if (object->id == id_ATAT && object->character_context == 0x17 && object->context_animation == 0x41)
        object->apiobj.field_0x1dc *= 2.0f;
}

static __used__ void SurfaceInfo_ExtraReflect(GameObject_s *object) {
    if (WORLD->current_level == CRUISERE_LDATA && object->field_0x1020 == 2000000.0f &&
        object->apiobj.position.x < 11.0f) {
        object->field_0x1087 = 3;
        object->field_0x1020 = -39.2f;
    }
    if (WORLD->current_level == DEATHSTARRESCUED_LDATA) {
        if (object->apiobj.position.z > 20.75f) {
            object->field_0x1020 = 22.4f;
            object->field_0x1087 = 3;
        }
        if (object->apiobj.position.x < -20.75f) {
            object->field_0x1087 = 1;
            object->field_0x1020 = -22.4f;
        }
    }
}

static __used__ void GameMsg_EndDelay_Game(GAMEMESSAGE_s *message) {
    if (message->score == 0)
        return;
    if (VehicleArea != 0) {
        DrawBuildUpTime = COINMSGTIME + 1.0f;
        return;
    }
    if (message->field_0xfe != 1)
        return;

    AddGameDebris(WORLD->debris_sys, 0x38, &message->position_a);
    if (WORLD->area != NULL && (WORLD->area->flags & 0x100) != 0) {
        message->position.x += static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.2f - 0.1f;
        message->position.y += static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.2f - 0.1f;
        message->position.z += static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.2f - 0.1f;
        message->field_0xf9 = 0;
    }
}

static __used__ void ExtraDieSfx_LSW(GameObject_s *object) {
    if (object->id == id_CHEWBACCA && Cheat[4].enabled != 0) {
        PlaySfx("C3_Death", &object->apiobj.collision_position);
    }
}

static __used__ void ExtraHurtSfx_LSW(GameObject_s *object) {
    if (object->id == id_CHEWBACCA && Cheat[4].enabled != 0) {
        PlaySfx("C3_Death", &object->apiobj.collision_position);
    }
}

static void BigJump_EndOfLand(GameObject_s *object) {
    if (object->id == id_THEEMPEROR && LEGOACT_COMBOLAND != -1 && object->context_animation == LEGOACT_COMBOLAND &&
        ((object->field_0xe22 & 1) == 0 || object->field_0xe32 == 2))
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
}

static void Jump_EndOfLandContext(GameObject_s *object) {
    if (object->id == id_THEEMPEROR && object->character_context == LEGOCONTEXT_LAND_COMBOJUMP &&
        LEGOACT_COMBOLAND != -1 && object->context_animation == LEGOACT_COMBOLAND &&
        ((object->field_0xe22 & 1) == 0 || object->weapon_scale_state == 2))
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
}

static __used__ void Collection_GetSelectingPlayerIDs(i16 *ids) {
    if (WORLD->area != NULL && WORLD->area == HUB_ADATA) {
        if (GetMenuID() == 17) {
            if (static_cast<u32>(freeplaymode) <= 3) {
                i32 offset = 0;
                if (MenuPacket.active_player[0] != 0 && freeplay_selected[0] <= 2) {
                    ids[0] = MenuPacket.player_model[0];
                    offset = 2;
                }
                if (MenuPacket.active_player[1] == 0)
                    return;
                if (freeplay_selected[1] > 2)
                    return;
                *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(ids) + offset) = MenuPacket.player_model[1];
            }
        }
    }
}

static i32 ActionMusicFn() {
    if (Arcade != 0 || DoubleScore != 0 || Cheat_PowerUpActive(-1) != 0) {
        return 1;
    }
    LEVELDATA_s *level = WORLD->current_level;
    if ((level == CRUISERA_LDATA && MiniCutCam != 0) || (level == CRUISERD_LDATA && CruiserD_LiftChase != 0) ||
        level == DEATHSTARRESCUEE_LDATA) {
        return 1;
    }
    if (level == MOSEISLEYD_LDATA && CheckMusicOther() != 0) {
        return 1;
    }
    if (level == CLOUDCITYESCAPEA_LDATA) {
        return 1;
    }
    if (level == SPEEDERCHASEA_LDATA) {
        nuvec_s position;
        SOCKPOSITION_s socket_position;
        if (Players_AveragePos(&position, &socket_position) != 0) {
            const i8 socket = socket_position.location.sock;
            if (socket == 7 || socket == 8) {
                return 1;
            }
            if (socket == 2 || socket == 3 || socket == 4 || socket == 6 || socket == 9) {
                return 0;
            }
        }
        level = WORLD->current_level;
    }
    if (level == HUB_LDATA) {
        return ai_fighting != 0;
    }
    for (i32 i = 0; i < 2; ++i) {
        GameObject_s *player = Player[i];
        if (player != NULL &&
            (player->ai.opponent != NULL || (player->ai.nearest_opponent != NULL &&
                                             static_cast<APIOBJECT *>(player->ai.nearest_opponent)->field_0x287 == 0 &&
                                             player->ai.nearest_opponent_metric < 3.0f))) {
            return 1;
        }
    }
    return 0;
}

static i32 Slam_GetDebris_Game(GameObject_s *object, i32 effect) {
    if (object->apiobj.field_0x27c != -1) {
        if (Cheat_IsOn(0x19))
            return 1;
        if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
            return 4;
    }
    if (object->id == id_GRIEVOUS)
        return (qrand() > 0x7fff) + 2;
    return effect == -1 ? 2 : effect;
}

static void GoThroughDoor_ExtraCode(WORLDINFO_s *world, DOOR_s *door) {
    VehicleAreaRememberSpeed = GetVehicleAreaRememberSpeed();
    if (world->current_level == VADERA_LDATA) {
        VaderA_GoneThroughDoor(world, door);
    }
    if (PODRACEB_LDATA != NULL && door->level == PODRACEB_LDATA->idx && world->current_level == PODRACEA_LDATA) {
        if (Lap == 3) {
            if (FreePlay == 0) {
                NewLData = PODRACEOUTRO1_LDATA;
                grab_screen_image = 1;
                if (waiting_for_level != -1) {
                    waiting_for_new_level = 1;
                }
            } else {
                CompleteLevel(world);
            }
        } else {
            PodRace_IncreaseLap();
        }
    }
}

static __used__ void CutScene_OverrideConfigFileName_LSW(char *filename, int, int) {
    if (PODSPRINT_ADATA == NULL || WorldInfo_CurrentlyActive()->area != PODSPRINT_ADATA) {
        return;
    }
    if (NuStrIStr(filename, const_cast<char *>("episodei\\ep1_podrace_")) != filename) {
        return;
    }
    char *suffix = filename + NuStrLen(const_cast<char *>("episodei\\ep1_podrace_"));
    if (NuStrICmp(suffix, "arrival1") == 0 || NuStrICmp(suffix, "arrival2") == 0 ||
        NuStrICmp(suffix, "arrival3") == 0 || NuStrICmp(suffix, "arrival4") == 0 || NuStrICmp(suffix, "intro") == 0 ||
        NuStrICmp(suffix, "tuskenraiders") == 0 || NuStrICmp(suffix, "outro1") == 0 ||
        NuStrICmp(suffix, "outro2") == 0) {
        NuStrCat(filename, "_sprint");
    }
}

static void Punch_HitExtraCode_LSW(GameObject_s *object, nuvec_s *position) {
    if (object->id == id_GAMORREANGUARD && ((object->character_context == 0x26 && object->context_animation == 0x56) ||
                                            object->character_context == 0x0d)) {
        NewRumbleAllPlayers(0.4f, 0.0f, 1, 0);
        NewRumble(object->pad_gamepad->pad, 0.7f, 0);
        GameCam_Judder(GameCam, 0.25f, 0, &object->apiobj.collision_position);
        PlaySfx("fs_gamorr_land", &object->apiobj.lower_position);
    } else if ((AnimMiscFlags(object->apiobj.character_model, object->context_animation) & 4) != 0) {
        if (position != NULL)
            PlaySfx("WhipHit", position);
        else
            PlaySfx("WhipNowt", &object->apiobj.collision_position);
    }
}

static void Punch_HitHold(GameObject_s *attacker, GameObject_s *target) {
    if (target != NULL && attacker != NULL && (target->id == id_IMPERIALGUARD || target->id == id_GAMORREANGUARD)) {
        GameAudio_PlaySfx(0x4a, &target->apiobj.collision_position, 0, 0);
    }
}

static i32 Punch_GetDamage_LSW(GameObject_s *attacker, GameObject_s *target) {
    if (attacker->apiobj.character_data->game_character->field275_0x116 == 7 && Cheat_IsOn(0x0c)) {
        GameAudio_PlaySfx(
            0x4a, target != NULL ? &target->apiobj.collision_position : &attacker->apiobj.collision_position, 0, 0);
        GameCam_NewShake(NULL, 0.75f, 0.75f, 1.0f);
        return -1;
    }
    if (target != NULL) {
        if (target->id == id_IMPERIALGUARD || target->id == id_GAMORREANGUARD) {
            if (target->character_context == 0x0c || target->character_context == 0x18) {
                GameAudio_PlaySfx(0x4a, &target->apiobj.collision_position, 0, 0);
            }
        }
    }
    return 1;
}

void ResetAdaptiveDifficulty() {
    ADAPTIVEDIFFICULTY_s *difficulty = (ADAPTIVEDIFFICULTY_s *)adaptivedifficulty;
    difficulty->multiplier = 0.5f;
    difficulty->elapsed = 0.0f;
    difficulty->difficulty = -4;
}

void InitGameBeforeConfig(void) {
    f32 frame_time;
    if (PAL != 0) {
        NuStrCpy(prodcode, "BESLES-54221");
        frame_time = 0.02f;
    } else {
        NuStrCpy(prodcode, "BASLUS-21409");
        frame_time = 0.016666668f;
    }
    FRAMETIME = frame_time;
    DEFAULTFPS = 1.0f / frame_time;
    DEFAULTFRAMETIME = 0.016666668f;
    MAXFRAMETIME = 0.1f;
    permbuffer_ptr = permbuffer_base;
    permbuffer_end = superbuffer_end;

    saveloadInit(&permbuffer_base, superbuffer_end, 0x7e58, prodcode, iconname, unicodename, 4);
    original_permbuffer_base = permbuffer_base;
    SaveSystemInitialise(3, (void *)MakeSaveHash, &Game, sizeof(GAMESAVE_s), 1, DrawAutoSaveIcon, &SuperOptions,
                         sizeof(SuperOptions));

    Game_OptionsSave = &Game.options_save;
    Game_LevelSave = Game.level_save;
    Game_AreaSave = Game.area_save;
    Game_EpisodeSave = Game.episode_save;
    Game_CharacterSave = Game.character_save;
    Game_CompletionSave = &Game.completion;
    Game_MissionSave = &Game.mission_save;

    ResetSeeds();
    ResetTimer(&GlobalTimer, 0.0f);
    ResetTimer(&OverallGamePlayTimer, 0.0f);
    SetAnimBlendMode(2);
    SetAPIObjPlaySfxByIdFn(PlaySfxById);
    NuSetPadDemoEndButtons(GAMEPAD_SKIP);

    // Original option defaults.  OPTIONSSAVE occupies Game[0x4..0x10]; the
    // final brightness byte at 0xc was previously misplaced outside the type.
    const i32 aspect = NuVideoGetAspect();
    Game.options_save.field0_0x0 = 1;
    Game.options_save.field1_0x1 = 1;
    Game.options_save.field2_0x2 = 0;
    Game.options_save.field4_0x4 = 10;
    Game.options_save.field3_0x3 = 10;
    Game.options_save.field7_0x7 = 0;
    Game.options_save.field8_0x8 = 0;
    Game.options_save.field5_0x5 = 10;
    Game.options_save.field6_0x6 = 1;
    const bool standard_aspect = aspect == 0 || aspect == 3;
    Game.options_save.field11_0xb = !standard_aspect;
    Game.options_save.field12_0xc = 10;

    // Persistent touch/control options are initialized before any save data is
    // loaded.  In particular, byte 0x14 is the original music-enable gate.
    SuperOptions.field0_0x0 = -1;
    SuperOptions.field8_0x15 = -1;
    SuperOptions.touch_controls = 1;
    SuperOptions.field2_0x3 = 1;
    if (NuIOS_IsSmallScreen() == 0) {
        SuperOptions.left_control_x = MechInputTouchVirtualConsoleController::s_defaultDPadPosX;
        SuperOptions.left_control_y = MechInputTouchVirtualConsoleController::s_defaultDPadPosY;
        SuperOptions.right_control_x = MechInputTouchVirtualConsoleController::s_defaultButtonsPosX;
        SuperOptions.right_control_y = MechInputTouchVirtualConsoleController::s_defaultButtonsPosY;
    } else {
        SuperOptions.left_control_x = MechInputTouchVirtualConsoleController::s_defaultDPadPosX_SmallScreen;
        SuperOptions.left_control_y = MechInputTouchVirtualConsoleController::s_defaultDPadPosY_SmallScreen;
        SuperOptions.right_control_x = MechInputTouchVirtualConsoleController::s_defaultButtonsPosX_SmallScreen;
        SuperOptions.right_control_y = MechInputTouchVirtualConsoleController::s_defaultButtonsPosY_SmallScreen;
    }
    SuperOptions.music_enabled = 1;

    GameSetSoundVolume(&Game.options_save);
    GameSetMusicVolume(&Game.options_save);
    Text_GameSetLanguageFn = Text_SetLanguage_Game;

    if (Text_Language != 0 && Text_Language != 13 && PAL != 0) {
        Text_SetLanguage(NuLanguageGet());
    }

    GameRegisterGizActions();
    WorldInfo_Reset(&WorldInfo[0], -1);
    WorldInfo_Reset(&WorldInfo[1], -1);
    memset(ZeroRTL, 0, sizeof(ZeroRTL));
    memset(GameCam, 0, sizeof(*GameCam));
    InitPartTable(partdebris_name);
    GameAudio_Reset();
    MENUTEXTSCALE *= 1.2f;
    MENUDY *= 1.5f;
}

i32 DoBuckStart(GameObject_s *object) {
    NUVEC delta;
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world->current_level != HOTHESCAPEB_LDATA || object->id != id_SNOWMOB)
        return 0;
    i32 jumped = 0;
    NUVEC forward = {0.0f, 0.0f, 1.0f};
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *player = Player[i];
        if (player == NULL || (player->apiobj.field_0x1f8 & 0x1001) != 0x1001 || player->apiobj.field_0x287 != 0 ||
            player->field_0xcc0 != NULL)
            continue;

        NuVecSub(&delta, &player->apiobj.lower_position, reinterpret_cast<NUVEC *>(&object->joint_matrices[1].m30));
        if (delta.y > -0.01f && delta.y < 0.1f && delta.x * delta.x + delta.z * delta.z < 0.04000000283122063f) {
            NUVEC destination;
            NuVecRotateY(&destination, &forward, object->apiobj.field_0x276);
            NuVecAdd(&destination, &destination, &Player[i]->apiobj.collision_position);
            f32 height = GameShadow(NULL, &destination, 5.0f, -1);
            if (height != 2000000.0f)
                destination.y = height;
            StartBigJump(Player[i], &destination, 0, 1.0f, 1.0f, 1, 0);
            jumped = 1;
        }
    }
    GameAudio_PlaySfx(4, &object->apiobj.collision_position, 0, 0);
    return jumped;
}

void BuckStartExtra_LSW(GameObject_s *object) {
    DoBuckStart(object);
}

void InitGameAfterConfig(void) {
    AREADATA *pAVar5 = ADataList;

    POINTS_PER_CHARACTER = 1;
    POINTS_PER_SUPERBONUSCOMPLETE = 3;
    POINTS_PER_TIMETRIAL = 2;
    POINTS_PER_STORY = 6;
    POINTS_PER_CHALLENGE = 2;
    POINTS_PER_MINIKIT = 2;
    SHOPHINTCOUNT = 0;
    // TopShelf[6].field0_0x0[0] = '\0';
    // TopShelf[6].field0_0x0[1] = '\0';
    // TopShelf[6].field0_0x0[2] = '\0';
    // TopShelf[6].field0_0x0[3] = '\0';

    POINTS_PER_TRUEJEDI = 2;
    POINTS_PER_REDBRICK = 1;
    POINTS_PER_MISSION = 2;
    POINTS_PER_CHEAT = 1;
    POINTS_PER_GOLDBRICK = 1;
    BOTHTRUEJEDIGOLDBRICKS = 0;
    SHOPGOLDBRICKS = 8;
    GOLDBRICKFORSUPERBONUS = 1;
    GOLDBRICKFORSUPERSTORY = 0;
    GOLDBRICKFORCHALLENGE = 1;

    if (EPISODECOUNT > 0) {
        i32 p = (EPISODECOUNT - 1) * POINTS_PER_SUPERSTORY;
        CompletionPointInfo[0] = POINTS_PER_SUPERSTORY + CompletionPointInfo[0] + p;
        COMPLETIONPOINTS = p + POINTS_PER_SUPERSTORY + COMPLETIONPOINTS;
    }

    AREADATA *pAVar10 = ADataList;

    if (0 < AREACOUNT) {
        i32 areaIndex;
        i32 areaId = 0;
        AREADATA *area = ADataList;
        u16 areaFlags;
        do {
            i32 episode = Episode_ContainsArea(areaId, &areaIndex);
            area->episode_index = episode;
            area->area_index = (byte)areaIndex;
            if (area != HUB_ADATA) {
                if ((areaFlags = area->flags, (areaFlags & 0x2022) == 0)) {
                    if ((areaFlags & 0x100) == 0) {
                        if ((areaFlags & 4) != 0) {
                            COMPLETIONPOINTS = COMPLETIONPOINTS + POINTS_PER_TIMETRIAL;
                            CompletionPointInfo[1] = CompletionPointInfo[1] + POINTS_PER_TIMETRIAL;
                            goto LAB_0012ca5f;
                        }
                        COMPLETIONPOINTS = POINTS_PER_STORY + COMPLETIONPOINTS;
                        CompletionPointInfo[1] = POINTS_PER_STORY + CompletionPointInfo[1];
                        if ((areaFlags & AREAFLAG_NO_GOLDBRICK) == 0) {
                            GOLDBRICKPOINTS = GOLDBRICKPOINTS + 1;
                        }

                        if ((areaFlags & 0x10) != 0) {
                            i32 local_24 = COMPLETIONPOINTS + POINTS_PER_MINIKIT + POINTS_PER_TRUEJEDI;
                            i32 iVar4 = GOLDBRICKPOINTS + 2;
                            i32 local_28 = POINTS_PER_MINIKIT + POINTS_PER_TRUEJEDI + CompletionPointInfo[1];
                            if (BOTHTRUEJEDIGOLDBRICKS != 0) {
                                local_24 = local_24 + POINTS_PER_TRUEJEDI;
                                local_28 = local_28 + POINTS_PER_TRUEJEDI;
                                iVar4 = GOLDBRICKPOINTS + 3;
                            }
                            COMPLETIONPOINTS = POINTS_PER_REDBRICK + local_24 + POINTS_PER_CHALLENGE;
                            GOLDBRICKPOINTS = iVar4;
                            CompletionPointInfo[1] = POINTS_PER_REDBRICK + POINTS_PER_CHALLENGE + local_28;
                            if (GOLDBRICKFORCHALLENGE != 0) {
                                GOLDBRICKPOINTS = GOLDBRICKPOINTS + 1;
                            }
                        } else {
                            if ((areaFlags & 0x4000) != 0) {
                                COMPLETIONPOINTS = COMPLETIONPOINTS + POINTS_PER_TRUEJEDI;
                                CompletionPointInfo[1] = POINTS_PER_TRUEJEDI + CompletionPointInfo[1];
                                GOLDBRICKPOINTS = GOLDBRICKPOINTS + 1;
                            }
                        }
                    } else {
                        COMPLETIONPOINTS = COMPLETIONPOINTS + POINTS_PER_SUPERBONUSCOMPLETE;
                        CompletionPointInfo[1] = CompletionPointInfo[1] + POINTS_PER_SUPERBONUSCOMPLETE;
                        if (GOLDBRICKFORSUPERBONUS == 0)
                            goto LAB_0012ca68;
                    LAB_0012ca5f:
                        GOLDBRICKPOINTS = GOLDBRICKPOINTS + 1;
                    }
                LAB_0012ca68:
                    if (area->cheat != 0xff) {
                        Cheat_SetArea((i32)(char)area->cheat, areaId);
                    }
                }
            }
            areaId = areaId + 1;
            area = area + 1;
        } while (areaId < AREACOUNT);
    }

    // char cVar1;
    // char cVar2;
    // ushort uVar3;
    // i32 iVar8;

    // i32 local_28;
    // i32 local_24;
    // i32 local_20[3];

    // i16 uVar4 = tUNKNOWN;

    AREADATA *pAVar2 = ADataList;
    i32 iVar3 = AREACOUNT;
    i32 areaId = LEVELCOUNT;

    // i16 tab = tUNKNOWN;
    AREADATA *area;
    u8 episode;
    u8 bVar1;

    i32 areaIndex;

    if (0 < LEVELCOUNT) {
        i32 i = 0;
        LEVELDATA *level = LDataList;
        do {
            if (static_cast<u16>(level->unknown_060) > 799) {
                level->unknown_060 = tUNKNOWN;
            }
            level->area_level_index = 0xff;
            level->area_index = 0xff;
            if (iVar3 < 1) {
                areaIndex = 0;
            } else {
                areaIndex = 0;
                area = pAVar2;
                do {
                    if (area->level_count != 0) {
                        i32 iVar4 = 0;
                        auto area_level_count = area->level_count;
                        do {
                            if (area->levels[iVar4] == i) {
                                level->area_level_index = (byte)iVar4;
                                level->area_index = (byte)areaIndex;
                                episode = (byte)areaIndex;
                            } else {
                                episode = level->area_index;
                            }
                            if (episode != 0xff) {
                                areaIndex = areaIndex + 1;
                                bVar1 = pAVar2[(char)episode].episode_index;
                                level->unknown_0ae = bVar1;
                                if ((pAVar2[(char)episode].flags & 1) != 0) {
                                    level->camera_judder_distance = 0x32;
                                    level->unknown_0da = 0x14;
                                }
                                if (((bVar1 != 0xff) && ((pAVar2[(char)episode].flags & 0x106) == 0)) &&
                                    ((level->flags & (LEVEL_STATUS | LEVEL_OUTRO | LEVEL_MIDTRO | LEVEL_INTRO)) == 0)) {
                                    level->flags |= 0x2000;
                                }
                                goto LAB_0012cb40;
                            }
                            iVar4 = iVar4 + 1;
                        } while (iVar4 < (i32)(u32)area_level_count);
                    }
                    areaIndex = areaIndex + 1;
                    area = area + 1;
                } while (areaIndex != iVar3);
            }
        LAB_0012cb40:
            level = level + 1;
            i = i + 1;
        } while (i < areaId);
    }

    Level_RegisterGameConfigKeywords(LevelConfigKeywords_BeforeLoad, &LevelConfigKeywords_AfterLoad);
    Suits_Init();

    Collection_Configure("chars\\collection.txt", &permbuffer_ptr, &permbuffer_end);
    LOG_INFO("Loaded %d collection items", COLLECTION_COMPLETIONCOUNT);

    CompletionPointInfo[2] = CompletionPointInfo[2] + POINTS_PER_CHARACTER * COLLECTION_COMPLETIONCOUNT;
    COMPLETIONPOINTS = COMPLETIONPOINTS + POINTS_PER_CHARACTER * COLLECTION_COMPLETIONCOUNT;

    MissionSys = Missions_Configure("levels\\missions.txt", &permbuffer_ptr, &permbuffer_end, &Game.mission_save);
    if (MissionSys != NULL) {
        areaId = (u32)MissionSys->count * POINTS_PER_MISSION;
        COMPLETIONPOINTS = COMPLETIONPOINTS + areaId;
        CompletionPointInfo[3] = CompletionPointInfo[3] + areaId;
        GOLDBRICKPOINTS = GOLDBRICKPOINTS + (u32)MissionSys->count;
    }

    Tag_DrawIconFn = Tag_DrawIcon_LSW;
    GlobalBoltType[0].object_ids[0] = 0x5d;
    GlobalBoltType[0].object_ids[1] = 0x5e;
    GlobalBoltType[1].object_ids[0] = 0x5f;
    GlobalBoltType[1].object_ids[1] = 0x60;
    GlobalBoltType[2].object_ids[0] = 0x61;
    GlobalBoltType[2].object_ids[1] = 0x62;
    DEFAULT_PLAYERHITPOINTS = 8;
    PLAYERHITPOINTS_2HEARTSIN1 = 1;
    CutScenes_InitSystem(&CutSceneSys_LSW);
    extern i32 NuGCutDebFixUp_SearchAllPages;
    NuGCutDebFixUp_SearchAllPages = 1;
    NarrowSockExceptions_Init(NarrowSockException_LSW);
    APIObjectRegisterAnimRedirect(reinterpret_cast<ANIMREDIRECTFN>(RedirectAnim), AnimRedirectList_LSW,
                                  const_cast<char *>("chars\\commonanims"));
    SurfaceMaskOff(reinterpret_cast<u32 *>(&TERRAINMASK_NONWEAPON));
    SurfaceMaskOn(reinterpret_cast<u32 *>(&TERRAINMASK_NONDROID));
    Hub_UsePlayerList = 1;
    BoltSys_Init(&BoltSys_LSW);
    GameAudio_Init(&GameAudio_LSW);
    extern f32 KITPOSX, KITPOS2X;
    KITPOSX = 0;
    KITPOS2X = 0;
    //  CONVERTOLDPICKUPS = 1;
    //  GrabScreenWhenFading = 1;
    troopers_gdeb[0] = 0x83;
    troopers_gdeb[1] = 0x49;
    troopers_gdeb[2] = 0x4a;
    troopers_gdeb[3] = 0x4b;
    LEGOHINT_SHOOTCAMERAS = 0x266;
    extern i32 LEGOHINT_PUSHBLOCKS;
    LEGOHINT_BUILD = 0x25c;
    LEGOHINT_PUSHBLOCKS = 0x267;
    LEGOHINT_FREEPLAYTOGGLE = 600;
    PUNCHGAP = 0.3f;
    PUNCHCHARGAP = 0.3f;
    //  f64Jump_AlwaysReachJump2Height = 1;
    //  f64Jump_JediSlam = 1;
    extern i32 CanPunchGirls;
    CanPunchGirls = 0;
    //  ExtraHurtSfxFn = ExtraHurtSfx_LSW;
    //  ExtraDieSfxFn = ExtraDieSfx_LSW;
    REDBRICKPOSX = 0.0f;
    BuckStartExtraFn = BuckStartExtra_LSW;
    BoltInitSfxFn = BoltInitSfx_LSW;
    REDBRICKPOSY = -0.5f;
    REDBRICKPOS2X = 1.25f;
    REDBRICKPOS2Y = 0.0f;
    GameObjectDimensionsExtraFn = GameObjectDimensionsExtra_LSW;
    Punch_GetDamageFn = Punch_GetDamage_LSW;
    Punch_HitHoldFn = Punch_HitHold;
    Punch_HitExtraCodeFn = Punch_HitExtraCode_LSW;
    SetSoundFadeDistCallBackFn = SetSoundFadeDistCallBackFn_LSW;
    PlayerItemTypes_Init(PlayerItemType_INDY);
    DisguiseAdjustFn = DisguiseAdjust_LSW;
    extern f32 SUPERCARRY_THROWSPEED_XZ;
    extern f32 SUPERCARRY_THROWSPEED_Y;
    extern f32 SUPERCARRY_RELEASESPEED_XZ;
    extern i32 SuperCarry_UseActionButton;
    SUPERCARRY_THROWSPEED_XZ = 2.0f;
    SUPERCARRY_RELEASESPEED_XZ = 1.0f;
    SUPERCARRY_THROWSPEED_Y = 1.5f;
    SuperCarry_UseActionButton = 1;
    //  CutScene_OverrideConfigFileNameFn = CutScene_OverrideConfigFileName_LSW;
    CharPivot_Init(CharPivot_LSW);
    DIEAIRSPEED = 3.5f;
    DIEAIRJUMPSPEED = 2.5f;
    ForcePush_Waft = 1;
    ForcePush_SuperPush = 1;
    ForcePush_SuperMindTrick = 1;
    Lighting_HighlightFlash = 1;
    Lighting_BlueFlickerFn = ObjZappedBlue;
    //  AddGameMsg_Default._56_4_ = GameMsg_EndDelay_Game;
    WorldInfo_InitMenuFn = Game_WorldInfo_InitMenu;
    WorldInfo_InitLastFn = Game_WorldInfo_InitLast;
    *reinterpret_cast<u16 *>(&TerSurface[12].field_0x08) = 0x15;
    *reinterpret_cast<u16 *>(&TerSurface[13].field_0x08) = 0x16;
    LEGOACT_IDLE = 1;
    LEGOACT_WALK = 0;
    LEGOACT_JUMP = 6;
    LEGOACT_LAND = 7;
    LEGOACT_JUMP2 = 9;
    LEGOACT_JUMP3 = 0xe;
    LEGOACT_LAND2 = 10;
    LEGOACT_LAND3 = 0x15;
    LEGOACT_COMBOJUMP = 0x12;
    LEGOACT_COMBOLAND = 0x13;
    LEGOACT_FALL = 5;
    LEGOACT_FALLLAND = 0x59;
    LEGOACT_BACKPACKFALLLAND = 0xb3;
    LEGOACT_BACKFLIP = 0x77;
    LEGOACT_DEACTIVATED = 0x41;
    LEGOACT_PUNCH_BEHIND = 0x94;
    LEGOACT_SHOOTRIGHT = 0x5b;
    LEGOACT_SHOOTLEFT = 0x5a;
    LEGOACT_SHOOTBACK = 0x5c;
    LEGOACT_LEDGE_GRAB = 0xda;
    LEGOACT_LEDGE_IDLE = 0x9d;
    LEGOACT_LEDGE_LEFT = 0x9e;
    LEGOACT_LEDGE_RIGHT = 0x9f;
    LEGOACT_HANG_IDLE = 0x91;
    LEGOACT_HANG_MOVE = 0x92;
    LEGOACT_CLIMB_IDLE = 0x87;
    LEGOACT_CLIMB_UP = 0xcd;
    LEGOACT_CLIMB_DOWN = 0xce;
    LEGOACT_CLIMB_LEFT = 0xcf;
    LEGOACT_CLIMB_RIGHT = 0xd0;
    LEGOACT_WALLSHUFFLE_IDLE = 0x8a;
    LEGOACT_WALLSHUFFLE_LEFT = 0x8b;
    LEGOACT_WALLSHUFFLE_RIGHT = 0x8c;
    LEGOACT_PUSH = 0x52;
    LEGOACT_SUPERPUSH_IDLE = 0xae;
    LEGOACT_SUPERPUSH_PUSH = 0xaf;
    LEGOACT_SUPERPUSH_PULL = 0xb0;
    LEGOACT_BUILD = 0x5f;
    LEGOACT_FLIP = 0xc;
    LEGOACT_FLIPLAND = 0xd;
    LEGOACT_BACKFLIP = 0x77;
    LEGOACT_WALLJUMP_WAIT = 0xa1;
    LEGOACT_GRAPPLE_IDLE = 0xba;
    LEGOACT_GRAPPLE_UP = 0xbb;
    LEGOACT_GRAPPLE_DOWN = 0xbc;
    LEGOACT_GRAPPLE_HANG = 0xbd;
    LEGOACT_GLIDE = 0x93;
    LEGOACT_SUPERCARRY_PICKUP = 0xa3;
    LEGOACT_SUPERCARRY_IDLE = 0xa4;
    LEGOACT_SUPERCARRY_WALK = 0xa5;
    LEGOACT_SUPERCARRY_THROW = 0xa6;
    LEGOACT_SUPERCARRY_PUTDOWN = 0xd5;
    LEGOACT_SUPERCARRY_BASH = 0xd6;
    LEGOACT_SUPERCARRY_JUMP = 0xd7;
    LEGOACT_SUPERCARRY_LAND = 0xd8;
    LEGOACT_SUPERCARRY_FALLLAND = 0xd9;
    LEGOACT_MAGNET_WALK_METAL = 0xc5;
    LEGOACT_MAGNET_TIPTOE = 0xc6;
    LEGOACT_MAGNET_WALK = 199;
    LEGOACT_MAGNET_RUN = 200;
    LEGOACT_MAGNET_JUMP = 0xc9;
    LEGOACT_MAGNET_LAND = 0xca;
    LEGOACT_LUNGE = 0x1f;
    LEGOACT_LUNGELAND = 0x20;
    LEGOACT_SLAM = 0x21;
    LEGOCONTEXT_JUMP = 0;
    DoubleJump_JediSlam = 1;
    DoubleJump_AlwaysReachJump2Height = 1;
    LEGOACT_SLAMLAND = 0x22;
    LEGOACT_COMBATROLL_JUMP = 0xb4;
    LEGOACT_COMBATROLL_FALL = 0xb5;
    LEGOACT_COMBATROLL_LAND = 0xb6;
    LEGOACT_COMBATROLL_FIRE = 0x57;
    LEGOACT_EXTRA_JUMP = 0x78;
    LEGOACT_EXTRA_JUMP2 = 0x79;
    LEGOACT_EXTRA_LAND = 0x7a;
    LEGOACT_EXTRA_LAND2 = 0x7b;
    LEGOACT_BUCK = 0x2a;
    LEGOACT_TEETER = 0xdb;
    LEGOACT_WHIP_START = 0xdc;
    LEGOACT_WHIP_CRACK = 0xdd;
    LEGOACT_WHIP_GRAB = 0xdd;
    LEGOACT_WHIP_BREAK = 0xdd;
    LEGOACT_WHIP_SWING_START = 0xe0;
    LEGOACT_WHIP_SWING_SWING = 0xe1;
    LEGOACT_WHIP_SWING_JUMP = 0xe2;
    LEGOCONTEXT_TUBE = 0x11;
    LEGOCONTEXT_GLIDE = 0x4f;
    LEGOCONTEXT_BLOCK = 0xc;
    LEGOCONTEXT_HOLD = 0x18;
    LEGOCONTEXT_DROPIN = 0x23;
    LEGOCONTEXT_DOOMED = 0x2b;
    LEGOCONTEXT_LEDGETERRAIN = 0x55;
    LEGOCONTEXT_CLIMB = 0x43;
    LEGOCONTEXT_HANG = 0x4e;
    //  LEGOCONTEXT_JUMP = 0;
    LEGOCONTEXT_BIGJUMP = 0x1f;
    LEGOCONTEXT_WALLSHUFFLE = 0x45;
    LEGOCONTEXT_COMBO = 5;
    LEGOCONTEXT_PUNCH = 0x26;
    LEGOCONTEXT_PUSH = 0x27;
    LEGOCONTEXT_PUSHSPINNER = 0x28;
    LEGOCONTEXT_PUSHOBSTACLE = 0x59;
    LEGOCONTEXT_BEENTAKENOVER = 0x3b;
    LEGOCONTEXT_GETIN = 0x3c;
    LEGOCONTEXT_BUILDIT = 0x2d;
    LEGOCONTEXT_WEAPONIN = 6;
    LEGOCONTEXT_WEAPONOUT = 7;
    LEGOCONTEXT_BACKFLIP = 0x20;
    LEGOCONTEXT_WALLJUMPWAIT = 0x57;
    LEGOCONTEXT_GRAPPLE = 0x46;
    LEGOCONTEXT_SUPERCARRY = 0x58;
    LEGOCONTEXT_LAND_JUMP = 1;
    LEGOCONTEXT_LAND_JUMP2 = 2;
    LEGOCONTEXT_LAND_FLIP = 3;
    LEGOCONTEXT_LAND_COMBOJUMP = 4;
    LEGOCONTEXT_LAND_LUNGE = 0xd;
    LEGOCONTEXT_LAND_SLAM = 0xe;
    LEGOCONTEXT_LAND_SPECIAL = 0x19;
    LEGOCONTEXT_LAND_COMBATROLL = 0x29;
    LEGOCONTEXT_EATEN = 0x39;
    LEGOCONTEXT_SPECIALMOVE_ATTACKER = 0x26;
    LEGOCONTEXT_SPECIALMOVE_VICTIM = 0x30;
    LEGOCONTEXT_BUCK = 0x3e;
    LEGOCONTEXT_WHIP = 0x62;
    LEGOCONTEXT_NETWAIT = 99;
    //  LEGOMENU_TITLES = 0;
    LEGOMENU_NEWGAME = LEGO_MENU_NEW_GAME;
    LEGOMENU_PAUSEMAIN = LEGO_MENU_PAUSE_MAIN;
    LEGOMENU_PAUSECUT = LEGO_MENU_PAUSE_CUTSCENE;
    LEGOMENU_CREDITS = LEGO_MENU_CREDITS;
    //  LEGOMENU_STORE_RESTORING = 0x16;
    //  LEGOMENU_STORE_PURCHASE = 0x17;
    //  LEGOMENU_RESTORE_NEWGAME = 0x1d;
    LEGOCAMMODE_DOORCUT = 4;
    LEGOCAMMODE_OBSTACLE = 2;
    extern i32 LEGOOBJ_ICON_FRAME_NEUTRAL, LEGOOBJ_ICON_FRAME_GREEN, LEGOOBJ_ICON_FRAME_BLUE;
    LEGOOBJ_ICON_FRAME_NEUTRAL = 0xa7;
    LEGOOBJ_ICON_FRAME_GREEN = 0xa5;
    LEGOOBJ_ICON_FRAME_BLUE = 0xa6;
    LEGOOBJ_ICON_QUESTION = 0xa1;
    LEGOOBJ_ICON_WEIRDO = 0xa3;
    extern i32 LEGOOBJ_CHARKIT, LEGOOBJ_MINIKIT;
    LEGOOBJ_CHARKIT = 0xcf;
    LEGOOBJ_MINIKIT = 0xce;
    LEGOSPL_SPLIT = 5;
    LEGOSPL_START = 0;
    //  LEGOGDEB_SPLASH = 0xd;
    GizBuilditGDeb[0] = 0x4c;
    GizBuilditGDeb[1] = 0x4d;
    GizBuilditGDeb[2] = 0x4e;
    GizBuilditGDeb[3] = 0x4f;
    GizBuilditGDeb[4] = 0x50;
    GizBuilditGDeb[5] = 0x51;
    extern i16 GizSpinnerGDeb_Fail[3];
    GizSpinnerGDeb_Fail[0] = 0x57;
    GizSpinnerGDeb_Fail[1] = 1;
    GizSpinnerGDeb_Fail[2] = 0x58;
    GamePads_IgnoreInputFn = Game_IgnoreInput;
    Door_GoThrough_ExtraCodeFn = GoThroughDoor_ExtraCode;
    GizmoBlowup_TransformDrawFn = GizmoBlowup_TransformDraw_Game;
    LEGOASCII_UP = ASCII_UP;
    extern char *ASCII_DOWN;
    LEGOASCII_DOWN = ASCII_DOWN;
    //  LEGOASCII_LEFT = ASCII_LEFT;
    //  LEGOASCII_RIGHT = ASCII_RIGHT;
    //  LEGOASCII_BIGARROW = ASCII_BIGARROW;
    KEEPONSCREEN_SIDESONLY = 1;
    CutScenePlayer_AcceptFn = CutScenePlayer_Accept;
    CutScenePlayer_Configure("cut\\clips.txt", &permbuffer_ptr, &permbuffer_end, &tCLIPi, &tINTRO, &tMIDTRO, &tOUTRO,
                             &tENDING);
    CanMagnetClimbFn = CanMagnetClimb_Game;
    CanPushObstaclesFn = CanPushObstacles_Game;
    CanSuperCarryFn = CanSuperCarry_Game;
    CanPushBlocksFn = CanPushBlocks_Game;
    CanGlideFn = CanGlide_Game;
    UsingExtraActionsFn = UsingExtraActions_Game;
    CanStartHoldFn = CanStartHold_Game;
    Player_ClearContextFn = Player_ClearContext_Game;
    extern i32 LEGOTHINGSSCENE_TER_SPINBASE;
    extern i32 LEGOTHINGSSCENE_TER_SPINARM;
    LEGOTHINGSSCENE_TER_SPINBASE = 0;
    LEGOTHINGSSCENE_TER_SPINARM = 1;
    GizBuildit_AutoBuildPosFn = GizBuildit_AutoBuildPos_Game;
    Fighting_WeaponInActionFn = Fighting_WeaponInAction_Game;
    Fighting_WeaponOutActionFn = Fighting_WeaponOutAction_Game;
    LEGOOBJ_FLOORTARGET = 0x55;
    LEGOOBJ_GRAPPLE_HOOK = 0x3d;
    Slam_GetDebrisFn = Slam_GetDebris_Game;
    IsWearingBackPackFn = IsWearingBackPack_Game;
    extern i32 Grass_Available;
    Grass_Available = 1;
    //  PauseGame_ExtraCodeFn = PauseGame_ExtraCode;
    CheckMusicOtherFn = CheckMusicOther;
    Hub_PanelBusyFn = Hub_PanelBusy;
    GizBuildIt_CanStartBuildingFn = GizBuildIt_CanStartBuildingFn_Game;
    GizBuildIt_FinishFn = GizBuildIt_FinishFn_Game;
    extern i16 tPOWERUP;
    POWERUP_TEXTID = tPOWERUP;
    //  BackDrop_AlphaFn = BackDrop_Alpha;
    //  Game_100PercentFn = LEGO_100PercentFn;
    //  Game_AllGoldBricksFn = LEGO_AllGoldBricksFn;
    LastSafePosExtraFn = LastSafePosExtra;
    GameAudio_ActionMusicFn = ActionMusicFn;
    //  PauseRenderOffFn = PauseRenderOff;
    FindSlamOrigin_UseCPosFn = FindSlamOrigin_UseCPos;
    GizmoBlowUp_NoTargetFn = GizmoBlowUp_NoTarget;
    //  MatrixReflection_CanOverrideFn = MatrixReflection_CanOverride;
    Jump_PreventJumpFn = Jump_PreventJump;
    //  SurfaceInfo_ExtraReflectFn = SurfaceInfo_ExtraReflect;
    Hint_AlphaTargetFn = Hint_AlphaTarget;
    //  Arcade_TextCrawlID = 0x1f1;
    //  Arcade_TextCrawlParagraphs = 2;
    //  GizmoPickups_Collide2DFn = GizmoPickups_Collide2D;
    GizmoBlowUp_SfxFn = GizmoBlowUp_Sfx;
    LEGOOBJ_DEFAULTLASTCOIN = 0xb7;
    //  Collection_GetSelectingPlayerIDsFn = Collection_GetSelectingPlayerIDs;
    Tag_NoHiddenIconFn = Tag_NoHiddenIcon;
    APIObjResetShadowMapRenderingFn = ResetShadowMapRenderingFn;
    APIObjEnableShadowMapRenderingFn = EnableShadowMapRenderingFn;
    //  GameMsg_GetExtraObjFn = GameMsg_GetExtraObj;
    Jump_EndOfLandContextFn = Jump_EndOfLandContext;
    BigJump_EndOfLandFn = BigJump_EndOfLand;

    COMPLETIONPOINTS = POINTS_PER_CHEAT * 0x2c + COMPLETIONPOINTS;
    CompletionPointInfo[4] = POINTS_PER_CHEAT * 0x2c + CompletionPointInfo[4];
    if (0 < SHOPGOLDBRICKS) {
        areaId = (SHOPGOLDBRICKS + -1) * POINTS_PER_GOLDBRICK;
        CompletionPointInfo[6] = CompletionPointInfo[6] + POINTS_PER_GOLDBRICK + areaId;
        COMPLETIONPOINTS = POINTS_PER_GOLDBRICK + COMPLETIONPOINTS + areaId;
        GOLDBRICKPOINTS = SHOPGOLDBRICKS + -1 + GOLDBRICKPOINTS + 1;
    }

    CUTSCENEPLAYER_s *clip_player = static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available());
    CutScenePlayCount = clip_player != NULL ? clip_player->clip_count : 0;
    if (g_lowEndLevelBehaviour != 0) {
        Reflections_On = 0;
        extern u8 CharClipToBlobShadows;
        CharClipToBlobShadows = 1;
    }
}

void LSW_SetIndy(i32) {
    LedgeTerrain_On = 0;
    Grapples_Available = 0;
    SuperCarry_Bash = 0;
    SuperCarry_Jump = 0;
    PUNCHGAP = 0.3f;
    PUNCHCHARGAP = 0.3f;
    HINTS_ON = 1;

    TechnoSys.interaction_time = 0.2f;
    TechnoSys.idle_offset = v000;
    TechnoSys.active_offset = v000;
    TechnoSys.active_effect_id = 0x2c;
    TechnoSys.success_effect_id = -1;
    TechnoSys.failure_effect_id = -1;
    TechnoSys.complete_offset = v000;
    TechnoSys.floor_target_object_id = LEGO_OBJECT_FLOOR_TARGET;
    TechnoSys.activation_effect_id = 0x2d;
    TechnoSys.completion_effect_id = 0x2e;

    LeverSys.floor_target_object_id = LEGO_OBJECT_FLOOR_TARGET;
    GameAudio_SetActionMusicTimes(1.0f, 6.0f);
}

void ReCalculateCompletionPoints() {
    Game.completion = 0;
    Game.gold_bricks = 0;
    memset(CompletionPointInfo_ReCalculate, 0, sizeof(CompletionPointInfo_ReCalculate));

    for (i32 index = 0; index < MasterCollection.count_y; ++index) {
        COLLECTID *entry = &MasterCollection.list[index];
        if (entry->field5_0x9 == 0) {
            continue;
        }
        CompletionPointInfo_ReCalculate[2] += POINTS_PER_CHARACTER;
        if (Collection_Got(entry->id) != 0) {
            AddToCompletionPoints(POINTS_PER_CHARACTER);
        }
    }

    AREADATA *area = ADataList;
    AREASAVE_s *save = Game.area_save;
    for (i32 index = 0; index < AREACOUNT; ++index, ++area, ++save) {
        if (area == HUB_ADATA ||
            (area->flags & (AREAFLAG_ENDING_AREA | AREAFLAG_TEST_AREA | AREAFLAG_NO_COMPLETION_POINTS)) != 0) {
            continue;
        }

        if ((area->flags & 0x100) != 0) {
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_SUPERBONUSCOMPLETE;
            if (save->area_complete != 0) {
                AddToCompletionPoints(POINTS_PER_SUPERBONUSCOMPLETE);
                if (GOLDBRICKFORSUPERBONUS != 0) {
                    AddToGoldBricks();
                }
            }
            continue;
        }

        if ((area->flags & AREAFLAG_BONUS_AREA) != 0) {
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_TIMETRIAL;
            if (save->area_complete != 0 ||
                save->challenge_trial_time < static_cast<f32>(static_cast<i32>(area->challenge_trial_time))) {
                AddToCompletionPoints(POINTS_PER_TIMETRIAL);
                AddToGoldBricks();
            }
            continue;
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_STORY;
        if (save->area_complete != 0) {
            AddToCompletionPoints(POINTS_PER_STORY);
            if ((area->flags & AREAFLAG_NO_GOLDBRICK) == 0) {
                AddToGoldBricks();
            }
        }

        if ((area->flags & AREAFLAG_MINIKIT) == 0) {
            if ((area->flags & AREAFLAG_TRUE_JEDI) != 0 &&
                (save->true_hero_complete[0] != 0 || save->true_hero_complete[1] != 0)) {
                AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                AddToGoldBricks();
            }
            continue;
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_MINIKIT;
        if (save->minikit_complete != 0) {
            AddToCompletionPoints(POINTS_PER_MINIKIT);
            AddToGoldBricks();
        }

        if (BOTHTRUEJEDIGOLDBRICKS != 0) {
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_TRUEJEDI;
            if (save->true_hero_complete[0] != 0) {
                AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                AddToGoldBricks();
            }
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_TRUEJEDI;
            if (save->true_hero_complete[1] != 0) {
                AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                AddToGoldBricks();
            }
        } else {
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_TRUEJEDI;
            if (save->true_hero_complete[0] != 0 || save->true_hero_complete[1] != 0) {
                AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                AddToGoldBricks();
            }
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_REDBRICK;
        if (save->red_brick_collected != 0) {
            AddToCompletionPoints(POINTS_PER_REDBRICK);
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_CHALLENGE;
        if (save->reserved_0x7 != 0) {
            AddToCompletionPoints(POINTS_PER_CHALLENGE);
            if (GOLDBRICKFORCHALLENGE != 0) {
                AddToGoldBricks();
            }
        }
    }

    if (MissionSys != NULL && MissionSys->count != 0) {
        for (i32 index = 0; index < MissionSys->count; ++index) {
            CompletionPointInfo_ReCalculate[3] += POINTS_PER_MISSION;
            if (Game.mission_save.completed[index] != 0) {
                AddToCompletionPoints(POINTS_PER_MISSION);
                AddToGoldBricks();
            }
        }
    }

    for (i32 index = 0; index < EPISODECOUNT; ++index) {
        CompletionPointInfo_ReCalculate[0] += POINTS_PER_SUPERSTORY;
        if ((Game.episode_save[index].flags & SAVE_SUPERSTORY_COMPLETE) != 0) {
            AddToCompletionPoints(POINTS_PER_SUPERSTORY);
            if (GOLDBRICKFORSUPERSTORY != 0) {
                AddToGoldBricks();
            }
        }
    }

    for (i32 index = 0; index < 44; ++index) {
        CompletionPointInfo_ReCalculate[4] += POINTS_PER_CHEAT;
        if ((Game.extra_purchased_bits[index >> 5] & (1U << (index & 31))) != 0) {
            AddToCompletionPoints(POINTS_PER_CHEAT);
        }
    }

    for (i32 index = 0; index < SHOPHINTCOUNT; ++index) {
        HINT_s *hint = Hint_FindHint(HintTab[index]);
        if (hint == NULL || hint->shop_price == 0) {
            continue;
        }
        CompletionPointInfo_ReCalculate[5] += POINTS_PER_HINT;
        if ((Game.shop_hint_purchased_bits[index >> 5] & (1U << (index & 31))) != 0) {
            AddToCompletionPoints(POINTS_PER_HINT);
        }
    }

    for (i32 index = 0; index < SHOPGOLDBRICKS; ++index) {
        CompletionPointInfo_ReCalculate[6] += POINTS_PER_GOLDBRICK;
        if ((Game.shop_gold_brick_purchased_bits & (1U << (index & 31))) != 0) {
            AddToCompletionPoints(POINTS_PER_GOLDBRICK);
            AddToGoldBricks();
        }
    }
}

void NewGame() {
    TempOptions = Game.options_save;
    memset(&Game, 0, sizeof(Game));
    Game.options_save = TempOptions;

    MenuLoadOccurred = 0;
    MenuSaveOccurred = 0;
    ResetAdaptiveDifficulty();
    Game.save_version = 5;

    if (EDataList != NULL) {
        Game.area_save[EDataList[0].area_ids[0]].complete = 1;
    } else {
        Game.area_save[0].complete = 1;
    }

    if (SENATE_ADATA != NULL) {
        Game.area_save[SENATE_ADATA->index].complete = 1;
    }
    if (UTAPAU_ADATA != NULL) {
        Game.area_save[UTAPAU_ADATA->index].complete = 1;
    }
    if (HOTH_ADATA != NULL) {
        Game.area_save[HOTH_ADATA->index].complete = 1;
    }
    if (BONUSDAGOBAH_ADATA != NULL) {
        Game.area_save[BONUSDAGOBAH_ADATA->index].complete = 1;
    }
    if (BONUSKAMINO_ADATA != NULL) {
        Game.area_save[BONUSKAMINO_ADATA->index].complete = 1;
    }
    if (BONUSKASHYYYK_ADATA != NULL) {
        Game.area_save[BONUSKASHYYYK_ADATA->index].complete = 1;
    }

    Cheats_TurnOff(0);
    Hint_ClearHintsAndDoneFlags();
    LSW_HintConditions.force_used = 0;
    LSW_HintConditions.tc14_present = 0;
    LSW_HintConditions.panel_used = 0;
    GamePad_InitButtons();
    Tag_DoneFirst = 0;
    Tag_DoneAny = 0;

    for (i32 i = 0; i < AREACOUNT; ++i) {
        Game.area_save[i].challenge_trial_time = static_cast<f32>(ADataList[i].challenge_trial_time);
    }

    Game.episode_save[0].superstory_time_limit = kSuperStoryTimeLimit;
    Game.episode_save[1].superstory_time_limit = kSuperStoryTimeLimit;
    Game.episode_save[2].superstory_time_limit = kSuperStoryTimeLimit;
    Game.episode_save[3].superstory_time_limit = kSuperStoryTimeLimit;
    Game.episode_save[4].superstory_time_limit = kSuperStoryTimeLimit;
    Game.episode_save[5].superstory_time_limit = kSuperStoryTimeLimit;
    Game.episode_save[0].superstory_score_target = kSuperStoryScoreTarget;
    Game.episode_save[1].superstory_score_target = kSuperStoryScoreTarget;
    Game.episode_save[2].superstory_score_target = kSuperStoryScoreTarget;
    Game.episode_save[3].superstory_score_target = kSuperStoryScoreTarget;
    Game.episode_save[4].superstory_score_target = kSuperStoryScoreTarget;
    Game.episode_save[5].superstory_score_target = kSuperStoryScoreTarget;

    Customiser_CopyDefaultPiecesToSave(CharacterCustomiser, Game.customizer);
    if (TTab != NULL) {
        if (TTab[kPrimaryCustomNameTextId] != NULL) {
            NuStrCpy(Game.customizer[0].name, TTab[kPrimaryCustomNameTextId]);
        }
        if (TTab[kSecondaryCustomNameTextId] != NULL) {
            NuStrCpy(Game.customizer[1].name, TTab[kSecondaryCustomNameTextId]);
        }
        FinishWeirdoNames(-1);
    }

    if (id_DEFAULTCHARACTER[0] != -1) {
        Game.character_save[id_DEFAULTCHARACTER[0]] |= SAVE_CHARACTER_AVAILABLE | SAVE_CHARACTER_UNLOCKED;
    }
    if (id_DEFAULTCHARACTER[1] != -1) {
        Game.character_save[id_DEFAULTCHARACTER[1]] |= SAVE_CHARACTER_AVAILABLE | SAVE_CHARACTER_UNLOCKED;
    }
    if (id_WEIRDO1 != -1) {
        Game.character_save[id_WEIRDO1] |= SAVE_CHARACTER_AVAILABLE | SAVE_CHARACTER_UNLOCKED;
    }
    if (id_WEIRDO1 != -1 && CDataList != NULL) {
        CDataList[id_WEIRDO1].field0_0x0 = kPrimaryCustomNameTextId;
    }
    if (id_WEIRDO2 != -1) {
        Game.character_save[id_WEIRDO2] |= SAVE_CHARACTER_AVAILABLE | SAVE_CHARACTER_UNLOCKED;
    }
    if (id_WEIRDO2 != -1 && CDataList != NULL) {
        CDataList[id_WEIRDO2].field0_0x0 = kSecondaryCustomNameTextId;
    }

    Game.customizer[0].use_saved_name = 1;
    Game.customizer[1].use_saved_name = 1;

    u8 previous_group = 0;
    for (i32 i = 0; i < 10; ++i) {
        if (Suit[i].group != previous_group) {
            Game.initial_store_pack_flags |= 1u << i;
        }
        previous_group = Suit[i].group;
    }

    for (i32 i = 0; i < 11; ++i) {
        if ((static_cast<u16>(SuperOptions.field0_0x0) & (1u << i)) != 0) {
            Store_UnlockPack(i, true);
        }
    }

    ReCalculateCompletionPoints();
}

void InitChallenge(i32) {
    ChallengeMode = 1;
    ResetTimer(&ChallengeTimer, 0.0f);
    Cheats_TurnOff(0);
    AreaGlobals.values.field_0x1c = 0;
}

void MenuUpdateEndChallenge(MENU_s *menu) {
    if (menu->menu_time >= 5.0f) {
        CompleteLevel(WORLD);
    } else if (menu->menu_time >= 1.5f && BonusWinFlag == 0) {
        PlaySfx(const_cast<char *>("Victory"), NULL);
        BonusWinFlag = 1;
    }
}

void MenuDrawEndChallenge(MENU_s *) {
    if (MenuStopDraw != 0)
        return;
    if (ChallengeMode != 2) {
        SmartText(TTab[tOUTOFTIME], 0.0f, STATSPOSY, 1.0f, 1.0f, 1.0f, 1.0f, 0,
                  191 + (static_cast<u32>(menu_flash) < 1 ? 64 : 0), 31 + (static_cast<u32>(menu_flash) < 1 ? 32 : 0),
                  0, 1.7f, 1);
    } else if (NuFmod(GameTimer.time_elapsed, 0.3f) < 0.2f) {
        char text[64];
        f32 remaining = static_cast<f32>(static_cast<i32>(ADataList[WORLD->level_sub_id].challenge_trial_time)) -
                        ChallengeTimer.time_elapsed;
        if (remaining < 0.0f)
            remaining = 0.0f;
        Text_MakeTime(remaining, 0, 1, 1, text);
        Text3D(text, 0.0f, STATSPOSY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0);
    }
    DrawMiniKitCount(minikittime, MiniKitScale, AreaGlobals.values.field_0x20, 10);
}

void SetBonusWinner(i32 player) {
    BonusWinner = player;
    GameCam_Blend(GameCam, 1.5f, 0.0f, 1);
    LookAtBoth = 1;
}

void EndChallenge(i32 param1, i32 param2) {
    if (netclient == 0 || param2 == 0) {
        ChallengeMode = param1;
        SetBonusWinner(qrand() / 0x8000);
        if (Player[BonusWinner] == NULL || !Player[BonusWinner]->apiobj.player_controlled) {
            BonusWinner = (BonusWinner == 0);
        }
        BonusWinFlag = 0;
        NewMenu(10, -1, -1);
        ResetGameMessages();
        if (param1 == 3) {
            PlaySfx("MK-Panel", 0);
        }
    }
}

void BuyAllShopExtras() {
    WORLDINFO_s *world = WORLD;
    shopitem_s *item = ExtraItems;
    for (i32 i = 0; i < 44; ++i, ++item) {
        Game.purchased_extra_bits[i >> 5] |= 1u << (i & 31);
        if (world->current_level == HUB_LDATA)
            item->unlocked = 1;
        i8 area = static_cast<i8>(Cheat[i].area);
        if (area != -1 && Game.area_save[area].red_brick_collected == 0)
            Game.area_save[area].red_brick_collected = 1;
    }
    Game.unlocked_extra_bits[0] = 0xffffffff;
    Game.unlocked_extra_bits[1] = 0xffffffff;
}

static void GizBuildIt_FinishFn_Game(GIZBUILDIT_s *buildit) {
    if (WORLD->area != NULL && (WORLD->area->flags & 0x40) != 0 && LevBuildIt[0] == buildit &&
        (Game.field_0x7c26[2] & 0x80) == 0) {
        AddFancyMessageRGB(TTab[tALLEXTRASUNLOCKED], 0.0f, 0.0f, 0.6f, 1.25f, 1, 255, 0, 0);
        BuyAllShopExtras();
        ReCalculateCompletionPoints();
        AddPartDebris(WORLD->part_debris_sys, 15, &LevBuildIt[0]->position);
    }
}

void InitGameMode() {
    if ((WORLD->current_level->flags & LEVEL_GAMEPLAY) != 0) {
        MenuReset();
    }

    GameObject_s *player = Player[0];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }
    player = Player[1];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }
    player = Player[2];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }
    player = Player[3];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }
    player = Player[4];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }
    player = Player[5];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }
    player = Player[6];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }
    player = Player[7];
    if (player != NULL && !player->not_with_party) {
        ResetPlayer(player, 1, NULL, 1);
    }

    f32 remember_speed = GetVehicleAreaRememberSpeed();
    if (VehicleAreaRememberSpeed < remember_speed) {
        VehicleAreaRememberSpeed = remember_speed;
    }
    AveragePlayerCurrentSpeedMul();
    SetPlayer();

    AREA_GLOBAL_VALUES &area_state = AreaGlobals.values;
    area_state.field_0x14 = WORLD->area != NULL ? area_state.field_0x0c : 0;
    area_state.field_0x20 = area_state.field_0x1c;
    NewLData = NULL;
    NewMode = 0;

    if (Player[0] != NULL && Player[0]->coinpacket != NULL) {
        GIZMO_PICKUP_TYPE *pickup_type = &GizmoPickupType[GetRandomCoinType()];
        Player[0]->coinpacket->lastcoin = pickup_type->first_model_id;
        if (pickup_type->random_model_count != 0) {
            Player[0]->coinpacket->lastcoin += qrand() / (0xffff / pickup_type->random_model_count + 1);
        }
    }

    if (Player[1] != NULL && Player[1]->coinpacket != NULL) {
        GIZMO_PICKUP_TYPE *pickup_type = &GizmoPickupType[GetRandomCoinType()];
        Player[1]->coinpacket->lastcoin = pickup_type->first_model_id;
        if (pickup_type->random_model_count != 0) {
            Player[1]->coinpacket->lastcoin += qrand() / (0xffff / pickup_type->random_model_count + 1);
        }
    }

    GameObject_s *object = Obj;
    const u16 player_object_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
        if ((object->apiobj.field_0x1f8 & player_object_flags) == player_object_flags) {
            object->reset_lighting = 1;
        }
    }

    GameFog_Reset();
    ConfigureComplexShadow(NULL);
}

void CheckResetBits() {
    if ((ResetBits & RESETBIT_CLEAR_LEVEL_PROGRESS) != 0 && WORLD->current_level->area_level_index != -1) {
        ClearLevelProgress(WORLD->current_level->area_level_index, WORLD);
    }

    Cheats_Reset();
    if (WORLD->level_progress != NULL) {
        ResetScene(WORLD->current_gscn, reinterpret_cast<SCENEPROGRESS_s *>(WORLD->level_progress));
        WORLD->level_progress->flags_low |= 1;
    }

    GizmoBlowupVisibilityOverrides(WORLD);
    texanimbits = 0;
    SetTexAnimSignals();
    SetTexAnimSignals();
    CutScenes_Reset(WORLD);

    if (NOSOUND == 0) {
        if ((ResetBits & RESETBIT_USE_CUSTOMISER_SETUP) == 0) {
            InitGameMode();
        }
        if (NOSOUND == 0) {
            Customiser_SetUpCharacterData(CharacterCustomiser);
        }
    }

    Surfaces_Reset();
    ResetStreaks();
    Bolts_Reset();
    Batarangs_Reset();
    Detonators_Reset();
    ResetParts();
    ResetExplosions();
    ShoveObjectSysReset();
    Panel_Clear();
    GameCam_Reset(GameCam);
    ResetGameMessages();
    Tag_ResetTransfers();
    Hint_Reset();
    Hint_CancelCurrent();
    Teleports_Reset(WORLD);
    TrafficAnimSys_Reset(WORLD->trafficanim_sys);
    Pulses_Reset(WORLD->pulses_sys);
    NuSound3StopRumble();
    ResetTimer(&GameTimer, 0.0f);
    ResetTimer(&GamePlayTimer, 0.0f);
    ResetTimer(&PauseTimer, 0.0f);
    ResetTimer(&JoinInTimer, 0.0f);
    if ((ResetBits & RESETBIT_CLEAR_LEVEL_PROGRESS) != 0 && MissionSys != NULL) {
        ResetTimer(&MissionSys->timer, 0.0f);
    }
    Hint_SetHintFromId(-1, 0, 0);
    ResetRepeatSfx();
    DEFAULT_MOVE_RANGE = 0.0f;
    alert_obj = NULL;
    alert_timer = 0.0f;
    drop_back_in_timer = 0.0f;
    ResetRippleSet(ripples);
    Grabber_Reset(WORLD);
    Faders_Reset(WORLD);
    chattersfxwait = 3.0f;
    last_chatter_sfx = -1;
    tieonsfxwait = 1.0f;
    tieoffsfxwait = 1.0f;
    FalconDebKey[0] = -1;
    FalconDebKey[1] = -1;
    LevelNameMul = 0.0f;
    LevelNameTime = LevelChange != 0 ? 3.0f : 0.0f;
    memset(&lev_rtldata, 0, sizeof(lev_rtldata));
    if (VehicleArea != 0) {
        if (WORLD->current_level == ASTEROIDCHASED_LDATA || WORLD->current_level == DEATHSTAR2BATTLEA_LDATA) {
            TargetDist_Near2 = 6.25f;
            TargetDist_Mid2 = 100.0f;
            TargetDeg_Near = 0x38e3;
            TargetDeg_Mid = 0x31c7;
            TargetDeg_Far = 0x2aaa;
        } else {
            TargetDist_Near2 = 6.25f;
            TargetDist_Mid2 = 100.0f;
            TargetDeg_Near = 0x2aaa;
            TargetDeg_Mid = 0x1c71;
            TargetDeg_Far = 0x0e38;
        }
    } else {
        TargetDist_Near2 = 0.25f;
        TargetDist_Mid2 = 1.0f;
        TargetDeg_Near = 0x2aaa;
        TargetDeg_Mid = 0x2000;
        TargetDeg_Far = 0x1555;
    }
    party_under_cover = 0;
    nbaddies_can_see_players = 0;
    if (reset_restart != 0) {
        LevTime[0] = 0.0f;
        LevTime[1] = 0.0f;
        BonusCoinTotal = 0;
    }

    if (WORLD->api_object_sys != NULL)
        WORLD->api_object_sys->flags_210 &= ~1;
    if ((ResetBits & RESETBIT_CLEAR_LEVEL_PROGRESS) != 0) {
        GizmoSysClearLevelProgress(WORLD, WORLD->current_level->area_level_index);
    }

    GameAnimSys_ReStoreProgress(WORLD->game_anim_sys, WORLD->current_level->area_level_index);
    GizmoSysReset(WORLD->gizmo_sys, WORLD, WORLD->current_level->area_level_index);

    if ((ResetBits & RESETBIT_REINITIALISE_LEVEL) != 0) {
        DrawBossHitPoints(NULL);
        ResetForceBack();
        ClearAICreatures();
        GameAISysReset(WORLD->ai_sys);
        ReStoreStatusTakeOverObjectSys(1);
        AIScriptInitConditions(WORLD->ai_sys);

        for (i32 player_index = 0; player_index < 8; ++player_index) {
            if (Player[player_index] == NULL)
                continue;
            InitPlayerAI(Player[player_index]);
            char script_name[32];
            if (FreePlay != 0) {
                if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("Freeplay"), 0, 1, 0) != NULL) {
                    strcpy(script_name, "Freeplay");
                } else {
                    if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("party"), 0, 1, 0) != NULL)
                        strcpy(script_name, "party");
                    else
                        strcpy(script_name, "GeneralParty");
                }
            } else if (Mission_Active(NULL) != NULL) {
                if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("Mission"), 0, 1, 0) != NULL) {
                    strcpy(script_name, "Mission");
                } else {
                    if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("party"), 0, 1, 0) != NULL)
                        strcpy(script_name, "party");
                    else
                        strcpy(script_name, "GeneralParty");
                }
            } else {
                if (AIScriptFind(WORLD->ai_sys, Player[player_index]->apiobj.character_data->file, 0, 1, 0) != NULL) {
                    sprintf(script_name, Player[player_index]->apiobj.character_data->file);
                } else {
                    if ((Player[player_index]->apiobj.character_data->model_flags & 8) != 0)
                        strcpy(script_name, "Jedi");
                    else if ((Player[player_index]->apiobj.character_data->model_flags & 0x80) != 0)
                        strcpy(script_name, "Blaster");
                    else
                        strcpy(script_name, "NoWeapon");
                    if (AIScriptFind(WORLD->ai_sys, script_name, 0, 1, 0) == NULL) {
                        if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("party"), 0, 1, 0) != NULL)
                            strcpy(script_name, "party");
                        else
                            strcpy(script_name, "GeneralParty");
                    }
                }
            }
            if (0) {
            party_script:
                if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("party"), 0, 1, 0) != NULL)
                    strcpy(script_name, "party");
                else
                    strcpy(script_name, "GeneralParty");
            }
            AIScriptProcessorInit(WORLD->ai_sys, &Player[player_index]->ai,
                                  reinterpret_cast<AISCRIPTPROCESS *>(&Player[player_index]->ai), NULL, script_name,
                                  NULL, 1, NULL, NULL);
            if (FreePlay == 0 && Mission_Active(NULL) == NULL && Player[player_index]->field_0xcc0 == NULL &&
                AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&Player[player_index]->ai),
                                                 const_cast<char *>("InActive")) != 0) {
                Player[player_index]->apiobj.flags_high &= ~0x10;
            }
        }
    } else {
        ReStoreStatusTakeOverObjectSys(0);
    }

    if (makebaddiesforgetinresetbits != 0)
        MakeBaddiesForgetAboutParty(0);
    if ((ResetBits & 2) != 0) {
        for (i32 i = 0; i < 8; ++i) {
            if (Player[i] != NULL)
                Player[i]->field_0xe38 = 4;
        }
    }
    if ((ResetBits & 4) != 0) {
        for (i32 i = 0; i < 8; ++i) {
            if (Player[i] != NULL) {
                if (Player[i]->apiobj.player_controlled)
                    Player[i]->spawn_protection_timer = 2.5f;
                else
                    Player[i]->spawn_protection_timer = 0.0f;
            }
        }
    }
    ResetRadios();
    SpecialMiniKits_Reset(WORLD);
    SuperCounters_FixUpGizmos(WORLD);
    AITriggerSetSysReset(WORLD->ai_trigger_set_sys);
    AITriggerSysAutoSetUp(WORLD, WORLD->ai_trigger_set_sys);
    if ((ResetBits & 0x10) != 0) {
        if (Player[0] != NULL)
            ResetPlayer(Player[0], 1, NULL, 0);
        if (Player[1] != NULL)
            ResetPlayer(Player[1], 1, NULL, 0);
        if (Player[2] != NULL)
            ResetPlayer(Player[2], 1, NULL, 0);
        if (Player[3] != NULL)
            ResetPlayer(Player[3], 1, NULL, 0);
        if (Player[4] != NULL)
            ResetPlayer(Player[4], 1, NULL, 0);
        if (Player[5] != NULL)
            ResetPlayer(Player[5], 1, NULL, 0);
        if (Player[6] != NULL)
            ResetPlayer(Player[6], 1, NULL, 0);
        if (Player[7] != NULL)
            ResetPlayer(Player[7], 1, NULL, 0);
        VehicleAreaRememberSpeed = GetVehicleAreaRememberSpeed();
    }

    LevHSpecialExists = 0;
    for (i32 i = 0; i < 88; ++i) {
        if (NuSpecialExistsFn(&LevHSpecial[i]) != 0) {
            LevHSpecialExists |= static_cast<u64>(1) << i;
        }
    }
    WORLD->field_0x50d0 = 0;
    if (WORLD->current_level->reset_fn != NULL) {
        WORLD->current_level->reset_fn(WORLD);
    }
    CharPlatforms_Reset(WORLD->char_platform_sys);
    SetSoundFadeDist(WORLD, &Game.options_save);
    TempOptions.field3_0x3 = Game.options_save.field3_0x3;
    TempOptions.field4_0x4 = Game.options_save.field4_0x4;
    TempOptions.field5_0x5 = Game.options_save.field5_0x5;

    if (WORLD->level_progress != NULL && (WORLD->level_progress->flags & 2) == 0) {
        nugspline_s *spline = reinterpret_cast<nugspline_s *>(WORLD->portal_places[1]);
        if (spline != NULL && gone_through_door_to_new_mode == 0 && gone_through_door_to_new_level == 0 &&
            come_from_an_editor == 0) {
            GameCameraMakeMiniCut(spline, 0.0f, 2.0f, 0.0f, 1.0f, 0, 1);
        }
    }
    gone_through_door_to_new_mode = 0;
    come_from_an_editor = 0;
    if (newmode_cutinfo != NULL) {
        NewCutScene(newmode_cutinfo, WORLD->cutscene_sys, NULL, 0);
        newmode_cutinfo = NULL;
    }

    if (BonusArea != 0)
        Cheats_TurnOff(1);
    BonusCoinTarget = TotalLevelCoinTally(WORLD, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if (BonusCoinTarget > 1000000u || (BonusArea != 0 && VehicleArea != 0 && BonusCoinTarget != 1000000u))
        BonusCoinTarget = 1000000u;
    if (netclient != 0)
        BonusCoinTarget = 1000000u;

    // Reset requests are edge-triggered. Leaving the bits set would rebuild the
    // level's AI and gizmo state again on every frame through Batman().
    ResetBits = 0;
    reset_reimport = 0;
    reset_restart = 0;
    CutScene_StartAudio();
    if (NOSOUND == 0)
        WORLD->reset_flags = 1;
    LookAtBoth = 0;
    oneAtOnce_SetNumAttackers(1);
    if (WORLD->level_progress != NULL) {
        if ((WORLD->level_progress->flags_low & 2) != 0)
            WORLD->field_0x5174 = (WORLD->level_progress->flags_low & 4) != 0;
        ResetGizFlow(WORLD->giz_flow, &WORLD->level_progress->giz_flow_progress);
    } else {
        ResetGizFlow(WORLD->giz_flow, NULL);
    }
    EffectOffProgress_Reset(WORLD->level_progress);
    if (HUB_ADATA != NULL && WORLD->area == HUB_ADATA) {
        Tag_SetMode(3);
    } else {
        Tag_SetMode(1);
    }
    if (WORLD->area != NULL && WORLD->area == JEDI_ADATA) {
        WeaponInOut_NoAIJediSfx = 1;
    } else {
        WeaponInOut_NoAIJediSfx = 0;
    }
    bonusmodearcade = 0;
    if (WORLD->area != NULL && (WORLD->area->flags & 0x10) != 0) {
        if (AreaGlobals.values.field_0x14 < AreaGlobals.values.field_0x0c) {
            if (AreaGlobals.values.field_0x0c > 10) {
                AreaGlobals.values.field_0x14 = 10;
            } else {
                AreaGlobals.values.field_0x14 = AreaGlobals.values.field_0x0c;
            }
        }
    }
    arcade_placed_stud_total = (GizmoPickups_TotalScore(WORLD) * 85u) / 100u;
    arcade_placed_stud_total = arcade_placed_stud_total / 1000u * 1000u;
}

void DeflectPart(PART_s *part, GameObject_s *object, float speed, float gravity, i32 deflect, i32 debris) {
    NUMTX matrix = part->transform;
    u32 flags = part->flags;
    i16 type = part->type_id;
    GameObject_s *owner = part->owner;
    nuhspecial_s *special = part->source_special;
    f32 radius = part->field_0e4;
    KillPart(part, 0);

    if (deflect != 0 && owner != NULL) {
        NUVEC velocity;
        f32 facing;
        if (object->apiobj.player_controlled) {
            if (owner->apiobj.field_0x27c == -1) {
                MakeThrowVector(&velocity, reinterpret_cast<NUVEC *>(&matrix.m30), &owner->apiobj.collision_position,
                                &owner->apiobj.velocity, speed, gravity);
                goto velocity_ready;
            }
        }
        velocity.y = 0.0f;
        velocity.x = 0.0f;
        velocity.z = 2.0f;
        NuVecRotateX(&velocity, &velocity, static_cast<i32>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * -5461.0f));
        facing = static_cast<f32>(object->apiobj.movement_facing_angle - 0x1555);
        NuVecRotateY(&velocity, &velocity,
                     static_cast<i32>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 10922.0f + facing));
    velocity_ready:
        ADDPART_s params = Default_ADDPART;
        params.matrix = &matrix;
        params.velocity = &velocity;
        params.special = special;
        params.field_14 = radius;
        params.gravity = gravity;
        params.field_28 = type;
        params.flags = flags;
        params.owner = object;
        params.field_40 = PartCollide_3D;
        params.time_step = FRAMETIME;
        PART_s *deflected = AddPart(&params);
        if (deflected != NULL) {
            u16 (*hit_flags)(GameObject_s *) = reinterpret_cast<u16 (*)(GameObject_s *)>(ObjHitObj_Flags);
            deflected->force_flags = hit_flags(object);
            if (part->update_callback != NULL)
                part->update_callback(deflected);
        }
    }
    if (object != NULL) {
        if (object->field_0x7a5 == 0x18)
            NewBlockAction(object);
        NewRumble(object->pad_gamepad->pad, 0.75f, 0);
        GameCam_Judder(GameCam, -0.2f, 0, NULL);
        if (debris != -1)
            LightSabreDebris(object);
    }
}

static void PartCollide(PART_s *part, i32 three_dimensional) {
    const f32 radius = part->field_0e4;
    NUVEC minimum = part->position;
    NUVEC maximum = part->position;
    minimum.x -= radius;
    maximum.x += radius;
    minimum.y -= radius;
    maximum.y += radius;
    minimum.z -= radius;
    maximum.z += radius;
    GameObject_s *object = Obj;
    i8 force_player_mask = part->force_player_mask;
    auto part_pickup_type = part->pickup_type;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        APIOBJECT_s *api = &object->apiobj;
        if ((api->field_0x1f8 & 0x1001) != 0x1001 || api->field_0x287 != 0)
            continue;
        i8 context = static_cast<i8>(object->character_context);
        if ((CInfo[context].flags & 0x8000) != 0 || (object->field_0xe20 & 0x20) != 0)
            continue;
        if (force_player_mask == 0) {
            if (part->owner == object) {
                if (part->scale_time < 0.5f)
                    continue;
            } else if (api->field_0x27c != -1 && part->scale_time < 0.25f)
                continue;
        } else if ((part->flags & 0x8000) == 0 && part->owner == object)
            continue;
        if (context == 0x39 || context == 0x3b || context == 0x3c)
            continue;
        if ((part->flags & 0x4000) != 0 && part->field_207 == api->field_0x289)
            continue;
        if ((part->flags & 4) != 0 && (api->flags_low & 0x80) == 0)
            continue;
        if (minimum.x > api->collision_max.x || api->collision_min.x > maximum.x || minimum.z > api->collision_max.z ||
            api->collision_min.z > maximum.z)
            continue;
        if (three_dimensional != 0) {
            if ((api->character_data->model_flags & 0x2000) != 0 && (part->flags & 0x40) != 0)
                goto collect_pickup;
            if (minimum.y > api->collision_max.y || api->collision_min.y > maximum.y)
                continue;
        }
        if ((part->flags & 0x40) == 0) {
            GAMECHARACTERDATA_s *character = static_cast<GAMECHARACTERDATA_s *>(api->character_data->field11_0x24);
            if ((character->flags_090 & 0x8000) != 0)
                continue;
            if (!SphereSphereOverlapScaleY(&part->position, part->field_0e4, part->field_0e4, &api->collision_position,
                                           api->collision_radius, api->field_0x1e0))
                continue;
            if (!((part->active & 2) != 0 && (part->flags & 0x1000008) == 0x1000008))
                KillPart(part, 1);
            if ((part->flags & 8) != 0 && !((part->flags & 0x1000000) != 0 && (part->active & 2) != 0) &&
                ((api->flags_low & 0x80) != 0 || (part->flags & 0x800000) == 0)) {
                u32 flags = CInfo[static_cast<i8>(object->character_context)].flags;
                if ((flags & 0x4000000) != 0 || ((flags & 0x8000000) != 0 && (object->jump_flags & 2) != 0)) {
                    i32 effect = -1;
                    if (object->id == id_BOB)
                        effect = (object->field_0xefd & 2) != 0 ? 2 : 3;
                    else if (object->blade_index != -1)
                        effect = BladeTab[object->blade_index].hit_effect;
                    DeflectPart(part, object, ForceThrowSpeed, ForceThrowGravity, 0, effect);
                } else {
                    GameCam_Judder(GameCam, 0.2f, 0, NULL);
                    ReleaseBuildIt(object, 0);
                    ReleasePush(object);
                    if (object->field_0xd24 >= 1.0f)
                        ObjHitShield(part->owner, object, object->field_0xe37, NULL);
                    else if (!CannotKill(object)) {
                        ObjHitObj((part->flags & 0x10000) != 0 ? NULL : part->owner, object, part->field_204,
                                  static_cast<u16>(part->force_flags), 0, 1);
                    } else if (part->owner != NULL) {
                        NewRumble(part->owner->pad_gamepad->pad, 0.5f, 0);
                        NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                    }
                }
            }
            if (api->field_0x287 != 0 || (api->flags_low & 2) != 0)
                continue;
            if ((part->active & 1) == 0) {
                f32 speed = static_cast<GAMECHARACTERDATA_s *>(api->character_data->field11_0x24)->run_speed;
                if (api->scaled_radius > 1.0f)
                    speed /= api->scaled_radius;
                NUVEC direction;
                NuVecNorm(&direction, &part->velocity);
                api->velocity.x += direction.x * speed;
                api->velocity.z += direction.z * speed;
            }
            if ((part->flags & 0x2000000) != 0 && ((part->flags & 0x4000000) == 0 || (part->active & 2) != 0)) {
                u16 angle = NuAtan2D(api->position.x - part->position.x, api->position.z - part->position.z);
                f32 speed = static_cast<GAMECHARACTERDATA_s *>(api->character_data->field11_0x24)->run_speed;
                api->field_0x1fc = NU_SIN_LUT(angle) * speed;
                api->field_0x204 = NU_COS_LUT(angle) * speed;
            }
        } else {
        collect_pickup:
            if ((part->active & 2) == 0) {
                if (part_pickup_type == 0xcb) {
                    if (!(part->scale_time >= 0.5f))
                        continue;
                    if (VehicleArea == 0 && (static_cast<i8>(object->current_hp) <= 0 ||
                                             static_cast<i8>(object->current_hp) >= object->hitpoints))
                        continue;
                } else if (!(part->scale_time >= 0.1f))
                    continue;
            }
            if ((api->field_0x1f4 & 0x40000) != 0) {
                KillPart(part, 2);
                continue;
            }
            if (part_pickup_type == 0xcb) {
                CollectHitPoint(object, &part->position, 1);
                KillPart(part, 2);
            } else if (part_pickup_type == 0xd0) {
                CollectPowerUp(object, &part->position, part->rotation_y, 1);
                KillPart(part, 2);
            } else if (force_player_mask == 3) {
                if ((api->character_data->model_flags & 0x2000) == 0 || object->torpedo == NULL)
                    continue;
                if (object->torpedo->count < getMaxTorpedos(object)) {
                    TORPEDOPACKET_s *packet = object->torpedo;
                    packet->pickup_positions[packet->count] = part->position;
                    packet->pickup_data[packet->count] = part->field_10c[0];
                    packet->pickup_flags[packet->count] = part->field_10c[1];
                    ++packet->count;
                    object->torpedo->field_08 = 0.0f;
                    object->torpedo->field_03 = 0;
                    KillPart(part, 2);
                }
            } else {
                GIZMOPICKUP_s pickup;
                SetCoinType(part_pickup_type, &pickup);
                GizmoPickup_CollectCoin(WORLD, &part->position, pickup.type_index, pickup.model_variant, object, 1);
                KillPart(part, 2);
            }
        }
    }
}

void PartCollide_3D(PART_s *part) {
    PartCollide(part, 1);
}

void PartCollide_2D(PART_s *part) {
    PartCollide(part, 0);
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

void PartKill_ForceThrow(PART_s *part, i32) {
    LEVELDATA *level = WorldInfo_CurrentlyActive()->current_level;
    i32 debris = -1;
    i32 part_debris = -1;
    if (level == CLOUDCITYTRAPB_LDATA) {
        debris = 0x75;
        part_debris = 8;
    } else if (level == MAULA_LDATA) {
        debris = 0x2d;
        part_debris = 9;
    } else if (level == MAULF_LDATA) {
        i32 type = 11;
        if (NuSpecialExistsFn(&LevHSpecial[0]) && NuSpecialCompare(&LevHSpecial[0], &part->special))
            type = 11;
        else if (NuSpecialExistsFn(&LevHSpecial[1]) && NuSpecialCompare(&LevHSpecial[1], &part->special))
            type = 12;
        else if (NuSpecialExistsFn(&LevHSpecial[2]) && NuSpecialCompare(&LevHSpecial[2], &part->special))
            type = 13;
        part_debris = type;
    } else if (level == DOOKUC_LDATA) {
        debris = 0x34;
        part_debris = 10;
    }
    if (debris != -1)
        AddGameDebris(WORLD->debris_sys, debris, &part->position);
    if (part_debris != -1)
        AddPartDebris(WORLD->part_debris_sys, part_debris, &part->position);
    NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
    GameCam_Judder(GameCam, qrand() > 0x7fff ? -0.4f : 0.4f, 2, NULL);
    GameCam_NewShake(GameCam, 0.6f, 0.6f, 1.0f);
    PlaySfx("Explode1", &part->position);
}

void Areas_OpenAll(i32 mode) {
    i32 area_index;

    if (mode != 0 && !Store_IsPackUnlocked(STORE_PACK_CHALLENGE)) {
        return;
    }

    for (area_index = 0; area_index < AREACOUNT; area_index++) {
        if ((ADataList[area_index].flags & AREAFLAG_MINIKIT) != 0) {
            if ((ADataList[area_index].episode_index == AREA_EPISODE_II &&
                 !Store_IsPackUnlocked(STORE_PACK_EPISODE_II)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_III &&
                 !Store_IsPackUnlocked(STORE_PACK_EPISODE_III)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_IV &&
                 !Store_IsPackUnlocked(STORE_PACK_EPISODE_IV)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_V &&
                 !Store_IsPackUnlocked(STORE_PACK_EPISODE_V)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_VI &&
                 !Store_IsPackUnlocked(STORE_PACK_EPISODE_VI))) {
                continue;
            }
        } else if ((ADataList[area_index].flags & (AREAFLAG_VEHICLE_AREA | AREAFLAG_SUPER_BONUS_AREA)) ==
                       AREAFLAG_BONUS_AREA &&
                   ADataList[area_index].episode_index != AREA_EPISODE_NONE &&
                   !Store_IsPackUnlocked(STORE_PACK_ARCADE)) {
            continue;
        }

        if (mode != 0) {
            Game.area_save[area_index].reserved_0x7 = 1;
        } else {
            Game.area_save[area_index].complete = 1;
            Game.area_save[area_index].area_complete = 1;
            if (ADataList[area_index].challenge_trial_time != 0) {
                Game.area_save[area_index].challenge_trial_time =
                    static_cast<f32>(static_cast<u32>(ADataList[area_index].challenge_trial_time)) - 0.5f;
            }
        }
    }

    Game.field_0x2[1] = 1;
    if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
        Hub_LockUnlockDoors(WORLD);
    }
    ReCalculateCompletionPoints();
}

void Episodes_CompleteAllSuperStories() {
    if (Game_EpisodeSave != NULL) {
        for (i32 i = 0; i < EPISODECOUNT; ++i) {
            EPISODESAVE_s *episode = &Game_EpisodeSave[i];
            episode->superstory_complete = 1;
            if (episode->superstory_score_target == 0) {
                episode->superstory_score_target = 1234560;
            }
            if (episode->superstory_time_limit <= 0.0f) {
                episode->superstory_time_limit = 3598.76f;
            }
        }
        ReCalculateCompletionPoints();
    }
}

void GizmoBlowupCreateStuff(GIZMOBLOWUP_s *blowup) {
    char script[32] = "GoonBox";
    i32 model = GetGenericGoon(0);
    f32 choice = qrand() * (1.0f / 65535.0f) * 100.0f;
    if (!(choice < 50.0f)) {
        choice -= 50.0f;
        if (choice < 50.0f) {
            i32 count = static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 3.0f);
            for (i32 i = 0; i <= count; ++i) {
                i32 angle = static_cast<u16>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 65536.0f);
                GameObject_s *object =
                    AddDynamicCreature(model, &blowup->mid_position, angle, script, NULL, NULL, 1, NULL, NULL, 0, -1);
                if (object != NULL) {
                    object->field_0x1038 = 0.0f;
                    object->field_0x1004 = 0.0f;
                    object->apiobj.field_0xa8 = 0.0f * object->apiobj.character_data->model_scale;
                } else {
                    i32 coins =
                        (static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 1000.0f) / 100) * 100;
                    if (coins < 100)
                        coins = 100;
                    NUVEC direction = v010;
                    NuVecRotateX(&direction, &direction, static_cast<u16>(blowup->field_0xf6));
                    NuVecRotateY(&direction, &direction, static_cast<u16>(blowup->field_0xf8));
                    AddPickups(coins, ReleaseHearts(), 0, 0, &blowup->mid_position, &direction, 0.0f, -1, 1.5f,
                               2000000.0f, NULL, 1, 1, true);
                }
            }
            return;
        }
        choice -= 50.0f;
        if (choice < 0.0f)
            return;
    }
    i32 coins = (static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 1000.0f) / 100) * 100;
    if (coins < 100)
        coins = 100;
    NUVEC direction = v010;
    NuVecRotateX(&direction, &direction, static_cast<u16>(blowup->field_0xf6));
    NuVecRotateY(&direction, &direction, static_cast<u16>(blowup->field_0xf8));
    AddPickups(coins, ReleaseHearts(), 0, 0, &blowup->mid_position, &direction, 0.0f, -1, 1.5f, 2000000.0f, NULL, 1, 1,
               true);
}

void FindGameCutScenes() {
    memset(&game_cutscenes, 0, sizeof(game_cutscenes));

    game_cutscenes.podrace_pod_explode = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_podexplode");
    game_cutscenes.podrace_out_of_time = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_outoftime");
    game_cutscenes.bonus_gunship_cavalry_explode =
        CutScene_Find(WORLD->cutscene_sys, "ep2_bonus_gunshipcavalry_explode");
    game_cutscenes.droid_factory_conveyor = CutScene_Find(WORLD->cutscene_sys, "ep2_droidfactory_conveyor");
    game_cutscenes.podrace_avalanche = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_avalanche");
    game_cutscenes.dogfight_die = CutScene_Find(WORLD->cutscene_sys, "ep3_dogfight_die");
    game_cutscenes.podrace_sebulba = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_sebulba");
    game_cutscenes.cutscene = CutScene_Find(WORLD->cutscene_sys, "ep1_podsprint_avalanche");
    game_cutscenes.podsprint_out_of_time = CutScene_Find(WORLD->cutscene_sys, "ep1_podsprint_outoftime");
    game_cutscenes.podsprint_sebulba = CutScene_Find(WORLD->cutscene_sys, "ep1_podsprint_sebulba");
}

i32 CutScene_StartFn_LSW(CUTINFO *cut) {
    LOG_INFO("cutscene start requested cut=%p world=%p", (void *)cut, (void *)WORLD);
    if (PODRACE_ADATA != NULL && PODRACE_ADATA == WORLD->area) {
        if (cut != game_cutscenes.podrace_pod_explode && cut != game_cutscenes.podrace_out_of_time) {
            return 0;
        }
        NUMTX matrix;
        NUVEC *position;
        NUANG rotation;
        if (WORLD->current_level == PODRACEA_LDATA) {
            position = &PodRaceADiePos;
            rotation = 0;
        } else {
            rotation = 0x4000;
            position = WORLD->current_level == PODRACEB_LDATA ? &PodRaceBDiePos : &PodRaceCDiePos;
        }

        NuMtxSetRotationY(&matrix, rotation);
        NuMtxTranslate(&matrix, position);
        instNuGCutSceneSetMtx(static_cast<instNUGCUTSCENE_s *>(cut->instance), &matrix);
        return 1;
    }

    if (BONUS_GUNSHIP_ADATA == NULL || BONUS_GUNSHIP_ADATA != WORLD->area ||
        cut != game_cutscenes.bonus_gunship_cavalry_explode) {
        return 0;
    }

    NUMTX matrix;
    NUVEC *position = WORLD->current_level == BONUS_GUNSHIPA_LDATA ? &GunshipADiePos : &GunshipBDiePos;
    NuMtxSetRotationY(&matrix, 0);
    NuMtxTranslate(&matrix, position);
    instNuGCutSceneSetMtx(static_cast<instNUGCUTSCENE_s *>(cut->instance), &matrix);
    return 1;
}

void CutScene_PreUpdateFn_LSW(CUTINFO *cut) {
    if (cut != game_cutscenes.podrace_avalanche && cut != game_cutscenes.cutscene) {
        return;
    }

    instNUGCUTSCENE_s *instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
    f32 intensity = 111.0f - instance->current_frame;
    if (-110.0f > intensity) {
        intensity = 110.0f;
    }
    intensity = 1.0f - __builtin_fabsf(intensity) / 110.0f;

    f32 distance = NuVecDist(&GameCam->pos, &PodAvalanchePeakPos, NULL);
    if (distance < 750.0f) {
        intensity *= 1.0f - distance / 750.0f;
    }

    NewRumbleAllPlayers(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * intensity, 0.0f, 0, 0);
    GameCam_NewShake(GameCam, intensity + intensity, 0.1f, 1.25f);
}

void CutScene_PostUpdateFn_LSW() {
}

void CutScene_StoppedFn_LSW(CUTINFO *cut) {
    LOG_INFO("cutscene stopped cut=%p world=%p", (void *)cut, (void *)WORLD);
    if (cut != game_cutscenes.cutscene) {
        return;
    }

    for (i32 i = 50; i <= 51; ++i) {
        nuhspecial_s *special = &LevHSpecial[i];
        if (NuSpecialExistsFn(special) != 0) {
            AddPartDebris(WORLD->part_debris_sys, 0x10, NuSpecialGetDrawPos(special));
            NuSpecialSetVisibility(special, 0);
        }
    }
}

i32 CutScene_ReplaceCharacterModelFn_LSW(CUTINFO *cut, NUGCUTCHAR_s *character) {
    if (character == NULL || cut == NULL) {
        return -1;
    }

    if (PODRACE_ADATA != NULL && PODRACE_ADATA == WORLD->area) {
        if (cut != game_cutscenes.podrace_pod_explode && cut != game_cutscenes.podrace_out_of_time &&
            cut != game_cutscenes.podrace_sebulba) {
            return -1;
        }
        if (id_ANAKINSPOD == -1 || NuStrICmp(apicharsys->char_data[id_ANAKINSPOD].file, character->name) != 0) {
            return -1;
        }
        GameObject_s *vehicle = CutDeadVehiclePlayer != NULL ? CutDeadVehiclePlayer : player;
        if (vehicle != NULL && vehicle->id == id_ANAKINSPODGREEN) {
            return vehicle->id;
        }
        return -1;
    }

    if (WORLD->current_level == PODSPRINTA_LDATA) {
        if (cut != game_cutscenes.podsprint_out_of_time && cut != game_cutscenes.podsprint_sebulba) {
            return -1;
        }
        if (id_ANAKINSNEWPOD == -1 || NuStrICmp(apicharsys->char_data[id_ANAKINSNEWPOD].file, character->name) != 0) {
            return -1;
        }
        GameObject_s *vehicle = CutDeadVehiclePlayer != NULL ? CutDeadVehiclePlayer : player;
        if (vehicle != NULL && vehicle->id == id_ANAKINSNEWPODGREEN) {
            return vehicle->id;
        }
        return id_ANAKINSNEWPOD;
    }

    if (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area) {
        if (cut == game_cutscenes.bonus_gunship_cavalry_explode && player != NULL &&
            player->id == id_REPUBLICGUNSHIP_GREEN) {
            return player->id;
        }
        return -1;
    }

    if (GUNSHIP_ADATA != NULL && GUNSHIP_ADATA == WORLD->area) {
        if (cut != game_cutscenes.bonus_gunship_cavalry_explode) {
            return -1;
        }
        if (player != NULL && player->id == id_NEW_REPUBLIC_GUNSHIP_GREEN) {
            return player->id;
        }
        return id_NEW_REPUBLIC_GUNSHIP;
    }

    if (BATTLEOVERCORUSCANT_ADATA != NULL && BATTLEOVERCORUSCANT_ADATA == WORLD->area &&
        cut == game_cutscenes.dogfight_die && player != NULL && player->id == id_JEDISTARFIGHTERYELLOWEP3) {
        return player->id;
    }
    return -1;
}

i32 InitBolt_AddMomentumType_LSW(BOLT_s *bolt, GameObject_s *object, NUVEC *momentum) {
    if (object == NULL)
        return 0;
    if (LSW1 != 0 && momentum != NULL) {
        f32 scale = WORLD->current_level == BONUS_GUNSHIPB_LDATA ? 0.25f : 0.5f;
        NuVecScale(momentum, &object->target_velocity, scale);
        bolt->speed += NuVecMag(momentum);
        return 0;
    }
    if (WORLD->current_level == SPEEDERCHASEA_LDATA && object->id == id_SPEEDERBIKE)
        return 1;
    if (WORLD->current_level == DEATHSTARBATTLED_LDATA)
        return 1;
    return 0;
}

i32 Bolt_HitPlatFn_LSW(BOLT_s *bolt) {
    if (WORLD->current_level == KAMINOE_LDATA)
        KaminoE_CheckPlatHit(bolt);
    return 0;
}

i32 Bolt_HitCustomFn_LSW(BOLT_s *bolt, nuvec_s *points) {
    if (WORLD->current_level == DOGFIGHTA_LDATA)
        return ChrisExtraBoltCollision(bolt, points);
    return 0;
}

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

void GizObstacle_SetDefaultSFXFn_LSW(void *, GIZOBSTACLE_s *obstacle) {
    if (obstacle->mode == 2 && obstacle->trigger_radius < 0.25f) {
        obstacle->start_sfx_id = static_cast<i16>(GetSfxId("SwPPad"));
    }
}

void SpeedBlur_Update() {
    static NUMTX cameraMtxs[2];
    static i32 lastCameraMtx = -1;
    if (lastCameraMtx < 0) {
        cameraMtxs[1] = GameCam->render_mtx;
        cameraMtxs[0] = GameCam->render_mtx;
        lastCameraMtx = 0;
    }
    NuLightSpeedBlurOldCameraPos(&cameraMtxs[lastCameraMtx]);
    lastCameraMtx = 1 - lastCameraMtx;
    cameraMtxs[lastCameraMtx] = GameCam->render_mtx;
}

void SpeedBlur_Apply(WORLDINFO_s *world) {
    if (editor_active != 0 || CUTSTOPGAME != 0 || MiniCutCam != 0) {
        return;
    }
    LEVELDATA *level = world->current_level;
    if (level == PODRACEA_LDATA || level == PODRACEB_LDATA || level == PODRACEC_LDATA) {
        if (PodRace_InStartCountdown(WORLD) != 0) {
            return;
        }
    } else {
        if (level != PODSPRINTA_LDATA) {
            return;
        }
        f32 countdown = PodSprint_InStartCountdown(WORLD);
        if (!(countdown <= 0.0f)) {
            return;
        }
    }
    NuSpeedBlurSetMotionFactors(0.01f, -0.024f, 0.02f);
    NuLightSpeedBlur(Paused, 0.5f);
}

static __used__ void LC_BL_wind_size(nufpar_s *fp) {
    levelconfig_ldata->wind_size = NuFParGetFloat(fp);
}

static __used__ void LC_BL_wind_speed(nufpar_s *fp) {
    levelconfig_ldata->wind_speed = NuFParGetFloat(fp);
}

static __used__ void LC_BL_max_bombgens(nufpar_s *fp) {
    levelconfig_ldata->max_bombgens = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_bombgen_objects(nufpar_s *fp) {
    levelconfig_ldata->max_bombgen_objs = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_plugs(nufpar_s *fp) {
    levelconfig_ldata->max_plugs = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_bridges(nufpar_s *fp) {
    levelconfig_ldata->max_bridges = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_torpmachines(nufpar_s *fp) {
    levelconfig_ldata->max_torp_machines = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_gizrandoms(nufpar_s *fp) {
    levelconfig_ldata->max_giz_randoms = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_teleports(nufpar_s *fp) {
    levelconfig_ldata->max_teleports = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_doors(nufpar_s *fp) {
    levelconfig_ldata->max_doors = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_hatmachines(nufpar_s *fp) {
    levelconfig_ldata->max_hat_machines = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_gizspecials(nufpar_s *fp) {
    levelconfig_ldata->max_giz_specials = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_pushblock_endpos(nufpar_s *fp) {
    levelconfig_ldata->max_push_block_end_pos = (u8)NuFParGetInt(fp);
    if (levelconfig_ldata->max_push_block_end_pos > 64) {
        levelconfig_ldata->max_push_block_end_pos = 64;
    }
}

static __used__ void LC_BL_max_pushblocks(nufpar_s *fp) {
    levelconfig_ldata->max_push_blocks = (u8)NuFParGetInt(fp);
    if (levelconfig_ldata->max_push_blocks > 16) {
        levelconfig_ldata->max_push_blocks = 16;
    }
}

static __used__ void LC_BL_max_force_objects(nufpar_s *fp) {
    levelconfig_ldata->max_force_objs = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_force(nufpar_s *fp) {
    levelconfig_ldata->max_force = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_gizpanels(nufpar_s *fp) {
    levelconfig_ldata->max_giz_panels = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_gameantinodes(nufpar_s *fp) {
    levelconfig_ldata->max_antinodes = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_pickups(nufpar_s *fp) {
    levelconfig_ldata->max_pickups = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_tubes(nufpar_s *fp) {
    levelconfig_ldata->max_tubes = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_securitydoors(nufpar_s *fp) {
    levelconfig_ldata->max_security_doors = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_ledges(nufpar_s *fp) {
    levelconfig_ldata->max_ledges = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_guidelines(nufpar_s *fp) {
    levelconfig_ldata->max_guidelines = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_timers(nufpar_s *fp) {
    levelconfig_ldata->max_giz_timers = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_attractos(nufpar_s *fp) {
    levelconfig_ldata->max_attractos = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_climb_objects(nufpar_s *fp) {
    levelconfig_ldata->max_climb_objs = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_gizmoblowups(nufpar_s *fp) {
    levelconfig_ldata->max_gizmo_blowups = (i16)NuFParGetInt(fp);
    if (levelconfig_ldata->max_gizmo_blowups > 512) {
        levelconfig_ldata->max_gizmo_blowups = 512;
    }
}

static __used__ void LC_BL_max_gizmoblowuptypes(nufpar_s *fp) {
    levelconfig_ldata->max_gizmo_blowup_types = (i16)NuFParGetInt(fp);
    if (levelconfig_ldata->max_gizmo_blowup_types > 255) {
        levelconfig_ldata->max_gizmo_blowup_types = 255;
    }
}

static __used__ void LC_BL_max_minicutParts(nufpar_s *fp) {
    levelconfig_ldata->max_minicut_parts = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_minicuts(nufpar_s *fp) {
    levelconfig_ldata->max_minicuts = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_spinners(nufpar_s *fp) {
    levelconfig_ldata->max_spinners = (u8)NuFParGetInt(fp);
    if (levelconfig_ldata->max_spinners > 8) {
        levelconfig_ldata->max_spinners = 8;
    }
}

static __used__ void LC_BL_max_shards(nufpar_s *fp) {
    levelconfig_ldata->max_shards = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_spinneranim_objs(nufpar_s *fp) {
    levelconfig_ldata->max_spinner_anim_objs = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_buildit_objects(nufpar_s *fp) {
    levelconfig_ldata->max_buildit_objs = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_buildits(nufpar_s *fp) {
    levelconfig_ldata->max_buildits = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_turrets(nufpar_s *fp) {
    levelconfig_ldata->max_turrets = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_obstacle_objects(nufpar_s *fp) {
    levelconfig_ldata->max_obstacle_objs = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_obstacles(nufpar_s *fp) {
    levelconfig_ldata->max_obstacles = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_grapples(nufpar_s *fp) {
    levelconfig_ldata->max_grapples = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_zipups(nufpar_s *fp) {
    levelconfig_ldata->max_zipups = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_technos(nufpar_s *fp) {
    levelconfig_ldata->max_technos = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_levers(nufpar_s *fp) {
    levelconfig_ldata->max_levers = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_signals(nufpar_s *fp) {
    levelconfig_ldata->max_signals = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_tightropes(nufpar_s *fp) {
    levelconfig_ldata->max_tightropes = (u8)NuFParGetInt(fp);
}

