#pragma once

#include "nu2api/nucore/nuvuvectype.hpp"

// The original static initializers set the homogeneous component to one.
static const VuVec VuVec_X{1, 0, 0, 1};
static const VuVec VuVec_Y{0, 1, 0, 1};
static const VuVec VuVec_Z{0, 0, 1, 1};

static const VuVec VuVec_Up{0, 1, 0, 1};
static const VuVec VuVec_Down{0, -1, 0, 1};

static const VuVec VuVec_Zero{0, 0, 0, 1};

// Retail emits a local copy of this helper in every unit that includes this header.
extern "C" {
    static void VuVecSet(VuVec *out, f32 x, f32 y, f32 z, f32 w) {
        out->x = x;
        out->y = y;
        out->z = z;
        out->w = w;
    }
}
