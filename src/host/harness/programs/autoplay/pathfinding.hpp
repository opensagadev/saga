#pragma once

// Route construction over the level's authored AI graph.

f32 horizontal_distance_squared(const NUVEC &a, const NUVEC &b) {
    const f32 x = a.x - b.x;
    const f32 z = a.z - b.z;
    return x * x + z * z;
}

f32 horizontal_distance(const NUVEC &a, const NUVEC &b) {
    return std::sqrt(horizontal_distance_squared(a, b));
}

f32 distance_3d(const NUVEC &a, const NUVEC &b) {
    const f32 x = a.x - b.x;
    const f32 y = a.y - b.y;
    const f32 z = a.z - b.z;
    return std::sqrt(x * x + y * y + z * z);
}

std::optional<std::vector<NUVEC>> build_authored_crossing_route(const NUVEC &origin, const NUVEC &crossing_hint) {
    if (WORLD == nullptr || WORLD->ai_sys == nullptr || WORLD->ai_sys->path_sys == nullptr) {
        return std::nullopt;
    }
    AIPATH *best_path = nullptr;
    i32 best_node_index = -1;
    f32 best_distance = 1.5f;
    AIPATHSYS *path_system = WORLD->ai_sys->path_sys;
    for (i32 path_index = 0; path_index < path_system->path_count; ++path_index) {
        AIPATH *path = path_system->paths[path_index];
        if (path == nullptr) {
            continue;
        }
        for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
            const f32 distance = horizontal_distance(crossing_hint, path->nodes[node_index].position);
            if (distance < best_distance) {
                best_distance = distance;
                best_path = path;
                best_node_index = node_index;
            }
        }
    }
    if (best_path == nullptr || best_node_index < 0) {
        return std::nullopt;
    }

    const AIPATHNODE &crossing_node = best_path->nodes[best_node_index];
    i32 exit_node_index = -1;
    f32 exit_distance_from_origin = -1.0f;
    for (i32 connection_index = 0; connection_index < crossing_node.connection_count; ++connection_index) {
        const AIPATHCNX *connection = crossing_node.connections[connection_index];
        if (connection == nullptr) {
            continue;
        }
        const i32 other =
            connection->node_indices[0] == best_node_index ? connection->node_indices[1] : connection->node_indices[0];
        if (other < 0 || other >= best_path->node_count) {
            continue;
        }
        const f32 distance = horizontal_distance(origin, best_path->nodes[other].position);
        if (distance > exit_distance_from_origin) {
            exit_distance_from_origin = distance;
            exit_node_index = other;
        }
    }
    if (exit_node_index < 0) {
        return std::nullopt;
    }

    const NUVEC crossing = crossing_node.position;
    const NUVEC exit = best_path->nodes[exit_node_index].position;
    std::vector<NUVEC> points{crossing, exit};
    LOG_INFO("autoplay: authored crossing on %s node %d -> %d, hint-distance=%.3f, from "
             "(%.3f,%.3f,%.3f) to (%.3f,%.3f,%.3f)",
             best_path->name, best_node_index, exit_node_index, best_distance, crossing.x, crossing.y, crossing.z,
             exit.x, exit.y, exit.z);
    return points;
}

bool finite_position(const NUVEC &position) {
    return std::isfinite(position.x) && std::isfinite(position.y) && std::isfinite(position.z);
}

struct ForcedRoute {
    std::vector<RailWaypoint> waypoints;
    bool authored = false;
};

