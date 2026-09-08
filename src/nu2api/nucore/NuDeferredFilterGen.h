#pragma once

#include "decomp_assert.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/NuPostFilterGen.h"

struct NuDynamicLight;

struct NuDeferredFilterGenLayout {
    u8 reserved_00[0x54];
    i32 dynamic_light_count;
    NuDynamicLight *dynamic_lights[32];
    i32 deferred_geometry_count;
    u8 reserved_dc[0x2f8 - 0xdc];
};
DECOMP_ASSERT(sizeof(NuDeferredFilterGenLayout) == 0x2f8, "NuDeferredFilterGenLayout size");

struct NuDeferredFilterGen : NuPostFilterGen {
    i32 sample_count;
    nushaderprogram_s *programs[6];
    nueffecttex_s *textures[6];
    u32 unknown_40;
    f32 parameters[4];
    i32 dynamic_light_count;
    NuDynamicLight *dynamic_lights[32];
    i32 deferred_geometry_count;
    void *geometry[128];
    nuframebuffer_s *shadow_fbos[6];
    nuframebuffer_s *light_fbo;

    NuDeferredFilterGen();
    void destroyResources();
    void destroyTextureResources();
    void initResources();
    void initTextureResources(i32, i32);
    void render();
    void renderStencilMask(NuDynamicLight &);
    void resetAll();
    virtual bool isEnabled() {
        return enabled && (dynamic_light_count > 0 || deferred_geometry_count > 0);
    }
};
DECOMP_ASSERT(sizeof(NuDeferredFilterGen) == 0x2f8, "NuDeferredFilterGen size");
