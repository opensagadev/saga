#include "gamelib/nuwind/nuwind.h"

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

i32 NuWindCurrent(NUWIND *wind) {
    if (wind == NULL || wind->unk1 < 0)
        return -1;
    return wind->unk0[wind->unk1];
}