std::optional<ForcedRoute> build_forced_ai_route(GameObject_s *character, const NUVEC &target) {
    if (character == nullptr || WORLD == nullptr || WORLD->ai_sys == nullptr || !finite_position(target)) {
        return std::nullopt;
    }
    NUVEC start = character->apiobj.position;
    NUVEC mutable_target = target;
    AIPATHINFO start_info{};
    AIPATHINFO target_info{};
    NUVEC projected_start{};
    NUVEC projected_target{};
    AISysGetPathPos2(WORLD->ai_sys, &start, &start_info, &projected_start, nullptr, 0xff);
    AISysGetPathPos2(WORLD->ai_sys, &mutable_target, &target_info, &projected_target, nullptr, 0xff);

    ForcedRoute route;
    if (start_info.connection == nullptr || target_info.connection == nullptr || start_info.path == nullptr ||
        start_info.path != target_info.path || start_info.path->node_count == 0) {
        route.waypoints.push_back({target});
        LOG_WARN("autoplay: no shared AI path; using direct forced rail from (%.3f,%.3f,%.3f) to "
                 "(%.3f,%.3f,%.3f)",
                 start.x, start.y, start.z, target.x, target.y, target.z);
        return route;
    }

    AIPATH *path = start_info.path;
    const i32 node_count = path->node_count;
    std::vector<f32> distances(node_count, std::numeric_limits<f32>::max());
    std::vector<i32> previous(node_count, -1);
    std::vector<u8> visited(node_count, false);
    for (i32 endpoint = 0; endpoint < 2; ++endpoint) {
        const i32 node = start_info.connection->node_indices[endpoint];
        if (node >= 0 && node < node_count) {
            distances[node] = horizontal_distance(projected_start, path->nodes[node].position);
        }
    }
    for (i32 iteration = 0; iteration < node_count; ++iteration) {
        i32 current = -1;
        f32 best = std::numeric_limits<f32>::max();
        for (i32 node = 0; node < node_count; ++node) {
            if (!visited[node] && distances[node] < best) {
                current = node;
                best = distances[node];
            }
        }
        if (current < 0) {
            break;
        }
        visited[current] = true;
        AIPATHNODE &node = path->nodes[current];
        for (i32 index = 0; index < node.connection_count; ++index) {
            AIPATHCNX *connection = node.connections[index];
            if (connection == nullptr) {
                continue;
            }
            const i32 next = connection->node_indices[0] == current   ? connection->node_indices[1]
                             : connection->node_indices[1] == current ? connection->node_indices[0]
                                                                      : -1;
            if (next < 0 || next >= node_count) {
                continue;
            }
            const i32 direction = connection->node_indices[0] == current ? 0 : 1;
            const u32 traversal_flags = connection->traversal_flags[direction];
            const u32 unavailable_flags =
                AIPATH_CONNECTION_FLAG_DYNAMIC_TOO_LONG | AIPATH_CONNECTION_FLAG_SPECIAL_UNAVAILABLE;
            const u32 required_capabilities =
                (traversal_flags & AIPATH_CONNECTION_CAPABILITY_MASK) & ~AIPATH_CONNECTION_FLAG_NO_CAPABILITY_REQUIRED;
            if ((traversal_flags & unavailable_flags) != 0 ||
                (required_capabilities != 0 && (character->ai.capabilities & required_capabilities) == 0)) {
                continue;
            }
            const f32 edge = connection->horizontal_distance > 0.0f
                                 ? connection->horizontal_distance
                                 : horizontal_distance(node.position, path->nodes[next].position);
            if (distances[current] + edge < distances[next]) {
                distances[next] = distances[current] + edge;
                previous[next] = current;
            }
        }
    }

    i32 goal = -1;
    f32 goal_distance = std::numeric_limits<f32>::max();
    for (i32 endpoint = 0; endpoint < 2; ++endpoint) {
        const i32 node = target_info.connection->node_indices[endpoint];
        if (node < 0 || node >= node_count || distances[node] == std::numeric_limits<f32>::max()) {
            continue;
        }
        const f32 candidate = distances[node] + horizontal_distance(path->nodes[node].position, projected_target);
        if (candidate < goal_distance) {
            goal = node;
            goal_distance = candidate;
        }
    }
    if (goal < 0) {
        route.waypoints.push_back({target});
        LOG_WARN("autoplay: authored AI graph has no route; using direct forced rail");
        return route;
    }

    std::vector<i32> reversed;
    for (i32 node = goal; node >= 0 && reversed.size() < static_cast<usize>(node_count); node = previous[node]) {
        reversed.push_back(node);
    }
    for (auto index = reversed.rbegin(); index != reversed.rend(); ++index) {
        const i32 node_index = *index;
        const NUVEC &point = path->nodes[node_index].position;
        if (finite_position(point) && horizontal_distance(point, start) > 0.25f) {
            RailWaypoint waypoint{point};
            const i32 previous_node = previous[node_index];
            if (previous_node >= 0) {
                AIPATHNODE &source = path->nodes[previous_node];
                for (i32 connection_index = 0; connection_index < source.connection_count; ++connection_index) {
                    AIPATHCNX *connection = source.connections[connection_index];
                    if (connection == nullptr) {
                        continue;
                    }
                    const i32 direction = connection->node_indices[0] == previous_node ? 0 : 1;
                    const i32 destination_index = connection->node_indices[1 - direction];
                    if (destination_index == node_index) {
                        waypoint.incoming_connection = connection;
                        waypoint.traversal_direction = direction;
                        break;
                    }
                }
            }
            route.waypoints.push_back(waypoint);
        }
    }
    route.waypoints.push_back({target});
    route.authored = true;
    LOG_INFO("autoplay: forced AI rail on %s from (%.3f,%.3f,%.3f) to (%.3f,%.3f,%.3f), %u waypoints", path->name,
             start.x, start.y, start.z, target.x, target.y, target.z, static_cast<unsigned>(route.waypoints.size()));
    return route;
}
