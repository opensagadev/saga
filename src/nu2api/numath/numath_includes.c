// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

// The original unit constructs the homogeneous VuVec header constants.
#include "nu2api/nucore/nuvuvec.hpp"

#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include "decomp.h"
#include "nu2api/nucore/common.h"
#include <float.h>
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec.h"
#include <stddef.h>
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/numemory.h"
#include <string.h>
#include <limits.h>
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nuplane.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numath_includes.h"

static f32 fetol = 0.01f;

// The original helpers are local but have C linkage.
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

f32 NUTRIG_SC0 = -0.1666666716337204f;
f32 NUTRIG_SC1 = 0.008333333767950535f;
f32 NUTRIG_SC2 = -0.00019841270113829523f;
f32 NUTRIG_SC3 = 2.7557318844628753e-06f;

static f32 pow_x[32], pow_y[32], pow_rv[32];
static i32 pow_cache_free;

#define PI_OVER_4 0.785398f
#define NEG_1_OVER_6 -0.166667f
#define NEG_3_OVER_40 -0.075f
#define NEG_5_OVER_112 -0.0446429f
#define NEG_35_OVER_1152 -0.0303819f
#define MAX_SHORT_OVER_PI 10430.4f

#define POW2(x) ((x) * (x))
#define POW3(x) ((x) * POW2(x))
#define POW4(x) (POW2(x) * POW2(x))
#define POW5(x) (POW2(x) * POW3(x))

// nutrig.c's own arcsine; C linkage in the original, so it keeps its plain
// name next to the C++ inline in nutrig.h.
extern "C" {
    static i16 NuASinC(f32 sin) __asm__("NuASin");
}

NUVEC v000 = {0};
NUVEC nuvec_zero = {0};
NUVEC v100 = {1.0f, 0.0f, 0.0f};
NUVEC nuvec_x = {1.0f, 0.0f, 0.0f};
NUVEC v010 = {0.0f, 1.0f, 0.0f};
NUVEC nuvec_y = {0.0f, 1.0f, 0.0f};
NUVEC v001 = {0.0f, 0.0f, 1.0f};
NUVEC v111 = {1.0f, 1.0f, 1.0f};
NUVEC nuvec_one = {1.0f, 1.0f, 1.0f};
NUVEC nuvec_minus_one = {-1.0f, -1.0f, -1.0f};

NUVEC4 nuvec4_zero = {0};

NUMTX numtx_zero = {0};

// clang-format off
NUMTX numtx_identity = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};
// clang-format on

u32 fseed;

static NURAND global_rand = {1};

// The legacy wind-group routines have unoptimized code in the reference.
extern "C" {
    u32 NuWindQS = 0x1365;

}

static i32 curve_points;
static nuvec_s *curve;
static nuvec_s *control;

struct legacy_nugraph_s {
    u8 pad_00[0x38];
    i8 interpolation;
    i8 point_count;
    u8 pad_3a[2];
    f32 x[32];
    f32 y[32];
    f32 x_scale;
    f32 y_scale;
    f32 x_extent;
    f32 y_extent;
    f32 x_offset;
    f32 y_offset;
};
DECOMP_ASSERT(sizeof(legacy_nugraph_s) == 0x154, "Legacy graph ABI");

extern "C" i32 nugraphFileLoadGraphOnlyToFile(nugraph_s *graph, NUFILE file);
extern "C" i32 nugraphFileSaveGraphOnlyToFile(nugraph_s *graph, NUFILE file);

extern "C" {
i32 nugraphFileLoadGraphOnlyToFile(nugraph_s *graph, NUFILE file);

i32 nugraphFileSaveGraphOnlyToFile(nugraph_s *graph, NUFILE file);
} // extern "C"

#define ANG_COUNT 513
static u16 ang[ANG_COUNT] = {
    0x0000, 0x0014, 0x0029, 0x003d, 0x0051, 0x0066, 0x007a, 0x008f, 0x00a3, 0x00b7, 0x00cc, 0x00e0, 0x00f4, 0x0109,
    0x011d, 0x0131, 0x0146, 0x015a, 0x016f, 0x0183, 0x0197, 0x01ac, 0x01c0, 0x01d4, 0x01e9, 0x01fd, 0x0211, 0x0226,
    0x023a, 0x024e, 0x0262, 0x0277, 0x028b, 0x029f, 0x02b4, 0x02c8, 0x02dc, 0x02f0, 0x0305, 0x0319, 0x032d, 0x0341,
    0x0356, 0x036a, 0x037e, 0x0392, 0x03a7, 0x03bb, 0x03cf, 0x03e3, 0x03f7, 0x040c, 0x0420, 0x0434, 0x0448, 0x045c,
    0x0470, 0x0484, 0x0499, 0x04ad, 0x04c1, 0x04d5, 0x04e9, 0x04fd, 0x0511, 0x0525, 0x0539, 0x054d, 0x0561, 0x0575,
    0x0589, 0x059d, 0x05b1, 0x05c5, 0x05d9, 0x05ed, 0x0601, 0x0615, 0x0629, 0x063d, 0x0651, 0x0665, 0x0678, 0x068c,
    0x06a0, 0x06b4, 0x06c8, 0x06dc, 0x06ef, 0x0703, 0x0717, 0x072b, 0x073e, 0x0752, 0x0766, 0x077a, 0x078d, 0x07a1,
    0x07b5, 0x07c8, 0x07dc, 0x07ef, 0x0803, 0x0817, 0x082a, 0x083e, 0x0851, 0x0865, 0x0878, 0x088c, 0x089f, 0x08b3,
    0x08c6, 0x08da, 0x08ed, 0x0901, 0x0914, 0x0927, 0x093b, 0x094e, 0x0961, 0x0975, 0x0988, 0x099b, 0x09ae, 0x09c2,
    0x09d5, 0x09e8, 0x09fb, 0x0a0e, 0x0a22, 0x0a35, 0x0a48, 0x0a5b, 0x0a6e, 0x0a81, 0x0a94, 0x0aa7, 0x0aba, 0x0acd,
    0x0ae0, 0x0af3, 0x0b06, 0x0b19, 0x0b2c, 0x0b3f, 0x0b51, 0x0b64, 0x0b77, 0x0b8a, 0x0b9d, 0x0baf, 0x0bc2, 0x0bd5,
    0x0be7, 0x0bfa, 0x0c0d, 0x0c1f, 0x0c32, 0x0c45, 0x0c57, 0x0c6a, 0x0c7c, 0x0c8f, 0x0ca1, 0x0cb4, 0x0cc6, 0x0cd9,
    0x0ceb, 0x0cfd, 0x0d10, 0x0d22, 0x0d34, 0x0d47, 0x0d59, 0x0d6b, 0x0d7d, 0x0d90, 0x0da2, 0x0db4, 0x0dc6, 0x0dd8,
    0x0dea, 0x0dfc, 0x0e0f, 0x0e21, 0x0e33, 0x0e45, 0x0e56, 0x0e68, 0x0e7a, 0x0e8c, 0x0e9e, 0x0eb0, 0x0ec2, 0x0ed4,
    0x0ee5, 0x0ef7, 0x0f09, 0x0f1b, 0x0f2c, 0x0f3e, 0x0f50, 0x0f61, 0x0f73, 0x0f84, 0x0f96, 0x0fa7, 0x0fb9, 0x0fca,
    0x0fdc, 0x0fed, 0x0fff, 0x1010, 0x1021, 0x1033, 0x1044, 0x1055, 0x1067, 0x1078, 0x1089, 0x109a, 0x10ab, 0x10bc,
    0x10ce, 0x10df, 0x10f0, 0x1101, 0x1112, 0x1123, 0x1134, 0x1145, 0x1156, 0x1166, 0x1177, 0x1188, 0x1199, 0x11aa,
    0x11bb, 0x11cb, 0x11dc, 0x11ed, 0x11fd, 0x120e, 0x121f, 0x122f, 0x1240, 0x1250, 0x1261, 0x1271, 0x1282, 0x1292,
    0x12a3, 0x12b3, 0x12c3, 0x12d4, 0x12e4, 0x12f4, 0x1305, 0x1315, 0x1325, 0x1335, 0x1345, 0x1355, 0x1366, 0x1376,
    0x1386, 0x1396, 0x13a6, 0x13b6, 0x13c6, 0x13d6, 0x13e6, 0x13f5, 0x1405, 0x1415, 0x1425, 0x1435, 0x1444, 0x1454,
    0x1464, 0x1473, 0x1483, 0x1493, 0x14a2, 0x14b2, 0x14c1, 0x14d1, 0x14e0, 0x14f0, 0x14ff, 0x150f, 0x151e, 0x152d,
    0x153d, 0x154c, 0x155b, 0x156b, 0x157a, 0x1589, 0x1598, 0x15a7, 0x15b7, 0x15c6, 0x15d5, 0x15e4, 0x15f3, 0x1602,
    0x1611, 0x1620, 0x162f, 0x163e, 0x164c, 0x165b, 0x166a, 0x1679, 0x1688, 0x1696, 0x16a5, 0x16b4, 0x16c2, 0x16d1,
    0x16e0, 0x16ee, 0x16fd, 0x170b, 0x171a, 0x1728, 0x1737, 0x1745, 0x1754, 0x1762, 0x1770, 0x177f, 0x178d, 0x179b,
    0x17aa, 0x17b8, 0x17c6, 0x17d4, 0x17e2, 0x17f0, 0x17fe, 0x180d, 0x181b, 0x1829, 0x1837, 0x1845, 0x1853, 0x1860,
    0x186e, 0x187c, 0x188a, 0x1898, 0x18a6, 0x18b3, 0x18c1, 0x18cf, 0x18dd, 0x18ea, 0x18f8, 0x1906, 0x1913, 0x1921,
    0x192e, 0x193c, 0x1949, 0x1957, 0x1964, 0x1972, 0x197f, 0x198c, 0x199a, 0x19a7, 0x19b4, 0x19c2, 0x19cf, 0x19dc,
    0x19e9, 0x19f6, 0x1a04, 0x1a11, 0x1a1e, 0x1a2b, 0x1a38, 0x1a45, 0x1a52, 0x1a5f, 0x1a6c, 0x1a79, 0x1a86, 0x1a93,
    0x1a9f, 0x1aac, 0x1ab9, 0x1ac6, 0x1ad3, 0x1adf, 0x1aec, 0x1af9, 0x1b05, 0x1b12, 0x1b1f, 0x1b2b, 0x1b38, 0x1b44,
    0x1b51, 0x1b5d, 0x1b6a, 0x1b76, 0x1b83, 0x1b8f, 0x1b9c, 0x1ba8, 0x1bb4, 0x1bc1, 0x1bcd, 0x1bd9, 0x1be5, 0x1bf2,
    0x1bfe, 0x1c0a, 0x1c16, 0x1c22, 0x1c2e, 0x1c3a, 0x1c46, 0x1c52, 0x1c5e, 0x1c6a, 0x1c76, 0x1c82, 0x1c8e, 0x1c9a,
    0x1ca6, 0x1cb2, 0x1cbe, 0x1cc9, 0x1cd5, 0x1ce1, 0x1ced, 0x1cf8, 0x1d04, 0x1d10, 0x1d1b, 0x1d27, 0x1d33, 0x1d3e,
    0x1d4a, 0x1d55, 0x1d61, 0x1d6c, 0x1d78, 0x1d83, 0x1d8e, 0x1d9a, 0x1da5, 0x1db1, 0x1dbc, 0x1dc7, 0x1dd3, 0x1dde,
    0x1de9, 0x1df4, 0x1dff, 0x1e0b, 0x1e16, 0x1e21, 0x1e2c, 0x1e37, 0x1e42, 0x1e4d, 0x1e58, 0x1e63, 0x1e6e, 0x1e79,
    0x1e84, 0x1e8f, 0x1e9a, 0x1ea5, 0x1eb0, 0x1eba, 0x1ec5, 0x1ed0, 0x1edb, 0x1ee6, 0x1ef0, 0x1efb, 0x1f06, 0x1f10,
    0x1f1b, 0x1f26, 0x1f30, 0x1f3b, 0x1f45, 0x1f50, 0x1f5a, 0x1f65, 0x1f6f, 0x1f7a, 0x1f84, 0x1f8f, 0x1f99, 0x1fa4,
    0x1fae, 0x1fb8, 0x1fc3, 0x1fcd, 0x1fd7, 0x1fe1, 0x1fec, 0x1ff6, 0x2000};

#define NU_ATAN2_LUT(y, x) ang[(y << 9) / x]
#define NU_ATAN2F_LUT(y, x) ang[(i32)((y * 512.0f) / x)]

extern "C" {
    static f32 NuFsel(f32 a, f32 b, f32 c) {
        return a >= 0.0f ? b : c;
    }

    static f32 NuTrunc(f32 a) {
        return (f32)(i32)a;
    }

}

extern "C" {
static i16 NuASinC(f32 sin) {
    f32 abs;
    f32 sqrt;
    f32 unknown_a;
    f32 unknown_b;
    f32 unknown_c;
    f32 unknown_d;

    abs = NuFabs(sin);
    sqrt = NuFsqrt(1.0f - sin * sin);

    unknown_a = MIN(sqrt, abs);

    unknown_b = MAX(MIN((abs - 0.70710677f) * 3.40282e+38f, 1.0f), -1.0f);

    unknown_c = MIN(sin * 3.40282e+38f, 1.0f);
    unknown_c = MAX(unknown_c, -1.0f);

    unknown_d = unknown_b * unknown_c + unknown_c;

    return (NEG_3_OVER_40 * POW2(unknown_b * unknown_c * unknown_a) * POW3(unknown_b * unknown_c * unknown_a) +
            (unknown_d * PI_OVER_4 - (unknown_b * unknown_c * unknown_a) +
             NEG_1_OVER_6 * (unknown_b * unknown_c * unknown_a) * POW2(unknown_b * unknown_c * unknown_a)) +
            NEG_5_OVER_112 * POW3(unknown_b * unknown_c * unknown_a) * POW4(unknown_b * unknown_c * unknown_a) +
            NEG_35_OVER_1152 * POW4(unknown_b * unknown_c * unknown_a) * POW5(unknown_b * unknown_c * unknown_a)) *
           MAX_SHORT_OVER_PI;
}
}

i16 NuACos(f32 cos) {
    return 0x4000 - NuASinC(cos);
}

extern "C" {
static void VuQuatBlend(NUQUAT *out, NUQUAT *a, NUQUAT *b, f32 t, f32 w) {
    out->x = a->x * t + b->x * w;
    out->y = a->y * t + b->y * w;
    out->z = a->z * t + b->z * w;
    out->w = a->w * t + b->w * w;
}
}

extern "C" {
static void VuQuatLerp(NUQUAT *out, NUQUAT *a, NUQUAT *b, f32 t) {
    f32 w = 1.0f - t;
    out->x = a->x * w + b->x * t;
    out->y = a->y * w + b->y * t;
    out->z = a->z * w + b->z * t;
    out->w = a->w * w + b->w * t;
}
}

extern "C" {
static void VuQuatCopy(NUQUAT *dst, NUQUAT *src) {
    *dst = *src;
}
}

extern "C" {
static void VuQuatNeg2(NUQUAT *out, NUQUAT *in) {
    out->x = -in->x;
    out->y = -in->y;
    out->z = -in->z;
    out->w = -in->w;
}
}

extern "C" {
static f32 VuQuatDot(NUQUAT *a, NUQUAT *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}
}

extern "C" {
static void VuQuatNormalise(NUQUAT *out, NUQUAT *in) {
    f32 magnitude = in->w * in->w + in->x * in->x + in->y * in->y + in->z * in->z;
    f32 scale = NuFdiv(1.0f, NuFsqrt(magnitude));
    out->w = in->w * scale;
    out->x = in->x * scale;
    out->y = in->y * scale;
    out->z = in->z * scale;
}
}

void NuVecMtxTransform(NUVEC *out, NUVEC *v, NUMTX *m) {
    f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + m->m31;
    f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + m->m32;

    out->x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + m->m30;
    out->y = y;
    out->z = z;
}

f32 NuVecMtxTransformH(NUVEC *out, NUVEC *v, NUMTX *m) {
    f32 reciprocal_w = NuFdiv(1.0f, v->x * m->m03 + v->y * m->m13 + v->z * m->m23 + m->m33);
    f32 y = (v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + m->m31) * reciprocal_w;
    f32 z = (v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + m->m32) * reciprocal_w;
    out->x = (v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + m->m30) * reciprocal_w;
    out->y = y;
    out->z = z;
    return reciprocal_w;
}

void NuVecMtxTranslate(NUVEC *out, NUVEC *v, NUMTX *m) {
    out->x = v->x + m->m30;
    out->y = v->y + m->m31;
    out->z = v->z + m->m32;
}

void NuVecMtxRotate(NUVEC *out, NUVEC *v, NUMTX *m) {
    f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21;
    f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22;

    out->x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20;
    out->y = y;
    out->z = z;
}

extern "C" {
    void NuVecMtxRotateValX(NUVEC *out, f32 value, NUMTX *matrix) {
        out->x = matrix->m00 * value;
        out->y = matrix->m01 * value;
        out->z = matrix->m02 * value;
    }

    void NuVecMtxRotateValY(NUVEC *out, f32 value, NUMTX *matrix) {
        out->x = matrix->m10 * value;
        out->y = matrix->m11 * value;
        out->z = matrix->m12 * value;
    }

    void NuVecMtxRotateValZ(NUVEC *out, f32 value, NUMTX *matrix) {
        out->x = matrix->m20 * value;
        out->y = matrix->m21 * value;
        out->z = matrix->m22 * value;
    }

    void NuVecMtxRotateH(NUVEC *out, NUVEC *v, NUMTX *m) {
        f32 reciprocal_w = 1.0f / (v->x * m->m03 + v->y * m->m13 + v->z * m->m23);
        f32 y = (v->x * m->m01 + v->y * m->m11 + v->z * m->m21) * reciprocal_w;
        f32 z = (v->x * m->m02 + v->y * m->m12 + v->z * m->m22) * reciprocal_w;
        out->x = (v->x * m->m00 + v->y * m->m10 + v->z * m->m20) * reciprocal_w;
        out->y = y;
        out->z = z;
    }

}

void NuVecMtxScale(NUVEC *out, NUVEC *v, NUMTX *m) {
    out->x = v->x * m->m00;
    out->y = v->y * m->m11;
    out->z = v->z * m->m22;
}

void NuVecInvMtxTransform(NUVEC *out, NUVEC *v, NUMTX *m) {
    f32 x = v->x - m->m30;
    f32 y = v->y - m->m31;
    f32 z = v->z - m->m32;

    out->x = x * m->m00 + y * m->m01 + z * m->m02;
    out->y = x * m->m10 + y * m->m11 + z * m->m12;
    out->z = x * m->m20 + y * m->m21 + z * m->m22;
}

extern "C" {
    void NuVecInvMtxTranslate(NUVEC *out, NUVEC *v, NUMTX *matrix) {
        out->x = v->x - matrix->m30;
        out->y = v->y - matrix->m31;
        out->z = v->z - matrix->m32;
    }

}

// Original: 296 bytes. Preserve the input when out == v.
void NuVecInvMtxRotate(NUVEC *out, NUVEC *v, NUMTX *m) {
    f32 y = v->x * m->m10 + v->y * m->m11 + v->z * m->m12;
    f32 z = v->x * m->m20 + v->y * m->m21 + v->z * m->m22;
    out->x = v->x * m->m00 + v->y * m->m01 + v->z * m->m02;
    out->y = y;
    out->z = z;
}

extern "C" {
    // The reference ignores the scalar argument and rereads the destination
    // axis after each store. Preserve that sequential behavior.
    void NuVecInvMtxRotateValX(NUVEC *out, f32, NUMTX *matrix) {
        out->x = out->x * matrix->m00;
        out->y = out->x * matrix->m10;
        out->z = out->x * matrix->m20;
    }

    void NuVecInvMtxRotateValY(NUVEC *out, f32, NUMTX *matrix) {
        out->x = out->y * matrix->m01;
        out->y = out->y * matrix->m11;
        out->z = out->y * matrix->m21;
    }

    void NuVecInvMtxRotateValZ(NUVEC *out, f32, NUMTX *matrix) {
        out->x = out->z * matrix->m02;
        out->y = out->z * matrix->m12;
        out->z = out->z * matrix->m22;
    }

    void NuVecInvMtxScale(NUVEC *out, NUVEC *v, NUMTX *matrix) {
        out->x = v->x / matrix->m00;
        out->y = v->y / matrix->m11;
        out->z = v->z / matrix->m22;
    }

}

void NuVecRotateX(NUVEC *v, NUVEC *v0, NUANG a) {
    f32 fVar1 = NU_COS_LUT(a);
    f32 fVar2 = NU_SIN_LUT(a);
    f32 fVar3 = v0->y;

    v->x = v0->x;
    v->y = fVar3 * fVar1 - v0->z * fVar2;
    v->z = fVar3 * fVar2 + v0->z * fVar1;
}

void NuVecRotateY(NUVEC *v, NUVEC *v0, NUANG a) {
    f32 fVar1 = NU_COS_LUT(a);
    f32 fVar2 = NU_SIN_LUT(a);
    f32 fVar3 = v0->x;

    v->x = fVar3 * fVar1 + v0->z * fVar2;
    v->y = v0->y;
    v->z = v0->z * fVar1 - fVar3 * fVar2;
}

void NuVecRotateZ(NUVEC *v, NUVEC *v0, NUANG a) {
    f32 fVar1 = NU_COS_LUT(a);
    f32 fVar2 = NU_SIN_LUT(a);
    f32 fVar3 = v0->x;

    v->x = fVar3 * fVar1 - v0->y * fVar2;
    v->y = fVar3 * fVar2 + v0->y * fVar1;
    v->z = v0->z;
}

void NuVecRotateYValZ(NUVEC *v, f32 magnitude, NUANG a) {
    f32 fVar1 = NU_COS_LUT(a);
    f32 fVar2 = NU_SIN_LUT(a);

    v->x = magnitude * fVar2;
    v->y = 0.0f;
    v->z = magnitude * fVar1;
}

extern "C" {
    void NuVecRotateYValX(NUVEC *out, f32 value, NUANG angle) {
        f32 cosine = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
        f32 sine = NuTrigTable[(angle >> 1) & 0x7fff];
        out->x = value * cosine;
        out->y = 0.0f;
        out->z = -value * sine;
    }

}

void NuVecNeg(NUVEC *v, NUVEC *v0) {
    v->x = -v0->x;
    v->y = -v0->y;
    v->z = -v0->z;
}

void NuVecAdd(NUVEC *v, NUVEC *v0, NUVEC *v1) {
    v->x = v0->x + v1->x;
    v->y = v0->y + v1->y;
    v->z = v0->z + v1->z;
}

void NuVecSub(NUVEC *v, NUVEC *v0, NUVEC *v1) {
    v->x = v0->x - v1->x;
    v->y = v0->y - v1->y;
    v->z = v0->z - v1->z;
}

void NuVecScale(NUVEC *v, NUVEC *v0, f32 k) {
    v->x = v0->x * k;
    v->y = v0->y * k;
    v->z = v0->z * k;
}

