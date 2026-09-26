#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/numath/nutrig.h"

extern "C" i32 NuIOS_IsSmallScreen();
i32 GetMenuID();
void RndrTexQuad(f32, f32, f32, f32, i32, numtl_s *, i32);

void VirtualControlButton_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlButtonMover_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlDPad_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlDPad_LockButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);

void VirtualControlDPad::Process(float elapsed) {
    MechTouchUITexButton::Process(elapsed);

    const f32 timer = MechInputTouchVirtualConsoleController::s_noInputTimer;
    if (timer >= 0.0f && owner != NULL) {
        alpha = 0.75f;
        alpha_to = 0.75f;
        alpha_elapsed = alpha_duration;
        disabled = 0;
    } else {
        if (timer >= 0.0f && (SuperOptions.dpad_locked == 0 || GetMenuID() == 25)) {
            alpha = 0.4f;
            alpha_to = 0.4f;
            alpha_elapsed = alpha_duration;
        }
        disabled = SuperOptions.dpad_locked != 0 && GetMenuID() != 25;
    }

    if (timer < 0.0f) {
        visible = 1;
        if (scale_duration < 0.0f || scale_elapsed >= scale_duration + scale_delay) {
            const f32 pulse = 1.0f + 0.05f * NU_SIN_LUT(static_cast<i32>(timer * 65536.0f));
            scale_elapsed = scale_duration;
            alpha = 0.75f;
            alpha_to = 0.75f;
            scale = pulse;
            scale_to = pulse;
            alpha_elapsed = alpha_duration;
        }
    }

    if (GetMenuID() == 25) {
        if (owner != NULL) {
            const f32 x = owner->touch_position.x + drag_offset.x;
            const f32 y = owner->touch_position.y + drag_offset.y;
            position.x = MAX(radius_x - 0.975f, MIN(x, -radius_x));
            position.y = MAX(radius_y - 0.975f, MIN(y, 0.0f));
            SuperOptions.left_control_x = position.x;
            SuperOptions.left_control_y = position.y;
        }
        return;
    }

    if (visible == 0) {
        MechSystems::Get()->gesture_controller->stick_values[0] = 0.0f;
        MechSystems::Get()->gesture_controller->stick_values[1] = 0.0f;
    }
    stick_values.x = 0.0f;
    stick_values.y = 0.0f;

    if (owner != NULL) {
        const f32 dx = owner->touch_position.x - position.x;
        const f32 dy = -(owner->touch_position.y - position.y);
        const f32 distance = NuFsqrt(dx * dx + dy * dy);
        const i32 angle = NuAtan2D(dx, dy);
        const f32 ratio = distance / radius_y;
        const f32 strength = ratio >= 1.0f || ratio < 0.0f
                                 ? 1.5f
                                 : static_cast<f32>(static_cast<double>(ratio) * 1.4) * 3.0f;
        stick_values.x = MAX(-1.0f, MIN(NU_SIN_LUT(angle) * strength, 1.0f));
        stick_values.y = MAX(-1.0f, MIN(NU_COS_LUT(angle) * strength, 1.0f));
        MechInputTouchVirtualConsoleController::s_noInputTimer = 20.0f;
    }

    MechSystems::Get()->gesture_controller->stick_values[0] = stick_values.x;
    MechSystems::Get()->gesture_controller->stick_values[1] = stick_values.y;
}

