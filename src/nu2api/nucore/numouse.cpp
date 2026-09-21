#include "decomp.h"
#include "nu2api/nucore/numouse.h"

// The Android implementations ignore their inputs and report no button.
extern "C" i32 NuMouseButton(void) {
    STUBBED();
    return 0;
}
extern "C" i32 NuMouseButton_db(void) {
    STUBBED();
    return 0;
}

u32 NuMouseReadButtons_db(void) {
    STUBBED();
    return 0;
}

f32 NuMouseReadXRel(void) {
    STUBBED();
    return 0.0f;
}

f32 NuMouseReadXVel(void) {
    STUBBED();
    return 0.0f;
}

f32 NuMouseReadYRel(void) {
    STUBBED();
    return 0.0f;
}

f32 NuMouseReadYVel(void) {
    STUBBED();
    return 0.0f;
}

f32 NuMouseReadZRel(void) {
    STUBBED();
    return 0.0f;
}

f32 NuMouseReadZVel(void) {
    STUBBED();
    return 0.0f;
}

void NuMouseRead(void) {
}

f32 NuMouseReadX(void) {
    STUBBED();
    return 0.0f;
}
f32 NuMouseReadY(void) {
    STUBBED();
    return 0.0f;
}
f32 NuMouseReadZ(void) {
    STUBBED();
    return 0.0f;
}
u32 NuMouseReadButtons(void) {
    STUBBED();
    return 0;
}
