// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/numtl.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nucore/NuInputDevice.h"
#include <string.h>
#include "gameframework/saveload.h"
#include "legoapi/menus/screens/store.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/audio/sfx.h"
#include <stdio.h>
#include "legoapi/render/core/screen.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouchMenuController.h"
#include "MechInputTouch/MechInputTouchVirtualConsoleController.h"
#include "MechInputTouch/MechTouchUIElements.h"
#include "nu2api/nuandroid/ios_device.h"
i16 MechInputTouchVirtualConsoleController::s_textures[9];
// Seconds without input before the console fades out; the original set the
// timer from a helper at static-init time rather than as constant data.
static inline f32 NoInputTimeout() {
    return 20.0f;
}

f32 MechInputTouchVirtualConsoleController::s_noInputTimer = NoInputTimeout();
bool lookAtMeBlendDone;
extern "C" {
    u8 hasDoneLoadPerm;
}

float MechInputTouchVirtualConsoleController::s_defaultDPadPosX = -0.72f;
float MechInputTouchVirtualConsoleController::s_defaultDPadPosY = -0.65f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosX = 0.77f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosY = -0.62f;
float MechInputTouchVirtualConsoleController::s_defaultDPadPosX_SmallScreen = -0.62f;
float MechInputTouchVirtualConsoleController::s_defaultDPadPosY_SmallScreen = -0.49f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosX_SmallScreen = 0.62f;
float MechInputTouchVirtualConsoleController::s_defaultButtonsPosY_SmallScreen = -0.49f;

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

MechInputTouchVirtualConsoleController::~MechInputTouchVirtualConsoleController() {
    delete buttons[0];
    delete buttons[1];
    delete buttons[2];
    delete buttons[3];
    delete dpad;
}

void VirtualControlDPad::Render() {
    MechTouchUITexButton::Render();

    const f32 centre_x = (position.x + 1.0f) * 0.5f;
    const f32 centre_y = (1.0f - position.y) * 0.5f;
    const f32 scaled_x = radius_x * scale;
    const f32 scaled_y = radius_y * scale;
    const bool all_active =
        (GetMenuID() == 25 && owner != NULL) || MechInputTouchVirtualConsoleController::s_noInputTimer < 0.0f;

    const f32 bottom_factor = stick_values.y > 0.2f || all_active ? 0.75f : 0.5f;
    const f32 arrow_radius = NuIOS_IsSmallScreen() ? 0.07f : 0.035f;
    numtl_s *arrow = arrow_material;
    i32 colour = (static_cast<i32>(128.0f * alpha * bottom_factor) << 24) | 0x808080;
    f32 arrow_height = arrow_radius * scale;
    f32 aspect = GetAspectRatio();
    const f32 vertical_offset = 0.4f * scaled_y;
    RndrTexQuad(centre_x, centre_y + vertical_offset, aspect * arrow_height, arrow_height, colour, arrow, 0x8000);

    const f32 left_factor = stick_values.x < -0.2f || all_active ? 1.0f : 0.5f;
    arrow = arrow_material;
    colour = (static_cast<i32>(128.0f * alpha * left_factor) << 24) | 0x808080;
    arrow_height = arrow_radius * scale;
    aspect = GetAspectRatio();
    const f32 horizontal_offset = scaled_x * 0.4f;
    RndrTexQuad(centre_x - horizontal_offset, centre_y, aspect * arrow_height, arrow_height, colour, arrow, 0xc000);

    const f32 top_factor = stick_values.y < -0.2f || all_active ? 1.0f : 0.5f;
    arrow = arrow_material;
    colour = (static_cast<i32>(128.0f * alpha * top_factor) << 24) | 0x808080;
    arrow_height = arrow_radius * scale;
    aspect = GetAspectRatio();
    RndrTexQuad(centre_x, centre_y - vertical_offset, aspect * arrow_height, arrow_height, colour, arrow, 0);

    const f32 right_factor = stick_values.x > 0.2f || all_active ? 1.0f : 0.5f;
    arrow = arrow_material;
    colour = (static_cast<i32>(128.0f * alpha * right_factor) << 24) | 0x808080;
    arrow_height = arrow_radius * scale;
    aspect = GetAspectRatio();
    RndrTexQuad(centre_x + horizontal_offset, centre_y, aspect * arrow_height, arrow_height, colour, arrow, 0x4000);

    if (GetMenuID() == 25 && controller->button_mover != NULL) {
        MechTouchUITexButton *button_mover = static_cast<MechTouchUITexButton *>(controller->button_mover);
        mover.position = position;
        mover.radius_x = button_mover->radius_x;
        mover.radius_y = button_mover->radius_y;
        mover.scale = mover.scale_to = button_mover->scale;
        mover.scale_elapsed = mover.scale_duration;
        if (owner != NULL) {
            mover.scale = mover.scale_to = 1.1f;
        }
        mover.MechTouchUITexButton::Render();
    }
}

