// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"

#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "nu2api/nu3d/nuspline.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/common.h"
#include "nu2api/numath/nuvec.h"
#include <new>
#include "nu2api/numath/nuang.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/items/collect/torpedo.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/world/world_shared.h"
#include <stdio.h>
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "nu2api/nu3d/nurndr.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <stdarg.h>
#include <limits.h>
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/misc/supportall_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "MechInputTouch/MechTouchUIElements.h"
#include "legoapi/actions/character/legoapi_tag.h"
#include "legoapi/actions/character/tagging.h"
#include "nu2api/nuandroid/ios_device.h"

extern "C" i32 CutSceneWaiting;
extern "C" i32 editor_active;
extern "C" i32 PANELOFF;
f32 TagButtonSize = ICONSIZE;
f32 BorderWidth = 0.02f;
f32 RadMult = 0.7f;
f32 GiveUpTime = 4.0f;
extern "C" {
    i32 procActive;
}

bool MechInputTouchGestureTracker::OnDown(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureTracker::OnRelease(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureTracker::OnClick(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureTracker::OnDoubleClick(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureTracker::OnHold(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureTracker::OnSwipe(GameObject_s &, TouchHolder &, i32) {
    return false;
}

void MechTouchUIElement::Process(float) {
}

void MechTouchUIElement::Render() {
}

struct MechTouchUIElement;
struct TouchHolder;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

extern "C" {
    extern "C++" FadeSystem FadeSys;
    extern i32 CutSceneWaiting;
}

struct VuVec;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

// The reference defines these with dynamic initialization in this unit.

static inline void TexQuadSubmit(NUVEC const &point, i32 colour, i32 u, i32 v) {
    TexQuadVertex *vertex = static_cast<TexQuadVertex *>(g_NuPrim_StreamBufferPtr->void_ptr);
    if (!g_NuPrim_NeedsOverbrightening)
        vertex->colour = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    else
        vertex->colour = colour;
    if (g_NuPrim_NeedsHalfUVs) {
        vertex->half_uv[0] = u ? 0x3c00 : 0;
        vertex->half_uv[1] = v ? 0x3c00 : 0;
    } else {
        vertex->uv[0] = static_cast<f32>(u);
        vertex->uv[1] = static_cast<f32>(v);
    }
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * point.x, static_cast<f32>(PS2_VREZ_H) * point.y, 0.0f);
}

static inline void TexQuadSubmit3D(f32 x, f32 y, i32 colour, i32 u, i32 v) {
    VARIPTR *stream = g_NuPrim_StreamBufferPtr;
    TexQuadVertex *vertex = static_cast<TexQuadVertex *>(stream->void_ptr);
    if (g_NuPrim_NeedsOverbrightening == 0) {
        vertex->colour = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    } else {
        vertex->colour = colour;
    }
    if (g_NuPrim_NeedsHalfUVs == 0) {
        vertex->uv[0] = static_cast<f32>(u);
        vertex->uv[1] = static_cast<f32>(v);
    } else {
        vertex->half_uv[0] = u != 0 ? 0x3c00 : 0;
        vertex->half_uv[1] = v != 0 ? 0x3c00 : 0;
    }
    vertex->x = x;
    vertex->y = y;
    vertex->z = 0.0f;
    stream->u8_ptr += sizeof(TexQuadVertex);
    ++g_NuPrim_VertexCount;
}

extern i16 id_GAMORREANGUARD;

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

bool MechTouchUI::OnRelease(GameObject_s &, TouchHolder &holder) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element != NULL && element->owner == &holder) {
            if (element->on_release != NULL && element->disabled == 0) {
                element->on_release(*element, holder);
            }
            elements[i]->owner = NULL;
        }
    }
    return false;
}

bool MechTouchUI::OnDoubleClick(GameObject_s &, TouchHolder &holder) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element != NULL && element->owner == &holder) {
            return true;
        }
    }
    return false;
}

bool MechTouchUI::OnClick(GameObject_s &, TouchHolder &holder) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element != NULL && element->owner == &holder) {
            if (element->hovered != 0 && element->on_click != NULL) {
                if (element->disabled == 0) {
                    element->on_click(*element, holder);
                } else {
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                }
            }
            return true;
        }
    }
    return false;
}

MechTouchUI::MechTouchUI() {
    memset(elements, 0, sizeof(elements));
}

MechTouchUI::~MechTouchUI() {
}

void MechTouchUI::Init() {
    MechSystems *systems = MechSystems::Get();
    systems->gesture_tracking_system.RegisterGestureTracker(*this, 100);
}

bool MechTouchUI::AddUIElement(MechTouchUIElement &element) {
    for (i32 i = 0; i < 32; ++i) {
        if (elements[i] == NULL) {
            elements[i] = &element;
            return true;
        }
    }
    return false;
}

bool MechTouchUI::RemoveUIElement(MechTouchUIElement &element) {
    for (i32 i = 0; i < 32; ++i) {
        if (elements[i] == &element) {
            element.hovered = 0;
            element.owner = NULL;
            elements[i] = NULL;
            return true;
        }
    }
    return false;
}

void MechTouchUI::Render() {
    if (PANELOFF != 0 && GetMenuID() == -1) {
        return;
    }
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element != NULL && element->visible != 0) {
            element->Render();
        }
    }
}

