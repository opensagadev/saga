// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/NuTouchInputButton.h"
#include "nu2api/nucore/NuTouchInputElement.h"
#include "nu2api/nucore/NuTouchInputStick.h"
#include "nu2api/nucore/NuVirtualTouchDevice.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"

#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/numtx.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/props/doors/door.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "batman.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/gamehint.h"
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
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numusic/sfx.h"
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
#include "nu2api/nucore/nupad_interface.h"

NuInputManager *inputManager;
NuVirtualTouchDevice *inputTouchDevice;
bool used_touch_IDs[10];
i32 white = 0x32ffffff;
i32 grey = 0x32646464;

i32 NuInputDevicePS::GetIdentifierPS(u32) {
    STUBBED();
    return 0;
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
#undef NUGCUT_CURVE_VALUE

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

    static inline void NuPadCopyTouchFields(NuInputTouch *destination, const NuInputTouch *source) {
        destination->unknown_04 = source->unknown_04;
        destination->unknown_00 = source->unknown_00;
        destination->unknown_08 = source->unknown_08;
        destination->unknown_01 = source->unknown_01;
        destination->unknown_0c = source->unknown_0c;
        destination->unknown_02 = source->unknown_02;
        destination->unknown_10 = source->unknown_10;
        destination->unknown_14 = source->unknown_14;
    }
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

void NuPad_Interface_InputManagerInitialise(void) {
    NuInputDevice *touch_dev;
    u32 i;

    inputManager = new NuInputManager();
    inputManager->UpdateAll(0.0f);

    touch_dev = inputManager->GetFirstDeviceByType(NUPADTYPE_TOUCH);
    if (touch_dev != NULL) {
        inputTouchDevice = new NuVirtualTouchDevice(4);
        inputTouchDevice->CreateDefaultLayout(0);

        touch_dev->AddTranslator(inputTouchDevice);
    }

    for (i = 0; i < 10; i++) {
        used_touch_IDs[i] = false;
    }
}

void NuPad_Interface_InputManagerUpdate(f32 delta_time) {
    inputManager->UpdateAll(delta_time);
}

u32 NuPad_Interface_NuPadRead(i32 port, u8 *analog_left_x, u8 *analog_left_y, u8 *analog_right_x, u8 *analog_right_y,
                              u8 *analog_l1, u8 *analog_l2, u8 *analog_r1, u8 *analog_r2, u32 *digital_buttons,
                              u8 *unknown, i32 *analog_button_flags) {
    NuInputDevice *device;

    device = inputManager->GetDevice(port);

    if (device == NULL) {
        return 0;
    }

    *analog_left_x = (device->GetAnalogValue(NUPADANALOGVALUE_LEFT_X) + 1.0f) * 127.0f;
    *analog_left_y = (device->GetAnalogValue(NUPADANALOGVALUE_LEFT_Y) + 1.0f) * 127.0f;
    *analog_right_x = (device->GetAnalogValue(NUPADANALOGVALUE_RIGHT_X) + 1.0f) * 127.0f;
    *analog_right_y = (device->GetAnalogValue(NUPADANALOGVALUE_RIGHT_Y) + 1.0f) * 127.0f;

    *analog_l1 = device->GetAnalogValue(NUPADANALOGVALUE_L1) * 127.0f;
    *analog_l2 = device->GetAnalogValue(NUPADANALOGVALUE_L2) * 127.0f;
    *analog_r1 = device->GetAnalogValue(NUPADANALOGVALUE_R1) * 127.0f;
    *analog_r2 = device->GetAnalogValue(NUPADANALOGVALUE_R2) * 127.0f;

    *digital_buttons = device->GetButtons();

    return device->IsConnected();
}

void NuPad_Interface_Render(void) {
    NuRndrBeginScene(-1);
    if (inputTouchDevice != NULL)
        inputTouchDevice->Render();
    NuRndrEndScene();
}

i32 NuPad_Interface_GetMaxDevices(void) {
    return inputManager->GetMaxDevices();
}

void NuPad_UpdateTouchScreenData(void) {
    NuInputDevice *device = inputManager->GetDevice(0);
    if (device != NULL) {
        NuInputDevicePS::ReadTouchDataPS(0, &device->touch_data);
    }
}

extern "C" {
    void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled) {
        NuInputDevice *device = inputManager->GetDevice(0);
        if (device == NULL) {
            return;
        }

        // The retail routine writes directly to the touch data at device+0x198.
        NuInputTouchData *data = reinterpret_cast<NuInputTouchData *>(reinterpret_cast<u8 *>(device) + 0x198);
        f32 width = static_cast<f32>(g_backingWidth);
        f32 height = static_cast<f32>(g_backingHeight);
        f32 old_x = static_cast<f32>(prior_x) / width;
        f32 old_y = static_cast<f32>(prior_y) / height;

        if (is_cancelled != 0) {
            f32 aspect = width / height;
            i32 count = data->touch_count;
            f32 nearest_distance = FLT_MAX;
            i32 nearest;
            for (i32 i = 0; i < count; ++i) {
                f32 dx = (data->touch_events[i].unknown_04 - old_x) * aspect;
                f32 dy = data->touch_events[i].unknown_08 - old_y;
                f32 distance = dx * dx + dy * dy;
                if (distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest = i;
                }
            }
            if (nearest_distance > 0.04f) {
                memset(data->touch_events, 0, sizeof(data->touch_events));
                data->touch_count = 0;
                memset(used_touch_IDs, 0, sizeof(used_touch_IDs));
                return;
            }
            used_touch_IDs[data->touch_events[nearest].unknown_14] = false;
            NuInputTouch *source = &data->touch_events[nearest + 1];
            for (i32 i = nearest + 1; i < count; ++i) {
                NuPadCopyTouchFields(source - 1, source);
                ++source;
            }
            NuInputTouch *last = &data->touch_events[count - 1];
            last->unknown_04 = 0.0f;
            last->unknown_08 = 0.0f;
            last->unknown_0c = 0.0f;
            last->unknown_10 = 0.0f;
            last->unknown_00 = 0;
            last->unknown_01 = 0;
            last->unknown_02 = 0;
            last->unknown_14 = 0;
            data->touch_count = count - 1;
            return;
        }

        f32 new_x = static_cast<f32>(x) / width;
        f32 new_y = static_cast<f32>(y) / height;
        if (is_move != 0) {
            i32 free_id = 0;
            while (free_id < 10 && used_touch_IDs[free_id]) {
                ++free_id;
            }
            for (i32 i = 0; i < 10; ++i) {
                if (!data->touch_events[i].unknown_00 && !data->touch_events[i].unknown_01 &&
                    !data->touch_events[i].unknown_02) {
                    NuInputTouch *touch = &data->touch_events[i];
                    touch->unknown_04 = new_x;
                    touch->unknown_00 = 0;
                    touch->unknown_08 = new_y;
                    touch->unknown_01 = 0;
                    touch->unknown_0c = 0.0f;
                    touch->unknown_10 = 0.0f;
                    touch->unknown_02 = 1;
                    touch->unknown_14 = free_id;
                    used_touch_IDs[free_id] = true;
                    ++data->touch_count;
                    return;
                }
            }
            return;
        }

        if (is_up != 0) {
            i32 selected = 10;
            for (i32 i = 0; i < 10; ++i) {
                NuInputTouch *touch = &data->touch_events[i];
                if ((touch->unknown_02 || touch->unknown_00) &&
                    ((touch->unknown_04 == old_x && touch->unknown_08 == old_y) ||
                     (touch->unknown_0c == old_x && touch->unknown_10 == old_y))) {
                    selected = i;
                    break;
                }
            }
            used_touch_IDs[data->touch_events[selected].unknown_14] = false;
            NuInputTouch *source = &data->touch_events[selected + 1];
            for (i32 i = selected + 1; i < static_cast<i32>(data->touch_count); ++i) {
                NuPadCopyTouchFields(source - 1, source);
                ++source;
            }
            NuInputTouch *last = &data->touch_events[--data->touch_count];
            last->unknown_01 = 0;
            last->unknown_00 = 0;
            last->unknown_02 = 0;
            last->unknown_04 = 0.0f;
            last->unknown_08 = 0.0f;
            last->unknown_0c = 0.0f;
            last->unknown_10 = 0.0f;
            last->unknown_14 = 0;
            return;
        }

        if (is_down != 0) {
            i32 selected = 10;
            for (i32 i = 0; i < 10; ++i) {
                NuInputTouch *touch = &data->touch_events[i];
                if ((touch->unknown_02 || touch->unknown_00) && touch->unknown_04 == old_x &&
                    touch->unknown_08 == old_y) {
                    selected = i;
                    break;
                }
            }
            NuInputTouch *touch = &data->touch_events[selected];
            touch->unknown_00 = 1;
            touch->unknown_01 = 0;
            touch->unknown_02 = 0;
            touch->unknown_04 = new_x;
            touch->unknown_08 = new_y;
            touch->unknown_0c = old_x;
            touch->unknown_10 = old_y;
        }
    }

extern "C" void NuPad_Interface_ResetAllTouches(void) {
    NuInputDevice *device = inputManager->GetDevice(0);

    memset(device->touch_data.touch_events, 0, sizeof(device->touch_data.touch_events));
    device->touch_data.touch_count = 0;
    memset(used_touch_IDs, 0, sizeof(used_touch_IDs));
}

}

