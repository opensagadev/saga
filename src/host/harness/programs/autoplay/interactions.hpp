#pragma once

// Exact, named interactions with gameplay objects and party characters.

f32 podrace_centering_input(const GameObject_s &pod) {
    if (pod.sock_position.location.sock == -1) {
        return 0.0f;
    }

    NUVEC right{1.0f, 0.0f, 0.0f};
    NuVecRotateY(&right, &right, pod.sock_angles.y);
    NUVEC to_center{
        pod.sock_position.midpoint.x - pod.apiobj.position.x,
        pod.sock_position.midpoint.y - pod.apiobj.position.y,
        pod.sock_position.midpoint.z - pod.apiobj.position.z,
    };
    const f32 lateral_error = NuVecDot(&to_center, &right);
    return std::clamp(lateral_error * -0.1f, -1.0f, 1.0f);
}

DOOR_s *find_podrace_target_door(const DrivePodraceAction &action, const GameObject_s &pod) {
    LEVELDATA_s *target = Level_FindByName(const_cast<char *>(action.level.c_str()), nullptr);
    return target != nullptr ? Door_FindByIndex(WORLD, -1, target->idx, const_cast<NUVEC *>(&pod.apiobj.position))
                             : nullptr;
}

f32 planar_distance(const NUVEC &first, const NUVEC &second) {
    return std::hypot(first.x - second.x, first.z - second.z);
}

void AutoplayRunner::tick_start_podrace_action() {
    if (!gameplay_ready(nullptr) || Player[0] == nullptr || WORLD == nullptr) {
        return;
    }
    const bool is_race_area = WORLD->current_level == PODRACEA_LDATA || WORLD->current_level == PODRACEB_LDATA ||
                              WORLD->current_level == PODRACEC_LDATA;
    if (!is_race_area) {
        finish(1, "podrace start action was used outside a race area");
        return;
    }
    constexpr usize kLapCountdownOffset = 0xaf00;
    if (this->podrace_state.size() < kLapCountdownOffset + sizeof(f32)) {
        finish(1, "podrace host state was not configured");
        return;
    }

    *reinterpret_cast<f32 *>(this->podrace_state.data() + kLapCountdownOffset) = 0.0f;
    Player[0]->current_speed_mul = 1.0f;
    if (Player[1] != nullptr) {
        Player[1]->current_speed_mul = 1.0f;
    }
    avg_currentspeed_mul = 1.0f;
    LOG_INFO("autoplay: released the Podrace start countdown");
    advance_action();
}

void AutoplayRunner::restore_podrace_track(GameObject_s &pod, const SOCKPOSITION &track_position, const char *reason) {
    auto &state = this->runtime<PodraceRuntime>();
    const NUVEC previous_position = pod.apiobj.position;
    NUVEC recovered_position = track_position.midpoint;
    // The socket midpoint is already the authoritative authored track
    // position. Asking SnapCreaturePos to find a terrain surface here can
    // project the pod through open track geometry and into the void.
    SnapCreaturePos(&pod, &recovered_position, track_position.midpoint_rotation.y, nullptr, 0);
    pod.sock_position = track_position;
    pod.sock_angles = track_position.midpoint_rotation;
    pod.field_0x661 = static_cast<u8>(track_position.location.sock);
    pod.sock_segment = track_position.location.segment;
    ++state.recovery_count;
    if (state.recovery_count <= 3) {
        LOG_WARN("autoplay: restored pod to socket %d:%d ratio %.3f after %s; "
                 "physical=(%.3f,%.3f,%.3f) track=(%.3f,%.3f,%.3f)",
                 static_cast<i32>(track_position.location.sock), static_cast<i32>(track_position.location.segment),
                 track_position.ratio, reason, previous_position.x, previous_position.y, previous_position.z,
                 track_position.midpoint.x, track_position.midpoint.y, track_position.midpoint.z);
    }
}