void NuVecAddScale(NUVEC *v, NUVEC *v0, NUVEC *v1, f32 k) {
    v->x = v0->x + v1->x * k;
    v->y = v0->y + v1->y * k;
    v->z = v0->z + v1->z * k;
}

void NuVecScaleAccum(NUVEC *v, NUVEC *v0, f32 k) {
    v->x += v0->x * k;
    v->y += v0->y * k;
    v->z += v0->z * k;
}

void NuVecInvScale(NUVEC *v, NUVEC *v0, f32 k) {
    f32 ki;

    if (k == 0.0f) {
        ki = 0.0f;
    } else {
        ki = 1.0f / k;
    }

    v->x = v0->x * ki;
    v->y = v0->y * ki;
    v->z = v0->z * ki;
}

void NuVecCross(NUVEC *v, NUVEC *v0, NUVEC *v1) {
    f32 y, z;
    y = v0->z * v1->x - v0->x * v1->z;
    z = v0->x * v1->y - v0->y * v1->x;
    v->x = v0->y * v1->z - v0->z * v1->y;

    v->y = y;
    v->z = z;
}

void NuVecCrossRel(NUVEC *v, NUVEC *basepnt, NUVEC *v0, NUVEC *v1) {
    f32 y, z;
    y = (v0->z - basepnt->z) * (v1->x - basepnt->x) - (v0->x - basepnt->x) * (v1->z - basepnt->z);
    z = (v0->x - basepnt->x) * (v1->y - basepnt->y) - (v0->y - basepnt->y) * (v1->x - basepnt->x);
    v->x = (v0->y - basepnt->y) * (v1->z - basepnt->z) - (v0->z - basepnt->z) * (v1->y - basepnt->y);
    v->y = y;
    v->z = z;
}

