#include "MechInputTouch_types.h"
#include "globals.h"

#include <string.h>

u32 colourPurple = 0xffff00ff;
f32 s_mechTouchTapFrequency = 0.4f;

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

void MechInputTouchMainController::RemoveUnpressedButtons(NuInputTouchData &, NuInputTouchData const &) {
}

void MechInputTouchMainController::Render() {
}

void MechInputTouchMainController::ResetButtons() {
    buttons_repeat = 0;
    buttons_pressed = 0;
    buttons_were_pressed = 0;
    memset(buttons_repeat_timers, 0, sizeof(buttons_repeat_timers));
}

void MechInputTouchMainController::Update(NuInputTouchData const *) {
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

MechInputTouchMainController::~MechInputTouchMainController() {
}
