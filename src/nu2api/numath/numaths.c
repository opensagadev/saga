#include "decomp.h"
#include "nu2api_numath_types.h"
#include "nu2api/numath/vuvec_internal.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuplane.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nucamera.h"

extern "C" {
    i32 NuPlnLineVU0(NUPLANE *plane, NUVEC *start, NUVEC *end, NUVEC *out) {
        return NuPlnLine(plane, start, end, out);
    }
}

i32 NuVecClipTestPointVU0(nuvec_s *, numtx_s *) {
    STUBBED();
    return 0;
}

extern "C" {
    void NuVec4MtxTransformHVU0(NUVEC4 *out, NUVEC4 *v, NUMTX *matrix) {
        NuVec4MtxTransformH(out, v, matrix);
    }

    void NuMtxMulArrayVU0(NUMTX *out, NUMTX *left, NUMTX *right, i32 count) {
        for (i32 i = 0; i < count; ++i)
            NuMtxMulH(&out[i], &left[i], &right[i]);
    }

    void NuVecConvertToIntVU0(void) {
        STUBBED();
    }

    void NuVec4ScaleXYZVU0(NUVEC4 *out, NUVEC4 *v, f32 scale) {
        NuVec4Scale(out, v, scale);
    }

    void NuVec4MtxTransformVU0(NUVEC4 *out, NUVEC4 *in, NUMTX *matrix) {
        f32 x = in->x * matrix->m00 + in->y * matrix->m10 + in->z * matrix->m20 + in->w * matrix->m30;
        f32 y = in->x * matrix->m01 + in->y * matrix->m11 + in->z * matrix->m21 + in->w * matrix->m31;
        f32 z = in->x * matrix->m02 + in->y * matrix->m12 + in->z * matrix->m22 + in->w * matrix->m32;
        f32 w = in->x * matrix->m03 + in->y * matrix->m13 + in->z * matrix->m23 + in->w * matrix->m33;
        out->x = x;
        out->y = y;
        out->z = z;
        out->w = w;
    }

    void NuVec4MtxRotateVU0(NUVEC4 *out, NUVEC4 *v, NUMTX *m) {
        f32 x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20;
        f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21;
        f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22;
        out->x = x;
        out->y = y;
        out->z = z;
        out->w = v->w;
    }

    void NuCameraInitClipTestVU0() {
        STUBBED();
    }

    void NuMtxMulnVU0(NUMTX *out, NUMTX *left, NUMTX **right) {
        NuMtxMul(out, left, *right);
    }

    void NuVec4MtxTransformVU0x2(NUVEC4 *out, NUVEC4 *v, NUMTX *m) {
        for (i32 i = 0; i < 2; ++i) {
            f32 x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + v->w * m->m30;
            f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + v->w * m->m31;
            f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + v->w * m->m32;
            f32 w = v->x * m->m03 + v->y * m->m13 + v->z * m->m23 + v->w * m->m33;
            out->x = x;
            out->y = y;
            out->z = z;
            out->w = w;
            ++out;
            ++v;
        }
    }

    void NuVec4MtxTransformVU0x3(NUVEC4 *out, NUVEC4 *v, NUMTX *m) {
        for (i32 i = 0; i < 3; ++i) {
            f32 x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + v->w * m->m30;
            f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + v->w * m->m31;
            f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + v->w * m->m32;
            f32 w = v->x * m->m03 + v->y * m->m13 + v->z * m->m23 + v->w * m->m33;
            out->x = x;
            out->y = y;
            out->z = z;
            out->w = w;
            ++out;
            ++v;
        }
    }

    void NuVec4MtxTransformVU0x4(NUVEC4 *out, NUVEC4 *v, NUMTX *m) {
        for (i32 i = 0; i < 4; ++i) {
            f32 x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + v->w * m->m30;
            f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + v->w * m->m31;
            f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + v->w * m->m32;
            f32 w = v->x * m->m03 + v->y * m->m13 + v->z * m->m23 + v->w * m->m33;
            out->x = x;
            out->y = y;
            out->z = z;
            out->w = w;
            ++out;
            ++v;
        }
    }

    void NuMtxPreScaleUVU0(NUMTX *matrix, f32 scale) {
        matrix->m00 *= scale;
        matrix->m01 *= scale;
        matrix->m02 *= scale;
        matrix->m10 *= scale;
        matrix->m11 *= scale;
        matrix->m12 *= scale;
        matrix->m20 *= scale;
        matrix->m21 *= scale;
        matrix->m22 *= scale;
    }

    void NuMtxSetRotationXYVU0(NUMTX *matrix, NUANGVEC *angles) {
        f32 cx = NU_COS_LUT(angles->x);
        f32 sx = NU_SIN_LUT(angles->x);
        f32 cy = NU_COS_LUT(angles->y);
        f32 sy = NU_SIN_LUT(angles->y);
        f32 cz = NU_COS_LUT(0);
        f32 sz = NU_SIN_LUT(0);
        matrix->m00 = cy * cz;
        matrix->m01 = cy * sz;
        matrix->m02 = -sy;
        matrix->m03 = 0.0f;
        matrix->m10 = sx * sy * cz - cx * sz;
        matrix->m11 = sx * sy * sz + cx * cz;
        matrix->m12 = sx * cy;
        matrix->m13 = 0.0f;
        matrix->m20 = cx * sy * cz + sx * sz;
        matrix->m21 = cx * sy * sz - sx * cz;
        matrix->m22 = cx * cy;
        matrix->m23 = 0.0f;
        matrix->m30 = 0.0f;
        matrix->m31 = 0.0f;
        matrix->m32 = 0.0f;
        matrix->m33 = 1.0f;
    }

    void NuMtxInvVU0(NUMTX *out, NUMTX *in) {
        f32 x = -in->m30;
        f32 y = -in->m31;
        f32 z = -in->m32;
        f32 temp = in->m01;
        out->m01 = in->m10;
        out->m10 = temp;
        temp = in->m02;
        out->m02 = in->m20;
        out->m20 = temp;
        temp = in->m12;
        out->m12 = in->m21;
        out->m21 = temp;
        out->m00 = in->m00;
        out->m11 = in->m11;
        out->m22 = in->m22;
        out->m30 = out->m00 * x + out->m10 * y + out->m20 * z;
        out->m31 = out->m01 * x + out->m11 * y + out->m21 * z;
        out->m32 = out->m02 * x + out->m12 * y + out->m22 * z;
        out->m03 = out->m13 = out->m23 = 0.0f;
        out->m33 = 1.0f;
    }
}

