// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "batman.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"

#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/numath/nufloat.h"

#include <string.h>
#include "legoapi/gizmos/transport/gizportal.h"
#include <stdio.h>
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufile.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include <math.h>
#include <stddef.h>
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "legoapi/render/fx/edsplines.h"
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/nucore/nuvideo.h"
#include <stdlib.h>
#include "gameframework/saveload.h"
#include <cstdio>
#include "nu2api/nu3d/nuspline.h"
#include <new>
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include "editor/edpath.h"
#include "nu2api/nucore/nulist.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "gameapi/ai/aisys/aipath.h"
#include "nu2api/nucore/numechptr.hpp"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include <ctype.h>
#include <limits.h>
#include "nu2api/nuplatform/nudevicespecs.hpp"
#include "nu2api/nusound/nusound.h"
#include "legoapi/render/fx/particles.h"
#include <cfloat>

#include "legoapi/misc/utilities.h"

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

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

extern "C" i16 id_LANDSPEEDER;

struct nuhspecial_s;

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

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct GameObject_s;
struct WORLDINFO_s;

extern i32 LevFlag[4];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

struct EDCREATURE_s;

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

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

}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

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

struct nufpar_s;

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// Original 400-pointer table at 0x61e860: 147 named effects, then empty slots.
extern "C" {
i32 Grass_Available;
}

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

static __used__ void LC_AL_backb(nufpar_s *fp);
static __used__ void LC_AL_backb_bottom(nufpar_s *fp);
static __used__ void LC_AL_backb_top(nufpar_s *fp);
static __used__ void LC_AL_backg(nufpar_s *fp);
static __used__ void LC_AL_backg_bottom(nufpar_s *fp);
static __used__ void LC_AL_backg_top(nufpar_s *fp);
static __used__ void LC_AL_backr(nufpar_s *fp);
static __used__ void LC_AL_backr_bottom(nufpar_s *fp);
static __used__ void LC_AL_backr_top(nufpar_s *fp);
static __used__ void LC_AL_blobshadow_alpha(nufpar_s *fp);
static __used__ void LC_AL_blobshadow_fadefar(nufpar_s *fp);
static __used__ void LC_AL_blobshadow_fadenear(nufpar_s *fp);
static __used__ void LC_AL_cam_tilt(nufpar_s *fp);
static __used__ void LC_AL_camang_seek(nufpar_s *fp);
static __used__ void LC_AL_campos_seek(nufpar_s *fp);
static __used__ void LC_AL_conveyor(nufpar_s *fp);
static __used__ void LC_AL_hover_height(nufpar_s *fp);
static __used__ void LC_AL_plat_scan_dist(nufpar_s *fp);
static __used__ void LC_AL_raycaststep(nufpar_s *fp);
static __used__ void LC_AL_reflect_range(nufpar_s *fp);
static __used__ void LC_AL_reflect_y(nufpar_s *fp);
static __used__ void LC_AL_sfx_ambient(nufpar_s *fp);
static __used__ void LC_AL_waterripple_endcol_a(nufpar_s *fp);
static __used__ void LC_AL_waterripple_endcol_b(nufpar_s *fp);
static __used__ void LC_AL_waterripple_endcol_g(nufpar_s *fp);
static __used__ void LC_AL_waterripple_endcol_r(nufpar_s *fp);
static __used__ void LC_AL_waterripple_life(nufpar_s *fp);
static __used__ void LC_AL_waterripple_startcol_a(nufpar_s *fp);
static __used__ void LC_AL_waterripple_startcol_b(nufpar_s *fp);
static __used__ void LC_AL_waterripple_startcol_g(nufpar_s *fp);
static __used__ void LC_AL_waterripple_startcol_r(nufpar_s *fp);
static __used__ void LC_BL_max_ter_groups(nufpar_s *fp);
static __used__ void LC_BL_max_ter_platforms(nufpar_s *fp);
static __used__ void LC_BL_mipmapmode(nufpar_s *fp);

static u8 load_conditionParam;
static __used__ void LC_AL_metal(nufpar_s *fp);
static __used__ void LC_AL_music(nufpar_s *fp);
static __used__ void LC_AL_music_other(nufpar_s *fp);
static __used__ void LC_AL_farclip(nufpar_s *fp);
static __used__ void LC_AL_camera_rain(nufpar_s *fp);
static __used__ void LC_AL_in_space(nufpar_s *fp);
static __used__ void LC_AL_flat_terrain(nufpar_s *fp);
static __used__ void LC_AL_hidden_icons(nufpar_s *fp);
static __used__ void LC_AL_double_score(nufpar_s *fp);
static __used__ void LC_AL_narrow_socks(nufpar_s *fp);
static __used__ void LC_AL_terrain_rain(nufpar_s *fp);
static __used__ void LC_AL_lowendfarclip(nufpar_s *fp);
static __used__ void LC_AL_cam_lateral_dist(nufpar_s *fp);
static __used__ void LC_AL_cam_pullback_dist(nufpar_s *fp);
static __used__ void LC_AL_lowendcharclip(nufpar_s *fp);
static __used__ void LC_AL_lowendfogstart(nufpar_s *fp);
static __used__ void LC_AL_nearclip(nufpar_s *fp);
static __used__ void LC_AL_farclip_hack(nufpar_s *fp);
static __used__ void LC_AL_lowendcamerazoom(nufpar_s *fp);
static __used__ void LC_AL_lowendparticlethin(nufpar_s *fp);
static __used__ void LC_AL_forget_takeovers(nufpar_s *fp);
static __used__ void LC_AL_pickups_to_panel(nufpar_s *fp);
static __used__ void LC_AL_override_nopickupgravity(nufpar_s *fp);
static __used__ void LC_BL_fix_strobing_anims(nufpar_s *fp);

static NUFPCOMJMP LevelConfig_BeforeLoad_GenericKeywords[] = {
    {const_cast<char *>("mipmapmode"), LC_BL_mipmapmode},
    {const_cast<char *>("fix_strobing_anims"), LC_BL_fix_strobing_anims},
    {const_cast<char *>("max_ter_groups"), LC_BL_max_ter_groups},
    {const_cast<char *>("max_ter_platforms"), LC_BL_max_ter_platforms},
    {NULL, NULL},
};