MechTouchUIElement *MechTouchUI::PickElement(NuVec2 &point) {
    float picked_depth = -1000000000.0f;
    MechTouchUIElement *picked = NULL;
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element == NULL || (element->visible == 0 && PANELOFF == 0) || element->disabled != 0) {
            continue;
        }

        bool inside;
        if (element->rectangular == 0) {
            const float y = point.y - element->position.y;
            const float x = point.x - element->position.x;
            inside =
                x * x / (element->radius_x * element->radius_x) + y * y / (element->radius_y * element->radius_y) <=
                1.0f;
        } else {
            const float half_x = element->radius_x * 0.5f;
            const float half_y = element->radius_y * 0.5f;
            inside = element->position.x - half_x <= point.x && point.x <= element->position.x + half_x &&
                     element->position.y - half_y <= point.y && point.y <= element->position.y + half_y;
        }
        if (inside && element->position.z > picked_depth) {
            picked = element;
            picked_depth = element->position.z;
        }
    }
    return picked;
}

bool MechTouchUI::OnHold(GameObject_s &, TouchHolder &holder) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element != NULL && element->owner == &holder) {
            if (PickElement(holder.touch_position) == element) {
                if (elements[i]->on_hold != NULL && elements[i]->disabled == 0) {
                    elements[i]->on_hold(*elements[i], holder);
                }
            }
            holder.consumed = 1;
            return true;
        }
    }
    return false;
}

bool MechTouchUI::OnDown(GameObject_s &, TouchHolder &holder) {
    MechTouchUIElement *element = PickElement(holder.down_position);
    if (element != NULL) {
        element->owner = &holder;
        if (element->on_down != NULL && element->disabled == 0) {
            element->on_down(*element, holder);
        }
    }
    return element != NULL;
}

void MechTouchUI::Process(float dt) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element == NULL) {
            continue;
        }
        element->Process(dt);
        const bool was_hovered = element->hovered != 0;
        element->hovered = element->owner != NULL && PickElement(element->owner->touch_position) == element;
        if (was_hovered && element->hovered == 0 && element->on_leave != NULL && element->owner != NULL) {
            element->on_leave(*element, *element->owner);
        }
    }
}

void MechTouchUITagButton::Render() {
    MechObjectInterface *interface = target_object.Get();
    if (__builtin_expect(interface == NULL || WORLD == NULL, 1)) {
        return;
    }
    GameObject_s *target = interface->GetCharacterObject();
    if (target == NULL) {
        return;
    }

    const f32 circle_x = (position.x + 1.0f) * 0.5f;
    const f32 circle_y = (1.0f - position.y) * 0.5f;
    f32 pulse = 1.3f;
    if (__builtin_expect(hovered == 0, 1)) {
        pulse = (1.0f + NuTrigTable[(static_cast<i32>(timer_animation.value) >> 1) & 0x7fff]) * 0.5f;
        pulse = pulse * (static_cast<i8>(tag_state) > 0 ? 0.7f : 0.35f) + 0.9f;
    }
    const f32 circle_radius = radius_x * RadMult * pulse;
    DrawCharIcon(target->id, position.x, position.y, position.z, TagButtonSize * pulse, 0xa6, first_fade.value,
                 hover_animation.value * first_fade.value, 1, NULL);

    numtl_s *material = MechSystems::Get()->tag_hold_background_material;
    const f32 circle_alpha = second_fade.value;
    const f32 depth = 0.002f + position.z;
    const f32 colour_alpha = first_fade.value * size_animation.value * 128.0f;
    const f32 aspect = GetAspectRatio();
    const i32 colour = (static_cast<i32>(colour_alpha) << 24) | 0x808080;
    RndrUnfilledCircle(circle_x, circle_y, circle_radius, BorderWidth, aspect, colour, circle_alpha, depth, material);
}

void MechTouchUITagButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &) {
    MechTouchUITagButton &button = static_cast<MechTouchUITagButton &>(element);
    MechObjectInterface *interface = button.target_object.Get();
    GameObject_s *target = interface != NULL ? interface->GetCharacterObject() : NULL;
    if (target == NULL || player == NULL || (player->field_0xcc0 != NULL && player->field_0xcc0->id != id_YODA) ||
        button.disabled != 0) {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
        return;
    }

    Hint_SetComplete(0x5f6);
    player->pause_input_state = 0;
    bool tagged = false;
    if (target->can_be_carried) {
        TakeOver2GetIn(target, player);
        tagged = true;
    } else if (TagCode(player, target, 0, 0, 1) == 1) {
        Tag_NewTransfer(player, target);
        tagged = true;
    }
    if (tagged) {
        GameAudio_PlaySfx(0x21, NULL, 0, 0);
    }
    target->pause_input_state = 0;
}

void MechTouchUICharIcon::Render() {
    f32 scale = icon_scale;
    if ((hovered != 0 || selected != 0) && disabled == 0)
        scale *= 1.15f;
    const f32 alpha = icon_alpha;
    const i32 frame = hovered != 0 || selected != 0 ? 0xa6 : 0xa7;
    DrawCharIcon(character_id, position.x, position.y, position.z, scale, frame, alpha, alpha, 1, NULL);
    if (hovered != 0 || selected != 0) {
        f32 width = radius_y * GetAspectRatio();
        f32 x = selector->player_button->position.x + static_cast<f32>(selector->icon_count) * width + width * 0.5f;
        SmartTextEx(TTab[CDataList[character_id].name_id], x, position.y, position.z, 0.4f, 0.4f, 0.4f, 2, 255, 255,
                    255, 0.7f, 1, NULL, 0, static_cast<i32>(alpha * 255.0f));
    }
}

