// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nucore/NuInputManager.h"

#include <new>

#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuMemoryManager.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuvuvec.hpp"

#include "decomp.h"
#include "nu2api/nucore/NuTouchInputButton.h"
#include "nu2api/nucore/NuTouchInputElement.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
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
#include "batman.h"
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
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/items/base/apiobject.h"
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

NuInputManager::~NuInputManager() {
    u32 i;

    if (this->max_devices == 0) {
        return;
    }

    for (i = 0; i < this->max_devices; i++) {
        NuMemoryGet()->GetThreadMem()->BlockFree(this->devices[i], 0);
    }

    NuMemoryGet()->GetThreadMem()->BlockFree(this->devices, 0);

    NuInputDevicePS::ClassShutdownPS();
}

u32 NuInputManager::GetMaxDevices() const {
    return this->max_devices;
}

NuInputDevice *NuInputManager::GetDevice(u32 port) {
    return this->devices[port];
}

const NuInputDevice *NuInputManager::GetDevice(u32 port) const {
    return devices[port];
}

NuInputDevice::~NuInputDevice() {
}

void NuInputDevice::SetDisconnected(void) {
    this->motor_1 = 0.0f;
    this->motor_2 = 0.0f;

    this->rumble_time_elapsed = 0.0f;

    this->is_connected = false;
    this->type = NUPADTYPE_UNKNOWN_7;
    this->idx_by_type = 0;

    this->caps = 0;

    this->is_intercepted = false;
    this->has_headphones_connected = false;

    this->rumble_cooldown = 0.0f;

    this->volume = 0.0f;
}

void NuInputDevice::Clear(void) {
    i32 i;

    this->button_states = 0;

    // ORIG_BUG: The full arrays are not zeroed. Mistaken division by
    // `sizeof(f32)` is a plausible explanation.
    memset(this->analog_values, 0, sizeof(this->analog_values) / sizeof(f32));
    memset(this->motion_values, 0, sizeof(this->motion_values) / sizeof(f32));

    memset(&this->touch_data, 0, sizeof(NuInputTouchData));
    memset(&this->mouse_data, 0, sizeof(NuInputMouseData));
}

NuInputDevice::NuInputDevice(u32 port) {
    this->port = port;
    this->is_rumble_killed = false;

    this->prev_valid_type = NUPADTYPE_UNKNOWN_7;
    this->prev_valid_idx_by_type = 7;

    this->translators[0] = NULL;
    this->translators[1] = NULL;

    this->translator_count = 0;

    memset(&this->unknown_touch_data, 0, sizeof(NuInputTouchData));

    SetDisconnected();
    Clear();
}

NuInputManager::NuInputManager() {
    u32 i;
    u32 max_devices;

    max_devices = NuInputDevicePS::ClassInitPS();

    this->devices = (NuInputDevice **)NU_ALLOC(max_devices * sizeof(NuInputDevice *), 0x4,
                                               NuMemoryManager::MEM_ALLOC_SET_TO_ZERO, "", NUMEMORY_CATEGORY_NONE);
    this->unknown_08 = 0;

    this->max_devices = max_devices;

    for (i = 0; i < this->max_devices; i++) {
        this->devices[i] = (NuInputDevice *)NU_ALLOC_T(NuInputDevice, NuMemoryManager::MEM_ALLOC_SET_TO_ZERO, "",
                                                       NUMEMORY_CATEGORY_NONE);
        NuInputDevice::InitSlot(this->devices[i], i);
        this->devices[i]->Clear();
    }
}

void NuInputDevice::DeadZone(NUPADANALOGVALUE input, f32 dead_zone) {
    f32 value;
    f32 new_value;

    value = this->analog_values[input];

    if (value > dead_zone) {
        new_value = (value - dead_zone) / (1.0f - dead_zone);
    } else if (!(value < -dead_zone)) {
        new_value = 0.0f;
    } else {
        new_value = (value + dead_zone) / (1.0f - dead_zone);
    }

    this->analog_values[input] = new_value;
}

void NuInputDevice::AddTranslator(NuInputDeviceTranslator *translator) {
    this->translators[this->translator_count] = translator;
    this->translator_count++;
}