void VirtualControlDPad::Process(float elapsed) {
    MechTouchUITexButton::Process(elapsed);

    if (MechInputTouchVirtualConsoleController::s_noInputTimer >= 0.0f &&
        (owner != NULL || SuperOptions.dpad_locked == 0 || GetMenuID() == 25)) {
        const f32 opacity = owner != NULL ? 0.75f : 0.4f;
        alpha = opacity;
        alpha_to = opacity;
        alpha_elapsed = alpha_duration;
    }
    disabled = !(!(MechInputTouchVirtualConsoleController::s_noInputTimer < 0.0f) && owner != NULL) &&
               SuperOptions.dpad_locked != 0 && GetMenuID() != 25;
    const f32 timer = MechInputTouchVirtualConsoleController::s_noInputTimer;

    if (timer < 0.0f) {
        visible = 1;
        if (scale_duration < 0.0f || scale_elapsed >= scale_duration + scale_delay) {
            const f32 pulse = 1.0f + 0.05f * NU_SIN_LUT(static_cast<i32>(timer * 65536.0f));
            scale_elapsed = scale_duration;
            alpha = 0.75f;
            alpha_to = 0.75f;
            scale = pulse;
            scale_to = pulse;
            alpha_elapsed = alpha_duration;
        }
    }

    if (GetMenuID() == 25) {
        if (owner != NULL) {
            const f32 x = owner->touch_position.x + drag_offset.x;
            const f32 y = owner->touch_position.y + drag_offset.y;
            position.x = MAX(radius_x - 0.975f, MIN(x, -radius_x));
            position.y = MAX(radius_y - 0.975f, MIN(y, 0.0f));
            SuperOptions.left_control_x = position.x;
            SuperOptions.left_control_y = position.y;
        }
        return;
    }

    if (visible == 0) {
        MechSystems::Get()->gesture_controller->stick_values[0] = 0.0f;
        MechSystems::Get()->gesture_controller->stick_values[1] = 0.0f;
    }
    stick_values.x = 0.0f;
    stick_values.y = 0.0f;

    if (owner != NULL) {
        const f32 dx = owner->touch_position.x - position.x;
        const f32 dy = -(owner->touch_position.y - position.y);
        const f32 distance = NuFsqrt(dx * dx + dy * dy);
        const i32 angle = NuAtan2D(dx, dy);
        const f32 ratio = distance / radius_y;
        const f32 strength = static_cast<f32>(static_cast<double>(MAX(0.0f, MIN(ratio, 1.0f))) * 1.4) * 3.0f;
        stick_values.x = MAX(-1.0f, MIN(NU_SIN_LUT(angle) * strength, 1.0f));
        stick_values.y = MAX(-1.0f, MIN(NU_COS_LUT(angle) * strength, 1.0f));
        MechInputTouchVirtualConsoleController::s_noInputTimer = 20.0f;
    }

    MechSystems::Get()->gesture_controller->stick_values[0] = stick_values.x;
    MechSystems::Get()->gesture_controller->stick_values[1] = stick_values.y;
}

