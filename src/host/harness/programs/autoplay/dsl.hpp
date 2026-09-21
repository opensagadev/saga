#pragma once

// Small declarative vocabulary used by files under scripts/.

namespace dsl {

    inline AutoplayAction checkpoint(const char *level) {
        AutoplayAction action{AutoplayActionKind::checkpoint};
        action.target_name = level;
        return action;
    }

    inline AutoplayAction wait_loaded(u32 timeout_ms = 60000) {
        return AutoplayAction{AutoplayActionKind::wait_loaded, timeout_ms};
    }

    inline AutoplayAction wait_time(u32 duration_ms) {
        AutoplayAction action{AutoplayActionKind::wait_time, duration_ms + 10000};
        action.duration_ms = duration_ms;
        return action;
    }

    inline AutoplayAction start_podrace() {
        return AutoplayAction{AutoplayActionKind::start_podrace, 1000};
    }

    inline AutoplayAction drive_podrace_to_level(const char *level, f32 speed_multiplier = 2.5f,
                                                 u32 timeout_ms = 120000) {
        AutoplayAction action{AutoplayActionKind::drive_podrace_to_level, timeout_ms};
        action.target_name = level;
        action.speed = speed_multiplier;
        return action;
    }

    inline AutoplayAction rail_forward(f32 distance, f32 speed = 6.0f, u32 timeout_ms = 15000) {
        AutoplayAction action{AutoplayActionKind::rail_forward, timeout_ms};
        action.distance = distance;
        action.speed = speed;
        return action;
    }

    inline AutoplayAction rail_relative_route(std::vector<NUVEC> points, f32 speed, u32 timeout_ms,
                                              bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_relative_route, timeout_ms};
        action.speed = speed;
        action.waypoints = std::move(points);
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_to_gizmo(const char *gizmo, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                        bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_to_gizmo, timeout_ms};
        action.target_name = gizmo;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_through_gizmo(const char *gizmo, f32 beyond_distance, f32 speed = 4.0f,
                                             u32 timeout_ms = 20000, bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_through_gizmo, timeout_ms};
        action.target_name = gizmo;
        action.overshoot = beyond_distance;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_through_path_connection(const char *from, const char *to, f32 speed = 4.0f,
                                                       u32 timeout_ms = 20000, bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_through_path_connection, timeout_ms};
        action.target_name = from;
        action.condition_name = to;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_to_character(const char *character, f32 tolerance = 0.75f, f32 speed = 6.0f,
                                            u32 timeout_ms = 30000, bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_to_character, timeout_ms};
        action.target_name = character;
        action.arrival_tolerance = tolerance;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_to_area(const char *area, f32 speed = 5.0f, u32 timeout_ms = 15000,
                                       bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_to_area, timeout_ms};
        action.target_name = area;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_to_path_node(const char *node, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                            bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_to_path_node, timeout_ms};
        action.target_name = node;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_to_locator(const char *locator, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                          bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_to_locator, timeout_ms};
        action.target_name = locator;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction rail_through_locator(const char *locator, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                               bool follow_height = false) {
        AutoplayAction action{AutoplayActionKind::rail_through_locator, timeout_ms};
        action.target_name = locator;
        action.speed = speed;
        action.follow_height = follow_height;
        return action;
    }

    inline AutoplayAction native_jump_relative(NUVEC offset, u32 timeout_ms = 10000) {
        AutoplayAction action{AutoplayActionKind::native_jump_relative, timeout_ms};
        action.position = offset;
        return action;
    }

    inline AutoplayAction native_jump_to_gizmo(const char *gizmo, u32 timeout_ms = 10000) {
        AutoplayAction action{AutoplayActionKind::native_jump_to_gizmo, timeout_ms};
        action.target_name = gizmo;
        return action;
    }

    inline AutoplayAction native_jump_to_locator(const char *locator, u32 timeout_ms = 10000) {
        AutoplayAction action{AutoplayActionKind::native_jump_to_locator, timeout_ms};
        action.target_name = locator;
        return action;
    }

    inline AutoplayAction teleport_to(f32 x, f32 y, f32 z) {
        AutoplayAction action{AutoplayActionKind::teleport_to, 1000};
        action.position = {x, y, z};
        return action;
    }

    inline AutoplayAction use_force_gizmo(const char *name, u32 timeout_ms = 15000) {
        AutoplayAction action{AutoplayActionKind::use_force_gizmo, timeout_ms};
        action.target_name = name;
        return action;
    }

    inline AutoplayAction use_panel_gizmo(const char *name, u32 timeout_ms = 10000) {
        AutoplayAction action{AutoplayActionKind::use_panel_gizmo, timeout_ms};
        action.target_name = name;
        return action;
    }

    inline AutoplayAction use_buildit_gizmo(const char *name, u32 timeout_ms = 15000) {
        AutoplayAction action{AutoplayActionKind::use_buildit_gizmo, timeout_ms};
        action.target_name = name;
        return action;
    }

    inline AutoplayAction use_zipup_gizmo(const char *name, u32 timeout_ms = 10000) {
        AutoplayAction action{AutoplayActionKind::use_zipup_gizmo, timeout_ms};
        action.target_name = name;
        return action;
    }

