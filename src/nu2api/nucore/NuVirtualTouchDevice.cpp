#include "nu2api/nucore/NuVirtualTouchDevice.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/NuTouchInputStick.h"
#include "nu2api/nucore/NuTouchInputButton.h"
#include <string.h>

NuTouchInputButton::NuTouchInputButton(i32 id, u32 index) : NuTouchInputElement(TYPE_BUTTON, id, index) {
    pressed = false;
}

NuTouchInputButton::NuTouchInputButton(i32 id, u32 index, float x, float y, float width, float height)
    : NuTouchInputElement(TYPE_BUTTON, id, index, x, y, width, height) {
    pressed = false;
}

NuTouchInputStick::NuTouchInputStick(NuTouchInputElement::TYPE type, i32 id, u32 index, float x, float y, float width,
                                     float height)
    : NuTouchInputElement(type, id, index, x, y, width, height) {
    unknown_3c = false;
    stick_x = 0.0f;
    stick_y = 0.0f;
}

NuVirtualTouchDevice::NuVirtualTouchDevice(u32 unknown) {
    u32 i;

    this->unknown_08.unknown_c8 = 0;

    for (i = 0; i < 10; i++) {
        this->unknown_d4[i].unknown_c8 = 0;
    }

    this->unknown_04 = 0;
}

void NuVirtualTouchDevice::Execute(u32 port, NUPADTYPE in_type, NUPADATTACHMENTTYPE in_attch_type, u32 in_caps,
                                   u32 in_buttons, const float *in_analog, const float *in_motion,
                                   const NuInputTouchData *in_touch_data, const NuInputMouseData *in_mouse_data,
                                   NUPADTYPE &out_pad_type, NUPADATTACHMENTTYPE &out_attch_type, u32 &out_caps,
                                   u32 &out_buttons, float *out_analog, float *out_motion,
                                   NuInputTouchData *out_touch_data, NuInputMouseData *out_mouse_data) {
    unknown_08.UpdateButtons(in_buttons);
    unknown_08.Update(in_touch_data);
    if (unknown_04 < 10)
        unknown_d4[unknown_04].Update(in_touch_data);

    out_pad_type = NUPADTYPE_GAMEPAD;
    out_attch_type = NUPADATTACHMENTTYPE_NONE;
    out_caps = 0x418;
    out_buttons = 0;
    memset(out_analog, 0, 12 * sizeof(float));
    for (u32 i = 0; i < unknown_d4[unknown_04].unknown_c8; ++i) {
        if (unknown_d4[unknown_04].elements[i]->type == NuTouchInputElement::TYPE_LEFT_STICK) {
            out_analog[8] = unknown_d4[unknown_04].elements[i]->GetStickX();
            out_analog[9] = unknown_d4[unknown_04].elements[i]->GetStickY();
        } else if (unknown_d4[unknown_04].elements[i]->type == NuTouchInputElement::TYPE_RIGHT_STICK) {
            out_analog[10] = unknown_d4[unknown_04].elements[i]->GetStickX();
            out_analog[11] = unknown_d4[unknown_04].elements[i]->GetStickY();
        } else {
            static i32 test;
            if (unknown_d4[unknown_04].elements[i]->IsPressed()) {
                ++test;
                out_buttons |= unknown_d4[unknown_04].elements[i]->index;
            } else {
                test = 0;
            }
        }
    }
    memcpy(out_touch_data, in_touch_data, sizeof(NuInputTouchData));
}

void NuVirtualTouchDevice::CreateDefaultLayout(u32 unknown) {
}

f32 NuVirtualTouchDevice::GetAspectRatio() {
    return (f32)g_backingHeight / (f32)g_backingWidth;
}

void NuVirtualTouchDevice::Render() {
    if (unknown_04 < 10)
        unknown_d4[unknown_04].Render();
}

void NuVirtualTouchDevice::SetCurrentLayoutIndex(u32 index) {
    if (unknown_04 < 10)
        unknown_d4[unknown_04].DeactivateLayout();
    unknown_04 = index;
    if (unknown_04 < 10)
        unknown_d4[unknown_04].ActivateLayout();
}
