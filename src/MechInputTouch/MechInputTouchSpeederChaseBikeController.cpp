#include "decomp.h"
#include "MechInputTouch_types.h"
#include "gameapi/gui/apimenu.h"
#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/nutrig.h"

i32 RotDiff(u16, u16);

void MechInputTouchSpeederChaseController::Activate() {
    if (!active) {
        active = true;
        MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
        MechSystems::Get()->gesture_controller = reinterpret_cast<MechInputTouchGestureBasedController *>(this);
        swipe_direction = true;
    }
}

void MechInputTouchSpeederChaseController::Deactivate() {
    if (active) {
        active = false;
        MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
    }
}

bool MechInputTouchSpeederChaseController::IsDownSwipe(NuVec2 const &start, NuVec2 const &end) {
    i32 diff = RotDiff(static_cast<u16>(NuAtan2D(end.x - start.x, end.y - start.y)), 0x8000);
    i32 sign = diff >> 31;
    diff ^= sign;
    diff -= sign;
    return diff <= 0x1c71;
}

i32 MechInputTouchSpeederChaseController::IsSwipeAgainstDirection(NuVec2 const &start, NuVec2 const &end,
                                                                  bool direction) {
    return direction ? IsDownSwipe(start, end) : IsUpSwipe(start, end);
}

i32 MechInputTouchSpeederChaseController::IsSwipeWithDirection(NuVec2 const &start, NuVec2 const &end, bool direction) {
    return direction ? IsUpSwipe(start, end) : IsDownSwipe(start, end);
}

bool MechInputTouchSpeederChaseController::IsUpSwipe(NuVec2 const &start, NuVec2 const &end) {
    i32 diff = RotDiff(static_cast<u16>(NuAtan2D(end.x - start.x, end.y - start.y)), 0);
    i32 sign = diff >> 31;
    diff ^= sign;
    diff -= sign;
    return diff <= 0x1c71;
}

MechInputTouchSpeederChaseController::MechInputTouchSpeederChaseController(i32 player)
    : MechInputTouchMainController(player) {
    active = false;
    touch_holder = NULL;
    cooldown = 0.0f;
    swipe_y = 0.0f;
}

bool MechInputTouchSpeederChaseController::OnClick(GameObject_s &, TouchHolder &) {
    button_was_pressed[0] = 1;
    return true;
}

bool MechInputTouchSpeederChaseController::OnDoubleClick(GameObject_s &, TouchHolder &) {
    button_was_pressed[0] = 1;
    return true;
}

bool MechInputTouchSpeederChaseController::OnDown(GameObject_s &, TouchHolder &touch) {
    if (touch_holder == NULL) {
        touch_holder = &touch;
    }
    return true;
}

bool MechInputTouchSpeederChaseController::OnRelease(GameObject_s &, TouchHolder &touch) {
    if (touch_holder == &touch) {
        touch_holder = NULL;
    }
    return true;
}

bool MechInputTouchSpeederChaseController::OnSwipe(GameObject_s &, TouchHolder &touch, i32 index) {
    const NuVec2 &start = *reinterpret_cast<const NuVec2 *>(reinterpret_cast<const u8 *>(&touch) + 0x34 + index * 0x2c);
    const NuVec2 &end = touch.touch_position;
    if (IsSwipeAgainstDirection(start, end, swipe_direction)) {
        swipe_direction ^= 1;
    } else if (IsSwipeWithDirection(start, end, swipe_direction)) {
        button_was_pressed[2] = 1;
        button_was_pressed[0] = 1;
    } else if (!IsDownSwipe(start, end)) {
        IsUpSwipe(start, end);
    }
    cooldown = 0.4f;
    swipe_y = end.x;
    return true;
}

void MechInputTouchSpeederChaseController::Update(NuInputTouchData const *) {
    if (player != NULL && NewMode == 0 && NewLData == NULL && FadeSys.fade == 0.0f && Paused == 0 && CUTSTOPGAME == 0 &&
        GetMenuID() != 12 && GetMenuID() != 16 && TouchHacks::TouchControlsActive && MiniCutCam != 2) {
        Activate();
    } else {
        Deactivate();
    }

    cooldown -= FRAMETIME;
    stick_values[0] = 0.0f;
    stick_values[1] = 0.0f;
    if (cooldown > 0.0f && player != NULL) {
        button_was_pressed[2] = 1;
        stick_values[0] = swipe_y;
        if (player->character_context == 0x3a) {
            cooldown = -1.0f;
        }
    } else if (touch_holder != NULL && WORLD != NULL) {
        if (!swipe_direction) {
            stick_values[1] = 1.0f;
            if (touch_holder->touch_position.y > 0.5f) {
                swipe_direction = true;
            }
        } else {
            stick_values[1] = -1.0f;
            if (touch_holder->touch_position.y < -0.5f) {
                swipe_direction = false;
            }
        }
        float x = touch_holder->touch_position.x;
        if (x < 1.0f) {
            x = x > -1.0f ? x : -1.0f;
        } else {
            x = x < 1.0f ? x : 1.0f;
        }
        stick_values[0] = x;
        button_was_pressed[0] = 1;
    }
    UpdateButtons();
}

MechInputTouchSpeederChaseController::~MechInputTouchSpeederChaseController() {
}
