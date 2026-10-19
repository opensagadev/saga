// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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
#include "legoapi/audio/audio.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/world/world.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/fx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/screens/arcade.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include <stdio.h>
#include <string.h>
#include "legoapi/items/objects/gameobjects_types.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/items/base/apiobject.h"
#include <new>
#include "nu2api/numath/nuang.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/gizmos/object/newblowup.h"
#include <stdlib.h>
#include "legoapi/cutscenes/minicamcut.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "legoapi/characters/motion/camera_types.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/core/players_types.h"
#include "nu2api/nufile/nufilepak.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/characters/core/characters_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/core/config/fileselect.h"
#include "nu2api/nu3d/nugscn.h"
#include "batman.h"
#include "gamelib/crc/crc.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/items/collect/spacelevel.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "legoapi/actions/movement/jumping.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edcam.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include <stddef.h>
#include "legoapi/render/core/terrain_stubs_types.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui_colour.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "gameapi/edtools/edtools_types.h"
#include "legoapi/misc/utilities.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/nuonline.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/numem.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nuquat.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/actions/character/speederchase_types.h"
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "nu2api/nucore/nulist.h"
#include "editor/edtoolsall_types.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/characters/motion/move_types.h"
#include "legoapi/characters/motion/gameanim_types.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "nu2api/nucore/numechptr.hpp"
#include "legoapi/world/levels/episodeV_types.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/items/collect/bolts.h"
#include "legoapi/gizmos/fx/guidelines.h"
#include "legoapi/gizmos/transport/ledges.h"
#include "legoapi/gizmos/traps/shards.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/episodeII_types.h"
#include "legoapi/misc.h"
#include "legoapi/world/levels/podrace.h"
#include "legoapi/world/levels/episodeI_types.h"
#include "legoapi/ai/core/gameai_types.h"
#include "legoapi/ai/game/creature_types.h"
#include <stdint.h>
#include "gameapi/ai/aisys/aiscript_types.h"
#include "legoapi/actions/character/simpletons_types.h"
#include "legoapi/ai/core/ai_pathgeometry.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "nu2api/nucore/nurdp.h"
#include "legoapi/ai/core/aisysall_types.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/path_connections.h"
#include "editor/edpath_types.h"
#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/actions/movement/jumping_types.h"
#include <ctype.h>
#include "legoapi/world/levels/episode_types.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/gizmo/object/gizmoblowups_types.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"
#include <limits.h>
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/misc/supportall_types.h"
#include "gameapi/ai/aisys/gameaiscript.h"
#include "legoapi/characters/motion/camera.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/menus/core/hint.h"
#include "legoapi/world/levels/episodeV.h"

i32 only_process_this_hint_id = -1;
f32 ICONXPOS_VIRTUALS = -0.61500001f;
f32 ICONYPOS_VIRTUALS = 0.47f;
f32 ICONYPOS_TOUCHYFEELY = -0.699999988f;
f32 AlphaBlendTime = 1.0f;

// The hint icon transitions and the touch icon x position are dynamically
// initialized in this unit's static constructor in the reference.
HintVectorTransition hintIconPos;
HintScalarTransition hintIconScale;
HintScalarTransition hintYPop;
f32 ICONXPOS_TOUCHYFEELY = -ICONX;
i32 Tilt_UpdateHint(HINT_s *) {
    return 0;
}

// Each control mode retains its own requested and interpolated hint row.

HINTUIBUTTON_s *hintUIButton = NULL;
f32 (*Hint_AlphaTargetFn)();
i32 (*Hub_PanelBusyFn)();

#include "legoapi/menus/core/gamehint_types.h"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern i32 LevFlag[4];

struct HINT_s;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

struct EDCREATURE_s;

extern f32 HUB_EPISODETITLEY;

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

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
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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

#include "nu2api/nucore/nucore_plain_types.h"

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
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

extern "C" {
#include "legoapi/ai/core/ai_sys_stubs_types.h"
} // extern "C"

