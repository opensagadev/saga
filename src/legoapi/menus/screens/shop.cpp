#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gamelib/crc/crc.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
#include <stdio.h>
extern "C" void PlaySfx(char *, NUVEC *);
extern "C" void NuIOS_RecordFlurryEvent(char *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
i32 AddToCollection(i32);
void AddToCompletionPoints(u32);
void AddToGoldBricks();
extern GAMESAVE_s TempGame;
extern i32 hub_forceshopsave;
i32 POINTS_PER_HINT;
__attribute__((visibility("hidden"))) f32 pickedbing;
__attribute__((visibility("hidden"))) i32 charcheatix;
__attribute__((visibility("hidden"))) i32 extracheatix;
__attribute__((visibility("hidden"))) f32 cheattimer;
__attribute__((visibility("hidden"))) f32 codebigscale[6];
__attribute__((visibility("hidden"))) f32 codeshelfscale[6];
__attribute__((visibility("hidden"))) f32 codemenuscale[6];
i32 codevalid;
i32 codechar;
char codechars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

shopitem_s TopShelf[6] = {};
shopitem_s *CharItems = NULL;
shopitem_s *HintItems = NULL;
shopitem_s *ExtraItems = NULL;
shopitem_s *CodeItems = NULL;
shopitem_s *BrickItems = NULL;
shopitem_s CutItems[128] = {};
nuhspecial_s extrasils[44] = {};
nuhspecial_s atoz0to9icon[36] = {};

u32 codelist[144] = {};
i32 SHOPCHARCOUNT = 0;
i32 SHOPHINTCOUNT = 0;
i32 SHOPEXTRACOUNT = 0;
i32 CutScenePlayCount = 0;
i16 HintTab[24] = {-1};

i32 CharShelfIds[7] = {};
i32 HintShelfIds[7] = {};
i32 ExtraShelfIds[7] = {};
i32 BrickShelfIds[7] = {};
i32 CutShelfIds[7] = {};
NUVEC CharCurPos[7] = {};
NUVEC HintCurPos[7] = {};
NUVEC ExtraCurPos[7] = {};
NUVEC BrickCurPos[7] = {};
NUVEC CutCurPos[7] = {};

__attribute__((visibility("hidden"))) NUGSPLINE *splshelf = NULL;
__attribute__((visibility("hidden"))) NUGSPLINE *splcharshelf = NULL;
__attribute__((visibility("hidden"))) NUGSPLINE *splcodes = NULL;
NUVEC ShelfPos[6] = {};
NUVEC SubShelfPos[7] = {};
NUVEC CodePos[7] = {};
u16 shelfang = 0;

nuhspecial_s iconback = {};
nuhspecial_s infoblank = {};
nuhspecial_s cutblank = {};
nuhspecial_s cutfilm_unlocked = {};
nuhspecial_s cutfilm_locked = {};
nuhspecial_s toolblank = {};
nuhspecial_s codeblank = {};
nuhspecial_s question = {};
nuhspecial_s arrow1 = {};
nuhspecial_s arrow2 = {};
nuhspecial_s arrow3 = {};
nuhspecial_s arrow4 = {};
NUGSPLINE *shopcamspline = NULL;
NUVEC *shopcampos = NULL;
NUVEC *shopcamlookat = NULL;
__attribute__((visibility("hidden"))) f32 TopShelfScale[6] = {};
__attribute__((visibility("hidden"))) f32 TopBigScale[6] = {};
__attribute__((visibility("hidden"))) f32 topscale[6] = {};
__attribute__((visibility("hidden"))) f32 TopShelfPush[6] = {};
__attribute__((visibility("hidden"))) f32 TopBigPush[6] = {};
__attribute__((visibility("hidden"))) f32 toppush[6] = {};
i32 oldpicked = 1;
i32 picked = 1;
i32 subpicked = 0;
i32 subitemselected = 0;
__attribute__((visibility("hidden"))) void (*drawptr)() = NULL;

__attribute__((visibility("hidden"))) f32 scale2 = 0.0f;
static f32 scalenorm;
static f32 scalepicked;
__attribute__((visibility("hidden"))) f32 scale3 = 0.0f;
__attribute__((visibility("hidden"))) f32 scale4 = 0.0f;
__attribute__((visibility("hidden"))) f32 subpush[3] = {};
__attribute__((visibility("hidden"))) i32 moveitems = 0;
NUVEC selectedoff = {};
void *shopcutsceneplayer = NULL;

f32 ShopLockedScale = 0.0f;
f32 ShopNameAlpha = 0.0f;
i32 enteredshop = 0;
i32 SHOPACTIVE = 0;
i32 shopmenu = 0;
i32 col = 0;
char usercode[6] = {'A', 'A', 'A', 'A', 'A', 'A'};
extern i32 shop_from_cutsceneplayer;

__attribute__((visibility("hidden"))) f32 SubBigCharPush = 0.0f;
__attribute__((visibility("hidden"))) f32 SubNormCharPush = 0.0f;
__attribute__((visibility("hidden"))) ShopMenuCallback menuptr = NULL;
__attribute__((visibility("hidden"))) ShopMenuCallback oldmenuptr = NULL;
__attribute__((visibility("hidden"))) ShopMenuCallback menuparent[3] = {};
__attribute__((visibility("hidden"))) ShopMenuCallback oldmenuparent[3] = {};
__attribute__((visibility("hidden"))) ShopDrawCallback drawparent[3] = {};
__attribute__((visibility("hidden"))) ShopDrawCallback olddrawparent[3] = {};
__attribute__((visibility("hidden"))) ShopDrawCallback drawpanelptr = NULL;
__attribute__((visibility("hidden"))) ShopDrawCallback olddrawpanelptr = NULL;
__attribute__((visibility("hidden"))) ShopDrawCallback olddrawptr = NULL;
__attribute__((visibility("hidden"))) i32 currentmenulevel = 0;
__attribute__((visibility("hidden"))) i32 oldcurrentmenulevel = 0;
__attribute__((visibility("hidden"))) i32 currentdrawlevel = 0;
__attribute__((visibility("hidden"))) i32 oldcurrentdrawlevel = 0;
__attribute__((visibility("hidden"))) f32 oldpickedscale = 0.0f;
__attribute__((visibility("hidden"))) f32 oldpickedpush = 0.0f;
__attribute__((visibility("hidden"))) f32 slidetimer = 0.0f;
__attribute__((visibility("hidden"))) f32 scaleoverride[7] = {};
__attribute__((visibility("hidden"))) i32 ExitMenu = 0;
__attribute__((visibility("hidden"))) i32 lastitem = 0;
__attribute__((visibility("hidden"))) i32 itemchanged = 0;
__attribute__((visibility("hidden"))) f32 hintdrawwait = 0.0f;
__attribute__((visibility("hidden"))) i32 movesfxlock = 0;
__attribute__((visibility("hidden"))) i32 scrollkeyhit = 0;
__attribute__((visibility("hidden"))) i32 SHOPCUTCOUNT = 128;
__attribute__((visibility("hidden"))) i32 SubMenu = 0;
__attribute__((visibility("hidden"))) i32 easesubin = 0;
__attribute__((visibility("hidden"))) f32 inoutscale = 0.0f;
__attribute__((visibility("hidden"))) char *cheatname = NULL;

extern i32 ItemMenu(MENU_s *menu);
extern void DrawItemMenu2D();
extern void InitAlphaList();
extern void InitExtraList();
extern HINT_s *Hint_FindHint(i32 hint_id);
extern i16 HintTab[24];
extern void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);

