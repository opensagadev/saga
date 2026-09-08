#include "decomp.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include <string.h>

#include <float.h>
#include <math.h>
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

static u32 CalculateRightIntersection(APIOBJECT *object, AIPATHCNX *first_connection, AIPATHCNX *second_connection,
                                      i32 first_angle, i32 second_angle, AIPATHNODE *before, AIPATHNODE *shared,
                                      AIPATHNODE *after, NUVEC *result) {
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
    NUANG angle =
        NuAngAdd(first_angle, NuASin((shared_radius - before_radius) / first_connection->horizontal_distance));
    NuVecRotateY(&first_start, &first_start, angle);
    NuVecRotateY(&first_end, &first_end, angle);
    NuVecAdd(&first_start, &first_start, &before->position);
    NuVecAdd(&first_end, &first_end, &before->position);

    NUVEC second_start = {-after_radius, 0.0f, 0.0f};
    NUVEC second_end = {-after_radius, 0.0f, second_connection->horizontal_distance};
    angle = NuAngSub(second_angle, NuASin((shared_radius - after_radius) / second_connection->horizontal_distance));
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
    if (NuLineLineIntersect(&first_origin, &first_direction, &second_origin, &second_direction, &first_parameter,
                            &second_parameter) != 0 &&
        first_parameter >= 0.0f && second_parameter >= 0.0f && first_parameter <= 1.0f && second_parameter <= 1.0f) {
        first_origin.y = shared->position.y * first_parameter + before->position.y * (1.0f - first_parameter);
        NuVecScale(&first_direction, &first_direction, first_parameter);
        NuVecAdd(result, &first_origin, &first_direction);
        return 1;
    }
    return 0;
}

static u32 CalculateIntersection(AISYS *system, AIPACKET *packet, APIOBJECT *object, AIPATHCNX *current_connection,
                                 AIPATHCNX *target_connection) {
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
        u32 right = CalculateRightIntersection(object, current_connection, target_connection, first_angle, second_angle,
                                               before, shared, after, &packet->right_diversion);
        packet->movement_flags = (packet->movement_flags & ~AIPACKET_MOVEMENT_DIVERSION_RIGHT) | ((right & 1) << 5);
        u32 left = CalculateRightIntersection(object, target_connection, current_connection, second_angle, first_angle,
                                              after, shared, before, &packet->left_diversion);
        packet->movement_flags = (packet->movement_flags & ~AIPACKET_MOVEMENT_DIVERSION_LEFT) | ((left & 1) << 6);
    }
    return ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) |
           ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0);
}

void AISysGetPathPos2(AISYS_s *system, nuvec_s *position, AIPATHINFO_s *path_info, nuvec_s *path_position, AIPATH_s *,
                      i32) {
    if (position != NULL && system != NULL && path_info != NULL) {
        APIOBJECT object;
        AIPACKET packet;
        memset(&object, 0, sizeof(object));
        memset(&packet, 0, sizeof(packet));
        object.ai = &packet;
        object.position = *position;
        packet.terrain_origin = *position;
        AISysGetCharacterPathPos(system, &object, &packet, 0xff, 1);
        *path_info = packet.path_info;
        if (path_position != NULL) {
            *path_position = packet.last_path_position;
        }
    }
}

