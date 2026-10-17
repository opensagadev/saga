// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/legoapi_types.h"
#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/ai/core/legoai.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/gamepads.h"
#include <math.h>
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include <string.h>
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nutex.h"
#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/motion/action_info.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"
#include <stdio.h>
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/audio/audio.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/misc/utilities.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameframework/saveload.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "batman.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "legoapi/render/core/terrain_internal.h"
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include <ctype.h>
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include <stdarg.h>
#include <limits.h>
#include "nu2api/nu3d/nugscn.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include <new>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/render/fx/particles.h"
#include <cfloat>
#include "legoapi/actions/movement/climbing.h"
struct CLIMBOBJECT_s;

f32 MAGNETOFFSET;

static __used__ void ClimbObject_FindNormal(CLIMBOBJECT_s *object);

f32 LEDGETERRAINLOOKAHEAD = 0.02f;
i32 LedgeTerrain_CheckAnims = 1;

extern i32 LevFlag[4];

extern "C" i16 id_LANDSPEEDER;

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

extern i16 id_MINIATST, id_MINIATTE;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

struct MechTouchUIElement;
struct TouchHolder;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace

extern "C" i16 id_LANDSPEEDER;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

struct FLOWBOX_s;

struct instNUGCUTSCENE_s;

struct JEDIBNETPACKET_s;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

struct nuhspecial_s;
struct nuvec_s;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct nupad_s;

struct EDAISHAREDPATHNODE_s;
struct eduimenu_s;
struct eduiitem_s;

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

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) __asm__(
    "_ZL22routeEditor_AddToRouteP14EDAIPATHNODE_sS0_.part.7");

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other);

struct EDCREATURE_s;

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM1 __attribute__((regparm(1)))
#else
#define CREATURE_EDITOR_REGPARM1
#endif
static __used__
    __attribute__((always_inline)) CREATURE_EDITOR_REGPARM1 void creatureEditor_Updated(EDCREATURE_s *creature);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbResetParams(eduimenu_s *menu,
                                                                                           eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbVisionMenu(eduimenu_s *parent,
                                                                                          eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *item,
                                                                                         unsigned int);

extern "C" {
    struct EDCREATURE_s;
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
static __used__ __attribute__((optimize("no-tree-vectorize"))) void DestroyLocator(EDLOCATOR_s *locator);

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before);

#if defined(__i386__)
static __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2))) unsigned int
#else
static __attribute__((noinline, optimize("no-tree-vectorize"))) unsigned int
#endif
AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator) __asm__(
    "_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.constprop.131");

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

struct STATUSPACKET_s;

#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

struct AIPACKET_s;
struct AISYS_s;

struct GIZMOPICKUP_s;
struct PART_s;

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

i32 (*Fighting_WeaponInActionFn)(GameObject_s *) = NULL;
i32 (*Fighting_WeaponOutActionFn)(GameObject_s *) = NULL;

