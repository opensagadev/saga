#include "host/platform/runtime.hpp"
#include "gameapi/gui/apimenu.h"

#include <SDL3/SDL.h>

extern "C" i32 __real_MenuInMemoryCard();

extern "C" i32 __wrap_MenuInMemoryCard() {
    // After MenuReset, the reference reads MenuInfo[-1].id from zero padding
    // at 0x665a24. Reproduce its false result without an invalid host read.
    if (GameMenuLevel != -1 && GameMenu[GameMenuLevel].menu == -1) {
        return 0;
    }
    return __real_MenuInMemoryCard();
}

const char *HostPlatformVideoDriver() {
    return "x11";
}

ANativeWindow *HostPlatformNativeWindow(SDL_Window *window) {
    const SDL_PropertiesID properties = SDL_GetWindowProperties(window);
    const i32 handle = static_cast<i32>(SDL_GetNumberProperty(properties, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
    return reinterpret_cast<ANativeWindow *>(handle);
}

// Recovery instrumentation is limited to the particle editor translation unit.
// Other ASan-instrumented code retains its non-recovering report calls.
extern "C" __attribute__((weak)) const char *__asan_default_options() {
    return "halt_on_error=0";
}
