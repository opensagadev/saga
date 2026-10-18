#pragma once

#include "nu2api/nucore/common.h"

#ifdef __cplusplus
void *renderThread_main(void *arg);
i64 getCurrentTime();

extern "C" {
#endif
    void NuRenderThreadLock(void);
    void NuRenderThreadUnlock(void);
    void NuRenderThreadPrepareRender(void);
    void NuRenderThreadStartRender(void);
    i32 NuRenderThreadIsLocked(void);
    i32 NuRenderThreadIsCurrentThread(void);
    void NuRenderThreadCreate(void);
    void NuRenderThreadDestroy(void);

    i32 renderThread_processRenderScenes(void);
#ifdef __cplusplus
}
#endif

// Declared here for the other units that use them.
extern f32 nuvec4_one[4];
extern i32 global_frame_count;
