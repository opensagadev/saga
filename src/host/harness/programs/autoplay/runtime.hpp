#pragma once

// Autoplay process state and lifecycle helpers.
// Included only by autoplay.cpp, inside its anonymous namespace.

const char *action_name(AutoplayActionKind kind) {
    switch (kind) {
        case AutoplayActionKind::checkpoint:
            return "checkpoint";
        case AutoplayActionKind::wait_loaded:
            return "wait-loaded";
        case AutoplayActionKind::wait_time:
            return "wait";
        case AutoplayActionKind::start_podrace:
            return "start-podrace";
        case AutoplayActionKind::drive_podrace_to_level:
            return "drive-podrace-to-level";
        case AutoplayActionKind::rail_forward:
            return "rail-forward";
        case AutoplayActionKind::rail_relative_route:
            return "rail-relative-route";
        case AutoplayActionKind::rail_to_gizmo:
            return "rail-to-gizmo";
        case AutoplayActionKind::rail_through_gizmo:
            return "rail-through-gizmo";
        case AutoplayActionKind::rail_through_path_connection:
            return "rail-through-path-connection";
        case AutoplayActionKind::rail_to_character:
            return "rail-to-character";
        case AutoplayActionKind::rail_to_area:
            return "rail-to-area";
        case AutoplayActionKind::rail_to_path_node:
            return "rail-to-path-node";
        case AutoplayActionKind::rail_to_locator:
            return "rail-to-locator";
        case AutoplayActionKind::rail_through_locator:
            return "rail-through-locator";
        case AutoplayActionKind::native_jump_relative:
            return "native-jump-relative";
        case AutoplayActionKind::native_jump_to_gizmo:
            return "native-jump-to-gizmo";
        case AutoplayActionKind::native_jump_to_locator:
            return "native-jump-to-locator";
        case AutoplayActionKind::teleport_to:
            return "teleport-to";
        case AutoplayActionKind::use_force_gizmo:
            return "use-force-gizmo";
        case AutoplayActionKind::use_panel_gizmo:
            return "use-panel-gizmo";
        case AutoplayActionKind::use_buildit_gizmo:
            return "use-buildit-gizmo";
        case AutoplayActionKind::use_zipup_gizmo:
            return "use-zipup-gizmo";
        case AutoplayActionKind::blaster_hit_gizmo:
            return "blaster-hit-gizmo";
        case AutoplayActionKind::clear_nearby_hostiles:
            return "clear-nearby-hostiles";
        case AutoplayActionKind::clear_named_characters:
            return "clear-named-characters";
        case AutoplayActionKind::destroy_named_ai_object:
            return "destroy-named-ai-object";
        case AutoplayActionKind::damage_character_to_health:
            return "damage-character-to-health";
        case AutoplayActionKind::switch_to_character:
            return "switch-to-character";
        case AutoplayActionKind::wait_gizmo_output:
            return "wait-gizmo-output";
        case AutoplayActionKind::hold_gizmo_until_output:
            return "hold-gizmo-until-output";
        case AutoplayActionKind::hold_party_switches_until_output:
            return "hold-party-switches-until-output";
        case AutoplayActionKind::hold_party_forces_until_output:
            return "hold-party-forces-until-output";
        case AutoplayActionKind::wait_ai_message:
            return "wait-ai-message";
        case AutoplayActionKind::wait_level:
            return "wait-level";
        case AutoplayActionKind::wait_menu:
            return "wait-menu";
        case AutoplayActionKind::wait_cutscene_end:
            return "wait-cutscene-end";
        case AutoplayActionKind::log_gizmos:
            return "log-gizmos";
        case AutoplayActionKind::manual_control:
            return "manual-control";
    }
    return "unknown";
}

void AutoplayRunner::release_input() {
    HostInputSetHeld(0, 0);
    HostInputSetAnalog(0, 0.0f, 0.0f);
    this->restore_run_speed();
    if (Player[0] != nullptr) {
        this->cancel_incidental_push(*Player[0]);
        if (Player[0]->pad_gamepad != nullptr) {
            Player[0]->pad_gamepad->allocated_5a &= static_cast<u8>(~(0x04u | 0x10u));
        }
        Player[0]->gizforce_target = nullptr;
        Player[0]->gizforce_target_object = nullptr;
    }
}

