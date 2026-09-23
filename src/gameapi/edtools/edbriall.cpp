#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include <stdio.h>
#include <string.h>

extern "C" {
    extern eduimenu_s *edbri_active_menu, *edbri_options_menu, *edbri_dpadmode_menu;
    extern eduimenu_s *edbri_plankinstance_menu, *edbri_postinstance_menu;
    extern eduimenu_s *edbri_plankcount_menu, *edbri_bridgeproperties_menu, *edbri_ropecolour_menu;
    extern i32 edbri_mode, edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    extern i32 edbri_planks, edbri_post_interval, edbri_bridges_used;
    extern NUGSCN *edbri_page_scene[8];
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern NUVEC edbri_cam_pos;
    extern i32 edbri_cam_ax, edbri_cam_ay, edbri_rotz, edbri_roty;
    extern f32 edbri_length, edbri_width;
    extern char edbits_level_save_directory[256], edbits_level_save_name[256], edbits_level_save_extension[256];
    extern NUGSCN *edbits_base_scene;
    void eduiCreateMessageMenu(eduimenu_s *, char *, i32);
    i32 edbitsLookupInstance(char *, NUGSCN *);
    void edbriBridgesReset(void);
    i32 edbriLoadPage(char *, void *);
    eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
    void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
}
void edbriBridgeUpdate(i32, NUGSCN *);
void edbriBridgePlace(i32, NUVEC *);
void edbriBridgeDestroy(i32);
i32 edbriBridgeCreate(NUVEC *);
void edbriDetermineNearest(f32);
i32 edbriFileSave(char *);

static void edbricbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static void edbricbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);
static void edbricbCancelPlankCountMenu(eduimenu_s *, eduimenu_s *);
static void edbricbCancelPostInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edbricbCancelPlankInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edbricbCancelBridgePropertiesMenu(eduimenu_s *, eduimenu_s *);
static void edbricbCancelRopeColourMenu(eduimenu_s *, eduimenu_s *);
static void edbricbSetPostInstanceType(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetPlankInstanceType(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetPlankCount(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetPostInterval(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetDpadMode(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetBridgeTension(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetBridgeDamp(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetBridgeGravity(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetBridgePlrweight(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetBridgeRopeheight(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetBridgeStability(eduimenu_s *, eduiitem_s *, u32);
static void edbricbSetRopeColour(eduimenu_s *, eduiitem_s *, u32);
static void edbricbRopeColourMenu(eduimenu_s *, eduiitem_s *, u32);

#define EDBRI_USED_ITEM(name) static void name(eduimenu_s *, eduiitem_s *, u32) __attribute__((used))
#define EDBRI_USED_MENU(name) static void name(eduimenu_s *, eduimenu_s *) __attribute__((used))
EDBRI_USED_ITEM(edbricbFileLoad);
EDBRI_USED_ITEM(edbricbFileSave);
EDBRI_USED_ITEM(edbricbDpadModeMenu);
EDBRI_USED_ITEM(edbricbPlankCountMenu);
EDBRI_USED_ITEM(edbricbPostInstanceMenu);
EDBRI_USED_ITEM(edbricbPlankInstanceMenu);
EDBRI_USED_ITEM(edbricbBridgePropertiesMenu);
EDBRI_USED_MENU(edbricbCancelOptMenu);
#undef EDBRI_USED_ITEM
#undef EDBRI_USED_MENU

static void edbriAttachMenu(eduimenu_s *parent, eduimenu_s *child) {
    eduiMenuAttach(parent, child);
    child->x = parent->x + 10;
    child->y = parent->y + 40;
}

static void edbricbSetPostInstanceType(eduimenu_s *, eduiitem_s *item, u32) {
    edbri_post_instance_type = item->data;
}
static void edbricbSetPlankInstanceType(eduimenu_s *, eduiitem_s *item, u32) {
    edbri_plank_instance_type = item->data;
}
static void edbricbSetPlankCount(eduimenu_s *, eduiitem_s *item, u32) {
    edbri_planks = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edbricbSetPostInterval(eduimenu_s *, eduiitem_s *item, u32) {
    edbri_post_interval = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edbricbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32) {
    edbri_mode = item->data;
}

static void edbricbCancelRopeColourMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edbri_ropecolour_menu);
    edbri_ropecolour_menu = NULL;
}
static void edbricbCancelBridgePropertiesMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edbri_bridgeproperties_menu);
    edbri_bridgeproperties_menu = NULL;
}
static void edbricbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edbri_dpadmode_menu);
    edbri_dpadmode_menu = NULL;
}
static void edbricbCancelPlankCountMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edbri_plankcount_menu);
    edbri_plankcount_menu = NULL;
}
static void edbricbCancelPostInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edbri_postinstance_menu);
    edbri_postinstance_menu = NULL;
}
static void edbricbCancelPlankInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edbri_plankinstance_menu);
    edbri_plankinstance_menu = NULL;
}
static void edbricbCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    edbri_active_menu = NULL;
    if (edbri_options_menu) {
        eduiMenuDestroy(edbri_options_menu);
        edbri_options_menu = NULL;
    }
}

