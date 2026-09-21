#include <SDL3/SDL.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <limits>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <unistd.h>

#include "decomp.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "host/harness/fixture.hpp"
#include "host/harness/programs/programs.hpp"
#include "host/harness/startup.hpp"
#include "host/platform/input.hpp"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nugscn.h"

extern i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode);
extern u8 cutskip_dontplaylevelintro;
extern void PodRace_IncreaseLap();
extern void ReleasePush(GameObject_s *object);
extern void GizPanel_Use(GameObject_s &object, GIZPANEL_s &panel);
extern void ZipUp_MoveCode(GameObject_s *object, i32 special_pressed);
extern rtldata_s lev_rtldata;
extern void AISysGetPathPos2(AISYS_s *system, NUVEC *position, AIPATHINFO_s *path_info, NUVEC *path_position,
                             AIPATH_s *path, i32 checks);
#if defined(__linux__) && !defined(__EMSCRIPTEN__)
extern "C" void __real__Z7EndPermv();
#endif

namespace {

#include "host/harness/programs/autoplay/model.hpp"
#include "host/harness/programs/autoplay/dsl.hpp"
#include "host/harness/programs/autoplay/scripts/01_negotiations.hpp"
#include "host/harness/programs/autoplay/scripts/02_invasion_of_naboo.hpp"
#include "host/harness/programs/autoplay/scripts/03_escape_from_naboo.hpp"
#include "host/harness/programs/autoplay/scripts/04_podrace.hpp"
#include "host/harness/programs/autoplay/scripts/05_retake_theed_palace.hpp"
#include "host/harness/programs/autoplay/scripts/06_darth_maul.hpp"
#include "host/harness/programs/autoplay/scripts/catalog.hpp"
#include "host/harness/programs/autoplay/runner.hpp"
#include "host/harness/programs/autoplay/runtime.hpp"
#include "host/harness/programs/autoplay/world.hpp"
#include "host/harness/programs/autoplay/pathfinding.hpp"
#include "host/harness/programs/autoplay/rail.hpp"
#include "host/harness/programs/autoplay/interactions.hpp"
#include "host/harness/programs/autoplay/executor.hpp"

    // The harness exposes C callbacks, but all mutable state belongs to this
    // one explicitly owned runner.
    AutoplayRunner AutoplayRunner::instance{};

    bool AutoplayRunner::active() const {
        return this->is_active.load(std::memory_order_acquire);
    }

    bool AutoplayRunner::done() const {
        return this->is_done.load(std::memory_order_acquire);
    }

    bool AutoplayRunner::allows_manual_input() const {
        return this->script != nullptr && this->script->options.allow_manual_input;
    }

    i32 AutoplayRunner::result() const {
        return this->result_code.load(std::memory_order_acquire);
    }

    void AutoplayRunner::print_scripts() const {
        printf("Known autoplay levels:\n");
        for (const auto &entry : scripts::by_level) {
            printf("  %-30s %s\n", entry.first.c_str(), entry.second.description.c_str());
        }
        printf("  %-30s Run every known level in story order\n", "all");
    }

    void AutoplayRunner::input_tick() {
        if (!active() || this->is_done.load(std::memory_order_relaxed)) {
            return;
        }

        reset_host_antilight_accumulators();
        ensure_invincibility();

        const Uint64 now = SDL_GetTicks();
        if (this->script != nullptr && this->script->timeout_ms != 0 &&
            now - this->script_started_at > this->script->timeout_ms) {
            finish(1, "script timeout");
            return;
        }
        if (!this->load_requested) {
            if (now - this->configured_at > kStartupTimeoutMs) {
                finish(1, "game did not reach the host level-selection hook before the startup timeout");
            }
            return;
        }

        if (this->action_index >= this->script->actions.size()) {
            complete_script();
            return;
        }

        const AutoplayAction &action = this->script->actions[this->action_index];
        if (!this->current_action.started && !begin_action(action)) {
            finish(1, "could not initialize action");
            return;
        }
        tick_action(action);
    }

