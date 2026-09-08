#include <string.h>
#include <stdio.h>

#include "decomp.h"
#include "globals.h"
#include "gameframework/saveload.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/text.h"
#include "gameapi/gui/apimenu.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numath/nufloat.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" i32 NewMode;
extern "C" i32 reset_load;
extern "C" i32 Paused;
extern FadeSystem FadeSys;
extern f32 statstime;
extern f32 cointotaltime;
extern i32 screendump;
extern i32 newgamecam;
extern STATUSPACKET_s StatusPacket;
extern AREADATA *NEGOTIATIONS_ADATA;

i32 GetMenuID();
extern "C" i32 MenuInMemoryCard();
void ResetRumble(RUMBLEPACKET *packet);
void ReCalculateCompletionPoints();
void StatusStage_Reset(STATUS_STAGE_s *stage);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void IncreaseScore(u32 *, u64, i32);
extern "C" void PlaySfx(char *, nuvec_s *);
void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
void FinishStatusPacket(i32);
extern "C" void BackupMenu();
void Text_FillInExtendedSaveInfo();
void DrawStatusBG_LSW(STATUSPACKET_s *);
i32 InitStatusScreen_LSW(WORLDINFO_s *, STATUSPACKET_s *);
i32 FinishStatusPacket_LSW(WORLDINFO_s *, STATUSPACKET_s *, i32);
void ResetStatusPacket_LSW(STATUSPACKET_s *);
void RegisterStatusScreen(STATUS_STAGE_s *, i32 *, REGISTERSTATUSPACKET_s *);
void EndOfDemo(i32);
static i32 gamedemo_option;
static i32 goldbrickmsgcount;
void AddToGoldBricks();

i32 AddGoldBrickMessage(STATUSPACKET_s *packet, i16 brick) {
    if (goldbrickmsgcount < 16) {
        reinterpret_cast<i16 *>(packet->field_0x12c)[goldbrickmsgcount] = brick;
        ++goldbrickmsgcount;
    }
    AddToGoldBricks();
    return 1;
}
extern f32 DROPINALPHA;
extern f32 PANEL_SCOREX;
extern f32 PANEL_SCORESCALE;
extern i16 tCOINTOTAL;
extern i16 tSELECT;
extern i16 tSELECTING;
void Text_MakeScore(u32, char *);
void DrawPlayerIconPrompts(i32, i32, f32, i32, i32, i32, i32, i32, i32, f32, i32, i32, i32, i32);
void Status_DrawPromptMenu(STATUSPACKET_s *, i32, f32);
f32 getFinishedStatusAlpha(STATUSPACKET_s *);
f32 STATUS_TITLE_Y = 0.5f;

namespace {
    enum PANEL_BLOCKING_MENU {
        PANEL_MENU_EPISODE_I = 20,
        PANEL_MENU_EPISODE_II = 21,
        PANEL_MENU_EPISODE_III = 22,
        PANEL_MENU_EPISODE_IV = 23,
        PANEL_MENU_SAVE = 25,
        PANEL_MENU_LOAD = 26,
    };

    bool CoinTotalCanOpen() {
        if (FadeSys.fade != 0.0f || CUTSTOPGAME != 0) {
            return false;
        }
        if (Paused == 0 && NetPaused == 0 && DrawCoinTotalTime <= 0.0f) {
            return false;
        }
        if (screendump != 0 || MenuInMemoryCard() != 0) {
            return false;
        }

        const i32 menu = GetMenuID();
        return menu != PANEL_MENU_EPISODE_I && menu != PANEL_MENU_EPISODE_II && menu != PANEL_MENU_EPISODE_III &&
               menu != PANEL_MENU_EPISODE_IV && menu != PANEL_MENU_SAVE && menu != PANEL_MENU_LOAD;
    }
} // namespace

u16 hub_iconang[4] = {};
static f32 hub_icontime[4] = {};
STATUS_STAGE_s *StatusStages;
f32 iconalphaoverride;
f32 icon_y;
i32 draw_player_icons;
i32 status_prompt;
static i32 DrawGoldBrick_Stage;
static i32 DrawGoldBrick_Phase;
void RedBrick_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void RedBrick_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void RedBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void CollectCharcters_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void CollectCharcters_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void CollectCharcters_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void CollectCharactersOff_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void CollectCharactersOff_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void CollectCharactersOff_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void Coins_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void Coins_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Coins_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void TrueHero_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void TrueHero_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void TrueHero_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void MiniKit_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void MiniKit_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void MiniKit_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void AllMiniKits_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void AllMiniKits_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void AllMiniKits_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void GoldBrick_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void GoldBrick_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void GoldBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void LevelComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void LevelComplete_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void LevelComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusTime_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusTime_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void ChallangeCash_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void ChallangeCash_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void ChallangeCash_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void SuperStoryTime_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void SuperStoryTime_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void SuperStoryTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void SuperStoryScore_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void SuperStoryScore_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void SuperStoryScore_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusComplete_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusWin_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusWin_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusWin_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void Prompt_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void Prompt_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Exit_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Save_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Fade_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);