i32 DoShopMenu(MENU_s *menu) {
    i32 result = 0;
    if (menuptr != NULL) {
        result = menuptr(menu);
        if (menu->cancel_pressed != 0) {
            result = currentmenulevel < 1 ? 5 : 6;
            for (i32 i = 4; i < 13; ++i) {
                menu->item_width[i] = 0.0f;
            }
            subitemselected = 0;
        }

        if (result == 5) {
            return 1;
        }
        if (result == 6) {
            GameCam_Blend(GameCam, 0.6f, 0.0f, 1);
            drawptr = NULL;

            menuptr = menuparent[currentmenulevel];
            menuparent[currentmenulevel] = NULL;
            --currentmenulevel;
            if (currentmenulevel < 0) {
                currentmenulevel = 0;
            }

            drawpanelptr = drawparent[currentdrawlevel];
            drawparent[currentdrawlevel] = NULL;
            --currentdrawlevel;
            if (currentdrawlevel < 0) {
                currentdrawlevel = 0;
            }
        }
    }
    return 0;
}

i32 UpdateShop(MENU_s *menu) {
    UpdateCharacterIDs();

    scaleoverride[0] = 0.0f;
    scaleoverride[1] = 0.4f;
    scaleoverride[2] = 0.8f;
    scaleoverride[4] = 0.8f;
    scaleoverride[5] = 0.4f;
    scaleoverride[6] = 0.0f;

    if (1.0f > ShopNameAlpha) {
        ShopNameAlpha += FRAMETIME + FRAMETIME;
        if (ShopNameAlpha > 1.0f) {
            ShopNameAlpha = 1.0f;
        }
    }

    ShopLockedScale = SeekLinearF(ShopLockedScale, 1.0f, FRAMETIME * 3.0f);
    SubNormCharPush = 0.02f;
    SubBigCharPush = 0.05f;

    if (menuptr == NULL) {
        if (shop_from_cutsceneplayer != 0) {
            shop_from_cutsceneplayer = 0;
            topscale[5] = oldpickedscale;
            menuptr = oldmenuptr;
            toppush[5] = oldpickedpush;
            drawparent[0] = olddrawparent[0];
            menuparent[0] = oldmenuparent[0];
            drawparent[1] = olddrawparent[1];
            menuparent[1] = oldmenuparent[1];
            drawparent[2] = olddrawparent[2];
            menuparent[2] = oldmenuparent[2];
            drawpanelptr = olddrawpanelptr;
            drawptr = olddrawptr;
            currentmenulevel = oldcurrentmenulevel;
            currentdrawlevel = oldcurrentdrawlevel;
            picked = oldpicked;
        } else {
            menuptr = ItemMenu;
            ExitMenu = 0;
            slidetimer = 0.125f;
            enteredshop = 1;
            currentmenulevel = 0;
            currentdrawlevel = 0;
            menuparent[0] = NULL;
            drawparent[0] = NULL;
            picked = oldpicked;
            drawpanelptr = DrawItemMenu2D;
        }
    }

    return DoShopMenu(menu) != 0;
}

