#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" i32 PS2_REZ_H;

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

static i32 selection_value;
static f32 cliph;
static f32 clips;
static f32 clipv;
static f32 clipg;
static f32 clipboard_r = 0.5f;
static f32 clipboard_g = 0.5f;
static f32 clipboard_b = 0.5f;

static eduimenu_s *colourmenu;
static u32 *cp_rgba;
static f32 cpt_r;
static f32 cpt_g;
static f32 cpt_b;
static f32 cpt_a;
static f32 *cp_r;
static f32 *cp_g;
static f32 *cp_b;
static f32 *cp_a;
static edui_colour_pick_s *cp_item;
static eduiitem_s *cp_info;
static i32 (*cp_process)(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static eduiitem_s *cp_copy;
static eduiitem_s *cp_paste;

static __attribute__((always_inline)) inline u32 cpPackColour(f32 red, f32 green, f32 blue, f32 alpha) {
    return (static_cast<i32>(alpha * 255.0f) << 24) | ((static_cast<i32>(blue * 255.0f) & 0xff) << 16) |
           ((static_cast<i32>(green * 255.0f) & 0xff) << 8) | (static_cast<i32>(red * 255.0f) & 0xff);
}

static __attribute__((always_inline)) inline void cpUpdateInfo(eduiitem_s *info) {
    char text[256];
    if (info->data == 0) {
        sprintf(text, "H:%1.2f S:%1.2f V:%1.2f", cp_item->hue, cp_item->saturation, cp_item->value);
    } else {
        sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", cp_item->red, cp_item->green, cp_item->blue);
    }
    eduiItemSetText(info, text);
}

static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32) {
    *cp_r = cp_item->red;
    *cp_g = cp_item->green;
    *cp_b = cp_item->blue;
    if (cp_rgba != nullptr)
        *cp_rgba = cpPackColour(*cp_r, *cp_g, *cp_b, *cp_a);
    eduiMenuDetach(menu);
}
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32) {
    item->data = 1 - item->data;
    cpUpdateInfo(item);
}
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32) {
    f32 red = *cp_r;
    f32 green = *cp_g;
    f32 blue = *cp_b;
    clipboard_r = red;
    clipboard_g = green;
    clipboard_b = blue;
    i32 red_byte = static_cast<i32>(red * 255.0f) & 0xff;
    i32 green_byte = static_cast<i32>(green * 255.0f) & 0xff;
    i32 blue_byte = static_cast<i32>(blue * 255.0f) & 0xff;
    u32 colour = 0x80000000u | red_byte;
    colour |= green_byte << 8;
    colour |= blue_byte << 16;
    cp_paste->colours[2] = colour;
}
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32) {
    *cp_r = clipboard_r;
    *cp_g = clipboard_g;
    *cp_b = clipboard_b;
    if (cp_rgba != nullptr)
        *cp_rgba = cpPackColour(*cp_r, *cp_g, *cp_b, *cp_a);
    eduiItemColourPickSetRGB(cp_item, clipboard_r, clipboard_g, clipboard_b);
}
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad) {
    i32 result = cp_process(menu, item, delta_time, pad);
    cpUpdateInfo(cp_info);
    return result;
}

