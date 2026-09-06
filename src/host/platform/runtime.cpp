#include "host/platform/runtime.hpp"

#include <SDL3/SDL.h>

#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"

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
    buttons |= keyboard[SDL_SCANCODE_RETURN] ? GAMEPAD_START | GAMEPAD_JUMP : 0;
    buttons |= keyboard[SDL_SCANCODE_UP] || keyboard[SDL_SCANCODE_W] ? GAMEPAD_DUP : 0;
    buttons |= keyboard[SDL_SCANCODE_DOWN] || keyboard[SDL_SCANCODE_S] ? GAMEPAD_DDOWN : 0;
    buttons |= keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_A] ? GAMEPAD_DLEFT : 0;
    buttons |= keyboard[SDL_SCANCODE_RIGHT] || keyboard[SDL_SCANCODE_D] ? GAMEPAD_DRIGHT : 0;
    buttons |= keyboard[SDL_SCANCODE_U] ? GAMEPAD_JUMP : 0;
    buttons |= keyboard[SDL_SCANCODE_H] || keyboard[SDL_SCANCODE_E] ? GAMEPAD_ACTION : 0;
    buttons |= keyboard[SDL_SCANCODE_J] || keyboard[SDL_SCANCODE_F] ? GAMEPAD_SPECIAL : 0;
    buttons |= keyboard[SDL_SCANCODE_K] ? GAMEPAD_TAG : 0;
    buttons |= keyboard[SDL_SCANCODE_SPACE] ? GAMEPAD_TOGGLELEFT : 0;
    buttons |= keyboard[SDL_SCANCODE_LCTRL] ? GAMEPAD_TOGGLERIGHT : 0;
    return buttons;
}