static NUFPCOMJMP LevelConfig_AfterLoad_GenericKeywords[] = {
    {const_cast<char *>("backr"), LC_AL_backr},
    {const_cast<char *>("backg"), LC_AL_backg},
    {const_cast<char *>("backb"), LC_AL_backb},
    {const_cast<char *>("backr_top"), LC_AL_backr_top},
    {const_cast<char *>("backg_top"), LC_AL_backg_top},
    {const_cast<char *>("backb_top"), LC_AL_backb_top},
    {const_cast<char *>("backr_bottom"), LC_AL_backr_bottom},
    {const_cast<char *>("backg_bottom"), LC_AL_backg_bottom},
    {const_cast<char *>("backb_bottom"), LC_AL_backb_bottom},
    {const_cast<char *>("farclip_hack"), LC_AL_farclip_hack},
    {const_cast<char *>("lowendfarclip"), LC_AL_lowendfarclip},
    {const_cast<char *>("lowendfogstart"), LC_AL_lowendfogstart},
    {const_cast<char *>("lowendparticlethin"), LC_AL_lowendparticlethin},
    {const_cast<char *>("farclip"), LC_AL_farclip},
    {const_cast<char *>("nearclip"), LC_AL_nearclip},
    {const_cast<char *>("pspcharclip"), LC_AL_lowendcharclip},
    {const_cast<char *>("lowendcharclip"), LC_AL_lowendcharclip},
    {const_cast<char *>("lowendcamerazoom"), LC_AL_lowendcamerazoom},
    {const_cast<char *>("farclip_pc"), LC_AL_farclip},
    {const_cast<char *>("nearclip_pc"), LC_AL_nearclip},
    {const_cast<char *>("hover_height"), LC_AL_hover_height},
    {const_cast<char *>("blobshadow_alpha"), LC_AL_blobshadow_alpha},
    {const_cast<char *>("blobshadow_fadenear"), LC_AL_blobshadow_fadenear},
    {const_cast<char *>("blobshadow_fadefar"), LC_AL_blobshadow_fadefar},
    {const_cast<char *>("reflect_range"), LC_AL_reflect_range},
    {const_cast<char *>("reflect_y"), LC_AL_reflect_y},
    {const_cast<char *>("metal"), LC_AL_metal},
    {const_cast<char *>("in_space"), LC_AL_in_space},
    {const_cast<char *>("override_nopickupgravity"), LC_AL_override_nopickupgravity},
    {const_cast<char *>("hidden_icons"), LC_AL_hidden_icons},
    {const_cast<char *>("pickups_to_panel"), LC_AL_pickups_to_panel},
    {const_cast<char *>("forget_takeovers"), LC_AL_forget_takeovers},
    {const_cast<char *>("narrow_socks"), LC_AL_narrow_socks},
    {const_cast<char *>("camera_rain"), LC_AL_camera_rain},
    {const_cast<char *>("terrain_rain"), LC_AL_terrain_rain},
    {const_cast<char *>("double_score"), LC_AL_double_score},
    {const_cast<char *>("flat_terrain"), LC_AL_flat_terrain},
    {const_cast<char *>("cam_tilt"), LC_AL_cam_tilt},
    {const_cast<char *>("raycaststep"), LC_AL_raycaststep},
    {const_cast<char *>("plat_scan_dist"), LC_AL_plat_scan_dist},
    {const_cast<char *>("waterripple_startcol_r"), LC_AL_waterripple_startcol_r},
    {const_cast<char *>("waterripple_startcol_g"), LC_AL_waterripple_startcol_g},
    {const_cast<char *>("waterripple_startcol_b"), LC_AL_waterripple_startcol_b},
    {const_cast<char *>("waterripple_startcol_a"), LC_AL_waterripple_startcol_a},
    {const_cast<char *>("waterripple_endcol_r"), LC_AL_waterripple_endcol_r},
    {const_cast<char *>("waterripple_endcol_g"), LC_AL_waterripple_endcol_g},
    {const_cast<char *>("waterripple_endcol_b"), LC_AL_waterripple_endcol_b},
    {const_cast<char *>("waterripple_endcol_a"), LC_AL_waterripple_endcol_a},
    {const_cast<char *>("waterripple_life"), LC_AL_waterripple_life},
    {const_cast<char *>("cam_pullback_dist"), LC_AL_cam_pullback_dist},
    {const_cast<char *>("cam_lateral_dist"), LC_AL_cam_lateral_dist},
    {const_cast<char *>("campos_seek"), LC_AL_campos_seek},
    {const_cast<char *>("camang_seek"), LC_AL_camang_seek},
    {const_cast<char *>("music"), LC_AL_music},
    {const_cast<char *>("music_other"), LC_AL_music_other},
    {const_cast<char *>("sfx_ambient"), LC_AL_sfx_ambient},
    {const_cast<char *>("conveyor"), LC_AL_conveyor},
    {NULL, NULL},
};

extern "C" char ConfigBuffer[0x10000];

// These are extern (U) in the original level.cpp.o — defined here as stubs
// until the original defining file is decompiled.

LEVELDATA *NEWGAME_LDATA = NULL;
LEVELDATA *LOADGAME_LDATA = NULL;

static i32 MAXLDATA = 0x64;

extern char ConfigBuffer[65536];

extern WORLDINFO WorldInfo[2];
extern "C++" volatile i32 abort_load;

extern i32 LevFlag[4]; // shared per-level 16-byte state scratch

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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

LEVELSPLINE *LevSplList;
static i32 levspl_i_start = -1;
static i32 levspl_i_startcam = -1;
static i32 LEVELOBJECTMAX;
static char *ExtraLevelObject_NameTable;
static i32 ExtraLevelObject_NameTableSize;
static i32 ExtraLevelObject_NameTableIndex;

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
    struct EDCREATURE_s;
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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
    extern i32 reset_reimport;
}

extern i32 reset_reimport;

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

// The original caller appears to treat this u16-returning function as i32;
// correcting this declaration lowers its match.

GameObject_s *CutDeadVehiclePlayer;