static void edbricbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edbri_dpadmode_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edbricbCancelDpadModeMenu, "Dpad Mode");
    if (!edbri_dpadmode_menu)
        return;
    eduiMenuAddItem(edbri_dpadmode_menu,
                    eduiItemCheckCreate(0, edblack, edbri_mode == 0, 1, edbricbSetDpadMode, "Bridge Orient"));
    eduiMenuAddItem(edbri_dpadmode_menu,
                    eduiItemCheckCreate(1, edblack, edbri_mode == 1, 1, edbricbSetDpadMode, "Bridge Size"));
    edbriAttachMenu(parent, edbri_dpadmode_menu);
}
static void edbricbPlankCountMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edbri_plankcount_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edbricbCancelPlankCountMenu, "Plank Count");
    if (!edbri_plankcount_menu)
        return;
    eduiMenuAddItem(edbri_plankcount_menu,
                    eduiItemSliderCreateInt(0, edblack, 0, edbricbSetPlankCount, 1, 23, edbri_planks, "Plank Count"));
    eduiMenuAddItem(edbri_plankcount_menu, eduiItemSliderCreateInt(0, edblack, 0, edbricbSetPostInterval, 1, 23,
                                                                   edbri_post_interval, "Post Interval"));
    edbriAttachMenu(parent, edbri_plankcount_menu);
}

static void edbricbPostInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edbri_postinstance_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edbricbCancelPostInstanceMenu, "Post Select");
    if (!edbri_postinstance_menu || !edbits_base_scene)
        return;
    eduiMenuAddItem(edbri_postinstance_menu,
                    eduiItemCheckCreate(static_cast<usize>(-1), edblack, edbri_post_instance_type == -1, 1,
                                        edbricbSetPostInstanceType, "None"));
    const i32 count = NuSpecialGetNumSpecials(edbits_base_scene);
    for (i32 i = 0; i < count; ++i) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, i);
        eduiMenuAddItem(edbri_postinstance_menu,
                        eduiItemCheckCreate(i, edblack, edbri_post_instance_type == i, 1, edbricbSetPostInstanceType,
                                            NuSpecialGetName(&special)));
        if (edbri_post_instance_type == i)
            edbri_postinstance_menu->selected = edui_last_item;
    }
    edbriAttachMenu(parent, edbri_postinstance_menu);
}
static void edbricbPlankInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edbri_plankinstance_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edbricbCancelPlankInstanceMenu, "Plank Select");
    if (!edbri_plankinstance_menu || !edbits_base_scene)
        return;
    eduiMenuAddItem(edbri_plankinstance_menu,
                    eduiItemCheckCreate(static_cast<usize>(-1), edblack, edbri_plank_instance_type == -1, 1,
                                        edbricbSetPlankInstanceType, "None"));
    const i32 count = NuSpecialGetNumSpecials(edbits_base_scene);
    for (i32 i = 0; i < count; ++i) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, i);
        eduiMenuAddItem(edbri_plankinstance_menu,
                        eduiItemCheckCreate(i, edblack, edbri_plank_instance_type == i, 1, edbricbSetPlankInstanceType,
                                            NuSpecialGetName(&special)));
        if (edbri_plank_instance_type == i)
            edbri_plankinstance_menu->selected = edui_last_item;
    }
    edbriAttachMenu(parent, edbri_plankinstance_menu);
}

