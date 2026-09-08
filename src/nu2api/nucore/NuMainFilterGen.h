#pragma once

#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nu3d/nupostparams.h"

struct nueffecttex_s;

struct NuMainFilterGen : NuPostFilterGen {
    NuMainFilterGen();
    void destroyResources();
    void destroyTextureResources();
    void initResources();
    void initTextureResources(i32, i32);
    void preprocessBlurTextures(nueffecttex_s *, nueffecttex_s *);
    void preprocessDofMotionBlur(nueffecttex_s *);
    void render();
    void reset();

    nushaderprogram_s *programs[18];
    nueffecttex_s *blur_texture;
    nuframebuffer_s *blur_fbo;
    nueffecttex_s *downsample_texture;
    f32 blur_gain;
    i32 downsample_lod;
    const NuBloomParameters *bloom;
    f32 blur_radius;
    f32 dof_strength, dof_near, dof_far, dof_blur;
    i32 dof_mode;
    f32 dof_bias;
    bool dof_enabled;
    bool bloom_enabled;
    bool motion_blur_enabled;
    u8 unknown_08b;
    i32 active_filter_count;
    NUMTX motion_previous, motion_current;
    f32 motion_scale, motion_maximum, motion_falloff;
    i32 accumulation_frames;
    f32 accumulation_blend;
    i32 accumulation_mode;
    virtual bool isEnabled() {
        return active_filter_count != 0;
    }
};
DECOMP_ASSERT(sizeof(NuMainFilterGen) == 0x128, "NuMainFilterGen size");
