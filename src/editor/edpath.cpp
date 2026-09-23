#include "decomp.h"
#include "editor/edpath.h"
#include "editor/path_connections.h"
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <stdint.h>

#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "globals.h"
extern "C" {
    extern void *ed_fnt;
    extern i32 AIEDITOR_PATHS;
    extern i32 AIEDITOR_ROUTES;
    extern i32 AIEDITOR_LOCATORS;
    extern i32 AIEDITOR_CREATURES;
    extern i32 aidata_version;
    extern i32 near_clip_at_cursor;
    extern f32 default_path_heighttol;
    extern f32 aiEditor_DrawYOffset;
    void AiRndrLine3d(NURND_VERTEX3D *, numtl_s *, NUMTX *);
    void creatureEditor_PathNodeMoved(EDAIPATHNODE_s *);
    void locatorEditor_PathNodeMoved(EDAIPATHNODE_s *);
    void creatureEditor_PathDeleted(EDAIPATH_s *);
    void locatorEditor_PathDeleted(EDAIPATH_s *);
    void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *);
    void locatorEditor_PathNodeDeleted(EDAIPATHNODE_s *);
    void creatureEditor_RenderAllCreatures(void);
    void areaEditorDrawAreas(void);
    void locatorEditorDrawLocators(void);
    void antinodeEditorDrawAntinodes(void);
    void pathEditorDrawPaths(void);
    void LocaledbitsDrawCircleXY(NUVEC *, f32, u32, i32, i32);
    void LocaledbitsDrawSolidCircleXY(NUVEC *, f32, f32, f32, u32, i32, i32);
    extern void (*AIPathDeletedFn)(EDAIPATH_s *);
    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
    void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSolidPathDisplayToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbDrawAllToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbShowCreaturesToggle(eduimenu_s *, eduiitem_s *, u32);
    void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *, u32);
    NUVEC edpath_addoffset;
    f32 default_path_node_radius = .25f;
    extern char *(*SpecialRouteCharacterNameFn)(u8);
}
struct AIPATHCNXTYPE_s {
    u32 connection_flag;
    void *context;
    char name[0x40];
    u32 flags;
};
DECOMP_ASSERT(sizeof(AIPATHCNXTYPE_s) == 0x4c, "editor path connection type size");
DECOMP_ASSERT(offsetof(AIPATHCNXTYPE_s, flags) == 0x48, "editor path connection options offset");
static AIPATHCNXTYPE_s aipathcnxtypes[32];
static i32 naipathcnxtypes;
static i32 iterator_count;
static f32 ***distance_tables;

extern "C" void aieditor_ClearAllPathCnxTypes(void) {
    naipathcnxtypes = 0;
    memset(aipathcnxtypes, 0, sizeof(aipathcnxtypes));
}

extern "C" void aieditor_RegisterDefaultPathCnxTypes(void) {
    aieditor_RegisterPathCnxType("Permanent Block", 0x40000000, nullptr, 0);
    aieditor_RegisterPathCnxType("Temporary Block", 0x80000000, nullptr, 0);
    aieditor_RegisterPathCnxType("Link Obstacle", 0x20000000, nullptr, 1);
}

extern "C" void aieditor_RegisterPathCnxType(const char *name, u32 connection_flag, void *context, u32 flags) {
    if (name == nullptr) {
        return;
    }
    usize length = strlen(name);
    if (length > 63 || connection_flag == 0 || naipathcnxtypes >= 32) {
        return;
    }
    AIPATHCNXTYPE_s &type = aipathcnxtypes[naipathcnxtypes++];
    memcpy(type.name, name, length + 1);
    type.connection_flag = connection_flag;
    type.context = context;
    type.flags = flags;
}

void pathEditorDrawConnectionInfo(nuvec_s *position, float radius, nuvec_s *other, u32 flags, i32 colour) {
    if (flags == 0) {
        return;
    }
    NUVEC start = {position->x, position->y + aiEditor_DrawYOffset, position->z};
    NUVEC end = {other->x, other->y + aiEditor_DrawYOffset, other->z};
    for (i32 i = 0; i < naipathcnxtypes; ++i) {
        AIPATHCNXTYPE_s *type = &aipathcnxtypes[i];
        if (type->context != nullptr && (type->connection_flag & flags) != 0) {
            typedef void DrawConnection(NUVEC *, f32, NUVEC *, u32, i32);
            reinterpret_cast<DrawConnection *>(type->context)(&start, radius, &end, flags, colour);
            return;
        }
    }
    NUVEC direction;
    f32 length = NuVecXZDist(&end, &start, &direction);
    if (length > 0.0f) {
        direction.x /= length;
        direction.z /= length;
    }
    NURND_VERTEX3D vertices[2];
    vertices[0].position = start;
    vertices[1].position = end;
    vertices[0].colour = colour;
    vertices[1].colour = colour;
    AiRndrLine3d(vertices, nullptr, nullptr);
    NuVecScale(&vertices[0].position, &direction, radius * 0.5f);
    vertices[0].position.y = 0.0f;
    NuVecAdd(&vertices[0].position, &vertices[0].position, &start);
    vertices[1].position = start;
    vertices[1].position.x += radius * direction.z * 0.5f;
    vertices[1].position.z -= radius * direction.x * 0.5f;
    AiRndrLine3d(vertices, nullptr, nullptr);
    vertices[1].position = start;
    vertices[1].position.x -= radius * direction.z * 0.5f;
    vertices[1].position.z += radius * direction.x * 0.5f;
    AiRndrLine3d(vertices, nullptr, nullptr);
    if (flags & 0x40000000) {
        NuVecScale(&vertices[0].position, &direction, radius * 0.5f);
        vertices[0].position.y = 0.0f;
        NuVecAdd(&vertices[0].position, &vertices[0].position, &start);
        vertices[1].position.y = vertices[0].position.y;
        vertices[1].position.x = vertices[0].position.x - radius * direction.z * 0.5f;
        vertices[1].position.z = vertices[0].position.z + radius * direction.x * 0.5f;
        vertices[0].position.x += radius * direction.z * 0.5f;
        vertices[0].position.z -= radius * direction.x * 0.5f;
        AiRndrLine3d(vertices, nullptr, nullptr);
    } else if (flags & 0x80000000) {
        NUVEC centre = vertices[0].position;
        vertices[0].colour = colour / 2;
        vertices[1].colour = colour / 2;
        vertices[1].position.x = centre.x + radius * direction.z * 0.25f;
        vertices[1].position.y = centre.y;
        vertices[1].position.z = centre.z - radius * direction.x * 0.25f;
        AiRndrLine3d(vertices, nullptr, nullptr);
        vertices[1].position.x = centre.x - radius * direction.z * 0.25f;
        vertices[1].position.y = centre.y;
        vertices[1].position.z = centre.z + radius * direction.x * 0.25f;
        AiRndrLine3d(vertices, nullptr, nullptr);
    } else if (flags & 0x20000000) {
        NUVEC centre = vertices[0].position;
        NuVecScale(&vertices[0].position, &direction, radius);
        vertices[0].position.y = 0.0f;
        NuVecAdd(&vertices[0].position, &vertices[0].position, position);
        vertices[1].position.x = centre.x + radius * direction.z * 0.15f;
        vertices[1].position.y = centre.y;
        vertices[1].position.z = centre.z - radius * direction.x * 0.15f;
        AiRndrLine3d(vertices, nullptr, nullptr);
        vertices[1].position.x = centre.x - radius * direction.z * 0.25f;
        vertices[1].position.y = centre.y;
        vertices[1].position.z = centre.z + radius * direction.x * 0.25f;
        AiRndrLine3d(vertices, nullptr, nullptr);
        vertices[1].position.x = centre.x - radius * direction.z * 0.35f;
        vertices[1].position.y = centre.y;
        vertices[1].position.z = centre.z + radius * direction.x * 0.35f;
        AiRndrLine3d(vertices, nullptr, nullptr);
    }
}

static u32 attr[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
static void DestroyAIPathNode(EDAIPATHNODE_s *, EDAIPATH_s *);
static void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *);
extern "C" void aieditor_ClearMainMenu(void);
extern "C" AIEDITORPATHNODECALLBACK *AIPathNodeMovedFn;

static inline void pathEditor_DisconnectNodes(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) {
    for (i32 i = 0; i < 8; ++i) {
        if (node->connections[i].node == other) {
            for (i32 j = 0; j < 8; ++j) {
                if (other->connections[j].node == node) {
                    memset(&node->connections[i], 0, sizeof(EDAIPATHCNX_s));
                    memset(&other->connections[j], 0, sizeof(EDAIPATHCNX_s));
                    return;
                }
            }
        }
    }
}

struct EDAISHAREDPATHNODE_s;
struct AIPATH_s;
struct eduimenu_s;
struct eduiitem_s;
struct nuvec_s;
struct nupad_s;

struct EdUiNameInputItem : eduiitem_s {
    u32 unknown_48;
    char name[0x40];
    u8 unknown_08c[0x15a - 0x8c];
    i16 max_name_length;
};
DECOMP_ASSERT(offsetof(EdUiNameInputItem, name) == 0x4c, "editor name input offset");
DECOMP_ASSERT(offsetof(EdUiNameInputItem, max_name_length) == 0x15a, "editor name input limit offset");

static __used__ void ParseAIPathCnxFlag(char *) {
}

void pathEditorDrawNode(NUVEC *position, f32 radius, f32 lower_height, f32 upper_height, u32 colour, numtl_s *material,
                        i32 segments, i32 solid);

static __used__ void pathEditorDrawPath(EDAIPATH_s *path, i32 path_index) {
    if (path == nullptr) {
        return;
    }
    u8 drawn_connections[0x1fe0] = {};
    u32 colour =
        path == aieditor->current_path ? 0xffffffff : AISysGetPathColour(path_index % AISysGetPathColourCount());
    i32 solid =
        aieditorsettings.solid_path_display && (static_cast<i16>(aieditorsettings.current_mode) == AIEDITOR_PATHS ||
                                                static_cast<i16>(aieditorsettings.current_mode) == AIEDITOR_CREATURES ||
                                                static_cast<i16>(aieditorsettings.current_mode) == AIEDITOR_LOCATORS);
    u32 active_route = 0;
    if (aieditorsettings.current_mode == AIEDITOR_ROUTES && aieditor->current_path != nullptr) {
        EDAIPATH_s *selected = aieditor->current_path;
        active_route = selected->current_route != nullptr ? 1u << (selected->current_route - selected->routes) : 1u;
    }
    for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes); node != nullptr;
         node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link)) {
        u32 node_colour = colour;
        i32 segments = 8;
        if (path == aieditor->current_path) {
            if (node == path->current_node) {
                node_colour = 0xff0000ff;
                segments = 32;
            } else if (node == path->other_node) {
                node_colour = 0xff00ffff;
                segments = 32;
            }
        }
        pathEditorDrawNode(&node->position, node->radius, node->position.y + node->lower_height,
                           node->position.y + node->upper_height, node_colour, nullptr, segments, solid);
        if (node->shared_node != nullptr && !(node->shared_node->draw_flags & 1)) {
            NURND_VERTEX3D vertices[2];
            vertices[0].position = node->position;
            vertices[1].position = node->position;
            vertices[0].colour = node_colour;
            vertices[1].colour = node_colour;
            vertices[0].position.x += node->radius;
            vertices[1].position.x -= node->radius;
            AiRndrLine3d(vertices, nullptr, nullptr);
            vertices[0].position = node->position;
            vertices[1].position = node->position;
            vertices[0].position.z += node->radius;
            vertices[1].position.z -= node->radius;
            AiRndrLine3d(vertices, nullptr, nullptr);
            node->shared_node->draw_flags |= 1;
        }
        if (active_route != 0 && (node->route_mask & active_route)) {
            pathEditorDrawNode(&node->position, node->radius * 0.9f, node->position.y + node->lower_height,
                               node->position.y + node->upper_height, node_colour, nullptr, segments, solid);
        }
        for (i32 slot = 0; slot < 8; ++slot) {
            EDAIPATHCNX_s *connection = &node->connections[slot];
            EDAIPATHNODE_s *other = connection->node;
            if (other == nullptr) {
                continue;
            }
            u32 connection_colour = colour;
            if (path == aieditor->current_path && node == path->current_node && other == path->other_node) {
                connection_colour = 0xff00ffff;
            }
            pathEditorDrawConnectionInfo(&node->position, node->radius, &other->position, connection->flags,
                                         connection_colour);

            i32 first_index = node->index;
            i32 second_index = other->index;
            if (first_index >= 0 && first_index < 255 && second_index >= 0 && second_index < 255) {
                u8 &seen = drawn_connections[first_index * 32 + second_index / 8];
                u8 bit = static_cast<u8>(1u << (second_index & 7));
                if (seen & bit) {
                    continue;
                }
                seen |= bit;
                drawn_connections[second_index * 32 + first_index / 8] |= static_cast<u8>(1u << (first_index & 7));
            }

            bool current_edge =
                path == aieditor->current_path && (node == path->current_node || other == path->current_node);
            bool selected_pair =
                path == aieditor->current_path && ((node == path->current_node && other == path->other_node) ||
                                                   (node == path->other_node && other == path->current_node));
            u32 edge_colour = selected_pair ? 0xff00ffff : current_edge ? 0xff0000ff : colour;
            NUVEC direction;
            f32 distance = NuVecXZDist(&node->position, &other->position, &direction);
            if (distance > 0.0f) {
                direction.x /= distance;
                direction.z /= distance;
            }
            i32 angle = 0x4000;
            if (distance > 0.0f && node->radius != other->radius) {
                angle -= NuASin((other->radius - node->radius) / distance);
            }
            NURND_VERTEX3D vertices[2];
            vertices[0].colour = edge_colour;
            vertices[1].colour = edge_colour;
            for (i32 side_index = 0; side_index < 2; ++side_index) {
                NUVEC side;
                NuVecRotateY(&side, &direction, side_index == 0 ? angle : -angle);
                vertices[0].position = node->position;
                vertices[1].position = other->position;
                vertices[0].position.x += node->radius * side.x;
                vertices[0].position.z += node->radius * side.z;
                vertices[1].position.x += other->radius * side.x;
                vertices[1].position.z += other->radius * side.z;
                if (solid) {
                    vertices[0].position.y += node->lower_height;
                    vertices[1].position.y += other->lower_height;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                    vertices[0].position.y = node->position.y + node->upper_height;
                    vertices[1].position.y = other->position.y + other->upper_height;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                } else {
                    vertices[0].position.y += aiEditor_DrawYOffset;
                    vertices[1].position.y += aiEditor_DrawYOffset;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                }
            }

            if (active_route != 0 && (connection->route_mask & active_route)) {
                vertices[0].position = node->position;
                vertices[1].position = other->position;
                vertices[0].position.y += aiEditor_DrawYOffset + 0.02f;
                vertices[1].position.y += aiEditor_DrawYOffset + 0.02f;
                u32 route_colour = selected_pair ? 0xff00ffff : 0xffff0000;
                vertices[0].colour = route_colour;
                vertices[1].colour = route_colour;
                AiRndrLine3d(vertices, nullptr, nullptr);
            }
        }
    }
}

