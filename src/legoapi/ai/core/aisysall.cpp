#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"

#include <float.h>
#include <math.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

static u32 CalculateRightIntersection(APIOBJECT *object, AIPATHCNX *first_connection,
                                      AIPATHCNX *second_connection, i32 first_angle, i32 second_angle,
                                      AIPATHNODE *before, AIPATHNODE *shared, AIPATHNODE *after, NUVEC *result) {
    f32 clearance = object->collision_radius + 0.05f;
    f32 before_radius = before->radius - clearance;
    if (before_radius < 0.0f) {
        before_radius = 0.0f;
    }
    f32 shared_radius = shared->radius - clearance;
    if (shared_radius < 0.0f) {
        shared_radius = 0.0f;
    }
    f32 after_radius = after->radius - clearance;
    if (after_radius < 0.0f) {
        after_radius = 0.0f;
    }
    NUVEC first_start = {before_radius, 0.0f, 0.0f};
    NUVEC first_end = {before_radius, 0.0f, first_connection->horizontal_distance};
    NUANG angle = NuAngAdd(first_angle, NuASin((shared_radius - before_radius) /
                                               first_connection->horizontal_distance));
    NuVecRotateY(&first_start, &first_start, angle);
    NuVecRotateY(&first_end, &first_end, angle);
    NuVecAdd(&first_start, &first_start, &before->position);
    NuVecAdd(&first_end, &first_end, &before->position);

    NUVEC second_start = {-after_radius, 0.0f, 0.0f};
    NUVEC second_end = {-after_radius, 0.0f, second_connection->horizontal_distance};
    angle = NuAngSub(second_angle, NuASin((shared_radius - after_radius) /
                                         second_connection->horizontal_distance));
    NuVecRotateY(&second_start, &second_start, angle);
    NuVecRotateY(&second_end, &second_end, angle);
    NuVecAdd(&second_start, &second_start, &after->position);
    NuVecAdd(&second_end, &second_end, &after->position);

    NUVEC first_origin = first_start;
    NUVEC first_direction;
    NuVecSub(&first_direction, &first_end, &first_origin);
    NUVEC second_origin = second_start;
    NUVEC second_direction;
    NuVecSub(&second_direction, &second_end, &second_origin);
    f32 first_parameter;
    f32 second_parameter;
    if (NuLineLineIntersect(&first_origin, &first_direction, &second_origin, &second_direction,
                            &first_parameter, &second_parameter) != 0 &&
        first_parameter >= 0.0f && second_parameter >= 0.0f &&
        first_parameter <= 1.0f && second_parameter <= 1.0f) {
        first_origin.y = shared->position.y * first_parameter + before->position.y * (1.0f - first_parameter);
        NuVecScale(&first_direction, &first_direction, first_parameter);
        NuVecAdd(result, &first_origin, &first_direction);
        return 1;
    }
    return 0;
}

