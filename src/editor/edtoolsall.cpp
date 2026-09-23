#include "decomp.h"
#include "editor/edpath.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include <stdio.h>
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
struct EDCREATURE_s;
i32 creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32);
i32 creatureEditor_IsSelectable(EDCREATURE_s *);
i32 CanWearHatsInFreePlay(i32);

extern "C" {
    extern void *ed_fnt;
    extern i32 AIEDITOR_ANTINODES;
    i32 aisys_maxnumcreaturesets = 32;
    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
    void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *, u32);
    void pathEditorDrawPaths();
    void creatureEditor_RenderAllCreatures();
    void areaEditorDrawAreas();
    void locatorEditorDrawLocators();
    void antinodeEditorDrawAntinodes();
    void AiRndrLine3d(NURND_VERTEX3D *, numtl_s *, NUMTX *);
}

static eduiiattr_s editor_mode_attr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};

struct EDANTINODE_s {
    NULISTLNK link;
    nuvec_s position;
    f32 radius;
    f32 lower_height;
    f32 upper_height;
    nuhspecial_s special;
    nuvec_s special_position;
    i32 flags;
    i32 rotation_offset;
    f32 base_radius;
    f32 base_height;
    u8 game_flags;
    u8 type;
    u8 unknown_4a[2];
};
DECOMP_ASSERT(sizeof(EDANTINODE_s) == 0x4c, "editor antinode stride");

static inline EDANTINODE_s *antinode_pool() {
    return reinterpret_cast<EDANTINODE_s *>(reinterpret_cast<u8 *>(aieditor) + 0x4088c);
}
static inline NULISTHDR *antinode_free_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e8c);
}
static inline NULISTHDR *antinode_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
}

extern "C" f32 default_path_heighttol;

#if defined(__i386__)
#define EDANTINODE_REGPARM1 __attribute__((regparm(1)))
#else
#define EDANTINODE_REGPARM1
#endif

static __used__ __attribute__((noinline, force_align_arg_pointer)) EDANTINODE_REGPARM1 EDANTINODE_s *
CreateAntinode(nuvec_s *position) {
    EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(antinode_free_list()));
    if (node == nullptr)
        return nullptr;
    NuLinkedListRemove(antinode_free_list(), &node->link);
    NuLinkedListAppend(antinode_list(), &node->link);
    node->position = *position;
    EDANTINODE_s *selected = reinterpret_cast<EDANTINODE_s *>(aieditor->mode_selection_42e9c);
    if (selected != nullptr) {
        node->radius = selected->radius;
        node->lower_height = selected->lower_height;
        node->upper_height = selected->upper_height;
        node->type = selected->type;
        node->base_radius = selected->base_radius;
        node->base_height = selected->base_height;
    } else {
        node->radius = 0.25f;
        node->lower_height = -default_path_heighttol;
        node->upper_height = default_path_heighttol;
    }
    return node;
}

static eduimenu_s *editorModeOptions(i32 mode, i32 height) {
    eduimenu_s *menu =
        eduiMenuCreate(200, 70, 240, height, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
    if (menu == nullptr)
        return nullptr;
    eduiMenuAddItem(menu, eduiItemSelCreate(mode, &editor_mode_attr, 0, 0, aieditor_cvSelectEditorMode,
                                            const_cast<char *>("Select Editor Mode")));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(1, &editor_mode_attr, 0, 0, aieditor_cbSave, const_cast<char *>("Save AI Data")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(1, &editor_mode_attr, 0, 0, aieditor_cbGoToPlayer, const_cast<char *>("Go To Player")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(1, &editor_mode_attr, 0, 0, aieditor_cbMovePlayer, const_cast<char *>("Move Player")));
    return menu;
}

void routeEditor_Render(i32 x, i32 y, float xscale, float yscale) {
    if (aieditor->current_path != NULL) {
        NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 - 40, 16, "Edit Routes (Path = \"%s\")",
                      aieditor->current_path->name);
        NuQFntSetColour(system_qfont, 0x80000000);
        NuQFntSetScale(system_qfont, xscale, yscale);
        if (aieditor->current_path->current_route != NULL) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "\"%s\"",
                          aieditor->current_path->current_route->name);
        } else {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "NO ROUTES AVAILABLE");
        }
        if (aieditor->current_path->current_node != NULL) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 240, 16, "SQR - Sub menu");
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 360, 16, "SELECT - Select nearest");
            if (aieditor->current_path->nearest_node != NULL &&
                aieditor->current_path->nearest_node != aieditor->current_path->current_node) {
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "X - Select");
            }
            if (!(aieditor->flags & 1) && aieditor->current_path->nearest_node != NULL) {
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 720, 16, "O - Add/remove cnx to route.");
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

