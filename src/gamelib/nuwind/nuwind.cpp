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

#if defined(__i386__) && defined(__GNUC__) && !defined(__clang__)
        typedef i32 Int4 __attribute__((vector_size(16)));
        const Int4 all_ones = {-1, -1, -1, -1};
        __builtin_memcpy(&wind->unk0[0], &all_ones, sizeof(all_ones));
        __builtin_memcpy(&wind->unk0[4], &all_ones, sizeof(all_ones));
#else
        for (usize i = 0; i < 8; ++i) {
            wind->unk0[i] = -1;
        }
#endif
    }
}

void NuWindSetWorldSize(NUWIND *wind, f32 size) {
    if (wind != NULL) {
        if (size >= 1.0f) {
            wind->unk2.x = size;
        } else {
            wind->unk2.x = 1.0f;
        }
    }
}

void NuWindSetSpeed(NUWIND *wind, f32 speed) {
    if (wind != NULL) {
        if (speed >= 1.0f) {
            wind->unk2.y = speed;
        } else {
            wind->unk2.y = 1.0f;
        }
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
