#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nucore/NuMainFilter.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/android/nupostshaders.h"

void NuMainFilter::initResources() {
    NuMainFilterGen::initResources();
    programs[15] = NuShaderProgramCreateIOS(blur7x7_vx, blur7x7dof_px);
}
