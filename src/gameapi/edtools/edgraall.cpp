#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include <stdio.h>
#include <string.h>

extern "C" void edgraInitAllClumps(void);
extern "C" void *ed_fnt;
extern "C" u32 edblack[4];
extern "C" u32 edgrey[4];
extern "C" i32 edgra_mode;
extern "C" i32 edgra_copy_source;
extern "C" char edbits_level_save_directory[256];
extern "C" char edbits_level_save_name[256];
extern "C" char edbits_level_save_extension[256];
extern "C" NUGSCN *edbits_base_scene;
extern "C" void *edbits_base_terrain;
extern "C" void edgraClumpsReset(void);
extern "C" void eduiCreateMessageMenu(eduimenu_s *, char *, i32);
i32 edgraFileSave(char *path);
void edgraClumpReseed(i32 index);
void edgraClumpDestroy(i32 index);
void edgraInstanceDestroy(i32 index);
void edgraClumpPlace(i32 index, NUVEC *position);
void edgraInstancePlace(i32 index, NUVEC *position);
i32 edgraClumpCreate(NUVEC *position);
void edgraInstanceCreate(NUVEC *position);
void edgraSortVectorBuffer(i32 index);
void edgraDetermineNearestInstance(f32 distance);

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
static i32 edgra_superscale = 64;

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
static void edgracbSetGlobalFadeIn(eduimenu_s *, eduiitem_s *, u32);
static void edgracbApplyGlobalFade(eduimenu_s *, eduiitem_s *, u32);
static void edgracbSetClumpFadeIn(eduimenu_s *, eduiitem_s *, u32);
static void edgracbSetClumpFadeOut(eduimenu_s *, eduiitem_s *, u32);
static void edgracbSetClumpHeight(eduimenu_s *, eduiitem_s *, u32);
static void edgracbSetClumpWind(eduimenu_s *, eduiitem_s *, u32);
static void edgracbToggleClumpTilt(eduimenu_s *, eduiitem_s *, u32);
static void edgracbToggleClumpReactive(eduimenu_s *, eduiitem_s *, u32);
static void edgracbClumpTerrainMenu(eduimenu_s *, eduiitem_s *, u32);
static void edgracbClumpFadeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edgracbClumpSizesMenu(eduimenu_s *, eduiitem_s *, u32);

// Graph editor subsystem stubs (static, internal linkage).

