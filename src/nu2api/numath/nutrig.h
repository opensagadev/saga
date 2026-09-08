#pragma once

#include <math.h>

#include "nu2api/nucore/common.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"

#ifdef __cplusplus
static inline i16 NuASin(f32 sin) {
    f32 abs;
    f32 sqrt;
    f32 unknown_a;
    f32 unknown_b;
    f32 unknown_c;
    f32 unknown_d;

    abs = NuFabs(sin);
    sqrt = NuFsqrt(1.0f - sin * sin);

    unknown_a = MIN(sqrt, abs);

    unknown_b = MAX((MIN((abs - 0.70710677f) * 3.40282e+38f, 1.0f)), -1.0f);

    unknown_c = MIN(sin * 3.40282e+38f, 1.0f);
    unknown_c = MAX(unknown_c, -1.0f);

    unknown_d = unknown_b * unknown_c + unknown_c;

    return (unknown_d * 0.785398f - (unknown_b * unknown_c * unknown_a) +
            -0.166667f * (unknown_b * unknown_c * unknown_a) *
                ((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a)) +
            -0.075f * ((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a)) *
                ((unknown_b * unknown_c * unknown_a) *
                 ((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a))) +
            -0.0446429f *
                ((unknown_b * unknown_c * unknown_a) *
                 ((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a))) *
                (((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a)) *
                 ((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a))) +
            -0.0303819f *
                (((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a)) *
                 ((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a))) *
                (((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a)) *
                 ((unknown_b * unknown_c * unknown_a) *
                  ((unknown_b * unknown_c * unknown_a) * (unknown_b * unknown_c * unknown_a))))) *
           10430.4f;
}

#endif

#define NUTRIGTABLE_COUNT 32768
#define NUTRIGTABLE_INTERVAL (f32)(2.0f * M_PI / NUTRIGTABLE_COUNT)

/// @brief The sine lookup table
/// @details The sine lookup table is a table of the sine function for the angles 0 to 2π.
extern f32 NuTrigTable[NUTRIGTABLE_COUNT];

#define NU_SIN_LUT(ang) NuTrigTable[(i32)(ang) >> 1 & 0x7fff]
#define NU_COS_LUT(ang) NuTrigTable[((i32)(ang) + NUANG_90DEG) >> 1 & 0x7fff]
#define NU_TAN_LUT(ang) (NuTrigTable[(i32)(ang) >> 1 & 0x7fff] / NuTrigTable[((i32)(ang) + NUANG_90DEG) >> 1 & 0x7fff])

#ifdef __cplusplus
extern "C" {
#endif
    /// @brief Initializes the sine lookup table
    /// @details Initializes the sine lookup table with the values of the sine function for the angles 0 to 2π.
    /// @return void
    void NuTrigInit(void);

    i16 NuACos(f32 cos);
    i32 NuAtan2D(f32 dx, f32 dy);
    f32 NuAtan2(f32 dx, f32 dy);

    f32 NuAtanf(f32 x);
    NUANG NuAtani(f32 x);
    NUANG NuAtan2DA(f32 dx, f32 dy);
    f32 NuAtan2DAF(f32 dx, f32 dy);

    f32 NuSinf(NUANG ang);
    f32 NuCosf(NUANG ang);
#ifdef __cplusplus
}

f32 NuSinApprox2(i32 ang);
f32 NuCosApprox2(i32 ang);
f32 NuSin_Accurate(f32 x);

#endif
