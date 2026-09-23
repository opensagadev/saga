#include "decomp.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nuqfnt.h"
#include <string.h>
#include <stdio.h>

struct nupad_s;

// The editor keeps 64 editable area records ahead of its two lists.
struct EDAIAREA_s {
    NULISTLNK link;
    char name[16];
    NUVEC position;
    NUVEC size;
    i16 rotation;
    u8 unknown_32;
    u8 flags;
    u8 unknown_34[16];
};
DECOMP_ASSERT(sizeof(EDAIAREA_s) == 0x44, "area editor record size");

static inline EDAIAREA_s *&area_selected() {
    return *reinterpret_cast<EDAIAREA_s **>(reinterpret_cast<u8 *>(aieditor) + 0x37a48);
}
static inline EDAIAREA_s *&area_hovered() {
    return *reinterpret_cast<EDAIAREA_s **>(reinterpret_cast<u8 *>(aieditor) + 0x37a4c);
}
static inline NULISTHDR *area_free_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a38);
}
static inline NULISTHDR *area_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40);
}
static inline i32 &area_rotation_step() {
    return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x36934);
}
static inline EDAIAREA_s *area_head() {
    return reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetHead(area_list()));
}
static inline EDAIAREA_s *area_next(EDAIAREA_s *area) {
    return reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetNext(area_list(), &area->link));
}

extern "C" void *ed_fnt;
extern "C" void aieditor_ClearMainMenu();
extern "C" void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
extern "C" void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
extern "C" void SetAiRndrCullDistance(f32);
extern "C" f32 AiRndrCullDistance;
extern "C" i32 aidata_version;
extern "C" i32 AIEDITOR_ROUTES;
void DrawAreaBox(NUVEC *, NUVEC *, i32, i32);
void DrawAreaCylinder(NUVEC *, NUVEC *, i32);
extern "C" void pathEditorDrawPaths();
extern "C" void creatureEditor_RenderAllCreatures();
extern "C" void antinodeEditorDrawAntinodes();
extern "C" void locatorEditorDrawLocators();

static eduiiattr_s area_attr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};

static __used__ void areaEditor_cbDeleteArea(eduimenu_s *menu, eduiitem_s *, unsigned int) {
    if (menu != NULL && menu->field_0c != NULL && area_selected() != NULL && area_selected() == area_hovered()) {
        NULISTHDR *scripts = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x36924);
        for (NULISTLNK *node = NuLinkedListGetHead(scripts); node != NULL; node = NuLinkedListGetNext(scripts, node)) {
            u8 *script = reinterpret_cast<u8 *>(node);
            if (*reinterpret_cast<EDAIAREA_s **>(script + 0x80) == area_selected()) {
                *reinterpret_cast<EDAIAREA_s **>(script + 0x80) = NULL;
            }
            if (*reinterpret_cast<EDAIAREA_s **>(script + 0x6c) == area_selected()) {
                *reinterpret_cast<EDAIAREA_s **>(script + 0x6c) = NULL;
            }
        }
        EDAIAREA_s *selected = area_selected();
        if (selected != NULL) {
            NuLinkedListRemove(area_list(), &selected->link);
            memset(selected, 0, sizeof(*selected));
            NuLinkedListAppend(area_free_list(), &selected->link);
        }
        area_selected() = NULL;
    }
    aieditor_ClearMainMenu();
}

static __used__ void areaEditor_cbRenameArea(eduimenu_s *, eduiitem_s *item, unsigned int) {
    EDAIAREA_s *selected = area_selected();
    const char *name = static_cast<edui_textpicker_s *>(item)->value;
    if (selected == NULL || name[0] == '\0') {
        return;
    }
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        if (NuStrICmp(area->name, name) == 0) {
            return;
        }
    }
    strcpy(area_selected()->name, name);
}

static __used__ void areaEditor_cbCancelRenameMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void areaEditor_cbRenameAreaMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    EDAIAREA_s *selected = area_selected();
    if (selected == NULL) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(240, 90, 240, 250, ed_fnt, areaEditor_cbCancelRenameMenu, const_cast<char *>("Rename Area"));
    if (menu == NULL) {
        return;
    }
    eduiMenuAddItem(menu,
                    eduiItemTextPickCreate(0, &area_attr, areaEditor_cbRenameArea, const_cast<char *>("Area Name")));
    strcpy(static_cast<edui_textpicker_s *>(edui_last_item)->value, area_selected()->name);
    static_cast<edui_textpicker_s *>(edui_last_item)->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void areaEditor_cbAreaCylinderToggle(eduimenu_s *, eduiitem_s *item, unsigned int) {
    EDAIAREA_s *selected = area_selected();
    if (selected != NULL) {
        if (item->flags & EDUI_ITEM_HIGHLIGHTED) {
            selected->flags |= 1;
        } else {
            selected->flags &= ~1;
        }
    }
}

