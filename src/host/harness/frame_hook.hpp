#pragma once

namespace saga::host::harness {

    using FrameCallback = void (*)();

    void set_frame_callback(FrameCallback callback) noexcept;
    void run_frame_callback() noexcept;

} // namespace saga::host::harness