static __used__ i32 TestPointPathCheck(nuvec_s *point, EDAIPATHNODE_s *first, EDAIPATHNODE_s *second, f32 *fraction,
                                       f32 *width, i32 *angle, f32 tolerance) {
    AIPATH path;
    AIPATHCNX connection;
    AIPATHNODE nodes[2];
    AIPATHINFO info;
    memset(&path, 0, sizeof(path));
    memset(&connection, 0, sizeof(connection));
    memset(nodes, 0, sizeof(nodes));
    memset(&info, 0, sizeof(info));
    nodes[0].position = first->position;
    nodes[0].radius = first->radius + tolerance;
    nodes[0].min_height = first->position.y + first->lower_height - tolerance;
    nodes[0].max_height = first->position.y + first->upper_height + tolerance;
    nodes[1].position = second->position;
    nodes[1].radius = second->radius + tolerance;
    nodes[1].min_height = second->position.y + second->lower_height - tolerance;
    nodes[1].max_height = second->position.y + second->upper_height + tolerance;
    path.nodes = nodes;
    path.connections = &connection;
    path.node_count = 2;
    path.connection_count = 1;
    connection.node_indices[0] = 1;
    NUVEC horizontal;
    f32 distance = NuVecXZDist(&first->position, &second->position, &horizontal);
    connection.horizontal_distance = distance != 0.0f ? distance : 0.0001f;
    connection.rotation = (i16)(NuAtan2(horizontal.x, horizontal.z) * 10430.378f);
    if (!WithinConnection(nullptr, point, &path, &connection, 1, nullptr, 0xff, 0xff, &info, 0.0f, 0)) {
        return 0;
    }

    NUVEC direction;
    NuVecSub(&direction, &second->position, &first->position);
    distance = NuVecMag(&direction);
    NuVecScale(&direction, &direction, 1.0f / distance);
    NUVEC relative;
    NuVecSub(&relative, point, &first->position);
    f32 along = NuVecDot(&direction, &relative);
    *fraction = along / distance;
    NUVEC nearest;
    f32 radius;
    if (*fraction <= 0.0f) {
        nearest = first->position;
        radius = first->radius;
    } else if (*fraction >= 1.0f) {
        nearest = second->position;
        radius = second->radius;
    } else {
        NuVecScale(&nearest, &direction, along);
        NuVecAdd(&nearest, &nearest, &first->position);
        radius = second->radius * *fraction + first->radius * (1.0f - *fraction);
    }
    NUVEC radial;
    f32 distance_squared = NuVecXZDistSqr(&nearest, point, &radial);
    radius += tolerance;
    *angle = (i32)(NuAtan2(direction.x, direction.z) * 10430.378f);
    NuVecRotateY(&relative, &relative, -*angle);
    if (*fraction <= 0.0f) {
        f32 beyond = -*fraction * distance;
        *width = distance_squared - beyond * beyond;
        *width = NuFsqrt(*width) / radius;
    } else if (*fraction >= 1.0f) {
        f32 beyond = (*fraction - 1.0f) * distance;
        *width = distance_squared - beyond * beyond;
        *width = NuFsqrt(*width) / radius;
    } else {
        *width = NuFsqrt(distance_squared) / radius;
    }
    if (relative.x < 0.0f) {
        *width = -*width;
    }
    return 1;
}

static __used__ void pathEditor_cbCreatePath(eduimenu_s *, eduiitem_s *, u32) {
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->free_paths);
    if (path == nullptr) {
        return;
    }
    NuLinkedListRemove(&aieditor->free_paths, &path->link);
    NuLinkedListAppend(&aieditor->paths, &path->link);
    char name[16];
    i32 number = 0;
    EDAIPATH_s *existing;
    do {
        sprintf(name, "NewPath%d", ++number);
        existing = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (existing != nullptr && NuStrICmp(name, existing->name) != 0) {
            existing = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &existing->link);
        }
    } while (existing != nullptr);
    strcpy(path->name, name);
    path->flags &= ~1;
    aieditor->current_path = path;
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbDeletePath(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (item == nullptr) {
        return;
    }
    switch (item->data) {
        case 0: {
            eduimenu_s *menu = eduiMenuCreate(240, 90, 240, 250, ed_fnt, nullptr, "Delete current path??");
            if (menu != nullptr) {
                eduiMenuAddItem(menu, eduiItemSelCreate(2, attr, 0, 0, pathEditor_cbDeletePath, "No"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbDeletePath, "Yes"));
                eduiMenuAttach(parent, menu);
            }
            break;
        }
        case 1: {
            if (aieditor->current_path == nullptr || (aieditor->current_path->flags & 1)) {
                return;
            }
            creatureEditor_PathDeleted(aieditor->current_path);
            locatorEditor_PathDeleted(aieditor->current_path);
            if (AIPathDeletedFn != nullptr) {
                AIPathDeletedFn(aieditor->current_path);
            }
            EDAIPATH_s *path = aieditor->current_path;
            if (path != nullptr) {
                EDAIPATHNODE_s *node;
                while ((node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes)) != nullptr) {
                    DestroyAIPathNode(node, path);
                }
                NuLinkedListRemove(&aieditor->paths, &path->link);
                memset(path, 0, sizeof(*path));
                NuLinkedListAppend(&aieditor->free_paths, &path->link);
            }
            aieditor->current_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
            aieditor_ClearMainMenu();
            break;
        }
        case 2:
            aieditor_ClearMainMenu();
            break;
    }
}

static __used__ void pathEditor_cbRenameNode(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_node == nullptr) {
        return;
    }
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(item);
    if (input->name[0] == '\0') {
        memset(path->current_node->name, 0, sizeof(path->current_node->name));
        return;
    }
    EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
    while (node != nullptr) {
        if (NuStrICmp(node->name, input->name) == 0) {
            return;
        }
        node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&aieditor->current_path->nodes, &node->link);
    }
    strcpy(aieditor->current_path->current_node->name, input->name);
}

static __used__ void pathEditor_cbRenamePath(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *current = aieditor->current_path;
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(item);
    if (current == nullptr || input->name[0] == '\0') {
        return;
    }
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    while (path != nullptr) {
        if (NuStrICmp(path->name, input->name) == 0) {
            return;
        }
        path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
    }
    strcpy(aieditor->current_path->name, input->name);
}

static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *, u32);
static __used__ void pathEditor_cbSetShareNode(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *current_path = aieditor->current_path;
    if (current_path == nullptr || current_path->current_node == nullptr) {
        return;
    }
    EDAIPATHNODE_s *current = current_path->current_node;
    EDAISHAREDPATHNODE_s *shared = current->shared_node;
    EDAIPATHNODE_s *matches[16];
    i32 match_count = 0;
    i32 path_number = 1;
    for (EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); path != nullptr;
         path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link)) {
        if (path == current_path) {
            continue;
        }
        if (item->data != 0 && path_number != item->data) {
            ++path_number;
            continue;
        }
        EDAIPATHNODE_s *nearest = nullptr;
        f32 nearest_distance = FLT_MAX;
        for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes); node != nullptr;
             node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link)) {
            f32 dx = node->position.x - current->position.x;
            f32 dz = node->position.z - current->position.z;
            f32 distance = NuFsqrt(dx * dx + dz * dz);
            if (distance < node->radius + current->radius &&
                current->position.y + current->lower_height <= node->position.y + node->upper_height &&
                node->position.y + node->lower_height <= current->position.y + current->upper_height &&
                distance < nearest_distance) {
                nearest_distance = distance;
                nearest = node;
            }
        }
        if (nearest != nullptr && match_count < 16) {
            matches[match_count++] = nearest;
            if (shared == nullptr) {
                shared = nearest->shared_node;
            }
        }
        ++path_number;
    }
    if (match_count == 0) {
        if (item->data == 0) {
            aieditor_ClearMainMenu();
        }
        return;
    }
    if (item->data != 0 && !(item->flags & EDUI_ITEM_HIGHLIGHTED)) {
        if (current->shared_node == nullptr) {
            return;
        }
        for (i32 i = 0; i < match_count; ++i) {
            EDAIPATHNODE_s *node = matches[i];
            if (node->shared_node == current->shared_node) {
                EDAISHAREDPATHNODE_s *old_shared = node->shared_node;
                --old_shared->reference_count;
                if (old_shared->reference_count <= 1) {
                    pathEditor_DestroySharedNode(old_shared);
                }
                node->shared_node = nullptr;
            }
        }
        return;
    }
    if (shared == nullptr) {
        shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->free_shared_path_nodes);
        if (shared == nullptr) {
            if (item->data == 0) {
                aieditor_ClearMainMenu();
            }
            return;
        }
        NuLinkedListRemove(&aieditor->free_shared_path_nodes, &shared->link);
        NuLinkedListAppend(&aieditor->shared_path_nodes, &shared->link);
    }
    if (current->shared_node == nullptr) {
        current->shared_node = shared;
        ++shared->reference_count;
    }
    for (i32 i = 0; i < match_count; ++i) {
        EDAIPATHNODE_s *node = matches[i];
        if (node->shared_node != shared) {
            if (node->shared_node != nullptr) {
                EDAISHAREDPATHNODE_s *old_shared = node->shared_node;
                --old_shared->reference_count;
                if (old_shared->reference_count <= 1) {
                    pathEditor_DestroySharedNode(old_shared);
                }
            }
            node->shared_node = shared;
            ++shared->reference_count;
            node->position = current->position;
            node->radius = current->radius;
            node->lower_height = current->lower_height;
            node->upper_height = current->upper_height;
            node->special = current->special;
            node->special_position = current->special_position;
        }
    }
    if (item->data == 0) {
        aieditor_ClearMainMenu();
    }
}

