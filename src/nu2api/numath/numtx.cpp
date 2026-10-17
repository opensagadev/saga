#include <limits.h>

#include "nu2api/numath/numtx.h"

#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

// clang-format off
// clang-format on

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

