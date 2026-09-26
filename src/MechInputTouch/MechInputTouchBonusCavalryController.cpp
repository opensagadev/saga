#include "decomp.h"
#include "MechInputTouch_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

i32 GetMenuID();

void MechInputTouchBonusCavalryController::Activate() {
    if (active) {
        return;
    }
    active = 1;
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
    MechSystems::Get()->gesture_controller = reinterpret_cast<MechInputTouchGestureBasedController *>(this);
}

void MechInputTouchBonusCavalryController::Deactivate() {
    if (!active) {
        return;
    }
    active = 0;
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
}

MechInputTouchBonusCavalryController::MechInputTouchBonusCavalryController(i32 index)
    : MechInputTouchMainController(index) {
    active = 0;
    touch = NULL;
}

bool MechInputTouchBonusCavalryController::OnDown(GameObject_s &, TouchHolder &holder) {
    if (touch != NULL) {
        return true;
    }
    touch = &holder;
    return true;
}

bool MechInputTouchBonusCavalryController::OnRelease(GameObject_s &, TouchHolder &holder) {
    if (touch != &holder) {
        return true;
    }
    touch = NULL;
    return true;
}

void MechInputTouchBonusCavalryController::Update(NuInputTouchData const *) {
    if (player != NULL && NewMode == 0 && NewLData == NULL && FadeSys.fade == 0.0f && Paused == 0 &&
        CUTSTOPGAME == 0 && GetMenuID() != 12 && GetMenuID() != 16 && TouchHacks::TouchControlsActive &&
        MiniCutCam != 2 &&
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(MechSystems::Get()) + 0x271c) == 0) {
        Activate();
    } else {
        Deactivate();
    }

    stick_values[0] = 0.0f;
    stick_values[1] = 0.0f;
    GameObject_s *object = player;
    if (touch == NULL || object == NULL) {
        UpdateButtons();
        return;
    }
    const f32 drag_x = touch->touch_position.x - object->camera_screen_position.x;
    const f32 drag_y = touch->touch_position.y - object->camera_screen_position.y;
    const f32 drag_length_squared = drag_x * drag_x + drag_y * drag_y;
    const f32 drag_length = NuFsqrt(drag_length_squared);
    const f32 drag_scale = WORLD->current_level == BONUS_GUNSHIPB_LDATA ? 0.2f : 0.5f;
    if (drag_length <= 0.05f) {
        UpdateButtons();
        return;
    }
    const f32 normalized_length = NuFsqrt(drag_length_squared);
    const f32 inverse_length = normalized_length == 0.0f ? 0.0f : 1.0f / normalized_length;
    const f32 direction_x = drag_x * inverse_length;
    const f32 direction_y = drag_y * inverse_length;

    GameObject_s *facing_object = player;
    const u16 angle = facing_object->apiobj.facing_angle;
    const f32 radius = facing_object->field_0x1008;
    VuVec world_target(facing_object->apiobj.position.x + radius * NU_SIN_LUT(angle),
                       facing_object->apiobj.position.y,
                       facing_object->apiobj.position.z + radius * NU_COS_LUT(angle), 1.0f);
    VuVec projected;
    NuCameraTransformScreenClip(&projected.xyz, &world_target.xyz, 1, NULL);
    GameObject_s *screen_object = player;
    const f32 forward_x = projected.x - screen_object->camera_screen_position.x;
    const f32 forward_y = projected.y - screen_object->camera_screen_position.y;
    const f32 forward_length = NuFsqrt(forward_x * forward_x + forward_y * forward_y);
    const f32 inverse_forward_length = forward_length == 0.0f ? 0.0f : 1.0f / forward_length;
    const f32 normalized_forward_x = forward_x * inverse_forward_length;
    const f32 normalized_forward_y = forward_y * inverse_forward_length;
    const f32 along = direction_x * normalized_forward_x + direction_y * normalized_forward_y;
    const f32 across = direction_x * normalized_forward_y - direction_y * normalized_forward_x;
    const f32 intensity = MAX(0.0f, MIN(1.0f, drag_length / drag_scale));
    stick_values[1] = -along * intensity;
    stick_values[0] = across * intensity;
    if (WORLD->current_level == BONUS_GUNSHIPB_LDATA && __builtin_fabsf(stick_values[0]) > 0.3f) {
        stick_values[0] = stick_values[0] < 0.0f ? -1.0f : 1.0f;
    }
    UpdateButtons();
}

MechInputTouchBonusCavalryController::~MechInputTouchBonusCavalryController() {
}