__attribute__((optimize("O2", "tree-vectorize"))) void LevelStreaming_Update(WORLDINFO_s *world) {
    if (LOADEROFF == 0 && BGLOAD != 0 && world->area != NULL && (world->area->flags & 8) == 0 && NewLData == NULL &&
        Mission_Active(NULL) == NULL) {

        other_level = -1;
        if (no_more_loads == 0) {
            CUTINFO *cut;
            if (other_level_override != -1) {
                other_level = other_level_override;
            } else if (CUTSTOPGAME != 0 && (cut = static_cast<CUTINFO *>(CutStopInfo)) != NULL &&
                       cut->skip_level != -1 &&
                       LDataList[cut->skip_level].area_index == LDataList[world->level_idx].area_index) {
                other_level = cut->skip_level;
            } else if ((world->current_level->flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == 0) {
                NUVEC position;
                if (Players_AveragePos(&position, NULL) != 0) {
                    other_level = -1;
                    f32 best_distance = 1000000000.0f;
                    for (i32 i = 0; i < world->door_count; ++i) {
                        i32 level = InStory() != 0 || world->doors[i].freeplay_level == -1
                                        ? world->doors[i].level
                                        : world->doors[i].freeplay_level;
                        if (level != world->level_idx) {
                            f32 distance = NuVecDistSqr(&world->doors[i].pos, &position, NULL);
                            if (distance < best_distance) {
                                best_distance = distance;
                                other_level = world->doors[i].level;
                            }
                        }
                    }

                    LEVELLOAD_s *load = LevelLoad;
                    i32 i;
                    for (i = 0; i < LevelLoadCount; ++i, ++load) {
                        if (load->level == world->level_idx && (((load->flags & 1) != 0 && InStory() != 0) ||
                                                                ((load->flags & 2) != 0 && InStory() == 0))) {
                            break;
                        }
                    }
                    if (i < LevelLoadCount) {
                        const i16 first_level = load->first_level;
                        const i16 second_level = load->second_level;
                        if (first_level != second_level) {
                            if (LEGOSPL_SPLIT != -1 && world->camera_splines[LEGOSPL_SPLIT] != NULL) {
                                NUVEC *points = world->camera_splines[LEGOSPL_SPLIT]->pts;
                                if (InsideLineXZ(position.x, position.z, points[0].x, points[0].z, points[1].x,
                                                 points[1].z) == 0) {
                                    other_level = second_level;
                                } else {
                                    other_level = first_level;
                                }
                            }
                        } else {
                            other_level = first_level;
                        }
                    }

                    if (other_level != -1) {
                        if (InStory() == 0 && (LDataList[other_level].flags & LEVEL_OUTRO) != 0) {
                            LEVELDATA_s *status_level = Area_FindStatusLevel(world->area, NULL);
                            if (status_level != NULL) {
                                other_level = status_level->idx;
                            }
                        }
                    }
                    if (other_level != -1 && InStory() == 0 &&
                        (LDataList[other_level].flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) != 0) {
                        other_level = -1;
                    }
                    if (other_level != -1 && CutScenePlayer_Active() != NULL &&
                        (LDataList[other_level].flags & LEVEL_STATUS) != 0) {
                        other_level = -1;
                    }
                }
            }
        }

        if (waiting_for_level == -1 && bgGetProcActive() == NULL && other_level != -1 &&
            WorldInfo_OtherLevel(world) != other_level && LDataList[other_level].area_index == world->level_sub_id) {
            NUTIME time;
            NuTimeGet(&time);
            if (NuTimeSeconds(&time) > g_BgLoadDelayHackTimer) {
                waiting_for_level = other_level;
                next_level = other_level;
                abort_load = 0;
                WaitingForLevelTime = 0.0f;
                bgPostRequest(WorldInfo_StreamLevel, NULL, NULL, 0);
            }
        } else if (waiting_for_level != -1 && bgGetProcActive() != NULL && other_level != -1 &&
                   other_level != waiting_for_level && LDataList[other_level].area_index == world->level_sub_id) {
            abort_load = 1;
        }
    }
}

void PortalDoors_Configure(WORLDINFO_s *world, char *config) {
    world->portal_doors = NULL;
    world->portal_door_count = 0;
    if (world->current_gscn == NULL) {
        return;
    }

    NUFPAR *parser = NuFParCreateMem(const_cast<char *>("portaldoors"), config, 0xffff);
    if (parser == NULL) {
        return;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    PORTALDOOR *portal_door = static_cast<PORTALDOOR *>(world->giz_buffer.void_ptr);
    world->portal_doors = portal_door;

    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, const_cast<char *>("portaldoor")) != 0) {
            continue;
        }

        *portal_door = {};
        if (NuFParGetWord(parser) == 0 ||
            NuSpecialFind(world->current_gscn, &portal_door->special, parser->word_buf, 1) == 0) {
            continue;
        }

        portal_door->portal_id = static_cast<u8>(NuFParGetInt(parser));
        while (NuFParGetWord(parser) != 0) {
            if (NuStrICmp(parser->word_buf, const_cast<char *>("trigger_at_end")) == 0) {
                portal_door->flags |= PORTALDOOR_TRIGGER_AT_END;
            }
        }

        ++portal_door;
        ++world->portal_door_count;
    }

    NuFParDestroy(parser);
    if (world->portal_door_count == 0) {
        world->portal_doors = NULL;
        return;
    }
    world->giz_buffer.addr = ALIGN(reinterpret_cast<usize>(portal_door), 16);
}

void PortalDoors_Update(WORLDINFO_s *world) {
    PORTALDOOR *door = world->portal_doors;
    if (door == NULL || world->portal_door_count <= 0) {
        return;
    }

    i32 index = 0;
    do {
        nuinstanim_s *animation = NuSpecialGetInstAnim(&door->special);
        if (animation != NULL) {
            const f32 end_frame = NuAnimEndFrameOld(door->special.scene->instance_animation_data[animation->anim_ix]);
            const u16 flags = door->flags;
            bool closed = false;
            if ((flags & PORTALDOOR_TRIGGER_AT_END) != 0) {
                closed = !(animation->ltime >= end_frame);
            } else {
                closed = !(animation->ltime > 1.0f);
            }

            if (closed) {
                if ((flags & (PORTALDOOR_OPENED | PORTALDOOR_CLOSED)) != PORTALDOOR_CLOSED) {
                    NuPortalSetActive(world->current_gscn, door->portal_id, 0);
                    door->flags = static_cast<u16>((door->flags & ~PORTALDOOR_OPENED) | PORTALDOOR_CLOSED);
                }
            } else if ((flags & PORTALDOOR_OPENED) == 0) {
                NuPortalSetActive(world->current_gscn, door->portal_id, 1);
                door->flags = static_cast<u16>((door->flags & ~PORTALDOOR_CLOSED) | PORTALDOOR_OPENED);
            }
        }
        ++index;
        ++door;
    } while (index < world->portal_door_count);
}

void PortalDoors_Reset(WORLDINFO *world_info) {
    PORTALDOOR *door = world_info->portal_doors;
    if (door == NULL) {
        return;
    }
    i32 count = world_info->portal_door_count;
    if (count <= 0) {
        return;
    }
    i32 index = 0;
    do {
        ++index;
        door->flags &= static_cast<u16>(~(PORTALDOOR_OPENED | PORTALDOOR_CLOSED));
        ++door;
    } while (index != count);
}

void EquivalentObjects_Configure(WORLDINFO_s *world, char *config) {
    world->equivalent_groups = NULL;
    world->equivalent_group_count = 0;
    if (world->current_gscn == NULL)
        return;

    NUFPAR *parser = NuFParCreateMem(const_cast<char *>("equivalentobjects"), config, 0xffff);
    if (parser == NULL)
        return;

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    u8 *cursor = reinterpret_cast<u8 *>(world->giz_buffer.addr);
    world->equivalent_groups = reinterpret_cast<EQUIVALENTOBJECTGROUP_s *>(cursor);
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, const_cast<char *>("equivalentobjects")) == 0) {

            EQUIVALENTOBJECTGROUP_s *group = reinterpret_cast<EQUIVALENTOBJECTGROUP_s *>(cursor);
            group->object_count = 0;
            group->byte_size = 4;
            while (NuFParGetWord(parser) != 0) {
                nuhspecial_s *handle = &group->objects[group->object_count];
                if (NuSpecialFind(world->current_gscn, handle, parser->word_buf, 1) == 0)
                    break;
                ++group->object_count;
                group->byte_size += sizeof(nuhspecial_s);
            }
            if (group->object_count > 0) {
                ++world->equivalent_group_count;
                cursor += group->byte_size;
            }
        }
    }
    NuFParDestroy(parser);
    if (world->equivalent_group_count == 0)
        world->equivalent_groups = NULL;
    else
        world->giz_buffer.addr = ALIGN(reinterpret_cast<usize>(cursor), 16);
}

i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special) {
    if (special != NULL && NuSpecialExistsFn(special)) {
        EQUIVALENTOBJECTGROUP_s *group = world->equivalent_groups;
        if (group != NULL) {
            for (i32 group_index = 0; group_index < world->equivalent_group_count; ++group_index) {
                for (i32 i = 0; i < group->object_count; ++i) {
                    if (NuSpecialCompare(special, &group->objects[i])) {
                        for (i32 j = 0; j < group->object_count; ++j) {
                            if (NuSpecialGetVisibilityFn(&group->objects[j])) {
                                *special = group->objects[j];
                                return 1;
                            }
                        }
                        return 0;
                    }
                }
                group = reinterpret_cast<EQUIVALENTOBJECTGROUP_s *>(reinterpret_cast<u8 *>(group) + group->byte_size);
            }
        }
    }
    return 0;
}

