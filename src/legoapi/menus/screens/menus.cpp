#include "decomp.h"
#include "gamelib/crc/crc.h"
#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
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
extern "C" i32 TestForController(void);
i32 GetMenuID(void);
void MenuDrawBackground(void);
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_TOGGLERIGHT;
extern GAMEPAD_s GamePad[64];

void UpdateGameMenu(GAMEPAD_s *pad, i32 a2) {
    (void)a2;
    if (pad == nullptr || GameMenuLevel < 0)
        return;

    // Original UpdateGameMenu (0x1192b0) skips menu callbacks while a level
    // change is pending. Re-entering the title menu here starts NewGame and
    // erases the save that the preceding frame just loaded.
    if (NewLData != NULL) {
        loadsaveCallEachFrame();
        return;
    }

    const u32 held = GamePad[0].unknown_04 | GamePad[1].unknown_04;
    const u32 pressed = GamePad[0].buttons_down_08 | GamePad[1].buttons_down_08;
    const u32 alternate_held = GamePad[0].unknown_0c | GamePad[1].unknown_0c;
    const u32 alternate_pressed = GamePad[0].unknown_10 | GamePad[1].unknown_10;
    UpdateMenu(held, pressed, alternate_held, alternate_pressed, FRAMETIME, GAMEPAD_MENUSELECT, GAMEPAD_MENUCANCEL,
               GAMEPAD_START, GAMEPAD_SELECT);
    loadsaveCallEachFrame();
}
i32 GetParentMenuID() {
    if (GameMenuLevel <= 1) {
        return -1;
    }

    const i16 parent_menu = GameMenu[GameMenuLevel - 1].menu;
    if (parent_menu == -1) {
        return -1;
    }
    return MenuInfo[parent_menu].id;
}
void GetMenuActiveChild(eduimenu_s *) {
}
void ResizePauseScreenTexture(i32, i32) {
}
i32 CodeMenu(MENU_s *);

extern void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
extern void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);
extern void Hint_CancelCurrent(void);
extern void DrawSubItemMenu2D(void);
extern void DrawSubItemMenu3D(void);
extern void DrawCodeMenu(void);
extern void DrawCodeMenu3D(void);
static i32 SubItemMenu(MENU_s *);
extern void Hint_ResetHint(i32, i32);
extern void Hint_SetHintFromId(i32, i32, i32);
extern i16 HintTab[24];
extern i16 tUNKNOWN;
extern "C" void PlaySfx(char *, NUVEC *);
extern void AddToCompletionPoints(u32);

static f32 ShopClamp01(f32 value) {
    return NuFmax(0.0f, NuFmin(value, 1.0f));
}