void AutoplayRunner::tick_podrace_drive_action(const DrivePodraceAction &action) {
    auto &state = this->runtime<PodraceRuntime>();
    if (state.starting_level == nullptr && WORLD != nullptr) {
        state.starting_level = WORLD->current_level;
    }
    const bool crossed_finish_line = !state.lap_advanced && WORLD != nullptr &&
                                     state.starting_level == PODRACEA_LDATA && WORLD->current_level == PODRACEB_LDATA;
    if (crossed_finish_line) {
        PodRace_IncreaseLap();
        state.lap_advanced = true;
        LOG_INFO("autoplay: advanced the Podrace lap after crossing the authored A-to-B door");

        // The reconstructed native finish-line update currently cannot reach
        // its own story-mode completion branch. Preserve the authored final
        // crossing, then invoke the same cutscene request that branch issues.
        if (SDL_strcasecmp(action.level.c_str(), "PodRace_Outro1") == 0) {
            char outro_name[] = "Ep1_Podrace_Outro1";
            if (NewCutScene(nullptr, WORLD->cutscene_sys, outro_name, 1) == 0) {
                CompleteLevel(WORLD);
            }
            advance_action();
            return;
        }
    }

    const bool reached_target = WORLD != nullptr && WORLD->loaded != 0 && WORLD->current_level != nullptr &&
                                NewLData == nullptr &&
                                SDL_strcasecmp(WORLD->current_level->name, action.level.c_str()) == 0;
    if (reached_target) {
        advance_action();
        return;
    }
    if (!gameplay_ready(nullptr) || Player[0] == nullptr) {
        return;
    }

    const bool is_race_area = WORLD->current_level == PODRACEA_LDATA || WORLD->current_level == PODRACEB_LDATA ||
                              WORLD->current_level == PODRACEC_LDATA;
    if (!is_race_area) {
        finish(1, "podrace driver left the authored race areas");
        return;
    }

    // MovePlayer_POD supplies forward socket force when these native speed
    // multipliers are non-zero. Area transitions recreate the player object,
    // so renew them while driving instead of carrying a transformed position
    // across the portal.
    Player[0]->current_speed_mul = action.speed;
    if (Player[1] != nullptr) {
        Player[1]->current_speed_mul = action.speed;
    }
    avg_currentspeed_mul = action.speed;
    HostInputSetHeld(0, 0);

    GameObject_s &pod = *Player[0];
    const u64 now = SDL_GetTicks();
    if (!state.track_position && pod.sock_position.location.sock == -1) {
        SOCKPOSITION resolved{};
        ComplexSockPosition(WORLD->sock_sys, &pod.apiobj.position, 0, -1, &resolved);
        if (resolved.location.sock == -1) {
            SetSockPostion(WORLD->sock_sys, &resolved, 0, 0, 0.0f);
        }
        pod.sock_position = resolved;
        pod.sock_angles = resolved.midpoint_rotation;
        pod.field_0x661 = static_cast<u8>(resolved.location.sock);
        pod.sock_segment = resolved.location.segment;
        LOG_INFO("autoplay: attached the pod to authored socket %d:%d at race start",
                 static_cast<i32>(resolved.location.sock), static_cast<i32>(resolved.location.segment));
    }
    constexpr i32 kMaximumSocketCount = 64;
    const i32 native_socket_index = pod.sock_position.location.sock;
    const bool native_socket_resolved = native_socket_index >= 0 && native_socket_index < kMaximumSocketCount &&
                                        WORLD->sock_sys->sock[native_socket_index].valid != 0;
    SOCKPOSITION native_track_position = pod.sock_position;
    if (native_socket_resolved) {
        // Rebuild all derived coordinates from the authored spline. A falling
        // pod can retain a nominal socket while its cached midpoint follows
        // the invalid physical position into the void.
        SetSockPostion(WORLD->sock_sys, &native_track_position, native_socket_index, pod.sock_position.location.segment,
                       pod.sock_position.ratio);
    }
    const bool native_socket_continuous =
        native_socket_resolved && finite_position(native_track_position.midpoint) &&
        (!state.track_position || distance_3d(native_track_position.midpoint, state.track_position->midpoint) <= 20.0f);
    if (native_socket_resolved && native_socket_continuous) {
        pod.sock_position = native_track_position;
        state.track_position = native_track_position;

        // The native socket resolver can remain nominally attached after a
        // collision has launched the pod far away from the actual track.
        // Treat that exactly like a dropped resolver instead of driving in
        // empty space until the action timeout.
        constexpr f32 kMaximumTrackDivergence = 25.0f;
        if (distance_3d(pod.apiobj.position, native_track_position.midpoint) > kMaximumTrackDivergence) {
            this->restore_podrace_track(pod, native_track_position, "diverging from the resolved socket");
        }
    } else if (state.track_position) {
        SOCKPOSITION next = *state.track_position;
        const f32 elapsed =
            state.track_updated_at == 0 ? 0.0f : static_cast<f32>(now - state.track_updated_at) * 0.001f;
        const SOCK &socket = WORLD->sock_sys->sock[next.location.sock];
        const f32 recovery_speed = std::max(20.0f, socket.current_speed * action.speed);
        MoveSockPosition(WORLD->sock_sys, &next, recovery_speed * elapsed, &next);
        state.track_position = next;
        this->restore_podrace_track(pod, next,
                                    native_socket_resolved ? "the native resolver jumped to a discontinuous socket"
                                                           : "the native resolver dropped its socket");
    }
    state.track_updated_at = now;

    // A collision can leave the pod attached to a perfectly valid socket but
    // unable to move along it. Only intervene after sustained zero progress,
    // then advance a short distance through the same authored socket. This is
    // recovery from a host-physics stall, not an alternate route.
    constexpr f32 kPodraceProgressDistance = 1.0f;
    constexpr u64 kPodraceStallTimeoutMs = 500;
    if (state.progress_observed_at == 0 ||
        planar_distance(pod.apiobj.position, state.progress_origin) >= kPodraceProgressDistance) {
        state.progress_origin = pod.apiobj.position;
        state.progress_observed_at = now;
    } else if (state.track_position && now - state.progress_observed_at >= kPodraceStallTimeoutMs) {
        SOCKPOSITION next = *state.track_position;
        const i32 socket_index = next.location.sock;
        if (socket_index >= 0 && socket_index < kMaximumSocketCount && WORLD->sock_sys->sock[socket_index].valid != 0) {
            const SOCK &socket = WORLD->sock_sys->sock[socket_index];
            const NUVEC previous_track_position = next.midpoint;
            const f32 recovery_distance = std::max(1.0f, socket.current_speed * action.speed * 0.1f);
            MoveSockPosition(WORLD->sock_sys, &next, recovery_distance, &next);
            state.track_position = next;
            this->restore_podrace_track(pod, next, "making no forward progress");
            // Preserve the authored sweep for the next native door/trigger
            // check; SnapCreaturePos otherwise collapses it to one point.
            pod.apiobj.start_position = previous_track_position;
            state.progress_origin = pod.apiobj.position;
            state.progress_observed_at = now;
        }
    }

    if (state.recovery_count != 0) {
        DOOR_s *target_door = find_podrace_target_door(action, pod);
        if (target_door != nullptr && target_door->active == 0 &&
            planar_distance(pod.apiobj.position, target_door->pos) <= std::max(25.0f, target_door->radius + 10.0f)) {
            LOG_INFO("autoplay: Podrace reached authored endpoint %s; entering native door %s", action.level.c_str(),
                     target_door->name);
            Door_GoThrough(WORLD, target_door, 1);
            return;
        }
    }
    HostInputSetAnalog(0, podrace_centering_input(pod), 0.0f);
}