void VirtualControlDPad::Render() {
    MechTouchUITexButton::Render();

    const f32 centre_x = (position.x + 1.0f) * 0.5f;
    const f32 centre_y = (1.0f - position.y) * 0.5f;
    const f32 scaled_x = radius_x * scale;
    const f32 scaled_y = radius_y * scale;
    const f32 arrow_radius = scale * (NuIOS_IsSmallScreen() ? 0.07f : 0.035f);
    const f32 arrow_width = GetAspectRatio() * arrow_radius;
    const bool all_active = MechInputTouchVirtualConsoleController::s_noInputTimer < 0.0f ||
                            (GetMenuID() == 25 && owner != NULL);

    const f32 bottom_factor = stick_values.y > 0.2f || all_active ? 0.75f : 0.5f;
    RndrTexQuad(centre_x, centre_y + 0.4f * scaled_y, arrow_width, arrow_radius,
                (static_cast<i32>(128.0f * alpha * bottom_factor) << 24) | 0x808080, arrow_material, 0x8000);

    const f32 left_factor = stick_values.x < -0.2f || all_active ? 1.0f : 0.5f;
    RndrTexQuad(centre_x - 0.2f * scaled_x, centre_y, arrow_width, arrow_radius,
                (static_cast<i32>(128.0f * alpha * left_factor) << 24) | 0x808080, arrow_material, 0xc000);

    const f32 top_factor = stick_values.y < -0.2f || all_active ? 1.0f : 0.5f;
    RndrTexQuad(centre_x, centre_y - 0.4f * scaled_y, arrow_width, arrow_radius,
                (static_cast<i32>(128.0f * alpha * top_factor) << 24) | 0x808080, arrow_material, 0);

    const f32 right_factor = stick_values.x > 0.2f || all_active ? 1.0f : 0.5f;
    RndrTexQuad(centre_x + 0.2f * scaled_x, centre_y, arrow_width, arrow_radius,
                (static_cast<i32>(128.0f * alpha * right_factor) << 24) | 0x808080, arrow_material, 0x4000);

    if (GetMenuID() == 25 && controller->lock_button != NULL) {
        MechTouchUITexButton *lock = static_cast<MechTouchUITexButton *>(controller->lock_button);
        mover.position = position;
        mover.radius_x = lock->radius_x;
        mover.radius_y = lock->radius_y;
        mover.scale = mover.scale_to = lock->scale;
        mover.scale_elapsed = mover.scale_duration;
        if (owner != NULL) {
            mover.scale = mover.scale_to = 1.1f;
        }
        mover.MechTouchUITexButton::Render();
    }
}

VirtualControlDPad::VirtualControlDPad(NuVec2 const &pos, float radius,
                                       MechInputTouchVirtualConsoleController &console)
    : MechTouchUITexButton(VuVec(pos.x, pos.y, 0.0f, 1.0f), radius), controller(&console), mover(console) {
    on_down = VirtualControlDPad_OnDown_Callback;
    scale = scale_to = 0.0f;
    scale_elapsed = scale_duration;
    stick_values.x = 0.0f;
    stick_values.y = 0.0f;
    UpdateTexture(console.s_textures[4]);

    arrow_material = NuMtlCreate(1);
    arrow_material->sort_pri = 255;
    arrow_material->diffuse_color.r = 0.0f;
    arrow_material->diffuse_color.g = 0.0f;
    arrow_material->diffuse_color.b = 0.0f;
    arrow_material->opacity = 0.0f;
    arrow_material->attribs.cull_mode = 2;
    arrow_material->attribs.z_mode = 1;
    arrow_material->attribs.alpha_mode = 1;
    arrow_material->attribs.unknown_2_1_2 = 2;
    arrow_material->attribs.alpha_test = 1;
    arrow_material->tex_id = console.s_textures[5];
    NuMtlUpdate(arrow_material);
}

VirtualControlDPad::~VirtualControlDPad() {
    NuMtlDestroy(arrow_material);
}

