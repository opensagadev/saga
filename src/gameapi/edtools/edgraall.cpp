#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/nucore/nustring.h"
#include <string.h>

extern "C" void edgraInitAllClumps(void);
extern "C" void *ed_fnt;
extern "C" u32 edblack[4];
extern "C" u32 edgrey[4];

static void edgraAttachMenu(eduimenu_s *parent, eduimenu_s *child) {
    eduiMenuAttach(parent, child);
    child->x = parent->x + 10;
    child->y = parent->y + 40;
}

// The two fade sliders are retained while their respective menus are open.
static edui_slider_s *edgra_global_fadein_slider;
static edui_slider_s *edgra_global_fadeout_slider;
static edui_slider_s *edgra_clump_fadein_slider;
static edui_slider_s *edgra_clump_fadeout_slider;
static i32 edgra_mode;
static i32 edgra_superscale = 64;
static i32 edgra_copy_source = -1;

// The original editor kept these local callback symbols in one translation unit.
// The current split leaves some menu entry paths in another unit, so retain the
// callbacks during -O3 reconstruction until that call graph is reconnected.
#define EDGRA_RETAIN_ITEM(name) static void name(eduimenu_s *, eduiitem_s *, u32) __attribute__((used))
#define EDGRA_RETAIN_MENU(name) static void name(eduimenu_s *, eduimenu_s *) __attribute__((used))
EDGRA_RETAIN_ITEM(edgracbFileLoad);
EDGRA_RETAIN_ITEM(edgracbFileSave);
EDGRA_RETAIN_ITEM(edgracbCopyClump);
EDGRA_RETAIN_ITEM(edgraToggleFilter);
EDGRA_RETAIN_ITEM(edgracbSScaleMenu);
EDGRA_RETAIN_ITEM(edgracbDiscardCopy);
EDGRA_RETAIN_ITEM(edgracbGlobalsMenu);
EDGRA_RETAIN_ITEM(edgracbSetDpadMode);
EDGRA_RETAIN_ITEM(edgracbChangeSScale);
EDGRA_RETAIN_ITEM(edgracbDpadModeMenu);
EDGRA_RETAIN_ITEM(edgracbInstanceMenu);
EDGRA_RETAIN_ITEM(edgracbSetClumpArea);
EDGRA_RETAIN_ITEM(edgracbSetClumpDist);
EDGRA_RETAIN_ITEM(edgracbSetClumpMode);
EDGRA_RETAIN_ITEM(edgracbSetClumpWind);
EDGRA_RETAIN_ITEM(edgracbClumpAreaMenu);
EDGRA_RETAIN_ITEM(edgracbClumpDistMenu);
EDGRA_RETAIN_ITEM(edgracbClumpFadeMenu);
EDGRA_RETAIN_ITEM(edgracbClumpModeMenu);
EDGRA_RETAIN_ITEM(edgracbClumpSizesMenu);
EDGRA_RETAIN_ITEM(edgracbSetClumpFadeIn);
EDGRA_RETAIN_ITEM(edgracbSetClumpHeight);
EDGRA_RETAIN_ITEM(edgracbApplyGlobalFade);
EDGRA_RETAIN_ITEM(edgracbSetClumpFadeOut);
EDGRA_RETAIN_ITEM(edgracbSetGlobalFadeIn);
EDGRA_RETAIN_ITEM(edgracbSetInstanceType);
EDGRA_RETAIN_ITEM(edgracbToggleClumpTilt);
EDGRA_RETAIN_ITEM(edgraChangeFilterName);
EDGRA_RETAIN_ITEM(edgracbClumpTerrainMenu);
EDGRA_RETAIN_ITEM(edgracbSetGlobalFadeOut);
EDGRA_RETAIN_ITEM(edgracbSetClumpMaxHeight);
EDGRA_RETAIN_ITEM(edgracbSetClumpMinHeight);
EDGRA_RETAIN_ITEM(edgracbChangeInstanceMenu);
EDGRA_RETAIN_ITEM(edgracbChangeInstanceType);
EDGRA_RETAIN_ITEM(edgracbToggleClumpTerrain);
EDGRA_RETAIN_ITEM(edgracbClumpPropertiesMenu);
EDGRA_RETAIN_ITEM(edgracbToggleClumpReactive);
EDGRA_RETAIN_ITEM(edgracbSetClumpTerrainOffset);
EDGRA_RETAIN_MENU(edgracbCancelOptMenu);
EDGRA_RETAIN_MENU(edgracbCancelSScaleMenu);
EDGRA_RETAIN_MENU(edgracbCancelGlobalsMenu);
EDGRA_RETAIN_MENU(edgracbCancelDpadModeMenu);
EDGRA_RETAIN_MENU(edgracbCancelInstanceMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpAreaMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpDistMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpFadeMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpModeMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpSizesMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpTerrainMenu);
EDGRA_RETAIN_MENU(edgracbCancelChangeInstanceMenu);
EDGRA_RETAIN_MENU(edgracbCancelClumpPropertiesMenu);
#undef EDGRA_RETAIN_ITEM
#undef EDGRA_RETAIN_MENU