void VirtualControlDPad_LockButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &) {
    GameAudio_PlaySfx(0x30, NULL, 0, 0);
    if (SuperOptions.dpad_locked == 0) {
        SuperOptions.dpad_locked = 1;
        static_cast<MechTouchUITexButton &>(element).UpdateTexture(
            MechInputTouchVirtualConsoleController::s_textures[7]);
    } else {
        SuperOptions.dpad_locked = 0;
        static_cast<MechTouchUITexButton &>(element).UpdateTexture(
            MechInputTouchVirtualConsoleController::s_textures[6]);
    }
}

void MechInputTouchVirtualConsoleController::Deactivate() {
    if (!active) {
        return;
    }
    active = 0;
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
    if (buttons[0] != NULL) {
        MechSystems::Get()->TouchUI().RemoveUIElement(*buttons[0]);
    }
    if (buttons[1] != NULL) {
        MechSystems::Get()->TouchUI().RemoveUIElement(*buttons[1]);
    }
    if (buttons[2] != NULL) {
        MechSystems::Get()->TouchUI().RemoveUIElement(*buttons[2]);
    }
    if (buttons[3] != NULL) {
        MechSystems::Get()->TouchUI().RemoveUIElement(*buttons[3]);
    }

    MechTouchUIAnimation *animations = reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(dpad) + 0x40);
    animations[1].value = 0.0f;
    animations[1].to = 0.0f;
    animations[1].elapsed = animations[1].duration;
    MechSystems::Get()->TouchUI().RemoveUIElement(*dpad);
    drag_touch = NULL;
    dpad->owner = NULL;
    stick_values[0] = 0.0f;
    stick_values[1] = 0.0f;

    if (lock_button != NULL) {
        MechSystems::Get()->TouchUI().RemoveUIElement(*lock_button);
        delete lock_button;
        lock_button = NULL;
    }
    if (button_mover != NULL) {
        MechSystems::Get()->TouchUI().RemoveUIElement(*button_mover);
        delete button_mover;
        button_mover = NULL;
    }
}

bool MechInputTouchVirtualConsoleController::OnRelease(GameObject_s &, TouchHolder &touch) {
    if (dpad_touch == &touch) {
        dpad_touch = NULL;
    }
    if (drag_touch != &touch) {
        return false;
    }

    MechTouchUIAnimation *animations = reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(dpad) + 0x40);
    animations[0].Start(*animations[0].target, 0.0f, 0.15f);
    animations[1].Start(*animations[1].target, 0.0f, 0.15f);
    drag_touch = NULL;
    return false;
}

bool MechInputTouchVirtualConsoleController::OnDown(GameObject_s &object, TouchHolder &touch) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL ||
        GetMenuID() == 25) {
        return false;
    }

    const u8 locked = SuperOptions.dpad_locked;
    if (locked == 0 || touch.down_position.x > 0.0f) {
        if (dpad_touch == NULL) {
            dpad_touch = &touch;
        }
        if (locked == 0) {
            return true;
        }
    }

    if (drag_touch == NULL && touch.down_position.x < 0.0f) {
        MechTouchUIElement *const pad = dpad;
        if (s_noInputTimer >= 0.0f) {
            MechTouchUIAnimation *animations =
                reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(pad) + 0x40);
            animations[0].Start(*animations[0].target, 1.0f, 0.15f);
            animations[1].Start(*animations[1].target, 1.0f, 0.15f);
            pad->position.x = touch.down_position.x;
            pad->position.y = touch.down_position.y;
        }
        pad->owner = &touch;
        drag_touch = &touch;
    }
    return true;
}

VirtualControlDPad::~VirtualControlDPad() {
    NuMtlDestroy(arrow_material);
}

VirtualControlDPad_LockButton::VirtualControlDPad_LockButton(VirtualControlDPad &pad)
    : MechTouchUITexButton(pad.position, 0.1f), dpad(&pad) {
    on_click = VirtualControlDPad_LockButton_OnClick_Callback;
    UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[SuperOptions.dpad_locked != 0 ? 7 : 6]);
}

