#include "decomp.h"
#include "MechInputTouch_types.h"
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
    return (diff < 0 ? -diff : diff) <= 0x1c71;
}

bool MechInputTouchSpeederChaseController::IsSwipeAgainstDirection(NuVec2 const &start,
                                                                   NuVec2 const &end, bool direction) {
    return direction ? IsDownSwipe(start, end) : IsUpSwipe(start, end);
}

bool MechInputTouchSpeederChaseController::IsSwipeWithDirection(NuVec2 const &start, NuVec2 const &end,
                                                                bool direction) {
    return direction ? IsUpSwipe(start, end) : IsDownSwipe(start, end);
}

bool MechInputTouchSpeederChaseController::IsUpSwipe(NuVec2 const &start, NuVec2 const &end) {
    i32 diff = RotDiff(static_cast<u16>(NuAtan2D(end.x - start.x, end.y - start.y)), 0);
    return (diff < 0 ? -diff : diff) <= 0x1c71;
}

MechInputTouchSpeederChaseController::MechInputTouchSpeederChaseController(i32 player)
    : MechInputTouchMainController(player), touch_holder(NULL), swipe_y(0.0f), cooldown(0.0f), active(false),
      swipe_direction(false) {
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
    const NuVec2 &start = *reinterpret_cast<const NuVec2 *>(
        reinterpret_cast<const u8 *>(&touch) + 0x34 + index * 0x2c);
    const NuVec2 &end = touch.touch_position;
    if (IsSwipeAgainstDirection(start, end, swipe_direction)) {
        swipe_direction ^= 1;
    } else if (IsSwipeWithDirection(start, end, swipe_direction)) {
        button_was_pressed[2] = 1;
        button_was_pressed[0] = 1;
    } else if (!IsDownSwipe(start, end)) {
        IsUpSwipe(start, end);
    }
    swipe_y = end.x;
    cooldown = 0.4f;
    return true;
}

void MechInputTouchSpeederChaseController::Update(NuInputTouchData const *) {
    STUBBED();
}

MechInputTouchSpeederChaseController::~MechInputTouchSpeederChaseController() {
}
