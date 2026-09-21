#pragma once

// Stateful execution engine. There is exactly one host-harness instance.
// Included only by autoplay.cpp, inside its anonymous namespace.

class AutoplayRunner final {
  public:
    static AutoplayRunner &get() {
        static AutoplayRunner instance;
        return instance;
    }

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
        std::string level_name;
        std::reference_wrapper<const AutoplayScript> script;
    };

    struct ActiveAction {
        explicit ActiveAction(ActionRuntime initial_state = std::monostate{})
            : started_at(SDL_GetTicks()), runtime(std::move(initial_state)) {
        }

        u64 started_at;
        ActionRuntime runtime;
    };

    struct RunSpeedOverride {
        GAMECHARACTERDATA_s *character;
        f32 original_speed;
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

    template <typename Runtime> Runtime &runtime() {
        return std::get<Runtime>(this->current_action->runtime);
    }

    void advance_action();
    bool begin_forward_rail(const RailAction &action, const RailForward &destination);
    bool begin_relative_rail(const RailAction &action, const RailRelativeRoute &destination);
    bool begin_target_rail(const RailAction &action);
    bool begin_crossing_rail(const RailAction &action, const RailThroughGizmo &destination);
    bool begin_path_connection_rail(const RailAction &action, const RailThroughPathConnection &destination);
    bool begin_native_jump(const NativeJumpAction &action);
    bool begin_action(const AutoplayAction &action);
    void set_rail_input(const GameObject_s &character, i32 world_angle);

    void tick_rail_action(const RailAction &action);
    void tick_rail_state(f32 speed, std::optional<std::reference_wrapper<const RailDestination>> destination);
    void tick_native_jump_action();
    void tick_start_podrace_action();
    void restore_podrace_track(GameObject_s &pod, const SOCKPOSITION &track_position, const char *reason);
    void tick_podrace_drive_action(const DrivePodraceAction &action);
    void tick_force_action(const UseForceAction &action);
    void tick_panel_action(const UsePanelAction &action);
    void tick_build_action(const UseBuildItAction &action);
    void tick_zipup_action(const UseZipUpAction &action);
    void tick_blaster_hit_action(const BlasterHitAction &action);
    void tick_clear_hostiles_action(const ClearHostilesAction &action);
    void tick_destroy_ai_object_action(const DestroyAiObjectAction &action);
    void tick_damage_character_action(const DamageCharacterAction &action);
    void tick_character_switch(const SwitchCharacterAction &action);
    void tick_hold_gizmo_action(const HoldGizmoAction &action);
    void tick_hold_party_switches_action(const HoldPartySwitchesAction &action);
    void tick_hold_party_forces_action(const HoldPartyForcesAction &action);
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
    u64 configured_at = 0;
    u64 script_started_at = 0;
    std::optional<ActiveAction> current_action;
    std::unordered_map<std::string, NUVEC> gizmo_position_snapshots;
    std::optional<i32> owned_invincibility_cheat;
    std::optional<i32> overridden_loader_mode;
    std::optional<RunSpeedOverride> run_speed_override;
    std::vector<u8> podrace_state;
};
