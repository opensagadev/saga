// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "nu2api/nu3d/nurndr.h"

#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nucore/nuapi.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "globals.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"

struct nugobj_s;
struct nugscn_s;
struct numtx_s;
struct nupad_s;
struct rndrstream_s;

#ifdef __ANDROID__
extern "C" __attribute__((weak)) int toupper(int c);
#endif

extern "C" {
extern "C" void NuRndrAddShadowPrims(void) {
    STUBBED();
}

extern "C" void NuRndrGradClear(i32 clear_flags, i32 top_colour, i32, f32 alpha) {
    NuRndrClear(clear_flags, top_colour, alpha);
}

}

void NuPs2PadSetMotors(nupad_s *, i32, i32) {
    STUBBED();
}

extern "C" {
extern "C" void NuSetPadDemoEndButtons(u32) {
}

extern "C" void NuPs2VideoSetPos(void) {
    STUBBED();
}

extern "C" void NuWaterReset(void) {
}

extern "C" void NuWaterInit(void) {
    STUBBED();
}

extern "C" void NuRndrShadowInit(u8 *) {
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

}

i32 NuRndrFlickerBeginScene(void) {
    return 1;
}

void NuRndrFlickerEnd(void) {
    STUBBED();
}

extern "C" {
extern "C" void NuLightSpotFadeSet(u32) {
}

extern "C" i32 NuMtlReadEventSetHandler(void) {
    STUBBED();
    return 0;
}

}

void NuPs2GetLanguage(void) {
    STUBBED();
}

extern "C" {
extern "C" void NuRndrShadowOnOff(i32 enabled) {
    (void)enabled;
}

}

void NuRndrSetXYOffset(i32, i32) {
    STUBBED();
}

extern "C" {
extern "C" void NuRndrLine3dDbgFlush(void) {
}

extern "C" void NuRndrFootPrints(void) {
    STUBBED();
}

}

void NuRndrSetScissor(i32, i32, i32, i32) {
    STUBBED();
}

extern "C" {
extern "C" void NuGHGPreRelocateFixupPS(nugscn_s *) {
}

extern "C" void NuGHGPostRelocateFixupPS(nugscn_s *, i32) {
}

extern "C" void NuLightInit(void) {
    STUBBED();
}

extern "C" void NuSetupVideoModeParams(void) {
    STUBBED();
}

    f32 FmvTimePS(void) {
        return 0.0f;
    }

}

void NuTexUnReserve() {
}

i32 NuRndrGobj(nugobj_s *, numtx_s *) {
    STUBBED();
    return 0;
}

i32 NuPs2PadDemoEnd(void) {
    STUBBED();
    return 0;
}

extern "C" {
extern "C" void NuRndrDither(void) {
    STUBBED();
}

extern "C" void NuRndrBurstObjEnd(void) {
    STUBBED();
}

}

i32 NuRndrBurstObjBegin(nugobj_s *, void (*)(rndrstream_s *, numtx_s *, i32)) {
    STUBBED();
    return 0;
}

extern "C" {
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

}
