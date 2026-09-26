#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/render/light/fade_material.h"
#include "nu2api/nu3d/nuspline.h"

#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>
#include <stdio.h>

struct GameObject_s;
struct LEVEL_PROGRESS_s;
struct WORLDINFO_s;
i32 NuIOS_GetPurchaseResult();
int NuIOS_PurchaseInAppProduct(char *);
i32 NuIOS_IsProductPurchased(char *);
struct NuIOS_InAppProduct;
i32 NuIOS_GetInAppProductByID(char *, NuIOS_InAppProduct *);
void NuIOS_RestoreInAppPurchases();
i32 NuIOS_AreInAppPurchasesAvailable();
i32 NuIOS_CanMakeInAppPurchases();
extern "C" void NuIOS_RecordFlurryEvent(char *);
extern "C" void BackupMenu();
extern i32 CutInstEndCount;
i32 LEGOMENU_PAUSESYNC = -1;
i32 LEGOMENU_STORE_PURCHASE = 0x17;
extern NuVec2 StoreTouchLastPos __asm__("_ZN28MechInputTouchMenuController12LastTouchPosE");
void GameCam_HitRoll();
void GameDrawMenuEntry(MENU_s *, char *);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void DrawRectRGBA(f32, f32, f32, f32, u32, numtl_s *, i32, f32);
extern i16 tCONTINUE;
extern i16 tACCEPT;
extern u8 RAP_WARNING_R, RAP_WARNING_G, RAP_WARNING_B;

struct STOREIAP_s {
    f32 x;
    f32 y;
    f32 title_y;
    f32 bottom_y;
    f32 width;
    char text[256];
};
DECOMP_ASSERT(sizeof(STOREIAP_s) == 0x114, "STOREIAP_s size");
struct STORE_PRODUCT_s {
    char name[256];
    char description[256];
    char price_text[256];
    f32 price;
};
DECOMP_ASSERT(sizeof(STORE_PRODUCT_s) == 0x304, "STORE_PRODUCT_s size");
static STOREIAP_s StoreIAP[3];
extern i32 menu_i_pack;
i32 menu_i_bundle = -1;
static char *menu_storepurchase_iap_name;
i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode);
extern f32 MENUENTRYEXWIDTH;

static void StoreUnlockArcade();
static void StoreUnlockBonus();
static void StoreUnlockBounty();
static void StoreUnlockChallenge();
static void StoreUnlockJedi();
static void StoreUnlockSith();
static void StoreUnlockEp3();
static void StoreUnlockEp2();
static void StoreUnlockEp6();
static void StoreUnlockEp5();
static void StoreUnlockEp4();

extern "C" {
    extern i16 id_DEXTER, id_WOOKIEE, id_TUSKENRAIDER, id_LOBOT, id_GAMORREANGUARD;
    extern i16 id_BOSSNASS, id_BIBFORTUNA, id_ADMIRALACKBAR;
}

STOREPACK StorePack[11] = {
    {const_cast<char *>("ep2"),
     {const_cast<char *>("EPISODE2")},
     {0x16b},
     0,
     3,
     StoreUnlockEp2,
     const_cast<char *>("episode_ii_door_to_map"),
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_DEXTER,
     {0.0f, 0.0f, 0.0f},
     0,
     0,
     0},
    {const_cast<char *>("ep3"),
     {const_cast<char *>("EPISODE3")},
     {0x16c},
     0,
     3,
     StoreUnlockEp3,
     const_cast<char *>("episode_iii_door_to_map"),
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_WOOKIEE,
     {0.0f, 0.0f, 0.0f},
     0,
     0,
     0},
    {const_cast<char *>("ep4"),
     {const_cast<char *>("EPISODE4")},
     {0x16d},
     0,
     3,
     StoreUnlockEp4,
     const_cast<char *>("episode_iv_door_to_map"),
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_TUSKENRAIDER,
     {0.0f, 0.0f, 0.0f},
     0,
     0,
     0},
    {const_cast<char *>("ep5"),
     {const_cast<char *>("EPISODE5")},
     {0x16e},
     0,
     3,
     StoreUnlockEp5,
     const_cast<char *>("episode_v_door_to_map"),
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_LOBOT,
     {0.0f, 0.0f, 0.0f},
     0,
     0,
     0},
    {const_cast<char *>("ep6"),
     {const_cast<char *>("EPISODE6")},
     {0x16f},
     0,
     3,
     StoreUnlockEp6,
     const_cast<char *>("episode_vi_door_to_map"),
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_GAMORREANGUARD,
     {0.0f, 0.0f, 0.0f},
     0,
     0,
     0},
    {const_cast<char *>("arcade"),
     {const_cast<char *>("ARCADE")},
     {0x170},
     0,
     2,
     StoreUnlockArcade,
     const_cast<char *>("door_to_network"),
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_SUPERBATTLEDROID,
     {0.0f, 0.0f, 0.0f},
     0,
     0,
     0},
    {const_cast<char *>("bonus"),
     {const_cast<char *>("BONUS")},
     {0x171},
     0,
     2,
     StoreUnlockBonus,
     const_cast<char *>("door_to_bonus"),
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_BOSSNASS,
     {0.0f, 0.0f, 0.0f},
     0,
     0,
     0},
    {const_cast<char *>("bounty"),
     {const_cast<char *>("BOUNTY")},
     {0x172},
     0,
     2,
     StoreUnlockBounty,
     const_cast<char *>("door_from_out2"),
     const_cast<char *>("JunkyardIdle"),
     0.5f,
     NULL,
     &id_BIBFORTUNA,
     {0.0f, 0.0f, 0.0f},
     0,
     7,
     0},
    {const_cast<char *>("challenge"),
     {const_cast<char *>("CHALLENGEPACK")},
     {0x173},
     0,
     2,
     StoreUnlockChallenge,
     NULL,
     const_cast<char *>("MainRoomIdle"),
     0.0f,
     NULL,
     &id_ADMIRALACKBAR,
     {-28.9f, 0.0f, -48.0f},
     0xcc16,
     0,
     0},
    {const_cast<char *>("jedi"),
     {const_cast<char *>("JEDIPACK")},
     {0x174},
     0,
     1,
     StoreUnlockJedi,
     NULL,
     const_cast<char *>("JunkyardIdle"),
     0.0f,
     NULL,
     &id_SHAAKTI,
     {-29.3f, 0.0f, -34.9f},
     0x2000,
     7,
     0},
    {const_cast<char *>("sith"),
     {const_cast<char *>("EMPIREPACK")},
     {0x175},
     0,
     1,
     StoreUnlockSith,
     NULL,
     const_cast<char *>("JunkyardIdle"),
     0.0f,
     NULL,
     &id_THEEMPEROR,
     {-31.0f, 0.0f, -34.1f},
     0x4000,
     7,
     0},
};
STOREBUNDLE StoreBundle[3] = {
    {const_cast<char *>("PREQUELPACK"), 0x103, 0x5fe},
    {const_cast<char *>("ORGINALPACK"), 0x1c, 0x5ff},
    {const_cast<char *>("COMPLETEPACK"), 0xffffffffu, 0x600},
};

