#include "decomp.h"
#include "MechInputTouch_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>

void PerformPauseButtonStuff();
extern i32 hub_freeplay_area;
bool backButtonPressedLastFrame = false;

f32 MechInputTouchMenuController::PackButtonW = 0.0f;
f32 MechInputTouchMenuController::PackButtonX = 0.0f;
f32 MechInputTouchMenuController::PackButtonY = 0.0f;
bool MechInputTouchMenuController::PackButtonActive = false;
NuVec2 MechInputTouchMenuController::LastTouchPos = {0.0f, 0.0f};

void MechInputTouchMenuController::Activate() {
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 50);
}

void MechInputTouchMenuController::Deactivate() {
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
}

MechInputTouchMenuController::MechInputTouchMenuController(i32 player_id)
    : MechInputTouchMainController(player_id), field_70(0), field_74(0), field_78(0) {
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 50);
}

bool MechInputTouchMenuController::OnClick(GameObject_s &, TouchHolder &holder) {
    if (!PackButtonActive) {
        return false;
    }
    PackButtonActive = false;
    NUVEC delta = {holder.touch_position.x - PackButtonX, holder.touch_position.y - PackButtonY, 0.0f};
    delta.x /= GetAspectRatio();
    if (NuVecMag(&delta) < PackButtonW) {
        PackButtonPressed = true;
        return true;
    }
    return false;
}

bool MechInputTouchMenuController::OnDoubleClick(GameObject_s &, TouchHolder &) {
    bool handled = false;
    asm volatile(".rept 6\n\tnop\n\t.endr" : "+a"(handled));
    return handled;
}

bool MechInputTouchMenuController::OnDown(GameObject_s &, TouchHolder &holder) {
    if (field_70 != NULL) {
        return false;
    }

    MENU &menu = GameMenu[GameMenuLevel];
    const NuVec2 touch = holder.down_position;
    LastTouchTime = GlobalTimer.time_elapsed;
    if (GetMenuID() == 17) {
        COLLECTION_s *collection = GetFreePlayCollection(hub_freeplay_area);
        const f32 radius = fabsf(0.5f * collection->field_14);
        for (i32 index = 0; index < collection->count_y; ++index) {
            const COLLECTID &item = collection->list[index];
            NUVEC delta = {touch.x - item.grid_x, touch.y - item.grid_y, 0.0f};
            delta.x /= GetAspectRatio();
            if (NuVecMag(&delta) < radius) {
                menu.queued_item = index;
                return true;
            }
        }
    }

    field_74 = &menu;
    field_70 = &holder;
    field_78 = GetMenuID() == 12;
    if (!PackButtonActive) {
        return false;
    }
    NUVEC delta = {touch.x - PackButtonX, touch.y - PackButtonY, 0.0f};
    delta.x /= GetAspectRatio();
    return NuVecMag(&delta) < PackButtonW;
}

bool MechInputTouchMenuController::OnHold(GameObject_s &, TouchHolder &holder) {
    const NuVec2 touch = holder.touch_position;
    MENU &menu = GameMenu[GameMenuLevel];
    LastTouchTime = GlobalTimer.time_elapsed;
    if (GetMenuID() != 17) {
        return false;
    }

    COLLECTION_s *collection = GetFreePlayCollection(hub_freeplay_area);
    const f32 radius = fabsf(0.5f * collection->field_14);
    for (i32 index = 0; index < collection->count_y; ++index) {
        const COLLECTID &item = collection->list[index];
        NUVEC delta = {touch.x - item.grid_x, touch.y - item.grid_y, 0.0f};
        delta.x /= GetAspectRatio();
        if (NuVecMag(&delta) < radius) {
            menu.queued_item = index;
            return true;
        }
    }
    return false;
}

bool MechInputTouchMenuController::OnRelease(GameObject_s &, TouchHolder &) {
    STUBBED();
    return false;
}

bool MechInputTouchMenuController::OnSwipe(GameObject_s &, TouchHolder &, i32) {
    bool handled = false;
    asm volatile(".rept 6\n\tnop\n\t.endr" : "+a"(handled));
    return handled;
}

void MechInputTouchMenuController::Render() {
    asm volatile(".rept 8\n\tnop\n\t.endr");
}

void MechInputTouchMenuController::Update(NuInputTouchData const *input) {
    for (i32 index = 0; index < 4; ++index) {
        stick_values[index] = 0.0f;
    }
    if (input != NULL) {
        AnyTouchesThisFrame = AnyTouchesThisFrame > 0 ? AnyTouchesThisFrame - 1 : 0;
    }
}

void MechInputTouchMenuController::UpdateButtons(i32 button) {
    bool pressed = button < 0;
    if (!pressed && backButtonPressedLastFrame) {
        PerformPauseButtonStuff();
    }
    backButtonPressedLastFrame = pressed;
}

MechInputTouchMenuController::~MechInputTouchMenuController() {
}
