#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/legoapi_types.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/antinode_editor_private.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include <string.h>

extern "C" {
    u64 GameAntinode_grid[64];
    extern f32 default_path_heighttol;
    extern i32 AIEDITOR_ANTINODES;
    extern i32 aidata_version;
    AIANTINODE_s *AIAntinodeCreateSingleFrame(NUVEC *, f32);
    void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);
}
i32 ViewCamGetMode();
NUVEC *ViewCamGetTgt();
void GameAntinode_FindGridPosition(WORLDINFO_s *, NUVEC *, f32, f32, u8 *, u8 *, u8 *, u8 *);
void GameAntinode_UnregisterAntiNode(GAMEANTINODESYS_s *, GAMEANTINODE_s *);

void *GameAntnode_CreateSys(WORLDINFO_s *world, variptr_u *buf, variptr_u *buf_end, i32 count) {
    usize node_bytes = count * sizeof(GAMEANTINODE_s);
    usize bytes = node_bytes + sizeof(GAMEANTINODESYS_s);
    buf->addr = (buf->addr + 15) & ~static_cast<usize>(15);
    if (count == 0 || buf->addr + bytes > buf_end->addr)
        return NULL;
    memset(buf->void_ptr, 0, bytes);
    GAMEANTINODESYS_s *system = static_cast<GAMEANTINODESYS_s *>(buf->void_ptr);
    buf->u8_ptr += sizeof(GAMEANTINODESYS_s);
    system->capacity = count;
    system->nodes = static_cast<GAMEANTINODE_s *>(buf->void_ptr);
    system->world = world;
    system->free = system->nodes;
    // The original links count - 1 pool entries, leaving the final entry unused.
    for (i32 i = 0; i < count - 2; ++i)
        system->nodes[i].next = &system->nodes[i + 1];
    buf->u8_ptr += node_bytes;
    return system;
}

void GameAntinode_Update(GAMEANTINODESYS_s *system) {
    if (system == NULL)
        return;
    memset(GameAntinode_grid, 0, sizeof(GameAntinode_grid));
    u8 min_x, min_z, max_x, max_z;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *object = &Obj[i];
        if ((object->apiobj.flags_high & 0x10) == 0 || object->apiobj.field_0x287 != 0)
            continue;
        GameAntinode_FindGridPosition(system->world, &object->apiobj.collision_position, object->field_0x1008,
                                      object->field_0x1008, &min_x, &min_z, &max_x, &max_z);
        for (i32 x = 0; x <= max_x - min_x; ++x)
            for (i32 z = 0; z <= max_z - min_z; ++z)
                GameAntinode_grid[min_x + x] |= 1ull << (min_z + z);
    }
    if (ViewCamGetMode() != 0) {
        GameAntinode_FindGridPosition(system->world, ViewCamGetTgt(), 0.1f, 0.1f, &min_x, &min_z, &max_x, &max_z);
        for (i32 x = 0; x <= max_x - min_x; ++x)
            for (i32 z = 0; z <= max_z - min_z; ++z)
                GameAntinode_grid[min_x + x] |= 1ull << (min_z + z);
    }
    for (GAMEANTINODE_s *node = system->active, *next; node != NULL; node = next) {
        next = node->next;
        if (node->remaining_time > 0.0f) {
            node->remaining_time -= FRAMETIME;
            if (node->remaining_time <= 0.0f) {
                GameAntinode_UnregisterAntiNode(system, node);
                continue;
            }
        }
        for (i32 x = 0; x <= node->grid_max_x - node->grid_min_x; ++x) {
            for (i32 z = 0; z <= node->grid_max_z - node->grid_min_z; ++z) {
                if (((GameAntinode_grid[node->grid_min_x + x] >> (node->grid_min_z + z)) & 1) == 0)
                    continue;
                AIANTINODE_s *active = AIAntinodeCreateSingleFrame(&node->position, node->radius);
                if (active != NULL) {
                    active->base_radius = node->extent_x;
                    active->base_height = node->extent_z;
                    active->type = node->shape;
                    active->height = node->min_y;
                    active->max_height = node->max_y;
                    active->flags = node->angle;
                    active->user_data[0] = node->user_data[0];
                    active->user_data[1] = node->user_data[1];
                }
                goto next_node;
            }
        }
    next_node:;
    }
}

