#ifndef LEGOAPI_MENUS_CORE_GAMEHINT_H
#define LEGOAPI_MENUS_CORE_GAMEHINT_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nuvuvec.hpp"

// Hint system (module legoapi/menus/core, gamehint.cpp).

extern HINTSYS_s hintsys;

struct HintScalarTransition {
    f32 *target;
    f32 from, to, elapsed, duration, delay, value;
    HintScalarTransition() : target(&value), elapsed(0.0f), duration(-1.0f), delay(0.0f) {
    }
    void Update(f32 dt) {
        if (!(duration < 0.0f) && !(elapsed >= duration + delay)) {
            elapsed += dt;
            if (elapsed > duration + delay)
                elapsed = duration + delay;
            if (elapsed >= delay)
                *target = ((elapsed - delay) / duration) * (to - from) + from;
        }
    }
};

struct HintVectorTransition {
    VuVec *target;
    VuVec from, to;
    f32 elapsed, duration, delay;
    VuVec value;
    HintVectorTransition() : target(&value), elapsed(0.0f), duration(-1.0f), delay(0.0f) {
    }
    void Update(f32 dt) {
        if (!(duration < 0.0f) && !(elapsed >= duration + delay)) {
            elapsed += dt;
            if (elapsed > duration + delay)
                elapsed = duration + delay;
            if (elapsed >= delay) {
                const f32 amount = (elapsed - delay) / duration;
                target->w = 0.0f;
                target->y = amount * (to.y - from.y) + from.y;
                target->z = amount * (to.z - from.z) + from.z;
                target->x = amount * (to.x - from.x) + from.x;
            }
        }
    }
};

extern HintVectorTransition hintIconPos;
extern HintScalarTransition hintIconScale;
extern HintScalarTransition hintYPop;
f32 CurrentHintAlpha();
f32 CurrentHintButtonScale();
void Hint_Draw(i32 viewport);

HINT_s *Hint_FindHint(i32 hint_id);
i32 Hint_CurrentId();
i32 Hint_isComplete(HINT_s *hint);
i32 Hint_isComplete(i32 hint_id);
void Hint_SaveBits(i32 hint_id, i32 complete);
void Hint_SetComplete(HINT_s *hint);
void Hint_SetComplete(i32 hint_id);
i32 Hint_isAvailable(i32 hint_id);

#endif
