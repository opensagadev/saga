#pragma once

#include "decomp.h"

extern "C" {
    void NuHtmlHBarGraph(void);
    void NuHtmlVBarGraph(void);
    void NuHtmlHLineGraph(void);
}

void setpoint(f32 x);
void setnextpoint(f32 x, f32 y);
i32 getnextdatapoint(f32 *value, i32 *delta);
