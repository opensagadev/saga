// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/props/doors/door.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "batman.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/core/startup/main.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/numath/numtx.h"

#include <GLES2/gl2.h>
#include <stdio.h>
#include <string.h>
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "nu2api/numath/nutrig.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/render/core/rtl.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/items/base/apiobject.h"
#include <new>
#include "nu2api/numath/nuang.h"
#include <stdlib.h>
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nu3d/nugscn.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "legoapi/misc/utilities.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
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
#include "nu2api/nu3d/nu3d_includes.h"

// The fade system and the four fade effects handed to FadeSystem::AddFade()
// are screen.cpp globals in the original: its static initializer registers
// their destructors (original bss @0x127bf20 / 0x127bf08 / 0x127bf00 /
// 0x127bef8 / 0x127bef0).
FadeSystem FadeSys;
Fade fade;
FadeWipe fadeWipe;
FadeStillWipe fadeStillWipe;
FadeStill fadeStill;

// Material handles used by the original 2-D fade paths.
NUMTL *ScreenFadeMtl;
NUMTL *FadeMtl2;
NUMTL *FadeMtl;

static u8 ScreenGrabNeeded;
static i32 pause_rt;
static NUMTL *pause_rndr_mtl;
static i32 old_pause_state;
i32 (*PauseRenderOffFn)(void);
i32 cut_waiting_for_new_level;

nugscn_s *FadeLoop_ObjScene;
nuhspecial_s FadeLoop_ObjHSpecial;

struct nuhspecial_s;

static NUGSCN *backdrop_scene = nullptr;
static nuhspecial_s backdrop_hspecial[4];
static f32 backdrop_back_wait = 0.0f;

f32 backdrop_top_r = 0.0f;
f32 backdrop_top_g = 0.0f;
f32 backdrop_top_b = 0.0f;
f32 backdrop_bot_r = 0.0f;
f32 backdrop_bot_g = 0.0f;
f32 backdrop_bot_b = 24.0f;

static f32 backdrop_top_tr = 0.0f;
static f32 backdrop_top_tg = 0.0f;
static f32 backdrop_top_tb = 0.0f;
static f32 backdrop_bot_tr = 0.0f;
static f32 backdrop_bot_tg = 0.0f;
static f32 backdrop_bot_tb = 24.0f;

i32 backdrop_black = 0;

void (*BackDrop_AlphaFn)(float *) = nullptr;

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).

GAMEFOG_STATE GameFog = {};
f32 dof_fstop;

static i32 GameFogSnap;
static rtlfog_s *GameFogSet;
static f32 GameFogDuration;
static f32 GameFogTime;
static GAMEFOG_STATE GameFogOld;
static GAMEFOG_STATE GameFogNew;

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct WORLDINFO_s;

extern i32 LevFlag[4];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

struct EDCREATURE_s;

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

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

union variptr_u;

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

struct eduimenu_s;
struct eduiitem_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

void FadeLoop_SetObj(nugscn_s *scene, char *name) {
    FadeLoop_ObjScene = scene;
    if (scene != NULL && NuSpecialFind(scene, &FadeLoop_ObjHSpecial, name, 1) == 0) {
        FadeLoop_ObjScene = NULL;
    }
}

i32 FadeLoop_UsingObj() {
    return FadeLoop_ObjScene != NULL;
}

void FadeLoop_DrawObj(float alpha) {
    if (FadeLoop_ObjScene != NULL && NuSpecialExistsFn(&FadeLoop_ObjHSpecial)) {
        NUVEC scale = {0.125f, 0.125f, 0.125f};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        matrix.m32 = 1.0f;
        NuSpecialDrawAtAlpha(&FadeLoop_ObjHSpecial, &matrix, alpha);
    }
}

void CreateFadeMaterials() {
    FadeMtl = NuMtlCreate(1);
    FadeMtl->attribs.z_mode = 3;
    FadeMtl->attribs.alpha_mode = 3;
    FadeMtl->attribs.unknown_2_1_2 = 2;
    FadeMtl->attribs.unknown_1_1_2 = 1;
    FadeMtl->attribs.unknown_1_4_8 = 1;
    FadeMtl->attribs.unknown_2_4 = 1;
    FadeMtl->attribs.filter_mode = 1;
    NuMtlUpdate(FadeMtl);

    FadeMtl2 = NuMtlCreate(1);
    FadeMtl2->attribs.z_mode = 3;
    FadeMtl2->attribs.alpha_mode = 1;
    FadeMtl2->attribs.unknown_2_1_2 = 2;
    FadeMtl2->attribs.unknown_1_1_2 = 1;
    FadeMtl2->attribs.unknown_1_4_8 = 1;
    FadeMtl2->attribs.unknown_2_4 = 1;
    FadeMtl2->attribs.filter_mode = 1;
    NuMtlUpdate(FadeMtl2);

    ScreenFadeMtl = NuMtlCreate(1);
    ScreenFadeMtl->opacity = 1.0f;
    ScreenFadeMtl->tex_id = 0;
    ScreenFadeMtl->sort_pri = 0x7ffe;
    ScreenFadeMtl->attribs.z_mode = 3;
    ScreenFadeMtl->attribs.unknown_1_1_2 = 1;
    ScreenFadeMtl->attribs.unknown_2_1_2 = 2;
    ScreenFadeMtl->attribs.unknown_2_4 = 1;
    ScreenFadeMtl->attribs.unknown_1_4_8 = 1;
    ScreenFadeMtl->attribs.alpha_mode = 1;
    ScreenFadeMtl->attribs.filter_mode = 1;
    NuMtlUpdate(ScreenFadeMtl);
}