static inline void FastWeaponInSfx(GameObject_s *object) {
    if (object->weapon_scale_state == WEAPON_SCALE_RETRACTING)
        return;
    const i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
    if (current_animation != -1) {
        CHARACTERANIM_s *animation =
            static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[current_animation]);
        if (animation != NULL && (animation->flags & CHARACTER_ANIMATION_FLAG_GUN_ON) != 0)
            return;
    }
    const u32 model_flags = object->apiobj.character_data->model_flags;
    if ((model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3d, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else if ((model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0) {
        GameAudio_PlaySfx(0x42, &object->apiobj.collision_position, 0, 1);
    }
}

static inline void FastWeaponOutSfx(GameObject_s *object) {
    if (object->weapon_scale_state == WEAPON_SCALE_EXTENDING)
        return;
    const i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
    if (current_animation != -1) {
        CHARACTERANIM_s *animation =
            static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[current_animation]);
        if (animation != NULL && (animation->flags & CHARACTER_ANIMATION_FLAG_GUN_OFF) != 0)
            return;
    }
    const u32 model_flags = object->apiobj.character_data->model_flags;
    if ((model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3e, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else if ((model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0) {
        GameAudio_PlaySfx(0x43, &object->apiobj.collision_position, 0, 1);
    }
}

namespace {

} // namespace

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
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

extern EdInputContext * Input;

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

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

extern "C" {
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
} // extern "C"

static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

struct GameObject_s;
struct WORLDINFO_s;
struct BOLT_s;

typedef isize __attribute__((may_alias)) NuGCutWord;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

extern "C" {
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

extern "C" {
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
} // extern "C"

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
} // extern "C"

namespace {

} // namespace

extern "C" {
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

extern "C" {
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
} // extern "C"

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
} // extern "C"

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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
} // extern "C"

static __attribute__((noinline, noclone)) i32 Bolt_HitPlat(BOLT_s *bolt, u8 *hit_flags)
    __asm__("_ZL12Bolt_HitPlatP6BOLT_sPhP11WORLDINFO_s.isra.38");

// The original caller appears to treat this u16-returning function as i32;
// correcting this declaration lowers its match.

namespace {

} // namespace

namespace {

} // namespace

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.

static __used__ void ClimbObject_FindNormal(CLIMBOBJECT_s *object) {
    AIPATHNODE *second = &object->path->nodes[object->connection->node_indices[1]];
    AIPATHNODE *first = &object->path->nodes[object->connection->node_indices[0]];
    AIPATHNODE *node = second->position.y > first->position.y ? first : second;
    NUVEC displacement, origin;
#define PROBE_CLIMB_NORMAL(angle)                                                                                      \
    do {                                                                                                               \
        displacement.x = node->radius + 0.5f;                                                                          \
        displacement.y = 0.0f;                                                                                         \
        displacement.z = 0.0f;                                                                                         \
        NuVecRotateY(&displacement, &displacement, angle);                                                             \
        origin.x = node->position.x;                                                                                   \
        origin.y = (node->max_height + node->min_height) * 0.5f;                                                       \
        origin.z = node->position.z;                                                                                   \
        if (GameRayCast(&origin, &displacement, 0.0f, TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0 &&     \
            (TerSurface[static_cast<i8>(NewRayCastGetImpactTerrainType())].flags & 0x10000) != 0) {                    \
            goto found_normal;                                                                                         \
        }                                                                                                              \
    } while (0)
    PROBE_CLIMB_NORMAL(0);
    PROBE_CLIMB_NORMAL(0x4000);
    PROBE_CLIMB_NORMAL(0x8000);
    PROBE_CLIMB_NORMAL(0xc000);
    PROBE_CLIMB_NORMAL(0x10000);
    PROBE_CLIMB_NORMAL(0x14000);
    PROBE_CLIMB_NORMAL(0x18000);
    PROBE_CLIMB_NORMAL(0x1c000);
#undef PROBE_CLIMB_NORMAL
    return;
found_normal:
    object->flags |= 1;
    object->impact_x = origin.x + displacement.x;
    object->impact_z = origin.z + displacement.z;
    object->normal = ShadNorm;
}

// Retail keeps the tail of LedgeTerrain_Attach split out by partial inlining;
// the contact checks stay inline in each caller.
#if defined(__i386__)
#define LEDGE_ATTACH_PART_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define LEDGE_ATTACH_PART_ATTR __attribute__((noinline, noclone))
#endif
static LEDGE_ATTACH_PART_ATTR i32 LedgeTerrain_AttachPart(GameObject_s *object, u16 facing, NUVEC *position,
                                                          u16 *wall_angle)
    __asm__("_ZL19LedgeTerrain_AttachP12GameObject_stP7nuvec_sPt.part.0");

static inline i32 LedgeTerrain_Attach(GameObject_s *object, u16 facing, NUVEC *position, u16 *wall_angle) {
    if (!object->field_0x1084)
        return 0;
    u8 surface = object->field_0x6b0;
    if (surface < 32 && (TerSurface[surface].flags & 0x581))
        return 0;
    if (object->apiobj.collision_position.y - 0.05f >= object->contact_position.y ||
        fabsf(object->contact_normal.y) > NuTrigTable[0x3000])
        return 0;
    return LedgeTerrain_AttachPart(object, facing, position, wall_angle);
}

static LEDGE_ATTACH_PART_ATTR i32 LedgeTerrain_AttachPart(GameObject_s *object, u16 facing, NUVEC *position,
                                                          u16 *wall_angle) {
    *wall_angle = NuAtan2D(object->contact_normal.x, object->contact_normal.z);
    i32 difference = RotDiff(facing, *wall_angle);
    if (difference < 0)
        difference = -difference;
    if (difference < 0x4000)
        return 0;

    u16 inward_angle = *wall_angle + 0x8000;
    f32 distance = object->apiobj.field_0x1dc + LEDGETERRAINLOOKAHEAD;
    position->x = NU_SIN_LUT(inward_angle) * distance + object->apiobj.position.x;
    position->y = object->apiobj.upper_position.y;
    position->z = NU_COS_LUT(inward_angle) * distance + object->apiobj.position.z;
    f32 height = GameShadow(NULL, position, 5.0f, -1);
    if (height == 2000000.0f)
        return 0;
    if (NuTrigTable[0x2555] > ShadNorm.y)
        return 0;
    f32 height_difference = position->y - height;
    if (height_difference > 0.025f || height_difference < -0.1f)
        return 0;
    if (object->apiobj.field_0x220 != 2000000.0f && height > object->apiobj.field_0x220)
        return 0;
    position->y = height;
    return 1;
}

void LedgeTerrain_MoveCode(GameObject_s *object) {
    if (LEGOCONTEXT_LEDGETERRAIN == -1)
        return;
    NUVEC position;
    u16 wall_angle;
    if (object->character_context == LEGOCONTEXT_LEDGETERRAIN) {
        if (object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) {
            StartJump(object, 0);
            object->movement_runtime_flags |= 0x10;
            f32 height = 0.1f + object->external_force.y - object->jump_start_height;
            if (height > 0.0f)
                object->apiobj.velocity.y =
                    NuFsqrt(-2.0f * object->apiobj.character_data->game_character->gravity * height);
            return;
        }
        if (object->apiobj.field_0x27d & 2) {
            object->character_context = -1;
            return;
        }
        if (object->field_0x7a3 || object->context_animation != LEGOACT_LEDGE_GRAB ||
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0)) {
            object->context_animation_timer += FRAMETIME;
        }
        if (!object->field_0x7a3 && object->context_animation_timer >= object->field_0x768)
            object->field_0x7a3 = 1;
        f32 travelled = 0.0f;
        if (LedgeTerrain_Attach(object, object->apiobj.movement_facing_angle, &position, &wall_angle)) {
            f32 x = object->external_force.x;
            f32 z = object->external_force.z;
            object->apiobj.movement_facing_angle = wall_angle + 0x8000;
            object->external_force.y = position.y;
            object->launch_origin.x = position.x;
            object->external_force.x = object->apiobj.position.x;
            object->external_force.z = object->apiobj.position.z;
            object->airborne_action_duration = 0.25f;
            object->launch_origin.y = position.y;
            object->launch_origin.z = position.z;
            z = object->apiobj.position.z - z;
            x = object->apiobj.position.x - x;
            travelled = NuFsqrt(x * x + z * z);
        } else {
            object->airborne_action_duration -= FRAMETIME;
            if (object->airborne_action_duration <= 0.0f) {
                object->character_context = -1;
                return;
            }
        }
        if (!object->field_0x7a3)
            return;
        if (!(object->pad_gamepad->input_magnitude > 0.0f)) {
            object->context_animation = LEGOACT_LEDGE_IDLE;
            return;
        }
        u16 input = GamePad_InputAngle(object, object->pad_gamepad);
        u16 sideways = object->apiobj.movement_facing_angle + 0x4000;
        f32 push = PushingTowardsAngle(input, sideways);
        f32 speed;
        if (push > NuTrigTable[0x3555]) {
            if (LEGOACT_LEDGE_RIGHT != -1 &&
                object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_RIGHT] != NULL) {
                object->context_animation = LEGOACT_LEDGE_RIGHT;
                speed = fabsf(AnimSpeed(object->apiobj.character_model, LEGOACT_LEDGE_RIGHT));
            } else {
                if (LedgeTerrain_CheckAnims)
                    return;
                speed = 0.5f;
                goto move;
            }
        } else if (push < -NuTrigTable[0x3555]) {
            if (LEGOACT_LEDGE_LEFT != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_LEFT] != NULL) {
                object->context_animation = LEGOACT_LEDGE_LEFT;
                speed = -fabsf(AnimSpeed(object->apiobj.character_model, LEGOACT_LEDGE_LEFT));
            } else {
                if (LedgeTerrain_CheckAnims)
                    return;
                speed = -0.5f;
                goto move;
            }
        } else {
            object->context_animation = LEGOACT_LEDGE_IDLE;
            return;
        }
        if (speed == 0.0f)
            return;
        if (LedgeTerrain_CheckAnims && !AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0))
            return;
    move:
        if (speed < 0.0f)
            travelled = -travelled;
        f32 distance = speed * FRAMETIME + travelled;
        NUVEC offset = {NU_SIN_LUT(sideways) * distance, 0.0f, NU_COS_LUT(sideways) * distance};
        NuVecAdd(&object->external_force, &object->external_force, &offset);
        NuVecAdd(&object->launch_origin, &object->launch_origin, &offset);
        return;
    }
    if (object->apiobj.field_0x27d != 0 || !(object->apiobj.velocity.y <= 0.0f))
        return;
    if (LedgeTerrain_CheckAnims &&
        (LEGOACT_LEDGE_IDLE == -1 || object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_IDLE] == NULL))
        return;
    if (object->character_context != -1 &&
        !(LEGOCONTEXT_CLIMB != -1 && object->character_context == LEGOCONTEXT_CLIMB)) {
        if (LEGOCONTEXT_JUMP == -1 || object->character_context != LEGOCONTEXT_JUMP ||
            !(object->context_animation_timer >= 0.1f))
            return;
    }
    if (!object->apiobj.player_controlled && !(object->field_0xf01 & 0x80))
        return;
    if (!(object->pad_gamepad->input_magnitude > 0.0f))
        return;
    u16 facing = GamePad_InputAngle(object, object->pad_gamepad);
    if (!LedgeTerrain_Attach(object, facing, &position, &wall_angle))
        return;
    object->field_0x7a3 = 0;
    object->context_animation_timer = 0.0f;
    object->character_context = LEGOCONTEXT_LEDGETERRAIN;
    object->apiobj.velocity = v000;
    if (LEGOACT_LEDGE_GRAB != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_GRAB] != NULL) {
        object->context_animation = LEGOACT_LEDGE_GRAB;
        object->field_0x768 = AnimDuration(object->id, LEGOACT_LEDGE_GRAB, 0.0f, 0.0f, 1);
    } else {
        object->field_0x768 = 0.1f;
        object->context_animation = LEGOACT_LEDGE_IDLE;
        ResetAnimPacket(&object->apiobj.anim_packet, LEGOACT_LEDGE_IDLE);
    }
    object->external_force.x = object->apiobj.position.x;
    object->external_force.y = position.y;
    object->external_force.z = object->apiobj.position.z;
    object->apiobj.movement_facing_angle = wall_angle + 0x8000;
    object->launch_origin = position;
    object->airborne_action_duration = 0.25f;
    return;
}