static u32 CalculateIntersection(AISYS *system, AIPACKET *packet, APIOBJECT *object,
                                 AIPATHCNX *current_connection, AIPATHCNX *target_connection) {
    if (packet->intersection_connection != current_connection ||
        packet->intersection_target_connection != target_connection) {
        packet->movement_flags &=
            static_cast<u8>(~(AIPACKET_MOVEMENT_DIVERSION_RIGHT | AIPACKET_MOVEMENT_DIVERSION_LEFT));
        packet->intersection_connection = current_connection;
        packet->intersection_target_connection = target_connection;

        AIPATH *path = packet->path_info.path;
        AIPATHNODE *before;
        AIPATHNODE *shared;
        AIPATHNODE *after;
        i32 first_angle;
        i32 second_angle;
        if (current_connection->node_indices[0] == target_connection->node_indices[0]) {
            before = &path->nodes[current_connection->node_indices[1]];
            shared = &path->nodes[current_connection->node_indices[0]];
            after = &path->nodes[target_connection->node_indices[1]];
            first_angle = NuAngAdd(current_connection->rotation, NUANG_180DEG);
            second_angle = NuAngAdd(target_connection->rotation, NUANG_180DEG);
        } else if (current_connection->node_indices[0] == target_connection->node_indices[1]) {
            before = &path->nodes[current_connection->node_indices[1]];
            shared = &path->nodes[current_connection->node_indices[0]];
            after = &path->nodes[target_connection->node_indices[0]];
            first_angle = NuAngAdd(current_connection->rotation, NUANG_180DEG);
            second_angle = target_connection->rotation;
        } else if (current_connection->node_indices[1] == target_connection->node_indices[0]) {
            before = &path->nodes[current_connection->node_indices[0]];
            shared = &path->nodes[current_connection->node_indices[1]];
            after = &path->nodes[target_connection->node_indices[1]];
            first_angle = current_connection->rotation;
            second_angle = NuAngAdd(target_connection->rotation, NUANG_180DEG);
        } else if (current_connection->node_indices[1] == target_connection->node_indices[1]) {
            before = &path->nodes[current_connection->node_indices[0]];
            shared = &path->nodes[current_connection->node_indices[1]];
            after = &path->nodes[target_connection->node_indices[0]];
            first_angle = current_connection->rotation;
            second_angle = target_connection->rotation;
        } else {
            return 0;
        }
        if (WithinConnection(system, &after->position, path, current_connection, 1, current_connection,
                             packet->current_route, object->field_0x289, NULL, 0.0f, 1) != 0) {
            return 0;
        }
        u32 right = CalculateRightIntersection(object, current_connection, target_connection,
                                               first_angle, second_angle, before, shared, after,
                                               &packet->right_diversion);
        packet->movement_flags = (packet->movement_flags & ~AIPACKET_MOVEMENT_DIVERSION_RIGHT) | ((right & 1) << 5);
        u32 left = CalculateRightIntersection(object, target_connection, current_connection,
                                              second_angle, first_angle, after, shared, before,
                                              &packet->left_diversion);
        packet->movement_flags = (packet->movement_flags & ~AIPACKET_MOVEMENT_DIVERSION_LEFT) | ((left & 1) << 6);
    }
    return ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) |
           ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0);
}

void AISysGetPathPos2(AISYS_s *, nuvec_s *, AIPATHINFO_s *, nuvec_s *, AIPATH_s *, i32) {
}

