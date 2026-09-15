#include "decomp.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuwater.h"

struct nugobj_s;
struct numtx_s;
struct nupad_s;
struct rndrstream_s;

extern "C" void NuRndrAddShadowPrims(void) {
    STUBBED();
}

extern "C" void NuRndrGradClear(i32 clear_flags, i32 top_colour, i32, f32 alpha) {
    NuRndrClear(clear_flags, top_colour, alpha);
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

extern "C" void NuWaterReset(void) {
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

// Original 0x316aa9: exact no-op in the platform support run.
extern "C" void NuRndrInitWorld(void) {
}

extern "C" void NuRndrAddFootPrint(void) {
    STUBBED();
}

extern "C" void NuLightFogG(void) {
    STUBBED();
}

i32 NuRndrFlickerBeginScene(void) {
    return 1;
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

extern "C" void NuRndrShadowOnOff(i32 enabled) {
    STUBBED();
    (void)enabled;
}

void NuRndrSetXYOffset(i32, i32) {
    STUBBED();
}

extern "C" void NuRndrLine3dDbgFlush(void) {
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