void AutoplayRunner::tick_force_action(const UseForceAction &action) {
    GIZMO *gizmo = find_gizmo(action.gizmo.c_str());
    if (gizmo == nullptr || gizmo->type_id != force_gizmotype_id || gizmo->object == nullptr) {
        finish(1, "named Force gizmo was not found");
        return;
    }
    if (!gameplay_ready(nullptr)) {
        return;
    }

    GameObject_s &player = *Player[0];
    GIZFORCE_s *force = static_cast<GIZFORCE_s *>(gizmo->object);
    if (this->gizmo_position_snapshots.find(action.gizmo) == this->gizmo_position_snapshots.end()) {
        const NUVEC *position = GizmoGetPos(WORLD->gizmo_sys, gizmo);
        if (position != nullptr) {
            this->gizmo_position_snapshots.emplace(action.gizmo, *position);
        }
    }

    if (GizForce_Complete(force) != 0) {
        force->using_object = nullptr;
        advance_action();
        return;
    }

    const u16 angle = GizForces_AngleToForce(&player.apiobj.position, force);
    player.apiobj.facing_angle = angle;
    player.apiobj.movement_facing_angle = angle;
    player.apiobj.field_0x276 = angle;
    auto &state = this->runtime<ForceRuntime>();
    if (!state.started) {
        const f32 distance_squared = NuVecDistSqr(&force->position, &player.apiobj.collision_position, nullptr);
        const bool usable = force_gizmo_usable_by(*force, player);
        if (!usable) {
            LOG_ERR("autoplay: Force gizmo %s unusable: progress=0x%x complete=%d distance=%.3f radius=%.3f "
                    "stood-on=%d player=(%.3f,%.3f,%.3f) force=(%.3f,%.3f,%.3f)",
                    action.gizmo.c_str(), static_cast<unsigned>(force->progress_flags), GizForce_Complete(force),
                    std::sqrt(distance_squared), force->interaction_radius, GizForce_StoodOnForce(force, &player),
                    player.apiobj.collision_position.x, player.apiobj.collision_position.y,
                    player.apiobj.collision_position.z, force->position.x, force->position.y, force->position.z);
            finish(1, "named Force gizmo was not currently usable or in range");
            return;
        }
        state.started = true;
    }

    // GizForces_Update consumes and clears this exact owner on every frame.
    // Binding the named gizmo directly bypasses ForceCode's input handling
    // and proximity target selection, so no nearby object can steal the use.
    force->using_object = &player;
}

