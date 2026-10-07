#include <float.h>
#include <math.h>
#include <new>
#include <string.h>

#include "decomp.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nupostshaderparams.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

extern "C" void DisplaySceneRndrSpecials(NUDLDLISTSCENE *, i32, void *);
extern "C" void NuShaderManagerSetfv(i32, const f32 *);
extern const f32 nuvec4_one[4];

static inline void PostBlurSetVertexParam(nushaderprogram_s *, u32, const f32 *, i32);
void NuDynamicLight::bindShaderResources(nushaderprogram_s *program) {
    NUMTX inverse;
    NuMtxInv(&inverse, &view);
    NUVEC light;
    if (parameter_4 == 0) {
        light.x = 0.0f;
        light.y = 0.0f;
        light.z = -1.0f;
        NuVecMtxRotate(&light, &light, &inverse);
        NuVecMtxRotate(&light, &light, reinterpret_cast<NUMTX *>(g_renderContext_view));
        NuVecNorm(&light, &light);
    } else {
        light.x = inverse.m30;
        light.y = inverse.m31;
        light.z = inverse.m32;
        NuVecMtxTransformH(&light, &light, reinterpret_cast<NUMTX *>(g_renderContext_view));
    }
    PostBlurSetVertexParam(program, 0x8085, &light.x, 4);
    NUVEC4 parameters;
    parameters.x = reserved_7c0[0];
    parameters.y = reserved_7c0[1] > 0.0f ? 1.0f / reserved_7c0[1] : 0.0f;
    parameters.z = 1.0f - reserved_7c8.x;
    parameters.w = reserved_7c8.y;
    PostBlurSetVertexParam(program, 0x8086, &parameters.x, 4);
    NUVEC4 split = {active_render_set_count == 1 ? 1000.0f : split_distances[1], 0, 0, 0};
    PostBlurSetVertexParam(program, 0x8088, &split.x, 4);
    NUMTX bias __attribute__((aligned(16))) = {0.5f, 0, 0, 0, 0, -0.5f, 0, 0, 0, 0, 0.5f, 0, 0.5f, 0.5f, 0.5f, 1};
    i32 semantic = 0x8e;
    for (i32 i = 0; i < active_render_set_count; ++i, semantic += 7) {
        RenderSet &set = render_sets[i];
        NUMTX shadow;
        NuMtxTranspose(&shadow, &set.shadow_transform);
        PostBlurSetVertexParam(program, (semantic - 5) | 0x8000, &shadow.m00, 16);
        NUMTX transform;
        NuMtxMulH(&transform, &inverse, &set.warp);
        NuMtxMulH(&transform, &transform, &bias);
        NuMtxInvH(&transform, &transform);
        NUVEC4 row_z = {transform.m02, transform.m12, transform.m22, transform.m32};
        NUVEC4 row_w = {transform.m03, transform.m13, transform.m23, transform.m33};
        PostBlurSetVertexParam(program, (semantic - 1) | 0x8000, &row_z.x, 4);
        PostBlurSetVertexParam(program, semantic | 0x8000, &row_w.x, 4);
        NUVEC4 shadow_parameters;
        shadow_parameters.x = set.parameter_100;
        shadow_parameters.y = 1.0f - set.parameter_104;
        shadow_parameters.z = reserved_7c8.z;
        shadow_parameters.w = reserved_7c8.w - reserved_7c8.z;
        PostBlurSetVertexParam(program, (semantic + 1) | 0x8000, &shadow_parameters.x, 4);
    }
}

static inline void LightBoundsPoint(const VuVec &point, NUVEC &minimum, NUVEC &maximum) {
    if (point.x < minimum.x)
        minimum.x = point.x;
    else if (point.x > maximum.x)
        maximum.x = point.x;
    if (point.y < minimum.y)
        minimum.y = point.y;
    else if (point.y > maximum.y)
        maximum.y = point.y;
    if (point.z < minimum.z)
        minimum.z = point.z;
    else if (point.z > maximum.z)
        maximum.z = point.z;
}
void NuDynamicLight::computeBoundingSpace(const VuVec *points, VuMtx *matrix) {
    NUVEC minimum = {FLT_MAX, FLT_MAX, FLT_MAX};
    NUVEC maximum = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    LightBoundsPoint(points[0], minimum, maximum);
    LightBoundsPoint(points[1], minimum, maximum);
    LightBoundsPoint(points[2], minimum, maximum);
    LightBoundsPoint(points[3], minimum, maximum);
    LightBoundsPoint(points[4], minimum, maximum);
    LightBoundsPoint(points[5], minimum, maximum);
    LightBoundsPoint(points[6], minimum, maximum);
    LightBoundsPoint(points[7], minimum, maximum);
    NuMtxSetOrthoBlend(&matrix->matrix, minimum.x, maximum.x, minimum.y, maximum.y, minimum.z, maximum.z);
}