i32 LedgeTerrain_SetTargetMom(GameObject_s *object) {
    object->target_velocity.x = (NU_SIN_LUT(object->apiobj.movement_facing_angle) * LEDGETERRAINLOOKAHEAD +
                                 object->external_force.x - object->apiobj.upper_position.x) *
                                10.0f;
    object->target_velocity.z = (NU_COS_LUT(object->apiobj.movement_facing_angle) * LEDGETERRAINLOOKAHEAD +
                                 object->external_force.z - object->apiobj.upper_position.z) *
                                10.0f;
    object->target_velocity.y = (object->external_force.y - object->apiobj.upper_position.y) * 10.0f;
    return 1;
}

i32 CanClimbSurface(GameObject_s *object, i32 surface) {
    i32 result = 0;
    if (static_cast<u32>(surface) < 32 && (TerSurface[surface].flags & 0x10000) != 0) {
        result = 1;
        if (surface == 5 && (CanMagnetClimbFn == NULL || CanMagnetClimbFn(object) == 0))
            result = 0;
    }
    return result;
}

void Climb_SetMagnetAction(GameObject_s *object) {
    object->context_animation = LEGOACT_IDLE;
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        if (LEGOACT_MAGNET_WALK_METAL != -1)
            object->context_animation = LEGOACT_MAGNET_WALK_METAL;
        else if (LEGOACT_WALK != -1)
            object->context_animation = LEGOACT_WALK;
    }
}