static __used__ void pathEditor_cbShareNodeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *current = aieditor->current_path;
    if (current == nullptr || current->current_node == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelSelectMenu, (char *)"Share node with...");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *all_paths = eduiItemSelCreate(0, &attr, 0, 1, pathEditor_cbSetShareNode, (char *)"All paths");
    eduiMenuAddItem(menu, all_paths);

    i32 index = 1;
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    while (path != nullptr) {
        if (path != aieditor->current_path) {
            i32 selected = 0;
            EDAISHAREDPATHNODE_s *shared = aieditor->current_path->current_node->shared_node;
            if (shared != nullptr) {
                EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
                while (node != nullptr) {
                    if (node->shared_node == shared) {
                        selected = 1;
                        break;
                    }
                    node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
                }
            }
            eduiitem_s *item =
                eduiItemToggleCreate(index, &attr, selected, index + 1, pathEditor_cbSetShareNode, path->name);
            eduiMenuAddItem(menu, item);
            ++index;
        }
        path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void pathEditorCalcRouteIterator(AIPATH_s *path, f32 *distances, u8 *visited, i32 previous, i32 current,
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

static __used__ void pathEditor_cbCnxFlagsToggle(eduimenu_s *, eduiitem_s *item, u32) {
    i32 type_index = item->data;
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr) {
        return;
    }
    EDAIPATHNODE_s *other = path->other_node;
    EDAIPATHNODE_s *node = path->current_node;
    if (other == nullptr || node == nullptr) {
        return;
    }
    for (i32 i = 0; i < 8; ++i) {
        if (node->connections[i].node != other) {
            continue;
        }
        AIPATHCNXTYPE_s *type = &aipathcnxtypes[type_index];
        EDAIPATHCNX_s *connection = &node->connections[i];
        u32 mask = type->connection_flag;
        if (connection->flags & mask) {
            connection->flags &= ~mask;
        } else {
            connection->flags |= mask;
        }
        if (type->flags != 0) {
            for (i32 j = 0; j < 8; ++j) {
                if (other->connections[j].node == node) {
                    if (connection->flags & mask) {
                        other->connections[j].flags |= mask;
                    } else {
                        other->connections[j].flags &= ~mask;
                    }
                    break;
                }
            }
        }
        break;
    }
}

static __used__ void pathEditor_cbDeletePathNode(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0) {
        EDAIPATH_s *path = aieditor->current_path;
        EDAIPATHNODE_s *node = path->current_node;
        if (node != nullptr && node == path->other_node) {
            DestroyAIPathNode(node, path);
            creatureEditor_PathNodeDeleted(node);
            locatorEditor_PathNodeDeleted(node);
            if (AIPathNodeMovedFn != nullptr) {
                AIPathNodeMovedFn(node);
            }
            if (node == aieditor->current_path->current_node) {
                aieditor->current_path->current_node = nullptr;
            }
        }
    }
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbRenameNodeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_node == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelRenameNodeMenu, (char *)"Rename Node");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *item = eduiItemTextPickCreate(0, &attr, pathEditor_cbRenameNode, (char *)"Node Name");
    eduiMenuAddItem(menu, item);
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(edui_last_item);
    strcpy(input->name, aieditor->current_path->current_node->name);
    input->max_name_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void pathEditor_cbRenamePathMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || (path->flags & 1) != 0) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelRenamePathMenu, (char *)"Rename Path");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *item = eduiItemTextPickCreate(0, &attr, pathEditor_cbRenamePath, (char *)"Path Name");
    eduiMenuAddItem(menu, item);
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(edui_last_item);
    strcpy(input->name, aieditor->current_path->name);
    input->max_name_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void pathEditor_cbSelectPathMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    eduimenu_s *menu =
        eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelSelectMenu, (char *)"Select Path");
    if (menu == nullptr) {
        return;
    }
    i32 index = 0;
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    while (path != nullptr) {
        eduiitem_s *item = eduiItemSelCreate(index, &attr, 0, 0, pathEditor_cbSetCurrentPath, path->name);
        eduiMenuAddItem(menu, item);
        ++index;
        path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        i32 index = 0;
        while (path != nullptr && index < item->data) {
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
            ++index;
        }
        if (path != nullptr && item->data >= 0) {
            aieditor->current_path = path;
            EDAIPATHNODE_s *nearest = nullptr;
            f32 best_distance = 3.402823466e38f;
            EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
            while (node != nullptr) {
                NUVEC difference;
                f32 distance = NuVecXZDistSqr(&aieditor->selection_position, &node->position, &difference);
                if (distance < best_distance) {
                    f32 height = aieditor->selection_position.y - node->position.y;
                    f32 upper = NuFmax(0.2f, node->upper_height);
                    f32 lower = NuFmin(-0.2f, node->lower_height);
                    if (height <= upper && height >= lower) {
                        best_distance = distance;
                        nearest = node;
                    }
                }
                node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
            }
            path->other_node = nearest;
            path->current_node = nearest;
            if (nearest != nullptr) {
                edcamSetPos(&nearest->position);
            }
        }
    }
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbNodeFlagsToggle(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATHNODE_s *node = aieditor->current_path->current_node;
    if (node != nullptr) {
        u32 flags = node->flags;
        u32 mask = item->data;
        if ((u8)flags & mask) {
            flags &= ~mask;
        } else {
            flags |= mask;
        }
        node->flags = flags;
        if ((i8)aieditor->current_path->current_node->flags < 0) {
            node->platform.scene = nullptr;
            node->platform.special = nullptr;
            node->platform.display_special = nullptr;
        }
    }
}

static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *, eduimenu_s *menu) {
    eduiMenuDestroy(menu);
}

static __used__ void pathEditor_cbDisconnectPathNode(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0) {
        EDAIPATHNODE_s *node = aieditor->current_path->current_node;
        EDAIPATHNODE_s *other = aieditor->current_path->other_node;
        if (node != nullptr && other != nullptr && node != other) {
            i32 i;
            i32 j;
#define DISCONNECT_OTHER(J)                                                                                            \
    if (other->connections[J].node == node) {                                                                          \
        j = J;                                                                                                         \
        goto disconnect_found;                                                                                         \
    }
#define DISCONNECT_NODE(I)                                                                                             \
    if (node->connections[I].node == other) {                                                                          \
        i = I;                                                                                                         \
        DISCONNECT_OTHER(0)                                                                                            \
        DISCONNECT_OTHER(1)                                                                                            \
        DISCONNECT_OTHER(2)                                                                                            \
        DISCONNECT_OTHER(3)                                                                                            \
        DISCONNECT_OTHER(4)                                                                                            \
        DISCONNECT_OTHER(5)                                                                                            \
        DISCONNECT_OTHER(6)                                                                                            \
        DISCONNECT_OTHER(7)                                                                                            \
    }
            DISCONNECT_NODE(0)
            DISCONNECT_NODE(1)
            DISCONNECT_NODE(2)
            DISCONNECT_NODE(3)
            DISCONNECT_NODE(4)
            DISCONNECT_NODE(5)
            DISCONNECT_NODE(6)
            DISCONNECT_NODE(7)
#undef DISCONNECT_NODE
#undef DISCONNECT_OTHER
            goto disconnected;
        disconnect_found:
            memset(&node->connections[i], 0, sizeof(EDAIPATHCNX_s));
            memset(&other->connections[j], 0, sizeof(EDAIPATHCNX_s));
        disconnected:
            creatureEditor_PathNodeDeleted(aieditor->current_path->current_node);
            locatorEditor_PathNodeDeleted(aieditor->current_path->current_node);
            if (AIPathNodeMovedFn != nullptr) {
                AIPathNodeMovedFn(aieditor->current_path->current_node);
            }
        }
    }
    aieditor_ClearMainMenu();
}

static __used__ f32 **pathEditorCalculateDistanceTable(AIPATH_s *path, i32 route_mask, variptr_u *cursor,
                                                       variptr_u *end) {
    if (distance_tables == 0 || path == nullptr || path->nodes == nullptr) {
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
static __used__ void pathEditorCreateSpecialRouteData(AIPATH_s *path, EDAIPATH_s *editor_path, VARIPTR *cursor,
                                                      VARIPTR *end) {
    i32 route_slots[16];
    i32 route_count = 0;
    for (i32 slot = 0; slot < 16; ++slot) {
        if (editor_path->routes[slot].flags & 1) {
            route_slots[route_count++] = slot;
        }
    }
    path->route_count = route_count;
    if (route_count == 0) {
        return;
    }
    path->routes = (AIPATHROUTE_s *)AISysBufferAlloc(cursor, end, route_count * sizeof(AIPATHROUTE_s));
    if (path->routes == nullptr) {
        return;
    }
    memset(path->routes, 0, route_count * sizeof(AIPATHROUTE_s));
    for (i32 route_index = 0; route_index < route_count; ++route_index) {
        EDAIPATHROUTE_s *editor_route = &editor_path->routes[route_slots[route_index]];
        AIPATHROUTE_s *route = &path->routes[route_index];
        i32 name_length = strlen(editor_route->name);
        if (name_length != 0) {
            route->name = (char *)AISysBufferAlloc(cursor, end, name_length + 1);
            if (route->name != nullptr) {
                strcpy(route->name, editor_route->name);
            }
        }
        route->character_masks[0] = (editor_route->user_mask & (1ULL << 63)) ? ~0ULL : editor_route->user_mask;
        route->character_masks[1] = editor_route->user_mask;
    }

    for (EDAIPATHNODE_s *editor_node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes);
         editor_node != nullptr;
         editor_node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &editor_node->link)) {
        AIPATHNODE_s *runtime_node = &path->nodes[editor_node->index];
        for (i32 slot = 0; slot < 8; ++slot) {
            EDAIPATHCNX_s *editor_connection = &editor_node->connections[slot];
            if (editor_connection->node == nullptr || editor_connection->route_mask == 0) {
                continue;
            }
            for (i32 edge = 0; edge < runtime_node->connection_count; ++edge) {
                AIPATHCNX_s *connection = runtime_node->connections[edge];
                if (connection == nullptr || (connection->node_indices[0] != editor_connection->node->index &&
                                              connection->node_indices[1] != editor_connection->node->index)) {
                    continue;
                }
                for (i32 route_index = 0; route_index < route_count; ++route_index) {
                    if (editor_connection->route_mask & (1u << route_slots[route_index])) {
                        connection->route_mask |= 1u << route_index;
                    }
                }
                break;
            }
        }
    }

    for (i32 route_index = 0; route_index < route_count; ++route_index) {
        AIPATHROUTE_s *route = &path->routes[route_index];
        i32 slot = route_slots[route_index];
        u8 members[256];
        memset(members, 0, sizeof(members));
        for (i32 edge = 0; edge < path->connection_count; ++edge) {
            AIPATHCNX_s *connection = &path->connections[edge];
            if (connection->route_mask & (1 << route_index)) {
                members[connection->node_indices[0]] = 1;
                members[connection->node_indices[1]] = 1;
            }
        }
        i32 exits = 0;
        for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes); node != nullptr;
             node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
            if (node->route_mask & (1 << slot)) {
                path->nodes[node->index].route_boundary_mask |= 1 << route_index;
            }
        }
        for (i32 node = 0; node < path->node_count; ++node) {
            if (members[node]) {
                ++route->route_count;
                path->nodes[node].route_membership_mask |= 1 << route_index;
            }
            exits += (path->nodes[node].route_boundary_mask & (1 << route_index)) != 0;
        }
        route->exit_node_count = exits;
        if (route->route_count == 0) {
            continue;
        }
        route->node_routes = (u8 *)AISysBufferAlloc(cursor, end, path->node_count);
        route->node_directions = (u8 *)AISysBufferAlloc(cursor, end, route->route_count);
        if (route->node_routes == nullptr || route->node_directions == nullptr) {
            continue;
        }
        memset(route->node_routes, 0xff, path->node_count);
        memset(route->node_directions, 0, route->route_count);
        if (exits != 0) {
            route->exit_nodes = (u8 *)AISysBufferAlloc(cursor, end, exits);
            if (route->exit_nodes != nullptr) {
                memset(route->exit_nodes, 0, exits);
            }
        }
        route->route_nodes = (u8 **)AISysBufferAlloc(cursor, end, route->route_count * sizeof(u8 *));
        if (route->route_nodes == nullptr) {
            continue;
        }
        memset(route->route_nodes, 0, route->route_count * sizeof(u8 *));
        for (i32 source = 0; source < route->route_count; ++source) {
            route->route_nodes[source] = (u8 *)AISysBufferAlloc(cursor, end, route->route_count);
            if (route->route_nodes[source] != nullptr) {
                memset(route->route_nodes[source], 0, route->route_count);
            }
        }
        i32 member_index = 0;
        i32 exit_index = 0;
        for (i32 node = 0; node < path->node_count; ++node) {
            if (!members[node]) {
                continue;
            }
            route->node_routes[node] = member_index;
            route->node_directions[member_index++] = node;
            if ((path->nodes[node].route_boundary_mask & (1 << route_index)) && route->exit_nodes != nullptr) {
                route->exit_nodes[exit_index++] = node;
            }
        }
        // A route-specific table permits only edges belonging to this route.
        f32 **distances = pathEditorCalculateDistanceTable(path, 1 << route_index, cursor, end);
        if (distances == nullptr) {
            continue;
        }
        for (i32 source = 0; source < route->route_count; ++source) {
            if (route->route_nodes[source] == nullptr) {
                continue;
            }
            i32 source_node = route->node_directions[source];
            for (i32 destination = 0; destination < route->route_count; ++destination) {
                i32 destination_node = route->node_directions[destination];
                if (source == destination) {
                    route->route_nodes[source][destination] = 0xff;
                    continue;
                }
                f32 best = FLT_MAX;
                u8 next = 0xff;
                AIPATHNODE_s *node = &path->nodes[source_node];
                for (i32 edge = 0; edge < node->connection_count; ++edge) {
                    AIPATHCNX_s *connection = node->connections[edge];
                    i32 direction = connection->node_indices[0] != source_node;
                    if (!(connection->route_mask & (1 << route_index)) ||
                        (connection->traversal_flags[direction] & 0x40000000)) {
                        continue;
                    }
                    i32 neighbor = connection->node_indices[!direction];
                    f32 distance = distances[source_node][neighbor] + distances[neighbor][destination_node];
                    if (distance < best) {
                        best = distance;
                        next = route->node_routes[neighbor];
                    }
                }
                route->route_nodes[source][destination] = next;
            }
        }
    }
}