void randyfloat() {
    STUBBED();
}

extern "C" {
    void NuVecInvMtxTransformVU0(NUVEC *out, NUVEC *v, NUMTX *m) {
        f32 x = v->x - m->m30;
        f32 y = v->y - m->m31;
        f32 z = v->z - m->m32;
        out->x = m->m00 * x + m->m01 * y + m->m02 * z;
        out->y = m->m10 * x + m->m11 * y + m->m12 * z;
        out->z = m->m20 * x + m->m21 * y + m->m22 * z;
    }
}

void makenuvec(float, float, float) {
    STUBBED();
}

void makenuvec4(float, float, float, float) {
    STUBBED();
}

u32 NuVecToRGBA(NUVEC *v, f32 alpha) {
    u32 red = (v->x + 1.0f) * 127.5f;
    u32 green = (v->y + 1.0f) * 127.5f;
    u32 blue = (v->z + 1.0f) * 127.5f;
    u32 opacity = alpha * 255.0f;
    return (opacity << 24) + (red << 16) + (green << 8) + blue;
}

extern "C" {
    f32 NuVecNormVU0(NUVEC *out, NUVEC *v) {
        f32 magnitude = NuFsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
        NUVEC normalized;
        if (magnitude != 0.0f) {
            normalized.x = v->x / magnitude;
            normalized.y = v->y / magnitude;
            normalized.z = v->z / magnitude;
        } else {
            normalized.x = 0.0f;
            normalized.y = 0.0f;
            normalized.z = 0.0f;
        }
        out->x = normalized.x;
        out->y = normalized.y;
        out->z = normalized.z;
        return magnitude;
    }

    void NuVec4MtxInvTransformVU0(NUVEC4 *out, NUVEC *v, NUMTX *matrix) {
        NUMTX inverse;
        NuMtxInv(&inverse, matrix);
        NuVec4MtxTransform(out, v, &inverse);
    }

    void NuVecMtxTransformVU0(NUVEC *out, NUVEC *input, NUMTX *matrix) {
        NuVecMtxTransform(out, input, matrix);
    }

    void NuVec4MtxInvRotVU0(NUVEC4 *out, NUVEC4 *v, NUMTX *m) {
        f32 y = v->x * m->m10 + v->y * m->m11 + v->z * m->m12;
        f32 z = v->x * m->m20 + v->y * m->m21 + v->z * m->m22;
        f32 w = v->x * m->m30 + v->y * m->m31 + v->z * m->m32;
        out->x = v->x * m->m00 + v->y * m->m01 + v->z * m->m02;
        out->y = y;
        out->z = z;
        out->w = w;
    }

    f32 NuVecDiffVU0(NUVEC *a, NUVEC *b) {
        f32 x = a->x - b->x;
        f32 y = a->y - b->y;
        f32 z = a->z - b->z;
        f32 squared = x * x + y * y + z * z;
        return NuFsqrt(squared);
    }

    f32 NuVecMagVU0(NUVEC *v) {
        f32 magnitude = NuFsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
        return magnitude;
    }

    f32 NuVecDiffSqrVU0(NUVEC *a, NUVEC *b) {
        f32 x = a->x - b->x;
        f32 y = a->y - b->y;
        f32 z = a->z - b->z;
        f32 squared = x * x + y * y + z * z;
        return squared;
    }
}