static void edgracbCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelClumpAreaMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelClumpDistMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelClumpModeMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelClumpPropertiesMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelClumpSizesMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelClumpFadeMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelClumpTerrainMenu(eduimenu_s *, eduimenu_s *);
static void edgracbCancelGlobalsMenu(eduimenu_s *, eduimenu_s *);
static void edgracbSetGlobalFadeOut(eduimenu_s *, eduiitem_s *, u32);
static void edgracbSetClumpMaxHeight(eduimenu_s *, eduiitem_s *, u32);
static void edgracbSetClumpMinHeight(eduimenu_s *, eduiitem_s *, u32);
static void edgracbToggleClumpTerrain(eduimenu_s *, eduiitem_s *, u32);
static void edgracbSetClumpTerrainOffset(eduimenu_s *, eduiitem_s *, u32);
static void edgracbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static void edgracbChangeSScale(eduimenu_s *, eduiitem_s *, u32);

// Graph editor subsystem stubs (static, internal linkage).

static void edgracbFileLoad(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edgracbFileSave(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edgracbCopyClump(eduimenu_s *menu, eduiitem_s *, u32) {
    edgra_copy_source = edgra_nearest;
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edgraToggleFilter(eduimenu_s *, eduiitem_s *item, u32) {
    edgra_filter = item->flags & EDUI_ITEM_HIGHLIGHTED;
}
static void edgracbSScaleMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edgra_sscale_menu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, edgracbCancelSScaleMenu, "Super Scale");
    if (edgra_sscale_menu) {
        eduiMenuAddItem(edgra_sscale_menu, eduiItemSliderCreateInt(0, edblack, 0, edgracbChangeSScale, 1, 99,
                                                                   edgra_superscale, "Super Scale"));
        edgraAttachMenu(parent, edgra_sscale_menu);
    }
}
static void edgracbDiscardCopy(eduimenu_s *menu, eduiitem_s *, u32) {
    edgra_copy_source = -1;
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edgracbGlobalsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edgracbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32) {
    edgra_dpadmode = item->data;
}
static void edgracbChangeSScale(eduimenu_s *, eduiitem_s *item, u32) {
    edgra_superscale = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edgracbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edgra_dpadmode_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edgracbCancelDpadModeMenu, "Dpad Mode");
    if (edgra_dpadmode_menu) {
        eduiMenuAddItem(edgra_dpadmode_menu,
                        eduiItemCheckCreate(0, colours, edgra_dpadmode == 0, 1, edgracbSetDpadMode, "Clump Size"));
        eduiMenuAddItem(edgra_dpadmode_menu,
                        eduiItemCheckCreate(1, colours, edgra_dpadmode == 1, 1, edgracbSetDpadMode, "Clump Tilt"));
        edgraAttachMenu(parent, edgra_dpadmode_menu);
    }
}
static void edgracbInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edgracbSetClumpArea(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].unknown_25 = item->data;
    edgraInitAllClumps();
}
static void edgracbSetClumpDist(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].unknown_26 = item->data;
    edgraInitAllClumps();
}
static void edgracbSetClumpMode(eduimenu_s *, eduiitem_s *item, u32) {
    edgra_mode = item->data;
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].kind = item->data;
    edgraInitAllClumps();
}
static void edgracbSetClumpWind(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].field_18 = static_cast<edui_slider_s *>(item)->value;
    edgraInitAllClumps();
}
static void edgracbCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    edgra_active_menu = NULL;
    if (edgra_options_menu) {
        eduiMenuDestroy(edgra_options_menu);
        edgra_options_menu = NULL;
    }
}
static void edgracbClumpAreaMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edgra_nearest == -1 || !GrassClumps[edgra_nearest].element_count)
        return;
    edgra_clumparea_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edgracbCancelClumpAreaMenu, "Clump Area Type");
    if (!edgra_clumparea_menu)
        return;
    const char *names[] = {"Legacy", "Circle", "Square", "Rectangle"};
    for (i32 i = 1; i <= 4; ++i)
        eduiMenuAddItem(edgra_clumparea_menu,
                        eduiItemCheckCreate(i, colours, GrassClumps[edgra_nearest].unknown_25 == i, 1,
                                            edgracbSetClumpArea, const_cast<char *>(names[i - 1])));
    edgraAttachMenu(parent, edgra_clumparea_menu);
}
static void edgracbClumpDistMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edgra_nearest == -1 || !GrassClumps[edgra_nearest].element_count)
        return;
    edgra_clumpdist_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edgracbCancelClumpDistMenu, "Clump Distribution");
    if (!edgra_clumpdist_menu)
        return;
    const char *names[] = {"Legacy", "Random", "Linear", "Bell Curve"};
    for (i32 i = 1; i <= 4; ++i)
        eduiMenuAddItem(edgra_clumpdist_menu,
                        eduiItemCheckCreate(i, colours, GrassClumps[edgra_nearest].unknown_26 == i, 1,
                                            edgracbSetClumpDist, const_cast<char *>(names[i - 1])));
    edgraAttachMenu(parent, edgra_clumpdist_menu);
}
static void edgracbClumpFadeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edgracbClumpModeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edgra_clumpmode_menu = eduiMenuCreate(70, 70, 200, 250, ed_fnt, edgracbCancelClumpModeMenu, "Clump Mode");
    if (!edgra_clumpmode_menu)
        return;
    bool individual = edgra_nearest != -1 && GrassClumps[edgra_nearest].kind == 3;
    eduiMenuAddItem(edgra_clumpmode_menu,
                    individual ? eduiItemSelCreate(1, edgrey, edgra_mode == 1, 0, NULL, "Wind Mode")
                               : eduiItemCheckCreate(1, edblack, edgra_mode == 1, 1, edgracbSetClumpMode, "Wind Mode"));
    eduiMenuAddItem(
        edgra_clumpmode_menu,
        individual ? eduiItemSelCreate(2, edgrey, edgra_mode == 2, 0, NULL, "Faded Static Mode")
                   : eduiItemCheckCreate(2, edblack, edgra_mode == 2, 1, edgracbSetClumpMode, "Faded Static Mode"));
    eduiMenuAddItem(edgra_clumpmode_menu,
                    edgra_nearest == -1 || individual
                        ? eduiItemCheckCreate(3, edblack, edgra_mode == 3, 1, edgracbSetClumpMode, "Individual FS Mode")
                        : eduiItemSelCreate(3, edgrey, edgra_mode == 3, 0, NULL, "Individual FS Mode"));
    edgraAttachMenu(parent, edgra_clumpmode_menu);
}
static void edgracbClumpSizesMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edgracbSetClumpFadeIn(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1) {
        edgra_clump_s &clump = GrassClumps[edgra_nearest];
        clump.near_distance = static_cast<edui_slider_s *>(item)->value;
        if (clump.near_distance > clump.far_distance) {
            clump.far_distance = clump.near_distance;
            edui_slider_s *slider = edgra_clump_fadeout_slider;
            slider->value = clump.far_distance;
            slider->normalized_value = (slider->value - slider->minimum) / slider->range;
        }
    }
    edgraInitAllClumps();
}
static void edgracbSetClumpHeight(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].field_20 = static_cast<edui_slider_s *>(item)->value;
    edgraInitAllClumps();
}
static void edgracbApplyGlobalFade(eduimenu_s *menu, eduiitem_s *, u32) {
    for (i32 i = 0; i < edgra_clumps_used; ++i) {
        if (GrassClumps[i].element_count) {
            GrassClumps[i].near_distance = edgra_global_fadein;
            GrassClumps[i].far_distance = edgra_global_fadeout;
        }
    }
    edgraInitAllClumps();
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edgracbSetClumpFadeOut(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1) {
        edgra_clump_s &clump = GrassClumps[edgra_nearest];
        clump.far_distance = static_cast<edui_slider_s *>(item)->value;
        if (clump.near_distance > clump.far_distance) {
            clump.near_distance = clump.far_distance;
            edui_slider_s *slider = edgra_clump_fadein_slider;
            slider->value = clump.near_distance;
            slider->normalized_value = (slider->value - slider->minimum) / slider->range;
        }
    }
    edgraInitAllClumps();
}
static void edgracbSetGlobalFadeIn(eduimenu_s *, eduiitem_s *item, u32) {
    edgra_global_fadein = static_cast<edui_slider_s *>(item)->value;
    if (edgra_global_fadein > edgra_global_fadeout) {
        edgra_global_fadeout = edgra_global_fadein;
        edui_slider_s *slider = edgra_global_fadeout_slider;
        slider->value = edgra_global_fadeout;
        slider->normalized_value = (slider->value - slider->minimum) / slider->range;
    }
}
static void edgracbSetInstanceType(eduimenu_s *, eduiitem_s *item, u32) {
    edgra_instance_type = item->data;
}
static void edgracbToggleClumpTilt(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].field_43 = item->flags & EDUI_ITEM_HIGHLIGHTED;
    edgraInitAllClumps();
}
static void edgraChangeFilterName(eduimenu_s *, eduiitem_s *item, u32) {
    NuStrNCpy(edgra_filter_string, static_cast<edui_textpicker_s *>(item)->value, 16);
}

