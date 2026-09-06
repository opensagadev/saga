
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec4.h"

extern "C" {

    void NuFloatToHalf(void) {
    }
    void NuHalfVec4ToNuVec4(void) {
    }
    void NuVec4Lerp(void) {
    }
    void NuVec4Mag(void) {
    }
    void NuVec4MagSqr(void) {
    }
    void NuVec4Max(void) {
    }
    void NuVec4Min(void) {
    }
    void NuVec4MtxInvRotVU0(void) {
    }
    void NuVec4MtxInvTransformVU0(void) {
    }
    void NuVec4MtxRotateVU0(void) {
    }
    void NuVec4MtxTransformHVU0(void) {
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
    void NuVec4MtxTransformVU0x2(void) {
    }
    void NuVec4MtxTransformVU0x3(NUVEC4 *out, NUVEC4 *in, NUMTX *matrix) {
        for (i32 i = 0; i < 3; ++i, ++out, ++in)
            NuVec4MtxTransformVU0(out, in, matrix);
    }
    void NuVec4MtxTransformVU0x4(void) {
    }
    void NuVec4ScaleAccum(void) {
    }
    void NuVec4ScaleXYZVU0(void) {
    }
    void NuVec4Sub(void) {
    }
    void NuVec4ToNuHalfVec4(void) {
    }
    void NuVecConvertToIntVU0(void) {
    }
    void NuVecDiffSqrVU0(void) {
    }
    void NuVecDiffVU0(void) {
    }
    void NuVecInvMtxRotateValX(void) {
    }
    void NuVecInvMtxRotateValY(void) {
    }
    void NuVecInvMtxRotateValZ(void) {
    }
    void NuVecInvMtxScale(void) {
    }
    void NuVecInvMtxTransformVU0(void) {
    }
    void NuVecInvMtxTranslate(void) {
    }
    void NuVecMagVU0(void) {
    }
    void NuVecMtxRotateH(void) {
    }
    void NuVecMtxRotateValX(void) {
    }
    void NuVecMtxRotateValY(void) {
    }
    void NuVecMtxRotateValZ(void) {
    }
    void NuVecMtxTransformVU0(NUVEC *out, NUVEC *input, NUMTX *matrix) {
        NuVecMtxTransform(out, input, matrix);
    }
    void NuVecNormVU0(void) {
    }
    void NuVecRotateYValX(void) {
    }
}

struct nuvec_s;

void NuVecToRGBA(nuvec_s *, float) {
}