static void edbricbSetBridgeTension(eduimenu_s *, eduiitem_s *item, u32) {
    edBridges[edbri_nearest].field_28 = static_cast<edui_slider_s *>(item)->value;
    edbriBridgeUpdate(edbri_nearest, edbits_base_scene);
}
static void edbricbSetBridgeDamp(eduimenu_s *, eduiitem_s *item, u32) {
    edBridges[edbri_nearest].field_2c = static_cast<edui_slider_s *>(item)->value;
    edbriBridgeUpdate(edbri_nearest, edbits_base_scene);
}
static void edbricbSetBridgeGravity(eduimenu_s *, eduiitem_s *item, u32) {
    edBridges[edbri_nearest].field_30 = static_cast<edui_slider_s *>(item)->value;
    edbriBridgeUpdate(edbri_nearest, edbits_base_scene);
}
static void edbricbSetBridgePlrweight(eduimenu_s *, eduiitem_s *item, u32) {
    edBridges[edbri_nearest].field_34 = static_cast<edui_slider_s *>(item)->value;
    edbriBridgeUpdate(edbri_nearest, edbits_base_scene);
}
static void edbricbSetBridgeRopeheight(eduimenu_s *, eduiitem_s *item, u32) {
    edBridges[edbri_nearest].field_38 = static_cast<edui_slider_s *>(item)->value;
    edbriBridgeUpdate(edbri_nearest, edbits_base_scene);
}
static void edbricbSetBridgeStability(eduimenu_s *, eduiitem_s *item, u32) {
    edBridges[edbri_nearest].field_3c = static_cast<edui_slider_s *>(item)->value;
    edbriBridgeUpdate(edbri_nearest, edbits_base_scene);
}
static void edbricbSetRopeColour(eduimenu_s *menu, eduiitem_s *item, u32) {
    edui_colour_pick_s *colour = static_cast<edui_colour_pick_s *>(item);
    edbridge_s &bridge = edBridges[edbri_nearest];
    bridge.red = static_cast<u8>(static_cast<i32>(colour->red * 255.0f));
    bridge.green = static_cast<u8>(static_cast<i32>(colour->green * 255.0f));
    bridge.blue = static_cast<u8>(static_cast<i32>(colour->blue * 255.0f));
    edbriBridgeUpdate(edbri_nearest, edbits_base_scene);
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edbricbRopeColourMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edbri_ropecolour_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edbricbCancelRopeColourMenu, "Rope Colour");
    if (!edbri_ropecolour_menu)
        return;
    eduiMenuAddItem(edbri_ropecolour_menu, eduiItemColourPickCreate(0, edblack, edbricbSetRopeColour, "Rope Colour"));
    edbridge_s &bridge = edBridges[edbri_nearest];
    eduiItemColourPickSetRGB(static_cast<edui_colour_pick_s *>(edui_last_item), bridge.red / 255.0f,
                             bridge.green / 255.0f, bridge.blue / 255.0f);
    edbriAttachMenu(parent, edbri_ropecolour_menu);
}
static void edbricbBridgePropertiesMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edbri_nearest == -1)
        return;
    edbri_bridgeproperties_menu =
        eduiMenuCreate(70, 70, 180, 250, ed_fnt, edbricbCancelBridgePropertiesMenu, "Bridge Properties");
    if (!edbri_bridgeproperties_menu)
        return;
    edbridge_s &bridge = edBridges[edbri_nearest];
    eduiMenuAddItem(edbri_bridgeproperties_menu, eduiItemSliderCreate(0, edblack, 0, edbricbSetBridgeTension, 0.0f,
                                                                      1.0f, bridge.field_28, "Tension"));
    eduiMenuAddItem(edbri_bridgeproperties_menu,
                    eduiItemSliderCreate(0, edblack, 0, edbricbSetBridgeDamp, 0.0f, 1.0f, bridge.field_2c, "Damp"));
    eduiMenuAddItem(edbri_bridgeproperties_menu, eduiItemSliderCreate(0, edblack, 0, edbricbSetBridgeGravity, -0.1f,
                                                                      0.2f, bridge.field_30, "Gravity"));
    eduiMenuAddItem(edbri_bridgeproperties_menu, eduiItemSliderCreate(0, edblack, 0, edbricbSetBridgePlrweight, 0.0f,
                                                                      10.0f, bridge.field_34, "Plrweight"));
    eduiMenuAddItem(edbri_bridgeproperties_menu, eduiItemSliderCreate(0, edblack, 0, edbricbSetBridgeRopeheight, 0.0f,
                                                                      2.0f, bridge.field_38, "Rope Height"));
    eduiMenuAddItem(edbri_bridgeproperties_menu, eduiItemSliderCreate(0, edblack, 0, edbricbSetBridgeStability, 0.0f,
                                                                      10.0f, bridge.field_3c, "Stability"));
    eduiMenuAddItem(edbri_bridgeproperties_menu,
                    eduiItemSelCreate(1, edblack, 0, 0, edbricbRopeColourMenu, "Rope Colour..."));
    edbriAttachMenu(parent, edbri_bridgeproperties_menu);
}