static void StoreUnlockArcade() {
}
static void StoreUnlockBonus() {
}
static void StoreUnlockBounty() {
}
static void StoreUnlockChallenge() {
    HUB_AREAPANELX = HUB_AREAPANELX_ONETRUEJEDIGOLDBRICK;
}
static void StoreUnlockJedi() {
}
static void StoreUnlockSith() {
}
static void StoreUnlockEp3() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[2].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(2);
        }
    }
}
static void StoreUnlockEp2() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[1].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(1);
        }
    }
}
static void StoreUnlockEp6() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[5].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(5);
        }
    }
}
static void StoreUnlockEp5() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[4].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(4);
        }
    }
}
static void StoreUnlockEp4() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[3].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(3);
        }
    }
}

void (*Game_100PercentFn)();
void (*Game_AllGoldBricksFn)();
u16 restoring_pack_bits;
u8 restoring_pack_count;
u8 restoring_pack_list[16];
u16 restoring_bundle_bits;
u8 restoring_bundle_count;
u8 restoring_bundle_list[16];
f32 restoring_wait = 3.0f;

void PauseGame(i32 pad_index) {
    Paused = 1;
    music_man.SetFader(0.0f, 0.5f);
    music_man.PauseTrack(0x10);
    NuSound3StopRumble();

    if (memcard_autosaveenabled != 0 && memcard_autosavedisabled != 0) {
        NewMenu(0x3f3, 0, -1);
    } else if (CUTSTOPGAME != 0) {
        NewMenu(LEGOMENU_PAUSECUT, 0, -1);
    } else {
        NewMenu(LEGOMENU_PAUSEMAIN, 0, -1);
    }

    ResetTimer(&PauseTimer, 0.0f);
    GameAudio_PlaySfx(0x36, NULL, 0, 0);
    DoubleScoreTime = 0.0f;
    ResetTimer(&JoinInTimer, 0.0f);
    pause_i_pad = pad_index;

    for (i32 i = 0; i < 8; ++i) {
        if (Player[i] != NULL) {
            Player[i]->hud_icon_timer = 0.0f;
            Player[i]->pause_context_state = 0;
            Player[i]->input_toggle_hold_time = TOGGLEHOLDTIME;
        }
    }

    if (PauseGame_ExtraCodeFn != NULL) {
        PauseGame_ExtraCodeFn();
    }
}

void NetworkSyncPause() {
    if (Paused == 0) {
        NeedScreenGrab(1);
    }
    Paused = 1;
    CutInstEndCount = 0;
    NewMenu(LEGOMENU_PAUSESYNC, 0, -1);
    ResetTimer(&PauseTimer, 0.0f);
    music_man.SetFader(0.0f, 0.5f);
    music_man.PauseTrack(0x10);
    NuSound3StopRumble();
    DoubleScoreTime = 0.0f;
    ResetTimer(&JoinInTimer, 0.0f);
    const f32 hold_time = TOGGLEHOLDTIME;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *player = Player[i];
        if (player != NULL) {
            player->pause_input_state = 0;
            player->pause_context_state = 0;
            player->input_toggle_hold_time = hold_time;
        }
    }
    if (PauseGame_ExtraCodeFn != NULL) {
        PauseGame_ExtraCodeFn();
    }
}

void ResumeGame(i32 play_sound, i32 resume_music) {
    Paused = 0;
    NetPaused = 0;
    MenuRememberCursor(&GameMenu[GameMenuLevel]);
    MenuReset();
    music_man.SetFader(1.0f, 0.5f);
    if (resume_music != 0) {
        music_man.ResumeTrack(0x10);
    }
    if (play_sound != 0) {
        GameAudio_PlaySfx(0x37, NULL, 0, 0);
    }
    if (ResumeGame_ExtraCodeFn != NULL) {
        ResumeGame_ExtraCodeFn();
    }
}

void ClearPause() {
    Paused = 0;
    NetPaused = 0;
}

void NewGameMode() {
    NewMode = 1;
    reset_load = 1;
}

void RestoreOptions() {
    GameSetSoundVolume(Game_OptionsSave);
    GameSetMusicVolume(Game_OptionsSave);

    if (Game_OptionsSave != NULL) {
        NuVideoSetBrightness(static_cast<f32>(Game_OptionsSave->field12_0xc) / 10.0f);
        MechSystems::Get()->input_touch_system.control_mode = SuperOptions.touch_controls == 0 ? 1 : 2;
    }
}

void InitSuperStory(i32 episode) {
    SuperStory = 1;
    SuperStoryEpisode = episode;
    ResetTimer(reinterpret_cast<TIMER *>(SuperStoryTimer), 0.0f);
    SuperStoryScore = 0;
    FreePlay = 0;
    NextArea_FreePlay = 0;
    Cheats_TurnOff(0);
}

i32 InStory() {
    if (FreePlay != 0 || ChallengeMode != 0 || Mission_Active(NULL) != NULL || Arcade != 0) {
        return 0;
    }
    return 1;
}

i32 Game_Exit(i32) {
    return 0;
}

LEVELDATA_s *CanSaveAndExit(WORLDINFO_s *world) {
    if (GAMEDEMO == 0 && SuperStory == 0 && world->area != NULL && world->area != HUB_ADATA &&
        (world->area->flags & 0x146) == 0 && Mission_Active(NULL) == NULL && ChallengeMode == 0 && Arcade == 0 &&
        CutScenePlayer_Active() == NULL && Game_AreaSave != NULL &&
        Game_AreaSave[world->level_sub_id].area_complete != 0 && AreaGlobals.values.field_0x18 > 0) {
        return Area_FindStatusLevel(world->area, NULL);
    }

    return NULL;
}