// Original 0x4f1f90, 134 bytes.
void Climb_UpdateMagnetRotation(GameObject_s *object) {
    object->field_0x1086 = 0;
    object->apiobj.pitch_angle =
        SeekRot(object->apiobj.pitch_angle, static_cast<u16>(-0x4000 - object->magnet_surface_angle), 6.0f);
    object->apiobj.roll_angle = SeekRot(object->apiobj.roll_angle, 0, 10.0f);
}

void Climb_SetMagnetDrawOffsetTarget(GameObject_s *object, nuvec_s *offset) {
    NuVecScale(offset, &object->external_force, MAGNETOFFSET);
    NuVecAdd(offset, offset, &object->launch_origin);
    NuVecSub(offset, offset, &object->apiobj.position);
}

void Climb_MoveCode(GameObject_s *object) {
    if (LEGOCONTEXT_CLIMB == -1)
        return;
    if (object->character_context != LEGOCONTEXT_CLIMB) {
        if (object->character_context == -1)
            goto enter_climb;
        if (LEGOCONTEXT_WALLSHUFFLE != -1 && object->character_context == LEGOCONTEXT_WALLSHUFFLE)
            goto enter_climb;
        if (LEGOCONTEXT_JUMP != -1 && object->character_context == LEGOCONTEXT_JUMP) {
            if (object->context_animation_timer >= 1.0f ||
                (object->context_animation_timer >= 0.1f && object->apiobj.velocity.y <= 0.0f))
                goto enter_climb;
        }
        if (LEGOCONTEXT_GLIDE != -1 && object->character_context == LEGOCONTEXT_GLIDE && object->field_0x788 == NULL)
            goto enter_climb;
        return;
    enter_climb:
        if (!(object->pad_gamepad->input_magnitude > 0.0f) || object->field_0x1084 == 0 ||
            !(fabsf(object->contact_normal.y) < NuTrigTable[0x3000]) ||
            CanClimbSurface(object, static_cast<i8>(object->field_0x6b0)) == 0)
            return;
        const NUVEC normal = object->contact_normal;
        const u8 surface = object->field_0x6b0;
        NUVEC forward;
        NuVecRotateY(&forward, &v001, object->apiobj.movement_facing_angle);
        const f32 x = object->contact_position.x - object->apiobj.position.x;
        const f32 z = object->contact_position.z - object->apiobj.position.z;
        if (!(forward.x * x + forward.z * z > 0.0f))
            return;
        object->apiobj.movement_facing_angle = NuAtan2D(x, z);
        object->apiobj.velocity.y = 0.0f;
        object->character_context = LEGOCONTEXT_CLIMB;
        object->field_0x7a6 = surface;
        object->airborne_action_duration = 0.25f;
        if (surface == 5) {
            Climb_SetMagnetAction(object);
            Climb_SetMagnetDrawOffsetTarget(object, &object->zipup_swing_position);
        } else {
            object->context_animation = LEGOACT_CLIMB_IDLE;
        }
        object->external_force = normal;
        object->launch_origin = object->contact_position;
        FindAnglesXY(&object->external_force, &object->magnet_surface_angle, NULL);
        object->apiobj.velocity.z = 0.0f;
        object->apiobj.velocity.x = 0.0f;
        return;
    }

    if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0 && object->field_0x7a6 != 5) {
        StartJump(object, 0);
        object->movement_runtime_flags |= 0x10;
        const u16 angle = object->apiobj.movement_facing_angle;
        const f32 speed = object->apiobj.character_data->game_character->run_speed;
        object->apiobj.velocity.x = -NU_SIN_LUT(angle) * speed;
        object->apiobj.velocity.z = -NU_COS_LUT(angle) * speed;
        return;
    }
    const u16 input_angle = GamePad_InputAngle(object, object->pad_gamepad);
    if ((object->apiobj.field_0x27d & 2) != 0 &&
        (object->apiobj.field_0x281 == object->field_0x7a6 ||
         (object->pad_gamepad->input_magnitude > 0.0f &&
          PushingTowardsAngle(input_angle, object->apiobj.movement_facing_angle) < 0.0f))) {
        object->character_context = -1;
    } else if (object->field_0x1084 != 0 && object->field_0x6b0 == object->field_0x7a6) {
        object->airborne_action_duration = 0.25f;
        object->apiobj.movement_facing_angle = NuAtan2D(object->contact_position.x - object->apiobj.position.x,
                                                        object->contact_position.z - object->apiobj.position.z);
        object->external_force = object->contact_normal;
        object->launch_origin = object->contact_position;
        FindAnglesXY(&object->external_force, &object->magnet_surface_angle, NULL);
    } else {
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f)
            object->character_context = -1;
    }
    if (object->character_context != LEGOCONTEXT_CLIMB)
        return;
    if (object->field_0x7a6 != 5) {
        if (object->pad_gamepad->input_magnitude > 0.0f) {
            i32 difference =
                RotDiff(object->apiobj.movement_facing_angle, GamePad_InputAngle(object, object->pad_gamepad));
            if (difference < 0)
                difference = -difference;
            object->context_animation = difference > 0x4000 ? LEGOACT_CLIMB_DOWN : LEGOACT_CLIMB_UP;
            if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL)
                return;
        }
        object->context_animation = LEGOACT_CLIMB_IDLE;
    } else {
        Climb_SetMagnetAction(object);
        Climb_SetMagnetDrawOffsetTarget(object, &object->zipup_swing_position);
        object->movement_runtime_flags |= 0x20;
        if (object->pad_gamepad->input_magnitude > 0.0f)
            object->tertiary_lean_angle =
                SeekRot(object->tertiary_lean_angle,
                        static_cast<u16>(RotDiff(object->apiobj.movement_facing_angle, input_angle)), 9.0f);
    }
}