void NuTouchInputButton::Update(NuInputTouchData const *data) {
    bool found = false;
    u32 count = data->touch_count;
    pressed = false;
    for (u32 i = 0; i < count; ++i) {
        u8 active = data->touch_events[i].unknown_00;
        u8 released = data->touch_events[i].unknown_01;
        u8 started = data->touch_events[i].unknown_02;
        float ty = data->touch_events[i].unknown_08;
        float tx = data->touch_events[i].unknown_04;
        u32 touch_id = data->touch_events[i].unknown_14;
        if (!released && !started && !active)
            continue;
        float dy = ty - y;
        float ry = height * 0.5f;
        float rx = width * 0.5f;
        float dx = tx - x;
        if (!touch_captured) {
            if (started) {
                if (dx * dx / (rx * rx) + dy * dy / (ry * ry) <= 1.0f) {
                    captured_touch_id = touch_id;
                    touch_captured = true;
                    pressed = true;
                    found = true;
                }
            }
        } else {
            if (__builtin_expect(touch_id == captured_touch_id, 0)) {
                pressed = true;
                found = true;
            }
        }
        if (released && captured_touch_id == touch_id)
            touch_captured = false;
    }
    if (!found)
        touch_captured = false;
}

void NuTouchInputStick::Update(NuInputTouchData const *data) {
    stick_x = 0.0f;
    stick_y = 0.0f;
    u32 count = data->touch_count;
    const NuInputTouch *touch_ptr = data->touch_events;
    for (; count != 0; --count, ++touch_ptr) {
        const NuInputTouch &touch = *touch_ptr;
        u8 active = touch.unknown_00;
        u8 started = touch.unknown_02;
        float tx = touch.unknown_04;
        float ty = touch.unknown_08;
        u32 touch_id = touch.unknown_14;
        if (!touch.unknown_01 && !started && !active)
            continue;
        float dx = tx - x;
        float dy = ty - y;
        float dx2 = dx * dx;
        float dy2 = dy * dy;
        float rx = width * 0.5f;
        float ry = height * 0.5f;
        if (!unknown_3c && started) {
            if (dx2 / (rx * rx) + dy2 / (ry * ry) <= 1.0f) {
                unknown_38 = touch_id;
                unknown_3c = true;
            }
        }
        if (touch.unknown_01 && unknown_38 == touch_id) {
            unknown_3c = false;
        } else if (unknown_3c && unknown_38 == touch_id) {
            float distance = NuFsqrt(dy2 + dx2);
            i32 angle = NuAtan2D(dx, dy);
            float magnitude = distance / height;
            float sx = NU_SIN_LUT(angle);
            float sy = NU_COS_LUT(angle);
            magnitude = magnitude < 1.0f ? (magnitude < 0.0f ? 0.0f : magnitude) : 1.0f;
            magnitude = (float)(magnitude * 1.4) * 3.0f;
            sx *= magnitude;
            sy *= magnitude;
            stick_x = sx < 1.0f ? (sx > -1.0f ? sx : -1.0f) : 1.0f;
            stick_y = sy < 1.0f ? (sy > -1.0f ? sy : -1.0f) : 1.0f;
            return;
        }
    }
    unknown_3c = false;
}