void AddToCompletionPoints(u32 points) {
    STATUSCOLLECT_s *save = reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave);
    const i32 maximum = COMPLETIONPOINTS;
    if (save != NULL && save->completion_points < maximum) {
        save->completion_points += points;
        if (save->completion_points >= maximum) {
            save->completion_points = maximum;
            if ((save->flags & SAVE_REWARD_100_PERCENT) == 0) {
                if (Game_100PercentFn != NULL) {
                    Game_100PercentFn();
                    save = reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave);
                }
                save->flags |= SAVE_REWARD_100_PERCENT;
            }
        }
    }
}

i32 Game_100PercentComplete() {
    if (Game_CompletionSave == NULL) {
        return 0;
    }
    return reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave)->flags & SAVE_REWARD_100_PERCENT;
}

void AddToGoldBricks() {
    STATUSCOLLECT_s *save = reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave);
    const i32 points = GOLDBRICKPOINTS;
    if (save != NULL && save->gold_bricks < points) {
        ++save->gold_bricks;
        if (save->gold_bricks == points && (save->flags & SAVE_REWARD_ALL_GOLD_BRICKS) == 0) {
            if (Game_AllGoldBricksFn != NULL)
                Game_AllGoldBricksFn();
            reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave)->flags |= SAVE_REWARD_ALL_GOLD_BRICKS;
        }
    }
}

i32 Game_GotAllGoldBricks() {
    if (Game_CompletionSave == NULL) {
        return 0;
    }
    return (reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave)->flags & SAVE_REWARD_ALL_GOLD_BRICKS) != 0;
}

i32 Game_AutoSaving() {
    if (memcard_autosaveneeded != 0) {
        return 1;
    }
    return memcard_autosaveinprogress != 0;
}

bool FreePlayUnlocked() {
    return true;
}

void Store_UnlockPack(i32, bool save) {
    if (save) {
        TriggerExtraDataSave();
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA)
            TriggerAutoSave();
    }
}

void Store_RestorePurchases() {
    if (WORLD != NULL && HUB_LDATA != NULL && WORLD->current_level == HUB_LDATA) {
        TriggerExtraDataSave();
        TriggerAutoSave();
    } else {
        TriggerExtraDataSave();
    }
}

i32 StoreBundle_FindByName(char *name) {
    STOREBUNDLE *bundles = StoreBundle;
    i32 result;
    char *second_name;
    char *third_name;
    if (__builtin_expect(NuStrICmp(bundles[0].name, name) == 0, 0)) {
        result = 0;
        goto done;
    }
    second_name = bundles[1].name;
    if (__builtin_expect(NuStrICmp(second_name, name) == 0, 0)) {
        result = 1;
        goto done;
    }
    third_name = bundles[2].name;
    result = NuStrICmp(third_name, name) == 0 ? 2 : -1;
done:
    return result;
}

bool Store_IsPackUnlocked(i32) {
    return 1;
}

bool Store_IsPackAvailable(i32, char *reason) {
    if (reason != NULL) {
        *reason = '\0';
    }
    if (memcard_autosavestarted != 0 || memcard_autosavepostdelay > 0.0f) {
        return false;
    }
    return memcard_autosavepredelay <= 0.0f;
}

i32 Store_FindPack(i32 id, char *name) {
    if (id == -1) {
        if (name == NULL) {
            return -1;
        }

        if (NuStrCmp(StorePack[0].name, name) == 0) {
            return 0;
        } else if (NuStrCmp(StorePack[1].name, name) == 0) {
            return 1;
        } else if (NuStrCmp(StorePack[2].name, name) == 0) {
            return 2;
        } else if (NuStrCmp(StorePack[3].name, name) == 0) {
            return 3;
        } else if (NuStrCmp(StorePack[4].name, name) == 0) {
            return 4;
        } else if (NuStrCmp(StorePack[5].name, name) == 0) {
            return 5;
        } else if (NuStrCmp(StorePack[6].name, name) == 0) {
            return 6;
        } else if (NuStrCmp(StorePack[7].name, name) == 0) {
            return 7;
        } else if (NuStrCmp(StorePack[8].name, name) == 0) {
            return 8;
        } else if (NuStrCmp(StorePack[9].name, name) == 0) {
            return 9;
        } else if (NuStrCmp(StorePack[10].name, name) == 0) {
            return 10;
        } else {
            return -1;
        }
    }

    if (name == NULL) {
        if (StorePack[0].id != NULL && *StorePack[0].id == id) {
            return 0;
        } else if (StorePack[1].id != NULL && *StorePack[1].id == id) {
            return 1;
        } else if (StorePack[2].id != NULL && *StorePack[2].id == id) {
            return 2;
        } else if (StorePack[3].id != NULL && *StorePack[3].id == id) {
            return 3;
        } else if (StorePack[4].id != NULL && *StorePack[4].id == id) {
            return 4;
        } else if (StorePack[5].id != NULL && *StorePack[5].id == id) {
            return 5;
        } else if (StorePack[6].id != NULL && *StorePack[6].id == id) {
            return 6;
        } else if (StorePack[7].id != NULL && *StorePack[7].id == id) {
            return 7;
        } else if (StorePack[8].id != NULL && *StorePack[8].id == id) {
            return 8;
        } else if (StorePack[9].id != NULL && *StorePack[9].id == id) {
            return 9;
        } else if (StorePack[10].id != NULL && *StorePack[10].id == id) {
            return 10;
        } else {
            return -1;
        }
    }

    if ((StorePack[0].id != NULL && *StorePack[0].id == id) || NuStrCmp(StorePack[0].name, name) == 0) {
        return 0;
    } else if ((StorePack[1].id != NULL && *StorePack[1].id == id) || NuStrCmp(StorePack[1].name, name) == 0) {
        return 1;
    } else if ((StorePack[2].id != NULL && *StorePack[2].id == id) || NuStrCmp(StorePack[2].name, name) == 0) {
        return 2;
    } else if ((StorePack[3].id != NULL && *StorePack[3].id == id) || NuStrCmp(StorePack[3].name, name) == 0) {
        return 3;
    } else if ((StorePack[4].id != NULL && *StorePack[4].id == id) || NuStrCmp(StorePack[4].name, name) == 0) {
        return 4;
    } else if ((StorePack[5].id != NULL && *StorePack[5].id == id) || NuStrCmp(StorePack[5].name, name) == 0) {
        return 5;
    } else if ((StorePack[6].id != NULL && *StorePack[6].id == id) || NuStrCmp(StorePack[6].name, name) == 0) {
        return 6;
    } else if ((StorePack[7].id != NULL && *StorePack[7].id == id) || NuStrCmp(StorePack[7].name, name) == 0) {
        return 7;
    } else if ((StorePack[8].id != NULL && *StorePack[8].id == id) || NuStrCmp(StorePack[8].name, name) == 0) {
        return 8;
    } else if ((StorePack[9].id != NULL && *StorePack[9].id == id) || NuStrCmp(StorePack[9].name, name) == 0) {
        return 9;
    } else if ((StorePack[10].id != NULL && *StorePack[10].id == id) || NuStrCmp(StorePack[10].name, name) == 0) {
        return 10;
    } else {
        return -1;
    }
}