template <bool RequireHats, bool RejectFlag40, bool RejectFlag80>
static __attribute__((always_inline)) inline i32 FindModelListDataFlags(APICHARACTERMODELLIST_s *models,
                                                                        u32 model_flags, u32 game_flags, i32 first_id) {
    for (i32 id = first_id; id != -1; ++models, id = models->model_id) {
        if (Collection_Got(id) == 0)
            continue;
        const GAMECHARACTERDATA &game_data = GCDataList[id];
        if ((game_data.flags_090 & game_flags) != game_flags ||
            (CDataList[id].model_flags & model_flags) != model_flags)
            continue;
        if (RejectFlag40 && (game_data.flags_094[1] & 0x40) != 0)
            continue;
        if (RejectFlag80 && static_cast<i8>(game_data.flags_094[1]) < 0)
            continue;
        if (RequireHats && CanWearHatsInFreePlay(id) == 0)
            continue;
        return 1;
    }
    return 0;
}

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *models, u32 model_flags, u32 game_flags, i32 require_hats,
                         i32 reject_flag_40) {
    if (models == nullptr)
        return 0;
    const i32 first_id = models->model_id;
    if (first_id == -1)
        return 0;
    const u32 reject_flag_80 = model_flags & 8;
    if (require_hats != 0) {
        if (reject_flag_80 != 0) {
            if (reject_flag_40 != 0)
                return FindModelListDataFlags<true, true, true>(models, model_flags, game_flags, first_id);
            return FindModelListDataFlags<true, false, true>(models, model_flags, game_flags, first_id);
        }
        if (reject_flag_40 != 0)
            return FindModelListDataFlags<true, true, false>(models, model_flags, game_flags, first_id);
        return FindModelListDataFlags<true, false, false>(models, model_flags, game_flags, first_id);
    }
    if (reject_flag_80 != 0) {
        if (reject_flag_40 != 0)
            return FindModelListDataFlags<false, true, true>(models, model_flags, game_flags, first_id);
        return FindModelListDataFlags<false, false, true>(models, model_flags, game_flags, first_id);
    }
    if (reject_flag_40 != 0)
        return FindModelListDataFlags<false, true, false>(models, model_flags, game_flags, first_id);
    return FindModelListDataFlags<false, false, false>(models, model_flags, game_flags, first_id);
}

void antinodeEditor_Enter() {
    antinode_list()->head = nullptr;
    antinode_list()->tail = nullptr;
    for (i32 i = 0; i < 128; ++i) {
        NuLinkedListAppend(antinode_free_list(), &antinode_pool()[i].link);
    }
    AISYS_s *system = aieditor->ai_system;
    for (i32 i = 0; i < system->antinode_count; ++i) {
        AIANTINODE *source = &system->antinodes[i];
        EDANTINODE_s *node = CreateAntinode(&source->position);
        if (node == nullptr)
            continue;
        node->position = source->position;
        node->radius = source->radius;
        node->lower_height = source->min_y - source->position.y;
        node->upper_height = source->max_y - source->position.y;
        node->game_flags = source->game_flags;
        node->special = source->special_handle;
        node->special_position = source->special_position;
        node->flags = source->rotation_offset;
        node->rotation_offset = source->flags;
        node->base_radius = source->base_radius;
        node->base_height = source->base_height;
        node->type = source->type;
    }
}

void antinodeEditor_Render(i32 x, i32 y, float xscale, float yscale) {
    i32 text_x = (x + 10) * 16;
    i32 text_y = y * 8;
    NuQFntPrintEx(system_qfont, text_x, text_y - 40, 16, "Antinode Editor");
    NuQFntSetColour(system_qfont, 0x80000000);
    NuQFntSetScale(system_qfont, xscale, yscale);
    EDANTINODE_s *selected = reinterpret_cast<EDANTINODE_s *>(aieditor->mode_selection_42e9c);
    EDANTINODE_s *nearest = *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0);
    EDANTINODE_s *display = selected != nullptr ? selected : nearest;
    if (display != nullptr) {
        nuvec_s displacement;
        NuVecXZDist(&display->position, &aieditor->camera_position, &displacement);
    }
    if (selected != nullptr && nearest != nullptr && selected == nearest) {
        char *platform_name = NuSpecialGetName(&selected->special);
        if (platform_name != nullptr) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 120, 16, "Platform=%s", platform_name);
        } else {
            NuQFntPrintEx(system_qfont, text_x, text_y + 120, 16, "Not attached to platform");
        }
        NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16, "X - Move selected/Adjust size");
        NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "TRI - Delete selected");
        if (selected->type == 0) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "LRIGHT - Increase radius, %.2f", selected->radius);
            NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "LLEFT - Decrease radius");
        } else if (aieditor->pad_buttons & 0x40) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "LRIGHT - Increase X, %.2f", selected->base_radius);
            NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "LLEFT - Decrease X");
            NuQFntPrintEx(system_qfont, text_x, text_y + 720, 16, "LUP - Increase Z, %.2f", selected->base_height);
            NuQFntPrintEx(system_qfont, text_x, text_y + 840, 16, "LDOWN - Decrease Z");
        } else if (aieditorsettings.solid_antinode_display && (aieditor->pad_buttons & 0x1000)) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "L1 - Increase upper height");
            NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "R1 - Decrease upper height");
        } else if (aieditorsettings.solid_antinode_display && (aieditor->pad_buttons & 0x4000)) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "L1 - Increase lower height");
            NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "R1 - Decrease lower height");
        } else {
            NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "LLEFT - Rotate left");
            NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "LRIGHT - Rotate right");
            if (aieditorsettings.solid_antinode_display) {
                NuQFntPrintEx(system_qfont, text_x, text_y + 720, 16, "LUP - Adjust upper height");
                NuQFntPrintEx(system_qfont, text_x, text_y + 840, 16, "LDOWN - Adjust lower height");
            }
        }
    } else {
        NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16,
                      nearest != nullptr ? "X - Select antinode" : "X - Create antinode");
        NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "SELECT - Select nearest");
    }
    antinodeEditorDrawAntinodes();
    areaEditorDrawAreas();
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display)
        creatureEditor_RenderAllCreatures();
    locatorEditorDrawLocators();
}