void GameAntinode_Debug_DrawGrid(WORLDINFO_s *world) {
    f32 step_x = (world->level_max[0] - world->level_min[0]) * (1.0f / 64.0f);
    f32 step_z = (world->level_max[2] - world->level_min[2]) * (1.0f / 64.0f);
    f32 index = 0.0f;
    for (i32 i = 0; i < 64; ++i, index += 1.0f) {
        f32 x = world->level_min[0] + step_x * index;
        f32 y = Player[0]->apiobj.collision_position.y;
        NuRndrLine3dDbg(x, y, world->level_min[2], x, y, world->level_max[2], 0xff0000ff);
    }
    index = 0.0f;
    for (i32 i = 0; i < 64; ++i, index += 1.0f) {
        f32 z = world->level_min[2] + step_z * index;
        f32 y = Player[0]->apiobj.collision_position.y;
        NuRndrLine3dDbg(world->level_min[0], y, z, world->level_max[0], y, z, 0xff00ff00);
    }
}

void GameAntinode_FindGridPosition(WORLDINFO_s *world, NUVEC *position, f32 radius_x, f32 radius_z, u8 *min_x,
                                   u8 *min_z, u8 *max_x, u8 *max_z) {
    f32 cell_x = (world->level_max[0] - world->level_min[0]) * (1.0f / 64.0f);
    f32 cell_z = (world->level_max[2] - world->level_min[2]) * (1.0f / 64.0f);
    if (min_x != NULL) {
        f32 offset = position->x - radius_x - world->level_min[0];
        *min_x = offset == 0.0f || cell_x == 0.0f ? 0 : static_cast<u8>(static_cast<i32>(offset / cell_x));
        if (*min_x > 63)
            *min_x = 63;
    }
    if (max_x != NULL) {
        f32 offset = radius_x + position->x - world->level_min[0];
        *max_x = offset == 0.0f || cell_x == 0.0f ? 0 : static_cast<u8>(static_cast<i32>(offset / cell_x));
        if (*max_x > 63)
            *max_x = 63;
    }
    if (min_z != NULL) {
        f32 offset = position->z - radius_z - world->level_min[2];
        *min_z = offset == 0.0f || cell_z == 0.0f ? 0 : static_cast<u8>(static_cast<i32>(offset / cell_z));
        if (*min_z > 63)
            *min_z = 63;
    }
    if (max_z != NULL) {
        f32 offset = radius_z + position->z - world->level_min[2];
        *max_z = offset == 0.0f || cell_z == 0.0f ? 0 : static_cast<u8>(static_cast<i32>(offset / cell_z));
        if (*max_z > 63)
            *max_z = 63;
    }
}

GAMEANTINODE_s *GameAntinode_RegisterAntiNode(GAMEANTINODESYS_s *system, NUVEC *position, f32 radius, f32 extent_x,
                                              f32 extent_z, u16 angle, i32 shape, f32 duration) {
    if (system == NULL)
        return NULL;
    GAMEANTINODE_s *node = system->free;
    if (node == NULL)
        return NULL;
    if (position == NULL)
        position = &v000;
    node->position = *position;
    node->radius = radius;
    node->extent_x = extent_x;
    node->extent_z = extent_z;
    node->angle = angle;
    node->shape = shape;
    node->min_y = node->position.y - default_path_heighttol;
    node->max_y = default_path_heighttol + node->position.y;
    node->remaining_time = duration;
    if (node->shape == 1)
        radius = extent_x > extent_z ? extent_x : extent_z;
    else if (node->shape == 2)
        radius = NuFsqrt(extent_x * extent_x + extent_z * extent_z);
    node->radius = radius;
    GameAntinode_FindGridPosition(system->world, &node->position, radius, radius, &node->grid_min_x, &node->grid_min_z,
                                  &node->grid_max_x, &node->grid_max_z);
    system->free = node->next;
    node->next = system->active;
    system->active = node;
    ++system->count;
    return node;
}

void GameAntinode_UnregisterAntiNode(GAMEANTINODESYS_s *system, GAMEANTINODE_s *node) {
    if (system == NULL || system->active == NULL || node == NULL)
        return;
    GAMEANTINODE_s *previous = system->active;
    if (previous == node) {
        system->active = previous->next;
        previous->next = system->free;
        system->free = previous;
    } else {
        while (previous->next != NULL) {
            if (previous->next == node) {
                previous->next = node->next;
                node->next = system->free;
                system->free = node;
                break;
            }
            previous = previous->next;
        }
    }
    --system->count;
}

GAMEANTINODE_s *GameAntinode_RegisterAntiNodeUsingData(GAMEANTINODESYS_s *system, NUVEC *position, u16 angle,
                                                       GAMEANTINODEDATA_s *data, f32 duration, i32 disabled) {
    if (disabled != 0 || (data->mode & 1) != 0 || position == NULL || system == NULL)
        return NULL;
    NUVEC center = data->position;
    NuVecRotateY(&center, &center, angle);
    NuVecAdd(&center, &center, position);
    GAMEANTINODE_s *node = GameAntinode_RegisterAntiNode(system, &center, data->radius, data->extent_x, data->extent_z,
                                                         angle + data->flags, data->use_largest_extent, duration);
    if (node == NULL)
        return NULL;
    node->min_y = data->min_y + position->y;
    node->max_y = position->y + data->max_y;
    return node;
}

