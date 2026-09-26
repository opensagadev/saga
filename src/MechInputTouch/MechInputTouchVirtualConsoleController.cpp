#include "decomp.h"
#include "MechInputTouch_types.h"
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
extern "C" i32 NuIOS_IsSmallScreen(void);
i32 GetMenuID();
extern FadeSystem FadeSys;
i16 MechInputTouchVirtualConsoleController::s_textures[9];
f32 MechInputTouchVirtualConsoleController::s_noInputTimer;
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

    MechTouchUIAnimation *animations =
        reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(dpad) + 0x40);
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

void MechInputTouchVirtualConsoleController::LoadPerm() {
    s_textures[2] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_INTERACTBUTTON"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[3] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_TAGBUTTON"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[0] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_JUMPBUTTON"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[1] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_FIGHTBUTTON"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[4] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_MOVEMENTWHEEL"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[5] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_MOVEARROW"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[6] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_LOCK_LOCKED"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[7] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_LOCK_UNLOCKED"),
                                             &permbuffer_ptr, permbuffer_end));
    s_textures[8] = static_cast<i16>(NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_CROSSHAIR"),
                                             &permbuffer_ptr, permbuffer_end));
    hasDoneLoadPerm = 1;
}

MechInputTouchVirtualConsoleController::MechInputTouchVirtualConsoleController(i32 player)
    : MechInputTouchMainController(player), active(0), dpad_touch(NULL), drag_touch(NULL),
      lock_button(NULL), button_mover(NULL) {
    for (i32 i = 0; i < 4; ++i) {
        buttons[i] = NULL;
    }
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

bool MechInputTouchVirtualConsoleController::OnRelease(GameObject_s &, TouchHolder &touch) {
    if (dpad_touch == &touch) {
        dpad_touch = NULL;
        return false;
    }
    if (drag_touch != &touch) {
        return false;
    }

    MechTouchUIAnimation *animations =
        reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(dpad) + 0x40);
    animations[0].Start(*animations[0].target, 0.0f, 0.15f);
    animations[1].Start(*animations[1].target, 0.0f, 0.15f);
    drag_touch = NULL;
    return false;
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
            const f32 stick_y = strength * cosine;
            const f32 stick_x = -(strength * sine);
            stick_values[2] = MAX(-1.0f, MIN(stick_x, 1.0f));
            stick_values[3] = MAX(-1.0f, MIN(stick_y, 1.0f));
        }
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

bool MechInputTouchVirtualConsoleController::ShouldBeActive() {
    if (player == NULL || NewMode != 0 || NewLData != NULL || FadeSys.fade != 0.0f) {
        return false;
    }
    if (Paused != 0 && GetMenuID() != 25) {
        return false;
    }
    if (CUTSTOPGAME != 0 || GetMenuID() == 12 || GetMenuID() == 16 || WORLD == NULL ||
        (WORLD->current_level->flags & 0x4e2) != 2 || !TouchHacks::TouchControlsActive || MiniCutCam == 2) {
        return false;
    }
    if (MechSystems::Get()->PlayerButton().selector != NULL &&
        MechSystems::Get()->PlayerButton().selector->field_0x88 == 0) {
        return MechSystems::Get()->PlayerButton().selector->BlendedOut();
    }
    return true;
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
        const f32 aspect_radius = aspect * radius;
        const NuVec2 left_position = {SuperOptions.left_control_x, SuperOptions.left_control_y};
        dpad = reinterpret_cast<MechTouchUIElement *>(new VirtualControlDPad(left_position, 0.25f, *this));

        const f32 zero_x = 0.0f * aspect_radius;
        const f32 x = SuperOptions.right_control_x;
        const f32 y = SuperOptions.right_control_y;
        const f32 dy = radius * 1.3f;
        const NuVec2 button_down = {x + zero_x, y - dy};
        buttons[0] = reinterpret_cast<MechTouchUIElement *>(new VirtualControlButton(
            button_down, radius, static_cast<MechInputTouchMainController::eButtonTypes>(2)));
        const f32 dx = aspect_radius * 1.3f;
        const f32 zero_y = 0.0f * radius;
        const NuVec2 button_right = {x + dx, y + zero_y};
        buttons[1] = reinterpret_cast<MechTouchUIElement *>(new VirtualControlButton(
            button_right, radius, static_cast<MechInputTouchMainController::eButtonTypes>(3)));
        const NuVec2 button_up = {x - zero_x, y + dy};
        buttons[2] = reinterpret_cast<MechTouchUIElement *>(new VirtualControlButton(
            button_up, radius, static_cast<MechInputTouchMainController::eButtonTypes>(1)));
        const NuVec2 button_left = {x - dx, y - zero_y};
        buttons[3] = reinterpret_cast<MechTouchUIElement *>(new VirtualControlButton(
            button_left, radius, static_cast<MechInputTouchMainController::eButtonTypes>(0)));
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

void MechInputTouchVirtualConsoleController::UpdateButtonPositions() {
    const f32 radius = NuIOS_IsSmallScreen() ? 0.29f : 0.23f;
    const f32 aspect = GetAspectRatio();
    const f32 y = SuperOptions.right_control_y;
    const f32 x = SuperOptions.right_control_x;
    const f32 dx = aspect * radius;

    MechTouchUIElement *button = buttons[0];
    button->position.z = 0.0f;
    button->position.w = 1.0f;
    button->position.y = y - radius;
    button->position.x = x;

    button = buttons[1];
    button->position.z = 0.0f;
    button->position.w = 1.0f;
    button->position.y = y;
    button->position.x = x + dx;

    button = buttons[2];
    button->position.z = 0.0f;
    button->position.w = 1.0f;
    button->position.y = y + radius;
    button->position.x = x;

    button = buttons[3];
    button->position.z = 0.0f;
    button->position.w = 1.0f;
    button->position.y = y;
    button->position.x = x - dx;

    if (button_mover != NULL) {
        button_mover->position.x = x;
        button_mover->position.y = y;
    }
}

void MechInputTouchVirtualConsoleController::UpdateDPadPos() {
    MechTouchUIElement *const element = dpad;
    element->position.x = SuperOptions.left_control_x;
    element->position.y = SuperOptions.left_control_y;
}

MechInputTouchVirtualConsoleController::~MechInputTouchVirtualConsoleController() {
    delete buttons[0];
    delete buttons[1];
    delete buttons[2];
    delete buttons[3];
    delete dpad;
}
