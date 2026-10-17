// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "nu2api/nucore/NuInputDevice.h"

#include <string.h>
#include <stddef.h>
#include "nu2api/nu3d/nurndr.h"
#include "MechInputTouch/MechInputTouchMainController.h"

u32 colourPurple = 0xffff00ff;
f32 s_mechTouchTapFrequency = 0.4f;

MechInputTouchMainController::~MechInputTouchMainController() {
}

void MechInputTouchMainController::Render() {
}

u8 MechInputTouchMainDummyButton::IsPressed() const {
    return controller->button_repeats[button_type];
}

void MechInputTouchMainController::ResetButtons() {
    buttons_repeat = 0;
    buttons_pressed = 0;
    buttons_were_pressed = 0;
    memset(buttons_repeat_timers, 0, sizeof(buttons_repeat_timers));
}

MechInputTouchMainController::MechInputTouchMainController(i32 index)
    : NuTouchInputElement(NuTouchInputElement::TYPE_LEFT_STICK, colourPurple, 0), player_id(index) {
    field_68 = 0;
    stick_values[0] = 0.0f;
    stick_values[1] = 0.0f;
    stick_values[2] = 0.0f;
    stick_values[3] = 0.0f;
    field_5c = 0.0f;
    field_60 = 0.0f;
    ResetButtons();
}

void MechInputTouchMainController::UpdateButtons() {
    const f32 frame_time = FRAMETIME;
    const f32 tap_frequency = s_mechTouchTapFrequency;

    button_repeats[0] = button_was_pressed[0];
    if (button_was_pressed[0] == 0 && button_pressed[0] != 0) {
        if ((buttons_repeat_timers[0] -= frame_time) <= 0.0f) {
            buttons_repeat_timers[0] = tap_frequency;
            button_repeats[0] = 1;
        }
    } else {
        buttons_repeat_timers[0] = 0.0f;
    }

    button_repeats[1] = button_was_pressed[1];
    if (button_was_pressed[1] == 0 && button_pressed[1] != 0) {
        if ((buttons_repeat_timers[1] -= frame_time) <= 0.0f) {
            buttons_repeat_timers[1] = tap_frequency;
            button_repeats[1] = 1;
        }
    } else {
        buttons_repeat_timers[1] = 0.0f;
    }

    button_repeats[2] = button_was_pressed[2];
    if (button_was_pressed[2] == 0 && button_pressed[2] != 0) {
        if ((buttons_repeat_timers[2] -= frame_time) <= 0.0f) {
            buttons_repeat_timers[2] = tap_frequency;
            button_repeats[2] = 1;
        }
    } else {
        buttons_repeat_timers[2] = 0.0f;
    }

    button_repeats[3] = button_was_pressed[3];
    if (button_was_pressed[3] == 0 && button_pressed[3] != 0) {
        if ((buttons_repeat_timers[3] -= frame_time) <= 0.0f) {
            buttons_repeat_timers[3] = tap_frequency;
            button_repeats[3] = 1;
        }
    } else {
        buttons_repeat_timers[3] = 0.0f;
    }

    button_pressed[0] = 0;
    button_was_pressed[0] = 0;
    button_pressed[1] = 0;
    button_was_pressed[1] = 0;
    button_pressed[2] = 0;
    button_was_pressed[2] = 0;
    button_pressed[3] = 0;
    button_was_pressed[3] = 0;
}

void MechInputTouchMainController::RemoveUnpressedButtons(NuInputTouchData &output, NuInputTouchData const &input) {
    output.touch_count = 0;
    for (i32 i = 0; i < static_cast<i32>(input.touch_count); ++i) {
        output.touch_events[i] = input.touch_events[i];
        output.touch_count = i + 1;
    }
}

void MechInputTouchMainController::Update(NuInputTouchData const *input) {
    NuInputTouchData filtered;
    RemoveUnpressedButtons(filtered, *input);
    stick_values[0] = 0.0f;
    stick_values[1] = 0.0f;
    stick_values[2] = 0.0f;
    stick_values[3] = 0.0f;
    UpdateButtons();
}

MechInputTouchMainDummyButton::MechInputTouchMainDummyButton(MechInputTouchMainController &main_controller, u32 id,
                                                             MechInputTouchMainController::eButtonTypes type)
    : NuTouchInputElement(TYPE_BUTTON, colourPurple, id, 0.0f, 0.0f, 0.0f, 0.0f), controller(&main_controller),
      button_type(static_cast<u32>(type)) {
}

MechInputTouchMainDummyStick::MechInputTouchMainDummyStick(MechInputTouchMainController &main_controller,
                                                           NuTouchInputElement::TYPE type)
    : NuTouchInputElement(type, colourPurple, 0), controller(&main_controller) {
}