void LoadTerrainFile(WORLDINFO *world) {
    char path[256];
    world->terrain = NULL;
    if ((world->current_level->flags & LEVEL_TERRAIN) != 0) {
        NuStrCpy(path, world->config_file);
        LEVELDATA *level = world->current_level;
        if (level == (LEVELDATA *)PLATFORM_LDATA) {
            NuStrCpy(path, "levels\\episode_v\\cloudcityescape\\cloudcityescape_b\\cloudcityescape_b");
            level = world->current_level;
        }
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        void *terrain = TerrainInitEx(world->level_idx, &world->giz_buffer, world->unknown_0108.void_ptr, 0, path,
                                      world->current_gscn, 0, (u32)(u16)level->max_ter_groups,
                                      (u32)(u16)level->max_ter_groups, (u32)(u16)level->max_ter_platforms);
        world->terrain = terrain;
    }
}

void LoadGrassFile(WORLDINFO *world) {
    char path[256];
    world->page_grass = -1;
    Grass_Available = 1;
    char *config = world->config_file;

    if (g_isLowEndDevice != 0) {
        char *found = NuStrIStr(config, "Gungan");
        if (found == NULL)
            found = NuStrIStr(config, "SpeederChase");
        if (found == NULL)
            found = NuStrIStr(config, "EndorBattle");
        if (found == NULL)
            found = NuStrIStr(config, "Retake");

        if (found != NULL) {
            Grass_Available = 0;
        } else {
            Grass_Available = 1;
        }
    }

    if (NuStrIStr(config, "SpeederChase") != NULL) {
        Grass_Available = 0;
    }

    if (NuDeviceSpecs::ms_instance->specs < 3 && NuStrIStr(config, "GunGan_A") != NULL) {
        Grass_Available = 0;
        return;
    }

    if (__builtin_expect(Grass_Available != 0, 1)) {
        sprintf(path, "%s.gra", config);
        if (NuFileExists(path)) {
            i32 page = edgraLoadPage(path, world->current_gscn, *(i32 *)&world->terrain, &world->giz_buffer,
                                     &world->unknown_0108);
            world->page_grass = page;
        }
    }
}

void LoadBridgeFile(WORLDINFO *world) {
    char path[256];
    world->page_bridge = -1;
    sprintf(path, "%s.bri", world->config_file);
    if (NuFileExists(path)) {
        i32 page = edbriLoadPage(path, world->current_gscn);
        world->page_bridge = page;
    }
}

static __used__ void LC_BL_fix_strobing_anims(nufpar_s *fp) {
    (void)fp;
    levelconfig_ldata->flags |= LEVEL_FIX_STROBING_ANIMS;
}

static __used__ void LC_AL_conveyor(nufpar_s *fp) {
    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "xspeed") == 0) {
            levelconfig_ldata->conveyor_x_speed = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "zspeed") == 0) {
            levelconfig_ldata->conveyor_z_speed = NuFParGetFloat(fp);
        }
    }
}

static __used__ void LC_AL_cam_lateral_dist(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.0f) {
        v = 0.0f;
    }
    levelconfig_ldata->cam_lateral_dist = v;
}

static __used__ void LC_AL_cam_pullback_dist(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.0f) {
        v = 0.0f;
    }
    levelconfig_ldata->cam_pullback_dist = v;
}

static __used__ void LC_AL_waterripple_life(nufpar_s *fp) {
    levelconfig_ldata->unknown_0c0 = NuFParGetFloat(fp);
}

static __used__ void LC_AL_cam_tilt(nufpar_s *fp) {
    levelconfig_ldata->cam_tilt = NuFParGetFloat(fp);
}

static __used__ void LC_AL_reflect_y(nufpar_s *fp) {
    levelconfig_ldata->unknown_0cc = NuFParGetFloat(fp);
}

static __used__ void LC_AL_hover_height(nufpar_s *fp) {
    levelconfig_ldata->hover_height = NuFParGetFloat(fp);
}

static __used__ void LC_AL_nearclip(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.001f) {
        v = 0.001f;
    } else if (v > 1.0f) {
        v = 1.0f;
    }
    levelconfig_ldata->data_display.unknown_04 = v;
}

static __used__ void LC_AL_lowendfogstart(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 2.0f) {
        v = 2.0f;
    } else if (v > 20000.0f) {
        v = 20000.0f;
    }
    levelconfig_ldata->data_display.fog_start = v;
}

static __used__ void LC_AL_farclip_hack(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.1f) {
        v = 0.1f;
    } else if (v > 50.0f) {
        v = 50.0f;
    }
    levelconfig_ldata->data_display.unknown_00 = v;
}