// Original 0x4f26a0, 498 bytes.
i32 Climb_SetTargetMom(GameObject_s *object, u16 input_angle) {
    f32 speed = 0.0f;
    i32 animation = object->context_animation;
    if (animation != -1 &&
        (animation == LEGOACT_CLIMB_UP || animation == LEGOACT_CLIMB_DOWN || animation == LEGOACT_CLIMB_LEFT ||
         animation == LEGOACT_CLIMB_RIGHT || animation == LEGOACT_MAGNET_WALK_METAL)) {
        if (object->apiobj.character_model->model_data_b[animation] != NULL)
            speed = fabsf(AnimSpeed(object->apiobj.character_model, animation));
    }
    if (speed == 0.0f)
        speed = 0.5f;
    u16 angle = object->apiobj.movement_facing_angle;
    object->target_velocity.x = NuTrigTable[static_cast<u16>(angle / 2)] * speed;
    object->target_velocity.y = 0.0f;
    object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        angle += 0x4000;
        f32 lateral = PushingTowardsAngle(input_angle, angle);
        object->target_velocity.x += NuTrigTable[static_cast<u16>(angle / 2)] * lateral * speed;
        object->target_velocity.z += lateral * NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
        f32 vertical = PushingTowardsAngle(input_angle, object->apiobj.movement_facing_angle);
        object->target_velocity.y += vertical * speed;
    }
    return 1;
}

void Hang_MoveCode(GameObject_s *object) {
    i16 animation;
    i8 context;
    i32 *hang_context = &LEGOCONTEXT_HANG;
    if (*hang_context == -1) {
        return;
    }

    if (*hang_context != static_cast<i8>(object->character_context))
        goto try_start_hang;

    if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0)
        goto check_hang_jump;

check_hang_contact:
    if (object->apiobj.field_0x27d == 0) {
        if (object->apiobj.field_0x280 != object->field_0x7a6)
            goto cancel_hang_velocity;

        if (object->pad_gamepad->input_magnitude > 0.0f) {
            animation = LEGOACT_HANG_MOVE;
            if (animation != -1)
                goto animation_selected;
        }
        animation = LEGOACT_HANG_IDLE;
    animation_selected:
        object->context_animation = animation;
        if (object->field_0x1084 != 0 && object->apiobj.field_0x280 == object->field_0x6b0) {
            object->airborne_action_duration = 0.25f;
            goto copy_hang_force;
        }

        const f32 duration = object->airborne_action_duration;
        const f32 frame_time = FRAMETIME;
        object->airborne_action_duration = duration - frame_time;
        if (duration - frame_time <= 0.0f)
            goto cancel_hang;
        return;
    }

cancel_hang:
    object->character_context = -1;
    return;

cancel_hang_velocity:
    object->apiobj.velocity.y = 0.0f;
    object->character_context = -1;
    return;

check_hang_jump:
    if (object->field_0x7a6 != 5)
        goto cancel_hang_velocity;
    goto check_hang_contact;

try_start_hang:
    if (object->apiobj.field_0x27d != 0)
        return;
    if (object->field_0x1084 == 0)
        return;
    if (CanClimbSurface(object, static_cast<i8>(object->field_0x6b0)) == 0)
        return;
    if (-NuTrigTable[0x3000] > object->contact_normal.y)
        goto hang_normal_valid;
    return;

hang_normal_valid:
    context = object->character_context;
    if (context != -1) {
        const i32 climb_context = LEGOCONTEXT_CLIMB;
        if (climb_context != -1 && climb_context == context)
            goto start_hang;
        const i32 jump_context = LEGOCONTEXT_JUMP;
        if (jump_context == -1 || jump_context != context)
            return;
        if (object->context_animation_timer >= 0.1f)
            goto start_hang;
        return;
    }

start_hang:
    object->airborne_action_duration = 0.25f;
    object->character_context = static_cast<i8>(*hang_context);
    object->context_animation = LEGOACT_HANG_IDLE;
    object->field_0x7a6 = object->field_0x6b0;

copy_hang_force:
    object->external_force = object->contact_normal;
}

i32 Hang_SetTargetMom(GameObject_s *object) {
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        f32 speed;
        if (LEGOACT_HANG_MOVE != -1 && object->apiobj.character_model->model_data_b[LEGOACT_HANG_MOVE] != NULL) {
            speed = AnimSpeed(object->apiobj.character_model, LEGOACT_HANG_MOVE);
            if (speed < 0.25f)
                speed = 0.25f;
        } else {
            speed = 0.5f;
        }
        object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * speed;
        object->target_velocity.z = NU_COS_LUT(object->apiobj.movement_facing_angle) * speed;
    } else {
        object->target_velocity.x = object->target_velocity.z = 0.0f;
    }
    object->target_velocity.y = 0.5f;
    return 1;
}

