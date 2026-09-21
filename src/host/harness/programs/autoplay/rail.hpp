#pragma once

// Deterministic movement along authored AI paths and explicit route segments.

bool AutoplayRunner::begin_relative_rail(const RailAction &action, const RailRelativeRoute &destination) {
    if (!gameplay_ready(nullptr) || destination.waypoints.empty() || action.speed <= 0.0f) {
        return false;
    }

    const NUVEC origin = Player[0]->apiobj.position;
    RailState &rail = this->runtime<RailRuntime>().rail;
    rail = RailState{};
    rail.waypoints.reserve(destination.waypoints.size());
    for (const NUVEC &offset : destination.waypoints) {
        rail.waypoints.push_back({{origin.x + offset.x, origin.y + offset.y, origin.z + offset.z}});
    }
    rail.character = Player[0];
    rail.starting_level = WORLD->current_level;
    LOG_INFO("autoplay: railing controlled character from (%.3f,%.3f,%.3f) through %u waypoint(s) at %.2fx run speed",
             origin.x, origin.y, origin.z, static_cast<unsigned>(rail.waypoints.size()), action.speed);
    return true;
}

bool AutoplayRunner::begin_forward_rail(const RailAction &action, const RailForward &destination) {
    if (!gameplay_ready(nullptr) || destination.distance <= 0.0f || action.speed <= 0.0f) {
        return false;
    }

    const NUVEC origin = Player[0]->apiobj.position;
    const f32 direction_x = NU_SIN_LUT(Player[0]->apiobj.facing_angle);
    const f32 direction_z = NU_COS_LUT(Player[0]->apiobj.facing_angle);
    RailState &rail = this->runtime<RailRuntime>().rail;
    rail = RailState{};
    rail.waypoints.push_back(
        {{origin.x + direction_x * destination.distance, origin.y, origin.z + direction_z * destination.distance}});
    rail.character = Player[0];
    rail.starting_level = WORLD->current_level;
    LOG_INFO("autoplay: railing controlled character %.2f units forward at %.2fx run speed", destination.distance,
             action.speed);
    return true;
}