void MechTouchUIPauseButton::Process(float dt) {
    const float next_disable_timer = disable_timer - dt;
    visible = 0;
    if (next_disable_timer < -10.0f) {
        disable_timer = -1.0f;
        disabled = 0;
    } else {
        disable_timer = next_disable_timer;
        disabled = next_disable_timer > 0.0f;
    }

    if (Paused == 0) {
        skip_prompt_timer -= dt;
    }
    if (MechInputTouchMenuController::AnyTouchesThisFrame > 0) {
        skip_prompt_timer = 3.0f;
    }

    if (NewMode != 0 || NewLData != NULL || editor_active != 0 || !(GameTimer.time_elapsed > 0.0f) ||
        GameTimer.update_count == 0 || WORLD == NULL || WORLD->current_level == TITLES_LDATA || CutSceneWaiting != 0) {
        return;
    }

    if (CUTSTOPGAME != 0) {
        if (CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo)) == 0 || !(skip_prompt_timer > 0.0f)) {
            return;
        }
    }

    if (MiniCutCam != 0 || (PANELOFF != 0 && GetMenuID() == -1) || WORLD->current_level == STATUS_LDATA ||
        (WORLD->current_level->flags & LEVEL_STATUS) != 0 || memcard_autosavestarted != 0 ||
        memcard_autosavepostdelay > 0.0f || memcard_autosavepredelay > 0.0f) {
        return;
    }

    visible = 1;
}

void MechTouchUIPartySelector_OnRelease_Callback(MechTouchUIElement &element, TouchHolder &) {
    MechTouchUICharIcon &icon = static_cast<MechTouchUICharIcon &>(element);
    if (icon.selector->field_0x88 != 0) {
        return;
    }
    if (player == NULL || player->field_0xcc0 != NULL || icon.disabled != 0) {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
        return;
    }
    if (icon.hovered == 0) {
        return;
    }

    if (FreePlay != 0) {
        Hint_SetComplete(0x5f5);
        NewPlayerCharacter(player, icon.character_id, player->id, 1);
        GameAudio_PlaySfx(0x24, NULL, 0, 0);
        player->pause_input_state = 0;
        return;
    }

    Hint_SetComplete(0x5f4);
    GameObject_s *target = NULL;
    for (i32 i = 0; i < 8; ++i) {
        if (Player[i] != NULL && Player[i]->id == icon.character_id) {
            target = Player[i];
            break;
        }
    }
    if (target != NULL) {
        bool tagged = false;
        if ((target->field_0xf00 & 2) == 0) {
            if (TagCode(player, target, 0, 0, 1) == 1) {
                Tag_NewTransfer(player, target);
                tagged = true;
            }
        } else {
            TakeOver2GetIn(target, player);
            tagged = true;
        }
        if (tagged) {
            GameAudio_PlaySfx(0x21, NULL, 0, 0);
        }
        target->pause_input_state = 0;
    }
    player->pause_input_state = 0;
}

void MechTouchUITexButton::Process(float) {
    f32 frame_time = FRAMETIME;
    if (!(scale_duration < 0.0f) && !(scale_elapsed >= scale_duration + scale_delay)) {
        f32 elapsed = scale_elapsed + frame_time;
        if (elapsed > scale_duration + scale_delay) {
            elapsed = scale_duration + scale_delay;
            scale_elapsed = elapsed;
        } else {
            scale_elapsed = elapsed;
        }
        if (elapsed >= scale_delay) {
            *scale_target = ((elapsed - scale_delay) / scale_duration) * (scale_to - scale_from) + scale_from;
            frame_time = FRAMETIME;
        }
    }
    if (!(alpha_duration < 0.0f) && __builtin_expect(!(alpha_elapsed >= alpha_duration + alpha_delay), 0)) {
        f32 elapsed = alpha_elapsed + frame_time;
        if (elapsed > alpha_duration + alpha_delay) {
            elapsed = alpha_duration + alpha_delay;
            alpha_elapsed = elapsed;
        } else {
            alpha_elapsed = elapsed;
        }
        if (elapsed >= alpha_delay)
            *alpha_target = ((elapsed - alpha_delay) / alpha_duration) * (alpha_to - alpha_from) + alpha_from;
    }
    visible = alpha > 0.001f;
}

MechTouchUITexButton::~MechTouchUITexButton() {
    NuMtlDestroy(material);
    material = NULL;
}

MechTouchUITagButton::~MechTouchUITagButton() {
}

MechTouchUIPlayerButton::MechTouchUIPlayerButton() : MechTouchUIElement(VuVec(-0.7725f, 0.7525f, 0.0f, 1.0f), 0.16f) {
    rectangular = 0;
    on_click = PlayerButton_OnClick_Callback_NextButton;
    on_hold = PlayerButton_OnHold_Callback;
    chooser_mode = 1;
    selector = NULL;
    on_leave = PlayerButton_OnLeave_Callback;
}