void *CreateClimbObjectSys(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    if (count == 0)
        return NULL;
    CLIMBOBJECTSYS_s *system = static_cast<CLIMBOBJECTSYS_s *>(GameBufferAlloc(buf, buf_end, sizeof(CLIMBOBJECTSYS_s)));
    if (system == NULL)
        return NULL;
    system->capacity = static_cast<u16>(count);
    system->objects =
        static_cast<CLIMBOBJECT_s *>(GameBufferAlloc(buf, buf_end, system->capacity * sizeof(CLIMBOBJECT_s)));
    return system;
}

void InitClimbObjectSys(WORLDINFO_s *world) {
    if (world == NULL || world->ai_sys == NULL || world->ai_sys->path_sys == NULL || world->climb_object_sys == NULL)
        return;
    world->climb_object_sys->count = 0;
    memset(world->climb_object_sys->objects, 0, world->climb_object_sys->capacity * sizeof(CLIMBOBJECT_s));
    for (i32 index = 0; index < world->ai_sys->path_sys->path_count; ++index) {
        AIPATH *path = world->ai_sys->path_sys->paths[index];
        if (path == NULL)
            continue;
        AIPATHCNX *connection = path->connections;
        for (i32 connection_index = 0; connection_index < path->connection_count; ++connection_index, ++connection) {
            CLIMBOBJECTSYS_s *system = world->climb_object_sys;
            if (system->count < system->capacity &&
                (connection->traversal_flags[0] & (LEGO_AIPATHCNX_MAGNETCLIMB | LEGO_AIPATHCNX_CLIMB)) != 0) {
                CLIMBOBJECT_s *object = &system->objects[system->count++];
                object->connection = connection;
                object->path = path;
                ClimbObject_FindNormal(object);
            }
        }
    }
}

CLIMBOBJECT_s *FindClimbObject(CLIMBOBJECTSYS_s *system, AIPATHCNX_s *connection) {
    if (system == NULL)
        return NULL;
    CLIMBOBJECT_s *object = system->objects;
    for (i32 index = 0; index < system->count; ++index, ++object) {
        if (object->connection != connection)
            continue;
        if ((object->flags & 2) != 0)
            return NULL;
        if ((object->flags & 1) != 0)
            return object;
        ClimbObject_FindNormal(object);
        if ((object->flags & 1) != 0)
            return object;
        object->flags |= 2;
    }
    return NULL;
}

// Original 0x4f2e50, 258 bytes. The original returns an integer.
i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap) {
    i32 result = 0;
    if (object->force_target != NULL) {
        NUVEC forward, difference;
        NuVecRotateY(&forward, &v001, object->apiobj.field_0x276);
        f32 distance = NuVecDistSqr(&opponent->apiobj.position, &object->apiobj.position, &difference);
        f32 dot = forward.x * difference.x + forward.z * difference.z;
        if (static_cast<i8>(object->context_flags) < 0 ? dot <= 0.0f : dot >= 0.0f) {
            f32 radius = object->apiobj.field_0x1dc + opponent->apiobj.field_0x1dc + gap;
            result = distance < radius * radius;
        }
    }
    return result;
}

void SetWeaponOut(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    object->weapon_scale = 1.0f;
    object->weapon_out_timer = 0.0f;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
}

void FastWeaponOut(GameObject_s *object, i32 force_sound) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    if (__builtin_expect(force_sound != 0, 1) && object->weapon_scale == 0.0f)
        FastWeaponOutSfx(object);
    object->weapon_scale_rate = 5.0f;
    object->weapon_out_timer = 0.0f;
    object->weapon_scale_state = WEAPON_SCALE_EXTENDING;
}

void SlowWeaponOut(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONOUT != -1 && Fighting_WeaponOutActionFn != NULL) {
        const i32 action = Fighting_WeaponOutActionFn(object);
        if (action != -1 && object->apiobj.character_model->model_data_b[action] != NULL) {
            const f32 end = NuAnimEndFrame(object->apiobj.character_model->model_data_b[action]);
            f32 start = AnimListFrame(object->apiobj.character_model, action, 0);
            if (start < 1.0f)
                start = 1.0f;
            if (start >= 1.0f && start < end) {
                const f32 finish = AnimListFrame(object->apiobj.character_model, action, 1);
                if (finish > end || finish > start) {
                    object->context_animation = action;
                    object->character_context = LEGOCONTEXT_WEAPONOUT;
                    object->context_animation_timer = AnimDuration(object->id, action, 0.0f, 0.0f, 1);
                    object->weapon_scale = 0.0f;
                    object->weapon_out_timer = 0.0f;
                    return;
                }
            }
        }
    }
    FastWeaponOut(object, 1);
}

void KeepWeaponOut(GameObject_s *object) {
    object->weapon_scale = 1.0f;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
    object->field_0xef8 |= GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
}

void SetWeaponIn(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    object->weapon_scale = 0.0f;
    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
}

void FastWeaponIn(GameObject_s *object, i32 force_sound) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    if (__builtin_expect(force_sound != 0, 1) && object->weapon_scale == 1.0f)
        FastWeaponInSfx(object);
    object->weapon_scale_rate = 5.0f;
    object->weapon_scale_state = WEAPON_SCALE_RETRACTING;
}