static __used__ void pathEditor_cbCancelDeleteAreaMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbCancelDeleteNodeMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *, eduimenu_s *menu) {
    eduiMenuDestroy(menu);
}

static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *, eduimenu_s *menu) {
    eduiMenuDestroy(menu);
}

static __used__ void pathEditor_cbDrawWallsplinesToggle(eduimenu_s *, eduiitem_s *item, u32) {
    aieditorsettings.draw_wallsplines = item->highlighted;
}

static __used__ void pathEditor_cbCancelDisconnectNodeMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *, eduimenu_s *);
static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *, u32);

static __used__ void routeEditor_cbRouteUsers(eduimenu_s *parent, eduiitem_s *, u32) {
    if (SpecialRouteCharacterNameFn == nullptr || aieditor->current_path == nullptr ||
        aieditor->current_path->current_route == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, routeEditor_cbCancelRouteUsers, (char *)"Route Users");
    if (menu == nullptr) {
        return;
    }
    i32 index = 0;
    char *name = SpecialRouteCharacterNameFn(0);
    while (index < 64 && name != nullptr) {
        i32 selected = (aieditor->current_path->current_route->user_mask >> index) & 1;
        eduiitem_s *item = eduiItemCheckCreate(index, &attr, selected, index + 1, routeEditor_cbSetRouteUsers, name);
        eduiMenuAddItem(menu, item);
        eduiMenuAttach(parent, menu);
        ++index;
        name = SpecialRouteCharacterNameFn(index);
    }
    if (index < 64) {
        i32 selected = (aieditor->current_path->current_route->user_mask >> 63) & 1;
        eduiitem_s *item = eduiItemCheckCreate(63, &attr, selected, 64, routeEditor_cbSetRouteUsers, (char *)"Global");
        eduiMenuAddItem(menu, item);
    }
}

static __used__ void routeEditor_cbCreateRoute(eduimenu_s *, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    i32 index = path->current_route != nullptr ? path->current_route - path->routes + 1 : 1;
    // The original checks each of the fixed 16 slots in sequence.
#define ROUTE_EDITOR_TRY_FREE()                                                                                        \
    if (index >= 16)                                                                                                   \
        index = 0;                                                                                                     \
    if (!(path->routes[index].flags & 1))                                                                              \
        goto route_found;                                                                                              \
    ++index
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
#undef ROUTE_EDITOR_TRY_FREE
    return;
route_found:
    EDAIPATHROUTE_s *route = &path->routes[index];
    route->flags |= 1;
    route->user_mask = 0;
    path->current_route = route;
    char name[16];
    i32 number = 0;
next_name:
    sprintf(name, "route%d", ++number);
#define ROUTE_EDITOR_CHECK_NAME(N)                                                                                     \
    if ((path->routes[N].flags & 1) && NuStrICmp(name, path->routes[N].name) == 0)                                     \
    goto next_name
    ROUTE_EDITOR_CHECK_NAME(0);
    ROUTE_EDITOR_CHECK_NAME(1);
    ROUTE_EDITOR_CHECK_NAME(2);
    ROUTE_EDITOR_CHECK_NAME(3);
    ROUTE_EDITOR_CHECK_NAME(4);
    ROUTE_EDITOR_CHECK_NAME(5);
    ROUTE_EDITOR_CHECK_NAME(6);
    ROUTE_EDITOR_CHECK_NAME(7);
    ROUTE_EDITOR_CHECK_NAME(8);
    ROUTE_EDITOR_CHECK_NAME(9);
    ROUTE_EDITOR_CHECK_NAME(10);
    ROUTE_EDITOR_CHECK_NAME(11);
    ROUTE_EDITOR_CHECK_NAME(12);
    ROUTE_EDITOR_CHECK_NAME(13);
    ROUTE_EDITOR_CHECK_NAME(14);
    ROUTE_EDITOR_CHECK_NAME(15);
#undef ROUTE_EDITOR_CHECK_NAME
    strcpy(route->name, name);
    aieditor_ClearMainMenu();
}

static __used__ void routeEditor_cbDeleteRoute(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (item == nullptr) {
        return;
    }
    switch (item->data) {
        case 0: {
            eduimenu_s *menu = eduiMenuCreate(240, 90, 240, 250, ed_fnt, nullptr, "Delete current route??");
            if (menu != nullptr) {
                eduiMenuAddItem(menu, eduiItemSelCreate(2, attr, 0, 0, routeEditor_cbDeleteRoute, "No"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbDeleteRoute, "Yes"));
                eduiMenuAttach(parent, menu);
            }
            break;
        }
        case 2:
            eduiMenuDestroy(parent);
            break;
        case 1: {
            EDAIPATH_s *path = aieditor->current_path;
            EDAIPATHROUTE_s *route = path->current_route;
            if (route == nullptr) {
                return;
            }
            i32 index = route - path->routes;
            route->flags &= ~u8(1);
            path->current_route = nullptr;
            u16 mask = ~(u16)(1 << index);
            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes); node != nullptr;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link)) {
#define CLEAR_CONNECTION_ROUTE(N) node->connections[N].route_mask &= mask
                CLEAR_CONNECTION_ROUTE(0);
                CLEAR_CONNECTION_ROUTE(1);
                CLEAR_CONNECTION_ROUTE(2);
                CLEAR_CONNECTION_ROUTE(3);
                CLEAR_CONNECTION_ROUTE(4);
                CLEAR_CONNECTION_ROUTE(5);
                CLEAR_CONNECTION_ROUTE(6);
                CLEAR_CONNECTION_ROUTE(7);
#undef CLEAR_CONNECTION_ROUTE
                node->route_mask &= mask;
            }
            i32 next_index = index;
#define TRY_NEXT_ROUTE()                                                                                               \
    ++next_index;                                                                                                      \
    if (next_index >= 16)                                                                                              \
        next_index = 0;                                                                                                \
    if (path->routes[next_index].flags & 1)                                                                            \
    path->current_route = &path->routes[next_index]
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
#undef TRY_NEXT_ROUTE
            aieditor_ClearMainMenu();
            break;
        }
    }
}

static __used__ void routeEditor_cbRenameRoute(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_route == nullptr) {
        return;
    }
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(item);
    if (input->name[0] == '\0') {
        return;
    }
    for (i32 index = 0; index < 16; ++index) {
        if (NuStrICmp(path->routes[index].name, input->name) == 0) {
            return;
        }
    }
    strcpy(aieditor->current_path->current_route->name, input->name);
}

static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *, eduimenu_s *);

static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *item, u32) {
    if (item == nullptr || aieditor->current_path == nullptr) {
        return;
    }
    EDAIPATHROUTE_s *route = aieditor->current_path->current_route;
    if (route == nullptr || (u32)item->data >= 64) {
        return;
    }
    if ((route->user_mask >> item->data) & 1) {
        route->user_mask &= ~(1ULL << item->data);
        item->highlighted = 0;
    } else {
        route->user_mask |= 1ULL << item->data;
        item->highlighted = 1;
    }
}

static __used__ void routeEditor_cbRenameRouteMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_route == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, routeEditor_cbCancelRenameRouteMenu, (char *)"Rename Route");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *item = eduiItemTextPickCreate(0, &attr, routeEditor_cbRenameRoute, (char *)"Route Name");
    eduiMenuAddItem(menu, item);
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(edui_last_item);
    strcpy(input->name, aieditor->current_path->current_route->name);
    input->max_name_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *, eduimenu_s *menu) {
    eduiMenuDestroy(menu);
}

static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *, eduimenu_s *menu) {
    eduiMenuDestroy(menu);
}

