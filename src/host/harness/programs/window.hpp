#pragma once

#include <SDL3/SDL_events.h>

#include "decomp.h"
#include "host/harness/program.hpp"

#include <string_view>
#include <variant>

namespace saga::host::harness {

    struct WindowHooks final {
        void (*handle_event)(const SDL_Event &) = nullptr;
        u32 (*filter_game_input)(u32 buttons) = nullptr;
        bool (*free_camera_ready)() = nullptr;
        int (*requested_exit_status)() = nullptr;
    };

    struct WindowOptions final {
        const char *documents_path = nullptr;
        bool capture = false;
        bool trace_movement = false;
        bool script_input = false;
        bool script_load = false;
        bool script_play = false;
        bool script_action = false;
        bool script_pause = false;
        bool camera_orbit = false;
        bool camera_free = false;
        bool hide_system_cursor = false;
        bool offscreen = false;
        bool mute = false;
        bool show_fps = false;
        bool msaa = true;
        bool portals = true;
        u64 script_tail_ms = 8000;
        u64 timeout_ms = 0;
        WindowHooks hooks;
    };

    enum class ParseDisposition {
        help,
        error,
    };

    struct ParseFailure final {
        ParseDisposition disposition;
    };

    using WindowParseResult = std::variant<WindowOptions, ParseFailure>;

    [[nodiscard]] WindowParseResult parse_window_options(const Invocation &invocation, std::string_view program_name);
    [[nodiscard]] int run_window(const WindowOptions &options);
    [[nodiscard]] const Program &window_program();

} // namespace saga::host::harness