void Store_HubInitFloorTargets(WORLDINFO_s *world) {
    for (i32 i = 0; i < 11; ++i) {
        STOREPACK &pack = StorePack[i];
        pack.floor_target_door = Door_FindByName(world, pack.floor_target_door_name);
        if (pack.floor_target_door != NULL) {
            NUVEC *points = pack.floor_target_door->spline->pts;
            NUVEC direction;
            NuVecSub(&direction, &points[6], &points[4]);
            pack.custodian_angle = static_cast<u16>(NuAtan2D(direction.x, direction.z) + 0x4000);
            NuVecAdd(&pack.custodian_position, &points[4], &points[6]);
            NuVecScale(&pack.custodian_position, &pack.custodian_position, 0.5f);
            if (pack.offset_floor_target != 0) {
                pack.custodian_position.x += NU_SIN_LUT(pack.custodian_angle) * 0.5f;
                pack.custodian_position.z += NU_COS_LUT(pack.custodian_angle) * 0.5f;
            }
        }
        pack.custodian_position.y = NewShadowEx(&pack.custodian_position, 0, 1.0f, 1.0f, 0) + 0.005f;
    }
}

void Store_HubDrawFloorTargets(WORLDINFO_s *world) {
    (void)NuFmod(GameTimer.time_elapsed, 4.0f);
    const u16 alpha_angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 65536.0f);
    const f32 frame_alpha = NU_SIN_LUT(alpha_angle) * 0.2f + 0.8f;

    for (i32 i = 0; i < 11; ++i) {
        STOREPACK &pack = StorePack[i];
        if (pack.floor_target_door == NULL && (pack.id == NULL || *pack.id == -1)) {
            continue;
        }
        if (Store_IsPackUnlocked(i) ||
            (pack.field44_0x32 != 0xff && pack.field44_0x32 != GameCam->sock_position.location.sock)) {
            continue;
        }

        const u16 frame_rotation = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.5f) / 1.5f * 65536.0f);
        Draw3DObjectAlpha(world, LEGOOBJ_ICON_FRAME_GREEN, &pack.custodian_position, 0x4000, frame_rotation, 0, 0.9f,
                          0.9f, 0.9f, 0, frame_alpha);

        if (g_lowEndLevelBehaviour == 0 || Hub_LowEnd_IconsInsteadOfModels == 0 || pack.id == NULL ||
            APICharacterLoaded(*pack.id) != NULL || big_icon_scene == NULL) {
            continue;
        }

        const i32 object_id = CDataList[*pack.id].field20_0x42;
        if (object_id == -1) {
            continue;
        }

        NUVEC position = pack.custodian_position;
        position.x +=
            NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed, 2.3f) / 2.3f * 65536.0f + i * 0x2000)) * 0.01f;
        position.y += CDataList[*pack.id].bounds_max_y * 0.75f - 0.01f;
        position.y +=
            NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed, 2.0f) * 0.5f * 65536.0f + i * 0x2aaa)) * 0.01f;
        position.z +=
            NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed, 2.4f) / 2.4f * 65536.0f + i * 0x2000 + 0x4000)) *
            0.01f;

        const u16 facing = NuAtan2D(position.x - GameCam->pos.x, position.z - GameCam->pos.z);
        nuhspecial_s icon_special;
        if (NuSpecialFind(big_icon_scene, &icon_special, ObjTabList[object_id].name, 1) != 0) {
            Draw3DObjectAlpha(world, object_id, &position, 0, facing, 0, 0.45f, 0.45f, 0.45f, 0, 1.0f);
        }
        Draw3DObjectAlpha(world, 0xa7, &position, 0, facing, 0, 0.45f, 0.45f, 0.45f, 0, 1.0f);
    }
}

void MenuUpdateDebugStore(MENU_s *menu) {
    if (menu->cancel_pressed != 0) {
        BackupMenu();
    } else if (menu->confirm_pressed != 0) {
        if (__builtin_expect(menu->selected_item > 10, 0)) {
            SuperOptions.store_pack_flags = 0xffff;
            SuperOptions.store_bundle_flags = 0xff;
            TriggerExtraDataSave();
            BackupMenu();
        } else if (__builtin_expect(Store_IsPackUnlocked(menu->selected_item), 1)) {
            GameAudio_PlaySfx(0x32, NULL, 0, 0);
        } else {
            Store_UnlockPack(menu->selected_item, true);
            GameAudio_PlaySfx(0x30, NULL, 0, 0);
            ReCalculateCompletionPoints();
            BackupMenu();
        }
    }
}