static inline VuVec LightClipTransform(const VuVec &point, const NUMTX &matrix) {
    VuVec result;
    result.x = point.x * matrix.m00;
    result.x += point.y * matrix.m10;
    result.x += point.z * matrix.m20;
    result.x += matrix.m30;
    result.y = point.x * matrix.m01;
    result.y += point.y * matrix.m11;
    result.y += point.z * matrix.m21;
    result.y += matrix.m31;
    result.z = point.x * matrix.m02;
    result.z += point.y * matrix.m12;
    result.z += point.z * matrix.m22;
    result.z += matrix.m32;
    result.w = point.x * matrix.m03;
    result.w += point.y * matrix.m13;
    result.w += point.z * matrix.m23;
    result.w += matrix.m33;
    const f32 reciprocal = 1.0f / result.w;
    result.x *= reciprocal;
    result.y *= reciprocal;
    result.z *= reciprocal;
    return result;
}
static inline NUVEC LightClipEdge(const VuVec &point, const VuVec &origin) {
    NUVEC edge = {point.x - origin.x, point.y - origin.y, point.z - origin.z};
    return edge;
}
static inline void LightClipPlane(VuVec &plane, const VuVec &origin, const NUVEC &first, const NUVEC &second) {
    plane.x = first.y * second.z - first.z * second.y;
    plane.y = first.z * second.x - first.x * second.z;
    plane.z = first.x * second.y - first.y * second.x;
    NuVecNorm((NUVEC *)&plane, (NUVEC *)&plane);
    plane.w = -(plane.x * origin.x + plane.y * origin.y + plane.z * origin.z);
}
void NuDynamicLight::computeClippingPlanes(const VuMtx &matrix, bool zero_near, VuVec &left, VuVec &right,
                                           VuVec &bottom, VuVec &top, VuVec &near_plane, VuVec &far_plane) {
    const VuVec symmetric[8] = {{-1, -1, -1, 1}, {-1, 1, -1, 1}, {1, 1, -1, 1}, {1, -1, -1, 1},
                                {-1, -1, 1, 1},  {-1, 1, 1, 1},  {1, 1, 1, 1},  {1, -1, 1, 1}};
    const VuVec positive[8] = {{-1, -1, 0, 1}, {-1, 1, 0, 1}, {1, 1, 0, 1}, {1, -1, 0, 1},
                               {-1, -1, 1, 1}, {-1, 1, 1, 1}, {1, 1, 1, 1}, {1, -1, 1, 1}};
    const VuVec *source = zero_near ? positive : symmetric;
    VuVec corners[8];
    VuVec *destination = corners;
    const VuVec *end = source + 8;
    do {
        *destination++ = LightClipTransform(*source++, matrix.matrix);
    } while (source != end);
    NUVEC near_up = LightClipEdge(corners[1], corners[0]);
    NUVEC depth_left = LightClipEdge(corners[4], corners[0]);
    NUVEC near_right = LightClipEdge(corners[3], corners[0]);
    NUVEC far_left = LightClipEdge(corners[5], corners[6]);
    NUVEC far_down = LightClipEdge(corners[7], corners[6]);
    NUVEC depth_right = LightClipEdge(corners[2], corners[6]);
    LightClipPlane(left, corners[0], near_up, depth_left);
    LightClipPlane(right, corners[6], depth_right, far_down);
    LightClipPlane(top, corners[6], far_left, depth_right);
    LightClipPlane(bottom, corners[0], depth_left, near_right);
    LightClipPlane(far_plane, corners[6], far_down, far_left);
    LightClipPlane(near_plane, corners[0], near_right, near_up);
}