void areaEditor_Enter() {
    area_list()->head = NULL;
    area_list()->tail = NULL;
    for (i32 index = 0; index < 64; ++index) {
        EDAIAREA_s *pool = reinterpret_cast<EDAIAREA_s *>(reinterpret_cast<u8 *>(aieditor) + 0x36938);
        NuLinkedListAppend(area_free_list(), &pool[index].link);
    }
    if (aieditor->ai_system == NULL) {
        return;
    }
    for (i32 index = 0; index < aieditor->ai_system->area_count; ++index) {
        AIAREA *source = &aieditor->ai_system->areas[index];
        i16 rotation = source->rotation;
        u8 flags = source->game_flags;
        EDAIAREA_s *area = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetHead(area_free_list()));
        if (area != NULL) {
            NuLinkedListRemove(area_free_list(), &area->link);
            NuLinkedListAppend(area_list(), &area->link);
            area->position = source->position;
            memcpy(&area->size, &source->half_width, sizeof(area->size));
            area->rotation = rotation;
            area->flags = flags;
        }
        NuStrNCpy(area->name, source->name, 16);
    }
}

extern "C" void areaEditorSaveData() {
    i32 count = 0;
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        ++count;
    }
    EdFileWriteInt(count);
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        EdFileWrite(area->name, 16);
        EdFileWriteFloat(area->position.x);
        EdFileWriteFloat(area->position.y);
        EdFileWriteFloat(area->position.z);
        EdFileWriteFloat(area->size.x);
        EdFileWriteFloat(area->size.y);
        EdFileWriteFloat(area->size.z);
        EdFileWriteShort(area->rotation);
        if (aidata_version > 19) {
            EdFileWriteChar(area->flags);
        } else {
            EdFileWriteChar(0);
        }
        EdFileWriteChar(0);
    }
}

extern "C" void areaEditorDrawAreas() {
    f32 previous_distance = AiRndrCullDistance;
    SetAiRndrCullDistance(0.0f);
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        i32 colour;
        if (area_selected() == area) {
            colour = 0xff0000ff;
            if (area != area_hovered()) {
                colour = 0x800000ff;
            }
        } else {
            colour = area == area_hovered() ? 0xffffffff : 0;
        }
        if (area->flags & 1) {
            DrawAreaCylinder(&area->position, &area->size, colour);
        } else {
            DrawAreaBox(&area->position, &area->size, area->rotation, colour);
        }
    }
    SetAiRndrCullDistance(previous_distance);
}

void areaEditor_Render(i32 x, i32 y, f32, f32) {
    i32 text_x = (x + 10) * 16;
    NuQFntPrintEx(system_qfont, text_x, y * 8 - 40, 16, "Area Editor");
    NuQFntSetColour(system_qfont, 0x80000000);
    EDAIAREA_s *focus = area_selected() != NULL ? area_selected() : area_hovered();
    if (focus != NULL) {
        NUVEC delta;
        f32 range = NuVecXZDist(&focus->position, &aieditor->camera_position, &delta);
        y += 15;
        NuQFntPrintEx(system_qfont, text_x, y * 8, 16, "\"%s\", xzrng=%.2f", focus->name, static_cast<f64>(range));
    }
    i32 text_y = y * 8;
    NuQFntPrintEx(system_qfont, text_x, text_y + 120, 16, "SQR - Options");
    if (area_hovered() == NULL) {
        NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16, "X - Create area");
        NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "SELECT - Select nearest");
    } else if (area_hovered() != area_selected()) {
        NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16, "X - Select area");
        NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "SELECT - Goto nearest");
    } else {
        NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16, "X - Move selected");
        if ((aieditor->pad_buttons & 0x40) != 0) {
            if (focus->flags & 1) {
                NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "LRIGHT/LLEFT - Adjust Radius");
            } else {
                NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "LRIGHT/LLEFT - Adjust X size");
                NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "LUP/LDOWN - Adjust Z size");
            }
        } else {
            NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "TRI - Delete selected");
            i32 next_line = text_y + 480;
            if ((focus->flags & 1) == 0) {
                NuQFntPrintEx(system_qfont, text_x, next_line, 16, "LLEFT/LRIGHT - Rotate");
                next_line += 120;
            }
            NuQFntPrintEx(system_qfont, text_x, next_line, 16, "LUP - Increase height");
            NuQFntPrintEx(system_qfont, text_x, next_line + 120, 16, "LDOWN - Decrease height");
        }
    }
    areaEditorDrawAreas();
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
    antinodeEditorDrawAntinodes();
    locatorEditorDrawLocators();
}

