#pragma once

#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"

// Inline forms of the NuMtxRotate* post-rotations. Some original translation units expand these rotations in place
// instead of calling the out-of-line versions in numtx.cpp; the arithmetic is identical.

#define NU_MTX_ROTATE_X_IMPL(function_name, storage)                                                                   \
    storage void function_name(NUMTX *m, NUANG a) {                                                                    \
        f32 cosx = NU_COS_LUT(a);                                                                                      \
        f32 sinx = NU_SIN_LUT(a);                                                                                      \
        f32 m01 = m->m01;                                                                                              \
        f32 m11 = m->m11;                                                                                              \
        f32 m21 = m->m21;                                                                                              \
        f32 m31 = m->m31;                                                                                              \
                                                                                                                       \
        m->m01 = m01 * cosx - m->m02 * sinx;                                                                           \
        m->m02 = m01 * sinx + m->m02 * cosx;                                                                           \
        m->m11 = m11 * cosx - m->m12 * sinx;                                                                           \
        m->m12 = m11 * sinx + m->m12 * cosx;                                                                           \
        m->m21 = m21 * cosx - m->m22 * sinx;                                                                           \
        m->m22 = m21 * sinx + m->m22 * cosx;                                                                           \
        m->m31 = m31 * cosx - m->m32 * sinx;                                                                           \
        m->m32 = m31 * sinx + m->m32 * cosx;                                                                           \
    }

NU_MTX_ROTATE_X_IMPL(NuMtxRotateXInline, static inline)

#define NU_MTX_ROTATE_Y_IMPL(function_name, storage)                                                                   \
    storage void function_name(NUMTX *m, NUANG a) {                                                                    \
        f32 cosx = NU_COS_LUT(a);                                                                                      \
        f32 sinx = NU_SIN_LUT(a);                                                                                      \
        f32 m00 = m->m00;                                                                                              \
        f32 m10 = m->m10;                                                                                              \
        f32 m20 = m->m20;                                                                                              \
        f32 m30 = m->m30;                                                                                              \
                                                                                                                       \
        m->m00 = m00 * cosx + m->m02 * sinx;                                                                           \
        m->m02 = m->m02 * cosx - m00 * sinx;                                                                           \
        m->m10 = m10 * cosx + m->m12 * sinx;                                                                           \
        m->m12 = m->m12 * cosx - m10 * sinx;                                                                           \
        m->m20 = m20 * cosx + m->m22 * sinx;                                                                           \
        m->m22 = m->m22 * cosx - m20 * sinx;                                                                           \
        m->m30 = m30 * cosx + m->m32 * sinx;                                                                           \
        m->m32 = m->m32 * cosx - m30 * sinx;                                                                           \
    }

NU_MTX_ROTATE_Y_IMPL(NuMtxRotateYInline, static inline)

#define NU_MTX_ROTATE_Z_IMPL(function_name, storage)                                                                   \
    storage void function_name(NUMTX *m, NUANG a) {                                                                    \
        f32 cosx = NU_COS_LUT(a);                                                                                      \
        f32 sinx = NU_SIN_LUT(a);                                                                                      \
        f32 m00 = m->m00;                                                                                              \
        f32 m10 = m->m10;                                                                                              \
        f32 m20 = m->m20;                                                                                              \
        f32 m30 = m->m30;                                                                                              \
                                                                                                                       \
        m->m00 = m00 * cosx - m->m01 * sinx;                                                                           \
        m->m01 = m00 * sinx + m->m01 * cosx;                                                                           \
        m->m10 = m10 * cosx - m->m11 * sinx;                                                                           \
        m->m11 = m10 * sinx + m->m11 * cosx;                                                                           \
        m->m20 = m20 * cosx - m->m21 * sinx;                                                                           \
        m->m21 = m20 * sinx + m->m21 * cosx;                                                                           \
        m->m30 = m30 * cosx - m->m31 * sinx;                                                                           \
        m->m31 = m30 * sinx + m->m31 * cosx;                                                                           \
    }

NU_MTX_ROTATE_Z_IMPL(NuMtxRotateZInline, static inline)

#define NU_MTX_SET_ROTATION_Y_IMPL(function_name, storage)                                                              \
    storage void function_name(NUMTX *m, NUANG a) {                                                                    \
        m->m00 = m->m22 = NU_COS_LUT(a);                                                                               \
        m->m20 = NU_SIN_LUT(a);                                                                                       \
        m->m02 = -m->m20;                                                                                             \
        m->m11 = 1.0f;                                                                                                \
        m->m01 = m->m10 = m->m03 = m->m23 = m->m12 = m->m21 = m->m13 = m->m30 = m->m31 = m->m32 = 0.0f;               \
        m->m33 = 1.0f;                                                                                                \
    }

NU_MTX_SET_ROTATION_Y_IMPL(NuMtxSetRotationYInline, static inline)
