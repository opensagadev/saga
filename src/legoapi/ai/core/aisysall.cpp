#include "decomp.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void AISysFindRoute(AIPACKET *packet);

static i16 IntersectionAngle(f32 value) {
    f32 absolute = NuFabs(value);
    f32 root = NuFsqrt(1.0f - value * value);
    f32 small = root < absolute ? root : absolute;
    f32 side = (absolute - 0.70710677f) * 3.40282e+38f;
    side = side < 1.0f ? (side > -1.0f ? side : -1.0f) : 1.0f;
    f32 sign = value * 3.40282e+38f;
    sign = sign < 1.0f ? (sign > -1.0f ? sign : -1.0f) : 1.0f;
    f32 product = side * sign;
    f32 x = small * product;
    f32 x2 = x * x;
    f32 x3 = x * x2;
    f32 x4 = x2 * x2;
    f32 x5 = x2 * x3;
    return static_cast<i16>(static_cast<i32>(((sign + product) * 0.785398f - x + (x * -0.166667f) * x2 +
                                              (-0.075f * x2) * x3 + (-0.0446429f * x3) * x4 + (x4 * -0.0303819f) * x5) *
                                             10430.4f));
}

static i32 CalculateRightIntersection(APIOBJECT_s *object, AIPATHCNX_s *first_connection,
                                      AIPATHCNX_s *second_connection, i32 first_angle, i32 second_angle,
                                      AIPATHNODE_s *first, AIPATHNODE_s *middle, AIPATHNODE_s *last, NUVEC *result) {
    const f32 margin = object->collision_radius + 0.05f;
    f32 first_radius = first->radius - margin;
    f32 middle_radius = middle->radius - margin;
    f32 last_radius = last->radius - margin;
    if (first_radius < 0.0f)
        first_radius = 0.0f;
    if (middle_radius < 0.0f)
        middle_radius = 0.0f;
    if (last_radius < 0.0f)
        last_radius = 0.0f;
    NUVEC a = {first_radius, 0.0f, 0.0f};
    NUVEC b = {first_radius, 0.0f, first_connection->horizontal_distance};
    i32 angle = NuAngAdd(first_angle,
                         IntersectionAngle((middle_radius - first_radius) / first_connection->horizontal_distance));
    NuVecRotateY(&a, &a, angle);
    NuVecRotateY(&b, &b, angle);
    NuVecAdd(&a, &a, &first->position);
    NuVecAdd(&b, &b, &first->position);
    NUVEC c = {-last_radius, 0.0f, 0.0f};
    NUVEC d = {-last_radius, 0.0f, second_connection->horizontal_distance};
    angle = NuAngSub(second_angle,
                     IntersectionAngle((middle_radius - last_radius) / second_connection->horizontal_distance));
    NuVecRotateY(&c, &c, angle);
    NuVecRotateY(&d, &d, angle);
    NuVecAdd(&c, &c, &last->position);
    NuVecAdd(&d, &d, &last->position);
    NUVEC first_point = a;
    NUVEC first_vector;
    NuVecSub(&first_vector, &b, &first_point);
    NUVEC second_point = c;
    NUVEC second_vector;
    NuVecSub(&second_vector, &d, &second_point);
    f32 s;
    f32 t;
    if (NuLineLineIntersect(&first_point, &first_vector, &second_point, &second_vector, &s, &t) != 0 && s >= 0.0f &&
        t >= 0.0f && s <= 1.0f && t <= 1.0f) {
        first_point.y = middle->position.y * s + (1.0f - s) * first->position.y;
        NuVecScale(&first_vector, &first_vector, s);
        NuVecAdd(result, &first_point, &first_vector);
        return 1;
    }
    return 0;
}

