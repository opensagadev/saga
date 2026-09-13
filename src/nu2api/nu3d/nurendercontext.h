#pragma once

#include "decomp.h"
#include "nu2api/numath/numtx.h"

extern i32 g_renderContext_zFunc;

extern "C" {
    extern f32 g_renderContext_viewProj[16];
    extern f32 g_renderContext_view[16];
    extern f32 g_renderContext_world[16];
    extern f32 g_renderContext_kTint[4];

    void NuRenderContextSetViewProj(NUMTX *view, NUMTX *projection);
    void NuRenderContextSetViewport(i32 x, i32 y, i32 width, i32 height);
    void NuRenderContextSetZFunc(i32 zfunc);
}
