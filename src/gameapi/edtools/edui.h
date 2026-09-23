#pragma once

#include "nu2api/nucore/common.h"
#include "decomp.h"

struct eduimenu_s;
struct eduiitem_s;
struct nupad_s;
struct edui_interact_s;
struct eduiiattr_s {
    u32 background;
    u32 text;
    u32 highlight;
    u32 disabled;
};
struct nugraph_s;
struct numtl_s;
typedef void (*EdUiMenuCallback)(eduimenu_s *menu, eduimenu_s *parent);
typedef void (*EdUiItemCallback)(eduimenu_s *menu, eduiitem_s *item, u32 value);

enum EdUiItemFlags {
    EDUI_ITEM_HIGHLIGHTED = 0x01,
    EDUI_ITEM_DISABLED = 0x02,
};

enum EdUiItemType {
    EDUI_ITEM_TOGGLE = 3,
    EDUI_ITEM_EXPANDER = 19,
};

enum EdUiExpanderFlags {
    EDUI_EXPANDER_OPEN = 1,
};

enum EdUiAnalogPad {
    EDUI_ANALOG_PAD_NONE = 0,
    EDUI_ANALOG_PAD_LEFT = 1,
    EDUI_ANALOG_PAD_RIGHT = 2,
};

enum EdUiCursorButtons {
    EDUI_CURSOR_SECONDARY = 0x10,
    EDUI_CURSOR_PRIMARY = 0x40,
};

// Fields not yet traced retain their original byte ranges.
struct eduiitem_s {
    eduiitem_s *next;
    eduiitem_s *previous;
    i32 type;
    union {
        i32 data;
        void *data_ptr;
        u8 unknown_0c[4];
    };
    u8 unknown_10;
    union {
        u8 flags;
        struct {
            u8 highlighted : 1;
            u8 disabled : 1;
            u8 unknown_flags : 6;
        };
    };
    i8 text_alignment;
    u8 unknown_13[5];
    i32 selection_group;
    i32 x;
    i32 y;
    char *text;
    u32 colours[4];
    i32 (*input)(eduimenu_s *, eduiitem_s *, u32, u32);
    i32 (*process)(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    i32 (*render)(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    void (*destroy)(eduimenu_s *, eduiitem_s *);
};

struct eduimenu_s {
    eduiitem_s *first;
    eduiitem_s *last;
    eduiitem_s *selected;
    eduiitem_s *field_0c;
    eduiitem_s *field_10;
    i32 x;
    i32 y;
    i32 width;
    i32 height;
    i32 field_24;
    i32 field_28;
    void *font;
    EdUiMenuCallback callback;
    char *title;
    u32 flags : 3;
    u32 unknown_flags : 29;
    eduimenu_s *child;
    eduimenu_s *parent;
};

struct ed_module_s {
    ed_module_s *next;
    ed_module_s *previous;
    const char *name;
    void (*init)();
    void (*close)();
    void (*activate)();
    void (*deactivate)();
    void (*apply)();
    void (*write)(i32 file);
    void (*read)(i32 file);
    u32 block_id;
    i32 (*process)(f32 delta_time, nupad_s *pad);
    void (*render)();
    void *reserved;
};

struct edui_slider_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    void (*changed)(eduimenu_s *, eduiitem_s *, u32);
    f32 normalized_value;
    f32 value;
    f32 minimum;
    f32 range;
    i32 change_timer;
    char *format;
    f32 granularity;
};

struct edui_sel_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    EdUiItemCallback selected;
    EdUiItemCallback held;
};

struct edui_text_selector_s : edui_slider_s {
    char **options;
};

struct edui_colour_slider_s : edui_slider_s {
    u8 red;
    u8 green;
    u8 blue;
    u8 unknown_6f;
};

struct edui_colour_pick_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    f32 cursor_x;
    f32 cursor_y;
    f32 hue;
    f32 saturation;
    f32 value;
    f32 red;
    f32 green;
    f32 blue;
    union {
        u8 interaction_flags;
        struct {
            u8 dragging_colour : 1;
            u8 dragging_saturation : 1;
            u8 confirm : 1;
            u8 unknown_interaction_flags : 5;
        };
    };
    u8 unknown_6d[3];
    EdUiItemCallback changed;
};

struct edui_texture_pick_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    f32 uv_x[2];
    f32 uv_y[2];
    EdUiItemCallback changed;
    numtl_s *material;
    i32 selected_corner;
    f32 zoom;
};