i32 BuyShopItem(shopitem_s *items, i32 index, i32 charge) {
    shopitem_s *item = &items[index];
    u32 *bits = NULL;
    if (item->type == 0)
        bits = Game.shop_hint_purchased_bits;
    else if (item->type == 1)
        bits = Game.shop_character_purchased_bits;
    else if (item->type == 2)
        bits = Game.extra_unlocked_bits;
    else if (item->type == 4)
        bits = &Game.shop_gold_brick_purchased_bits;
    else if (item->type == 5) {
        PlaySfx("MenuSelect", 0);
        return 1;
    }
    if (bits != NULL)
        bits[index / 32] |= static_cast<u32>(u64(1) << (index % 32));
    if (charge != 0)
        Game.coins -= item->price;
    pickedbing = 0.35f;
    item->unlocked = 1;
    if (charge != 0)
        PlaySfx("Shop_BuyCheat", &SubShelfPos[3]);
    return 1;
}

void SelectSubItem() {
    shopitem_s *items;
    i32 index;
    switch (picked) {
        case 0:
            items = HintItems;
            index = HintShelfIds[3];
            break;
        case 1:
            items = CharItems;
            index = CharShelfIds[3];
            break;
        case 2:
            items = ExtraItems;
            index = ExtraShelfIds[3];
            break;
        case 4:
            items = BrickItems;
            index = BrickShelfIds[3];
            break;
        case 5:
            items = CutItems;
            index = CutShelfIds[3];
            break;
        default:
            return;
    }
    if (index != -1 && items != NULL) {
        const i32 result = SelectShopItem(items, index);
        if (result == -1)
            PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
        if (result == 1)
            ShopNameAlpha = 0.0f;
    }
}