void VirtualControlButton::Process(float) {
    if (hovered != 0 && GetMenuID() != 25) {
        MechSystems::Get()->gesture_controller->button_was_pressed[button_type] = 1;
        MechInputTouchVirtualConsoleController::s_noInputTimer = 20.0f;
    }

    alpha = alpha_to = hovered != 0 || MechInputTouchVirtualConsoleController::s_noInputTimer < 0.0f
                             ? 1.0f
                             : 0.4f;
    alpha_elapsed = alpha_duration;
    scale = scale_to = hovered != 0 ? 1.2f : 1.0f;
    scale_elapsed = scale_duration;
    if (MechInputTouchVirtualConsoleController::s_noInputTimer < 0.0f) {
        const f32 pulse =
            1.0f + 0.05f * NU_SIN_LUT(static_cast<i32>(MechInputTouchVirtualConsoleController::s_noInputTimer * 65536.0f));
        scale = scale_to = scale * pulse;
    }
}

void VirtualControlButton::Render() {
    MechTouchUITexButton::Render();
}

VirtualControlButton::VirtualControlButton(NuVec2 const &pos, float radius,
                                           MechInputTouchMainController::eButtonTypes type)
    : MechTouchUITexButton(VuVec(pos.x, pos.y, 0.0f, 1.0f), radius), button_type(type) {
    on_down = VirtualControlButton_OnDown_Callback;
    switch (type) {
    case 0:
        UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[1]);
        break;
    case 1:
        UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[3]);
        break;
    case 2:
        UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[0]);
        break;
    case 3:
        UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[2]);
        break;
    default:
        break;
    }
}

void VirtualControlButtonMover::Process(float elapsed) {
    pulse_phase += static_cast<i32>(elapsed * 65536.0f);
    f32 pulse = 1.0f + 0.1f * NU_SIN_LUT(pulse_phase);
    scale = scale_to = pulse;
    scale_elapsed = scale_duration;

    if (owner != NULL) {
        scale = scale_to = 1.1f;
        position.x = owner->touch_position.x + drag_offset.x;
        position.y = owner->touch_position.y + drag_offset.y;
        if (NuIOS_IsSmallScreen()) {
            position.x = MAX(0.38f, MIN(position.x, 0.62f));
            position.y = MAX(-0.49f, MIN(position.y, 0.0f));
        } else {
            position.x = MAX(0.23f, MIN(position.x, 0.77f));
            position.y = MAX(-0.62f, MIN(position.y, 0.0f));
        }
        SuperOptions.right_control_x = position.x;
        SuperOptions.right_control_y = position.y;
        controller->UpdateButtonPositions();
    }
}

VirtualControlButtonMover::VirtualControlButtonMover(MechInputTouchVirtualConsoleController &console)
    : MechTouchUITexButton(VuVec(SuperOptions.right_control_x, SuperOptions.right_control_y, 1.0f, 1.0f), 0.1f),
      controller(&console) {
    on_down = VirtualControlButtonMover_OnDown_Callback;
    UpdateTexture(console.s_textures[8]);
    pulse_phase = 0;
}

void VirtualControlDPad_LockButton::Process(float) {
    const f32 button_radius_x = radius_x;
    const f32 button_radius_y = radius_y;
    position = dpad->position;
    const f32 desired_x = dpad->position.z * 0.75f + button_radius_x + dpad->position.x;
    const f32 desired_y = dpad->position.y - (dpad->position.w * 0.75f + button_radius_y);
    position.x = MAX(button_radius_x - 0.975f, MIN(desired_x, 0.1f));
    position.y = MAX(button_radius_y - 0.975f, MIN(desired_y, 1.0f - button_radius_y));
    alpha = alpha_to = hovered != 0 ? 1.0f : 0.4f;
    alpha_elapsed = alpha_duration;
    scale = scale_to = hovered != 0 ? 1.2f : 1.0f;
    scale_elapsed = scale_duration;
}

void VirtualControlDPad_LockButton::Render() {
    MechTouchUITexButton::Render();
}

VirtualControlDPad_LockButton::VirtualControlDPad_LockButton(VirtualControlDPad &pad)
    : MechTouchUITexButton(pad.position, 0.1f), dpad(&pad) {
    on_click = VirtualControlDPad_LockButton_OnClick_Callback;
    UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[SuperOptions.dpad_locked != 0 ? 7 : 6]);
}
