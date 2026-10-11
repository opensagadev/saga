#include "host/platform/runtime.hpp"

#include <SDL3/SDL.h>
#include <emscripten.h>

#include "host/platform/input.hpp"
#include "host/platform/opensl.hpp"
#include "legoapi/characters/core/players.h"

namespace {
    char *host_wasm_arguments[3];
}

const char *HostPlatformVideoDriver() {
    return "emscripten";
}

bool HostPlatformAudioOutputEnabled() {
    return true;
}

u64 HostPlatformWindowFlags() {
    return SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
}

ANativeWindow *HostPlatformNativeWindow(SDL_Window *) {
    return nullptr;
}

void HostPlatformConfigureSDL() {
    // The game is embedded in a page: only take keys while the canvas has focus,
    // so the rest of the page keeps normal keyboard scrolling and form input.
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
}

void HostPlatformPrepareArguments(i32 *argc, char ***argv) {
    // The page may pass harness arguments to callMain; otherwise run the game.
    if (*argc >= 2) {
        return;
    }

    host_wasm_arguments[0] = (*argv)[0];
    host_wasm_arguments[1] = const_cast<char *>("window");
    host_wasm_arguments[2] = nullptr;
    *argc = 2;
    *argv = host_wasm_arguments;
}

void HostPlatformHandleInputEvent(const SDL_Event &event, i32 width, i32 height) {
    if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN && event.type != SDL_EVENT_FINGER_DOWN) {
        return;
    }
    SDL_Window *window = SDL_GetWindowFromEvent(&event);
    if (window != nullptr) {
        SDL_GetWindowSize(window, &width, &height);
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        HostInputTouch(static_cast<i32>(event.button.x), static_cast<i32>(event.button.y), width, height);
    } else if (event.type == SDL_EVENT_FINGER_DOWN) {
        HostInputTouch(static_cast<i32>(event.tfinger.x * width), static_cast<i32>(event.tfinger.y * height), width,
                       height);
    }
}

// Called by the web page's volume control from the browser main thread.
extern "C" EMSCRIPTEN_KEEPALIVE void saga_set_volume(f32 volume) {
    hostsl::HostSetMasterGain(volume < 0.0f ? 0.0f : volume > 1.0f ? 1.0f : volume);
}