struct eduimenu_s;
struct eduiitem_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {
#include "legoapi/gizmo/gizmos/gizmos_gizactions_types.h"

} // namespace

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

void *GetHintFromUIButton() {
    return hintUIButton != NULL ? hintUIButton->field_0x78 : NULL;
}

void Hint_ExpandButtons(char *input, char *output) {
    char token[256];
    char expanded[64];

    *output = '\0';
    while (*input != '\0') {
        if (*input != '[') {
            *output = *input;
            output[1] = '\0';
            ++input;
            ++output;
            continue;
        }

        token[0] = '[';
        i32 token_length = 1;
        while (input[token_length] != ']' && input[token_length] != '\0') {
            token[token_length] = input[token_length];
            ++token_length;
        }

        char literal = '[';
        if (input[token_length] != '\0') {
            token[token_length] = ']';
            token[token_length + 1] = '\0';
            token_length = NuStrLen(token);
            if (token_length > 0) {
                if (Text_ExpandButtonString(token, expanded) != 0) {
                    char *character = expanded;
                    while (*character != '\0') {
                        *output = *character;
                        output[1] = '\0';
                        ++character;
                        ++output;
                    }
                    NuStrCat(output, expanded);
                    input += token_length;
                    continue;
                }
            }
            literal = *input;
        }

        *output = literal;
        output[1] = '\0';
        ++input;
        ++output;
    }
    *output = '\0';
}

i32 Hint_CurrentId() {
    HINT_s *hint = hintsys.active_hint;
    if (hint == NULL) {
        return -1;
    }
    return hint->control_mode_ids[MechInputTouchSystem::s_baseControlMode];
}

HINT_s *Hint_FindHint(i32 hint_id) {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return NULL;
    }

    while (hint->control_mode_ids[0] != -1) {
        if (hint->control_mode_ids[0] == hint_id || hint->control_mode_ids[1] == hint_id) {
            return hint;
        }
        ++hint;
    }
    return NULL;
}

i32 Hint_isAvailable(i32 hint_id) {
    HINT_s *active_hint = hintsys.active_hint;
    if (active_hint != NULL && active_hint->control_mode_ids[MechInputTouchSystem::s_baseControlMode] == hint_id) {
        return 0;
    }
    if (hint_id < 0 || TTab[hint_id] == NULL) {
        return 0;
    }

    HINT_s *hint = Hint_FindHint(hint_id);
    if (hint == NULL || hint->completion_flags[MechInputTouchSystem::s_baseControlMode] != 0) {
        return 0;
    }
    return !(hint->field_0x20 > 0.0f);
}

i32 Hint_isComplete(i32 hint_id) {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return 0;
    }

    hint = Hint_FindHint(hint_id);
    if (hint == NULL) {
        return 0;
    }
    return hint->completion_flags[MechInputTouchSystem::s_baseControlMode];
}

i32 Hint_isComplete(HINT_s *hint) {
    return hint->completion_flags[MechInputTouchSystem::s_baseControlMode];
}

void initHintSys() {
    hintsys.current_hint = 0;
    hintsys.state = 2;
    hintsys.active_hint = NULL;
    MechHintUIButton *button = new MechHintUIButton();
    button->UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[3]);
    button->alpha = 0.0f;
    button->alpha_to = 0.0f;
    button->slide = -1.5f;
    button->visible = 0;
    button->slide_to = -1.5f;
    button->pulse_active = 0;
    button->slide_elapsed = button->slide_duration;
    hintUIButton = reinterpret_cast<HINTUIBUTTON_s *>(button);
    button->alpha_elapsed = button->alpha_duration;
    MechSystems::Get()->TouchUI().AddUIElement(*button);
}

void RegisterWithHintSys(void (*update_fn)(HINT_s *, i32), HINT_s *hints, u32 *save_bits, i32 save_bit_count) {
    hintsys.update_fn = update_fn;
    hintsys.hints = hints;
    hintsys.save_bits = save_bits;
    hintsys.save_bit_count = save_bit_count;
    initHintSys();
}