struct edui_expander_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    eduiitem_s *first_child;
    eduiitem_s *last_child;
    u32 open : 1;
    u32 unknown_flags : 31;
    f32 button_size;
    f32 button_x;
    f32 button_y;
    i32 depth;
    EdUiItemCallback changed;
};

struct edui_graph_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    nugraph_s *graph;
    nugraph_s *onion_skins[8];
    EdUiItemCallback changed;
    f32 cursor_x;
    f32 cursor_y;
    i32 width;
    i32 height;
    f32 x_scale;
    f32 y_scale;
    i32 selected_point;
    char x_label[16];
    char y_label[16];
    char title[16];
};

struct edui_file_pick_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    EdUiItemCallback changed;
    char *format;
    u16 reserved_54;
    u16 reopen_directory;
    char name[0x40];
    char directory[0x100];
    char filename[0x108];
    i32 (*compare_entries)(const void *, const void *);
};

struct edui_prop_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    u16 unknown_property_flags : 6;
    u16 button_type : 3;
    u16 remaining_property_flags : 7;
    u8 unknown_4e[6];
    f32 label_width;
    f32 button_size;
    f32 button_x;
    f32 button_y;
    char *property_text;
    u8 unknown_68[4];
    i32 depth;
    EdUiItemCallback selected;
    EdUiItemCallback button;
    EdUiItemCallback changed;
    i32 extra_data;
};

struct edui_textpicker_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    char value[256];
    i32 keyboard_column;
    i32 keyboard_row;
    i32 cursor;
    i16 editing;
    i16 max_length;
    EdUiItemCallback callback;
    char *format;
    u32 keyboard_flags;
};

struct edui_gradient_stage_s {
    f32 time;
    f32 hue;
    f32 saturation;
    f32 value;
    f32 red;
    f32 green;
    f32 blue;
    u32 colour;
    u32 metadata;
};

struct edui_gradient_node_s {
    edui_gradient_node_s *next;
    edui_gradient_node_s *previous;
    f32 time;
    u32 colour;
    f32 hue;
    f32 saturation;
    f32 value;
    u32 metadata;
};

struct edui_gradient_pick_s : eduiitem_s {
    i32 (*interact)(edui_interact_s *);
    edui_gradient_node_s *first_stage;
    edui_gradient_node_s *selected_stage;
    EdUiItemCallback changed;
    EdUiItemCallback press;
    EdUiItemCallback add;
    EdUiItemCallback remove;
    EdUiItemCallback copy;
    EdUiItemCallback paste;
    i32 change_timer;
};

struct edui_filter_s : edui_prop_s {
    eduiitem_s *first_child;
    u8 unknown_84[8];
};

DECOMP_ASSERT(sizeof(eduiitem_s) == 0x48, "eduiitem_s ABI");
DECOMP_ASSERT(offsetof(eduiitem_s, colours) == 0x28, "eduiitem_s colours ABI");
DECOMP_ASSERT(offsetof(eduiitem_s, input) == 0x38, "eduiitem_s input ABI");
DECOMP_ASSERT(sizeof(edui_sel_s) == 0x54, "edui_sel_s ABI");
DECOMP_ASSERT(sizeof(edui_slider_s) == 0x6c, "edui_slider_s ABI");
DECOMP_ASSERT(sizeof(edui_colour_pick_s) == 0x74, "edui_colour_pick_s ABI");
DECOMP_ASSERT(sizeof(edui_texture_pick_s) == 0x6c, "edui_texture_pick_s ABI");
DECOMP_ASSERT(sizeof(edui_expander_s) == 0x6c, "edui_expander_s ABI");
DECOMP_ASSERT(sizeof(edui_graph_s) == 0xc0, "edui_graph_s ABI");
DECOMP_ASSERT(sizeof(edui_file_pick_s) == 0x2a4, "edui_file_pick_s ABI");
DECOMP_ASSERT(sizeof(edui_filter_s) == 0x8c, "edui_filter_s ABI");
DECOMP_ASSERT(sizeof(edui_prop_s) == 0x80, "edui_prop_s ABI");
DECOMP_ASSERT(sizeof(edui_textpicker_s) == 0x168, "edui_textpicker_s ABI");
DECOMP_ASSERT(offsetof(edui_textpicker_s, value) == 0x4c, "edui_textpicker_s value ABI");
DECOMP_ASSERT(offsetof(edui_textpicker_s, max_length) == 0x15a, "edui_textpicker_s length ABI");
DECOMP_ASSERT(sizeof(edui_gradient_stage_s) == 0x24, "edui_gradient_stage_s ABI");
DECOMP_ASSERT(sizeof(edui_gradient_node_s) == 0x20, "edui_gradient_node_s ABI");
DECOMP_ASSERT(sizeof(edui_gradient_pick_s) == 0x70, "edui_gradient_pick_s ABI");

