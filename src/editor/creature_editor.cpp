
#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/edpath.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nurndr.h"
#include <float.h>
#include <stdio.h>
#include <string.h>
#pragma GCC optimize("O2", "omit-frame-pointer")
struct EDCREATURE_s;
struct AIPATHSYS_s;

// Private view of the fields accessed by creature editor callbacks.
struct CreatureEditorRecord {
    NULISTLNK link;
    char name[0x10];
    char script_name[0x10];
    NUVEC position;
    i32 angle;
    union {
        u8 path_check[0x1c];
        struct {
            u8 unknown_38[0x3c - 0x38];
            void *path;
            u8 unknown_40[0x54 - 0x40];
        };
    };
    u32 valid_positions;
    i16 character_type;
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
    void *locator;
    void *respawn_locator;
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
    f32 negative_min_view_height;
};
DECOMP_ASSERT(offsetof(CreatureEditorRecord, valid_positions) == 0x54, "creature valid position mask offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, position) == 0x28, "creature position offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_name) == 0x18, "creature script name offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_params) == 0x70, "creature script params offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, angle) == 0x34, "creature angle offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, character_type) == 0x58, "creature character type offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, path) == 0x3c, "creature path offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, set) == 0x5a, "creature set offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, flags) == 0x68, "creature flags offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation_area) == 0x6c, "creature activation area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, trigger_area) == 0x80, "creature trigger area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation) == 0x8f, "creature activation offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, view_distance) == 0x9c, "creature view distance offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, negative_min_view_height) == 0xa8, "creature min view height offset");
DECOMP_ASSERT(sizeof(CreatureEditorRecord) == 0xac, "editor creature record size");

static inline __attribute__((always_inline)) NULISTHDR *creatureEditor_AreaList() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40);
}

static inline __attribute__((always_inline)) NULISTHDR *creatureEditor_LocatorList() {
    return &aieditor->locators;
}

static inline __attribute__((always_inline)) NULISTLNK *creatureEditor_ListItem(NULISTHDR *list, i32 index) {
    if (index < 0)
        return nullptr;
    NULISTLNK *entry = NuLinkedListGetHead(list);
    for (i32 i = 0; entry != nullptr && i < index; ++i) {
        entry = NuLinkedListGetNext(list, entry);
    }
    return entry;
}

static inline __attribute__((always_inline)) CreatureEditorRecord *creatureEditor_Current() {
    return reinterpret_cast<CreatureEditorRecord *>(aieditor->mode_selection_36930);
}