f32 GetAspectRatio() {
    return static_cast<f32>(g_backingHeight) / static_cast<f32>(g_backingWidth);
}

i32 RndrUnfilledCircle(f32 x, f32 y, f32 radius, f32 border_width, f32 aspect, i32 colour, f32 progress, f32 z,
                       numtl_s *material) {
    NuPrim2DBegin(1, 7, material);

    radius *= 0.5f;
    const f32 inner_radius = radius - border_width * 0.5f;
    const f32 outer_top = y - radius;
    const f32 inner_top = y - inner_radius;

    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 1.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * x, static_cast<f32>(PS2_VREZ_H) * outer_top, z);

    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * x, static_cast<f32>(PS2_VREZ_H) * inner_top, z);

    const f32 segments = progress * 360.0f;
    // Retail CVTTSS2SI returns integer-indefinite for unordered/out-of-range
    // inputs. The touch widgets actually pass packed colours as progress;
    // off target the intrinsic keeps that no-segment result well defined.
#if defined(__ANDROID__) && defined(__i386__)
    const i32 segment_count = static_cast<i32>(segments);
#elif defined(__SSE__)
    const i32 segment_count = _mm_cvtt_ss2si(_mm_set_ss(segments));
#else
    const i32 segment_count =
        segments >= -2147483648.0f && segments < 2147483648.0f ? static_cast<i32>(segments) : INT_MIN;
