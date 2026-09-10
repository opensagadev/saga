#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include "decomp.h"
#include "nu2api/nucore/common.h"

static f32 fetol = 0.01f;

i32 NuEquiv(f32 a, f32 b) {
    return NuFabs(a - b) < fetol;
}

void NuEquivTollerance(f32 tolerance) {
    fetol = tolerance;
}

f32 NuFnabs(f32 f) {
    f32 rv;

    *(i32 *)&rv = *(i32 *)&f | 0x80000000;
    return rv;
}

f32 NuFneg(f32 v) {
    f32 rv;

    *(i32 *)&rv = *(i32 *)&v ^ 0x80000000;
    return rv;
}

f32 NuFsign(f32 f) {
    if (*(i32 *)&f < 0) {
        return -1.0f;
    } else {
        return 1.0f;
    }
}

f32 NuFsqrt(f32 f) {
    if (f <= 1e-6f) {
        return 0.0f;
    }

    return sqrtf(f);
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

f32 NuFmod(f32 a, f32 b) {
    return a - (i32)(a / b) * b;
}

f32 NuFloor(f32 f) {
    return (f32)(i32)f;
}

static f32 NuTrunc(f32 a) {
    return (f32)(i32)a;
}

static f32 NuFsel(f32 a, f32 b, f32 c) {
    return a >= 0.0f ? b : c;
}

f32 NUTRIG_SC0 = -0.1666666716337204f;
f32 NUTRIG_SC1 = 0.008333333767950535f;
f32 NUTRIG_SC2 = -0.00019841270113829523f;
f32 NUTRIG_SC3 = 2.7557318844628753e-06f;

f32 NuSinf(f32 angle) {
    f32 magnitude = NuFabs(angle);
    f32 sign = NuFsel(angle, 1.0f, -1.0f);
    f32 turns = NuTrunc(magnitude * 0.31830987334251404f + 0.5f);
    f32 parity = NuTrunc(turns * 0.5f) - turns * 0.5f;
    f32 reduced = magnitude - turns * 3.1415927410125732f;
    f32 square = reduced * reduced;
    f32 polynomial = (((NUTRIG_SC3 * square + NUTRIG_SC2) * square + NUTRIG_SC1) * square + NUTRIG_SC0) * square;
    sign = parity * sign * 4.0f + sign;
    return (reduced * polynomial + reduced) * sign;
}

f32 NuCosf(f32 angle) {
    return NuSinf(angle + 1.5707963705062866f);
}

f32 NuPowFast(f32 base, f32 exponent) {
    if (base == 0.0) {
        return 0.0;
    }

    f32 result;
    result = NuExp10(NuLog10(base) * exponent);
    return result;
}
/* Both exception controls are empty in the original Android binary. */
void NuFpExceptionMask(void) {
}

void NuFpException(void) {
}
