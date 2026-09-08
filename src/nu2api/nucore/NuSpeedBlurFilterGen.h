#pragma once

#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nu3d/nupostparams.h"

struct NuSpeedBlurFilterGen : NuPostFilterGen {
    NuSpeedBlurFilterGen();
    void computeSpeedBlur(VuVec &);
    void destroyTextureResources();
    void initTextureResources(i32, i32);
    void render();
    nushaderprogram_s *programs[3];
    nueffecttex_s *texture;
    const NuSpeedBlurParameters *parameters;
};
DECOMP_ASSERT(sizeof(NuSpeedBlurFilterGen) == 0x20, "NuSpeedBlurFilterGen size");