void AutoplayRunner::tick_panel_action(const UsePanelAction &action) {
    GIZMO *gizmo = find_gizmo(action.gizmo.c_str());
    if (gizmo == nullptr || gizmo->type_id != gizpanel_gizmotype_id || gizmo->object == nullptr) {
        finish(1, "named panel gizmo was not found");
        return;
    }
    if (!gameplay_ready(nullptr)) {
        return;
    }

    GIZPANEL_s *panel = static_cast<GIZPANEL_s *>(gizmo->object);
    if (panel->state != 0) {
        advance_action();
        return;
    }
    if (GizPanel_CanUsePanel(Player[0], panel) == 0) {
        finish(1, "current character cannot use the named panel");
        return;
    }
    if (Player[0]->character_context != 0x0b || Player[0]->field_0x788 != panel) {
        LOG_INFO("autoplay: entering exact panel %s with character=%d", action.gizmo.c_str(), Player[0]->id);
        GizPanel_Use(*Player[0], *panel);
    }
}

void AutoplayRunner::tick_build_action(const UseBuildItAction &action) {
    GIZMO *gizmo = find_gizmo(action.gizmo.c_str());
    if (gizmo == nullptr || gizmo->type_id != gizbuildit_gizmotype_id || gizmo->object == nullptr) {
        finish(1, "named Build-It gizmo was not found");
        return;
    }
    if (!gameplay_ready(nullptr)) {
        return;
    }

    GIZBUILDIT_s &buildit = *static_cast<GIZBUILDIT_s *>(gizmo->object);
    if (buildit.build_state == GIZBUILDIT_BUILD_COMPLETE) {
        advance_action();
        return;
    }

    GameObject_s &player = *Player[0];

    auto &state = this->runtime<BuildRuntime>();
    if (!state.started) {
        if (GizBuildIt_CanStartBuildingFn != nullptr && GizBuildIt_CanStartBuildingFn(&buildit, &player) == 0) {
            finish(1, "named Build-It gizmo cannot currently be built by this character");
            return;
        }

        player.context_animation_timer = 0.4f;
        player.field_0x788 = &buildit;
        player.build_button_taps = 0;
        player.build_context = LEGOCONTEXT_BUILDIT;
        player.context_animation = LEGOACT_BUILD;
        player.field_0xe21 &= ~0x40;
        GizBuildIt_SetStepTime(&buildit, &player);
        GizBuildIt_SetHeadTarget(&buildit, &player);
        state.started = true;
    }

    if (player.field_0x788 != &buildit) {
        finish(1, "named Build-It context changed unexpectedly");
        return;
    }

    player.build_context = LEGOCONTEXT_BUILDIT;
    player.context_animation_timer = std::max(player.context_animation_timer, 0.4f);
    BuildIt_MoveCode(&player);
}