i32 AIMoveCanReachPath(AISYS_s *system, AIPATH_s *path, AIPATH_s *destination) {
    AIPATHNODELINK *links = path->special_routes;
    for (i32 link_index = 0; link_index < path->special_route_count; ++link_index) {
        AIPATHSPECIALROUTE *route =
            &system->path_sys->special_routes[links[link_index].special_route_index];
        // The original increments the outer cursor here and recursively tests
        // the same path pair. Preserve those accesses, including path slot zero.
        for (i32 path_index = 0; path_index < route->path_count; ++link_index) {
            if (route->paths[path_index] == destination) {
                return 1;
            }
            if (AIMoveCanReachPath(system, path, destination) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void AIMoveToDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    AIPATH *path = packet->path_info.path;
    AIPATHCNX *connection = packet->path_info.connection;
    AIPATHINFO &destination_path_info = packet->fallback_path_info;

    // The target asks AIMoveFindDivertNode for a cross-path transition and
    // stops at the current position when none is available. Never substitute a
    // straight line to a destination on an unrelated path graph.
    packet->movement_destination = object->position;
    packet->movement_stopping_distance = 0.0f;
    if (path == NULL || path != destination_path_info.path || connection == NULL ||
        destination_path_info.connection == NULL || path->nodes == NULL) {
        packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED;
        return;
    }

    AIPATHCNX *destination_connection = destination_path_info.connection;
    if (connection == destination_connection) {
        packet->movement_destination = packet->fallback_destination;
        packet->movement_stopping_distance = packet->fallback_stopping_distance;
        packet->goal_path_node = NULL;
        packet->runtime_flags &=
            static_cast<u8>(~(AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT));
        return;
    }

    // The target first adopts the destination connection when the character's
    // terrain origin is already inside its corridor. Its direction is chosen
    // from the endpoint shared with the current connection.
    if (WithinConnection(system, &packet->terrain_origin, path, destination_connection, checks, connection,
                         packet->current_route, object->field_0x289, NULL, object->collision_radius, 0) != 0) {
        const u8 destination_node_a = destination_connection->node_indices[0];
        const i32 destination_direction =
            destination_node_a == connection->node_indices[0] || destination_node_a == connection->node_indices[1] ? 0
                                                                                                                   : 1;
        AISysCharacterSetPathCnx(packet, &object->position, destination_connection, destination_direction);
        if (packet->path_info.connection == destination_connection) {
            packet->movement_event_flags |= AIPACKET_PATH_CONNECTION_CHANGED;
            packet->movement_destination = packet->fallback_destination;
            packet->movement_stopping_distance = packet->fallback_stopping_distance;
            packet->goal_path_node = NULL;
            packet->runtime_flags &=
                static_cast<u8>(~(AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT));
            return;
        }
    }

    const u8 current_node_a = connection->node_indices[0];
    const u8 current_node_b = connection->node_indices[1];
    const u8 destination_node_a = destination_connection->node_indices[0];
    const u8 destination_node_b = destination_connection->node_indices[1];
    if (current_node_a >= path->node_count || current_node_b >= path->node_count ||
        destination_node_a >= path->node_count || destination_node_b >= path->node_count) {
        packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED;
        return;
    }

    // Match the target's ordinary endpoint solver: compare all four graph
    // routes between the current and destination connection endpoints, adding
    // the partial distance along each connection. This also permits a clean
    // reversal when the valid route leaves behind the character.
    f32 current_dist = packet->path_info.dist;
    f32 distance_to_current_a = connection->distance * current_dist;
    if (distance_to_current_a < 0.0f) {
        distance_to_current_a = -distance_to_current_a;
    }
    f32 distance_to_current_b = connection->distance * (1.0f - current_dist);
    if (distance_to_current_b < 0.0f) {
        distance_to_current_b = -distance_to_current_b;
    }
    f32 destination_dist = destination_path_info.dist;
    f32 distance_from_a = destination_connection->distance * destination_dist;
    if (distance_from_a < 0.0f) {
        distance_from_a = -distance_from_a;
    }
    f32 distance_from_b = destination_connection->distance * (1.0f - destination_dist);
    if (distance_from_b < 0.0f) {
        distance_from_b = -distance_from_b;
    }
    u8 waypoint_index = connection->node_indices[(packet->path_info.direction != 0 ? 1 : 0) ^ 1];
    u8 goal_index = destination_connection->node_indices[destination_path_info.direction != 0 ? 1 : 0];
    f32 best_route_distance = (waypoint_index == current_node_a ? distance_to_current_a : distance_to_current_b) +
                              AIPathNodeDistanceToPathNode(path, waypoint_index, goal_index, packet->current_route, 0) +
                              (goal_index == destination_node_a ? distance_from_a : distance_from_b);

    const u8 current_nodes[2] = {current_node_a, current_node_b};
    const u8 destination_nodes[2] = {destination_node_a, destination_node_b};
    const f32 current_endpoint_distances[2] = {distance_to_current_a, distance_to_current_b};
    const f32 destination_endpoint_distances[2] = {distance_from_a, distance_from_b};
    for (i32 current_end = 0; current_end < 2; ++current_end) {
        for (i32 destination_end = 0; destination_end < 2; ++destination_end) {
            const f32 candidate_route_distance =
                current_endpoint_distances[current_end] +
                AIPathNodeDistanceToPathNode(path, current_nodes[current_end], destination_nodes[destination_end],
                                             packet->current_route, 0) +
                destination_endpoint_distances[destination_end];
            if (candidate_route_distance < best_route_distance) {
                best_route_distance = candidate_route_distance;
                waypoint_index = current_nodes[current_end];
                goal_index = destination_nodes[destination_end];
            }
        }
    }

    const i32 route_direction = waypoint_index == current_node_b ? 0 : 1;
    if (packet->path_info.direction != route_direction) {
        AISysCharacterSetPathCnx(packet, &object->position, connection, route_direction);
        if (packet->path_info.direction != route_direction) {
            packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED;
            return;
        }
        packet->movement_event_flags |= AIPACKET_PATH_CONNECTION_CHANGED;
    }

    AIPATHNODE *waypoint = &path->nodes[waypoint_index];
    packet->goal_path_node = &path->nodes[goal_index];
    packet->movement_destination = waypoint->position;
    packet->movement_stopping_distance = 0.0f;
    packet->runtime_flags |= AIPACKET_RUNTIME_USING_PATH_WAYPOINT;

    AIPATHCNX *next_connection = NULL;
    if (goal_index == waypoint_index) {
        next_connection = destination_connection;
    } else if (path->route_matrix != NULL && path->route_matrix[waypoint_index] != NULL &&
               waypoint->connections != NULL) {
        const u8 next_connection_index = path->route_matrix[waypoint_index][goal_index];
        if (next_connection_index < waypoint->connection_count) {
            next_connection = waypoint->connections[next_connection_index];
        }
    }

    if (next_connection == NULL || next_connection == connection) {
        packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
        return;
    }

    i32 next_direction;
    if (next_connection->node_indices[0] == waypoint_index) {
        next_direction = 0;
    } else if (next_connection->node_indices[1] == waypoint_index) {
        next_direction = 1;
    } else {
        packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
        return;
    }

    if (packet->current_route != 0xff) {
        if (packet->current_route >= 16) {
            packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
            return;
        }
        const u16 route_bit = static_cast<u16>(1u << packet->current_route);
        if ((next_connection->route_mask & route_bit) == 0 && (waypoint->route_boundary_mask & route_bit) == 0) {
            packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
            return;
        }
    }

    const u32 traversal_flags = next_connection->traversal_flags[next_direction];
    if ((traversal_flags & AIPATH_CONNECTION_CAPABILITY_MASK) != 0) {
        if ((traversal_flags & AIPATH_CONNECTION_SPECIAL_MASK) != 0) {
            packet->movement_destination = object->position;
            packet->frame_state = traversal_flags;
            packet->runtime_flags &= static_cast<u8>(~AIPACKET_RUNTIME_PATH_BLOCKED);
            packet->runtime_flags |= AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
            return;
        }
        if ((packet->capabilities & traversal_flags) == 0) {
            packet->frame_state = traversal_flags;
            packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
            return;
        }
        packet->navigation_flags |= AIPACKET_NAVIGATION_FLAG_TRANSIENT;
    }

    // Proximity to the node is insufficient at corners and intersections. The
    // target changes connection only after the collision-sized terrain origin
    // lies inside the next corridor.
    if (WithinConnection(system, &packet->terrain_origin, path, next_connection, checks, connection,
                         packet->current_route, object->field_0x289, NULL, object->collision_radius, 0) == 0) {
        packet->runtime_flags &= static_cast<u8>(~AIPACKET_RUNTIME_PATH_BLOCKED);
        return;
    }

    AISysCharacterSetPathCnx(packet, &object->position, next_connection, next_direction);
    if (packet->path_info.connection != next_connection) {
        packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED | AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
        return;
    }

    packet->movement_event_flags |= AIPACKET_PATH_CONNECTION_CHANGED;
    packet->runtime_flags &= static_cast<u8>(~AIPACKET_RUNTIME_PATH_BLOCKED);
    if (next_connection == destination_connection) {
        packet->movement_destination = packet->fallback_destination;
        packet->movement_stopping_distance = packet->fallback_stopping_distance;
        packet->goal_path_node = NULL;
        packet->runtime_flags &= static_cast<u8>(~AIPACKET_RUNTIME_USING_PATH_WAYPOINT);
        return;
    }

    const u8 next_waypoint_index = next_connection->node_indices[next_direction ^ 1];
    if (next_waypoint_index < path->node_count) {
        packet->movement_destination = path->nodes[next_waypoint_index].position;
    }
}

void AIRetreatFromDestination2(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks);

void AIRetreatFromDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    NUVEC difference;
    if (new_retreat != 0) {
        AIRetreatFromDestination2(system, packet, object, checks);
        return;
    }
    if ((packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) != 0) {
        f32 distance = NuVecXZDist(&object->position, &packet->fallback_destination, &difference);
        if (distance != 0.0f) {
            NuVecScale(&difference, &difference, packet->movement_parameter / distance);
        } else {
            difference.x = packet->movement_parameter;
            difference.y = 0.0f;
            difference.z = 0.0f;
        }
        NuVecAdd(&packet->movement_destination, &object->position, &difference);
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    AIPATHCNX *connection = packet->path_info.connection;
    if (connection == NULL || packet->fallback_path_info.connection == NULL ||
        packet->path_info.path != packet->fallback_path_info.path) {
        return;
    }
    AIPATH *path = packet->path_info.path;
    AIPATHCNX *destination_connection = packet->fallback_path_info.connection;
    if (connection == destination_connection) {
        AISysCharacterSetPathCnx(packet, &object->position, connection,
                                 packet->fallback_path_info.dist > packet->path_info.dist);
    } else {
        f32 parameter = packet->path_info.dist;
        if (parameter > 1.0f) {
            parameter = 1.0f;
        } else if (parameter < 0.0f) {
            parameter = 0.0f;
        }
        f32 remaining_parameter = 1.0f - parameter;
        f32 destination_parameter = packet->fallback_path_info.dist;
        f32 destination_a = fabsf(destination_parameter * destination_connection->distance);
        f32 destination_b = fabsf((1.0f - destination_parameter) * destination_connection->distance);
        f32 length = connection->distance;
        if (packet->current_route != 0xff &&
            ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) == 0) {
            AIPATHNODE *node = &path->nodes[destination_connection->node_indices[0]];
            if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >>
                 packet->current_route) & 1) {
                destination_a = FLT_MAX;
            }
            node = &path->nodes[destination_connection->node_indices[1]];
            if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >>
                 packet->current_route) & 1) {
                destination_b = FLT_MAX;
            }
        }

        f32 best_distance = 0.0f;
        i32 direction = 0;
        if ((connection->traversal_flags[1] & 0x40000000u) == 0) {
            f32 partial_distance = fabsf(parameter * length);
            f32 candidate = AIPathNodeDistanceToPathNode(path, connection->node_indices[0],
                destination_connection->node_indices[0], packet->current_route, 0) + partial_distance + destination_a;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 1;
            }
            candidate = AIPathNodeDistanceToPathNode(packet->path_info.path, packet->path_info.connection->node_indices[0],
                packet->fallback_path_info.connection->node_indices[1], packet->current_route, 0) +
                partial_distance + destination_b;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 1;
            }
            connection = packet->path_info.connection;
        }
        if ((connection->traversal_flags[0] & 0x40000000u) == 0) {
            f32 partial_distance = fabsf(length * remaining_parameter);
            f32 candidate = AIPathNodeDistanceToPathNode(packet->path_info.path, connection->node_indices[1],
                packet->fallback_path_info.connection->node_indices[0], packet->current_route, 0) +
                partial_distance + destination_a;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 0;
            }
            candidate = AIPathNodeDistanceToPathNode(packet->path_info.path, packet->path_info.connection->node_indices[1],
                packet->fallback_path_info.connection->node_indices[1], packet->current_route, 0) +
                partial_distance + destination_b;
            if (candidate > best_distance) {
                direction = 0;
            }
            connection = packet->path_info.connection;
        }
        AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
    }

    connection = packet->path_info.connection;
    u8 end_node = connection->node_indices[packet->path_info.direction == 0];
    packet->goal_path_node = &path->nodes[end_node];
    if (packet->movement_target != NULL && packet->movement_target->node_indices[0] != end_node &&
        packet->movement_target->node_indices[1] != end_node) {
        packet->movement_target = NULL;
    }
    if (packet->movement_target == NULL) {
        AIPATHNODE *node = &packet->path_info.path->nodes[end_node];
        i32 start = NuRandInt() % node->connection_count;
        connection = packet->path_info.connection;
        for (i32 index = 0; index < node->connection_count; ++index) {
            packet->movement_target = node->connections[(index + start) % node->connection_count];
            AIPATHCNX *target = packet->movement_target;
            if (target != connection &&
                (packet->current_route == 0xff ||
                 ((static_cast<u64>(target->route_mask) >> packet->current_route) & 1) != 0 ||
                 ((static_cast<u64>(node->route_boundary_mask) >> packet->current_route) & 1) != 0) &&
                target->traversal_flags[target->node_indices[0] != end_node] == 0) {
                break;
            }
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        u8 old_direction = packet->path_info.direction;
        end_node = connection->node_indices[old_direction];
        AIPATHNODE *node = &path->nodes[end_node];
        for (i32 index = 0; index < node->connection_count; ++index) {
            packet->movement_target = node->connections[index];
            AIPATHCNX *target = packet->movement_target;
            if (target != connection && target->traversal_flags[target->node_indices[0] != end_node] == 0) {
                AISysCharacterSetPathCnx(packet, &object->position, connection, old_direction == 0);
                connection = packet->path_info.connection;
                end_node = connection->node_indices[packet->path_info.direction == 0];
                packet->goal_path_node = &path->nodes[end_node];
                break;
            }
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }
    if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, packet->movement_target,
                         checks, connection, packet->current_route, object->field_0x289, NULL,
                         object->collision_radius, 0) != 0) {
        AISysCharacterSetPathCnx(packet, &object->position, packet->movement_target,
                                 packet->movement_target->node_indices[0] != end_node);
        packet->goal_path_node =
            &path->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_target = NULL;
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    packet->movement_destination = packet->path_info.path->nodes[
        packet->movement_target->node_indices[packet->movement_target_direction == 0]].position;
    packet->movement_stopping_distance = 0.0f;
    if (CalculateIntersection(system, packet, object, packet->path_info.connection, packet->movement_target) == 0) {
        return;
    }
    if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0) {
        NuVecSub(&difference, &packet->left_diversion, &object->position);
        i32 angle = NuAtan2D(difference.x, difference.z);
        NuVecSub(&difference, &packet->movement_destination, &object->position);
        if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) < 0) {
            packet->movement_destination = packet->left_diversion;
            return;
        }
    }
    if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) {
        NuVecSub(&difference, &packet->right_diversion, &object->position);
        i32 angle = NuAtan2D(difference.x, difference.z);
        NuVecSub(&difference, &packet->movement_destination, &object->position);
        if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) > 0) {
            packet->movement_destination = packet->right_diversion;
        }
    }
}

