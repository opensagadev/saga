#include "nu2api/nu3d/nuocclusion.h"

#include "decomp.h"
#include "legoapi/legoapi_types.h"

void NuOcclusionManagerBeginFrame(void) {
    g_OcclusionManager.BeginFrame();
}