GAMEANTINODE_s *GameAntinode_UpdateAntiNodeUsingData(GAMEANTINODESYS_s *system, GAMEANTINODE_s *node, NUVEC *position,
                                                     u16 angle, GAMEANTINODEDATA_s *data, f32 duration, i32 disabled) {
    if (data == NULL || position == NULL)
        return node;

    if ((data->mode & 1) != 0) {
        if (node != NULL) {
            GameAntinode_UnregisterAntiNode(system, node);
            node = NULL;
        }
    } else if (node != NULL) {
        if (disabled != 0) {
            GameAntinode_UnregisterAntiNode(system, node);
            node = NULL;
        } else {
            node->position = data->position;
            NuVecRotateY(&node->position, &node->position, angle);
            NuVecAdd(&node->position, &node->position, position);
            node->min_y = data->min_y + position->y;
            node->max_y = position->y + data->max_y;
            node->radius = data->radius;
            node->extent_x = data->extent_x;
            node->extent_z = data->extent_z;
            node->angle = data->flags + angle;
            node->shape = data->use_largest_extent;
            node->remaining_time = duration;

            f32 radius = node->radius;
            if (node->shape == 1)
                radius = node->extent_x > node->extent_z ? node->extent_x : node->extent_z;
            else if (node->shape == 2)
                radius = NuFsqrt(node->extent_x * node->extent_x + node->extent_z * node->extent_z);

            // Unlike registration, updates use the enclosing radius only for grid coverage.
            GameAntinode_FindGridPosition(system->world, &node->position, radius, radius, &node->grid_min_x,
                                          &node->grid_min_z, &node->grid_max_x, &node->grid_max_z);
        }
    } else if (disabled == 0) {
        node = GameAntinode_RegisterAntiNodeUsingData(system, position, angle, data, duration, 0);
    }

    return node;
}

extern "C" {

    void antinodeEditorDrawAntinodes(void) {
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        EDANTINODE_s *selected = static_cast<EDANTINODE_s *>(aieditor->mode_selection_42e9c);
        EDANTINODE_s *nearest = *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0);
        const bool active_mode = aieditorsettings.current_mode == AIEDITOR_ANTINODES;
        const bool solid = aieditorsettings.solid_antinode_display;
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(list, &node->link))) {
            u32 colour = 0x50505050;
            i32 segments = 8;
            if (active_mode) {
                if (node == selected) {
                    segments = 32;
                    if (node == nearest)
                        colour = NuSpecialExistsFn(&node->special) ? 0xffff0064 : 0xffff0000;
                    else
                        colour = NuSpecialExistsFn(&node->special) ? 0x80800064 : 0x80800000;
                } else if (node == nearest) {
                    colour = 0xffffffff;
                    segments = 32;
                } else {
                    colour = 0x80640000;
                }
            }
            if (node->type == 0) {
                if (solid)
                    LocaledbitsDrawSolidCircleXY(&node->position, node->radius, node->position.y + node->lower_height,
                                                 node->position.y + node->upper_height, colour, 0, segments);
                else
                    LocaledbitsDrawCircleXY(&node->position, node->radius, colour, 0, segments);
            } else if (node->type == 1) {
                f32 lower = solid ? node->position.y + node->lower_height : node->position.y;
                f32 upper = solid ? node->position.y + node->upper_height : node->position.y;
                LocaledbitsDrawSolidEllipseXY(&node->position, node->base_radius, node->base_height, node->flags, lower,
                                              upper, colour, 0, segments);
            } else if (node->type == 2) {
                NUMTX matrix;
                NuMtxSetTranslation(&matrix, &node->position);
                NuMtxPreRotateY(&matrix, node->flags);
                NUVEC minimum = {-node->base_radius, solid ? node->lower_height : 0.0f, -node->base_height};
                NUVEC maximum = {node->base_radius, solid ? node->upper_height : 0.0f, node->base_height};
                NuRndrBoundingBox(&minimum, &maximum, &matrix, colour);
            }
        }
    }

    void antinodeEditorSaveData(void) {
        if (aidata_version <= 12)
            return;
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        i32 count = 0;
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(list, &node->link)))
            ++count;
        EdFileWriteInt(count);
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(list, &node->link))) {
            if (node->type == 2)
                node->radius = NuFsqrt(node->base_radius * node->base_radius + node->base_height * node->base_height);
            else if (node->type == 1)
                node->radius = node->base_radius > node->base_height ? node->base_radius : node->base_height;
            EdFileWriteFloat(node->position.x);
            EdFileWriteFloat(node->position.y);
            EdFileWriteFloat(node->position.z);
            EdFileWriteFloat(node->radius);
            EdFileWriteFloat(node->position.y + node->lower_height);
            EdFileWriteFloat(node->position.y + node->upper_height);
            if (aidata_version > 14) {
                EdFileWriteInt(node->flags);
                EdFileWriteFloat(node->base_radius);
                EdFileWriteFloat(node->base_height);
            }
            EdFileWriteChar(aidata_version > 14 ? node->type : 0);
            EdFileWriteChar(0);
            EdFileWriteChar(0);
            EdFileWriteChar(node->game_flags);
            char *name = NuSpecialExistsFn(&node->special) ? NuSpecialGetName(&node->special) : nullptr;
            if (name == nullptr) {
                EdFileWriteChar(0);
            } else {
                i32 length = strlen(name) + 1;
                EdFileWriteChar(length);
                EdFileWrite(name, length);
                EdFileWriteFloat(node->special_position.x);
                EdFileWriteFloat(node->special_position.y);
                EdFileWriteFloat(node->special_position.z);
                if (aidata_version > 14)
                    EdFileWriteInt(node->rotation_offset);
            }
        }
    }

    void antinodeEditor_UpdateAntiNodesOnPlatforms(void) {
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        NUVEC forward = {0.0f, 0.0f, 1.0f};
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(list, &node->link))) {
            if (!NuSpecialExistsFn(&node->special))
                continue;
            NUMTX *matrix = NuSpecialGetDrawMtx(&node->special);
            NuVecMtxTransform(&node->position, &node->special_position, matrix);
            NUVEC rotated;
            NuVecMtxRotate(&rotated, &forward, matrix);
            node->flags = NuAngAdd(NuAtan2D(rotated.x, rotated.z), node->rotation_offset);
        }
    }

} // extern "C"