extern "C" {

    AIPATHSYS_s *pathEditorCreateData(VARIPTR *cursor, VARIPTR *end, void **scratch_cursor, void **scratch_end) {
        iterator_count = 0;
        VARIPTR scratch;
        scratch.void_ptr = *scratch_cursor;
        if (aieditor->current_path == nullptr) {
            return nullptr;
        }
        if (NuLinkedListGetHead(&aieditor->paths) == nullptr) {
            return nullptr;
        }

        i32 path_count = 0;
        for (EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); path != nullptr;
             path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link)) {
            path->draw_index = path_count++;
        }
        AIPATHSYS_s *system = (AIPATHSYS_s *)AISysBufferAlloc(cursor, end, sizeof(AIPATHSYS_s));
        if (system == nullptr) {
            return nullptr;
        }
        memset(system, 0, sizeof(*system));
        system->path_count = path_count;
        distance_tables = nullptr;
        VARIPTR scratch_limit;
        scratch_limit.void_ptr = *scratch_end;
        distance_tables = (f32 ***)AISysBufferAlloc(&scratch, &scratch_limit, path_count * sizeof(f32 **));
        if (distance_tables != nullptr) {
            memset(distance_tables, 0, path_count * sizeof(f32 **));
        }

        i32 shared_count = 0;
        for (EDAISHAREDPATHNODE_s *shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->shared_path_nodes);
             shared != nullptr;
             shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetNext(&aieditor->shared_path_nodes, &shared->link)) {
            shared->runtime_index = shared_count++;
        }
        system->special_route_count = shared_count;
        if (shared_count != 0) {
            system->special_routes = (AIPATHSPECIALROUTE_s *)AISysBufferAlloc(
                &scratch, &scratch_limit, shared_count * sizeof(AIPATHSPECIALROUTE_s));
            if (system->special_routes == nullptr) {
                return nullptr;
            }
            memset(system->special_routes, 0, shared_count * sizeof(AIPATHSPECIALROUTE_s));
            for (EDAISHAREDPATHNODE_s *shared =
                     (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->shared_path_nodes);
                 shared != nullptr;
                 shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetNext(&aieditor->shared_path_nodes, &shared->link)) {
                AIPATHSPECIALROUTE_s *route = &system->special_routes[shared->runtime_index];
                i32 participants = shared->reference_count;
                route->paths = (AIPATH_s **)AISysBufferAlloc(&scratch, &scratch_limit, participants * sizeof(AIPATH_s));
                if (route->paths == nullptr) {
                    return nullptr;
                }
                memset(route->paths, 0, participants * sizeof(AIPATH_s));
            }
        }
        system->paths = (AIPATH_s **)AISysBufferAlloc(cursor, end, path_count * sizeof(AIPATH_s *));
        if (system->paths == nullptr) {
            return nullptr;
        }
        memset(system->paths, 0, path_count * sizeof(AIPATH_s *));

        i32 path_index = 0;
        for (EDAIPATH_s *editor_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); editor_path != nullptr;
             editor_path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &editor_path->link), ++path_index) {
            AIPATH_s *path = (AIPATH_s *)AISysBufferAlloc(cursor, end, sizeof(AIPATH_s));
            system->paths[path_index] = path;
            if (path == nullptr) {
                distance_tables = nullptr;
                return nullptr;
            }
            memset(path, 0, sizeof(*path));
            strcpy(path->name, editor_path->name);
            path->index = path_index;

            i32 edge_count = 0;
            i32 special_count = 0;
            i32 node_index = 0;
            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes); node != nullptr;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
                node->index = node_index++;
                for (i32 slot = 0; slot < 8; ++slot) {
                    edge_count += node->connections[slot].node != nullptr;
                }
                special_count += node->shared_node != nullptr;
            }
            path->node_count = node_index;
            path->connection_count = edge_count / 2;
            path->special_route_count = special_count;
            if (path->connection_count != 0) {
                path->connections =
                    (AIPATHCNX_s *)AISysBufferAlloc(cursor, end, path->connection_count * sizeof(AIPATHCNX_s));
                if (path->connections != nullptr) {
                    memset(path->connections, 0, path->connection_count * sizeof(AIPATHCNX_s));
                } else {
                    distance_tables = nullptr;
                    return nullptr;
                }
            }
            if (path->special_route_count != 0) {
                path->special_routes = (AIPATHNODELINK_s *)AISysBufferAlloc(
                    cursor, end, path->special_route_count * sizeof(AIPATHNODELINK_s));
                if (path->special_routes != nullptr) {
                    memset(path->special_routes, 0, path->special_route_count * sizeof(AIPATHNODELINK_s));
                } else {
                    distance_tables = nullptr;
                    return nullptr;
                }
            }
            if (path->node_count != 0) {
                path->nodes = (AIPATHNODE_s *)AISysBufferAlloc(cursor, end, path->node_count * sizeof(AIPATHNODE_s));
                if (path->nodes != nullptr) {
                    memset(path->nodes, 0, path->node_count * sizeof(AIPATHNODE_s));
                } else {
                    distance_tables = nullptr;
                    return nullptr;
                }
            }

            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes); node != nullptr;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
                if (path->nodes == nullptr) {
                    break;
                }
                AIPATHNODE_s *runtime = &path->nodes[node->index];
                if (node->name[0] != '\0') {
                    i32 length = strlen(node->name) + 1;
                    runtime->name = (char *)AISysBufferAlloc(cursor, end, length);
                    if (runtime->name != nullptr) {
                        strcpy(runtime->name, node->name);
                    }
                }
                runtime->position = node->position;
                runtime->radius = node->radius;
                runtime->radius_squared = node->radius * node->radius;
                runtime->min_height = node->position.y + node->lower_height;
                runtime->min_height_offset = node->lower_height;
                runtime->max_height = node->position.y + node->upper_height;
                runtime->max_height_offset = node->upper_height;
                runtime->runtime_flags = node->flags;
                runtime->special_handle = node->special;
                runtime->special_position = node->special_position;
                runtime->has_special = NuSpecialExistsFn(&runtime->special_handle) != 0;
                for (i32 slot = 0; slot < 8; ++slot) {
                    runtime->connection_count += node->connections[slot].node != nullptr;
                }
                runtime->distance_cache_nodes[0] = 0xff;
                runtime->distance_cache_nodes[1] = 0xff;
            }

            i32 connection_index = 0;
            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes); node != nullptr;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
                AIPATHNODE_s *runtime = &path->nodes[node->index];
                if (runtime->connection_count != 0) {
                    runtime->connections = (AIPATHCNX_s **)AISysBufferAlloc(
                        cursor, end, runtime->connection_count * sizeof(AIPATHCNX_s *));
                    if (runtime->connections == nullptr) {
                        distance_tables = nullptr;
                        return nullptr;
                    }
                    memset(runtime->connections, 0, runtime->connection_count * sizeof(AIPATHCNX_s *));
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
                                if (connection != nullptr && connection->node_indices[1] == node->index) {
                                    runtime->connections[next] = connection;
                                    connection->traversal_flags[1] = editor_connection->flags;
                                    connection->original_traversal_flags[1] = editor_connection->flags;
                                    break;
                                }
                            }
                        } else {
                            AIPATHCNX_s *connection = &path->connections[connection_index++];
                            runtime->connections[next] = connection;
                            connection->node_indices[0] = node->index;
                            connection->node_indices[1] = other->index;
                            connection->traversal_flags[0] = editor_connection->flags;
                            connection->original_traversal_flags[0] = editor_connection->flags;
                            NUVEC difference;
                            connection->distance = NuVecDist(&other->position, &node->position, &difference);
                            connection->horizontal_distance =
                                NuVecXZDist(&other->position, &node->position, &difference);
                            if (connection->horizontal_distance == 0.0f) {
                                connection->horizontal_distance = 0.0001f;
                            }
                            connection->rotation = (i16)(NuAtan2(difference.x, difference.z) * 10430.378f);
                        }
                        ++next;
                    }
                }
                if (node->shared_node != nullptr && path->special_routes != nullptr) {
                    AIPATHNODELINK_s *link =
                        &path->special_routes[runtime->special_route_index =
                                                  (u8)(path->special_route_count - special_count--)];
                    link->node_index = node->index;
                    link->special_route_index = node->shared_node->runtime_index;
                    AIPATHSPECIALROUTE_s *shared_route = &system->special_routes[node->shared_node->runtime_index];
                    if (shared_route->paths != nullptr &&
                        shared_route->path_count < node->shared_node->reference_count) {
                        shared_route->paths[shared_route->path_count++] = path;
                    }
                } else {
                    runtime->special_route_index = 0xff;
                }
            }
            if (distance_tables != nullptr) {
                distance_tables[path_index] = pathEditorCalculateDistanceTable(path, 0, &scratch, &scratch_limit);
            }

            if (path->node_count != 0) {
                path->route_matrix = (u8 **)AISysBufferAlloc(cursor, end, path->node_count * sizeof(u8 *));
                if (path->route_matrix == nullptr) {
                    distance_tables = nullptr;
                    return nullptr;
                }
                memset(path->route_matrix, 0, path->node_count * sizeof(u8 *));
                {
                    for (i32 source = 0; source < path->node_count; ++source) {
                        path->route_matrix[source] = (u8 *)AISysBufferAlloc(cursor, end, path->node_count);
                        if (path->route_matrix[source] == nullptr) {
                            distance_tables = nullptr;
                            return nullptr;
                        }
                        memset(path->route_matrix[source], 0, path->node_count);
                        if (distance_tables == nullptr || distance_tables[path_index] == nullptr) {
                            continue;
                        }
                        for (i32 destination = 0; destination < path->node_count; ++destination) {
                            path->route_matrix[source][destination] = 0xff;
                            if (destination == source) {
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
                                i32 neighbor = connection->node_indices[!direction];
                                f32 distance = distance_tables[path_index][source][neighbor] +
                                               distance_tables[path_index][neighbor][destination];
                                if (distance < best) {
                                    best = distance;
                                    path->route_matrix[source][destination] = edge;
                                }
                            }
                        }
                    }
                }
            }
            pathEditorCreateSpecialRouteData(path, editor_path, cursor, end);
            if (path->route_count != 0 && path->routes == nullptr) {
                distance_tables = nullptr;
                return nullptr;
            }
        }
        if (path_count > 1 && system->paths[0] != nullptr) {
            AIPATHINFO info;
            memset(&info, 0, sizeof(info));
            AIPATH_s *first_path = system->paths[0];
            for (i32 path_index = 1; path_index < path_count; ++path_index) {
                AIPATH_s *path = system->paths[path_index];
                if (path == nullptr || path->nodes == nullptr) {
                    continue;
                }
                for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
                    AIPATHNODE_s *node = &path->nodes[node_index];
                    AISysGetPathPos(aieditor->ai_system, &node->position, &info, first_path, 0xff);
                    if (!info.on_path || info.connection == nullptr || info.path == nullptr) {
                        node->path_flags = -1;
                        node->runtime_flags &= ~u8(1);
                        continue;
                    }
                    node->path_flags = info.connection - path->connections;
                    if (info.dist > 0.5f || (info.direction != 0 && info.dist < 0.5f)) {
                        node->runtime_flags |= 4;
                    }
                    node->runtime_flags |= 1;
                    node->path_flags = info.connection - info.path->connections;
                    path->flags |= 2;
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
                        if (candidate != node_index && (path->nodes[candidate].runtime_flags & 1) &&
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

    void pathEditorDrawPaths(void) {
        AIEDITOR_RENDER_STATE *state = aieditor;
        EDAIPATHWALL_s *wall = (EDAIPATHWALL_s *)NuLinkedListGetHead(&state->path_walls);
        while (wall != nullptr) {
            wall->flags &= ~u8(1);
            wall = (EDAIPATHWALL_s *)NuLinkedListGetNext(&state->path_walls, &wall->link);
        }

        if (aieditorsettings.draw_all_paths) {
            i32 index = 0;
            EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&state->paths);
            while (path != nullptr) {
                path->draw_index = index++;
                path = (EDAIPATH_s *)NuLinkedListGetNext(&state->paths, &path->link);
            }
            if (state->current_path != nullptr) {
                pathEditorDrawPath(state->current_path, state->current_path->draw_index);
            }
            path = (EDAIPATH_s *)NuLinkedListGetHead(&state->paths);
            while (path != nullptr) {
                if (path != state->current_path) {
                    pathEditorDrawPath(path, path->draw_index);
                }
                path = (EDAIPATH_s *)NuLinkedListGetNext(&state->paths, &path->link);
            }
        } else {
            pathEditorDrawPath(state->current_path, 0);
        }
    }

    void pathEditorSaveData(AIPATHSYS_s *system) {
        EdFileWriteInt(system->path_count);
        for (i32 path_index = 0; path_index < system->path_count; ++path_index) {
            AIPATH_s *path = system->paths[path_index];
            EdFileWrite(path->name, 16);
            EdFileWriteChar(path->node_count);
            EdFileWriteChar(path->flags);
            EdFileWriteShort(path->connection_count);
            for (i32 index = 0; index < path->connection_count; ++index) {
                AIPATHCNX_s *connection = &path->connections[index];
                EdFileWriteChar(connection->node_indices[0]);
                EdFileWriteChar(connection->node_indices[1]);
                if (aidata_version > 11) {
                    EdFileWriteInt(connection->traversal_flags[0]);
                    EdFileWriteInt(connection->traversal_flags[1]);
                } else if (aidata_version > 8) {
                    EdFileWriteShort(connection->traversal_flags[0]);
                    EdFileWriteShort(connection->traversal_flags[1]);
                } else {
                    EdFileWriteChar(connection->traversal_flags[0]);
                    EdFileWriteChar(connection->traversal_flags[1]);
                }
                EdFileWriteShort(connection->rotation);
                EdFileWriteShort(connection->route_mask);
                EdFileWriteFloat(connection->distance);
                EdFileWriteFloat(connection->horizontal_distance);
            }
            for (i32 index = 0; index < path->node_count; ++index) {
                AIPATHNODE_s *node = &path->nodes[index];
                i32 length = node->name != nullptr ? strlen(node->name) + 1 : 0;
                EdFileWriteInt(length);
                if (length != 0) {
                    EdFileWrite(node->name, length);
                }
                EdFileWriteFloat(node->position.x);
                EdFileWriteFloat(node->position.y);
                EdFileWriteFloat(node->position.z);
                EdFileWriteFloat(node->radius);
                if (aidata_version > 7) {
                    EdFileWriteFloat(node->min_height);
                    EdFileWriteFloat(node->max_height);
                }
                EdFileWriteChar(node->connection_count);
                EdFileWriteChar(node->flags);
                EdFileWriteChar(0);
                EdFileWriteChar(node->runtime_flags);
                EdFileWriteShort(node->path_flags);
                if (aidata_version > 18) {
                    EdFileWriteChar(node->special_route_index);
                } else {
                    EdFileWriteChar(0);
                }
                char *special_name = nullptr;
                if (NuSpecialExistsFn(&node->special_handle)) {
                    special_name = NuSpecialGetName(&node->special_handle);
                }
                if (special_name != nullptr) {
                    i32 special_length = strlen(special_name) + 1;
                    EdFileWriteChar(special_length);
                    EdFileWrite(special_name, special_length);
                    EdFileWriteFloat(node->special_position.x);
                    EdFileWriteFloat(node->special_position.y);
                    EdFileWriteFloat(node->special_position.z);
                } else {
                    EdFileWriteChar(0);
                }
                for (i32 connection = 0; connection < node->connection_count; ++connection) {
                    EdFileWriteShort(node->connections[connection] - path->connections);
                }
                if (node->connection_count & 1) {
                    EdFileWriteShort(0);
                }
                EdFileWriteShort(node->route_membership_mask);
                EdFileWriteShort(node->route_boundary_mask);
            }
            for (i32 index = 0; index < path->node_count; ++index) {
                EdFileWrite(path->route_matrix[index], path->node_count);
            }
            EdFileWriteChar(path->route_count);
            for (i32 index = 0; index < path->route_count; ++index) {
                AIPATHROUTE_s *route = &path->routes[index];
                i32 length = strlen(route->name) + 1;
                EdFileWriteChar(length);
                EdFileWrite(route->name, length);
                EdFileWriteChar(route->route_count);
                EdFileWriteChar(route->exit_node_count);
                EdFileWriteChar(0);
                EdFileWriteChar(0);
                if (route->route_count != 0) {
                    EdFileWrite(route->node_routes, path->node_count);
                    EdFileWrite(route->node_directions, route->route_count);
                    for (i32 node = 0; node < route->route_count; ++node) {
                        EdFileWrite(route->route_nodes[node], route->route_count);
                    }
                    if (route->exit_node_count != 0) {
                        EdFileWrite(route->exit_nodes, route->exit_node_count);
                    }
                }
                if (SpecialRouteCharacterNameFn != nullptr) {
                    i32 user_count = 0;
                    for (i32 user = 0; user < 64; ++user) {
                        user_count += (route->character_masks[1] >> user) & 1;
                    }
                    EdFileWriteChar(user_count);
                    for (i32 user = 0; user < 63; ++user) {
                        if ((route->character_masks[1] >> user) & 1) {
                            char *name = SpecialRouteCharacterNameFn(user);
                            if (name != nullptr) {
                                i32 name_length = strlen(name) + 1;
                                EdFileWriteChar(name_length);
                                EdFileWrite(name, name_length);
                            } else {
                                EdFileWriteChar(0);
                            }
                        }
                    }
                    if ((route->character_masks[1] >> 63) & 1) {
                        EdFileWriteChar(9);
                        EdFileWrite((char *)"Everyone", 9);
                    }
                } else {
                    EdFileWriteChar(0);
                }
            }
            if (aidata_version > 18) {
                EdFileWriteChar(path->special_route_count);
                AIPATHNODELINK_s *link = path->special_routes;
                for (i32 index = 0; index < path->special_route_count; ++index, ++link) {
                    EdFileWriteUnsignedChar(link->node_index);
                    EdFileWriteShort(link->special_route_index);
                }
            }
        }
        if (aidata_version > 18) {
            EdFileWriteShort(system->special_route_count);
            AIPATHSPECIALROUTE_s *route = system->special_routes;
            for (i32 index = 0; index < system->special_route_count; ++index, ++route) {
                EdFileWriteChar(route->path_count);
                for (i32 path = 0; path < route->path_count; ++path) {
                    EdFileWriteChar(route->paths[path]->index);
                }
            }
        }
    }

    void pathEditor_CalcNodeIXs(void) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (path != nullptr) {
            i32 index = 0;
            EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
            while (node != nullptr) {
                node->index = index++;
                node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
            }
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
        }
    }

    EDAIPATH_s *pathEditor_GetPath(const char *name) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (path != nullptr && name != nullptr) {
            if (NuStrICmp(name, path->name) == 0) {
                return path;
            }
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
        }
        return aieditor->current_path;
    }

    void pathEditor_OnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *result, EDAIPATH_s *path, f32 tolerance) {
        u8 checked[255][32];
        memset(checked, 0, sizeof(checked));
        memset(result, 0, sizeof(*result));
        if (path == nullptr) {
            return;
        }
        EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
        while (node != nullptr) {
#define CHECK_PATH_CONNECTION(slot)                                                                                    \
    {                                                                                                                  \
        EDAIPATHNODE_s *other = node->connections[slot].node;                                                          \
        if (other != nullptr && !(checked[node->index][other->index / 8] & (1 << (other->index % 8)))) {               \
            checked[node->index][other->index / 8] |= 1 << (other->index % 8);                                         \
            checked[other->index][node->index / 8] |= 1 << (node->index % 8);                                          \
            f32 fraction;                                                                                              \
            f32 width;                                                                                                 \
            i32 angle;                                                                                                 \
            if (TestPointPathCheck(point, node, other, &fraction, &width, &angle, tolerance)) {                        \
                result->on_path = 1;                                                                                   \
                result->path = path;                                                                                   \
                result->first = node;                                                                                  \
                result->second = other;                                                                                \
                result->fraction = fraction;                                                                           \
                result->width = width;                                                                                 \
                result->angle = angle;                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
    }
            CHECK_PATH_CONNECTION(0);
            CHECK_PATH_CONNECTION(1);
            CHECK_PATH_CONNECTION(2);
            CHECK_PATH_CONNECTION(3);
            CHECK_PATH_CONNECTION(4);
            CHECK_PATH_CONNECTION(5);
            CHECK_PATH_CONNECTION(6);
            CHECK_PATH_CONNECTION(7);
#undef CHECK_PATH_CONNECTION
            node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
        }
    }

    void pathEditor_QuickOnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *previous, EDAIPATHCHECK_s *result) {
        result->on_path = 0;
        if (TestPointPathCheck(point, previous->first, previous->second, &result->fraction, &result->width,
                               &result->angle, 0.0f)) {
            result->on_path = 1;
            return;
        }
        for (i32 index = 0; index < 8; ++index) {
            EDAIPATHNODE_s *other = previous->first->connections[index].node;
            if (other != nullptr && TestPointPathCheck(point, other, previous->first, &result->fraction, &result->width,
                                                       &result->angle, 0.0f)) {
                result->on_path = 1;
                return;
            }
            other = previous->second->connections[index].node;
            if (other != nullptr && TestPointPathCheck(point, previous->second, other, &result->fraction,
                                                       &result->width, &result->angle, 0.0f)) {
                result->on_path = 1;
                return;
            }
        }
    }

    void pathEditor_UpdateNodesOnPlatforms(void) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (path != nullptr) {
            EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
            while (node != nullptr) {
                if (NuSpecialExistsFn(&node->special)) {
                    NuVecMtxTransform(&node->position, &node->special_position, NuSpecialGetDrawMtx(&node->special));
                }
                node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
            }
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
        }
    }

} // extern "C"

