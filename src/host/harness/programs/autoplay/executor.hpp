#pragma once

// Action lifecycle and dispatch. Domain-specific work lives in rail.hpp and
// interactions.hpp.

void AutoplayRunner::advance_action() {
    release_input();
    LOG_INFO("autoplay %s: completed action %u/%u (%s)", this->level_name.c_str(),
             static_cast<unsigned>(this->action_index + 1), static_cast<unsigned>(this->script->actions.size()),
             action_name(this->script->actions[this->action_index].kind));
    ++this->action_index;
    this->current_action.clear();
    if (this->action_index == this->script->actions.size()) {
        complete_script();
    }
}

bool AutoplayRunner::begin_action(const AutoplayAction &action) {
    this->current_action.started_at = SDL_GetTicks();
    this->current_action.started = true;
    LOG_INFO("autoplay %s: starting action %u/%u (%s%s%s)", this->level_name.c_str(),
             static_cast<unsigned>(this->action_index + 1), static_cast<unsigned>(this->script->actions.size()),
             action_name(action.kind), action.target_name.empty() ? "" : " ", action.target_name.c_str());

    switch (action.kind) {
        case AutoplayActionKind::rail_forward:
            return begin_forward_rail(action);
        case AutoplayActionKind::rail_relative_route:
            return begin_relative_rail(action);
        case AutoplayActionKind::rail_to_gizmo:
        case AutoplayActionKind::rail_to_character:
        case AutoplayActionKind::rail_to_area:
        case AutoplayActionKind::rail_to_path_node:
        case AutoplayActionKind::rail_to_locator:
        case AutoplayActionKind::rail_through_locator:
            return begin_target_rail(action);
        case AutoplayActionKind::rail_through_gizmo:
            return begin_crossing_rail(action);
        case AutoplayActionKind::rail_through_path_connection:
            return begin_path_connection_rail(action);
        case AutoplayActionKind::native_jump_relative:
        case AutoplayActionKind::native_jump_to_gizmo:
        case AutoplayActionKind::native_jump_to_locator:
            return begin_native_jump(action);
        case AutoplayActionKind::hold_gizmo_until_output: {
            NUVEC target{};
            if (!gizmo_use_position(find_gizmo(action.target_name.c_str()), target)) {
                return false;
            }
            this->current_action.rail.waypoints = {RailWaypoint{target}};
            return true;
        }
        case AutoplayActionKind::hold_party_switches_until_output:
            this->current_action.party_switch_positions.reserve(action.party_switches.size());
            for (const PartySwitchPlacement &placement : action.party_switches) {
                NUVEC position{};
                if (!gizmo_use_position(find_gizmo(placement.gizmo), position)) {
                    return false;
                }
                this->current_action.party_switch_positions.push_back(position);
            }
            return true;
        case AutoplayActionKind::hold_party_forces_until_output:
            return true;
        default:
            return true;
    }
}

void AutoplayRunner::tick_action(const AutoplayAction &action) {
    const Uint64 elapsed = SDL_GetTicks() - this->current_action.started_at;
    if (action.timeout_ms != 0 && elapsed > action.timeout_ms) {
        finish(1, "action timeout");
        return;
    }

    switch (action.kind) {
        case AutoplayActionKind::checkpoint:
            advance_action();
            return;
        case AutoplayActionKind::wait_loaded:
            if (gameplay_ready(nullptr)) {
                advance_action();
            }
            return;
        case AutoplayActionKind::wait_time:
            if (elapsed >= action.duration_ms) {
                advance_action();
            }
            return;
        case AutoplayActionKind::start_podrace:
            tick_start_podrace_action();
            return;
        case AutoplayActionKind::drive_podrace_to_level:
            tick_podrace_drive_action(action);
            return;
        case AutoplayActionKind::rail_forward:
        case AutoplayActionKind::rail_relative_route:
        case AutoplayActionKind::rail_to_gizmo:
        case AutoplayActionKind::rail_through_gizmo:
        case AutoplayActionKind::rail_through_path_connection:
        case AutoplayActionKind::rail_to_character:
        case AutoplayActionKind::rail_to_area:
        case AutoplayActionKind::rail_to_path_node:
        case AutoplayActionKind::rail_to_locator:
        case AutoplayActionKind::rail_through_locator:
        case AutoplayActionKind::native_jump_relative:
        case AutoplayActionKind::native_jump_to_gizmo:
        case AutoplayActionKind::native_jump_to_locator:
            tick_rail_action(action);
            return;
        case AutoplayActionKind::teleport_to: {
            if (!gameplay_ready(nullptr) || Player[0] == nullptr) {
                return;
            }
            NUVEC position = action.position;
            const i32 angle = Player[0]->apiobj.facing_angle;
            SnapCreaturePos(Player[0], &position, angle, nullptr, 1);
            LOG_INFO("autoplay %s: teleported player to (%.3f,%.3f,%.3f)", this->level_name.c_str(),
                     Player[0]->apiobj.position.x, Player[0]->apiobj.position.y, Player[0]->apiobj.position.z);
            advance_action();
            return;
        }
        case AutoplayActionKind::use_force_gizmo:
            tick_force_action(action);
            return;
        case AutoplayActionKind::use_panel_gizmo:
            tick_panel_action(action);
            return;
        case AutoplayActionKind::use_buildit_gizmo:
            tick_build_action(action);
            return;
        case AutoplayActionKind::use_zipup_gizmo:
            tick_zipup_action(action);
            return;
        case AutoplayActionKind::blaster_hit_gizmo:
            tick_blaster_hit_action(action);
            return;
        case AutoplayActionKind::clear_nearby_hostiles:
        case AutoplayActionKind::clear_named_characters:
            tick_clear_hostiles_action(action);
            return;
        case AutoplayActionKind::destroy_named_ai_object:
            tick_destroy_ai_object_action(action);
            return;
        case AutoplayActionKind::damage_character_to_health:
            tick_damage_character_action(action);
            return;
        case AutoplayActionKind::switch_to_character:
            tick_character_switch(action);
            return;
        case AutoplayActionKind::wait_gizmo_output: {
            GIZMO *gizmo = find_gizmo(action.target_name.c_str());
            if (gizmo != nullptr &&
                GizmoGetOutput(WORLD->gizmo_sys, gizmo, action.output_index, 1) == action.expected_output) {
                advance_action();
            }
            return;
        }
        case AutoplayActionKind::hold_gizmo_until_output:
            tick_hold_gizmo_action(action);
            return;
        case AutoplayActionKind::hold_party_switches_until_output:
            tick_hold_party_switches_action(action);
            return;
        case AutoplayActionKind::hold_party_forces_until_output:
            tick_hold_party_forces_action(action);
            return;
        case AutoplayActionKind::wait_ai_message: {
            GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, action.target_name.c_str(), nullptr);
            if (message != nullptr && static_cast<i32>(message->value) == action.expected_output) {
                advance_action();
            }
            return;
        }
        case AutoplayActionKind::wait_level:
            if (gameplay_ready(action.target_name.c_str())) {
                advance_action();
            }
            return;
        case AutoplayActionKind::wait_menu:
            if (GetMenuID() == action.output_index) {
                advance_action();
            }
            return;
        case AutoplayActionKind::wait_cutscene_end:
            if (CUTSTOPGAME == 0 && CutStopInfo == nullptr && GetMenuID() == -1 &&
                gameplay_ready(action.target_name.c_str())) {
                advance_action();
            }
            return;
        case AutoplayActionKind::log_gizmos:
            log_loaded_gizmos();
            advance_action();
            return;
        case AutoplayActionKind::manual_control:
            return;
    }
}
