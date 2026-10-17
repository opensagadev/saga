#include <limits.h>

#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"

#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nucamera.h"

extern NUMTX numtx_zero;

// clang-format off
extern NUMTX numtx_identity;
// clang-format on

void NuMtxSetZero(NUMTX *m);

void NuMtxSetIdentity(NUMTX *m);

void NuMtxSetTranslation(NUMTX *m, NUVEC *t);

void NuMtxSetTranslationNeg(NUMTX *m, NUVEC *t);

void NuMtxSetScale(NUMTX *m, NUVEC *s);

void NuMtxSetRotationX(NUMTX *m, NUANG a);


void NuMtxSetRotationZ(NUMTX *m, NUANG a);

void NuMtxSetRotationZ(NUMTX *m, NUANG a);

void NuMtxSetRotationAxis(NUMTX *m, NUANG ang, NUVEC *ax);

void NuMtxTranslate(NUMTX *m, NUVEC *t);

void NuMtxTranslateNeg(NUMTX *m, NUVEC *t);

void NuMtxPreTranslate(NUMTX *m, NUVEC *t);

void NuMtxPreTranslateX(NUMTX *m, f32 tx);

void NuMtxPreTranslateNeg(NUMTX *m, NUVEC *t);

void NuMtxScale(NUMTX *m, NUVEC *s);

void NuMtxScaleU(NUMTX *m, f32 s);

void NuMtxPreScaleU(NUMTX *m, f32 s);

NUVEC NuMtxGetScale(NUMTX *m);

void NuMtxPreScale(NUMTX *m, NUVEC *s);

void NuMtxPreScaleX(NUMTX *m, f32 ScaleX);


void NuMtxPreRotateX(NUMTX *m, NUANG a);

void NuMtxPreRotateX(NUMTX *m, NUANG a);


void NuMtxPreRotateY(NUMTX *m, NUANG a);

void NuMtxPreRotateY(NUMTX *m, NUANG a);


void NuMtxPreRotateZ(NUMTX *m, NUANG a);

void NuMtxPreRotateZ(NUMTX *m, NUANG a);

void NuMtxPreRotateY180(NUMTX *m);

void NuMtxPreRotateY180X(NUMTX *m, NUANG a);

void NuMtxPreSkewYX(NUMTX *Mtx, f32 SkewVal);

void NuMtxTransposeR(NUMTX *m, NUMTX *m0);

void NuMtxTranspose(NUMTX *m, NUMTX *m0);

void NuMtxInv(NUMTX *m, NUMTX *m0);

void NuMtxInvR(NUMTX *m, NUMTX *m0);

f32 NuMtxDet3(NUMTX *m);

void NuMtxLookAtX(NUMTX *m, NUVEC *pnt);

void NuMtxLookAtY(NUMTX *m, NUVEC *pnt);

void NuMtxLookAtZ(NUMTX *m, NUVEC *pnt);

void NuMtxInvLookAtX(NUMTX *m, NUVEC *pnt);

void NuMtxInvLookAtY(NUMTX *m, NUVEC *pnt);

void NuMtxInvLookAtZ(NUMTX *m, NUVEC *pnt);

void NuMtxAddR(NUMTX *m, NUMTX *m0, NUMTX *m1);

void NuMtxSubR(NUMTX *m, NUMTX *m0, NUMTX *m1);

void NuMtxSkewSymmetric(NUMTX *m, NUVEC *v);

void NuMtxGetXAxis(NUMTX *m, NUVEC *x);

void NuMtxGetYAxis(NUMTX *m, NUVEC *y);

void NuMtxGetZAxis(NUMTX *m, NUVEC *z);

void NuMtxGetTranslation(NUMTX *m, NUVEC *t);

i32 NuMtxCompare(NUMTX *a, NUMTX *b);

void NuMtxTruncate24Bit(NUMTX *trunc, NUMTX *mtx);