void creatureEditor_Render(i32 x, i32 y, float xscale, float yscale) {
    if (GlobalCharacterRenderFn == nullptr)
        return;
    i32 count = NuLinkedListCheck(&aieditor->creatures);
    i32 text_x = (x + 10) * 16;
    i32 text_y = y * 8;
    NuQFntPrintEx(system_qfont, text_x, text_y - 40, 16, "Creature Editor  (%d placed)", count);
    NuQFntSetColour(system_qfont, 0x80000000);
    NuQFntSetScale(system_qfont, xscale, yscale);

    u8 *selected = reinterpret_cast<u8 *>(aieditor->mode_selection_36930);
    u8 *nearest = *reinterpret_cast<u8 **>(aieditor->unknown_3692c);
    if (selected != nullptr) {
        nuvec_s displacement;
        f32 distance =
            NuVecXZDist(reinterpret_cast<nuvec_s *>(selected + 0x28), &aieditor->camera_position, &displacement);
        if (*reinterpret_cast<u32 *>(selected + 0x68) & 0x20) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 120, 16, "\"%s\", xzrng=%.2f (NotLowEnd)", selected + 8,
                          distance);
        } else {
            NuQFntPrintEx(system_qfont, text_x, text_y + 120, 16, "\"%s\", xzrng=%.2f", selected + 8, distance);
        }
        u8 set = selected[0x5a];
        char set_name[32];
        if (set != 0) {
            sprintf(set_name, "Set=%d", set);
        } else {
            strcpy(set_name, "Set=NONE");
        }
        if (selected[0x18] != 0) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16, "Script = \"%s\", %s", selected + 0x18, set_name);
        } else {
            NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16, "Script = NONE, %s", set_name);
        }
        EDLOCATOR_s *area = *reinterpret_cast<EDLOCATOR_s **>(selected + 0x80);
        EDLOCATOR_s *locator = *reinterpret_cast<EDLOCATOR_s **>(selected + 0x84);
        if (area != nullptr)
            NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "Area = \"%s\"", area->name);
        if (locator != nullptr)
            NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "Locator = \"%s\"", locator->name);
        NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "SQR - Options");
        if (nearest != nullptr && nearest != selected) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 720, 16, "X - Select creature");
        } else if (nearest == selected) {
            NuQFntPrintEx(system_qfont, text_x, text_y + 720, 16, "X - Move selected");
            NuQFntPrintEx(system_qfont, text_x, text_y + 840, 16, "TRI - Delete selected");
        }
    } else {
        NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "SQR - Options");
        NuQFntPrintEx(system_qfont, text_x, text_y + 720, 16,
                      nearest != nullptr ? "X - Select creature" : "X - Create creature");
    }

    for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
         link = NuLinkedListGetNext(&aieditor->creatures, link)) {
        u8 *record = reinterpret_cast<u8 *>(link);
        EDCREATURE_s *creature = reinterpret_cast<EDCREATURE_s *>(record);
        if (!creatureEditor_IsSelectable(creature))
            continue;
        i32 render_colour =
            record == selected ? (record == nearest ? 0xff0000ff : 0x800000ff) : (record == nearest ? -1 : 0);
        i32 group_count = record[0x5b];
        u32 valid_positions = *reinterpret_cast<u32 *>(record + 0x54);
        for (i32 group = 0; group < group_count; ++group) {
            if (group >= 32 || !(valid_positions & (1u << group)))
                continue;
            nuvec_s position;
            creatureEditor_CalculatePos(creature, group, &position, 0);
            i16 angle = *reinterpret_cast<i16 *>(record + 0x34);
            i16 type = *reinterpret_cast<i16 *>(record + 0x58);
            GlobalCharacterRenderFn(&position, angle, type, render_colour, creature);
            EDLOCATOR_s *locator = *reinterpret_cast<EDLOCATOR_s **>(record + 0x84);
            if (locator != nullptr) {
                NURND_VERTEX3D line[2];
                line[0].position = *reinterpret_cast<nuvec_s *>(record + 0x28);
                line[1].position = locator->position;
                line[0].colour = render_colour;
                line[1].colour = render_colour;
                AiRndrLine3d(line, nullptr, nullptr);
            }
            EDLOCATOR_s *respawn = *reinterpret_cast<EDLOCATOR_s **>(record + 0x88);
            if (respawn != nullptr) {
                NURND_VERTEX3D line[2];
                line[0].position = *reinterpret_cast<nuvec_s *>(record + 0x28);
                line[1].position = respawn->position;
                line[0].position.y += 0.1f;
                line[1].position.y += 0.1f;
                line[0].colour = render_colour + 0x8000;
                line[1].colour = render_colour + 0x8000;
                AiRndrLine3d(line, nullptr, nullptr);
            }
        }
    }
    pathEditorDrawPaths();
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    antinodeEditorDrawAntinodes();
}

