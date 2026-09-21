// Original nurendercontext.cpp: render-context state and the contiguous
// [0x2a30d0, 0x2a3b0c) function run.
#include <GLES2/gl2.h>
#include <string.h>

#include "decomp.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/android/nublend_internal.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/numath/nuvec.h"

extern "C" {
    // GCC emits these zero-initialized definitions in reverse source order.
    // This is the original context matrix/state order, omitting absent state.
    f32 g_renderContext_view[16];
    f32 g_renderContext_projection[16];
    f32 g_renderContect_worldView[16];
    f32 g_renderContext_viewProj[16];
    f32 g_renderContext_viewProjInverse[16];
    f32 g_renderContext_position[4];
    f32 g_renderContext_kTint[4];
}
numtl_s *g_renderContext_materialInUse = nullptr;
extern "C" {
    f32 g_renderContext_gpuTime;
    f32 g_renderContext_3dTime;
    f32 g_renderContext_postEffectTime;
}
i32 g_renderContext_zFunc = 3;

extern "C" {

    void NuRenderContextInit(void) {
        memcpy(g_renderContext_viewProj, &numtx_identity, sizeof(numtx_identity));
        memcpy(g_renderContext_view, &numtx_identity, sizeof(numtx_identity));
        memcpy(g_renderContext_projection, &numtx_identity, sizeof(numtx_identity));
        memcpy(g_renderContect_worldView, &numtx_identity, sizeof(numtx_identity));
    }

    // Original 0x2a33d0, 9 bytes: this platform deliberately does nothing.
    void NuRenderContextSetViewport(i32, i32, i32, i32) {
    }

    SAGA_HOST_WEAK void NuRenderContextSetViewProj(NUMTX *view, NUMTX *projection) {
        NUVEC scale = {
            g_NuVpRegion.projection_x_scale,
            g_NuVpRegion.projection_y_scale,
            1.0f,
        };
        NUVEC translation = {
            g_NuVpRegion.projection_x_offset,
            g_NuVpRegion.projection_y_offset,
            0.0f,
        };
        NUMTX scale_mtx;
        NUMTX translation_mtx;
        NUMTX adjusted_projection;
        NuMtxSetScale(&scale_mtx, &scale);
        NuMtxSetTranslation(&translation_mtx, &translation);
        NuMtxMulH(&adjusted_projection, projection, &scale_mtx);
        NuMtxMulH(&adjusted_projection, &adjusted_projection, &translation_mtx);

        memcpy(g_renderContext_view, view, sizeof(NUMTX));
        memcpy(g_renderContext_projection, &adjusted_projection, sizeof(NUMTX));

        NUMTX inverse_view;
        NuMtxInv(&inverse_view, view);
        g_renderContext_position[0] = inverse_view.m30 / inverse_view.m33;
        g_renderContext_position[1] = inverse_view.m31 / inverse_view.m33;
        g_renderContext_position[2] = inverse_view.m32 / inverse_view.m33;
        g_renderContext_position[3] = 1.0f;

        NuMtxMulH(reinterpret_cast<NUMTX *>(g_renderContext_viewProj), view, &adjusted_projection);
        NuMtxInvH(reinterpret_cast<NUMTX *>(g_renderContext_viewProjInverse),
                  reinterpret_cast<NUMTX *>(g_renderContext_viewProj));

        // OpenGL's clip-space depth is [-w,+w], while the engine camera
        // packet contains the original D3D-style [0,+w] projection.
        NUMTX depth_remap = numtx_identity;
        depth_remap.m22 = 2.0f;
        depth_remap.m32 = -1.0f;
        NuMtxMulH(reinterpret_cast<NUMTX *>(g_renderContext_viewProj),
                  reinterpret_cast<NUMTX *>(g_renderContext_viewProj), &depth_remap);

        NuShaderManagerSetfv(0x3d, g_renderContext_view);
        NuShaderManagerSetfv(0x3e, g_renderContext_viewProj);
        NuShaderManagerSetfv(0x56, g_renderContext_position);

        f32 fov;
        f32 aspect;
        f32 near_clip;
        f32 far_clip;
        f32 perspective[4];
        NuMtxGetPerspectiveD3D(projection, &fov, &aspect, &near_clip, &far_clip);
        perspective[0] = near_clip;
        perspective[1] = far_clip;
        perspective[2] = far_clip - near_clip;
        perspective[3] = perspective[2] / far_clip;
        NuShaderManagerSetfv(0x49, perspective);

        f32 frustum[4];
        NuMtxGetFrustumD3D(projection, &frustum[0], &frustum[1], &frustum[2], &frustum[3], &near_clip, &far_clip);
        frustum[1] -= frustum[0];
        frustum[3] -= frustum[2];
        NuShaderManagerSetfv(0x4a, frustum);
    }
}

void NuRenderContextForceSamplerStatePS(i32, const d3dsamplerstate_u *) {
    STUBBED();
}

extern "C" {
    void NuRenderContextSetZFunc(i32 zfunc) {
        if (zfunc == g_renderContext_zFunc) {
            return;
        }

        switch (zfunc) {
            case 0: // depth test + write, LEQUAL
                glEnable(GL_DEPTH_TEST);
                glDepthMask(GL_TRUE);
                glDepthFunc(GL_LEQUAL);
                break;
            case 1: // depth test, no write (decal / transparent)
                glEnable(GL_DEPTH_TEST);
                glDepthMask(GL_FALSE);
                glDepthFunc(GL_LEQUAL);
                break;
            case 2: // no depth test, write enabled
                glDisable(GL_DEPTH_TEST);
                glDepthMask(GL_TRUE);
                break;
            case 3: // no depth test, no write (UI / 2D)
                glDisable(GL_DEPTH_TEST);
                glDepthMask(GL_FALSE);
                break;
            default:
                break;
        }
        g_renderContext_zFunc = zfunc;
    }

    void NuRenderContextSetAlphaBlend(i32 blend, i32 alpha_reference) {
        NuSetBlendState(blend, [alpha_reference]() { return alpha_reference; });
    }

    void NuRenderContext360BeginGameTime(void) {
        STUBBED();
    }

    void NuRenderContext360EndGameTime(void) {
        STUBBED();
    }
}
