#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
extern float nuapi_forced_frame_time;
extern i32 file_criticalsection;
extern i32 nuapi_max_fps;
extern i32 nufile_buffering_enabled;
isize BinarySearch(u32 element, u32 *array, usize length);
void NuTerminateHardware();
extern "C" {
    char *NuGetErrN(i32);
    extern void * NuMem_Heap;
    i32 NuHasError(void);
    void NuClearError();
    void NuErrorCheck(void);
    void NuErrorSleep();
    void NuSevereWarning(const char *, ...);
    void NuRegisterEndFrameCallBackFn(void (*callback)(void));
}
