#pragma once

#include <SDL3/SDL.h>
#include <utility>

namespace saga::host {

    void begin_editor_mouse_frame() noexcept;
    void set_editor_mouse_enabled(bool enabled) noexcept;
    void update_mouse_state(float x, float y, SDL_MouseButtonFlags buttons, int width, int height) noexcept;
    void add_editor_mouse_wheel(float y) noexcept;
    std::pair<float, float> editor_mouse_position() noexcept;

} // namespace saga::host