void Hint_CancelCurrent() {
    HINT_s *hint = hintsys.active_hint;
    if (hint != NULL) {
        if (hint->display_duration != 0.0f && hint->display_duration - 0.5f > hintsys.display_elapsed)
            hintsys.display_elapsed = hint->display_duration - 0.5f;
        hint->field_0x20 = 0.0f;
    }
    HINTUIBUTTON_s *button = hintUIButton;
    if (button->field_0x78 != NULL) {
        const f32 blend_time = AlphaBlendTime;
        button->field_0x7c = NULL;
        const f32 position = *button->field_0x84;
        f32 *alpha = button->field_0x40;
        button->field_0x8c = -1.5f;
        button->field_0x90 = 0.0f;
        button->field_0x98 = 0.0f;
        button->field_0x88 = position;
        button->field_0x78 = NULL;
        button->field_0x94 = blend_time;
        const f32 alpha_value = *alpha;
        button->field_0xa0 = 0;
        button->field_0x48 = 0.0f;
        button->field_0x4c = 0.0f;
        button->field_0x54 = 0.0f;
        button->field_0x44 = alpha_value;
        button->field_0x50 = blend_time;
    }
}

void Hint_Reset() {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return;
    }

    while (hint->control_mode_ids[0] != -1) {
        hint->field_0x20 = 0;
        hint++;
    }

    hintsys.state = 2;
    hintsys.current_hint = 0;

    HINTUIBUTTON_s *button = hintUIButton;
    if (button->field_0x78 != NULL) {
        f32 alpha_blend_time = AlphaBlendTime;
        button->field_0x7c = 0;
        f32 field_0x84 = *button->field_0x84;
        f32 *field_0x40 = button->field_0x40;
        button->field_0x8c = -1.5f;
        button->field_0x90 = 0.0f;
        button->field_0x98 = 0.0f;
        button->field_0x88 = field_0x84;
        button->field_0x78 = NULL;
        button->field_0x94 = alpha_blend_time;
        f32 field_0x40_value = *field_0x40;
        button->field_0xa0 = 0;
        button->field_0x48 = 0.0f;
        button->field_0x4c = 0.0f;
        button->field_0x54 = 0.0f;
        button->field_0x44 = field_0x40_value;
        button->field_0x50 = alpha_blend_time;
    }

    hintsys.field_0x1c = 0;
}

void Hint_ResetHint(i32 hint_id, i32 reset_completed) {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    while (hint->control_mode_ids[control_mode] != hint_id) {
        hint++;
        if (hint->control_mode_ids[0] == -1) {
            return;
        }
    }

    if (reset_completed == 1) {
        hint->completion_flags[control_mode] = 0;
    }
    hintsys.current_hint = 0;
}

void Hint_ClearHintsAndDoneFlags() {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    do {
        hint->completion_flags[control_mode] = 0;
        hint->flags &= ~0x42;
        hint++;
    } while (hint->control_mode_ids[0] != -1);
}

void Hint_SaveBits(i32 hint_id, i32 complete) {
    if (complete != 0) {
        SET_HINT_COMPLETE(hint_id);
        return;
    }
    CLEAR_HINT_COMPLETE(hint_id);
}

void Hint_SaveGameState(HINT_s *hint) {
    HINT_s *candidate = hintsys.hints;
    if (candidate == NULL) {
        return;
    }
    if (hintsys.save_bits == NULL) {
        return;
    }
    if (candidate->control_mode_ids[0] == -1) {
        return;
    }

    const u16 hint_id = static_cast<u16>(hint->control_mode_ids[0]);
    i32 index = 0;
    u16 candidate_id = static_cast<u16>(candidate->control_mode_ids[0]);
    if (candidate_id == hint_id) {
        goto found;
    }
    do {
        ++candidate;
        ++index;
        candidate_id = static_cast<u16>(candidate->control_mode_ids[0]);
        if (candidate_id == 0xffff) {
            return;
        }
    } while (candidate_id != hint_id);

found:
    Hint_SaveBits(index, 1);
}