void NuDynamicLight::computeFrustumCube(nucamera_s const *camera, VuVec *corners, VuVec *far_plane) {
    i32 angle = static_cast<i32>((camera->fov * 0.5f) * 10430.3779296875f);
    f32 near_z = camera->near_clip;
    f32 near_y = near_z * NU_SIN_LUT(angle) / NU_COS_LUT(angle);
    f32 near_x = near_y / camera->aspect;
    f32 ratio = camera->far_clip / near_z;
    corners[0].w = corners[1].w = corners[2].w = corners[3].w = 1.0f;
    corners[4].w = corners[5].w = corners[6].w = corners[7].w = 0.0f;
    corners[0].x = -near_x;
    corners[0].y = -near_y;
    corners[0].z = near_z;
    corners[1].x = -near_x;
    corners[1].y = near_y;
    corners[1].z = near_z;
    corners[2].x = near_x;
    corners[2].y = near_y;
    corners[2].z = near_z;
    corners[3].x = near_x;
    corners[3].y = -near_y;
    corners[3].z = near_z;
    corners[4].x = corners[0].x * ratio;
    corners[4].y = corners[0].y * ratio;
    corners[4].z = corners[0].z * ratio;
    corners[5].x = corners[1].x * ratio;
    corners[5].y = corners[1].y * ratio;
    corners[5].z = corners[1].z * ratio;
    corners[6].x = corners[2].x * ratio;
    corners[6].y = corners[2].y * ratio;
    corners[6].z = corners[2].z * ratio;
    corners[7].x = corners[3].x * ratio;
    corners[7].y = corners[3].y * ratio;
    corners[7].z = corners[3].z * ratio;
    NUMTX matrix;
    memcpy(&matrix, &camera->mtx, sizeof(matrix));
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[0]), reinterpret_cast<NUVEC *>(&corners[0]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[1]), reinterpret_cast<NUVEC *>(&corners[1]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[2]), reinterpret_cast<NUVEC *>(&corners[2]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[3]), reinterpret_cast<NUVEC *>(&corners[3]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[4]), reinterpret_cast<NUVEC *>(&corners[4]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[5]), reinterpret_cast<NUVEC *>(&corners[5]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[6]), reinterpret_cast<NUVEC *>(&corners[6]), &matrix);
    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&corners[7]), reinterpret_cast<NUVEC *>(&corners[7]), &matrix);
    NUVEC edge1 = {corners[5].x - corners[4].x, corners[5].y - corners[4].y, corners[5].z - corners[4].z};
    NUVEC edge2 = {corners[6].x - corners[4].x, corners[6].y - corners[4].y, corners[6].z - corners[4].z};
    far_plane->x = edge1.y * edge2.z - edge1.z * edge2.y;
    far_plane->y = edge1.z * edge2.x - edge1.x * edge2.z;
    far_plane->z = edge1.x * edge2.y - edge1.y * edge2.x;
    NuVecNorm(reinterpret_cast<NUVEC *>(far_plane), reinterpret_cast<NUVEC *>(far_plane));
    far_plane->w = -(far_plane->x * corners[4].x + far_plane->y * corners[4].y + far_plane->z * corners[4].z);
}

void NuDynamicLight::computeLightSpace(NUVEC *direction, NUVEC *camera, NUMTX *view, NUMTX *inverse) {
    NuMtxSetIdentity(inverse);
    NUVEC *up = reinterpret_cast<NUVEC *>(&inverse->m10);
    up->z = -direction->z;
    up->y = -direction->y;
    up->x = -direction->x;
    NUVEC cross;
    cross.z = up->y * camera->x - up->x * camera->y;
    cross.y = up->x * camera->z - up->z * camera->x;
    cross.x = up->z * camera->y - up->y * camera->z;
    NUVEC forward;
    forward.z = up->x * cross.y - up->y * cross.x;
    forward.y = up->z * cross.x - up->x * cross.z;
    forward.x = up->y * cross.z - up->z * cross.y;
    f32 length = NuFsqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    forward.z /= length;
    forward.y /= length;
    forward.x /= length;
    NUVEC *axis = reinterpret_cast<NUVEC *>(&inverse->m20);
    axis->z = forward.z;
    axis->y = forward.y;
    axis->x = forward.x;
    NUVEC *right = reinterpret_cast<NUVEC *>(&inverse->m00);
    right->x = up->y * axis->z - up->z * axis->y;
    right->y = up->z * axis->x - up->x * axis->z;
    right->z = up->x * axis->y - up->y * axis->x;
    NuMtxInv(view, inverse);
}

