// Exercise the actual reset callbacks with controlled floor-query results.
// Matrix helpers are inert because these cases check floor state and angle dispatch.
#ifndef LEVER_SOURCE
#define LEVER_SOURCE "legoapi/gizmos/object/lever.cpp"
#endif
#ifndef DOOR_SOURCE
#define DOOR_SOURCE "legoapi/props/doors/securitydoor.cpp"
#endif
#ifndef ZIPUP_SOURCE
#define ZIPUP_SOURCE "legoapi/props/objects/zipup.cpp"
#endif
#include LEVER_SOURCE
#include DOOR_SOURCE
#include ZIPUP_SOURCE
#include <math.h>
#include <stdio.h>
WORLDINFO *WORLD;
LEVELDATA *CLOUDCITYESCAPEC_LDATA;
NUVEC ShadNorm = {0, 1, 0};
static float next_height;
static int angle_calls;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32) {
    return next_height;
}
void FindAnglesZX(NUVEC *, u16 *x, u16 *z) {
    ++angle_calls;
    *x = 123;
    *z = 456;
}
extern "C" void NuVecRotateX(NUVEC *out, NUVEC *in, NUANG) {
    *out = *in;
}
extern "C" void NuVecRotateY(NUVEC *out, NUVEC *in, NUANG) {
    *out = *in;
}
extern "C" void NuVecAdd(NUVEC *out, NUVEC *a, NUVEC *b) {
    *out = {a->x + b->x, a->y + b->y, a->z + b->z};
}
extern "C" void NuMtxSetRotationY(NUMTX *, NUANG) {
}
extern "C" void NuMtxTranslate(NUMTX *, NUVEC *) {
}
extern "C" void NuMtxRotateY(NUMTX *, NUANG) {
}
extern "C" void NewTerrPlatformsOff() {
}
extern "C" void PlatInstRotate(i32, i32) {
}
extern "C" i32 NuAtan2D(f32, f32) {
    return 0;
}
static unsigned failures, checks;
static void check(bool yes, const char *msg) {
    ++checks;
    if (!yes) {
        fprintf(stderr, "FAIL %s\n", msg);
        ++failures;
    }
}
static bool equal(float a, float b) {
    return isnan(b) ? isnan(a) : a == b;
}
int main() {
    float heights[] = {2000000.0f, -1.0f, -78.0f, 0.0f, 1.0f, __builtin_nanf("")};
    WORLDINFO world = {};
    WORLD = &world;
    for (float height : heights) {
        next_height = height;
        bool missing = height == 2000000.0f;
        float expected = missing ? 2000000.0f : height + 0.005f;
        LEVER_s lever = {};
        angle_calls = 0;
        Lever_Reset(&lever);
        check(equal(lever.target_offset.y, expected), "lever target floor/sentinel");
        check(angle_calls == (missing ? 0 : 1), "lever skips angles only for missing floor");
        SECURITYDOOR door = {};
        world.security_doors = &door;
        world.security_door_count = 1;
        angle_calls = 0;
        SecurityDoors_Reset(&world, NULL, NULL);
        check(equal(door.player_position.y, expected), "door player floor offset");
        check(angle_calls == (missing ? 0 : 1), "door skips angles only for missing floor");
        ZIPUP zipup = {};
        world.zipups = &zipup;
        world.zipup_count = 1;
        angle_calls = 0;
        ZipUps_Reset(&world, NULL, NULL);
        check(equal(zipup.lower_ground_height, expected), "zipup lower floor/sentinel");
        check(equal(zipup.upper_ground_height, expected), "zipup upper floor/sentinel");
        check(angle_calls == (missing ? 0 : 2), "zipup skips angles only for missing floor");
    }
    printf("terrain callbacks: 18 cases, %u assertions, %u failures\n", checks, failures);
    return failures ? 1 : 0;
}
