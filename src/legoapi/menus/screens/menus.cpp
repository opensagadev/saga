#include "decomp.h"
#include "batman.h"
#include "gamelib/crc/crc.h"
#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/audio/audio.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/render/fx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);
extern "C" void loadsaveCallEachFrame(void);
extern "C" bool TestForController(void);
i32 GetMenuID(void);
void MenuDrawBackground(void);
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_TOGGLERIGHT;
extern GAMEPAD_s GamePad[64];
extern i32 readpads_always;
extern i32 reset_area;
extern i32 MenuSFX;

eduimenu_s *GetMenuActiveChild(eduimenu_s *menu) {
    if (menu == NULL) {
        return NULL;
    }
    if (menu->child != NULL)
        return GetMenuActiveChild(menu->child);
    return menu;
}
void ResizePauseScreenTexture(i32, i32) {
    STUBBED();
}

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3) {
    (void)param3;

    CurrentMenuId = menu_id;

    i32 menu_index = -1;
    for (i32 i = 0; i < MenusUsed; ++i) {
        if (MenuInfo[i].id == menu_id) {
            menu_index = i;
            break;
        }
    }

    MENU *previous = &GameMenu[GameMenuLevel];
    const i16 previous_menu = previous->menu;
    previous->field_bc = 0;
    MenuRememberCursor(previous);
    if (menu_index == -1) {
        GameMenu[GameMenuLevel].menu = -1;
        MenuAlpha = 0.0f;
        MenuA = 0;
        MenuValidated = 0;
        return;
    }

    if (GameMenuLevel + 1 < 10) {
        ++GameMenuLevel;
    }

    MENU *menu = &GameMenu[GameMenuLevel];
    menu->menu_time = 0.0f;
    menu->unk = 0.0f;
    menu->menu = static_cast<i16>(menu_index);
    menu->previous_menu = static_cast<i8>(previous_menu);
    menu->first_column = 0;
    menu->first_row = 0;
    menu->last_column = 0;
    menu->last_row = 0;
    menu->state = 0;
    menu->field_b8 = 1;
    menu->draw_item = 0;
    menu->field_b4 = 1;
    menu->field_b0 = 0;
    menu->field_c0 = 0;
    menu->field_50 = 0;

    MenuStopDraw = 1;
    if (MenuInfo[menu->menu].id != -1 && MenuInfo[menu->menu].draw_fn != NULL) {
        MenuInfo[menu->menu].draw_fn(menu);
    }
    MenuStopDraw = 0;
    menu->last_row = static_cast<i16>(menu->draw_item - 1);
    menu->selected_row = (menu_y >= menu->first_row && menu_y <= menu->last_row) ? static_cast<i16>(menu_y) : 0;
    menu->selected_column = 0;

    if (MenuInfo[menu->menu].enter_fn != NULL) {
        MenuInfo[menu->menu].enter_fn(menu);
    }

    if (MenuInfo[menu->menu].memory_x != -1) {
        menu->selected_column = MenuInfo[menu->menu].memory_x;
    }
    if (menu->selected_column < menu->first_column) {
        menu->selected_column = menu->first_column;
    } else if (menu->selected_column > menu->last_column) {
        menu->selected_column = menu->last_column;
    }
    if (menu_y < menu->first_row || menu_y > menu->last_row) {
        if (MenuInfo[menu->menu].memory_y != -1) {
            menu->selected_row = MenuInfo[menu->menu].memory_y;
        }
        if (menu->selected_row < menu->first_row) {
            menu->selected_row = menu->first_row;
        } else if (menu->selected_row > menu->last_row) {
            menu->selected_row = menu->last_row;
        }
    } else {
        menu->selected_row = static_cast<i16>(menu_y);
    }

    menu->menu_time = 0.0f;
    menu->unk = 0.0f;
    menu->flags_17 = -1;
    menu->transition_time = 0.0f;
    menu->transition_duration = 0.0f;
    memset(menu->item_width, 0, sizeof(menu->item_width));
    memset(menu->item_height, 0, sizeof(menu->item_height));
    menu->queued_item = -1;
    MenuAlpha = 0.0f;
    MenuA = 0;
    MenuValidated = 0;
}
