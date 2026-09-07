#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
    void NuOcclusionManagerBeginFrame(void);
    bool NuOcclusionManagerIsInitialised(void);
    void NuOcclusionManagerOnCameraSet(void);
#ifdef __cplusplus
}
#endif