extern "C" void pathEditor_CalcNodeIXs(void);
extern "C" EDAIPATH_s *pathEditor_GetPath(const char *name);

void pathEditor_Enter(void) {
    aieditor->paths.head = nullptr;
    aieditor->paths.tail = nullptr;
    for (i32 index = 0; index < 32; ++index) {
        NuLinkedListAppend(&aieditor->free_paths, &aieditor->path_storage[index].link);
    }
    aieditor->free_path_nodes.head = nullptr;
    aieditor->free_path_nodes.tail = nullptr;
    for (i32 index = 0; index < 1024; ++index) {
        NuLinkedListAppend(&aieditor->free_path_nodes, &aieditor->node_storage[index].link);
    }
    aieditor->free_shared_nodes.head = nullptr;
    aieditor->free_shared_nodes.tail = nullptr;
    for (i32 index = 0; index < 64; ++index) {
        NuLinkedListAppend(&aieditor->free_shared_nodes, &aieditor->shared_node_storage[index].link);
    }

    EDAISHAREDPATHNODE_s *shared_nodes[64];
    memset(shared_nodes, 0, sizeof(shared_nodes));
    AIPATHSYS_s *runtime_system = aieditor->ai_system != nullptr ? aieditor->ai_system->path_sys : nullptr;
    bool rebuild_paths = aieditor->cached_path_system != runtime_system;
    if (rebuild_paths) {
        aieditor->cached_path_system = runtime_system;
    }
    if (rebuild_paths && runtime_system != nullptr && runtime_system->path_count != 0) {
        i32 node_storage_start = 0;
        for (i32 path_index = 0; path_index < runtime_system->path_count; ++path_index) {
            AIPATH_s *runtime_path = runtime_system->paths[path_index];
            EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->free_paths);
            if (runtime_path == nullptr || path == nullptr) {
                break;
            }
            NuLinkedListRemove(&aieditor->free_paths, &path->link);
            NuLinkedListAppend(&aieditor->paths, &path->link);
            strcpy(path->name, runtime_path->name);
            if (NuStrICmp(path->name, "LevelPath") == 0) {
                path->flags |= 1;
            }
            for (i32 route_index = 0; route_index < runtime_path->route_count; ++route_index) {
                AIPATHROUTE_s *source = &runtime_path->routes[route_index];
                EDAIPATHROUTE_s *route = &path->routes[route_index];
                route->flags |= 1;
                NuStrCpy(route->name, source->name);
                route->user_mask = source->character_masks[1];
            }

            for (i32 node_index = 0; node_index < runtime_path->node_count; ++node_index) {
                AIPATHNODE_s *source = &runtime_path->nodes[node_index];
                EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&aieditor->free_path_nodes);
                if (node == nullptr || path->node_count >= 254) {
                    break;
                }
                NuLinkedListRemove(&aieditor->free_path_nodes, &node->link);
                NuLinkedListAppend(&path->nodes, &node->link);
                node->index = node_index;
                ++path->node_count;
                if (source->name != nullptr)
                    strcpy(node->name, source->name);
                node->position = source->position;
                node->radius = source->radius;
                node->lower_height = source->min_height - source->position.y;
                node->upper_height = source->max_height - source->position.y;
                node->flags = source->runtime_flags;
                node->special = source->special_handle;
                node->special_position = source->special_position;
                if (source->special_route_index < runtime_path->special_route_count) {
                    AIPATHNODELINK_s *link = &runtime_path->special_routes[source->special_route_index];
                    i32 shared_index = link->special_route_index;
                    if (shared_index >= 0 && shared_index < 64) {
                        EDAISHAREDPATHNODE_s *shared = shared_nodes[shared_index];
                        if (shared == nullptr) {
                            shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->free_shared_nodes);
                            if (shared != nullptr) {
                                NuLinkedListRemove(&aieditor->free_shared_nodes, &shared->link);
                                NuLinkedListAppend(&aieditor->shared_path_nodes, &shared->link);
                                shared_nodes[shared_index] = shared;
                            }
                        }
                        if (shared != nullptr) {
                            if (node->shared_node != shared) {
                                if (node->shared_node != nullptr) {
                                    EDAISHAREDPATHNODE_s *previous = node->shared_node;
                                    --previous->reference_count;
                                    if (previous->reference_count <= 1) {
                                        pathEditor_DestroySharedNode(previous);
                                    }
                                }
                                node->shared_node = shared;
                                ++shared->reference_count;
                            }
                        }
                    }
                }
            }
            for (i32 node_index = 0; node_index < runtime_path->node_count; ++node_index) {
                AIPATHNODE_s *source = &runtime_path->nodes[node_index];
                EDAIPATHNODE_s *node = &aieditor->node_storage[node_storage_start + node_index];
                for (i32 edge_index = 0; edge_index < source->connection_count; ++edge_index) {
                    AIPATHCNX_s *connection = source->connections[edge_index];
                    i32 direction = connection->node_indices[0] != node_index;
                    i32 other_index = connection->node_indices[direction ^ 1];
                    EDAIPATHNODE_s *other = &aieditor->node_storage[node_storage_start + other_index];
                    EDAIPATHCNX_s *editor_connection = nullptr;
                    if (node->connections[0].node == other)
                        editor_connection = &node->connections[0];
                    else if (node->connections[1].node == other)
                        editor_connection = &node->connections[1];
                    else if (node->connections[2].node == other)
                        editor_connection = &node->connections[2];
                    else if (node->connections[3].node == other)
                        editor_connection = &node->connections[3];
                    else if (node->connections[4].node == other)
                        editor_connection = &node->connections[4];
                    else if (node->connections[5].node == other)
                        editor_connection = &node->connections[5];
                    else if (node->connections[6].node == other)
                        editor_connection = &node->connections[6];
                    else if (node->connections[7].node == other)
                        editor_connection = &node->connections[7];
                    if (editor_connection == nullptr) {
                        i32 free_node_slot = -1;
                        i32 free_other_slot = -1;
#define FIND_FREE_OTHER(source_slot)                                                                                   \
    if (other->connections[0].node == nullptr) {                                                                       \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 0;                                                                                           \
    } else if (other->connections[1].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 1;                                                                                           \
    } else if (other->connections[2].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 2;                                                                                           \
    } else if (other->connections[3].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 3;                                                                                           \
    } else if (other->connections[4].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 4;                                                                                           \
    } else if (other->connections[5].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 5;                                                                                           \
    } else if (other->connections[6].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 6;                                                                                           \
    } else if (other->connections[7].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 7;                                                                                           \
    }
                        if (node->connections[0].node == nullptr) {
                            FIND_FREE_OTHER(0)
                        } else if (node->connections[1].node == nullptr) {
                            FIND_FREE_OTHER(1)
                        } else if (node->connections[2].node == nullptr) {
                            FIND_FREE_OTHER(2)
                        } else if (node->connections[3].node == nullptr) {
                            FIND_FREE_OTHER(3)
                        } else if (node->connections[4].node == nullptr) {
                            FIND_FREE_OTHER(4)
                        } else if (node->connections[5].node == nullptr) {
                            FIND_FREE_OTHER(5)
                        } else if (node->connections[6].node == nullptr) {
                            FIND_FREE_OTHER(6)
                        } else if (node->connections[7].node == nullptr) {
                            FIND_FREE_OTHER(7)
                        }
#undef FIND_FREE_OTHER
                        if (free_node_slot < 0 || free_other_slot < 0)
                            continue;
                        editor_connection = &node->connections[free_node_slot];
                        EDAIPATHCNX_s *reciprocal = &other->connections[free_other_slot];
                        editor_connection->node = other;
                        editor_connection->flags = 0;
                        reciprocal->node = node;
                        reciprocal->flags = 0;
                    }
                    editor_connection->route_mask |= connection->route_mask;
                }
                node->route_mask = source->route_boundary_mask;
            }
            node_storage_start += runtime_path->node_count;
        }
    }
    if (runtime_system == nullptr) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->free_paths);
        if (path != nullptr) {
            NuLinkedListRemove(&aieditor->free_paths, &path->link);
            NuLinkedListAppend(&aieditor->paths, &path->link);
            strcpy(path->name, "LevelPath");
            path->flags |= 1;
        }
    }
    if (aieditorsettings.current_path_name[0] != '\0') {
        aieditor->current_path = pathEditor_GetPath(aieditorsettings.current_path_name);
    } else {
        aieditor->current_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    }
    pathEditor_CalcNodeIXs();
    EDAIPATH_s *path = aieditor->current_path;
    if (path != nullptr) {
        path->current_route = &path->routes[0];
        if (path->routes[0].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[1];
        if (path->routes[1].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[2];
        if (path->routes[2].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[3];
        if (path->routes[3].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[4];
        if (path->routes[4].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[5];
        if (path->routes[5].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[6];
        if (path->routes[6].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[7];
        if (path->routes[7].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[8];
        if (path->routes[8].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[9];
        if (path->routes[9].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[10];
        if (path->routes[10].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[11];
        if (path->routes[11].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[12];
        if (path->routes[12].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[13];
        if (path->routes[13].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[14];
        if (path->routes[14].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[15];
        if ((path->current_route->flags & 1) == 0)
            path->current_route = nullptr;
    route_selected:;
    }
}

void pathEditor_Render(i32 x, i32 y, float x_scale, float y_scale) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path != nullptr) {
        i32 screen_x = (x + 10) * 16;
        i32 screen_y = y * 8;
        if (aieditorsettings.unknown_060_bit0) {
            NuQFntPrintEx(system_qfont, screen_x, screen_y - 40, 16, "Show Routes : \"%s\"", path->name);
            NuQFntSetColour(system_qfont, 0x80000000);
            NuQFntSetScale(system_qfont, x_scale, y_scale);
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "%s", path->name);
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 240, 16, "SQR - Sub menu");
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 360, 16, "SELECT - Goto nearest");
            if (path->runtime_nearest >= 0) {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "TRI - Set as start of route");
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 600, 16, "X - Set as end of route");
            }
        } else {
            NuQFntPrintEx(system_qfont, screen_x, screen_y - 40, 16, "AI Path Editor: \"%s\"", path->name);
            NuQFntSetColour(system_qfont, 0x80000000);
            NuQFntSetScale(system_qfont, x_scale, y_scale);
            if (path->current_node != nullptr) {
                if (path->current_node->name[0] != '\0') {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "\"%s\" %d nodes",
                                  path->current_node->name, path->node_count);
                } else {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "ix=%d, %d nodes",
                                  path->current_node->index, path->node_count);
                }
            } else {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "%d nodes", path->node_count);
            }
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 240, 16, "SQR - Sub menu");
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 360, 16, "SELECT - Select nearest");
            if (aieditor->flags & 1) {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "X - Move selected");
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 600, 16, "TRI - Delete selected");
                if (path->current_node != nullptr) {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 840, 16, "LRIGHT - Increase radius, %.2f",
                                  path->current_node->radius);
                } else {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 840, 16, "LRIGHT - Increase radius");
                }
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 960, 16, "LLEFT - Decrease radius");
            } else if (path->nearest_node != nullptr) {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "X - Select");
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 720, 16, "O - Link/unlink to selected");
            } else {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "X - Create");
            }
        }
    }
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    antinodeEditorDrawAntinodes();
}

