#pragma once

#include <SDL3/SDL.h>

namespace saga::host {

    void update_keyboard_state(const bool *keys) noexcept;
    void queue_key_event(SDL_Scancode scancode, SDL_Keymod modifiers);
    void set_editor_arrow_selection_enabled(bool enabled) noexcept;

} // namespace saga::host
