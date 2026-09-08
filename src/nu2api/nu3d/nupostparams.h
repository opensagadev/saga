#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/numath/numtx.h"
#include "decomp_assert.h"

typedef struct NuBloomParameters {
    i32 enabled;
    f32 near_angle;
    f32 near_scale;
    f32 far_angle;
    f32 far_scale;
    f32 intensity;
    f32 unknown_18;
    f32 blur_iterations;
    f32 blend;
    f32 threshold;
    i32 directional;
    NUVEC direction;
    f32 direction_near_angle;
    f32 direction_far_scale;
    f32 direction_far_angle;
    f32 direction_near_scale;
    f32 direction_bias;
    f32 unknown_4c;
    f32 unknown_50;
} NuBloomParameters;
DECOMP_ASSERT(sizeof(NuBloomParameters) == 0x54, "bloom parameters");

typedef struct NuDepthOfFieldParameters {
    i32 enabled;
    f32 strength;
    f32 near_distance;
    f32 far_distance;
    i32 mode;
    f32 bias;
} NuDepthOfFieldParameters;

typedef struct NuSpeedBlurParameters {
    i32 enabled;
    NUMTX previous;
    NUMTX current;
    f32 unknown_84;
    f32 scale;
} NuSpeedBlurParameters;

#ifdef __cplusplus
extern "C" {
#endif
    void NuPostBloom(i32, const NuBloomParameters *);
    void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *);
    void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
    void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
    void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
    void NuCameraMotionBlurParams(const NUMTX *);
    void NuCameraMotionBlurEffect(const NUMTX *, const NUMTX *, f32, f32, f32);
    void NuAccumulationMotionBlurParams(i32, f32, i32);
    void NuAccumulationMotionBlurEffect(i32, f32, i32);
#ifdef __cplusplus
}
#endif