void BackDrop_ResetColours() {
    backdrop_top_r = 0.0f;
    backdrop_top_g = 0.0f;
    backdrop_top_b = 0.0f;
    backdrop_bot_r = 0.0f;
    backdrop_bot_g = 0.0f;
    backdrop_bot_b = 0.0f;
    backdrop_back_wait = 0.0001f;
    backdrop_top_tr = 0.0f;
    backdrop_top_tg = 0.0f;
    backdrop_top_tb = 0.0f;
    backdrop_bot_tr = 0.0f;
    backdrop_bot_tg = 0.0f;
    backdrop_bot_tb = 0.0f;
}

void BackDrop_UpdateColours(i32 instant) {
    if (backdrop_black) {
        backdrop_top_r = 0.0f;
        backdrop_top_g = 0.0f;
        backdrop_top_b = 0.0f;
        backdrop_bot_r = 0.0f;
        backdrop_bot_g = 0.0f;
        backdrop_bot_b = 0.0f;
        return;
    }

    if (backdrop_back_wait > 0.0f) {
        backdrop_back_wait -= FRAMETIME;
        if (!(0.0f >= backdrop_back_wait)) {
            return;
        }
        if (instant != 0) {
            backdrop_top_tb = 0.0f;
            backdrop_top_tg = 0.0f;
            backdrop_top_tr = 0.0f;
            backdrop_bot_tb = 0.0f;
            backdrop_bot_tg = 0.0f;
            backdrop_bot_tr = 0.0f;
        } else {
            backdrop_top_tr = 0.0f;
            backdrop_top_tg = 0.0f;
            backdrop_top_tb = 0.0f;
            backdrop_bot_tr = 0.0f;
            backdrop_bot_tg = 0.0f;
            backdrop_bot_tb = 0.0f;
        }
        return;
    }

    if (backdrop_top_r == backdrop_top_tr && backdrop_top_g == backdrop_top_tg && backdrop_top_b == backdrop_top_tb &&
        backdrop_bot_r == backdrop_bot_tr && backdrop_bot_g == backdrop_bot_tg && backdrop_bot_b == backdrop_bot_tb) {
        backdrop_back_wait = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 2.5f + 2.5f;
        return;
    }

    const f32 step = FRAMETIME * 10.0f;
    backdrop_top_r = SeekLinearF(backdrop_top_r, backdrop_top_tr, step);
    backdrop_top_g = SeekLinearF(backdrop_top_g, backdrop_top_tg, step);
    backdrop_top_b = SeekLinearF(backdrop_top_b, backdrop_top_tb, step);
    backdrop_bot_r = SeekLinearF(backdrop_bot_r, backdrop_bot_tr, step);
    backdrop_bot_g = SeekLinearF(backdrop_bot_g, backdrop_bot_tg, step);
    backdrop_bot_b = SeekLinearF(backdrop_bot_b, backdrop_bot_tb, step);
}

void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end) {
    if (backdrop_scene == NULL) {
        backdrop_scene = NuGScnRead(buf, *buf_end, path);
    }
    memset(backdrop_hspecial, 0, sizeof(backdrop_hspecial));
    if (backdrop_scene == NULL) {
        return;
    }
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[0], (char *)"ball1", 1);
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[1], (char *)"ball2", 1);
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[2], (char *)"ball3", 1);
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[3], (char *)"ball4", 1);
}

void BackDrop_Update(float dt) {
    if (backdrop_scene != NULL) {
        NuGScnUpdate(backdrop_scene, dt * 60.0f);
    }
}

void BackDrop_Draw(float alpha, i32 flags) {
    if (backdrop_scene == NULL) {
        return;
    }
    if (flags == 0 && BackDrop_AlphaFn != NULL) {
        BackDrop_AlphaFn(&alpha);
    }
    if (!(alpha > 0.0f)) {
        return;
    }

    for (i32 special_index = 0; special_index < 2; ++special_index) {
        nuhspecial_s *special = &backdrop_hspecial[special_index];
        if (NuSpecialExistsFn(special) == 0) {
            continue;
        }
        // Retail realigns the frame (and esp,-16) for this matrix.
        NUMTX_ALIGNED16 mtx = *NuSpecialGetDrawMtx(special);
        NuSpecialDrawAtAlpha(special, &mtx, alpha);
        f32 x = mtx.m30;
        f32 y = mtx.m31;
        f32 z = mtx.m32;
        u16 angle = (u16)qrand();
        for (i32 i = 0; i < 3; ++i) {
            mtx.m30 = NU_SIN_LUT(angle) * 0.01f + x;
            mtx.m31 = NU_COS_LUT(angle) * 0.01f + y;
            mtx.m32 = z;
            NuSpecialDrawAtAlpha(special, &mtx, alpha);
            angle = (u16)(angle + 0x5555);
        }
    }
}