static __used__ void LC_AL_camang_seek(nufpar_s *fp) {
    levelconfig_ldata->cam_angle_seek = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_campos_seek(nufpar_s *fp) {
    levelconfig_ldata->cam_pos_seek = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_endcol_a(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bf = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_endcol_b(nufpar_s *fp) {
    levelconfig_ldata->unknown_0be = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_endcol_g(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bd = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_endcol_r(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bc = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_startcol_a(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bb = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_startcol_b(nufpar_s *fp) {
    levelconfig_ldata->unknown_0ba = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_startcol_g(nufpar_s *fp) {
    levelconfig_ldata->unknown_0b9 = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_startcol_r(nufpar_s *fp) {
    levelconfig_ldata->unknown_0b8 = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_plat_scan_dist(nufpar_s *fp) {
    levelconfig_ldata->unknown_0db = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_raycaststep(nufpar_s *fp) {
    levelconfig_ldata->unknown_0da = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_reflect_range(nufpar_s *fp) {
    levelconfig_ldata->camera_judder_distance = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_blobshadow_fadefar(nufpar_s *fp) {
    levelconfig_ldata->blob_shadow_fade_far = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_blobshadow_fadenear(nufpar_s *fp) {
    levelconfig_ldata->blob_shadow_fade_near = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_blobshadow_alpha(nufpar_s *fp) {
    levelconfig_ldata->blob_shadow_alpha = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backb_bottom(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_blue_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backg_bottom(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_green_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backr_bottom(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_red_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backb_top(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_blue_top = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backg_top(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_green_top = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backr_top(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_red_top = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backb(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_blue_top =
        levelconfig_ldata->data_display.bg_blue_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backg(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_green_top =
        levelconfig_ldata->data_display.bg_green_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backr(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_red_top =
        levelconfig_ldata->data_display.bg_red_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_ter_platforms(nufpar_s *fp) {
    levelconfig_ldata->max_ter_platforms = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_ter_groups(nufpar_s *fp) {
    levelconfig_ldata->max_ter_groups = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_mipmapmode(nufpar_s *fp) {
    levelconfig_ldata->mipmap_mode = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_lowendcamerazoom(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->unknown_120 = v;
    } else {
        levelconfig_ldata->unknown_120 = 1.0f;
    }
}

static __used__ void LC_AL_lowendcharclip(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->unknown_11c = v;
    } else {
        levelconfig_ldata->unknown_11c = 0.0f;
    }
}

static __used__ void LC_AL_farclip(nufpar_s *fp) {
    i32 v = NuFParGetInt(fp);
    i32 farclip;
    if (v > 9) {
        farclip = (v <= 20000) ? v : 20000;
    } else {
        farclip = 10;
    }
    levelconfig_ldata->data_display.unknown_14 = (i16)(
        NuIOS_IsLowEndDevice()
            ? ((f32)farclip < levelconfig_ldata->data_display.far_clip
                   ? (f32)farclip
                   : levelconfig_ldata->data_display.far_clip)
            : (f32)farclip);
}

static __used__ void LC_AL_lowendparticlethin(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->data_display.particle_thin = v;
    } else {
        levelconfig_ldata->data_display.particle_thin = 1.0f;
    }
}

static __used__ void LC_AL_lowendfarclip(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 2.0f) {
        v = 2.0f;
    } else if (v > 20000.0f) {
        v = 20000.0f;
    }
    levelconfig_ldata->data_display.far_clip = v;
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->data_display.unknown_14 = (i16)v;
    }
}

static __used__ void LC_AL_terrain_rain(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_TERRAIN_RAIN;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_TERRAIN_RAIN;
    }
}

static __used__ void LC_AL_hidden_icons(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "on") == 0) {
            levelconfig_ldata->flags &= ~LEVEL_HIDE_ICONS;
            return;
        }
        if (NuStrICmp(fp->word_buf, "off") == 0) {
            levelconfig_ldata->flags |= LEVEL_HIDE_ICONS;
        }
    }
}

static __used__ void LC_AL_sfx_ambient(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        levelconfig_ldata->unknown_0a2 = (i16)GetSfxId(fp->word_buf);
    }
}

static __used__ void LC_AL_music_other(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        levelconfig_ldata->unknown_0a8 = (i16)GetMusicIndex(fp->word_buf, MusicInfo, -1);
        levelconfig_ldata->music_tracks[0][1] = music_man.GetTrackHandle(TRACK_CLASS_QUIET, fp->word_buf);
        levelconfig_ldata->music_tracks[1][1] = music_man.GetTrackHandle(TRACK_CLASS_ACTION, fp->word_buf);
        levelconfig_ldata->music_tracks[2][1] = music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, fp->word_buf);
    }
}

static __used__ void LC_AL_music(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        levelconfig_ldata->music_index = (i16)GetMusicIndex(fp->word_buf, MusicInfo, -1);
        levelconfig_ldata->music_tracks[0][0] = music_man.GetTrackHandle(TRACK_CLASS_QUIET, fp->word_buf);
        levelconfig_ldata->music_tracks[1][0] = music_man.GetTrackHandle(TRACK_CLASS_ACTION, fp->word_buf);
        levelconfig_ldata->music_tracks[2][0] = music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, fp->word_buf);
    }
}

static __used__ void LC_AL_metal(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_METAL;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_METAL;
    }
}

static __used__ void LC_AL_forget_takeovers(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_FORGET_TAKEOVERS;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_FORGET_TAKEOVERS;
    }
}

static __used__ void LC_AL_pickups_to_panel(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_PICKUPS_TO_PANEL;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_PICKUPS_TO_PANEL;
    }
}

static __used__ void LC_AL_override_nopickupgravity(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_OVERRIDE_NO_PICKUP_GRAVITY;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_OVERRIDE_NO_PICKUP_GRAVITY;
    }
}

static __used__ void LC_AL_narrow_socks(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_NARROW_SOCKS;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_NARROW_SOCKS;
    }
}

static __used__ void LC_AL_double_score(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_DOUBLE_SCORE;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_DOUBLE_SCORE;
    }
}

static __used__ void LC_AL_in_space(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_IN_SPACE;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_IN_SPACE;
    }
}

static __used__ void LC_AL_camera_rain(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_CAMERA_RAIN;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_CAMERA_RAIN;
    }
}

static __used__ void LC_AL_flat_terrain(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_FLAT_TERRAIN;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_FLAT_TERRAIN;
    }
}

void Level_RegisterGameConfigKeywords(nufpcomjmp_s *beforeLoadKeywords, nufpcomjmp_s *afterLoadKeywords) {
    Level_ConfigBeforeLoad_GameKeywords = beforeLoadKeywords;
    Level_ConfigAfterLoad_GameKeywords = afterLoadKeywords;
}

LEVELDATA *Levels_ConfigureList(char *file, VARIPTR *buf, VARIPTR *buf_end, i32 max_level_count, i32 *level_count_out,
                                LEVELSETDEFAULTSFN *set_defaults_fn) {
    NUFPAR *parser;
    i32 in_level_config;
    i32 n;
    LEVELDATA *level_data;
    LEVELDATA *cur_level;
    i32 i;
    i32 j;

    parser = NuFParCreate(file);

    MAXLDATA = max_level_count;
    n = 0;

    level_data = (LEVELDATA *)ALIGN(buf->addr, 0x4);
    buf->void_ptr = level_data;

    cur_level = level_data;

    in_level_config = 0;

    while (NuFParGetLine(parser) != 0) {
        NuFParGetWord(parser);
        if (parser->word_buf[0] != '\0') {
            if (in_level_config) {
                if (NuStrICmp(parser->word_buf, "level_end") == 0) {
                    in_level_config = 0;

                    if (cur_level->dir[0] == '\0' || cur_level->name[0] == '\0' ||
                        (cur_level->flags & LEVEL_TEST) != 0) {
                        continue;
                    }

                    if ((cur_level->flags & LEVEL_NEWGAME) != 0) {
                        NEWGAME_LDATA = cur_level;
                    }

                    if ((cur_level->flags & LEVEL_LOADGAME) != 0) {
                        LOADGAME_LDATA = cur_level;
                    }

                    n++;
                    in_level_config = 0;
                    cur_level++;

                    continue;
                }

                if (NuStrICmp(parser->word_buf, "dir") == 0) {
                    if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 0x40) {
                        NuStrCpy(cur_level->dir, parser->word_buf);
                    }
                } else if (NuStrICmp(parser->word_buf, "file") == 0) {
                    if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 0x20) {
                        NuStrCpy(cur_level->name, parser->word_buf);
                    }
                }

                if (NuStrICmp(parser->word_buf, "test_level") == 0) {
                    cur_level->flags |= LEVEL_TEST;
                } else if (NuStrICmp(parser->word_buf, "intro_level") == 0) {
                    cur_level->flags |= LEVEL_INTRO;
                } else if (NuStrICmp(parser->word_buf, "midtro_level") == 0 ||
                           NuStrICmp(parser->word_buf, "cutscene_level") == 0) {
                    cur_level->flags |= LEVEL_MIDTRO;
                } else if (NuStrICmp(parser->word_buf, "outro_level") == 0) {
                    cur_level->flags |= LEVEL_OUTRO;
                } else if (NuStrICmp(parser->word_buf, "status_level") == 0) {
                    cur_level->flags &= ~LEVEL_GAMEPLAY;
                    cur_level->flags &= ~LEVEL_TERRAIN;
                    cur_level->flags |= LEVEL_STATUS;
                } else if (NuStrICmp(parser->word_buf, "newgame_level") == 0) {
                    if (NEWGAME_LDATA == NULL) {
                        cur_level->flags |= LEVEL_NEWGAME;
                    }
                } else if (NuStrICmp(parser->word_buf, "loadgame_level") == 0) {
                    if (LOADGAME_LDATA == NULL) {
                        cur_level->flags |= LEVEL_LOADGAME;
                    }
                }

                continue;
            }

            if (NuStrICmp(parser->word_buf, "level_start") == 0 && n < MAXLDATA) {
                cur_level->dir[0] = '\0';
                cur_level->name[0] = '\0';

                cur_level->unknown_060 = -1;

                cur_level->idx = n;

                cur_level->flags = LEVEL_GAMEPLAY | LEVEL_UNKNOWN_FLAG_4 | LEVEL_TERRAIN;

                cur_level->load_fn = NULL;
                cur_level->init_fn = NULL;
                cur_level->reset_fn = NULL;
                cur_level->update_fn = NULL;
                cur_level->always_update_fn = NULL;
                cur_level->draw_fn = NULL;
                cur_level->draw_status_fn = NULL;

                cur_level->data_display.unknown_14 = 20000;
                cur_level->data_display.unknown_00 = 0.1f;
                cur_level->data_display.unknown_04 = 0.15f;
                cur_level->data_display.far_clip = 20000.0f;
                cur_level->data_display.fog_start = 20100.0f;
                cur_level->data_display.particle_thin = g_isLowEndDevice ? 4.0f : 1.0f;
                cur_level->data_display.bg_red_bottom = 0;
                cur_level->data_display.bg_red_top = 0;
                cur_level->data_display.bg_green_bottom = 0;
                cur_level->data_display.bg_green_top = 0;
                cur_level->data_display.bg_blue_bottom = 0;
                cur_level->data_display.bg_blue_top = 0;

                cur_level->music_index = -1;

                cur_level->unknown_11c = 0.0f;
                cur_level->unknown_120 = 1.0f;

                cur_level->unknown_0a2 = -1;
                cur_level->max_ter_platforms = 0x80;
                cur_level->max_ter_groups = 0x100;
                cur_level->unknown_0a8 = -1;
                cur_level->unknown_0aa = -1;

                cur_level->mipmap_mode = 0x03;
                cur_level->blob_shadow_alpha = 0x7f;
                cur_level->unknown_0ae = -1;
                cur_level->area_index = -1;

                cur_level->unknown_0b8 = 0x50;

                cur_level->cam_tilt = 0.0f;

                cur_level->hover_height = 0.0f;

                cur_level->unknown_0b9 = 0x50;
                cur_level->unknown_0ba = 0x50;
                cur_level->unknown_0bb = 0x32;
                cur_level->unknown_0bc = 0x00;
                cur_level->unknown_0bd = 0x00;
                cur_level->unknown_0be = 0x00;
                cur_level->unknown_0bf = 0x00;

                cur_level->unknown_0c0 = 0.5f;
                cur_level->cam_pullback_dist = 0.0f;
                cur_level->cam_lateral_dist = 0.0f;
                cur_level->unknown_0cc = 2e+06f;

                cur_level->area_level_index = -1;
                cur_level->blob_shadow_fade_near = 5;
                cur_level->blob_shadow_fade_far = 10;
                cur_level->cam_pos_seek = 5;
                cur_level->cam_angle_seek = 5;
                cur_level->camera_judder_distance = 10;
                cur_level->unknown_0da = 5;
                cur_level->unknown_0db = 5;

                cur_level->conveyor_x_speed = 0.0f;
                cur_level->conveyor_z_speed = 0.0f;

                for (i = 0; i < 2; i++) {
                    for (j = 0; j < 3; j++) {
                        cur_level->music_tracks[j][i] = -1;
                    }
                }

                in_level_config = 1;

                if (set_defaults_fn != NULL) {
                    (*set_defaults_fn)(cur_level);
                }
            }
        }
    }

    NuFParDestroy(parser);

    if (n == 0) {
        return NULL;
    }

    buf->void_ptr = cur_level;

    if (level_count_out != NULL) {
        *level_count_out = n;
    }

    return level_data;
}

LEVELDATA *Level_FindByName(char *name, i32 *idx_out) {
    for (i32 i = 0; i < LEVELCOUNT; i++) {
        if (NuStrICmp(LDataList[i].name, name) == 0) {
            if (idx_out != NULL) {
                *idx_out = i;
            }

            return &LDataList[i];
        }
    }

    if (idx_out != NULL) {
        *idx_out = -1;
    }

    return NULL;
}

void Levels_FixUp(LEVELFIXUP *fixup) {
    if (fixup != NULL) {
        for (; fixup->name != NULL; fixup++) {
            if (fixup->level != NULL) {
                LEVELDATA *level = Level_FindByName(fixup->name, NULL);
                *fixup->level = level;
                if (level != NULL) {
                    if (fixup->load_fn != NULL) {
                        level->load_fn = fixup->load_fn;
                    }
                    if (fixup->init_fn != NULL) {
                        level->init_fn = fixup->init_fn;
                    }
                    if (fixup->reset_fn != NULL) {
                        level->reset_fn = fixup->reset_fn;
                    }
                    if (fixup->update_fn != NULL) {
                        level->update_fn = fixup->update_fn;
                    }
                    if (fixup->always_update_fn != NULL) {
                        level->always_update_fn = fixup->always_update_fn;
                    }
                    if (fixup->draw_fn != NULL) {
                        level->draw_fn = fixup->draw_fn;
                    }
                    if (fixup->draw_status_fn != NULL) {
                        level->draw_status_fn = fixup->draw_status_fn;
                    }
                }
            }
        }
    }
}

void LevelConfig_BeforeLoad(LEVELDATA *level, char *buffer, nufpcomjmp_s *keywords) {
    NUFPAR *parser = NuFParCreateMem("levelbeforeload", buffer, 0xffff);
    if (parser == NULL) {
        return;
    }

    levelconfig_ldata = level;
    NuFParPushCom2(parser, LevelConfig_BeforeLoad_GenericKeywords, keywords);
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0) {
            NuFParInterpretWord(parser);
        }
    }
    NuFParDestroy(parser);
}

void LevelConfig_AfterLoad(LEVELDATA *level, char *buffer, nufpcomjmp_s *keywords) {
    NUFPAR *parser = NuFParCreateMem("levelafterload", buffer, 0xffff);
    if (parser == NULL) {
        return;
    }

    levelconfig_ldata = level;
    NuFParPushCom2(parser, LevelConfig_AfterLoad_GenericKeywords, keywords);
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0) {
            NuFParInterpretWord(parser);
        }
    }
    NuFParDestroy(parser);

    if (level->blob_shadow_fade_near > level->blob_shadow_fade_far) {
        level->blob_shadow_fade_near = level->blob_shadow_fade_far;
    }
    level->field91_0x118 = level->data_display.unknown_14;
    level->flags |= LEVEL_CONFIG_LOADED;
}

