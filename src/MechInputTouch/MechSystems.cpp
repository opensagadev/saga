// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "MechInputTouch/MechTouchUIElements.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/numtx.h"

#include <stddef.h>
#include <string.h>
#include <new>
#include "legoapi/render/core/screen.h"
#include "legoapi/menus/core/text.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/characters/motion/gameanim.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "batman.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/core/startup/main.h"
#include "nu2api/nucore/nuapi.h"
#include <GLES2/gl2.h>
#include <stdio.h>
#include "gameframework/saveload.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/core/render.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <stdlib.h>
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nucore/nutime.h"
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
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/render/fx/game_deb.h"
#include <ctype.h>
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include <stdarg.h>
#include <limits.h>
#include "MechInputTouch/MechSystems.h"
#include "MechInputTouch/MechTouchUIElements.h"

extern void (*preRenderFlashingHack)(void);
extern void (*postRenderFlashingHack)(void);

u8 MechSystems::SkipTextScroll = 0;

// vtable for MechSystems @0x66b320 slot 0x08: returns the static name string
// @0x573c42 (from MechSystems::GetName @0x45def0).
char const *MechSystems::GetName() {
    return "MechSystems";
}

i32 MechInputTouchMenuController::AnyTouchesThisFrame = 0;
bool MechInputTouchMenuController::PackButtonPressed = false;
i32 MechInputTouchMenuController::PackButtonID = -1;

u32 animSetVisibilityHack[6];
i32 specVisibilityFlashHack;

extern f32 hackFlashTimer;
extern GAMEANIMSET_s *hackFlashingGameAnimSet;
extern nuhspecial_s *hackFlashingSpecial;

struct TouchHolder;
struct nuhspecial_s;
struct VuVec;

// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.

f32 SwipeDecalRenderer_MaxWidth = 0.6f;
f32 SwipeDecalRenderer_MinWidth = 0.2f;

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

extern i32 LevFlag[4];

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct EDCREATURE_s;

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

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

void MechSystems::Reset(ThingResetData *) {
}

void MechSystems::ExitLevel(WORLDINFO_s *) {
    if (level_ui_elements[0] != NULL) {
        TouchUI().RemoveUIElement(*level_ui_elements[0]);
        delete level_ui_elements[0];
        level_ui_elements[0] = NULL;
    }
    if (level_ui_elements[1] != NULL) {
        TouchUI().RemoveUIElement(*level_ui_elements[1]);
        delete level_ui_elements[1];
        level_ui_elements[1] = NULL;
    }
    if (level_ui_elements[2] != NULL) {
        TouchUI().RemoveUIElement(*level_ui_elements[2]);
        delete level_ui_elements[2];
        level_ui_elements[2] = NULL;
    }
}

void PostRenderFlashHack() {
    if (hackFlashingGameAnimSet != NULL && hackFlashingGameAnimSet->objects != NULL) {
        GAMEANIMOBJ_s *object = hackFlashingGameAnimSet->objects;
        NuSpecialSetVisibility(&object->special, animSetVisibilityHack[0] & 1);
        u32 index = 1;
        while ((object = object->next) != NULL) {
            NuSpecialSetVisibility(&object->special,
                                   (static_cast<i32>(animSetVisibilityHack[index >> 5]) >> (index & 31)) & 1);
            if (++index == 0xc1)
                break;
        }
    }
    if (hackFlashingSpecial != NULL)
        NuSpecialSetVisibility(hackFlashingSpecial, specVisibilityFlashHack);
    if (hackFlashTimer <= 0.0f && hackFlashTimer != 0.0f) {
        hackFlashingSpecial = NULL;
        hackFlashingGameAnimSet = NULL;
    }
}

