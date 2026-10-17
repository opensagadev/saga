// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "globals.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/NuInputDevice.h"
#include <string.h>
#include "gameframework/saveload.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/base/apiobject.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/audio/sfx.h"
#include <stdio.h>
#include "legoapi/render/core/screen.h"
#include "legoapi/world/mission.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechTouchUIElements.h"
#include "legoapi/world/levels/hub.h"

bool backButtonPressedLastFrame = false;

f32 MechInputTouchMenuController::PackButtonW = 0.0f;
f32 MechInputTouchMenuController::PackButtonX = 0.0f;
f32 MechInputTouchMenuController::PackButtonY = 0.0f;
bool MechInputTouchMenuController::PackButtonActive = false;
NuVec2 MechInputTouchMenuController::LastTouchPos = {0.0f, 0.0f};

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.

void MechInputTouchMenuController::Update(NuInputTouchData const *input) {
    for (i32 index = 0; index < 4; ++index) {
        stick_values[index] = 0.0f;
    }
    if (input != NULL) {
        i32 *counter = &AnyTouchesThisFrame;
        i32 zero = 0;
        i32 remaining = *counter - 1;
        if (remaining < 0) {
            *counter = zero;
        } else {
            *counter = remaining;
        }
    }
}

void MechInputTouchMenuController::Render() {
}

bool MechInputTouchMenuController::OnDoubleClick(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchMenuController::OnSwipe(GameObject_s &, TouchHolder &, i32) {
    return false;
}

void MechInputTouchMenuController::Activate() {
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 50);
}

