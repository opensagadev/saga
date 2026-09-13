#include "nu2api/nu3d/nurndr.h"
#include "decomp.h"
#include "globals.h"
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/android/nurenderthread.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"

#include <float.h>
#include <string.h>

extern "C" {
    // Original 0x295fa9: private to this renderer translation unit.
    static void NuDisplayListResetBuffer(void) {
        display_list_buffer = reinterpret_cast<VARIPTR *>(&rndrstream_free);
        display_list_buffer_end = reinterpret_cast<VARIPTR *>(rndrstream_end.addr);
    }

    // Original 0x295fd7.
    static void NuDisplayListCheckBuffer(void) {
    }
}

extern "C" void FaceYDirStream(i32 y_angle) {
}

extern "C" i32 NuRndrSetViewMtx(NUMTX *vpcs_mtx, NUMTX *viewport_vpc_mtx, NUMTX *scissor_vpc_mtx) {
    return 0;
}

extern "C" i32 NuRndrSetSpecularLightPS(const NUVEC *direction, const NUCOLOUR4 *intensity) {
    static NUVEC rndrstream_specular_dir = {0.0f, 0.0f, 1.0f};
    static NUCOLOUR4 rndrstream_specular_intensity = {1.0f, 1.0f, 1.0f, 1.0f};
    static NUVEC camvec = {0.0f, 0.0f, -1.0f};
    if (direction != NULL)
        rndrstream_specular_dir = *direction;
    if (intensity != NULL)
        rndrstream_specular_intensity = *intensity;
    NUVEC view_direction, half_direction, light_direction, eye_direction;
    NuVecInvMtxRotate(&view_direction, &rndrstream_specular_dir, &global_camera.mtx);
    NuVecLerp(&half_direction, &view_direction, &camvec, 0.5f);
    NuVecMtxRotate(&half_direction, &half_direction, &global_camera.mtx);
    NuVecNorm(&half_direction, &half_direction);
    NuVecNorm(&light_direction, &rndrstream_specular_dir);
    eye_direction.x = -global_camera.mtx.m20;
    eye_direction.y = -global_camera.mtx.m21;
    eye_direction.z = -global_camera.mtx.m22;
    f32 facing = NuVecDot(&eye_direction, &light_direction);
    NUVEC axis = half_direction;
    NUMTX matrix = numtx_identity;
    f32 horizontal = NuFsqrt(axis.x * axis.x + axis.z * axis.z);
    if (horizontal > FLT_MIN) {
        matrix.m00 = axis.z / horizontal;
        matrix.m10 = (axis.x * axis.y) / horizontal;
        matrix.m20 = -axis.x;
        matrix.m01 = 0.0f;
        matrix.m11 = horizontal;
        matrix.m21 = axis.y;
        matrix.m02 = axis.x / horizontal;
        matrix.m12 = (axis.z * -axis.y) / horizontal;
        matrix.m22 = axis.z;
    } else {
        matrix.m22 = matrix.m11 = 0.0f;
        matrix.m21 = axis.y;
        matrix.m12 = -axis.y;
    }
    matrix.m30 = matrix.m31 = 0.5f;
    if (0.0f > facing) {
        f32 weight = facing * facing;
        weight *= weight;
        weight *= weight;
        weight *= weight;
        weight *= weight;
        f32 remainder = 1.0f - weight;
        if (matrix.m00 * light_direction.x + matrix.m10 * light_direction.y + matrix.m20 * light_direction.z >= 0.0f) {
            matrix.m00 = matrix.m00 * remainder + light_direction.x * weight;
            matrix.m10 = matrix.m10 * remainder + light_direction.y * weight;
            matrix.m20 = matrix.m20 * remainder + light_direction.z * weight;
        } else {
            matrix.m00 = matrix.m00 * remainder - light_direction.x * weight;
            matrix.m10 = matrix.m10 * remainder - light_direction.y * weight;
            matrix.m20 = matrix.m20 * remainder - light_direction.z * weight;
        }
        if (matrix.m01 * light_direction.x + matrix.m11 * light_direction.y + matrix.m21 * light_direction.z >= 0.0f) {
            matrix.m01 = matrix.m01 * remainder + light_direction.x * weight;
            matrix.m11 = matrix.m11 * remainder + light_direction.y * weight;
            matrix.m21 = matrix.m21 * remainder + light_direction.z * weight;
        } else {
            matrix.m01 = matrix.m01 * remainder - light_direction.x * weight;
            matrix.m11 = matrix.m11 * remainder - light_direction.y * weight;
            matrix.m21 = matrix.m21 * remainder - light_direction.z * weight;
        }
    }
    NuVecNeg(&view_direction, &view_direction);
    NuRndrStateSetSpecularLightEx(&view_direction, &matrix,
                                  reinterpret_cast<const NUCOLOUR3 *>(&rndrstream_specular_intensity));
    return 1;
}

// Original 0x2967db: present the frame, then pace until the app is active.
extern "C" SAGA_HOST_WEAK i32 NuRndrSwapScreen(i32 /*mode*/) {
    NuRenderThreadLock();
    rndr_blend_shape_deformer_wt_cnt = 0x3f00;
    rndr_blend_shape_deformer_wt_ptrs_cnt = 0x800;
    NuRenderThreadPrepareRender();
    NuShaderManagerBindShader(0);
    NuDebrisRendererFlushBuffers();
    NuDisplayListSwapBuffersEndFrame();
    NuRndrSwapStreamBuffers();
    NuDisplayListSwapBuffersBeginFrame();
    NuDisplayListCheckBuffer();
    NuDisplayListResetBuffer();
    NuRenderThreadUnlock();
    NuRenderThreadStartRender();

    for (;;) {
        if (NuCore::GetApplicationState()->GetStatus() != 1) {
            break;
        }
        g_isBlockedInSwapScreen = 1;
        NuThreadSleep(1);
    }
    g_isBlockedInSwapScreen = 0;
    return 1;
}

// Original 0x296888.
extern "C" i32 NuRndrSwapScreenEx(i32 mode, void (*callback)(void)) {
    if (callback != nullptr) {
        callback();
    }
    return NuRndrSwapScreen(mode);
}

// Original 0x2968b8: finish the frame's laser effects inside the renderer TU.
extern "C" void NuRndrFx(i32 paused, void *) {
    if (NuRndrBeginSceneEx(-1, -2, 0) != 0) {
        NuLgtLaserDraw(paused);
        NuLgtArcLaserDraw(paused);
        NuRndrEndSceneEx(0);
    }
}
