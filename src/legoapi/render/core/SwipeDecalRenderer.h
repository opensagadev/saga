#pragma once

#include "decomp.h"
#include "nu2api/nucore/fixed_width.h"

struct TouchHolder;

struct SwipeDecalRenderer {
    enum Style : i32 {};
    SwipeDecalRenderer(TouchHolder &, i32, SwipeDecalRenderer::Style);
    void Process(float);
    void Render();

    struct Animation {
        f32 *target;
        f32 from;
        f32 to;
        f32 elapsed;
        f32 duration;
        f32 delay;
        f32 value;

        void Initialize() {
            target = &value;
            elapsed = 0.0f;
            duration = -1.0f;
            delay = 0.0f;
        }
        void Start(f32 start, f32 end, f32 time) {
            from = start;
            to = end;
            elapsed = 0.0f;
            duration = time;
            delay = 0.0f;
        }
        bool IsActive() const {
            return !(duration < 0.0f) && !(elapsed >= duration + delay);
        }
        void Process(f32 frame_time) {
            if (IsActive()) {
                elapsed += frame_time;
                if (elapsed > duration + delay) {
                    elapsed = duration + delay;
                }
                if (elapsed >= delay) {
                    *target = ((elapsed - delay) / duration) * (to - from) + from;
                }
            }
        }
    };

    f32 x;
    f32 y;
    i32 angle;
    Animation alpha;
    Animation width;
    Style style;
};
DECOMP_ASSERT(sizeof(SwipeDecalRenderer) == 0x48, "Swipe decal renderer ABI");
DECOMP_ASSERT(offsetof(SwipeDecalRenderer, alpha) == 0xc, "Swipe decal alpha offset");
DECOMP_ASSERT(offsetof(SwipeDecalRenderer, width) == 0x28, "Swipe decal width offset");
DECOMP_ASSERT(offsetof(SwipeDecalRenderer, style) == 0x44, "Swipe decal style offset");
