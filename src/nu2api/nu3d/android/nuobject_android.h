#pragma once

#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuspecial.h"

struct NuFadeObjGType;

#ifdef __cplusplus
extern "C" {
#endif

    void NuFadeObjInit(void);
    void NuFadeObjSet_SetLightsFn(void (*global_lights)(void), void (*object_lights)(void));
    NuFadeObjGType *NuFadeObjCreateMtx(nuhspecial_s *source, NUMTX *matrices, i16 count, f32 near_distance,
                                       f32 far_distance, i32 flags);
    void NuFadeObjUpdateArray(NUVEC **positions);
    void NuFadeObjUpdate(NUVEC *position);
    void NuFadeObjDraw(void);
    void NuFadeObjSetup(VARIPTR *buffer, VARIPTR end, i32 matrix_count, i32 group_count);

#ifdef __cplusplus
}
#endif