    inline AutoplayAction blaster_hit_gizmo(const char *name, u32 timeout_ms = 5000) {
        AutoplayAction action{AutoplayActionKind::blaster_hit_gizmo, timeout_ms};
        action.target_name = name;
        return action;
    }

    inline AutoplayAction clear_nearby_hostiles(f32 radius = 16.0f, u32 quiet_period_ms = 750, u32 timeout_ms = 30000) {
        AutoplayAction action{AutoplayActionKind::clear_nearby_hostiles, timeout_ms};
        action.distance = radius;
        action.duration_ms = quiet_period_ms;
        return action;
    }

    inline AutoplayAction clear_named_characters(const char *character, f32 radius, u32 quiet_period_ms = 1000,
                                                 u32 timeout_ms = 30000) {
        AutoplayAction action{AutoplayActionKind::clear_named_characters, timeout_ms};
        action.target_name = character;
        action.distance = radius;
        action.duration_ms = quiet_period_ms;
        return action;
    }

    inline AutoplayAction destroy_named_ai_object(const char *name, f32 maximum_range = 15.0f, u32 timeout_ms = 15000) {
        AutoplayAction action{AutoplayActionKind::destroy_named_ai_object, timeout_ms};
        action.target_name = name;
        action.distance = maximum_range;
        return action;
    }

    inline AutoplayAction damage_character_to_health(const char *character, i32 health, f32 range = 3.5f,
                                                     u32 timeout_ms = 30000) {
        AutoplayAction action{AutoplayActionKind::damage_character_to_health, timeout_ms};
        action.target_name = character;
        action.expected_output = health;
        action.distance = range;
        return action;
    }

    inline AutoplayAction switch_to_character(const char *name, u32 timeout_ms = 5000) {
        AutoplayAction action{AutoplayActionKind::switch_to_character, timeout_ms};
        action.target_name = name;
        return action;
    }

    inline AutoplayAction wait_gizmo_output(const char *name, i32 output_index, i32 expected, u32 timeout_ms) {
        AutoplayAction action{AutoplayActionKind::wait_gizmo_output, timeout_ms};
        action.target_name = name;
        action.output_index = output_index;
        action.expected_output = expected;
        return action;
    }

    inline AutoplayAction wait_ai_message(const char *name, i32 expected, u32 timeout_ms = 15000) {
        AutoplayAction action{AutoplayActionKind::wait_ai_message, timeout_ms};
        action.target_name = name;
        action.expected_output = expected;
        return action;
    }

    inline AutoplayAction hold_gizmo_until_output(const char *standing_gizmo, const char *watched_gizmo,
                                                  i32 output_index, i32 expected, f32 return_speed = 5.0f,
                                                  u32 timeout_ms = 20000) {
        AutoplayAction action{AutoplayActionKind::hold_gizmo_until_output, timeout_ms};
        action.target_name = standing_gizmo;
        action.condition_name = watched_gizmo;
        action.output_index = output_index;
        action.expected_output = expected;
        action.speed = return_speed;
        return action;
    }

    inline AutoplayAction hold_party_switches_until_output(std::initializer_list<PartySwitchPlacement> placements,
                                                           const char *watched_gizmo, i32 output_index, i32 expected,
                                                           u32 timeout_ms = 20000) {
        AutoplayAction action{AutoplayActionKind::hold_party_switches_until_output, timeout_ms};
        action.condition_name = watched_gizmo;
        action.output_index = output_index;
        action.expected_output = expected;
        action.party_switches.assign(placements);
        return action;
    }

    inline AutoplayAction hold_party_forces_until_output(std::initializer_list<PartyForceUse> uses,
                                                         const char *watched_gizmo, i32 output_index, i32 expected,
                                                         u32 timeout_ms = 20000) {
        AutoplayAction action{AutoplayActionKind::hold_party_forces_until_output, timeout_ms};
        action.condition_name = watched_gizmo;
        action.output_index = output_index;
        action.expected_output = expected;
        action.party_forces.assign(uses);
        return action;
    }

    inline AutoplayAction wait_level(const char *name, u32 timeout_ms = 60000) {
        AutoplayAction action{AutoplayActionKind::wait_level, timeout_ms};
        action.target_name = name;
        return action;
    }

    inline AutoplayAction wait_menu(i32 menu_id, u32 timeout_ms = 15000) {
        AutoplayAction action{AutoplayActionKind::wait_menu, timeout_ms};
        action.output_index = menu_id;
        return action;
    }

    inline AutoplayAction wait_cutscene_end(const char *level_name, u32 timeout_ms = 15000) {
        AutoplayAction action{AutoplayActionKind::wait_cutscene_end, timeout_ms};
        action.target_name = level_name;
        return action;
    }

    inline AutoplayAction log_gizmos() {
        return AutoplayAction{AutoplayActionKind::log_gizmos, 1000};
    }

    inline AutoplayAction manual_control() {
        return AutoplayAction{AutoplayActionKind::manual_control};
    }

} // namespace dsl
