#include "editor/edpath.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include <float.h>
#include <string.h>

static i32 iterator_count;
static f32 ***distance_tables;

static void pathEditorCalcRouteIterator(AIPATH_s *path, f32 *distances, u8 *visited, i32 previous, i32 current,
                                        f32 distance, i32 route_mask) {
    ++iterator_count;
    if ((visited[current / 8] & (1 << (current % 8))) != 0) {
        return;
    }
    visited[current / 8] |= 1 << (current % 8);
    AIPATHNODE_s *node = &path->nodes[current];
    NUVEC difference;
    distance += NuVecDist(&path->nodes[previous].position, &node->position, &difference);
    if (distances[current] >= distance) {
        distances[current] = distance;
        if (route_mask != 0) {
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX_s *connection = node->connections[index];
                bool direction = connection->node_indices[0] != current;
                if ((connection->traversal_flags[direction] & 0x40000000) == 0 &&
                    (connection->route_mask & route_mask) != 0) {
                    pathEditorCalcRouteIterator(path, distances, visited, current, connection->node_indices[!direction],
                                                distance, route_mask);
                }
            }
        } else {
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX_s *connection = node->connections[index];
                bool direction = connection->node_indices[0] != current;
                if ((connection->traversal_flags[direction] & 0x40000000) == 0) {
                    pathEditorCalcRouteIterator(path, distances, visited, current, connection->node_indices[!direction],
                                                distance, 0);
                }
            }
        }
    }
    visited[current / 8] &= ~(1 << (current % 8));
}

static __used__ f32 **pathEditorCalculateDistanceTable(AIPATH_s *path, i32 route_mask, variptr_u *cursor,
                                                       variptr_u *end) {
    if (distance_tables == 0 || path->nodes == nullptr) {
        return nullptr;
    }
    f32 **table = (f32 **)AISysBufferAlloc(cursor, end, path->node_count * sizeof(f32 *));
    if (table == nullptr) {
        return nullptr;
    }
    memset(table, 0, path->node_count * sizeof(f32 *));
    for (i32 row = 0; row < path->node_count; ++row) {
        f32 *distances = (f32 *)AISysBufferAlloc(cursor, end, path->node_count * sizeof(f32));
        table[row] = distances;
        for (i32 column = 0; column < path->node_count; ++column) {
            distances[column] = FLT_MAX;
        }
    }
    for (i32 row = 0; row < path->node_count; ++row) {
        u8 visited[32];
        memset(visited, 0, sizeof(visited));
        pathEditorCalcRouteIterator(path, table[row], visited, row, row, 0.0f, route_mask);
    }
    return table;
}

