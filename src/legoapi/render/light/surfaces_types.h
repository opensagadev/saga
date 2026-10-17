#pragma once

// Types shared by legoapi/render/light/surfaces.cpp and legoapi/render/core/gameliball.cpp.

enum SurfaceDeflectMode : i32 {
    SURFACE_DEFLECT_SLIDE = 0,
    SURFACE_DEFLECT_PUSH_OUT_SLOW = 1,
    SURFACE_DEFLECT_PUSH_OUT_FAST = 2,
};
