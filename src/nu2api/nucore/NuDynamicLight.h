#pragma once

#include "nu2api/nucore/common.h"

struct nuframebuffer_s;
struct nugscn_s;
struct nushaderprogram_s;
struct VuVec;
struct VuMtx;
struct nucamera_s;
struct nuvec_s;
struct numtx_s;

struct NuDynamicLight {
    struct RenderSet {
        RenderSet();
    };
    NuDynamicLight();
    void addShadowCasterScene(nugscn_s *);
    void bindShaderResources(nushaderprogram_s *);
    void clone(variptr_u *, variptr_u);
    void computeBoundingSpace(VuVec const *, VuMtx *);
    void computeClippingPlanes(VuMtx const &, bool, VuVec &, VuVec &, VuVec &, VuVec &, VuVec &, VuVec &);
    void computeFrustumCube(nucamera_s const *, VuVec *, VuVec *);
    void computeLightSpace(nuvec_s *, nuvec_s *, numtx_s *, numtx_s *);
    void computeShadowClippingPlanes(VuVec const &, VuVec const *, VuVec *);
    void computeShadowFrustrumCapsule(VuVec const &, VuVec const *, VuVec &, VuVec &, float &);
    void computeWarpEffect(NuDynamicLight::RenderSet &);
    void create();
    void destroy(NuDynamicLight *);
    void refreshShadowTransform(NuDynamicLight::RenderSet &);
    void renderShadowMap(i32, nuframebuffer_s *);
    void resetGeometry();
    void setCameraViewProj(numtx_s *, numtx_s *);
    void setupCustomCameraFrustum(nucamera_s *, float const *, i32);
    void testShadowExtrusion(VuVec const &, VuVec const &, i32);
    void testShadowExtrusions(VuVec const &, VuVec const &);

    u8 pad_0[0x7b4];
    i32 parameter_4;
    i32 parameter_5;
    u8 pad_7bc[0x1c];
    i32 used_on_specials;
};
DECOMP_ASSERT(offsetof(NuDynamicLight, parameter_4) == 0x7b4, "Dynamic light parameter 4 offset");
DECOMP_ASSERT(offsetof(NuDynamicLight, parameter_5) == 0x7b8, "Dynamic light parameter 5 offset");
DECOMP_ASSERT(offsetof(NuDynamicLight, used_on_specials) == 0x7d8, "Dynamic light special-use flag offset");