bool AutoplayRunner::begin_target_rail(const RailAction &action) {
    if (!gameplay_ready(nullptr) || action.speed <= 0.0f) {
        return false;
    }

    GIZMO *gizmo = nullptr;
    NUVEC destination{};
    const std::string *target_name = nullptr;
    if (const auto *target_spec = std::get_if<RailToCharacter>(&action.destination)) {
        target_name = &target_spec->name;
        const GameObject_s *target = find_character(target_spec->name.c_str());
        if (target == nullptr) {
            return false;
        }
        destination = target->apiobj.position;
    } else if (const auto *target_spec = std::get_if<RailToArea>(&action.destination)) {
        target_name = &target_spec->name;
        const AIAREA_s *area = AISysFindArea(WORLD->ai_sys, const_cast<char *>(target_spec->name.c_str()));
        if (area == nullptr) {
            return false;
        }

        const NUVEC &origin = Player[0]->apiobj.position;
        const f32 sine = NU_SIN_LUT(area->rotation);
        const f32 cosine = NU_COS_LUT(area->rotation);
        const f32 offset_x = origin.x - area->position.x;
        const f32 offset_z = origin.z - area->position.z;
        const f32 local_x = offset_x * cosine + offset_z * sine;
        const f32 local_z = -offset_x * sine + offset_z * cosine;
        const f32 half_width = std::max(0.0f, area->half_width - 0.15f);
        const f32 half_depth = std::max(0.0f, area->half_depth - 0.15f);
        const f32 target_x = std::max(-half_width, std::min(half_width, local_x));
        const f32 target_z = std::max(-half_depth, std::min(half_depth, local_z));
        destination = {area->position.x + target_x * cosine - target_z * sine, area->position.y,
                       area->position.z + target_x * sine + target_z * cosine};
        LOG_INFO("autoplay: entering AI area %s at nearest point (%.3f,%.3f,%.3f)", target_spec->name.c_str(),
                 destination.x, destination.y, destination.z);
    } else if (const auto *target_spec = std::get_if<RailToPathNode>(&action.destination)) {
        target_name = &target_spec->name;
        if (WORLD->ai_sys->path_sys == nullptr || WORLD->ai_sys->path_sys->active_path == nullptr) {
            return false;
        }
        AIPATH *path = WORLD->ai_sys->path_sys->active_path;
        const AIPATHNODE *node = AIPathFindNode(WORLD->ai_sys, path, const_cast<char *>(target_spec->name.c_str()));
        if (node == nullptr) {
            LOG_ERR("autoplay: path node %s was not found", target_spec->name.c_str());
            return false;
        }
        destination = node->position;
    } else if (const auto *target_spec = std::get_if<RailToLocator>(&action.destination)) {
        target_name = &target_spec->name;
        const AILOCATOR *locator = AIPathFindLocator(WORLD->ai_sys, const_cast<char *>(target_spec->name.c_str()));
        if (locator == nullptr) {
            LOG_ERR("autoplay: AI locator %s was not found", target_spec->name.c_str());
            return false;
        }
        destination = locator->position;
    } else if (const auto *target_spec = std::get_if<RailThroughLocator>(&action.destination)) {
        target_name = &target_spec->name;
        const AILOCATOR *locator = AIPathFindLocator(WORLD->ai_sys, const_cast<char *>(target_spec->name.c_str()));
        if (locator == nullptr) {
            LOG_ERR("autoplay: AI locator %s was not found", target_spec->name.c_str());
            return false;
        }
        destination = locator->position;
    } else if (const auto *target_spec = std::get_if<RailToGizmo>(&action.destination)) {
        target_name = &target_spec->name;
        gizmo = find_gizmo(target_spec->name.c_str());
        auto position = gizmo_use_position(gizmo);
        if (!position) {
            return false;
        }
        destination = *position;
    } else {
        return false;
    }

    GameObject_s *character = Player[0];
    RailState &rail = this->runtime<RailRuntime>().rail;
    rail = RailState{};
    constexpr f32 kDirectApproachDistance = 2.0f;
    if (horizontal_distance(character->apiobj.position, destination) <= kDirectApproachDistance) {
        rail.waypoints.push_back({destination});
        LOG_INFO("autoplay: target is nearby; using a direct collision-aware approach");
    } else {
        auto route = build_forced_ai_route(character, destination);
        if (!route) {
            return false;
        }
        rail.waypoints = std::move(route->waypoints);
    }
    rail.character = character;
    rail.starting_level = WORLD->current_level;
    rail.stop_on_transition = std::holds_alternative<RailThroughLocator>(action.destination);
    if (gizmo != nullptr && gizmo->type_id == force_gizmotype_id && gizmo->object != nullptr) {
        rail.final_facing = GizForces_AngleToForce(&destination, static_cast<GIZFORCE_s *>(gizmo->object));
    }
    LOG_INFO("autoplay: railing current player to %s at %.2fx run speed", target_name->c_str(), action.speed);
    return true;
}