eduimenu_s *antinodeEditor_Process(nupad_s *pad) {
    if (pad->digital_buttons_pressed & 0x80) {
        eduimenu_s *menu = editorModeOptions(AIEDITOR_ANTINODES, 270);
        if (menu != nullptr) {
            eduiMenuAddItem(menu,
                            eduiItemToggleCreate(1, &editor_mode_attr, -i32(aieditorsettings.solid_antinode_display), 4,
                                                 aieditor_cbSolidAntinodeDisplayToggle,
                                                 const_cast<char *>("Solid Antinode Display")));
            eduiMenuAddItem(menu,
                            eduiItemToggleCreate(1, &editor_mode_attr, -i32(aieditorsettings.stop_platforms), 3,
                                                 aieditor_cbStopPlatformsToggle, const_cast<char *>("Stop Platforms")));
            eduiMenuAddItem(menu,
                            eduiItemToggleCreate(1, &editor_mode_attr, -i32(aieditorsettings.snap_height_display), 2,
                                                 aieditor_cbSnapHeightToggle, const_cast<char *>("Snap Height")));
        }
        return menu;
    }
    void *nearest = *reinterpret_cast<void **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0);
    if ((pad->digital_buttons & 0x40) && (pad->digital_buttons_pressed & 0x40) && nearest != nullptr) {
        aieditor->mode_selection_42e9c = nearest;
        edcamSetPos(reinterpret_cast<nuvec_s *>(reinterpret_cast<u8 *>(aieditor->mode_selection_42e9c) + 8));
    }
    return nullptr;
}

struct CreaturePositionRecord {
    u8 unknown_00[0x28];
    nuvec_s position;
    i32 angle;
    u8 unknown_38[0x3c - 0x38];
    EDAIPATH_s *path;
    u8 unknown_40[0x5c - 0x40];
    u8 across_count;
    u8 unknown_5d[3];
    f32 x_spacing;
    f32 z_spacing;
};

__attribute__((force_align_arg_pointer)) i32 creatureEditor_CalculatePos(EDCREATURE_s *creature, i32 index,
                                                                         nuvec_s *position, i32 check_path) {
    CreaturePositionRecord *record = reinterpret_cast<CreaturePositionRecord *>(creature);
    if (index == 0) {
        *position = record->position;
        return 1;
    }
    i32 across = record->across_count;
    i32 column = index % across;
    i32 row = index / across;
    nuvec_s offset = {((column + 1) / 2) * record->x_spacing * ((column & 1) ? -1.0f : 1.0f), 0.0f,
                      -row * record->z_spacing};
    NuVecRotateY(&offset, &offset, record->angle);
    NuVecAdd(position, &offset, &record->position);
    if (check_path != 0) {
        EDAIPATHCHECK_s result;
        pathEditor_OnPathCheck(position, &result, record->path, 0.0f);
        return result.on_path;
    }
    return 1;
}

i32 creatureEditor_IsSelectable(EDCREATURE_s *creature) {
    if (!aieditorsettings.show_creatures_set || aieditor->mode_selection_36930 == nullptr)
        return 1;
    u8 current_set = *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(aieditor->mode_selection_36930) + 0x5a);
    u8 candidate_set = *(reinterpret_cast<u8 *>(creature) + 0x5a);
    return current_set == candidate_set;
}
