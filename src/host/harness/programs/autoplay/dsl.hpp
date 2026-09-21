#pragma once

// Small declarative vocabulary used by files under scripts/.

namespace dsl {

    template <typename Payload> AutoplayAction action(Payload payload, u32 timeout_ms = 0) {
        return {std::move(payload), Milliseconds{timeout_ms}};
    }

    inline AutoplayAction checkpoint(std::string level) {
        return action(CheckpointAction{std::move(level)});
    }
    inline AutoplayAction wait_loaded(u32 timeout_ms = 60000) {
        return action(WaitLoadedAction{}, timeout_ms);
    }
    inline AutoplayAction wait_time(u32 duration_ms) {
        return action(WaitAction{Milliseconds{duration_ms}}, duration_ms + 10000);
    }
    inline AutoplayAction start_podrace() {
        return action(StartPodraceAction{}, 1000);
    }
    inline AutoplayAction drive_podrace_to_level(std::string level, f32 speed_multiplier = 2.5f,
                                                 u32 timeout_ms = 120000) {
        return action(DrivePodraceAction{std::move(level), speed_multiplier}, timeout_ms);
    }
    inline AutoplayAction rail_forward(f32 distance, f32 speed = 6.0f, u32 timeout_ms = 15000) {
        return action(RailAction{RailForward{distance}, speed, false}, timeout_ms);
    }
    inline AutoplayAction rail_relative_route(std::vector<NUVEC> points, f32 speed, u32 timeout_ms,
                                              bool follow_height = false) {
        return action(RailAction{RailRelativeRoute{std::move(points)}, speed, follow_height}, timeout_ms);
    }
    inline AutoplayAction rail_to_gizmo(std::string gizmo, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                        bool follow_height = false) {
        return action(RailAction{RailToGizmo{std::move(gizmo)}, speed, follow_height}, timeout_ms);
    }
    inline AutoplayAction rail_through_gizmo(std::string gizmo, f32 beyond_distance, f32 speed = 4.0f,
                                             u32 timeout_ms = 20000, bool follow_height = false) {
        return action(RailAction{RailThroughGizmo{std::move(gizmo), beyond_distance}, speed, follow_height},
                      timeout_ms);
    }
    inline AutoplayAction rail_through_path_connection(std::string from, std::string to, f32 speed = 4.0f,
                                                       u32 timeout_ms = 20000, bool follow_height = false) {
        return action(RailAction{RailThroughPathConnection{std::move(from), std::move(to)}, speed, follow_height},
                      timeout_ms);
    }
    inline AutoplayAction rail_to_character(std::string character, f32 tolerance = 0.75f, f32 speed = 6.0f,
                                            u32 timeout_ms = 30000, bool follow_height = false) {
        return action(RailAction{RailToCharacter{std::move(character), tolerance}, speed, follow_height}, timeout_ms);
    }
    inline AutoplayAction rail_to_area(std::string area, f32 speed = 5.0f, u32 timeout_ms = 15000,
                                       bool follow_height = false) {
        return action(RailAction{RailToArea{std::move(area)}, speed, follow_height}, timeout_ms);
    }
    inline AutoplayAction rail_to_path_node(std::string node, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                            bool follow_height = false) {
        return action(RailAction{RailToPathNode{std::move(node)}, speed, follow_height}, timeout_ms);
    }
    inline AutoplayAction rail_to_locator(std::string locator, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                          bool follow_height = false) {
        return action(RailAction{RailToLocator{std::move(locator)}, speed, follow_height}, timeout_ms);
    }
    inline AutoplayAction rail_through_locator(std::string locator, f32 speed = 6.0f, u32 timeout_ms = 20000,
                                               bool follow_height = false) {
        return action(RailAction{RailThroughLocator{std::move(locator)}, speed, follow_height}, timeout_ms);
    }
    inline AutoplayAction native_jump_relative(NUVEC offset, u32 timeout_ms = 10000) {
        return action(NativeJumpAction{JumpRelative{offset}}, timeout_ms);
    }
    inline AutoplayAction native_jump_to_gizmo(std::string gizmo, u32 timeout_ms = 10000) {
        return action(NativeJumpAction{JumpToGizmo{std::move(gizmo)}}, timeout_ms);
    }
    inline AutoplayAction native_jump_to_locator(std::string locator, u32 timeout_ms = 10000) {
        return action(NativeJumpAction{JumpToLocator{std::move(locator)}}, timeout_ms);
    }
    inline AutoplayAction teleport_to(f32 x, f32 y, f32 z) {
        return action(TeleportAction{{x, y, z}}, 1000);
    }
    inline AutoplayAction use_force_gizmo(std::string name, u32 timeout_ms = 15000) {
        return action(UseForceAction{std::move(name)}, timeout_ms);
    }
    inline AutoplayAction use_panel_gizmo(std::string name, u32 timeout_ms = 10000) {
        return action(UsePanelAction{std::move(name)}, timeout_ms);
    }
    inline AutoplayAction use_buildit_gizmo(std::string name, u32 timeout_ms = 15000) {
        return action(UseBuildItAction{std::move(name)}, timeout_ms);
    }
    inline AutoplayAction use_zipup_gizmo(std::string name, u32 timeout_ms = 10000) {
        return action(UseZipUpAction{std::move(name)}, timeout_ms);
    }
    inline AutoplayAction blaster_hit_gizmo(std::string name, u32 timeout_ms = 5000) {
        return action(BlasterHitAction{std::move(name)}, timeout_ms);
    }
    inline AutoplayAction clear_nearby_hostiles(f32 radius = 16.0f, u32 quiet_period_ms = 750, u32 timeout_ms = 30000) {
        return action(ClearHostilesAction{std::nullopt, radius, Milliseconds{quiet_period_ms}}, timeout_ms);
    }
    inline AutoplayAction clear_named_characters(std::string character, f32 radius, u32 quiet_period_ms = 1000,
                                                 u32 timeout_ms = 30000) {
        return action(ClearHostilesAction{std::move(character), radius, Milliseconds{quiet_period_ms}}, timeout_ms);
    }
    inline AutoplayAction destroy_named_ai_object(std::string name, f32 maximum_range = 15.0f, u32 timeout_ms = 15000) {
        return action(DestroyAiObjectAction{std::move(name), maximum_range}, timeout_ms);
    }
    inline AutoplayAction damage_character_to_health(std::string character, i32 health, f32 range = 3.5f,
                                                     u32 timeout_ms = 30000) {
        return action(DamageCharacterAction{std::move(character), health, range}, timeout_ms);
    }
    inline AutoplayAction switch_to_character(std::string name, u32 timeout_ms = 5000) {
        return action(SwitchCharacterAction{std::move(name)}, timeout_ms);
    }
    inline AutoplayAction wait_gizmo_output(std::string name, i32 output_index, i32 expected, u32 timeout_ms) {
        return action(WaitGizmoOutputAction{{std::move(name), output_index, expected}}, timeout_ms);
    }
    inline AutoplayAction wait_ai_message(std::string name, i32 expected, u32 timeout_ms = 15000) {
        return action(WaitAiMessageAction{std::move(name), expected}, timeout_ms);
    }
    inline AutoplayAction hold_gizmo_until_output(std::string standing_gizmo, std::string watched_gizmo,
                                                  i32 output_index, i32 expected, f32 return_speed = 5.0f,
                                                  u32 timeout_ms = 20000) {
        return action(HoldGizmoAction{std::move(standing_gizmo),
                                      {std::move(watched_gizmo), output_index, expected},
                                      return_speed},
                      timeout_ms);
    }
    inline AutoplayAction hold_party_switches_until_output(std::initializer_list<PartySwitchPlacement> placements,
                                                           std::string watched_gizmo, i32 output_index, i32 expected,
                                                           u32 timeout_ms = 20000) {
        return action(HoldPartySwitchesAction{{placements}, {std::move(watched_gizmo), output_index, expected}},
                      timeout_ms);
    }
    inline AutoplayAction hold_party_forces_until_output(std::initializer_list<PartyForceUse> uses,
                                                         std::string watched_gizmo, i32 output_index, i32 expected,
                                                         u32 timeout_ms = 20000) {
        return action(HoldPartyForcesAction{{uses}, {std::move(watched_gizmo), output_index, expected}}, timeout_ms);
    }
    inline AutoplayAction wait_level(std::string name, u32 timeout_ms = 60000) {
        return action(WaitLevelAction{std::move(name)}, timeout_ms);
    }
    inline AutoplayAction wait_menu(i32 menu_id, u32 timeout_ms = 15000) {
        return action(WaitMenuAction{menu_id}, timeout_ms);
    }
    inline AutoplayAction wait_cutscene_end(std::string level_name, u32 timeout_ms = 15000) {
        return action(WaitCutsceneEndAction{std::move(level_name)}, timeout_ms);
    }
    inline AutoplayAction log_gizmos() {
        return action(LogGizmosAction{}, 1000);
    }
    inline AutoplayAction manual_control() {
        return action(ManualControlAction{});
    }

} // namespace dsl