static void edbriMakePath(char *path) {
    char directory[256], name[256], extension[256];
    if (!edbits_level_save_directory[0])
        strcpy(directory, ".");
    else
        strcpy(directory, edbits_level_save_directory);
    if (!edbits_level_save_name[0])
        strcpy(name, "bridge");
    else
        strcpy(name, edbits_level_save_name);
    if (edbits_level_save_extension[0])
        strcpy(extension, edbits_level_save_extension);
    else
        strcpy(extension, "bri");
    sprintf(path, "%s\\%s.%s", directory, name, extension);
}
static void edbricbFileSave(eduimenu_s *menu, eduiitem_s *, u32) {
    char path[256];
    edbriMakePath(path);
    if (edbriFileSave(path))
        eduiCreateMessageMenu(menu, "Saved OK", 1);
    else
        eduiCreateMessageMenu(menu, "File Save Error", 0);
}
static void edbricbFileLoad(eduimenu_s *menu, eduiitem_s *, u32) {
    char path[256];
    edbriMakePath(path);
    edbriBridgesReset();
    if (NuFileExists(path))
        edbriLoadPage(path, edbits_base_scene);
    edbriStartAllPages();
    edbriDetermineNearest(1.0f);
    eduiCreateMessageMenu(menu, "Loaded OK", 1);
}

extern "C" i32 edbriLoadPage(char *path, void *gscn) {
    i32 page = 0;
    while (page < 8 && edbri_page_used[page])
        ++page;
    if (page == 8)
        return -1;
    EdFileSetMedia(1);
    if (!EdFileOpen(path, NUFILE_READ))
        return -1;
    const i32 version = EdFileReadInt();
    if (version >= 2) {
        EdFileClose();
        return -1;
    }
    i32 count = EdFileReadInt();
    if (count + edbri_bridges_used > 64)
        count = 64 - edbri_bridges_used;
    i32 index = 0;
    i32 i = 0;
    while (i < count) {
        while (index < 64 && edBridges[index].connection_index != 0xff)
            ++index;
        if (index >= 64) {
            ++i;
            continue;
        }
        edbridge_s &bridge = edBridges[index];
        bridge.instance_id = -1;
        EdFileReadNuVec(&bridge.position);
        bridge.length = EdFileReadFloat();
        bridge.field_14 = EdFileReadFloat();
        bridge.rotation_z = EdFileReadShort();
        bridge.rotation_y = EdFileReadShort();
        bridge.connection_index = page;
        bridge.field_1d = EdFileReadChar();
        bridge.field_1e = EdFileReadChar();
        char name[20];
        EdFileRead(name, 20);
        bridge.special_20 = name[0] ? edbitsLookupInstance(name, static_cast<NUGSCN *>(gscn)) : -1;
        EdFileRead(name, 20);
        bridge.special_24 = name[0] ? edbitsLookupInstance(name, static_cast<NUGSCN *>(gscn)) : -1;
        bridge.field_28 = EdFileReadFloat();
        bridge.field_2c = EdFileReadFloat();
        bridge.field_30 = EdFileReadFloat();
        bridge.field_34 = EdFileReadFloat();
        bridge.field_38 = EdFileReadFloat();
        bridge.field_3c = EdFileReadFloat();
        bridge.red = EdFileReadChar();
        bridge.green = EdFileReadChar();
        bridge.blue = EdFileReadChar();
        bridge.field_43 = EdFileReadChar();
        ++edbri_bridges_used;
        ++i;
    }
    EdFileClose();
    edbri_nearest = -1;
    edbriDetermineNearest(1.0f);
    edbri_page_used[page] = 1;
    edbri_page_scene[page] = static_cast<NUGSCN *>(gscn);
    return page;
}