extern i32 creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32);
extern i32 creatureEditor_IsSelectable(EDCREATURE_s *);
extern "C" EDLOCATOR_s *locatorEditor_GetNearest(i32);
extern "C" void AiRndrLine3d(NURND_VERTEX3D *, struct numtl_s *, struct numtx_s *);
extern "C" f32 aieditor_y_tolerance;
extern "C" void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);
extern "C" void aieditor_ClearMainMenu(void);
extern "C" i32 aidata_version;
static eduiitem_s *reset_params_option;
static u32 creature_editor_item_colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
extern "C" void *ed_fnt;
extern "C" i32 AIEDITOR_CREATURES;
extern "C" i32 AIEDITOR_ROUTES;
extern "C" void (*ClearAICreaturesFn)();
extern "C" u8 default_ngroup;
extern "C" u8 default_nacross;
extern "C" f32 default_xspacing;
extern "C" f32 default_zspacing;
extern "C" f32 default_stagger_start;
extern "C" u8 default_activate_difficulty;
extern "C" u8 default_min_n_respawns;
extern "C" u8 default_max_n_respawns;
extern "C" f32 default_min_t_respawn;
extern "C" f32 default_max_t_respawn;
extern "C" EDAIPATH_s *pathEditor_GetPath(const char *);
extern "C" i32 aisys_maxnumcreaturesets;
extern "C" void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
extern "C" void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
extern "C" void aieditor_cbShowCreaturesSetToggle(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_stagger_start(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_maxviewheight(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_minviewheight(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSelectRespawnLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetRespawnLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetTriggerArea(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_difficulty(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetActivation(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetAreaActivation(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_assigntoset(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbCancelMenu(eduimenu_s *, eduimenu_s *);
static void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbResetParams(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSelectTriggerArea(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSelectLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetScriptParam(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *, u32);

static __used__ void *CreateCreature(i32 type, nuvec_s *position, i32 angle) {
    if (type == -1) {
        return nullptr;
    }
    NULISTHDR *free_creatures = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
    CreatureEditorRecord *creature = (CreatureEditorRecord *)NuLinkedListGetHead(free_creatures);
    if (creature == nullptr) {
        return nullptr;
    }
    NuLinkedListRemove(free_creatures, &creature->link);
    NuLinkedListAppend(&aieditor->creatures, &creature->link);
    creature->character_type = type;
    creature->set = 0;
    creature->group_count = default_ngroup;
    creature->across_count = default_nacross;
    creature->x_spacing = default_xspacing;
    creature->z_spacing = default_zspacing;
    creature->stagger_start = default_stagger_start;
    creature->view_distance = GetViewRangeFn != nullptr ? GetViewRangeFn(type) : 1.0f;
    creature->hear_distance = GetHearDistanceFn != nullptr ? GetHearDistanceFn(type) : 1.0f;
    creature->max_view_height = GetMaxViewHeightFn != nullptr ? GetMaxViewHeightFn(type) : 1.0f;
    creature->negative_min_view_height = GetMinViewHeightFn != nullptr ? GetMinViewHeightFn(type) : 1.0f;
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

static __attribute__((always_inline, optimize("O3"))) inline void *FindCreatureArea(const char *name) {
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

static __attribute__((always_inline, optimize("O3"))) inline EDLOCATOR_s *FindCreatureLocator(const char *name) {
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

__attribute__((optimize("O3"))) void creatureEditor_Enter() {
    aieditor->creatures.head = nullptr;
    aieditor->creatures.tail = nullptr;
    NULISTHDR *free_creatures = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
    CreatureEditorRecord *pool = reinterpret_cast<CreatureEditorRecord *>(reinterpret_cast<u8 *>(aieditor) + 0x3131c);
    for (i32 i = 0; i < 128; ++i)
        NuLinkedListAppend(free_creatures, &pool[i].link);

    AISYS *system = aieditor->ai_system;
    if (system != nullptr) {
        for (i32 i = 0; i < system->creature_count; ++i) {
            AICREATURE *source = &system->creatures[i];
            CreatureEditorRecord *creature =
                (CreatureEditorRecord *)CreateCreature(source->type, &source->pos, source->y_rot);
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
                creature->trigger_area = FindCreatureArea(source->area->name);
            if (source->locator != nullptr)
                creature->locator = FindCreatureLocator(source->locator->name);
            if (source->respawn_locator != nullptr)
                creature->respawn_locator = FindCreatureLocator(source->respawn_locator->name);
            creature->activation = source->activate_type;
            if (source->activate_type == 1) {
                creature->activation = 0;
                if (source->activate_area != nullptr) {
                    creature->activation_area = FindCreatureArea(source->activate_area->name);
                    if (creature->activation_area != nullptr)
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
            creature->negative_min_view_height = source->min_view_height;
        }
    }
    if (aieditorsettings.current_path_type == -1 && LevelCharacterGlobalIDFn != nullptr)
        aieditorsettings.current_path_type = LevelCharacterGlobalIDFn(0);
    if (ClearAICreaturesFn != nullptr)
        ClearAICreaturesFn();
    if (aieditorsettings.current_area_name[0] != 0) {
        CreatureEditorRecord *creature = (CreatureEditorRecord *)NuLinkedListGetHead(&aieditor->creatures);
        while (creature != nullptr) {
            if (NuStrICmp(creature->name, aieditorsettings.current_area_name) == 0) {
                aieditor->mode_selection_36930 = (EditorNamedEntry *)creature;
                aieditor_SetCurrentScript(creature->script_name, (AIEditorScriptSelection *)creature);
                break;
            }
            creature = (CreatureEditorRecord *)NuLinkedListGetNext(&aieditor->creatures, &creature->link);
        }
    }
}

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM1 __attribute__((regparm(1)))
#else
#define CREATURE_EDITOR_REGPARM1
#endif
static __used__
    __attribute__((always_inline)) CREATURE_EDITOR_REGPARM1 void creatureEditor_Updated(EDCREATURE_s *creature) {
    CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(creature);
    record->valid_positions = 1;
    for (i32 i = 1; i < record->group_count; ++i) {
        nuvec_s position;
        if (creatureEditor_CalculatePos(creature, i, &position, 1)) {
            record->valid_positions |= ((i & 0x20) == 0) << i;
        }
    }
}
#undef CREATURE_EDITOR_REGPARM1

static __used__ void creatureEditor_cbActivationMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Activation Condition");
    if (menu == nullptr)
        return;
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, creature_editor_item_colours, 0, creatureEditor_cb_difficulty, 1,
                                                  9, creature->difficulty, "Activation Difficulty"));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, creature_editor_item_colours, creature->activation == 0, 1,
                                              creatureEditor_cbSetActivation, "AUTOMATIC"));
    eduiMenuAddItem(menu, eduiItemCheckCreate(2, creature_editor_item_colours, creature->activation == 2, 1,
                                              creatureEditor_cbSetActivation, "SCRIPT"));
    i32 index = 0;
    char label[64];
    NULISTHDR *list = creatureEditor_AreaList();
    for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr;
         link = NuLinkedListGetNext(list, link), ++index) {
        sprintf(label, "AREA \"%s\"", reinterpret_cast<char *>(link) + 8);
        bool selected = creatureEditor_Current()->activation_area == link;
        eduiMenuAddItem(menu, eduiItemCheckCreate(index, creature_editor_item_colours, selected, 1,
                                                  creatureEditor_cbSetAreaActivation, label));
        if (selected)
            menu->selected = edui_last_item;
        eduiMenuAttach(parent, menu);
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cbCancelMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void pathEditor_cbCancelDeleteCreatureMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void creatureEditor_cbDeleteCreature(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
    if (item != nullptr && item->data != 0 && creature != nullptr && creature == nearest) {
        NuLinkedListRemove(&aieditor->creatures, &creature->link);
        memset(creature, 0, sizeof(*creature));
        NULISTHDR *free_list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
        NuLinkedListAppend(free_list, &creature->link);
        aieditor->mode_selection_36930 = nullptr;
    }
    aieditor_ClearMainMenu();
}

static __used__ void creatureEditor_cbFlagsToggle(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        if (creature->flags & item->data) {
            creature->flags &= ~item->data;
        } else {
            creature->flags |= item->data;
        }
    }
}

static __used__ void creatureEditor_cbGroupMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Group Values");
    if (menu == nullptr)
        return;
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, creature_editor_item_colours, 0, creatureEditor_cb_ngroup, 1, 31,
                                                  creature->group_count, "Group Size"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_stagger_start,
                                               0.0f, 60.0f, creature->stagger_start, "Stagger Time"));
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, creature_editor_item_colours, 0, creatureEditor_cb_nacross, 1, 31,
                                                  creature->across_count, "Formation Width"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_xspacing, 0.2f,
                                               4.8f, creature->x_spacing, "Formation X Spacing"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_zspacing, 0.2f,
                                               4.8f, creature->z_spacing, "Formation Z Spacing"));
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cbRenameCreature(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    const char *name = reinterpret_cast<edui_textpicker_s *>(item)->value;
    if (creature == nullptr || name[0] == '\0')
        return;
    NULISTHDR *list = &aieditor->creatures;
    for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr; link = NuLinkedListGetNext(list, link)) {
        CreatureEditorRecord *other = reinterpret_cast<CreatureEditorRecord *>(link);
        if (NuStrICmp(other->name, name) == 0)
            return;
    }
    strcpy(creature->name, name);
}

static __used__ void creatureEditor_cbRenameCreatureMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(240, 90, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Rename Creature");
    if (menu == nullptr)
        return;
    eduiitem_s *item =
        eduiItemTextPickCreate(0, creature_editor_item_colours, creatureEditor_cbRenameCreature, "Creature Name");
    eduiMenuAddItem(menu, item);
    edui_textpicker_s *picker = reinterpret_cast<edui_textpicker_s *>(item);
    strcpy(picker->value, creature->name);
    picker->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbResetParams(eduimenu_s *menu,
                                                                                           eduiitem_s *, unsigned int) {
    aieditor_SetCurrentScript(aieditorsettings.current_script_name, nullptr);
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->flags &= ~0x1e;
        memcpy(creature->script_params, aieditorsettings.current_script_params, sizeof(creature->script_params));
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    reset_params_option = nullptr;
}

static __used__ void creatureEditor_cbRespawnMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Respawn Values");
    if (menu == nullptr)
        return;
    if (NuLinkedListGetHead(creatureEditor_LocatorList()) != nullptr) {
        char label[64];
        if (creature->respawn_locator != nullptr) {
            EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(creature->respawn_locator);
            sprintf(label, "Respawn Locator \"%s\"", locator->name);
        } else {
            strcpy(label, "Respawn Locator NONE");
        }
        eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0,
                                                creatureEditor_cbSelectRespawnLocator, label));
    }
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, creature_editor_item_colours, 0, creatureEditor_cb_min_n_respawns,
                                                  -1, 33, static_cast<i8>(creature->min_respawns), "Min Num Respawns"));
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, creature_editor_item_colours, 0, creatureEditor_cb_max_n_respawns,
                                                  -1, 33, static_cast<i8>(creature->max_respawns), "Max Num Respawns"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_min_t_respawn,
                                               0.0f, 60.0f, creature->min_respawn_time, "Min Respawn Time"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_max_t_respawn,
                                               0.0f, 60.0f, creature->max_respawn_time, "Max Respawn Time"));
    eduiMenuAttach(parent, menu);
}

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbVisionMenu(eduimenu_s *parent,
                                                                                          eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Vision");
    if (menu == nullptr)
        return;
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_viewdistance, 0.5f,
                                               49.5f, creature->view_distance, "View Distance"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_heardistance, 0.5f,
                                               49.5f, creature->hear_distance, "Hearing Distance"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_maxviewheight,
                                               0.1f, 99.9f, creature->max_view_height, "Max View Height"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cb_minviewheight,
                                               0.1f, 99.9f, -creature->negative_min_view_height, "Min View Height -"));
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cbScriptParams(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Script Params");
    if (menu == nullptr)
        return;
    char label[64];
    if (NuLinkedListGetHead(creatureEditor_AreaList()) != nullptr) {
        if (creature->trigger_area != nullptr) {
            sprintf(label, "Trigger Area \"%s\"", reinterpret_cast<char *>(creature->trigger_area) + 8);
        } else {
            strcpy(label, "Trigger Area NONE");
        }
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbSelectTriggerArea, label));
    }
    if (NuLinkedListGetHead(creatureEditor_LocatorList()) != nullptr) {
        if (creature->locator != nullptr) {
            sprintf(label, "Locator \"%s\"", reinterpret_cast<char *>(creature->locator) + 8);
        } else {
            strcpy(label, "Locator NONE");
        }
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbSelectLocator, label));
    }
    AISCRIPT *script = AIScriptFind(aieditor->ai_system, creature->script_name, 1, 1, 1);
    const char *param_name = script != nullptr ? script->params[0].name : nullptr;
    if (param_name != nullptr)
        sprintf(label, param_name);
    else
        sprintf(label, "Param%d", 0);
    eduiMenuAddItem(menu, eduiItemSliderCreate(0, creature_editor_item_colours, 0, creatureEditor_cbSetScriptParam,
                                               0.0f, 100.0f, aieditorsettings.current_script_params[0], label));
    eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);

    param_name = script != nullptr ? script->params[1].name : nullptr;
    if (param_name != nullptr)
        sprintf(label, param_name);
    else
        sprintf(label, "Param%d", 1);
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, creature_editor_item_colours, 0, creatureEditor_cbSetScriptParam,
                                               0.0f, 100.0f, aieditorsettings.current_script_params[1], label));
    eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);

    param_name = script != nullptr ? script->params[2].name : nullptr;
    if (param_name != nullptr)
        sprintf(label, param_name);
    else
        sprintf(label, "Param%d", 2);
    eduiMenuAddItem(menu, eduiItemSliderCreate(2, creature_editor_item_colours, 0, creatureEditor_cbSetScriptParam,
                                               0.0f, 100.0f, aieditorsettings.current_script_params[2], label));
    eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);

    param_name = script != nullptr ? script->params[3].name : nullptr;
    if (param_name != nullptr)
        sprintf(label, param_name);
    else
        sprintf(label, "Param%d", 3);
    eduiMenuAddItem(menu, eduiItemSliderCreate(3, creature_editor_item_colours, 0, creatureEditor_cbSetScriptParam,
                                               0.0f, 100.0f, aieditorsettings.current_script_params[3], label));
    eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);
    reset_params_option = nullptr;
    if ((aieditorsettings.current_script_flags & 0x1e) != 0) {
        reset_params_option = eduiMenuAddItem(menu, eduiItemToggleCreate(1, creature_editor_item_colours, 1, 1,
                                                                         creatureEditor_cbResetParams, "Reset Params"));
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cbSelectScript(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Script");
    if (menu == nullptr)
        return;
    for (i32 index = 0;; ++index) {
        char *name = AIScriptNameFromIx(aieditor->ai_system, index);
        if (name == nullptr)
            break;
        if (NuStrICmp(name, "Level") == 0)
            continue;
        CreatureEditorRecord *creature = creatureEditor_Current();
        bool selected = creature != nullptr && NuStrICmp(creature->script_name, name) == 0;
        eduiMenuAddItem(menu, eduiItemCheckCreate(index, creature_editor_item_colours, selected, 1,
                                                  creatureEditor_cbSetScript, name));
        if (selected)
            menu->selected = edui_last_item;
        eduiMenuAttach(parent, menu);
    }
}

static __used__ void creatureEditor_cbSelectType(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    if (LevelCharacterGlobalIDFn == nullptr || GlobalCharacterNameFn == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select AI Type");
    if (menu == nullptr)
        return;
    for (i32 index = 0;; ++index) {
        i32 type = LevelCharacterGlobalIDFn(static_cast<u8>(index));
        if (type == -1)
            break;
        bool selected = aieditorsettings.current_path_type == type;
        eduiMenuAddItem(menu, eduiItemCheckCreate(type, creature_editor_item_colours, selected, 1,
                                                  creatureEditor_cbSetType, GlobalCharacterNameFn(type)));
        if (selected)
            menu->selected = edui_last_item;
        eduiMenuAttach(parent, menu);
    }
}

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *item,
                                                                                         unsigned int) {
    if (item == nullptr || creatureEditor_Current() == nullptr)
        return;
    char *name = AIScriptNameFromIx(aieditor->ai_system, item->data);
    if (name == nullptr)
        return;
    aieditor_SetCurrentScript(name, nullptr);
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (NuStrICmp(aieditorsettings.current_script_name, creature->script_name) != 0) {
        strcpy(creature->script_name, aieditorsettings.current_script_name);
        memcpy(creature->script_params, aieditorsettings.current_script_params, sizeof(creature->script_params));
        creature->flags = (creature->flags & ~0x1e) | aieditorsettings.current_script_flags;
    }
}

static __used__ void creatureEditor_cbSetScriptParam(eduimenu_s *menu, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (item == nullptr || creature == nullptr)
        return;
    i32 index = item->data;
    f32 value = reinterpret_cast<edui_slider_s *>(item)->value;
    if (value == aieditorsettings.current_script_params[index])
        return;
    aieditorsettings.current_script_params[index] = value;
    u32 bit = (index & 0x20) != 0 ? 0 : 2u << index;
    aieditorsettings.current_script_flags |= bit;
    creature->script_params[index] = value;
    creature->flags = (creature->flags & ~0x1e) | aieditorsettings.current_script_flags;
    if (menu != nullptr && reset_params_option == nullptr) {
        reset_params_option = eduiMenuAddItem(menu, eduiItemToggleCreate(1, creature_editor_item_colours, 1, 1,
                                                                         creatureEditor_cbResetParams, "Reset Params"));
    }
}

static __used__ void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item != nullptr)
        aieditorsettings.current_path_type = item->data;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
    if (creature != nearest)
        return;
    i32 type = aieditorsettings.current_path_type;
    creature->character_type = type;
    if (GetViewRangeFn != nullptr) {
        const f32 value = GetViewRangeFn(type);
        creature->view_distance = value;
        creature = creatureEditor_Current();
    } else {
        creature->view_distance = 1.0f;
    }
    if (GetHearDistanceFn != nullptr) {
        const f32 value = GetHearDistanceFn(aieditorsettings.current_path_type);
        creature->hear_distance = value;
        creature = creatureEditor_Current();
    } else {
        creature->hear_distance = 1.0f;
    }
    if (GetMaxViewHeightFn != nullptr) {
        const f32 value = GetMaxViewHeightFn(aieditorsettings.current_path_type);
        creature->max_view_height = value;
        creature = creatureEditor_Current();
    } else {
        creature->max_view_height = 1.0f;
    }
    if (GetMinViewHeightFn != nullptr) {
        const f32 value = GetMinViewHeightFn(aieditorsettings.current_path_type);
        creature->negative_min_view_height = value;
    } else {
        creature->negative_min_view_height = 1.0f;
    }
}