void PreRenderFlashHack() {
    if (hackFlashingGameAnimSet == NULL && hackFlashingSpecial == NULL)
        return;

    TouchHacks::TintStack tint;
    hackFlashTimer -= FRAMETIME;
    if (TouchHacks::ShouldFlash(hackFlashTimer)) {
        NUCOLOUR3 *colour = TouchHacks::GetFlashColour();
        NuRndrLightingStateCurrent.ambient = *colour;
        NuRndrSetAmbientLightPS(colour);
    }

    if (hackFlashingGameAnimSet != NULL) {
        GameAnimSet_Draw(*hackFlashingGameAnimSet);
        animSetVisibilityHack[1] = 0;
        GAMEANIMOBJ_s *object = hackFlashingGameAnimSet->objects;
        animSetVisibilityHack[0] = 0;
        u32 index = 0;
        while (object != NULL) {
            if (NuSpecialGetVisibilityFn(&object->special) != 0)
                animSetVisibilityHack[index >> 5] |= 1u << (index & 31);
            ++index;
            NuSpecialSetVisibility(&object->special, 0);
            if (index == 0xc0)
                break;
            object = object->next;
        }
    }
    if (hackFlashingSpecial != NULL) {
        NuSpecialDrawAt(hackFlashingSpecial, NuSpecialGetDrawMtx(hackFlashingSpecial));
        specVisibilityFlashHack = NuSpecialGetVisibilityFn(hackFlashingSpecial);
        NuSpecialSetVisibility(hackFlashingSpecial, 0);
    }
}

MechSystems::~MechSystems() {
    NuMtlDestroy(location_ping_material);
    location_ping_material = NULL;
    NuMtlDestroy(swipe_material);
    swipe_material = NULL;
    NuMtlDestroy(tag_hold_background_material);
    tag_hold_background_material = NULL;
    NuMtlDestroy(radar_pulse_material);
    radar_pulse_material = NULL;
    PauseButton().MechTouchUIPauseButton::~MechTouchUIPauseButton();
    PlayerButton().MechTouchUIPlayerButton::~MechTouchUIPlayerButton();
    TouchUI().~MechTouchUI();
}

HudRadarPulse::HudRadarPulse(VuVec const &initial_position) : position(initial_position) {
    pulses[1].delay = 0.2f;
    pulses[2].delay = 0.4f;

    pulses[0].delay_finished = 0;
    pulses[0].finished = 0;
    pulses[0].angle = 0;
    pulses[0].delay = 0.0f;
    pulses[1].delay_finished = 0;
    pulses[0].radius = 0.0f;
    pulses[1].finished = 0;
    pulses[0].speed = 0.75f;
    pulses[1].angle = 0;
    pulses[1].radius = 0.0f;
    pulses[2].delay_finished = 0;
    pulses[1].speed = 0.75f;
    pulses[2].finished = 0;
    pulses[2].radius = 0.0f;
    pulses[2].angle = 0;
    pulses[2].speed = 0.75f;

    active = 1;
    paused = 0;
}

void HudRadarPulse::Process(float delta) {
    if (active) {
        active = 0;
        if (NewMode == 0 && NewLData == NULL && editor_active == 0 && GameTimer.time_elapsed > 0.0f &&
            GameTimer.update_count != 0 && WORLD != NULL && CutSceneWaiting == 0 &&
            (Paused == 0 || GetMenuID() == 0x19 || GetMenuID() == 0x15) &&
            (CUTSTOPGAME == 0 || CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo))) && MiniCutCam == 0 &&
            (paused || (WORLD->current_level != TITLES_LDATA && WORLD->current_level != STATUS_LDATA &&
                        !(WORLD->current_level->flags & 0x400) && memcard_autosavestarted == 0 &&
                        !(memcard_autosavepostdelay > 0.0f) && !(memcard_autosavepredelay > 0.0f))))
            active = 1;
    }
    for (i32 i = 0; i < 3; ++i) {
        if (!pulses[i].delay_finished) {
            pulses[i].delay -= delta;
            if (pulses[i].delay <= 0.0f)
                pulses[i].delay_finished = 1;
        }
        if (pulses[i].delay_finished && !pulses[i].finished) {
            float speed = pulses[i].speed;
            pulses[i].radius += speed * delta;
            speed -= 0.4f * delta;
            pulses[i].speed = speed > 0.0f ? speed : 0.0f;
            pulses[i].angle += static_cast<i32>(32768.0f * delta);
            if (pulses[i].angle > 0x7fff)
                pulses[i].finished = 1;
        }
    }
}

u8 HudRadarPulse::IsFinished() {
    for (i32 i = 0; i < 3; ++i) {
        if (!pulses[i].delay_finished || !pulses[i].finished)
            return 0;
    }
    return 1;
}