static eduimenu_s *areaEditorOptionsMenu() {
    eduimenu_s *menu =
        eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
    if (menu == NULL) {
        return NULL;
    }
    eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ROUTES, &area_attr, 0, 0, aieditor_cvSelectEditorMode,
                                            const_cast<char *>("Select Editor Mode")));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &area_attr, 0, 0, aieditor_cbSave, const_cast<char *>("Save AI Data")));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(1, &area_attr, 0, 0, aieditor_cbGoToPlayer, const_cast<char *>("Go To Player")));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(1, &area_attr, 0, 0, aieditor_cbMovePlayer, const_cast<char *>("Move Player")));
    if (area_selected() != NULL && AIScriptNameFromIx(aieditor->ai_system, 0) != NULL) {
        eduiMenuAddItem(menu, eduiItemSelCreate(1, &area_attr, 0, 0, areaEditor_cbRenameAreaMenu,
                                                const_cast<char *>("Rename Area")));
    }
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &area_attr, -i32(aieditorsettings.stop_platforms), 3,
                                               aieditor_cbStopPlatformsToggle, const_cast<char *>("Stop Platforms")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &area_attr, -i32(aieditorsettings.snap_height_display), 2,
                                               aieditor_cbSnapHeightToggle, const_cast<char *>("Snap Height")));
    if (area_selected() != NULL) {
        eduiMenuAddItem(menu, eduiItemSelCreate(1, &area_attr, 0, 0, NULL, const_cast<char *>("=================")));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, &area_attr, area_selected()->flags & 1, 4,
                                                   areaEditor_cbAreaCylinderToggle, const_cast<char *>("Cylinder")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, &area_attr, 0, 0, NULL, const_cast<char *>("=================")));
    }
    return menu;
}

static eduimenu_s *areaEditorDeleteMenu() {
    eduimenu_s *menu =
        eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Delete area??"));
    if (menu != NULL) {
        eduiMenuAddItem(menu,
                        eduiItemSelCreate(0, &area_attr, 0, 0, areaEditor_cbDeleteArea, const_cast<char *>("No")));
        eduiMenuAddItem(menu,
                        eduiItemSelCreate(1, &area_attr, 0, 0, areaEditor_cbDeleteArea, const_cast<char *>("Yes")));
    }
    return menu;
}

static EDAIAREA_s *areaEditorFindHover() {
    EDAIAREA_s *nearest = NULL;
    f32 best_distance = 3.402823466e38f;
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        NUVEC difference;
        f32 distance = NuVecXZDistSqr(&aieditor->camera_position, &area->position, &difference);
        if (distance >= best_distance) {
            continue;
        }
        f32 height = aieditor->camera_position.y - area->position.y;
        if (area->flags & 1) {
            if (distance < area->size.x * area->size.x && height < area->size.y && height > -2.0f) {
                nearest = area;
                best_distance = distance;
            }
        } else {
            NuVecRotateY(&difference, &difference, -area->rotation);
            if (difference.x < area->size.x && difference.x > -area->size.x && difference.z < area->size.z &&
                difference.z > -area->size.z && height < area->size.y && height > -2.0f) {
                nearest = area;
                best_distance = distance;
            }
        }
    }
    return nearest;
}

static EDAIAREA_s *areaEditorCreateArea() {
    EDAIAREA_s *previous = area_selected();
    EDAIAREA_s *area = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetHead(area_free_list()));
    if (area == NULL) {
        area_selected() = NULL;
        return NULL;
    }
    NuLinkedListRemove(area_free_list(), &area->link);
    NuLinkedListAppend(area_list(), &area->link);
    area->position = aieditor->camera_position;
    area->rotation = static_cast<i16>(aieditorsettings.area_rotation);
    if (previous != NULL) {
        area->size = previous->size;
        area->flags = previous->flags;
    } else {
        area->size.x = 1.0f;
        area->size.y = 1.0f;
        area->size.z = 1.0f;
        area->flags = 0;
    }
    area_selected() = area;
    char name[16];
    i32 suffix = 0;
    for (;;) {
        sprintf(name, "Area%d", ++suffix);
        bool duplicate = false;
        for (EDAIAREA_s *other = area_head(); other != NULL; other = area_next(other)) {
            if (NuStrICmp(other->name, name) == 0) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            strcpy(area->name, name);
            return area;
        }
    }
}

static void areaEditorNormalizeCylinder() {
    EDAIAREA_s *selected = area_selected();
    if (selected != NULL && (selected->flags & 1) != 0 && selected->size.x != selected->size.z) {
        selected->size.x = NuFmin(selected->size.x, selected->size.z);
    }
}