#endif
    if (segment_count >= 0) {
        f32 angle = 0.0f;
        for (i32 segment = 0; segment <= segment_count; ++segment, angle += 0.017455555f) {
            const f32 sine = NuSinf(angle);
            const f32 negative_cosine = -NuCosf(angle);
            const f32 outer_x = radius * sine * aspect + x;
            const f32 outer_y = radius * negative_cosine + y;
            const f32 inner_x = inner_radius * sine * aspect + x;
            const f32 inner_y = inner_radius * negative_cosine + y;

            NuRndrPrimSetColour(colour);
            NuRndrPrimUV(0.0f, 1.0f);
            NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * outer_x, static_cast<f32>(PS2_VREZ_H) * outer_y, z);

            NuRndrPrimSetColour(colour);
            NuRndrPrimUV(0.0f, 0.0f);
            NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * inner_x, static_cast<f32>(PS2_VREZ_H) * inner_y, z);
        }
    }

    NuPrim2DEnd();
    return 1;
}

void RndrArrow(float x, float y, float scale, i32 angle, i32 colour) {
    NUVEC points[4] = {};
    points[0].x = -1.0f;
    points[0].y = -1.0f;
    points[1].y = 1.0f;
    points[2].y = -0.5f;
    points[3].x = 1.0f;
    points[3].y = -1.0f;
    const f32 aspect = GetAspectRatio();
    NuVecRotateZ(&points[0], &points[0], angle);
    NuVecRotateZ(&points[1], &points[1], angle);
    NuVecRotateZ(&points[2], &points[2], angle);
    NuVecRotateZ(&points[3], &points[3], angle);
    points[0].x *= scale;
    points[0].y *= scale;
    points[1].x *= scale;
    points[1].y *= scale;
    points[2].x *= scale;
    points[2].y *= scale;
    points[3].x *= scale;
    points[3].y *= scale;
    points[0].x *= aspect;
    points[1].x *= aspect;
    points[2].x *= aspect;
    points[3].x *= aspect;
    points[0].x += x;
    points[0].y += y;
    points[1].x += x;
    points[1].y += y;
    points[2].x += x;
    points[2].y += y;
    points[3].x += x;
    points[3].y += y;
    NuPrim2DBegin(1, 5, NULL);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[0].x, static_cast<f32>(PS2_VREZ_H) * points[0].y, 0.0f);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[1].x, static_cast<f32>(PS2_VREZ_H) * points[1].y, 0.0f);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[2].x, static_cast<f32>(PS2_VREZ_H) * points[2].y, 0.0f);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[3].x, static_cast<f32>(PS2_VREZ_H) * points[3].y, 0.0f);
    NuPrim2DEnd();
}

void NuTouchInputStick::Render() {
    const f32 progress = unknown_3c ? white : grey;
    const f32 aspect = GetAspectRatio();
    // This argument order is intentional: retail uses colour 128 and converts
    // the selected packed colour to the circle helper's progress parameter.
    RndrUnfilledCircle(x, y, width, 0.005f, aspect, 128, progress, 0.0f, NULL);
    RndrArrow(x, height * 0.3f + y, 0.0075f, 0, stick_y > 0.2f ? white : grey);
    RndrArrow(x - width * 0.3f, y, 0.0075f, 0x4000, stick_x < -0.2f ? white : grey);
    RndrArrow(x, y - height * 0.3f, 0.0075f, 0x8000, stick_y < -0.2f ? white : grey);
    RndrArrow(width * 0.3f + x, y, 0.0075f, 0xc000, stick_x > 0.2f ? white : grey);
}

void NuTouchInputButton::Render() {
    i32 colour = grey;
    if (pressed)
        colour = white;
    const f32 aspect = GetAspectRatio();
    RndrUnfilledCircle(x, y, width, 0.005f, aspect, 128, static_cast<f32>(colour), 0.0f, NULL);
    i32 angle = 0;
    if (index == 0x80)
        angle = 0x4000;
    else if (index == 0x10)
        angle = 0x8000;
    else if (index == 0x20)
        angle = 0xc000;
    RndrArrow(x, y, 0.02f, angle, colour);
}