static inline NUVEC ShadowCross(const VuVec &a, const VuVec &b, const VuVec &origin) {
    NUVEC first = {b.x - origin.x, b.y - origin.y, b.z - origin.z};
    NUVEC second = {a.x - origin.x, a.y - origin.y, a.z - origin.z};
    NUVEC normal;
    normal.y = first.x * second.z - first.z * second.x;
    normal.z = second.x * first.y - first.x * second.y;
    normal.x = second.y * first.z - second.z * first.y;
    return normal;
}
i32 NuDynamicLight::computeShadowClippingPlanes(const VuVec &direction, const VuVec *corners, VuVec *planes) {
    NUVEC extrusion = {direction.x * -120.0f, direction.y * -120.0f, direction.z * -120.0f};
    i32 count = 0;
    bool front[6];
    {
        NUVEC normal = ShadowCross(corners[1], corners[2], corners[0]);
        front[0] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[0]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[0].x + plane.y * corners[0].y + plane.z * corners[0].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[2], corners[6], corners[3]);
        front[1] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[1]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[3].x + plane.y * corners[3].y + plane.z * corners[3].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[6], corners[5], corners[7]);
        front[2] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[2]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[7].x + plane.y * corners[7].y + plane.z * corners[7].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[5], corners[1], corners[4]);
        front[3] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[3]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[4].x + plane.y * corners[4].y + plane.z * corners[4].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[5], corners[6], corners[1]);
        front[4] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[4]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[1].x + plane.y * corners[1].y + plane.z * corners[1].z);
        }
    }
    {
        NUVEC normal = ShadowCross(corners[0], corners[3], corners[4]);
        front[5] = extrusion.x * normal.x + extrusion.y * normal.y + extrusion.z * normal.z >= 0.0f;
        if (!front[5]) {
            VuVec &plane = planes[count++];
            plane.x = -normal.x;
            plane.y = -normal.y;
            plane.z = -normal.z;
            plane.w = 0.0f;
            NuVecNorm(&plane.xyz, &plane.xyz);
            plane.w = -(plane.x * corners[4].x + plane.y * corners[4].y + plane.z * corners[4].z);
        }
    }
    static const i32 edge_faces[12][2] = {{3, 0}, {4, 0}, {1, 0}, {5, 0}, {2, 3}, {2, 4},
                                          {2, 1}, {2, 5}, {5, 3}, {3, 4}, {4, 1}, {1, 5}};
    static const i32 edge_vertices[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6},
                                             {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
    for (i32 edge = 0; edge < 12 && count <= 8; ++edge) {
        bool first_front = front[edge_faces[edge][0]];
        if (first_front == front[edge_faces[edge][1]])
            continue;
        const VuVec &first = corners[edge_vertices[edge][0]];
        const VuVec &second = corners[edge_vertices[edge][1]];
        VuVec extruded = {extrusion.x + first.x, extrusion.y + first.y, extrusion.z + first.z, 0.0f};
        VuVec &plane = planes[count++];
        if (first_front) {
            NUVEC normal = ShadowCross(second, extruded, first);
            plane.x = normal.x;
            plane.y = normal.y;
            plane.z = normal.z;
            NuVecNorm((NUVEC *)&plane, (NUVEC *)&plane);
            plane.w = -(plane.x * first.x + plane.y * first.y + plane.z * first.z);
        } else {
            NUVEC normal = ShadowCross(second, first, extruded);
            plane.x = normal.x;
            plane.y = normal.y;
            plane.z = normal.z;
            NuVecNorm((NUVEC *)&plane, (NUVEC *)&plane);
            plane.w = -(extruded.x * plane.x + extruded.y * plane.y + extruded.z * plane.z);
        }
    }
    return count;
}

