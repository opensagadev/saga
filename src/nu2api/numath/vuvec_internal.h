#pragma once

#include "nu2api/nucore/nuvuvec.hpp"

extern "C" {
    static void VuVecSet(VuVec *out, f32 x, f32 y, f32 z, f32 w) {
        out->x = x;
        out->y = y;
        out->z = z;
        out->w = w;
    }
}