SwipeDecalRenderer::SwipeDecalRenderer(TouchHolder &holder, i32 index, SwipeDecalRenderer::Style style) {
    alpha.Initialize();
    width.Initialize();
    this->style = style;

    const NuVec2 &swipe_point = holder.swipe_samples[index].position;
    const f32 swipe_x = (swipe_point.x + 1.0f) * 0.5f;
    const f32 swipe_y = (swipe_point.y + 1.0f) * 0.5f;
    const f32 current_x = (holder.touch_position.x + 1.0f) * 0.5f;
    const f32 current_y = (holder.touch_position.y + 1.0f) * 0.5f;
    const f32 dx = current_x - swipe_x;
    const f32 dy = current_y - swipe_y;
    f32 target_width = NuFsqrt(dx * dx + dy * dy);
    // MAX/MIN expand the inner MIN twice, which gives retail's re-tested clamp branches.
    target_width = MAX(SwipeDecalRenderer_MinWidth, MIN(target_width, SwipeDecalRenderer_MaxWidth));

    width.Start(0.0f, target_width, 0.1f);
    *width.target = 0.0f;
    x = swipe_x;
    y = swipe_y;
    angle = NuAtan2D(dx, dy);
    alpha.Start(0.02f, 1.0f, 0.1f);
    *alpha.target = 0.02f;
}

void SwipeDecalRenderer::Process(float frame_time) {
    width.Process(frame_time);
    alpha.Process(frame_time);

    if (alpha.value > 0.5f && !alpha.IsActive() && !width.IsActive()) {
        alpha.Start(*alpha.target, 0.0f, 0.3f);
        alpha.delay = 0.2f;
    }
}

MoveToMarker::MoveToMarker(MechObjectInterface &object) : target(&object) {
    field_5c = 0.0f;
    field_108_0 = false;
    field_108_1 = false;
    fading_out = false;
    field_108_3 = false;
    blowing_up = false;
    persistent = false;
    object.GetFloorTargetPos(position, -1);
    field_fc = 0.0f;
    field_100 = 0.0f;
    rotation.Start(0.0f, 21845.0f, 0.5f);
    *rotation.target = 0.0f;
    colour.elapsed = 0.0f;
    colour.duration = 0.5f;
    colour.delay = 0.0f;
    colour.from = VuVec(255.0f, 255.0f, 255.0f, 1.0f);
    colour.to = VuVec(0.0f, 85.0f, 253.0f, 1.0f);
    *colour.target = VuVec(255.0f, 255.0f, 255.0f, 1.0f);
    scale.Start(0.0f, 1.0f, 0.5f);
    *scale.target = 0.0f;
    secondary_rotation.Start(0.0f, 32768.0f, 0.5f);
    *secondary_rotation.target = 0.0f;
    height = object.GetHeight();
    if (object.GetObjectType() == 1) {
        temporary_target = true;
        radius.Start(0.0f, object.GetRadius(), 0.2f);
        *radius.target = 0.0f;
        alpha.Start(0.0f, 1.0f, 0.1f);
        *alpha.target = 0.0f;
    }
}

void MoveToMarker::FadeOut() {
    if (!persistent) {
        fading_out = true;
    }
}

void MoveToMarker::Process(float frame_time) {
    field_5c -= frame_time;
    if (fading_out) {
        if (!field_108_3 && field_5c < 0.0f) {
            field_108_3 = true;
            fading_out = false;
            scale.Start(*scale.target, 0.0f, 0.3f);
            alpha.Start(*alpha.target, 0.0f, 0.3f);
            secondary_rotation.Start(*secondary_rotation.target, 0.0f, 0.3f);
            if (target.Get() != NULL && target.Get()->GetObjectType() == 1) {
                radius.Start(*radius.target, 0.0f, 0.7f);
            }
        }
    }
    if (target.Get() != NULL) {
        if (target.Get()->GetObjectType() != 1) {
            target.Get()->GetFloorTargetPos(position, -1);
        }
    }
    rotation.Process(frame_time);
    scale.Process(frame_time);
    colour.Process(frame_time);
    radius.Process(frame_time);
    alpha.Process(frame_time);
    secondary_rotation.Process(frame_time);
    if (!radius.IsActive() && radius.value > 0.0f) {
        if (target.Get() != NULL && target.Get()->GetObjectType() == 1) {
            radius.Start(*radius.target, radius.value * 2.0f, 0.2f);
            alpha.Start(*alpha.target, 0.0f, 0.1f);
        }
        if (field_108_1) {
            FadeOut();
        }
    }
    if (!rotation.IsActive()) {
        field_100 -= static_cast<i32>(frame_time * 65536.0f);
    }
    field_fc += static_cast<i32>(frame_time * 65536.0f);
}