STATUSPACKET_LSW_s StatusPacket_LSW;
STATUS_STAGE_s StatusStages_LSW[] = {
    {RedBrick_LSW_Draw, RedBrick_LSW_Update, RedBrick_LSW_Skip, 18, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {CollectCharcters_Draw, CollectCharcters_Update, CollectCharcters_Skip, 35, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {CollectCharactersOff_Draw, CollectCharactersOff_Update, CollectCharactersOff_Skip, 36, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {Coins_LSW_Draw, Coins_LSW_Update, Coins_LSW_Skip, 3, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {TrueHero_LSW_Draw, TrueHero_LSW_Update, TrueHero_LSW_Skip, 1, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {TrueHero_LSW_Draw, TrueHero_LSW_Update, TrueHero_LSW_Skip, 2, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {MiniKit_LSW_Draw, MiniKit_LSW_Update, MiniKit_LSW_Skip, 4, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {AllMiniKits_LSW_Draw, AllMiniKits_LSW_Update, AllMiniKits_LSW_Skip, 17, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {GoldBrick_LSW_Draw, GoldBrick_LSW_Update, GoldBrick_LSW_Skip, 19, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 13, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 8, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 28, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 29, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 30, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 31, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 32, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 33, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 34, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 14, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 23, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusTime_LSW_Draw, BonusTime_LSW_Update, BonusTime_LSW_Skip, 24, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {ChallangeCash_Draw, ChallangeCash_Update, ChallangeCash_Skip, 25, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 26, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusTime_LSW_Draw, BonusTime_LSW_Update, BonusTime_LSW_Skip, 27, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {SuperStoryTime_LSW_Draw, SuperStoryTime_LSW_Update, SuperStoryTime_LSW_Skip, 15, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {SuperStoryScore_LSW_Draw, SuperStoryScore_LSW_Update, SuperStoryScore_LSW_Skip, 16, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusComplete_LSW_Draw, BonusComplete_LSW_Update, BonusComplete_LSW_Skip, 22, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusWin_LSW_Draw, BonusWin_LSW_Update, BonusWin_LSW_Skip, 20, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusTime_LSW_Draw, BonusTime_LSW_Update, BonusTime_LSW_Skip, 21, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {Prompt_LSW_Draw, Prompt_LSW_Update, NULL, 9, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, Exit_LSW_Update, NULL, 10, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, Save_LSW_Update, NULL, 11, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, Fade_LSW_Update, NULL, 12, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, NULL, NULL, -1, 0xffff, 0, 0, 0, 0.0f, 0.0f},
};


void NewGameMode() {
    NewMode = 1;
    reset_load = 1;
}

void UpdateStats() {
    LEVELDATA *level = WORLD->current_level;
    if ((level->flags & LEVEL_GAMEPLAY) == 0) {
        return;
    }

    f32 stats_target = 0.0f;
    if (FadeSys.fade == 0.0f && CUTSTOPGAME == 0 && newgamecam == 0) {
        const bool hub_camera_hidden = HUB_ADATA != NULL && WORLD->area == HUB_ADATA && GameCam->mode == 4;
        const i32 menu = GetMenuID();
        if (!hub_camera_hidden && (menu < PANEL_MENU_EPISODE_I || menu > PANEL_MENU_EPISODE_IV)) {
            stats_target = 1.0f;
        }
    }
    statstime = SeekLinearF(statstime, stats_target, FRAMETIME);

    if ((level->flags & LEVEL_SHOW_COIN_TOTAL) != 0) {
        DrawCoinTotalTime = 1.0f;
    }
    if (DrawCoinTotalTime > 0.0f) {
        DrawCoinTotalTime -= FRAMETIME;
    }

    const f32 coin_total_target = CoinTotalCanOpen() ? 1.0f : 0.0f;
    cointotaltime = SeekLinearF(cointotaltime, coin_total_target, FRAMETIME);
    CoinTotalScale = SeekLinearF(CoinTotalScale, 1.0f, 3.0f * FRAMETIME);
}

void AddStatusStage(STATUSPACKET_s *packet, i32 type, i32 gold_brick_enabled) {
    const u8 index = packet->stage_count;
    packet->stage_types[index] = static_cast<i8>(type);
    packet->gold_brick_enabled[index] = static_cast<u8>(gold_brick_enabled);
    packet->stage_count = index + 1;
}

void SetBonusWinner(i32 player) {
    BonusWinner = player;
    GameCam_Blend(GameCam, 1.5f, 0.0f, 1);
    LookAtBoth = 1;
}

STATUS_STAGE_s *FindStatusStage(i32 type) {
    for (STATUS_STAGE_s *stage = StatusStages; stage->type != -1; ++stage) {
        if (stage->type == type) {
            return stage;
        }
    }
    return NULL;
}

void NextStatusStage(STATUSPACKET_s *packet) {
    STATUS_STAGE_s *stage = packet->stage;
    if (stage != NULL) {
        if (stage->field_0x14 == -1 && packet->field_0x68 > stage->field_0x18) {
            packet->field_0xb0 |= 2;
            return;
        }
        stage->field_0x12 = 1;
    }

    packet->previous_stage_2 = packet->previous_stage;
    packet->previous_stage = stage;
    ++packet->current_gold_brick;
    packet->stage = packet->next_stage;
    packet->next_stage = FindStatusStage(packet->stage_types[packet->current_gold_brick + 1]);

    while (packet->stage == NULL || packet->next_stage == NULL) {
        if (packet->stage == NULL && packet->next_stage != NULL) {
            packet->stage = packet->next_stage;
            packet->next_stage = NULL;
        }

        ++packet->current_gold_brick;
        if (packet->current_gold_brick + 1 < packet->stage_count) {
            packet->next_stage = FindStatusStage(packet->stage_types[packet->current_gold_brick + 1]);
        } else {
            packet->next_stage = FindStatusStage(10);
        }
    }

    StatusStage_Reset(packet->stage);
    packet->field_0xb0 &= ~2;
    status_prompt = 0;

    if (packet->stage->type == 10 || packet->stage->type == 11) {
        ResetRumble(reinterpret_cast<RUMBLEPACKET *>(&packet->player0_rumble_amount));
        ResetRumble(reinterpret_cast<RUMBLEPACKET *>(&packet->player1_rumble_amount));
    }
    if (packet->stage->type == 11) {
        ReCalculateCompletionPoints();
    }
}

void Prompt_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active != 0) {
        if (stage->field_0x18 >= 0.5f) {
            Status_DrawPromptMenu(packet, 1, 1.0f);
            DrawPlayerIconPrompts(packet->player0_active, tSELECT, 1.0f, -1, -1, -1, tSELECTING,
                                 packet->player1_active, tSELECT, 1.0f, -1, -1, -1, tSELECTING);
        } else {
            f32 alpha = NuTrigTable[(static_cast<i32>((stage->field_0x18 + stage->field_0x18) * 16384.0f) >> 1) & 0x7fff];
            if (stage->field_0x14 == 1) {
                alpha = 1.0f - alpha;
            }
            Status_DrawPromptMenu(packet, 0, alpha);
            draw_player_icons = 1;
        }
    }
}

void ResetIconWibble() {
    hub_iconang[0] = static_cast<u16>(qrand());
    hub_icontime[0] = 0.0f;
    hub_iconang[1] = static_cast<u16>(qrand());
    hub_icontime[1] = 0.0f;
    hub_iconang[2] = static_cast<u16>(qrand());
    hub_icontime[2] = 0.0f;
    hub_iconang[3] = static_cast<u16>(qrand());
    hub_icontime[3] = 0.0f;
}

void Save_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (packet->save_state == 3) {
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            if ((packet->mode_flags & 4) == 0 && packet->prompt_choice == 0 &&
                (packet->field_0xb0 & 0x40) == 0 && packet->challenge_state == 0 && packet->mission_state == 0) {
                packet->mode_flags |= 0x10;
            }
            NextStatusStage(packet);
        }
    } else if (packet->save_state == 0) {
        packet->save_state = 1;
        Text_FillInExtendedSaveInfo();
        if (TriggerAutoSave() == 0) {
            packet->save_state = 2;
            NewMenu(1000, -1, -1);
        }
    } else if ((packet->save_state == 2 ? MenuInMemoryCard() : memcard_autosaveinprogress) == 0) {
        packet->save_state = 3;
    }
}

extern i32 from_save_and_exit;
extern TIMER BonusTimer;
extern i16 id_SLAVE1;
extern i16 tSUPERSTORYCOMPLETE, tNEWBESTTIME, tLEVELCOMPLETE, tMISSIONCOMPLETE;
extern i16 tCHALLENGECOMPLETE, tTRUEHERO, tMINIKIT;
extern "C" void NuIOS_RecordFlurryEvent(char *);
extern "C" void NuStrCpy(char *, const char *);
void AddToCompletionPoints(u32);
void AddToCollection(i32);
i32 AllMiniKitsDone(AREASAVE_s *);
i32 Mission_CurrentState(MISSIONSYS *);
i32 newCharactersCollected(STATUSPACKET_s *);
void StatusPacketReset(STATUSPACKET_s *);
i32 newminikitcount;
i32 currentminikit;
static u32 collect_suit_bits;

struct NEWMINIPIECE_s {
    char name[8];
    i16 level;
    u8 pad[2];
};
DECOMP_ASSERT(sizeof(NEWMINIPIECE_s) == 12, "new minikit piece ABI");
NEWMINIPIECE_s NewMiniPiece[10];

void InitStatusScreen(WORLDINFO_s *world) {
    STATUSPACKET_s &p = StatusPacket;
    char event[72];
    if (p.lsw_packet == NULL) {
        return;
    }
    for (i32 i = 0; StatusStages[i].type != -1; ++i) {
        StatusStage_Reset(&StatusStages[i]);
    }
    StatusPacketReset(&p);
    icon_y = STATSPOSY;
    newminikitcount = 0;
    currentminikit = 0;
    p.field_0xb0 &= 0xfe;
    ResetRumble(reinterpret_cast<RUMBLEPACKET *>(&p.player0_rumble_amount));
    ResetRumble(reinterpret_cast<RUMBLEPACKET *>(&p.player1_rumble_amount));
    CoinTotalScale = 1.0f;
    goldbrickmsgcount = 0;
    if (netclient == 0) {
        if (Player[0] == NULL) {
            p.player0_model = 0xffff;
        } else {
            p.player0_model = Player[0]->id;
            p.player0_active = Player[0]->apiobj.flags_low >> 7;
            p.coins_remaining[0] = Player[0]->coinpacket == NULL ? 0 : Player[0]->coinpacket->coins;
            ++p.status_flags;
        }
        if (Player[1] == NULL) {
            p.player1_model = 0xffff;
        } else {
            p.player1_model = Player[1]->id;
            p.player1_active = Player[1]->apiobj.flags_low >> 7;
            p.coins_remaining[1] = Player[1]->coinpacket == NULL ? 0 : Player[1]->coinpacket->coins;
            ++p.status_flags;
        }
    }
    if (p.status_flags == 0) {
        goldbrickmsgcount = 0;
        CoinTotalScale = 1.0f;
        return;
    }
    p.stage_count = 0;
    p.field_0xb0 = (p.field_0xb0 & 0xbf) | ((FreePlay & 1) << 6);
    p.mode_flags = (p.mode_flags & 0xe3) | ((SuperStory & 1) << 2) | ((from_save_and_exit & 1) << 3);
    p.mission = Mission_Active(NULL);
    p.mission_state = p.mission == NULL ? 0 : Mission_CurrentState(NULL);
    if (from_save_and_exit != 0) {
        from_save_and_exit = 0;
        level_already_loaded = -1;
    }
    p.next_area = -1;
    p.field_0xbe = -1;
    p.newly_completed = 0;
    p.field_0xbd = 0;
    p.challenge_state = ChallengeMode;
    const i32 area = static_cast<i8>(world->level_sub_id);
    p.field_0xb0 &= 0x77;
    p.score = &Game.coins;
    p.previous_completion = Game.completion;
    p.area_id = area;
    p.previous_gold_bricks = Game.field_0x7c26[0];
    p.displayed_gold_bricks = Game.field_0x7c26[0];
    p.mode_flags &= 0xfc;
    i32 episode = -1;
    if (area == -1) {
        p.area = NULL;
        p.episode = NULL;
        p.chapter = -1;
    } else {
        p.area = ADataList + area;
        p.episode_id = p.area->episode_index;
        episode = p.episode_id;
        if (episode != -1) {
            p.episode = EDataList + episode;
        }
        p.chapter = p.area->area_index;
        if ((p.area->flags & 1) != 0) p.field_0xb0 |= 0x80;
        if ((p.area->flags & 4) != 0) p.mode_flags |= 1;
        if ((p.area->flags & 0x100) != 0) p.mode_flags |= 2;
    }
    if (p.init_callback(world, &p) != 0) {
        return;
    }
    if ((p.mode_flags & 4) != 0) {
        if (NewLData == STATUS_LDATA || WORLD->current_level == STATUS_LDATA) {
            EPISODESAVE_s &save = Game.episode_save[episode];
            p.previous_best_time = save.superstory_time_limit;
            p.previous_best_score = save.superstory_score_target;
            p.superstory_time = SuperStoryTimer[0];
            p.superstory_score = SuperStoryScore;
            i32 gold = 0;
            if (episode == -1) {
                p.new_best_score = 0;
                p.new_best_time = 0.0f;
                p.time_reward_score = *p.score;
            } else {
                p.new_best_time = p.superstory_time;
                if (p.previous_best_time <= p.superstory_time) p.new_best_time = 0.0f;
                else save.superstory_time_limit = p.superstory_time;
                p.new_best_score = p.superstory_score;
                if (p.previous_best_score < p.superstory_score) save.superstory_score_target = p.superstory_score;
                else p.new_best_score = 0;
                if ((save.flags & 0xff) == 0) {
                    save.flags = (save.flags & 0xffffff00) | 1;
                    AddToCompletionPoints(POINTS_PER_SUPERSTORY);
                    if (GOLDBRICKFORSUPERSTORY != 0) gold = AddGoldBrickMessage(&p, tSUPERSTORYCOMPLETE);
                }
                p.time_reward_score = *p.score;
                if (p.new_best_time != 0.0f) {
                    u64 reward = static_cast<i64>(static_cast<i32>(p.previous_best_time - p.new_best_time) * 100);
                    if (reward < 100) reward = 100;
                    IncreaseScore(&p.time_reward_score, reward, 0);
                }
            }
            p.final_reward_score = p.time_reward_score;
            IncreaseScore(&p.final_reward_score, static_cast<u64>(p.superstory_score), 0);
            AddStatusStage(&p, 14, gold);
            AddStatusStage(&p, 15, 0);
            AddStatusStage(&p, 16, 0);
            if (p.previous_gold_bricks < Game.field_0x7c26[0]) AddStatusStage(&p, 19, 0);
            if (netclient == 0 && (p.new_best_time != 0.0f || p.new_best_score != 0 || p.superstory_score != 0)) {
                AddStatusStage(&p, 11, 0);
            }
            hub_from_superstory = static_cast<i8>(p.area->episode_index);
        }
        AddStatusStage(&p, 10, 0);
        p.current_gold_brick = -1;
        p.stage_types[p.stage_count] = -1;
        p.next_stage = FindStatusStage(p.stage_types[0]);
        NextStatusStage(&p);
        if (episode != -1 && p.chapter < EDataList[episode].regular_areas) {
            p.next_area = EDataList[episode].area_ids[p.chapter + 1];
            if (p.next_area != -1 && (ADataList[p.next_area].flags & 2) != 0) hub_from_superstory = episode;
        }
        return;
    }
    if ((p.mode_flags & 1) != 0 && area != -1) {
        p.field_0xb0 = (p.field_0xb0 & 0xdf) | ((BonusWinner & 1) << 5);
        p.previous_best_time = Game.area_save[area].challenge_trial_time;
        p.elapsed_time = BonusTimer.time_elapsed;
        i32 gold = 0;
        if (p.previous_best_time <= 0.0f || p.elapsed_time < p.previous_best_time) {
            if (WORLD->area->challenge_trial_time != 0 && WORLD->area->challenge_trial_time <= p.previous_best_time) {
                AddToCompletionPoints(POINTS_PER_TIMETRIAL);
                gold = AddGoldBrickMessage(&p, tNEWBESTTIME);
            }
            p.new_best_time = p.elapsed_time;
            Game.area_save[area].challenge_trial_time = p.elapsed_time;
        }
        if ((p.mode_flags & 2) == 0) {
            AddStatusStage(&p, 20, 0);
            if (p.elapsed_time < p.previous_best_time) AddStatusStage(&p, 3, 0);
        } else {
            if (Game.area_save[p.area_id].area_complete == 0) {
                Game.area_save[p.area_id].area_complete = 1;
                AddToCompletionPoints(POINTS_PER_SUPERBONUSCOMPLETE);
                p.newly_completed = 1;
                if (GOLDBRICKFORSUPERBONUS != 0) gold = AddGoldBrickMessage(&p, tLEVELCOMPLETE);
            }
            AddStatusStage(&p, 22, gold);
            gold = 0;
        }
        AddStatusStage(&p, 21, gold);
        if (p.previous_gold_bricks < Game.field_0x7c26[0]) AddStatusStage(&p, 19, 0);
        AddStatusStage(&p, 9, 0);
        AddStatusStage(&p, 10, 0);
        if (netclient == 0) AddStatusStage(&p, 11, 0);
        AddStatusStage(&p, 12, 0);
        p.current_gold_brick = -1;
        p.stage_types[p.stage_count] = -1;
        p.next_stage = FindStatusStage(p.stage_types[0]);
        NextStatusStage(&p);
        draw_player_icons = 1;
        return;
    }
    if (p.mission_state != 0) {
        p.newly_completed = 0;
        p.original_score = *p.score;
        p.elapsed_time = MissionSys->timer.time_elapsed;
        p.reward_score = p.original_score;
        i32 gold = 0;
        if (p.mission_state == 2) {
            const i32 mission = static_cast<i8>(MissionSys->mission->count);
            if (Game_MissionSave == NULL || reinterpret_cast<u8 *>(Game_MissionSave)[0x50 + mission] == 0) {
                if (Game_MissionSave != NULL) reinterpret_cast<u8 *>(Game_MissionSave)[0x50 + mission] = 1;
                AddToCompletionPoints(POINTS_PER_MISSION);
                gold = AddGoldBrickMessage(&p, tMISSIONCOMPLETE);
                sprintf(event, "bounty_mission_%i_complete", mission + 1);
                NuIOS_RecordFlurryEvent(event);
            }
            u64 reward = static_cast<i64>(static_cast<i32>(static_cast<u16>(p.mission->time) - p.elapsed_time) * 150);
            if (reward < 150) reward = 150;
            IncreaseScore(&p.reward_score, reward, 0);
            if (Game_MissionSave != NULL) {
                f32 &best = reinterpret_cast<f32 *>(Game_MissionSave)[mission];
                if (best == 0.0f || p.elapsed_time < best) best = p.elapsed_time;
            }
        }
        AddStatusStage(&p, 26, gold);
        if (p.previous_gold_bricks < Game.field_0x7c26[0]) AddStatusStage(&p, 19, 0);
        AddStatusStage(&p, 9, 0);
        AddStatusStage(&p, 10, 0);
        goto challenge_finish;
    }
    if (p.challenge_state != 0) {
        p.elapsed_time = ChallengeTimer.time_elapsed;
        p.original_score = *p.score;
        p.reward_score = p.original_score;
        if (p.challenge_state == 2) {
            i32 gold = 0;
            if (Game.area_save[p.area_id].field_0x5[2] == 0) {
                AddToCompletionPoints(POINTS_PER_CHALLENGE);
                if (GOLDBRICKFORCHALLENGE != 0) gold = AddGoldBrickMessage(&p, tCHALLENGECOMPLETE);
                Game.area_save[p.area_id].field_0x5[2] = 1;
                if (p.episode_id != -1 && p.chapter != -1) {
                    sprintf(event, "challenge_ep%i_ch%i_complete", p.episode_id + 1, p.chapter + 1);
                    NuIOS_RecordFlurryEvent(event);
                }
            }
            u64 reward = static_cast<i64>(static_cast<i32>(ADataList[p.area_id].challenge_trial_time - p.elapsed_time) * 500);
            if (reward < 500) reward = 500;
            IncreaseScore(&p.reward_score, reward, 0);
            AddStatusStage(&p, 23, 0);
            if (p.challenge_state == 2 && gold != 0) AddStatusStage(&p, 25, 0);
        } else AddStatusStage(&p, 23, 0);
        if (p.previous_gold_bricks < Game.field_0x7c26[0]) AddStatusStage(&p, 19, 0);
        AddStatusStage(&p, 9, 0);
        AddStatusStage(&p, 10, 0);
challenge_finish:
        if (netclient == 0) AddStatusStage(&p, 11, 0);
        AddStatusStage(&p, 12, 0);
        p.current_gold_brick = -1;
        p.stage_types[p.stage_count] = -1;
        p.next_stage = FindStatusStage(p.stage_types[0]);
        NextStatusStage(&p);
        draw_player_icons = 1;
        cointotal_i_obj[0] = cointotal_i_obj[1] = 0xc3;
        return;
    }
    if (area != -1) {
        if (BuildUpDone != 0) p.field_0xb0 |= 4;
        p.minikit_count = Game.area_save[area].field_0x5[0] + AreaGlobals.values.field_0x10;
        p.minikit_max = (p.area->flags & 0x10) != 0 ? 10 : 0;
        if (p.minikit_count > p.minikit_max) p.minikit_count = p.minikit_max;
        p.true_hero_target = static_cast<u32>((p.field_0xb0 & 0x40) != 0 ? p.area->field38_0x90 : p.area->field37_0x8c);
    }
    p.area_time = AreaTimer.time_elapsed;
    const u64 total = static_cast<u64>(p.coins_remaining[0]) + p.coins_remaining[1];
    p.collected_score = total > 4000000000ULL ? 4000000000.0f : static_cast<f32>(static_cast<u32>(total));
    p.newly_completed = 0;
    if ((p.field_0xb0 & 4) != 0) p.true_hero_percent = 100.0f;
    else if (p.collected_score * 100.0f == 0.0f || p.true_hero_target == 0.0f) p.true_hero_percent = 0.0f;
    else {
        p.true_hero_percent = p.collected_score * 100.0f / p.true_hero_target;
        if (p.true_hero_percent > 99.0f) p.true_hero_percent = 99.0f;
    }
    p.coins_collected[0] = p.coins_remaining[0];
    p.coins_collected[1] = p.coins_remaining[1];
    draw_player_icons = 0;
    if (area != -1) {
        if ((p.field_0xb0 & 0x40) == 0 && p.episode != NULL && p.chapter < p.episode->regular_areas) {
            p.next_area = p.episode->area_ids[p.chapter + 1];
            if (p.chapter < p.episode->regular_areas - 1) p.field_0xb0 |= 8;
        }
        for (i32 i = 0; i < AreaGlobals.values.field_0x10; ++i) {
            if (Game.area_save[area].field_0x5[0] < 10) ++Game.area_save[area].field_0x5[0];
            u8 *save = Game.level_save + NewMiniPiece[i].level * 0x54;
            if (save[0x50] < 10) {
                NuStrCpy(reinterpret_cast<char *>(save + save[0x50] * 8), NewMiniPiece[i].name);
                ++save[0x50];
            }
        }
        i32 gold = 0;
        i32 completed_episode = 0;
        if ((p.field_0xb0 & 0x40) == 0 && Game.area_save[area].area_complete == 0 && (p.area->flags & 0x26) == 0) {
            Game.area_save[area].area_complete = 1;
            p.newly_completed = 1;
            AddToCompletionPoints(POINTS_PER_STORY);
            if ((p.mode_flags & 1) == 0 && (p.area->flags & 0x800) == 0) gold = AddGoldBrickMessage(&p, tLEVELCOMPLETE);
            if (p.episode != NULL && Episode_IsComplete(p.episode, NULL) != 0) completed_episode = static_cast<i8>(p.area->episode_index);
            if (p.episode_id != -1 && p.chapter != -1) {
                sprintf(event, "story_ep%i_ch%i_complete", p.episode_id + 1, p.chapter + 1);
                NuIOS_RecordFlurryEvent(event);
            }
            if (p.area == NEGOTIATIONS_ADATA) {
                Game.area_save[EDataList[1].area_ids[0]].complete = 1;
                Game.area_save[EDataList[2].area_ids[0]].complete = 1;
                Game.area_save[EDataList[3].area_ids[0]].complete = 1;
                Game.area_save[EDataList[4].area_ids[0]].complete = 1;
                Game.area_save[EDataList[5].area_ids[0]].complete = 1;
            }
            if ((p.field_0xb0 & 8) != 0) Game.area_save[p.next_area].complete = 1;
            else if (p.episode != NULL) {
                const i32 bonus = Episode_FindAreaFromFlags(p.episode, 5, 4);
                hub_startoutsidebonusdoor_area = Episode_FindAreaFromFlags(p.episode, 5, 5);
                if (bonus != -1) Game.area_save[bonus].complete = 1;
                if (hub_startoutsidebonusdoor_area != -1) Game.area_save[hub_startoutsidebonusdoor_area].complete = 1;
            }
        }
        if (p.newly_completed != 0) AddStatusStage(&p, 13, gold);
        if (completed_episode != 0) AddStatusStage(&p, completed_episode + 29, 0);
    }
    if (p.area != NULL && static_cast<i8>(p.area->cheat) != -1 && Game.area_save[area].field_0x5[1] == 0 && AreaGlobals.values.field_0x08 != 0) {
        AddToCompletionPoints(POINTS_PER_REDBRICK);
        Game.area_save[area].field_0x5[1] = 1;
        AddStatusStage(&p, 18, 0);
        if (p.episode_id != -1 && p.chapter != -1) {
            sprintf(event, "redbrick_ep%i_ch%i_awarded", p.episode_id + 1, p.chapter + 1);
            NuIOS_RecordFlurryEvent(event);
        }
    }
    if (p.newly_completed != 0 && newCharactersCollected(&p) > 0) {
        AddStatusStage(&p, 35, 0);
        AddStatusStage(&p, 36, 0);
        RememberPlayerIDs(1, static_cast<i16>(p.player0_model), static_cast<i16>(p.player1_model));
    }
    if (area != -1 && ((p.area->flags & 0x136) == 0x10 || (p.area->flags & 0x4000) != 0)) {
        AREASAVE_s &save = Game.area_save[area];
        if (BOTHTRUEJEDIGOLDBRICKS != 0) {
            u8 &complete = (p.field_0xb0 & 0x40) != 0 ? save.freeplay_buildup_complete : save.story_buildup_complete;
            if (complete == 0) {
                if ((p.field_0xb0 & 4) != 0) {
                    complete = 1;
                    AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                    AddStatusStage(&p, 1, AddGoldBrickMessage(&p, tTRUEHERO));
                } else if (p.collected_score > 0.0f) AddStatusStage(&p, 2, 0);
            }
        } else if (save.story_buildup_complete == 0 && save.freeplay_buildup_complete == 0) {
            if ((p.field_0xb0 & 4) != 0) {
                save.story_buildup_complete = save.freeplay_buildup_complete = 1;
                AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                AddStatusStage(&p, 1, AddGoldBrickMessage(&p, tTRUEHERO));
                if (p.episode_id == -1 || p.chapter == -1) sprintf(event, "truejedi_%s_awarded", p.area->file);
                else sprintf(event, "truejedi_ep%i_ch%i_awarded", p.episode_id + 1, p.chapter + 1);
                NuIOS_RecordFlurryEvent(event);
            } else if (p.collected_score > 0.0f && (p.mode_flags & 8) == 0) AddStatusStage(&p, 2, 0);
        }
    }
    if (p.collected_score > 0.0f) AddStatusStage(&p, 3, 0);
    if (p.minikit_max != 0 && (p.area->flags & 0x36) == 0x10 && Game.area_save[area].minikit_count == 0 && AreaGlobals.values.field_0x10 > 0) {
        p.new_minikits = AreaGlobals.values.field_0x10;
        i32 gold = 0;
        if (p.minikit_max <= Game.area_save[area].field_0x5[0]) {
            Game.area_save[area].minikit_count = 1;
            p.field_0xb0 |= 0x10;
            AddToCompletionPoints(POINTS_PER_MINIKIT);
            gold = AddGoldBrickMessage(&p, tMINIKIT);
        }
        if (from_save_and_exit == 0 || p.new_minikits != 0) AddStatusStage(&p, 4, gold);
        if ((p.field_0xb0 & 0x10) != 0 && AllMiniKitsDone(Game.area_save) != 0) {
            AddToCollection(id_SLAVE1);
            AddStatusStage(&p, 17, 0);
        }
    }
    if (p.previous_gold_bricks < Game.field_0x7c26[0]) AddStatusStage(&p, 19, 0);
    if ((p.mode_flags & 8) == 0) AddStatusStage(&p, 9, 0);
    AddStatusStage(&p, 10, 0);
    if (netclient == 0) AddStatusStage(&p, 11, 0);
    AddStatusStage(&p, 12, 0);
    p.current_gold_brick = -1;
    p.stage_types[p.stage_count] = -1;
    p.next_stage = FindStatusStage(p.stage_types[0]);
    NextStatusStage(&p);
    collect_suit_bits = Game.initial_store_pack_flags;
    Game.initial_store_pack_flags = areaSuitBits;
}

void SetDrawGoldBrick(STATUSPACKET_s *packet, i32) {
    const i32 stage = packet->current_gold_brick;
    if (packet->gold_brick_enabled[stage] != 0) {
        DrawGoldBrick_Stage = stage;
        DrawGoldBrick_Phase = packet->stage->field_0x14;
    }
}

void NewStatusRumbleBuzz(i32 player, float amount, float buzz, i32 priority) {
    if (!(amount > 0.0f)) {
        if (StatusPacket.player0_active != 0 && (player == 0 || player == -1)) {
            goto player0_buzz;
        }
        if (StatusPacket.player1_active != 0 && (player == 1 || player == -1)) {
            goto player1_buzz;
        }
        return;
    }

    if (StatusPacket.player0_active != 0 && (player == 0 || player == -1)) {
        if (StatusPacket.player0_rumble_time <= 0.0f || amount > StatusPacket.player0_rumble_time /
                                                                     StatusPacket.player0_rumble_duration *
                                                                     StatusPacket.player0_rumble_amount) {
            StatusPacket.player0_rumble_amount = amount;
            StatusPacket.player0_rumble_duration = amount;
            StatusPacket.player0_rumble_time = amount;
        }
    player0_buzz:
        if (buzz > StatusPacket.player0_buzz_amount) {
            StatusPacket.player0_buzz_amount = buzz;
        }
        if (priority > 0) {
            const i32 old_priority = StatusPacket.player0_rumble_priority;
            ++priority;
            if (priority > old_priority) {
                StatusPacket.player0_rumble_priority = static_cast<u8>(priority);
            }
        }
    }

    if (StatusPacket.player1_active != 0 && (player == 1 || player == -1)) {
        if (StatusPacket.player1_rumble_time <= 0.0f || amount > StatusPacket.player1_rumble_time /
                                                                     StatusPacket.player1_rumble_duration *
                                                                     StatusPacket.player1_rumble_amount) {
            StatusPacket.player1_rumble_amount = amount;
            StatusPacket.player1_rumble_duration = amount;
            StatusPacket.player1_rumble_time = amount;
        }
    player1_buzz:
        if (buzz > StatusPacket.player1_buzz_amount) {
            StatusPacket.player1_buzz_amount = buzz;
        }
        if (priority > 0) {
            const i32 old_priority = StatusPacket.player1_rumble_priority;
            ++priority;
            if (priority > old_priority) {
                StatusPacket.player1_rumble_priority = static_cast<u8>(priority);
            }
        }
    }
}

f32 StatusIconsOnOff(f32 progress) {
    return NuTrigTable[(static_cast<i32>(progress * 16384.0f) >> 1) & 0x7fff] * (STATSPOSY - STATSPOS2Y) + STATSPOS2Y;
}

void UpdateIconWibble() {
}

void Prompt_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        if (stage->field_0x18 == 0.0f) {
            NewMenu(0x400, 0, -1);
        }
        stage->field_0x18 += elapsed;
        const i32 menu_level = GameMenuLevel;
        if (stage->field_0x18 >= 0.5f) {
            for (i32 player = 0; player < 2; ++player) {
                if ((player == 0 ? packet->player0_active : packet->player1_active) == 0) {
                    continue;
                }
                bool selected = false;
                if (menu_level > 0 && GameMenu[menu_level].queued_item != -1) {
                    status_prompt = GameMenu[menu_level].queued_item;
                    GameMenu[menu_level].queued_item = -1;
                    selected = true;
                }
                const u32 buttons = GamePad[player].buttons_pressed;
                if ((buttons & GAMEPAD_MENUSELECT) != 0) {
                    selected = true;
                }
                bool moved = false;
                if ((buttons & GAMEPAD_MENUSELECT) == 0 &&
                    (((packet->field_0xb0 & 0x48) == 8 &&
                      (packet->next_area == -1 || (packet->area->flags & 2) != 0 ||
                       Game.area_save[packet->next_area].complete != 0)) ||
                     (packet->mode_flags & 1) != 0 || packet->challenge_state != 0 || packet->mission_state != 0)) {
                    const u32 navigation = buttons | GamePad[player].left_directions;
                    if (status_prompt < 1) {
                        if ((navigation & GAMEPAD_DDOWN) != 0) {
                            GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                            ++status_prompt;
                            moved = true;
                        }
                    } else if ((navigation & GAMEPAD_DUP) != 0) {
                        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                        --status_prompt;
                        moved = true;
                    }
                }
                if (selected) {
                    packet->prompt_choice =
                        (packet->field_0xb0 & 8) != 0 && packet->next_area != -1 &&
                        (packet->area->flags & 2) == 0 && Game.area_save[packet->next_area].complete == 0
                            ? 1 : static_cast<u8>(status_prompt);
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                    stage->field_0x14 = 1;
                    stage->field_0x18 = 0.0f;
                    stage->field_0x1c = 0.5f;
                    return;
                }
                if (moved) {
                    return;
                }
            }
        }
    } else if (stage->field_0x14 == 1) {
        if (stage->field_0x18 == 0.0f) {
            BackupMenu();
        }
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 > stage->field_0x1c) {
            NextStatusStage(packet);
        }
    }
}

extern i32 STATUS_R, STATUS_G, STATUS_B;
extern i16 tUNLOCKED;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
void RedBrick_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active == 0 || stage->field_0x14 <= 0) return;
    f32 blend = 1.0f;
    if (stage->field_0x18 < 1.0f) blend = NuTrigTable[(static_cast<i32>(stage->field_0x18 * 16384.0f) >> 1) & 0x7fff];
    else if (stage->field_0x18 >= stage->field_0x1c - 1.0f) blend = NuTrigTable[(static_cast<i32>((1.0f - (stage->field_0x18 - (stage->field_0x1c - 1.0f))) * 16384.0f) >> 1) & 0x7fff];
    SmartTextEx(TTab[tUNLOCKED], 0.0f, 0.05f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G, STATUS_B, 1.7f, 1, NULL, 0, static_cast<i32>(blend * 128.0f));
    const u32 angle = NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f;
    DrawPanel3DObjectNoAlpha(0.0f, blend - 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, static_cast<i32>(NuTrigTable[angle & 0x7fff] * 1820.0f), angle, 0, reinterpret_cast<nuhspecial_s *>(&WORLD->lev_objs[0xd2]), 2);
    SmartTextEx(TTab[*Cheat[static_cast<i8>(packet->area->cheat)].text_id], 0.0f, 1.5f - blend, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255, 1.7f, 1, NULL, 0, 128);
}

void RedBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}

void StatusPacketReset(STATUSPACKET_s *packet) {
    const STATUSPACKET_LSW_s *lsw_packet = packet->lsw_packet;
    auto init_callback = packet->init_callback;
    auto finish_callback = packet->finish_callback;
    void (*reset_callback)(STATUSPACKET_s *) = packet->reset_callback;
    void (*draw_background_callback)(STATUSPACKET_s *) = packet->draw_background_callback;
    const f32 field_0x68 = packet->field_0x68;

    reset_callback(packet);
    memset(packet, 0, sizeof(*packet));

    packet->reset_callback = reset_callback;
    packet->lsw_packet = const_cast<STATUSPACKET_LSW_s *>(lsw_packet);
    packet->init_callback = init_callback;
    packet->finish_callback = finish_callback;
    packet->draw_background_callback = draw_background_callback;
    packet->field_0x68 = field_0x68;
}

void StatusStage_Reset(STATUS_STAGE_s *stage) {
    if (stage != NULL) {
        stage->field_0x18 = 0;
        stage->field_0x1c = 1.0f;
        stage->field_0x14 = -1;
        stage->field_0x12 = 0;
    }
}

void DrawBuildUpBar(f32, f32, i32, i32, f32, f32, f32, u16);
extern i16 tTRUEJEDI, tSTORY, tFREEPLAY;
extern "C" void NuStrCat(char *, const char *);

void TrueHero_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active == 0) return;
    char text[252];
    f32 alpha = 1.0f;
    const f32 time = stage->field_0x18;
    switch (stage->field_0x14) {
        case 0: alpha = 0.0f; break;
        case 1: {
            alpha = time;
            const f32 ratio = stage->field_0x1c != 0.0f && time != 0.0f ? time / stage->field_0x1c : 0.0f;
            const f32 blend = 1.0f - (NuTrigTable[(static_cast<i32>(ratio * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
            DrawBuildUpBar(0.0f, (0.7f - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(blend * 16384.0f) >> 1) & 0x7fff] + STATSPOS2Y,
                           0, 100, 1.0f, blend * 0.75f + 1.0f, 1.0f, 0);
            break;
        }
        case 2: {
            const f32 blend = 1.0f - (NuTrigTable[(static_cast<i32>((time <= 1.0f ? time : 1.0f) * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
            DrawBuildUpBar(0.0f, 0.7f, packet->true_hero_percent, 100, 1.0f, 1.75f, 1.0f, 0);
            sprintf(text, "%i%%", static_cast<i32>(packet->true_hero_percent));
            Text3DEx(text, 0.0f, 0.3f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 191, 0, static_cast<i32>(blend * 128.0f) & 255);
            break;
        }
        case 3: {
            const f32 blend = 1.0f - (NuTrigTable[time < 1.0f ? ((static_cast<i32>(time * 32768.0f + 16384.0f) >> 1) & 0x7fff) : 0x6000] + 1.0f) * 0.5f;
            if (blend < 1.0f) {
                const i32 opacity = static_cast<i32>((1.0f - blend) * 64.0f) & 255;
                f32 scale = (blend * 0.5f + 1.0f) * 0.8f;
                Text3DEx(const_cast<char *>("100%"), 0.0f, 0.3f, 1.0f, scale, scale, scale, 0, 255, 191, 0, opacity);
                scale = (1.0f - blend * 0.5f) * 0.8f;
                Text3DEx(const_cast<char *>("100%"), 0.0f, 0.3f, 1.0f, scale, scale, scale, 0, 255, 191, 0, opacity);
            }
            DrawBuildUpBar(0.0f, 0.7f, packet->true_hero_percent, 100, 1.0f, 1.75f, 1.0f, 0);
            break;
        }
        case 6: {
            alpha = 1.0f - time;
            if (stage->type == 2 && alpha > 0.0f) {
                sprintf(text, "%i%%", static_cast<i32>(packet->true_hero_percent));
                Text3DEx(text, 0.0f, 0.3f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 191, 0, static_cast<i32>(alpha * 128.0f) & 255);
            }
            const f32 ratio = stage->field_0x18 < 1.0f ? 1.0f - stage->field_0x18 : 0.0f;
            const f32 blend = 1.0f - (NuTrigTable[(static_cast<i32>(ratio * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
            DrawBuildUpBar(0.0f, (0.7f - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(blend * 16384.0f) >> 1) & 0x7fff] + STATSPOS2Y,
                           packet->true_hero_percent, 100, 1.0f, blend * 0.75f + 1.0f, 1.0f, 0);
            break;
        }
    }
    if (alpha < 0.0f) alpha = 0.0f;
    else if (alpha > 1.0f) alpha = 1.0f;
    NuStrCpy(text, TTab[tTRUEJEDI]);
    if (BOTHTRUEJEDIGOLDBRICKS != 0) {
        NuStrCat(text, " ");
        NuStrCat(text, TTab[(packet->field_0xb0 & 0x40) != 0 ? tFREEPLAY : tSTORY]);
    }
    Text3DEx(text, 0.0f, STATUS_TITLE_Y, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 255, 255, static_cast<i32>(alpha * 128.0f) & 255);
}

void TrueHero_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}

i32 UpdateAchievements(STATUSPACKET_s *) {
    return 0;
}

void UpdateStatusScreen(WORLDINFO_s *) {
    const f32 elapsed = FRAMETIME;
    CoinTotalScale = SeekLinearF(CoinTotalScale, 1.0f, FRAMETIME * 3.0f);
    DrawGoldBrick_Phase = -1;
    DrawGoldBrick_Stage = -1;
    if (GAMEDEMO != 0) {
        if (GAMEDEMO == 2) {
            if (GameTimer.time_elapsed >= 8.0f) {
                EndOfDemo(0);
            }
            return;
        }
        if (GAMEDEMO != 1 || FadeSys.fade != 0.0f || NewLData != NULL) {
            return;
        }
        for (i32 player = 0; player < 2; ++player) {
            if ((player == 0 ? StatusPacket.player0_active : StatusPacket.player1_active) == 0) {
                continue;
            }
            if ((GamePad[player].buttons_pressed & GAMEPAD_MENUSELECT) != 0) {
                GameAudio_PlaySfx(0x30, NULL, 0, 0);
                if (gamedemo_option == 0) {
                    new_level_from_menu = 1;
                    NewLData = HUB_LDATA;
                    GAMEDEMO = 2;
                } else if (gamedemo_option == 1) {
                    EndOfDemo(0);
                }
                return;
            }
            const u32 navigation = GamePad[player].buttons_pressed | GamePad[player].left_directions;
            if ((navigation & GAMEPAD_DUP) != 0) {
                if (gamedemo_option > 0) {
                    --gamedemo_option;
                    GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                }
                return;
            }
            if ((navigation & GAMEPAD_DDOWN) != 0) {
                if (gamedemo_option < 1) {
                    ++gamedemo_option;
                    GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                }
                return;
            }
        }
        return;
    }
    if (StatusPacket.status_flags == 0) {
        return;
    }
    if ((StatusPacket.field_0xb0 & 2) == 0) {
        bool skip = MechInputTouchMenuController::AnyTouchesThisFrame > 0;
        if (skip) {
            MechInputTouchMenuController::AnyTouchesThisFrame = 0;
        } else {
            const u32 button = Text_Language == 0 ? GAMEPAD_MENUSELECT : GAMEPAD_JUMP;
            skip = (StatusPacket.player0_active != 0 && (GamePad[0].buttons_pressed & button) != 0) ||
                   (StatusPacket.player1_active != 0 && (GamePad[1].buttons_pressed & button) != 0);
        }
        if (skip) {
            if (StatusPacket.stage == NULL) {
                return;
            }
            if (StatusPacket.stage->skip_callback != NULL) {
                StatusPacket.stage->skip_callback(StatusPacket.stage, &StatusPacket);
            }
        }
    }
    if (StatusPacket.stage != NULL && StatusPacket.stage->update_callback != NULL) {
        StatusPacket.stage->update_callback(StatusPacket.stage, &StatusPacket, elapsed);
    }
    if (StatusPacket.status_flags == 0 && GameTimer.time_elapsed >= 5.0f) {
        FinishStatusPacket(0);
    }
}

void RedBrick_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x18 = 0.0f;
        stage->field_0x1c = 5.0f;
        stage->field_0x14 = 1;
    } else if (stage->field_0x14 == 1) {
        const f32 previous = stage->field_0x18;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) NextStatusStage(packet);
        else if (previous < 0.75f && stage->field_0x18 >= 0.75f) {
            PlaySfx(const_cast<char *>("TrueJedi_100pc"), NULL);
            NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
        }
    }
}

f32 nextsoundpercent;
void TrueHero_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    static f32 spdelay;
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 1.0f;
            break;
        case 1:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                stage->field_0x14 = 2;
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 3.0f;
                nextsoundpercent = 5.0f;
                packet->true_hero_percent = 0.0f;
            }
            break;
        case 2: {
            SetDrawGoldBrick(packet, packet->current_gold_brick);
            f32 percent = 100.0f;
            if ((packet->field_0xb0 & 4) == 0) {
                const f32 ratio = packet->collected_score * 100.0f / packet->true_hero_target;
                percent = ratio <= 99.0f ? ratio : 99.0f;
            }
            stage->field_0x18 += elapsed;
            packet->true_hero_percent += (100.0f / stage->field_0x1c) * elapsed;
            if (packet->true_hero_percent > percent) packet->true_hero_percent = percent;
            else if (packet->true_hero_percent >= nextsoundpercent) {
                PlaySfx(const_cast<char *>("Status_GoldBarDec"), NULL);
                nextsoundpercent += 10.0f;
            }
            if (stage->field_0x18 > stage->field_0x1c) {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 1.0f;
                if (stage->type == 2) {
                    stage->field_0x14 = 6;
                    PlaySfx(const_cast<char *>("TrueJedi_NOT"), NULL);
                } else {
                    stage->field_0x14 = 3;
                    PlaySfx(const_cast<char *>("TrueJedi_100pc"), NULL);
                }
            }
            break;
        }
        case 3:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 1.0f;
                stage->field_0x14 = 6;
                spdelay = 0.0f;
            }
            break;
        case 6:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) NextStatusStage(packet);
            break;
    }
}

i32 InitStatusScreen_LSW(WORLDINFO_s *, STATUSPACKET_s *) {
    return 0;
}

void RegisterStatusScreen(STATUS_STAGE_s *stages, i32 *, REGISTERSTATUSPACKET_s *registration) {
    StatusStages = stages;
    StatusPacket.init_callback = registration->init_callback;
    StatusPacket.finish_callback = registration->finish_callback;
    StatusPacket.reset_callback = registration->reset_callback;
    StatusPacket.draw_background_callback = registration->draw_background_callback;
    StatusPacket.lsw_packet = registration->lsw_packet;
    StatusPacket.field_0x68 = registration->stage_delay;
}

void ResetStatusPacket_LSW(STATUSPACKET_s *packet) {
    packet->lsw_packet->field_0x00 = 0;
}

extern i16 tMAP, tRESTART, tFINISHSTORY, tCONTINUESTORY, tFINALCHAPTERLOCKED, tNEXTCHAPTERLOCKED;
extern f32 text3d_width, text3d_height;
extern "C" {
    extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
    extern u8 MENUNORMALR, MENUNORMALG, MENUNORMALB;
    extern u8 MENUFLASH0R, MENUFLASH0G, MENUFLASH0B;
    extern u8 MENUFLASH1R, MENUFLASH1G, MENUFLASH1B;
}
void Hint_SetHintFromId(i32, i32, i32);
void Hint_SetComplete(i32);
void Hint_Draw(i32);

void Status_DrawPromptMenu(STATUSPACKET_s *packet, i32 selected, float alpha) {
    i32 labels[2];
    labels[1] = -1;
    MENU_s *menu = &GameMenu[GameMenuLevel];
    bool locked = false;
    if ((packet->mode_flags & 1) != 0 || packet->challenge_state != 0 || packet->mission_state != 0) {
        labels[0] = tRESTART;
        labels[1] = tMAP;
    } else if ((packet->field_0xb0 & 0x40) != 0 ||
               (packet->area_id != -1 && ADataList[packet->area_id].episode_index == 0xff)) {
        labels[0] = tMAP;
    } else {
        labels[0] = tFINISHSTORY;
        if ((packet->field_0xb0 & 8) != 0) {
            if (packet->next_area != -1 && (ADataList[packet->next_area].flags & 2) == 0 &&
                Game.area_save[packet->next_area].complete == 0) {
                labels[0] = tMAP;
                const i16 text = ADataList[packet->next_area].area_index == 5 ? tFINALCHAPTERLOCKED : tNEXTCHAPTERLOCKED;
                SmartTextEx(TTab[text], 0.0f, 0.325f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 63, 0, 1.7f, 1,
                            NULL, 0, static_cast<i32>(alpha * 128.0f));
                locked = true;
            } else {
                if (packet->next_area == -1 || (ADataList[packet->next_area].flags & 2) == 0) {
                    labels[0] = tCONTINUESTORY;
                }
                labels[1] = tMAP;
            }
        } else if (packet->next_area == -1 || (ADataList[packet->next_area].flags & 2) == 0) {
            labels[0] = tCONTINUESTORY;
        }
    }
    const i32 count = labels[1] == -1 ? 1 : 2;
    f32 y = count == 1 ? 0.125f : 0.1f;
    if (!locked && count == 2 && packet->newly_completed != 0 && packet->area != NULL &&
        packet->area == NEGOTIATIONS_ADATA) {
        Hint_SetHintFromId(0x26c, 0, 1);
        Hint_SetComplete(0x26c);
        Hint_Draw(-1);
        y = 0.48999998f;
    }
    if ((packet->mode_flags & 1) != 0) {
        y += 0.075f;
    }
    for (i32 i = 0; i < count; ++i) {
        u32 red, green, blue;
        if (selected != 0 && status_prompt == i && TestForController()) {
            if (menu_pulsate > 0.0f) {
                const f32 inverse = 1.0f - menu_pulsate;
                red = MENUFLASH0R * menu_pulsate + MENUFLASH1R * inverse;
                green = MENUFLASH0G * menu_pulsate + MENUFLASH1G * inverse;
                blue = MENUFLASH0B * menu_pulsate + MENUFLASH1B * inverse;
            } else if (menu_flash == 0) {
                red = MENUFLASH1R; green = MENUFLASH1G; blue = MENUFLASH1B;
            } else {
                red = MENUFLASH0R; green = MENUFLASH0G; blue = MENUFLASH0B;
            }
        } else if (menu_pulse > 0.0f) {
            const f32 inverse = 1.0f - menu_pulse;
            red = MENUFLASH0R * menu_pulse + MENUNORMALR * inverse;
            green = MENUFLASH0G * menu_pulse + MENUNORMALG * inverse;
            blue = MENUFLASH0B * menu_pulse + MENUNORMALB * inverse;
        } else {
            red = MENUENTRYR; green = MENUENTRYG; blue = MENUENTRYB;
        }
        smarttextex_drawmessagebox = 1;
        SmartTextEx(TTab[labels[i]], 0.0f, y, 1.0f, 0.7f, 0.7f, 0.7f, 0, red, green, blue, 1.7f, 1,
                    NULL, 0, static_cast<i32>(alpha * 128.0f) & 0xff);
        menu->item_x[i] = 0.0f;
        menu->item_y[i] = y;
        menu->item_width[i] = text3d_width;
        menu->item_height[i] = text3d_height;
        menu->item_column[i] = 0;
        menu->item_row[i] = i;
        y -= 0.2f;
    }
}

f32 getFinishedStatusAlpha(STATUSPACKET_s *packet) {
    STATUS_STAGE_s *stage = packet->stage;
    f32 alpha = 1.0f;
    if (stage->type == 11 || stage->type == 12) {
        alpha = 0.0f;
    } else if (stage->type == 10) {
        alpha = stage->field_0x18 < 1.0f ? 1.0f - stage->field_0x18 : 0.0f;
    } else if (stage->type == 19 && stage->field_0x14 != 0) {
        if (stage->field_0x18 < 1.0f) {
            alpha = 1.0f - stage->field_0x18;
        } else {
            const f32 fade_start = stage->field_0x1c - 1.0f;
            alpha = stage->field_0x18 < fade_start
                        ? 0.0f
                        : (stage->field_0x18 - fade_start) / (stage->field_0x1c - fade_start);
        }
    }
    return alpha;
}

void SuperStoryTime_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32) {
}

void SuperStoryTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    *packet->score = packet->time_reward_score;
    NextStatusStage(packet);
}

void LSW_registerStatusScreen() {
    REGISTERSTATUSPACKET_s registration = {
        &StatusPacket_LSW, InitStatusScreen_LSW, FinishStatusPacket_LSW,
        ResetStatusPacket_LSW, DrawStatusBG_LSW, 0.5f,
    };
    RegisterStatusScreen(StatusStages_LSW, NULL, &registration);
}

void SuperStoryScore_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32) {
}

void SuperStoryScore_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    *packet->score = packet->final_reward_score;
    NextStatusStage(packet);
}

void SuperStoryTime_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x14 = 1;
        stage->field_0x18 = 0.0f;
        stage->field_0x1c = 5.0f;
        packet->original_score = *packet->score;
    } else if (stage->field_0x14 == 1) {
        const f32 previous = stage->field_0x18;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            *packet->score = packet->time_reward_score;
            NextStatusStage(packet);
        } else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
            if (packet->new_best_time == 0.0f) GameAudio_PlaySfx(0x32, NULL, 0, 0);
            else PlaySfx(const_cast<char *>("StatusAward"), NULL);
            NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
        }
        if (packet->new_best_time != 0.0f) {
            if (previous < 4.0f && stage->field_0x18 >= 4.0f) {
                CoinTotalScale = 1.5f;
                NewStatusRumbleBuzz(-1, 0.0f, 0.1f, 0);
                PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
            }
            if (stage->field_0x18 >= 4.5f) *packet->score = packet->time_reward_score;
            else if (stage->field_0x18 >= 0.5f && stage->field_0x18 < 4.0f) {
                const f32 blend = NuTrigTable[(static_cast<i32>(((stage->field_0x18 - 0.5f) / 3.5f) * 16384.0f) >> 1) & 0x7fff];
                const u32 difference = packet->time_reward_score - packet->original_score;
                *packet->score = packet->original_score;
                IncreaseScore(packet->score, static_cast<i64>(static_cast<i32>(static_cast<f32>(difference) * blend)), 0);
                if (static_cast<i32>(previous / 0.2f) != static_cast<i32>(stage->field_0x18 / 0.2f)) PlaySfx(const_cast<char *>("PickupCoin"), NULL);
            }
        }
    }
}

