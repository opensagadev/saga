#include "decomp.h"
#include <float.h>
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nuapi.h"

#include <new>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "nu2api/nucore/NuDataPortManager.h"
#include "nu2api/nucore/NuDeferredFilterGen.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nucore/NuMotionAccumFilterGen.h"
#include "nu2api/nucore/NuMotionFilterGen.h"
#include "nu2api/nucore/NuNetEmu.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nucore/NuSpeedBlurFilterGen.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"

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