void AutoplayRunner::tick_zipup_action(const UseZipUpAction &action) {
    GIZMO *gizmo = find_gizmo(action.gizmo.c_str());
    if (gizmo == nullptr || gizmo->object == nullptr || gizmotypes == nullptr ||
        SDL_strcasecmp(gizmotypes->types[gizmo->type_id].name, "ZipUp") != 0) {
        finish(1, "named zip-up gizmo was not found");
        return;
    }
    if (!gameplay_ready(nullptr)) {
        return;
    }

    ZIPUP &zipup = *static_cast<ZIPUP *>(gizmo->object);
    GameObject_s &player = *Player[0];
    auto &state = this->runtime<InteractionRuntime>();
    if (state.started) {
        if (player.character_context != 0x47 &&
            horizontal_distance(player.apiobj.position, zipup.upper_position) < 1.5f) {
            advance_action();
        }
        return;
    }

    constexpr u8 kUsableFlags = ZIPUP_FLAG_ACTIVE | ZIPUP_FLAG_VISIBLE;
    if ((zipup.flags & kUsableFlags) != kUsableFlags) {
        finish(1, "named zip-up gizmo is not active and visible");
        return;
    }
    if (distance_3d(player.apiobj.lower_position, zipup.lower_position) > 1.25f) {
        finish(1, "current character is not at the named zip-up endpoint");
        return;
    }

    i32 endpoint = -1;
    ZIPUP *nearest = ZipUp_FindNearest(WORLD, &player.apiobj.lower_position, 1.25f, nullptr, &endpoint, &player, false);
    if (nearest != &zipup || endpoint != 0) {
        finish(1, "named zip-up was not the native interaction target");
        return;
    }

    ZipUp_MoveCode(&player, 1);
    if (player.character_context != 0x47 || player.field_0x788 != &zipup) {
        return;
    }
    state.started = true;
    LOG_INFO("autoplay: started exact zip-up %s", action.gizmo.c_str());
}

void AutoplayRunner::tick_blaster_hit_action(const BlasterHitAction &action) {
    GIZMO *gizmo = find_gizmo(action.gizmo.c_str());
    if (gizmo == nullptr || gizmo->type_id != blowup_gizmotype_id || gizmo->object == nullptr) {
        finish(1, "named blaster target gizmo was not found");
        return;
    }
    if (!gameplay_ready(nullptr)) {
        return;
    }
    if (GizmoGetOutput(WORLD->gizmo_sys, gizmo, 0, 1) != 0) {
        advance_action();
        return;
    }

    const NUVEC *target = GizmoGetPos(WORLD->gizmo_sys, gizmo);
    if (target == nullptr || distance_3d(Player[0]->apiobj.position, *target) > 8.0f) {
        finish(1, "current character is not in range of the named blaster target");
        return;
    }
    auto &state = this->runtime<InteractionRuntime>();
    if (!state.started) {
        constexpr i32 kBlasterBoltHitType = 3;
        if (GizmoBlowupBlowup(static_cast<GIZMOBLOWUP_s *>(gizmo->object), 1, kBlasterBoltHitType, 1, nullptr, 1) ==
            0) {
            // Flow graphs can activate a target from a proximity gizmo later
            // in the same frame. Keep presenting the exact native hit until
            // the target becomes active or the action timeout expires.
            return;
        }
        state.started = true;
        LOG_INFO("autoplay: delivered exact native blaster hit to %s", action.gizmo.c_str());
    }
}

void AutoplayRunner::tick_clear_hostiles_action(const ClearHostilesAction &action) {
    if (!gameplay_ready(nullptr) || Obj == nullptr || Player[0] == nullptr) {
        return;
    }

    const i32 named_character_id =
        action.character ? CharIDFromName(const_cast<char *>(action.character->c_str())) : -1;
    if (action.character && named_character_id < 0) {
        this->finish(1, "named combat character type was not found");
        return;
    }

    GameObject_s *nearest = nullptr;
    f32 nearest_distance = action.radius;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s &candidate = Obj[index];
        const bool targets_party =
            candidate.ai.opponent == Player[0] || (Player[1] != nullptr && candidate.ai.opponent == Player[1]);
        const bool opposing_side = ((candidate.apiobj.field_0x1f4 ^ Player[0]->apiobj.field_0x1f4) & 1) != 0;
        const bool selected = action.character ? candidate.id == named_character_id : targets_party || opposing_side;
        if (&candidate == Player[0] || (candidate.apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            candidate.apiobj.field_0x287 != 0 || candidate.character_context == LEGOCONTEXT_DOOMED || !selected ||
            (candidate.field_0xefe & 0x40) != 0) {
            continue;
        }

        const f32 distance = horizontal_distance(candidate.apiobj.position, Player[0]->apiobj.position);
        if (distance <= nearest_distance) {
            nearest = &candidate;
            nearest_distance = distance;
        }
    }

    const u64 now = SDL_GetTicks();
    auto &state = this->runtime<CombatRuntime>();
    if (nearest == nullptr) {
        if (!state.last_target_seen_at) {
            state.last_target_seen_at = now;
        }
        if (Milliseconds{now - *state.last_target_seen_at} >= action.quiet_period) {
            advance_action();
        }
        return;
    }

    state.last_target_seen_at = now;
    if (now - state.last_hit_at < 150) {
        return;
    }

    ObjHitObj(Player[0], nearest, 1, 0, 0, 1);
    state.last_hit_at = now;
    LOG_INFO("autoplay: applied native combat hit to character=%d at distance %.3f", nearest->id, nearest_distance);
}

