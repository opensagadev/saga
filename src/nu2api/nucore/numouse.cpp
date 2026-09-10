#include "nu2api/nucore/numouse.h"

// The Android implementations ignore their inputs and report no button.
extern "C" i32 NuMouseButton(void) {
    return 0;
}
extern "C" i32 NuMouseButton_db(void) {
    return 0;
}

u32 NuMouseReadButtons_db(void) {
    return 0;
}

f32 NuMouseReadXRel(void) {
    return 0.0f;
}

f32 NuMouseReadXVel(void) {
    return 0.0f;
}

f32 NuMouseReadYRel(void) {
    return 0.0f;
}

f32 NuMouseReadYVel(void) {
    return 0.0f;
}

f32 NuMouseReadZRel(void) {
    return 0.0f;
}

f32 NuMouseReadZVel(void) {
    return 0.0f;
}

void NuMouseRead(void) {
}

f32 NuMouseReadX(void) {
    return 0.0f;
}
f32 NuMouseReadY(void) {
    return 0.0f;
}
f32 NuMouseReadZ(void) {
    return 0.0f;
}
u32 NuMouseReadButtons(void) {
    return 0;
}
