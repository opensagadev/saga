#pragma once

// Level-keyed, non-owning registry for playthrough definitions. Multiple area
// names can reference one immutable script without copying its action graph.

namespace scripts {

    struct CaseInsensitiveLevelName {
        using is_transparent = void;

        bool operator()(std::string_view left, std::string_view right) const {
            return SDL_strcasecmp(left.data(), right.data()) < 0;
        }
    };

    using ScriptReference = std::reference_wrapper<const AutoplayScript>;
    using Catalog = std::map<std::string_view, ScriptReference, CaseInsensitiveLevelName>;

    const Catalog by_level{
        {"gungan_a", std::cref(gungan)},
        {"maul_a", std::cref(darth_maul)},
        {"maul_b", std::cref(darth_maul)},
        {"maul_d", std::cref(darth_maul)},
        {"maul_e", std::cref(darth_maul)},
        {"maul_f", std::cref(darth_maul)},
        {"negotiations_a", std::cref(negotiations)},
        {"podrace_b", std::cref(podrace)},
        {"podrace_c", std::cref(podrace)},
        {"rescue_a", std::cref(escape_from_naboo)},
        {"retake_a", std::cref(retake_theed_palace)},
        {"retake_b", std::cref(retake_theed_palace)},
        {"retake_d", std::cref(retake_theed_palace)},
        {"retake_e", std::cref(retake_theed_palace)},
        {"retake_f", std::cref(retake_theed_palace)},
        {"retake_g", std::cref(retake_theed_palace)},
    };

    constexpr std::array<std::string_view, 6> story_order{
        "negotiations_a", "gungan_a", "rescue_a", "podrace_b", "retake_a", "maul_a",
    };

} // namespace scripts