VirtualControlButton::VirtualControlButton(NuVec2 const &pos, float radius,
                                           MechInputTouchMainController::eButtonTypes type)
    : MechTouchUITexButton(VuVec(pos.x, pos.y, 0.0f, 1.0f), radius), button_type(type) {
    on_down = VirtualControlButton_OnDown_Callback;
    switch (type) {
        case 0:
            UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[1]);
            break;
        case 1:
            UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[3]);
            break;
        case 2:
            UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[0]);
            break;
        case 3:
            UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[2]);
            break;
        default:
            break;
    }
}

VirtualControlButtonMover::VirtualControlButtonMover(MechInputTouchVirtualConsoleController &console)
    : MechTouchUITexButton(VuVec(SuperOptions.right_control_x, SuperOptions.right_control_y, 1.0f, 1.0f), 0.1f),
      controller(&console) {
    on_down = VirtualControlButtonMover_OnDown_Callback;
    UpdateTexture(console.s_textures[8]);
    pulse_phase = 0;
}

VirtualControlDPad::VirtualControlDPad(NuVec2 const &pos, float radius, MechInputTouchVirtualConsoleController &console)
    : MechTouchUITexButton(VuVec(pos.x, pos.y, 0.0f, 1.0f), radius), controller(&console), mover(console) {
    on_down = VirtualControlDPad_OnDown_Callback;
    scale = scale_to = 0.0f;
    scale_elapsed = scale_duration;
    stick_values.x = 0.0f;
    stick_values.y = 0.0f;
    UpdateTexture(console.s_textures[4]);

    arrow_material = NuMtlCreate(1);
    arrow_material->sort_pri = 255;
    arrow_material->diffuse_color.r = 0.0f;
    arrow_material->diffuse_color.g = 0.0f;
    arrow_material->diffuse_color.b = 0.0f;
    arrow_material->opacity = 0.0f;
    arrow_material->attribs.cull_mode = 2;
    arrow_material->attribs.z_mode = 1;
    arrow_material->attribs.alpha_mode = 1;
    arrow_material->attribs.unknown_2_1_2 = 2;
    arrow_material->attribs.alpha_test = 1;
    arrow_material->tex_id = console.s_textures[5];
    NuMtlUpdate(arrow_material);
}

MechInputTouchVirtualConsoleController::MechInputTouchVirtualConsoleController(i32 player)
    : MechInputTouchMainController(player), active(0), dpad_touch(NULL), drag_touch(NULL), lock_button(NULL),
      button_mover(NULL) {
    for (i32 i = 0; i < 4; ++i) {
        buttons[i] = NULL;
    }
}

void MechInputTouchVirtualConsoleController::UpdateDPadPos() {
    MechTouchUIElement *const element = dpad;
    element->position.x = SuperOptions.left_control_x;
    element->position.y = SuperOptions.left_control_y;
}

void MechInputTouchVirtualConsoleController::UpdateButtonPositions() {
    f32 radius = 0.23f;
    if (NuIOS_IsSmallScreen()) {
        radius = 0.29f;
    }
    const f32 aspect = GetAspectRatio();
    const f32 x = SuperOptions.right_control_x;
    const f32 y = SuperOptions.right_control_y;
    const f32 dx = aspect * radius;

    MechTouchUIElement *button = buttons[0];
    button->position = VuVec(x, y - radius, 0.0f, 1.0f);

    button = buttons[1];
    button->position = VuVec(x + dx, y, 0.0f, 1.0f);

    button = buttons[2];
    button->position = VuVec(x, y + radius, 0.0f, 1.0f);

    button = buttons[3];
    button->position = VuVec(x - dx, y, 0.0f, 1.0f);

    MechTouchUIElement *const mover = button_mover;
    if (mover != NULL) {
        mover->position.x = x;
        mover->position.y = y;
    }
}