f32 NuVecDot(NUVEC *v0, NUVEC *v1) {
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

f32 NuVecMag(NUVEC *v0) {
    return NuFsqrt(NuVecMagSqr(v0));
}

f32 NuVecMagXZ(NUVEC *v0) {
    return NuFsqrt(v0->x * v0->x + v0->z * v0->z);
}

void NuVecMax(NUVEC *v, NUVEC *v0, NUVEC *v1) {
    if (v0->x > v1->x) {
        v->x = v0->x;
    } else {
        v->x = v1->x;
    }

    if (v0->y > v1->y) {
        v->y = v0->y;
    } else {
        v->y = v1->y;
    }

    if (v0->z > v1->z) {
        v->z = v0->z;
    } else {
        v->z = v1->z;
    }
}

void NuVecMin(NUVEC *v, NUVEC *v0, NUVEC *v1) {
    if (v0->x < v1->x) {
        v->x = v0->x;
    } else {
        v->x = v1->x;
    }

    if (v0->y < v1->y) {
        v->y = v0->y;
    } else {
        v->y = v1->y;
    }

    if (v0->z < v1->z) {
        v->z = v0->z;
    } else {
        v->z = v1->z;
    }
}

f32 NuVecMagSqr(NUVEC *v0) {
    return v0->x * v0->x + v0->y * v0->y + v0->z * v0->z;
}

f32 NuVecNorm(NUVEC *v, NUVEC *v0) {
    f32 len = NuFsqrt(NuVecMagSqr(v0));
    f32 leni;

    if (len > 0.0f) {
        leni = 1.0f / len;
    } else {
        leni = 0.0f;
    }

    v->x = v0->x * leni;
    v->y = v0->y * leni;
    v->z = v0->z * leni;

    return len;
}

void NuVecSurfaceNormal(NUVEC *v, NUVEC *v0, NUVEC *v1, NUVEC *v2) {
    NUVEC vecA, vecB;

    NuVecSub(&vecA, v0, v1);
    NuVecSub(&vecB, v0, v2);
    NuVecCross(v, &vecB, &vecA);
    NuVecNorm(v, v);
}

f32 NuVecDist(NUVEC *v0, NUVEC *v1, NUVEC *d) {
    NUVEC dist;

    if (d != NULL) {
        NuVecSub(d, v0, v1);
        return NuVecMag(d);
    } else {
        NuVecSub(&dist, v0, v1);
        return NuVecMag(&dist);
    }
}

f32 NuVecDistSqr(NUVEC *v0, NUVEC *v1, NUVEC *d) {
    NUVEC dist;

    if (d != NULL) {
        NuVecSub(d, v0, v1);
        return NuVecMagSqr(d);
    } else {
        NuVecSub(&dist, v0, v1);
        return NuVecMagSqr(&dist);
    }
}

f32 NuVecXZDist(NUVEC *v0, NUVEC *v1, NUVEC *d) {
    NUVEC dist;

    if (d != NULL) {
        d->x = v0->x - v1->x;
        d->y = 0.0f;
        d->z = v0->z - v1->z;
        return NuVecMag(d);
    } else {
        dist.x = v0->x - v1->x;
        dist.y = 0.0f;
        dist.z = v0->z - v1->z;
        return NuVecMag(&dist);
    }
}

f32 NuVecXZDistSqr(NUVEC *v0, NUVEC *v1, NUVEC *d) {
    NUVEC dist;

    if (d != NULL) {
        d->x = v0->x - v1->x;
        d->y = 0.0f;
        d->z = v0->z - v1->z;
        return NuVecMagSqr(d);
    } else {
        dist.x = v0->x - v1->x;
        dist.y = 0.0f;
        dist.z = v0->z - v1->z;
        return NuVecMagSqr(&dist);
    }
}

void NuVecLerp(NUVEC *vt, NUVEC *v1, NUVEC *v0, f32 t) {
    vt->x = ((v1->x - v0->x) * t) + v0->x;
    vt->y = ((v1->y - v0->y) * t) + v0->y;
    vt->z = ((v1->z - v0->z) * t) + v0->z;
}

i32 NuLineLineIntersect(NUVEC *pnt0, NUVEC *v0, NUVEC *pnt1, NUVEC *v1, f32 *s, f32 *t) {
    NUVEC cross;
    f32 len;
    f32 inv_len;
    NUMTX m;

    NuVecCross(&cross, v0, v1);
    len = NuVecMag(&cross);

    if (len == 0.0f) {
        inv_len = 0.0f;
    } else {
        inv_len = 1.0f / len;
    }

    inv_len *= inv_len;

    m.m00 = pnt1->x - pnt0->x;
    m.m01 = pnt1->y - pnt0->y;
    m.m02 = pnt1->z - pnt0->z;

    m.m10 = v1->x;
    m.m11 = v1->y;
    m.m12 = v1->z;

    m.m20 = cross.x;
    m.m21 = cross.y;
    m.m22 = cross.z;

    *s = NuMtxDet3(&m) * inv_len;

    m.m10 = v0->x;
    m.m11 = v0->y;
    m.m12 = v0->z;

    *t = NuMtxDet3(&m) * inv_len;

    if (inv_len != 0.0f) {
        return 1;
    }

    return 0;
}

// Original: 1,446 bytes. The strict face boundaries and start-point outcode
// are intentional: these are the original clipping rules.
i32 BoundingBoxToLine(NUVEC *minimum, NUVEC *maximum, NUMTX *matrix, NUVEC *start, NUVEC *end, f32 expansion,
                      NUVEC *intersection) {
    NUVEC a, b, direction, hit;
    NUVEC lo = {minimum->x - expansion, minimum->y - expansion, minimum->z - expansion};
    NUVEC hi = {maximum->x + expansion, maximum->y + expansion, maximum->z + expansion};
    NuVecInvMtxTransform(&a, start, matrix);
    i32 ca = NuPointRelToBoundingBox(&a, &hi, &lo);
    if (ca == 0 && intersection == NULL)
        return 1;
    NuVecInvMtxTransform(&b, end, matrix);
    i32 cb = NuPointRelToBoundingBox(&b, &hi, &lo);
    if (cb == 0 && intersection == NULL)
        return 1;
    if (ca == 0 && cb == 0) {
        if (intersection != NULL) {
            intersection->x = 0.0f;
            intersection->y = 0.0f;
            intersection->z = 0.0f;
        }
        return 1;
    }
    if (ca & cb)
        return 0;
    NuVecSub(&direction, &b, &a);
    NuVecNorm(&direction, &direction);
    if (ca & 1) {
        NuClipXPlane(&hit, &a, &direction, &hi.x);
        if (hit.y < hi.y && hit.y > lo.y && hit.z < hi.z && hit.z > lo.z) {
            if (intersection != NULL)
                *intersection = hit;
            return 1;
        }
    } else if (ca & 8) {
        NuClipXPlane(&hit, &a, &direction, &lo.x);
        if (hit.y < hi.y && hit.y > lo.y && hit.z < hi.z && hit.z > lo.z) {
            if (intersection != NULL)
                *intersection = hit;
            return 1;
        }
    }
    if (ca & 2) {
        NuClipYPlane(&hit, &a, &direction, &hi.y);
        if (hit.x < hi.x && hit.x > lo.x && hit.z < hi.z && hit.z > lo.z) {
            if (intersection != NULL)
                *intersection = hit;
            return 1;
        }
    } else if (ca & 16) {
        NuClipYPlane(&hit, &a, &direction, &lo.y);
        if (hit.x < hi.x && hit.x > lo.x && hit.z < hi.z && hit.z > lo.z) {
            if (intersection != NULL)
                *intersection = hit;
            return 1;
        }
    }
    if (ca & 4) {
        NuClipZPlane(&hit, &a, &direction, &hi.z);
        if (hit.x < hi.x && hit.x > lo.x && hit.y < hi.y && hit.y > lo.y) {
            if (intersection != NULL)
                *intersection = hit;
            return 1;
        }
    } else if (ca & 32) {
        NuClipZPlane(&hit, &a, &direction, &lo.z);
        if (hit.x < hi.x && hit.x > lo.x && hit.y < hi.y && hit.y > lo.y) {
            if (intersection != NULL)
                *intersection = hit;
            return 1;
        }
    }
    return 0;
}

i32 NuVecCompareTolerance(NUVEC *a, NUVEC *b, f32 tolerance) {
    NUVEC diff;
    NuVecSub(&diff, a, b);

    f32 result = NuVecMag(&diff);
    if (result > tolerance) {
        return 0;
    } else {
        return 1;
    }
}

void NuVec4Add(NUVEC4 *v, NUVEC4 *v0, NUVEC4 *v1) {
    v->w = v0->w + v1->w;
    v->x = v0->x + v1->x;
    v->y = v0->y + v1->y;
    v->z = v0->z + v1->z;
}

f32 NuVec4Dot(NUVEC4 *v0, NUVEC4 *v1) {
    return v0->w * v1->w + v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

void NuVec4Scale(NUVEC4 *v, NUVEC4 *v0, f32 k) {
    v->x = v0->x * k;
    v->y = v0->y * k;
    v->z = v0->z * k;
    v->w = v0->w * k;
}

extern "C" {
    void NuVec4ScaleAccum(NUVEC4 *out, NUVEC4 *v, f32 scale) {
        out->x += v->x * scale;
        out->y += v->y * scale;
        out->z += v->z * scale;
        out->w += v->w * scale;
    }

    void NuVec4Sub(NUVEC4 *out, NUVEC4 *a, NUVEC4 *b) {
        out->x = a->x - b->x;
        out->y = a->y - b->y;
        out->z = a->z - b->z;
        out->w = a->w - b->w;
    }

}

void NuVec4MtxTransform(NUVEC4 *v, NUVEC *v0, NUMTX *m0) {
    f32 y = v0->x * m0->m01 + v0->y * m0->m11 + v0->z * m0->m21 + m0->m31;
    f32 z = v0->x * m0->m02 + v0->y * m0->m12 + v0->z * m0->m22 + m0->m32;
    f32 w = v0->x * m0->m03 + v0->y * m0->m13 + v0->z * m0->m23 + m0->m33;
    v->x = v0->x * m0->m00 + v0->y * m0->m10 + v0->z * m0->m20 + m0->m30;
    v->y = y;
    v->z = z;
    v->w = w;
}

void NuVec4MtxTransformH(NUVEC4 *v, NUVEC4 *v0, NUMTX *m0) {
    f32 x = v0->x * m0->m00 + v0->y * m0->m10 + v0->z * m0->m20 + v0->w * m0->m30;
    f32 y = v0->x * m0->m01 + v0->y * m0->m11 + v0->z * m0->m21 + v0->w * m0->m31;
    f32 z = v0->x * m0->m02 + v0->y * m0->m12 + v0->z * m0->m22 + v0->w * m0->m32;
    f32 w = v0->x * m0->m03 + v0->y * m0->m13 + v0->z * m0->m23 + v0->w * m0->m33;

    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
}

extern "C" {
    void NuVec4Max(NUVEC4 *out, NUVEC4 *a, NUVEC4 *b) {
        if (a->x > b->x)
            out->x = a->x;
        else
            out->x = b->x;
        if (a->y > b->y)
            out->y = a->y;
        else
            out->y = b->y;
        if (a->z > b->z)
            out->z = a->z;
        else
            out->z = b->z;
        if (a->w > b->w)
            out->w = a->w;
        else
            out->w = b->w;
    }

    void NuVec4Min(NUVEC4 *out, NUVEC4 *a, NUVEC4 *b) {
        if (a->x < b->x)
            out->x = a->x;
        else
            out->x = b->x;
        if (a->y < b->y)
            out->y = a->y;
        else
            out->y = b->y;
        if (a->z < b->z)
            out->z = a->z;
        else
            out->z = b->z;
        if (a->w < b->w)
            out->w = a->w;
        else
            out->w = b->w;
    }

    f32 NuVec4Mag(NUVEC4 *v) {
        return NuFsqrt(v->w * v->w + v->x * v->x + v->y * v->y + v->z * v->z);
    }

    f32 NuVec4MagSqr(NUVEC4 *v) {
        return v->w * v->w + v->x * v->x + v->y * v->y + v->z * v->z;
    }

    void NuVec4Lerp(NUVEC4 *out, NUVEC4 *a, NUVEC4 *b, f32 t) {
        out->x = b->x + (a->x - b->x) * t;
        out->y = b->y + (a->y - b->y) * t;
        out->z = b->z + (a->z - b->z) * t;
        out->w = b->w + (a->w - b->w) * t;
    }

    i16 NuFloatToHalf(f32 value) {
        i32 exponent;
        u32 bits = *reinterpret_cast<const u32 *>(&value);
        i32 sign = bits >> 31;
        exponent = (bits >> 23) & 0xff;
        i32 mantissa = bits & 0x7fffff;
        // The original clamps the exponent and truncates the mantissa;
        // it does not implement IEEE half rounding or denormal handling.
        exponent -= 112;
        if (exponent < 0)
            exponent = 0;
        if (exponent > 31)
            exponent = 31;
        i16 result = (sign << 15) | ((exponent & 31) << 10) | (mantissa >> 13);
        return *reinterpret_cast<const u16 *>(&result);
    }

    f32 NuHalfToFloat(i16 value) {
        u16 bits = value;
        if (bits == 0)
            return 0.0f;
        i32 sign = bits >> 15;
        i32 exponent = (bits >> 10) & 31;
        i32 mantissa = bits & 0x3ff;
        exponent += 112;
        union {
            u32 bits;
            f32 value;
        } result;
        result.bits = (u32(sign) << 31) | (exponent << 23) | (mantissa << 13);
        return result.value;
    }

    void NuVec4ToNuHalfVec4(NUVEC4 *v, NUHALFVEC4 *out) {
        out->x = NuFloatToHalf(v->x);
        out->y = NuFloatToHalf(v->y);
        out->z = NuFloatToHalf(v->z);
        out->w = NuFloatToHalf(v->w);
    }

    void NuHalfVec4ToNuVec4(NUHALFVEC4 *v, NUVEC4 *out) {
        out->x = NuHalfToFloat(v->x);
        out->y = NuHalfToFloat(v->y);
        out->z = NuHalfToFloat(v->z);
        out->w = NuHalfToFloat(v->w);
    }

}

void NuMtxSetZero(NUMTX *m) {
    *m = numtx_zero;
}

void NuMtxSetIdentity(NUMTX *m) {
    *m = numtx_identity;
}

void NuMtxSetTranslation(NUMTX *m, NUVEC *t) {
    m->m30 = t->x;
    m->m31 = t->y;
    m->m32 = t->z;
    m->m01 = m->m02 = m->m03 = m->m10 = m->m12 = m->m13 = m->m20 = m->m21 = m->m23 = 0.0f;
    m->m00 = m->m11 = m->m22 = m->m33 = 1.0f;
}

void NuMtxSetTranslationNeg(NUMTX *m, NUVEC *t) {
    m->m30 = -t->x;
    m->m31 = -t->y;
    m->m32 = -t->z;
    m->m01 = m->m02 = m->m03 = m->m10 = m->m12 = m->m13 = m->m20 = m->m21 = m->m23 = 0.0f;
    m->m00 = m->m11 = m->m22 = m->m33 = 1.0f;
}

void NuMtxSetScale(NUMTX *m, NUVEC *s) {
    m->m00 = s->x;
    m->m11 = s->y;
    m->m22 = s->z;
    m->m01 = m->m02 = m->m03 = m->m10 = m->m12 = m->m13 = m->m20 = m->m21 = m->m23 = m->m30 = m->m31 = m->m32 = 0.0f;
    m->m33 = 1.0f;
}

void NuMtxSetRotationX(NUMTX *m, NUANG a) {
    m->m11 = m->m22 = NU_COS_LUT(a);
    m->m12 = NU_SIN_LUT(a);
    m->m21 = -m->m12;
    m->m00 = 1.0f;
    m->m01 = m->m02 = m->m03 = m->m23 = m->m10 = m->m20 = m->m13 = m->m30 = m->m31 = m->m32 = 0.0f;
    m->m33 = 1.0f;
}

NU_MTX_SET_ROTATION_Y_IMPL(NuMtxSetRotationY, )

void NuMtxSetRotationZ(NUMTX *m, NUANG a) {
    m->m00 = m->m11 = NU_COS_LUT(a);
    m->m01 = NU_SIN_LUT(a);
    m->m10 = -m->m01;
    m->m22 = 1.0;
    m->m02 = m->m12 = m->m03 = m->m23 = m->m20 = m->m21 = m->m13 = m->m30 = m->m31 = m->m32 = 0.0f;
    m->m33 = 1.0;
}

void NuMtxSetRotationAxis(NUMTX *m, NUANG ang, NUVEC *ax) {
    NUANG angle = ang >> 1; // get half angle
    f32 s = NU_SIN_LUT(angle);
    f32 t = NU_COS_LUT(angle);

    f32 a = ax->x * s;
    f32 _2a = a + a;
    f32 b = ax->y * s;
    f32 _2b = b + b;
    f32 c = ax->z * s;
    f32 _2c = c + c;

    f32 _2sa = t * _2a;
    f32 _2aa = a * _2a;
    f32 _2sb = t * _2b;
    f32 _2ab = a * _2b;
    f32 _2bb = b * _2b;
    f32 _2sc = t * _2c;
    f32 _2ac = a * _2c;
    f32 _2bc = b * _2c;
    f32 _2cc = c * _2c;

    m->m00 = 1.0f - (_2bb + _2cc);
    m->m11 = 1.0f - (_2aa + _2cc);
    m->m22 = 1.0f - (_2aa + _2bb);
    m->m33 = 1.0f;
    m->m01 = _2ab + _2sc;
    m->m02 = _2ac - _2sb;
    m->m03 = 0.0f;
    m->m10 = _2ab - _2sc;
    m->m12 = _2bc + _2sa;
    m->m13 = 0.0f;
    m->m20 = _2ac + _2sb;
    m->m21 = _2bc - _2sa;
    m->m23 = 0.0f;
    m->m30 = m->m31 = m->m32 = 0.0f;
}

void NuMtxTranslate(NUMTX *m, NUVEC *t) {
    m->m30 += t->x;
    m->m31 += t->y;
    m->m32 += t->z;
}

void NuMtxTranslateNeg(NUMTX *m, NUVEC *t) {
    m->m30 -= t->x;
    m->m31 -= t->y;
    m->m32 -= t->z;
}

void NuMtxPreTranslate(NUMTX *m, NUVEC *t) {
    m->m30 += t->x * m->m00 + t->y * m->m10 + t->z * m->m20;
    m->m31 += t->x * m->m01 + t->y * m->m11 + t->z * m->m21;
    m->m32 += t->x * m->m02 + t->y * m->m12 + t->z * m->m22;
}

void NuMtxPreTranslateX(NUMTX *m, f32 tx) {
    m->m30 = m->m30 + m->m00 * tx;
    m->m31 = m->m31 + m->m01 * tx;
    m->m32 = m->m32 + m->m02 * tx;
}

void NuMtxPreTranslateNeg(NUMTX *m, NUVEC *t) {
    m->m30 -= t->x * m->m00 + t->y * m->m10 + t->z * m->m20;
    m->m31 -= t->x * m->m01 + t->y * m->m11 + t->z * m->m21;
    m->m32 -= t->x * m->m02 + t->y * m->m12 + t->z * m->m22;
}

void NuMtxScale(NUMTX *m, NUVEC *s) {
    m->m00 *= s->x;
    m->m01 *= s->y;
    m->m02 *= s->z;
    m->m10 *= s->x;
    m->m11 *= s->y;
    m->m12 *= s->z;
    m->m20 *= s->x;
    m->m21 *= s->y;
    m->m22 *= s->z;
    m->m30 *= s->x;
    m->m31 *= s->y;
    m->m32 *= s->z;
}

void NuMtxScaleU(NUMTX *m, f32 s) {
    m->m00 *= s;
    m->m01 *= s;
    m->m02 *= s;
    m->m10 *= s;
    m->m11 *= s;
    m->m12 *= s;
    m->m20 *= s;
    m->m21 *= s;
    m->m22 *= s;
    m->m30 *= s;
    m->m31 *= s;
    m->m32 *= s;
}

void NuMtxPreScaleU(NUMTX *m, f32 s) {
    m->m00 *= s;
    m->m01 *= s;
    m->m02 *= s;
    m->m10 *= s;
    m->m11 *= s;
    m->m12 *= s;
    m->m20 *= s;
    m->m21 *= s;
    m->m22 *= s;
}

NUVEC NuMtxGetScale(NUMTX *m) {
    NUVEC scale;

    scale.x = NuFsqrt(m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02);
    scale.y = NuFsqrt(m->m10 * m->m10 + m->m11 * m->m11 + m->m12 * m->m12);
    scale.z = NuFsqrt(m->m20 * m->m20 + m->m21 * m->m21 + m->m22 * m->m22);

    return scale;
}

void NuMtxPreScale(NUMTX *m, NUVEC *s) {
    m->m00 *= s->x;
    m->m01 *= s->x;
    m->m02 *= s->x;
    m->m10 *= s->y;
    m->m11 *= s->y;
    m->m12 *= s->y;
    m->m20 *= s->z;
    m->m21 *= s->z;
    m->m22 *= s->z;
}

void NuMtxPreScaleX(NUMTX *m, f32 ScaleX) {
    m->m00 = m->m00 * ScaleX;
    m->m01 = m->m01 * ScaleX;
    m->m02 = m->m02 * ScaleX;
}

void NuMtxPreSkewYX(NUMTX *Mtx, f32 SkewVal) {
    Mtx->m00 = Mtx->m00 + Mtx->m10 * SkewVal;
    Mtx->m01 = Mtx->m01 + Mtx->m11 * SkewVal;
    Mtx->m02 = Mtx->m02 + Mtx->m12 * SkewVal;
}

NU_MTX_ROTATE_X_IMPL(NuMtxRotateX, )

void NuMtxPreRotateX(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m10 = m->m10;
    f32 m11 = m->m11;
    f32 m12 = m->m12;

    m->m10 = cosx * m10 + m->m20 * sinx;
    m->m11 = cosx * m11 + m->m21 * sinx;
    m->m12 = cosx * m12 + m->m22 * sinx;
    m->m20 = m->m20 * cosx - sinx * m10;
    m->m21 = m->m21 * cosx - sinx * m11;
    m->m22 = m->m22 * cosx - sinx * m12;
}

void NuMtxPreRotateY180X(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m10 = m->m10;
    f32 m11 = m->m11;
    f32 m12 = m->m12;

    m->m00 = -m->m00;
    m->m01 = -m->m01;
    m->m02 = -m->m02;
    m->m10 = cosx * m10 - m->m20 * sinx;
    m->m11 = cosx * m11 - m->m21 * sinx;
    m->m12 = cosx * m12 - m->m22 * sinx;
    m->m20 = -(m->m20 * cosx) - sinx * m10;
    m->m21 = -(m->m21 * cosx) - sinx * m11;
    m->m22 = -(m->m22 * cosx) - sinx * m12;
}

NU_MTX_ROTATE_Y_IMPL(NuMtxRotateY, )

void NuMtxPreRotateY(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m01 = m->m01;
    f32 m02 = m->m02;

    m->m00 = cosx * m00 - m->m20 * sinx;
    m->m01 = cosx * m01 - m->m21 * sinx;
    m->m02 = cosx * m02 - m->m22 * sinx;
    m->m20 = sinx * m00 + m->m20 * cosx;
    m->m21 = sinx * m01 + m->m21 * cosx;
    m->m22 = sinx * m02 + m->m22 * cosx;
}

void NuMtxPreRotateY180(NUMTX *m) {
    m->m00 = -m->m00;
    m->m01 = -m->m01;
    m->m02 = -m->m02;
    m->m20 = -m->m20;
    m->m21 = -m->m21;
    m->m22 = -m->m22;
}

NU_MTX_ROTATE_Z_IMPL(NuMtxRotateZ, )

void NuMtxPreRotateZ(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m01 = m->m01;
    f32 m02 = m->m02;

    m->m00 = cosx * m00 + m->m10 * sinx;
    m->m01 = cosx * m01 + m->m11 * sinx;
    m->m02 = cosx * m02 + m->m12 * sinx;
    m->m10 = m->m10 * cosx - sinx * m00;
    m->m11 = m->m11 * cosx - sinx * m01;
    m->m12 = m->m12 * cosx - sinx * m02;
}

void NuMtxSetRotateXYZ(NUMTX *m, NUANGVEC *a) {
    f32 cosx = NU_COS_LUT(a->x);
    f32 sinx = NU_SIN_LUT(a->x);
    f32 cosy = NU_COS_LUT(a->y);
    f32 siny = NU_SIN_LUT(a->y);
    f32 cosz = NU_COS_LUT(a->z);
    f32 sinz = NU_SIN_LUT(a->z);

    m->m00 = cosy * cosz;
    m->m01 = cosy * sinz;
    m->m02 = -siny;
    m->m03 = 0.0f;
    m->m10 = sinx * siny * cosz - cosx * sinz;
    m->m11 = sinx * siny * sinz + cosx * cosz;
    m->m12 = sinx * cosy;
    m->m13 = 0.0f;
    m->m20 = cosx * siny * cosz + sinx * sinz;
    m->m21 = cosx * siny * sinz - sinx * cosz;
    m->m22 = cosx * cosy;
    m->m23 = 0.0f;
    m->m30 = 0.0f;
    m->m31 = 0.0f;
    m->m32 = 0.0f;
    m->m33 = 1.0f;
}

void NuMtxMul(NUMTX *m, NUMTX *m0, NUMTX *m1) {
    NUMTX gm;

    if ((m == m0) || (m == m1)) {
        gm.m00 = m0->m00 * m1->m00 + m0->m01 * m1->m10 + m0->m02 * m1->m20;
        gm.m01 = m0->m00 * m1->m01 + m0->m01 * m1->m11 + m0->m02 * m1->m21;
        gm.m02 = m0->m00 * m1->m02 + m0->m01 * m1->m12 + m0->m02 * m1->m22;
        gm.m03 = 0.0f;
        gm.m10 = m0->m10 * m1->m00 + m0->m11 * m1->m10 + m0->m12 * m1->m20;
        gm.m11 = m0->m10 * m1->m01 + m0->m11 * m1->m11 + m0->m12 * m1->m21;
        gm.m12 = m0->m10 * m1->m02 + m0->m11 * m1->m12 + m0->m12 * m1->m22;
        gm.m13 = 0.0f;
        gm.m20 = m0->m20 * m1->m00 + m0->m21 * m1->m10 + m0->m22 * m1->m20;
        gm.m21 = m0->m20 * m1->m01 + m0->m21 * m1->m11 + m0->m22 * m1->m21;
        gm.m22 = m0->m20 * m1->m02 + m0->m21 * m1->m12 + m0->m22 * m1->m22;
        gm.m23 = 0.0f;
        gm.m30 = m0->m30 * m1->m00 + m0->m31 * m1->m10 + m0->m32 * m1->m20 + m1->m30;
        gm.m31 = m0->m30 * m1->m01 + m0->m31 * m1->m11 + m0->m32 * m1->m21 + m1->m31;
        gm.m32 = m0->m30 * m1->m02 + m0->m31 * m1->m12 + m0->m32 * m1->m22 + m1->m32;
        gm.m33 = 1.0f;
        *m = gm;
    } else {
        m->m00 = m0->m00 * m1->m00 + m0->m01 * m1->m10 + m0->m02 * m1->m20;
        m->m01 = m0->m00 * m1->m01 + m0->m01 * m1->m11 + m0->m02 * m1->m21;
        m->m02 = m0->m00 * m1->m02 + m0->m01 * m1->m12 + m0->m02 * m1->m22;
        m->m03 = 0.0f;
        m->m10 = m0->m10 * m1->m00 + m0->m11 * m1->m10 + m0->m12 * m1->m20;
        m->m11 = m0->m10 * m1->m01 + m0->m11 * m1->m11 + m0->m12 * m1->m21;
        m->m12 = m0->m10 * m1->m02 + m0->m11 * m1->m12 + m0->m12 * m1->m22;
        m->m13 = 0.0f;
        m->m20 = m0->m20 * m1->m00 + m0->m21 * m1->m10 + m0->m22 * m1->m20;
        m->m21 = m0->m20 * m1->m01 + m0->m21 * m1->m11 + m0->m22 * m1->m21;
        m->m22 = m0->m20 * m1->m02 + m0->m21 * m1->m12 + m0->m22 * m1->m22;
        m->m23 = 0.0f;
        m->m30 = m0->m30 * m1->m00 + m0->m31 * m1->m10 + m0->m32 * m1->m20 + m1->m30;
        m->m31 = m0->m30 * m1->m01 + m0->m31 * m1->m11 + m0->m32 * m1->m21 + m1->m31;
        m->m32 = m0->m30 * m1->m02 + m0->m31 * m1->m12 + m0->m32 * m1->m22 + m1->m32;
        m->m33 = 1.0f;
    }
}

void NuMtxMulH(NUMTX *m, NUMTX *m0, NUMTX *m1) {
    NUMTX gm;

    if ((m == m0) || (m == m1)) {
        gm.m00 = m0->m00 * m1->m00 + m0->m01 * m1->m10 + m0->m02 * m1->m20 + m0->m03 * m1->m30;
        gm.m01 = m0->m00 * m1->m01 + m0->m01 * m1->m11 + m0->m02 * m1->m21 + m0->m03 * m1->m31;
        gm.m02 = m0->m00 * m1->m02 + m0->m01 * m1->m12 + m0->m02 * m1->m22 + m0->m03 * m1->m32;
        gm.m03 = m0->m00 * m1->m03 + m0->m01 * m1->m13 + m0->m02 * m1->m23 + m0->m03 * m1->m33;
        gm.m10 = m0->m10 * m1->m00 + m0->m11 * m1->m10 + m0->m12 * m1->m20 + m0->m13 * m1->m30;
        gm.m11 = m0->m10 * m1->m01 + m0->m11 * m1->m11 + m0->m12 * m1->m21 + m0->m13 * m1->m31;
        gm.m12 = m0->m10 * m1->m02 + m0->m11 * m1->m12 + m0->m12 * m1->m22 + m0->m13 * m1->m32;
        gm.m13 = m0->m10 * m1->m03 + m0->m11 * m1->m13 + m0->m12 * m1->m23 + m0->m13 * m1->m33;
        gm.m20 = m0->m20 * m1->m00 + m0->m21 * m1->m10 + m0->m22 * m1->m20 + m0->m23 * m1->m30;
        gm.m21 = m0->m20 * m1->m01 + m0->m21 * m1->m11 + m0->m22 * m1->m21 + m0->m23 * m1->m31;
        gm.m22 = m0->m20 * m1->m02 + m0->m21 * m1->m12 + m0->m22 * m1->m22 + m0->m23 * m1->m32;
        gm.m23 = m0->m20 * m1->m03 + m0->m21 * m1->m13 + m0->m22 * m1->m23 + m0->m23 * m1->m33;
        gm.m30 = m0->m30 * m1->m00 + m0->m31 * m1->m10 + m0->m32 * m1->m20 + m0->m33 * m1->m30;
        gm.m31 = m0->m30 * m1->m01 + m0->m31 * m1->m11 + m0->m32 * m1->m21 + m0->m33 * m1->m31;
        gm.m32 = m0->m30 * m1->m02 + m0->m31 * m1->m12 + m0->m32 * m1->m22 + m0->m33 * m1->m32;
        gm.m33 = m0->m30 * m1->m03 + m0->m31 * m1->m13 + m0->m32 * m1->m23 + m0->m33 * m1->m33;
        *m = gm;
    } else {
        m->m00 = m0->m00 * m1->m00 + m0->m01 * m1->m10 + m0->m02 * m1->m20 + m0->m03 * m1->m30;
        m->m01 = m0->m00 * m1->m01 + m0->m01 * m1->m11 + m0->m02 * m1->m21 + m0->m03 * m1->m31;
        m->m02 = m0->m00 * m1->m02 + m0->m01 * m1->m12 + m0->m02 * m1->m22 + m0->m03 * m1->m32;
        m->m03 = m0->m00 * m1->m03 + m0->m01 * m1->m13 + m0->m02 * m1->m23 + m0->m03 * m1->m33;
        m->m10 = m0->m10 * m1->m00 + m0->m11 * m1->m10 + m0->m12 * m1->m20 + m0->m13 * m1->m30;
        m->m11 = m0->m10 * m1->m01 + m0->m11 * m1->m11 + m0->m12 * m1->m21 + m0->m13 * m1->m31;
        m->m12 = m0->m10 * m1->m02 + m0->m11 * m1->m12 + m0->m12 * m1->m22 + m0->m13 * m1->m32;
        m->m13 = m0->m10 * m1->m03 + m0->m11 * m1->m13 + m0->m12 * m1->m23 + m0->m13 * m1->m33;
        m->m20 = m0->m20 * m1->m00 + m0->m21 * m1->m10 + m0->m22 * m1->m20 + m0->m23 * m1->m30;
        m->m21 = m0->m20 * m1->m01 + m0->m21 * m1->m11 + m0->m22 * m1->m21 + m0->m23 * m1->m31;
        m->m22 = m0->m20 * m1->m02 + m0->m21 * m1->m12 + m0->m22 * m1->m22 + m0->m23 * m1->m32;
        m->m23 = m0->m20 * m1->m03 + m0->m21 * m1->m13 + m0->m22 * m1->m23 + m0->m23 * m1->m33;
        m->m30 = m0->m30 * m1->m00 + m0->m31 * m1->m10 + m0->m32 * m1->m20 + m0->m33 * m1->m30;
        m->m31 = m0->m30 * m1->m01 + m0->m31 * m1->m11 + m0->m32 * m1->m21 + m0->m33 * m1->m31;
        m->m32 = m0->m30 * m1->m02 + m0->m31 * m1->m12 + m0->m32 * m1->m22 + m0->m33 * m1->m32;
        m->m33 = m0->m30 * m1->m03 + m0->m31 * m1->m13 + m0->m32 * m1->m23 + m0->m33 * m1->m33;
    }
}

void NuMtxMulR(NUMTX *m, NUMTX *m0, NUMTX *m1) {
    NUMTX gm;

    if ((m == m0) || (m == m1)) {
        gm.m00 = m0->m00 * m1->m00 + m0->m01 * m1->m10 + m0->m02 * m1->m20;
        gm.m01 = m0->m00 * m1->m01 + m0->m01 * m1->m11 + m0->m02 * m1->m21;
        gm.m02 = m0->m00 * m1->m02 + m0->m01 * m1->m12 + m0->m02 * m1->m22;
        gm.m03 = 0.0;
        gm.m10 = m0->m10 * m1->m00 + m0->m11 * m1->m10 + m0->m12 * m1->m20;
        gm.m11 = m0->m10 * m1->m01 + m0->m11 * m1->m11 + m0->m12 * m1->m21;
        gm.m12 = m0->m10 * m1->m02 + m0->m11 * m1->m12 + m0->m12 * m1->m22;
        gm.m13 = 0.0;
        gm.m20 = m0->m20 * m1->m00 + m0->m21 * m1->m10 + m0->m22 * m1->m20;
        gm.m21 = m0->m20 * m1->m01 + m0->m21 * m1->m11 + m0->m22 * m1->m21;
        gm.m22 = m0->m20 * m1->m02 + m0->m21 * m1->m12 + m0->m22 * m1->m22;
        gm.m23 = 0.0;
        gm.m30 = gm.m31 = gm.m32 = 0.0f;
        gm.m33 = 1.0;
        *m = gm;
    } else {
        m->m00 = m0->m00 * m1->m00 + m0->m01 * m1->m10 + m0->m02 * m1->m20;
        m->m01 = m0->m00 * m1->m01 + m0->m01 * m1->m11 + m0->m02 * m1->m21;
        m->m02 = m0->m00 * m1->m02 + m0->m01 * m1->m12 + m0->m02 * m1->m22;
        m->m03 = 0.0;
        m->m10 = m0->m10 * m1->m00 + m0->m11 * m1->m10 + m0->m12 * m1->m20;
        m->m11 = m0->m10 * m1->m01 + m0->m11 * m1->m11 + m0->m12 * m1->m21;
        m->m12 = m0->m10 * m1->m02 + m0->m11 * m1->m12 + m0->m12 * m1->m22;
        m->m13 = 0.0;
        m->m20 = m0->m20 * m1->m00 + m0->m21 * m1->m10 + m0->m22 * m1->m20;
        m->m21 = m0->m20 * m1->m01 + m0->m21 * m1->m11 + m0->m22 * m1->m21;
        m->m22 = m0->m20 * m1->m02 + m0->m21 * m1->m12 + m0->m22 * m1->m22;
        m->m23 = 0.0;
        m->m30 = m->m31 = m->m32 = 0.0f;
        m->m33 = 1.0;
    }
}

void NuMtxTransposeR(NUMTX *m, NUMTX *m0) {
    f32 t;

    t = m0->m01;
    m->m01 = m0->m10;
    m->m10 = t;
    t = m0->m02;
    m->m02 = m0->m20;
    m->m20 = t;
    t = m0->m12;
    m->m12 = m0->m21;
    m->m21 = t;
    m->m00 = m0->m00;
    m->m11 = m0->m11;
    m->m22 = m0->m22;
    m->m30 = m0->m30;
    m->m31 = m0->m31;
    m->m32 = m0->m32;
    m->m33 = m0->m33;
}

void NuMtxTranspose(NUMTX *m, NUMTX *m0) {
    f32 t;

    t = m0->m01;
    m->m01 = m0->m10;
    m->m10 = t;
    t = m0->m02;
    m->m02 = m0->m20;
    m->m20 = t;
    t = m0->m03;
    m->m03 = m0->m30;
    m->m30 = t;
    t = m0->m12;
    m->m12 = m0->m21;
    m->m21 = t;
    t = m0->m13;
    m->m13 = m0->m31;
    m->m31 = t;
    t = m0->m23;
    m->m23 = m0->m32;
    m->m32 = t;
    m->m00 = m0->m00;
    m->m11 = m0->m11;
    m->m22 = m0->m22;
    m->m33 = m0->m33;
}

void NuMtxInv(NUMTX *m, NUMTX *m0) {
    f32 t;

    f32 tx = -m0->m30;
    f32 ty = -m0->m31;
    f32 tz = -m0->m32;

    t = m0->m01;
    m->m01 = m0->m10;
    m->m10 = t;
    t = m0->m02;
    m->m02 = m0->m20;
    m->m20 = t;
    t = m0->m12;
    m->m12 = m0->m21;
    m->m21 = t;
    m->m00 = m0->m00;
    m->m11 = m0->m11;
    m->m22 = m0->m22;
    m->m30 = m->m00 * tx + m->m10 * ty + m->m20 * tz;
    m->m31 = m->m01 * tx + m->m11 * ty + m->m21 * tz;
    m->m32 = m->m02 * tx + m->m12 * ty + m->m22 * tz;
    m->m23 = 0.0f;
    m->m13 = m->m23;
    m->m03 = m->m13;
    m->m33 = 1.0f;
}

void NuMtxInvR(NUMTX *m, NUMTX *m0) {
    f32 t;

    t = m0->m01;
    m->m01 = m0->m10;
    m->m10 = t;
    t = m0->m02;
    m->m02 = m0->m20;
    m->m20 = t;
    t = m0->m12;
    m->m12 = m0->m21;
    m->m21 = t;
    m->m00 = m0->m00;
    m->m11 = m0->m11;
    m->m22 = m0->m22;
    m->m23 = 0.0f;
    m->m13 = m->m23;
    m->m03 = m->m13;
    m->m32 = 0.0f;
    m->m31 = m->m32;
    m->m30 = m->m31;
    m->m33 = 1.0f;
}

void NuMtxInvRSS(NUMTX *inv, NUMTX *T) {
    NUMTX gm;

    f32 det = T->m00 * (T->m11 * T->m22 - T->m12 * T->m21) - T->m01 * (T->m10 * T->m22 - T->m12 * T->m20) +
              T->m02 * (T->m10 * T->m21 - T->m11 * T->m20);
    f32 invdet = det == 0.0f ? 0.0f : 1.0f / det;

    gm.m00 = (T->m11 * T->m22 - T->m12 * T->m21) * invdet;
    gm.m10 = (T->m10 * T->m22 - T->m12 * T->m20) * -invdet;
    gm.m20 = (T->m10 * T->m21 - T->m11 * T->m20) * invdet;
    gm.m01 = (T->m01 * T->m22 - T->m02 * T->m21) * -invdet;
    gm.m11 = (T->m00 * T->m22 - T->m02 * T->m20) * invdet;
    gm.m21 = (T->m00 * T->m21 - T->m01 * T->m20) * -invdet;
    gm.m02 = (T->m01 * T->m12 - T->m02 * T->m11) * invdet;
    gm.m12 = (T->m00 * T->m12 - T->m02 * T->m10) * -invdet;
    gm.m22 = (T->m00 * T->m11 - T->m01 * T->m10) * invdet;
    gm.m03 = 0.0f;
    gm.m13 = 0.0f;
    gm.m23 = 0.0f;
    gm.m33 = 1.0f;
    gm.m30 = 0.0f;
    gm.m31 = 0.0f;
    gm.m32 = 0.0f;
    *inv = gm;
}

void NuMtxInvRSSH(NUMTX *inv, NUMTX *T) {
    NUMTX gm;

    f32 tx = -T->m30;
    f32 ty = -T->m31;
    f32 tz = -T->m32;
    f32 det = T->m00 * (T->m11 * T->m22 - T->m12 * T->m21) - T->m01 * (T->m10 * T->m22 - T->m12 * T->m20) +
              T->m02 * (T->m10 * T->m21 - T->m11 * T->m20);
    f32 invdet = 1.0f / det;

    gm.m00 = (T->m11 * T->m22 - T->m12 * T->m21) * invdet;
    gm.m10 = (T->m10 * T->m22 - T->m12 * T->m20) * -invdet;
    gm.m20 = (T->m10 * T->m21 - T->m11 * T->m20) * invdet;
    gm.m01 = (T->m01 * T->m22 - T->m02 * T->m21) * -invdet;
    gm.m11 = (T->m00 * T->m22 - T->m02 * T->m20) * invdet;
    gm.m21 = (T->m00 * T->m21 - T->m01 * T->m20) * -invdet;
    gm.m02 = (T->m01 * T->m12 - T->m02 * T->m11) * invdet;
    gm.m12 = (T->m00 * T->m12 - T->m02 * T->m10) * -invdet;
    gm.m22 = (T->m00 * T->m11 - T->m01 * T->m10) * invdet;
    gm.m30 = gm.m00 * tx + gm.m10 * ty + gm.m20 * tz;
    gm.m31 = gm.m01 * tx + gm.m11 * ty + gm.m21 * tz;
    gm.m32 = gm.m02 * tx + gm.m12 * ty + gm.m22 * tz;
    gm.m03 = 0.0f;
    gm.m13 = 0.0f;
    gm.m23 = 0.0f;
    gm.m33 = 1.0f;
    *inv = gm;
}

void NuMtxInvH(NUMTX *mi, NUMTX *m0) {
    // there's some weird stack alignment stuff going on here...
    // also not sure if this semantically matches the original implementation
    i32 p[4];
    i32 j;
    i32 k;
    i32 i;
    f32 a[4][4];
    i32 n;
    f32 local_1c;
    f32 localm20;
    f32 localm28;
    f32 localm30;

    n = 4;

    a[0][0] = m0->m00;
    a[0][1] = m0->m01;
    a[0][2] = m0->m02;
    a[0][3] = m0->m03;
    a[1][0] = m0->m10;
    a[1][1] = m0->m11;
    a[1][2] = m0->m12;
    a[1][3] = m0->m13;
    a[2][0] = m0->m20;
    a[2][1] = m0->m21;
    a[2][2] = m0->m22;
    a[2][3] = m0->m23;
    a[3][0] = m0->m30;
    a[3][1] = m0->m31;
    a[3][2] = m0->m32;
    a[3][3] = m0->m33;

    for (i = 0; i < n; i++) {
        localm20 = 0.0f;
        p[i] = 0;

        for (j = i; j < n; j++) {
            local_1c = 0.0f;

            for (k = i; k < n; k++) {
                local_1c += NuFabs(a[j][k]);
            }

            localm28 = NuFdiv(NuFabs(a[j][i]), local_1c);

            if (localm20 < localm28) {
                localm20 = localm28;
                p[i] = j;
            }
        }

        if (localm20 == 0.0f) {
            return;
        }

        if (p[i] != i) {
            for (k = 0; k < n; k++) {
                f32 f;

                f = a[i][k];
                a[i][k] = a[p[i]][k];
                a[p[i]][k] = f;
            }
        }

        localm30 = a[i][i];

        for (k = 0; k < n; k++) {
            if (k != i) {
                a[i][k] = -a[i][k] / localm30;
                for (j = 0; j < n; j++) {
                    if (j != i) {
                        a[j][k] += a[j][i] * a[i][k];
                    }
                }
            }
        }

        for (j = 0; j < n; j++) {
            a[j][i] = a[j][i] / localm30;
        }

        a[i][i] = 1.0f / localm30;
    }

    for (i = n - 1; i >= 0; i--) {
        if (p[i] != i) {
            for (j = 0; j < n; j++) {
                f32 f;

                f = a[j][i];
                a[j][i] = a[j][p[i]];
                a[j][p[i]] = f;
            }
        }
    }

    mi->m00 = a[0][0];
    mi->m01 = a[0][1];
    mi->m02 = a[0][2];
    mi->m03 = a[0][3];
    mi->m10 = a[1][0];
    mi->m11 = a[1][1];
    mi->m12 = a[1][2];
    mi->m13 = a[1][3];
    mi->m20 = a[2][0];
    mi->m21 = a[2][1];
    mi->m22 = a[2][2];
    mi->m23 = a[2][3];
    mi->m30 = a[3][0];
    mi->m31 = a[3][1];
    mi->m32 = a[3][2];
    mi->m33 = a[3][3];
}

void NuMtxAlignX(NUMTX *m, NUVEC *v) {
    m->m00 = v->x;
    m->m01 = v->y;
    m->m02 = v->z;
    m->m20 = m->m01 * m->m12 - m->m02 * m->m11;
    m->m21 = m->m02 * m->m10 - m->m00 * m->m12;
    m->m22 = m->m00 * m->m11 - m->m01 * m->m10;

    f32 s = NuFsqrt(m->m20 * m->m20 + m->m21 * m->m21 + m->m22 * m->m22);
    s = NuFdiv(1.0f, s);

    m->m20 = m->m20 * s;
    m->m21 = m->m21 * s;
    m->m22 = m->m22 * s;
    m->m10 = m->m21 * m->m02 - m->m22 * m->m01;
    m->m11 = m->m22 * m->m00 - m->m20 * m->m02;
    m->m12 = m->m20 * m->m01 - m->m21 * m->m00;
}

void NuMtxAlignY(NUMTX *m, NUVEC *v) {
    m->m10 = v->x;
    m->m11 = v->y;
    m->m12 = v->z;
    m->m00 = m->m11 * m->m22 - m->m12 * m->m21;
    m->m01 = m->m12 * m->m20 - m->m10 * m->m22;
    m->m02 = m->m10 * m->m21 - m->m11 * m->m20;

    f32 s = NuFsqrt(m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02);
    s = NuFdiv(1.0f, s);

    m->m00 = m->m00 * s;
    m->m01 = m->m01 * s;
    m->m02 = m->m02 * s;
    m->m20 = m->m01 * m->m12 - m->m02 * m->m11;
    m->m21 = m->m02 * m->m10 - m->m00 * m->m12;
    m->m22 = m->m00 * m->m11 - m->m01 * m->m10;
}

void NuMtxAlignZ(NUMTX *m, NUVEC *v) {
    f32 xLenSq = m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02;
    f32 yLenSq = m->m10 * m->m10 + m->m11 * m->m11 + m->m12 * m->m12;
    f32 zLenSq = m->m20 * m->m20 + m->m21 * m->m21 + m->m22 * m->m22;

    f32 len = NuFsqrt(NuFdiv(zLenSq, v->x * v->x + v->y * v->y + v->z * v->z));
    m->m20 = v->x * len;
    m->m21 = v->y * len;
    m->m22 = v->z * len;

    if (NuFabs(NuVecDot(NUMTX_GET_ROW_VEC(m, 1), NUMTX_GET_ROW_VEC(m, 2))) > 0.86602540378f /* cos(30 deg) */) {
        NuVecCross(NUMTX_GET_ROW_VEC(m, 1), NUMTX_GET_ROW_VEC(m, 2), NUMTX_GET_ROW_VEC(m, 0));
        len = NuFsqrt(NuFdiv(yLenSq, m->m10 * m->m10 + m->m11 * m->m11 + m->m12 * m->m12));
        m->m10 = m->m10 * len;
        m->m11 = m->m11 * len;
        m->m12 = m->m12 * len;
        NuVecCross(NUMTX_GET_ROW_VEC(m, 0), NUMTX_GET_ROW_VEC(m, 1), NUMTX_GET_ROW_VEC(m, 2));
        len = NuFsqrt(NuFdiv(xLenSq, m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02));
        m->m00 = m->m00 * len;
        m->m01 = m->m01 * len;
        m->m02 = m->m02 * len;
    } else {
        NuVecCross(NUMTX_GET_ROW_VEC(m, 0), NUMTX_GET_ROW_VEC(m, 1), NUMTX_GET_ROW_VEC(m, 2));
        len = NuFsqrt(NuFdiv(xLenSq, m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02));
        m->m00 = m->m00 * len;
        m->m01 = m->m01 * len;
        m->m02 = m->m02 * len;
        NuVecCross(NUMTX_GET_ROW_VEC(m, 1), NUMTX_GET_ROW_VEC(m, 2), NUMTX_GET_ROW_VEC(m, 0));
        len = NuFsqrt(NuFdiv(yLenSq, m->m10 * m->m10 + m->m11 * m->m11 + m->m12 * m->m12));
        m->m10 = m->m10 * len;
        m->m11 = m->m11 * len;
        m->m12 = m->m12 * len;
    }
}

void NuMtxLookAtX(NUMTX *m, NUVEC *pnt) {
    NUVEC v;

    v.x = pnt->x - m->m30;
    v.y = pnt->y - m->m31;
    v.z = pnt->z - m->m32;

    NuVecNorm(&v, &v);
    NuMtxAlignX(m, &v);
}

void NuMtxLookAtY(NUMTX *m, NUVEC *pnt) {
    NUVEC v;

    v.x = pnt->x - m->m30;
    v.y = pnt->y - m->m31;
    v.z = pnt->z - m->m32;

    NuVecNorm(&v, &v);
    NuMtxAlignY(m, &v);
}

void NuMtxLookAtZ(NUMTX *m, NUVEC *pnt) {
    NUVEC v;

    v.x = pnt->x - m->m30;
    v.y = pnt->y - m->m31;
    v.z = pnt->z - m->m32;

    NuVecNorm(&v, &v);
    NuMtxAlignZ(m, &v);
}

void NuMtxInvLookAtX(NUMTX *m, NUVEC *pnt) {
    NUVEC v;

    v.x = m->m30 - pnt->x;
    v.y = m->m31 - pnt->y;
    v.z = m->m32 - pnt->z;

    NuVecNorm(&v, &v);
    NuMtxAlignX(m, &v);
}

void NuMtxInvLookAtY(NUMTX *m, NUVEC *pnt) {
    NUVEC v;

    v.x = m->m30 - pnt->x;
    v.y = m->m31 - pnt->y;
    v.z = m->m32 - pnt->z;

    NuVecNorm(&v, &v);
    NuMtxAlignY(m, &v);
}

void NuMtxInvLookAtZ(NUMTX *m, NUVEC *pnt) {
    NUVEC v;

    v.x = m->m30 - pnt->x;
    v.y = m->m31 - pnt->y;
    v.z = m->m32 - pnt->z;

    NuVecNorm(&v, &v);
    NuMtxAlignZ(m, &v);
}

void NuMtxAddR(NUMTX *m, NUMTX *m0, NUMTX *m1) {
    m->m00 = m0->m00 + m1->m00;
    m->m01 = m0->m01 + m1->m01;
    m->m02 = m0->m02 + m1->m02;
    m->m03 = 0.0f;
    m->m10 = m0->m10 + m1->m10;
    m->m11 = m0->m11 + m1->m11;
    m->m12 = m0->m12 + m1->m12;
    m->m13 = 0.0f;
    m->m20 = m0->m20 + m1->m20;
    m->m21 = m0->m21 + m1->m21;
    m->m22 = m0->m22 + m1->m22;
    m->m23 = 0.0f;
    m->m30 = 0.0f;
    m->m31 = 0.0f;
    m->m32 = 0.0f;
    m->m33 = 1.0f;
}

void NuMtxSubR(NUMTX *m, NUMTX *m0, NUMTX *m1) {
    m->m00 = m0->m00 - m1->m00;
    m->m01 = m0->m01 - m1->m01;
    m->m02 = m0->m02 - m1->m02;
    m->m03 = 0.0f;
    m->m10 = m0->m10 - m1->m10;
    m->m11 = m0->m11 - m1->m11;
    m->m12 = m0->m12 - m1->m12;
    m->m13 = 0.0f;
    m->m20 = m0->m20 - m1->m20;
    m->m21 = m0->m21 - m1->m21;
    m->m22 = m0->m22 - m1->m22;
    m->m23 = 0.0f;
    m->m30 = 0.0f;
    m->m31 = 0.0f;
    m->m32 = 0.0f;
    m->m33 = 1.0f;
}

void NuMtxSkewSymmetric(NUMTX *m, NUVEC *v) {
    m->m00 = 0.0;
    m->m01 = -v->z;
    m->m02 = v->y;
    m->m03 = 0.0;
    m->m10 = v->z;
    m->m11 = 0.0;
    m->m12 = -v->x;
    m->m13 = 0.0;
    m->m20 = -v->y;
    m->m21 = v->x;
    m->m22 = 0.0;
    m->m23 = 0.0;
    m->m30 = 0.0;
    m->m31 = 0.0;
    m->m32 = 0.0;
    m->m33 = 1.0;
}

void NuMtxOrth(NUMTX *m) {
    f32 magnitude;

    magnitude = 1.0f / NuFsqrt(m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02);
    m->m00 = m->m00 * magnitude;
    m->m01 = m->m01 * magnitude;
    m->m02 = m->m02 * magnitude;

    magnitude = 1.0f / NuFsqrt(m->m10 * m->m10 + m->m11 * m->m11 + m->m12 * m->m12);
    m->m10 = m->m10 * magnitude;
    m->m11 = m->m11 * magnitude;
    m->m12 = m->m12 * magnitude;
    m->m20 = m->m01 * m->m12 - m->m02 * m->m11;
    m->m21 = m->m02 * m->m10 - m->m00 * m->m12;
    m->m22 = m->m00 * m->m11 - m->m01 * m->m10;
    m->m10 = m->m21 * m->m02 - m->m22 * m->m01;
    m->m11 = m->m22 * m->m00 - m->m20 * m->m02;
    m->m12 = m->m20 * m->m01 - m->m21 * m->m00;
}

void NuMtxGetXAxis(NUMTX *m, NUVEC *x) {
    x->x = m->m00;
    x->y = m->m01;
    x->z = m->m02;
}

void NuMtxGetYAxis(NUMTX *m, NUVEC *y) {
    y->x = m->m10;
    y->y = m->m11;
    y->z = m->m12;
}

void NuMtxGetZAxis(NUMTX *m, NUVEC *z) {
    z->x = m->m20;
    z->y = m->m21;
    z->z = m->m22;
}

void NuMtxGetTranslation(NUMTX *m, NUVEC *t) {
    t->x = m->m30;
    t->y = m->m31;
    t->z = m->m32;
}

void NuMtxRotateAng(NUANG ang, f32 x, f32 z, f32 *rx, f32 *rz) {
    f32 sinx = NU_SIN_LUT(ang);
    f32 cosx = NU_COS_LUT(ang);

    *rx = x * cosx - z * sinx;
    *rz = x * sinx + z * cosx;
}

void NuMtxVecToEulerXYZ(NUVEC *XVec, NUVEC *ZVec, NUANG *x, NUANG *y, NUANG *z) {
    NUVEC XVec2;
    NUVEC ZVec2;

    *z = NuAtan2D(XVec->y, XVec->x);
    NuMtxRotateAng(-*z, XVec->x, XVec->y, &XVec2.x, &XVec2.y);
    XVec2.z = XVec->z;
    NuMtxRotateAng(-*z, ZVec->x, ZVec->y, &ZVec2.x, &ZVec2.y);
    ZVec2.z = ZVec->z;
    *y = -NuAtan2D(XVec2.z, XVec2.x);
    NuMtxRotateAng(*y, ZVec2.x, ZVec2.z, &ZVec2.x, &ZVec2.z);
    *x = -NuAtan2D(ZVec2.y, ZVec2.z);
}

void NuMtxGetEulerXYZ(NUMTX *Mat, NUANG *x, NUANG *y, NUANG *z) {
    NUVEC XVec;
    NUVEC ZVec;

    NuMtxGetXAxis(Mat, &XVec);
    NuMtxGetZAxis(Mat, &ZVec);
    NuMtxVecToEulerXYZ(&XVec, &ZVec, x, y, z);
}

f32 NuMtxDet3(NUMTX *m) {
    return m->m00 * (m->m11 * m->m22 - m->m12 * m->m21) - m->m01 * (m->m10 * m->m22 - m->m12 * m->m20) +
           m->m02 * (m->m10 * m->m21 - m->m11 * m->m20);
}

i32 NuMtxCompare(NUMTX *a, NUMTX *b) {
    f32 *aa = &a->m00;
    f32 *bb = &b->m00;

    for (i32 i = 0; i < 16; i++) {
        if (*aa < *bb) {
            return -1;
        }
        if (*aa > *bb) {
            return 1;
        }
        aa += 1;
        bb += 1;
    }

    return 0;
}

f32 NuMtxSSE(NUMTX *a, NUMTX *b) {
    f32 sse;

    sse = (a->m00 - b->m00) * (a->m00 - b->m00);
    sse += (a->m01 - b->m01) * (a->m01 - b->m01);
    sse += (a->m02 - b->m02) * (a->m02 - b->m02);
    sse += (a->m03 - b->m03) * (a->m03 - b->m03);
    sse += (a->m10 - b->m10) * (a->m10 - b->m10);
    sse += (a->m11 - b->m11) * (a->m11 - b->m11);
    sse += (a->m12 - b->m12) * (a->m12 - b->m12);
    sse += (a->m13 - b->m13) * (a->m13 - b->m13);
    sse += (a->m20 - b->m20) * (a->m20 - b->m20);
    sse += (a->m21 - b->m21) * (a->m21 - b->m21);
    sse += (a->m22 - b->m22) * (a->m22 - b->m22);
    sse += (a->m23 - b->m23) * (a->m23 - b->m23);
    sse += (a->m30 - b->m30) * (a->m30 - b->m30);
    sse += (a->m31 - b->m31) * (a->m31 - b->m31);
    sse += (a->m32 - b->m32) * (a->m32 - b->m32);
    sse += (a->m33 - b->m33) * (a->m33 - b->m33);

    return sse;
}

void NuMtxTruncate24Bit(NUMTX *trunc, NUMTX *mtx) {
    // was this really worth it?
    // struct padding and uints bigger than 4 bytes be damned
    u32 *trunc_int = (u32 *)trunc;
    u32 *mtx_int = (u32 *)mtx;

    trunc_int[0] = mtx_int[0] & 0xffffff00;
    trunc_int[1] = mtx_int[1] & 0xffffff00;
    trunc_int[2] = mtx_int[2] & 0xffffff00;
    trunc_int[3] = mtx_int[3] & 0xffffff00;

    trunc_int[4] = mtx_int[4] & 0xffffff00;
    trunc_int[5] = mtx_int[5] & 0xffffff00;
    trunc_int[6] = mtx_int[6] & 0xffffff00;
    trunc_int[7] = mtx_int[7] & 0xffffff00;

    trunc_int[8] = mtx_int[8] & 0xffffff00;
    trunc_int[9] = mtx_int[9] & 0xffffff00;
    trunc_int[10] = mtx_int[10] & 0xffffff00;
    trunc_int[11] = mtx_int[11] & 0xffffff00;

    trunc_int[12] = mtx_int[12] & 0xffffff00;
    trunc_int[13] = mtx_int[13] & 0xffffff00;
    trunc_int[14] = mtx_int[14] & 0xffffff00;
    trunc_int[15] = mtx_int[15] & 0xffffff00;
}

void NuMtx24BitCorrection(NUMTX *correction, NUMTX *matrix) {
    f32 best_error = 3.4028234663852886e+38f;
    i32 element;
    i32 pass;
    f32 step = 1.0f;
    NUMTX truncated;
    NUMTX product;
    NuMtxTruncate24Bit(&truncated, matrix);
    *correction = numtx_identity;
    for (pass = 0; pass < 100; ++pass) {
        for (element = 0; element < 16; ++element) {
            if (element % 4 == 3)
                continue;
            f32 previous;
            f32 error;
            f32 *value = &correction->m00 + element;
            previous = *value;
            *value += step;
            reinterpret_cast<u8 *>(value)[0] = 0;
            NuMtxMulH(&product, correction, &truncated);
            error = NuMtxSSE(&product, matrix);
            if (error >= best_error)
                *value = previous;
            else
                best_error = error;
            previous = *value;
            *value -= step;
            reinterpret_cast<u8 *>(value)[0] = 0;
            NuMtxMulH(&product, correction, &truncated);
            error = NuMtxSSE(&product, matrix);
            if (error >= best_error)
                *value = previous;
            else
                best_error = error;
        }
        step *= 0.9f;
    }
}

void NuMtxLookAtD3D(NUMTX *mtx, NUVEC *eye, NUVEC *center, NUVEC *up) {
    NUVEC ax;
    NUVEC ay;
    NUVEC az;

    NuVecSub(&az, center, eye);
    NuVecNorm(&az, &az);
    NuVecCross(&ax, up, &az);
    NuVecNorm(&ax, &ax);
    NuVecCross(&ay, &az, &ax);
    NuVecNorm(&ay, &ay);

    mtx->m00 = ax.x;
    mtx->m01 = ay.x;
    mtx->m02 = az.x;
    mtx->m03 = 0.0f;
    mtx->m10 = ax.y;
    mtx->m11 = ay.y;
    mtx->m12 = az.y;
    mtx->m13 = 0.0f;
    mtx->m20 = ax.z;
    mtx->m21 = ay.z;
    mtx->m22 = az.z;
    mtx->m23 = 0.0f;
    mtx->m30 = (-eye->x * ax.x - eye->y * ax.y) - eye->z * ax.z;
    mtx->m31 = (-eye->x * ay.x - eye->y * ay.y) - eye->z * ay.z;
    mtx->m32 = (-eye->x * az.x - eye->y * az.y) - eye->z * az.z;
    mtx->m33 = 1.0f;
}

void NuMtxLookAtInverseD3D(NUMTX *mtx, NUVEC *eye, NUVEC *center, NUVEC *up) {
    NUVEC ax;
    NUVEC ay;
    NUVEC az;

    NuVecSub(&az, center, eye);
    NuVecNorm(&az, &az);
    NuVecCross(&ax, up, &az);
    NuVecNorm(&ax, &ax);
    NuVecCross(&ay, &az, &ax);
    NuVecNorm(&ay, &ay);

    mtx->m00 = ax.x;
    mtx->m01 = ax.y;
    mtx->m02 = ax.z;
    mtx->m03 = 0.0f;
    mtx->m10 = ay.x;
    mtx->m11 = ay.y;
    mtx->m12 = ay.z;
    mtx->m13 = 0.0f;
    mtx->m20 = az.x;
    mtx->m21 = az.y;
    mtx->m22 = az.z;
    mtx->m23 = 0.0f;
    mtx->m30 = eye->x;
    mtx->m31 = eye->y;
    mtx->m32 = eye->z;
    mtx->m33 = 1.0f;
}

void NuMtxSetPerspectiveD3D(NUMTX *mtx, f32 fovy, f32 aspect, f32 zNear, f32 zFar) {
    f32 tanFovy2 = NU_SIN_LUT((i32)((fovy / 2.0f) * (65536.0f / 360.0f))) /
                   NU_COS_LUT((i32)((fovy / 2.0f) * (65536.0f / 360.0f))); // 65536 / 360 angle units per degree
    mtx->m00 = 1.0 / (aspect * tanFovy2);
    mtx->m01 = 0.0;
    mtx->m02 = 0.0;
    mtx->m03 = 0.0;
    mtx->m10 = 0.0;
    mtx->m11 = 1.0 / tanFovy2;
    mtx->m12 = 0.0;
    mtx->m13 = 0.0;
    mtx->m20 = 0.0;
    mtx->m21 = 0.0;
    mtx->m22 = zFar / (zFar - zNear);
    mtx->m23 = 1.0;
    mtx->m30 = 0.0;
    mtx->m31 = 0.0;
    mtx->m32 = (-zFar * zNear) / (zFar - zNear);
    mtx->m33 = 0.0;
}

void NuMtxSetPerspectiveBlend(NUMTX *mtx, f32 fovy, f32 aspect, f32 zNear, f32 zFar) {
    f32 tanFovy2 = NU_SIN_LUT((i32)((fovy / 2.0f) * (65536.0f / 360.0f))) /
                   NU_COS_LUT((i32)((fovy / 2.0f) * (65536.0f / 360.0f))); // 65536 / 360 angle units per degree
    mtx->m00 = 1.0f / (aspect * tanFovy2);
    mtx->m01 = 0.0f;
    mtx->m02 = 0.0f;
    mtx->m03 = 0.0f;
    mtx->m10 = 0.0f;
    mtx->m11 = 1.0f / tanFovy2;
    mtx->m12 = 0.0f;
    mtx->m13 = 0.0f;
    mtx->m20 = 0.0f;
    mtx->m21 = 0.0f;
    mtx->m22 = (zFar + zNear) / (zFar - zNear);
    mtx->m23 = 1.0f;
    mtx->m30 = 0.0f;
    mtx->m31 = 0.0f;
    mtx->m32 = (zFar * -2.0f * zNear) / (zFar - zNear);
    mtx->m33 = 0.0f;
}

void NuMtxSetFrustumD3D(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    mtx->m00 = (n + n) / (r - l);
    mtx->m01 = 0.0f;
    mtx->m02 = 0.0f;
    mtx->m03 = 0.0f;
    mtx->m10 = 0.0f;
    mtx->m11 = (n + n) / (t - b);
    mtx->m12 = 0.0f;
    mtx->m13 = 0.0f;
    mtx->m20 = (r + l) / (l - r);
    mtx->m21 = (t + b) / (b - t);
    mtx->m22 = f / (f - n);
    mtx->m23 = 1.0f;
    mtx->m30 = 0.0f;
    mtx->m31 = 0.0f;
    mtx->m32 = (-f * n) / (f - n);
    mtx->m33 = 0.0f;
}

void NuMtxSetFrustumBlend(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    mtx->m00 = (n + n) / (r - l);
    mtx->m01 = 0.0f;
    mtx->m02 = 0.0f;
    mtx->m03 = 0.0f;
    mtx->m10 = 0.0f;
    mtx->m11 = (n + n) / (t - b);
    mtx->m12 = 0.0f;
    mtx->m13 = 0.0f;
    mtx->m20 = (r + l) / (l - r);
    mtx->m21 = (t + b) / (b - t);
    mtx->m22 = (f + n) / (f - n);
    mtx->m23 = 1.0f;
    mtx->m30 = 0.0f;
    mtx->m31 = 0.0f;
    mtx->m32 = (f * -2.0f * n) / (f - n);
    mtx->m33 = 0.0f;
}

void NuMtxSetOrthoD3D(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    mtx->m00 = 2.0f / (r - l);
    mtx->m01 = 0.0f;
    mtx->m02 = 0.0f;
    mtx->m03 = 0.0f;
    mtx->m10 = 0.0f;
    mtx->m11 = 2.0f / (t - b);
    mtx->m12 = 0.0f;
    mtx->m13 = 0.0f;
    mtx->m20 = 0.0f;
    mtx->m21 = 0.0f;
    mtx->m22 = 1.0f / (f - n);
    mtx->m23 = 0.0f;
    mtx->m30 = (r + l) / (l - r);
    mtx->m31 = (t + b) / (b - t);
    mtx->m32 = (f + n) / (n - f);
    mtx->m33 = 1.0f;
}

void NuMtxSetOrthoBlend(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    mtx->m00 = 2.0f / (r - l);
    mtx->m01 = 0.0f;
    mtx->m02 = 0.0f;
    mtx->m03 = 0.0f;
    mtx->m10 = 0.0f;
    mtx->m11 = 2.0f / (t - b);
    mtx->m12 = 0.0f;
    mtx->m13 = 0.0f;
    mtx->m20 = 0.0f;
    mtx->m21 = 0.0f;
    mtx->m22 = 2.0f / (f - n);
    mtx->m23 = 0.0f;
    mtx->m30 = (r + l) / (l - r);
    mtx->m31 = (t + b) / (b - t);
    mtx->m32 = (f + n) / (n - f);
    mtx->m33 = 1.0f;
}

void NuMtxGetPerspectiveD3D(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar) {
    f32 Q = mtx->m22;
    *zNear = -mtx->m32 / Q;
    *zFar = (*zNear * Q) / (Q - 1.0f);
    *aspect = mtx->m11 / mtx->m00;
    *fovy = NuAtan2(1.0f / mtx->m11, 1.0f) * 360.0f / (f32)3.14159265358979323846;
}

void NuMtxGetPerspectivePS3(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar) {
    f32 Q = mtx->m22;
    f32 B = mtx->m32;

    *zNear = -B / (Q + 1.0f);
    *zFar = (*zNear * B) / ((*zNear + *zNear) + B);
    *aspect = mtx->m11 / mtx->m00;
    *fovy = NuAtan2(1.0f / mtx->m11, 1.0f) * 360.0f / (f32)3.14159265358979323846;
}

void NuMtxGetPerspectiveOGL(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar) {
    f32 depth = mtx->m22;
    *zNear = (depth + depth) / mtx->m32 - 1.0f;
    *zFar = (*zNear * depth) / (depth - 1.0f);
    *aspect = mtx->m11 / mtx->m00;
    *fovy = NuAtan2(1.0f / mtx->m11, 1.0f) * 360.0f / (f32)3.14159265358979323846;
}

void NuMtxGetPerspectiveBlend(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar) {
    f32 Q = (mtx->m22 + 1.0f) * 0.5f;
    *zNear = -(mtx->m32 * 0.5f) / Q;
    *zFar = (*zNear * Q) / (Q - 1.0f);
    *aspect = mtx->m11 / mtx->m00;
    *fovy = NuAtan2(1.0f / mtx->m11, 1.0f) * 360.0f / (f32)3.14159265358979323846;
}

void NuMtxGetFrustumD3D(NUMTX *mtx, f32 *l, f32 *r, f32 *b, f32 *t, f32 *n, f32 *f) {
    f32 Q = mtx->m22;
    f32 zNear = -mtx->m32 / Q;

    if (f != NULL) {
        *f = (Q * zNear) / (Q - 1.0f);
    }
    if (l != NULL) {
        *l = ((-1.0f - mtx->m20) * zNear) / mtx->m00;
    }
    if (r != NULL) {
        *r = ((1.0f - mtx->m20) * zNear) / mtx->m00;
    }
    if (b != NULL) {
        *b = ((-1.0f - mtx->m21) * zNear) / mtx->m11;
    }
    if (t != NULL) {
        *t = ((1.0f - mtx->m21) * zNear) / mtx->m11;
    }
    if (n != NULL) {
        *n = zNear;
    }
}

void NuMtxGetFrustumBlend(NUMTX *mtx, f32 *l, f32 *r, f32 *b, f32 *t, f32 *n, f32 *f) {
    f32 Q = (mtx->m22 + 1.0f) * 0.5f;
    f32 zNear = -(mtx->m32 * 0.5f) / Q;

    if (f != NULL) {
        *f = (Q * zNear) / (Q - 1.0f);
    }
    if (l != NULL) {
        *l = ((-1.0f - mtx->m20) * zNear) / mtx->m00;
    }
    if (r != NULL) {
        *r = ((1.0f - mtx->m20) * zNear) / mtx->m00;
    }
    if (b != NULL) {
        *b = ((-1.0f - mtx->m21) * zNear) / mtx->m11;
    }
    if (t != NULL) {
        *t = ((1.0f - mtx->m21) * zNear) / mtx->m11;
    }
    if (n != NULL) {
        *n = zNear;
    }
}

f32 NuFnabs(f32 f) {
    f32 rv;

    *(i32 *)&rv = *(i32 *)&f | 0x80000000;
    return rv;
}

f32 NuFneg(f32 v) {
    f32 rv;

    *(i32 *)&rv = *(i32 *)&v ^ 0x80000000;
    return rv;
}

f32 NuFsign(f32 f) {
    if (*(i32 *)&f < 0) {
        return -1.0f;
    } else {
        return 1.0f;
    }
}

i32 NuEquiv(f32 a, f32 b) {
    return NuFabs(a - b) < fetol;
}

void NuEquivTollerance(f32 tolerance) {
    fetol = tolerance;
}

/* Both exception controls are empty in the original Android binary. */
void NuFpExceptionMask(void) {
}

void NuFpException(void) {
}

f32 NuFloor(f32 f) {
    return (f32)(i32)f;
}

extern "C" {
    f32 NuCeil(f32 value) {
        f32 lower = NuFloor(value);
        return lower != value ? lower + 1.0f : value;
    }

}

f32 NuFmod(f32 a, f32 b) {
    return a - (i32)(a / b) * b;
}

void NuQuatFromEulerXYZ(NUQUAT *out, NUANG psi, NUANG theta, NUANG phi) {
    f32 sin_psi_over_2;
    f32 cos_psi_over_2;
    f32 sin_theta_over_2;
    f32 cos_theta_over_2;
    f32 sin_phi_over_2;
    f32 cos_phi_over_2;
    f32 sin_phi_times_sin_theta;
    f32 cos_phi_times_cos_theta;
    f32 sin_phi_times_cos_theta;
    f32 cos_phi_times_sin_theta;

    psi /= 2;
    theta /= 2;
    phi /= 2;

    cos_psi_over_2 = NU_COS_LUT(psi);
    sin_psi_over_2 = NU_SIN_LUT(psi);

    sin_theta_over_2 = NU_SIN_LUT(theta);
    cos_theta_over_2 = NU_COS_LUT(theta);

    cos_phi_over_2 = NU_COS_LUT(phi);
    sin_phi_over_2 = NU_SIN_LUT(phi);

    cos_phi_times_cos_theta = cos_phi_over_2 * cos_theta_over_2;
    sin_phi_times_sin_theta = sin_phi_over_2 * sin_theta_over_2;

    out->w = cos_phi_times_cos_theta * cos_psi_over_2 + sin_phi_times_sin_theta * sin_psi_over_2;
    out->x = cos_phi_times_cos_theta * sin_psi_over_2 - sin_phi_times_sin_theta * cos_psi_over_2;

    cos_phi_times_sin_theta = cos_phi_over_2 * sin_theta_over_2;
    sin_phi_times_cos_theta = sin_phi_over_2 * cos_theta_over_2;

    out->y = cos_phi_times_sin_theta * cos_psi_over_2 + sin_phi_times_cos_theta * sin_psi_over_2;
    out->z = sin_phi_times_cos_theta * cos_psi_over_2 - cos_phi_times_sin_theta * sin_psi_over_2;
}

void NuEulerXYZFromQuat(NUANG *x, NUANG *y, NUANG *z, NUQUAT *input) {
    NUQUAT q = *input;
    f32 sin_y = -2.0f * (q.x * q.z - q.w * q.y);
    if (sin_y < -1.0f) {
        sin_y = -1.0f;
    }
    if (sin_y > 1.0f) {
        sin_y = 1.0f;
    }

    *y = NuASin(sin_y);
    *x = NuAtan2D(2.0f * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
    *z = NuAtan2D(2.0f * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
}

void NuMtxToQuat(NUMTX *m, struct nuquat_s *out) {
    f32 s;
    i32 i;
    f32 trace;
    i32 j, k;
    i32 next[3] = {1, 2, 0};
    f32 q[4];
    trace = reinterpret_cast<f32 *>(m)[0] + reinterpret_cast<f32 *>(m)[5] + reinterpret_cast<f32 *>(m)[10];

    if (trace > 0.0f) {
        s = NuFsqrt(trace + 1.0f);
        out->w = s * 0.5f;
        s = 0.5f / s;
        out->x = (reinterpret_cast<f32 *>(m)[6] - reinterpret_cast<f32 *>(m)[9]) * s;
        out->y = (reinterpret_cast<f32 *>(m)[8] - reinterpret_cast<f32 *>(m)[2]) * s;
        out->z = (reinterpret_cast<f32 *>(m)[1] - reinterpret_cast<f32 *>(m)[4]) * s;
    } else {
        i = 0;
        if (reinterpret_cast<f32 *>(m)[5] > reinterpret_cast<f32 *>(m)[0])
            i = 1;
        if (reinterpret_cast<f32 *>(m)[10] > (&m->m00)[(i << 2) + i])
            i = 2;
        j = next[i];
        k = next[j];
        s = NuFsqrt(((&m->m00)[(i << 2) + i] - ((&m->m00)[(j << 2) + j] + (&m->m00)[(k << 2) + k])) + 1.0f);
        q[i] = s * 0.5f;
        if (s != 0.0f)
            s = 0.5f / s;
        q[3] = ((&m->m00)[(j << 2) + k] - (&m->m00)[(k << 2) + j]) * s;
        q[j] = ((&m->m00)[(i << 2) + j] + (&m->m00)[(j << 2) + i]) * s;
        q[k] = ((&m->m00)[(i << 2) + k] + (&m->m00)[(k << 2) + i]) * s;
        out->x = q[0];
        out->y = q[1];
        out->z = q[2];
        out->w = q[3];
    }
}

void NuQuatToMtx(NUQUAT *quat, NUMTX *out) {
    f32 w_sq;
    f32 x_sq;
    f32 y_sq;
    f32 z_sq;
    f32 xy_times_2;
    f32 xz_times_2;
    f32 xw_times_2;
    f32 yz_times_2;
    f32 yw_times_2;
    f32 zw_times_2;

    w_sq = quat->w * quat->w;
    x_sq = quat->x * quat->x;
    y_sq = quat->y * quat->y;
    z_sq = quat->z * quat->z;

    xy_times_2 = (quat->x * quat->y) + (quat->x * quat->y);
    xz_times_2 = (quat->x * quat->z) + (quat->x * quat->z);
    xw_times_2 = (quat->x * quat->w) + (quat->x * quat->w);
    yz_times_2 = (quat->y * quat->z) + (quat->y * quat->z);
    yw_times_2 = (quat->y * quat->w) + (quat->y * quat->w);
    zw_times_2 = (quat->z * quat->w) + (quat->z * quat->w);

    out->m00 = w_sq + x_sq - y_sq - z_sq;
    out->m10 = xy_times_2 - zw_times_2;
    out->m20 = xz_times_2 + yw_times_2;
    out->m30 = 0.0f;

    out->m01 = xy_times_2 + zw_times_2;
    out->m11 = w_sq - x_sq + y_sq - z_sq;
    out->m21 = yz_times_2 - xw_times_2;
    out->m31 = 0.0f;

    out->m02 = xz_times_2 - yw_times_2;
    out->m12 = yz_times_2 + xw_times_2;
    out->m22 = w_sq - x_sq - y_sq + z_sq;
    out->m32 = 0.0f;

    out->m03 = 0.0f;
    out->m13 = 0.0f;
    out->m23 = 0.0f;
    out->m33 = 1.0f;
}

void NuQuatAdd(NUQUAT *out, NUQUAT *q0, NUQUAT *q1) {
    out->w = q0->w + q1->w;
    out->x = q0->x + q1->x;
    out->y = q0->y + q1->y;
    out->z = q0->z + q1->z;
}

void NuQuatSub(NUQUAT *out, NUQUAT *q0, NUQUAT *q1) {
    out->w = q0->w - q1->w;
    out->x = q0->x - q1->x;
    out->y = q0->y - q1->y;
    out->z = q0->z - q1->z;
}

void NuQuatMul(NUQUAT *out, NUQUAT *q0, NUQUAT *q1) {
    NUQUAT result;
    result.w = q1->w * q0->w - q1->x * q0->x - q1->y * q0->y - q1->z * q0->z;
    result.x = q1->w * q0->x + q1->x * q0->w + q1->y * q0->z - q1->z * q0->y;
    result.y = q1->w * q0->y + q1->y * q0->w + q1->z * q0->x - q1->x * q0->z;
    out->z = q1->w * q0->z + q1->z * q0->w + q1->x * q0->y - q1->y * q0->x;
    out->x = result.x;
    out->y = result.y;
    out->w = result.w;
}

void NuQuatInv(NUQUAT *out, NUQUAT *quat) {
    f32 recip_sq;

    recip_sq = 1.0f / (quat->w * quat->w + quat->x * quat->x + quat->y * quat->y + quat->z * quat->z);

    out->w = quat->w * recip_sq;
    out->x = -quat->x * recip_sq;
    out->y = -quat->y * recip_sq;
    out->z = -quat->z * recip_sq;
}

void NuQuatNormalise(NUQUAT *out, NUQUAT *q) {
    f32 magnitude_squared = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
    if (magnitude_squared > 0.0f) {
        f32 inverse_magnitude = NuFdiv(1.0f, NuFsqrt(magnitude_squared));
        out->w = q->w * inverse_magnitude;
        out->x = q->x * inverse_magnitude;
        out->y = q->y * inverse_magnitude;
        out->z = q->z * inverse_magnitude;
    } else {
        *out = *q;
    }
}

f32 NuQuatMagnitude(NUQUAT *q) {
    return q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
}

void NuQuatHarmonize(NUQUAT *a, NUQUAT *b) {
    f32 dot = a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;

    if (dot < 0.0f) {
        b->x = -b->x;
        b->y = -b->y;
        b->z = -b->z;
        b->w = -b->w;
    }
}

void NuQuatLerp(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t) {
    out->x = (to->x - from->x) * t + from->x;
    out->y = (to->y - from->y) * t + from->y;
    out->z = (to->z - from->z) * t + from->z;
    out->w = (to->w - from->w) * t + from->w;
}

f32 NuCubicInterpolation(f32 m, f32 y0, f32 y1, f32 y2, f32 t) {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 t_cb;
    f32 t_sq;

    a = -m / 6.0f + y0 / 2.0f + -y1 / 2.0f + y2 / 6.0f;
    b = m / 2.0f - y0 + y1 / 2.0f;
    c = -m / 3.0f + -y0 / 2.0f + y1 + -y2 / 6.0f;
    d = y0;

    t_sq = t * t;
    t_cb = t * t_sq;

    return a * t_cb + b * t_sq + c * t + d;
}

void NuQuatCubicInt(NUQUAT *out, NUQUAT *m, NUQUAT *a, NUQUAT *b, NUQUAT *c, f32 t) {
    NuQuatHarmonize(m, a);
    NuQuatHarmonize(a, b);
    NuQuatHarmonize(b, c);

    out->x = NuCubicInterpolation(m->x, a->x, b->x, c->x, t);
    out->y = NuCubicInterpolation(m->y, a->y, b->y, c->y, t);
    out->z = NuCubicInterpolation(m->z, a->z, b->z, c->z, t);
    out->w = NuCubicInterpolation(m->w, a->w, b->w, c->w, t);
}

f32 NuHermiteInterpolation(f32 m, f32 y0, f32 y1, f32 y2, f32 t) {
    f32 m0;
    f32 m1;
    f32 t_sq;
    f32 t_cb;
    f32 h0;
    f32 h1;
    f32 h2;
    f32 h3;

    m0 = (y1 - m) / 2.0f;
    m1 = (y2 - y0) / 2.0f;

    t_sq = t * t;
    t_cb = t * t_sq;

    h0 = (t_cb + t_cb - t_sq * 3.0f) + 1.0f;
    h1 = t_cb - (t_sq + t_sq) + t;
    h2 = t_cb * -2.0f + t_sq * 3.0f;
    h3 = t_cb - t_sq;

    return h0 * y0 + h1 * m0 + h2 * y1 + h3 * m1;
}

void NuQuatHermiteInt(NUQUAT *out, NUQUAT *m, NUQUAT *q0, NUQUAT *q1, NUQUAT *q2, f32 t) {
    NuQuatHarmonize(m, q0);
    NuQuatHarmonize(q0, q1);
    NuQuatHarmonize(q1, q2);

    out->x = NuHermiteInterpolation(m->x, q0->x, q1->x, q2->x, t);
    out->y = NuHermiteInterpolation(m->y, q0->y, q1->y, q2->y, t);
    out->z = NuHermiteInterpolation(m->z, q0->z, q1->z, q2->z, t);
    out->w = NuHermiteInterpolation(m->w, q0->w, q1->w, q2->w, t);
}

void NuQuatSlerp(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t) {
    f32 _unused;
    NUQUAT to_prime;
    f32 scale;
    f32 omega;
    f32 sin_omega;
    f32 from_factor;
    f32 to_factor;

    // Even though this same number is used later in the function, the
    // assignment here has nothing to do with it.
    _unused = 1e-05f;

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

    if (1.0f - scale > 1e-05f) {
        omega = NuACos(scale);

        sin_omega = NU_SIN_LUT((i32)omega);
        from_factor = NuFdiv(NU_SIN_LUT((i32)((1.0f - t) * omega)), sin_omega);
        to_factor = NuFdiv(NU_SIN_LUT((i32)(t * omega)), sin_omega);
    } else {
        from_factor = 1.0f - t;
        to_factor = t;
    }

    out->x = from->x * from_factor + to_prime.x * to_factor;
    out->y = from->y * from_factor + to_prime.y * to_factor;
    out->z = from->z * from_factor + to_prime.z * to_factor;
    out->w = from->w * from_factor + to_prime.w * to_factor;
}

void NuQuatBlend(NUQUAT *out, NUQUAT *q0, NUQUAT *q1, f32 blendA, f32 blendB) {
    out->x = q0->x * blendA + q1->x * blendB;
    out->y = q0->y * blendA + q1->y * blendB;
    out->z = q0->z * blendA + q1->z * blendB;
    out->w = q0->w * blendA + q1->w * blendB;
}

void NuQuatSlerpFast(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t) {
    f32 threshold = 0.85f;
    f32 dot = from->x * to->x + from->y * to->y + from->z * to->z + from->w * to->w;
    NUQUAT negative;
    NUQUAT *target;
    if (dot < 0.0f) {
        dot = -dot;
        target = &negative;
        target->x = -to->x;
        target->y = -to->y;
        target->z = -to->z;
        target->w = -to->w;
    } else {
        target = to;
    }
    if (dot > threshold) {
        f32 inverse_t = 1.0f - t;
        out->x = from->x * inverse_t + target->x * t;
        out->y = from->y * inverse_t + target->y * t;
        out->z = from->z * inverse_t + target->z * t;
        out->w = from->w * inverse_t + target->w * t;
        NuQuatNormalise(out, out);
    } else {
        dot = dot < 0.995f ? dot : 0.995f;
        f32 a = 2.2184808254241943f + (dot * 0.223403662443161f) * dot;
        f32 b = dot * 2.4418842792510986f;
        f32 angle = 1.5707963705062866f + (NuFsqrt(a - b) - NuFsqrt(a + b)) + dot * 0.6391287446022034f;
        f32 inverse_sine = 1.0f / NuFsqrt(1.0f - dot * dot);
        f32 u = (1.0f - t) * angle - 1.5707963705062866f;
        u = u * u;
        f32 v = t * angle - 1.5707963705062866f;
        v = v * v;
        f32 sin_u = 0.9999999403953552f +
                    (((u * 2.3154014343163e-05f - 0.0013853708514943719f) * u + 0.04166358336806297f) * u -
                     0.4999990463256836f) *
                        u;
        f32 sin_v = 0.9999999403953552f +
                    (((v * 2.3154014343163e-05f - 0.0013853708514943719f) * v + 0.04166358336806297f) * v -
                     0.4999990463256836f) *
                        v;
        f32 from_factor = sin_u * inverse_sine;
        f32 to_factor = sin_v * inverse_sine;
        out->x = from->x * from_factor + target->x * to_factor;
        out->y = from->y * from_factor + target->y * to_factor;
        out->z = from->z * from_factor + target->z * to_factor;
        out->w = from->w * from_factor + target->w * to_factor;
    }
}

f32 NuQuatDot(NUQUAT *q0, NUQUAT *q1) {
    return q0->x * q1->x + q0->y * q1->y + q0->z * q1->z + q0->w * q1->w;
}

void NuQuatNeg2(NUQUAT *out, NUQUAT *in) {
    out->x = -in->x;
    out->y = -in->y;
    out->z = -in->z;
    out->w = -in->w;
}

/* The original calls the VU helpers in this math unit, rather than the
 * similarly named NuQuat helpers used by the
 * former game-utility copy. */
void VuQuatSlerpFast(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t) {
    f32 dot;
    f32 threshold = 0.85f;
    dot = VuQuatDot(from, to);
    NUQUAT target;
    if (dot < 0.0f) {
        dot = -dot;
        VuQuatNeg2(&target, to);
    } else {
        VuQuatCopy(&target, to);
    }

    if (dot > threshold) {
        VuQuatLerp(out, from, &target, t);
        VuQuatNormalise(out, out);
        return;
    }

    dot = dot < 0.995f ? dot : 0.995f;

    const f32 estimate = dot * 0.22340366f * dot + 2.2184808f;
    const f32 linear = dot * 2.4418843f;
    const f32 angle = NuFsqrt(estimate - linear) - NuFsqrt(estimate + linear) + 1.5707964f + dot * 0.63912874f;
    const f32 inverse_sine = 1.0f / NuFsqrt(1.0f - dot * dot);

    f32 from_angle = (1.0f - t) * angle - 1.5707964f;
    from_angle *= from_angle;
    f32 to_angle = t * angle - 1.5707964f;
    to_angle *= to_angle;

    const f32 from_poly =
        ((((from_angle * 2.3154014e-5f - 0.0013853709f) * from_angle + 0.041663583f) * from_angle - 0.49999905f) *
             from_angle +
         0.99999994f);
    const f32 to_poly =
        ((((to_angle * 2.3154014e-5f - 0.0013853709f) * to_angle + 0.041663583f) * to_angle - 0.49999905f) * to_angle +
         0.99999994f);
    const f32 from_weight = from_poly * inverse_sine;
    const f32 to_weight = to_poly * inverse_sine;
    VuQuatBlend(out, from, &target, from_weight, to_weight);
}

void NuPlnEqn(NUPLANE *out, NUVEC *pnt0, NUVEC *pnt1, NUVEC *pnt2) {
    NUVEC *pln;
    NUVEC v1_minus_0;
    NUVEC v2_minus_0;
    NUVEC normal;

    pln = (NUVEC *)out;

    NuVecSub(&v1_minus_0, pnt1, pnt0);
    NuVecSub(&v2_minus_0, pnt2, pnt0);

    NuVecCross(&normal, &v1_minus_0, &v2_minus_0);

    NuVecNorm(pln, &normal);

    out->d = -(out->a * pnt0->x + out->b * pnt0->y + out->c * pnt0->z);
}

void NuPlnEqnPn(NUPLANE *out, NUVEC *point, NUVEC *normal) {
    out->a = normal->x;
    out->b = normal->y;
    out->c = normal->z;
    out->d = -(out->a * point->x + out->b * point->y + out->c * point->z);
}

f32 NuPlnDist(NUPLANE *plane, NUVEC *point) {
    f32 distance = NuVecDot(point, (NUVEC *)plane) + plane->d;
    return distance;
}

f32 NuPlnDist2(NUPLANE *plane, NUVEC *a, NUVEC *b) {
    f32 da = NuVecDot(a, (NUVEC *)plane) + plane->d;
    f32 db = NuVecDot(b, (NUVEC *)plane) + plane->d;
    if (da < 0.0f && db < 0.0f)
        return da > db ? da : db;
    if (da > 0.0f && db > 0.0f)
        return da < db ? da : db;
    return 0.0f;
}

i32 NuPlnLine(NUPLANE *plane, NUVEC *s, NUVEC *e, NUVEC *out) {
    f32 dot_sp;
    f32 dot_ep;
    NUVEC dir;
    f32 inv_magnitude;
    i32 is_acute;

    dot_sp = NuVecDot(s, (NUVEC *)plane) + plane->d;
    dot_ep = NuVecDot(e, (NUVEC *)plane) + plane->d;

    if (NuFsign(dot_sp) != NuFsign(dot_ep)) {
        NuVecSub(&dir, e, s);

        inv_magnitude = -dot_sp / (dot_ep - dot_sp);
        NuVecScale(out, &dir, inv_magnitude);

        NuVecAdd(out, out, s);

        return 1;
    }

    return 0;
}

i32 NuPlnLine2(NUPLANE *plane, NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *start, NUVEC *end, NUVEC *out, f32 *distance,
               f32 *fraction) {
    if (NuPlnLine(plane, start, end, out)) {
        if (NuPtInPoly(out, a, b, c, plane)) {
            if (distance != NULL || fraction != NULL) {
                f32 hit_distance;
                f32 line_length;
                NUVEC delta;
                NUVEC line;
                NuVecSub(&delta, out, start);
                hit_distance = NuVecMag(&delta);
                if (distance != NULL)
                    *distance = hit_distance;
                if (fraction != NULL) {
                    NuVecSub(&line, end, start);
                    line_length = NuVecMag(&line);
                    *fraction = hit_distance / line_length;
                }
            }
            return 1;
        }
    }
    return 0;
}

f32 NuPlnLine3(NUPLANE *plane, NUVEC *pnt, NUVEC *v, NUVEC *out) {
    NUVEC *pln;
    f32 dist;
    f32 dot;

    NuVecNorm(v, v);

    // It's not entirely clear why the plane is stored in a separate variable,
    // or that this cast is what originally took place.
    pln = (NUVEC *)plane;
    dot = v->x * pln->x + v->y * pln->y + v->z * pln->z;

    dist = ((plane->a * -plane->d - pnt->x) * pln->x + (plane->b * -plane->d - pnt->y) * pln->y +
            (plane->c * -plane->d - pnt->z) * pln->z) /
           dot;

    out->x = pnt->x + v->x * dist;
    out->y = pnt->y + v->y * dist;
    out->z = pnt->z + v->z * dist;

    return dist;
}

i32 NuPtInPoly(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2, NUPLANE *plane) {
    f32 abs_a;
    f32 abs_b;
    f32 abs_c;
    i32 result;

    abs_a = NuFabs(plane->a);
    abs_b = NuFabs(plane->b);
    abs_c = NuFabs(plane->c);

    if (abs_a > abs_b) {
        if (abs_a > abs_c) {
            if (plane->a <= 0.0f) {
                result = NuPtInPolyYZ(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyZY(pnt, t0, t1, t2);
            }
        } else {
            if (plane->c <= 0.0f) {
                result = NuPtInPolyXY(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyYX(pnt, t0, t1, t2);
            }
        }
    } else {
        if (abs_b > abs_c) {
            if (plane->b <= 0.0f) {
                result = NuPtInPolyZX(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyXZ(pnt, t0, t1, t2);
            }
        } else {
            if (plane->c <= 0.0f) {
                result = NuPtInPolyXY(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyYX(pnt, t0, t1, t2);
            }
        }
    }

    return result;
}

i32 NuPtInPolyXY(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->x - t0->x) * (pnt->y - t1->y) - (pnt->x - t1->x) * (t1->y - t0->y) <= 0.0f &&
        (t2->x - t1->x) * (pnt->y - t2->y) - (pnt->x - t2->x) * (t2->y - t1->y) <= 0.0f &&
        (t0->x - t2->x) * (pnt->y - t0->y) - (pnt->x - t0->x) * (t0->y - t2->y) <= 0.0f) {
        if ((t0->x != t1->x || t0->y != t1->y) && (t0->x != t2->x || t0->y != t2->y) &&
            (t1->x != t2->x || t1->y != t2->y)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyYX(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->y - t0->y) * (pnt->x - t1->x) - (pnt->y - t1->y) * (t1->x - t0->x) <= 0.0f &&
        (t2->y - t1->y) * (pnt->x - t2->x) - (pnt->y - t2->y) * (t2->x - t1->x) <= 0.0f &&
        (t0->y - t2->y) * (pnt->x - t0->x) - (pnt->y - t0->y) * (t0->x - t2->x) <= 0.0f) {
        if ((t0->y != t1->y || t0->x != t1->x) && (t0->y != t2->y || t0->x != t2->x) &&
            (t1->y != t2->y || t1->x != t2->x)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyXZ(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->x - t0->x) * (pnt->z - t1->z) - (pnt->x - t1->x) * (t1->z - t0->z) <= 0.0f &&
        (t2->x - t1->x) * (pnt->z - t2->z) - (pnt->x - t2->x) * (t2->z - t1->z) <= 0.0f &&
        (t0->x - t2->x) * (pnt->z - t0->z) - (pnt->x - t0->x) * (t0->z - t2->z) <= 0.0f) {
        if ((t0->x != t1->x || t0->z != t1->z) && (t0->x != t2->x || t0->z != t2->z) &&
            (t1->x != t2->x || t1->z != t2->z)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyZX(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->z - t0->z) * (pnt->x - t1->x) - (pnt->z - t1->z) * (t1->x - t0->x) <= 0.0f &&
        (t2->z - t1->z) * (pnt->x - t2->x) - (pnt->z - t2->z) * (t2->x - t1->x) <= 0.0f &&
        (t0->z - t2->z) * (pnt->x - t0->x) - (pnt->z - t0->z) * (t0->x - t2->x) <= 0.0f) {
        if ((t0->z != t1->z || t0->x != t1->x) && (t0->z != t2->z || t0->x != t2->x) &&
            (t1->z != t2->z || t1->x != t2->x)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyYZ(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->y - t0->y) * (pnt->z - t1->z) - (pnt->y - t1->y) * (t1->z - t0->z) <= 0.0f &&
        (t2->y - t1->y) * (pnt->z - t2->z) - (pnt->y - t2->y) * (t2->z - t1->z) <= 0.0f &&
        (t0->y - t2->y) * (pnt->z - t0->z) - (pnt->y - t0->y) * (t0->z - t2->z) <= 0.0f) {
        if ((t0->y != t1->y || t0->z != t1->z) && (t0->y != t2->y || t0->z != t2->z) &&
            (t1->y != t2->y || t1->z != t2->z)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyZY(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->z - t0->z) * (pnt->y - t1->y) - (pnt->z - t1->z) * (t1->y - t0->y) <= 0.0f &&
        (t2->z - t1->z) * (pnt->y - t2->y) - (pnt->z - t2->z) * (t2->y - t1->y) <= 0.0f &&
        (t0->z - t2->z) * (pnt->y - t0->y) - (pnt->z - t0->z) * (t0->y - t2->y) <= 0.0f) {
        if ((t0->z != t1->z || t0->y != t1->y) && (t0->z != t2->z || t0->y != t2->y) &&
            (t1->z != t2->z || t1->y != t2->y)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPlnPlnIntersect(NUPLANE *first, NUPLANE *second, NUVEC *point, NUVEC *direction) {
    f32 cross_y = first->c * second->a - first->a * second->c;
    f32 cross_z = first->a * second->b - first->b * second->a;
    NUVEC normal;
    normal.x = first->b * second->c - first->c * second->b;
    normal.y = cross_y;
    normal.z = cross_z;
    NuVecNorm(&normal, &normal);
    *direction = normal;
    normal.x = NuFabs(normal.x);
    normal.y = NuFabs(normal.y);
    normal.z = NuFabs(normal.z);
    if (normal.z >= normal.x && normal.z >= normal.y) {
        if (NuFabs(second->a) < 0.0001f) {
            point->y = (first->d * second->a / first->a - second->d) / (-second->a * first->b / first->a + second->b);
            point->x = (-first->d - first->b * point->y) / first->a;
            point->z = 0.0f;
        } else {
            point->y = (second->d * first->a / second->a - first->d) / (-first->a * second->b / second->a + first->b);
            point->x = (-second->d - second->b * point->y) / second->a;
            point->z = 0.0f;
        }
    } else if (normal.y >= normal.x && normal.y >= normal.z) {
        if (NuFabs(second->a) < 0.0001f) {
            point->z = (first->d * second->a / first->a - second->d) / (-second->a * first->c / first->a + second->c);
            point->x = (-first->d - first->c * point->z) / first->a;
            point->y = 0.0f;
        } else {
            point->z = (second->d * first->a / second->a - first->d) / (-first->a * second->c / second->a + first->c);
            point->x = (-second->d - second->c * point->z) / second->a;
            point->y = 0.0f;
        }
    } else {
        if (NuFabs(second->b) < 0.0001f) {
            point->z = (first->d * second->b / first->b - second->d) / (-second->b * first->c / first->b + second->c);
            point->y = (-first->d - first->c * point->z) / first->b;
            point->x = 0.0f;
        } else {
            point->z = (second->d * first->b / second->b - first->d) / (-first->b * second->c / second->b + first->c);
            point->y = (-second->d - second->c * point->z) / second->b;
            point->x = 0.0f;
        }
    }
    // The original evaluates this residual but returns zero on both paths.
    if (first->a * point->x + first->b * point->y + first->c * point->z + first->d > 0.0001f) {
        return 0;
    }
    return 0;
}

f32 NuLineToPointDistSqrEx(NUVEC *s, NUVEC *e, NUVEC *pnt, NUVEC *out) {
    NUVEC s_to_e;
    f32 len;
    NUVEC pnt_to_s;
    f32 dot;
    NUVEC nearest;
    f32 dist;
    NUVEC dir;

    NuVecSub(&s_to_e, e, s);
    len = NuVecMag(&s_to_e);

    NuVecScale(&s_to_e, &s_to_e, 1.0f / len);

    NuVecSub(&pnt_to_s, pnt, s);
    dot = NuVecDot(&s_to_e, &pnt_to_s);

    if (dot <= 0.0f) {
        nearest = *s;
    } else if (dot >= len) {
        nearest = *e;
    } else {
        NuVecScale(&nearest, &s_to_e, dot);
        NuVecAdd(&nearest, &nearest, s);
    }

    dist = NuVecDistSqr(&nearest, pnt, &dir);

    if (out != NULL) {
        *out = nearest;
    }

    return dist;
}

f32 NuLineToPointDistSqr(NUVEC *start, NUVEC *end, NUVEC *point) {
    return NuLineToPointDistSqrEx(start, end, point, NULL);
}

f32 NuInfiniteLineToPointDistSqrEx(NUVEC *start, NUVEC *end, NUVEC *point, NUVEC *closest) {
    f32 length;
    f32 projection;
    f32 distance;
    NUVEC direction;
    NUVEC relative;
    NUVEC projected;
    NUVEC delta;
    NuVecSub(&direction, end, start);
    length = NuVecMag(&direction);
    NuVecScale(&direction, &direction, 1.0f / length);
    NuVecSub(&relative, point, start);
    projection = NuVecDot(&direction, &relative);
    NuVecScale(&projected, &direction, projection);
    NuVecAdd(&projected, &projected, start);
    distance = NuVecDistSqr(&projected, point, &delta);
    if (closest != NULL)
        *closest = projected;
    return distance;
}

f32 NuInfiniteLineToPointDistSqr(NUVEC *start, NUVEC *end, NUVEC *point) {
    return NuInfiniteLineToPointDistSqrEx(start, end, point, NULL);
}

// Original: 196 bytes.
i32 NuPointRelToBoundingBox(NUVEC *point, NUVEC *maximum, NUVEC *minimum) {
    i32 result = 0;
    if (point->x >= maximum->x)
        result |= 1;
    else if (point->x <= minimum->x)
        result |= 8;
    if (point->y >= maximum->y)
        result |= 2;
    else if (point->y <= minimum->y)
        result |= 16;
    if (point->z >= maximum->z)
        result |= 4;
    else if (point->z <= minimum->z)
        result |= 32;
    return result;
}

// Original: 204 bytes.
void NuClipXPlane(NUVEC *out, NUVEC *point, NUVEC *direction, f32 *plane) {
    if (plane != NULL) {
        out->x = *plane;
    } else {
        out->x = 0.0f;
    }
    f32 delta = out->x - point->x;
    out->y = point->y + (direction->y * delta) / direction->x;
    out->z = point->z + (direction->z * delta) / direction->x;
}

// Original: 207 bytes.
void NuClipYPlane(NUVEC *out, NUVEC *point, NUVEC *direction, f32 *plane) {
    if (plane != NULL) {
        out->y = *plane;
    } else {
        out->y = 0.0f;
    }
    f32 delta = out->y - point->y;
    out->x = point->x + (direction->x * delta) / direction->y;
    out->z = point->z + (direction->z * delta) / direction->y;
}

void NuClipZPlane(NUVEC *out, NUVEC *point, NUVEC *direction, f32 *plane) {
    if (plane != NULL) {
        out->z = *plane;
    } else {
        out->z = 0.0f;
    }
    f32 delta = out->z - point->z;
    out->x = point->x + (direction->x * delta) / direction->z;
    out->y = point->y + (direction->y * delta) / direction->z;
}

f32 NuLineToLineDist(NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *d) {
    f32 distance;
    NUVEC offset;
    NUVEC first;
    NUVEC second;
    NUVEC between;
    NUVEC normal;
    NuVecSub(&offset, c, a);
    NuVecSub(&first, b, a);
    NuVecSub(&second, d, c);
    NuVecSub(&between, b, c);
    NuVecCross(&normal, &first, &second);
    distance = NuVecDot(&offset, &normal);
    distance /= NuVecMag(&normal);
    return distance;
}

void NuRandSeed(u32 seed) {
    fseed = seed;
}

u32 NuRandGetSeed(void) {
    return fseed;
}

f32 NuRandFloatSeeded(u32 *seed) {
    union {
        u32 int_val;
        f32 float_val;
    } value;

    *seed = *seed * 0x019660d + 0x3c6ef35f;
    value.int_val = *seed & 0x07fffff | 0x3f800000;

    return value.float_val - 1.0f;
}

f32 NuRandFloat() {
    return NuRandFloatSeeded(&fseed);
}

u32 NuRandIntSeeded(u32 *seed) {
    *seed = *seed * 0x019660d + 0x3c6ef35f;

    return *seed;
}

u32 NuRandInt() {
    return NuRandIntSeeded(&fseed);
}

extern "C" {
    u32 NuWindRand(void) {
        NuWindQS = (NuWindQS * 0x24cd + 1) & 0xffff;
        return NuWindQS;
    }

}

void NuRandSetSeed(NURAND *rand, i32 seed) {
    if (rand == NULL) {
        rand = &global_rand;
    }

    rand->value = seed;
}

i32 NuRand(NURAND *rand) {
    i32 value;

    if (rand != NULL) {
        if (rand->value == 0) {
            rand->value = 1;
        }
    } else {
        rand = &global_rand;
    }

    rand->value ^= 0x075bd924;

    value = rand->value / 0x1f31d;
    rand->value = (rand->value + value * -0x1f31d) * 0x41a7 + value * -0xb14;

    if (rand->value < 0) {
        rand->value += 0x7fffffff;
    }

    rand->value ^= 0x075bd924;

    return rand->value;
}

f32 NuFloatRand(NURAND *rand) {
    return NuRand(rand) / 2.1474836e+09f;
}

extern "C" {
    void NuCubicToBez3(f32 *cubic, f32 *bezier) {
        bezier[0] = cubic[3];
        bezier[1] = cubic[2] / 3.0f;
        bezier[2] = (cubic[1] + 2.0f * cubic[2]) / 3.0f;
        // The original writes offset 0x10, leaving element 3 untouched.
        bezier[4] = cubic[0] + cubic[1] + cubic[2] - cubic[3];
    }

    void NuBez3ToCubic(f32 *bezier, f32 *cubic) {
        cubic[0] = bezier[0] + 3.0f * bezier[1] - 3.0f * bezier[2] + bezier[3];
        cubic[1] = 3.0f * bezier[2] - 6.0f * bezier[1];
        // These repeated writes to element 0 are present in the original.
        cubic[0] = 3.0f * bezier[1];
        cubic[0] = bezier[0];
    }

    void NuBez3Subdiv(f32 *control, f32 t, f32 *left, f32 *right) {
        f32 remaining = 1.0f - t;
        f32 middle;
        left[0] = control[0];
        left[1] = control[0] * remaining + control[1] * t;
        middle = control[1] * remaining + control[2] * t;
        left[2] = left[1] * remaining + middle * t;
        right[3] = control[3];
        right[2] = control[2] * remaining + control[3] * t;
        right[1] = middle * remaining + right[2] * t;
        left[3] = right[0] = left[2] * remaining + right[1] * t;
    }

    NUVEC NuBezierCubicPatchEvaluate(NUVEC *control, f32 u, f32 v) {
        u32 i, j;
        f32 remaining_u = 1.0f - u;
        f32 remaining_v = 1.0f - v;
        f32 weights_u[4], weights_v[4];
        NUVEC result;
        weights_u[0] = remaining_u * remaining_u * remaining_u;
        weights_u[1] = 3.0f * remaining_u * remaining_u * u;
        weights_u[2] = 3.0f * remaining_u * u * u;
        weights_u[3] = u * u * u;
        weights_v[0] = remaining_v * remaining_v * remaining_v;
        weights_v[1] = 3.0f * remaining_v * remaining_v * v;
        weights_v[2] = 3.0f * remaining_v * v * v;
        weights_v[3] = v * v * v;
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                NuVecScaleAccum(&result, control + i + j * 4, weights_u[i] * weights_v[j]);
            }
        }
        return result;
    }

    NUVEC NuBezierCubicPatchEvaluatePartials(NUVEC *control, f32 u, f32 v, i32 order_u, i32 order_v) {
        u32 i, j;
        f32 remaining_u = 1.0f - u;
        f32 remaining_v = 1.0f - v;
        f32 weights_u[4], weights_v[4];
        NUVEC result;
        switch (order_u) {
            case 0:
                weights_u[0] = remaining_u * remaining_u * remaining_u;
                weights_u[1] = 3.0f * remaining_u * remaining_u * u;
                weights_u[2] = 3.0f * remaining_u * u * u;
                weights_u[3] = u * u * u;
                break;
            case 1:
                weights_u[0] = -3.0f * remaining_u * remaining_u;
                weights_u[1] = 9.0f * u * u - 9.0f;
                weights_u[2] = 6.0f * u - 9.0f * u * u;
                weights_u[3] = 3.0f * u * u;
                break;
            case 2:
                weights_u[0] = 6.0f - 6.0f * u;
                weights_u[1] = 18.0f * u - 12.0f;
                weights_u[2] = 6.0f - 18.0f * u;
                weights_u[3] = 6.0f * u;
                break;
            case 3:
                weights_u[0] = -6.0f;
                weights_u[1] = 18.0f;
                weights_u[2] = -18.0f;
                weights_u[3] = 6.0f;
                break;
        }
        switch (order_v) {
            case 0:
                weights_v[0] = remaining_v * remaining_v * remaining_v;
                weights_v[1] = 3.0f * remaining_v * remaining_v * v;
                weights_v[2] = 3.0f * remaining_v * v * v;
                weights_v[3] = v * v * v;
                break;
            case 1:
                weights_v[0] = -3.0f * remaining_v * remaining_v;
                weights_v[1] = 9.0f * v * v - 9.0f;
                weights_v[2] = 6.0f * v - 9.0f * v * v;
                weights_v[3] = 3.0f * v * v;
                break;
            case 2:
                weights_v[0] = 6.0f - 6.0f * v;
                weights_v[1] = 18.0f * v - 12.0f;
                weights_v[2] = 6.0f - 18.0f * v;
                weights_v[3] = 6.0f * v;
                break;
            case 3:
                weights_v[0] = -6.0f;
                weights_v[1] = 18.0f;
                weights_v[2] = -18.0f;
                weights_v[3] = 6.0f;
                break;
        }
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                NuVecScaleAccum(&result, control + i + j * 4, weights_u[i] * weights_v[j]);
            }
        }
        return result;
    }

    void NuBezierCubicPatchPartialsU(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], -3.0f);
        NuVecScaleAccum(out0, &control[1], 3.0f);
        NuVecScale(out1, &control[12], -3.0f);
        NuVecScaleAccum(out1, &control[13], 3.0f);
        NuVecScale(out2, &control[2], -3.0f);
        NuVecScaleAccum(out2, &control[3], 3.0f);
        NuVecScale(out3, &control[14], -3.0f);
        NuVecScaleAccum(out3, &control[15], 3.0f);
    }

    void NuBezierCubicPatchPartialsV(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], -3.0f);
        NuVecScaleAccum(out0, &control[4], 3.0f);
        NuVecScale(out1, &control[8], -3.0f);
        NuVecScaleAccum(out1, &control[12], 3.0f);
        NuVecScale(out2, &control[3], -3.0f);
        NuVecScaleAccum(out2, &control[7], 3.0f);
        NuVecScale(out3, &control[11], -3.0f);
        NuVecScaleAccum(out3, &control[15], 3.0f);
    }

    void NuBezierCubicPatchPartialsUUVV(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], 36.0f);
        NuVecScaleAccum(out0, &control[1], -72.0f);
        NuVecScaleAccum(out0, &control[2], 36.0f);
        NuVecScaleAccum(out0, &control[4], -72.0f);
        NuVecScaleAccum(out0, &control[5], 144.0f);
        NuVecScaleAccum(out0, &control[6], -72.0f);
        NuVecScaleAccum(out0, &control[8], 36.0f);
        NuVecScaleAccum(out0, &control[9], -72.0f);
        NuVecScaleAccum(out0, &control[10], 36.0f);
        NuVecScale(out1, &control[4], 36.0f);
        NuVecScaleAccum(out1, &control[5], -72.0f);
        NuVecScaleAccum(out1, &control[6], 36.0f);
        NuVecScaleAccum(out1, &control[8], -72.0f);
        NuVecScaleAccum(out1, &control[9], 144.0f);
        NuVecScaleAccum(out1, &control[10], -72.0f);
        NuVecScaleAccum(out1, &control[12], 36.0f);
        NuVecScaleAccum(out1, &control[13], -72.0f);
        NuVecScaleAccum(out1, &control[14], 36.0f);
        NuVecScale(out2, &control[1], 36.0f);
        NuVecScaleAccum(out2, &control[2], -72.0f);
        NuVecScaleAccum(out2, &control[3], 36.0f);
        NuVecScaleAccum(out2, &control[5], -72.0f);
        NuVecScaleAccum(out2, &control[6], 144.0f);
        NuVecScaleAccum(out2, &control[7], -72.0f);
        NuVecScaleAccum(out2, &control[9], 36.0f);
        NuVecScaleAccum(out2, &control[10], -72.0f);
        NuVecScaleAccum(out2, &control[11], 36.0f);
        NuVecScale(out3, &control[5], 36.0f);
        NuVecScaleAccum(out3, &control[6], -72.0f);
        NuVecScaleAccum(out3, &control[7], 36.0f);
        NuVecScaleAccum(out3, &control[9], -72.0f);
        NuVecScaleAccum(out3, &control[10], 144.0f);
        NuVecScaleAccum(out3, &control[11], -72.0f);
        NuVecScaleAccum(out3, &control[13], 36.0f);
        NuVecScaleAccum(out3, &control[14], -72.0f);
        NuVecScaleAccum(out3, &control[15], 36.0f);
    }

    void NuBezierCubicPatchPartialsUUV(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], -18.0f);
        NuVecScaleAccum(out0, &control[1], 36.0f);
        NuVecScaleAccum(out0, &control[2], -18.0f);
        NuVecScaleAccum(out0, &control[4], 18.0f);
        NuVecScaleAccum(out0, &control[5], -36.0f);
        NuVecScaleAccum(out0, &control[6], 18.0f);
        NuVecScale(out1, &control[8], -18.0f);
        NuVecScaleAccum(out1, &control[9], 36.0f);
        NuVecScaleAccum(out1, &control[10], -18.0f);
        NuVecScaleAccum(out1, &control[12], 18.0f);
        NuVecScaleAccum(out1, &control[13], -36.0f);
        NuVecScaleAccum(out1, &control[14], 18.0f);
        NuVecScale(out2, &control[1], -18.0f);
        NuVecScaleAccum(out2, &control[2], 36.0f);
        NuVecScaleAccum(out2, &control[3], -18.0f);
        NuVecScaleAccum(out2, &control[5], 18.0f);
        NuVecScaleAccum(out2, &control[6], -36.0f);
        NuVecScaleAccum(out2, &control[7], 18.0f);
        NuVecScale(out3, &control[9], -18.0f);
        NuVecScaleAccum(out3, &control[10], 36.0f);
        NuVecScaleAccum(out3, &control[11], -18.0f);
        NuVecScaleAccum(out3, &control[13], 18.0f);
        NuVecScaleAccum(out3, &control[14], -36.0f);
        NuVecScaleAccum(out3, &control[15], 18.0f);
    }

    void NuBezierCubicPatchPartialsUVV(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], -18.0f);
        NuVecScaleAccum(out0, &control[1], 18.0f);
        NuVecScaleAccum(out0, &control[4], 36.0f);
        NuVecScaleAccum(out0, &control[5], -36.0f);
        NuVecScaleAccum(out0, &control[8], -18.0f);
        NuVecScaleAccum(out0, &control[9], 18.0f);
        NuVecScale(out1, &control[4], -18.0f);
        NuVecScaleAccum(out1, &control[5], 18.0f);
        NuVecScaleAccum(out1, &control[8], 36.0f);
        NuVecScaleAccum(out1, &control[9], -36.0f);
        NuVecScaleAccum(out1, &control[12], -18.0f);
        NuVecScaleAccum(out1, &control[13], 18.0f);
        NuVecScale(out2, &control[2], -18.0f);
        NuVecScaleAccum(out2, &control[3], 18.0f);
        NuVecScaleAccum(out2, &control[6], 36.0f);
        NuVecScaleAccum(out2, &control[7], -36.0f);
        NuVecScaleAccum(out2, &control[10], -18.0f);
        NuVecScaleAccum(out2, &control[11], 18.0f);
        NuVecScale(out3, &control[6], -18.0f);
        NuVecScaleAccum(out3, &control[7], 18.0f);
        NuVecScaleAccum(out3, &control[10], 36.0f);
        NuVecScaleAccum(out3, &control[11], -36.0f);
        NuVecScaleAccum(out3, &control[14], -18.0f);
        NuVecScaleAccum(out3, &control[15], 18.0f);
    }

    void NuBezierCubicPatchPartialsUUVVV(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], -36.0f);
        NuVecScaleAccum(out0, &control[1], 72.0f);
        NuVecScaleAccum(out0, &control[2], -36.0f);
        NuVecScaleAccum(out0, &control[4], 108.0f);
        NuVecScaleAccum(out0, &control[5], -216.0f);
        NuVecScaleAccum(out0, &control[6], 108.0f);
        NuVecScaleAccum(out0, &control[8], -108.0f);
        NuVecScaleAccum(out0, &control[9], 216.0f);
        NuVecScaleAccum(out0, &control[10], -108.0f);
        NuVecScaleAccum(out0, &control[12], 36.0f);
        NuVecScaleAccum(out0, &control[13], -72.0f);
        NuVecScaleAccum(out0, &control[14], 36.0f);
        *out1 = *out0;
        NuVecScale(out3, &control[1], -36.0f);
        NuVecScaleAccum(out3, &control[2], 72.0f);
        NuVecScaleAccum(out3, &control[3], -36.0f);
        NuVecScaleAccum(out3, &control[5], 108.0f);
        NuVecScaleAccum(out3, &control[6], -216.0f);
        NuVecScaleAccum(out3, &control[7], 108.0f);
        NuVecScaleAccum(out3, &control[9], -108.0f);
        NuVecScaleAccum(out3, &control[10], 216.0f);
        NuVecScaleAccum(out3, &control[11], -108.0f);
        NuVecScaleAccum(out3, &control[13], 36.0f);
        NuVecScaleAccum(out3, &control[14], -72.0f);
        NuVecScaleAccum(out3, &control[15], 36.0f);
        *out2 = *out3;
    }

    void NuBezierCubicPatchPartialsVV(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], 6.0f);
        NuVecScaleAccum(out0, &control[4], -12.0f);
        NuVecScaleAccum(out0, &control[8], 6.0f);
        NuVecScale(out1, &control[4], 6.0f);
        NuVecScaleAccum(out1, &control[8], -12.0f);
        NuVecScaleAccum(out1, &control[12], 6.0f);
        NuVecScale(out2, &control[3], 6.0f);
        NuVecScaleAccum(out2, &control[7], -12.0f);
        NuVecScaleAccum(out2, &control[11], 6.0f);
        NuVecScale(out3, &control[7], 6.0f);
        NuVecScaleAccum(out3, &control[11], -12.0f);
        NuVecScaleAccum(out3, &control[15], 6.0f);
    }

    void NuBezierCubicPatchPartialsVVV(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], -6.0f);
        NuVecScaleAccum(out0, &control[4], 18.0f);
        NuVecScaleAccum(out0, &control[8], -18.0f);
        NuVecScaleAccum(out0, &control[12], 6.0f);
        *out1 = *out0;
        NuVecScale(out3, &control[3], -6.0f);
        NuVecScaleAccum(out3, &control[7], 18.0f);
        NuVecScaleAccum(out3, &control[11], -18.0f);
        NuVecScaleAccum(out3, &control[15], 6.0f);
        *out2 = *out3;
    }

    void NuBezierCubicPatchPartialsUU(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], 6.0f);
        NuVecScaleAccum(out0, &control[1], -12.0f);
        NuVecScaleAccum(out0, &control[2], 6.0f);
        NuVecScale(out1, &control[12], 6.0f);
        NuVecScaleAccum(out1, &control[13], -12.0f);
        NuVecScaleAccum(out1, &control[14], 6.0f);
        NuVecScale(out2, &control[1], 6.0f);
        NuVecScaleAccum(out2, &control[2], -12.0f);
        NuVecScaleAccum(out2, &control[3], 6.0f);
        NuVecScale(out3, &control[13], 6.0f);
        NuVecScaleAccum(out3, &control[14], -12.0f);
        NuVecScaleAccum(out3, &control[15], 6.0f);
    }

    void NuBezierCubicPatchPartialsUUU(NUVEC *control, NUVEC *out0, NUVEC *out1, NUVEC *out2, NUVEC *out3) {
        NuVecScale(out0, &control[0], -6.0f);
        NuVecScaleAccum(out0, &control[1], 18.0f);
        NuVecScaleAccum(out0, &control[2], -18.0f);
        NuVecScaleAccum(out0, &control[3], 6.0f);
        *out2 = *out0;
        NuVecScale(out3, &control[12], -6.0f);
        NuVecScaleAccum(out3, &control[13], 18.0f);
        NuVecScaleAccum(out3, &control[14], -18.0f);
        NuVecScaleAccum(out3, &control[15], 6.0f);
        *out1 = *out3;
    }

    NUVEC NuBezierQuadraticTriangleEvaluateBarycentric(NUVEC *control, f32 u, f32 v, f32 w) {
        NUVEC result;
        NuVecScale(&result, &control[0], u * u);
        NuVecScaleAccum(&result, &control[1], 2.0f * u * v);
        NuVecScaleAccum(&result, &control[2], 2.0f * u * w);
        NuVecScaleAccum(&result, &control[3], v * v);
        NuVecScaleAccum(&result, &control[4], 2.0f * v * w);
        NuVecScaleAccum(&result, &control[5], w * w);
        return result;
    }

    NUVEC NuBezierQuadraticTriangleEvaluateParametric(NUVEC *control, f32 u, f32 v) {
        f32 w = 1.0f - u - v;
        NUVEC result;
        NuVecScale(&result, &control[0], v * v);
        NuVecScaleAccum(&result, &control[1], 2.0f * v * w);
        NuVecScaleAccum(&result, &control[2], 2.0f * u * v);
        NuVecScaleAccum(&result, &control[3], w * w);
        NuVecScaleAccum(&result, &control[4], 2.0f * w * u);
        NuVecScaleAccum(&result, &control[5], u * u);
        return result;
    }

    NUVEC NuBezierQuadraticTrianglePartialsUU(NUVEC *control) {
        NUVEC result;
        NuVecScale(&result, &control[3], 2.0f);
        NuVecScaleAccum(&result, &control[4], -4.0f);
        NuVecScaleAccum(&result, &control[5], 2.0f);
        return result;
    }

    NUVEC NuBezierQuadraticTrianglePartialsVV(NUVEC *control) {
        NUVEC result;
        NuVecScale(&result, &control[0], 2.0f);
        NuVecScaleAccum(&result, &control[1], -4.0f);
        NuVecScaleAccum(&result, &control[3], 2.0f);
        return result;
    }

    NUVEC NuBezierQuadraticTrianglePartialsUeV(NUVEC *control) {
        NUVEC result;
        NuVecScale(&result, &control[0], 2.0f);
        NuVecScaleAccum(&result, &control[2], -4.0f);
        NuVecScaleAccum(&result, &control[5], 2.0f);
        return result;
    }

}