void MechTouchUIPlayerButton::TriggerTagNext() {
    if (FreePlay != 0) {
        for (i32 index = 0; index < 32; ++index) {
            target_ids[index] = -1;
        }
        i32 target_count = 0;
        for (i32 slot = 0; slot < 8; ++slot) {
            GameObject_s *target = Player[slot];
            if (target != NULL && (static_cast<u16>(target->apiobj.field_0x1f8) & 0x1001) == 0x1001) {
                target_ids[target_count++] = target->id;
            }
        }
    }

    GameObject_s *source = player;
    i32 current_index = -1;
    for (i32 index = 0; index < 32; ++index) {
        if (target_ids[index] >= 0 && target_ids[index] == player->id) {
            current_index = index;
            break;
        }
    }
    if (current_index == -1) {
        return;
    }

    i32 index = current_index + 1;
    for (;;) {
        if (index == 32) {
            index = 0;
        }
        if (target_ids[index] >= 0) {
            for (i32 slot = 0; slot < 8; ++slot) {
                GameObject_s *target = Player[slot];
                if (target != NULL) {
                    if (target->id == target_ids[index]) {
                        if (!(!TouchHacks::CanTagTo(*player, *target))) {
                            if (TagCode(player, target, 0, 0, 1) == 1) {
                                GameAudio_PlaySfx(0x21, NULL, 0, 0);
                                Tag_NewTransfer(source, target);
                            }
                            return;
                        }
                    }
                }
            }
        }
        if (index == current_index) {
            return;
        }
        ++index;
        if (index == current_index) {
            return;
        }
    }
}

void PlayerButton_OnClick_Callback_NextButton(MechTouchUIElement &element, TouchHolder &) {
    if (player != NULL && (player->field_0xcc0 == NULL || player->field_0xcc0->id == id_YODA)) {
        static_cast<MechTouchUIPlayerButton &>(element).TriggerTagNext();
    } else {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
    }
}

void MechTouchUIPlayerButton::SetupTargetIds() {
    for (i32 i = 0; i < 32; ++i) {
        target_ids[i] = -1;
        free_play_target_ids[i] = -1;
        field_0x144[i] = 0;
    }

    procActive = 0;
    chooser_mode = 1;

    i32 target_count = 0;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *player = Player[i];
        if (player != NULL && (static_cast<u16>(player->apiobj.field_0x1f8) & 0x1001) == 0x1001) {
            target_ids[target_count++] = player->id;
        }
    }

    if (FreePlay == 0 || apicharsys == NULL || apicharsys->loaded_model_count <= 0) {
        return;
    }

    i32 free_play_count = 0;
    for (i32 i = 0; i < apicharsys->loaded_model_count; ++i) {
        const APICHARACTERMODEL &model = apicharsys->models[i];
        if ((model.flags & 1) == 0) {
            continue;
        }

        const i32 id = model.model_id;
        const i32 collected = InCollectList_Index(id, NULL, 0);
        if (!(collected == -1 && static_cast<i32>(GCDataList[id].flags_090) >= 0 &&
              (GCDataList[id].flags_094[3] & 1) == 0 &&
              !(VehicleArea != 0 && BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 &&
                (GCDataList[id].flags_094[3] & 2) != 0))) {

            if (VehicleArea != 0) {
                const u32 model_flags = CDataList[id].model_flags;
                if ((model_flags & 0x2000) == 0 && !(BonusArea != 0 && (model_flags & 0x4000000) != 0) &&
                    static_cast<i32>(GCDataList[id].flags_090) >= 0 &&
                    !(VehicleArea != 0 && BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 &&
                      (GCDataList[id].flags_094[3] & 2) != 0)) {
                    continue;
                }

                if (BonusArea != 0) {
                    const i32 area = AreaFromMiniKitID(id);
                    if (area != -1) {
                        if (Game_AreaSave == NULL || Game_AreaSave[area].minikit_complete == 0) {
                            continue;
                        }
                    } else if (static_cast<i32>(GCDataList[id].flags_090) >= 0 &&
                               !(VehicleArea != 0 && BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 &&
                                 (GCDataList[id].flags_094[3] & 2) != 0)) {
                        continue;
                    }
                }
            } else if ((CDataList[id].model_flags & 0x2000) == 0 && (GCDataList[id].flags_090 & 0x40) == 0) {

                if (BonusArea == 0 || VehicleArea == 0) {
                    if (!(static_cast<i32>(GCDataList[id].flags_090) < 0)) {
                        if ((GCDataList[id].flags_094[3] & 1) == 0) {
                            if (collected == -1 || Collection_Got(id) == 0) {
                                continue;
                            }
                        }
                    } else {
                        if (Cheats_CheckFlags(0x100) == 0) {
                            continue;
                        }
                    }
                }

                free_play_target_ids[free_play_count++] = id;
                if (free_play_count == 32) {
                    return;
                }
            }
        }
    }
}

MechTouchUICharIcon::MechTouchUICharIcon(MechTouchUIPartySelector &party, VuVec const &pos, i32 id, float scale)
    : MechTouchUIElement(pos, scale), character_id(id), icon_scale(scale), alpha_target(&icon_alpha),
      alpha_duration(-1.0f), selector(&party) {
    alpha_elapsed = 0.0f;
    field_0x45 = 0;
    alpha_delay = 0.0f;
    selected = 0;
    icon_alpha = 0.0f;
    rectangular = 1;
    alpha_end = 0.0f;
    field_0x46 = 0;
    alpha_start = 0.0f;
}

void MechTouchUICharIcon::SetupDisabled() {
    disabled = 0;
    if (FreePlay != 0) {
        disabled = player == NULL || !TouchHacks::CanToggleTo(*player, character_id);
        return;
    }

    disabled = 1;
    if (player == NULL || player->field_0xcc0 != NULL) {
        return;
    }
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *target = Player[i];
        if (target != NULL && target->id == character_id) {
            disabled = !TouchHacks::CanTagTo(*player, *target);
        }
    }
}

