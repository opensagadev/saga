#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/vuvec_internal.h"

void NuHGobjRead(variptr_u *, char *) {
    STUBBED();
}

extern "C" void NuHGobjPOILocalMtxFromIX(nuhgobj_s *object, u8 index, NUMTX *joint_matrices, NUMTX *result) {
    nuhgobjpoi_s *point = &object->points_of_interest[index];
    if (point->joint_index == 0xff) {
        *result = point->local_matrix;
    } else {
        NuMtxMulVU0(result, &point->local_matrix, &joint_matrices[point->joint_index]);
    }
}

extern "C" void NuHGobjPOIMtxFromIX(void) {
    STUBBED();
}

extern "C" i32 NuHGobjRndr(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices) {
    NUMTX joint_matrices[256];
    NuHGobjEval(object, 0, NULL, joint_matrices);
    return NuHGobjRndrMtxDwa(object, world_matrix, render_count, render_indices, joint_matrices, NULL, 0);
}

extern "C" nuhgobjpoi_s *NuHGobjGetPOI(nuhgobj_s *object, i32 index) {
    const u8 mapped_index = static_cast<u8>(index);
    if (mapped_index >= object->point_of_interest_count) {
        return NULL;
    }
    const u8 point_index = object->point_of_interest_map[mapped_index];
    if (point_index == 0xff) {
        return NULL;
    }
    return &object->points_of_interest[point_index];
}