struct edui_interact_s {
    f32 x, y, width, height;
    eduimenu_s *menu;
    eduiitem_s *item;
    i32 (*callback)(edui_interact_s *);
    u32 buttons;
    i32 field_20;
};

extern "C" {
    extern i32 eduiPropTextPos;
    extern ed_module_s edptldesc;
    extern ed_module_s edgradesc;
    extern ed_module_s edbridesc;
    extern ed_module_s edanimdesc;
    extern ed_module_s edrtldesc;
    extern ed_module_s edpartdesc;
    extern ed_module_s edTimingDesc;
    void edmainInit(void *font, char *configuration_file);
    void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
    i32 edmainProcess(f32 delta_time, nupad_s *pad);
    void edmainRender(void);
    f32 edmainSetMainMenuScale(f32 scale);
    void edmainSetCursorEnabled(i32 enabled);
    void eduiInit(void);
    void eduiInitMaterials(void);
    void eduiRenderCursor(void);
    void eduiRenderInteracts(void);
    void eduiSetFont(void *font);
    i32 edmainActivate(ed_module_s *module, i32 notify);
    ed_module_s *edmainCurrent(void);
    i32 edmainRegister(ed_module_s *module);
    void edmainClose(void);
    eduimenu_s *edGetMainMenu(void);
    void eduiGetCursorCoords(f32 *x, f32 *y);
    void eduiSetCursorCoords(f32 x, f32 y);
    void eduiGetCursorDelta(f32 *x, f32 *y);
    void eduiProcessCursor(f32 delta_time, nupad_s *pad);
    void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
    i32 eduiUsedAlgPad(nupad_s *pad);
    extern i32 bUsingMenuFocus;
    i32 eduiGetUsingMenuFocus(void);
    i32 eduiGetCameraEnabled(void);
    void eduiSetUsingMenuFocus(i32 enabled);
    eduimenu_s *eduiGetActiveMenu(void);
    eduimenu_s *eduiGetActiveMenuParent(void);
    eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
    void eduiSetActiveMenu(eduimenu_s *menu);
    void eduiSetDefaultActiveMenu(eduimenu_s *menu);
    i32 eduiClearActiveMenu(void);
    i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
    i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
    i32 eduiMenuDetach(eduimenu_s *menu);
    i32 eduiMenuIsActive(eduimenu_s *menu);
    void eduiMenuEnsureSelection(eduimenu_s *menu);
    void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
    void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
    eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
    eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
    eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
    eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
    eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
    eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
    eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 maximum, f32 value, char *text);
    eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 maximum, i32 value, char *text);
    eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
    i32 eduiItemSetText(eduiitem_s *item, char *text);
    i32 eduiItemPropSetText(edui_prop_s *item, char *text);
    void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
    void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
    void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
    void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
    void eduiMenuDestroy(eduimenu_s *menu);
    void eduiMenuDestroyItems(eduimenu_s *menu);
    void eduicbMenuCloseAllexpanders(eduimenu_s *menu);
    void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
    void eduiMenuSelectFirstEntry(eduimenu_s *menu);
    i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
    i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
    i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
    i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
    i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
    void eduiFlushInteracts(void);
    i32 eduiCursorOverMenu(eduimenu_s *menu);
    i32 cbInteractMenuTitle(edui_interact_s *interact);
    i32 cbInteractMenuScrollUp(edui_interact_s *interact);
    i32 cbInteractMenuScrollDown(edui_interact_s *interact);
    void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
    void cbInteractMenuKeySelect(eduimenu_s *menu);
    eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback, char *title);
    extern eduiitem_s *edui_last_item;
    eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
    void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
    void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
    i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
    i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
    i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
    void eduiMenuRender(eduimenu_s *menu);
    void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
    void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
    void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
    void eduiMenuSortItemsByTxt(eduimenu_s *menu);
    void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
    f32 eduiGetAnalougePadValue(nupad_s *pad);
    void eduiSetGlobalSliderAccel(f32 acceleration);
    i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 count);
    void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
    eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
    void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
    void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
    void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
    void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
    void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
    void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
    void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
    void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
    void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
    void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
    void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
    edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
    edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
    eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
    eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
    eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
    eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
    eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
    eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
}
