#include "host/harness/frame_hook.hpp"

#include <atomic>

namespace saga::host::harness {
    namespace {

        std::atomic<FrameCallback> callback;

    } // namespace

    void set_frame_callback(FrameCallback next) noexcept {
        callback.store(next, std::memory_order_release);
    }

    void run_frame_callback() noexcept {
        if (const auto current = callback.load(std::memory_order_acquire))
            current();
    }

} // namespace saga::host::harness
