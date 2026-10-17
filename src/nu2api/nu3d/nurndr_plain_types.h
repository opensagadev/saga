#pragma once

// Types shared by nu2api/nu3d/nurndr_plain.cpp and nu2api/nu3d/android/numtl_android.cpp.

struct ScreenGrabTileParams {
    u16 width;
    i8 channels[4];
};
struct NuLineVertex2D {
    f32 x, y;
    f32 unused[2];
    i32 colour;
    f32 u, v;
};
