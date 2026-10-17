#pragma once

#include "decomp.h"

// Types shared by legoapi/menus/core/gamehint.cpp and legogame/game.cpp.

struct MechHintUIButton : MechTouchUITexButton {
    HINT_s *active_hint;
    HINT_s *pending_hint;
    f32 display_elapsed;
    f32 *slide_target;
    f32 slide_from, slide_to, slide_elapsed, slide_duration, slide_delay, slide;
    u8 pulse_active;
    f32 pulse_remaining;

    MechHintUIButton() : MechTouchUITexButton(VuVec(ICONXPOS_TOUCHYFEELY, ICONYPOS_TOUCHYFEELY, 0.0f, 1.0f), 0.075f) {
        slide_elapsed = 0.0f;
        slide_duration = -1.0f;
        slide_delay = 0.0f;
        active_hint = NULL;
        pending_hint = NULL;
        slide_target = &slide;
        on_click = MechHintUIButton_OnClick_Callback;
    }
    ~MechHintUIButton() override {
    }
    void __attribute__((weak)) Process(float) override;
    void __attribute__((weak)) Render() override;
    void StartPulse() {
        static f32 PulseAgainTime = 2.0f;
        pulse_active = 1;
        pulse_remaining = PulseAgainTime;
        if (visible)
            MechSystems::Get()->NewRadarPulse(position, false);
    }
};

DECOMP_ASSERT(offsetof(MechHintUIButton, display_elapsed) == 0x80, "Touch hint display timer offset");
DECOMP_ASSERT(offsetof(MechHintUIButton, pending_hint) == 0x7c, "Pending touch hint offset");
DECOMP_ASSERT(sizeof(MechHintUIButton) == 0xa8, "MechHintUIButton size");
