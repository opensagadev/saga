#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/vuvec_internal.h"

extern "C" {
    f32 NuFsqrt(f32 f) {
        if (f <= 1e-6f) {
            return 0.0f;
        }

        return sqrtf(f);
    }

    f32 NuFrsqrt(f32 value) {
        return value <= 0.0f ? 0.0f : 1.0f / sqrtf(value);
    }

    f32 NuFmax(f32 a, f32 b) {
        if (a > b) {
            return a;
        }

        return b;
    }

    f32 NuFmin(f32 a, f32 b) {
        if (a < b) {
            return a;
        }

        return b;
    }
}