f32 NuBez3EvaluateX(NUVEC4 *control, f32 t) {
    NUVEC4 powers;
    NUVEC4 coefficients;
    NUMTX basis = {-1.0f, 3.0f, -3.0f, 1.0f, 3.0f, -6.0f, 3.0f, 0.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    powers.x = t * t * t;
    powers.y = t * t;
    powers.z = t;
    powers.w = 1.0f;
    NuVec4MtxTransformH(&coefficients, control, &basis);
    return NuVec4Dot(&coefficients, &powers);
}

extern "C" {
    void nugraphInit(nugraph_s *graph) {
        graph->interpolation = 1;
        graph->point_count = 2;
        graph->x[0] = 0.0f;
        graph->y[0] = 0.0f;
        graph->x[1] = 1.0f;
        graph->y[1] = 1.0f;
        graph->x_scale = 1.0f;
        graph->y_scale = 1.0f;
        graph->x_extent = 10.0f;
        graph->y_extent = 10.0f;
    }

    i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y) {
        i32 index;
        i32 insert_index = 0;
        if (graph == NULL) {
            return 0;
        }
        if (graph->point_count > 7) {
            return 0;
        }
        if (graph->point_count <= 1) {
            return 0;
        }

        for (index = 0; index < graph->point_count; ++index) {
            if (graph->x[index] == x) {
                return 0;
            }
            if (graph->x[index] > x) {
                insert_index = index;
                break;
            }
        }

        for (index = graph->point_count; index >= insert_index; --index) {
            graph->x[index] = graph->x[index - 1];
            graph->y[index] = graph->y[index - 1];
        }
        ++graph->point_count;
        graph->x[insert_index] = x;
        graph->y[insert_index] = y;
        return 1;
    }

    i32 nugraphDeletePoint(nugraph_s *graph, i32 index) {
        if (graph == NULL) {
            return 0;
        }
        if (graph->point_count - 1 <= index) {
            return 0;
        }
        if (index <= 0) {
            return 0;
        }

        --graph->point_count;
        for (i32 i = index; i < graph->point_count; ++i) {
            graph->x[i] = graph->x[i + 1];
            graph->y[i] = graph->y[i + 1];
        }
        return 1;
    }

}