void FinishLoop(i32 frames) {
    if (!FinishLoop_On) {
        return;
    }

    if (finishloop_backdroponly != 0) {
        pNuCam->mtx = numtx_identity;
        NuCameraSet(pNuCam);

        do {
            NuFrameBegin();
            NuRndrBeginScene(-1);
            NuRndrClear(0xb00, 0, 1.0f);
            BackDrop_Draw(1.0f, 1);
            NuRndrEndScene();
            edGraEnableTerrainSwap();
            NuFrameEnd();
            edGraDisableTerrainSwap();
        } while (--frames > 0);
        finishloop_backdroponly = 0;
        return;
    }

    if (FadeSys.pending_type == FADE_TYPE_NONE) {
        do {
            NuFrameBegin();
            NuRndrBeginScene(-1);
            NuRndrClear(0xb00, 0, 1.0f);
            NuRndrEndScene();
            edGraEnableTerrainSwap();
            NuFrameEnd();
            edGraDisableTerrainSwap();
        } while (--frames > 0);
    } else {
        do {
            NuFrameBegin();
            FadeSys.Draw();
            edGraEnableTerrainSwap();
            NuFrameEnd();
            edGraDisableTerrainSwap();
        } while (--frames > 0);
    }
}

void FadeLoop(char *text, i32 direction, float duration, void (*draw)(float)) {
    f32 alpha = 0.0f;
    f32 target = 1.0f;
    if (direction != 0) {
        alpha = 1.0f;
        target = 0.0f;
    }
    f32 rate = 10.0f;
    if (duration != 0.0f) {
        rate = 1.0f / duration;
    }
    FRAMETIME = DEFAULTFRAMETIME;
    while (alpha != target) {
        NuFrameBegin();
        alpha = SeekLinearF(alpha, target, rate * FRAMETIME);
        NuRndrBeginScene(-1);
        NuRndrClear(0xb00, 0, 1.0f);
        if (FadeLoop_ObjScene != NULL) {
            FadeLoop_DrawObj(alpha);
        }
        if (text != NULL) {
            SetQFont2D();
            Text3D(text, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, static_cast<u8>(alpha * 0.0f),
                   static_cast<u8>(alpha * 191.0f), static_cast<u8>(alpha * 255.0f));
        }
        if (draw != NULL) {
            draw(alpha);
        }
        NuRndrEndScene();
        edGraEnableTerrainSwap();
        FRAMETIME = NuFrameEnd();
        edGraDisableTerrainSwap();
    }
    if (direction == 1) {
        FadeLoop_SetObj(NULL, NULL);
        FinishLoop(2);
    }
}

void NoRender() {
    pNuCam->mtx = numtx_identity;
    NuCameraSet(pNuCam);
    NuRndrBeginScene(-1);

    if (back_rgba[0] == back_rgba[1]) {
        NuRndrClear(0xf00, back_rgba[0], 1.0f);
    } else {
        NuRndrGradClear(0xf00, back_rgba[0], back_rgba[1], 1.0f);
    }

    if (MainRenderTime != 1.0f) {
        BackDrop_Draw(1.0f - MainRenderTime, 0);
    }
    NuRndrEndScene();
}

void BackDrop_Dump() {
    if (backdrop_scene != NULL) {
        NuGScnRemove(backdrop_scene);
    }
}

void DrawParallax(nuhspecial_s *special) {
    if (NuIOS_IsLowEndDevice() != 0 && WORLD != NULL && WORLD->current_level != NULL &&
        WORLD->current_level->data_display.far_clip < 20000.0f &&
        WORLD->current_level->data_display.fog_start < 20000.0f) {
        return;
    }

    const f32 uniform_scale = pNuCam->far_clip * 0.1f * 0.5f;
    NUVEC scale = {uniform_scale, uniform_scale, uniform_scale};
    NUMTX_ALIGNED16 matrix;
    NuMtxSetScale(&matrix, &scale);
    matrix.m30 = pNuCam->mtx.m30;
    matrix.m31 = pNuCam->mtx.m31;
    matrix.m32 = pNuCam->mtx.m32;
    NuRndrStateSetFogEnabled(0);
    NuSpecialDrawAt(special, &matrix);
    NuRndrStateSetFogEnabled(1);
}

void GameFog_Reset() {
    GameFogSnap = 1;
    GameFogDuration = 0.0f;
    GameFogSet = 0;
    GameFogTime = 0.0f;
}

