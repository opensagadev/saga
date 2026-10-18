// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "nu2api/nucore/NuVirtualTouchDevice.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/NuTouchInputStick.h"
#include "nu2api/nucore/NuTouchInputButton.h"
#include <string.h>

#include "nu2api/nucore/nuvuvec.hpp"

#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuTouchInputElement.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/numtx.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/props/doors/door.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/core/input/gamepads.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nu3d/nuportal.h"
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
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "gameapi/edtools/edanim_internal.h"
#include <cstdio>
#include "nu2api/nu3d/nuspline.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "editor/edpath.h"
#include "nu2api/nucore/nulist.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/characters/motion/action_info.h"
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
#undef NUGCUT_CURVE_VALUE

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
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

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern i32 LevFlag[4];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

struct EDCREATURE_s;

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

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

NuVirtualTouchDevice::NuVirtualTouchDevice(u32 unknown) {
    u32 i;

    this->unknown_08.unknown_c8 = 0;

    for (i = 0; i < 10; i++) {
        this->unknown_d4[i].unknown_c8 = 0;
    }

    this->unknown_04 = 0;
}

f32 NuVirtualTouchDevice::GetAspectRatio() {
    return (f32)g_backingHeight / (f32)g_backingWidth;
}

void NuVirtualTouchDevice::CreateDefaultLayout(u32 unknown) {
    STUBBED();
}

void NuButtonLayout::UpdateButtons(i32 index) {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->UpdateButtons(index);
}

void NuButtonLayout::Update(NuInputTouchData const *data) {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Update(data);
}

void NuVirtualTouchDevice::Execute(u32 port, NUPADTYPE in_type, NUPADATTACHMENTTYPE in_attch_type, u32 in_caps,
                                   u32 in_buttons, const float *in_analog, const float *in_motion,
                                   const NuInputTouchData *in_touch_data, const NuInputMouseData *in_mouse_data,
                                   NUPADTYPE &out_pad_type, NUPADATTACHMENTTYPE &out_attch_type, u32 &out_caps,
                                   u32 &out_buttons, float *out_analog, float *out_motion,
                                   NuInputTouchData *out_touch_data, NuInputMouseData *out_mouse_data) {
    unknown_08.UpdateButtons(in_buttons);
    unknown_08.Update(in_touch_data);
    if (unknown_04 < 10)
        unknown_d4[unknown_04].Update(in_touch_data);

    out_pad_type = NUPADTYPE_GAMEPAD;
    out_attch_type = NUPADATTACHMENTTYPE_NONE;
    out_caps = 0x418;
    out_buttons = 0;
    memset(out_analog, 0, 12 * sizeof(float));
    for (u32 i = 0; i < CurrentLayout()->unknown_c8; ++i) {
        if (CurrentLayout()->elements[i]->type == NuTouchInputElement::TYPE_LEFT_STICK) {
            out_analog[8] = CurrentLayout()->elements[i]->GetStickX();
            out_analog[9] = CurrentLayout()->elements[i]->GetStickY();
        } else if (CurrentLayout()->elements[i]->type == NuTouchInputElement::TYPE_RIGHT_STICK) {
            out_analog[10] = CurrentLayout()->elements[i]->GetStickX();
            out_analog[11] = CurrentLayout()->elements[i]->GetStickY();
        } else {
            static i32 test;
            if (CurrentLayout()->elements[i]->IsPressed()) {
                ++test;
                out_buttons |= CurrentLayout()->elements[i]->index;
            } else {
                test = 0;
            }
        }
    }
    memcpy(out_touch_data, in_touch_data, sizeof(NuInputTouchData));
}

void NuButtonLayout::Render() {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Render();
}

void NuVirtualTouchDevice::Render() {
    if (unknown_04 < 10)
        unknown_d4[unknown_04].Render();
}

void NuButtonLayout::DeactivateLayout() {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Deactivate();
}

void NuButtonLayout::ActivateLayout() {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Activate();
}

void NuVirtualTouchDevice::SetCurrentLayoutIndex(u32 index) {
    if (unknown_04 < 10)
        unknown_d4[unknown_04].DeactivateLayout();
    unknown_04 = index;
    if (unknown_04 < 10)
        unknown_d4[unknown_04].ActivateLayout();
}

NuButtonLayout::~NuButtonLayout() {
    for (u32 i = 0; i < unknown_c8; ++i)
        delete elements[i];
}

NuTouchInputElement::NuTouchInputElement(NuTouchInputElement::TYPE type, i32 id, u32 index) {
    this->id = id;
    this->index = index;
    this->type = type;
}

NuTouchInputButton::NuTouchInputButton(i32 id, u32 index) : NuTouchInputElement(TYPE_BUTTON, id, index) {
    pressed = false;
}

NuTouchInputElement::NuTouchInputElement(NuTouchInputElement::TYPE type, i32 id, u32 index, float x, float y,
                                         float width, float height) {
    this->id = id;
    this->index = index;
    this->type = type;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

NuTouchInputStick::NuTouchInputStick(NuTouchInputElement::TYPE type, i32 id, u32 index, float x, float y, float width,
                                     float height)
    : NuTouchInputElement(type, id, index, x, y, width, height) {
    unknown_3c = false;
    stick_x = 0.0f;
    stick_y = 0.0f;
}

NuTouchInputButton::NuTouchInputButton(i32 id, u32 index, float x, float y, float width, float height)
    : NuTouchInputElement(TYPE_BUTTON, id, index, x, y, width, height) {
    pressed = false;
}