f32 nugraphGetXatIndex(nugraph_s *graph, i32 index) {
    if (graph == NULL) {
        return 0.0f;
    }
    if (index >= 0 && graph->point_count > index) {
        return graph->x[index];
    }
    if (index == -1) {
        return graph->x[0] + (graph->x[0] - graph->x[1]);
    }
    if (graph->point_count == index) {
        return graph->x[graph->point_count] + (graph->x[graph->point_count] - graph->x[graph->point_count - 1]);
    }
    return 0.0f;
}

f32 nugraphGetYatIndex(nugraph_s *graph, i32 index) {
    if (graph == NULL) {
        return 0.0f;
    }
    if (index >= 0 && graph->point_count > index) {
        return graph->y[index];
    }
    if (index == -1) {
        return graph->y[0] + (graph->y[0] - graph->y[1]);
    }
    if (graph->point_count == index) {
        return graph->y[graph->point_count] + (graph->y[graph->point_count] - graph->y[graph->point_count - 1]);
    }
    return 0.0f;
}

f32 nugraph_blend(i32 interval, i32 degree, i32 *knots, f32 parameter) {
    f32 result;
    if (degree == 1) {
        if ((f32)knots[interval] <= parameter && parameter < (f32)knots[interval + 1]) {
            result = 1.0f;
        } else {
            result = 0.0f;
        }
    } else {
        if (knots[interval + degree - 1] == knots[interval] && knots[interval + degree] == knots[interval + 1]) {
            result = 0.0f;
        } else if (knots[interval + degree - 1] == knots[interval]) {
            result = ((f32)knots[interval + degree] - parameter) /
                     (f32)(knots[interval + degree] - knots[interval + 1]) *
                     nugraph_blend(interval + 1, degree - 1, knots, parameter);
        } else if (knots[interval + degree] == knots[interval + 1]) {
            result = (parameter - (f32)knots[interval]) / (f32)(knots[interval + degree - 1] - knots[interval]) *
                     nugraph_blend(interval, degree - 1, knots, parameter);
        } else {
            result = (parameter - (f32)knots[interval]) / (f32)(knots[interval + degree - 1] - knots[interval]) *
                         nugraph_blend(interval, degree - 1, knots, parameter) +
                     ((f32)knots[interval + degree] - parameter) /
                         (f32)(knots[interval + degree] - knots[interval + 1]) *
                         nugraph_blend(interval + 1, degree - 1, knots, parameter);
        }
    }
    return result;
}