void VirtualControlButtonMover::Process(float elapsed) {
    pulse_phase += static_cast<i32>(elapsed * 65536.0f);
    f32 pulse = 1.0f + 0.1f * NU_SIN_LUT(pulse_phase);
    scale = scale_to = pulse;
    scale_elapsed = scale_duration;

    if (owner != NULL) {
        scale = scale_to = 1.1f;
        position.x = owner->touch_position.x + drag_offset.x;
        position.y = owner->touch_position.y + drag_offset.y;
        if (NuIOS_IsSmallScreen()) {
            position.x = MAX(0.38f, MIN(position.x, 0.62f));
            position.y = MAX(-0.49f, MIN(position.y, 0.0f));
        } else {
            position.x = MAX(0.23f, MIN(position.x, 0.77f));
            position.y = MAX(-0.62f, MIN(position.y, 0.0f));
        }
        SuperOptions.right_control_x = position.x;
        SuperOptions.right_control_y = position.y;
        controller->UpdateButtonPositions();
    }
}

void MechInputTouchVirtualConsoleController::ResetButtonPositionsToDefault() {
    MechSystems *systems = MechSystems::Get();
    if (systems->input_touch_system.control_mode != 1) {
        return;
    }

    MechInputTouchVirtualConsoleController *controller =
        reinterpret_cast<MechInputTouchVirtualConsoleController *>(MechSystems::Get()->gesture_controller);
    if (NuIOS_IsSmallScreen()) {
        SuperOptions.left_control_x = s_defaultDPadPosX_SmallScreen;
        SuperOptions.left_control_y = s_defaultDPadPosY_SmallScreen;
        SuperOptions.right_control_x = s_defaultButtonsPosX_SmallScreen;
        SuperOptions.right_control_y = s_defaultButtonsPosY_SmallScreen;
    } else {
        SuperOptions.left_control_x = s_defaultDPadPosX;
        SuperOptions.left_control_y = s_defaultDPadPosY;
        SuperOptions.right_control_x = s_defaultButtonsPosX;
        SuperOptions.right_control_y = s_defaultButtonsPosY;
    }
    controller->UpdateButtonPositions();
    controller->UpdateDPadPos();
}