// Remaining grass-editor UI/menu callbacks.

static void edgracbCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_sscale_menu);
    edgra_sscale_menu = NULL;
}

static void edgracbClumpTerrainMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edgracbSetGlobalFadeOut(eduimenu_s *, eduiitem_s *item, u32) {
    edgra_global_fadeout = static_cast<edui_slider_s *>(item)->value;
    if (edgra_global_fadein > edgra_global_fadeout) {
        edgra_global_fadein = edgra_global_fadeout;
        edui_slider_s *slider = edgra_global_fadein_slider;
        slider->value = edgra_global_fadein;
        slider->normalized_value = (slider->value - slider->minimum) / slider->range;
    }
}

static void edgracbCancelGlobalsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_globals_menu);
    edgra_globals_menu = NULL;
}

static void edgracbSetClumpMaxHeight(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].field_30 = static_cast<edui_slider_s *>(item)->value;
    edgraInitAllClumps();
}

static void edgracbSetClumpMinHeight(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].field_2c = static_cast<edui_slider_s *>(item)->value;
    edgraInitAllClumps();
}

static void edgracbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_dpadmode_menu);
    edgra_dpadmode_menu = NULL;
}

static void edgracbCancelInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_instance_menu);
    edgra_instance_menu = NULL;
}

static void edgracbChangeInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edgracbChangeInstanceType(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1) {
        GrassClumps[edgra_nearest].special_index = item->data;
        edgraInitAllClumps();
    }
}