bool AutoplayRunner::begin_crossing_rail(const RailAction &action, const RailThroughGizmo &destination) {
    if (!gameplay_ready(nullptr) || action.speed <= 0.0f || destination.overshoot < 0.0f) {
        return false;
    }

    GIZMO *gizmo = find_gizmo(destination.name.c_str());
    auto crossing_position = gizmo_use_position(gizmo);
    if (!crossing_position) {
        return false;
    }
    NUVEC crossing = *crossing_position;

    const auto snapshot = this->gizmo_position_snapshots.find(destination.name);
    if (snapshot != this->gizmo_position_snapshots.end()) {
        crossing = snapshot->second;
    }

    RailState &rail = this->runtime<RailRuntime>().rail;
    rail = RailState{};
    const bool is_door = gizmo->object != nullptr && gizmotypes != nullptr &&
                         SDL_strcasecmp(gizmotypes->types[gizmo->type_id].name, "Door") == 0;
    if (is_door) {
        const DOOR_s &door = *static_cast<DOOR_s *>(gizmo->object);
        const f32 normal_length = std::sqrt(door.normal.x * door.normal.x + door.normal.z * door.normal.z);
        const NUVEC &origin = Player[0]->apiobj.position;
        const f32 approach_distance = std::max(door.radius + 0.25f, 0.75f);
        NUVEC approach{};
        NUVEC exit{};
        if (normal_length > 0.001f) {
            const f32 normal_x = door.normal.x / normal_length;
            const f32 normal_z = door.normal.z / normal_length;
            const f32 origin_side = (origin.x - crossing.x) * normal_x + (origin.z - crossing.z) * normal_z;
            const f32 side = origin_side >= 0.0f ? 1.0f : -1.0f;
            approach = {crossing.x + normal_x * side * approach_distance, crossing.y,
                        crossing.z + normal_z * side * approach_distance};
            exit = {crossing.x - normal_x * side * approach_distance, crossing.y,
                    crossing.z - normal_z * side * approach_distance};
        } else if (std::fabs(door.normal.y) > 0.001f && action.follow_height) {
            const f32 normal_y = door.normal.y > 0.0f ? 1.0f : -1.0f;
            const f32 origin_side = (origin.y - crossing.y) * normal_y;
            const f32 side = origin_side >= 0.0f ? 1.0f : -1.0f;
            approach = {crossing.x, crossing.y + normal_y * side * approach_distance, crossing.z};
            exit = {crossing.x, crossing.y - normal_y * side * approach_distance, crossing.z};
            rail.vertical_travel = true;
        } else {
            return false;
        }
        auto route = build_forced_ai_route(Player[0], approach);
        if (!route) {
            return false;
        }
        rail.waypoints = std::move(route->waypoints);
        rail.waypoints.push_back({crossing});
        rail.waypoints.push_back({exit});
        LOG_INFO("autoplay: explicit door-plane crossing for %s from (%.3f,%.3f,%.3f) to (%.3f,%.3f,%.3f)",
                 destination.name.c_str(), approach.x, approach.y, approach.z, exit.x, exit.y, exit.z);
    } else {
        auto crossing_points = build_authored_crossing_route(Player[0]->apiobj.position, crossing);
        if (!crossing_points) {
            LOG_ERR("autoplay: no authored AI crossing route near %s", destination.name.c_str());
            return false;
        }
        auto route = build_forced_ai_route(Player[0], crossing_points->front());
        if (!route) {
            return false;
        }
        rail.waypoints = std::move(route->waypoints);
        for (auto point = crossing_points->begin() + 1; point != crossing_points->end(); ++point) {
            rail.waypoints.push_back({*point});
        }
    }
    rail.character = Player[0];
    rail.starting_level = WORLD->current_level;
    rail.stop_on_transition = true;
    const NUVEC &before_exit = rail.waypoints[rail.waypoints.size() - 2].position;
    const NUVEC &exit = rail.waypoints.back().position;
    const NUVEC exit_direction{exit.x - before_exit.x, exit.y - before_exit.y, exit.z - before_exit.z};
    if (destination.overshoot > 0.0f) {
        const f32 direction_length =
            rail.vertical_travel ? distance_3d(NUVEC{}, exit_direction) : horizontal_distance(NUVEC{}, exit_direction);
        if (direction_length > 0.001f) {
            rail.waypoints.push_back({{exit.x + exit_direction.x * destination.overshoot / direction_length,
                                       exit.y + exit_direction.y * destination.overshoot / direction_length,
                                       exit.z + exit_direction.z * destination.overshoot / direction_length}});
        }
    }
    LOG_INFO("autoplay: railing controlled character through %s's authored crossing at %.2fx run speed",
             destination.name.c_str(), action.speed);
    return true;
}