void Hint_SetComplete(HINT_s *hint) {
    if (hint == NULL) {
        return;
    }

    const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    if (hint->completion_flags[control_mode] == 0) {
        hint->completion_flags[control_mode] = 1;
        Hint_SaveGameState(hint);
    }
}

void Hint_SetComplete(i32 hint_id) {
    if (hint_id == -1) {
        return;
    }

    HINT_s *hint = Hint_FindHint(hint_id);
    if (hint == NULL || (hint->flags & 0x40) == 0) {
        return;
    }

    const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    if (hint->completion_flags[control_mode] == 0) {
        hint->completion_flags[control_mode] = 1;
        Hint_SaveGameState(hint);
    }
}

void Hint_SetHint(HINT_s *hint, i32 force, i32 allow_completed) {
    if (hintsys.active_hint != NULL && hintsys.active_hint == hint)
        return;
    HINTUIBUTTON_s *button = hintUIButton;
    if (button->field_0x78 != NULL && button->field_0x78 == hint)
        return;
    if (hint == NULL) {
        hintsys.active_hint = NULL;
        if (button->field_0x78 != NULL) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = NULL;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x78 = NULL;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x50 = blend_time;
            button->field_0x44 = alpha_value;
        }
        return;
    }
    if (force == 0) {
        if (hint->completion_flags[MechInputTouchSystem::s_baseControlMode] != 0 && allow_completed == 0)
            return;
        if (hint->field_0x20 > 0.0f)
            return;
    }
    if ((hint->flags & 8) == 0) {
        if (button->field_0x78 != hint) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = hint;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x44 = alpha_value;
            button->field_0x50 = blend_time;
        }
    } else {
        if (button->field_0x78 != NULL) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = NULL;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x78 = NULL;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x44 = alpha_value;
            button->field_0x50 = blend_time;
        }
        hintsys.active_hint = hint;
        if (hint->on_display != NULL)
            hint->on_display(hint);
    }
    hintsys.current_hint = 0;
    hintsys.field_0x1c = 0;
    if (force == 0 && (hint->flags & 1) != 0) {
        hint->completion_flags[MechInputTouchSystem::s_baseControlMode] = 1;
        Hint_SaveGameState(hint);
    }
    i32 state;
    do {
        state = qrand() / 16384;
    } while (state == hintsys.state);
    hintsys.state = static_cast<u8>(state);
    hint->field_0x20 = hint->repeat_delay;
    hintsys.field_0x1c = 0;
}

void Hint_SetHintFromId(i32 hint_id, i32 force, i32 allow_completed) {
    if (hintsys.active_hint != NULL &&
        hintsys.active_hint->control_mode_ids[MechInputTouchSystem::s_baseControlMode] == hint_id)
        return;
    if (hint_id < 0 || TTab[hint_id] == NULL) {
        HINTUIBUTTON_s *button = hintUIButton;
        if (button->field_0x78 != NULL) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = NULL;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x78 = NULL;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x44 = alpha_value;
            button->field_0x50 = blend_time;
        }
        return;
    }
    HINT_s *hint = Hint_FindHint(hint_id);
    if (hint != NULL)
        Hint_SetHint(hint, force, allow_completed);
}

void Hint_SaveAllGameState() {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return;
    }
    u32 *save_bits = hintsys.save_bits;
    if (save_bits == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    i32 index = 0;
    const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    do {
        if (hint->completion_flags[control_mode] != 0) {
            Hint_SaveBits(index, 1);
        } else {
            Hint_SaveBits(index, 0);
        }
        ++hint;
        ++index;
    } while (hint->control_mode_ids[0] != -1);
}

