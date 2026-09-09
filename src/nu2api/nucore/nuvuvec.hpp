#pragma once

#include "decomp.h"
#include "decomp.h"
#include "nu2api/numath/nuvec.h"

class VuVec {
  public:
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
        NUVEC xyz;
    };
    f32 w;

    VuVec() = default;

    VuVec(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {
    }
};
DECOMP_ASSERT(sizeof(VuVec) == 0x10, "VuVec size");
DECOMP_ASSERT(offsetof(VuVec, xyz) == 0, "VuVec three-dimensional view offset");
DECOMP_ASSERT(offsetof(VuVec, w) == 0xc, "VuVec fourth component offset");

// The original static initializers set the homogeneous component to one.
static const VuVec VuVec_X{1, 0, 0, 1};
static const VuVec VuVec_Y{0, 1, 0, 1};
static const VuVec VuVec_Z{0, 0, 1, 1};

static const VuVec VuVec_Up{0, 1, 0, 1};
static const VuVec VuVec_Down{0, -1, 0, 1};

static const VuVec VuVec_Zero{0, 0, 0, 1};
