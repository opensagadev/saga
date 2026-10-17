#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct nugscn_s;
extern "C" {
    void NuDisableVBlankE(void);
    void NuEnableVBlankE(void);
    void NuGHGPostRelocateFixupPS(nugscn_s *scene, i32 delta);
    void NuGHGPreRelocateFixupPS(nugscn_s *scene);
    void NuLightSpotFadeSet(u32);
    void NuRndrShadowInit(u8 *);
    void NuWaterInit(void);
}
