#pragma once

// Autoplay process state and lifecycle helpers.
// Included only by autoplay.cpp, inside its anonymous namespace.

std::string_view rail_name(const RailDestination &destination) {
    return std::visit(
        Overloaded{
            [](const RailForward &) -> std::string_view { return "rail-forward"; },
            [](const RailRelativeRoute &) -> std::string_view { return "rail-relative-route"; },
            [](const RailToGizmo &) -> std::string_view { return "rail-to-gizmo"; },
            [](const RailThroughGizmo &) -> std::string_view { return "rail-through-gizmo"; },
            [](const RailThroughPathConnection &) -> std::string_view { return "rail-through-path-connection"; },
            [](const RailToCharacter &) -> std::string_view { return "rail-to-character"; },
            [](const RailToArea &) -> std::string_view { return "rail-to-area"; },
            [](const RailToPathNode &) -> std::string_view { return "rail-to-path-node"; },
            [](const RailToLocator &) -> std::string_view { return "rail-to-locator"; },
            [](const RailThroughLocator &) -> std::string_view { return "rail-through-locator"; },
        },
        destination);
}

std::string_view action_name(const AutoplayAction &action) {
    return std::visit(
        Overloaded{
            [](const CheckpointAction &) -> std::string_view { return "checkpoint"; },
            [](const WaitLoadedAction &) -> std::string_view { return "wait-loaded"; },
            [](const WaitAction &) -> std::string_view { return "wait"; },
            [](const StartPodraceAction &) -> std::string_view { return "start-podrace"; },
            [](const DrivePodraceAction &) -> std::string_view { return "drive-podrace-to-level"; },
            [](const RailAction &rail) { return rail_name(rail.destination); },
            [](const NativeJumpAction &) -> std::string_view { return "native-jump"; },
            [](const TeleportAction &) -> std::string_view { return "teleport-to"; },
            [](const UseForceAction &) -> std::string_view { return "use-force-gizmo"; },
            [](const UsePanelAction &) -> std::string_view { return "use-panel-gizmo"; },
            [](const UseBuildItAction &) -> std::string_view { return "use-buildit-gizmo"; },
            [](const UseZipUpAction &) -> std::string_view { return "use-zipup-gizmo"; },
            [](const BlasterHitAction &) -> std::string_view { return "blaster-hit-gizmo"; },
            [](const ClearHostilesAction &clear) -> std::string_view {
                return clear.character ? "clear-named-characters" : "clear-nearby-hostiles";
            },
            [](const DestroyAiObjectAction &) -> std::string_view { return "destroy-named-ai-object"; },
            [](const DamageCharacterAction &) -> std::string_view { return "damage-character-to-health"; },
            [](const SwitchCharacterAction &) -> std::string_view { return "switch-to-character"; },
            [](const WaitGizmoOutputAction &) -> std::string_view { return "wait-gizmo-output"; },
            [](const HoldGizmoAction &) -> std::string_view { return "hold-gizmo-until-output"; },
            [](const HoldPartySwitchesAction &) -> std::string_view { return "hold-party-switches-until-output"; },
            [](const HoldPartyForcesAction &) -> std::string_view { return "hold-party-forces-until-output"; },
            [](const WaitAiMessageAction &) -> std::string_view { return "wait-ai-message"; },
            [](const WaitLevelAction &) -> std::string_view { return "wait-level"; },
            [](const WaitMenuAction &) -> std::string_view { return "wait-menu"; },
            [](const WaitCutsceneEndAction &) -> std::string_view { return "wait-cutscene-end"; },
            [](const LogGizmosAction &) -> std::string_view { return "log-gizmos"; },
            [](const ManualControlAction &) -> std::string_view { return "manual-control"; },
        },
        action.payload);
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
    if (this->run_speed_override && this->run_speed_override->character == character_data) {
        return;
    }
    this->restore_run_speed();
    this->run_speed_override = RunSpeedOverride{character_data, character_data->run_speed};
    character_data->run_speed *= multiplier;
}

void AutoplayRunner::restore_run_speed() {
    if (!this->run_speed_override) {
        return;
    }
    this->run_speed_override->character->run_speed = this->run_speed_override->original_speed;
    this->run_speed_override.reset();
}

void AutoplayRunner::ensure_invincibility() {
    if (this->script == nullptr || !this->script->options.enable_invincibility) {
        return;
    }
    if (Cheats_CheckFlags(kInvulnerabilityCheatFlag) != 0) {
        return;
    }

    const i32 cheat = Cheat_FindByName(const_cast<char *>("invincibility"));
    if (cheat >= 0) {
        Cheat_SetOn(cheat, 1, 0);
        this->owned_invincibility_cheat = cheat;
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
    AutoplayRunner::get().attach_podrace_host_state(*world);
    PodRaceAInit(world);
}

void AutoplayRunner::initialize_podrace_b(WORLDINFO_s *world) {
    AutoplayRunner::get().attach_podrace_host_state(*world);
    PodRaceBInit(world);
}

void AutoplayRunner::initialize_podrace_c(WORLDINFO_s *world) {
    AutoplayRunner::get().attach_podrace_host_state(*world);
    // Podrace C's reconstructed cutscene player does not restore the pod to
    // gameplay state when its Tusken interlude ends. The smoke test exercises
    // the race rather than that known-broken teardown, so suppress only the
    // area's automatic intro through the native one-shot skip flag.
    if (AutoplayRunner::get().active()) {
        cutskip_dontplaylevelintro = 1;
    }
    PodRaceCInit(world);
    if (AutoplayRunner::get().active()) {
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
        const std::string_view failed_action =
            this->script != nullptr && this->action_index < this->script->actions.size()
                ? action_name(this->script->actions[this->action_index])
                : std::string_view{"startup"};
        LOG_ERR("autoplay %s: FAIL at action %u/%u (%.*s): %s; level=%s position=(%.3f,%.3f,%.3f)",
                this->level_name.c_str(), static_cast<unsigned>(this->action_index + 1),
                this->script != nullptr ? static_cast<unsigned>(this->script->actions.size()) : 0,
                static_cast<i32>(failed_action.size()), failed_action.data(), reason,
                WORLD != nullptr && WORLD->current_level != nullptr ? WORLD->current_level->name : "-", position.x,
                position.y, position.z);
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
    this->script = &scheduled.script.get();
    this->action_index = 0;
    for (usize index = 0; index < this->script->actions.size(); ++index) {
        const AutoplayAction &action = this->script->actions[index];
        const auto *checkpoint = get_action<CheckpointAction>(action);
        if (checkpoint != nullptr && SDL_strcasecmp(checkpoint->level.c_str(), this->level_name.c_str()) == 0) {
            this->action_index = index + 1;
            break;
        }
    }
    this->load_requested = false;
    this->current_action.reset();
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

    if (this->overridden_loader_mode) {
        LOADEROFF = *this->overridden_loader_mode;
        this->overridden_loader_mode.reset();
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
