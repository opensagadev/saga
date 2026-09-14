#include "nu2api/nucore/nuhtml.h"

static f32 curx;
static f32 cury;
static f32 nextx;
static f32 dx;
i32 size;

extern "C" {
    void NuHtmlHBarGraph(void) {
        STUBBED();
    }

    void NuHtmlVBarGraph(void) {
        STUBBED();
    }
}

void setpoint(f32 x) {
    curx = x;
    cury = 0.0f;
}

void setnextpoint(f32 x, f32 y) {
    nextx = x;
    dx = (x - curx) / y;
    size = static_cast<i32>(y);
}

i32 getnextdatapoint(f32 *value, i32 *delta) {
    const f32 next_y = cury + 1.0f;
    const f32 old_x = curx;
    *value = old_x;
    const i32 old_value = static_cast<i32>(old_x);
    cury = next_y;
    --size;
    const f32 next_x = old_x + dx;
    curx = next_x;
    *delta = static_cast<i32>(next_x) - old_value;
    if (size != 0) {
        return 0;
    }
    curx = nextx;
    return -1;
}

extern "C" {
    void NuHtmlHLineGraph(void) {
        STUBBED();
    }
}