i32 AIMoveCanReachPath(AISYS_s *system, AIPATH_s *path, AIPATH_s *destination) {
    AIPATHNODELINK *links = path->special_routes;
    for (i32 link_index = 0; link_index < path->special_route_count; ++link_index) {
        AIPATHSPECIALROUTE *route = &system->path_sys->special_routes[links[link_index].special_route_index];
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

static void AIMoveFindDivertNode(AISYS *, AIPATH *path, AIPACKET *packet, NUVEC *destination) {
    if (path->special_route_count != 0 &&
        (packet->navigation_flags & AIPACKET_NAVIGATION_FLAG_USE_SPECIAL_ROUTES) != 0) {
        AIPATHNODELINK *links = path->special_routes;
        if (packet->divert_node_index >= path->node_count) {
            packet->divert_node_index = 0xff;
        }
        if (packet->divert_search_index >= path->special_route_count) {
            packet->divert_search_index = 0;
        }
        f32 nearest = FLT_MAX;
        if (packet->divert_node_index < path->node_count) {
            nearest = NuVecDistSqr(&path->nodes[packet->divert_node_index].position, destination, NULL);
        }
        u8 initial_index = packet->divert_search_index;
        for (i32 index = 0; index < 4; ++index) {
            if (packet->divert_search_index != packet->divert_node_index) {
                f32 distance = NuVecDistSqr(&path->nodes[links[packet->divert_search_index].node_index].position,
                                            destination, NULL);
                if (distance < nearest) {
                    nearest = distance;
                    packet->divert_node_index = links[packet->divert_search_index].node_index;
                }
            }
            ++packet->divert_search_index;
            if (packet->divert_search_index > path->special_route_count) {
                packet->divert_search_index = 0;
            }
            if (packet->divert_search_index == initial_index) {
                break;
            }
        }
    } else {
        if (packet->divert_node_index >= path->node_count) {
            packet->divert_node_index = 0xff;
        }
        if (packet->divert_search_index >= path->node_count) {
            packet->divert_search_index = 0;
        }
        f32 nearest = FLT_MAX;
        if (packet->divert_node_index < path->node_count) {
            nearest = NuVecDistSqr(&path->nodes[packet->divert_node_index].position, destination, NULL);
        }
        for (i32 index = 0; index < 4; ++index) {
            f32 distance = NuVecDistSqr(&path->nodes[packet->divert_search_index].position, destination, NULL);
            if (distance < nearest) {
                nearest = distance;
                packet->divert_node_index = packet->divert_search_index;
            }
            ++packet->divert_search_index;
            if (packet->divert_search_index > path->node_count) {
                packet->divert_search_index = 0;
            }
        }
    }
}

static i32 AIMoveAdjustDestinationPath(AISYS *system, AIPACKET *packet) {
    if (packet->path_info.path == packet->fallback_path_info.path && packet->fallback_path_info.connection != NULL) {
        return 0;
    }
    AIMoveFindDivertNode(system, packet->path_info.path, packet, &packet->fallback_destination);
    if (packet->divert_node_index >= packet->path_info.path->node_count) {
        return -1;
    }
    AIPATHNODE *node = &packet->path_info.path->nodes[packet->divert_node_index];
    if (node->connection_count == 0) {
        return -1;
    }
    packet->fallback_destination = node->position;
    packet->fallback_stopping_distance = 0.0f;
    packet->movement_parameter = NuFmax(node->radius - 1.0f, 1.0f);
    memset(&packet->fallback_path_info, 0, sizeof(packet->fallback_path_info));
    packet->fallback_path_info.path = packet->path_info.path;
    packet->fallback_path_info.connection = node->connections[0];
    packet->fallback_path_info.direction = node->connections[0]->node_indices[0] == packet->divert_node_index;
    packet->fallback_path_info.dist = node->connections[0]->node_indices[0] == packet->divert_node_index ? 0.0f : 1.0f;
    return 1;
}

static i32 AIMoveChooseExitNodePath(AISYS *system, AIPACKET *packet) {
    AIPATHNODE *first = &packet->path_info.path->nodes[packet->path_info.connection->node_indices[0]];
    AIPATHNODE *second = &packet->path_info.path->nodes[packet->path_info.connection->node_indices[1]];
    f32 distance = NuVecDistSqr(&packet->owner->apiobj.position, &first->position, NULL) - first->radius_squared;
    f32 second_distance =
        NuVecDistSqr(&packet->owner->apiobj.position, &second->position, NULL) - second->radius_squared;
    AIPATHNODE *node = distance < second_distance ? first : second;
    if (node->special_route_index >= packet->path_info.path->special_route_count) {
        return 0;
    }
    AIPATHSPECIALROUTE *route =
        &system->path_sys
             ->special_routes[packet->path_info.path->special_routes[node->special_route_index].special_route_index];
    for (i32 index = 0; index < route->path_count; ++index) {
        if (route->paths[index] == packet->fallback_path_info.path) {
            AISysCharacterSetPath(packet, packet->fallback_path_info.path);
            return 1;
        }
    }
    f32 nearest = FLT_MAX;
    AIPATH *selected = NULL;
    for (i32 index = 0; index < route->path_count; ++index) {
        AIPATH *path = route->paths[index];
        AIPATHNODELINK *links = path->special_routes;
        i32 nearest_node = -1;
        f32 nearest_node_distance = FLT_MAX;
        for (i32 link = 0; link < path->special_route_count; ++link) {
            f32 candidate =
                NuVecDistSqr(&path->nodes[links[link].node_index].position, &packet->fallback_destination, NULL);
            if (candidate < nearest_node_distance) {
                nearest_node_distance = candidate;
                nearest_node = links[link].node_index;
            }
        }
        if (nearest_node != -1) {
            distance = nearest_node_distance;
        }
        if (distance < nearest) {
            nearest = distance;
            selected = route->paths[index];
        }
    }
    if (selected != NULL) {
        AISysCharacterSetPath(packet, selected);
        return 1;
    }
    return 0;
}

static AIPATHCNX *GetNextConnection(const AIPACKET *packet, i32 *direction) {
    if (packet->goal_path_node == NULL || packet->path_info.connection == NULL) {
        return NULL;
    }
    AIPATH *path = packet->path_info.path;
    i32 node_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
    AIPATHNODE *node = &path->nodes[node_index];
    u32 destination = packet->goal_path_node - path->nodes;
    if (destination > 0xff) {
        return NULL;
    }
    AIPATHCNX *connection = NULL;
    i32 connection_index = path->route_matrix[node_index][destination];
    if (connection_index < node->connection_count) {
        connection = node->connections[connection_index];
        *direction = connection->node_indices[0] != node_index;
    }
    u8 route_index = packet->current_route;
    if (route_index == 0xff) {
        return connection;
    }
    if ((static_cast<u64>(static_cast<u16>(node->route_membership_mask)) >> route_index & 1) == 0) {
        return NULL;
    }
    if ((static_cast<u64>(static_cast<u16>(node->route_boundary_mask)) >> route_index & 1) != 0) {
        if (connection == NULL) {
            return NULL;
        }
        if ((static_cast<u64>(connection->route_mask) >> route_index & 1) == 0) {
            return connection;
        }
    }
    AIPATHROUTE *route = &path->routes[route_index];
    i32 next_node = destination;
    if ((static_cast<u64>(static_cast<u16>(path->nodes[destination].route_membership_mask)) >> route_index & 1) == 0) {
        if (route->exit_node_count == 0) {
            return NULL;
        }
        f32 nearest = FLT_MAX;
        next_node = 0;
        for (i32 index = 0; index < route->exit_node_count; ++index) {
            f32 first = AIPathNodeDistanceToPathNode(packet->path_info.path, node_index, route->exit_nodes[index],
                                                     packet->current_route, 0);
            f32 second =
                AIPathNodeDistanceToPathNode(packet->path_info.path, route->exit_nodes[index], destination, 0xff,
                                             static_cast<u32>(static_cast<u64>(1) << packet->current_route));
            f32 distance = first != FLT_MAX && second != FLT_MAX ? first + second : FLT_MAX;
            if (distance < nearest) {
                nearest = distance;
                next_node = route->exit_nodes[index];
            }
        }
        if (nearest == FLT_MAX) {
            return NULL;
        }
    }
    connection_index = route->route_nodes[route->node_routes[node_index]][route->node_routes[next_node]];
    if (connection_index >= node->connection_count) {
        return NULL;
    }
    connection = node->connections[connection_index];
    *direction = connection->node_indices[0] != node_index;
    return connection;
}

extern "C" i32 AISysGetCharacterWaypoint(const AIPACKET *packet, NUVEC *position) {
    if (packet->path_info.connection != packet->fallback_path_info.connection) {
        i32 direction;
        AIPATHCNX *connection = GetNextConnection(packet, &direction);
        if (connection != NULL) {
            if (position != NULL) {
                *position = packet->path_info.path->nodes[connection->node_indices[direction == 0]].position;
            }
            return 1;
        }
    }
    return 0;
}

void AIMoveToDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    i32 direction = 0;
    NUVEC saved_destination = packet->fallback_destination;
    f32 saved_stopping_distance = packet->fallback_stopping_distance;
    f32 saved_parameter = packet->movement_parameter;
    AIPATHINFO saved_path_info = packet->fallback_path_info;
    AIPATH *path = packet->path_info.path;
    i32 diverted = 0;
    if (path != packet->fallback_path_info.path || packet->fallback_path_info.connection == NULL) {
        AIMoveFindDivertNode(system, path, packet, &packet->fallback_destination);
        if (packet->divert_node_index >= packet->path_info.path->node_count ||
            packet->path_info.path->nodes[packet->divert_node_index].connection_count == 0) {
            packet->movement_destination = packet->owner->apiobj.position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
        AIPATHNODE *node = &packet->path_info.path->nodes[packet->divert_node_index];
        packet->fallback_destination = node->position;
        packet->fallback_stopping_distance = 0.0f;
        packet->movement_parameter = NuFmax(node->radius - 1.0f, 1.0f);
        memset(&packet->fallback_path_info, 0, sizeof(packet->fallback_path_info));
        packet->fallback_path_info.path = packet->path_info.path;
        path = packet->path_info.path;
        packet->fallback_path_info.connection = node->connections[0];
        packet->fallback_path_info.direction = node->connections[0]->node_indices[0] == packet->divert_node_index;
        packet->fallback_path_info.dist =
            node->connections[0]->node_indices[0] == packet->divert_node_index ? 0.0f : 1.0f;
        diverted = 1;
    }
    AIPATHCNX *connection = packet->path_info.connection;
    AIPATHCNX *destination_connection = packet->fallback_path_info.connection;
    NUVEC difference;
    f32 distance_squared = NuVecDistSqr(&packet->fallback_destination, &object->position, &difference);
    if ((object->supporting_platform_id == -1 || (path->nodes[connection->node_indices[0]].has_special != 0 &&
                                                  path->nodes[connection->node_indices[1]].has_special != 0)) &&
        packet->movement_parameter != 0.0f && difference.y < 0.5f && difference.y > -0.5f &&
        distance_squared < packet->movement_parameter * packet->movement_parameter) {
        if (diverted == 0) {
            packet->movement_destination = object->position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
        if ((packet->navigation_flags & 4) != 0) {
            packet->fallback_destination = saved_destination;
            packet->fallback_stopping_distance = saved_stopping_distance;
            packet->movement_parameter = saved_parameter;
            packet->fallback_path_info = saved_path_info;
            AIMoveChooseExitNodePath(system, packet);
            AIMoveAdjustDestinationPath(system, packet);
            path = packet->path_info.path;
            connection = packet->path_info.connection;
            destination_connection = packet->fallback_path_info.connection;
        }
    }
    if (connection == NULL || path == NULL || destination_connection == NULL) {
        packet->movement_destination = packet->owner->apiobj.position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }
    if (packet->path_info.connection != packet->fallback_path_info.connection) {
        if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path,
                             packet->fallback_path_info.connection, checks, packet->path_info.connection,
                             packet->current_route, object->field_0x289, NULL, object->collision_radius, 0) != 0) {
            AIPATHCNX *target = packet->fallback_path_info.connection;
            AIPATHCNX *current = packet->path_info.connection;
            direction = target->node_indices[0] != current->node_indices[0] &&
                        target->node_indices[0] != current->node_indices[1];
            AISysCharacterSetPathCnx(packet, &object->position, target, direction);
        }
        if (packet->path_info.connection != packet->fallback_path_info.connection) {
            direction = packet->path_info.direction;
            i32 goal_index = destination_connection->node_indices[0];
            f32 current_distance = packet->path_info.dist;
            if (current_distance > 1.0f) {
                current_distance = 1.0f;
            } else if (current_distance < 0.0f) {
                current_distance = 0.0f;
            }
            f32 endpoint_distances[2] = {fabsf(current_distance * connection->distance),
                                         fabsf((1.0f - current_distance) * connection->distance)};
            if (packet->current_route != 0xff) {
                packet->runtime_flags |= 0x10;
            }
            f32 best_distance = FLT_MAX;
            u32 attempted_routes = 0;
            for (;;) {
                f32 destination_distances[2] = {
                    fabsf(packet->fallback_path_info.dist * destination_connection->distance),
                    fabsf((1.0f - packet->fallback_path_info.dist) * destination_connection->distance)};
                if (packet->current_route != 0xff &&
                    ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) == 0) {
                    for (i32 end = 0; end < 2; ++end) {
                        AIPATHNODE *node = &path->nodes[destination_connection->node_indices[end]];
                        if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >>
                                 packet->current_route &
                             1) != 0) {
                            destination_distances[end] = FLT_MAX;
                        }
                    }
                }
                if ((connection->traversal_flags[packet->path_info.direction] & 0x40000000) == 0) {
                    {
                        f32 candidate = FLT_MAX;
                        if (destination_distances[0] != FLT_MAX &&
                            endpoint_distances[packet->path_info.direction == 0] != FLT_MAX) {
                            candidate = AIPathNodeDistanceToPathNode(
                                packet->path_info.path, connection->node_indices[packet->path_info.direction == 0],
                                destination_connection->node_indices[0], packet->current_route, 0);
                            if (candidate != FLT_MAX) {
                                candidate +=
                                    endpoint_distances[packet->path_info.direction == 0] + destination_distances[0];
                            }
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[0];
                            direction = packet->path_info.direction;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                    {
                        f32 candidate = FLT_MAX;
                        if (destination_distances[1] != FLT_MAX &&
                            endpoint_distances[packet->path_info.direction == 0] != FLT_MAX) {
                            candidate = AIPathNodeDistanceToPathNode(
                                packet->path_info.path, connection->node_indices[packet->path_info.direction == 0],
                                destination_connection->node_indices[1], packet->current_route, 0);
                            if (candidate != FLT_MAX) {
                                candidate +=
                                    endpoint_distances[packet->path_info.direction == 0] + destination_distances[1];
                            }
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[1];
                            direction = packet->path_info.direction;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                }
                if ((connection->traversal_flags[packet->path_info.direction == 0] & 0x40000000) == 0) {
                    {
                        f32 candidate = FLT_MAX;
                        if (destination_distances[0] != FLT_MAX &&
                            endpoint_distances[packet->path_info.direction] != FLT_MAX) {
                            candidate = AIPathNodeDistanceToPathNode(
                                packet->path_info.path, connection->node_indices[packet->path_info.direction],
                                destination_connection->node_indices[0], packet->current_route, 0);
                            if (candidate != FLT_MAX) {
                                candidate += destination_distances[0] + endpoint_distances[packet->path_info.direction];
                            }
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[0];
                            direction = packet->path_info.direction == 0;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                    {
                        f32 candidate = FLT_MAX;
                        if (destination_distances[1] != FLT_MAX &&
                            endpoint_distances[packet->path_info.direction] != FLT_MAX) {
                            candidate = AIPathNodeDistanceToPathNode(
                                packet->path_info.path, connection->node_indices[packet->path_info.direction],
                                destination_connection->node_indices[1], packet->current_route, 0);
                            if (candidate != FLT_MAX) {
                                candidate += destination_distances[1] + endpoint_distances[packet->path_info.direction];
                            }
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[1];
                            direction = packet->path_info.direction == 0;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                }
                if ((packet->runtime_flags & 0x10) == 0) {
                    break;
                }
                if (packet->current_route != 0xff) {
                    attempted_routes |= 1u << packet->current_route;
                }
                AISysFindRoute(packet);
                if (((attempted_routes >> packet->current_route) & 1) != 0) {
                    return;
                }
            }
            packet->goal_path_node = &path->nodes[goal_index];
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            AIPATHCNX *next = GetNextConnection(packet, &direction);
            if (next != NULL) {
                while (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, next, checks,
                                        packet->path_info.connection, packet->current_route, object->field_0x289, NULL,
                                        object->collision_radius, 0) != 0) {
                    if (connection == destination_connection) {
                        break;
                    }
                    AISysCharacterSetPathCnx(packet, &object->position, next, direction);
                    if (packet->path_info.connection != next || packet->path_info.direction != direction) {
                        connection = next;
                        break;
                    }
                    connection = next;
                    next = GetNextConnection(packet, &direction);
                    if (next == connection || next == NULL) {
                        break;
                    }
                }
            } else {
                u32 flags = destination_connection->traversal_flags[0] | destination_connection->traversal_flags[1];
                if ((flags & 0xd8000000) == 0 &&
                    ((flags & 0xdfffffff) == 0 || (packet->capabilities & (flags & 0xdfffffff)) != 0) &&
                    WithinConnection(system, &packet->terrain_origin, packet->path_info.path, destination_connection,
                                     checks, packet->path_info.connection, packet->current_route, object->field_0x289,
                                     NULL, object->collision_radius, 0) != 0) {
                    AISysCharacterSetPathCnx(packet, &object->position, destination_connection, 0);
                }
            }
        }
    }
    if (PreparingForSpecialMoveFn != NULL) {
        if (connection != destination_connection) {
            packet->movement_event_flags |= 0x20;
        }
        if (PreparingForSpecialMoveFn(packet, object, checks) != 0) {
            packet->runtime_flags |= 8;
            packet->movement_event_flags |= 0x40;
            return;
        }
    }
    AIPATHCNX *current = packet->path_info.connection;
    u32 flags = current->traversal_flags[packet->path_info.direction];
    if ((flags & 0xd8000000) != 0) {
        packet->movement_destination = path->nodes[current->node_indices[packet->path_info.direction]].position;
        packet->runtime_flags |= 0x40;
        packet->movement_stopping_distance = 0.0f;
        packet->frame_state = flags;
        return;
    }
    if (current == destination_connection &&
        (packet->current_route == 0xff ||
         ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) != 0)) {
        if ((packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) == 0 || packet->movement_parameter == 0.0f) {
            packet->movement_stopping_distance = packet->fallback_stopping_distance;
            packet->movement_destination = packet->fallback_destination;
            return;
        }
        if (distance_squared < packet->movement_parameter * packet->movement_parameter) {
            if (diverted == 0) {
                packet->movement_destination = object->position;
                packet->movement_stopping_distance = 0.0f;
                return;
            }
            if ((packet->navigation_flags & 4) != 0) {
                packet->fallback_destination = saved_destination;
                packet->fallback_stopping_distance = saved_stopping_distance;
                packet->movement_parameter = saved_parameter;
                packet->fallback_path_info = saved_path_info;
                AIMoveChooseExitNodePath(system, packet);
                AIMoveAdjustDestinationPath(system, packet);
            }
        }
        NuVecScale(&difference, &difference, packet->movement_parameter / NuFsqrt(distance_squared));
        packet->movement_destination.x = packet->fallback_destination.x - difference.x;
        packet->movement_destination.y = packet->fallback_destination.y;
        packet->movement_destination.z = packet->fallback_destination.z - difference.z;
        packet->movement_stopping_distance =
            packet->fallback_stopping_distance < packet->movement_parameter ? 0.0f : packet->fallback_stopping_distance;
        return;
    }
    AIPATHNODE *shared = NULL;
    if (current->node_indices[0] == destination_connection->node_indices[0]) {
        shared = &path->nodes[destination_connection->node_indices[0]];
        direction = 0;
    } else if (current->node_indices[0] == destination_connection->node_indices[1]) {
        shared = &path->nodes[destination_connection->node_indices[1]];
        direction = 1;
    } else if (current->node_indices[1] == destination_connection->node_indices[0]) {
        shared = &path->nodes[destination_connection->node_indices[0]];
        direction = 0;
    } else if (current->node_indices[1] == destination_connection->node_indices[1]) {
        shared = &path->nodes[destination_connection->node_indices[1]];
        direction = 1;
    }
    AIPATHCNX *next;
    if (shared != NULL && (packet->current_route == 0xff ||
                           ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) != 0 ||
                           ((static_cast<u64>(shared->route_boundary_mask) >> packet->current_route) & 1) != 0)) {
        next = destination_connection;
    } else {
        next = GetNextConnection(packet, &direction);
    }
    if (next != NULL) {
        flags = next->traversal_flags[direction];
        if ((flags & 0xdfffffff) != 0) {
            if ((flags & 0xd8000000) != 0) {
                packet->movement_destination = object->position;
                packet->runtime_flags |= 0x40;
                packet->movement_stopping_distance = 0.0f;
                packet->frame_state = flags;
                return;
            }
            if ((packet->capabilities & flags) != 0) {
                packet->navigation_flags |= 1;
            } else {
                packet->runtime_flags |= 0x60;
                packet->frame_state = next->traversal_flags[direction];
                next = NULL;
            }
        }
        if (next != NULL) {
            NUVEC target;
            if (next == destination_connection) {
                target = packet->fallback_destination;
            } else {
                target = path->nodes[next->node_indices[direction]].position;
                packet->runtime_flags |= 8;
            }
            if (WithinConnection(system, &target, packet->path_info.path, current, checks, packet->path_info.connection,
                                 packet->current_route, object->field_0x289, NULL, object->collision_radius, 0) == 0) {
                if (CalculateIntersection(system, packet, object, current, next) != 0) {
                    bool use_left = false;
                    NUVEC offset;
                    if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0) {
                        NuVecSub(&offset, &packet->left_diversion, &object->position);
                        i32 angle = NuAtan2D(offset.x, offset.z);
                        NuVecSub(&offset, &target, &object->position);
                        if (NuAngSub(NuAtan2D(offset.x, offset.z), angle) < 0) {
                            target = packet->left_diversion;
                            packet->runtime_flags |= 8;
                            use_left = true;
                        }
                    }
                    if (!use_left && (packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) {
                        NuVecSub(&offset, &packet->right_diversion, &object->position);
                        i32 angle = NuAtan2D(offset.x, offset.z);
                        NuVecSub(&offset, &target, &object->position);
                        if (NuAngSub(NuAtan2D(offset.x, offset.z), angle) > 0) {
                            target = packet->right_diversion;
                            packet->runtime_flags |= 8;
                        }
                    }
                } else {
                    target = path->nodes[next->node_indices[direction]].position;
                }
            }
            packet->movement_stopping_distance = 0.0f;
            packet->movement_destination = target;
            return;
        }
    }
    packet->movement_destination =
        path->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]].position;
    packet->movement_stopping_distance = 0.0f;
    NUVEC offset;
    if (NuVecDistSqr(&packet->movement_destination, &object->position, &offset) < 1.0f) {
        packet->movement_destination = object->position;
    }
    packet->runtime_flags |= 0x40;
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
            if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >> packet->current_route) &
                1) {
                destination_a = FLT_MAX;
            }
            node = &path->nodes[destination_connection->node_indices[1]];
            if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >> packet->current_route) &
                1) {
                destination_b = FLT_MAX;
            }
        }

        f32 best_distance = 0.0f;
        i32 direction = 0;
        if ((connection->traversal_flags[1] & 0x40000000u) == 0) {
            f32 partial_distance = fabsf(parameter * length);
            f32 candidate =
                AIPathNodeDistanceToPathNode(path, connection->node_indices[0], destination_connection->node_indices[0],
                                             packet->current_route, 0) +
                partial_distance + destination_a;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 1;
            }
            candidate = AIPathNodeDistanceToPathNode(
                            packet->path_info.path, packet->path_info.connection->node_indices[0],
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
                                                         packet->fallback_path_info.connection->node_indices[0],
                                                         packet->current_route, 0) +
                            partial_distance + destination_a;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 0;
            }
            candidate = AIPathNodeDistanceToPathNode(
                            packet->path_info.path, packet->path_info.connection->node_indices[1],
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
    if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, packet->movement_target, checks,
                         connection, packet->current_route, object->field_0x289, NULL, object->collision_radius,
                         0) != 0) {
        AISysCharacterSetPathCnx(packet, &object->position, packet->movement_target,
                                 packet->movement_target->node_indices[0] != end_node);
        packet->goal_path_node =
            &path->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_target = NULL;
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    packet->movement_destination =
        packet->path_info.path->nodes[packet->movement_target->node_indices[packet->movement_target_direction == 0]]
            .position;
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

bool AISysCharacterCanReachThisJumpConnection(GameObject_s &object, AIPATH_s &path, AIPATHCNX_s &jump_connection,
                                              i32 jump_direction) {
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
        AIPATHNODE *node = &packet->path_info.path
                                ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        f32 distance = NuVecXZDistSqr(&object->position, &node->position, &difference);
        if (distance < node->radius_squared) {
            AISysCharacterSetPathCnx(packet, &object->position, packet->path_info.connection,
                                     packet->path_info.direction == 0);
            node = &packet->path_info.path
                        ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
            packet->goal_path_node = node;
        }
        packet->movement_destination = node->position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    packet->runtime_flags |= AIPACKET_RUNTIME_SPECIAL_MOVE;
    if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, packet->movement_target, checks,
                         packet->path_info.connection, packet->current_route, object->field_0x289, NULL,
                         object->collision_radius, 0) == 0) {
        packet->movement_destination =
            packet->path_info.path->nodes[packet->movement_target->node_indices[packet->movement_target_direction == 0]]
                .position;
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
        packet->goal_path_node =
            &packet->path_info.path
                 ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_stopping_distance = 0.0f;
        packet->movement_target = NULL;
    }
}