void LevelSplines_InitForGame(LEVELSPLINE *splines) {
    LevSplList = splines;
    LEVELSPLINECOUNT = 0;

    if (splines == NULL) {
        return;
    }

    for (LEVELSPLINE *spline = splines; spline->name != NULL; ++spline) {
        if (levspl_i_start == -1 && NuStrICmp(spline->name, "start") == 0) {
            levspl_i_start = LEVELSPLINECOUNT;
        } else if (levspl_i_startcam == -1 && NuStrICmp(spline->name, "start_cam") == 0) {
            levspl_i_startcam = LEVELSPLINECOUNT;
        }
        ++LEVELSPLINECOUNT;
    }
}

void LevelSplines_InitForLevel(WORLDINFO_s *world) {
    char name[64];
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    world->portal_places = reinterpret_cast<PORTALPOS **>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr += LEVELSPLINECOUNT * sizeof(*world->portal_places);
    memset(world->portal_places, 0, LEVELSPLINECOUNT * sizeof(*world->portal_places));

    if (LevSplList == NULL) {
        return;
    }

    for (i32 i = 0; i < LEVELSPLINECOUNT; ++i) {
        LEVELSPLINE *entry = &LevSplList[i];
        if (__builtin_expect((entry->area != -1 && world->level_sub_id != entry->area) ||
            (entry->level != -1 && world->level_idx != entry->level), 0)) {
            continue;
        }

        NUGSCN *scene = *(entry->scene != NULL ? entry->scene : &world->current_gscn);
        if (scene != NULL) {
            NUGSPLINE *spline = NuSplineFind(scene, const_cast<char *>(entry->name));
            world->portal_places[i] = reinterpret_cast<PORTALPOS *>(spline);
            if (spline != NULL) {
                const i32 point_count = spline->length;
                if (__builtin_expect((entry->min_points != 0 && point_count < entry->min_points) ||
                    (entry->max_points != 0 && entry->min_points <= entry->max_points &&
                     point_count > entry->max_points), 1)) {
                    world->portal_places[i] = NULL;
                }
            }
        }

        if (scene == NULL) {
            continue;
        }

        if (levspl_i_start >= 0 && levspl_i_start < LEVELSPLINECOUNT) {

            name[0] = '\0';
            if (Mission_Active(NULL) != NULL) {
                NuStrCpy(name, "bounty_start");
            } else if (world->level_sub_id != -1 && (ADataList[world->level_sub_id].flags & 0x40) != 0 &&
                       hub_from_cutsceneplayer != 0) {
                NuStrCpy(name, "shop_start");
                if (CutScenePlayer_Available() != NULL &&
                    static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available())->return_door != -1) {
                    name[0] = '\0';
                }
            }
            if (name[0] != '\0') {
                NUGSPLINE *start = NuSplineFind(scene, name);
                if (start != NULL && start->length > 1) {
                    world->portal_places[levspl_i_start] = reinterpret_cast<PORTALPOS *>(start);
                    if (levspl_i_startcam >= 0 && levspl_i_startcam < LEVELSPLINECOUNT) {
                        world->portal_places[levspl_i_startcam] = NULL;
                    }
                }
            }
        }
    }
}