// The route records use a dense route index, while the editor keeps sixteen
// independently switchable slots.  The original helper first marks both ends
// of every route connection, then builds the compact node lookup and matrix.
static void pathEditorCreateSpecialRouteData(AIPATH_s *path, i32, EDAIPATH_s *editor_path, VARIPTR *cursor,
                                             VARIPTR *end) {
    i32 route_indices[16] = {};
    u8 route_members[16][32] = {};
    for (i32 slot = 0; slot < 16; ++slot) {
        if (editor_path->routes[slot].flags & 1) {
            route_indices[slot] = path->route_count++;
        }
    }
    if (path->route_count == 0) {
        return;
    }
    path->routes = (AIPATHROUTE_s *)AISysBufferAlloc(cursor, end, path->route_count * sizeof(AIPATHROUTE_s));
    memset(path->routes, 0, path->route_count * sizeof(AIPATHROUTE_s));
    for (i32 slot = 0; slot < 16; ++slot) {
        EDAIPATHROUTE_s *editor_route = &editor_path->routes[slot];
        if (!(editor_route->flags & 1)) {
            continue;
        }
        AIPATHROUTE_s *route = &path->routes[route_indices[slot]];
        i32 name_length = strlen(editor_route->name);
        if (name_length != 0) {
            route->name = (char *)AISysBufferAlloc(cursor, end, name_length + 1);
            strcpy(route->name, editor_route->name);
        }
        route->character_masks[0] = (editor_route->user_mask & (1ULL << 63)) ? ~0ULL : editor_route->user_mask;
        route->character_masks[1] = editor_route->user_mask;
    }

    for (EDAIPATHNODE_s *editor_node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes);
         editor_node != nullptr;
         editor_node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &editor_node->link)) {
        AIPATHNODE_s *runtime_node = &path->nodes[editor_node->index];
        for (i32 slot = 0; slot < 16; ++slot) {
            if (editor_node->route_mask & (1ULL << slot)) {
                runtime_node->route_boundary_mask |= 1ULL << route_indices[slot];
            }
        }
        for (i32 slot = 0; slot < 8; ++slot) {
            EDAIPATHCNX_s *editor_connection = &editor_node->connections[slot];
            if (editor_connection->node == nullptr || editor_connection->route_mask == 0) {
                continue;
            }
            for (i32 edge = 0; edge < runtime_node->connection_count; ++edge) {
                AIPATHCNX_s *connection = runtime_node->connections[edge];
                if (connection->node_indices[0] != editor_connection->node->index &&
                    connection->node_indices[1] != editor_connection->node->index) {
                    continue;
                }
                for (i32 route_slot = 0; route_slot < 16; ++route_slot) {
                    if (editor_connection->route_mask & (1ULL << route_slot)) {
                        i32 route_index = route_indices[route_slot];
                        u64 bit = 1ULL << route_index;
                        if ((connection->route_mask & bit) == 0) {
                            connection->route_mask |= bit;
                            for (i32 endpoint = 0; endpoint < 2; ++endpoint) {
                                u8 node_index = connection->node_indices[endpoint];
                                route_members[route_index][node_index >> 3] |= 1u << (node_index & 7);
                            }
                        }
                    }
                }
            }
        }
    }

    for (i32 route_index = 0; route_index < path->route_count; ++route_index) {
        AIPATHROUTE_s *route = &path->routes[route_index];
        for (i32 node = 0; node < path->node_count; ++node) {
            if (route_members[route_index][node >> 3] & (1u << (node & 7))) {
                ++route->route_count;
            }
            if (path->nodes[node].route_boundary_mask & (1ULL << route_indices[route_index])) {
                ++route->exit_node_count;
            }
        }
        if (path->node_count != 0) {
            route->node_routes = (u8 *)AISysBufferAlloc(cursor, end, path->node_count);
            memset(route->node_routes, 0, path->node_count);
        }
        if (route->route_count != 0) {
            route->node_directions = (u8 *)AISysBufferAlloc(cursor, end, route->route_count);
            memset(route->node_directions, 0, route->route_count);
        }
        if (route->exit_node_count != 0) {
            route->exit_nodes = (u8 *)AISysBufferAlloc(cursor, end, route->exit_node_count);
            memset(route->exit_nodes, 0, route->exit_node_count);
        }
        i32 member_index = 0;
        i32 exit_index = 0;
        for (i32 node = 0; node < path->node_count; ++node) {
            route->node_routes[node] = 0xff;
            if (!(route_members[route_index][node >> 3] & (1u << (node & 7)))) {
                continue;
            }
            route->node_routes[node] = member_index;
            path->nodes[node].route_membership_mask |= 1ULL << route_indices[route_index];
            route->node_directions[member_index++] = node;
            if ((path->nodes[node].route_boundary_mask >> route_indices[route_index]) & 1ULL) {
                route->exit_nodes[exit_index++] = node;
            }
        }
        if (route->route_count == 0) {
            continue;
        }
        route->route_nodes = (u8 **)AISysBufferAlloc(cursor, end, route->route_count * sizeof(u8 *));
        memset(route->route_nodes, 0, route->route_count * sizeof(u8 *));
        for (i32 source = 0; source < route->route_count; ++source) {
            route->route_nodes[source] = (u8 *)AISysBufferAlloc(cursor, end, route->route_count);
            memset(route->route_nodes[source], 0, route->route_count);
        }
        // The distance and full-node next-hop tables are temporary. Keep their
        // allocations beyond the persistent route data without advancing it.
        VARIPTR scratch = *cursor;
        f32 **distances = pathEditorCalculateDistanceTable(path, 1ULL << route_index, &scratch, end);
        if (distances == nullptr) {
            continue;
        }
        u8 **next_hops = (u8 **)AISysBufferAlloc(&scratch, end, path->node_count * sizeof(u8 *));
        memset(next_hops, 0, path->node_count * sizeof(u8 *));
        for (i32 source = 0; source < path->node_count; ++source) {
            next_hops[source] = (u8 *)AISysBufferAlloc(&scratch, end, path->node_count);
            memset(next_hops[source], 0, path->node_count);
        }
        for (i32 source = 0; source < path->node_count; ++source) {
            for (i32 destination = 0; destination < path->node_count; ++destination) {
                next_hops[source][destination] = 0xff;
                if (source == destination) {
                    continue;
                }
                f32 best = FLT_MAX;
                AIPATHNODE_s *node = &path->nodes[source];
                for (i32 edge = 0; edge < node->connection_count; ++edge) {
                    AIPATHCNX_s *connection = node->connections[edge];
                    i32 direction = connection->node_indices[0] != source;
                    if (connection->traversal_flags[direction] & 0x40000000) {
                        continue;
                    }
                    if (!(connection->route_mask & (1ULL << route_index))) {
                        continue;
                    }
                    i32 neighbor = connection->node_indices[!direction];
                    f32 distance = distances[source][neighbor] + distances[neighbor][destination];
                    if (distance < best) {
                        best = distance;
                        next_hops[source][destination] = edge;
                    }
                }
            }
        }
        for (i32 source = 0; source < route->route_count; ++source) {
            for (i32 destination = 0; destination < route->route_count; ++destination) {
                route->route_nodes[source][destination] =
                    next_hops[route->node_directions[source]][route->node_directions[destination]];
            }
        }
    }
}