void GameFog_Update(WORLDINFO_s *world) {
    rtlfog_s *previous = GameFogSet;
    if (world->rtl_set != NULL &&
        !(CUTSTOPGAME != 0 && CutStopInfo != NULL && (static_cast<CUTINFO *>(CutStopInfo)->flags & 4) != 0)) {
        GameFogSet = rtlGetFogSet(world->rtl_set, NUMTX_GET_ROW_VEC(&pNuCam->mtx, 3));
    } else {
        GameFogSet = NULL;
    }

    if (GameFogSnap == 0 && GameFogSet != previous) {
        GameFogTime = 0.0f;
        GameFogDuration = 3.0f;
        GameFogOld = GameFog;
    }
    if (GameFogSnap != 0 || GameFogSet != previous) {
        if (GameFogSet != NULL) {
            u32 colour = GameFogSet->colour;
            GameFogNew.high_quality_colour_channels[0] = colour >> 24;
            GameFogNew.high_quality_colour_channels[1] = (colour >> 16) & 0xff;
            GameFogNew.high_quality_colour_channels[2] = (colour >> 8) & 0xff;
            GameFogNew.high_quality_colour_channels[3] = colour & 0xff;
            GameFogNew.high_quality_start = GameFogSet->start;
            GameFogNew.high_quality_end = GameFogSet->end;
            GameFogNew.high_quality_density = GameFogSet->density;
            GameFogNew.low_quality_density = GameFogSet->low_quality_density;
            colour = GameFogSet->low_quality_colour;
            GameFogNew.low_quality_colour_channels[0] = colour >> 24;
            GameFogNew.low_quality_colour_channels[1] = (colour >> 16) & 0xff;
            GameFogNew.low_quality_colour_channels[2] = (colour >> 8) & 0xff;
            GameFogNew.low_quality_colour_channels[3] = colour & 0xff;
        } else {
            GameFogNew.high_quality_colour_channels[0] = GameFogNew.high_quality_colour_channels[1] =
                GameFogNew.high_quality_colour_channels[2] = GameFogNew.high_quality_colour_channels[3] = 0.0f;
            GameFogNew.high_quality_start = 5.0f;
            GameFogNew.high_quality_end = 20.0f;
            GameFogNew.high_quality_density = 0.0f;
            GameFogNew.low_quality_density = 0.0f;
            GameFogNew.low_quality_colour_channels[0] = GameFogNew.low_quality_colour_channels[1] =
                GameFogNew.low_quality_colour_channels[2] = GameFogNew.low_quality_colour_channels[3] = 0.0f;
        }
    }

    if (GameFogSet == NULL) {
        dof_fstop = 22.0f;
    } else {
        dof_fstop = static_cast<f32>(static_cast<u32>(GameFogSet->depth_of_field_fstop)) / 10.0f;
        if (dof_fstop == 0.0f)
            dof_fstop = 22.0f;
    }

    if (NuIOS_IsLowEndDevice() && world->current_level != NULL) {
        GameFogNew.low_quality_end = world->current_level->data_display.far_clip;
        GameFogNew.low_quality_start = world->current_level->data_display.fog_start;
    }
    if (GameFogSnap != 0) {
        GameFog = GameFogNew;
        GameFogDuration = 0.0f;
        GameFogTime = 0.0f;
    } else {
        if (GameFogTime < GameFogDuration) {
            GameFogTime += FRAMETIME;
            if (GameFogTime > GameFogDuration)
                GameFogTime = GameFogDuration;
        }
        if (GameFogTime >= GameFogDuration) {
            GameFog = GameFogNew;
        } else {
            f32 blend = 1.0f - (NU_SIN_LUT(GameFogTime / GameFogDuration * 32768.0f + 16384.0f) + 1.0f) * 0.5f;
            GameFog.high_quality_colour_channels[0] =
                GameFogOld.high_quality_colour_channels[0] +
                (GameFogNew.high_quality_colour_channels[0] - GameFogOld.high_quality_colour_channels[0]) * blend;
            GameFog.high_quality_colour_channels[1] =
                GameFogOld.high_quality_colour_channels[1] +
                (GameFogNew.high_quality_colour_channels[1] - GameFogOld.high_quality_colour_channels[1]) * blend;
            GameFog.high_quality_colour_channels[2] =
                GameFogOld.high_quality_colour_channels[2] +
                (GameFogNew.high_quality_colour_channels[2] - GameFogOld.high_quality_colour_channels[2]) * blend;
            GameFog.high_quality_colour_channels[3] =
                GameFogOld.high_quality_colour_channels[3] +
                (GameFogNew.high_quality_colour_channels[3] - GameFogOld.high_quality_colour_channels[3]) * blend;
            GameFog.high_quality_start =
                GameFogOld.high_quality_start + (GameFogNew.high_quality_start - GameFogOld.high_quality_start) * blend;
            GameFog.high_quality_end =
                GameFogOld.high_quality_end + (GameFogNew.high_quality_end - GameFogOld.high_quality_end) * blend;
            GameFog.high_quality_density = GameFogOld.high_quality_density +
                                           (GameFogNew.high_quality_density - GameFogOld.high_quality_density) * blend;
            GameFog.low_quality_density = GameFogOld.low_quality_density +
                                          (GameFogNew.low_quality_density - GameFogOld.low_quality_density) * blend;
            GameFog.low_quality_colour_channels[0] =
                GameFogOld.low_quality_colour_channels[0] +
                (GameFogNew.low_quality_colour_channels[0] - GameFogOld.low_quality_colour_channels[0]) * blend;
            GameFog.low_quality_colour_channels[1] =
                GameFogOld.low_quality_colour_channels[1] +
                (GameFogNew.low_quality_colour_channels[1] - GameFogOld.low_quality_colour_channels[1]) * blend;
            GameFog.low_quality_colour_channels[2] =
                GameFogOld.low_quality_colour_channels[2] +
                (GameFogNew.low_quality_colour_channels[2] - GameFogOld.low_quality_colour_channels[2]) * blend;
            GameFog.low_quality_colour_channels[3] =
                GameFogOld.low_quality_colour_channels[3] +
                (GameFogNew.low_quality_colour_channels[3] - GameFogOld.low_quality_colour_channels[3]) * blend;
        }
    }
    GameFog.colour = (static_cast<i32>(GameFog.high_quality_colour_channels[0]) << 24) +
                     (static_cast<i32>(GameFog.high_quality_colour_channels[1]) << 16) +
                     (static_cast<i32>(GameFog.high_quality_colour_channels[2]) << 8) +
                     static_cast<i32>(GameFog.high_quality_colour_channels[3]);
    GameFog.low_quality_colour = (static_cast<i32>(GameFog.low_quality_colour_channels[0]) << 24) +
                                 (static_cast<i32>(GameFog.low_quality_colour_channels[1]) << 16) +
                                 (static_cast<i32>(GameFog.low_quality_colour_channels[2]) << 8) +
                                 static_cast<i32>(GameFog.low_quality_colour_channels[3]);
    GameFogSnap = 0;
    g_BackgroundColour = GameFog.colour;
}

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours) {
    CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && cut->camera_near_clip != 0.0f) {
        pNuCam->near_clip = cut->camera_near_clip;
    } else {
        pNuCam->near_clip = display->unknown_04;
    }

    u16 far_clip;
    if (cut != NULL) {
        far_clip = cut->camera_far_clip;
        if (far_clip == 0) {
            far_clip = static_cast<u16>(display->unknown_14);
        }
    } else {
        far_clip = static_cast<u16>(display->unknown_14);
    }
    pNuCam->far_clip = static_cast<f32>(static_cast<u32>(far_clip));

    LEVELDATA *level = WORLD->current_level;
    const bool use_backdrop = level == TITLES_LDATA || (level->flags & LEVEL_STATUS) != 0 || level == STATUS_LDATA ||
                              level == CREDITS_LDATA || MainRenderTime < 1.0f;
    if (!use_backdrop) {
        background_colours[0] =
            0x80000000u | static_cast<u8>(display->bg_red_top) |
            (static_cast<u8>(display->bg_green_top) << 8 | static_cast<u8>(display->bg_blue_top) << 16);
        background_colours[1] =
            0x80000000u | static_cast<u8>(display->bg_red_bottom) |
            (static_cast<u8>(display->bg_green_bottom) << 8 | static_cast<u8>(display->bg_blue_bottom) << 16);
        return;
    }

    BackDrop_UpdateColours(0);

    i32 top_b = static_cast<i32>(backdrop_top_b);
    i32 top_g = static_cast<i32>(backdrop_top_g);
    i32 top_r = static_cast<i32>(backdrop_top_r);
    i32 bottom_r = static_cast<i32>(backdrop_bot_r);
    i32 bottom_b = static_cast<i32>(backdrop_bot_b);
    i32 bottom_g = static_cast<i32>(backdrop_bot_g);

    f32 alpha = 1.0f;
    if (BackDrop_AlphaFn != NULL) {
        BackDrop_AlphaFn(&alpha);
        if (alpha != 1.0f) {
            top_r = static_cast<i32>(static_cast<f32>(top_r) * alpha);
            top_b = static_cast<i32>(static_cast<f32>(top_b) * alpha);
            top_g = static_cast<i32>(static_cast<f32>(top_g) * alpha);
            bottom_r = static_cast<i32>(static_cast<f32>(bottom_r) * alpha);
            bottom_g = static_cast<i32>(static_cast<f32>(bottom_g) * alpha);
            bottom_b = static_cast<i32>(static_cast<f32>(bottom_b) * alpha);
        }
    }

    background_colours[0] = 0x80000000u | (top_r & 0xff) | (top_g & 0xff) << 8 | (top_b & 0xff) << 16;
    background_colours[1] = 0x80000000u | (bottom_r & 0xff) | (bottom_g & 0xff) << 8 | (bottom_b & 0xff) << 16;
}