static __used__ void creatureEditor_cbSetActivation(eduimenu_s *, eduiitem_s *item, unsigned int) {
    creatureEditor_Current()->activation = item->data;
}

static __used__ void creatureEditor_cbSetAreaActivation(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item == nullptr || creatureEditor_Current() == nullptr)
        return;
    NULISTLNK *area = creatureEditor_ListItem(creatureEditor_AreaList(), item->data);
    if (area != nullptr) {
        CreatureEditorRecord *creature = creatureEditor_Current();
        creature->activation_area = area;
        creature->activation = 1;
    }
}

static __used__ void creatureEditor_cbSelectLocator(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Locator");
    if (menu == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    eduiMenuAddItem(menu, eduiItemCheckCreate(-1, creature_editor_item_colours, creature->locator == nullptr, 1,
                                              creatureEditor_cbSetLocator, "NONE"));
    i32 index = 0;
    NULISTHDR *list = creatureEditor_LocatorList();
    for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr; link = NuLinkedListGetNext(list, link)) {
        EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(link);
        if (*reinterpret_cast<void **>(reinterpret_cast<u8 *>(locator) + 0x2c) != creature->path)
            continue;
        bool selected = creatureEditor_Current()->locator == locator;
        eduiMenuAddItem(menu, eduiItemCheckCreate(index, creature_editor_item_colours, selected, 1,
                                                  creatureEditor_cbSetLocator, locator->name));
        if (selected)
            menu->selected = edui_last_item;
        eduiMenuAttach(parent, menu);
        ++index;
    }
}

