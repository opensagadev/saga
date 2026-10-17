#include "nu2api/numath/nutrig.h"
#include "nu2api/nucore/nuvuvec.hpp"

f32 NuTrigTable[NUTRIGTABLE_COUNT];

f32 NuATanf(f32 value) {
    return atanf(value);
}

f32 NuATan2f(f32 y, f32 x) {
    return atan2f(y, x);
}

