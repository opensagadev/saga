#include "host/platform/keyboard.hpp"

#include <SDL3/SDL.h>

#include "nu2api/nucore/nukeyboard.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <deque>
#include <mutex>
#include <utility>

namespace {

    // NuKeyboard uses the original PC set-1 scan codes, not SDL scan codes.
    constexpr auto sdl_scancodes = [] {
        std::array<SDL_Scancode, 0xd4> keys{};
        keys[0x01] = SDL_SCANCODE_ESCAPE;
        keys[0x02] = SDL_SCANCODE_1;
        keys[0x03] = SDL_SCANCODE_2;
        keys[0x04] = SDL_SCANCODE_3;
        keys[0x05] = SDL_SCANCODE_4;
        keys[0x06] = SDL_SCANCODE_5;
        keys[0x07] = SDL_SCANCODE_6;
        keys[0x08] = SDL_SCANCODE_7;
        keys[0x09] = SDL_SCANCODE_8;
        keys[0x0a] = SDL_SCANCODE_9;
        keys[0x0b] = SDL_SCANCODE_0;
        keys[0x0c] = SDL_SCANCODE_MINUS;
        keys[0x0d] = SDL_SCANCODE_EQUALS;
        keys[0x0e] = SDL_SCANCODE_BACKSPACE;
        keys[0x0f] = SDL_SCANCODE_TAB;
        keys[0x10] = SDL_SCANCODE_Q;
        keys[0x11] = SDL_SCANCODE_W;
        keys[0x12] = SDL_SCANCODE_E;
        keys[0x13] = SDL_SCANCODE_R;
        keys[0x14] = SDL_SCANCODE_T;
        keys[0x15] = SDL_SCANCODE_Y;
        keys[0x16] = SDL_SCANCODE_U;
        keys[0x17] = SDL_SCANCODE_I;
        keys[0x18] = SDL_SCANCODE_O;
        keys[0x19] = SDL_SCANCODE_P;
        keys[0x1a] = SDL_SCANCODE_LEFTBRACKET;
        keys[0x1b] = SDL_SCANCODE_RIGHTBRACKET;
        keys[0x1c] = SDL_SCANCODE_RETURN;
        keys[0x1d] = SDL_SCANCODE_LCTRL;
        keys[0x1e] = SDL_SCANCODE_A;
        keys[0x1f] = SDL_SCANCODE_S;
        keys[0x20] = SDL_SCANCODE_D;
        keys[0x21] = SDL_SCANCODE_F;
        keys[0x22] = SDL_SCANCODE_G;
        keys[0x23] = SDL_SCANCODE_H;
        keys[0x24] = SDL_SCANCODE_J;
        keys[0x25] = SDL_SCANCODE_K;
        keys[0x26] = SDL_SCANCODE_L;
        keys[0x27] = SDL_SCANCODE_SEMICOLON;
        keys[0x28] = SDL_SCANCODE_APOSTROPHE;
        keys[0x29] = SDL_SCANCODE_GRAVE;
        keys[0x2a] = SDL_SCANCODE_LSHIFT;
        keys[0x2b] = SDL_SCANCODE_BACKSLASH;
        keys[0x2c] = SDL_SCANCODE_Z;
        keys[0x2d] = SDL_SCANCODE_X;
        keys[0x2e] = SDL_SCANCODE_C;
        keys[0x2f] = SDL_SCANCODE_V;
        keys[0x30] = SDL_SCANCODE_B;
        keys[0x31] = SDL_SCANCODE_N;
        keys[0x32] = SDL_SCANCODE_M;
        keys[0x33] = SDL_SCANCODE_COMMA;
        keys[0x34] = SDL_SCANCODE_PERIOD;
        keys[0x35] = SDL_SCANCODE_SLASH;
        keys[0x36] = SDL_SCANCODE_RSHIFT;
        keys[0x37] = SDL_SCANCODE_KP_MULTIPLY;
        keys[0x38] = SDL_SCANCODE_LALT;
        keys[0x39] = SDL_SCANCODE_SPACE;
        keys[0x3a] = SDL_SCANCODE_CAPSLOCK;
        keys[0x3b] = SDL_SCANCODE_F1;
        keys[0x3c] = SDL_SCANCODE_F2;
        keys[0x3d] = SDL_SCANCODE_F3;
        keys[0x3e] = SDL_SCANCODE_F4;
        keys[0x3f] = SDL_SCANCODE_F5;
        keys[0x40] = SDL_SCANCODE_F6;
        keys[0x41] = SDL_SCANCODE_F7;
        keys[0x42] = SDL_SCANCODE_F8;
        keys[0x43] = SDL_SCANCODE_F9;
        keys[0x44] = SDL_SCANCODE_F10;
        keys[0x45] = SDL_SCANCODE_NUMLOCKCLEAR;
        keys[0x46] = SDL_SCANCODE_SCROLLLOCK;
        keys[0x47] = SDL_SCANCODE_KP_7;
        keys[0x48] = SDL_SCANCODE_KP_8;
        keys[0x49] = SDL_SCANCODE_KP_9;
        keys[0x4a] = SDL_SCANCODE_KP_MINUS;
        keys[0x4b] = SDL_SCANCODE_KP_4;
        keys[0x4c] = SDL_SCANCODE_KP_5;
        keys[0x4d] = SDL_SCANCODE_KP_6;
        keys[0x4e] = SDL_SCANCODE_KP_PLUS;
        keys[0x4f] = SDL_SCANCODE_KP_1;
        keys[0x50] = SDL_SCANCODE_KP_2;
        keys[0x51] = SDL_SCANCODE_KP_3;
        keys[0x52] = SDL_SCANCODE_KP_0;
        keys[0x53] = SDL_SCANCODE_KP_PERIOD;
        keys[0x57] = SDL_SCANCODE_F11;
        keys[0x58] = SDL_SCANCODE_F12;
        keys[0x9d] = SDL_SCANCODE_RCTRL;
        keys[0xb8] = SDL_SCANCODE_RALT;
        keys[0xc8] = SDL_SCANCODE_UP;
        keys[0xcb] = SDL_SCANCODE_LEFT;
        keys[0xcd] = SDL_SCANCODE_RIGHT;
        keys[0xd0] = SDL_SCANCODE_DOWN;
        keys[0xd2] = SDL_SCANCODE_INSERT;
        keys[0xd3] = SDL_SCANCODE_DELETE;
        return keys;
    }();