static __used__ void creatureEditor_cbSetLocator(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    if (item->data == -1) {
        creature->locator = nullptr;
        return;
    }
    NULISTHDR *list = creatureEditor_LocatorList();
    i32 index = 0;
    for (NULISTLNK *locator = NuLinkedListGetHead(list); locator != nullptr;
         locator = NuLinkedListGetNext(list, locator)) {
        if (*reinterpret_cast<void **>(reinterpret_cast<u8 *>(locator) + 0x2c) == creatureEditor_Current()->path) {
            if (index == item->data) {
                creatureEditor_Current()->locator = locator;
                return;
            }
            ++index;
        }
    }
}

static __used__ void creatureEditor_cbSelectRespawnLocator(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Locator");
    if (menu == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    eduiMenuAddItem(menu, eduiItemCheckCreate(-1, creature_editor_item_colours, creature->respawn_locator == nullptr, 1,
                                              creatureEditor_cbSetRespawnLocator, "NONE"));
    i32 index = 0;
    for (NULISTLNK *link = NuLinkedListGetHead(creatureEditor_LocatorList()); link != nullptr;
         link = NuLinkedListGetNext(creatureEditor_LocatorList(), link)) {
        EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(link);
        if (creatureEditor_Current()->respawn_locator == locator) {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, creature_editor_item_colours, 1, 1,
                                                      creatureEditor_cbSetRespawnLocator, locator->name));
            menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, creature_editor_item_colours, 0, 1,
                                                      creatureEditor_cbSetRespawnLocator, locator->name));
        }
        ++index;
        eduiMenuAttach(parent, menu);
    }
}

static __used__ void creatureEditor_cbSetRespawnLocator(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (item == nullptr || creature == nullptr)
        return;
    if (item->data == -1) {
        creature->locator = nullptr;
        return;
    }
    NULISTLNK *locator = creatureEditor_ListItem(creatureEditor_LocatorList(), item->data);
    if (locator != nullptr)
        creatureEditor_Current()->respawn_locator = locator;
}