extern "C" {
    AIPATHSYS_s *pathEditorCreateData(VARIPTR *cursor, VARIPTR *end, void **scratch_cursor, void **scratch_end) {
        iterator_count = 0;
        VARIPTR scratch;
        scratch.void_ptr = *scratch_cursor;
        if (aieditor->current_path == nullptr) {
            distance_tables = nullptr;
            return nullptr;
        }
        EDAIPATH_s *first_editor_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        if (first_editor_path == nullptr) {
            distance_tables = nullptr;
            return nullptr;
        }

        i32 path_count = 0;
        for (EDAIPATH_s *path = first_editor_path; path != nullptr;
             path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link)) {
            path->draw_index = path_count++;
        }
        AIPATHSYS_s *system = (AIPATHSYS_s *)AISysBufferAlloc(cursor, end, sizeof(AIPATHSYS_s));
        if (system == nullptr) {
            distance_tables = nullptr;
            return nullptr;
        }
        memset(system, 0, sizeof(*system));
        system->path_count = path_count;
        distance_tables = nullptr;
        VARIPTR *scratch_limit = reinterpret_cast<VARIPTR *>(scratch_end);
        if (system->path_count != 0) {
            distance_tables = (f32 ***)AISysBufferAlloc(&scratch, scratch_limit, system->path_count * sizeof(f32 **));
            memset(distance_tables, 0, system->path_count * sizeof(f32 **));
        }

        system->special_route_count = 0;
        for (EDAISHAREDPATHNODE_s *shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->shared_path_nodes);
             shared != nullptr;
             shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetNext(&aieditor->shared_path_nodes, &shared->link)) {
            shared->runtime_index = system->special_route_count++;
        }
        if (system->special_route_count != 0) {
            system->special_routes = (AIPATHSPECIALROUTE_s *)AISysBufferAlloc(
                &scratch, scratch_limit, system->special_route_count * sizeof(AIPATHSPECIALROUTE_s));
            memset(system->special_routes, 0, system->special_route_count * sizeof(AIPATHSPECIALROUTE_s));
            for (EDAISHAREDPATHNODE_s *shared =
                     (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->shared_path_nodes);
                 shared != nullptr;
                 shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetNext(&aieditor->shared_path_nodes, &shared->link)) {
                AIPATHSPECIALROUTE_s *route = &system->special_routes[shared->runtime_index];
                route->paths =
                    (AIPATH_s **)AISysBufferAlloc(&scratch, scratch_limit, shared->reference_count * sizeof(AIPATH_s));
                memset(route->paths, 0, shared->reference_count * sizeof(AIPATH_s));
            }
        }
        system->paths = (AIPATH_s **)AISysBufferAlloc(cursor, end, system->path_count * sizeof(AIPATH_s *));
        if (system->paths == nullptr) {
            distance_tables = nullptr;
            return system;
        }
        memset(system->paths, 0, system->path_count * sizeof(AIPATH_s *));
        i32 path_index = 0;
        for (EDAIPATH_s *editor_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); editor_path != nullptr;
             editor_path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &editor_path->link), ++path_index) {
            system->paths[path_index] = (AIPATH_s *)AISysBufferAlloc(cursor, end, sizeof(AIPATH_s));
            AIPATH_s *path = system->paths[path_index];
            if (path == nullptr) {
                continue;
            }
            memset(path, 0, sizeof(*path));
            strcpy(path->name, editor_path->name);
            path->index = path_index;

            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes); node != nullptr;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
                node->index = path->node_count++;
                for (i32 slot = 0; slot < 8; ++slot) {
                    if (node->connections[slot].node != nullptr) {
                        ++path->connection_count;
                    }
                }
                if (node->shared_node != nullptr) {
                    ++path->special_route_count;
                }
            }
            if (path->connection_count != 0) {
                path->connection_count /= 2;
                path->connections =
                    (AIPATHCNX_s *)AISysBufferAlloc(cursor, end, path->connection_count * sizeof(AIPATHCNX_s));
                if (path->connections != nullptr) {
                    memset(path->connections, 0, path->connection_count * sizeof(AIPATHCNX_s));
                }
            }
            if (path->special_route_count != 0) {
                path->special_routes = (AIPATHNODELINK_s *)AISysBufferAlloc(
                    cursor, end, path->special_route_count * sizeof(AIPATHNODELINK_s));
                memset(path->special_routes, 0, path->special_route_count * sizeof(AIPATHNODELINK_s));
                path->special_route_count = 0;
            }
            if (path->node_count != 0) {
                path->nodes = (AIPATHNODE_s *)AISysBufferAlloc(cursor, end, path->node_count * sizeof(AIPATHNODE_s));
                if (path->nodes != nullptr) {
                    memset(path->nodes, 0, path->node_count * sizeof(AIPATHNODE_s));

                    i32 connection_index = 0;
                    for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes);
                         node != nullptr;
                         node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
                        AIPATHNODE_s *runtime = &path->nodes[node->index];
                        i32 length = strlen(node->name);
                        if (length != 0) {
                            runtime->name = (char *)AISysBufferAlloc(cursor, end, length + 1);
                            strcpy(runtime->name, node->name);
                        }
                        runtime->position = node->position;
                        runtime->radius = node->radius;
                        runtime->radius_squared = node->radius * node->radius;
                        runtime->runtime_flags = node->flags;
                        runtime->special_handle = node->special;
                        if (NuSpecialExistsFn(&runtime->special_handle)) {
                            runtime->has_special = 1;
                        }
                        runtime->special_position = node->special_position;
                        runtime->min_height = runtime->position.y + node->lower_height;
                        runtime->max_height = runtime->position.y + node->upper_height;
                        runtime->min_height_offset = runtime->min_height - runtime->position.y;
                        runtime->max_height_offset = runtime->max_height - runtime->position.y;
                        for (i32 slot = 0; slot < 8; ++slot) {
                            if (node->connections[slot].node != nullptr) {
                                ++runtime->connection_count;
                            }
                        }
                        if (path->connections != nullptr) {
                            if (runtime->connection_count != 0) {
                                runtime->connections = (AIPATHCNX_s **)AISysBufferAlloc(
                                    cursor, end, runtime->connection_count * sizeof(AIPATHCNX_s *));
                                if (runtime->connections != nullptr) {
                                    memset(runtime->connections, 0, runtime->connection_count * sizeof(AIPATHCNX_s *));
                                }
                                i32 next = 0;
                                for (i32 slot = 0; slot < 8; ++slot) {
                                    EDAIPATHCNX_s *editor_connection = &node->connections[slot];
                                    EDAIPATHNODE_s *other = editor_connection->node;
                                    if (other == nullptr) {
                                        continue;
                                    }
                                    if (other->index < node->index) {
                                        AIPATHNODE_s *other_runtime = &path->nodes[other->index];
                                        for (i32 reverse = 0; reverse < other_runtime->connection_count; ++reverse) {
                                            AIPATHCNX_s *connection = other_runtime->connections[reverse];
                                            if (connection->node_indices[1] == node->index) {
                                                runtime->connections[next] = connection;
                                                runtime->connections[next]->traversal_flags[1] =
                                                    editor_connection->flags;
                                                runtime->connections[next]->original_traversal_flags[1] =
                                                    editor_connection->flags;
                                                break;
                                            }
                                        }
                                    } else {
                                        runtime->connections[next] = &path->connections[connection_index++];
                                        runtime->connections[next]->node_indices[0] = node->index;
                                        runtime->connections[next]->node_indices[1] = editor_connection->node->index;
                                        AIPATHCNX_s *connection = runtime->connections[next];
                                        connection->traversal_flags[0] = editor_connection->flags;
                                        connection->original_traversal_flags[0] = editor_connection->flags;
                                        NUVEC difference;
                                        connection->distance =
                                            NuVecDist(&editor_connection->node->position, &node->position, &difference);
                                        runtime->connections[next]->horizontal_distance = NuVecXZDist(
                                            &editor_connection->node->position, &node->position, &difference);
                                        connection = runtime->connections[next];
                                        if (connection->horizontal_distance == 0.0f) {
                                            connection->horizontal_distance = 0.0001f;
                                        }
                                        connection->rotation = (i16)(NuAtan2(difference.x, difference.z) * 10430.378f);
                                    }
                                    ++next;
                                }
                            }
                        }
                        runtime->distance_cache_nodes[0] = 0xff;
                        runtime->distance_cache_nodes[1] = 0xff;
                        if (node->shared_node != nullptr) {
                            AIPATHSPECIALROUTE_s *shared_route =
                                &system->special_routes[node->shared_node->runtime_index];
                            AIPATHNODELINK_s *link =
                                &path->special_routes[runtime->special_route_index = path->special_route_count++];
                            shared_route->paths[shared_route->path_count++] = path;
                            link->node_index = node->index;
                            link->special_route_index = node->shared_node->runtime_index;
                        } else {
                            runtime->special_route_index = 0xff;
                        }
                    }
                }
            }
            distance_tables[path_index] = pathEditorCalculateDistanceTable(path, 0, &scratch, scratch_limit);

            if (path->node_count != 0) {
                path->route_matrix = (u8 **)AISysBufferAlloc(cursor, end, path->node_count * sizeof(u8 *));
                memset(path->route_matrix, 0, path->node_count * sizeof(u8 *));
                {
                    for (i32 source = 0; source < path->node_count; ++source) {
                        path->route_matrix[source] = (u8 *)AISysBufferAlloc(cursor, end, path->node_count);
                        memset(path->route_matrix[source], 0, path->node_count);
                    }
                }
                u8 **route_matrix = path->route_matrix;
                f32 **distances = distance_tables[path_index];
                if (distance_tables != nullptr && path->nodes != nullptr && distances != nullptr) {
                    for (i32 source = 0; source < path->node_count; ++source) {
                        AIPATHNODE_s *node = &path->nodes[source];
                        for (i32 destination = 0; destination < path->node_count; ++destination) {
                            route_matrix[source][destination] = 0xff;
                            if (destination == source) {
                                continue;
                            }
                            f32 best = FLT_MAX;
                            for (i32 edge = 0; edge < node->connection_count; ++edge) {
                                AIPATHCNX_s *connection = node->connections[edge];
                                bool direction = connection->node_indices[0] != source;
                                if (connection->traversal_flags[direction] & 0x40000000) {
                                    continue;
                                }
                                i32 neighbor = connection->node_indices[!direction];
                                f32 distance = distances[source][neighbor] + distances[neighbor][destination];
                                if (distance < best) {
                                    best = distance;
                                    route_matrix[source][destination] = edge;
                                }
                            }
                        }
                    }
                }
                pathEditorCreateSpecialRouteData(path, path_index, editor_path, &scratch, scratch_limit);
            }
        }
        if (system->path_count > 1) {
            AIPATHINFO info;
            AIPATH_s *first_path = system->paths[0];
            memset(&info, 0, sizeof(info));
            for (i32 path_index = 1; path_index < system->path_count; ++path_index) {
                AIPATH_s *path = system->paths[path_index];
                for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
                    AIPATHNODE_s *node = &path->nodes[node_index];
                    AISysGetPathPos(aieditor->ai_system, &node->position, &info, first_path, 0xff);
                    if (info.on_path) {
                        node->path_flags = info.connection - path->connections;
                        if (info.dist > 0.5f || (info.direction != 0 && info.dist < 0.5f)) {
                            node->runtime_flags |= 4;
                        }
                        node->runtime_flags |= 1;
                        node->path_flags = info.connection - info.path->connections;
                        path->flags |= 2;
                    } else {
                        node->path_flags = -1;
                        node->runtime_flags &= ~u8(1);
                    }
                }
                if (!(path->flags & 2) || distance_tables == nullptr || distance_tables[path_index] == nullptr) {
                    continue;
                }
                for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
                    AIPATHNODE_s *node = &path->nodes[node_index];
                    if (node->runtime_flags & 1) {
                        continue;
                    }
                    f32 nearest = FLT_MAX;
                    for (i32 candidate = 0; candidate < path->node_count; ++candidate) {
                        if (&path->nodes[candidate] != node && (path->nodes[candidate].runtime_flags & 1) &&
                            distance_tables[path_index][node_index][candidate] < nearest) {
                            nearest = distance_tables[path_index][node_index][candidate];
                            node->path_flags = candidate;
                        }
                    }
                }
            }
        }
        distance_tables = nullptr;
        return system;
    }
}
