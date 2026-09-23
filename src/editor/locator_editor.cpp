#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nu3d/nuqfnt.h"
#include <string.h>
#include <stdio.h>
#include <float.h>

extern "C" {
    extern void *ed_fnt;
    extern f32 aiEditor_LocatorWidth;
    struct EDCREATURE_s;
    EDCREATURE_s *creatureEditor_GetNearest(i32);
    extern i32 AIEDITOR_LOCATORS;
    void aieditor_ClearMainMenu();
    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
    void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
    EDAIPATH_s *pathEditor_GetPath(const char *);
    void areaEditorDrawAreas();
    void pathEditorDrawPaths();
    void antinodeEditorDrawAntinodes();
    void creatureEditor_RenderAllCreatures();
}

static u32 locator_attr[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
void DrawLocator(nuvec_s *, f32, i32, i32);
static void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *, eduimenu_s *);
static void locatorEditor_cbCancelRenameMenu(eduimenu_s *, eduimenu_s *);
static void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *);
static void DestroyLocator(EDLOCATOR_s *);
static unsigned int AddLocatorToSet(EDLOCATORSET_s *, EDLOCATOR_s *, EDLOCATOR_s *);
static __used__ void locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *, u32);

struct eduimenu_s;
struct eduiitem_s;
struct nupad_s;