void MechInputTouchVirtualConsoleController::LoadPerm() {
    s_textures[2] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_INTERACTBUTTON"), &permbuffer_ptr, permbuffer_end));
    s_textures[3] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_TAGBUTTON"), &permbuffer_ptr, permbuffer_end));
    s_textures[0] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_JUMPBUTTON"), &permbuffer_ptr, permbuffer_end));
    s_textures[1] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_FIGHTBUTTON"), &permbuffer_ptr, permbuffer_end));
    s_textures[4] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_MOVEMENTWHEEL"), &permbuffer_ptr, permbuffer_end));
    s_textures[5] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_MOVEARROW"), &permbuffer_ptr, permbuffer_end));
    s_textures[6] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_LOCK_LOCKED"), &permbuffer_ptr, permbuffer_end));
    s_textures[7] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_LOCK_UNLOCKED"), &permbuffer_ptr, permbuffer_end));
    s_textures[8] = static_cast<i16>(
        NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_CROSSHAIR"), &permbuffer_ptr, permbuffer_end));
    hasDoneLoadPerm = 1;
}

void MechInputTouchVirtualConsoleController::ProcessDragMovement(GameObject_s &) {
    if (dpad_touch == NULL) {
        return;
    }
    const f32 down_x = dpad_touch->down_position.x;
    const f32 down_y = dpad_touch->down_position.y;
    if (SuperOptions.dpad_locked && down_x < 0.0f) {
        return;
    }
    const f32 dy = down_y - dpad_touch->touch_position.y;
    const f32 dx = down_x - dpad_touch->touch_position.x;
    const f32 distance = NuFsqrt(dx * dx + dy * dy);
    if (distance > 0.05f) {
        if (dpad_touch->held_time > 0.2f) {
            const i32 angle = NuAtan2D(dx, dy);
            const f32 sine = NU_SIN_LUT(angle);
            const f32 cosine = NU_COS_LUT(angle);
            const f32 strength = MAX(0.0f, MIN((distance - 0.05f) * 4.0f, 1.0f)) * 1.4;
            const f32 stick_x = -(strength * sine);
            const f32 stick_y = strength * cosine;
            stick_values[2] = MAX(-1.0f, MIN(stick_x, 1.0f));
            stick_values[3] = MAX(-1.0f, MIN(stick_y, 1.0f));
        }
    }
}

bool MechInputTouchVirtualConsoleController::ShouldBeActive() {
    if (player == NULL || NewMode != 0 || NewLData != NULL || FadeSys.fade != 0.0f) {
        return false;
    }
    if (Paused != 0 && GetMenuID() != 25) {
        return false;
    }
    if (CUTSTOPGAME != 0) {
        return false;
    }
    if (GetMenuID() == 12) {
        return false;
    }
    if (GetMenuID() == 16) {
        return false;
    }
    if (WORLD == NULL) {
        return false;
    }
    if ((WORLD->current_level->flags & 0x4e2) != 2) {
        return false;
    }
    if (TouchHacks::TouchControlsActive) {
        return false;
    }
    if (MiniCutCam == 2) {
        return false;
    }
    if (MechSystems::Get()->PlayerButton().selector == NULL) {
        return true;
    }
    if (MechSystems::Get()->PlayerButton().selector->field_0x88 != 0) {
        return true;
    }
    return MechSystems::Get()->PlayerButton().selector->BlendedOut();
}

void MechInputTouchVirtualConsoleController::Activate() {
    if (active || buttons[0] == NULL || !ShouldBeActive()) {
        return;
    }
    s_noInputTimer = 20.0f;
    if (dpad_touch != NULL && !dpad_touch->is_down) {
        dpad_touch = NULL;
    }
    active = 1;
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
    dpad_touch = NULL;

    if (buttons[0] != NULL) {
        MechSystems::Get()->TouchUI().AddUIElement(*buttons[0]);
    }
    if (buttons[1] != NULL) {
        MechSystems::Get()->TouchUI().AddUIElement(*buttons[1]);
    }
    if (buttons[2] != NULL) {
        MechSystems::Get()->TouchUI().AddUIElement(*buttons[2]);
    }
    if (buttons[3] != NULL) {
        MechSystems::Get()->TouchUI().AddUIElement(*buttons[3]);
    }
    MechSystems::Get()->TouchUI().AddUIElement(*dpad);
    dpad->position.x = SuperOptions.left_control_x;
    dpad->position.y = SuperOptions.left_control_y;

    if (!SuperOptions.dpad_locked || GetMenuID() != -1) {
        MechTouchUIAnimation *animations =
            reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(dpad) + 0x40);
        animations[0].value = 1.0f;
        animations[0].to = 1.0f;
        animations[1].value = 1.0f;
        animations[1].to = 1.0f;
        animations[0].elapsed = animations[0].duration;
        dpad->visible = 1;
        animations[1].elapsed = animations[1].duration;
    }

    if (GetMenuID() != -1) {
        if (lock_button != NULL) {
            MechSystems::Get()->TouchUI().RemoveUIElement(*lock_button);
            delete lock_button;
            lock_button = NULL;
        }
        lock_button = reinterpret_cast<MechTouchUIElement *>(
            new VirtualControlDPad_LockButton(*reinterpret_cast<VirtualControlDPad *>(dpad)));
        MechSystems::Get()->TouchUI().AddUIElement(*lock_button);

        if (button_mover != NULL) {
            MechSystems::Get()->TouchUI().RemoveUIElement(*button_mover);
            delete button_mover;
            button_mover = NULL;
        }
        button_mover = reinterpret_cast<MechTouchUIElement *>(new VirtualControlButtonMover(*this));
        MechSystems::Get()->TouchUI().AddUIElement(*button_mover);
    }
    MechSystems::Get()->gesture_controller = reinterpret_cast<MechInputTouchGestureBasedController *>(this);
}

