#pragma once

#include "decomp.h"
#include "nu2api/numath/numtx.h"

struct numtl_s;
struct d3dsamplerstate_u;

extern i32 g_renderContext_zFunc;
extern numtl_s *g_renderContext_materialInUse;
void NuRenderContextForceSamplerStatePS(i32, const d3dsamplerstate_u *);

extern "C" {
    extern f32 g_renderContext_viewProj[16];
    extern f32 g_renderContext_viewProjInverse[16];
    extern f32 g_renderContext_view[16];
    extern f32 g_renderContext_projection[16];
    extern f32 g_renderContect_worldView[16];
    extern f32 g_renderContext_position[4];
    extern f32 g_renderContext_kTint[4];
    extern f32 g_renderContext_gpuTime;
    extern f32 g_renderContext_postEffectTime;
    extern f32 g_renderContext_3dTime;

    void NuRenderContextInit(void);
    void NuRenderContextSetViewProj(NUMTX *view, NUMTX *projection);
    void NuRenderContextSetViewport(i32 x, i32 y, i32 width, i32 height);
    void NuRenderContextSetZFunc(i32 zfunc);
    void NuRenderContextSetAlphaBlend(void);
    void NuRenderContext360BeginGameTime(void);
    void NuRenderContext360EndGameTime(void);
}
