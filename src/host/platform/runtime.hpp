#pragma once

#include "nu2api/nucore/common.h"

struct ANativeWindow;
union SDL_Event;
struct SDL_Window;

// Common host entry points have weak fallbacks in runtime.cpp. A selected
// platform directory may replace any of them with a strong definition.
const char *HostPlatformVideoDriver();
// Sets platform SDL hints before SDL_Init.
void HostPlatformConfigureSDL();
bool HostPlatformAudioOutputEnabled();
u64 HostPlatformWindowFlags();
ANativeWindow *HostPlatformNativeWindow(SDL_Window *window);
#if defined(__linux__) && !defined(__EMSCRIPTEN__)
void HostSetSDLGraphics(SDL_Window *window, void *context);
#endif
void HostPlatformPrepareArguments(i32 *argc, char ***argv);
void HostPlatformHandleInputEvent(const SDL_Event &event, i32 width, i32 height);
u32 HostPlatformKeyboardButtons(const bool *keyboard);