static i32 CalculateIntersection(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object,
                                 AIPATHCNX_s *current_connection, AIPATHCNX_s *target_connection) {
    if (packet->intersection_connection == current_connection &&
        packet->intersection_target_connection == target_connection) {
        return ((packet->movement_flags >> 5) | (packet->movement_flags >> 6)) & 1;
    }
    packet->movement_flags &= 0x9f;
    packet->intersection_connection = current_connection;
    packet->intersection_target_connection = target_connection;
    AIPATH *path = packet->path_info.path;
    AIPATHNODE *first;
    AIPATHNODE *middle;
    AIPATHNODE *last;
    i32 first_angle;
    i32 second_angle;
    if (current_connection->node_indices[0] == target_connection->node_indices[0]) {
        first = &path->nodes[current_connection->node_indices[1]];
        middle = &path->nodes[current_connection->node_indices[0]];
        last = &path->nodes[target_connection->node_indices[1]];
        first_angle = NuAngAdd(current_connection->rotation, 0x8000);
        second_angle = NuAngAdd(target_connection->rotation, 0x8000);
    } else if (current_connection->node_indices[0] == target_connection->node_indices[1]) {
        first = &path->nodes[current_connection->node_indices[1]];
        middle = &path->nodes[current_connection->node_indices[0]];
        last = &path->nodes[target_connection->node_indices[0]];
        first_angle = NuAngAdd(current_connection->rotation, 0x8000);
        second_angle = target_connection->rotation;
    } else if (current_connection->node_indices[1] == target_connection->node_indices[0]) {
        first = &path->nodes[current_connection->node_indices[0]];
        middle = &path->nodes[target_connection->node_indices[0]];
        last = &path->nodes[target_connection->node_indices[1]];
        first_angle = current_connection->rotation;
        second_angle = NuAngAdd(target_connection->rotation, 0x8000);
    } else if (current_connection->node_indices[1] == target_connection->node_indices[1]) {
        first = &path->nodes[current_connection->node_indices[0]];
        middle = &path->nodes[target_connection->node_indices[1]];
        last = &path->nodes[target_connection->node_indices[0]];
        first_angle = current_connection->rotation;
        second_angle = target_connection->rotation;
    } else {
        return 0;
    }
    if (WithinConnection(system, &last->position, path, current_connection, 1, current_connection,
                         packet->current_route, object->field_0x289, NULL, 0.0f, 1) != 0) {
        return 0;
    }
    i32 right = CalculateRightIntersection(object, current_connection, target_connection, first_angle, second_angle,
                                           first, middle, last, &packet->right_diversion);
    packet->movement_flags = (packet->movement_flags & 0xdf) | ((right & 1) << 5);
    i32 left = CalculateRightIntersection(object, target_connection, current_connection, second_angle, first_angle,
                                          last, middle, first, &packet->left_diversion);
    packet->movement_flags = (packet->movement_flags & 0xbf) | ((left & 1) << 6);
    return (left | (packet->movement_flags >> 5)) & 1;
}

void AISysGetPathPos2(AISYS_s *system, nuvec_s *position, AIPATHINFO_s *path_info, nuvec_s *path_position, AIPATH_s *,
                      i32) {
    if (position != NULL && system != NULL && path_info != NULL) {
        APIOBJECT_s object;
        AIPACKET_s packet;
        memset(&object, 0, sizeof(object));
        memset(&packet, 0, sizeof(packet));
        object.position = *position;
        object.ai = &packet;
        packet.terrain_origin = *position;
        AISysGetCharacterPathPos(system, &object, &packet, 0xff, 1);
        *path_info = packet.path_info;
        if (path_position != NULL) {
            *path_position = packet.last_path_position;
        }
    }
}

void FreeTorpedoPacket(TORPEDOPACKET **packet);
void RemoveGameObject(GameObject_s *object, i32 mode);

void ClearAICreatures() {
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.flags_low & 1) != 0 && (object->apiobj.field_0x1f4 & 0x400) != 0) {
            FreeTorpedoPacket(&object->torpedo);
            RemoveGameObject(object, 1);
        }
    }
}

void AIMoveCanReachPath(AISYS_s *, AIPATH_s *, AIPATH_s *) {
}

static AIPATHCNX *GetNextConnection(const AIPACKET *packet, i32 *direction) {
    if (packet->goal_path_node == NULL || packet->path_info.connection == NULL) {
        return NULL;
    }
    i32 current = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
    AIPATH *path = packet->path_info.path;
    AIPATHNODE *node = &path->nodes[current];
    i32 goal = packet->goal_path_node - path->nodes;
    if (static_cast<u32>(goal) > 0xff) {
        return NULL;
    }
    AIPATHCNX *connection = NULL;
    i32 next = path->route_matrix[current][goal];
    if (next < node->connection_count) {
        connection = node->connections[next];
        *direction = connection->node_indices[0] != current;
    }
    u8 route = packet->current_route;
    if (route == 0xff) {
        return connection;
    }
    if (((static_cast<u64>(static_cast<u16>(node->value_0x58)) >> route) & 1) == 0) {
        return NULL;
    }
    if (((static_cast<u64>(static_cast<u16>(node->value_0x5a)) >> route) & 1) != 0) {
        if (connection == NULL) {
            return NULL;
        }
        if (((static_cast<u64>(connection->route_mask) >> route) & 1) == 0) {
            return connection;
        }
    }
    AIPATHROUTE *route_info = &path->routes[route];
    if (((static_cast<u64>(static_cast<u16>(path->nodes[goal].value_0x58)) >> route) & 1) != 0) {
        next = route_info->route_nodes[route_info->node_routes[current]][route_info->node_routes[goal]];
    } else {
        if (route_info->character_count == 0) {
            return NULL;
        }
        f32 best_distance = 3.4028234663852886e+38f;
        i32 exit_node = 0;
        for (i32 i = 0; i < route_info->character_count; ++i) {
            f32 first = AIPathNodeDistanceToPathNode(packet->path_info.path, current, route_info->characters[i],
                                                     packet->current_route, 0);
            f32 second = AIPathNodeDistanceToPathNode(packet->path_info.path, route_info->characters[i], goal, 0xff,
                                                      static_cast<u32>(u64(1) << packet->current_route));
            f32 distance = first != 3.4028234663852886e+38f && second != 3.4028234663852886e+38f
                               ? first + second
                               : 3.4028234663852886e+38f;
            if (distance < best_distance) {
                best_distance = distance;
                exit_node = route_info->characters[i];
            }
        }
        if (best_distance == 3.4028234663852886e+38f) {
            return NULL;
        }
        next = route_info->route_nodes[route_info->node_routes[current]][route_info->node_routes[exit_node]];
    }
    if (next >= node->connection_count) {
        return NULL;
    }
    connection = node->connections[next];
    *direction = connection->node_indices[0] != current;
    return connection;
}