static __used__ void creatureEditor_cbSelectTriggerArea(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Trigger Area");
    if (menu == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    bool none_selected = creature->trigger_area == nullptr;
    eduiMenuAddItem(menu, eduiItemCheckCreate(-1, creature_editor_item_colours, none_selected, 1,
                                              creatureEditor_cbSetTriggerArea, "NONE"));
    if (none_selected)
        menu->selected = edui_last_item;
    eduiMenuAttach(parent, menu);
    i32 index = 0;
    NULISTHDR *list = creatureEditor_AreaList();
    for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr;
         link = NuLinkedListGetNext(list, link), ++index) {
        bool selected = creatureEditor_Current()->trigger_area == link;
        eduiMenuAddItem(menu, eduiItemCheckCreate(index, creature_editor_item_colours, selected, 1,
                                                  creatureEditor_cbSetTriggerArea, reinterpret_cast<char *>(link) + 8));
        if (selected)
            menu->selected = edui_last_item;
        eduiMenuAttach(parent, menu);
    }
}

static __used__ void creatureEditor_cbSetTriggerArea(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (item == nullptr || creature == nullptr)
        return;
    if (item->data == -1) {
        creature->trigger_area = nullptr;
        return;
    }
    NULISTLNK *area = creatureEditor_ListItem(creatureEditor_AreaList(), item->data);
    if (area != nullptr)
        creatureEditor_Current()->trigger_area = area;
}

static __used__ void creatureEditor_cb_assigntoset(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->set = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
}

static __used__ void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->group_count = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

static __used__ void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->across_count = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

static __used__ void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->x_spacing = reinterpret_cast<edui_slider_s *>(item)->value;
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

static __used__ void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->z_spacing = reinterpret_cast<edui_slider_s *>(item)->value;
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

static __used__ void creatureEditor_cb_difficulty(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->difficulty = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
}

static __used__ void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->view_distance;
        bool exceeds_limit = value > limit;
        creature->hear_distance = value;
        if (exceeds_limit) {
            creature->hear_distance = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->hear_distance;
        bool below_limit = value < limit;
        creature->view_distance = value;
        if (below_limit) {
            creature->view_distance = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_minviewheight(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->negative_min_view_height = -reinterpret_cast<edui_slider_s *>(item)->value;
}

static __used__ void creatureEditor_cb_maxviewheight(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->max_view_height = reinterpret_cast<edui_slider_s *>(item)->value;
}

static __used__ void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        creature->min_respawns = static_cast<i32>(slider->value);
        if (static_cast<i8>(creature->min_respawns) > creature->max_respawns) {
            creature->min_respawns = creature->max_respawns;
            eduiItemSliderSetVal(slider, static_cast<i8>(creature->min_respawns));
        }
    }
}

static __used__ void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        creature->max_respawns = static_cast<i32>(slider->value);
        if (static_cast<i8>(creature->max_respawns) < creature->min_respawns) {
            creature->max_respawns = creature->min_respawns;
            eduiItemSliderSetVal(slider, static_cast<i8>(creature->max_respawns));
        }
    }
}

