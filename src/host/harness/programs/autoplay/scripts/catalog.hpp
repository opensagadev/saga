#pragma once

// Level-keyed registry for playthrough definitions from the scripts directory.

namespace scripts {

    struct CaseInsensitiveLevelName {
        bool operator()(const std::string &left, const std::string &right) const {
            return SDL_strcasecmp(left.c_str(), right.c_str()) < 0;
        }
    };

    using Catalog = std::map<std::string, AutoplayScript, CaseInsensitiveLevelName>;

    const Catalog by_level = {
        {"gungan_a", gungan},
        {"maul_a", darth_maul},
        {"maul_b", darth_maul},
        {"maul_d", darth_maul},
        {"maul_e", darth_maul},
        {"maul_f", darth_maul},
        {"negotiations_a", negotiations},
        {"podrace_b", podrace},
        {"podrace_c", podrace},
        {"rescue_a", escape_from_naboo},
        {"retake_a", retake_theed_palace},
        {"retake_b", retake_theed_palace},
        {"retake_d", retake_theed_palace},
        {"retake_e", retake_theed_palace},
        {"retake_f", retake_theed_palace},
        {"retake_g", retake_theed_palace},
    };

    const std::vector<std::string> story_order = {
        "negotiations_a", "gungan_a", "rescue_a", "podrace_b", "retake_a", "maul_a",
    };

} // namespace scripts