void MechInputTouchMenuController::Deactivate() {
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
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

bool MechInputTouchMenuController::OnHold(GameObject_s &, TouchHolder &holder) {
    const volatile NuVec2 &touch_position = holder.touch_position;
    const f32 touch_y = touch_position.y;
    const f32 touch_x = touch_position.x;
    MENU &menu = GameMenu[GameMenuLevel];
    LastTouchTime = GlobalTimer.time_elapsed;
    if (GetMenuID() != 17) {
        return false;
    }

    COLLECTION_s *collection = GetFreePlayCollection(hub_freeplay_area);
    const f32 radius = fabsf(0.5f * collection->field_14);
    for (i32 index = 0; index < collection->count_y; ++index) {
        const COLLECTID &item = collection->list[index];
        NUVEC delta = {touch_x - item.grid_x, touch_y - item.grid_y, 0.0f};
        delta.x /= GetAspectRatio();
        if (NuVecMag(&delta) < radius) {
            menu.queued_item = index;
            return true;
        }
    }
    return false;
}

bool MechInputTouchMenuController::OnRelease(GameObject_s &, TouchHolder &holder) {
    if (field_70 != &holder) {
        field_70 = NULL;
        return false;
    }

    const NuVec2 down = holder.down_position;
    const NuVec2 touch = holder.touch_position;
    const f32 dy = touch.y - down.y;
    const f32 dx = touch.x - down.x;
    LastTouchTime = GlobalTimer.time_elapsed;
    LastTouchPos = touch;
    AnyTouchesThisFrame = 3;

    if (GetMenuID() == 12) {
        if (field_78 == 0) {
            field_70 = NULL;
            return false;
        }
        CUSTOMISER *customiser = CharacterCustomiser;
        if (dx > 0.1f || dx < -0.1f) {
            customiser->field_0xd16 = 1;
            return true;
        }
        if (dy > 0.1f) {
            customiser->field_0xd12 = 1;
            return true;
        }
        if (dy < -0.1f) {
            customiser->field_0xd13 = 1;
            return true;
        }

#define CUSTOMISER_HIT(index)                                                                                          \
    {                                                                                                                  \
        const f32 width = customiser->touch_widths[index];                                                             \
        if (width > 0.0f) {                                                                                            \
            const f32 x = down.x - customiser->touch_positions[index].x;                                               \
            const f32 centre_y = customiser->touch_positions[index].y;                                                 \
            const f32 height = customiser->touch_heights[index];                                                       \
            const f32 half_width = fabsf(0.5f * width);                                                                \
            if (x > -half_width && x < half_width) {                                                                   \
                const f32 y = down.y - centre_y;                                                                       \
                const f32 half_height = fabsf(0.5f * height);                                                          \
                if (y > -half_height && y < half_height) {                                                             \
                    customiser->touch_requests[index] = 1;                                                             \
                    return true;                                                                                       \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    }
        CUSTOMISER_HIT(0);
        CUSTOMISER_HIT(1);
        CUSTOMISER_HIT(2);
        CUSTOMISER_HIT(3);
        CUSTOMISER_HIT(4);
        CUSTOMISER_HIT(5);
#undef CUSTOMISER_HIT
    }

    MENU &menu = GameMenu[GameMenuLevel];
    if (GetMenuID() == 17) {
        GetFreePlayCollection(hub_freeplay_area);
        NUVEC delta = {touch.x - menu.item_x[0], touch.y - menu.item_y[0], 0.0f};
        delta.x /= GetAspectRatio();
        if (NuVecMag(&delta) < menu.item_width[0]) {
            menu.queued_item = 9999;
            return true;
        }
        return false;
    }

    if (GameMenuLevel <= 0) {
        field_74 = NULL;
        field_70 = NULL;
        return false;
    }
    bool same_menu = field_74 == &menu;
    field_70 = NULL;
    field_74 = NULL;
    if (!same_menu) {
        return false;
    }

    if (dx > 0.1f) {
        menu.horizontal_scroll_distance = dx;
        menu.move_left = 1;
        return GetMenuID() != 25;
    }
    if (dx < -0.1f) {
        menu.move_right = 1;
        menu.horizontal_scroll_distance = -dx;
        return GetMenuID() != 25;
    }
    if (dy > 0.1f) {
        menu.horizontal_scroll_distance = dy;
        menu.move_down = 1;
        return GetMenuID() != 25;
    }
    if (dy < -0.1f) {
        menu.move_up = 1;
        menu.horizontal_scroll_distance = -dy;
        return GetMenuID() != 25;
    }

    for (i32 index = 0; index < 400; ++index) {
        const f32 width = menu.item_width[index];
        if (!(width > 0.0f)) {
            continue;
        }
        const f32 height = menu.item_height[index];
        const f32 x = down.x - menu.item_x[index];
        const f32 y = down.y - menu.item_y[index];
        bool hit;
        if (height == 0.0f) {
            NUVEC delta = {x, y, 0.0f};
            delta.x /= GetAspectRatio();
            hit = NuVecMag(&delta) < width;
        } else {
            const f32 half_width = fabsf(0.5f * width);
            const f32 half_height = fabsf(0.5f * height);
            hit = x > -half_width && x < half_width && y > -half_height && y < half_height;
        }
        if (hit) {
            menu.queued_item = index;
            menu.queued_column = menu.item_column[index];
            menu.queued_row = menu.item_row[index];
            return GetMenuID() != 25;
        }
    }
    return false;
}

bool MechInputTouchMenuController::OnClick(GameObject_s &, TouchHolder &holder) {
    const volatile NuVec2 &touch = holder.touch_position;
    const f32 touch_y = touch.y;
    const f32 touch_x = touch.x;
    if (!PackButtonActive) {
        return false;
    }
    PackButtonActive = false;
    NUVEC delta = {touch_x - PackButtonX, touch_y - PackButtonY, 0.0f};
    delta.x /= GetAspectRatio();
    if (NuVecMag(&delta) < PackButtonW) {
        PackButtonPressed = true;
        return true;
    }
    return false;
}

bool MechInputTouchMenuController::OnDown(GameObject_s &, TouchHolder &holder) {
    if (field_70 != NULL) {
        return false;
    }

    MENU &menu = GameMenu[GameMenuLevel];
    const volatile NuVec2 &touch_position = holder.down_position;
    const f32 touch_y = touch_position.y;
    const f32 touch_x = touch_position.x;
    LastTouchTime = GlobalTimer.time_elapsed;
    if (GetMenuID() == 17) {
        COLLECTION_s *collection = GetFreePlayCollection(hub_freeplay_area);
        const f32 radius = fabsf(0.5f * collection->field_14);
        for (i32 index = 0; index < collection->count_y; ++index) {
            const COLLECTID &item = collection->list[index];
            NUVEC delta = {touch_x - item.grid_x, touch_y - item.grid_y, 0.0f};
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
    NUVEC delta = {touch_x - PackButtonX, touch_y - PackButtonY, 0.0f};
    delta.x /= GetAspectRatio();
    return NuVecMag(&delta) < PackButtonW;
}

MechInputTouchMenuController::MechInputTouchMenuController(i32 player_id)
    : MechInputTouchMainController(player_id), field_70(0), field_74(0), field_78(0) {
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 50);
}

void VirtualControlDPad_LockButton::Process(float) {
    const f32 button_radius_x = radius_x;
    const f32 button_radius_y = radius_y;
    position = dpad->position;
    const f32 desired_y = dpad->position.y - (dpad->position.w * 0.75f + button_radius_y);
    const f32 desired_x = dpad->position.z * 0.75f + button_radius_x + dpad->position.x;
    position.x = MAX(button_radius_x - 0.975f, MIN(desired_x, 0.1f));
    position.y = MAX(button_radius_y - 0.975f, MIN(desired_y, 1.0f - button_radius_y));
    alpha = alpha_to = hovered != 0 ? 1.0f : 0.4f;
    alpha_elapsed = alpha_duration;
    scale = scale_to = hovered != 0 ? 1.2f : 1.0f;
    scale_elapsed = scale_duration;
}

void VirtualControlDPad_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch) {
    f32 *offset = reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(&element) + 0x80);
    offset[0] = element.position.x - touch.down_position.x;
    offset[1] = element.position.y - touch.down_position.y;
}

void VirtualControlButton_OnDown_Callback(MechTouchUIElement &, TouchHolder &) {
}

void VirtualControlButtonMover_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch) {
    f32 *offset = reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(&element) + 0x7c);
    offset[0] = element.position.x - touch.down_position.x;
    offset[1] = element.position.y - touch.down_position.y;
}

void VirtualControlButton::Render() {
    MechTouchUITexButton::Render();
}

void VirtualControlDPad_LockButton::Render() {
    MechTouchUITexButton::Render();
}

void VirtualControlButton::Process(float) {
    if (hovered != 0 && GetMenuID() != 25) {
        MechSystems::Get()->gesture_controller->button_was_pressed[button_type] = 1;
        MechInputTouchVirtualConsoleController::s_noInputTimer = 20.0f;
    }

    alpha = alpha_to = hovered != 0 || MechInputTouchVirtualConsoleController::s_noInputTimer < 0.0f ? 1.0f : 0.4f;
    alpha_elapsed = alpha_duration;
    scale = scale_to = hovered != 0 ? 1.2f : 1.0f;
    scale_elapsed = scale_duration;
    if (MechInputTouchVirtualConsoleController::s_noInputTimer < 0.0f) {
        const f32 pulse =
            1.0f +
            0.05f * NU_SIN_LUT(static_cast<i32>(MechInputTouchVirtualConsoleController::s_noInputTimer * 65536.0f));
        scale = scale_to = scale * pulse;
    }
}