static void edgracbToggleClumpTerrain(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].field_42 = item->flags & EDUI_ITEM_HIGHLIGHTED;
    edgraInitAllClumps();
}

static void edgracbCancelClumpAreaMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_clumparea_menu);
    edgra_clumparea_menu = NULL;
}

static void edgracbCancelClumpDistMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_clumpdist_menu);
    edgra_clumpdist_menu = NULL;
}

static void edgracbCancelClumpFadeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_clumpfade_menu);
    edgra_clumpfade_menu = NULL;
    edgra_clump_fadein_slider = NULL;
    edgra_clump_fadeout_slider = NULL;
}

static void edgracbCancelClumpModeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_clumpmode_menu);
    edgra_clumpmode_menu = NULL;
}

static void edgracbClumpPropertiesMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edgracbToggleClumpReactive(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].flags = item->flags & EDUI_ITEM_HIGHLIGHTED;
}

static void edgracbCancelClumpSizesMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_clumpsizes_menu);
    edgra_clumpsizes_menu = NULL;
}

static void edgracbSetClumpTerrainOffset(eduimenu_s *, eduiitem_s *item, u32) {
    if (edgra_nearest != -1)
        GrassClumps[edgra_nearest].field_44 = static_cast<edui_slider_s *>(item)->value;
    edgraInitAllClumps();
}

static void edgracbCancelClumpTerrainMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_clumpterrain_menu);
    edgra_clumpterrain_menu = NULL;
}

static void edgracbCancelChangeInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_changeinstance_menu);
    edgra_changeinstance_menu = NULL;
}

static void edgracbCancelClumpPropertiesMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edgra_clumpproperties_menu);
    edgra_clumpproperties_menu = NULL;
}