void AIRetreatFromDestination2(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    NUVEC difference;
    f32 distance = NuVecXZDist(&object->position, &packet->fallback_destination, &difference);
    if (distance != 0.0f) {
        NuVecScale(&difference, &difference, packet->movement_parameter / distance);
    } else {
        difference.x = packet->movement_parameter;
        difference.y = 0.0f;
        difference.z = 0.0f;
    }
    NuVecAdd(&packet->movement_destination, &packet->fallback_destination, &difference);
    packet->movement_stopping_distance = 0.0f;
    distance = NuVecXZDist(&packet->movement_destination, &object->position, &difference);
    if (distance > ai_moveradius) {
        NuVecScale(&difference, &difference, ai_moveradius / distance);
        NuVecAdd(&packet->movement_destination, &object->position, &difference);
    }

    NUVEC position = object->position;
    NUVEC path_position = packet->last_path_position;
    AIPATHINFO path_info = packet->path_info;
    object->position = packet->movement_destination;
    AISysGetCharacterPathPos(system, object, packet, 0, 1);
    packet->movement_destination = packet->last_path_position;
    packet->path_info = path_info;
    object->position = position;
    packet->last_path_position = path_position;
}

void AIMoveDirectlyToDestination(AISYS_s *, AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    if (packet->movement_parameter > 0.0f) {
        NUVEC delta;
        const f32 distance_squared = NuVecDistSqr(&packet->fallback_destination, &object->position, &delta);
        if (packet->movement_parameter * packet->movement_parameter > distance_squared) {
            packet->movement_destination = object->position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
    }
    packet->movement_destination = packet->fallback_destination;
    packet->movement_stopping_distance = 0.0f;
}

void AIMoveToDestinationAvoidingCamera(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    const NUVEC destination = packet->fallback_destination;
    AIMoveToDestination(system, packet, object, checks);
    packet->fallback_destination = destination;
}

bool AISysNodeCanReachThisJumpConnection(GameObject_s &, AIPATH_s &path, unsigned char node_index,
                                       AIPATHCNX_s &jump_connection, i32 jump_direction) {
    u8 from_node = jump_connection.node_indices[jump_direction];
    u8 to_node = jump_connection.node_indices[jump_direction == 0];
    while (node_index != from_node) {
        if (node_index == to_node) {
            return 0;
        }
        u8 connection_index = path.route_matrix[node_index][from_node];
        if (connection_index == 0xff) {
            return 0;
        }
        AIPATHCNX *connection = path.nodes[node_index].connections[connection_index];
        if (connection == NULL) {
            return 0;
        }
        u8 direction = node_index == connection->node_indices[1];
        if ((connection->traversal_flags[direction] & mechAutoJumpCantReachFlags) != 0) {
            return 0;
        }
        node_index = connection->node_indices[direction ^ 1];
    }
    return 1;
}

bool AISysCharacterCanReachThisJumpConnection(GameObject_s &object, AIPATH_s &path,
                                            AIPATHCNX_s &jump_connection, i32 jump_direction) {
    if (object.ai.path_info.path != &path || object.ai.path_info.connection == NULL) {
        return 0;
    }
    u8 node_index = object.ai.path_info.dist < 0.5f ? object.ai.path_info.connection->node_indices[0]
                                                  : object.ai.path_info.connection->node_indices[1];
    return AISysNodeCanReachThisJumpConnection(object, path, node_index, jump_connection, jump_direction);
}

u32 DoSomeChecks(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &connection, i32 direction) {
    AIPATHNODE *node = &path.nodes[connection.node_indices[direction]];
    if (!(object.ai.terrain_origin.y > node->min_height && object.ai.terrain_origin.y < node->max_height)) {
        return 0;
    }
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->current_level == E1CHARACTERBONUSA_LDATA) {
        AIPATHNODE *other_node = &path.nodes[connection.node_indices[direction == 0]];
        if (node->position.y - other_node->position.y > 0.5f) {
            return 0;
        }
    }
    f32 distance = NuVecXZDistSqr(&object.ai.terrain_origin, &node->position, NULL);
    u32 result = 0;
    if (distance < node->radius_squared || distance < testAutoJumpXZCanUseRangeSqr) {
        result |= 1;
    }
    if (distance < testAutoJumpXZCanDisplayRangeSqr) {
        result |= 2;
    }
    if (result == 0 || !AISysCharacterCanReachThisJumpConnection(object, path, connection, direction)) {
        return 0;
    }
    if ((connection.traversal_flags[direction] & object.ai.capabilities & mechAutoJumpFlags) != 0) {
        result |= 4;
    }
    return result;
}

