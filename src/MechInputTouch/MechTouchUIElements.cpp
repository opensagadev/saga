#include "decomp.h"
#include "MechInputTouch_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"

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

void RndrTexQuad(f32, f32, f32, f32, i32, numtl_s *, i32);
i32 RndrUnfilledCircle(f32, f32, f32, f32, f32, i32, f32, f32, numtl_s *);

extern i32 CutSceneWaiting;
extern i32 editor_active;
extern i32 NewMode;
extern i32 PANELOFF;
extern i32 Paused;
extern FadeSystem FadeSys;
f32 TagButtonSize;
f32 BorderWidth = 0.02f;
f32 RadMult = 0.7f;
f32 GiveUpTime = 4.0f;
extern "C" {
    i32 procActive;
}

i32 GetMenuID();
float GetAspectRatio();
void PlayerButton_OnClick_Callback_NextButton(MechTouchUIElement &, TouchHolder &);
void PlayerButton_OnHold_Callback(MechTouchUIElement &, TouchHolder &);
void PlayerButton_OnLeave_Callback(MechTouchUIElement &, TouchHolder &);
void MechTouchUIPauseButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);
void MechTouchUIPartySelector_OnRelease_Callback(MechTouchUIElement &, TouchHolder &);
void MechTouchUITagButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);
extern "C" i32 NuIOS_IsSmallScreen(void);
extern i16 id_YODA;
i32 TagCode(GameObject_s *, GameObject_s *, i32, i32, i32);
void Tag_NewTransfer(GameObject_s *, GameObject_s *);