void nugraph_compute_point(i32 *knots, i32 segment_count, i32 degree, f32 parameter, nuvec_s *control_points,
                           nuvec_s *point) {
    point->x = 0.0f;
    point->y = 0.0f;
    point->z = 0.0f;
    for (i32 i = 0; i <= segment_count; ++i) {
        f32 weight = nugraph_blend(i, degree, knots, parameter);
        point->x += control_points[i].x * weight;
        point->y += control_points[i].y * weight;
    }
}

void nugraph_compute_catmull_point(i32 segment_count, f32 parameter, nuvec_s *control_points, nuvec_s *point) {
    if (segment_count <= 0) {
        return;
    }

    nuvec_s first;
    first.x = control_points[0].x + (control_points[0].x - control_points[1].x) * 0.1f;
    first.y = control_points[0].y + (control_points[0].y - control_points[1].y) * 0.1f;

    nuvec_s last;
    last.x = control_points[segment_count].x +
             (control_points[segment_count].x - control_points[segment_count - 1].x) * 0.1f;
    last.y = control_points[segment_count].y +
             (control_points[segment_count].y - control_points[segment_count - 1].y) * 0.1f;

    i32 segment = 1;
    nuvec_s *p0;
    nuvec_s *p3;
    f32 start;
    f32 fraction;
    nuvec_s *p1;
    nuvec_s *p2;
    for (; segment <= segment_count; ++segment) {
        if ((f32)segment / (f32)segment_count > parameter) {
            break;
        }
    }

    start = (f32)(segment - 1) / (f32)segment_count;
    fraction = parameter - start;
    fraction *= (f32)segment_count;

    if (segment == 1)
        p0 = &first;
    else
        p0 = &control_points[segment - 2];
    p1 = &control_points[segment - 1];
    p2 = &control_points[segment];
    if (segment >= segment_count)
        p3 = &last;
    else
        p3 = &control_points[segment + 1];

    point->x = 0.5f * (2.0f * p1->x + (p2->x - p0->x) * fraction +
                       (2.0f * p0->x - 5.0f * p1->x + 4.0f * p2->x - p3->x) * (fraction * fraction) +
                       (3.0f * p1->x - p0->x - 3.0f * p2->x + p3->x) * (fraction * fraction * fraction));
    point->y = 0.5f * (2.0f * p1->y + (p2->y - p0->y) * fraction +
                       (2.0f * p0->y - 5.0f * p1->y + 4.0f * p2->y - p3->y) * (fraction * fraction) +
                       (3.0f * p1->y - p0->y - 3.0f * p2->y + p3->y) * (fraction * fraction * fraction));
    point->z = 0.0f;
}