void NuInputDevice::ProcessTouchData() {
    if (!(caps & 0x400))
        return;

    u32 count = touch_data.touch_count;
    for (u32 i = 0; i < count; ++i) {
        touch_data.touch_events[i].unknown_01 = 0;
        touch_data.touch_events[i].unknown_02 = 0;
    }
    u32 previous_count = unknown_touch_data.touch_count;
    for (u32 i = 0; i < count; ++i) {
        i32 previous = 0xff;
        for (u32 j = 0; j < previous_count; ++j) {
            if (unknown_touch_data.touch_events[j].unknown_14 == touch_data.touch_events[i].unknown_14) {
                previous = j;
                break;
            }
        }
        if (previous == 0xff)
            touch_data.touch_events[i].unknown_02 = 1;
    }
    u32 added = 0;
    for (u32 i = 0; i < previous_count; ++i) {
        if (__builtin_expect(unknown_touch_data.touch_events[i].unknown_01 != 0, 0))
            continue;
        i32 current = 0xff;
        for (u32 j = 0; j < count; ++j) {
            if (touch_data.touch_events[j].unknown_14 == unknown_touch_data.touch_events[i].unknown_14) {
                current = j;
                break;
            }
        }
        if (current == 0xff && added < 10 - count) {
            NuInputTouch *touch = &touch_data.touch_events[count + added++];
            *touch = unknown_touch_data.touch_events[i];
            touch->unknown_01 = 1;
            touch->unknown_02 = 0;
        }
    }
    touch_data.touch_count = count + added;
}

void NuInputDevice::SetIndexByType(u32 idx_by_type) {
    this->idx_by_type = idx_by_type;
    this->prev_valid_idx_by_type = idx_by_type;
}

u32 NuInputDevice::GetPort() const {
    return port;
}

NUPADTYPE NuInputDevice::GetType(void) const {
    return this->type;
}

const NuInputDevice *NuInputManager::GetFirstDeviceByType(NUPADTYPE type) const {
    for (u32 i = 0; i < max_devices; ++i) {
        if (devices[i]->GetType() == type)
            return devices[i];
    }
    return NULL;
}

NuInputDevice *NuInputManager::GetFirstDeviceByType(NUPADTYPE type) {
    u32 i;

    for (i = 0; i < this->max_devices; i++) {
        if (this->devices[i]->GetType() == type) {
            return this->devices[i];
        }
    }

    return NULL;
}

u32 NuInputDevice::GetIndexByType() const {
    return idx_by_type;
}

NUPADTYPE NuInputDevice::GetLastValidType() const {
    return prev_valid_type;
}

u32 NuInputDevice::GetLastValidIndexByType() const {
    return prev_valid_idx_by_type;
}

NUPADATTACHMENTTYPE NuInputDevice::GetAttachmentType() const {
    return attch_type;
}

u32 NuInputDevice::GetCaps() const {
    return caps;
}

bool NuInputDevice::SupportsCaps(u32 mask) const {
    return (GetCaps() & mask) != 0;
}

bool NuInputDevice::IsConnected(void) const {
    return this->is_connected;
}

bool NuInputDevice::IsIntercepted() const {
    return is_intercepted;
}

void NuInputDevice::KillRumble() {
    motor_1 = 0.0f;
    motor_2 = 0.0f;
    is_rumble_killed = true;
}

bool NuInputDevice::IsButtonPressed(u32 buttons) const {
    return IsConnected() && (button_states & buttons) != 0;
}

u32 NuInputDevice::GetButtons(void) const {
    if (IsConnected()) {
        return this->button_states;
    }

    return 0;
}

f32 NuInputDevice::GetAnalogValue(NUPADANALOGVALUE input) const {
    if (IsConnected()) {
        return this->analog_values[input];
    }

    return 0.0f;
}

f32 NuInputDevice::GetMotionValue(NUPADMOTIONVALUE input) const {
    if (IsConnected())
        return motion_values[input];
    return 0.0f;
}

const NuInputTouchData *NuInputDevice::GetTouchData() const {
    return &touch_data;
}

const NuInputMouseData *NuInputDevice::GetMouseData() const {
    return &mouse_data;
}

