#pragma once

#include "decomp.h"
#include "nu2api/numath/numtx.h"

struct GameObject_s;

struct _vuv_s {
    float x, y, z, w;
};

// FlightSpline_Init uses this 0x52c-byte stride. The evaluator and pod
// creation agree on the point array, count, total distance, and spline ID.
struct flightspline_s {
    _vuv_s points[64];
    i32 point_count; // 0x400
    union {
        u8 unknown_404[4];
        f32 spawn_time;
    };
    f32 field_0x408;
    f32 field_0x40c;
    f32 length;                   // 0x410
    f32 cumulative_distances[64]; // 0x414
    i32 field_0x514;
    union {
        u8 unknown_518[4];
        i32 repeat_count;
    };
    i32 field_0x51c;
    i32 field_0x520;
    i32 id; // 0x524
    i32 unknown_528;
};
DECOMP_ASSERT(sizeof(flightspline_s) == 0x52c, "flight spline size");
DECOMP_ASSERT(offsetof(flightspline_s, point_count) == 0x400, "flight spline point count offset");
DECOMP_ASSERT(offsetof(flightspline_s, field_0x408) == 0x408, "flight spline first file parameter offset");
DECOMP_ASSERT(offsetof(flightspline_s, field_0x40c) == 0x40c, "flight spline version two parameter offset");
DECOMP_ASSERT(offsetof(flightspline_s, length) == 0x410, "flight spline length offset");
DECOMP_ASSERT(offsetof(flightspline_s, cumulative_distances) == 0x414, "flight spline distance table offset");
DECOMP_ASSERT(offsetof(flightspline_s, id) == 0x524, "flight spline ID offset");
DECOMP_ASSERT(offsetof(flightspline_s, field_0x514) == 0x514, "flight spline version two integer offset");
DECOMP_ASSERT(offsetof(flightspline_s, field_0x51c) == 0x51c, "flight spline version three parameter offset");
DECOMP_ASSERT(offsetof(flightspline_s, field_0x520) == 0x520, "flight spline version three index offset");

// One pod in the race state. Shared with the level-state allocator so host
// allocations follow pointer-bearing fields instead of the target byte size.
struct racepod_s {
    NUMTX matrix;
    _vuv_s previous_axis;
    _vuv_s previous_position;
    _vuv_s previous_displacement; // 0x60
    i32 pitch;
    i32 yaw;
    i32 pad_0x78;
    float speed;
    union {
        u32 *data;
        flightspline_s *spline;
    };
    float start;
    i16 model_id;
    i16 pad_0x8a;
    float distance;
    GameObject_s *object;
    void *next;
};
using PODRACE_LAPENTRY_s = racepod_s;
DECOMP_ASSERT(sizeof(racepod_s) == 0x98, "racepod layout");

struct PODRACE_s {
    flightspline_s splines[32];
    PODRACE_LAPENTRY_s lap_entries[0x10];
    float lap_countdown;
    float mushroom_timer;
    float lap_display;
    float prev_lap_display;
    float max_lap_time;
    float lap_time_increment;
    i32 lap_attempts_per_increment;
    i32 lap_attempts; // 0xaf1c
    u8 flags;
    char pad_0xaf21[0xaf24 - 0xaf21];
};
DECOMP_ASSERT(offsetof(PODRACE_s, lap_entries) == 0xa580, "podrace lap entries offset");
DECOMP_ASSERT(sizeof(PODRACE_s) == 0xaf24, "podrace state size");
