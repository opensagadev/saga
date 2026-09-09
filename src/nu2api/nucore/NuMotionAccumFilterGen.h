#pragma once

#include "nu2api/nucore/NuPostFilterGen.h"
#include "decomp.h"

struct NuMotionAccumFilterGen : NuPostFilterGen {
    f32 GetTiming(i32 *);
    NuMotionAccumFilterGen();
    void destroyResources();
    void destroyTextureResources();
    void initResources();
    void initTextureResources(i32, i32);
    void render();
    nushaderprogram_s *program;
    nuframebuffer_s *accumulation_fbo;
    nueffecttex_s *accumulation_texture;
    i32 frames;
    f32 blend;
    i32 mode, current_frame;
};
DECOMP_ASSERT(sizeof(NuMotionAccumFilterGen) == 0x28, "NuMotionAccumFilterGen size");
