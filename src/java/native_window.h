#pragma once
#include <stdint.h>
struct ANativeWindow;
extern "C" {
    int32_t ANativeWindow_getWidth(ANativeWindow *);
    int32_t ANativeWindow_getHeight(ANativeWindow *);
    int32_t ANativeWindow_setBuffersGeometry(ANativeWindow *, int32_t, int32_t, int32_t);
}