i32 SelectShopItem(shopitem_s *items, i32 index) {
    shopitem_s *item = &items[index];
    char event[128];
    switch (item->type) {
        case 0: {
            const i32 id = static_cast<u16>(item->item_id);
            if (item->unlocked == 1 || item->price == 0)
                return 1;
            if (!CheckCash(items, index))
                break;
            AddToCompletionPoints(POINTS_PER_HINT);
            if (HINT_s *hint = Hint_FindHint(HintTab[id]))
                hint->flags |= HINT_SHOP_PURCHASED;
            return BuyShopItem(items, index, 1);
        }
        case 1: {
            const i32 id = static_cast<u16>(item->item_id);
            if (item->unlocked == 1 || id >= CHARCOUNT)
                return 0;
            if (!CollectIDUnlocked(id))
                goto locked;
            if (!CheckCash(items, index))
                break;
            if (!AddToCollection(id))
                return 1;
            AddToCompletionPoints(POINTS_PER_CHARACTER);
            sprintf(event, "hubshop_buychar_%s", CDataList[id].file);
            NuIOS_RecordFlurryEvent(event);
            return BuyShopItem(items, index, 1);
        }
        case 2: {
            const i32 id = static_cast<u16>(item->item_id);
            if (item->unlocked == 1 || (Game.extra_purchased_bits[id >> 5] & (1U << (id & 31))))
                return 0;
            const i32 area = static_cast<i8>(Cheat[id].area);
            if (area != -1 && !Game.area_save[area].red_brick_collected)
                goto locked;
            if (!CheckCash(items, index))
                break;
            Game.extra_purchased_bits[id >> 5] |= 1U << (id & 31);
            AddToCompletionPoints(POINTS_PER_CHEAT);
            sprintf(event, "hubshop_buyextra_%s", Cheat[id].name);
            NuIOS_RecordFlurryEvent(event);
            return BuyShopItem(items, index, 1);
        }
        case 4:
            if (item->unlocked == 1 || item->price == 0)
                return 0;
            if (static_cast<f32>(index * 3600) > Game.field30_0x7c2c)
                goto locked;
            if (!CheckCash(items, index))
                break;
            AddToCompletionPoints(POINTS_PER_GOLDBRICK);
            AddToGoldBricks();
            sprintf(event, "hubshop_buygoldbrick_%i", index + 1);
            NuIOS_RecordFlurryEvent(event);
            return BuyShopItem(items, index, 1);
        case 5:
            if (!CutScenePlayer_CanStart(index)) {
                PlaySfx("MenuNoEntry", 0);
                ShopLockedScale = 1.5f;
                return 0;
            }
            oldmenuptr = menuptr;
            for (i32 i = 0; i < 3; ++i) {
                olddrawparent[i] = drawparent[i];
                oldmenuparent[i] = menuparent[i];
            }
            olddrawpanelptr = drawpanelptr;
            olddrawptr = drawptr;
            oldcurrentmenulevel = currentmenulevel;
            oldcurrentdrawlevel = currentdrawlevel;
            oldpickedscale = topscale[5];
            oldpickedpush = toppush[5];
            menuptr = NULL;
            for (i32 i = 0; i < 3; ++i) {
                drawparent[i] = NULL;
                menuparent[i] = NULL;
            }
            currentmenulevel = currentdrawlevel = 0;
            oldpicked = picked;
            SHOPACTIVE = 0;
            BuyShopItem(items, index, 0);
            TempGame.field30_0x7c2c = Game.field30_0x7c2c;
            pickedbing = 0.0f;
            if (memcmp(&TempGame, &Game, sizeof(Game)))
                hub_forceshopsave = 1;
            CutScenePlayer_Start(index, -1);
            return 1;
        default:
            return -1;
    }
    CoinTotalScale = 1.5f;
    return -1;
locked:
    GameAudio_PlaySfx(0x32, NULL, 0, 0);
    ShopLockedScale = 1.5f;
    return 0;
}

static NUVEC HubShopPos = {-26.3f, 0.0f, -49.5f};