static EDAIPATHNODE_s *pathEditor_GetNearestNode(EDAIPATH_s *path, i32 require_radius) {
    if (path == nullptr) {
        return nullptr;
    }
    EDAIPATHNODE_s *node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes));
    if (node == nullptr) {
        return nullptr;
    }
    EDAIPATHNODE_s *nearest = nullptr;
    f32 nearest_distance = FLT_MAX;
    if (require_radius) {
        for (; node != nullptr;
             node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetNext(&path->nodes, &node->link))) {
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
            if (distance >= nearest_distance) {
                continue;
            }
            f32 height = aieditor->cursor_position.y - node->position.y;
            f32 upper = NuFmax(0.2f, node->height_max);
            f32 lower = NuFmin(-0.2f, node->height_min);
            if (height <= upper && height >= lower && distance < node->radius * node->radius) {
                nearest = node;
                nearest_distance = distance;
            }
        }
    } else {
        for (; node != nullptr;
             node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetNext(&path->nodes, &node->link))) {
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
            if (nearest_distance > distance) {
                f32 height = aieditor->cursor_position.y - node->position.y;
                f32 upper = NuFmax(0.2f, node->height_max);
                f32 lower = NuFmin(-0.2f, node->height_min);
                if (height <= upper && height >= lower) {
                    nearest = node;
                    nearest_distance = distance;
                }
            }
        }
    }
    return nearest;
}

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) __asm__(
    "_ZL22routeEditor_AddToRouteP14EDAIPATHNODE_sS0_.part.7");

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_route == nullptr)
        return 0;
    i32 index = path->current_route - path->routes;
    if (index > 15)
        return 0;
    u16 mask = 1u << index;
    for (i32 slot = 0; slot < 8; ++slot) {
        EDAIPATHCNX_s *connection = &node->connections[slot];
        if (connection->node != other)
            continue;
        if (!(connection->route_mask & mask)) {
            connection->route_mask |= mask;
            return 1;
        }
        connection->route_mask &= ~mask;
        if (other->connections[slot].node != nullptr && (other->connections[slot].route_mask & mask))
            other->route_mask &= ~mask;
        return -1;
    }
    return 0;
}

eduimenu_s *routeEditor_Process(nupad_s *pad) {
    if (pad->digital_buttons_pressed & 0x80) {
        eduimenu_s *menu =
            eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
        if (menu == nullptr)
            return nullptr;
        eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ROUTES, attr, 0, 0, aieditor_cvSelectEditorMode,
                                                const_cast<char *>("Select Editor Mode")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbSave, const_cast<char *>("Save AI Data")));
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbCreateRoute, const_cast<char *>("Create New Route")));
        EDAIPATH_s *selected_path = aieditor->current_path;
        if (selected_path != nullptr && selected_path->current_route != nullptr) {
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbRenameRouteMenu,
                                                    const_cast<char *>("Rename Current Route")));
            eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, routeEditor_cbDeleteRoute,
                                                    const_cast<char *>("Destroy Current Route")));
            eduiMenuAddItem(
                menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbRouteUsers, const_cast<char *>("Route Users")));
        }
        return menu;
    }
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr)
        return nullptr;
    // Both route cycling directions are expanded across the 16 fixed slots in the original.
    if (pad->digital_buttons_pressed & 0x1000) {
        i32 index = path->current_route == nullptr ? 1 : path->current_route - path->routes + 1;
#define ROUTE_EDITOR_TRY_FORWARD()                                                                                     \
    if (index >= 16)                                                                                                   \
        index = 0;                                                                                                     \
    path->current_route = &path->routes[index];                                                                        \
    if (path->current_route->flags & 1)                                                                                \
        goto route_selected;                                                                                           \
    ++index
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
        ROUTE_EDITOR_TRY_FORWARD();
#undef ROUTE_EDITOR_TRY_FORWARD
        path->current_route = nullptr;
        return nullptr;
    } else if (pad->digital_buttons_pressed & 0x4000) {
        i32 index = path->current_route == nullptr ? 15 : path->current_route - path->routes - 1;
#define ROUTE_EDITOR_TRY_BACKWARD()                                                                                    \
    if (index < 0)                                                                                                     \
        index = 15;                                                                                                    \
    path->current_route = &path->routes[index];                                                                        \
    if (path->current_route->flags & 1)                                                                                \
        goto route_selected;                                                                                           \
    --index
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
        ROUTE_EDITOR_TRY_BACKWARD();
#undef ROUTE_EDITOR_TRY_BACKWARD
        path->current_route = nullptr;
        return nullptr;
    } else if (path->current_route == nullptr) {
        return nullptr;
    }
route_selected:
    if ((pad->digital_buttons & 0x40) && (pad->digital_buttons_pressed & 0x40) && path->nearest_node != nullptr) {
        path->current_node = path->nearest_node;
        nuvec_s position = path->current_node->position;
        position.y = aieditor->cursor_position.y;
        edcamSetPos(&position);
    }
    if ((pad->digital_buttons_pressed & 0x20) && path->current_node != nullptr && path->nearest_node != nullptr &&
        path->current_node != path->nearest_node) {
        EDAIPATHNODE_s *nearest = path->nearest_node;
        i32 change = routeEditor_AddToRoute(path->current_node, nearest);
        routeEditor_AddToRoute(nearest, path->current_node);
        if (change == 1 || change == -1) {
            path->current_node = nearest;
            nuvec_s position = nearest->position;
            position.y = aieditor->cursor_position.y;
            edcamSetPos(&position);
        }
    }
    if ((pad->digital_buttons_pressed & 0x10) && path->current_node != nullptr) {
        i32 index = path->current_route - path->routes;
        u16 mask = 1u << index;
        for (i32 slot = 0; slot < 8; ++slot) {
            EDAIPATHCNX_s *connection = &path->current_node->connections[slot];
            if (connection->node != nullptr && (connection->route_mask & mask)) {
                path->current_node->route_mask ^= mask;
                break;
            }
        }
    }
    if (pad->digital_buttons_pressed & 0x100) {
        path->current_node = pathEditor_GetNearestNode(path, 0);
        if (path->current_node != nullptr)
            edcamSetPos(&path->current_node->position);
    }
    path->nearest_node = pathEditor_GetNearestNode(path, 1);
    aieditor->flags &= ~u8(1);
    if (path->current_node != nullptr && path->current_node == path->nearest_node)
        aieditor->flags |= 1;
    return nullptr;
}

static void pathEditor_PathNodeMoved(EDAIPATHNODE_s *node) {
    if (!(node->flags & 0x80) && NuSpecialExistsFn(&aieditor->cursor_platform)) {
        node->platform = aieditor->cursor_platform;
        NuVecInvMtxTransform(&node->platform_position, &node->position,
                             NuSpecialGetDrawMtx(&aieditor->cursor_platform));
    } else {
        node->platform.scene = NULL;
        node->platform.special = NULL;
        node->platform.display_special = NULL;
    }
    creatureEditor_PathNodeMoved(node);
    locatorEditor_PathNodeMoved(node);
    if (AIPathNodeDeletedFn != NULL)
        AIPathNodeDeletedFn(aieditor->current_path->current_node);
    if (node->shared_node != NULL) {
        for (EDAIPATH_s *path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetHead(&aieditor->paths)); path != NULL;
             path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetNext(&aieditor->paths, &path->link))) {
            for (EDAIPATHNODE_s *other = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes));
                 other != NULL;
                 other = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetNext(&path->nodes, &other->link))) {
                if (other != node && other->shared_node == node->shared_node) {
                    other->radius = node->radius;
                    other->position.x = node->position.x;
                    other->height_min = node->height_min;
                    other->position.y = node->position.y;
                    other->height_max = node->height_max;
                    other->position.z = node->position.z;
                    other->platform = node->platform;
                    other->platform_position = node->platform_position;
                    creatureEditor_PathNodeMoved(other);
                    locatorEditor_PathNodeMoved(other);
                    break;
                }
            }
        }
    }
}

static __used__ void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *shared) {
    if (shared != NULL) {
        for (EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); path != NULL;
             path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link)) {
            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&aieditor->current_path->nodes);
                 node != NULL;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&aieditor->current_path->nodes, &node->link)) {
                if (node->shared_node == shared) {
                    node->shared_node = NULL;
                    break;
                }
            }
        }
        NuLinkedListRemove(&aieditor->shared_path_nodes, &shared->link);
        NuLinkedListAppend(&aieditor->free_shared_path_nodes, &shared->link);
    }
}

static void DestroyAIPathNode(EDAIPATHNODE_s *node, EDAIPATH_s *path) {
    if (node == NULL || path == NULL)
        return;
    if (node->shared_node != NULL) {
        --node->shared_node->reference_count;
        if (node->shared_node->reference_count <= 1)
            pathEditor_DestroySharedNode(node->shared_node);
        node->shared_node = NULL;
    }
    for (i32 connection = 0; connection < 8; ++connection) {
        EDAIPATHNODE_s *other = node->connections[connection].node;
        if (other != NULL) {
            pathEditor_DisconnectNodes(node, other);
        }
    }
    NuLinkedListRemove(&path->nodes, &node->link);
    --path->node_count;
    memset(node, 0, sizeof(*node));
    NuLinkedListAppend(&aieditor->free_path_nodes, &node->link);
}