static void AIMoveFindDivertNode(AISYS_s *, AIPATH *path, AIPACKET *packet, NUVEC *destination) {
    if (path->special_route_count != 0 && (packet->navigation_flags & 4) != 0) {
        if (packet->divert_node >= path->node_count) {
            packet->divert_node = 0xff;
        }
        AIPATHNODELINK *links = path->special_routes;
        if (packet->divert_search_cursor >= path->special_route_count) {
            packet->divert_search_cursor = 0;
        }
        f32 best = 3.4028234663852886e+38f;
        if (packet->divert_node < path->node_count) {
            best = NuVecDistSqr(&path->nodes[packet->divert_node].position, destination, NULL);
        }
        u8 start = packet->divert_search_cursor;
        for (i32 i = 0; i < 4; ++i) {
            if (packet->divert_search_cursor != packet->divert_node) {
                f32 distance =
                    NuVecDistSqr(&path->nodes[links[packet->divert_search_cursor].type].position, destination, NULL);
                if (distance < best) {
                    best = distance;
                    packet->divert_node = links[packet->divert_search_cursor].type;
                }
            }
            ++packet->divert_search_cursor;
            if (packet->divert_search_cursor > path->special_route_count) {
                packet->divert_search_cursor = 0;
            }
            if (packet->divert_search_cursor == start) {
                break;
            }
        }
    } else {
        if (packet->divert_node >= path->node_count) {
            packet->divert_node = 0xff;
        }
        if (packet->divert_search_cursor >= path->node_count) {
            packet->divert_search_cursor = 0;
        }
        f32 best = 3.4028234663852886e+38f;
        if (packet->divert_node < path->node_count) {
            best = NuVecDistSqr(&path->nodes[packet->divert_node].position, destination, NULL);
        }
        for (i32 i = 0; i < 4; ++i) {
            f32 distance = NuVecDistSqr(&path->nodes[packet->divert_search_cursor].position, destination, NULL);
            if (distance < best) {
                best = distance;
                packet->divert_node = packet->divert_search_cursor;
            }
            ++packet->divert_search_cursor;
            if (packet->divert_search_cursor > path->node_count) {
                packet->divert_search_cursor = 0;
            }
        }
    }
}

static i32 AIMoveChooseExitNodePath(AISYS *system, AIPACKET *packet) {
    AIPATH *path = packet->path_info.path;
    AIPATHCNX *connection = packet->path_info.connection;
    AIPATHNODE *first = &path->nodes[connection->node_indices[0]];
    AIPATHNODE *second = &path->nodes[connection->node_indices[1]];
    f32 candidate_distance =
        NuVecDistSqr(&packet->owner->apiobj.position, &first->position, NULL) - first->radius_squared;
    f32 second_distance =
        NuVecDistSqr(&packet->owner->apiobj.position, &second->position, NULL) - second->radius_squared;
    AIPATHNODE *node = candidate_distance < second_distance ? first : second;
    if (node->special_type >= packet->path_info.path->special_route_count) {
        return 0;
    }
    AIPATHSPECIALROUTE *route =
        &system->path_sys->special_routes[packet->path_info.path->special_routes[node->special_type].node];
    if (route->path_count == 0) {
        return 0;
    }
    for (i32 i = 0; i < route->path_count; ++i) {
        if (route->paths[i] == packet->fallback_path_info.path) {
            AISysCharacterSetPath(packet, packet->fallback_path_info.path);
            return 1;
        }
    }
    AIPATH *best_path = NULL;
    f32 best_distance = 3.4028234663852886e+38f;
    for (i32 i = 0; i < route->path_count; ++i) {
        AIPATH *candidate = route->paths[i];
        f32 nearest_distance = 3.4028234663852886e+38f;
        i32 nearest_node = -1;
        for (i32 j = 0; j < candidate->special_route_count; ++j) {
            f32 distance = NuVecDistSqr(&candidate->nodes[candidate->special_routes[j].type].position,
                                        &packet->fallback_destination, NULL);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest_node = candidate->special_routes[j].type;
            }
        }
        if (nearest_node != -1) {
            candidate_distance = nearest_distance;
        }
        if (candidate_distance < best_distance) {
            best_distance = candidate_distance;
            best_path = route->paths[i];
        }
    }
    if (best_path == NULL) {
        return 0;
    }
    AISysCharacterSetPath(packet, best_path);
    return 1;
}