void LevelObjects_InitForGame(LEVELOBJECT *tab, VARIPTR *buf, VARIPTR *buf_end, i32 max, i32 name_table_size) {
    LEVOBJREF_FIRSTOBJ = -1;
    LEVOBJREF_LASTOBJ = -1;
    LEVOBJREF_FIRSTREFOBJ = -1;
    LEVOBJREF_LASTREFOBJ = -1;
    LEVELOBJECTMAX = max;
    ObjTabList = tab;
    LEVELSPLINECOUNT = 0;

    for (; tab->kind != 0xff; ++tab, ++LEVELOBJECTCOUNT) {
        if (tab->reflection != 1) {
            if (tab->reflection == 2) {
                if (LEVOBJREF_FIRSTREFOBJ == -1) {
                    LEVOBJREF_FIRSTREFOBJ = LEVELOBJECTCOUNT;
                }
                LEVOBJREF_LASTREFOBJ = LEVELOBJECTCOUNT;
            }
        } else {
            if (LEVOBJREF_FIRSTOBJ == -1) {
                LEVOBJREF_FIRSTOBJ = LEVELOBJECTCOUNT;
            }
            LEVOBJREF_LASTOBJ = LEVELOBJECTCOUNT;
        }
    }

    const i32 object_range = LEVOBJREF_LASTOBJ - LEVOBJREF_FIRSTOBJ;
    const i32 reflection_range = LEVOBJREF_LASTREFOBJ - LEVOBJREF_FIRSTREFOBJ;
    if (object_range > reflection_range) {
        LEVOBJREF_LASTOBJ = LEVOBJREF_FIRSTOBJ + reflection_range;
    } else if (object_range < reflection_range) {
        LEVOBJREF_LASTREFOBJ = LEVOBJREF_FIRSTREFOBJ + object_range;
    }

    if (name_table_size > 0) {
        ExtraLevelObject_NameTable = (char *)buf->u8_ptr;
        ExtraLevelObject_NameTableSize = name_table_size;
        buf->addr += name_table_size;
    }
}

i32 LevelObject_AddExtra(char *name, i32 kind) {
    if (LEVELOBJECTCOUNT < LEVELOBJECTMAX && ExtraLevelObject_NameTable != NULL) {
        i32 nameLen = NuStrLen(name);
        if (nameLen + ExtraLevelObject_NameTableIndex + 1 < ExtraLevelObject_NameTableSize) {
            char *nameDest = ExtraLevelObject_NameTable + ExtraLevelObject_NameTableIndex;
            LEVELOBJECT *obj = &ObjTabList[LEVELOBJECTCOUNT];
            obj->kind = (u8)kind;
            obj->name = nameDest;
            LEVELOBJECTCOUNT++;
            EXTRALEVELOBJECTCOUNT++;
            NuStrCpy(nameDest, name);
            ExtraLevelObject_NameTableIndex += nameLen + 1;
            return 1;
        }
    }
    return 0;
}

// LevelObjects_InitForLevel @0x475630. Creates the runtime model table and
// resolves each registered model from the scene selected by its table kind.
// Found models are hidden in their source scene: their runtime handle is the
// template used by gizmos and other placed objects to draw them elsewhere.
void LevelObjects_InitForLevel(WORLDINFO_s *world) {
    usize aligned = ALIGN(world->giz_buffer.addr, 4);
    world->lev_objs = reinterpret_cast<LEVEL_OBJECT_RUNTIME *>(aligned);
    world->giz_buffer.addr = aligned + static_cast<usize>(LEVELOBJECTCOUNT) * sizeof(LEVEL_OBJECT_RUNTIME);
    memset(world->lev_objs, 0, static_cast<usize>(LEVELOBJECTCOUNT) * sizeof(LEVEL_OBJECT_RUNTIME));

    if (ObjTabList == NULL || LEVELOBJECTCOUNT <= 0) {
        return;
    }

    for (i32 object_index = 0; object_index < LEVELOBJECTCOUNT; ++object_index) {
        LEVELOBJECT &object_type = ObjTabList[object_index];
        LEVEL_OBJECT_RUNTIME &object = world->lev_objs[object_index];

        if (NuStrICmp(object_type.name, "power_up") == 0) {
            KNOBS = object_index;
        }

        NUGSCN *scene = NULL;
        switch (object_type.kind) {
            case LEVEL_OBJECT_SCENE_LEVEL:
                scene = world->current_gscn;
                break;

            case LEVEL_OBJECT_SCENE_AREA:
                scene = area_scene != NULL ? area_scene : world->current_gscn;
                break;

            case LEVEL_OBJECT_SCENE_CHARACTER_ICON:
                for (i32 character_id = 0; character_id < CHARCOUNT; ++character_id) {
                    scene = IconScene_FindById(character_id);
                    if (scene != NULL && NuSpecialFind(scene, &object.special, object_type.name, 1) != 0) {
                        goto object_resolved;
                    }
                }
                scene = big_icon_scene != NULL ? big_icon_scene : world->icons_gscn;
                break;

            case LEVEL_OBJECT_SCENE_SAVE_ICON:
                scene = saveicon_scene;
                break;

            case LEVEL_OBJECT_SCENE_VEHICLE:
                scene = vehicle_scene;
                break;

            case LEVEL_OBJECT_SCENE_BUTTON:
                scene = button_scene;
                break;

            case LEVEL_OBJECT_SCENE_THINGS:
            default:
                if (area_scene != NULL && world->area != NULL &&
                    (world->area->flags & AREAFLAG_OVERRIDE_THINGS_SCENE) != 0) {
                    if (NuSpecialFind(area_scene, &object.special, object_type.name, 1) != 0) {
                        goto object_resolved;
                    }
                }
                scene = things_scene;
                break;
        }

        if (scene != NULL) {
            NuSpecialFind(scene, &object.special, object_type.name, 1);
        }

    object_resolved:
        object.active = static_cast<u8>(NuSpecialExistsFn(&object.special));
        if (object.active != 0) {
            NuSpecialSetVisibility(&object.special, 0);
        }
    }
}

