#include "decomp.h"
#include "globals.h"
#include "legoapi/core/input/timing.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 do_multiframe_update;
extern i32 TimingBarSet;
extern i32 app_tbgameset;
extern i32 app_tbplayerset;
extern i32 app_tbaiset;
extern i32 app_tbdrawset;

void TimingBars() {
    if (TimingBarSet == 0) {
        return;
    }
    NuFntSet(0);
    NuFntSetPen(0x7fffffff);
    NuFntScale(0x10, 0x20);
    switch (TimingBarSet) {
        case 1:
            NuTimeBarSetRender(1);
            break;
        case 2:
            NuTimeBarSetRender(app_tbgameset);
            break;
        case 3:
            NuTimeBarSetRender(app_tbplayerset);
            break;
        case 4:
            NuTimeBarSetRender(app_tbaiset);
            break;
        case 5:
            NuTimeBarSetRender(app_tbdrawset);
            break;
        default:
            NuTimeBarSetRender(0);
            break;
    }
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

void UpdatePickupFlicker() {
    STUBBED();
}