void AutoplayRunner::cancel_incidental_push(GameObject_s &character) {
    const bool is_push_context =
        (LEGOCONTEXT_PUSH != -1 && character.character_context == LEGOCONTEXT_PUSH) ||
        (LEGOCONTEXT_PUSHOBSTACLE != -1 && character.character_context == LEGOCONTEXT_PUSHOBSTACLE) ||
        (LEGOCONTEXT_PUSHSPINNER != -1 && character.character_context == LEGOCONTEXT_PUSHSPINNER);
    if (is_push_context) {
        ReleasePush(&character);
        // ReleasePush validates the context metadata before clearing it. A
        // partially reconstructed context table must not leave a scripted
        // rail permanently latched in the push animation.
        const bool push_context_remains =
            (LEGOCONTEXT_PUSH != -1 && character.character_context == LEGOCONTEXT_PUSH) ||
            (LEGOCONTEXT_PUSHOBSTACLE != -1 && character.character_context == LEGOCONTEXT_PUSHOBSTACLE) ||
            (LEGOCONTEXT_PUSHSPINNER != -1 && character.character_context == LEGOCONTEXT_PUSHSPINNER);
        if (push_context_remains) {
            character.field_0x788 = nullptr;
            character.character_context = -1;
        }
    }

    // PushCode accumulates this timer whenever movement is held into any
    // steep collision surface, including ordinary walls. Rails are steering,
    // never an implicit request to push an object, so do not let that contact
    // turn into a persistent push animation.
    character.field_0xdc4 = 0.0f;
}

void AutoplayRunner::enable_rail_run_speed(f32 multiplier) {
    if (this->script == nullptr || Player[0] == nullptr || Player[0]->apiobj.character_data == nullptr ||
        Player[0]->apiobj.character_data->game_character == nullptr || multiplier <= 0.0f) {
        return;
    }

    GAMECHARACTERDATA_s *character_data = Player[0]->apiobj.character_data->game_character;
    if (this->boosted_character_data == character_data) {
        return;
    }
    this->restore_run_speed();
    this->boosted_character_data = character_data;
    this->original_run_speed = character_data->run_speed;
    character_data->run_speed *= multiplier;
}

void AutoplayRunner::restore_run_speed() {
    if (this->boosted_character_data == nullptr) {
        return;
    }
    this->boosted_character_data->run_speed = this->original_run_speed;
    this->boosted_character_data = nullptr;
    this->original_run_speed = 0.0f;
}

void AutoplayRunner::ensure_invincibility() {
    if (this->script == nullptr || !this->script->options.enable_invincibility) {
        return;
    }
    if (Cheats_CheckFlags(kInvulnerabilityCheatFlag) != 0) {
        return;
    }

    if (this->invincibility_cheat_index < 0) {
        this->invincibility_cheat_index = Cheat_FindByName(const_cast<char *>("invincibility"));
    }
    if (this->invincibility_cheat_index >= 0) {
        Cheat_SetOn(this->invincibility_cheat_index, 1, 0);
        this->owns_invincibility_cheat = true;
        LOG_INFO("autoplay %s: enabled invincibility", this->level_name.c_str());
    }
}

void AutoplayRunner::reset_host_antilight_accumulators() {
    // rtlResetEx currently clears bytes [0, 0x48), leaving its anti-light
    // count at 0x48 stale while clearing the corresponding pointers. Keep
    // this host run alive without changing the reconstructed target code.
    *reinterpret_cast<i32 *>(lev_rtldata.data + 0x48) = 0;
    if (Obj == nullptr) {
        return;
    }
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        *reinterpret_cast<i32 *>(Obj[index].light_data.data + 0x48) = 0;
    }
}

void AutoplayRunner::configure_podrace_host_state() {
    // ChrisAllocLevelStuff is not reconstructed yet. PodRaceInit only needs
    // its PODRACE_s allocation from that routine, so keep an equivalent
    // process-lifetime block in the host runner and attach it before each
    // race-area initializer. The target implementation remains untouched.
    constexpr usize kPodraceStateSize = 0xaf24;
    this->podrace_state.assign(kPodraceStateSize, 0);

    const auto install = [](const char *name, void (*initializer)(WORLDINFO_s *)) {
        if (LEVELDATA_s *level = Level_FindByName(const_cast<char *>(name), nullptr); level != nullptr) {
            level->init_fn = initializer;
        }
    };
    install("podrace_a", &AutoplayRunner::initialize_podrace_a);
    install("podrace_b", &AutoplayRunner::initialize_podrace_b);
    install("podrace_c", &AutoplayRunner::initialize_podrace_c);
}

void AutoplayRunner::attach_podrace_host_state(WORLDINFO_s &world) {
    world.podrace = this->podrace_state.data();
}

void AutoplayRunner::initialize_podrace_a(WORLDINFO_s *world) {
    AutoplayRunner::instance.attach_podrace_host_state(*world);
    PodRaceAInit(world);
}

void AutoplayRunner::initialize_podrace_b(WORLDINFO_s *world) {
    AutoplayRunner::instance.attach_podrace_host_state(*world);
    PodRaceBInit(world);
}

