#include "host/platform/runtime.hpp"

#include <SDL3/SDL.h>
#include <emscripten.h>

#include "host/platform/input.hpp"
#include "legoapi/characters/core/players.h"

namespace {
    char *host_wasm_arguments[4];
}

const char *HostPlatformVideoDriver() {
    return "emscripten";
}

bool HostPlatformAudioOutputEnabled() {
    return true;
}

ANativeWindow *HostPlatformNativeWindow(SDL_Window *) {
    return nullptr;
}

void HostPlatformPrepareArguments(i32 *argc, char ***argv) {
    if (*argc >= 2) {
        return;
    }

    host_wasm_arguments[0] = (*argv)[0];
    host_wasm_arguments[1] = const_cast<char *>("window");
    host_wasm_arguments[2] = nullptr;
    *argc = 2;
    *argv = host_wasm_arguments;
    // This macro contains JavaScript, whose strict equality operator is not C++ syntax.
    // clang-format off
    if (MAIN_THREAD_EM_ASM_INT({
            return (location.hostname === '127.0.0.1' || location.hostname === 'localhost') &&
                   new URLSearchParams(location.search).get('movement-trace') === '1';
        })) {
        // clang-format on
        host_wasm_arguments[(*argc)++] = const_cast<char *>("--trace-movement");
        host_wasm_arguments[*argc] = nullptr;
    }
}

void HostPlatformHandleInputEvent(const SDL_Event &event, i32 width, i32 height) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        HostInputTouch(static_cast<i32>(event.button.x), static_cast<i32>(event.button.y), width, height);
    } else if (event.type == SDL_EVENT_FINGER_DOWN) {
        HostInputTouch(static_cast<i32>(event.tfinger.x * width), static_cast<i32>(event.tfinger.y * height), width,
                       height);
    }
}
