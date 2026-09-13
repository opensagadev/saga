#include "gamelib/nuwind/nuwind.h"
#include "nu2api/numath/nufloat.h"

void NuWindInitialise(NUWIND *wind) {
    if (wind != NULL) {
        wind->unk2.x = 1.0f;
        wind->unk2.y = 1.0f;
        wind->unk2.z = 0.0f;
        wind->unk2.w = 0.0f;
        wind->unk3 = 0.0f;
        wind->unk1 = -1;

        for (usize i = 0; i < 8; ++i) {
            wind->unk0[i] = -1;
        }
    }
}

void NuWindSetWorldSize(NUWIND *wind, f32 size) {
    if (wind != NULL) {
        wind->unk2.x = 1.0f <= size ? size : 1.0f;
    }
}

void NuWindSetSpeed(NUWIND *wind, f32 speed) {
    if (wind != NULL) {
        wind->unk2.y = 1.0f <= speed ? speed : 1.0f;
    }
}

i32 NuWindCurrent(NUWIND *wind) {
    if (wind == NULL || wind->unk1 < 0)
        return -1;
    return wind->unk0[wind->unk1];
}

void NuWindAnimate(NUWIND *wind, f32 frametime) {
    if (wind != NULL) {
        wind->unk2.z += (1.0f / 256.0f) * wind->unk2.y * frametime;
        if (wind->unk2.z >= 1.0f) {
            wind->unk2.z = NuFmod(wind->unk2.z, 1.0f);
        }
        f32 scaled_time = 5.0f * frametime;
        wind->unk2.w = frametime + wind->unk2.w;
        wind->unk3 = scaled_time + wind->unk3;
    }
}