void MenuDrawDebugStore(MENU_s *menu) {
    menu->item_scale = 0.5f;
    dme_b = 0;
    menu->centre_offset = static_cast<f32>(menu->last_row - menu->first_row) * MENUDY * 0.5f * 0.5f;
    menu->draw_y = -menu->centre_offset;

    STORE_PRODUCT_s product;
    char line[64];
    f32 total = 0.0f;
    f32 unlocked_total = 0.0f;
#define DRAW_DEBUG_PACK(INDEX)                                                                                         \
    do {                                                                                                               \
        product.price = 0.0f;                                                                                          \
        char *product_id = *reinterpret_cast<char **>(&StorePack[INDEX].field1_0x4);                                   \
        if (NuIOS_GetInAppProductByID(product_id, reinterpret_cast<NuIOS_InAppProduct *>(&product)) != 0) {            \
            total += product.price;                                                                                    \
        }                                                                                                              \
        if (Store_IsPackUnlocked(INDEX)) {                                                                             \
            unlocked_total += product.price;                                                                           \
            dme_g = 255;                                                                                               \
            dme_r = 31;                                                                                                \
        } else {                                                                                                       \
            dme_g = 31;                                                                                                \
            dme_r = 255;                                                                                               \
        }                                                                                                              \
        dme_rgb = 1;                                                                                                   \
        sprintf(line, "%s ~0%.2f~~", StorePack[INDEX].name, static_cast<double>(product.price));                       \
        dme_sy = menu->item_scale;                                                                                     \
        GameDrawMenuEntry(menu, line);                                                                                 \
    } while (0)
    DRAW_DEBUG_PACK(0);
    DRAW_DEBUG_PACK(1);
    DRAW_DEBUG_PACK(2);
    DRAW_DEBUG_PACK(3);
    DRAW_DEBUG_PACK(4);
    DRAW_DEBUG_PACK(5);
    DRAW_DEBUG_PACK(6);
    DRAW_DEBUG_PACK(7);
    DRAW_DEBUG_PACK(8);
    DRAW_DEBUG_PACK(9);
    DRAW_DEBUG_PACK(10);
#undef DRAW_DEBUG_PACK

    dme_r = 191;
    dme_g = 255;
    dme_b = 0;
    dme_rgb = 1;
    GameDrawMenuEntry(menu, const_cast<char *>("Un-buy All"));
    sprintf(line, "%.2f/%.2f", static_cast<double>(unlocked_total), static_cast<double>(total));
    const f32 scale = menu->item_scale * MENUTEXTSCALE;
    MenuSmartTextEx(line, menu->draw_x, menu->draw_y, menu->draw_z, scale, scale, scale, dme_align, 255, 255, 255,
                    MENUENTRYEXWIDTH, 1, NULL, 0, 128);
}

