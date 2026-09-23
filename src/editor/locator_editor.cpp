#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
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
    void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);
    extern void (*ClearAICreaturesFn)();
    extern u8 default_ngroup;
    extern u8 default_nacross;
    extern f32 default_xspacing;
    extern f32 default_zspacing;
    extern f32 default_stagger_start;
    extern u8 default_activate_difficulty;
    extern u8 default_min_n_respawns;
    extern u8 default_max_n_respawns;
    extern f32 default_min_t_respawn;
    extern f32 default_max_t_respawn;
    extern i32 aidata_version;
}

extern "C" {
    u8 default_ngroup = 1;
    u8 default_nacross = 2;
    f32 default_xspacing = 0.4f;
    f32 default_zspacing = 0.4f;
    f32 default_stagger_start;
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

struct LocatorCreatureRecord {
    NULISTLNK link;
    char name[0x10];
    char script_name[0x10];
    NUVEC position;
    i32 angle;
    u8 path_check[0x1c];
    u32 valid_positions;
    i16 type;
    u8 set;
    u8 group_count;
    u8 across_count;
    u8 unknown_5d[3];
    f32 x_spacing;
    f32 z_spacing;
    u32 flags;
    void *activation_area;
    f32 script_params[4];
    void *trigger_area;
    EDLOCATOR_s *locator;
    EDLOCATOR_s *respawn_locator;
    u8 difficulty;
    u8 min_respawns;
    u8 max_respawns;
    u8 activation;
    f32 min_respawn_time;
    f32 max_respawn_time;
    f32 stagger_start;
    f32 view_distance;
    f32 hear_distance;
    f32 max_view_height;
    f32 min_view_height;
};
DECOMP_ASSERT(sizeof(LocatorCreatureRecord) == 0xac, "locator-created creature record stride");
DECOMP_ASSERT(offsetof(LocatorCreatureRecord, type) == 0x58, "locator-created creature type offset");
DECOMP_ASSERT(offsetof(LocatorCreatureRecord, difficulty) == 0x8c, "locator-created creature defaults offset");
DECOMP_ASSERT(offsetof(LocatorCreatureRecord, path_check) == 0x38, "locator-created creature path check offset");

static __used__ void *CreateCreature(i32 type, nuvec_s *position, i32 angle) {
    if (type == -1) {
        return nullptr;
    }
    NULISTHDR *free_creatures = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
    LocatorCreatureRecord *creature = (LocatorCreatureRecord *)NuLinkedListGetHead(free_creatures);
    if (creature == nullptr) {
        return nullptr;
    }
    NuLinkedListRemove(free_creatures, &creature->link);
    NuLinkedListAppend(&aieditor->creatures, &creature->link);
    creature->type = type;
    creature->set = 0;
    creature->group_count = default_ngroup;
    creature->across_count = default_nacross;
    creature->x_spacing = default_xspacing;
    creature->z_spacing = default_zspacing;
    creature->stagger_start = default_stagger_start;
    creature->view_distance = GetViewRangeFn != nullptr ? GetViewRangeFn(type) : 1.0f;
    creature->hear_distance = GetHearDistanceFn != nullptr ? GetHearDistanceFn(type) : 1.0f;
    creature->max_view_height = GetMaxViewHeightFn != nullptr ? GetMaxViewHeightFn(type) : 1.0f;
    creature->min_view_height = GetMinViewHeightFn != nullptr ? GetMinViewHeightFn(type) : 1.0f;
    creature->difficulty = default_activate_difficulty;
    creature->min_respawns = default_min_n_respawns;
    creature->max_respawns = default_max_n_respawns;
    creature->min_respawn_time = default_min_t_respawn;
    creature->max_respawn_time = default_max_t_respawn;
    if (position != nullptr) {
        creature->position = *position;
        creature->angle = angle;
    }
    return creature;
}

static void *FindCreatureArea(const char *name) {
    if (name == nullptr)
        return nullptr;
    NULISTHDR *areas = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40);
    EditorNamedEntry *area = (EditorNamedEntry *)NuLinkedListGetHead(areas);
    while (area != nullptr) {
        if (NuStrICmp(area->name, name) == 0)
            return area;
        area = (EditorNamedEntry *)NuLinkedListGetNext(areas, &area->link);
    }
    return nullptr;
}

