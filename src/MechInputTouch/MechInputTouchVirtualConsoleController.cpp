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
extern "C" {
    i32 hasDoneLoadPerm;
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
    STUBBED();
}

void MechInputTouchVirtualConsoleController::Deactivate() {
    if (!active) {
        return;
    }
    active = 0;
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
    for (i32 i = 0; i < 4; ++i) {
        if (buttons[i] != NULL) {
            MechSystems::Get()->TouchUI().RemoveUIElement(*buttons[i]);
        }
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
    struct TextureLoad {
        i32 index;
        const char *name;
    };
    const TextureLoad textures[] = {
        {2, "STUFF/UIBUTTONS/UIBUTTONS_INTERACTBUTTON"},
        {3, "STUFF/UIBUTTONS/UIBUTTONS_TAGBUTTON"},
        {0, "STUFF/UIBUTTONS/UIBUTTONS_JUMPBUTTON"},
        {1, "STUFF/UIBUTTONS/UIBUTTONS_FIGHTBUTTON"},
        {4, "STUFF/UIBUTTONS/UIBUTTONS_MOVEMENTWHEEL"},
        {5, "STUFF/UIBUTTONS/UIBUTTONS_MOVEARROW"},
        {6, "STUFF/UIBUTTONS/UIBUTTONS_LOCK_LOCKED"},
        {7, "STUFF/UIBUTTONS/UIBUTTONS_LOCK_UNLOCKED"},
        {8, "STUFF/UIBUTTONS/UIBUTTONS_CROSSHAIR"},
    };
    for (u32 i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i) {
        s_textures[textures[i].index] = static_cast<i16>(
            NuTexRead(const_cast<char *>(textures[i].name), &permbuffer_ptr, permbuffer_end));
    }
    hasDoneLoadPerm = 1;
}

MechInputTouchVirtualConsoleController::MechInputTouchVirtualConsoleController(i32 player)
    : MechInputTouchMainController(player), active(0), dpad_touch(NULL), drag_touch(NULL), dpad(NULL),
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

    const bool locked = SuperOptions.dpad_locked != 0;
    if (!locked || touch.down_position.x > 0.0f) {
        if (dpad_touch == NULL) {
            dpad_touch = &touch;
        }
        if (!locked) {
            return true;
        }
    }

    if (drag_touch != NULL || touch.down_position.x >= 0.0f) {
        return true;
    }
    if (s_noInputTimer >= 0.0f) {
        MechTouchUIAnimation *animations =
            reinterpret_cast<MechTouchUIAnimation *>(reinterpret_cast<u8 *>(dpad) + 0x40);
        animations[0].Start(*animations[0].target, 1.0f, 0.15f);
        animations[1].Start(*animations[1].target, 1.0f, 0.15f);
        dpad->position.x = touch.down_position.x;
        dpad->position.y = touch.down_position.y;
    }
    dpad->owner = &touch;
    drag_touch = &touch;
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
    if (SuperOptions.dpad_locked && dpad_touch->down_position.x < 0.0f) {
        return;
    }
    const f32 dx = dpad_touch->down_position.x - dpad_touch->touch_position.x;
    const f32 dy = dpad_touch->down_position.y - dpad_touch->touch_position.y;
    const f32 distance = NuFsqrt(dx * dx + dy * dy);
    if (distance <= 0.05f || dpad_touch->held_time <= 0.2f) {
        return;
    }
    const i32 angle = NuAtan2D(dx, dy);
    const f32 strength = MAX(0.0f, MIN((distance - 0.05f) * 4.0f, 1.0f)) * 1.4f;
    const f32 stick_x = -(strength * NU_SIN_LUT(angle));
    const f32 stick_y = strength * NU_COS_LUT(angle);
    stick_values[2] = MAX(-1.0f, MIN(stick_x, 1.0f));
    stick_values[3] = MAX(-1.0f, MIN(stick_y, 1.0f));
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
    MechTouchUIPartySelector *selector = MechSystems::Get()->PlayerButton().selector;
    if (selector != NULL && selector->field_0x88 == 0) {
        return selector->BlendedOut();
    }
    return true;
}

void MechInputTouchVirtualConsoleController::Update(NuInputTouchData const *) {
    STUBBED();
}

void MechInputTouchVirtualConsoleController::UpdateButtonPositions() {
    const f32 radius = NuIOS_IsSmallScreen() ? 0.29f : 0.23f;
    const f32 aspect = GetAspectRatio();
    const f32 x = SuperOptions.right_control_x;
    const f32 y = SuperOptions.right_control_y;
    const f32 dx = aspect * radius;

    buttons[0]->position.x = x;
    buttons[0]->position.y = y - radius;
    buttons[0]->position.z = 0.0f;
    buttons[0]->position.w = 1.0f;

    buttons[1]->position.x = x + dx;
    buttons[1]->position.y = y;
    buttons[1]->position.z = 0.0f;
    buttons[1]->position.w = 1.0f;

    buttons[2]->position.x = x;
    buttons[2]->position.y = y + radius;
    buttons[2]->position.z = 0.0f;
    buttons[2]->position.w = 1.0f;

    buttons[3]->position.x = x - dx;
    buttons[3]->position.y = y;
    buttons[3]->position.z = 0.0f;
    buttons[3]->position.w = 1.0f;

    if (button_mover != NULL) {
        button_mover->position.x = x;
        button_mover->position.y = y;
    }
}

void MechInputTouchVirtualConsoleController::UpdateDPadPos() {
    dpad->position.x = SuperOptions.left_control_x;
    dpad->position.y = SuperOptions.left_control_y;
}

MechInputTouchVirtualConsoleController::~MechInputTouchVirtualConsoleController() {
}