void MechTouchUICharIcon::Process(float) {
    if (selector->icon_count == 1) {
        radius_x = 10.0f;
    }

    u8 is_hovered = hovered != 0;
    if (is_hovered && field_0x46 == 0) {
        PlaySfx(const_cast<char *>("LegoClicks2"), NULL);
        is_hovered = hovered;
    }
    field_0x46 = is_hovered;

    const f32 dt = FRAMETIME;
    if (!(alpha_duration < 0.0f) && !(alpha_elapsed >= alpha_duration + alpha_delay)) {
        f32 elapsed = alpha_elapsed + dt;
        if (elapsed > alpha_duration + alpha_delay) {
            elapsed = alpha_duration + alpha_delay;
        }
        alpha_elapsed = elapsed;
        if (elapsed >= alpha_delay) {
            *alpha_target = ((elapsed - alpha_delay) / alpha_duration) * (alpha_end - alpha_start) + alpha_start;
        }
    }

    visible = 0;
    if (player == NULL) {
        return;
    }
    if (NewMode == 0 && NewLData == NULL && editor_active == 0 && GameTimer.time_elapsed > 0.0f &&
        GameTimer.update_count != 0 && WORLD != NULL && WORLD->current_level != TITLES_LDATA && Paused == 0 &&
        GameMenu[GameMenuLevel].menu == -1 && CutSceneWaiting == 0 && CUTSTOPGAME == 0 && MiniCutCam == 0) {
        visible = 1;
    }

    const u8 was_disabled = disabled;
    SetupDisabled();
    if (field_0x45 == 0 && was_disabled != disabled) {
        if (alpha_duration < 0.0f || alpha_elapsed >= alpha_duration + alpha_delay) {
            const f32 destination = disabled != 0 ? 0.3f : 1.0f;
            const f32 start = *alpha_target;
            alpha_elapsed = 0.0f;
            alpha_duration = 0.4f;
            alpha_delay = 0.0f;
            alpha_start = start;
            alpha_end = destination;
        } else {
            alpha_end = disabled == 0 ? 1.0f : 0.3f;
        }
    }
}

void MechTouchUIPartySelector::Cleanup() {
    for (i32 i = 0; i < 32; ++i) {
        if (icons[i] != NULL) {
            MechSystems::Get()->TouchUI().RemoveUIElement(*icons[i]);
            delete icons[i];
            icons[i] = NULL;
        }
    }
}

MechTouchUIPartySelector::~MechTouchUIPartySelector() {
    Cleanup();
}

MechTouchUIPartySelector::MechTouchUIPartySelector(MechTouchUIPlayerButton &button, i32 *target_ids)
    : icon_count(0), player_button(&button), field_0x88(0) {
    memset(icons, 0, sizeof(icons));

    Cleanup();
    const VuVec button_position = button.position;
    const f32 button_bottom = button.position.y - button.radius_y;
    const bool small_screen = NuIOS_IsSmallScreen() != 0;
    const f32 scale = small_screen ? 0.16f : 0.15f;
    const f32 gap = small_screen ? 0.11f : 0.06f;
    const i32 icons_per_row = small_screen ? 5 : 7;
    const f32 first_y = (button_bottom - gap) - gap;
    f32 x = button_position.x;
    f32 y = first_y;
    f32 delay = 0.0f;
    f32 delay_step = 0.03f;
    i32 icon_index = 0;
    i32 column = 0;

    for (i32 target_index = 0; target_index < 32; ++target_index) {
        if (target_ids[target_index] < 0 || player == NULL) {
            continue;
        }
        if (column == 0) {
            ++icon_count;
        }

        MechTouchUICharIcon *icon = new MechTouchUICharIcon(*this, ::VuVec_Zero, target_ids[target_index], scale);
        icons[icon_index] = icon;
        icon->on_release = MechTouchUIPartySelector_OnRelease_Callback;
        icon->owner = button.owner;
        icon->position = VuVec(x, y, button_position.z, button_position.w);
        icon->SetupDisabled();
        icon = icons[icon_index];
        icon->alpha_end = icon->disabled != 0 ? 0.3f : 1.0f;
        icon->alpha_delay = delay;
        icon->alpha_start = 0.0f;
        icon->alpha_elapsed = 0.0f;
        icon->alpha_duration = 0.3f;
        *icon->alpha_target = 0.0f;
        ++column;
        MechSystems::Get()->TouchUI().AddUIElement(*icon);
        ++icon_index;
        if (!(column >= icons_per_row)) {
            y -= scale + gap;
            delay += delay_step;
            delay_step *= 0.9f;
        } else {
            column = 0;
            x += GetAspectRatio() * (scale + gap);
            y = first_y;
            delay = 0.0f;
            delay_step = 0.03f;
        }
    }
}

void MechTouchUIPlayerButton::ShowChooser() {
    if (FreePlay == 0 && target_ids[1] == -1) {
        return;
    }
    if (selector != NULL) {
        delete selector;
        selector = NULL;
    }
    selector = new MechTouchUIPartySelector(*this, FreePlay != 0 ? free_play_target_ids : target_ids);
    GameAudio_PlaySfx(0x30, NULL, 0, 0);
}

void PlayerButton_OnLeave_Callback(MechTouchUIElement &element, TouchHolder &holder) {
    MechTouchUIPlayerButton &button = static_cast<MechTouchUIPlayerButton &>(element);
    if (player != NULL && (player->field_0xcc0 == NULL || player->field_0xcc0->id == id_YODA) &&
        button.selector == NULL && holder.touch_position.y <= button.position.y &&
        holder.touch_position.y != button.position.y) {
        button.ShowChooser();
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
    }
}

void PlayerButton_OnHold_Callback(MechTouchUIElement &element, TouchHolder &) {
    if (player != NULL && (player->field_0xcc0 == NULL || player->field_0xcc0->id == id_YODA)) {
        static_cast<MechTouchUIPlayerButton &>(element).ShowChooser();
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
    } else {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
    }
}