static __used__ void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->max_respawn_time;
        bool exceeds_limit = value > limit;
        creature->min_respawn_time = value;
        if (exceeds_limit) {
            creature->min_respawn_time = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->min_respawn_time;
        bool below_limit = value < limit;
        creature->max_respawn_time = value;
        if (below_limit) {
            creature->max_respawn_time = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_stagger_start(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->stagger_start = reinterpret_cast<edui_slider_s *>(item)->value;
}

extern "C" {

    f32 aieditor_y_tolerance = 0.1f;

    __attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditorSaveData(AIPATHSYS_s *system) {
        if (GlobalCharacterNameFn == nullptr) {
            EdFileWriteInt(0);
            return;
        }

        NULISTHDR *list = &aieditor->creatures;
        i32 count = 0;
        for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr; link = NuLinkedListGetNext(list, link)) {
            CreatureEditorRecord *creature = reinterpret_cast<CreatureEditorRecord *>(link);
            if (creature->path != nullptr && GlobalCharacterNameFn(creature->character_type) != nullptr)
                ++count;
        }
        EdFileWriteInt(count);

        for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr; link = NuLinkedListGetNext(list, link)) {
            CreatureEditorRecord *creature = reinterpret_cast<CreatureEditorRecord *>(link);
            char *character_name = GlobalCharacterNameFn(creature->character_type);
            if (creature->path == nullptr || character_name == nullptr)
                continue;

            EdFileWrite(creature->name, 0x10);
            EdFileWrite(creature->script_name, 0x10);
            EdFileWrite(character_name, aidata_version > 13 ? 0x20 : 0x10);
            EdFileWriteFloat(creature->position.x);
            EdFileWriteFloat(creature->position.y);
            EdFileWriteFloat(creature->position.z);
            EdFileWriteShort(creature->angle);
            if (aidata_version > 15)
                EdFileWriteChar(creature->set);
            EdFileWriteChar(creature->group_count);
            EdFileWriteChar(creature->across_count);
            EdFileWriteInt(creature->valid_positions);
            EdFileWriteFloat(creature->x_spacing);
            EdFileWriteFloat(creature->z_spacing);
            EdFileWriteInt(creature->flags);

            EDAIPATH_s *editor_path = reinterpret_cast<EDAIPATH_s *>(creature->path);
            EdFileWriteChar(editor_path->draw_index);
            AIPATH *runtime_path = system->paths[editor_path->draw_index];
            EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(creature) + 0x38);
            i16 connection_index = 0;
            bool connection_found = false;
            bool reversed = false;
            for (i32 index = 0; index < runtime_path->connection_count; ++index) {
                AIPATHCNX *connection = &runtime_path->connections[index];
                i32 first_index = check->first->index;
                i32 second_index = check->second->index;
                if (connection->node_indices[0] == first_index && connection->node_indices[1] == second_index) {
                    connection_index = index;
                    connection_found = true;
                    break;
                }
                if (connection->node_indices[0] == second_index && connection->node_indices[1] == first_index) {
                    connection_index = index;
                    connection_found = true;
                    reversed = true;
                    break;
                }
            }
            i32 path_angle = check->angle;
            const bool turned_around = (path_angle < 0 ? -path_angle : path_angle) >= 0x4000;
            EdFileWriteChar(connection_found && (turned_around != reversed));
            EdFileWriteShort(connection_index);

            for (i32 index = 0; index < 4; ++index)
                EdFileWriteFloat(creature->script_params[index]);
            if (creature->trigger_area != nullptr) {
                EdFileWriteInt(1);
                EdFileWrite(reinterpret_cast<char *>(creature->trigger_area) + 8, 0x10);
            } else {
                EdFileWriteInt(0);
            }
            if (creature->locator != nullptr) {
                EdFileWriteInt(1);
                EdFileWrite(reinterpret_cast<char *>(creature->locator) + 8, 0x10);
            } else {
                EdFileWriteInt(0);
            }
            if (aidata_version > 16) {
                if (creature->respawn_locator != nullptr) {
                    EdFileWriteInt(1);
                    EdFileWrite(reinterpret_cast<char *>(creature->respawn_locator) + 8, 0x10);
                } else {
                    EdFileWriteInt(0);
                }
            }

            if (aidata_version <= 7)
                continue;
            EdFileWriteChar(creature->difficulty);
            EdFileWriteChar(creature->min_respawns);
            EdFileWriteChar(creature->max_respawns);
            EdFileWriteChar(creature->activation);
            EdFileWriteFloat(creature->min_respawn_time);
            EdFileWriteFloat(creature->max_respawn_time);
            if (aidata_version > 9)
                EdFileWriteFloat(creature->stagger_start);
            if (creature->activation == 1) {
                EdFileWrite(reinterpret_cast<char *>(creature->activation_area) + 8, 0x10);
            }
            if (aidata_version <= 10)
                continue;
            EdFileWriteFloat(GetViewRangeFn != nullptr &&
                                     creature->view_distance == GetViewRangeFn(creature->character_type)
                                 ? 0.0f
                                 : creature->view_distance);
            EdFileWriteFloat(GetHearDistanceFn != nullptr &&
                                     creature->hear_distance == GetHearDistanceFn(creature->character_type)
                                 ? 0.0f
                                 : creature->hear_distance);
            EdFileWriteFloat(GetMaxViewHeightFn != nullptr &&
                                     creature->max_view_height == GetMaxViewHeightFn(creature->character_type)
                                 ? 0.0f
                                 : creature->max_view_height);
            EdFileWriteFloat(GetMinViewHeightFn != nullptr &&
                                     creature->negative_min_view_height == GetMinViewHeightFn(creature->character_type)
                                 ? 0.0f
                                 : creature->negative_min_view_height);
            EdFileWriteInt(0);
        }
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) EDCREATURE_s *creatureEditor_GetNearest(i32 use_radius) {
        if (GlobalCharacterHGobjFn == nullptr)
            return nullptr;

        EDCREATURE_s *nearest = nullptr;
        f32 nearest_distance = FLT_MAX;
        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
            EDCREATURE_s *creature = reinterpret_cast<EDCREATURE_s *>(link);
            if (!creatureEditor_IsSelectable(creature))
                continue;
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(creature);
            u8 *model = reinterpret_cast<u8 *>(GlobalCharacterHGobjFn(record->character_type));
            f32 min_height = 0.0f;
            f32 height = 0.4f;
            f32 radius = 0.4f;
            if (model != nullptr) {
                min_height = *reinterpret_cast<f32 *>(model + 0x1c4);
                height = *reinterpret_cast<f32 *>(model + 0x1c8);
                radius = *reinterpret_cast<f32 *>(model + 0x1cc);
            }
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->camera_position, &record->position, &delta);
            if (!(distance < nearest_distance))
                continue;
            if (use_radius == 0) {
                nearest = creature;
                nearest_distance = distance;
                continue;
            }
            f32 lower = record->position.y + min_height;
            if (lower > record->position.y) {
                height += min_height;
                lower = record->position.y;
            }
            if (!(distance < radius * radius))
                continue;
            if (aieditor->camera_position.y < lower - aieditor_y_tolerance)
                continue;
            if (aieditor->camera_position.y <= lower + height + aieditor_y_tolerance) {
                nearest = creature;
                nearest_distance = distance;
            }
        }
        return nearest;
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathDeleted(EDAIPATH_s *path) {
        NULISTHDR *list = &aieditor->creatures;
        NULISTHDR *free_list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
        for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr;) {
            NULISTLNK *next = NuLinkedListGetNext(list, link);
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            if (record->path == path) {
                NuLinkedListRemove(list, link);
                memset(record, 0, sizeof(*record));
                NuLinkedListAppend(free_list, link);
                if (aieditor->mode_selection_36930 == reinterpret_cast<EditorNamedEntry *>(record)) {
                    aieditor->mode_selection_36930 = nullptr;
                }
            }
            link = next;
        }
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *node) {
        NULISTHDR *list = &aieditor->creatures;
        NULISTHDR *free_list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
        for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr;) {
            NULISTLNK *next = NuLinkedListGetNext(list, link);
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(record) + 0x38);
            if (check->first == node || check->second == node) {
                pathEditor_OnPathCheck(&record->position, check, aieditor->current_path, 0.0f);
                if (check->on_path == 0) {
                    NuLinkedListRemove(list, link);
                    memset(record, 0, sizeof(*record));
                    NuLinkedListAppend(free_list, link);
                    if (aieditor->mode_selection_36930 == reinterpret_cast<EditorNamedEntry *>(record)) {
                        aieditor->mode_selection_36930 = nullptr;
                    }
                }
            }
            link = next;
        }
    }

    __attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_PathNodeMoved(EDAIPATHNODE_s *node) {
        NULISTHDR *list = &aieditor->creatures;
        for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr; link = NuLinkedListGetNext(list, link)) {
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(record) + 0x38);
            if (check->first != node && check->second != node)
                continue;

            NUVEC direction;
            NuVecSub(&direction, &check->second->position, &check->first->position);
            NUVEC perpendicular;
            NuVecNorm(&perpendicular, &direction);
            f32 fraction = check->fraction;
            f32 radius;
            if (fraction > 1.0f)
                radius = check->second->radius;
            else if (fraction < 0.0f)
                radius = check->first->radius;
            else
                radius = check->first->radius * (1.0f - fraction) + check->second->radius * fraction;

            f32 old_y = record->position.y;
            record->position = check->first->position;
            f32 normalized_x = perpendicular.x;
            perpendicular.x = perpendicular.z * radius;
            perpendicular.z = -normalized_x * radius;
            NUVEC scaled;
            NuVecScale(&scaled, &direction, fraction);
            NuVecAdd(&record->position, &record->position, &scaled);
            NuVecScale(&scaled, &perpendicular, check->width);
            NuVecAdd(&record->position, &record->position, &scaled);
            if (fabsf(old_y - record->position.y) > 1.5f)
                record->position.y = old_y;
            record->angle = NuAngAdd(static_cast<i32>(NuAtan2(direction.x, direction.z) * 10430.378f), check->angle);
        }
    }

    __attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_RenderAllCreatures(void) {
        if (GlobalCharacterRenderFn == nullptr)
            return;

        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            EDCREATURE_s *creature = reinterpret_cast<EDCREATURE_s *>(record);
            for (i32 group = 0; group < record->group_count; ++group) {
                if (((static_cast<u64>(record->valid_positions) >> group) & 1) == 0)
                    continue;
                NURND_VERTEX3D vertices[2];
                vertices[0].colour = 0xffffffff;
                vertices[1].colour = 0xffffffff;
                if (!creatureEditor_IsSelectable(creature))
                    continue;

                NUVEC position;
                creatureEditor_CalculatePos(creature, group, &position, 0);
                GlobalCharacterRenderFn(&position, static_cast<i16>(record->angle), record->character_type, 0,
                                        creature);

                if (record->locator != nullptr) {
                    EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(record->locator);
                    vertices[0].position = record->position;
                    vertices[1].position = locator->position;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                }
                if (record->respawn_locator != nullptr) {
                    EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(record->respawn_locator);
                    vertices[0].colour = 0x8000;
                    vertices[1].colour = 0x8000;
                    vertices[0].position = record->position;
                    vertices[1].position = locator->position;
                    vertices[0].position.y += 0.1f;
                    vertices[1].position.y += 0.1f;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                }
            }
        }
    }

} // extern "C"

