#include "decomp.h"

#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec.h"

static void VuQuatCopy(NUQUAT *dst, NUQUAT *src) {
    *dst = *src;
}

static void VuQuatBlend(NUQUAT *out, NUQUAT *a, NUQUAT *b, f32 t, f32 w) {
    out->x = a->x * t + b->x * w;
    out->y = a->y * t + b->y * w;
    out->z = a->z * t + b->z * w;
    out->w = a->w * t + b->w * w;
}

static f32 VuQuatDot(NUQUAT *a, NUQUAT *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

static void VuQuatLerp(NUQUAT *out, NUQUAT *a, NUQUAT *b, f32 t) {
    out->x = (b->x - a->x) * t + a->x;
    out->y = (b->y - a->y) * t + a->y;
    out->z = (b->z - a->z) * t + a->z;
    out->w = (b->w - a->w) * t + a->w;
}

static void VuQuatNeg2(NUQUAT *out, NUQUAT *in) {
    out->x = -in->x;
    out->y = -in->y;
    out->z = -in->z;
    out->w = -in->w;
}

static void VuQuatNormalise(NUQUAT *out, NUQUAT *in) {
    f32 magnitude = NuFsqrt(in->x * in->x + in->y * in->y + in->z * in->z + in->w * in->w);
    magnitude = magnitude > 0.0f ? 1.0f / magnitude : 0.0f;
    out->x = in->x * magnitude;
    out->y = in->y * magnitude;
    out->z = in->z * magnitude;
    out->w = in->w * magnitude;
}

static void VuVecMtxMul(NUVEC *out, NUVEC *v, NUMTX *m) {
    (void)out;
    (void)v;
    (void)m;
}

static void VuVecSet(f32 *out, f32 x, f32 y, f32 z, f32 w) {
    (void)out;
    (void)x;
    (void)y;
    (void)z;
    (void)w;
}

static void VuMtxTranspose(NUMTX *dst, NUMTX *src) {
    (void)dst;
    (void)src;
}