void AISysDrawAntinode_Circle(AIANTINODE_s *node, u32 colour) {
    LocaledbitsDrawSolidCircleXY(&node->position, node->radius, node->min_y, node->max_y, colour, 0, 16);
}

void AISysDrawAntinode_Ellipse(AIANTINODE_s *node, u32 colour) {
    LocaledbitsDrawSolidEllipseXY(&node->position, node->base_radius, node->base_height, node->flags, node->min_y,
                                  node->max_y, colour, 0, 16);
}

void AISysDrawAntinode_Rectangle(AIANTINODE_s *node, u32 colour) {
    NUMTX matrix;
    NuMtxSetTranslation(&matrix, &node->position);
    NuMtxPreRotateY(&matrix, node->flags);

    NUVEC corners[8] = {
        {-node->base_radius, node->min_y, -node->base_height}, {node->base_radius, node->min_y, -node->base_height},
        {node->base_radius, node->min_y, node->base_height},   {-node->base_radius, node->min_y, node->base_height},
        {-node->base_radius, node->max_y, -node->base_height}, {node->base_radius, node->max_y, -node->base_height},
        {node->base_radius, node->max_y, node->base_height},   {-node->base_radius, node->max_y, node->base_height},
    };
    NuVecMtxTransform(&corners[0], &corners[0], &matrix);
    NuVecMtxTransform(&corners[1], &corners[1], &matrix);
    NuVecMtxTransform(&corners[2], &corners[2], &matrix);
    NuVecMtxTransform(&corners[3], &corners[3], &matrix);
    NuVecMtxTransform(&corners[4], &corners[4], &matrix);
    NuVecMtxTransform(&corners[5], &corners[5], &matrix);
    NuVecMtxTransform(&corners[6], &corners[6], &matrix);
    NuVecMtxTransform(&corners[7], &corners[7], &matrix);

#define ANTINODE_DEBUG_EDGE(a, b)                                                                                      \
    AiRndrLine3dDbg(corners[a].x, corners[a].y, corners[a].z, corners[b].x, corners[b].y, corners[b].z, colour)
    ANTINODE_DEBUG_EDGE(0, 1);
    ANTINODE_DEBUG_EDGE(1, 2);
    ANTINODE_DEBUG_EDGE(2, 3);
    ANTINODE_DEBUG_EDGE(3, 0);
    ANTINODE_DEBUG_EDGE(4, 5);
    ANTINODE_DEBUG_EDGE(5, 6);
    ANTINODE_DEBUG_EDGE(6, 7);
    ANTINODE_DEBUG_EDGE(7, 4);
    ANTINODE_DEBUG_EDGE(0, 4);
    ANTINODE_DEBUG_EDGE(1, 5);
    ANTINODE_DEBUG_EDGE(2, 6);
    ANTINODE_DEBUG_EDGE(3, 7);
#undef ANTINODE_DEBUG_EDGE
}