void edbriDoInput(nupad_s *pad) {
    bool camera_locked = (pad->digital_buttons & 0x100) != 0;
    if (!camera_locked) {
        edcamMove(pad);
        camera_locked = (pad->digital_buttons & 0x100) != 0;
    }
    if (camera_locked) {
        if (edbri_nearest == -1) {
            edbriDetermineNearest(-1.0f);
        } else {
            i32 index = edbri_nearest;
            if (pad->digital_buttons_pressed & 8) {
                do {
                    ++index;
                    if (index == 64)
                        index = 0;
                } while (edBridges[index].instance_id == -1);
                edbri_nearest = index;
            }
            if (pad->digital_buttons_pressed & 2) {
                do {
                    --index;
                    if (index == -1)
                        index = 63;
                } while (edBridges[index].instance_id == -1);
                edbri_nearest = index;
            }
        }
        if (edbri_nearest != -1) {
            edbridge_s &bridge = edBridges[edbri_nearest];
            edcamSetPos(&bridge.position);
            edbri_rotz = bridge.rotation_z;
            edbri_roty = bridge.rotation_y;
            edbri_length = bridge.length;
            edbri_width = bridge.field_14;
            edbri_planks = bridge.field_1d;
            edbri_post_interval = bridge.field_1e;
            edbri_plank_instance_type = bridge.special_20;
            edbri_post_instance_type = bridge.special_24;
        }
    }
    edcamGetPosAng(&edbri_cam_pos, &edbri_cam_ax, &edbri_cam_ay);
    if (!camera_locked) {
        const u32 pressed = pad->digital_buttons_pressed;
        if (pressed & 0x80) {
            edbri_options_menu = eduiMenuCreate(70, 70, 220, 300, ed_fnt, edbricbCancelOptMenu, "Options");
            if (edbri_options_menu) {
                eduiMenuAddItem(edbri_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edbricbPlankInstanceMenu, "Plank Instance..."));
                eduiMenuAddItem(edbri_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edbricbPostInstanceMenu, "Post Instance..."));
                eduiMenuAddItem(edbri_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edbricbPlankCountMenu, "Plank Counts..."));
                eduiMenuAddItem(edbri_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edbricbDpadModeMenu, "Dpad Mode..."));
                eduiMenuAddItem(edbri_options_menu,
                                eduiItemSelCreate(1, edbri_nearest == -1 ? edgrey : edblack, 0, 0,
                                                  edbri_nearest == -1 ? NULL : edbricbBridgePropertiesMenu,
                                                  "Bridge Properties..."));
                eduiMenuAddItem(edbri_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edbricbFileSave, "Save Bridges"));
                eduiMenuAddItem(edbri_options_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edbricbFileLoad, "Load Bridges"));
            }
            edbri_active_menu = edbri_options_menu;
        }
        if ((pressed & 0x40) && edbri_plank_instance_type != -1)
            edbriBridgeCreate(&edbri_cam_pos);
        if ((pad->digital_buttons & 0x20) && edbri_nearest != -1)
            edbriBridgePlace(edbri_nearest, &edbri_cam_pos);
        if (pressed & 0x10) {
            if (edbri_nearest != -1)
                edbriBridgeDestroy(edbri_nearest);
            edbri_nearest = -1;
        }
    }
    if (edbri_mode == 0) {
        edbri_roty += pad->analog_left_pad_right - pad->analog_left_pad_left;
        i32 rotation = edbri_rotz + pad->analog_left_pad_up;
        if (rotation > 0x4000)
            rotation = 0x2000;
        rotation -= pad->analog_left_pad_down;
        if (rotation < -0x4000)
            rotation = -0x2000;
        edbri_rotz = rotation;
    } else if (edbri_mode == 1) {
        edbri_length += pad->analog_left_pad_up / 2500.0f;
        edbri_length -= pad->analog_left_pad_down / 2500.0f;
        if (edbri_length < 0.1f)
            edbri_length = 0.1f;
        if (edbri_length > 5.0f)
            edbri_length = 5.0f;
        edbri_width += pad->analog_left_pad_right / 5000.0f;
        edbri_width -= pad->analog_left_pad_left / 5000.0f;
        if (edbri_width < 0.1f)
            edbri_width = 0.1f;
        if (edbri_width > 5.0f)
            edbri_width = 5.0f;
    }
}