bool AutoplayRunner::begin_path_connection_rail(const RailAction &action,
                                                const RailThroughPathConnection &destination) {
    if (!gameplay_ready(nullptr) || action.speed <= 0.0f || WORLD->ai_sys == nullptr ||
        WORLD->ai_sys->path_sys == nullptr || WORLD->ai_sys->path_sys->active_path == nullptr) {
        return false;
    }

    AIPATH *path = WORLD->ai_sys->path_sys->active_path;
    AIPATHNODE *from = AIPathFindNode(WORLD->ai_sys, path, const_cast<char *>(destination.from.c_str()));
    AIPATHNODE *to = AIPathFindNode(WORLD->ai_sys, path, const_cast<char *>(destination.to.c_str()));
    if (from == nullptr || to == nullptr) {
        LOG_ERR("autoplay: path connection %s -> %s was not found", destination.from.c_str(), destination.to.c_str());
        return false;
    }

    AIPathNodeUpdatePos(WORLD->ai_sys, path, from);
    AIPathNodeUpdatePos(WORLD->ai_sys, path, to);

    const i32 from_index = static_cast<i32>(from - path->nodes);
    const i32 to_index = static_cast<i32>(to - path->nodes);
    AIPATHCNX *selected_connection = nullptr;
    i32 direction = 0;
    for (i32 index = 0; index < from->connection_count; ++index) {
        AIPATHCNX *connection = from->connections[index];
        if (connection != nullptr &&
            (connection->node_indices[0] == to_index || connection->node_indices[1] == to_index)) {
            selected_connection = connection;
            direction = connection->node_indices[0] == from_index ? 0 : 1;
            break;
        }
    }
    if (selected_connection == nullptr) {
        LOG_ERR("autoplay: path nodes %s and %s are not directly connected", destination.from.c_str(),
                destination.to.c_str());
        return false;
    }

    RailState &rail = this->runtime<RailRuntime>().rail;
    rail = RailState{};
    auto route = build_forced_ai_route(Player[0], from->position);
    if (!route || !route->authored) {
        LOG_ERR("autoplay: no authored route to path node %s", destination.from.c_str());
        return false;
    }
    rail.waypoints = std::move(route->waypoints);
    rail.waypoints.push_back({to->position, selected_connection, direction});
    rail.character = Player[0];
    rail.starting_level = WORLD->current_level;
    LOG_INFO("autoplay: railing through authored path connection %s (%.3f,%.3f,%.3f) -> %s "
             "(%.3f,%.3f,%.3f), flags=0x%08x at %.2fx run speed",
             destination.from.c_str(), from->position.x, from->position.y, from->position.z, destination.to.c_str(),
             to->position.x, to->position.y, to->position.z, selected_connection->traversal_flags[direction],
             action.speed);
    return true;
}

bool AutoplayRunner::begin_native_jump(const NativeJumpAction &action) {
    if (!gameplay_ready(nullptr)) {
        return false;
    }

    NUVEC destination{};
    std::string_view target_name;
    if (const auto *target = std::get_if<JumpRelative>(&action.destination)) {
        const NUVEC &origin = Player[0]->apiobj.position;
        destination = {origin.x + target->offset.x, origin.y + target->offset.y, origin.z + target->offset.z};
    } else if (const auto *target = std::get_if<JumpToLocator>(&action.destination)) {
        target_name = target->name;
        const AILOCATOR *locator = AIPathFindLocator(WORLD->ai_sys, const_cast<char *>(target->name.c_str()));
        if (locator == nullptr) {
            LOG_ERR("autoplay: jump locator %s was not found", target->name.c_str());
            return false;
        }
        destination = locator->position;
    } else {
        const auto &gizmo_target = std::get<JumpToGizmo>(action.destination);
        target_name = gizmo_target.name;
        auto position = gizmo_use_position(find_gizmo(gizmo_target.name.c_str()));
        if (!position) {
            return false;
        }
        destination = *position;
    }

    RailState &rail = this->runtime<RailRuntime>().rail;
    rail = RailState{};
    rail.character = Player[0];
    rail.starting_level = WORLD->current_level;
    rail.waypoints.push_back({destination});
    rail.native_traversal = NativeTraversal{std::min(Player[0]->apiobj.position.y, destination.y) - 0.75f};
    if (StartBigJump(Player[0], &destination, 0, 0.5f, 1.0f, 0, 0) == 0) {
        return false;
    }
    LOG_INFO("autoplay: started native jump%s%.*s at (%.3f,%.3f,%.3f)", target_name.empty() ? "" : " to ",
             static_cast<i32>(target_name.size()), target_name.data(), destination.x, destination.y, destination.z);
    return true;
}