void MechTouchUIPartySelector::BlendOut() {
    field_0x88 = 1;
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUICharIcon *icon = icons[i];
        if (icon != NULL) {
            icon->field_0x45 = 1;
            bool selected = icon->hovered != 0 && icon->disabled == 0;
            f32 delay = selected ? 0.4f : 0.0f;
            icon->selected = selected;
            icon->alpha_start = *icon->alpha_target;
            icon->alpha_end = 0.0f;
            icon->alpha_elapsed = 0.0f;
            icon->alpha_duration = 0.3f;
            icon->alpha_delay = delay;
        }
    }
}

bool MechTouchUIPartySelector::BlendedOut() {
    if (field_0x88 == 0)
        return false;
    for (i32 i = 0; i < 32; ++i) {
        if (icons[i] != NULL && icons[i]->icon_alpha > 0.001f)
            return false;
    }
    return true;
}

void MechTouchUIPlayerButton::Process(float) {
    disabled = GetMenuID() != -1 || Paused != 0 || NewMode != 0 || NewLData != NULL || CUTSTOPGAME != 0 ||
               MiniCutCam != 0 || FadeSys.fade > 0.0f || player == NULL;

    if (selector != NULL && owner == NULL && selector->field_0x88 == 0) {
        selector->BlendOut();
    }
    if (selector != NULL && selector->BlendedOut()) {
        delete selector;
        selector = NULL;
    }
    if (disabled) {
        return;
    }

    if (chooser_mode != 0) {
        if (player == NULL) {
            return;
        }
        if (procActive++ <= 20) {
            return;
        }
        chooser_mode = 0;
        for (i32 target_index = 0; target_index < 32; ++target_index) {
            for (i32 slot = 0; slot < 8; ++slot) {
                GameObject_s *target = Player[slot];
                if (target != NULL && target->id == target_ids[target_index]) {
                    field_0x144[target_index] = target == player || TouchHacks::CanTagTo(*player, *target);
                    break;
                }
            }
        }
        return;
    }

    if (player == NULL) {
        return;
    }
    const i32 update = procActive++;
    if (update % 5 != 0) {
        return;
    }
    for (i32 target_index = 0; target_index < 32; ++target_index) {
        if (field_0x144[target_index] != 0) {
            continue;
        }
        for (i32 slot = 0; slot < 8; ++slot) {
            GameObject_s *target = Player[slot];
            if (target != NULL && target->id == target_ids[target_index]) {
                if (TouchHacks::CanTagTo(*player, *target)) {
                    field_0x144[target_index] = 1;
                    MechSystems::Get()->NewRadarPulse(position, false);
                    break;
                }
            }
        }
    }
}

void PerformPauseButtonStuff() {
    if (WORLD == NULL) {
        return;
    }
    if (Paused != 0) {
        BackupMenu();
        GameAudio_PlaySfx(0x31, NULL, 0, 0);
        if (GameMenuLevel == 0 && Paused != 0) {
            ResumeGame(0, 1);
        }
        return;
    }
    if (__builtin_expect(GameMenuLevel == 0, 0)) {
        PauseGame(0);
        return;
    }

    if (GetMenuID() == 12) {
        CharacterCustomiser->field_0xd17 = 1;
        return;
    }

    const i32 menu_id = GetMenuID();
    if (menu_id == 13)
        goto close_menu;
    if (menu_id == 1)
        goto close_menu;
    if (menu_id == 8)
        goto close_menu;
    if (menu_id == 17)
        goto close_menu;
    if ((menu_id & ~2) == 16)
        goto close_menu;
    if (static_cast<u32>(menu_id - 14) <= 1)
        goto close_menu;
    if (static_cast<u32>(menu_id - 20) <= 1)
        goto close_menu;
    if (menu_id == 22)
        goto close_menu;
    if (menu_id == 1000)
        goto close_menu;
    if (menu_id == 33)
        goto close_menu;
    if ((menu_id & ~8) == 1008)
        goto close_menu;
    if (static_cast<u32>(menu_id - 1012) <= 1)
        goto close_menu;
    if (menu_id == 1017)
        goto close_menu;
    {
        MechInputTouchMainController *controller = MechSystems::Get()->active_main_controller;
        if (controller != NULL) {
            controller->button_pressed[3] = 1;
        }
    }
    return;

close_menu:
    GameMenu[GameMenuLevel].close_requested = 1;
}

void MechTouchUIPauseButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &) {
    static_cast<MechTouchUIPauseButton &>(element).disable_timer = 0.5f;
    PerformPauseButtonStuff();
}

