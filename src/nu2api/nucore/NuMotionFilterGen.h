#pragma once

#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nu3d/nupostparams.h"

struct NuMotionFilterGen : NuPostFilterGen {
    NuMotionFilterGen();
    void render();
    nushaderprogram_s *programs[2];
    NUMTX previous, current;
    f32 scale, maximum, falloff;
};
DECOMP_ASSERT(sizeof(NuMotionFilterGen) == 0xa0, "NuMotionFilterGen size");