eduimenu_s *areaEditor_Process(nupad_s *pad) {
    if ((pad->digital_buttons_pressed & 0x80) != 0) {
        return areaEditorOptionsMenu();
    }
    area_hovered() = areaEditorFindHover();
    if ((pad->digital_buttons & 0x40) != 0) {
        bool selected_hover_on_press = false;
        if ((pad->digital_buttons_pressed & 0x40) != 0) {
            if (area_hovered() != NULL) {
                area_selected() = area_hovered();
                aieditorsettings.area_rotation = area_selected()->rotation;
                edcamSetPos(&area_selected()->position);
                selected_hover_on_press = true;
            } else {
                areaEditorCreateArea();
            }
        }
        EDAIAREA_s *selected = area_selected();
        if (selected != NULL && area_hovered() != NULL) {
            if (!selected_hover_on_press) {
                selected->position = aieditor->camera_position;
            }
            u32 buttons = pad->digital_buttons;
            if (buttons & 0x2000) {
                selected->size.x *= 1.01f;
            } else if (buttons & 0x8000) {
                selected->size.x *= 0.99f;
            }
            if ((selected->flags & 1) != 0) {
                selected->size.z = selected->size.x;
            } else if (buttons & 0x1000) {
                selected->size.z *= 1.01f;
            } else if (buttons & 0x4000) {
                selected->size.z *= 0.99f;
            }
        }
        areaEditorNormalizeCylinder();
        return NULL;
    }
    if ((pad->digital_buttons_pressed & 0x10) != 0) {
        if (area_selected() != NULL && area_selected() == area_hovered()) {
            return areaEditorDeleteMenu();
        }
        areaEditorNormalizeCylinder();
        return NULL;
    }
    if ((pad->digital_buttons_pressed & 0x100) != 0) {
        EDAIAREA_s *nearest = NULL;
        f32 distance = 3.402823466e38f;
        for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
            NUVEC difference;
            f32 current = NuVecXZDistSqr(&aieditor->camera_position, &area->position, &difference);
            if (current < distance) {
                nearest = area;
                distance = current;
            }
        }
        area_selected() = nearest;
        if (nearest != NULL) {
            edcamSetPos(&nearest->position);
        }
        areaEditorNormalizeCylinder();
        return NULL;
    }
    u32 buttons = pad->digital_buttons;
    u32 pressed = pad->digital_buttons_pressed;
    EDAIAREA_s *selected = area_selected();
    if (buttons & 0x2000 || buttons & 0x8000) {
        if (selected != NULL && selected == area_hovered()) {
            aieditorsettings.area_rotation = selected->rotation;
        }
        if (buttons & 0x2000) {
            area_rotation_step() = pressed & 0x8000 ? 20 : area_rotation_step() + 20;
            if (area_rotation_step() > 600) {
                area_rotation_step() = 600;
            }
            aieditorsettings.area_rotation = NuAngAdd(aieditorsettings.area_rotation, area_rotation_step());
        } else {
            area_rotation_step() = pressed & 0x2000 ? 20 : area_rotation_step() + 20;
            if (area_rotation_step() > 600) {
                area_rotation_step() = 600;
            }
            aieditorsettings.area_rotation = NuAngSub(aieditorsettings.area_rotation, area_rotation_step());
        }
        if (selected != NULL && selected == area_hovered()) {
            selected->rotation = static_cast<i16>(aieditorsettings.area_rotation);
        }
        areaEditorNormalizeCylinder();
        return NULL;
    }
    if (buttons & 0x4000 || buttons & 0x1000) {
        if (selected != NULL && selected == area_hovered()) {
            selected->size.y *= buttons & 0x4000 ? 0.99f : 1.01f;
        }
        areaEditorNormalizeCylinder();
        return NULL;
    }
    if ((buttons & 0x100) != 0 && (pressed & 0x0a) != 0) {
        EDAIAREA_s *next = NULL;
        if (pressed & 0x08) {
            next = area_selected() == NULL
                       ? area_head()
                       : reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetNext(area_list(), &area_selected()->link));
            if (next == NULL) {
                next = area_head();
            }
        } else if (pressed & 0x02) {
            next = area_selected() == NULL
                       ? reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetTail(area_list()))
                       : reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetPrev(area_list(), &area_selected()->link));
            if (next == NULL) {
                next = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetTail(area_list()));
            }
        }
        area_selected() = next;
        if (next != NULL) {
            edcamSetPos(&next->position);
        }
    }
    areaEditorNormalizeCylinder();
    return NULL;
}