void AICircle(AISYS_s *, AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    f32 radius = MAX(packet->fallback_stopping_distance, packet->movement_parameter);
    NUVEC difference;
    NuVecSub(&difference, &object->position, &packet->fallback_destination);
    i32 angle = NuAtan2D(difference.x, difference.z);
    difference.x = 0.0f;
    difference.z = radius;
    NuVecRotateY(&difference, &difference, angle);
    NUVEC circle_position;
    NuVecAdd(&circle_position, &packet->fallback_destination, &difference);
    if (packet->circle_clockwise != 0) {
        angle = NuAngSub(angle, NUANG_90DEG);
    } else {
        angle = NuAngAdd(angle, NUANG_90DEG);
    }
    difference.x = 0.0f;
    difference.y = 0.0f;
    difference.z = 0.5f;
    NuVecRotateY(&difference, &difference, angle);
    NuVecAdd(&packet->movement_destination, &circle_position, &difference);
    packet->circle_active = 1;
    packet->movement_stopping_distance = 0.0f;
}

void AIWander(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    NUVEC difference;
    if (packet->movement_target != NULL) {
        i32 node_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
        if (packet->movement_target->node_indices[0] != node_index &&
            packet->movement_target->node_indices[1] != node_index) {
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        u8 node_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
        AIPATHNODE *node = &packet->path_info.path->nodes[node_index];
        i32 start = NuRandInt() % node->connection_count;
        for (i32 index = 0; index < node->connection_count; ++index) {
            packet->movement_target = node->connections[(index + start) % node->connection_count];
            AIPATHCNX *target = packet->movement_target;
            if (target != packet->path_info.connection) {
                if (packet->current_route == 0xff ||
                    ((static_cast<u64>(target->route_mask) >> packet->current_route) & 1) != 0 ||
                    ((static_cast<u64>(node->route_boundary_mask) >> packet->current_route) & 1) != 0) {
                    packet->movement_target_direction = node_index != target->node_indices[0];
                    if (target->traversal_flags[packet->movement_target_direction] == 0) {
                        break;
                    }
                }
            }
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        AIPATHNODE *node = &packet->path_info.path->nodes[
            packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        f32 distance = NuVecXZDistSqr(&object->position, &node->position, &difference);
        if (distance < node->radius_squared) {
            AISysCharacterSetPathCnx(packet, &object->position, packet->path_info.connection,
                                     packet->path_info.direction == 0);
            node = &packet->path_info.path->nodes[
                packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
            packet->goal_path_node = node;
        }
        packet->movement_destination = node->position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    packet->runtime_flags |= AIPACKET_RUNTIME_SPECIAL_MOVE;
    if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, packet->movement_target,
                         checks, packet->path_info.connection, packet->current_route, object->field_0x289,
                         NULL, object->collision_radius, 0) == 0) {
        packet->movement_destination = packet->path_info.path->nodes[
            packet->movement_target->node_indices[packet->movement_target_direction == 0]].position;
        packet->movement_stopping_distance = 0.0f;
        if (CalculateIntersection(system, packet, object, packet->path_info.connection, packet->movement_target) != 0) {
            if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0) {
                NuVecSub(&difference, &packet->left_diversion, &object->position);
                i32 angle = NuAtan2D(difference.x, difference.z);
                NuVecSub(&difference, &packet->movement_destination, &object->position);
                if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) < 0) {
                    packet->movement_destination = packet->left_diversion;
                    return;
                }
            }
            if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) {
                NuVecSub(&difference, &packet->right_diversion, &object->position);
                i32 angle = NuAtan2D(difference.x, difference.z);
                NuVecSub(&difference, &packet->movement_destination, &object->position);
                if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) > 0) {
                    packet->movement_destination = packet->right_diversion;
                }
            }
        }
    } else {
        AISysCharacterSetPathCnx(packet, &object->position, packet->movement_target, packet->movement_target_direction);
        packet->goal_path_node = &packet->path_info.path->nodes[
            packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_stopping_distance = 0.0f;
        packet->movement_target = NULL;
    }
}