void SuperStoryScore_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x14 = 1;
        stage->field_0x18 = 0.0f;
        stage->field_0x1c = 5.0f;
        packet->original_score = *packet->score;
    } else if (stage->field_0x14 == 1) {
        const f32 previous = stage->field_0x18;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            *packet->score = packet->final_reward_score;
            NextStatusStage(packet);
        } else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
            if (packet->new_best_score == 0) GameAudio_PlaySfx(0x32, NULL, 0, 0);
            else PlaySfx(const_cast<char *>("StatusAward"), NULL);
            NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
        }
        if (packet->final_reward_score != packet->original_score) {
            if (previous < 4.0f && stage->field_0x18 >= 4.0f) {
                CoinTotalScale = 1.5f;
                NewStatusRumbleBuzz(-1, 0.0f, 0.1f, 0);
                PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
            }
            if (stage->field_0x18 >= 4.0f) *packet->score = packet->final_reward_score;
            else if (stage->field_0x18 >= 0.5f && stage->field_0x18 < 4.0f) {
                const f32 blend = 1.0f - NuTrigTable[(static_cast<i32>(((stage->field_0x18 - 0.5f) / 3.5f) * 16384.0f + 16384.0f) >> 1) & 0x7fff];
                const u32 difference = packet->final_reward_score - packet->original_score;
                *packet->score = packet->original_score;
                IncreaseScore(packet->score, static_cast<i64>(static_cast<i32>(static_cast<f32>(difference) * blend)), 0);
                if (static_cast<i32>(previous / 0.2f) != static_cast<i32>(stage->field_0x18 / 0.2f)) PlaySfx(const_cast<char *>("PickupCoin"), NULL);
            }
        }
    }
}

