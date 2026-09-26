#pragma once

#include "nu2api/nucore/NuPostFilterGen.h"

struct nuframebuffer_s;
struct nueffecttex_s;

struct NuCopyFilter : NuPostFilterGen {
    nuframebuffer_s *copy_fbo;
    nueffecttex_s *copy_texture;

    void destroyResources();
    void initResources();
    virtual void render(nuframebuffer_s *);
    void reset();
};