__attribute__((optimize("O2"))) eduimenu_s *creatureEditor_Process(nupad_s *pad) {
    if (pad->digital_buttons_pressed & 0x80) {
        eduimenu_s *menu =
            eduiMenuCreate(200, 70, 240, 330, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
        if (menu == nullptr)
            return nullptr;
        eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ROUTES, creature_editor_item_colours, 0, 0,
                                                aieditor_cvSelectEditorMode, const_cast<char *>("Select Editor Mode")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, aieditor_cbSave,
                                                const_cast<char *>("Save AI Data")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, aieditor_cbGoToPlayer,
                                                const_cast<char *>("Go To Player")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, aieditor_cbMovePlayer,
                                                const_cast<char *>("Move Player")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbSelectType,
                                                const_cast<char *>("Select Creature Type")));
        CreatureEditorRecord *creature = creatureEditor_Current();
        if (creature != nullptr) {
            if (AIScriptNameFromIx(aieditor->ai_system, 0) != nullptr) {
                eduiMenuAddItem(menu,
                                eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbSelectScript,
                                                  const_cast<char *>("Select Script")));
            }
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbScriptParams,
                                              const_cast<char *>("Script Params")));
            eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, creature_editor_item_colours, 0,
                                                          creatureEditor_cb_assigntoset, 0, aisys_maxnumcreaturesets,
                                                          creature->set, const_cast<char *>("Assigned To Set")));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0,
                                                    creatureEditor_cbRenameCreatureMenu,
                                                    const_cast<char *>("Rename Creature")));
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbActivationMenu,
                                              const_cast<char *>("Activation Conditions")));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbRespawnMenu,
                                                    const_cast<char *>("Respawn Values")));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbGroupMenu,
                                                    const_cast<char *>("Group Values")));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0, creatureEditor_cbVisionMenu,
                                                    const_cast<char *>("Vision")));
            eduiMenuAddItem(menu, eduiItemToggleCreate(1, creature_editor_item_colours, creature->flags & 1, 1,
                                                       creatureEditor_cbFlagsToggle,
                                                       const_cast<char *>("Ignore Wall Splines")));
            eduiMenuAddItem(menu, eduiItemToggleCreate(0x20, creature_editor_item_colours, (creature->flags >> 5) & 1,
                                                       2, creatureEditor_cbFlagsToggle,
                                                       const_cast<char *>("Not On LowEnd Device")));
        }
        const i32 toggle_offset = creature != nullptr ? 2 : 0;
        eduiMenuAddItem(menu,
                        eduiItemToggleCreate(1, creature_editor_item_colours, -i32(aieditorsettings.stop_platforms),
                                             1 + toggle_offset, aieditor_cbStopPlatformsToggle,
                                             const_cast<char *>("Stop Platforms")));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, creature_editor_item_colours,
                                                   -i32(aieditorsettings.snap_height_display), 2 + toggle_offset,
                                                   aieditor_cbSnapHeightToggle, const_cast<char *>("Snap Height")));
        eduiMenuAddItem(menu,
                        eduiItemToggleCreate(1, creature_editor_item_colours, -i32(aieditorsettings.show_creatures_set),
                                             3 + toggle_offset, aieditor_cbShowCreaturesSetToggle,
                                             const_cast<char *>("Show Current Set")));
        return menu;
    }

    if ((pad->digital_buttons & 0x40) == 0) {
        if (pad->digital_buttons_pressed & 0x10) {
            CreatureEditorRecord *selected = creatureEditor_Current();
            CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
            if (selected != nullptr && selected == nearest) {
                eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, pathEditor_cbCancelDeleteCreatureMenu,
                                                  const_cast<char *>("Delete creature??"));
                if (menu == nullptr)
                    return nullptr;
                eduiMenuAddItem(menu, eduiItemSelCreate(0, creature_editor_item_colours, 0, 0,
                                                        creatureEditor_cbDeleteCreature, const_cast<char *>("No")));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, creature_editor_item_colours, 0, 0,
                                                        creatureEditor_cbDeleteCreature, const_cast<char *>("Yes")));
                return menu;
            }
            EDLOCATOR_s *locator = aieditor->nearest_locator;
            if (selected != nullptr && locator != nullptr) {
                if (selected->respawn_locator == locator)
                    selected->respawn_locator = nullptr;
                else if (selected->path == locator->path)
                    selected->respawn_locator = locator;
            } else {
                aieditor->mode_selection_36930 = nullptr;
            }
        } else if (pad->digital_buttons_pressed & 0x20) {
            CreatureEditorRecord *selected = creatureEditor_Current();
            EDLOCATOR_s *locator = aieditor->nearest_locator;
            if (selected != nullptr && locator != nullptr) {
                if (selected->locator == locator)
                    selected->locator = nullptr;
                else if (selected->path == locator->path)
                    selected->locator = locator;
            }
        } else {
            NULISTHDR *creatures = &aieditor->creatures;
            CreatureEditorRecord *selected = creatureEditor_Current();
            CreatureEditorRecord *next = nullptr;
            bool change_selection = false;
            if ((pad->digital_buttons_pressed & 0x1000) != 0 ||
                ((pad->digital_buttons & 0x100) != 0 && (pad->digital_buttons_pressed & 8) != 0)) {
                next = selected != nullptr
                           ? reinterpret_cast<CreatureEditorRecord *>(NuLinkedListGetNext(creatures, &selected->link))
                           : nullptr;
                if (next == nullptr)
                    next = reinterpret_cast<CreatureEditorRecord *>(NuLinkedListGetHead(creatures));
                change_selection = true;
            } else if ((pad->digital_buttons & 0x100) != 0 && (pad->digital_buttons_pressed & 2) != 0) {
                next = selected != nullptr
                           ? reinterpret_cast<CreatureEditorRecord *>(NuLinkedListGetPrev(creatures, &selected->link))
                           : nullptr;
                if (next == nullptr)
                    next = reinterpret_cast<CreatureEditorRecord *>(NuLinkedListGetTail(creatures));
                change_selection = true;
            } else if (pad->digital_buttons_pressed & 0x100) {
                next = reinterpret_cast<CreatureEditorRecord *>(creatureEditor_GetNearest(0));
                change_selection = true;
            }

            if (change_selection) {
                aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(next);
                if (next != nullptr) {
                    aieditor->current_path = reinterpret_cast<EDAIPATH_s *>(next->path);
                    edcamSetPos(&next->position);
                    aieditorsettings.current_path_type = next->character_type;
                    aieditor_SetCurrentScript(next->script_name,
                                              reinterpret_cast<const AIEditorScriptSelection *>(next));
                }
            } else if ((pad->digital_buttons & 0x100) == 0) {
                EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48);
                i32 angle = aieditorsettings.area_rotation;
                bool rotate = false;
                if (pad->digital_buttons & (0x2000 | 0x8000)) {
                    rotate = true;
                    CreatureEditorRecord *hover = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
                    if (selected != nullptr && selected == hover)
                        angle = selected->angle;
                    i32 &step = *reinterpret_cast<i32 *>(aieditor->unknown_36934);
                    const u32 direction = (pad->digital_buttons & 0x2000) ? 0x2000 : 0x8000;
                    if (pad->digital_buttons_pressed & direction)
                        step = 0x14;
                    else if (step < 600)
                        step += 0x14;
                    if (step > 600)
                        step = 600;
                    angle = (pad->digital_buttons & 0x2000) ? NuAngAdd(angle, step) : NuAngSub(angle, step);
                } else if (pad->digital_buttons & 0x4000) {
                    rotate = true;
                    const i32 relative = NuAngSub(angle, check->angle);
                    i32 quarter_turns = relative / 0x4000;
                    if (relative % 0x4000 > 0x2000)
                        ++quarter_turns;
                    else if (relative % 0x4000 < -0x2000)
                        --quarter_turns;
                    angle = NuAngAdd(quarter_turns << 14, check->angle);
                }
                if (rotate) {
                    aieditorsettings.area_rotation = angle;
                    CreatureEditorRecord *hover = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
                    if (selected != nullptr && selected == hover) {
                        selected->angle = angle;
                        check = reinterpret_cast<EDAIPATHCHECK_s *>(selected->path_check);
                        check->angle = NuAngSub(
                            angle, reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48)->angle);
                        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(selected));
                    }
                }
            }
        }
    } else {
        CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
        if (nearest != nullptr) {
            if (pad->digital_buttons_pressed & 0x40) {
                aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(nearest);
                aieditor->current_path = reinterpret_cast<EDAIPATH_s *>(nearest->path);
                aieditorsettings.area_rotation = nearest->angle;
                edcamSetPos(&nearest->position);
                aieditorsettings.current_path_type = nearest->character_type;
                aieditor_SetCurrentScript(nearest->script_name,
                                          reinterpret_cast<const AIEditorScriptSelection *>(nearest));
            } else {
                CreatureEditorRecord *selected = creatureEditor_Current();
                if (selected != nullptr) {
                    EDAIPATHCHECK_s *check =
                        reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48);
                    if (check->on_path == 0) {
                        edcamSetPos(&selected->position);
                    } else {
                        selected->position = aieditor->camera_position;
                        memcpy(selected->path_check, check, sizeof(selected->path_check));
                        reinterpret_cast<EDAIPATHCHECK_s *>(selected->path_check)->angle =
                            NuAngSub(selected->angle, check->angle);
                        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(selected));
                    }
                }
            }
        } else if ((pad->digital_buttons_pressed & 0x40) != 0 &&
                   reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48)->on_path != 0) {
            CreatureEditorRecord *previous = creatureEditor_Current();
            char base_name[0x10];
            u8 set = 0;
            if (previous == nullptr) {
                NuStrNCpy(base_name, GlobalCharacterNameFn(aieditorsettings.current_path_type), 0xd);
            } else {
                NuStrCpy(base_name, previous->name);
                char *suffix = strrchr(base_name, '_');
                if (suffix != nullptr)
                    *suffix = 0;
                base_name[12] = 0;
                set = previous->set;
            }

            CreatureEditorRecord *created = static_cast<CreatureEditorRecord *>(CreateCreature(
                aieditorsettings.current_path_type, &aieditor->camera_position, aieditorsettings.area_rotation));
            aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(created);
            if (created != nullptr) {
                char name[0x20];
                for (i32 number = 1;; ++number) {
                    sprintf(name, "%s_%d", base_name, number);
                    bool in_use = false;
                    for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
                         link = NuLinkedListGetNext(&aieditor->creatures, link)) {
                        if (NuStrICmp(name, reinterpret_cast<CreatureEditorRecord *>(link)->name) == 0) {
                            in_use = true;
                            break;
                        }
                    }
                    if (!in_use)
                        break;
                }
                strcpy(created->name, name);
                if (aieditorsettings.current_script_name[0] != 0)
                    NuStrCpy(created->script_name, aieditorsettings.current_script_name);
                memcpy(created->script_params, aieditorsettings.current_script_params, sizeof(created->script_params));
                created->flags = (created->flags & ~0x1eu) | aieditorsettings.current_script_flags;
                memcpy(created->path_check, reinterpret_cast<u8 *>(aieditor) + 0x48, sizeof(created->path_check));
                created->angle =
                    NuAngSub(created->angle, *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60));
                created->set = set;
                creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(created));
            }
        }
    }

    aieditor->nearest_locator = locatorEditor_GetNearest(1);
    *reinterpret_cast<EDCREATURE_s **>(aieditor->unknown_3692c) = creatureEditor_GetNearest(1);
    return nullptr;
}