static __used__ void locatorEditor_cbDeleteLocator(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0 && aieditor->current_locator != nullptr &&
        aieditor->current_locator == aieditor->nearest_locator) {
        EDCREATURELOCATOR_s *creature = (EDCREATURELOCATOR_s *)NuLinkedListGetHead(&aieditor->creatures);
        while (creature != nullptr) {
            if (creature->locator == aieditor->current_locator) {
                creature->locator = nullptr;
            }
            creature = (EDCREATURELOCATOR_s *)NuLinkedListGetNext(&aieditor->creatures, &creature->link);
        }
        DestroyLocator(aieditor->current_locator);
        aieditor->current_locator = nullptr;
    }
    aieditor_ClearMainMenu();
}
static __used__ void locatorEditor_cbRenameLocator(eduimenu_s *, eduiitem_s *item, u32) {
    char *name = ((edui_textpicker_s *)item)->value;
    if (aieditor->current_locator == nullptr || name[0] == 0) {
        return;
    }
    EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
    while (locator != nullptr) {
        if (NuStrICmp(locator->name, name) == 0) {
            return;
        }
        locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
    }
    strcpy(aieditor->current_locator->name, name);
}
static __used__ void locatorEditor_cbSetLocatorSet(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr) {
        if (item->data == 0) {
            aieditor->current_locator_set = nullptr;
        } else {
            EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
            for (i32 index = 1; set != nullptr && index < item->data; ++index) {
                set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
            }
            if (set != nullptr && set != aieditor->current_locator_set) {
                aieditor->current_locator_set = set;
                aieditor->current_locator = set->locators[0];
                if (aieditor->current_locator != nullptr) {
                    aieditor->current_path = aieditor->current_locator->path;
                    edcamSetPos(&aieditor->current_locator->position);
                }
            }
        }
    }
    aieditor_ClearMainMenu();
}
static __used__ void locatorEditor_cbEmptyLocatorSet(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (item == nullptr) {
        return;
    }
    if (item->data == 0) {
        eduimenu_s *menu =
            eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, nullptr, (char *)"Empty current locator set?");
        if (menu != nullptr) {
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(2, locator_attr, 0, 0, locatorEditor_cbEmptyLocatorSet, (char *)"No"));
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbEmptyLocatorSet, (char *)"Yes"));
            eduiMenuAttach(parent, menu);
        }
    } else if (item->data == 1) {
        if (aieditor->current_locator_set != nullptr) {
            memset(aieditor->current_locator_set->locators, 0, sizeof(aieditor->current_locator_set->locators));
        }
        aieditor_ClearMainMenu();
    } else if (item->data == 2) {
        aieditor_ClearMainMenu();
    }
}
static __used__ void locatorEditor_cbCreateLocatorSet(eduimenu_s *, eduiitem_s *, u32) {
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->free_locator_sets);
    if (set == nullptr) {
        return;
    }
    NuLinkedListRemove(&aieditor->free_locator_sets, &set->link);
    memset(set, 0, sizeof(*set));
    NuLinkedListAppend(&aieditor->locator_sets, &set->link);
    char name[16];
    i32 index = 0;
    EDLOCATORSET_s *other;
    do {
        sprintf(name, "NewSet%d", ++index);
        other = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
        while (other != nullptr) {
            if (NuStrICmp(name, other->name) == 0) {
                break;
            }
            other = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &other->link);
        }
    } while (other != nullptr);
    strcpy(set->name, name);
    aieditor->current_locator_set = set;
    aieditor->current_locator = nullptr;
    aieditor_ClearMainMenu();
}
static __used__ void locatorEditor_cbDeleteLocatorSet(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (item == nullptr) {
        return;
    }
    if (item->data == 0) {
        eduimenu_s *menu =
            eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, nullptr, (char *)"Delete current locator set?");
        if (menu != nullptr) {
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(2, locator_attr, 0, 0, locatorEditor_cbDeleteLocatorSet, (char *)"No"));
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbDeleteLocatorSet, (char *)"Yes"));
            eduiMenuAttach(parent, menu);
        }
    } else if (item->data == 1) {
        EDLOCATORSET_s *set = aieditor->current_locator_set;
        if (set != nullptr) {
            NuLinkedListRemove(&aieditor->locator_sets, &set->link);
            memset(set, 0, sizeof(*set));
            NuLinkedListAppend(&aieditor->free_locator_sets, &set->link);
            aieditor->current_locator_set = nullptr;
            aieditor_ClearMainMenu();
        }
    } else if (item->data == 2) {
        aieditor_ClearMainMenu();
    }
}
static __used__ void locatorEditor_cbRenameLocatorSet(eduimenu_s *, eduiitem_s *item, u32) {
    char *name = ((edui_textpicker_s *)item)->value;
    if (aieditor->current_locator_set == nullptr || name[0] == 0) {
        return;
    }
    if (NuStrICmp("NONE", name) == 0) {
        return;
    }
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
    while (set != nullptr) {
        if (NuStrICmp(set->name, name) == 0) {
            return;
        }
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
    }
    strcpy(aieditor->current_locator_set->name, name);
}
static __used__ void locatorEditor_cbSelectLocatorSet(eduimenu_s *parent, eduiitem_s *, u32) {
    eduimenu_s *menu = eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelSelectLocatorSet,
                                      (char *)"Select Locator Set");
    if (menu == nullptr) {
        return;
    }
    eduiMenuAddItem(menu, eduiItemSelCreate(0, locator_attr, 0, 0, locatorEditor_cbSetLocatorSet, (char *)"NONE"));
    i32 index = 1;
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
    while (set != nullptr) {
        eduiMenuAddItem(menu, eduiItemSelCreate(index, locator_attr, 0, 0, locatorEditor_cbSetLocatorSet, set->name));
        ++index;
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
    }
    eduiMenuAttach(parent, menu);
}
static __used__ void locatorEditor_cbAddLocatorsByName(eduimenu_s *, eduiitem_s *item, u32) {
    if (aieditor->current_locator_set != nullptr && ((edui_textpicker_s *)item)->value[0] != 0) {
        NuStrNCpy(aieditor->pending_locator_name, ((edui_textpicker_s *)item)->value, 16);
    }
}
static __used__ void locatorEditor_cbRenameLocatorMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (aieditor->current_locator == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelRenameMenu, (char *)"Rename Locator");
    if (menu == nullptr) {
        return;
    }
    edui_textpicker_s *item = (edui_textpicker_s *)eduiItemTextPickCreate(
        0, locator_attr, locatorEditor_cbRenameLocator, (char *)"Locator Name");
    eduiMenuAddItem(menu, item);
    strcpy(item->value, aieditor->current_locator->name);
    item->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}
static __used__ void locatorEditor_cbRenameLocatorSetMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (aieditor->current_locator_set == nullptr) {
        return;
    }
    eduimenu_s *menu = eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelRenameLocatorSetMenu,
                                      (char *)"Rename Locator Set");
    if (menu == nullptr) {
        return;
    }
    edui_textpicker_s *item =
        (edui_textpicker_s *)eduiItemTextPickCreate(0, locator_attr, locatorEditor_cbRenameLocatorSet, (char *)"Name");
    eduiMenuAddItem(menu, item);
    strcpy(item->value, aieditor->current_locator_set->name);
    item->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}
