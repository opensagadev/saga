// Original camera TU basename is nucamera.c; compiled as C++ in this build.
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>

NUMTX clip_test_mtx;

extern "C" {
    // Original LOCAL helper at 0x2a3b35, called by the camera bounds query.
    static void VuVecMtxMul(NUVEC *out, NUVEC *value, NUMTX *matrix) {
        NuVecMtxTransform(out, value, matrix);
    }
}

void NuCameraSetProjectionMtx(NUMTX *mtx, f32 fov, f32 aspect, f32 near_clip, f32 far_clip) {
    near_clip = near_clip < 0.1f ? 0.1f : near_clip;
    i32 angle = (i32)(fov / 2.0f * 10430.378f);
    f32 cotangent = NuTrigTable[(angle + 0x4000) >> 1 & 0x7fff] / NuTrigTable[angle >> 1 & 0x7fff];
    f32 x_scale = aspect * cotangent;
    f32 y_scale = cotangent;
    f32 depth = far_clip / (far_clip - near_clip);
    memset(mtx, 0, sizeof(*mtx));
    mtx->m00 = x_scale;
    mtx->m11 = y_scale;
    mtx->m22 = depth;
    mtx->m23 = 1.0f;
    mtx->m32 = -depth * near_clip;
}

void NuCameraRestoreState(i32 handle) {
    i32 index = handle - 1;
    if (index < cam_state_count) {
        NUCAMERASTATE *state = &cam_state[index];
        global_camera = state->camera;
        vmtx = state->view;
        pmtx = state->projection;
        smtx = state->screen;
        vpmtx = state->view_projection;
        vpc_sci_mtx = state->view_projection_scissor;
        vpc_vport_mtx = state->view_projection_viewport;
        pc_vport_mtx = state->viewport;
        vpsmtx = state->view_projection_screen;
        psmtx = state->projection_screen;
        camfx = state->effects;
        zx = state->zx;
        zy = state->zy;
        zxs = state->zxs;
        zys = state->zys;
        // The original restores the derived matrices but not ClipPlanes or
        // the stack count; the handle remains available for subsequent use.
        FaceYDirStream(NuAtan2D(-global_camera.mtx.m20, -global_camera.mtx.m22));
        NuRndrSetViewMtx(&vpsmtx, &vpc_vport_mtx, &vpc_sci_mtx);
    }
}

void NuVecMtxTransformBlock(NUVEC *out, NUVEC *v, NUMTX *m, i32 count) {
    i32 i;

    for (i = 0; i < count; i++) {
        out->x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + m->m30;
        out->y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + m->m31;
        out->z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + m->m32;

        out++;
        v++;
    }
}

SAGA_HOST_WEAK i32 NuCameraClipTestExtents(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip,
                                           i32 should_clip_to_screen) {
    NUVEC extents[8];
    NUVEC clip_space_extents[8];
    char results[8];
    i32 i;

    if (far_clip == 0.0f) {
        if (global_camera.unknown_64 != 0.0f) {
            far_clip = global_camera.unknown_64;
        } else {
            far_clip = global_camera.far_clip;
        }
    }

    if (world_mtx == NULL || world_mtx == &numtx_identity) {
        clip_test_mtx = vmtx;
    } else {
        NuMtxMul(&clip_test_mtx, world_mtx, &vmtx);
    }

    extents[0].x = min->x;
    extents[0].y = min->y;
    extents[0].z = min->z;

    extents[1].x = max->x;
    extents[1].y = max->y;
    extents[1].z = max->z;

    extents[2].x = min->x;
    extents[2].y = min->y;
    extents[2].z = max->z;

    extents[3].x = max->x;
    extents[3].y = max->y;
    extents[3].z = min->z;

    extents[4].x = min->x;
    extents[4].y = max->y;
    extents[4].z = min->z;

    extents[5].x = min->x;
    extents[5].y = max->y;
    extents[5].z = max->z;

    extents[6].x = max->x;
    extents[6].y = min->y;
    extents[6].z = min->z;

    extents[7].x = max->x;
    extents[7].y = min->y;
    extents[7].z = max->z;

    NuVecMtxTransformBlock(clip_space_extents, extents, &clip_test_mtx, 8);
    memset(results, 0, sizeof(results));

    for (i = 0; i < 8; i++) {
        if (clip_space_extents[i].x > clip_space_extents[i].z * zx) {
            results[i] |= 0x02;
        }

        if (clip_space_extents[i].y > clip_space_extents[i].z * zy) {
            results[i] |= 0x04;
        }

        if (clip_space_extents[i].z > far_clip) {
            results[i] |= 0x10;
        }

        if (clip_space_extents[i].x < -clip_space_extents[i].z * zx) {
            results[i] |= 0x01;
        }

        if (clip_space_extents[i].y < -clip_space_extents[i].z * zy) {
            results[i] |= 0x08;
        }

        if (clip_space_extents[i].z < global_camera.near_clip) {
            results[i] |= 0x20;
        }
    }

    if ((results[0] & results[1] & results[2] & results[3] & results[4] & results[5] & results[6] & results[7]) != 0) {
        return 0;
    }

    if ((results[0] | results[1] | results[2] | results[3] | results[4] | results[5] | results[6] | results[7]) == 0) {
        return 1;
    }

    return 2;
}

