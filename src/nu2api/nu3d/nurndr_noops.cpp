#include "decomp.h"
#include "legoapi/legoapi_types.h"

extern "C" i32 NuRndrSetBlendData(void) {
    STUBBED();
    return 0;
}

i32 NuRndrFlickerBeginScene(void) {
    return 1;
}

extern "C" void NuRndrLine3dDbgFlush(void) {
    STUBBED();
}

extern "C" void NuRndrShadowOnOff(i32 enabled) {
    STUBBED();
    (void)enabled;
}

extern "C" void NuRndrScreenGrabTileInit(void *, i32, f32, f32, f32) {
    STUBBED();
}

extern "C" void NuRndrScreenGrabTileDeInit(void *) {
    STUBBED();
}

extern "C" void NuRndrScreenGrabTileBegin(void **) {
    STUBBED();
}

extern "C" void NuRndrScreenGrabTileEnd(void **) {
    STUBBED();
}
