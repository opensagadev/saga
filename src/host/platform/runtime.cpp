#include "host/platform/runtime.hpp"

#include <SDL3/SDL.h>

#include "host/platform/keyboard_mapping.hpp"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"

namespace {

    u32 HostKeyboardActionButtons(HOST_KEYBOARD_ACTION action) {
        switch (action) {
            case HOST_KEYBOARD_START:
                return GAMEPAD_START | GAMEPAD_JUMP;
            case HOST_KEYBOARD_MOVE_UP:
                return GAMEPAD_DUP;
            case HOST_KEYBOARD_MOVE_DOWN:
                return GAMEPAD_DDOWN;
            case HOST_KEYBOARD_MOVE_LEFT:
                return GAMEPAD_DLEFT;
            case HOST_KEYBOARD_MOVE_RIGHT:
                return GAMEPAD_DRIGHT;
            case HOST_KEYBOARD_JUMP:
                return GAMEPAD_JUMP;
            case HOST_KEYBOARD_PRIMARY_ACTION:
                return GAMEPAD_ACTION;
            case HOST_KEYBOARD_SPECIAL:
                return GAMEPAD_SPECIAL;
            case HOST_KEYBOARD_CHARACTER_SWAP:
                return GAMEPAD_TAG;
            case HOST_KEYBOARD_TOGGLE_RIGHT:
                return GAMEPAD_TOGGLERIGHT;
        }
        return 0;
    }

} // namespace

__attribute__((weak)) const char *HostPlatformVideoDriver() {
    return "dummy";
}

__attribute__((weak)) bool HostPlatformAudioOutputEnabled() {
    return true;
}

__attribute__((weak)) ANativeWindow *HostPlatformNativeWindow(SDL_Window *) {
    return nullptr;
}

__attribute__((weak)) void HostPlatformPrepareArguments(i32 *, char ***) {
}

__attribute__((weak)) void HostPlatformHandleInputEvent(const SDL_Event &, i32, i32) {
}

u32 HostPlatformKeyboardButtons(const bool *keyboard) {
    u32 buttons = 0;
    for (const HOST_KEYBOARD_BINDING &binding : HOST_KEYBOARD_BINDINGS) {
        if (keyboard[binding.scancode]) {
            buttons |= HostKeyboardActionButtons(binding.action);
        }
    }
    return buttons;
}
