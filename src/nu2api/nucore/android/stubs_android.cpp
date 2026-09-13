#include "decomp.h"
#include "nu2api/nucore/common.h"

struct nugobj_s;
struct numtx_s;
struct nupad_s;
struct rndrstream_s;

extern "C" void NuRndrAddShadowPrims(void) {
    STUBBED();
}

void NuPs2PadSetMotors(nupad_s *, i32, i32) {
    STUBBED();
}

extern "C" void NuSetPadDemoEndButtons(u32) {
    STUBBED();
}

extern "C" void NuPs2VideoSetPos(void) {
    STUBBED();
}

extern "C" void NuWaterInit(void) {
    STUBBED();
}

extern "C" void NuRndrShadowInit(u8 *) {
    STUBBED();
}

extern "C" void NuRndrTrailEx(void) {
    STUBBED();
}

extern "C" void NuLightMatInit(void) {
    STUBBED();
}

extern "C" void NuRndrAddFootPrint(void) {
    STUBBED();
}

extern "C" void NuLightFogG(void) {
    STUBBED();
}

void NuRndrFlickerEnd(void) {
    STUBBED();
}

extern "C" void NuLightSpotFadeSet(u32) {
    STUBBED();
}

extern "C" i32 NuMtlReadEventSetHandler(void) {
    STUBBED();
    return 0;
}

void NuPs2GetLanguage(void) {
    STUBBED();
}

void NuRndrSetXYOffset(i32, i32) {
    STUBBED();
}

extern "C" void NuRndrFootPrints(void) {
    STUBBED();
}

void NuRndrSetScissor(i32, i32, i32, i32) {
    STUBBED();
}

extern "C" void NuGHGPreRelocateFixupPS(void) {
    STUBBED();
}

extern "C" void NuGHGPostRelocateFixupPS(void) {
    STUBBED();
}

extern "C" void NuLightInit(void) {
    STUBBED();
}

i32 NuRndrGobj(nugobj_s *, numtx_s *) {
    STUBBED();
    return 0;
}

i32 NuPs2PadDemoEnd(void) {
    STUBBED();
    return 0;
}

extern "C" void NuRndrDither(void) {
    STUBBED();
}

extern "C" void NuRndrBurstObjEnd(void) {
    STUBBED();
}

i32 NuRndrBurstObjBegin(nugobj_s *, void (*)(rndrstream_s *, numtx_s *, i32)) {
    STUBBED();
    return 0;
}

extern "C" i32 NuRndrBurstObjAdd(void) {
    STUBBED();
    return 0;
}

extern "C" void NuRndrBurstObjAddNoClip(void) {
    STUBBED();
}

extern "C" void NuEnableVBlankE(void) {
    STUBBED();
}

extern "C" void NuDisableVBlankE(void) {
    STUBBED();
}

extern "C" void NuLightFogPal(void) {
    STUBBED();
}

extern "C" void NuWaterRender(void) {
    STUBBED();
}

extern "C" void NuLightAddSpot(void) {
    STUBBED();
}
