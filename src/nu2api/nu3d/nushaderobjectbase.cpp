#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>

static f32 water_theta_step = 0.26666668f;

void NuShaderObjectBaseCreate(NUSHADEROBJECTBASE *shader) {
    shader->field0 = -1;
    shader->field1 = 0;
    shader->key = 0;
    shader->field3 = 0;
}

void NuShaderObjectBaseDestroy(NUSHADEROBJECTBASE *shader) {
    shader->field1 = 0;
}

void NuShaderObjectBaseInit(NUSHADEROBJECTBASE *shader, NUSHADEROBJECTKEY *key, i32 unk) {
    memcpy(&shader->key, key, sizeof(shader->key));
    shader->field0 = unk;
}

void NuShaderObjectBaseUnInit(NUSHADEROBJECTBASE *shader) {
    shader->field0 = -1;
}

void NuShaderObjectBaseSetWaterSpeed(f32 speed) {
    water_theta_step = speed * 0.1f;
}

extern "C" void NuShaderObjectBaseUpdateWaterTable(NUSHADEROBJECT *shader, numtl_s *mtl) {
    static NUVEC4 waterTable[32];
    static i32 lastintsame = -1;
    static numtl_s *prev_mtl;
    static f32 theta = 0.7f;
    i32 frame;
    memcpy(&frame, &nuapi.frame_count, sizeof(frame));
    const f32 *material = reinterpret_cast<const f32 *>(mtl);
    if (frame != lastintsame)
        theta = material[0x60 / 4] * water_theta_step + theta;
    if (frame != lastintsame || mtl != prev_mtl) {
        NUMTX inverse;
        NUVEC scale = {0.5f, 0.5f, 0.5f};
        NuMtxInvR(&inverse, &global_camera.mtx);
        NuMtxScale(&inverse, &scale);
        inverse.m03 = inverse.m13 = inverse.m23 = 0.0f;
        const f32 amplitude = 0.1f * material[0x6c / 4];
        u32 seed = 17;
        NuRandFloatSeeded(&seed);
        for (i32 i = 0; i < 32; ++i) {
            NUVEC displacement;
            f32 phase = (NuRandFloatSeeded(&seed) * 0.4f + 0.8f) * theta;
            i32 angle = static_cast<i32>((NuRandFloatSeeded(&seed) * 6.283f + phase) * 10430.3779296875f);
            displacement.x = (amplitude * NuTrigTable[(angle >> 1) & 0x7fff]) * 4.0f;
            phase = (NuRandFloatSeeded(&seed) * 0.8f + 0.6f) * theta;
            angle = static_cast<i32>((NuRandFloatSeeded(&seed) * 5.717f + phase) * 10430.3779296875f);
            displacement.y = (amplitude * NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff]) * 4.0f;
            phase = (NuRandFloatSeeded(&seed) * 0.4f + 0.7f) * theta;
            angle = static_cast<i32>((NuRandFloatSeeded(&seed) * 6.283f + phase) * 10430.3779296875f);
            displacement.z = amplitude * NuTrigTable[(angle >> 1) & 0x7fff];
            waterTable[i].w = 0.25f * displacement.x;
            NuVecMtxTransformH(reinterpret_cast<NUVEC *>(&waterTable[i]), &displacement, &inverse);
        }
    }
    NuShaderObjectSetElementsfv(shader, 31, 0, 32, reinterpret_cast<const f32 *>(waterTable));
    prev_mtl = mtl;
    memcpy(&lastintsame, &nuapi.frame_count, sizeof(lastintsame));
}
