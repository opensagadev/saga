#include "decomp.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nuwater.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"

static void NuPrimReset() {
}

void NuFrameBegin(void) {
    nuapi.nuframe_begin_cnt++;

    NuPortalInit();
    NuPrimReset();
    NuWaterReset();
    NuMouseRead();
    NuKeyboardRead();
    NuOcclusionManagerBeginFrame();
}