void SetCameraZoom(f32 zoom) {
    CameraZoom = zoom;
}

void WidescreenCode(i32) {
    pNuCam->aspect = 1.0f / NuIOS_GetAspectRatio();
    pNuCam->fov = (1.0f / NuIOS_GetAspectRatio() + 0.75f) * 0.5f * (1.0f / CameraZoom);
    SmartTextSetWidescreen(1.3333334f / NuIOS_GetAspectRatio(), 1.0f);
}

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity) {
    if (position->x < -0.85f) {
        position->x = -0.85f;
        if (velocity != NULL)
            velocity->x = 0.0f;
    } else if (position->x > 0.85f) {
        position->x = 0.85f;
        if (velocity != NULL)
            velocity->x = 0.0f;
    }
    if (position->y < -0.85f) {
        position->y = -0.85f;
        if (velocity != NULL)
            velocity->y = 0.0f;
    } else if (position->y > 0.85f) {
        position->y = 0.85f;
        if (velocity != NULL)
            velocity->y = 0.0f;
    }
}

void UpdateCutBorders() {
    f32 target_scale = 1.0f;

    if (CUTSTOPGAME == 0 && (LEGOCAMMODE_DOORCUT == -1 || GameCam->mode != LEGOCAMMODE_DOORCUT)) {
        if (LEGOCAMMODE_OBSTACLE == -1 || GameCam->mode != LEGOCAMMODE_OBSTACLE || ObstacleCamBorders == 0) {
            target_scale = 0.0f;
        }
    }

    CutBorderScale = SeekLinearF(CutBorderScale, target_scale, FRAMETIME * 2.0f);
}