static inline void CapsuleAdd(VuVec &sum, const VuVec &point) {
    sum.x += point.x;
    sum.y += point.y;
    sum.z += point.z;
}
static inline float CapsuleDistance(const VuVec &center, const VuVec &point, float &radius) {
    float x = center.x - point.x, y = center.y - point.y, z = center.z - point.z;
    float distance = x * x + y * y + z * z;
    radius = distance > radius ? distance : radius;
    return distance;
}
void NuDynamicLight::computeShadowFrustrumCapsule(const VuVec &direction, const VuVec *corners, VuVec &center,
                                                  VuVec &end, float &radius) {
    center.x = corners[0].x;
    center.y = corners[0].y;
    center.z = corners[0].z;
    center.w = corners[0].w;
    CapsuleAdd(center, corners[1]);
    CapsuleAdd(center, corners[2]);
    CapsuleAdd(center, corners[3]);
    CapsuleAdd(center, corners[4]);
    CapsuleAdd(center, corners[5]);
    CapsuleAdd(center, corners[6]);
    CapsuleAdd(center, corners[7]);
    center.x *= 0.125f;
    center.y *= 0.125f;
    center.z *= 0.125f;
    radius = 0.0f;
    CapsuleDistance(center, corners[0], radius);
    CapsuleDistance(center, corners[1], radius);
    CapsuleDistance(center, corners[2], radius);
    CapsuleDistance(center, corners[3], radius);
    CapsuleDistance(center, corners[4], radius);
    CapsuleDistance(center, corners[5], radius);
    CapsuleDistance(center, corners[6], radius);
    float distance = CapsuleDistance(center, corners[7], radius);
    // The original takes the final corner distance here, after the maximum stores.
    radius = NuFsqrt(distance);
    VuVec result = {center.x - direction.x * 100.0f, center.y - direction.y * 100.0f, center.z - direction.z * 100.0f,
                    0.0f};
    end.x = result.x;
    end.y = result.y;
    end.z = result.z;
    end.w = 0.0f;
}