void MoveToMarker::BlowUp() {
    if (blowing_up) {
        return;
    }
    blowing_up = true;
    fading_out = false;
    field_108_3 = false;
    secondary_rotation.Start(*secondary_rotation.target, 16384.0f, 0.3f);
    colour.from.x = colour.target->x;
    colour.from.y = colour.target->y;
    colour.from.z = colour.target->z;
    colour.from.w = colour.target->w;
    colour.to = VuVec(21.0f, 146.0f, 27.0f, 1.0f);
    colour.elapsed = 0.0f;
    colour.duration = 0.2f;
    colour.delay = 0.0f;
    rotation.Start(*rotation.target, 16384.0f, 0.3f);
    scale.Start(*scale.target, 1.0f, 0.3f);
}

MechSystems::MechSystems() {
    new (ui_storage) MechTouchUI();
    new (player_button_storage) MechTouchUIPlayerButton();
    new (pause_button_storage) MechTouchUIPauseButton();
    initialized = 0;
    new (click_to_press_start_tracker_storage) ClickToPressStartGestureTracker();
    gesture_controller = NULL;
    location_ping_material = NULL;
    swipe_material = NULL;
    tag_hold_background_material = NULL;
    radar_pulse_material = NULL;
    memset(move_to_markers, 0, sizeof(move_to_markers));
    swipe_markers[0] = NULL;
    swipe_markers[1] = NULL;
    swipe_markers[2] = NULL;
    swipe_markers[3] = NULL;
    level_ui_elements[0] = NULL;
    level_ui_elements[1] = NULL;
    level_ui_elements[2] = NULL;
    radar_pulses[0] = NULL;
    radar_pulses[1] = NULL;
    radar_pulses[2] = NULL;
    radar_pulses[3] = NULL;
}

void MechSystems::LoadPerm() {
    MechInputTouchVirtualConsoleController::LoadPerm();

    if (location_ping_material == NULL) {
        location_ping_material = NuMtlCreate3D(1);
        NUMTL *material = location_ping_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_LOCATIONPING"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(location_ping_material);
    }
    if (swipe_material == NULL) {
        swipe_material = NuMtlCreate(1);
        NUMTL *material = swipe_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_SWIPE"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(swipe_material);
    }
    if (tag_hold_background_material == NULL) {
        tag_hold_background_material = NuMtlCreate(1);
        NUMTL *material = tag_hold_background_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_TAGHOLDBGTEX"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(tag_hold_background_material);
    }
    if (radar_pulse_material == NULL) {
        radar_pulse_material = NuMtlCreate(1);
        NUMTL *material = radar_pulse_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_RIPPLE"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(radar_pulse_material);
    }
}

void MechSystems::Init() {
    if (initialized == 0) {
        input_touch_system.Init();
        TouchUI().Init();
        TouchUI().AddUIElement(PlayerButton());
        TouchUI().AddUIElement(PauseButton());
        initialized = 1;
        preRenderFlashingHack = PreRenderFlashHack;
        postRenderFlashingHack = PostRenderFlashHack;
    }
}

void MechSystems::ProcessOnlyWhenPaused(ThingProcessData *) {
    if (initialized == 0) {
        Init();
    }
}

