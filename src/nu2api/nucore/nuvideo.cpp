#include "decomp.h"
#include "nu2api/nucore/nuvideo.h"

#include "nu2api/nucore/nuapi.h"

extern "C" void NuSetupVideoModeParams(void) {
    STUBBED();
}

i32 NuVideoGetMode(void);

void NuVideoSetMode(i32 mode);

i32 NuVideoGetAspect(void);

NUVIDEO_SWAPMODE NuVideoGetSwapMode(void);

void NuVideoRollingFrameRateReset(void);

void NuVideoSetBrightness(f32 brightness);

void NuVideoSetResolution(i32 width, i32 height);

NUVIDEO_SWAPMODE NuVideoSetSwapMode(NUVIDEO_SWAPMODE video_swap_mode);