bool MechInputTouchGestureTracker::OnDown(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureTracker::OnRelease(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureTracker::OnClick(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureTracker::OnDoubleClick(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureTracker::OnHold(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchGestureTracker::OnSwipe(GameObject_s &, TouchHolder &, i32) {
    STUBBED();
    return false;
}

void MechTouchUIElement::Process(float) {
}

void MechTouchUIElement::Render() {
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

void MechTouchUI::Init() {
    MechSystems *systems = MechSystems::Get();
    systems->gesture_tracking_system.RegisterGestureTracker(*this, 100);
}

MechTouchUI::MechTouchUI() {
    memset(elements, 0, sizeof(elements));
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

bool MechTouchUI::OnDoubleClick(GameObject_s &, TouchHolder &holder) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element != NULL && element->owner == &holder) {
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

bool MechTouchUI::OnHold(GameObject_s &, TouchHolder &holder) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element != NULL && element->owner == &holder) {
            if (PickElement(holder.touch_position) == element && elements[i]->on_hold != NULL &&
                elements[i]->disabled == 0) {
                elements[i]->on_hold(*elements[i], holder);
            }
            holder.field_0x0[7] = 1;
            return true;
        }
    }
    return false;
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

MechTouchUIElement *MechTouchUI::PickElement(NuVec2 &point) {
    MechTouchUIElement *picked = NULL;
    float picked_depth = -1000000000.0f;
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element == NULL || (element->visible == 0 && PANELOFF == 0) || element->disabled != 0) {
            continue;
        }

        if (element->rectangular == 0) {
            const float x = point.x - element->position.x;
            const float y = point.y - element->position.y;
            if (1.0f <
                x * x / (element->radius_x * element->radius_x) + y * y / (element->radius_y * element->radius_y)) {
                continue;
            }
        } else {
            const float half_x = element->radius_x * 0.5f;
            const float half_y = element->radius_y * 0.5f;
            if (point.x < element->position.x - half_x || point.x > element->position.x + half_x ||
                point.y < element->position.y - half_y || point.y > element->position.y + half_y) {
                continue;
            }
        }
        if (element->position.z > picked_depth) {
            picked = element;
            picked_depth = element->position.z;
        }
    }
    return picked;
}

void MechTouchUI::Process(float dt) {
    for (i32 i = 0; i < 32; ++i) {
        MechTouchUIElement *element = elements[i];
        if (element == NULL) {
            continue;
        }
        element->Process(dt);
        const bool was_hovered = element->hovered != 0;
        if (element->owner != NULL && PickElement(element->owner->touch_position) == element) {
            element->hovered = 1;
        } else {
            element->hovered = 0;
            if (was_hovered && element->on_leave != NULL && element->owner != NULL) {
                element->on_leave(*element, *element->owner);
            }
        }
    }
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

MechTouchUI::~MechTouchUI() {
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

void MechTouchUICharIcon::Process(float) {
    if (selector->field_0x88 != 0) {
        position.z = 10.0f;
    }

    const bool is_hovered = hovered != 0;
    if (is_hovered && field_0x46 == 0) {
        PlaySfx(const_cast<char *>("LegoClicks2"), NULL);
    }
    field_0x46 = is_hovered;

    if (alpha_duration >= 0.0f && alpha_elapsed < alpha_duration + alpha_delay) {
        alpha_elapsed = MIN(alpha_elapsed + FRAMETIME, alpha_duration + alpha_delay);
        if (alpha_elapsed >= alpha_delay) {
            *alpha_target = ((alpha_elapsed - alpha_delay) / alpha_duration) * (alpha_end - alpha_start) + alpha_start;
        }
    }

    visible = player != NULL && NewMode == 0 && NewLData == NULL && editor_active == 0 &&
              GameTimer.time_elapsed > 0.0f && GameTimer.update_count != 0 && WORLD != NULL &&
              WORLD->current_level != TITLES_LDATA && Paused == 0 && GameMenu[GameMenuLevel].menu == -1 &&
              CutSceneWaiting == 0 && CUTSTOPGAME == 0 && MiniCutCam == 0;
    if (player == NULL) {
        return;
    }

    const bool was_disabled = disabled != 0;
    SetupDisabled();
    if (field_0x45 == 0 && was_disabled != (disabled != 0)) {
        const f32 destination = disabled != 0 ? 0.3f : 1.0f;
        if (alpha_duration < 0.0f || alpha_elapsed >= alpha_duration + alpha_delay) {
            alpha_start = *alpha_target;
            alpha_elapsed = 0.0f;
            alpha_duration = 0.4f;
            alpha_delay = 0.0f;
        }
        alpha_end = destination;
    }
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
            return;
        }
    }
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

MechTouchUITagButton::MechTouchUITagButton(GameObject_s &object, TouchHolder &holder)
    : MechTouchUIElement(VuVec(object.camera_screen_position.x, object.camera_screen_position.y,
                               object.camera_screen_position.z, 1.0f),
                         TagButtonSize),
      target_object(object.GetMechObjectInterface()), touch_holder(&holder) {
    position.z = 0.0f;
    rectangular = 0;
    tag_state = 2;
    fading_out = 0;
    touched = 0;
    enabled = 1;

    first_fade.target = &first_fade.value;
    first_fade.from = 0.0f;
    first_fade.to = 1.0f;
    first_fade.elapsed = 0.0f;
    first_fade.duration = 0.3f;
    first_fade.delay = 0.0f;
    first_fade.value = 0.0f;

    hover_animation.target = &hover_animation.value;
    hover_animation.to = 0.0f;
    hover_animation.elapsed = -1.0f;
    hover_animation.duration = -1.0f;
    hover_animation.delay = 0.0f;
    hover_animation.value = 0.0f;

    size_animation.target = &size_animation.value;
    size_animation.to = 1.0f;
    size_animation.elapsed = -1.0f;
    size_animation.duration = -1.0f;
    size_animation.delay = 0.0f;
    size_animation.value = 1.0f;

    second_fade.target = &second_fade.value;
    second_fade.from = 0.0f;
    second_fade.to = 1.0f;
    second_fade.elapsed = 0.0f;
    second_fade.duration = 0.8f;
    second_fade.delay = 0.0f;
    second_fade.value = 0.0f;
    field_0xbc = 0.0f;

    timer_animation.target = &timer_animation.value;
    timer_animation.to = 0.0f;
    timer_animation.elapsed = -1.0f;
    timer_animation.duration = -1.0f;
    timer_animation.delay = 0.0f;
    timer_animation.value = 0.0f;
    timer = 1.0f;
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
        timer = GiveUpTime - dt;
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
    disabled = second_fade.value < 1.0f;

    NUVEC world_position = {
        target->apiobj.collision_position.x,
        target->apiobj.position.y + target->character_top * target->apiobj.field_0xa8 + 0.1f,
        target->apiobj.collision_position.z,
    };
    NUVEC screen_position;
    NuCameraTransformScreenClip(&screen_position, &world_position, 1, NULL);

    const f32 lower = TagButtonSize - 1.0f;
    const f32 upper = 1.0f - TagButtonSize;
    position.x = screen_position.x < lower ? lower : (screen_position.x > upper ? upper : screen_position.x);
    position.y = screen_position.y < lower ? lower : (screen_position.y > upper ? upper : screen_position.y);
}

void MechTouchUITagButton::Render() {
    MechObjectInterface *interface = target_object.Get();
    if (interface == NULL || WORLD == NULL) {
        return;
    }
    GameObject_s *target = interface->GetCharacterObject();
    if (target == NULL) {
        return;
    }

    f32 circle_radius = radius_x * RadMult;
    f32 icon_radius = TagButtonSize;
    if (hovered != 0) {
        circle_radius *= 1.3f;
        icon_radius *= 1.3f;
    } else {
        f32 pulse = (1.0f + NuTrigTable[(static_cast<i32>(timer_animation.value) >> 1) & 0x7fff]) * 0.5f;
        pulse = pulse * (tag_state > 0 ? 0.7f : 0.35f) + 0.9f;
        circle_radius *= pulse;
        icon_radius *= pulse;
    }

    DrawCharIcon(target->id, position.x, position.y, position.z, icon_radius, 0xa6, first_fade.value,
                 hover_animation.value * first_fade.value, 1, NULL);

    const i32 colour = (static_cast<i32>(first_fade.value * size_animation.value * 128.0f) << 24) | 0x808080;
    RndrUnfilledCircle((position.x + 1.0f) * 0.5f, (1.0f - position.y) * 0.5f, circle_radius, BorderWidth,
                       GetAspectRatio(), colour, second_fade.value, position.z + 0.002f,
                       MechSystems::Get()->tag_hold_background_material);
}

MechTouchUITagButton::~MechTouchUITagButton() {
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

void MechTouchUITexButton::Process(float) {
    f32 frame_time = FRAMETIME;
    if (!(scale_duration < 0.0f) && !(scale_elapsed >= scale_duration + scale_delay)) {
        scale_elapsed += frame_time;
        if (scale_elapsed > scale_duration + scale_delay)
            scale_elapsed = scale_duration + scale_delay;
        if (scale_elapsed >= scale_delay)
            *scale_target = ((scale_elapsed - scale_delay) / scale_duration) * (scale_to - scale_from) + scale_from;
    }
    if (!(alpha_duration < 0.0f) && !(alpha_elapsed >= alpha_duration + alpha_delay)) {
        alpha_elapsed += frame_time;
        if (alpha_elapsed > alpha_duration + alpha_delay)
            alpha_elapsed = alpha_duration + alpha_delay;
        if (alpha_elapsed >= alpha_delay)
            *alpha_target = ((alpha_elapsed - alpha_delay) / alpha_duration) * (alpha_to - alpha_from) + alpha_from;
    }
    visible = alpha > 0.001f;
}

void MechTouchUITexButton::Render() {
    RndrTexQuad((position.x + 1.0f) * 0.5f, (1.0f - position.y) * 0.5f, scale * radius_x, radius_y * scale,
                static_cast<i32>((static_cast<u32>(static_cast<i32>(alpha * 128.0f)) << 24) | 0x808080), material, 0);
}

void MechTouchUITexButton::UpdateTexture(i16 texture) {
    material->tex_id = texture;
    NuMtlUpdate(material);
}

MechTouchUITexButton::~MechTouchUITexButton() {
    NuMtlDestroy(material);
    material = NULL;
}

MechTouchUIPauseButton::MechTouchUIPauseButton() : MechTouchUIElement(VuVec(0.7725f, 0.7525f, 0.0f, 1.0f), 0.16f) {
    on_click = MechTouchUIPauseButton_OnClick_Callback;
    disable_timer = 0.0f;
    skip_prompt_timer = 0.0f;
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

    if (NewMode != 0 || NewLData != NULL || editor_active != 0 || GameTimer.time_elapsed <= 0.0f ||
        GameTimer.update_count == 0 || WORLD == NULL || WORLD->current_level == TITLES_LDATA || CutSceneWaiting != 0) {
        return;
    }

    if (CUTSTOPGAME != 0) {
        if (CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo)) == 0 || skip_prompt_timer <= 0.0f) {
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

void MechTouchUIPauseButton::Render() {
    if (NewMode != 0 || NewLData != NULL || editor_active != 0 || GameTimer.time_elapsed <= 0.0f ||
        GameTimer.update_count == 0 || WORLD == NULL || WORLD->current_level == TITLES_LDATA ||
        WORLD->current_level == CREDITS_LDATA) {
        return;
    }

    if (Paused != 0) {
        const i32 menu_id = GetMenuID();
        DrawTouchPrompt(
            const_cast<char *>(menu_id == LEGO_MENU_PAUSE_MAIN || menu_id == LEGO_MENU_PAUSE_CUTSCENE ? ">" : "<<"),
            NULL, hovered != 0, true);
        return;
    }

    if (GameMenu[GameMenuLevel].menu != -1 || CutSceneWaiting != 0 || MiniCutCam != 0 || memcard_autosavestarted != 0 ||
        memcard_autosavepostdelay > 0.0f || memcard_autosavepredelay > 0.0f ||
        (CUTSTOPGAME != 0 && CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo)) == 0)) {
        if (GetMenuID() != -1) {
            DrawTouchPrompt(const_cast<char *>("<<"), NULL, hovered != 0, true);
        }
        return;
    }

    DrawTouchPrompt(const_cast<char *>("II"), NULL, hovered != 0, false);
}

MechTouchUIPlayerButton::MechTouchUIPlayerButton() : MechTouchUIElement(VuVec(-0.7725f, 0.7525f, 0.0f, 1.0f), 0.16f) {
    on_click = PlayerButton_OnClick_Callback_NextButton;
    on_hold = PlayerButton_OnHold_Callback;
    on_leave = PlayerButton_OnLeave_Callback;
    selector = NULL;
    chooser_mode = 1;
}

void MechTouchUIPlayerButton::Process(float) {
    disabled = GetMenuID() != -1 || Paused != 0 || NewMode != 0 || NewLData != NULL || CUTSTOPGAME != 0 ||
               MiniCutCam != 0 || FadeSys.fade > 0.0f || player == NULL;

    if (selector != NULL) {
        if (owner == NULL && selector->field_0x88 == 0) {
            selector->BlendOut();
        }
        if (selector->BlendedOut()) {
            delete selector;
            selector = NULL;
        }
    }
    if (disabled || player == NULL) {
        return;
    }

    if (chooser_mode == 0) {
        const i32 update = procActive++;
        if (update % 5 != 0) {
            return;
        }
        for (i32 target_index = 0; target_index < 32; ++target_index) {
            if (field_0x144[target_index] != 0 || target_ids[target_index] < 0) {
                continue;
            }
            for (i32 slot = 0; slot < 8; ++slot) {
                GameObject_s *target = Player[slot];
                if (target != NULL && target->id == target_ids[target_index] &&
                    TouchHacks::CanTagTo(*player, *target)) {
                    field_0x144[target_index] = 1;
                    MechSystems::Get()->NewRadarPulse(position, false);
                    break;
                }
            }
        }
        return;
    }

    if (procActive++ <= 20) {
        return;
    }
    chooser_mode = 0;
    for (i32 target_index = 0; target_index < 32; ++target_index) {
        field_0x144[target_index] = 0;
        for (i32 slot = 0; slot < 8; ++slot) {
            GameObject_s *target = Player[slot];
            if (target != NULL && target->id == target_ids[target_index]) {
                field_0x144[target_index] = target == player || TouchHacks::CanTagTo(*player, *target);
                break;
            }
        }
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
        if (player != NULL && (player->apiobj.object_flags & 0x1001) == 0x1001) {
            target_ids[target_count++] = player->id;
        }
    }

    if (FreePlay == 0 || apicharsys == NULL || apicharsys->loaded_model_count <= 0) {
        return;
    }

    i32 free_play_count = 0;
    const bool cheat_enabled = Cheats_CheckFlags(0x100) != 0;
    for (i32 i = 0; i < apicharsys->loaded_model_count; ++i) {
        const APICHARACTERMODEL &model = apicharsys->models[i];
        if ((model.flags & 1) == 0) {
            continue;
        }

        const i32 id = model.model_id;
        const i32 collected = InCollectList_Index(id, NULL, 0);
        const GAMECHARACTERDATA &game_character = GCDataList[id];
        const bool bonus_cheat_character =
            VehicleArea != 0 && BonusArea != 0 && cheat_enabled && (game_character.flags_094[3] & 2) != 0;

        if (collected == -1 && static_cast<i32>(game_character.flags_090) >= 0 &&
            (game_character.flags_094[3] & 1) == 0 && !bonus_cheat_character) {
            continue;
        }

        if (VehicleArea != 0) {
            const u32 model_flags = CDataList[id].model_flags;
            if ((model_flags & 0x2000) == 0 && !(BonusArea != 0 && (model_flags & 0x4000000) != 0) &&
                static_cast<i32>(game_character.flags_090) >= 0 && !bonus_cheat_character) {
                continue;
            }

            if (BonusArea != 0) {
                const i32 area = AreaFromMiniKitID(id);
                if (area != -1) {
                    if (Game_AreaSave == NULL || Game_AreaSave[area].minikit_complete == 0) {
                        continue;
                    }
                } else if (static_cast<i32>(game_character.flags_090) >= 0 && !bonus_cheat_character) {
                    continue;
                }
            }
        } else if ((CDataList[id].model_flags & 0x2000) != 0 || (game_character.flags_090 & 0x40) != 0) {
            continue;
        }

        if (BonusArea == 0 || VehicleArea == 0) {
            if (static_cast<i32>(game_character.flags_090) < 0) {
                if (!cheat_enabled) {
                    continue;
                }
            } else if ((game_character.flags_094[3] & 1) == 0 && (collected == -1 || Collection_Got(id) == 0)) {
                continue;
            }
        }

        free_play_target_ids[free_play_count++] = id;
        if (free_play_count == 32) {
            return;
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

void MechTouchUIPlayerButton::TriggerTagNext() {
    if (player == NULL) {
        return;
    }
    if (FreePlay != 0) {
        for (i32 index = 0; index < 32; ++index) {
            target_ids[index] = -1;
        }
        i32 target_count = 0;
        for (i32 slot = 0; slot < 8 && target_count < 32; ++slot) {
            GameObject_s *target = Player[slot];
            if (target != NULL && (target->apiobj.field_0x1f8 & 0x1001) == 0x1001) {
                target_ids[target_count++] = target->id;
            }
        }
    }

    i32 current_index = -1;
    for (i32 index = 0; index < 32; ++index) {
        if (target_ids[index] == player->id) {
            current_index = index;
            break;
        }
    }
    if (current_index == -1) {
        return;
    }

    for (i32 offset = 1; offset < 32; ++offset) {
        const i32 index = (current_index + offset) & 31;
        if (target_ids[index] < 0) {
            continue;
        }
        for (i32 slot = 0; slot < 8; ++slot) {
            GameObject_s *target = Player[slot];
            if (target == NULL || target->id != target_ids[index] || !TouchHacks::CanTagTo(*player, *target)) {
                continue;
            }
            GameObject_s *source = player;
            if (TagCode(source, target, 0, 0, 1) == 1) {
                GameAudio_PlaySfx(0x21, NULL, 0, 0);
                Tag_NewTransfer(source, target);
            }
            return;
        }
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

void MechTouchUIPartySelector::Cleanup() {
    for (i32 i = 0; i < 32; ++i) {
        if (icons[i] != NULL) {
            MechSystems::Get()->TouchUI().RemoveUIElement(*icons[i]);
            delete icons[i];
            icons[i] = NULL;
        }
    }
}

MechTouchUIPartySelector::MechTouchUIPartySelector(MechTouchUIPlayerButton &button, i32 *target_ids)
    : icon_count(0), player_button(&button), field_0x88(0) {
    memset(icons, 0, sizeof(icons));

    const bool small_screen = NuIOS_IsSmallScreen() != 0;
    const f32 gap = small_screen ? 0.11f : 0.06f;
    const f32 scale = small_screen ? 0.16f : 0.15f;
    const i32 icons_per_row = small_screen ? 5 : 7;
    const f32 first_y = button.position.y - button.radius_y - gap * 2.0f;
    f32 x = button.position.x;
    f32 y = first_y;
    f32 delay = 0.0f;
    f32 delay_step = 0.03f;
    i32 column = 0;

    for (i32 target_index = 0; target_index < 32; ++target_index) {
        if (target_ids[target_index] < 0 || player == NULL) {
            continue;
        }
        if (column == 0) {
            ++icon_count;
        }

        VuVec position(x, y, button.position.z, button.position.w);
        MechTouchUICharIcon *icon = new MechTouchUICharIcon(*this, position, target_ids[target_index], scale);
        icons[target_index] = icon;
        icon->on_release = MechTouchUIPartySelector_OnRelease_Callback;
        icon->owner = button.owner;
        icon->SetupDisabled();
        icon->alpha_end = icon->disabled != 0 ? 0.3f : 1.0f;
        icon->alpha_delay = delay;
        icon->alpha_start = 0.0f;
        icon->alpha_elapsed = 0.0f;
        icon->alpha_duration = 0.3f;
        *icon->alpha_target = 0.0f;
        MechSystems::Get()->TouchUI().AddUIElement(*icon);

        ++column;
        if (column >= icons_per_row) {
            column = 0;
            x += GetAspectRatio() * (scale + gap);
            y = first_y;
            delay = 0.0f;
            delay_step = 0.03f;
        } else {
            y -= scale + gap;
            delay += delay_step;
            delay_step *= 0.9f;
        }
    }
}

MechTouchUIPartySelector::~MechTouchUIPartySelector() {
    Cleanup();
}
