#include "nu2api/nu3d/nupostresources.h"

// The Android binary supplies empty platform entry points for these resources.
extern "C" nueffecttex_s *NuEffectTexCreate2D(i32, i32, i32, i32, i32) {
}

extern "C" nuframebuffer_s *NuFramebufferCreate() {
}
