#pragma once

// Stateful execution engine. There is exactly one host-harness instance.
// Included only by autoplay.cpp, inside its anonymous namespace.

class AutoplayRunner final {
  public:
    // C++11 requires the definition in autoplay.cpp. Keeping the sole
    // instance on the type makes ownership explicit at every callback site.
    static AutoplayRunner instance;

    bool active() const;
    bool done() const;
    bool allows_manual_input() const;
    i32 result() const;
    void print_scripts() const;
    void prepare_level();
    void input_tick();
    i32 run(const char *level_name);

  private:
    struct ScheduledScript {
        ScheduledScript(std::string scheduled_level, const AutoplayScript &scheduled_script)
            : level_name(std::move(scheduled_level)), script(&scheduled_script) {
        }

        std::string level_name;
        const AutoplayScript *script;
    };

    struct ActionState {
        bool started = false;
        bool character_switch_requested = false;
        bool force_started = false;
        bool build_started = false;
        bool interaction_started = false;
        bool has_podrace_track_position = false;
        bool podrace_lap_advanced = false;
        u32 podrace_recovery_count = 0;
        Uint64 started_at = 0;
        Uint64 last_combat_hit_at = 0;
        Uint64 last_hostile_seen_at = 0;
        Uint64 podrace_progress_observed_at = 0;
        Uint64 podrace_track_updated_at = 0;
        LEVELDATA_s *podrace_starting_level = nullptr;
        NUVEC podrace_progress_origin{};
        SOCKPOSITION podrace_track_position{};
        RailState rail{};
        std::vector<NUVEC> party_switch_positions;

        void clear() {
            *this = ActionState{};
        }
    };

    AutoplayRunner() = default;
    AutoplayRunner(const AutoplayRunner &) = delete;
    AutoplayRunner &operator=(const AutoplayRunner &) = delete;

    void release_input();
    void cancel_incidental_push(GameObject_s &character);
    void enable_rail_run_speed(f32 multiplier);
    void restore_run_speed();
    void ensure_invincibility();
    void reset_host_antilight_accumulators();
    void configure_podrace_host_state();
    void attach_podrace_host_state(WORLDINFO_s &world);
    static void initialize_podrace_a(WORLDINFO_s *world);
    static void initialize_podrace_b(WORLDINFO_s *world);
    static void initialize_podrace_c(WORLDINFO_s *world);
    void finish(i32 status, const char *reason);
    bool select_script();
    bool enter_level();
    void complete_script();
    bool gameplay_ready(const char *level_name) const;

    void advance_action();
    bool begin_forward_rail(const AutoplayAction &action);
    bool begin_relative_rail(const AutoplayAction &action);
    bool begin_target_rail(const AutoplayAction &action);
    bool begin_crossing_rail(const AutoplayAction &action);
    bool begin_path_connection_rail(const AutoplayAction &action);
    bool begin_native_jump(const AutoplayAction &action);
    bool begin_action(const AutoplayAction &action);
    void set_rail_input(const GameObject_s &character, i32 world_angle);

    void tick_rail_action(const AutoplayAction &action);
    void tick_start_podrace_action();
    void restore_podrace_track(GameObject_s &pod, const SOCKPOSITION &track_position, const char *reason);
    void tick_podrace_drive_action(const AutoplayAction &action);
    void tick_force_action(const AutoplayAction &action);
    void tick_panel_action(const AutoplayAction &action);
    void tick_build_action(const AutoplayAction &action);
    void tick_zipup_action(const AutoplayAction &action);
    void tick_blaster_hit_action(const AutoplayAction &action);
    void tick_clear_hostiles_action(const AutoplayAction &action);
    void tick_destroy_ai_object_action(const AutoplayAction &action);
    void tick_damage_character_action(const AutoplayAction &action);
    void tick_character_switch(const AutoplayAction &action);
    void tick_hold_gizmo_action(const AutoplayAction &action);
    void tick_hold_party_switches_action(const AutoplayAction &action);
    void tick_hold_party_forces_action(const AutoplayAction &action);
    void tick_action(const AutoplayAction &action);

    std::atomic<bool> is_active{false};
    std::atomic<bool> is_done{false};
    std::atomic<i32> result_code{1};
    std::vector<ScheduledScript> schedule;
    usize schedule_index = 0;
    std::string level_name;
    const AutoplayScript *script = nullptr;
    GAMESAVE_s fixture{};
    usize action_index = 0;
    bool load_requested = false;
    Uint64 configured_at = 0;
    Uint64 script_started_at = 0;
    ActionState current_action{};
    std::unordered_map<std::string, NUVEC> gizmo_position_snapshots;
    i32 invincibility_cheat_index = -1;
    bool owns_invincibility_cheat = false;
    i32 previous_loader_mode = 0;
    bool loader_mode_overridden = false;
    GAMECHARACTERDATA_s *boosted_character_data = nullptr;
    f32 original_run_speed = 0.0f;
    std::vector<u8> podrace_state;
};