static EDLOCATOR_s *FindCreatureLocator(const char *name) {
    if (name == nullptr)
        return nullptr;
    EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
    while (locator != nullptr) {
        if (NuStrICmp(locator->name, name) == 0)
            return locator;
        locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
    }
    return nullptr;
}

void creatureEditor_Enter() {
    aieditor->creatures.head = nullptr;
    aieditor->creatures.tail = nullptr;
    NULISTHDR *free_creatures = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
    LocatorCreatureRecord *pool = reinterpret_cast<LocatorCreatureRecord *>(reinterpret_cast<u8 *>(aieditor) + 0x3131c);
    for (i32 i = 0; i < 128; ++i)
        NuLinkedListAppend(free_creatures, &pool[i].link);

    AISYS *system = aieditor->ai_system;
    if (system != nullptr) {
        for (i32 i = 0; i < system->creature_count; ++i) {
            AICREATURE *source = &system->creatures[i];
            LocatorCreatureRecord *creature =
                (LocatorCreatureRecord *)CreateCreature(source->type, &source->pos, source->y_rot);
            if (creature == nullptr)
                continue;
            EDAIPATH_s *path = pathEditor_GetPath(reinterpret_cast<const char *>(source->path_info.path));
            f32 tolerance = 0.0f;
            do {
                pathEditor_OnPathCheck(&creature->position, (EDAIPATHCHECK_s *)creature->path_check, path, tolerance);
                tolerance += 0.01f;
            } while (*reinterpret_cast<i32 *>(creature->path_check) == 0);
            i32 *path_angle = reinterpret_cast<i32 *>(creature->path_check + 0x18);
            *path_angle = NuAngSub(creature->angle, *path_angle);
            strcpy(creature->name, source->name);
            strcpy(creature->script_name, source->script_name);
            creature->set = source->set;
            creature->group_count = source->count;
            creature->across_count = source->count_across;
            creature->valid_positions = source->active_mask;
            creature->x_spacing = source->x_spacing;
            creature->flags = source->flags;
            creature->z_spacing = source->z_spacing;
            for (i32 p = 0; p < 4; ++p)
                creature->script_params[p] = source->script_params[p];
            AISCRIPT *script = AIScriptFind(system, creature->script_name, 1, 1, 1);
            if (script != nullptr) {
                for (i32 p = 0; p < 4; ++p) {
                    if ((source->flags & (2 << p)) == 0)
                        creature->script_params[p] = script->params[p].default_val;
                }
            }
            if (source->area != nullptr)
                creature->activation_area = FindCreatureArea(source->area->name);
            if (source->locator != nullptr)
                creature->locator = FindCreatureLocator(source->locator->name);
            if (source->respawn_locator != nullptr)
                creature->respawn_locator = FindCreatureLocator(source->respawn_locator->name);
            creature->activation = source->activate_type;
            if (source->activate_type == 1) {
                creature->activation = 0;
                if (source->activate_area != nullptr) {
                    creature->trigger_area = FindCreatureArea(source->activate_area->name);
                    if (creature->trigger_area != nullptr)
                        creature->activation = 1;
                }
            }
            creature->difficulty = source->activation_difficulty;
            creature->min_respawns = source->min_respawn_count;
            creature->max_respawns = source->max_respawn_count;
            creature->min_respawn_time = source->min_respawn_time;
            creature->max_respawn_time = source->max_respawn_time;
            creature->stagger_start = source->start_stagger;
            creature->view_distance = source->view_distance;
            creature->hear_distance = source->hear_distance;
            creature->max_view_height = source->max_view_height;
            creature->min_view_height = source->min_view_height;
        }
    }
    if (aieditorsettings.current_path_type == -1 && LevelCharacterGlobalIDFn != nullptr)
        aieditorsettings.current_path_type = LevelCharacterGlobalIDFn(0);
    if (ClearAICreaturesFn != nullptr)
        ClearAICreaturesFn();
    if (aieditorsettings.current_area_name[0] != 0) {
        LocatorCreatureRecord *creature = (LocatorCreatureRecord *)NuLinkedListGetHead(&aieditor->creatures);
        while (creature != nullptr) {
            if (NuStrICmp(creature->name, aieditorsettings.current_area_name) == 0) {
                aieditor->mode_selection_36930 = (EditorNamedEntry *)creature;
                aieditor_SetCurrentScript(creature->script_name, (AIEditorScriptSelection *)creature);
                break;
            }
            creature = (LocatorCreatureRecord *)NuLinkedListGetNext(&aieditor->creatures, &creature->link);
        }
    }
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

    void locatorEditorSaveData(AIPATHSYS_s *path_system) {
        i32 locator_count = 0;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            locator->runtime_index = 0xff;
            if (locator->path != nullptr) {
                locator->runtime_index = locator_count++;
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        EdFileWriteInt(locator_count);
        locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            if (locator->path != nullptr) {
                EdFileWrite(locator->name, 16);
                EdFileWriteFloat(locator->position.x);
                EdFileWriteFloat(locator->position.y);
                EdFileWriteFloat(locator->position.z);
                EdFileWriteShort(locator->direction);
                i32 path_index = locator->path->draw_index;
                EdFileWriteChar(path_index);
                i32 connection_index = 0;
                AIPATH_s *path = path_system->paths[path_index];
                for (i32 index = 0; index < path->connection_count; ++index) {
                    AIPATHCNX_s *connection = &path->connections[index];
                    i32 first = locator->first_node->index;
                    i32 second = locator->second_node->index;
                    if ((connection->node_indices[0] == first && connection->node_indices[1] == second) ||
                        (connection->node_indices[0] == second && connection->node_indices[1] == first)) {
                        connection_index = index;
                        break;
                    }
                }
                i32 angle = locator->path_angle;
                i32 magnitude = angle < 0 ? -angle : angle;
                EdFileWriteChar(magnitude > 0x3fff);
                EdFileWriteShort(connection_index);
                EdFileWriteFloat(locator->path_fraction);
                EdFileWriteFloat(locator->path_width);
                if (aidata_version > 14) {
                    EdFileWriteInt(angle);
                }
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        if (aidata_version > 17) {
            i32 set_count = 0;
            EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
            while (set != nullptr) {
                ++set_count;
                set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
            }
            EdFileWriteInt(set_count);
            set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
            while (set != nullptr) {
                i32 member_count = 0;
                for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
                    if (set->locators[index]->runtime_index != 0xff) {
                        ++member_count;
                    }
                }
                EdFileWrite(set->name, 16);
                EdFileWriteInt(member_count);
                if (member_count != 0) {
                    for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
                        if (set->locators[index]->runtime_index != 0xff) {
                            EdFileWriteChar(set->locators[index]->runtime_index);
                        }
                    }
                }
                set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
            }
        }
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

    void locatorEditor_PathDeleted(EDAIPATH_s *path) {
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            EDLOCATOR_s *next = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
            if (locator->path == path) {
                DestroyLocator(locator);
                if (aieditor->current_locator == locator) {
                    aieditor->current_locator = nullptr;
                }
            }
            locator = next;
        }
    }

    void locatorEditor_PathNodeDeleted(EDAIPATHNODE_s *node) {
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            EDLOCATOR_s *next = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
            if (locator->first_node == node || locator->second_node == node) {
                pathEditor_OnPathCheck(&locator->position, (EDAIPATHCHECK_s *)locator->path_check,
                                       aieditor->current_path, 0.0f);
                if (!locator->on_path) {
                    DestroyLocator(locator);
                    if (aieditor->current_locator == locator) {
                        aieditor->current_locator = nullptr;
                    }
                }
            }
            locator = next;
        }
    }

    void locatorEditor_PathNodeMoved(EDAIPATHNODE_s *node) {
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            if (locator->first_node == node || locator->second_node == node) {
                NUVEC difference;
                NUVEC direction;
                NUVEC movement;
                NuVecSub(&difference, &locator->second_node->position, &locator->first_node->position);
                NuVecNorm(&direction, &difference);
                f32 radius;
                if (locator->path_fraction > 1.0f) {
                    radius = locator->second_node->radius;
                } else if (locator->path_fraction < 0.0f) {
                    radius = locator->first_node->radius;
                } else {
                    radius = locator->second_node->radius * locator->path_fraction +
                             locator->first_node->radius * (1.0f - locator->path_fraction);
                }
                locator->position = locator->first_node->position;
                direction.x = -direction.x * radius;
                direction.z *= radius;
                NuVecScale(&movement, &difference, locator->path_fraction);
                NuVecAdd(&locator->position, &locator->position, &movement);
                NuVecScale(&movement, &direction, locator->path_width);
                NuVecAdd(&locator->position, &locator->position, &movement);
                locator->direction =
                    NuAngAdd((i32)(NuAtan2(difference.x, difference.z) * 10430.378f), locator->path_angle);
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
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
