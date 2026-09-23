
#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/edpath.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
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
    i16 angle;
    u8 unknown_36[0x3c - 0x36];
    void *path;
    u8 unknown_40[0x54 - 0x40];
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
extern "C" void AiRndrLine3d(NURND_VERTEX3D *, struct numtl_s *, struct numtx_s *);
extern "C" f32 aieditor_y_tolerance;
extern "C" void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);
extern "C" void aieditor_ClearMainMenu(void);
static i32 reset_params_option;
static u32 creature_editor_item_colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
extern "C" void *ed_fnt;
extern "C" i32 AIEDITOR_CREATURES;
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
static void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *, u32);

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
                                              creatureEditor_cbSetActivation, "AREA"));
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

static __used__ void creatureEditor_cbDeleteCreature(eduimenu_s *menu, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
    if (menu != nullptr && menu->field_0c != nullptr && creature != nullptr && creature == nearest) {
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
    reset_params_option = 0;
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

static __used__ void creatureEditor_cbScriptParams(eduimenu_s *, eduiitem_s *, unsigned int) {
    STUBBED();
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
    for (i32 index = 0; index < 256; ++index) {
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

static __used__ void creatureEditor_cbSetScriptParam(eduimenu_s *, eduiitem_s *, unsigned int) {
    STUBBED();
}

static __used__ void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item != nullptr)
        aieditorsettings.current_path_type = item->data;
    CreatureEditorRecord *creature = creatureEditor_Current();
    CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
    if (creature == nullptr || creature != nearest)
        return;
    i32 type = aieditorsettings.current_path_type;
    creature->character_type = type;
    creature->view_distance = GetViewRangeFn != nullptr ? GetViewRangeFn(type) : 1.0f;
    creature->hear_distance = GetHearDistanceFn != nullptr ? GetHearDistanceFn(type) : 1.0f;
    creature->max_view_height = GetMaxViewHeightFn != nullptr ? GetMaxViewHeightFn(type) : 1.0f;
    creature->negative_min_view_height = GetMinViewHeightFn != nullptr ? GetMinViewHeightFn(type) : 1.0f;
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
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (item == nullptr || creature == nullptr)
        return;
    if (item->data == -1) {
        creature->locator = nullptr;
        return;
    }
    NULISTHDR *list = creatureEditor_LocatorList();
    i32 index = 0;
    for (NULISTLNK *locator = NuLinkedListGetHead(list); locator != nullptr;
         locator = NuLinkedListGetNext(list, locator)) {
        if (*reinterpret_cast<void **>(reinterpret_cast<u8 *>(locator) + 0x2c) == creature->path) {
            if (index == item->data) {
                creature->locator = locator;
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
         link = NuLinkedListGetNext(creatureEditor_LocatorList(), link), ++index) {
        EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(link);
        bool selected = creatureEditor_Current()->respawn_locator == locator;
        eduiMenuAddItem(menu, eduiItemCheckCreate(index, creature_editor_item_colours, selected, 1,
                                                  creatureEditor_cbSetRespawnLocator, locator->name));
        if (selected)
            menu->selected = edui_last_item;
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

    void creatureEditorSaveData(AIPATHSYS_s *) {
        STUBBED();
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
            if (distance >= nearest_distance)
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
            if (radius * radius <= distance)
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

    void creatureEditor_PathNodeMoved(EDAIPATHNODE_s *) {
        STUBBED();
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
                GlobalCharacterRenderFn(&position, record->angle, record->character_type, 0, creature);

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

eduimenu_s *creatureEditor_Process(nupad_s *pad) {
    if (pad->digital_buttons_pressed & 0x80) {
        eduimenu_s *menu =
            eduiMenuCreate(200, 70, 240, 330, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
        if (menu == nullptr)
            return nullptr;
        eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_CREATURES, creature_editor_item_colours, 0, 0,
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
        eduiMenuAddItem(menu,
                        eduiItemToggleCreate(1, creature_editor_item_colours, -i32(aieditorsettings.stop_platforms), 3,
                                             aieditor_cbStopPlatformsToggle, const_cast<char *>("Stop Platforms")));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, creature_editor_item_colours,
                                                   -i32(aieditorsettings.snap_height_display), 2,
                                                   aieditor_cbSnapHeightToggle, const_cast<char *>("Snap Height")));
        eduiMenuAddItem(
            menu, eduiItemToggleCreate(1, creature_editor_item_colours, -i32(aieditorsettings.show_creatures_set), 4,
                                       aieditor_cbShowCreaturesSetToggle, const_cast<char *>("Show Current Set")));
        return menu;
    }
    if (pad->digital_buttons_pressed & 0x10) {
        *reinterpret_cast<EDCREATURE_s **>(aieditor->unknown_3692c) = creatureEditor_GetNearest(1);
    }
    EDCREATURE_s *nearest = *reinterpret_cast<EDCREATURE_s **>(aieditor->unknown_3692c);
    if ((pad->digital_buttons & 0x40) && (pad->digital_buttons_pressed & 0x40) && nearest != nullptr) {
        CreatureEditorRecord *selected = reinterpret_cast<CreatureEditorRecord *>(nearest);
        aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(selected);
        aieditor->current_path = reinterpret_cast<EDAIPATH_s *>(selected->path);
        edcamSetPos(&selected->position);
    }
    return nullptr;
}
