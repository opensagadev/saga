#include "nu2api/nucore/nutime.h"

#include "nu2api/nucore/nuapi.h"

void NuTimeGet(NUTIME *t) {
    NuTimeGetTicksPS(&t->low, (u32 *)&t->high);
}

f32 NuTimeGetFrameTime(void) {
    return nuapi.frametime;
}

void NuTimeForceFrameTime(f32 frame_time) {
    nuapi.forced_frame_time = frame_time;
}

void NuTimeWait(f32 milliseconds) {
    f32 elapsed = 0.0f;
    NUTIME start;
    NUTIME current;
    NUTIME difference;

    NuTimeGet(&start);
    while (elapsed < milliseconds) {
        NuTimeGet(&current);
        NuTimeSub(&difference, &current, &start);
        elapsed = NuTimeMilliSeconds(&difference);
    }
}

f32 NuTimeScanlines(NUTIME *t) {
    return NuTimeSeconds(t) * 272.0f * 60.0f;
}

f32 NuTimeSeconds(NUTIME *t) {
    u32 low;
    u32 high;
    f64 ticks;
    f64 ticks_per_second;
    f64 seconds;

    NuTimeGetTicksPerSecondPS(&low, &high);

    ticks = t->low + t->high * 4.294967295e+09;
    ticks_per_second = low + high * 4.294967295e+09;

    seconds = ticks / ticks_per_second;

    return seconds;
}

f32 NuTimeMilliSeconds(NUTIME *t) {
    u32 low;
    u32 high;
    f64 ticks;
    f64 ticks_per_second;
    f64 millis;

    NuTimeGetTicksPerSecondPS(&low, &high);

    ticks = t->low + t->high * 4.294967295e+09;
    ticks_per_second = low + high * 4.294967295e+09;

    millis = ticks / ticks_per_second * 1000.0;

    return millis;
}

f32 NuTimeMicroSeconds(NUTIME *t) {
    u32 low;
    u32 high;
    f64 ticks;
    f64 ticks_per_second;
    f64 micros;

    NuTimeGetTicksPerSecondPS(&low, &high);

    ticks = t->low + t->high * 4.294967295e+09;
    ticks_per_second = low + high * 4.294967295e+09;

    micros = ticks / ticks_per_second * 1000000.0;

    return micros;
}

void NuTimeSub(NUTIME *t, NUTIME *a, NUTIME *b) {
    if (a->low >= b->low) {
        t->low = a->low - b->low;
        t->high = a->high - b->high;
    } else {
        t->low = a->low - b->low;
        t->high = a->high - 1 - b->high;
    }
}
