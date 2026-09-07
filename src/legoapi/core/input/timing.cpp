#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include <time.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 do_multiframe_update;

void TimingBars() {
}

// Original C++ entry point 0x2a6020, distinct from the C entry point.
i64 getCurrentTime() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (i64)ts.tv_sec * 1000 + (i64)ts.tv_nsec;
}

void SetFramesToWait(u32) {
}

void ResetFrameCounters() {
    MainFrameCounters.warmup_frame = 0;
    MainFrameCounters.every_frame = 0xff;
    MainFrameCounters.alternate_frame = 0xff;
    MainFrameCounters.third_frame = 0xff;
    MainFrameCounters.fourth_frame = 0xff;
}

void UpdateFrameCounters() {
    if (do_multiframe_update == 0) {
        ResetFrameCounters();
        return;
    }

    if (static_cast<i8>(MainFrameCounters.warmup_frame) <= 0) {
        ++MainFrameCounters.warmup_frame;
        return;
    }

    MainFrameCounters.every_frame = 1;
    MainFrameCounters.alternate_frame = (MainFrameCounters.alternate_frame + 1) & 1;
    if (++MainFrameCounters.third_frame == 3) {
        MainFrameCounters.third_frame = 0;
    }
    MainFrameCounters.fourth_frame = (MainFrameCounters.fourth_frame + 1) & 3;
}