i32 Shop_UpdateHint(HINT_s *hint) {
    if (!Game.coins || !WORLD || WORLD->current_level != HUB_LDATA)
        return 0;
    if (GameCam->sock_position.location.sock != 0 || !player)
        return 0;
    if (!(NuVecXZDistSqr(&player->apiobj.position, &HubShopPos, NULL) < 1.0f))
        return 0;

    if (hint->control_mode_ids[0] == 0x5ea) {
        for (i32 i = 0; i < ShopCollection.count_y && i < 100; ++i) {
            if (CollectIDUnlocked(ShopCollection.list[i].id) && !Collection_Got(ShopCollection.list[i].id) &&
                Game.coins >= static_cast<u32>(ShopCollection.list[i].field3_0x4))
                return 1;
        }
    } else if (hint->control_mode_ids[0] == 0x5eb) {
        for (i32 i = 0; i < 44; ++i) {
            const i8 area = static_cast<i8>(Cheat[i].area);
            if (!(Game.extra_purchased_bits[i >> 5] >> (i & 31) & 1) &&
                (area == -1 || Game.area_save[area].red_brick_collected) &&
                Game.coins >= static_cast<u32>(Cheat[i].extra_price))
                return 1;
        }
    } else if (hint->control_mode_ids[0] == 0x5ec) {
        for (i32 i = 0; i < SHOPGOLDBRICKS; ++i) {
            if (!(static_cast<u64>((&Game.shop_gold_brick_purchased_bits)[i >> 5]) >> (i & 31) & 1) &&
                static_cast<f32>(i * 3600) <= Game.field30_0x7c2c &&
                Game.coins >= static_cast<u32>(BrickItems[i].price))
                return 1;
        }
    }
    return 0;
}

void BuyAllShopExtras() {
    WORLDINFO_s *world = WORLD;
    shopitem_s *item = ExtraItems;
    for (i32 i = 0; i < 44; ++i, ++item) {
        Game.purchased_extra_bits[i >> 5] |= 1u << (i & 31);
        if (world->current_level == HUB_LDATA)
            item->unlocked = 1;
        i32 area = static_cast<i8>(Cheat[i].area);
        if (area != -1 && Game.area_save[area].field_0x5[1] == 0)
            Game.area_save[area].field_0x5[1] = 1;
    }
    Game.unlocked_extra_bits[0] = 0xffffffff;
    Game.unlocked_extra_bits[1] = 0xffffffff;
}

void GetShopCamLookPos(nuvec_s *position) {
    if (menuptr == ItemMenu && splshelf != NULL && picked != -1)
        *position = splshelf->pts[picked];
    else
        *position = *shopcamlookat;
}