f32 NuASin_Accurate(f32 value) {
    if (value >= 0.0f) {
        if (value >= 1.0f)
            return 1.57079637f;
        return 1.57079637f -
               NuFsqrt(1.0f - value) * (1.57072878f - 0.212114394f * value + 0.0742610022f * value * value -
                                        0.0187292993f * value * value * value);
    }
    if (value <= -1.0f)
        return -1.57079637f;
    return NuFsqrt(value + 1.0f) * (1.57072878f + 0.212114394f * value + 0.0742610022f * value * value +
                                    0.0187292993f * value * value * value) -
           1.57079637f;
}

float NuSin_Accurate(float x) {
    STUBBED();
    (void)x;
}

void NuQuatLerp2(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t) {
    f32 dot = to->x * from->x + to->y * from->y + to->z * from->z + to->w * from->w;
    if (dot < 0.0f) {
        out->x = (to->x + from->x) * t - from->x;
        out->y = (to->y + from->y) * t - from->y;
        out->z = (to->z + from->z) * t - from->z;
        out->w = (to->w + from->w) * t - from->w;
    } else {
        out->x = (to->x - from->x) * t + from->x;
        out->y = (to->y - from->y) * t + from->y;
        out->z = (to->z - from->z) * t + from->z;
        out->w = (to->w - from->w) * t + from->w;
    }
}

void NuQuatSlerp_Accurate(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t) {
    f32 scale;
    f32 from_factor;
    f32 to_factor;
    f32 unused = 0.0f;
    f32 omega;
    f32 sin_omega;
    NUQUAT to_prime;
    scale = from->x * to->x + from->y * to->y + from->z * to->z + from->w * to->w;
    if (scale < 0.0f) {
        scale = -scale;
        to_prime.x = -to->x;
        to_prime.y = -to->y;
        to_prime.z = -to->z;
        to_prime.w = -to->w;
    } else {
        to_prime.x = to->x;
        to_prime.y = to->y;
        to_prime.z = to->z;
        to_prime.w = to->w;
    }
    if (1.0f - scale > 0.0f) {
        omega = 1.5707963705062866f - NuASin_Accurate(scale);
        sin_omega = NuSin_Accurate(omega);
        from_factor = NuSin_Accurate((1.0f - t) * omega) / sin_omega;
        to_factor = NuSin_Accurate(t * omega) / sin_omega;
    } else {
        from_factor = 1.0f - t;
        to_factor = t;
    }
    out->x = from->x * from_factor + to_prime.x * to_factor;
    out->y = from->y * from_factor + to_prime.y * to_factor;
    out->z = from->z * from_factor + to_prime.z * to_factor;
    out->w = from->w * from_factor + to_prime.w * to_factor;
}

extern "C" {
    void NuMtxMulVU0(NUMTX *result, NUMTX *left, NUMTX *right) {
        NuMtxMulH(result, left, right);
    }

    void NuMtxScaleVU0(NUMTX *matrix, NUVEC *scale) {
        NuMtxScale(matrix, scale);
    }

    void NuMtxPreScaleVU0(NUMTX *matrix, NUVEC *scale) {
        NuMtxPreScale(matrix, scale);
    }

    void NuMtxMulRVU0(NUMTX *result, NUMTX *left, NUMTX *right) {
        NuMtxMulR(result, left, right);
    }

    void NuMtxSetRotateXYZVU0(NUMTX *matrix, NUANGVEC *angles) {
        NuMtxSetRotateXYZ(matrix, angles);
    }
}