void DrawCutBorders(i32 widescreen) {
    if (!(CutBorderScale > 0.0f)) {
        return;
    }

    NuRndrBeginScene(-1);
    f32 border_scale = 0.1f;
    const CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && (cut->flags & 0x4000) != 0) {
        border_scale = 0.21276596f;
    } else {
        const CUTSYS *system = WORLD->cutscene_sys;
        if (system != NULL && system->count == 1 && (system->cuts[0]->flags & 0x4000) != 0) {
            border_scale = 0.21276596f;
        }
    }
    if (widescreen != 0) {
        border_scale = (border_scale * 4.0f - 0.5f) / 3.0f;
    }
    if (border_scale < 0.02f) {
        border_scale = 0.02f;
    }
    const i32 height =
        static_cast<i32>(border_scale * 3584.0f * NU_SIN_LUT(static_cast<i32>(CutBorderScale * 16384.0f)));
    NuRndrRect2di(0, 0, 0x2800, height, 0x80ffffff, FadeMtl);
    NuRndrRect2di(0, 0xe00 - height, 0x2800, height, 0x80ffffff, FadeMtl);
    NuRndrEndScene();
}

void InitStillRender(variptr_u *, variptr_u) {
    static NUNATIVETEX nativePauseTex;

    pause_rt = NuTexGenTexture(&nativePauseTex);
    nativePauseTex.ref_count = 1;
    // Retail writes each checksum byte (48c587..48c60c); image_data and size retain their initialized state.
    nativePauseTex.checksum[0] = 0;
    nativePauseTex.checksum[1] = 0;
    nativePauseTex.checksum[2] = 0;
    nativePauseTex.checksum[3] = 0;
    nativePauseTex.checksum[4] = 0;
    nativePauseTex.checksum[5] = 0;
    nativePauseTex.checksum[6] = 0;
    nativePauseTex.checksum[7] = 0;
    nativePauseTex.checksum[8] = 0;
    nativePauseTex.checksum[9] = 0;
    nativePauseTex.checksum[10] = 0;
    nativePauseTex.checksum[11] = 0;
    nativePauseTex.checksum[12] = 0;
    nativePauseTex.checksum[13] = 0;
    nativePauseTex.checksum[14] = 0;
    nativePauseTex.checksum[15] = 0;
    nativePauseTex.height = g_backingHeight;
    nativePauseTex.width = g_backingWidth;

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/legoapi.saga/screen.cpp", 0x561);
    glGenTextures(1, &nativePauseTex.platform.gl_tex);
    glActiveTexture(GL_TEXTURE0);
    g_currentTexUnit = 0;
    glBindTexture(GL_TEXTURE_2D, nativePauseTex.platform.gl_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_backingWidth, g_backingHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/legoapi.saga/screen.cpp", 0x56d);

    pause_rndr_mtl = NuMtlCreate3D(1);
    pause_rndr_mtl->sort_pri = 0x7fff;
    pause_rndr_mtl->shader_desc.flags = 0x1000;
    pause_rndr_mtl->opacity = 1.0f;
    pause_rndr_mtl->attribs.unknown_1_1_2 = 1;
    pause_rndr_mtl->attribs.unknown_1_4_8 = 1;
    pause_rndr_mtl->attribs.z_mode = 3;
    pause_rndr_mtl->attribs.unknown_2_1_2 = 2;
    pause_rndr_mtl->attribs.alpha_mode = 1;
    pause_rndr_mtl->attribs.unknown_2_4 = 1;
    pause_rndr_mtl->attribs.filter_mode = 1;
    pause_rndr_mtl->tex_id = static_cast<i16>(pause_rt);
    pause_rndr_mtl->attribs.cull_mode = 2;
    pause_rndr_mtl->shader_desc.diffuse_color[0] = -1;
    pause_rndr_mtl->shader_desc.unknown_a8 = 1;
    pause_rndr_mtl->shader_desc.vtx_desc.tex_coord_mode = 1;
    pause_rndr_mtl->shader_desc.vtx_desc.has_no_transform = 1;
    NuMtlUpdate(pause_rndr_mtl);
    pause_rndr_on = 0;
}

void ResizePauseScreenTexture(i32, i32) {
    STUBBED();
}

void DrawPauseScreenWipe() {
    NuRndrBeginScene(-1);

    i32 x = 0;
    i32 y = 0;
    i32 width = 0x2800;
    i32 height = 0xe00;
    f32 u0 = 0.0f;
    f32 top = 0.0f;
    f32 bottom = 1.0f;
    f32 u1 = 1.0f;
    u32 colours[4];

    if ((FadeSys.direction & 3) != 0) {
        if ((FadeSys.direction & 1) != 0) {
            const f32 fade = FadeSys.fade;
            u0 = 1.0f - fade;
            x = static_cast<i32>(u0 * 10240.0f);
            width = 0x2800 - x;
            colours[0] = 0x00808080u;
            colours[1] = 0x80808080u;
            colours[2] = 0x00808080u;
            colours[3] = 0x80808080u;
            NuRndrGradRectUV2di(x - 0x400, 0, 0x400, 0xe00, u0 - 0.1f, 1.0f, u0, 0.0f, colours, pause_rndr_mtl);
        } else {
            const f32 fade = FadeSys.fade;
            width = static_cast<i32>(fade * 10240.0f);
            colours[0] = 0x80808080u;
            colours[1] = 0x00808080u;
            colours[2] = 0x80808080u;
            colours[3] = 0x00808080u;
            NuRndrGradRectUV2di(width, 0, 0x400, 0xe00, fade, 1.0f, fade + 0.1f, 0.0f, colours, pause_rndr_mtl);
            u1 = fade;
        }
    } else if ((FadeSys.direction & 0xc) != 0) {
        if ((FadeSys.direction & 4) != 0) {
            const f32 fade = FadeSys.fade;
            const f32 edge = 1.0f - fade;
            y = static_cast<i32>(edge * 3584.0f);
            height = 0xe00 - y;
            colours[0] = 0x00808080u;
            colours[1] = 0x00808080u;
            colours[2] = 0x80808080u;
            colours[3] = 0x80808080u;
            NuRndrGradRectUV2di(0, y - 0x166, 0x2800, 0x166, 0.0f, 1.0f - (edge - 0.1f), 1.0f, 1.0f - edge, colours,
                                pause_rndr_mtl);
            top = edge;
        } else {
            const f32 fade = FadeSys.fade;
            height = static_cast<i32>(fade * 3584.0f);
            colours[0] = 0x80808080u;
            colours[1] = 0x80808080u;
            colours[2] = 0x00808080u;
            colours[3] = 0x00808080u;
            NuRndrGradRectUV2di(0, height, 0x2800, 0x166, 0.0f, 1.0f - fade, 1.0f, 1.0f - (fade + 0.1f), colours,
                                pause_rndr_mtl);
            bottom = fade;
        }
    }

    NuRndrRectUV2di(x, y, width, height, u0, 1.0f - top, u1, 1.0f - bottom, 0x80808080u, pause_rndr_mtl);
    NuRndrEndScene();
}

void StillMemRestore() {
    // No operation in the Android build.
}

void GrabStillScreen() {
    if (ScreenGrabNeeded != 0) {
        ScreenGrabNeeded = 0;
        NuRndrBeginScene(-1);
        NuBackbufferCopy(pause_rt);
        NuRndrEndScene();
        pause_fade = 0;
    }
}

void NeedScreenGrab(i32 needed) {
    ScreenGrabNeeded = needed != 0;
}

i8 IsGrabbingScreen() {
    return ScreenGrabNeeded;
}

void DrawStillScreen(i32 clear) {
    NuRndrBeginScene(-1);
    NuVpGetCurrentViewport();
    if (clear != 0) {
        NuRndrClear(0x500, 0, 1.0f);
    }
    if (MainRenderTime < 1.0f) {
        const u32 colour = (static_cast<i32>(MainRenderTime * 128.0f) << 24) | 0x00808080u;
        u32 colours[4] = {colour, colour, colour, colour};
        NuRndrGradRectUV2di(0, 0, 0x2800, 0xe00, 0.0f, 1.0f, 1.0f, 0.0f, colours, pause_rndr_mtl);
    } else {
        NuRndrRectUV2di(0, 0, 0x2800, 0xe00, 0.0f, 1.0f, 1.0f, 0.0f, 0x80808080u, pause_rndr_mtl);
    }
    NuRndrEndScene();
}

void DrawPauseFade() {
    if (editor_active != 0 || screendump != 0)
        return;

    f32 current = pause_fade;
    if (Paused != 0 || NetPaused != 0) {
        i32 fade = static_cast<i32>(current + FRAMETIME * 2.0f);
        if (fade > 0) {
            pause_fade = 1.0f;
        } else {
            pause_fade = static_cast<f32>(fade);
        }
    } else {
        i32 fade = static_cast<i32>(current - FRAMETIME * 2.0f);
        if (fade < 0)
            fade = 0;
        pause_fade = static_cast<f32>(fade);
    }
}

void ClearStill() {
    old_pause_state = 0;
    Paused = 0;
}

void HandleStillRender() {
    GetMenuID();
    if (__builtin_expect(pause_rndr_on != 0 && FadeSys.pending_type == FADE_TYPE_NONE, 1)) {
        NuRndrBeginScene(-1);
        if (MainRenderTime < 1.0f) {
            if (__builtin_expect(back_rgba[0] == back_rgba[1], 0)) {
                NuRndrClear(0xf00, back_rgba[0], 1.0f);
            } else {
                NuRndrGradClear(0xf00, back_rgba[0], back_rgba[1], 1.0f);
            }
            BackDrop_Draw(1.0f - MainRenderTime, 0);
        } else {
            NuRndrClear(0x300, 0, 1.0f);
        }
        NuRndrEndScene();
    }

    if (__builtin_expect(grab_screen_image != 2 && pause_rndr_on != 0 && pause_rt != 0 && FadeSys.pending_type == FADE_TYPE_NONE, 0)) {
        DrawStillScreen(clear_screen_onstill);
    }

    if (Paused != 0) {
        if (old_pause_state == 0 && FadeSys.pending_type == FADE_TYPE_NONE) {
            NeedScreenGrab(1);
        }
    }
    if (Paused == 0 && old_pause_state != 0) {
        pause_rndr_on = 0;
    }
    old_pause_state = Paused;

    if (!((waiting_for_level == -1 ||
           (gone_through_door_to_new_level == 0 && cut_waiting_for_new_level == 0 && waiting_for_new_level == 0)) &&
          (NewLData != CREDITS_LDATA || LastLData != STATUS_LDATA) &&
          (NewLData != STATUS_LDATA ||
           ((WORLD->level_sub_id == -1 || (WORLD->area[WORLD->level_sub_id].flags & 2) == 0) && GAMEDEMO == 0)) &&
          NewLData != CREDITS_LDATA &&
          (NewLData != HUB_LDATA || WORLD->level_sub_id == -1 || (WORLD->area[WORLD->level_sub_id].flags & 2) == 0) &&
          !(MainRenderTime > 0.0f && MainRenderTime < 1.0f))) {
        pause_rndr_on = 1;
    } else {
        if (Paused == 0 || IsGrabbingScreen() != 0) {
            pause_rndr_on = static_cast<u32>(grab_screen_image) > 1;
        } else if (PauseRenderOffFn != NULL && PauseRenderOffFn() != 0) {
            pause_rndr_on = 0;
        } else {
            pause_rndr_on = 1;
        }
    }

    if (screendump != 0) {
        pause_rndr_on = 0;
    }
    clear_screen_onstill = 1;
    grab_screen_image = 0;
}

void DrawFadeScreenWipe() {
    NuRndrBeginScene(-1);
    extern numtl_s *SolidMtl;

    FadeSystem &fade_info = FadeSys;
    const f32 fade_amount = fade_info.fade;
    const u32 direction = fade_info.direction;
    i32 gradient[4];
    i32 solid_x = 0;
    i32 solid_y = 0;
    i32 solid_width = 10240;
    i32 solid_height = 3584;

    if ((direction & 3) != 0) {
        i32 gradient_x;
        const bool positive = (direction & 1) != 0;
        if ((positive && fade_info.rate > 0.0f) || (!positive && !(fade_info.rate > 0.0f))) {
            gradient[0] = static_cast<i32>(0x80000000u);
            gradient[1] = 0;
            gradient[2] = static_cast<i32>(0x80000000u);
            gradient[3] = 0;
            solid_width = static_cast<i32>(fade_amount * 10240.0f);
            gradient_x = solid_width;
        } else {
            gradient[0] = 0;
            gradient[1] = static_cast<i32>(0x80000000u);
            gradient[2] = 0;
            gradient[3] = static_cast<i32>(0x80000000u);
            const f32 edge = (1.0f - fade_amount) * 10240.0f;
            solid_x = static_cast<i32>(edge);
            solid_width = 10240 - solid_x;
            gradient_x = static_cast<i32>(edge - 1024.0f);
        }
        NuRndrGradRect2di(gradient_x, 0, 1024, 3584, gradient, FadeMtl2);
    } else if ((direction & 0xc) != 0) {
        // The vertical sign test is the same two-way rate/direction test as
        // the horizontal one, with bit 2 selecting the opposite side.
        i32 gradient_y;
        const bool positive = (direction & 4) == 0;
        if ((positive && fade_info.rate > 0.0f) || (!positive && !(fade_info.rate > 0.0f))) {
            gradient[0] = 0;
            gradient[1] = 0;
            gradient[2] = static_cast<i32>(0x80000000u);
            gradient[3] = static_cast<i32>(0x80000000u);
            const f32 edge = (1.0f - fade_amount) * 3584.0f;
            solid_y = static_cast<i32>(edge);
            solid_height = 3584 - solid_y;
            gradient_y = static_cast<i32>(edge - 358.0f);
        } else {
            gradient[0] = static_cast<i32>(0x80000000u);
            gradient[1] = static_cast<i32>(0x80000000u);
            gradient[2] = 0;
            gradient[3] = 0;
            solid_height = static_cast<i32>(fade_amount * 3584.0f);
            gradient_y = solid_height;
        }
        NuRndrGradRect2di(0, gradient_y, 10240, 358, gradient, FadeMtl2);
    }

    if (!(fade_amount < 0.0f)) {
        NuRndrRect2di(solid_x, solid_y, solid_width, solid_height, 0, SolidMtl);
    }
    NuRndrEndScene();
}

