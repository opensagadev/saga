#include "decomp.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuwater.h"

struct nugobj_s;
struct nugscn_s;
struct numtx_s;
struct nupad_s;
struct rndrstream_s;

extern "C" void NuRndrAddShadowPrims(void) {
}

extern "C" void NuRndrGradClear(i32 clear_flags, i32 top_colour, i32, f32 alpha) {
    NuRndrClear(clear_flags, top_colour, alpha);
}

void NuPs2PadSetMotors(nupad_s *, i32, i32) {
}

extern "C" void NuSetPadDemoEndButtons(u32) {
}

extern "C" void NuPs2VideoSetPos(void) {
}

extern "C" void NuWaterReset(void) {
}

extern "C" void NuWaterInit(void) {
}

extern "C" void NuRndrShadowInit(u8 *) {
}

extern "C" void NuRndrTrailEx(void) {
}

extern "C" void NuLightMatInit(void) {
}

// Original 0x316aa9: exact no-op in the platform support run.
extern "C" void NuRndrInitWorld(void) {
}

extern "C" void NuRndrAddFootPrint(void) {
}

extern "C" void NuLightFogG(void) {
}

i32 NuRndrFlickerBeginScene(void) {
    return 1;
}

void NuRndrFlickerEnd(void) {
}

extern "C" void NuLightSpotFadeSet(u32) {
}

extern "C" i32 NuMtlReadEventSetHandler(void) {
    return 0;
}

void NuPs2GetLanguage(void) {
}

extern "C" void NuRndrShadowOnOff(i32 enabled) {
    (void)enabled;
}

void NuRndrSetXYOffset(i32, i32) {
}

extern "C" void NuRndrLine3dDbgFlush(void) {
}

extern "C" void NuRndrFootPrints(void) {
}

void NuRndrSetScissor(i32, i32, i32, i32) {
}

extern "C" void NuGHGPreRelocateFixupPS(nugscn_s *) {
}

extern "C" void NuGHGPostRelocateFixupPS(nugscn_s *, i32) {
}

extern "C" void NuLightInit(void) {
}

i32 NuRndrGobj(nugobj_s *, numtx_s *) {
    return 0;
}

i32 NuPs2PadDemoEnd(void) {
    return 0;
}

extern "C" void NuRndrDither(void) {
}

extern "C" void NuRndrBurstObjEnd(void) {
}

i32 NuRndrBurstObjBegin(nugobj_s *, void (*)(rndrstream_s *, numtx_s *, i32)) {
    return 0;
}

extern "C" i32 NuRndrBurstObjAdd(void) {
    return 0;
}

extern "C" void NuRndrBurstObjAddNoClip(void) {
}

extern "C" void NuEnableVBlankE(void) {
}

extern "C" void NuDisableVBlankE(void) {
}

extern "C" void NuLightFogPal(void) {
}

extern "C" void NuWaterRender(void) {
}

extern "C" void NuLightAddSpot(void) {
}
