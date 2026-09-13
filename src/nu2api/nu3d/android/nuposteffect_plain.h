// Android framebuffer clear/swap entry points.
#pragma once

#include "decomp.h"

extern "C" void NuFramebufferClear(u32 clear_flags, u32 colour);
extern "C" void NuFramebufferSwapBuffers(void);