MechTouchUIPauseButton::MechTouchUIPauseButton() : MechTouchUIElement(VuVec(0.7725f, 0.7525f, 0.0f, 1.0f), 0.16f) {
    on_click = MechTouchUIPauseButton_OnClick_Callback;
    disable_timer = 0.0f;
    skip_prompt_timer = 0.0f;
}

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large) {
    (void)unused_label;
    float text_x_offset;
    float text_y_offset;

    // Retained from the original routine even though the result is unused.
    NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);

    if (NuStrICmp(prompt, ">") == 0) {
        text_x_offset = 0.005f;
        text_y_offset = 0.0f;
    } else if (NuStrICmp(prompt, "<<") == 0) {
        text_x_offset = -0.01f;
        text_y_offset = 0.0f;
    } else if (NuStrICmp(prompt, "II") == 0) {
        text_x_offset = 0.0f;
        text_y_offset = -0.01f;
    } else {
        text_x_offset = 0.0f;
        text_y_offset = 0.0f;
    }

    const float zero = 0.0f;
    float pulse = 1.0f;
    if (TestForController() == 0) {
        const float elapsed_since_touch = GlobalTimer.time_elapsed - (LastTouchTime + zero);
        if (elapsed_since_touch > 4.0f) {
            const float phase = NuFmod(elapsed_since_touch, 4.0f);
            const i32 angle = static_cast<i32>(phase * 0.25f * 65536.0f);
            const float wave = NuTrigTable[(angle >> 1) & 0x7fff] - 0.8f;
            if (!(wave < zero)) {
                pulse = wave + 1.0f;
            }
        }
    }

    float icon_scale = ICONSIZE;
    if (!hovered) {
        if (!large) {
            DrawPanel3DObject(ICONX, STATSPOSY, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0,
                              &WORLD->lev_objs[0xa5].special, 0, 0.75f);
            pulse = 0.6f;
        } else {
            icon_scale *= pulse;
            DrawPanel3DObject(ICONX, STATSPOSY, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0,
                              &WORLD->lev_objs[0xa5].special, 0, 0.75f);
            pulse *= 0.6f;
        }
    } else {
        icon_scale *= 1.25f;
        DrawPanel3DObject(ICONX, STATSPOSY, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0,
                          &WORLD->lev_objs[0xa5].special, 0, 0.75f);
        pulse = 0.75f;
    }

    if (prompt != NULL) {
        Text3DEx(prompt, ICONX + text_x_offset * pulse, STATSPOSY + text_y_offset * pulse, 1.0f, pulse * 1.3f, pulse,
                 pulse, 0, 255, 255, 255, 0x60);
    }
}

void MechTouchUIPauseButton::Render() {
    if (NewMode != 0 || NewLData != NULL || editor_active != 0 || !(GameTimer.time_elapsed > 0.0f) ||
        GameTimer.update_count == 0 || WORLD == NULL || WORLD->current_level == TITLES_LDATA ||
        WORLD->current_level == CREDITS_LDATA) {
        return;
    }

    if (Paused != 0) {
        if (GetMenuID() == LEGO_MENU_PAUSE_MAIN || GetMenuID() == LEGO_MENU_PAUSE_CUTSCENE) {
            DrawTouchPrompt(const_cast<char *>(">"), NULL, hovered != 0, true);
        } else {
            DrawTouchPrompt(const_cast<char *>("<<"), NULL, hovered != 0, true);
        }
        return;
    }

    if (GameMenu[GameMenuLevel].menu != -1 || CutSceneWaiting != 0 ||
        (CUTSTOPGAME != 0 && CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo)) == 0) || MiniCutCam != 0 ||
        memcard_autosavestarted != 0 || memcard_autosavepostdelay > 0.0f || memcard_autosavepredelay > 0.0f) {
        if (GetMenuID() != -1) {
            DrawTouchPrompt(const_cast<char *>("<<"), NULL, hovered != 0, true);
        }
        return;
    }

    DrawTouchPrompt(const_cast<char *>("II"), NULL, hovered != 0, false);
}

void RndrTexQuad(f32 x, f32 y, f32 width, f32 height, i32 colour, numtl_s *material, i32 angle) {
    NUVEC points[4] = {};
    points[0].x = -0.5f;
    points[0].y = -0.5f;
    points[1].x = 0.5f;
    points[1].y = -0.5f;
    points[2].x = -0.5f;
    points[2].y = 0.5f;
    points[3].x = 0.5f;
    points[3].y = 0.5f;
    NuVecRotateZ(&points[0], &points[0], angle);
    NuVecRotateZ(&points[1], &points[1], angle);
    NuVecRotateZ(&points[2], &points[2], angle);
    NuVecRotateZ(&points[3], &points[3], angle);
    points[0].x *= width;
    points[0].y *= height;
    points[1].x *= width;
    points[1].y *= height;
    points[2].x *= width;
    points[2].y *= height;
    points[3].x *= width;
    points[3].y *= height;
    points[0].x += x;
    points[0].y += y;
    points[1].x += x;
    points[1].y += y;
    points[2].x += x;
    points[2].y += y;
    points[3].x += x;
    points[3].y += y;
    NuPrim2DBegin(1, 7, material);
    TexQuadSubmit(points[0], colour, 0, 0);
    TexQuadSubmit(points[1], colour, 1, 0);
    TexQuadSubmit(points[2], colour, 0, 1);
    TexQuadSubmit(points[3], colour, 1, 1);
    NuPrim2DEnd();
}

void MechTouchUITexButton::Render() {
    RndrTexQuad((position.x + 1.0f) * 0.5f, (1.0f - position.y) * 0.5f, scale * radius_x, radius_y * scale,
                static_cast<i32>((static_cast<u32>(static_cast<i32>(alpha * 128.0f)) << 24) | 0x808080), material, 0);
}

void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material) {
    NuPrim3DBegin(1, 7, material, reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(&matrix)));
    TexQuadSubmit3D(-0.5f, -0.5f, colour, 0, 0);
    TexQuadSubmit3D(0.5f, -0.5f, colour, 1, 0);
    TexQuadSubmit3D(-0.5f, 0.5f, colour, 0, 1);
    TexQuadSubmit3D(0.5f, 0.5f, colour, 1, 1);
    NuPrim3DEnd();
}

