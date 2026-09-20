#pragma once

#include "nu2api/nucore/common.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
    void NuOcclusionManagerBeginFrame(void);
    void NuOcclusionManagerEndFrame(void);
    bool NuOcclusionManagerIsEnabled(void);
    bool NuOcclusionManagerIsInitialised(void);
    void NuOcclusionManagerOnCameraSet(void);
    void NuOcclusionManagerRenderZPass(void);
    void NuOcclusionManagerSetEnabled(i32 enabled);
    void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
    void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
#ifdef __cplusplus
}
#endif
