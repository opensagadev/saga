#include "MechInputTouch_types.h"

#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

i32 GetMenuID();
GIZTURRET_s *GizTurret_FindByController(GIZTURRETSYS_s *, GameObject_s &);

void MechInputTouchDeathStarTurretController::Activate() {
    if (active) {
        return;
    }
    active = 1;
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
    MechSystems::Get()->active_main_controller = this;
    turret = NULL;
    if (player != NULL && WORLD != NULL) {
        turret = GizTurret_FindByController(WORLD->giz_turret_sys, *player);
    }
}

void MechInputTouchDeathStarTurretController::Deactivate() {
    if (active) {
        active = 0;
        MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
    }
}

MechInputTouchDeathStarTurretController::MechInputTouchDeathStarTurretController(i32 player_index)
    : MechInputTouchMainController(player_index) {
    active = 0;
    aim_touch = NULL;
    turret = NULL;
}

bool MechInputTouchDeathStarTurretController::OnDown(GameObject_s &, TouchHolder &holder) {
    if (aim_touch == NULL) {
        aim_touch = &holder;
    }
    return true;
}

bool MechInputTouchDeathStarTurretController::OnRelease(GameObject_s &, TouchHolder &holder) {
    if (aim_touch == &holder) {
        aim_touch = NULL;
    }
    return true;
}

bool MechInputTouchDeathStarTurretController::OnSwipe(GameObject_s &, TouchHolder &holder, i32 direction) {
    const f32 *previous_y =
        reinterpret_cast<const f32 *>(reinterpret_cast<const u8 *>(&holder) + direction * 44 + 0x38);
    if (holder.touch_position.y > *previous_y) {
        button_was_pressed[2] = 1;
        return true;
    }
    // Retail returns the holder address in EAX here; callers test its low byte.
    // Preserve that result without falling off a non-void function (C++ UB).
    return (reinterpret_cast<usize>(&holder) & 0xff) != 0;
}

void MechInputTouchDeathStarTurretController::Update(NuInputTouchData const *) {
    if (player != NULL && NewMode == 0 && NewLData == NULL && FadeSys.fade == 0.0f && Paused == 0 && CUTSTOPGAME == 0 &&
        GetMenuID() != 12 && GetMenuID() != 16 && TouchHacks::TouchControlsActive && MiniCutCam != 2 &&
        MechSystems::Get()->PlayerButton().selector == NULL) {
        Activate();
    } else {
        Deactivate();
    }

    stick_values[0] = 0.0f;
    stick_values[1] = 0.0f;
    if (aim_touch != NULL && player != NULL && turret != NULL) {
        button_was_pressed[0] = 1;
        const f32 touch_x = aim_touch->touch_position.x;
        const f32 touch_y = aim_touch->touch_position.y;
        NUVEC screen_position;
        NuCameraTransformScreenClip(&screen_position, &turret->position, 1, NULL);
        const f32 horizontal_offset = touch_x - screen_position.x;
        const f32 vertical_offset = touch_y - screen_position.y;
        const i32 horizontal_angle = static_cast<i32>(horizontal_offset * 8192.0f);
        i32 camera_angle = NuAtan2D(GameCam->pos.z - GameCam->target.z, GameCam->pos.x - GameCam->target.x);
        camera_angle -= 0x8000;
        i32 yaw_target = horizontal_angle - camera_angle;
        if (vertical_offset > 0.0f) {
            const u16 pitch_target = static_cast<u16>(static_cast<i32>(vertical_offset * 3641.0f));
            turret->pitch = SeekRot(turret->pitch, pitch_target, 8.0f);
        }
        yaw_target &= 0xffff;
        turret->yaw = SeekRot(turret->yaw, yaw_target, 8.0f);
    }
    UpdateButtons();
}

MechInputTouchDeathStarTurretController::~MechInputTouchDeathStarTurretController() {
}