static inline VuVec WarpTransformPoint(const VuVec &point, const NUMTX &matrix) {
    NUMTX copy;
    copy.m00 = matrix.m00;
    copy.m01 = matrix.m01;
    copy.m02 = matrix.m02;
    copy.m03 = matrix.m03;
    copy.m10 = matrix.m10;
    copy.m11 = matrix.m11;
    copy.m12 = matrix.m12;
    copy.m13 = matrix.m13;
    copy.m20 = matrix.m20;
    copy.m21 = matrix.m21;
    copy.m22 = matrix.m22;
    copy.m23 = matrix.m23;
    copy.m30 = matrix.m30;
    copy.m31 = matrix.m31;
    copy.m32 = matrix.m32;
    copy.m33 = matrix.m33;
    VuVec output;
    NuVecMtxTransformH(&output.xyz, const_cast<NUVEC *>(&point.xyz), &copy);
    return output;
}
static inline void WarpAccumulatePoint(const VuVec &point, const NUMTX &matrix, VuVec &output, f32 &min_z, f32 &min_y,
                                       f32 &max_z, f32 &max_y) {
    VuVec result = WarpTransformPoint(point, matrix);
    output.x = result.x;
    output.y = result.y;
    output.z = result.z;
    output.w = result.w;
    min_z = result.z < min_z ? result.z : min_z;
    min_y = result.y < min_y ? result.y : min_y;
    max_z = max_z < result.z ? result.z : max_z;
    max_y = max_y < result.y ? result.y : max_y;
}
static inline void WarpProjectBounds(VuVec &point, NUVEC &translation, f32 near_plane, f32 &left, f32 &bottom,
                                     f32 &right, f32 &top) {
    NUVEC shifted;
    NuVecAdd(&shifted, &point.xyz, &translation);
    f32 x = shifted.x * near_plane / shifted.z;
    f32 y = shifted.y * near_plane / shifted.z;
    if (left > x)
        left = x;
    else if (x > right)
        right = x;
    if (bottom > y)
        bottom = y;
    else if (y > top)
        top = y;
}
void NuDynamicLight::computeWarpEffect(NuDynamicLight::RenderSet &set) {
    NUMTX render_view = set.view;
    NUMTX camera_view = cacheCameraView;
    NUMTX render_view_projection, inverse_view_projection, inverse_render_view, inverse_camera_view;
    NuMtxMulH(&render_view_projection, &render_view, &set.projection);
    NuMtxInvH(&inverse_view_projection, &render_view_projection);
    NuMtxInv(&inverse_render_view, &render_view);
    NuMtxInv(&inverse_camera_view, &camera_view);
    NUVEC light_forward = {inverse_render_view.m20, inverse_render_view.m21, inverse_render_view.m22};
    NUVEC camera_forward = {inverse_camera_view.m20, inverse_camera_view.m21, inverse_camera_view.m22};
    NUMTX light_space, inverse_light_space;
    computeLightSpace(&light_forward, &camera_forward, &light_space, &inverse_light_space);
    VuVec points[10];
    const NUVEC4 *source = set.corners;
    VuVec *destination = points;
    const NUVEC4 *end = set.corners + 8;
    while (source < end) {
        *destination++ = VuVec(source->x, source->y, source->z, source->w);
        ++source;
    }
    points[8].x = direction.x * 200.0f + points[0].x;
    points[8].y = direction.y * 200.0f + points[0].y;
    points[8].z = direction.z * 200.0f + points[0].z;
    points[8].w = 0.0f;
    points[9].x = points[0].x - direction.x * 200.0f;
    points[9].y = points[0].y - direction.y * 200.0f;
    points[9].z = points[0].z - direction.z * 200.0f;
    points[9].w = 0.0f;
    VuVec transformed[10];
    f32 min_z = FLT_MAX, min_y = FLT_MAX, max_z = -FLT_MAX, max_y = -FLT_MAX;
    for (i32 i = 0; i < 10; ++i)
        WarpAccumulatePoint(points[i], light_space, transformed[i], min_z, min_y, max_z, max_y);
    f32 fovy, aspect, camera_near, camera_far;
    NuMtxGetPerspectiveD3D(&cacheCameraProj, &fovy, &aspect, &camera_near, &camera_far);
    f32 depth = max_z - min_z;
    camera_far = depth;
    f32 weight = 1.0f - fabsf(camera_forward.x * light_forward.x + camera_forward.y * light_forward.y +
                              camera_forward.z * light_forward.z);
    f32 near_plane = (NuFsqrt(depth * camera_near) + camera_near) * set.warp_factor / weight;
    NUVEC camera_position = {inverse_camera_view.m30, inverse_camera_view.m31, inverse_camera_view.m32};
    NUVEC camera_light;
    NuVecMtxTransformH(&camera_light, &camera_position, &light_space);
    NUVEC translation = {-camera_light.x, -((max_y + min_y) * 0.5f), -(min_z - near_plane)};
    f32 left = FLT_MAX, bottom = FLT_MAX, right = -FLT_MAX, top = -FLT_MAX;
    for (i32 i = 0; i < 10; ++i)
        WarpProjectBounds(transformed[i], translation, near_plane, left, bottom, right, top);
    NUMTX offset, frustum;
    NuMtxSetTranslation(&offset, &translation);
    NuMtxSetFrustumBlend(&frustum, left, right, bottom, top, near_plane, depth + near_plane);
    NUMTX axes = {1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1};
    NUMTX shifted_light, projected_light, result;
    NuMtxMulH(&shifted_light, &light_space, &offset);
    NuMtxMulH(&projected_light, &shifted_light, &frustum);
    NuMtxMulH(&result, &projected_light, &axes);
    set.warp = result;
}

void NuDynamicLight::refreshShadowTransform(RenderSet &set) {
    NUMTX inverse_camera;
    NuMtxInvH(&inverse_camera, &cacheCameraView);
    computeWarpEffect(set);
    NUMTX bias = {0.5f, 0, 0, 0, 0, -0.5f, 0, 0, 0, 0, 0.5f, 0, 0.5f, 0.5f, 0.5f, 1};
    NuMtxMulH(&set.shadow_transform, &inverse_camera, &set.warp);
    NuMtxMulH(&set.shadow_transform, &set.shadow_transform, &bias);
}

