#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
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

struct GameObject_s;
struct LEVEL_PROGRESS_s;
struct WORLDINFO_s;

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

static void StoreUnlockArcade() {
    STUBBED();
}
static void StoreUnlockBonus() {
    STUBBED();
}
static void StoreUnlockBounty() {
    STUBBED();
}
static void StoreUnlockChallenge() {
    HUB_AREAPANELX = HUB_AREAPANELX_ONETRUEJEDIGOLDBRICK;
}
static void StoreUnlockJedi() {
    STUBBED();
}
static void StoreUnlockSith() {
    STUBBED();
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
    STUBBED();
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

void InitSuperStory(i32) {
    STUBBED();
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

void Game_GotAllGoldBricks() {
    STUBBED();
}

void Game_AutoSaving() {
    STUBBED();
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

void StoreBundle_FindByName(char *) {
    STUBBED();
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
    STUBBED();
}

void MenuUpdateStoreHolding(MENU_s *) {
    STUBBED();
}

void MenuDrawStoreHolding(MENU_s *) {
    STUBBED();
}

void MenuExitStoreHolding(MENU_s *) {
    STUBBED();
}

void MenuInitStore(MENU_s *) {
    STUBBED();
}

void MenuUpdateStore(MENU_s *) {
    STUBBED();
}

void MenuDrawStore(MENU_s *) {
    STUBBED();
}

void MenuExitStore(MENU_s *) {
    STUBBED();
}

void MenuInitStoreRestoring(MENU_s *) {
    STUBBED();
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
    STUBBED();
}

void MenuDrawStorePurchase(MENU_s *) {
    STUBBED();
}

void MenuExitStorePurchase(MENU_s *) {
    STUBBED();
}

void Store_RootPackCustodian(i32, GameObject_s *) {
    STUBBED();
}

void Store_UprootPackCustodian(i32, GameObject_s *) {
    STUBBED();
}

void MenuUpdateStorePurchase(MENU_s *) {
    STUBBED();
}
