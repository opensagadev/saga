// Android framebuffer clear/swap entry points.
#include "decomp.h"
#include "nu2api/nu3d/android/nuposteffect_plain.h"
#include "nu2api/nuandroid/ios_graphics.h"

// original 0x2a2720 — Android forwarder.
extern "C" void NuFramebufferClear(u32 clear_flags, u32 colour) {
    Nu360_dxClear(clear_flags, colour);
}

// original 0x2a2700 — EGL swap is owned by the render thread.
extern "C" void NuFramebufferSwapBuffers(void) {
}
