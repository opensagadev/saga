#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/numtx.h"

f32 *NuGCutLocatorCalcMtx_3(NUGCUTLOCATOR_s *locator, numtx_s *mtx, float frame) {
    ani3_animheader_s *animation = reinterpret_cast<ani3_animheader_s *>(locator->animation);
    f32 *values = NuAnimCurveExtractAllNodeCurves_3(animation, 0, frame, NULL);
    if ((*animation->node_flags & 1) != 0) {
        NUANGVEC angles = {
            static_cast<NUANG>(values[3] * 10430.378f),
            static_cast<NUANG>(values[4] * 10430.378f),
            static_cast<NUANG>(values[5] * 10430.378f),
        };
        NuMtxSetRotateXYZ(mtx, &angles);
    } else {
        NuMtxSetIdentity(mtx);
    }
    NUVEC translation = {values[0], values[1], values[2]};
    NuMtxTranslate(mtx, &translation);
    mtx->m02 = -mtx->m02;
    mtx->m12 = -mtx->m12;
    mtx->m20 = -mtx->m20;
    mtx->m21 = -mtx->m21;
    mtx->m23 = -mtx->m23;
    mtx->m32 = -mtx->m32;
    NuMtxTranslate(mtx, reinterpret_cast<NUVEC *>(&locator->base_matrix.m30));
    return values;
}

extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *locator, float frame, NUMTX *mtx, nuanimtime_s *time) {
    if (locator->animation == NULL) {
        *mtx = locator->base_matrix;
        return NULL;
    }
    if (__builtin_expect(*reinterpret_cast<u32 *>(locator->animation) + 0xbeb1b6ccU < 2, 0)) {
        return NuGCutLocatorCalcMtx_3(locator, mtx, frame);
    }
    nuanimdata2_s *animation = locator->animation;
    u8 *curve_types = animation->curve_types;
    nuanimcurve2_s *curves = animation->curves;
    auto evaluate = [&](u32 curve) {
        const i8 type = static_cast<i8>(curve_types[curve]);
        return type == 0 ? curves[curve].data.constant
                         : NuAnimCurve2CalcValEx(&curves[curve], time, static_cast<u32>(type));
    };
    if ((*animation->node_flags & 1) == 0) {
        NuMtxSetIdentity(mtx);
    } else {
        const f32 rotation_x = evaluate(3);
        const f32 rotation_y = evaluate(4);
        const f32 rotation_z = evaluate(5);
        NUANGVEC angles = {
            static_cast<NUANG>(rotation_x * 10430.378f),
            static_cast<NUANG>(rotation_y * 10430.378f),
            static_cast<NUANG>(rotation_z * 10430.378f),
        };
        NuMtxSetRotateXYZ(mtx, &angles);
    }
    NUVEC translation = {evaluate(0), evaluate(1), evaluate(2)};
    NuMtxTranslate(mtx, &translation);
    mtx->m02 = -mtx->m02;
    mtx->m12 = -mtx->m12;
    mtx->m20 = -mtx->m20;
    mtx->m21 = -mtx->m21;
    mtx->m23 = -mtx->m23;
    mtx->m32 = -mtx->m32;
    NuMtxTranslate(mtx, reinterpret_cast<NUVEC *>(&locator->base_matrix.m30));
    return NULL;
}

char CurveFilterLOCATORScale[] = {6, 7, 8, static_cast<char>(0xff)};
char CurveFilterLOCATOR[] = {6, 7, static_cast<char>(0xff)};
char CurveFilterSFX[] = {3, static_cast<char>(0xff)};

i32 NuGCutLocatorIsVisble_3(NUGCUTLOCATOR_s *locator, float frame, float *scale, float *rate) {
    // The original rereads the locator's animation header for every test.
    const u16 curve_count = reinterpret_cast<ani3_animheader_s *>(locator->animation)->curve_count;
    char *filter = curve_count == 4 ? CurveFilterSFX : (curve_count == 8 ? CurveFilterLOCATOR : CurveFilterLOCATORScale);
    f32 *values = NuAnimCurveExtractAllNodeCurves_3(reinterpret_cast<ani3_animheader_s *>(locator->animation), 0,
                                                    frame, filter);
    if (reinterpret_cast<ani3_animheader_s *>(locator->animation)->curve_count == 4) {
        f32 visible = values[3];
        return static_cast<i32>(visible < 0.0f ? visible - 0.5f : visible + 0.5f);
    }
    f32 visible = values[7];
    i32 result = static_cast<i32>(visible < 0.0f ? visible - 0.5f : visible + 0.5f);
    if (result != 0 && scale != NULL) {
        *scale = values[6];
    }
    if (rate != NULL) {
        *rate = reinterpret_cast<ani3_animheader_s *>(locator->animation)->curve_count < 9 ? 1.0f : values[8];
    }
    return result;
}

extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *locator, float frame, nuanimtime_s *time, float *scale,
                                     float *rate) {
    if (locator->animation == NULL) {
        const i32 visible = locator->flags & 8;
        if (scale != NULL && visible != 0) {
            *scale = locator->locator_scale;
        }
        return visible;
    }
    if (*reinterpret_cast<u32 *>(locator->animation) + 0xbeb1b6ccU < 2) {
        return NuGCutLocatorIsVisble_3(locator, frame, scale, rate);
    }
    nuanimdata2_s *animation = locator->animation;
    if (rate != NULL) {
        *rate = 1.0f;
    }
    nuanimcurve2_s *curves = animation->curves;
    u8 *curve_types = animation->curve_types;
    if (animation->curve_count == 4) {
        const i8 type = curve_types[3];
        if (type == 0) {
            return static_cast<i32>(curves[3].data.constant);
        }
        return static_cast<i32>(NuAnimCurve2CalcValEx(&curves[3], time, static_cast<u32>(type)));
    }
    const i8 visible_type = curve_types[7];
    i32 visible;
    if (visible_type == 0) {
        visible = static_cast<i32>(curves[7].data.constant);
    } else {
        visible = static_cast<i32>(NuAnimCurve2CalcValEx(&curves[7], time, static_cast<u32>(visible_type)));
    }
    if (visible != 0 && scale != NULL) {
        const i8 scale_type = curve_types[6];
        *scale = scale_type == 0 ? curves[6].data.constant
                                 : NuAnimCurve2CalcValEx(&curves[6], time, static_cast<u32>(scale_type));
    }
    return visible;
}