i32 MoveSubItemsRight(i32 *ids, NUVEC *positions, i32 count) {
    if (slidetimer > 0.0f) {
        const f32 phase = 1.0f - slidetimer * 8.0f;
        const i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
        f32 t = 1.0f - (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
        for (i32 i = 1; i < 7; ++i) {
            t = NuFmax(0.0f, NuFmin(t, 1.0f));
            positions[i].x = SubShelfPos[i].x + (SubShelfPos[i - 1].x - SubShelfPos[i].x) * t;
            positions[i].y = SubShelfPos[i].y + (SubShelfPos[i - 1].y - SubShelfPos[i].y) * t;
            positions[i].z = SubShelfPos[i].z + (SubShelfPos[i - 1].z - SubShelfPos[i].z) * t;
        }
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale3 = scalepicked + (scalenorm - scalepicked) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale4 = scalenorm + (scalepicked - scalenorm) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[1] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[2] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * t;
        scaleoverride[6] = phase;
        scaleoverride[0] = 1.0f - phase;
        return 0;
    }
    for (i32 i = 0; i < 6; ++i)
        ids[i] = ids[i + 1];
    ids[6] = ids[5] + 1;
    if (ids[6] >= count)
        ids[6] = 0;
    for (i32 i = 0; i < 7; ++i)
        positions[i] = SubShelfPos[i];
    slidetimer = 0.0f;
    scale2 = scalenorm;
    scale3 = scalepicked;
    scale4 = scalenorm;
    subpush[0] = SubNormCharPush;
    subpush[1] = SubBigCharPush;
    subpush[2] = SubNormCharPush;
    --moveitems;
    scaleoverride[0] = 0.0f;
    scaleoverride[1] = 0.4f;
    scaleoverride[2] = 0.8f;
    scaleoverride[4] = 0.8f;
    scaleoverride[5] = 0.4f;
    scaleoverride[6] = 0.0f;
    return 1;
}

i32 MoveSubItemsLeft(i32 *ids, NUVEC *positions, i32 count) {
    if (slidetimer > 0.0f) {
        const f32 phase = 1.0f - slidetimer * 8.0f;
        const i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
        f32 t = 1.0f - (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
        for (i32 i = 0; i < 6; ++i) {
            t = NuFmax(0.0f, NuFmin(t, 1.0f));
            positions[i].x = SubShelfPos[i].x + (SubShelfPos[i + 1].x - SubShelfPos[i].x) * t;
            positions[i].y = SubShelfPos[i].y + (SubShelfPos[i + 1].y - SubShelfPos[i].y) * t;
            positions[i].z = SubShelfPos[i].z + (SubShelfPos[i + 1].z - SubShelfPos[i].z) * t;
        }
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale2 = scalenorm + (scalepicked - scalenorm) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale3 = scalepicked + (scalenorm - scalepicked) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[0] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[1] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * t;
        scaleoverride[0] = phase;
        scaleoverride[6] = 1.0f - phase;
        return 0;
    }
    for (i32 i = 6; i > 0; --i)
        ids[i] = ids[i - 1];
    ids[0] = ids[1] - 1;
    if (ids[0] < 0)
        ids[0] = count - 1;
    for (i32 i = 0; i < 7; ++i)
        positions[i] = SubShelfPos[i];
    slidetimer = 0.0f;
    scale2 = scalenorm;
    scale3 = scalepicked;
    scale4 = scalenorm;
    subpush[0] = SubNormCharPush;
    subpush[1] = SubBigCharPush;
    subpush[2] = SubNormCharPush;
    ++moveitems;
    scaleoverride[0] = 0.0f;
    scaleoverride[1] = 0.4f;
    scaleoverride[2] = 0.8f;
    scaleoverride[4] = 0.8f;
    scaleoverride[5] = 0.4f;
    scaleoverride[6] = 0.0f;
    return 1;
}

void Shop_CollectAllCharacters(i32 mode) {
    if (mode != 0)
        return;
    const i32 count = ShopCollection.count_y;
    for (i32 i = 0; i != count && i != 100; ++i) {
        Game.shop_character_purchased_bits[i >> 5] |= static_cast<u32>(u64(1) << (i & 31));
        if (WORLD->current_level == HUB_LDATA)
            CharItems[i].unlocked = SAVE_ON;
    }
}

void InitShop(WORLDINFO_s *world) {
    CharItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 100 * sizeof(shopitem_s)));
    HintItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 64 * sizeof(shopitem_s)));
    ExtraItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 64 * sizeof(shopitem_s)));
    CodeItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 64 * sizeof(shopitem_s)));
    BrickItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 14 * sizeof(shopitem_s)));

    if (CharItems == NULL || HintItems == NULL || ExtraItems == NULL || CodeItems == NULL || BrickItems == NULL) {
        return;
    }

    TopShelf[0].type = 0;
    NuStrCpy(TopShelf[0].name, "Hint");
    NuStrCpy(TopShelf[0].special_name, "info");
    NuSpecialFind(things_scene, &TopShelf[0].special, "info", 1);

    TopShelf[1].type = 1;
    TopShelf[1].name[0] = '\0';
    TopShelf[1].special_name[0] = '\0';
    memset(&TopShelf[1].special, 0, sizeof(TopShelf[1].special));

    TopShelf[2].type = 2;
    NuStrCpy(TopShelf[2].name, "Extra");
    NuStrCpy(TopShelf[2].special_name, "tool_box");
    NuSpecialFind(world->current_gscn, &TopShelf[2].special, "tool_box", 1);

    TopShelf[3].type = 3;
    NuStrCpy(TopShelf[3].name, "Code");
    NuStrCpy(TopShelf[3].special_name, "shop_question");
    NuSpecialFind(world->current_gscn, &TopShelf[3].special, "shop_question", 1);

    TopShelf[4].type = 4;
    NuStrCpy(TopShelf[4].name, "Gold Bricks");
    NuStrCpy(TopShelf[4].special_name, "gold_brick");
    NuSpecialFind(things_scene, &TopShelf[4].special, "gold_brick", 1);

    TopShelf[5].type = 5;
    NuStrCpy(TopShelf[5].name, "Cut Scenes");
    NuStrCpy(TopShelf[5].special_name, "FMV");
    NuSpecialFind(world->current_gscn, &TopShelf[5].special, "fmv", 1);

    memset(codelist, 0, sizeof(codelist));
    SHOPCHARCOUNT = 0;
    i32 code_count = 0;
    for (i32 i = 0; i < ShopCollection.count_y && i < 100; ++i) {
        COLLECTID *collect = &ShopCollection.list[i];
        shopitem_s *item = &CharItems[SHOPCHARCOUNT];
        item->unlocked = 0;
        item->type = 1;

        if (((static_cast<u64>(Game.shop_character_purchased_bits[SHOPCHARCOUNT >> 5]) >> (SHOPCHARCOUNT & 0x1f)) &
             1) != 0) {
            item->unlocked = 1;
        }

        const i32 character_id = collect->id;
        if (CDataList[character_id].name_id == -1) {
            NuStrCpy(item->name, CDataList[character_id].dir);
        } else {
            NuStrCpy(item->name, TTab[CDataList[character_id].name_id]);
        }
        memset(&item->special, 0, sizeof(item->special));

        if (code_count <= 143) {
            codelist[code_count] = CRC_ProcessStringIgnoreCase(collect->cheat_code);
            ++code_count;
        }
        ++SHOPCHARCOUNT;
    }

    UpdateCharacterIDs();
    charcheatix = code_count;
    for (i32 i = 0; i < 44; ++i) {
        if (code_count < 144)
            codelist[code_count++] = CRC_ProcessStringIgnoreCase(Cheat[i].code);
    }
    extracheatix = code_count;
    NuSpecialFind(things_scene, &iconback, "icon_back_neutral", 1);

    NuSpecialFind(WORLD->current_gscn, &infoblank, "info_blank", 1);
    NuSpecialFind(WORLD->current_gscn, &cutblank, "fmv_blank", 1);
    NuSpecialFind(WORLD->current_gscn, &cutfilm_unlocked, "shop_film1", 1);
    NuSpecialFind(WORLD->current_gscn, &cutfilm_locked, "shop_film1b", 1);
    NuSpecialFind(WORLD->current_gscn, &toolblank, "tool_blank", 1);
    NuSpecialFind(WORLD->current_gscn, &codeblank, "code_blank", 1);
    NuSpecialFind(things_scene, &question, "question_icon", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow1, "shop_arrow1", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow2, "shop_arrow2", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow3, "shop_arrow3", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow4, "shop_arrow4", 1);
    NuSpecialSetVisibility(&arrow1, 0);
    NuSpecialSetVisibility(&arrow2, 0);
    NuSpecialSetVisibility(&arrow3, 0);
    NuSpecialSetVisibility(&arrow4, 0);

    InitExtraList();
    InitAlphaList();

    for (i32 i = 0; i < SHOPHINTCOUNT; ++i) {
        HINT_s *hint = Hint_FindHint(HintTab[i]);
        shopitem_s *item = &HintItems[i];
        item->item_id = i;
        item->type = 0;
        item->price = hint != NULL ? hint->shop_price : 0;
        item->unlocked = SAVE_OFF;
        if ((static_cast<u64>(Game.shop_hint_purchased_bits[i >> 5]) >> (i & 31)) & 1)
            item->unlocked = SAVE_ON;
        if (item->price == 0) {
            item->unlocked = 1;
        }
        item->special = atoz0to9icon[i];
    }

    shopcamspline = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shop_cam"));
    if (shopcamspline == NULL) {
        return;
    }
    shopcampos = shopcamspline->pts;
    shopcamlookat = shopcamspline->pts + 1;
    LoadShelfSplines();

    if (SHOPCHARCOUNT > 0) {
        for (i32 i = 0; i < 7; ++i) {
            CharShelfIds[i] = (SHOPCHARCOUNT + i - 3) % SHOPCHARCOUNT;
            CharCurPos[i] = SubShelfPos[i];
        }
    }
    if (SHOPHINTCOUNT > 0) {
        for (i32 i = 0; i < 7; ++i) {
            HintShelfIds[i] = (SHOPHINTCOUNT + i - 3) % SHOPHINTCOUNT;
            HintCurPos[i] = SubShelfPos[i];
        }
    }
    if (SHOPEXTRACOUNT > 0) {
        for (i32 i = 0; i < 7; ++i) {
            ExtraShelfIds[i] = (SHOPEXTRACOUNT + i - 3) % SHOPEXTRACOUNT;
            ExtraCurPos[i] = SubShelfPos[i];
        }
    }

    const i32 brick_shelf_ids[7] = {11, 12, 13, 0, 1, 2, 3};
    for (i32 i = 0; i < 7; ++i) {
        BrickShelfIds[i] = brick_shelf_ids[i];
        BrickCurPos[i] = SubShelfPos[i];
    }

    for (i32 i = 0; i < SHOPGOLDBRICKS; ++i) {
        shopitem_s *item = &BrickItems[i];
        item->item_id = i;
        item->type = 4;
        item->price = 10000 + i * 5000;
        item->unlocked = SAVE_OFF;
        if ((static_cast<u64>((&Game.shop_gold_brick_purchased_bits)[i >> 5]) >> (i & 31)) & 1)
            item->unlocked = SAVE_ON;
        item->special = TopShelf[4].special;
    }

    SHOPCUTCOUNT = CutScenePlayCount;
    for (i32 i = 0; i < CutScenePlayCount && i < 128; ++i) {
        shopitem_s *item = &CutItems[i];
        item->item_id = i;
        item->type = 5;
        item->price = 0;
        item->unlocked = 0;
        item->special = TopShelf[5].special;
    }

    if (shop_from_cutsceneplayer == 0 && CutScenePlayCount > 0) {
        for (i32 i = 0; i < 7; ++i) {
            CutShelfIds[i] = (CutScenePlayCount + i - 3) % CutScenePlayCount;
            CutCurPos[i] = SubShelfPos[i];
        }
    }

    const f32 shelf_scale[6] = {0.9f, 0.28f, 0.9f, 0.95f, 0.8f, 0.9f};
    const f32 shelf_push[6] = {0.045f, 0.045f, 0.045f, 0.05f, 0.015f, 0.045f};
    const f32 current_push[6] = {0.0f, 0.0f, -0.005f, 0.0f, -0.01f, 0.0f};
    memcpy(TopShelfScale, shelf_scale, sizeof(TopShelfScale));
    memcpy(topscale, shelf_scale, sizeof(topscale));
    memcpy(TopShelfPush, shelf_push, sizeof(TopShelfPush));
    memcpy(TopBigPush, current_push, sizeof(TopBigPush));
    memcpy(toppush, current_push, sizeof(toppush));
    const f32 big_scale[6] = {1.44f, 0.448f, 1.44f, 1.5675f, 1.35f, 1.44f};
    memcpy(TopBigScale, big_scale, sizeof(TopBigScale));
    SubBigCharPush = 0.05f;
    SubNormCharPush = 0.02f;
    subpush[0] = subpush[2] = SubNormCharPush;
    subpush[1] = SubBigCharPush;
    for (i32 i = 0; i < 6; ++i) {
        codebigscale[i] = 1.254f;
        codeshelfscale[i] = codemenuscale[i] = 0.76f;
    }
    picked = oldpicked;
    subpicked = 0;
    scalenorm = 1.0f;
    scalepicked = 1.4f;
    scale2 = scale4 = scalenorm;
    scale3 = scalepicked;
    shopcutsceneplayer = CutScenePlayer_Available();
}

i32 CheckCash(shopitem_s *items, i32 item) {
    return Game.coins >= items[item].price;
}
