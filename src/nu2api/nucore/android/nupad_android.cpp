#include "decomp.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"

i32 ScaleAndClamp(i32 value) {
    value = value * 4224 / 1048576;
    if (value < -128)
        value = -128;
    if (value > 127)
        value = 127;
    return value + 128;
}

void UCStretchToCorners(i16 *horizontal, i16 *vertical) {
    f32 x = *horizontal;
    f32 y = *vertical;
    f32 scale;
    f32 abs_x = NuFabs(x);
    f32 abs_y = NuFabs(y);
    f32 radius = NuFsqrt(abs_x * abs_x + abs_y * abs_y);
    if (abs_y > abs_x)
        scale = 32767.0f / abs_y;
    else if (abs_x != 0.0f)
        scale = 32767.0f / abs_x;
    else
        scale = 1.0f;
    radius *= scale;
    scale = radius / 32768.0f;
    x *= scale;
    y *= scale;
    if (x < -32767.0f)
        x = -32767.0f;
    if (x > 32767.0f)
        x = 32767.0f;
    if (y < -32767.0f)
        y = -32767.0f;
    if (y > 32767.0f)
        y = 32767.0f;
    *horizontal = static_cast<i16>(x);
    *vertical = static_cast<i16>(y);
}

i32 NuPadReadPS(i32, u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, u32 *, u8 *, u32 *) {
    i32 result = 0;
    return result;
}

void NuPadInitPS(NUGENERICPAD *pad) {
    STUBBED();
}

i32 NuPadGetNumberOfPortsPS(void) {
    return 4;
}

void NuPadOpenPS(NUPAD *pad) {
    pad->is_valid = true;
}

void NuPadClosePS(NUPAD *pad) {
    STUBBED();
}

void NuPadSetMotorsPS(i32 port, i32 motor0, i32 motor1) {
    STUBBED();
}

void NuPadGetDeadzonePS(NUPAD *pad) {
    STUBBED();
}

i32 NuPadGetDeadzoneByPortPS(i32 port) {
    // No idea where this value comes from.
    return 0x14;
}