void AutoplayRunner::set_rail_input(const GameObject_s &character, i32 world_angle) {
    i32 input_reference_angle = GameCam != nullptr ? GameCam->input_yaw : 0;
    if (static_cast<i8>(character.apiobj.field_0x1f8) < 0 && character.field_0x661 != 0xff && WORLD != nullptr &&
        WORLD->sock_sys != nullptr) {
        const SOCK &socket = WORLD->sock_sys->sock[static_cast<i8>(character.field_0x661)];
        if ((socket.flags & 0x40) != 0) {
            input_reference_angle = character.yrot + socket.input_yaw;
        }
    }

    const i32 input_angle = world_angle - input_reference_angle;
    const f32 input_x = NU_SIN_LUT(input_angle);
    const f32 input_z = NU_COS_LUT(input_angle);
    const f32 input_y = WORLD != nullptr && WORLD->current_level == DOGFIGHTA_LDATA ? input_z : -input_z;

    // Native character movement uses the largest stick axis as its magnitude,
    // so a unit-circle vector would drop to walking speed near diagonals.
    // Scale to the square gate used by that code while preserving direction.
    const f32 largest_axis = std::max(std::abs(input_x), std::abs(input_y));
    HostInputSetAnalog(0, input_x / largest_axis, input_y / largest_axis);
}

void AutoplayRunner::tick_rail_action(const RailAction &action) {
    this->tick_rail_state(action.speed, std::cref(action.destination));
}

void AutoplayRunner::tick_native_jump_action() {
    this->tick_rail_state(1.0f, std::nullopt);
}