static void edgracbFileLoad(eduimenu_s *menu, eduiitem_s *, u32) {
    char path[256];
    char directory[256];
    char name[256];
    char extension[256];
    if (edbits_level_save_directory[0] == 0)
        strcpy(directory, ".");
    else
        strcpy(directory, edbits_level_save_directory);
    if (edbits_level_save_name[0] == 0)
        strcpy(name, "grass");
    else
        strcpy(name, edbits_level_save_name);
    if (edbits_level_save_extension[0] == 0)
        strcpy(extension, "gra");
    else
        strcpy(extension, edbits_level_save_extension);
    sprintf(path, "%s\\%s.%s", directory, name, extension);
    edgraClumpsReset();
    if (NuFileExists(path))
        edgraLoadPage(path, edbits_base_scene, reinterpret_cast<usize>(edbits_base_terrain), NULL, NULL);
    edgraInitAllClumps();
    eduiCreateMessageMenu(menu, "Loaded OK", 1);
}
static void edgracbFileSave(eduimenu_s *menu, eduiitem_s *, u32) {
    char path[256];
    char directory[256];
    char name[256];
    char extension[256];
    if (edbits_level_save_directory[0] == 0)
        strcpy(directory, ".");
    else
        strcpy(directory, edbits_level_save_directory);
    if (edbits_level_save_name[0] == 0)
        strcpy(name, "grass");
    else
        strcpy(name, edbits_level_save_name);
    if (edbits_level_save_extension[0] == 0)
        strcpy(extension, "gra");
    else
        strcpy(extension, edbits_level_save_extension);
    sprintf(path, "%s\\%s.%s", directory, name, extension);
    if (edgraFileSave(path))
        eduiCreateMessageMenu(menu, "Saved OK", 1);
    else
        eduiCreateMessageMenu(menu, "File Save Error", 0);
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
static void edgracbGlobalsMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edgra_globals_menu =
        eduiMenuCreate(70, 70, 220, 300, ed_fnt, edgracbCancelGlobalsMenu, "Global Options (Take Care!)");
    if (!edgra_globals_menu)
        return;
    eduiMenuAddItem(edgra_globals_menu,
                    eduiItemSliderCreate(0, edblack, 0, edgracbSetGlobalFadeIn, 0.0f, edgra_superscale * 2.0f,
                                         edgra_global_fadein, "Global Start of Fade"));
    edgra_global_fadein_slider = static_cast<edui_slider_s *>(edui_last_item);
    eduiItemSliderSetFmt(static_cast<edui_slider_s *>(edui_last_item), "(%1.01f)");
    eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(edui_last_item), 0.1f);
    eduiMenuAddItem(edgra_globals_menu,
                    eduiItemSliderCreate(0, edblack, 0, edgracbSetGlobalFadeOut, 0.0f, edgra_superscale * 2.0f,
                                         edgra_global_fadeout, "Global End of Fade"));
    edgra_global_fadeout_slider = static_cast<edui_slider_s *>(edui_last_item);
    eduiItemSliderSetFmt(static_cast<edui_slider_s *>(edui_last_item), "(%1.01f)");
    eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(edui_last_item), 0.1f);
    eduiMenuAddItem(edgra_globals_menu,
                    eduiItemSelCreate(1, edblack, 0, 0, edgracbApplyGlobalFade, "Apply Globally (Careful!)"));
    edgraAttachMenu(parent, edgra_globals_menu);
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
        eduiMenuAttach(parent, edgra_dpadmode_menu);
        edgra_dpadmode_menu->x = parent->x + 10;
        edgra_dpadmode_menu->y = parent->y + 40;
    }
}
static void edgracbInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edgra_instance_menu =
        eduiMenuCreate(70, 70, 220, 250, ed_fnt, edgracbCancelInstanceMenu,
                       const_cast<char *>(edgra_filter ? "Base Instance Select (Filtered)" : "Base Instance Select"));
    if (!edgra_instance_menu || !edbits_base_scene)
        return;
    i32 selected_found = 0;
    i32 added = 1;
    const i32 count = NuSpecialGetNumSpecials(edbits_base_scene);
    char *name = NULL;
    for (i32 i = 0; i < count; ++i) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, i);
        if (NuSpecialExistsFn(&special))
            name = NuSpecialGetName(&special);
        const i32 selected = i == edgra_instance_type;
        if (edgra_filter && NuStrNCmp(edgra_filter_string, name, NuStrLen(edgra_filter_string))) {
            if (!selected)
                continue;
            eduiMenuAddItem(edgra_instance_menu,
                            eduiItemCheckCreate(i, edblack, selected, 1, edgracbSetInstanceType, name));
            ++added;
        } else {
            eduiMenuAddItem(edgra_instance_menu,
                            eduiItemCheckCreate(i, edblack, selected, 1, edgracbSetInstanceType, name));
            ++added;
        }
        if (selected != 0) {
            selected_found = 1;
            edgra_instance_menu->selected = edui_last_item;
        }
    }
    if (added == 1)
        eduiMenuAddItem(edgra_instance_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "All Instances Filtered"));
    eduiMenuAddItemFirst(edgra_instance_menu,
                         eduiItemCheckCreate(static_cast<usize>(-1), edblack, edgra_instance_type == -1, 1,
                                             edgracbSetInstanceType, "None"));
    edgraAttachMenu(parent, edgra_instance_menu);
    if (selected_found)
        edgra_instance_menu->field_0c = edgra_instance_menu->selected;
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
    eduiMenuAddItem(edgra_clumparea_menu, eduiItemCheckCreate(1, colours, GrassClumps[edgra_nearest].unknown_25 == 1, 1,
                                                              edgracbSetClumpArea, "Legacy"));
    eduiMenuAddItem(edgra_clumparea_menu, eduiItemCheckCreate(2, colours, GrassClumps[edgra_nearest].unknown_25 == 2, 1,
                                                              edgracbSetClumpArea, "Circle"));
    eduiMenuAddItem(edgra_clumparea_menu, eduiItemCheckCreate(3, colours, GrassClumps[edgra_nearest].unknown_25 == 3, 1,
                                                              edgracbSetClumpArea, "Square"));
    eduiMenuAddItem(edgra_clumparea_menu, eduiItemCheckCreate(4, colours, GrassClumps[edgra_nearest].unknown_25 == 4, 1,
                                                              edgracbSetClumpArea, "Rectangle"));
    edgraAttachMenu(parent, edgra_clumparea_menu);
}
static void edgracbClumpDistMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edgra_nearest == -1 || !GrassClumps[edgra_nearest].element_count)
        return;
    edgra_clumpdist_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edgracbCancelClumpDistMenu, "Clump Distribution");
    if (!edgra_clumpdist_menu)
        return;
    eduiMenuAddItem(edgra_clumpdist_menu, eduiItemCheckCreate(1, colours, GrassClumps[edgra_nearest].unknown_26 == 1, 1,
                                                              edgracbSetClumpDist, "Legacy"));
    eduiMenuAddItem(edgra_clumpdist_menu, eduiItemCheckCreate(2, colours, GrassClumps[edgra_nearest].unknown_26 == 2, 1,
                                                              edgracbSetClumpDist, "Random"));
    eduiMenuAddItem(edgra_clumpdist_menu, eduiItemCheckCreate(3, colours, GrassClumps[edgra_nearest].unknown_26 == 3, 1,
                                                              edgracbSetClumpDist, "Linear"));
    eduiMenuAddItem(edgra_clumpdist_menu, eduiItemCheckCreate(4, colours, GrassClumps[edgra_nearest].unknown_26 == 4, 1,
                                                              edgracbSetClumpDist, "Bell Curve"));
    edgraAttachMenu(parent, edgra_clumpdist_menu);
}
static void edgracbClumpFadeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edgra_nearest == -1 || !GrassClumps[edgra_nearest].element_count)
        return;
    edgra_clumpfade_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edgracbCancelClumpFadeMenu, "Clump Fading");
    if (!edgra_clumpfade_menu)
        return;
    eduiMenuAddItem(edgra_clumpfade_menu,
                    eduiItemSliderCreate(0, edblack, 0, edgracbSetClumpFadeIn, 0.0f, edgra_superscale * 2.0f,
                                         GrassClumps[edgra_nearest].near_distance, "Start of Fade"));
    edgra_clump_fadein_slider = static_cast<edui_slider_s *>(edui_last_item);
    eduiItemSliderSetFmt(static_cast<edui_slider_s *>(edui_last_item), "(%1.01f)");
    eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(edui_last_item), 0.1f);
    eduiMenuAddItem(edgra_clumpfade_menu,
                    eduiItemSliderCreate(0, edblack, 0, edgracbSetClumpFadeOut, 0.0f, edgra_superscale * 2.0f,
                                         GrassClumps[edgra_nearest].far_distance, "End of Fade"));
    edgra_clump_fadeout_slider = static_cast<edui_slider_s *>(edui_last_item);
    eduiItemSliderSetFmt(static_cast<edui_slider_s *>(edui_last_item), "(%1.01f)");
    eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(edui_last_item), 0.1f);
    edgraAttachMenu(parent, edgra_clumpfade_menu);
}
static void edgracbClumpModeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edgra_clumpmode_menu = eduiMenuCreate(70, 70, 200, 250, ed_fnt, edgracbCancelClumpModeMenu, "Clump Mode");
    if (!edgra_clumpmode_menu)
        return;
    if (edgra_nearest != -1 && GrassClumps[edgra_nearest].kind == 3) {
        eduiMenuAddItem(edgra_clumpmode_menu, eduiItemSelCreate(1, edgrey, edgra_mode == 1, 0, NULL, "Wind Mode"));
        eduiMenuAddItem(edgra_clumpmode_menu,
                        eduiItemSelCreate(2, edgrey, edgra_mode == 2, 0, NULL, "Faded Static Mode"));
    } else {
        eduiMenuAddItem(edgra_clumpmode_menu,
                        eduiItemCheckCreate(1, edblack, edgra_mode == 1, 1, edgracbSetClumpMode, "Wind Mode"));
        eduiMenuAddItem(edgra_clumpmode_menu,
                        eduiItemCheckCreate(2, edblack, edgra_mode == 2, 1, edgracbSetClumpMode, "Faded Static Mode"));
    }
    if (edgra_nearest != -1 && GrassClumps[edgra_nearest].kind != 3)
        eduiMenuAddItem(edgra_clumpmode_menu,
                        eduiItemSelCreate(3, edgrey, edgra_mode == 3, 0, NULL, "Individual FS Mode"));
    else
        eduiMenuAddItem(edgra_clumpmode_menu,
                        eduiItemCheckCreate(3, edblack, edgra_mode == 3, 1, edgracbSetClumpMode, "Individual FS Mode"));
    edgraAttachMenu(parent, edgra_clumpmode_menu);
}
static void edgracbClumpSizesMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edgra_nearest == -1 || !GrassClumps[edgra_nearest].element_count)
        return;
    edgra_clumpsizes_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edgracbCancelClumpSizesMenu, "Clump Sizes");
    if (!edgra_clumpsizes_menu)
        return;
    eduiMenuAddItem(edgra_clumpsizes_menu, eduiItemSliderCreate(0, colours, 0, edgracbSetClumpMinHeight, 0.0f, 5.0f,
                                                                GrassClumps[edgra_nearest].field_2c, "Min Model Size"));
    eduiMenuAddItem(edgra_clumpsizes_menu, eduiItemSliderCreate(0, colours, 0, edgracbSetClumpMaxHeight, 0.0f, 5.0f,
                                                                GrassClumps[edgra_nearest].field_30, "Max Model Size"));
    if (GrassClumps[edgra_nearest].kind == 1)
        eduiMenuAddItem(edgra_clumpsizes_menu,
                        eduiItemSliderCreate(0, colours, 0, edgracbSetClumpHeight, 0.1f, 9.9f,
                                             GrassClumps[edgra_nearest].field_20, "Bend Height"));
    edgraAttachMenu(parent, edgra_clumpsizes_menu);
}
static void edgracbSetClumpFadeIn(eduimenu_s *, eduiitem_s *item, u32) {
    edui_slider_s *slider = edgra_clump_fadeout_slider;
    if (edgra_nearest != -1) {
        edgra_clump_s &clump = GrassClumps[edgra_nearest];
        clump.near_distance = static_cast<edui_slider_s *>(item)->value;
        if (clump.near_distance > clump.far_distance) {
            clump.far_distance = clump.near_distance;
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
    for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
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
    edui_slider_s *slider = edgra_clump_fadein_slider;
    if (edgra_nearest != -1) {
        edgra_clump_s &clump = GrassClumps[edgra_nearest];
        clump.far_distance = static_cast<edui_slider_s *>(item)->value;
        if (clump.near_distance > clump.far_distance) {
            clump.near_distance = clump.far_distance;
            slider->value = clump.near_distance;
            slider->normalized_value = (slider->value - slider->minimum) / slider->range;
        }
    }
    edgraInitAllClumps();
}
static void edgracbSetGlobalFadeIn(eduimenu_s *, eduiitem_s *item, u32) {
    edui_slider_s *slider = edgra_global_fadeout_slider;
    edgra_global_fadein = static_cast<edui_slider_s *>(item)->value;
    if (edgra_global_fadein > edgra_global_fadeout) {
        edgra_global_fadeout = edgra_global_fadein;
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

static void edgracbClumpTerrainMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edgra_nearest == -1 || !GrassClumps[edgra_nearest].element_count)
        return;
    edgra_clumpterrain_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edgracbCancelClumpTerrainMenu, "Clump Terraining");
    if (!edgra_clumpterrain_menu)
        return;
    eduiMenuAddItem(edgra_clumpterrain_menu,
                    eduiItemToggleCreate(0, edblack, static_cast<i8>(GrassClumps[edgra_nearest].field_42), 1,
                                         edgracbToggleClumpTerrain, "Clump Terraining"));
    if (GrassClumps[edgra_nearest].kind != 1)
        eduiMenuAddItem(edgra_clumpterrain_menu,
                        eduiItemToggleCreate(0, edblack, static_cast<i8>(GrassClumps[edgra_nearest].field_43), 2,
                                             edgracbToggleClumpTilt, "Clump Tilting"));
    eduiMenuAddItem(edgra_clumpterrain_menu,
                    eduiItemSliderCreate(0, edblack, 0, edgracbSetClumpTerrainOffset, -1.0f, 2.0f,
                                         GrassClumps[edgra_nearest].field_44, "Terraining Offset"));
    edgraAttachMenu(parent, edgra_clumpterrain_menu);
}

static void edgracbSetGlobalFadeOut(eduimenu_s *, eduiitem_s *item, u32) {
    edui_slider_s *slider = edgra_global_fadein_slider;
    edgra_global_fadeout = static_cast<edui_slider_s *>(item)->value;
    if (edgra_global_fadein > edgra_global_fadeout) {
        edgra_global_fadein = edgra_global_fadeout;
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

static void edgracbChangeInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edgra_changeinstance_menu =
        eduiMenuCreate(70, 70, 220, 250, ed_fnt, edgracbCancelChangeInstanceMenu,
                       const_cast<char *>(edgra_filter ? "Change Instance (Filtered)" : "Change Instance"));
    if (!edgra_changeinstance_menu || !edbits_base_scene)
        return;
    i32 selected_found = 0;
    i32 added = 1;
    const i32 count = NuSpecialGetNumSpecials(edbits_base_scene);
    char *name = NULL;
    for (i32 i = 0; i < count; ++i) {
        nuhspecial_s special;
        const i32 selected = i == GrassClumps[edgra_nearest].special_index;
        NuGScnGetSpecial(&special, edbits_base_scene, i);
        if (NuSpecialExistsFn(&special))
            name = NuSpecialGetName(&special);
        if (edgra_filter && NuStrNCmp(edgra_filter_string, name, NuStrLen(edgra_filter_string))) {
            if (!selected)
                continue;
            eduiMenuAddItem(edgra_changeinstance_menu,
                            eduiItemCheckCreate(i, edblack, selected, 1, edgracbChangeInstanceType, name));
            ++added;
        } else {
            eduiMenuAddItem(edgra_changeinstance_menu,
                            eduiItemCheckCreate(i, edblack, selected, 1, edgracbChangeInstanceType, name));
            ++added;
        }
        if (selected) {
            selected_found = 1;
            edgra_changeinstance_menu->selected = edui_last_item;
        }
    }
    if (added == 1)
        eduiMenuAddItem(edgra_changeinstance_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "All Instances Filtered"));
    eduiMenuAttach(parent, edgra_changeinstance_menu);
    edgra_changeinstance_menu->x = parent->x + 10;
    edgra_changeinstance_menu->y = parent->y + 40;
    if (selected_found)
        edgra_changeinstance_menu->field_0c = edgra_changeinstance_menu->selected;
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

static void edgracbClumpPropertiesMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edgra_nearest == -1 || !GrassClumps[edgra_nearest].element_count)
        return;
    edgra_clumpproperties_menu =
        eduiMenuCreate(70, 70, 220, 250, ed_fnt, edgracbCancelClumpPropertiesMenu, "Clump Properties");
    if (!edgra_clumpproperties_menu)
        return;
    eduiMenuAddItem(edgra_clumpproperties_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edgracbClumpSizesMenu, "Clump Sizes..."));
    if (GrassClumps[edgra_nearest].kind == 3) {
        eduiMenuAddItem(edgra_clumpproperties_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Clump Area Type..."));
        eduiMenuAddItem(edgra_clumpproperties_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Clump Distribution..."));
    } else {
        eduiMenuAddItem(edgra_clumpproperties_menu,
                        eduiItemSelCreate(1, colours, 0, 0, edgracbClumpAreaMenu, "Clump Area Type..."));
        eduiMenuAddItem(edgra_clumpproperties_menu,
                        eduiItemSelCreate(1, colours, 0, 0, edgracbClumpDistMenu, "Clump Distribution..."));
    }
    eduiMenuAddItem(edgra_clumpproperties_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edgracbClumpFadeMenu, "Clump Fading..."));
    eduiMenuAddItem(edgra_clumpproperties_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edgracbClumpTerrainMenu, "Clump Terraining..."));
    if (GrassClumps[edgra_nearest].kind == 1)
        eduiMenuAddItem(edgra_clumpproperties_menu,
                        eduiItemSliderCreate(0, colours, 0, edgracbSetClumpWind, 0.01f, 1.99f,
                                             GrassClumps[edgra_nearest].field_18, "Wind Effect"));
    eduiMenuAddItem(edgra_clumpproperties_menu,
                    eduiItemToggleCreate(0, colours, GrassClumps[edgra_nearest].flags, 1, edgracbToggleClumpReactive,
                                         "Collide with Player"));
    edgraAttachMenu(parent, edgra_clumpproperties_menu);
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

