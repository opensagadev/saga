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
void NuIOS_RestoreInAppPurchases();
extern "C" void NuIOS_RecordFlurryEvent(char *);
extern "C" void BackupMenu();
extern i32 CutInstEndCount;
i32 LEGOMENU_PAUSESYNC = -1;
void GameCam_HitRoll();

struct STOREIAP_s {
    f32 x;
    f32 y;
    f32 title_y;
    f32 bottom_y;
    f32 width;
    char text[256];
};
DECOMP_ASSERT(sizeof(STOREIAP_s) == 0x114, "STOREIAP_s size");
static STOREIAP_s StoreIAP[3];
extern i32 menu_i_pack;
static char *menu_storepurchase_iap_name;

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

STOREPACK StorePack[11] = {
    {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockEp2},       {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockEp3},
    {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockEp4},       {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockEp5},
    {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockEp6},       {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockArcade},
    {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockBonus},     {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockBounty},
    {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockChallenge}, {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockJedi},
    {NULL, 0, 0, 0, 0, {{0, 0}}, 0, 0, StoreUnlockSith},
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
u16 restoring_bundle_bits;
u8 restoring_bundle_count;
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
            asm volatile("" ::: "memory");
            player->pause_context_state = 0;
            asm volatile("" ::: "memory");
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
    asm volatile("" : "+D"(bundles));
    i32 result;
    char *second_name;
    char *third_name;
    if (__builtin_expect(NuStrICmp(bundles[0].name, name) == 0, 0)) {
        result = 0;
        goto done;
    }
    second_name = bundles[1].name;
    asm volatile("" : "+a"(second_name));
    if (__builtin_expect(NuStrICmp(second_name, name) == 0, 0)) {
        result = 1;
        goto done;
    }
    third_name = bundles[2].name;
    asm volatile("" : "+a"(third_name));
    result = NuStrICmp(third_name, name) == 0 ? 2 : -1;
done:
    asm volatile("" : "+a"(result));
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

void MenuUpdateDebugStore(MENU_s *) {
    STUBBED();
}

void MenuDrawDebugStore(MENU_s *) {
    STUBBED();
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

void MenuDrawStoreHolding(MENU_s *) {
    STUBBED();
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

void MenuUpdateStore(MENU_s *) {
    STUBBED();
}

void MenuDrawStore(MENU_s *) {
    STUBBED();
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

void MenuUpdateStoreRestoring(MENU_s *) {
    STUBBED();
}

void MenuDrawStoreRestoring(MENU_s *) {
    STUBBED();
}

void MenuExitStoreRestoring(MENU_s *) {
    STUBBED();
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
    asm volatile("" ::: "memory");
    u32 object_flags = custodian->apiobj.field_0x1f4;
    object_flags &= ~1u;
    asm volatile("" : "+r"(object_flags));
    object_flags |= 0x80000004u;
    custodian->apiobj.field_0x1f4 = object_flags;
}

void Store_UprootPackCustodian(i32, GameObject_s *custodian) {
    CHARACTERDATA **character_list = &CDataList;
    asm volatile("" : "+c"(character_list));
    u8 flags = custodian->field_0xefc;
    custodian->apiobj.flags_low &= ~2u;
    flags &= ~0x12u;
    custodian->field_0xefe &= ~0x40u;
    custodian->field_0xefc = flags;
    asm volatile("" ::: "memory");

    CHARACTERDATA *characters = *character_list;
    asm volatile("" : "+c"(characters) :: "memory");
    if ((reinterpret_cast<u8 *>(characters)[custodian->id * sizeof(CHARACTERDATA) + 5] & 2) == 0 &&
        (apicharsys->char_data[custodian->id].model_flags & 4) != 0) {
        custodian->apiobj.field_0x1f4 |= 1;
    }
    custodian->apiobj.field_0x1f4 &= 0x7fffffffu;
}

void MenuUpdateStorePurchase(MENU_s *) {
    STUBBED();
}
