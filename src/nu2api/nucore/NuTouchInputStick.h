#pragma once

#include "nu2api/nucore/NuTouchInputElement.h"

struct NuInputTouchData;

struct NuTouchInputStick : NuTouchInputElement {
    NuTouchInputStick(NuTouchInputElement::TYPE, i32, u32, float, float, float, float);
    void Render() override;
    void Update(NuInputTouchData const *) override;
    float GetStickX() const override {
        return stick_x;
    }
    float GetStickY() const override {
        return stick_y;
    }

    float stick_x;
    float stick_y;
    u32 unknown_38;
    bool unknown_3c;
};