static i32 AIMoveAdjustDestinationPath(AISYS *system, AIPACKET *packet) {
    if (packet->path_info.path == packet->fallback_path_info.path && packet->fallback_path_info.connection != NULL) {
        return 0;
    }
    AIMoveFindDivertNode(system, packet->path_info.path, packet, &packet->fallback_destination);
    if (packet->divert_node >= packet->path_info.path->node_count) {
        return -1;
    }
    AIPATHNODE *node = &packet->path_info.path->nodes[packet->divert_node];
    if (node->connection_count == 0) {
        return -1;
    }
    packet->fallback_destination = node->position;
    packet->fallback_stopping_distance = 0.0f;
    packet->movement_parameter = NuFmax(node->radius - 1.0f, 1.0f);
    memset(&packet->fallback_path_info, 0, sizeof(packet->fallback_path_info));
    packet->fallback_path_info.path = packet->path_info.path;
    packet->fallback_path_info.connection = node->connections[0];
    packet->fallback_path_info.direction = node->connections[0]->node_indices[0] == packet->divert_node;
    packet->fallback_path_info.dist = node->connections[0]->node_indices[0] == packet->divert_node ? 0.0f : 1.0f;
    return 1;
}

void AIMoveToDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    i32 direction = 0;
    NUVEC saved_destination = packet->fallback_destination;
    f32 saved_stopping_distance = packet->fallback_stopping_distance;
    f32 saved_parameter = packet->movement_parameter;
    AIPATHINFO saved_path_info = packet->fallback_path_info;
    AIPATH *path = packet->path_info.path;
    AIPATHINFO &destination_path_info = packet->fallback_path_info;
    i32 diverted = 0;

    if (path != destination_path_info.path || destination_path_info.connection == NULL) {
        AIMoveFindDivertNode(system, path, packet, &packet->fallback_destination);
        if (packet->divert_node >= path->node_count || path->nodes[packet->divert_node].connection_count == 0) {
            packet->movement_destination = packet->owner->apiobj.position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
        AIPATHNODE *node = &path->nodes[packet->divert_node];
        packet->fallback_destination = node->position;
        packet->fallback_stopping_distance = 0.0f;
        packet->movement_parameter = NuFmax(node->radius - 1.0f, 1.0f);
        memset(&packet->fallback_path_info, 0, sizeof(packet->fallback_path_info));
        packet->fallback_path_info.path = packet->path_info.path;
        packet->fallback_path_info.connection = node->connections[0];
        packet->fallback_path_info.direction = node->connections[0]->node_indices[0] == packet->divert_node;
        packet->fallback_path_info.dist = node->connections[0]->node_indices[0] == packet->divert_node ? 0.0f : 1.0f;
        diverted = 1;
    }
    AIPATHCNX *connection = packet->path_info.connection;
    NUVEC destination_delta;
    f32 destination_distance_squared =
        NuVecDistSqr(&packet->fallback_destination, &object->position, &destination_delta);
    if ((object->supporting_platform_id == -1 || (path->nodes[connection->node_indices[0]].has_special != 0 &&
                                                  path->nodes[connection->node_indices[1]].has_special != 0)) &&
        packet->movement_parameter != 0.0f && destination_delta.y < 0.5f && destination_delta.y > -0.5f &&
        destination_distance_squared < packet->movement_parameter * packet->movement_parameter) {
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
        }
    }

    if (connection == NULL || path == NULL || destination_path_info.connection == NULL) {
        packet->movement_destination = packet->owner->apiobj.position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    AIPATHCNX *destination_connection = destination_path_info.connection;
    // The target first adopts the destination connection when the character's
    // terrain origin is already inside its corridor. Its direction is chosen
    // from the endpoint shared with the current connection.
    if (connection != destination_connection &&
        WithinConnection(system, &packet->terrain_origin, path, destination_connection, checks, connection,
                         packet->current_route, object->field_0x289, NULL, object->collision_radius, 0) != 0) {
        const u8 destination_node_a = destination_connection->node_indices[0];
        direction =
            destination_node_a == connection->node_indices[0] || destination_node_a == connection->node_indices[1] ? 0
                                                                                                                   : 1;
        AISysCharacterSetPathCnx(packet, &object->position, destination_connection, direction);
        if (packet->path_info.connection == destination_connection) {
            packet->movement_event_flags |= AIPACKET_PATH_CONNECTION_CHANGED;
            connection = packet->path_info.connection;
        }
    }

    if (connection != destination_connection) {
        const u8 destination_node_a = destination_connection->node_indices[0];
        const u8 destination_node_b = destination_connection->node_indices[1];
        // Match the target's ordinary endpoint solver: compare all four graph
        // routes between the current and destination connection endpoints, adding
        // the partial distance along each connection. This also permits a clean
        // reversal when the valid route leaves behind the character.
        f32 current_dist = packet->path_info.dist;
        if (current_dist > 1.0f) {
            current_dist = 1.0f;
        } else if (current_dist < 0.0f) {
            current_dist = 0.0f;
        }
        f32 distance_to_current_a = NuFabs(connection->distance * current_dist);
        f32 distance_to_current_b = NuFabs(connection->distance * (1.0f - current_dist));
        direction = packet->path_info.direction;
        u8 goal_index = destination_node_a;
        f32 best_route_distance = 3.4028234663852886e+38f;
        u32 attempted_routes = 0;
        if (packet->current_route != 0xff) {
            packet->runtime_flags |= 0x10;
        }
    retry_route:
        f32 destination_dist = destination_path_info.dist;
        f32 distance_from_a = NuFabs(destination_connection->distance * destination_dist);
        f32 distance_from_b = NuFabs(destination_connection->distance * (1.0f - destination_dist));
        if (packet->current_route != 0xff &&
            ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) == 0) {
            AIPATHNODE *destination_a = &path->nodes[destination_node_a];
            AIPATHNODE *destination_b = &path->nodes[destination_node_b];
            if (((static_cast<u64>(destination_a->value_0x58 & ~destination_a->value_0x5a) >> packet->current_route) &
                 1) != 0) {
                distance_from_a = 3.4028234663852886e+38f;
            }
            if (((static_cast<u64>(destination_b->value_0x58 & ~destination_b->value_0x5a) >> packet->current_route) &
                 1) != 0) {
                distance_from_b = 3.4028234663852886e+38f;
            }
        }

        const f32 current_endpoint_distances[2] = {distance_to_current_a, distance_to_current_b};
        if ((connection->traversal_flags[packet->path_info.direction] & 0x40000000) == 0) {
            f32 candidate_a = 3.4028234663852886e+38f;
            if (distance_from_a != 3.4028234663852886e+38f &&
                current_endpoint_distances[packet->path_info.direction == 0] != 3.4028234663852886e+38f) {
                candidate_a =
                    AIPathNodeDistanceToPathNode(path, connection->node_indices[packet->path_info.direction == 0],
                                                 destination_node_a, packet->current_route, 0);
                if (candidate_a != 3.4028234663852886e+38f) {
                    candidate_a += current_endpoint_distances[packet->path_info.direction == 0] + distance_from_a;
                }
            }
            if (best_route_distance > candidate_a) {
                best_route_distance = candidate_a;
                direction = packet->path_info.direction;
                goal_index = destination_node_a;
                packet->runtime_flags &= static_cast<u8>(~0x10);
            }
            f32 candidate_b = 3.4028234663852886e+38f;
            if (distance_from_b != 3.4028234663852886e+38f &&
                current_endpoint_distances[packet->path_info.direction == 0] != 3.4028234663852886e+38f) {
                candidate_b =
                    AIPathNodeDistanceToPathNode(path, connection->node_indices[packet->path_info.direction == 0],
                                                 destination_node_b, packet->current_route, 0);
                if (candidate_b != 3.4028234663852886e+38f) {
                    candidate_b += current_endpoint_distances[packet->path_info.direction == 0] + distance_from_b;
                }
            }
            if (best_route_distance > candidate_b) {
                best_route_distance = candidate_b;
                direction = packet->path_info.direction;
                goal_index = destination_node_b;
                packet->runtime_flags &= static_cast<u8>(~0x10);
            }
        }
        if ((connection->traversal_flags[packet->path_info.direction == 0] & 0x40000000) == 0) {
            f32 candidate_a = 3.4028234663852886e+38f;
            if (distance_from_a != 3.4028234663852886e+38f &&
                current_endpoint_distances[packet->path_info.direction] != 3.4028234663852886e+38f) {
                candidate_a = AIPathNodeDistanceToPathNode(path, connection->node_indices[packet->path_info.direction],
                                                           destination_node_a, packet->current_route, 0);
                if (candidate_a != 3.4028234663852886e+38f) {
                    candidate_a += current_endpoint_distances[packet->path_info.direction] + distance_from_a;
                }
            }
            if (best_route_distance > candidate_a) {
                best_route_distance = candidate_a;
                direction = packet->path_info.direction == 0;
                goal_index = destination_node_a;
                packet->runtime_flags &= static_cast<u8>(~0x10);
            }
            f32 candidate_b = 3.4028234663852886e+38f;
            if (distance_from_b != 3.4028234663852886e+38f &&
                current_endpoint_distances[packet->path_info.direction] != 3.4028234663852886e+38f) {
                candidate_b = AIPathNodeDistanceToPathNode(path, connection->node_indices[packet->path_info.direction],
                                                           destination_node_b, packet->current_route, 0);
                if (candidate_b != 3.4028234663852886e+38f) {
                    candidate_b += current_endpoint_distances[packet->path_info.direction] + distance_from_b;
                }
            }
            if (best_route_distance > candidate_b) {
                best_route_distance = candidate_b;
                direction = packet->path_info.direction == 0;
                goal_index = destination_node_b;
                packet->runtime_flags &= static_cast<u8>(~0x10);
            }
        }

        if ((packet->runtime_flags & 0x10) != 0) {
            if (packet->current_route != 0xff) {
                attempted_routes |= 1u << (packet->current_route & 31);
            }
            AISysFindRoute(packet);
            if ((attempted_routes & (1u << (packet->current_route & 31))) != 0) {
                return;
            }
            goto retry_route;
        }

        packet->goal_path_node = &path->nodes[goal_index];
        AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
        AIPATHCNX *next_connection = GetNextConnection(packet, &direction);
        if (next_connection == NULL) {
            const u32 flags = destination_connection->traversal_flags[0] | destination_connection->traversal_flags[1];
            const u32 capabilities = flags & 0xdfffffff;
            if ((flags & 0xd8000000) == 0 && (capabilities == 0 || (packet->capabilities & capabilities) != 0) &&
                WithinConnection(system, &packet->terrain_origin, packet->path_info.path, destination_connection,
                                 checks, packet->path_info.connection, packet->current_route, object->field_0x289, NULL,
                                 object->collision_radius, 0) != 0) {
                AISysCharacterSetPathCnx(packet, &object->position, destination_connection, 0);
            }
        } else {
            do {
                if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, next_connection, checks,
                                     packet->path_info.connection, packet->current_route, object->field_0x289, NULL,
                                     object->collision_radius, 0) == 0) {
                    break;
                }
                if (connection == destination_connection) {
                    break;
                }
                AISysCharacterSetPathCnx(packet, &object->position, next_connection, direction);
                if (packet->path_info.connection != next_connection || packet->path_info.direction != direction) {
                    break;
                }
                connection = next_connection;
                next_connection = GetNextConnection(packet, &direction);
            } while (next_connection != NULL && next_connection != connection);
        }
    }
    connection = packet->path_info.connection;
    const u32 current_traversal_flags = packet->path_info.connection->traversal_flags[packet->path_info.direction];
    if ((current_traversal_flags & 0xd8000000) != 0) {
        packet->movement_destination =
            path->nodes[packet->path_info.connection->node_indices[packet->path_info.direction]].position;
        packet->runtime_flags |= AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
        packet->movement_stopping_distance = 0.0f;
        packet->frame_state = current_traversal_flags;
        return;
    }

    if (connection == destination_connection &&
        (packet->current_route == 0xff ||
         ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) != 0)) {
        if (!packet->path_info.on_path || packet->movement_parameter == 0.0f) {
            packet->movement_destination = packet->fallback_destination;
            packet->movement_stopping_distance = packet->fallback_stopping_distance;
            return;
        }
        if (packet->movement_parameter * packet->movement_parameter > destination_distance_squared) {
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
        f32 distance = NuFsqrt(destination_distance_squared);
        NuVecScale(&destination_delta, &destination_delta, packet->movement_parameter / distance);
        packet->movement_destination.x = packet->fallback_destination.x - destination_delta.x;
        packet->movement_destination.y = packet->fallback_destination.y;
        packet->movement_destination.z = packet->fallback_destination.z - destination_delta.z;
        packet->movement_stopping_distance =
            packet->fallback_stopping_distance < packet->movement_parameter ? 0.0f : packet->fallback_stopping_distance;
        return;
    }

    AIPATHNODE *shared_node = NULL;
    if (connection->node_indices[0] == destination_connection->node_indices[0]) {
        shared_node = &path->nodes[connection->node_indices[0]];
        direction = 0;
    } else if (connection->node_indices[0] == destination_connection->node_indices[1]) {
        shared_node = &path->nodes[connection->node_indices[0]];
        direction = 1;
    } else if (connection->node_indices[1] == destination_connection->node_indices[0]) {
        shared_node = &path->nodes[connection->node_indices[1]];
        direction = 0;
    } else if (connection->node_indices[1] == destination_connection->node_indices[1]) {
        shared_node = &path->nodes[connection->node_indices[1]];
        direction = 1;
    }
    AIPATHCNX *target_connection;
    if (shared_node != NULL &&
        (packet->current_route == 0xff ||
         ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) != 0 ||
         ((static_cast<u64>(shared_node->value_0x5a) >> packet->current_route) & 1) != 0)) {
        target_connection = destination_connection;
    } else {
        target_connection = GetNextConnection(packet, &direction);
    }
    if (target_connection != NULL) {
        const u32 flags = target_connection->traversal_flags[direction];
        if ((flags & 0xdfffffff) != 0 && (flags & 0xd8000000) != 0) {
            packet->movement_destination = object->position;
            packet->runtime_flags |= AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
            packet->movement_stopping_distance = 0.0f;
            packet->frame_state = flags;
            return;
        }
        if ((flags & 0xdfffffff) == 0 || (packet->capabilities & flags) != 0) {
            if ((flags & 0xdfffffff) != 0) {
                packet->navigation_flags |= 1;
            }
            NUVEC target;
            if (target_connection == destination_connection) {
                target = packet->fallback_destination;
            } else {
                target = path->nodes[target_connection->node_indices[direction]].position;
                packet->runtime_flags |= 8;
            }
            if (WithinConnection(system, &target, packet->path_info.path, connection, checks,
                                 packet->path_info.connection, packet->current_route, object->field_0x289, NULL,
                                 object->collision_radius, 0) == 0) {
                if (CalculateIntersection(system, packet, object, connection, target_connection) == 0) {
                    target = path->nodes[target_connection->node_indices[direction]].position;
                } else {
                    NUVEC delta;
                    if ((packet->movement_flags & 0x40) != 0) {
                        NuVecSub(&delta, &packet->left_diversion, &object->position);
                        i32 left_angle = NuAtan2D(delta.x, delta.z);
                        NuVecSub(&delta, &target, &object->position);
                        if (NuAngSub(NuAtan2D(delta.x, delta.z), left_angle) < 0) {
                            target = packet->left_diversion;
                            packet->runtime_flags |= 8;
                            goto corner_destination;
                        }
                    }
                    if ((packet->movement_flags & 0x20) != 0) {
                        NuVecSub(&delta, &packet->right_diversion, &object->position);
                        i32 right_angle = NuAtan2D(delta.x, delta.z);
                        NuVecSub(&delta, &target, &object->position);
                        if (NuAngSub(NuAtan2D(delta.x, delta.z), right_angle) > 0) {
                            target = packet->right_diversion;
                            packet->runtime_flags |= 8;
                        }
                    }
                }
            }
        corner_destination:
            packet->movement_destination = target;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
        packet->runtime_flags |= 0x60;
        packet->frame_state = flags;
    }
    packet->movement_destination = path->nodes[connection->node_indices[packet->path_info.direction == 0]].position;
    packet->movement_stopping_distance = 0.0f;
    NUVEC waypoint_delta;
    if (NuVecDistSqr(&packet->movement_destination, &object->position, &waypoint_delta) < 1.0f) {
        packet->movement_destination = object->position;
    }
    packet->runtime_flags |= AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
}

