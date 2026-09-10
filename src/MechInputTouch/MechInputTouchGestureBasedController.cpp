#include "MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"

i32 GetMenuID();

void MechInputTouchGestureBasedController::Activate() {
}

void MechInputTouchGestureBasedController::Deactivate() {
}

void MechInputTouchGestureBasedController::KillTasks(bool kill_active_task) {
    GameObject_s *object = Player[player_id];
    if (object == NULL) {
        return;
    }
    if (object->touch_task == NULL) {
        return;
    }
    if ((object->touch_task->flags & 2) == 0 || kill_active_task) {
        object->KillTasks();
    }
}

MechInputTouchGestureBasedController::MechInputTouchGestureBasedController(
    i32 index, MechInputTouchGestureBasedController::StickMode mode)
    : MechInputTouchMainController(index), temporary_position(), stick_mode(mode), field_a5(0), target(NULL) {
    active = 0;
    field_a6 = 0;
    field_98 = 0.0f;
    current_task = NULL;
    field_90 = NULL;
    field_94 = NULL;
    field_9c = 0.0f;
    field_a0 = 0.0f;
}

bool MechInputTouchGestureBasedController::MenuDisable() {
    const i32 menu_id = GetMenuID();
    if (menu_id == 12 || menu_id == 16 || menu_id == 13 || menu_id == 17 || menu_id == 8 || menu_id == 18) {
        return true;
    }
    return menu_id == 14;
}

bool MechInputTouchGestureBasedController::OnClick(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureBasedController::OnDoubleClick(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureBasedController::OnDown(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureBasedController::OnHold(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureBasedController::OnRelease(GameObject_s &, TouchHolder &) {
    return false;
}

bool MechInputTouchGestureBasedController::OnSwipe(GameObject_s &, TouchHolder &, i32) {
    return false;
}

void MechInputTouchGestureBasedController::PerformCloseMechanic(GameObject_s &, TouchHolder &) {
}

void MechInputTouchGestureBasedController::ProcessAutoJumpOverGap(GameObject_s *) {
}

void MechInputTouchGestureBasedController::ProcessAutoJumpWhenStuck(GameObject_s &) {
}

void MechInputTouchGestureBasedController::ProcessDragMovement(GameObject_s &) {
}

void MechInputTouchGestureBasedController::Render() {
}

void MechInputTouchGestureBasedController::StartJumpUsingAIPath(JumpTriggerPacket const &, i32) {
}

void MechInputTouchGestureBasedController::StartNewTask(MechTouchTask *, TouchHolder &, bool, bool) {
}

void MechInputTouchGestureBasedController::TriggerJumpTask(JumpTriggerPacket const &, bool, bool, bool) {
}

void MechInputTouchGestureBasedController::Update(NuInputTouchData const *) {
}

MechInputTouchGestureBasedController::~MechInputTouchGestureBasedController() {
}