void AutoplayRunner::tick_rail_state(f32 speed,
                                     std::optional<std::reference_wrapper<const RailDestination>> destination) {
    RailState &rail = this->runtime<RailRuntime>().rail;
    const bool level_changed =
        WORLD != nullptr && rail.starting_level != nullptr && WORLD->current_level != rail.starting_level;
    if (level_changed) {
        if (rail.stop_on_transition) {
            LOG_INFO("autoplay: rail observed an area transition");
            this->advance_action();
        } else {
            this->finish(1, "rail unexpectedly crossed a level boundary");
        }
        return;
    }

    GameObject_s *character = rail.character;
    if (character == nullptr) {
        this->finish(1, "rail lost its controlled character");
        return;
    }

    this->cancel_incidental_push(*character);

    if (destination) {
        const auto *target_spec = std::get_if<RailToCharacter>(&destination->get());
        if (target_spec != nullptr) {
            const GameObject_s *target = find_character(target_spec->name.c_str());
            if (target == nullptr) {
                finish(1, "rail lost its target character");
                return;
            }
            if (horizontal_distance(character->apiobj.position, target->apiobj.position) <=
                target_spec->arrival_tolerance) {
                advance_action();
                return;
            }
        }
    }

    const bool cutscene_started = CUTSTOPGAME != 0 || CutStopInfo != nullptr;
    if (rail.stop_on_transition && (NewLData != nullptr || cutscene_started)) {
        LOG_INFO("autoplay: rail observed an area transition");
        this->advance_action();
        return;
    }
    if (!gameplay_ready(nullptr)) {
        return;
    }

    if (rail.native_traversal) {
        this->restore_run_speed();
        HostInputSetAnalog(0, 0.0f, 0.0f);
        const NUVEC &destination = rail.waypoints[rail.waypoint_index].position;
        if (character->apiobj.field_0x27d == 0) {
            rail.native_traversal->airborne = true;
        }
        if (character->apiobj.position.y < rail.native_traversal->minimum_height) {
            finish(1, "native traversal fell below its authored route");
            return;
        }
        if (rail.native_traversal->airborne && horizontal_distance(character->apiobj.position, destination) <= 0.5f &&
            character->apiobj.field_0x27d != 0) {
            ++rail.waypoint_index;
            rail.native_traversal.reset();
            if (rail.waypoint_index == rail.waypoints.size()) {
                advance_action();
            }
        }
        return;
    }
    this->enable_rail_run_speed(speed);

    if (rail.waypoint_index >= rail.waypoints.size()) {
        if (rail.stop_on_transition) {
            HostInputSetAnalog(0, 0.0f, 0.0f);
            return;
        }
        advance_action();
        return;
    }

    const NUVEC &route_position = character->apiobj.position;
    const RailWaypoint &waypoint = rail.waypoints[rail.waypoint_index];
    const NUVEC target = waypoint.position;
    const f32 target_dx = target.x - route_position.x;
    const bool reached_last_waypoint = rail.waypoint_index + 1 == rail.waypoints.size();
    const bool validate_destination_height = reached_last_waypoint && destination &&
                                             (std::holds_alternative<RailToPathNode>(destination->get()) ||
                                              std::holds_alternative<RailToLocator>(destination->get()) ||
                                              std::holds_alternative<RailThroughLocator>(destination->get()) ||
                                              std::holds_alternative<RailThroughPathConnection>(destination->get()));
    const f32 target_dy = rail.vertical_travel || validate_destination_height ? target.y - route_position.y : 0.0f;
    const f32 target_dz = target.z - route_position.z;
    const f32 distance = std::sqrt(target_dx * target_dx + target_dy * target_dy + target_dz * target_dz);
    // Intermediate graph nodes are frequently placed close to wall corners.
    // A wide radius cuts those corners, particularly at smoke-test speed, and
    // leaves the character continuously steering into the wall.
    constexpr f32 kPathWaypointTolerance = 0.2f;
    constexpr f32 kDestinationTolerance = 0.75f;
    constexpr f32 kGizmoDestinationTolerance = 0.85f;
    const bool targets_named_gizmo = destination && std::holds_alternative<RailToGizmo>(destination->get());
    const f32 arrival_tolerance = reached_last_waypoint
                                      ? (targets_named_gizmo ? kGizmoDestinationTolerance : kDestinationTolerance)
                                      : kPathWaypointTolerance;

    if (waypoint.incoming_connection != nullptr) {
        const u32 flags = waypoint.incoming_connection->traversal_flags[waypoint.traversal_direction];
        const u32 jump_capabilities = static_cast<u32>(LEGO_AIPATHCNX_JUMP | LEGO_AIPATHCNX_DOUBLE_JUMP |
                                                       LEGO_AIPATHCNX_HIGH_JUMP | LEGO_AIPATHCNX_BIGJUMP);
        const bool authored_jump = (flags & character->ai.capabilities & jump_capabilities) != 0;
        const bool raised_destination = target.y - route_position.y > 0.35f;
        if (authored_jump || raised_destination) {
            this->restore_run_speed();
            NUVEC jump_destination = target;
            const i8 jump_variant = (flags & static_cast<u32>(LEGO_AIPATHCNX_DOUBLE_JUMP)) != 0 ? 2 : 0;
            if (StartBigJump(character, &jump_destination, 0, 0.5f, 1.0f, 0, jump_variant) == 0) {
                finish(1, "could not start native path-connection jump");
                return;
            }
            rail.native_traversal = NativeTraversal{std::min(route_position.y, target.y) - 0.75f};
            HostInputSetAnalog(0, 0.0f, 0.0f);
            LOG_INFO("autoplay: started native jump across path connection (flags=0x%08x)", flags);
            return;
        }
    }
    bool targets_force_gizmo = false;
    bool targets_obstacle_gizmo = false;
    bool reached_usable_force = false;
    bool reached_active_obstacle = false;
    if (reached_last_waypoint && targets_named_gizmo) {
        const auto &target_spec = std::get<RailToGizmo>(destination->get());
        GIZMO *gizmo = find_gizmo(target_spec.name.c_str());
        targets_force_gizmo = gizmo != nullptr && gizmo->type_id == force_gizmotype_id && gizmo->object != nullptr;
        targets_obstacle_gizmo =
            gizmo != nullptr && gizmo->type_id == obstacle_gizmotype_id && gizmo->object != nullptr;
        reached_usable_force =
            targets_force_gizmo && force_gizmo_usable_by(*static_cast<GIZFORCE_s *>(gizmo->object), *character);
        reached_active_obstacle =
            targets_obstacle_gizmo && GizmoGetOutput(WORLD->gizmo_sys, gizmo, GIZOBSTACLE_OUTPUT_PROXIMITY, 1) != 0;
    }
    if ((targets_force_gizmo && reached_usable_force) || reached_active_obstacle ||
        (!targets_force_gizmo && distance <= arrival_tolerance)) {
        if (reached_last_waypoint && rail.final_facing) {
            character->apiobj.facing_angle = *rail.final_facing;
            character->apiobj.movement_facing_angle = *rail.final_facing;
            character->apiobj.field_0x276 = *rail.final_facing;
        }
        LOG_INFO("autoplay: railed controlled character through waypoint %u at (%.3f,%.3f,%.3f)",
                 static_cast<unsigned>(rail.waypoint_index), route_position.x, route_position.y, route_position.z);
        ++rail.waypoint_index;
        rail.progress_observed_at.reset();
        return;
    }

    // A rail supplies steering only. Native character movement owns position,
    // terrain collision, gravity, animation, slopes, and door/trigger checks.
    // For a vertical waypoint (for example, a drop through a floor portal),
    // stop steering once horizontally aligned and let gravity do the rest.
    const f32 horizontal_distance_to_target = std::sqrt(target_dx * target_dx + target_dz * target_dz);
    if (horizontal_distance_to_target <= arrival_tolerance) {
        HostInputSetAnalog(0, 0.0f, 0.0f);
        return;
    }

    constexpr f32 kMinimumProgress = 0.15f;
    constexpr f32 kCollisionLimitedWaypointTolerance = 1.0f;
    constexpr u64 kCollisionLimitedWaypointTimeoutMs = 500;
    // Short scripted transitions and landing animations can legitimately
    // suppress movement for around a second.  Two seconds still prevents a
    // latched push state without mistaking those pauses for blocked geometry.
    constexpr u64 kStallTimeoutMs = 2000;
    const u64 now = SDL_GetTicks();
    if (!rail.progress_observed_at || horizontal_distance(route_position, rail.progress_origin) >= kMinimumProgress) {
        rail.progress_origin = route_position;
        rail.progress_observed_at = now;
    } else if ((!reached_last_waypoint || targets_obstacle_gizmo) &&
               horizontal_distance_to_target <= kCollisionLimitedWaypointTolerance &&
               now - *rail.progress_observed_at >= kCollisionLimitedWaypointTimeoutMs) {
        HostInputSetAnalog(0, 0.0f, 0.0f);
        this->restore_run_speed();
        LOG_WARN("autoplay: accepting collision-limited waypoint %u/%u at %.3f units",
                 static_cast<unsigned>(rail.waypoint_index + 1), static_cast<unsigned>(rail.waypoints.size()),
                 horizontal_distance_to_target);
        ++rail.waypoint_index;
        rail.progress_observed_at.reset();
        return;
    } else if (now - *rail.progress_observed_at >= kStallTimeoutMs) {
        HostInputSetAnalog(0, 0.0f, 0.0f);
        this->restore_run_speed();
        LOG_ERR("autoplay: rail stalled before waypoint %u/%u; current=(%.3f,%.3f,%.3f), "
                "target=(%.3f,%.3f,%.3f)",
                static_cast<unsigned>(rail.waypoint_index + 1), static_cast<unsigned>(rail.waypoints.size()),
                route_position.x, route_position.y, route_position.z, target.x, target.y, target.z);
        finish(1, "rail made no horizontal progress");
        return;
    }

    this->set_rail_input(*character, NuAtan2D(target_dx, target_dz));
}