void MechSystems::ProcessEvenWhenPaused(ThingProcessData *data) {
    if (initialized == 0) {
        Init();
    }
    if (radar_pulses[0] != NULL) {
        radar_pulses[0]->Process(FRAMETIME);
        if (radar_pulses[0]->IsFinished()) {
            delete radar_pulses[0];
            radar_pulses[0] = NULL;
        }
    }
    if (radar_pulses[1] != NULL) {
        radar_pulses[1]->Process(FRAMETIME);
        if (radar_pulses[1]->IsFinished()) {
            delete radar_pulses[1];
            radar_pulses[1] = NULL;
        }
    }
    if (radar_pulses[2] != NULL) {
        radar_pulses[2]->Process(FRAMETIME);
        if (radar_pulses[2]->IsFinished()) {
            delete radar_pulses[2];
            radar_pulses[2] = NULL;
        }
    }
    if (radar_pulses[3] != NULL) {
        radar_pulses[3]->Process(FRAMETIME);
        if (radar_pulses[3]->IsFinished()) {
            delete radar_pulses[3];
            radar_pulses[3] = NULL;
        }
    }
    input_touch_system.ProcessEvenWhenPaused(data);
    TouchUI().Process(FRAMETIME);
}

void MechSystems::EnterLevel(WORLDINFO_s *) {
    if (initialized == 0) {
        Init();
    }
    if (initialized != 0) {
        PlayerButton().SetupTargetIds();
    }
}

void MechSystems::Process(ThingProcessData *) {
    if (initialized == 0) {
        Init();
    }
    if (initialized != 0) {
        for (i32 i = 0; i < 32; ++i) {
            if (move_to_markers[i] != NULL) {
                move_to_markers[i]->Process(FRAMETIME);
                if (move_to_markers[i]->field_108_3 && move_to_markers[i]->scale.value <= 0.001f) {
                    delete move_to_markers[i];
                    move_to_markers[i] = NULL;
                }
            }
        }
        for (i32 i = 0; i < 4; ++i) {
            if (swipe_markers[i] != NULL) {
                swipe_markers[i]->Process(FRAMETIME);
                if (swipe_markers[i]->alpha.value < 0.01f) {
                    delete swipe_markers[i];
                    swipe_markers[i] = NULL;
                }
            }
        }
        for (i32 i = 0; i < 3; ++i) {
            if (level_ui_elements[i] != NULL && level_ui_elements[i]->fading_out != 0 &&
                level_ui_elements[i]->first_fade.value <= 0.0f) {
                TouchUI().RemoveUIElement(*level_ui_elements[i]);
                delete level_ui_elements[i];
                level_ui_elements[i] = NULL;
            }
        }
    }
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->mech_auto_jump_manager != NULL) {
        world->mech_auto_jump_manager->Process();
    }
}

MechSystems *MechSystems::Get() {
    static MechSystems *instance = NULL;
    if (instance == NULL) {
        instance = new MechSystems();
    }
    return instance;
}

void HudRadarPulse::Render() {
    if (active) {
        for (i32 i = 0; i < 3; ++i) {
            if (pulses[i].delay_finished && !pulses[i].finished) {
                f32 x = position.x + 1.0f;
                f32 y = 1.0f - position.y;
                numtl_s *material = MechSystems::Get()->radar_pulse_material;
                i32 alpha = static_cast<i32>(90.0f * NU_SIN_LUT(pulses[i].angle));
                f32 radius = pulses[i].radius;
                f32 width = GetAspectRatio() * radius;
                RndrTexQuad(x * 0.5f, y * 0.5f, width, radius,
                            static_cast<i32>((static_cast<u32>(alpha) << 24) | 0x808080), material, 0);
            }
        }
    }
}

