#include "decomp.h"

#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec.h"

static void VuQuatCopy(NUQUAT *dst, NUQUAT *src);

static void VuQuatBlend(NUQUAT *out, NUQUAT *a, NUQUAT *b, f32 t, f32 w);

static f32 VuQuatDot(NUQUAT *a, NUQUAT *b);

static void VuQuatLerp(NUQUAT *out, NUQUAT *a, NUQUAT *b, f32 t);

static void VuQuatNeg2(NUQUAT *out, NUQUAT *in);

static void VuQuatNormalise(NUQUAT *out, NUQUAT *in);

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

void VuQuatSlerpFast(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t);
