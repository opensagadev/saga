#include "nu2api/nu3d/nupostresources.h"

// These resources are unsupported by the Android backend reused on the host.
// Define the result explicitly rather than falling off a non-void C++ function.
extern "C" nueffecttex_s *NuEffectTexCreate2D(i32, i32, i32, i32, i32) {
    return NULL;
}

extern "C" nuframebuffer_s *NuFramebufferCreate() {
    return NULL;
}