MechTouchUITexButton::MechTouchUITexButton(VuVec const &pos, float radius) : MechTouchUIElement(pos, radius) {
    rectangular = 0;
    alpha_target = &alpha;
    alpha_elapsed = 0.0f;
    alpha_duration = -1.0f;
    alpha_delay = 0.0f;
    scale_target = &scale;
    scale_elapsed = 0.0f;
    scale_duration = -1.0f;
    scale_delay = 0.0f;
    alpha = alpha_to = alpha_from = 1.0f;
    scale = scale_to = scale_from = 1.0f;
    material = NuMtlCreate(1);
    material->attribs.cull_mode = 2;
    material->attribs.z_mode = 1;
    material->attribs.alpha_mode = 1;
    material->attribs.unknown_2_1_2 = 2;
    material->attribs.alpha_test = 1;
    material->diffuse_color.r = 0.0f;
    material->diffuse_color.g = 0.0f;
    material->diffuse_color.b = 0.0f;
    material->opacity = 0.0f;
    material->sort_pri = 255;
}

void MechTouchUITexButton::UpdateTexture(i16 texture) {
    material->tex_id = texture;
    NuMtlUpdate(material);
}

MechTouchUITagButton::MechTouchUITagButton(GameObject_s &object, TouchHolder &holder)
    : MechTouchUIElement(VuVec(object.camera_screen_position.x, object.camera_screen_position.y,
                               object.camera_screen_position.z, 1.0f),
                         TagButtonSize),
      target_object(object.GetMechObjectInterface()), touch_holder(&holder) {
    position.z = 0.0f;
    tag_state = 2;
    fading_out = 0;
    touched = 0;
    enabled = 1;
    rectangular = 0;

    first_fade.from = 0.0f;
    first_fade.to = 1.0f;
    first_fade.duration = 0.3f;
    first_fade.value = 0.0f;

    hover_animation.to = 0.0f;
    hover_animation.elapsed = -1.0f;
    hover_animation.value = 0.0f;

    size_animation.to = 1.0f;
    size_animation.elapsed = -1.0f;
    size_animation.value = 1.0f;

    second_fade.from = 0.0f;
    second_fade.to = 1.0f;
    second_fade.duration = 0.8f;
    second_fade.value = 0.0f;
    field_0xbc = 0.0f;

    timer_animation.to = 0.0f;
    timer_animation.elapsed = -1.0f;
    timer_animation.value = 0.0f;
    timer = 1.0f;
}

void MechTouchUITagButton::FadeOut() {
    if (fading_out == 0) {
        fading_out = 1;
        if (1.0f > second_fade.value) {
            second_fade.from = *second_fade.target;
            second_fade.to = 0.0f;
            second_fade.elapsed = 0.0f;
            second_fade.duration = 0.2f;
            second_fade.delay = 0.0f;
        }
        first_fade.from = *first_fade.target;
        first_fade.to = 0.0f;
        first_fade.elapsed = 0.0f;
        first_fade.duration = 0.2f;
        first_fade.delay = 0.0f;
    }
}

void MechTouchUITagButton::Process(float dt) {
    MechObjectInterface *interface = target_object.Get();
    GameObject_s *target = interface != NULL ? interface->GetCharacterObject() : NULL;
    if (target == NULL) {
        first_fade.value = 0.0f;
        first_fade.to = 0.0f;
        first_fade.elapsed = first_fade.duration;
        fading_out = 1;
        visible = 0;
        return;
    }
    if (player == NULL) {
        FadeOut();
        return;
    }

    if (touch_holder->is_down == 0 && (second_fade.value < 1.0f || (enabled != 0 && touched != 0))) {
        FadeOut();
    }

    if (!second_fade.IsActive() && on_click == NULL) {
        on_click = MechTouchUITagButton_OnClick_Callback;
        on_click(*this, *touch_holder);
        FadeOut();
    }

    if (second_fade.IsActive()) {
        timer = GiveUpTime;
    }
    timer -= dt;
    if (timer <= 0.0f) {
        FadeOut();
    }
    if (on_click != NULL) {
        field_0xbc -= dt;
        if (field_0xbc < 0.0f) {
            --tag_state;
            field_0xbc = 0.75f;
            timer_animation.from = 0.0f;
            timer_animation.to = 32768.0f;
            timer_animation.elapsed = 0.0f;
            timer_animation.duration = 0.5f;
            timer_animation.delay = 0.0f;
            *timer_animation.target = 0.0f;
        }
    }

    if (!TouchHacks::CanTagTo(*player, *target)) {
        FadeOut();
    }

    enabled = touch_holder->is_down;
    touched |= !touch_holder->is_down;
    size_animation.Process(dt);
    first_fade.Process(dt);
    second_fade.Process(dt);
    hover_animation.Process(dt);
    timer_animation.Process(dt);
    disabled = !(second_fade.value >= 1.0f);

    NUVEC world_position = {
        target->apiobj.collision_position.x,
        target->apiobj.position.y + target->character_top * target->apiobj.field_0xa8 + 0.1f,
        target->apiobj.collision_position.z,
    };
    NUVEC screen_position;
    NuCameraTransformScreenClip(&screen_position, &world_position, 1, NULL);

    const f32 lower = TagButtonSize - 1.0f;
    const f32 upper = 1.0f - TagButtonSize;
    position.x = MAX(lower, MIN(screen_position.x, upper));
    position.y = MAX(lower, MIN(screen_position.y, upper));
}

bool ClickToPressStartGestureTracker::OnClick(GameObject_s &, TouchHolder &) {
    GamePad[0].buttons_down_08 |= GAMEPAD_START;
    MechSystems::SkipTextScroll = 1;
    return true;
}

