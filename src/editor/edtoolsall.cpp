#include "decomp.h"
#include "editor/edpath.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

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
}

static eduiiattr_s editor_mode_attr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};

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

void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *, u32, u32, i32, i32) {
    STUBBED();
    return 0;
}

void antinodeEditor_Enter() {
    STUBBED();
}

void creatureEditor_Enter() {
    STUBBED();
}

void antinodeEditor_Render(i32, i32, float, float) {
    STUBBED();
}

void creatureEditor_Render(i32, i32, float, float) {
    STUBBED();
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