void SlowWeaponIn(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONIN != -1 && Fighting_WeaponInActionFn != NULL) {
        const i32 action = Fighting_WeaponInActionFn(object);
        if (action != -1 && object->apiobj.character_model->model_data_b[action] != NULL) {
            const f32 end = NuAnimEndFrame(object->apiobj.character_model->model_data_b[action]);
            f32 start = AnimListFrame(object->apiobj.character_model, action, 0);
            if (!(start >= 1.0f))
                start = 1.0f;
            if (start >= 1.0f && start < end) {
                const f32 finish = AnimListFrame(object->apiobj.character_model, action, 1);
                if (finish > end || finish > start) {
                    object->context_animation = action;
                    object->character_context = LEGOCONTEXT_WEAPONIN;
                    object->context_animation_timer = AnimDuration(object->id, action, 0.0f, 0.0f, 1);
                    object->weapon_scale = 1.0f;
                    return;
                }
            }
        }
    }
    FastWeaponIn(object, 1);
}

void WeaponOutCode(GameObject_s *object) {
    object->weapon_out_timer += FRAMETIME;
    if (TouchHacks::ShouldKeepWeaponOut(*object)) {
        object->weapon_out_timer = 0.0f;
        if ((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) == 0 && object->character_context != 7) {
            if (object->weapon_scale_state == WEAPON_SCALE_IDLE && object->apiobj.field_0x27d != 0 &&
                object->pad_gamepad->input_magnitude == 0.0f) {
                i32 action = 17;
                if (object->apiobj.character_data->game_character->field275_0x116 == 0 &&
                    (object->apiobj.character_data->model_flags & 0x80) != 0)
                    action = 127;
                if (object->apiobj.character_model->model_data_b[action] != NULL &&
                    (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0)) {
                    SlowWeaponOut(object);
                    goto after_weapon_touch;
                }
            }
            FastWeaponOut(object, 1);
        }
    } else if (TouchHacks::ShouldPutWeaponAway(*object) &&
               (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 && object->character_context != 6) {
        SlowWeaponIn(object);
    }
after_weapon_touch:
    if (LEGOCONTEXT_WEAPONOUT == -1 || object->character_context != LEGOCONTEXT_WEAPONOUT)
        return;
    if (object->pad_gamepad->input_magnitude > 0.0f &&
        (object->apiobj.character_data->game_character->field_0x94 & 0x1000) == 0) {
        if (object->weapon_scale == 0.0f && object->weapon_scale_state != WEAPON_SCALE_EXTENDING) {
            FastWeaponOut(object, 0);
        }
        object->character_context = -1;
        return;
    }
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 animation = packet.blending ? packet.blend_animation_b : packet.animation_index;
    if (animation != object->context_animation) {
        object->weapon_scale = 0.0f;
        return;
    }
    const f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if (object->weapon_scale != 0.0f)
            return;
    } else {
        if (object->weapon_scale != 0.0f || object->weapon_scale_state == WEAPON_SCALE_EXTENDING ||
            packet.requested_animation != object->context_animation)
            return;
        const f32 time = packet.blending ? packet.blend_target_time : packet.current_time;
        if (!(time >= start))
            return;
    }
    const f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
    object->weapon_scale_state = WEAPON_SCALE_EXTENDING;
    object->weapon_scale_rate = 1.0f / AnimDuration(object->id, object->context_animation, start, end, 0);
    if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3e, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else {
        GameAudio_PlaySfx(0x43, &object->apiobj.collision_position, 0, 1);
    }
    ResetCharacterIdle(object, 1, GetDefaultIdle(object));
}

void WeaponInCode(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONIN == -1 || object->character_context != LEGOCONTEXT_WEAPONIN)
        return;
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (object->pad_gamepad->input_magnitude > 0.0f && (data->field_0x94 & 0x1000) == 0) {
        if (object->weapon_scale == 1.0f && object->weapon_scale_state != WEAPON_SCALE_RETRACTING) {
            FastWeaponIn(object, 0);
        }
        object->character_context = -1;
        return;
    }
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 animation = packet.blending ? packet.blend_animation_b : packet.animation_index;
    if (animation != object->context_animation) {
        object->weapon_scale = 1.0f;
        return;
    }
    const f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if (object->weapon_scale != 1.0f)
            return;
    } else {
        if (object->weapon_scale != 1.0f || object->weapon_scale_state == WEAPON_SCALE_RETRACTING ||
            packet.requested_animation != object->context_animation)
            return;
        const f32 time = packet.blending ? packet.blend_target_time : packet.current_time;
        if (!(time >= start))
            return;
    }
    const f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
    object->weapon_scale_state = WEAPON_SCALE_RETRACTING;
    object->weapon_scale_rate = 1.0f / AnimDuration(object->id, object->context_animation, start, end, 0);
    if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3d, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else {
        GameAudio_PlaySfx(0x42, &object->apiobj.collision_position, 0, 1);
    }
    ResetCharacterIdle(object, 1, GetDefaultIdle(object));
}

void KeepWeaponIn(GameObject_s *object) {
    object->weapon_scale = 0.0f;
    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
    object->field_0xef8 &= ~GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
}