    i32 AutoplayRunner::run(const char *level_name) {
        if (level_name != nullptr && SDL_strcasecmp(level_name, "list") == 0) {
            print_scripts();
            return 0;
        }

        this->schedule.clear();
        if (level_name != nullptr && SDL_strcasecmp(level_name, "all") == 0) {
            for (const std::string &story_level : scripts::story_order) {
                const auto script = scripts::by_level.find(story_level);
                if (script != scripts::by_level.end()) {
                    this->schedule.emplace_back(script->first, script->second);
                }
            }
        } else if (level_name != nullptr) {
            const auto found = scripts::by_level.find(level_name);
            if (found != scripts::by_level.end()) {
                this->schedule.emplace_back(found->first, found->second);
            }
        }
        if (this->schedule.empty()) {
            fprintf(stderr, "No autoplay script for level: %s\n", level_name != nullptr ? level_name : "(null)");
            print_scripts();
            return 2;
        }

        constexpr const char *kFixturePath = "res/SavedGames/SaveGame0.LEGO Star Wars - The Complete Saga_SavedGame";
        if (!host_read_game_fixture(kFixturePath, this->fixture)) {
            fprintf(stderr, "autoplay: fixture validation failed: %s\n", kFixturePath);
            return 2;
        }

        if (!SDL_CreateDirectory(".work/autoplay")) {
            fprintf(stderr, "autoplay: could not create .work/autoplay: %s\n", SDL_GetError());
            return 1;
        }
        const std::string documents_path = ".work/autoplay/" + std::to_string(static_cast<i32>(getpid())) + "-" +
                                           std::to_string(static_cast<u64>(SDL_GetTicksNS())) + "/";
        if (!SDL_CreateDirectory(documents_path.c_str())) {
            fprintf(stderr, "autoplay: could not create %s: %s\n", documents_path.c_str(), SDL_GetError());
            return 1;
        }

        this->schedule_index = 0;
        this->owns_invincibility_cheat = false;
        this->result_code.store(1, std::memory_order_relaxed);
        this->is_done.store(false, std::memory_order_relaxed);
        this->configured_at = SDL_GetTicks();
        if (!select_script()) {
            return 2;
        }
        this->is_active.store(true, std::memory_order_release);

        HostWindowOptions options;
        options.documents_path = documents_path.c_str();
        options.timeout_ms = 0;
        this->previous_loader_mode = LOADEROFF;
        this->loader_mode_overridden = true;
        LOADEROFF = 1;
        const i32 window_result = host_run_window(options);

        release_input();
        if (this->loader_mode_overridden) {
            LOADEROFF = this->previous_loader_mode;
            this->loader_mode_overridden = false;
        }
        if (this->owns_invincibility_cheat && this->invincibility_cheat_index >= 0) {
            Cheat_SetOn(this->invincibility_cheat_index, 0, 0);
        }
        this->is_active.store(false, std::memory_order_release);
        if (!done()) {
            LOG_ERR("autoplay %s: window exited before the script completed", this->level_name.c_str());
            return window_result != 0 ? window_result : 1;
        }
        return result() != 0 ? result() : window_result;
    }

} // namespace

#if defined(__linux__) && !defined(__EMSCRIPTEN__)
extern "C" __attribute__((weak)) void __wrap__Z7EndPermv() {
    __real__Z7EndPermv();
    AutoplayRunner::instance.prepare_level();
}
#endif

void host_autoplay_print_scripts() {
    AutoplayRunner::instance.print_scripts();
}

bool host_autoplay_active() {
    return AutoplayRunner::instance.active();
}

bool host_autoplay_done() {
    return AutoplayRunner::instance.done();
}

bool host_autoplay_allows_manual_input() {
    return AutoplayRunner::instance.allows_manual_input();
}

i32 host_autoplay_result() {
    return AutoplayRunner::instance.result();
}

void host_autoplay_input_tick() {
    AutoplayRunner::instance.input_tick();
}

i32 host_run_autoplay(const char *level_name) {
    return AutoplayRunner::instance.run(level_name);
}