void edgraDoInput(nupad_s *pad) {
    if ((pad->digital_buttons & 0x100) == 0) {
        edcamMove(pad);
    }

    if ((pad->digital_buttons & 0x100) != 0) {
        const u32 pressed = pad->digital_buttons_pressed;
        if ((pressed & 0x20) && edgra_nearest != -1)
            edgraClumpReseed(edgra_nearest);

        if ((pressed & 0x40) && edgra_nearest != -1) {
            if (edgra_editormode == 1) {
                edgra_editormode = 0;
            } else if (edgra_mode == 3) {
                edgra_editormode = 1;
            }
        }

        if (edgra_editormode == 1) {
            if (edgra_nearest_instance == -1) {
                edgraDetermineNearestInstance(-1.0f);
            } else if (edgra_nearest != -1) {
                const i32 count = GrassClumps[edgra_nearest].element_count;
                if (pressed & 8) {
                    ++edgra_nearest_instance;
                    if (edgra_nearest_instance == count)
                        edgra_nearest_instance = 0;
                }
                if (pressed & 2) {
                    --edgra_nearest_instance;
                    if (edgra_nearest_instance == -1)
                        edgra_nearest_instance = count - 1;
                }
            }
        } else {
            if (edgra_nearest == -1) {
                edgraDetermineNearestClump(-1.0f);
            } else {
                if (pressed & 8) {
                    do {
                        ++edgra_nearest;
                        if (edgra_nearest == EDGRA_MAX_CLUMPS)
                            edgra_nearest = 0;
                    } while (!GrassClumps[edgra_nearest].element_count);
                    edgraSortVectorBuffer(edgra_nearest);
                }
                if (pressed & 2) {
                    do {
                        --edgra_nearest;
                        if (edgra_nearest == -1)
                            edgra_nearest = EDGRA_MAX_CLUMPS - 1;
                    } while (!GrassClumps[edgra_nearest].element_count);
                    edgraSortVectorBuffer(edgra_nearest);
                }
            }
        }

        if (edgra_editormode == 1) {
            if (edgra_nearest != -1 && edgra_nearest_instance != -1) {
                edgra_clump_s &clump = GrassClumps[edgra_nearest];
                NUVEC position;
                NuVecAdd(&position, &clump.position,
                         &GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->position);
                edcamSetPos(&position);
                edgra_rotz = GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_10;
                edgra_roty = GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_12;
            }
        } else if (edgra_nearest != -1) {
            edcamSetPos(&GrassClumps[edgra_nearest].position);
            const i32 kind = static_cast<i8>(GrassClumps[edgra_nearest].kind);
            edgra_mode = kind;
            edgra_instance_type = GrassClumps[edgra_nearest].special_index;
            if (kind != 3) {
                edgra_size = GrassClumps[edgra_nearest].size;
                edgra_clump_size = GrassClumps[edgra_nearest].element_count;
                edgra_rotz = GrassClumps[edgra_nearest].rotation_z;
                edgra_roty = GrassClumps[edgra_nearest].rotation_y;
            }
        }
    }

    edcamGetPosAng(&edgra_cam_pos, &edgra_cam_ax, &edgra_cam_ay);
    if ((pad->digital_buttons & 0x100) == 0) {
        const u32 pressed = pad->digital_buttons_pressed;
        if (pressed & 0x80) {
            edgra_options_menu = eduiMenuCreate(70, 70, 220, 300, ed_fnt, edgracbCancelOptMenu, "Options");
            if (edgra_options_menu) {
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edgracbInstanceMenu, "Base Instance Select..."));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edgra_nearest == -1 ? edgrey : edblack, 0, 0,
                                                  edgra_nearest == -1 ? NULL : edgracbChangeInstanceMenu,
                                                  "Change Instance..."));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edgracbClumpModeMenu, "Clump Mode..."));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edgra_nearest == -1 ? edgrey : edblack, 0, 0,
                                                  edgra_nearest == -1 ? NULL : edgracbClumpPropertiesMenu,
                                                  "Clump Properties..."));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edgracbDpadModeMenu, "Dpad Mode..."));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edgra_nearest == -1 ? edgrey : edblack, 0, 0,
                                                  edgra_nearest == -1 ? NULL : edgracbCopyClump, "Copy Clump"));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edgra_copy_source == -1 ? edgrey : edblack, 0, 0,
                                                  edgra_copy_source == -1 ? NULL : edgracbDiscardCopy, "Discard Copy"));
                eduiMenuAddItem(edgra_options_menu, eduiItemSelCreate(1, edblack, 0, 0, edgracbFileSave, "Save Grass"));
                eduiMenuAddItem(edgra_options_menu, eduiItemSelCreate(1, edblack, 0, 0, edgracbFileLoad, "Load Grass"));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edgracbSScaleMenu, "Super Scale..."));
                eduiMenuAddItem(edgra_options_menu, eduiItemToggleCreate(1, edblack, edgra_filter, 1, edgraToggleFilter,
                                                                         "Instance Filter"));
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemTextPickCreate(0, edblack, edgraChangeFilterName, "Filter String: "));
                edui_textpicker_s *filter = static_cast<edui_textpicker_s *>(edui_last_item);
                strcpy(filter->value, edgra_filter_string);
                filter->max_length = 15;
                eduiMenuAddItem(edgra_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edgracbGlobalsMenu, "Global Options..."));
            }
            edgra_active_menu = edgra_options_menu;
        }
        if (pressed & 0x40) {
            if (edgra_editormode == 1)
                edgraInstanceCreate(&edgra_cam_pos);
            else if (edgra_instance_type != -1)
                edgraClumpCreate(&edgra_cam_pos);
        }
        if (pad->digital_buttons & 0x20) {
            if (edgra_editormode == 1) {
                if (edgra_nearest_instance != -1)
                    edgraInstancePlace(edgra_nearest_instance, &edgra_cam_pos);
            } else if (edgra_nearest != -1) {
                edgraClumpPlace(edgra_nearest, &edgra_cam_pos);
            }
        }
        if (pressed & 0x10) {
            if (edgra_editormode == 1) {
                if (edgra_nearest_instance != -1)
                    edgraInstanceDestroy(edgra_nearest_instance);
                edgra_nearest_instance = -1;
            } else {
                if (edgra_nearest != -1)
                    edgraClumpDestroy(edgra_nearest);
                edgra_nearest = -1;
            }
        }
    }

    if (edgra_dpadmode == 0) {
        if (edgra_mode == 3) {
            if (edgra_nearest != -1 && edgra_nearest_instance != -1) {
                edgra_clump_s &clump = GrassClumps[edgra_nearest];
                GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_0c +=
                    static_cast<f32>(pad->analog_left_pad_up) / 5000.0f;
                GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_0c -=
                    static_cast<f32>(pad->analog_left_pad_down) / 5000.0f;
                if (GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_0c > 1.0f)
                    GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_0c = 1.0f;
                if (GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_0c < 0.1f)
                    GetIndGrassClump(clump.individual_index, edgra_nearest_instance)->field_0c = 0.1f;
            }
            if (pad->analog_left_pad_up || pad->analog_left_pad_down)
                edgraInitAllClumps();
        } else {
            edgra_size += static_cast<f32>(pad->analog_left_pad_up) / 5000.0f;
            edgra_size -= static_cast<f32>(pad->analog_left_pad_down) / 5000.0f;
            if (edgra_size < 0.1f)
                edgra_size = 0.1f;
            if (edgra_size > 10.0f)
                edgra_size = 10.0f;
            if (pad->analog_left_pad_left == 255 || (pad->digital_buttons_pressed & 0x8000))
                --edgra_clump_size;
            if (pad->analog_left_pad_right == 255 || (pad->digital_buttons_pressed & 0x2000))
                ++edgra_clump_size;
            if (edgra_clump_size < 4)
                edgra_clump_size = 4;
            if (edgra_clump_size > 256)
                edgra_clump_size = 256;
        }
    } else if (edgra_dpadmode == 1) {
        edgra_roty += pad->analog_left_pad_right - pad->analog_left_pad_left;
        edgra_rotz += pad->analog_left_pad_up;
        if (edgra_rotz > 32768)
            edgra_rotz = 32768;
        edgra_rotz -= pad->analog_left_pad_down;
        if (edgra_rotz < -32768)
            edgra_rotz = -32768;
    }
}