void NuMtxRotateAng(NUANG ang, f32 x, f32 z, f32 *rx, f32 *rz);

void NuMtxGetEulerXYZ(NUMTX *Mat, NUANG *x, NUANG *y, NUANG *z);

void NuMtxLookAtD3D(NUMTX *mtx, NUVEC *eye, NUVEC *center, NUVEC *up);

void NuMtxSetPerspectiveD3D(NUMTX *mtx, f32 fovy, f32 aspect, f32 zNear, f32 zFar);

void NuMtxSetPerspectiveBlend(NUMTX *mtx, f32 fovy, f32 aspect, f32 zNear, f32 zFar);

void NuMtxSetFrustumD3D(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

void NuMtxSetFrustumBlend(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

void NuMtxSetOrthoD3D(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

void NuMtxSetOrthoBlend(NUMTX *mtx, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

void NuMtxGetPerspectiveD3D(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar);

void NuMtxGetPerspectiveBlend(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar);

void NuMtxGetFrustumD3D(NUMTX *mtx, f32 *l, f32 *r, f32 *b, f32 *t, f32 *n, f32 *f);

void NuMtxGetFrustumBlend(NUMTX *mtx, f32 *l, f32 *r, f32 *b, f32 *t, f32 *n, f32 *f);

void NuMtxSetRotateXYZ(NUMTX *m, NUANGVEC *a);

void NuMtxMul(NUMTX *m, NUMTX *m0, NUMTX *m1);

void NuMtxMulH(NUMTX *m, NUMTX *m0, NUMTX *m1);

void NuMtxMulR(NUMTX *m, NUMTX *m0, NUMTX *m1);

void NuMtxInvRSS(NUMTX *inv, NUMTX *T);

void NuMtxInvRSSH(NUMTX *inv, NUMTX *T);

void NuMtxInvH(NUMTX *mi, NUMTX *m0);

void NuMtxAlignX(NUMTX *m, NUVEC *v);

void NuMtxAlignY(NUMTX *m, NUVEC *v);

void NuMtxAlignZ(NUMTX *m, NUVEC *v);

void NuMtxOrth(NUMTX *m);

void NuMtxVecToEulerXYZ(NUVEC *XVec, NUVEC *ZVec, NUANG *x, NUANG *y, NUANG *z);

f32 NuMtxSSE(NUMTX *a, NUMTX *b);

void NuMtx24BitCorrection(NUMTX *correction, NUMTX *matrix);

void NuMtxCalcCheapFaceOnDebug(NUMTX *m, NUVEC *v) {
    NUMTX camera = {
        -0.9711849093437195f,  0.0f,
        -0.23832732439041138f, 0.0f,
        0.08866473287343979f,  0.9282209873199463f,
        -0.3613092005252838f,  0.0f,
        0.22122041881084442f,  -0.37202924489974976f,
        -0.9014742374420166f,  0.0f,
        -0.6754902601242065f,  1.3077397346496582f,
        -0.7606481909751892f,  1.0f,
    };
    m->m00 = -camera.m00;
    m->m10 = camera.m01;
    m->m20 = -camera.m02;
    m->m01 = -camera.m10;
    m->m11 = camera.m11;
    m->m21 = -camera.m12;
    m->m02 = -camera.m20;
    m->m12 = camera.m21;
    m->m22 = -camera.m22;
    m->m03 = m->m13 = m->m23 = 0.0f;
    m->m33 = 1.0f;
    m->m30 = v->x;
    m->m31 = v->y;
    m->m32 = v->z;
}

void NuMtxGetPerspectivePS3(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar);

void NuMtxGetPerspectiveOGL(NUMTX *mtx, f32 *fovy, f32 *aspect, f32 *zNear, f32 *zFar);

void NuMtxLookAtInverseD3D(NUMTX *mtx, NUVEC *eye, NUVEC *center, NUVEC *up);

void NuMtxToQuat(NUMTX *m, struct nuquat_s *out);
