#pragma once

#include "legoapi/legoapi_types.h"
#include "legoapi/render/light/fade_material.h"

void SetFramesToWait(u32 frames);

inline FADETYPE_VALUE Fade::GetFadeType() const {
    return FADE_TYPE_SCREEN;
}

inline FADETYPE_VALUE FadeWipe::GetFadeType() const {
    return FADE_TYPE_WIPE;
}

inline FADETYPE_VALUE FadeStillWipe::GetFadeType() const {
    return FADE_TYPE_STILL_WIPE;
}

inline FADETYPE_VALUE FadeStill::GetFadeType() const {
    return FADE_TYPE_STILL;
}