void nugraph_compute_linear_point(i32 segment_count, f32 parameter, nuvec_s *control_points, nuvec_s *point) {
    i32 segment = 1;
    for (; segment <= segment_count; ++segment) {
        if ((f32)segment / (f32)segment_count > parameter) {
            break;
        }
    }

    f32 start = (f32)(segment - 1) / (f32)segment_count;
    f32 fraction = parameter - start;
    fraction *= (f32)segment_count;
    point->x = control_points[segment - 1].x + (control_points[segment].x - control_points[segment - 1].x) * fraction;
    point->y = control_points[segment - 1].y + (control_points[segment].y - control_points[segment - 1].y) * fraction;
    point->z = 0.0f;
}

void nugraph_compute_intervals(i32 *knots, i32 segment_count, i32 degree) {
    for (i32 i = 0; i <= segment_count + degree; ++i) {
        if (i < degree) {
            knots[i] = 0;
        } else if (degree <= i && i <= segment_count) {
            knots[i] = i - degree + 1;
        } else if (i > segment_count) {
            knots[i] = segment_count - degree + 2;
        }
    }
}

void nugraph_bspline(i32 segment_count, i32 degree, nuvec_s *control_points, nuvec_s *output, i32 output_count) {
    i32 knots[16];
    nugraph_compute_intervals(knots, segment_count, degree);
    f32 increment = (f32)(segment_count - degree + 2) / (f32)(output_count - 1);
    f32 parameter = 0.0f;
    for (i32 i = 0; i < output_count - 1; ++i) {
        nuvec_s point;
        nugraph_compute_point(knots, segment_count, degree, parameter, control_points, &point);
        output[i].x = point.x;
        output[i].y = point.y;
        output[i].z = point.z;
        parameter += increment;
    }
    output[output_count - 1].x = control_points[segment_count].x;
    output[output_count - 1].y = control_points[segment_count].y;
    output[output_count - 1].z = control_points[segment_count].z;
}

void nugraph_catmullrom(i32 segment_count, nuvec_s *control_points, nuvec_s *output, i32 output_count) {
    f32 increment = 1.0f / (f32)(output_count - 1);
    f32 parameter = 0.0f;
    for (i32 i = 0; i < output_count - 1; ++i) {
        nuvec_s point;
        nugraph_compute_catmull_point(segment_count, parameter, control_points, &point);
        output[i].x = point.x;
        output[i].y = point.y;
        output[i].z = point.z;
        parameter += increment;
    }
    output[output_count - 1].x = control_points[segment_count].x;
    output[output_count - 1].y = control_points[segment_count].y;
    output[output_count - 1].z = control_points[segment_count].z;
}

void nugraph_linear(i32 segment_count, nuvec_s *control_points, nuvec_s *output, i32 output_count) {
    f32 increment = 1.0f / (f32)(output_count - 1);
    f32 parameter = 0.0f;
    for (i32 i = 0; i < output_count - 1; ++i) {
        nuvec_s point;
        nugraph_compute_linear_point(segment_count, parameter, control_points, &point);
        output[i].x = point.x;
        output[i].y = point.y;
        output[i].z = point.z;
        parameter += increment;
    }
    output[output_count - 1].x = control_points[segment_count].x;
    output[output_count - 1].y = control_points[segment_count].y;
    output[output_count - 1].z = control_points[segment_count].z;
}