// The display-scene format stores each axis-aligned bound as center + extent,
// not as minimum + maximum corners.  The target tests those values directly
// against the world-space planes built by NuCameraBuildClipPlanes.  Treating
// them as corners makes large bounds (notably the Cantina floor and walls)
// appear outside the camera even while the camera is inside them.
SAGA_HOST_WEAK i32 NuCameraClipTestExtentsAxisAligned(NUVEC *center, NUVEC *extent, f32 far_clip) {
    struct BoundProjection {
        NUVEC sides;
        f32 fourth;
        f32 far;
        f32 near;
    } distance, radius;
    const f32 saved_far = ClipPlanes.near_far_planes.m30;
    if (far_clip != 0.0f) {
        ClipPlanes.near_far_planes.m30 -= global_camera.far_clip;
        ClipPlanes.near_far_planes.m30 += far_clip;
    }
    VuVecMtxMul(&distance.sides, center, &ClipPlanes.frustum_planes);
    distance.fourth = ClipPlanes.frustum_planes.m33 +
                      (center->z * ClipPlanes.frustum_planes.m23 +
                       (center->x * ClipPlanes.frustum_planes.m03 + center->y * ClipPlanes.frustum_planes.m13));
    distance.far = ClipPlanes.near_far_planes.m30 +
                   (center->z * ClipPlanes.near_far_planes.m20 +
                    (center->x * ClipPlanes.near_far_planes.m00 + center->y * ClipPlanes.near_far_planes.m10));
    distance.near = ClipPlanes.near_far_planes.m31 +
                    (center->z * ClipPlanes.near_far_planes.m21 +
                     (center->x * ClipPlanes.near_far_planes.m01 + center->y * ClipPlanes.near_far_planes.m11));
    VuVecMtxMul(&radius.sides, extent, &ClipPlanes.abs_frustum_planes);
    radius.fourth = extent->x * ClipPlanes.abs_frustum_planes.m03 + extent->y * ClipPlanes.abs_frustum_planes.m13 +
                    extent->z * ClipPlanes.abs_frustum_planes.m23;
    radius.far = extent->x * ClipPlanes.near_far_planes.m02 + extent->y * ClipPlanes.near_far_planes.m12 +
                 extent->z * ClipPlanes.near_far_planes.m22;
    radius.near = extent->x * ClipPlanes.near_far_planes.m03 + extent->y * ClipPlanes.near_far_planes.m13 +
                  extent->z * ClipPlanes.near_far_planes.m23;
    ClipPlanes.near_far_planes.m30 = saved_far;

    if (distance.sides.x < -radius.sides.x || distance.sides.y < -radius.sides.y ||
        distance.sides.z < -radius.sides.z || distance.fourth < -radius.fourth || distance.far < -radius.far ||
        distance.near < -radius.near)
        return 0;
    if (distance.sides.x < radius.sides.x || distance.sides.y < radius.sides.y || distance.sides.z < radius.sides.z ||
        distance.fourth < radius.fourth) {
        VuVecMtxMul(&distance.sides, center, &ClipPlanes.scissor_planes);
        distance.fourth = ClipPlanes.scissor_planes.m33 +
                          (center->z * ClipPlanes.scissor_planes.m23 +
                           (center->x * ClipPlanes.scissor_planes.m03 + center->y * ClipPlanes.scissor_planes.m13));
        VuVecMtxMul(&radius.sides, extent, &ClipPlanes.abs_scissor_planes);
        radius.fourth =
            ClipPlanes.abs_scissor_planes.m33 +
            (extent->z * ClipPlanes.abs_scissor_planes.m23 +
             (extent->x * ClipPlanes.abs_scissor_planes.m03 + extent->y * ClipPlanes.abs_scissor_planes.m13));
        if (distance.sides.x < radius.sides.x || distance.sides.y < radius.sides.y ||
            distance.sides.z < radius.sides.z || distance.fourth < radius.fourth)
            return 2;
    }
    return 1;
}

// Original 0x2a5128: the render-stream camera packet closes the nucamera.c run.
void NuIOSDLCameraCallback(void *arg) {
    struct NuIOSCameraPacket {
        i32 id;
        NUMTX view;
        NUMTX projection;
        f32 viewport[4];
    };
    static i32 lastId = -1;
    g_boundCameraPacket = arg;
    auto *packet = static_cast<NuIOSCameraPacket *>(arg);
    if (packet->id != lastId) {
        NUMTX *view = &packet->view;
        NUMTX *projection = view + 1;
        f32 *viewport = reinterpret_cast<f32 *>(projection + 1);
        lastId = packet->id;
        NuRenderContextSetViewProj(view, projection);
        NuRenderContextSetViewport(static_cast<i32>(viewport[0]), static_cast<i32>(viewport[1]),
                                   static_cast<i32>(viewport[2]), static_cast<i32>(viewport[3]));
    } else {
        return;
    }
}