extern "C" void CreateColourPicker(void) {
    static i32 initialised;
    const u32 colours[4] __attribute__((aligned(16))) = {0x80000000u, 0x800000ffu, 0x80808080u, 0x80f0f0f0u};
    if (initialised != 0)
        return;

    initialised = 1;
    colourmenu = eduiMenuCreate(200, 70, 180, 250, ed_fnt, nullptr, const_cast<char *>("Pick Colour"));
    if (colourmenu == nullptr)
        return;

    cp_item = static_cast<edui_colour_pick_s *>(
        eduiMenuAddItem(colourmenu, eduiItemColourPickCreate(0, colours, cbColourPickSel, const_cast<char *>(""))));
    cp_process = cp_item->process;
    cp_item->process = cbProcessColourPick;
    cp_info = eduiMenuAddItem(
        colourmenu, eduiItemSelCreate(1, colours, 0, 0, cbToggleIndicatorMode, const_cast<char *>("R:?? G:?? B:??")));
    cp_copy = eduiMenuAddItem(colourmenu,
                              eduiItemSelCreate(1, colours, 0, 0, cbCopy, const_cast<char *>("Copy To Clipboard")));
    cp_paste = eduiMenuAddItem(
        colourmenu, eduiItemSelCreate(1, colours, 0, 0, cbPaste, const_cast<char *>("Paste From Clipboard")));
}

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba) {
    eduiMenuAttach(parent, colourmenu);
    colourmenu->x = (item != nullptr ? item->x : parent->x) + 10;
    i32 y = (item != nullptr ? item->y : parent->y) + 10;
    i32 bottom = PS2_REZ_H - 20 - colourmenu->height;
    colourmenu->y = y <= bottom ? y : bottom;
    cp_rgba = rgba;

    f32 red_value;
    f32 green_value;
    f32 blue_value;
    if (rgba != nullptr) {
        u32 colour = *rgba;
        red_value = static_cast<f32>(colour & 0xff) / 255.0f;
        green_value = static_cast<f32>((colour >> 8) & 0xff) / 255.0f;
        blue_value = static_cast<f32>((colour >> 16) & 0xff) / 255.0f;
        f32 alpha_value = static_cast<f32>(colour >> 24) / 255.0f;
        cpt_r = red_value;
        cpt_g = green_value;
        cpt_b = blue_value;
        cpt_a = alpha_value;
        cp_r = &cpt_r;
        cp_g = &cpt_g;
        cp_b = &cpt_b;
    } else {
        cp_r = red;
        cp_g = green;
        cp_b = blue;
        red_value = *red;
        green_value = *green;
        blue_value = *blue;
    }
    cp_a = &cpt_a;

    eduiItemColourPickSetRGB(cp_item, red_value, green_value, blue_value);
    char text[256];
    sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", *cp_r, *cp_g, *cp_b);
    eduiItemSetText(cp_info, text);
}

extern "C" {

    static __used__ void cbGradChange(void) {
    }

    static __used__ void cbSel(eduimenu_s *, eduiitem_s *item) {
        selection_value = item->data;
    }

    static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item) {
        eduiMenuAttach(menu, reinterpret_cast<eduimenu_s *>(item->data_ptr));
    }

    static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        f32 time = 0.0f;
        f32 hue = 0.0f;
        f32 saturation = 0.0f;
        f32 intensity = 1.0f;
        if (stage != nullptr) {
            time = ((stage->next != nullptr ? stage->next->time : 1.0f) + stage->time) * 0.5f;
            hue = stage->hue;
            saturation = stage->saturation;
            intensity = stage->value;
        }
        picker->selected_stage = eduiGradStageAdd(picker, time, hue, saturation, intensity);
        if (picker->add != nullptr)
            picker->add(menu, item, value);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        if (picker->selected_stage != nullptr && (item->type == 10 || item->type == 11)) {
            edui_colour_pick_s *colour = reinterpret_cast<edui_colour_pick_s *>(item);
            eduiGradStageSetHSV(picker->selected_stage, colour->hue, colour->saturation, colour->value);
        }
        if (picker->press != nullptr)
            picker->press(menu, item, value);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        if (stage != nullptr) {
            if (item->type == 7) {
                cliph = stage->hue;
                clips = stage->saturation;
                clipv = stage->value;
            } else if (item->type == 8) {
                clipg = stage->value;
            }
            if (picker->copy != nullptr)
                picker->copy(menu, item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        if (picker->remove != nullptr)
            picker->remove(menu, item, value);
        if (picker->selected_stage != nullptr)
            eduiGradStageDelete(picker, picker->selected_stage);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        if (stage != nullptr) {
            if (item->type == 7)
                eduiGradStageSetHSV(stage, cliph, clips, clipv);
            else if (item->type == 8)
                eduiGradStageSetHSV(stage, 0.0f, 0.0f, clipg);
            if (picker->paste != nullptr)
                picker->paste(menu, item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

} // extern "C"

#pragma GCC pop_options