extern "C" {
    void nugraphFreeTempCurveData() {
        if (curve != NULL) {
            NU_FREE(curve);
        }
        curve = NULL;
        if (control != NULL) {
            NU_FREE(control);
        }
        control = NULL;
    }

    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count) {
        if (graph == NULL) {
            return 0;
        }

        nugraphFreeTempCurveData();
        if (curve == NULL) {
            curve = static_cast<nuvec_s *>(NU_ALLOC(100 * sizeof(nuvec_s), 4, 1, "", 0));
        }
        if (control == NULL) {
            control = static_cast<nuvec_s *>(NU_ALLOC(8 * sizeof(nuvec_s), 4, 1, "", 0));
        }
        if (curve == NULL || control == NULL) {
            return 0;
        }

        if (point_count > 100) {
            point_count = 100;
        }
        curve_points = point_count;

        for (i32 i = 0; i < graph->point_count; ++i) {
            control[i].x = graph->x[i];
            control[i].y = graph->y[i];
            control[i].z = 0.0f;
        }

        switch (graph->interpolation) {
            case 0:
                nugraph_linear(graph->point_count - 1, control, curve, curve_points);
                break;
            case 1:
                nugraph_catmullrom(graph->point_count - 1, control, curve, curve_points);
                break;
            case 2:
                nugraph_bspline(graph->point_count - 1, 3, control, curve, curve_points);
                break;
        }
        return 1;
    }

    f32 nugraphGetXatT(nugraph_s *, f32 t) {
        t *= (f32)curve_points;
        i32 index = (i32)(t + 0.5f);
        if (index >= curve_points) {
            index = curve_points - 1;
        }
        if (curve == NULL) {
            return 0.0f;
        }
        return curve[index].x;
    }

    f32 nugraphGetYatT(nugraph_s *, f32 t) {
        t *= (f32)curve_points;
        i32 index = (i32)(t + 0.5f);
        if (index >= curve_points) {
            index = curve_points - 1;
        }
        if (curve == NULL) {
            return 0.0f;
        }
        return curve[index].y;
    }

    f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 iterations) {
        f32 result;
        f32 current_x = 0.0f;
        f32 upper_parameter = 1.0f;
        f32 lower_parameter = 0.0f;
        f32 lower_x = 0.0f;
        f32 upper_x = 1.0f;
        f32 bound_y = 0.0f;
        f32 parameter = 0.0f;
        f32 parameter_squared = 0.0f;
        f32 step = 0.5f;
        i32 last_index = graph->point_count - 1;
        nuvec_s p0;
        nuvec_s p1;
        nuvec_s p2;
        nuvec_s p3;

        x /= graph->x_extent * graph->x_scale;
        x -= graph->x_offset;
        if (x > 1.0f) {
            x = 1.0f;
        }
        if (x < 0.0f) {
            x = 0.0f;
        }
        if (iterations <= 0) {
            iterations = 3;
        }

        for (i32 segment = 1; segment < graph->point_count; ++segment) {
            if (x <= graph->x[segment]) {
                switch (graph->interpolation) {
                    case 0: {
                        p1.x = graph->x[segment - 1];
                        p1.y = graph->y[segment - 1];
                        p2.x = graph->x[segment];
                        p2.y = graph->y[segment];
                        f32 fraction = (x - p1.x) / (p2.x - p1.x);
                        result = p1.y + (p2.y - p1.y) * fraction;
                        return result;
                    }

                    case 1: {
                        if (segment > 1) {
                            p0.x = graph->x[segment - 2];
                            p0.y = graph->y[segment - 2];
                        } else {
                            p0.x = graph->x[0] + (graph->x[0] - graph->x[1]) * 0.1f;
                            p0.y = graph->y[0] + (graph->y[0] - graph->y[1]) * 0.1f;
                        }
                        p1.x = graph->x[segment - 1];
                        p1.y = graph->y[segment - 1];
                        p2.x = graph->x[segment];
                        p2.y = graph->y[segment];
                        if (segment <= graph->point_count - 2) {
                            p3.x = graph->x[segment + 1];
                            p3.y = graph->y[segment + 1];
                        } else {
                            p3.x = graph->x[last_index] + (graph->x[last_index] - graph->x[last_index - 1]) * 0.1f;
                            p3.y = graph->y[last_index] + (graph->y[last_index] - graph->y[last_index - 1]) * 0.1f;
                        }

                        lower_x = p1.x;
                        upper_x = p2.x;
                        current_x = p1.x;
                        for (i32 i = 0; i < iterations; ++i) {
                            if (x > current_x) {
                                lower_parameter = parameter;
                                lower_x = current_x;
                                parameter += step;
                            } else {
                                upper_parameter = parameter;
                                upper_x = current_x;
                                parameter -= step;
                            }
                            step *= 0.5f;
                            parameter_squared = parameter * parameter;
                            current_x =
                                0.5f * (2.0f * p1.x + (p2.x - p0.x) * parameter +
                                        (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * parameter_squared +
                                        (3.0f * p1.x - p0.x - 3.0f * p2.x + p3.x) * (parameter * parameter_squared));
                        }

                        f32 current_y =
                            0.5f * (2.0f * p1.y + (p2.y - p0.y) * parameter +
                                    (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * parameter_squared +
                                    (3.0f * p1.y - p0.y - 3.0f * p2.y + p3.y) * (parameter * parameter_squared));
                        if (x > current_x) {
                            bound_y = 0.5f * (2.0f * p1.y + (p2.y - p0.y) * upper_parameter +
                                              (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) *
                                                  (upper_parameter * upper_parameter) +
                                              (3.0f * p1.y - p0.y - 3.0f * p2.y + p3.y) *
                                                  (upper_parameter * upper_parameter * upper_parameter));
                            result = current_y + (bound_y - current_y) * (x - current_x) / (upper_x - current_x);
                        } else {
                            bound_y = 0.5f * (2.0f * p1.y + (p2.y - p0.y) * lower_parameter +
                                              (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) *
                                                  (lower_parameter * lower_parameter) +
                                              (3.0f * p1.y - p0.y - 3.0f * p2.y + p3.y) *
                                                  (lower_parameter * lower_parameter * lower_parameter));
                            result = current_y + (bound_y - current_y) * (x - current_x) / (lower_x - current_x);
                        }
                        return result;
                    }

                    case 2:
                        return 0.0f;
                }
            }
        }
        return 0.0f;
    }

    f32 nugraphGetYatXScaled(nugraph_s *graph, f32 x, i32 iterations) {
        return graph->y_extent * graph->y_scale * nugraphGetYatX(graph, x, iterations);
    }

    i32 nugraphGenerateLookupTable(nugraph_s *graph, f32 *table, i32 table_count) {
        i32 i;
        f32 x0;
        f32 x1;
        f32 fraction;
        f32 parameter = 0.0f;
        f32 table_step = 1.0f / (f32)(table_count - 1);
        f32 curve_step = 1.0f / 99.0f;

        nugraphCalcCurve(graph, 100);
        for (i = 0; i < table_count; ++i) {
            f32 x = (f32)i * table_step;
            x0 = nugraphGetXatT(graph, parameter);
            x1 = nugraphGetXatT(graph, parameter + curve_step);
            while (x >= x1) {
                parameter += curve_step;
                x0 = x1;
                x1 = nugraphGetXatT(graph, parameter + curve_step);
            }

            if (x0 == x1) {
                fraction = 0.0f;
            } else {
                fraction = (x - x0) / (x1 - x0);
            }

            f32 y0 = nugraphGetYatT(graph, parameter);
            f32 y1 = nugraphGetYatT(graph, parameter + curve_step);
            table[i] = y0 + (y1 - y0) * fraction;
            if (table[i] > 1.0f) {
                table[i] = 1.0f;
            }
            table[i] *= graph->y_extent;
        }
        return 0;
    }

    i32 nugraphFileSave(nugraph_s *graph, char *filename) {
        i32 version = 1;
        f32 table[100];
        nugraphGenerateLookupTable(graph, table, 100);
        NUFILE file = NuFileOpen(filename, NUFILE_WRITE);
        if (file != 0) {
            NuFileWrite(file, &version, sizeof(version));
            NuFileWrite(file, graph, sizeof(*graph));
            NuFileWrite(file, table, sizeof(table));
            NuFileClose(file);
            return 1;
        }
        return 0;
    }

    i32 nugraphFileSaveGraphOnly(nugraph_s *graph, char *filename) {
        i32 result = 0;
        NUFILE file = NuFileOpen(filename, NUFILE_WRITE);
        if (file != 0) {
            result = nugraphFileSaveGraphOnlyToFile(graph, file);
            NuFileClose(file);
        }
        return result;
    }

    i32 nugraphFileLoadGraphOnly(nugraph_s *graph, char *filename) {
        i32 result = 0;
        NUFILE file = NuFileOpen(filename, NUFILE_READ);
        if (file != 0) {
            result = nugraphFileLoadGraphOnlyToFile(graph, file);
            NuFileClose(file);
        }
        return result;
    }

    i32 nugraphFileSaveGraphOnlyToFile(nugraph_s *graph, NUFILE file) {
        i32 version = 2;
        if (file != 0) {
            NuFileWrite(file, &version, sizeof(version));
            NuFileWrite(file, graph, sizeof(*graph));
            return 1;
        }
        return 0;
    }

    i32 nugraphFileLoadGraphOnlyToFile(nugraph_s *graph, NUFILE file) {
        i32 version = 0;
        if (file != 0) {
            NuFileRead(file, &version, sizeof(version));
            if (version <= 1) {
                legacy_nugraph_s legacy;
                NuFileRead(file, &legacy, sizeof(legacy));
                graph->x_extent = legacy.x_extent;
                graph->y_extent = legacy.y_extent;
                graph->x_offset = legacy.x_offset;
                graph->y_offset = legacy.y_offset;
                graph->x_scale = legacy.x_scale;
                graph->y_scale = legacy.y_scale;
                graph->interpolation = legacy.interpolation;
                graph->point_count = legacy.point_count;
                for (i32 i = 0; i < 8; ++i) {
                    graph->x[i] = legacy.x[i];
                    graph->y[i] = legacy.y[i];
                }
            } else {
                NuFileRead(file, graph, sizeof(*graph));
            }
            return 1;
        }
        return 0;
    }

    i32 nugraphFileLoadGraph(char *filename, VARIPTR *buffer, VARIPTR *buffer_end) {
        i32 loaded;
        i32 version = 0;
        VARIPTR start = *buffer;
        loaded = NuFileLoadBuffer(filename, buffer->void_ptr, buffer_end->addr - buffer->addr);
        if (loaded != 0) {
            memmove(&version, buffer->void_ptr, sizeof(version));
            buffer->addr += sizeof(version);
            memmove(start.void_ptr, buffer->void_ptr, sizeof(nugraph_s));
            buffer->addr += sizeof(nugraph_s) - sizeof(version);
            return 1;
        }
        return 0;
    }

    i32 nugraphFileLoadTable(char *filename, VARIPTR *buffer, VARIPTR *buffer_end) {
        i32 loaded;
        i32 version = 0;
        VARIPTR start = *buffer;
        loaded = NuFileLoadBuffer(filename, buffer->void_ptr, buffer_end->addr - buffer->addr);
        if (loaded != 0) {
            memmove(&version, buffer->void_ptr, sizeof(version));
            buffer->addr += sizeof(version);
            buffer->addr += sizeof(nugraph_s);
            memmove(start.void_ptr, buffer->void_ptr, 100 * sizeof(f32));
            buffer->addr = start.addr + 100 * sizeof(f32);
            return 1;
        }
        return 0;
    }

}

void NuTrigInit(void) {
    for (i32 i = 0; (unsigned char)(i <= NUTRIGTABLE_COUNT - 1); i++) {
        NuTrigTable[i] = sin(i * NUTRIGTABLE_INTERVAL);
    }
}

static f32 NuSinApprox3(i32 angle) {
    f32 x;
    angle &= 0xffff;
    if (angle > 0xc000)
        angle -= 0xc000;
    else if (angle > 0x4000)
        angle = 0xc000 - angle;
    else
        angle += 0x4000;
    x = angle * 9.58738019107841e-05f;
    x -= 1.5707963705062866f;
    return x + -0.16666656732559204f * (x * x * x) + (x * x * x) * (x * x) * 0.008333025500178337f +
           (x * x * x) * (x * x) * (x * x) * -0.00019807413627859205f +
           (x * x * x) * (x * x) * (x * x) * (x * x) * 2.601886990305502e-06f;
}

float NuSinApprox2(i32 ang) {
    return NuSinApprox3(ang);
}

float NuCosApprox2(i32 ang) {
    return NuSinApprox3(ang + 0x4000);
}

static u16 xy(u32 dx, u32 dy) {
    if (dx > dy) {
        return NUANG_90DEG - NU_ATAN2_LUT(dy, dx);
    } else {
        return NU_ATAN2_LUT(dx, dy);
    }
}

i32 NuAtani(i32 dx, i32 dy) {
    if (dx == 0) {
        return dy < 0 ? 0x8000 : 0;
    } else if (dy == 0) {
        return dx < 0 ? 0xc000 : 0x4000;
    } else if (dx < 0) {
        if (dy < 0)
            return xy(-dx, -dy) + 0x8000;
        return -xy(-dx, dy);
    } else if (dy < 0) {
        return 0x8000 - xy(dx, -dy);
    }
    return xy(dx, dy);
}

static f32 fxy(u32 dx, u32 dy) {
    if (dx > dy)
        return (0x4000 - (i32)NU_ATAN2_LUT(dy, dx)) * 0.000095873802492f;
    return (u32)NU_ATAN2_LUT(dx, dy) * 0.000095873802492f;
}

f32 NuAtanf(i32 dx, i32 dy) {
    if (dx == 0) {
        return dy < 0 ? 3.1415927410125732f : 0.0f;
    } else if (dy == 0) {
        return dx < 0 ? 4.71238899230957f : 1.5707963705062866f;
    } else if (dx < 0) {
        if (dy < 0)
            return fxy(-dx, -dy) + 3.1415927410125732f;
        return -fxy(-dx, dy);
    } else if (dy < 0) {
        return 3.1415927410125732f - fxy(dx, -dy);
    }
    return fxy(dx, dy);
}

f32 NuAtan2(f32 dx, f32 dy) {
    return NuAtan2D(dx, dy) * 0.000095873802492f;
}

static u16 fxyd(f32 dx, f32 dy) {
    if (dx > dy) {
        return NUANG_90DEG - NU_ATAN2F_LUT(dy, dx);
    } else {
        return NU_ATAN2F_LUT(dx, dy);
    }
}

static u16 fxyda(f32 dx, f32 dy) {
    f32 fraction;
    i32 index;
    f32 result;
    if (dx > dy) {
        fraction = dy * 512.0f / dx;
        index = (i32)fraction;
        fraction -= index;
        result = (0x4000 - (i32)ang[index]) * (1.0f - fraction);
        result += (0x4000 - (i32)ang[index + 1]) * fraction;
        return result;
    } else if (dx == dy) {
        return ang[512];
    } else {
        fraction = dx * 512.0f / dy;
        index = (i32)fraction;
        fraction -= index;
        result = (u32)ang[index] * (1.0f - fraction);
        result += (u32)ang[index + 1] * fraction;
        return result;
    }
}

static f32 fxydaf(f32 dx, f32 dy) {
    f32 fraction;
    i32 index;
    f32 result;
    if (dx > dy) {
        fraction = dy * 512.0f / dx;
        index = (i32)fraction;
        fraction -= index;
        result = (0x4000 - (i32)ang[index]) * (1.0f - fraction);
        result += (0x4000 - (i32)ang[index + 1]) * fraction;
        return result;
    } else if (dx == dy) {
        return (u32)ang[512];
    } else {
        fraction = dx * 512.0f / dy;
        index = (i32)fraction;
        fraction -= index;
        result = (u32)ang[index] * (1.0f - fraction);
        result += (u32)ang[index + 1] * fraction;
        return result;
    }
}

i32 NuAtan2D(f32 dx, f32 dy) {
    if (dx == 0.0f) {
        return 0.0f > dy ? NUANG_180DEG : 0;
    } else if (dy == 0.0f) {
        return 0.0f > dx ? NUANG_270DEG : NUANG_90DEG;
    } else if (0.0f > dx) {
        if (0.0f > dy) {
            return fxyd(-dx, -dy) + NUANG_180DEG;
        } else {
            return -fxyd(-dx, dy);
        }
    } else if (0.0f > dy) {
        return NUANG_180DEG - fxyd(dx, -dy);
    } else {
        return fxyd(dx, dy);
    }
}

i32 NuAtan2DA(f32 dx, f32 dy) {
    if (dx == 0.0f) {
        return 0.0f > dy ? 0x8000 : 0;
    } else if (dy == 0.0f) {
        return 0.0f > dx ? 0xc000 : 0x4000;
    } else if (0.0f > dx) {
        if (0.0f > dy)
            return fxyda(-dx, -dy) + 0x8000;
        return -fxyda(-dx, dy);
    } else if (0.0f > dy) {
        return 0x8000 - fxyda(dx, -dy);
    }
    return fxyda(dx, dy);
}

f32 NuAtan2DAF(f32 dx, f32 dy) {
    if (dx == 0.0f) {
        return 0.0f > dy ? 32768.0f : 0.0f;
    } else if (dy == 0.0f) {
        return 0.0f > dx ? -16384.0f : 16384.0f;
    } else if (0.0f > dx) {
        if (0.0f > dy)
            return fxydaf(-dx, -dy) - 32768.0f;
        return -fxydaf(-dx, dy);
    } else if (0.0f > dy) {
        return 32768.0f - fxydaf(dx, -dy);
    }
    return fxydaf(dx, dy);
}

NUANG NuAngAdd(NUANG a, NUANG b) {
    NUANG sum = (a + b) & 0xffff;
    if (sum > 0x7fff) {
        sum -= 0x10000;
    }
    return sum;
}

NUANG NuAngSub(NUANG a, NUANG b) {
    NUANG difference = (a - b) & 0xffff;
    if (difference > 0x7fff) {
        difference -= 0x10000;
    }
    return difference;
}

// The original's register hints change its codegen; C++17 hosts reject the keyword.
#ifdef __ANDROID__
#define NU_POWER2_REGISTER register
#else
#define NU_POWER2_REGISTER
#endif
i32 NuPower2(i32 value) {
    NU_POWER2_REGISTER i32 power;
    NU_POWER2_REGISTER i32 input;
    if (value > 127) {
        power = 128;
    } else {
        power = 1;
    }
    input = value;
    while (power < input)
        power += power;
    return power;
}

void NuAng2AltSol(NUANG *out_x, NUANG *out_y, NUANG *out_z, NUANG x, NUANG y, NUANG z) {
    *out_x = x + 0x8000;
    *out_y = -0x8000 - y;
    *out_z = z + 0x8000;
}

f32 NuLog2(f32 value) {
    return NuLog10(value) * 3.321928f;
}

f32 NuPow(f32 x, f32 y) {
    if (x == 0.0f)
        return 0.0f;
    static i32 first_time = 1;
    if (first_time != 0) {
        first_time = 0;
        for (i32 i = 0; i < 32; ++i)
            pow_x[i] = FLT_MAX;
    }
    for (i32 i = 0; i < 32; ++i) {
        if (pow_x[i] == x && pow_y[i] == y)
            return pow_rv[i];
    }
    f32 result = (f32)exp((double)y * log((double)x));
    pow_x[pow_cache_free] = x;
    pow_y[pow_cache_free] = y;
    pow_rv[pow_cache_free] = result;
    pow_cache_free = (pow_cache_free + 1) & 31;
    return result;
}

f32 NuPowFast(f32 base, f32 exponent) {
    if (base == 0.0) {
        return 0.0;
    }

    f32 result;
    result = NuExp10(NuLog10(base) * exponent);
    return result;
}

extern "C" {
    f32 NuLog10(f32 x) {
        f32 exponent;
        f32 i;
        f32 position;
        i32 index;
        f32 fraction;
        static i32 first_time = 1;
        static f32 *logtable;
        if (first_time) {
            logtable = (f32 *)NU_ALLOC(9000 * sizeof(f32), 4, 1, "", 0);
            if (!logtable)
                return 0.0f;
            for (i = 1000.0f; i <= 9999.0f; i += 1.0f)
                logtable[(i32)i - 1000] = log10(i / 1000.0f);
            first_time = 0;
        }
        if (x <= 0.0f)
            return FLT_MAX;
        exponent = 3.0f;
        if (x >= 10000.0f) {
            while (x > 10000.0f) {
                x *= 0.1f;
                exponent += 1.0f;
            }
        } else if (x < 1000.0f) {
            while (x < 1000.0f) {
                x *= 10.0f;
                exponent -= 1.0f;
            }
        }
        position = x - 1000.0f;
        index = (i32)position;
        fraction = position - index;
        if (index < 8999)
            return logtable[index + 1] * fraction + logtable[index] * (1.0f - fraction) + exponent;
        return logtable[index] + exponent;
    }

    f32 NuExp10(f32 x) {
        f32 result;
        i32 exponent;
        f32 i;
        f32 position;
        i32 index;
        f32 fraction;
        static i32 first_time = 1;
        static f32 *alogtable;
        if (first_time) {
            alogtable = (f32 *)NU_ALLOC(10000 * sizeof(f32), 4, 1, "", 0);
            if (!alogtable)
                return 0.0f;
            for (i = 0.0f; i <= 9999.0f; i += 1.0f)
                alogtable[(i32)i] = pow(10.0, i / 10000.0f);
            first_time = 0;
        }
        if (x == FLT_MAX)
            return FLT_MAX;
        exponent = (i32)x;
        x -= exponent;
        if (x < 0.0f) {
            --exponent;
            x += 1.0f;
        }
        position = x * 10000.0f;
        index = (i32)position;
        fraction = position - index;
        if (index < 9999)
            result = alogtable[index + 1] * fraction + alogtable[index] * (1.0f - fraction);
        else
            result = alogtable[index];
        if (exponent > 0) {
            while (exponent > 0) {
                result *= 10.0f;
                --exponent;
            }
        } else if (exponent < 0) {
            while (exponent < 0) {
                result *= 0.1f;
                ++exponent;
            }
        }
        return result;
    }

    i32 NuMiscNextPow2(i32 value) {
        i32 shift = 0;
        while ((1 << shift) < value)
            ++shift;
        return 1 << shift;
    }

    i32 NuMiscPow2Exp(i32 value) {
        i32 shift = 0;
        while (value > 1) {
            value >>= 1;
            ++shift;
        }
        return shift;
    }

    i32 NuMiscNextPow2Exp(i32 value) {
        i32 shift = 0;
        while ((1 << shift) < value)
            ++shift;
        return shift;
    }

}

f32 NuSinf(f32 angle) {
    f32 magnitude = NuFabs(angle);
    f32 sign = NuFsel(angle, 1.0f, -1.0f);
    f32 turns = NuTrunc(magnitude * 0.31830987334251404f + 0.5f);
    f32 parity = NuTrunc(turns * 0.5f) - turns * 0.5f;
    f32 reduced = magnitude - turns * 3.1415927410125732f;
    f32 square = reduced * reduced;
    f32 polynomial = (((NUTRIG_SC3 * square + NUTRIG_SC2) * square + NUTRIG_SC1) * square + NUTRIG_SC0) * square;
    sign = parity * sign * 4.0f + sign;
    return (reduced * polynomial + reduced) * sign;
}

f32 NuCosf(f32 angle) {
    return NuSinf(angle + 1.5707963705062866f);
}

