#include "decomp.h"

#include <string.h>

i32 rndr_blend_shape_deformer_wt_cnt = 0x3f00;
i32 rndr_blend_shape_deformer_wt_ptrs_cnt = 0x800;
static f32 rndr_blend_shape_deformer_wts[0x4000][0x21];
static f32 *rndr_blend_shape_deformer_wt_ptrs[0x800];

extern "C" f32 *NuRndrCreateBlendShapeDeformerWeightsArray(i32 count) {
    i32 size = (count + 0x20) * sizeof(f32);
    rndr_blend_shape_deformer_wt_cnt -= size;
    if (rndr_blend_shape_deformer_wt_cnt < 0) {
        return NULL;
    }
    f32 *weights = rndr_blend_shape_deformer_wts[rndr_blend_shape_deformer_wt_cnt];
    memset(weights, 0, size);
    return weights;
}

f32 **NuRndrCreateBlendShapeDWAPointers(i32 count) {
    rndr_blend_shape_deformer_wt_ptrs_cnt -= count;
    if (rndr_blend_shape_deformer_wt_cnt < 0) {
        return NULL;
    }
    f32 **pointers = rndr_blend_shape_deformer_wt_ptrs + rndr_blend_shape_deformer_wt_ptrs_cnt;
    return pointers;
}
