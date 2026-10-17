#include "decomp.h"
#include <float.h>
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nuapi.h"

#include <new>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "nu2api/nucore/nustring.h"

#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuCopyFilter.h"
#include "nu2api/nucore/NuDataPortManager.h"
#include "nu2api/nucore/NuDeferredFilter.h"
#include "nu2api/nucore/NuDeferredFilterGen.h"
#include "nu2api/nucore/NuDeviceSpecs.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/numtl.h"
extern "C" void DisplaySceneRndrSpecials(NUDLDLISTSCENE *, i32, void *);
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/NuMainFilter.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nucore/NuMotionAccumFilter.h"
#include "nu2api/nucore/NuMotionAccumFilterGen.h"
#include "nu2api/nucore/NuMotionFilter.h"
#include "nu2api/nucore/NuMotionFilterGen.h"
#include "nu2api/nucore/NuNetEmu.h"
#include "nu2api/nucore/NuPlatform.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nucore/NuSpeedBlurFilter.h"
#include "nu2api/nucore/NuSpeedBlurFilterGen.h"
#include "nu2api/nucore/NuVoiceAndroid.h"
#include "gamelib/util/Utilities.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nupostdraw.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nuvuvectype.hpp"

extern const f32 nuvec4_one[4];
extern "C" void NuShaderManagerSetfv(i32, const f32 *);


NuApplicationState *NuCore::m_applicationState;
NuThreadManager *NuCore::m_threadManager;

void NuCore::Initialize() {
    GetApplicationState();

    m_threadManager = new NuThreadManager();
}

NuApplicationState *NuCore::GetApplicationState(void) {
    if (m_applicationState != NULL) {
        return m_applicationState;
    }

    NuApplicationState *state = NU_ALLOC_T(NuApplicationState, 1, "", NUMEMORY_CATEGORY_NONE);
    if (state != NULL) {
        new (state) NuApplicationState();
    }

    m_applicationState = state;

    return state;
}













extern NUMTX cacheCameraView;
extern NUMTX cacheCameraProj;