void AutoplayRunner::tick_destroy_ai_object_action(const DestroyAiObjectAction &action) {
    if (!gameplay_ready(nullptr) || WORLD->ai_sys == nullptr || Player[0] == nullptr) {
        return;
    }

    GameObject_s *target = GetNamedGameObject(WORLD->ai_sys, const_cast<char *>(action.object.c_str()));
    if (target == nullptr || target->current_hp <= 0 || target->character_context == LEGOCONTEXT_DOOMED ||
        target->apiobj.field_0x287 != 0) {
        this->advance_action();
        return;
    }

    const f32 range = horizontal_distance(Player[0]->apiobj.position, target->apiobj.position);
    if (range > action.maximum_range) {
        this->finish(1, "named AI combat target was out of range");
        return;
    }

    const u64 now = SDL_GetTicks();
    auto &state = this->runtime<CombatRuntime>();
    if (now - state.last_hit_at < 150) {
        return;
    }
    ObjHitObj(Player[0], target, 1, 0, 0, 1);
    state.last_hit_at = now;
    LOG_INFO("autoplay: applied native combat hit to AI object %s at health=%d", action.object.c_str(),
             static_cast<i32>(target->current_hp));
}

void AutoplayRunner::tick_damage_character_action(const DamageCharacterAction &action) {
    if (!gameplay_ready(nullptr) || Player[0] == nullptr) {
        return;
    }

    GameObject_s *target = find_character(action.character.c_str());
    if (target == nullptr) {
        if (action.target_health == 0) {
            advance_action();
        }
        return;
    }
    if (target->current_hp <= action.target_health) {
        advance_action();
        return;
    }

    const f32 range = horizontal_distance(Player[0]->apiobj.position, target->apiobj.position);
    if (range > action.maximum_range) {
        finish(1, "named combat target moved out of range");
        return;
    }

    const u64 now = SDL_GetTicks();
    auto &state = this->runtime<CombatRuntime>();
    if (now - state.last_hit_at < 150) {
        return;
    }
    ObjHitObj(Player[0], target, 1, 0, 0, 1);
    state.last_hit_at = now;
    LOG_INFO("autoplay: applied native combat hit to %s at health=%d", action.character.c_str(),
             static_cast<i32>(target->current_hp));
}

void AutoplayRunner::tick_character_switch(const SwitchCharacterAction &action) {
    if (!gameplay_ready(nullptr)) {
        return;
    }

    const i32 character_id = CharIDFromName(const_cast<char *>(action.character.c_str()));
    if (character_id < 0) {
        finish(1, "named character type was not found");
        return;
    }
    if (Player[0]->id == character_id) {
        advance_action();
        return;
    }

    GameObject_s *target = find_character(action.character.c_str());
    if (target == nullptr) {
        finish(1, "named live character was not found");
        return;
    }
    auto &state = this->runtime<CharacterSwitchRuntime>();
    if (state.requested) {
        return;
    }
    if (static_cast<i8>(target->apiobj.flags_low) < 0 || (target->tag_context_flags & 2) != 0) {
        finish(1, "named character was not available for switching");
        return;
    }

    const f32 distance = horizontal_distance(Player[0]->apiobj.position, target->apiobj.position);
    const i32 result = TagCharacter(Player[0], target, 1);
    state.requested = true;
    LOG_INFO("autoplay %s: forced native character switch to %s at distance %.3f (result=%d)", this->level_name.c_str(),
             action.character.c_str(), distance, result);
    if (result == 0) {
        finish(1, "native forced character switch was rejected");
    } else if (Player[0] != nullptr && Player[0]->id == character_id) {
        advance_action();
    }
}