void MenuInitStoreHolding(MENU_s *) {
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

void MenuUpdateStoreHolding(MENU_s *menu) {
    if (menu->cancel_pressed != 0) {
        BackupMenu();
        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    } else if (menu->confirm_pressed != 0) {
        if (menu->selected_item == 0) {
            NewMenu(21, -1, -1);
        } else if (menu->selected_item == 1) {
            NuIOS_RecordFlurryEvent(const_cast<char *>("menu_restore"));
            NuIOS_RestoreInAppPurchases();
            NewMenu(22, -1, -1);
        }
    }
}

void MenuDrawStoreHolding(MENU_s *menu) {
    menu->draw_y = -0.4f;
    GameDrawMenuEntry(menu, TTab[tCONTINUE]);
    if (MenuStopDraw != 0) {
        return;
    }
    DrawCharIcon(*StorePack[menu_i_pack].id, 0.0f, 0.5f, 0.0f, 1.75f * ICONSIZE, 0xa5, MenuAlpha, MenuAlpha, 1, NULL);
    if (TTab[1578] != NULL) {
        SmartTextEx(TTab[1578], 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0, RAP_WARNING_R, RAP_WARNING_G, RAP_WARNING_B,
                    1.9f, 4, NULL, 0, static_cast<i32>(128.0f * MenuAlpha));
    }
}

void MenuExitStoreHolding(MENU_s *) {
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

void MenuInitStore(MENU_s *) {
    NuIOS_GetPurchaseResult();
    Store_RestorePurchases();
    if (static_cast<u32>(menu_i_pack) > 10 || Store_IsPackUnlocked(menu_i_pack)) {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
        GameCam_HitRoll();
        MenuReset();
        return;
    }

    StoreIAP[0].text[0] = 0;
    StoreIAP[1].text[0] = 0;
    StoreIAP[2].text[0] = 0;
    char event[128];
    char *product_id = *reinterpret_cast<char **>(&StorePack[menu_i_pack].field1_0x4);
    sprintf(event, "pack_%s_tapped", product_id);
    NuIOS_RecordFlurryEvent(event);
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

void MenuUpdateStore(MENU_s *menu) {
    if (__builtin_expect(menu->cancel_pressed != 0 || StoreIAP[0].text[0] == 'x', 0)) {
        GameAudio_PlaySfx(0x31, NULL, 0, 0);
        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        MenuReset();
        return;
    }

    const f32 time = NuFmod(menu->menu_time, 12.0f);
    i32 hit;
#define CHECK_STORE_TOUCH_POINT(INDEX)                                                                                 \
    if (MenuAlpha >= 1.0f && MechInputTouchMenuController::AnyTouchesThisFrame > 0) {                                  \
        const f32 dx = StoreTouchLastPos.x - entry.x;                                                                  \
        const f32 dy = StoreTouchLastPos.y - entry.y;                                                                  \
        if (__builtin_expect(dx * dx + dy * dy < 0.01f, 0)) {                                                          \
            hit = INDEX;                                                                                               \
            goto store_touch_selected;                                                                                 \
        }                                                                                                              \
    }
#define CHECK_STORE_TOUCH(INDEX)                                                                                       \
    do {                                                                                                               \
        STOREIAP_s &entry = StoreIAP[INDEX];                                                                           \
        if (entry.text[0] == 0) {                                                                                      \
            return;                                                                                                    \
        }                                                                                                              \
        CHECK_STORE_TOUCH_POINT(INDEX);                                                                                \
    } while (0)
#define CHECK_STORE_PULSE(INDEX)                                                                                       \
    do {                                                                                                               \
        STOREIAP_s &entry = StoreIAP[INDEX];                                                                           \
        if (entry.text[0] == 0) {                                                                                      \
            return;                                                                                                    \
        }                                                                                                              \
        const f32 trigger = MenuAlpha - (entry.y + 1.0f) * 4.0f;                                                       \
        if (time >= trigger && time - FRAMETIME < trigger) {                                                           \
            VuVec pulse __attribute__((aligned(16))) = VuVec(entry.x, entry.y, 1.0f, 1.0f);                            \
            MechSystems::Get()->NewRadarPulse(pulse, false);                                                           \
        }                                                                                                              \
        CHECK_STORE_TOUCH_POINT(INDEX);                                                                                \
    } while (0)
    if (__builtin_expect(time >= 8.0f, 1)) {
        CHECK_STORE_TOUCH(0);
        CHECK_STORE_TOUCH(1);
        CHECK_STORE_TOUCH(2);
    } else {
        CHECK_STORE_PULSE(0);
        CHECK_STORE_PULSE(1);
        CHECK_STORE_PULSE(2);
    }
#undef CHECK_STORE_PULSE
#undef CHECK_STORE_TOUCH
#undef CHECK_STORE_TOUCH_POINT
    return;

store_touch_selected:
    if (NuIOS_AreInAppPurchasesAvailable() != 0 && NuIOS_CanMakeInAppPurchases() != 0) {
        menu_storepurchase_iap_name = StoreIAP[hit].text;
        menu_i_bundle = StoreBundle_FindByName(menu_storepurchase_iap_name);
        NewMenu(LEGOMENU_STORE_PURCHASE, -1, -1);
        char event[128];
        if (menu_i_bundle != -1) {
            sprintf(event, "bundle_%s_bought", StoreBundle[menu_i_bundle].name);
        } else if (menu_i_pack != -1) {
            sprintf(event, "pack_%s_bought", *reinterpret_cast<char **>(&StorePack[menu_i_pack].field1_0x4));
        } else {
            return;
        }
        NuIOS_RecordFlurryEvent(event);
        return;
    }
    if (TTab[0x60f] != NULL) {
        NUVEC position = {0.0f, -0.6f, 1.0f};
        GAMEMESSAGE_s *message = static_cast<GAMEMESSAGE_s *>(
            AddGameMessage(TTab[0x60f], &position, 0.5f, &position, 0.55f, 255, 31, 31, 0x20, 2.5f));
        if (message != NULL) {
            message->field_0xfe = 0xff;
        }
    }
    GameAudio_PlaySfx(0x32, NULL, 0, 0);
    GameCam_HitRoll();
    MenuReset();
}

void MenuDrawStore(MENU_s *) {
    const i32 selected_id = *StorePack[menu_i_pack].id;
    const f32 phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    const i32 angle = static_cast<i32>((phase + phase) * 65536.0f);
    const f32 pulse = NuTrigTable[(angle >> 1) & 0x7fff];
    if (MenuStopDraw != 0) {
        return;
    }

#define DRAW_STORE_PANEL(INDEX)                                                                                        \
    if (StoreIAP[INDEX].text[0] != 0) {                                                                                \
        const i32 alpha = ((MenuA * 7) / 8) << 24;                                                                     \
        DrawRectRGBA(0.0f, StoreIAP[INDEX].title_y, 2.1f, StoreIAP[INDEX].title_y - StoreIAP[INDEX].bottom_y + 0.02f,  \
                     static_cast<u32>(alpha), FadeMtl2, 1, 1.0f);                                                      \
    }
    DRAW_STORE_PANEL(0);
    DRAW_STORE_PANEL(1);
    DRAW_STORE_PANEL(2);
#undef DRAW_STORE_PANEL

    const f32 icon_alpha = (pulse * 0.2f + 0.8f) * MenuAlpha;
    DrawCharIcon(selected_id, 0.0f, 0.65f, 0.0f, ICONSIZE, 0xa5, icon_alpha, icon_alpha, 1, NULL);

    STORE_PRODUCT_s product;
    char line[256];
    char discount[256];
    product.description[0] = 0;
    product.price = 0.0f;
    StoreIAP[0].x = 0.0f;
    StoreIAP[0].y = 0.65f;
    char *selected_product = *reinterpret_cast<char **>(&StorePack[menu_i_pack].field1_0x4);
    if (NuIOS_GetInAppProductByID(selected_product, reinterpret_cast<NuIOS_InAppProduct *>(&product)) != 0) {
        NuStrCpy(StoreIAP[0].text, product.price_text);
    }
    sprintf(line, "%s ~0%.2f~~", TTab[StorePack[menu_i_pack].message_text_index], static_cast<double>(product.price));
    Text3DEx(line, 0.0f, 0.55f, 1.0f, 0.4f, 0.4f, 0.4f, 1, 255, 191, 0, static_cast<u8>(MenuA));
    StoreIAP[0].title_y = 0.55f;
    StoreIAP[0].bottom_y = 0.55f + text3d_height;
    StoreIAP[0].width = text3d_width;
    if (product.description[0] != 0) {
        SmartTextEx(product.description, 0.0f, StoreIAP[0].bottom_y, 1.0f, 0.3f, 0.3f, 0.3f, 1, 255, 127, 0, 1.9f, 2,
                    NULL, 0, MenuA);
        StoreIAP[0].bottom_y -= text3d_height;
        if (StoreIAP[0].width < text3d_width) {
            StoreIAP[0].width = text3d_width;
        }
    }

    i32 slot = 1;
    for (i32 bundle_index = 0; bundle_index < 3; ++bundle_index) {
        STOREBUNDLE &bundle = StoreBundle[bundle_index];
        if ((bundle.pack_mask & (1u << menu_i_pack)) == 0) {
            continue;
        }
        if (NuIOS_IsProductPurchased(bundle.name) != 0 ||
            (SuperOptions.store_bundle_flags & (1u << bundle_index)) != 0) {
            StoreIAP[0].text[0] = 'x';
            return;
        }

        product.description[0] = 0;
        product.price_text[0] = 0;
        product.price = 0.0f;
        NuIOS_GetInAppProductByID(bundle.name, reinterpret_cast<NuIOS_InAppProduct *>(&product));
        const f32 bundle_price = product.price;
        f32 separate_price = 0.0f;
        i32 character_ids[11];
        i32 already_owned[11];
        i32 pack_indices[11];
        i32 count = 0;
#define ADD_BUNDLE_PACK(INDEX)                                                                                         \
    if ((bundle.pack_mask & (1u << INDEX)) != 0) {                                                                     \
        pack_indices[count] = INDEX;                                                                                   \
        character_ids[count] = *StorePack[INDEX].id;                                                                   \
        already_owned[count] = Store_IsPackUnlocked(INDEX);                                                            \
        if (already_owned[count] == 0) {                                                                               \
            STORE_PRODUCT_s pack_product;                                                                              \
            pack_product.price = 0.0f;                                                                                 \
            char *pack_product_id = *reinterpret_cast<char **>(&StorePack[INDEX].field1_0x4);                          \
            if (NuIOS_GetInAppProductByID(pack_product_id, reinterpret_cast<NuIOS_InAppProduct *>(&pack_product)) !=   \
                0) {                                                                                                   \
                separate_price += pack_product.price;                                                                  \
            }                                                                                                          \
        }                                                                                                              \
        ++count;                                                                                                       \
    }
        ADD_BUNDLE_PACK(0);
        ADD_BUNDLE_PACK(1);
        ADD_BUNDLE_PACK(2);
        ADD_BUNDLE_PACK(3);
        ADD_BUNDLE_PACK(4);
        ADD_BUNDLE_PACK(5);
        ADD_BUNDLE_PACK(6);
        ADD_BUNDLE_PACK(7);
        ADD_BUNDLE_PACK(8);
        ADD_BUNDLE_PACK(9);
        ADD_BUNDLE_PACK(10);
#undef ADD_BUNDLE_PACK

        if (separate_price <= bundle_price || slot >= 3) {
            continue;
        }
        f32 y = StoreIAP[slot - 1].bottom_y - 0.2f;
        const f32 step = count > 1 ? 0.75f : 0.0f;
        const f32 start_x = -static_cast<f32>(count - 1) * step * 0.5f;
        for (i32 i = 0; i < count; ++i) {
            const f32 x = start_x + static_cast<f32>(i) * step;
            DrawCharIcon(character_ids[i], x, y, 0.0f, ICONSIZE, character_ids[i] == selected_id ? 0xa5 : 0xa7,
                         icon_alpha, icon_alpha, 1, NULL);
            if (already_owned[i] != 0) {
                Text3DEx(const_cast<char *>("X"), x, y, 1.0f, 0.6f, 0.6f, 0.6f, 0, 0, 255, 255, static_cast<u8>(MenuA));
            }
        }
        StoreIAP[slot].x = 0.0f;
        StoreIAP[slot].y = y;
        NuStrCpy(StoreIAP[slot].text, product.price_text);
        y -= 0.2f;
        sprintf(line, "%s ~0%.2f~~", TTab[bundle.text_index], static_cast<double>(bundle_price));
        Text3DEx(line, 0.0f, y, 1.0f, 0.4f, 0.4f, 0.4f, 1, 255, 191, 0, static_cast<u8>(MenuA));
        StoreIAP[slot].title_y = y;
        StoreIAP[slot].bottom_y = y + text3d_height;
        StoreIAP[slot].width = text3d_width;
        if (product.description[0] != 0) {
            SmartTextEx(product.description, 0.0f, StoreIAP[slot].bottom_y, 1.0f, 0.3f, 0.3f, 0.3f, 1, 0, 191, 255,
                        1.9f, 1, NULL, 0, MenuA);
            StoreIAP[slot].bottom_y -= text3d_height;
            if (StoreIAP[slot].width < text3d_width) {
                StoreIAP[slot].width = text3d_width;
            }
        }
        sprintf(discount, "%.2f", static_cast<double>(separate_price - bundle_price));
        if (TTab[tCONTINUE] != NULL) {
            sprintf(line, TTab[tCONTINUE], discount);
            SmartTextEx(line, 0.0f, StoreIAP[slot].bottom_y, 1.0f, 0.3f, 0.3f, 0.3f, 1, 0, 191, 255, 1.9f, 1, NULL, 0,
                        MenuA);
            StoreIAP[slot].bottom_y -= text3d_height;
            if (StoreIAP[slot].width < text3d_width) {
                StoreIAP[slot].width = text3d_width;
            }
        }
        ++slot;
    }
}

void MenuExitStore(MENU_s *) {
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

void MenuInitStoreRestoring(MENU_s *) {
    restoring_pack_bits = 0;
    restoring_pack_count = 0;
    restoring_bundle_bits = 0;
    restoring_bundle_count = 0;
    restoring_wait = 3.0f;
}

void MenuUpdateStoreRestoring(MENU_s *menu) {
    if (restoring_wait > 0.0f) {
        restoring_wait -= FRAMETIME;
    }

#define CHECK_RESTORING_PACK(INDEX)                                                                                    \
    do {                                                                                                               \
        char *product = *reinterpret_cast<char **>(&StorePack[INDEX].field1_0x4);                                      \
        if (NuIOS_IsProductPurchased(product) != 0 && (restoring_pack_bits & (1u << INDEX)) == 0) {                    \
            restoring_wait = 3.0f;                                                                                     \
            restoring_pack_bits |= 1u << INDEX;                                                                        \
            restoring_pack_list[restoring_pack_count] = INDEX;                                                         \
            ++restoring_pack_count;                                                                                    \
        }                                                                                                              \
    } while (0)
    if (__builtin_expect(restoring_pack_count <= 10, 1)) {
        CHECK_RESTORING_PACK(0);
        CHECK_RESTORING_PACK(1);
        CHECK_RESTORING_PACK(2);
        CHECK_RESTORING_PACK(3);
        CHECK_RESTORING_PACK(4);
        CHECK_RESTORING_PACK(5);
        CHECK_RESTORING_PACK(6);
        CHECK_RESTORING_PACK(7);
        CHECK_RESTORING_PACK(8);
        CHECK_RESTORING_PACK(9);
        CHECK_RESTORING_PACK(10);
    }
#undef CHECK_RESTORING_PACK

#define CHECK_RESTORING_BUNDLE(INDEX)                                                                                  \
    do {                                                                                                               \
        if (NuIOS_IsProductPurchased(StoreBundle[INDEX].name) != 0 && (restoring_bundle_bits & (1u << INDEX)) == 0) {  \
            restoring_bundle_bits |= 1u << INDEX;                                                                      \
            restoring_bundle_list[restoring_bundle_count] = INDEX;                                                     \
            ++restoring_bundle_count;                                                                                  \
            restoring_wait = 3.0f;                                                                                     \
        }                                                                                                              \
    } while (0)
    if (__builtin_expect(restoring_bundle_count <= 2, 1)) {
        CHECK_RESTORING_BUNDLE(0);
        CHECK_RESTORING_BUNDLE(1);
        CHECK_RESTORING_BUNDLE(2);
    }
#undef CHECK_RESTORING_BUNDLE

    if (menu->confirm_pressed != 0) {
        if (restoring_wait <= 0.0f) {
            BackupMenu();
        }
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
    }
}

void MenuDrawStoreRestoring(MENU_s *menu) {
    if (__builtin_expect(restoring_wait <= 0.0f || MenuStopDraw != 0, 0)) {
        menu->draw_y = -0.6f;
        GameDrawMenuEntry(menu, TTab[tACCEPT]);
        if (MenuStopDraw != 0) {
            return;
        }
    }

    const f32 pulse_alpha = MenuAlpha * 128.0f;
    const f32 phase = NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);
    const i32 angle = static_cast<i32>((phase + phase) * 65536.0f);
    const i32 alpha = static_cast<i32>((NuTrigTable[(angle >> 1) & 0x7fff] * 0.2f + 0.8f) * pulse_alpha);
    char *title = TTab[0x626] != NULL ? TTab[0x626] : const_cast<char *>("Restoring Purchases");
    SmartTextEx(title, 0.0f, 0.5f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, 255, 159, 0, 1.9f, 1, NULL, 0,
                alpha);

    f32 y = 0.5f + text3d_height;
    STORE_PRODUCT_s product;
    for (i32 i = 0; i < restoring_pack_count; ++i) {
        char *product_id = *reinterpret_cast<char **>(&StorePack[restoring_pack_list[i]].field1_0x4);
        if (NuIOS_GetInAppProductByID(product_id, reinterpret_cast<NuIOS_InAppProduct *>(&product)) != 0) {
            SmartTextEx(product.name, 0.0f, y, 1.0f, 0.3f, 0.3f, 0.3f, 0, 255, 255, 255, 1.9f, 1, NULL, 0, MenuA);
            y += text3d_height;
        }
    }
    for (i32 i = 0; i < restoring_bundle_count; ++i) {
        if (NuIOS_GetInAppProductByID(StoreBundle[restoring_bundle_list[i]].name,
                                      reinterpret_cast<NuIOS_InAppProduct *>(&product)) != 0) {
            SmartTextEx(product.name, 0.0f, y, 1.0f, 0.3f, 0.3f, 0.3f, 0, 255, 255, 255, 1.9f, 1, NULL, 0, MenuA);
            y += text3d_height;
        }
    }
}

void MenuExitStoreRestoring(MENU_s *) {
    u8 pack_count = restoring_pack_count;
    if (pack_count != 0) {
        u8 *item = restoring_pack_list;
        u8 *end = item + pack_count;
        i32 flags = SuperOptions.store_pack_flags;
        do {
            flags |= 1u << *item++;
        } while (item != end);
        SuperOptions.store_pack_flags = flags;
    }
    u8 bundle_count = restoring_bundle_count;
    if (bundle_count != 0) {
        u8 *item = restoring_bundle_list;
        u8 *end = item + bundle_count;
        i32 flags = SuperOptions.store_bundle_flags;
        do {
            flags |= 1u << *item++;
        } while (item != end);
        SuperOptions.store_bundle_flags = flags;
    }
    Store_RestorePurchases();
}

void MenuInitStorePurchase(MENU_s *) {
    NuIOS_PurchaseInAppProduct(menu_storepurchase_iap_name);
}

void MenuDrawStorePurchase(MENU_s *) {
}

void MenuExitStorePurchase(MENU_s *) {
    NuIOS_GetPurchaseResult();
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

void Store_RootPackCustodian(i32, GameObject_s *custodian) {
    u8 flags = custodian->field_0xefc;
    custodian->apiobj.flags_low |= 2;
    flags |= 0x12;
    custodian->field_0xefe |= 0x40;
    custodian->field_0xefc = flags;
    u32 object_flags = custodian->apiobj.field_0x1f4;
    object_flags &= ~1u;
    object_flags |= 0x80000004u;
    custodian->apiobj.field_0x1f4 = object_flags;
}

void Store_UprootPackCustodian(i32, GameObject_s *custodian) {
    CHARACTERDATA **character_list = &CDataList;
    u8 flags = custodian->field_0xefc;
    custodian->apiobj.flags_low &= ~2u;
    flags &= ~0x12u;
    custodian->field_0xefe &= ~0x40u;
    custodian->field_0xefc = flags;

    CHARACTERDATA *characters = *character_list;
    if ((reinterpret_cast<u8 *>(characters)[custodian->id * sizeof(CHARACTERDATA) + 5] & 2) == 0 &&
        (apicharsys->char_data[custodian->id].model_flags & 4) != 0) {
        custodian->apiobj.field_0x1f4 |= 1;
    }
    custodian->apiobj.field_0x1f4 &= 0x7fffffffu;
}

void MenuUpdateStorePurchase(MENU_s *) {
    const i32 result = NuIOS_GetPurchaseResult();
    if (result == 0) {
        return;
    }
    if (result == 2) {
        GameAudio_PlaySfx(0x26, NULL, 0, 0);
        if (menu_i_bundle != -1) {
            SuperOptions.store_bundle_flags |= 1u << menu_i_bundle;
#define UNLOCK_BUNDLE_PACK(INDEX)                                                                                      \
    if ((StoreBundle[menu_i_bundle].pack_mask & (1u << INDEX)) != 0) {                                                 \
        Store_UnlockPack(INDEX, false);                                                                                \
    }
            UNLOCK_BUNDLE_PACK(0);
            UNLOCK_BUNDLE_PACK(1);
            UNLOCK_BUNDLE_PACK(2);
            UNLOCK_BUNDLE_PACK(3);
            UNLOCK_BUNDLE_PACK(4);
            UNLOCK_BUNDLE_PACK(5);
            UNLOCK_BUNDLE_PACK(6);
            UNLOCK_BUNDLE_PACK(7);
            UNLOCK_BUNDLE_PACK(8);
            UNLOCK_BUNDLE_PACK(9);
            UNLOCK_BUNDLE_PACK(10);
#undef UNLOCK_BUNDLE_PACK
        } else {
            Store_UnlockPack(menu_i_pack, false);
        }

        GameObject_s *custodian = FindGameObject(*StorePack[menu_i_pack].id, 0, 0, 1, 0);
        if (custodian != NULL) {
            if (static_cast<i32>(custodian->apiobj.field_0x1f4) < 0) {
                Store_UprootPackCustodian(menu_i_pack, custodian);
            }
            if (__builtin_expect(NuVecXZDistSqr(&player->apiobj.position, &custodian->apiobj.position, NULL) < 4.0f,
                                 0)) {
                TagCharacter(player, custodian, 1);
            }
        }
        TriggerExtraDataSave();
        TriggerAutoSave();
        MenuReset();
        return;
    }

    if (result == 1) {
        return;
    }
    if (result == 3 && TTab[1551] != NULL) {
        NUVEC position = {0.0f, -0.6f, 1.0f};
        GAMEMESSAGE_s *message = static_cast<GAMEMESSAGE_s *>(
            AddGameMessage(TTab[1551], &position, 0.5f, &position, 0.55f, 255, 31, 31, 0x20, 2.5f));
        if (message != NULL) {
            message->field_0xfe = 0xff;
        }
    }
    GameAudio_PlaySfx(0x32, NULL, 0, 0);
    GameCam_HitRoll();
    MenuReset();
}