void AIRetreatFromDestination(AISYS_s *, AIPACKET_s *, APIOBJECT_s *, i32) {
}

void AIRetreatFromDestination2(AISYS_s *, AIPACKET_s *, APIOBJECT_s *, i32) {
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

void AISysNodeCanReachThisJumpConnection(GameObject_s &, AIPATH_s &, unsigned char, AIPATHCNX_s &, i32) {
}

void AISysCharacterCanReachThisJumpConnection(GameObject_s &, AIPATH_s &, AIPATHCNX_s &, i32) {
}

void AICircle(AISYS_s *, AIPACKET_s *, APIOBJECT_s *, i32) {
}

void AIWander(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    if (packet == NULL || object == NULL || packet->path_info.path == NULL || packet->path_info.path->nodes == NULL ||
        packet->path_info.connection == NULL) {
        if (packet != NULL && object != NULL) {
            packet->movement_destination = object->position;
            packet->movement_stopping_distance = 0.0f;
            packet->movement_target = NULL;
            packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED;
        }
        return;
    }

    AIPATHCNX *target_connection = packet->movement_target;
    AIPATHCNX *current_connection = packet->path_info.connection;
    const u8 current_node_index = current_connection->node_indices[packet->path_info.direction == 0];
    if (current_node_index >= packet->path_info.path->node_count) {
        packet->movement_destination = object->position;
        packet->movement_stopping_distance = 0.0f;
        packet->movement_target = NULL;
        packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED;
        return;
    }

    if (target_connection != NULL && target_connection->direction_a != current_node_index &&
        target_connection->direction_b != current_node_index) {
        packet->movement_target = NULL;
        target_connection = NULL;
    }

    if (target_connection == NULL) {
        AIPATH *path = packet->path_info.path;
        AIPATHNODE *current_node = &path->nodes[current_node_index];
        const i32 connection_count = current_node->connection_count;

        if (connection_count != 0 && current_node->connections != NULL) {
            const i32 first_connection = static_cast<i32>(NuRandInt() % connection_count);
            for (i32 offset = 0; offset < connection_count; ++offset) {
                AIPATHCNX *candidate = current_node->connections[(first_connection + offset) % connection_count];
                if (candidate == NULL ||
                    (candidate->direction_a != current_node_index && candidate->direction_b != current_node_index)) {
                    continue;
                }
                packet->movement_target = candidate;
                target_connection = candidate;

                if (candidate == current_connection) {
                    packet->movement_target = NULL;
                    target_connection = NULL;
                    continue;
                }

                if (packet->current_route != 0xff &&
                    ((static_cast<u64>(candidate->route_mask) >> packet->current_route) & 1) == 0 &&
                    ((static_cast<u64>(static_cast<u16>(current_node->value_0x5a)) >> packet->current_route) & 1) ==
                        0) {
                    packet->movement_target = NULL;
                    target_connection = NULL;
                    continue;
                }

                const u8 direction = current_node_index != candidate->direction_a;
                packet->movement_target_direction = direction;
                const u32 traversal_flags = candidate->traversal_flags[direction];
                if (traversal_flags == 0) {
                    break;
                }

                packet->movement_target = NULL;
                target_connection = NULL;
            }
        }

        if (target_connection == NULL) {
            current_connection = packet->path_info.connection;
            const u8 node_index = current_connection->node_indices[packet->path_info.direction == 0];
            AIPATHNODE *node = &packet->path_info.path->nodes[node_index];
            NUVEC delta;
            const f32 distance_squared = NuVecXZDistSqr(&object->position, &node->position, &delta);

            if (distance_squared < node->radius_squared) {
                AISysCharacterSetPathCnx(packet, &object->position, current_connection,
                                         packet->path_info.direction == 0);
                current_connection = packet->path_info.connection;
                const u8 updated_node_index = current_connection->node_indices[packet->path_info.direction == 0];
                node = &packet->path_info.path->nodes[updated_node_index];
                packet->goal_path_node = node;
            }

            packet->movement_destination = node->position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
    }

    packet->field_0x1e6 |= AIPACKET_RUNTIME_SPECIAL_MOVE;
    if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, target_connection, checks,
                         current_connection, packet->current_route, object->field_0x289, NULL, object->collision_radius,
                         0) != 0) {
        AISysCharacterSetPathCnx(packet, &object->position, target_connection, packet->movement_target_direction);

        current_connection = packet->path_info.connection;
        const u8 node_index = current_connection->node_indices[packet->path_info.direction == 0];
        AIPATHNODE *node = &packet->path_info.path->nodes[node_index];
        packet->goal_path_node = node;
        packet->movement_destination = node->position;
        packet->movement_stopping_distance = 0.0f;
        packet->movement_target = NULL;
        return;
    }

    const u8 destination_node_index = target_connection->node_indices[packet->movement_target_direction == 0];
    if (destination_node_index >= packet->path_info.path->node_count) {
        packet->movement_destination = object->position;
        packet->movement_stopping_distance = 0.0f;
        packet->movement_target = NULL;
        packet->runtime_flags |= AIPACKET_RUNTIME_PATH_BLOCKED;
        return;
    }
    packet->movement_destination = packet->path_info.path->nodes[destination_node_index].position;
    packet->movement_stopping_distance = 0.0f;

    if (CalculateIntersection(system, packet, object, current_connection, target_connection) != 0) {
        NUVEC delta;

        if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0) {
            NuVecSub(&delta, &packet->left_diversion, &object->position);
            const NUANG diversion_angle = NuAtan2D(delta.x, delta.z);
            NuVecSub(&delta, &packet->movement_destination, &object->position);
            const NUANG destination_angle = NuAtan2D(delta.x, delta.z);
            if (NuAngSub(destination_angle, diversion_angle) < 0) {
                packet->movement_destination = packet->left_diversion;
                return;
            }
        }

        if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) {
            NuVecSub(&delta, &packet->right_diversion, &object->position);
            const NUANG diversion_angle = NuAtan2D(delta.x, delta.z);
            NuVecSub(&delta, &packet->movement_destination, &object->position);
            const NUANG destination_angle = NuAtan2D(delta.x, delta.z);
            if (NuAngSub(destination_angle, diversion_angle) > 0) {
                packet->movement_destination = packet->right_diversion;
            }
        }
    }
}
