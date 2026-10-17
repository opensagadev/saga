#include "legoapi/core/startup/main.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/android/NuThread_android.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nunativegscene_s;

extern "C" {
    extern i32 FinishLoop_On;
    i32 NuRndrBeginScene(i32 flags);
    void NuRndrEndScene(void);
    f32 NuFrameEnd(void);
    void edGraEnableTerrainSwap(void);
    void edGraDisableTerrainSwap(void);
}

void BackDrop_Draw(f32 alpha, i32 flags);

void FinishLoop(i32 frames);

void bgSuspendMain(i32) {
}

