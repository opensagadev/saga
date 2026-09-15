#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void DisplayListGenerateTransforms(nudisplayscene_s *) {
    STUBBED();
}

void bgprocIsFrozen() {
    STUBBED();
}

void DisplayListCreateGeomItemPS(variptr_u *, void *, numtl_s *) {
    STUBBED();
}

void DisplayListCreateInstSurfGeomPS(variptr_u *, numtx_s *) {
    STUBBED();
}

// Flag-sensitive moves from supportall.cpp (-O2): these match at the
// default flag. GetBuffer stays a call and float scheduling matches.
void *RndrStateBuildKonstState(nuglobalrndrstate_s *state) {
    VARIPTR *buffer = NuDisplayListGetBuffer();
    f32 *konst = static_cast<f32 *>(buffer->void_ptr);
    f32 *result = konst;

    if (state->const_tint_enabled == 0) {
        konst[0] = 1.0f;
        konst[1] = 1.0f;
        konst[2] = 1.0f;
    } else {
        konst[0] = state->const_tint.r;
        konst[1] = state->const_tint.g;
        konst[2] = state->const_tint.b;
    }
    konst[3] = state->const_alpha_enabled == 0 ? 1.0f : state->const_alpha;
    buffer->addr += sizeof(f32) * 4;
    return result;
}