static f32 ShopSinePhase(f32 phase) {
    const i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
    return (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
}

static void Shop_GetInput(SHOPINPUT *input) {
    memset(input, 0, sizeof(*input));
    const u32 select = GAMEPAD_MENUSELECT;
    const i32 client = netclient;
    const u32 cancel = GAMEPAD_MENUCANCEL;
    const u32 left = GAMEPAD_DLEFT | GAMEPAD_TOGGLELEFT;
    const u32 right = GAMEPAD_DRIGHT | GAMEPAD_TOGGLERIGHT;
    const u32 up = GAMEPAD_DUP;
    const u32 down = GAMEPAD_DDOWN;
    for (i32 p = 0; p < 2; ++p) {
        if (!MenuPacket.active_player[p])
            continue;
        const u32 pressed = GamePad[p].buttons_pressed;
        if ((pressed & select) != 0 && client == 0) {
            input->confirm = 1;
            return;
        }
        if ((pressed & cancel) != 0) {
            input->cancel = 1;
            return;
        }
        if (client != 0)
            continue;
        const u32 held = GamePad[p].buttons_held | GamePad[p].unknown_0c;
        const u32 edge = pressed | GamePad[p].unknown_10;
        if (held & left)
            input->left_held = 1;
        if (held & right)
            input->right_held = 1;
        if (input->left_held && input->right_held)
            input->left_held = input->right_held = 0;
        if (held & up)
            input->up_held = 1;
        if (held & down)
            input->down_held = 1;
        if (input->up_held && input->down_held)
            input->up_held = input->down_held = 0;
        if (edge & left)
            input->left_pressed = 1;
        if (edge & right)
            input->right_pressed = 1;
        if (input->left_pressed && input->right_pressed)
            input->left_pressed = input->right_pressed = 0;
        if (edge & up)
            input->up_pressed = 1;
        if (edge & down)
            input->down_pressed = 1;
        if (input->up_pressed && input->down_pressed)
            input->up_pressed = input->down_pressed = 0;
    }
}

static i32 SubItemMenu(MENU_s *menu) {
    static i32 movesfxlock;
    SHOPINPUT input;
    Shop_GetInput(&input);
    if (hintdrawwait > 0.0f) {
        hintdrawwait -= FRAMETIME;
        ShopNameAlpha = 0.0f;
    }
    if (ExitMenu)
        ShopNameAlpha = 0.0f;

    shopitem_s *items = NULL;
    i32 *ids = NULL, *count = NULL;
    NUVEC *positions = NULL;
    f32 ypush = 0.0f;
    switch (picked) {
        case 0:
            items = HintItems;
            ids = HintShelfIds;
            positions = HintCurPos;
            count = &SHOPHINTCOUNT;
            if (hintdrawwait <= 0.0f && (items[ids[3]].unlocked == 1 || items[ids[3]].price == 0)) {
                Hint_ResetHint(HintTab[static_cast<u16>(items[ids[3]].item_id)], 0);
                Hint_SetHintFromId(HintTab[static_cast<u16>(items[ids[3]].item_id)], 1, 0);
            } else
                Hint_CancelCurrent();
            ypush = SubNormCharPush;
            break;
        case 1:
            items = CharItems;
            ids = CharShelfIds;
            positions = CharCurPos;
            count = &SHOPCHARCOUNT;
            ypush = SubNormCharPush;
            break;
        case 2:
            items = ExtraItems;
            ids = ExtraShelfIds;
            positions = ExtraCurPos;
            count = &SHOPEXTRACOUNT;
            ypush = SubNormCharPush;
            break;
        case 4:
            items = BrickItems;
            ids = BrickShelfIds;
            positions = BrickCurPos;
            count = &SHOPGOLDBRICKS;
            ypush = SubNormCharPush;
            break;
        case 5:
            items = CutItems;
            ids = CutShelfIds;
            positions = CutCurPos;
            count = &SHOPCUTCOUNT;
            ypush = SubNormCharPush;
            break;
    }
    if (slidetimer >= 0.0f) {
        slidetimer -= FRAMETIME;
        if (slidetimer < 0.0f)
            ShopNameAlpha = 0.0f;
    }
    if (easesubin != 0) {
        const f32 t = ShopClamp01(1.0f - ShopSinePhase(1.0f - slidetimer * 8.0f));
        if (easesubin == 1)
            inoutscale = t;
        else if (easesubin == -1)
            inoutscale = 1.0f - t;
    }

    i32 left = 0, right = 0;
    if (moveitems != 0) {
        if (movesfxlock == 0) {
            movesfxlock = 1;
            GameAudio_PlaySfx(0x2f, positions, 0, 0);
        }
        const i32 previous = moveitems;
        i32 moved = 0;
        if (previous < 0) {
            moved = MoveSubItemsLeft(ids, positions, *count);
            left = moveitems != 0 && previous != moveitems;
        } else if (previous > 0) {
            moved = MoveSubItemsRight(ids, positions, *count);
            right = moveitems != 0 && previous != moveitems;
        }
        if (moved && items != NULL && items[ids[3]].type == 0 && items[ids[3]].unlocked == 1)
            SelectSubItem();
    } else
        movesfxlock = 0;

    i32 cancel = 0, confirm = 0;
    if (slidetimer < 0.0f) {
        easesubin = 0;
        if (input.left_held)
            left = 1;
        else if (input.right_held)
            right = 1;
        else if (input.confirm && !ExitMenu)
            confirm = 1;
        else if (input.cancel)
            cancel = 1;
    }
    if (subitemselected == 1)
        subitemselected = 2;
    if (menu->confirm_pressed) {
        if (menu->selected_row == 1) {
            if (menu->selected_column == 3)
                confirm = 1;
            else {
                moveitems = menu->selected_column - 3;
                if (moveitems < 0)
                    left = 1;
                else if (moveitems > 0)
                    right = 1;
            }
        } else if (menu->selected_row == 2 && subitemselected > 0) {
            if (menu->selected_column == 0)
                confirm = 1;
            else {
                subitemselected = 0;
                PlaySfx("MenuSelect", 0);
            }
        }
    }
    if (menu->left_pressed && subitemselected == 0) {
        moveitems = -static_cast<i32>(menu->horizontal_scroll_distance / 0.15f);
        left = 1;
    }
    if (menu->right_pressed && subitemselected == 0) {
        moveitems = static_cast<i32>(menu->horizontal_scroll_distance / 0.15f);
        right = 1;
    }

    i32 scrolling = 0;
    if (!ExitMenu) {
        if (left || right) {
            scaleoverride[0] = left ? 0.0f : 1.0f;
            scaleoverride[1] = 0.4f;
            scaleoverride[2] = 0.8f;
            scaleoverride[4] = 0.8f;
            scaleoverride[5] = 0.4f;
            scaleoverride[6] = left ? 1.0f : 0.0f;
            NUVEC pos = SubShelfPos[left ? 0 : 6];
            pos.y += ypush;
            AddGameDebris(WORLD->debris_sys, qrand() / 10923 + 76, &pos);
            pos = SubShelfPos[left ? 6 : 0];
            pos.y += ypush;
            AddGameDebris(WORLD->debris_sys, qrand() / 10923 + 76, &pos);
            if (moveitems == 0)
                moveitems = left ? -1 : 1;
            slidetimer = 0.125f;
            hintdrawwait = 0.2f;
            scrolling = 1;
        }
        if (cancel) {
            easesubin = -1;
            slidetimer = 0.125f;
            ExitMenu = 1;
            GameAudio_PlaySfx(0x31, NULL, 0, 0);
        }
        if (confirm) {
            if (subitemselected > 0) {
                SelectSubItem();
                subitemselected = 0;
            } else
                subitemselected = 1;
        }
    }
    if (ExitMenu && slidetimer < 0.0f) {
        ExitMenu = 0;
        easesubin = -1;
        lastitem = -1;
        slidetimer = 0.125f;
        GameCam_Blend(GameCam, 0.6f, 0.0f, 1);
        ShopNameAlpha = 0.0f;
        return 6;
    }
    scrollkeyhit = scrolling;
    return 0;
}

i32 CodeMenu(MENU_s *) {
    static f32 timer;
    static __used__ i32 itemchanged;
    static i32 movesfxlock;
    SHOPINPUT input;
    Shop_GetInput(&input);
    if (slidetimer >= 0.0f)
        slidetimer -= FRAMETIME;
    if (timer >= 0.0f)
        timer -= FRAMETIME;
    itemchanged = 0;
    if (easesubin) {
        f32 factor = 1.0f - ShopSinePhase(1.0f - 8.0f * slidetimer);
        if (easesubin == 1)
            inoutscale = 0.0f + ShopClamp01(factor);
        else if (easesubin == -1)
            inoutscale = 1.0f - ShopClamp01(factor);
    }
    if (slidetimer <= 0.0f) {
        easesubin = 0;
        codevalid = 0;
        i32 left = 0, right = 0, down = 0, up = 0;
        if (input.left_held)
            left = 1;
        else if (input.right_held)
            right = 1;
        else if (input.down_held) {
            if (timer <= 0.0f || input.down_pressed)
                down = 1;
        } else if (input.up_held) {
            if (timer <= 0.0f || input.up_pressed)
                up = 1;
        }
        i32 confirm = input.confirm != 0;
        i32 cancel = !confirm && input.cancel != 0;
        if (down) {
            timer = 0.125f;
            movesfxlock = 1;
            if (--codechar < 0)
                codechar = 35;
            usercode[col] = codechars[codechar];
            GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
        }
        if (up) {
            timer = 0.125f;
            movesfxlock = 1;
            if (++codechar >= 36)
                codechar = 0;
            codevalid = 0;
            usercode[col] = codechars[codechar];
            GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
        }
        if (left) {
            lastitem = col;
            if (--col < 0)
                col = 0;
            codevalid = 0;
            if (!usercode[col])
                codechar = 0;
            else
                for (i32 i = 0; i < 36; ++i) {
                    if (usercode[col] == codechars[i]) {
                        codechar = i;
                        break;
                    }
                }
            if (lastitem != col) {
                slidetimer = 0.125f;
                if (!movesfxlock) {
                    movesfxlock = 1;
                    GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
                }
            }
        }
        if (right) {
            lastitem = col;
            if (++col > 5)
                col = 5;
            codevalid = 0;
            if (!usercode[col])
                codechar = 0;
            else
                for (i32 i = 0; i < 36; ++i) {
                    if (usercode[col] == codechars[i]) {
                        codechar = i;
                        break;
                    }
                }
            if (lastitem != col) {
                slidetimer = 0.125f;
                if (!movesfxlock) {
                    movesfxlock = 1;
                    GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
                }
            }
        }
        if (cancel) {
            easesubin = -1;
            slidetimer = 0.125f;
            codevalid = codechar = 0;
            ExitMenu = 1;
            GameAudio_PlaySfx(0x31, NULL, 0, 0);
        }
        if (confirm) {
            char code[7];
            code[6] = 0;
            usercode[col] = codechars[codechar];
            for (i32 i = 0; i < 6; ++i)
                code[i] = usercode[i];
            u32 crc = CRC_ProcessStringIgnoreCase(code);
            i32 i;
            for (i = 0; i < 144; ++i)
                if (codelist[i] && codelist[i] == crc)
                    break;
            if (i == 144) {
                PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
                cheatname = NULL;
            } else {
                codevalid = 1;
                if (i < charcheatix) {
                    u16 id = CharItems[i].item_id;
                    if (Game_CharacterSave && !(Game_CharacterSave[id] & SAVE_CHARACTER_UNLOCKED)) {
                        Game_CharacterSave[id] |= SAVE_CHARACTER_UNLOCKED;
                        AddToCompletionPoints(POINTS_PER_CHARACTER);
                        PlaySfx("Shop_BuyCheat", &SubShelfPos[3]);
                    } else
                        PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
                    cheatname = TTab[CDataList[id].name_id];
                } else if (i < extracheatix) {
                    i -= charcheatix;
                    if (!(Game.extra_purchased_bits[i / 32] & (1U << (i & 31)))) {
                        Game.extra_purchased_bits[i / 32] |= 1U << (i & 31);
                        ExtraItems[i].unlocked = SAVE_ON;
                        AddToCompletionPoints(POINTS_PER_CHEAT);
                        i8 area = static_cast<i8>(Cheat[i].area);
                        if (area != -1 && !Game.area_save[area].red_brick_collected) {
                            Game.area_save[area].red_brick_collected = SAVE_COMPLETE;
                            AddToCompletionPoints(POINTS_PER_REDBRICK);
                        }
                        Game.extra_unlocked_bits[i / 32] |= static_cast<u32>(1ULL << (i % 32));
                        PlaySfx("Shop_BuyCheat", &SubShelfPos[3]);
                    } else
                        PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
                    cheatname = TTab[Cheat[i].text_id ? *Cheat[i].text_id : tUNKNOWN];
                }
                pickedbing = 0.35f;
                cheattimer = 3.1499998569488525f;
            }
        }
    } else
        movesfxlock = 0;

    if (slidetimer >= 0.0f) {
        f32 factor = 1.0f - ShopSinePhase(1.0f - 8.0f * slidetimer);
        if (lastitem != -1) {
            factor = ShopClamp01(factor);
            if (easesubin == 1) {
                codemenuscale[lastitem] =
                    codeshelfscale[lastitem] + (codebigscale[lastitem] - codeshelfscale[lastitem]) * factor;
                factor = ShopClamp01(factor);
                subpush[2] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * factor;
            } else {
                codemenuscale[lastitem] =
                    codebigscale[lastitem] + (codeshelfscale[lastitem] - codebigscale[lastitem]) * factor;
                factor = ShopClamp01(factor);
                subpush[2] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * factor;
            }
        }
        if (col != -1) {
            factor = ShopClamp01(factor);
            if (easesubin == -1) {
                codemenuscale[col] = codebigscale[col] + (codeshelfscale[col] - codebigscale[col]) * factor;
                factor = ShopClamp01(factor);
                subpush[1] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * factor;
            } else {
                codemenuscale[col] = codeshelfscale[col] + (codebigscale[col] - codeshelfscale[col]) * factor;
                factor = ShopClamp01(factor);
                subpush[1] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * factor;
            }
        }
    } else
        lastitem = -1;
    if (ExitMenu && slidetimer < 0.0f) {
        ExitMenu = 0;
        ShopNameAlpha = 0.0f;
        easesubin = -1;
        lastitem = -1;
        subpush[1] = 0.05f;
        slidetimer = 0.125f;
        GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
        return 6;
    }
    return 7;
}

i32 ItemMenu(MENU_s *menu) {
    SHOPINPUT input;
    const i32 entry_picked = picked;
    Shop_GetInput(&input);

    if (slidetimer >= 0.0f) {
        slidetimer -= FRAMETIME;
        ShopNameAlpha = 0.0f;
    }
    if (ExitMenu)
        ShopNameAlpha = 0.0f;

    i32 candidate = entry_picked;
    i32 cancel = 0;
    i32 selection = -1;
    if (menu->confirm_pressed != 0 && menu->selected_row == 0) {
        candidate = static_cast<i32>(menu->selected_column) + 1;
        if (candidate == entry_picked) {
            input.value[8] = 1;
        }
    }

    itemchanged = 0;
    Hint_CancelCurrent();
    hintdrawwait = 0.2f;

    if (slidetimer < 0.0f) {
        movesfxlock = 0;

        if (input.value[0] != 0 && picked > 1) {
            candidate = picked - 1;
            if (candidate == 4 && SHOPGOLDBRICKS == 0)
                candidate = picked - 2;
            if (candidate == 3)
                candidate = 2;
        } else if (input.value[1] != 0 && picked <= 3) {
            candidate = picked + 1;
            if (candidate == 3)
                candidate = picked + 2;
            if (candidate == 4 && SHOPGOLDBRICKS == 0)
                candidate = picked;
        } else if (input.value[8] != 0) {
            if (picked == 0) {
                picked = 1;
            } else if (picked == 5) {
                picked = SHOPGOLDBRICKS != 0 ? 4 : 3;
            } else
                selection = picked;
            if (selection == -1)
                candidate = entry_picked;
        } else if (input.value[9] != 0) {
            cancel = 1;
        }
    }

    if (candidate != entry_picked) {
        lastitem = picked;
        SubMenu = 0;
        picked = candidate <= 0 ? 1 : candidate > 5 ? 5 : candidate;

        slidetimer = 0.125f;
        if (movesfxlock == 0) {
            movesfxlock = 1;
            GameAudio_PlaySfx(0x2f, reinterpret_cast<nuvec_s *>(SubShelfPos) + 5, 0, 0);
        }
        GameCam_Blend(GameCam, 0.3f, 0.0f, 1);
        ShopNameAlpha = 0.0f;
    }

    if (selection != -1) {
        if (selection != picked) {
            picked = selection;
            if (!movesfxlock) {
                movesfxlock = 1;
                GameAudio_PlaySfx(0x2f, &SubShelfPos[5], 0, 0);
            }
            ShopNameAlpha = 0.0f;
        }
        lastitem = picked;
        picked = -1;
        slidetimer = 0.125f;
        SubMenu = 1;
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
        GameCam_Blend(GameCam, 0.6f, 0.0f, 1);
        ShopNameAlpha = 0.0f;
    }
    if (cancel) {
        ExitMenu = 1;
        slidetimer = 0.125f;
        lastitem = picked;
        picked = -1;
        SubMenu = 0;
        GameAudio_PlaySfx(0x31, NULL, 0, 0);
    }

    if (slidetimer < 0.0f)
        enteredshop = 0;
    else if ((lastitem != picked || enteredshop) && !SubMenu) {
        const f32 reverse_phase = slidetimer * 8.0f;
        const f32 old_factor = ShopClamp01(1.0f - ShopSinePhase(1.0f - reverse_phase));
        const f32 forward_factor = ShopClamp01(1.0f - ShopSinePhase(reverse_phase));

        if (lastitem != -1) {
            topscale[lastitem] = TopBigScale[lastitem] + (TopShelfScale[lastitem] - TopBigScale[lastitem]) * old_factor;
            toppush[lastitem] = TopBigPush[lastitem] + (TopShelfPush[lastitem] - TopBigPush[lastitem]) * forward_factor;
        }

        if (picked != -1 && lastitem != -1) {
            topscale[picked] = TopShelfScale[picked] + (TopBigScale[picked] - TopShelfScale[picked]) * old_factor;
            toppush[picked] = TopShelfPush[picked] + (TopBigPush[picked] - TopShelfPush[picked]) * forward_factor;
        }
    }

    if (SubMenu == 0) {
        if (ExitMenu && slidetimer < 0.0f) {
            menuptr = nullptr;
            return 5;
        }
        return 7;
    }

    if (slidetimer > 0.0f)
        return 7;

    picked = lastitem;
    SubMenu = 0;
    if (TopShelf[lastitem].type == 3) {
        lastitem = -1;
        easesubin = 1;
        inoutscale = 0.0f;
        cheatname = 0;
        slidetimer = 0.125f;
        shopmenu = 11;
        col = 0;

        ++currentmenulevel;
        menuparent[currentmenulevel] = menuptr;
        menuptr = CodeMenu;
        memcpy(usercode, "AAAAAA", 6);

        ++currentdrawlevel;
        drawparent[currentdrawlevel] = drawpanelptr;
        drawpanelptr = DrawCodeMenu;
        drawptr = DrawCodeMenu3D;
    } else {
        easesubin = 1;
        ++currentmenulevel;
        inoutscale = 0.0f;
        menuparent[currentmenulevel] = menuptr;
        menuptr = SubItemMenu;

        ++currentdrawlevel;
        slidetimer = 0.125f;
        drawparent[currentdrawlevel] = drawpanelptr;
        drawpanelptr = DrawSubItemMenu2D;
        drawptr = DrawSubItemMenu3D;
    }
    return 7;
}
extern i16 tSELECT, tEXIT, tBACK, tBUY, tPLAY, tSELECTING;
void DrawPlayerIconPrompts(i32, i32, f32, i32, i32, i32, i32, i32, i32, f32, i32, i32, i32, i32);

void DrawShopPrompts() {
    i32 select = -1;
    i32 back = -1;
    f32 alpha = 1.0f;
    if (menuptr == ItemMenu) {
        select = tSELECT;
        back = tEXIT;
    } else if (menuptr == SubItemMenu) {
        select = picked == 5 ? tPLAY : tBUY;
        back = tBACK;
        alpha = 0.25f;
        switch (picked) {
            case 0: {
                shopitem_s *item = &HintItems[HintShelfIds[3]];
                if (item->price <= Game.coins && item->unlocked != 1)
                    alpha = 1.0f;
                break;
            }
            case 1: {
                shopitem_s *item = &CharItems[CharShelfIds[3]];
                if (item->price <= Game.coins && CollectIDUnlocked(static_cast<u16>(item->item_id)) &&
                    item->unlocked != 1)
                    alpha = 1.0f;
                break;
            }
            case 2: {
                shopitem_s *item = &ExtraItems[ExtraShelfIds[3]];
                u16 id = item->item_id;
                i8 area = static_cast<i8>(Cheat[id].area);
                if (item->price <= Game.coins && (id <= 7 || area == -1 || Game.area_save[area].red_brick_collected) &&
                    !(Game.extra_purchased_bits[id >> 5] >> (id & 31) & 1) && item->unlocked != 1)
                    alpha = 1.0f;
                break;
            }
            case 4: {
                i32 id = BrickShelfIds[3];
                if (BrickItems[id].price <= Game.coins && static_cast<f32>(id * 3600) <= Game.field30_0x7c2c)
                    alpha = 1.0f;
                break;
            }
            case 5:
                if (CutScenePlayer_CanStart(CutShelfIds[3]))
                    alpha = 1.0f;
                break;
        }
    } else if (menuptr == CodeMenu) {
        select = tSELECT;
        back = tBACK;
    }
    if (netclient)
        select = -1;
    if (select != -1 || back != -1)
        DrawPlayerIconPrompts(MenuPacket.active_player[0], select, alpha, -1, back, -1, tSELECTING,
                              MenuPacket.active_player[1], select, alpha, -1, back, -1, tSELECTING);
}

i32 GetMenuID(void) {
    if (GameMenu[GameMenuLevel].menu != -1) {
        return MenuInfo[GameMenu[GameMenuLevel].menu].id;
    }
    return -1;
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
