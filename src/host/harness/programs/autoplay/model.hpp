#pragma once

// Data model shared by the script DSL and execution engine.

enum class AutoplayActionKind {
    checkpoint,
    wait_loaded,
    wait_time,
    start_podrace,
    drive_podrace_to_level,
    rail_forward,
    rail_relative_route,
    rail_to_gizmo,
    rail_through_gizmo,
    rail_through_path_connection,
    rail_to_character,
    rail_to_area,
    rail_to_path_node,
    rail_to_locator,
    rail_through_locator,
    native_jump_relative,
    native_jump_to_gizmo,
    native_jump_to_locator,
    teleport_to,
    use_force_gizmo,
    use_panel_gizmo,
    use_buildit_gizmo,
    use_zipup_gizmo,
    blaster_hit_gizmo,
    clear_nearby_hostiles,
    clear_named_characters,
    destroy_named_ai_object,
    damage_character_to_health,
    switch_to_character,
    wait_gizmo_output,
    hold_gizmo_until_output,
    hold_party_switches_until_output,
    hold_party_forces_until_output,
    wait_ai_message,
    wait_level,
    wait_menu,
    wait_cutscene_end,
    log_gizmos,
    manual_control,
};

struct PartySwitchPlacement {
    const char *character;
    const char *gizmo;
};

struct PartyForceUse {
    const char *character;
    const char *gizmo;
};

struct AutoplayAction {
    explicit AutoplayAction(AutoplayActionKind action_kind, u32 timeout = 0) : kind(action_kind), timeout_ms(timeout) {
    }

    AutoplayActionKind kind;
    std::string target_name;
    std::string condition_name;
    NUVEC position{};
    f32 arrival_tolerance = 0.0f;
    f32 distance = 0.0f;
    f32 speed = 0.0f;
    f32 overshoot = 0.0f;
    u32 duration_ms = 0;
    u32 timeout_ms = 0;
    i32 output_index = 0;
    i32 expected_output = 0;
    std::vector<NUVEC> waypoints;
    std::vector<PartySwitchPlacement> party_switches;
    std::vector<PartyForceUse> party_forces;
    bool follow_height = false;
};

struct AutoplayOptions {
    explicit AutoplayOptions(bool manual_input = false, bool invincible = true)
        : allow_manual_input(manual_input), enable_invincibility(invincible) {
    }

    bool allow_manual_input;
    bool enable_invincibility;
};

struct AutoplayScript {
    AutoplayScript(std::string script_description, std::vector<AutoplayAction> script_actions,
                   AutoplayOptions script_options = AutoplayOptions{}, u32 script_timeout_ms = 240000)
        : description(std::move(script_description)), actions(std::move(script_actions)), options(script_options),
          timeout_ms(script_timeout_ms) {
    }

    std::string description;
    std::vector<AutoplayAction> actions;
    AutoplayOptions options;
    u32 timeout_ms;
};

struct RailWaypoint {
    NUVEC position{};
    AIPATHCNX *incoming_connection = nullptr;
    i32 traversal_direction = 0;
};

struct RailState {
    std::vector<RailWaypoint> waypoints;
    usize waypoint_index = 0;
    NUVEC progress_origin{};
    NUVEC exit_direction{};
    GameObject_s *character = nullptr;
    LEVELDATA_s *starting_level = nullptr;
    Uint64 progress_observed_at = 0;
    bool progress_observation_started = false;
    bool stop_on_transition = false;
    bool follow_height = false;
    bool vertical_travel = false;
    bool native_traversal_started = false;
    bool native_traversal_airborne = false;
    f32 native_traversal_minimum_height = 0.0f;
    f32 overshoot = 0.0f;
    i32 final_facing = 0;
    bool has_final_facing = false;
};

constexpr u32 kStartupTimeoutMs = 90000;
constexpr u32 kInvulnerabilityCheatFlag = 0x80;