static __used__ void locatorEditor_cbAddLocatorsByNameMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (aieditor->current_locator_set == nullptr) {
        return;
    }
    eduimenu_s *menu = eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelRenameLocatorSetMenu,
                                      (char *)"Add Locators By Name");
    if (menu == nullptr) {
        return;
    }
    edui_textpicker_s *item =
        (edui_textpicker_s *)eduiItemTextPickCreate(0, locator_attr, locatorEditor_cbAddLocatorsByName, (char *)"Name");
    eduiMenuAddItem(menu, item);
    if (aieditor->current_locator != nullptr) {
        strcpy(aieditor->pending_locator_name, aieditor->current_locator->name);
    } else {
        strcpy(aieditor->pending_locator_name, aieditor->current_locator_set->name);
    }
    NuStrCpy(item->value, aieditor->pending_locator_name);
    item->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}
static __used__ void locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0 && aieditor->pending_locator_name[0] != 0 &&
        aieditor->current_locator_set != nullptr) {
        EDLOCATORSET_s *set = aieditor->current_locator_set;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            if (NuStrNICmp(aieditor->pending_locator_name, locator->name, -1) == 0) {
                EDLOCATOR_s *before = nullptr;
                for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
                    if (NuStrICmp(set->locators[index]->name, locator->name) > 0) {
                        before = set->locators[index];
                        break;
                    }
                }
                AddLocatorToSet(set, locator, before);
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        aieditor->current_locator = set->locators[0];
        if (aieditor->current_locator != nullptr) {
            aieditor->current_path = aieditor->current_locator->path;
            edcamSetPos(&aieditor->current_locator->position);
        }
    }
    memset(aieditor->pending_locator_name, 0, sizeof(aieditor->pending_locator_name));
    aieditor_ClearMainMenu();
}
static __used__ void locatorEditor_cbCancelRenameMenu(eduimenu_s *, eduimenu_s *menu) {
    eduiMenuDestroy(menu);
}
static __used__ void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *, eduimenu_s *menu) {
    eduiMenuDestroy(menu);
}
static __used__ void locatorEditor_cbCancelDeleteLocatorMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}
static __used__ void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void *CreateCreature(int, nuvec_s *, int) {
    STUBBED();
    return {};
}

static __used__ void DestroyLocator(EDLOCATOR_s *locator) {
    if (locator == nullptr) {
        return;
    }
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
    while (set != nullptr) {
        for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
            if (set->locators[index] == locator) {
                for (i32 move = index; move < 63; ++move) {
                    set->locators[move] = set->locators[move + 1];
                }
                set->locators[63] = nullptr;
                break;
            }
        }
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
    }
    NuLinkedListRemove(&aieditor->locators, &locator->link);
    memset(locator, 0, sizeof(*locator));
    NuLinkedListAppend(&aieditor->free_locators, &locator->link);
}

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before) {
    if (set == nullptr || locator == nullptr || set->locators[63] != nullptr) {
        return 0;
    }
    if (set->locators[0] != nullptr && set->locators[0]->path != locator->path) {
        return 0;
    }
    for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
        if (set->locators[index] == locator) {
            for (i32 move = index; move < 63; ++move) {
                set->locators[move] = set->locators[move + 1];
            }
            set->locators[63] = nullptr;
            break;
        }
    }
    if (before != nullptr) {
        for (i32 index = 0; index < 64; ++index) {
            if (set->locators[index] == before) {
                for (i32 move = 62; move >= index; --move) {
                    set->locators[move + 1] = set->locators[move];
                }
                set->locators[index] = locator;
                return 1;
            }
        }
    }
    for (i32 index = 0; index < 64; ++index) {
        if (set->locators[index] == nullptr) {
            set->locators[index] = locator;
            if (index < 63) {
                set->locators[index + 1] = nullptr;
            }
            return 1;
        }
    }
    return 0;
}