i32 LevelObject_FindIndexFromName(char *name) {
    if (ObjTabList != NULL && LEVELOBJECTCOUNT > 0) {
        for (i32 i = 0; i < LEVELOBJECTCOUNT; i++) {
            if (NuStrICmp(ObjTabList[i].name, name) == 0) {
                return i;
            }
        }
    }
    return -1;
}

void LevObj_FixUpPlatIDs(WORLDINFO_s *world) {
    i32 i;
    LEVEL_OBJECT_RUNTIME *obj;

    if (ObjTabList == NULL || LEVELOBJECTCOUNT <= 0) {
        return;
    }
    for (i = 0; i < LEVELOBJECTCOUNT; i++) {
        obj = &world->lev_objs[i];
        obj->platform_id = -1;
        if (world->terrain != NULL && NuSpecialExistsFn(&obj->special) && ObjTabList[i].kind == 1) {
            obj = &world->lev_objs[i];
            obj->platform_id = FindPlatInst(NuSpecialGetInstanceix(&obj->special));
        }
    }
}

void *LevObj_FindByPlatID(WORLDINFO_s *world, i32 platID) {
    LEVEL_OBJECT_RUNTIME *obj = world->lev_objs;
    for (i32 i = 0; i < LEVELOBJECTCOUNT; i++, obj++) {
        if (obj->platform_id == platID) {
            return obj;
        }
    }
    return NULL;
}

char *LevelObject_FindNameFromIndex(i32 index) {
    char *name = (char *)"";
    if (ObjTabList != NULL && index >= 0 && index < LEVELOBJECTCOUNT) {
        name = ObjTabList[index].name;
    }
    return name;
}

i32 LevelObject_GetReflection(i32 objId) {
    if (objId != -1 && LEVOBJREF_FIRSTOBJ <= objId && objId <= LEVOBJREF_LASTOBJ) {
        objId = (objId - LEVOBJREF_FIRSTOBJ) + LEVOBJREF_FIRSTREFOBJ;
    }
    return objId;
}

i32 LevelObject_FindIndexFromName_RefOnly(char *name) {
    if (ObjTabList != NULL && LEVOBJREF_FIRSTOBJ != -1 && LEVOBJREF_FIRSTOBJ <= LEVOBJREF_LASTOBJ) {
        for (i32 i = LEVOBJREF_FIRSTOBJ; i <= LEVOBJREF_LASTOBJ; ++i) {
            if (NuStrICmp(ObjTabList[i].name, name) == 0) {
                return i;
            }
        }
    }
    return -1;
}

void Level_LoadConfigFile(WORLDINFO *world) {
    char name[128];

    ConfigBuffer[0] = '\0';
    sprintf(name, "%s.txt", world->config_file);

    world->giz_buffer.void_ptr = (void *)ALIGN((usize)world->giz_buffer.void_ptr, 4);
    i32 bytesRead = NuFileLoadBuffer(name, world->giz_buffer.void_ptr, 0x10000);
    world->config_count = bytesRead;
    if (bytesRead > 0) {
        ((char *)world->giz_buffer.void_ptr)[bytesRead] = '\0';
        bytesRead = Text_StripComments((char *)world->giz_buffer.void_ptr, ConfigBuffer, 1);
        world->config_count = bytesRead;
    }
}

void Level_Update(WORLDINFO *world) {
    void (*updateFn)(WORLDINFO *) = world->current_level->update_fn;
    if (updateFn != NULL) {
        updateFn(world);
    }

    if (DoubleScoreTime > 0.0f) {
        if ((i32)(GameTimer.time_elapsed + GameTimer.time_elapsed) !=
            (i32)(GameTimer.last_time_elapsed + GameTimer.last_time_elapsed)) {
            GameAudio_PlaySfxAndSetVolume(0x35, NULL, DoubleScoreTime);
        }
    }

    if (AreaGlobals.values.field_0x00 == 0) {
        if (Cheats_CheckFlags(0x2000) != 0) {
            AreaGlobals.values.field_0x00 = 1;
        }
    }
}

void Level_Draw(WORLDINFO *world) {
    if (world->current_level->draw_fn != NULL) {
        world->current_level->draw_fn(world);
    }
}

void GoToNewLevel(i32 levelIdx) {
    NewLData = &LDataList[levelIdx];
    if (waiting_for_level != -1) {
        waiting_for_new_level = 1;
    }
}

void CompleteLevel(WORLDINFO *world) {
    grab_screen_image = 1;

    if (NewLData == NULL || ((NewLData->flags & 0xe0) != 0 && FreePlay != 0)) {
        LEVELDATA *statusLevel = Area_FindStatusLevel(world->area, NULL);
        NewLData = statusLevel;
        if (statusLevel == NULL) {
            NewLData = HUB_LDATA;
        }
    }

    if (waiting_for_level != -1) {
        waiting_for_new_level = 1;
    }
}

void ResetLevel(WORLDINFO_s *world, char *cutscene, i32) {
    CutDeadVehiclePlayer = NULL;
    if (world != NULL && NewCutScene(NULL, world->cutscene_sys, cutscene, 0) != NULL) {
        ResetBits |= 7;
        if (player != NULL && player->apiobj.field_0x287 != 0)
            CutDeadVehiclePlayer = player;
        else if (player2 != NULL && player2->apiobj.field_0x287 != 0)
            CutDeadVehiclePlayer = player2;
        reset_reimport = 1;
    } else {
        NewGameMode();
        reset_restart = 1;
        grab_screen_image = 1;
        ResetBits |= 0x17;
        GizmoTypeStoreProgress(WORLD->gizmo_sys, WORLD, (i8)WORLD->current_level->area_level_index, gizmopickup_typeid,
                               NULL);
    }
}

void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32) {
    if (no_more_loads == 0) {
        volatile i32 *abort = &abort_load;
        no_more_loads = 1;
        *abort = 1;
    }

    Door_Reset();
    NewLData = level;
    new_level_from_menu = 1;
    newlevelfrommenu_newmenuid = menu_id;
    newlevelfrommenu_newmenuy = menu_y;

    if (HUB_ADATA != NULL && WORLD->area == HUB_ADATA) {
        return;
    }

    if (SuperStory != 0) {
        hub_from_superstory = SuperStoryEpisode;
        return;
    }

    MISSIONDATA *mission = Mission_Active(NULL);
    if (mission != NULL && NewLData != NULL && NewLData == HUB_LDATA) {
        hub_from_mission = static_cast<i8>(mission->count);
    }
}

void LevelStreaming_DoorOverride(WORLDINFO_s *world, LEVELDATA_s *level, f32 radius, f32 *distance) {
    NUVEC position;
    if (level != NULL && Players_AveragePos(&position, NULL) != 0) {
        DOOR_s *door = Door_FindByIndex(world, -1, level->idx, &position);
        if (door != NULL) {
            f32 distance_squared = NuVecDistSqr(&position, &door->pos, NULL);
            if (distance_squared < radius * radius) {
                other_level_override = level->idx;
                if (distance != NULL) {
                    *distance = NuFsqrt(distance_squared);
                }
            }
        }
    }
}

