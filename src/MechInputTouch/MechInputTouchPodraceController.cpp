#include "decomp.h"
#include "MechInputTouch_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/common.h"

i32 GetMenuID();

void MechInputTouchPodraceController::Activate() {
    if (active) {
        return;
    }
    active = 1;
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
    MechSystems::Get()->gesture_controller = reinterpret_cast<MechInputTouchGestureBasedController *>(this);
}

void MechInputTouchPodraceController::Deactivate() {
    if (!active) {
        return;
    }
    active = 0;
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
}

MechInputTouchPodraceController::MechInputTouchPodraceController(i32 index) : MechInputTouchMainController(index) {
    active = 0;
    steering_touch = NULL;
}

bool MechInputTouchPodraceController::OnDown(GameObject_s &, TouchHolder &holder) {
    if (steering_touch != NULL) {
        return true;
    }
    steering_touch = &holder;
    return true;
}

bool MechInputTouchPodraceController::OnRelease(GameObject_s &, TouchHolder &holder) {
    if (steering_touch != &holder) {
        return true;
    }
    steering_touch = NULL;
    return true;
}

void MechInputTouchPodraceController::Update(NuInputTouchData const *) {
    if (player != NULL && NewMode == 0 && NewLData == NULL && FadeSys.fade == 0.0f && Paused == 0 && CUTSTOPGAME == 0 &&
        GetMenuID() != 12 && GetMenuID() != 16 && TouchHacks::TouchControlsActive && MiniCutCam != 2 &&
        MechSystems::Get()->PlayerButton().selector == NULL) {
        Activate();
    } else {
        Deactivate();
    }

    stick_values[0] = 0.0f;
    stick_values[1] = 0.0f;
    if (steering_touch == NULL || WORLD == NULL) {
        return;
    }

    if (WORLD->area == PODSPRINT_ADATA) {
        stick_values[1] = -1.0f;
    }
    button_was_pressed[2] = 1;
    const f32 steering = steering_touch->touch_position.x * 3.0f;
    stick_values[0] = MAX(-1.0f, (MIN(steering, 1.0f)));
    UpdateButtons();
}

MechInputTouchPodraceController::~MechInputTouchPodraceController() {
}
