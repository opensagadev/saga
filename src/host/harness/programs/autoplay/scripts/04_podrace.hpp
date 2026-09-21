#pragma once

namespace scripts {

    const AutoplayScript podrace{
        "Boonta Eve Classic: follow the authored pod track through all three laps",
        {
            dsl::wait_loaded(),
            dsl::wait_time(3500),
            dsl::start_podrace(),
            dsl::drive_podrace_to_level("podrace_c"),
            dsl::drive_podrace_to_level("podrace_a"),
            dsl::drive_podrace_to_level("podrace_b"),
            dsl::drive_podrace_to_level("podrace_c"),
            dsl::drive_podrace_to_level("podrace_a"),
            dsl::drive_podrace_to_level("podrace_b"),
            dsl::drive_podrace_to_level("podrace_c"),
            dsl::drive_podrace_to_level("podrace_a"),
            dsl::drive_podrace_to_level("PodRace_Outro1"),
        },
        AutoplayOptions{},
        Milliseconds{900000},
    };

} // namespace scripts
