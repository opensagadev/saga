#include "host/platform/mouse.hpp"

#include "decomp.h"

#include <atomic>
#include <mutex>

extern i32 global_frame_count;

namespace {

    std::atomic<bool> editor_mouse_enabled{false};
    std::atomic<u32> mouse_buttons{0};
    std::atomic<float> normalized_mouse_x{0.5f};
    std::atomic<float> normalized_mouse_y{0.5f};
    std::mutex motion_mutex;
    float pending_delta_x = 0.0f;
    float pending_delta_y = 0.0f;
    float frame_delta_x = 0.0f;
    float frame_delta_y = 0.0f;
    i32 motion_frame = -1;
    float previous_x = 0.0f;
    float previous_y = 0.0f;
    bool previous_position_valid = false;

    float read_motion(bool horizontal) {
        std::lock_guard lock{motion_mutex};
        if (motion_frame != global_frame_count) {
            frame_delta_x = pending_delta_x;
            frame_delta_y = pending_delta_y;
            pending_delta_x = 0.0f;
            pending_delta_y = 0.0f;
            motion_frame = global_frame_count;
        }
        return horizontal ? frame_delta_x : frame_delta_y;
    }

} // namespace

namespace saga::host {

    void set_editor_mouse_enabled(bool enabled) noexcept {
        editor_mouse_enabled.store(enabled, std::memory_order_release);
        if (!enabled) {
            std::lock_guard lock{motion_mutex};
            pending_delta_x = 0.0f;
            pending_delta_y = 0.0f;
            frame_delta_x = 0.0f;
            frame_delta_y = 0.0f;
            mouse_buttons.store(0, std::memory_order_release);
        }
    }

    void update_mouse_state(float x, float y, SDL_MouseButtonFlags buttons, int width, int height) noexcept {
        const bool enabled = editor_mouse_enabled.load(std::memory_order_acquire);
        // eduiProcessCursorDefault applies -velocity * 0.1 in its 640x224 space.
        const float delta_x =
            enabled && previous_position_valid && width > 0 ? -10.0f * (x - previous_x) * (640.0f / width) : 0.0f;
        const float delta_y =
            enabled && previous_position_valid && height > 0 ? -10.0f * (y - previous_y) * (224.0f / height) : 0.0f;
        previous_x = x;
        previous_y = y;
        previous_position_valid = true;
        if (width > 0 && height > 0) {
            normalized_mouse_x.store(x / static_cast<float>(width), std::memory_order_release);
            normalized_mouse_y.store(y / static_cast<float>(height), std::memory_order_release);
        }
        {
            std::lock_guard lock{motion_mutex};
            pending_delta_x += delta_x;
            pending_delta_y += delta_y;
        }

        u32 mapped_buttons = 0;
        if (enabled && (buttons & SDL_BUTTON_LMASK))
            mapped_buttons |= 1;
        if (enabled && (buttons & SDL_BUTTON_RMASK))
            mapped_buttons |= 2;
        mouse_buttons.store(mapped_buttons, std::memory_order_release);
    }

    std::pair<float, float> editor_mouse_position() noexcept {
        return {normalized_mouse_x.load(std::memory_order_acquire), normalized_mouse_y.load(std::memory_order_acquire)};
    }

} // namespace saga::host

extern "C" f32 __wrap_NuMouseReadXVel() {
    return read_motion(true);
}

extern "C" f32 __wrap_NuMouseReadYVel() {
    return read_motion(false);
}

extern "C" f32 __wrap_NuMouseReadZVel() {
    return 0.0f;
}

extern "C" u32 __wrap_NuMouseReadButtons() {
    return mouse_buttons.load(std::memory_order_acquire);
}