void SwipeDecalRenderer::Render() {
    NUVEC vertices[4] = {
        {-0.037499998f, 0.0f, 0.0f},
        {0.037499998f, 0.0f, 0.0f},
        {-0.037499998f, -width.value, 0.0f},
        {0.037499998f, -width.value, 0.0f},
    };
    const f32 uv_height = width.value / width.to;

    NuVecRotateZ(&vertices[0], &vertices[0], angle);
    NuVecRotateZ(&vertices[1], &vertices[1], angle);
    NuVecRotateZ(&vertices[2], &vertices[2], angle);
    NuVecRotateZ(&vertices[3], &vertices[3], angle);
    for (i32 i = 0; i < 4; ++i) {
        vertices[i].x += x;
        vertices[i].y += 1.0f - y;
    }

    NuPrim2DBegin(1, 7, MechSystems::Get()->swipe_material);
    u32 colour = (static_cast<u32>(static_cast<i32>(alpha.value * 128.0f)) << 24) | 0x000080;
    if (style != static_cast<Style>(1)) {
        if (__builtin_expect(style == static_cast<Style>(2), 0))
            colour = (static_cast<u32>(static_cast<i32>(alpha.value * 255.0f)) << 24) | 0x0000ff;
        else
            colour |= 0x808000;
    }

    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(0.0f, 0.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[0].x, static_cast<f32>(PS2_VREZ_H) * vertices[0].y, 0.0f);
    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(1.0f, 0.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[1].x, static_cast<f32>(PS2_VREZ_H) * vertices[1].y, 0.0f);
    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(0.0f, uv_height);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[2].x, static_cast<f32>(PS2_VREZ_H) * vertices[2].y, 0.0f);
    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(1.0f, uv_height);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[3].x, static_cast<f32>(PS2_VREZ_H) * vertices[3].y, 0.0f);
    NuPrim2DEnd();
}

void MechSystems::Display(ThingRenderData *) {
    TouchUI().Render();
    if (initialized != 0) {
        for (i32 i = 0; i < 4; ++i) {
            if (swipe_markers[i] != NULL) {
                swipe_markers[i]->Render();
            }
        }
        for (i32 i = 0; i < 4; ++i) {
            if (radar_pulses[i] != NULL) {
                radar_pulses[i]->Render();
            }
        }
    }
}

void MoveToMarker::Render() {
    NUVEC draw_position = {position.x, position.y, position.z};
    NUMTX_ALIGNED16 ping_matrix;
    NUMTX_ALIGNED16 text_matrix;
    if (temporary_target) {
        MechSystems *systems = MechSystems::Get();
        if (systems->location_ping_material != NULL) {
            NUVEC ping_scale = {radius.value, radius.value, radius.value};
            NuMtxSetRotationY(&ping_matrix, 0);
            NuMtxRotateX(&ping_matrix, 0x4000);
            NuMtxScale(&ping_matrix, &ping_scale);
            draw_position.y += 0.01f;
            NuMtxTranslate(&ping_matrix, &draw_position);
            const i32 ping_colour = (static_cast<i32>(alpha.value * 255.0f) << 24) | 0xffffff;
            RndrTexQuad3D(reinterpret_cast<VuMtx const &>(ping_matrix), ping_colour, systems->location_ping_material);
        }
    }
    if (!field_108_0) {

        draw_position.y = NuTrigTable[(static_cast<i32>(rotation.value) >> 1) & 0x7fff] * 0.1f + height + position.y +
                          NuTrigTable[(field_100 >> 1) & 0x7fff] * 0.005f;
        const f32 marker_scale =
            (NuTrigTable[(static_cast<i32>(secondary_rotation.value) >> 1) & 0x7fff] * 0.25f + 1.0f) * 0.1f;
        NUVEC text_scale = {marker_scale, marker_scale, marker_scale};
        NuMtxSetRotationY(&text_matrix, field_fc);
        NuMtxScale(&text_matrix, &text_scale);
        NuMtxTranslate(&text_matrix, &draw_position);

        extern char *ASCII_DOWN;
        NuQFntPushPrintMode(4);
        NuQFntSet(QFont3DZ);
        NuQFntSetMtx(QFont3DZ, &text_matrix);
        NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
        const i32 colour = static_cast<u8>(this->colour.value.x) |
                           (static_cast<u8>(this->colour.value.y) << 8) |
                           (static_cast<u8>(this->colour.value.z) << 16) |
                           (static_cast<i32>(scale.value * 255.0f) << 24);
        NuQFntSetColour(QFont3DZ, colour);
        NuQFntSetScale(QFont3DZ, 0.0375f, 0.05f);
        NuQFntMove(QFont3DZ, NuQFntPrintLenU(QFont3DZ, ASCII_DOWN) * -0.5f, 0.0f, 0.0f);
        NuQFntPrintU(QFont3DZ, ASCII_DOWN);
        NuQFntPopPrintMode();
    }
}

