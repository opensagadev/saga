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
static u32 buttons_store[15];

void UpdateGameMenu(GAMEPAD_s *pad, i32 a2) {
    if (pad == NULL || GameMenuLevel < 0 || GameMenuLevel >= 10)
        return;
    const i32 menu_id = GetMenuID();
    if (a2 != 0 && WORLD != NULL) {
        if (Paused != 0 || GetMenuID() != -1) {
            memset(buttons_store, 0, sizeof(buttons_store));
        } else if (GamePad[0].buttons_down_08 != 0) {
            for (i32 index = 14; index > 0; --index)
                buttons_store[index] = buttons_store[index - 1];
            buttons_store[0] = GamePad[0].buttons_down_08;
        }
    }
    MENU *menu = &GameMenu[GameMenuLevel];
    const i16 previous_menu = menu->menu;
    bool both_pads = false;
    if ((Paused != 0 || NetPaused != 0) && pause_i_pad >= 0 && pause_i_pad < 64) {
        pad = &GamePad[pause_i_pad];
    } else if (WORLD == NULL) {
        both_pads = true;
    } else if (menu_id != -1 && WORLD->current_level == TITLES_LDATA) {
        both_pads = false;
    } else if (menu_id == 0x1e && PlayerProgress[0].active != 0 && PlayerProgress[1].active != 0) {
        both_pads = true;
    } else if (Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.field_0x1f8) < 0 && Player[1] != NULL &&
               static_cast<i8>(Player[1]->apiobj.field_0x1f8) < 0) {
        both_pads = true;
    } else if (pad == &GamePad[0]) {
        if (WORLD->current_level == CREDITS_LDATA) {
            if (PlayerProgress[1].active != 0) {
                if (PlayerProgress[0].active != 0)
                    both_pads = true;
                else
                    pad = &GamePad[1];
            } else if (PlayerProgress[0].active == 0) {
                readpads_always = 1;
                both_pads = true;
            }
        } else if (Player[0] != NULL && Player[1] != NULL) {
            const bool first_active =
                (Player[0]->apiobj.field_0x1f4 & 0x40000) == 0 && static_cast<i8>(Player[0]->apiobj.field_0x1f8) < 0;
            const bool second_active =
                (Player[1]->apiobj.field_0x1f4 & 0x40000) == 0 && static_cast<i8>(Player[1]->apiobj.field_0x1f8) < 0;
            if (second_active && !first_active && Player[1]->pad_gamepad != NULL)
                pad = Player[1]->pad_gamepad;
            else if (!first_active && !second_active) {
                readpads_always = 1;
                both_pads = true;
            }
        } else {
            readpads_always = 1;
            both_pads = true;
        }
    }
    if (editor_active == 0) {
        const u8 widescreen = Game.options_save.widescreen;
        loadsaveCallEachFrame();
        Game.options_save.widescreen = widescreen;
    }
    // Pending transitions must not re-enter menu callbacks (e.g. NewGame)
    // after the save loader has selected the destination level.
    if (FadeSys.fade <= 0.0f && NewMode == 0 && NewLData == NULL && editor_active == 0) {
        u32 held = 0, pressed = 0, alternate_held = 0, alternate_pressed = 0;
        if (menu_id != 1 || GameTimer.time_elapsed >= 4.0f) {
            if (both_pads) {
                held = GamePad[0].unknown_04 | GamePad[1].unknown_04;
                pressed = GamePad[0].buttons_down_08 | GamePad[1].buttons_down_08;
                alternate_held = GamePad[0].unknown_0c | GamePad[1].unknown_0c;
                alternate_pressed = GamePad[0].unknown_10 | GamePad[1].unknown_10;
            } else {
                held = pad->unknown_04;
                pressed = pad->buttons_down_08;
                alternate_held = pad->unknown_0c;
                alternate_pressed = pad->unknown_10;
            }
        }
        const i32 previous_item = menu->selected_item;
        const i32 previous_column = menu->selected_item_column;
        const i32 result = UpdateMenu(held, pressed, alternate_held, alternate_pressed, FRAMETIME, GAMEPAD_MENUSELECT,
                                      GAMEPAD_MENUCANCEL, GAMEPAD_START, GAMEPAD_SELECT);
        if (MenuSFX == -1 && menu->selected_item != -1 && menu->selected_item_column != -1 && previous_item != -1 &&
            previous_column != -1 &&
            (previous_item != menu->selected_item || previous_column != menu->selected_item_column) &&
            GetMenuID() == menu_id)
            GameAudio_PlaySfx(0x2f, NULL, 0, 0);
        if (result == 1 && Paused != 0)
            ResumeGame(1, 1);
    }
    if (previous_menu != -1 && NewLData != NULL && menu_id != 0x1b && gone_through_door_to_new_level == 0)
        reset_area = 1;
}
eduimenu_s *GetMenuActiveChild(eduimenu_s *menu) {
    if (menu == NULL) {
        return NULL;
    }
    while (menu->child != NULL) {
        menu = menu->child;
    }
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

    if (menu_index == -1) {
        MENU *menu = &GameMenu[GameMenuLevel];
        menu->field_bc = 0;
        MenuRememberCursor(menu);
        menu->menu = -1;
        MenuAlpha = 0.0f;
        MenuA = 0;
        MenuValidated = 0;
        return;
    }

    MENU *previous = &GameMenu[GameMenuLevel];
    const i16 previous_menu = previous->menu;
    previous->field_bc = 0;
    MenuRememberCursor(previous);
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
    menu->draw_item = 0;
    menu->field_b8 = 1;
    menu->field_b4 = 1;
    menu->field_b0 = 0;
    menu->field_c0 = 0;
    menu->field_50 = 0;

    MenuStopDraw = 1;
    if (MenuInfo[menu_index].draw_fn != NULL) {
        MenuInfo[menu_index].draw_fn(menu);
    }
    MenuStopDraw = 0;
    menu->last_row = static_cast<i16>(menu->draw_item - 1);
    menu->selected_row = (menu_y >= menu->first_row && menu_y <= menu->last_row) ? static_cast<i16>(menu_y) : 0;
    menu->selected_column = 0;

    if (MenuInfo[menu_index].enter_fn != NULL) {
        MenuInfo[menu_index].enter_fn(menu);
    }

    if (MenuInfo[menu_index].memory_x != -1) {
        menu->selected_column = MenuInfo[menu_index].memory_x;
        if (menu->selected_column < menu->first_column) {
            menu->selected_column = menu->first_column;
        } else if (menu->selected_column > menu->last_column) {
            menu->selected_column = menu->last_column;
        }
    }
    if (menu_y < menu->first_row || menu_y > menu->last_row) {
        if (MenuInfo[menu_index].memory_y != -1) {
            menu->selected_row = MenuInfo[menu_index].memory_y;
            if (menu->selected_row < menu->first_row) {
                menu->selected_row = menu->first_row;
            } else if (menu->selected_row > menu->last_row) {
                menu->selected_row = menu->last_row;
            }
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
