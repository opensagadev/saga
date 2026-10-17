#include "decomp.h"

#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

static void VuVecMtxMul(NUVEC *out, NUVEC *v, NUMTX *m) {
    NuVecMtxTransform(out, v, m);
}

static void VuVecSet(f32 *out, f32 x, f32 y, f32 z, f32 w) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
    out[3] = w;
}

static void VuMtxTranspose(f32 dst[4][4], f32 src[4][4]) {
    NuMtxTranspose((NUMTX *)&dst[0][0], (NUMTX *)&src[0][0]);
}