void Hint_LoadAllGameState() {
    HINTSYS_s *system = &hintsys;
    HINT_s *hint = system->hints;
    if (hint == NULL || system->save_bits == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    for (i32 index = 0; (index >> 5) < system->save_bit_count; ++index) {
        if (HINT_COMPLETE(index) == 0) {
            hint->completion_flags[MechInputTouchSystem::s_baseControlMode] = 0;
        } else {
            hint->completion_flags[MechInputTouchSystem::s_baseControlMode] = 1;
        }
        ++hint;
        if (hint->control_mode_ids[0] == -1) {
            return;
        }
    }
}

void Hint_Process(float elapsed) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    const i32 only_hint = only_process_this_hint_id;
    only_process_this_hint_id = -1;
    hintIconPos.Update(elapsed);
    hintIconScale.Update(elapsed);
    hintYPop.Update(elapsed);
    if (hintsys.hints == NULL)
        return;

    i32 available =
        HINTS_ON != 0 && CUTSTOPGAME == 0 && MiniCutCam != 1 && MiniCutCam != 2 && MiniCutCam != 3 && SuperStory == 0 &&
        BonusArea == 0 && (world->current_level->flags & 0x200) == 0 &&
        (world->area == NULL || (world->area->flags & 0x124) == 0) && GetMenuID() == -1 && FadeSys.fade == 0.0f &&
        (world->area == NULL || world->area != HUB_ADATA || Hub_PanelBusyFn == NULL || Hub_PanelBusyFn() == 0);
    if (ChallengeMode != 0)
        available = false;
    else if (Mission_Active(NULL) != NULL)
        available = false;
    if (hintUIButton->field_0x78 != NULL || hintUIButton->field_0x7c != NULL)
        available = false;

    for (HINT_s *hint = hintsys.hints; hint->control_mode_ids[0] != -1; ++hint) {
        if (hintsys.active_hint != hint && hint->field_0x20 > 0.0f)
            hint->field_0x20 -= elapsed;
        if (only_hint != -1 && hint->control_mode_ids[0] != only_hint)
            continue;
        const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
        const i32 id = hint->control_mode_ids[control_mode];
        if (id == -1 || TTab[id] == NULL || !available || hintUIButton->field_0x78 != NULL ||
            hintUIButton->field_0x7c != NULL || hintsys.active_hint != NULL ||
            hint->completion_flags[control_mode] != 0 || hint->availability_fn == NULL)
            continue;
        if (hint->availability_fn(hint) == 0 || hint->field_0x20 > 0.0f ||
            hint->completion_flags[MechInputTouchSystem::s_baseControlMode] == 1)
            continue;
        Hint_SetHint(hint, 0, 0);
    }

    const f32 target = Hint_AlphaTargetFn != NULL ? Hint_AlphaTargetFn() : 1.0f;
    hintsys.alpha = SeekLinearF(hintsys.alpha, target, 3.0f * FRAMETIME);
    HINT_s *hint = hintsys.active_hint;
    if (hint != NULL) {
        hintsys.display_elapsed += elapsed;
        if ((hint->display_duration > 0.0f && !(hintsys.display_elapsed < hint->display_duration)) ||
            MechSystems::Get()->PlayerButton().selector != NULL)
            hintsys.active_hint = NULL;
    }
}

void Hint_Draw(i32 viewport) {
    if (hintsys.active_hint != NULL && hintsys.update_fn != NULL)
        hintsys.update_fn(hintsys.active_hint, viewport);
}

// GameThingManager D1 dtor @0x4e8a80 clears the global before destruction.
GameThingManager::~GameThingManager() {
    theGameThings = NULL;
}

// GameThingManager::GameThingManager @0x4e8b00: stores the object in
// theGameThings (the vptr switch to the derived vtable is compiler-generated).
GameThingManager::GameThingManager(i32 max_things) : ThingManager(max_things) {
    theGameThings = this;
}