void NuInputDevice::SetMotors(float motor_1, float motor_2) {
    this->motor_1 = motor_1;
    this->motor_2 = motor_2;
}

void NuInputDevice::EnableDPD() {
    NuInputDevicePS::EnableDPDPS(port);
}

void NuInputDevice::DisableDPD() {
    NuInputDevicePS::DisableDPDPS(port);
}

bool NuInputDevice::HasHeadphonesConnected() const {
    return has_headphones_connected;
}

f32 NuInputDevice::GetVolume() const {
    return volume;
}

void NuInputDevice::ConvertToEmulatedTouchFromMouse(void) {
    type = NUPADTYPE_TOUCH;
    idx_by_type = 0;
    prev_valid_type = NUPADTYPE_TOUCH;
    prev_valid_idx_by_type = 0;
    touch_data.touch_count = 0;
    if (mouse_data.buttons[0].active != 0) {
        NuInputTouch &touch = touch_data.touch_events[0];
        touch.unknown_04 = mouse_data.x;
        touch.unknown_01 = mouse_data.buttons[0].released;
        touch.unknown_00 = 1;
        touch.unknown_08 = mouse_data.y;
        touch.unknown_02 = mouse_data.buttons[0].pressed;
        touch.unknown_14 = 0;
        touch_data.touch_count = 1;
    }
    if (mouse_data.buttons[1].active != 0) {
        u32 index = touch_data.touch_count;
        touch_data.touch_events[index].unknown_04 = mouse_data.x;
        touch_data.touch_events[index].unknown_01 = mouse_data.buttons[1].released;
        touch_data.touch_events[index].unknown_00 = 1;
        touch_data.touch_events[index].unknown_08 = mouse_data.y;
        touch_data.touch_events[index].unknown_02 = mouse_data.buttons[1].pressed;
        touch_data.touch_events[index].unknown_14 = 1;
        touch_data.touch_count = index + 1;
    }
    memset(&mouse_data, 0, sizeof(mouse_data));
    caps = 0x400;
}

