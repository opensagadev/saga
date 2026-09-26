#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/android/nudlist_callbacks.h"

i32 NuInitHardwareFirst(i32, variptr_u *, i32 *, i32) {
    return 0;
}

void NuDisplayListSetInstSurfGeom(void *) {
    STUBBED();
}

void NuTerminateHardware() {
    STUBBED();
}
