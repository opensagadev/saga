#include "nu2api/nucore/nupad.h"

i32 NuPadReadPS(i32, u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, u8 *, u32 *, u8 *, u32 *) {
    i32 result = 0;
    return result;
}

void NuPadInitPS(NUGENERICPAD *pad) {
}

i32 NuPadGetNumberOfPortsPS(void) {
    return 4;
}

void NuPadOpenPS(NUPAD *pad) {
    pad->is_valid = true;
}

void NuPadClosePS(NUPAD *pad) {
}

void NuPadSetMotorsPS(i32 port, i32 motor0, i32 motor1) {
}

void NuPadGetDeadzonePS(NUPAD *pad) {
}

i32 NuPadGetDeadzoneByPortPS(i32 port) {
    // No idea where this value comes from.
    return 0x14;
}