extern "C" {

    void locatorEditorDrawLocators(void) {
        AIEDITOR_RENDER_STATE *const *state = &aieditor;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&(*state)->locators);
        while (locator != nullptr) {
            locator->drawn = 0;
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&(*state)->locators, &locator->link);
        }
        if ((*state)->current_locator_set != nullptr) {
            for (i32 index = 0; index < 64 && (*state)->current_locator_set->locators[index] != nullptr; ++index) {
                locator = (*state)->current_locator_set->locators[index];
                i32 colour;
                if (locator == (*state)->current_locator) {
                    colour = locator == (*state)->nearest_locator ? 0xff0000ff : 0x800000ff;
                } else {
                    colour = locator == (*state)->nearest_locator ? -1 : 0x32323232;
                }
                DrawLocator(&locator->position, aiEditor_LocatorWidth, locator->direction, colour);
                locator->drawn = 1;
            }
        }
        locator = (EDLOCATOR_s *)NuLinkedListGetHead(&(*state)->locators);
        while (locator != nullptr) {
            if (!locator->drawn) {
                i32 colour;
                if ((*state)->current_locator_set != nullptr) {
                    if (locator == (*state)->current_locator) {
                        colour = locator == (*state)->nearest_locator ? 0x50 : 0x28;
                    } else {
                        colour = locator == (*state)->nearest_locator ? 0x64646464 : 0x32323232;
                    }
                } else if (locator == (*state)->current_locator) {
                    colour = locator == (*state)->nearest_locator ? 0xff0000ff : 0x800000ff;
                } else {
                    colour = locator == (*state)->nearest_locator ? -1 : 0x32323232;
                }
                DrawLocator(&locator->position, aiEditor_LocatorWidth, locator->direction, colour);
                locator->drawn = 1;
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&(*state)->locators, &locator->link);
        }
    }

    void locatorEditorSaveData(AIPATHSYS_s *) {
        STUBBED();
    }

    EDLOCATOR_s *locatorEditor_GetNearest(i32 use_width) {
        EDLOCATOR_s *nearest = nullptr;
        f32 nearest_distance = FLT_MAX;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->camera_position, &locator->position, &delta);
            if (distance < nearest_distance) {
                if (use_width != 0) {
                    NuVecRotateY(&delta, &delta, -locator->direction);
                    if (delta.x < aiEditor_LocatorWidth && delta.y < aiEditor_LocatorWidth &&
                        delta.z < aiEditor_LocatorWidth && -delta.x < aiEditor_LocatorWidth &&
                        -delta.y < aiEditor_LocatorWidth && -delta.z < aiEditor_LocatorWidth) {
                        nearest = locator;
                        nearest_distance = distance;
                    }
                } else {
                    nearest = locator;
                    nearest_distance = distance;
                }
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        return nearest;
    }

    void locatorEditor_PathDeleted(EDAIPATH_s *) {
        STUBBED();
    }

    void locatorEditor_PathNodeDeleted(EDAIPATHNODE_s *) {
        STUBBED();
    }

    void locatorEditor_PathNodeMoved(EDAIPATHNODE_s *) {
        STUBBED();
    }

} // extern "C"

void locatorEditor_Enter(void) {
    aieditor->locators.head = nullptr;
    aieditor->locators.tail = nullptr;
    aieditor->current_locator = nullptr;
    aieditor->nearest_locator = nullptr;
    for (i32 index = 0; index < 256; ++index) {
        NuLinkedListAppend(&aieditor->free_locators, &aieditor->locator_pool[index].link);
    }
    aieditor->locator_sets.head = nullptr;
    aieditor->locator_sets.tail = nullptr;
    aieditor->current_locator_set = nullptr;
    for (i32 index = 0; index < 64; ++index) {
        NuLinkedListAppend(&aieditor->free_locator_sets, &aieditor->locator_set_pool[index].link);
    }
    if (aieditor->ai_system != nullptr) {
        AISYS_s *system = aieditor->ai_system;
        for (i32 index = 0; index < system->locator_count; ++index) {
            AILOCATOR *source = &system->locators[index];
            EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->free_locators);
            if (locator != nullptr) {
                NuLinkedListRemove(&aieditor->free_locators, &locator->link);
                NuLinkedListAppend(&aieditor->locators, &locator->link);
                locator->position = source->position;
                locator->direction = source->direction;
            }
            strcpy(locator->name, source->name);
            EDAIPATH_s *path = pathEditor_GetPath((const char *)source->path_info.path);
            f32 tolerance = 0.0f;
            do {
                pathEditor_OnPathCheck(&locator->position, (EDAIPATHCHECK_s *)locator->path_check, path, tolerance);
                tolerance += 0.01f;
            } while (!locator->on_path);
            locator->path_angle = NuAngSub(locator->direction, locator->path_angle);
        }
        for (i32 index = 0; index < system->locator_set_count; ++index) {
            AILOCATORSET *source = &system->locator_sets[index];
            EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->free_locator_sets);
            if (set == nullptr) {
                break;
            }
            NuLinkedListRemove(&aieditor->free_locator_sets, &set->link);
            memset(set, 0, sizeof(*set));
            NuLinkedListAppend(&aieditor->locator_sets, &set->link);
            strcpy(set->name, source->name);
            for (i32 member = 0; member < source->locator_count; ++member) {
                set->locators[member] = &aieditor->locator_pool[source->locator_entries[member]];
            }
        }
    }
    if (aieditor->current_locator_set != nullptr) {
        strcpy(aieditorsettings.current_route_name, aieditor->current_locator_set->name);
    }
    if (aieditorsettings.current_route_name[0] == 0) {
        aieditor->current_locator_set = nullptr;
        return;
    }
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
    while (set != nullptr && NuStrICmp(aieditorsettings.current_route_name, set->name) != 0) {
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
    }
    aieditor->current_locator_set = set;
}

