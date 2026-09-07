#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include <float.h>
extern "C" void NuRndrStateSetSpecularLightEx(NUVEC *, NUMTX *, const NUCOLOUR4 *);

extern "C" i32 NuRndrSetSpecularLightPS(const NUVEC *direction, const NUCOLOUR4 *intensity) {
    static NUVEC rndrstream_specular_dir = {0.0f, 0.0f, 1.0f};
    static NUCOLOUR4 rndrstream_specular_intensity = {1.0f, 1.0f, 1.0f, 1.0f};
    static NUVEC camvec = {0.0f, 0.0f, -1.0f};
    if (direction != NULL) rndrstream_specular_dir = *direction;
    if (intensity != NULL) rndrstream_specular_intensity = *intensity;
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
    NuRndrStateSetSpecularLightEx(&view_direction, &matrix, &rndrstream_specular_intensity);
    return 1;
}