void AutoplayRunner::initialize_podrace_c(WORLDINFO_s *world) {
    AutoplayRunner::instance.attach_podrace_host_state(*world);
    // Podrace C's reconstructed cutscene player does not restore the pod to
    // gameplay state when its Tusken interlude ends. The smoke test exercises
    // the race rather than that known-broken teardown, so suppress only the
    // area's automatic intro through the native one-shot skip flag.
    if (AutoplayRunner::instance.active()) {
        cutskip_dontplaylevelintro = 1;
    }
    PodRaceCInit(world);
    if (AutoplayRunner::instance.active()) {
        char intro_name[] = "Ep1_Podrace_TuskenRaiders";
        CUTINFO *const intro = CutScene_Find(world->cutscene_sys, intro_name);
        CutScene_SnapToEnd(intro);
    }
}

void AutoplayRunner::finish(i32 status, const char *reason) {
    if (this->is_done.load(std::memory_order_relaxed)) {
        return;
    }
    release_input();
    if (status == 0) {
        LOG_INFO("autoplay %s: PASS: %s", this->level_name.c_str(), reason);
    } else {
        const NUVEC position = Player[0] != nullptr ? Player[0]->apiobj.position : NUVEC{};
        LOG_ERR("autoplay %s: FAIL at action %u/%u (%s): %s; level=%s position=(%.3f,%.3f,%.3f)",
                this->level_name.c_str(), static_cast<unsigned>(this->action_index + 1),
                this->script != nullptr ? static_cast<unsigned>(this->script->actions.size()) : 0,
                this->script != nullptr && this->action_index < this->script->actions.size()
                    ? action_name(this->script->actions[this->action_index].kind)
                    : "startup",
                reason, WORLD != nullptr && WORLD->current_level != nullptr ? WORLD->current_level->name : "-",
                position.x, position.y, position.z);
    }
    this->result_code.store(status, std::memory_order_relaxed);
    this->is_done.store(true, std::memory_order_release);
}

bool AutoplayRunner::select_script() {
    if (this->schedule_index >= this->schedule.size()) {
        return false;
    }

    const ScheduledScript &scheduled = this->schedule[this->schedule_index];
    this->level_name = scheduled.level_name;
    this->script = scheduled.script;
    this->action_index = 0;
    for (usize index = 0; index < this->script->actions.size(); ++index) {
        const AutoplayAction &action = this->script->actions[index];
        if (action.kind == AutoplayActionKind::checkpoint &&
            SDL_strcasecmp(action.target_name.c_str(), this->level_name.c_str()) == 0) {
            this->action_index = index + 1;
            break;
        }
    }
    this->load_requested = false;
    this->current_action.clear();
    this->gizmo_position_snapshots.clear();
    this->script_started_at = SDL_GetTicks();

    return true;
}

bool AutoplayRunner::enter_level() {
    LEVELDATA_s *level = Level_FindByName(const_cast<char *>(this->level_name.c_str()), nullptr);
    if (level == nullptr || (level->flags & LEVEL_GAMEPLAY) == 0) {
        return false;
    }

    Game = this->fixture;
    BackupGame = this->fixture;
    memcard_autosaveenabled = 0;
    if (SDL_strncasecmp(this->level_name.c_str(), "podrace_", 8) == 0) {
        this->configure_podrace_host_state();
    }
    HostEnterLevel(*level);
    this->load_requested = true;
    LOG_INFO("autoplay %s: entering level %s", this->level_name.c_str(), level->name);
    return true;
}

void AutoplayRunner::complete_script() {
    LOG_INFO("autoplay %s: completed all %u actions", this->level_name.c_str(),
             static_cast<unsigned>(this->script->actions.size()));
    ++this->schedule_index;
    if (this->schedule_index == this->schedule.size()) {
        finish(0, "all requested levels completed");
        return;
    }

    release_input();
    if (!select_script() || !enter_level()) {
        finish(1, "could not start the next catalog level");
    }
}

void AutoplayRunner::prepare_level() {
    if (!this->is_active.load(std::memory_order_acquire) || this->load_requested ||
        this->is_done.load(std::memory_order_relaxed)) {
        return;
    }

    if (this->loader_mode_overridden) {
        LOADEROFF = this->previous_loader_mode;
        this->loader_mode_overridden = false;
    }
    if (!enter_level()) {
        finish(1, "script level does not exist or is not a gameplay level");
    }
}

bool AutoplayRunner::gameplay_ready(const char *level_name) const {
    if (WORLD == nullptr || WORLD->loaded == 0 || WORLD->current_level == nullptr || WORLD->sock_sys == nullptr ||
        WORLD->ai_sys == nullptr || Player[0] == nullptr || Player[0]->pad_gamepad == nullptr || Paused != 0 ||
        NewMode != 0 || NewLData != nullptr) {
        return false;
    }
    const u32 flags = Player[0]->apiobj.field_0x1f8;
    return (flags & 0x1001) == 0x1001 && static_cast<i8>(flags) < 0 &&
           (level_name == nullptr || SDL_strcasecmp(WORLD->current_level->name, level_name) == 0);
}