void locatorEditor_Render(i32 x, i32 y, float x_scale, float y_scale) {
    i32 text_x = (x + 10) * 16;
    i32 text_y = y * 8;
    if (aieditor->current_locator_set != nullptr) {
        NuQFntPrintEx(system_qfont, text_x, text_y - 40, 16, "Locator Editor: Set=\"%s\"",
                      aieditor->current_locator_set->name);
    } else {
        NuQFntPrintEx(system_qfont, text_x, text_y - 40, 16, "Locator Editor: Set=\"NONE\"");
    }
    NuQFntSetColour(system_qfont, 0x80000000);
    NuQFntSetScale(system_qfont, x_scale, y_scale);
    if (aieditor->current_locator != nullptr) {
        NUVEC delta;
        f32 distance = NuVecXZDist(&aieditor->current_locator->position, &aieditor->camera_position, &delta);
        NuQFntPrintEx(system_qfont, text_x, text_y + 120, 16, "\"%s\", xzrng=%.2f", aieditor->current_locator->name,
                      static_cast<f64>(distance));
    }
    NuQFntPrintEx(system_qfont, text_x, text_y + 240, 16, "SQR - Options");
    if (aieditor->nearest_locator == nullptr) {
        NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "X - Create locator");
    } else if (aieditor->nearest_locator != aieditor->current_locator) {
        NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "X - Select locator");
    } else {
        NuQFntPrintEx(system_qfont, text_x, text_y + 360, 16, "X - Move selected");
        NuQFntPrintEx(system_qfont, text_x, text_y + 480, 16, "TRI - Delete selected");
        NuQFntPrintEx(system_qfont, text_x, text_y + 600, 16, "LLEFT/LRight - Rotate");
    }
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    pathEditorDrawPaths();
    antinodeEditorDrawAntinodes();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
}