void AutoplayRunner::tick_hold_gizmo_action(const HoldGizmoAction &action) {
    if (!gameplay_ready(nullptr) || Player[0] == nullptr || action.return_speed <= 0.0f) {
        return;
    }

    const auto &[gizmo_name, output_index, expected] = action.condition;
    GIZMO *watched = find_gizmo(gizmo_name.c_str());
    if (watched == nullptr) {
        finish(1, "watched gizmo was not found while holding position");
        return;
    }
    if (GizmoGetOutput(WORLD->gizmo_sys, watched, output_index, 1) == expected) {
        advance_action();
        return;
    }
    this->enable_rail_run_speed(action.return_speed);

    const NUVEC &target = this->runtime<RailRuntime>().rail.waypoints.front().position;

    GameObject_s &character = *Player[0];
    const NUVEC &position = character.apiobj.position;
    const f32 dx = target.x - position.x;
    const f32 dz = target.z - position.z;
    const f32 distance = std::sqrt(dx * dx + dz * dz);
    constexpr f32 kHoldTolerance = 0.15f;

    HostInputSetHeld(0, 0);
    if (distance <= kHoldTolerance) {
        HostInputSetAnalog(0, 0.0f, 0.0f);
        return;
    }

    const i32 angle = dx * dx + dz * dz > 0.0001f ? NuAtan2D(dx, dz) : character.apiobj.facing_angle;
    this->set_rail_input(character, angle);
}

void AutoplayRunner::tick_hold_party_switches_action(const HoldPartySwitchesAction &action) {
    if (!gameplay_ready(nullptr)) {
        return;
    }

    const auto &[gizmo_name, output_index, expected] = action.condition;
    GIZMO *watched = find_gizmo(gizmo_name.c_str());
    if (watched == nullptr) {
        finish(1, "watched gizmo was not found while holding party switches");
        return;
    }
    if (GizmoGetOutput(WORLD->gizmo_sys, watched, output_index, 1) == expected) {
        advance_action();
        return;
    }

    const auto &positions = this->runtime<PartySwitchRuntime>().positions;
    for (usize index = 0; index < action.placements.size(); ++index) {
        const auto &[character_name, gizmo] = action.placements[index];
        static_cast<void>(gizmo);
        GameObject_s *character = find_character(character_name.c_str());
        if (character == nullptr || index >= positions.size()) {
            finish(1, "named party member was not found while holding party switches");
            return;
        }

        const NUVEC &switch_position = positions[index];
        const NUVEC &position = character->apiobj.position;
        if (std::hypot(position.x - switch_position.x, position.z - switch_position.z) > 0.1f ||
            std::abs(position.y - switch_position.y) > 0.25f) {
            NUVEC destination = switch_position;
            SnapCreaturePos(character, &destination, character->apiobj.facing_angle, nullptr, 1);
        }
    }
}

void AutoplayRunner::tick_hold_party_forces_action(const HoldPartyForcesAction &action) {
    if (!gameplay_ready(nullptr)) {
        return;
    }

    const auto &[gizmo_name, output_index, expected] = action.condition;
    GIZMO *watched = find_gizmo(gizmo_name.c_str());
    if (watched == nullptr) {
        finish(1, "watched gizmo was not found while holding party Force interactions");
        return;
    }
    if (GizmoGetOutput(WORLD->gizmo_sys, watched, output_index, 1) == expected) {
        advance_action();
        return;
    }

    for (const auto &[character_name, gizmo_name] : action.uses) {
        GameObject_s *character = find_character(character_name.c_str());
        GIZMO *gizmo = find_gizmo(gizmo_name.c_str());
        if (character == nullptr || gizmo == nullptr || gizmo->type_id != force_gizmotype_id ||
            gizmo->object == nullptr) {
            finish(1, "named character or Force gizmo was not found for party Force interaction");
            return;
        }

        GIZFORCE_s &force = *static_cast<GIZFORCE_s *>(gizmo->object);
        if (GizForce_Complete(&force) != 0) {
            continue;
        }
        if (!force_gizmo_usable_by(force, *character)) {
            finish(1, "named party member was not in range of its Force gizmo");
            return;
        }

        const u16 angle = GizForces_AngleToForce(&character->apiobj.position, &force);
        character->apiobj.facing_angle = angle;
        character->apiobj.movement_facing_angle = angle;
        character->apiobj.field_0x276 = angle;
        force.using_object = character;
    }
}