void AutoWeaponOnOff(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONOUT || context == LEGOCONTEXT_WEAPONIN))
        return;
    const i32 index = CurrentAnim(&object->apiobj.anim_packet);
    if (index == -1)
        return;
    CHARACTERANIM_s *animation = static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[index]);
    const u32 flags = animation->flags;
    const ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((flags & CHARACTER_ANIMATION_FLAG_GUN_OFF) != 0) {
        if (object->weapon_scale > 0.0f) {
            if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 && packet.blending != 0) {
                FastWeaponIn(object, 0);
            } else if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 ||
                       object->weapon_scale_state != WEAPON_SCALE_RETRACTING) {
                SetWeaponIn(object);
            }
        }
    } else if ((flags & CHARACTER_ANIMATION_FLAG_GUN_ON) != 0 && object->weapon_scale < 1.0f) {
        if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 && packet.blending != 0) {
            FastWeaponOut(object, 0);
        } else if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 ||
                   object->weapon_scale_state != WEAPON_SCALE_EXTENDING) {
            SetWeaponOut(object);
        }
    }
}

void WeaponScalingCode(GameObject_s *object) {
    switch (object->weapon_scale_state) {
        case WEAPON_SCALE_EXTENDING:
            object->weapon_scale = SeekLinearF(object->weapon_scale, 1.0f, object->weapon_scale_rate * FRAMETIME);
            if (object->weapon_scale >= 1.0f) {
                object->weapon_scale_state = WEAPON_SCALE_IDLE;
                object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
            }
            break;
        case WEAPON_SCALE_RETRACTING:
            object->weapon_scale = SeekLinearF(object->weapon_scale, 0.0f, object->weapon_scale_rate * FRAMETIME);
            if (object->weapon_scale <= 0.0f) {
                object->weapon_scale_state = WEAPON_SCALE_IDLE;
                object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
            }
            break;
        default:
            break;
    }
}

i32 NewBlockAction(GameObject_s *object) {
    i32 actions[3];
    i32 count = 0;
    for (i32 action = 0; action < apicharsys->model_id_capacity && count < 3; ++action) {
        if ((ActionInfo[action].flags & 8) != 0 && object->apiobj.character_model->model_data_b[action] != NULL) {
            actions[count++] = action;
        }
    }
    if (count == 0)
        return 0;
    i32 action = actions[0];
    if (count != 1) {
        do {
            action = actions[qrand() / (0xffff / count + 1)];
        } while (action == object->previous_block_animation);
    }
    object->previous_block_animation = action;
    object->context_animation = action;
    return 1;
}

void BlockSfx(GameObject_s *object) {
    GameObject_s *target = object->force_target;
    if (target != NULL) {
        const u32 context_flags = CInfo[target->character_context].flags;
        if ((context_flags & 0x4000000) != 0 || ((context_flags & 0x8000000) != 0 && (target->jump_flags & 2) != 0)) {
            const i32 sfx_bits = GameAudio_GetPlrSfxBits(target);
            GameAudio_PlaySfx(0x3f, &object->apiobj.collision_position, sfx_bits, 0);
        }
    }
}

i32 CanStepBack(GameObject_s *object) {
    i8 context = object->character_context;
    if ((CInfo[context].flags & 0x10000) != 0 ||
        (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == context && object->action_movement_state == 3)) {
        return 1;
    }
    GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if ((character->flags_090 & 0x8000) != 0 && LEGOCONTEXT_COMBO != 0 && LEGOCONTEXT_COMBO == context) {
        return 1;
    }
    return 0;
}

i32 StepBackFromTarget(GameObject_s *object) {
    NUVEC *target_position;
    float inner_distance;
    float outer_distance;
    if (object->blowup_target != NULL) {
        target_position = &object->blowup_target->mid_position;
        if ((LEGOCONTEXT_COMBO != -1 && LEGOCONTEXT_COMBO == object->character_context) ||
            (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == object->character_context &&
             object->action_movement_state == 3)) {
            float target_radius = object->blowup_target->target_scale;
            inner_distance = object->apiobj.collision_radius * 1.75f + target_radius;
            outer_distance = object->apiobj.collision_radius * 2.25f + target_radius;
        } else {
            inner_distance = object->apiobj.collision_radius * 0.75f + object->blowup_target->target_scale;
            outer_distance = object->apiobj.collision_radius * 1.25f + object->blowup_target->target_scale;
        }
    } else {
        GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if ((character->flags_090 & 0x8000) == 0 || object->force_target == NULL) {
            return 0;
        }
        if ((LEGOCONTEXT_COMBO == -1 || LEGOCONTEXT_COMBO != object->character_context) &&
            (LEGOCONTEXT_JUMP == -1 || LEGOCONTEXT_JUMP != object->character_context ||
             object->action_movement_state != 3)) {
            return 0;
        }
        float target_radius = object->force_target->apiobj.field_0x1dc;
        target_position = &object->force_target->apiobj.collision_position;
        inner_distance = object->apiobj.field_0x1dc * 1.75f + target_radius;
        outer_distance = object->apiobj.field_0x1dc * 2.25f + target_radius;
    }
    float speed = AnimSpeed(object->apiobj.character_model, object->context_animation);
    if (speed != 0.0f) {
        float distance_squared = NuVecXZDistSqr(&object->apiobj.collision_position, target_position, NULL);
        if (!(distance_squared >= outer_distance * outer_distance)) {
            float blend = 1.0f;
            if (!(inner_distance * inner_distance >= distance_squared)) {
                blend = 1.0f - (NuFsqrt(distance_squared) - inner_distance) / (outer_distance - inner_distance);
            }
            speed = (-speed - speed) * blend + speed;
            object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * speed;
            object->target_velocity.z = speed * NU_COS_LUT(object->apiobj.movement_facing_angle);
            return 1;
        }
    }
    return 0;
}