static inline void SetLightCameraVector(NUVEC4 &destination, f32 x, f32 y, f32 z, f32 w) {
    destination.x = x;
    destination.y = y;
    destination.z = z;
    destination.w = w;
}
void NuDynamicLight::setupCustomCameraFrustum(NUCAMERA *camera, const f32 *splits, i32 split_count) {
    NuMtxSetPerspectiveD3D(&cacheCameraProj, (180.0f * camera->fov) / 3.1415927410125732f, 1.0f / camera->aspect,
                           camera->near_clip, camera->far_clip);
    NuMtxInv(&cacheCameraView, &camera->mtx);
    SetLightCameraVector(camera_position, camera->mtx.m30, camera->mtx.m31, camera->mtx.m32, 1.0f);
    SetLightCameraVector(camera_forward, camera->mtx.m20, camera->mtx.m21, camera->mtx.m22, 0.0f);
    i32 interval_count = split_count - 1;
    active_render_set_count = interval_count;
    for (i32 i = 0; i < split_count; ++i) {
        split_distances[i] = splits[i];
    }
    f32 near_clip = camera->near_clip;
    f32 far_clip = camera->far_clip;
    for (i32 i = 0; i < interval_count; ++i) {
        RenderSet &set = render_sets[i];
        camera->near_clip = splits[i];
        camera->far_clip = splits[i + 1];
        computeFrustumCube(camera, reinterpret_cast<VuVec *>(set.corners), reinterpret_cast<VuVec *>(&set.far_plane));
        set.shadow_plane_count = computeShadowClippingPlanes(reinterpret_cast<const VuVec &>(direction),
                                                             reinterpret_cast<const VuVec *>(set.corners),
                                                             reinterpret_cast<VuVec *>(set.shadow_planes));
        set.view = view;
        set.projection = projection;
        computeShadowFrustrumCapsule(reinterpret_cast<const VuVec &>(direction),
                                     reinterpret_cast<const VuVec *>(set.corners),
                                     reinterpret_cast<VuVec &>(set.capsule_center),
                                     reinterpret_cast<VuVec &>(set.capsule_end), set.capsule_radius);
    }
    for (i32 i = 0; i < interval_count; ++i) {
        split_planes[i].x = 0.0f;
        split_planes[i].y = 0.0f;
        split_planes[i].z = 1.0f / splits[i + 1];
        split_planes[i].w = -1.0f;
    }
    camera->near_clip = near_clip;
    camera->far_clip = far_clip;
}

bool NuDynamicLight::testShadowExtrusion(const VuVec &minimum, const VuVec &maximum, i32 set_index) {
    const RenderSet &set = render_sets[set_index];
    const f32 min_x = minimum.x, min_y = minimum.y, min_z = minimum.z;
    const f32 max_y = maximum.y, max_x = maximum.x, max_z = maximum.z;
    const i32 count = set.shadow_plane_count;
    for (i32 i = 0; i < count; ++i) {
        const NUVEC4 &plane = set.shadow_planes[i];
        f32 x0 = min_x * plane.x;
        f32 y0 = min_y * plane.y;
        f32 z0 = min_z * plane.z;
        f32 xy00 = x0 + y0;
        if ((xy00 + z0) + plane.w > 0.0f)
            continue;
        f32 y1 = max_y * plane.y;
        f32 xy01 = x0 + y1;
        if ((xy01 + z0) + plane.w > 0.0f)
            continue;
        f32 x1 = max_x * plane.x;
        f32 xy11 = y1 + x1;
        if ((z0 + xy11) + plane.w > 0.0f)
            continue;
        f32 xy10 = y0 + x1;
        if ((z0 + xy10) + plane.w > 0.0f)
            continue;
        f32 z1 = max_z * plane.z;
        if ((xy00 + z1) + plane.w > 0.0f)
            continue;
        if ((xy01 + z1) + plane.w > 0.0f)
            continue;
        if ((xy11 + z1) + plane.w > 0.0f)
            continue;
        if ((xy10 + z1) + plane.w > 0.0f)
            continue;
        return false;
    }
    return true;
}

i32 NuDynamicLight::testShadowExtrusions(const VuVec &minimum, const VuVec &maximum) {
    i32 result = 0;
    for (i32 i = 0; i < active_render_set_count; ++i) {
        if (testShadowExtrusion(minimum, maximum, i))
            result |= 1 << i;
    }
    return result;
}