void Coins_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active == 0) {
        if (stage->field_0x12 != 0) {
            const f32 alpha = getFinishedStatusAlpha(packet);
            CoinTotal_Draw(*packet->score, StatusIconsOnOff(alpha), CoinTotalScale, 1, 1.0f, 255, 191, 0);
        }
        return;
    }
    f32 alpha = 1.0f;
    f32 coin_y = STATSPOS2Y;
    f32 total_y = STATSPOS2Y;
    f32 scale = 2.0f;
    switch (stage->field_0x14) {
    case 0:
        alpha = 0.0f;
        break;
    case 1: {
        alpha = stage->field_0x18;
        f32 ratio = stage->field_0x18 / stage->field_0x1c;
        const f32 blend = 1.0f - (NuTrigTable[(static_cast<i32>(ratio * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
        if (stage->field_0x1c == 0.0f || stage->field_0x18 == 0.0f) {
            ratio = 0.0f;
        }
        icon_y = StatusIconsOnOff(ratio);
        total_y = (0.3f - STATSPOS2Y) * blend + STATSPOS2Y;
        coin_y = (STATSPOSY - STATSPOS2Y) * blend + STATSPOS2Y;
        break;
    }
    case 2:
        total_y = 0.3f;
        coin_y = STATSPOSY;
        break;
    case 3: {
        alpha = 1.0f - stage->field_0x18;
        const f32 blend = 1.0f - (NuTrigTable[(static_cast<i32>((1.0f - stage->field_0x18 / stage->field_0x1c) * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
        total_y = (0.3f - STATSPOSY) * blend + STATSPOSY;
        scale = (2.0f - CoinTotalScale) * blend + CoinTotalScale;
        coin_y = STATSPOS2Y + (STATSPOSY - STATSPOS2Y) * blend;
        break;
    }
    }
    if (stage->field_0x14 >= 1) {
        const f32 player_alpha = packet->player0_active == 0 ? DROPINALPHA : 1.0f;
        const u32 coins = packet->coins_remaining[0];
        const i32 object_index = coins < 100 ? 0xb3 : coins < 1000 ? 0xbb : 0xc3;
        if (WORLD->lev_objs[object_index].active != 0) {
            DrawPanel3DObject(-PANEL_COINX, coin_y, 1.0f, PANEL_COINSCALE_END, PANEL_COINSCALE_END,
                              PANEL_COINSCALE_END, 0, 0, 0, &WORLD->lev_objs[object_index].special, 0, player_alpha);
        }
        char text[256];
        Text_MakeScore(packet->coins_remaining[0], text);
        Text3DEx(text, -PANEL_SCOREX, coin_y, 1.0f, PANEL_SCORESCALE, PANEL_SCORESCALE, PANEL_SCORESCALE,
                 2, 255, 191, 0, static_cast<u8>(player_alpha * 128.0f));
        CoinTotal_Draw(*packet->score, total_y, scale, 1, 1.0f, 255, 191, 0);
    }
    alpha = alpha < 0.0f ? 0.0f : alpha > 1.0f ? 1.0f : alpha;
    Text3DEx(TTab[tCOINTOTAL], 0.0f, STATUS_TITLE_Y, 1.0f, 0.5f, 0.5f, 0.5f, 0,
             255, 255, 255, static_cast<u8>(alpha * 128.0f));
}
void Coins_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    icon_y = StatusIconsOnOff(1.0f);
    IncreaseScore(packet->score, static_cast<u64>(packet->coins_remaining[0] + packet->coins_remaining[1]), 0);
    packet->coins_remaining[0] = 0;
    packet->coins_remaining[1] = 0;
    NextStatusStage(packet);
}
void Coins_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
    case 0:
        stage->field_0x14 = 1;
        stage->field_0x18 = 0.0f;
        stage->field_0x1c = 1.0f;
        draw_player_icons = 1;
        break;
    case 1:
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            stage->field_0x14 = 2;
        }
        break;
    case 2: {
        i32 finished = 0;
        for (i32 player = 0; player < 2; ++player) {
            u32 remaining = packet->coins_remaining[player];
            if (remaining != 0) {
                u32 rate = 35;
                for (u32 threshold = 100; threshold <= remaining; threshold *= 10) {
                    rate *= 10;
                }
                u32 amount = static_cast<u32>(static_cast<f32>(rate) * elapsed);
                if (amount > remaining) {
                    amount = remaining;
                } else if (amount == 0) {
                    amount = 1;
                }
                packet->coins_remaining[player] -= amount;
                IncreaseScore(packet->score, static_cast<u64>(amount), 0);
            }
            finished += packet->coins_remaining[player] == 0;
        }
        PlaySfx("PickupCoin", 0);
        if (finished == 2) {
            stage->field_0x14 = 3;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 1.0f;
            if (packet->coins_collected[0] + packet->coins_collected[1] == 0) {
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
            } else {
                PlaySfx("Shop_BuyCheat", 0);
            }
        }
        break;
    }
    case 3:
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            NextStatusStage(packet);
        }
        break;
    }
}
extern i16 tWINNER;
void DrawBonusScore(f32, i32, i32, f32, i32 *);
void DrawBonusTime(STATUSPACKET_s *, f32, i32);

void BonusWin_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (current == 0) {
        if (stage->field_0x12 != 0) {
            const f32 alpha = getFinishedStatusAlpha(packet);
            if (packet->stage->type != 25) {
                DrawBonusScore(STATSPOSY, StatusPacket.player0_active, StatusPacket.player1_active, alpha, BonusScore);
            }
        }
    } else if (stage->field_0x14 == 0) {
        iconalphaoverride = 0.0f;
    } else {
        const f32 time = stage->field_0x18;
        if (stage->field_0x1c - 0.1f <= time) {
            iconalphaoverride = 0.0f;
        } else if (time < stage->field_0x1c - 0.6f) {
            iconalphaoverride = time < 0.5f ? time + time : 1.0f;
        } else {
            const f32 fade = time - (stage->field_0x1c - 0.6f);
            iconalphaoverride = 1.0f - (fade + fade);
        }
        if (time < 2.0f && NuFmod(time, 0.2f) >= 0.1f) {
            const i32 winner = (packet->field_0xb0 & 0x20) != 0;
            SmartTextEx(TTab[tWINNER], winner ? 0.675f : -0.675f, STATSPOSY, 1.0f,
                        0.7f, 0.7f, 0.7f, winner ? 8 : 2, 0, 255, 0, 0.35f, 1, NULL, 0,
                        static_cast<i32>(iconalphaoverride * 128.0f));
        }
        DrawBonusScore(STATSPOSY, StatusPacket.player0_active, StatusPacket.player1_active, iconalphaoverride, BonusScore);
    }
}
void BonusWin_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}
void BonusTime_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (current == 0) {
        if (stage->field_0x12 != 0) {
            DrawBonusTime(packet, 1.0f, static_cast<i32>(getFinishedStatusAlpha(packet) * 128.0f));
        }
    } else if (stage->field_0x14 > 0) {
        f32 alpha = stage->field_0x18;
        f32 time;
        if (alpha >= 0.5f) {
            time = alpha < 3.0f ? 0.0f : alpha - 3.0f;
            alpha = 1.0f;
        } else {
            alpha += alpha;
            time = 0.0f;
        }
        DrawBonusTime(packet, 1.0f - (NuTrigTable[(static_cast<i32>(time * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f,
                      static_cast<i32>(alpha * 128.0f));
    }
}
void BonusTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}
void ChallangeCash_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32) {
}
void ChallangeCash_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}
void BonusWin_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x14 = 1; stage->field_0x18 = 0.0f; stage->field_0x1c = 4.0f;
        draw_player_icons = 1;
    } else if (stage->field_0x14 == 1) {
        const f32 previous = stage->field_0x18;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) NextStatusStage(packet);
        else if (previous < stage->field_0x1c * 0.5f && stage->field_0x18 >= stage->field_0x1c * 0.5f) {
            PlaySfx(const_cast<char *>("TrueJedi_100pc"), NULL);
            NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
            ++BonusScore[(packet->field_0xb0 >> 5) & 1];
        }
    }
}
void BonusTime_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x18 = 0.0f; stage->field_0x1c = 4.0f; stage->field_0x14 = 1;
    } else if (stage->field_0x14 == 1) {
        SetDrawGoldBrick(packet, packet->current_gold_brick);
        const f32 previous = stage->field_0x18;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) NextStatusStage(packet);
        else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
            if (packet->new_best_time == 0.0f) GameAudio_PlaySfx(0x32, NULL, 0, 0);
            else PlaySfx(const_cast<char *>("StatusAward"), NULL);
            NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
        }
    }
}
void ChallangeCash_Update(STATUS_STAGE_s *, STATUSPACKET_s *, float) {
}
void BonusComplete_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (current == 0) return;
    if (stage->field_0x14 > 0 && packet->newly_completed != 0) {
        const f32 time = stage->field_0x18;
        f32 alpha;
        if (time < 0.5f) alpha = time + time;
        else if (time >= 3.5f) alpha = 1.0f - ((time - 3.5f) + (time - 3.5f));
        else alpha = 1.0f;
        if (alpha > 0.0f) {
            SmartTextEx(TTab[tLEVELCOMPLETE], 0.0f, 0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0,
                        STATUS_R, STATUS_G, STATUS_B, 1.7f, 1, NULL, 0, static_cast<i32>(alpha * 128.0f));
        }
    }
    if (stage->field_0x14 == 0) {
        iconalphaoverride = 0.0f;
    } else if (packet->newly_completed == 0) {
        iconalphaoverride = stage->field_0x18 / stage->field_0x1c;
    } else if (stage->field_0x18 < stage->field_0x1c - 0.5f) {
        iconalphaoverride = 0.0f;
    } else {
        const f32 time = stage->field_0x18 - (stage->field_0x1c - 0.5f);
        iconalphaoverride = time + time;
    }
}
void BonusComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}
void BonusComplete_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x18 = 0.0f; stage->field_0x14 = 1;
        stage->field_0x1c = packet->newly_completed == 0 ? 0.5f : 4.0f;
        draw_player_icons = 1;
    } else if (stage->field_0x14 == 1) {
        SetDrawGoldBrick(packet, packet->current_gold_brick);
        const f32 previous = stage->field_0x18;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) NextStatusStage(packet);
        else if (packet->newly_completed != 0 && previous < 0.5f && stage->field_0x18 >= 0.5f) {
            PlaySfx(const_cast<char *>("StatusAward"), NULL);
            NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
        }
    }
}
