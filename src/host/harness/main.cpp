#include "host/harness/programs/programs.hpp"
#include "host/platform/runtime.hpp"
#include "java/android.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string_view>

#include <unistd.h>

#if defined(__SANITIZE_ADDRESS__)
// Original game code can trip ASan during normal play. Keep diagnostics visible
// without terminating the game thread; ASAN_OPTIONS can override this default.
extern "C" const char *__asan_default_options() {
    return "halt_on_error=0";
}
#endif

namespace saga::host::harness {
    namespace {

        template <std::size_t program_count>
        void print_usage(std::string_view executable, const std::array<const Program *, program_count> &programs) {
            std::cout << "Usage: " << executable << " <program> [options]\n\nHost programs:\n";
            for (const Program *program : programs)
                std::cout << "  " << program->name << "\t" << program->description << '\n';
            std::cout << "\nRun '" << executable << " <program> --help' for program-specific options.\n";
        }

        void initialize_language() {
            const char *locale = std::getenv("LANG");
            if (!locale)
                locale = "en-us";

            std::snprintf(g_language, sizeof(g_language), "%.63s", locale);
            std::replace(std::begin(g_language), std::end(g_language), '_', '-');
        }

        [[noreturn]] void finish_engine_session(int status) {
            // NuMain is process-lifetime code. Host programs can finish while its
            // worker threads still exist, so keep that hard boundary in one place.
            std::cout.flush();
            std::cerr.flush();
            std::clog.flush();
            std::fflush(nullptr);
            _exit(status);
        }

    } // namespace

    int run(int argc, char **argv) {
        HostPlatformPrepareArguments(&argc, &argv);

        const std::array programs{&window_program(), &editor_program()};
        const Arguments arguments{argc, argv};
        const std::string_view executable = arguments.empty() ? "saga" : arguments[0];
        if (arguments.size() < 2 || arguments[1] == "--help" || arguments[1] == "-h") {
            print_usage(executable, programs);
            return arguments.size() < 2 ? 1 : 0;
        }

        const std::string_view requested = arguments[1];
        const auto program = std::find_if(programs.begin(), programs.end(),
                                          [requested](const Program *entry) { return entry->name == requested; });
        if (program == programs.end()) {
            std::cerr << "Unknown host program: " << requested << "\n\n";
            print_usage(executable, programs);
            return 1;
        }

        initialize_language();
        const int result = (*program)->run({executable, arguments.drop(2)});
        finish_engine_session(result);
    }

} // namespace saga::host::harness

int main(int argc, char **argv) {
    return saga::host::harness::run(argc, argv);
}