    std::array<std::atomic<bool>, sdl_scancodes.size()> held{};
    std::atomic<bool> editor_arrow_selection_enabled{true};
    std::mutex key_queue_mutex;
    std::deque<std::pair<i32, u32>> key_queue;
    bool editing_property_text = false;

} // namespace

namespace saga::host {

    void set_editor_arrow_selection_enabled(bool enabled) noexcept {
        editor_arrow_selection_enabled.store(enabled, std::memory_order_release);
    }

    void update_keyboard_state(const bool *keys) noexcept {
        for (std::size_t index = 0; index < held.size(); ++index) {
            const SDL_Scancode scancode = sdl_scancodes[index];
            bool down = keys && scancode != SDL_SCANCODE_UNKNOWN && keys[scancode];
            if (keys && index == 0x1d)
                down = down || keys[SDL_SCANCODE_RCTRL];
            if (keys && index == 0x38)
                down = down || keys[SDL_SCANCODE_RALT];
            held[index].store(down, std::memory_order_release);
        }
    }

    void queue_key_event(SDL_Scancode scancode, SDL_Keymod modifiers) {
        const auto match = std::find(sdl_scancodes.begin(), sdl_scancodes.end(), scancode);
        if (match == sdl_scancodes.end() || scancode == SDL_SCANCODE_UNKNOWN)
            return;
        const i32 key = static_cast<i32>(match - sdl_scancodes.begin());
        const u32 original_modifiers = (modifiers & SDL_KMOD_SHIFT) != 0 ? 1u : 0u;
        std::lock_guard lock{key_queue_mutex};
        // The game maps WASD to movement; feeding those keystrokes to the
        // editor's optional menu type-ahead also changes its selection. The
        // property editor explicitly calls NuKeyFlush when text entry begins.
        if (!editing_property_text)
            return;
        if (key_queue.size() == 256)
            key_queue.pop_front();
        key_queue.emplace_back(key, original_modifiers);
    }

} // namespace saga::host

extern "C" i32 __wrap_NuKeyboard(i32 key) {
    if ((key == 0xcb || key == 0xcd) && !editor_arrow_selection_enabled.load(std::memory_order_acquire))
        return 0;
    return key >= 0 && static_cast<std::size_t>(key) < held.size()
               ? held[static_cast<std::size_t>(key)].load(std::memory_order_acquire)
               : 0;
}

extern "C" i32 __wrap_NuKeyGet(u32 *modifiers) {
    std::lock_guard lock{key_queue_mutex};
    if (!editing_property_text || key_queue.empty())
        return -1;
    const auto [key, flags] = key_queue.front();
    key_queue.pop_front();
    if (key == 0x1c || key == 0x01)
        editing_property_text = false;
    if (modifiers)
        *modifiers = flags;
    return key;
}

extern "C" void __wrap_NuKeyFlush() {
    std::lock_guard lock{key_queue_mutex};
    key_queue.clear();
    editing_property_text = true;
}