void MechSystems::Render(ThingRenderData *) {
    if (initialized != 0) {
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
            if (Obj[i].touch_task != NULL) {
                Obj[i].touch_task->Render();
            }
        }
        for (i32 i = 0; i < 32; ++i) {
            if (move_to_markers[i] != NULL) {
                move_to_markers[i]->Render();
            }
        }
    }
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->mech_auto_jump_manager != NULL) {
        world->mech_auto_jump_manager->Render();
    }
}

MoveToMarker *MechSystems::NewMoveToMarker(MechObjectInterface &object) {
    if (object.GetObjectType() == 3) {
        return NULL;
    }
    for (i32 i = 0; i < 32; ++i) {
        if (move_to_markers[i] == NULL) {
            MoveToMarker *marker = new MoveToMarker(object);
            move_to_markers[i] = marker;
            return marker;
        }
    }
    return NULL;
}

MoveToMarker *MechSystems::FindMoveToMarkerAtPos(VuVec const &position, bool active_only) {
    for (i32 i = 0; i < 32; ++i) {
        MoveToMarker *marker = move_to_markers[i];
        if (marker == NULL || (active_only && marker->field_108_0)) {
            continue;
        }
        const f32 dx = marker->position.x - position.x;
        const f32 dy = marker->position.y - position.y;
        const f32 dz = marker->position.z - position.z;
        if (dx * dx + dy * dy + dz * dz < 0.4f * 0.4f) {
            return marker;
        }
    }
    return NULL;
}

void MechSystems::NewSwipeMarker(TouchHolder &holder, i32 direction, SwipeDecalRenderer::Style style) {
    for (i32 slot = 0; slot < 4; slot++) {
        if (swipe_markers[slot] == NULL) {
            swipe_markers[slot] = new SwipeDecalRenderer(holder, direction, style);
            return;
        }
    }
}

void MechSystems::NewRadarPulse(VuVec const &position, bool paused) {
    for (i32 slot = 0; slot < 4; slot++) {
        if (radar_pulses[slot] == NULL) {
            HudRadarPulse *pulse = new HudRadarPulse(position);
            radar_pulses[slot] = pulse;
            if (paused) {
                pulse->paused = 1;
            }
            return;
        }
    }
}

MechTouchUITagButton *MechSystems::NewTagButton(GameObject_s &object, TouchHolder &holder) {
    if (level_ui_elements[0] != NULL && level_ui_elements[0]->target_object.Get() == object.GetMechObjectInterface()) {
        return NULL;
    }
    if (level_ui_elements[1] != NULL && level_ui_elements[1]->target_object.Get() == object.GetMechObjectInterface()) {
        return NULL;
    }
    if (level_ui_elements[2] != NULL && level_ui_elements[2]->target_object.Get() == object.GetMechObjectInterface()) {
        return NULL;
    }

    i32 slot;
    for (slot = 0; slot < 3; slot++) {
        if (level_ui_elements[slot] == NULL) {
            break;
        }
    }
    if (slot == 3) {
        return NULL;
    }
    level_ui_elements[slot] = new MechTouchUITagButton(object, holder);
    TouchUI().AddUIElement(*level_ui_elements[slot]);
    return level_ui_elements[slot];
}

void MechSystems::RenderCurrentPlayerHighlight() {
    if (player != NULL && WORLD->lev_objs[166].active != 0) {
        NUVEC scale = {0.6f, 0.6f, 0.6f};
        NUVEC position = player->apiobj.position;
        position.y = GameShadow(player, &position, 2.0f, -1) + 0.001f;

        NUMTX_ALIGNED16 matrix;
        NuMtxSetRotationY(&matrix, 0);
        NuMtxRotateX(&matrix, 0x4000);
        NuMtxScale(&matrix, &scale);
        NuMtxTranslate(&matrix, &position);
        NuSpecialDrawAtAlpha(&WORLD->lev_objs[166].special, &matrix, 0.25f);
    }
}

void MechSystems::HookUpClickToPressStart() {
    gesture_tracking_system.RegisterGestureTracker(ClickToPressStartTracker(), 0);
}

void MechSystems::UnhookClickToPressStart() {
    gesture_tracking_system.UnregisterGestureTracker(ClickToPressStartTracker());
}