eduimenu_s *locatorEditor_Process(nupad_s *pad) {
    if ((pad->digital_buttons_pressed & 0x80) != 0) {
        eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, (char *)"Options");
        if (menu == nullptr) {
            return nullptr;
        }
        eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_LOCATORS, locator_attr, 0, 0, aieditor_cvSelectEditorMode,
                                                (char *)"Select Editor Mode"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, aieditor_cbSave, (char *)"Save AI Data"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, aieditor_cbGoToPlayer, (char *)"Go To Player"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, aieditor_cbMovePlayer, (char *)"Move Player"));
        if (aieditor->current_locator != nullptr && AIScriptNameFromIx(aieditor->ai_system, 0) != nullptr) {
            eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbRenameLocatorMenu,
                                                    (char *)"Rename Locator"));
        }
        eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbCreateLocatorSet,
                                                (char *)"Create Locator Set"));
        if (NuLinkedListGetHead(&aieditor->locator_sets) != nullptr) {
            eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbSelectLocatorSet,
                                                    (char *)"Select Locator Set"));
            if (aieditor->current_locator_set != nullptr) {
                eduiMenuAddItem(menu, eduiItemSelCreate(0, locator_attr, 0, 0, locatorEditor_cbDeleteLocatorSet,
                                                        (char *)"Delete Locator Set"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbRenameLocatorSetMenu,
                                                        (char *)"Rename Locator Set"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbAddLocatorsByNameMenu,
                                                        (char *)"Add Locators To Set By Name"));
                eduiMenuAddItem(menu, eduiItemSelCreate(0, locator_attr, 0, 0, locatorEditor_cbEmptyLocatorSet,
                                                        (char *)"Empty Locator Set"));
            }
        }
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, locator_attr, aieditorsettings.stop_platforms, 3,
                                                   aieditor_cbStopPlatformsToggle, (char *)"Stop Platforms"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, locator_attr, aieditorsettings.snap_height_display, 2,
                                                   aieditor_cbSnapHeightToggle, (char *)"Snap Height"));
        return menu;
    }
    if (NuStrLen(aieditor->pending_locator_name) != 0) {
        if (aieditor->current_locator_set != nullptr) {
            char title[128];
            sprintf(title, "Add locators \"%s\" to set \"%s\"?", aieditor->pending_locator_name,
                    aieditor->current_locator_set->name);
            eduimenu_s *menu = eduiMenuCreate(100, 70, 440, 270, ed_fnt, aieditor_cbCancelMainMenu, title);
            if (menu != nullptr) {
                eduiMenuAddItem(menu, eduiItemSelCreate(0, locator_attr, 0, 0, locatorEditor_cbAddLocatorsByNameYesNo,
                                                        (char *)"No"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbAddLocatorsByNameYesNo,
                                                        (char *)"Yes"));
                return menu;
            }
        }
        memset(aieditor->pending_locator_name, 0, sizeof(aieditor->pending_locator_name));
    }
    if ((pad->digital_buttons & 0x40) != 0) {
        if (aieditor->nearest_locator != nullptr) {
            if ((pad->digital_buttons_pressed & 0x40) != 0) {
                aieditor->current_locator = aieditor->nearest_locator;
                aieditor->current_path = aieditor->current_locator->path;
                aieditorsettings.area_rotation = aieditor->current_locator->direction;
                edcamSetPos(&aieditor->current_locator->position);
            } else if (aieditor->current_locator != nullptr &&
                       *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x48) != 0) {
                EDLOCATOR_s *locator = aieditor->current_locator;
                locator->position = aieditor->camera_position;
                memcpy(locator->path_check, reinterpret_cast<u8 *>(aieditor) + 0x48, 0x1c);
                locator->path_angle = NuAngSub(locator->direction, locator->path_angle);
            }
        } else if ((pad->digital_buttons_pressed & 0x40) != 0 &&
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x48) != 0) {
            EDLOCATOR_s *previous = aieditor->current_locator;
            EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->free_locators);
            if (locator != nullptr) {
                NuLinkedListRemove(&aieditor->free_locators, &locator->link);
                NuLinkedListAppend(&aieditor->locators, &locator->link);
                locator->position = aieditor->camera_position;
                locator->direction = aieditorsettings.area_rotation;
            }
            aieditor->current_locator = locator;
            if (locator != nullptr) {
                char base[32];
                if (aieditor->current_locator_set != nullptr && aieditor->current_locator_set->name[0] != 0) {
                    NuStrCpy(base, aieditor->current_locator_set->name);
                } else if (previous != nullptr) {
                    NuStrCpy(base, previous->name);
                } else {
                    NuStrCpy(base, "Locator");
                }
                char *suffix = strrchr(base, '_');
                if (suffix != nullptr) {
                    *suffix = 0;
                }
                char name[16];
                i32 index = 0;
                EDLOCATOR_s *other;
                do {
                    sprintf(name, "%s_%d", base, ++index);
                    other = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
                    while (other != nullptr && NuStrICmp(name, other->name) != 0) {
                        other = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &other->link);
                    }
                } while (other != nullptr);
                strcpy(locator->name, name);
                memcpy(locator->path_check, reinterpret_cast<u8 *>(aieditor) + 0x48, 0x1c);
                locator->path_angle = NuAngSub(locator->direction, locator->path_angle);
                if (aieditor->current_locator_set != nullptr) {
                    AddLocatorToSet(aieditor->current_locator_set, locator, nullptr);
                }
            }
        }
    } else if ((pad->digital_buttons_pressed & 0x10) != 0 && aieditor->current_locator != nullptr &&
               aieditor->current_locator == aieditor->nearest_locator) {
        eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, locatorEditor_cbCancelDeleteLocatorMenu,
                                          (char *)"Delete locator??");
        if (menu != nullptr) {
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(0, locator_attr, 0, 0, locatorEditor_cbDeleteLocator, (char *)"No"));
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(1, locator_attr, 0, 0, locatorEditor_cbDeleteLocator, (char *)"Yes"));
        }
        return menu;
    } else if ((pad->digital_buttons_pressed & 0x10) != 0) {
        // A delete press without a matching selected locator only refreshes hover state.
    } else if ((pad->digital_buttons & 0x100) != 0 && (pad->digital_buttons_pressed & (0x08 | 0x02)) != 0) {
        EDLOCATOR_s *next = nullptr;
        EDLOCATORSET_s *set = aieditor->current_locator_set;
        if (set != nullptr) {
            i32 index = -1;
            for (i32 i = 0; i < 64; ++i) {
                if (set->locators[i] == aieditor->current_locator) {
                    index = i;
                    break;
                }
            }
            if ((pad->digital_buttons_pressed & 0x08) != 0) {
                next = index < 0 || index >= 63 || set->locators[index + 1] == nullptr ? set->locators[0]
                                                                                       : set->locators[index + 1];
            } else {
                if (index > 0) {
                    next = set->locators[index - 1];
                } else {
                    for (i32 i = 63; i >= 0; --i) {
                        if (set->locators[i] != nullptr) {
                            next = set->locators[i];
                            break;
                        }
                    }
                }
            }
        } else if ((pad->digital_buttons_pressed & 0x08) != 0) {
            next = aieditor->current_locator == nullptr
                       ? (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators)
                       : (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &aieditor->current_locator->link);
            if (next == nullptr) {
                next = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
            }
        } else {
            next = aieditor->current_locator == nullptr
                       ? (EDLOCATOR_s *)NuLinkedListGetTail(&aieditor->locators)
                       : (EDLOCATOR_s *)NuLinkedListGetPrev(&aieditor->locators, &aieditor->current_locator->link);
            if (next == nullptr) {
                next = (EDLOCATOR_s *)NuLinkedListGetTail(&aieditor->locators);
            }
        }
        aieditor->current_locator = next;
        if (next != nullptr) {
            aieditor->current_path = next->path;
            edcamSetPos(&next->position);
        }
    } else if ((pad->digital_buttons_pressed & 0x01) != 0) {
        aieditor->current_locator = locatorEditor_GetNearest(0);
        if (aieditor->current_locator != nullptr) {
            aieditor->current_path = aieditor->current_locator->path;
            edcamSetPos(&aieditor->current_locator->position);
        }
    } else if ((pad->digital_buttons & (0x2000 | 0x8000)) != 0) {
        i32 &step = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x36934);
        if (aieditor->current_locator != nullptr && aieditor->current_locator == aieditor->nearest_locator) {
            aieditorsettings.area_rotation = aieditor->current_locator->direction;
        }
        if ((pad->digital_buttons & 0x2000) != 0) {
            step = (pad->digital_buttons_pressed & 0x2000) != 0 ? 20 : step + 20;
            if (step > 600) {
                step = 600;
            }
            aieditorsettings.area_rotation = NuAngAdd(aieditorsettings.area_rotation, step);
        } else {
            step = (pad->digital_buttons_pressed & 0x8000) != 0 ? 20 : step + 20;
            if (step > 600) {
                step = 600;
            }
            aieditorsettings.area_rotation = NuAngSub(aieditorsettings.area_rotation, step);
        }
        if (aieditor->current_locator != nullptr && aieditor->current_locator == aieditor->nearest_locator) {
            aieditor->current_locator->direction = aieditorsettings.area_rotation;
            aieditor->current_locator->path_angle =
                NuAngSub(aieditor->current_locator->direction,
                         *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60));
        }
    }
    *reinterpret_cast<EDCREATURE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x3692c) = creatureEditor_GetNearest(1);
    aieditor->nearest_locator = locatorEditor_GetNearest(1);
    return nullptr;
}