__attribute__((optimize("O2"))) void NuInputDevice::Update(f32 delta_time, bool emulate_touch) {
    u32 i;

    f32 raw_motion_values[20];
    NuInputTouchData raw_touch_data;
    NuInputMouseData raw_mouse_data;

    this->is_connected = NuInputDevicePS::IsConnectedPS(this->port);
    if (!this->is_connected) {
        SetDisconnected();

        return;
    }

    this->type = NuInputDevicePS::GetTypePS(this->port);
    this->attch_type = NuInputDevicePS::GetAttachmentTypePS(this->port);

    this->caps = NuInputDevicePS::GetCapsPS(this->port);

    this->is_intercepted = NuInputDevicePS::IsInterceptedPS(this->port);

    if (!this->is_intercepted) {
        f32 raw_analog_values[12];
        NuInputDevicePS::ReadButtonsPS(this->port, &this->button_states);
        NuInputDevicePS::ReadAnalogValuesPS(this->port, this->analog_values);
        NuInputDevicePS::ReadMotionValuesPS(this->port, this->motion_values);
        NuInputDevicePS::ReadTouchDataPS(this->port, &this->touch_data);
        NuInputDevicePS::ReadMouseDataPS(this->port, &this->mouse_data);

        DeadZone(NUPADANALOGVALUE_UNKNOWN_0, 0.2f);
        DeadZone(NUPADANALOGVALUE_UNKNOWN_1, 0.2f);
        DeadZone(NUPADANALOGVALUE_UNKNOWN_2, 0.2f);
        DeadZone(NUPADANALOGVALUE_UNKNOWN_3, 0.2f);

        DeadZone(NUPADANALOGVALUE_L1, 0.2f);
        DeadZone(NUPADANALOGVALUE_R1, 0.2f);
        DeadZone(NUPADANALOGVALUE_L2, 0.2f);
        DeadZone(NUPADANALOGVALUE_R2, 0.2f);

        DeadZone(NUPADANALOGVALUE_LEFT_X, 0.2f);
        DeadZone(NUPADANALOGVALUE_LEFT_Y, 0.2f);
        DeadZone(NUPADANALOGVALUE_RIGHT_X, 0.2f);
        DeadZone(NUPADANALOGVALUE_RIGHT_Y, 0.2f);

        if (emulate_touch && this->type == NUPADTYPE_MOUSE) {
            ConvertToEmulatedTouchFromMouse();
        }

        memcpy(&this->unknown_touch_data, &this->touch_data, sizeof(NuInputTouchData));

        memcpy(raw_analog_values, this->analog_values, sizeof(this->analog_values));
        memcpy(raw_motion_values, this->motion_values, sizeof(this->motion_values));
        memcpy(&raw_touch_data, &this->touch_data, sizeof(NuInputTouchData));
        memcpy(&raw_mouse_data, &this->mouse_data, sizeof(NuInputMouseData));

        if (this->type == NUPADTYPE_TOUCH) {
            for (i = 0; i < this->translator_count; i++) {
                this->translators[i]->Execute(this->port, this->type, this->attch_type, this->caps, this->button_states,
                                              raw_analog_values, raw_motion_values, &raw_touch_data, &raw_mouse_data,
                                              this->type, this->attch_type, this->caps, this->button_states,
                                              this->analog_values, this->motion_values, &this->touch_data,
                                              &this->mouse_data);
            }
        } else if (this->type == NUPADTYPE_UNKNOWN_6) {
            NUPADTYPE out_type;
            NUPADATTACHMENTTYPE out_attch_type;
            f32 out_analog_values[12];
            f32 out_motion_values[20];
            u32 out_caps;
            u32 out_button_states;

            for (i = 0; i < this->translator_count; i++) {
                this->translators[i]->Execute(this->port, this->type, this->attch_type, this->caps, this->button_states,
                                              raw_analog_values, raw_motion_values, &raw_touch_data, &raw_mouse_data,
                                              out_type, out_attch_type, out_caps, out_button_states, out_analog_values,
                                              out_motion_values, &this->touch_data, &this->mouse_data);
            }
        }
    } else {
        Clear();
    }

    this->prev_valid_type = this->type;

    if (this->motor_1 > 0.0f || this->motor_2 > 0.0f) {
        this->rumble_time_elapsed += delta_time;

        if (this->rumble_time_elapsed > 7.0f && this->rumble_cooldown == 0.0f) {
            this->rumble_cooldown = 2.0f;
            this->rumble_time_elapsed = 0.0f;
        }
    } else {
        this->rumble_time_elapsed -= NuFmax(delta_time, 0.5f);
        if (this->rumble_time_elapsed < 0.0f) {
            this->rumble_time_elapsed = 0.0f;
        }
    }

    if (this->rumble_cooldown > 0.0f || this->is_rumble_killed || this->is_intercepted) {
        NuInputDevicePS::SetMotorsPS(this->port, 0.0f, 0.0f);

        this->rumble_cooldown -= delta_time;
        if (this->rumble_cooldown < 0.0f) {
            this->rumble_cooldown = 0.0f;
        }

        this->is_rumble_killed = false;
    } else {
        NuInputDevicePS::SetMotorsPS(this->port, this->motor_1, this->motor_2);
    }

    this->has_headphones_connected = NuInputDevicePS::HasHeadphonesConnectedPS(this->port);
    this->volume = NuInputDevicePS::GetVolumePS(this->port);
}

void NuInputManager::UpdateAll(f32 delta_time) {
    u32 i;
    NUPADTYPE type;

    NuInputDevicePS::UpdateAllPS(delta_time);

    NuPad_UpdateTouchScreenData();

    memset(this->next_idx_by_type, 0, sizeof(this->next_idx_by_type));

    for (i = 0; i < this->max_devices; i++) {
        this->devices[i]->Update(delta_time, enable_touch_controls);

        type = this->devices[i]->GetType();

        this->devices[i]->SetIndexByType(this->next_idx_by_type[type]);
        this->next_idx_by_type[type]++;
    }
}

void NuInputManager::KillRumbleAll() {
    for (u32 i = 0; i < max_devices; ++i)
        devices[i]->KillRumble();
    UpdateAll(0.0f);
}

