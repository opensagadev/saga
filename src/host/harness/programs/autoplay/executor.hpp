#pragma once

// Action lifecycle and dispatch. std::variant makes every dispatch exhaustive:
// adding an action type requires handling it here at compile time.

void AutoplayRunner::advance_action() {
    release_input();
    const auto name = action_name(this->script->actions[this->action_index]);
    LOG_INFO("autoplay %s: completed action %u/%u (%.*s)", this->level_name.c_str(),
             static_cast<unsigned>(this->action_index + 1), static_cast<unsigned>(this->script->actions.size()),
             static_cast<i32>(name.size()), name.data());
    ++this->action_index;
    this->current_action.reset();
    if (this->action_index == this->script->actions.size()) {
        complete_script();
    }
}

bool AutoplayRunner::begin_action(const AutoplayAction &action) {
    this->current_action.emplace(make_runtime_for(action.payload));
    const auto name = action_name(action);
    LOG_INFO("autoplay %s: starting action %u/%u (%.*s)", this->level_name.c_str(),
             static_cast<unsigned>(this->action_index + 1), static_cast<unsigned>(this->script->actions.size()),
             static_cast<i32>(name.size()), name.data());

    return std::visit(Overloaded{
                          [this](const RailAction &rail) {
                              return std::visit(
                                  Overloaded{
                                      [this, &rail](const RailForward &destination) {
                                          return this->begin_forward_rail(rail, destination);
                                      },
                                      [this, &rail](const RailRelativeRoute &destination) {
                                          return this->begin_relative_rail(rail, destination);
                                      },
                                      [this, &rail](const RailThroughGizmo &destination) {
                                          return this->begin_crossing_rail(rail, destination);
                                      },
                                      [this, &rail](const RailThroughPathConnection &destination) {
                                          return this->begin_path_connection_rail(rail, destination);
                                      },
                                      [this, &rail](const auto &) { return this->begin_target_rail(rail); },
                                  },
                                  rail.destination);
                          },
                          [this](const NativeJumpAction &jump) { return this->begin_native_jump(jump); },
                          [this](const HoldGizmoAction &hold) {
                              auto target = gizmo_use_position(find_gizmo(hold.standing_gizmo.c_str()));
                              if (!target) {
                                  return false;
                              }
                              this->runtime<RailRuntime>().rail.waypoints = {RailWaypoint{*target}};
                              return true;
                          },
                          [this](const HoldPartySwitchesAction &hold) {
                              auto &positions = this->runtime<PartySwitchRuntime>().positions;
                              positions.reserve(hold.placements.size());
                              for (const auto &[character, gizmo] : hold.placements) {
                                  static_cast<void>(character);
                                  auto position = gizmo_use_position(find_gizmo(gizmo.c_str()));
                                  if (!position) {
                                      return false;
                                  }
                                  positions.push_back(*position);
                              }
                              return true;
                          },
                          [](const auto &) { return true; },
                      },
                      action.payload);
}

void AutoplayRunner::tick_action(const AutoplayAction &action) {
    const Milliseconds elapsed{SDL_GetTicks() - this->current_action->started_at};
    if (action.timeout != Milliseconds::zero() && elapsed > action.timeout) {
        finish(1, "action timeout");
        return;
    }

    std::visit(Overloaded{
                   [this](const CheckpointAction &) { advance_action(); },
                   [this](const WaitLoadedAction &) {
                       if (gameplay_ready(nullptr)) {
                           advance_action();
                       }
                   },
                   [this, elapsed](const WaitAction &wait) {
                       if (elapsed >= wait.duration) {
                           advance_action();
                       }
                   },
                   [this](const StartPodraceAction &) { tick_start_podrace_action(); },
                   [this](const DrivePodraceAction &drive) { tick_podrace_drive_action(drive); },
                   [this](const RailAction &rail) { tick_rail_action(rail); },
                   [this](const NativeJumpAction &) { tick_native_jump_action(); },
                   [this](const TeleportAction &teleport) {
                       if (!gameplay_ready(nullptr) || Player[0] == nullptr) {
                           return;
                       }
                       NUVEC position = teleport.position;
                       const i32 angle = Player[0]->apiobj.facing_angle;
                       SnapCreaturePos(Player[0], &position, angle, nullptr, 1);
                       LOG_INFO("autoplay %s: teleported player to (%.3f,%.3f,%.3f)", this->level_name.c_str(),
                                Player[0]->apiobj.position.x, Player[0]->apiobj.position.y,
                                Player[0]->apiobj.position.z);
                       advance_action();
                   },
                   [this](const UseForceAction &use) { tick_force_action(use); },
                   [this](const UsePanelAction &use) { tick_panel_action(use); },
                   [this](const UseBuildItAction &use) { tick_build_action(use); },
                   [this](const UseZipUpAction &use) { tick_zipup_action(use); },
                   [this](const BlasterHitAction &hit) { tick_blaster_hit_action(hit); },
                   [this](const ClearHostilesAction &clear) { tick_clear_hostiles_action(clear); },
                   [this](const DestroyAiObjectAction &destroy) { tick_destroy_ai_object_action(destroy); },
                   [this](const DamageCharacterAction &damage) { tick_damage_character_action(damage); },
                   [this](const SwitchCharacterAction &change) { tick_character_switch(change); },
                   [this](const WaitGizmoOutputAction &wait) {
                       const auto &[gizmo_name, output_index, expected] = wait.condition;
                       GIZMO *gizmo = find_gizmo(gizmo_name.c_str());
                       if (gizmo != nullptr && GizmoGetOutput(WORLD->gizmo_sys, gizmo, output_index, 1) == expected) {
                           advance_action();
                       }
                   },
                   [this](const HoldGizmoAction &hold) { tick_hold_gizmo_action(hold); },
                   [this](const HoldPartySwitchesAction &hold) { tick_hold_party_switches_action(hold); },
                   [this](const HoldPartyForcesAction &hold) { tick_hold_party_forces_action(hold); },
                   [this](const WaitAiMessageAction &wait) {
                       GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, wait.message.c_str(), nullptr);
                       if (message != nullptr && static_cast<i32>(message->value) == wait.expected) {
                           advance_action();
                       }
                   },
                   [this](const WaitLevelAction &wait) {
                       if (gameplay_ready(wait.level.c_str())) {
                           advance_action();
                       }
                   },
                   [this](const WaitMenuAction &wait) {
                       if (GetMenuID() == wait.menu_id) {
                           advance_action();
                       }
                   },
                   [this](const WaitCutsceneEndAction &wait) {
                       if (CUTSTOPGAME == 0 && CutStopInfo == nullptr && GetMenuID() == -1 &&
                           gameplay_ready(wait.level.c_str())) {
                           advance_action();
                       }
                   },
                   [this](const LogGizmosAction &) {
                       log_loaded_gizmos();
                       advance_action();
                   },
                   [](const ManualControlAction &) {},
               },
               action.payload);
}
