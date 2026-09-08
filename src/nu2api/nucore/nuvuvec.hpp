#pragma once

#include "decomp.h"

class VuVec {
  public:
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    VuVec() = default;

    VuVec(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {
    }
};

// The original static initializers set the homogeneous component to one.
static const VuVec VuVec_X{1, 0, 0, 1};
static const VuVec VuVec_Y{0, 1, 0, 1};
static const VuVec VuVec_Z{0, 0, 1, 1};

static const VuVec VuVec_Up{0, 1, 0, 1};
static const VuVec VuVec_Down{0, -1, 0, 1};

static const VuVec VuVec_Zero{0, 0, 0, 1};