void MechInputTouchVirtualConsoleController::Update(NuInputTouchData const *) {
    if (ShouldBeActive()) {
        Activate();
    } else {
        Deactivate();
    }
    stick_values[2] = 0.0f;
    stick_values[3] = 0.0f;

    if (hasDoneLoadPerm != 0 && buttons[0] == NULL) {
        const f32 radius = NuIOS_IsSmallScreen() ? 0.20f : 0.14f;
        const f32 aspect = GetAspectRatio();
        const NuVec2 left_position = {SuperOptions.left_control_x, SuperOptions.left_control_y};
        dpad = reinterpret_cast<MechTouchUIElement *>(new VirtualControlDPad(left_position, 0.25f, *this));

        const f32 aspect_radius = aspect * radius;
        const f32 zero_x = 0.0f * aspect_radius;
        const f32 x = SuperOptions.right_control_x;
        const f32 y = SuperOptions.right_control_y;
        const f32 dy = radius * 1.3f;
        const NuVec2 button_down = {x + zero_x, y - dy};
        buttons[0] = reinterpret_cast<MechTouchUIElement *>(
            new VirtualControlButton(button_down, radius, static_cast<MechInputTouchMainController::eButtonTypes>(2)));
        const f32 dx = aspect_radius * 1.3f;
        const f32 zero_y = 0.0f * radius;
        const NuVec2 button_right = {x + dx, y + zero_y};
        buttons[1] = reinterpret_cast<MechTouchUIElement *>(
            new VirtualControlButton(button_right, radius, static_cast<MechInputTouchMainController::eButtonTypes>(3)));
        const NuVec2 button_up = {x - zero_x, y + dy};
        buttons[2] = reinterpret_cast<MechTouchUIElement *>(
            new VirtualControlButton(button_up, radius, static_cast<MechInputTouchMainController::eButtonTypes>(1)));
        const NuVec2 button_left = {x - dx, y - zero_y};
        buttons[3] = reinterpret_cast<MechTouchUIElement *>(
            new VirtualControlButton(button_left, radius, static_cast<MechInputTouchMainController::eButtonTypes>(0)));
        UpdateButtonPositions();
        Activate();
    }
    if (!active) {
        return;
    }

    if (s_noInputTimer >= 0.0f) {
        s_noInputTimer -= FRAMETIME;
    }
    if (s_noInputTimer < 0.0f) {
        if (GetMenuID() == 25) {
            MechSystems::Get()->NewRadarPulse(dpad->position, false);
            if (button_mover != NULL) {
                MechSystems::Get()->NewRadarPulse(button_mover->position, false);
            }
            s_noInputTimer = 20.0f;
        } else {
            MechTouchUIAnimation *animations =
                reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(dpad) + 0x40);
            if (!animations[0].IsActive()) {
                if (lookAtMeBlendDone) {
                    s_noInputTimer -= FRAMETIME;
                }
                if (SuperOptions.dpad_locked && drag_touch == NULL && !lookAtMeBlendDone) {
                    animations[0].Start(*animations[0].target, 1.0f, 0.3f);
                    animations[1].Start(*animations[1].target, 1.0f, 0.3f);
                    lookAtMeBlendDone = true;
                }
            }
            if (s_noInputTimer < -5.0f) {
                s_noInputTimer = 20.0f;
                if (SuperOptions.dpad_locked && drag_touch == NULL) {
                    animations[0].Start(*animations[0].target, 0.0f, 0.3f);
                    animations[1].Start(*animations[1].target, 0.0f, 0.3f);
                    lookAtMeBlendDone = false;
                }
            }
        }
    }
    GameObject_s *object = Player[player_id];
    if (object != NULL) {
        ProcessDragMovement(*object);
        UpdateButtons();
    }
}