eduimenu_s *pathEditor_Process(nupad_s *pad) {
    if (pad->digital_buttons_pressed & 0x80) {
        eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, "Options");
        if (menu == NULL)
            return NULL;
        eduiMenuAddItem(
            menu, eduiItemSelCreate(AIEDITOR_ROUTES, attr, 0, 0, aieditor_cvSelectEditorMode, "Select Editor Mode"));
        if (aieditor->current_path && aieditor->current_path->current_node)
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbRenameNodeMenu, "Rename Node"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbSelectPathMenu, "Select Path Network"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbSave, "Save AI Data"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbGoToPlayer, "Go To Player"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbMovePlayer, "Move Player"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbCreatePath, "Create Path Network"));
        if (aieditor->current_path) {
            if (aieditor->current_path->current_node)
                eduiMenuAddItem(menu,
                                eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbShareNodeMenu, "Share node with..."));
            if (aieditor->current_path && !(aieditor->current_path->flags & 1))
                eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, pathEditor_cbDeletePath, "Delete Path Network"));
            if (aieditor->current_path && !(aieditor->current_path->flags & 1))
                eduiMenuAddItem(menu,
                                eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbRenamePathMenu, "Rename Path Network"));
        }
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, static_cast<i8>(aieditorsettings.path_flags) >> 7, 1,
                                                   aieditor_cbSolidPathDisplayToggle, "Solid Path Display"));
        i32 row = 2;
        if (aieditor->current_path) {
            EDAIPATHNODE_s *node = aieditor->current_path->current_node;
            EDAIPATHNODE_s *nearest = aieditor->current_path->nearest_node;
            EDAIPATHCNX_s *connection = NULL;
            if (node && nearest) {
                for (i32 i = 0; i < 8; ++i)
                    if (node->connections[i].node == nearest) {
                        connection = &node->connections[i];
                        break;
                    }
            }
            if (connection && naipathcnxtypes != 0) {
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, NULL, "================="));
                for (i32 i = 0; i < naipathcnxtypes; ++i) {
                    eduiMenuAddItem(menu, eduiItemToggleCreate(
                                              i, attr, (connection->flags & aipathcnxtypes[i].connection_flag) != 0,
                                              i + 2, pathEditor_cbCnxFlagsToggle, aipathcnxtypes[i].name));
                    row = i + 3;
                }
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, NULL, "================="));
            }
            node = aieditor->current_path->current_node;
            if (node) {
                eduiMenuAddItem(menu, eduiItemToggleCreate(0x80, attr, (node->flags >> 7) & 1, row,
                                                           pathEditor_cbNodeFlagsToggle, "Never On A Platform"));
                ++row;
            }
        }
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 6) & 1), row,
                                                   aieditor_cbStopPlatformsToggle, "Stop Platforms"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags2 >> 1) & 1), row + 1,
                                                   pathEditor_cbDrawWallsplinesToggle, "Draw Wallsplines"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 1) & 1), row + 2,
                                                   aieditor_cbDrawAllToggle, "Draw All Paths"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 4) & 1), row + 3,
                                                   aieditor_cbSnapHeightToggle, "Snap Height"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 3) & 1), row + 4,
                                                   aieditor_cbShowCreaturesToggle, "Show Creatures"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, near_clip_at_cursor, row + 5, cbNearClipAtCursor,
                                                   "Near Clip At Cursor"));
        return menu;
    }
    EDAIPATH_s *path = aieditor->current_path;
    if (!path)
        return NULL;
    EDAIPATHNODE_s *previous = path->current_node;
    if (!(path->flags & 1))
        aieditorsettings.path_flags &= ~1;
    if (aieditorsettings.path_flags & 1) {
        AIPATH_s *runtime = aieditor->runtime_path;
        i32 nearest = -1;
        f32 best = 3.402823466e38f;
        if (runtime && runtime->node_count) {
            for (i32 i = 0; i < runtime->node_count; ++i) {
                NUVEC delta;
                AIPATHNODE_s *node = &runtime->nodes[i];
                f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
                if (distance < best && distance < node->radius_squared) {
                    best = distance;
                    nearest = i;
                }
            }
        }
        path->runtime_nearest = static_cast<i16>(nearest);
        if (pad->digital_buttons_pressed & 0x40) {
            path = aieditor->current_path;
            if (path->runtime_nearest >= 0)
                path->runtime_start = path->runtime_nearest;
        }
        if (pad->digital_buttons_pressed & 0x10) {
            path = aieditor->current_path;
            if (path->runtime_nearest >= 0)
                path->runtime_end = path->runtime_nearest;
        }
        if (pad->digital_buttons_pressed & 0x100) {
            path = aieditor->current_path;
            if (path->runtime_nearest >= 0) {
                path->runtime_start = path->runtime_nearest;
                edcamSetPos(&aieditor->runtime_path->nodes[path->runtime_nearest].position);
                path = aieditor->current_path;
            }
            EDAIPATHNODE_s *node = pathEditor_GetNearestNode(path, 0);
            path->current_node = node;
            if (aieditor->current_path->current_node)
                edcamSetPos(&aieditor->current_path->current_node->position);
        }
        return NULL;
    }
    if (pad->digital_buttons & 0x40) {
        EDAIPATHNODE_s *nearest = path->nearest_node;
        if (!nearest) {
            if (pad->digital_buttons_pressed & 0x40) {
                EDAIPATHNODE_s *node = NULL;
                if (path->node_count <= 253) {
                    node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&aieditor->free_path_nodes));
                    if (node) {
                        NuLinkedListRemove(&aieditor->free_path_nodes, &node->link);
                        NuLinkedListAppend(&path->nodes, &node->link);
                        ++path->node_count;
                        node->position = aieditor->camera_position;
                        node->radius = default_path_node_radius;
                        node->height_max = default_path_heighttol;
                        node->height_min = -default_path_heighttol;
                    }
                }
                path->current_node = node;
                if (previous) {
                    path = aieditor->current_path;
                    node = path->current_node;
                    if (node) {
                        node->radius = previous->radius;
                        node->height_max = previous->height_max;
                        node->height_min = previous->height_min;
                        bool connected = false;
                        for (i32 i = 0; i < 8 && !connected; ++i)
                            if (!node->connections[i].node) {
                                for (i32 j = 0; j < 8; ++j)
                                    if (!previous->connections[j].node) {
                                        node->connections[i].node = previous;
                                        node->connections[i].flags = 0;
                                        previous->connections[j].node = node;
                                        previous->connections[j].flags = 0;
                                        connected = true;
                                        break;
                                    }
                            }
                        if (!connected) {
                            DestroyAIPathNode(node, path);
                            aieditor->current_path->current_node = previous;
                        }
                    } else
                        aieditor->current_path->current_node = previous;
                }
            }
        } else if (pad->digital_buttons_pressed & 0x40) {
            path->current_node = nearest;
            NUVEC position = {nearest->position.x, aieditor->cursor_position.y, nearest->position.z};
            edcamSetPos(&position);
        } else if (path->current_node && path->current_node == nearest) {
            f32 old_x = nearest->position.x, old_z = nearest->position.z;
            nearest->position = aieditor->camera_position;
            pathEditor_PathNodeMoved(nearest);
            locatorEditor_PathNodeMoved(aieditor->current_path->current_node);
            if (AIPathNodeDeletedFn)
                AIPathNodeDeletedFn(aieditor->current_path->current_node);
            if (pad->digital_buttons & 0x200) {
                path = aieditor->current_path;
                f32 dx = path->current_node->position.x - old_x, dz = path->current_node->position.z - old_z;
                for (EDAIPATHNODE_s *node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes)); node;
                     node = reinterpret_cast<EDAIPATHNODE_s *>(
                         NuLinkedListGetNext(&aieditor->current_path->nodes, &node->link))) {
                    if (node != aieditor->current_path->current_node) {
                        node->position.x += dx;
                        node->position.z += dz;
                        locatorEditor_PathNodeMoved(node);
                        if (AIPathNodeDeletedFn)
                            AIPathNodeDeletedFn(node);
                    }
                }
            }
        }
    }
    if (pad->digital_buttons_pressed & 0x20) {
        path = aieditor->current_path;
        EDAIPATHNODE_s *node = path->current_node, *nearest = path->nearest_node;
        if (node && nearest && node != nearest) {
            bool connected = false;
            for (i32 i = 0; i < 8; ++i)
                if (node->connections[i].node == nearest) {
                    connected = true;
                    break;
                }
            if (connected) {
                eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, pathEditor_cbCancelDisconnectNodeMenu,
                                                  "Disconnect current path node??");
                if (menu) {
                    eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, pathEditor_cbDisconnectPathNode, "No"));
                    eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbDisconnectPathNode, "Yes"));
                }
                return menu;
            }
            for (i32 i = 0; i < 8 && !connected; ++i)
                if (!node->connections[i].node) {
                    for (i32 j = 0; j < 8; ++j)
                        if (!nearest->connections[j].node) {
                            node->connections[i].node = nearest;
                            node->connections[i].flags = 0;
                            nearest->connections[j].node = node;
                            nearest->connections[j].flags = 0;
                            connected = true;
                            break;
                        }
                }
            NUVEC position = {nearest->position.x, aieditor->cursor_position.y, nearest->position.z};
            edcamSetPos(&position);
        }
    }
    if (pad->digital_buttons_pressed & 0x10) {
        path = aieditor->current_path;
        if (path->current_node && path->current_node == path->nearest_node) {
            eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, pathEditor_cbCancelDeleteNodeMenu,
                                              "Delete current path node??");
            if (menu) {
                eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, pathEditor_cbDeletePathNode, "No"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbDeletePathNode, "Yes"));
            }
            return menu;
        }
    }
    if (pad->digital_buttons_pressed & 0x100) {
        if (aieditorsettings.path_flags & 2) {
            EDAIPATH_s *nearest_path = NULL;
            EDAIPATHNODE_s *nearest_node = NULL;
            f32 best = 3.402823466e38f;
            for (path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetHead(&aieditor->paths)); path;
                 path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetNext(&aieditor->paths, &path->link))) {
                EDAIPATHNODE_s *node = pathEditor_GetNearestNode(path, 0);
                if (node) {
                    NUVEC delta;
                    f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
                    if (distance < best) {
                        nearest_path = path;
                        nearest_node = node;
                        best = distance;
                    }
                }
            }
            if (nearest_path && nearest_node) {
                aieditor->current_path = nearest_path;
                nearest_path->current_node = nearest_node;
                edcamSetPos(&nearest_node->position);
            }
        } else {
            path = aieditor->current_path;
            path->current_node = pathEditor_GetNearestNode(path, 0);
            EDAIPATHNODE_s *node = aieditor->current_path->current_node;
            if (node) {
                if (edpath_addoffset.x != 0 || edpath_addoffset.y != 0 || edpath_addoffset.z != 0) {
                    NuVecAdd(&node->position, &node->position, &edpath_addoffset);
                    pathEditor_PathNodeMoved(aieditor->current_path->current_node);
                    locatorEditor_PathNodeMoved(aieditor->current_path->current_node);
                }
                edcamSetPos(&aieditor->current_path->current_node->position);
            }
        }
    }
    aieditor->flags &= ~1;
    path = aieditor->current_path;
    EDAIPATHNODE_s *node = path->current_node;
    if (node && node == path->nearest_node) {
        if (pad->digital_buttons & (0x8000 | 0x2000)) {
            node->radius *= (pad->digital_buttons & 0x8000) ? .99f : 1.01f;
            if (node->radius < .01f)
                node->radius = .01f;
            pathEditor_PathNodeMoved(node);
            locatorEditor_PathNodeMoved(aieditor->current_path->current_node);
            if (AIPathNodeDeletedFn)
                AIPathNodeDeletedFn(aieditor->current_path->current_node);
            path = aieditor->current_path;
        }
        if (aieditorsettings.path_flags & 0x80) {
            if (pad->digital_buttons & 0x1000) {
                aieditor->flags |= 4;
                node = path->current_node;
                f32 step = node->height_max * .05f;
                if (pad->digital_buttons & 4)
                    node->height_max += step;
                else if (pad->digital_buttons & 1)
                    node->height_max -= step;
                if (node->height_max < .01f)
                    node->height_max = .01f;
            } else if (pad->digital_buttons & 0x4000) {
                aieditor->flags |= 4;
                node = path->current_node;
                f32 step = -node->height_min * .05f;
                if (pad->digital_buttons & 4)
                    node->height_min += step;
                else if (pad->digital_buttons & 1)
                    node->height_min -= step;
                if (node->height_min > -.01f)
                    node->height_min = -.01f;
            }
        }
        aieditor->flags |= 1;
    }
    i32 direction = 0;
    bool named_only = false;
    if (pad->digital_buttons & 0x100) {
        if (pad->digital_buttons_pressed & 8)
            direction = 1;
        else if (pad->digital_buttons_pressed & 2)
            direction = -1;
        else if (pad->digital_buttons_pressed & 4) {
            direction = 1;
            named_only = true;
        } else if (pad->digital_buttons_pressed & 1) {
            direction = -1;
            named_only = true;
        }
    }
    if (!direction && !(aieditorsettings.path_flags & 0x80)) {
        if (pad->digital_buttons_pressed & 0x1000)
            direction = 1;
        else if (pad->digital_buttons_pressed & 0x4000)
            direction = -1;
    }
    if (direction) {
        path = aieditor->current_path;
        if (path->current_node)
            path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(
                direction > 0 ? NuLinkedListGetNext(&path->nodes, &path->current_node->link)
                              : NuLinkedListGetPrev(&path->nodes, &path->current_node->link));
        path = aieditor->current_path;
        if (!path->current_node)
            path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(direction > 0 ? NuLinkedListGetHead(&path->nodes)
                                                                                  : NuLinkedListGetTail(&path->nodes));
        path = aieditor->current_path;
        if (named_only && path->current_node && NuStrLen(path->current_node->name) == 0) {
            EDAIPATHNODE_s *start = path->current_node;
            do {
                path = aieditor->current_path;
                path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(
                    direction > 0 ? NuLinkedListGetNext(&path->nodes, &path->current_node->link)
                                  : NuLinkedListGetPrev(&path->nodes, &path->current_node->link));
                path = aieditor->current_path;
                if (!path->current_node)
                    path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(
                        direction > 0 ? NuLinkedListGetHead(&path->nodes) : NuLinkedListGetTail(&path->nodes));
                path = aieditor->current_path;
                if (!path->current_node || path->current_node == start)
                    break;
            } while (NuStrLen(path->current_node->name) == 0);
        }
        if (path->current_node)
            edcamSetPos(&path->current_node->position);
    }
    path = aieditor->current_path;
    path->nearest_node = pathEditor_GetNearestNode(path, 1);
    return NULL;
}
